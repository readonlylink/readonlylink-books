---
title: Programs with input
---

A program without input is a program with a single task. A program with
input may have many tasks, which it will perform as directed by its
input. Thus I consider input to be control information, and the control
information to define a control language.

We shall have a problem in this chapter, for we are discussing a
loop. Each element of the loop depends on its predecessor and successor,
 and we have nowhere to start. I have done the best I could, but am
obliged to refer to things before I define them. Especially in the next
section where I try to justify some of the details we'll encounter
immediately after.

This chapter is full of details, more than I anticipated when I
started it. Although I'm surprised there's so much to say, I think it's
all of value. I only caution you not to get lost in the details; the
structure, the concept of the program are what is important.

To set the stage, let me briefly outline how our program must
operate. You are sitting at a keyboard typing input. You type a string
of characters that the computer breaks into words. It finds each word in
 a dictionary, and executes the code indicated by the dictionary entry,
perhaps using parameters also supplied by the entry. The process of
reading words, identifying them and executing code for them is certainly
 not unusual. I am simply trying to systematize the process, to extract
the inevitable functions and see that they are efficiently performed.

## 3.1 Nouns and verbs

I've mentioned the dictionary and we'll soon examine the details
required to implement it. But first I'd like to talk a bit about
individual entries to try and give you a feel for what we're doing.

We're going to read words from your input, find them in the
dictionary, and execute their code. A particular kind of word is a
literal, a word that identifies itself:

- 1 17 -3 .5

We won't find such words in the dictionary, but we can identify them by
their appearance. Such words act as if they were in the dictionary, and
the code executed for them places them on a push-down stack.

Other words act upon arguments found on this stack, for example:

- + add the last 2 numbers placed on the stack, leave the sum there.
- , type the number on top of the stack, and remove it from the stack.

If we type a phrase such as:

- 1 17 + ,

We are saying: put 1 onto the stack, 17 onto the stack, add them, and
type their sum. Each word performs its specific, limited function;
independently of any other word. Yet the combination of words achieves
something useful. In fact if we type:

- 4837 758 + -338 + 23 + 4457 + -8354 + ,

we can even do something non-trivial: each number is added to the sum of its predecessors, and the result typed.

This is basically the value of our program. It lets us combine simple operations in a flexible way to accomplish a task.

Let's look more closely at the words we used above. They fall into 2 distinct classes; English even provides names for them:

- Nouns place arguments onto the stack.
- Verbs operate upon arguments on the stack.

All words cause code to be executed. However in the case of nouns, the
code does very little: simply place a number on the stack. Verbs are
considerably more varied in their effects. They may do as little as add 2
 arguments, or as much as type out a result - which requires a great
deal of code.

In effect, nouns place arguments onto the stack in anticipation of
verbs that will act upon them. The word anticipation is a good one. In
order to keep our verbs simple, we promise that their arguments are
available. We could define a verb that reads the next word and uses it
as an argument; but in general we don't. It is not the business of a
verb to provide its own arguments; we use nouns to provide arguments
before we execute the verb. In fact, this substantially simplifies our
program.

We can extend the characterization of entries a little further. Verbs have different numbers of arguments:

- Unary verbs modify the number on the stack.
- Binary verbs combine 2 arguments to leave a single result.

Arithmetic operations are binary, arithmetic functions are usually
unary. However, there are more verbs than we can usefully catagorize.
For example, the verb "," that types the stack is not unary, since it
removes the number from the stack. Although it does have a single
argument.

Another way of distinguishing verbs is:

- Destructive verb removes its arguments from the stack.
- Non-destructive verb leaves its arguments on the stack.

Unary and binary verbs, as well as the type verb ",", are destructive.
The verb DUP, which I define to duplicate the top of the stack, is
non-destructive. In general verbs are destructive. In fact, I
deliberately define verbs to be destructive in order to simplify the
task of remembering which are and which aren't. I recommend that you do
the same.

Literals are nouns. We can define other words as nouns; words that use their parameter field to place numbers onto the stack:

- Constants place the contents of their parameter field onto the stack.
- Variables place the address of their parameter field onto the stack.

For example, if PI is a constant, it places 3.14 onto the stack. Thus:

- 1. PI 2. * / ,

reads: place 1. onto the stack, place 3.14 onto the stack, place 2. onto
 the stack, multiply (2. and PI), divide (1. by 2PI), and type.
Constants are particularly useful when you're using code numbers. It
lets you give names to numbers that might otherwise be hard to remember.

However the most important nouns by far are literals and variables. A
 variable gives a name to a location and not to a value, as elementary
programming texts laboriously explain. However, what higher-level
languages conceal is that variables may be used in 2 distinct ways:

