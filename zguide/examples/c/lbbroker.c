//  Load-balancing broker
//  Clients and workers are shown here in-process

#include "zhelpers.h"
#include <pthread.h>
#define NBR_CLIENTS 10
#define NBR_WORKERS 3

//  Dequeue operation for queue implemented as array of anything
#define DEQUEUE(q) memmove (&(q)[0], &(q)[1], sizeof (q) - sizeof (q [0]))

//  Basic request-reply client using REQ socket
//  Because s_send and s_recv can't handle 0MQ binary identities, we
//  set a printable text identity to allow routing.
//
static void *
client_task(void *args)
{
    void *context = zmq_ctx_new();
    void *client = zmq_socket(context, ZMQ_REQ);

#if (defined (WIN32))
    s_set_id(client, (intptr_t)args);
    zmq_connect(client, "tcp://localhost:5672"); // frontend
#else
    s_set_id(client); // Set a printable identity
    zmq_connect(client, "ipc://frontend.ipc");
#endif

    //  Send request, get reply
    s_send(client, "HELLO");
    char *reply = s_recv(client);
    printf("Client: %s\n", reply);
    free(reply);
    zmq_close(client);
    zmq_ctx_destroy(context);
    return NULL;
}

//  .split worker task
//  While this example runs in a single process, that is just to make
//  it easier to start and stop the example. Each thread has its own
//  context and conceptually acts as a separate process.
//  This is the worker task, using a REQ socket to do load-balancing.
//  Because s_send and s_recv can't handle 0MQ binary identities, we
//  set a printable text identity to allow routing.

static void *
worker_task(void *args)
{
    void *context = zmq_ctx_new();
    void *worker = zmq_socket(context, ZMQ_REQ);

#if (defined (WIN32))
    s_set_id(worker, (intptr_t)args);
    zmq_connect(worker, "tcp://localhost:5673"); // backend
#else
    s_set_id(worker);
    zmq_connect(worker, "ipc://backend.ipc");
#endif

    //  Tell broker we're ready for work
    s_send(worker, "READY");

    while (1) {
        //  Read and save all frames until we get an empty frame
        //  In this example there is only 1, but there could be more
        char *identity = s_recv(worker);
        char *empty = s_recv(worker);
        assert(*empty == 0);
        free(empty);

        //  Get request, send reply
        char *request = s_recv(worker);
        printf("Worker: %s\n", request);
        free(request);

        s_sendmore(worker, identity);
        s_sendmore(worker, "");
        s_send(worker, "OK");
        free(identity);
    }
    zmq_close(worker);
    zmq_ctx_destroy(context);
    return NULL;
}

//  .split main task
//  This is the main task. It starts the clients and workers, and then
//  routes requests between the two layers. Workers signal READY when
//  they start; after that we treat them as ready when they reply with
//  a response back to a client. The load-balancing data structure is 
//  just a queue of next available workers.

int main(void)
{
    //  Prepare our context and sockets
    void *context = zmq_ctx_new();
    void *frontend = zmq_socket(context, ZMQ_ROUTER);
    void *backend = zmq_socket(context, ZMQ_ROUTER);

#if (defined (WIN32))
    zmq_bind(frontend, "tcp://*:5672"); // frontend
    zmq_bind(backend, "tcp://*:5673"); // backend
#else
    zmq_bind(frontend, "ipc://frontend.ipc");
    zmq_bind(backend, "ipc://backend.ipc");
#endif

    int client_nbr;
    for (client_nbr = 0; client_nbr < NBR_CLIENTS; client_nbr++) {
        pthread_t client;
        pthread_create(&client, NULL, client_task, (void *)(intptr_t)client_nbr);
    }
    int worker_nbr;
    for (worker_nbr = 0; worker_nbr < NBR_WORKERS; worker_nbr++) {
        pthread_t worker;
        pthread_create(&worker, NULL, worker_task, (void *)(intptr_t)worker_nbr);
    }
    //  .split main task body
    //  Here is the main loop for the least-recently-used queue. It has two
    //  sockets; a frontend for clients and a backend for workers. It polls
    //  the backend in all cases, and polls the frontend only when there are
    //  one or more workers ready. This is a neat way to use 0MQ's own queues
    //  to hold messages we're not ready to process yet. When we get a client
    //  request, we pop the next available worker and send the request to it,
    //  including the originating client identity. When a worker replies, we
    //  requeue that worker and forward the reply to the original client
    //  using the reply envelope.

    //  Queue of available workers
    int available_workers = 0;
    char *worker_queue[10];

    while (1) {
        zmq_pollitem_t items[] = {
                { backend, 0, ZMQ_POLLIN, 0 },
                { frontend, 0, ZMQ_POLLIN, 0 }
        };
        //  Poll frontend only if we have available workers
        int rc = zmq_poll(items, available_workers ? 2 : 1, -1);
        if (rc == -1)
            break;              //  Interrupted

        //  Handle worker activity on backend
        if (items[0].revents & ZMQ_POLLIN) {
            //  Queue worker identity for load-balancing
            char *worker_id = s_recv(backend);
            assert(available_workers < NBR_WORKERS);
            worker_queue[available_workers++] = worker_id;

            //  Second frame is empty
            char *empty = s_recv(backend);
            assert(empty[0] == 0);
            free(empty);

            //  Third frame is READY or else a client reply identity
            char *client_id = s_recv(backend);

            //  If client reply, send rest back to frontend
            if (strcmp(client_id, "READY") != 0) {
                empty = s_recv(backend);
                assert(empty[0] == 0);
                free(empty);
                char *reply = s_recv(backend);
                s_sendmore(frontend, client_id);
                s_sendmore(frontend, "");
                s_send(frontend, reply);
                free(reply);
                if (--client_nbr == 0)
                    break;      //  Exit after N messages
            }
            free(client_id);
        }
        //  .split handling a client request
        //  Here is how we handle a client request:

        if (items[1].revents & ZMQ_POLLIN) {
            //  Now get next client request, route to last-used worker
            //  Client request is [identity][empty][request]
            char *client_id = s_recv(frontend);
            char *empty = s_recv(frontend);
            assert(empty[0] == 0);
            free(empty);
            char *request = s_recv(frontend);

            s_sendmore(backend, worker_queue[0]);
            s_sendmore(backend, "");
            s_sendmore(backend, client_id);
            s_sendmore(backend, "");
            s_send(backend, request);

            free(client_id);
            free(request);

            //  Dequeue and drop the next worker identity
            free(worker_queue[0]);
            DEQUEUE(worker_queue);
            available_workers--;
        }
    }
    zmq_close(frontend);
    zmq_close(backend);
    zmq_ctx_destroy(context);
    return 0;
}