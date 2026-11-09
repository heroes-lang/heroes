# Panel 240: completeness critic, first pass (the briefs)

Read 2026-11-09, about 21:40 to 22:10, against base `429464c94`. Every command
is `git -C $R ...` with `R=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/b27-round`,
reading the commit and never the working tree. No compiler was built or run
(the round's gate owns that tree). There is no verdict on the rename here.

**The round moved while I read.** At the end, `git -C $R rev-parse HEAD` gave
`d723e92d6` (commits `0561a89e5` and `d723e92d6`, 21:45), and `git status
--porcelain` was empty, although `critic.md:7-8` speaks of uncommitted work.
`git diff --name-only 429464c94 HEAD` touches 6 files (`.claude/skills/graph/*`,
`ci.yml`'s Windows timeout, `cli/const_elements.hero`, `emit/shadowed.hero`,
`suite_latency.hero`). None of them is a path any brief cites, so every fact
below holds at both commits.

## (b) Wrong or misleading facts, first

1. **"five whose own names say *package* in the C sense"** (00-shared.md:85-87,
   carried into compiler-engineer.md:22). This is wrong in both directions.
   - `selfhost/cli/package_stand_in.hero` is of the **fetched** sense. Its head,
     lines 1-14: "What a package's units ... may read ... a fetched package's
     headers reading outside its places", citing panel 235's R9 and panel 237.
   - Two files whose names say *package* in the C sense are missing:
     `selfhost/head_package.hero` (80 lines: "What a `package` string may hold
     ... where `pkg-config` splits its argument") and
     `selfhost/cli/package_words.hero` (424 lines: "The words of a package's
     answer", panel 198). Neither is among the 20 files the brief's grep finds,
     because neither contains `"package"`, `.package` or `package:`.
   - From `git ls-tree -r --name-only 429464c94 -- selfhost | grep -i packag`
     and each file's head, the 12 files split as follows.
     - **C sense (6):** `cli/package_answer.hero`, `cli/package_macros.hero`,
       `cli/package_missing.hero`, `cli/package_words.hero`, `head_package.hero`,
       `emit/header_packages.hero`.
     - **Fetched sense (6):** `cli/package_stand_in.hero`, `cli/package_reads.hero`,
       `cli/package_units.hero`, `package_mark.hero`, `module/package_told.hero`,
       `module/packages.hero`.
2. **The thirteen codes are not the C sense's codes.** The regex
   (00-shared.md:73) only sees names that contain *package*. It misses
   **`no_pkg_config`**, a C-sense code (`selfhost/cli/libraries.hero:308`, test
   at :759; pkg-config absent or not startable). The 13 counts reproduce
   exactly, but compiler-engineer.md:11's question "which of the thirteen are
   of the C sense" needs `no_pkg_config` added. Codes that a `package` head
   emits and shares with `link` heads (`option_like_name`, `unwritable_name`,
   `machine_locked_path`, `ffi_missing_library`) do not need a rename, but
   should be named as checked: `head_package.hero:22,31,67`,
   `head_names.hero:50-162`, `cli/package_missing.hero:3`.
3. **"library = C code a build links (`link "m"`, spec § 13)"** (00-shared.md:38).
   The spec never writes `link "m"`: `git show 429464c94:spec/heroes-spec.md |
   grep -c 'link "m"'` gives 0. § 13's example is `link "sqlite3"` (spec:389).
   `link "m"` comes from the site (`site/src/html/docs/the-c-boundary.html:128`)
   and the compiler (`selfhost/head_names.hero:130`).
4. **"§ The word *package*, line 509"** (00-shared.md:50-51). Line 509 opens a
   bold-led paragraph, `**The word *package*.**`, not a `§` heading (the file has
   no such heading). The quoted words, "the collision with § 13 is not ruled",
   are on **line 513**.
5. **The list of files that "re-print or colour a program and hold the word"**
   (00-shared.md:89-96). The command reproduces exactly, but the files hold
   three different senses, and only some of them are the § 13 mark.
   (`git grep -nw package 429464c94 -- editors/ site/src/lib/ selfhost/print/
   selfhost/probe/ 'selfhost/mutate*'`)
   - **The § 13 mark:** `heroes.tmLanguage.json:77` (`(link|package)`),
     `editors/README.md:25,27`, `site/src/lib/highlight.ts:434,468`,
     `selfhost/print/fmt.hero:58,354,363,392`,
     `selfhost/print/dump.hero:165,233,237`, and
     `site/src/lib/examples-model.ts:179` (see d.2).
   - **The fetched sense only:** `mutate/op_decls.hero:264,268`,
     `probe/judge.hero:341`, `examples-render.ts:228`, `i18n.ts:44`, the rest
     of `examples-model.ts`, `fmt.hero:1393,1397`, `dump.hero:240,243`.
   - **The VS Code extension's own package (npm/VSIX):** every hit in
     `editors/vscode/extension.js`, `editors/vsix/[Content_Types].xml:2` (an XML
     namespace URL), `extension.vsixmanifest:25`, and
     `editors/README.md:10,38,64-70,96-99,381,383`.
6. **"the lexer's contextual marks (`selfhost/keywords.hero`)"**
   (compiler-engineer.md:15). `keywords.hero:66` says these are words "the
   parser reads in ONE position". The parser's sites are
   `selfhost/parse/group_head.hero:102,114,116,124` and
   `selfhost/heads_told.hero:68`. Neither is among the 20 files, because both
   read the constant `keywords.WORD_PACKAGE` (`keywords.hero:83-84,140`), not
   the literal.
7. **`site/src/lib/lexed.ts`** (compiler-engineer.md:19) holds no *package*
   (`git grep -c` gives 0). It is the site's check that colours each program
   as `heroes lex` tokenises it (`.claude/rules/diagnostics-and-goldens.md:134-137`),
   not a copy of the word.

## (a) Facts verified (command, then result)

- Base and tree: `rev-parse 429464c94 '429464c94^{tree}'` gives `429464c94012e0691dfa091e0b38070c5c9550f0`
  and `9f5ae3df8732db56336f726265a199019e4ad8cd` (00-shared.md:11-13).
- Seat worktrees: `worktree list` shows `240-compiler-engineer` and
  `240-spec-warden` detached at the base. The historian has none.
- Spec:22 "One file is one module". Spec:31 holds "`use <json>/parse` reaches
  the module `parse` of the package `json`", with the paragraph running to 37.
  Spec:479-482 match the quote word for word (`git show 429464c94:spec/heroes-spec.md | sed -n ...`).
- Panel 050 is `docs/panel/050-the-package-clause.md`, `retro-record — author
  decision, 2026-08-14` (its line 3). Panel 235's file exists at the long name.
  Journal 075 carries the question at `075-package-manager.md:149-151`.
  Panel 238's R10 is at its line 282: "The index moves to a root `packages/`".
- `heroes.packages/` is where fetch places trees (`selfhost/cli/table.hero:183-184`,
  `docs/design.md:566`). `packages/` exists at the root (8 files under `packages/core/0.1.0/`).
- `git grep -lE '^\s*extern "[^"]*" package "' 429464c94 -- '*.hero' | wc -l`
  gives **70**, and `-h` with `wc -l` gives **87**.
- The 13 codes and their counts reproduce exactly (same command at the base).
- `selfhost/head_names.hero:426` is `assert said(spelled: "\"zlib=1\"", what: "package") == "package_comparison"`.
- `git grep -lE 'ffi_package|package_comparison' 429464c94 -- tests/ | wc -l`
  gives **26**: 13 recovery, 6 unsupported, 2 check, 1 run, 4 harness.
- The 20 `selfhost/` files reproduce exactly.
- Pages with `package "x"`: exactly the two `the-c-boundary.html` pages (but see d.6).
- 8325 and 10,900: `docs/measurements/010-spec-budget-ledger.md:241` (row 8325,
  "10,900 real on `claude-opus-5`", priced 2026-11-08) and
  `selfhost/measure/pinned.hero:53-60` agree. Not re-measured: see (c).
- Panel 235 spec-warden § 8 is "Question 20: what a reader must learn,
  counted", and "a sixteenth concept" is at its line 430.
- Panel 012 is `docs/panel/012-spec-budget-3000.md`. Its lines 45-46 call for
  "a named removal or a pre-registered falsifiable prediction", and the ledger's
  "How to append" is at lines 20-21.
- design.md §1.2 is at line 190, §1.6 at 253, §4.17 at 2333 (with the
  `certain | guess` rule at 2382), and §4.19 at 2609.
- `.claude/rules/records.md:15-23` makes `tests/golden/` append-only, and
  `.claude/rules/diagnostics-and-goldens.md:72-74` forbids `UPDATE_GOLDEN`
  outright in `check/` and `ir/`.
- The suite names `check full run unsupported emission` all exist in
  `tests/harness/main.hero`. `crates/` is absent at the base.
- CLAUDE.md § 2's rule is at line 136 at the base, written by `1985cdfef`.

## (c) Not verified, and why

- **8325 and 10,900 as numbers measured tonight.** No compiler was built: the
  gate is running and the critic's instructions forbid it. The spec and the pin
  last changed together, in `29c6efeec` (`git log -1` on both paths). That the
  pin matches the base spec is an **inference**, because its digest is the
  compiler's own function (`selfhost/cli/digest.hero:22`) and I did not run it.
- The seed build taking "a few seconds" and stage 2 "about a minute" were not
  timed.
- The author's words of 2026-11-09 21:30 ("fix it, one name for one thing")
  appear in no record at the base. They are the coordinator's account.
- Whether any suite parses the 41 `docs/panel/**/*.hero` files that hold the
  spelling. If one does, the rename turns 41 append-only records red. I searched
  `tests/harness/` and `selfhost/mutate/` for walkers and found
  `selfhost/mutate/cut_corpus.hero:136-138` ("named so no walker of `.hero`
  files reads a program the language may move under"). That does not settle it.
- **Every precedent in historian.md:9-25 is outside the repository**: Zig
  `linkSystemLibrary`, Cargo's `pkg-config`/`system-deps`, Meson
  `dependency()`, CMake `pkg_check_modules`, Nim `gorge`, cgo
  `#cgo pkg-config:`, SwiftPM `.systemLibrary(pkgConfig:)`, Cabal
  `pkgconfig-depends:`, Crystal `@[Link]` "which calls pkg-config", and Q2's
  ecosystems. The brief states each as a premise ("which calls pkg-config").
  Each is the historian's to verify on the page.
  - The brief puts Dart and Swift under "call a file a module". What each
    calls one file is a question for the historian, not a premise; I did not
    run it.

## (d) Routes, candidates, tools and places the briefs do not list

**d.1 Candidate spellings nobody listed, and words already taken.** The
evidence for each is from the world; nothing here judges between them.
- **Spellings nobody listed:**
  - `pkgconf`: the implementation Homebrew and vcpkg ship (`ci.yml:410-427,
    456-462`; the compiler asks for `pkg-config` "by that name").
  - `machine` or `asks`: panel 050's own title is "the machine is asked, not
    told" (050:1), and `ast.hero:440` says `package` is "the program asking
    the MACHINE".
  - `pc`: the `.pc` file (`head_package.hero:5`).
- **Words the language or the compiler already uses for something else:**
  - `dependency` is taken by the fetched sense (spec:39 `[dependencies.json]`).
  - `library` is what `link` names: spec:375 "`link` a library", spec:479
    "instead of a library", the CLI flag `--library <dir>`
    (`cli/table.hero:110,119,127`), and the AST's `variant Library` whose cases
    are `link` and `package` (`ast.hero:444-454`).
  - `system` is already "asks the system" (spec:480), and the C library's and
    toolchain's own headers in the compiler (`cli/system_paths.hero:1-3`,
    `cli/shadowed_words.hero`).
- **The diagnostics take their noun from the mark.** `head_names.hero:123-126`:
  `noun("link")` is "library", and any other mark is its own word, so a
  package group is told "this package's name". Under `pkgconfig` the message
  would read "this pkgconfig's name" unless `noun()` changes. Under `library`,
  the `link` group and the renamed mark would share one noun.
- **Every candidate spelling is new to the spec's text.** The spec never
  names `pkg-config` (`grep -c pkg-config` gives 0). The candidate prose
  (00-shared.md:59) brings the tool's name in for the first time, which the
  spec-warden should price. On Windows the tool is vcpkg's pkgconf copied
  under that name (`ci.yml:454-462`).

**d.2 Routes not listed.**
- **The conservative route**: keep `package` and disambiguate only in prose.
  CLAUDE.md § 4 asks the synthesis to record what conservative would have been.
- **Folding into `link` with a qualifier** (for example `link pkgconfig
  "raylib"`). This removes a contextual word instead of renaming one, and the
  AST keeps its two-case variant (`ast.hero:436-438`, "one of two, never both").
- **Renaming the fetched sense instead.** The author's acceptance excludes
  this (00-shared.md:30-37); the brief could say it is ruled, not open.

**d.3 Readers of the mark missing from compiler-engineer.md:15-20's walk.**
Several match text and would silently stop matching after a rename.
- `site/src/lib/examples-model.ts:170-182` parses each extern line with
  `/\bpackage "([^"]+)"/` into `pkg`. After a rename it yields `null`, with no
  error.
- `tests/harness/suite_emission.hero:633-634` parses `" package \""` from
  extern rows, and :612 is the `pkg-config --cflags` of every `package`.
- `tests/harness/absence.hero:154` pins the message text "pkg-config is not on
  this machine, and `package \"".
- `tests/harness/suite_golden.hero:799,813,902,947` hold fixtures with the
  spelling, and one is annotated `#~ ffi_package`.
- The rule's own walk (`.claude/rules/diagnostics-and-goldens.md:108-138`)
  names entries the brief omits: the formatter's self-check
  `selfhost/cli/syntax_cmds.hero`, the IR and scope printers, and `heroes
  measure`. `heroes grammar` prints `keywords.contextual()`
  (`keywords.hero:140`).
  - `suite_grammar.hero:108-111` and `suite_spec.hero:3042-3050` read the
    `WORD_` constants as text, so they follow a renamed constant by
    themselves.
  - `suite_grammar.hero:436`: nothing compares the spec's `Extern` production
    to the parser.
- **Help and diagnostic text that tells the user to write the old spelling.**
  58 non-comment lines in `selfhost/*.hero` hold `package \"` or `` `package` ``,
  of which the main advice sites are:
  - `emit/ffi_build.hero:104,252`
  - `head_names.hero:132,222,225-226`
  - `parse/group_head.hero:124`
  - `cli/package_answer.hero:117`
  - `emit/header_reach.hero:121`

  After the rename these would advise a refused form. None of them is a `Fix`:
  no `Fix` writes `package` today, so the brief's "`Fix` replacements" find
  nothing to change.
- **The driver marker `"heroes-ffi-package "`** (`emit/ffi_lookup.hero:35-36`)
  appears in 6 `selfhost/` files, `seed/heroes.c`, two
  `tests/golden/unsupported` programs and `tests/harness/absence.hero`. Its
  user-facing form is "the package `zlib` cannot be looked up"
  (`cli/package_answer.hero:342`, `emit/ffi_build.hero:182`).
- `package_comparison` is in `diag.hero:228`'s `is_thesis_rule` list (Part 11's
  control arm, dropped under `--permissive`) and in
  `docs/metrics/trap-families.md:62`.
- `seed/heroes.c` holds the codes (3 lines) and the marker (2 lines), so the
  seed has to be regenerated.

**d.4 Goldens and fixtures the 26-file count and the `*.hero` pattern do not reach.**
- **The 70 files split as 41 `docs/panel/` records, 27 `tests/golden/`, and 2
  `examples/`.** The 27 are 11 check, 9 run, 4 unsupported and **3
  `permissive`**. The run line in compiler-engineer.md:33 omits the
  `permissive` form, and it also omits `spec`, `grammar`, `golden`, `seed` and
  `corpus`, which this rename touches.
- **The recovery corpus.**
  - 34 `tests/golden/recovery/corpus/**/*.hero.frozen` files hold the spelling
    on 35 lines.
  - `plan-singles.jsonl` and `plan-pairs.jsonl` hunks write it, for example
    `extern \"curl/curl.h\" package \"libcurl\" {`.
  - `corpus-messages.jsonl` names C-sense files.

  `selfhost/mutate/differential.hero:56-61` replays them. The question is what
  a rename does to a frozen corpus that is append-only.
- **Four golden case names say *package* in the C sense, and a case's name is
  a record**: `check/fixedbugs-216-a-package-names-one-package`,
  `unsupported/ffi-records-only-group-cannot-claim-its-package`,
  `unsupported/fixedbugs-226-a-package-not-installed-is-told-at-its-string`, and
  `unsupported/fixedbugs-237-a-package-name-holding-a-backtick`.
  So a reader of `tests/golden/` meets both senses whatever is renamed.
- **The thesis contexts.** `tests/thesis/contexts/spec-without-productions.md:392`
  and `spec-with-one-worked-diagnostic.md` are sealed copies of the spec at
  `a4e09e061`, their sha256 named by a pre-registration (`contexts/README.md:7-11`).
  They keep teaching the old spelling. Does a held-out run's context then teach
  a refused form?

**d.5 The stale pin, with no paid run tonight.** `pinned.hero:44-47,62-63`:
`SPEC_DIGEST "43a8cf64676f5a99"`, and a mismatch WITHHOLDS the verdict.
`cli/measure.hero:117-121,243`: `heroes measure` on a stale spec prints `STALE`
and exits **1**. Any § 13 edit without `--refresh` therefore leaves the real
row withheld. Both spec landings since `ca83d921d`'s `--refresh`
(`7ee862fcf`, `29c6efeec`) changed the pin in the same commit. Whether
`suite_spec.hero:721-733` (`spec/spendable`, which needs a sentence `measure`
prints) and the `contract` check go **red** on a stale pin is **unrun**. It
decides whether the rename can land tonight at all. 00-shared.md:8-9 says
only that "every verdict that rests on it says so".

**d.6 Live files naming the § 13 sense that no brief lists.**
- `docs/design.md`:
  - 2746 and 3093 name `package`.
  - 3099, 3116, 3129 and 3167 name `ffi_package`.
- `examples/README.md:87`; `examples/raylib/main.hero` (lines 6, 9, 54, 58);
  `examples/sdl/main.hero:10-11` (in a comment).
- `docs/roadmap/milestones/M-core-packages.md:32,89` and
  `M-reflection-verdict.md:53`.
- `.claude/rules/platforms.md:42`; `.claude/rules/diagnostics-and-goldens.md:146`
  (a dated history paragraph).
- `.github/workflows/ci.yml:340-457`, in comments only.
- `editors/README.md:25-27`.

**d.7 Other senses of *package* a reader still meets after the rename.**
- The OS package manager: `examples/raylib/main.hero:96`, and `ci.yml:370`
  "has no SDL3 package".
- pkg-config's own noun: `head_package.hero:22` "splits its argument into
  packages".
- The VSIX/npm extension package in `editors/`.

The spec-warden's concept count (spec-warden.md:15-18) is about the spec alone.
Should the sitting say which of these the rename means to remove?

**d.8 Both senses in one artefact, today.** `cli/package_units.hero:6-7` and
`cli/package_reads.hero:3`: "the folders its own `package` groups' answer
names". These are fetched packages whose groups use the § 13 mark.
`docs/roadmap/milestones/M-core-packages.md:32` plans `crypto/sha256` binding
`package "libcrypto"`. This is the sentence the rename exists to fix, and no
brief shows it.

**d.9 Questions for the historian beyond its list.**
- CMake's `pkg_check_modules`, already in the brief, and autoconf's
  `PKG_CHECK_MODULES` call a pkg-config unit a *module* in their own names.
  What noun do pkg-config's own documents use? That noun bears on both
  *module* and *package*.
- The brief asks "which word each puts in the program's text". Cargo's
  `build.rs` and `Cargo.toml` are not the program's text, so the in-source
  spelling, Rust's `#[link(name = ...)]`, is the comparable one.
- vcpkg and pkgconf (`ci.yml:456-462`) are absent from the list.

**d.10 Process facts the shared brief omits.**
- Every path this rename touches is in `.github/workflows/deploy-site.yml`'s
  `paths:`: `selfhost/**`, `spec/heroes-spec.md`, `site/**`, `examples/**`,
  `seed/heroes.c`, the TextMate grammar, `ci.yml`, `docs/panel/**` and
  `tests/harness/suite_spec.hero`. Pushing it publishes the site, which needs
  the author's yes with the count of site commits (CLAUDE.md, Hard stops).
- There is no earlier rename of a contextual word to take as precedent.
  `git log -G'constant WORD_' -- selfhost/keywords.hero` shows only additions
  (`4309e69e9`, `a3a04d4ee`, `d48c44e23`). For a code rename, the one
  commit-message hit is `c13d60f66` (defect 135, about a rename *fix*), which
  is not a code rename. compiler-engineer.md:29-31 assumes a precedent exists.
  That should be a question, not a premise.
