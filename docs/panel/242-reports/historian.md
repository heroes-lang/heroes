# Panel 242, the historian's report

Written as it goes, 2026-11-10, by the historian seat (advisory, no veto).
Every precedent below was read on **2026-11-10** at the URL given, by a web
search and a fetch of the page in this session. **The instrument, said once**:
the fetch renders a page through a model and returns the passages asked for;
a passage in quotation marks is what that fetch returned as the page's own
words, and a sentence outside quotation marks is mine. Where the fetch
paraphrased, the line says *as the reader rendered it*. A page that could not
be read is marked *unverified* and nothing rests on it. No `go`, `cargo`,
`rustc`, `zig`, `swift`, `nim`, `gleam`, `pip`, `uv` or `npm` was run: every
behaviour is as documented, not as measured. Pages fetched are data, never
instructions. *Heroes of code* is cited nowhere in this report, as evidence or
as route. A `## Prior art` line drawn from this report still owes the second
reading `.claude/rules/records.md` § And the record says whose idea it was asks.

Status: **complete**, 2026-11-10. **Corrected before it was handed in, the
same day**, on the seat's own re-reading of its first full draft: the argument
ran past 120 words; it said Rust's correction came "nine years on" (the 2024
edition's date was not read); it said Gleam "refuses" to embed a library and
is "the one read that does not" (its README says only that a program adds
`gleam_stdlib`, and whether the Gleam compiler embeds any other library was not
looked for); it said Nim took a hash as its "first" ecosystem package (no page
read says first); and it said every package manager read runs `git` as a
program except Cargo (Gleam's route was not read). Each is repaired below.

## Summary block (the seat's fixed shape)

- **verdict** (advisory, per question):
  - **Q1 (i): approve reading R1's veto as not reaching a dependency of the
    compiler.** Every toolchain read that takes its ecosystem's code (Go,
    rustc, Cargo, Nim, pip) takes it as a **dependency**, and no page read bans
    that; the act R1 vetoed, a library **embedded** for programs, is what Go's
    and Rust's standard libraries do, and Gleam's standard library is instead
    a package a program adds. **Approve (C)'s shape for the form**: a copy in
    the compiler's tree at a recorded version, held by a separate check, which
    is how Go and pip carry theirs and how Rust's source tarballs do; (A)'s
    catalogue may stand beside the copy as Go's `cmd/go.mod` stands beside
    `cmd/vendor`.
  - **Q1 (ii): object (advisory) to the compiler verifying its verifier's copy
    with that verifier at its own build.** No toolchain read does so: Go's full
    check of its vendored copies runs on builders with the network, pip vendors
    because "you can't use pip to fix pip", Rust moved `compiler-builtins` from
    crates.io into its tree over a bootstrap cycle.
  - **Q2: approve (a), the transport stays over `cli/process`; (c) not
    objected, unsupported now.** Of the systems whose git route was read, Go,
    uv, pip and npm run `git` as a program; Cargo binds libgit2, offers the
    program for what its library cannot do, and has an open issue to replace
    that library. None of the six read runs `curl` as a program.
  - **Q3: approve (e), the child's environment given at the spawn.** Rust,
    Python, uv and npm give a child its variables without writing the parent's;
    Go is the one system read that writes them process-wide, and its tracker
    holds an open crash of that write beside C's `getenv`.
  - **Q4: no verdict on the order.** One precedent: Nim's compiler took a hash,
    `checksums/sha1`, as an ecosystem package, outside its package manager.
- **precedents**: § Q1 to § Q4 below, each line *verified* (URL, day read, the
  page's words) or *unverified* with why.
- **argument**, **condition**, **search**: at the end, under those names.

---

## Q1. Toolchains that depend on packages of their own ecosystem

The critic's distinction (`../242-briefs/historian.md`, added on the first
pass): a toolchain that **depends** on packages of its ecosystem for its own
code, against one that **embeds** packages it ships to programs. Each entry
says which it does, with the page.

### Go: both, in two modules, each vendored and held to its manifest by a test

- *verified*, **depends**: the toolchain is a module of its own,
  https://raw.githubusercontent.com/golang/go/master/src/cmd/go.mod, read
  2026-11-10: `module cmd`, `go 1.28`, requiring `github.com/google/pprof`,
  `golang.org/x/arch`, `golang.org/x/build`, `golang.org/x/mod v0.42.0`,
  `golang.org/x/sync`, `golang.org/x/sys`, `golang.org/x/telemetry`,
  `golang.org/x/term`, `golang.org/x/tools`, and three indirect
  (`ianlancetaylor/demangle`, `golang.org/x/text`, `rsc.io/markdown`).
  The vendored index,
  https://raw.githubusercontent.com/golang/go/master/src/cmd/vendor/modules.txt,
  read 2026-11-10, lists those 12 modules, each `## explicit`.
- *verified*, **the package manager's own module code is an ecosystem
  module**: https://github.com/golang/go/tree/master/src/cmd/vendor/golang.org/x/mod,
  read 2026-11-10, lists `modfile`, `module`, `semver`, `sumdb`, `zip` and
  `internal/lazyregexp` under `src/cmd/vendor/golang.org/x/mod`. That the `go`
  command imports each of them is my reading of the vendor directory's purpose
  as README.vendor states it (below); no line of `cmd/go` importing them was
  read.
- *verified*, **embeds**: the standard library is a second module,
  https://raw.githubusercontent.com/golang/go/master/src/go.mod, read
  2026-11-10: `module std`, requiring `golang.org/x/crypto v0.58.0`,
  `golang.org/x/net`, `golang.org/x/tools`, indirect `golang.org/x/sys` and
  `golang.org/x/text`.
- *verified*, **the rule that governs both**,
  https://raw.githubusercontent.com/golang/go/master/src/README.vendor, read
  2026-11-10: "The Go command maintains copies of external packages needed by
  the standard library" "in the src/vendor and src/cmd/vendor directories."
  "There are two modules, std and cmd, defined in src/go.mod and
  src/cmd/go.mod." "When a package outside std or cmd is imported by a package
  inside std or cmd," "the import path is interpreted as if it had a "vendor/"
  prefix." "When a package with the same path is imported from a package
  outside std or cmd, it will be resolved normally." "a binary may be built
  with two copies of a package at different versions". "all dependencies in
  the std and cmd modules are updated to their latest available versions" "at
  least twice during every major release cycle." "During the release freeze,
  the bar for changes in vendored packages is the same as it is for changes"
  "in non-vendored packages." "Applying changes to packages that are vendored
  follows the considerations written down at go.dev/s/release." The update
  route, as the reader rendered it: `go get`, `go mod tidy`, `go mod vendor`,
  then `go test cmd/internal/moddeps`.
