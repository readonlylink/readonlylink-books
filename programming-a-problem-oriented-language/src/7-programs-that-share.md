---
title: Programs that share
---

It is not obvious, but a program orgainzed as we have discussed is
ideally suited to handling several users simultaneously. All of the
basic problems of interactive processing have been solved by interacting
 with one user. The organization is such that all data is, or can be,
stored in the user's dictionary. To distinguish users merely requires
the program recognise the proper dictionary.

Of course the value of multiple users depends upon the application.
There appears to be a correlation between the complexity of an
application and the number of potential users. An application that
deserves a problem-oriented-langage my well be of interest to many users
 on a continuous basis.

Moreover, once the basic program is available, it is relatively
simple to add other, even unrelated, applications. The ability to
control your vocabulary by reading screens allows a terminal to be used
by different people with absolute minimum effort: each can have a
personal screen that will load his dictionary with the vocabulary he
wants.

Providing the message traffic from any one terminal is low
enough, as is inevitably the case - for we have in effect slowed the
computer down to human speed - we can handle a much larger number of
terminals than can fit in core, hundreds, by storing inactive users on
disk.

However there is a cost, primarily of assuring that re-entrant
programming rules are strictly followed. The additional code required to
 switch the computer's attention among users and the additional core
required for disk buffers and user dictionaries demand that a single
user application by de-bugged first. And then the capacity of the
computer multiplied with a multiple-user control routine as the demand
develops. It is all too easy to get bogged down in the multiple-user
controller and never to perfect an application. Or to perfect a
multiple-user control and never to find a demand to justify it.

Given a successful single-user application, I will show how it
can be expanded to many users. If you plan to take this step, there are
certain precautions you should take with your original implementation.
But mind the Basic Principle!

### 7.0.1 Non-user activities

Each user has a position in the ready table to identify his status. The
computer examines this table to decide what to do next. You may want to
add to the ready table entries not associated with users, but
representing tasks that must be performed by the computer.

For example, if you have to poll phone lines to acquire input, you
want to perform these polls asynchronously with whatever other work
you're doing. Since interrupt routines are best kept small, the task of
translating character sets, checking parity, distributing messages, ets.
 should be performed at lower priority. This is easy to do with an entry
 in the ready table. The interrupt routine sets a message routine
"ready" and the computer will process it when possible.

Each such independent activity should have a ready table entry
and a (perhaps) small dictionary in which to store its parameters;
return address, register contents, etc. in the same format as a user
activity. In fact these activities are competely equivalent to users,
except that they don't process users. This is significant, for it means
they never generate error messages, they must handle their own errors,
somehow.

If you haven't already noticed, we're now talking about operating
 systems. I don't have much more to say on the subject, but there are
other asynchronous activities you might want:
   - A clock to handle the timer interrupt and maintain a time and
date in core and disk. It might ready other activities that relinquished
 control for a fixed time.
   - A routine to write blocks on disk. Periodically it might
scan the block buffers for blocks to copy. (however, writing blocks when
 the read routine needs a buffer seems simpler.)
Such activities cost little, and usually provide the simplest answer to
any asynchronous problem. Mind the Basic Principle, though!

### 7.0.2 Message handling

If you can read input from one user, you can read input from many. You
must get an interrupt that tells you input is available and from whom it
 comes. You simple direct it to the proper message buffer. Likewise with
 output.

It needn't be simple, but it certainly depends on hardware
exclusively. If you have to poll terminals, it can become very
interesting, indeed. But the problem remains beyond the scope of this
book.

If all your users are not core resident, it is better if none of
them are. Then any input message can be written into the message buffer
area on disk. And all output messages read from disk. The fact that some
 users might reside in core, causes an unreasonable complication, and
the fact that disk access is fast compared to message transmission means
 that to attempt to save such disk accesses is not efficient.

## 7.1 User control

The fact that you have several users creates a new problem. Of course
the computer can only process one user at a time (we assume a single
processor). But when it's finished with one user, it must switch its
attention to another.

When is it finished with one user? Clearly, if a user is awaiting
input the computer is finished. We are talking about keyboard input,
which will take many seconds to arrive. Similarly if the user is sending
 output, the computer may as well stop. Output will take several
seconds, especially if an acknowledgement from the device is
anticipated. It needn't stop. While sending one message, it could be
composing the next. But it's much simpler not to attempt such overlap.
If the user is reading disk, the computer can stop.

I want to define a single phrase to cover these situations. I
shall say that a user relinquishes control of the processor whenever he
does message or disk I/O. This is a voluntary action on his part, and
those are the only times he relinquishes control. In particular, there
is no time quantum that will take control from him. For this reason:
With several users, code must clearly be re-entrant. However, if a user
is promised that he will be allowed to finish what he starts, if he will
 not lose control to someone else except when he relinquishes it, the
re-entrant requirements become much less onorous. The program need only
be re-entrant across I/O, which can save a lot of bother.