- To name a location from which a value is to be taken.
- To name a location into which a value is to be stored.

A constant automatically performs the first; and inherently prevents the
 second (you can't store a value into a constant, for you don't know
where the constant came from). Rather than try to distinguish function
by context, as compilers do, we shall define 2 verbs that act upon
variables:

   - @ replace the address on the stack with its contents.
   - = Store into the address on the stack, the value just beneath it on the stack.
Thus if I type, where X is a variable,
   - X @ ,
I mean: place the address of X onto the stack, fetch its value, and type. And if I type,
   - X @ Y @ + ,
I mean: fetch the value of X, the value of Y, add, and type. On the other hand,
   - X @ Y =
will: fetch the address of X, then its value, fetch the address of Y, and store the value of X into Y. But if I type
   - X Y =

I'm saying: fetch the address of X, the address of Y, and store the
address of X into Y. Maybe this is that I mean to do, it's not
unreasonable.

I don't want to belabor the point, for we're getting ahead of
ourselves. But variables require special verbs, one of which (@) is not
ordinarily explicit. Incidently, I originally used the word VALUE for @.
 But the verb is used so often it deserves a single character name, and I
 thought @ (at) had some mnemonic value, besides being otherwise
useless.

I urge you to adopt the vereb @. Although you can conceal it in
various ways - we'll discuss one later - it adds needless complication.
Such a useful verb oughtn't be invisible. Besides it lets you store
addresses in variables - indirect addressing

   - X Y = Y @ @ ,

reads: store the address of X in Y; place the address of Y on the stack, fetch its value (the address of X) fetch *its* value (the contents of X), and type.

I hope I've given you some idea of how you can put arguments onto the
 stack and act on them with verbs. Although I define constants and
variables, unary and binary verbs, I hope it's clear that these are only
 examples. You must define the nouns and verbs and perhaps other kinds
of words that are useful for your application. In fact, I think that is
what programming is all about. If you have available a program such as I
 will now describe, once you decide what entries an application
requires, you'll find it absolutely trivial to code those entries, and
thus complete your problem.

## 3.2 Control loop

Our program has a structure that is easy to miss: it is a single loop.
However, it is a loop that is diffuse - scattered among all the code in
the program. Very few instructions are gathered together to form an
identifiable loop, so the loop warrants some explanation.

We are going to read a word from the input string, look up that word
in the dictionary, and jump to the routine it specifies. Each routine
will return to the top of the loop to read another word. We will be
discussing many routines and it will be helpful to have a term to
identify "return to the top of the loop to read another word". I will
use the word RETURN; you should provide a standard macro or label in
your program for the same purpose.

Actually, you accomplish 2 purposes: you mark the end of a
routine. And you identify the preceeding code as being a routine, as
distinct from a subroutine. Thus, I use the word RETURN with a totally
different meaning from the FORTRAN RETURN statement. I shall speak of
EXITing from a subroutine.

Included in your control loop should be a check that the
parameter stack has not exceeded its limits. This is best done after
RETURNing from a routine, and only needs to be done for routines that
use the stack. Thus there are 2 possible RETURN points (actually 3).

The control loop must be efficient. If you count the instructions
 it contains, you measure the overhead associated with your program. You
 will be executing some very small routines, and it's embarrassing to
find overhead dominating machine use. In particular you don't need to
check other than the parameter stack.

One more routine belongs in this section: an error routine.
Whenever an error is detected, a routine should jump to ERROR which will
 type the offending word and an error message. It will then reset all
stacks and the input pointer and RETURN normally.

The problem of how to treat error messages is an important one.
We are in a position to do a good job: to avoid setting and testing
flags; to avoid cascading back through subroutine calls. By clearing the
 return stack we eliminate any pending subroutine returns. By not
returning with an error flag, we avoid having the subroutine have to
worry about errors. This simplifies the code, but we must have a
standard method of handling problems.

The image of a person at a keyboard in invaluable for this
purpose. No matter what problem arises, we needn't worry about what to
do. Pass the buck; ask the user. For example, he types a word not in the
 dictionary. What to do? Ask him: type the word and an error message, in
 this case "?". He tries to add 2 numbers and there's only 1 on the
stack: type the word and "STACK!". He tries to access a field beyond the
 limit of his memory: type the word and "LIMIT!".

Of course you want to be careful not to pose the user problems he
 can't solve. Faced with a message "MEMORY PARITY" what can he do about
it? But he's certainly in a better position than your program to take
corrective action to most problems. And of course it's up to you to
decide what situations are problems.

