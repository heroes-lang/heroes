# Panel 225, completeness critic, first pass: the briefs' framing facts, run

Written 2026-10-18 as the facts were run, in my own copy
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/225-critic` (made with `git -C
/Users/joseph/Temp/heroes/heroes-lang worktree add --detach .claude/worktrees/225-critic
7b97b9438`), with my own stage 2. Every number carries the command that produced it; a fact I
could not run says so. No verdict is given here. The reproducers were copied into
`.claude/worktrees/225-critic/critic-work/repro/` (`cp -p` from
`.claude/worktrees/scratch-clean-list/repro/`); every build below ran from my copy, and every
program I wrote is under `.claude/worktrees/225-critic/critic-work/` (`shapes/`, `pre/`).
**First pass complete, 2026-10-18 23:40 +0200** (`date`); the second pass, after the seats, goes
to `critic.md` beside this file.

## Summary for the seats

- **Every framing fact of the three briefs holds as a reading of the world**, with the
  exceptions of § 3: the tag's instant is its commit's (a lightweight tag), `counts_because` is
  not the function the next tag is judged by (`records_github.standing` is, with the same
  conclusion), and the brief's gate names 5 of the 17 golden cases that exercise the probe.
- **The class is wider than the two reproducers, measured (§ 5)**: a SECOND refused unit is
  never asked again and its own refusal is not told either (S1, S2); a pre-unit refusal other
  than the pointee check withholds the value too (L1, `ffi_lock_copied`); and a refusal of the
  value's own unit can be a `_Static_assert` that `-w` does not lift (U1, `ffi_union_field`).
- **The lane reading is not wrong on the evidence (§ 6)**, with two cautions.
- **Routes and questions the briefs do not list: § 7.**

## 1. My compiler

- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-s1`, `real 7.61`.
- Stage 2: `./heroes-s1 build selfhost/main.hero -o heroes`, `real 76.75 user 59.74 sys 11.55`
  (`/usr/bin/time -p`; other sessions may have been running, so the duration is not a
  measurement anyone should reuse).
- `./heroes --version` prints `heroes 0.4.0` / `sources 08c492ac05cbb532`: **the brief's
  `sources 08c492ac05cbb532` holds.**
- `clang --version`: `Apple clang version 21.0.0 (clang-2100.3.34.2)`, target
  `arm64-apple-darwin27.0.0`.

## 2. Facts that hold, each with its command

