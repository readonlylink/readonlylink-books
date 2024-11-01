---
title: Programs that grow
---

So far our dictionary has been static. It contains all the entries you
need - placed there when the program was compiled. This need not be. We
can define entries that will cause additional entries to be made and
deleted. Let me point out why this might be desirable.

You have a program that controls an application. Based upon the words
 you type, it will do as you direct. In Chapter 3 we provided the
ability to type out results. Not the sort of results that are the
inevitable result of the application, but variables that you'd maybe
like to see. More a conversational sort of output, since it is
controlled directly by input.

There are 2 problems with this situation. Firsts, to add an entry
 to your dictionary you must re-compile the program. Clearly, you won't
be adding many entries - but maybe you won't have to. Second, all your
entries must be present at the same time. This creates, not so much a
volume problem, as a complexity problem. If your application is complex,
 it becomes increasingly difficult to make all aspects compatible. For
instance, to find distinct names for all fields. Third, if you find an
error in an entry you must recompile the program. You have no ability to
 correct an entry - though of course you could define entries to provide
 that ability.

If you can create dictionary entries you can accomplish 2 things:
 You can apply your program to different aspects of your application -
without conflicts and reducing complexity. You can create a dictionary
entry differently, and thus correct an error. In fact, the purpose of
your program undergoes a gradual but important change. You started with a
 program that controlled an application. You now have a program that
provides the capability to control an application. In effect, you have
moved up a level from language to meta-language. This is an extremely
important step. It may not be productive. It leads you from talking *to* your application to talking *about* your application.

Another way of viewing the transition is the entries in your
dictionary. At first they were words that executed pieces of code that
constituted your application program. A purely control function. Now
they tend to become words that let you construct your application
program. They constsitute a problem-oriented-language. The distinction
need not be abrupt but it is irreversible. You change from an
application to a system programmer - your system being your application.

I hesitate to say whether this is good or bad. By now you surely
know - it depends on the application. I suspect any application of
sufficient complexity, and surely any application of any generality,
must develop a specialized language. Not a control language, but a
descriptive language.

Some examples: A simulator does not want a control language. It
is important to be able to describe with great facility the system being
 simulated. A linear-programming problem needs a language that can
describe the problem. A compiler actually provides a descriptive
language for use with the programs it compiles. A compiler-compiler
describes compilers. What is a compile-compiler that can execute the
compiler it describes and in turn execute the program it compiled? That
is the question!

Let me now assume that you have a problem that qualifies for a descriptive language. What dictionary entries do you need?

## 4.1 Adding dictionary entries

Let us now assume that you want to expand your dictionary; that you have
 a sufficiently complex application to justify a specialized language.
How do you make a dictionary entry?

Recall the control loop: it reads a word and searches the dictionary.
 If you want to define a word, you must not let the control loop see it.
 Instead you must define an entry that will read the next word and use
it before RETURNing to the control loop. In effect, it renders the
following word invisible. It must call the word subroutine, which is why
 it is a subroutine rather than a routine. Let us call such an entry a *defining* entry, its purpose is to define the next word.

In principle we only need one defining entry, but we must supply
as a parameter the address of the code to be executed for the entry it
defines. Remember that 4 fields are required for each entry: the word,
its code address, a link, and (optionally) parameters. The word we
obtain from the word subroutine; the link we construct; the parameters
we take from the stack. We could also take the address from the stack,
but it's more convenient to have a separate defining word for each kind
of entry to be constructed. That is, to have a separate defining entry
for each address we need, that provides the address from its parameter
field.

I'm afraid this is confusing. We have one entry that supplies the
 address field of a new entry from its own parameter field. Let's take
an example; suppose we want to define a constant:
   - 0 CONSTANT ZERO
0 is placed on the stack; the code for the word CONSTANT reads the next
word, ZERO, and constructs a dictionary entry for it: it establishes the
 link to a previous entry, stores 0 from the stack into the parameter
field, and from its own parameter field stores the address of the code
ZERO will execute. This is, presumably, the address of code that will
place the contents of the parameter field onto the stack.

Thus for each kind of entry we will be making, we need a defining
entry to supply the code address and do the work. Since all defining
entries have much in common, you should write an ENTRY subroutine they
can call. It should have as parameter the code address, and construct
all of the new entry except the parameter field, which is specialized by
 the defining entry.

Other defining entries might be:
   - 0 INTEGER I - an integer-size parameter field is initialized to 0; its address will be placed on the stack.
   - 1. REAL X - a floating-point parameter field is initialized to 1.
   - 8 ARRAY TEMP - an 8 word parameter field is cleared to 0; the address of its 1st word will be placed on the stack.
I must emphasize the word "might". Different applications will require
different defining entries; even the same word might act differently for
 different applications. But you are in a position to define any kind of
 noun you need, and then create as many instances of that noun as you
like. It is a futile exercise to attempt to establish a universal set of
 nouns. Compiler languages have repeatedly stumbled by not providing
enough, and no matter how many they provide, someone will want one more.

For example, you might define the following noun:
   - 0 8 INDEX J - J is defined to be an index, that varies from 0 to 8. When executed, it adds its value to the top of the stack.
