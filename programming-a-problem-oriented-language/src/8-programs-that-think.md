---
title: Programs that think
---

The mystery of consciousness has intrigued philosophers for a long time.
 It now seems apparant that just as life is a result of complex
organization, so is consciousness. It is somehow a byproduct of complex
interactions among data. Interactions so complex they only occur in
mammalian brains.

Therefore, one way of investigating the mind is to experiment with
manipulating data. The obvious way to do this is on a computer. We now
have a program with capabilities previously unattainable. Why not use it
 in such a way as to probe the realm of 'thinking'? I don't propose that
 you become a psychobiologist. But you can have a lot of fun, and do
some really impressive things with simple extensions to your program.

I will describe a number of entries of unusual capability. If you
 have an application that can use them, or if you can create an
application to use them, by all means give them a try. However, the
Basic Principle forbids you including them without a purpose. They are
sufficiently elaborate and sufficiently specialized as to never prove
unexpectedly valuable.

I have had all the entries I describe in a single program. This
program had less than 1500 instructions so it is practical to include
everything in a single program. But I was experimenting, and never found
 an application that needed a fraction of them.

## 8.1 Word dissection

One of the most awkward characteristics of our program is that words
must be separated by spaces. Very often you'd like to suffix punctuation
 or operator without an intervening space. And we will soon add
abilities that make prefixing desirable, too.

It is not difficult to modify the word subroutine to recognise
characters other than space as terminating characters. But it is
impossible to provide satisfying generality. Inevitably, you complicate
the word subroutine unduely by considering innumerable special cases.
And you can waste much ingenuity trying to achieve generality. For
example, there are no simple rules that permit all these to be words:
   - HELLO GOOD-BY 3.14 I.B.M. -.5 1.E-3
Likewise, there are no simple rules that separate these strings into the words intended:
   - -ALPHA 1+ ALPHA+BETA +X**-3 X,Y,Z; X.OR.Y
But don't dispair! There is a general solution that can handle all these
 cases. It is expensive in time, perhaps very expensive. But it solves
the problem so thoroughly, while demonstrating that no lesser solution
is possible, that I consider it well worth the price. Besides, the speed
 of processing text is not a critical factor. We maximize speed
precisely so that we can afford extravagances such as this.

If you haven't already guessed: We read a word terminated by a space,
 search the dictionary, convert it to a number. If it isn't a word by
this defintion, we drop the last character and try again. Eventually we
strip off enough characters so that the remainder is a word.

Let me review the cost. We do as many dictionary searches (plus
numeric conversions) as there are letters to be dropped. This encourages
 fast searches and quick recognition of non-numbers. It also encourages
minimizing the length of strings that must be dissected. But let's be
practical: The number of occassions when dissection is convenient are
few enough that you can afford the price. With the exception of compiler
 source code. But I'm not writing a compiler, and if you are you can
probably make your word subroutine cope.

There are several things to be careful of: As you drop characters
 from the aligned word, you must keep track of your current position
within this word. However, you must also back-up the input pointer so
that you can start the next word correctly. Incidently this requires an
initial back-up over the terminal space that is not repeated.

Backing the input pointer is not possible with unbuffered input.
This is why I suggested that you buffer un-buffered devices back in
Chapter 3. If you aren't going to dissect, apply the Basic Principle.

You must also have a way to detect that you have dropped the last
 character: a counter is one solution. Another is to place a space
immediately ahead of your aligned word, and to stop on the space. I
prefer the second, for I find I lack a convenient counter that is
preserved over dictionary search and numeric conversion. But this means
that I must fetch each character before I deposit a space over it. And
this means that my fetch subroutine must operate backwards, the only
place I ever need to fetch backwards. It depends on your hardware.

There are 2 things we can do to refine this dissection. They are
incompatible and the choice depends on your application: We don't need
to drop characters one-at-a-time. If you have several letters in
succession, or several digits, or perhaps a combination, you might drop
the all and then perform a single search/conversion. This means that you
 must examine each character (which suggests the second termination
above). It also means that you must be able to distinguish alphanumerics
 from special-characters. This requires a 64-character table of
