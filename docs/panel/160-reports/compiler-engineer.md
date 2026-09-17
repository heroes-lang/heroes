# Panel 160, compiler-engineer report

Seat: compiler-engineer (design.md §1.1 ceiling, §1.7 core plus elaboration, Part 5).
Sitting 2026-09-17, M-check-completeness, defect 050. Every number below was produced by
a command run in this session in a copy of the tree under the scratchpad (`ce160/`);
the working tree was not touched except for this file. Anything not run says UNRUN.

## Ground: the baseline compiler is the seed

`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed` (exit 0). Then
`heroes-seed build selfhost/main.hero --emit-c -o stage1-base.c` re-emits `seed/heroes.c`
**byte-identical**: 822,780 lines each, `cmp` exit 0. So `heroes-seed` IS the live
compiler and every baseline row below is measured on it.

Baseline probes reproduce all six rows of the shared brief's table plus `find` over `[i64?]`
(exit 0), `wrap<T>` applied to an `i64?` (exit 0), and three single-level controls (exit 0).
One correction to the brief: `.default(0)` on the nested value fires **two** diagnostics,
`bad_operand` (`print` of an `i64?`) and then `type_mismatch`, not one.

## 1. Core or sugar?

Neither. Part 5's row (`design.md:2567`) keeps `.must()`, `.default()`, `.is_err()` as sugar
for "a branch on the ok/err tag" and the prototype leaves that row, the lowering and the
emitter untouched: `git diff --stat` over the copy touches `selfhost/check/` and
`selfhost/flow_errors.hero` only, plus one new module. The change is a NARROWING of what the
checker accepts, a special case added to the checker (§1.7's criterion: it does not remove
one). No lexer, parser, descriptor, ownership, IR or emitter arm is added or changed.

## 2. Lines, and where they land (unit: `layout`'s `code_lines`, replicated in awk and
## validated against the suite's own row: `builtins.hero` = 377 = its DECIDED ceiling)

| file | before | E | A | ceiling | note |
|---|---|---|---|---|---|
| `selfhost/check/nested.hero` (NEW) | 0 | 40 | 40 | 300 | `is_nested`, `is_err_type`, `peel`; 44 wc |
| `selfhost/flow_errors.hero` | 117 | 134 | 134 | 300 | `nested_read` constructor: 10 code + 7 comment |
| `selfhost/check/builtins.hero` | 377 | 378 | 380 | **377** (`suite_layout.hero:407`) | E: +1 is the `use`; `is_err` arm rewritten one-for-one. A: +2 in the `default` arm; `must` arm one-for-one |
| `selfhost/check/access.hero` | 283 | 283 | 284 | 300 | A only: `use` line; `try_type` arm one-for-one |
| `selfhost/check/walk.hero` | 1863 | 1863 | 1863 | 1870 | untouched |
| **total added** | | **+58** | **+61** | | all in the checker and one errors module |

E breaches `builtins.hero`'s DECIDED ceiling by **1** (the import); A by **3**. The row moves
by its measured lines, as that file's own comments record happening at four milestones.

Landing costs beyond the prototype, UNRUN (not built here), estimated from the files read:
one golden under `tests/golden/check/` with `#~ nested_read` and `.expected` (§9); a
constructor test in `flow_errors.hero` (~5 lines); the DECIDED row and its paragraph;
and a gate in `selfhost/discard_errors.hero` (177 code lines): `discarded_failure`'s message
and its `guess` fix "drop the failure on purpose" (`discard_errors.hero:57,77,103,130`) send a
program to `.is_err()`, which E refuses on this value, so `answers()` needs one more bool the
way it already gates `.default(v)` on `has_payload` (~4 lines for E; A gates all three routes,
~8). Measured on `_ = m["a"]`: baseline, E and A all print the same `discarded_failure`.

## 3. `check` time (`/usr/bin/time -p ./heroes-X check selfhost/main.hero`, alone, 3 runs)

| compiler | real (s) | user (s) | median real |
|---|---|---|---|
| baseline | 18.53 / 17.65 / 17.27 | 18.23 / 17.42 / 17.19 | 17.65 |
| E | 18.56 / 17.79 / 17.65 | 17.96 / 17.64 / 17.58 | 17.79 |
| A | 17.75 / 17.29 / 17.21 | 17.18 / 17.16 / 17.14 | 17.29 |

