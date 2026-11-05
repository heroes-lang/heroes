# Panel 234, the historian's brief

Read `00-shared.md` first, then `proposal.md` and `conventions-draft.md`.
You judge precedent, and **every precedent you give is verified by a web
search and a page you read**: its URL, the day you read it, and what the page
itself states, in its own words where they matter. A precedent you cannot
source is marked *unverified* and the sitting will not rest on it. Your seat
is advisory; the author asked for your work by name this time (instruction 2
of 2026-11-05: *look at how the libraries of Odin, Zig, Hylo and the
languages like Heroes are made*). Your report is
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/234-reports/historian.md`,
written as you go. You read the repository; you write nothing in it but your
report.

## Your items

**The survey the author asked for (A, B, C).** For each of **Go** (the
standard library), **Odin** (`core:` and `vendor:`), **Zig** (`std`),
**Hylo** (its standard library, as far as one exists), **Hare** (its
standard library), **Nim** (`std/`), and **Goose**
(github.com/aardappel/goose, its `docs/stdlib.md` and
`docs/design/stdlib_design.md`, which the milestone page read on
2026-10-25), say, from the language's own pages:
1. **What the library holds and what it refuses to hold**, and whether the
   language states a rule for the line (Goose's admission test is one such
   rule; find the others, or say you searched and found none, naming where).
2. **How a program reaches the library**: a path the compiler knows (Go's
   `GOROOT`, Odin's collections, Zig's built-in `std`), a search path (Hare,
   Nim), a manifest, or a copy; and whether any language that began with
   imports resolved relative to the program's own root, no manifest and no
   search path, was later forced to add **a manifest** (panel 099's historian
   searched for that, and only that, and found none,
   `docs/panel/099-where-a-use-path-starts.md:189-191`) **or a search path**
   (a half nobody has searched; F34 of the critic's first pass). And for the
   two routes the critic added to item B, B5 and B6: which languages ship
   their library inside or beside the compiler and find it relative to the
   compiler's own executable (Odin's `core:`, Zig's `lib/std`, Go's
   `GOROOT`), and whether any of them guards that path against a decoy, the
   trust argument panel 028 R3 refused a prelude on disk with.
   Nim's `std/` and `pkg/` prefixes are one precedent for item B's `core/`;
   read what Nim's own documents say they were for.
3. **Names and contracts, for the author's rule** (*a Go name only where the
   contract is Go's*). For one operation, the position of a substring, give
   each language's name and its answer when the substring is absent: Go's
   `strings.Index` (`-1`), Odin's `core:strings` (its name and its absent
   answer), Zig's `std.mem` (its name and whether it answers an optional),
   Rust's `str::find`, Hare's `strings::index`, Nim's `strutils.find`,
   Python's `str.find` and `str.index`, and Swift's, whose name for the
   position of a **substring** you find rather than take from this brief
   (`firstIndex(of:)` finds an element; F34). Then say
   which languages kept a familiar name while changing its contract, and
   whether any of them recorded that as a source of mistakes (an issue, a
   proposal, a changelog), with its link. Do the same, more briefly, for
   splitting text into lines and for the failure convention of the whole
   library (Go's `(value, error)`, Odin's multiple returns with `ok`, Zig's
   error unions, Rust's `Option` and `Result`, Hare's tagged unions).
4. **Thin or thick over C (D).** Verify the milestone page's PHP precedent:
   php.net's wording on `mysqli` and `PDO_MySQL`, and
   `wiki.php.net/rfc/consistent_function_names`, what it proposed and what
   became of it. Then two contrasting shapes over one C library: Go's
   `database/sql` with its drivers, and Python's `sqlite3` (DB-API, PEP 249);
   and Odin's split between `vendor:` (bindings) and `core:` (its own code).
   Which language states a rule for how thick a wrapper over C is.

**F, the platform layer (the author's instruction 4: *look at whether
something like it already exists, as Qt and GTK do it*).** For **GLib**
(under GTK; GIO, GThread), **Qt** (its platform abstraction: QPA plugins, and
the per-platform code of QtCore), **libuv** (under Node; its `src/unix/` and
`src/win/`), **the Apache Portable Runtime**, **NSPR** (Mozilla's), **SDL**
(its per-platform backends), **Go** (its per-OS files and build constraints,
`syscall` and `internal/poll`), **Zig** (`std.posix`, `std.os`), **Odin**
(`core:os`, its 2025 redesign the design document cites, and `core:sys`),
and **Rust** (`std::sys`): say how each separates the platform's code from
its API (one header and per-platform source files, plugins chosen at run
time, files chosen at build time), whether that layer is a separate library a
program links on demand or part of the one runtime, and what each says it
cost them (a bug class, a redesign). Which of these is the shape the author
described, *a C library pulled in beside the runtime with the `#ifdef`s
underneath*, and which is not.

**J, `alias`.** Only if your reading of Go's 1.9 type aliases differs from
design.md Part 7 item 5's corrected paragraph (its warrant was moving a type
between packages): say so with the source. Otherwise one line.

**M, sorting by a caller's order.** Each language's name and shape for it:
Go's `sort.Slice` and `slices.SortFunc`, Odin's `slice.sort_by`, Zig's
`std.mem.sort` with its `lessThan`, Rust's `sort_by`, and whether it lives in
the language's core or in a package.

**N, measuring a library's effect on a model.** Whether any language, or any
study, measured how a standard library's names or documentation changed the
code a language model writes; a source, or the searches you ran and found
nothing in.

## Report shape

Per item: the precedents, each `verified` (URL, day read, what the page
states) or `unverified`; then **verdict** (which route the precedent favours,
and how strongly), **prediction** (falsifiable, with when it is checked) and
**condition**. Keep a precedent's line to what its page says: the sitting
measured in 2026-10-11 that a third of such lines, re-read by others, said
more than the page did.