character type tailored to your particular character set and
application. If your hardware permits, you may be able to use a 64-bit
table - classic trade-off of time vs. space.

However, this means you cannot dissect letter strings and you
might want to. Plurals, for instance, can be easily accomodated by
dropping the terminal 's'. On the other hand, you can easily
mis-identify words by dissecting letter strings: I once dissected the
word SWAP: S was defined, W was defined and my error message was AP ?
Perhaps when dropping a single letter you should replace it with a dash
to indicate a word stem. Or perhaps it doesn't matter if unidentifiable
words are mis-identified.

One further caution: If you are going to dissect, you must not
discard extra characters while initially aligning the word. Your input
pointer must be positioned so that you can backspace it correctly. If
you exceed maximum word size, stop immediately and supply a terminal
space. This means that no single word can exceed maximum size, which has
 now become maximum string size.

Another optimization has to do with the size of words in the
dictionary. If you only match part of the word, you may as well start
dropping characters at that point, if number format permits.

What does word dissection mean to a program? How does it help it
'think'? It means that your program can read your mind. It means that no
 matter how you type something, the computer will extract the meaning
you intended. It will use the meaning of the longest character string it
 can, consistant with a left-to-right scan. It's not infallible: if you
define +1 and then type +1000 it will mis-understand. But if you use
your language consistantly, it will follow.

I would like to be able to say that this ability will impress
people. It will impress you - at least it should. But ordinary people,
like your boss, expect this kind of ability from computers. They are
only impressed, negatively, if they discover its absence.

## 8.2 Level definitions

I am embarrassed not to know the standard terminology for what I am
going to discuss. I have never heard it discussed and I have never
searched for it. But it must be a standard aspect of compiler writing -
discussed in courses dealing with compilers. If you know the
terminology, you also know most of what I'm going to say: although I
hope I can get you to stretch its application.

Our arithmetic operators have found their arguments already on the
stack. Conventional algebraic notation uses such operators as infixes,
and a left-right scan provides only 1 operand when the operator is
discovered. Consequently the operation must be deferred until the other
operand is available.

Moreover, we have a hierarchy of operations than control when that other operator becomes available. For example:
   - A+B*C
the multiply must be done before the add. Moreover, parentheses are used to modify the standard heirarchy:
   - A*(B+C)
Such a notation is competely equivalent to ours. It offers no advantages
 over the operands-preceeding-operator and has some limitations. But
people are accustomed to it and negatively-impressed by its absence. So I
 will show you how to provide the capability.

However there is no reason to restrict our attention to the customary
 arithmetic and/or logical operators. I will show you some other similar
 heirarchies. The capability I describe will handle them all.

Let us establish a new kind of dictionary entry. It is identical
to a definition except that it has a number appended, a level number. So
 let's call it a level-definition. The rule is that a level-definition
is not to be executed when it is encountered, but rather placed on a
push-down stack. It will be executed when another definition with a
equal or smaller level number is encountered.

A convenient format for level-definitions is:
   - 2 :L word . . . ;
The 2 is the level number, taken from the stack. :L declares the next word as a level-definition. ';' marks the end.

Let's talk about + and *:
   - 0 :L , ;
   - 1 :L + + ;
   - 2 :L * * ;
We have re-defined them in terms of their old definitions, but as
level-definitions. We defined ',' to have some way to stop. Now we can
say:
   - 3 + 4 * 5 ,
What happened? 3 goes onto the parameter stack, + goes onto the
level-stack, 4 onto the parameter stack, * onto the level-stack (since
it has a higher level number than the + already there), 5 onto the
parameter stack. Now ',' forces the * to be executed (since its level
number is smaller) and * finds 5 and 4 on the parameter stack. ',' also
forces + to be executed (with arguments 20 and 3) and then, because its
level number is 0, is itself executed and does nothing.

Clear? I would like to assume you're familiar with this technique,
but I don't quite dare. All I'm really contributing is a way to
implement with dictionary entries a technique usually built into
compilers. Perhaps the cop-out of suggesting you define the arithmetic
operators and work out some examples for yourself. Remember that *equal* level operators force each other out, and that a *lower* level operator forces out a higher. It is strangely easy to reason out the relative levels of operators incorrectly.

