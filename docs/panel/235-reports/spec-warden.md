# Panel 235, the spec-warden's report

2026-11-07. Worktree `.claude/worktrees/235-spec-warden/`, detached at
`3b071e4f12f2d3357ca2bd3bc338464dc54aa655`, parents `328524be86e7b667611610c183cde217063e86d8`
and `9791d4787d4e63e5b7ded2c7c7cf1cf66347b2f7`, tree
`f4dbb2b8db0c072de8a01cc6558307af4a4b89e1` (`git -C <worktree> log -1
--format='%H %P %T'`, run before anything else).

## 1. The predictions, registered before any reader session

Registered after reading, in the worktree and in this order, my brief, `00-shared.md`,
`proposal.md`, `probe/README.md`, `blind/README.md` and
`docs/panel/235-reports/critic-briefs.md`, and **nothing else**: no section,
task, package, template, `dry-run.txt`, panel 234 result or score was opened
before this section was saved, and no command but the `git log` above was run.
This section is not changed afterwards; what is learnt later goes in later
sections.

**The cells.** Per arm and per model, over the arm's three tasks, the count
right out of 15 first turns at pass@1 (the first turn passes) and pass@2 (the
first turn passes, or its one repair turn does), as `blind/README.md` § The
cells defines them. Each cell is a point prediction and, in brackets, the
floor and ceiling I mean as an 80% interval. Where `--samples` cuts a cell, or
sessions are void or absent, the cell is read as its share times 15.

| arm | Haiku pass@1 | Haiku pass@2 | Sonnet pass@1 | Sonnet pass@2 |
|---|---|---|---|---|
| U-angle | 8 [4, 12] | 11 [7, 14] | 12 [9, 15] | 14 [11, 15] |
| U-bare | 7 [3, 11] | 10 [6, 14] | 11 [8, 14] | 13 [10, 15] |
| U-pkg | 8 [4, 12] | 11 [7, 14] | 12 [9, 15] | 14 [11, 15] |
| F-yaml | 6 [2, 10] | 9 [5, 13] | 10 [7, 13] | 13 [10, 15] |
| F-toml | 8 [4, 12] | 11 [7, 14] | 12 [9, 15] | 14 [11, 15] |
| F-json | 8 [4, 12] | 11 [7, 14] | 12 [9, 15] | 14 [11, 15] |
| F-lines | 8 [4, 12] | 11 [7, 14] | 12 [9, 15] | 14 [11, 15] |
| F-hero | 7 [3, 11] | 10 [6, 14] | 11 [8, 14] | 14 [11, 15] |

**Why these numbers, said before the data so it can be wrong.** The program a
reader writes is the same Heroes in every arm but its `use` lines, so I expect
the program, not the catalogue, to carry most of the failures, and the arms to
sit within noise of one another. Two catalogue-side hazards are the exceptions
I register: **F-yaml on T2**, where the section's example writes plain scalars
and ben's tag `1.10` read plain is the number 1.1 (about two to three first
turns lost per model, and a repair turn that then meets the program's own
errors, which the refused catalogue had hidden); and **U-bare on T3**, where the
entry must not be named `geom` (about one first turn per model). F-hero is set
one lower than the TOML level as a novel format, a weak expectation. The level
itself (Sonnet near 80% at the first turn, Haiku near 53%) is a guess from no
measurement of these tasks; it is the part of this table I hold least firmly,
and the rule below scores it apart from the structure.

**The rule by which the set is judged**, each part reported separately:

- **R1, level**: the mean absolute error over the 32 cells, predicted count
  against observed share × 15, is **at most 2.5**.
- **R2, calibration**: the observed value falls inside the stated floor and
  ceiling in **at least 24 of the 32 cells**; fewer means the intervals were
  too confident.
- **R3, structure**, six claims, each holding or not:
  - S1: Sonnet's pass@1 is at least Haiku's in at least 7 of the 8 arms;
  - S2: the gain from pass@1 to pass@2, pooled over the eight arms, is larger
    for Haiku than for Sonnet;
  - S3: `score` does not flag the A/A pair (U-angle against F-toml: no one of
    its four one-sided p-values below 0.05);
  - S4: question U finds no control significantly above or below U-angle after
    Holm, so *the author's form stands as measured* is what `score` prints;
  - S5: F-yaml's pooled pass@1 (both models) is the lowest of the five F arms,
    a tie for lowest holding;
  - S6: no F arm other than F-yaml is found significantly below another after
    Holm.
- **The set holds** where R1 and R2 hold and at least 4 of the 6 claims of R3
  hold; otherwise it is falsified, and this report's later section names which
  part failed. A level miss with the structure held (R1 failing, R3 holding)
  is read as *the base rate was wrong, the arm differences were not*, and the
  reverse as the more serious failure for this seat, since the structure is
  what a removal or a registered prediction pays tokens with (panel 012).

## 2. The briefs' facts I checked, and none was false

Every one below was run in my worktree after section 1 was saved; none of the
facts the briefs state that I re-ran turned out false.

