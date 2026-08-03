# Panel 000 — compiler engineering review (verdict: APPROVE-WITH-CHANGES)

## Top objections (all adopted in revision 2)

**1. No per-type generated code, so §4.3 and §4.10 had no implementation.**
`ts[0] == .num(v:12)` needs recursive structural equality; `b = a` on a record
holding a `str` needs an incref-ing copy; dropping `[Expr]` needs a recursive
destructor; `{K:V}` needs hash+eq for arbitrary `K`. C has no copy ctors, no
dtors, no generic compare. **Fix:** a *type-descriptor pass* generating
`h_T_copy/drop/eq/hash` per reachable type; container representation decided
in M0 by a hand-written spike (spike 04). The type-erased `void*` alternative
would void the "clang type-checks every runtime call" property.

**2. Refcount insertion was put in the wrong phase.** Revision 1's M5b said
"refcounting and COW **in the runtime**" — but `runtime.c` cannot know where a
scope ends. Every exit edge (`return`, `?`, `break`, `continue`, `panic`,
match-arm fallthrough) must decref live locals and copy-out `@` parameters
(§4.8: "copy-out happens always"). Miss one edge in `group()` — which exits
via `?`, via three `fail`s, and via `return inner` — and you leak or
double-free silently. **Fix:** an ownership pass emitting explicit
`incref/decref/cow_check` IR instructions, visible in `--dump-ir`, plus
cleanup-label chains; goldens under `-fsanitize=address,undefined` before
arrays land.

**3. The fixpoint command tested the wrong pair.** Revision 1 compared A's
output with B's output — a rustc-vs-Heroes comparison that would never pass
byte-identical. And binary identity depends on clang/ld noise (LC_UUID, DWARF
paths, mtimes). **Fix:** three compilations, `diff B.c C.c` on generated C.

**4. Nothing emitted C forward declarations, and the mangler had no
namespace.** Order-free top level (§4.2) needs a prototype pass; records need
`typedef struct` **topologically sorted by by-value containment**; `h_` alone
leaves no room for monomorphised instances or M8 modules; fields/variant
cases/labels must be mangled too (`default` is not a legal C member).
**Fix:** decl-ordering pass in M5a; `h_<module>_<name>[_<typehash>]`.

**5. "An emitter bug surfaces as a clang error" is false at -O2.** Missing
`return` after an exhaustive match, uninitialised hoisted temporaries, and
union punning are UB that clang -O2 *exploits*. **Fix:**
`-Wall -Werror=return-type -Werror=uninitialized -fno-strict-aliasing`,
`hero_unreachable()` at every unreachable end, `heroes run` at -O2 by default.

## Threats to the fixpoint (all adopted)

- Rust `HashMap`'s per-process iteration order → mandate `BTreeMap` (clippy).
- Heroes' own `{K:V}` iteration order unspecified → specify insertion order,
  fixed hash seed.
- Monomorphisation/temp/label counters keyed by discovery order — keep
  discovery deterministic.
- No timestamps/absolute paths in emitted C; pin and record the clang version.
- C goldens with `#line` would churn on every edit → no C goldens; determinism
  by double-emit diff, shape by the fixpoint.

`#line` granularity: on source-line *change*, restored to the generated file
around synthetic prologue/cleanup code; `--emit-c --no-line` for debugging.

## Milestone corrections (adopted)

Spike 04 added to M0 (decides the runtime's whole shape); M5 split into
M5a/M5b/M5c around the two passes; named-arg checking ordered before
monomorphisation; M8 split into modules/port/fixpoint; the HashMap-vs-
determinism contradiction resolved at M0, not M8.

## On QBE

The strongest case against the switch: the headline "optimisation" rationale
was *inadmissible* under Part 2 ("performance is not a goal"), §3.2 records
the learning goal as the sole reason C was rejected, and the plan celebrated
deleting the SSA/register/layout theory notes — the book's material. Not
withheld: §1.11 is the founding constraint and §4.19 names `importc` as the
cheapest FFI that exists, C-backend-only. **Verdict: the switch stands, but
the rationale must rest on §1.11/§4.19, and the QBE backend is *scheduled*
post-fixpoint rather than "if and when".** (Adopted.)
