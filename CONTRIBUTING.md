# Contributing

**Issues and questions are welcome. Code is not, yet.**

Today this repository takes reports and questions, and not patches. Code
contributions are meant to open later, and the rest of this file says why they
are shut now and what has to be true before they are not, because a refusal with
no reason reads as a door with no handle, and one with no date reads as a wall.

## Why not code, yet

This repository is one person learning compilers, and the way it is built is the
point of it rather than a detail of it. Every line goes through a written
contract, `CLAUDE.md`; every design change goes through a panel of five judges
with differentiated inputs, recorded in `docs/panel/`; every decision is one
dated line in `DESIGN-LOG.md`; every claim is measured in the session that writes
it, or it says out loud that it is unmeasured. A patch arriving from outside that
process cannot carry its own panel session, its own measurement or its own case
law, and the honest options today would be to merge it without them, which breaks
the thing being demonstrated, or to redo it here, which wastes your afternoon.

So it is not a judgement of your patch, which nobody has read. It is that the
process a patch would have to enter is still being written, in public, by the
person writing the compiler.

## What has to be true before code opens

Stated so that it is a condition and not a mood, and so that you can check it
yourself rather than wait to be told:

- **The language stops moving under a patch.** While the version is below
  `1.0.0` the surface still changes between minor versions, which means a patch
  can be made stale by a decision it never heard about. The compatibility
  paragraph that `1.0.0` owes is the same milestone that makes an outside patch
  survivable.
- **The process a change goes through is reachable from outside.** A design
  change here needs a panel session and a design-log line; today both are things
  that happen inside one working session, and neither has a shape somebody
  outside can take part in.
- **The rules a patch must satisfy are checkable before it is sent.** Most of
  them already are, and the three suites in `README.md` are how: what is missing
  is the part of the contract that lives in a session rather than in a test.

None of the three has a date, and this file will say so plainly rather than
inventing one. When they are met, this section is replaced by the shape a
contribution takes, and the change is announced where everything else here is,
in the build log at heroes-lang.org.

## What the licence lets you do anyway, and that is not withdrawn

Heroes is Apache-2.0 (`LICENSE`), with a runtime exception
(`LICENSE-RUNTIME-EXCEPTION`) so that a program you compile owes nothing for the
runtime inside it. **That licence grants you the right to fork, modify and
redistribute, in writing, and a repository that takes no patches does not take
that back.** Fork it, cut it up, build something else out of it. Attribution is
owed by whoever redistributes Heroes itself, never by somebody who merely uses
it. If you do build something, an issue saying so would be read with pleasure.

## If you want to support it

Buy the book. *Heroes of code* is the author's history of programming languages,
and it came before this compiler did: the name, the bolt and the panel's
historian seat all come out of it. This project takes no code contributions yet
and asks for nobody's money, so a copy is the one thing that funds the time the
language is built in.

Published on Amazon, in
[English](https://www.amazon.it/Heroes-code-journey-programming-languages/dp/B0HHLBWWZH/)
and in
[Italian](https://www.amazon.it/Gli-eroi-del-codice-programmazione/dp/B0HHL2YQ1S/).

## What is genuinely useful

- **A program that should compile and does not, or compiles and does the wrong
  thing.** This is the most valuable thing anyone can send. The language exists
  to make a wrong program fail to compile, so a wrong program that compiles is a
  hole in the thesis, not a nuisance.
- **A diagnostic that does not tell you how to fix your program.** A diagnostic
  here is a deliverable: it is supposed to carry the place, the other end of the
  story and the repair, without your opening another file. One that does not is a
  defect.
- **A build that fails on your machine.** Three platforms are tested and yours
  may be a fourth.
- **A question about why something is the way it is.** If the answer is not
  already in `design.md`, `spec/heroes-spec.md` or a panel session, the question
  has found a gap in the record.

Open an issue. Include the program, the command and what happened, and the
compiler you built it with (`heroes --version`).

## Before you open one

`heroes doctor` reports what your machine is missing. `design.md` Part 6 is a
list of things this language refuses on purpose, each with the condition that
would make the refusal wrong — if what you want is there, the interesting issue
is about that condition and not about the feature.

## Security

`SECURITY.md`.
