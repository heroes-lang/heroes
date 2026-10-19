# Panel 225, compiler-engineer, third step: the union, built before the synthesis

**Written 2026-10-19 by the coordinator**, after the critic's second pass
(`docs/panel/225-reports/critic.md`, complete 04:59 +0200). `/panel` § 3c: *a seat with Bash
builds the adopted route in its own copy and runs the cases the sitting named, before the
synthesis is written; a route that does not build is not adopted.* The route the synthesis will
adopt, provisionally, is **the union** the critic names in its § 7, which no seat built whole:
yours (`heroes-g`) answers `twin` and `first` and FP's does not; FP's (`heroes-p`) answers `tdef`
and builds `forge1` and yours does not; neither survives `forge2c` / `forge2p` (critic § 2, § 3.1).

## What to build, in your own copy

`.claude/worktrees/225-compiler-engineer`, on top of the diff that is `heroes-g` (`git -C
.claude/worktrees/225-compiler-engineer status --short` lists its ten files, run by the
coordinator 2026-10-19). Never another seat's directory or the critic's, except to READ and
`cp -p` their programs into your `ce-work/`; never the lane.

1. **The cut is applied to a copy alone** (FP's `Unit.cut`): a unit that compiled is probed
   as today, its preprocessed text never cut. The critic's `forge1` (a correct program whose
   header holds your mark) is refused at exit 2 by `heroes-g` and builds under s2 and `heroes-p`.
2. **The copy keeps the opening and the asked accessors** (FP's extent): every other stretch
   of the unit, the typedefs, the string-literal table, the assertions, the functions and the
   refused accessors among them, stands inside a cut stretch. If an asked accessor needs
   anything else to compile, keep that and say what it is and which case showed it (FP's
   `heroes-p` built `run/fixedbugs-2109-...` beside a refusal with its two record constants,
   FP § 12). The critic's `tdef` is told `K_DIV` by `heroes-p` and not by `heroes-g`.
3. **A refused accessor is written as the program writes it, inside a cut stretch, and left
   out of `Unit.defined`** (yours; no inert twin, so `twin` answers); `constant_accessor.inert`
   stays retired (your § 9, which repairs the false message filed as defect 2145).
4. **E one unit at a time** (yours, `first`), and **F** where your step 3 put it: `check_all`,
   `lock_told`, `two_ways`/`defined_twice`. Not at `bound_macro` and `pragma_ask`, for your
   § 11's reason, unless you run a shape there that shows it safe.
5. **A mark no header can write, both forms the critic's § 7 names (G):**
   - **(a) one identifier carrying the copy's own key**, for instance `#pragma
     hero_probe_cut_<key>` and `#pragma hero_probe_keep_<key>`, never a pragma of two or more
     tokens: **on an MSVC target `clang -E` expands macros inside an unknown pragma**, measured by
     the coordinator 2026-10-19 on the Windows box (clang 23.1.1, `x86_64-pc-windows-msvc`) and on
     this Mac (`clang -E -P -fms-extensions`), with a file defining `cut 1`, `keep 2`,
     `heroes_probe 3` and `hero_probe_cut on_by_macro`: `#pragma heroes_probe cut` printed
     `#pragma 3 1`, `#pragma hero_probe_cut on` printed `#pragma on_by_macro on`, and
     `#pragma hero_probe_cut_0123abcd` printed itself; under `-fno-ms-extensions`, under
     `--target=x86_64-pc-linux-gnu` and on this Mac's default target all three printed
     themselves. A `_Pragma` of the keyed name inside an expression printed on a line of its own,
     the expression split around it (`int c = (` / `#pragma hero_probe_keep_0123abcd` / `3);`).
     The file is `/c/w/p225g/marks.c` on the box. The key is **deterministic**, so a warm build
     reads the copy's kept verdict; say what it is digested from and why no header can know it.
   - **(b) the count**: the cut knows how many marks the writer wrote and finds exactly that many
     lines carrying the key, or the copy is **silent** (no verdict, told by nobody, as a copy that
     does not compile today). A compiler test feeds the cut a text with one keyed line too many,
     and one too few.
6. **FP's de-duplication** (`whose.with_beside` drops a probe finding at a span the refusal
   already tells), with a compiler test that exercises it: the critic's § 6 found nothing run that
   does.

## What to run, each with its command in your report

- **Every program of the critic's §§ 2 to 5** (`.claude/worktrees/225-critic/critic-work/pass2/`:
  `tdef`, `twin`, `first`, `mixed`, `forge1`, `forge2c` and `forge2p` with their `-next`,
  `forge3c`, `forge3p`, `optoff` at `-O0` and `-O2` with `opt-next`, `fword` (`fshift`,
  `fshift-next`, `fi8`), `forgeF` (`ff`, `ff-next`), `line` (`asw`, `deleted`, `renamed`)), your
  own (`d2137`, `d2136`, S1, S2, U1, L1, `ctr`, `inert` and their corrected forms) and FP's
  `.claude/worktrees/225-ffi-pragmatist/ffi-work/cnt/` (`cnt`, `next`, `next32`), each built in a
  fresh directory by s2, `heroes-g` and the union, in one table as the critic's § 2. What the
  union owes: wherever either prototype tells a value, it tells it; no `forge*` program tells
  `K_SZ`; every program s2 builds, it builds.
- **The gate the sitting named**: the critic's 17 probe cases; the whole `unsupported` and `full`
  forms; `./<union> test selfhost/main.hero`; `layout`. Your § 8 measured `rounds.hero` 293,
  `decls.hero` 300 and `writer.hero` 298 by `layout`'s count with `heroes-g`: a change that takes
  one past 300 owes its split in this route, not later.
- **Nothing a build compiles moves**: `--emit-c` and the units of your `ce-work/same/` programs,
  by `cmp`, against s2; no mark in any `build/tu-*/*.c`.
- **The cost**: clang runs (`ce-work/countpath/clang`) for `plain`, the 2072 case, `d2137`,
  `d2136` and U1, cold and warm, beside your § 8's table.
- **For the box**: name the programs the coordinator should run there under the union (the
  marks are clang's, and Windows is where they are rewritten).

## Rules, as before

Report to `docs/panel/225-reports/compiler-engineer-union.md` in the lane
(`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-witnessed-seed`), **as you go**, so
a stopped run leaves what it had. Your compiler is built in your copy from your own stage 2. **No
paid run; no credential printed** (`gh auth status` only). A program that may not end runs as a
built binary under `timeout`, its output bounded; never `timeout heroes run`. A command longer
than nine minutes runs in the background, split. Every number carries its command; a claim not run
says *unrun*. Commit nothing; leave the diff uncommitted, and save it as `ce-work/union.diff`
(`git diff` plus the new files) for the coordinator, who lands it in the lane after the synthesis.
