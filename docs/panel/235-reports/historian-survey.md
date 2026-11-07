# Panel 235, the historian's survey, written before the sitting

**Status: complete, 2026-11-07** (written in seven passes the same day, each
saved as it was reached; the last pass removed quotation marks from four
phrases a page reader had paraphrased, and added the YAML 1.1 specification's
own words to Table 4).

This survey gives no verdict and recommends nothing (the brief,
`docs/panel/235-briefs/historian-survey.md`). It reports what other package
systems do, each fact from a page read on **2026-11-07**. Every cell carries
its own URL and the day it was read. A quotation is the page's own words; a
cell says only what its page states. Where a page was searched and did not
carry the answer, the cell says `not found` and names what was looked for and
where; it never says that the thing does not exist.

**How the pages were read.** Every page was read through a fetching tool that
answers a prompt about the page; where a cell says *as the reader rendered
it*, the wording is that tool's paraphrase of the page and not a quotation,
and it is kept only where the page's own words were not returned. Nothing was
run: no `go`, `cargo`, `npm` or other command was executed, so every behaviour
below is as documented, not as measured.

*Heroes of code* was not used as a source for any cell.

---

## Table 1, the full inventory of the two candidates

### 1A. Go: `go.mod` and `go.sum`

**The release.** The newest Go release is **go1.27.3, released 2026-10-19**;
the newest major release is **go1.27.0, released 2026-08-19**
(https://go.dev/doc/devel/release, read 2026-11-07). The inventory below is the
Go Modules Reference as served on that day (https://go.dev/ref/mod, read
2026-11-07), which does not name the release it describes. Its opening: "The
`go.mod` file is line-oriented. Each line holds a single directive, made up of
a keyword followed by arguments." "The `go.mod` file is designed to be human
readable and machine writable." (same page, read 2026-11-07).

| directive | what it does, in the page's words | required? | declares a language version or a toolchain? | source, read |
|---|---|---|---|---|
| `module` | "A `module` directive defines the main module's path." | Yes: "A `go.mod` file must contain exactly one `module` directive." | No | https://go.dev/ref/mod, read 2026-11-07 |
| `go` | "A `go` directive indicates that a module was written assuming the semantics of a given version of Go." | "A `go.mod` file may contain at most one `go` directive. Most commands will add a `go` directive with the current Go version if one is not present." "If the `go` directive is missing, `go 1.16` is assumed." | **Yes, a language version.** "Before Go 1.21, the directive was advisory only; now it is a mandatory requirement: Go toolchains refuse to use modules declaring newer Go versions." "The `go` directive is an input into selecting which Go toolchain to run." Behaviour is gated on its value at 1.14, 1.16, 1.17, 1.21 and 1.27 (row *go-gated behaviour* below). | https://go.dev/ref/mod, read 2026-11-07 |
| `toolchain` | "A `toolchain` directive declares a suggested Go toolchain to use with a module." | Not stated as required. | **Yes, a toolchain.** "The suggested Go toolchain's version cannot be less than the required Go version declared in the `go` directive. The `toolchain` directive only has an effect when the module is the main module and the default toolchain's version is less than the suggested toolchain's version." | https://go.dev/ref/mod, read 2026-11-07 |
| `godebug` | "A `godebug` directive declares a single GODEBUG setting to apply when this module is the main module." | Not stated as required. | Not a version, but Go 1.27 refuses an old value of a removed setting: "If they are set to an old value, the `go` command will fail." | https://go.dev/ref/mod and https://go.dev/doc/go1.27, both read 2026-11-07 |
| `require` | "A `require` directive declares a minimum required version of a given module dependency." | Not stated as required. | No; at `go 1.17` and above the file lists every module providing a transitively imported package (row *go-gated behaviour*). | https://go.dev/ref/mod, read 2026-11-07 |
| `tool` | "Since Go 1.24, a `tool` directive adds a package as a dependency of the current module." | Not stated as required. | No | https://go.dev/ref/mod, read 2026-11-07 |
| `ignore` | "An `ignore` directive will cause the go command ignore the slash-separated directory paths, and any files or directories recursively contained in them, when matching package patterns." | Not stated as required. | No | https://go.dev/ref/mod, read 2026-11-07 |
| `exclude` | "An `exclude` directive prevents a module version from being loaded by the `go` command." | Not stated as required. | No | https://go.dev/ref/mod, read 2026-11-07 |
| `replace` | "A `replace` directive replaces the contents of a specific version of a module, or all versions of a module, with contents found elsewhere." A local path "must contain a `go.mod` file. The replacement version must be omitted in this case." | Not stated as required. | No | https://go.dev/ref/mod, read 2026-11-07 |
| `retract` | "A `retract` directive indicates that a version or range of versions of the module defined by `go.mod` should not be depended upon." | Not stated as required. | No; "The `retract` directive was added in Go 1.16." | https://go.dev/ref/mod, read 2026-11-07 |
| `// indirect` (a comment the `go` command reads) | At `go 1.17` and above, "Because there may be many more `// indirect` dependencies than in previous `go` versions, indirect dependencies are recorded in a separate block within the `go.mod` file." | No | No | https://go.dev/ref/mod, read 2026-11-07 |
| `// Deprecated:` (a module comment) | "Module authors may deprecate a module by adding a `// Deprecated:` comment to `go.mod`, then tagging a new version. `go` `get` now prints a warning if a module needed to build packages named on the command line is deprecated." (Go 1.17) | No | No | https://go.dev/doc/go1.17, read 2026-11-07 |
| *go-gated behaviour* | "At `go 1.14` or higher, automatic vendoring may be enabled." "At `go 1.16` or higher, the `all` package pattern matches only packages transitively imported by packages and tests in the main module." At 1.17: "The `go.mod` file includes an explicit `require` directive for each module that provides any package transitively imported by a package or test in the main module." At 1.21: "The `go` line must be greater than or equal to the `go` line of all dependencies." Go 1.27: "For modules specifying `go 1.27` or later in their `go.mod` file, `go mod tidy` now automatically merges duplicate require blocks." | n/a | **Yes**: the meaning of the file changes with the declared version. | https://go.dev/ref/mod and https://go.dev/doc/go1.27, both read 2026-11-07 |
| workspaces (`go.work`) | **Excluded by the author.** The page's `go.work` grammar exists beside `go.mod`'s. | n/a | n/a | https://go.dev/ref/mod, read 2026-11-07 |

**Two inconsistencies on the reference page itself**, read twice on
2026-11-07 (https://go.dev/ref/mod): the grammar's `Directive` production lists
`ModuleDirective | GoDirective | ToolDirective | IgnoreDirective |
RequireDirective | ExcludeDirective | ReplaceDirective | RetractDirective`,
while `ToolchainDirective` and `GodebugDirective` are defined elsewhere on the
page and are not in it; and the lexical section says "Allowed keywords are
`module`, `go`, `require`, `replace`, `exclude`, and `retract`", four
directives short. And the page says "For reproducibility, the `go` command
writes its own toolchain name in a `toolchain` line any time it is updating
the `go` version in the `go.mod` file (usually during `go get`)", while the Go
1.25 release notes say "When the go command updates the `go` line in a
`go.mod` or `go.work` file, it no longer adds a toolchain line specifying the
command's current version" (https://go.dev/doc/go1.25, read 2026-11-07).

| `go.sum` line | what it is, in the page's words | source, read |
|---|---|---|
| `<module> <version> h1:<hash>` | "Each line in go.sum has three fields separated by spaces: a module path, a version (possibly ending with `/go.mod`), and a hash." Without the `/go.mod` suffix, "the hash is for the files within the module's `.zip` file." "For module zip files, the `go` command computes the hash from the names and contents of files within the archive in a deterministic order." "Note that the `.zip` file itself is not hashed, so file order, compression, alignment, and metadata don't affect the hash." | https://go.dev/ref/mod, read 2026-11-07 |
| `<module> <version>/go.mod h1:<hash>` | "If the version ends with `/go.mod`, the hash is for the module's `go.mod` file only." "For `go.mod` files, the `go` command computes the hash from the file content." | https://go.dev/ref/mod, read 2026-11-07 |
| the hash field | "an algorithm name (like `h1`) and a base64-encoded cryptographic hash, separated by a colon (`:`). Currently, SHA-256 (`h1`) is the only supported hash algorithm." | https://go.dev/ref/mod, read 2026-11-07 |

### 1B. Cargo: `Cargo.toml` and `Cargo.lock`

**The release.** "Announcing Rust 1.99.0", Oct. 1, 2026, and "Announcing
Rust 1.99.1", Oct. 15, 2026, are the two newest announcements
(https://blog.rust-lang.org/releases/, read 2026-11-07); the 1.99.0 post links
Cargo's notes as "cargo-199-2026-10-01"
(https://blog.rust-lang.org/2026/10/01/Rust-1.99.0/, read 2026-11-07). The
stable changelog served that day names `Cargo 1.99 (2026-10-01)` with empty
*Added* and *Fixed* sections and a comparison link to `HEAD`
(https://doc.rust-lang.org/cargo/CHANGELOG.html, read 2026-11-07). The
inventory is the manifest reference as served on that day
(https://doc.rust-lang.org/cargo/reference/manifest.html, read 2026-11-07).

**Top-level tables**, every one the page lists, its own description:

| table | what it does | required? | declares a language version, toolchain or edition? | source, read |
|---|---|---|---|---|
| `cargo-features` | "Unstable, nightly-only features." | No | No | https://doc.rust-lang.org/cargo/reference/manifest.html, read 2026-11-07 |
| `[package]` | "Defines a package." | `name` is: "The only field required by Cargo is `name`." | Holds `edition`, `rust-version` and `resolver` (below) | same, read 2026-11-07 |
| `[lib]` | "Library target settings." | No | No | same, read 2026-11-07 |
| `[[bin]]` | "Binary target settings." | No | No | same, read 2026-11-07 |
| `[[example]]` | "Example target settings." | No | No | same, read 2026-11-07 |
| `[[test]]` | "Test target settings." | No | No | same, read 2026-11-07 |
| `[[bench]]` | "Benchmark target settings." | No | No | same, read 2026-11-07 |
| `[dependencies]` | "Package library dependencies." | No | No | same, read 2026-11-07 |
| `[dev-dependencies]` | "Dependencies for examples, tests, and benchmarks." | No | No | same, read 2026-11-07 |
| `[build-dependencies]` | "Dependencies for build scripts." | No | No | same, read 2026-11-07 |
| `[target]` | "Platform-specific dependencies." | No | No | same, read 2026-11-07 |
| `[badges]` | "Badges to display on a registry." | No | No | same, read 2026-11-07 |
| `[features]` | "Conditional compilation features." | No | No | same, read 2026-11-07 |
| `[lints]` | "Configure linters for this package." | No | No | same, read 2026-11-07 |
| `[hints]` | "Provide hints for compiling this package." "Hints are, by design, always safe for Cargo to ignore" … "There are no stable hints at this time." "MSRV: Respected as of 1.90." | No | No | same, read 2026-11-07 |
| `[patch]` | "Override dependencies." | No | No | same, read 2026-11-07 |
| `[replace]` | "Override dependencies (deprecated)." | No | No | same, read 2026-11-07 |
| `[profile]` | "Compiler settings and optimizations." | No | No | same, read 2026-11-07 |
| `[workspace]` | "The workspace definition." **Excluded by the author.** | No | n/a | same, read 2026-11-07 |

**`[package]` keys**, every one the page lists:

| key | what it does | required? | declares a language version, toolchain or edition? | source, read |
|---|---|---|---|---|
| `name` | "The name of the package." "The name must use only alphanumeric characters or `-` or `_`, and cannot be empty." | **Yes**, the only one | No | https://doc.rust-lang.org/cargo/reference/manifest.html, read 2026-11-07 |
| `version` | "The version of the package." Optional, defaulting to `0.0.0`; "The field is required for publishing packages." | For publishing | No | same, read 2026-11-07 |
| `authors` | "The authors of the package (deprecated)." | No | No | same, read 2026-11-07 |
| `edition` | "The `edition` key is an optional key that affects which Rust Edition your package is compiled with." "By default `cargo new` creates a manifest with the 2024 edition currently." "If the `edition` field is not present in `Cargo.toml`, then the 2015 edition is assumed for backwards compatibility." | No | **Yes, an edition**; its absence also means one (2015). | same, read 2026-11-07 |
| `rust-version` | "The `rust-version` field tells cargo what version of the Rust toolchain you support for your package." | No | **Yes, a toolchain version** | same, read 2026-11-07 |
| `description` | "A description of the package." "crates.io requires the `description` to be set." | For crates.io | No | same, read 2026-11-07 |
| `documentation` | "URL of the package documentation." | No | No | same, read 2026-11-07 |
| `readme` | "Path to the package's README file." | No | No | same, read 2026-11-07 |
| `homepage` | "URL of the package homepage." | No | No | same, read 2026-11-07 |
| `repository` | "URL of the package source repository." | No | No | same, read 2026-11-07 |
| `license` | "The package license." "crates.io requires either `license` or `license-file` to be set." | One of the two, for crates.io | No | same, read 2026-11-07 |
| `license-file` | "Path to the text of the license." | (above) | No | same, read 2026-11-07 |
| `keywords` | "Keywords for the package." | No | No | same, read 2026-11-07 |
| `categories` | "Categories of the package." | No | No | same, read 2026-11-07 |
| `workspace` | "Path to the workspace for the package." **Excluded with workspaces.** | No | No | same, read 2026-11-07 |
| `build` | "The default is `"build.rs"`, which loads the script from a file named `build.rs` in the root of the package." | No | No | same, read 2026-11-07 |
| `links` | "The `links` field specifies the name of a native library that is being linked to." | No | No | same, read 2026-11-07 |
| `exclude` | "Files to exclude when publishing." | No | No | same, read 2026-11-07 |
| `include` | "Files to include when publishing." | No | No | same, read 2026-11-07 |
| `publish` | "Can be used to prevent publishing the package." | No | No | same, read 2026-11-07 |
| `metadata` | "Extra settings for external tools." | No | No | same, read 2026-11-07 |
| `default-run` | "The default binary to run by `cargo run`." | No | No | same, read 2026-11-07 |
| `autolib`, `autobins`, `autoexamples`, `autotests`, `autobenches` | "Disables library auto discovery." and the same for binaries, examples, tests and benches. | No | No | same, read 2026-11-07 |
| `resolver` | "Sets the dependency resolver to use." Values `"1"`, `"2"` ("`edition = "2021"` default") and `"3"` ("`edition = "2024"` default, requires Rust 1.84+"). "The `resolver` version in dependencies is ignored, only the value in the top-level package will be used." | No | **Tied to the edition**: its default follows `edition`. | same, and https://doc.rust-lang.org/cargo/reference/resolver.html, read 2026-11-07 |

**A dependency entry's keys** (https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html, read 2026-11-07):
`version` (a requirement, "`1.2.3 := >=1.2.3, <2.0.0`"); `git` with one of
`branch`, `tag`, `rev`; `path`; `registry` ("the registry name configured in
`.cargo/config.toml`"); `package` ("The `package` key, if not specified,
defaults to the name of the dependency being requested."); `features`,
`default-features`, `optional`; `workspace = true` (excluded with workspaces).
"It is possible to specify both a registry version and a `git` or `path`
location … Other combinations are not allowed."

**`Cargo.lock`**: the documentation describes it by example and states "It is
maintained by Cargo and should not be manually edited"
(https://doc.rust-lang.org/cargo/guide/cargo-toml-vs-cargo-lock.html, read
2026-11-07). Its fields are the schema in Cargo's own source
(https://raw.githubusercontent.com/rust-lang/cargo/master/crates/cargo-util-schemas/src/lockfile.rs,
read 2026-11-07):

| field | what it holds, in the source's doc comments | required? | declares a version? | source, read |
|---|---|---|---|---|
| `version` | "The lockfile format version (`version =` field)." "This field is optional for backward compatibility." | No | **A format version** of the lock file, not of the language | lockfile.rs above, read 2026-11-07 |
| `[[package]]` | "The list of `[[package]]` entries describing each resolved dependency." | No | No | same, read 2026-11-07 |
| `[[package]].name` | "The name of the dependency." | Yes (a `String`) | No | same, read 2026-11-07 |
| `[[package]].version` | "The version of the dependency." | Yes (a `String`) | No | same, read 2026-11-07 |
| `[[package]].source` | Optional; "Cargo does not serialize path dependencies." Encodings shown: `registry+https://github.com/rust-lang/crates.io-index`, `sparse+https://…`, `git+https://…` with `?branch=`, `?tag=`, `?rev=`, `path+file:///…`. The guide's example shows a git source ending `#<commit>`: `git+https://github.com/rust-lang/regex.git#9f9f693768c584971a4d53bc3c586c33ed3a6831`. | No | No | same, and the guide above, read 2026-11-07 |
| `[[package]].checksum` | "The checksum of the dependency." Optional. | No | No | same, read 2026-11-07 |
| `[[package]].dependencies` | "The transitive dependencies used by this dependency." Each named by `name`, with `version` and `source` added "only when necessary to disambiguate between packages". | No | No | same, read 2026-11-07 |
| `[[package]].replace` | "The replace of the dependency." | No | No | same, read 2026-11-07 |
| `[root]` (legacy) | "Older lockfiles have the root package separated." | No | No | same, read 2026-11-07 |
| `[metadata]` (legacy) | "In older lockfile versions, dependency checksums were stored here instead of alongside each" [package]. | No | No | same, read 2026-11-07 |
| `[[patch.unused]]` | "The list of unused dependency patches." | No | No | same, read 2026-11-07 |

The default lock format is version 4 since Cargo 1.83.0: "This commit makes
lockfile version 4 the default version when Cargo tries to write to a
lockfile." "The lockfile version 4 has been stabilized since 1.78.0, and will
become default in 1.83.0." (rust-lang/cargo pull request 14595, merged
2024-09-25, https://github.com/rust-lang/cargo/pull/14595, read 2026-11-07).

---

## Table 2, the package systems

Each system is asked the brief's ten probes, in its order: (1) naming in the
source; (2) where address, version and fingerprint are written, and over which
bytes; (3) the words; (4) version selection, and two versions of one
dependency; (5) registry, git and local folders together; (6) where fetched
code is kept; (7) whether reading, fetching or building runs the dependency's
code; (8) commands and the exact errors; (9) privacy of a dependency's
internals; (10) what its history had to correct, with release and date.

### 2.1 Go modules (Go 1.27.3, 2026-10-19)

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | A dependency is named by its full import path, a string: `ImportSpec = [ "." \| PackageName ] ImportPath .` A local name can stand for it: "The PackageName is used in qualified identifiers to access exported identifiers of the package within the importing source file. It is declared in the file block." "The interpretation of the ImportPath is implementation-dependent". | https://go.dev/ref/spec (offset 200000), read 2026-11-07 |
| 1. own code against fetched | "A package path is the module path joined with the subdirectory containing the package (relative to the module root)." The standard library is told by its path: "The Go standard library uses package paths that do not contain a dot in the first path element, and the `go` command does not attempt to resolve such paths from network servers." | https://go.dev/ref/mod, read 2026-11-07 |
| 1. two claim one name | "If exactly one module in the build list provides the package, that module is used. If no modules provide the package or if two or more modules provide the package, the `go` command reports an error." The error: `"ambiguous import: found package %s in multiple %s:"`, the second `%s` being `"modules"` or `"directories"`. | https://go.dev/ref/mod and https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/modload/import.go, both read 2026-11-07 |
| 2. where written | The minimum version in `go.mod`'s `require`; the fingerprint in a separate file, `go.sum` (Table 1A). Over which bytes: "the names and contents of files within the archive in a deterministic order", SHA-256, "the `.zip` file itself is not hashed"; a second line hashes the `go.mod` file's content. The page states one hash algorithm, `h1`, and no separate kind of line for a module fetched directly from version control. | https://go.dev/ref/mod, read 2026-11-07 |
| 2. a second check | "The checksum database is a global source of `go.sum` lines." "It is a Transparent Log (or "Merkle Tree") of `go.sum` line hashes, which is backed by Trillian." "`GOSUMDB` defaults to `sum.golang.org`, the Go checksum database run by Google." | https://go.dev/ref/mod, read 2026-11-07 |
| 3. words | "A module is a collection of packages that are released, versioned, and distributed together." "Each package within a module is a collection of source files in the same directory that are compiled together." "A module is identified by a module path, which is declared in a `go.mod` file". "A version identifies an immutable snapshot of a module". | https://go.dev/ref/mod, read 2026-11-07 |
| 4. selection | "Go uses an algorithm called Minimal version selection (MVS)". "At the end of the traversal, the highest required versions comprise the build list: they are the minimum versions that satisfy all requirements." Two versions: "Major version suffixes let multiple major versions of a module coexist in the same build"; "modules with distinct suffixes are treated as separate modules". | https://go.dev/ref/mod, read 2026-11-07 |
| 5. sources together | The default "`GOPROXY` is: `https://proxy.golang.org,direct`"; "The keyword `direct` instructs the `go` command to download modules from version control repositories where they're developed instead of using a proxy." `GOPRIVATE` "instructs the `go` command not to connect to a proxy or to the checksum database" for matching modules. A local folder is a `replace` to a path that "must contain a `go.mod` file". A `go.sum` line holds three fields, "a module path, a version (possibly ending with `/go.mod`), and a hash". | https://go.dev/ref/mod, read 2026-11-07 |
| 6. where kept | "The default location of the module cache is `$GOPATH/pkg/mod`." "The `go` command creates module source files and directories in the cache with read-only permissions". In the project, optionally: "The `go mod vendor` command constructs a directory named `vendor` in the main module's root directory". | https://go.dev/ref/mod, read 2026-11-07 |
| 7. runs code? | **No, by design.** Under the heading "Building code doesn't execute it": "It is an explicit security design goal of the Go toolchain that neither fetching nor building code will let that code execute, even if it is untrusted and malicious." "This is different from most other ecosystems, many of which have first-class support for running code at package fetch time." (Filippo Valsorda, *How Go Mitigates Supply Chain Attacks*, 31 March 2022.) A `tool` directive's tool runs when invoked: "the `go tool` command can be used to build and run tools declared in `go.mod` files". | https://go.dev/blog/supply-chain (read twice) and https://go.dev/ref/mod, read 2026-11-07 |
| 8. commands | `go get` (the page: "Users should prefer `go get path@version` or `go get path@none`"), `go mod tidy` ("adds any missing module requirements … It also adds any missing entries to `go.sum` and removes unnecessary entries"), `go mod download`, `go mod verify` ("checks that dependencies of the main module stored in the module cache have not been modified since they were downloaded"). | https://go.dev/ref/mod, read 2026-11-07 |
| 8. errors | Fingerprint mismatch: `"verifying %s@%s: checksum mismatch\n\tdownloaded: %v\n\t%s:     %v"` then "SECURITY ERROR / This download does NOT match an earlier download recorded in go.sum. / The bits may have been replaced on the origin server, or an attacker may / have intercepted the download attempt. / For more information, see 'go help module-auth'." It names a help topic, not a command. Missing dependency: `"no required module provides package %s"` then `"; to add it:\n\tgo get %s"`; missing sum: `"missing go.sum entry for module providing package %s%s"` with `"; to add:\n\tgo get%s %s%s"` or `"; to add:\n\tgo mod download %s"`. Both name their fix. | https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/modfetch/fetch.go and …/modload/import.go, both read 2026-11-07 |
| 9. privacy | "When the `go` command sees an import of a package with `internal` in its path, it verifies that the package doing the import is within the tree rooted at the parent of the `internal` directory." "For Go 1.4, the internal package mechanism is enforced for the main Go repository; from 1.5 and onward it will be enforced for any repository." The error text an `internal` import produces: `not found` in `modload/import.go` (searched for it there). | https://go.dev/doc/go1.4 and https://go.dev/doc/go1.5, read 2026-11-07 |
| 10. history | See the timeline below the table. | (each row its own) |

**Go's history, from its first way of getting a dependency to today's**, each
date from https://go.dev/doc/devel/release (read 2026-11-07) and each sentence
from the page named:

- **go1, 2012-03-28.** What it lacked, in the words of the proposal that
  replaced it six years later: "But go get is missing any concept of
  versioning." "When go get needs a package, it always fetches the latest
  copy"; "without a concept of versioning, it cannot ensure or even express
  the idea of a reproducible build"; and "the inability of GOPATH to
  comprehend multiple versions of a package" (Russ Cox, *Go += Package
  Versioning*, February 20, 2018, https://research.swtch.com/vgo-intro, read
  2026-11-07).
- **go1.4, 2014-12-10**: `internal` packages for the main repository, and
  canonical import comments, `package pdf // import "rsc.io/pdf"`, after which
  "the `go` command will refuse to compile a package that imports
  `github.com/rsc/pdf`" (https://go.dev/doc/go1.4, read 2026-11-07).
- **go1.5, 2015-08-19**: "experimental support for “vendoring” external
  dependencies"; `internal` "now provided for all repositories"
  (https://go.dev/doc/go1.5, read 2026-11-07).
- **go1.6, 2016-02-17**: vendoring "no longer considered experimental, and
  enables it by default" (https://go.dev/doc/go1.6, read 2026-11-07).
- **dep**: "Dep was an official experiment to implement a package manager for
  Go." "As of 2020, Dep is deprecated and archived in favor of Go modules";
  "This repository was archived by the owner on Sep 9, 2020"
  (https://github.com/golang/dep, read 2026-11-07). The vgo post: "dep is not
  a direct prototype of the eventual go command integration of package
  versioning." "The prototype is a standalone command we call vgo."
  (https://research.swtch.com/vgo-intro, read 2026-11-07).
- **go1.11, 2018-08-24**: "Go 1.11 adds preliminary support for a new concept
  called "modules," an alternative to GOPATH". "Module support is considered
  experimental." (https://go.dev/doc/go1.11, read 2026-11-07).
- **go1.13, 2019-09-03**: "the go command by default downloads and
  authenticates modules using the Go module mirror and Go checksum database
  run by Google"; `GOPRIVATE` added (https://go.dev/doc/go1.13, read
  2026-11-07).
- **go1.14, 2020-02-25**: "Module support in the `go` command is now ready
  for production use" (https://go.dev/doc/go1.14, read 2026-11-07).
- **go1.16, 2021-02-16**: "Module-aware mode is enabled by default"; "Build
  commands like `go build` and `go test` no longer modify `go.mod` and
  `go.sum` by default"; `retract` (https://go.dev/doc/go1.16, read
  2026-11-07).
- **go1.17, 2021-08-16**: pruned module graphs, indirect requirements listed,
  `// Deprecated:` (https://go.dev/doc/go1.17, read 2026-11-07).
- **go1.21.0, 2023-08-08**: "Go 1.21 now reads the `go` line in a `go.work`
  or `go.mod` file as a strict minimum requirement"; "The new `toolchain`
  directive sets a suggested minimum toolchain to use"
  (https://go.dev/doc/go1.21, read 2026-11-07).
- **go1.23.0, 2024-08-13**: "The new `godebug` directive in `go.mod` and
  `go.work` declares a GODEBUG setting" (https://go.dev/doc/go1.23, read
  2026-11-07).
- **go1.24.0, 2025-02-11**: `tool` directives, "This removes the need for the
  previous workaround of adding tools as blank imports to a file
  conventionally named "tools.go"" (https://go.dev/doc/go1.24, read
  2026-11-07).
- **go1.25.0, 2025-08-12**: the `ignore` directive; no longer adds a
  toolchain line when updating `go` (https://go.dev/doc/go1.25, read
  2026-11-07).
- **go1.26.0, 2026-02-10**: "`go mod init` now defaults to a lower `go`
  version in new `go.mod` files" (https://go.dev/doc/go1.26, read 2026-11-07).
- **go1.27.0, 2026-08-19**: "The `go` command no longer has support for the
  `bzr` version control system"; `go mod tidy` merges duplicate require blocks
  (https://go.dev/doc/go1.27, read 2026-11-07).

The FAQ's versioning entry was read for a sentence on how long versioning
took: `not found` there; it opens "Modules were introduced in Go 1.11 and have
been ready for production use since 1.14" (https://go.dev/doc/faq, read
2026-11-07). On the author's premise that Go took years: the dates above put
go1 at 2012-03-28, modules' first preliminary release at 2018-08-24, "ready for
production use" at 2020-02-25 and module mode by default at 2021-02-16; how
long the work itself took is not stated on any page read.

### 2.2 Cargo (Rust 1.99.0, 2026-10-01; 1.99.1, 2026-10-15)

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | In code a crate is a name in `use` paths: "Beginning in the 2018 edition, use declarations can reference crates in the extern prelude". A local name can stand for it in the manifest: "Cargo supports a `package` key in the `[dependencies]` section of which package should be depended on": `bar = { git = "https://github.com/example/project.git", package = "foo" }`, used as `bar`; "The `package` key, if not specified, defaults to the name of the dependency being requested." | https://doc.rust-lang.org/reference/names/preludes.html and https://doc.rust-lang.org/cargo/reference/specifying-dependencies.html, read 2026-11-07 |
| 1. own code against fetched | "you can refer to the root of the current crate with the `crate::` prefix"; "The prefix `::` … now unambiguously refers to an external crate. For instance, `::foo::bar` always refers to the name `bar` inside the external crate `foo`." Neither prefix is required. | https://doc.rust-lang.org/edition-guide/rust-2018/path-changes.html, read 2026-11-07 |
| 1. two claim one name | A local module against a crate: "If you have a local module or item with the same name as an external crate, a path beginning with that name will be taken to refer to the local module or item. To explicitly refer to the external crate, use the `::name` form." Two dependencies wanting one name: the `package` key above renames one of them. | the edition guide and specifying-dependencies above, read 2026-11-07 |
| 2. where written | Requirement in `Cargo.toml`; `checksum` in `Cargo.lock`, a separate file (Table 1B). For a registry source the index's `cksum` is "A SHA256 checksum of the `.crate` file" (the archive's bytes). For a git source the guide's lock example writes `source = "git+https://github.com/rust-lang/regex.git#9f9f693768c584971a4d53bc3c586c33ed3a6831"` and no `checksum` line; "Cargo locks the commits of `git` dependencies in `Cargo.lock` file at the time of their addition". Path sources: "Cargo does not serialize path dependencies." | https://doc.rust-lang.org/cargo/reference/registry-index.html, https://doc.rust-lang.org/cargo/guide/cargo-toml-vs-cargo-lock.html, specifying-dependencies above, lockfile.rs (Table 1B), all read 2026-11-07 |
| 2. the check in code | `let actual = Sha256::new().update(data).finish_hex(); if actual != checksum { anyhow::bail!("failed to verify the checksum of `{}`", pkg) }` | https://doc.rust-lang.org/beta/nightly-rustc/src/cargo/sources/registry/download.rs.html, read 2026-11-07 |
| 3. words | "A Rust crate is either a library or an executable program". "A package is a collection of source files and a `Cargo.toml` manifest file which describes the package. A package has a name and version". "A package contains multiple targets, each of which is a crate." Modules "provide isolated namespaces within the code"; "The source code for a given crate may be subdivided into one or more separate modules." "A manifest is a description of a package or a workspace in a file named `Cargo.toml`." "The `Cargo.lock` lock file is a file that captures the exact version of every dependency". The unit with a version is the package; a crate "may also refer to a compressed package fetched from a registry". A word for one source file: `not found` in the glossary (searched there). | https://doc.rust-lang.org/cargo/appendix/glossary.html, read 2026-11-07 |
| 4. selection | A solver over ranges: "Generally, Cargo prefers the highest version currently available." "Cargo assumes packages follow SemVer and will unify dependency versions if they are SemVer compatible according to the Caret version requirements. If two compatible versions cannot be unified because of conflicting version requirements, Cargo will error." Two versions of one crate: "two different versions (e.g. 0.6.5 and 0.7.3) will be resolved and built" where only incompatible versions satisfy. "The resolver will attempt to find a graph where there is only one instance of each `links` name." Yanked: "it will ignore all yanked releases unless they already exist in the `Cargo.lock` file". Requirements: "Caret requirements are the default"; "Versions are considered compatible if their left-most non-zero major/minor/patch component is the same." | https://doc.rust-lang.org/cargo/reference/resolver.html and specifying-dependencies above, read 2026-11-07 |
| 5. sources together | "Cargo is configured to look for dependencies on crates.io by default"; `registry = "my-registry"` for another; `git`; `path`; "Crates that use dependencies specified with only a path are not permitted on crates.io." The glossary's kinds: registry, local registry, directory, path and git sources. The lock records each as `registry+`, `sparse+`, `git+…#<commit>` or no `source` for a path. | specifying-dependencies, the glossary and lockfile.rs above, read 2026-11-07 |
| 6. where kept | Both. Per machine: "By default, the Cargo home is located in `$HOME/.cargo/`"; `registry/cache` holds ".crate" archives, `registry/src` the unpacked sources, `git/db` bare clones, `git/checkouts` "the required commit"; "the internal structure of the Cargo home is not stabilized". Per project, built artifacts only: "By default this is a directory named `target` at the workspace root, or the package root". Vendoring: "See the `cargo vendor` subcommand." | https://doc.rust-lang.org/cargo/guide/cargo-home.html and the glossary above, read 2026-11-07 |
| 7. runs code? | **Yes, at build.** "Placing a file named `build.rs` in the root of a package will cause Cargo to compile that script and execute it just before building the package." And procedural macros: "Procedural macros run during compilation, and thus have the same resources that the compiler has." "procedural macros have the same security concerns that Cargo's build scripts have." The manifest itself is TOML (Table 4). The build-script page says nothing on security (searched there). | https://doc.rust-lang.org/cargo/reference/build-scripts.html and https://doc.rust-lang.org/reference/procedural-macros.html, read 2026-11-07 |
| 8. commands | `cargo add`: "This command can add or modify dependencies", sources `crate@version`, `--path`, `--git`; with none given, "Latest release in the registry"; `--rename`. `cargo update` "re-calculates dependencies" (the guide). Verification flags: `--locked` "Asserts that the exact same dependencies and versions are used as when the existing `Cargo.lock` file was originally generated"; `--offline`; `--frozen` "Equivalent to specifying both `--locked` and `--offline`." | https://doc.rust-lang.org/cargo/commands/cargo-add.html, https://doc.rust-lang.org/cargo/commands/cargo-build.html, the lock guide above, read 2026-11-07 |
| 8. errors | Archive mismatch: "failed to verify the checksum of `{}`" (above). Lock mismatch, as Cargo's test suite expects it: "[ERROR] checksum for `bar v0.1.0` changed between lock files / this could be indicative of a few possible errors: / * the lock file is corrupt / * a replacement source in use (e.g., a mirror) returned a different checksum / * the source itself may be corrupt in one way or another / unable to verify that `bar v0.1.0` is the same as when the lockfile was generated". Missing dependency: "[ERROR] no matching package named `nonexistent` found / location searched: `dummy-registry` index (which is replacing registry `crates-io`) / required by package `foo v0.0.1 ([ROOT]/foo)`". None of the three names a command that fixes it. | https://rust.googlesource.com/cargo/+/HEAD/tests/testsuite/lockfile_compat.rs and https://rust.googlesource.com/cargo/+/HEAD/tests/testsuite/registry.rs (test `nonexistent_http`), read 2026-11-07 |
| 9. privacy | In the language, not the package system: "By default, everything is private"; "`pub(crate)` makes an item visible within the current crate."; "If an item is private, it may be accessed by the current module and its descendants." A directory rule like Go's `internal/`: `not found` in the Cargo manifest reference or the Reference's visibility chapter (searched both). | https://doc.rust-lang.org/reference/visibility-and-privacy.html, read 2026-11-07 |
| 10. history | See the timeline below the table. | (each row its own) |

**Cargo's history, from its first way of getting a dependency to today's**:

- **Rust 0.10, "3 April, 2014"**: "`rustpkg` has been deprecated and removed
  from the main repository. Its replacement, `cargo`, is under development."
  (https://releases.rs/docs/0.10.0, read 2026-11-07; the same line on
  https://github.com/rust-lang/rust/releases/tag/0.10, read 2026-11-07, whose
  date shows no year).
- **2014-11-20**, *Cargo: Rust's community crate host*, Alex Crichton:
  "Cargo is a package manager for Rust, in Rust." "Today it is my pleasure to
  announce that crates.io is online and ready for action." Before it, "users
  of Cargo have largely just downloaded dependencies directly from the source
  GitHub repository." "Cargo generates a Cargo.lock to encode this
  information." (https://blog.rust-lang.org/2014/11/20/Cargo/, read 2026-11-07).
- **Rust 1.0, May 15, 2015**: "Cargo, the Rust package manager and build
  system, is designed to make this easy." "crates.io is a central package
  repository for Rust code." (https://blog.rust-lang.org/2015/05/15/Rust-1.0/,
  read 2026-11-07).
- **Rust 1.31, Dec. 6, 2018**: the `edition` key: "A new key has been added
  under `[package]`: `edition`." "You can also set it to `2015`, which is the
  default if the key does not exist."
  (https://blog.rust-lang.org/2018/12/06/Rust-1.31-and-rust-2018/, read
  2026-11-07).
- **Rust 1.56, October 21, 2021**: the 2021 edition; `rust-version`, "Cargo
  will exit with an early error if that is not satisfied"; "Default Cargo
  feature resolver is now version 2."
  (https://blog.rust-lang.org/2021/10/21/Rust-1.56.0/, read 2026-11-07).
- **Rust 1.62, June 30, 2022**: "You can now add new dependencies directly
  from the command line using `cargo add`."
  (https://blog.rust-lang.org/2022/06/30/Rust-1.62.0/, read 2026-11-07).
- **Rust 1.68, Mar. 9, 2023**: "Cargo's "sparse" registry protocol has been
  stabilized for reading the index of crates"
  (https://blog.rust-lang.org/2023/03/09/Rust-1.68.0/, read 2026-11-07).
- **Rust 1.70, June 1, 2023**: "Cargo's "sparse" protocol is now enabled by
  default for reading the index from crates.io." "this also changes the path
  to the crate cache, so dependencies will be downloaded anew."
  (https://blog.rust-lang.org/2023/06/01/Rust-1.70.0/, read 2026-11-07).
- **Aug. 29, 2023**, *Change in Guidance on Committing Lockfiles*: "For years,
  the Cargo team has encouraged Rust developers to commit their `Cargo.lock`
  file for packages with binaries but not libraries." "We now recommend
  people do what is best for their project."
  (https://blog.rust-lang.org/2023/08/29/committing-lockfiles/, read
  2026-11-07).
- **Cargo 1.78.0 / 1.83.0**: lock format version 4 stabilized, then the
  default (pull request 14595, merged 2024-09-25,
  https://github.com/rust-lang/cargo/pull/14595, read 2026-11-07).
- **Rust 1.84, Jan. 9, 2025**: "1.84.0 stabilizes the minimum supported Rust
  version (MSRV) aware resolver"; "The new resolver will be enabled by default
  for projects using the 2024 edition"
  (https://blog.rust-lang.org/2025/01/09/Rust-1.84.0/, read 2026-11-07).
- **Rust 1.85, Feb. 20, 2025**: "We are excited to announce that the Rust 2024
  Edition is now stable!" (https://blog.rust-lang.org/2025/02/20/Rust-1.85.0/,
  read 2026-11-07).
- **July 11, 2025**, crates.io: "We are excited to announce that we have
  implemented "Trusted Publishing" support on crates.io"
  (https://blog.rust-lang.org/2025/07/11/crates-io-development-update-2025-07,
  read 2026-11-07).
- **Rust 1.99.0, Oct. 1, 2026**: the newest; the manifest reference of that
  day lists `[hints]`, "Respected as of 1.90"
  (https://doc.rust-lang.org/cargo/reference/manifest.html, read 2026-11-07).

### 2.3 Dart and pub

The release: `not found` (the pages read state no current SDK release; the
hooks page says hooks were "introduced in Dart 3.10").

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | "For built-in libraries, the URI has the special `dart:` scheme." "For other libraries, you can use a file system path or the `package:` scheme." "The `package:` scheme specifies libraries provided by a package manager such as the pub tool." A local name: "If you import two libraries that have conflicting identifiers, then you can specify a prefix for one or both libraries", `import 'package:lib2/lib2.dart' as lib2;`. | https://dart.dev/language/libraries, read 2026-11-07 |
| 1. own code against fetched; collisions | `dart pub get` writes "`.dart_tool/package_config.json` file mapping _package names_ to directories", which tools use to resolve `package:` imports. Two packages of one name: the solver keeps "at most one package version with a given name". A dependency against a local directory of the same name: `not found` (searched the libraries, dependencies and package-layout pages). | https://dart.dev/tools/pub/package-layout and https://github.com/dart-lang/pub/blob/master/doc/solver.md, read 2026-11-07 |
| 2. where written | `pubspec.yaml` holds the requirement and its source (hosted, `git` with `ref`/`path`/`tag_pattern`, `path`, `sdk`); `pubspec.lock` is separate: "A file named pubspec.lock that specifies the versions of each dependency." The fingerprint: "Compute a hash of each downloaded archive"; "When writing a pubspec.lock file, the sha256 is included in the description of each hosted package." (pub commit *Content hashing of archives (#3482)*, Sigurd Meldgaard, Oct 18 2022). The glossary: "SHA256 hashes maintained by pub.dev to validate package integrity." For a git source, what the lock records: `not found` on the pages read. | https://dart.dev/tools/pub/dependencies, https://dart.dev/tools/pub/glossary, https://dart.googlesource.com/pub.git/+/4c9ebd0e, read 2026-11-07 |
| 3. words | "Package: A directory with a collection of Dart libraries, resources, and a pubspec.yaml file describing them." "Library: A single compilation unit in Dart, made up of a primary Dart file and its parts." "Lockfile: A file named pubspec.lock". "Source: A kind of place that pub can get packages from." | https://dart.dev/tools/pub/glossary, read 2026-11-07 |
| 4. selection | A solver: "Pub's version solving algorithm, called Pubgrub, solves these issues by adapting state-of-the-art techniques for solving Boolean satisfiability"; "only one version of each package is selected." Constraints are ranges: "`^version` means the range of all versions guaranteed to be backwards compatible with the given version"; "If no version constraint is given, `any` is assumed." | https://github.com/dart-lang/pub/blob/master/doc/solver.md and https://dart.dev/tools/pub/dependencies, read 2026-11-07 |
| 5. sources together | Hosted, git, path and SDK sources in one pubspec; "you cannot upload a package to the pub.dev site if it has any path dependencies in its pubspec." "Git dependencies are not allowed as dependencies for packages uploaded to pub.dev." `dependency_overrides`: "Only the dependency overrides in a package's own pubspec are considered during package resolution." | https://dart.dev/tools/pub/dependencies, read 2026-11-07 |
| 6. where kept | A machine cache, by default "located in the `.pub-cache` subdirectory of your home directory", movable with `PUB_CACHE`; the project holds only `.dart_tool/package_config.json`, and `.dart_tool/` is not to be checked into source control. | https://dart.dev/tools/pub/cmd/pub-get and https://dart.dev/tools/pub/package-layout, read 2026-11-07 |
| 7. runs code? | **Yes, at build, since hooks.** "A package's build hook is automatically invoked by the Dart SDK at an appropriate time during the build process." "The hooks are run automatically when invoking the `run`, `build`, or `test` commands." Hooks "will be run by packages and applications depending on your package." Support "was introduced in Dart 3.10". The pubspec is YAML (Table 4). | https://dart.dev/tools/hooks, read 2026-11-07 |
| 8. commands | `dart pub add` "adds the specified packages to the `pubspec.yaml` as dependencies", by default "the latest stable version of the package from the pub.dev site that is compatible with your SDK constraints"; `dart pub get`, `upgrade`, `downgrade`; `--enforce-lockfile` fails "if any content hash of a hosted package has changed". | https://dart.dev/tools/pub/cmd/pub-add and https://dart.dev/tools/pub/cmd/pub-get, read 2026-11-07 |
| 8. errors | Hash mismatch under `--enforce-lockfile`, as pub's tests expect: "Cached version of foo-1.0.0 has wrong hash - redownloading." "The existing content-hash from pubspec.lock doesn't match contents for:" " * foo-1.0.0 from "${server.url}"" "Unable to satisfy `pubspec.yaml` using `pubspec.lock`." No lock: "Cannot do `--enforce-lockfile` without an existing `pubspec.lock`." "Try running `dart pub get` to create `pubspec.lock`." Missing package: "Because myapp depends on foo any which doesn't exist (could not find package foo at http://localhost:…), version solving failed." The no-lock error names its fix; the other two do not name a command. | https://dart.googlesource.com/pub.git/+/21eb39e1/test/get/enforce_lockfile_test.dart and https://dart.googlesource.com/pub.git/+/c4e9ddc8/test/hosted/fail_gracefully_on_missing_package_test.dart (both test files at the commits named), read 2026-11-07 |
| 9. privacy | A convention on a directory: `lib/src` files "are not part of the package's public API"; "never import from another package's `lib/src` directory." Whether a tool refuses such an import: `not found` on that page. In the language: "identifiers that start with an underscore (`_`) are visible only inside the library." | https://dart.dev/tools/pub/package-layout and https://dart.dev/language/libraries, read 2026-11-07 |
| 10. history | `environment` "Required as of Dart 2"; "Omitting the SDK constraint is an error." The SDK constraint is also a language version: "Each package has a default language version equal to the lower bound of the SDK constraint in the pubspec.yaml file", overridable per file by `// @dart = <major>.<minor>`. A YAML number in `version`: a 2014 change, "Printing a useful error message if a version field is a num" (Table 4). Content hashes in the lock: the commit of Oct 18 2022 above. Hooks: Dart 3.10. The date PubGrub entered pub: `not found` (solver.md gives none; the author's Medium post, https://nex3.medium.com/pubgrub-2fb6470504f, answered 403 and is not cited). | https://dart.dev/tools/pub/pubspec, https://dart.dev/language/versioning, https://codereview.chromium.org/632483002, read 2026-11-07 |

**`pubspec.yaml` declares a language version.** Its `environment` SDK
constraint is required and its lower bound is the package's language version
(the two pages above); this is the same question the brief raises for Go's
`go` line and Cargo's `edition`.

### 2.4 Zig (`build.zig.zon`; Zig 0.17.1, 2026-11-02)

The newest release is "Latest Stable Version (0.17.1)", 2026-11-02
(https://ziglang.org/download/, read 2026-11-07); 0.17.0 is dated 2026-10-01
(https://ziglang.org/download/index.json, read 2026-11-07).

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | "`target` is either a relative path to another file from the file containing the `@import` call, or it is the name of a module, with the import referring to the root source file of that module." The file path must end in "`.zig` (for a Zig source file) or `.zon` (for a ZON data file)". Always available: `@import("std")`, `@import("builtin")`, `@import("root")`. The manifest's `name`: "This is the default name used by packages depending on this one." How the importing build gives a module its name in code: `not found` on the build-system page (it shows `addAnonymousImport` only) or the read parts of the language reference. | https://ziglang.org/documentation/master/ (offset 300000), https://raw.githubusercontent.com/ziglang/zig/master/doc/build.zig.zon.md, https://ziglang.org/learn/build-system/, read 2026-11-07 |
| 1. own code against fetched; collisions | A file is a path ending `.zig`; a module is a bare name. Two dependencies or a dependency and a directory wanting one name: `not found` on the pages read. | as above, read 2026-11-07 |
| 2. where written | **Beside the requirement, in the one file**: "Each dependency must either provide a `url` and `hash`, or a `path`." The hash "is computed from the file contents of the directory of files that is obtained after fetching `url`", "Only files included in the package are used to compute a package's `hash`", and "This field is the source of truth; packages do not come from a `url`; they come from a `hash`." 0.11: "The hash is not of the archive itself, but of its contents." 0.14's format: `mime-3.0.0-zwmL-6wgAADuFwn7gr-_DAQDGJdIim94aDIPa6qO-6GT`, name, version, the fingerprint's id, the unpacked size and "200 bits of SHA-256". A `path` dependency has no hash. No lock file is named on these pages. | https://raw.githubusercontent.com/ziglang/zig/master/doc/build.zig.zon.md, https://ziglang.org/download/0.11.0/release-notes.html (offset 100000), https://ziglang.org/download/0.14.0/release-notes.html, read 2026-11-07 |
| 3. words | Package ("the set of files and directories that are included in this package", the `paths` field), module (the `@import` sentence above), and a source file, which `@import` returns as "that file's corresponding struct type". The file: `build.zig.zon`. | the zon document and the language reference above, read 2026-11-07 |
| 4. selection | Exact pins by hash; no range syntax on the zon page. 0.14 says the new hash would let tooling perform version selection without fetching (as the reader rendered it). Two versions of one package: `not found`. | the 0.14.0 notes and zon document above, read 2026-11-07 |
| 5. sources together | `url` with `hash`, or `path`; `lazy` "a package is declared to be lazily fetched". "There is no "official" package repository: packages are simply arbitrary directory trees." (0.11). 0.16: `zig build --fork=[path]` overrides a package locally. | the zon document, the 0.11.0 notes and https://ziglang.org/download/0.16.0/release-notes.html (offset 100000), read 2026-11-07 |
| 6. where kept | Both, since 0.16: packages go into a `zig-pkg` directory beside `build.zig` ("Users are generally encouraged to not commit these files to source control"), and the global cache keeps "a canonical `$GLOBAL_ZIG_CACHE/p/$HASH.tar.gz`". 0.17: "`zig build` always fetches locally (in addition to globally)." | the 0.16.0 notes (offset 100000) and https://ziglang.org/download/0.17.0/release-notes.html, read 2026-11-07 |
| 7. runs code? | **Yes, at build**: the build is a program; "This part of the help menu is auto-generated based on running the `build.zig` logic." The manifest itself is ZON, "a ZON data file". Whether a dependency's own `build.zig` runs in the consumer's build: `not found` stated on the pages read. | https://ziglang.org/learn/build-system/ and the language reference above, read 2026-11-07 |
| 8. commands, errors | `zig fetch`, "if `--save` (or any variant) is used, then it also fetches into the local package path"; `zig build` fetches. Missing hash: "`zig build` will emit an error containing the expected hash" (0.11). Mismatch, as pasted in a pull request of the project: "error: hash mismatch: manifest declares 12209a8b…" followed by "but the fetched package has" and the computed hash. Neither names a command; the first gives the value to write. A dependency name missing from the manifest: `not found` in a primary source (the search found user reports only). | the 0.17.0 and 0.11.0 notes above, https://github.com/ziglang/zig/pull/21589, read 2026-11-07 |
| 9. privacy | `not found`: the sentence on `pub` visibility across files was searched for in the language reference's text at offsets 100000 to 400000 and not reached. | https://ziglang.org/documentation/master/, read 2026-11-07 |
| 10. history | **0.11.0, 2023-08-04**: "Zig 0.11 is the début of the official package manager." "The package manager is in its early stages, and will likely undergo significant changes before 1.0." **0.14.0, 2025-03-05**: the `fingerprint` field and the new hash format, which "provides a better user experience when package hashes show up in compile errors or file paths". **0.16.0, 2026-04-13**: project-local `zig-pkg`, "to make it easier to tinker". **0.17.0, 2026-10-01**: "All package management functionality has been moved out of the Compiler and into the Build System." | the release notes above and https://ziglang.org/download/index.json, read 2026-11-07 |

**`build.zig.zon` field inventory** (https://raw.githubusercontent.com/ziglang/zig/master/doc/build.zig.zon.md, read 2026-11-07):
`name` (required, "a valid bare Zig identifier", 32 bytes), `fingerprint`
("Together with `name`, this represents a globally unique package
identifier"), `version` (required), `minimum_zig_version` (optional, **a
toolchain version**: "This is currently advisory only; the compiler does not
yet do anything with this version."), `dependencies` (`url`, `hash`, `path`,
`lazy`), `paths` (required, "the set of files and directories that are
included in this package").

### 2.5 Deno (Deno 2.9.8, 2026-10-21)

The newest release is v2.9.8, "2.9.8 / 2026.10.21"
(https://github.com/denoland/deno/releases, read 2026-11-07).

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | Third-party specifiers "start with `jsr:` or `npm:`"; "You can centralize management of remote modules with an `imports` field in your `deno.json` file", mapping `"@luca/cases"` to `"jsr:@luca/cases@^1.0.0"`; "The remapped name can be any valid specifier." Own code: "local import specifiers must always include the full file extension." | https://docs.deno.com/runtime/fundamentals/modules/, read 2026-11-07 |
| 1. collisions | `scopes` "lets you override import mappings for modules loaded from a particular path prefix"; `links` replaces dependencies with local packages on disk (the page's field table, as the reader summarised it). What happens when two mappings claim one name: `not found`. | https://docs.deno.com/runtime/reference/deno_json/, read 2026-11-07 |
| 2. where written | The name and range in `deno.json`'s `imports` (or `package.json`); Deno "automatically maintains a `deno.lock` file recording the exact version and integrity hash of every dependency", a separate file. Over which bytes, for each source kind: `not found` on the packages guide, the modules page and the `deno.json` reference; the `deno_lockfile` repository's README carries no format ("This repo was moved into https://github.com/denoland/deno"). | https://docs.deno.com/runtime/packages/ and https://github.com/denoland/deno_lockfile, read 2026-11-07 |
| 3. words | "Deno uses ECMAScript modules as its primary module system"; the file is a "`deno.json` (or `deno.jsonc`) configuration file"; `exports` defines "the package's public entry points". | the modules page and the `deno.json` reference above, read 2026-11-07 |
| 4. selection | Ranges in specifiers (`^1.0.0`); `deno update` moves to newer semver-compatible versions. The algorithm, and two versions of one package: `not found` on the pages read. | https://docs.deno.com/runtime/packages/, read 2026-11-07 |
| 5. sources together | `jsr:`, `npm:` and `https:` in one program; the guide recommends JSR and npm for applications, HTTPS imports not being managed by `deno add` or `deno install`; private hosts through `DENO_AUTH_TOKENS`. | https://docs.deno.com/runtime/packages/, read 2026-11-07 |
| 6. where kept | "By default, Deno uses a global cache directory (`DENO_DIR`) for downloaded dependencies"; a local `node_modules` only when the project has a `package.json` (controlled by `nodeModulesDir`); `"vendor": true` caches into a local `vendor` directory. | https://docs.deno.com/runtime/packages/, read 2026-11-07 |
| 7. runs code? | Deno does not run npm `preinstall`/`postinstall` scripts by default; they run with `deno install --allow-scripts=npm:<package>` or `deno approve-scripts` (as the reader rendered the page). The configuration file is JSON or JSONC. | https://docs.deno.com/runtime/packages/ and https://docs.deno.com/runtime/reference/deno_json/, read 2026-11-07 |
| 8. commands, errors | `deno add`, `deno remove`, `deno install`, `deno outdated`, `deno update`, `deno ci` ("errors if `deno.lock` is missing or out of date"), `--frozen`. The integrity error as Deno 1.28.1's source wrote it: "Integrity check failed for npm package: "{}". Unable to verify that the package is the same as when the lockfile was generated. This could be caused by: * the lock file may be corrupt * the source itself may be corrupt Use "--lock-write" flag to regenerate the lockfile at "{}"." It names a flag. The current release's text: `not found` (the 1.28.1 source is from 2022; a forum thread with a newer report answered 429). | https://docs.deno.com/runtime/packages/ and https://docs.rs/crate/deno/1.28.1/source/lockfile.rs, read 2026-11-07 |
| 9. privacy | `exports` defines the package's public entry points. Whether an import past them is refused: `not found`. | https://docs.deno.com/runtime/reference/deno_json/, read 2026-11-07 |
| 10. history | **Deno 1.0, May 13, 2020**: dependencies by URL, `import { serve } from "https://deno.land/std@0.50.0/http/server.ts";`; "There are no additional configuration files, there is no install to do beforehand"; of Node, "the mechanism for linking to external libraries is fundamentally centralized through the NPM repository." **Deno 2.0, October 9, 2024**: "Native support for `package.json` and `node_modules`"; `deno install`, `deno add`, `deno remove`; JSR, "a modern registry for sharing JavaScript libraries across runtimes". | https://deno.com/blog/v1 and https://deno.com/blog/v2.0, read 2026-11-07 |

### 2.6 Node and npm (Node.js v26.13.0 documentation; npm CLI v11 documentation)

The Node documentation read is headed "Node.js v26.13.0 documentation"
(https://nodejs.org/api/modules.html, read 2026-11-07); npm's pages are those
of the CLI's v11 documentation (URLs below).

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | A bare specifier: "If the module identifier passed to `require()` is not a built-in module, and does not begin with `'/'`, `'../'`, or `'./'`, then Node.js starts at the directory of the current module, and adds `/node_modules`", then each parent "until the root of the file system is reached." A local name: `npm install my-react@npm:react`, "Install a package under a custom alias. Allows multiple versions of a same-name package side-by-side"; "Aliasing works only on your project and does not rename packages in transitive dependencies." Inside a package, `imports` creates "private mappings that only apply to import specifiers from within the package itself", entries that "must always start with `#` to ensure they are disambiguated from external package specifiers", and "the `"imports"` field permits mapping to external packages." | https://nodejs.org/api/modules.html, https://nodejs.org/api/packages.html, https://docs.npmjs.com/cli/v11/commands/npm-install, read 2026-11-07 |
| 1. own code against fetched; collisions | "Without a leading `'/'`, `'./'`, or `'../'` to indicate a file, the module must either be a core module or is loaded from a `node_modules` folder." A built-in against a file: "`require('http')` will always return the built-in HTTP module, even if there is a file by that name." New built-ins take the prefix: "This requirement exists to prevent newly introduced built-in modules from having a conflict with user land packages that already have taken the name." | https://nodejs.org/api/modules.html, read 2026-11-07 |
| 2. where written | Ranges in `package.json`; `package-lock.json` separate, describing "the exact tree that was generated". Per package: "integrity: A sha512 or sha1 Standard Subresource Integrity string for the artifact that was unpacked in this location."; `resolved`: "The place where the package was actually resolved from." For git, `resolved` is "the full git url with commit sha" (as the reader rendered it). | https://docs.npmjs.com/cli/v11/configuring-npm/package-lock-json (read twice), read 2026-11-07 |
| 3. words | `package.json` and `package-lock.json`; Node's documentation calls a file a module ("Loading from `node_modules` folders"). A glossary definition of package or module: `not found` on the pages read. | https://nodejs.org/api/modules.html and the npm pages above, read 2026-11-07 |
| 4. selection | Ranges: `^version` "Compatible with version", `~version` "Approximately equivalent to version". Two versions: **yes**, "Install non-duplicated in top-level, and duplicated as necessary within directory structure"; "C now has to install D@2 privately for itself." The lock against the manifest: "When the lockfile's versions satisfy the `package.json` ranges, the lockfile wins." | https://docs.npmjs.com/cli/v11/configuring-npm/package-json and https://docs.npmjs.com/cli/v11/commands/npm-install, read 2026-11-07 |
| 5. sources together | One `dependencies` table takes a version, a range, "a tarball URL in place of a version range", a git URL such as `git+ssh://git@github.com:npm/cli.git#v1.0.27`, a GitHub `user/repo`, or `file:../dyl`. The lock's `resolved` records where each came from. | the package.json page and the package-lock page above, read 2026-11-07 |
| 6. where kept | In the project, `node_modules` (above). A machine cache: `not found` on the pages read (npm-install, package-lock, package.json). | the npm-install page above, read 2026-11-07 |
| 7. runs code? | **Yes, at install.** `npm install` runs `preinstall`, `install`, `postinstall`, `prepublish`, `preprepare`, `prepare`, `postprepare`; "npm will default the `install` command to compile using node-gyp via `node-gyp rebuild`" when `binding.gyp` exists. "The only valid use of `install` or `preinstall` scripts is for compilation which must be done on the target architecture." Opt-out: `--ignore-scripts`, "If true, npm does not run scripts specified in package.json files." | https://docs.npmjs.com/cli/v11/using-npm/scripts and https://docs.npmjs.com/cli/v11/commands/npm-install, read 2026-11-07 |
| 8. commands, errors | `npm install` (and `npm install <alias>@npm:<name>`). Integrity mismatch, from `ssri`, error code `EINTEGRITY`: "Integrity checksum failed when using ${algorithm}: Wanted ${sri}, but got ${newSri}. (${data.length} bytes)". Missing module at run time, `ERR_MODULE_NOT_FOUND`: "A module file could not be resolved by the ECMAScript modules loader while attempting an `import` operation or when loading the program entry point." Neither names a fix. | https://raw.githubusercontent.com/npm/ssri/main/lib/index.js and https://nodejs.org/api/errors.html (offset 100000), read 2026-11-07 |
| 9. privacy | `exports`: "When the `"exports"` field is defined, all subpaths of the package are encapsulated and no longer available to importers." `ERR_PACKAGE_PATH_NOT_EXPORTED`: "Because exports are encapsulated, private internal modules that are not exported cannot be imported through the package resolution, unless using an absolute URL." | https://nodejs.org/api/packages.html and https://nodejs.org/api/errors.html (offset 100000), read 2026-11-07 |
| 10. history | Lock formats: `lockfileVersion` 1 "used by npm v5 and v6", 2 "used by npm v7 and v8", 3 "used by npm v9 and above"; no version marks an "ancient" shrinkwrap from before npm v5 (as the reader rendered it). **March 23rd, 2016**, *kik, left-pad, and npm* (@izs): "Azer unpublished his `kik` package and 272 other packages." "One of those was left-pad." "We conferred with Cameron and took the unprecedented step of re-publishing the original `0.0.3`." "We will make it harder to un-publish a version of a package if doing so would break other packages." The `node:` prefix made mandatory for new built-ins (row 1). | the package-lock page above and https://blog.npmjs.org/post/141577284765/kik-left-pad-and-npm, read 2026-11-07 |

**`package.json` declares a toolchain version, advisory**: `engines`
"Declares supported Node.js and npm versions"; unless `engine-strict` is set
it "is advisory only and will only produce warnings"
(https://docs.npmjs.com/cli/v11/configuring-npm/package-json, read 2026-11-07).

### 2.7 Swift Package Manager (Swift 6.4.1, October 12, 2026)

The install page names "Install (6.4.2)" without a date and dates Swift 6.4.1
"October 12, 2026" (https://www.swift.org/install/macos/, read 2026-11-07).
The SwiftPM documentation read is the DocC catalog in the repository's `main`
branch, `Sources/PackageManagerDocs/Documentation.docc/`; the
docs.swift.org address of its index answered 404.

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | Code imports modules: "When you import a library in Swift, you're importing the modules that make up that library." A local name, for a collision: "As you add dependencies to your package, a name collision can occur among modules from different packages"; the package manager "provides the parameter `moduleAliases` when defining dependencies for targets", e.g. `moduleAliases: ["Utils": "MyPackageUtils"]`, "from Swift 5.7 or later". A mark telling own modules from fetched in the import line: `not found` on the pages read. | https://raw.githubusercontent.com/swiftlang/swift-package-manager/main/Sources/PackageManagerDocs/Documentation.docc/IntroducingPackages.md and …/ModuleAliasing.md, read 2026-11-07 |
| 2. where written | Dependencies in `Package.swift`; `Package.resolved` holds the chosen versions; its fields: `not found` on the resolution page. Fingerprints are kept apart, per machine: "Git hash of the revision" for a git package, "Checksum of the source archive" for a registry package, under `~/.swiftpm/security/fingerprints`; trust on first use. | …/ResolvingPackageVersions.md and …/PackageSecurity.md, read 2026-11-07 |
| 3. words | "A package consists of a `Package.swift` manifest file along with source files, resources, and other assets." "Each package declares Products". "Each product is made up of one or more Targets". "A module specifies a namespace and enforces access controls"; "Each target you define in a Swift package is a module." | …/IntroducingPackages.md, read 2026-11-07 |
| 4. selection | The resolver is PubGrub by its source's names: `Sources/PackageGraph/Resolution/PubGrub/` holds `PubGrubDependencyResolver.swift` among others. The resolution page names no algorithm and does not state one version per package. "the `resolve` command resolves to those versions as long as they are still eligible." | https://github.com/swiftlang/swift-package-manager/tree/main/Sources/PackageGraph/Resolution/PubGrub and …/ResolvingPackageVersions.md, read 2026-11-07 |
| 5. sources together | Source control and registries (the fingerprint page). Local folders: `not found` on the pages read. | …/PackageSecurity.md, read 2026-11-07 |
| 6. where kept | `not found` on the pages read (introduction, resolution, security, plugins). | as above, read 2026-11-07 |
| 7. runs code? | **Yes, reading the manifest runs it**: "SwiftPM's manifest is a Swift program (a script of sorts) which SwiftPM builds and executes as a separate process"; "The security sandbox limits access to network and file system." (tomerd, Swift Forums, November 12, 2021.) Plugins: "Custom build tool tasks that provide commands to run before or during the build"; "On platforms that support sandboxing, package manager wraps the plugin in a sandbox that prevents network access and attempts to write to arbitrary locations in the file system." | https://forums.swift.org/t/pre-pitch-swiftpm-manifest-based-on-result-builders/53457 and …/Plugins.md, read 2026-11-07 |
| 8. commands, errors | `swift package resolve`, `--force-resolved-versions`, and the update command, which rewrites `Package.resolved`; a fingerprint conflict "reports an error", downgradable with `--resolver-fingerprint-checking` set to `warn`. The error's text: `not found` on the security page. | …/ResolvingPackageVersions.md and …/PackageSecurity.md, read 2026-11-07 |
| 9. privacy | Modules enforce access control; the `package` modifier "allows symbols to be accessed from outside of their defining module, but only from other modules in the same package." Status: "Implemented (Swift 5.9)". | https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0386-package-access-modifier.md, read 2026-11-07 |
| 10. history | **September 13, 2016**: Swift 3.0 brought "the first official release of the Swift Package Manager." 5.7: module aliasing. 5.9: `package`. Since 6.0 the tools-version comment may follow leading comments (below). | https://www.swift.org/blog/swift-3.0-released/ and the pages above, read 2026-11-07 |

**`Package.swift` declares a toolchain version, and it is required**: "A
`Package.swift` manifest file must begin with the string `//
swift-tools-version:` followed by a version number specifier"; "The Swift
tools version declares the version of the `PackageDescription` library, the
minimum version of the Swift tools and Swift language compatibility version
to process the manifest" (https://docs.swift.org/package-manager/PackageDescription/PackageDescription.html,
read 2026-11-07, a page showing the older `swiftLanguageVersions` parameter).
The DocC page: the comment "declares the minimum version of the Swift
compiler required"; it must be on the first line for tools versions before
6.0; a dependency whose tools version is newer than the one in use is
ineligible and "dependency resolution continues with evaluating the
next-best version" (…/SettingSwiftToolsVersion.md, read 2026-11-07).

### 2.8 Gleam (Gleam v1.19.2, 12 Oct 2026)

The newest release is v1.19.2, marked "Latest", 12 Oct 2026
(https://github.com/gleam-lang/gleam/releases, read 2026-11-07); the
compiler's `Cargo.toml` reads `version = "1.19.2"`
(https://raw.githubusercontent.com/gleam-lang/gleam/main/compiler-core/Cargo.toml,
read 2026-11-07).

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | A module's name "comes from the name of the file it's in"; `gleam/io` is "a file called `io.gleam` in a directory called `gleam`"; once imported, "the name used to refer to it is the last part of the module name"; "The `as` keyword can be used to refer to a module by a different name." A mark telling the program's modules from a dependency's: `not found`; a collision of two packages' module names: `not found` in Gleam's pages (searched the tour and a web search for a duplicate-module error); Hex's publishing guide says of its ecosystem that two packages defining the same module name cannot be used together (as the reader rendered https://hex.pm/docs/publish, read 2026-11-07). | https://tour.gleam.run/basics/modules/, read 2026-11-07 |
| 2. where written | `gleam.toml` holds requirements ("The version requirement string uses the Hex requirement format"; `path`; `git` and `ref`); `manifest.toml` is separate and "locks all the dependency packages to specific versions". Its schema: a Hex package carries `outer_checksum`; a git one `repo`, `commit`, `path`; a local one `path`. Hex's outer checksum is "the SHA-256 checksum of the entirety of the tarball file" (Table 3). | https://gleam.run/documentation/gleam-toml-reference, https://gleam.run/writing-gleam/, https://raw.githubusercontent.com/gleam-lang/gleam/main/compiler-core/src/manifest.rs, read 2026-11-07 |
| 3. words | Module (a file); package; `gleam.toml`; `manifest.toml`. | the pages above, read 2026-11-07 |
| 4. selection | The compiler depends on `pubgrub = "0.3"`; `gleam deps update`: "Upgrade the dependencies to the newest available versions that are compatible with the requirements." One version per package: `not found`. | the compiler's Cargo.toml above and https://gleam.run/documentation/command-line-reference/, read 2026-11-07 |
| 5. sources together | Hex, `path` and `git`; "Always prefer a commit sha reference over a branch or tag reference"; the manifest tags each package `hex`, `git` or `local`. | the gleam.toml reference and manifest.rs above, read 2026-11-07 |
| 6. where kept | Both: the project's `build/packages` (`self.build_directory().join("packages")`) and a machine cache under the user cache directory, `gleam/hex/hexpm/packages/<checksum>.tar`. | https://raw.githubusercontent.com/gleam-lang/gleam/main/compiler-core/src/paths.rs, read 2026-11-07 |
| 7. runs code? | `not found`: whether building an Erlang or Elixir dependency runs its build tool is not stated on the guide or command reference (searched for rebar3 and mix); the manifest's package entry has a `build_tools` field. The manifest is TOML. | the pages above, read 2026-11-07 |
| 8. commands, errors | `gleam add` "Add new dependencies to the package."; `gleam remove`; `gleam deps download` (writes `manifest.toml` "to lock the used versions"); `gleam deps list`; `gleam deps update`; `gleam update`. The checksum-mismatch and missing-package texts: `not found` on the command reference. | https://gleam.run/documentation/command-line-reference/, read 2026-11-07 |
| 9. privacy | `internal_modules`: "An internal module is a module that is not part of the public API of the package." Whether a consumer's import of one is refused: `not found` on that page. | https://gleam.run/documentation/gleam-toml-reference, read 2026-11-07 |
| 10. history | CVE-2026-59247, *Insufficient verification of Hex package metadata in Gleam*, published 2026-07-29, affecting ≥ 0.18.0 < 1.18.0: Gleam "records the outer_checksum and dependency names from that JSON response into manifest.toml", and "The Hex repository signature does not cover the API response." Builds reusing an unchanged `manifest.toml` kept checking its pinned checksum. | https://cna.erlef.org/cves/CVE-2026-59247.html, read 2026-11-07 |

**`gleam.toml` declares a toolchain version**: `gleam`, "The required Gleam
compiler version for the package", and "An error is raised if the compiler
version used to compile the package does not satisfy this requirement"
(https://gleam.run/documentation/gleam-toml-reference, read 2026-11-07).

### 2.9 Python (`requirements.txt`, `pyproject.toml`, uv's `uv.lock`, `pylock.toml`)

The documentation read: "Python 3.15.2" (https://docs.python.org/3/reference/import.html,
read 2026-11-07, "last updated Nov 07, 2026"); "pip documentation v26.3"
(https://pip.pypa.io/en/stable/reference/requirements-file-format/, read
2026-11-07); uv's documentation at docs.astral.sh, which names no release on
the pages read.

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | An import names a module by its own name, not by its distribution's: "Absolute imports may use either the `import <>` or `from <> import <>` syntax". A local name: `from .moduleY import spam as ham`. Own code against fetched: "Relative imports use leading dots. A single leading dot indicates a relative import, starting with the current package." Two claim one name: "The path based finder iterates over every entry in the search path"; a sentence saying which entry wins: `not found` on that page. `pyproject.toml` has an optional `import-names` key. | https://docs.python.org/3/reference/import.html and https://packaging.python.org/en/latest/specifications/pyproject-toml/, read 2026-11-07 |
| 2. where written | `requirements.txt`: a requirement per line, with `--hash` per requirement; "Specifying `--hash` against *any* requirement will activate this mode globally"; "Requirements must be pinned (either to a URL, filesystem path or using `==`)." `pyproject.toml`'s `dependencies` (no hash). `pylock.toml` (PEP 751): `packages.archive.hashes`, "A table listing known hash values of the file where the key is the hash algorithm and the value is the hash value", the file being the archive; `packages.vcs` and `packages.directory` for other sources. `uv.lock`: whether it holds hashes, `not found` on the layout page. | https://pip.pypa.io/en/stable/reference/requirements-file-format/, https://pip.pypa.io/en/stable/topics/secure-installs/, https://peps.python.org/pep-0751/, https://docs.astral.sh/uv/concepts/projects/layout/, read 2026-11-07 |
| 3. words | "all packages are modules, but not all modules are packages"; "any module that contains a `__path__` attribute is considered a package"; "A regular package is typically implemented as a directory containing an `__init__.py` file." "Requirements files serve as a list of items to be installed by pip". | https://docs.python.org/3/reference/import.html and the requirements-file page above, read 2026-11-07 |
| 4. selection | pip: "When pip finds that an assumption it made earlier is incorrect, it has to backtrack." "Changed in version 20.3: Pip's dependency resolver is now capable of backtracking." uv: "uv uses pubgrub-rs, the Rust implementation of PubGrub, an incremental version solver." One version: "Since Python only allows one version of each package, a naive resolver would error here"; uv's universal lock may list "a package … multiple times with different versions or URLs" for different markers. | https://pip.pypa.io/en/stable/topics/dependency-resolution/, https://docs.astral.sh/uv/reference/internals/resolver/, https://docs.astral.sh/uv/concepts/resolution/, read 2026-11-07 |
| 5. sources together | A requirements file takes requirement specifiers, "`<archive url/path>`", "`[-e] <local project path>`", "`[-e] <vcs project url>`", and `--index-url`, `--extra-index-url`; `pylock.toml` records `vcs`, `directory`, `archive`, `sdist`, `wheels` and `index` per package. | the requirements-file page and PEP 751 above, read 2026-11-07 |
| 6. where kept | uv: a machine cache, "`$XDG_CACHE_HOME/uv` or `$HOME/.cache/uv` on Unix", and the project's environment in `.venv` beside `pyproject.toml`; "uv cannot link files from the cache into the environment and must fall back to slower copy operations" across filesystems. pip's cache: `not found` on the pages read. | https://docs.astral.sh/uv/concepts/cache/ and https://docs.astral.sh/uv/concepts/projects/layout/, read 2026-11-07 |
| 7. runs code? | **Yes, by default.** pip's installs involve "running arbitrary code from distributions"; the guide's countermeasure, "Disallow source distributions, by passing" `--only-binary :all:`. Building from source: "You can't execute a `setup.py` file without knowing its dependencies" (PEP 518's rationale). Importing a regular package runs it: "this `__init__.py` file is implicitly executed". The manifest is TOML. | https://pip.pypa.io/en/stable/topics/secure-installs/, https://peps.python.org/pep-0518/, https://docs.python.org/3/reference/import.html, read 2026-11-07 |
| 8. commands, errors | `pip install -r`, `--require-hashes`; `uv lock`, `uv sync`, `uv run`, `uv export -o pylock.toml`. Hash mismatch, as posted on the Python forum, August 9, 2020: "ERROR: THESE PACKAGES DO NOT MATCH THE HASHES FROM THE REQUIREMENTS FILE." "If you have updated the package versions, please update the hashes." "Otherwise, examine the package contents carefully; someone may have tampered with them." It names what to change, not a command. pip's source for it, and the missing-package text: `not found` (pip's `exceptions.py` answered 404 at both addresses tried). | https://discuss.python.org/t/error-these-packages-do-not-match-the-hashes-from-the-requirements-file/4926 and the uv pages above, read 2026-11-07 |
| 9. privacy | A convention: `_single_leading_underscore`, "weak “internal use” indicator. E.g. `from M import *` does not import objects whose names start with an underscore." "Even with `__all__` set appropriately, internal interfaces (…) should still be prefixed with a single leading underscore." | https://peps.python.org/pep-0008/, read 2026-11-07 |
| 10. history | **PEP 518, created 10-May-2016**: build requirements in `pyproject.toml`, for the catch-22 of `setup.py`. **PEP 621, created 22-Jun-2020**: "how to write a project’s core metadata in a pyproject.toml file". **pip 20.3**: the backtracking resolver. **PEP 751, resolved 31 March 2025**, status Final: `pylock.toml`, which uv's page calls a format intended to replace `requirements.txt` (as the reader rendered it). | https://peps.python.org/pep-0518/, https://peps.python.org/pep-0621/, the pip resolution page, https://peps.python.org/pep-0751/, read 2026-11-07 |

**Version declarations**: `requires-python`, "The Python version requirements
of the project" (https://packaging.python.org/en/latest/specifications/pyproject-toml/,
read 2026-11-07); `pylock.toml` carries a required `lock-version`, `"1.0"`,
and an optional `requires-python` (https://peps.python.org/pep-0751/, read
2026-11-07).

### 2.10 Nim and Nimble (Nim 2.4.0, 28 October 2026)

"Nim version 2.4.0 released", 28 October 2026, is the newest post
(https://nim-lang.org/blog.html, read 2026-11-07).

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | By path: "Nim has the concept of a global search path (PATH)" "queried to determine where to find imported modules or include files." Pseudo-directories: "The `std` pseudo directory is the abstract location of Nim's standard library"; `pkg` refers unambiguously to a Nimble package and ignores the standard library's locations (as the reader rendered it); "recommended and preferred but not currently enforced that all stdlib module imports include the std/" pseudo directory. | https://nim-lang.org/docs/nimc.html and https://raw.githubusercontent.com/nim-lang/Nim/devel/doc/manual.md (offset 200000), read 2026-11-07 (panel 234's historian cited the manual at its lines 230 to 240, read 2026-11-05) |
| 1. collisions | "If multiple files are found an ambiguity error is produced." "However before the PATH is used the current directory is checked for the file's existence." | https://nim-lang.org/docs/nimc.html, read 2026-11-07 |
| 2. where written | The `.nimble` file's `requires`: "a list of package names with an optional version range separated by commas", e.g. `nim >= 0.10.0, jester`, or a local `requires "file://./libs/myutil"`. `nimble.lock`, separate: `vcsRevision` "The revision at which the dependency is locked", `url`, `downloadMethod` ("`git` or `hg`"), `dependencies`, and `checksums`, only `sha1`: "The _sha1_ checksum of the package files." | https://nim-lang.github.io/nimble/nimble-reference.html and https://nim-lang.github.io/nimble/workflow.html, read 2026-11-07 |
| 3. words | Module, package, the `.nimble` file, `nimble.lock`; the required `.nimble` fields are `version`, `author`, `description`, `license`. | the reference page above, read 2026-11-07 |
| 4. selection | Ranges in `requires`. The compiler "can only use the latest version of a package when compiling", so Nimble "feeds the path of each package to the compiler so that it knows precisely which version to use." The algorithm: `not found` on the guide's pages read. | https://nim-lang.github.io/nimble/folder-structure.html, read 2026-11-07 |
| 5. sources together | Git or hg repositories (`downloadMethod`), `file://` paths, and `nimble develop`, which "adds cloned packages to the special `nimble.develop` file"; packages can be uploaded "to the official package list". | the workflow, reference and index pages, https://nim-lang.github.io/nimble/index.html, read 2026-11-07 |
| 6. where kept | "Nimble stores all installed packages and metadata in `$HOME/.nimble` by default." "Libraries are stored in `$nimbleDir/pkgs2`". | https://nim-lang.github.io/nimble/folder-structure.html, read 2026-11-07 |
| 7. runs code? | The `.nimble` reference mentions "the new NimScript format"; "NimScript is the subset of Nim that can be evaluated by Nim's builtin virtual machine (VM)", which can run tasks and external commands (as the reader rendered the page). A sentence saying that reading or installing a dependency's `.nimble` runs it: `not found` on the reference, the NimScript page or the guide's index. | https://nim-lang.github.io/nimble/nimble-reference.html and https://nim-lang.org/docs/nims.html, read 2026-11-07 |
| 8. commands, errors | `nimble lock`, `nimble develop -a <path>`. On mismatch: "The checksum of the downloaded package is compared against the one written in the lock file"; "In the case the two checksums are not equal then it will be printed error message and the operation will be aborted." The text: `not found` on that page. | https://nim-lang.github.io/nimble/workflow.html, read 2026-11-07 |
| 9. privacy | `not found` on the pages read (the manual's pseudo-path section, the compiler guide, the Nimble guide's pages). | as above, read 2026-11-07 |
| 10. history | **01 March 2018, Nim 0.18.0**: "Standard library modules can now also be imported via the `std` pseudo-directory." "This is useful in order to distinguish between standard library and nimble package imports". | https://nim-lang.org/blog/2018/03/01/version-0180-released.html, read 2026-11-07 (panel 234's historian cited it, read 2026-11-05) |

**Version declarations**: `requires "nim >= 0.10.0"` declares the compiler a
package needs (https://nim-lang.github.io/nimble/nimble-reference.html, read
2026-11-07); "The exact Nim version can be pinned in `nimble.lock`"
(https://nim-lang.github.io/nimble/index.html, read 2026-11-07).

### 2.11 Odin (library collections; release dev-2026-11a, 03 Nov)

The newest release is `dev-2026-11a`, "released this 03 Nov"
(https://github.com/odin-lang/Odin/releases, read 2026-11-07).

| probe | what the page states | source, read |
|---|---|---|
| 1. naming | "The `core:` prefix is used to state where the import is meant to look; this is called a library collection." "If no prefix is present, the import will look relative to the current file." A local name: `import foo "core:fmt"`. Custom collections: "custom collections defined with the `-collection:<name>=<path>` command line switch". | https://odin-lang.org/docs/overview/ (offsets 0 and 100000), read 2026-11-07 (panel 234's historian cited the same sentences, read 2026-11-05) |
| 2 to 8 | **No package manager**: "Odin will **never** officially support a package manager." The FAQ's answer to managing code without one: "Copying and vendoring each package manually, and fixing the specific versions down is the most practical approach". So no catalogue, fingerprint, solver, index or fetch command is described; the code lives in the project's own tree (the FAQ's "vendoring"). | https://odin-lang.org/docs/faq/, read 2026-11-07 |
| 3. words | "A package is a directory of Odin code files, all of which have the same package declaration at the top."; "library collection". | https://odin-lang.org/docs/overview/, read 2026-11-07 |
| 9. privacy | "All declarations in a package are public by default." "The `private` attribute can be applied to an entity to prevent it from being exported from a package"; `@(private="file")` makes an entity private "to _the file_ instead of the package." | https://odin-lang.org/docs/overview/, read 2026-11-07 |
| 10. history | The stance is the FAQ's, undated on the page. | https://odin-lang.org/docs/faq/, read 2026-11-07 |

### 2.12 Goose (`github.com/aardappel/goose`, `docs/design/packages.md`)

The document says it was "written against the tree at `de24a156`" and states
no date (https://raw.githubusercontent.com/aardappel/goose/master/docs/design/packages.md,
read 2026-11-07; every cell below is that page, read that day).

| probe | what the page states |
|---|---|
| 1. naming | The root file declares a fetched package in its import line, `import name from "source";`, e.g. `import gfx from "aardappel/goose_gfx@v1";` (§4.2); `owner/repo@ref` means GitHub. Imports inside a package: "Imports resolve against the importing file's package, not the program's" (§4); "resolved relative to the *importing file's package root*" (§4.4). An alias syntax beyond the declared name: `not found` in the document. |
| 1. two claim one name | A requirement in a non-root file: the root must declare it, "packages are declared by the root file only" (§4.3); a different source for a declared name is an error, a different ref a warning, "since it is the root that knows which versions work together" (§4.3). |
| 2. where written | **In the import line**: "`@ref` is required for a git source" (§4.2); "Pinning a commit hash protects against a tag being moved to a different commit." (§4.6). "Lockfiles, version ranges and a registry are not used" (§1); a lockfile is deferred (§11). |
| 3. words | "A **package** is a directory with an entry file `<name>.goose` at its root"; "A **module** is one `.goose` file" (§4.1). |
| 4. selection | "Exactly one version of a package exists in a program" (§1); no ranges; the root decides (§4.3). |
| 5. sources together | Git repositories, GitHub by default; local folders, used in place and relative to the importing file (§4.2, as the reader rendered it); no registry (§1). |
| 6. where kept | `GOOSE_HOME`, default `~/.goose`; checkouts in `pkg/<host>/<owner>/<repo>/<ref>/`, native builds under `build/pkg/` (§4.6). |
| 7. runs code? | **Yes**: "code from the repository runs on the machine at build time, as with every language's build scripts" (§4.6); building a CMake package runs CMake. |
| 8. commands, errors | `goose build`, `goose run`, `goose check`, `goose fetch --update`, `goose toolchain`, `goose build --dry-run` (§3, §6.3). Missing requirement: "joe/mylib needs the package vec_extras: add import vec_extras from "github.com/x/vec_extras@v2"; to foo.goose (packages are declared by the root file only)" (§4.3), which names the line to add. Missing tools: "cannot find `git` on PATH, needed to fetch the package gfx from" (§4.6). A fingerprint error: none described (no fingerprint beyond the commit). |
| 9. privacy | None: "a library cannot hide its internals" (§11); "All names are public, and namespaces are global" (§2). |
| 10. history | `not found` as dates in the document. The milestone page of this project already cites it (lines 46 to 57 of `docs/roadmap/milestones/M-package-manager.md`), and panel 234's historian reported the move to per-package roots; neither is used here as a source. |

---

## Table 3, the indexes

Six questions per index: what it holds; the protocol; whether it can be served
as static files; who may publish a name, and namespaces; what happens to a
withdrawn version or name; how publishing is authenticated.

| index | holds | protocol | static files? | who publishes; namespaces | withdrawn | authentication | source, read |
|---|---|---|---|---|---|---|---|
| **Go module proxy** (proxy.golang.org) **and checksum database** (sum.golang.org) | Per version: "`$base/$module/@v/list`", "`$version.info`", "`$version.mod`", "`$version.zip`", optional "`$base/$module/@latest`". The database: "`$base/lookup/$module@$version`" returns "the `go.sum` lines for `$module` at `$version`" and "a signed, encoded tree description"; "`$base/tile/…`" returns log tiles. | Plain HTTP GET. | **Yes**: "The requests have no query parameters, and no specific headers are required, so even a site serving from a fixed file system (including a `file://` URL) can be a module proxy." A proxy may also "mirror the checksum database". | "In Go, there is no such thing as a package repository account." A version enters by its tag: "make sure there is a semantically versioned tag for this revision in the underlying source repository", then `go get module@version`; "The new version should be available within one minute." The module path is the namespace: "A module path should describe both what the module does and where to find it." | `retract` in the module's own `go.mod`; retracted versions are ignored by queries other than for named versions. The mirror: "the mirror aims to cache content in order to avoid breaking builds for people that depend on your package"; but "proxy.golang.org does not save all modules forever", a module without a detectable suitable license being cached only temporarily (as the reader rendered it). | None to publish (above). | https://go.dev/ref/mod (offsets 0 and 100000), https://go.dev/blog/supply-chain, https://proxy.golang.org/, read 2026-11-07 |
| **crates.io** | Index lines with `name`, `vers`, `deps`, `cksum` ("A SHA256 checksum of the `.crate` file"), `features`, `yanked`, `links`, `v`, `features2`, `rust_version`; `config.json` with `dl`, `api`, `auth-required`. | "two remote registry protocols: `git` and `sparse`"; crates.io's sparse index is `sparse+https://index.crates.io/`. | `not found`: the index page describes ETag/304, 401 and 404/410/451 behaviour and does not say static hosting suffices (searched there). | "crate names on crates.io are allocated on a first-come-first-serve basis. Once a crate name is taken, it cannot be used for another crate." Namespaces: `not found` on the publishing page. | "The version can never be overwritten, and the code cannot be deleted." Yank: "no new dependencies can be created against that version, but all existing dependencies continue to work"; "A yank **does not** delete any code." The index: "The JSON objects should not be modified after they are added except for the `yanked` field". | `cargo login` stores an API token, "a **secret**"; Trusted Publishing since July 11, 2025: a configured repository may "request a short-lived API token for publishing in a secure way using OpenID Connect (OIDC)"; "you'll need to publish your first release manually." | https://doc.rust-lang.org/cargo/reference/registry-index.html, https://doc.rust-lang.org/cargo/reference/publishing.html, https://blog.rust-lang.org/2025/07/11/crates-io-development-update-2025-07, read 2026-11-07 |
| **pub.dev** (hosted pub repository, spec v2) | Per package, `versions` with each `pubspec` and `archive_url`; "The `archive_sha256` should be the hex-encoded sha256 checksum of the file at archive_url"; `retracted` per version; `isDiscontinued` and `replacedBy` per package. | HTTP, `GET <hosted-url>/api/packages/<package>`; publishing by `GET <hosted-url>/api/packages/versions/new`, a multipart upload and a finalize request. | `not found` in the spec (its upload and 401 behaviour need a server; "The `archive_url` may be temporary"). | The first person to publish a package becomes the first and only person authorized to upload more versions, and may add uploaders; members of a verified publisher may all upload (as the reader rendered the page). Namespaces: `not found` on the publishing page. | "Keep in mind that a published package lasts forever."; a version may be retracted only "within seven days of publication", and stays usable by those whose `pubspec.lock` holds it; discontinued packages stay published (as the reader rendered the page). | "`Authorization: Bearer <token>`"; "authentication can only be used when `<hosted-url>` uses HTTPS". Upload permission by Google account; after the first version, automated publishing "through GitHub Actions or Google Cloud service accounts". | https://github.com/dart-lang/pub/blob/master/doc/repository-spec-v2.md and https://dart.dev/tools/pub/publishing, read 2026-11-07 |
| **npm registry** | The "packument": `GET /{package}` "responds with the package metadata document"; each version's `dist` has a `shasum` and a `tarball` URL (`dist.integrity`: `not found` on that page; the lock's `integrity` is Table 2.6). | HTTP. | `not found` on the registry API page. | Scopes: "you are granted a scope that matches your user or organization name"; a scope "allows you to create a package with the same name as a package created by another user"; "Unscoped packages are always public." Whether unscoped names are first-come: `not found` there. | Unpublishing is allowed within the first 72 hours if no other package depends on it; after that only with no dependents, fewer than 300 downloads in the last week and a single owner (as the reader rendered the page); "Once `package@version` has been used, you can never use it again."; otherwise `npm deprecate`. | "Trusted publishing allows you to publish npm packages directly from your CI/CD workflows using OpenID Connect (OIDC) authentication, eliminating the need for long-lived npm tokens"; GitHub Actions, GitLab CI/CD and CircleCI; a package may "Require two-factor authentication and disallow tokens." | https://raw.githubusercontent.com/npm/registry/main/docs/REGISTRY-API.md, https://docs.npmjs.com/about-scopes, https://docs.npmjs.com/policies/unpublish, https://docs.npmjs.com/trusted-publishers, read 2026-11-07 |
| **PyPI** (the "simple" API) | A root page with "a single anchor element per project", a project page with "a single anchor element per file", each URL with "`#<hashname>=<hashvalue>`", the digest of the file; in JSON, `hashes` maps names to "a hex encoded digest of the file"; `core-metadata` at `{file_url}.metadata`. | HTML (`application/vnd.pypi.simple.v1+html`, alias `text/html`) or JSON (`application/vnd.pypi.simple.v1+json`, since PEP 691). | `not found`: neither PEP 503 nor the current specification mentions static hosting (searched both). | Names normalized: "lowercased with all runs of the characters `.`, `-`, or `_` replaced with a single `-` character." "Under no circumstances will a name be reassigned against the wishes of a reachable owner"; abandonment and transfer rules under PEP 541. Organization accounts exist (the documentation index lists them); whether they form a name namespace: `not found`. | Yank, "a non-destructive alternative to deletion": a yanked release is "always ignored by an installer, unless it is the only release that matches a version specifier". Filename reuse after deletion: `not found` on the yanking page. | Trusted Publishing: "using the OpenID Connect (OIDC) standard to exchange short-lived identity tokens between a trusted third-party service and PyPI", which "eliminates the need to use manually generated API tokens". | https://peps.python.org/pep-0503/, https://packaging.python.org/en/latest/specifications/simple-repository-api/, https://docs.pypi.org/ and its yanking, trusted-publishers and name-retention pages, read 2026-11-07 |
| **Hex** (hex.pm) | `/names`, `/versions`, `/packages/NAME`, Protocol Buffers, gzipped, "signed by the repository's private key"; tarball with `VERSION`, `metadata.config`, `contents.tar.gz`; the outer checksum is "the SHA-256 checksum of the entirety of the tarball file"; the inner one "is deprecated in favor of the 'outer checksum.'" | HTTP resources; "The signature is an (unencoded) RSA signature of the (unencoded) SHA-512 digest of the payload." | `not found`: the registry spec asks only for "appropriate cache headers"; the self-hosting page answered 404. | Organization packages are published to the global repository namespace, like other public packages; two packages defining the same module name cannot be used together (both as the reader rendered the FAQ and the publishing guide). First-come names: `not found` in the FAQ. | Changed or unpublished "within 60 minutes of the package version release" or "within 24 hours of initial release of the package"; otherwise retire: "Instead of unpublishing we recommend to instead retire a package or release", retired releases still resolvable with a warning. | `mix hex.user auth`; API keys in `HEX_API_KEY`, scoped to `publish:name` and `api:read`; 2FA codes to create keys; Workload Identity (Trusted Publishing) recommended for GitHub Actions (as the reader rendered the page). | https://raw.githubusercontent.com/hexpm/specifications/main/registry-v2.md, …/package_tarball.md, https://hex.pm/docs/faq, https://hex.pm/docs/publish, read 2026-11-07 |
| **JSR** (jsr.io) | `https://jsr.io/@<scope>/<package-name>/meta.json` (versions), `…/<version>_meta.json` (each file's `size` and `checksum`, e.g. "sha256-a41796ce…"), `…/<version>/<path>` (modules); an npm-compatible registry at `https://npm.jsr.io`. | HTTP GET, with an `Accept` header that does not include `text/html`. | `not found` as a statement; the page says "Modules are immutable, so they can be cached indefinitely." | "On JSR, all packages are contained within a scope"; anyone can create a scope, whose creator "is automatically an admin"; scope names unique across JSR. | "JSR is immutable. This means that once you publish a package version, you can’t change it." "Note that yanking does not remove the contents of the package version from the registry." | "Authentication is done using an `Authorization` header with a `Bearer` or `githuboidc` token"; GitHub Actions OIDC tokens "only support the `package/publish` permission". | https://jsr.io/docs/api, https://jsr.io/docs/scopes, https://jsr.io/docs/immutability, https://jsr.io/docs/introduction, read 2026-11-07 |

---

## Table 4, the catalogue's possible formats

| format | systems that use it (Table 2) | can reading it run code? | pitfalls its specification, documentation or a tracker records | source, read |
|---|---|---|---|---|
| **YAML** | Dart's `pubspec.yaml`. | The YAML specifications read say nothing on code execution (`not found` in 1.1's resolution section and 1.2.2's sections 1.2, 6.8.1 and 10); PEP 518 judged YAML "not safe by default". | **Read from the specifications themselves.** YAML 1.1 (Final Draft, 2005-01-18) leaves typing to the application: "Tag resolution is specific to the application"; "plain scalars may be matched against a set of regular expressions to provide automatic resolution of integers, floats", the expressions living in the type pages for 1.1. There, the boolean type is `y\|Y\|yes\|Yes\|YES\|n\|N\|no\|No\|NO\|true\|True\|TRUE\|false\|False\|FALSE\|on\|On\|ON\|off\|Off\|OFF`, so a plain `no` or `on` matches it; the base-10 float is `[-+]?([0-9][0-9_]*)?\.[0-9.]*([eE][-+][0-9]+)?`, which `1.10` matches. YAML 1.2.2 (Revision 1.2.2, 2021-10-01), core schema: bool is "`true \| True \| TRUE \| false \| False \| FALSE`", so `no` and `on` match nothing and, "if none of the regular expressions matches, the scalar is resolved to `tag:yaml.org,2002:str`"; float (Number) is "`[-+]? ( \. [0-9]+ \| [0-9]+ ( \. [0-9]* )? ) ( [eE] [-+]? [0-9]+ )?`", which `1.10` still matches. So under both versions `1.10` is a float, the same number as `1.1`; `no` and `on` change from booleans (1.1) to strings (1.2.2). And "A version 1.2 YAML processor must also accept documents with an explicit “%YAML 1.1” directive." In a tracker: pub, 2014, "Printing a useful error message if a version field is a num", for `version: 1.0`; the new message: ""version" field must have three numeric components: major, minor, and patch. Instead of "1.0", consider "1.0.0"." Which YAML version Dart's parser implements: `not found` on its package page (`yaml` 3.1.4). PEP 518: "the specification is large: 86 pages if printed on letter-sized paper". | https://yaml.org/spec/1.1/, https://yaml.org/type/bool.html, https://yaml.org/type/float.html, https://yaml.org/spec/1.2.2/ (offsets 0 and 150000), https://codereview.chromium.org/632483002, https://pub.dev/packages/yaml, https://peps.python.org/pep-0518/, read 2026-11-07 |
| **TOML** | `Cargo.toml`, `Cargo.lock`, `pyproject.toml`, `pylock.toml`, `uv.lock`, `gleam.toml`, Gleam's `manifest.toml`. | A sentence on code execution: `not found` in the specification; its objective is to "map unambiguously to a hash table". | "Defining a key multiple times is invalid." A bare `1.10` is a float (a float has "an integer part" and "a fractional part"); strings must be quoted ("Basic strings are surrounded by quotation marks"; "Unspecified values are invalid"). Versions differ: v1.0.0, "Inline tables are intended to appear on a single line", "No newlines are allowed between the curly braces unless they are valid within a value", "A terminating comma (also called trailing comma) is not permitted after the last key/value pair in an inline table"; v1.1.0, "Published on 12/18/2025", lets inline tables span lines and permits a trailing comma (as the reader rendered it). PEP 518 chose TOML over JSON, YAML and configparser for the reasons in those rows. | https://toml.io/en/latest, https://toml.io/en/v1.0.0, https://peps.python.org/pep-0518/, read 2026-11-07 |
| **JSON** | npm's `package.json` and `package-lock.json`; Deno's `deno.json` ("or `deno.jsonc`"); Nimble's `nimble.lock`; JSR's `meta.json`. | A sentence on code execution: `not found` in RFC 8259 (it defines data only). | "The names within an object SHOULD be unique." "When the names within an object are not unique, the behavior of software that receives such an object is unpredictable." "Many implementations report the last name/value pair only." No comment syntax: whitespace is only "`ws = *( %x20 / %x09 / %x0A / %x0D )`". PEP 518: JSON "does not lend itself to easy editing by a human being" and allows no comments. | https://www.rfc-editor.org/rfc/rfc8259, https://docs.deno.com/runtime/reference/deno_json/, https://peps.python.org/pep-0518/, read 2026-11-07 |
| **A line format of its own** | Go's `go.mod` and `go.sum`; pip's `requirements.txt`. | `not found` as a statement in either reference; `go.mod`'s grammar holds directives only. | `go.mod`: "line-oriented. Each line holds a single directive"; "Comments start with `//` and run to the end of a line. `/* */` comments are not allowed."; the reference's own grammar and keyword list omit directives it documents (Table 1A). `requirements.txt`: lines may carry options such as `-i, --index-url` and `--extra-index-url`, may refer to other files with `-r` and `-c`, and "Pip supports the use of environment variables inside the requirements file", in the form `${API_TOKEN}` only. | https://go.dev/ref/mod and https://pip.pypa.io/en/stable/reference/requirements-file-format/, read 2026-11-07 |
| **A file in a language's own syntax** | Zig's `build.zig.zon`; Swift's `Package.swift`; Nimble's `.nimble` (NimScript); Goose's root import line. | It depends on the file. ZON is "a ZON data file", imported as data. `Package.swift` is "a Swift program (a script of sorts) which SwiftPM builds and executes as a separate process", sandboxed. NimScript "can be evaluated by Nim's builtin virtual machine", with tasks and external commands. Goose's lines are import declarations of the language. | Swift's manifest must open with `// swift-tools-version:`, a version declaration on line one before 6.0. Zig's `minimum_zig_version` "is currently advisory only". PEP 518's catch-22 for an executable manifest: "You can't execute a `setup.py` file without knowing its dependencies." | https://ziglang.org/documentation/master/ (offset 300000), https://forums.swift.org/t/pre-pitch-swiftpm-manifest-based-on-result-builders/53457, https://nim-lang.org/docs/nims.html, the Goose document, https://raw.githubusercontent.com/swiftlang/swift-package-manager/main/Sources/PackageManagerDocs/Documentation.docc/SettingSwiftToolsVersion.md, https://peps.python.org/pep-0518/, read 2026-11-07 |

---

## Table 5, angle brackets and prefixes in imports

| system | the mark | what it means, in the page's words | source, read |
|---|---|---|---|
| **C** | `<…>` against `"…"` | The standard: `# include <h-char-sequence>` "searches a sequence of implementation-defined places for a header identified uniquely by the specified sequence between the < and > delimiters"; `# include "q-char-sequence"` names a source file "searched for in an implementation-defined manner. If this search is not supported, or if the search fails, the directive is reprocessed as if it read `# include <h-char-sequence>`". Its example: `#include <stdio.h>` and `#include "myprog.h"`. GCC: `<file>` "is used for system header files"; `"file"` "is used for header files of your own program", searched "first in the directory containing the current file, then in the quote directories and then the same directories used for <file>." | ISO/IEC 9899:201x, Committee Draft N1570, April 12, 2011, §6.10.2, printed pages 164 and 165, https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf; https://gcc.gnu.org/onlinedocs/cpp/Include-Syntax.html; both read 2026-11-07 |
| **Go** | none; a dot in the first path element | "The Go standard library uses package paths that do not contain a dot in the first path element, and the `go` command does not attempt to resolve such paths from network servers." | https://go.dev/ref/mod, read 2026-11-07 (re-read; panel 234's historian cited it at its line 90, read 2026-11-05) |
| **Rust** | an optional `::` before an external crate, `crate::` before the program's own | "`::foo::bar` always refers to the name `bar` inside the external crate `foo`"; "you can refer to the root of the current crate with the `crate::` prefix" | https://doc.rust-lang.org/edition-guide/rust-2018/path-changes.html, read 2026-11-07 |
| **Dart** | `dart:` for the SDK's libraries, `package:` for a package's, a path for a file | "For built-in libraries, the URI has the special `dart:` scheme." "The `package:` scheme specifies libraries provided by a package manager such as the pub tool." | https://dart.dev/language/libraries, read 2026-11-07 |
| **Deno** | `jsr:` and `npm:` in the specifier, or a bare name the import map turns into one | Third-party specifiers "start with `jsr:` or `npm:`"; "The remapped name can be any valid specifier." | https://docs.deno.com/runtime/fundamentals/modules/, read 2026-11-07 |
| **Zig** | a file path ending `.zig`, against a bare module name | "`target` is either a relative path to another file … or it is the name of a module" | https://ziglang.org/documentation/master/ (offset 300000), read 2026-11-07 |
| **Node** | `node:` for a built-in; `#` for a package's private mapping; `./`, `../`, `/` for a file; a bare name for `node_modules` | "Built-in modules can be identified using the `node:` prefix, in which case it bypasses the `require` cache." Mandatory for some, "to prevent newly introduced built-in modules from having a conflict with user land packages that already have taken the name" (the list that day: `node:ffi`, `node:sea`, `node:sqlite`, `node:test`, `node:test/reporters`). `#` entries "must always start with `#` to ensure they are disambiguated from external package specifiers." | https://nodejs.org/api/modules.html and https://nodejs.org/api/packages.html, read 2026-11-07 (panel 234's historian cited Node's `node:` at its line 41, read 2026-11-05) |
| **Nim** | `std/` and `pkg/` | Two pseudo directories for avoiding ambiguity when several modules share a path (as the reader rendered the manual); `std` "is the abstract location of Nim's standard library"; `pkg` names a Nimble package; `std/` "recommended and preferred but not currently enforced". Added in 0.18.0, "to distinguish between standard library and nimble package imports". | https://raw.githubusercontent.com/nim-lang/Nim/devel/doc/manual.md (offset 200000) and https://nim-lang.org/blog/2018/03/01/version-0180-released.html, read 2026-11-07 (panel 234's historian cited both at its lines 230 to 245, read 2026-11-05) |
| **Odin** | `core:`, `vendor:`, a custom `name:` | "The `core:` prefix is used to state where the import is meant to look; this is called a library collection." "If no prefix is present, the import will look relative to the current file." "The default paths to the `core` and `vendor` library collections"; custom ones from "the `-collection:<name>=<path>` command line switch"; an example comment, `foreign import baz "shared:libbaz.a"  // with -collection:shared=path/to/shared`. | https://odin-lang.org/docs/overview/ (offsets 0 and 100000), read 2026-11-07 |
| **Python** | a leading dot for the program's own package, none otherwise | "Relative imports use leading dots." | https://docs.python.org/3/reference/import.html, read 2026-11-07 |
| **Goose** | a source string in the root's import line | `import gfx from "aardappel/goose_gfx@v1";` (§4.2) | https://raw.githubusercontent.com/aardappel/goose/master/docs/design/packages.md, read 2026-11-07 |
| **Swift** | `not found`: modules are imported by name, and a collision is answered in the manifest by `moduleAliases` | (Table 2.7, probe 1) | …/IntroducingPackages.md and …/ModuleAliasing.md, read 2026-11-07 |
| **Gleam** | `not found`: a module path from the package's source tree | (Table 2.8, probe 1) | https://tour.gleam.run/basics/modules/, read 2026-11-07 |

---

## What a reader of these tables should not conclude

- **That a `not found` is an absence.** It names a page that did not carry the
  answer. Swift's cache location, Gleam's build-tool execution, Deno's hashed
  bytes and Zig's privacy rule are each documented somewhere this survey did
  not reach, or nowhere; the tables cannot say which.
- **That rows or `not found` counts rank the systems.** A system with more
  `not found` cells is a system whose pages this reader found less of, not a
  weaker system. Go and Cargo were read in the most depth on purpose (the
  brief), so they have the fewest gaps.
- **That the quoted errors are the newest releases' output.** Several come
  from test suites (Cargo, pub), from older sources (Deno 1.28.1, 2022), from
  a user's post (pip, 2020) or from a pull request (Zig, 2024). Nothing was
  run, so none is a measurement of today's message.
- **That two "checksums" give the same guarantee.** They are computed over
  different bytes: Go over file names and contents, not the zip; Zig over the
  selected files' contents; Cargo, Hex's outer checksum, pub, PyPI and
  `pylock.toml` over the archive file; npm over the unpacked artifact as
  Subresource Integrity; Nimble with SHA-1 over package files; SwiftPM per
  machine, trust on first use; Goose with the commit hash alone. Gleam's CVE
  shows that where the value came from matters as much as what it hashes. The
  tables report each mechanism; they do not compare their strength.
- **That "runs code" means one thing.** Package.swift runs when it is read;
  npm's scripts run at install; Cargo's build scripts and procedural macros,
  Dart's hooks and Zig's `build.zig` run at build; Goose runs a package's
  build at build time; Go states that neither fetching nor building runs a
  dependency. Sandboxes differ (SwiftPM's on supporting platforms, Deno's
  opt-in scripts). The rows are not interchangeable.
- **That the timelines measure how hard the problem is.** They give release
  dates. Go's run from go1 (2012) to module mode by default (2021) and
  Cargo's from rustpkg's removal (2014) to the MSRV-aware resolver (2025)
  cover different languages at different ages and sizes; no page read states
  why either took the time it took, and the survey cannot supply the reason.
- **That the documentation describes the shipped behaviour.** Go's reference
  disagrees with itself and with the Go 1.25 notes (Table 1A), and the Cargo
  changelog served on 2026-11-07 shows an unfinished 1.99 entry; where pages
  disagree, only running the tool would settle it, and nothing was run.
- **That a format's specification predicts a given parser.** The YAML rows
  are what the 1.1 and 1.2.2 texts say; 1.1 itself says "Tag resolution is
  specific to the application", which rules Dart's parser applies was not
  found, and a 1.2 processor "must also accept documents with an explicit
  “%YAML 1.1” directive".
- **That an index can or cannot be served as static files** where the cell
  says `not found`: only Go's protocol states it can; the others' pages were
  silent, which is not a refusal.
- **Anything about language models.** Which of these systems a model writes
  most reliably was not measured here; the brief's criteria name a reader test
  the sitting registers, and no table above is that test.

---

## Counts, by hand

Counted by hand from the tables above, not by an instrument: Table 1 holds
207 finding cells (45 for Go, 162 for Cargo), none `not found`; Table 2 holds
140 probe rows, 40 of which contain a `not found`; Table 3 holds 42 finding
cells, 12 of which contain one; Table 4 holds 15, 4 of which contain one;
Table 5 holds 26, 2 of which are `not found`. In all, **430 finding cells, 58
containing a `not found`** (most of them partly sourced, a `not found` naming
the one sub-question the page did not answer), and **372 carrying only
sourced statements**.

## Search

Every page below was fetched on 2026-11-07 with a page-reading tool; each
cell above names the pages it rests on. The web searches are listed with
their exact queries.

- **Go**: https://go.dev/doc/devel/release (twice: newest release, then the
  dates of go1, 1.4, 1.5, 1.6, 1.11, 1.13, 1.14, 1.16, 1.17, 1.18, 1.21.0 to
  1.25.0); https://go.dev/ref/mod (eight reads at offsets 0 and 100000: the
  directives, the file's opening, `go.sum`, the checksum database and its
  protocol, the proxy protocol, MVS, version queries, the grammar's
  inconsistencies read twice); https://go.dev/ref/spec (offset 0 did not reach
  the import section; offset 200000 did); https://go.dev/blog/supply-chain
  (twice, the second for the whole sentence); the `go` command's source,
  `modfetch/fetch.go` and `modload/import.go`, raw from github.com/golang/go
  at `master`; release notes go1.4, 1.5, 1.6, 1.11, 1.13, 1.14, 1.16, 1.17,
  1.21, 1.23, 1.24, 1.25, 1.26, 1.27; https://go.dev/doc/faq (read for a
  sentence on how long versioning took, not found there);
  https://research.swtch.com/vgo-intro; https://github.com/golang/dep;
  https://proxy.golang.org/.
- **Cargo and Rust**: https://doc.rust-lang.org/cargo/CHANGELOG.html;
  https://blog.rust-lang.org/releases/; release posts 1.0, 1.31, 1.56, 1.62,
  1.68, 1.70, 1.84, 1.85, 1.99.0; the 2014 Cargo post (its `.html` address is
  a redirect stub, read again without it); the lockfile guidance post; the
  crates.io update of July 11, 2025; the manifest, specifying-dependencies,
  resolver, build-scripts, registry-index and publishing references; the
  glossary; the Cargo home guide; the `cargo add` and `cargo build` pages; the
  Cargo.toml-vs-Cargo.lock guide; `crates/cargo-util-schemas/src/lockfile.rs`
  raw at `master`; `sources/registry/download.rs` in the nightly-rustc source
  docs; Cargo's tests `lockfile_compat.rs` and `registry.rs` on
  rust.googlesource.com; pull request 14595; the Rust Reference's preludes,
  visibility and procedural-macros chapters; the edition guide's path-changes
  page; https://releases.rs/docs/0.10.0 and the GitHub release `0.10`.
- **Dart**: dart.dev's pubspec, dependencies, glossary, package-layout,
  `pub get`, `pub add`, publishing, hooks, libraries, language-evolution and
  versioning pages; `doc/solver.md` and `doc/repository-spec-v2.md` in
  github.com/dart-lang/pub; pub's commit `4c9ebd0e` and two test files on
  dart.googlesource.com; the 2014 change on codereview.chromium.org; the
  `yaml` package's page on pub.dev.
- **Zig**: `doc/build.zig.zon.md` raw at `master`;
  https://ziglang.org/download/ (twice) and `index.json`; release notes
  0.11.0 (offsets 0 and 100000), 0.14.0, 0.16.0 (offsets 0 and 100000) and
  0.17.0; the language reference at offsets 0, 100000 and 300000 (the `pub`
  visibility sentence looked for at 100000 to 400000, not reached); the
  build-system guide; pull request 21589.
- **Deno**: the modules page, the packages guide, the `deno.json` reference,
  the releases page, the 1.0 and 2.0 posts, the `deno_lockfile` repository,
  Deno 1.28.1's `lockfile.rs` on docs.rs.
- **Node and npm**: Node's modules, packages and errors pages (errors at
  offsets 0 and 100000); npm's package.json, package-lock.json (twice),
  scripts, npm-install, unpublish-policy, scopes and trusted-publishers pages;
  `lib/index.js` of npm/ssri raw at `main`; `docs/REGISTRY-API.md` of
  npm/registry raw at `main`; the npm blog post of March 23rd, 2016.
- **Swift**: swift.org's install and download pages (the second a redirect);
  the PackageDescription page on docs.swift.org; the DocC catalog's directory
  listing and its IntroducingPackages, ResolvingPackageVersions,
  PackageSecurity, ModuleAliasing, Plugins and SettingSwiftToolsVersion files;
  the PubGrub source directory listing; SE-0386; the Swift 3.0 post; a Swift
  Forums pre-pitch of 2021.
- **Gleam**: the releases page; gleam.run's documentation index, gleam.toml
  reference (its old address redirects), command-line reference and
  writing-gleam guide; the tour's modules page; `manifest.rs`, `paths.rs` and
  `compiler-core/Cargo.toml` raw at `main`; the ERLEF advisory for
  CVE-2026-59247.
- **Python**: pip's requirements-file, secure-installs and
  dependency-resolution pages; the pyproject.toml and simple-repository-API
  specifications; PEPs 8, 503, 518, 621 and 751; the import-system chapter;
  uv's resolution, resolver-internals, project-layout and cache pages;
  docs.pypi.org's index, yanking, trusted-publishers and name-retention pages
  (pypi.org/help redirects there); a Python forum thread of 2020.
- **Nim**: the Nimble README and guide (index, reference, folder structure,
  workflow); the manual (offsets 0 and 200000); the compiler guide; the
  NimScript page; the 0.18.0 post; the blog index.
- **Odin**: the FAQ; the overview (offsets 0 and 100000); the releases page.
- **Goose**: `docs/design/packages.md` raw at `master`.
- **Indexes not above**: Hex's `registry-v2.md` and `package_tarball.md` in
  hexpm/specifications, its FAQ and publishing guide; JSR's introduction,
  API, scopes and immutability pages.
- **Formats**: https://yaml.org/spec/1.1/ (offset 0, its resolution section);
  https://yaml.org/type/bool.html and https://yaml.org/type/float.html (the
  1.1 types); https://yaml.org/spec/1.2.2/ (offsets 0 and 150000; offset 0
  alone did not reach chapter 10); https://toml.io/en/latest and
  https://toml.io/en/v1.0.0; RFC 8259 at rfc-editor.org.
- **C**: GCC's Include Syntax page; the N1570 draft from open-std.org, whose
  text the page reader could not decode; the tool stored the PDF it fetched
  in its own results folder outside this repository
  (`~/.claude/projects/…/tool-results/webfetch-1794054623377-aaf84e.pdf`, a
  file this seat did not write), and its pages 182 and 183, printed 164 and
  165, were read from there as images.
- **Re-reads the brief asked for**: panel 234's historian's Nim rows (its
  lines 216 to 245), Node's `node:` (line 41) and Go's dot (line 90) were each
  re-read at their pages on 2026-11-07 and cited from today's reading; its
  Odin rows (lines 98 to 128) likewise for the overview and FAQ. Odin's
  `vendor/README.md`, which panel 234 also cited, was not re-read and is not
  used.
- **Pages that failed, none cited**: `src/cargo/core/resolver/resolve.rs` raw
  at `master` (404); https://nex3.medium.com/pubgrub-2fb6470504f (403);
  `tests/specs/lockfile/no_lock/main.out` raw in denoland/deno (404); a thread
  on questions.deno.com (429);
  https://docs.swift.org/swiftpm/documentation/packagemanagerdocs/ (404);
  pip's `src/pip/_internal/exceptions.py`, raw and on github.com (404 both);
  https://hex.pm/docs/self_hosting (404).
- **Web searches**: `"Announcing Rust 1.99.0"`; `Cargo.lock lockfile "version
  4" default Cargo 1.83 changelog ResolveVersion V4`; `cargo "failed to verify
  the checksum of" source code registry download.rs`; `cargo error "checksum
  for" "changed between lock files" "this could be indicative of"`; `cargo "no
  matching package named" "found" "location searched" error message`
  (third-party reports only, not cited; the test suite was read instead);
  `rustpkg removed Rust 0.10 release notes "rustpkg" Cargo replacement 2014`;
  `dart pubspec.lock sha256 content hash "pub get" verify "--enforce-lockfile"
  package integrity`; `dart pub "content-hash" OR "sha256" pubspec.lock
  introduced Dart 2.19 OR "Dart 3" changelog pub client hashes`; `dart pub
  "which doesn't exist (could not find package" "version solving failed"`;
  `PubGrub "Next-Generation Version Solving" Natalie Weizenbaum 2018 pub`;
  `zig "ZIG_LOCAL_PKG_DIR" OR "zig-pkg" local package directory default fetch
  project` (third-party results, not cited; the 0.16.0 notes were read
  instead); `zig build "error: hash mismatch" "expected" "found" build.zig.zon
  package`; `zig "hash mismatch: manifest declares" "but the fetched package
  has"`; `zig build "no dependency named" "in 'build.zig.zon'" error` (user
  reports only, not cited); `deno.lock "Integrity check failed" error message
  lockfile jsr npm sha256 format`; `deno 2 "Integrity check failed for"
  "remote specifier" OR "package" "The source code is invalid, as it does not
  match the expected hash in the lock file"`; `npm blog "kik, left-pad, and
  npm" 2016 unpublish` (secondary coverage, not cited; the post itself was
  read); `Swift Package Manager documentation Package.swift
  "swift-tools-version" manifest Package.resolved docs.swift.org swiftpm`;
  `docs.swift.org swiftpm documentation "Package.resolved" "resolving"
  dependencies article`; `swiftpm documentation docc "ModuleAliasing" OR
  "Module aliasing" swift package manager docs.swift.org/swiftpm`; `SwiftPM
  manifest Package.swift "compiled" "executed" sandbox "manifest" swift
  package manager evaluates manifest security`; `gleam compiler error
  "Duplicate module" two packages same module name OR gleam "outer checksum"
  mismatch error manifest.toml`; `pip "THESE PACKAGES DO NOT MATCH THE HASHES
  FROM THE REQUIREMENTS FILE" "someone may have tampered with them"`;
  `crates.io trusted publishing announcement blog.rust-lang.org OIDC GitHub
  Actions`; `dart pub pubspec "version" field "must be a string" yaml number
  OR float "1.10" issue dart-lang/pub`.
- **Looked for and not found, by where**: a sentence on how long Go's
  versioning work took (the Go FAQ, the vgo post); the date PubGrub entered
  pub (solver.md; the Medium post refused); the bytes Deno's lock hashes per
  source kind (its packages guide, modules page, `deno.json` reference, the
  archived lockfile repository); Swift's cache location and `Package.resolved`
  fields (the DocC introduction, resolution, security and plugins files);
  Gleam's handling of Erlang build tools and its error texts (its guide and
  command reference); Zig's cross-file `pub` rule (its language reference at
  offsets 100000 to 400000); static hosting for crates.io, pub.dev, npm, PyPI,
  Hex and JSR (each index's protocol page named in Table 3); which YAML
  version Dart's parser implements (its pub.dev page).
