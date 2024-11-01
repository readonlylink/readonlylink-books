---
title: Programs without input
---

The simplest possible program is one that has no input. That is a
somewhat foolish statement, but if you'll give me a chance to explain we
 can establish some useful definitions.

First consider the word "input". I want to use it in a specific sense:

- Input is information that controls a program.

In particular, I do not consider as input:

- Moving data between media within the computer. For instance,
  - copying tape onto disk, or disk into core.
- Reading data into the computer. This is really a transfer between media:
  - from card to core.

However, data very often has input mixed with it - information that
identifies or disposes of the data. For example, a code in col. 80 might
 identify a card. It is input, the rest of the card probably data.

Many programs have input of a kind I shall disregard: operating
systems use control cards to specify which files to assign, which
subroutines to collect, etc. Such information is definitely input to the
 operating system. Although it may affect the operation of your program,
 ignore it because it is not under your control - unless your program is
 the operating system itself.

In order to sharpen your recognition of input, let me describe a
program that has input. Consider a program that fits a smooth curve
through measured data points. It needs a lot of information in order to
run: the number of data points, the spacing between points, the number
of iterations to perform, perhaps even which function to fit. This
information might be built into the program; if it is not, it must be
supplied as input. The measured data itself, the object of the entire
program, is not input; but must be accompanied by input in order to to
intelligible.

A program that has no input may be extremely complex. Lacking
input simply means the program knows what to do without being told. That
 built into the code is all the information needed to run. If you are
willing to re-compile the program, you can even modify it without input.

But I'll be viewing programs from the input side. I'll be ranking
 programs according to the complexity of their input and I plan to
demonstrate that a modest increase in the complexity of input can
provide a substantial decrease in the complexity of the program. From
this point of view, a program with no input is simple.

Since I'm going to be talking about input, a program without
input leaves me nothing to talk about. But I want to make some points
about programs in general, so I'll make them here. For one thing, we
will be climbing a tree. When we reach the higher branches we'll have
enough trouble keeping our balance without worrying about the roots.

## 2.1 Choosing a language

We shall be less interested in computer language than most programmers.
For 3 reasons: First, we will eventually define our own
application-oriented language. How we implement that language is of
lesser concern. Second, you probably aren't in a position to pick a
language. Your installation probably has reduced your choice to nil.
Third, we won't be talking about problems at the language level.

This last comment deserves elaboration. I assume that you are already
 a competent programmer. I'm not interested in teaching you how a
computer works, or how a language conceals the computer. I want to talk
about problems common to all programs in a machine-independent and
language-independent manner. I will leave to you the details of
implementation. I am not going to write a program, I am going to show
you how to write a program.

I hope that you are a good enough programmer to think in
computerese. That is, as someone discusses their application, you
interpret it in terms of computer operations: a loop here, a calculation
 there, a decision . . . The details are largely irrelevant, the gross
structure of the program is of concern.

As you put more thought into the problem, you begin to relate it
to your particular machine: this data comes off tape, that loop is
stopped by . . ., this is really a 3-way branch. you modify the problem
as required by your particular hardware configuration.

Finally, you must translate your program into a particular
language. You encounter a new class of problem: your FORTRAN won't run
that loop backwards, COBOL doesn't have a 3-way branch, you couldn't
access the data that way. . . Current languages put more constraints on
this last coding process than they should.

I'll have a bit more to say about languages, but mostly we'll
stay at the most abstract level - talking computerese. We won't be
talking in meta-language exclusively. I may tell you to load an
index-register or to jump on negative and you'll have to translate that
into the equivalent for your computer and language.

Now let's look at the major failing of higher-level languages. In
 attempting to achieve machine-independence and to be applicable to a
wide range of applications, they only give you acess to a fraction of
the capabilities of your computer. If you compare the number of loop
control instructions on your computer to the number of loop constructs
in your language, you'll see what I mean.

Let me indulge in a 1-sentence characterization of 3 popular languages to illustrate their restricted capabilities:

- FORTRAN is great at evaluating complicated algebraic expressions.
- COBOL is great a processing packed decimal data.
- ALGOL is great a providing loops and conditional statements.

