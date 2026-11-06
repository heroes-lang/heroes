# Panel 234, the historian's report

Written as it goes, 2026-11-05, by the historian seat (advisory, no veto).
Every precedent below was read on **2026-11-05** at the URL given, by a web
search and a fetch of the page in this session. **The instrument, said once**:
the fetch renders a page through a model and returns the passages asked for;
every passage in quotation marks below is what that fetch returned as the
page's own words, and a sentence of this report outside quotation marks is mine.
A second reader re-reading the page is the check `.claude/rules/records.md`
§ And the record says whose idea it was asks for before a `## Prior art` line is
drawn from this report. A precedent whose page could not be read is marked
*unverified* and nothing rests on it. The book *Heroes of code* is cited nowhere
in this report, as evidence or as route.

**Corrected before the report was handed in, 2026-11-05**, on the seat's own
re-reading of two earlier drafts of this file: the drafts said that Nim and
Node added their prefixes *after collisions* (their pages say the prefixes
exist to tell the library from packages, and neither says a collision came
first); that Odin designed `core:` in *from the start* (no page read says when
it arrived); that Zig and Rust choose their platform files *at build time*
(verified only for Go and Odin); that Qt's per-platform files are chosen at
build time (the page read shows only their names); that Goose met its import
hazard *within weeks of shipping a library* (its commit ties the change to
libraries fetched from elsewhere); and they counted their own lines and words
without an instrument. Each is repaired below.

## Summary block (the seat's fixed shape)

- **verdict**: **approve** (advisory), with three findings the synthesis owes a
  sentence each: the milestone page's PHP precedent does not hold as cited
  (item D); the language shaped most like Heroes here, Goose, met probe 1's
  hazard when it designed packages fetched from elsewhere and moved to
  per-package roots the same day, 2026-10-11 (item B); and the author's
  platform shape is the shape of libuv, APR, NSPR and GLib, while every
  language surveyed keeps that layer inside its one library (item F).
- **precedents**: per item below, each line marked *verified* (URL, the day
  read, the page's words) or *unverified*.
- **argument**: Every surveyed language keeps its library as ordinary code the
  compiler finds; Hare, Goose, Go and Odin state a rule for its line, and Go's
  FAQ records what lacking one early cost. Precedent favours a distinguishing
  prefix (Nim's `std/`, Node's `node:`, Odin's `core:`), the first two added
  later to tell the library from packages; Python calls the collision a common
  mistake. Goose, shaped most like Heroes, hit probe 1's hazard and moved to
  per-package roots. The milestone's PHP precedent fails as cited: the RFC
  blames names older than a coding standard and takes C's names as the repair.
  The author's platform shape is libuv's and APR's; every language surveyed
  keeps that layer inside its one library, and GLib folded its separately
  linked thread library back in.
- **condition**: in § Condition, at the end; four findings, any one of which
  would move a verdict.
- **search**: in § Search, at the end; every query, every page that failed,
  and what was looked for and not found, by item.

## Items A, B, C: the survey the author asked for

### The seven libraries, one by one

Each entry: what the library holds and refuses and whether a rule states the
line (A1); how a program reaches it (A2). Names and contracts are under C below.

**Go**

- *verified*: Go FAQ, *Why isn't X in the standard library?*,
  https://go.dev/doc/faq, read 2026-11-05: "The standard library's purpose is
  to support the runtime library, connect to the operating system, and provide
  key functionality that many Go programs require, such as formatted I/O and
  networking. It also contains elements important for web programming,
  including cryptography and support for standards like HTTP, JSON, and XML."
  On a rule: "There is no clear criterion that defines what is included because
  for a long time, this was the _only_ Go library. There are criteria that
  define what gets added today, however." and "New additions to the standard
  library are rare and the bar for inclusion is high." On the cost of
  admission: code there "bears a large ongoing maintenance cost (often borne by
  those other than the original author), is subject to the Go 1 compatibility
  promise (blocking fixes to any flaws in the API)". On what it cannot take
  back: "Although there are pieces in the standard library that don't really
  belong, such as `log/syslog`, we continue to maintain everything in the
  library because of the Go 1 compatibility promise."
- *verified*: how a program reaches it, `findGOROOT` in the `go` command,
  https://raw.githubusercontent.com/golang/go/master/src/cmd/go/internal/cfg/cfg.go,
  read 2026-11-05: "findGOROOT returns the GOROOT value, using either an
  explicitly provided environment variable, a GOROOT that contains the current
  os.Executable value, or else the GOROOT that the binary was built with from
  runtime.GOROOT()." The environment variable is returned after
  `filepath.Clean` and nothing else; a directory two or three levels above the
  executable is taken where `isGOROOT` holds, which it decides "by looking for
  the path/pkg/tool directory".
- *verified*: how a standard path is told from any other,
  https://go.dev/ref/mod, read 2026-11-05: "The Go standard library uses
  package paths that do not contain a dot in the first path element, and the
  `go` command does not attempt to resolve such paths from network servers."
- *verified*: the search path came first and a manifest after it,
  https://go.dev/doc/go1.11, read 2026-11-05: "Go 1.11 adds preliminary support
  for a new concept called "modules," an alternative to GOPATH with integrated
  support for versioning and package distribution. Using modules, developers
  are no longer confined to working inside GOPATH".

**Odin**

- *verified*: Odin FAQ, https://odin-lang.org/docs/faq/, read 2026-11-05,
  *Why isn't X in the core library?*: "The core library is not yet complete."
  "However when it is complete, it will remain small, as its purpose is to
  support the runtime," "operating system specific calls, formatted I/O, and
  other key functionality that most Odin programs require." For `vendor`: "The
  criteria is approximately the following:" "Widely useable", "Does not cause
  tons of maintenance burden", "Ginger Bill does not hate it". On a package
  manager: "Odin will never officially support a package manager.", "Through
  manual dependency management.", "Copying and vendoring each package manually,
  and fixing the specific versions down is the most practical approach".
- *verified*: the three collections, https://pkg.odin-lang.org/, read
  2026-11-05: base, "The base every Odin program builds on: the runtime,
  built-in procedures, and compiler intrinsics." and "Required by every target
  and implementation of Odin."; core, "The core library: formatted printing,
  strings, memory and allocators, containers, math, encoding," "OS access,
  threading, and more."; vendor, "Bindings to popular third-party libraries for
  graphics, windowing, audio, physics, and more, all shipped with the compiler."
- *verified*: the line between `core:` and `vendor:`,
  https://raw.githubusercontent.com/odin-lang/Odin/master/vendor/README.md,
  read 2026-11-05: "The `vendor:` prefix for Odin imports is a package
  collection" that comes with this implementation; "Its use is similar to that
  of `core:` packages, which would be available in any Odin implementation."
  "These packages and their associated binary files are curated and maintained
  by the Odin team."
- *verified*: how a program reaches it, https://odin-lang.org/docs/overview/,
  read 2026-11-05: "The `core:` prefix is used to state where the import is
  meant to look; this is called a library collection." "If no prefix is
  present, the import will look relative to the current file." Custom
  collections come from "the `-collection:<name>=<path>` command line switch".
- *verified*: where `core:` is on disk, the compiler's own source,
  https://raw.githubusercontent.com/odin-lang/Odin/master/src/build_settings.cpp,
  read 2026-11-05: `odin_root_dir()` reads `gb_get_env("ODIN_ROOT", a)` first
  and otherwise falls back to `internal_odin_root_dir()`, which takes the
  running executable's own path (`GetModuleFileNameW`, `_NSGetExecutablePath`,
  `readlink("/proc/self/exe", ...)`) cut back to its directory;
  `get_fullpath_core_collection` joins that root with `"core/"`. The excerpt
  read holds no check of what that directory contains.

**Zig**

- *verified*: a stated inclusion rule, none found. The nearest text is issue
  #1629, *audit the standard library API and implementations*,
  https://github.com/ziglang/zig/issues/1629, read 2026-11-05, opened by
  andrewrk on 2018-10-03, open, milestone pre-1.0: "Remove unnecessary APIs.
  They can go into third party maintained projects/packages." and "Compare with
  other language std libs, make sure we are not missing some critical
  component".
- *verified*: how a program reaches it, the README at the repository's new home,
  https://codeberg.org/ziglang/zig/raw/branch/master/README.md, read
  2026-11-05: "At runtime, the executable searches up the file system for the
  lib/ directory," relative to itself, "lib/", "lib/zig/", "../lib/",
  "../lib/zig/", "(and so on)"; "In other words, you can **unpack a release of
  Zig anywhere**, and then begin using it immediately." (The GitHub README at
  https://raw.githubusercontent.com/ziglang/zig/master/README.md, read the same
  day, says "Moved to Codeberg" and "This repository is not mirrored.")
- *verified*: the manifest came later,
  https://ziglang.org/download/0.11.0/release-notes.html, read 2026-11-05:
  "Zig 0.11 is the début of the official package manager." "Package
  information is declared in a file named `build.zig.zon`."

**Hylo**

