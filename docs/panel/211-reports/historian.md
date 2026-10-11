# Panel 211, historian (advisory, no veto)

Written as I went, 2026-10-11, from about 02:15. **Times are approximate**:
this seat had no shell, so `date` and `caffeinate` were not run, and nor was
a grep of `docs/panel/` or `issues/` for an earlier sitting on `--apply`
(whether one exists is a question, not a premise). Instrument: web search
and WebFetch only, no build. Every claim below carries the URL it was read
at, and a claim I could not fetch is marked **unverified**. A quotation
from source code is what the fetched page held tonight, not a release I
pinned. Where a fetch summarised a page, only the quoted words are its own.

## verdict

**approve (advisory)**: route W, bounded by route L, each file written whole
from a plan made before the first write, every written file named, and the
exit code nonzero when any planned file was not written.

## precedents

### cargo fix (Rust): the closest precedent, a root that reaches modules

- **verified**: by default it fixes every target of the selected packages
  ("When no target selection options are given, `cargo fix` will fix all
  targets (`--all-targets` implied)"), the package set being the manifest's
  package or the workspace's default members, `--workspace` for all.
  https://doc.rust-lang.org/cargo/commands/cargo-fix.html
  A crate is a root file reaching module files through `mod`, and the fixes
  are rustc's over the whole crate, so `cargo fix` writes files the command
  line never named (the module page below: rustfix applies "the suggestions
  to the files on the file system").
- **verified**: it refuses a dirty tree unless told: `--allow-dirty` "Fix code
  even if the working directory has changes (including staged changes)",
  `--allow-staged`, `--allow-no-vcs` (same URL). The message: "the working
  directory of this package has uncommitted changes, and `cargo fix` can
  potentially perform destructive changes; if you'd like to suppress this
  error pass `--allow-dirty`, or commit the changes to these files:", each
  file marked `(dirty)` or `(staged)`.
  https://doc.rust-lang.org/stable/nightly-rustc/src/cargo/ops/cargo_fix/mod.rs.html
- **verified**: its safety is a re-check and a back-out, not an atomic write:
  "rustc is run again to verify the suggestions didn't break anything. The
  change will be backed out if it fails (unless `--broken-code` is used)."
  https://doc.rust-lang.org/stable/nightly-rustc/cargo/ops/cargo_fix/index.html
  The back-out writes each touched file's saved `original_code` back with
  `paths::write`, one after another, in `fix_exec_rustc` ("Back out all of
  the changes unless --broken-code was used"); a file's original is saved
  the first time it is touched (`rustfix_and_fix`). A lock per package:
  "It ensures each package has only one fix running at once".
  https://doc.rust-lang.org/stable/nightly-rustc/src/cargo/ops/cargo_fix/mod.rs.html
- **verified**: it names each file written, a status line `Fixed {file}
  ({fixes} fix|fixes)`, and a failed re-check reports "errors present after
  applying fixes" with each file listed and "to possibly apply more fixes,
  pass in the `--broken-code` flag".
  https://doc.rust-lang.org/stable/nightly-rustc/src/cargo/util/diagnostic_server.rs.html
- **verified, what went wrong**: writing every file a diagnostic names
  reached past the user's code. cargo#9857, opened 2021-08-30, "`cargo fix`
  may attempt to write to the registry cache": "cargo should never apply
  fixes to non-path dependencies"; still open, labelled 2025-11-07 as
  needing design work. https://github.com/rust-lang/cargo/issues/9857
  Its rustc side, rust#88514, the same day, "Diagnostic suggestions sometimes
  incorrectly point to external crates", the cases a crater run found all
  caused by macros; open. https://github.com/rust-lang/rust/issues/88514
- **verified, how it was softened**: by guards on where a write may land,
  never by retreating to the named file. PR #9938, merged 2021-10-07, "Skip
  all `cargo fix` that tends to write to registry cache", "a temporary hack
  for #9857". https://github.com/rust-lang/cargo/pull/9938
  PR #13792, merged 2024-04-30, "fix(cargo-fix): dont fix into standard
  library", which stops "any write into sysroot" and **rejected limiting
  fixes to paths under the workspace root** because of `#[path]` attributes
  and path dependencies outside the workspace.
  https://github.com/rust-lang/cargo/pull/13792
- **verified**: the edition guide's migration is one command over the
  package, `cargo fix --edition`, and advises committing its changes
  separately; it says the migration "does not update doctests", and macros
  may need manual updating.
  https://doc.rust-lang.org/edition-guide/editions/transitioning-an-existing-project-to-a-new-edition.html

### rustfmt (Rust): the same walk in a rewriting tool

- **verified**: "This runs rustfmt on the given file, if the file includes
  out of line modules, then we reformat those too. So to run on a whole
  module or crate, you just need to run on the root file (usually mod.rs or
  lib.rs)." https://raw.githubusercontent.com/rust-lang/rustfmt/master/README.md
- **verified**: the root-only behaviour exists only as an opt-out,
  `skip_children`, "Don't reformat out of line modules", default `false`,
  "Stable: No (tracking issue: #3389)".
  https://raw.githubusercontent.com/rust-lang/rustfmt/master/Configurations.md
  #3389 was opened 2019-02-13 and closed 2020-06-07, a 2019 comment pointing
  at "deprecation in #3893", while the page above still lists the option.
  https://github.com/rust-lang/rustfmt/issues/3389
  Why it never stabilised: **unverified**, the issue page does not say.

### go fix and gofmt -w (Go): the named packages, never their imports

- **verified**: the old `go tool fix` rewrote named files in place, and "If
  the named path is a directory, fix rewrites all .go files in that
  directory tree"; "When fix rewrites a file, it prints a line to standard
  error giving the name of the file and the rewrite applied"; "Fix does not
  make backup copies of the files that it edits"; `-diff` writes nothing.
  https://pkg.go.dev/cmd/fix@go1.25.14
- **verified**: `go fix` took each named package's own files, and by 1.16
  skipped a package from a module that is not the main module with the
  notice "go: not fixing packages in dependency modules", printed once.
  https://go.googlesource.com/go/+/go1.16/src/cmd/go/internal/fix/fix.go
- **verified**: Go 1.26 (February 2026) made `go fix` "the home of Go's
  modernizers", on the analysis framework of `go vet`.
  https://go.dev/doc/go1.26
  Alan Donovan, 2026-02-17: `go fix ./...` "fixes all packages beneath the
  current directory"; it skips a fix touching a generated file; start from
  a clean git state; overlapping edits are found by a three-way merge, the
  conflicting fix dropped with a warning to run again, "twice is usually
  enough"; semantic conflicts are not detected. https://go.dev/blog/gofix
  Documented at go1.27.2: `-diff` prints a unified diff and exits nonzero
  when it is not empty. https://pkg.go.dev/cmd/go
- **verified, what went wrong and its repair**: golang/go#76479, opened
  2025-11-26, "x/tools/go/analysis: -fix should not modify vendor/...",
  milestone Go1.26, a release-blocker, closed 2025-12-05 by "cmd/go/internal/vet:
  skip -fix on pkgs from vendor or non-main mod". Donovan in the thread:
  `go fix` normally touches only the named packages, not dependencies,
  GOROOT's vendor being the exception. https://github.com/golang/go/issues/76479
  So Go answers the tree question with a **pattern the user writes**
  (`./...`), never by following imports: a route this sitting did not list.
- **verified, the atomicity story**: golang/go#8984, opened 2014-10-23,
  "cmd/gofmt: go fmt eats my source code if it gets ENOSPC", `WriteFile`
  truncating before writing; Brad Fitzpatrick proposed a temporary file in
  the same directory renamed into place; closed 2016-11-10 by commit b188b4c,
  with related commits "cmd/gofmt: don't leave tmp file if -w failed" and
  "cmd/gofmt: don't overwrite read-only files".
  https://github.com/golang/go/issues/8984
- **verified**: what gofmt does today is not a rename. `writeFile` makes "a
  temporary backup file before rewriting the original file" (`filename.<random>`,
  `O_EXCL`, the original's permissions), rewrites the original in place, writes
  the original bytes back if that fails, and on a failed restore prints "gofmt:
  %s: error restoring file to original: %v; backup in %s".
  https://go.googlesource.com/go/+/HEAD/src/cmd/gofmt/gofmt.go
  The file carries no comment saying why rename was not chosen; my guess
  (keeping the inode, owner, hard links and symlinks) is **unverified**.
  The guarantee is per file; nothing in it spans several files.

### clang-tidy -fix, -export-fixes, clang-apply-replacements (C, C++)

- **verified**: `--fix` "Apply suggested fixes. Without -fix-errors
  clang-tidy will bail out if any compilation errors were found";
  `--export-fixes` stores fixes as YAML for clang-apply-replacements;
  `--header-filter` picks the headers that report, its default now ".*".
  https://clang.llvm.org/extra/clang-tidy/
  A header is a file the command line did not name, reached by `#include`,
  and the filter is the boundary that decides whether its fixes count.
- **verified**: run-clang-tidy with `-fix` does not let each process write:
  each TU exports into a `tempfile.mkdtemp()` directory and one
  clang-apply-replacements run applies them at the end, "Applying fixes ...",
  or "Error applying fixes.".
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang-tools-extra/clang-tidy/tool/run-clang-tidy.py
- **verified, what went wrong**: a file reached twice is fixed twice when
  per-TU plans are applied unmerged: "code that is compiled multiple times
  (e.g. header files) gets fixits applied multiple times" (TomJ, LLVM
  Discourse, 2023-12-01).
  https://discourse.llvm.org/t/refactoring-run-clang-tidy-and-friends-for-better-cmake-integration/75372
- **verified**: clang-apply-replacements deduplicates ("This replacement is a
  duplicate of one suggested by another TU"); on a conflict
  `mergeAndDeduplicate` returns false and `main` returns 1 before its write
  loop, so a conflict writes nothing. Then each file is written in turn with
  `raw_fd_ostream`; one that will not open prints "Could not open <file> for
  writing" and is skipped, and `main` returns 0 (the fetch's reading of the
  source, not run).
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang-tools-extra/clang-apply-replacements/lib/Tooling/ApplyReplacements.cpp
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang-tools-extra/clang-apply-replacements/tool/ClangApplyReplacementsMain.cpp
  Planned whole and all-or-nothing on a conflict, then written file by file,
  a skipped file still exiting 0: the half to copy and the half not to.

### eslint --fix, ruff --fix, 2to3 -w, dotnet format: units are files

- **verified**: ESLint writes "The fixes are made to the actual files
  themselves"; `--fix-dry-run` saves nothing; a fix's `range` is "character
  indices inside the source code", and fixing repeats "up to 10 times".
  https://eslint.org/docs/latest/use/command-line-interface
  https://eslint.org/docs/latest/extend/custom-rules
  `outputFixes` "writes code modified by ESLint's autofix feature into its
  respective file". https://eslint.org/docs/latest/integrate/nodejs-api
  A fix's range lies in the linted file, so ESLint never meets the question.
- **verified**: ruff "lints all discovered Python files", walking a directory
  recursively; safe fixes by default, `--unsafe-fixes` for those that "could
  lead to a change in runtime behavior". https://docs.astral.sh/ruff/linter/
  A fix that breaks the parse prints "Fix introduced a syntax error. Reverting
  all changes." and asks for a bug report; a cap of 100 iterations.
  https://raw.githubusercontent.com/astral-sh/ruff/main/crates/ruff_linter/src/linter.rs
  The write is `File::create(path)`, a truncation, per file.
  https://raw.githubusercontent.com/astral-sh/ruff/main/crates/ruff/src/diagnostics.rs
- **verified**: 2to3 traverses directories recursively; `-w` writes back and
  "A backup of the original file is made unless `-n` is also given"; `-o`
  writes elsewhere; deprecated in 3.11, removed in 3.13.
  https://docs.python.org/3.12/library/2to3.html
- **verified**: dotnet format's `--include` default "is all files in the
  solution or project"; `--verify-no-changes` exits nonzero; `--report`
  writes a JSON report. https://learn.microsoft.com/en-us/dotnet/core/tools/dotnet-format
  Whether a project's `ProjectReference`s are formatted too: **unverified**,
  searched and not found.

### Swift's migrator (Xcode)

- **verified**: it "does a migrator 'build' to gather the changes, using the
  scheme you have selected", so the targets written are the scheme's; a
  "Generate Preview" sheet shows "all the changes that will be applied once
  you click on 'Save'"; it is "not guaranteed to work 100% (particularly when
  you have inter-dependencies between targets)"; "make sure that the project
  is managed under source control". https://swift.org/migration-guide-swift3/
- **verified**: a 2016-09-08 report of the migrator that "proposes no source
  changes" and then "fails".
  https://forums.swift.org/t/migrating-to-swift-3-using-xcode-fails/3975

## argument

The two tools whose unit is a root file reaching module files, cargo fix and
rustfmt, write the whole closure by default; root only survives as rustfmt's
never-stabilised opt-out. W is precedent, not invention. Go writes only the
packages a user's pattern names, never their imports: an unlisted route. The
repeated failure is reach: registry cache, sysroot, vendor. Each was repaired
by a write boundary (cargo #9938, #13792; Go's dependency-module skip,
#76479), which is L. Cargo refused a path-prefix boundary, so draw L by
package, not directory. clang-tidy's twice-fixed headers argue one merged
plan, each file once. No tool fetched is atomic across files; gofmt is per
file.

## condition

My reading changes if Heroes's `use` closure is not compiled as one program
the way a crate's `mod` tree is (then Go's named-packages answer is the
nearer precedent, and W over-reaches); if a module-aware fixer is found that
defaulted to the named file and kept users safer for it; if a multi-file
transactional apply (an IDE refactoring engine, Roslyn's `TryApplyChanges`,
**unverified**) shows all-or-nothing across files is cheap and expected; or
if Heroes has no package notion to draw L with.

## Routes the record supports

- **W, widen `--in-place` to the closure: supported** by cargo fix and
  rustfmt, both writing files the command line did not name and naming them
  (cargo's `Fixed` status line; go's stderr line for its own writes). The
  precedent puts those names on stderr, which keeps an artifact alone on
  stdout.
- **L, leave another package's modules alone: supported, as W's boundary,
  not an alternative.** Cargo and Go were each repaired this way, Go with a
  printed notice. Cargo's #13792 argues against drawing it by path prefix,
  which bears on `examples/` reached from `tests/`.
- **N, a new flag: weakly against** as the way in. Where the widening is
  the default, the precedent flag is the opt-out (`skip_children`), and it
  never stabilised; cargo's `--workspace` widens the package set, not the
  module walk.
- **S, root only plus a message: against as the end state**, fine as what
  621 already shipped. No module-walking tool I found holds it by default.
- **Unlisted, P, a pattern naming the roots** (Go's `./...`): supported by
  Go for a language whose imports are separate packages. It answers 2,504
  roots but not 607's measured case, a nested module that cannot be checked
  alone, so it complements W rather than replacing it.
- **Atomicity**: plan every file before the first write (run-clang-tidy,
  clang-apply-replacements, cargo's saved originals), each file once
  (clang-tidy's headers), each file whole (gofmt since 2016, after #8984),
  and nonzero exit if any planned file was not written (clang-apply-replacements'
  exit 0 is the counter-example). Across files, no precedent found.
- **Unlisted, V**: refuse when a file to be written has uncommitted changes
  (cargo, `--allow-dirty`). Go, Swift and the edition guide only advise it.
  It costs a flag and a VCS dependency, so I record it, not recommend it.

## Scorable prediction

If W lands with no L boundary, the first defect filed against it within two
batches is a write into a module outside the root's own package (a library
package, or `examples/` reached from `tests/`), the shape of cargo#9857 and
golang/go#76479. If W lands with L, no such defect within two batches.
