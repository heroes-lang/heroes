# 240: a package is code others publish, and the mark that asks `pkg-config` is spelled `pkgconfig`

**Status** `retro-record`, a reduced sitting by the author's instruction of
2026-11-09; the resolution's spelling is `provisional — author ratification
pending`.
**Seats** compiler-engineer, spec-warden, historian; the completeness critic on
the briefs before any seat and on the reports after. No llm-ergonomist and no
paid run by any seat (the reduced sitting the author asked for); the one paid
call, `heroes measure --refresh` at the landing, was put to the author after the
seats had sat and answered about 22:15 the same evening, *yes, run the refresh*
(the briefs, written before, say it was excluded, true when they were written).
**Base** `429464c94012e0691dfa091e0b38070c5c9550f0`, tree
`9f5ae3df8732db56336f726265a199019e4ad8cd`.
**Briefs** `docs/panel/240-briefs/`, **reports** `docs/panel/240-reports/`.

## The decision taken

Asked whether the root folder `packages/` (panel 238's R10) was misnamed because
"in Heroes it is called a module", the author was shown the spec's words and
answered, about 21:30 on 2026-11-09, meant as: *fix it, one name for one thing,
before the tag, without a sitting or with a reduced one*. The words, as the
author accepted them:

- **module**: one `.hero` file (spec § 1, line 22);
- **package**: code others publish, which the catalogue `heroes.toml` fetches
  (spec § 1, lines 31 to 37); so `packages/` and `heroes.packages/` keep their
  names, and the author's question had found no error there;
- **library**: C code a build links (`link "sqlite3"`, spec § 13 line 389).

What broke the rule was spec § 13, lines 479 to 482: `extern "raylib.h"
package "raylib"`, *package* for a C library `pkg-config` is asked about, the
clause panel 050 wrote on 2026-08-14 and panel 235 named and left unruled.

## The proposal, verbatim

Rename § 13's mark so that *package* means one thing; the coordinator's
candidate `pkgconfig`, with the diagnostic codes of that sense renamed to
match, no alias and no old spelling kept (CLAUDE.md § 2). The name was the
open question: `pkgconfig`, `system`, `library`, or one nobody listed.

## The verdicts

| seat | verdict | name | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | approve, with conditions; object if `library` is chosen or `pkgconfig_spelled` dropped | `pkgconfig`, the near spellings told | prototype: 116 files, 110 modified (+431 −342), 6 new; `parse/` at its budget, 8782 of 8782; its spec hunk (+4 vendored) is the coordinator's withdrawn prose, not T1 | panel 187's R2 over the frozen recovery corpus reads no finding caused by the rename | the near spellings (`pkg-config`, `pkg_config`, `pkgConfig`, `PkgConfig`) told by `pkgconfig_spelled` with a `certain` fix |
| spec-warden | approve, provisional on the real row | `pkgconfig`, in T1's wording | 8325 to **8319** vendored (−6); real unmeasured | W1: the landing's refresh reads T1 at 10,890 to 10,896 real, above 10,900 falsifies | a check holding the spec's `Extern` marks to the compiler's `WORD_*` both ways; the refresh and its pins in one commit; the wrong-mark message names both words and no message reads "this pkgconfig's name" |
| historian | approve (advisory) | `pkgconfig` | — | no program needing a second lookup mechanism by M-core-packages' `crypto/sha256` tag, else the name is re-sat | the error refusing `package` names `pkgconfig`, Rust 0.8's shape |

## What each seat measured

**The compiler-engineer** (`240-reports/compiler-engineer.md`). Of the
fourteen codes saying *package*, three are of the C sense: `ffi_package`
(`emit/ffi_build.hero:285`), `package_comparison` (`head_package.hero:76`) and
`no_pkg_config` (`cli/libraries.hero:308`); eleven are the fetched sense's and
keep their names. Twelve `selfhost/` files say *package* in their names, six of
each sense, confirmed by reading each head. The mark is read through
`WORD_PACKAGE` (`keywords.hero:83-84`, `parse/group_head.hero`,
`heads_told.hero:68`) and matched as the AST case `.package` on 16 lines in 12
files. **The precedent is a surface spelling**: `fcec76d8d`, panel 209,
2026-10-10, `@` to `@=`, the old form told once by `old_cell_symbol` with a
`certain` fix and the tree moved by that fix, `check --apply --in-place`: 453
files under `tests/golden/` in a commit of 1,181, 48 of them expectation files
(`git show --name-only --format= fcec76d8d`, the critic's recount; the seat's 62
was `--stat` shortening long paths), with its sequels `88077be1a` and
`a6832251f`, each a miss of that landing repaired a day later. A prototype in its worktree, 116 files, 110 modified (+431 −342) and 6 new,
refuses `extern "zlib.h" package "zlib"` with `old_package_word` at exit 1 and a
`certain` fix writing `pkgconfig`, after which the file checks at exit 0, formats
byte for byte and runs, asking `pkg-config` for zlib; and it tells
`pkg-config`, `pkg_config`, `pkgConfig` and `PkgConfig` with
`pkgconfig_spelled`, since a mark named after the tool invites the tool's own
spelling, which the lexer splits into three tokens. Readers measured against
the prototype: `grammar` stays green with the spec unchanged (in silence),
the TextMate grammar turns `spec/colouring` red, `highlight.ts` and
`examples-model.ts` would stop matching in silence by reading. Run on the prototype's stage 2, built in two stages from the seed: the
compiler's own tests 1989 of 1989 and the fixpoint by `cmp`; `check` 755/0,
`unsupported` 429/0, `permissive` 23/0, `full` 73/0, `annotations` 1345/0,
`fixes` 1077/0, each the base's count plus the seat's two new cases; `layout`,
`canonical`, `probe` and `surface` green; the net's own tests 584 with 1
failed, the round's latency test, which pinned decision 2282's old ceilings
and is repaired in the round (`b7aaa1db1`); `compat` over v0.6.0 refusing
exactly the raylib and SDL examples with `old_package_word`, both healed by
their `certain` fixes alone, *all ok*. Costs the brief did not count: the old
advice ("name a `package` and let `pkg-config` answer") stands in 27 expected
files of cases holding no mark; 27 diagnostic locations and their carets move
two columns; 11 blessed emissions move, nine at the source key alone and two
(the `396-sha256` cases) at byte offsets too. With `./heroes` built from the
old seed, which CLAUDE.md § Commands' first line builds, four of the net's own
tests fail, each building a program in the new spelling with a compiler that
refuses it; the CI names stage 2 and is unaffected. The seat's report
recommends the seed's refresh in the landing at its line 317 and puts it at
the next release at line 347; R9 decides.

**The spec-warden** (`240-reports/spec-warden.md`). 23 drafts of § 13 priced,
stage 1 and stage 2 agreeing on the 18 of its § 2, T1 to T5 read by stage 2
alone. `pkgconfig` and `pkgconf` cost one vendored token more per occurrence
than a one-token word, so T1's shape is +2 over the cheapest; T1, *"Instead of `link`, `extern "raylib.h"
pkgconfig "raylib"` asks the system where its headers and libraries are and
what else it needs."*, reads **8319**, six fewer than today, the removed clause
paying for the mark; keeping `package` and explaining it costs +7 to +11. Of
the seven names the tree writes after the mark, six are not the name `link`
takes (`pkg-config --libs` on this Mac), so the mark says on the line which
kind of name the string is. Every candidate but `library` removes panel 235's
sixteenth concept; `library` moves it onto `link`. With the spec renamed and
the compiler not, `grammar`, `special`, `fixes` and `unseen` stay green, and the
compiler-engineer measured the other way round (`grammar` 9/0, the compiler
renamed and the spec not): the net cannot see the spec and the compiler part,
and does see the compiler and the editor's grammar part (`spec/colouring` red).

**The historian** (`240-reports/historian.md`), every line read on its page on
2026-11-09. No system of fourteen writes the bare word *package* in source for
a `pkg-config` lookup; six put the tool's name in the text, and the five that
offer a plain link beside a `pkg-config` route all name the second after the
tool. `system` points the other way (Odin's `system:` is a plain library).
pkg-config's own manual calls a `.pc` unit a *package*, pkgconf and CMake a
*module*: the tool's vocabulary is split between the two words Heroes has
already given away. Zig 0.11 renamed its old *package* to *module* "to free up
the term package to be used in the context of package management", in the
historian's words *the same motive this sitting has*, though a build-system noun
and not a mark in program text, and what an old `build.zig` was told is
unverified; Rust 0.8 refused obsolete forms with the new spelling and no alias.
Its prediction: by the tag of the milestone that lands `crypto/sha256`'s binding
(M-core-packages), no program in `tests/`, `examples/` or `packages/` holds an
extern head whose library `pkg-config` cannot find under the renamed mark and
that needs a second lookup mechanism; if one does, the name is re-sat. What
would change its reading: a Meson-style `auto`, one mark answered by more than
one tool, or a system that kept the bare word with no confusion.

**The critic** read the briefs before any seat (`240-reports/critic-briefs.md`)
and found seven framing facts wrong or misleading, repaired in place before the
seats started, the first drafts kept beside them as `*.before-critic.md`: the
file split by sense (wrong both ways), `no_pkg_config` missing from the codes,
`link "m"` cited as spec § 13's when it is `link "sqlite3"`, panel 235's
paragraph cited as a section, the re-printers mixing three senses,
`WORD_PACKAGE` unseen by the grep, and `lexed.ts` listed though it never says
the word; and it named the stale real pin, the silent readers, the taken words
and two routes nobody listed. Its second pass (`240-reports/critic.md`) corrected this
file in thirteen places (§ B: among them the precedent's 453 files, not 62, the
historian's dropped prediction, the stage that read T1, the unrecorded yes),
carried five conditions into the resolutions (§ C), found user-facing C-sense
*package* the prototype had left in five messages, one pinned by four expected
files (§ A1), and asked what the compiler's own words call `pkg-config`'s unit
(§ D1), which R2 now answers.

## The resolution adopted, provisionally

`provisional — author ratification pending`. The most robust and complete
resolution (CLAUDE.md § 4):

- **R1, the words.** *module* is a file, *package* is what the catalogue
  fetches, *library* is C code a build links. `packages/` and
  `heroes.packages/` keep their names.
- **R2, the mark.** § 13's mark is spelled `pkgconfig`, in T1's wording:
  *Instead of `link`, `extern "raylib.h" pkgconfig "raylib"` asks the system
  where its headers and libraries are and what else it needs.*, and the
  production `( "link" | "pkgconfig" )`. T1 is chosen for what is not a
  number, one spelling of the mark in the document and no `pkg-config` beside
  `pkgconfig`, a vendored ranking judging no two drafts
  (`.claude/rules/spec-shape.md`); the refresh prices T1 alone. The name rests
  on the historian's V, Cabal, SwiftPM and cgo. In the compiler's own words what
  `pkg-config` is asked about is a *library*, and a line relayed from the tool
  stays verbatim (the critic's D1).
- **R3, the codes.** `ffi_package` becomes `ffi_pkgconfig` and
  `package_comparison` becomes `pkgconfig_comparison`; `no_pkg_config` keeps its
  name, which already names the tool; the driver marker becomes
  `heroes-ffi-pkgconfig`; `shadowed_words`' origin `PACKAGE` becomes `PKGCONFIG`,
  and its fallback with it, so no note says *package* in two senses; no message
  reads "this pkgconfig's name"; every user-facing message of the C sense is
  respelled, `cli/package_units.hero`'s `NOTE` among them, which four
  `fixedbugs-2351` expected files pin (the critic's A1). Codes may be renamed before `1.0.0` (panel 232's R9).
- **R4, the old spelling told, never kept.** `package` in a group's head is
  refused at exit 1 by `old_package_word`, its `certain` fix writing
  `pkgconfig`, and the head read on so the rest of the file is judged
  (`old_cell_symbol`'s shape); `pkg-config`, `pkg_config`, `pkgConfig` and
  `PkgConfig` there are refused by `pkgconfig_spelled` with the same fix; any
  other word in that position is told by a message naming the two words that
  may stand there, `link` and `pkgconfig`, in place of `expected_end_of_line`'s
  advice, which points the wrong way (the spec-warden's condition 3, the
  critic's C1); and `suite_compat.hero`'s `known()` takes the row that names
  the change.
- **R5, the tree moved by the fix.** Programs are rewritten by
  `check --apply --in-place`, as `fcec76d8d` moved its tree, and the expectation
  files follow the new compiler's output, each read before it is written, the
  fix moving no `.expected` file (the critic's A3); the eleven emissions whose source the rename edits are blessed again by name,
  nine moving at the `hero_program_sources` line alone and two (the
  `396-sha256` cases) at byte offsets too (A4), and batch 27's own eleven new
  cases are blessed in the same pass; the changed expectation lines are
  audited as `fcec76d8d` audited its own, each equal to its old self once the
  spelling, the codes and the two-column shift are put back (the critic's
  final pass, item 3); comments that quote
  past messages and the four golden case names that say *package* in the C
  sense stay, being records.
- **R6, every reader in the same commit.** The TextMate grammar,
  `site/src/lib/highlight.ts`, `site/src/lib/examples-model.ts`'s regex,
  `tests/harness/suite_emission.hero`, `absence.hero`, `probe.hero` and the
  fixtures that pin the codes or the marker; and **a check that holds the
  spec's `Extern` production's marks to `WORD_LINK` and `WORD_PKGCONFIG` both
  ways**, since no suite sees the spec and the compiler part; not every `WORD_*`
  against every production, which reads red at the base, `rest` and `zero`
  being constants no production quotes (the critic's C2), and not panel 133's
  refused shape, the grammar against the parser, but two word lists held to
  each other. The order the spec-warden asked, the instrument before the text,
  is kept inside the landing's one commit by seeing the check fire first.
- **R7, the count.** `heroes measure --refresh` on the spec, its pins
  (`selfhost/measure/pinned.hero`, `tests/harness/suite_spec.hero`) and its
  ledger row land in the rename's one commit, the spec-warden's condition 2,
  the coordinator making that commit after the lane's work is verified (the
  author's yes to the call, about 22:15 on 2026-11-09).
- **R8, the internal names.** The six C-sense files and the 293 identifiers
  that say *package* inside the C-sense modules are renamed in a mechanical
  commit of their own, a task of the next milestone; the compiler checks every
  identifier, so a missed one is a build error, while the file names are also
  read by path where no build looks (`tests/harness/absence.hero:197`, which the
  net's own tests catch; `suite_layout.hero:622`, the verification map's row
  and `docs/design.md`, which nothing checks), and the task names them.
- **R9, the seed.** Refreshed in the landing, by the one-stage route, emitted
  after R7's pin moves (the pin lives in `selfhost/measure/pinned.hero`), its
  fixpoint over two generations. M-bounded-seed's rhythm refreshes the seed
  where the two-stage build goes red or a release closes; here the two-stage
  build stays green, and what goes red is the first command CLAUDE.md §
  Commands gives, the compiler built from the seed, which reads four of the
  net's own tests red on programs in the new spelling. A documented command red
  is the reason recorded.

**What conservative would have been**: the mark kept as `package` and only the
prose changed, +7 to +11 tokens, the collision left in every program that
binds through `pkg-config`; or the rename without R4, the old spelling told by
`expected_end_of_line`, which names no new spelling and which `suite_compat`
could never list as a known change.

## Disagreements, not smoothed

- **The wording.** The coordinator's prose brought the tool's name in beside
  the mark (`pkg-config` and `pkgconfig`, one hyphen apart, +4); the
  spec-warden's T1 (−6) is adopted and the coordinator's withdrawn.
- **`system`.** Two tokens cheaper than `pkgconfig` in the spec-warden's
  pricing, and the spec-warden would move to it on a reader run showing it
  does as well; the historian measures it pointing the other way in precedent,
  and the word stands on 113 `selfhost/` lines in every sense, the toolchain's
  own headers (`cli/system_paths.hero`) among them. Adopted: `pkgconfig`; the reader run is not
  made (the reduced sitting), and W2 below is registered and unscored.
- **The internal names.** The author's *one name for one thing* reaches them;
  the compiler-engineer measures their rename as behaviour-free and colliding
  with every lane on `cli/libraries.hero`, and recommends a commit of its own
  after the tag. R8 adopts that order and owes it as a task.

## The prior art, per resolution

- R1: Python, npm, Dart, Swift, Cargo and Hex call the fetched unit a package
  (the historian's question 2); Zig 0.11 freed *package* for package
  management.
- R2: V `#pkgconfig` beside `#flag -l`, Cabal `pkgconfig-depends` beside
  `extra-libraries`, SwiftPM `.systemLibrary(pkgConfig:)`, cgo `#cgo
  pkg-config:`.
