# Panel 205, historian

Copied by the coordinator at 03:05 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 205, historian

I did all research by web search and fetch in this session. I compiled nothing and started no paid run. My notes are in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/205-historian/notes.md`. *Heroes of code* is not cited as evidence anywhere below.

The fetch tool returns at most 125 characters per quotation. How each item is marked:
- **verified**: the quotation came back verbatim.
- **summarised**: the tool paraphrased.
- **search only**: I could not fetch the page and have only a search result.
- **inference**: my own reasoning, not a source.
- **unverified**: I could not confirm it.

I have not repeated panel 204's precedents: C11 7.1.2p4, POSIX 2.2.1.1, feature_test_macros(7), CPython's "Python.h first", cgo's preamble, `@cDefine` and Zig 0.16, bindgen's `-include`, clang modules, Cython's placement rules, Vala and GHC.

## verdict

**approve (advisory)** of one combination, and **object** to three things.

I approve:
- **Q1**: a macro the program chooses that reaches the unit's command line (the critic's route A, or a group form that compiles to a `-D`). It must be scoped per module or per program and carried in the cache key. Route B, a prefix that reads no libc header, also has a precedent.
- **Q2**: route D (judge a header by whose directory it was found in), but only together with route C (raise again, after the groups' headers, every warning a check rests on).

I object to:
- route E (`-Wsystem-headers`);
- a diagnostic pragma wrapped around each `#include` without route C;
- keeping panel 198's premise and the note at `header_refused.hero:290` as written. The GCC documentation and clang's own source contradict both.

## precedents

### Q1: where a macro that must come before every header is put

