# Contributing

**Issues and questions are welcome. Pull requests are not accepted.**

That is the whole policy. The rest of this file is why, and what you can do
instead, because a refusal with no reason reads as a door with no handle.

## Why no pull requests

This repository is one person learning compilers, and the way it is built is the
point of it rather than a detail of it. Every line goes through a written
contract, `CLAUDE.md`; every design change goes through a panel of five judges
with differentiated inputs, recorded in `docs/panel/`; every decision is one
dated line in `DESIGN-LOG.md`; every claim is measured in the session that writes
it, or it says out loud that it is unmeasured. A patch arriving from outside that
process cannot carry its own panel session, its own measurement or its own case
law, and the honest options would be to merge it without them, which breaks the
thing being demonstrated, or to redo it here, which wastes your afternoon.

So the answer is no, and it is no for everybody, today. It is not a judgement of
your patch, which nobody has read.

## What the licence lets you do anyway, and that is not withdrawn

Heroes is Apache-2.0 (`LICENSE`), with a runtime exception
(`LICENSE-RUNTIME-EXCEPTION`) so that a program you compile owes nothing for the
runtime inside it. **That licence grants you the right to fork, modify and
redistribute, in writing, and a repository that takes no patches does not take
that back.** Fork it, cut it up, build something else out of it. Attribution is
owed by whoever redistributes Heroes itself, never by somebody who merely uses
it. If you do build something, an issue saying so would be read with pleasure.

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
