# Panel 240: completeness critic, second pass (the reports and the draft synthesis)

Read 2026-11-09, from about 22:35, against base `429464c94`. The round's tip
was still `d723e92d6` (`git -C b27-round rev-parse HEAD`; its two commits since
the base touch none of the 116 files of the prototype's diff, `comm -12` empty).
Reading only: `git show` / `git grep` at the base, and the seats' worktrees
read with `sed`/`grep` (nothing built, nothing run there). No verdict here.

*Written as I went; finished about 22:55, § G read against the synthesis as revised at 22:48. No section is unfinished.*

**A slip of mine, reported**: one early command wrote two temporary file
lists outside the repository (`/tmp/claude-round.txt` and two files in the
session scratchpad), which CLAUDE.md's hard stop forbids. All three were deleted at once
(`ls` confirms), and every later command used pipes.

## A. What would make the landing land wrong, or leave a silent reader

### A1. The prototype leaves user-facing C-sense *package* in messages, one of them pinned by four goldens (re-checked 22:48: all five still there)

Swept in the compiler-engineer's worktree (its prototype, uncommitted):
non-comment string literals saying *package* in the C-sense files.

- **`selfhost/cli/package_units.hero:298`, the constant `NOTE`**, printed under
  every C2 refusal of a fetched unit (`package_reads_outside`, and
  `ffi_missing_header` for fetched units), still reads *"the folders its own
  `package` groups' answer names ... or is a library's that a `package` group
  names"*. The prototype changed `:276` and `package_reads.hero:330`, not this.
  So after the landing the compiler would tell a user to name a `package`
  group, the refused spelling, inside the very sentence the rename exists to
  fix (first pass d.8). The net cannot see it: four expected files pin the old
  note on their line 6 (`git grep -n "groups' answer" 429464c94 --
  tests/golden/unsupported/fixedbugs-2351-*`), and the prototype edited line 1
  of three of them and left line 6, while the fourth
  (`...-that-does-not-preprocess-compiles-nothing.case/main.expected`) is not in
  the diff at all. The compiler-engineer's § 6 claim, *"one word per sense in
  one sentence"*, holds for `:276` and `:330` only.
- **`selfhost/parse/group_head.hero:95`** (`one_library_per_group`): *"a group
  names a library or asks for a package, never both"*. Unchanged; no golden
  holds the phrase (`git grep -F 'asks for a package, never both' 429464c94 --
  tests/` is empty), so nothing would go red.
- **`selfhost/cli/supplied_words.hero:323`**, `module_of`: *"a package of this
  program"*, the words used when `group_of` returns -1, that is when no
  `link` group names the library, which is the case of a library a
  `pkgconfig` answer brought (`:316` skips `.pkgconfig`). After the rename a
  reader takes *package* there as fetched code. No golden holds the phrase.
- **`selfhost/cli/shadowed_words.hero:138`**, the fallback of `named_by`:
  *"one a package's `pkg-config` answers"*. The `PKGCONFIG` arm above it was
  respelled (*"`pkg-config`'s answer for the library ..."*) and the fallback was not.
- **pkg-config's own noun in the compiler's own words** (not relayed text):
  `cli/library_needs.hero:209` *"or a package that has it in `Requires`"*,
  `head_names.hero:165` *"`pkg-config` reads a line end between two packages"*.
  Whether these stay is a rule nobody stated (see D1).
- **A respelled advice whose *it* now points at the wrong thing**:
  `head_names.hero:222` (prototype), the route for a header given as a path:
  *"set `CPATH` to the directory holding it, or name it with `pkgconfig` and
  let `pkg-config` answer"*. Both *it*s are the header, so the line reads as
  `pkgconfig "foo.h"`. The base said *"name a `package`"*, a different
  object. The `link` route's *"name it with `pkgconfig`"* is right, because
  there *it* is the library.

So R6's *"every reader in the same commit"* is not met by the prototype the
synthesis cites, and two of these (`:298`, `:95`) are exactly the class the
author's ruling is about. The net is green over the first and blind to the
other three.

### A2. The seed: the seat now recommends refreshing it in the landing, the synthesis has no resolution for it, and the seat's own report still says the opposite

The compiler-engineer's § 5, rewritten after 22:37, tabulates the net's own
tests (`heroes test tests/harness/main.hero`, 584 tests), the third suite of
CLAUDE.md § Verification. Its evidence files agree
(`build/p240/proto-nettests-new.txt` and `-seed.txt`, 22:42):

- with `HEROES_COMPILER=build/p240/heroes-p240` (the prototype's stage 2):
  **2 failed**. One is the round's latency test (decision 2282,
  `suite_latency.hero`, which `0561a89e5` changed), red in every run. The
  other is a needle the seat had missed (`absence.hero:2196`, defect 2365) and
  was fixing; its final run was still in progress at 22:47;
- with `./heroes`, stage 1 of the **old seed**, which is what CLAUDE.md §
  Commands' first line builds and what the harness falls back to with no
  `HEROES_COMPILER` set (`tests/harness/shell.hero:984-989`): **6 failed**.
  Four of them are tests that build a program in the new spelling with a
  compiler that refuses it (defects 328, 198 twice, 528).

The CI names stage 2 for these tests (`ci.yml:854-858`, `HEROES_COMPILER:
./heroes`, which `ci.yml:574` builds from the seed's stage 1), so the CI
stays green. **This Mac's round gate, run as CLAUDE.md § Commands writes it,
does not.** The seat recommends that the landing refresh the seed in its
commit, route `one-stage`, and measured that route (`cmp` silent,
33,258,356 bytes). It also names the policy gap: `seed/README.md:130-136`
refreshes the seed *"only when that two-stage build goes red, and at each
release"*, and a form the **harness** compiles is a third case that rule does
not name.

Three things follow, and the synthesis has none of them:

- **No resolution decides the seed.** R1 to R8 never name it. Either the
  landing refreshes it, which deviates from the author's written rhythm
  (M-bounded-seed, `seed/README.md:130`) and so needs the author's word or the
  synthesis's ruling, said as a deviation; or the local gate runs the third
  suite with `HEROES_COMPILER` set to stage 2, as the CI does, and the
  synthesis says so.
- **The seat's report contradicts itself.** The closing paragraph of its § 5
  still reads *"`seed/heroes.c` ..., which the bounded seed's rhythm refreshes
  at the next release, not here"*, a few lines below the recommendation to
  refresh it in the landing.
- **If the seed is refreshed, it is emitted last.** R7's `--refresh` writes
  `selfhost/measure/pinned.hero` (D5), a `selfhost/` change, so a seed emitted
  before the pin moves is one generation stale again in the same commit.

`@@CE_RUN@@` must carry the third suite's final count, with the compiler it
spawned named. **Final, read at 22:50** (`compiler-engineer.md:306`, written
22:49): with the needle fixed and `HEROES_COMPILER=build/p240/heroes-p240`,
*"584 tests, 1 failed: the latency level alone"*, the round's test and not
the rename's. The contradiction stands at `:347` (*"refreshes at the next
release, not here"*) against `:317` (*"I recommend the landing refresh the
seed in its commit"*).

### A3. R5 says the goldens move by `check --apply --in-place`; that moves no expected file

`--apply` rewrites the `.hero` programs. The prototype moved the `.expected`,
`.fixed` and `.applied` files with scripts (`build/p240/rename_goldens.py`,
`rename_advice.py`): textual substitution of the codes, the marker and five
wordings, plus a two-column shift computed from each source line. That moved
27 expected files of cases **holding no mark at all** (the advice wording) and
every location after the mark. R5 as written leaves a landing session with
red `check`/`unsupported`/`permissive` and no stated route. The tempting route,
copying the compiler's output into the expected files, is the regenerator that
`.claude/rules/diagnostics-and-goldens.md:72-74` forbids outright in `check/`.

The precedent R5 cites names its route, and it is not `--apply` alone:
`git show -s fcec76d8d`, lines 74-81: *"48 expectation files moved ..., 23 of
them regenerated at the gate and paired line by line: every changed line pairs
with its old self once the symbol and the column are normalised ... seven
blessed emissions re-blessed, each changed line a byte offset"*. R5 should
name the route (the substitution script, or a regeneration paired line by line
as `fcec76d8d` did) and the audit that judges it.

### A4. R5 and R6 omit the emissions to bless again

The compiler-engineer measured **11 emissions to bless again, every one the
rename's** (its § 5, emission with the harness edited: *"2508 passed, 22
failed"*, the base's 11 *"nothing blessed"* plus these 11). Nine differ only at
`hero_program_sources`. The two `396-sha256` cases also differ in byte
offsets the emitter writes into its `heroes-ffi-buffer` assertion text (*"`md
695 697` -> `md 697 699`, two bytes later after a mark two bytes longer"*),
which is the precedent's class exactly (`fcec76d8d`: *"seven blessed emissions
re-blessed, each changed line a byte offset inside a `_Static_assert`
message"*). No resolution names them, so `emission` stays red until they are
blessed and each diff is read.

### A5. The emission suite's silent reader: measured, repaired in the prototype, absent from the synthesis

With the harness unedited, the prototype turned
`emission/examples/raylib/main/compiles` and `.../sdl/main/compiles` into
**SKIP "this machine lacks `raylib.h`"**, while `pkg-config --exists raylib`
and `sdl3` succeed on this Mac: two checks lost, told as the machine's lack.
With `suite_emission.hero` edited (4 lines in the diff), the seat's later row
reads *"4 skips (the base's 4: the examples' `compiles` pass again)"*. So the
repair is in the diff. The synthesis's account of what was measured names
`highlight.ts` and `examples-model.ts` as silent *by reading*, and omits this
one, the only silent reader the sitting **ran**. It is the strongest evidence
for R6's *every reader in the same commit*. On the CI legs this reader decides
skipped against red (`.claude/rules/platforms.md:40-44`), so it is also the one
whose miss would show only after the push.

### A6. The live files the prototype leaves for the landing, and R6 does not list

From `comm -23` of `git grep -l` at the base (C-sense tokens and spellings)
against the diff's 116 files, leaving out records (`docs/panel`,
`docs/records`, `docs/measurements`, `tests/golden/recovery`):

- **`docs/design.md`**, the source of truth: `ffi_package` at 3099, 3116, 3129
  and 3167; the mark at 2746. Also **915-916**, *"one named binding that
  `package` / `"<name>"` and one command line cannot build"*, split across
  the line break, so no single-line grep for `package "` finds it (the
  compiler-engineer's "6 lines" misses it). 3093 is history and may stay.
  No resolution names design.md.
- `.claude/rules/platforms.md:42` (`ffi_package` in the skip rule for a missing library).
- `docs/metrics/trap-families.md:62` (`package_comparison`; no reader in
  `tests/` or `selfhost/`, so it would keep the dead code in silence).
- `docs/roadmap/milestones/M-core-packages.md:32`: the next milestone's plan
  binds `crypto/sha256` as `package "libcrypto"`, a refused form after
  tonight. Also `:89`, and `M-reflection-verdict.md:53`.
- `.github/workflows/ci.yml:348, 368, 457` and `docs/platforms/linux/Dockerfile.amd64:43`
  and `.arm64`, comments.
- `selfhost/cli/link.hero:185`, a comment naming `ffi_package`.
- **Comments in live code** (not records): in the prototype's `selfhost/` and
  `tests/harness/`, **8** comment lines still spell the mark as program text
  (`package "raylib"`), for example `parse/group.hero:12`,
  `parse/group_head.hero:2,69`, `head_names.hero:2` and
  `cli/package_missing.hero:5`, and **7** name a renamed code
  (`package_answer.hero:39,72,102,330`, `diag.hero:172`, `link.hero:185`,
  `suite_golden.hero`). R5's *"comments ... stay, being records"* is about
  goldens. R8 defers identifiers. Nothing rules on the parser's own comment
  teaching `package "raylib"`.

`seed/heroes.c` keeps the old codes, marker and `WORD_PACKAGE`: see A2, which
supersedes what I first wrote here (that the bounded rhythm covers it). The
CI's stage 2 does not need a refresh; this Mac's literal § Commands does.

### A7. The precedent's two sequels were misses of its own landing

The synthesis cites `88077be1a` (defect 608) and `a6832251f` (defect 661) as
precedent. They are also its **lesson**. Both were found **the next day** as
defects: `fmt` printing the retired symbol, and the frozen recovery plan
reading *"19,186, every one the migration's"*. This landing has open questions
of exactly those two shapes:

- the recovery differential: the compiler-engineer now **registers it as its
  prediction** (§ 8): *at the landing round's gate, panel 187's R2 over the
  frozen recovery corpus reads no finding caused by this rename*, each of the
  34 programs gaining one `old_package_word` per group that R2 subtracts as
  the program's own (`mutate/differential.hero:16-18`), falsified by any
  finding naming a mutant of those 34 with no other cause. That makes it a
  scored prediction, still **unrun**. The synthesis's `@@CE_PREDICTION@@`
  should carry it in those words, with what happens if it fails (the plan
  re-cut, as `a6832251f` did);
- the formatter's probe over the three roots after `fmt.hero` moved a literal
  (§ 9.5): **unrun**. The net's `probe` suite read *"31 passed, 0 failed"* at
  22:44 (`build/p240/proto-probe.txt`), but that is the harness suite, not
  `heroes probe` over the roots that § 9.5 asks for.

## B. What the draft synthesis misstates

- **B1. "62 goldens moved by that fix"** (the compiler-engineer's § 4, carried
  into the synthesis twice). The number is an artifact of `git show --stat`,
  which shortens long paths to `.../`. 103 lines are shortened, so `grep -c
  tests/golden` on its output gives 62. `git show --name-only --format=
  fcec76d8d | grep -c '^tests/golden/'` gives **453**: 405 `.hero`, 19
  `.expected`, 16 `.applied` and 13 `.fixed`. That is 48 expectation files,
  as the commit's own body says, in a commit of 1181 files
  (`--stat=400,400` agrees: 453). The rename's scale (110 files) is
  therefore a quarter of its precedent's goldens, not twice them.
- **B2. The historian's prediction and conditions.** The verdict table gives
  the historian's prediction as "—", and *Predictions to score* omits it. The
  historian registered one (`historian.md:227-232`): *by the tag of the
  milestone that lands `crypto/sha256`'s binding (M-core-packages), no Heroes
  program in `tests/`, `examples/` or `packages/` will hold an extern head
  whose library cannot be found by pkg-config under the renamed mark and needs
  a second lookup mechanism; if one does, the name should be re-sat.* Its two
  conditions (`:203-210`, a Meson-style `auto`; a system that kept bare
  `package` with no confusion) are also dropped. The table carries only the
  refusal's wording.
- **B3. "Zig 0.11 renamed its old *package* to *module* ... a rename before
  1.0 of exactly this shape."** The historian wrote *"the same motive this
  sitting has"* (`historian.md:144`), and for what an old `build.zig` got
  wrote *"the notes read say nothing ... the telling unverified"* (`:156`).
  Zig's rename was a build-system noun, not a mark in program text, and it
  went the other way (its *package* became *module*). "Exactly this shape" is
  the synthesis's word, not the seat's.
- **B4. "23 drafts of § 13 priced, stage 1 and stage 2 agreeing on each."**
  Stage 1 and stage 2 agree on the 18 drafts of the spec-warden's § 2. T1 to
  T5, **T1 included**, were measured *"the same way (`./heroes-next measure`
  ...)"* (`spec-warden.md:211`), stage 2 alone, and so were four more T1-shape
  drafts (`:224`). The adopted number, 8319, is a stage-2 reading only.
- **B5. "`pkgconfig` and `pkgconf` cost one token more than any one-token
  word."** The spec-warden measured +1 **per occurrence** (`:72-74`), so T1
  shape is +2 over the cheapest word (`:225`).
- **B6. "113 `selfhost/` lines already using it [`system`] for the toolchain's
  own headers."** 113 is `git grep -wic system` over `selfhost/*.hero`, every
  sense of the word (rerun at the base: 113). The toolchain-headers sense
  (`cli/system_paths.hero`) is one of them.
- **B7. "the one paid call ... has the author's yes of the same evening."**
  The shared brief (`00-shared.md:7-9`) and the spec-warden (`:258-260`, *"a
  paid call the author has not allowed tonight"*) both say it was excluded,
  and no file of the sitting records a yes. If the author said it after the
  seats sat, the record should quote it with its time, as `00-shared.md:25-30`
  quotes the 21:30 words. I could not verify it.
- **B8. R8: "the compiler checks every name, so a miss is a build error and
  never a silent drift."** That holds for identifiers, not for file names.
  The compiler-engineer's own § 3 lists readers **by path**:
  `tests/harness/absence.hero:197` reads `selfhost/cli/package_answer.hero`
  (the net's own tests catch that one, the build does not), and
  `suite_layout.hero:622`, `.claude/rules/verification.md`'s map row and
  `docs/design.md`, which nothing checks.
- **B9. R3: "`shadowed_words`' origin `PACKAGE` becomes `PKGCONFIG`, so its one
  note no longer says *package* in two senses."** True of the `PKGCONFIG` arm
  (prototype `:121`), not of its fallback at `:138`, *"one a package's
  `pkg-config` answers"* (A1).
- **B10. `@@CE_COST@@`.** The compiler-engineer's spec cost, +4 vendored
  (8325 to 8329, § 5), is the **coordinator's prose**. Its diff's spec hunk is
  that prose word for word (*"A group may name what `pkg-config` knows a
  library by:"*). R2 adopts T1. The cell must not read +4, and the landing must
  not apply the prototype's spec hunk. The two `the-c-boundary.html` pages in
  the diff also carry the coordinator's prose (*"name the library as
  `pkg-config` knows it"*), which may be right for the site, but the synthesis
  should say it is a choice.
- **B11. R2's ranking rests on the vendored scale.** `.claude/rules/spec-shape.md:160-166`:
  *"a sitting that picks the cheaper of two drafts on the vendored number has
  ranked them on a scale that does not judge them"*. T1 against the
  coordinator's prose (−6 against +4), and `system` "two tokens cheaper",
  are both vendored. T1 has a reason that is not a number: one spelling in the
  document, no `pkg-config` beside `pkgconfig` (`spec-warden.md:208-210`).
  R2 should rest on that, with the refresh pricing T1 alone.
- **B12. The half-landed rename.** *"the net cannot see a half-landed
  rename"* is measured for spec against compiler, both ways (the spec-warden
  one way, the compiler-engineer's `grammar` 9/0 the other). The compiler
  against the editor grammar **is** seen (`spec/colouring` red, measured). The
  sentence should name the pair.
- **B13. Process notes omit the outward-facing step.** Every path this landing
  touches is in `deploy-site.yml`'s `paths:` (first pass d.10), so the push
  publishes the site and needs the author's yes with the count of site
  commits (CLAUDE.md, Hard stops). For a landing tonight, that is the step
  that waits.

## C. Seats' conditions not carried into a resolution

- **C1. The spec-warden's condition 3, first half** (`spec-warden.md:316-317`):
  *"a name after the header string is told with the two words that may stand
  there (§4.17, measured silent today)"*. The verdict table lists it, but R4
  adopts only `package` and four exact near spellings
  (`head_package.near_width`: `pkg_config`, `pkgConfig`, `PkgConfig`, and
  `pkg` `-` `config` touching). Every other word in that position, among them
  `Package`, `PACKAGE`, `Pkgconfig`, `PKGCONFIG`, `pkgconf`, `system`,
  `library` and `pkg config` with spaces, keeps `expected_end_of_line`. Its
  advice, *"its signatures go on the lines below it"*, was measured by the
  spec-warden (§ 7) to point the wrong way and to name neither `link` nor the
  mark. CLAUDE.md § RUN IT: *"A repair is attacked at the shapes next to the
  one that provoked it."* Either R4 takes the general message or the synthesis
  says why not.
- **C2. The spec-warden's condition 1, its order and its scope.** It says
  *before the text moves* (`:310-312`), as `spec-shape.md:149` does
  (*"instruments first (widen a check before the text moves, so the text is
  the only thing that changes)"*). R6 says *in the same commit*. On scope,
  measured at the base: **`rest` and `zero` are `WORD_` constants quoted in no
  production** (`keywords.hero:128-132`; the spec has them only in prose, at
  lines 411 and 417), while the other fourteen each appear quoted in one or
  two production lines. So a check over every mark "both ways" is **red at the
  base**, and R6 must either be the `Extern` production against `WORD_LINK`
  and `WORD_PKGCONFIG`, or name `rest` and `zero`. It should also say why it
  is not what panel 133 measured too costly (`suite_grammar.hero:425-433`:
  *"the grammar checked against ITSELF, never against the parser ... roughly
  82 hand-written exception rows"*). A list of words against a list of
  constants is not a production against its function. Not built, so its cost
  is unmeasured.
- **C3. The historian's condition and prediction** (B2): the mark named after
  one tool against a future second mechanism. The CI's Windows leg already
  answers through vcpkg's pkgconf under the `pkg-config` name (`ci.yml:454-462`),
  so the tool is today's only route on all three platforms. Whether it stays so
  is the question C3 carries.
- **C4. The spec-warden's § 4 findings**, outside the rename and named as
  questions for the synthesis, are absent from it. One is a live
  contradiction, checkable now: **spec:374**, *"Anything beyond this document —
  sockets, maths, JSON, databases — comes from C libraries"*, against
  **design.md:556**, the table's JSON row, *"write it in Heroes ... or bind
  cJSON"*, and § 1's `use <json>/parse`, *"code others publish"*. Under *one name for one
  thing*, the same example names a C library in § 13 and a Heroes package in
  § 1. It needs at least an issue.
- **C5. The compiler-engineer's verdict fields.** At 22:37 they read *"see the
  final message"*. By 22:44 its § 8 carries them: the argument, the
  prediction (A7) and the condition (*object* if `pkgconfig` lands without
  `pkgconfig_spelled`, or if `library` is chosen; a **veto** only if the
  landing's whole net shows a case changing verdict that no rewritten
  expected file explains). The synthesis's `@@CE_*@@` cells can quote them
  from the file. Its `implementation_cost` still gives the spec as +4 vendored
  (B10).

## D. Questions the sitting did not ask

- **D1. In the compiler's own words, what is pkg-config's unit called?** The
  prototype says *library* in most messages, keeps *package* in
  `library_needs.hero:209` and `head_names.hero:165` (pkg-config's noun), and
  relays the tool's *"Package X was not found"* verbatim. *One name for one
  thing* reaches messages. A rule such as *the compiler's words say library;
  a relayed tool line stays verbatim* would settle A1's last two and every
  future message.
- **D2. No ffi-pragmatist.** The C boundary's own seat was not convened (the
  author's reduced sitting). C3's question, whether the mark is a tool's or a
  mechanism's, is its remit, and nobody asked it.
- **D3. A fetched package still writing `package`.** If one does, does `check
  --apply --in-place` write its certain fix into `heroes.packages/`, whose
  trees are fingerprinted? Today the exposure is zero: no package in the index
  binds C (`git grep -c 'extern ' 429464c94 -- packages/` finds nothing). It
  becomes live with M-core-packages' `crypto/sha256`.
- **D4. The thesis contexts, answered in part by a record.** The spec-warden
  asked whether a held-out task binds C through the mark.
  `docs/metrics/trap-families.md:62-63` says *"a held-out task has no `extern`
  and no `use`"*. That is the page's statement; I did not open the sealed
  reserve. The `spec` arm reads the live spec (`thesis_prompt.hero:36`), and the
  two variants were sealed at `a4e09e061`. The spec has changed in 10 commits
  since then (`git log --oneline a4e09e061..429464c94 -- spec/heroes-spec.md`),
  so the drift is older than this rename, which adds one more line to it.
- **D5. Where the refresh is pasted.** `spec-shape.md:154` and the
  spec-warden's § 8 name `selfhost/cli/measure.hero`. The pin is in
  `selfhost/measure/pinned.hero:55,64` and `tests/harness/suite_spec.hero:174,225`,
  which `29c6efeec`, the last spec landing, moved with the ledger. A landing
  following either text looks in the wrong file, and the rule's line is
  stale.
- **D6. The editor extension** is *"not on a marketplace, so nothing updates
  it for you"* (`editors/README.md:82`), and its version is the language's.
  Until a user reinstalls, their editor paints `pkgconfig` as a name, which is
  a reason to say it in the release note.

## E. Between the seats, and which is checkable

- **E1. The spec's cost**: compiler-engineer +4, spec-warden −6. These are
  different texts, not a disagreement (B10), and both are checkable from
  their diffs.
- **E2. "One token"**: the compiler-engineer's column says `pkgconfig` is one
  token, against `pkg-config`'s three. That is the Heroes lexer's token. The
  spec-warden's +1 per occurrence is the vendored tokenisers'. Both are right.
  A synthesis quoting "one token" must name the unit.
- **E3. What `package` gets after the rename**: the spec-warden inferred it
  from today's compiler meeting an unknown word and said so (§ 7). The
  compiler-engineer built it (`old_package_word`, exit 1, certain fix, then
  exit 0). The built answer supersedes the inference. No conflict.
- **E4. Counts rerun at the base and agreeing**: the marker on 40 lines in 6
  files; `.package` on 16 lines in 12 files; `system` 113, `library` 1086,
  `pkgconf` 16 and `pkgconfig` 2 in `selfhost/`; `fcec76d8d`, `88077be1a` and
  `a6832251f` as described, the last reading *"19,186, every one the
  migration's"*; `design.md:2437` (codes renamed from `1.0.0` never, panel
  232's R9); `old_cell_symbol`'s message citing *"until panel 209"*, the
  precedent for `old_package_word`'s *"until panel 240"*. The one count that
  fails is the precedent's 62 (B1).

## F. The re-read at the end (22:44 to 22:48), and what the landing's net must still run

The compiler-engineer's report grew from 493 to 572 lines (22:44). The diff
was saved again at 22:36. The live worktree read *"110 files changed, 431
insertions(+), 342 deletions(-)"* plus 6 untracked new-case files at 22:48.
The changes since my first read are folded into A2, A4, A5, A7 and C5 above.
At 22:48 the five A1 survivors were still in the prototype.

**Forms of the net with no prototype evidence** (the seat's report, and
`ls build/p240/proto*` against the suite names in `tests/harness/main.hero`):
`run` whole (the base's own was stopped at about four fifths, and the
prototype ran 9 cases through 7 filters), `corpus` (`examples/`, raylib and
SDL included; base only, 71/0), `seed`, `determinism`, `descriptors`,
`wholes`, `warnings`, `lines`, `ir`, `emit`, `runtime`, `cache`, `units`,
`unseen`, `control`, `latency`, `fetch`, `packages`, `package_cards`, `shape`,
`generated` and `thesis`. Most cannot meet the mark. But the seat's
**veto condition** is *"the landing's whole net shows a case changing verdict
that no rewritten expected file explains"*, so it can only be judged by a
whole-net run at the landing, and the synthesis should say that run is owed
before the commit, not after.

## Summary, landing-blocking first

1. **A1**: five C-sense *package* messages survive the prototype, among them
   `package_units.hero:298`'s note, which would tell users to write a
   `package` group, the refused spelling, in the very sentence the rename
   exists to fix. Four expected files pin it, so the net stays green.
2. **A2**: the seed. Locally, the net's own tests read 6 failed with the old
   seed's stage 1 against 2 with stage 2. The seat recommends refreshing the
   seed in the landing (measured `one-stage`, `cmp` silent), against the
   written rhythm. No resolution decides it, and the seat's own report says
   both.
3. **A3**: R5's route (`check --apply`) moves no expected file. The prototype
   used substitution scripts, and the precedent regenerated 23 files and
   paired them line by line. The route and its audit must be named, or a
   landing reaches for the regenerator forbidden in `check/`.
4. **A4, A6**: the 11 emissions to bless, `docs/design.md` (7 lines, one split
   across a line break), `platforms.md`, `trap-families.md`,
   `M-core-packages.md`'s planned `package "libcrypto"`, and 15 live comments
   are in no resolution.
5. **C1**: the spec-warden's general refusal for any word in the mark's
   position is listed, not adopted. The prototype catches four exact
   spellings; `Package`, `PKGCONFIG`, `pkgconf` and the rest get advice
   measured to point the wrong way.
6. **C2**: R6's check, written "both ways" over every mark, is red at the
   base on `rest` and `zero`. Its scope and its order (instruments first,
   `spec-shape.md:149`) need saying.
7. **B1**: the precedent moved 453 golden files, not 62 (`--stat`
   truncation). **B2**: the historian's prediction and conditions are
   dropped. **B3**: Zig is "the same motive", not "exactly this shape".
   **B7**: the author's yes for `--refresh` is in no file of the sitting.
   **B10**: `@@CE_COST@@` must not be +4, which is the coordinator's prose
   and not T1. **B13**: the push publishes the site.
8. The rest: B4 to B6, B8, B9, B11, B12, C3 to C5, D1 to D6, E1 to E4.

## G. Against the synthesis as revised at 22:48

The coordinator revised `synthesis.md` at 22:48 (5 `@@` placeholders left)
and took in B1 to B9, B12, B13, C1, C2, C4 and D1 to D6. Read whole again,
this is what remains or is new:

- **G1. The seed (A2) is in no resolution and no open question.** That is
  still the most likely way for the landing to go red on this Mac. Locally,
  `./heroes` is the old seed's stage 1, and with it the net's own tests read
  6 failed against 2. The synthesis must choose (refresh it in the landing,
  emitted after R7's pin moves; or run the third suite with `HEROES_COMPILER`
  set to stage 2, as the CI does) and say which way it went.
- **G2. R4's third clause and R3's "every user-facing message" are adopted
  unbuilt.** The prototype has neither the general message for *any other
  word in that position* nor the five survivors of A1 respelled. The
  compiler-engineer's brief says *"a route that does not build is not
  adopted"* (`compiler-engineer.md` brief, item 5). R4 should name the code
  the general message carries (`expected_end_of_line` reworded, or a new code,
  which is a diagnostic class this sitting would then be ruling) and the
  golden case that fires it. The suite counts the synthesis will quote from
  the prototype measure a smaller change than the one adopted, so the
  landing's whole net, the seat's own veto condition (§ F), is what judges the
  adopted one.
- **G3. R5's route lost the precedent's audit.** *"the expectation files
  follow the new compiler's output, each read before it is written"* is a
  regeneration with a human read. What `fcec76d8d` added, and what makes it
  other than the regenerator `diagnostics-and-goldens.md:72-74` forbids in
  `check/`, is the mechanical pairing: *"every changed line pairs with its old
  self once the symbol and the column are normalised, but the two messages
  this step reworded"*. Here that is: every changed line, once
  `pkgconfig`→`package`, the three codes and the marker are put back, and the
  column after a mark is moved two left, equals its old self, except the
  rewordings R3 names. The prototype's `rename_goldens.py` is exactly that map
  run forward, so the audit costs one inverse.
- **G4. R5's emissions: "their `hero_program_sources` line the one that
  moves"** is wrong for two of the eleven. The seat measured that the two
  `396-sha256` cases also move byte offsets in their `heroes-ffi-buffer`
  assertion text (`md 695 697` -> `md 697 699`). The precedent read that class
  line by line. Also, *"beside batch 27's eleven"* can be read as the landing
  blessing the round's own 11 *"nothing blessed"* cases (defects 2388, 2389,
  2395), work that is not this rename's.
- **G5. Live documents (A6) are still in no resolution.** R6 lists readers.
  The source of truth, `docs/design.md` (`ffi_package` at 3099, 3116, 3129 and
  3167, the mark at 2746 and 915-916), and `.claude/rules/platforms.md:42`,
  `docs/metrics/trap-families.md:62`, the next milestone's planned `package
  "libcrypto"` (`M-core-packages.md:32`) and the 15 live comments of A6 are
  named nowhere. R8 names `docs/design.md` only as a reader of file names.
- **G6. D3 is now stated as a fact it is not.** The synthesis's *Left open*
  says `check --apply --in-place` *"would write into a fingerprinted
  `heroes.packages/` tree"*. My D3 asked whether it does. I did not find out:
  no guard turned up in `cli/certain.hero`, `apply_answer.hero` or
  `agreed_writes.hero`, but that does not show `--apply` reaches a fetched
  tree. It should stay a question, or be run.
- **G7. "the author's standing yes for this evening's pushes."** No file of
  the sitting records it (`grep -rn` over `p240/` finds only this sentence).
  CLAUDE.md's hard stop says a push that publishes the site is *"asked for,
  every time"*, with one exception: a session the author leaves unattended,
  asked once as they leave. If the yes exists, the record should quote it
  with its time, as the header now quotes the 22:15 yes for the refresh.
- **G8. Small things.** R6's check pairs the `Extern` marks with two
  constants named by hand. It catches a respelling on either side, and a
  third mark added to the spec, but not a third mark the parser reads and
  the spec does not, unless it reads `group_head.library_word`'s arms. And
  the account of what the compiler-engineer measured still omits the one
  silent reader that was **run** (A5, the emission SKIPs), while naming the
  two that were only read.
