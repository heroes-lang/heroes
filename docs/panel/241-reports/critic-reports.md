# Panel 241, the completeness critic's second pass: the two seat reports

Read 2026-11-10, 08:36 to 08:51, at `87d77dec2`. Both briefs (`00-shared.md`, `compiler-engineer.md`,
`ffi-pragmatist.md`, rewritten on the first pass) and both reports (`compiler-engineer.md`, 391
lines; `ffi-pragmatist.md`, 468 lines). Every run below was made in a detached worktree of that
commit, `.claude/worktrees/p241-critic2` (removed when done), with its own compiler: stage 1 by
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 9.88`, `sources
5aae00294f7cd6fa`), stage 2 by `./heroes build selfhost/main.hero -o heroes-next` (`real 113.06 user
85.87 sys 18.86`, `sources 110fad075b8d3bca`, the trunk's key; load 9.85 at the start, a planning
figure only). Logs, probes and that compiler are in `.claude/worktrees/p241-scratch/critic/pass2/`.
No paid run, no GitHub write (issue reads only), no credential printed, nothing timed. One slip: a
list was redirected to `/tmp/x` for one command and deleted by the same command; no other file was
written outside the repository.

No verdict. Items are numbered most important first; each names its command.

## A. What the synthesis must settle, most important first

1. **The two seats do not contradict each other on the compiler's own hashing; they answer two
   subjects, and the cost of the third one is unmeasured.** The compiler-engineer approves *"(c) or
   (d) for anything the compiler itself hashes"*. The ffi-pragmatist objects to (c) for the PACKAGE
   `crypto/sha256` and writes *"The compiler keeps what it has"*: `hero_file_sha256` and
   `hero_text_sha256` stay in `hero_compiler.h`. On the compiler both land in one place, the C door
   it binds today (`sed -n 11,14p selfhost/module/fingerprint.hero`: *"Every file is hashed over
   its bytes, in C (`hero_file_sha256`)"*). The real disagreement is a program's package on (c):
   the compiler-engineer is indifferent (*"costs the compiler 0 lines"*), the ffi-pragmatist objects
   with a measured fact (against an unmodified runtime the package is `ffi_unknown_name`, so it is
   tied to the compiler's release). **What nobody measured**: (d) as the compiler's own fingerprint,
   which a package manager *"rewritten over the core packages"* would mean if `crypto/sha256`
   is one of them. The compiler-engineer's *"(d) costs nothing it does not already hold"* counts
   lines, not work. Arithmetic, unrun: route 1's placed tree is 93,113 bytes in 13 files
   (`find packages/core/0.2.0/files -type f -exec cat {} + | wc -c`), and the compiler's Heroes
   SHA-256 costs 1,686 to 1,695 instructions a byte at `-O0` (`sed -n 12,20p
   selfhost/cli/sha256.hero`), so about 157 million instructions a check, about 0.14 % of the
   110,503,846,740 the compiler-engineer measured for `heroes check selfhost/main.hero`: half again
   route 1's whole measured cost (+0.285 %). Panel 235 put the fingerprint on the C door for two
   reasons the synthesis should name if it keeps it there: a file that is not UTF-8 is no `str`
   (`fingerprint.hero:11-14`) and 1,603 instructions a byte at `-O0` (`sed -n 132,141p
   docs/panel/235-*.md`).

2. **The compiler-engineer's approved repair does not close both panics it found; and a third
   panic stands in `fetch/`, the very folder route 1 rewrites.** Both reproduced as stated with my
   own compiler, exit 134 each (`pass2/crash/build.log`: *"panic: string slice splits a character
   at emit/ffi_nested.hero:248:12, in ffi_nested.has"*; `pass2/cat/check.log`: *"... at
   module/catalogue.hero:268:39, in catalogue.table"*). But the repair approved is *"the 33 hand
   copies outside `cli/strings` (488 lines) repaired onto `cli/strings`' three, which closes the two
   live panics"*, and `catalogue.hero:268` is in none of the 36 copies (`grep catalogue
   p241-scratch/compiler-engineer/copies.txt`: nothing). It is one of the 15 inline cuts outside
   the copies: `git grep -nE` of the two slice shapes over `selfhost/`, lines above each module's
   `## Tests`, gives 18 code lines (and one comment, `bytes.hero:57`), the compiler-engineer's 18;
   3 of them are in copies (`symbolized.hero:73`, `ffi_field_told.hero:165`, `ffi_nested.hero:248`)
   and 15 are not (`pass2/slice-lines.txt`). **A third panic, reproduced here**: `heroes add core
   --index file://<repo>/packages` on a catalogue whose one line is `#12345678901234567é` (the `é`
   at bytes 18 and 19, so the cut at 19 splits it) ends at exit 134, *"panic: string slice splits a
   character at fetch/catalogue_edit.hero:197:39, in catalogue_edit.starts_table"*, after writing
   the placed tree (`find pass2/edit/heroes.packages -type f | wc -l`: 13) and leaving the catalogue
   unedited (`cmp heroes.toml heroes.toml.before`: equal). The same line with `ab` in place of `é`
   is `added core 0.2.0`, exit 0 (`pass2/edit-ascii/`). Route 1's diff leaves
   `fetch/catalogue_edit.hero` untouched (`git status --short` in `241-compiler-engineer`: 10
   `fetch/` modules, not that one). Searched and not filed: `gh issue list --state all --search
   catalogue_edit` returns nothing. **The first two are filed since the report said "Not filed"**:
   defects 2409 and 2410, both `class:blocking`, created 07:35:27Z and 07:35:38Z (`gh issue view
   2409`, `2410`).

