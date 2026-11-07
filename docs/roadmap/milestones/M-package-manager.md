# M-package-manager — packages, and what stands in for a standard library


**Scheduled, no warrant.** Not a decision to take later: design.md §3.5 already
fixes the shape — *"No package manager exists before modules do; when it arrives
it will be `heroes add`/`heroes fetch` — inside the same binary"* (never a second
binary, CLAUDE.md §6 and §10). Its real prerequisite is
**M-separate-compilation**, not M-module-namespace: without separate compilation,
installing a package means recompiling the world on every build. **Since
2026-08-25 there is a second one in front of it** — M-package-layout, which rules
on how a fetched package's modules are named and reached; `heroes add` cannot
place files it has no spelling for. **And since 2026-09-03 the packages themselves
stand in front of it**: M-core-packages, whose step 13 — `heroes fetch`
under the root, the root-level driver and the `heroes check` item panel 091 filed
here — moved into this milestone, because one distributes what exists. **One
question its sitting owes, found the same night**: pinning what `fetch` brings —
Go's `go.sum`, Cargo's lock file — is a per-project file, and CLAUDE.md §10 admits
no fourth input class; the sitting says how a fetched package is fixed to a
version without one, or names which of panel 056's three return conditions it
meets.

**And this is where "a standard library that wraps C" goes.** §1.11 refuses a
standard library permanently, and that refusal is the founding constraint rather
than a shortage of effort — but what a standard library is *wanted* for arrives
here in a form the constraint permits: **distributable bindings**, ordinary
Heroes modules over real C headers, each with its link flag declared next to the
`extern` that needs it (§3.5). The difference is not cosmetic: a binding is
verified by clang against the header it names, and a standard library is verified
by whoever wrote it. **What this milestone does not deliver is the packages**:
those are M-core-packages, the row before it since 2026-09-03 — and the sentence just above,
*verified by clang, or by whoever wrote it*, is the first question that
milestone's opening sitting has to answer for a package written in Heroes alone.

**Panel 056's return conditions live here, as that sitting ruled** (`docs/panel/056`
§ Resolution, deliverable D, ratified 2026-08-15 — and written into this entry only
on 2026-09-03, by a reasoning session that read the panel forward and found the
deliverable undischarged). A per-project file is refused (CLAUDE.md §10: there is
no fourth input class), and **three conditions, all met, return the question to a
panel**: *one key, not two* — a `prefix <dir>` from which `-I` and `-L` are both
derived, so the skew is unrepresentable; *no string in the file may also appear in
a `.hero` file*, so deleting the file never changes which symbols are linked, only
whether the build succeeds; and *one named binding that `package "<name>"` and one
command line cannot build* — a set that was empty on 2026-08-15. Meeting fewer
does not.