If you then define appropriate verbs to advance, test and reset J, you can have a powerful indexing facility. Or define:
   - 3 VECTOR X  3 VECTOR Y  9 VECTOR Z
and define arithmetic verbs to implement vector arithmetic:
   - X Z = Z Y + add X and Y, store in Z.
   - X Y Z *C multiply X and Y (outer product), store in Z.
Anything you need for your application you can define. But you can never define everything. Basic Principle!

## 4.2 Deleting entries

So far we've only discussed defining nouns. Actually you'll be using
more verbs than nouns, but they require much longer explanations. Here
is one kind of verb.

If you can add entries to your dictionary, eventually you're going to
 want to get rid of them. You'll need to delete entries in order to
re-enter them correctly, or delete entries in order to make room for
another application. After all, your dictionary is finite; no matter how
 large you make it, you will be aware of its upper limit. Parkinson's
Law may be rephrased: Dictionaries expand to fill the available space.

There is only one feasible way to delete entries. That is to
delete all entries after a certain point. If you were to delete specific
 entries, you would leave holes in the dictionary, since it occupies
contiguous core. If you attempt to pack the dictionary to recover the
holes, you are faced with a wicked re-location problem, since we use
absolute addresses. To avoid absolute addresses is inefficient and
unnecessary.

Deleting trailing entries is a completely satisfactory solution. I
 know of no argument to prove thie, except to say try it and see. You'll
 find that, in practice, you add a bunch of entries; find a problem;
delete those entries; fix the problem; and reenter all the entries. Or
you fill your dictionary for one application; clear it; and re-fill with
 another application. Or you might re-load the same application just to
clear some fields. In each case, you want to get rid of all the latest
entries.

One exception is when you use some entries to construct others.
The constructing entries are then no longer needed, and there is no way
to get rid of them. It happens; I may even give some examples later. But
 all you lose is dictionary space, and I can't see a practical solution.

OK, how do you delete trailing entries? You want to mark a point
in your dictionary and reset evereything to that position. One thing is
the dictionary pointer that identifies the next available word in the
dictionary. That's easy. However you must reset the chain heads that
identify the previous entry for each of your search chains. It only
takes a small loop: follow each chain back, as you do when searching,
until you find a link that preceeds your indicated point.

If you have fixed-size entries, you must reset the pointer to the parameter area, but you don't have to follow links.

A convenient way to specify the point you want to delete from is
to place a special entry there. A verb that will delete itself and
evereything following it when you execute it. For example,
   - REMEMBER HERE
is a defining entry. When you type HERE, it is forgotten; it both marks a
 place in the dictionary and executes the deleting code. HERE doesn't
need a parameter field, unless you use fixed-length entries, whereupon
it must save the current value of the parameter pointer. This is our
first example of a verb-defining entry.

## 4.3 Operations

Recall that the stack is where arguments are found. There are some words
 you may want to define to provide arithmetic capabilities. They are of
little value to a control language, but essential to add power to it.
I'll use logical constructs TRUE (1) and FALSE (0). And remember the
definition of top and lower from 3.6.

Unary operators: change the number on top of the stack.
   - MINUS changes sign of top.
   - ABS sets sign positive.
   - ZERO if top is zero, replace it with TRUE; otherwise place FALSE onto the stack.
   - NONZERO if top is nonzero, place TRUE onto the stack; otherwise leave it alone (leave FALSE on the stack).

Binary operators: Remove top from the stack and replace lower by a function of both.
   - + add top to lower.
   - * multiply lower by top.
   - - subtract top from lower.
   - / divide lower by top, leave the quotient.
   - MOD divide lower by top, leave the remainder.
   - MAX if top is larger than lower, replace lower by top.
   - MIN if top is smaller than lower, replace lower by top.
   - ** raise lower to power of top.

These are only samples. Clearly you are free to define whatever words
you feel useful. Keep in mind that you must place the arguments on the
stack before you operate on them. Numbers are automatically placed on
the stack. Constants are too. Thus the following make sense:
   - 1 2 +
   - PI 2. *
   - 1 2 + 3 * 7 MOD 4 MAX
   - 1 2 3 + *

This notation permits arithmetic calculation in the same manner a desk
calculator. It is often called parenthesis-free representation or
perhaps right-handed Polish, but it is simply the way you work with
arguments on a stack. Conventional algebraic notation is much harder to
implement (8.2).

Other binary operations are the arithmetic relations: these leave a truth value on the stack:
   - = are they equal?
   - &lt; is top greater than lower?
   - &gt; is top less than lower?
   - &gt;= is top not greater than lower?
   - &lt;= is top not less than lower?

The logical operations include a unary and several binary:
   - NOT if top is FALSE, replace with TRUE; otherwise replace with FALSE.
   - OR logical or.
   - AND logical and.
   - IMP logical implication.
   - XOR logical exclusive or.

Your stack must have a fixed word-length. However the operations
mentioned above might apply to several kinds of numbers: integers,
fixed-point fractions, floating-point fractions, double-precision
fractions, complex numbers, vectors of the above kinds. The truth values
 are only 1 bit. Clearly, the stack must be able to hold the largest
