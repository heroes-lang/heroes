# Panel 224, shared brief: a value C leaves undefined, told in one build on a target that defines it

**Convened 2026-10-17** by the author's answer that afternoon (*convene the two sittings now*), on
defect 2120 (https://github.com/heroes-lang/heroes/issues/2120), `adjacent` since 2026-10-17 and
counted `blocking`, with defect 2099, at M-bounded-seed's tag: the coordinator placed the tag
locally on `3c478500b` and ran `records` (`records/tagged`: *placed over 2 open defect(s)*), then
removed it, so no `m-bounded-seed` tag exists; the critic reproduced the reading in a scratch clone
(25 and 1 with the tag, 26 and 0 without). **Lane: soundness** (compiler-engineer, ffi-pragmatist, a
completeness critic): no surface, no spec token; the diagnostic class `ffi_constant_type` is
unchanged, and the question is WHEN it is told. The critic says if that reading is wrong.

**The tree**: lane `lane-land215`, HEAD `3c478500b`, clean but the briefs and reports of panels 223
and 224. **Read `docs/panel/224-reports/critic-briefs.md` after this brief**: its findings are
framing facts you inherit, and they changed this brief. **The Windows box is on** (the author,
2026-10-17); **its repository does not hold `3c478500b`**: push it first, `ssh win`; commits reach it by `git push win <sha>:refs/heads/<name>`
and `git --git-dir=/c/w/heroes.git archive --prefix=<dir>/ <sha> | tar -x` in `/c/w`, the
compiler built there with `clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o
heroes.exe`. Each seat works in its own `/c/w/224-<seat>` and never in another's.

## The question

`tests/golden/unsupported/fixedbugs-2072-a-value-c-leaves-undefined-is-told.hero` binds six header
values C leaves undefined. The program's own unit makes clang's warnings for them errors at each
accessor (the region, `selfhost/emit/value_check.hero`, 123 lines by `wc -l`; the warnings are
`selfhost/emit/undefined_words.hero`'s `UNDEFINED`, lines 13 to 21: `-Wshift-count-overflow`,
`-Wshift-count-negative`, `-Wshift-negative-value`, `-Wshift-sign-overflow`,
`-Winteger-overflow`, `-Wdivision-by-zero`). On the Windows box, clang 23.1.1 for
`x86_64-pc-windows-msvc` adds `-fwrapv` to every compile (with `-fms-compatibility`, on any target,
from the driver itself, the critic measured; the CI's Windows leg runs clang 20.1.8, which adds
none, and reads the case green) (`clang -### -c w.c` there prints
`"-fwrapv"`, run by the coordinator 2026-10-17; Apple clang 21 on this Mac with
`--target=x86_64-pc-windows-msvc` adds none, `grep -c -- -fwrapv` 0), the region is silent on
`-1 << 2` and `1 << 31`, and the unit's compile stops on the other four before panel 216's probe,
which asks with `-fno-wrapv` (panel 218's R3, ratified) and would refuse the two, runs. So the
two are told only once the other four are corrected (the issue's body at `bc978b09d`; the critic
reproduced it at `3c478500b` on the box, 0 and 1, and the case reads 1 and 0 on this Mac). **And it
is not Windows' alone**: on this Mac, with no `-fwrapv`, a region-refused value beside `INT_MIN /
-1` or a float out of an integer's range, the same pair split across two modules, and a name the
header lacks beside `INT_MIN / -1` each tell the first only (the critic's first pass). 2120 is the
`-fwrapv` case of that order.
**How should every undefined value a program binds be told in its first build on every target?**

## Routes the coordinator listed (not the option set)

- **A, `-fno-wrapv` in every compile's words** (`selfhost/cli/flags.hero`'s `flags()`): the region
  then warns as on the other targets. It **reverses panel 218's ratified ruling** (`-fno-wrapv` in
  the probe's words, never the program's), changes what C means for a header's own inline code on
  a target whose clang made signed overflow defined, and moves every object's and the runtime's key
  on every platform.
- **B, the probe asked even where the region refused**: the values the region did not refuse are
  asked by panel 216's probe in the same build (the probe compiled without the refused accessors,
  or the region's errors told and the probe run over the rest), so all six are told at once.
  The program's own C keeps its target's meaning; the probe keeps ISO's (218 R3). The probe tells a
  value in its own words, not the clang words the 2072 case's `.expected` pins, and it compiles
  with `-w`, which also silences the region's pragma-promoted errors: say which reading the case
  takes on Windows under B.
- **C, left open and said**: told on the second build. A skip by the machine's clang is what
  `tests/harness/platform_skips.hero`'s own rule excludes, and as worded it would skip a case green
  on the CI; and no rule lets a defect left open stop counting against a tag.
- **D, the region's words carry `-fno-wrapv` alone**: the critic tried nine per-function and
  per-region spellings on both clangs and none brings the two warnings back; beyond that vocabulary,
  a question.
- **E, a front-end pass**: `-fsyntax-only -fno-wrapv` over the units that hold a region, before or
  beside their compile; on the box's real unit the critic read all six told, in the `.expected`'s
  words. **E'**: the accessors compiled in a unit of their own with `-fno-wrapv`. **F**: the
  region's six kept as warnings and read back after the unit compiles, as record accessors already
  do (panel 221's route S). **G**: `-fno-ms-compatibility`. Each is the critic's, unbuilt but E.

## What is to be measured, not argued

1. The defect reproduced on the box with the landed compiler (the case through the harness:
   `./heroes.exe run tests/harness/main.hero -- ./heroes.exe unsupported fixedbugs-2072`).
2. For A and G: which of the box's Windows SDK and UCRT headers' inline functions hold signed
   arithmetic that `-fno-wrapv` lets clang optimise differently (the box has none of curl, sqlite,
   raylib or SDL3; the UCRT and the SDK are the set; a seat installs nothing on the box).
4. The Mac shapes above: which route tells each of them in one build.
3. For the route you would adopt (B or E or another): built, and the case read in one build on the
   box and on this Mac; lines in `selfhost/` (`value_run.hero` 280, `value_ask.hero` 133 by
   `wc -l`); the cold and warm build cost of a program binding no header value, which must not move,
   and of one that does.

## Rules for every seat

Your own copy **inside the repository's root**, `.claude/worktrees/224-<seat>/` under
`/Users/joseph/Temp/heroes/heroes-lang/` (`git -C <lane> archive 3c478500b | tar -x -C <dir>`;
CLAUDE.md's hard stop forbids a file outside the root), your own `/c/w/224-<seat>` on the box. Report to `docs/panel/224-reports/<seat>.md` in
the lane as you go. **No paid run**, **no credential printed** (`gh auth status` only). A program
that may not end runs as a built binary under `timeout`, output bounded. Every number carries its
command; a claim not run says *unrun*.

## The lane's condition (the critic's)

The soundness lane holds for a resolution that writes nothing into design.md §4.17 (which counts a
mistake told only after another is fixed as a cost) or into spec § 13. A seat whose route needs
either says so, and the sitting is convened again in full.
