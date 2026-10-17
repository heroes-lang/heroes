# Panel 224, completeness critic, first pass: the briefs' framing facts, run

2026-10-17, before any seat started. Lane `lane-land215`, HEAD `3c478500b`. Read: `00-shared.md`,
`compiler-engineer.md`, `ffi-pragmatist.md`, `critic.md` in `docs/panel/224-briefs/`. Everything
below was run in my own copies: `$S` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad/224-critic`
(`git -C <lane> archive 3c478500b | tar -x -C $S/tree`), and `/c/w/224-critic` on the box (the
same archive, carried by `ssh` as a `.tar.gz`, since the box's `heroes.git` does not hold
`3c478500b`, below). No paid run, no credential printed (`gh auth status` only), no route judged.
Nothing was written in the lane but this file.

The compilers used: on this Mac, stage 1 from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, exit 0) and stage 2 (`./heroes build selfhost/main.hero -o
heroes-next`, exit 0), both printing `sources 5f45d6514b22d650`; on the box, the seed's line with
`-Wl,/STACK:67108864` (exit 0, the seed's `md5sum` `39cead75dc984a74429b47424e2af623`, the same as
this Mac's `md5 -r`) and stage 2 from it (exit 0, `sources 5f45d6514b22d650`).

## 1. Facts that hold, with the command that settled each

| the brief's fact | command | result |
|---|---|---|
| issue 2120 is `adjacent`, filed 2026-10-17 | `gh issue view 2120 --json labels,createdAt,state` | `class:adjacent`, created `2026-10-17T10:16:24Z`, open |
| the case binds six values | `cat -n tests/golden/unsupported/fixedbugs-2072-a-value-c-leaves-undefined-is-told.hero` | six `constant`s, lines 14 to 19, each `#~ ffi_constant_type` |
| `value_check.hero` 123 lines | `wc -l selfhost/emit/value_check.hero` | 123 |
| `UNDEFINED`, lines 13 to 21, the six warnings as listed | `cat -n selfhost/emit/undefined_words.hero` | `constant UNDEFINED` at 13, `]` at 21, the six in the brief's order |
| `value_run.hero` 280, `value_ask.hero` 133 | `wc -l selfhost/cli/value_run.hero selfhost/cli/value_ask.hero` | 280, 133 (both under `selfhost/cli/`) |
| the probe is asked once the units compile | `sed -n 139,260p selfhost/cli/assemble.hero` | `if stopped.is_err()` returns (line 212) before `value_ask.told` (line 223); `asked` holds only units whose compile succeeded |
| the probe asks with `-fno-wrapv` | `grep -n wrapv selfhost/cli/value_run.hero` | `checks()` line 110, asserted by its test at 273 |
| panel 218's R3, ratified | `tail -25 docs/panel/218-*.md`; `git show --stat b2c81211d`; `gh issue view 2115` | **RATIFIED, 2026-10-17** under the verdict; issue 2115 closed `Settled by b2c81211d...` |
| box clang 23.1.1 adds `-fwrapv` | `ssh win 'clang --version; clang -### -c w.c 2>&1 \| grep -c -- -fwrapv'` | `clang version 23.1.1`, target `x86_64-pc-windows-msvc`, 1 |
| Apple clang 21 adds none for that target | `clang --version; clang --target=x86_64-pc-windows-msvc -### -c w.c 2>&1 \| grep -c -- -fwrapv` | `Apple clang version 21.0.0 (clang-2100.3.34.2)`, 0 (also 0 natively and for `x86_64-w64-windows-gnu`) |
| the region is silent on the two under `-fwrapv` | a mimic of the region over the case's header (`$S/wrapv/region.c`), `clang -std=gnu11 [-fwrapv] -fsyntax-only` | Mac: 6 errors, 4 with `-fwrapv` (the two missing are `-Wshift-negative-value`, `-Wshift-sign-overflow`); box: 4 by default, 6 with `-fno-wrapv`; `-fwrapv -fno-wrapv` gives 6 (the last word wins) |
| ... and on the real unit | the case's unit from the box's `build/tu-5d76de4b36791a97/`, `clang ... -fsyntax-only [-fno-wrapv]` with a subset of `flags()` (the exact words unread) | 4 region errors by default, 6 with `-fno-wrapv` |
| the defect, at the brief's HEAD on the box (measurement 1) | `./heroes-next.exe run tests/harness/main.hero -- ./heroes-next.exe unsupported fixedbugs-2072` in `/c/w/224-critic/tree` | `0 passed, 1 failed`; the actual output lacks exactly `SHIFT_NEGATIVE` and `SHIFT_INTO_SIGN`. On this Mac, the same command with `./heroes-next`: `1 passed, 0 failed` |
| the probe would refuse the two | `/c/w/224-critic/two/main.hero`, binding only `SHIFT_NEGATIVE` and `SHIFT_INTO_SIGN`, built on the box | exit 1, both `ffi_constant_type` (but in the probe's words, § 3) |
| harness command form | `sed -n 120,200p tests/harness/main.hero` | `<compiler> <suite> <filter>`, `unsupported` a form taking a filter |
| box logistics | `git remote -v`; `seed/README.md:24`; `ssh win 'git --git-dir=/c/w/heroes.git cat-file -t bc978b09d'` | `win` is `win:/c/w/heroes.git`; README line 24 is the brief's build line (`-o heroes`, not `heroes.exe`); `bc978b09d` is there |
| `flags.hero` in the `run` and `cache` rows | `.claude/rules/verification.md`, the map | "**`run`** whole and **`cache` alone** (every unit's and the runtime's key moves...)", plus `lldb` for `debug_words` |
| the CI's four legs | `sed -n 288,302p .github/workflows/ci.yml` | Linux x86-64 `ubuntu-latest`, Linux arm64 `ubuntu-24.04-arm`, Darwin arm64 `macos-26`, Windows x86-64 `windows-latest` |

## 2. Facts that do not hold, or hold only under a condition the brief does not state

1. **`records/tagged` counts 2120 `blocking` only over a tag that does not exist.** `git tag
   --list | grep -i bounded` prints nothing; the newest `m-*` tag is `m-buildable-structs`
   (`6f83c0a37`, 2026-10-17 14:34 +0200, lightweight). The check judges the newest tag
   (`suite_records.hero:1094`), and an `adjacent` defect counts from `AGED_AT` = 2 tags
   (`records_github.hero:64`). Run in a scratch clone (`git clone --shared --no-checkout`, HEAD
   `3c478500b`): **with a local tag** `git tag m-bounded-seed 3c478500b`, `./heroes run
   tests/harness/main.hero -- ./heroes records` read `25 passed, 1 failed`, `records/tagged`
   naming **two** defects, 2120 **and 2099** (*the padding bits inside a byte a named bit-field
   holds*), each "open at 2 tags (m-bounded-seed m-buildable-structs)"; **without it**, `26 passed,
   0 failed`. So the brief's sentence holds for a tag placed at `3c478500b`, and should say where
   that tag was placed and that 2099 blocks the same tag, so a reader does not expect 224's
   resolution alone to free it. The local tag was deleted (`git tag -d`), in the clone only.
2. **"MSVC (which never optimises on signed overflow)"**, the ffi-pragmatist brief's
   parenthesis, asserted without a command, **does not hold.** The box has MSVC's `cl.exe`
   (`.../BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe`). `cl /nologo /c /O2 /FA` of
   `int mul(int x) { return (x * 2) / 2; }` emits `mov eax, ecx; ret`: it folds to `x`, which holds
   only if `x * 2` cannot overflow. The box's clang at `-O2` keeps `leal (%rcx,%rcx); sarl` under
   its default `-fwrapv` and folds to `movl %ecx, %eax` under `-fno-wrapv`, as MSVC does. (MSVC kept
   `x + 1 > x` and a loop bound: it exploits some shapes, not all.) The seat's question should be
   put without the premise.
3. **The trigger of clang 23's `-fwrapv` is MS compatibility, not the target.** On the box:
   `clang --no-default-config -### -c w.c` still prints `"-fwrapv"`, and `ls "/c/Program
   Files/LLVM/bin/"*.cfg` finds no config file, so it is the driver's own default; `clang
   --target=x86_64-linux-gnu -fms-compatibility -### -c w.c` prints `"-fwrapv"`; `clang
   -fno-ms-compatibility -### -c w.c` prints none, and the region mimic under
   `-fno-ms-compatibility` tells all six. `-fms-compatibility-version=19.29` and `19.33` still add
   it. The brief's "for `x86_64-pc-windows-msvc` adds `-fwrapv` to every compile" is true of the
   box's default words; the mechanism belongs in the brief because it bears on which words a route
   can rely on. `grep -rn -E 'fms-|ms-compat|ms-extensions' selfhost/ runtime/` finds none of the
   compiler's own words touching it.
4. **The defect's evidence was at `bc978b09d`, and the box does not hold `3c478500b`.** `ssh win
   'git --git-dir=/c/w/heroes.git cat-file -t 3c478500b'`: `fatal: Not a valid object name`.
   `git log --oneline bc978b09d..HEAD | wc -l`: 27 commits, and `git diff --stat` over the region,
   the probe, `assemble.hero` and `platform_skips.hero` moves 7 files (468 insertions, 312
   deletions), a refactor in the main (`UNDEFINED` moved to `undefined_words.hero`, `value_ask`
   split into `value_run`). I reproduced it at HEAD (§ 1), so the claim now holds at `3c478500b`;
   a seat still has to push the commit before it can work from `heroes.git`.
5. **The CI's Windows clang is readable, and it is 20.1.8.** `gh run view --job 117400428380
   --log` (run 39034080946, `6f83c0a37`, 2026-10-17): `clang version 20.1.8` at log line 360, and
   `unsupported: 330 passed, 1 failed`, the failure being `fixedbugs-2109`, so 2072 read green
   there. The issue's "version unread" can now be filled in. So 2120 shows on the box alone today, and
   **would turn the CI's Windows leg red the day `windows-latest` carries clang 23 or later**
   (inference, from § 2.3 and the box). It bears on route C. `platform_skips.hero` skips by
   platform (`shell.exe_suffix() == ".exe"`, line 30), and its header says a skip there
   "holds of every machine of the platform ... never about the machine". A skip conditioned on
   whether a machine's clang adds `-fwrapv` is a new mechanism, of `absence.hero`'s kind, not a row
   of that file. As worded today, it would also skip a case that is green on the CI.
6. **Item 2's header set cannot be run on the box as written.** The box holds none of
   `sqlite3.h`, `curl/curl.h`, `raylib.h`, `SDL3/SDL.h` (`find /c -maxdepth 7`, outside my
   directory and the clones of the tree: nothing), no `vcpkg`, no `pkg-config`. The CI's Windows
   leg installs SDL3 alone (`ci.yml:450-466`, vcpkg) and lets sqlite, curl and raylib skip
   (`ci.yml:468-471`). The shared brief asks for all four; the ffi-pragmatist's brief asks for "the
   headers the Windows CI leg's examples bind and the UCRT's", which is SDL3 and the SDK. The two
   briefs should name one set, and the SDL3 install on the box should be named as an act a seat
   may take. The SDK and UCRT are there: `clang -v -fsyntax-only` lists `Windows Kits\10\Include\10.0.26100.0\{ucrt,shared,um,winrt,cppwinrt}` and MSVC `14.44.35207\include`.
7. **"Clean but these briefs"**: `git status --short` also lists `docs/panel/223-briefs/`, and
   empty `223-reports/` and `224-reports/` directories stood before this file. Trivial.

## 3. Facts asserted without their command, now run or still open

- **Route B's "so all six are told at once"** is true of the count. It is not true of the words.
  On the box, the two come back in the probe's words: "*its expression holds an operation C
  leaves undefined for the operands it is given, and the program's own unit, built with clang's
  checks ... traps where it reads it*". The case's `.expected` pins clang's region words for
  them: "*shifting a negative signed value is undefined*" and "*signed shift result (0x80000000)
  sets the sign bit ...*". So route B as worded leaves the case red on the box unless the brief
  says which reading the case takes there. The words of route A or a front-end pass (§ 6, E) are
  the region's.
- **What `-fwrapv` silences, exactly**: `$S/wrapv/ovf.c` on both clangs. `-Winteger-overflow`
  still fires under `-fwrapv` (`2147483647 + 1`, `-(-2147483647 - 1)`, `65536 * 65536`: three
  errors either way). `3 << 30` is silenced like `1 << 31`. `-8 >> 1` warns nowhere. So the two
  warnings are the whole of what `-fwrapv` takes from the region's six; any sign-bit shift is in
  the class, not `1 << 31` alone.
- **Route A, "changes what C means for a header's own inline code"**: plausible and unrun, the
  seat's item 2. One related fact, run: `flags()` already carries two words that define C
  behaviour program-wide, `-fno-strict-aliasing` and `-fno-delete-null-pointer-checks`
  (`flags.hero:140-141`). `flags()` also reaches the runtime's object (`.claude/rules/generated-c.md`,
  `level_words`), so the runtime's key moves on every platform, not only "every object's".
- **Route A against a ratified ruling**: panel 218's R2 item 4 and its ratification read
  "`-fno-wrapv` in its words **and never in the program's**". Route A reverses a ruling ratified
  this morning, and the brief does not say so. R3 also says the region "IS the program's compile
  and cannot take the word, is superseded there by R1 as everywhere". 2120 is that supersession
  failing because of the order in `assemble.hero`, which the brief could quote.

## 4. Route D's premise

**It holds as far as I searched, and beyond that it is a question.** Nine spellings, each around
the two accessors under `-fwrapv` (Mac) and the box's default: `__attribute__((optimize("no-wrapv")))`
(`-Wunknown-attributes` on both, ignored), `#pragma GCC optimize("no-wrapv")`, `#pragma clang
optimize off`, `__attribute__((optnone))`, MSVC's `#pragma optimize("", off)`, `#pragma clang
attribute push (optnone)`, `#pragma STDC FP_CONTRACT OFF`, `#pragma GCC push_options` with a
`target` attribute. None brings either warning back (`$S/wrapv/d1.c` to `d8.c`, zero region
diagnostics each, against `d0.c` the control). The vocabulary searched: every string holding
`wrapv` in both binaries (`strings -a` of Xcode's clang; `grep -a -o` of the box's `clang.exe`)
is one of `-fwrapv`, `-fno-wrapv`, `-fwrapv-pointer`, `-fno-wrapv-pointer`; `clang -cc1 --help`
lists only the options. The silence is in the front end (`-fsyntax-only` shows it). What D reaches
for is reachable **per compile** rather than per pragma; § 6 names those.

## 5. The soundness lane

**Right on the letter, with two conditions the brief should state.** No spec token: `grep -n -i -E
'undefined|wrapv' spec/heroes-spec.md` finds nothing, and § 13 says only "clang checks every
result type, constant and record field against that header". No surface. `ffi_constant_type`
stays the class on every route. But:

- **WHEN a diagnostic is told is design.md §4.17's subject**, lines 2333 to 2345: "*What a broken
  program costs beyond that is counted rather than promised: a second message for one mistake, a
  mistake of the parse stage told only once another is fixed ... A later stage's mistakes wait for
  the earlier stage's by design*". 2120's class line uses the same words. A resolution that
  *promises* one build for this class amends a design.md Part 4 sentence, and so does route C if
  "said" lands in spec § 13 or §4.17. Either leaves this lane: the full panel, with the
  llm-ergonomist, whose instrument is Part 11's metric 4 (§4.17 names it). The brief should say
  the lane holds only for a resolution that writes neither.
- **Routes B and C change the words a Windows author reads for two values** (§ 3). The class is
  unchanged. CLAUDE.md § 4's trigger is "a diagnostic **class**", but the `/panel` skill's lane
  test is "no surface, **no diagnostic** and no spec token". The synthesis should say which reading
  it took.

## 6. What the framing misses: the class is not Windows's, and routes nobody listed

**The same two-build order holds on this Mac, with no `-fwrapv` anywhere**, by Apple clang 21 and
`$S/tree/heroes-next build`. The region's comment names values that fold with no warning
(`value_check.hero:35-40`). Each of the following exits 1:

| program (`$S/shapes/`) | binds | told |
|---|---|---|
| `both.hero` | `SHIFT_PAST (1 << 40)`, `DIV_MIN ((-2147483647 - 1) / -1)`, `FCAST ((int)1e20)` | `SHIFT_PAST` alone |
| `probe_only.hero` | `DIV_MIN`, `FCAST` | both, by the probe |
| `mm/main.hero` + `mm/other.hero` | `SHIFT_PAST` in `main`, `DIV_MIN` in `other` | `SHIFT_PAST` alone |
| `missing.hero` | a name the header lacks, `DIV_MIN` | `ffi_unknown_name` alone |

So 2120 is the `-fwrapv` instance of a platform-independent order. Every value only the probe
sees (`INT_MIN / -1`, a float cast out of range, panel 218's initialisers) waits until **every**
unit of the program has compiled, on every target. Routes that act on `-fwrapv` (A, D, E below)
close the box's two and leave these shapes at two builds. The brief's measured set has the 2072
case alone, and should add them, without that deciding anything.

**Routes the briefs did not list** (the option set, not a ranking):

- **E, a front-end pass under `-fno-wrapv`**, `-fsyntax-only` over each unit that holds a region,
  for its diagnostics alone, with the object compiled under the target's own words. Measured on the
  box's real unit (approximate words): six region errors, the case's own words. Its cost, a
  second front-end run per such unit, is unmeasured.
- **E', the accessors in a translation unit of their own**, compiled with `-fno-wrapv`: D by a
  compile's words rather than a pragma. Open question: a header's inline code would then be
  compiled under two meanings in one program.
- **F, the region's six as warnings read back after the unit compiles**, so the probe runs in the
  same build. The precedent is in the tree: `undefined_words.hero:6-8`, for a record's accessor,
  "*Never errors in a record's accessor: an error stops the unit's compile, and every constant the
  probes would tell waits for the next build*" (panel 221). B's two variants keep the region's
  errors; F removes them.
- **G, `-fno-ms-compatibility`**: measured to drop clang 23's `-fwrapv` (§ 2.3), and it changes
  far more than that, since the SDK's headers rely on MS compatibility. Listed for the set and as
  the trigger.
- **A fact for B, run**: `-w`, which the probe's compile carries (`value_run.hero:131`), silences
  the region's `#pragma clang diagnostic error` too. `clang -w [-fno-wrapv] -fsyntax-only` of the
  mimic and of its `-E` output gives 0 errors on both clangs. So a probe compiled over a refused
  unit's text would not stop on the region. It is unrun on the real probe text.

**The question the sitting might also ask**: is the order (a refused unit stops the probe for the
whole program) the defect, and `-fwrapv` the platform that exposed it?

## Brief repairs recommended

1. Say where the `m-bounded-seed` tag was placed for the `records/tagged` reading (none exists in
   the repository; without it the check reads 26 passed, 0 failed), and that 2099 is counted
   against the same tag.
2. Strike "which never optimises on signed overflow": MSVC 14.44 at `/O2` folds `(x * 2) / 2` to
   `x`.
3. State the trigger: clang 23 adds `-fwrapv` with `-fms-compatibility` on any target, from the
   driver and not a config file; `-fno-ms-compatibility` drops it.
4. Say the box lacks `3c478500b` (push it first), and that measurement 1 now reproduces at HEAD:
   `0 passed, 1 failed`.
5. Fill in the CI's Windows clang (20.1.8, the case green there), and say route C's skip is by
   machine, which `platform_skips.hero`'s rule excludes, and would skip a case green on the CI.
6. Name one header set for item 2 (the box has none of curl, sqlite, raylib, SDL3; the CI leg
   installs SDL3 alone), and whether a seat may install SDL3 on the box.
7. Route A: say it reverses panel 218's ratified "never in the program's" and moves the runtime's
   key on every platform.
8. Route B: say which reading the 2072 case takes on Windows, since the probe tells the two in its
   own words, not the `.expected`'s.
9. Add the platform-independent shapes (§ 6) to what is measured, and E, E', F, G to the routes
   listed.
10. The lane: state that it holds only for a resolution that writes no promise into design.md §4.17
    and no sentence into spec § 13; otherwise reconvene full.
