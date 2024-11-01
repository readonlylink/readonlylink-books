---
title: Introduction
---

I'm not sure why you're reading this book. It's taken me a while to discover why I'm writing it. Let's examine the title: *Programming a Problem-Oriented-Language*. The key word is programming. I've written many programs over the years. I've tried to write *good*
 programs, and I've observed the manner in which I write them rather
critically. My goal has been to decrease the effort required and
increase the quality produced.

In the course of these observations, I've found myself making the
same mistakes repeatedly. Mistakes that are obvious in retrospect, but
difficult to recognise in context. I thought that if I wrote a
prescription for programming, I could at least remind myself of
problems. And if the result is of value to me, it should be of value to
others; if what I say is new to you, you may learn something of value;
if I cover familiar ground, you at least get a new point of view.

I've also been distressed at the lack of concern from others
about problems I consider significant. It amounts to a general
indifference to quality; a casual attitude of confidence that one's
programs are pretty good, in any case as good as necessary. I'm
convinced this confidence is misplaced. Moreover this attitude is
reinforced by the massive trend to high-level languages and a placid
acceptance of their inefficiencies: What's the use of designing a really
 good algorithm if the compiler's going to botch it up anyway?

So I've written a book about programming. I have no great taste
for debating over a one-way communication link and no real interest in
convincing you that I'm right in what I say. So you'll probably find
that I'm being brusk. I'm quite likely to state bluntly something you
may take issue with. Please do! My intention is to document an approach
I've found useful, and perhaps to stimulate critical interest in
programming. If you care enough to take issue, I'm delighted.

Back to the title. What about Problem-Oriented-Language? I didn't
 start out to write about that; and I'm not sure that I'm qualified to
do so. But I discovered that in order to justify what I was doing and
identify the appropriate circumstances for doing it, the term became
essential.

A problem-oriented-language is a language tailored to a
particular application. To avoid that uniquely clumsy term, I'll usually
 substitute *application language* as synonymous. Very often such
 a language isn't recognised for what it is. For instance, if your
program reads a code in column 80 to identify an input card, you are
implementing an application language. A very crude one, a very awkward
one; mostly because you hadn't given the matter any thought. Recognising
 the problem, I'm sure you can design a better solution. This book will
show you how.

## 1.1 The Basic Principle

We have a large number of subjects to talk about. I'm going to throw
before you a lot of techniques that you may be able to use. This is
basically the result of the nature of a digital computer: a general
purpose tool for processing information.

A computer can do anything. I hope that your realize that, providing
you allow me to define "anything", I can prove this. I mean real,
incontrovertible, mathematical-type proof. A computer cannot do
everything. I can prove this, too. But most important, with only you and
 I to program it, a computer can not even do very much. This is of the
nature of an empirical discovery.

So to offer guidance when the trade-offs become obscure, I am going to define the Basic Principle:

- Keep it Simple

As the number of capabilities you add to a program increases, the
complexity of the program increases exponentially. The problem of
maintaining compatibility among these capabililties, to say nothing of
some sort of internal consistency in the program, can easily get out of
hand. You can avoid this if you apply the Basic Principle. You may be
acquainted with an operating system that ignored the Basic Principle.

It is very hard to apply. All the pressures, internal and
external, conspire to add features to your program. After all, it only
takes a half-dozen instructions; so why not? The only opposing pressure
is the Basic Principle, and if you ignore it, there is no opposing
pressure.

In order to help you apply the Basic Principle, I'm going to tell
 you how many instructions you should use in some routines. And how
large a program with certain capabilities should be. These numbers are
largely machine independent; basically they measure the complexity of
the task. They are based upon routines I have used in my programs, so I
can substantiate them. Let me warn you now that I'll be talking about
programs that will fit comfortably in 4K words of core.

The Basic Principle has a corollary:

- Do Not Speculate!

Do not put code in your program that *might* be used. Do not
leave hooks on which you can hang extensions. The things you might want
to do are infinite; that means that each one has 0 probability of
realization. If you need an extension later, you can code it later - and
 probably do a better job than if you did it now. And if someone else
adds the extension, will they notice the hooks you left? Will you
document that aspect of your program?

The Basic Principle has another corollary:

- Do It Yourself!

Now we get down the the nitty-gritty. This is our first clash with the
establishment. The conventionsl approach, enforced to a greater or
lesser extent, is that you shall use a standard subroutine. I say that
you should write your own subroutines.