- The ceiling: `grep -n '16384' docs/design.md` prints line 255, §1.6, *must
  fit in 16384 tokens, measured by `claude-opus-5` through `POST
  /v1/messages/count_tokens`*. The 1,500-token rule of the consolidation block
  does not bind this row: it covers rows 66 to 74 (M-buildable-structs to
  M-compatibility-promise) *before the thesis harness has run*, and `grep -n
  'M-thesis-harness' ROADMAP.md` gives row 70, done on 2026-10-24; this
  milestone is row 77.
- The base: `git log -1 --format='%H %P'` gives first parent `328524be8`, which
  `git -C .claude/worktrees/pm-opening log --oneline -1` names as the lane's
  head, and second parent `9791d4787`, core-b1.
- The compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
  (real 9.11 s), `./heroes --version`: `heroes 0.6.0`, `sources
  1a1e340778db789a`, the probe's.
- The specification: `shasum -a 256 spec/heroes-spec.md` gives `0d88388a…f923a`,
  the README's; `./heroes measure spec/heroes-spec.md` gives 7876 legacy, 8019
  cl100k, as the probe says; `--refresh` gave **10,514 real today** (§ 3), the
  probe's figure from 2026-10-30, unchanged.
- The seven sections: each one's sha256 matches its line in
  `blind/manifest.sha256` (lines 128 to 134), and `./heroes measure` on each
  one alone gives the README's vendored figures exactly (504/514 to 544/556).
- `heroes add` stands at `docs/design.md:897-898` (`grep -n 'heroes add'`), in
  §3.5, and `cli-surface.md` still cites `design.md:772` (§ 7 below repairs
  that).

## 3. What each variant costs the specification

**The method.** Each arm's specification is built the way the runner builds it
(`session_spec` in `blind235.py:1810`: the spec, its trailing newlines cut, a
blank line, the section). The vendored count is `./heroes measure` on that
file. **The real count is the instrument §1.6 names, applied to the real
path and reverted**, the route `.claude/rules/spec-shape.md` § How a change to
the document is made gives, since `--refresh` refuses every path but
`spec/heroes-spec.md` and `CLAUDE.md`: each arm's file copied over
`spec/heroes-spec.md` in my worktree, `./heroes measure spec/heroes-spec.md
--refresh`, the original copied back, its sha256 `0d88388a…f923a` checked
after the last run and `git status --short` empty. The delta is the arm's
count less the base's from the same instrument on the same day.

| arm | section | vendored, section alone (legacy / cl100k) | vendored delta on the spec (legacy / cl100k) | **real, the spec with it** | **real delta** | against the ceiling (real + 60 FFI floor) |
|---|---|---|---|---|---|---|
| none (today) | — | — | — | **10,514** | — | 10,574 of 16,384 |
| U-angle, F-toml | `angle-toml.md` | 504 / 514 | +505 / +515 | **11,184** | **+670** | 11,244 |
| U-bare | `bare-toml.md` | 530 / 542 | +531 / +543 | **11,207** | **+693** | 11,267 |
| U-pkg | `pkg-toml.md` | 544 / 556 | +545 / +557 | **11,237** | **+723** | 11,297 |
| F-yaml | `angle-yaml.md` | 488 / 506 | +489 / +507 | **11,179** | **+665** | 11,239 |
| F-json | `angle-json.md` | 519 / 540 | +520 / +541 | **11,209** | **+695** | 11,269 |
| F-lines | `angle-lines.md` | 496 / 507 | +497 / +508 | **11,176** | **+662** | 11,236 |
| F-hero | `angle-hero.md` | 514 / 532 | +515 / +533 | **11,205** | **+691** | 11,265 |
| **merged-min** (mine, below) | § 1 edited, no § 14 | — | +93 / +97 | **10,650** | **+136** | 10,710 |

**No arm comes near the ceiling**: the largest, U-pkg, leaves 5,087 real
tokens of headroom (16,384 − 11,297). **No veto on the budget.**

**What the differences between arms are worth under §1.2.** The F arms span
33 real tokens (F-lines 662 to F-json 695), the U arms 53 (U-angle 670 to
U-pkg 723). design.md §1.6 cites measurement 052's round trip, median 3,619
tokens on Sonnet 5.5 strict (`docs/measurements/052-…md:197`, read, not
re-run). So 33 tokens are worth 0.9 points of first-turn rate (33 / 3,619),
and 53 worth 1.5 points; the reader test resolves about 17 points at its best
(30/30 against 25/30, one-sided p 0.026, as its README says). **Tokens cannot
choose between these arms; only a tie in the test lets them break it**, and
then by less than one point of rewrite rate.

**What the test's section is not: the landed text.** Two things move the price
in opposite directions, neither measured by the test:

