---
title: Preface
---

The computing world has undergone a revolution since the publication
of The C Programming Language in 1978. Big computers are much bigger,
and personal computers have capabilities that rival mainframes of a
decade ago. During this time, C has changed too, although only
modestly, and it has spread far beyond its origins as the language of
the UNIX operating system.

The growing popularity of C, the changes in the language over the
years, and the creation of compilers by groups not involved in its
design, combined to demonstrate a need for a more precise and more
contemporary definition of the language than the first edition of this
book provided. In 1983, the American National Standards Institute
(ANSI) established a committee whose goal was to produce "an
unambiguous and machine-independent definition of the language C",
while still retaining its spirit. The result is the ANSI standard for
C.

The standard formalizes constructions that were hinted but not
described in the first edition, particularly structure assignment and
enumerations. It provides a new form of function declaration that
permits cross-checking of definition with use. It specifies a standard
library, with an extensive set of functions for performing input and
output, memory management, string manipulation, and similar tasks. It
makes precise the behavior of features that were not spelled out in
the original definition, and at the same time states explicitly which
aspects of the language remain machine-dependent.

This Second Edition of The C Programming Language describes C as
defined by the ANSI standard. Although we have noted the places where
the language has evolved, we have chosen to write exclusively in the
new form. For the most part, this makes no significant difference; the
most visible change is the new form of function declaration and
definition. Modern compilers already support most features of the
standard.

We have tried to retain the brevity of the first edition. C is not a
big language, and it is not well served by a big book. We have
improved the exposition of critical features, such as pointers, that
are central to C programming. We have refined the original examples,
and have added new examples in several chapters. For instance, the
treatment of complicated declarations is augmented by programs that
convert declarations into words and vice versa.  As before, all
examples have been tested directly from the text, which is in
machine-readable form.

Appendix A, the reference manual, is not the standard, but our attempt
to convey the essentials of the standard in a smaller space. It is
meant for easy comprehension by programmers, but not as a definition
for compiler writers -- that role properly belongs to the standard
itself. Appendix B is a summary of the facilities of the standard
library. It too is meant for reference by programmers, not
implementers. Appendix C is a concise summary of the changes from the
original version.

As we said in the preface to the first edition, C "wears well as
one's experience with it grows". With a decade more experience, we
still feel that way. We hope that this book will help you learn C and
use it well.

We are deeply indebted to friends who helped us to produce this second
edition. Jon Bently, Doug Gwyn, Doug McIlroy, Peter Nelson, and Rob
Pike gave us perceptive comments on almost every page of draft
manuscripts. We are grateful for careful reading by Al Aho, Dennis
Allison, Joe Campbell, G.R. Emlin, Karen Fortgang, Allen Holub, Andrew
Hume, Dave Kristol, John Linderman, Dave Prosser, Gene Spafford, and
Chris van Wyk. We also received helpful suggestions from Bill
Cheswick, Mark Kernighan, Andy Koenig, Robin Lake, Tom London, Jim
Reeds, Clovis Tondo, and Peter Weinberger. Dave Prosser answered many
detailed questions about the ANSI standard. We used Bjarne
Stroustrup's C++ translator extensively for local testing of our
programs, and Dave Kristol provided us with an ANSI C compiler for
final testing. Rich Drechsler helped greatly with typesetting.

Our sincere thanks to all.

Brian W. Kernighan

Dennis M. Ritchie