Before you can write your own subroutine, you have to know how. This
means, to be practical, that you have written it before; which makes it
difficult to get started. But give it a try. After writing the same
subroutine a dozen times on as many computers and languages, you'll be
pretty good at it. If you don't plan to be programming that long, you
won't be interested in this book.

What sort of subroutines do you write for yourself? I have
acquired respect for SQRT subroutines. They're tricky things; seem to
attract a lot of talent. You can use the library routine to good
advantage. Input subroutines now. They seem to have crawled out from
under a rock. I somehow can't agree that the last word was said 15 years
 ago when FORMAT statements were invented.

As I will detail later, the input routine is the most important
code in your program. After all, no one sees your program; but everyone
sees your input. To abdicate to a system subroutine that hasn't the
slightest interest in your particular problem is foolish. The same can
be said for output subroutine and disk-access subroutine.

Moreovere, the task is not that great as to deter you. Although
it takes hundreds of instructions to write a general purpose subroutine,
 you can do what you need with tens of instructions. In fact, I would
advise against writing a subroutine longer that a hundred instructions.

So if you want to read double-precision, complex integers; don't
rely on the COBOL input subroutine, or wait till the manufacturer
revises it. It's a lot easier to write your own.

But suppose everyone wrote their own subroutines? Isn't that a
step backward; away from the millenium when our programs are machine
independent, when we all write in the same language, maybe even on the
same computer? Let me take a stand: I can't solve the problems of the
world. With luck, I can write a good program.

## 1.2 Preview

I'm going to tell you how to write a program. It is a specific program;
that is, a program with a specific structure and capabilities. In
particular, it is a program that can be expanded from simple to complex
along a well defined path, to handle a wide range of problems, likewise
varying from simple to complex. One of the problems it considers is
exactly the problem of complexity. How can you control your program so
that it doesn't grow more complicated than your application warrants?

First I'll define "input", and mention some general rules of
programming that apply to all programs, whether they have input or not.
Actually we will be almost exclusively concerned with input, so I've not
 much to say about programs lacking input.

By admitting input, a program acquires a control language by
which a user can guide the program through a maze of possibilities.
Naturally, this increases the flexibility of the program, it also
requires a more complex application to justify it. However it is
possible to achieve a considerable simplification of the program, by
recognising that it needs a control language as a tool of
implementation.

The next step is a problem-oriented-language. By permitting the
program to dynamically modify its control language, we mark a
qualitative change in capability. We also change our attention from the
program to the language it implements. This is an important, and
dangerous, diversion. For it's easy to lose sight of the problem amidst
the beauty of the solution.

In a sense, our program has evolved into a meta-language, which
describes a language we apply to the application. But having mentioned
meta-language, I want to explain why I won't use the term again. You see
 things get pretty complicated, particularly on a philosophic level. To
precisely describe our situation requires not 2 levels of language -
language and meta-language - but a least 4 levels. To distinguish
between these levels requires subtle arguments that promote not clarity
but confusion. Moreover, the various levels can often be interchanged in
 practice, which reduces the philosophic arguments to hair-splitting.

A problem-oriented-language can express any problem I've
encountered. And remember, we're not concerned with the language, but
with the program that makes the language work. By modifying the language
 we can apply the same program to many applications. However there are a
 class of extensions to the language that constitute another qualitative
 change. They don't increase the capacity of the program, but the
increase the capability of the language. That is, they make the language
 more expressive. We will consider some such extensions in Chapter 8. I
gathered them together chiefly because they share the common property
that I don't quite comprehend their potential. For example, I think the
language applies the concepts of English.

Finally, I want to describe a process whereby you can implement
this program in machine language. That is, a bootstrap technique whereby
 a basic program can modify and expand itself.

I hope you find the ideas I describe of value to you. In
particular, I hope that you will agree that the program I describe has a
 certain inevitability; that it must do certain things, it must do them
in a certain order, and that a certain set of conventions yield an
optimal solution.

I've gone to some lengths to simplify. I hope that you don't find
 too many violations of the Basic Principle, for it's much easier to
elaborate upon a program than it is to strip it to basics. You should
feel free to build upon my basic routines, provided that you recognise
that you are adding a convenience. If you confuse what is expedient with
 what is necessary, I guarantee your program will never stop growing.

You will notice a lack of flow-charts. I've never liked them, for
 they seem to include a useless amount of information - either too
little or too much. Besides they imply a greater rigidity in program
structure than usually exists. I will be quite specific about what I
think you should do and how you should do it. But I will use words, and
not diagrams. I doubt that you would give a diagram the attention it
deserved, anyway. Or that I would in preparing it.
