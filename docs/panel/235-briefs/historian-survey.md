# Panel 235, the historian's survey, written before the sitting

**Read this brief first, then work.** You are the historian seat of panel 235,
M-package-manager's opening sitting, called before the other seats to write a
survey the author reads first: the author asked to reason about the
dependency system at length, on facts, before any seat rules. **This survey
gives no verdict and recommends nothing.** It reports what other package
systems do, each fact from a page you read today.

**This brief overrides your agent definition's output structure** for this
task: no `verdict` and no `argument`. Keep its discipline: every factual
claim carries its source, and keep its `search` section. A cell you cannot
source is not written as `unverified`; it is written `not found`, with what
you searched for and where.

## Where everything is: absolute paths, one lane

You work in the lane `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/pm-opening`
(branch `pm-opening`, commit `b32ac8632` or later). Read every project file
from there, never from the repository's main checkout, whose copy of the
pages below is older. Write only your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/pm-opening/docs/panel/235-reports/historian-survey.md`,
**as you go**, so a stopped run leaves what it had. Write nowhere else.

## What the sitting is about

Read the author's position, the criteria and the sitting's questions in the
lane's `docs/roadmap/milestones/M-package-manager.md`, **lines 59 to 165**
(from *Opened 2026-11-07* to the end of the page). In short, and the page wins
where this summary differs:

- copy one widely used package system whole, Go's modules or Rust's Cargo,
  its newest release, without workspaces, with Heroes' own names;
- in the code a dependency is named by an alias in angle brackets as the
  first element of the import, `use <json>/parse`, looked up in a catalogue
  file of the program whose entry gives the source, the version and a
  fingerprint; the domain never appears in the code; `use geom/shapes`,
  without brackets, stays a module of the program's own tree;
- an entry's source is the index, by name and version, or git, by address
  and reference, or a local folder while a package is developed; the verb
  that adds a dependency resolves a bare name against the index;
- git today, and one day a central index, a site packages are downloaded
  from, as `pip install` reads PyPI;
- the catalogue in YAML or something nicer; the words (module, package,
  library) reconsidered;
- the criteria: the most solid, the most robust and the simplest solution,
  small conventions accepted, intelligible to language models, measured by
  the tokens a variant adds to the specification, the words a reader must
  learn, the files a program must have (zero if it depends on nothing), and
  every plausible mistake carried over from another system's habits.

Two facts of this project bear on what you collect: today a Heroes `use`
names one file, read from the directory of the file the compiler is given
(the lane's `spec/heroes-spec.md`, lines 27 to 30); and CLAUDE.md § 2 forbids,
before version `1.0.0`, *"no declared version, no mode, no old spelling kept
beside the new"*, so a line of a copied file that declares a language
version or an edition is a question the sitting must face.

**Panel 234's historian already verified rows you should re-read, not copy**:
the lane's `docs/panel/234-reports/historian.md` on Nim's `std/` and `pkg/`
prefixes (its lines 216 to 245), Node's `node:` prefix (line 41) and Go's
dot in the first path element (line 90). Re-read each page they cite before
you use it, and say the day you read it.

## What to write

Every cell of every table carries **its own URL and the day it was read**.
Every probe below is asked of **every** system; nothing is asked of one
system alone. An example in this brief is a question to check, never a fact
to copy.

**Table 1, the full inventory of the two candidates.** Every directive of
`go.mod` and every kind of line of `go.sum`, and every table and key of
`Cargo.toml` and every field of `Cargo.lock`, in the newest releases of Go
and Cargo, the release named and dated. For each: what it does, whether it
is required, and whether it declares a language version, a toolchain or an
edition. Workspaces are listed and marked as excluded by the author.

**Table 2, the package systems**, with Go's modules and Cargo first and in
the most detail, then Dart (pub), Zig (`build.zig.zon`), Deno (import maps
and JSR), Node and npm (`package.json`, its `imports` field, `package-lock.json`),
Swift Package Manager, Gleam, Python (`requirements.txt`, `pyproject.toml`,
uv's lock file), Nim (Nimble), Odin (its library collections), and Goose
(`github.com/aardappel/goose`, its `docs/design/packages.md`). For each:

1. how the source code names a dependency; whether a name local to the
   project can stand for it, and how; how an import line tells the program's
   own code from fetched code; what happens when two dependencies or a
   dependency and a local directory want the same name;
2. where the dependency's address, version and fingerprint are written, in
   which file and format; whether the fingerprint sits beside the
   requirement or in a separate lock file; over which bytes it is computed
   and with which hash, for a registry source and for a git source;
3. the words it uses for a file, a directory of files, the unit with a
   version, and the file that lists dependencies;
4. how it selects versions (minimal version selection, a solver, exact pins,
   ranges), and whether one program may hold two versions of one dependency;
5. how a central registry, git sources and local folders live together, and
   how the lock file records which one each dependency came from;
6. where fetched code is kept (a cache for the machine, a directory inside
   the project, or both);
7. **whether reading the dependency file, fetching, or building a dependency
   runs that dependency's own code** (build scripts, install scripts, a
   manifest that is itself a program);
8. the commands that add, fetch, update and verify, and the exact error a
   fingerprint mismatch or a missing dependency produces, quoted, and whether
   the error names its fix;
9. whether it has a privacy rule for a dependency's internals, such as Go's
   `internal/` directories;
10. what its history had to correct, each correction with its release and
    date: for Go and for Cargo alike, from their first way of getting a
    dependency to today's, since the author's premise is that Go took years
    to bring a package manager in, and the same question is owed to Cargo.

**Table 3, the indexes**: PyPI (its "simple" API), crates.io (its index
protocols), npm's registry, Hex, pub.dev, Go's module proxy protocol and its
checksum database, and JSR. For each: what the index holds (archives,
fingerprints, metadata); the protocol a client reads it by; whether it can
be served as static files or needs a running service; who may publish a
name, and whether names have namespaces; what happens to a version or a
name that is withdrawn; and how publishing is authenticated.

**Table 4, the catalogue's possible formats**: YAML, TOML, JSON, a line
format of its own (as `go.mod`), and a file in a language's own syntax
(Zig's ZON, Swift's `Package.swift`): for each, which systems use it, whether
reading it can run code, and the pitfalls its own specification, its
documentation or the issue trackers of the systems that use it record. For
YAML, read from the YAML 1.1 and 1.2 specifications themselves how a plain
scalar such as `1.10`, `no` or `on` is resolved in each; do not assume the
answer.

**Table 5, angle brackets and prefixes in imports**: C's `#include <...>`
against `#include "..."`, Nim's `pkg/` and `std/`, Node's `node:`, Odin's
`core:` and named collections, Dart's `package:`, and any other system the
tables above show marking a fetched dependency in the import line itself.

## The rules of the seat

- A cell says only what its page states. A claim the page does not carry is
  cut. A page you cannot read is not cited.
- Quote briefly, in the page's own words, where a quotation decides a cell.
- Where you searched and found nothing, write what you searched for and
  where; never write that something does not exist.
- Run no paid model session and call no paid API. Web search and page reads
  are your tools.
- Print no credential, ever.
- End the report with **What a reader of these tables should not conclude**,
  naming the comparisons the tables cannot support, and with your `search`
  section.
