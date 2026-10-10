# Panel 204, historian

Copied by the coordinator at 01:46 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 204, historian

I read the briefs and reports from 2026-10-10. I did all my web research in this session and compiled nothing. I wrote no notes file, so this reply is the whole report. *Heroes of code* is not used as evidence anywhere below.

Every quotation below came back from the fetch tool, which returns at most 125 characters at a time. Where I asked for verbatim pieces and got them, I mark the source **verified**. Where the tool paraphrased, I add **(summarised)**. Anything I inferred is marked **inference**, and anything I could not reach is marked **unverified**.

## verdict

**object (advisory).** I object to three things:
- a canonical order by bytes;
- per-group units as the way to remove order dependence;
- leaving today's content-dependent reordering in place without a word in the spec (critic, repair 1).

The precedent supports critic route 5 together with three additions:
- the written group order *is* the include order, record-only groups included;
- one sentence in the spec that says so and also names the compiler's own prefix;
- messages that name an order to write, instead of telling the author to repair a header.

## precedents

**What C itself promises: order freedom for standard headers only**
- **C11 7.1.2p4** ([N1570](https://port70.net/~nsz/c/c11/n1570.html), verified):
  - "Standard headers may be included in any order; each may be included more than once in a given scope, with no effect different from being included only once, except that the effect of including <assert.h> depends on the definition of NDEBUG."
  - The guarantee covers *standard* headers. Nothing in the paragraph extends it to `jpeglib.h`.
  - The "more than once" clause is what makes the emitter's dropping of a repeated header (critic, repair 7) safe for standard headers. Clang's modules documentation names the class it is not safe for: headers that "are often included many times in a single translation unit, and will have no include guards" ([Clang Modules](https://clang.llvm.org/docs/Modules.html), verified).
- **Feature-test macros must come before every header:**
  - POSIX, Issue 8 (IEEE Std 1003.1-2024), 2.2.1.1: "…shall ensure that the feature test macro _POSIX_C_SOURCE is defined before inclusion of any header" ([Open Group](https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html), verified; the tool trimmed the opening words).
  - glibc: "In order to be effective, a feature test macro must be defined before including any header files" ([feature_test_macros(7)](https://man7.org/linux/man-pages/man7/feature_test_macros.7.html), verified).
  - This is the primary source for critic repair 2: as long as a hidden prefix comes first, a switch written by the author can never be first.

**The headers in this sitting's cases, in their owners' own words**
- **libjpeg** ([libjpeg-turbo doc/libjpeg.txt](https://raw.githubusercontent.com/libjpeg-turbo/libjpeg-turbo/main/doc/libjpeg.txt), verified; the text's date is unverified):
  - "Before including jpeglib.h, include system headers that define at least the typedefs FILE and size_t."
  - So `ba.hero` is a library used as documented, written in the wrong order. Today's advice, *repair the header*, contradicts the library's own documentation.
- **Winsock** ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/winsock/include-files-2), verified; ms.date 2018-05-31, updated 2025-03-11):
  - "The declarations in the *Winsock.h* header file will conflict with the declarations in the *Winsock2.h* header file."
  - Windows.h pulls in Winsock.h by default, so the order depends on a header two levels down.
- **CPython** ([C API intro, 3.15](https://docs.python.org/3/c-api/intro.html), verified):
  - "you must include Python.h before any standard headers are included."
  - This is a documented "our header first" rule, the same shape as Heroes' prefix.
  - The cost of breaking it: Fedora bug [2416110](https://bugzilla.redhat.com/show_bug.cgi?id=2416110) (2025-11-20, CLOSED NOTABUG) reports "warning: '_POSIX_C_SOURCE' redefined" from stdio.h coming before Python.h. That is `cfgone/main2`'s `-Wmacro-redefined` class in the wild. Verified (summarised).

**Tools that sorted `#include` lines, and what happened**
- **clang-format** ([style options](https://clang.llvm.org/docs/ClangFormatStyleOptions.html), verified):
  - `SortIncludes` exists since clang-format 3.8; `IncludeBlocks` since clang-format 6.
  - `IBS_Preserve` is "Sort each `#include` block separately."
- **LLVM, 2016:** on 2016-03-11 Chandler Carruth proposed re-sorting all LLVM includes with it. The first reply cited DIASupport.h, "atlbase.h has to come before windows.h", and said "Sorting those would break stuff" ([cfe-dev 047881](https://lists.llvm.org/pipermail/cfe-dev/2016-March/047881.html), [047882](https://lists.llvm.org/pipermail/cfe-dev/2016-March/047882.html), verified).
- **Chromium, 2017:** "Let clang-format sort includes" landed on 2017-02-02 ([codereview 2669263003](https://codereview.chromium.org/2669263003), verified).
  - It records that "shellapi.h only compiles if windows.h was included first."
  - The remedy was an author's fence: "clang-format doesn't reorder across blocks."
- **LLVM bug 41740, 2019-05-04:** "due to IncludeBlocks it always sort includes which breaks some of my code", with `SortIncludes: false` set ([llvm-bugs](https://lists.llvm.org/pipermail/llvm-bugs/2019-May/074363.html), verified). Its resolution is unverified.
- **GCC's `gcc-order-headers`** ([contrib/header-tools/README](https://gnu.googlesource.com/gcc/+/trunk/contrib/header-tools/README), verified; the README carries 2015 dates, its author is unverified):
  - It reorders headers "known to the tool into a canonical order which will resolve any hidden dependencies."
  - "Any unknown headers will simply be placed after the recognized files" and "retain the same relative ordering they had."
  - It refuses to reorder headers inside a conditional block.
  - So GCC's canonical order is a list of known dependencies, never a sort by bytes.
- **Go 1.21** (released 2023-08-08, [release page](https://go.dev/doc/devel/release); [notes](https://go.dev/doc/go1.21), verified):
  - "Sort all packages by import path", then initialise the first package whose imports are done.
  - "This may change the behavior of some programs that rely on a specific initialization ordering that was not expressed by explicit imports."
  - The byte sort there only breaks ties under explicit dependency edges. C headers declare no such edges. That last point is an **inference**.
  - Go's notes are also the precedent for critic question 5: Go changed program behaviour deliberately and wrote it down. Whether the change was gated by module version is unverified.

**Binding tools that keep the author's order**
- **cgo** ([cmd/cgo](https://pkg.go.dev/cmd/cgo), verified):
  - The preamble "is used as a header when compiling the C parts of the package", and it "may contain any C code."
  - A preamble is "copied into two different C output files" ([go#10982](https://github.com/golang/go/issues/10982), 2015-05-29, verified).
  - That each file's preamble forms its own unit is an **inference**.
- **bindgen 0.73.2** ([Builder](https://docs.rs/bindgen/latest/bindgen/struct.Builder.html); [lib.rs](https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen/lib.rs), verified):
  - Every input header but the last becomes `-include`; the last is the main input.
  - GCC: "If multiple -include options are given, the files are included in the order they appear on the command line" ([GCC](https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html), verified).
  - The tutorial's `wrapper.h` "will include all the various headers" ([book](https://rust-lang.github.io/rust-bindgen/tutorial-2.html), verified, summarised).
- **Zig:**
  - 0.13.0: "Top-level declarations are order-independent", while `@cDefine` "appends `#define $name $value` to the `@cImport` temporary buffer" ([docs](https://ziglang.org/documentation/0.13.0/), verified).
  - [0.16.0 notes](https://ziglang.org/download/0.16.0/release-notes.html): `@cImport` is "now deprecated", and its includes move into a C file `c.h` that the author writes (verified, summarised).
  - The release date, April 2026, comes only from a secondary aggregator, so it is unverified.
  - [zig#20630](https://github.com/ziglang/zig/issues/20630), opened by andrewrk on 2024-07-14, was closed as completed on 2026-04-20 (verified, summarised).
- **SwiftPM:** a `shim.h` holding the `#include` lines, named by a `module.modulemap` (Ole Begemann, [2017-12-18](https://oleb.net/blog/2017/12/importing-c-library-into-swift/), a secondary source, verified).
- **Odin:** `foreign import kernel32 "system:kernel32.lib"` names a library, not a header ([overview](https://odin-lang.org/docs/overview/), verified). That Odin therefore has no include order is an **inference**.
- **Nim:**
  - The `header` pragma means "the generated code should contain an #include", and `emit` accepts `/*INCLUDESECTION*/` ([manual](https://nim-lang.org/docs/manual.html), verified).
  - Nim's rule for ordering includes: **unverified**. I searched GitHub issues and the forum for "header pragma include order" and found nothing.

**Isolation (each header parsed on its own), and its documented price**
- **Clang modules** ([Modules](https://clang.llvm.org/docs/Modules.html), verified):
  - The claim: "one software library can not affect how another software library is compiled, eliminating include-order dependencies."
  - The price, in the same document: "the module may fail to build if there are missing includes". Headers that cannot comply are escaped with `textual` headers and `config_macros`.
  - The design was presented by Doug Gregor at the November 2012 LLVM Developers' Meeting ([isocpp](https://isocpp.org/blog/2012/11/modules-update-on-work-in-progress-doug-gregor); search summary only).
  - `jpeglib.h` fails exactly that test, by its own documentation.
- **Headers compiled alone:**
  - Bazel's `parse_headers` "expects headers to be" independently buildable, and BoringSSL disabled it on 2024-08-28 ([commit](https://boringssl.googlesource.com/boringssl/+/296ef284e51a687920a1975a1a34fd2ffce0a646%5E%21), David Benjamin, verified in part).
  - Linux nolibc, 2025-04-24: "Each nolibc header should be valid for inclusion irrespective of any special ordering requirements" ([LKML](https://lkml.rescloud.iu.edu/2504.3/00834.html), verified).
  - Both apply only to a project's *own* headers.
- **A tool that compares a program over several orders:** none found. I searched for permutation checks of include order; the nearest are self-containment checkers. Route (b) and critic route 6 have no precedent that I found, and that is a question rather than a fact.

**C-emitting compilers that chose the order themselves**
- **Cython:**
  - 2011-07-21, Robert Bradshaw: "we make sure to emit the #include statements in the same order as they are encountered in the Cython sources", and "C is sensitive to this kind of thing" ([cython-devel](https://mail.python.org/pipermail/cython-devel/2011-July/001024.html), verified).
  - Today's documentation: a block that "contains only function or variable declarations (and no type declarations of any kind)" gets its `#include` "after all declarations generated by Cython" ([docs](https://cython.readthedocs.io/en/latest/src/userguide/external_C_code.html), verified).
  - That is the mirror image of critic repair 1: the order chosen by what a block holds, but *documented*.
  - The directive `preliminary_late_includes_cy28` is commented "Temporary directive in 0.28 … (see GH#2079)" ([Options.py](https://raw.githubusercontent.com/cython/cython/0.29.x/Cython/Compiler/Options.py), verified).
  - [#2079](https://github.com/cython/cython/issues/2079), opened 2018-01-21, is still open (verified, summarised).
  - Which release made content-based placement the default: **unverified**; CHANGES.rst does not say.
  - That a Cython function-only `stdio.h` block written first could reproduce `jpeg`: **inference, unrun**.
- **Vala:**
  - [Bug 618931](https://bugzilla.gnome.org/show_bug.cgi?id=618931), 2010-05-17: "The problem depends on how Vala chooses to order #include lines in the generated C code."
  - The fix proposed was a `crequire_symbol` attribute, the critic's route 1 (a group naming its prerequisites).
  - It was closed RESOLVED OBSOLETE on 2018-05-22 and moved to [vala#98](https://gitlab.gnome.org/GNOME/vala/-/issues/98) (verified). Whether #98 is still open is unverified.
- **GHC 9.14.1** ([FFI guide](https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/ffi.html), verified):
  - Versions 6.8.3 and earlier included the header in the generated C. "GHC no longer includes external header files when compiling via C."
  - The reason given: comply with an FFI specification that "requires that FFI calls are not subject to macro expansion."
  - GHC removed order by removing the headers themselves, and gave up the C compiler's type check to do it.

**Languages that say declaration order never matters, and what they say about foreign headers**
- **D:**
  - "The ordering of CImportDeclarations has no significance" ([ImportC](https://dlang.org/spec/importc.html), verified).
  - ImportC allows forward references to C declarations.
  - "src/importc.h will automatically be #included first": a hidden prefix that D documents.
- **Zig:** states order independence, and its foreign text is a buffer filled in order (above).
- **Neither** writes an explicit exception sentence for `#include` order. I searched only those two specifications and cgo's documentation, so this negative claim is a question.

## argument

cgo, bindgen and Zig keep the author's written order, and no binding tool I found sorts third-party headers. The sorters that touched such headers broke builds (cfe-dev 2016, Chromium 2017, LLVM 41740). They survived only by letting authors fence the order off. GCC's canonical order is a dependency list, never bytes. Isolation (Clang modules) removes order only for self-contained headers, and libjpeg documents that it is not one. The compilers that chose the order themselves left tickets unresolved: Vala's 2010 report was closed unfixed, and Cython's 2018 ticket is open. The precedent therefore points to written order (record-only groups included), a stated prefix, and messages that name an order.

## condition

Any one of these would change my reading:
- **A binding tool or C-emitting compiler that sorted third-party headers by name** in generated code for years without reports of breakage. That would weaken my objection to the byte sort.
- **A census on the unit Heroes really compiles** (prefix included) finding no installed pair that fails or changes meaning in one order. The carried census counts failures only, and none of its counts is zero.
- **A tool that compares a program's meaning over several include orders** with a public track record. That would give the comparison route (b, critic route 6) a precedent.
- **Evidence that Cython's content-based late placement caused no breakage**, or a stated reason for Heroes' record-only reordering. That would weaken my objection to keeping repair 1's order with a spec sentence instead of removing it.
- **No non-self-contained header in the 107 tracked files**, nor in panel 202's census. Per-group units would then cost little.

## Repository files read

- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-204/docs/panel/204-briefs/00-shared.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-204/docs/panel/204-briefs/historian.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-204/docs/panel/204-reports/completeness-critic-pass1.md
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/202-reports/historian.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-204/issues/2026-09/07/2026-09-07-0000-four-repairs-to-design-md-that-ride-its-opening-sitting-4-19.md
- /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-204/spec/heroes-spec.md (lines 105-119)