**A header of the project's own, first in every unit**
- **gnulib**: "Furthermore <config.h> must be the first include in every compilation unit." The reason it gives: "On many systems, <config.h> is used to set system dependent flags (such as `_GNU_SOURCE` on GNU systems), and these flags have no effect after any system header file has been included." ([manual](https://www.gnu.org/software/gnulib/manual/html_node/Include-_003cconfig_002eh_003e.html), verified)
- **Autoconf 2.72**: `AC_USE_SYSTEM_EXTENSIONS` defines `_GNU_SOURCE` unconditionally ("Enable extensions on GNU systems"). The manual says "This should be called before any macros that run the C compiler", and that the macro "was introduced in Autoconf 2.60" ([manual](https://www.gnu.org/software/autoconf/manual/autoconf-2.72/html_node/C-and-Posix-Variants.html), verified; the list placement is summarised).
- **What happened**: this is the route with the longest record. It works because the project owns line one of every unit. The date the `config.h`-first convention began is unverified.
- **CPython 3.15** explains its own first-header rule: "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you *must* include `Python.h` before any standard headers are included." ([C API intro](https://docs.python.org/3/c-api/intro.html), verified)
  - Its `configure.ac` on main defines `_POSIX_C_SOURCE` as `202405L` and `_XOPEN_SOURCE` as `800` ([configure.ac](https://raw.githubusercontent.com/python/cpython/main/configure.ac), verified).
  - **Inference**: a host whose header goes first must set the switches itself. Heroes' prefix sets none for programs.

**Binding tools: a macro reaches the command line**
- **cgo**: `// #cgo CFLAGS: -DPNG_DEBUG=1`, and "All the cgo CPPFLAGS and CFLAGS directives in a package are concatenated and used to compile C files in that package" ([cmd/cgo](https://pkg.go.dev/cmd/cgo), verified).
  - **Its own headers come first in one file only.** In [out.go](https://raw.githubusercontent.com/golang/go/master/src/cmd/cgo/out.go) on master, each `.cgo2.c` file gets `builtinProlog` (only `#include <stddef.h>`), then the preamble, then cgo's prolog (verified).
  - `_cgo_export.c` instead opens with `#include <stdlib.h>` and then `#include "_cgo_export.h"`, which is where the preamble goes (verified).
  - **What happened**: [go#35315](https://github.com/golang/go/issues/35315), "cmd/cgo: inject preamble before other include directives", was opened by philtay on 2019-11-02. It says: "It precludes the possibility to use some "include first" headers without errors or warnings, such as Python."
  - Ian Lance Taylor replied on 2019-11-20: "A `#cgo` directive seems like kind of a last resort."
  - It was moved to Unplanned on 2020-12-07 ("currently we don't have a plan to address this") and is still open (verified, the replies summarised).
- **Zig**: on master, `TranslateC.defineCMacro` hands `"-D", name=value` to the translate step's `cc_argv` ([Codeberg master](https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/Build/Step/TranslateC.zig), read 2026-10-10, verified).
  - So the macro moved out of the `@cImport` buffer text (panel 204) and onto the command line.
  - Whether `defineCMacro` ships in the 0.16.0 release itself is unverified.
- **bindgen 0.73.2**: `clang_arg` is "Add an argument to be passed straight through to Clang." ([docs.rs](https://docs.rs/bindgen/latest/bindgen/struct.Builder.html), verified)
- **Nim chose for every program.**
  - `nimbase.h` on devel has `#  define _GNU_SOURCE 1` under `#if defined(__GNUC__) && !defined(__ZEPHYR__)`, before its first `#include <limits.h>` ([nimbase.h](https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/nimbase.h), verified).
  - For the program's own choice, `passc` passes flags to the C compiler, and `localPassC` does so "but only for the C/C++ file that is produced from the Nim module the pragma resides in" ([manual](https://nim-lang.org/docs/manual.html), verified). That is a precedent for a per-module `-D`.
  - **Inference**: this is the choice panel 076 refused for Heroes programs, and Heroes' runtime makes it for its own unit only.
- **Cython**: a `#distutils:` comment in the source sets `define_macros` ([tests/run/define_macro.pyx](https://raw.githubusercontent.com/cython/cython/master/tests/run/define_macro.pyx), summarised). I did not find it in Cython's documentation (unverified there).
- **D ImportC** is the nearest precedent for route B.
  - "The druntime file src/importc.h will automatically be #included first." Preprocessor flags go through `-P` ([ImportC](https://dlang.org/spec/importc.html), verified).
  - [importc.h](https://raw.githubusercontent.com/dlang/dmd/master/druntime/src/importc.h) on master includes no libc header except `"sys/cdefs.h"` under `__FreeBSD__`. It defines its own `__uint16_t`-style typedefs (verified).
  - **Inference, unrun**: on FreeBSD that one include would latch the POSIX visibility switches.
- **Build systems**
  - CMake `add_compile_definitions` was "Added in version 3.12" and fills `COMPILE_DEFINITIONS` ([CMake](https://cmake.org/cmake/help/latest/command/add_compile_definitions.html), verified).
  - Bazel `defines` go to "this and all dependent targets". `local_defines` are "only added to the compile command line for this target" ([Bazel](https://bazel.build/reference/be/c-cpp), verified).
  - Clang modules `config_macros`: "The compiler is required to maintain different variants of the given module for differing definitions of any of the named macros", and a changed one warns under `-Wconfig-macros` ([Modules](https://clang.llvm.org/docs/Modules.html), verified).
  - **Inference**: the modules rule is precedent for the critic's question whether the cache key carries the macro.
- **A package answering a feature macro, and what happened**: an OpenAFS commit by Andrew Deason, 2026-07-31, says three things ([openafs-cvs](https://lists.openafs.org/pipermail/openafs-cvs/2026-August/038906.html), verified):
  - "'pkg-config --cflags' for fuse2 includes -D_FILE_OFFSET_BITS=64";
  - "fuse3 has a static assert to make sure that off_t is 64 bits";
  - its fix: "just move our define for _FILE_OFFSET_BITS earlier, before including anything".

  This is defect 568's class, repaired this summer. A libfuse commit, "don't force -D_FILE_OFFSET_BITS=64 in pkgconfig file", reportedly gave mismatched `off_t` across compilation units as its reason (search only; date unverified). That bears on the critic's per-module question.

### Q2: warnings inside a library's own header code

**Judging by where a header was found is how the compilers are designed**
- GCC: "All warnings, other than those generated by '#warning' …, are suppressed while GCC is processing a system header." Also: "Macros defined in a system header are immune to a few warnings wherever they are expanded." ([cpp](https://gcc.gnu.org/onlinedocs/cpp/System-Headers.html), verified)
- GCC: "If a standard system include directory, or a directory specified with -isystem, is also specified with -I," "the -I option is ignored." ([Directory Options](https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html), verified)
- Clang's source mirrors this: "ignore the user's request and drop the user dir... keeping the system dir. This is weird, but required to emulate GCC's search path correctly." ([InitHeaderSearch.cpp](https://clang.llvm.org/doxygen/InitHeaderSearch_8cpp_source.html), clang 23.0.0git, verified)
  - So the critic's item 8 is designed behaviour, and panel 198's *as `-I`* cannot reach `/usr/include`.
- `CPATH` is searched "as if specified with -I". `C_INCLUDE_PATH` and its siblings are searched "as if specified with -isystem" ([GCC](https://gcc.gnu.org/onlinedocs/cpp/Environment-Variables.html), verified). The critic's item 9 is documented behaviour.
- Promoting a default directory with `-isystem` has its own cost. GCC PR 70129 (2016-03-07), "[6 Regression] stdlib.h: No such file or directory when using -isystem /usr/include", was closed WONTFIX (search only; Bugzilla refused the fetch).

**Third-party headers: the tools moved toward judging by owner**
- **MSVC** `/external` is "available starting in Visual Studio 2017 version 15.6" and needed `/experimental:external` until "Visual Studio 2019 version 16.10" ([Microsoft Learn](https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics), ms.date 2022-06-29, verified).
  - The page says `/external:Wn` "has an effect similar to wrapping an included header in a `#pragma warning` directive" (push 0, then pop). That is exactly the brief's Q2 route of a pragma around each `#include`.
  - Yuriy Solodkyy, 2017-12-13: "We chose the notion of 'external header' over 'system header' that other compilers use" ([blog](https://devblogs.microsoft.com/cppblog/broken-warnings-theory/), verified; context summarised).
- **CMake**: "By default, SYSTEM is true for imported targets and false for other target types" ([NO_SYSTEM_FROM_IMPORTED](https://cmake.org/cmake/help/latest/prop_tgt/NO_SYSTEM_FROM_IMPORTED.html)). The `SYSTEM` property was "Added in version 3.25" ([SYSTEM](https://cmake.org/cmake/help/latest/prop_tgt/SYSTEM.html)). Both verified; the version that made imported targets SYSTEM is unverified.
- **Meson**: `include_type` (since 0.52.0) defaults to `'preserve'`; `'system'` uses `-isystem` "where possible" ([Meson](https://mesonbuild.com/Reference-manual_functions.html), verified).
- **Bazel** conflicts with itself:
  - rules_cc's `includes` doc now says "will produce `-I path_to_package/include_entry`", for "third-party libraries that do not conform to the Google style" ([attrs.bzl](https://bazel.googlesource.com/rules_cc/+/refs/heads/main/cc/private/rules_impl/attrs.bzl), verified).
  - A rules_python commit of 2025-09-17 says Bazel 9 fills `.includes` rather than `.system_includes`, yet adds "both result in the includes being added as system include paths" ([commit](https://pigweed.googlesource.com/third_party/github/bazelbuild/rules_python/+/0cd9bfafc7ebf4cde2e4f84cab6ae756753f9660), verified).
  - Which flag Bazel 9 actually emits is unverified.

**What silence cost: real bugs hidden**
- Qt list, 2014-05-07, Robert Knight: it "ended up suppressing a real and serious warning that occurred when a generic class in Qt was instantiated with a particular type in my code" (`QSharedPointer` with a forward-declared `T`) ([Qt development](https://lists.qt-project.org/pipermail/development/2014-May/016887.html), verified).
- cmake-developers, 2016-04-07, Tamás Kenéz: "The -Wformat warnings are suppressed if the containing log.h is found on isystem path." Brad King answered "Yes": intended, opt out with `NO_SYSTEM_FROM_IMPORTED` ([question](https://cmake.org/pipermail/cmake-developers/2016-April/028202.html), [reply](https://cmake.org/pipermail/cmake-developers/2016-April/028203.html), verified).
- MSVC, in its own documentation: "Setting a low warning level for external headers can hide some actionable warnings." Its repair is `/external:templates-` (verified, same page).
- **Inference**: in C the analogue is a header's macro or `static inline` expanded in the emitted lines, given GCC's "immune ... wherever they are expanded". Whether any Heroes check rests on a warning inside such an expansion is a question for the compiler-engineer. The critic's item 15 found the type checks survive under `-isystem`.

**What strictness cost: builds refused by a library's own warning**
- Gentoo devmanual, "-Werror compiler flag not removed": "there are numerous cases where this breaks without purpose". It lists "new warnings on version bumps of GCC/glibc", "libraries adding deprecated API warnings although that API is still working/supported", and "on less known architectures we may get different/more warnings than on common ones" ([devmanual](https://devmanual.gentoo.org/ebuild-writing/common-mistakes/index.html), verified).
  - **Inference**: these match defect 569's Homebrew-against-apt split and the critic's `deprecated` row.
- GCC 14: "Certain warnings are now errors" ([porting](https://gcc.gnu.org/gcc-14/porting_to.html), verified). Eiffel PR# 19927, 2024-06-14, a C-emitting compiler: "In GCC 14 warning incompatible-pointer-types was promoted from a warning to an error." Status: Analyzed ([Eiffel](https://support.eiffel.com/report_detail/19927), verified).
- The opposite pole, silencing everything:
  - Nim's `nim.cfg` has `clang.options.always = "-w -ferror-limit=3 -fno-strict-aliasing"` and `-w` for gcc ([nim.cfg](https://raw.githubusercontent.com/nim-lang/Nim/devel/config/nim.cfg), verified).
  - D says: "ImportC does not emit warnings." (verified)

### Defect 570: a header's pragma outranks the command line
- GCC: "Note that these pragmas override any command-line options." and "If a pop has no matching push, the command-line options are restored." ([GCC](https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html), verified). GCC 4.6 added selective control by `#pragma GCC diagnostic` ([changes](https://gcc.gnu.org/gcc-4.6/changes.html), verified).
- Clang: push and pop "save and restore the full diagnostic state of the compiler, regardless of how it was set" ([manual](https://clang.llvm.org/docs/UsersManual.html), verified).
- The history of the hole:
  - cfe-dev, 2009-07-05, Louis Gerbarg: "the current pragmas force permanent changes in the diagnostic configuration for the entire rest of that compilation unit" ([cfe-dev](https://lists.llvm.org/pipermail/cfe-dev/2009-July/005667.html), verified).
  - LLVM bug 20022, 2014-06-12: "we don't warn on push with no pop", and "GCC doesn't warn on anything I tried, not even popping an empty stack." ([llvm-bugs](https://lists.llvm.org/pipermail/llvm-bugs/2014-June/034621.html), verified). Whether it was ever fixed is unverified.
- MSVC: a library's pragma wins, because "The **`/external`** options don't override that deliberate choice." (verified)
- cgo writes its own `#pragma GCC diagnostic ignored` lines after the user's preamble (out.go, verified). That is route C's position, used to relax warnings rather than raise them.
- No tool I examined protects the rest of a unit from a header's bare pragma. That negative claim rests on GCC's, clang's and MSVC's documentation and bug 20022, so it is a question.

## argument

The command line is the one place every tool examined guarantees precedes every header. cgo, Zig, bindgen, Nim, CMake and Bazel route a feature macro through `-D`; Nim scopes it per module, Bazel per target. Autoconf's `config.h`-first works because the project owns line one; D keeps its hidden prefix free of libc headers. cgo's one libc-first file is a ticket open since 2019. For Q2, GCC, clang, MSVC and CMake judge a header by where it was found, so panel 198's *as `-I`* cannot hold for system directories, by design. The recorded price is bugs hidden in macros and templates. A header's pragma outranks the command line everywhere examined: re-raise the warnings after the groups.

## condition

Any one of these would change my reading:
- **A binding tool that let source text precede its own libc-including prefix** for years with no ticket like go#35315. That would favour "a header of the program's own before the prefix" over `-D`.
- **A toolchain that enforces warnings inside third-party or system headers by default** (route E) without Gentoo's list of breakages.
- **A documented case where judging by location hid a type mismatch located in a binding tool's own generated lines.** That would weigh against route D.
- **A compiler diagnostic that refuses a header's bare `ignored` or unbalanced push** (bug 20022 closed). That would make route C less necessary.
- **Bazel 9 shown to have moved third-party `includes` from `-isystem` to `-I` deliberately, with a stated reason.** That would give panel 198's direction a precedent.

Not searched or not found: SwiftPM's `cSettings.define`, Meson's `add_project_arguments`, Vala, Microsoft's `WIN32_LEAN_AND_MEAN` documentation, the 0.16.0 release date. GCC PR 70129 and the libfuse commit are search results only.

## Repository files read

- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-205/docs/panel/205-briefs/00-shared.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-205/docs/panel/205-briefs/historian.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-205/docs/panel/205-reports/completeness-critic-pass1.md
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/204-reports/historian.md
- Written: /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/205-historian/notes.md (notes only)
