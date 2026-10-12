# Contributing

**Issues and questions are welcome from anyone. Code is welcome from
authorized contributors, through an issue first.**

Heroes is below `1.0.0` and moves along a roadmap, `docs/ROADMAP.md` § The
chain, where every row is a milestone and every change belongs to one. A patch
that arrives unannounced cannot be placed on that chain, and a language that is
still moving can make it stale before anyone reads it. A patch from somebody the
maintainer knows, agreed in an issue, can be placed, read and merged. So the
door is open, and it opens in this order: ask once to be authorized, then one
issue per change, then the pull request.

## Who may send code

Authorized contributors are the people the maintainer has added as
collaborators of this repository, which GitHub shows on their pull requests. To
become one, open *An offer to contribute* from the issue chooser: what you want
to work on, why, and which row of the chain or which open defect it serves. The
maintainer answers in the issue; on *yes* you are added as a collaborator with
the Triage role, which carries no right to push and needs none, since a change
arrives as a pull request from a fork. There is one maintainer and no rota, so
an answer arrives in days rather than hours.

A pull request from somebody not yet authorized is closed with a link to this
file, and the defect or the idea inside it is reopened as an issue and answered
there, so nothing you found is lost.

## The three steps, for every change

1. **Open an issue first.** A defect, a question, or an offer to contribute: the
   forms are in the issue chooser. One change per issue.
2. **Wait for the answer, in the issue.** The maintainer says one of three
   things: *accepted*, with where it sits (a defect's number, a milestone's
   row) and anything the change must respect; *later*, with the condition it
   waits on; or *no*, with the reason, which for a refused feature is a row of
   `docs/design.md` Part 6 and the condition that would make the refusal wrong.
   A change to the language itself, the specification, a syntax, a semantics, a
   class of diagnostic, the architecture, goes through a design sitting
   (`docs/panel/`) before any code is written; the maintainer convenes it, and
   the issue is where you ask for one.
3. **Then open the pull request**, from a fork, its first line naming the issue
   (`Closes #NNN`). A pull request with no accepted issue behind it is closed
   with a link to this file, and its content reopened as an issue.

## What a pull request carries

What the maintainer checks before merging, written out so you can check it
first:

- **One change, the one the issue agreed**, in commits that carry only its
  files.
- **Formatted at the moment of writing**: `heroes fmt <file> --in-place` on every
  `.hero` file touched. The canonical form is what makes two versions
  comparable, so an unformatted file is a diff nobody can read.
- **Its cases.** A golden case per shape under `tests/golden/`, every
  diagnostic annotated in the source that provokes it (`#~ <code>` on the line,
  `#~v <code>` for the next) as well as snapshotted in its `.expected`; a
  repaired defect gets a case named after it, `fixedbugs-NNN-<slug>`. A change
  nothing can falsify is not landed.
- **The three suites green on your machine**, with the commands `README.md`
  gives: the compiler's own tests, the net, and the net's own tests. The CI runs
  them again on four platforms after the merge, and a red leg files a defect.
- **`seed/heroes.c` untouched.** The seed is the compiler's own C, written by
  the compiler, and the maintainer regenerates it at the close of each batch
  with its fixpoint verified; a hand edit there is reverted.
- **No second binary, no script, no Makefile.** A new capability is a `heroes`
  subcommand or flag, and a diagnostic says how to fix the program it names.
- **English everywhere**: code, comments, messages, commit bodies.
- **The licence travels with it.** Apache-2.0 § 5 already says that a
  contribution you send on purpose is licensed under the same terms, so there is
  no agreement to sign. The commit that merges names you, and so does the
  record, which says whose idea a thing was.

## What does not open

- A change to the language without its sitting, however small the diff.
- Anything `docs/design.md` Part 6 refuses, unless the issue is about the
  condition that row names.
- Speed as the reason for a change. A cost that stops a needed program from
  running at all is a defect; a faster compiler with nothing else to show is
  not.

## How a change is merged

The maintainer merges, with a merge commit and never a rebase, because the
record cites commit hashes and a rebase would falsify them. An issue's labels,
class and closing are written by the maintainer; you do not need to learn the
record's format to contribute to the compiler.

## What the licence lets you do anyway, and that is not withdrawn

Heroes is Apache-2.0 (`LICENSE`), with a runtime exception
(`LICENSE-RUNTIME-EXCEPTION`) so that a program you compile owes nothing for the
runtime inside it. **That licence grants you the right to fork, modify and
redistribute, in writing, and a repository that reads patches through an issue
first does not take that back.** Fork it, cut it up, build something else out
of it. Attribution is owed by whoever redistributes Heroes itself, never by
somebody who merely uses it. If you do build something, an issue saying so
would be read with pleasure.

## If you want to support it

Buy the book. *Heroes of code* is the author's history of programming languages,
and it came before this compiler did: the name, the bolt and the panel's
historian seat all come out of it. This project asks for nobody's money, so a
copy is the one thing that funds the time the language is built in.

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
- **A build that fails on your machine.** Four platforms are tested and yours
  may be a fifth.
- **A question about why something is the way it is.** If the answer is not
  already in `design.md`, `spec/heroes-spec.md` or a panel session, the question
  has found a gap in the record.

Open an issue. Include the program, the command and what happened, and the
compiler you built it with (`heroes --version`).

## Before you open one

`heroes doctor` reports what your machine is missing. `design.md` Part 6 is a
list of things this language refuses on purpose, each with the condition that
would make the refusal wrong. If what you want is there, the interesting issue
is about that condition and not about the feature.

## Conduct

`CODE_OF_CONDUCT.md` is the Contributor Covenant 3.0, with where a report goes
written in.

## Security

`SECURITY.md`.
