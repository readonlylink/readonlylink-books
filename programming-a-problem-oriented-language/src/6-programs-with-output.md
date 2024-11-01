---
title: Programs with output
---

By now I'm sure you're aware that the heart of your program is its
control loop. It not only controls the operation, but also the
philosophy and organization of the program. Let me review its operation:
 it reads a word, finds it in the dictionary and executes its code;
failing that it converts it to a binary number and places it onto the
stack; failing that it types an error message.

So far I've ignored that error message; not because it's unimportant
or trivial to implement, but because it's part of a diffcult subject -
output. Logically I oughtn't have delayed discussing output this long,
for even a control language needs output. But as usual in this program
it is involved with other features that we've only just discussed. I'll
leave it to you to implement those features of the output capabilities
I'll present, that your application requires.

Most compilers, and therefore most programmers, regard output the
 inverse of input. For example, FORTRAN uses the same FORMAT statements
for output as for input, thereby suggesting that the two processes are
very similar. But are they?

*You* conpose input: you select words and combine them
into fairly complex phrases; your program spends considerable effort
deciphering this input and extracting its meaning. In reply it will not
go through any such elaborate procedure. You'll see that most of its
output consists of the word OK. You are talking to the computer, but it
is hardly talking to you; at best it's grunting.

I maintain that the two processes have nothing in common, that
the computer does not prepare output in a manner analogous to you
preparing input. In Chapter 8 I'll describe a way your program can
compose complex output messages. Although such a technique might provide
 a 2-way dialog, it has even less similarity to interpreting input.

## 6.1 Output routines

You will need 3 output subroutines; conceivably you could get by with 2.
 One to type a number of spaces. One to type a number of characters from
 a specified location (TYPE<sub>N</sub>). One to type characters until it encounters a space (TYPE<sub>B</sub>)
 and including the space. This last depends on your dictionary format,
for it is used to type entry words. Of course, these should use the
fetch and deposit subroutines you use for input.

Let us use the composition of an error message as an example. You
have just typed an input message, the carriage is positioned at the last
 character. First you want a space. Then use TYPE<sub>B</sub> to type
the current word. It caused the error and will tell you where it
occurred. You don't need this for an unbuffered device. Then use TYPE<sub>B</sub>
 again to type a word that describes the error. Avoid long error
messages - you're the one who will wait while they're typed. You can
detect a number of errors, so it's worth your while to devise a routine
to generate them.

After finding an error, you of course quit doing whatever you
were doing. There is no point in trying to continue when you're standing
 by ready to correct and start again. However it is convenient to reset
things that you'd probably have to reset anyway. In particular, set the
stacks empty. This is sometimes unfortunate since the parameter stack
might help you locate an error. But it usually is most convenient. Don't
 try to reset the dictionary since you're not sure what you may want to
reset it to.

## 6.2 Acknowledgement

I mentioned in Chapter 3 that you must write subroutines to send and
receive messages. Now I must expand on exactly how you should use these
subroutines.

Recall that input and output share the same message buffer. This now
causes trouble. However it considerably simplifies the more powerful
message routines of Chapter 7. On balance the single message buffer
seems optimal.

First let me call the subroutine that sends a message SEND. It
sends a single line and should add a carriage return to the line, as
well as any other control characters needed, and translate characters as
 required. The routine that receives a message is QUERY. It is a
routine, and not a subroutine. QUERY calls SEND to send a message, and
then awaits and processes an input message. stripping control characters
 and translating characters as required. It initializes the input
pointer IP and jumps to NEXT<sub>W</sub>.

Notice that your program can send output via SEND wherever it pleases.
However it can only receive input in conjunction with output, via QUERY.
 You have no provision for receiving successive messages without
intervening output. This is exactly the behavior you need, and actually
simplifies the coding of message I/O.

Now let me describe the use of QUERY. Each input message is
terminated with an end-of-message word, a non-printing character
surrounded by spaces. This word has a dictionary entry that types the
word OK and jumps to QUERY. Thus after interpreting each input message,
your program types a brief acknowledgement - OK, message received and
understood - and awaits further input.

Notice that if an input message generates output it destroys
itself. That is, the output is placed in the message buffer irrespective
 of the input already there. Thus a word that generates output should be
 the last word in a message, since succeeding words will not be seen. In
 particular, the end-of-message word won't be seen and the reply OK
won't be typed. This is what you want: OK is only typed in lieu of any
other output.

OK should appear on the same line as the input message, separated
 from the last word by a least one space. QUERY should not acknowledge
receipt of a message - as most time-sharing systems do - with a
carriage-return. The only acknowledgement is the OK at completion of
interpretation. Placing OK on the same line helps distinguish output
from input and compresses the conversation, especially valuable on a
limited-size scope face. A user must not type input until he receives
output. It's only important to enforce this rule with multi-user
programs. For this see Chapter 7.

In order to determine whether there is input in the message
buffer, establish a field EMPTY. QUERY should set empty false and each
output generating entry should set it true. Actually output generating
verbs have much in common with each other, and each should jump to a
routine that does the following:
   - Drop the stack. Each output verb must have an argument. Its last