3. **17 of the 36 hand copies, 351 of their 520 lines, are test support.** For each line of
   `copies-lines.txt`, compared with the line of its module's first `## Tests` heading (`grep -n
   '^## Tests'`): 17 copies stand below it (351 lines), 19 above (169 lines), 3 of those
   `cli/strings`' own. So the live copies outside `cli/strings` are **16, 137 lines**, not 33 and
   488. They are still emitted into the compiler's C (`grep -o 'h_checkholes_has_text'
   base-stage2.c`: prototype and definition), so *"in one binary"* holds; *"the robustness it
   closes"* is three live copies that slice (`symbolized`, `ffi_field_told`, `ffi_nested`) and one
   test one (`parse/records.opens_with`). Also, the report's *"33 hand copies"* and route 1's *"33
   calls"* are two different counts with one number; the synthesis must not merge them.

4. **Route 1 as scoped does not meet feature 2315's own sentence of what it replaces, and the
   milestone's likeliest compiler package is not in `core` 0.2.0.** 2315's body (saved in the first
   pass, `critic/2315-body.md`): *"What it replaces: the package manager's parts over C"*. The
   package manager's C groups, `git grep -n '^extern' -- selfhost/fetch selfhost/module/fingerprint.hero`:
   `index_source.hero:39,43`, `made_repo.hero:20`, `transport.hero:46`, `fingerprint.hero:23,27`;
   and `cli/process` in 11 of `fetch/`'s 21 modules (`grep -rl '^use cli/process$' selfhost/fetch |
   wc -l`). Route 1 moves none of them: its diff deletes 0 functions (`git diff -- selfhost | grep
   -c '^-function'` in the compiler-engineer's worktree: 0) and changes only `strings` calls. The
   compiler-engineer's *"fetch/ holds no copy"* is true of 0.2.0's contracts and of no other:
   `M-core-packages.md:448-452` names `path/filepath` the package *"the corpus will first need ...
   the package manager rewritten over these packages being the likeliest"*, and defect 2406 (open,
   `gh issue view 2406`) lists four `path`-shaped copies in `fetch/`: `last_slash` at
   `git_source.hero:180` and `index_source.hero:277` (both above `## Tests`), `made_index.hero:40`,
   `made_repo.hero:223` `last_part` (test support). No seat counted them, since `core` 0.2.0 has no
   `path`; taking it is a new `core` version, which by the compiler-engineer's own reading of
   `seed/README.md:142-153` is a seed refresh.

5. **Question 3 has no built route, and the transport panel 235 meant is not the one either seat
   priced.** The compiler-engineer approves *"R19 as written, the transport alone"* and objects to
   the one move of it built out on paper (an `os/exec` package double-binding `hero_os.h` beside
   `cli/process`, 96 modules by `grep -rl '^use cli/process$' selfhost | wc -l`); so its approval
   names no route. Panel 235's R10 (`sed -n 693,700p docs/panel/235-*.md`): the first version runs
   `git` and `curl` as programs, *"the seed's one clang line stays as it is"*, and *"The second
   version, over the core packages, meets the seed's question again"*. The milestone page plans the
   `net/http` client over libcurl (line 33) and `os` as *"`selfhost/cli/process.hero:37-56` lifted
   out of the compiler"* (line 25); neither seat read either row. A transport over a libcurl
   package puts libcurl on the seed's line: the ffi-pragmatist measured the libcrypto analogue
   (`'openssl/evp.h' file not found`, then five undefined `_EVP_*`, exit 1, without the three
   flags); for libcurl it is unrun, and on the Windows box unread. The ffi-pragmatist's container
   finding bears on it: the Linux image has libcurl and no `curl` program, so the first version's
   `heroes add` is `no_curl` there.

