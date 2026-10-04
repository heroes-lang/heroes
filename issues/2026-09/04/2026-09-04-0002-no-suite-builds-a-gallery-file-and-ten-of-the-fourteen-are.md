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
