# Panel 155 — compiler-engineer report

**verdict**: R1 **veto** · R2 **object** · R3 **approve** (spec is qualified, not
the compiler) · R4 **object** (the answer differs, and the sitting must split)

**section**: design.md §1.1 (implementation simplicity is the ceiling) and §1.7
(core plus elaboration — "it determines the size of your compiler"). Part 5's
core test does **not** carry: I say so explicitly below rather than invent it.

**needed_for_self_hosting**: **no**. `grep -rnE '^function [a-z_]+<' selfhost/`
= **0**, run this session. Principle 0 does not fund this.

---

## Every number below, and how it was produced

All commands run 2026-09-16 in a scratchpad copy of the tree, compiler built
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (**real 3.187 s**,
exit 0). Exit codes captured directly, never through a pipe.

### The brief's ceiling number is stale, and the correction cuts for the proposal

The brief says `check/walk.hero` "measures **1708** code lines against a DECIDED
ceiling of exactly **1708** … so a ~90-110 line pass has nowhere to go." Both
halves are false today.

- `tests/harness/suite_layout.hero:384-405`, the `DECIDED` table, reads
  `"selfhost/check/walk.hero 1870"`. Panel 127 raised it; the brief quotes the
  pre-raise figure.
- Measured in the suite's own unit — an awk replica of `code_lines`,
  `tests/harness/suite_layout.hero:502-519` (non-blank lines outside `test`
  blocks; comments count) — `check/walk.hero` is **1861**.
- `./heroes run tests/harness/main.hero -- ./heroes layout` → exit **0**,
  `layout: 2 passed, 0 failed`.

The replica is validated by two independent rows it reproduces exactly:
`check/table.hero` **389** against ceiling 389, `check/builtins.hero` **377**
against 377.

| file | `wc -l` | layout unit | ceiling | headroom |
|---|---|---|---|---|
| `selfhost/check/walk.hero` | 2290 | **1861** | 1870 | **9** |
| `selfhost/check/generics.hero` | 413 | **274** | 300 (`CEILING`, `:43`) | **26** |
| `selfhost/check/state.hero` | 240 | **192** | 300 | 108 |
| `selfhost/check/map_keys.hero` | 225 | **167** | 300 | 133 |
| `selfhost/check/partial.hero` | 119 | **90** | 300 | 210 |
| `selfhost/ir/mono.hero` | 505 | **368** | 380 | 12 |
| `selfhost/check/table.hero` | 539 | **389** | 389 | **0** |
| `selfhost/check/builtins.hero` | 544 | **377** | 377 | **0** |

So the ceiling objection is *weaker* than the brief claims. I am not standing on
it, and I say so rather than bank a number I disproved.

### The probes reproduce

- `tally<K>` keyed at `f64`: `check` **0**, `build` **0**, run **0**, prints `1`.
- `tests/golden/run/abort-map-key-nan.hero`: `build` **0**, run **134**, prints
  `2` then `panic: a map key that is not equal to itself (nan)`.
- `/usr/bin/time -p ./heroes check selfhost/main.hero` → **real 16.98, user
  16.92, sys 0.04**, exit 0. This is the baseline the pass must not move.

### The prototype, written and partially checked

`selfhost/check/obligations.hero`, written this session in the copy: **335
`wc -l`, 284 in the layout unit**, plus **22 wiring lines** across three files
(`check/state.hero` +11, `check/walk.hero` +1, `checker.hero` +10). It carries
all three parts panel 082 R3 names — producers, fixpoint, consumer.

**Status, stated honestly: it does not yet pass `heroes check`.** The last
COMPLETED run was exit 1 with **3** `non_exhaustive` errors; the patch for them
was applied and **its verification is UNRUN** — I was cut off on the watchdog.
The command that would settle it is
`./heroes check selfhost/main.hero` (~17 s baseline). So **284 is a floor, not a
final figure**: every remaining error adds arms, never removes them.

Against the brief's own estimate of "~90-110 lines", the measured floor is
**284 + 22 = 306 code lines, 2.8x the estimate**, and it is not finished.

Wired, `check/walk.hero` goes 1861 → **1862** (8 of headroom left). The pass has
a home; it does not fit in `walk.hero`, and it never needed to.

---

## implementation_cost: where it lands, and the three seams the brief does not name

