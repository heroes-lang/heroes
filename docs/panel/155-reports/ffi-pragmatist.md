# Panel 155 — ffi-pragmatist

Seat: the founding constraint, design.md §1.11 and §4.19. Method: compile, do
not opine. Every number below is marked **measured** (with the command and the
C) or **unrun**.

Compiler: built in a scratchpad copy of the tree,
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed`, exit 0.
Exit codes captured directly, never through a pipe. The repository working tree
was not modified; this file is the one exception the brief permits.

## Verdicts

| | |
|---|---|
| **R1** | **approve for `ffi_partial_operation`, object to the milestone's reason** |
| **R2** | **approve** — the padding question is clean, and it is not a defect |
| **R3** | **the compiler has the bug, and the bug is larger than the generic route** |
| **R4** | **object** — the two halves have the SAME structure, not different ones |
| **veto** | **NOT cast.** Nothing here breaks the C ABI or makes a binding harder |

---

## The headline, and it is the question the brief asked me alone

**No. A `partial` record reaching `==` through a generic cannot produce a wrong
answer at exit 0. Every route I compiled reaches the abort.** Measured, six
holder routes plus the direct one, all exit **134**, all with the same named
panic. The milestone's *consistency and not safety* claim survives my attack on
the `==` half.

**But the claim is true for a reason the milestone does not state, and the
reason inverts the decision.** The refusal is complete at run time because it
lives **inside the generated `_eq` and `_hash` of the partial type itself** —
the one function every route must call — and not in any walk that could miss a
route. It is therefore **load-bearing, not a belt over braces**, and I have a
compiled program that proves it: see § The depth-16 hole.

---

## R1 — the holder routes, measured

Real header, real binding. `extern "sys/stat.h"`, `record FileStat tag stat
partial` naming only `st_size`. Six programs, each wrapping the value in a
different holder and comparing the holder through
`function same<A>(x: A, y: A) -> bool` whose body is `return x == y`.

| holder | `check` | `build` | run | output |
|---|---|---|---|---|
| `record Holder { f: FileStat }` | 0 | 0 | **134** | `panic: h_hrecord_FileStat_eq: a partial record has no structural equality` |
| `[FileStat]` | 0 | 0 | **134** | same panic |
| `{i64: FileStat}` (map value) | 0 | 0 | **134** | same panic |
| `variant Shape { one { f: FileStat } }` | 0 | 0 | **134** | same panic |
| `FileStat?` inside a record | 0 | 0 | **134** | same panic |
| `{Holder: i64}` through `tally<K>` | 0 | 0 | **134** | same panic |

Zero wrong answers at exit 0. The brief's own toy reproduces identically:
`check` 0, `build` 0, run **134**.

**Why there is no hole, from the emitted C rather than from reading.** Two
facts, both measured:

- `grep memcmp runtime/parts/*.c` returns **two** hits, both in `str.c`
  (`:160`, `:167`). There is **no byte-wise shortcut for any aggregate**.
  `hero_array_eq` (`array.c:259`) goes through `work.elem->eq`; `hero_map_eq`
  (`map.c:229`) through `a->val->eq`; `hero_map_slot_of` (`map.c:85`) through
  `m->key->eq(key, key)` — which is the nan guard, and it is what fires first on
  a partial key.
- The emitted C for the partial type, read out of a real emission
  (`--emit-c` on the `stat` binding, lines 217-223):

```c
HERO_TU_LOCAL bool h_realstat_FileStat_eq(const struct stat *a, const struct stat *b) {
    hero_panic("h_realstat_FileStat_eq: a partial record has no structural equality");
}
HERO_TU_LOCAL uint64_t h_realstat_FileStat_hash(const void *elem) {
    (void)elem; hero_panic("h_realstat_FileStat_hash: a partial record has no structural hash");
}
```

Every short-circuit that answers before reaching this function — unequal array
lengths, unequal map sizes, differing variant tags, an earlier field that
differs — answers about parts the program **can** see, so its answer is correct.
The only way to a wrong answer is to return `true` while hidden bytes differ,
and that requires calling this function.

**Under `--sanitize`:** identical. `build` 0, run **134**, same message, for the
generic toy and for the depth-17 case below. Measured.

**The empty shape** (CL-061 asks for it): `record Db tag sqlite3 partial` with
no fields, and `record FileStat tag stat partial` with no fields, are both
`check` 1, `error[empty_record]`, with a diagnostic that names the repair. Not a
hole.

---

## THE DEPTH-16 HOLE — no generic in it, and it is the finding of this seat

`selfhost/check/partial.hero:49-51`:

```
function reaches_within(c: state.Checker, decls: [ast.Decl], ty: i64, depth: i64) -> i64?
    if depth > 16
        return fail(code: "absent", msg: "past any header's nesting")
```

Its own comment says *"the fallback is absence, the safe direction here."*
Absence is the **permissive** direction. I compiled a nest of N records, each
holding the next, with the `partial` `FileStat` at the bottom, and `a == b` at
the top. **No generic anywhere in the program.**

| nesting depth | `check` | `build` | run |
|---|---|---|---|
| 10 | **1** `error[ffi_partial_operation]` | 1 | — |
| 16 | **1** `error[ffi_partial_operation]` | 1 | — |
| **17** | **0** | **0** | **134** `panic: h_nest17_FileStat_eq: …` |
| 18 | 0 | 0 | **134** |
| 20 | 0 | 0 | **134** |
| 24 | 0 | 0 | **134** |

Measured this session. Programs generated into the scratchpad and compiled with
`heroes-seed`.

**What this does to the sitting.** R3 asks whether spec § 13's *"for it and for
any value holding it"* is qualified by generics, or whether the compiler is
wrong. Neither framing survives: **the gap is not made by generics.** It exists
today in straight-line code, by the checker's own deliberate bound, and the
generated `hero_panic` is what closes it. Closing the generic route adds a
seventh early report; it does not make the static rule complete, because a
call-site check that reuses `partial.reaches` inherits the same bound.

CLAUDE.md § 12 — spec beats compiler — therefore says the compiler has the bug,
and the bug is **two** routes wide, not one.

---

## R2 — the padding question, mine alone, and the answer is clean

Measured with clang on this Mac: `sizeof(struct stat) == 144`,
`offsetof(st_size) == 96`, `sizeof(st_size) == 8`. So a `partial` naming only
`st_size` leaves **136 bytes** the field list does not name, including a
four-byte padding hole at offset 28 that no member names at all.

The emitter builds one with a C designated-initialiser compound literal. The
exact line, from the real emission (`realstat.c:145`):

```c
t2 = (struct stat){.st_size = t1};
```

I lifted that line into hand-written C, dirtied the stack frame with 0xAA
beforehand, and counted:

```c
static struct stat build(int64_t t1) {
    struct stat t2;
    t2 = (struct stat){.st_size = t1};
    return t2;
}
```

| clang | unnamed+padding bytes | nonzero | padding[28..31] | struct assignment memcmp |
|---|---|---|---|---|
| `-O0` | 136 | **0** | `00 00 00 00` | **0** |
| `-O2` | 136 | **0** | `00 00 00 00` | **0** |

**Verdict: approve.** The bytes the field list does not name are not
uninitialised garbage — they are zero, deterministically, C11 6.7.9p21's rule,
and clang zeroes the padding hole too at both optimisation levels. A plain C
struct assignment carries all 144 bytes verbatim. Panel 061's *124 of 124*
reproduces here as **136 of 136** for `struct stat` on this machine. There is no
second defect hiding behind the one the rule names, and in any case `==` never
reads those bytes: it panics first.

---

## R3 — does the proposal break any binding that exists?

**Count, measured this session by grep, not impression.**

| | count |
|---|---|
| generic declarations `^function [a-z_]+<` in `examples/` | **17** |
| the same in `tests/golden/` | **33** |
| the same in `selfhost/` | **0** |
| `extern` groups in `examples/` | **22**, in 22 files |
| files declaring **both** an `extern` group and a generic, in `examples/` and `tests/golden/` | **0** |

**I confirm the spec-warden's zero, measured independently with a different
command** (I iterated the `extern`-declaring files and asked each whether it also
declares a generic; the warden used `comm -12` on two file sets). Our answers
agree: **the proposal deletes no program that is in this repository today.**

**And that is not the same as deleting no program, which is where I part from
the warden's conclusion.** I compiled the exact analogue of
`tests/golden/run/abort-map-key-nan.hero` for my half:

```
extern "sys/stat.h"
    record FileStat tag stat partial
        st_size: i64
    function stat(path: cstr, @out: FileStat) -> i32

function any_equal<A>(xs: [A]) -> bool
    i: i64 @ 0

    while i + 1 < len(xs)
        if xs[i] == xs[i + 1]
            return true
        i @ i + 1
    return false

function main()
    st: FileStat @ FileStat(st_size: 0)
    _ = stat("/etc/hosts".cstr(), @st)
    one: [FileStat] @ [st]
    print(any_equal(xs: one))      # never reaches the comparison
    two: [FileStat] @ [st, st]
    print(any_equal(xs: two))      # reaches it
```

Measured: `check` 0, `build` 0, prints **`false`** — the correct answer — and
then aborts **134**. Exit code 134.

That is byte for byte the shape the sitting exists for: a generic whose body
compares, a first call where the comparison is data-unreachable and the answer
is right at exit 0, a second call where it is reached. **The call-site refusal
deletes `any_equal(xs: one)`, exactly as it deletes `count([1.5, 2.5])`.**

So the honest statement is: *the proposal deletes nothing in this repository,
and it deletes a program shape a binding author would plausibly write, which I
compiled and ran.* The milestone's stated reason for preferring the call over
the body does not distinguish the two halves — the shared brief's finding holds
on my half too.

**The real binding works, and the proposal does not touch it.** The binding a
reader would genuinely write:

```
extern "sys/stat.h"
    record FileStat tag stat partial
        st_size: i64
    function stat(path: cstr, @out: FileStat) -> i32

function main()
    st: FileStat @ FileStat(st_size: 0)
    rc = stat("/etc/hosts".cstr(), @st)
    print(rc.to_i64().must())
    print(st.st_size)
```

`check` 0, `build` 0, run **0**, prints `0` then `544`. `ls -l /etc/hosts` says
544. clang accepted the emitted C, including the `_Static_assert` that verifies
`st_size`'s width and signedness against the header
(`realstat.c:13`) and the return-type assert for `stat`
(`realstat.c:33`). This is §4.19's thesis working: a wrong width in that record
is a clang error, not a run-time surprise.

Two earlier drafts of that same binding were **refused** by the compiler on the
way here, which is the thesis doing its job: `print(rc.to_i64())` is
`error[bad_operand]` because `i32 -> i64` is fallible, and `stat(path: …, @out:
st)` is `error[expected_args_close]` because an `@` argument is positional.

---

## R4 — do the two halves differ? No, and that is the answer

The milestone says `float_map_key` has a runtime guard and `ffi_partial_operation`
has *panel 061's deliberate `hero_panic`*, as if these were different kinds of
thing. Measured, they are the same kind of thing with the same structure:

| | `float_map_key` | `ffi_partial_operation` |
|---|---|---|
| static walk | `check/map_keys.hero` `reaches_float`, gives up at `depth > 16` | `check/partial.hero:50` `reaches_within`, gives up at `depth > 16` |
| fallback direction | permissive | permissive |
| runtime guard | `map.c:85` `eq(key, key)` | generated `_eq`/`_hash` `hero_panic` |
| guard completeness | complete: every key insert hashes | complete: every comparison calls `_eq` |
| a working program deleted by the call-site refusal | `count([1.5, 2.5])`, in the repository | `any_equal(xs: one)`, compiled here, not in the repository |
| static hole reachable **without** a generic | unrun by me | **measured, depth ≥ 17** |

They differ in one respect only: **how many programs already in the tree the
refusal would delete** — one for float, zero for partial. That is a cost
difference, not a structural one, and it is exactly the warden's argument for
splitting R1. I support the split on that ground and on no other.

**What the panel must not conclude from either half.** Both guards must stay,
and neither may be re-described as redundant once the static routes are closed.
For my half I have the receipt: at nesting depth 17 the static rule is silent
and the `hero_panic` is the only thing standing between the program and a
comparison of bytes it cannot see.

---

## What I left UNRUN, named with the command that would settle it

- **The suites.** I ran none. `./heroes run tests/harness/main.hero -- ./heroes`
  and the `check` / `run` / `corpus` selectors would score any resolution here.
  `.claude/rules/verification.md` says a change to what the checker REFUSES is
  judged by every golden tree, so this proposal owes `check` `run` `emission`
  `determinism` `corpus`.
- **Whether the shipped SQLite bindings still build.** I did not build
  `examples/ledger/db/sqlite.hero` or `examples/sqlite/main.hero`.
  `./heroes build examples/sqlite/main.hero -o /tmp/x` settles it. My count
  above says neither file declares a generic, so I expect no change — but that
  is an inference from grep, not a build.
- **Whether a call-site check would in fact inherit the depth-16 bound.** That
  is a reading of `selfhost/check/partial.hero:49`, not a measurement of a
  compiler built from the proposal. Nobody has built one.
- **Cross-module**: a generic declared in one module, the `partial` record in
  another. Not compiled.
- **Linux and Windows.** `.claude/rules/platforms.md` — a program that declares
  an `extern` runs its Linux leg under `--sanitize`, and LeakSanitizer exists
  only there. Everything above is this Mac.
- **`--sanitize` on the six holder routes.** I ran it on two of the seven
  programs (the brief's toy and depth-17), not on all.

## Prediction

A golden case shaped like my `nest17.hero` — seventeen nested records with a
`partial` at the bottom and `a == b` at the top, no generic — will pass `heroes
check` at exit **0** and abort **134**, both on the compiler at `10480d65` and
on any compiler built from panel 155's resolution, because a call-site check
reuses `selfhost/check/partial.hero:50`'s bound. **The instrument that scores it
is the `run` suite, and the case cannot live in `tests/golden/check/` at all,
because it produces no diagnostic to snapshot.** If somebody writes it into
`tests/golden/check/` and it goes green, I am wrong.

Second, weaker and also falsifiable: `examples/ledger/db/sqlite.hero` and
`examples/sqlite/main.hero` need no shim and no edit under this rule — step 3 of
§4.19's ladder is untouched. The `corpus` suite scores it.

## Condition under which I change my verdict — and where my veto sits

I do **not** cast my veto. Nothing proposed changes layout, ownership, NUL
termination, refcount visibility, or `importc`-style header verification; the
`_Static_assert`s I compiled are untouched by every option on the table, and
refusing a bad generic call at the call is the §4.19 thesis moving in the right
direction.

**I cast it the moment a resolution touches
`selfhost/emit/structural.hero:57` or `:218`** — the `hero_panic` in the
generated `_eq` and `_hash` — on the ground that the static rule now covers it.
It does not. Depth 17 is measured above, and deleting that panic converts a
named abort into a comparison of bytes the program cannot see, at exit 0. That
is design.md §1.12, which outranks everything else in this sitting.

I would also move from approve to object if the resolution lands the call-site
refusal **without** filing the depth-17 hole as a defect, because shipping a
rule advertised as *now every route is a compile error* while a non-generic
route is still exit 0 is worse than the gap the sitting convened over: it
retires the reader's suspicion without retiring the defect.