number you expect to use. Less clear is how you should distinguish among
 various kinds of numbers.

One way is to define separate operations for each kind of number:
   - + integer and fixed-point add (they are the same).
   - +F floating-point add.
   - +D double-precision add.

Another is to make a stack entry long enough to contain a code
identifying the kind of number. This makes the code defining each
operation more elaborate and raises the problem of illegal arguments. I
recommend not checking arguments and defining separate operations, for
reasons of simplicity. Actually, you are working with one kind of number
 at a time and the problem may never arise.

Do not bother with mixed-mode arithmetic. You never *need* it,
 and it's not convenient often enough to be worth the great bother. With
 multiple word numbers (complex, double-precision) you may put the
address of the number on the stack. However, this leads to 3-address
operations with the result generally replacing one of the arguments. And
 this, in turn, leads to complications about constants.

In general, the number of things you *might* do with numbers increases indefinitely. Many of these are mutually incompatible. Basic Principle!

## 4.4 Definition entries

I must now describe an entry more complicated than any so far, though
not the most complicated that you'll see. It is also exceptional in that
 it's not optional. For this ability is required for any effective
application language: to be able to define one word in terms of others.
To abbreviate, if you will. You recall that I characterised words as
being simple in themselves, but powerful in combination. Well here is a
way to combine words.

A definition consists of a defining entry ":" followed by a series of
 words terminated by ";". The intention is that the word defined by ":"
has the meaning expressed by the words that follow. For example:
   - : ABS DUP 0 LESS IF MINUS THEN ;
This is a definition of the word ABS. Its purpose is to take the
absolute value of the number on the stack. It does this by executing a
series of words that have the proper effect.

You may consider this a rather clumsy definition of ABS. Especially
since there is an instruction on your computer that does exactly that.
you're quite right, definitions tend to be clumsy. But they let us use
words that we hadn't the foresight to provide entries for. Given certain
 basic words we can construct any entry we need. Definitions provide a
succinct distinction betwen a control language and an application
language: The control language must have all its capabilities built in;
the application language can construct those capabilities it needs.

To implement definitions is simple, yet awkwardly subtle. The
parameter field of a definition contains the addresses of the dictionary
 entries that define it. You must somehow deposit these entries in the
parameter area, and later fetch them when you execute the definition.
The complementary processes of definition and execution are more
involved than for any other entry we've encountered.

Before I describe these processes in detail, let me try to
clarify exactly what a definition is. You recall that the code executed
for a word is a routine, and not a subroutine. And yet a series of words
 is like a series of subroutine calls, for the control loop serves the
function of returning to a position where the next word can be found.
You might consider a definition to be just that: a series of subroutine
calls with the addresses of the subroutines constituting the definition.

Another viewpoint is concealed in an abbreviation I use: I speak
of "executing a word", when I really mean executing the code associated
with the word. Or even more precisely, executing the code whose address
is stored in the dictionary entry for the word. The abbreviation is not
only convenient, it suggests that a word is an instruction that can be
executed. And indeed, it is helpful to think of a word as an
instruction: an instruction for a computer that is being simulated by
our real computer. Let's call that imaginary computer the "virtual
computer". Thus when you type words you are presenting instructions to
the virtual computer. The control loop becomes the instruction fetch
circuitry of the virtual computer.

If we extend this analogy to definitions, a definition becomes a
subroutine for the virtual computer. And the process of defining a
definition is equivalent to compiling this subroutine. We'll return to
this analogy later.

You'll see that the virtual computer is a real help in
understanding definitions. In fact, it originally led me to apply
compiler techniques to definitions - techniques that otherwise wouldn't
have occurred to me. But although it may be helpful to programmers, it
is only confusing to non-programmers. So I prefer the name "definition"
for this type of entry, and the phrase "defining one word in terms of
others" as its explanation.

Definitions are extremely powerful. Why, is hard to explain, hard
 even to comprehend. Their value is best appreciated by hindsight. You
complete a ludicrously simple implementation of an application, discover
 that you used a dozen definitions and nested them 8 deep. The
definitions appear responsible for the simplicity.

But there are several properties that emphasize the value of
definitions over their equivalent, a series of subroutine calls. First,
you needn't be concerned about call sequence, about what registers are
available and what must be saved; simply type a word. Second, one
definition can execute another. That is, you can nest definitions, again
 without any concern about saving return addresses or other register
conflicts. You can even use definitions recursively without concern.
Third, you can pass arguments among definitions effortlessly, in fact
invisibly, since they are on the stack. Again you have no concern for
calling sequence or storage conflicts. Plenty of temporary storage is
available, too; again on the stack.

Of course you have to pay for this convenience, though probably
less than you would with FORTRAN subroutine calls. The price is the
control loop. It's pure overhead. Executing the code for each entry of
course proceeds at computer speed; however obtaining the address of the
next code to execute takes some instructions, about 8. This is why I
urge you to optimize your control loop.