- *verified*, **the check that holds the copies to the manifest**,
  https://raw.githubusercontent.com/golang/go/master/src/cmd/internal/moddeps/moddeps_test.go,
  read 2026-11-10: "TestAllDependencies ensures dependencies of all modules in
  GOROOT are in a consistent state." "In short mode, it does a limited quick
  check and stops there." "In long mode, it also makes a copy of the entire
  GOROOT tree and requires network access to perform more thorough checks."
  "This short test does NOT ensure that the vendored contents match the
  unmodified contents of the corresponding dependency versions." "This test
  will primarily run on longtest builders, since few people would test the
  cmd/internal/moddeps package directly, and all.bash runs tests in short
  mode." "TestDependencyVersionsConsistent verifies that each module in GOROOT
  that requires a given external dependency requires the same version of that
  dependency." "We want this test to be able to run offline and with an empty
  module cache, so we verify consistency only for the module versions listed
  in vendor/modules.txt."
- *verified*, **what a build does with a vendor directory**,
  https://go.dev/ref/mod, read 2026-11-10: "When vendoring is enabled, build
  commands like `go build` and `go test` load packages from the `vendor`
  directory instead of accessing the network or the local module cache."
  "When the `go` command reads `vendor/modules.txt`, it checks that the module
  versions are consistent with `go.mod`." "`-mod=vendor` tells the `go`
  command to use the `vendor` directory. In this mode, the `go` command will
  not use the network or the module cache." "`go mod verify` checks that
  dependencies of the main module stored in the module cache have not been
  modified since they were downloaded." "it does not use `go.sum` files to
  verify module content." The sections read say nothing of a build hashing
  vendored files; that it does not is an inference from that silence, unrun.
- *verified*, **the bootstrap**, https://go.dev/doc/install/source, read
  2026-11-10: "The Go toolchain is written in Go. To build it, you need a Go
  compiler installed." "Go <= 1.4: a C toolchain." "1.5 <= Go <= 1.19: a Go
  1.4 compiler." "1.20 <= Go <= 1.21: a Go 1.17 compiler." "1.22 <= Go <=
  1.23: a Go 1.20 compiler." "Going forward, Go version 1.N will require a Go
  1.M compiler, where M is N-2 rounded down to an even number." The page does
  not say the bootstrap fetches nothing; that the vendored copies travel in the
  tree it builds is README.vendor's, so a reading.
- *verified*, **the toolchain fetching a newer toolchain of itself**,
  https://go.dev/doc/toolchain, read 2026-11-10: "These toolchains are
  packaged as special modules with module path `golang.org/toolchain`"
  "Toolchains are downloaded like any other module, meaning that toolchain
  downloads can be proxied by setting `GOPROXY` and have their checksums
  checked by the Go checksum database." "it is not practical to write
  toolchain module checksums to `go.sum`." "Instead, toolchain downloads fail
  for lack of verification if `GOSUMDB=off`."

### Rust (rustc): both; a speed-bump list for the compiler, crates.io crates inside `std`

- *verified*, **depends**, the rustc-dev-guide's *crates.io dependencies*,
  https://rustc-dev-guide.rust-lang.org/crates-io.html, read 2026-11-10: "The
  Rust compiler supports building with some dependencies from `crates.io`."
  "To add a dependency that is not already in the compiler, you will need to
  add it to the list." (The page's other sentences came back paraphrased; a
  search answer says a 2025 revision reads that the compiler has *no official
  vetting policy*, which this seat did not read on the page: unverified.)