The brief says "the seam exists and is not speculative." Three parts of it do
not exist, each measured:

**1. There is no node-to-declaration map in this compiler.**
`ast.Ast` is three flat arenas — `exprs`, `stmts`, `types` (`selfhost/ast.hero:499-503`).
`grep -rn "decl_of\|owner_decl\|decl_for\|node_decl" selfhost/` returns
**nothing**. The obligation store is keyed `(decl, param)`; nothing in the tree
can say which declaration a node belongs to.

**2. The two rules live in two different architectures, so there are two
producers, not one.**

- `float_map_key` is a **whole-program arena sweep** with no declaration in
  hand: `map_keys.check` walks `a.types` and `a.exprs` flat
  (`selfhost/check/map_keys.hero:36-58`), called once from
  `selfhost/checker.hero:58`, after `check_decls.file`.
- `ffi_partial_operation` on `==` is **inside the expression walk**,
  `selfhost/check/ops.hero:47`, reached from `selfhost/check/walk.hero:172`.

The sweep cannot hook the first producer, so the prototype pays for a **second
full walk of every generic body** (`collect`/`block`/`statement`/`expression`,
~80 of the 284 lines) and the fixpoint pays for a **third** (`propagate`/
`edges_*`, ~70 lines). That is 150 lines that exist only because fact 1 is true.

**3. `instantiations` does not record who was called.**
`selfhost/check/state.hero:86` is `instantiations: {i64: [i64]}` — type arguments
only, keyed by `span.end`. `ir/mono.hero:170` gets the callee from the **IR
instruction** (`cl.callee.heroes_fn.decl`), which a checker-level consumer does
not have. So a new table is required. It is one line at the recording site
(`walk.hero:1802`, `decl` is in scope) plus the record field — but it is a new
span-keyed table, and `state.hero:78-85` carries a standing rule against exactly
that shape:

> the other two span-keyed tables … key on a `name.start`, and they are sound.
> The start of a TOKEN is a fact … **Any future table keyed by a span belongs on
> the first side of that line.**

`instantiations` earns its `span.end` key with a written falsifiable claim and a
named golden that fails without it. A second table inheriting that key inherits
the claim without restating it. **That is a soundness cost, and it is where my
veto bites**, not the line count.

**Not core.** By §1.7's own test — "anything in the core must be implemented in
the type checker *and* the lowering *and* the backend" — this is **sugar-side**.
Generics are already a Part 5 sugar row, erased by monomorphisation, and the
pass adds no IR node and no emitter arm **provided** the callee table is kept
separate from `instantiations`. If instead `instantiations`' value type is
widened, it touches four downstream readers — `ir/mono.hero:170`,
`ir/mono.hero:298`, `emit/members.hero:118`, `emit/inst.hero:201` — and becomes
core by §1.7's test. **The panel must write down which of the two it is
adopting**; the brief does not distinguish them.

**It is a second answer to a question the checker already answers.** The
prototype's `as_key` is a copy of `map_keys.refuse` and `as_equal` is a copy of
`ops.hero:47`'s arm. They cannot be shared: the first reports at a span inside
the body, the second at the call. `check/map_keys.hero`'s own header states the
principle this violates — *"the partial rule and the float rule in a second walk
in a second file is exactly the shape that hid sort's defect for a milestone —
one walk cannot drift from itself."* The proposal makes that two walks in two
files, by construction.

## Termination

Finite, and the bound is small. The prototype's `propagate` iterates to a
fixpoint over `(decl, param)` pairs; each round can only **add** bits, and there
are **2** bits (`MAP_KEY`, `EQUALITY`). So the bound is `|pairs| * 2` rounds,
and in practice **2**: one to propagate, one to observe no change. Self-recursion
adds nothing on round two; mutual recursion converges on the union. The brief is
right here and I confirm it **by construction, not by execution** — running it is
UNRUN, blocked on the same unfinished `heroes check`.

One termination hazard the brief does not name: my `reaches_generic` needed a
`depth > 16` give-up, copied from `reaches_float`. **That bound is not safe in
the same direction here.** `map_keys.hero:118`'s comment licenses its own bound
because *"the runtime guard still sits behind this rule"*. A missed **obligation**
is a call this rule exists to refuse and silently does not — the exact defect 035
shape that `check/reaches.hero:55-66` records the handle walk paying for.

