# Panel 168 — brief for the ffi-pragmatist

Read `docs/panel/168-briefs/00-shared.md` first. It carries the proposal, the
runtime facts with their line numbers, and three measurements you should re-run
rather than trust.

## Your axis, and why this sitting is yours before it is anyone's

You judge the founding constraint: design.md §1.11 — there is no standard
library, everything comes from C — and you hold the veto on ABI breakage.

**The ground this sitting attacks is your own, from panel 167.** That sitting
adopted a trailing header on your measurement: a leading one means the pointer C
receives is not the allocation base, so `sqlite3_bind_blob` with a real
destructor is `SIGABRT, exit 134`. Nothing about that measurement is disputed.
What is disputed is what follows from it. **You are the seat best placed to say
whether the coordinator has read your own finding correctly, and you are free to
say it has not.**

## What the coordinator claims, and it must be compiled rather than believed

That the trailing header does not make the give-away case *work*; it makes it
fail differently. The model at `docs/panel/168-briefs/layouts.c`, 94 lines, run
five times each on Darwin arm64 with
`clang -std=gnu11 -Wall -Wextra -Wpedantic -Werror`:

- **leading, C frees, then `end_lease`** — exit **133**, five of five, **no
  message at all**, and `MallocErrorAbort=1` changes nothing. Under
  `-fsanitize=address,undefined`: `bad-free`, named at `c_destructor`, which is
  C's own line.
- **trailing, C frees, then `end_lease`, with the CORRECT length** — exit
  **134**, five of five, carrying the runtime's own panic text: *"end_lease of a
  pointer no `.lease()` made — the checker admits `end_lease` only on a lease
  cell, so this is a compiler bug, please report it"*. Under ASan:
  `heap-use-after-free`, named inside the release.
- both layouts **identical and correct** in the ordinary case.

The claim that follows: since `end_lease` is owed on every lease and route A
adds no spelling that waives it, the give-away case is unreachable in a correct
program under either layout, and the trailing header therefore buys nothing
while costing the release its route to its own header.

## Build the binding C this implies, which is the part only you do

The real header, the real library. `sqlite3_bind_blob`'s signature is
`(sqlite3_stmt*, int, const void*, int, void(*)(void*))`. Panel 167's record says
you bound it against the real `sqlite3.h` and linked the real
`libsqlite3.dylib`; do that again, and this time run the **full** sequence a
Heroes program would run:

1. lease, bind with `SQLITE_TRANSIENT`, step, finalize, **`end_lease`**;
2. the same with `SQLITE_STATIC`;
3. the same with `sqlite3_free` as the destructor, **including the `end_lease`
   the language requires**.

The third is the one that decides this sitting. Panel 167's prediction — *"under
the trailing-header allocation, `sqlite3_bind_blob` needs no shim for all three
retention modes"* — is checkable now, and the question is whether it was scored
with the mandatory `end_lease` in the sequence or without it.

## What the compiler already refuses, re-run rather than asserted

`free(p: cstr)` in a group: `error[ffi_writable_parameter]`, exit 1 — so the
give-away case is reached only through a `const`-qualified parameter. Confirm
this and say whether any real header offers a **non-`const`** route you would
want bound.

## The questions

1. Is the proposal right — leading header for route A, §4.19's third reserved
   case left to the feature that builds it with its own spelling?
2. **Is there a route nobody listed?** Say what would have to be true for a
   third to exist, and search a real header for it. Specifically: is there a
   retention mode in a shipping library where the *library* frees the buffer and
   the program is expected not to?
3. Does route A's field lease need the allocation base for any binding you would
   actually write? A field lease copies a fixed-array field's bytes out of an
   `extern` record — `i8[8]`, `char[260]` — into a block the program owns.
4. Under **both** layouts the give-away case ends in an abort and the program
   learns nothing about what it did wrong. Is that a defect to file, and against
   what? A wrong diagnostic that names the compiler for a program's fault is
   arguably worse than none.
5. Does anything here break ABI for a binding already in this repository?
   `examples/ledger/db/sqlite.hero` is the one with a real library behind it.
6. **Register a falsifiable prediction** with an instrument that exists today.

## Working rules

- `cp -r` the tree to your scratchpad, then `rm -rf target build` in the copy.
- A compiler in 3.80 s:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
- Write your report to `docs/panel/168-reports/ffi-pragmatist.md`.
- You have a veto on ABI breakage. A veto is a refusal, not a price.