What do we have so far? Why should you be interested in
level-definitions? You've seen a couple, their definitions are simple.
Level-definitions tend to be simple compared to ordinary definitions.
But given level-definitions you can write a compiler, for any language!
Level-definitions are necessary and sufficient to implement any
context-free grammer, not only the LR-1 grammers at the base of
contemporary languages. Frankly I don't know what to do with the power
they provide, but I'll toss out some suggestions later.

Now back to work. You've seen some level definitions. I hope
you've played with them some. How do we implement them? Well we don't.
Rather we implement a generalization: level-entries. When I found an
application for level-entries I also found out it was cheaper to
implement level-definitions as such than the way I was doing.

Every dictionary entry may be considered a virtual-computer
instruction, as discussed in Chapter 5. We consider a level-entry an
instruction whose execution can be delayed - after the fashion of a
level definition. Why not? A definition is, after all, only a particular
 sort of instruction. If it may be profitably delayed, so might other
instructions.

I'm sorry if it seems complicated. It is! It's going to get more
complicated - you aren't getting something for nothing. But it's worth
it. However, notice that everything we're doing now builds on everything
 we've done before. Notice that the concept of a special sort of entry
depends on having a dictionary available; and the extension of
definitions to include level numbers depends on having definitions. We
are gradually building a tree and are in the higher branches. We might
not depend on all the lower branches, but we have to have some.

How do you execute a level-entry? Exactly the same as any other.
However, the first thing the level-entry does is execute the LEVEL
routine, to give it a name, with its level number as parameter. LEVEL
tests this level number against the level-stack. 3 cases arise:
   - It may place the level number and entry on the level-stack (higher level entry) and RETURN.
   - It may replace the top of the level-stack with this entry, and execute the old top.
   - If the level-stack is empty, and the level is 0, it will execute this entry.
All 3 cases are required!

Before actually executing an entry from the stack, LEVEL must set the
 SOURCE address to reference another routine, FORCE. You recall that
your main control loop obtains its next entry either by reading a word
and searching, or by fetching from a definition. Well here is a third
source, the level-stack. As for a definition, the old value of SOURCE
and the virtual-IC must be saved - on the return-stack.

When you finally force execution of a level-entry, you must
remember that it has already been executed, and immediately jump to
LEVEL. This re-execution must start at a different place, 1 or 2
instructions below the routine address, perhaps. Or you might include
the re-start address as a parameter, and keep it in the level-stack.

When a level-entry is done, it will RETURN and your control loop
will go to FORCE. The only way you can get to FORCE is by completing a
level-entry. Its function is to check the level stack and see if any
other entry can be forced off by the one on top. 3 cases arise:
   - It may leave the level-stack alone (higher level on top), and restore SOURCE and virtual-IC from return-stack, and RETURN.
   - It may execute the lower entry, replacing it with the top - thus dropping the level-stack.
   - If there is no lower entry, and the level is 0, it will
execute the top entry - thus emptying the level-stack. At this time it
will restore from the return-stack.
Let me emphasize the importance of the return-stack, and the necessity
of saving SOURCE. If a level-entry is in fact a definition, SOURCE will
be reset yet again. It may be some time before we return and encounter
FORCE once again. For in fact, a level-definition may occur within a
definition; and it may execute other definitions - indeed, other level
definitions. The whole process may become incomprehensibly enmeshed, and
 indeed it does in practice. But it will sort itself out. The beauty of
definitions, level-definitions particularly, is similar to that of
recursive functions. You need consider only the simple case when making
the definition; complex cases take care of themselves.

Now you should be able to implement level-entries, definitions among them. What can you do with them?
   - You can define the customary arithmetic operations: + - * / MOD **.
   - You can define the customary logical operations: OR AND NOT IMPL.
   - You can define infix relations: = &lt; &gt; &lt;= &gt;= /=.
   - You can define an infix replacement: = := (one that works in either direction).
   - You can define all the above.
It depends on your application.
   - You can define words like PLUS MINUS TIMES DIVIDED-BY EQUALS; an English language arithmetic.
   - You can define phrases like MOVE . . TO . . or DIVIDE . . INTO . . or ADD . . TO . . A COBOL language arithmetic.