`real` / (`user`+`sys`) is at most 1.04 in all nine runs: nothing was waiting. The three
medians sit inside the baseline's own 1.26 s spread; no slowdown is measurable at this scale.

## 4. R3, Principle 0: does the compiler apply a reader to a nested fallible?

**No, under either option.** `./heroes-E check selfhost/main.hero`: exit 0, 0 diagnostics.
`./heroes-A check selfhost/main.hero`: exit 0, 0 diagnostics. Static context: `selfhost/`
holds 1096 `.is_err()` sites, 1202 `.must()`, 55 `.default(`, 252 line-ending `?`, and
**0** generic functions. Neither option breaks the bootstrap; the price is zero sites.

## 5. Corpus breakage: none

368 files (`examples/` 120, `tests/golden/run/` 124, `tests/golden/check/` 124), each run
through `check --brief`, recording exit code and the multiset of error codes. Baseline:
230 exit 0, 138 non-zero (14 examples, 0 run, 124 check). Under E and under A the per-file
record is **identical to baseline** (`diff` exit 0, empty); `nested_read` fires on 0 corpus
files. The brief's inference about the 72 map-read sites is now run: all single-level.

**The `find` door**: all 6 `find(` sites are in `examples/readings/main.hero`, over
`good = all_week.filter(answered)` / `week().filter(answered)` / `empty`, and
`function week() -> [Reading]` (line 31): `[Reading]`, zero over a `[T?]`.

## 6. What E and A refuse and let through (probes, exit codes)

| probe | base | E | A |
|---|---|---|---|
| `m["a"].is_err()` on `{str: i64?}` | 0 | **1** | **1** |
| call form `is_err(m["a"])` | 0 | **1** | **1** |
| `if m["a"].is_err()` | 0 | **1** | **1** |
| `xs.find(f).is_err()` on `[i64?]` | 0 | **1** | **1** |
| `wrap<T>(v).is_err()` with `v: i64?` | 0 | **1** | **1** |
| `v: i64? = m["a"].must()` | 0 | 0 | **1** |
| `m["a"].must().must()` | 0 | 0 | **1** |
| `v: i64? = m["a"]?` in a fallible fn | 0 | 0 | **1** |
| `.default(ok(0))` | 0 | 0 | **1** |
| `.default(0)` | 1 (two codes) | 1 | 1 (`nested_read` replaces both) |
| `match` naming both levels | 0 | 0 | 0 |
| three single-level controls (`{str: i64}` read, `?`, `.must()`) | 0 | 0 | 0 |
| **`is_bad<A>(x: A?) -> bool` = `x.is_err()`, called with `A := i64?`** | 0 | **0** | **0** |

The last row is the hole both options share: a generic body is checked once with a
`.generic` payload, and instantiation happens after the checker in `selfhost/ir/mono.hero`
(368 code lines against a DECIDED 380). Closing it is a post-`mono` check, a second layer
in §1.7's sense, and nobody has costed it. Its live instances: 0 in `selfhost/`, 0 in the
corpus (8 corpus generics carry an `A?`; the two that touch it use `.len()` and `==`).

## 7. R2: what the diagnostic prints

The message never prints the outer type. It names the PAYLOAD, which is spellable:

    error[nested_read]: `.is_err()` reads one level, and this value is fallible twice — its
    payload is itself a `i64?`, so the failure stored inside would go unread
      note: `match` names both levels: `.ok inner` when the outer level succeeded, then
      `match inner` for the `i64?`; `.err e` when it did not

So the rule does **not** wait on panel 158 R1's `(i64?)?`. And the premise behind R2 is
already false in the shipping compiler: the baseline's `discarded_failure` on `_ = m["a"]`
prints "`would drop the error in this `i64??``" today. `i64??` is a spelling the parser
recognises and refuses under `nested_fallible` (`selfhost/parse/type.hero:55`), not gibberish.

## 8. The fix

None; one note. Both repairs (the outer question via `match`, the inner via `.must()` then
`.is_err()`) change what the program means, and a `match` skeleton is multi-line control
flow: `guess` at best, and `.claude/rules/diagnostics-and-goldens.md` § Errors are a
deliverable says a fix that leaves the defect standing is a guess however well it compiles.
Precedent in the same file: `missing_return` (`flow_errors.hero:117-122`) ships a note and no
fix for exactly this shape. The prototype ships zero `Fix(`.

