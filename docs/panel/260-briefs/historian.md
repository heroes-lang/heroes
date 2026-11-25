# Panel 260, the historian's brief

Read `00-shared.md` first, whole: § 1 (the author's principle, which is the proposal), § 2 (the questions), § 3.2 (what
the language already has where a declaration could live) and § 7.1 to § 7.4 (the homes, the check, the diagnostic, a
declaration that binds nothing). **Your agent file is the base's** (`.claude/agents/historian.md`): every factual claim
carries a source, verified by a web search or a page you read, and **anything you could not confirm is written
`unverified`**. You have no veto. Your report is `docs/panel/260-reports/historian.md`, written as you go, in the tree
the coordinator's prompt names. You read pages; you start no paid run, download nothing, and save no file but your report.

## The question you answer

**Who has made a library declare, where it is defined, that it needs native code or the platform, and what did they
check?** The author's principle (`00-shared.md` § 1): the declaration is the library's, written where it is defined and
never by its importer; the compiler holds it to the code, an undeclared use being an error with no fallback; a program
that uses no such library pays nothing. Every precedent below is what the coordinator or the clerk recalls, **none
verified by this sitting**: each is a question for you, and a precedent you cannot source is written `unverified` or
dropped. For each, say:

- **(where)** where the declaration lives: the library's own manifest, its source at the binding, a build script, the
  importer's manifest or command line, or the user's run;
- **(checked)** whether the tool holds it to the code: an error when the code uses native linking undeclared, an error
  when declared and unused, a warning, or trust;
- **(pays)** what an importer pays for a declared dependency (a C toolchain, a build step, a link), and what a program
  that uses none pays;
- **(history)** what happened: the bugs, the complaints, the rule the project wrote down, with dates and versions.

1. **Cargo's `links` key** (recalled: `links = "foo"` in a package's `Cargo.toml` says it links a native library; at most
   one package in a dependency graph may name a given `links` value; a package with `links` must have a build script,
   which emits `cargo:rustc-link-lib`; dependents read its `DEP_<NAME>_<KEY>` metadata). Is a crate that links natively
   **without** `links` refused, or does `#[link(name = ...)]` work undeclared? Quote the Cargo book.
2. **Go's cgo** (recalled: a file that writes `import "C"` makes its package a cgo package; `#cgo LDFLAGS:` directives
   sit in the source beside it; `CGO_ENABLED=0` excludes cgo files by the `cgo` build constraint, so a package needing
   them fails to build). What does a program importing a cgo package pay (a C toolchain, external linking), and what
   does `go build` say when cgo is off and a package needs it?
3. **Nim's pragmas** (recalled: `{.importc.}`, `{.header: "x.h".}`, `{.passL: "-lfoo".}`, `{.compile: "x.c".}`,
   `{.dynlib.}`, at the binding in the source). Does anything check that a module using them is marked, or are they the
   mark themselves?
4. **Zig** (recalled: `linkSystemLibrary`, `linkLibC` or a module's `link_libc` in `build.zig`; and a compile error,
   recalled as *"dependency on libc must be explicitly specified in the build command"*, when code calls a C library
   function undeclared). If that error exists, it is the closest precedent to the author's *an error, never a
   fallback*: find its text, the version that introduced it, and the issue.
5. **Java's native access** (recalled: JEP 454's restricted methods and JEP 472's *"Prepare to Restrict the Use of
   JNI"*, the `Enable-Native-Access` attribute of an executable JAR's manifest and `--enable-native-access`, a warning
   today and a planned error). Is the grant the library's own, or the application's at launch? What do the JEPs say about
   why?
6. **Dart and Flutter** (recalled: a plugin's `pubspec.yaml` marking `ffiPlugin: true` per platform; Dart's native assets
   and a package's build hook, `hook/build.dart`). Declared at definition, and checked?
7. **npm** (recalled: `"gypfile": true` in `package.json`, or a `binding.gyp` beside it, makes the installer run
   `node-gyp`). Declared by the package, and what does an importer pay?
8. **Swift Package Manager** (recalled: `.systemLibrary(name:pkgConfig:providers:)` targets and `linkerSettings:
   [.linkedLibrary(...)]` in `Package.swift`). Checked against the code?
9. **Python packaging** (recalled: `ext_modules`, a wheel's `Root-Is-Purelib: false` and its platform tags). A package
   declares itself not pure at definition; an importer gets a platform wheel or builds one.
10. **The C world's own declarations, which the ffi-pragmatist's item 7 sends to you** (that seat has a shell and no
    web): **pkg-config's `Libs.private`** (recalled: a `.pc` file lists, apart from `Libs`, the libraries a static
    link of the library also needs, read by `pkg-config --static`) and **libtool's `.la` files' `dependency_libs`**
    (recalled: the libraries a libtool library was linked against, carried to whoever links it; recalled too that
    distributions came to delete `.la` files because of what they dragged onto link lines). Is either the library's
    own declaration, written where the library is defined, and is either checked against what the library's objects
    call? What did each cost the people who used it (overlinking, stale entries, the removals), with dates?
11. **Go's two other homes, which the critic named and no question asked** (`critic-briefs.md` § C): **a file-name
    build constraint** (recalled: `_linux.go`, `_windows_amd64.go` and the like make a file build only for that
    system, with no line inside it), the precedent for a declaration that is the binding file's own name, as every
    one of Rebel's 32 floors is named `hero_<name>.hero` (`00-shared.md` § 3.1); and **cgo's `import "C"` as a
    declaration by import** (recalled: the file that binds imports a pseudo-package, and that import is what makes the
    package a cgo one), the precedent for a `use` of the layer itself written in the module that binds. For each,
    (where), (checked), (pays), (history), as above.
12. **Beside them, if time allows, each sourced or dropped**: Haskell Cabal's `extra-libraries` and `c-sources`; Deno's
    `--allow-ffi`, a grant at the user's run (the opposite home); Rust's `#![no_std]`, a crate declaring the absence of
    the standard library at its root (the inverse declaration); and any tool where an importer, not the library,
    declares, so the sitting can say why the author's home is the other one.

## What the sitting does with it

The synthesis names, per resolution, the precedent it rests on, `verified` and re-read, or none
(`.claude/skills/panel/SKILL.md` § 4, `.claude/rules/records.md` § And the record says whose idea it was): a line of
prior art says **what the cited page itself states, and nothing of what Heroes did**, and is one of two forms, *Taken
from* or *Reached first by*. So quote the page's own words for every precedent you mark `verified`, with its URL and the
day you read it.

Output the structure of your agent file (`verdict`, `precedents` each marked verified or unverified, `argument` of at
most 120 words, `condition`, `search` with every query made and every source swept), then one line per numbered
question above: (where), (checked), (pays), its source, and what would change it.