But let me mention 2 particular uses:
   - Consider a statement with the form
      - IF relation THEN statement ELSE statement ;
   Define IF so it will be forced out by THEN and generate a conditional
 branch. Define THEN so it will be forced out by ELSE and fix-up the
adddress left dangling by IF. Define ELSE so it will first generate an
unconditional branch, then force out THEN, and then await being forced
out itself. Define ; to force out ELSE and fix-up the forward branch.

With a few statements you can implement any such compiler construct.
   - Consider a statement like
      - 1800. FT / SEC ** 2
   Define a kind of entry UNIT that puts a constant on the stack
immediately and acts like a multiply when it's forced to. Define / to
put a 1. on the stack immediately and divide when it's forced to. Define
 ** as an infix, and FT and SEC as UNITs.
This expression and any others you construct will be evaluated correctly.

I pass the ball to you. If you have an application that could profit
from a natural language input format, you have the capability with level
 definitions to implement it. For example, it would not be hard to teach
 your program to solve the problems at the end of a high-school physics
text.

Keep in mind, that level-entries do not enhance the power of the
computer. They merely let you specify instructons in what, to the
computer, is an unnatural order. You are well advised to get your
application working, and *then* to append a fancy control language.

How does this relate to a program 'thinking'? Solely by deferring
 to the human-oriented format of control languages. Not even this is
impressive to anyone but us! And even how impressed are you by FORTRAN's
 expression evaluator any longer?

## 8.3 Infinite dictionary

I would guess that your dictionary will average several hundred entries.
 Even a small amount of data seems to generate a large number of fields -
 to mention one source. However some applications need much larger
vocabularies. Perhaps you need to identify one of 10,000 customers; or
maybe you want the symbols for 104 elements available; or the names of
1000 approved additives.

Clearly such volume must be stored on disk. Also clearly, you don't
want to have to search disk explicitely. There is a gratifyingly
effective solution: If you can't find the word in the core dictionary,
and it's not a number, search a block on disk. Now the question reduces
to: Which block?

Establish a field called CONTEXT. Treat it like you did a block
address: it both identifies a block and suggests where it might be in
core. Search this block. By changing CONTEXT you can search different
disk dictionaries. By linking several blocks together, you can search
larger amounts of disk; or search several dictionaries in sequence.

You can afford to search a fair amount of disk, because if you
can't find the word you're going to generate an error message. A delay
in typing that message to make sure you can't find the word, is an
excellent investment. Still for really large vocabularies - thousands of
 entries - such an approach is inadequate.

For very large dictionaries, scramble the word into a block
address and search that block. By that I mean compute a block address
from the letters in a word, just as we did for multiple chains in the
core dictionary, though you'll probably want a different algorithm. You
can search one of a thousand blocks and be assured that if the word is
anywhere, it's in that block. Because you used the same scramble
technique to put it there as you use to find it. Since many words will
scramble into the same block, you of course search for an exact match.
Again, just as in core.

With such a large disk dictionary, you want to be careful of several
things. First, once you choose a scrambling algorithm you can never
change it; so make a good choice before you define lots of entries.
Second, try to keep the number of entries roughly the same in all
blocks; and roughly equal to half the capacity of a block - to
compensate for the first "roughly". Or else provide for overflow by
linking blocks together.

Such a disk dictionary can be really impressive - even to
non-computer folk - because you have fast access to a prodigous
vocabulary. Fast means you can search tens-of-thousands of entries in a
single disk access.

What do disk dictionary entries look like? I have found that 2
fields are sufficient: the word field, the same size as the core
dictionary word field; and a parameter field, 1 word long. If you find a
 match on disk, you put the parameter on the stack. Remember that you
can't afford to store absolute addresses on disk, so you can't have an
address field as in core. You could provide a coded address field, but
it seems adequate to treat disk entries as constants.

For instance you can name blocks. When you type the name of a
block its address is moved from the parameter field onto the stack. That
 is an excellent place for it, because if you type the block number