Each language can be very efficient at its sort of job. But if you want
conditional loops involving complicated decimal expressions you have a
problem.

We are going to be concerned with efficiency. We are going to do some
 things that if we don't do efficiently, we can't do at all. Most of
these things will not fit in the framework of a higher-level language.
Some will; others will demand controlled use of the hardware that a
compiler doesn't permit. For example, upon entering a FORTRAN subroutine
 it may save the registers it uses. If you didn't need to save them
you've wasted time and space. An ALGOL subroutine may expect registers
available that you have reserved; then you have to save them. It may
well cost you more effort to interface with the compiler than it saves
you in return.

Moreover, none of these languages are very good at moving things
around. Most statements are data transfers - count them in your latest
program. There is a profound philosophical truth concealed in how much
we can accomplish by moving numbers around. If we can move several
things with one instruction, or put the same register several places -
we can't afford not to.

You will have to code in assembler! Not the whole program, if you
 insist, but the important parts that we'll be concentrating on. You
might be able to do some of these in FORTRAN, but it simply isn't worth
the effort. I'll show you where higher-level subroutines can go, and I
think you'll agree there is good reason to restrict them to that
function.

I recognise the drawbacks of assembler and chafe at them as much
as anyone. I don't like to punch and debug 10 times as many cards
either. But I will in order to get the performance I need. By the way, I
 will use the word "compiler" to include assembler; we will *compile* an assembly language program.

Later I'll show you how to write a program in a forgotten
language: machine language. By that I mean sitting at the console and
entering absolute, binary instructions with the switches. Depending on
the hardware and software available, and the nature of your application,
 it may just be the best language of all.

## 2.2 Choosing a computer

Of course I don't expect that you're in a position to choose a computer.
 Nor am I going to discuss hardware at all. But I do have a mental image
 of the kind of computer, and explaining it may help you understand some
 of my comments.

Most applications can be programmed very nicely on a small computer:
say 4K of 16-bit words with a typical instruction set, floating-point
hardware if needed. If, that is, the computer is augmented with random
access secondary memory, which I will call disk. The capacity of disk is
 unimportant, even a small disk providing plenty for our purposes, and
is determined by the application. However, it is important to be able to
 copy the disk onto another disk, or tape, for back-up. Thus I envisage a
 small computer with 2 secondary memories, and of course a keyboard or
card-reader and printer or scope for input and output.

Instead of running applications in serial on a small computer,
you can run them in parallel on a large one. I see no advantage, for the
 amount of core and disk you can afford to use for a single application
is about that available on a small computer. You don't gain speed, you
suffer from a complex operating system, and you have a enormous capital
investment. But the configuration I have in mind remains the same: 4K of
 core, secondary memory and input/output device.

## 2.3 Arrangement and formatting

Now I'm goiong to tell you how to write a program. Independent of
language or computer. Things you ought to be doing already, but probably
 aren't because on one ever told you to. Little things. But if you don't
 do them you won't have a good program; and we're going to write a good
program.

Remember the Basic Principle! If you didn't read the Introduction, do it now.

Declare all variables. Even in FORTRAN when you don't have to.
Everyone likes to know what parameters you are using, presumably need to
 use; likes to count them, to see if they could use fewer; is annoyed if
 you slip one in without mentioning it.

Define everything you can before you reference it. Even in
FORTRAN when you don't have to. Why not? You don't like to read a
program backwards either. 'Everything you can' means everything except
forward jumps. You better not have many forward jumps.

Make variables as GLOBAL as possible. Why not? You can save some
space and clarify your requirements. For instance, how many Is, Js and
Ks do you need? In most cases a single copy in COMMON would suffice (you
 have to declare them, remember, and may as well put them in COMMON);
you can redefine it locally if you must; and it is of interest that you
must.

Indent! High-level languages, even modern assemblers, fail to
insist that you start in column x. But you do! The unbelievable appeal
of a straight left margin! Paper is 2-dimensional. Use it! If you indent
 all statements inside a loop, it's obvious at a glance the extent of
the loop. If you indent conditionally executed statements you'll find
that nested conditions sort themselves out - automatically. If you
indent little statements you wish you didn't have to include (I = I)
you'll find they intrude less as you glance through the listing. Always
indent the same amount, 3 spaces/level is good. Be consistant and be
accurate. Sloppy indenting is obvious.