Notice that if the code executed for words is long compared to
the control loop, the cost is negligible. This is the principle of
control languages. As the code shrinks to control loop size, and
smaller, overhead rises to 50% and higher. This is the price of an
application language. Note, however, that 50% overhead is easily reached
 with operating systems and compilers that support an application
program.

I suggest that you compromise. Code the computation-limited
portions of your problem and use definitions for the rest. The use of
definitions to control, rather than perform, calculations is
inexpensive. And the ease of constructing them reduces the time and
effort, and thus cost, of implementation.

### 4.4.1 Defining a definition

The defining entry ":" acts just like any other. It passes the address
EXECUTE to the ENTRY subroutine. I'll discuss that code in the next
section.

It then sets a switch STATE. The control loop must be changed to test
 STATE: if it is 0, words are executed as I've already described; if it
is 1, words are compiled. Let me repeat: if you add definitions to your
program, you must modify the control loop so that it will either execute
 or compile words. If you plan to include definitions from the start,
you should plan the control loop accordingly. Implement the switch so
that executing words is as fast a possible; you'll execute many more
words than you'll compile.

To compile a word is simple. After finding it in the dictionary,
you have the address of its dictionary entry. Deposit this address in
the parameter field. Notice 2 things: we already have a mechanism for
depositing words in the dictionary. ENTRY uses it as well as many
defining entries for parameters. The dictionary pointer DP identifies
the next available word in the dictionary. All you must do to compile a
word is to store its address at DP and advance DP. Also notice that we
deposit the address of the *entry* not the address of the code
executed. This is so we have access not only to the code but also to the
 parameter field, and even the word itself should we need it.

All right, so much for compiling words. What about numbers? A
number presented to a compiler is called a literal. And literals are a
problem to any compiler. Fortunately we can define our virtual computer
so that it can handle literals in-line. You must again modify the
control loop to test STATE when a number is successfully converted.

Before showing how to compile a number, let me define
pseudo-entries. A pseudo-entry is a dictionary entry that is not in the
dictionary. That is, it has the format of an entry but it is not linked
to other entries. Thus it would never be found during a dictionary
search. You see, we occassionally need entries to permit the virtual
computer to run smoothly, but we don't want to slow the dictionary
search by including non-referencable entries.

As you've probably guessed, in order to compile a literal you
compile a pseudo-entry. You then follow it by the number itself; that
is, you compile the number also. The result is a double-length
virtual-computer instruction. The code executed for the pseudo-entry
must fetch the number and place it onto the stack. Thus literals that
are compiled have the same effect, when executed, as if they were
executed immediately.

Notice that if you have different-size literals, you'll need
different pseudo-entries for them And having brought up the subject, let
 me discuss word length a moment. Word length for the virtual computer
should be about 12 bits. This is because each instruction is composed of
 simply a dictionary address and 12 bits is enough to identify one of
perhaps 1000 entries. If your real computer word length is longer than
18 bits you should pack several virtual-computer instructions into one
word. This is possibly awkward, since you must modify DP to address
other than a real computer word. But you'll save a lot of space.

Incidently, since literals require extra space when compiled, you might define commonly used literals as words:
   - 1 CONSTANT 1
Recall that numbers may be words, since the dictionary is searched
before numeric conversion is attempted. And a word requires only a
single-length virtual-computer instruction. On the other hand, a
dictionary entry takes much more space than a compiled literal, so watch
 the trade-off.

The code in the control loop that compiles words much watch for ";".
It is compiled as usual, but it also resets STATE to prevent further
compiling. It also performs another task, which requires a digression.

Notice that when we're compiling a definition we're searching the
 dictionary for each word. If we reference the word we've just defined,
we'll find it. Thus we'll have made a recursive reference. If you want
recursive definitions, fine. However it's extrememly convenient to
exchange recursion for re-definition. That is, to understand a reference
 to itself inside a definition to refer to an earlier definition. For
example,
   - : = SWAP = ;
Here I redefine the = verb to operate upon arguments in the opposite
order. I could use a different word for that purpose, but = has mnemonic
 significance.

In any case, the capability is easy to provide. Let ":" bugger the
search so the latest entry cannot be found. And let ";" unbugger the
search and thereby activate the new definition. If you want recursive
definitions, you could provide a defining entry ":R" that did not
bugger, providing you make ";" work for both. I'll mention another
technique later.

### 4.4.2 Executing a definition

I named the code executed for a definition EXECUTE. It must modify the instruction-fetch circuitry of the virtual computer.

Recall the structure of the control loop: the routine NEXT<sub>W</sub> provides the address of a dictionary entry; the routine associated with this entry is entered; it ultimately returns to NEXT<sub>W</sub>. The same procedure is required in order to execute a definition, with the exception that NEXT<sub>W</sub> is replaced by NEXT<sub>I</sub>. Where NEXT<sub>W</sub> read a word and found it in the dictionary, NEXT<sub>I</sub> simply fetches the next entry from the parameter field of the definition.

Thus you need a variable that identifies the routine to be
entered for the next entry. One implementation is to define a field NEXT
 that contains either the address of NEXT<sub>W</sub> or NEXT<sub>I</sub>.
 If you jump indirect to NEXT, you will enter the appropriate routine.
