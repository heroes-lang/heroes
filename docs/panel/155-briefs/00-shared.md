# Panel 155 — shared brief

**The question.** Should a generic CALL be refused when the types it binds make
the generic's body illegal — closing `float_map_key` and `ffi_partial_operation`
through a generic — given that doing so deletes a program this repository
documents, in its own voice, as one that must keep working?

Convened at M-check-completeness, 2026-09-16. Full panel: the proposal changes a
diagnostic class and bears on a spec sentence.

## The gap, measured 2026-09-16

Compiler built from the seed, `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`. Exit codes captured directly, not through a pipe.

| shape | `check` | `build` | run |
|---|---|---|---|
| `m: {f64: i64} @ {}` written down | 1 | 1 | — |
| `{K: i64}` keyed at `f64` through `tally<K>` | 0 | 0 | 0, prints `1` |
| `a == b` on a `partial` group record, written down | 1 | 1 | — |
| `same<A>(x: a, y: b)` on that same record | 0 | 0 | **134**, `panic: a partial record has no structural equality` |

One rule, two answers, depending on whether a generic stands in the middle.

The two probes, in full:

```
function tally<K>(k: K) -> i64
    m: {K: i64} @ {}
    m[k] @ 1
    return len(m)

function main()
    print(tally(1.5))
```

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

## The proposed shape (panel 082 R3's, ruled 2026-08-16, never built)

While checking a generic function's body, record per `(decl, type-parameter
index)` the obligations that body incurs — *used as a map key*, *compared with
`==`*. Propagate through generic-to-generic calls by a fixpoint over `(decl,
param)` pairs, finite, so termination is free. At each call site, where the
bindings are known, check each obligation against the concrete type bound and
report **there**.

The seam exists and is not speculative. `selfhost/check/walk.hero:1802` already
records `c.out.instantiations[span.end] @ resolved_args`, keyed by the
**call-site span**; `walk.hero:2225` already walks
`keys(c.out.instantiations)`. The IR route provably cannot carry the diagnostic:
monomorphisation copies the template's span and discards the call's.

## Why not in the body

Panel 084 R1 refused `sort` on a type parameter **in the body**, and it worked
because `sort`'s domain is twelve types — the eight integer widths, the two
floats, `str`, `bool` — so refusing it on `[A]` deleted no program: nothing a
call could choose both sorts and is still a type parameter there. That third of
this milestone is closed, and was pinned to a golden case on the morning of this
sitting (`tests/golden/check/sort-through-a-type-parameter.hero`).

A map keyed on `K` is legal at `str` and every integer width. `==` is legal on
almost everything. So a body refusal for these two deletes programs that run.

## THE FINDING THIS SITTING EXISTS FOR

**Refusing at the call deletes the same program, one line lower.**
`tests/golden/run/abort-map-key-nan.hero` is in the repository today:

```
function nan() -> f64
    return 0.0 / 0.0

function count<K>(ks: [K]) -> i64
    m: {K: i64} @ {}

    for k in ks
        m[k] @ 1

    return len(m)

function main()
    print(count([1.5, 2.5]))
    print(count([nan()]))
```

Measured today: `build` 0, prints `2`, then aborts **134** with `panic: a map key
that is not equal to itself (nan)`. That is its `.expected`, byte for byte.

Its own comment states, in this repository's voice:

> So why does this case still exist? **Because the static rule cannot see every
> route, and the guard behind it must stay tested.** Inside `count<K>`, the key
> type is still a type parameter; only monomorphisation knows what the call
> chose, and `count([1.5, 2.5])` is a working program that must keep working —
> refusing the generic body would delete it.

> If this case ever goes green, the guard has been deleted as redundant — and it
> is not.

The proposed rule refuses `count([1.5, 2.5])` at its call. **So the milestone's
stated reason for preferring the call over the body does not distinguish the
two**: both delete this program. The file `docs/work/milestones/M-check-completeness.md`
says *"a refusal inside the generic's body would delete working programs"* and
recommends the call site — and this is the program, deleted either way.

## The second-order cost, unmeasured and owed

If every static route to a float map key is closed, what still reaches
`hero_map_slot_of`'s `eq(key, key)` guard? `selfhost/check/map_keys.hero`'s
`reaches_float` gives up at `depth > 16` **by design**, and its own comment says
absence at that bound is safe *"because the runtime guard still sits behind this
rule"*. Closing the reachable routes may leave that guard necessary and untested.

## What the sitting is asked to decide

- **R1.** Is a generic call whose bindings make the body illegal a compile
  error, or is the generic body a place these two rules deliberately do not
  reach?
- **R2.** If it is an error, what happens to `count([1.5, 2.5])` and to the
  runtime guard behind it? Is the golden rewritten, and what then keeps the
  guard tested?
- **R3.** If it is not an error, what does spec § 13's *"for it and for any
  value holding it"* mean through a generic, given it is exit 0 and run 134
  today? Is the specification qualified, or is the compiler wrong
  (CLAUDE.md § 12: spec beats compiler)?
- **R4.** Does the answer differ between the two rules? `float_map_key` has a
  runtime guard behind it and a clean named abort; `ffi_partial_operation` has
  panel 061's deliberate `hero_panic`. The milestone file measured both as
  *consistency and not safety*.

## Context every seat should have

- `grep -rnE '^function [a-z_]+<' selfhost/` is **0**. The compiler itself uses
  no generics, so Principle 0's closure list does not need this. It is a thesis
  or robustness argument or it is nothing.
- **17** generic functions exist across `examples/` and the golden trees.
- The llm-ergonomist's veto at panel 082 stands untouched: the generic body's
  line is undecidable from the line plus its signature, and lifting that needs
  constraints on generics, which is the author's trade. Nothing here proposes
  constraints.
- Both rules were measured at panel 084 as **consistency, not safety**: the real
  `nan` key gives a clean named abort and the `partial` case gives panel 061's
  deliberate `hero_panic`, both identical under `--sanitize`.

## Process rules binding every seat

- **Build in a copy.** `cp -r` the tree to your scratchpad and work there;
  `rm -rf target build` after the copy. The repository's working tree is frozen
  for the duration of this sitting.
- The seed builds in a few seconds: `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`. Rebuilding from `selfhost/` is ~20 minutes and
  will kill you on the watchdog. Do not do it.
- **Never read `archive/bootstrap-rs/` or `crates/`.** That tree is archived,
  nothing builds it, and a measurement taken there is of a compiler that does
  not ship.
- Capture exit codes directly. `cmd | head` returns `head`'s status, which
  produced a false number in this sitting's own preparation before it was
  caught.
- Your verdict owes a **prediction** that is falsifiable and names the
  instrument that would score it, and a **condition** under which you would
  change your vote.