itself that's where it would be placed. You can use block numbers and
block names interchangeably. Thus when you type an account number the
block associated with that account is placed onto the stack, whereupon
you store it into the base word that its fields reference. An illegal
account will cause an error message, in the ordinary way. Or you might
name the instructions for your computer. Then typing its name will place
 a 1-word instruction on the stack, ready for further processing.

Although I spoke of account numbers, notice that you can't number
 blocks. That is, the name of a disk dictionary entry cannot be a
number. For if you type a number it will be converted onto the stack,
and never sought on disk. And you must attempt to convert *before*
 searching disk or you'll search disk for every literal you type. But
then "numbers" often don't look much like the numbers defined by NUMBER.
 They tend to have embedded dashes, letters and such; or else you can
prefix a letter or suffix a # character.

How do you put an entry on disk? A special defining entry:
   - 0 NAME ZERO
analogous to CONSTANT. Alternatively you might set a flag and let the
dictionary entry subroutine decide whether to use disk or core. This
latter is preferable if you have several dfferent kinds of entries that
might go either to disk or core.

You will also need a way to forget disk entries:
   - FORGET ZERO
FORGET must call WORD as defining entries do, since this is a
non-typical use of the word ZERO. When it finds the entry, it simple
clears it without trying to pack. Your entry routine should first search
 disk to see if the word is already there. You don't want multiple
definitions on disk, even though there're useful in core. Then it should
 search for a hole. If it finds the word already there, or if it can't
find a hole? You guessed it, an error message.

Let's talk about a refinement. With a thousand names on disk it's
easy to run out of mnemonics. Let's re-use the field CONTEXT: after you
scramble the word into a block address, add the contents of CONTEXT and
search that block. If CONTEXT is 0, no difference. But if CONTEXT is
non-zero, you're searching a different block. If CONTEXT can vary from 0
 to 15, you can have 16 different definitions of the same word. You'll
find the one that had the same value of CONTEXT when you defined it. If
there is no entry for a word under a given CONTEXT, you won't get a
match. A block containing a definition for the same word under a
different CONTEXT won't be searched.

For example, stock numbers might look the same for different
sales-lines. By setting CONTEXT you can distinguish them. You can use
the same name for a report screen that you use for its instruction
screen; distinguish them by CONTEXT. If you're scrambling anyway, you
may as well add in CONTEXT (modulo a power of 2); it costs nothing, and
vastly extends the universe of names. In fact, you can use CONTEXT in
both the ways we've discussed, simultaneously. For as an aditive
constant it tends to be small; and as a block number, large. So your
search routine can decide whether to scramble or not based on its size.

A problem arises if you plan to dissect words. All those extra
dictionary searches are augmented by disk searches and their attendant
disk accesses. Several solutions are possible: Scramble with only the
first couple of characters, so at least the disk searches are in the
same block - which will be in core. Or use only non-zero values of
CONTEXT and let 0 inhibit the disk search. That is, make dissection and
disk searching mutually exclusive. As is often the case, the problem is
serious only if you aren't aware of it.

## 8.4 Infinite memory

Of course you can't really have infinite memory. Not even unlimited
memory. But you can access directly the entire random memory available
to your computer. A small augmentation of the field entries introduced
in Chapter 4 will do it. I postponed the discussion to here because it
has no particular connection with output, and because it's impressive
enough to relate to 'thinking'.

The problem of what to do with infinite memory, I leave up to you.
You will have to organize it somehow. Examine different parts of it,
move fields around, what you will. All I can do is show you how to
eliminate any explicit reference to disk.

Let's include in our field a parameter that points to a disk
address. The field is assumed to be relative to that address; that is,
contained in the disk block. The program will automatically read the
block to obtain the field. Of course a number of fields will point to
the same block address.

Before you start objecting, let me rush on. Stored with the block
 address is the location of the core buffer that block last occuppied.
So the program needn't actually read disk, or even search core buffers
for the block, unless the block has been overlaid. Hence repeated
accesses to the same block cost little.

Several trade-offs are involved: You should have a generous
number of core buffers to minimize overlays. You should choose you block
 size with this use in mind. Accessing such disk-resident fields is