- R4: Rust 0.8's obsolete-syntax errors.
- R3, R5 to R8: none outside this project (`fcec76d8d` is this project's own).

## Left open, as questions

- **§ 13's sentence that JSON comes from C libraries** (spec line 374) beside
  § 1's `use <json>/parse`, *code others publish*, and design.md:556's JSON row:
  under *one name for one thing* the same example names a C library in one
  section and a Heroes package in the other (the spec-warden's § 4, the critic's
  C4). Filed as a task.
- **A fetched package that still writes `package`** (the critic's D3, a
  question): does `check --apply --in-place` write its fix into a fingerprinted
  `heroes.packages/` tree? The critic read three writers and found no guard,
  which settles nothing; no package in the index binds C today, so the
  exposure starts with `crypto/sha256`.
- **The C boundary's own seat was not convened** (D2, the reduced sitting):
  whether the mark is a tool's or a mechanism's is the ffi-pragmatist's
  question, carried meanwhile by the historian's prediction.
- **The editor extension** is installed by hand (`editors/README.md:82`), so a
  user's editor paints `pkgconfig` as a name until reinstalled: a line for the
  next release note (D6).
- **Where the refresh is pasted** (D5): `.claude/rules/spec-shape.md:154` names
  `selfhost/cli/measure.hero`, while the pin lives in
  `selfhost/measure/pinned.hero` and `tests/harness/suite_spec.hero`; the
  landing writes there and corrects that line.

## Predictions to score

- **spec-warden W1**: the landing's `--refresh` reads T1 at 10,890 to 10,896
  real; above 10,900 falsifies it and the row owes a named removal. Checkable
  at the landing.
- **spec-warden W2** (registered as an observation, unscored without a paid
  reader run): at least 8 of 10 blind readers write `pkgconfig "libcurl"` on
  their first try.
- **compiler-engineer**: at the landing round's gate, panel 187's R2 over the
  frozen recovery corpus reads no finding caused by the rename: each of the 34
  frozen programs with an extern head in the old spelling gains only its own
  `old_package_word`, which R2 subtracts; any finding on those programs'
  mutants falsifies it, and the plan is then re-cut as `a6832251f` did.
  Checkable at the landing's gate.
- **historian**: by the tag of the milestone that lands `crypto/sha256`'s
  binding (M-core-packages), no program in `tests/`, `examples/` or `packages/`
  holds an extern head whose library `pkg-config` cannot find under the renamed
  mark and that needs a second lookup mechanism; if one does, the name is
  re-sat.

## Process notes

- The sitting was convened while the round's gate B read its tree, so the
  briefs and reports were written in the round's scratch
  (`.claude/worktrees/b27-scratch-round/p240/`) and copied here at the
  synthesis, a deviation from `/panel` § 2 recorded as panel 234 recorded its
  own.
- Reduced by the author's instruction: three seats, no blind seat, no paid run
  by a seat; the critic sat both passes, the first repairing seven framing facts
  of the briefs before any seat started, the second correcting this file in
  thirteen places (`240-reports/critic.md` § B) and carrying five conditions
  into the resolutions (§ C); the first drafts are kept as `*.before-critic.md`.
- **The push that carries the landing publishes the site**, every path it
  touches being in `deploy-site.yml`'s `paths:` (the critic's B13); it is said to the author with the count of site commits before it travels
  (CLAUDE.md § Hard stops).
- The landing follows in its own commit, after this file and the decision
  issue, as CLAUDE.md § 4 asks.
