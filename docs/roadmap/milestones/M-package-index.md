# M-package-index — the index a Heroes package is published to and fetched from

**Scheduled 2026-11-07, at row 78, by the author's instruction of 2026-11-06**,
immediately after M-package-manager, meant as: *one day there will be a Heroes
index, a site we download these packages from as well as from git, like `pip
install`; we have to work out how to run that big index one day, and the
catalogue's structure, a YAML file or whatever it becomes, must allow for it
now*. Given in conversation while M-package-manager's plan was being written.

**Why it is a row of its own, and why it sits here.** M-package-manager
delivers the catalogue, the file that names a program's dependencies, and the
author ruled the same day that each of its entries says where its dependency
comes from: the index, by name and version, or git, by address and reference,
always with its fingerprint (`M-package-manager.md`, the author's position,
point 6). So the index's shape reaches the catalogue before the index exists,
and row 77's opening sitting, panel 235, decides what of it the first version
must already respect, so that the catalogue does not change when the index
arrives. Delivering the index itself, the site, publishing a package to it,
who may take a name and what happens to a withdrawn one, is a different
deliverable, so a different row (CLAUDE.md § 14).

**What its opening sitting inherits**, none of it measured yet: the survey of
precedents written for panel 235 carries a table of the existing indexes
(PyPI, crates.io, npm, Hex, pub.dev, Go's module proxy protocol, JSR), what
each holds, the protocol a client reads it by, whether it is a static site or
a service, who may publish a name and what a withdrawn name does; and panel
235 measures a static index, files served by a site, as one variant, because
it has no service to keep running and can be tried from a local folder. The
code does not change when a dependency moves from git to the index: it writes
`<json>` either way, and only the catalogue's entry changes.

**Feature 2315 is this row's since 2026-11-15**: the package manager rewritten over the core
packages, every golden of its interface unchanged, moved here from M-core-packages by the author's
answer *(b)* on panel 250 (decision 2564), panel 242's R7 *"Feature 2315 waits for that package"*
holding here: the parts it would replace (`git` and `curl` launched, the index read over HTTP) wait
for core packages that hold them.

**Decided 2026-11-17, by the author's choice among three** (on the question *after the package
manager and the web framework, should we make a site that acts as the Heroes package
repository?*): the index comes after M-rebel-packages-manager and is **written in Heroes with
Rebel** (`web`, `http`, `sql/postgres`, `s3`, `oauth2`, `jose`, `semver`, `archive`, `crypto`,
`metrics`, `tracing`), the first large public application the library carries; **its static
half first**, the indexes as files a site serves, which panel 235 measured as a variant and which
can be tried from a local folder, **then the service that publishes** (uploads, who holds a name,
what a withdrawn version does). Rebel is the first package it publishes. Refused: a static index
alone with publishing left to git and a request by hand, and leaving the row as it stood.

**Feature 2315 moves to M-rebel-packages-manager, 2026-11-19**, at M-rebel-library's landing, by
the author's decision 4 of 2026-11-15 (`docs/roadmap/milestones/M-rebel-library.md`): the package
manager written with the library has a row of its own, row 78, and this row comes after it, as the
paragraph above decided on 2026-11-17. So the paragraph of 2026-11-15 that made 2315 this row's
stands at its date and holds no longer; the issue's GitHub milestone moves with it.

**A variant the opening sitting measures, the author's of 2026-11-23** (given while M-rebel-library's folders
were being moved, `docs/roadmap/milestones/M-rebel-library.md` § The author's answers of 2026-11-23, about 23:00),
meant as: *the index should not hold a package's files, only a pointer, a sort of link, saying how to fetch them;
one day the index may be a repository of its own, as Homebrew's taps are, Rebel another, its examples a third.* It
turns part of the choice of 2026-11-17 above, *the indexes as files a site serves*, so the sitting puts the two side
by side and the author chooses with the measurements in front of them. What the sitting is handed, each read on
2026-11-23 and to be read again on its own day:

- **the protocol that stands**: `<index>/<name>/versions`, `<index>/<name>/<version>/list` and
  `<index>/<name>/<version>/files/<path>`, every file at its own address and never rewritten once published
  (panel 235, ratified 2026-11-07; `selfhost/fetch/index_source.hero`), its names fixed by the `fetch` goldens;
- **what a pointer already is**: a catalogue's git entry, `git`, `tag`, `commit` and `sum`, which a program can write
  today with no index, so an index of pointers is a list from a name and a version to such an entry;
- **what it does not yet say**: the git entry names no folder inside a repository (searched in the specification,
  `docs/design.md` and `M-package-manager.md`, nothing found), so a package must stand at its repository's root or
  the form must learn a folder;
- **the question a sitting owes an answer to**: a pointer keeps a version's integrity, the `sum` being checked either
  way, and gives up its availability, since a repository deleted or a tag rewritten ends a build the index cannot
  mend; which is why the precedents to verify, never to recall, are Go's module proxy and checksum database beside
  its fetch from version control, Homebrew's formulae beside the bottles it hosts itself, and crates.io's index of
  checksums beside the archives it stores;
- **two facts of the day it was given**: Rebel's source stood at `packages/rebel/0.1.0/files`, a published version's
  address, with neither `versions` nor `list`, which is what moved it to `rebel/library/` (whether a source's path
  carries its version at all is this sitting's too); and `index.heroes-lang.org`, the address a catalogue with no
  `index` line reads, resolved to nothing, `heroes add rebel` ending at exit 2 on `curl`'s *Could not resolve host*.