By the way. Since you don't check the stack until after you
executed a routine, it will exceed stack limits before you know it. Thus
 stack overflow and underflow should be non-fatal. A good solution is to
 let the parameter stack overflow into the return stack, and underflow
into the message buffer. The return stack should never underflow.

## 3.3 Word subroutine

I've described the control loop that will run our program. The first
thing it does is to read a word; so the first thing we shall discuss is
how to read a word.

What is a word? Not a computer word, as I'm sure you realise,
although we shall have to use the word "word" in that sense. A word is a
 string of characters bounded by spaces. It is extracted from a larger
string of characters by the routine we are discussing.

Let me contrast this definition with more conventional input
routines. FORTRAN formatted input, for example, doesn't speak of words
but of fields. The meaning of a number is determined by the field it
resides in; that is, by its position on a card. Since we are not using
cards, the notion of position becomes clumsy and we replace it with
order: The order of the words we read is significant, though their
position is not. We lose, however, the ability to leave a field empty,
since we cannot recognise an empty word. All our data must be explicit,
which is probably a good idea but a slow one to learn. Decide now that
you will not specify input conventions that have optional parameters.

Very well, let's write the WORD subroutine. It uses the input
pointer to point at the current position in the source text, the output
pointer to point at the current position in memory where we will move
the word. We must move it; partly to align it on a computer-word
boundary and partly because we may want to modify it.

Fetch input characters and discard them so long as they're
spaces. Thereafter deposit them until you find another space. Deposit
this space and as many others as needed to fill out the last
computer-word. If you have a character-oriented machine you may be
amused at my insistance on word-alignment. Mainly I'm anticipating the
search subroutine when we'll want to compare as large a piece of the
word as possible. If a word holds 6 characters (or even 2) it's much
more efficient to compare them in parallel than serially, even if you
have the hardware.

You may want to set an upper limit on word length. Such a limit
should include the largest number you will be using. Then the question
arises as to what to do with a longer word. You might simply discard the
 excess characters, providing you don't plan to dissect the word
(Chapter 8). Better, perhaps, that you force a space into the word at
the limit. That is, break the word into 2 words. Presumably something's
wrong and you will eventually discover it in attempting to process the
fragments. However this limit should be large enough - 10 to 20
characters - so that it does not constitute a real restriction on your
input. It should also be 1 character less than a multiple of your
computer-word length, so that you can always include the terminal space
in the aligned word.

Words are bounded by spaces. You can probably find objections to
such a simple definition. For instance, arithmetic expressions often do
not have spaces between words. We shall discuss this in Chapter 9. Let
me just say that we need to embed periods, dashes, and other characters
in words in order not to unreasonably restrict our potential vocabulary.
 We'd like these to be words:

   - 1,000 1.E-6 I.B.M. B&amp;O 4'3" $4.95

### 3.3.1 Message I/O

The WORD subroutine presumably examines input characters. Where does it get these characters?

Although it's possible to read cards, I'm going to assume that you
have a keyboard to type input. Now there are 2 kinds of keyboards,
buffered and unbuffered. A buffered keyboard stores the message until
you type an end-of-message character. An unbuffered keyboard sends each
character as you type it. Your hardware, in turn, may buffer input for
you or not.

In any case we may want to examine each character more than once,
 so we want buffered input. Even if you can process characters as they
arrive, don't. Store them into a message buffer.

Set aside a 1-line message buffer. Its size is the maximum size
of a message, either input or output, so if you plan to use a 132
position printer make it large enough.

If you simulate buffering, you should implement a backspace
character and a cancel message character. For you will make a lot of
typing errors. If your hardware buffers, but does not provide these
capabilities, you should do so. This probably means a prescan of the
input; any other technique gets too complicated, and probably costs more
 in the end.

Mark the end of an input message with an end-of-message word.
This is a word bounded by spaces like any other. It may or may not
coincide with the end-of-message character that you typed, depending on
your hardware and character set as to whether the required spaces can be
 provided. This word permits ready detection of the last word in a
message. It will have a specific definition and perform a valuable task.

In addition to a keyboard, you must have some sort of output
device: a printer or scope. Again it may be buffered or unbuffered.
Unlike input, we have no reason not to use unbuffered output. However if
 you have several output devices, odds are one is buffered. If so, treat
 them all as buffered, simulating the buffering where needed.

We will use the same message buffer for both input and output. My
 motivation is to save space, or rather to increase the utilization of
space. My reasoning is that input and output are mutually exclusive.
There are exceptions, but we don't usually read input and prepare output
 simultaneously. At least we never have to.

However, we do need a switch (1 bit) that states whether the
message buffer still contains input. The first time (or perhaps
everytime) we type output, we must reset this switch. We'll use it
later.