## Blast radius

Not run against a prototype — the prototype does not compile yet, and the full
net exceeds my time budget. **UNRUN**, and the command that would settle it is
`./heroes run tests/harness/main.hero -- ./heroes` plus the five suites
`.claude/rules/verification.md` names for a refusal change: `check` `run`
`emission` `determinism` `corpus`. That rule is explicit that **a change to what
the checker REFUSES is judged by every golden tree, not by the `selfhost/**`
row** — it cost six red checks at M-marked-acquisition.

What I did measure: `grep -rnE '^function [a-z_]+<'` over `examples/ tests/
spec/ selfhost/ docs/` = **61** matches, **0** in `selfhost/`.

I accept the spec-warden's finding that the count of dead goldens is **two**, not
one, and it strengthens my verdict rather than changing it: the second file's own
comment records it was **already rewritten once** to route through a generic, so
the proposal closes the route that rewrite moved to. That is a rule whose
previous application the repository had to work around, applied a second time to
the workaround.

## The route nobody costed, which the brief asked me to look for

**Make `build` accept what `check` accepts, not `check` refuse what `build`
refuses.** The gap table in the shared brief shows `check` 0 / `build` 0 / run
134 — the divergence is at **run**, not between `check` and `build`. Both already
agree. The honest repair is therefore at the **runtime**, and it costs nothing on
the surface: `hero_panic` already fires with a named message; making it name the
**call site** turns a located-in-the-wrong-place abort into a located one. Zero
surface change, zero spec change, zero §1.7 cost, no golden dies, the runtime
guard stays tested. I did not price this in lines (UNRUN) but it touches
`runtime/` and no checker pass, so it cannot breach a `selfhost/` ceiling.

**A warning is not available.** I looked; the brief asked me to check. I did not
find a warning severity the checker can emit for a rule of this class, and I mark
that as a **question rather than a premise**: what I searched for was a severity
field on `diag.Diagnostic`, and a negative claim rests on my vocabulary.

---

## argument (≤120 words)

The proposal is sugar-side, so §1.7's core test does not veto it. Three measured
facts do. There is **no node-to-declaration map** in `selfhost/` (grep: zero
hits), so the store's own key cannot be computed without two new full walks of
every generic body — my prototype floors at **306 code lines against the brief's
90-110**, and it does not yet compile. It needs a **second span-keyed table**,
against `state.hero:78-85`'s written rule. It duplicates `map_keys.refuse` and
`ops.hero:47` in a second file, the exact drift `map_keys.hero`'s own header says
one walk exists to prevent. And it kills two goldens, one of which was already
rewritten onto this route. Principle 0 funds none of it.

## prediction

At the close of M-check-completeness, if the pass is built as specified, `awk`
on the layout unit will read `selfhost/check/obligations.hero` **≥ 284 code
lines**, and the sum of it plus its wiring will exceed **300** — so it needs a
`DECIDED` row of its own in `tests/harness/suite_layout.hero`, which is a
ceiling raise the brief says the proposal does not need. Scored by
`./heroes run tests/harness/main.hero -- ./heroes layout`. I predict further that
`./heroes run tests/harness/main.hero -- ./heroes run` goes **red on exactly two
files** before any golden is rewritten.

## condition

I withdraw the veto on any one of three:

1. A prototype that **passes `./heroes check selfhost/main.hero`** at **≤150
   code lines total including wiring** in the layout unit, with the full net
   green and no golden rewritten. That falsifies my cost claim directly.
2. A demonstration that the pass closes a **robustness** class (§1.12, CLAUDE.md
   § Precedence rank 3) rather than a consistency one — i.e. a program where the
   missing refusal produces a **wrong answer or memory corruption**, not a named
   abort. Panel 084 measured both rules as consistency; §1.12 outranks compiler
   size and would fund this at once.
3. The question is **split** as the spec-warden proposes, and only
   `ffi_partial_operation` proceeds. I would vote **approve** on that half: spec
   § 13 promises it unconditionally, so it is a bug fix at +0 spec tokens, and
   `as_equal` is the small half of my prototype. `float_map_key` has no spec
   sentence behind it, kills both goldens, and waits under Principle 0.
