# Panel 159 — compiler-engineer report

- **verdict**: object
- **section**: design.md §1.1 (implementation simplicity is a constraint, sets
  the ceiling) and §1.7 (core plus elaboration). The objection is not to a cost:
  it is that **two of the three "silences" are not silences of the same kind**,
  and one of them sits on top of a defect this seat reproduced.
- **needed_for_self_hosting**: no for all four resolutions (the compiler compiles
  itself today with the spec exactly as it stands). Principle 0 does not carry
  this sitting; design.md Part 11 / §1.6 has to.
- **veto**: NOT cast. No resolution on the table adds a core construct. The
  fourth option I was asked to price (accept `-> ()?`) WOULD, and I say below at
  what price and why I would veto it if it were proposed as written.

All numbers below were produced this session by commands run in a scratchpad
copy at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/bf49271c-9c48-4701-9a34-ec92d4cc09af/scratchpad/tree`,
built with `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed`.

---

## R1 — `sort` is ascending BY CONSTRUCTION, and a silent reversal is impossible

**By construction, in twelve places.** `runtime/parts/sort.c` (217 lines) holds
twelve comparators, each written `return a < b ? -1 : (a > b ? 1 : 0);`
(`:34-112`), dispatched by pointer identity on the static descriptor in
`hero_cmp_for` (`:120-134`). `hero_merge_two` (`:139-155`) takes from the right
run only when `cmp(hi, lo) < 0`, which is what makes it ascending AND stable.
Ascending is therefore a decision written twelve times plus once in the merge,
not a property of a library sort.

**And it cannot reverse in silence.** `tests/golden/run/builtins.hero:19-37`
pins it on four element kinds and `tests/golden/run/builtins.expected` holds the
answers: `123` (line 1), `aladdin|aladdin|pearl|sane` (3), `0.5 2.5` (4),
`falsefalsetruetruetrue` (5). A reversed comparator moves four expected lines and
the `run` suite goes red. **So the document's sentence does not need to be
defensive.** One word, "ascending", is the whole cost, and the instrument that
keeps it true already exists.

**Correction to the shared brief, and it is load-bearing for the sentence's
wording.** The brief's table says *"run on all three orderable kinds"*. The
orderable set is **twelve**: `i64 i8 i16 i32 u8 u16 u32 u64 f32 f64 str bool`
(`runtime/parts/sort.c:121-132`). So a sentence of the form *"`sort` is
ascending on `i64`, `str` and `bool`"* would be born stale. `spec/heroes-spec.md:294`
already says it correctly and generically — *"`sort` (a number, `str` or `bool`,
never a type parameter)"* — so the sentence R1 owes is **one word added to an
existing clause**, not a new clause.

**A second rot, still live, same shape.** `runtime/heroes_runtime.h:421-422`
still says `sort` *"Works on `[i64]`, `[f64]` and `[str]`, which is exactly the
set with an order"*. That is false since M-sized-integers and panel 068.
`tests/golden/run/builtins.hero:8-11` says in its own comment that the emitter's
copy of that list was found stale "until this sitting" — the runtime header's
copy was not. Not this panel's business to fix, but it is the evidence that a
TYPE LIST rots and the word "ascending" does not.

## R2 — `xs[i] @ v` is core, deliberate, and the neighbouring sentence would be WRONG

**It is not an accident of the `Place` production.** There is a dedicated
place-path walk in `selfhost/emit/container.hero` (393 lines), `:150-234`, with a
`.index_step` arm at `:195-230` that unshares once per array step and writes back
at each level; `runtime/parts/cow.c` (126 lines) exists only to hold that
argument; `selfhost/emit/container.hero:391-393` unit-tests the emitted lines; the
arm carries its own fixedbugs note from 2026-08-17. design.md:1343-1349 builds a
step-by-step alias test on `@xs[0]` / `@xs[1]`. This is a core form with a
runtime part, an emitter arm, a test and a panel behind it.

**Measured: `xs[i] @ v` through a FIELD does NOT copy the whole array.** Four
programs, `--emit-c`, all exit 0:

| program | emitted call |
|---|---|
| `xs @ xs.push(4)` on a name | `hero_array_push_owned(&h0_xs, &t6)` — grows in place |
| `b.xs @ b.xs.push(4)` through a field | `hero_array_push(t7, &t8)` — **always** a new array (`runtime/parts/array.c:116-132` allocates `len+1` unconditionally) |
| `xs[1] @ 99` on a name | `hero_array_set(&(h0_xs), t5, &t6)` |
| `b.xs[1] @ 99` through a field | `hero_array_set(&(h0_b.f_xs), t6, &t7)` |

`hero_array_set` (`runtime/parts/cow.c:111-126`) bounds-checks, then calls
`hero_array_unshare`, which **returns immediately when `refcount == 1`**
(`:44`). So the indexed store through a field copies **only when the array is
shared**, and the copy is observable exactly there: `ys = xs` then `xs[0] @ 99`
prints `99 1` (run, exit 0), and `g.rows[0][1] @ 99` with `h = g` prints `99 2`
(run, exit 0) — per-step unshare through two array levels.

**This is the finding that changes the sentence.** `spec/heroes-spec.md:283-285`
says of `push`: *"reached through a field or an index it copies the whole
array"*. That is TRUE for `push` and **FALSE for `xs[i] @ v`**, measured above. A
merged sentence that reaches for economy by attaching `xs[i] @ v` to the push
clause would publish a false performance rule. The sentence R2 owes must
distinguish, and it costs more than one clause.

**And `xs[i] @ v` needs a CELL.** `xs = [1,2,3]` then `xs[1] @ 99` is refused:
`error[not_mutable]`, exit 1. The brief's *"yes, exit 0, prints 99"* holds only
for `xs: [i64] @ [1, 2, 3]`. Any spec example must be written with `@`, or the
reader copies a program that does not compile.

### DEFECT found this session, on exactly R2's form

```
function main()
    s: str @ "abc"
    s[0] @ 65
    print(s)
