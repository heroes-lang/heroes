# 075 · M-package-manager: a dependency named by an alias in angle brackets, a catalogue that holds the build list with its fingerprints, and two verbs, the only ones that fetch

## Goal

Until this row a program reached code of somebody else's only by carrying a copy of it: the
thirteen programs of `examples/` that used the core packages each held a `core/` folder, reached by
a bare `use core/…` and held byte-identical by a check. design.md §3.5 had fixed the shape since
August, *`heroes add`/`heroes fetch`, inside the same binary*, and panel 056 had refused a
per-project file on three conditions. The author opened the row on 2026-11-06 beside
M-core-packages, with seven positions (`docs/roadmap/milestones/M-package-manager.md`): copy Go or
Cargo whole rather than invent; the first element of an external `use` an alias between angle
brackets, `use <json>/parse`, looked up in the program's catalogue, never a domain in the code; a
bare `use` the program's own module, so the bracket is the whole difference between mine and
fetched; a catalogue whose shape admits a Heroes index tomorrow (row 78); and the work in three
tempos, a first version over C, the core packages distributed by it, then the package manager
rewritten over the core packages with its interface unchanged.

Panel 235 sat on 2026-11-07, after a survey of thirteen package systems and seven indexes the
author read first, and ruled R1 to R20 (decision 2329, ratified that evening): Go's modules in
their newest release, the program's catalogue `heroes.toml` holding every package of the build
with the version minimal version selection chose and its fingerprint, no lock file; a TOML subset
read all or nothing; a fingerprint over a package's committed blobs, checked at every build; the
placed trees inside the program in a folder no `use` can spell, `heroes.packages/`; `build`,
`check` and `run` never fetching; the C boundary's C1 to C5, no fetched code run at build; `core`
one fingerprinted entry. **This row delivers that first version**, its interface fixed by goldens
(R19), and the core packages' first batch distributed by it.

## What surprised

**A measurement of what models write measured the example they were given.** The reader test ran
240 first turns over eight arms, three spellings and five catalogue formats, and 236 were right at
the first turn; none failed on its catalogue or its `use` lines, and every answer copied its
section's example to the indentation (120 of 120 TOML catalogues with `[dependencies.<name>]`
tables). The test could say that the author's form stood, and nothing about which form is better,
because a form a model copies is not a form a model chooses. The question that would have parted
the formats, how a catalogue a verb writes is read, reviewed and merged, was found after the money
was spent, and is owed.

**A fix that crosses a boundary has to ask what stands on the other side.** The sitting's prototype
offered *write `use roman/numeral`*, `certain`, where the catalogue had been deleted beside a
program holding its own `roman/numeral.hero`: applied, it would have compiled the program's module
in the package's place. The rule the landing keeps is that a fix moving a `use` across the mark is
`certain` only where nothing on the other side answers the same path; and the same prototype
compiled a program at exit 0 from the entries read above a refused line, which is why a catalogue
that does not read names nothing.