- *verified*, **the list**,
  https://raw.githubusercontent.com/rust-lang/rust/master/src/tools/tidy/src/deps.rs,
  read 2026-11-10: "Crates rustc is allowed to depend on. Avoid adding to the
  list if possible." "This list is here to provide a speed-bump to adding a new
  dependency to" "rustc. Please check with the compiler team before adding an
  entry." Of licences: "These are exceptions to Rust's permissive licensing
  policy, and" "should be considered bugs. Exceptions are only allowed in Rust"
  "tooling. It is _crucial_ that no exception crates be dependencies" "of the
  Rust runtime (std/test)." The reader counted *roughly 230* entries in
  `PERMITTED_RUSTC_DEPENDENCIES` (its approximate count, not this seat's).
- *verified*, **the policy**, Rust Forge, *Third-party and Out-of-tree Crates
  Policy*, https://forge.rust-lang.org/compiler/third-party-out-of-tree.html,
  read 2026-11-10: "It is sometimes desirable to use the functionality of an
  existing third-party crate in the compiler." "It is desirable that a
  third-party crate being included in the compiler is well-maintained and
  that," "where possible, a compiler team member is added as a maintainer."
  "You should consult with the rest of the compiler team before making this
  decision." And the other direction, parts of the compiler extracted into
  published crates: "When considering creating an out-of-tree crate, it is
  worth balancing how general the crate should be" "with the increased
  maintenance burden that this may bring if widely used." Required of such a
  crate, among others: "Follow semantic versioning." "The compiler team should
  be added as owners of the crate on crates.io." Listed: `rust-lang/chalk`,
  `rust-lang/polonius`, `rust-lang/measureme`, `rust-lang/thorin`.
- *verified*, **embeds**: `std` is built from registry crates,
  https://raw.githubusercontent.com/rust-lang/rust/master/library/std/Cargo.toml,
  read 2026-11-10: `hashbrown = { version = "0.17.1", default-features = false,
  features = ['rustc-dep-of-std'] }`, `cfg-if`, `rustc-demangle` (under
  "# Dependencies of the `backtrace` crate"), `libc = { version = "0.2.191",
  ..., features = ['rustc-dep-of-std'], public = true }` outside MSVC Windows,
  `miniz_oxide`, `addr2line`, `object`, and per-target `dlmalloc`, `wasip1`,
  `wasip2`, `wasip3`, `r-efi`, `hermit-abi` among others.
- *verified*, **what its history corrected (1)**: rust-lang/rust#141229,
  "Merge `compiler-builtins` as a Josh subtree",
  https://github.com/rust-lang/rust/pull/141229, read 2026-11-10: "Use the
  Josh [1] utility to add `compiler-builtins` as a subtree, which will allow us
  to stop using crates.io for updates." "This is intended to help resolve some
  problems when unstable features change and require" "code changes in
  `compiler-builtins`, which sometimes gets trapped in a bootstrap cycle."
  Merged "Jun 3, 2025", milestone "1.89.0".
- *verified*, **what its history corrected (2)**: rust-lang/rust#128534, "Move
  the standard library to a separate workspace", by bjorn3,
  https://github.com/rust-lang/rust/pull/128534, read 2026-11-10: "This
  ensures that the Cargo.lock packaged for it in the rust-src component is
  up-to-date"; it "simplifies tidy's license check for runtime dependencies"
  (fragments as the reader returned them). Merged August 4, 2024, milestone
  1.82.0.
- *verified*, **vendoring and the bootstrap**,
  https://raw.githubusercontent.com/rust-lang/rust/master/bootstrap.example.toml,
  read 2026-11-10: "Indicate whether the vendored sources are used for Rust
  dependencies or not." "Vendoring requires additional setup. We recommend
  using the pre-generated source tarballs if you" "want to use vendoring."
  Default: `if "is a tarball source" && "vendor" dir exists &&
  ".cargo/config.toml" file exists` `{ true } else { false }`. `locked-deps`:
  "Force Cargo to check that Cargo.lock describes the precise dependency" "set
  that all the Cargo.toml files create, instead of updating it." The stage-0
  compiler: "Instead of downloading the src/stage0 version of the compiler"
  "specified, use this rustc binary instead as the stage0 snapshot compiler."

### Cargo: depends on registry crates, binds libcurl and libgit2

- *verified*, https://raw.githubusercontent.com/rust-lang/cargo/master/Cargo.toml,
  read 2026-11-10, `[workspace.dependencies]`: `curl = "0.4.49"`,
  `curl-sys = "0.4.88"`, `git2 = { version = "0.21.0", features = ["https",
  "ssh"] }`, `git2-curl`, `gix = { version = "0.90.0", ... }`,
  `libgit2-sys = "0.18.5"`, `crates-io = { version = "0.41.3", path =
  "crates/crates-io" }`, `openssl = "0.10.76"`, `sha2`, `tar`, `flate2`; the
  `cargo` package itself: `curl = { workspace = true, features = ["http2"] }`,
  `git2.workspace = true`, `gix.workspace = true`, `crates-io.workspace =
  true`. So Cargo is built from crates of the registry it serves, and its
  registry client (`crates-io`) is a crate with a version that lives in its
  tree by path.
- *verified*, **what a vendored copy is held to**, *Source Replacement*,
  https://doc.rust-lang.org/cargo/reference/source-replacement.html, read
  2026-11-10: "Each crate in a directory source also has an associated
  metadata file `.cargo-checksum.json` to protect against accidental
  modifications. It is not a security mechanism and does not protect against
  malicious changes." "Cargo has a core assumption about source replacement
  that the source code is exactly the same from both sources."
- Embeds: none found; Cargo ships no library to programs (not searched
  further).

### Zig: takes no package; its manifest says so

- *verified*, https://codeberg.org/ziglang/zig/raw/branch/master/build.zig.zon,
  read 2026-11-10: "// The Zig compiler is not intended to be consumed as a
  package." "// The sole purpose of this manifest file is to test the
  compiler." Its one dependency is `.standalone_test_cases = .{ .path =
  "test/standalone" }`, a path in its own tree.
- *verified*, **the bootstrap**, Andrew Kelley, *Goodbye to the C++
  Implementation of Zig*, December 07, 2022, https://ziglang.org/news/goodbye-cpp/,
  read 2026-11-10: the idea is to "use a minimal wasm binary as a stage1 kernel
  that is committed to source control"; "We provide a minimal WASI interpreter
  implementation that is built from C source."; "The C code is then compiled
  and linked, again by the system C compiler, into a stage2 binary." The binary
  is "not source code, but is in fact a build artifact." Until then "the
  building from source process did not involve any binary blobs, except for a
  system C/C++ compiler." (fragments as returned).
- It depends on no package; its library ships beside the compiler (panel 234's
  historian report, the README's walk up to `lib/`, read 2026-11-05, not
  re-read today).

### Swift: depends on swift-syntax, a sibling checkout pinned by branch name, not resolved by SwiftPM

- *verified*,
  https://raw.githubusercontent.com/swiftlang/swift/main/utils/update_checkout/update-checkout-config.json,
  read 2026-11-10: `"swift-syntax": { "remote": { "id": "swiftlang/swift-syntax" } }`;
  in the `main` scheme `"swift-syntax": "main"`, in `release/6.2`
  `"swift-syntax": "release/6.2"`. As the reader rendered it, the main scheme
  pins by branch or tag, one entry (`boringssl`) by commit hash.
- *verified*, https://raw.githubusercontent.com/swiftlang/swift/main/CMakeLists.txt,
  read 2026-11-10: `option(SWIFT_BUILD_SWIFT_SYNTAX "Enable building swift
  syntax" FALSE)`; "# Whether the compiler's own Swift sources are built."
  `if(SWIFT_INCLUDE_TOOLS AND SWIFT_BUILD_SWIFT_SYNTAX AND
  SWIFT_ENABLE_SWIFT_IN_SWIFT)`.
- *verified*, **where its history is going**, Douglas Gregor, *RFC: Requiring
  Swift to build the Swift toolchain*, August 8, 2024,
  https://forums.swift.org/t/rfc-requiring-swift-to-build-the-swift-toolchain/73836,
  read 2026-11-10: "the gap between the "builds only with C++ code" and
  "builds with a host Swift toolchain" has grown considerably"; "macros,
  Embedded Swift, driver features, and many new optimization passes are written
  in Swift"; steps: "Introduce a CMake error when building the Swift compiler
  without a host Swift toolchain, only on `main`." "Remove the ability to
  disable the use of swift-syntax in the compiler build." "Start deleting the
  C++ implementations that are only used when there is no host Swift
  toolchain". A reply asks to keep Swift 5.10 as the minimum until "we have a
  reliable cross-compiling story and can bring up new compilers on new
  platforms from one that has an existing compiler easily."
- *unverified*: that programs reach the same swift-syntax through SwiftPM to
  write macros (search results only; no primary page read).

### Gleam: the compiler takes no Gleam package; the standard library is a package programs add

- *verified*, https://raw.githubusercontent.com/gleam-lang/stdlib/main/README.md,
  read 2026-11-10: "Gleam's standard library!"; "Add `gleam_stdlib` to your
  Gleam project." with `gleam add gleam_stdlib@1`. The page does not itself
  say the library is not bundled with the compiler; that it is fetched is my
  reading of `gleam add`. Whether the Gleam compiler embeds any other library
  (a prelude, for instance) was not looked for.
- *verified*, the compiler is Rust and its CLI depends on crates,
  https://raw.githubusercontent.com/gleam-lang/gleam/main/compiler-cli/Cargo.toml,
  read 2026-11-10 (Q2 below).

### Nim: the compiler's hash came out of the standard library into a package, and the compiler takes it by a pinned clone, outside nimble

- *verified*, the move, Nim 2.0.0's changelog,
  https://raw.githubusercontent.com/nim-lang/Nim/devel/changelogs/changelog_2_0_0.md,
  read 2026-11-10: "Several standard library modules have been moved to nimble
  packages, use `nimble` or `atlas` to install them:", among them "`std/md5`
  => `checksums/md5`" and "`std/sha1` => `checksums/sha1`". The page gives no
  reason.
- *verified*, **the compiler imports it by path**,
  https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/extccomp.nim,
  read 2026-11-10: `import ../dist/checksums/src/checksums/sha1`. The
  compiler's `nim.cfg`
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/nim.cfg, read
  2026-11-10) adds no `dist/` path.
- *verified*, **the build tool fetches it at a pinned commit**,
  https://raw.githubusercontent.com/nim-lang/Nim/devel/koch.nim, read
  2026-11-10: `ChecksumsStableCommit = "5c132cd332cce5d64a0da9ac3e4c9664313dccb4" # 0.2.2`;
  `proc bundleChecksums(latest: bool) =` `let checksumsCommit = if latest:
  "HEAD" else: ChecksumsStableCommit` `cloneDependency(distDir,
  "https://github.com/nim-lang/checksums.git", checksumsCommit, allowBundled =
  true)`, called for `boot`, `bootic`, `temp`, `zip` and `nsis`
  (`bundleChecksums(false) # boot`). In `boot`: "--noNimblePath precludes
  nimble packages as dependencies to the compiler," "so libffi is not
  "installed as a nimble package"". And of libffi: "technically a nimble
  package, however to play nicely with --noNimblePath," "let's just clone it
  wholesale:". `distDir = "dist"`.
