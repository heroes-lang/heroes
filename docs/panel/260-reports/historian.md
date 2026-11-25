# Panel 260, the historian's report

Seat: historian (advisory, no veto). Base: `c3885b81b`. Written 2026-11-25 as the search went; every page below was
read on 2026-11-25. **Ran nothing; read web pages only** (WebSearch, WebFetch). Nothing in the tree was built or run
by this seat. No book of this project's author is cited, and none was used as a route here.

**One slip to report**: during the Java step this seat wrote a stray file by mistake,
`docs/panel/260-reports/historian.md.zig-nim.part` in the panel260 worktree (one placeholder line, nothing else).
This seat has no delete tool; the coordinator removes it.

## verdict

**approve** (advisory). The principle departs from most precedent on purpose, and the departure is the right way:
most tools let a package declare native code where it is defined, and almost none hold the declaration to the code.
The one found that does, Zig, does it as the author asks: a compile error, no fallback. Conditions on the reading are
under `condition`.

## precedents

Each: **where** the declaration lives; whether it is **checked** against the code; what an importer **pays**; its
**history**. `verified` means the quoted words were read on the cited page today; `unverified` marks every sentence
that a page did not carry.

### 1. Cargo's `links` key (Rust) — verified

- **Where**: the package's own manifest. Cargo book, *Build Scripts*, § The `links` Manifest Key
  (https://doc.rust-lang.org/cargo/reference/build-scripts.html): *"The `package.links` key may be set in the
  `Cargo.toml` manifest to declare that the package links with the given native library. The purpose of this manifest
  key is to give Cargo an understanding about the set of native dependencies that a package has, as well as providing
  a principled system of passing metadata between package build scripts."*
- **Checked, two ways and only two**: (1) *"When using the `links` key, the package must have a build script, and the
  build script should use the `rustc-link-lib` instruction to link the library"* (same page); RFC 403 (start date
  2014-10-30, https://rust-lang.github.io/rfcs/0403-cargo-build-command.html): *"It is illegal to define `links`
  without also defining `build`."* (2) *"Cargo requires that there is at most one package per `links` value. In other
  words, it is forbidden to have two packages link to the same native library. This helps prevent duplicate symbols
  between crates"* (the book); the RFC: *"the same C library _must not_ appear more than once in a dependency graph."*
- **Not checked against the code**: the RFC says *"Unlike `links`, it will be legal to specify `build` without
  specifying `links`"*, so a build script may emit `rustc-link-lib` undeclared; and the Rust Reference's `#[link]`
  attribute (https://doc.rust-lang.org/reference/items/external-blocks.html) *"specifies the name of a native library
  that the compiler should link with for the items within an `extern` block"*, with no Cargo key named. On the Rust
  internals forum, 2022-11-15 (https://internals.rust-lang.org/t/build-time-detection-of-linked-libraries/17747),
  @tari: *"some crates link to libraries that can't be put in the crate's manifest `links`"*; @Nemo157: *"the `links`
  key is just a mitigation against having multiple `-sys` libraries"*. So: **declared at definition, checked for form
  and uniqueness, never held to what the crate's code links.** That Cargo prints nothing at all for an undeclared
  native link is read from the absence of any such rule on these pages, not from a page saying so: `unverified` as a
  positive statement.
- **Pays**: a dependent gets the build script's metadata, *"passed to immediate dependents, not transitive
  dependents"* (the book; RFC: `DEP_<foo>_<key>`). The RFC's stated reason for knowing the set: *"By knowing the exact
  set of dependencies, Cargo paves a way into the future to extend its handling of native dependencies."*
- **History**: the convention that grew around it, the book's *`*-sys` packages*: *"Any package named `foo-sys` should
  provide two major pieces of functionality: The library crate should link to the native library `libfoo` ... The
  library crate should provide declarations for types and functions in `libfoo`, but not higher-level abstractions."*
  A naming convention the tool does not enforce (the book's word is *"naming convention"*). Open request, cargo issue
  12848, 2023-10-18 (https://github.com/rust-lang/cargo/issues/12848), *"Per-crate linking option in Cargo
  manifest"*: linking through a build script *"still doesn't show native library dependencies in documented format"*;
  maintainer epage the same day, *"no decision has been made at this time"*.

### 2. Go's cgo, `import "C"` (question 2 and the second half of 11) — verified

- **Where**: in the source file that binds, by an import. cmd/cgo (https://pkg.go.dev/cmd/cgo): *"To use cgo write
  normal Go code that imports a pseudo-package "C"."* Link flags beside it: *"CFLAGS, CPPFLAGS, CXXFLAGS, FFLAGS and
  LDFLAGS may be defined with pseudo #cgo directives within these comments"*, and *"All the LDFLAGS directives in any
  package in the program are concatenated and used at link time"*: they travel to every program importing the package.
- **Checked**: the import IS the mark, so there is no separate declaration to hold against the code. With cgo off,
  the page: *"The go tool will set the build constraint "cgo" if cgo is enabled ... Therefore, if cgo is disabled,
  files that import "C" will not be built by the go tool."* Go's own test pins what a package whose only file imports
  C gets: `install_cgo_excluded.txt` (https://go.dev/src/cmd/go/testdata/script/install_cgo_excluded.txt), `env
  CGO_ENABLED=0`, `! go install cgotest`, `stderr 'build constraints exclude all Go files'`.
- **Pays**: a C toolchain. cmd/cgo: cgo is *"disabled by default when cross-compiling"* and when *"the CC environment
  variable is unset and the default C compiler (typically gcc or clang) cannot be found on the system PATH"*; *"When
  cross-compiling, you must specify a C cross-compiler for cgo to use."* That a program importing no cgo package pays
  none of it is an inference from the build-constraint rule: `unverified` as a sentence of Go's.
- **History, the closest precedent to the author's *an error, never a fallback***: issue 12860, *"cmd/go: importing
  "C" with CGO disabled should be an error"*, opened 2015-10-07 by pebbe against go1.5.1
  (https://github.com/golang/go/issues/12860): *"Building with `CGO_ENABLED=0` silently skips source files that have
  `import "C"`"*, asking for *"import "C" failed because CGO is disabled"*. Refused for compatibility: minux, *"I'm
  worrying about compatibility with the old behavior"*; rakyll, *"We shouldn't fail this package just because it is
  importing C"*; adg, *"Whether we like it or not, `import "C"` is an implicit `cgo` build tag."* Closed 2019-01-23
  into issue 24068, *"cmd/go: give a better error message when building Go package with CGO_ENABLED=0"*, opened
  2018-02-23 (https://github.com/golang/go/issues/24068): **still open when read today**, moved from Go1.14 to Backlog
  on 2019-10-09; the page shows a gopherbot link dated 2026-09-24 to CL 838025, *"cmd/go: note files excluded by
  disabled cgo when compile fails"*, and does not say whether it merged. **What it says**: a mark that is the binding
  itself, plus a silent exclusion when the platform is off, has left Go with an eleven-year-old request (2015-10-07 to
  today) for an error naming the cause. It was refused once for compatibility and has not been delivered since.
- **And Go's standard library ships the fallback the author refuses**, at package grain: `net`
  (https://pkg.go.dev/net), *"The netgo build tag disables entirely the use of the native (CGO) resolver"*; *"With the
  netcgo build tag the native and the pure Go resolver are compiled into the binary"*; `os/user`
  (https://pkg.go.dev/os/user), *"this package has two internal implementations ... One is written in pure Go and
  parses /etc/passwd and /etc/group. The other is cgo-based and relies on the standard C library (libc) routines ...
  When cgo is available ... cgo-based (libc-backed) code is used."* A program built without cgo silently gets the
  other implementation.

### 3. Go's file-name build constraint (question 11, first half) — verified

- **Where**: the file's own name. `go help buildconstraint` (https://pkg.go.dev/cmd/go#hdr-Build_constraints): *"If a
  file's name, after stripping the extension and a possible _test suffix, matches any of the following patterns:"*
  `*_GOOS`, `*_GOARCH`, `*_GOOS_GOARCH`. go/build (https://pkg.go.dev/go/build): *"Build constraints may also be part
  of a file's name"*, `source_windows.go` *"will only be included if the target operating system is windows."*
- **Checked**: nothing to check; the name is the constraint. It excludes a file and demands nothing of it.
- **Pays**: nothing per file; the cost falls on the namespace.
- **History, the cost of a declaration that is a file's name**: Go 1.11, *"released 2018-08-24"*
  (https://go.dev/doc/devel/release), release notes (https://go.dev/doc/go1.11): *"As a result of the addition of the
  new `GOOS` value "`js`" and `GOARCH` value "`wasm`", Go files named `*_js.go` or `*_wasm.go` will now be ignored by
  Go tools except when those GOOS/GOARCH values are being used. If you have existing filenames matching those
  patterns, you will need to rename them."* A name that was ordinary in one release became a declaration in the next,
  and files changed meaning without a line of theirs changing.

### 4. Zig: `extern "c"`, `link_libc`, and the error the brief recalled — verified (its first version unverified)

- **The error exists, word for word**: *"dependency on libc must be explicitly specified in the build command"*,
  reported on Ziggit 2023-03-27 from `src/lua.zig:10:8`
  (https://ziggit.dev/t/tests-building-fails-due-to-lack-of-c-library-linking/572), and present in the compiler at
  tag 0.14.0 (https://raw.githubusercontent.com/ziglang/zig/0.14.0/src/Sema.zig, function `handleExternLibName`).
- **Where the two declarations live**: one at the binding, the library name of an `extern` declaration (the function's
  doc comment: *"Given a library name, examines if the library name should end up in ... (for example, libc is always
  specified via dedicated flag `link_libc` instead)"*, the name *"saved as part of the respective `Decl` (either
  `ExternFn` or `Var`)"*); one in the build, the module's `link_libc` option, which a package sets in its own
  `build.zig` (Ziggit, 2024-04-11, https://ziggit.dev/t/declare-package-depends-on-lib-c/3913: dimdin's reply, *"the
  Module type has a flag called `link_libc`"*, `b.addModule(... .link_libc = true)`).
- **Checked, read in the 0.14.0 source** (each line quoted from the function, in order):
  `if (std.zig.target.isLibCLibName(target, lib_name))` then `if (!comp.config.link_libc)` → *"dependency on libc
  must be explicitly specified in the build command"*; the same for libc++ (`link_libcpp`) and `"unwind"`
  (`link_libunwind`); then `if (!target.cpu.arch.isWasm() and !block.ownerModule().pic)` → *"dependency on dynamic
  library '{s}' requires enabling Position Independent Code; fixed by '-l{s}' or '-fPIC'"*; otherwise
  `comp.addLinkLib(lib_name)`. So **for libc, libc++ and libunwind the binding's name is held to the build's
  declaration, and an undeclared use is a compile error with no fallback. For every other library the binding's own
  `extern "name"` IS the declaration, and the compiler adds it to the link itself.**
- **Pays**: libc linked for a module that declares it. That a program whose code names no `extern "c"` pays no libc
  through this rule is an inference from the function's conditions: `unverified` as a sentence of Zig's.
- **History**: the version that introduced the error, and its issue: **unverified**; searched (queries below) and not
  found. Zig's issue 514, *"implement libc in zig"* (https://github.com/ziglang/zig/issues/514, opened 2017-10-01,
  closed 2019-07-12), is the context (*"Zig will no longer look for the native libc and try to build against it"*)
  but does not name the error. A 2026 regression note in a binding generator says Zig 0.16 shows the error and the fix
  is `.link_libc = true` on each module (a search-result snippet of
  https://docs.rs/crate/alef/latest/source/tests/e2e_zig_link_libc.rs; the page itself did not render: `unverified`).
- **What it says**: the one precedent found where **an undeclared use of the platform is a compile error at the
  binding, not a link failure or a fallback**. Its check reads the library name the binding writes (`"c"`), not the
  symbol: a binding that writes no library name is outside it (an inference from the function's single input,
  `lib_name`; `unverified` as a behaviour).

### 5. Nim's pragmas (question 3) — verified

- **Where**: at the binding, in the source. Nim manual, v0.16.0 copy
  (https://web.mit.edu/nim-lang_v0.16.0/nim-0.16.0/doc/manual/ffi.txt): *"The ``importc`` pragma provides a means to
  import a proc or a variable from C."*; *"With the ``dynlib`` pragma a procedure or a variable can be imported from a
  dynamic library"*; *"At runtime the dynamic library is searched for (in this order)"*. Current manual
  (https://nim-lang.org/docs/manual.html): the Compile pragma *"can be used to compile and link a C/C++ source file
  with the project"*; the Link pragma *"can be used to link an additional file with the project"*; the passl pragma
  *"can be used to pass additional parameters to the linker"*.
- **Checked**: the pragmas are the mark. The manual sections read name no package-level declaration they are held to.
  Nimble's manifest has one native key, `foreignDep` (Nim's `distros` module, https://nim-lang.org/docs/distros.html:
  *"Registers foreignPackageName to the internal list of foreign deps"*; *"Its primary purpose is to produce output
  for Nimble packages"*), and Nimble's readme (v0.16.0 copy,
  https://web.mit.edu/nim-lang_v0.16.0/nim-0.16.0/dist/nimble/readme.markdown) says such dependencies *"are not
  managed by Nimble and can only be installed via your system's package manager or downloaded manually via the
  internet"*, printing `Hint: This package requires some external dependencies.` A hint, and no page read says it is
  held to the pragmas: `unverified` as a positive statement.
- **Pays**: `dynlib` moves the cost to run time, a library looked up when the program starts.

### 6. Java's native access (question 5) — verified through Oracle's guide; the JEP pages unread

- **Where**: the application, at launch. Oracle, *Restricted Methods*, Java SE 24
  (https://docs.oracle.com/en/java/javase/24/core/restricted-methods.html): *"You must enable native access if an
  application calls any of the following restricted methods"*; `java --enable-native-access=M1,M2,... MyApp`,
  `java --enable-native-access=ALL-UNNAMED MyApp`; *"Add `Enable-Native-Access: ALL-UNNAMED` to the manifest of an
  executable JAR file."* The manifest home is the executable JAR, which is the application, not a library on its
  class path.
- **Checked, at run time**: `--illegal-native-access`, *"warn: Allows the restricted operation to proceed and issues a
  warning the first time that an illegal native access occurs in a particular module ... This is the default value in
  JDK 24 and later."*; *"deny: Throws an IllegalCallerException for every illegal native access operation. This will
  be the default value in a future release of the JDK."*
- **Why the application's**: nipafx.dev, 2024-12-09 (https://nipafx.dev/jni-restriction/): *"all JDK features that
  are capable of breaking integrity must obtain explicit approval from the application's developer"*. InfoQ,
  2024-07-15 (https://www.infoq.com/news/2024/07/jep-472-restrict-jni-jdk-24/): future uses *"will require explicit
  approval from the application's developer at startup"*, tied to *"integrity by default"*.
- **The JEP pages themselves** (https://openjdk.org/jeps/472, https://openjdk.org/jeps/454) answered 403 to every
  fetch, as did openjdk.java.net, bugs.openjdk.org and mail.openjdk.org. Their own words are **unverified** here; the
  lines above quote Oracle's guide and two secondary pages.
- **What it says**: Java puts the grant in the home the author refuses (*"when it is imported"*, here *when it is
  launched*), for a stated reason, **integrity**: the party who bears the risk consents. Its check runs at run time,
  warn now and deny later. In Heroes the declaration guards the importer's build cost, not its integrity, and the
  package's writer is the one who knows whether the code binds; the two cases have different reasons.

### 7. Dart and Flutter (question 6) — verified

- **Where**: the package's own folder and manifest. Dart, *Hooks* (https://dart.dev/tools/hooks): *"Hooks are Dart
  scripts placed in the `hook/` directory of your Dart package"*; *"Build hooks can only output assets in their own
  package"*; *"Support for build hooks was introduced in Dart 3.10."* Flutter's plugin template
  (https://flutter.googlesource.com/mirrors/flutter/+/HEAD/packages/flutter_tools/templates/plugin_shared/pubspec.yaml.tmpl):
  `ffiPlugin: true` per platform, commented *"The 'ffiPlugin' specifies that native code should be built and
  bundled."* and *"This is required for using `dart:ffi`."*; Flutter's docs
  (https://docs.flutter.dev/packages-and-plugins/developing-packages): the `package_ffi` template *"is the recommended
  approach to build and bundle native code since Flutter 3.38"*.
- **Checked**: none of the four pages read describes a check that a package's `dart:ffi` code is declared, or that a
  declared package binds anything: `unverified` either way.
- **Pays**: *"The hooks are run automatically when invoking the `run`, `build`, or `test` commands"*, and hooks
  *"might do longer running operations such as downloading or calling a native compiler"* (Dart's Hooks page): the
  importer's build runs the package's hook.

### 8. npm, `binding.gyp` and `gypfile` (question 7) — verified

- **Where**: the package's own root, by a file's presence. npm, *package.json*
  (https://docs.npmjs.com/cli/v11/configuring-npm/package-json): *"If there is a `binding.gyp` file in the root of your
  package and you have not defined an `install` or `preinstall` script, npm will default the `install` command to
  compile using node-gyp"* (default `"install": "node-gyp rebuild"`); *"To prevent npm from automatically building
  your module with node-gyp, set `gypfile` to `false`"*. npm, *scripts* (https://docs.npmjs.com/cli/v11/using-npm/scripts):
  *"Don't use `install`. Use a `.gyp` file for compilation, and `prepare` for anything else."*
- **Checked**: by presence only; no page read describes a check against the package's code.
- **Pays**: the importer's machine compiles at install. node-gyp (https://github.com/nodejs/node-gyp): *"node-gyp is a
  cross-platform command-line tool written in Node.js for compiling native addon modules for Node.js"*, needing on
  Unix *"A supported version of Python"*, *"make"* and *"A proper C/C++ compiler toolchain, like GCC"*; on macOS
  *"Xcode Command Line Tools"*; on Windows Visual Studio's *"Desktop development with C++"* workload.

### 9. Swift Package Manager and clang module maps (question 8) — verified

- **Where**: the package manifest, per target. PackageDescription
  (https://docs.swift.org/package-manager/PackageDescription/PackageDescription.html): `systemLibrary`, *"Use system
  library targets to adapt a library installed on the system to work with Swift packages ... by providing a
  `modulemap` file along with other metadata such as the library's `pkgConfig` name"*; `linkedLibrary`, *"Declares
  linkage to a system library. This setting is most useful when the library can't be linked automatically, such as C++
  based libraries and non-modular libraries."*
- **Checked**: the page describes no check of a linker setting against the target's code. It describes one
  definition-side property held at import: *"As some build flags can be exploited for unsupported or malicious
  behavior, a product can't be used as a dependency in another package if one of its targets uses unsafe flags."*
  (`unsafeFlags`, *"First available in PackageDescription 5.0"*).
- **Clang's module map, the home "linked automatically" refers to**, Clang, *Modules*
  (https://clang.llvm.org/docs/Modules.html): *"A link-declaration specifies a library or framework against which a
  program should be linked if the enclosing module is imported in any translation unit in that program."* The
  declaration is written beside the headers it covers, and it travels by import. The same page: *"Automatic linking
  with the link directive is not yet widely implemented"*, *"because it requires support from both the object file
  format and the linker"*; *"The notion is similar to Microsoft Visual Studio's #pragma comment(lib...)."* That the
  SwiftPM page's *"linked automatically"* means this mechanism is an inference: `unverified`.

### 10. Ada's `pragma Linker_Options` (not in the brief; found while searching for travel by use) — verified

- **Where**: in the compilation unit, at a declarative item. Ada 95 Reference Manual B.1
  (https://www.adaic.org/resources/add_content/standards/95lrm/ARM_HTML/RM-B-1.html), paragraph 37, and Ada 2022 RM
  B.1 (https://ada-rapporteur-group.github.io/ARM/Ada_2022/RM-B-1.html): *"Pragma Linker_Options has the effect of
  passing its string argument as a parameter to the system linker (if one exists), if the immediately enclosing
  compilation unit is included in the partition being linked."* Ada 2022's paragraph 9: *"A pragma Linker_Options is
  allowed only at the place of a declarative_item."*
- **Checked**: no (the string is passed through; its interpretation is implementation-defined, B.1(37)).
- **Pays**: **only a partition that includes the unit**, since Ada 95 at the latest: the declaration lives at
  definition, travels by inclusion, and a program that includes no such unit pays nothing.

### 11. Python: `ext_modules`, `Root-Is-Purelib`, and auditwheel (question 9) — verified

- **Where**: the package's build configuration and the wheel it makes. setuptools
  (https://setuptools.pypa.io/en/latest/userguide/ext_modules.html): *"The keyword argument `ext_modules` of `setup()`
  should be a list of instances of the"* `setuptools.Extension` class, or `[tool.setuptools]` `ext-modules` in
  `pyproject.toml`. Wheel format (https://packaging.python.org/en/latest/specifications/binary-distribution-format/):
  *"Root-Is-Purelib is true if the top level directory of the archive should be installed into purelib; otherwise the
  root should be installed into platlib"*; file name `{distribution}-{version}(-{build tag})?-{python tag}-{abi
  tag}-{platform tag}.whl`; *"February 2013: This specification was approved through PEP 427."*
- **Checked**: the purelib flag is guidance (the spec's FAQ: *"a wheel should have only one of 'purelib' or 'platlib'
  depending on whether it is pure Python or not"*); the spec describes no check. **But the platform tag is held to the
  binaries by a tool**: PEP 513, created 19-Jan-2016, now superseded by PEP 600 (https://peps.python.org/pep-0513/):
  *"The tool inspects all of the ELF files inside a wheel to check for dependencies on versioned symbols or external
  shared libraries, and verifies conformance with the `manylinux1` policy."* auditwheel
  (https://github.com/pypa/auditwheel): `show` *"shows external shared libraries that the wheel depends on (beyond the
  libraries included in the `manylinux` policies)"* and *"checks the extension modules for the use of versioned symbols
  that exceed the `manylinux` ABI"*; `repair` *"copies these external shared libraries into the wheel itself"*. A
  declaration (a tag) held to what the built objects link: the shape of § 6.2's *"undefined names of the program's own
  objects, read by `nm`"*, run by the package's writer before publishing.

### 12. pkg-config's `Libs.private` (question 10) — verified

- **Where**: the library's own `.pc` file, written by the library. Dan Nicholson, *Guide to pkg-config* (2010; Debian's
  copy, https://sources.debian.org/src/pkg-config/0.29.2-1/pkg-config-guide.html/): `Libs`, *"The link flags specific
  to this package and any required libraries that don't support pkg-config"*; `Libs.private`, *"The link flags for
  private libraries required by this package but not exposed to applications"*; *"libraries from Requires.private will
  only be included when static linking"*.
- **Checked**: not against the library's objects; no page read describes such a check.
- **Why the split exists**, the guide: *"it is preferred to add link flags for external libraries to the Libs.private
  field so programs do not acquire an additional direct dependency"*, and for `Requires`, *"usually preferred to use
  the private variant of Requires to avoid exposing unnecessary libraries to the program that is linking with your
  library"*, citing overlinking. **History**: a consumer that does not ask `--static` gets no private flags. reproc's
  issue (https://www.github.com/DaanDeMeyer/reproc/issues/59, read only as a search snippet: `unverified`) is one
  report; the pkg-config list thread of 2024-09 on static-only installs answered 403 (`unverified`).

### 13. libtool's `.la` and `dependency_libs` (question 10) — verified

- **Where**: written by libtool beside the library, at the library's build; carried to whoever links it.
- **Checked**: no; libtool records the dependencies. Gentoo QA, *Handling Libtool Archives*, last edited 2020-10-23
  (https://wiki.gentoo.org/wiki/Project:Quality_Assurance/Handling_Libtool_Archives): libtool *"pulls in all direct
  and **indirect** dependencies into the `.la` files it creates"*, causing *"massive _overlinking_, which is toxic to
  the Gentoo ecosystem"*; *"the previous overlinking cannot be prevented by any LDFLAGS wizardry"*; *"In 95% of cases,
  deleting the libtool archives can be done safely."*
- **History, the removals**: Debian's release goal page, last modified 2011-06-20
  (https://wiki.debian.org/ReleaseGoals/LAFileRemoval): RC bugs *"due to dependencies hidden in .la-files"*, files
  that *"introduced unneeded dependencies"*, and *"dependency_libs mentions absolute paths, it'll break multiarch"*.
  Debian Policy § 10.2 (https://www.debian.org/doc/debian-policy/ch-files.html): *"For public libraries intended for
  use by other packages, these files normally should not be included in the Debian package"*; where one is needed,
  *"the `dependency_libs` setting in the `.la` file should normally be set to the empty string"*, and a historical one
  *"must be retained (with `dependency_libs` emptied) until all libraries that depend on it have removed or emptied
  `dependency_libs`"*. Linux From Scratch (https://linuxfromscratch.org/blfs/view/svn/introduction/la-files.html):
  *"The solution is to remove the .la files."* **What it says**: a declaration that travels **whole and transitively**
  to every consumer was what three distributions spent years removing.

### 14. Haskell Cabal's `extra-libraries` and `c-sources` (question 12) — verified for the fields, unverified for the check

- **Where**: the package's `.cabal` file. Cabal user guide
  (https://cabal.readthedocs.io/en/stable/cabal-package-description-file.html): `extra-libraries`, *"A list of extra
  libraries to link with (when not linking fully static executables)"*; `c-sources`, *"A list of C source files to be
  compiled and linked with the Haskell files"*; `pkgconfig-depends`, *"Cabal uses pkg-config to find if the packages
  are available on the system and to find the extra compilation and linker options needed to use the packages."*
- **Checked**: for presence on the system, not against the code. Ticket 262, *"Check for required C libraries during
  configure"*, posted 2008-03-25
  (https://mailman.haskell.org/archives/list/cabal-devel@haskell.org/thread/B4ATNWNPWGRYYPU6BDSEAWBEZHJ7SRNK/):
  *"Cabal should check for required C libraries and other non-Haskell dependencies at the configure stage."* That
  today's check links a small C program per library is a search-result summary of mailing-list threads:
  `unverified`.

### 15. Deno's `--allow-ffi` (question 12, the opposite home) — verified

- **Where**: the user's run. Deno, *Security* (https://docs.deno.com/runtime/fundamentals/security/): *"`--allow-ffi`
  loads native libraries through FFI or Node-API addons"*; *"You must explicitly grant access to these resources with
  command line flags or with a runtime permission prompt"*; *"a native library runs as compiled machine code in the
  same process and can issue system calls directly"*; *"Treat both as equivalent to `--allow-all`"*.
- **What it says**: like Java, a grant at the run for integrity's sake, not a statement of what the code needs.

### 16. Rust's `#![no_std]` (question 12, the inverse declaration) and `unused_crate_dependencies` (question 4) — verified

- **Where**: the crate root. Rust Reference, *Preludes* (https://doc.rust-lang.org/reference/names/preludes.html):
  *"The `no_std` attribute may only be applied to the crate root"*; it *"causes the `std` crate to not be linked
  automatically"*.
- **Does not bind its dependencies**: *"Using `no_std` does not prevent the standard library from being linked. It is
  still valid to write `extern crate std` in the crate or in one of its dependencies; this will cause the compiler to
  link the `std` crate into the program."* A declaration at definition that **one dependency undoes in silence**.
- **A declared dependency that is unused**, the rustc book
  (https://doc.rust-lang.org/rustc/lints/listing/allowed-by-default.html): *"The `unused_crate_dependencies` lint
  detects crate dependencies that are never used"*, and it is allow-by-default *"because it can provide false positives
  depending on how the build system is configured. For example, when using Cargo, a "package" consists of multiple
  crates (such as a library and a binary), but the dependencies are defined for the package as a whole. If there is a
  dependency that is only used in the binary, but not the library, then the lint will be incorrectly issued in the
  library."* **This is § 7.4's question asked elsewhere**: an *unused* check is only as sound as the match between the
  grain of the declaration (the package) and the grain of the reading (the crate). Here a package-level declaration
  read crate by crate gave false positives, and the lint was left off by default.

## argument

Declaring native code where it is defined is old and common: Ada 95's `Linker_Options`, Cargo's `links` (2014),
Cabal, npm's `binding.gyp`, Flutter's `ffiPlugin`, SwiftPM, clang's module-map `link`. **Holding the declaration to
the code is rare**: Cargo checks form and uniqueness, Python's purelib flag is guidance, and no page read shows a check
for npm, Dart or Nim. Among the tools read, only Zig refuses an undeclared use at compile time, for libc, libc++ and
libunwind, by the name the binding writes. Three failures bear on the principle: a silent exclusion naming no cause
(Go, asked since 2015, still open); a declaration travelling whole and transitively (libtool, removed by
distributions); an *unused* check at the wrong grain (Rust, off by default).

## condition

My reading would change on any of these: (1) a page showing a tool that **refused an unused native declaration and
then retracted it** for false positives at the declaration's own grain, which would argue for a warning in § 7.4
rather than an error; (2) a page showing Zig **withdrew** the libc error, or replaced it with automatic linking, and
why; (3) a page showing that Go's pure-Go fallbacks (`net`, `os/user`) were **chosen over an error** for a measured
reason that applies to Heroes, such as cross-compilation with no C toolchain; (4) a source on the JEP 472 pages
saying a library-side grant was **considered and refused** for a reason other than integrity.

## contract (shared brief § 10)

- **Verdict**: approve, advisory. It rests on the shared brief's § 1 (the principle), § 7.1 (homes), § 7.2 (the
  check) and § 7.4 (a declaration that binds nothing).
- **What the precedents say for each question**:
  - **Where (§ 7.1)**: the mark at the binding (cgo, Nim, Zig's `extern "c"`, Ada, clang) and a summary in the
    package (Cargo, Cabal, npm, Flutter) are both common. Zig is the only one found that holds one to the other: the
    binding's name against the build's `link_libc`.
  - **Travel (§ 2's fifth question)**: Ada's `Linker_Options` and clang's `link` travel by inclusion or import, and
    whoever includes nothing pays nothing. libtool's whole, transitive travel is what was removed, and pkg-config split
    `Libs.private` to stop it.
  - **Unused (§ 7.4)**: no precedent found refuses a declaration that binds nothing. The one found that tried
    (Rust's lint) is off by default because its grain differed from the declaration's.
- **Cost named**: no precedent found prices a declared build, so the cost of R6 whole for a declared build stays the
  other seats' to measure.
- **One falsifiable prediction**: if § 7.4's *binds nothing* is read at a grain other than the declaration's (say a
  declaration on the face, *binds* read in the face's own module), it will refuse correct packages, as Rust's lint
  would have. It is checkable at landing by running the check over Rebel's 115 faces: the 78 that reach a floor only
  by `use` (§ 3.1) are the ones that will be refused at that grain, unless *binds* follows `use` to the floor.
- **Where the brief was false or silent**:
  - Question 1's *"dependents read its `DEP_<NAME>_<KEY>` metadata"*: true, but **immediate dependents only**.
  - Question 2's *"a package needing them fails to build"*: only when no buildable file remains
    (`build constraints exclude all Go files`). Otherwise the cgo files drop out silently, which is issue 12860.
  - Question 4's recalled error text is exact.
  - Question 10's recollection that distributions deleted `.la` files is true (Debian Policy § 10.2, Gentoo, LFS).
  - The brief is silent on Ada's `Linker_Options`, clang's module-map `link`, auditwheel, Rust's
    `unused_crate_dependencies`, and Go's own `net` and `os/user` fallbacks. All five bear on § 7.1, § 7.2 and § 7.4.

## search

**How**: WebSearch for each tool's own documentation and history, then WebFetch of the primary page so the quote is
the page's own; a secondary page only where the primary refused the fetch, and said so. Every quote above was read
on 2026-11-25.

**Queries made** (WebSearch, in order): `cargo issue build script rustc-link-lib without "links" key warning not
enforced`; `"links" key cargo "only" "one package" "native library" RFC 403 build scripts links manifest key history`;
`cargo error "specifies that it links to" "but does not have a custom build script"`; `"multiple packages link to
native library" "a native library can be linked only once" cargo error`; `rust "links" key not required "#[link(name"
crate without build script links native library unchecked cargo doesn't know`; `go build "build constraints exclude
all Go files in" CGO_ENABLED=0 package import "C" error message`; `Go 1.11 release notes "_js.go" "_wasm.go" "will now
be ignored by Go tools" rename`; `zig error "dependency on libc must be explicitly specified in the build command"`;
`"must be explicitly specified in the build command" zig Sema libc extern`; `zig Sema.zig handleExternLibName
"dependency on libc" "requires enabling Position Independent Code"`; `zig stage1 "dependency on library c must be
explicitly specified in the build command"`; `zig 0.12.0 release notes Module link_libc option "link_libc"
std.Build.Module createModule`; `"reported_bad_link_libc_error" zig`; `"must be explicitly specified in the build
command" zig github issue 2018 OR 2019 OR 2020 libc`; `nim manual "Dynlib pragma for import" "With the dynlib pragma, a
procedure or a variable can be imported from a dynamic library"`; `nim manual "Foreign function interface" "Nim's FFI
(foreign function interface) is extensive" importc pragma`; `nimble "foreignDep" external dependencies hint nimble
file`; `JEP 472 "Prepare to Restrict the Use of JNI" "Enable-Native-Access" manifest executable JAR application
developer library`; `docs.oracle.com JAR file specification "Enable-Native-Access" attribute "executable JAR"
ALL-UNNAMED`; `docs.oracle.com java 24 "restricted methods" "--enable-native-access" "--illegal-native-access" warn deny
core libraries guide`; `flutter plugin pubspec "ffiPlugin: true" platforms FFI plugin template`; `npm package.json
"gypfile" field "binding.gyp" install script "node-gyp rebuild" default`; `Debian policy libtool .la files
dependency_libs remove "dependency_libs" overlinking Debian wiki ReleaseGoals LAFileRemoval`; `Gentoo devmanual libtool
archives .la files "overlinking" remove reasons`; `pkg-config guide "Libs.private" "The link flags for private
libraries required by this package but not exposed to applications"`; `cabal configure "Missing dependency on a
foreign library" "Missing (or bad) C library" extra-libraries check`; `Ada reference manual "pragma Linker_Options"
"B.1" "the partition" library unit with`.

**Pages fetched** (WebFetch): the Cargo book's build scripts; RFC 403; the Rust Reference's external blocks and
preludes; the rustc book's allowed-by-default lints (two chunks); cargo issue 12848; Rust internals thread 17747;
users.rust-lang.org thread 137697; cmd/cgo; `go help buildconstraint` (two chunks); go/build; Go issues 12860 and
24068; `install_cgo_excluded.txt`; go1.11 notes; Go's release history; `net` and `os/user`; Ziggit threads 572 and
3913; Zig issue 514; Zig 0.14.0 `src/Sema.zig` (chunk 0 to 100000, then 380000 to 480000 twice); Zig 0.14.0 language
reference (first chunk); the Nim manual (four chunks) and its v0.16.0 FFI chapter; Nim's `distros`; Nimble's reference
and its v0.16.0 readme; Oracle's Restricted Methods (Java SE 24); InfoQ 2024-07-15; nipafx 2024-12-09; Dart's Hooks
and C interop pages; Flutter's developing-packages page and plugin template; npm's package.json and scripts pages;
node-gyp's README; SwiftPM's PackageDescription (twice); clang's Modules; PEP 513; auditwheel's README; the wheel spec;
setuptools' ext_modules; pkg-config's guide (Debian's copy); Debian's LAFileRemoval wiki, Policy ch. 8 and § 10.2, the
debian-devel post of 2009-09-03; Gentoo's QA page; LFS's la-files page; Cabal's package description (two chunks);
cabal-devel ticket 262; Deno's security page; the Ada 95 and Ada 2022 RM B.1.

**Refused or empty, so not cited as evidence**: openjdk.org/jeps/472 (403), openjdk.java.net/jeps/472 (403),
bugs.openjdk.org JDK-8331672 (403), mail.openjdk.org jdk-dev 2024-05 (403), the pkg-config guide at
people.freedesktop.org (403), lists.freedesktop.org pkg-config 2024-09 (403), Fedora's packaging guidelines (a bot
wall), the jakstys Zig mirror (connection refused), docs.rs's alef test (did not render), Debian's lintian tag page
(empty), cs.fsu.edu's Ada 95 RM (403), the doc.rust-lang.org paths to Cargo's `links.rs` (404 twice).

**Looked for and not found** (a statement about these searches, not about the world): the Zig version and issue that
introduced the libc error (Sema source at 0.14.0, the Zig issue tracker through web search, Ziggit, release notes of
0.12.0 by search); a Cargo page stating that an undeclared `#[link]` or `rustc-link-lib` draws no diagnostic (the
book, RFC 403, the Reference, internals and users forums); any tool that refuses a native declaration that binds
nothing (Cargo, SwiftPM, Cabal, Nimble, npm, Dart, Flutter, pkg-config, libtool pages above); a check in Dart,
Flutter, npm or Nimble of the package's code against its native declaration (their pages above). Not searched at all:
OCaml/dune, Erlang NIFs, .NET P/Invoke, Kotlin/Native cinterop, Julia's BinaryBuilder, Bazel's `cc_library` and
`linkopts`, Meson's dependency objects.

## The brief's questions, one line each

1. **Cargo `links`**: where, the package manifest; checked, form (a build script) and uniqueness in the graph, never
   against the code; pays, the build script's run and its metadata to immediate dependents; sources the Cargo book and
   RFC 403. Would change with a Cargo page stating an undeclared native link is refused.
2. **Go cgo**: where, the binding file (`import "C"`, `#cgo LDFLAGS`); checked, the import is the mark, and with cgo
   off the files drop out silently (`build constraints exclude all Go files`); pays, a C toolchain, external flags
   concatenated at link; sources cmd/cgo, Go 12860 and 24068. Would change if 24068's CL lands with an error rather
   than a note.
3. **Nim**: where, the binding's pragmas; checked, the pragmas are the mark, and Nimble's `foreignDep` prints a hint;
   pays, a link flag or a run-time `dynlib` lookup; sources the Nim manual and `distros`. Would change with a Nimble
   check of pragmas against `foreignDep`.
4. **Zig**: where, the binding's `extern "c"` plus the module's `link_libc` in `build.zig`; checked, a compile error,
   *"dependency on libc must be explicitly specified in the build command"*, for libc, libc++ and libunwind, and an
   automatic link for any other name; pays, libc for a declared module; source Sema.zig at 0.14.0. The version that
   introduced it is unverified.
5. **Java**: where, the application at launch (`--enable-native-access`, an executable JAR's manifest); checked, at
   run time, warn by default in JDK 24, deny planned; why, integrity, the approval of *"the application's developer"*;
   sources Oracle's guide and nipafx (the JEP pages unread, 403). Would change with JEP text weighing a library-side
   grant.
6. **Dart/Flutter**: where, the package (`hook/build.dart`, `ffiPlugin: true`); checked, no check described; pays,
   the hook runs on the importer's `run`, `build` and `test`; sources dart.dev Hooks and Flutter's template.
7. **npm**: where, `binding.gyp` in the package root (`gypfile` to opt out); checked, by presence only; pays,
   node-gyp on the importer's machine with Python and a C/C++ toolchain; sources npm docs and node-gyp's README.
8. **SwiftPM**: where, `Package.swift` per target (`systemLibrary`, `linkedLibrary`) and clang module maps' `link`;
   checked, against the code no, while `unsafeFlags` makes a product ineligible as a dependency; pays, travel by
   import (module map); sources PackageDescription and clang's Modules page.
9. **Python**: where, `ext_modules` and the wheel's `Root-Is-Purelib` and platform tag; checked, the flag is
   guidance, while auditwheel checks a wheel's ELF dependencies against the `manylinux` policy; pays, a platform wheel
   or a source build; sources the wheel spec, PEP 513 and auditwheel.
10. **pkg-config `Libs.private` and libtool `dependency_libs`**: where, both are the library's own, written where it
    is built; checked, neither against the objects; pays, `Libs.private` only on `--static`, while `dependency_libs`
    carried every direct and indirect dependency to every consumer, which Debian Policy § 10.2 now empties and Gentoo
    and LFS delete; sources the pkg-config guide, Debian, Gentoo and LFS.
11. **Go file names and `import "C"`**: where, the file's own name (`*_GOOS`) and the binding file's import; checked,
    neither (both are the mark); pays, nothing per file. History: Go 1.11 (2018-08-24) made `*_js.go` and `*_wasm.go`
    mean something new and asked people to rename files. Sources go help buildconstraint and the Go 1.11 notes.
12. **Beside them**: Cabal, the `.cabal` file, checked for presence at configure (ticket 262, 2008; the mechanism
    unverified); Deno, the user's `--allow-ffi` at run, for integrity; Rust `no_std`, the crate root, undone by any
    dependency; Ada `Linker_Options`, the unit, applied only where the unit is in the partition linked.