**A comparison read on 2026-10-25, and one answer for panel 056's open
question**, how a fetched package is fixed to a version without a per-project
file. Goose (github.com/aardappel/goose, [goose/docs/design/packages.md](https://github.com/aardappel/goose/blob/master/docs/design/packages.md) § 1 and
§ 4.3) pins it in the root file's own import line, `import x from
"owner/repo@v1";`, a git source at a tag or a commit, and the same line in any
other file is a *requirement* the root must satisfy, refused with the line to
add: "No lockfile, no version ranges, no registry", one version of a package
per program. A `use` here already resolves from the compiled file's directory
(spec § 1), so the shape fits this language's rule that the root decides;
whether the source belongs in the `use` line is a question of surface, so this
row's sitting's. Beside it, Goose keeps one cache for the machine, where this
compiler keeps `build/` per project.

**Opened 2026-11-07, beside M-core-packages, by the author's instructions of
2026-11-06.** What follows is the author's position as it was built in
conversation that day, the criteria it gives the opening sitting, the order of
the work, and the sitting's questions. The work items are GitHub issues of
this milestone; this page holds the reasoning.

**The author's position**, in the order it was given, each point meant as:

1. *copy Go: why invent where Go has already solved packages and their sums;
   it is better for LLMs too*;
2. *with Heroes' own peculiarities and names, never `go` in a file's name; the
   newest version of the base, without workspaces; a sitting makes the
   choices*;
3. *but not the GitHub address inside the import, which in Go is history: the
   first element of an external `use` is an alias in angle brackets, `use
   <json>/parse`, looked up in the program's catalogue, where the name has an
   entry with its git address, version and fingerprint; the domain never
   appears in the code. A catalogue in YAML or something nicer; think again
   about the words (modules, packages, libraries); modern and simple; git
   addresses today, our own central index tomorrow, like Python's and
   Cargo's. Reason about it at length before the core packages*;
4. *the order, in two tempos: the package manager first, over C; then the core
   packages; then the package manager rewritten over the core packages, its
   interface unchanged*. The circle, the core packages wanting the package
   manager to be distributed and the package manager wanting the core
   packages to fetch, breaks the way the compiler broke its own, first in
   Rust and then in Heroes;
5. *I said Go because I know it; weigh whether copying Rust makes more sense;
   I would stay with one of these two widely used languages*. So the base is
   one, Go's modules or Cargo, chosen by the sitting and bent only where a rule
   of the author's demands it;
6. *one day there will be a Heroes index, a site to download these packages
   from as well as from git, like `pip install`; the catalogue's structure
   must allow for it now* (row 78, M-package-index). So an entry says where
   its dependency comes from, the index by name and version or git by
   address and reference, always with its fingerprint, and the verb that adds
   a dependency resolves a bare name against the index;
7. *a `use` of a module in the project's own file system stays, with no angle
   brackets*: `use geom/shapes` is the program's own module, from the root as
   today (spec § 1), and `use <json>/parse` an entry of the catalogue. The
   bracket is the whole difference between mine and fetched, visible on the
   line without opening the catalogue.

**The criteria the author gives the sitting**, meant as: *the package manager
is a very delicate subject; Go took years to bring it in, and I want it in
from the start: the most solid, the most robust and also the simplest
solution, accepting a few small conventions, small, because everything must
stay very simple and intelligible for LLMs too*. How long Go took is the
historian's to verify. Simplicity is measured, not argued: the tokens each
variant adds to the spec; the words and concepts a reader must learn,
counted; the catalogue's grammar, in productions; the files a program must
have, zero if it depends on nothing; every plausible mistake, from Go's,
Cargo's, npm's and Python's habits, with the error and the `certain` fix
that closes it, compiled; and the blind seat and a reader test registered
before it runs, within the author's cap of 25 USD for the whole sitting.

**The author's answers of the same day**: the first batch of M-core-packages
is pushed after the sitting, adapted to what it decides (put to the author
again at the ratification, because adapting it needs this row's first
version built and not only the sitting); paid runs up to 25 USD in all and no
more; and a survey of the precedents first, read by the author, then the
sitting.

**Why the first version needs no core package**, read in the compiler on
2026-11-06: `git` can be run by `process.run` (`selfhost/cli/process.hero`),
which already runs `pkg-config`, `uname` and `curl`; SHA-256 exists in Heroes
(`selfhost/cli/sha256.hero`) on text only, so how a file of a fetched tree
that is not text is read, copied and fingerprinted is a question for the
sitting and not a fact; and the one network access is `heroes measure
--refresh`, which runs `curl`. The second version meets no bootstrap paradox:
the core packages it uses are copied into the compiler's own tree, the shape
panel 234's R13 already gives `core/slices`, so building the compiler fetches
nothing.

**What the decision touches**, each rule reopened by name and never around
it: the three inputs and the per-project file (`.claude/rules/cli-surface.md`
§ There is no fourth slot, since 2026-11-07 § Four input classes, and panel 056's three return conditions, whose
second a catalogue repeating the alias `use` writes cannot meet, so that
point is the author's call and is recorded as such); the grammar of `use`
(`spec/heroes-spec.md` § 1); panel 099's premise, *no manifest*; panel 100's
R9, the rename it deferred to this day; panel 101's R3 condition, two
installed libraries colliding on a last part; panel 234's R2 and the copies
under `core/`; design.md §3.5's `heroes add` and `heroes fetch`; and panel
032's R6, `use` scoped to the program.

**The order, in three tempos.** First, this row's first version over C, its
interface fixed by goldens, its witness the first batch of M-core-packages
adapted and fetched from its own repository; the row closes after that batch
lands, since a tag stands only over no open `blocking` defect. Second, the
core packages, resumed and distributed by it. Third, the second version over
the core packages, every golden of its interface passing unchanged, which is
also an acceptance condition of M-core-packages.

**The opening sitting's questions** (panel 235): the base, Go or Cargo, one
and whole; the words; `use <name>/…` beside `use name/…`, and their plausible
mistakes as errors with `certain` fixes; a dependency's own `use` lines
starting from its own root and not the consumer's; the catalogue's format
(YAML, TOML, JSON, a line format, or Heroes' own syntax as Zig's ZON); an
entry's sources (the index, git, a local folder while a package is
developed) and its fingerprint, over which bytes; versions (exact pins or
ranges, MVS or PubGrub, one version per program); where fetched code lives,
and what `corpus`, `packages`, `records` and `unseen` must know of it; the
catalogue optional, a program without one compiling as today; the rewrite of
the three-inputs rule; R2 and `core` as a catalogue entry; `internal/` and the
new verbs; the C under the first version, programs run or libraries bound;
the interface that stays fixed from the first version to the second; and what
row 78 must deliver.

**The author's word on CLAUDE.md § 2, 2026-11-07**, given in conversation when
panel 235's survey brief found that copying Go's `go.mod` or Cargo's
`Cargo.toml` whole brings a line declaring a language version or an edition
(`go`, `toolchain`, `edition`, `rust-version`), which § 2 forbids before
`1.0.0` (*no declared version, no mode*), meant as: *if it is needed, let us
relax this rule*. So the sitting may keep such a line where it measures that
the dependency system needs it, and says which line and why; § 2 is then
amended in the commit that lands the ruling, under the section it amends,
with this date, and the contract's count re-measured with it. Where the
sitting finds the line not needed, § 2 stands as written.

**The author's position after reading the survey, 2026-11-07**
(`docs/panel/235-reports/historian-survey.md`), meant as: *the sitting decides
between Go and Cargo; the sitting decides the catalogue's format, YAML, TOML
or another; I want the most modern and current solution, the one that works
best for language models*. So panel 235 rules both, and the author's
criterion is measured rather than assumed: the blind seat and the reader test,
within the author's cap of 25 USD for the whole sitting, compare the
competing forms on what a model writes right at the first turn and what it
is told when it does not.

**Panel 235, ratified 2026-11-07** (decision 2329,
`docs/panel/235-dependencies-named-by-an-alias-in-angle-brackets-that-the-programs-catalogue-resolves-gos-modules-copied-whole-a-toml-catalogue-listing-every-package-and-no-fetched-code-run-at-build.md`):
R1 to R20 as adopted. The author's `use <name>/module`, a bare `use` the
program's own; Go's modules in their newest release, the program's catalogue
the build list, minimal version selection in the verb, no lock file;
`heroes.toml`, a TOML subset read all or nothing; fingerprints over the
committed blobs, `path` entries told at every build; fetched code placed where
no `use` can spell it and checked at every build; `build`, `check` and `run`
never fetch; the ffi-pragmatist's C1 to C5, no fetched code run at build;
`heroes add` and `heroes fetch`; the three-inputs rule rewritten; CLAUDE.md §
2 unchanged; `core` as fingerprinted entries, amending panel 234's R2. **What
the first version owes, in its lanes**: R12's wording for design.md §3.5,
`.claude/rules/cli-surface.md` and CLAUDE.md § 10 in one commit; the
specification's text with the code that makes it true (R13); the `use`
cluster, defects 2290, 2292, 2308 and 2325, and the header search of C1 with
defect 2324, in the first lane; R19's goldens before the close.
