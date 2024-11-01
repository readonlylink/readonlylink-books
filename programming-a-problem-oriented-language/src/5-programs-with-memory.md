---
title: Programs with memory
---

You may perhaps grant the value of a program that grows, without being
willing to provide the volume of input required. Naturally it does
little good to have a hundred dictionary entries if you must type every
one. Obviously we need a place to save entries and obviously that place
is disk (or drum, or other random secondary memory).

What is not obvious is how to store entries. It ought to be a Second
Principle that you never save anything on disk without being able to
modify it, but this rule is universally ignored. To simply copy
dictionary entries violates another cardinal principle: never store core
 address on disk. You could never modify your program without chasing
down all code addresses.

Fortunately there is a solution. Store on disk the text from
which dictionary entries are constructed. It is a simple matter to
divert the input routine from reading your message buffer to reading
disk. This chapter will show how.

## 5.1 Organization of disk

There is only one way to organize disk. In the same way that core is
divided into a large number of words, disk must be divided into a large
number of blocks. In the same way that words are the smallest field that
 can be fetched from core, blocks are the smallest field that can be
fetched from disk. A block contains 256 words.

A block contains 256 words because that is the size of a 1-byte
address, and because 256 4-byte words hold 1024 bytes which is the
amount of text that can be displayed on a typical scope.

However, here is another instance in which your application and
hardware must play a dominant role. Disks usually have a hardware
block-size that offers advantages. You must choose a multiple of that.
Your application may involve storing data on disk, and you must choose a
 block size useful for data as well as text. I say no less than 512
characters nor more than 1024. 128 word blocks have recently been
mentioned; fine if the words are 6 or 3 bytes (characters).

### 5.1.1 Getting blocks

In trying to anticipate the organization of a random file, certain
principles are obvious. Cross-references between blocks will probably be
 wanted. Such references are simple if they use absolute block
addresses; extremely clumsy otherwise. We may use absolute addresses if
we promise never to move a block. This means we can never pack disk. We
agree cheerfully because we didn't want to pack disk anyway.

This means that as the data in blocks becomes useless, space will
become available in block-sized holes. We must somehow re-use these
holes. Which means that we must allocate, and re-allocate, disk in
block-sized pieces.

All addresses start at 0, block addresses included (otherwise you
 find youself forever adding and subtracting 1). However we cannot use
block 0 - for anything. You will find that most addressing errors
involve block 0. If you look at block 0 from time to time you will find
the most amazing things there. You will find block 1 a useful place to
store things you need to remember from run to run. Like the address of
the first block available for re-use - none: 0. And the address of the
last block used - initially: 1.

You will want to copy disk (onto another disk, or tape) for
protection. You need only copy the nuber of blocks used, which is
usually less than half the disk capacity, or else you're pretty worried
about space. If you destroy block 1 (you will) you will have to re-load
the entire disk from your back-up. Never try to recover just block 1,
you'll end up horribly confused.

