# Panel 250, spec-warden's report

Written as the sitting went, 2026-11-14 23:50 to 2026-11-15, in my own copy
`.claude/worktrees/p250-scratch/250-spec-warden/` (`tree/` a `git archive` of `48f53b343`; every
file named below without a root is in that folder). No paid run, no API call, no GitHub write.

**Said first, as 00-shared.md asks.** (1) **The route I would adopt changes no program, no
catalogue, no `use` line, no message a reader acts on, no help line and no sentence of the spec**:
placement left whole (W), with R2's ruler either kept and its unit stated (rU) or counted by the
modules the root reaches (rM2), both on design.md alone. (2) **Under M, G and H a program's reader
meets a new refusal, measured below**: a correct `use <core>/strconv` written after the fetch,
which builds today, is refused until `heroes fetch` runs again; and **the spec's own route may
reach it**, `heroes add` before the `use` line, which is the compiler-engineer's to build and run.
(3) **The brief is wrong in one place**: `00-shared.md:87-88` says `.claude/worktrees/p250`
*"still stands at `722ae76bf`"*; `git -C .claude/worktrees/p250 log --oneline -1` printed
`48f53b343 Batch 37 round: merge lane b37-check, feature 2299 and defect 2532` at 23:51. (4) **The
brief undercounts the messages M makes false**: there are more than two (§ Q1).

## The verdict, in my definition's structure

- `verdict`: **object, provisional**, to M, G and H as the sitting's resolution, and so to p1 as
  briefed; **veto, provisional, to p2** (F2), whose burden under Principle 0 is unmet; **approve,
  provisional, p0 under W**, with **rU** (+4 vendored on design.md) where the ruler stays, or **rM2**
  (+9 / +10) where the sitting moves it. Every count is vendored, a lower bound.
- `section`: design.md §1.2 (190: *"real cost = program tokens × (1 + rewrite rate)"*), §1.6
  (253-330, the budget and the payment rule at 324-330), §1.0 (112-135, Principle zero's burden),
  §1.7 (425) and §1.12 (620) for the routes outside the spec; panel 244's R1 for the spec's
  sentences.
- `spec_token_delta`: x and p0 **8483 / 8634 → unchanged**; p1 **+29 / +29** (8512 / 8663); p1r
  **+30 / +30**; p2 **+29 / +29**; p2's sentence alone +7 / +7. Vendored (claude-legacy /
  cl100k_base), **a lower bound**; real **unmeasured** (no `--refresh` named), about +38 real for
  p1 at the base's ratio, an inference, unrun.
- `removal`: for p1 or p1r, **nothing, and that is a problem**: § 1 holds no sentence that M makes
  removable. For rU and rM2, none is owed: design.md has no ceiling (the brief's search, carried; my
  `measure` of it prints *"No ceiling judges"*).
- `needed_for_self_hosting`: **no**. The compiler builds itself today with no package; the sitting's
  case is feature 2315's acceptance, a process condition (CLAUDE.md § Precedence rank 5).