| Brief's fact | Command | Reading |
|---|---|---|
| Lane HEAD and main at `7b97b9438`, pushed | `git rev-parse --short=9 --verify` of `main`, `lane-witnessed-seed`, `origin/main`; `git ls-remote origin refs/heads/main` | all four `7b97b9438` |
| `m-witnessed-seed` not yet placed | `git tag --list m-witnessed-seed \| wc -l`; `git ls-remote --tags origin 'refs/tags/m-*'` | 0 locally; 56 `m-*` tags on each side, the same set (`comm` printed nothing either way) |
| `m-readonly-lend` at 05:07:29 UTC | `git tag --list m-readonly-lend --format='%(creatordate:iso-strict)'` | `2026-10-18T07:07:29+02:00` = 05:07:29Z (see § 3.1 on what this command reads) |
| 2136 and 2137 opened 05:01 UTC 2026-10-18 | `gh issue view <n> --json createdAt` | 2136 `2026-10-18T05:01:28Z`, 2137 `2026-10-18T05:01:37Z` |
| 2137 `adjacent` since 2026-10-18 | `gh issue view 2137 --json labels,body` | label `class:adjacent`; body `**adjacent**, 2026-10-18` (2136 also `class:adjacent`) |
| the next tag would be their second, so they count `blocking` | `gh issue list --state open --label kind:defect` and `--label kind:decision`; the code of § 3.2 | open defects: 2136, 2137 (`adjacent`, open at `m-readonly-lend`), 2139 (`adjacent`, `2026-10-18T16:50:47Z`, after that tag, so one tag at the next), 2142 (`blocking`); no open decision; no open issue without a `kind:` label. So the next tag is refused over 2136, 2137 and 2142 while they stand open |
| `AGED_AT` 2 | `sed -n 55,70p tests/harness/records_github.hero` | `constant AGED_AT: i64` = `2` (line 64) |
| defect 2142 is `selfhost/cli/link.hero` | `gh issue view 2142 --json body` | its *Where to look* opens with `selfhost/cli/link.hero` |
| Panel 216's R1 ratified 2026-10-17 | its file, lines 118-132 and 215 | R1 as described; *RATIFIED, 2026-10-17* |
| Defect 2128's repair is M-readonly-lend step 7, `value_beside.hero` | `git log --format='%h %cI %s' -- selfhost/cli/value_beside.hero` | `062f8c262 2026-10-18T05:01:41+02:00 M-readonly-lend step 7: defect 2128, ...`; its body names 2136's and 2137's shapes as filed apart |
| `value_beside.hero` 248, `value_ask.hero` 151, `value_run.hero` 280, `assemble.hero` 287, `rounds.hero` 291 lines | `wc -l` | 248, 151, 280, 287, 291 |
| `refused` lines 61 to 88, `again` 90 to 122 | Read of `selfhost/cli/value_beside.hero` | comment opens 61, `return held` 88; comment opens 90, `return ok(...)` 122 |
| the module's comment, lines 18 and 19 | same | *An error anywhere else, or with no place, and the copy / would not compile either, so the refused unit is not asked again.* |
| `value_beside.hero` lines 21 to 24 | same | *Where the probe cannot answer, its own failure is not told beside an author's refusal: the next build, past the refusal, asks it again and says why.* |
| `emit/constant_accessor.inert` | `grep -n 'function inert' selfhost/emit/constant_accessor.hero` | line 91; the signature at the constant's line and `return (<type>){0};` |
| `value_run.compiled`, lines 122 to 133, its words | Read of `selfhost/cli/value_run.hero` | comment 122, function 125, words 129-132: `clang -x cpp-output` + the unit's words + `checks()` + level, `-g0`, `-ffunction-sections`, `-fdata-sections`, `-w`, `-c`; `response_file.carried` at 133 |
| `assemble.hero` is where the probe is asked once the units compile | Read | `value_ask.judged` at lines 195-206 |
| 2136's refusal at `probing.check_all`'s verdict (line 179), before any unit | Read of `selfhost/cli/rounds.hero`; `d2136.hero` built in a fresh directory, then `ls build` | line 179 is the call, its verdict returned at 181-187; `beside` reset to empty at 157. The build left `meaning`, `pointee-<key>`, `pragma-<key>`, `reads`, 3 `runtime-<key>`, `words` and **no `tu-<key>`**: no unit was compiled |
| the reproducers and the three headers as described | `cat` of each; `diff` of both 2128 headers against `tests/golden/unsupported/` | as described; both 2128 headers byte-identical to the golden copies. `d2136-next.hero`'s "it" is `fill`'s `@out`, declared `i64` |
| `d2137.hero`: `error[ffi_parameter_type]` for `narrow` alone, exit 1 | `../../heroes build d2137.hero -o d2137` | one diagnostic, `narrow` at 2:5, exit 1; stderr byte-identical to the coordinator's kept `d2137.emit.err` (`diff`) |
| `d2137-next.hero`: `error[ffi_constant_type]` for `K_DIV`, exit 1 | `../../heroes build d2137-next.hero -o d2137-next` | one diagnostic, `K_DIV` at 5:5, exit 1 |
| `d2136.hero`: `error[ffi_parameter_type]` for `fill` alone, exit 1 | `../../heroes build d2136.hero -o d2136` | one diagnostic, `out` of `fill` at 2:20, exit 1 |
| `d2136-next.hero`: `K_DIV`, exit 1 | `../../heroes build d2136-next.hero -o d2136-next` | `error[ffi_constant_type]` for `K_DIV` alone, exit 1 |
| both refusals are the build's (the lane's condition) | `../../heroes check d2137.hero`, `../../heroes check d2136.hero` | exit 0 each, 0 bytes on stderr |
| the same pair across two modules told in one build; the 2128 cases and the 2072 case green today | `./heroes run tests/harness/main.hero -- ./heroes unsupported fixedbugs-2128`, then `... fixedbugs-2072` | `4 passed, 0 failed`, exit 0; `1 passed, 0 failed`, exit 0 |
| §4.17's two phrases (panel 187, ratified 2026-10-03) | `awk` of design.md from line 2265 to §4.18 | both italic phrases verbatim; *(panel 187, ratified 2026-10-03)* holds |
| Panel 216 chose the program's own unit so that a header reads as the program reads it | panel 216's file, lines 39-55 | holds in substance (the brief gives no line): F and F2, a probe text of their own, *judge another header than the build compiles* (`feat.h` on `__OPTIMIZE__`) and at `-O0` miss `__LINE__`, `sizeof(__FILE__)`, a `__builtin_constant_p` of a const function and falsely refuse `__COUNTER__`; Ux-whole, the program's own unit preprocessed under its own words, caught `FEAT_UB` at both levels |