You may want to put your object proram on this disk. Fine! It
won't even take many blocks. You may need to start it in block 0 in
order to do an initial load (bootstrap). OK, but be able to re-load the
program (only) from back-up because you will destroy block 0. Only if
you destroy the block (we'll call it block 1) containing available space
 information must you re-load data (all data). Unless you destroy many
blocks. Choose the path of least confusion, not least effort. Re-loading
 disk will confuse you, you'll forget what you've changed and be days
discovering it. Much better you spend hours re-typing text and
re-entering data.

So when you need a block, you type a word (GET) which reads block
 1, places the block up for re-use on the stack, reads that block,
places the contents of its first word into block 1, and re-writes block
1. The first word, of course, contains the address of the next block up
for re-use. If no block was availabe for re-use (initially the case),
GET increments the last block used, puts it on the stack and re-writes
block 1. GET then clears your new block to 0 and re-writes it.


Several comments: Notice that GET places its result on the stack -
 the logical place where it is available for further use. Notice that
blocks are re-used in preference to expanding the disk used. This makes
sense except for the problem of arm motion. Forget arm motion. You just
have to live with it. This is, after all, a random memory. Don't neglect
 clearing the block to 0.

### 5.1.2 Releasing blocks

To release a block, put it on the stack and say RELEASE. It will read
block 1, extract the next block for re-use, place the stack there and
write block 1; then read the released block and place the old next-block
 in the first word. All we're doing, of course, in constructing the
chain of available blocks used by GET. Possibly the block you release is
 linked to other blocks. You must release all those, too. A convenient
way is to use the first word as a link field. Then the available block
chain is the same as any other block chain. To concatenate chains you
place the first block in block 1, run down the chain to the last block
(0 in link) and place the old next-block in that link.

Don't be tempted to maintain a count of the available blocks. Its not
 worth the trouble. If you must know, you can count the length of the
available chain.

If you have enough different kinds of blocks, it may be useful to
 store a code identifying the block in the first word (or second). You
can then examine all blocks of a certain kind. Available blocks should
have code 0.

How many blocks you can have is probably limited by the disk,
however it may be limited by the field you choose to store block
addresses in. Be careful! You can circumvent the first limit by
modifying your read subroutine to choose one of several disks. You must
re-format all your block addresses (cross-references on disk, remember)
to expand the second.

### 5.1.3 Reading and writing disk

I'm sure you know how to read disk. However, do not choose a block size
that causes the slightest difficulty: like half a block between tracks.
If you check the GET routine, you'll see that you'll need 2 blocks in
core at once. This is a reasonable minimum, it makes it easy to move
things from one block to another. However, you'll have lots of core left
 over and you might as well use it for buffering disk; especially if
access time is noticable.

You'll want a table specifying which blocks are in core: your read routine can check this table before reading.

But you should not write a block when you change it. Rather mark
it 'to be written' in the buffer table. When you come to re-use that
buffer, write the old block first. The principle is that you're likely
to change a block again if you change it once. If you minimize writes
you can save a lot of disk accesses. Of course, there is a trade-off -
if you program crashes, you may have updated blocks in core that aren't
on disk. You should be able to re-start your program and preserve the
core buffers.

Of course, multiple core buffers imply an allocation problem. A simple round-robin is as effective a scheme as any.

If you are going to scan data sequentially, you can save many
accesses by reading consecutive blocks at the same time. However it is
likely that random reads may be interspersed with these sequential ones.
 An effective solution is to store the *last* block in the
sequential area and the number of blocks somewhere for your read
subroutine. If the block isn't in core, and is within the sequential
range, it can read as many consecutive blocks as there are consecutive
buffers available. Don't attempt more than this - ie, making more
buffers available. The net effect is that you will do the best you can
with sequential blocks, subject to interfering constraints.

You will inevitably spend a lot of effort reading-writing disk. But remember the Basic Principle!

## 5.2 Text on disk

You will store a lot of text on disk - hundreds of blocks - but this is
probably a small fraction of your disk. The rest is presumably data for
your application(s).

A block that contains text (I mean text to be read and executed by
your program) contains one long character string. If the first word
contains control information, it starts in the second word and extends
until a particular word marks the end (perhaps ;S). This end word is
important because it is inconvenient to have the input routine test for
end-of-block. You quickly learn not to leave that word out.

A block that contains text should have a special name, for you
will be using it often in conversation. I have called such blocks SHEETs
 - since the text filled a sheet of paper - and SCREENs - since the text
 filled the screen of a scope. Define the word READ to save the input
address, the block and character position of the next character to be
scanned, on the return stack; and reset the input pointer to the block
on the stack and the first character position. Define the word ;S to
restore the original input pointer. Very simply you can have your
program read block 123:
   - 123 READ
However . . . there's always a however, isn't there. You must modify
your word routine to read the current block before scanning. This is
expensive but essential (of course no actual read is performed if the
block is in core), for the last word executed may have caused a block to
 be read that overlaid the block the word was read from. This can
especially occur if one screen directs the reading of others (as they
will). No other solution to this problem has been satisfactory, so
swallow the code - which need not be great.

You will find that with text on disk, the original characterization
of 'input' as low volume is strained. You will read many words and do
many dictionary searches. However, on a microsecond computer, you won't
notice it.

### 5.2.1 Text editing

Never put anything on disk you can't modify! And we haven't discussed
how you get text on disk in the first place. Do not load it from cards!
You're misdirecting your effort toward card reading, and you had to
punch the cards anyway. Type it. The definitions required to edit the
text stored in blocks (SCREENs) is simple.

You must be able to handle character strings surrounded with quotes
(4.1). Given that, I shall exhibit a text editing screen. This is a
simple example of the value of definitions. You may notice it is the
first non-trivial exmple I've given. You should be motivated by now to
give it proper attention.

Naturally, you're going to have to type these definitions twice.
Once to put them into your dictionary; again, to use them to put them in
 a screen (bootstrapping). In fact you'll probably type them many times,
 but 2 is minimum.

I'm going to exhibit an annotated copy of the EDIT screen I used
in a particular program. It uses system entries whose value may not be
clear. They are borrowed from other aspects of the application.
   - 0 C1 42 # :R RECORD
Here I am constructing a field description: RECORD is a 42 character
field starting in character 1 of word 0 of the current block
(understood). I'm using blocks that can hold 15 42-character lines; a
word has 6 characters, so that's 15 7-word lines.
   - : LINE 1 - 7 * RECORD + ;
Here I'm defining a verb that will convert a line number (1-15) to a
field address. It modifies the RECORD descriptor by changing the word
specification (low order bits). Thus line 1 starts in word 0; line 2 in
word 7; etc.
   - : T CR LINE ,C ; 
If I type 3 T - I want line 3 typed. T does a carriage return (CR),
executes LINE to compute the field address, and copies the (character)
field into the message buffer (,C).
   - : R LINE =C ;
If I type " NEW TEXT" 6 R - I want line 6 to be replaced by the text in
quotes. The leading quote puts a string descriptor on the stack. R then
executes LINE, followed by =C to store the quote string in the field.
The block will automatically be re-written, since it was changed.
   - : LIST 15 0 DO 1 +
   - CR DUP LINE ,C DUP ,I CONTINUE ;
LIST will list the entire block: 15 42-character lines followed by line
numbers. It sets up a DO-CONTINUE loop with the stack varying from 1 -
15. Each time through the loop it: does a CR; copies the stack and
executes LINE; types the field (,C); copies the stack again and types it
 as an integer (,I).
   - : I 1 + DUP 15 DO 1 -
   - DUP LINE DUP 7 + =C CONTINUE R ;
If I type " NEW TEXT" 6 I - I want the text inserted after line 6. "I"
must first shift lines 7 - 14 down one position (losing line 15) and
then replace line 7. It adds 1 to the line number, sets up a backwards
DO-CONTINUE loop starting at 14, constructs two field descriptors, LINE
and LINE+7, and shifts them (,C). When the loop if finished, it does an
R.
   - : D 15 SWAP DO 1 +
   - DUP LINE DUP 7 - =C CONTINUE "  " 15 R ;
If I type 12 D - I want to delete line 12. D must move lines 13-15 up
one position and clear line 15: It sets up a DO-CONTINUE loop from
stack+1 to 15. Each iteration it: constructs fields LINE and LINE-7 and
shifts them (=C). Then it replaces line 15 with spaces.

That's it. With 10 lines of code I can define a text-editor. It's not
 the most efficient possible, but it's fast enough and illustrates many
points: In dealing with small amounts of text, you needn't be clever;
let the machine do the work. The verb LINE is an extremely useful one;
such useful verbs are invariably an empirical discovery. The verbs ,C
and =C are the heart of the method; incidently, they only work on fields
 less than 64 characters. Notice how one definition wants to reference
another (R used by I and D; LINE used by all). Notice how I and D are
similar yet different. And notice how a few verbs eliminate a lot of
bookkeeping and let you concentrate on the problem and not the details.