We need a receive subroutine that will exit when we have a
complete input message. Likewise a transmit subroutine that will exit
after sending an output message. It should await an acknowledgement if
the hardware provides one. Don't try to overlap transmission of one
message with preparation of the next. Transmission is so slow and
preparation so fast that no noticable increase in speed is available.
And it complicates the program considerably.

### 3.3.2 Moving characters

I will speak of fetching and depositing characters several times, mostly
 concerned with input and output. For example, the WORD subroutine moves
 characters from the message buffer to a word buffer. A simple task
conceptually, but a difficult one to implement. We would have exactly
the same problem moving arrays from place to place. But in fact we
needn't move arrays and we must move characters.

Let us define 2 entities: an input pointer and an output pointer. For
 the moment you can think of them as index registers, although we will
have to generalize later. Let's also write 2 subroutines, although your
hardware may permit them to be instructions: FETCH will load the
character identified by the input pointer into a register, and advance
the input pointer; DEPOSIT will store that register at the position
identified by the output pointer, and advance the output pointer.

Depending on your computer, FETCH and DEPOSIT can be veery
simple, or extremely complex. If they require more than 1 instruction,
they should be subroutines, for we'll use them often. By combining them,
 we can perform a move. However, it's important to be able to examine
the character before depositing it. A hardware move instruction is of
little value.

The input and output pointers use index registers. However, those
 registers should only be used during a move. They should be loaded
prior to a move and saved after it, for they will be used for a number
of purposes, and it becomes impractical to store anything there
permanently.

## 3.4 Decimal conversion

After isolating and aligning a word from the input string, your control
loop searches the dictionary for it. If it isn't in the dictionary, it
might be a number. A number is a special kind of word that doesn't need a
 dictionary entry; by examining the word itself we can decide what to do
 with it. The code executed for a number will place the binary
representation of the number onto the stack.

We will discuss the stack in the next section. First let's define a number more precisely.

### 3.4.1 Numbers

It is very hard to state exactly what is a number and what is not. You
will have to write a NUMBER subroutine to convert numbers to binary, and
 this subroutine is the definition of a number. If it can convert a word
 to binary, that word is a number; otherwise not.

It is foolish to examine a word to see if it is a number, and then to
 convert the number to binary. Examination and conversion can be
combined into one process very easily.

There is one kind of word that invariably is a number: a string
of digits possible prefixed with a minus. Such numbers are usually
converted to binary integers. For example:

   - 1 4096 -3 7777 0 00100 10000000 6AF2 -B

are some decimal, octal and hex numbers. The number does not specify its
 base, and a word that may be a hexadecimal number, may not be a decimal
 number.

So already base has complicated numbers. And beyond simple integers
are endless other kinds of numbers: fixed-point fractions,
floating-point fractions double-precision integers, complex fractions,
etc. And such numbers can have many different formats as words: decimal
point, implied decimal point, exponents, suffixes. Indeed, the same word
 may represent different numbers depending on its context.

One of your major tasks will be to decide what kinds of numbers
you need for your application, how you will format them, and how you
will convert them. Each kind of number must be uniquely identifiable by
the NUMBER subroutine, and for each you must provide an output
conversion routine.

I suggest the following guidelines: always define integers and
negative integers; do not permit a prefixed plus sign, it is useless on a
 number and useful as a word; if you have floating-point hardware,
distinguish floating-point fractions by a decimal point; if you lack
floating-point hardware, use the decimal point to identify fixed-point
fractions; don't simulate floating-point; don't permit exponents on
fractions. These rules permit a simple NUMBER subroutine which I will
outline.

Your application may need special number formats:

   - 45'6 for 45 ft. 6 in., an integer
   - 1,000,000 an integer
   - $45.69 an integer

It is not hard to include such numbers in NUMBER, but you cannot include all possible formats. Some are incompatible:

   - 3'9 for 3 ft. 9 in.
   - 12'30 for 12 min. 30 sec. of arc
   - 12'30 for 12 min. 30 sec. of time
   - 4'6 for 4 shillings 6 pence

Basic Principle!

Fixed-point numbers are rarely used. I am convinced of their value
and would like to show you. With floating-point hardware, they offer
only the advantage of greater significance, which is probably not worth
much. However, without floating-point hardware they offer most of the
capabilities of floating-point numbers, without the very great cost of
floating-point software. The exception is a wide range of exponents.

I am convinced that exponents are badly misused on computers.
Most applications use real numbers that can be used on a desk-calculator
 - say between 10<sup>6</sup> and 10<sup>-6</sup>. Such numbers can be