6. **The Windows `hero_env_put` finding: reproduced as stated by reading; its repair is unproven at
   the empty value, which a caller in the tree passes.** The runtime says it itself
   (`runtime/parts/os.c:900-905`): *"The CRT's narrow copy on Windows is not updated, and nothing
   here reads it"*; the first box run's log agrees (`sed -n 68,74p p241-scratch/ffi/win.log`:
   `hero_env_put: 0`, `parent sees null_cstr`, the child sees `café-2`). It is no defect of the
   compiler today: its three callers write for a child (`transport.hero:162-163`, `made_repo.hero:68-69`,
   `symbolized.hero:56`), and the harness reads back through `hero_env_shown`, the process block
   (`tests/harness/shell.hero:938-944`). It becomes one the day a program binds the door and a C
   library reads `getenv`. **The repair's run is not on disk**: `grep -rl 'process block holds'
   p241-scratch` finds only the probe's emitted C; `win.log` is the first run, before the probe read
   the block; only `runtime-e/parts/os.c:910`, the changed line, is kept. **The adjacent shape**:
   `tests/harness/case_setting.hero:212-226` restores a variable that was unset as `""`
   (`shell.env` answers `ok("")` for `HERO_OS_NOT_FOUND`), so `hero_env_put(name, "")` is a live
   call. `setenv(name, "", 1)` and `SetEnvironmentVariableW(name, L"")` set an empty value; the
   UCRT's documentation for `_putenv_s` says an empty value removes the variable. Whether
   `_wputenv_s(name, L"")` removes it on the box is **a question, unrun** (Windows read only here).
   A route nobody built: the narrow `_putenv_s` under the runtime's UTF-8 manifest
   (`runtime/parts/codepage.c:11-14`: *"every narrow call in it reads UTF-8"*), which the
   ffi-pragmatist's own package header used and read `café-1` intact.

7. **Moving names into `hero_os.h` meets an open task neither seat cited.** Task 2263 (open,
   M-publication-gate, `gh issue view 2263`, from panel 232's R6 and R8): *"the functions no program
   outside the compiler binds moved to a header of the compiler's own or kept knowingly, and what
   each `HeroStr` result hands its caller stated where a check reads it"*. The ffi-pragmatist's
   approved move (`hero_env_put` alone, `int64_t`) fits it only once an `os` package binds the door;
   the four-name move it performed for the costs puts three `HeroStr` results (`hero_file_sha256`,
   `hero_text_sha256`, `hero_bytes_sha256`) under the promise, and 2263 owes each a stated
   ownership. And decision 2228's promise sits at `docs/design.md:3641-3645`, Part 7, one of the
   Parts CLAUDE.md § 4 sends to a panel: whether adding a promised door is a "surface" the soundness
   lane may not touch is the synthesis's to say in words.

8. **The seed, measured now for (c) and for the move; (b) at the seed read from emitted C.**
   `clang -I <runtime-c> seed/heroes.c <runtime-c>/runtime.c` over the ffi-pragmatist's route (c)
   runtime (a copy; `diff -r` against the commit's: 3 lines in `hero_compiler.h`, 8 in
   `parts/sha256.c`): exit 0, `real 9.38`, `sources 5aae00294f7cd6fa`. Over a copy with
   `hero_env_put`, `hero_file_sha256`, `hero_text_sha256` and `hero_bytes_sha256` cut from
   `hero_compiler.h` and appended to `hero_os.h` (`pass2/runtime-m`, guard words not added): exit 0,
   `real 9.55`. So the compiler-engineer's unrun *"a stage 1 built with the changed runtime"* builds,
   for stage 1; stage 2 over the moved runtime is unrun. For (b) as the compiler's: the
   ffi-pragmatist's route-(b) program's emitted C reads `#include <crypto/sha256.h>`
   (`grep -n sha256.h p241-scratch/ffi/prog-b/build/layout-707c383756847ed8/locks-3a0e2fd41effb8ae.c`:
   line 4), found by an `-I` at the package's root; a seed emitted over it would need an `-I` naming
   the placed folder, whose name moves with each version (`placed.hero:108-110`). Unrun as a seed.

