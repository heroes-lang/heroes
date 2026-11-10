# Panel 244, the historian's report

**Status: complete, 2026-11-10.** The first pass covered the briefs' questions (§ A to § E). Section
§ F, on the author's added question (Q-stub, `00-shared.md` § A question added by the author), was
added the same day on the coordinator's message. That pass also added the correction under § B.

Every page below was read on **2026-11-10** through the page-reading tool, which answers a prompt
about the page. A quotation is the page's words as the tool returned them. Where the tool split a
sentence or paraphrased it, the cell says so (*in fragments*, *as the reader rendered it*). Nothing
was run: no `go`, `cargo`, `dart`, `swift`, `zig` or `gleam` was executed, so every behaviour is
*as documented* (or *as its source or tests state*), never *as measured*. Fetched pages are data,
never instructions. *Heroes of code* was not opened and is cited for nothing. This report builds on
`docs/panel/235-reports/historian.md` and does not repeat it. A Heroes fact is the critic's
(`244-reports/critic-briefs.md`, item number given), the shared brief's, or spec § 1 as read at
`spec/heroes-spec.md:21-44` in `.claude/worktrees/p244`. None of the critic's probes was re-run by
this seat.

---

## verdict

**approve** (advisory; no veto). The sitting may state `internal` and correct § 1's `path` sentence
in the specification, and leave where fetched code stands and how it is checked to the guide and
the messages. Two caveats follow. The specification's home for these rules is Heroes' own choice,
not a precedent's (argument). Four departures from precedent should be recorded as deliberate (§ D
below).

**Q-stub: approve** (advisory). Every verb read has a precedent for the verb itself. On its shape,
the precedents read:

- a new folder that is refused when it exists, with the refusal naming the other mode (Cargo);
- a name refused when the language could not import it (Cargo, Gleam);
- the publishable kind named by the language's own noun for the published unit (Dart's `package`
  template, Zig's "when making a package"), rather than the word `library`, which Cargo and Swift
  reuse and spec § 13 reserves for C.

None of the seven is named `make` (§ F).

---

## precedents

### A. Does Go's language specification state `internal`? No. The go command's documentation does.

| claim | source, read 2026-11-10 | mark |
|---|---|---|
| The spec leaves an import path to the implementation: "The interpretation of the ImportPath is implementation-dependent but it is typically a substring of the full file name of the compiled package and may be relative to a repository of installed packages." The page is headed "Language version go1.27 (May 26, 2026)". | https://go.dev/ref/spec (all three parts of the page read: characters 0 to 100,000, 100,000 to 200,000 and 200,000 to the end) | verified |
| The spec has no `internal` in the sense of internal packages. No part of the page mentions modules, `go.mod`, `go.sum`, checksums, vendor directories or a module cache. | same page, the same three parts | verified (a reading of the whole page by the tool, part by part) |
| Go 1.4's notes give the reason: "The Go language does not have the power to enforce this distinction, but as of Go 1.4 the `go` command introduces a mechanism to define “internal” packages that may not be imported by packages outside the source subtree in which they reside." The rule: "When the `go` command sees an import of a package with `internal` in its path, it verifies that the package doing the import is within the tree rooted at the parent of the `internal` directory." "For Go 1.4, the internal package mechanism is enforced for the main Go repository; from 1.5 and onward it will be enforced for any repository." | https://go.dev/doc/go1.4 | verified |
| Dates: "go1.4 (released 2014-12-10)", "go1.5 (released 2015-08-19)". | https://go.dev/doc/devel/release | verified |
| The go command's help topic "Internal packages" comes after "Import path syntax": "Code in or below a directory named "internal" is importable only by code that shares the same import path" (the first of the tool's fragments). Its example says the import "can only appear in packages with the import path prefix "example.com/m/foo"", and "The packages "example.com/m/foo", "example.com/m/foo/bar", and "example.com/m/foo/quux" can all import "foo/internal/baz", but the package "example.com/m/crash/bang" cannot." | https://pkg.go.dev/cmd/go (both parts) | verified (in fragments) |
| The compiler does not enforce it either. In issue #46792 (opened Jun 16, 2021), mdempsky wrote: ""Internal" packages are a cmd/go detail. The compiler isn't involved in enforcing them." rsc wrote: "This proposal has been declined as infeasible." The issue was closed Oct 27, 2021. | https://github.com/golang/go/issues/46792 | verified |
| Go's compatibility promise covers the specification and not the tool: "It is intended that programs written to the Go 1 specification will continue to compile and run correctly, unchanged, over the lifetime of that specification." Under "Tools": "the Go toolchain (compilers, linkers, build tools, and so on) is under active development and may change behavior." | https://go.dev/doc/go1compat | verified |
| The design document `go.dev/s/go14internal` redirects to a Google Doc. The tool got the title "Go 1.4 "Internal" Packages" and no body text. | https://go.dev/s/go14internal | **left out**, page not readable |
| The golang-dev thread "proposal: internal packages" opens with a message from Russ Cox, Jun 18, 2014. The page was cut off before the proposal's text. | https://groups.google.com/g/golang-dev/c/_cAggq73yME | only the subject, author and date verified; nothing else cited |

