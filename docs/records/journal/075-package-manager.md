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

@@CLOSE@@
