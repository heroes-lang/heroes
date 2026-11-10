# Panel 242, the spec-warden's report

Written 2026-11-10, as the checks ran, by the spec-warden seat. Read-only against the trunk
`/Users/joseph/Temp/heroes/heroes-lang` at `1edabb9573a00db79fadda2bff695e98371a3290`
(`git rev-parse HEAD`), `git status --short` empty at the start. Compiler:
`.claude/worktrees/probe-2419/heroes-probe`, `heroes 0.6.0`, `sources 45824e7205ca021e`
(`--version`), the tree's key per the shared brief. No paid run: `measure` was run without
`--refresh` throughout. Scratch: `.claude/worktrees/p242-scratch/spec-warden/`.

## 0. The ceiling, read today and not from my brief

`grep -n '1\.6' docs/design.md` -> `253:### 1.6 The spec budget`; `sed -n 255-257p` reads *"must
fit in 16384 tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*. Two
rules come with that raise (design.md:289-293): the consolidation block spends at most **1,500 real
tokens** before the thesis harness has run, and the payment rule is unconditional (a named removal
or a registered prediction naming an instrument that exists today, design.md:318-333).

## 1. The measurement

```
$ cd /Users/joseph/Temp/heroes/heroes-lang && .claude/worktrees/probe-2419/heroes-probe measure spec/heroes-spec.md
  claude-legacy      8167   (64995 ranks)
  cl100k_base        8314   (100256 ranks)
  maximum            8314   a lower bound, not the reader's tokeniser
  spread              147   between the two vendored tables (1%)
  real              10898   claude-opus-5, 2026-11-09 — the binding number
  Headroom: 5486 against the 16384 ceiling — but the FFI floor mortgages 60 of it (panel 030 R3),
  so what is measured against the ceiling is 10958
exit 0
```

- **The real row is of these bytes, not recalled**: `selfhost/cli/measure.hero:117-121` prints
  `STALE` and withholds the verdict when the file's digest is not `pinned.SPEC_DIGEST`
  (`18bc6ed030668b91`, `selfhost/measure/pinned.hero:63-64`); no such line printed and the exit was
  0. The spec's last commit is `fd15c2280` 2026-11-10 00:22 (panel 240's landing), the pin's file
  moved in the same commit (`git log -1 -- spec/heroes-spec.md`, `-- selfhost/measure/pinned.hero`).
- `build/`'s mtime read `Nov 10 12:07:50` before and after the run (`stat -f '%Sm' build`), the
  critic's slip time; the measure wrote nothing; `git status --short` empty after.