equally well represented in fixed-point format. Floating-point is not
needed, although if hardware is available it might as well be used.
There are cases, especially in physics, when large exponents occur - 10<sup>43</sup> or 10<sup>-13</sup>. But this usually indicates that the proper units have not been chosen, or maybe even that logarithms should be used.

Of course compilers do not implement fixed-point, so people don't
 use it. We are in a position to implement it, and to take advantage of
the speed possible with fixed-point (integer) instructions. What does a
fixed-point number look like? Choose the number of decimal places you
want to use. You may change this from time-to-time, but shouldn't mix
numbers with different precision. Have your NUMBER subroutine align all
numbers (with decimal points) as if you had typed exactly that number of
 decimal places. Thereafter treat that number like an integer. That is,
if you choose 3 decimal places:

   - 1. is considered 1.000 and treated as 1000
   - 3.14 is 3.140 and 3140
   - 2.71828 is 2.718 and 2718
   - -.5 is -.500 and -500

I wouldn't bother rounding unless your application demanded it, or your hardware made it easy.

You can add and subtract such numbers without concern; their decimal
points are aligned. After multiplying 2 numbers, you must divide by 1000
 to re-align the decimal points. Hardware usually facilitates this; the
result of a multiply is a double-precision product in the proper
position for a dividend. Before dividing 2 numbers, you must multiply
the dividend by 1000 to maintain precision and align the decimal points.
 Again this is easy.

So providing your words are large enough to store the number of
decimal places you need, fixed-point arithmetic is easy. If you have the
 hardware, double-precision numbers and operations let you deal with
larger numbers. Just as easily. And much easier than simulating
floating-point operations. You may have to write your own square-root
and trig-function subroutines, but there are approximations available
that make this not-difficult. And they'll be much faster than the
equivalent simulated floating-point subroutines.

Aligning decimal points is easy to visualize, and avoids
truncation problems. However you may prefer to align binary points. That
 is, instead of 3 decimal places, keep 10 binary places to the right of
the point. The multiplication and division by 1000 can then be replaced
by binary shifts - the equivalent for binary - which are much faster.
You must balance the gain in speed against the problem of alignment
during conversion (input and output) and truncation during
multiplication and division being more subtle. And possibly the
difficulty of explaining your arithmetic.

### 3.4.2 Input conversion

Now let's discuss the NUMBER subroutine in detail. First, why is it a
subroutine? If you examine the program I've outlined so far, and even
the program as augmented by the end of the book, you'll fiind NUMBER is
called only once - in the control loop. By my own rules NUMBER should
thus be in-line code. However, I can't bring myself to put it in line;
the logic in NuMBER is so complex that I want to isolate it away from
the control loop, to emphasize its logical function - one purpose of a
subroutine - and to reduce confusion in the control loop itself; also
I'm never confident that I won't want to call NUMBER from some other
routine, in fact I have. But I think that such violations of programming
 standards should be explicitly recognised.

The key to a good NUMBER subroutine is another subroutine that it
calls. This subroutine has 2 entry points: SIGNED tests the next
character for minus, sets a switch, zeros number-so-far and falls into
NATURAL. NATURAL fetches characters, tests that they're digits,
multiplies the number-so-far by 10 and adds the digit. It repeats until
it finds a non-digit.

With this routine, NUMBER can work as follows: set the input
pointer to the start of the aligned word, call SIGNED. If the stopping
character is a decimal point, clear counter, call NATURAL to get the
fraction, and use counter to choose a power-of-ten to convert to a
floating or fixed-point fraction. In any case, apply SIGNED's switch to
make number-so-far negative. Exit.

The routine that calls NUMBER can test the stopping character:

   - If it is a space, the conversion was successful.
   - Otherwise, the word was not a number.

For example, the following are numbers:

   - 0 3.14 -17 -.5

The following are not:

   - 0- 3.14. +17 -.5Z X 6.-3 1.E3

In each case NUMBER will stop on a non-space. The number-so-far will be
correctly converted up to that point (possibly 0) but it is of no value.

SIGNED/NATURAL is a valid subroutine since it is called twice.
Moreover, if you define other number formats, you'll find it useful. For
 example, the format ft'in

   - After calling SIGNED, if the stopping character is a ' multiply

number-so-far by 12 and call NATURAL. Then proceed as usual, testing for
 decimal point.
If you want to verify that "in" are less than 12, you'll want to modify this slightly.

In NATURAL the number-so-far is multipled by 10. Don't use a litereal
 10, but rathere define a field (BASE) and store a 10 there as
