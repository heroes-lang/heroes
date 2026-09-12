# M-ffi-ladder — the FFI ladder, and the route for the last three rows

## Goal

The milestone the whole architecture was chosen for. design.md §1.11 is where
this project started — *a simple compiled language with modern syntax that can
use other languages' libraries, so I don't have to write a standard library* —
and §4.19 states the promise that makes it survivable: **clang verifies the
declared signature against the real header, so a wrong FFI type is a compile
error rather than a runtime disaster.**

Two deliverables, fixed before the milestone opened. The ladder's third rung —
*SQLite: open a database, run a query, read a result, close* — with §4.19's own
sentence attached to it: *if this works without you having written a standard
library, the architecture holds*. And the **route** for the closure list's last
three rows: file I/O, `args()` and `exit(code)`, as `extern`s, as a shim, or as
built-ins, with the measured spec cost of whichever it turned out to be.

Both are met. `examples/sqlite/main.hero` opens `:memory:`, creates a table,
inserts, prepares, steps, reads a column, finalizes and closes — against the
SDK's own `sqlite3.h`, **with no shim**, clean under `--sanitize`. The three rows
are Tier 2: `read_file`, `write_file`, `args` and `exit` are written in Heroes,
in `library/source.hero`, over `extern` declarations against a new
`runtime/hero_os.h`, so they come through the same door as SQLite itself.

The ladder gained a fourth rung on the author's instruction, **libcurl**, and it
paid for itself in the first compile (below).

Nine steps, one panel, spec 2422 → 2560.

## What surprised

**A mechanism can be verified against one library and verified against nothing
else.** The return-type assertion — one `_Static_assert` over a `_Generic`, which
is what makes §4.19's central sentence true rather than aspirational — was
compiled, measured and shipped against SQLite, libm and libc: eleven bindings out
of eleven correct, the wrong ones firing, `size_t` turning loud. Then it met
libcurl and refused **every enum-returning C function in existence**. `_Generic`
selects on an enum's own type, never on `int`; SQLite returns plain `int`, so
nothing in three rungs of the ladder could have shown it. No reading of the C
standard found it either. One compile did, in the first minute of the rung the
author asked for.

**Two silent routes were stopped by judges who compiled the proposal instead of
reading it.** The include form and the emission mode both looked settled on the
page. With a decoy `sqlite3.h` beside the generated unit, `#include "sqlite3.h"`
read the decoy with **zero diagnostics under `-Weverything`** — panel 020's
decoy-`runtime/` finding relocated to a header, where the `_Static_assert` that
rescued it cannot be written. And include-only emission, which is what Nim's
`importc` does and what the whole design rests on, makes
`extern function sqrt(x: f64) -> int` compile clean, exit 0 and print `1`: four
wrong bindings out of six passed in silence, because the `#include` checks a
call's arguments and nothing else.

**The gate's type table emptied, and that is what a milestone finishing looks
like.** `ptr` and `cstr` were the last two rows, and the only two whose reason
was a veto rather than a schedule. Every type in the language now emits.

**A hole in the type system appeared only when a signature needed it, six
milestones late.** `()?` had no constructible success: `ok(())` is a parse error
because `()` is a type and not a value, `ok()` was `wrong_arity`, a bare `return`
is `missing_value`. `write_file(path, text) -> ()?` is the first signature in the
language that needs one. Panel 036's spec-warden predicted the failure before the
signature existed.

**A stale comment found a live defect, which is CLAUDE.md §11's own example
happening.** `emit/decls.rs` said "no other type crosses the boundary: `ffi_type`
refuses them in the checker" — and nothing did. The premise had been true when it
was written and died silently; the comment went on reading as correct because the
argument was still valid.

## What broke and why

**Panel 007's trap, reopened by a literal rather than by a rule.** `nullptr` is a
keyword, and a keyword that is a *value* must be in `is_line_ender` or the line
plants no terminator. It was not, so `p: ptr @ nullptr` swallowed the next line
and the error landed on a statement the author had done nothing wrong on. The
list is a premise about the language's value-producing tokens, and it now owes a
test that fires when the premise dies.

**Four warnings on correct programs, all in the emitter, none of them new.**
Three were pre-existing and invisible until this milestone gave them a program:
`_ = f(x)` declared a temporary nothing read (`-Wunused-but-set-variable`, and
ignoring a C status code is the ordinary case); a library function the program
never reaches was pruned while its **string literals** were not (invisible until
`read_file` and `write_file` became the first library functions with strings in
them); and `r.must()` on a `()?` loads a value whose only reader emits nothing.
The fourth was mine: an `@` parameter's zero in a return assertion was the
value's rather than a pointer's — `-Wint-conversion`, a warning rather than an
error under C11, which is exactly how it survived a green test run until the
goldens were read.

**A pruning that was tried and reverted, on the record.** Every translation unit
now carries two option structs and eight per-type functions it may never use,
because `read_file -> str?` and `write_file -> ()?` are the first library
functions with a `T?` in their signature. Deriving the emitted set from the IR
misses a `T?` reached through a declared record's field, and a missing name is a
hard error — found by the mutant corpus rather than by any case somebody wrote.
The honest filter needs the declaration graph as well as the IR, and it is queued
rather than half-done.

**One clang failure is now the author's**, and it is the only one: the return
assertion exists to fail when a declaration disagrees with the real header.
Reported as an internal error it printed C the author never wrote, named a file
under `build/<hash>/`, and blamed the compiler for a mistake in a `.hero` file.
CLAUDE.md §7's rule — a clang failure exits 2 and says the compiler is wrong —
now has one named exception, and the author should decide whether §7 records it.

## What landed, and what carried forward

**M-ffi-ladder closed 2026-08-12, tag `m-ffi-ladder` — Heroes calls C, and a
wrong binding cannot compile.**

    $ heroes run examples/sqlite/main.hero
    rows: 3
    longest: 6

The ladder, all four rungs: printf/puts and libm (`run/ffi-libm`), SQLite with no
shim, libcurl with a variadic and an enum return. The closure list's last three
rows land as Tier 2 over `runtime/hero_os.h`, so §1.0's fourteen rows are
complete and **the language's own list is closed**.

**452 crate tests · 41 CLI surface · 13 golden harnesses**, clippy clean, spec at
**2560** of 4096, `heroes mutate` at **93% / 78% over 1253** across a corpus that
grew from 17 programs to 19.

Carried forward: the option-type pruning above; `heroes run` cannot pass
arguments to the program, so `args()` is only exercisable through a built binary
(CLAUDE.md §10's stopping rule decides it, and M-program-corpus is where the
question arrives); `compile "shim.c"` and `framework "…"` were both deferred by
the panel, so **`heroes cc` stays unbuilt and undecided**; and what the group head
actually lacks is search paths, measured — `-lraylib` fails on a clean machine
without `-L/opt/homebrew/lib`.

The record: `docs/panel/036-the-ffi-ladder.md` · `runtime/hero_os.h` ·
`examples/sqlite/main.hero` · `examples/curl/main.hero`.
