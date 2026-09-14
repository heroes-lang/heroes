# Panel 148 — llm-ergonomist

**Do NOT read `00-shared.md`, and do not open the repository.** Your verdict is
an experiment and it is only worth having if you know nothing a reader of this
language would not know. Your world is the three specification variants below
and the tasks. You judge: can a competent writer, given only the specification,
write the right program — and does a plausible mistake become a compile error
or a silent wrong answer? **Veto on non-local constructs.**

## Your inputs, and there are no others

- `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/d2c6e340-fa30-4088-9630-1b1df8597855/scratchpad/panel-148/variants/variant-1.md`
- `…/variant-2.md`
- `…/variant-3.md`

They differ in a small number of places. **Finding where is part of the job**,
and so is saying whether a reader would notice unaided. Do not try to work out
which is current; if you form a guess, say what gave it away.

## The situation

A program in this language gets everything beyond the specification from C
libraries. Some C libraries hand you a thing you must later hand back — open a
database, prepare a query, later close and finalise. Nothing hands it back for
you, and the program may leave a block early by returning or by propagating an
error.

## Tasks — write real programs, from each variant in turn

**Task 1.** Under each variant, write the `extern` group for a database library
that opens a connection, prepares a statement, steps rows, finalises and
closes. Declare it so that the language knows which calls hand you something you
must give back. Say, for each variant, **what you had to write and what you had
to know**.

**Task 2.** Under each variant, a library has a function that hands back a
statement **it still owns** — you must NOT finalise it. Declare that function.
Say whether the variant lets you, and what happens if a writer gets it wrong.
**This is the question that matters most; answer it from the text alone.**

**Task 3.** For each variant, name the most plausible mistake a writer makes
about giving a C resource back, and say whether that variant makes it a compile
error, a loud runtime abort, or a silent wrong answer. Rank the three variants
by that alone.

## Your report

A verdict on each variant; which you would hand a writer and why; the sentence
in each that carried most weight; anything you would veto as **non-local** —
a line whose meaning needs something held from elsewhere in the file or another
file. A finding that two variants are equivalent is real and wanted if true.
