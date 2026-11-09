# Panel 240: historian

Read 2026-11-09, from about 22:15. No worktree, no compiler built, no paid run,
no credential printed. Every precedent below carries the URL and the day it was
read (2026-11-09); `verified` means the page itself says it. A precedent whose
page could not be read is not written. Quotes are as the fetch tool returned
them from the page; where it split a sentence, the split is kept. *Heroes of
code* was not used as a source for any line below.

## Question 1: how others spell "find this C library through pkg-config"

### What the program's text says, system by system

| System | What is written in the file that asks | Calls pkg-config? | Source, read 2026-11-09 | Status |
|---|---|---|---|---|
| Go, cgo | `// #cgo pkg-config: png cairo` in the comment before `import "C"` | yes: "Alternatively, CPPFLAGS and LDFLAGS may be obtained via the pkg-config tool using a '#cgo pkg-config:' directive followed by the package names" | https://pkg.go.dev/cmd/cgo | verified |
| Zig, `build.zig` | `linkSystemLibrary(name, options)`; the option is `use_pkg_config: SystemLib.UsePkgConfig = .yes`, values `no` ("Don't use pkg-config, just pass -lfoo"), `yes` ("Try to get information on how to link the library from pkg-config. If that fails, fall back to passing -lfoo"), `force` ("If that fails, error out") | yes, by default | https://raw.githubusercontent.com/ziglang/zig/master/lib/std/Build/Module.zig | verified (master, not a release tag) |
| Swift PM, `Package.swift` | `.systemLibrary(name:path:pkgConfig:providers:)`, e.g. `pkgConfig: "zlib"` | yes, the argument is named for the tool | https://github.com/swiftlang/swift-evolution/blob/main/proposals/0208-package-manager-system-library-targets.md (status "Implemented (Swift 4.2)") | verified |
| Haskell Cabal, `.cabal` | `pkgconfig-depends: gtk+-2.0 >= 2.10, cairo >= 1.0`, "A list of pkg-config packages, needed to build this package"; beside it, `extra-libraries`, "A list of extra libraries to link with" | yes: "Cabal uses pkg-config to find if the packages are available on the system" | https://cabal.readthedocs.io/en/latest/cabal-package-description-file.html | verified |
| Meson, `meson.build` | `dependency('zlib', version : '>=1.2.8')`; to force the tool, `dependency('cups', method : 'pkg-config')` | yes, first in the `auto` order ("pkg-config", then "cmake", then "extraframework") | https://mesonbuild.com/Dependencies.html | verified |
| CMake, `FindPkgConfig` | `pkg_check_modules(<prefix> [REQUIRED] [IMPORTED_TARGET] <module-spec>...)`, "Checks for all the given modules" | yes; a `<module-spec>` is "a bare module name (as defined in its PC metadata file name `<module-name>.pc`)" | https://cmake.org/cmake/help/latest/module/FindPkgConfig.html | verified |
| autoconf, `pkg.m4` | `PKG_CHECK_MODULES(VARIABLE-PREFIX, MODULES, ...)`, "Checks whether a given module set exists" | yes | https://www.mankier.com/7/pkg.m4 (the pkgconf-shipped man page) | verified |
| V | `#pkgconfig r_core` (same as `#pkgconfig --cflags --libs r_core`), "tell the compiler which modules should be used for compiling and linking using the pkg-config files"; beside it, `#flag -lsqlite3` | yes | https://docs.vlang.io/v-and-c.html | verified |
| Nim | no mark: `{.passl: gorge("pkg-config --libs sdl").}` and `{.passc: gorge("pkg-config --cflags sdl").}`, the tool run as a command "during semantic analysis" | by hand | https://nim-lang.org/docs/manual.html (passc and passl pragma sections) | verified |
| Crystal | `@[Link("pcre")]`: "will pass `-lpcre` to the linker, but the compiler will first try to use pkg-config" | yes, implicitly; the word in the text is `Link` | https://crystal-lang.org/reference/latest/syntax_and_semantics/c_bindings/lib.html | verified |
| Rust, in source | `#[link(name = "crypto")]`, "The `name` key is the name of the native library to link"; kinds `dylib`, `static`, `framework`, `raw-dylib` | the page does not mention pkg-config | https://doc.rust-lang.org/reference/items/external-blocks.html | verified, as the page's silence |
| Rust, `build.rs` | `pkg_config::probe_library("foo")`, "A build dependency for Cargo libraries to find system artifacts through the `pkg-config` utility"; the argument is "the system library named `foo`" | yes | https://docs.rs/pkg-config/latest/pkg_config/ | verified |
| Rust, `Cargo.toml` via `system-deps` | `[package.metadata.system-deps]` then `testlib = "1.2"`, "The libraries specified with this option can have any form readable by `pkg-config`" | yes | https://docs.rs/system-deps/latest/system_deps/ | verified. `package` there is Cargo's own package table, not the C sense |
| Odin | `foreign import bar "system:bar"` ("a system library, e.g. `libbar.so`, `bar.lib`") | the overview page, read whole in two parts, never mentions pkg-config | https://odin-lang.org/docs/overview/ (Foreign system section) | verified as the page's silence; whether the compiler calls pkg-config anywhere is **unverified** |

