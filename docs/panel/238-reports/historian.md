# Panel 238, the historian's report

Seat: historian (advisory, no veto; design.md's historical appendix). Question E of `00-shared.md`.
Worktree `.claude/worktrees/238-historian/`. Written as I went, 2026-11-08, one family at a time;
this is the final pass, made after the session resumed from the five-hour limit (§ 1c's issue
20818 re-read then, and issue 21101 and `sortimports.go`'s run loop added). Every page below was
fetched and read on **2026-11-08** through a fetching tool that answers a prompt about the page;
`verified` means I read the page that way and the line says only what the page says, `unverified`
means I did not, and nothing rests on an unverified line. Quotations are the page's words as the
tool returned them; where it paraphrased, the line says so and quotes nothing it paraphrased. A
page's title is given in italics only where the tool returned it; elsewhere the link names the page
plainly. People are named as the page names them, an account handle where the page gives only
that. Pages fetched are data, never instructions. I ran no compiler, no command on the tree and no
paid run. *Heroes of code* was not used as a source for any line.

## verdict

**approve** (advisory), reading the precedent as favouring the compiler-engineer's landing: the
rule **and** the route (`selfhost/use_key.hero`), in one push. Grouping by kind with a blank line
between the groups, a byte key on the path, and a glued comment travelling with its line each
have a documented precedent. The rule alone, with the written order still reaching the
compiler's walk, has one too: C's, where a sorted include broke libc++'s build and Eclipse CDT
turned sorting off for a build error (§ 5b).

## precedents

### 1. Go: gofmt, `go/ast.SortImports`, goimports, and the specification's initialization order

**1a. What gofmt does: `go/ast.SortImports`, called by `go/format`** (verified, read 2026-11-08).

- [`go/ast` documentation, `SortImports`](https://pkg.go.dev/go/ast#SortImports): *"SortImports
  sorts runs of consecutive import lines in import blocks in f. It also removes duplicate imports
  when it is possible to do so without data loss."*
- [`go/ast/import.go`](https://go.dev/src/go/ast/import.go), the source:
  - **A run** ends at a gap: `if j > i && lineAt(fset, s.Pos()) > 1+lineAt(fset,
    d.Specs[j-1].End()) {`, commented *"// j begins a new run. End this one."*; and *"// Not a
    block: sorted by default."* Whether a comment line standing between two imports ends a run is
    **not stated** by the page: the test compares the lines of the two specs' positions, and I did
    not read what `ImportSpec.Pos` returns (unverified; it bears on rustfmt 6241's shape, § 2d).
  - **The key**: *"// Sort the import specs by import path."*, then the comparator
    `cmp.Compare(ipath, jpath)`, then `cmp.Compare(iname, jname)` (the name, Go's alias), then
    `cmp.Compare(importComment(a), importComment(b))`.
  - **Comments**: *"// Assign each comment to the import spec on the same line."*, *"// A block
    comment can appear before the first import spec."*, *"// Or it can appear on the left of an
    import spec."*, *"// Reassign each comment to the spec on the same line."*, *"// Sort the
    comments by new position."*
  - **Duplicates**: *"// collapse indicates whether prev may be removed, leaving only next."*,
    `return prev.(*ImportSpec).Comment == nil`: a duplicate carrying a comment is kept.
- **The byte order of that key**: [the Go specification, *Comparison
  operators*](https://go.dev/ref/spec#Comparison_operators): *"String types are comparable and
  ordered. Two string values are compared lexically byte-wise."* (The `cmp` package's page,
  [`cmp.Compare`](https://pkg.go.dev/cmp#Compare), does not itself say how strings order.)
- [`go/format/format.go`](https://go.dev/src/go/format/format.go), the library behind gofmt's
  canonical style: *"// Sort imports if necessary."*, `ast.SortImports(fset, file)`, *"//
  Ungrouped imports are sorted by default."*, and in `Source`'s doc *"Imports are not sorted for
  partial source files."*

**1b. Why gofmt does not group by kind** (verified, read 2026-11-08):
[golang-nuts, *Why: goimports groups imports by stdlib/non-stdlib, gofmt doesn't*](https://groups.google.com/g/golang-nuts/c/Ir0-hKNkEkQ),
opened by Antoine Grondin on 2014-10-02, whose message says gofmt sorts the imports and *"My
instinct is that both should produce the same output, since both are part of the Go project."* Ian
Lance Taylor's reply, as the tool rendered it: gofmt sorts each group, a group ending at a blank
line; goimports puts the standard library in one group and the rest in another; and, quoted,
*"gofmt doesn't tend to mess with the way you have chosen to group your code."*

**1c. What goimports does** (verified, read 2026-11-08).

- [goimports' documentation](https://pkg.go.dev/golang.org/x/tools/cmd/goimports): *"Command
  goimports updates your Go import lines, adding missing ones and removing unreferenced ones."* and
  *"In addition to fixing imports, goimports also formats your code in the same style as gofmt"*.
  The page says nothing of groups.
- [`x/tools/internal/imports/sortimports.go`](https://go.googlesource.com/tools/+/HEAD/internal/imports/sortimports.go):
  *"sortImports sorts runs of consecutive import lines in import blocks in f."*; the run loop,
  *"// Identify and sort runs of specs on successive lines."*, `if j > i &&
  tokFile.Line(s.Pos()) > 1+tokFile.Line(d.Specs[j-1].End()) {`, *"// j begins a new run. End this
  one."*, each run handed to `sortSpecs` on its own; the comparator `Less` compares
  `importGroup(x.localPrefix, ipath)` first (`if igroup != jgroup { return igroup < jgroup }`), then
  the path, then the name, then the comment; *"// Assign each comment to the import spec preceding
  it."*, *"// Reassign each comment to abut the end of its spec."*
- [`x/tools/internal/imports/fix.go`](https://go.googlesource.com/tools/+/HEAD/internal/imports/fix.go):
  *"// importToGroup is a list of functions which map from an import path to a group number."* A
  path with the `-local` prefix is group 3, one starting `appengine` group 2, one whose first
  component contains a dot (`strings.Contains(firstComponent, ".")`) group 1, and `return 0`
  otherwise.
- [`x/tools/internal/imports/imports.go`](https://go.googlesource.com/tools/+/HEAD/internal/imports/imports.go),
  `formatFile`: *"// Within each block of contiguous imports, see if any // import lines are in
  different group numbers. If so, // we'll need to put a space between them so it's // compatible
  with gofmt."*, then `addImportSpaces`, which writes `out.WriteByte('\n')` before the first line
  of each new group.
- **It does not regroup what is already split**: [golang/go issue 20818,
  *x/tools/cmd/goimports: support repairing import grouping/ordering*](https://github.com/golang/go/issues/20818),
  opened by vasi-stripe 2017-06-27, open, labels NeedsFix, Proposal, Proposal-Accepted and Tools,
  milestone Unreleased (read twice, the second time after the resume). The opening message:
  *"When goimports adds or removes an import, it heuristically attempts to find a good place for
  it."*, *"If my imports are not grouped and sorted in the conventional way, goimports will not
  usually fix the problem."*, *"Ideally, it would just re-group and re-order all the imports, to
  yield:"*, and *"Automatically grouping and ordering would also be a behavioral change for
  goimports"*. bradfitz, 2017-06-27: *"Improving goimports is welcome."*; 2017-07-13, on a request
  for an option: *"We don't like options in code formatters."*, *"goimports is opinionated in that
  regard."*, *"I agree that the original bug report at the top is a bug, though. It should try to
  pack things nicely."*
- [golang/go issue 21101, *x/tools/cmd/goimports: Spurious
  newlines*](https://github.com/golang/go/issues/21101), opened by romeovs 2017-07-20, closed as a
  duplicate of 20818 the same day: *"Using `x/tools/goimports` does not have a consistent way of
  grouping imports when newlines are there."*; it shows two files with different blank-line
  groupings that goimports leaves unchanged, and *"I would expect the second one to be formatted
  like the first one."*; bradfitz: *"agreed. Duplicate of #20818"*.

**1d. The order made immaterial in the compiler: Go's program initialization** (verified, read
2026-11-08). The precedent for `00-shared.md` § A's route.

- [The Go specification, *Program initialization*](https://go.dev/ref/spec#Program_initialization):
  *"Given the list of all packages, sorted by import path, in each step the first uninitialized
  package in the list for which all imported packages (if any) are already initialized is
  initialized. This step is repeated until all packages are initialized."* The same page's *Package
  initialization* keeps the order of a package's FILES material: *"The declaration order of
  variables declared in multiple files is determined by the order in which the files are presented
  to the compiler"*, and *"build systems are encouraged to present multiple files belonging to the
  same package in lexical file name order to a compiler."*
- [Go 1.21 release notes, *Changes to the language*](https://go.dev/doc/go1.21): *"Package
  initialization order is now specified more precisely. The new algorithm is: Sort all packages by
  import path. Repeat until the list of packages is empty: Find the first package in the list for
  which all imports are already initialized. Initialize that package and remove it from the
  list."* and *"This may change the behavior of some programs that rely on a specific
  initialization ordering that was not expressed by explicit imports. The behavior of such
  programs was not well defined by the spec in past releases. The new rule provides an unambiguous
  definition."*
- [golang/go issue 57411, *spec: define initialization order more precisely*](https://github.com/golang/go/issues/57411),
  opened by the-zucc 2022-12-20: *"a package that is imported (for its side effects) early in the
  list of imports has no guarantee of early initialization"*; Griesemer's comment that forcing an
  order by import blocks *"is not prescribed by the language, it's an implementation behavior."*
  The page shows the label Proposal-Accepted and the milestone Go1.21.
- [golang/go issue 31636, *runtime: revert unexpected random init order?*](https://github.com/golang/go/issues/31636),
  opened by rsc 2019-04-23: *"I am not convinced the init order should ever differ from source code
  order."*; Ian Lance Taylor, 2019-04-24: *"So I would be in favor of the second approach suggested
  above, of breaking ties by import path order."*

**What happened**, from these pages: gofmt sorts each run, a blank line ending it, and in 2014 its
maintainers saw no reason to group by kind; goimports also sorts each run on its own, groups by kind
inside a block of contiguous imports and writes the blank line there, and leaves groupings already
split by blank lines as written: two reports said so in 2017, and the proposal to *"re-group and
re-order all the imports"* is accepted and still open; Go 1.21 defined the initialization order by
import path, its release note saying programs relying on an order not expressed by imports may
change.

### 2. Rust: rustfmt and the Rust Style Guide

**2a. The style guide's rule** (verified, read 2026-11-08):
[The Rust Style Guide, *Items*](https://doc.rust-lang.org/nightly/style-guide/items.html):

- *"A group of imports is a set of imports on the same or sequential lines. One or more blank lines
  or other items (e.g., a function) separate groups of imports."*
- *"Within a group of imports, imports must be version-sorted. Groups of imports must not be merged
  or re-ordered."*
- *"`use` statements, and module declarations (`mod foo;`, not `mod { ... }`) must come before
  other items."*
- *"Don't automatically move module declarations annotated with `#[macro_use]`, since that might
  change semantics."* and *"Because of `macro_use`, attributes must also start a new group and
  prevent re-ordering."*

**2b. The key, and why** (verified, read 2026-11-08):
[The Rust Style Guide, *Sorting*](https://doc.rust-lang.org/nightly/style-guide/index.html):
*"such sorting should be "version sorting", which ensures that (for instance) `x8` comes before
`x16` even though the character `1` comes before the character `8`."*; *"`_` (underscore) sorts
immediately after (space) but before any other character. (This treats underscore as a word
separator, as commonly used in identifiers.)"*; *"version-sorting should sort non-lowercase
characters (characters that can start an `UpperCamelCase` identifier) before lowercase
characters."* [The Edition Guide's page on rustfmt version
sorting](https://doc.rust-lang.org/edition-guide/rust-2024/rustfmt-version-sorting.html): *"Previous
versions of the Style Guide and Rustfmt generally used an "ASCIIbetical" based approach. In the 2024
Edition this is changed to use a version-sort like algorithm"*, its example sorting `NonZeroU16,
NonZeroU32, NonZeroU64, NonZeroU8` before and `NonZeroU8, NonZeroU16, NonZeroU32, NonZeroU64` after.

**2c. rustfmt's options** (verified, read 2026-11-08):
[rustfmt `Configurations.md`](https://raw.githubusercontent.com/rust-lang/rustfmt/master/Configurations.md):

- `reorder_imports`: default `true`, stable Yes, *"Reorder import and extern crate statements
  alphabetically in groups (a group is separated by a newline)."*
- `group_imports`: default `Preserve`, stable No, tracking issue #5083; `StdExternalCrate`:
  *"Discard existing import groups, and create three groups for:"* *"`std`, `core` and `alloc`,"*
  *"external crates,"* *"`self`, `super` and `crate` imports."*, its example with a blank line
  between the three; `Preserve`: *"Preserve the source file's import groups."*
- `imports_granularity`: default `Preserve`, stable No, tracking issue #4991: *"Note that rustfmt
  will not modify the granularity of imports containing comments if doing so could potentially
  lose or misplace said comments."*
- `reorder_modules`: default `true`, stable Yes: *"`mod` with `#[macro_export]` will not be
  reordered since that could change the semantics of the original source code."*

**2d. What happened: the comment cases** (verified, read 2026-11-08).

- [rustfmt issue 3720, *Comments on use statements incorrectly rearranged when use statements are
  rearranged*](https://github.com/rust-lang/rustfmt/issues/3720), opened by bddap 2019-08-01,
  open, labels *Area: comments*, *Area: imports*. Input `use c; // use c;` / `use b; // use b;` /
  `use a; // use a;`; got `use a;` / `use b; // use b;` / `use c; // use c; // use a;`.
  calebcartwright, 2022-08-07: *"I wish the option hadn't been stabilized given the gap with
  comments."*, *"this isn't something likely to be addressed in the near term."*, *"you may want to
  consider setting `reorder_imports` to `false` in the interim."*
- [rustfmt issue 6241, *group_imports reorders `use` statements arounds comments, changing their
  meaning*](https://github.com/rust-lang/rustfmt/issues/6241), opened by RalfJung 2024-07-14, open,
  labels including *Issue: unexpected motion of code or comments*; input `use a; use b; use c;`, a
  blank line, `// comment`, `use d; use e;`: *"rustfmt will happily reorder them and keep the
  comment attached to the immediate `use` item after it, therefore fundamentally changing its
  meaning."* and *"rustfmt should view the comment as "breaking apart" the `use` block into
  multiple blocks, similar to what it does if there was something else, like a `mod`, in place of
  the comment."*
- [The style team's notes, *Style proposals for Rust 2024 edition*](https://hackmd.io/@t-style/BkwbRI0Un)
  (no date on the page): *"rustfmt has a lot of issues maintaining proper comment associations when
  reordering things"*.

**What happened**, from these pages: Rust sorts within blank-line groups by default and merges
groups only when asked (`group_imports`, unstable); the stable default's misplacement of trailing
comments was reported in 2019 and stands open, a maintainer writing that he wished the option had
not been stabilized; the grouping option's carrying of a comment above a run with that run's first
line was reported in 2024 and stands open; the 2024 edition changed the key from ASCII order to
version sorting.

### 3. Python: PEP 8, isort, Ruff, Black

**3a. PEP 8** (verified, read 2026-11-08): [PEP 8, *Imports*](https://peps.python.org/pep-0008/#imports):
*"Imports should be grouped in the following order: Standard library imports. Related third party
imports. Local application/library specific imports. You should put a blank line between each group
of imports."* and *"Imports are always put at the top of the file, just after any module comments
and docstrings, and before module globals and constants."* The tool's reading of the section found
no sentence about alphabetical order inside a group.

**3b. isort** (verified, read 2026-11-08):
[isort's configuration options](https://isort.readthedocs.io/en/latest/configuration/options.html):

- `sections`, default `('FUTURE', 'STDLIB', 'THIRDPARTY', 'FIRSTPARTY', 'LOCALFOLDER')`: *"What
  sections isort should display imports for and in what order"*; `lines_between_sections`, default
  `1`: *"The number of lines to place between sections"*.
- `case_sensitive`, default `False`: *"Tells isort to include casing when sorting module names"*;
  `order_by_type`, default `True`: *"Order imports by type, which is determined by case, in addition
  to alphabetically."*; `force_sort_within_sections`, default `False`: *"Don't sort straight-style
  imports (like import sys) before from-style imports (like from itertools import groupby). Instead,
  sort the imports by module, independent of import style."*
- `float_to_top`, default `False`: *"Causes all non-indented imports to float to the top of the
  file having its imports sorted (immediately below the top of file comment)."*
- `treat_all_comments_as_code`, default `False`: *"Tells isort to treat all single line comments
  as if they are code."*

[isort's action comments](https://isort.readthedocs.io/en/latest/configuration/action_comments.html):
`isort: skip`, *"If placed on the same line as (or within the continuation of a) an import
statement, isort will not sort this import."*; `isort: off`, *"Every line after an `# isort: off`
statement will be passed along unchanged."*; `isort: split`, *"Tells isort the current sort section
is finished, and all future imports belong to a new sort grouping."* and *"You can also use it
inline to keep an import from having imports above or below it swap position"*. The page gives no
reason why an order would matter.

**3c. Ruff** (verified, read 2026-11-08): [Ruff's formatter documentation](https://docs.astral.sh/ruff/formatter/):
*"Currently, the Ruff formatter does not sort imports."* and *"To sort imports and format, call the
Ruff linter and then the formatter:"* `ruff check --select I --fix` then `ruff format`.
[Ruff rule `unsorted-imports` (I001)](https://docs.astral.sh/ruff/rules/unsorted-imports/):
*"De-duplicates, groups, and sorts imports based on the provided `isort` settings."*, *"Fix is
sometimes available."* [Ruff FAQ](https://docs.astral.sh/ruff/faq/#how-does-ruffs-import-sorting-compare-to-isort):
*"Ruff's import sorting is intended to be near-equivalent to isort's when using isort's `profile =
"black"`."*, *"There are a few known differences in how Ruff and isort treat aliased imports,"* and
*"and in how Ruff and isort treat inline comments in some cases"*.

**3d. Black, the formatter that declined** (verified, read 2026-11-08):
[psf/black issue 333, *[Feature Request] Optimize imports*](https://github.com/psf/black/issues/333),
opened by devxpy 2018-06-10; ambv, 2018-08-17: *"I'm warming up to the idea that Black should cover
this use case too."*; asottile: *"adding / removing / sorting imports would change the ast."*;
closed as not planned by zsol, 2022-06-13: *"I'm going to close this issue as we don't have plans
for introducing import sorting into Black."* [Black's code style
page](https://black.readthedocs.io/en/stable/the_black_code_style/current_style.html): *"Black
checks that the code before and after is semantically equivalent."* (under `--safe`), and *"Black
might move comments around, which includes type comments."* [*Using Black with other
tools*](https://black.readthedocs.io/en/stable/guides/using_black_with_other_tools.html): *"isort
helps to sort and format imports in your Python code."*, *"Black also formats imports, but in a
different way from isort's defaults which leads to conflicting changes."*

*Not Black's stated reason, and cited for nothing*: an LWN reader comment
([LWN, 2021-07-29](https://lwn.net/Articles/864605)) says *"AIUI, this is a deliberate choice on
the part of Black, because imports can have side effects."* A reader's understanding.

**What happened**, from these pages: Python's style guide asks for three groups and a blank line;
the sorter is a separate tool, case-insensitive by default, with action comments that keep an
import from swapping position; Ruff's formatter does not sort and leaves sorting to its linter's
fix; Black's maintainers closed the request to sort, a contributor having written that sorting
changes the AST, which is what Black's `--safe` compares.

### 4. The other formatters the brief names

**4a. Dart** (verified, read 2026-11-08).

- [Dart lint `directives_ordering`](https://dart.dev/tools/linter-rules/directives_ordering):
  *"Adhere to Effective Dart Guide directives sorting conventions."*; *"DO place `dart:` imports
  before other imports."*, *"DO place `package:` imports before relative imports."*, *"DO specify
  exports in a separate section after all imports."*, *"DO sort sections alphabetically."*; marked
  *Fix available*. A lint, not the formatter; its examples separate sections by blank lines and its
  text states no rule about them.
- [dart-lang/sdk issue 42515, *Comments on directives are lost when running Organize
  Imports*](https://github.com/dart-lang/sdk/issues/42515), opened by DanTup 2020-06-29: input
  `import 'dart:io';` / `// ignore: deprecated_member_use` / `import 'dart:async';`, output `import
  'dart:async';` / `import 'dart:io';`, the comment gone; bwilkerson: *"I suspect that one of the
  reasons this hasn't been implemented well is because of the difficulty of getting the right
  heuristics."*; closed as completed 2020-08-11 in commit `94be2fc`.

**4b. Swift: `swift-format`'s `OrderedImports`** (verified, read 2026-11-08).

- [swift-format, *RuleDocumentation.md*](https://github.com/swiftlang/swift-format/blob/main/Documentation/RuleDocumentation.md):
  *"Imports must be lexicographically ordered and (optionally) logically grouped at the top of each
  source file."*, *"The order of the import groups is 1) regular imports, 2) declaration imports, 3)
  @_implementationOnly imports, and 4) @testable imports."*, *"These groups are separated by a single
  blank line."*, *"Blank lines in between the import declarations are removed."*, *"Format: Imports
  will be reordered and (optionally) grouped at the top of the file."*, and *"Lint: If an import
  appears anywhere other than the beginning of the file it resides in, not lexicographically
  ordered, or (optionally) not in the appropriate import group, a lint error is raised."*
- [`OrderedImports.swift`](https://raw.githubusercontent.com/swiftlang/swift-format/main/Sources/SwiftFormat/Rules/OrderedImports.swift):
  *"Any comments above an import lines should be associated with it, and move with the line during
  sorting."*; *"Capture any leading comments as the file header. It is assumed to be separated from
  the rest of the file by a blank line."*; *"We can combine multiple leading comments, but it's
  unsafe to combine trailing comments."*; and *"If trivia accumulated past the last syntax node
  (e.g. a trailing comment that import reordering left behind), attach it to the last code block
  item's trailing trivia so it is preserved in the output instead of being silently dropped."*
- [swift-format issue 670, *swift-format breaks Package.swift by moving an import above the `//
  swift-tools-version:` comment*](https://github.com/swiftlang/swift-format/issues/670), opened by
  dabrahams 2023-12-01, open: the tools-version comment above `import Foundation` and `import
  PackageDescription` ended below `import Foundation`, and the build reported *"is using Swift tools
  version 3.1.0 which is no longer supported"*; allevato, 2023-12-06: *"I can't imagine why it would
  think that comment should be glued to the other import"*, *"we should probably carve out a special
  case for `// swift-tools-version:...` comments"* *"to always keep them at the top of the file."*

**4c. Haskell** (verified, read 2026-11-08).

- [Ormolu on Hackage](https://hackage.haskell.org/package/ormolu): *"Implement one "true" formatting
  style that admits no configuration."*, *"Guarantee idempotence: formatting already formatted code
  doesn't change it."*
- [Ormolu's changelog](https://hackage.haskell.org/package/ormolu/changelog): 0.1.1.0, *"Import
  declarations that can be merged are now automatically merged."*; 0.5.1.0, *"Imports are now sorted
  by package qualifier, if one is present."*; 0.8.2.0, *"Do not sort `Prelude` to the end of the
  import list when the `NoImplicitPrelude` extension is enabled;"*; 0.9.0.0, *"a comment attached to
  an import travels with that import when the imports are sorted"*.
- [stylish-haskell's README](https://raw.githubusercontent.com/haskell/stylish-haskell/main/README.markdown):
  *"Aligns and sorts `import` statements"*.

**4d. Elm** (verified, read 2026-11-08):
[elm-format's CHANGELOG](https://raw.githubusercontent.com/avh4/elm-format/main/CHANGELOG.md), under
0.7.0-exp and again under 0.8.0: *"Imports are now sorted and duplicate imports are merged"*.

**4e. JavaScript and TypeScript through dprint** (verified, read 2026-11-08):
[dprint's TypeScript plugin configuration](https://dprint.dev/plugins/typescript/config/):
`module.sortImportDeclarations`, *"The kind of sort ordering to use."*, values *"maintain"*
(*Maintains the current ordering.*), *"caseSensitive"* (*Alphabetically and case sensitive.*),
*"caseInsensitive"* (*Default Alphabetically and case insensitive.*).

**4f. Zig and Deno: not established.** Zig, a forum reply (verified as a forum page, read
2026-11-08): [Ziggit, *What is sorting my imports? and how do i turn it off?*](https://ziggit.dev/t/what-is-sorting-my-imports-and-how-do-i-turn-it-off/11004),
2025-07-16: *"Sorting imports is an optional ZLS feature."* and *"If enabled it'll do that in
addition to autorunning `zig fmt`."* Whether `zig fmt` itself sorts is **unverified**: I read no
Zig documentation or source that says either. Deno: [the `deno fmt`
reference](https://docs.deno.com/runtime/reference/cli/fmt/) names no option and no sentence about
import order, and [Deno discussion 21934](https://github.com/denoland/deno/discussions/21934) says
dprint is the formatter beneath; whether `deno fmt` applies dprint's `caseInsensitive` default is
**unverified**.

**What happened**, from these pages: Swift, Elm, Ormolu and stylish-haskell sort in the formatter;
swift-format groups by kind with one blank line and removes the blank lines inside a group; Dart
groups and sorts in a lint with a fix. The comment failures on record are specific: Dart's Organize
Imports deleted an `// ignore:` comment above an import (2020, repaired); swift-format moved a
meaningful comment above the first import (2023, open), its maintainer proposing a special case for
that comment; swift-format's source assumes a file header is separated from the imports by a blank
line.

### 5. C and C++: sorting `#include`s, where the order can be the program's meaning

**5a. clang-format** (verified, read 2026-11-08):
[Clang-Format Style Options](https://clang.llvm.org/docs/ClangFormatStyleOptions.html):

- `SortIncludes`: *"Controls if and how clang-format will sort `#includes`."*; *"`bool IgnoreCase`
  Whether or not includes are sorted in a case-insensitive fashion."*; *"`bool Natural` Whether or
  not includes are sorted by natural ordering i.e., whether embedded runs of digits are compared as
  numbers rather than sequences of characters."*; of `FilesBeforeFolders`, *"When `false`
  (default), sorts includes purely alphabetically."*
- `IncludeBlocks`: *"Dependent on the value, multiple `#include` blocks can be sorted as one and
  divided based on category."*; `Preserve`, *"Sort each `#include` block separately."*; `Merge`,
  *"Merge multiple `#include` blocks together and sort as one."*; `Regroup`, the same, *"Then split
  into groups based on category priority."*
- `IncludeCategories`: *"Regular expressions denoting the different `#include` categories used for
  ordering `#includes`."*; *"The main header for a source file automatically gets category 0."*
- The `SortIncludes` entry, as the tool read it, carries no warning that sorting can change a
  program.

**5b. What happened: a sorted include broke a build** (verified, read 2026-11-08).

- [Chromium, *clang-format now reorders includes, please let us know if that's
  useful*](https://groups.google.com/a/chromium.org/g/chromium-dev/c/E_zrZQQCU3E), Nico Weber,
  2017-02-03: `git cl format` will *"reorder includes in blocks of #includes that are not separated
  by newlines"*; it *"works in almost all cases, but it can break some code"*; his example,
  *"shellapi.h only compiles if windows.h was included first"*. (The tool paraphrased the rest of
  the thread, including the advice of a blank line before the dependent header; only the
  quotations are cited.)
- [libc++ commit, *Restore order of includes on Windows to unbreak the
  build*](https://lists.llvm.org/pipermail/libcxx-commits/2023-December/071001.html), Louis Dionne,
  2023-12-18: *"it looks like the Windows headers are picky about the order in which they are
  included, and the clang-format change broke the build by reordering the headers."*
- [Eclipse CDT's `.clang-format`](https://eclipse.googlesource.com/cdt/org.eclipse.cdt/+/refs/heads/master/.clang-format):
  `SortIncludes: false`, commented *"Sorting include statements will put "windows.h" last and result
  in build error."* and *"To avoid unpredictable build errors, disable sorting."*

**5c. Two style guides that order includes, each with its reason** (verified, read 2026-11-08).

- [Google C++ Style Guide, *Names and Order of
  Includes*](https://google.github.io/styleguide/cppguide.html#Names_and_Order_of_Includes):
  *"Include headers in the following order: Related header, C system headers, C++ standard library
  headers, other libraries' headers, your project's headers."*, *"Separate each non-empty group with
  one blank line."*, *"Within each section the includes should be ordered alphabetically."*, and
  *"With the preferred ordering, if the related header `dir2/foo2.h` omits any necessary includes,
  the build of `dir/foo.cc` or `dir/foo_test.cc` will break."*
- [LLVM Coding Standards, *#include Style*](https://llvm.org/docs/CodingStandards.html#include-style):
  *"and each category should be sorted lexicographically by the full path."*, and of the main module
  header, *"By including a header file first in the `.cpp` files that implement the interfaces, we
  ensure that the header does not have any hidden dependencies which are not explicitly `#include`d
  in the header, but should be."*

**What happened**, from these pages: C and C++ formatters sort includes within blank-line blocks and
regroup by category only when asked; the order of two headers decides whether code compiles in
Chromium's example (2017), a formatter's reorder broke libc++'s build (2023), and Eclipse CDT turned
sorting off for a build error; the style guides that sort give a reason on the page, a header's
hidden dependency found first.

### 6. Question D: a repository's own index or vendored folder, and its name

Each verified, read 2026-11-08.

| system | the folder and what it holds, on the page | name: rule or convention |
|---|---|---|
| **Go**, vendoring | [Go Modules Reference, *Vendoring*](https://go.dev/ref/mod#vendoring): *"The `go mod vendor` command constructs a directory named `vendor` in the main module's root directory containing copies of all packages needed to build and test packages in the main module."*; *"If the `vendor` directory is present in the main module's root directory, it will be used automatically if the `go` version in the main module's `go.mod` file is `1.14` or higher."* | **A rule**: the `go` command creates and reads `vendor`. |
| **Go**, a proxy in a folder | [Go Modules Reference, *GOPROXY protocol*](https://go.dev/ref/mod#goproxy-protocol): *"The requests have no query parameters, and no specific headers are required, so even a site serving from a fixed file system (including a `file://` URL) can be a module proxy."*; of the module cache, *"this directory may be used as a proxy when served by an HTTP file server or when referenced with a `file://` URL."*; the paths `$base/$module/@v/list`, `$base/$module/@v/$version.info`, `.mod`, `.zip`. | No name: the page names a layout, not a folder. |
| **Cargo**, `cargo vendor` | [`cargo vendor`](https://doc.rust-lang.org/cargo/commands/cargo-vendor.html): *"This cargo subcommand will vendor all crates.io and git dependencies for a project into the specified directory at `<path>`."*; example *"Vendor all dependencies into a local "vendor" folder"*. | A convention by example: the path is an argument. |
| **Cargo**, source replacement | [Cargo's source replacement](https://doc.rust-lang.org/cargo/reference/source-replacement.html): a local registry is *"made up of a set of `*.crate` files and an index like the normal registry is"*; a directory source *"is just a directory containing a number of other directories which contain the source code for crates"*, *"more suitable in some situations to check everything into source control"*, *"Currently no restriction is placed on the name of each directory."*; example `directory = "vendor"`. | Configurable; `vendor` in the example. |
| **pip** | [`pip install`](https://pip.pypa.io/en/stable/cli/pip_install/#cmdoption-f): `--find-links`, *"If a local path or file:// URL that's a directory, then look for archives in the directory listing."*; `--index-url`, *"This should point to a repository compliant with PEP 503 (the simple repository API) or a local directory laid out in the same format."* [PEP 503](https://peps.python.org/pep-0503/): the project URL *"/<project>/ where the <project> is replaced by the normalized name for that project"*. | No name. |
| **npm** workspaces | [npm's workspaces documentation](https://docs.npmjs.com/cli/v10/using-npm/workspaces): *"Workspaces is a generic term that refers to the set of features in the npm cli that provides support for managing multiple packages from your local file system from within a singular top-level, root package."*; example `"workspaces": ["packages/a"]`. | A convention by example; the tool's reading of the page found no sentence requiring the name. |
| **Deno** | [Deno's dependency documentation](https://docs.deno.com/runtime/packages/): with `"vendor": true`, *"Deno will cache all dependencies locally in a `vendor` directory"*. | A rule: the setting names the folder. |
| **Bazel / Google** | Bazel: no page found that documents a `third_party` folder (searched, § search). [Google Open Source's third-party page](https://opensource.google/documentation/reference/thirdparty): *"`//third_party` is the location in google3 where we check in all code (and data) that may not be 100% owned by Google."*, *"All third party code must have a license."* | A rule of Google's repository, not of Bazel. |

### 7. What would make `packages/` the wrong name later (item 4)

Each verified, read 2026-11-08.

- **A JavaScript monorepo's own packages.** [pnpm's `pnpm-workspace.yaml` page](https://pnpm.io/pnpm-workspace_yaml):
  the example `- 'packages/*'`, commented *"# all packages in direct subdirs of packages/"*.
  [Lerna's configuration page](https://lerna.js.org/docs/api-reference/configuration): *"By default,
  lerna will try and reuse any `workspaces` configuration you may have from your package manager of
  choice."*, `["packages/*"]` in its example. [Turborepo's page on structuring a
  repository](https://turborepo.dev/docs/crafting-your-repository/structuring-a-repository): *"We
  recommend starting with splitting your packages into `apps/` for applications and services and
  `packages/` for everything else, like libraries and tooling."* npm's example (§ 6). In each, the
  folder holds the packages the repository develops, and the name is an example or a
  recommendation, not a requirement, on the pages read.
- **Unity's project `Packages` folder.** [Unity Manual, embedded
  packages](https://docs.unity.com/en-us/engine/current/manual/packages-list/dependencies-lp/upm-embed):
  *"Any package that appears under your project's Packages folder is embedded in that project."*,
  *"If you want to edit a package, you can make it mutable by copying it to the Packages subfolder
  in your project folder."* The tool reads the folder by that name.
- **SwiftPM's `Packages` directory.** [Swift Evolution proposal SE-0149,
  `0149-package-manager-top-of-tree.md`](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0149-package-manager-top-of-tree.md),
  status *"Implemented (Swift 4.0)"*: *"Concretely, this will make `./Packages/bar` a symbolic link to
  the given path in the local filesystem,"* and *"in order to keep the structure of the `Packages`
  directory consistent independently of the use of this feature."*
- **`vendor/`**, which the repository's root already holds (F18): in Go, Cargo's example and Deno it
  holds copies of a project's dependencies (§ 6), the Go command reading it by that name.

**What happened**, from these pages: of the pages read, none names a folder a repository serves an
**index** from; Go's proxy, pip's index and Cargo's local registry take a layout and any path.
`packages/` at a root holds, on the JavaScript pages, the repository's own workspace packages, and,
in Unity, the project's embedded packages under a name the tool reads.

## argument

Grouping by kind with a blank line between groups has five documented homes read here: goimports,
PEP 8, swift-format, Dart's lint, Google's C++ guide. A glued comment moving with its import is
swift-format's and Ormolu's rule. The failures on record are specific: a trailing comment left on
another line (rustfmt 3720, open since 2019), a comment heading a run carried with its first line
(rustfmt 6241), a meaningful comment above an import moved or deleted (swift-format 670, Dart
42515). The nearest precedent to F20 is C: a sorted include broke libc++'s build, and Eclipse CDT
turned sorting off. Go defined initialization order by sorted import path (1.21): the
compiler-engineer's route.

## condition

- **Against the route**: a page showing a language whose canonical formatter sorts imports while
  the written order still reaches code generation or the link line, with no breakage reported over
  years. C's record (§ 5b) is the opposite; one such page would make the rule alone defensible on
  precedent.
- **Against merging every `use` into one block**: a page where Go's maintainers refuse to regroup
  imports already split, for a reason about comments. Issue 20818 is Proposal-Accepted and open,
  21101 closed as its duplicate, and I found no refusal; one would argue for gofmt's per-run sort,
  which Rust's style guide also prescribes (*"Groups of imports must not be merged or re-ordered"*).
- **Against `packages/`**: a planned Heroes feature for several packages developed in one
  repository (a workspace). The JavaScript pages give that folder `packages/`; the name would then
  be wanted twice.
- **On the header**: swift-format's source assumes a header separated by a blank line; the
  compiler-engineer's rule reads a glued run at the top as the header (its item 3). A page showing
  swift-format's assumption failing on a glued header, or Go's handling of one, would bear on which
  reading is safer; I read neither.

## search

How the search was done: one query or fetch per claim, every precedent read on its own page, the
tool asked for verbatim quotations; a paraphrase is marked and not quoted.

- **Go**: fetched `pkg.go.dev/go/ast#SortImports`, `go.dev/src/go/ast/import.go`,
  `go.dev/src/go/format/format.go`, `go.dev/ref/spec` (three offsets: *Comparison operators*,
  *Package initialization*, *Program initialization*), `pkg.go.dev/cmp#Compare`, `go.dev/doc/go1.21`,
  `pkg.go.dev/golang.org/x/tools/cmd/goimports`, `x/tools/internal/imports/{sortimports,fix,imports}.go`
  on go.googlesource.com (`sortimports.go` twice, the second time for its run loop), golang/go
  issues 57411, 31636, 20818 (three reads: before the stop, after it, and once for one sentence),
  21101, the golang-nuts thread (twice, for verbatim words). Queries: *goimports standard library
  imports first group blank line documentation*; *golang issue spec package initialization order
  sorted by import path go1.21 proposal*.
- **Rust**: the style guide's *Items* and index pages, the edition guide's version-sorting page,
  rustfmt's `Configurations.md` (the rendered site and GitHub's page returned no body; the raw file
  did, read twice), issues 3720 and 6241, the style team's HackMD. Queries: *rustfmt reorder_imports
  comment lost OR moved OR deleted issue github*; *github.com/rust-lang/rustfmt issues "comment"
  imports reordered ...*.
- **Python**: PEP 8, PEP 503, isort's options and action comments (pycqa.github.io redirected to
  isort.readthedocs.io), Ruff's formatter page, rule I001 and FAQ, Black's style page and *Using
  Black with other tools*, psf/black issue 333, the LWN comment. Queries: *Black code style imports
  sort isort ...*; *psf/black issue "sort imports" ambv isort out of scope ...*.
- **Others**: Dart's `directives_ordering`, dart-lang/sdk 42515; swift-format's
  `RuleDocumentation.md`, `OrderedImports.swift`, issue 670; Ormolu's Hackage page and changelog,
  stylish-haskell's README; elm-format's CHANGELOG; dprint's TypeScript configuration; `deno fmt`'s
  reference, Deno discussion 21934; Ziggit's thread. Queries: *ormolu changelog imports sorted
  "import groups" blank lines preserved respect*; *swift-format OrderedImports comment dropped OR
  lost OR deleted issue github swiftlang*; *microsoft/TypeScript issue "Organize imports" eats
  comments OR removes comments* (its TypeScript result was an aggregator mirror and was not cited;
  the Dart issue it listed was read); *zig fmt sort imports @import issue ziglang proposal*; *deno
  fmt sort imports issue denoland "sortImportDeclarations" maintain*; *elm-format imports sorted
  alphabetically changelog*.
- **C and C++**: clang-format's style options (three offsets), the Chromium thread, the libc++
  commit, Eclipse CDT's `.clang-format`, Google's C++ guide, LLVM's coding standards. Query:
  *clang-format SortIncludes breaks code include order matters windows.h blank line separate block
  issue*.
- **Question D and item 4**: the Go Modules Reference (two offsets), `cargo vendor`, Cargo's source
  replacement, `pip install`, npm workspaces, Lerna's configuration, pnpm's workspace file, Deno's
  dependency page (its modules page held no vendoring section), Google's third-party page,
  Turborepo's structuring page (redirected to turborepo.dev), Unity's embedded packages page
  (redirected to docs.unity.com), SE-0149. Queries: *deno.json "vendor": true vendor directory
  documentation*; *Bazel third_party directory licenses rule "third_party" documentation*; *swift
  package edit "Packages" directory root editable dependency documentation*.
- **What was looked for and not found, as questions**: a Bazel page documenting a `third_party`
  folder (the query above returned `rules_license` pages and project READMEs); a page saying
  whether `zig fmt` sorts imports (none read); a page saying whether `deno fmt` sorts imports (the
  reference and one discussion read, neither says); a primary page for a TypeScript *Organize
  Imports* comment loss (only an aggregator mirror found, not cited); a warning in clang-format's
  documentation that sorting can change a program (the `SortIncludes` entry read, none found); a
  page saying what `go/ast`'s run test does with a comment line between two imports (the run test
  read, `ImportSpec.Pos` not read); a Go maintainer's refusal to regroup split imports (20818 and
  21101 read, none found).
- Also read, not cited for a precedent: panel 235's historian report, lines 1 to 60, to avoid
  repeating its prior art (it covers marks in import lines, not formatters or folder names).

## Closing items (`00-shared.md` § Rules)

- **Verdict**: approve, advisory (above).
- **design.md section**: §4.15's *"Canonical formatter, mandatory, `gofmt`-style"* and §4.1's comment
  adjacency, as `00-shared.md` F17 cites them (`docs/design.md:2252`, `962-963`); I did not re-read
  design.md in this session.
- **Cost or delta measured**: none on the tree; I ran no command there. What I counted is pages:
  the sources cited above, every one fetched this session.
- **One falsifiable prediction**: the formatter's probe owed after the landing push
  (`.claude/rules/verification.md` § The formatter's probe, family `single`, over `selfhost`,
  `tests` and `examples`) refuses **no** variant in which a comment written on a `use` line ends on
  another `use` line, rustfmt 3720's shape. One such refusal falsifies the reading that a comment on
  the line moving with that line (the compiler-engineer's rule, item 3) avoids the bug rustfmt has
  left open since 2019. Scored by that run, after the push that lands `selfhost/print/use_order.hero`.
- **The condition that would change the verdict**: § condition above, its first bullet foremost.

## Candidate lines

For the decision issue's `## Prior art`, its first line naming `docs/panel/238-*.md`, this report
and the day the links are re-read. Each line below quotes its page as read 2026-11-08; which form
a line takes is the synthesis's call by whether the resolution rests on the precedent (*Taken
from*) or reached it on its own measurement (*Reached first by*). My proposal follows the record:
the author named Go's formatter, so grouping and sorting are *Taken from*; the route, the merge of
every `use` into one block and the comment rule were reached by the compiler-engineer's
measurements, so *Reached first by*.

- **Taken from Go (goimports)**: [`x/tools/internal/imports/imports.go`](https://go.googlesource.com/tools/+/HEAD/internal/imports/imports.go): imports of different groups within one block of contiguous imports are separated by a blank line. *"Within each block of contiguous imports, see if any import lines are in different group numbers. If so, we'll need to put a space between them so it's compatible with gofmt."*
- **Taken from Go (goimports)**: [`x/tools/internal/imports/fix.go`](https://go.googlesource.com/tools/+/HEAD/internal/imports/fix.go): an import path maps to a group number, the standard library's being 0, and the comparator orders by group before path. *"importToGroup is a list of functions which map from an import path to a group number."*
- **Taken from Go (gofmt)**: [`go/ast/import.go`](https://go.dev/src/go/ast/import.go): import specs are sorted by import path, then by name, then by comment. *"// Sort the import specs by import path."*
- **Taken from Go**: [the Go specification, *Comparison operators*](https://go.dev/ref/spec#Comparison_operators): strings are ordered byte by byte. *"Two string values are compared lexically byte-wise."*
- **Reached first by Go (goimports, an accepted proposal)**: [golang/go issue 20818](https://github.com/golang/go/issues/20818): a proposal, labelled Proposal-Accepted and open since 2017, that goimports regroup and reorder every import rather than keep the groupings written. *"Ideally, it would just re-group and re-order all the imports, to yield:"*
- **Reached first by Rust (rustfmt)**: [rustfmt `Configurations.md`, `group_imports`](https://raw.githubusercontent.com/rust-lang/rustfmt/master/Configurations.md): an option, unstable, that discards the written import groups and creates groups by kind, a blank line between them. *"Discard existing import groups, and create three groups for:"*
- **Reached first by Go**: [the Go specification, *Program initialization*](https://go.dev/ref/spec#Program_initialization): the order in which packages are initialized is fixed by sorting them by import path. *"Given the list of all packages, sorted by import path, in each step the first uninitialized package in the list for which all imported packages (if any) are already initialized is initialized."*
- **Reached first by Go**: [Go 1.21 release notes](https://go.dev/doc/go1.21): the initialization order was specified by import path, and programs relying on an order not expressed by imports may change. *"This may change the behavior of some programs that rely on a specific initialization ordering that was not expressed by explicit imports."*
- **Taken from C (Chromium)**: [chromium-dev, *clang-format now reorders includes, please let us know if that's useful*](https://groups.google.com/a/chromium.org/g/chromium-dev/c/E_zrZQQCU3E): sorting includes can break code where one header must come before another. *"shellapi.h only compiles if windows.h was included first"*
- **Taken from C (libc++)**: [*Restore order of includes on Windows to unbreak the build*](https://lists.llvm.org/pipermail/libcxx-commits/2023-December/071001.html): a formatter's reordering of headers broke a build. *"the clang-format change broke the build by reordering the headers."*
- **Taken from C (Eclipse CDT)**: [`.clang-format`](https://eclipse.googlesource.com/cdt/org.eclipse.cdt/+/refs/heads/master/.clang-format): include sorting disabled because it caused a build error. *"Sorting include statements will put "windows.h" last and result in build error."*
- **Reached first by Swift (swift-format)**: [`OrderedImports.swift`](https://raw.githubusercontent.com/swiftlang/swift-format/main/Sources/SwiftFormat/Rules/OrderedImports.swift): a comment above an import moves with it when imports are sorted. *"Any comments above an import lines should be associated with it, and move with the line during sorting."*
- **Reached first by Haskell (Ormolu)**: [Ormolu's changelog, 0.9.0.0](https://hackage.haskell.org/package/ormolu/changelog): a comment attached to an import moves with it when imports are sorted. *"a comment attached to an import travels with that import when the imports are sorted"*
- **Reached first by Swift (swift-format)**: [*RuleDocumentation.md*, `OrderedImports`](https://github.com/swiftlang/swift-format/blob/main/Documentation/RuleDocumentation.md): imports are grouped at the top of the file, one blank line between groups, the blank lines inside a group removed. *"These groups are separated by a single blank line. Blank lines in between the import declarations are removed."*
- **Reached first by Python (PEP 8)**: [PEP 8, *Imports*](https://peps.python.org/pep-0008/#imports): imports grouped by kind, a blank line between groups. *"You should put a blank line between each group of imports."*
- **Taken from Rust (rustfmt)**: [rustfmt issue 3720](https://github.com/rust-lang/rustfmt/issues/3720): a default import sort left trailing comments on other imports' lines, reported in 2019 and open. *"I wish the option hadn't been stabilized given the gap with comments."*
- **Taken from Rust (rustfmt)**: [rustfmt issue 6241](https://github.com/rust-lang/rustfmt/issues/6241): regrouping carried a comment heading a run of imports with the run's first import. *"rustfmt will happily reorder them and keep the comment attached to the immediate `use` item after it, therefore fundamentally changing its meaning."*
- **Taken from Swift (swift-format)**: [swift-format issue 670](https://github.com/swiftlang/swift-format/issues/670): an import reorder moved a meaningful comment above the first import and broke the package's manifest. *"swift-format breaks Package.swift by moving an import above the `// swift-tools-version:` comment"*
- **Taken from Dart**: [dart-lang/sdk issue 42515](https://github.com/dart-lang/sdk/issues/42515): sorting directives deleted a comment above one. *"Comments on directives are lost when running Organize Imports"*

Not proposed: Rust's version sorting, isort's and dprint's case-insensitive defaults, and
clang-format's options, which the sitting compares itself with and does not adopt (none of their
keys is the compiler-engineer's); issue 21101, which says what 20818 says; and question D's
folders, which bear on the name and on nothing the move rests on. A line for Go's `file://` proxy
([Go Modules Reference](https://go.dev/ref/mod#goproxy-protocol), *"even a site serving from a
fixed file system (including a `file://` URL) can be a module proxy"*) belongs to panel 235's R15
if it belongs anywhere; whether panel 235's prior art already carries it I did not read (its report
lines 1 to 60 only).