- *verified*: the implementation moved,
  https://raw.githubusercontent.com/hylo-lang/hylo-old/main/README.md, read
  2026-11-05: "# Outdated Implementation of Hylo", "The current implementation
  is in https://github.com/hylo-lang/hylo-new".
- *verified*: what it holds; no stated rule found. The former implementation
  splits the library in two by its doc comments,
  https://raw.githubusercontent.com/hylo-lang/hylo-old/main/StandardLibrary/StandardLibrary.swift,
  read 2026-11-05: "The root of a directory hierarchy containing all the
  standard library sources.", "The root of a directory hierarchy containing the
  sources for the standard library's freestanding core." (its `Core` folder,
  built with `ConditionalCompilationFactors(freestanding: true)`), and
  "Location of prebuilt C libraries that the standard library depends on."
  Its `Sources/LibC.hylo`
  (https://raw.githubusercontent.com/hylo-lang/hylo-old/main/StandardLibrary/Sources/LibC.hylo)
  binds `malloc`, `free`, `fdopen`, `fwrite`, `memcpy` and others with
  `@ffi("...")`. The new implementation's `StandardLibrary/Sources`
  (https://github.com/hylo-lang/hylo-new/tree/main/StandardLibrary/Sources)
  lists `Core` and `shims.c`, whose top comment reads "C implementations of
  standard library functions declared with `@extern_c_indirect`."
  (https://raw.githubusercontent.com/hylo-lang/hylo-new/main/StandardLibrary/Sources/shims.c),
  two functions, `c_malloc_indirect` and `c_free_indirect`, and no platform
  conditional.
- *verified*: how a program reaches it,
  https://raw.githubusercontent.com/hylo-lang/hylo-new/main/StandardLibrary/StandardLibrary.swift,
  read 2026-11-05: "The path to the bundled standard library's root folder."
  "This folder is intended to be used in distributable builds, in order to
  bundle the standard library together with the executable." The code finds a
  `Hylo_StandardLibrary.resources` directory beside the executable (or a
  package resource), and a local folder found by `#filePath` "should be
  preferred during development".

**Hare**

- *verified*: a stated rule, the standard library mandate,
  https://docs.harelang.org/, read 2026-11-05: "The standard library mandate
  states that the following services are provided:" "An interface to the host
  operating system", "Implementations of broadly useful algorithms",
  "Implementations of broadly useful formats and protocols", "Useful features
  to complement Hare language features", "Introspective meta-features for
  Hare-aware programs". The four requirements each service meets, as Drew
  DeVault quoted the mandate on 2021-03-19,
  https://harelang.org/blog/2021-03-19-stdlib-development/, read 2026-11-05:
  "Each of these services shall:" "Have a concise and straightforward
  interface", "Correctly and completely implement the useful subset of the
  required behavior", "Provide complete documentation for each exported
  symbol", "Be sufficiently tested to provide confidence in the
  implementation".
- *verified*: how a program reaches it, a search path,
  https://harelang.org/documentation/usage/modules.html, read 2026-11-05: "The
  search path always starts with the current working directory, then checks
  each of the default search paths in order.", conventionally
  "/usr/src/hare/third-party" and "/usr/src/hare/stdlib"; "If the HAREPATH
  environment variable is set, the default search paths are not used at all."

**Nim**

- *verified*: what it holds, a classification and no admission rule,
  https://nim-lang.org/docs/lib.html, read 2026-11-05: "Nim's library is
  divided into pure libraries, impure libraries, and wrappers." "Pure libraries
  do not depend on any external *.dll or lib*.so binary while impure libraries
  do." "A wrapper is an impure library that is a very low-level interface to a
  C library."
- *verified*: how a program reaches it, a search path,
  https://nim-lang.org/docs/nimc.html, read 2026-11-05: "Nim has the concept
  of a global search path (PATH) that is queried to determine where to find
  imported modules" "or include files. If multiple files are found an
  ambiguity error is produced." "However before the PATH is used the current
  directory is checked for the file's existence."
- *verified*: what `std/` and `pkg/` were for, the manual's *Pseudo
  import/include paths*,
  https://raw.githubusercontent.com/nim-lang/Nim/devel/doc/manual.md, read
  2026-11-05: "They can be used to avoid ambiguity when there are multiple
  modules with the same path." "`std`: The `std` pseudo directory is the
  abstract location of Nim's standard library." "For example, the syntax
  `import std / strutils` is used to unambiguously refer to the standard
  library's `strutils` module." "`pkg`: The `pkg` pseudo directory is used to
  unambiguously refer to a Nimble package." "It is recommended and preferred
  but not currently enforced that all stdlib module imports include the std/
  "pseudo directory" as part of the import name."
- *verified*: when and why the prefix arrived, Nim 0.18.0's announcement,
  https://nim-lang.org/blog/2018/03/01/version-0180-released.html, read
  2026-11-05, dated 01 March 2018: "Standard library modules can now also be
  imported via the `std` pseudo-directory." "This is useful in order to
  distinguish between standard library and nimble package imports".

**Goose**

- *verified*: a stated rule,
  https://raw.githubusercontent.com/aardappel/goose/master/docs/design/stdlib_design.md,
  read 2026-11-05, § 1: "A function is included only when the usual
  hand-written loop takes more than three lines or is easy to get wrong," "as
  with sorting, hashing, UTF-8, float printing, and binary search." § 9 opens
  "These operations can be expressed using existing functions and short
  loops:" and lists, among others, "`lines`: `each_split(s, '\n')` plus
  `trim`." § 2.2: "There is no `Option<T>` value type and the library does not
  invent one."
- *verified*: the wording read today is not the milestone page's quotation of
  2026-10-25 (*"is the idiomatic hand-written loop more than three lines, or is
  it easy to get subtly wrong ... If neither, it is not in the library"*). The
  file's history, https://github.com/aardappel/goose/commits/master/docs/design/stdlib_design.md,
  read 2026-11-05, shows two commits on 2026-10-25, "Rewrite dense and
  formulaic documentation prose" and "Rewrite LLM-isms and over-dense
  sentences in the docs", and three after them (2026-10-26, 2026-10-29,
  2026-11-04). So the two readings may be of two versions; both carry the two
  halves, three lines or easy to get wrong.
- *verified*: how a program reaches it, the same file, § 3: "The compiler
  resolves `import std;` by looking in three places, in order." "First it
  looks in the importing file's package root"; "Next it looks in `--stdlib
  <dir>` or `GOOSE_STDLIB`, if given." "Finally it walks up from its own
  executable's directory, looking for `stdlib/std.goose` (three levels)." The
  language's specification says the last step differently,
  https://raw.githubusercontent.com/aardappel/goose/master/docs/goose_spec.md
  § 11.1, read 2026-11-05: "Last, it is resolved in the standard library's
  directory (`--stdlib <dir>`, else `GOOSE_STDLIB`, else" "the `stdlib/` of
  the source tree the compiler was built in)."

### A1, read across: whose line is a rule

Four state one: **Hare** (five services and four requirements each meets),
**Goose** (three lines or easy to get wrong, with a published refused list),
**Go** (a purpose and a high bar, and in its own words no clear criterion for
what was already there), **Odin** (small, to support the runtime, OS calls,
formatted I/O; and three criteria for `vendor:`). Three state none that I
found: **Zig** (an open pre-1.0 audit issue), **Nim** (a classification by
what a library links), **Hylo** (a freestanding core and a hosted rest, by
directory). Go's FAQ is the one that says what having no rule early cost:
a library it would not admit today and cannot remove, `log/syslog`, kept by the
compatibility promise. Heroes' frame 2 removes that cost until `1.0.0`.

**Verdict (A)**: the precedent favours a stated admission rule with a
published refused list, **moderately**: Hare and Goose state one, Go records
the cost of the years without one. No precedent tells the sitting where
§1.11's line falls; that is item A's own question.
**Prediction (A)**: if the sitting adopts Goose's test, at least one function
Goose's own § 9 refuses (`lines`, *"`each_split(s, '\n')` plus `trim`"*) is
admitted to `core/strings`, because the loop Goose's refusal rests on calls an
`each_split` Heroes does not have (the critic's F1: no `split` built-in).
Checked at the step that writes `core/strings`.
**Condition (A)**: a language that stated an admission rule and later recorded
it as the cause of a library too thin to use would weaken the verdict.

### B: where the packages live and how a program reaches them

**B1 and B2, a bare path or a distinguishing prefix.**

- *verified*: Nim added `std/` in 2018 "to distinguish between standard
  library and nimble package imports" and still only recommends it (both
  pages above).
- *verified*: Node.js, https://nodejs.org/api/modules.html, read 2026-11-05:
  "Built-in modules can be identified using the `node:` prefix, in which case
  it bypasses the `require` cache." "Some built-in modules are always
  preferentially loaded if their identifier is passed to `require()`. For
  instance, `require('http')` will always return the built-in HTTP module,
  even if there is a file by that name." And of the modules that must carry
  it: "This requirement exists to prevent newly introduced built-in modules
  from having a conflict with user land packages that already have taken the
  name." (`node:sqlite`, `node:test` and three more). Its history: `node:`
  support was added to `require(...)` in "v16.0.0, v14.18.0".
- *verified*: Python records the collision B1 makes structural as a common
  mistake, https://docs.python.org/3/whatsnew/3.13.html, read 2026-11-05: "A
  common mistake is to write a script with the same name as a standard library
  module. When this results in errors, we now display a more helpful error
  message:", the message ending "(consider renaming '/home/me/random.py' since
  it has the same name as the standard library module named 'random' and
  prevents importing that standard library module)".
- Odin's `core:` (above) is a prefix its documents present as the collection
  mechanism itself; when it arrived in the language I did not search.

So B2's `core/` has three precedents, two of them (Nim's, Node's) added later
in their language's life to tell the library from packages and user files.
Under B1 there is nothing for a prefix to tell apart, since the program's own
`strings.hero` **is** the package's path (the critic's F5): that is the
collision Python's message describes, with no second location to report.

**The history the brief asks about (panel 099's half and the critic's F34
half).** Whether any language that began with imports resolved from the
program's own root, with no manifest and no search path, was later forced to
add a manifest or a search path.

- *verified*: **Goose**, the nearest shape. Commit b59e41d,
  https://github.com/aardappel/goose/commit/b59e41df068ab7b49d0e7551c709394271c8cba8,
  read 2026-11-05, dated 2026-10-11 by the file history above: "Ast::packages
  and Ast::filepkg record which package each source belongs to: the program
  (its root file's directory)" "or a standard library directory." "An
  `import a.b;` in a library module now resolves under the library, not the
  program (spec §11.1)," "which is what a library fetched from elsewhere
  needs." The specification today, § 11.1 (URL above): "The file is first
  resolved relative to the root of the importing file's package:" "for the
  program's own files that is the directory of the root file being compiled,"
  "and for a library's modules the library's directory." And dependencies,
  designed the same day ("Design: goose build, packages from git repositories,
  and the migration plan", 2026-10-11, in
  https://github.com/aardappel/goose/commits/master/docs/design/packages.md):
  "`import name from "source";` in the *root file* declares the package
  `name`", "Only the root file declares packages, so a program's dependencies
  are flat and in one place." (§ 11.1), and
  https://raw.githubusercontent.com/aardappel/goose/master/docs/design/packages.md:
  "Dependencies are declared in the root source file, as imports with a
  source:", "Lockfiles, version ranges and a registry are not used:"; fetched
  packages go to a cache under `GOOSE_HOME`, by default `~/.goose` (the fetch's
  summary of that page, not its quoted words). The commit's own words name
  "a standard library directory" beside the program's root, so Goose did not
  begin with **no** search; it is not the case the brief describes. What it
  shows is the hazard probe 1 measured, a library's module whose own import
  read from the program's root, met and repaired by giving each package its
  own root, not by a manifest file.
- *verified*: **Deno**, file- and URL-relative rather than root-relative, began
  with no manifest and added one. https://deno.com/blog/v1, read 2026-11-05,
  2020-05-13: "There are no additional configuration files, there is no
  install to do beforehand, just deno run example.js."
  https://deno.com/blog/v2.0, read 2026-11-05, 2024-10-09: "Deno 2 understands
  `package.json`, the `node_modules` folder, and even npm workspaces";
  "Deno 2 is backwards compatible with Node and npm."; "supporting Node and npm
  compatibility is essential".
- *verified*: **the browser's modules**, URL-relative, began with no mapping
  and added one. The archived explainer,
  https://github.com/WICG/import-maps/blob/abc4c6b24e0cc9a764091be916c5057e83c30c23/README.md,
  read 2026-11-05: "This proposal allows control over what URLs get fetched by
  JavaScript `import` statements and `import()` expressions." "This allows
  "bare import specifiers", such as `import moment from "moment"`, to work."
  "Today, this throws, as such bare specifiers are explicitly reserved."; and
  https://github.com/WICG/import-maps, read 2026-11-05: "Import maps are now
  part of the HTML Standard."
- *verified* above: **Zig** (file-relative, the library beside the
  executable) added a manifest in 0.11; **Go** began with a search path
  (GOPATH) and added a manifest in 1.11.
- *unverified*: **Elm**, whose early compiler may have found modules from the
  directory it ran in before `elm-package.json`'s `source-directories`; I found
  no primary page for the rule before 0.14 (§ Search).

So, extending panel 099's search (`docs/panel/099-where-a-use-path-starts.md:189-191`)
to the search-path half: **no documented case found** of a language that
began root-relative with neither manifest nor search path and was later forced
to add a search path. What was found is adjacent and points one way: the
languages that began with no manifest (Deno, the browser) added a mapping or a
manifest under an ecosystem's pressure, and the one language shaped most like
Heroes (Goose) kept root-relative resolution for the program and gave each
library its own root as soon as a library lived outside it. **For Heroes that
reads as: B1, B2, B3 and B6 keep every package under the program's root, where
panel 099's rule already reaches; B4 and B5 put the tree outside it, where a
package's own `use core/strings` must resolve against the tree's root and not
the program's, which is Goose's change of 2026-10-11.** That last sentence is
my inference from the commit's own words, not the commit's claim.

**B and F coupled (the critic's F15): a package that ships its own header.**

- *verified*: Goose met it and answered with a file per package. PR #141,
  https://github.com/aardappel/goose/pull/141, read 2026-11-05, opened and
  merged 2026-10-29: "Adds `c_headers = ["include/adapters.h"];` to package and
  program sidecars", "This lets packages supply inline C adapters without
  consumer flags or separate compilation.", "Paths are package-relative;
  platform suffixes are supported." The sidecar, `packages.md` (URL above):
  "Information that is not code (C sources to compile, system libraries, CMake
  options) goes in an optional sidecar file", named `<name>.build` beside the
  entry file, and "Every `c_*` key takes a `_windows`, `_linux` or `_macos`
  suffix for a platform-only part."

That is F15's case, a package's `static inline` header found from the
package's own place without a flag at the consumer, and P3 and P4 together
(`c_headers` and `c_sources`, each with a per-platform suffix), held in a
per-package file. The sitting weighs it against panel 056's refusal of a
per-project file, panel 036's standing on `compile` and panel 114 R3's
instruments; this report rules none of them.

**B5 and B6, the library inside or beside the compiler, and a guard against a
decoy (panel 028 R3's trust argument).**

- *verified* (above): beside the compiler, found from the executable's own
  path: **Go** (`findGOROOT`), **Odin** (`odin_root_dir`), **Zig** (the
  README's walk up to `lib/`), **Hylo** (`Hylo_StandardLibrary.resources`
  beside the executable), **Goose** (§ 3's last step).
- *verified*: inside the binary. Node.js, same page as above: "Node.js has
  several modules compiled into the binary." Python,
  https://docs.python.org/3/whatsnew/3.11.html, read 2026-11-05: "In Python
  3.11, the core modules essential for Python startup are "frozen"." and
  "Interpreter startup is now 10-15% faster in Python 3.11."
- *verified*: the guards found. **Go** checks a candidate's shape, a
  `pkg/tool` directory, and takes `GOROOT` from the environment unchecked
  (above). **Python** 3.11, same page: "Added a `-P` command line option and a
  `PYTHONSAFEPATH` environment variable, which disable the automatic
  prepending to `sys.path` of the script's directory when running a script,
  or the current directory when using `-c` and `-m`. This ensures only stdlib
  and installed modules are picked up by `import`, and avoids unintentionally
  or maliciously shadowing modules with those in a local (and typically
  user-writable) directory." **Node** gives some built-ins precedence over a
  file of the same name (above). In Odin's root-finding code and Zig's README I
  read no check of what the directory holds; Goose's § 3 looks in "the
  importing file's package root" first, before the library, which by my
  reading lets a program's own file stand where the library's would (an
  inference from the order the page states).
- B6, one canonical tree through a symbolic link: **not searched**; nothing
  here bears on it.

So panel 028 R3's argument meets no counter-precedent: of the languages read,
none verifies a library tree's contents before compiling against it; the
guards that exist are a shape check (Go), an opt-in that removes the program's
own directory from the search (Python), and precedence for built-ins (Node).
Compiling the library in (B5) has two precedents, Node's built-ins and
Python's startup modules, the second given for speed.

**Verdict (B)**: the precedent favours **B2, a distinguishing prefix under the
program's root, moderately to strongly**: Nim and Node added theirs later to
tell the library from packages and user files, Odin's documents present its
prefix as the collection mechanism itself, and Python records the collision as
a common mistake. Against B4 and B5 it says two things: a root outside the
program's tree needs per-package roots (Goose's change, under a month old, so
weakly), and no language read guards that root against a decoy (so 028 R3
stands unanswered by precedent rather than refuted by it).
**Prediction (B)**: if B4 or B5 lands, the first package of this milestone
that `use`s another package and is reached from a program outside the tree's
root is refused `unknown_module` until resolution is per package, Goose's
change. Checked at the first step that writes a package using another
(`net/http` over `strings`, step 1 or 6), by compiling it from a program
outside the tree.
**Condition (B)**: a language that kept one program-root rule for a library
living outside the program's root, without per-package roots or a manifest,
and recorded no defect for it, would remove the second half of the verdict.

### C: names and contracts, for the author's rule

**The position of a substring, and what each answers when it is absent.**

| language | name | absent | source (read 2026-11-05) |
|---|---|---|---|
| Go | `strings.Index` | `-1`: "Index returns the index of the first instance of substr in s, or -1 if substr is not present in s." | https://pkg.go.dev/strings |
| Odin | `strings.index` | `-1`: "Returns the byte offset of the string `substr` in the string `s`, -1 when not found." | https://pkg.odin-lang.org/core/strings/ |
| Zig | `std.mem.find` (was `indexOf`) | `null`, an optional: `pub fn find(...) ?usize`, "Returns null if needle is not found."; `pub const indexOf = find;` under "Deprecated in favor of `find`." | https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/mem.zig |
| Rust | `str::find` | `None`: `-> Option<usize>`, "Returns `None` if the pattern doesn't match." | https://doc.rust-lang.org/std/primitive.str.html |
| Hare | `strings::index` | `void`: `(size \| void)`, "or void if not present."; "The index returned is the rune-wise index, not the byte-wise index." | https://docs.harelang.org/strings |
| Nim | `strutils.find` | `-1`: "If sub is not in s, -1 is returned." | https://nim-lang.org/docs/strutils.html |
| Python | `str.find`, `str.index` | `-1`, and `ValueError`: "Return `-1` if _sub_ is not found."; "Like find(), but raise ValueError when the substring is not found." | https://docs.python.org/3/library/stdtypes.html |
| Swift | `firstRange(of:)` | `nil`: `-> Range<Index>?`, "Returns nil if `sequence` is not found." (SE-0357, "Implemented (Swift 5.7)") | https://github.com/swiftlang/swift-evolution/blob/main/proposals/0357-regex-string-processing-algorithms.md |
| Swift, Foundation | `range(of:)` | `nil` on `StringProtocol`: `-> Range<Self.Index>?`; on `NSString`: "Returns `{NSNotFound, 0}` if `searchString` is not found or is empty" | https://developer.apple.com/documentation/swift/stringprotocol/range(of:options:range:locale:).md and https://developer.apple.com/documentation/foundation/nsstring/range(of:).md |
| Goose | `find` | `-1`: "Positions are `i64`, `-1` for none." | https://raw.githubusercontent.com/aardappel/goose/master/docs/stdlib.md |

On F34: Swift's names for a substring's position are the ones the critic
expected, `firstRange(of:)` in the standard library and `range(of:)` from
Foundation; `firstIndex(of:)` is not among SE-0357's declarations, and I did not
fetch its own page.

**A familiar name kept while its contract changed**, each *verified* on the
page in the table or named here:

- **Swift**: `range(of:)` answers Foundation's sentinel on `NSString` and an
  optional on Swift's `StringProtocol`, one name across the two.
- **Zig**: `cut`, Go's name, answers an optional: "Returns slice of `haystack`
  before and after first occurrence of `needle`," "or `null` if not found."
  (`std.mem`, URL in the table).
- **Hare**: `index` answers `void` and counts runes; `cut` drops Go's flag
  altogether, `fn cut(in: str, delim: str) (str, str);`, "The caller must
  ensure that 'delim' is not an empty string." (URL in the table).
- **Odin**: `cut` is another operation under Go's name: `cut :: proc(s:
  string, rune_offset: int = int(0), rune_length: int = int(0)) -> (res:
  string)`, "Returns a substring of the input string `s` with the specified
  rune offset and length" (URL in the table).
- **Rust and Zig** answer an optional from `find`, the name Python and Nim
  give the `-1` contract.

**Whether any of them recorded it as a source of mistakes.** None found for a
reused name (§ Search names the queries). What is on the record is about the
**sentinel contract itself**, and about a **second name** beside it:

- *verified*: Python, python-dev, 2005-08-27,
  https://mail.python.org/pipermail/python-dev/2005-August/055744.html, read
  2026-11-05: Raymond Hettinger quotes Guido van Rossum, "However, after 12
  years, I believe that the small benefit of having" "find() is outweighed by
  the frequent occurrence of bugs in its use.", and answers "My little code
  transformation exercise is bearing that out. Two of the" "first four cases in
  the standard library were buggy :-(". The thread's subject is *Remove
  str.find in 3.0?* (https://mail.python.org/pipermail/python-dev/2005-August/055707.html).
- *verified*: what Python did instead, https://docs.python.org/3/whatsnew/2.5.html,
  read 2026-11-05: "The `find(S)` method is often used to get an index which is
  then used to slice the string and obtain the pieces that are before and
  after the separator. `partition(sep)` condenses this pattern into a single
  method call", "(Implemented by Fredrik Lundh following a suggestion by
  Raymond Hettinger.)". `find` stayed: its documentation today (URL in the
  table) says "The `find()` method should be used only if you need to know the
  position of _sub_. To check if _sub_ is a substring or not, use the in
  operator".
- *verified*: Go, issue #46336, *bytes, strings: add Cut*,
  https://github.com/golang/go/issues/46336, read 2026-11-05, opened by rsc on
  2021-05-24, milestone Go1.18: "311 Index calls outside examples and
  testdata.", "That leaves 285 calls. Of those, 221 were better written as
  Cut, leaving 64 that were not.", "That is, 77% of Index calls are more
  clearly written using Cut. That's incredible!" The fetch found no sentence
  calling `Index` error-prone.

So the two records found, Python's and Go's, each **added a second name for a
second contract and kept the first**: `partition` beside `find`, `Cut` beside
`Index`; Python's `index` already stood beside `find` (Hettinger's message of
the day before, https://mail.python.org/pipermail/python-dev/2005-August/055706.html,
read 2026-11-05, rewrites `zipfile.py`'s `rfind` as `rindex` inside a
`try`/`except ValueError`). That is the author's rule as practised inside one
library, where both contracts live. The languages that wrote a library around
an optional from the start (Rust, Zig, Hare, Swift's overlay) kept the
familiar names for the optional contract, and I found no record of it costing
them.

**Splitting text into lines**, near names, different contracts:

- *verified*: Go 1.24's `strings.Lines` (URL in the table): "Lines returns an
  iterator over the newline-terminated lines in the string s." "The lines
  yielded by the iterator include their terminating newlines."
- *verified*: Rust's `str::lines` (URL in the table): "Line terminators are not
  included in the lines returned by the iterator."; split at `\n` or `\r\n`;
  "any carriage return (`\r`) not immediately followed by a line feed (`\n`)
  does not split a line."; "The final line ending is optional."
- *verified*: Python's `str.splitlines` (URL in the table): "Line breaks are
  not included in the resulting list unless *keepends* is given and true.",
  eleven boundaries, `\v`, `\f`, `\x1c`, `\x1d`, `\x1e`, `\x85`, `\u2028`,
  `\u2029` among them; "Changed in version 3.2: `\v` and `\f` added to list of
  line boundaries."; "this method returns an empty list for the empty string,
  and a terminal line break does not result in an extra line".
- *verified*: Nim's `splitLines` (URL in the table): "Every character literal
  newline combination (CR, LF, CR-LF) is supported." "The result strings
  contain no trailing end of line characters unless the parameter keepEol is
  set to true."
- *verified*: Odin's `split_lines` (URL in the table): "Splits the input string
  at every line break `\n`."; the page names no `\r`.
- Zig's `std.mem` and Hare's `strings` pages read show no line splitter
  (`splitScalar`, `tokenize`, `split` are there); Goose refuses one (§ 9,
  above).

Go's `Lines` keeps the terminator that Rust's `lines` drops, as Python's
`splitlines` and Nim's `splitLines` drop it by default: the one name `lines`
already means two contracts in Go and Rust.

**The failure convention of the whole library.**

- *verified*: Go, https://go.dev/doc/effective_go, read 2026-11-05: "Library
  routines must often return some sort of error indication to the caller. As
  mentioned earlier, Go's multivalue return makes it easy to return a detailed
  error description alongside the normal return value." "By convention, errors
  have type `error`, a simple built-in interface."
- *verified*: Odin, FAQ (above): "Odin uses plain error handling through the
  use of multiple return values."; overview (above): "Note an error is
  indicated by `ok` being `false`", "Odin has multiple return values;
  `Maybe(T)` is used less frequently or rarely in the `core` library."
- *verified*: Zig, https://ziglang.org/documentation/master/, read 2026-11-05:
  "An error set is like an enum." "An error set type and normal type can be
  combined with the `!` binary operator to form an error union type."
- *verified*: Rust, https://doc.rust-lang.org/std/result/index.html, read
  2026-11-05: "Error handling with the `Result` type." "`Result<T, E>` is the
  type used for returning and propagating errors."
- *verified*: Hare, https://harelang.org/tutorials/introduction/, read
  2026-11-05: "This type is a _tagged union_ which represents each of the
  errors that can be caused during file operations."; "Note the `!` operator
  which follows the function call: this is the error assertion operator."; of
  `?`: "The purpose of this operator is to check for errors and, if found,
  return them to a higher call frame."
- *verified*: Nim, `parseInt` (URL in the table): "If s is not a valid
  integer, ValueError is raised."; and `find`'s `-1`.
- *verified*: Goose, stdlib.md (URL in the table): "Two-outcome scalars return
  a trailing `bool`: `let v, ok = parse_int(s);`.", ""Maybe an element" is a
  `T?`: a reference into the input, or null for none.", "Positions are `i64`,
  `-1` for none."

On the critic's F23 (whether Go's `(T, error)` and a Heroes `T?` are one
contract): Effective Go's own words put the error "alongside the normal return
value", a value handed back beside the result; that is the shape the
coordinator's boundary calls one contract with `T?`. Precedent does not rule
the boundary; it shows the two languages that met a sentinel inside their own
library naming the new contract apart.

**Verdict (C)**: the precedent is **split, and weak either way**. For the
author's rule: Python and Go, the two libraries that recorded a sentinel's
cost, gave the new contract a new name. Against its strict reading: every
library built around an optional from the start kept the familiar name, and
no record of harm was found. The one recorded harm, Python's, is the `-1`
itself, which a `T?` removes under either name, and Heroes refuses Go's
contract written on a `T?` (`bad_operand`, the shared brief's probe 4).
**Prediction (C)**: in the reader test's bare arms, the failures at the four
renamed or kept functions are compile errors in both arms and no wrong answer
in either: in the arm with Go's names, Go's contract written on a `T?`
(`bad_operand`); in the arm with other names, Go's name written anyway (an
unknown name with no suggestion, probe 4). Checked when experiment S's
results are read.
**Condition (C)**: a tracker entry, in any language, recording mistakes caused
by a familiar library name kept with an optional where the original had a
sentinel would move the verdict toward the author's rule.

## Item D: thin or thick over C

**The milestone page's PHP precedent, verified and read whole.**

- *verified*: https://www.php.net/manual/en/mysqlinfo.library.choosing.php,
  read 2026-11-05: "The mysqli and PDO_MySQL PHP extensions are lightweight
  wrappers on top of a C client library. The extensions can either use the
  mysqlnd library or the libmysqlclient library. Choosing a library is a
  compile time decision." The wording the milestone page quotes is exact. The
  same page: "The mysqlnd library is part of the PHP distribution." and "It is
  recommended to use the mysqlnd library instead of the MySQL Client Server
  library (libmysqlclient)."
- *verified*: PDO is the other shape, a face over drivers. The manual's source
  for its introduction, https://raw.githubusercontent.com/php/doc-en/master/reference/pdo/book.xml,
  read 2026-11-05 (php.net's own `intro.pdo` URLs answered 404 to the fetch,
  while the search engine lists them with the same sentence): "The PHP Data
  Objects (PDO) extension defines a lightweight, consistent interface for
  accessing databases in PHP."; "you must use a database-specific PDO driver to
  access a database server."; "PDO provides a data-access abstraction layer,
  which means that, regardless of which database you're using, you use the same
  functions to issue queries and fetch data."
- *verified*: the RFC, https://wiki.php.net/rfc/consistent_function_names,
  read 2026-11-05: "Version: 0.9", "Date: 2015-03-01", "Author: Yasuo
  Ohgaki", "Status: Under Discussion", target "PHP 7.0". What it proposed:
  "First of all, this RFC will not remove any old function names." "This RFC
  only proposes renaming offending function names and have aliases for old
  names." "Old names are _not_ deprecated." The argument order: "String and
  array function have different parameter order for needle and subject to be
  searched." What became of it: the RFC index, https://wiki.php.net/rfc, read
  2026-11-05, lists it under **"Inactive"**; the RFC's own page records no vote.
- *verified*, and it is the finding: **the cause the RFC names is not C**.
  Same page: "There are many functions that named before naming standard in
  CODING_STANDARDS." "Therefore, PHP has function names like phpversion() and
  htmlspecialchars()." And C's names and order are its **repair**: "We have
  many function names that comply IEEE 1003.1 standard names." "IEEE 1003.1
  names can be use as aliases." "Other well established library, e.g. libc,
  function name may be defined as alias of main function" "even if it
  violates PHP coding standards as long as it has module prefixes." "Fix array
  function parameter ordering issue by using IEEE parameter order."
- *verified*: the mitigation the page names,
  https://wiki.php.net/rfc/named_params, read 2026-11-05: "Date: 2013-09-06,
  significantly updated 2020-05-05", "Author: Nikita Popov", "Status:
  Implemented", "Target Version: PHP 8.0"; it "makes the arguments
  order-independent" and "makes the meaning of the argument self-documenting".

So the page's two facts hold as quotations and do not carry its reading: the
"lightweight wrappers" sentence describes two extensions over **either of two**
C client libraries, one of which (mysqlnd) the same page calls part of PHP's
distribution and recommends, beside PDO, which the manual calls an
abstraction layer; and the RFC blames names that predate PHP's coding standard
while taking the C (IEEE 1003.1) names and parameter order as the standard to
align to. The milestone page's hypothesis, that following each C library's
conventions caused the inconsistency (CL-018), **is contradicted by the RFC it
cites**. PHP is a precedent for one language carrying both shapes over one C
library (mysqli and PDO), not for thin.

**Two contrasting shapes over one C library.**

- *verified*: Go, https://pkg.go.dev/database/sql, read 2026-11-05: "Package
  sql provides a generic interface around SQL (or SQL-like) databases." "The
  sql package must be used in conjunction with a database driver."; and
  https://pkg.go.dev/database/sql/driver, read 2026-11-05: "Package driver
  defines interfaces to be implemented by database drivers as used by package
  sql." "Most code should use the database/sql package."
- *verified*: Python, https://docs.python.org/3/library/sqlite3.html, read
  2026-11-05: "SQLite is a C library that provides a lightweight disk-based
  database"; "The `sqlite3` module was written by Gerhard Häring. It provides
  an SQL interface compliant with the DB-API 2.0 specification described by
  PEP 249, and requires the third-party SQLite library." PEP 249,
  https://peps.python.org/pep-0249/, read 2026-11-05, Marc-André Lemburg,
  Final, created 12-Apr-1999: "This API has been defined to encourage
  similarity between the Python modules that are used to access databases."
- *verified*: Odin's split (above): `vendor:` "Bindings to popular third-party
  libraries", curated by the Odin team; `core:` would be "available in any
  Odin implementation".

**Which language states a rule for how thick a wrapper over C is.**

- *verified*: Rust's Cargo, the clearest,
  https://doc.rust-lang.org/cargo/reference/build-scripts.html (§ `*-sys`
  Packages), read 2026-11-05: "The library crate should provide
  **declarations** for types and functions in `libfoo`, but **not**
  higher-level abstractions." and "It is common to have a companion package
  without the `-sys` suffix that provides a safe, high-level abstractions on
  top of the sys package. For example, the `git2` crate provides a high-level
  interface to the `libgit2-sys` crate."
- *verified*: Nim's classification (above): "A wrapper is an impure library
  that is a very low-level interface to a C library."
- PEP 249 and Go's `database/sql` state a rule for the **face** (one API over
  many C libraries), not for the raw layer.

**Verdict (D)**: the precedent favours **two layers, the raw one thin by rule
and the package's face an API over it, moderately to strongly**: Cargo states
it, Go, Python and PHP's PDO each put a generic face over a C library and a
driver beneath. Panel 033 R5 already makes Heroes' raw layer a module of its
own; precedent says that module is the floor, which is what
`conventions-draft.md` §1.4 says. PHP, read whole, is not a precedent for
thin.
**Prediction (D)**: in experiment Q the thick arm's `right@1` is at least the
thin arm's on both models, because every precedent above that put a database
behind a face chose a generic one; if the critic's F1 holds (text parsing in
both arms), the two arms tie at a floor rather than the thin one winning.
Checked when experiment Q's results are read.
**Condition (D)**: a PHP source (the RFC's discussion, an internals thread)
attributing the inconsistency to following C libraries' conventions would
restore the page's reading; a language whose thin bindings were its library's
face and recorded that as a success would weaken the verdict.

## Item F: the platform layer

How each separates the platform's code from its API; whether that layer is a
separate library or part of the one runtime; what it says it cost.

**GLib (under GTK; GIO, GThread)**

- *verified*: separate libraries, https://docs.gtk.org/glib/, read
  2026-11-05: "GLib is a general-purpose, portable utility library, which
  provides many useful data types, macros,"; beside it GModule "Portable API
  for dynamically loading modules", GObject "The base type system library",
  GIO "GObject Interfaces and Objects, Networking, IPC, and I/O".
- *verified*: per-platform source files under one API,
  https://raw.githubusercontent.com/GNOME/glib/main/glib/gthread-posix.c, read
  2026-11-05, header "gthread.c: posix thread system implementation"; and
  https://raw.githubusercontent.com/GNOME/glib/main/glib/gthread-win32.c, read
  2026-11-05, whose code includes `<windows.h>` and whose header still reads
  "gthread.c: solaris thread system implementation".
- *verified*: platform-only API in a header family a program opts into,
  https://docs.gtk.org/gio-unix/, read 2026-11-05: "Unix-specific APIs in
  Gio", pkg-config `gio-unix-2.0`; https://docs.gtk.org/gio-win32/, read
  2026-11-05: "Windows-specific APIs in Gio", pkg-config `gio-windows-2.0`.
- *verified*: the cost, a separately linked platform piece folded back in,
  https://gitlab.gnome.org/GNOME/glib/-/raw/glib-2-32/README.in (*Notes about
  GLib 2.32*), read 2026-11-05: "It is no longer necessary to use
  g_thread_init() or to link against libgthread." "libglib is now always
  thread-enabled." "Custom thread system implementations are no longer
  supported (including errorcheck mutexes)." "gthread has now been removed as
  a gobject dependency, which will cause such programs to break."

**Qt (QPA, and QtCore's per-platform code)**

- *verified*: https://doc.qt.io/qt-6/qpa.html, read 2026-11-05: "The Qt
  Platform Abstraction (QPA) is the main platform abstraction layer in Qt."
  "The API can be identified by the `QPlatform*` class prefix"; "Concrete
  implementations of the interfaces listed above live in QPA plugins, or
  platform plugins,"; "Qt will select a default QPA plugin, depending on the
  platform." "The `QT_QPA_PLATFORM` environment variable and the `-platform`
  command line argument allow you to override this default." (`qcocoa`,
  `qwindows`, `qxcb`, `qwayland` and nine more).
- *verified*: the cost, https://doc.qt.io/qt-5.15/qpa.html, read 2026-11-05:
  QPA "replaces Qt for Embedded Linux and the platform ports from Qt 4."
  "There are no source or binary compatibility guarantees for the QPA classes"
  "meaning that a platform plugin is only guaranteed to work with the Qt
  version it was developed against."
- *verified*: QtCore's per-platform files, by name,
  https://github.com/qt/qtbase/tree/dev/src/corelib/io, read 2026-11-05:
  `qfilesystemengine_unix.cpp`, `qfilesystemengine_win.cpp`,
  `qfilesystemengine_mac.mm`, `qprocess_unix.cpp`, `qprocess_win.cpp`,
  `qprocess_darwin.mm`, `qlockfile_unix.cpp`, `qlockfile_win.cpp` among the
  listing (how the build picks one is not on the page read).

**libuv (under Node)**

- *verified*: https://docs.libuv.org/en/v1.x/design.html, read 2026-11-05:
  "libuv is cross-platform support library which was originally written for
  Node.js." "While the polling mechanism is different, libuv makes the
  execution model consistent across Unix systems and Windows."
- *verified*: one header family and per-platform directories,
  https://github.com/libuv/libuv/tree/v1.x/include/uv (`unix.h`, `win.h`,
  `darwin.h`, `linux.h`, `posix.h` and nine more) and
  https://github.com/libuv/libuv/tree/v1.x/src (`unix/`, `win/`, `rtos/`
  beside shared files `uv-common.c`, `threadpool.c`, `fs-common.c`), both read
  2026-11-05.
- *verified*: why it exists, Ryan Dahl,
  https://nodejs.org/en/blog/uncategorized/libuv-status-report, read
  2026-11-05 (the page shows "Last Updated Sep 23, 2011"): "`libuv`'s purpose
  is to abstract platform-dependent code in Node into one place where it can
  be tested for correctness and performance before bindings to V8 are added."
  and, of its first Unix half, "The entire Unix backend sits heavily on Marc
  Lehmann's beautiful libraries libev and libeio."
- *unverified*: that libev was later removed from libuv (only third-party
  pages found; the libuv issue a search result named, #485, is about a
  Windows pipe bug).

**The Apache Portable Runtime**

- *verified*: https://apr.apache.org/, read 2026-11-05: "The mission of the
  Apache Portable Runtime (APR) project is to create and maintain software
  libraries" "that provide a predictable and consistent interface to
  underlying platform-specific implementations." "relieving them" "of the need
  to code special-case conditions to work around or take advantage of
  platform-specific deficiencies or features." and "After several years of
  development, APR is beginning to see use outside of the Apache HTTP Server."
- *verified*: per-platform directories per subsystem,
  https://github.com/apache/apr/tree/trunk/file_io, read 2026-11-05: `os2`,
  `unix`, `win32`. No cost statement found.

**NSPR (Mozilla's)**

- *verified*: https://searchfox.org/firefox-main/source/docs/nspr/index.md,
  read 2026-11-05: "Netscape Portable Runtime (NSPR) provides a
  platform-neutral API for system level and libc-like functions." Its status,
  the one cost stated: "NSPR is a legacy library. We do not recommend using it
  in new projects." "It is maintained primarily for existing Mozilla code that
  still depends on it; modern platform and standard-library facilities should
  be preferred instead."

**SDL**

- *verified*: backends as source directories,
  https://github.com/libsdl-org/SDL/tree/main/src/video, read 2026-11-05: 29
  entries, `cocoa`, `windows`, `x11`, `wayland`, `kmsdrm`, `emscripten` and
  others; chosen at initialisation,
  https://wiki.libsdl.org/SDL3/SDL_HINT_VIDEO_DRIVER, read 2026-11-05: the hint
  "specifies a video backend to use", "accepts a comma-separated list of driver
  names", tried "in the order listed during init, until one succeeds or all of
  them fail", environment name `SDL_VIDEO_DRIVER`, "available since SDL
  3.2.0". One library. No cost statement found.

**Go**

- *verified*: files chosen at build time by name,
  https://pkg.go.dev/go/build, read 2026-11-05: "Build constraints may also be
  part of a file's name (for example, source_windows.go will only be included
  if the target operating system is windows)."; and by a line,
  https://pkg.go.dev/cmd/go, read 2026-11-05: "A build constraint, also known
  as a build tag, is a condition under which a file should be included in the
  package."
- *verified*: the cost, the freeze of `syscall`, Rob Pike, 2014,
  https://go.googlesource.com/proposal/+/refs/heads/master/design/freeze-syscall.md
  (from `go.dev/s/go1.4-syscall`), read 2026-11-05: its problems are bloat,
  testing ("cross-platform testing is impossible"), curation ("essentially
  anything goes"), documentation ("godoc only shows the variant for its own
  native environment") and compatibility; the proposal freezes "the package as
  of Go 1.3", and "A new subrepository, `go.sys`, will be created". The
  package today, https://pkg.go.dev/syscall, read 2026-11-05: "That package
  has more system call support than this one, and most new code should prefer
  that package where possible."
- *verified*: part of the one runtime, https://pkg.go.dev/internal/poll, read
  2026-11-05: "This is used by the net and os packages." "It uses a poller
  built into the runtime, with support from the runtime scheduler."

**Zig**

- *verified*: https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/posix.zig,
  read 2026-11-05: "POSIX API layer." "This is more cross platform than using
  OS-specific APIs, however, it is" "lower-level and less portable than other
  namespaces such as `std.Io` and" "`std.process`." "These APIs are generally
  lowered to libc function calls if and only if libc" "is linked."; and
  https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/os.zig, read
  2026-11-05: `std.os` now declares the namespaces `linux`, `plan9`, `uefi`,
  `wasi`, `emscripten`, `windows`.
- *verified*: the cost, a rename,
  https://ziglang.org/download/0.12.0/release-notes.html (§ *std.os renamed to
  std.posix*), read 2026-11-05: "Generally, one should prefer to use the
  higher-level cross-platform abstractions" "rather than reaching into the
  POSIX API layer." "For example, `std.process.exit` is more portable than
  `std.posix.exit`."

**Odin**

- *verified*: https://pkg.odin-lang.org/core/, read 2026-11-05: `os`, "A
  cross-platform API for things like `I/O`, intended to be uniform across all
  operating systems."; `sys`, "Platform specific packages", fifteen, among
  them `posix`, "Raw bindings for most `POSIX` APIs."; and files chosen by
  name (overview, above): "`foobar_windows.odin` would only be compiled on
  Windows, `foobar_linux.odin` only on Linux,".
- *verified*: the cost, the redesign design.md §1.11 cites,
  https://odin-lang.org/news/moving-towards-a-new-core-os/, read 2026-11-05,
  dated 2025-10-31: "`core:os`'s general API was poorly designed and
  inconsistent across platforms." "Constants and errors messages were all
  platform-specific, leading to less than ideal cross-platform behaviour."
  "When the transition happens, this will become `core:os`, and `core:os/os2`
  will be no more." "We are expecting to transition to the new design of
  `core:os` early next year (Q1 2026)." Its arrival,
  https://forum.odin-lang.org/t/2026-02-release-2026-q1-newsletter/1627, read
  2026-11-05, gingerBill on 2026-03-02: "This is intended as the merge happened
  AFTER the dev-2026-02 release." "The next release will have the new
  `core:os`." The redesign page does not say it was possible *because* `core:os`
  was a package; that is design.md's reading (`docs/design.md:533-534`),
  consistent with the page (the new design was built as `core:os/os2` and
  swapped in) but not stated by it.

**Rust**

- *verified*: https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sys/pal/mod.rs,
  read 2026-11-05: "The PAL (platform abstraction layer) contains
  platform-specific abstractions" "for implementing the features in the other
  submodules, such as e.g. bindings."
- *verified*: the cost, a reorganisation still open,
  https://github.com/rust-lang/rust/issues/117276, read 2026-11-05, opened by
  joboet on 2023-10-27, *Tracking Issue for restructuring `std::sys`*: "the
  platform-specific modules inside `std`, namely `sys` and `sys_common`"
  "These modules will be merged into one `sys` module with a feature-first
  organization:"; and today's `sys/mod.rs`,
  https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/sys/mod.rs,
  read 2026-11-05, still carries "FIXME(117276): remove this, move feature
  implementations into individual" "submodules."

**Read across.**

| system | separation | separate library or the one runtime | cost the page states |
|---|---|---|---|
| GLib | per-platform files under one API; platform-only API in its own header family and pkg-config module | separate libraries (GLib, GIO, GObject, GModule) | libgthread folded into libglib (2.32), programs broke |
| Qt | QtCore: per-platform files by name; GUI: QPA plugins, a default per platform, overridable at launch | part of Qt's libraries, plus plugins | QPA classes carry no compatibility guarantee |
| libuv | one header family, per-platform directories | separate library under Node | none stated; built first on libev and libeio |
| APR | per-platform directories per subsystem | separate library | none stated |
| NSPR | one platform-neutral API | separate library | now legacy |
| SDL | per-platform backend directories, chosen at initialisation | one library | none stated |
| Go | files by name or `//go:build` at build time | the one standard library and runtime | `syscall` frozen in 2014 |
| Zig | `std.posix` layer, `std.os.<os>` namespaces | the one standard library | `std.os` renamed `std.posix` (0.12) |
| Odin | `core:os` over `core:sys/<os>`, files by name | the one `core` collection | `core:os` redesigned (2025-26) |
| Rust | `std::sys::pal` | the one standard library | `std::sys` restructured, open since 2023 |

**Which is the author's shape**, *a C library pulled in beside the runtime,
with the `#ifdef`s underneath*: **libuv, APR, NSPR, and GLib with GIO** are that
shape, separate C libraries with one API and the platform's code in files
beneath it. **Not that shape**: Go, Zig, Odin and Rust (the platform layer
inside the one standard library), QtCore (inside one library), and Qt's QPA and
SDL (one backend chosen at launch or at initialisation among several). Two
things the shape's own precedents say that bear on P2:

1. Those four serve **programs in many languages and products** (Node over V8,
   httpd, Mozilla's clients, GTK); libuv's stated reason is "one place where it
   can be tested for correctness and performance before bindings to V8 are
   added". No language in the survey links its platform layer as a second
   library on demand.
2. The one record found of a **platform piece linked on demand**, GLib's
   libgthread, ends with it folded into the main library in 2.32 ("libglib is
   now always thread-enabled"), at the cost of programs that broke.

And one cost that binds every two-arm route whatever the object: Go's freeze
document names "cross-platform testing is impossible" and "godoc only shows the
variant for its own native environment", which is panel 114 R3's instrument
(2), the compiler reporting the `extern` declarations it did not verify on this
machine.

**Verdict (F)**: as a **source layout**, the precedent favours per-platform
files behind one API and one header family, **strongly** (libuv's `uv/unix.h`
and `uv/win.h`, GLib's `gio-unix-2.0` and `gio-windows-2.0`, QtCore's
`_unix`/`_win` files, Go's and Odin's file suffixes). On the **object**, it
favours keeping the platform layer inside the one runtime (P1 or P1'),
**moderately**: every language surveyed does, and the one separately linked
platform piece found was folded back. The author's shape (P2) is real and
long-lived, but every instance found is a library serving many programs, a
different job from a language's runtime.
**Prediction (F)**: whichever route lands, the first platform defect filed
against a package of this milestone that binds a type whose width differs by
platform (`net`, `time`) is found by a CI leg after a push and not by a build
on this Mac, Go's "cross-platform testing is impossible" arriving. Checked at
the milestone's close, over the defects filed against `net` and `time`.
**Condition (F)**: a language whose runtime links its platform layer as a
separate C library only where a program names it, and has kept it so for
several releases without folding it back, would move the object half of the
verdict toward P2.

## Item J: `alias`

My reading agrees with design.md Part 7 item 5's correction:
https://go.dev/doc/go1.9, read 2026-11-05: "Go now supports type aliases to
support gradual code repair while moving a type between packages." One
addition from the design document it links,
https://go.googlesource.com/proposal/+/master/design/18130-type-alias.md
(Russ Cox and Robert Griesemer, "Last updated: December 16, 2016"), read
2026-11-05, which bears on `conventions-draft.md` §1.3 and is not in design.md's
paragraph: "Type aliases may also be useful for allowing large packages to be
split into multiple implementation packages" "with a single top-level exported
API,". The same page calls the earlier proposal "originally targeted for, but
held back from, Go 1.8"; it gives no date for that, so design.md's 2016-11-04
is not re-verified here.

## Item M: sorting by a caller's order

- *verified*: Go, `sort.Slice`, https://pkg.go.dev/sort, read 2026-11-05:
  `func Slice(x any, less func(i, j int) bool)`, "Slice sorts the slice x
  given the provided less function.", and "Note: in many situations, the newer
  slices.SortFunc function is more ergonomic and runs faster." It arrived in
  Go 1.8, https://go.dev/doc/go1.8, read 2026-11-05: "The sort package now
  includes a convenience function Slice to sort a slice given a _less_
  function."
- *verified*: Go, `slices.SortFunc`, https://pkg.go.dev/slices, read
  2026-11-05: `func SortFunc[S ~[]E, E any](x S, cmp func(a, b E) int)`,
  "cmp(a, b) should return a negative number when a < b, a positive number when
  a > b and zero when a == b"; the package arrived in Go 1.21,
  https://go.dev/doc/go1.21, read 2026-11-05: "The new slices package provides
  many common operations on slices, using generic functions that work with
  slices of any element type."
- *verified*: Odin, https://pkg.odin-lang.org/core/slice/, read 2026-11-05:
  `sort_by :: proc(data: $T/[]$E, less: proc(i, j: $E) -> bool)`, "sort_by
  sorts a slice with a given procedure to test whether two values are ordered
  "i < j"", "This sort is not guaranteed to be stable"; beside it `sort_by_cmp`
  (an `Ordering`) and `stable_sort_by`. A package, `core:slice`.
- *verified*: Zig, `std.mem.sort` (URL in item C's table): "Sorts a slice
  in-place using a stable algorithm (maintains relative order of equal
  elements).", `pub fn sort(comptime T: type, items: []T, context: anytype,
  comptime lessThanFn: fn (@TypeOf(context), lhs: T, rhs: T) bool) void`; and
  `sortUnstable` with the same parameters. In the standard library.
- *verified*: Rust, https://doc.rust-lang.org/std/primitive.slice.html, read
  2026-11-05: `pub fn sort_by<F>(&mut self, compare: F) where F: FnMut(&T,
  &T) -> Ordering`, "This sort is stable", listed under `impl<T> [T]` with its
  source in `src/alloc/slice.rs`, and "medium sized slices allocate
  `self.len()`"; `sort_unstable_by`, the same comparator, "in-place (i.e.,
  does not allocate)". A method of the built-in slice type, in the standard
  library's allocating half for the stable sort.
- *verified*: Goose, stdlib.md (URL in item C's table): "sort<T>(xs: T[:])
  // by <; sort(xs) { a, b => ... } by the block", "A comparator is always "a
  goes before b"."

**Verdict (M)**: no language surveyed makes a caller's order a language form;
every one puts it in a library (Go, Odin and Zig in a package of the library,
Rust as a method of the built-in slice type defined in its standard library).
The comparator splits: a two-place `bool` "goes before" (Go's `sort.Slice`,
Odin's `sort_by`, Zig, Goose) against a three-way answer (Go's
`slices.SortFunc`, Rust, Odin's `sort_by_cmp`), and Go's older function now
points to its newer one. The precedent favours **a library function and not a
built-in, strongly**; it does not decide between panel 097's condition 5 and
panel 137, which is a question about Heroes' own two texts.
**Prediction (M)**: the four hand-written sorts issue 1154 names in
`selfhost/` each order by a `bool` less-than or a key, and none by a three-way
answer, so a `bool` comparator replaces all four. Checked by reading the four
sites at the step that lands item M.
**Condition (M)**: a language that moved sorting by a caller's order from its
library into the language would weaken the verdict.

## Item N: measuring a library's effect on a model

- *verified*: documentation in the prompt, measured. Zhou et al.,
  *DocPrompting: Generating Code by Retrieving the Docs*,
  https://arxiv.org/abs/2207.05987, read 2026-11-05, ICLR 2023:
  "existing models inherently cannot generalize to using unseen functions and
  libraries"; "DocPrompting consistently improves NL-to-code models."; on
  CoNaLa it improves CodeT5 by 2.85% in pass@1 (52% relative) "and 4.39% in
  pass@10 (30% relative gain) in execution-based evaluation on the popular
  Python CoNaLa benchmark."
- *verified*: how often a model invents an API, by its frequency. Jain et al.,
  *On Mitigating Code LLM Hallucinations with API Documentation*,
  https://arxiv.org/abs/2407.09726, read 2026-11-05, submitted 2024-07-13:
  "In this study, we address the issue of API hallucinations in various
  software engineering contexts." (its findings, that rarely used APIs fare
  worst and documentation helps them while noisy retrieval hurts frequent
  ones, are the fetch's summary of the abstract, not quoted words).
- *verified*: names, measured on analysis rather than generation. Wang et al.,
  *How Does Naming Affect LLMs on Code Analysis Tasks?*,
  https://arxiv.org/abs/2307.12488, read 2026-11-05: "we create a set of
  datasets with code containing nonsense or misleading names for variables,
  methods, and functions"; "naming has a significant impact on the
  performance of code analysis tasks based on LLMs".
- *verified*: borrowing from a better-known language. Gu et al., *Challenges
  and Paths Towards AI for Software Engineering*,
  https://arxiv.org/abs/2503.22625 (submitted 2025-03-28), § 3.7 at
  https://arxiv.org/html/2503.22625, read 2026-11-05: "Models have been shown
  to hallucinate constructs from higher resource languages when working in
  low-resource languages.", citing Blinn et al. (2024): "contemporary LLMs fail
  to follow Hazel's syntax and semantics, often borrowing syntactic forms"
  (from OCaml and Elm, the page says).
- *verified*: library evolution. Wang et al., *LLMs Meet Library Evolution:
  Evaluating Deprecated API Usage in LLM-based Code Completion*,
  https://arxiv.org/abs/2406.09834, read 2026-11-05, v3 of 2025-02-13; the
  abstract as fetched gives no rate (*unverified*: the 25 to 38% a search
  summary reported).
- *verified*: invented package names. Spracklen et al., *We Have a Package for
  You!*, https://arxiv.org/abs/2406.10279, read 2026-11-05: "the average
  percentage of hallucinated packages is at least 5.2% for commercial models
  and 21.7% for open-source models".

**What I did not find**: any language, or any study, that measured how **a
language's own standard library**, its names or its documentation, changed the
code a model writes in that language, before and after, which is what the
thesis harness's two runs would measure. The nearest measure documentation in
the prompt (DocPrompting), a name's frequency (Jain et al.), names in code
being analysed (Wang et al. 2023), and borrowing from a better-known language
(Gu et al., Blinn et al.).

**Verdict (N)**: no precedent for the measurement; the studies found predict
its direction for one variable each: documentation in context helps with
functions the model has not seen, and a model writing a less-known language
borrows from a better-known one, which for Heroes is Go's names and contracts.
**Prediction (N)**: in the thesis harness's run after the packages, at least
one task's first answer writes a Go name or Go's contract against a Heroes
package (`strings.Index`, or `strings.index(...) < 0` on a `T?`), refused by
the compiler as an unknown name or `bad_operand`. Checked when the after-run is
read.
**Condition (N)**: a study measuring a language's own library before and after
on generated code would replace this verdict with its numbers.

## Condition (the seat's, across items)

What, if found, would change my reading:

1. a language whose **runtime** links its platform layer as a separate C
   library only where a program names it, kept so for years without folding it
   back (item F's object half);
2. a recorded defect class, in any tracker, caused by **a familiar library name
   kept with a changed contract** (item C);
3. a root-relative language that reached a library tree **outside** the
   program's root without per-package roots or a manifest (item B);
4. a PHP source attributing its name and order inconsistency to **following C
   libraries' conventions** (item D).

## Search: what was searched, where, and what was not found

Every search was a web search or a fetch of a named page on 2026-11-05. By item:

- **A**: Go FAQ, Odin FAQ, `pkg.odin-lang.org` (root and `core/`), Odin
  overview, Odin `vendor/README.md`; query *Zig standard library scope "what
  belongs in the standard library" Andrew Kelley issue std lib policy* (found
  #1629 and secondary essays, none a policy page); Hylo: the organisation page,
  `hylo-old` and `hylo-new` READMEs, both `StandardLibrary.swift`, the
  `Sources` and `Core` listings, `docs.hylo-lang.org` (its tour mentions no
  library), query *Hylo language standard library StandardLibrary sources
  github* (no rule found); Hare: `docs.harelang.org`, the 2021 blog post; the
  mandate file itself failed (Debian's copy 404, `git.sr.ht` 502 twice); Nim
  `lib.html`, `apis.html`; Goose `stdlib_design.md`, `stdlib.md`, and the
  file's commit history.
- **B**: Odin's `build_settings.cpp`; query *ODIN_ROOT core collection location
  compiler executable directory* (secondary pages only, then the source); Zig
  README (GitHub, then Codeberg); Go `cmd/go` page (its *Relative import paths*
  section did not come back in two fetches), `go.dev/ref/mod`,
  `go.dev/doc/install/source` (no executable rule there), `cfg.go`; Nim
  `nimc.html`, the manual's raw source at three offsets, the 0.18.0
  announcement (the changelog file 404); Goose's spec § 11.1, `packages.md`
  and its history, commit b59e41d, PR #141; Deno 1.0 and 2.0 posts; the
  import-maps repository and archived explainer; Zig 0.11 notes; Go 1.11
  notes; Python 3.11 and 3.13 *What's New*; Node's modules page; query *Zig
  "import of file outside module path"* (community pages only, not used).
  **Not found**: a root-relative language with neither manifest nor search
  path later forced to add a search path (the search-path half F34 asked for);
  queries above plus *Elm early versions module lookup "--make" ...
  source-directories introduced* (no primary page, Elm marked unverified).
  **Not searched**: B6's symbolic link; when Odin's `core:` prefix arrived.
- **C**: the API pages in item C's table; SE-0357 for Swift; queries *Apple
  developer documentation StringProtocol range(of:)* (then the two Apple
  pages); *zig std.mem rename indexOf to find deprecated pull request 2025*
  and *codeberg ziglang zig "rename all" "index of" functions find std.mem
  commit* (the rename's commit and its stated reason, *one word per concept*,
  were found only on third-party mirrors; Codeberg's PR page answered 500 and
  its commit page served the fetch a page of unrelated text, so the reason is
  **unverified**); *"Remove str.find in 3.0" python-dev Raymond Hettinger
  2005* (four messages read); Go #46336; *Odin core:strings "cut" ... Go
  strings.Cut different meaning issue OR discussion* (nothing found); *Rust
  str::find returns Option instead of -1 rationale RFC* (nothing found).
  **Not found**: any tracker entry recording a reused familiar name with a
  changed contract as a source of mistakes.
- **D**: php.net's *Choosing a library* page (twice) and *Choosing an API*
  page, the PDO introduction (php.net's `intro.pdo` URLs 404 to the fetch;
  read from `php/doc-en`), the RFC and the RFC index (found under *Inactive*
  in its second half), the named-arguments RFC; Go `database/sql` and
  `driver`; Python `sqlite3`, PEP 249; Cargo's build-scripts page; Odin
  `vendor/`. **Not found**: a PHP source naming C conventions as the cause; the
  RFC's discussion section was not read (§ Condition's item 4 is where it
  would bite).
- **F**: GLib docs, `gthread-posix.c`, `gthread-win32.c`, the 2.32 README,
  `gio-unix` and `gio-win32` docs (the GitLab tree page did not render; the
  GitHub mirror's listing was partial); Qt 6 and Qt 5.15 QPA pages (Qt 5's
  plain URL 404), the `qtbase` `io` listing, query *Qt 5 "Lighthouse" project
  QPA* (forum pages only, not used); libuv design page, `include/uv` and `src`
  listings, the 2011 status report, query *libuv history libev removed*
  (third-party only, marked unverified; issue #485 read and unrelated); APR
  home and `file_io`; NSPR via Searchfox; SDL's hint page and `src/video`;
  Go `go/build`, `cmd/go`, the syscall freeze document, `syscall`,
  `internal/poll`; Zig 0.12 notes, `posix.zig`, `os.zig`; Odin `core/` index,
  the 2025 post, the 2026 newsletter thread; Rust `sys/mod.rs`,
  `sys/pal/mod.rs`, #117276. Goose's sidecar and PR #141 (item B) bear on F
  too.
- **J**: Go 1.9 and 1.8 notes, the type-alias design document.
- **M**: Go `sort`, `slices`, 1.8 and 1.21 notes; Odin `core:slice`; Zig
  `mem.zig`; Rust's slice page at four offsets and `core`'s at one; Goose
  `stdlib.md`.
- **N**: queries *LLM code generation effect of API documentation or library
  naming on generated code study "API hallucination"*, *"How and Why LLMs Use
  Deprecated APIs"*, *"How Does Naming Affect" LLMs code*, *LLM code
  generation low-resource programming language hallucinate functions from
  other languages standard library documentation*, *"package hallucination"
  LLM study*; the six arXiv pages above. **Not found**: a before-and-after
  measurement of one language's own library on the code a model writes.