- **Binding**: 10898 real, + 60 FFI floor = 10958 against 16384; **5426 free**.
- **The ledger agrees with the instrument**: `docs/measurements/010-spec-budget-ledger.md:242`, the
  newest row (panel 240's landing), reads *"8314 vendored (... legacy 8178 to 8167) and **10,898
  real**"*, the three numbers the instrument printed.
- **CLAUDE.md, the contract's own** (my brief asks whether (A) touches § 13's line): `heroes-probe
  measure CLAUDE.md` -> legacy 6236, cl100k 6347, maximum 6347, **real 8359** (`claude-opus-5`,
  2026-10-29, the contract's pin carries no digest by panel 134's measured refusal,
  `selfhost/measure/pinned.hero:66-81`, so that real row is the last refresh's and not a count of
  today's bytes), headroom 3929 against the contract's 12288; exit 0. § 13's line is
  `CLAUDE.md:278-279` (`grep -n 'standard library' CLAUDE.md` -> 279).

## 2. Spec § 13's sentence and question 3: no word is owed, by the sentence's own rule

The brief asks whether moving `hero_env_put` into `hero_os.h`, or route (e), needs *"and the
environment"* at spec:374-375.

- **The sentence is a ranked gloss with a stated rule, not a list of the header.** It entered at
  `2f012152a` 2026-10-27, decision 2228 (`git log -S"threads, processes and files" --
  spec/heroes-spec.md`), and its ledger row says why those three words:
  *"the three families the programs binding it bind most, counted that day outside the recovery
  corpus (threads 39, processes 38, the file system 58 bindings, the error stream 3)"*
  (`010-spec-budget-ledger.md:235`). The error stream, bound 3 times, was left unnamed by that
  rule.
- **The environment was already in the header when the sentence was written, and was left out**:
  `hero_env_shown` is `runtime/hero_os.h:84`, entered `815bcbb8b` 2026-10-04 (defect 243, `git log
  -S'hero_env_shown' -- runtime/hero_os.h`), 23 days before. The header also declares arguments
  (`hero_args_*`), `hero_os_pid`, `hero_word_bits`, `hero_print_str` and `hero_program_sources`
  (`grep -n '^[A-Za-z].*hero_[a-z_]*(' runtime/hero_os.h`), none named by the three words.
- **The environment's bindings today, by the same ruler**: 7 in the whole tree, 2 readers of
  `hero_os.h` and 5 writers of `hero_compiler.h`, 0 in `examples/` (`git grep -n 'function hero_env_'
  -- '*.hero'`: `selfhost` 4, `tests/harness` 3). Moving the writer into `hero_os.h` takes the
  family to 7 bindings of that header against the 38 of the least-bound family the sentence names;
  route (e) removes the 5 writers and adds a staging door of the spawn, which is the *processes*
  family by name.
- **Priced anyway**, vendored, on scratch copies in `p242-scratch/spec-warden/`: *"threads,
  processes, files and the environment"* and *"threads, processes, their environment and files"*
  each read legacy 8170, cl100k 8317, maximum **8317 (+3)** against the base copy's 8314
  (`heroes-probe measure <copy>`, three runs, `diff` showing line 375 alone changed). The real
  price is **unrun** (a `--refresh` is a paid call); at today's ratio, 10898 / 8314 = 1.31, it is
  about +4 real, an inference and not a measurement.
- **Verdict on the spec for question 3: approve every route at 0 spec tokens; no blind seat is
  owed.** The word would be owed when the environment's bindings of `hero_os.h` outnumber those of
  one family the sentence names, which is the sentence's own rule, and that is the condition.

## 3. Question 1, the compiler carrying a package

### 3.1 Tokens: no route moves the specification or the contract

- The spec says nothing about what the compiler is built from: `grep -n -i
  'compiler\|standard library\|prelude\|ships\|shipped\|built into\|compiled into'
  spec/heroes-spec.md` -> two lines, 369 (`???`) and 452 (`owned`), neither about the compiler's
  makeup. § 1's paragraph (spec:29-37) gives a program's catalogue *"beside the file you compile"*;
  the compiler's root is `selfhost/main.hero`, so `selfhost/heroes.toml` is that file under (A)
  and § 1 already covers it, and *"a program that uses no package has no catalogue"* covers (B).
  **(A), (B), (C), (D): 0 spec tokens.**
- **CLAUDE.md § 13's *"A standard library"* (`CLAUDE.md:279`) is untouched by (A)**: panel 234's
  spec-warden defined the harm the line names, for B5, as *"every function becomes a name the
  compiler offers to every program without a file in its tree, which is what a built-in is to a
  reader"* (`docs/panel/234-reports/spec-warden.md:267-270`); (A) offers no name to any program
  (3.2 below, measured). **0 CLAUDE.md tokens under every route of question 1.**
- **Principle 0 does not reach (A), by a ratified ruling**: panel 234's R1 reads *"Principle 0
  binds no package and no function of one, and binds every form, built-in and sentence of the
  specification the milestone would add"* (`docs/panel/234-the-core-*.md:272-274`, R1). (A) adds no form, no
  built-in, no production, no diagnostic class and no spec token. Panel 241's R1 used Principle 0's
  words of route 1 (*"Principle 0 admits a form the compiler needs or one with a measured effect;
  this one has neither"*); by 234's R1 that is the burden by analogy, not the rule. **So my veto on
  Principle 0 has no form to fall on.** The burden that binds (A) is design.md §1.7's criterion,
  *"does it ... remove a special case from the compiler?"* (design.md:437-438), and
  panel 241's ratified R3, *"only scoped to **replace**"*. The sitting should say which of the two
  readings governs, since both are ratified.

### 3.2 Question 1 (i): (A) is outside R1's veto of B5, read back to the source

Read to the source, in order:

1. **Panel 097's condition 5** is *"Nothing reaches `selfhost/library_source.hero` — the prelude
   is the standard library design.md §1.11:443-445 forbids permanently"*
   (`docs/panel/097-*.md:129-130`): the source every program is given.
2. **B5** is *"the canonical tree compiled into the compiler, panel 028's P-embed applied to
   `core/`"* (`docs/panel/234-briefs/00-shared.md:200-201`), vetoed as *"that reason applied to
   twenty packages rather than one file"* (`docs/panel/234-reports/compiler-engineer.md:180-187`).
3. **R1's word is *shipped***: *"**No core package is shipped with the compiler or the runtime**:
   route B5 is vetoed"*; its falsifier, *"a core package that cannot work unless the compiler or
   the runtime ships it"*.
4. **R2 names the compiler among the programs that carry packages**: *"a program over the packages
   outside `examples/` (the hooks of issue 2082, the compiler's copy of R13) owes the suites that
   judge it"*. R13's *"copy under `selfhost/core/`"* was R2's layout of that day; panel 235's R15
   replaced that layout by the catalogue and `heroes.packages/`
   (design.md:565-568), so **(A) is R13 carried through R15's amendment, not a reversal of B5**.
5. **The letter drifted in transcription, twice**: `git show 79a42b509 -- docs/design.md` (task
   2301, 2026-11-07) writes *"No package is compiled into the compiler or the runtime"*: *core
   package* became *package*, and *shipped with* became *compiled into*. `b8f29508b` re-wrapped it
   unchanged.

**Measured, the harm B5 meant does not come with (A)**: a program with `use <core>/strings` and
no catalogue, checked by the probe compiler in `p242-scratch/spec-warden/nocat/`, is refused
`main.hero:1:5: error[unknown_entry]: `<core>` names no entry: there is no catalogue
`heroes.toml` beside `main.hero`...`, exit 1: a package reaches a program only through that
program's own catalogue. That (A) leaves this byte-identical is an inference until a compiler that
carries a package is built; prediction P1 scores it.

### 3.3 A reading or an amendment: the amendment costs the record less

- **A reading** costs design.md 0 tokens today and leaves its letter refusing what the tree would do.
  design.md is reached by grep (CLAUDE.md's opening paragraph), so every later seat finds the letter
  first: the letter has already cost panel 241 a resolution (its R4, *"reads by its letter against
  any catalogue entry of the compiler's"*) and this sitting its first question. A reading in a
  panel file is a second place where the rule lives, which is CL-064's shape.
- **An amendment** costs one sentence of an unbudgeted document (only the spec and CLAUDE.md are
  judged, `selfhost/measure/judged.hero:27-28,130-131`), 0 spec and 0 contract tokens.
- **Under (C) the letter fails as well**: a copy under `selfhost/` is either *"a package compiled
  into the compiler"*, so R13 already contradicts design.md, or it is no package, so the letter
  allows the unverified copy (241's R3: *"a package with no fingerprint, told *unverified* on every
  check"*) and forbids the verified form. Ranking the less robust form above the more robust one is
  what CLAUDE.md § Precedence refuses. **(B) leaves R13 contradicting the letter** unless R13 is
  withdrawn.

### 3.4 Three wordings for design.md:570-572 under route (A)

Each replaces lines 570-572 (from *"No package is compiled"* to *"and none ships platform C of its
own:"*), spliced into a scratch copy by `p242-scratch/spec-warden/tools/splice.py` (it asserts both
anchor lines first) and priced vendored by `heroes-probe measure <copy>`: the base copy 105496
legacy, 108747 cl100k, **maximum 108747**.

**W1, the sentence read back to its source** (maximum 108846, **+99**; legacy +97):

> No package is shipped with the compiler or the runtime for programs: nothing a program reaches
> without an entry of its own catalogue is a package's code, and no package's source reaches
> `selfhost/library_source.hero` (panel 097's condition 5; panel 234's R1, whose word was
> *shipped*: such source is the standard library this section forbids). The compiler is a program
> like any other here, and may carry a package through its own catalogue, `selfhost/heroes.toml`,
> none of which a program it compiles can reach (panel 242). And no package ships platform C of its
> own:

*What it claims that a later program could falsify*: a program with no catalogue entry for `p`
compiling a `use <p>/...` because the compiler carries `p` (instrument: `heroes check`, the
`unknown_entry` refusal above, a `tests/golden/check/` case); or a package's source reaching
`selfhost/library_source.hero` (a `git diff` of that file).

**W2, W1 and the verifier held out of the verified** (maximum 108883, **+136**; legacy +134):

> ... none of which a program it compiles can reach; it checks every placed tree's fingerprint, its
> own included, with the runtime's SHA-256 door and never with a package's code, so no package
> verifies itself (panel 242). And no package ships platform C of its own:

*What it claims*: W1's, and that the compiler's fingerprint is never a package's code. True
today: `module/fingerprint.hero:11-14,23-25,32-33` hashes every file and the listing through
`hero_file_sha256` and `hero_text_sha256` (`runtime/parts/sha256.c:108,121`), and `cli/sha256.hero`
serves only placed-folder names (`module/placed.hero:112,117`), `check --json`'s digests
(`cli/check_json.hero:132`) and the cross-check test (`module/fingerprint.hero:76`). Falsified
by a commit after which `module/fingerprint.hero`'s `of` or `listing` reaches a module under a
`<name>/`, or by a byte edited in a placed tree compiling at exit 0 (instrument measured today:
a copy of `examples/ini` checks at exit 0, and with one newline appended to its placed
`strings.hero` it is refused `error[package_unread]: ... a file of it was changed, added or taken
out since it was placed`, exit 1, `p242-scratch/spec-warden/ini/`).

**W3, W2 and panel 241's R3 scope in the one document a step greps** (maximum 108918, **+171**;
legacy +168):

> ... so no package verifies itself; and it takes one only to replace, the step that adds the entry
> deleting the compiler's own copies of that package's contract (panel 241's R3, panel 242). And no
> package ships platform C of its own:

*What it claims*: W2's, and that no commit adds an entry to `selfhost/heroes.toml` while a
compiler module keeping a copy of that package's contract survives it (for `crypto/sha256`:
`selfhost/cli/sha256.hero`, 292 lines by `wc -l`, `use`d by three modules, still present after the
entry lands). A first draft of W3 also carried *"each version it takes a seed refresh"*, +178;
taken out because R3 sends that sentence to `seed/README.md`, and one rule has one home.

**My recommendation is W3**, the robust and complete one: the failure its last clause guards
against is measured (241's R1: route 1 *"deletes no line and replaces no function"*), and R3 lives
today only in a sitting's file, which no step greps. **The conservative one is W1**, which settles
the scope and nothing else.

### 3.5 What (A) would have to replace, and question 1 (ii)

- **Compiler need: none.** The compiler builds itself from this tree with no catalogue:
  `ls selfhost/heroes.toml` -> *No such file or directory*, and the probe compiler, `sources
  45824e7205ca021e`, is that tree's build. Feature 2315 is the driver, and 2315 is a milestone's
  acceptance condition (`gh issue view 2315`: OPEN, M-core-packages), not the closure list and
  not a Part 11 effect. **No Part 11 effect is measured for the compiler carrying a package**:
  what I searched is `grep -rln -i 'compiled into the compiler\|selfhost/heroes\.toml\|the
  compiler takes a copy\|compiler carr[a-z]* a package\|compiler takes a package\|compiler.s copy
  of R13' docs/panel`, which finds only panels 234 and 241 and their folders, and `gh issue list
  --state all --search '"compiled into the compiler" in:body'` -> `[]`.
- **Route 1's measured cost, split** (241's compiler-engineer report:15-16,110-113,270-280, and
  `git apply --numstat` of its `route1.diff` from the critic's extraction): fixed machinery,
  `provenance.py` +60 −5 and `test_hooks.py` +18 −2, four walks *"about 130 lines by
  `277676555`'s precedent"* (an estimate in that report, not a count), a fingerprint read on every
  check (+0.285 % for that tree), 13 seed `#line`s, a seed refresh per version; per package, the
  placed tree, 2,334 lines of which the compiler reached 368; deleted, **0**.
- **So (A) meets the burden only on a step that deletes a compiler module the package's contract
  duplicates**, which is 241's R3 and W3's last clause. The one candidate measured today is
  `selfhost/cli/sha256.hero`, **292 lines** (`wc -l`), `use`d by `module/fingerprint.hero`,
  `module/placed.hero` and `cli/check_json.hero` (`grep -rlE '^use cli/sha256( |$)' selfhost
  tests/harness`), 2 `test` blocks. **No such package exists**: `git ls-files packages | grep -i
  -c crypto` -> 0, and the index holds 0.1.0 and 0.2.0 (`cat packages/core/versions`), 0.2.0's
  files `encoding`, `strconv`, `strings` (`ls -R packages/core/0.2.0`). **(A) can therefore
  replace nothing until `crypto/sha256`, or `path/filepath` over defect 2406's copies, is written
  and in the index.**
- **The verifier among the verified (a question, unrun; my reading of the code, not a run)**: today
  no. The fingerprint is the C door's (3.4, W2). If `crypto/sha256` replaced `cli/sha256.hero`,
  the package's code would name the placed folders (`placed.hero:112,117`), its own folder's
  name included; a wrong digest there names a folder nobody placed, so the build is refused rather
  than misled (an inference). The circle opens only if the fingerprint itself moves into the
  package, the question 241's R5 sent here (*"whether its own fingerprint moves to (d)"*): then the
  test at `fingerprint.hero:76`, two implementations held to each other, is the only witness. W2
  and W3 refuse that move in words, and 241's critic's ~0.14 % of a check, unrun, is the price of
  keeping the door.

### 3.6 Verdicts on question 1

| route | verdict | spec Δ (vendored max / real) | CLAUDE.md Δ | design.md Δ (vendored max, no ceiling) |
|---|---|---|---|---|
| (A) | **approve, with W3 (W1 conservative); object to any step that takes a package and deletes no compiler module**, so its first step waits for a package that can delete one (none is in the index today, 3.5). No veto: no form enters (234's R1) and no budget moves | 0 / 0 | 0 | W1 +99, W2 +136, W3 +171 |
| (B) | **object**: it keeps a letter that drifted from its source in transcription, leaves 234's R13 contradicting it, and withdraws 2315 for that letter | 0 / 0 | 0 | 0, or R13's withdrawal |
| (C) | **object**: under the letter it is forbidden too or it ranks the unverified copy above the verified form (3.3) | 0 / 0 | 0 | the same amendment as (A) |
| (D) | **approve as the order** (question 4), not as an answer to question 1, which it postpones | 0 / 0 | 0 | 0 |

## 4. Question 2, the transport: tokens only

The spec names no seed, no build line and no transport. § 13's *"sockets ... comes from C
libraries; threads, processes and files from the language's `hero_os.h`"* already covers an `os`
package over `hero_os.h` (b) and an HTTP package over libcurl (c). **0 spec tokens under (a), (b),
(c), (d).**

- **(c) moves a judged document**: CLAUDE.md § Commands spells the seed's line,
  `CLAUDE.md:348`, *"clang -I runtime seed/heroes.c runtime/runtime.c -o heroes   # the compiler,
  from C alone"*. Its least edit, ` -lcurl`, on a scratch copy: maximum 6347 -> **6350 (+3)**,
  legacy 6236 -> 6239 (`heroes-probe measure`); under `DELTA_GATE`, 50
  (`selfhost/measure/judged.hero:162-163`). That edit also makes the comment beside it
  arguable, the Windows leg having no curl library (`ci.yml:517`, *"sqlite, curl and raylib are not
  there"*), and it runs against ROADMAP row 83, *"`heroes` needing nothing else installed"*
  (`ROADMAP.md:224`, scheduled). The probe compiler links `/usr/lib/libSystem.B.dylib` alone
  (`otool -L`). The site's `STAGE_ONE` and the other 335 files are the critic's count, not mine.
- **Verdicts**: (a) **approve**, 0 everywhere; (b) **approve on tokens**, 0, with defect 2413's
  `stdlib.h` binding (`cli/process.hero:66-69`) repaired by the lift rather than carried;
  (c) **object**: it is the one route of the sitting that moves a judged document and a promise
  the contract states, for an index read `curl` already does as a program; (d) **approve**, 0.

## 5. Question 3: see § 2 above

0 spec tokens under every route; no blind seat is owed by the sentence's own rule. On the routes
themselves the warden has no token to weigh. **Route (e), the environment given at the spawn, is
the one whose promise is smallest** (no process-wide write, no new door of decision 2228's header
beyond a staging call of the *processes* family), so it is the one I would put first, since a door
added to `hero_os.h` is promised for ever from `v1.0.0`; the cost and the Windows arm are the
ffi-pragmatist's to measure.

## 6. Question 4, the order: 0 tokens under every order

From the warden's seat the order that gives a measurement before a sentence: **(D) first**,
`path/filepath` admitted by a program outside the compiler (the net or the hooks of issue 2082),
which reaches no word of design.md:570-572; then `crypto/sha256` written as 241's R5 says; then
the compiler's first catalogue entry, in the commit that deletes `selfhost/cli/sha256.hero`, with
the design.md amendment in the same sitting's landing. An amendment that lands earlier is true but
has no witness until that commit.

## 7. Predictions, each with its instrument and its milestone

- **P1 (W1's scope)**: at the M-core-packages step that first writes `selfhost/heroes.toml`, the
  program in `p242-scratch/spec-warden/nocat/main.hero` (`use <core>/strings`, no catalogue),
  checked by the compiler that step builds, prints the same `error[unknown_entry]` at `1:5` and
  exits 1, as today. Instrument: `heroes check` (exists). Any other output falsifies (A)'s claim
  to be outside B5, and I would then veto (A) as B5 in fact.
- **P2 (the specification)**: the landings of this sitting, under any route listed except a § 13
  word for the environment, leave `spec/heroes-spec.md` at **8314** vendored maximum and **10,898**
  real, digest `18bc6ed030668b91`. Instrument: `heroes measure` (exists), at each landing commit of
  M-core-packages. Any spec diff in them falsifies it.
- **P3 (the contract)**: CLAUDE.md's vendored maximum moves by 0 under every adopted route but
  (c), and by at least +3 under (c). Instrument: `heroes measure CLAUDE.md`, at the landing.
- **P4 (W2's verifier)**: at M-core-packages' close, `module/fingerprint.hero` holds no `use <`
  line (`grep -c '^use <'` -> 0, as today) and a byte appended to a placed file of the compiler's
  own tree, in a copy, is refused `package_unread`, exit 1. Instrument: `grep` and `heroes check`,
  both existing.
- **P5 (W3's scope)**: the commit that adds `crypto/sha256` to `selfhost/heroes.toml` deletes
  `selfhost/cli/sha256.hero`, and `git diff --numstat` of `selfhost/` outside `heroes.packages/`
  shows a net loss of at least 250 lines in that commit. Instrument: git (exists). Milestone:
  M-core-packages, the step of 2315.
- **No rewrite-rate prediction is owed or possible**: the spec does not move, so design.md §1.2's
  real cost is 0 × (1 + r) under every route, whatever r is.

## 8. What would change my verdicts

- **Question 1**: P1 falsified (a program reaching a package the compiler carries with no entry
  of its own) turns (A) into a veto; a route of (A) whose honest wording needs a spec sentence
  (for instance a reader told that the compiler carries packages) turns it into a priced amendment
  and owes the blind seat; a step that takes a package and deletes nothing, or moves the
  fingerprint into a package, I object to whatever the wording; panel 234's R1 read as letting
  Principle 0 bind the compiler's catalogue would put the veto back on the table, and (A) would then
  fail it today (no compiler need, no measured effect).
- **Question 2**: a measurement that `curl` as a program cannot serve the index read (on the box or
  a CI leg) would move (c) from object to approve-at-its-price, the price then CLAUDE.md:348 and
  the site's `STAGE_ONE`.
- **Question 3**: the environment's bindings of `hero_os.h` outnumbering 38, the least-bound family
  the sentence names, would make the word owed (+3 vendored measured, real unrun) and the blind
  seat with it.

## 9. State left

`git -C /Users/joseph/Temp/heroes/heroes-lang status --short` empty and `rev-parse HEAD`
`1edabb957...` at the end, as at the start; the trunk's `build/` still dated `Nov 10 12:07:50`
(`stat -f '%Sm' build`), so nothing of mine wrote there. Every file I wrote is under
`.claude/worktrees/p242-scratch/spec-warden/` (the spec, contract and design copies, `drafts/`,
`tools/splice.py`, `nocat/`, `ini/`, `ini-edited.txt`) or this report. No `--refresh`, no paid
call, no GitHub write.