1. **What the section carries that the specification may not need**: the
   catalogue's format, its two-entry example with two 64-digit sums (77 / 75
   vendored for the sums alone, the README's figure), and the entry rules. I
   wrote the smallest robust alternative without them and priced it,
   `merged-min`: § 1's third bullet and its `Use` production edited
   in place (spec-shape.md: *merging beats appending*, and § 1 is the home of
   `use`), saying that `use <json>/parse` reaches the package's module, that
   the catalogue `heroes.toml` beside the compiled file names it with its
   source, exact version and fingerprint, that **`heroes add json` writes that
   entry**, and that a program with no package has no catalogue. It costs
   **+136 real** (+93 / +97 vendored), against +662 to +723 for the sections:
   **526 to 587 real tokens less**, 79 to 81% of each section's price. Its
   text is
   `.claude/worktrees/235-spec-warden-scratch/arms/merged-min.md`, lines 29 to
   35 and 45, diffed against the spec above.
2. **What the landed text needs that no section carries.** No section names the
   verb that writes an entry, a dependency's own `use` lines starting from its
   own root (question 3), one version per program (question 6), a local folder
   as a source (question 5) or `internal/` (question 12). **The fingerprint
   makes the verb a necessity, not a convenience**: every task gives its sum,
   and a model writing a new entry by hand in real use has no sum to write
   unless it invents one, which the arms refuse (`sum_mismatch`, fail-closed,
   robust and a round trip). So the test measures a hand holding a given sum,
   and the landed specification must name the verb whatever format wins.

**What pays, per variant** (panel 012, as panel 046 holds the two payments):

- **No removal of existing text is owed, and I name none**: nothing in today's
  specification is made redundant by any arm (§ 1's sentence *Every `use`
  starts at the directory of the file you compile* is amended, not removed),
  and 5,087 tokens of headroom do not oblige one. What is owed is the smallest
  addition that is robust, and the format paragraph is the part whose place in
  the prompt the sitting has not shown: a model that writes no catalogue by hand
  still reads it on every session, a program with no package included.
- **Registered predictions, each naming an instrument that exists today**:
  - **W1, the price at landing**: the commit that lands the ruling, priced by
    `heroes measure spec/heroes-spec.md --refresh`, adds **at most +250 real**
    if it carries the `use` form, the verb, the dependency's own root and one
    version per program without a format example, and **at most +800 real** if
    it carries a format and its example besides. Scored in that commit,
    M-package-manager.
  - **W2, the reader test**: section 1's cells and rule, scored by `score`.
  - **W3, the mistakes as errors**: at M-package-manager's close the `check`
    golden form holds a case, annotated, for each of: the brackets forgotten
    (`use json/parse` with an entry `json`), added (`use <geom>/area` for the
    program's own module), the whole path inside them (`use <json/parse>`), the
    domain (`use github.com/acme/json`, `use <github.com/acme/json>/parse`) and
    `import`; **every one exits 1 with a `certain` fix and none compiles**.
  - **W4, question 17's falsifier** (§ 6): at the same close, a case for the
    catalogue deleted, an entry deleted, each field of an entry mistyped and a
    fetched copy left after its entry is deleted; **each exits 1, none compiles**.

## 4. Principle 0: which forms are the language's, and what would pay for each

Measured first: **the compiler needs no fetched code**. `grep -rh '^use core/'
selfhost | wc -l` reads 0, and `ls -d selfhost/core` finds no folder; the page
says the second version's core packages are *copied into the compiler's own
tree*, so building the compiler fetches nothing. No form below is on the
closure list (design.md §1.0); each enters on the thesis or waits.

| form | a form of the language? | what would pay for it | what would not |
|---|---|---|---|
| `use <name>/…` (U-angle and the five F arms) | **Yes**: a new alternative in § 1's `Use` production; `<` after `use` is refused today (`expected_module_name`, the probe) | the author's position 3 and 7, and the thesis: with fetched code placed where no bracket-less `use` reaches (§ 6), forgetting or adding the brackets becomes a compile error with a `certain` fix (W3), which is the language's claim; and `score` printing *the author's form stands as measured* | a control significantly above it by the registered rule, which goes to the author as a measured case |
| `pkg` reserved as a `use`'s first name (U-pkg) | **Yes**: a reserved first element changes what an existing production means. It refuses no tracked program: `git ls-files \| grep -cE '(^\|/)pkg/'` reads 0 | only a significant lead over U-angle, and then as a case put to the author | a tie: it costs 53 real tokens more than U-angle and one reserved word, and buys nothing a tie can show |
| an unmarked `use` resolved by the catalogue first (U-bare) | **No production, a new meaning** of an existing form, and a non-local one (design.md §1.3): what `use json/parse` reaches depends on another file | a significant lead over U-angle, then a case to the author; and the locality veto is the blind seat's | it fails question 17's purpose wherever fetched code sits in the tree (§ 6, measured) |
| a catalogue in Heroes' own syntax (F-hero) | **A second grammar** in the specification's notation, read by the tool but not a `.hero` module: about three productions (`Catalogue = { Dependency } .`, `Dependency = "dependency" ident NEWLINE INDENT { Field } DEDENT .`, `Field = ident ":" string NEWLINE .`, my draft) and a parser | a significant lead over every other F arm at pass@1 | a tie: it adds productions and a parser where TOML, JSON or a line format adds none to the language |
| TOML, YAML, JSON | **Files outside the language**: the specification shows them by example and the reader brings their grammar from elsewhere | the test, then hazards and parser size | — |
| a line format | **Outside the language**, a grammar of the project's own: about three productions (`Catalogue = { Line } .`, `Line = [ Entry ] [ comment ] NEWLINE .`, `Entry = "dependency" name ( "index" version \| "git" address tag ) sum .`, my draft) | as above | — |

**What the test cannot pay for**: the section's place in the prompt. It
compares forms given a section; it never runs a specification without the
format paragraph, so it cannot show that the paragraph earns its 500-odd tokens
over the merged form plus the verb and the diagnostics' text (design.md §4.17,
*a diagnostic carries everything needed to fix the mistake*). That stays a
question; W1 prices both, and only an arm the test never ran could measure it.