multiplier. Then you can change BASE to 8 (or 16) and handle octal
numbers. You can even change it to 2 to use binary numberes. NATURAL
should test for digits by comparing them with BASE, thus prohibiting 9
in an octal number. Hexadecimal input numbers cause an additional
problem because the digits A-Z do not follow 9 in standard character
sets. It is thus harder to recognise digits; but this problem is
isolated in a single place (NATURAL) and is easy to code:

   - An origin must usually be subtracted from a digit to get its

binary value. If BASE is 16, a different origin is subtracted from A-F.

NUMBER should be efficient, at least in recognising words that are
not numbers. Not so much because you will use so many numbers, but
because you will examine many words that aren't numbers. We will discuss
 this further in Chapter 8. It is also important that you examine the
aligned copy of a word. There are several reasons: to avoid trouble with
 the input pointer, to guarantee a terminal space. However this creates a
 problem: the largest number you will use must fit in the aligned word;
this may require a longer word than you would otherwise use. A number
longer than word-size will have its right-most digits discarded. This
will probably not destroy its numeric appearance so that no error will
be detected; but the conversion will be incorrect. This problem is not
serious, just be aware of it.

### 3.4.3 Output conversion

Numeric output is harder than numeric input because there is an extra
step involved. During input, you multiply the number by 10 and add each
digit. You can work from left to right. During output, you divide by 10,
 save the remainder for the digit, and repeat with the quotient until it
 becomes 0. You get the digits from right to left, but you want to type
them from left to right.

Thus you need somewhere to store the digits temporarily. A good place
 is the far end of the message buffer. The space is unused since you
presumably have enough space for the number. Of course, you can use the
stack. If you place a space at the right end of your temporary storage,
and then deposit the digits from right to left, you can use the TYPE<sub>B</sub> subroutine to finally type the number.

You'll probably want to handle both negative numbers and
fractions. Remember the number is negative and work with its absolute
value. After you're finished, prefix a minus. Fractions require 2
conversion loops: one to convert the fraction, counting the number of
digits and depositing a decimal point; another to convert the integer,
stopping when the quotient becomes 0. You don't want to test the
quotient in the fraction.

If you take the care, and spend a couple of instructions, you can improve the appearance of your numbers by:

   - Not typing a decimal point if the number has no decimal places.
   - Not typing a leading zero to the left of the decimal point.

You will probably have several dictionary entries specifying different
output formats. For example, each kind of number: integer, float,
complex will need its own output routine.  However the actual conversion
 should be done by a single subroutine with parameters to distinguish
special cases. That is, a single subroutine inverse to the NUMBER
subroutine. The similarities among different numbers are much greater
than their differences.

If you use decimal fixed-point fractions, you already have a field D
that specifies the number of decimal places. The same field is used to
control decimal placement on output. Ordinarily decimal places on input
and output will be the same. Even with floating-point numbers you need
that field, since you're rarely interested in full precision output.

If you want to produce reports - carefully formatted columns of
numbers - you will need to right-justify numbers. That is, to line up
decimal points. For this you need another parameter F, the width of the
field in which the number is to be right-justified. It's easy to use:
after converting the number right-left, compute the number of spaces you
 need and call SPACE. Then call TYPE<sub>B</sub>. In determining spaces, remember that TYPE<sub>B</sub> always types a space *after*
 the number. Thus you will always have at least a single space between
numbers. If the number won't fit in the field you specify, you'll still
have that one space, and the full number will be typed - fouling up the
report format - but showing you the bad number.

Let me acknowledge that if you are going to right-justify numbers
 you can place the digits directly into position from right to left, for
 you know where the rightmost digit must go. But then you must
space-fill the message buffeer before starting output, and you can't
type unbuffered output immediately. However, my main objection is that
you can't compose free-format output. For example, place a number in a
sentence without extra leading spaces. And very often unformatted output
 is adequate, saving you having to specify field sizes you don't care
about.

Depending on your formatting requirements, there are other
dictionary entries you might want: A SPACE entry, to space the number of
 positions on the stack. It can even space backwards - by changing the
output pointer - if the stack is negative. This is useful if you want to
 suppress that space provided by TYPE<sub>B</sub>. A tab entry might calculate the amount to space in order to reach a specific position on the stack.

## 3.5 Stacks

We will be using several push-down stacks and I want to make sure you
can implement them. A push-down stack operates in a last-in first-out
fashion. It is composed of an array and a pointer. The pointer
identifies the last word placed in the array. To place a word onto the
stack you must advance the pointer, and store the word (in that order).
To take a word off the stack you must fetch the word and drop the
pointer (in that order). There is no actual pushing-down involved,
though the effect is the same.

A stack pointer is an excellent use for an index register, if you
have enough. Indirect addressing is also a possibility, especially if
you have an add-to-memory instruction.

### 3.5.1 Return stack

