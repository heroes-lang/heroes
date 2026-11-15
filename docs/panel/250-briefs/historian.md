# Panel 250, historian

Read `00-shared.md` first, whole: the question, the base (rebased to `48f53b343` after the critic's
first pass), the words (*entry*, *module*, *face*, *Go package*, *placed*, *reached*), the project's own
history (§ Before whole placement: the examples held only the Go packages they reached from `43aae88d8`
to `31c34d962`, and whole placement came with panel 235's R15) and the routes (W, M, G, H; F1 to F4;
R2's ruler moved; `core` split; trees not committed; the nine nobody listed in the first briefs). You judge precedent, advisory as always: who else placed or vendored only what a program
uses, what each one's lock or sum covers, what each verifies and when, and what each had to correct.
Your report is `docs/panel/250-reports/historian.md` in `.claude/worktrees/p250`, in your definition's
output structure (`.claude/agents/historian.md`), **written by you, as you go** (`SKILL.md` 3c). You build
nothing and run no paid call.

**Every precedent is a page you read today**, its URL and the day beside it, and says only what the page
states: a comparison is not an adoption, a mechanism the page does not describe is not cited, a page you
cannot read is not cited (`.claude/rules/records.md` § And the record says whose idea it was: of 90
proposed prior-art lines, 33 said more than their page did). Mark each `verified` or `unverified`. End
with candidate prior-art lines per route in the two forms that section allows, `Taken from` or `Reached
first by`. Your `search` field names the queries and sources swept, so a sentence about something not
found says what was looked for and where.

**Where a fetched page lands**: panel 247's historian found its fetching tool had saved one page outside
the repository's root (`docs/panel/247-reports/historian.md` § One thing written outside the
repository's root). CLAUDE.md § Hard stops writes no file outside the root: if your tool saves a file,
say where, and if it is outside `/Users/joseph/Temp/heroes/heroes-lang`, report it as 247's did.

**Already read by earlier historians: cite them, do not repeat them**, and re-read one only where this
sitting leans on a sentence that report did not quote, saying so:

- **Panel 235's survey** (`docs/panel/235-reports/historian-survey.md`, 863 lines), its § 2.1 Go
  (191), 2.2 Cargo (281), 2.4 Zig (385; its row 2 quotes the manifest's *"Only files included in the
  package are used to compute a package's `hash`"*), 2.5 Deno (414), 2.6 npm (433), 2.9 Python with
  uv's `uv.lock` and `pylock.toml` (518); and panel 235's report (`235-reports/historian.md`), whose
  line 408 quotes Go's reference: *"the `go` command computes the hash from the names and contents of
  files within the archive in a deterministic order"*, *"the `.zip` file itself is not hashed"*.
- **Panel 242's report** (`docs/panel/242-reports/historian.md`): Go's `src/cmd/vendor` with
  `modules.txt` and `moddeps_test.go` (its lines 69-124); **what a Go build does with a vendor folder**
  (125-136: *"When vendoring is enabled, build commands like `go build` and `go test` load packages from
  the `vendor` directory"*; *"When the `go` command reads `vendor/modules.txt`, it checks that the
  module versions are consistent with `go.mod`"*; *"`go mod verify` ... does not use `go.sum` files to
  verify module content"*, and the seat's own *"that it does not is an inference from that silence,
  unrun"*); Cargo's `.cargo-checksum.json` (235-241: *"It is not a security mechanism and does not
  protect against malicious changes"*); pip's vendoring policy (342-350).

The list below is the coordinator's, **written from memory and unverified**: each item is a question,
and a project named here that does not do what the line says is a finding, not an embarrassment.

## What each places or vendors, and at what grain

- **Go**: whether `go mod vendor` copies whole modules or only the packages the main module's packages
  and tests import, and what `vendor/modules.txt` lists per module (its packages?); whether it leaves out
  a dependency's test files; what `-mod=vendor` checks at build beyond `modules.txt` against `go.mod`;
  and the vocabulary map: a Go module is the unit of version and of `go.sum`, a Go package (a directory)
  the unit of compilation and of import. Does the reference say so in those words?
- **Cargo**: whether `cargo vendor` copies each crate whole (every file of the `.crate`), what
  `.cargo-checksum.json` holds (a `package` checksum of the archive and a `files` map of each file's
  SHA-256?), what Cargo does at build when a vendored file listed there is missing, changed or added,
  and whether a crate's `include`/`exclude` keys (in the manifest) shape what is published and so what
  is summed.
- **Deno**: what its vendor folder holds (`"vendor": true` in `deno.json`, and the older `deno vendor`
  subcommand, with the version that changed it): only the remote modules the module graph reaches, or
  whole packages; what `deno.lock` records for a `jsr:` package (a hash of the version's metadata, which
  lists each file with its checksum?) and for a remote URL (one hash per module?); and **whether Deno
  downloads only the files of a JSR package its graph reaches**. The index here already serves *"each
  file at its own address, as JSR serves them"* (`selfhost/fetch/index_source.hero:16-18`), so a JSR
  answer bears on route M with F1 directly.
- **npm**: what lands in `node_modules` (the published tarball whole?); what `package-lock.json`'s
  `integrity` covers (the tarball) and when it is checked (at install only?); and whether any npm page
  says it removes a package's unused files (bundlers' tree-shaking is outside npm: say where a page says
  so, or leave it).
- **pip and uv**: what a wheel installs (every file, listed in its `RECORD` with a hash and a size?);
  what `--require-hashes` and `uv.lock` hash (the archive?); and whether anything checks an installed
  file against `RECORD` after install.
- **Zig**: the `paths` field of `build.zig.zon` (the survey's row): who chooses the files a package's
  hash covers, the publisher or the consumer, and whether a consumer can fetch fewer.
- **Others you find**: any package manager that places a subset of a version by the importer's reach,
  and what its sum covers (Bazel's or Buck's per-target fetching, Swift's, Dart's, Gleam's, Nim's), each
  only where a page says so.

## What each one's sum covers, and when it is verified

For each system above: is the sum over the whole version (an archive, a tree, a listing) or per file or
per module; is it checked at fetch, at every build, or on a command (`go mod verify`); and what each says
of a placed or vendored copy edited by hand (refused, overwritten, ignored). This is the evidence for
F1 to F4 in 00-shared.md, and **R7's *"checked against its fingerprint at every build"***
(`docs/panel/235-*.md:635-651`) is the rule it is read against: say which system, if any, checks a
vendored copy at every build.

## What each corrected, and why

For every precedent above that changed its grain or its sum, the version, the date, the change and the
page that says why: a vendor command that went from whole modules to packages or back; a lock that
gained per-file hashes; a registry that started serving files one by one; a manager that stopped
committing vendored trees, or started. And for each: does the language's own reference say it, or only
the tool's documentation? Spec § 1 here names the catalogue, its `sum` and nothing of placement (`grep -n
'heroes.packages' spec/heroes-spec.md` prints nothing), as panel 244's R1 ruled (`244-*.md:95-96`,
*"Out: `heroes fetch` and `heroes.packages/`"*): say which precedents put placement or the sum's
coverage in the language's reference and which leave it to the tool's page.

## Precedent for the routes nobody listed in the first briefs

00-shared.md lists nine. Where a page you read bears on one, say which: a listing or per-file hashes kept
beside the manifest rather than in the tree (route 4: Cargo's `.cargo-checksum.json`, a JSR version's
metadata, a wheel's `RECORD`); a publisher declaring which files a package's hash covers (route 6: Zig's
`paths`, the survey's row at `235-reports/historian-survey.md:395`); a vendor or placement step chosen by
a flag, the whole by default (route 8); a copy per program held by a check rather than a sum at every
build (route 1, the project's own grain from `43aae88d8` to `31c34d962`). Only where a page says so.

## Why this project went from the reached grain to the whole

The examples held only the reached Go packages until `31c34d962` (2026-11-08), whose body says each
program holds *"the tree it placed"*; the critic's search of panel 235's record, reports and briefs for a
weighing of a subset against the whole printed nothing about placement (its § B, carried). Read panel
235's survey and report for any page they quoted on a manager placing a subset, or on why one places the
whole, and say what you find, or that you found none and where you looked.

## The two alternatives

- **A ruler counted by the modules reached rather than the files placed**: any toolchain that measures
  its own dependencies' cost by what it compiles rather than what it carries (Go's or Rust's bootstrap
  notes, the 242 report's § on Go and rustc), only where a page says so.
- **A standard set split into small packages**: who split a large first-party package into several
  versioned units, or merged several into one (Go's `golang.org/x/...` modules, Rust's `std` against
  crates, Deno's `@std/*` on JSR with a version each), with the date and the reason the page gives.