One task of EXECUTE is therefore to store the address of NEXT<sub>I</sub> into NEXT, causing subsequent entries to be obtained in a different way.

Of course NEXT<sub>I</sub> must know where to find the next
entry. Here the virtual computer analogy is extended by the addition of
an instruction counter. If you define a field, preferably an index
register, named IC it can act exactly like an instruction counter on a
real computer. It identifies the next entry to be executed, and must be
advanced during execution.

You can now see the complete operation of NEXT<sub>I</sub>: fetch the entry identified by IC, advance IC to the next enty, and return to the same point NEXT<sub>W</sub>
 does to execute the entry (or compile it, as the case may be). If you
use definitions at all, you'll use them extensively. So NEXT<sub>I</sub> should be optimized at the cost of NEXT<sub>W</sub>. In particular, the code that executes (compiles) entries should be fallen into from NEXT<sub>I</sub> and jumped to from NEXT<sub>W</sub>. This saves one instruction (a jump) in the control loop using NEXT<sub>I</sub>. This can be 20% of the loop, apart from actually executing the entry's code, for a substantial saving.

Now let's return to EXECUTE. Clearly, in addition to establishing NEXT<sub>I</sub>
 it must initialize IC. But first it must save IC. The process is
analogous to a virtual-computer subroutine call. The obvious place to
save IC is the return stack. Although it is used for other purposes,
none of these conflict with such use. If one definition is executed from
 within another, it is clear the current IC must be saved. Otherwise the
 current value of IC is undefined.

One more routine is involved in this process. The code executed
for ";" must return from the definition. This means simply that it must
restore IC from the return stack. However it must also restore the value
 of NEXT, which was set to NEXT<sub>I</sub> by EXECUTE. You might store
the old value of NEXT in the return stack and let ";" recover it.
Simpler, perhaps, is to let the undefined value of IC be zero, and act
as a flag to restore NEXT to NEXT<sub>W</sub>. For while executing definitions, NEXT will always contain NEXT<sub>I</sub>. Only when returning from a definition that originated within the source text must NEXT<sub>W</sub> be reestablished. Since while executing source text IC is irrelevant, it might as well by useful in this limited way.

That's all there is to it. The combination of EXECUTE, NEXT<sub>I</sub>
 and ";" provide a powerful and efficient subroutine facility. Notice
that the code "executed" for a definition might actually be compiled,
depending on the field STATE, as dicussed earlier. Notice also that the
entries executed by a definition might compile other entries. That is,
one entry might deposit numbers in the dictionary, using DP. Thus
although the fields IC and DP are similar in use, DP deposits entries
and IC fetches them, they may both be in use at the same time. If you're
 short of index registers, don't try to combine them.

### 4.4.3 Conditions

Let me review briefly the process of defining a definition: The word ":"
 sets a switch that modifies the control loop; it will now compile words
 instead of executing them. The word ";" is compiled, but also causes
the switch to be reset, ending the process of compilation. Following
words will now be executed as usual.

We can thus view ";" as being an exceptional word, for it is - in a
sense - executed during compilation, at which time it resets that
switch. Of course it is also executed during execution of the
definition, with a different effect: it resets IC.

There are other words like ";" that must be executed during
compilation. These words control the compilation. They perform code more
 complicated that simply depositing an entry address. In particular,
they are required to provide forward and backward branching.

Rather than talk abstractly about a difficult and subtle point,
I'll give some examples of words that I've found useful. As always, you
are free to choose your own conventions, but they will probably resemble
 mine in their basic effects.

Define the words IF, ELSE and THEN to permit the following conditional statement format:
   - boolean value IF true statement ELSE false statement THEN continue
The words have a certain mnemonic value, though they are permuted from
the familiar ALGOL format. Such a statement can only appear in a
definition, for IF, ELSE and THEN are instruction-generating words.

At definition time, the word IF is executed. It compiles a forward
jump. Now I must sidetrack the discussion and define jumps. A jump
instruction for the virtual computer is similar to a literal. An in-line
 literal is a double-length instruction. The code executed for the
pseudo-entry comprising the first half, uses the second half as a
parameter. Likewise for jumps: a pseudo-entry uses an in-line parameter
to change the virtual-computer instruction-counter (IC). This parameter
is the amount, positive or negative, to be added to IC: positive for a
forward jump, negative for a backward jump. It is a relative jump
address, and the whole construction is used by some real computers.

Actually we need 2 jump pseudo-entries: a conditional jump and an
 unconditional jump. The conditional jump jumps only if the stack is
non-zero, and it is a destructive operation (its argument is dropped).

All right, back to IF. At definition time it compiles the
conditional jump pseudo-entry, followed by a 0. For it doesn't know how
far to jump. And it places the location of the 0, the unknown address,
onto the stack. Remember that the stack is currently not in use, because
 we're defining. Later it wil be used by those words we're defining, but
 at the moment we're free to use it to help in the process.

Now look at ELSE. At definition time it compiles an unconditional
 jump pseudo-entry followed by 0. But then it stores the current value