This stack stores return information. One use is to store the return
address for subroutines, when subroutine calls use an index register.
The last-in first-out nature of a stack is exactly the behavior required
 for nested subroutine calls. We will later encounter several other
kinds of return inforrmation that can be stored in the same stack. It is
 important not to attempt to combine the return stack and the parameter
stack. They are not synchronized. 8 words is probably enough space for
the return stack.

### 3.5.2 Parameter stack

This stack is the one I intend when I say simply stack. Numbers,
constants, variables are all placed on this stack, as will be discussed
later. This stack is used to pass parameters among routines. Each
routine can find its arguments there, regardless of how many other
parameters are present, or how long ago they were placed there. You
should not implement a parameter stack less than 16 words long.

A valuable refinement to the parameter stack is to set aside a
register to hold the word on top of the stack. Several rules must be
religously observed if this is not to cause trouble:

   - You must never use this register for any other purpose.
   - You must keep this register full; no flag to indicate that it's empty.
If you cannot fulfill these conditions, you're better off with the stack entirely in core.

We need some terminology:

   - You *place* a word *onto* then stack, thereby increasing its size.
   - You *drop* a word *from* the stack, thereby decreasing its size.
   - The word on top of the stack is called the *top* word.
   - The word immediately below the top of the stack is called the *lower* word.

You may need to control the parameter stack from the input. These words
(dictionary entries) are extremely useful, and illustrate the
terminology above:

   - DROP drop the top word from the stack.
   - DUP place the top word onto the stack, thereby duplicating it.
   - SWAP exchange the top and lower words.
   - OVER place the lower word onto the stack; move it over the top word.

## 3.6 Dictionary

Every program with input must have a dictionary. Many programs without
input have dictionaries. However these are often not recognised as such.
 A common 'casual' dictionary is a sereies of IF . . . ELSE IF . . .
ELSE IF . . . statements, or their equivalent. Indeed this is a
reasonable implementation if the dictionary is small (8 entries) and
non-expandable.

It is important to acknowledge the function and existence of a
dictionary, to concentrate it in a single place and to standardize the
format of entries. A common characteristic of bad programs is that the
equivalent of a dictionary is scattered all over the program at great
cost in space, time and apparant complexity.

The most important property of an entry is one that is usually
overlooked. Each entry should identify a routine that is to be executed.
 Very often many entries execute the same routine. Perhaps there are few
 routines to choose among. This tends to conceal the importance of
specifying what is to be done for each entry. By placing the address of a
 routine in each entry, an optimal and standard procedure for getting to
 that code can be designed.

Significantly, the IF . . . ELSE IF construction has the characteristic of associating a routine with each entry.

### 3.6.1 Entry format

There are 2 distinct ways to organize dictionary entries. The choice may
 depend upon hardware characteristics, but I recommend the second. A
dominant feature of entries is that they have variable length. A part of
 the entry may be the code to be executed, or parameters or an area of
storage, all of which may have arbitrary length.

One possibility is to split an entry into two portions, one of fixed
size, one of variable size. This permits scanning fixed size entries to
identify a word and often there are hardware instructions to speed this
search. A part of the fixed entry can be a link to a variable area; of
course you choose the fixed size so as to make the link in the nature of
 an overflow - an exception.

However, since input is relatively small volume (even as
augmented in definitions), to minimize the time required to search the
dictionary does not lead to a global optimum. You can gain greater
flexibility, a simpler allocation of core, and ultimately greater speed
by chaining the variable-sized entries together directly. This is the
organization I shall discuss.

An entry has 4 fields: the word being defined, the code to be
executed, a link to the next entry and parameters. Each of these
warrants discussion.

The format of a word must be decided in conjunction with the word
 input routine. It should have a fixed size which may be smaller than
that defined by NEXT, but must be a multiple of hardware word size.
However, more sophisticated applications use the dictionary words to
construct output messages. Then it is important not to truncate words,
in which case the word field must have variable length. To mark the size
 of this field the terminal space should be used rather than a character
 count. To handle a variable word field within a variable entry, the
word should extend in one direction (backwards) and the parameter in the
 other (forwards). Fixed or variable word size requires application of
the Basic Principle.

The code field should contain the address of a routine rather
than an index to a table or other abbreviation. Program efficiency
depends strongly on how long it takes to get to the code once a entry is
 identified, as discussed in 3.9. However, the small size of your
program may permit this address to fit in less space than the hardware
address field.

The link field may likewise be smaller than hardware-specified.
It should contain the absolute location of the next entry rather than
its distance from the current entry.

