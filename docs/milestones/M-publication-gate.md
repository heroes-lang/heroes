# M-publication-gate — the last gate


The repository is **private** today and publishing is a hard stop that only the
author lifts (CLAUDE.md §14). This entry is the checklist that has to be true
first, and it exists because most of its items get worse the longer they wait.

**Overtaken in part on 2026-09-08 by `M-open-repository`** (author instruction;
this paragraph is written underneath rather than in place of the sentence above,
which was true when it was written). **The repository is public since
2026-09-08**, verified unauthenticated, and three of the bullets below were
banked by that milestone rather than by this one. **The licence re-check ran** and disagreed with what `NOTICE` said, which
is recorded there and in `vendor/tokenizers/README.md`. **The contribution
policy is in force**, and the finding is that neither half of it is a repository
setting: forking cannot be disabled on a public repository and pull requests
cannot be closed, so the policy is declared in `CONTRIBUTING.md` and a template
and `main` is protected instead. **The `main`-cannot-fail defect is measured**
rather than remembered, and it is in `docs/work/SCHEDULED.md (retired 2026-09-12)` with its three
exit codes.

**What this entry still owns is unchanged**: the thesis measured, the
compatibility paragraph and the `1.0.0` it governs, the trademark question, and
the outward act of the channels. Only `0.x`'s one sentence is published so far.
The gate is now the gate for *a finished thing being announced*, which was
always its subject, and no longer the gate for the source being readable.

**Already done, ahead of the milestone** (2026-08-11, because a repository
accumulates history and history cannot be relicensed retroactively): `LICENSE`
(Apache-2.0), `LICENSE-RUNTIME-EXCEPTION` — so a program compiled with Heroes
owes nothing for the runtime inside it — `NOTICE`, `README.md`, SPDX headers
across `runtime/`, and the attribution of the two vendored BPE tables.

**Still owed here:**

- **The thesis, measured.** Metric 2 has never run; §1.2's formula has two
  factors and only one is audited. The site and both books will state the claim,
  and stating it unmeasured publishes an opinion with a decimal point — the one
  thing §12 forbids, the author included. Not delegable: the held-out tasks must
  be author-written, or they measure the assistant's priors (panel 011).
- **A compatibility policy.** What v1 promises to somebody who writes code
  against it, in one honest paragraph. Silence reads as a promise.
- **The licence re-check on vendored material**, against the upstream
  repositories rather than against this project's recollection
  (`vendor/tokenizers/README.md` § Licensing).
- **The trademark question**, in the narrow form that actually applies: the name
  is a common word and does not worry anybody, but `site/`'s Aladdin Sane bolt is
  iconography attached to an actively managed estate. The style guide already
  keeps lyrics out; this is the other half, and it is cheaper to answer before
  publication than after.
- **Contribution policy in force** — the README's current answer ("issues yes,
  pull requests not yet") either stands or is replaced deliberately.
- **One defect that shows up on the second page of any tour**: `main` cannot
  fail, so a program that goes wrong still tells the shell it succeeded (queued
  from panel 030). Whatever M-ffi-ladder decides for `exit(code)`, this must not
  be true on the day the examples go up.
- **The outward act of M-install-channels**: the tap, the manifest, the flake and
  the image go where a stranger can reach them here and not before, and each is
  installed once more from its public address.
- **The version scheme is in force** — a number `heroes --version` prints and a
  formula can pin, with the compatibility paragraph above saying what it
  promises; `heroes 0.0.1` and milestone-named tags are what stood on 2026-09-03.
  **In force since 2026-09-07** (CLAUDE.md §14 § Release tags): `vX.Y.Z` tags,
  `v0.1.0` first, and what 0.x promises is one sentence. What this bullet still
  owes is `1.0.0`, which is the day the compatibility paragraph above is
  published and not before.
---

*******************************************************************************
**OPEN: 3**