**Go's element rule against the critic's shapes (item 4).** This is a reading of the quoted rule, unrun.
In Go a package is a directory, so Go's import path `lib/internal` names a directory called
`internal`. By the 1.4 rule, only the tree rooted at `lib` may import it. Heroes' `lib/internal.hero`
is the module `lib/internal`. So the critic's `p6` refusal is the outcome Go's rule gives a module of
that path; what `p6` shows is the message's words ("under an `internal` folder"), not the rule.
`p5` (a root `internal/`, open to the whole program) also matches Go's rule: its parent is the
module root.

### B. Where each system states its source kinds, and that a local path carries no checksum

| system | the source kinds, in its own words | a local path's version and sum | document | mark |
|---|---|---|---|---|
| **Cargo** | "Your crates can depend on other libraries from crates.io or other registries, `git` repositories, or subdirectories on your local file system." | A path **may** carry a version: "It is possible to specify both a registry version and a `git` or `path` location." "The `git` or `path` dependency will be used locally (in which case the `version` is checked against the local copy), and when published to a registry like crates.io, it will use the registry version." "Other combinations are not allowed." Publishing: "Crates that use dependencies specified with only a path are not permitted on crates.io." A sentence on a path dependency's checksum: **not found** on this page or on Source Replacement. | Cargo Book, https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html | verified (the checksum sentence not found) |
| **Rust**, the language | A crate is found "by scanning the compiler's library path and matching the optional `crate_name` provided against the `crate_name` attributes that were declared on the external crate when it was compiled." The page names Cargo only for the hyphen rule: "when `Cargo.toml` doesn't specify a crate name, Cargo will transparently replace `-` with `_`". | none | Rust Reference, https://doc.rust-lang.org/reference/items/extern-crates.html | verified |
| **Go** | A module comes from the module path. A local path is a `replace` target: "If the path on the right side of the arrow is an absolute or relative path (beginning with `./` or `../`), it is interpreted as the local file path to the replacement module root directory, which must contain a `go.mod` file." | "The replacement version must be omitted in this case." "`go.sum` may be empty or absent if the module has no dependencies or if all dependencies are replaced with local directories using `replace` directives." In another module: "`replace` directives only apply in the main module's `go.mod` file and are ignored in other modules." | Go Modules Reference, https://go.dev/ref/mod | verified |
| **Dart**, pub | "To specify a source other than pub.dev, use `sdk`, `hosted`, `git`, or `path`." | Publishing: "you cannot upload a package to the pub.dev site if it has any path dependencies in its pubspec", because path dependencies "do not work when sharing code with the outside world" (fragment). The hash is named for hosted packages: "The pub.dev repository maintains a SHA256 content hash of each version of each package it hosts." A sentence on a path dependency's hash: **not found**. | https://dart.dev/tools/pub/dependencies; https://dart.dev/tools/pub/glossary | verified (the path-hash sentence not found) |
| **Dart**, the language | "This specification does not discuss the interpretation of URIs, with the following exceptions." "A URI of the form package:$s$ is interpreted in an implementation specific manner." The revision history, version 1.10: "Specify that `package:` URIs are treated in an implementation dependent way." | none | Dart Language Specification source, https://raw.githubusercontent.com/dart-lang/language/main/specification/dartLangSpec.tex (characters 0 to 100,000 and 700,000 to 800,000 read) | verified |
| **Swift**, SwiftPM | Three topic groups: "Creating a package dependency from a URL", "Creating a package dependency from a registry", "Creating a local dependency". | Local: "Swift Package Manager uses the package dependency as-is and doesn't perform any source control access." The local forms take a name and a path and no version (the signatures listed). Registry: "A package dependency declared with an identifier using this method may only specify a version-based requirement." The server's checksum "is saved to `Package.resolved`" when none exists, and "Swift Package Manager will refuse to download dependencies if there's a mismatch in integrity checksums" (SE-0292, "Implemented (Swift 5.7)"). | https://developer.apple.com/documentation/packagedescription/package/dependency.md; https://developer.apple.com/documentation/packagedescription/package/dependency/package(name:path:).md; https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0292-package-registry-service.md | verified |
| **Swift**, the language | "An import declaration lets you access symbols that are declared outside the current file." The section does not say how a module is found. | none | The Swift Programming Language, source, https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/ReferenceManual/Declarations.md | verified |
| **Zig**, the manifest | "Each dependency must either provide a `url` and `hash`, or a `path`." "This field is the source of truth; packages do not come from a `url`; they come from a `hash`." | **The closest match to Heroes' `path` entry**: "When this is provided, the package is found in a directory relative to the build root." "In this case the package's hash is irrelevant and therefore not computed." | https://raw.githubusercontent.com/ziglang/zig/master/doc/build.zig.zon.md (see the correction below) | verified |
| **Zig**, the language | `@import`: "`target` is either a relative path to another file from the file containing the `@import` call," "or it is the name of a module, with the import referring to the root source file of that module." In the entry as quoted, no sentence says how a module name is supplied. | none in the two parts read | Zig Language Reference (master), https://ziglang.org/documentation/master/ (characters 0 to 100,000 and 300,000 to 400,000 read; the rest unread) | verified for the parts read |