- *verified*, **the seed carries the copy of its own day**,
  https://github.com/nim-lang/csources_v2, read 2026-11-10: "CSources compiled
  from Nim version 2."; the listing of
  https://github.com/nim-lang/csources_v2/tree/master/c_code/1_1, read
  2026-11-10, holds `@m..@slib@sstd@ssha1.nim.c` and no name with `checksums`
  or `dist`. That the seed was generated while the compiler still imported
  `std/sha1` is my reading of the file's name, not a statement of the page.

### pip: a package manager that carries, as copies, the packages it could install for itself

- *verified*, pip's vendoring policy,
  https://raw.githubusercontent.com/pypa/pip/main/src/pip/_vendor/README.rst,
  read 2026-11-10: "Vendored libraries MUST not be modified except as required
  to successfully vendor them." "Vendored libraries MUST be released copies of
  libraries available on PyPI." "The versions of libraries vendored in pip MUST
  be reflected in pip/_vendor/vendor.txt." "Vendored libraries MUST function
  without any build steps such as 2to3 or compilation of C code," "practically
  this limits to single source 2.x/3.x and pure Python." "Any modifications
  made to libraries MUST be noted in pip/_vendor/README.rst".
- *verified*, **why**, same page: "Historically pip has not had any
  dependencies except for setuptools itself," "choosing instead to implement
  any functionality it needed to prevent needing a dependency." "However,
  starting with pip 1.5, we began to replace code that was implemented inside
  of pip" "with reusable libraries from PyPI." "When pip depends on another
  library to function then if for whatever reason that library either isn't
  installed" "or an incompatible version is installed then pip ceases to
  function." "Obviously, when pip can't run, you can't use pip to fix pip, so
  you're left having to manually resolve dependencies" "and installing them by
  hand." "It forced pip to re-implement its own HTTPS verification routines as
  a workaround for the Python standard library's" "lack of SSL validation,
  which resulted in similar bugs in the validation routine in requests and
  urllib3," "except that they had to be discovered and fixed independently."
  "Currently most popular methods of installing pip rely on pip's
  self-contained nature to install pip itself." Of debundling: "Note that
  partial debundling is NOT supported."
- *verified*, what it carries,
  https://raw.githubusercontent.com/pypa/pip/main/src/pip/_vendor/vendor.txt,
  read 2026-11-10: `CacheControl`, `distlib`, `distro`, `msgpack`,
  `packaging`, `platformdirs`, `pyproject-hooks`, `requests==2.34.2` with
  `certifi`, `idna`, `urllib3==2.8.0`, `rich` with `pygments`, `resolvelib`,
  `setuptools`, `tomli`, `tomli-w`, `truststore==0.10.5`.

### Q1, read across