argument can be dropped at this point, and the stack pointer checked
against its lower limit.
   - Set EMPTY true.
   - If NEXT contains NEXT<sub>W</sub> and SCREEN is 0, jump to QUERY. Under these circumstances there is no further input available in the message buffer.
   - Jump to NEXT.
Notice that if entries are coming from a definition or from a screen, no
 conflict can arise with the message buffer. Only if input is currently
being read from the message buffer is there a problem.

However there are 2 places where source of input is changed. This is in the code for ";" and ";S". If ";" restores NEXT<sub>W</sub>
 to NEXT, it must guarantee that input is available. That is, jump to
QUERY if EMPTY is true and SCREEN is 0. Likewise, if ";S" restores
SCREEN to 0, it should jump to QUERY if EMPTY is true (NEXT is guaranted
 to be NEXT<sub>W</sub>.

The logic required is summarized in Fig 6.2 and is the price paid
 for duplexing the message buffer. One final complication concerns
EMPTY. If true, it states that input has been destroyed; it does not
indicate that output is currently in the message buffer. Output may have
 been placed there and already sent. If the message buffer is empty,
type OK before jumping to QUERY.

## 6.3 Character strings

Everything isn't easy, and this particular feature is my nemesis.
Perhaps a measure of its value is the difficulty of its implementation. A
 character string is an awkward entity. Mostly because there is nowhere
to put it. Numeric literals go on the stack in a most natural fashion.
Character strings won't fit, and that isn't what we want to do with them
 anyway.

My solution is this. When you see a character string, leave it alone.
 Put on the stack a descriptor giving the address of the first
charactere and the number of characters in the string. Skip over the
string. That is, advance the input pointer to its end. You can't do it
in quite that order, of course, because only by skipping can you
discover the number of characters.

What does a character string look like? Of all the ways you might choose, one is completely natural:
   - "ABCDEF . . . XYZ"
A character string is enclosed in quotes. It can contain any character except a quote, specifically including spaces.

We get in trouble immediately! How do you recognize a character
string? By the leading quote, for course. But do you modify your word
subroutine to recognize that quote? If you do so you may never use a
leading quote for any other purpose. Much better that the quote is a
word by itself, treated like any other dictionary entry, for it can then
 be re-defined. But words are terminated by spaces, and I still resist
making quote an exception. So let's type character strings:
   - " ABCDEF . . . XYZ"
The extra space is annoying, but in Chapter 8 I will tell you how to
eliminate it without the objections I just raised. So a character string
 is started with a quote-space and terminated by a quote.

Remember that we leave the character string alone, merely remembering
 where it is. We are talking about character strings in the input buffer
 (so far), and we had better use the string before we destroy it with
output or additional input. *When* it is destroyed depends on many things, so the best rule is to use it immediately.

What can you do with a character string? I've only found 2 uses.
They are very similar, but part of the frustration of implementing them
is to take advantage of the similarity. You can type a string, or you
can move it to a character field.

To type a string is easy. Define an entry that uses the descriptor on the stack to set parameters for the TYPE<sub>N</sub> subroutine.

To move a string is harder, but still easy. You have 2
descriptors on the stack: on top a field descriptor; below the string
descriptor. Set the input and output pointers, and do a character move
of length the smaller of the 2 field sizes. Space fill the remainder of
the destination field. Notice that you musn't move more characters than
you have, or will fit. And of course, string descriptors will rarely
have the right size. Truncating a string is not an error condition!

If you can do the above, you can also move one character field to
 another. That is, if you make your character string and field
descriptors compatible - which adds to the fun. You might want to
prevent moving a field to a string, but than who cares.

The problem is to reconcile all the above requirements. Not
really to produce optimum code, but even to produce code that is
remotely acceptable in size, speed, restrictions and correct operation.

We've slid into the subject of field descriptors. You might want
to type a character field, and of course the same code should work as
for string descriptors.

## 6.4 Field entries

We've talked about the different kinds of numbers you might want, and
the different entries these require. However, all these entries dealt
with computation. Another kind of entry is useful for more sophisticated
 output purposes. I call it a field-entry because its most common use is
 to define a field in a data record.

In addition to the descriptor associated with a variable, a field
entry needs additional parameters that specify the output format. It is
extremely useful to be able to specify a field width for output once and
 for all, and then use it automatically on all reports. Also it is
useful to be able to reference the name of the field - which of course
is contained in the dictionary entry.

So a useful convention is that a field entry puts the address of
itself - that is the dictionary entry - on the stack. Recall that a
variable entry places the address of the variable on the stack. If you
want the name of the entry, this address tells you where it is. If you
want the format, this address - offset by some constant - tells you
where to find it. And if you want the address of the field, you can get
that too - a process that is executed automatically for a variable.

These various capabilities require various entries to effect them. You might define:
   - ,NM - type out field name.
   - F - extract field width.
   - @F - obtain field address.
Depending (as usual) you might be able to make @F compatible with @. Or
make @ automatically work correctly for field entries. You may want to
distinguish addresses of variables from address of field entries. This
would be analogous to distinguishing different kinds of numbers, and for
 the same reason - so that the same operations (in this case probably @
and =) will work on all.

Apply the Basic Principle.