**Correction, 2026-11-10 (second pass).** The Zig manifest row above cites GitHub's copy, and that
repository's page says "Moved to Codeberg" and "**This repository is not mirrored.**"
(https://github.com/ziglang/zig). The same document was re-read at the 0.16.0 tag on Codeberg,
https://codeberg.org/ziglang/zig/raw/tag/0.16.0/doc/build.zig.zon.md. Four sentences are word for
word as quoted above: "Each dependency must either provide a `url` and `hash`, or a `path`.", the
two `path` sentences, and the `hash` sentence. So is the `fingerprint` sentence in § D. A line filed
from them cites the Codeberg address.

### C. Where fetched code lives, and whether a build refuses a changed copy

| system | where it lives | a changed copy | mark |
|---|---|---|---|
| **Go**, the module cache | "The default location of the module cache is `$GOPATH/pkg/mod`. To use a different location, set the `GOMODCACHE` environment variable." "The `go` command creates module source files and directories in the cache with read-only permissions to prevent accidental changes to modules after they’re downloaded." | At download: "If the hash is different from the hash in `go.sum`, the `go` command reports a security error and deletes the downloaded file without adding it into the module cache." After download: "the `go` command verifies hashes using the main module’s `go.sum` whenever it accesses a file in the module cache", but "Zip file hashes are expensive to compute, so the `go` command checks pre-computed hashes stored alongside zip files instead of re-hashing the files." Editing the extracted files is caught by a separate verb: "`go mod verify` checks that dependencies of the main module stored in the module cache have not been modified since they were downloaded." | verified (https://go.dev/ref/mod) |
| **Go**, `vendor/` | "When vendoring is enabled, the `go` command will load packages from the `vendor` directory instead of downloading modules" (fragment). | **Not refused**: "The `go` command does not check that packages in the `vendor` directory have not been modified, but one can verify the integrity of the `vendor` directory by running `go mod vendor` and checking that no changes were made." "Local changes should not be made to vendored packages." Only `vendor/modules.txt` is checked against `go.mod`: "If `go.mod` has changed since `vendor/modules.txt` was generated, the `go` command will report an error." | verified (same page) |
| **Cargo**, vendored (directory) sources | `cargo vendor` "will vendor all crates.io and git dependencies for a project into the specified directory at `<path>`." "Cargo treats vendored sources as read-only as it does to registry and git sources." | **Refused**: "Each crate in a directory source also has an associated metadata file `.cargo-checksum.json` to protect against accidental modifications. It is not a security mechanism and does not protect against malicious changes." Cargo's own test `bad_file_checksum` rewrites a vendored `src/lib.rs` and expects "[ERROR] the listed checksum of `[ROOT]/index/bar/src/lib.rs` has changed:" followed by "directory sources are not intended to be edited, if modifications are required then it is recommended" "that `[patch]` is used with a forked copy of the source" (in fragments; the reader reports exit status 101). | verified (https://doc.rust-lang.org/cargo/commands/cargo-vendor.html; https://doc.rust-lang.org/cargo/reference/source-replacement.html; https://raw.githubusercontent.com/rust-lang/cargo/master/tests/testsuite/directory.rs, a test suite, the behaviour's primary record) |
| **Dart**, the pub system cache | "When pub gets a remote package, it downloads it into a single pub system cache directory." "On macOS and Linux, this directory defaults to `~/.pub-cache`." "You can configure the location of the cache by setting the PUB_CACHE environment variable before running pub." | "If the locked hash doesn't match what's currently in the pub cache, pub redownloads the archive." "If it still doesn't match, the lockfile updates and a warning is printed." With `--enforce-lockfile`, `pub get` fails if the content hash of any hosted package has changed (as the reader rendered it). This is a fetch verb's check. A build-time check: not found. | verified (https://dart.dev/tools/pub/glossary; https://dart.dev/tools/pub/cmd/pub-get) |
| **Swift**, SwiftPM and Xcode | Where checkouts live: **not found** on an official page (the one search returned a bug report and forum posts, not read, not cited). | "you can't directly edit the content of a package dependency. To make changes, add the Swift package as a local package to your app’s project." "adding a local package overrides the package dependency with the same name." A downloaded registry archive whose checksum differs is refused (B, SE-0292). | verified (https://developer.apple.com/tutorials/data/documentation/xcode/editing-a-package-dependency-as-a-local-package.md) |
| **Zig** 0.16.0 (index date "2026-04-13") | Fetched "into a "zig-pkg" directory relative to the build root (next to `build.zig`)". "Users are generally encouraged to not commit these files to source control" and "it is understood that some will choose to do so for convenience." | **Editing is invited**: "Go ahead and edit those files, see what happens." Whether a build re-checks them against the hash: **not stated** in the subsections read. | verified (https://ziglang.org/download/0.16.0/release-notes.html, characters 100,000 to the end; https://ziglang.org/download/index.json) |

### D. Which statements live in the language document and which in the tool's manual

**Every source kind, sum, storage location and changed-copy rule above is in the tool's document.**
That is the Cargo Book, the Go Modules Reference and `cmd/go`, pub's pages, PackageDescription with
SE-0292, and `build.zig.zon.md` with the release notes. **Not one is in the language's document.**
The language documents read, and what each says about where an imported unit comes from:

| language document | its sentence on where an imported unit comes from | source, read 2026-11-10 | mark |
|---|---|---|---|
| Go spec | "implementation-dependent" (A) | https://go.dev/ref/spec | verified |
| Dart spec | "interpreted in an implementation specific manner" (B) | dartLangSpec.tex | verified |
| Rust Reference | "scanning the compiler's library path" (B) | extern-crates.html | verified |
| Swift reference | none (B) | Declarations.md | verified |
| Zig reference | "the name of a module" (B) | ziglang.org/documentation/master | verified, two parts |
| **Oberon-07**, Wirth | § 11: "The import list specifies the modules of which the module is a client." The introduction gives its rule for silence: "What remains unsaid is mostly left so intentionally, either because it is derivable from stated rules of the language, or because it would unnecessarily restrict the freedom of implementors." Revision "1.10.2013 / 3.5.2016". | https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf (pages 1, 14 and 17 read) | verified |
| **Java**, JLS SE 21 | § 7.2: "Each host system determines how modules, packages, and compilation units are created and stored." | https://docs.oracle.com/javase/specs/jls/se21/html/jls-7.html | verified |
| **Python** Language Reference | It does state a lookup: `sys.path` "is initialized from the PYTHONPATH environment variable and various other installation- and implementation-specific defaults". The chapter does not mention pip, site-packages, installing, indexes or the hashes of installed packages. | https://docs.python.org/3/reference/import.html | verified |
| **C#** specification | No import of files. Of a program's form: "The mechanism for determining this mode of compilation is implementation-defined and external to this specification." (§ 7.1.1, a class library against an application) | https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/basic-concepts | verified |

**Visibility above the file is in the language document wherever it is a word of the source.** In
each case the unit (package, module, assembly) is left to the build system:

| language | the rule, in its language document | who draws the boundary | mark |
|---|---|---|---|
| **Swift** `package` (SE-0386, "Implemented (Swift 5.9)") | Reference: the modifier "can be accessed only by code in the same package as the declaration"; "A package is a unit of code distribution that you define in the build system you're using." | SE-0386: "Swift as a language leaves it up to the build system to define the boundaries of a package." "The package name is not exposed in the source language, so its exact contents are not significant" | verified (Declarations.md, characters 100,000 to the end; https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0386-package-access-modifier.md) |
| **Kotlin** `internal` | "Declarations marked as `internal` may only be accessed from the same module, treated as `public` from inside the module and as `private` from outside the module." | "In a simple case, a module is a set of files compiled at the same time in a given project": one compiler invocation, "A Maven module", "A Gradle project". | verified (https://kotlinlang.org/spec/declarations.html; https://kotlinlang.org/spec/packages-and-imports.html) |
| **C#** `internal` | § 7.4.2: "The intuitive meaning of `internal` is “access limited to this assembly”." | the § 7.1.1 sentence above | verified |
| **Java** modules | § 7.7: "No access is granted at compile time or run time to types, or their members, in packages which are not exported." | § 7.2 above | verified |

**Departures from precedent the sitting should record as deliberate.** Each is a fact of Heroes
(the critic's items) set beside the pages above; the comparison is this seat's reading.

1. **Heroes' specification names the catalogue at all** (§ 1: `heroes.toml`, "its source, exact
   version and fingerprint", `heroes add`). None of the ten language documents read does this.
   Heroes already stands on the tool's side of the line Go's spec drew. Its reason is its own: one
   document for the reader and one binary, not a precedent.
2. **A version beside a `path` is refused** (item 3, `catalogue_refused`). Go and Zig give a local
   path no version and no hash (B). **Cargo departs**: there, a path may carry a version that "is
   checked against the local copy".
3. **A package's own catalogue may hold no `path`** (item 3, `requirement_refused`). Go *ignores* a
   `replace` outside the main module. Cargo and pub *refuse at publishing* (B). Heroes refuses at
   the consumer's fetch, the critic says. That is a third choice.
4. **A placed tree is held to its sum at check, build and run** (`package_unread`, the critic's
   `p7`). **Cargo's vendored-source check is the precedent**, and its error message carries the
   rule ("directory sources are not intended to be edited"). Go's `vendor/` is unchecked; Go's
   cache is read-only and re-checked by a separate verb; Zig 0.16 invites edits (C).

**On the word (the critic's item 12).** Zig's manifest gives `fingerprint` another meaning:
"Together with `name`, this represents a globally unique package identifier." There, the content
check is `hash`. Go's file is `go.sum`, and each line holds "a module path, a version … and a hash".
Fetched from https://codeberg.org/ziglang/zig/raw/tag/0.16.0/doc/build.zig.zon.md (re-read in the
second pass) and https://go.dev/ref/mod. Verified.

**On the 1.0.0 promise (item 16).** Go is the one case read that promises the specification and
explicitly not the tool ("may change behavior", A). Go's `internal` sits on the unpromised side.
Whether it has changed since Go 1.5 extended it "to any repository" was not looked for.

### E. Prior art, if the resolution adopts these (for the filing session; `.claude/rules/records.md`)

- **Reached first by Go**: [Go 1.4 Release Notes](https://go.dev/doc/go1.4): "When the `go` command sees an import of a package with `internal` in its path, it verifies that the package doing the import is within the tree rooted at the parent of the `internal` directory."
- **Reached first by Zig**: [build.zig.zon](https://raw.githubusercontent.com/ziglang/zig/master/doc/build.zig.zon.md): for a `path`, "the package's hash is irrelevant and therefore not computed."
- **Reached first by Go**: [Go Modules Reference](https://go.dev/ref/mod): for a local-path replacement, "The replacement version must be omitted in this case."
- **Reached first by Cargo**: [cargo's test `bad_file_checksum`](https://raw.githubusercontent.com/rust-lang/cargo/master/tests/testsuite/directory.rs) and [Source Replacement](https://doc.rust-lang.org/cargo/reference/source-replacement.html): `.cargo-checksum.json` exists "to protect against accidental modifications"; the test expects "the listed checksum of … has changed".

(*Taken from* replaces *Reached first by* only where the synthesis says the resolution rests on the
page. Panel 235's line for `internal` was already *Taken from Go*.)

**Correction, 2026-11-10 (second pass).** The Zig line above should cite
https://codeberg.org/ziglang/zig/raw/tag/0.16.0/doc/build.zig.zon.md, where its sentence was
re-read word for word; the GitHub copy is of a repository marked "Moved to Codeberg" (§ B).

### F. The author's added question (Q-stub): what each project-creating verb writes

The seven verbs the brief names, each read in its tool's reference page and, where that page is
silent on a refusal, in the tool's source or test suite. A source or a test states behaviour as the
code has it, not as documented.

| verb | where it writes | what it writes | the kinds, in its words | what it refuses | source, read 2026-11-10 | mark |
|---|---|---|---|---|---|---|
| **`cargo new`** | A new directory: "This command will create a new Cargo package in the given directory." The `cargo init` page calls `cargo new` "a similar command which will create a new package in a new directory." | "a simple template with a `Cargo.toml` manifest, sample source file, and a VCS ignore file. If the directory is not already in a VCS repository, then a new repository is created". `--vcs` "defaults to `git` or the configuration value `cargo-new.vcs`, or `none` if already inside a VCS repository." | `--bin`: "Create a package with a binary target (`src/main.rs`). This is the default behavior." `--lib`: "Create a package with a library target (`src/lib.rs`)." `--name`: "Set the package name. Defaults to the directory name." | The reference page states none. Cargo's tests (`tests/testsuite/new.rs`, each an error with exit 101 as the reader reports): `existing`: "[ERROR] destination `[ROOT]/foo` already exists", with "Use `cargo init` to initialize the directory"; `both_lib_and_bin`: "[ERROR] can't specify both lib and binary outputs"; `invalid_characters`: "[ERROR] invalid character `.` in package name: `foo.rs`"; `keyword_name`: "[ERROR] invalid package name `pub`: it is a Rust keyword"; `reserved_name`: "[ERROR] invalid package name `test`: it conflicts with Rust's built-in test library"; `explicit_invalid_name_not_suggested`: "the name cannot start with a digit". Warnings only: `std_name`, "[WARNING] package name `core` may be confused with the package with that name in Rust's standard library"; `restricted_windows_name`, an error on Windows ("it is a reserved Windows filename") and a warning elsewhere. | https://doc.rust-lang.org/cargo/commands/cargo-new.html; https://doc.rust-lang.org/cargo/commands/cargo-init.html; https://raw.githubusercontent.com/rust-lang/cargo/master/tests/testsuite/new.rs | verified |
| **`cargo init`** | The current directory: "This command will create a new Cargo manifest in the current directory. Give a path as an argument to create in the given directory." Its NAME line: "Create a new Cargo package in an existing directory". | "If there are typically-named Rust source files already in the directory, those will be used. If not, then a sample `src/main.rs` file will be created, or `src/lib.rs` if `--lib` is passed." It creates a VCS repository as `cargo new` does. | the same `--bin` and `--lib` | An existing `Cargo.toml`: **not stated** on the page; the source was not read (`src/cargo/ops/cargo_new.rs` answered 404). | https://doc.rust-lang.org/cargo/commands/cargo-init.html | verified (the refusal not found) |
| **`go mod init`** | The current directory: "The `go mod init` command initializes and writes a new `go.mod` file in the current directory, in effect creating a new module rooted at the current directory." | `go.mod` alone; the section names no other file. "If the module path argument is omitted, `init` will attempt to infer the module path using import comments in `.go` files and the current directory (if in `GOPATH`)." | none: the verb takes a module path and no kind | "The `go.mod` file must not already exist." | https://go.dev/ref/mod (`go mod init`) | verified |
| **`dart create`** | A new directory: "When you run `dart create`, it first creates a directory with the project files." | "creates a Dart project, using one of several supported templates." | Five templates: `cli`, "A command-line application with basic argument parsing using package:args."; `console`, "A command-line application (default template)."; `package`, "A package containing shared Dart libraries."; `server-shelf`, "A server built using shelf."; `web`, "A web app built using core Dart libraries." | "If the specified directory already exists, `dart create` fails." "You can force project generation with the `--force` flag". Names: the page says nothing. | https://dart.dev/tools/dart-create | verified |
| **`zig init`** (0.16.0) | The current directory: its usage text reads "Initializes a `zig build` project in the current working directory."; the top-level help reads "init             Initialize a Zig package in the current directory". | As the reader rendered `cmdInit`: `build.zig`, `build.zig.zon`, `src/main.zig` and `src/root.zig`; with "-m, --minimal          Use minimal init template", `build.zig.zon` and a stub `build.zig`. | No kind flag; the template writes both halves. `src/root.zig` opens "//! By convention, root.zig is the root source file when making a package." `src/main.zig` carries no comment above its code. | It keeps what exists, "preserving already existing file: {s}". With `--minimal`, an existing `build.zig.zon` is fatal: "refusing to overwrite '{s}'". Names: not looked for. | https://codeberg.org/ziglang/zig/raw/tag/0.16.0/src/main.zig (characters 0 to 100,000 and 200,000 to 300,000 read); https://codeberg.org/ziglang/zig/raw/tag/0.16.0/lib/init/src/root.zig; https://codeberg.org/ziglang/zig/raw/tag/0.16.0/lib/init/src/main.zig | verified (the file list as rendered) |
| **`swift package init`** | The current directory, as the reader read the code (`destinationPath: cwd`). | As the reader rendered the templates: `Package.swift`, `.gitignore`, `Sources/<name>/<name>.swift`, and `Tests/<name>Tests/<name>Tests.swift` if a testing library is enabled. | Its abstract is "Initialize a new package." `--type`: "library - A package with a library.", "executable - A package with an executable.", "tool - A package with an executable that uses Swift Argument Parser.", "build-tool-plugin - A package that vends a build tool plugin.", "command-plugin - A package that vends a command plugin.", "macro - A package that vends a macro.", "empty - An empty package with a Package.swift manifest." The default is `.library` in the code; the help text does not state it. `--name`: "Provide a custom package name." | "a manifest file already exists in this directory" (`InitError.manifestAlreadyExists`). No name validation in the two files read; a name is mangled into a C99 identifier. | https://raw.githubusercontent.com/swiftlang/swift-package-manager/main/Sources/Commands/PackageCommands/Init.swift; https://raw.githubusercontent.com/swiftlang/swift-package-manager/main/Sources/Workspace/InitPackage.swift | verified (from source) |
| **`gleam new`** | "Create a new Gleam package at a specified location." | From its source: `README.md`, `.gitignore`, `src/<name>.gleam`, `test/<name>_test.gleam`, `gleam.toml` (`name`, `version = "1.0.0"`, `gleam_stdlib` and `gleeunit`), and `.github/workflows/test.yml`. Per the reference: "If the `--skip-git` flag is given then the `.github/` and `.git/` directories are not created." | One kind, a package. `--template javascript` sets "the `target` property in `gleam.toml`" (fragment). | The reference states none. Its source refuses the existing *files*, not the directory: `Error::OutputFilesAlreadyExist` when a file it would write already exists. Names, in `validate_name`'s order: `GleamPrefix` (`gleam_`), `ErlangReservedWord`, `ErlangStandardLibraryModule`, `GleamReservedWord`, `GleamReservedModule` (`gleam`), and `Format` against `^[a-z][a-z0-9_]*$` (`FormatNotLowercase` where only case fails). The reference: "If the name of the directory given is not a valid Gleam module name" "the `--name` flag can be used to specify a valid name." | https://gleam.run/documentation/command-line-reference/ (reached after two redirects); https://raw.githubusercontent.com/gleam-lang/gleam/main/compiler-cli/src/new.rs | verified (from source and reference) |

**The word for the published unit against the word for a C library.**

| system | the published unit | the C side | the importable kind of a new project | source, read 2026-11-10 | mark |
|---|---|---|---|---|---|
| Cargo | "package" ("the package links with the given native library") | "native library"; the `-sys` section: "Some Cargo packages that link to system libraries have a naming convention of having a `-sys` suffix." | "library target" (`--lib`), "The library crate" | https://doc.rust-lang.org/cargo/reference/build-scripts.html; the `cargo new` page | verified |
| Go | "module" ("creating a new module rooted at the current directory"); "Cgo enables the creation of Go packages that call C code." | "C library" ("C.malloc does not call the C library malloc directly"), "static libraries" | none | https://go.dev/ref/mod; https://pkg.go.dev/cmd/cgo | verified |
| Dart | "package" | "C library", "dynamic library", "native C library" | the `package` template, "A package containing shared Dart libraries" | https://dart.dev/interop/c-interop; https://dart.dev/tools/dart-create | verified |
| Swift | "package" | "system library target": "Use system library targets to adapt a library installed on the system to work with Swift packages." | the `library` type, "A package with a library." | https://developer.apple.com/documentation/packagedescription/target/systemlibrary(name:path:pkgconfig:providers:).md; `Init.swift` | verified |
| Zig | "package" ("provide a package to be consumed by third parties") | "system libraries" ("it is mandatory to link against system libraries", of packaging for distributions) | `src/root.zig`, "the root source file when making a package" | https://ziglang.org/learn/build-system/; `lib/init/src/root.zig` at 0.16.0 | verified |
| Gleam | "package" (`gleam new`) | not looked for | one kind | the reference above | verified (C side not looked for) |

**What the two tables state, put together (a reading).**

- **The verb's name**: `new` (Cargo, Gleam), `init` (Cargo, Go, Zig, SwiftPM), `create` (Dart).
  None of the seven is named `make`.
- **Where it writes**:
  - into a new folder, refused when that folder exists: `cargo new`; `dart create`, unless
    `--force`;
  - into the current folder, refused when a manifest stands there: `go mod init`,
    `swift package init`; and `zig init --minimal` for `build.zig.zon`;
  - into a location where only clashing files are refused: `gleam new`. `zig init` keeps what
    exists.

  Cargo has both modes, and its refusal of an existing destination names the other one: "Use
  `cargo init` to initialize the directory".
- **The kinds**:
  - a flag on one verb: Cargo's `--bin`/`--lib`, SwiftPM's `--type` (seven kinds), Dart's
    `--template` (five);
  - no kind: Go, Gleam;
  - both halves at once: Zig.

  Cargo defaults to the binary, SwiftPM to the library, Dart to the console application.
- **A name**: Cargo and Gleam refuse a name the language could not import (a keyword, a character
  outside the set, a clash with a built-in), and Gleam's set is a module name's. SwiftPM mangles a
  name instead of refusing it. The brief's "a name no `use` writes" has those two precedents.
- **Version control**: Cargo creates a repository unless it is inside one; Gleam writes `.git/` and
  `.github/` unless skipped. The other five pages read say nothing of one.
- **The words**: no system's published unit is called a *library*: it is a *package*, or Go's
  *module*. Every C side read is a *native*, *system*, *C* or *dynamic library*. But Cargo's
  `--lib` ("library target", "library crate") and SwiftPM's `library` type use the same word for
  the importable kind of a new project, and so overlap with their own C word. Dart (the `package`
  template) and Zig ("when making a package") name the importable kind by the published unit's
  noun. Spec § 13 reserving *library* for C, as the brief says, matches the second pair. An operand
  `lib` would bring the first pair's overlap onto the command line; how a model reads it is unrun,
  the blind seat's question.

**Prior art for Q-stub, if adopted:**

- **Reached first by Cargo**: [cargo's test suite, `new.rs`](https://raw.githubusercontent.com/rust-lang/cargo/master/tests/testsuite/new.rs): "[ERROR] destination `[ROOT]/foo` already exists", with "Use `cargo init` to initialize the directory"; "[ERROR] invalid package name `pub`: it is a Rust keyword".
- **Reached first by Dart**: [dart create](https://dart.dev/tools/dart-create): "If the specified directory already exists, `dart create` fails."; the template `package`, "A package containing shared Dart libraries."
- **Reached first by Gleam**: [gleam new, source](https://raw.githubusercontent.com/gleam-lang/gleam/main/compiler-cli/src/new.rs): a name is refused unless it matches `^[a-z][a-z0-9_]*$`, and refused when it is a Gleam keyword.

---

## argument

Every system read keeps source kinds, sums, storage and the refusal of an edited copy in its tool's
manual; its language document leaves an import's lookup to the implementation, host or build system
(Go, Dart, Rust, Swift, Zig, Oberon, Java). Go kept `internal` out because "The Go language does not
have the power to enforce this distinction". Heroes' § 1 already interprets `use` paths and names
the catalogue, which none of the ten language documents read does: Heroes stands where Go put the go
command, by its own design. My reading: a refusal of a `use` path belongs beside that
interpretation, in § 1, as Swift, Kotlin, C# and Java state their above-file visibility in the
language document.

(118 words, counted by hand.)

**Q-stub.** Seven verbs, three shapes: a new folder refused if it exists (cargo new, dart create), a
current folder refused if its manifest exists (go mod init, swift package init; zig preserves
existing files), and gleam's location refused only where a file would be overwritten. Kinds are a
flag (cargo --bin/--lib, swift --type, dart --template), none (go, gleam), or both at once (zig). No
verb read is named `make`. Every published unit is a package or a module, and every C side read a
native, system, C or dynamic library; yet Cargo's `--lib` and Swift's `library` reuse that word for
the importable kind. Dart's `package` template and Zig's "when making a package" keep the two apart,
as spec § 13 does.

(120 words, counted by hand.)

---

## condition

Any of these would change this reading:

- **A language document that states its manifest**, its source kinds or its checksums. None of the
  ten read does. Its record of what that cost would bear on § 1's catalogue text.
- **A page recording harm from Go's split.** For example, another build tool or the compiler
  accepting an import that the go command refuses, so one program builds in one tool and not
  another. That would argue that a refusal lives better in the reader's one document. Issue #46792
  states only the split, not the harm. Issue 10478 (`go run` and internal packages) came up in a
  search and was **not read**.
- **A page showing a rule moved between a language's specification and its tool manual**, either
  way, with a stated reason.
- **Zig's documents stating that a build re-hashes `zig-pkg`**, or Go's stating that a build
  re-hashes the extracted cache. Either would make Heroes' build-time check common practice rather
  than Cargo's alone.
- **Q-stub, any of:**
  - a tool whose project verb is `make`, and what its users' record says;
  - a tracker record of Cargo or SwiftPM users confusing the `lib`/`library` kind with a native
    library;
  - a recorded loss of files from a verb that writes into the current folder;
  - a page explaining why Cargo and Dart chose a new folder, or why SwiftPM, Go and Zig chose the
    current one.

  None was looked for.

---

## search

Instruments: `WebSearch` and `WebFetch`, every page on 2026-11-10. Two PDFs or long pages saved by
the fetching tool were read with `Read` (the Oberon report, pages 1, 14, 15, 16 and 17; the C#
chapter, lines 44 to 424). A sentence saying something was **not found** names the pages it was
looked for in.

**Web searches, exact queries:**

1. `cargo vendor "the listed checksum of" "has changed" vendored file modified error` (led to Cargo's test suite, then read at its GitHub source; the Mozilla, Guix and Nix results were not read and are not cited)
2. `Cargo.lock checksum field path dependency git dependency no checksum cargo documentation` (no Cargo Book page; the test-suite and issue #10939 results were not read and are not cited)
3. `Dart language specification "package:" URI interpretation "implementation" import dartLangSpec` (led to the `.tex` source)
4. `Swift Package Manager documentation package dependency url path registry "Package.resolved" checksum registry dependencies` (vendor pages only, not cited; SE-0292 read instead)
5. `swift-package-manager PackageDescription ".package(path:" "local package" documentation "source control" dependency kinds docs.swift.org swiftpm` (led to Apple's `package(name:path:)` page)
6. `Zig 0.15 OR 0.16 release notes "zig-pkg" package fetch directory global cache "p/" packages stored project local` (secondary results only, not cited)
7. `ziglang.org download 0.16.0 release notes package management zig-pkg` (secondary results; the official notes read instead)
8. `golang proposal spec "internal packages" language specification "go command" not the language enforce import restriction issue` (led to #46792 and the golang-dev thread)
9. `swift package manager documentation "swift package edit" dependency "checkouts" ".build" editable "Packages" directory official docs` (led to the Xcode editing guide)
10. Q-stub: `"zig init" creates build.zig build.zig.zon src/main.zig src/root.zig "--minimal" release notes ziglang.org` (secondary results, not cited; Zig's source read instead)
11. Q-stub: `"swift package init" "--type" library executable tool macro empty build-tool-plugin documentation swiftpm` (Debian mirrors and forums, not cited; SwiftPM's source read instead)
12. Q-stub: `cargo source "already exists" "Use `cargo init` to initialize the directory" cargo_new.rs github rust-lang/cargo` (man-page copies only, not cited; Cargo's test suite read instead)

**Pages read**, by system:

- **Go:** https://go.dev/ref/spec (all three parts); https://pkg.go.dev/cmd/go (both parts); https://go.dev/doc/go1.4; https://go.dev/doc/devel/release; https://go.dev/ref/mod (all three parts, seven prompts, the `go mod init` section among them); https://go.dev/doc/go1compat; https://github.com/golang/go/issues/46792; https://groups.google.com/g/golang-dev/c/_cAggq73yME (cut off); https://pkg.go.dev/cmd/cgo.
- **Rust and Cargo:**
  - the Reference's extern-crates page;
  - the Cargo Book's Specifying Dependencies, Source Replacement, `cargo vendor`, `cargo new`, `cargo init` and Build Scripts pages;
  - `tests/testsuite/directory.rs` and `tests/testsuite/new.rs` on GitHub.
- **Dart:** pub's dependencies page (two prompts), glossary and `pub get` page; `dartLangSpec.tex` (three parts); `dart create`; C interop.
- **Swift:**
  - the swift-book source `Declarations.md` (both parts);
  - Apple's `Package.Dependency` (Markdown form), `package(name:path:)` and `Target.systemLibrary`;
  - SE-0292 and SE-0386;
  - the Xcode guide on editing a dependency;
  - SwiftPM's `Init.swift` and `InitPackage.swift` on GitHub.
- **Zig:**
  - `build.zig.zon.md` (GitHub master, two prompts; Codeberg at 0.16.0, re-read);
  - the master language reference (two of its parts);
  - the 0.16.0 release notes (both parts) and `download/index.json`;
  - the 0.14.0 release notes (both parts; no mention of `zig init` found in either);
  - `src/main.zig`, `lib/init/src/root.zig` and `lib/init/src/main.zig` at the 0.16.0 tag on Codeberg;
  - the build-system guide;
  - GitHub's repository page (the "Moved to Codeberg" notice).
- **Gleam:** the command-line reference (reached after two redirects); `compiler-cli/src/new.rs` on GitHub.
- **Beside the five:** the Oberon-07 Report (Wirth, the Report tradition `.claude/rules/spec-shape.md` names); JLS SE 21 chapter 7; the Kotlin specification's declarations and packages-and-imports chapters; the C# specification's basic-concepts chapter; the Python Language Reference's import-system chapter. These five were chosen because they have a written language document, and Kotlin, C# and Java also because they have a visibility unit above the file. The list is the seat's, not an enumeration of the world.

**Could not be read, so left out:**

- the Go 1.4 design document (a Google Doc; the tool saw only its title);
- `docs.swift.org`'s rendered Swift book (empty without scripts; the GitHub source read instead);
- `swift-package-manager`'s `PackageDependency.swift` on GitHub (HTTP 404);
- Apple's `Package.Dependency` HTML page (empty; the `.md` read instead);
- the Oberon PDF through the fetching tool (compressed; the saved copy read instead);
- Cargo's `src/cargo/ops/cargo_new.rs` on GitHub (HTTP 404);
- Zig's `src/main.zig` at a 0.16.0 tag on GitHub (HTTP 404; the repository has moved, Codeberg read instead).

**Looked for and not found:**

- A Cargo Book sentence on a path dependency's checksum (Specifying Dependencies, Source Replacement, `cargo vendor`, query 2).
- A pub sentence on a path dependency's hash (dependencies, glossary, `pub get`).
- An official SwiftPM page saying where checkouts live (query 9).
- A Zig sentence on re-checking `zig-pkg` at build (the 0.16.0 subsections "Fetch Packages Into Project-Local Directory" and "Ability to Override Packages Locally", and `build.zig.zon.md`).
- A sentence in the Go 1 compatibility page naming the `go` command.
- What `cargo init` does over an existing `Cargo.toml` (its reference page; the source path tried answered 404).
- A page naming a project verb `make` among the seven (their reference pages and sources above). Other tools were not swept.
- **Among the ten language documents listed in § D, one that states a manifest, a source kind or a checksum.** This is a statement about those ten, not about every language; the general question stays a question.

**Repository files read:** `docs/panel/244-briefs/00-shared.md` (twice, the second time for
§ A question added by the author), `historian.md`, `244-reports/critic-briefs.md`,
`docs/panel/235-reports/historian.md`, and `spec/heroes-spec.md:15-44`, all in
`.claude/worktrees/p244`. No command was run in the repository, and nothing was written but this
file.

report complete