## 5. CLAUDE.md § 2 and the author's word

**The lines that declare a language version, a toolchain or an edition**, read
from the survey's Table 1 (its cells are documentation read on 2026-11-07,
nothing run, as it says itself):

| file | line | what it declares | does it change a program's meaning? |
|---|---|---|---|
| `go.mod` | `go` | a language version; *a mandatory requirement* since Go 1.21; *if the `go` directive is missing, `go 1.16` is assumed* | **Yes**: behaviour is gated on it at 1.14, 1.16, 1.17, 1.21 and 1.27 (Table 1A, *go-gated behaviour*). It is a mode |
| `go.mod` | `toolchain` | a suggested toolchain | No, it chooses which compiler runs |
| `go.mod` | `godebug` | not a version, a setting, and Go 1.27 refuses old values of removed ones | Yes, a setting is a mode |
| `Cargo.toml` | `edition` | an edition; absent means 2015 | **Yes**, a mode by definition |
| `Cargo.toml` | `rust-version` | the oldest toolchain supported | No, it refuses an older toolchain |
| `Cargo.toml` | `resolver` | the resolver, its default tied to `edition` | Yes, a mode of resolution |
| `Cargo.toml` | `cargo-features` | nightly features | Yes, a mode |
| `Cargo.lock` | `version` | the lock file's own format | No: a format version of a file, not of the language |

**Heroes needs none of them in this milestone**, and § 2 should stand as
written. The reasons, each a fact read or run: (a) the promise of `1.0.0`
(M-compatibility-promise's page, panel 232's R6) is *a program that compiles
under Heroes 1.0.0 compiles under every later 1.x compiler*, so no 1.x
release ever needs an edition, and before `1.0.0` § 2 says a refusing change
is *told, never undone*; (b) the arms pin every dependency by exact version and
fingerprint, so what a program builds is fixed by its catalogue and the
compiler is the only moving part; (c) the one service a version line gives
that nothing else does, refusing a package **by name** when the compiler is
older than the one it was written for, is unmeasured here: no Heroes package is
published, and the index (row 78) could carry the compiler each version was
published with, without a line in any file a program carries. A git source has
no such metadata, which is where the question would return.

**The sentence § 2 would gain if the sitting keeps one**, written so that it
admits a refusal and never a meaning:

> **One line is excepted** (panel 235, on the author's word of 2026-11-07, meant
> as: *if it is needed, let us relax this rule*): a package may name the oldest
> compiler it builds with, so an older compiler refuses it by that name; no
> line chooses what a program means, and a line that would is refused.

**Its falsifier**: one program whose output, or whose compiling at all on a
compiler at or above the named one, changes with that line.

## 6. Question 17: which spellings and which placements meet panel 056's purpose

The purpose, design.md §3.5 and panel 056's llm-ergonomist (`docs/panel/056-…md:50-54`):
*nothing can notice the file went missing or was mistyped* is what the second
condition refused. Asked of a catalogue: **deleting or mistyping an entry never
changes which module compiles, only whether the program builds.**

**Measured today, the placement half** (`.claude/worktrees/235-spec-warden-scratch/q17/`,
a `main.hero` and a module `go() -> str` in each folder, `./heroes check
main.hero`):