**Where 2137's refusal stands, run**: the refused unit is kept at
`build/tu-1dd245ea39c6e8a8/d2137.c` (206 lines by `wc -l`). Compiled again by hand under the
build's own response file (`clang @build/words/words-571f3c485f4bd012.rsp -c
build/tu-1dd245ea39c6e8a8/d2137.c -o /dev/null`), clang prints **two** errors, both
`[-Werror,-Wshorten-64-to-32]`: `d2137.hero:2:96`, the narrowing probe
`hero_ffi_probe_h_d2137_narrow` (unit line 105, inside `#pragma clang diagnostic error
"-Wimplicit-int-conversion"`, lines 99 to 107), and `d2137.hero:8:34`, the call `t2 = narrow(t1)`
in `main`'s body. `K_DIV`'s accessor stands at unit lines 123 to 139 (`#line 5 "d2137.hero"`).
So *a line no constant's accessor holds* holds, for two lines, one of them in a function body the
program wrote (defect 2137's own `Origin` says the same). The bracket answers half of route A's
question by measurement, which word makes it an error: `-Werror=shorten-64-to-32`, item 11 of the
response file. **Whether `-w` then lets this unit compile I did not run**: it is the
ffi-pragmatist's assigned run. The compiler's own comment says the same by reading
(`selfhost/emit/ffi_narrowed.hero` lines 33-41).

**The verbs**: `heroes build --emit-c`, `heroes run` and `heroes test` on `d2137.hero` and
`d2137-next.hero` each read as `build` does (`ffi_parameter_type` alone at exit 1, then
`ffi_constant_type` at exit 1), since all four go through `cli/rounds.hero`. The briefs measure
`build` alone; a route's gate can name the other three or say why they share the path.

## 3. Facts that do not hold as written, or hold by another route than the cited one

### 3.1 `m-readonly-lend` is a lightweight tag: its `creatordate` is its commit's date

`git cat-file -t m-readonly-lend` prints `commit` (so do `m-bounded-seed` and
`m-buildable-structs`; `m-typed-inspection` and `m-issue-files` print `tag`). For a lightweight
tag `%(creatordate)` is the tagged commit's committer date: `git log -1 --format=%cI
m-readonly-lend` prints `2026-10-18T07:07:29+02:00`, the same instant. The ref file
`.git/refs/tags/m-readonly-lend` was written `2026-10-18T07:08:13+0200` (`stat -f '%Sm'`), 44 s
later. So *placed at 05:07:29 UTC* is the commit's time, not the placing; the conclusion stands,
both instants being after both filings. **What follows, read and not run**:
`records_github.tags_of` reads the same `%(creatordate:unix)` (`tests/harness/suite_records.hero`
line 1212) and `open_at` compares an issue's `createdAt` with it, so a defect filed after a
commit and before a lightweight tag is placed on it is read as filed after the tag. Nothing in
this sitting turns on it; the coordinator may want it filed.

### 3.2 The next tag is judged by `records_github.standing`, not by `counts_because`

`records/tagged` (`tests/harness/suite_records.hero`, `tagged`, lines 1069-1119) takes the tree
route, `counted_at` and `counts_because`, only where the newest tag's tree holds `issues`,
`docs/work/defects` or `docs/work/DEFECTS.md`, and the GitHub route otherwise. `git ls-tree
--name-only <t> -- issues docs/work/defects docs/work/DEFECTS.md` prints nothing for
`m-readonly-lend` nor for `7b97b9438`. So a tag placed now is judged by `github_route`, then
`records_github.standing` (`tests/harness/records_github.hero` line 662): an
`adjacent` defect counts once it stood open at `AGED_AT` tags, the judged included, by instant.
**The conclusion holds**: 2136 and 2137 stood open at `m-readonly-lend` and would at the next
tag, two tags against `AGED_AT` 2. The citation of `counts_because` names the route a tag placed before 2026-10-12 took.

### 3.3 The gate names 5 of the 17 golden cases that exercise the probe

The brief's measure 2 names `unsupported fixedbugs-2128` (4) and `fixedbugs-2072` (1, under
`unsupported`). `ls tests/golden/*/ | grep -E '^fixedbugs-(2072|2075|2101|2104|2109|2120|2128)-.*\.hero$'`
lists 17: 13 under `unsupported` and 4 under `run`, among them
`run/fixedbugs-2072-values-c-defines-still-build`, the false-refusal guard of 2072 itself, and
`unsupported/fixedbugs-2075-a-probe-that-does-not-finish-is-told-as-such` and `...-does-not-link-...`,
which a route compiling another text (A, C) can move. All 17 are green on my stage 2:
`unsupported` 2072 1/0, 2075 5/0, 2101 1/0, 2104 1/0, 2109 1/0, 2128 4/0; `run` 2072, 2075,
2101, 2109 1/0 each (`./heroes run tests/harness/main.hero -- ./heroes <form> <filter>`, each
exit 0, log `critic-work/golden-probe-baseline.log`). `unsupported fixedbugs-2120` selects
nothing (`0 passed, 0 failed`, exit 2): no case carries 2120's number. The cases whose
`.expected` carries the probe's own words (`find tests/golden -name '*.expected' | xargs grep -l
"traps where it reads it\|did not finish\|header constant's value\|the probe of"`) are 9, all under
`unsupported`, all among the 17.

### 3.4 Smaller

- *the refused unit again* (00-shared, the question): it is the FIRST refused unit
  (`assemble.hero` lines 113-156 keep `first` and `first_text` once); § 5.1 measures what that
  leaves.
- §4.17 *measured rather than promised*: design.md says *counted rather than promised*; the
  italic phrases themselves are verbatim.
- *a header included before it in the same group* (ffi-pragmatist): a group names one header
  (spec § 13, `Extern = "extern" string [ ( "link" | "package" ) string ] NEWLINE`, line 462), so
  the phrase can only mean a header the group's header includes itself; the seat may want to read
  it so.

## 4. What I could not verify

- *Convened 2026-10-18 by the author's answer that evening, meant as yes, now*: a conversation;
  there is no open decision on GitHub (`gh issue list --state open --label kind:decision` printed
  nothing) and nothing in the tree records it.
- *While the sitting runs the coordinator repairs defect 2142 in that lane*: ongoing, not
  checkable; the lane's `git status` showed only `?? docs/panel/225-briefs/` at the start.
- *Reproduced by the coordinator ... with the lane's stage 2*: the coordinator's run itself is not
  re-runnable; my stage 2 has the same `sources` key and reproduces it, and the kept stderr is
  byte-identical to mine.

## 5. Shapes beside the reproducers that the briefs do not name, measured

Programs under `critic-work/shapes/` and `critic-work/pre/`; each built with `../../heroes build
<p>.hero -o <p>` (or `../../../heroes` one level deeper), formatted by `heroes fmt` first.

### 5.1 A second refused unit is not asked, and its own refusal is not told

- **S1**: the root binds `narrow(x: i64)` (refused) and `use s1mod/vals`; `s1mod/vals.hero`
  binds `K_NONE` (a name the header lacks) and `K_DIV`. Build: **`ffi_parameter_type` for
  `narrow` alone, exit 1** (stderr 7 lines). Neither `K_NONE`, the second unit's OWN refusal, nor
  `K_DIV` is told. With `narrow` corrected (`s1-next.hero`), the next build tells `K_NONE`
  (`ffi_unknown_name`) and `K_DIV` (`ffi_constant_type`).
- **S2**, the modules swapped (the root binds `K_NONE` and `K_DIV`, `s2mod/narrowed.hero` binds
  `narrow`): `ffi_unknown_name` for `K_NONE` and `ffi_constant_type` for `K_DIV`, exit 1, and
  **not `narrow`**.

So beside a refusal only the first refused unit is asked again (`per_module`'s `first`), a
second refused unit is neither asked nor told, and this holds whether its errors stand in its
accessors (S1, the shape 2128's repair reaches when the unit is first) or elsewhere. Every route
A to C speaks of *the refused unit*, singular. The second unit's own refusal waiting a build is a
shape of its own, beside 2137 rather than inside it; panel 093's R5 says every dirty unit compiles
before any verdict and *the declaring TU's clang failure maps to exit 1*, and says nothing of
telling every refused unit. I searched the issues for it (`gh issue list --state all --search`
with `"second unit"`, `refused unit first verdict`, `"two modules" refusal "next build"`, `every
unit refused told`, `"first failure"`, `units refused one build`, `"only the first"`) and found
none that names it: a question rather than a premise, and the coordinator's to file if real.

### 5.2 2136's family is wider than the pointee check

Read from `selfhost/cli/rounds.hero` and `selfhost/cli/probing.hero`, the refusals made before
any unit compiles: `bound_macro.told` (`run`, lines 71-80); `pragma_ask.told` (`round`, 170-178);
`probing.check_all`, the pointee check (probing.hero 189-201) and the layout check (203-219);
`lock_told` (192-197); `two_ways.told` and `defined_twice.told` (204-213). **Measured, one more
than the brief's**: **L1** (`pre/l1/l1.hero`, `record Box tag box partial` over
`ffi-lock-copied.h` copied by an array literal, beside `K_DIV`): `ffi_lock_copied` alone, exit 1,
no `tu-` directory, `K_DIV` not told. **Read, not run**: those of the five that fail with code
`diagnostics` end `rounds.run` at lines 104-105, before `value_beside.told` (line 125) is
reached, so a route that fills `beside` for them must also pass that return; `check_all`'s own
verdict reaches line 125 with `beside` empty.

### 5.3 A refusal of the value's own unit that `-w` does not lift

**U1** (`pre/u1/u1.hero`, `record UT` over `ffi-a-construction.h`'s union, built
`UT(i: 1, f: 1.5)`, beside `K_DIV`): `ffi_union_field` alone, exit 1; the unit `u1.c` was written
and has no object, so clang refused the unit itself: 2137's shape, at a line no accessor holds.
`clang @build/words/words-571f3c485f4bd012.rsp -c build/tu-5626e2d64be14bfb/u1.c -o /dev/null`:
`u1.hero:2:16: error: static assertion failed due to requirement '__builtin_classify_type(*(UT
*)0) != 13': heroes-ffi-union UT i f`, and **the same error with `-w` added after the words**,
exit 1. So route A alone cannot reach every member of 2137's class, and route B's *the binding's
code and its calls* has to say what inert means for a record's own assertion at its declaration
line and a construction in `main`.

## 6. The lane reading

**Not wrong on the evidence**, read as the brief reads it:

- The skill's soundness lane is for *no surface, no diagnostic and no spec token*
  (`.claude/skills/panel/SKILL.md` lines 18-23) and the full panel's list names *a diagnostic
  class* (line 26). The brief reads *no diagnostic* as *no new diagnostic class*, which is the
  reading panel 216 recorded for the sitting whose R1 this amends (its file, lines 5-8: *No route
  adopted adds surface, a diagnostic class or a spec token*); and 2128, the same question of WHEN
  one step earlier, was repaired with no sitting (`062f8c262`).
- No document the lane condition guards holds the mechanism: `grep -c -i undefined
  spec/heroes-spec.md` prints 0; `grep -c 'panel 216\|panel 218\|panel 224' docs/design.md`
  prints 0 and `grep -i 'leaves undefined' docs/design.md` prints nothing; §4.17's counted list
  names the parse stage only. In every shape above the value is told as `ffi_constant_type`.

Two cautions, neither run against a document change because none is proposed yet:

- The skill adds *a question that looks internal but has a sentence in the spec behind it is a
  full panel*. The nearest sentence is spec § 13's *clang checks every result type, constant and
  record field against that header*. Route C judges a constant by a unit the program never
  compiles; whether that is still *that header* as the program reads it is the ffi-pragmatist's
  soundness question, and it stays in the lane only while no route needs that sentence changed.
- Route D lets no tag be placed (§ 2), and the only form of D that would (a ruling that a value
  told one build late beside the author's own refusal is a counted cost, pinned) writes into
  §4.17, which by the brief's own condition convenes the full panel. Likewise a route whose
  resolution PROMISES *every value C leaves undefined is told in the build that refuses anything
  else* is a §4.17 sentence.

## 7. Routes and questions the briefs do not list

Not the option set either; each is something a seat could be asked.

- **E, every refused unit, not only the first** (§ 5.1): whichever of A to C is adopted, applied
  to each refused unit, with the extra clang runs that costs counted per refused unit; and whether
  the second unit's own refusal, told a build late today, is in or out of the resolution.
- **F, the probe asked before the first pre-unit refusal**: the value probe (of whatever text the
  route compiles) asked at the probing stage, before `bound_macro`, `pragma_ask`, `check_all`,
  `lock_told` and `two_ways`, which would reach 2136's whole family (§ 5.2) rather than its
  pointee member; route C's unit, needing none of the module's functions to compile, is the one
  that could run there. Its cost: a build refused before any unit compiles pays the probe, cold.
- **A, B and C combined** rather than exclusive: A where every error clang reports is a warning
  made an error by a word or a pragma (d2137's two), and B or C where one is not (U1's static
  assertion). The seats can be asked whether the complete route is a combination and which part
  decides first.
- **For route C, the attack list the brief hands the ffi-pragmatist omits the shapes that sank
  F and F2 in panel 216** (its lines 39-49): a header branching on `__OPTIMIZE__`, `__LINE__` and
  `sizeof(__FILE__)` read at the accessor's `#line`, `__COUNTER__`, and a `__builtin_constant_p`
  of a const function. A unit of the constants alone is a probe text of its own, the kind panel
  216 measured wrong. For `__COUNTER__`, a question and unrun: it counts every expansion in the
  unit, and the unit expands a header's name outside the accessors too (the extern probe calls
  `(narrow)(a0)` at unit line 105 and `hero_extern_refs` takes `&narrow` at line 111 of `d2137.c`),
  so whether route C keeps those lines decides whether a `__COUNTER__` behind an object-like macro
  of a bound function reads the same.
- **The cost question for E and F**: the brief's measure 3 asks *how many* extra compiles a
  refused build runs; with E it is per refused unit, and with F it moves onto builds refused
  before any unit, which today pay no probe at all (`d2136`'s build directory holds no `value-` or
  `init-` entry, `ls build`).

## 8. Two slips of mine, said

- One command redirected `git ls-remote` into `/tmp/x`, outside the repository's root, against
  the hard stop. I read it back (written `2026-10-18T23:28:32+0200`, 56 lines, the remote tag
  names, seven seconds before I looked) and removed it (`rm /tmp/x`; `ls` then says no such file).
  Every later listing was held in memory.
- `heroes run` on `d2137.hero` and `d2137-next.hero` ran as `timeout 60 heroes run`; both builds
  were refused, so no program ran, and `pgrep -fl critic-work` afterwards found nothing running.