## 9. A fifth reader? No

`grep -rn "\.fallible [a-z_]* *=>" selfhost/check/` lists 33 arms. Every arm that PRODUCES a
value from the payload keeps it: `builtins.hero:58` (`.default`'s argument type), `:346`,
`:359`, `access.hero:302` (`?`), `walk.hero:857` (map-write place: `m[k] @ v` takes a `V`,
correct), `:1087` (`match .ok`), `:1974` (`ok`/`fail` against an expected type). The rest are
predicates or notes that hand no value to the program (`is_fallible` twice,
`carries_a_payload`, `ordering.hero:87`, exhaustiveness at `walk.hero:1164`, the type walkers).
The one reader whose RESULT type forgets the payload is `is_err` at `builtins.hero:378`,
which did not even bind `o`. E names exactly that arm.

## 10. Two interactions outside this seat, measured and handed on

- design.md's map section (`:1388-1390`) strikes `has(m, k)` because `!m[k].is_err()` is
  its spelling. Under E or A a `{K: V?}` map has no one-line presence test; `match` only.
  That paragraph owes a sentence, or option C is where it goes. llm-ergonomist / spec-warden.
- Under A, `tests/golden/check/a-fallible-type-is-never-written-fallible-twice.hero`'s
  comment ("`m[k]` hands back a doubly-fallible value that `.must().must()` consumes") is
  falsified (probe: base 0, E 0, A 1), and `tests/golden/` is append-only, so A owes a dated
  correction under it. E does not.

## Verdict block

- **verdict**: approve (option E: refuse `.is_err()` alone on a value whose payload is
  itself fallible). No veto: no core construct, no lowering or emitter arm, Part 5 unchanged.
- **section**: design.md §1.7 (core plus elaboration; "does it remove a special case?"),
  under §1.1's ceiling; Part 5 row `design.md:2567`.
- **implementation_cost**: E **+58** `code_lines`, all checker plus one errors module:
  `selfhost/check/nested.hero` NEW 40, `selfhost/flow_errors.hero` 117 -> 134,
  `selfhost/check/builtins.hero` 377 -> 378 (DECIDED 377 moves by 1). A is **+61**
  (`builtins.hero` 380, `access.hero` 284). `check selfhost/main.hero` median 17.65 s ->
  17.79 s (E) / 17.29 s (A), inside the baseline spread. UNRUN landing extras: golden,
  constructor test, DECIDED row, a ~4-line gate in `discard_errors.hero`.
- **needed_for_self_hosting**: no. The refusal is not a form the compiler needs; the
  compiler survives it at 0 sites (`check selfhost/main.hero` exit 0 under E and under A).
- **argument**: The ceiling does not discriminate: E and A are 58 and 61 checker-side
  lines, zero bootstrap sites, zero corpus files, flat `check` time. §1.7 does. E adds one
  special case at the one reader whose result type forgets the payload; A adds four, and
  three of them refuse idioms (`.must()`, `?`, `.default(ok(v))`) whose second level
  survives in the TYPE and is caught one line later, so they close no silent door while
  costing first-attempt compiles of correct programs (§1.1's objective) and two more repairs
  (a golden's comment, `discarded_failure`'s three routes). Both share the generic-body
  hole, uncosted and live nowhere. Take E; record A as the complete-but-costlier form.
- **prediction**: at the M-check-completeness close with E landed, `suite_layout.hero`'s
  row reads `selfhost/check/builtins.hero 378`, `selfhost/check/nested.hero` is at most 60
  `code_lines`, `heroes check selfhost/main.hero` medians within 0.5 s of 17.65 s on this Mac,
  and `nested_read` fires on 0 files under `examples/` and `tests/golden/run/`. If landing E
  touches any file under `selfhost/ir/` or `selfhost/emit/`, this report was wrong.
- **condition**: I move to **object** if a seat shows a program in `selfhost/` or
  `examples/` that must apply `.is_err()` to a nested fallible where `match` cannot serve
  in three lines. I move to **veto** if the resolution requires closing the generic-body hole,
  because that is a post-`mono` check in `selfhost/ir/` (368 of 380), a second layer under
  §1.7 whose cost nobody has measured. I withdraw the §1.7 objection to A if a seat measures
  a program where `.must()`, `?` or `.default(ok(v))` compiles at exit 0 and returns a wrong
  answer with no type surfacing the second level; my probes found none.
