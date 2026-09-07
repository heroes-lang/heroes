# Security

## Reporting

**Use GitHub's private vulnerability reporting**, on this repository's Security
tab. It opens a channel only the maintainer can read, which a public issue is
not. If it is unavailable to you for any reason, open a public issue saying only
that you have something to report and asking for a channel, with no detail in it.

There is one maintainer and no rota, so an answer arrives in days rather than
hours.

## What counts here

Heroes compiles a program you wrote into C, which clang compiles into a binary
you run. Three things are worth a report:

- **A Heroes program that segfaults or corrupts memory.** This is the one the
  language exists to prevent: design.md §1.12 states that a Heroes program must
  not do either, and CLAUDE.md ranks that goal above elegance, token cost,
  compiler size and speed. A reproducer is more valuable than a diagnosis.
- **Emitted C that reaches undefined behaviour** — an overflow that wraps
  instead of aborting, an out-of-bounds read the runtime does not stop, a
  use-after-free through the ownership pass.
- **A compiler that reads or writes outside what it was asked to touch**, or
  that executes something a source file put there.

## What does not

- **A Heroes program that aborts on purpose.** An out-of-bounds index, a division
  by zero, an overflow, a stack that runs out: those are the guards working, and
  the abort is the specified behaviour.
- **Anything about a C library you bound yourself.** The FFI hands your
  declaration to clang and checks it against the header, but a library's own
  bugs are that library's.
- **The vendored BPE tables** in `vendor/tokenizers/`. They are data read by
  `heroes measure` and nothing executes them.

## What is supported

The version you built from a `vX.Y.Z` tag, and `main`. There is no long-term
support branch and no backporting: while the version is below `1.0.0` the fix
lands on `main` and the next release carries it. `heroes --version` says what
you hold.

## No prebuilt binaries exist

Nothing here ships a compiled artifact — no release attaches one, by a decision
on the record (`DESIGN-LOG.md`, 2026-09-03). If you are holding a binary that
claims to be Heroes and did not come out of your own clang, it did not come from
this project.
