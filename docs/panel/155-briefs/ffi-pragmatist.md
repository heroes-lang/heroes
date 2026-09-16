# Panel 155 — ffi-pragmatist brief

Read `docs/panel/155-briefs/00-shared.md` first.

You judge the founding constraint: design.md §1.11 and §4.19 — there is no
standard library, everything comes from C. You have a veto on ABI breakage.
**Your input must include the C a real binding would need, and you compile it.**

## Your half of this question is `ffi_partial_operation`, and it is the half
## with a real library behind it

The rule under discussion is spec § 13's: a `partial` group record — one naming
only some of a C struct's fields — may not be compared with `==`, may not be
hashed, and may not be a map key, *"for it and for any value holding it"*. Its
size stays C's, not the field list's, which is exactly why: the fields it does
not name are part of the answer and the program cannot see them.

Measured 2026-09-16 on the shipped compiler, that promise holds when the
comparison is written directly and **fails through a generic**:

```
extern "sys/stat.h"
    record FileStat tag stat partial
        st_size: i64

function same<A>(x: A, y: A) -> bool
    return x == y

function main()
    a: FileStat @ FileStat(st_size: 0)
    b: FileStat @ FileStat(st_size: 0)
    print(same(x: a, y: b))
```

`check` 0, `build` 0, run **134**, `panic: a partial record has no structural
equality`. Written as `print(a == b)` it is `check` 1 with
`error[ffi_partial_operation]`.

## What you are asked to measure

1. **Is the abort a correctness question or a tidiness question?** The milestone
   file asserts both these rules are *"consistency and not safety, and that is
   measured rather than assumed"* — a clean stop, identical under `--sanitize`.
   Test it against a **real** binding rather than the toy above. Take a real
   header with a struct a binding would genuinely declare `partial` — `struct
   stat` is the obvious one, `struct addrinfo` another, and this repository has
   a shipped SQLite binding under `examples/ledger/db/`. Write the binding,
   compile it, and find out whether a `partial` record reaching `==` through a
   generic can produce a **wrong answer at exit 0** rather than an abort. If it
   can, the milestone's *consistency not safety* claim is false and this becomes
   a §1.12 question, which outranks everything else in this sitting.

   Specifically worth probing: the abort fires from a generated `_eq` for the
   named type. What happens when the `partial` record is *inside* something —
   a field of another record, an element of an array, a map value — and that
   holder is compared through a generic? Does every route reach the abort, or
   is there one that compares the bytes it can see and returns a confident
   `true`? The rule's own words are *"for it and for any value holding it"*, so
   the holder routes are the ones that matter.

2. **The padding question, and it is yours alone.** A `partial` record's size is
   C's. When such a value is copied, passed to a generic, and compared — what
   is in the bytes the field list does not name? If they are uninitialised
   padding, then even a byte-wise comparison is comparing values nobody wrote,
   which is a different defect from the one the rule names. Compile it and look.

3. **Does the proposal break any binding that exists?** `examples/ledger/db/`
   ships a real SQLite binding. Does refusing generic calls whose bindings make
   the body illegal refuse anything in it, or in any binding a reader would
   plausibly write? Name the count, not an impression.

4. **R4 specifically**: does your half of the question have the same answer as
   the map-key half? The map rule has a runtime guard behind it that catches the
   one case a type cannot decide. Does the `partial` rule have an equivalent
   guard, and is it as complete?

## Process

Build in a copy: `cp -r` the tree to your scratchpad, then `rm -rf target
build`. The repository's working tree is frozen for this sitting. The seed
builds in a few seconds with `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`; do **not** rebuild from `selfhost/`, which takes ~20 minutes and will
kill you on the watchdog. Never read `archive/bootstrap-rs/`.

Capture exit codes directly, not through a pipe.

## Your verdict owes

A verdict per R1-R4, the C you compiled, a falsifiable prediction naming the
instrument that would score it, and the condition under which you would change
your vote. Say plainly whether you are casting your veto.

Write your report to `docs/panel/155-reports/ffi-pragmatist.md`.