| the `use` line | the folder in the program's tree | exit |
|---|---|---|
| `use json/parse` | `json/parse.hero` | **0**, compiles |
| `use build/parse` | `build/parse.hero` (the name of the compiler's per-project cache) | **0**, compiles |
| `use core/strings/cut` | `core/strings/cut.hero` (panel 234's R2 placement) | **0**, compiles |
| `use json-1/parse` | — | 1, `expected_declaration` at the `-` |
| `use json@1.4.2/parse` | — | 1, `expected_declaration` at the `@` |
| `use .heroes/json/parse` | — | 1, `module_path_climbs` |
| `use <json>/parse` | — | 1, `expected_module_name` |

So **any folder of the program's tree whose path is a run of identifiers is a
module a bracket-less `use` reaches**, the compiler's own `build/` included; a
path holding `-`, `@` or opening with `.` is one no `use` can spell.

**Which spellings meet it, and on what conditions:**

- **U-angle** meets it, on three conditions: (1) **no fallback either way**, a
  bracketed `use` resolving through the catalogue alone and a bare one through
  the tree alone; (2) **fetched code lives where no bare `use` can spell it**,
  outside the program's tree (a cache) or under a first element no identifier
  makes; and (3) **every source is checked against a fingerprint**, so that a
  mistyped version or tag fetches other bytes and is refused. Then a deleted or
  misnamed entry is `unknown_entry`, a deleted catalogue the same for every
  bracketed line, a mistyped field a fetch or sum failure: each changes only
  whether the program builds.
- **U-pkg** meets it on the same three conditions, the second sharpened: `pkg/`
  must be a namespace the compiler resolves through the catalogue only, never a
  folder at `<root>/pkg/`, or a stale copy compiles after its entry is deleted.
- **U-bare** meets it only where fetched code sits outside the tree. With the
  copy in the tree, as the grader places it at `json/` and as R2 places core,
  deleting the entry leaves `use json/parse` compiling the copy as the
  program's own module, unpinned and unchecked, measured in the first row
  above: **the purpose fails**.
- **Panel 234's R2, as written, fails it under every spelling**: its copies stand
  at `<program>/core/` and are reached by `use core/strings` (41 such lines in
  `examples/`, `grep -rh --include='*.hero' '^use core/' examples | wc -l`; 0 in
  `v0.6.0`'s examples, `git grep -h '^use core/' v0.6.0 -- examples | wc -l`),
  and R2 says *held so by a check of the net until `heroes fetch` places
  them*. If `heroes fetch` places a pinned package there, a bare `use` compiles
  it whatever the catalogue says. Either `core` is an entry, reached as
  `use <core>/strings` with its code out of the tree's reach, the 41 lines
  rewritten with no old spelling kept (CLAUDE.md § 2), or the copy is the
  program's own code, which nothing fetches or pins. Question 11 rules which;
  this is why it cannot be both.
- **A local folder as a source** (question 5) has no fingerprint by its nature:
  a mistyped path to another folder holding modules of the same names changes
  which module compiles. **A question, not a measurement**: what a local entry
  is checked against so that a mistyping is told.

## 7. The three-inputs rule, rewritten

**Where the rule is written, enumerated from the tree** (`git grep -l 'no fourth
slot'` outside `docs/panel/`, and `git grep -n 'fourth input'` in `CLAUDE.md`,
`docs/design.md` and `.claude/`): `.claude/rules/cli-surface.md`, its home;
`docs/design.md` §3.5; **`CLAUDE.md` § 10**, line 238, *the refusal of a
fourth input class are in `.claude/rules/cli-surface.md`*, a third place the
brief does not name; `docs/roadmap/milestones/M-package-manager.md`, which
cites the heading; and `docs/records/contract/case-law.md`'s CL-016, a record,
corrected underneath and never rewritten. So the commit touches the contract,
whose count is re-measured with it (CLAUDE.md's preamble).

**`.claude/rules/cli-surface.md`**, the section replaced, its heading with it
and the living citation of it in the milestone's page rewritten in the same
change (`.claude/rules/records.md` § A tree of entries):

> ## Four input classes: the catalogue is the fourth, and there is no fifth
>
> The shapes above answer *what does this capability do to the inputs the tool
> already has*: a `.hero` file, argv, the machine's environment, and **one
> per-project file, the program's catalogue**, beside the file compiled,
> holding the dependencies a `use <name>/…` reaches and nothing else (spec § 1).
> It is admitted by the author's call (panel 235, 2026-11-07, recorded
> `retro-record`), not by panel 056's three return conditions: its entries
> repeat the names the `use` lines write, so the second condition's letter
> cannot hold, and what is kept is that condition's purpose, as the falsifier
> below. **A program with no `use <name>/…` has no catalogue and reads none.**
> The catalogue holds, per entry, its source, its exact version or tag and its
> fingerprint; a compiler flag, a search path, a library or package of spec
> § 13, a language version, a mode or the program's own name is refused by
> the reader, never ignored.
>
> **The falsifier**: deleting the catalogue, deleting one of its entries, or
> mistyping any field of one never changes which module compiles, only whether
> the program builds, and a fetched copy left behind after its entry is gone
> compiles nothing. The `check` golden form holds one case for each; one
> program that compiles another module after such an edit returns the
> admission to a sitting.
>
> **A fifth input class is refused** on panel 056's three conditions, its only
> amendment path: a second per-project file beside the catalogue, a lock, a
> workspace or a configuration, is a fifth class.
>
> `heroes add` and `heroes fetch` are subcommands when they arrive (design.md
> §3.5, cited by its section, CL-037).

**`docs/design.md` §3.5**, the italic paragraph replaced:

> ***A per-project file is admitted once, as the program's catalogue*** (panel
> 235, the author's call of 2026-11-07, recorded `retro-record`; examined and
> deliberately unplaced from panel 056, ratified 2026-08-15, until then). It
> holds the dependencies a `use <name>/…` reaches, each with its source, its
> exact version and its fingerprint, and nothing else; a program that reaches
> none has none. Panel 056's second condition, *no string in the file also in a
> `.hero` file*, cannot hold for a file whose entries the `use` lines name; its
> purpose is kept as the admission's falsifier: **deleting or mistyping an
> entry never changes which module compiles, only whether the program builds**,
> which holds only where a `use` without brackets never reaches fetched code
> and every source is checked against its fingerprint. Every other per-project
> file stays examined and unplaced, its three conditions kept on
> M-package-manager's page.

**`CLAUDE.md` § 10**, the clause at line 238: *the refusal of a fourth input
class* becomes *the fourth input class, the catalogue, and the refusal of a
fifth*.

**Which spellings and placements meet the falsifier** is § 6: U-angle and U-pkg
with fetched code out of a bare `use`'s reach, never U-bare with the copy in the
tree, and never R2's placement as written.

## 8. Question 20: what a reader must learn, counted

**How counted**: the concepts are the rules each section states, enumerated by
reading the seven sections and their `diff` against `angle-toml.md` (§ 3's
files); the words are the terms a section defines or makes a key of a file.
The vocabulary not found in today's spec was cross-checked by a script
(`python3 -I`, every code span outside a fence tested against the spec's
text): 7 code spans new to the spec in each angle section, 6 in U-bare's, 9 in
U-pkg's.

**Shared by all eight arms, 11 concepts**: a package; the catalogue, beside the
compiled file, absent without packages; one entry per package; the catalogue
holds dependencies only; an index source, known by the entry's name; an exact
version of three numbers; a git source, address and tag, its name the
program's; the sum, `sha256-` and 64 hex digits, a mismatch refused; the entry
name's letters; how a `use` reaches an entry's module, binding the last part,
`as` renaming; the program's own modules as before.

| arm | the mark | the format | concepts in all | words a file uses | productions in the spec |
|---|---|---|---|---|---|
| U-angle, F-toml | 1, the angle brackets | 2, a table header and `key = "string"` | **14** | `dependencies`, `version`, `git`, `tag`, `sum` | 1 changed (`Use`); TOML's grammar brought by the reader |
| U-bare | 2, the catalogue looked up first, the clash with an own folder an error | 2 | **15** | the same five | 0 changed |
| U-pkg | 2, the `pkg/` prefix, `pkg` reserved | 2 | **15** | the five and `pkg` | 1 changed |
| F-yaml | 1 | 2 stated, **and 1 unstated**: a plain scalar's type, which is T2's trap | **14 + 1** | the five | 1 changed; YAML's grammar brought |
| F-json | 1 | 2, nested objects, quoted keys and values | **14** | the five | 1 changed; JSON's grammar brought |
| F-lines | 1 | 3, one line per entry, two word orders, `#` lines | **15** | `dependency`, `index`, `git`, the rest by position | 1 changed, about 3 added (§ 4) |
| F-hero | 1 | 2, a declaration and indented `field: "string"`, the second § 4's | **14** | `dependency`, `version`, `git`, `tag`, `sum` | 1 changed, about 3 added (§ 4) |

**One word collides with a word the specification already uses.** `grep -n -i
package spec/heroes-spec.md` prints lines 462 and 465: § 13's `extern "raylib.h"
package "raylib"` and the `Extern` production, where `package` asks the system
for a C library. Every arm appends a § 14 titled *Packages* meaning fetched
Heroes code. That is one prompt holding two meanings of one word, which no arm
varies and the README names in its *cannot measure*. It is question 1's to
rule; I count it here as a sixteenth concept every arm carries until it is.

**Files a program must have**: **zero** with no dependency under every arm (each
section's *a program that uses none has none*); **one** with dependencies, the
catalogue, the sums inline and no lock file. Copied whole, Go brings two with a
dependency (`go.mod`, `go.sum`) and requires `go.mod`'s `module` line, Cargo
brings two (`Cargo.toml`, `Cargo.lock`) and requires `[package]`'s `name`
(the survey's Table 1, documentation, nothing run). Whether either builds a
program that has no manifest is **unrun**, a question for the historian.

## 9. Question 21, with the historian: Go against Cargo, and the habit count

**The reader test does not vary the base.** All eight arms share one semantics,
which is neither base whole: exact pins, the fingerprint inline, one file, no
identity of the program. **Both bases' one required line is refused by every
arm**: Go's `module` (*a `go.mod` file must contain exactly one `module`
directive*) and Cargo's `name` (*the only field required by Cargo*), each
counted by the grader as a habit (`HABITS`, rows *a `module` line naming the
program* and *`[package]`*). So the arms already bend both bases on a line the
author's rules do not name; the sitting owes the list of bends, each traced to
the rule that demands it (position 5: *bent only where a rule of the author's
demands it*). The catalogue being optional (Frame 1) demands that no identity
line be required; it does not demand that one be refused.

**What the habit count can say**: which ecosystem's habits, carried into an
unfamiliar file, made a first turn fail, per arm and model. **What it cannot
say**, each read from the README's own table and rule:

- **It is not comparable across ecosystems**: Go has four patterns, Cargo two,
  Python and npm one each, and the first match wins; a larger Go count may be
  more patterns, not more habit.
- **It is confounded with the format**: `[package]` can only be written as such
  in TOML, so Cargo's count comes from four of eight arms; Go's `require` reads
  in every format.
- **It counts a habit only where it failed the build and was the first
  message**; a habit that is harmless, or second, is invisible.
- **It cannot rank the bases, because a habit is a vote for copying it**: a
  model writing `[package]` under TOML is wrong only because the arms refuse
  what Cargo requires. Under Cargo copied whole that slip would be right.
  Cargo's default `version = "1.2.3"` means `^1.2.3` (the survey, Table 1B),
  so a Cargo-trained reader reads an exact pin as a range: a misreading that
  builds, which the test cannot see.

**So under the author's criterion the test measures the catalogue's syntax and
the `use` mark, never Go against Cargo.** Measuring *the one that works best for
language models* between the two needs an arm per base copied whole (its files,
its required lines, its version semantics) on the same tasks: unrun, and a
question I put to the synthesis with its size: two arms, 60 first turns, about
1.5 USD, and 3.1 with a second turn for every one, at the README's projection
(6.13 USD for 240 first turns, 12.27 with a second turn each).

## 10. The paid calls

`heroes measure --refresh` makes **three requests** to
`https://api.anthropic.com/v1/messages/count_tokens` per run (two probes of the
request's own size, then the document: `selfhost/cli/refresh.hero`, `probed`
and `refreshed`). I ran it **nine times**: the specification as it stands, the
seven sections each appended, and `merged-min`: **27 requests**, every one
answered (each run printed its count). The key was loaded by `set -a; .
/Users/joseph/Temp/heroes/heroes-lang/.env; set +a` and only its length was
printed (108). **The reckoning**: the nine documents counted 99,561 tokens net
of the requests' offset; with the offset and the probes, about 99,800 tokens
sent. Priced as input at the README's list price for Opus 5.5, 4 USD per
million (the brief gives no price for `claude-opus-5`), that is **0.40 USD**,
within the 3.00. Whether the endpoint bills at all is a question: I ran no
command that reads a bill.

## 11. Found beside the work, not filed by this seat

`use .heroes/json/parse` and `use ./json/parse` are refused as
`module_path_climbs`, *a `use` path cannot climb*, with the note *a module
above that directory belongs to a different program*; neither path climbs.
Searched first: `gh issue list --state all --search module_path_climbs` finds
open defect 2308 (a quoted `use` advised a line the compiler refuses), whose
table holds `"../x"` and not these; `grep -rln module_path_climbs
tests/golden` finds `use-has-a-path.hero`, whose climbing cases are `../token`
and `lex/..`. **Whether this is a false message, and its class, is the
coordinator's to decide**; I name it because a placement under a `.`-led
folder is one of § 6's candidates, and its message would be what a reader is
told.

## 12. The reader test ran while I worked: my predictions scored by their own rule

**Where the numbers come from.** The coordinator's `score.md`
(`.claude/worktrees/235-coordinator/scratch-repair/score.md`, 17:44) read, and
**recomputed by me** from `sealed/grades.jsonl` and `sealed/key.tsv` with a
`python3 -I` script that joins each first turn to its arm, task and model and
its one second turn to its parent: 240 first turns graded, none void, **236
right at the first turn and 237 after the repair turn**, one second turn drawn
in all (`sealed/key.log`: *repairs: 1 second turns drawn*). Every cell below
agrees with `score.md`.

| arm | Haiku pass@1 (mine) | Haiku pass@2 (mine) | Sonnet pass@1 (mine) | Sonnet pass@2 (mine) |
|---|---|---|---|---|
| U-angle | **15** (8) | **15** (11) | **15** (12) | **15** (14) |
| U-bare | **15** (7) | **15** (10) | **15** (11) | **15** (13) |
| U-pkg | **14** (8) | **14** (11) | **15** (12) | **15** (14) |
| F-yaml | **14** (6) | **14** (9) | **14** (10) | **15** (13) |
| F-toml | **15** (8) | **15** (11) | **15** (12) | **15** (14) |
| F-json | **15** (8) | **15** (11) | **15** (12) | **15** (14) |
| F-lines | **14** (8) | **14** (11) | **15** (12) | **15** (14) |
| F-hero | **15** (7) | **15** (10) | **15** (11) | **15** (14) |

**By my rule:**

- **R1, level: fails.** The absolute errors sum to 127 over 32 cells, a mean
  of **3.97** against the 2.5 I allowed (57 on Haiku's pass@1, 33 on its
  pass@2, 27 on Sonnet's pass@1, 10 on its pass@2).
- **R2, calibration: fails.** **15 of 32** observed values fall inside my
  intervals against the 24 I required: none of Haiku's pass@1, 2 of its
  pass@2, 5 of Sonnet's pass@1, all 8 of its pass@2. My intervals were too
  low, not too narrow: every miss is above its ceiling.
- **R3, structure: 5 of 6 hold**, S2 fails. S1 (Sonnet at least Haiku at
  pass@1, 8 of 8), S3 (the A/A pair, every p 1.0000), S4 (*the author's form
  stands as measured*, every Holm p 1.0000), S5 (F-yaml lowest of the F arms,
  28 of 30) and S6 (no F pair significant) hold. S2 fails: Haiku gained 0 from
  pass@1 to pass@2, Sonnet 1.
- **The set is falsified**, on level and calibration. And **the structure that
  held is mostly the ceiling's**: with 236 of 240 right, S1, S3, S4 and S6
  could hardly fail. **The two mechanisms I registered both failed**: the YAML
  trap fired in **0 of 10** F-yaml T2 first turns, each of which wrote `tag:
  "1.10"` quoted (read from the ten sessions' result text in
  `235-base/build/b235-out/`, 8 of them leaving `v1.2.0` plain beside it); and
  U-bare's T3 lost nothing, 10 of 10. S5 held on other causes: F-yaml's two
  first-turn misses are a Haiku `silent` on T3 and a Sonnet `cannot_infer`, a
  compiler code, on T2.

**What the result says for my questions:**

- **Principle 0.** At 30, 30 and 29 of 30, the U question cannot show a lead,
  so **no control pays for itself**: U-pkg's reserved word and U-bare's
  catalogue-first lookup wait (§ 4). U-angle stands as measured, and what pays
  for it is the author's position and W3, the goldens. F-hero's grammar has no
  lead to pay for its productions and parser.
- **The formats are tied, so `score` sends them to the other measures**, of
  which tokens are mine: F-lines 662, F-yaml 665, F-toml 670, F-hero 691,
  F-json 695 real (§ 3), a spread of 33 tokens, **0.9 points of rewrite rate**
  at 3,619 tokens a round trip. Tokens cannot carry that choice either; it
  goes to hazards and parser size, on robustness first (CLAUDE.md §
  Precedence).
- **The test is at its ceiling on these tasks**, so a second run of the same
  shape on two bases copied whole (§ 9) would likely tie too. What could part
  the forms and the bases is a task the arms did not set: an entry written
  without its sum, a dependency with dependencies, two packages asking for two
  versions of a third. **A question for the synthesis, not a run I propose
  here.**
- **Three of the four misses are one shape, outside every form**: the three
  `silent` first turns, all Haiku, each print `print(a, b, "\n")` and so write a
  blank line after every line (read from sessions 048, 135 and 235's result
  text). A plausible mistake that compiles and prints wrong, 3 of 240, is the
  thesis's business; I name it for the llm-ergonomist and the coordinator and
  file nothing.

## 13. Verdict

- **verdict**: **object**, not veto. Counts measured on the binding
  instrument, so not provisional on tokens.
- **section**: design.md §1.6 (the budget and the payment rule), §1.2 (real
  cost), §1.0 (Principle 0), §3.5 (panel 056's purpose).
- **spec_token_delta**: measured, `claude-opus-5` by `--refresh`, base 10,514:
  the arms **+662 to +723** (U-pkg's 11,237, with the FFI floor 11,297 of
  16,384); `merged-min` **+136**.
- **removal**: nothing of today's text, and none is owed at 5,087 tokens of
  headroom; the candidate is inside the addition, the catalogue's format and
  example, **526 to 587 real**, which no measurement shows earns its place in
  every session's prompt over the verb and the diagnostics.
- **needed_for_self_hosting**: **no**: 0 `use core/` lines in `selfhost/`, no
  `selfhost/core`, and the second version copies its core packages into the
  compiler's tree.
- **argument**: the reader test sits at its ceiling, 236 of 240, so it lets the
  author's form stand and pays for no other new form. What it priced is not
  what lands: no section names the verb a fingerprint makes necessary, nor a
  dependency's own root, one version per program or a local source, so the
  landed price is unrun. And panel 056's purpose fails today wherever fetched
  code sits in the tree: a bare `use` reaches `json/`, `build/` and R2's `core/`
  (exit 0, measured), so a deleted entry still compiles its copy.
- **prediction**: W1, the landing commit at most +250 real without a format
  example and at most +800 with one, by `--refresh`, in M-package-manager; W3
  and W4, every listed mistake and every deleted or mistyped entry exits 1 in
  the `check` form, none compiling, at M-package-manager's close.
- **condition**: **approve** when (a) fetched code is ruled out of every bare
  `use`'s reach, R2's `core/` included; (b) the landed text names the verb and
  is priced on `--refresh` within W1; (c) § 2 stands, or gains only § 5's
  refusal-only sentence. **Veto** on Principle 0 if `pkg`'s reservation, a
  catalogue-first lookup or a catalogue grammar of Heroes' own enters without a
  measured lead; on the budget only past 16,384 real.

| seat | verdict | design.md | cost | prediction, and the milestone that scores it | condition that changes it |
|---|---|---|---|---|---|
| spec-warden | **object** (no veto; tokens measured) | §1.6, §1.2, §1.0, §3.5 | arms +662 to +723 real (largest 11,297 with the floor, of 16,384); `merged-min` +136; 27 `count_tokens` requests, about 0.40 USD reckoned | W1 at the landing commit; W3 and W4 at M-package-manager's close | approve on (a) fetched code out of every bare `use`'s reach, R2's `core/` included, (b) the verb in the landed text priced within W1, (c) § 2 unchanged or only § 5's sentence; veto if a form beyond U-angle enters with no measured lead |

report complete
