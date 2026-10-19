# Panel 225, shared brief: a value only the probe sees, told beside a refusal of its own unit

**Convened 2026-10-18** by the author's answer that evening, meant as *yes, now*, to the
coordinator's question whether to convene a sitting on defect 2137 and repair defect 2136 so that
`m-witnessed-seed` can be placed; on defect 2137
(https://github.com/heroes-lang/heroes/issues/2137), `adjacent` since 2026-10-18. It and defect
2136 were opened at 05:01 UTC on 2026-10-18 and `m-readonly-lend` was placed at 05:07:29 UTC
(`gh issue view <n> --json createdAt`, `git tag --list m-readonly-lend
--format='%(creatordate:iso-strict)'`, run by the coordinator 2026-10-18), so the next milestone
tag would be their second, where `records/tagged` counts an `adjacent` item as `blocking`
(`counts_because` in `tests/harness/suite_records.hero`, `AGED_AT` 2 in
`tests/harness/records_github.hero`). **Lane: soundness** (compiler-engineer, ffi-pragmatist, a
completeness critic): no surface, no spec token; the diagnostic class `ffi_constant_type` is
unchanged, and the question is WHEN it is told. The critic says if that reading is wrong.

**The tree**: main and the lane `lane-witnessed-seed` at `7b97b9438` (`git log -1`), M-witnessed-
seed's close, pushed. While the sitting runs the coordinator repairs defect 2142
(`selfhost/cli/link.hero`) in that lane; your copy is made from `7b97b9438` and never sees it.
Defect 2136 waits for this sitting's resolution.

## The question

Panel 216's R1 (ratified 2026-10-17) reads a header value C leaves undefined by running the
program's own module unit, preprocessed under the program's own words, compiled with clang's
checks in trap mode (`selfhost/cli/value_run.hero`, `selfhost/cli/value_ask.hero`). Defect 2128's
repair (M-readonly-lend step 7, `selfhost/cli/value_beside.hero`) asks that probe beside a build's
earlier refusal: every unit that compiled, and the refused unit again, with each constant clang
refused written inert (`emit/constant_accessor.inert`), **only where every error clang reported
stands in a constant's accessor** (`refused`, lines 61 to 88; `again`, lines 90 to 122). Where an
error stands anywhere else, the refused unit is not asked again (the module's comment, lines 18
and 19).

Defect 2137 is that anywhere else. Reproduced by the coordinator on 2026-10-18 with the lane's
stage 2 (`heroes --version`: `sources 08c492ac05cbb532`), in
`.claude/worktrees/scratch-clean-list/repro/`: `d2137.hero` binds `function narrow(x: i64) -> i32`
against `fixedbugs-2128-narrow.h`'s `static inline int narrow(int x)`, and `constant K_DIV: i64`
against `fixedbugs-2128-values.h`'s `#define K_DIV ((-2147483647 - 1) / -1)`, in one module, and
calls both in `main`. `heroes build d2137.hero -o d2137` prints `error[ffi_parameter_type]` for
`narrow` alone, exit 1; with the parameter declared `i32` (`d2137-next.hero`) the next build prints
`error[ffi_constant_type]` for `K_DIV`, exit 1. The same pair across two modules is told in one
build (`tests/golden/unsupported/fixedbugs-2128-a-value-only-the-probe-sees-is-told-beside-another-module-s-parameter.hero`).

**How is a value only the probe sees told in the same build as a refusal of its own module's unit
at a line no constant's accessor holds?**

Beside it, defect 2136 (https://github.com/heroes-lang/heroes/issues/2136), the same class one
step earlier: `d2136.hero` binds `function fill(@out: i32)` against `fill.h`'s `static inline void
fill(int64_t *out)` beside the same `K_DIV`; the build prints `error[ffi_parameter_type]` for
`fill` alone, exit 1, and `K_DIV` once `d2136-next.hero` declares it `i64`. Its refusal is made in
`selfhost/cli/rounds.hero`'s `round` at `probing.check_all`'s verdict (line 179), before
`cli/assemble.hero` emits any unit. A route the sitting adopts says whether it reaches that shape
too; the coordinator repairs 2136 after the resolution, by it where it does.

## Routes the coordinator listed (not the option set)

- **A, the refused unit asked as it is, under the probe's words.** `value_run.compiled` (lines 122
  to 133) compiles a probe's text under the unit's words, then the checks, the level, `-g0`,
  `-ffunction-sections`, `-fdata-sections`, `-w` and `-c`. Where the program's compile refused the
  unit for a warning its words or the unit's pragmas made an error, the probe's `-w` may let the
  unit compile; where the probe's compile still fails, its failure is not told beside the author's
  refusal (`value_beside.hero`, lines 21 to 24). Whether `narrow`'s refusal is such a warning is a
  question: unrun.
- **B, a copy with the refused binding written inert too**, as `constant_accessor.inert` writes a
  refused constant's accessor: the binding's code and its calls made to read nothing of the header.
  Unbuilt.
- **C, a unit of the constants alone**: the unit's own preamble, its include lines and defines, and
  the constants' accessors, preprocessed under the program's own words, the module's functions left
  out. Panel 216 chose the program's own unit so that a header reads as the program reads it:
  whether C keeps that reading, and where it does not, is what this route asks.
- **D, left as it is**: the value told on the next build. 2137 stays open, and the next tag counts
  it `blocking`.

## What is to be measured, not argued

1. The defect reproduced in your copy with your own stage 2, the reproducers above copied into
   your directory.
2. For the route you would adopt: built in your copy; `d2137.hero` read in one build, both
   diagnostics, exit 1; the 2128 cases and the 2072 case unchanged (`./heroes run
   tests/harness/main.hero -- ./heroes unsupported fixedbugs-2128`, and `fixedbugs-2072`); and
   whether it reaches `d2136.hero`.
3. The cost: a program binding no header value builds as before, its probe not run, cold and warm;
   a refused build runs no extra compile beyond what the route needs, and you say how many.
4. The ffi-pragmatist's: a header whose value depends on what the program's own words or the unit's
   other text set (a define the program passes, a header included before, a `static inline` the
   constant's expression calls), read by the route's unit as the program's own unit reads it, or
   the route says where it does not.

## Rules for every seat

Your own copy inside the repository's root, made with `git -C /Users/joseph/Temp/heroes/heroes-lang
worktree add --detach .claude/worktrees/225-<seat> 7b97b9438`; never another seat's directory and
never the lane. Your compiler is built inside it: `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes-s1`, then `./heroes-s1 build selfhost/main.hero -o heroes` (stage 2, which the guard asks
of a run of the net). Report to `docs/panel/225-reports/<seat>.md` in the lane
(`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-witnessed-seed`), as you go. **No paid
run**, **no credential printed** (`gh auth status` only). A program that may not end runs as a
built binary under `timeout`, its output bounded. Every number carries its command; a claim not run
says *unrun*. Commit nothing, and leave your worktree for the coordinator to remove.

## The lane's condition

The soundness lane holds for a resolution that writes nothing into design.md §4.17 or spec § 13.
§4.17 (panel 187, ratified 2026-10-03) counts *a mistake of the parse stage told only once another
is fixed* among what a broken program costs, measured rather than promised, and says *a later
stage's mistakes wait for the earlier stage's by design, since `heroes check` runs a stage only
when the one before said nothing*; both refusals here are the build's. A seat whose route needs
either document changed says so, and the sitting is convened again in full.

## Corrections after the critic's first pass (`docs/panel/225-reports/critic-briefs.md`)

Read that report before you start: its measured findings are framing facts you inherit, and they
changed this brief in five places.

- **"The refused unit" is the FIRST refused unit.** `cli/assemble.hero`'s `per_module` keeps every
  refused unit's failure in `refusals` and the first as the verdict, and only that first unit is
  asked again (`first`, `first_text`). A second refused unit is neither asked nor told, whether its
  errors stand in its accessors or elsewhere (the critic's S1 and S2, reproduced by the coordinator,
  now defect 2143, https://github.com/heroes-lang/heroes/issues/2143, `adjacent`). A route says
  whether it reaches 2143 too: the critic's **E**, every refused unit asked and told.
- **The family of 2136 is wider than the pointee check**: the refusals made before any unit
  compiles are `bound_macro.told`, `pragma_ask.told`, `probing.check_all` (the pointee and layout
  checks), `lock_told`, `two_ways.told` and `defined_twice.told`; the critic ran **L1**, a copied
  record holding a lock (`ffi_lock_copied`) beside `K_DIV`, told alone. The critic's **F**: the
  probe asked before the first refusal that comes before any unit.
- **A refusal `-w` does not lift**: **U1**, a union built from two members (`ffi_union_field`), is
  refused by a `_Static_assert` that stays an error under `-w`, so route A alone does not reach
  2137's class, and route B says what *inert* means for a record's assertion and a construction.
- **The gate is every golden case that exercises the probe**, the critic's 17 (§ 3.3 of its
  report), `run/fixedbugs-2072-values-c-defines-still-build` among them, which guards against a
  value C defines being refused.
- **Routes may combine**, A, B and C where each reaches what the others do not; and route C is
  attacked with panel 216's own failing shapes (`__OPTIMIZE__`, `__LINE__` and `__FILE__`,
  `__COUNTER__`, a `__builtin_constant_p` of a const function), and weighed against spec § 13's
  sentence on what clang checks against the header. The only form of route D that lets a tag be
  placed, a ruling that pins the cost, would write into design.md §4.17, which convenes the
  sitting in full.

Two framing facts held by another route, the conclusions unchanged: `m-readonly-lend` is a
lightweight tag, its `creatordate` its commit's date (git wrote it 44 s later); and the next tag is
judged by `records_github.standing`, `counts_because` reading only a tree that holds the old issue
files.