**No system in this sweep uses the bare word `package` as the mark in the
program's text.** The marks found are of three kinds:

- **A tool-named directive or argument**: `pkg-config:` in cgo, `pkgConfig:` in
  SwiftPM, `#pkgconfig` in V, `pkgconfig-depends` in Cabal, `use_pkg_config`
  in Zig, and `method : 'pkg-config'` in Meson. That is six of the fourteen
  rows, a count over this table and not over the world.
- **A generic word**: `dependency` in Meson, `Link` in Crystal, `system:` in
  Odin, `linkSystemLibrary` in Zig.
- **A *module* word**: CMake's `pkg_check_modules`, autoconf's
  `PKG_CHECK_MODULES`.

**The pattern that matters for § 13.** Where a system offers both a plain
link and a pkg-config route side by side, as Heroes does with `link` and the
§ 13 mark, the pkg-config route is named by the tool in every case in this
sweep:

- Cabal's `extra-libraries` beside `pkgconfig-depends`;
- V's `#flag -l...` beside `#pkgconfig`;
- cgo's flags beside `#cgo pkg-config:` ("Alternatively");
- Zig's `use_pkg_config = .no` beside `.yes` and `.force`;
- Meson's `method` values.

Crystal is the one that folds both into one mark (`Link`). Rust's in-source
attribute has no pkg-config route on its page.

**The pages give *system* to the plain route, not to pkg-config.** Odin's
`system:` names "a system library" with no pkg-config on its page, and Zig's
`linkSystemLibrary` covers both routes, choosing between them by an option.
So `system` as the § 13 mark would carry, in precedent, the meaning Heroes
gives `link`. That is an inference from two pages, said as one.

### What pkg-config itself calls the thing a `.pc` file describes

This bears on question 2 too, as the critic asked (d.9).

- **freedesktop pkg-config** (the original): NAME "pkg-config - Return
  metainformation about installed libraries"; "pkg-config retrieves
  information about packages from special metadata files"; "The package name
  specified on the pkg-config command line is defined to be the name of the
  metadata file"; `Requires` is "a comma-separated list of packages that are
  required by your package". It also says "modules" in places ("If one or more
  of the modules on the command line"). https://linux.die.net/man/1/pkg-config,
  read 2026-11-09. **verified**; the copy is old and the page as returned
  carries no date.
- **pkgconf**: its SYNOPSIS is `pkgconf [options] module ...`. Its DESCRIPTION
  says "The pkgconf program retrieves configuration information related to the
  module arguments" and "The module arguments correspond to the file names,
  but without the .pc filename extension". *package* survives only in
  `--list-package-names` and `--validate package ...`. Read 2026-11-09 at
  https://www.mankier.com/1/pkgconf and at
  https://man.archlinux.org/man/pkg-config.1.en (Arch serves pkgconf's page
  under the name `pkg-config`). **verified**.
