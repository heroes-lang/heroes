# Panel 237, the historian's brief

Read `00-shared.md` and `proposal.md` beside this file first. You are advisory, with no veto. Every
precedent you cite you read on the web while writing, with its URL and the day read, marked
`verified`; a claim you could not read is marked `unverified` and nothing rests on it. Your worktree
is `.claude/worktrees/237-historian/` if you need the tree; you need no compiler.

## What the sitting needs from you

1. **Header search and a project's file standing in for a system header** (questions 1 and 3): what
   the C compilers' search orders are and were for (GCC's and clang's `-I`, `-iquote`, `-isystem`,
   `-idirafter`, `#include_next`), and how build systems and languages that compile C for a user
   meet a project's file named like a library header: refused, reordered, warned, or left (CMake,
   Meson, Go's cgo, Rust's `cc` crate, Zig's `@cImport` and its build system, Nim, any other you
   find). Dates where the change was a correction.
2. **A package's compiler flags held to a list** (question 3's second half): Go's cgo restricts the
   flags a `#cgo` directive and `pkg-config` may pass since a security fix (the release and the
   advisory, read); what it allows of `-D` and `-U`; and any other system that judges a package's
   `-D`.
3. **SQLite's own word on closing** (question 7): what SQLite's documentation says `sqlite3_close`
   and `sqlite3_close_v2` do with a statement still prepared, and for whom `_v2` was made; and how
   mature bindings bind the close (Python's `sqlite3` module, Go's `mattn/go-sqlite3`, Rust's
   `rusqlite`, Node's `better-sqlite3`), each read in its source or documentation.
4. **Error messages that name a file standing in**: a compiler or build tool whose diagnostic says
   *this file shadows that header* and what it tells the user to do.

Your report: `docs/panel/237-reports/historian.md`, written as you go, ending as `00-shared.md`
§ Rules says. It is the record the sitting's `## Prior art` is written from, so a line of it says only
what the cited page states.