All right, what happens when a user relinquishes control? The
computer simple scans a table of users to see if anyone else is ready.
The table contains the address of the user's dictionary and a flag:
ready or not? The I/O complete interrupt routines simply mark the proper
 user ready. And if no one is ready, the computer scans the table
endlessly - it's got nothing better to do. Naturally, upon program
start-up, no one is ready.

## 7.2 Queing

You can save yourself a lot of trouble by putting some code in the user
controller. Two subroutines: QUE and UNQUE. When a user needs a facility
 that might be in use by someone else, he calls QUE. If it's available,
he gets it. If it's not available, he joins the que of people waiting
for it. When it is released, and his turn, he will get it.

For example, he can't read disk if someone else if reading disk. Or
at least he can't use a particular channel or device. While he's
waiting, of course he relinquishes control. When he's through with the
facility, he calls UNQUE which passes it to someone else.

These are extremely valuable routines, for there are many
facilities that can be handled in the manner; each disk, each line
(shared lines), the printer, block 1 (disk allocation), non-re-entrant
routines (SQRT). An extension will even permit exclusive use of blocks.

Naturally, I have in mind a specific way to implement QUE and
UNQUE. And I caution you, more strongly than usual, that plausible
modifications won't work. I'll try to mention all the reasons.

In addition to the user's dictionary address and ready flag, each
 user must have a link field - not in his dictionary, but in user
control. Each facility that is to be protected must have associated with
 it 2 fields: the owner, and the first person waiting. The best
arrangement is to have a table of such que-words, one for each facility.
 If a facility is free, its owner is 0; otherwise its owner is the
number of the user owning it. A user's number is his position in the
table of users, starting at 1. If no one is waiting, a facility's waiter
 field is 0; otherwise it is the number of the user waiting.

If I want a facility and its free:
   - I place my number in the owner field and exit.
If it's busy, but no one's waiting:
   - I place my number in the waiter field, 0 my link field, and relinquish control.
If someone's waiting:
   - I follow the chain of links starting at the waiter's link field
until I find a 0 link; I place my number there, 0 my link field, and
relinquish control.
When I'm through with a facility (UNQUE):
   - IF no one's waiting, I 0 the owner field, and exit.
   - If someone's waiting, I move his number to the owner field, move his link field to the waiter field, mark him ready, and exit.

The whole procedure is simple and efficient. It handles a lot of
potential problems in a reasonable and effective way. Several comments:
The ques will probably be very short. In fact, facilities will usually
be free, unless the computer is over-loaded. A user can not be in more
than one que. However, a user can own more than one facility. Hence the
need for a waiter field with each facility: a que must descend from each
 facility, and not from each owner; the two concepts are independent.
You must add to the error routine a loop to release any facilities held
by the current user. Since a user needs to know his own number in order
to que, this number must be stored in a field in his dictionary, and be
set by the re-initialize routine.

It's complicated, it's troublesome, and it's the price you must pay for multiple users.

## 7.4

To gain exclusive use of a block, with the exception of block 1, best
handled as an exception, set aside some facility que-words for this
purpose. Find a free one and store the block number it represents
somewhere, then treat that block like any other facility. When the last
waiter releases the block, release the facility que-word for re-use.
Notice that this technique has no effect upon the block itself. It may
be resident in core, or not. Anyone may read or write it. However, no
one else may have exclusive use of it. If all users cooperate to request
 exclusive use when the should, it works perfectly - with no extra cost
to ordinary reads/writes. Actually, exclusive use of a block is
necessary only under exceptional circumstances. Block 1 is an example of
 such: The block may not be used by anyone else until another block has
been read, and the available space up-dated.

## 7.3 Private dictionaries

The key to the case of conversion to multiple users is that all required
 information about a user is stored in his dictionary - a single
contiguous area of core. He makes extensive use of code that belongs to
the system, and that does not reside in his dictionary. On the other
hand, code unique to his application may reside there. Here is the first
 decision that you must make: What belongs in the user's private
dictionary?

Let us look at the arrangement of core. If we choose, and we should,
it follows dictionary format: each entry followed by the code it
executes. Each entry is linked to the previous so that the dictionary
may be searched backwards. Some entries are obviously of interest to all
 applications: those that control the stack, that define dictionary
entries, that specify fields such as BASE, CONTEXT, etc. Other entries
are probably of local concern: the names of fields in records,
definitions used to edit text, special purpose code (random number
generator, square root, etc.). At some point you must separate the
system and user dictionaries.

If you establish several user dictionaries, the first entry in
each will link to the system dictionary (Fig 7.1) at the same point.Thus
 each user is unaware of any other user, and his dictionary search is
unaffected.

### 7.3.2 Controlled access

It would appear that you want the system dictionary as large as possible
 to avoid redundancy. That is not necessarily the case. There are some
entries that might go into the system dictionary - except that you
specifically want to deny them to some users. Prime examples are the GET
 and DELETE entries that control disk allocation. Misuse of these words