**The fingerprint is a question of which bytes, before it is a question of which hash.** One
commit's archive, made by five producers, gave four SHA-256s; a clone with `core.autocrlf`, or a
repository whose own `.gitattributes` writes CRLF, moves the working tree's bytes and `git
archive`'s; the committed blobs move with neither. So the sum is Go's `h1` shape over the blobs,
one line per file sorted by the name's bytes, and a file that is not text needed a runtime door,
`runtime/parts/sha256.c`, because the compiler's own SHA-256 read text alone.

**The tools a verb runs are code that runs.** A URL handed to `git` without `--` ran
`--upload-pack`; a committed symlink came through a clone pointing outside the package; two names
that differ only in case left one file on a case-insensitive disk; and a header that binds a
`constant` and carries a constructor ran its C during a cold build, because judging a constant's
value is done by running a probe (panel 216's grant, given for a program's own headers). Each
became a refusal before any file is written (C3, C4) or a route that runs nothing (C5's reader of
clang's AST dump).

**A route written and tested is not a route taken.** C1's per-package header search
(`compiling.of_package`), C2's listing of what a package's unit reads (`compiling.within_root`) and
C5's reader (`cli/constant_dump.hero`) were each built as a function with its own tests, the first
step saying another lane would plug it in; no build calls any of them. The close's scoring found
it by running the predictions as written: a fetched package's own header is found only through
`--include`, and through it a fetched header's constructor ran twice during a cold build.

**The cost landed beside the folder that was budgeted for it.** The compiler-engineer priced the
spelling at 95 to 165 lines of `selfhost/parse/`, from a prototype that read the mark inside the
parser; the landing read what the mark holds from the line's tokens, along a seam, in
`selfhost/package_mark.hero`, and `parse/` grew 42. A budget held per directory is met by the
directory a module stands in, which is the premise `suite_layout.hero`'s `BUDGETS` comment already
states about itself.

**Placing fetched code where no `use` can spell it closes the double reach only for fetched code.**
A placed tree is refused when spelled bare (`use_holds_an_address`); a `path` entry naming a folder
inside the program's own tree is reached both ways, and compiles twice, two types for one record:
Dart's recorded failure, which the historian predicted and the close measured.

**The compiler a stranger builds is the seed's.** The batch that carried the first version kept the
seed of `v0.6.0`, by M-bounded-seed's rhythm, while the site's pages that went out with the same
push tell a reader to run `heroes add core`; the compiler the README's clang line builds answers
`unknown command add` and refuses `use <core>/strconv` with `expected_module_name`.

## What broke and why

**Found by the sitting, beside the proposal**: defect 2324, a folder named by `--include` searched
before the runtime's headers, so a planted `heroes_standard.h` switched off the overflow abort in
the program's own code, repaired at `547c0eb3e`, every unit searching the runtime first; 2325, `use
./json/parse` told the path climbs, repaired at `f24f97e9a`; 2327, a module named `_0opt` spelling
C names in the emitter's own namespace, repaired with R17 at step 4 (`b17a032da`). **Found by the first lane**: 2333, a
`stdint.h` beside a program standing in for the C library's, repaired at `96d42c250` as a refusal
(`shadowed_header`) and sent to a sitting, panel 237, which found the class open through two more
doors, the environment (2344) and a header a group reads (2343), and a package's `-D` refused under
the allow-list's false notes (2348). **Found by the gate**: 2341, the `emission` suite skipping a
case that is a folder, the shape the fetch goldens introduced; 2345, `heroes help` saying `add` and
`fetch` were not built, under the rows that describe them. **Found by the CI after the push**: 2350,
on Windows a tree holding a git repository cannot be removed, 14 of the compiler's own 1,900 tests
failing at the clean-up of the package manager's tests, and the net never reached on that leg.

**Why the routes went unwired.** The C boundary's work was split across lanes by file: lane
pm-cbound wrote C1, C2 and C5's readers against a package root passed as a parameter, *since
nothing on `main` places a package yet*, and lanes pm-use and pm-fetch placed packages and wired the
`use` resolution and the verbs. Each lane's gate ran its own cases, and no case holds a header
inside a placed tree or an index (`find tests/golden examples -path '*heroes.packages*' -name
'*.h'` and the same over `tests/golden/fetch` and `examples/index` find none); the interface's goldens (R19) fix what the verbs print and write, not what a
build of a placed package reads. The predictions registered at the sitting were the instrument that
asked the question, and they were scheduled for the close.

## What landed, and what carried forward

**What landed**, every item a commit on `main`: `b32ac8632`, step 0, the row opened and row 78
entered; `e368cb861` and `6656268fe`, the author's word on CLAUDE.md § 2 and position after the
survey; `95a995bf7` and `4c2f1fcb9`, panel 235's briefs and sitting; `8f0e33203`, step 1, R12, the
catalogue the fourth input class; `574e579c7`, step 2, the form `use <name>/module`, its mistakes
told, and the walk of a new surface form; `301ce606c` and `234e157e8`, `009370e27`, step 3, the
catalogue read all or nothing and a bracketed `use` resolved through it, the specification's § 1
text (+390 real, priced at `ca83d921d`); `b17a032da`, `c5f904bc2`, `42fee1a66`, step 4, scope, a
package's C names its own, `internal/` held, a `path` entry told; `29def8a17`, `e08de7d40`,
`40781979f`, step 5, the fingerprint over every file's bytes, checked at every build; `961897eb2`,
step 6, the falsifier's goldens and a case that is a folder; `6358dddfd`, step 7, C1 and C2 as
functions; `4457ddbb3`, step 8, C5's reader; `b8257ee10`, step 9, the transport; `4a1e574aa`, step
10, staging from the committed blobs; `0ef6c2ea7`, step 11, the static index; `3d51f8958`, step 12,
`heroes add` with minimal version selection and `heroes fetch`; `9d29c5767` and `95e39a124`, step
13, the interface's goldens under `tests/golden/fetch/`; `5abbe7724` to `b8f29508b`, step 14, the
placed tree left out of `mutate`, `corpus`, `records` and `unseen`, and `core` one catalogue entry
fetched by fourteen programs from `examples/index/`, the site and the documents saying so. Defects
2290, 2292, 2308, 2324, 2325, 2327, 2333, 2341 and 2345 closed with batch 24, whose gate,
`a789691f1`, read 0 failed in each of 37 suites, the compiler's own tests 1,900 and the net's own
571. Panels 236 and 237 sat under this row's gate; 237 is ratified and lands in batch 25.

**Scored at the close** (panel 235's file, § Scored at M-package-manager's close, `be810d473`): the
compiler-engineer's line counts falsified as registered in the cheap direction, the ffi-pragmatist's
P1 and P2 falsified on this Mac, the spec-warden's W1, W3 as restated and W4 as restated held, the
historian's (One) held; the ledger's row 8327 paid by its W4 half.

**Carried forward**, as the close found it on 2026-11-08: panel 237's landing (batch 25), with
defects 2343, 2344 and 2348, `blocking`, which its R9 closes before this row's tag, and 2347;
defect 2350, `blocking`, the Windows leg red; C1, C2 and C5 taken by no build, a fetched package's
own header found only through `--include` and its constructor run at a cold build through it,
reported for filing; `use_holds_an_address` calling a placed tree's folder an address, and no case
holding `package_unmarked`, reported for filing; the compiler the seed builds, which reads no `use
<x>/` while the published site asks for `heroes add core`; the clause R13 bought, re-argued under
the removal branch, its line-count payment falsified; panel 237's compiler-engineer prediction,
registered at this close (`shadowed.hero` at most 260 lines, the two-line program's warm build 5
clangs), to be scored once batch 25 lands; panel 236's R7c on Windows, owed at the first Windows
leg that runs the net; the measurement panel 235 left owed, how a catalogue is read, edited and
merged (about 19 USD of the author's cap unspent); the word *package*, § 1's fetched package beside
§ 13's `package "raylib"`, the sitting's question 1, ruled by no commit of the landing; feature 974
recommended for re-homing to M-package-index; the open items of this milestone, feature 2311 and
tasks 2312 and 2313, and of the third tempo, 2314 and 2315; the historian's (Two), at
M-core-packages' resumption; and the latency ceiling's `record` and `compare` against
`m-latency-ceiling`'s compiler, the close's own measurement.

## The close, 2026-11-09

**Batch 26 went out first**, `1517fae51` pushed in the afternoon with 22 issues closed, and its CI
run, 42639480634, read Darwin arm64 green and the seed's refresh rebuilt by the seed before it;
Linux x86-64 and arm64 red on the case of defect 2056, which was defect 2395 seen on glibc; and
Windows **cancelled at the job's 240 minutes**, its net past 21 suites and still passing them, with
`unsupported: 379 passed, 4 failed` and the four never named, since the net prints its `FAIL` lines
at its end. The Windows box, started by the author that evening, did not reproduce them: at
`1517fae51` and at the round's head, built in two stages from each seed with `pkgconf` installed
from vcpkg as the CI's step installs it, `unsupported` read 383 and 0 and 393 and 0. So the four
are a fact of the runner the box does not share, filed `blocking` as defect 2398 with
everything measured; and the job's ceiling went to 360 minutes, GitHub's own for a hosted runner,
the author's answer the same evening, with splitting Windows' net into jobs a task
(task 2400).

**Batch 27 repaired five defects**: 2388, a `const` member inside an array's element, which C11
refuses to assign, told `ffi_const_field` at exit 1 (decision 2387, the author's *repair it
tonight*); 2389, a header macro that renames one of the compiler's own C names, three shapes;
2390, a placed tree's folder no longer called an address; 2391, a number inside a `use` path told
once; and 2395, `fabs` and `ldexp` that glibc defines in libc and libm. Gate A, the fixpoint
`7273148d6736fed6`, the compiler's own 1,989 tests and the net's own 584; gate B, every suite and
form, 11,858 passed and 13 failed, each told before it was answered: two `keys()` walks the lanes
left without an `ORDER` mark, a comment of the coordinator's that wrapped *M-package-manager* at a
line's end, and the eleven new cases with nothing blessed (`0561a89e5`, the emissions blessed with
the round's last compiler).

**The latency ceiling was the close's own measurement**, on the still machine after the gate: the
row of 2026-11-09 read `check` 8.10 s, the seed 9.32 s and the warm build 44.68 s, past decision
2282's level of 7.50, 9.00 and 41.00 s, and the compare against `m-latency-ceiling`'s compiler read
`check` 1.4 % slower on the same tree, the build without a verdict. Two causes, both measured: the
tree grew 12.3 % (182,687 lines in 701 modules to 205,235 in 786), the previous tag's compiler
taking 8.00 s of `check` on it; and a bisect of eleven steps named the merge of step 4
(`b17a032da`) the first commit to read slower, 0.8 %, the rest not located. Decision
2396 raised the level by 2282's own rule to 8.9, 10.3 and 49.1 s and the start page to
*under ten seconds* (`429464c94`); the 0.8 % and the build's verdict are a task (task 2399). The
level's own test had pinned the old ceilings in its text and read red in the net's own tests from
that commit, which only gate B, the net, had been run over; panel 240's compiler-engineer found it,
and `b7aaa1db1` writes the text from the constants it judges.

**And the words.** Asked that evening whether the root folder `packages/` was misnamed, *since in
Heroes it is called a module*, the author was shown that a module is a file and a package what the
catalogue fetches, so the folder was right, and that the word meant something else in one place,
§ 13's `package "raylib"`, a C library `pkg-config` is asked about, the clause panel 050 wrote and
panel 235 left unruled. The author's answer, *one name for one thing, before the tag, with a
reduced sitting*, convened panel 240: three seats and the critic, no blind seat, no paid run by a
seat. Its compiler-engineer built the rename and found the precedent, `@` to `@=` at panel 209; its
spec-warden priced 23 drafts and gave the wording, T1, six tokens below today; its historian found
no system of fourteen that writes the bare word for a `pkg-config` lookup and five that name the
second route after the tool. The critic repaired seven facts of the briefs before any seat started,
corrected the synthesis in thirteen places and found the five user-facing messages the prototype had
left saying *package* in the C sense, one pinned by four expected files the net would have kept
green. The landing (`fd15c2280`) spells the mark `pkgconfig`, refuses `package` with
`old_package_word` and the tool's own spelling with `pkgconfig_spelled`, each with a `certain` fix,
moves the tree by that fix, holds § 13's marks to the compiler's words with a check that fires,
and refreshes the seed in the same commit. Its count is the night's other measurement: T1 read 10,904
real on the first `--refresh`, above 10,900, which falsified the spec-warden's registered W1, and the
removal W1 named paid in the same commit, the sentence losing *and what else it needs*, 10,898 real on
the second; the vendored scale had said −6 where the reader's own said +4. The compiler's internal
names are a task (task 2401), § 13's sentence that JSON comes from C libraries another (task 2402),
and the author ratified the sitting at 23:50, by reading, decision 2397.

**And two habits.** The plan's graph, drawn by hand all evening and published again each time the
author asked, became the skill `/graph` on the author's word (`d723e92d6`, its template made English at
`20b500353` when `records` read it). And asked for a way to clean the worktrees, the coordinator
measured the 29 standing: 18 merged into `main` with nothing uncommitted, the only untracked files a
rebuildable seed binary and sitting 239's reports identical to the committed ones, removed on the
author's yes with their merged branches (36 GB); and `/step` § 5 carries the removal at every close,
by the same three tests (`683fc2289`).

**The author left the session at about 00:20 on 2026-11-10**, giving it, for itself alone, the box,
the commits, the pushes, the site's publication and the tag (CLAUDE.md § Hard stops, a session left
unattended).

So the row says *done*. The tag is placed over this commit once the round's push reads its Windows
leg: `unsupported` with no failure closes defect 2398 by that run and the tag follows; a
failure names its cases at the net's end, and the tag waits on their repair.

**And the Windows leg, 2026-11-10.** The round's push (`c338bef33`, CI run 42721242127) read Linux
x86-64 and arm64 `harness: 11865 passed, 0 failed` and Darwin arm64 `11906 passed, 0 failed`; the
Windows leg ran its net to the end in 4 h 09 min under the 360-minute ceiling and read `unsupported:
389 passed, 4 failed`, every other suite at 0. So defect 2398's four were named at last, and they
were not the cases the box judges only with `pkg-config` but defect 2372's: a header's name nothing
links, told `ffi_missing_link` on every other linker and an `internal error` at exit 2 under MSVC
`link.exe`, the CI's linker, whose `LNK2019` line no reader of a refused link knew. The box had read
them green because its clang links through `lld-link`; made to link through `link.exe` by a
`clang.cfg`, it reproduced the four with the CI's words, and the repair (`c6dd426cd`) reads that
line as every other linker's, the four green on the box under both linkers and the whole
`unsupported` form 390 and 0. Two neighbours of the same family were found there and repaired beside
it (`3f8f9d15a`, defects 2407 and 2408): a missing library under `link.exe` and a
`__declspec(dllimport)` name under `lld-link`, each an exit 2 that is now an exit 1. Defect 2398
closes with that repair at the gate, and the tag waits, as the author asked, for a Windows leg that
reads `unsupported` with no failure: it goes on the first commit whose leg does, the push that
carries the repair.

**And what that leg read, 2026-11-10.** The push of `87d77dec2` (CI run 42760969959) read Linux
x86-64 and arm64 `11865 passed, 0 failed` and Darwin arm64 `11906 passed, 0 failed` again, and the
Windows leg ran its net to the end with `harness: 11607 passed, 0 failed` and `unsupported: 393
passed, 0 failed`: defect 2398's repair held where it had to. The leg was red all the same, on the
next step, the net's own tests, 588 run and 7 failed, which no Windows leg had reached since
`5bd4cf3e4` on 2026-11-07, each leg since stopping earlier: two tests of defect 238, a program built
in a folder above ASCII and one built by a compiler installed under such a folder, counted once for
each program whose own tests run with the net's. Both ended at `0xC0000409`, the UCRT's abort, and
the same builds on this Mac stopped at exit 134 in `cli/package_reads.hero`, a cut of clang's `-M`
listing at every byte offset since 2026-11-08: every build of a program in a folder above ASCII, on
every platform, and only those two Windows tests had ever built in one. It is defect 2418; the round
that panel 241's three crashes had opened already repaired it, as a shape beside defect 2409
(`2bc18d007`), and gave it a test of the net's own that every machine runs (`fce7db267`).

That round, `b28-round`: defects 2409, 2410 and 2411 repaired onto `cli/strings`' byte reads, 57
lines and a guard of their shape with them; defect 2412, the hooks' own tests red since `ce9c288c3`;
panel 241's sitting; the ROADMAP's counts re-measured. Its gate on this Mac: stage 1 from the seed,
stages 2 and 3 emitting the same C (`cmp` silent), the compiler's own tests 1,995 and the net's own
588 all passed, the hooks' own 215; the full net, 31 suites and 6 forms four at a time with `cache`
alone, 0 failed in each but `records`, 27 and 1 on an undated citation this round had written
(repaired), `run` 736, `emission` 2,541, `check` 760, `annotations` 1,353, `unsupported` 432, and
`FLOOR fetch` raised to 14; then the suites the changes after it touched, read once together:
`records` 28, `unseen` 3, `canonical` 2, `fetch` 15, `spec` 35, `probe` 31, the net's own 589, all
at 0 failed; Linux arm64's `emission` 2502 and 0 before and after its blessing, the blessed tree
equal to the round's by `diff -r`; the site's build green. The author asked, as the leg came back,
that GitHub run only the part of Windows that fails and not five hours of the rest, so the push
carries `[skip ci]`, and the workflow gained `net: own`, which with `only: windows` builds the
compiler and runs the net's own tests alone; the tag waits on that dispatch reading 0 failed.

**And the tag, 2026-11-10.** The round's push (`a090af316`) carried `[skip ci]`, by the author's
instruction that GitHub run only the part of Windows that failed, and the author dispatched it at
11:13 with `only: windows` and `net: own` (CI run 42797273588): the Windows leg built the compiler
in two stages, skipped the compiler's own tests and the whole net, and read the net's own tests 589,
all passed, defect 238's two tests among them and defect 2418's new one. The round's defects were
closed after that push, 2409 to 2412 and 2418, so `records/tagged` refused the tag placed on
`a090af316` itself, whose commit is older than their closing; the list now holds no `blocking` and
no `systemic` defect, nine `adjacent` ones at their first tag, three `improvement` ones, and one
decision, panel 241's ratification (2415). So `m-package-manager` stands on this commit, the one
after those closes, as `m-latency-ceiling` stood on the commit after defect 2278's; `a090af316`
beneath it is the tree both Windows runs judged, and this commit adds only this paragraph and the
page's.