slower than if you deliberately read the block into a fixed location and
 access it there; but the ease with which you can address data scattered
 on disk, and the beauty of being able to forget that some data is on
disk and other data in core, to me make up for the loss in speed.
Besides, it's your problem to implement the feature in a way that is
efficient.

Suppose you want to scan a portion of disk. All you have to do is
 define the fields and establish a loop: start with the first block
address, store it in the base location where the fields expect it and
increment it each time through the loop. All right, your advantage is
marginal. All you save is a read instruction. But if that block links to
 another one, all you need do is store the link in the base location for
 other fields, and forget that a link is involved. If you access fields
in the link it will automatically be read. If not, it won't be. The more
 complex your data, the greater the advantage.

Of course, you don't have to worry about writing blocks either.
Chapter 6 talked about flagging blocks that need writing, rather than
writing them immediately. Pays off here! If you change a field, its
block will be re-written; if you don't, it won't. Just make sure that
when you say GOOD-BY your program writes all changed blocks.

You can make these field entries identical with those accessing
core, by making the pointer to the base address 0. If you don't point to
 a disk address, you must mean core.

Notice that this addition of a base to a field entry defines a
data structure very much like the levels in COBOL's data division: 01
level corresponding to the disk address; 02 levels to the fields
themselves. For a few extra instructions you can add higher levels: If
the pointer does not reference a disk address, but another field
description, you have the equivalent of 03 level, etc.

Consider how the field reference actually works. In the field
entry you have a word parameter that tells which word the field is in
(or starts in). If this field references another, you add the word
parameters together. When you find the core address of the disk block,
you add the word offset and voila': you have the word you want. Going
through intermediate fields has little advangage unless the intermediate
 fields change. Why not? By incrementing a base field address, you can
access different rows of a matrix or different records in a block. Or
you can access different sub-records of a record. Very useful! It's
enough to make me think COBOL is a pretty good language. Of course you
can do the same thing with core fields, you just never point to a disk
address at the very end.

A word of warning! Don't try to gain efficiency by putting the
core address of a block in an index register. It's too hard to keep
track of which block, if any, the index is currently identifying. You
simply have to go through a fair bit of code to provide useful
generality. Of course, you hardware might have some special features:
maybe microprogramming? Even indirect addressing might be helpful.

Given such elaborate addressing capabilities, you can use some
help debugging your screens. Memory protection is easy to provide, and
very helpful. Include with each field entry a maximum size (in words)
for that field. When you calculate an address that purports to be in
that field, make sure it is. The upper limit for the final block
reference is of course the block size. The upper limit for a core
reference is also known. A simple error message stating OVERFLOW will
catch trouble before it has a chance to propagate.

You might want to implement an additional kind of field entry.
This one has a link. If you make a reference that lies outside the
field, it will follow this link and attempt to satisfy your request. In
particular, a record entry that points to a block: If you increment the
record offset beyond the end of the block, you can pick up a link from
the block, change the block address, reset the record offset and access
the new, overflow block. Automatically! This makes for a very attractive
 implementation of variable length records (actually blocks), providing
the records are composed of fixed-length pieces.

If you want such an overflow capability, you must provide a way
of constructing the links. You need an entry that will search a block
(chain) for a record-size hole - of course all holes are the same size.
If you can't find a hold, you must GET a new block, link it, and then
you have a block full of holes. A hole should be identified by a 0 in
the first word, character or bit, so that when GET clears the new block
to 0, all record positions are empty. Naturally you have no guarantee
that overflow blocks will be near each other. Almost certainly they
won't be. Either you don't care, or you initially allocate each block
chain sequentially, up to mean size.

It is easy to remove a record. You create a hole by storing 0 in
the first word. It is hard to discover whether by doing this you have
caused an empty block which can be un-chained. Unless you expect your
data to shrink and need to recover space, don't bother. How can data
shrink? Also, don't move records around - to squeeze out holes perhaps.
Just as we want to use absolute block addresses, we want to use absolute
 record addresses (if we use record addresses at all).

So, we can have automatic access to fields scattered all over disk and in variable size records at that. Basic Principle!

One thing! If field entries can address other field entries, you
need some way to distinguish a field from a disk address. I have no
suggestion.