of DP, the next available location, into the location on the stack. Thus
 it provides the distance for the conditional jump generated by IF.
Actually it must subtract to get a relative address, but the principle
is clear. In turn it leaves the location of its address on the stack.

Finally we come to THEN. It fixes-up the address that ELSE left
dangling. That is, it subtracts the stack from DP and stores the result
indirectly in the stack; and destructively. Thus the combination of IF,
ELSE and THEN use the stack to construct forward jump virtual-computer
instructions. Since ELSE and THEN act identically in fixing-up the
missing address, ELSE can be omitted without any modification. Also
since the stack is used to store unfulfilled jumps, IF . . . THEN
statements may be nested. The only restriction is that all addresses are
 determined; that is, that all locations are removed from the stack.
This will be the case if every IF has a matching THEN; ELSE is always
optional.

Of course there's nothing unusual about this technique. All
compilers generate forward jumps in this manner. What is somewhat
unusual is applying it to the compilation of instructions for a
virtual-computer. But it seems to be the best way.

Let's consider a related construction. Very often we are faced
with logical expressions that consist of a string of ANDs or a string of
 ORs. The truth value of such expressions may be determined before the
entire expression is evaluated. You can save time by quitting once you
know the final result. For example, consider the statement:
   - a b AND c AND IF . . . THEN
where a, b, c are boolean expressions; and the statement would read in ALGOL
   - *if* a *and* b *and* c *then* . . .
If a is false, we might as well quit, since the disjunction cannot possibly be true. If you re-write the statement as:
   - a IF b IF c IF . . . THEN THEN THEN
the effect is the same; if a, b and c are all true the conditional
statement is executed. Otherwise not. Each IF generates a forward jump
that is caught by its matching THEN. Note that you must still match IFs
with THENs. In fact this is one sort of nested IF . . . THEN statement.
It is an extremely efficient construction.

Now consider the corresponding statement with ORs:
   - a b OR c OR IF . . . THEN
or in ALGOL
   - *if* a *or* b *or* c *then*
If a is true you may as well quit, for the conjunction cannot be false. If you re-write the statement as
   - a -IF b -IF c IF HERE HERE . . . THEN
and if you define
   - : HERE SWAP THEN ;
   - : -IF NOT IF ;
the statement works as follows: if a is true, -IF will jump; if b is
true, -if will jump; if c is false, IF will jump. The first HERE will
catch b's jump (the SWAP gets c's address out of the way); the second
HERE catches a's jump; THEN catches c's jump. Thus a and b jump into the
 condition, while c jumps over it.

This is a slightly clumsy statement, but I've found no simpler
solution. If you used them regularly, you'd doubtless acquire facility,
and it would seem quite natural. Just watch that you match all IFs.
Moreover the same technique could be applied to more complex logical
expressions - with even greater clumsiness.

### 4.4.2 Loops

I'll continue with a couple more examples of words executed at
definition time. This time examples of backward jumps, used to construct
 loops.

Consider the pair of words BEGIN and END, as used in a statement like:
   - BEGIN . . . boolean END
BEGIN stores DP onto the stack, thus marking the beginning of a loop.
END generates a conditional backward jump to the location left by BEGIN.
 That is, it deposits a conditional jump pseudo-entry, subtracts DP+1
from the stack, and deposits that relative address. If the boolean value
 is false during execution, you stay in the loop. When it becomes true,
you exit.

BEGIN and END provide a loop terminated by a logical condition. Let's
 define another loop. This one counts an index through a range to
control the looping:
   - a b DO . . . CONTINUE
a and b represent arguments on the stack. DO acts just like BEGIN.
CONTINUE requires a new pseudo-entry that tests the top 2 words on the
stack for equality, and jumps if they are unequal. During compilation
CONTINUE deposits this pseudo-entry and then computes the jump distance
as did END. Thus CONTINUE uses another conditional jump: one that tests
the stack for equal, instead of for false. It is also a non-destructive
operation, so long as its arguments are unequal. When they become equal
and terminate the loop, it drops them.

Presumably, inside the DO . . . CONTINUE loop the arguments are
modified so as to terminate the loop. This can be done many ways. For
example, to run the loop from 1 to 10:
   - 10 0 DO 1 + . . . CONTINUE
The first argument is 10, the stopping value; the second is 0, which is
immediately incremented to 1, the index value. Within the loop this
index is available for use. the DUP operation will obtain a copy. Each
time through the loop the index will be incremented by 1. After the loop
 is executed for index value 10, the CONTINUE operation will stop the
loop and drop the 2 arguments - now both 10.

Alternatively, the same loop could be written:
   - 11 1 DO . . . 1 + CONTINUE
Here the index is incremented at the end of the loop, instead of the
beginning. Upon reaching 11 and exceeding the limit of 10, the loop is
stopped.

Naturally loops can be counted backwards, or indeed many other
methods of modifying the index used. It will always terminate on
equality. Of course, such a flexible loop control runs the risk of never
 stopping at all. If you increment the index incorrectly, it will
happily run forever. But used carefully, it's a convenient tool.

