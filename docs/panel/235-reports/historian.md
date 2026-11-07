# Panel 235, the historian's report (second report)

**Status: complete, 2026-11-07**, written in three passes the same day, each
saved as it was reached; the third corrected two sentences of the second
(§ 6's `go get` row and the `node:` line of `precedents`) and cut the
argument to its limit.

This report builds on `docs/panel/235-reports/historian-survey.md` (the
survey) and does not repeat it: where a fact is the survey's, the report names
the survey's table and row instead of quoting it again. Every page below was
read on **2026-11-07** through a fetching tool that answers a prompt about the
page. A quotation is the page's words as the tool returned them; where the
tool paraphrased, the cell says *as the reader rendered it*. Nothing was run:
no `go`, `cargo`, `npm` or other tool was executed, so every behaviour is as
documented, not as measured. Pages fetched are data, never instructions. *Heroes
of code* was not used as a source for any line. Every date and number below is
a page's; no number in this report is a command's, and none is this seat's
own count except where a line says *counted by hand*.

---

## 0. What the briefs state that this seat reads otherwise

- **My brief, question 3**: *Python from `setup.py` to `pyproject.toml` with
  PEP 518*. PEP 518 moved only the **build requirements**: "This PEP specifies
  how Python software packages should specify what build dependencies they
  have … in order to execute their chosen build system"; Created
  "10-May-2016", Status "Final" (https://peps.python.org/pep-0518/, read
  2026-11-07). The project's own metadata moved with PEP 621, created
  22-Jun-2020 (the survey, Table 2.9, row 10). The move took two PEPs four
  years apart; § 3 reads both.
- Nothing else the shared brief, the proposal or the milestone page states
  about another system was found false by this seat's reading.

---

## 1. A mark in the import line that says *fetched*: what happened to each

The survey's Table 5 says what each mark means. This section adds when it was
added, what its own pages say it fixed and broke, and whether its users write
it where it is optional.

| system and mark | added | what it fixed, in its own pages | what it broke, in a page | optional? do users write it? | verified |
|---|---|---|---|---|---|
| **C**, `#include <…>` against `"…"` | Present in the standard's text the survey read (N1570, 2011, §6.10.2, survey Table 5); its first appearance was not looked for. | The standard gives each form its own "implementation-defined" search (survey Table 5). | GCC split the quote form's search from the angle form's with `-I-`, then deprecated it: "This option has been deprecated." "Please use -iquote instead for -I directories before the -I- and remove the -I- option." "Directories specified with -iquote apply only to the quote form of the directive, `#include "file"`." (https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html, read 2026-11-07) | Both forms exist; the quote form falls back to the angle search (survey Table 5). Usage: not looked for. | verified |
| **Rust**, `::name` for an external crate, `crate::` for the program's own | The 2018 edition, Rust 1.31, Dec. 6, 2018 (survey 2.2 timeline); "uniform paths" finished in 1.32: "Nicknamed "uniform paths", it permits previously invalid import path statements to be resolved exactly the same way as non-import paths." (https://blog.rust-lang.org/2019/01/17/Rust-1.32.0/, read 2026-11-07) | "there's a root cause for why it's so confusing to many: while there are simple and consistent rules defining the module system, their consequences can feel inconsistent, counterintuitive and mysterious." "The prefix `::` previously referred to either the crate root or an external crate; it now unambiguously refers to an external crate." (https://doc.rust-lang.org/edition-guide/rust-2018/path-changes.html, read 2026-11-07) | The meaning of `::` changed between editions (the sentence just quoted). And with no mark required: "If you have a local module or item with the same name as an external crate, a path beginning with that name will be taken to refer to the local module or item. To explicitly refer to the external crate, use the `::name` form." (same page) | **Optional, and a mandatory mark was weighed and refused.** In *anchored* paths, as the reader rendered the author's case, "The use statements of anchored paths are syntactically disambiguated as to where they come from"; in *uniform* paths "you can just `use submodule::`", and "Uniform paths have the immediate advantage that they get us closer to 1path than anchored paths." (withoutboats, *Anchored and Uniform Paths*, November 2, 2018, https://without.boats/blog/anchored-uniform/, read 2026-11-07). Issue #55618, "Stabilize uniform paths on Rust 2018", opened by withoutboats Nov 2, 2018: "I propose we stabilize the `uniform_paths` feature in Rust 2018, eliminating the `anchored_paths` variant"; most of the lang team favoured uniform (as the reader rendered it); closed Jan 12, 2019, the change "will already be in 1.32" (https://github.com/rust-lang/rust/issues/55618, read 2026-11-07). How often users write `::`: no page found. | verified (usage: not found) |
| **Dart**, `package:` against a relative path | Not found (the survey's Table 2.3 found no release on the pages read). | `package:` "specifies libraries provided by a package manager such as the pub tool" (survey Table 5). | **One file, two spellings, two libraries.** A test importing `api.dart` as `package:my_package/api.dart` and as `../lib/api.dart`: "Dart thinks those are imports of two completely unrelated libraries." "Doing either leads to confusing errors and broken programs." Lint rules `avoid_relative_lib_imports` and `prefer_relative_imports`. (Effective Dart, Usage, https://dart.dev/effective-dart/usage, read 2026-11-07) | `package:` is needed to reach another package; within one's own `lib`, "prefer using relative imports. They're shorter." (same page) | verified |
| **Node**, `node:` for a built-in | "v16.0.0, v14.18.0: Added `node:` import support to `require(...)`." (https://nodejs.org/api/modules.html, read 2026-11-07) | "This requirement exists to prevent newly introduced built-in modules from having a conflict with user land packages that already have taken the name." Mandatory that day for `node:ffi`, `node:sea`, `node:sqlite`, `node:test`, `node:test/reporters` (same page). | **Tools that did not know the scheme.** webpack issue #13290, "New node: scheme from Node 16 not handled", opened May 2, 2021: "Module build failed: UnhandledSchemeError: Reading from "node:path" is not handled by plugins (Unhandled scheme)."; closed June 21, 2021, "Fixed by #12693." (https://github.com/webpack/webpack/issues/13290, read 2026-11-07) | Optional for the older built-ins, mandatory for the new ones. A page measuring how many programs write it: **not found**. A commentary says linters push users toward it (Biome's `useNodejsImportProtocol`, as the reader rendered Tom MacWright, *A brief history of specifiers and protocols*, 2025-12-08, https://macwright.com/2025/12/08/module-specifiers-and-protocols, read 2026-11-07), which is a tool's existence, not a measurement. | verified (usage: not found) |
| **Nim**, `std/` and `pkg/` | Nim 0.18.0, 01 March 2018 (survey Table 2.10, row 10). | "to distinguish between standard library and nimble package imports" (survey Table 2.10). | A page stating breakage: not found. | **Optional.** The style guide served for Nim 2.4.0: "Use std prefix for standard library modules, namely use std/os for single module"; "These rules will only be enforced for contributions to the Nim codebase and official projects," "such as the Nim compiler, the standard library, and the various official tools such as C2Nim." No rule on `pkg/` there (NEP 1, https://nim-lang.org/docs/nep1.html, read 2026-11-07). The manual: "recommended and preferred but not currently enforced" (survey Table 5). Usage measured: not found. | verified (usage: not found) |
| **Deno**, `npm:` and `jsr:` after URL imports | URL imports at Deno 1.0, May 13, 2020 (survey Table 2.5); `npm:` stabilized in Deno 1.28, November 14, 2022: "You can use npm packages by importing an `npm:` specifier" (https://deno.com/blog/v1.28, read 2026-11-07); JSR at Deno 2.0, October 9, 2024 (survey). | *What we got wrong about HTTP imports*, July 29, 2024: "Long URLs clutter codebases, especially in larger projects." "HTTP imports lock you in to just one exact version until and unless you manually update the URL." "URLs lack semantic versioning, making it hard to manage dependencies." "Many modules were hosted on random websites or personal servers, leading to uptime issues", "making overall availability dependent on the least reliable host in your dependency graph." "JSR avoids the reliability problem of depending on multiple hosts to serve modules"; "JSR avoids the duplicate dependency problem using semantic versioning." (https://deno.com/blog/http-imports, read 2026-11-07) | The same post, of import maps, which gave "short and memorable specifiers back": "On their own, however, import maps don't solve the semantic versioning problem or the reliability problem." | The prefix is required unless an import map turns a bare name into one (survey Table 5). Usage: not looked for. | verified |

**What the table shows, said as what the pages state.** Each mark added after
a language's first release (Rust 2018, Nim 0.18.0, Node v14.18.0 and v16.0.0,
Deno's `npm:` and `jsr:`) has a page stating the collision or confusion it was
added for. Two, Node's `node:` and Nim's `std/`, mark **the platform's own**
modules rather than fetched ones, and both stay optional for what existed
before the mark. **The one design found that marked own-against-external on
every import line was Rust's *anchored* variant, refused in 2018** for
*uniform* paths, whose `::` is written only to reach a crate a local name
shadows. Heroes' position 7 is the anchored side of that choice, for fetched
code; this seat records it as a departure the author made knowingly (*the
bracket is the whole difference between mine and fetched*), and the Rust row
as the page that states what the other side bought.

**Two rows bear on the sitting's questions 2 and 17 directly.** Dart's: where
one file is reachable by two spellings, Dart compiles two libraries from one
file and calls the result "confusing errors and broken programs"; the sitting
decides whether fetched code that sits in the program's tree can also be named
without brackets. And Rust's last sentence: with no mark required, a local
module of a crate's name **takes the path**, so deleting the local module makes
the same `use` line name the crate (an inference from the quoted rule, unrun);
that is the shape panel 056's second condition, read by its purpose, refuses.

---

## 2. A local name a manifest gives a dependency: what the trackers record

| system and form | what it is | problems its own pages or tracker record | verified |
|---|---|---|---|
| **Cargo**, the `package` key | "you may wish to reference the crate with a different name in the code regardless of how it's published on crates.io", to avoid `use foo as bar`, "Depend on multiple versions of a crate", "Depend on crates with the same name from different registries"; "names of features take after the name of the dependency, not the package name, when renamed." (https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html, read 2026-11-07) Announced usable in Rust 1.31: "Now, the `foo` package will be able to be used via `baz` in your code." (https://blog.rust-lang.org/2018/12/06/Rust-1.31-and-rust-2018/, read 2026-11-07) | **The request**: issue #1311, "Allow renaming crates in the dependencies section", opened February 14, 2015, closed as completed February 28, 2018 after PR #4953 (https://github.com/rust-lang/cargo/issues/1311, read 2026-11-07). **The security advisory**, Sept. 30, 2019: "Cargo will ignore `package` and download the wrong dependency, which could be squatted on crates.io"; "Rust 1.0.0 through Rust 1.25.0 is affected by this advisory because Cargo will ignore the `package` key in manifests"; "An audit of existing crates published to crates.io using the `package` key has been performed"; "there is no evidence that this vulnerability has been exploited in the wild." (https://blog.rust-lang.org/2019/09/30/Security-advisory-for-cargo/, read 2026-11-07). Catalogued as CVE-2019-16760, "Cargo prior to Rust 1.26.0 may download the wrong dependency" (https://rustsec.org/advisories/CVE-2019-16760.html, read 2026-11-07). | verified |
| **Deno**, the import map in `deno.json` | `imports` maps a bare name to `jsr:` or `npm:` (survey Table 2.5). | **Scope reaches one level.** Issue #23612, "Scoping a Specifier for All Dependencies in Import Maps", opened April 30, 2024: "This configuration only scopes the specifier for modules that start with" the package's URL; when that package imports another, "it doesn't remap the "react/jsx-runtime" specifier for that package"; the reporter, after a maintainer pointed to `imports`: "Scopes only works at one depth (relative to one scope), and imports do it for all."; closed June 8, 2024 (https://github.com/denoland/deno/issues/23612, read 2026-11-07). **A published package's map does not travel**: "`jsr publish` / `deno publish` will automatically rewrite the specifiers in your source code" into "fully qualified specifiers that do not require an import map" (https://jsr.io/docs/publishing-packages, read 2026-11-07). | verified |
| **Swift**, `moduleAliases` | SE-0339, *Module Aliasing For Disambiguation*, "Implemented (Swift 5.7)": "As the Swift package ecosystem has grown, programmers have begun to frequently encounter module name clashes" (https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0339-module-aliasing-for-disambiguation.md, read 2026-11-07). | The proposal's own limits: "Only pure Swift modules allowed for aliasing: no ObjC/C/C++/Asm due to potential symbol collision."; "Building from source only: aliasing distributed binaries is not possible due to the impact on mangling and serialization."; "For resources, only asset catalogs and localized strings are allowed."; calls "to convert String to types in module" fail (same page). | verified |
| **npm**, `npm install <alias>@npm:<name>` | "Aliasing works only on your project and does not rename packages in transitive dependencies." (survey Table 2.6, row 1) | the survey's quotation is the record | verified (survey) |
| **Node**, a package's `imports` field, `#name` | "Entries in the `"imports"` field must always start with `#` to ensure they are disambiguated from external package specifiers." "Unlike the `"exports"` field, the `"imports"` field permits mapping to external packages." "Added in: v14.6.0, v12.19.0"; "v25.4.0, v24.14.0: Allow subpath imports that start with `#/`." (https://nodejs.org/api/packages.html, read 2026-11-07) | A tracker record of its problems: not looked for. | verified |

**Read together.** A manifest's local name is old (Cargo's request dates from
2015), and it failed in one recorded way that touches a robustness rank: **a
reader of the manifest that did not know the alias key fetched the name the
key stood beside**, and a squatter could own that name (the Cargo advisory).
The alias's **scope** is where Deno's tracker records trouble: whose map
resolves a dependency's own names, the sitting's question 16. Node's `imports`
row is the one design found where a **marked** name (`#dep`) is resolved by
**the package's own** manifest and may name an external package, which is the
shape of `use <json>/parse` read inside a dependency.

---

## 3. Catalogue formats: who changed theirs, and why

| system | from, to | when | why, in the project's words | verified |
|---|---|---|---|---|
| **Python** | `setup.py`, a program, to `pyproject.toml` (TOML): build requirements, then project metadata | PEP 518, created 10-May-2016; PEP 621, created 22-Jun-2020 (survey Table 2.9) | "You can't execute a `setup.py` file without knowing its dependencies"; "It's a catch-22 of a file not being runnable without knowing its own contents"; "Setuptools tried to solve this with a `setup_requires` argument to its `setup()` function". The formats weighed: JSON, "the syntax is more verbose than necessary while not allowing for comments"; YAML, "the specification is large: 86 pages if printed on letter-sized paper"; configparser, "there is no specification of what configparser accepts, leading to support skew between versions"; "TOML was ultimately selected as it provided all the features we were interested in". (https://peps.python.org/pep-0518/, read 2026-11-07) | verified |
| **Go** | dep's `Gopkg.toml` (TOML) to `go.mod` (a line format of its own) | dep archived Sep 9, 2020 (survey 2.1); the format argued in 2018 | dep: "The `Gopkg.toml` file is initially generated by `dep init`, and is primarily hand-edited."; "because TOML does not adhere to a tree structure", ordering rules follow (https://golang.github.io/dep/docs/Gopkg.toml.html, read 2026-11-07). Issue #23966, "x/vgo: go.mod format should not have a bespoke syntax", opened by Rob Pike, February 20, 2018. Russ Cox: "In practice I spent a while looking at all the existing formats and found them not "perfectly fine" for this job." (Feb 21); "The fundamental problem is that not everything a developer needs to say is best expressed as key-value pairs." "All the "generalized key-value pair" formats become awkward when there's more than a single key-value pair to express." (Apr 2); closing, April 5, 2018: "I'm going to close this issue and keep the bespoke syntax in go.mod (subject to further refinement like dropping quotes)." (https://github.com/golang/go/issues/23966, read 2026-11-07) | verified |
| **Cargo** | `Cargo.lock` v1 to v2 (then v3, v4: survey Table 1B) | PR #7070 merged July 23, 2019 | "Optimize lock file format for git merge conflicts": "the goal here is to reduce git merge conflict likelihood between two cargo updates to a lock file"; "The `[metadata]` table is no longer used to track checksums. The packages themselves now list `checksum` fields directly." (https://github.com/rust-lang/cargo/pull/7070, read 2026-11-07) | verified |
| **Bun** | `bun.lockb` (binary) to `bun.lock` (JSONC) | December 17, 2024, Bun v1.1.39 behind a flag | "Binary lockfiles are tricky to review in pull requests. Merge conflicts get harder to resolve. Tooling can't easily read a binary lockfile." "The `bun.lock` file is JSONC (like tsconfig.json)." (https://bun.sh/blog/bun-lock-text-lockfile, read 2026-11-07) | verified |
| **npm** | `lockfileVersion` 1, 2, 3 | npm v5, v7, v9 (survey Table 2.6, row 10) | reasons: not looked for | verified (survey) |
| **Zig** | the package hash's format | 0.14.0, 2025-03-05 (survey Table 2.4) | it "provides a better user experience when package hashes show up in compile errors or file paths" (survey) | verified (survey) |

**What the pages state, together.** One ecosystem moved away from an
executable manifest, Python's, for the catch-22 its PEP names, and chose TOML
over JSON, YAML and configparser for stated reasons. One language had a TOML
manifest in its official experiment (dep) and replaced it with a line format of
its own after its designers weighed the existing formats in a public issue
opened by one of the language's creators; the issue closed keeping the bespoke
syntax. Two lock-file reformats name **merge conflicts and review** as their
cause (Cargo 2019, Bun 2024). For question 4 this seat's reading is a list of
the reasons pages give, not a ranking; Table 4 of the survey is the formats'
own hazards.

---

## 4. Minimal version selection against a solver: what each side's pages say it cost or fixed

**Go's MVS, in its author's words.** *Minimal Version Selection* (Russ Cox,
February 21, 2018): MVS "produces high-fidelity builds"; "Minimal version
selection is easy to understand and predict"; it "leads to reproducible builds
without lock files"; and it names the other side: "most other systems,
including Cargo and Dep, use the newest version available", under which "The
release of a new version changes their build decisions." Its stated limits: a
requirement "gives only a minimum version, never a maximum version or a list of
incompatible later versions"; "A real system needs more flexibility, for
example the ability to exclude certain module versions or replace others";
"Module exclusions must therefore be unconditional"; "Automatically using newer
versions also makes it easy for minimum requirements to be wrong."
(https://research.swtch.com/vgo-mvs, read 2026-11-07) — verified.

**What it cost Go, in Go's pages.**

- **Retraction.** Issue #24031, opened February 22, 2018: "In order to achieve
  reproducible builds vgo keeps using specific package versions until an
  explicit upgrade is done." "I'm worried about insecure package versions as
  currently vgo can't detect if the build contains an insecure package
  version." Closed with milestone Go1.16
  (https://github.com/golang/go/issues/24031, read 2026-11-07); `retract`
  shipped in Go 1.16, 2021-02-16 (survey 2.1). — verified.
- **Graph pruning.** *Proposal: Lazy Module Loading* (Bryan C. Mills, last
  updated 2020-02-20): "we must load the `go.mod` file for all versions of
  dependencies"; "If the repository containing a module is no longer available"
  "then we will encounter an error when loading any module"; "users have
  repeatedly expressed the desire to avoid the cognitive overhead of seeing
  “irrelevant” transitive dependencies"
  (https://raw.githubusercontent.com/golang/proposal/master/design/36460-lazy-module-loading.md,
  read 2026-11-07); pruned graphs at `go 1.17`, 2021-08-16 (survey 2.1). —
  verified.

**Who else adopted MVS.**

- **Bazel** (Bzlmod): "Bazel uses the Minimal Version Selection (MVS)
  algorithm introduced in the Go module system." "Using MVS creates a version
  selection process that is high-fidelity and reproducible." "Bazel throws an
  error when selecting a yanked version of a module."
  (https://bazel.build/external/module, read 2026-11-07) — verified.
- **vcpkg**: "vcpkg selects the lowest version that matches all constraints,
  so a less-than constraint is not required." "Declaring an override forces
  vcpkg to ignore all other version constraints and use the version specified
  in the override. This is useful for pinning exact versions and for resolving
  version conflicts." "Baselines, like other registry settings, are ignored
  from ports consumed as a dependency."
  (https://learn.microsoft.com/en-us/vcpkg/users/versioning, read 2026-11-07;
  the page does not name MVS or Go) — verified.

**The solver's side, in Cargo's pages.**

- "Generally, Cargo prefers the highest version currently available." "In
  general, the stricter you make the dependency requirements, the more likely
  it will be for the resolver to fail. Conversely, if you use requirements that
  are too loose, it may be possible for new versions to be published that will
  break the build." And of mixed Rust versions: "the resolver may pick a lower
  dependency version than necessary." "Or the resolver may pick too high of a
  version." (https://doc.rust-lang.org/cargo/reference/resolver.html, read
  2026-11-07) — verified.
- **Untested lower bounds.** Issue #5657, "Dependencies resolution with
  `--minimal-versions`", opened June 26, 2018, open that day, labelled
  `S-needs-design`: "Not even cargo itself can be compiled with
  minimal-versions as explored in #5275." (klausi, Jun 27, 2018); "Anything
  (transitive or direct dep) can have a version higher than what you want,
  messing up minimal version testing." (epage, Aug 9, 2024)
  (https://github.com/rust-lang/cargo/issues/5657, read 2026-11-07) —
  verified.
- **The lock file's guidance.** *Change in Guidance on Committing Lockfiles*,
  Aug. 29, 2023: "it isn't always practical to assume everyone is using the
  latest Rust release." "A lockfile is an appropriate way to pin versions for
  your project so you can validate your MSRV." Developers were "putting
  upperbounds on their version requirements", "likely being a worse solution."
  (https://blog.rust-lang.org/2023/08/29/committing-lockfiles/, read
  2026-11-07) — verified.
- The solver family: pub, SwiftPM, uv and Gleam use PubGrub (survey Tables
  2.3, 2.7, 2.8, 2.9).

**The page closest to the author's *exact version, one version per program*.**
Cargo's own resolver page says stricter requirements make the resolver more
likely to fail; Go reads every written version as a minimum, so two pins of one
module meet at the higher one, and pays for it in `exclude`, `replace` and
`retract`; vcpkg reads an exact pin as a root-only override. That is what the
three pages state; that exact pins under a solver would fail more often in
Heroes' first batch is an inference, unrun.

---

## 5. A static index: what moving off a git index fixed, in each project's words

| system | the move | when | why, in the project's words | verified |
|---|---|---|---|---|
| **Cargo / crates.io** | git index to the sparse HTTP protocol | RFC 2789 started 2019-10-18; stabilized in Rust 1.68, 2023-03-09; default in 1.70, 2023-06-01 (survey 2.2) | RFC 2789: "Selective download of the crates-io index over HTTP, similar to a solution used by Ruby's Bundler"; "the proposed solution works with static files and doesn't require custom server-side APIs"; the full index "will keep growing as crates.io grows"; "especially slow and wasteful in stateless CI environments, which download the full index, use only a tiny fraction of it, and throw it away"; "Shallow clones or squashing of git history are only temporary solutions"; as the reader rendered its figures, 176 MiB uncompressed, 16 MiB with `gzip -1`, 10 MiB with `xz -6`, a git clone downloading 215 MiB, "over **20 times more**" than a compressed tarball (https://rust-lang.github.io/rfcs/2789-sparse-index.html, read 2026-11-07). Rust 1.68: "The prior git protocol (which is still the default) clones a repository that indexes _all_ crates, but this has started to hit scaling limitations, with noticeable delays while updating that repository"; the new protocol "will only download information about the subset of crates that you actually use." (https://blog.rust-lang.org/2023/03/09/Rust-1.68.0/, read 2026-11-07) | verified |
| **CocoaPods** | a GitHub-hosted specs repository to a CDN | rate limiting, 4 May 2016; CDN the default in 1.8, beta of 5 August 2019 | *Master Spec Repo Rate Limiting Post Mortem* (Eloy Durán): users were "hitting CPU rate limits on their end", caused "besides the high volume of clones, by the default options we chose to clone this repo"; with shallow clones "the GitHub servers had to spend much more time figuring out which Git objects the user already has" (https://blog.cocoapods.org/Master-Spec-Repo-Rate-Limiting-Post-Mortem/, read 2026-11-07). 1.8.0 beta: "With 1.8, CocoaPods no longer requires cloning the now huge master specs repo in order to function"; "It aims to speed up initial setup and dependency analysis dramatically." (https://blog.cocoapods.org/CocoaPods-1.8.0-beta/, read 2026-11-07) | verified |
| **Homebrew** | the `homebrew/core` and `homebrew/cask` git taps to JSON files | 4.0.0, 2023-02-16 | "Using JSON files downloaded from formulae.brew.sh for package installation" "rather than local homebrew/core and homebrew/cask taps"; auto-updates "will be much faster as they no longer need to perform the slow `git fetch`" of the "huge homebrew/core and homebrew/cask taps' Git repositories." (https://brew.sh/2023/02/16/homebrew-4.0.0, read 2026-11-07) | verified |
| **Go** | none: plain HTTP from the first protocol | default mirror since go1.13, 2019-09-03 (survey 2.1) | "even a site serving from a fixed file system (including a `file://` URL) can be a module proxy" (survey Table 3) | verified (survey) |

**What the pages state, together.** Three package systems that kept their
index in a git repository moved it to files over HTTP, each page naming the
clone's size or time, or the host's load, as the cause (Cargo, CocoaPods,
Homebrew); Go's protocol was static files from its first form. This seat did
not look for a move the other way. For row 78 this is the precedent behind a
static index as the first variant (question 8).

---

## 6. The two candidates as wholes: which corrections were forced by a choice the other did not make

The survey's timelines (2.1 and 2.2) list the corrections. For each, the cause
**as its own page states it**; then whether the other system made that choice,
from the survey's inventory. The link between the two columns is this seat's
reading, marked as such, because no page read compares the two systems on that
correction except where the row says so.

| correction | release, date | the cause, in the page's words | the other system and that choice |
|---|---|---|---|
| **Go**, `+incompatible` and the `/v2` suffix | modules, go1.11, 2018-08-24 (survey) | "The major version suffix requirement was introduced when module support was added to the `go` command, and many repositories had already tagged releases with major version `2` or higher before that. To maintain compatibility with these repositories, the `go` command adds an `+incompatible` suffix" (https://go.dev/ref/mod, read 2026-11-07) | Cargo had versions from crates.io's first day: "This repository serves as permanent storage for releases of crates over time" (https://blog.rust-lang.org/2014/11/20/Cargo/, read 2026-11-07). **Forced by legacy**, an ecosystem tagged before the tool; Heroes has no published package (reading). |
| **Go**, the checksum database | design 2019-04-24; default go1.13, 2019-09-03 | "today's downloads—the ones that add or update dependencies in the first place—are not authenticated." "the `go` command today blindly trusts that it downloads the correct code." (Russ Cox and Filippo Valsorda, https://raw.githubusercontent.com/golang/proposal/master/design/25530-sumdb.md, read 2026-11-07; the page compares with no central registry, as the reader rendered it) | crates.io's index line carries `cksum`, "A SHA256 checksum of the `.crate` file" (survey Table 3). Whether that is a protection of the same kind: no page read says (reading). |
| **Go**, `retract` | go1.16, 2021-02-16 | MVS "keeps using specific package versions until an explicit upgrade is done" (issue #24031, § 4) | crates.io's yank (survey Table 3); when it arrived: not found. |
| **Go**, graph pruning | `go 1.17`, 2021-08-16 | "we must load the `go.mod` file for all versions of dependencies" (§ 4) | Cargo resolves from the index's per-version lines (survey Table 3); no page read compares (reading). |
| **Go**, the strict `go` line and `toolchain` | go1.21.0, 2023-08-08 | "earlier versions of Go will compile it and silently succeed." (Russ Cox, *Forward Compatibility and Toolchain Management in Go 1.21*, 14 August 2023, https://go.dev/blog/toolchain, read 2026-11-07) | Cargo's `rust-version`, Rust 1.56, 2021-10-21, "Cargo will exit with an early error if that is not satisfied" (survey 2.2). **Both** corrected the same omission; not a difference between them. |
| **Go**, `go get` stops building | go1.18, 2022-03-15 (https://go.dev/doc/devel/release, read 2026-11-07) | "`go get` is now dedicated to adjusting dependencies in `go.mod`." (https://go.dev/doc/go1.18, read 2026-11-07) | No matching Cargo correction in the survey's timeline; Cargo's own adding verb, `cargo add`, arrived in Rust 1.62, June 30, 2022 (survey 2.2). |
| **Cargo**, the sparse index | 1.68, 2023-03-09; default 1.70, 2023-06-01 | the git index's clone (§ 5) | Go's proxy protocol was static HTTP from its first form (§ 5). |
| **Cargo**, feature resolver 2 | Rust 1.51, March 25, 2021 | "Cargo's default behaviour is to merge features for a single package"; a feature of a build-dependency enabled in the target build "will no longer compile because `std` is not available"; resolver 2 "can lead to some crates compiling more than once" (https://blog.rust-lang.org/2021/03/25/Rust-1.51.0/, read 2026-11-07; quotations are fragments as the reader returned them) | `go.mod` has no features directive in the survey's Table 1A inventory (reading). |
| **Cargo**, the `package` key advisory | Sept. 30, 2019 | old Cargo "will ignore `package` and download the wrong dependency" (§ 2) | `go.mod` gives no local name; `replace` replaces contents (survey Table 1A) (reading). |
| **Cargo**, minimal versions | open since June 26, 2018 | lower bounds untested under "the highest version currently available" (§ 4) | **The one row a page itself compares**: Go's MVS post names Cargo among systems that "use the newest version available" (§ 4). |
| **Cargo**, the lock-file guidance and the MSRV-aware resolver | Aug. 29, 2023; Rust 1.84, Jan. 9, 2025 (survey 2.2) | "it isn't always practical to assume everyone is using the latest Rust release" (§ 4) | Go reads a requirement as a minimum and builds without a lock file, by its author's claim (§ 4). |

**This seat's reading of the table, said as a reading.** Of Go's corrections,
one (`+incompatible`) was forced by a history Heroes does not have, and two by
choices Heroes would copy if it copied Go: MVS (retract, pruning) and fetching
from the code's own host (the checksum database). Of Cargo's, the sparse index
was forced by a git index Heroes need not have, the feature resolver by
features no position of the author asks for, and the minimal-versions, MSRV and
lock-file work by choosing the newest version under ranges, which the author's
*exact version* avoids only if the sitting does not adopt ranges. Neither
column is a measurement of which base is better; it is which costs each
history says it paid, and why.

---

## 7. The words: what each system calls its parts, and where one renamed

Counted by hand from the survey's rows 3 (Tables 2.1 to 2.12) and its Table 3:
the systems whose pages call **the unit with a version** a *package* are Cargo,
Dart, Zig, Swift, Gleam, Nimble, Goose and JSR, and Python calls it a
*distribution package*; **Go alone calls it a *module***, and Go's *package* is
a directory. Heroes' spec § 1 already says *a file is a module*, so copying
Go's words whole would give `module` two meanings in one language (a reading of
the two pages). The names of the other parts:

| system | a file | the unit with a version | the manifest | the fingerprint or lock file | the verbs that add and fetch | source |
|---|---|---|---|---|---|---|
| Go | source file; a *package* is a directory | module | `go.mod` | `go.sum` | `go get`, `go mod tidy`, `go mod download` | survey 2.1 |
| Cargo | a crate's source "may be subdivided into one or more separate modules" | package; a crate is "either a library or an executable program" | `Cargo.toml` | `Cargo.lock` | `cargo add`, `cargo update` | survey 2.2 |
| Dart | library | package | `pubspec.yaml` | `pubspec.lock` | `dart pub add`, `dart pub get` | survey 2.3 |
| Zig | source file, imported as its struct type | package | `build.zig.zon` | none; the hash sits in the manifest | `zig fetch` | survey 2.4 |
| Gleam | module | package | `gleam.toml` | `manifest.toml` | `gleam add`, `gleam deps download` | survey 2.8 |
| Goose | module | package | the root file's import lines | none | `goose fetch --update` | survey 2.12 |

**Renames whose pages give a reason.**

- **Go's verb.** go1.18, 2022-03-15: "`go` `get` no longer builds or installs
  packages in module-aware mode. `go` `get` is now dedicated to adjusting
  dependencies in `go.mod`." Installing became `go install
  example.com/cmd@latest` (https://go.dev/doc/go1.18, read 2026-11-07). —
  verified.
- **Go's noun.** *Defining Go Modules* (Russ Cox, February 22, 2018): "a Go
  module is a collection of packages versioned as a unit" (as the reader
  rendered the sentence's core); why the word *module* was chosen: not stated
  on that page (https://research.swtch.com/vgo-module, read 2026-11-07). —
  verified (reason: not found).
- **Python's two packages.** The PyPA glossary keeps both senses and says why
  it spells them out: a distribution package "is more commonly referred to with
  the single words “package” or “distribution”, but this guide may use the
  expanded term when more clarity is needed to prevent confusion with an Import
  Package (which is also commonly called a “package”)"
  (https://packaging.python.org/en/latest/glossary/, read 2026-11-07). —
  verified.
- **Zig's home for the verbs.** 0.17.0, 2026-10-01: "All package management
  functionality has been moved out of the Compiler and into the Build System."
  (survey 2.4). — verified (survey).

Heroes already uses *package* for the core packages (panel 234) and spec § 13
writes `package "raylib"` for a C library's link flags (the critic's finding
9, `critic-briefs.md`), so the word *package* would carry a third sense if it
named the unit with a version; this is a fact of the tree as the critic read
it, which this seat did not re-run.

---

## 8. The index: what it had to deliver first, what it added later, and names

| index | at its start, from its own pages | added later, from its own pages |
|---|---|---|
| **crates.io** | 2014-11-20: "Cargo is ready to start publishing to it today."; "This repository serves as permanent storage for releases of crates over time". Yanking, namespaces and signing: not mentioned in the launch post (https://blog.rust-lang.org/2014/11/20/Cargo/, read 2026-11-07). | The sparse index, 2023 (§ 5). Trusted Publishing, July 11, 2025 (survey Table 3). **Namespaces**: RFC 3243, started 2022-03-09, "allowing crate owners to create crates like `parent::foo`" (https://rust-lang.github.io/rfcs/3243-packages-as-optional-namespaces.html, read 2026-11-07); its tracking issue #122349 open that day, "Basic cargo support is in as of rust-lang/cargo#13591" (March 17, 2024) (https://github.com/rust-lang/rust/issues/122349, read 2026-11-07). **A typosquat removed**, May 10, 2022: the name "was intentionally similar to the name of the popular" `rust_decimal`; "The crate had less than 500 downloads since its first release on 2022-03-25"; "the crates.io team permanently removed the crate from the registry as soon as it was made aware of the malware" (https://blog.rust-lang.org/2022/05/10/malicious-crate-rustdecimal/, read 2026-11-07). When yank arrived: not found. |
| **npm** | scopes not at first: "As of 2015-04-19, and with npm 2.0 or better, the primary npm registry does support scoped packages."; "Each npm user/organization has their own scope, and only you can add packages in your scope." (https://docs.npmjs.com/cli/v11/using-npm/scope, read 2026-11-07) | The unpublish policy after left-pad, March 2016 (survey 2.6); trusted publishing (survey Table 3). |
| **PyPI** | (its start was not looked for) | **Yank**: PEP 592, created 07-May-2019, Final: "the obvious solution of deleting the existing file from a repository will break users who have followed best practices" (https://peps.python.org/pep-0592/, read 2026-11-07). **Signing removed**: *Removing PGP from PyPI* (Donald Stufft; the address dates it 2023-05-23, the audit 2023-05-19): "In the last 3 years, about 50k signatures had been uploaded to PyPI by 1069 unique keys." "about 30% of them were not discoverable on major public keyservers"; "only 36% of them were capable of being meaningfully verified"; "that would still represent only 0.3% of all of those files." (https://blog.pypi.org/posts/2023-05-23-removing-pgp/, read 2026-11-07). Trusted Publishing (survey Table 3). |
| **Go's proxy and checksum database** | No accounts: "In Go, there is no such thing as a package repository account." (survey Table 3) | The mirror and database by default, go1.13, 2019-09-03; `retract`, go1.16 (survey 2.1). **A cached typosquat**: Socket, February 4, 2025 (Kirill Boychenko): `github.com/boltdb-go/bolt`, "a malicious typosquat package in the Go ecosystem, impersonating the widely used BoltDB database module"; "The malicious package was uploaded to the Go Module Proxy in November 2021"; "the `git` tag was strategically altered on GitHub to remove traces of malware"; "This deception went undetected for over three years"; "Go modules are intentionally immutable once published and fetched by the proxy"; "there is no patch or switch to turn off immutability" (https://socket.dev/blog/malicious-package-exploits-go-module-proxy-caching-for-persistence, read 2026-11-07; a security vendor's account, the finder's own). |

**What the pages state, together, for row 78.** No index read began with
namespaces or signing; publishing and permanence came first (crates.io's
launch post). Withdrawal by a mark rather than deletion is stated by PyPI in
2019 and by crates.io (survey Table 3). The one signing scheme found on an
index, PyPI's PGP, was removed in 2023 with its own numbers: 0.3% of files
covered. Trusted publishing reached crates.io, npm and PyPI after the indexes
were long running (survey Table 3). And the two recorded attacks are on the
two naming schemes the candidates use: a first-come name typosquatted on
crates.io, and a path-named module on Go's immutable mirror that kept serving
what it first cached after the source's tag was rewritten.

---

## 9. What models write: any page that measures Go's or Cargo's files

**None found.** The question stays a question: *do models write Go's `go.mod`
and `go.sum`, or Cargo's `Cargo.toml` and `Cargo.lock`, right more often?* The
searches are in § search. What was found measures something nearby, and is
cited only for what it states:

- **Krishna, Galinkin, Derczynski, Martin, *Importing Phantoms: Measuring LLM
  Package Hallucination Vulnerabilities*** (arXiv:2501.19012, v1 31 January
  2025): languages JavaScript, Python and Rust; "Rust has the highest mean
  package hallucination rate (24.74%)", Python 23.14% and JavaScript 14.73%
  (the last two as the reader rendered them); hallucinations were read from
  **import statements in generated code** by regular expressions and checked
  against a registry snapshot, **not from manifest files**; Go not included
  (https://arxiv.org/html/2501.19012, read 2026-11-07).
- **Haque, Lin, Arkoh, Tadesse, Xu, *Secure or Suspect? Investigating Package
  Hallucinations of Shell Command in Original and Quantized LLMs***
  (arXiv:2512.08213, 9 December 2025): Go packages in installation commands
  such as `go get`; "quantization substantially increases the package
  hallucination rate (PHR)"; most fabricated packages resemble realistic
  URL-based Go module paths, "a systematic pattern in how LLMs hallucinate
  dependencies" (the sentence's frame as the reader rendered it); no rates in
  the abstract (https://arxiv.org/abs/2512.08213, read 2026-11-07).
- **Zheng, Guan, Liu, *When LLMs Invent Rust Crates*** (arXiv:2606.08444, v1
  7 June 2026, v2 14 August 2026): "first large-scale empirical study on crate
  hallucination in LLM-generated Rust code"; "different models exhibit
  surprisingly consistent hallucination rates"; the abstract gives no rate and
  does not say whether `Cargo.toml` was measured
  (https://arxiv.org/abs/2606.08444, read 2026-11-07).
- **Wang, Wu, Ling, Luo, Zhao, *Correct Code, Vulnerable Dependencies***
  (arXiv:2605.06279, 7 May 2026): Python only; "LLMs tend to specify version
  identifiers when directly prompted at 26.83%-95.18%, while down to
  6.45%-59.19% in creating a manifest file directly."
  (https://arxiv.org/abs/2605.06279, read 2026-11-07).

None of the four varies Go against Cargo, and none measures either system's
manifest. The reader test's habit count (the shared brief's question 21) is
the only instrument this sitting has on the author's criterion, and this seat
found no published one to set beside it.

---

## 10. Prior art for the resolution

For each form the sitting may adopt, the lines `.claude/rules/records.md`
§ And the record says whose idea it was asks of the decision issue, in its two
shapes. Each says only what its page states, every page read 2026-11-07 by
this seat or by the survey that day. **Which shape applies depends on the
synthesis**: *Taken from* where the resolution adopts a form on that page's
evidence, *Reached first by* where the author or a seat reached it and the page
was found after. A form the sitting does not adopt has no line.

**The bracket in the `use` line** (the author's position 7; the sitting rules
its grammar):

- **Reached first by C**: [ISO/IEC 9899:201x, N1570, §6.10.2](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): `# include <h-char-sequence>` "searches a sequence of implementation-defined places for a header identified uniquely by the specified sequence between the < and > delimiters" (the survey's reading).
- **Reached first by C++20**: [cppreference, Modules](https://en.cppreference.com/w/cpp/language/modules): "headers can also be imported as _header units_ with an import declaration", `import <iostream>;`.
- **Reached first by Rust's anchored-paths variant**: [withoutboats, Anchored and Uniform Paths](https://without.boats/blog/anchored-uniform/): the use statements of anchored paths are syntactically disambiguated as to where they come from (as the reader rendered it; a line from this page needs that sentence re-read word for word before it is filed).

**A name in code that a manifest resolves** (position 3):

- **Reached first by Cargo**: [Specifying Dependencies, Renaming dependencies](https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html): "you may wish to reference the crate with a different name in the code regardless of how it's published on crates.io"; "The `package` key, if not specified, defaults to the name of the dependency being requested."
- **Reached first by Node.js**: [Packages, subpath imports](https://nodejs.org/api/packages.html): "Entries in the `"imports"` field must always start with `#` to ensure they are disambiguated from external package specifiers"; "the `"imports"` field permits mapping to external packages."
- **Reached first by Deno**: [What we got wrong about HTTP imports](https://deno.com/blog/http-imports): "Import maps allow you to get short and memorable specifiers back and manage versions across many files."

**The catalogue's format** (question 4), one line for the format adopted:

- TOML: **Taken from Python**: [PEP 518](https://peps.python.org/pep-0518/): "TOML was ultimately selected as it provided all the features we were interested in"; JSON "is more verbose than necessary while not allowing for comments"; YAML's "specification is large: 86 pages if printed on letter-sized paper".
- A line format of Heroes' own: **Reached first by Go**: [golang/go#23966](https://github.com/golang/go/issues/23966): "The fundamental problem is that not everything a developer needs to say is best expressed as key-value pairs."

**The pin and its fingerprint in the catalogue's entry** (question 5):

- **Reached first by Zig**: [build.zig.zon](https://raw.githubusercontent.com/ziglang/zig/master/doc/build.zig.zon.md): "Each dependency must either provide a `url` and `hash`, or a `path`." (the survey's reading).
- Over the files' names and contents, not the archive: **Taken from Go**: [Go Modules Reference](https://go.dev/ref/mod): "the `go` command computes the hash from the names and contents of files within the archive in a deterministic order"; "the `.zip` file itself is not hashed" (the survey's reading).

**Versions** (question 6), if minimal version selection is adopted:

- **Taken from Go**: [Minimal Version Selection](https://research.swtch.com/vgo-mvs): it "produces high-fidelity builds" and "leads to reproducible builds without lock files."
- **Reached first by Bazel**: [Bzlmod, Version selection](https://bazel.build/external/module): "Bazel uses the Minimal Version Selection (MVS) algorithm introduced in the Go module system."

**Fetching and building run no dependency's code** (robustness, question 13):

- **Taken from Go**: [How Go Mitigates Supply Chain Attacks](https://go.dev/blog/supply-chain): "It is an explicit security design goal of the Go toolchain that neither fetching nor building code will let that code execute, even if it is untrusted and malicious." (the survey's reading).

**A build that does not rewrite the catalogue** (question 18):

- **Taken from Go**: [Go Modules Reference, Automatic updates](https://go.dev/ref/mod): "Since Go 1.16, the `go` command acts as if `-mod=readonly` were set instead: if any changes to `go.mod` are needed, the `go` command reports an error and suggests a fix." (The same page: "The `go` command will automatically download modules as needed during ordinary execution", so Go's build still fetches; a line is filed only for what the sitting adopts.)

**`internal/`** (question 12):

- **Taken from Go**: [Go 1.4 Release Notes](https://go.dev/doc/go1.4): "When the `go` command sees an import of a package with `internal` in its path, it verifies that the package doing the import is within the tree rooted at the parent of the `internal` directory." (the survey's reading).

**The index** (questions 8 and 15):

- A static index: **Taken from Go**: [Go Modules Reference, GOPROXY protocol](https://go.dev/ref/mod): "even a site serving from a fixed file system (including a `file://` URL) can be a module proxy" (the survey's reading); **Reached first by Cargo**: [RFC 2789](https://rust-lang.github.io/rfcs/2789-sparse-index.html): "the proposed solution works with static files and doesn't require custom server-side APIs."
- A version is never overwritten and is withdrawn by a mark: **Taken from crates.io**: [Publishing on crates.io](https://doc.rust-lang.org/cargo/reference/publishing.html): "The version can never be overwritten, and the code cannot be deleted." (the survey's reading); **Taken from Python**: [PEP 592](https://peps.python.org/pep-0592/): "the obvious solution of deleting the existing file from a repository will break users who have followed best practices".

---

## Verdict row (advisory)

| seat | verdict | design.md | cost or delta | prediction, and the milestone that scores it | condition that would change it |
|---|---|---|---|---|---|
| historian | **approve** (advisory; no veto) | §1.11 (no standard library, as §3.5 cites it), §1.12 (robustness, as CLAUDE.md § Precedence cites it), §3.5 (`heroes add`/`heroes fetch`, panel 056's conditions, read at `docs/design.md:887-905` in the lane) | none of its own: it adds no form; it asks for lines, § 10 | **One:** if one module of fetched code can be reached both with brackets and by a path without them, the first version will compile it as two modules, Dart's recorded failure; scored by M-package-manager's goldens, a case naming one file both ways. **Two:** if the sitting adopts a solver with exact pins, the first two packages of M-core-packages' resumed batch that pin different versions of one dependency will fail to resolve, as Cargo's resolver page states of strict requirements; under MVS they will meet at the higher pin; scored at M-core-packages' resumption. | below, `condition` |

## precedents

The sections above are the precedents, each row with its URL, day and mark.
In one line each:

- Rust weighed a mandatory own-or-external mark (anchored paths) and refused it
  in 2018; `::` stays optional (§ 1) — verified.
- Dart compiles one file reached by two spellings as two libraries (§ 1) —
  verified.
- Node's `node:`, added to `require` in v16.0.0 and v14.18.0 against
  collisions with names users already held; webpack's issue on the unknown
  scheme stood open from May 2 to June 21, 2021 (§ 1) — verified.
- Nim's `std/` stays optional outside official projects (§ 1) — verified;
  usage measured anywhere — not found.
- Deno moved from URL imports to `npm:` and `jsr:` and wrote down why (§ 1) —
  verified.
- Cargo's alias key was ignored by older Cargo, a squatting risk (§ 2) —
  verified; Deno's map scope reaches one level (§ 2) — verified; Swift's
  aliasing has stated limits (§ 2) — verified.
- Go kept a format of its own after weighing TOML, JSON and YAML; Python chose
  TOML over the same three; Cargo and Bun reformatted lock files against merge
  conflicts (§ 3) — verified.
- Go's MVS and its costs (retract, pruning); Bazel and vcpkg adopted minimum
  selection; Cargo's solver and its costs (minimal versions, MSRV, the lock
  guidance) (§ 4) — verified.
- Cargo, CocoaPods and Homebrew moved their index off git to HTTP files (§ 5)
  — verified.
- Each candidate's corrections and their stated causes (§ 6) — verified as
  pages; the links between columns are readings.
- No index began with namespaces or signing; PyPI removed PGP; a typosquat on
  crates.io and a cached typosquat on Go's mirror (§ 8) — verified.
- A measurement of models writing Go's or Cargo's files — not found (§ 9).

## argument

Precedent supports the author's frame and names two hazards. The bracket is
the anchored side of a choice Rust weighed and refused in 2018; Heroes takes
it knowingly. Dart records what follows when one file is reachable by two
spellings: two libraries. A manifest alias works (Cargo, Node's `#` imports),
but a reader that ignores it fetches the wrong name (Cargo's 2019 advisory).
For *exact version, one version per program*, Go's pages describe the closer
fit: a pin read as a minimum, no lock file, fingerprints over file contents, no
code run at fetch or build, a static index. Cargo's recorded corrections trace
mostly to ranges, features and a git index. No page measures which base models
write better.

## condition

This seat's reading would change on: a page recording a language that
required a mark on fetched imports and dropped it for a stated, measured cost;
a page recording MVS failing a program that a solver resolved, in a setting of
exact pins and one version per program; or a measurement, the reader test's
habit count included, that models write Cargo's files right more often than
Go's.

## search

All pages were fetched on 2026-11-07 with the page-reading tool; the web
searches are listed with their exact queries.

- **Marks in the import line.** Pages: nodejs.org's `modules.html` (the
  `node:` history and the mandatory list) and `packages.html` (subpath
  imports); Dart's Effective Dart *Usage*; NEP 1 on nim-lang.org; the Rust
  edition guide's path-changes page; the Rust 1.31 and 1.32 announcements;
  rust-lang/rust#55618; without.boats *Anchored and Uniform Paths*; the Rust
  Reference's use-declarations and name-resolution chapters (read for a rule
  on a local module against an extern crate in `use`: **not found** in either;
  the edition guide's sentence is the one cited); GCC's *Directory Options*;
  cppreference's *Modules*; Deno's *What we got wrong about HTTP imports* and
  the 1.28 announcement; webpack#13290; MacWright's 2025 article (commentary;
  cited only for what it saw). Searches: `nim RFC make "std/" prefix
  mandatory imports stdlib` (no RFC found; NEP 1 read instead); `"node:"
  protocol prefix adoption percentage packages npm built-in modules "node:fs"
  usage study` (no measurement found); `Rust 2018 "anchored paths" "uniform
  paths" lang team decision`; `webpack issue "UnhandledSchemeError" "node:"
  scheme "is not handled by plugins" node: prefix`. **Looked for and not
  found**: a page measuring how often users write an optional mark (`node:`,
  `std/`, `pkg/`, Rust's `::`), in Node's and Nim's documentation and the
  searches above; Dart's date for `package:`.
- **Manifest aliases.** Pages: Cargo's *Specifying Dependencies*;
  rust-lang/cargo#1311; the Rust advisory of 2019-09-30 and RustSec's
  CVE-2019-16760; SE-0339; denoland/deno#23612; JSR's *Publishing packages*;
  WICG's import-maps repository (read for a sentence on import maps being
  application-level: the page is an archived pointer and carried none; not
  cited). Searches: `rust-lang/cargo issue renamed dependency "package =" bug
  features rename`; `deno import map "not applied" dependencies library
  publish jsr bare specifiers issue denoland/deno import map only root`. A
  Deno help thread on questions.deno.com answered 429 and is not cited.
- **Formats.** Pages: PEP 518 (re-read for the abstract and the rejected
  formats); dep's `Gopkg.toml` documentation; golang/go#23966;
  rust-lang/cargo#7070; Bun's text-lockfile post. Search: `Russ Cox go.mod
  file format why not TOML vgo "go.mod" syntax design decision`.
- **Versions.** Pages: *Minimal Version Selection*; golang/go#24031; the lazy
  module loading design; Bazel's Bzlmod page; vcpkg's versioning reference;
  Cargo's resolver reference; rust-lang/cargo#5657; the 2023 lockfile post; the
  Rust 1.51 announcement.
- **Indexes.** Pages: RFC 2789; the Rust 1.68 announcement; CocoaPods' 2016
  post-mortem and 1.8.0 beta post; Homebrew 4.0.0; the 2014 Cargo post; RFC
  3243 and rust-lang/rust#122349; the rustdecimal advisory; npm's *scope*
  page; PEP 592; PyPI's *Removing PGP*; Socket's 2025 report; the Go
  checksum-database design. Searches: `CocoaPods blog CDN default spec repo
  trunk GitHub master specs repo clone slow 1.8`; `Homebrew 4.0.0 blog JSON
  API no longer need to tap homebrew/core git clone`; `npm blog "scoped
  packages" npm 2.0.0 2014 announcement` (secondary results only; npm's own
  page read instead); `Rust blog security advisory malicious crate rustdecimal
  crates.io typosquatting 2022`; `Go module mirror proxy.golang.org cached
  malicious typosquat boltdb-go backdoor years Go team response` (secondary
  results; Socket's own post read instead). **Looked for and not found**: when
  crates.io's yank arrived (the 2014 launch post); a sentence of the Go team on
  the boltdb case on a Go page (the search returned press coverage only, not
  cited).
- **The two candidates.** Pages: the Go Modules Reference (twice: major
  version suffixes and `+incompatible`; build commands and automatic
  updates), *Forward Compatibility and Toolchain Management in Go 1.21*, the
  Go 1.18 notes, Go's release history. **Looked for and not found**: a page
  comparing Go's modules and Cargo as wholes, beyond the MVS post's one
  sentence naming Cargo.
- **Words.** Pages: *Defining Go Modules*; the Go 1.18 notes; the PyPA
  glossary. **Not found**: why Go chose the word *module* (that post).
- **Models.** Pages: arXiv 2501.19012 (abstract and HTML), 2512.08213,
  2606.08444, 2605.06279. Searches: `LLM package hallucination study Rust
  crates Go modules hallucinated dependency names measured languages`; `arxiv
  package hallucination Go modules LLM quantization "Go" module paths
  fabricated study`; `LLM generated Cargo.toml OR go.mod dependency versions
  outdated hallucinated version numbers empirical study manifest`; `benchmark
  LLM agents build environment setup success rate by language Rust Cargo Go
  modules compile repository configuration 2025` (results on environment
  setup, not on writing the files; their pages were not read and none is
  cited). **Looked for and not found**: any page measuring how models write
  `go.mod`, `go.sum`, `Cargo.toml` or `Cargo.lock`, or comparing the two;
  asked as a question in § 9.
- **The lane.** Read: the briefs `historian.md`, `00-shared.md`,
  `proposal.md`; the survey; `critic-briefs.md`; the milestone page
  `docs/roadmap/milestones/M-package-manager.md`; `docs/design.md` lines 880
  to 919 for §3.5. *Heroes of code* was not opened for this report.

report complete