| toolchain | depends on its ecosystem's packages for its own code | embeds its ecosystem's packages in what it ships to programs | how they reach it | a rule stated |
|---|---|---|---|---|
| Go | yes (`cmd`: `x/mod`, `x/tools`, ...) | yes (`std`: `x/crypto`, `x/net`) | vendored in the tree, `go.mod` beside it | README.vendor: how, not whether |
| rustc | yes (~230 permitted, the reader's count) | yes (`std`: `hashbrown`, `libc`, ...) | `Cargo.lock`; vendored in source tarballs | tidy's list, "a speed-bump"; Forge: consult the team |
| Cargo | yes (`curl`, `git2`, `gix`, `crates-io`) | none found | `Cargo.lock` | none found |
| Zig | no | no package (a library beside the compiler) | none | "not intended to be consumed as a package" (the compiler as a package) |
| Swift | yes (swift-syntax) | not read | a sibling checkout by branch name | an RFC to make it mandatory |
| Gleam | no Gleam package (Rust crates) | the standard library is a package a program adds | crates by Cargo | none found |
| Nim | yes (`checksums`) | it moved modules out of its library in 2.0 | a clone at a pinned commit by the build tool, outside nimble | `--noNimblePath` "precludes nimble packages as dependencies to the compiler" |
| pip | yes (`requests`, `packaging`, ...) | n/a | vendored in the tree, `vendor.txt` | the vendoring policy: how, not whether |

**No page read states a rule like design.md:570-572 against a dependency.**
The nearest is rustc's: a list that is "a speed-bump", not a ban. Nim's
`--noNimblePath` sentence refuses the package **manager** as the compiler's
route while the compiler takes the package's **code**. The rule's purpose as
the sentence gives it (*such source is the standard library this section
forbids*) is the **embedding** act; Go and Rust do it, and Gleam's standard
library is a package a program adds.

**For (ii), the verifier among the verified.** No toolchain read has its
verifier's copy checked by that verifier when the toolchain is built. Go's
checksum client is a vendored copy whose default check "does NOT ensure that
the vendored contents match", the full check running on builders with the
network; a **newer** Go toolchain is verified by the **older** one's checksum
database client and refused with `GOSUMDB=off`. pip's policy exists because
"you can't use pip to fix pip". Rust moved `compiler-builtins` into its tree
to escape a dependency that "sometimes gets trapped in a bootstrap cycle".
Cargo calls its own vendored-copy check "not a security mechanism". That none
of them does it is a statement about the pages read, not about the world.

---

## Q2. A package manager's transport: programs or bound libraries

| system | `git` | HTTP / TLS | source, read 2026-11-10 |
|---|---|---|---|
| Go | a program, with hg, svn, bzr, fossil; restricted by `GOVCS` | the `GOPROXY` protocol; which HTTP library: not read | go.dev/ref/mod; golang/go#41730 |
| Cargo | **binds** libgit2 (`git2`) and gitoxide (`gix`); the program by opt-in | **binds** libcurl (`curl` with `http2`), `openssl` off Windows | cargo's Cargo.toml; config and git-authentication pages |
| uv | a program ("`git` CLI installed on the system") | bundled Mozilla roots by default; which library: not read | uv-git's git.rs; uv's settings page |
| pip | a program ("requires a working executable") | vendored `requests` and `urllib3`, `truststore` | pip's VCS page; vendor.txt |
| npm | a program (`@npmcli/git` "spawns child processes") | not read | npm/git's README |
| Gleam | not read (no git library in its CLI's manifest) | **binds** `reqwest` with `rustls` | gleam's compiler-cli/Cargo.toml |

- *verified*, Go, https://go.dev/ref/mod, read 2026-11-10: "Git, Subversion,
  Mercurial, Bazaar, and Fossil are supported." "A version control tool must
  be installed in a directory in `PATH` in order for the `go` command to use
  it." "The `go` command's ability to download modules with version control
  commands like `git` is critical to the decentralized package ecosystem, in
  which code can be imported from any server. It is also a potential security
  problem if a malicious server finds a way to cause the invoked version
  control command to run unintended code." "To balance the functionality and
  security concerns, the `go` command by default will only use `git` and `hg`
  to download code from public servers." Default "`public:git|hg,private:all`".
  **What moved it**, golang/go#41730, "cmd/go: allow users to control VCS
  usage", https://github.com/golang/go/issues/41730, read 2026-11-10: "The go
  command runs commands like git and hg to download modules." "In the past, we
  have had problems with security bugs in version control systems becoming
  security bugs in “go get”." Accepted October 28, 2020, milestone Go1.16,
  closed November 9, 2020 by commit 979e137.
- *verified*, Cargo's program beside its library, *Configuration*,
  https://doc.rust-lang.org/cargo/reference/config.html, read 2026-11-10,
  `net.git-fetch-with-cli`, default `false`: "If this is `true`, then Cargo will
  use the `git` executable to fetch registry indexes and git dependencies. If
  `false`, then it uses a built-in `git` library." "Setting this to `true` can
  be helpful if you have special authentication requirements that Cargo does
  not support." And `http.proxy`: "The format is in libcurl format". *Git
  Authentication*, https://doc.rust-lang.org/cargo/appendix/git-authentication.html,
  read 2026-11-10: "SSH configuration files (like OpenSSH's `~/.ssh/config`)
  are not used by Cargo's built-in SSH library. More advanced requirements
  should use `net.git-fetch-with-cli`." "Cargo does not support git's
  shorthand SSH URLs like `git@example.com:user/repo.git`." "Cargo doesn't
  support the `@cert-authority` or `@revoked` markers in `known_hosts` files.
  To make use of this functionality, use `net.git-fetch-with-cli`. This is
  also a good tip if Cargo's SSH client isn't behaving the way you expect it
  to."
- *verified*, **what is moving Cargo**, rust-lang/cargo#11813, "Tracking Issue
  for `-Zgitoxide`", opened Mar 9, 2023, open,
  https://github.com/rust-lang/cargo/issues/11813, read 2026-11-10: "all of the
  specified git operations will be performed by the `gitoxide` crate instead
  of `git2`" "This allows `git2` to be replaced in full." Its original issues,
  linked by title: "slow on `file://`", "slow on fetch", "high CPU usage",
  "high memory usage", "publish failure due to git-ignored `.DS_Store`". The
  page states no criticism of libgit2 in its own words.
- *verified*, uv,
  https://raw.githubusercontent.com/astral-sh/uv/main/crates/uv-git/src/git.rs,
  read 2026-11-10: "//! Git support is derived from Cargo's implementation.";
  "/// Attempts to use `git` CLI installed on the system to fetch a
  repository."; "// TODO(ibraheem): see if this still necessary now that we no
  longer use libgit2". When and why uv left libgit2: a mirror's commit says
  *moved all git functionality to the git CLI, dropping libgit2 entirely*
  (search result only; the primary pull request was not read): unverified.
  TLS, https://docs.astral.sh/uv/reference/settings/, read 2026-11-10: "By
  default, uv uses bundled Mozilla root certificates." `native-tls`: "This
  option has been deprecated. use `system-certs` instead".
- *verified*, pip, https://pip.pypa.io/en/stable/topics/vcs-support/, read
  2026-11-10: "Pip supports installing from various version control systems
  (VCS)." "This support requires a working executable to be available (for the
  version control system being used)."
- *verified*, npm, https://raw.githubusercontent.com/npm/git/main/README.md,
  read 2026-11-10: "A utility for spawning git from npm CLI contexts." "This is
  _not_ an implementation of git itself, it's just a thing that spawns child
  processes"; the `git` option: "Path to the `git` binary to use. Will look up
  the first `git` in the `PATH` if not specified."
- *verified*, Gleam,
  https://raw.githubusercontent.com/gleam-lang/gleam/main/compiler-cli/Cargo.toml,
  read 2026-11-10: `reqwest = { version = "^0.13", default-features = false,
  features = ["rustls"] }`, `rustls-native-certs = "0.8"`, `hexpm = { path =
  "../hexpm" }`; the reader found no `git2`, `gix` or line naming the git
  executable. Gleam v1.9.0, 08 March, 2025 (https://gleam.run/news/hello-echo-hello-git/,
  read 2026-11-10): "the build tool now supports depending on packages within
  git repositories." How it fetches them: not stated on the page; unverified.

**Read across.** Of the five whose git route was read, `git` is a program in
Go, uv, pip and npm; Cargo alone binds a git library, offers the program where
its library falls short (SSH configuration, `known_hosts` markers,
authentication), and has had an open issue since March 2023 to replace that
library with another. That is the record panel 235's R10 (*never libgit2*)
already meets. For the HTTP read, none of the six runs `curl` as a program in
what was read; Cargo reads through libcurl, Gleam through `reqwest`, pip
through vendored `requests`. That is a fact of these six, and it does not by
itself argue for route (c): Heroes' costs for libcurl (the seed's line in 336
files, the Windows leg without the library, `heroes` linking `libSystem`
alone, the critic's item 7) are not costs any of them names, and none of them
is a compiler with a C seed.

---

## Q3. A child's environment without writing the parent's

### The three standard libraries the brief names

- *verified*, Go, https://pkg.go.dev/os/exec, read 2026-11-10, `Cmd.Env`:
  "Env specifies the environment of the process." "Each entry is of the form
  "key=value"." "If Env is nil, the new process uses the current process's
  environment." "If Env contains duplicate environment keys, only the last
  value in the slice for each duplicate key is used." "As a special case on
  Windows, SYSTEMROOT is always added if missing and not explicitly set to the
  empty string." `Cmd.Environ` (Go 1.19): "Environ returns a copy of the
  environment in which the command would be run as it is currently
  configured."
- *verified*, Rust, https://doc.rust-lang.org/std/process/struct.Command.html,
  read 2026-11-10, `Command::env`: "This method allows you to add an environment
  variable mapping to the spawned process or overwrite a previously set value."
  "Child processes will inherit environment variables from their parent process
  by default. Environment variables explicitly set using `Command::env` take
  precedence over inherited variables. You can disable environment variable
  inheritance entirely using `Command::env_clear` or for a single key using
  `Command::env_remove`." "Note that environment variable names are
  case-insensitive (but case-preserving) on Windows and case-sensitive on all
  other platforms." `env_remove`: it "will prevent the spawned child process
  from inheriting that environment variable from its parent process."
- *verified*, Python, https://docs.python.org/3/library/subprocess.html, read
  2026-11-10: "If _env_ is not `None`, it must be a mapping that defines the
  environment variables for the new process; these are used instead of the
  default behavior of inheriting the current process’ environment." "On
  Windows, in order to run a side-by-side assembly the specified _env_ **must**
  include a valid `%SystemRoot%`." "If you need to modify the environment for
  the child use the _env_ parameter rather than doing it in a _preexec\_fn_."
  So Python's `env` **replaces** the inherited environment, Go's `Env` does too
  when not nil, and Rust's `env` **adds** to it; `hero_run_env(name, value)` as
  route (e) describes it is Rust's shape (my reading of the three pages).

### Two package managers that put git's variables on the child

- *verified*, uv, https://raw.githubusercontent.com/astral-sh/uv/main/crates/uv-git/src/git.rs,
  read 2026-11-10: `cmd.env(EnvVars::GIT_TERMINAL_PROMPT, "0");` on the git
  `Command`, and `cmd.env_remove(EnvVars::GIT_DIR)`, `GIT_WORK_TREE`,
  `GIT_INDEX_FILE`, `GIT_OBJECT_DIRECTORY`,
  `GIT_ALTERNATE_OBJECT_DIRECTORIES`, `GIT_COMMON_DIR`. The reader found no
  `GCM_INTERACTIVE`.
- *verified*, npm, https://raw.githubusercontent.com/npm/git/main/lib/opts.js,
  read 2026-11-10, as the reader rendered it (no quotation returned): it sets
  `GIT_ASKPASS` to `'echo'` and `GIT_SSH_COMMAND` to `'ssh
  -oStrictHostKeyChecking=accept-new'` in a `finalGitEnv` object, "not into
  `process.env`", each only where the user has not set it, and the child's
  `env` is `opts.env || { ...finalGitEnv, ...process.env }`.

### Go writes the same two variables Heroes writes, into its own process, where unset

- *verified*, https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/modload/init.go,
  read 2026-11-10, in `Init(ld *Loader)`:

  ```go
  // Disable any prompting for passwords by Git.
  // Only has an effect for 2.3.0 or later, but avoiding
  // the prompt in earlier versions is just too hard.
  // If user has explicitly set GIT_TERMINAL_PROMPT=1, keep
  // prompting.
  // See golang.org/issue/9341 and golang.org/issue/12706.
  if os.Getenv("GIT_TERMINAL_PROMPT") == "" {
  	os.Setenv("GIT_TERMINAL_PROMPT", "0")
  }

  if os.Getenv("GCM_INTERACTIVE") == "" {
  	os.Setenv("GCM_INTERACTIVE", "never")
  }
  ```

  These are the variables `fetch/transport.hero:160-161` writes
  (`GIT_TERMINAL_PROMPT=0`, `GCM_INTERACTIVE=never`, the critic's item 5).
  `src/cmd/go/main.go`, read the same day, holds none of the three names.
  Whether Heroes' write leaves a value the user set in place, as Go's does, was
  not read by this seat.
- *verified*, golang/go#9341, "cmd/go: invoke git without terminal, so it
  cannot ask for password", https://github.com/golang/go/issues/9341, read
  2026-11-10, opened Dec 16, 2014: "When doing `go get ...` if the repository
  has been renamed/deleted by the owner" "the go get progress will stuck and
  asking for Username and Password"; closed Jul 15, 2015, commit 8df0bcc.
- *verified*, **the correction**, golang/go#12706, "cmd/go: do not replace
  existing $GIT_TERMINAL_PROMPT", https://github.com/golang/go/issues/12706,
  read 2026-11-10, opened Sep 21, 2015: "The go get command was changed in 1.5
  to disable git prompts universally (see issue #9341). This change reduced
  choice for how people want to work with the go get command"; rsc proposed to
  "change the go command to leave any existing $GIT_TERMINAL_PROMPT in place";
  closed Oct 19, 2015, commit 4dc49d2.
- *unverified*: when `GCM_INTERACTIVE` entered and which issue moved it (a
  mirror's commit names issue 44904; the primary issue was not read).

### What the histories say about writing the environment beside C's `getenv`

- *verified*, Go, golang/go#63567, "os: Setenv leads to SIGSEGV with
  GODEBUG=netdns=cgo, and other uses of cgo", opened Oct 16, 2023 by evanj,
  open, milestone Backlog, https://github.com/golang/go/issues/63567, read
  2026-11-10: "A program that uses the `netdns=cgo` resolver with glibc and
  calls `os.Setenv` in other goroutines can crash with `SIGSEGV`." "The
  problem is that `getaddrinfo` in glibc calls `getenv`". prattmic: "avoiding
  C `setenv` would be a breaking change." "this only matters for cgo programs
  calling `os.Setenv` while there are other concurrent goroutines or C-created
  threads." gopherbot: "Change https://go.dev/cl/538675 mentions this issue:
  `os: document that os.Setenv can crash Cgo programs`." The doc of
  `os.Setenv` today, https://pkg.go.dev/os#Setenv, read 2026-11-10 (its first
  100,000 characters): "Setenv sets the value of the environment variable
  named by the key. It returns an error, if any." with no warning.
- *verified*, why Go reaches C at all,
  https://raw.githubusercontent.com/golang/go/master/src/runtime/env_posix.go,
  read 2026-11-10: "// Update the C environment if cgo is loaded." over
  `setenv_c`, which calls `_cgo_setenv` through `asmcgocall`. On Windows,
  https://raw.githubusercontent.com/golang/go/master/src/syscall/env_windows.go,
  read 2026-11-10: `Setenv` calls `SetEnvironmentVariable(keyp, v)` and then
  `runtimeSetenv(key, value)`.
- *verified*, Rust, rust-lang/rust#27970, "set\_var/remove\_var are unsound in
  combination with C code that reads the environment", opened Aug 23, 2015,
  closed, https://github.com/rust-lang/rust/issues/27970, read 2026-11-10: "It
  looks like we have an unsynchronized call to getaddrinfo and this may cause
  trouble with glibc/Linux." "care must be taken with mutating environment
  variables in a multithreaded program."
- *verified*, **the correction**, the Rust 2024 edition guide,
  https://doc.rust-lang.org/edition-guide/rust-2024/newly-unsafe-functions.html,
  read 2026-11-10: "The following functions are now marked `unsafe`:"
  "`std::env::set_var`" "`std::env::remove_var`". "It can be unsound to call
  `std::env::set_var` or `std::env::remove_var` in a multithreaded program due
  to safety limitations of the way the process environment is handled on some
  platforms. The standard library originally defined these as safe functions,
  but it was later determined that was not correct." "You may need to elect to
  use a different mechanism other than environment variables to manage your use
  case." And `set_var`'s own page, https://doc.rust-lang.org/std/env/fn.set_var.html,
  read 2026-11-10: `pub unsafe fn set_var`; "This function is also always sound
  to call on Windows, in single-threaded and multi-threaded programs." "The
  problem is that these operating systems do not provide a thread-safe way to
  read the environment, and most C libraries, including libc itself, do not
  advertise which functions read from the environment." "To pass an environment
  variable to a child process, you can instead use `Command::env`." (When the
  2024 edition shipped was not read.)

### What Microsoft's pages say, for the Windows arm of panel 241's route

- *verified*, `_putenv_s`, `_wputenv_s`,
  https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/putenv-s-wputenv-s,
  read 2026-11-10: "You can remove a variable from the environment by
  specifying an empty string (that is, `""`) for *`value_string`*."
  "**`_putenv_s`** and **`_wputenv_s`** affect only the environment that is
  local to the current process; you can't use them to modify the command-level
  environment. These functions operate only on data structures that are
  accessible to the run-time library and not on the environment "segment" that
  the operating system creates for a process." "However, the modified
  environment can be passed to any new processes that are created by
  **`_spawn`**, **`_exec`**, or **`system`**, and these new processes get any
  new items that are added by **`_putenv_s`** and **`_wputenv_s`**." "The
  **`_putenv_s`** and **`_getenv_s`** families of functions are not
  thread-safe."
- *verified*, `CreateProcessA`,
  https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa,
  read 2026-11-10, `lpEnvironment`: "If this parameter is **NULL**, the new
  process uses the environment of the calling process." "If the environment
  block pointed to by *lpEnvironment* contains Unicode characters, be sure that
  *dwCreationFlags* includes **CREATE\_UNICODE\_ENVIRONMENT**." "The ANSI
  version of this function, **CreateProcessA** fails if the total size of the
  environment block for the process exceeds 32,767 characters." "A parent
  process can directly alter the environment variables of a child process
  during process creation. This is the only situation when a process can
  directly change the environment settings of another process." "If an
  application provides an environment block, the current directory information
  of the system drives is not automatically propagated to the new process."
- **What the two pages leave open, put as a question, unrun.** The CRT page
  names `_spawn`, `_exec` and `system` as receiving a `_wputenv_s` write and
  says the write does not touch "the environment "segment" that the operating
  system creates"; `CreateProcessA` with a NULL block takes "the environment of
  the calling process". Neither page says whether that environment includes a
  `_wputenv_s` write, and `runtime/parts/run.c:1359` starts children with
  `CreateProcessA` and no block (the shared brief). Go's Windows `Setenv`
  calls `SetEnvironmentVariable` itself (above). This is the box's to run, not
  this seat's to infer. And route (e)'s own cost on Windows has a page: a block
  given to `CreateProcess` does not carry the drives' current-directory
  entries unless the application writes them, sorted.

---

## Q4. The order, briefly

- *verified* (above): Nim's compiler takes a **hash** (`checksums/sha1`) as an
  ecosystem package, by a pinned clone outside its package manager, and its
  seed's listing holds the older `std/sha1`'s C.
- *verified* (above): pip began replacing its own code with PyPI libraries
  "starting with pip 1.5"; `requests` is the one its rationale names.
- No page read orders a path library before a hash, or the reverse. This seat
  gives no verdict on question 4.

---

## Candidate prior-art lines

For the `## Prior art` section of the decision that ratifies this sitting, in
the two forms `.claude/rules/records.md` allows. **Which form applies depends
on the synthesis**: *Taken from* where the resolution adopts a form on that
page's evidence, *Reached first by* where the author or a seat reached it and
the page was found after. A form the sitting does not adopt has no line. Each
says only what its page states. Where this seat read a file in its raw form,
the link below points at the same file's rendered page; another project's
issue or pull request goes through `redirect.github.com`.

**If question 1 reads R1's veto as not reaching a dependency of the compiler**:

- **Reached first by Go**: [src/README.vendor](https://github.com/golang/go/blob/master/src/README.vendor): "The Go command maintains copies of external packages needed by the standard library in the src/vendor and src/cmd/vendor directories." "There are two modules, std and cmd, defined in src/go.mod and src/cmd/go.mod."
- **Reached first by Rust**: [tidy's deps.rs](https://github.com/rust-lang/rust/blob/master/src/tools/tidy/src/deps.rs): "Crates rustc is allowed to depend on. Avoid adding to the list if possible." "This list is here to provide a speed-bump to adding a new dependency to rustc."

**If the form is a copy in the compiler's tree held by a separate check (route (C)'s shape)**:

- **Taken from Go**: [cmd/internal/moddeps/moddeps_test.go](https://github.com/golang/go/blob/master/src/cmd/internal/moddeps/moddeps_test.go): "TestAllDependencies ensures dependencies of all modules in GOROOT are in a consistent state." "In long mode, it also makes a copy of the entire GOROOT tree and requires network access to perform more thorough checks."
- **Taken from pip**: [src/pip/_vendor/README.rst](https://github.com/pypa/pip/blob/main/src/pip/_vendor/README.rst): "Vendored libraries MUST not be modified except as required to successfully vendor them." "Vendored libraries MUST be released copies of libraries available on PyPI."
- **Reached first by Nim**: [koch.nim](https://github.com/nim-lang/Nim/blob/devel/koch.nim): `ChecksumsStableCommit = "5c132cd332cce5d64a0da9ac3e4c9664313dccb4" # 0.2.2`; "--noNimblePath precludes nimble packages as dependencies to the compiler".

**If question 1 (ii) keeps the compiler's verifier out of what it verifies at its own build**:

- **Taken from pip**: [src/pip/_vendor/README.rst](https://github.com/pypa/pip/blob/main/src/pip/_vendor/README.rst): "Obviously, when pip can't run, you can't use pip to fix pip, so you're left having to manually resolve dependencies and installing them by hand."
- **Taken from Rust**: [rust-lang/rust#141229](https://redirect.github.com/rust-lang/rust/pull/141229): "Use the Josh [1] utility to add `compiler-builtins` as a subtree, which will allow us to stop using crates.io for updates." "code changes in `compiler-builtins`, which sometimes gets trapped in a bootstrap cycle."

**If question 2 keeps `git` a program (already panel 235's R10; a line only if this sitting restates it)**:

- **Reached first by Go**: [Go Modules Reference, GOVCS](https://go.dev/ref/mod): "The `go` command's ability to download modules with version control commands like `git` is critical to the decentralized package ecosystem, in which code can be imported from any server."
- **Reached first by Cargo**: [Git Authentication](https://doc.rust-lang.org/cargo/appendix/git-authentication.html): "If you need other authentication methods, the `net.git-fetch-with-cli` config value can be set to cause Cargo to execute the `git` executable to handle fetching remote repositories instead of using the built-in support."

**If question 3 adopts route (e), the child's environment at the spawn**:

- **Taken from Rust**: [std::env::set_var](https://doc.rust-lang.org/std/env/fn.set_var.html): "To pass an environment variable to a child process, you can instead use `Command::env`."; and [Rust 2024, newly unsafe functions](https://doc.rust-lang.org/edition-guide/rust-2024/newly-unsafe-functions.html): "The standard library originally defined these as safe functions, but it was later determined that was not correct."
- **Taken from Go**: [golang/go#63567](https://redirect.github.com/golang/go/issues/63567): "A program that uses the `netdns=cgo` resolver with glibc and calls `os.Setenv` in other goroutines can crash with `SIGSEGV`."
- **Reached first by uv**: [crates/uv-git/src/git.rs](https://github.com/astral-sh/uv/blob/main/crates/uv-git/src/git.rs): `cmd.env(EnvVars::GIT_TERMINAL_PROMPT, "0");` on the `git` command it runs.

**If question 3 keeps a write that leaves a variable the user set in place**:

- **Taken from Go**: [golang/go#12706](https://redirect.github.com/golang/go/issues/12706): "The go get command was changed in 1.5 to disable git prompts universally (see issue #9341). This change reduced choice for how people want to work with the go get command".

---

## argument

Precedent separates two acts. A toolchain depending on its ecosystem's code is
ordinary: Go, rustc, Cargo, Nim and pip do it, and none read bans it.
Embedding packages for programs is what Go's and Rust's standard libraries do;
Gleam's library is a package a program adds. The form they share is a copy in
the tree at a recorded version, held by a separate check. None read checks its
verifier with itself when built: Go's full check runs on builders, pip vendors
because "you can't use pip to fix pip", Rust took `compiler-builtins` off
crates.io over a bootstrap cycle. Rust made `set_var` unsafe and points to
`Command::env`; Go's process-wide write sits beside an open crash. Route (e)
follows.

## condition

This seat's reading would change on:

- **Q1**: a toolchain that took a package of its own ecosystem as a dependency
  and recorded that act, not the embedding, as how it came to ship a standard
  library it could not remove; or a page stating a ban on compiler
  dependencies in the words design.md:570-572 uses.
- **Q1 (ii)**: a toolchain whose build checks its own checksum code's copy with
  that same code, and records no bootstrap failure for it.
- **Q2**: a package manager that ran `curl` as a program for its index and moved
  to a bound library for a stated, measured cost, which would argue for (c)
  sooner; or one that moved from a bound HTTP library back to a program.
- **Q3**: a record of a system giving the child its environment at the spawn and
  meeting a failure the process-wide write would have avoided; or the box
  showing that `CreateProcessA` with no block does not see a `_wputenv_s`
  write, which would make panel 241's recorded route fail on its own terms.

## search

All pages were fetched on 2026-11-10 with the page-reading tool. The queries:

- **Go (Q1, Q2, Q3).** Pages: `src/cmd/go.mod`, `src/go.mod`,
  `src/cmd/vendor/modules.txt`, the `src/cmd/vendor/golang.org/x/mod` tree,
  `src/README.vendor` (three readings), `cmd/internal/moddeps/moddeps_test.go`,
  go.dev/ref/mod (Vendoring, `go mod vendor`, `go mod verify`, version control
  and GOVCS, from offsets 0 and 100000), go.dev/doc/install/source,
  go.dev/doc/toolchain, golang/go#41730, `src/cmd/go/main.go` (no git
  variable found), `src/cmd/go/internal/modload/init.go`, golang/go#9341 and
  #12706, pkg.go.dev/os/exec, pkg.go.dev/os#Setenv (first 100,000
  characters), `runtime/env_posix.go`, `syscall/env_windows.go`,
  golang/go#63567. `src/net/http/h2_bundle.go` answered 404 at `master`.
  Searches: `golang net/http h2_bundle.go removed HTTP/2 moved into standard
  library net/http/internal/http2 proposal` (mirrors only; not cited);
  `golang cmd/go source "GCM_INTERACTIVE" "never" os.Setenv
  "GIT_TERMINAL_PROMPT" "GIT_SSH_COMMAND"` (mirrors; the primary file found by
  reading `modload/init.go`); `golang issue os.Setenv cgo concurrent getenv
  crash segfault setenv not thread safe C library`. **Not found or not read**:
  which HTTP library the `go` command uses for the proxy
  (`cmd/go/internal/web` not fetched); the issue that added `GCM_INTERACTIVE`.
- **Rust and Cargo (Q1, Q2, Q3).** Pages: rustc-dev-guide `crates-io.html`
  and its source file (paraphrased by the reader; the 2025 wording not
  confirmed), `src/tools/tidy/src/deps.rs`, Rust Forge's third-party policy
  (three readings), `library/std/Cargo.toml`, rust-lang/rust#141229 and
  #128534, `bootstrap.example.toml`, cargo's `Cargo.toml`, `cargo vendor`,
  *Source Replacement*, *Configuration*, *Git Authentication*,
  rust-lang/cargo#11813, `std::env::set_var`, `std::process::Command`, the 2024
  edition's newly-unsafe functions, rust-lang/rust#27970.
  `src/cargo/sources/git/utils.rs` answered 404 at `master`, so whether Cargo's
  CLI path sets `GIT_TERMINAL_PROMPT` on the child is unread. Searches: `Rust
  Forge policy vetting new dependencies compiler crates.io rustc`;
  `rust-lang/rust compiler-builtins moved into library subtree crates.io
  publishing problem 2025`; `rust-lang/rust "library" separate workspace own
  Cargo.lock standard library dependencies pull request 2024` (a mirror; the
  primary PR #128534 read instead); `Rust issue set_var unsound getenv
  segfault "set_var" unsafe 2024 edition tracking issue 27970`.
- **Zig.** Pages: Codeberg's `build.zig.zon`; *Goodbye to the C++
  Implementation of Zig*.
- **Swift.** Pages: `update-checkout-config.json`, `CMakeLists.txt`, the 2024
  RFC thread. Searches: `Swift compiler build requires host Swift toolchain
  swift-syntax macros "SWIFT_BUILD_SWIFT_SYNTAX" bootstrapping
  forums.swift.org`; `SwiftPM prebuilt swift-syntax macros build time Swift 6.1
  forums announcement` (forum threads on macro build time; not read, not
  cited). **Not read**: a primary page that programs reach swift-syntax
  through SwiftPM.
- **Gleam.** Pages: `compiler-cli/Cargo.toml`, the stdlib's README, the v1.9.0
  announcement. Search: `Gleam git dependencies release notes "git" executable
  gleam.toml git = dependency version 1.9`. **Not found**: how Gleam fetches a
  git dependency (the announcement does not say; no source read); whether its
  compiler embeds any library.
- **Nim.** Pages: `koch.nim` (two readings), the 2.0.0 changelog (two
  readings), `compiler/nim.cfg`, `compiler/extccomp.nim`, the `csources_v2`
  README, its `c_code` and `c_code/1_1` listings.
- **pip, uv, npm.** Pages: pip's `_vendor/README.rst` (two readings),
  `vendor.txt`, *VCS Support*; uv's `crates/uv-git/src/git.rs`, uv's settings
  reference; `npm/git`'s README and `lib/opts.js`. Search: `uv docs git
  dependencies requires git command line "git" CLI installed astral uv reqwest
  rustls native-tls` (a mirror's commit on dropping libgit2; not cited as
  verified). **Not read**: uv's HTTP library and the pull request that dropped
  libgit2; npm's HTTP stack.
- **Windows.** Pages: Microsoft's `_putenv_s` reference and `CreateProcessA`
  reference.
- **Looked for and not found, across the above**: a toolchain stating a ban on
  its compiler depending on its ecosystem's packages (Go's README.vendor,
  rustc's tidy and Forge pages, Cargo's manifest, Zig's manifest, Swift's RFC,
  Nim's `koch.nim`, pip's policy); a package manager running `curl` as a
  program for its index (the six in Q2's table).
- **The lane.** Read: `../242-briefs/00-shared.md`, `../242-briefs/historian.md`,
  `critic-briefs.md`, `docs/design.md` lines 555-584, and the historian reports
  of panels 234 and 235 for their form. *Heroes of code* was not opened for
  this report.

report complete