A refinement of DO . . . CONTINUE is not difficult. If the
arguments are equal to start with, DO can generate a conditional forward
 jump that CONTINUE will fix-up. Thus you may do a loop *no*
times. However, such loops are the exception; but if you encounter one,
you'll find the conditional statement required to protect it most
awkward.

### 4.4.3 Implementation

I hope you now appreciate the need for words that are executed at define
 time. I'm sure you're aware of the need for branches and loops. Perhaps
 you'll notice that I did not mention labels; the branch generating
words I mentioned, and others you can invent, are perfectly capable of
handling jumps without labels. You saw in the definition of HERE how the
 stack can be manipulated to permit overlapping jumps as well as nested
ones. However in a sense we have many labels, for every dictionary entry
 effectively assigns a name to a piece of code.

Now to consider some problems I glossed over. Clearly you must be
able to recognize those words that are to be executed during
definitions. That is, IF, THEN, BEGIN, END, etc. must somehow override
the normal mechanism whereby the control loop would compile them. I
mentioned a switch that distinguished execution from compilation. Let's
establish a similar flag (1 bit) in each dictionary entry, with the
values
   - 1: execute
   - 0: compile
applying both to switch and flag.

For a given entry, 'or' the switch and flag together; if either is 1, execute the word, else compile it.

The above rule is correct, and even fairly efficient. Remember
that we want the control loop efficient! And it's adequate providing all
 words that must be executed are built into your system dictionary.
Unfortunately, it's not adequate for the examples I gave above, which
probably means it's inadequate, since those were pretty simple examples.
 But complication is part of the fun of programming. So pay attention
and I'll try to explain some problems I don't understand very well
myself.

> Editor: I don't understand my concern about SWAP below. The word ! did not endure. Don't try to reconcile what I said. I can't.

Consider the definition of HERE I gave above:
   - : HERE SWAP THEN ;
Here is one of those imperative words; it must be executed at definition
 time. But it is defined as an ordinary definition - and would be
compiled. Even if we managed to execute HERE, the first word in its
definition is SWAP: a most ordinary word, and one that would certainly
be compiled, except that we intend it, too, to be executed. The next
word, THEN, offers no problem - or does it? If we can execute HERE we'll
 also execute THEN, since it's imperative. However we have a problem at
the time we *define* HERE; we'll try to *execute* THEN, when we want to *compile*
 it. That is, sometimes we want to compile imperative words; and
sometimes we want to execute ordinary words - even in a definition.

So, what to do? I bet you think I have a solution. Your faith is
touching, but I don't have a very good one. It suffers a small
restriction, but a nagging one: you may not execute a literal in a
definition. To phrase it positively: literals must be compiled inside
definitions. Let's see how it works.

Consider the switch STATE. It's normally 0; ":" makes it 1 to
indicate compilation. Let's define a new defining entry ":!" that acts
exactly like ":" with 2 exceptions:
   - It sets the entry flag to 1; to mark an imperative word.
   - It sets STATE to 2; to force all words to be compiled. Since
 the test in the control loop is to execute if STATE and flag are equal,
 nothing will execute.
";" is unchanged; its sets STATE to 0 for both sorts of definitions.
This solves all our problems except SWAP. How do we execute words that
ordinarily would be compiled?

Define a new entry "!". Let it execute the last entry compiled and
remove it from the compilation. Now we can re-write the definition of
HERE as
   - :! HERE SWAP ! THEN ;
and it will work.

I'll review the rules:
   - All words are normally executed.
   - Only words flagged imperative are executed in definitions.
   - Any word can be made imperative by following it with an "!".
   - A definition can be made imperative by using ":!" instead of ":" to define it.
Now the restriction I mentioned should be apparant. A literal cannot be
made imperative with a "!" because it's a double-length instruction -
and the "!" code has no way of knowing that. Oh well, we could set a
field to indicate the length of the last compiled instruction, but it's
not that great a problem. Besides, in that case successive !s wouldn't
work.

## 4.5 Code entries

I've explained definitions and how they, in effect, compile instructions
 for the virtual-computer. What about compiling code for your real
computer then? Of course you can. But you probably won't.

The Basic Principle intrudes. If you add code entries to your
program, you add enormous power and flexibility. Anything your computer
can do, any instructions it has, any tricks you can play with its
hardware are at you fingertips. This is fine, but you rarely need such
power. And the cost is appreciable. You'll need many entries (say 10) to
 provide a useful compiler; plus all the instruction mnemonics. Moreover
 you'll have to design an application language directed at the problem
of compiling code.

I don't want to down-grade the possibility or value of such
efforts, but you wrote your program in some language to start with. If
you need additional code it's much easier to re-compile your program and
 add what you need. Only if you have an application that needs tailored
code. or can profit by providing different code to different users, or
different code at different times, can you satisfy the Basic Principle.

On the other hand, if you start with code entries, you can
construct all the other entries I've been talking about: arithmetic
operators, noun entries, definitions. In Chapter 9 I'll show how you can
 use code entries in a really essential role; and achieve a
significantly more efficient and powerful program than by any other
means. But except for that I'm afraid they are marginal.

