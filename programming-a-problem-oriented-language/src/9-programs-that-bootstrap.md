---
title: Programs that bootstrap
---

It's sometimes hard to appreciate how it all gets started. We have been
tacitly assuming that your computer has a compiler and that you used it
to compile your program. But how did your compiler get written? Today
the answer is certainly that it was prepared by another compiler on
another computer. We've achieved parity with the biological maxim: all
life comes from previously existing life. For practical purposes, all
programs are prepared by previously existing programs.

Although this makes life somewhat easier for compiler writers,
expecially when the target computer isn't built yet, it has a drawback.
You can never drop you ultimate dependence on the pre-existing program.
If you use a compiler that generates certain instructions, or assumes a
certain disk format, you are constrained to be compatible. Consider that
 a simple version of our program, providing it includes compiler verbs,
is perfectly capable of compiling itself. It can do this with greater
freedom than the standard compiler, but more important, you can then
discard the standard compiler.

In Chapter 1, I discussed the sad state of software quality.
Although we can prepare an excellent object program, we are obliged to
maintain it as a source program for an unhappy compromise of a compiler.
 I must admit that this is the most expedient way to get the program
started. However, I question whether it is most efficient over the long
haul of re-compiling and modifying.

Let us imagine a situation in which you have access to your
computer. I mean sole user sitting at the board with all the lights, for
 some hours at a time. This is admittedly an a-typical situation, but
one that can always be arranged if you are competent, press hard, and
will work odd hours. Can you and the computer write a program? Can you
write a program that didn't descend from a pre-existing program? You can
 learn a bit and have a lot of fun trying.

## 9.1 Getting started

First you'll have to know some things: how to turn the computer on and
off (odd hours), how to enter and display data from the console
switches, and how to avoid damaging data stored on disk. You may have to
 corner operators or engineers to discover such information; it is so
rarely of interest it doesn't get written down.

So now you're face to face with the computer. What do you do? First
an exercise. Initialize the interrupt locations in such a way that the
computer will run, will execute an endless loop, when you start it. OK?
Then modify your loop so that it will clear memory. OK? You've probably
learned a lot.

Now we're going to start for real. We're going to start building
your dictionary, even though you can't use it yet. You must choose your
entry format now; variable-sized entries are required, but you can
decide about word-size and layout. The first entry is SAVE; it will save
 your program on disk. Lacking a control loop you'll have to jump to it
manually, but at least you can minimize re-doing a lot of work. The
second entry is LOAD; it will re-load your program from disk. You may
have a hardware load button, if you can store your program compatibly
with it, fine. You might want to punch a load card, to provide initial
load otherwise. But it's always convenient to be able to re-start from
core.

The third entry is DUMP; it will dump core onto the printer. It
needn't be very fast to be a lot faster than looking with the switches.
This probably isn't a trivial routine, but it oughtn't take more than a
dozen instructions. You might want to postpone it just a bit.

So, with a couple hours work - providing you read the manual
first - you have an operating system (SAVE, LOAD) and debugging package
(DUMP). And you know a lot about your computer.

## 9.2 The roots

Lest you worry, I have gone through this process myself. I've done it
twice, actually, and I'm not describing it as I did it, but as I now
think I should have done it. So you've room for improvisation.

In a sense we're building a tree. We've now reached a point where we
can start making the roots. For a while everything will be concealed but
 we'll eventually reach daylight and start on branches.

I presume you can LOAD your program and DUMP core. It's time to
get away from the switches and use the typewriter. So set up a message
buffer from which you can send and receive text. Presumably when
awaiting text your program sits in an endless loop somewhere. Learn to
recognise that loop. You'll spend most of your running time there and
it's reassuring to know that everything's allright.

No dictionary entry is associated with message I/O. You could
define one, but we won't need it. In general we will construct entries
only when they'll be needed. We can always add any entry we need, later.

Your're doing great. Now establish the stacks, the dictionary
search subroutine and entries for WORD and NUMBER. Be very careful to do
 it right the first time; that is, don't simplify NUMBER and plan to
re-do it later. The total amount of work is greater, even using the
switches.

Now write a control loop. You might test the stack, but jump to
an unspecified error routine. And run. DUMP is still our only output
routine, but you should be able to read and execute words like DUMP,
SAVE and LOAD.

Write an entry for ENTRY, the subroutine that constructs
dictionary entries. I haven't specified the code executed for WORD,
NUMBER and ENTRY. These are subroutines, and the only time we'll use
their names is when compiling code. So they should execute code that
generates a call instruction. We haven't written that code yet.

Now define the code-entry, the word that names code; and the
deposit word, the word that places the stack in core. Now you can type
octal numbers and store them in the dictionary. No more switches. You
can also construct new dictionary entries, for code.

## 9.3 The branches

We've reached a milestone. The invisible work is done and we can have a
written record of what remains. There are many things to do and the
order not so obvious.

We've reached the position of having a source language, and we need to
be able to modify it and re-compile without re-doing everything. Here
we're forced to generate temporary code that will become obsolete, but
it will save a lot of effort.

First a READ and WRITE entry to provide disk-access to a single core
buffer. Then a simple T and R to type and replace lines of text in that
block. These entries will later become obsolete, so keep them simple.

We now need the READ and ;S verbs for screens. Specify a block number and we can read the text in that block.

Now we write screens that provide definitions, an improved
compiler, improved block handler, improved text-editor and we can
proceed with our application. We want a REMEMBER entry. We haven't
needed it so far bacause we could always reach in and reset the
dictionary manually.

I'm sure you've noticed the difficulty with modifying code in the
 root. A powerful tool is to be able to shift the dictionary in core. If
 the root doesn't use absolute addresses, define a SHIFT entry and use
it. Otherwise minimize the number of absolute addresses and define a
more elaborate SHIFT verb that adjusts them.

Be careful SAVEing your program. Keep a back-up of your old version before SAVEing a new one, just in case.