- [ ] **M-publication-gate** | no suite builds a gallery file, and ten of the fourteen are named by no test | `examples/gallery/` · `tests/harness/suite_corpus.hero:44` · `examples/README.md`

    **Origin:** measured 2026-09-04 at the close of M-corpus-depth, which is
    also when it got this home: that milestone closed with this half unbuilt
    while the rest of the item went to the record, and `examples/README.md` was
    still filing it at the closed milestone. The gate is where it belongs
    because these are the files the site shows — measured 2026-09-04,
    `site/src/html` carries **160** `data-src` references to 16 files and
    **148** of them point into `gallery/`.

    `examples/gallery/` holds **12** `.hero` files and no `main.hero`, so
    `tests/harness/suite_corpus.hero` cannot reach the directory — its floor is
    deliberately one under the directory count for exactly that reason
    (`suite_corpus.hero:44`). Measured over `tests/` the same day:
    `gallery/00-first.hero` is named **31** times, `09-holes.hero` **5**,
    `01-points.hero` **once**, and the other **nine are named nowhere**. What
    does hold is worth keeping straight rather than overstating: every file
    checks clean, because `heroes mutate examples/gallery` is a `suite_surface`
    row and `mutate` refuses a corpus that does not compile, and every file is
    canonical byte for byte. **What nothing asserts is that a gallery file
    BUILDS** in the three configurations — `00-first.hero` by surface rows and
    `09-holes.hero` to exit 1 on purpose are the two exceptions, and the other
    ten are checked, formatted and never lowered.

    **Two shapes.** (a) Give the gallery a `main.hero` so the corpus suite
    reaches it. (b) Teach the corpus suite a directory with no `main.hero`,
    whose files are built one at a time. **Recommended: (b)** — these files are
    deliberately independent one-form programs, and a `main.hero` would be a
    program written for the harness rather than for a reader, which is the one
    thing this directory is not for.

    **Where to look also:** `examples/README.md` § What holds the gallery.
    **Why it matters:** the ten files a stranger reads first are the ten nothing
    compiles.

    **Re-verified 2026-09-10: STILL OPEN, every count STALE, and one half
    repaired.** `examples/gallery/` holds **14** `.hero` files, not 12
    (`12-interpolation.hero` and `13-lease.hero` joined), and **ten of the fourteen
    are named by no test**, not nine of twelve: `00-first` is named 31 times,
    `01-points` once, `09-holes` four times and `13-lease` five, all five of those in
    `suite_records.hero`, which is a document check and not a build. The site's
    slices are **166** `data-src` references to **18** files, **154** of them into
    `gallery/`, where the item says 160 / 16 / 148. **Already repaired**: the
    misfiling its origin note complains about is gone — `examples/README.md:150-153`
    files this at M-publication-gate and says so — though that README still carries
    the old counts, so it is the same sweep's second half.

- [ ] **M-publication-gate** | `main` still cannot fail, and the measurement says exactly how far `exit(code)` got | `docs/panel/030-the-build-order-revised.md:225` · `spec/heroes-spec.md:190-194` · `examples/`

    **Origin:** M-open-repository, 2026-09-08. The gate's own checklist names
    this as *"one defect that shows up on the second page of any tour"*, queued
    from panel 030, and says it must not be true on the day the examples go up.

    **Measured this session, on a rebuilt compiler**, three shapes rather than
    the one the gate remembered. A program that receives a `fail`, matches it
    and prints it exits **0** — panel 030's sentence, still exactly true. A
    program that calls the built-in `exit(1)` exits **1**, so the escape hatch
    the spec gained is real and works. An out-of-bounds index exits **134**, so
    the guards are unaffected. **11 of the 54 example programs call `exit(`**
    and none of the gallery's 12 do, which is the corpus half of the same
    question: the ones that can fail mostly remember, and nothing makes them.

    So the finding is narrower than *"main cannot fail"* and worse than
    *"solved"*: **the language has a way to report failure and no way to oblige
    it**, and the default for a program that handles its own error is to tell
    the shell it succeeded. Changing what `main` returns is a language change
    and owes a panel; that is why this is filed and not fixed here.

    **Where to look also:** `docs/ROADMAP.md` § M-publication-gate.
    **Why it matters:** a script that calls a Heroes program cannot tell whether
    it worked, which is the one thing an exit code is for.

    **Re-verified 2026-09-10: STILL OPEN, the finding intact, one exit code
    UNPINNED.** `main` still may not declare a result — `selfhost/check/decls.hero:79-81`
    raises `main_returns`, tested at `:218-220` — so *the language has a way to report
    failure and no way to oblige it* stands. `exit(code)` is asserted end to end:
    `tests/golden/run/exit-status.expected` demands `!exit: 3` and
    `tests/harness/expectation.hero:55-56`, `:84-88` make that a demand on the shell
    status. **The abort's 134 is pinned nowhere**: the abort goldens demand only the
    message and a non-zero code (`expectation.hero:89-96`), so *"an out-of-range index
    is 134"* is **UNSETTLED** as a number and settled as non-zero-with-a-message.
    Counts: **11 of 54** program directories call `exit(`, unchanged; the gallery is
    **14** files, not twelve, and still none of them calls it. One pointer moved:
    `exit(code: i64)` is `spec/heroes-spec.md:203`, not `spec:190-194`. **And nothing
    was decided elsewhere**: `docs/work/DECIDE.md` is empty and no sitting on what
    `main` returns has ever been queued.

- [ ] **M-publication-gate** | the trademark question, in the narrow form that applies: the Aladdin Sane bolt | `site/README.md` § Style guide · `docs/assets/`

    **Origin:** the gate's own checklist, restated at M-open-repository
    2026-09-08 because opening the repository did not touch it.

    The name is a common word and worries nobody. The bolt is iconography
    attached to an actively managed estate, it is in the site's banner and in
    this repository's own `docs/assets/`, and the style guide already keeps
    lyrics out, which is the other half of the same care. **Unchanged by the
    repository opening**: the site has carried it publicly since 2026-09-03, so
    the exposure is the same today as yesterday and this stays where it was.

    **Why it matters:** the gate's own words are that it is cheaper to answer
    before publication than after, and publication is now closer.

    **Re-verified 2026-09-10: STILL OPEN and unchanged.** `site/README.md`
    § *One thing the publication gate owns* restates the question and answers
    nothing, and the lyrics rule beside it is unchanged: titles as nods, never
    lyrics. The item's claim that the bolt is in this repository's own assets is
    true — `docs/assets/banner-light.svg` and `banner-dark.svg`, described in
    `docs/assets/README.md`.

*******************************************************************************