- **Homebrew**: formula `pkgconf` 3.0.10, "Package compiler and linker metadata
  toolkit", "Also known as: pkg-config, pkgconfig", binaries listed include
  `pkg-config` and `pkgconf`. https://formulae.brew.sh/formula/pkgconf, read
  2026-11-09. **verified**. vcpkg's port was not read: **unverified**.
- **freedesktop's guide** (https://people.freedesktop.org/~dbn/pkg-config-guide.html)
  returned HTTP 403 and is not cited.

**What this means for the sitting.** *package* for this sense is the original
tool's own noun. cgo's documentation uses it too ("the package names"), and so
does Cabal's ("pkg-config packages"). pkgconf, CMake and autoconf say
*module*. Whether panel 050 chose its word for this reason, its record's first
80 lines (`docs/panel/050-the-package-clause.md`, read 2026-11-09) do not say.
They say the mark asks the machine "through `pkg-config`".

The tool's own noun is therefore split between *package* and *module*, and
**both are words the spec already gives to something else** (spec § 1, per
00-shared.md). This is the precedent-side reason to name the mechanism rather
than the thing. Five of five side-by-side systems above did so.

## Question 2: which ecosystems call a file a module and the fetched unit a package

| Ecosystem | One file is called | The fetched, versioned unit is called | Source, read 2026-11-09 | Status |
|---|---|---|---|---|
| Python | a **module**: "A module is a file containing Python definitions and statements." | a **package**, but the word is double. The language's own package is "a way of structuring Python's module namespace by using 'dotted module names'". PyPA's glossary separates the "Distribution Package", "A versioned archive file ... what an end-user will download from the internet and install", "more commonly referred to with the single words 'package' or 'distribution'", from the "Import Package", "(which is also commonly called a 'package')". | https://docs.python.org/3/tutorial/modules.html ; https://packaging.python.org/en/latest/glossary/ | verified |
| npm / Node | a **module**, roughly: "A module is any file or directory in the node_modules directory that can be loaded by the Node.js require() or import syntax." | a **package**: "A package is a file or directory that is described by a package.json file." "A package must contain a package.json file in order to be published to the npm registry." | https://docs.npmjs.com/about-packages-and-modules | verified |
| Dart | a **library**, not a module: "Every Dart file (plus its parts) is a library, even if it doesn't use a library directive." The page never calls a file a module. | a **package**: "Libraries can be distributed using packages"; `package:` "specifies libraries provided by a package manager such as the pub tool" | https://dart.dev/language/libraries | verified. **Refutes the brief's placing of Dart** under "call a file a module" |
| Swift | a **source file**, "a single Swift source code file within a module". A **module** is "a single unit of code distribution --- a framework or application that's built and shipped as a single unit" | a **package**: "A package is a group of modules that you develop as a unit." | https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/AccessControl.md (the rendered docs.swift.org page returned no text) | verified. **Refutes the brief's placing of Swift** under "call a file a module" |
| Rust / Cargo | the page read defines **modules** as what "Let you control the organization, scope, and privacy of paths"; that one file is one module was not read: **unverified** | a **package**: "A Cargo feature that lets you build, test, and share crates", "a bundle of one or more crates". The **crate** is "the smallest amount of code that the Rust compiler considers at a time", and "Rustaceans ... use 'crate' interchangeably with the general programming concept of a 'library'" | https://doc.rust-lang.org/book/ch07-00-managing-growing-projects-with-packages-crates-and-modules.html ; https://doc.rust-lang.org/book/ch07-01-packages-and-crates.html | verified for package and crate |
| Hex (Elixir, Erlang) | modules are named units of the BEAM, not files, on the page read: "there can only be one module with a given name running in a BEAM system" | a **package**: "Publishing a package to Hex consists of registering a Hex user, adding metadata to the project's `mix.exs` file" | https://hex.pm/docs/publish | verified for package |
| Go | neither: "Each package within a module is a collection of source files in the same directory that are compiled together." | a **module**: "A module is a collection of packages that are released, versioned, and distributed together." | https://go.dev/ref/mod | verified, the reversal |
| Zig, from 0.11 | a **module** is "a collection of source files, with a single root source file, which can be imported in your code by name", "(previously known as 'package')" | a **package**: "A package is a directory of files, uniquely identified by a hash of all files, which can export any number of compilation artifacts and modules." "Package information is declared in a file named `build.zig.zon`." | https://ziglang.org/download/0.11.0/release-notes.html | verified |
| OCaml | a compilation unit, "derived from the names of the files containing the interface and the implementation", which "behaves roughly as the module definition" | not read | https://ocaml.org/manual/latest/compunit.html | verified for the unit only |

