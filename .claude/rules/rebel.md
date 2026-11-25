---
paths:
  - "rebel/library/**"
  - "rebel/examples/**"
  - "examples/**"
  - "selfhost/**"
---

# Rebel, the compiler and the two groups of examples

Home of the two rules the author's decision 14 of 2026-11-15 put into the
contract (`docs/roadmap/milestones/M-rebel-library.md`, the author's decisions,
14 with 6 and 7). Rebel is a package a program takes or leaves, never the
language's (CLAUDE.md § 13, design.md §1.11); how it is made and how its code
is written is `rebel/examples/README.md`.

## The compiler does not use the library to write itself

`selfhost/` takes nothing of Rebel: no `use <rebel>/…`, no catalogue, no copy
of a Rebel function under another name. What the compiler needs it writes
itself, in its own tree, and design.md §1.11 says why: the language must be
whole without a library, and the compiler is the program that proves it.
Decision 6 names the one exception it foresees, **the package manager written
with Rebel**, on its own row of the chain, M-rebel-packages-manager; until that
row lands nothing under `selfhost/` reaches the library, and when it lands
panel 242's ruler binds what it takes (design.md §1.11).

## The examples are two groups

- **`examples/`** holds the programs the compiler is tested on. They use no
  library, neither Rebel nor any other package, and stay a fairly complete set
  of what Heroes writes; a program there that needs a library's function
  carries its own, written from its contract.
- **`rebel/examples/`** holds the programs that use Rebel, each package's
  driver among them (`rebel/examples/README.md` § A2). Decision 7 gives Rebel
  a CI pipeline of its own, which compiles and tests Rebel and nothing else
  and starts only when a push touches it.

A program moves between the groups whole: one that takes Rebel leaves
`examples/` for `rebel/examples/`, and never takes it where it stands.