```

`heroes check` exits **0**. `--emit-c` exits 0 and writes
`hero_unreachable(); /* not an element write */` (line 119 of the emitted C).
`heroes run` exits **134**:
`panic: entered unreachable code — this is a compiler bug, please report it`.

The `Place` production derives `s[i] @ v`, `spec § 10` says `s[i]` yields a `u8`,
and nothing between the grammar and the runtime refuses the store: the
`.index_step` arm at `selfhost/emit/container.hero:201-206` matches `.str` into
the do-nothing group, falls through to `:216` `if element < 0` and returns the
`"not a container step"` failure, which the caller turns into `hero_unreachable`.
This is the identical shape the same file's comment at `:164-169` records being
fixed for `xs[i].f @ v` on 2026-08-17. It is not in `docs/work/DEFECTS.md` (one
open item, not this).

**Reproduced on the SELF-HOSTED compiler, not only the seed.**
`./heroes-seed build selfhost/main.hero -o heroes-next` succeeded this session;
`heroes-next check n1.hero` exits **0** and `heroes-next run n1.hero` exits
**134** with the same panic. Exit codes captured directly, not through a pipe.
`heroes-next check m1.hero` (the `-> ()?` program) exits **1**, and
`heroes-next run idx3.hero` (`b.xs[1] @ 99`) exits **0** printing `1993` — so
every R2 and R3 number here holds on the compiler that ships.

**Why this binds the panel rather than a later step.** design.md's thesis is
that every plausible mistake is a compile error. A reader who is TAUGHT
`xs[i] @ v` by a new spec sentence, and who has also been taught `s[i]` yields a
`u8`, will write `s[i] @ v` — the sentence makes the defect more likely to be
reached, not less. The spec change and the checker refusal should land together.

## R3 — `main` may not be `-> ()?`, the refusal is right, and the fourth option is expensive

`selfhost/value_errors.hero:165-172` is the diagnostic; the site is
`selfhost/check/decls.hero:81-83`, whose condition is
`result != table.unit_ty(c.out.types)`. Run: `function main() -> ()?` produces
**two** diagnostics, `main_returns` and `missing_return`, exit 1.

**What `main` would do with a failure: nothing exists for it.** The emitted entry
point is `selfhost/emit/decls.hero:266-278`, and it is four lines with a
hard-wired `return 0`:

```
int main(int argc, char **argv) { hero_args_set(argc, argv); h_X_main(); hero_runtime_check_leaks(); return 0; }
```

Accepting `-> ()?` therefore needs, at minimum: the refusal removed
(`selfhost/check/decls.hero:81-83`) **and its golden and its annotation**; the
shim taught to test the fallible tag, render the failure and return non-zero
(`selfhost/emit/decls.hero:266-278`, ~15 lines, plus the `hero_runtime_check_leaks`
ordering question on the failure path); a **rendering** for a `HeroFailure` at
the process boundary, which `runtime/parts/failure.c` (125 lines) does not have —
it has retain/release/eq/hash and the static failures, no printer; and a
**specified exit code**, which is a new promise the spec has never made about a
compiled program.

**That last item is why I would veto it as written.** The exit code of a Heroes
program becomes an observable the spec must state, the goldens must pin and every
platform must agree on — a language-level promise, not a compiler detail. Panel
035's finding is that the SILENT version of this shipped once already
(`main() -> i64?` returning `fail(...)` printed nothing and exited 0). Replacing
a good refusal with a new cross-platform promise is the opposite of the ceiling
rule. **The fourth option exists; it is the most expensive thing on the table and
it buys a reader nothing the diagnostic does not already give them.**

**A fourth silence nobody listed**, measured: `function main() -> ()` — explicit
unit — **compiles and runs, exit 0**. So a sentence phrased *"`main` takes no
result type"* would be false as written. The true sentence is the diagnostic's:
`main` **produces nothing**.

## R4 — merging, with one limit

Merging is right for R1 (one word into `spec:294` or `:288`) and for R3 (a clause
onto the `function main()` sentence). It is **wrong for R2**, because the nearest
sentence, `spec:283-285`, states a copy rule that this seat measured to be false
for `xs[i] @ v`. Two merges and one new clause, not three merges.

## The standing question: what would have to be true for a route nobody listed?

The listed routes all assume **the compiler is right and the document is
behind**. The defect above is a fifth route: *the document is behind AND the
compiler is wrong on a neighbouring shape*, so the spec sentence and a checker
refusal land in one step. That route exists whenever a form is taught for the
first time, because teaching a form is what sends readers to its edges.

## UNRUN

- The `run`, `check`, `emission`, `determinism` and `corpus` suites. Never run
  the full net (watchdog). The commands that would settle them:
  `./heroes run tests/harness/main.hero -- ./heroes run` and the same for each name.
- Whether any program in `examples/` or `tests/golden/` contains `s[i] @ v` in a
  shape my regex missed. Measured: **90** indexed stores across those two trees,
  **0** matches for a store into a `str` index. A negative grep is my vocabulary,
  not the world.
- Timings. No `/usr/bin/time -p` figure is reported here, so no cost claim about
  compiler speed is made.

## prediction

Falsifiable, at the close of the milestone that lands panel 159's spec change:
**a `tests/golden/check/` case containing `s: str @ "abc"` / `s[0] @ 65` will
require a NEW checker refusal to be a check case at all** — i.e. at the commit
that adds it, `heroes check` on that file exits 0 and `heroes run` exits 134
until a diagnostic is written, and the diagnostic costs **under 40 lines**: one
constructor in a `selfhost/*_errors.hero` module (the **10** constructors in
`selfhost/value_errors.hero` average 337/10 ≈ 34 lines
each, and the `main_returns` constructor is 8 lines, `:165-172`) plus one guard in the walk in
`selfhost/emit/container.hero` or its checker counterpart. Falsified if the
refusal exceeds 40 lines, or if `heroes check` on that program already exits
non-zero at that commit.

## condition

I withdraw the objection to R4's three-way merge if a run shows
`b.xs[1] @ 99` allocating a new array on the unique path — i.e. if somebody
produces a program where the indexed store through a field copies the whole array
with refcount 1, making `spec:283-285`'s clause true for `xs[i] @ v` after all.
`hero_array_unshare`'s `if (a->refcount == 1) return;` at `runtime/parts/cow.c:44`
is the line that would have to be wrong.

I raise the objection to a **veto** if the sitting adopts the fourth option
(accepting `main() -> ()?`) without first specifying the program's exit code in
`spec/heroes-spec.md` and pinning it in a golden on all three platforms.