by ignorant users can badly damage data stored on disk. The best
solution is to place the code in the system, without a dictionary entry.
 Define a table of entry points into code of this nature. Then if a user
 wants to use an entry point, he must first define it, perhaps:
   - 17 ENTRY GET 18 ENTRY RELEASE
establishing the words GET and RELEASE with the code identified in the
17th and 18th table positions. Library subroutines (FORTRAN arithmetic
subroutines) might be treated similarly.

Incidently, this illustrates a general method of protection: In
addition to using a word, the user must define it correctly. Clearly you
 can cascade the process. The value of such protection against malicious
 mischief depends on secrecy, which is always the ultimate protection.
However even in the absence of secrecy, it provides valuable protection
against inadvertant damage.

### 7.3.1 Memory protection

If all users will fit in core simultaneously, we are finished. You
divide memory into the appropriate dictionaries. You should provide
memory protection so that one user cannot damage another. The stack and
dictionary size checking discussed earlier, should be augmented by
checks on the = operator, so that a user cannot write outside his
dictionary, or outside a block he has read. If you have hardware memory
protect, you will find it difficult to take advantage of. The user must
be able to read his dictionary, the system dictionary and the block
buffers; he must be able to write his dictionary and the block buffers.
Several users might want to write the same block buffer; if not
simultaneously, at least consecutively. If your hardware can help, it's
better than any I've seen. But software protection can be made adequate -
 except against malicious mischief.

Although a user cannot hurt anyone else, he is certainly capable of
destroying himself. Thus you should have a system entry that will
restore his dictionary to empty, with all control fields reset. Such an
entry will get heavy use, for it is a simple way to start over.

If you have implemented fixed-size entries, you have no links to
lead to the system dictionary. Your search routine must separately
search the user's dictionary and the system dictionary, since not all
users can be contiguous to the system. This should only cost a few
instructions, but is another reason to prefer the linked entries.

If you have multiple chains in your dictionary, each chain must
jump from the user's to the system dictionary. This is only a problem
when re-initializing the dictionary, and can be easily solved by keeping
 a copy of the chain heads for the system dictionary.

## 7.4 Disk buffers

The fact that you may have several users reading disk simultaneously has
 no effect at all upon the disk-access subroutine. It can search the
block buffers and find an available buffer without concern as to who
asked for it. Of course you must have at least as many buffers as users.
 In fact, all of core not required for dictionaries might as well be
devoted to block buffers, as left idle. However, if a block is being
read, you should mark the buffer busy some way, so someone else will not
 assume it's there before it arrives. If you attempt to read a busy
block, you should relinquish control and try again when you're
re-started.

## 7.5 User swapping

So far we've had all users resident in core. This is by far the best
arrangement for handling a small number of users. The step to allowing
more users than can be simultaneously resident is a small one
philosophically, but can be very difficult to implement.

Suppose we had room for 4 user's dictionaries in core, but we wanted to
permit 40 users. Clearly we can store all 40 user dictionaries on disk
and load each one into core when he becomes active. Providing disk I/O
is substantially faster than message I/O there is not even a performance
 penalty associated. When a user is awaiting message I/O we write him
out to disk. When he completes his message I/O we read him back into
core. Naturally, we do not move him from core when he is waiting for
disk I/O, since it would take unreasonably long to write him out and
read him back compared to the original delay.

So far there are no problems. The problem arises as to where to read
him back into. We have 4 buffers: if we load users always into the same
buffer we have 4 classes of users, each of which can go into a single
buffer. We are begging for delays at one buffer while another is empty.

If we are going to the trouble anyway, we should make all buffers
 equivalent, and load a user into whichever one is free. However, now a
user's dictionary must be relocatable. That is, any references to his
dictionary must be relative to its origin, which is presumably stored in
 an index register.
This isn't too bad if we've planned from the start - way back with a
single-user program - to make all dictionary references relative; it
requires almost a complete re-write of the program if we did not, for
all dictionary references, and they're scattered all through the
program, must be indexed.

Actually, since any references to a block must be relative to the
 (variable) origin of the block, we aren't introducing a new problem;
merely extending an old one. However, there's another complication. We
now have a real distinction between our 2 dictionaries: the system
dictionary is absolute and the user dictionary is relative. Therefore
the same kind of entry must be treated differently, depending on which
dictionary it's in.

For example, if we have compiled code in the parameter area, an
absolute user dictionary can store the code address in the address
field. However a relative user dictionary must store the address of a
routine that will, in turn, jump into the parameter field. Or else
relative addresses must be distinguished from absolute addresses,
perhaps by size, and treated properly.

To avoid impossible difficulties, you should be careful to write your single-user program with the following constraints:
   - Reserve an index register for a user pointer, the origin of the user's dictionary, andd *use* this index. That is, treat the dictionary as relative, even though you needn't.
   - Make all code re-entrant. At least all code within which a user might relinquish control - which turns out to be most code.
Do this if you have the slightest intention of implementing a many-user
version. This violates the Basic Principle, but we're dealing with such
basic issues as to be worth it.