So how can you generate code? First you need a defining entry
that defines a code entry. The characteristic of a code entry is that it
 executes code stored in its parameter field. Thus the address passed to
 ENTRY by its defining entry (say CODE) must be the location into which
will be placed the first instruction. This is not DP, because the entry
itself takes space; but is simply DP plus a constant.

Second you need an entry to deposit a number at DP. We have used
such a routine several times, constructing variables and definitions,
but we've not had an entry for it. I suggest the word "," although that
might conflict with your output entries. All it does is move a number
from the stack to the parameter field. Instructions are numbers of
course. You'll construct them on the stack and then deposit them.
Incidently, this is a useful entry - apart from compiling code. You'll
find it useful for initializing data arrays of all kinds.

Now you can appreciate the source of my earlier caution. You'll
have to provide a flock of entries that access code compiled into your
program that we've not needed to reference directly before. For example
RETURN: when you routine is finished, it must jump to the control loop,
just as you built-in entries do. However you don't know the location of
the control loop in core; and it moves as you change your program. So
you must have an entry to generate a RETURN instruction.

Likewise, if you plan to compile defining entries you must
provide entries that will generate subroutine calls to ENTRY. Other code
 might want to access WORD or NUMBER or indeed any facility already
available in your program. Moreover you will have to define variable
entries for those fields you will use: D and F for output; perhaps STATE
 and BASE; Basically, the problem is that you must make available
outside your program, all the labels available inside it already. You
must use them enough to justify the effort.

All right, you've done that much. Now you've got to decide how to
 construct an instruction. They have several fields - instruction,
index, adddress - that you'll want to put onto the stack separately and
combine somehow. This is easy to do, but hard to design. You probably
don't want to copy your assembler, and probably couldn't follow its
format conveniently anyway. In fact you can do a good job of designing a
 readable compiler language; but it will take some effort. Definitions
provide all the tools you need.

For example, you might write a definition that will "or" together
 an instruction and address and deposit it. Or if your hardware's
awkward, you can provide a definition that converts absolute addresses
to relative, or supplies appropriate paging controls. Whatever you need,
 or want can be readily defined. Done properly, such a compiler is a
substantial application in itself, and if you're going to do it at all,
plan to spend the necessary time and effort.

We discussed conditional statements and loops for the virtual
computer. Precisely the same techniques apply here, with due allowance
for hardware variations. In fact, I originally applied the
stack-oriented branch generation to code for my real computer. Such
statements are really the difference between an assembler and a
compiler. Keep in mind the Basic Principle.

One valuable use of a compiler is the permit the definition of
new kinds of nouns. That is, to construct new defining entries. As an
example consider using the primitive compiler to define instruction
entries as described just above. Or you might want to define entries
that multiply the top of the stack by a constant.

As usual when adding an ability, several distinct entries must
cooperate to provide it. In this case ENTER and ;CODE. Let me
illustrate:
   - : UNIT ENTER , ;CODE 1 V LDA , SP MPY , SP STA , NEXT ,
   - 2.54 UNIT IN
   - 4. IN
The first line defines the word UNIT. The next line uses this defining
entry to define the word IN (inches). The last line uses IN in a way
that puts 4 inches onto the stack, as centimeters. The 3 lines are
equivalent to
   - : IN 2.54 * ;
which is certainly simpler. But if you want to define many UNITs, a
special defining entry is much more convenient and efficient.

The first special word is ENTER. It calls the ENTRY subroutine used
by all your defining entries, but passes a 0 address as the location of
the code to be executed. Look at the definition of UNIT. The word ENTER
is imperative. It generates a double-length pseudo-instruction; a
pseudo-entry for the first half and a 0 constant for the second. At
execution time, the pseudo-entry will call ENTRY to construct a new
dictionary entry, passing the following constant as the address of code
to be executed. The word ;CODE is a combination of the words ";" and
CODE. It terminates the definition of UNIT and stores DP into the
address field established by ENTER. Thus the code that follows ;CODE is
the code that will be executed for all entries created by UNIT. ;CODE
knows where to store DP because ENTER is restricted to being the first
word in any definition that uses it; and ;CODE knows which definition it
 is terminating.

The restriction on the position of ENTER is unimportant, it may
as well be first as anywhere else. In the case of UNIT, only a "," to
deposit the constant was needed. Other nouns might need more elaborate
processing to establish their parameter field.

You notice I gave an example of code following ;CODE. You see
instruction mnemonics and addresses deposited by ",". I don't want to
explain this compiler language, for it is not relevant for your
computer.

One more suggestion might prove helpful. You might define a new
kind of constant: an instruction. When executed, an instruction expects
an address on the stack, extracts a constant from its parameter field
and constrcts and deposits a completed instruction. You'll probably have
 a large number of instructions, and use a large number. This will save
you many deposit entries.

I'm sorry, but I think it's infeasible to attempt an example. If
you can't see how to construct your own code entries from what I've
already said, forget it. The application is extremely machine dependent -
 and rightly so. Don't attempt to apply the same code to several
computers; definitions already do that for you. The purpose of code is
to exploit the properties of your particular computer.