9. **design.md:571's letter is checkable, and its source reads as the compiler-engineer reads it.**
   `sed -n 570,572p docs/design.md`: *"No package is compiled into the compiler or the runtime
   (panel 097's condition 5: such source is the standard library this section forbids)"*. Panel 097's
   condition 5 (`sed -n 128,129p docs/panel/097-*.md`): *"Nothing reaches
   `selfhost/library_source.hero` -- the prelude"*. Panel 234's R13 already lets the compiler take
   a checked copy (*"the compiler takes a copy under `selfhost/core/` only if the admission test
   admits it"*, `sed -n 488,499p`), and the compiler-engineer's own row there approves *"M a package
   reached by the compiler through a checked copy"* (line 160). The ffi-pragmatist's *"A Heroes-only
   module such as `core/strings` costs the boundary nothing"* speaks of C and not of this line, so
   it is no approval of route 1 against it. Whatever the synthesis adopts for question 1, it owes
   design.md:571 either amended or read in words, and a Part 1 sentence is what CLAUDE.md § 4 and
   the panel skill's *"When in doubt take the full panel"* put in question for this lane.

10. **Costs the whole-compiler variant of route 1 owes, uncounted.** 114 modules `use cli/strings`
    (`grep -rl '^use cli/strings$' selfhost | wc -l`); 50 call only `starts_with`, `ends_with`,
    `holds`, and 64 also call a function `core` 0.2.0 has no contract for (`without_returns`,
    `trimmed`, `blank`, `bool_text`, `with_all`; `pass2/cli-strings-users.txt` and the loop beside
    it). Those 64 would hold `use cli/strings` and `use <core>/strings` together, both bound as
    `strings`, so each needs an `as` (the compiler-engineer already wrote `as core_strings` in two).

## B. Claims checked, and what they read

11. **The hooks' own tests are red at `87d77dec2`: reproduced.** `python3 -I -B -m unittest discover
    -s .claude/hooks -t .claude/hooks` in `p241-critic2`: `Ran 214 tests`, `FAILED (failures=1)`,
    `TrackedAsks.test_the_program_asks_nothing_and_a_push_is_still_asked`, the test pinning
    `['Bash(git push*)']` against five rules (`pass2/hooks-tests.log`). The rule moved at
    `ce9c288c3` (2026-11-09 09:37, *"the push rule narrowed to the forms that reach GitHub"*;
    `git log -- .claude/settings.json`); no issue found (`gh issue list --search` of the test's
    words). The compiler-engineer's patched run, 215 and 1 red, and its `+60 -5`, `+18 -2`:
    confirmed (`git diff --numstat -- .claude/hooks` in its worktree; `hooks-tests-2.log`).

12. **Stale figures, reproduced.** `ROADMAP.md:21` says 205,397 lines in 786 modules; `find selfhost
    -name '*.hero' -exec cat {} + | wc -l` gives 205,539 in 786. The same row's seed, 317,924 lines
    and 33,263,247 bytes; `wc -lc seed/heroes.c` gives 317,928 and 33,265,381. And
    `.claude/rules/verification.md:1109-1110` still gives the latency level as 7.50 s, 9.00 s and
    41.00 s, where `tests/harness/suite_latency.hero:51` says *"8.9, 10.3 and 49.1 s since
    2026-11-09"*: my first pass (item C3) quoted the stale rule; the brief's 8.9 and 49.1 are the
    suite's.

13. **A fourth live slicing copy, Windows only: read, unrun.** `selfhost/cli/symbolized.hero:69-77`,
    `holds`, cuts at every byte; `prepare` returns unless `process.exe_suffix() == ".exe"` (line 44),
    then calls it on the user's `ASAN_OPTIONS` (line 61). So `heroes run --sanitize` on Windows with
    an `ASAN_OPTIONS` of 25 bytes or more holding a character past ASCII would reach the cut. Not run
    (Windows read only).

14. **Windows for route 1, one cost already paid.** Neither seat built the compiler over route 1 on
    the box. Line ends cannot move its fingerprint: `git check-attr -a --
    selfhost/heroes.packages/core@0.2.0-4029f4f2486e6ca7/strings.hero` reads `text: unset`, by
    `.gitattributes:47` (`**/heroes.packages/** -text`), so panel 235's `core.autocrlf` finding
    (lines 135-137) does not reach a committed placed tree. What stays unrun: the three package
    routes' `fopen` on a path past ASCII on the box (every file the ffi-pragmatist hashed there was
    ASCII-named).

15. **The ffi-pragmatist's `const` finding is a ruled refusal, not a new gap.** Its negative search
    holds (`grep -c const spec/heroes-spec.md`: 15, none on a result; no golden says ``returns
    `const``), but panel 083's R4 refused a `const` spelling and panel 112 sat on it again
    (`sed -n 13,30p docs/panel/112-const-is-the-wrong-noun.md`). `-> ptr` is the ruled route.

16. **Small.** The ffi-pragmatist's *"the milestone page's row (line 32, measured 2026-09-03)"*: the
    row entered that file at `efb769424` on 2026-09-12 and was respelled at `fd15c2280` today (`git
    log -L32,32`); 2026-09-03 is the page's scheduling date. The compiler-engineer's *"R1's
    admission test admits on the count and on the two panics"*: only `ffi_nested`'s panic is in a
    copy of a `core` contract; `catalogue.table`'s is not (item 2).

17. **Verified as written.** The 33 calls in `fetch/` (22, 6, 5) and the 370 outside it (109 in 49
    modules, 60 in 32, 201 in 54) by `grep -rnoE 'strings\.<name>\('`; `cli/strings.starts_with`
    and `core`'s `has_prefix` the same loop (`sed -n 112,122p selfhost/cli/strings.hero`, `sed -n
    17,27p packages/core/0.2.0/files/strings.hero`); the five compiler modules binding
    `hero_compiler.h` for the env write or the digests (`git grep -l 'extern "hero_compiler.h"'`);
    core 0.2.0's 10 modules and 2,334 lines; route 1's 796 and 207,884 (205,539 + 2,334 + 11); the
    site's walk counting `heroes.packages/` (`site/src/lib/repo.ts:149-155` skips only names
    opening with `.`); `packages` pinning `file://../../packages` (`suite_packages.hero:90-93`);
    `order` not among the suites that ask `placed_trees` (`grep -ln placed_trees tests/harness/*.hero`:
    canonical, corpus, emission, packages, records, unseen) and core's `json.hero:186` an unmarked
    `sort(keys(...))`; `heroes build` at `-O0` and `heroes run` at `-O2` by default (`--help`, lines
    29 and 39); the spec naming no platform condition. The compiler-engineer's latency and
    instruction counts were not re-run: they are its own measurements, on a machine it says was not
    still, and the instruction count is the reading that machine moves least.

## C. What the synthesis must not claim

- That the seats contradict each other on the compiler's own hash, or that both approve (c): both
  keep the compiler on `hero_compiler.h`'s C doors; they differ only on a program's package (item 1).
- That (c) or (d) costs the compiler nothing: (d) as its fingerprint is about 0.14 % of a check by
  arithmetic, unrun (item 1).
- That repairing the hand copies onto `cli/strings` closes the two panics: it closes `ffi_nested`'s;
  `catalogue.table`'s and the third, `catalogue_edit.starts_table`, are inline cuts (item 2).
- That the panics are unfiled: 2409 and 2410 are open and `blocking`; the third is unfiled (item 2).
- That 36 copies, 520 lines, or *"33 copies, 488 lines"* are live code: 16 copies and 137 lines are,
  outside `cli/strings` (item 3).
- That route 1 replaces the package manager's parts over C, as 2315's body says the feature does
  (item 4); or that `fetch/` holds no hand copy, unqualified (item 4).
- That question 3 has an approved route: *"the transport alone"* has none built, and R10's seed
  question for it is unasked (item 5).
- That `_wputenv_s` is verified at every shape: the empty value is unrun, and the run of the table is
  not on disk (item 6).
- That moving `hero_env_put` into `hero_os.h` is free of the promise's open work (task 2263, item 7),
  or that it sits plainly inside the soundness lane (items 7 and 9).
- That the seed owes nothing for (c) beyond stage 1: stage 1 builds over both changed runtimes, stage
  2 over the moved one is unrun (item 8).
- That `heroes check`'s latency moved or did not move on the clock: no verdict either way; the work
  moved +0.285 % (the compiler-engineer's count, item 17).
- That the hooks' tests or the ROADMAP's counts are green or current at `87d77dec2` (items 11, 12).
