# Panel 237: a file standing in for a C library header is refused from every folder a build reads, and a close that can refuse is bound `when 0`

2026-11-08, convened by the coordinator of batch 24 on decision 2342
(https://github.com/heroes-lang/heroes/issues/2342), under M-package-manager, whose panel gate was
asked once at panel 235. **Full panel**: the compiler-engineer, the ffi-pragmatist, the spec-warden,
the historian, the llm-ergonomist as one blind `claude -p` session, and beside them the coordinator's
reader test, 288 `claude -p` sessions over four tasks. A completeness critic read the briefs before
any seat started (22 findings, every one repaired before the first seat, `57d0909d1`) and the reports
after (8 findings and 7 things the synthesis must not adopt without a further run). Paid runs: the
author's yes of 11:44, a cap of 25 USD for the sitting; spent 17.7277 USD by the CLI's own reports, for the blind seat and the 288 reader sessions (`blind237.py sum`), beside the spec-warden's twelve `count_tokens` requests, whose price no command here reported. **Provisional, author
ratification pending**, asked by decision 2349 (https://github.com/heroes-lang/heroes/issues/2349).

## The proposal, verbatim (`237-briefs/proposal.md`)

1. **`shadowed_header`, landed on robustness** at `96d42c250` (defect 2333). Before any probe
   compiles, one `clang -v -M` over the runtime's own opening lists the C library headers the
   compiler's C reads; a listed file that lies in a folder the build names (the program's own
   folder, one `--include` names, one a package's `pkg-config` answers) stands in for the library's
   header, and the build is refused at exit 1, naming the file, the folder and the library header,
   the fix said in a note (rename the file or move it out of the folder), with no `Fix`.
2. **A package's `-D` or `-U` of a name the guard keeps** (`emit/guarded_names.hero`: every word the
   runtime's headers and the library's standard headers write, and the names C reserves that the
   compiler's C writes; decision 2342 calls them *a name C reserves*) is refused as `ffi_package`,
   landed with it; the compiler's own two `-D` switches excepted.
3. **Spec § 13's first fenced block binds `function sqlite3_close(db: Db consumes) -> i64 when 0`**,
   held out of the round at `ebdc8a28c` (defect 2322, repaired at `37fb22678`), and design.md
   §4.19's copy of the example the same.

## The resolution, provisional

**R1. Item 1 stays a refusal, under its own code, `shadowed_header`; no reorder of the search.**
Every seat that built or wrote the C kept the refusal's shape, and every reorder route failed on a
measured program. `-isystem` is no route: on Apple clang 21 and Debian clang 22.1.8 a folder named by
it is searched before the platform's, and its `stdint.h` is read (the critic's first pass, finding 9;
the ffi-pragmatist's 22 rows, the same verdicts on both). `-idirafter` for a package's or an
`--include` folder is **vetoed by the ffi-pragmatist on ABI breakage**: GNU readline's
`readline/readline.h` under `fluidsynth`'s folder loses to the SDK's libedit header, and `HIST_ENTRY`
comes out 16 bytes where the library writes 24, accepted by clang; for the program's own folder it
refuses a correct program's own `util.h` with a false message (the compiler-engineer). `-iquote`
needs every group's header emitted quoted, against design.md §4.19's *never quoted*, breaks the
header the compiler's own `cli/macro_twice.hero` drafts and 12 tracked include lines, and reopens
§4.19's quoted-include decoy (it prints `decoy` again, the ffi-pragmatist). The refusal stops no
package on this Mac (0 of 507 by folder) and none built on the two Linux images (0 of 52, 0 of 61),
refuses no tracked program (55 of 55 examples and 12 of 12 representative `run` goldens build), and
costs one clang process a build, +244 million instructions (+17.4 % on a warm two-line program,
+13.3 % with one group, the compiler-engineer, children counted). It rests on C itself: a file with a
standard header's name placed where includes are searched is undefined behaviour (ISO C, N1570
§7.1.2p3), and CERT C's PRE04-C says not to reuse one (the historian).

**R2. The refusal reaches every folder a build reads a library header from, the environment's
included, and a header read through a group's.** The class the landing closed, a wrong value at exit
0 where the language promises the program's own meaning (design.md §1.12), stood open through two
doors the brief did not name, each measured at exit 0 with the landed compiler and filed `blocking`:

- **the environment, defect 2344** (https://github.com/heroes-lang/heroes/issues/2344): a
  `stdbool.h` in a folder `CPATH` or `C_INCLUDE_PATH` names makes `print(true)` print `false`; a
  `math.h` there makes `sqrt(2.0)` print `42.0` on this Mac and Linux arm64; an empty element of
  either variable adds the folder clang runs in (`':'`, `'/x:'`, `'/x::/y'`, the critic's second
  pass); and `CCC_OVERRIDE_OPTIONS` adds a `-D` of `__builtin_add_overflow`, silently under a leading
  `#`. **The route**: the compiler-engineer's build of the critic's route (`237-reports/compiler-engineer.md` § Routes built for 2343 and 2344, prototype `sources 222d4bf371b8e243`): the cc1 job of the `clang -###` answer every build already asks is read, and a folder clang searches before the platform's that the compiler did not pass (`-I`, `-isystem`, `-c-isystem`, `.` from an empty element) and that is none of clang's own counts as a named folder for the refusal, the note naming the variable; a `-D` or `-U` of a name the guard keeps that the compiler did not pass is refused under a new code, `shadowed_macro` (R3), naming the variable; every other edit goes through. Measured: the critic's six cases refused, and three nobody had listed (`-I`, `-include` and `-imacros` added through `CCC_OVERRIDE_OPTIONS`, the last two printing `false` at exit 0 before); the critic's `envown` (2333), the readline binding through `CPATH` (2051) and `CPATH` naming the SDK's own folder still build; `-fuse-ld=ld` and `-Wl,-dead_strip` link. About 147 lines in `selfhost/cli/`, no clang process added, about +20 million instructions, +1.1 to +1.8 % of a warm build. It reads clang's answer rather than re-deriving clang's rules, which differ by platform: an empty `CPATH` adds nothing under Debian clang 22.1.8 and adds `.` under Apple clang 21. One strict refusal, said: `-Dtrue=0` through the environment is refused though harmless today, the line item 2 draws for a package. Measurement 055's `-fuse-ld=lld` fails to link on this Mac with the route or without it (Homebrew's `lld@22` cannot read the SDK's `libSystem.tbd`), so the route refuses nothing there. The compiler-engineer withdrew its blanket `#` and its 25-line count rule, which left seven of the cases at a wrong value.
- **a header read through a group, defect 2343** (https://github.com/heroes-lang/heroes/issues/2343):
  a program binding `stdio.h`'s `BUFSIZ` with its own `_stdio.h` beside it prints `7` where the SDK
  says `1024`, since the refusal lists only what the runtime's opening reads with no group. **The
  route**: the compiler-engineer's `tree` route: the refusal's one clang run gains `-H` and `-MG` and lists the runtime's opening and then the program's groups; a named folder's file that a group's header includes directly is the program's; a nested file is refused where a platform or runtime header included it, or where the platform has a header of that name; a file found beside the header that includes it, by a quoted name, is that header's own. Measured: `_stdio.h` and the critic's `viaown` refused; clean `viaown`, `utilown` (7) and `stdiogroup` (99) build; every landed refusal kept, and 67 of 67 correct programs build with both routes on. **It refuses one program that may be correct, and the sitting rules it refused**: `sibutil-angled`, a program's own header writing `#include <util.h>` for its own `util.h` where the SDK has a `util.h`, the angle form searching the system's places for a name the platform holds; the quoted form, `#include "util.h"`, builds, and the note says so (CERT C's PRE04-C, the historian). **And it rules `stdiogroup`'s premise**: a header a group names is the program's choice, read from where the program put it. Cost as a prototype: about 110 lines, no clang process added, +144 to +259 million instructions in the compiler, +8.2 to +13.0 % of a warm build, unprofiled; the landing measures it again and reports it (CLAUDE.md, Precedence: a guard that closes a corruption class lands with its cost measured and reported). With both routes `shadowed.hero` stood at 409 lines against the ceiling of 300, so the landing splits them into modules of their own.

**Refused, each on a measurement**: clearing `CPATH` or `C_INCLUDE_PATH` for clang, or refusing a
build where either is set (**the ffi-pragmatist's veto**: the compiler's own `machine_locked_path`
note sends a GNU readline binding to `CPATH`, and with the variable cleared it builds at exit 0 and
prints `1026`, libedit's header against GNU readline's library, where it printed `2051`); and a
blanket `CCC_OVERRIDE_OPTIONS='#'` for every clang (the compiler-engineer's condition), which would
drop in silence an edit this repository made on purpose, measurement 055's `#+-fuse-ld=lld` (the
critic's second pass, finding 5).

**R3. The words.** `shadowed_header`'s note names who named the folder: the program's own folder,
`--include`, the package by its name, or the environment variable by its name, since the note today
says only *the folder is this program's own, one `--include` names, or one a package's `pkg-config`
answers* (the ffi-pragmatist, design.md §4.17: everything needed to fix the mistake without opening
another file). **No `Fix`**: the repair is a file renamed or moved, which a fix of the program's text
cannot write; the two diagnostics the historian found that name a file standing in, Python 3.13's
and clang 22's `-Wshadow-header`, offer none either. The blind seat, given the refusal alone, wrote a
repair that builds and prints the right value (`237-reports/llm-ergonomist.md`, task 2, judged by
the registered criterion). **Item 2 stays `ffi_package`**, the same class as the other refusals of a
package's answer, and **its notes are rewritten**: they are today the flag allow-list's, which say
`-D` is accepted, give Go's CVE-2018-6574 as the reason, and advise `link`, which drops the
package's `-I` (defect 2348, `blocking`, https://github.com/heroes-lang/heroes/issues/2348). The note
says the refusal's own reason, that a `-D` stands before every header and every guard so nothing
undoes it, and names no repair that loses the header's folder. **The environment's `-D` or `-U` of a kept name is a code of its own, `shadowed_macro`**, beside `shadowed_header`: neither a package's answer nor a file, a word the build did not write, its note naming the variable.

**R4. `check` and the four verbs that compile C keep their line, stated.** `check` exits 0 on a
program `build`, `run`, `test` and `--emit-c` refuse at exit 1. `check` starts no clang today and
costs 76.3 million instructions on a two-line program; asking the refusal there would make it about
4.2 times as expensive (the compiler-engineer), the trade panel 082's R3 refused once on cost and
feature 974 names for a bindings module. The line is the C boundary: a program's C is judged by the
verbs that compile it, and design.md §4.19 and §4.17 say so in the sentences R9 lists. Feature 974
stays open for its own question and is told this ruling.

**R5. Pinned by a golden case whose stand-in does harm.** `special`'s two programs still panic
correctly with the refusal taken out, so they pin its message and not the class (the
compiler-engineer). The `unsupported` form reads a `.case` folder today (the compiler-engineer built
one, 1 passed and annotated at `1:1`): a folder whose `stdbool.h` makes `true` read as 0, beside the
environment's and the group's cases of R2.

**R6. Item 3: fence B.** `function sqlite3_close(db: Db consumes) -> i64 when 0`: **+4 real tokens**
(10904 to 10908, `claude-opus-5`, four paid refreshes by the spec-warden, the specification reverted
after each), against C's +6 and D's +41. It is what SQLite's own page says the call does: with a
statement unfinalized it leaves the connection open and returns `SQLITE_BUSY`, `sqlite3_close_v2`
being *intended for use with host languages that are garbage collected* (the historian); both SQLite
programs of `examples/` bind it so since defect 2291; and C would break `special`'s fence program
(`unknown_name`) and the examples' agreement (the spec-warden). The blind seat approved B and D and
objected to A and C; the ffi-pragmatist and the spec-warden approved B; the historian B or D. **The
reader test**: R1 to R3 named the refusal in their tasks and could not separate the arms (the critic's second pass); **R4**, the same SQLite program with the refusal named nowhere, registered before its sessions (`62a95b20a`), **decides nothing under its rule either**: silent 1 of 18 under A and 0 under B, C and D (one-sided Fisher p 0.5), correct 5, 7, 8 and 8. Read descriptively, and only so: the close was bound bare 4 times under A and 3 under C, never under B or D, and the one silent program, a Haiku session under A, printed `5` and `0` and exited 0 with the connection open, defect 2322's report; Opus 5.5 and Haiku 5.5 mostly bound the close `when 0` from what they know of SQLite whatever the fence said (`237-reports/reader-test.md`). **The payment** (design.md §1.6): a prediction registered here, before any landing, naming an instrument that exists (the reader test's runner, kept in the evidence): **with R6 and R7 landed, R4 run again on the landed specification (the same three models, 72 sessions) binds no close bare under fence B, and reads `lent_shape` as the first error of at most 19 of its first tries, half of today's 38**. It is a paid run of about 4.3 USD, the author's to allow at the landing; unrun or falsified, B's +4 and R7's tokens (+7 vendored for both, the real count owed at the landing) are paid by the removal the spec-warden priced, *and a call returning nothing takes no clause* in § 13's `when` paragraph (−10 vendored), counted on the real tokeniser then. design.md §4.19's copy
becomes identical to the landed fence, `lent` and `constant SQLITE_OK` included (it is refused today,
`lend_kept`, its `path` without `lent`), and a check holds the two identical; `suite_spec`'s
`fragments` moves with the fence in the same commit. The two thesis contexts stay as they are: the
registered prediction names the `spec` context alone, at `a4e09e061`, and these two are its
exploratory variants (`tests/thesis/held-out/prediction.md`), pinned by `README.md`'s sha256 as the
inputs a past run scored.

**R7. The specification takes in panel 171's ruling on `lent`.** Spec § 13 says *a group's record
declared `lent` without `@` is a pointer to const*, and a handle is a group's record with a `tag` and
no fields; the compiler refuses `lent` on a handle (*the pointer itself, which C receives by value,
so nothing is lent*) on panel 171's ruling, which the specification never absorbed (the critic's
second pass, finding 2). The reader test measured the cost: `lent` on a handle parameter was the
first error of **111 of the 118 refused first tries** across 216 sessions, Sonnet 5.5 building 0 of
72; deleting the word in R1's and R2's 75 such programs ran 66 right. So the sentence reads *a
group's record that is no handle, declared `lent` without `@`, is a pointer to const*, priced on the
real tokeniser with R6, and **`lent_shape` on a handle gains a `certain` fix deleting the word**,
which repairs exactly the defect it names (defect 2347, widened to the contradiction and re-classed
`blocking`: the specification admits a program the compiler refuses).

**R8. No spec text for items 1 and 2.** The specification names no diagnostic code
(`grep -c 'error\['` is 0), and no tracked `.h` outside `runtime/` bears one of the opening's 36
names; a sentence would cost every prompt for a case no tracked program meets (the spec-warden,
+32 vendored for the shortest).

**R9. The landing owes**: R2's two routes with every case listed under them; R3's notes; R5's cases;
R6's fence, `fragments`, design.md's copy and its check; R7's sentence and fix; design.md §4.19's
sentences on the refusal, its reach and the environment (the spec-warden's draft, 558 vendored
tokens, `237-reports/spec-warden.md`), and §4.17's on `check`'s line; defects 2343, 2344, 2347 and
2348 closed by it, before M-package-manager's tag (they are `blocking`).

## The verdicts

| seat | verdict | rests on | cost or delta measured | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **object**, no veto | §1.1, §1.7, Part 5 | +244 M instructions a build, one clang; `check` would be ~4.2x | `shadowed.hero` at most 260 lines by `layout` at M-package-manager's close, and a warm build of its two-line program still 5 clangs | approve with the environment counted, `CCC_OVERRIDE_OPTIONS` neutral, 2343 filed, a harmful golden |
| ffi-pragmatist | **object** to item 1 without the environment; **approve** items 2 and 3 (B); **veto** `-idirafter` for package folders and clearing `CPATH` | §1.11, §4.19, `design.md:2983` | 22 search-order rows on two platforms; 0 of 507, 52, 61 packages refused | with `CPATH` and `C_INCLUDE_PATH` folders counted, the `libm` program refused on both platforms, every `extern` example and the readline binding (2051) unchanged | approve as landed if the environment is ruled the user's trust boundary and §4.19 says so |
| spec-warden | **object** to item 3 as written, **approve B** on four conditions; no veto | §1.6, §1.2, CLAUDE.md § 2 | real counts A 10904, B 10908, C 10910, D 10945 | P1, the counts' band and order: held; P2, B preferred over A on R2 and C not: **falsified**; P3, B at most 2 silent of 36: held (A was 0 too) | a payment, `fragments`, design.md's copy identical and held, the contexts alone |
| historian | **approve** (advisory): item 1 as a refusal, item 2 as landed, item 3 as B or D | the precedents below | none run | C not preferred over A on R2: **held** | a tool that refused or reordered and gave it up for a measured cost; a SQLite page recommending `_v2` for all code |
| llm-ergonomist (blind) | **approve** B and D, **object** A and C, no veto | the specification alone | one session, 0.3635 USD | silent first tries about 40 % under A and 45 % under C on the store task: **falsified**, 0 of 18 each | every variant to approve by its own condition, under the reader test's ceiling (the critic) |
| reader test | no verdict | Part 11, metrics 2 and 4 | 288 sessions and the blind seat, 17.7277 USD | the decision rule's lines, `237-reports/reader-test.md` | R4: no arm preferred; a bare close 4 times under A, 3 under C, none under B or D |

## Disagreements

- **What to do with `CCC_OVERRIDE_OPTIONS`.** The compiler-engineer: set it to `#` for every clang.
  The ffi-pragmatist: tell it. The commands favour telling: a blanket `#` drops measurement 055's
  `#+-fuse-ld=lld` in silence. R2 takes the route that refuses only the words the class reads.
- **Counting `CPATH` against clearing it.** Not a disagreement on inspection (the critic): both seats
  count and both reject clearing; they differ only on empty elements, which neither measured and the
  critic did.
- **The reader test.** The coordinator's first reading of R1 to R3 said more than the test could
  show; the critic's second pass found the tasks named the refusal, and the reading was corrected
  under its own dated section before R4 ran.

## Prior art

Under the decision issue, from `237-reports/historian.md` § Candidate lines, each page re-read.

## Predictions to score

- The compiler-engineer's, at M-package-manager's close: `shadowed.hero` at most 260 lines by
  `layout`'s count, and the two-line program's warm build starting exactly 5 clangs.
- The ffi-pragmatist's, at the landing: the `libm` program through `CPATH` refused on this Mac and
  Linux arm64; the 20 `examples/` folders with an `extern` and the readline binding through `CPATH`
  (2051) building and printing as at `163912031`.
- The spec-warden's P1 held, P2 falsified, P3 held (here); the blind seat's falsified (here); the
  historian's held (here).
- **The payment of R6 and R7**, at the landing: R4 run again on the landed specification binds no close bare under fence B and reads `lent_shape` first in at most 19 of its 72 first tries; unrun or falsified, the removal R6 names pays.

## What the conservative resolution would have been, so the author can choose it

- **R2**: the environment ruled the user's own trust boundary, the folders `CPATH` and
  `C_INCLUDE_PATH` name left as clang reads them, and design.md §4.19's `CPATH` sentence and the
  `machine_locked_path` note saying so (the ffi-pragmatist's condition for approving item 1 as
  landed); 2344 then closed as a ruling rather than a repair.
- **R6**: the fence left as it stands, A, at 0 tokens, 2322 closed on the reader test's R1 to R3.
- **R7**: defect 2347's `Fix` alone, the specification's sentence left as it is.
- **Every other item**: the conservative and the adopted resolution coincide.

## Author's verdict

**Ratified, 2026-11-08, about 13:57**, by the author's reading of the sitting's results as the coordinator set them out in the conversation (R1 to R9, the measurements each rests on, the case `sibutil-angled` that R2's group route refuses, the conservative alternatives and the costs), in the author's words, meant as: *ok, I ratify, and ok to 10 dollars of extra spending*. Asked by decision 2349 (https://github.com/heroes-lang/heroes/issues/2349). The yes settles R1 to R9 as written; the landing owes R9's list before M-package-manager's tag. **The extra 10 USD** are the author's yes to paid runs beyond the sitting's cap of 25: R6's payment, R4 run again on the landed specification (about 4.3 USD), is allowed within them.

## Process notes

- The seats worked in detached worktrees under `.claude/worktrees/237-<seat>/`, not the scratchpad
  the `/panel` skill names, as panels 235 and 236 did: the contract forbids a file outside the
  repository's root.
- The blind seat and the reader test ran from folders under lane `p237`'s `build/` (ignored by git),
  with `--bare --restricted --safe-mode --strict-mcp-config`, git blind (`GIT_CEILING_DIRECTORIES`)
  and the key loaded by the runner, never printed. **The blind seat's own `context` section says the
  working-directory path reached it**, and that path names the repository and the lane; the
  coordinator's header to its report first said nothing of the project reached it, which the critic's
  second pass corrected. Its reading is kept, the path named here.
- The blind seat answered that it had no tool to write files, as panel 235's did, though `--tools`
  and `--allowedTools` named `Write`; why is unrun.
- R3 and R4 were registered in `blind/README.md` and committed (`15a9b28d7`, `62a95b20a`) before any of
  their sessions ran; R1's and R2's were running when R3 was written, and none of their results was
  read before.
- The coordinator wrote in the reader test's first report that 51 of R1's programs bound `_v2` and 21
  marked the close `when 0`, reading the runner's label column; all 72 mark it, 51 binding `_v2`
  beside it. Corrected under its own section.
- The historian wrote one stray file, `historian.argument.tmp`, in its worktree; the coordinator read
  and removed it before copying the report.