**Reading.** The coordinator's reading holds in its **package** half and is
narrower than stated in its **module** half.

- **Package as the fetched, versioned unit.** Python (as the distribution
  package), npm, Dart, Swift, Cargo, Hex and Zig from 0.11 all call it a
  package. Go is the one reversal in this sweep, and Go's package is a
  directory.
- **Module as one file.** Python's tutorial says so outright, npm says so
  roughly, and OCaml's compilation unit comes close. Dart calls a file a
  *library*, Swift a *source file* (its module is a framework), and Zig's
  module is a root file and what it imports.

So "one file is one module" is a Heroes ruling with precedent in Python and
not a universal one. It is also not what this sitting rules: spec § 1 already
says it.

**Two precedents on the word itself.**

- **Python lives with the collision Heroes is fixing.** The packaging
  authority's glossary documents *package* in two senses and resolves the
  ambiguity in prose ("this guide may use the expanded term when more clarity
  is needed"). That is the conservative route of the critic's d.2, already
  taken by a large ecosystem, and the glossary is the cost it pays: two
  entries to say what one word means.
- **Zig took the other route before 1.0.** Its 0.11 notes say of the old
  package: "It is now instead called a **module**. This frees up the term
  **package** to be used in the context of package management." That is the
  same motive this sitting has, *package* reserved for what package management
  fetches.

## Question 3: renames of a mark before 1.0, and what old programs got

| Language | What was renamed, and when | What an old program got | Source, read 2026-11-09 | Status |
|---|---|---|---|---|
| Rust 0.8 | `const` items became `static` items | an error from the parser, which kept the old syntax only to refuse it: the module "Support for parsing unsupported, old syntaxes, for the purpose of reporting errors", the message "obsolete syntax: %s", and for this case ("`const` item", "`const` items are now `static` items; replace `const` with `static`") | https://fuchsia.googlesource.com/third_party/rust/+/0.8/src/libsyntax/parse/obsolete.rs | verified |
| Rust 0.10 | `loop` as a skip became `continue` | the same machinery: ("`loop` instead of `continue`", "`loop` is now only used for loops and `continue` is used for skipping iterations") | https://fuchsia.googlesource.com/third_party/rust/+/refs/tags/0.10/src/libsyntax/parse/obsolete.rs | verified |
| Rust 0.10 (April 2014) | "`extern mod` is now `extern crate`" | **unverified**: no `ObsoleteSyntax` variant at the 0.10 tag names `extern mod`, and the release notes name no error | https://fuchsia.googlesource.com/third_party/rust/+/refs/tags/0.11.0/RELEASES.txt | the rename verified, the telling unverified |
| Rust, 2018 | the obsolete-syntax module itself removed, PR #49395 "libsyntax: Remove obsolete.rs", merged 2018-03-28: "This little piece of infra is obsolete (ha-ha) and is unlikely to be used in the future, even if new obsolete syntax appears." | (the telling machinery has a lifetime) | https://github.com/rust-lang/rust/pull/49395 | verified |
| Zig 0.11 | builtins renamed | "`zig fmt` will automatically update usages of the old builtin names in your code"; "Where possible, `zig fmt` has been made to automatically migrate uses of the old builtins", with named exceptions it "will not modify" | https://ziglang.org/download/0.11.0/release-notes.html | verified (the list of renamed builtins was not in the parts read) |
| Zig 0.11 | *package* renamed *module* in the build system | the notes read say nothing of what an old `build.zig` got | same page | the rename verified, the telling unverified |
| Go, before Go 1 | API changes across releases | a rewriting tool: "Gofix is a new tool that reduces the amount of effort it takes to update existing code" ("just run gofix on your source directory"), and "When gofix cannot rewrite a use of an old API, it prints a warning giving the file name and line number of the use." Russ Cox, 15 April 2011 | https://go.dev/blog/introducing-gofix | verified |
| SwiftPM (Swift 4.2, so not a pre-1.0 case) | SE-0208, implemented in Swift 4.2, "moves the current system-module packages feature from package to target level" | a warning, "system packages are deprecated; use system library targets instead" (forum post of 2019-04-01). The old surface is still listed today: Apple's `Package` page keeps "Configuring System Packages" with `var pkgConfig: String?`, "The name to use for C modules", and initialisers taking `pkgConfig:` | https://github.com/swiftlang/swift-evolution/blob/main/proposals/0208-package-manager-system-library-targets.md ; https://forums.swift.org/t/swiftpm-how-to-migrate-system-package-library-to-system-library-target/22427 ; https://developer.apple.com/tutorials/data/documentation/packagedescription/package.md | verified |

**Reading.**

- **The refusal that names the new spelling has precedent.** Rust before 1.0
  is the case: it renamed keywords, kept no alias, and refused the old
  spelling with a message saying what to write (`replace const with static`).
  That is CLAUDE.md § 2's shape: no old spelling kept beside the new, and the
  change told.
- **The mechanical rewrite has precedent too.** Zig's `zig fmt` and Go's gofix
  are the other half. Go's prints a warning with the file and line where it
  cannot rewrite; Zig's leaves the uses it cannot translate unmodified.
- **SwiftPM shows what § 2 avoids.** Its pkg-config sense also stopped being a
  *package*, by a deprecation rather than a refusal, and Apple's documentation
  today still lists the old `pkgConfig` on `Package`.

  SE-0208's stated motive was structural, not lexical: "libraries that need it
  often need to create a separate repository containing the system package",
  "In practice, this effort did not work out". So it is a precedent for
  leaving *package* in this sense, not a precedent for the reason.

## The verdict, in the historian's structure

- **verdict**: approve (advisory). The name is `pkgconfig`.
- **precedents**: the three tables above, every line marked; the four on which
  the name rests:
  - Cabal `pkgconfig-depends` beside `extra-libraries` (verified);
  - V `#pkgconfig` beside `#flag -l` (verified);
  - SwiftPM `pkgConfig:` (verified);
  - cgo `#cgo pkg-config:` (verified).

  Zig's `use_pkg_config` and Meson's `method : 'pkg-config'` support it. The
  rename itself rests on Zig 0.11 freeing *package* (verified). The refusal
  form rests on Rust 0.8's and 0.10's obsolete-syntax errors (verified).
- **argument** (120 words or fewer): No system swept puts bare *package* in
  source for a pkg-config lookup. Every one that offers both a plain link and
  a pkg-config route names the second after the tool: Cabal, V, cgo, Zig and
  Meson. The tool's own noun is split between *package* (freedesktop) and
  *module* (pkgconf, CMake, autoconf), and Heroes has given both away. So a
  noun cannot carry this sense without colliding, and a mechanism name can.
  `system` belongs, in precedent, to plain linking (Odin's `system:`).
  `library` is `link`'s noun already. `pkgconf` names one implementation,
  which no system here does. Zig freed *package* for package management
  before 1.0, saying so in its notes. Rust before 1.0 refused renamed
  keywords with a message naming the new spelling.
- **condition**: two findings would change this reading.
  - **A Meson-style `auto`.** If Heroes means the mark to be answered someday
    by something other than pkg-config (CMake config files, frameworks,
    vcpkg's own manifests), precedent favours a mechanism-neutral word, as
    Meson's `dependency` and Crystal's `Link` are. A tool-named mark would then
    say less than it does, which is SwiftPM's `pkgConfig:` position today.
  - **A system that kept bare `package` for this sense** beside a fetched
    *package* and reports no confusion, which this sweep did not find.
- **search**: below.

### The four answers every seat owes

1. **The name**: `pkgconfig`, one word, as V writes it, as Cabal writes it
   inside `pkgconfig-depends`, as SwiftPM camel-cases it, and as Homebrew lists
   it among the formula's aliases. The diagnostic noun is the critic's d.1:
   `head_names.hero:123-126` would print "this pkgconfig's name". Precedent
   says nothing on that; it is the spec-warden's.
2. **Verdict on the rename**: adopt, with one condition from precedent. **The
   refusal of `package` in an extern head names `pkgconfig` in its message.**
   This is Rust 0.8's `obsolete syntax` shape: the old spelling parsed only to
   be refused, and the message says what to write. It is not an alias, so
   CLAUDE.md § 2 is kept. Whether it is a `certain` `Fix` is the
   compiler-engineer's; Zig's and Go's tools rewrote mechanically where they
   could.
3. **Prediction**: by the tag of the milestone that lands `crypto/sha256`'s
   binding (M-core-packages), no Heroes program in `tests/`, `examples/` or
   `packages/` will hold an extern head whose library cannot be found by
   pkg-config under the renamed mark and needs a second lookup mechanism
   behind it. If one does, the tool-named mark has started to say less than it
   does, as SwiftPM's did, and the name should be re-sat.
4. **Not measured, as questions**:
   - Did rustc 0.10 tell an `extern mod` program to write `extern crate`?
   - Did Zig 0.11 refuse an old `std.build.Pkg` with a message naming
     `addModule`?
   - Does vcpkg's port call itself pkgconf?
   - Does Odin's compiler call pkg-config anywhere outside the overview page?
   - Does any language not swept here (D's dub, Vala, Julia, Haskell's
     in-source `extra-libraries`, Kotlin/Native's `.def` files) use bare
     *package* for this sense?

### search

Queries made on 2026-11-09, in order:

- `zig build linkSystemLibrary pkg-config use_pkg_config`
- `go cgo "#cgo pkg-config:" directive documentation`
- `Swift Package Manager systemLibrary pkgConfig parameter documentation`
- `vlang "#pkgconfig" directive C interop documentation`
- `PKG_CHECK_MODULES pkg.m4 documentation autoconf macro ...`
- `Zig 0.11.0 release notes "packages" renamed "modules" build system addModule`
- `"Introducing Gofix" Go blog 2011 ...`
- `Rust 0.10 release notes "extern mod" renamed "extern crate"`
- `rustc libsyntax "obsolete.rs" ObsoleteSyntax ...`
- `rust parser "extern mod" "is obsolete" ...`
- `swift-package-manager PackageDescription "public var pkgConfig: String?" ...`

Pages fetched and read: every URL in the three tables; the Nim manual and the
Odin overview at offsets until the sections were found; the Zig 0.11 notes,
characters 0 to 200,000 of 228,604, the last 28,604 not read.

Pages that failed:

- people.freedesktop.org's pkg-config guide: HTTP 403;
- the swift-package-manager `PackageDescription.swift` raw file: HTTP 404;
- docs.swift.org's Access Control page: no text, read instead from its
  Markdown source.

Swept and found **without** a bare `package` mark: the fourteen rows of
question 1. Not swept: D's dub, Vala, Julia, Kotlin/Native, Haskell's
in-source pragmas, Ada's GPR files, .NET's NuGet native assets, Conan,
vcpkg's manifests.

Repository files read:

- the three briefs;
- the critic's first pass;
- `docs/panel/050-the-package-clause.md`, lines 1 to 80.

*Heroes of code* was not opened for this sitting and no line rests on it.