## 2.4 Mnemonics

You will find as you read, that I have strong opinions on some subjects
and no opinion of others. Actually I have strong opinions on all, but
sometimes I can't make up my mind which to express. Fortunately it
leaves you some decisions to make for yourself.

Use words with mnemonic value. Unfortunately what is mnemonic to you
may not be mnemonic to me; and I'm the one who judges. Also
unfortunately, mnemonic words tend to be long, which conflicts with:

Use short words. You don't want to type long words, and I don't
want to read them. In COBOL this means avoid dashes and avoid
qualification, though both can be useful upon occassion.

So let me suggest a compromise: abbreviate in some consistant
fashion and stick to your own rules. I can probably figure out the rules
 you're using. You might even mention them in a comment.

Use words with the correct grammatical connotations: nouns for variables, verbs for subroutines, adjectives for . . . Do *not*
 use clever words (GO TO HELL). Their cuteness wears off very fast and
their mnemonic value is too subjective. Besides they offer an unwanted
insight into your personality.

Use comments sparingly! (I bet that's welcome.) Remember that
program you looked through - the one with all the comments? How helpful
were all those comments? How soon did you quit reading them? Programs
are self-documenting, even assembler programs, with a modicum of help
from mnemonics. It does no good to say:

- LA B . Load A with B

In fact it does positive bad: if I see comments like that I'll quit reading them - and miss the helpful ones.

What comments should say is *what* the program is doing. I have to figure out *how* it's doing it from the instructions anyway. A comment like this is welcome:

- COMMENT SEARCH FOR DAMAGED SHIPMENTS

Mnemonics apply to variables and labels (You can even get mnemonic
value in FORTRAN statement numbers). Where possible you should apply
them to registers also. You may do well to assign several names to the
same entity, to indicate its current use. However, don't waste effort
naming things that don't need names. If you need a counter, use I, J, K;
 to assign a big name (EXC-CNTR) to an insignificant variable is no
help.

## 2.5 Routines and subroutines

There are two words I need to establish precise definitions for: A *subroutine* is a set of instructions that return from whence they came. A *routine* is a set of instructions that return to some standard place.

To put it another way, you *jump* to a routine, you *call* a subroutine. The difference is retained in higher-level languages: GO TO versus CALL or ENTER.

So what? Subroutines suffer from nesting. If you call a
subroutine from within a subroutine you must somehow save the original
return address. I'm sure you can rattle-off a dozen hardware/software
ways of doing this. They're all expensive.

If you jump somewhere, not intending to come back, you can save
trouble, time and space. But only if you really never come back. To
simulate a subroutine call is worse than ever.

Higher-level languages conceal this by nesting automatically. The
 best solution is to nest if you must, but only when you must, and never
 to save the same address more than once. That is, upon entering a
subroutine, save the return address if you intend to call other
subroutines. When you're finally ready to return, then un-nest.

Obvious? Perhaps. But it's usually done wrong! Sometimes the
problem only arises with recursive subroutine calls; depending on
hardware. It always arises with re-entrant programming.

So we can get in and out of routines and subroutines. How do we
pass parameters? Again, there are as many answers as computers,
languages and programmers. We shall standardize: you pass what you can
in registers; the rest via a push-down stack.

It is extremely important for routines to be able to communicate *efficiently*.
 I hope you are aware of the cost of a FORTRAN subroutine call. I
consider it a basic flaw in the language. We will be moving among so
many subroutines that failing to minimize overhead could easily halve
our running speed.

You must also consider the value of a subroutine. It isolates a
logical function and it eliminates repeated instructions. The first is
acceptable only at minimal cost. The second only if space is saved: a
1-instruction subroutine is ridiculous; a 2-instruction must be called
from 3 places to break even. Be careful!

Finally, it is important to use registers efficiently. Assign
registers for specific purposes and use them consistently. Re-assign
registers if you must to avoid conflicts. Do not move data from one
register to another; see that it is where it belongs in the first place.

When I say register, I'm obviously thinking assembler. However,
you will have to simulate the function of registers with subscripts,
etc. in other languages, and the same considerations apply.