The parameter field will typically contain 4 kinds of information:

   - A number, constant or variable, of variable size.
     The nature of the number is determined by the code it executes.
   - Space in which numbers will be stored - an array.
     The size of the array may be a parameter, or may be implicit in the code
     executed.
   - A definition: an array of dictionary entries representing
     virtual-computer instructions; see 3.9.
   - Machine instructions: code compiled by your program which is
     itself executed for this entry. Such data must probably be aligned on
     word boundary, the other need not.

### 3.6.2 Search strategies

One basic principle applies to dictionary search: it must be backwards -
 from latest to oldest entries. You have perhaps noticed that the
dictionary is *not* arranged in any order (ie. alphabetical)
other than that in which entries are made. This permits the same word to
 be re-defined, and the latest meaning to be obtained. There is no
trade-off valuable enough to compromise this property.

To identify a word, place it (or its first portion) in a register and
 compare for equality with each entry (or its first portion). An
algebraic comparison is adequate. Concern is sometimes expressed that
treating words as floating-point numbers may permit a false equality.
This has 0 probablity and you can always change the word - ignore it.

A full-word compare (rather than a character-by-character) should
 be used for speed. A match is usually found on the first portion, and
extensions may be treated with less efficiency (though still full-word
compares).

Fixed-length entries may be scanned with a simple loop. Linked
entries require an equally simple loop, but usually a slower one.
However the speed of a linked search can be increased without limit:
Rather than link each entry to its physical predecessor, link it to a
predecessor in one of a number of chains. Scramble the word to determine
 which chain it belongs in, both when you enter it and when you search
for it. Thus, only a fraction of the total dictionary need be searched
to find the word or assure its absence.

The number of chains should be a power of 2: 8 will provide a
useful increase in speed. The scramble technique may be very simple: add
 the first few characters together and use the low-order bits. In order
to maintain a linked dictionary, the next available location and the
location of the last entry must be kept. A multiply-chained dictionary
requires the location of the last entry for each chain: a small price in
 space for a large gain in time.

However, search time is not a important consideration, and I
advise against multiple chains unless the dictionary is very large
(hundreds of entries).

### 3.6.3 Initialization

The dictionary is built into your program and is presumably initialized
by your compiler. This is centainly true if you have fixed-size entries.
 Variable-sized entries must be linked together, however, and this can
be beyond the ability of your compiler, especially if you have multiple
chains.

In such a case, it is a simple matter to write a loop that scans the
dictionary and establishes the links. It should scan the core occupied
by the dictionary and recognise an entry by some unique flag (7's in the
 link field). It can the pick up the word, scramble it and add it to the
 appropriate chain.

This is purely temporary code. Although it may call permanent
subroutines to scramble and link, the initialization code will have no
further use. Thus it should be placed where it can be overlaid as the
program proceeds. The message buffer, if large enough, or the disk
buffer are possibilities.

Other things may need initializing, particularly any registers
that are assigned specific tasks. All such duties should be concentrated
 in this one place.

## 3.7 Control language, example

Applications tend to be complicated before they become interesting. But
here's a fairly common problem that shows off a control language to
advantage. Implementation would be tricky, execution woud be
inefficient; but the program would be simple, and its application
flexible.

The problem is to examine a sequential file, select certain records,
sort them, and list them - in many different ways. Suppose these
variables define the fields in the record:

   - NAME AGE SALARY DEPT JOB SENIORITY
Let's define these verbs:
   - LIST SORT EQUAL GREATER LESS
Each acts upon the temporary file produced by the previous, in accordance with the following examples:

List in alphabetical order all employees in dept 6:
   - 6 DEPT EQUAL NAME SORT LIST
First we choose records with dept = 6 and copy them into a temporary file. Then we sort that file by name. Then we list it.

List twice, by seniority, all employees holding job 17 in dept 3:
   - 17 JOB EQUAL 3 DEPT EQUAL SENIORITY SORT LIST LIST
List, by age, all employees whose salary is greater than $10,000; and identify those whose seniority is less than 3:
   - 10000 SALARY GREATER AGE SORT LIST 3 SENIORITY LESS LIST
Several comments seem indicated. We can apply a logical "and" by using
several select verbs in sequence; we cannot use a logical "or". We can
sort on several fields, if our sorting technique does not unnecessarily
re-arrange records. We need 2 more verbs:
   - REWIND END
to start over with the original file, and to quit.

Actually many other capabilities could be provided, including the
ability to locate specific records and modify them. But rather than
design a particular application, I just want to show how nouns and verbs
 combine to provide great flexibility with a simple program. Notice how
even such a simple example uses all our facilities: the word subroutine,
 the number subroutine, the dictionary, the stack. We're not
speculating, we are providing essential code.