- `argument`: M makes a correct program refused until a tool step runs, in the edit an assistant
  makes most: adding a `use`. Under §1.2 each such refusal is a round trip of 500 to 2000 tokens, and
  p1 (+29 vendored per prompt) only turns it into a planned `fetch`. What M buys is a smaller count
  for one program, the compiler, under panel 242's ruler, and rM2 gives the compiler the same count
  (368 reached lines, against M's 368 plus a 30-line listing) with no reader touched. Panel 242
  measured the reached set alone, route (C), still growing the tree: +215 lines at 0.2.0. If it
  still grows here, M buys 2315 nothing. F2 changes the catalogue's format, which nothing needs,
  since F1 checks the same files.
- `prediction`: (a) counted either way, by M's placed files or by rM2's reached modules, **the
  compiler's step taking `core` 0.5.0's `strings` leaves its tree larger than before at
  `48f53b343`**, as route (C) left it +215 lines at 0.2.0: scored by the compiler-engineer's `git
  diff --shortstat -- selfhost` plus `wc -l` of what is placed, in this sitting. (b) Under a built M,
  **17 of the 18 programs** (every program but the driver, which reaches all 23 modules) have a face
  of `core` they do not reach, and adding a `use` of it is refused by `check`, 17 of 17, against 0 of
  17 under W: scored by `heroes check` on M's build.
- `condition`: the compiler-engineer's count at this base showing the reached-grain step
  **shrinking** the compiler's tree, while W's grows. That would give M a case on its ruler, and I
  would move to object-to-p0, owing p1r (+30) and its blind reading. The blind reading measuring that
  readers of p0 do predict the refusal would drop p1r. A program that needs a per-module `sum` that
  F1 cannot verify would lift the veto on p2.

## Where I stand, and what was run

- The copy: `git -C .claude/worktrees/p250 archive 48f53b3437c6cd46aba5923f4f4780ac7b347283 | tar -x
  -C .../250-spec-warden/tree`. The seed, `clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes`, exit 0 (`seed.log`). Stage 2, `./heroes build selfhost/main.hero -o heroes-next`, exit 0,
  919,050,350,061 instructions retired (`/usr/bin/time -l`, `stage2.log`). Its `real 160.12` against
  `user 117.77` + `sys 27.24` is no measurement while other sessions run (CL-025).
  `./heroes-next --version`: `heroes 0.6.0`, **`sources 5bac791363384945`**, so on the base.
  `doctor`: every line `ok`, Apple clang 21.0.0.
- design.md reached by grep at the start: `grep -n '^### 1.6' docs/design.md` 253, whose first
  sentence reads **16384** tokens *"measured by `claude-opus-5` through `POST
  /v1/messages/count_tokens`"* (255-256). `'^### 1.0'` 112, `'^### 1.1 '` 174, `'^### 1.2'` 190,
  `'^### 1.7'` 425, `'^### 1.11'` 482, `'^### 1.12'` 620, `'^### 3.5'` 900. `grep -n payment`: 282,
  301, 324, 328, 330, 4053. All as the brief says.
- No process of mine left: `pgrep -f 250-spec-warden` printed nothing after the probes.

## The counts, measured in my tree

`./heroes-next measure spec/heroes-spec.md`, exit 0 (`measure/spec.txt`): claude-legacy **8483**,
cl100k_base **8634**, maximum 8634 *"a lower bound, not the reader's tokeniser"*, spread 151, real
**11303** *"claude-opus-5, 2026-11-14 — the binding number"*, headroom 5081 against 16384, the FFI
floor's 60 making 11363 what is held to the ceiling. `measure CLAUDE.md`, exit 0: 6236 / 6347, real
8359 (*2026-10-29*), against 12288. **The same figures as the brief.** `CEILING` 16384 and
`DELTA_GATE` 50 read at `tests/harness/suite_spec.hero:162-163` and `:217-218`.

**No real delta is measured in this sitting**: `measure --refresh` is an API call and none is named
(`00-shared.md:17-19`). Every delta below is **vendored and a lower bound, said in those words**, and
every verdict on a draft is `provisional`. A draft file carries no pin, so `measure` prints no real
row for it: *"No ceiling judges `../drafts/p1.md` (panel 123 R5), so this is a count and not a
verdict"*. `measure` must run from a root holding `vendor/tokenizers/`: run from my folder it
exited 2 (*"cannot read `vendor/tokenizers/claude-legacy.json`"*), so every draft was measured from
`tree/` as `./heroes-next measure ../drafts/<d>.md`.

Each draft was built by `scripts/drafts.py`, `scripts/outside.py` and `scripts/rulers.py` from the
brief's words or mine, each anchor asserted to match exactly once. Every run exited 0
(`measure/<d>.txt`):

| draft | file | change | legacy | cl100k | delta (vendored, a lower bound) |
|---|---|---|---|---|---|
| x, p0 | spec | none (p0: no sentence, 244's R1 standing) | 8483 | 8634 | 0 |
| p1 | spec § 1, line 35 | the brief's words: *"`heroes add json` writes that entry, and `heroes fetch` places the modules the program's `use` lines reach, each file held to the listing the `sum` names;"* | 8512 | 8663 | **+29 / +29** |
| p1r | spec § 1, line 35 | mine, stating the refusal rather than the mechanism: *"`heroes add json` writes that entry, and `heroes fetch` places the modules the `use` lines reach, so a `use` written after it is refused until it runs again;"* | 8513 | 8664 | **+30 / +30** |
| p2 | spec § 1, line 33 and the example | *"... exact version and a `sum` for each module it uses ..."*. Each entry's `sum = "…"` line becomes a table `[dependencies.<n>.sum]` of quoted module keys, `"parse" = "sha256-…"` | 8512 | 8663 | **+29 / +29** |
| p2s | spec § 1, line 33 alone | p2's sentence, the example untouched (the sentence then contradicts its own example) | 8490 | 8641 | +7 / +7 |
| fM | `.claude/rules/cli-surface.md:70-71` | the falsifier names the edits M opens (§ Q5) | 1214 → 1284 | 1236 → 1305 | **+70 / +69** |
| rM | `docs/design.md:578-580` | my first ruler counted by reach. **Withdrawn**: it also narrowed 242's deletion duty to *"the contract of each module it reaches"* | 109337 → 109351 | 112733 → 112747 | +14 / +14 |
| **rM2** | `docs/design.md:578-579` | *"...where the lines of the modules its root reaches, a placed module counted as its own, are fewer after the step than before, or a copy the step deletes..."*, the deletion duty kept | → 109346 | → 112743 | **+9 / +10** |
| **rU** | `docs/design.md:578-579` | the ruler kept, its unit stated: *"...where its own tree, every line of every placed file counted, is smaller after the step than before..."* | → 109341 | → 112737 | **+4 / +4** |

p1 at 8663 vendored stands far under 16384. At the base's ratio of real to vendored (11303 / 8634 =
1.309) it would read about +38 real: **an inference, unrun**. No draft breaches the ceiling, so no
veto of mine rests on the budget. What each draft owes is the payment rule (design.md §1.6:324-330:
*"Every amendment owes a named removal or a registered prediction naming an instrument that exists
today"*) and 244's bar.

**Why p2's example has that shape.** The reader refuses every key an entry does not hold
(`selfhost/module/catalogue.hero:9-12`, *"a catalogue holds what `HOLDS` says and nothing else"*).
Dotted keys and arrays are refused by name (R5, `235-*.md:589-592`). A module path such as
`net/http` is not a bare TOML key. So **no spelling of F2 exists inside R5's subset**: every one
widens the format, which R19 fixes as golden (*"the catalogue's file, format and fields"*). The shape
priced is the least I could write: a third-level table and quoted keys, two forms the subset does
not hold today.

## What a reader meets (question 11), re-run in my copy

`probe/` holds `tree/packages` and copies of `tree/examples/ini` under `probe/examples/`. The edit:
`use <core>/strconv` above `use <core>/strings`, and `print(strconv.atoi(s:
"7").default(0).to_str())` as `main`'s first line, then `heroes-next fmt main.hero --in-place`, exit
0. Every compile under `timeout -s KILL 300`, the binary under `timeout -s KILL 60`, its output
through `head -c 65536` into `out/`.

| copy | placed tree | command | exit | what it said (`out/`) |
|---|---|---|---|---|
| whole | 30 files | `check` / `build -o sw-ini-whole` / the binary | 0 / 0 / 0 | the binary printed `7` first, then `,client,server` |
| pruned | `strings.hero`, `strings.md`, `strings/internal/scan.hero` | `check` | **1** | `error[package_unread]` at `main.hero:28:5`, the new `use <core>/strconv`: *"the tree placed at ... is `sha256-ae99678252e9…`, and the catalogue's `sum` for it is `sha256-38e61943…`: a file of it was changed, added or taken out since it was placed — move it out of `heroes.packages/` to keep the change, or remove it, and `heroes fetch` places it again"* |
| pruned | same | `fetch` | **1** | `sum_mismatch`, *"`encoding/json.hero` taken out, ... `net.hero` taken out and 17 more; the change is yours, so nothing was placed over it"*, 3 files left |
| reached | exactly the edited program's reach: `strings.hero`, `strings/internal/scan.hero`, `strconv.hero`, `strconv/internal/shown.hero` | `check` | **1** | `package_unread`, the tree `sha256-313514f3…`: **the set M would place is refused today**, having no listing |
| added | whole, plus `notes.md` | `check` | **1** | *"holds `notes.md`, a file added since it was placed ... remove `…/notes.md`, or remove the tree, and `heroes fetch` places it again"* |
| addfirst | none, then `heroes add core --index file://../../packages` before any `use <core>` | `add`, then `use <core>/strconv` written, `check` | 0, 0 | `added core 0.5.0`, `wrote heroes.toml`, **30 files placed**; the `use` written after it builds |

The brief's measurement holds in my copy. **What each route owes that reader:**

- **Under W** nothing changes, and today's advice is true: the pruned tree *is* an edit, and
  removing it lets `fetch` place the version again.
- **Under M, G or H, today's advice is false in its cause and over-prescribes its fix.** After an M
  fetch nothing was *"changed, added or taken out since it was placed"*: the program's reach moved.
  And *"remove it"* is needed only if M's `fetch` refuses to extend an unedited tree. The message M
  owes names the module that is not placed and one command: *the tree at `…` holds the modules the
  program reached when it was fetched, `strconv.hero` not among them — `heroes fetch` places it*.
  It is a message, not a spec sentence, and it is the compiler-engineer's to write and pin.
- **`addfirst` is the shape that decides p1** (§ Q2). Under W, `add` with no `use` yet places the
  whole version, so the spec's sentence order (add, then use) builds. Under M an `add` run before any
  `use <core>` line has an empty reached set. Unless M's `add` places by some other rule, the spec's
  own route ends in the refusal above. **Unrun by me** (M is not built in my tree): the
  compiler-engineer's built M answers which set `add` places with no `use` naming the entry.

## Q1. Which sentence each route makes false

Read in my tree, each with its line. **The coordinator's reading is confirmed in its shape and
corrected in four places.**

- **Spec § 1: no sentence becomes false under W, M, G, H, F1, F4, the split or rM2/rU.** *"names with
  its source, exact version and `sum`"* (33) stays true under F1, where the `sum` is unchanged.
  *"`heroes add json` writes that entry"* (35) and *"a program holds one version of each package,
  every one listed in its catalogue"* (39-40) stay true everywhere. **F2 makes line 33 false** (a
  `sum` per module) and its example too. **F3 leaves line 33 true in letter**, but the example's
  `sum` stops being the kind a reader's catalogue holds, since it moves with the `use` lines. **Under
  the split no sentence becomes false either.** But the `Use` production (61) takes an `ident`
  between the brackets, so an entry cannot be named by a Go path: `encoding/json` and `net/http`
  would need one-word entry names, and Go's tree under `core`, which panel 234's R2 chose and 235's
  R15 kept (*"R2's choice of Go's tree ... stands"*, `234-*.md:692-699`), would break. That is my
  reading of the production, not a run.
- **README**: the opening (*"places the version's files under the program's own `heroes.packages/`,
  the index's files byte for byte"*, 21-23) and § 1 rule 4 (*"places the version's files under
  `heroes.packages/`"*, 49; *"The compiler holds the placed tree to the entry's `sum` at every
  build"*, 50-51) become false under M, G and H. Rule 4's last sentence also becomes false under F4.
- **The guide**, `site/src/html/docs/packages.html` (and its twin): line 112, *"Inside is a
  byte-for-byte copy of the package's files as the index publishes them"*, **one more sentence the
  brief did not name**, becomes false under M, G and H. Line 183, *"checks the copy ... against the sum
  at every build"*, becomes false under F4 and stays true in substance under F1. Lines 186 and 195
  quote the two messages whole, so they move with them.
- **design.md**: §1.11 *"it checks every placed tree's fingerprint, its own included"* (576-577) is
  false in letter under F1, which never computes a placed tree's fingerprint (only its listing's and
  each file's line), and false outright under F4. *"placed files counted"* (578) is changed by rM2 and
  rU and by no placement route. §3.5's *"every source is checked against its fingerprint"* (921-922)
  becomes false under F4.
- **The verbs' help is not made false by M, refuting the coordinator's reading in part.**
  `table.hero:195`, *"place every package heroes.toml names under heroes.packages/, each checked
  against its sum"*, stays true in letter under M with F1: every package is placed, in part, and each
  is held to its sum through its listing. It becomes incomplete, not false. Under F4, *"checked"*
  stays true of the fetch. `add`'s *"places each"* (194) stays true too.
- **The messages: more than two.** `grep -rn -F 'places it again' selfhost` and `grep -rn -F 'placed
  again' selfhost` in my tree name, besides the brief's two:
  - `selfhost/module/package_told.hero:53`, `unknown_entry`: *"the package names a package its
    catalogue does not hold, and **a package is fetched whole** — `heroes fetch` places it again"*.
    False under M, G and H. **No golden pins it** (`grep -rl -F 'a package is fetched whole'
    tests/golden | wc -l`: 0).
  - `selfhost/module/packages.hero:227` (*"cannot be read whole"*), `:253` (the brief's
    `package_unread`), `:256` and `:262` (the *"added since it was placed"* forms, measured above as
    `added`), and `selfhost/fetch/edited.hero:73` (the brief's `sum_mismatch`).

  Goldens pinning them, by folder: *"places it again"* 6 (`check/package-tree-edited`, four
  `check/fixedbugs-2459-*`, `fetch/fixedbugs-2493-a-fetch-over-an-edited-placed-tree-*`); *"to have
  the tree placed again"* 3 (`fetch/fixedbugs-2493-*`); *"a file of it was changed, added or taken
  out"* 3. Whether M moves them turns on whether a tree with no listing keeps today's whole-tree
  check (the brief's point on the 55 hand-placed cases). **The compiler-engineer's run decides this**,
  not my reading.
- **The ruler moved (rM2)** changes design.md:578-579 and panel 242's R2, and no reader-facing text.
  **The split** changes R15 and every `use <core>/…` line, as the coordinator says, plus the Go-path
  point above.

## Q2. Reader-facing or not, route by route

| route | program | catalogue | `use` line | a message acted on | verbs' help | spec sentence |
|---|---|---|---|---|---|---|
| W | no | no | no | no | no | no |
| **M, G, H** | **yes**: a `use` written after the fetch is refused until it runs (measured, `pruned`, `reached`) | no (F1) | its validity now depends on the fetch's state | **yes**: the messages of Q1 are false in cause | incomplete, not false | none false; **p1 or p1r owed** if 244's bar is met (below) |
| F1 | (with M) | no | no | a new refusal for a file present and unlisted | no | no |
| F2 | yes | **yes**: a `sum` per module, moving with the `use` lines, in a widened format | its edit makes the catalogue stale | yes | yes (*"with its sum"*) | **yes, line 33 and the example (p2)** |
| F3 | yes | **yes**: its `sum` moves with the `use` lines and the folder's key with it (`placed.hero:129-130` hashes `e.sum`) | its edit makes the catalogue stale | yes | no | none false in letter; the example's `sum` misleads |
| F4 | no change of what builds while trees are untouched; **an edited placed tree builds** | no | no | the edit refusal disappears | no | no |
| split | **yes**: every program using `core` | **yes**: up to 5 entries | **yes**: every `use <core>/…` | yes | no | none false; a reader writes `use <strings>/strings` |
| trees not committed | **yes**: a fresh checkout does not build before `fetch` (R8) | no | no | yes | no | no |
| rM2, rU | no | no | no | no | no | no |

**Confirmed, all four**: the measured edit is reader-facing for M, G and H. F3 moves the catalogue's
`sum` with the `use` lines. p1 is a spec sentence. And trees not committed is a workflow change a
reader meets first.

**244's bar, for p1.** Panel 244's spec-warden objected to d06 (*"`heroes add json` writes that
entry and places the package, `heroes fetch` places every entry's"*, +23 / +24) on two grounds
(`244-reports/spec-warden.md:194-198`). First, *"`heroes add` places the package as it writes the
entry (`a0`), so the spec's route needs no `fetch`"*. Second, the clone case is told the exact
command (`b1`). **Under M both grounds are open again.** The first falls if M's `add` places the
empty reach (`addfirst`, owed by the compiler-engineer). The second holds only if M's message names
the unplaced module and `heroes fetch`, which today's does not. So:

- **under W, p0 stands as 244 ruled**, and p1 would be false: `fetch` places whole;
- **under M, p0 is admissible only if** (i) the built `add` lets the spec's route build, and (ii) the
  refusal's message names the module and `heroes fetch` as its one fix. Failing either, a reader of p0
  is *"unable to predict a refusal"* in the edit made most often, 244's bar is met in its letter, and
  **p1r is the sentence**, over p1. p1r states the refusal a reader must predict. p1 states a
  mechanism (*"held to the listing the `sum` names"*: a `sum` names nothing, it is a hash) that a
  reader can neither see nor act on, and 244 kept tool facts out of the spec.
- **Admitting either re-opens 244's R1**, whose *"Out"* names `heroes fetch`. This sitting may rule
  it, as an amendment of a ratified ruling with its own ratification. These briefs do not rule it,
  and nor do I.
- **The measurement 244's bar asks is a blind reading**, a paid run named by nobody here: a reader
  holding p0 or p1r, given the measured edit under a built M. I name it as owed, sized as 244's was
  (*"Sì, fino a 3 USD"*, `244-*.md:220`), for the coordinator to put to the author.

## Q3. Panel 235's R2

**My reading is behind the historian's report, which I read first** (`250-reports/historian.md`,
its items 1 and 2 and § 1, each marked *verified*). Go's own reference says `go mod vendor` places
the reached Go packages, not whole modules. `vendor/modules.txt` lists the vendored packages. The
sum covers the whole module's zip. And *"The `go` command does not check that packages in the
`vendor` directory have not been modified"*.

- **R2's catalogue sentence is untouched by M with F1**: *"the program's catalogue lists every
  package in the build, transitive ones included, ... each with its fingerprint"*
  (`235-*.md:546-549`). Under F1 every entry keeps its `sum`, unchanged.
- **R2's build sentence is kept in substance and broken in letter**: *"at build it checks ... each
  placed tree's fingerprint (R7)"* (`:550-551`). Under F1 every placed byte is held to the `sum`
  through the listing, but no placed tree's fingerprint is computed. That clause was itself 235's bend
  from Go, a check Go does not make (the historian, item 1), so placing a subset departs from Go in
  nothing R2 copied from Go.
- **So M departs from R6's and R7's wording, not from R2's rule.** R6: *"every regular file under the
  package's root"* is what the `sum` is taken over at the source, unchanged by F1. But a listing placed
  inside the key folder is such a file of the placed root, which `fingerprint.hero:2-4` hashes today.
  R7: *"one tree per package"* still holds; *"The placed tree is checked against its fingerprint at
  every build"* changes mechanism.
- **The proposal said** *"The semantics copy one widely used system whole, Go's modules"*
  (`235-*.md:60`). Go's vendor grain is the reached Go package (the historian, verified), so under
  that proposal G is the grain that copies Go and M is finer than Go. **F4 is Go's own behaviour and
  still refused** (Q10): 235 departed from Go there on purpose, and that departure is ratified.

## Q4. Panel 234's R2, and why whole placement replaced it

- **234's R2 placed the reached grain, by Go package**: *"it carries under its own root the
  packages it reaches"* (`234-*.md:291`), where a *package* was a Go package under `core/` (*"`use
  core/strings` binds `strings`"*, 289-290). Its copies were the reached Go packages
  (`00-shared.md` § Before whole placement: `ini` 2 files, `todo` 4). **So G, not M, is that
  sentence's grain.** M is finer, by module, and coincides with G for every program here only
  because every reached face's `internal/` folder is reached whole.
- **235's R15 amended "R2's placement", meaning where, not what**: *"a copy of `core/` in each
  program's own root reached by a bare `use`, fails that sitting's question 17"*
  (`234-*.md:692-699`), the copy left behind compiling as the program's own. Nothing in R15 is about
  grain.
- **Whole placement arrived as a consequence, unweighed.** R6 took Go's `h1` over *"every regular
  file under the package's root"*. R15 made the entry `core` *"one catalogue entry"*. So *package*
  changed unit, from Go package (234) to entry (235). `31c34d962`'s body (2026-11-08) describes the
  verb's behaviour (*"the tree it placed, `core@0.1.0-db7f072b4678b0b1`"*), over a version of six
  files, where whole and reached barely differed. **No record weighs a subset**: my `grep -n -i -E
  'it reaches|they reach|reached (go )?packages|only the (files|modules|faces)|every file of|whole
  (version|package|tree)|the version.s (files|tree)|unreached|not reached|per.face|per face|grain'`
  over `docs/panel/235-*.md`, `235-briefs/*.md` and `235-reports/*.md` printed four lines, none about
  placement's grain. That is a different vocabulary from the critic's, which also printed nothing
  (its § B, carried). **This is a negative claim resting on two searches' vocabularies, put as a
  question**: did any reading weigh it under words neither search used?
- **234's R13 and 241's R3**: R13's *"a copy under `selfhost/core/`"* (`234-*.md:488-499`) was
  withdrawn by 241's R3 (`241-*.md:192-203`: *"When the compiler takes a package, it takes it as
  route 1 takes `core`: a catalogue entry, a placed tree, a fingerprint"*). Feature 2315's notes still
  name `selfhost/core/` (its body's line 22, carried). That is a stale carrier in an open issue, to be
  corrected by `comment` with its date, whatever this sitting rules.

## Q5. The fifth input class

**By the paragraph's letter** (`cli-surface.md:76-78`, *"a second per-project file beside the
catalogue, a lock, a workspace or a configuration, is a fifth class"*) and panel 056's three
conditions, I take this reading:

- **F1's listing placed in the tree is no fifth class**, if all three of these hold. (i) It is,
  byte for byte, the text the `sum` is taken over (the index's `list`), so it carries no fact the
  catalogue does not fix. (ii) The build reads it only to verify, never to decide which module
  compiles, the reached set being the `use` lines' alone. (iii) Its absence, edit or move only
  refuses. It is then part of the verified fetched copy, per package, inside the placed folder, not
  per project beside the catalogue. Break (ii) and it decides what compiles: a lock in substance.
- **A record of which modules were placed** (a subset list the verb writes) **is a fifth class**:
  it records a resolution the catalogue does not hold, which is what a lock is. Refused, unless the
  verb derives the set from the files on disk, which are themselves that record.
- **The verb reading the program's `use` lines is no new class**: it reads class one, a `.hero`
  file. But `fetch` takes no operand (`table.hero:195`), so the reach must be computed over every
  `.hero` under the folder or over named roots. `tests/harness` holds six roots. That is a rule the
  verb must state and the help must say. It is not a class.

**fM, the falsifier for M** (+70 / +69 vendored on `cli-surface.md`, which no ceiling judges, so the
price is information): *"...and a fetched copy left behind after its entry is gone compiles nothing;
nor does a `use <name>/…` line added or removed after the fetch, a placed file removed or added, or
the placed listing edited, moved or removed: a module the program reaches that the fetch did not
place, and a placed file the listing does not name, are refused, never looked for elsewhere; a
`path` entry, ..."*. Each shape it names needs a `check` case, by the paragraph's own rule (*"The
`check` golden form holds one case for each"*), and **none exists**. A file of the version that the
author copies back in, matching its line, builds where it was refused: that changes *whether*, not
*which*, so the falsifier holds.

## Q6. R19

- **M changes R19's interface at *"the error codes and texts"***: at least one new text (the module
  not placed), and every message of Q1 rewritten in its cause. It leaves the catalogue, its format,
  the fingerprint's algorithm (F1 keeps the `sum` over the whole listing), the placement folder's name
  and the verbs' output unchanged. The `fetch` form prints folder names alone
  (`suite_fetch.hero:452-456`, read), so no `fetch` transcript moves by what is placed.
- **R19's last sentence**, *"The second version replaces the transport alone"*, and 242's R5,
  *"replaces no transport"*: M is neither. It changes placement, so if it lands before 2315, task
  2312's goldens are re-fixed first and 2315's *"unchanged"* is measured against the new ones. The
  order is the synthesis's question, as the brief says.
- F2 and F3 change *"the catalogue each writes, byte for byte"*. F2 also changes *"the catalogue's
  file, format and fields"*.
- **Where my reading and the compiler-engineer's run differ, the run stands.**

## Q7. Decision 2528 under the split

*"every program moves to the newest version of core and the older versions are removed"* names an
entry the split abolishes, so it reads as nothing until reworded: *each* entry's newest version, five
newest versions moving apart. The verb's MVS check (*"the program's version of a package is at least
each dependency's"*, 235's R2) then binds inside what was `core` (`net/http` over `strings`), a class
of refusal no reader meets today. **2528 is the author's decision, so the split needs the author's
word on it as well as a sitting on R15.** A sitting cannot reword an author's decision.

## Q8. R2's words

- **The unit.** design.md:578-579 and 242's R2 say *"smaller"* and name no unit. 242's
  compiler-engineer counted lines and summed unlike things, *"2,334 placed and 5 of catalogue"*
  (`242-reports/compiler-engineer.md:107-109`), the catalogue being TOML. The author was given every
  file's lines, cards included (2,389 = 2,334 + 55). **My reading of *"placed files counted"*: every
  placed file counts, whatever its kind.** So cards, a placed listing and the compiler's own
  catalogue all count, in lines, because any unit that leaves a kind out invites moving content into
  it. **rU (+4 / +4) writes that down** if the ruler stays.
- **Why placed files were counted is unweighed in the record.** `grep` of `242-*.md`, its briefs and
  its reports for the ruler's reasoning (*"placed files counted"*, *"tree grows"*, *"smaller after"*,
  *"its own tree"*) prints counts and the ruling, and no sentence on whether an **unreached** placed
  file should count. The ruling was written over a whole placement taken as given. design.md §1.1,
  which 242's R2 names as what binds (*"§1.1's ceiling"*), says *"Implementation simplicity ... sets
  the ceiling"* (178-180), and an unreached placed file is no part of what the compiler implements.
  That is **an argument for rM2, not a measurement**: rM2 states what *"placed files counted"* most
  plausibly guarded against, code moved out of `selfhost/` into a package to make the count shrink.
- **The compiler's tree** is the folder of its root module and what it carries: `selfhost/` with its
  own `heroes.packages/` under (A). §1.11 says *"The compiler is a program like any other here, and
  may carry a package through a catalogue of its own beside its root module"* (574-575). Not
  `runtime/`, which the program links but does not carry. Not the repository's `packages/`, an index
  serving every program. **So route 9**, the compiler reading `packages/core/<v>/files/`, places
  nothing and counts nothing. That is a `path` entry in substance: it collides with R15's *"never
  `path` entries"* (`235-*.md:747-748`) and is the reading that empties the ruler. Objected.
- **Which package**: 242's R2 says *"A step that adds an entry to `selfhost/heroes.toml` deletes
  every live copy of that package's contract"* (`242-*.md:141-142`). So *package* is the **entry**,
  every face of `core` whose contract the compiler copies (its 7 json files, carried, among them).
  rM2 keeps that duty. My first draft, rM, narrowed it and is withdrawn.
- **What M and rM2 give the compiler, measured here**: the reached `strings` set is 368 lines
  (`wc -l packages/core/0.5.0/files/strings.hero files/strings/internal/scan.hero`: 258 + 110). The
  listing is 30 lines, the card 21, the whole version 6,574 lines in 30 files. Under M with F1, *"placed
  files counted"* counts 368 + 30 = 398. H counts 419. rM2 counts 368. W counts 6,574. **M and rM2
  differ for the compiler by the listing alone, so choosing between them does not decide whether 2315
  meets R2. It decides who pays: every program's reader, or one design.md sentence.** Whether either
  shrinks the tree is the compiler-engineer's count. 242's route (C), the reached set alone, grew it
  +215 at 0.2.0 (`242-reports/compiler-engineer.md:287-296`, carried by the brief).

## Q9. Panel 246's R2 and defect 2493

*"An edit inside a placed package tree is the author's work and is never discarded in silence"*;
*"a fetch or an add never replaces an edited placed tree"*. What they require of M:

- a `fetch` that **adds** newly reached files to a tree whose present files each match their listing
  line discards nothing, so it is allowed;
- a `fetch` that **removes** newly unreached files may remove only files matching their line. One
  that differs is an author's edit, refused by name as 2493 refuses. A plain removal would discard it
  in silence. Keeping them instead makes the tree grow back towards the whole, use by use;
- **under F1 an author's removal of an unreached file is invisible**: it is the normal state of an M
  tree. Today it is an edit (`pruned` above, *"taken out"*). That is a loss of what the build can
  tell the author, not a discarded edit, so 246's letter holds. It belongs in the falsifier's cases
  (fM).

## Q10. Principle 0 and the burden, route by route

**No route adds a form of the language**, so Principle 0 binds only the spec sentences (p1, p1r,
p2), under 244's bar and the payment rule. The routes themselves rest on these ranks:

| route | rank its case rests on | verdict | what pays, or what would make my refusal wrong (CLAUDE.md § 12) |
|---|---|---|---|
| W, with rU | 5 (process: 2315 stays unmet unless the compiler-engineer counts otherwise) | **approve** | rU needs no payment (no ceiling). Wrong if: none, it changes no behaviour |
| W, with rM2 | 5 | **approve, provisional**; amends 242's R2 and design.md Part 1, so ratification is owed | wrong if a step counted by reach shrinks while the compiler's repository carries unreached code that some later step reaches with no step of its own. Under 242's R2 the same gap stands, since its ruler fires only when an entry is added |
| M, G, H | 5, paid at §1.2's indicator (rank 4) | **object** | wrong if the compiler-engineer's count shows the reached grain shrinking the compiler's tree where W grows it, *and* the blind reading shows p1r's readers running `fetch` unasked. If adopted anyway: **G over M**, the grain of 234's R2 and of Go's vendor (historian, verified), with no C closure to compute. Go paid for its package grain with a package's C files pruned from subfolders (golang/go#26366, the historian, verified) |
| F1 | (with M) | the only fingerprint I would admit under M | — |
| **F2 / p2** | none | **veto** (Principle 0's burden unmet) | F1 verifies the same placed files with no catalogue change, so F2's widened format (two forms R5's subset lacks) and its sentence (+29) are needed by nothing, and serve no Part 11 effect. Wrong if a program needs a per-module `sum` that F1 cannot check |
| F3 | none | **object** | the folder key, every `#line` path and the seed move with every `use` change (`placed.hero:129-130`). The `packages` suite's `placed` check stops holding |
| F4 | **rank 3 against it** (robustness) | **object**; not my veto, which is budget and burden | an edited placed tree compiles at exit 0, the outcome 235's R7 refused and W4's *"the placed tree edited"* tests. Wrong only if no program can be handed an edited placed tree, and R7's *"a folder anything can edit"* says one can |
| split | 5 | **object** | changes R15, 17 programs' and the net's `use` lines, Go's tree (the `ident` point, Q1), and needs the author on 2528 |
| trees not committed | 5 | **object** | reverses R7's *"inside the program"* and makes a fresh checkout unbuildable before `fetch` (R8) |
| route 3, M for the compiler alone | 5 | **object** | a special case for one program, against 242's R3 *"the one mechanism every program already uses, so no special case (§1.7)"* |
| route 2, W with R2's defect branch | 5 | no objection | within the ruled text, reader untouched; the measurement is the compiler-engineer's |
| route 9 | 5 | **object** | Q8 |

**Payment for p1r if the sitting admits it**: no removal exists in § 1 (`nothing, and that is a
problem`). So it needs **a registered prediction with an instrument that exists today**. The panel's
blind seat (`.claude/skills/panel/SKILL.md` § 2) does: *with p1r, at least 7 of 10 blind runs given
the measured edit under a built M run `heroes fetch` before their first `check`; with p0, at most 3
of 10*, scored at this milestone's close. It is a paid run, owed, and the author's to allow.

## Questions I could not run, for the coordinator

- **Which set M's `add` places when no `use` names the entry yet** (`addfirst`): the
  compiler-engineer's built M. It decides p0 against p1r.
- **Whether the compiler's step shrinks at the reached grain at `48f53b343`**: the
  compiler-engineer's count, which my prediction (a) is scored by.
- **The blind reading** of p0 against p1r under a built M: a paid run, about 3 USD on 244's
  precedent, the author's to allow.
- **The real delta of p1r**: `measure --refresh`, an API call, the author's to allow, owed only if
  p1r is adopted.
