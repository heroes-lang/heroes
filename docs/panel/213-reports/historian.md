# Panel 213, the historian's report

Seat: historian (advisory, no veto). Written 2026-10-13, as the research went.
Every precedent below carries the URL of the page read this session; `verified`
means the page read says it, `unverified` means it does not, or could not be
read. **No build and no debugger was run by this seat**: its tools are Read,
Write, WebSearch and WebFetch, and its questions are about other projects. Every
sentence below about what Heroes would get from a precedent is an inference and
is marked so. *Heroes of code* was not used as a source for anything below.

## verdict

**approve (advisory)**, for a surface built on **C (formatters embedded in the
binary as data, no Python) together with E (the author's names in the emitted
C)**, the editor reached through what lldb loads by itself. **Object (advisory)
to B as the primary surface**: a Python file found by path beside the binary is
the route the precedents below show breaking.

## precedents

### Route C: formatters the debugger loads from the binary by itself

1. **LLDB loads type summaries and formatter bytecode embedded in a binary, on
   every module load, with no Python and no setting.** `Target::ModulesDidLoad`
   (Target.cpp lines 1846 to 1864 as rendered) calls
   `LoadTypeSummariesForModule(module_sp)` and `LoadFormattersForModule(module_sp)`
   for each module loaded:
   https://codebrowser.dev/llvm/lldb/source/Target/Target.cpp.html **verified**
   (at the revision codebrowser renders; the revision is not shown).
   `FormatterSection.cpp` reads `eSectionTypeLLDBTypeSummaries` (records:
   version 1, size, a type name or a `^` regex, a summary string) and
   `eSectionTypeLLDBFormatters` (bytecode), into the `default` category:
   https://codebrowser.dev/llvm/lldb/source/DataFormatters/FormatterSection.cpp.html
   **verified**.
2. **The section names, per object format.** Mach-O: LLDB's own test writes
   `__attribute__((section("__TEXT,__lldbsummaries"), used))` and is
   Darwin-only:
   https://raw.githubusercontent.com/llvm/llvm-project/main/lldb/test/API/functionalities/data-formatter/embedded-summary/main.c
   and `.../TestEmbeddedTypeSummary.py` **verified**. ELF: LLDB's unit test
   builds ELF objects with `.lldbsummaries` and `.lldbformatters` and asserts
   summaries and version-2 synthetic children load from them:
   https://llvm.googlesource.com/llvm-project/lldb/+/refs/heads/master/unittests/DataFormatter/FormatterSectionTest.cpp
   **verified** (at `main`). PE/COFF: `ObjectFilePECOFF::GetSectionType` maps
   `.Case(".lldbsummaries", eSectionTypeLLDBTypeSummaries)`:
   https://codebrowser.dev/llvm/lldb/source/Plugins/ObjectFile/PECOFF/ObjectFilePECOFF.cpp.html
   **verified** for `.lldbsummaries`; for `.lldbformatters` the fetch reported a
   mapping without quoting the line, **unverified**. Mach-O's
   `__lldbformatters`: not on a page read, **unverified** (the critic found the
   string in this Mac's LLDB framework, critic-briefs § 5).
3. **Swift shipped it first, for exactly this reason.** SE-0440
   `@DebugDescription`, *"Implemented (Swift 6.0)"*: the summary string *"is
   emitted into the binary, where LLDB will load it automatically"*; it *"does
   not affect ABI"*; a Type Summary *"does not execute code inside the debugged
   process"*, so it shows *"in situations where code execution is not performed,
   such as the variable list of an IDE"*; it cannot be attached to generic
   types: https://github.com/swiftlang/swift-evolution/blob/main/proposals/0440-debug-description-macro.md
   **verified**.
4. **Formatter bytecode, LLDB's stated successor.** *"an efficient and secure
   encoding for data formatters"*, embedded in `.lldbformatters`, *"an
   evolution of the Swift `.lldbsummaries` section"*, so LLDB can
   *"automatically find them and run them securely"*; *"For security reasons the
   list of functions callable with `call` is predefined"*; methods `@summary`,
   `@get_num_children`, `@get_child_at_index`, `@update` and others; control
   flow `if`, `ifelse`, `return` and no loop listed; selectors include
   `get_child_with_name`, `cast`, `create_child_at_offset`, `read_memory`, and
   **no global symbol lookup**; no producing tool named:
   https://lldb.llvm.org/resources/formatterbytecode.html **verified**.
   Proposed by Adrian Prantl, 2024-10-22 (*"it is potentially dangerous to just
   load an execute untrusted Python code in LLDB"*); David Spickett, 2024-10-28:
   *"I don't see library authors being very receptive to having to write their
   own compiler"*:
   https://discourse.llvm.org/t/a-bytecode-for-lldb-data-formatters/82696
   **verified**. **The LLVM release that first shipped either loader:
   unverified** (the LLVM 20.1.0 release notes do not mention it,
   https://releases.llvm.org/20.1.0/docs/ReleaseNotes.html, read).
5. **Rust names the bytecode as its intended LLDB route** and says LLDB is not
   supported by `#[debugger_visualizer]` today: *"LLDB is not currently
   supported"*; the *"intended method"* avoids *"the safety concerns associated
   with embedding an entire Python script"*:
   https://rustc-dev-guide.rust-lang.org/debuginfo/debugger-visualizers.html
   **verified**.
6. **RFC 3191 `#[debugger_visualizer]`** (Rust): natvis passed to the MSVC
   linker as `/NATVIS` and embedded in the PDB; gdb Python embedded in
   `.debug_gdb_scripts`, which *"has proven to be a more reliable route than
   depending on file paths which can be unstable at times"*:
   https://rust-lang.github.io/rfcs/3191-debugger-visualizer.html **verified**.
7. **GDB's equivalent is gated.** `.debug_gdb_scripts` holds a file name or
   inline Python, and both *"require"* a configured `auto-load safe-path`:
   https://sourceware.org/gdb/current/onlinedocs/gdb.html/dotdebug_005fgdb_005fscripts-section.html
   **verified**; safe-path exists because auto-loaded files may come from
   untrusted sources:
   https://sourceware.org/gdb/current/onlinedocs/gdb.html/Auto_002dloading-safe-path.html
   **verified**.

### Route B: a formatter file beside the binary, or per toolchain (G)

8. **Go wrote a path to its Python helper into the binary, and it broke
   twice.** 2017: the path was found from `runtime/debug.go`, which *"can easily
   get dead-code eliminated entirely"*, so binaries lacked the helpers:
   https://go.googlesource.com/go/+/ba42b3ffd3220516911705fd2d060ea02e5502bb
   **verified**. 2021: the section *"is not always
   "$GOROOT/src/runtime/runtime-gdb.py" as expected"*, a user's `proc.go` could
   win: https://github.com/golang/go/issues/47881 **verified**. Go's own page
   now says *"Delve is a better alternative to GDB"*: https://go.dev/doc/gdb
   **verified**.
9. **LLDB's Python-beside-the-binary route is Apple-only and off by default.**
   2016, Greg Clayton: *"only currently hooked up for Mach-O binaries when you
   have a dSYM"*; Enrico Granata: dSYMs *"only exist on Apple systems"*, off by
   default *"due to security concerns"*:
   https://discourse.llvm.org/t/lldb-support-for-python-scripts-in-elf-section-like-debug-gdb-scripts-in-gdb/42700
   **verified**. Upstream default of `load-script-from-symbol-file` today is
   `eLoadScriptFromSymFileTrusted`:
   https://raw.githubusercontent.com/llvm/llvm-project/main/lldb/source/Target/TargetProperties.td
   **verified** (what `trusted` admits is not defined on that page,
   **unverified**). Qt took it, 2017-07-07, noting *"a slight security risk, as
   other libraries might have scripts of their own"*:
   https://code.qt.io/cgit/qt/qtbase.git/log/mkspecs/features/unix/separate_debug_info.prf?showmsg=1
   **verified**. (A Heroes build makes no dSYM, the shared brief § The evidence.)
10. **Per-toolchain Python formatters (route G's shape) are what Nim, Zig and
    Crystal ship, loaded by hand.** Nim's `nimlldb.py` keys on the emitted C's
    type names by regex (`tySequence_+...`, `NimStringV2$`) and demangles no
    variable name:
    https://raw.githubusercontent.com/nim-lang/Nim/devel/tools/debug/nimlldb.py
    **verified**. Zig's `lldb_pretty_printers.py`: the editor plugin *"won't read
    your .lldbinit"*, so users copy commands into `initCommands`; on macOS it
    failed with a `SyntaxError` at `match` under Xcode's older Python:
    https://ziggit.dev/t/display-slices-in-the-debugger/5079 **verified**.
    Crystal's formatter: *"The issue with Apple LLDB is that it does not have a
    built-in Python interpreter for scripting"* (a user, 2020-11-28, contested
    in the same forum): https://forum.crystal-lang.org/t/vanilla-lldb-cli-unable-to-import-python-type-formatting/2669
    **verified as said by that user**, not as a fact about LLDB.
11. **Zig's users went further and hit LLDB's limits**: *"Python formatters
    help a bit but don't fix expressions"*; zdb calls LLDB's *"internal APIs via
    versioned offset tables"*:
    https://ziggit.dev/t/hacking-lldb-for-a-great-zig-debugging-experience/13562
    **verified**.

### Route D: a command that starts the debugger with the formatters

12. **Rust**: `rust-lldb`, `rust-gdb`, `rust-windbg.cmd` *"are distributed with
    Rust toolchains"* and launch the debugger with the visualizers loaded:
    https://rustc-dev-guide.rust-lang.org/debuginfo/debugger-visualizers.html
    **verified**. That an lldb-dap user copies `rust-lldb`'s commands into
    `initCommands` (https://github.com/helix-editor/helix/wiki/Debugger-Configurations):
    **unverified** (a search summary; page not opened).
13. **Cython** is the nearest C-emitting precedent to B plus D: the build
    writes `cython_debug/cython_debug_info_*` and `cygdb` starts gdb with a
    temporary command file:
    https://mojo.googlesource.com/mojo/+/HEAD/third_party/cython/src/Cython/Debugger/Cygdb.py
    **verified**; `cy locals` and `cy print` show Cython-level names; gdb 7.2 or
    later *"built with Python 3 support"*:
    https://cython.readthedocs.io/en/latest/src/userguide/debugging.html
    **verified**. LLDB or Windows: not mentioned on that page.
14. **lldb-dap's `initCommands`**: *"LLDB commands executed upon debugger
    startup prior to creating the LLDB target"*; the page does not say whether
    lldb-dap reads `~/.lldbinit`: https://lldb.llvm.org/use/lldbdap.html
    **verified**.

### Route E: the author's names in what the debugger reads

15. **Nim keeps locals unmangled where it can**: *"it's always
    variablename_HASH"*, *"most local vars are not mangled at all"* (Araq,
    2017-03-07): https://irclogs.nim-lang.org/07-03-2017.html **verified**.
16. **A name map beside the build that no debugger reads: Nim's `.ndi`.** *"the
    name mapping are stored in .ndi files, debuggers can make use of it (but
    don't)"*; the user: *"It's now the first time I hear about this .ndi file"*
    (same page, **verified**); the module still *"implements the generation of
    `.ndi` files"* in the 2.2 docs: https://docs.w3cub.com/nim/compiler/ndi
    **verified**. This bears on the *slot table beside the binary* question.
17. **D's `-gc`, debug information that pretends to be C**: *"-gc Act as C, no
    D extensions"*, so *"all your classes appear as MyClass\* rather than
    MyClass"* (2010-05-02):
    https://lists.puremagic.com/pipermail/dmd-internals/2010-May/000085.html
    **verified**. Whether `-gc` still exists today: **unverified**.
18. **Odin emits type names in its own syntax, and a type-name matcher refused
    them**: the compiler's slice type name is `"[]u8"`, and vsdbg reported
    *"Invalid type name '[]u8'"*, unresolved:
    https://forum.odin-lang.org/t/natvis-compatibility-with-odin/896
    **verified**.

### Route F: the frame's temporaries

19. **LLDB hides artificial variables from `frame variable`**: D61451,
    Adrian Prantl, 2019-05-02, *"by respecting the "artificial" attribute on
    variables"*; C had no runtime plugin, and the C VLA test failed on Linux:
    https://reviews.llvm.org/D61451 **verified**. `display-runtime-support-values`
    defaults to false:
    https://raw.githubusercontent.com/llvm/llvm-project/main/lldb/source/Target/TargetProperties.td
    **verified**. Whether C source can mark a local artificial: no page read
    says so, **unverified** (a question for the compiler engineer, not a
    premise).

### Teaching the debugger the language (the route nobody listed, and why not)

20. **LLDB removed its Go and Java language plugins as unmaintained**, and
    Rust's plugin *"did not survive even in Rust's private branch due to the
    difficulty of rebasing on top of LLVM 9"* (2019-09-28 and 30):
    https://lists.llvm.org/pipermail/lldb-dev/2019-September/015546.html and
    https://lists.llvm.org/pipermail/lldb-dev/2019-October/015550.html
    **verified**. The rustc guide: LLDB *"has a plugin architecture but that
    does not work for language support"*:
    https://rustc-dev-guide.rust-lang.org/debugging-support-in-rustc.html
    **verified**.

### The Windows question

21. **LLVM's `lldb.exe` crashes without the Python DLL it was built against.**
    PR 164893, merged 2025-11-03: before #162509 lldb *"would silently crash
    (no error message/crash report)"* from PowerShell; the PR prints an error
    *"before lldb crashes due to the missing dll"*:
    https://github.com/llvm/llvm-project/pull/164893 **verified**. This matches
    `ci.yml:487`'s `0xC0000135` (the shared brief); that the CI's lldb is that
    build: **unverified**.
22. **Natvis**: Visual Studio loads a `.natvis` embedded in the PDB
    automatically (*"the debugger loads the .natvis file from the .pdb file,
    even if you don't have the project open"*), first in its search order:
    https://learn.microsoft.com/en-us/visualstudio/debugger/create-custom-views-of-native-objects
    **verified**. Rust embeds `std`'s natvis in PDBs for `*-pc-windows-msvc`:
    https://rustc-dev-guide.rust-lang.org/debugging-support-in-rustc.html
    **verified**. **Upstream LLDB reading natvis: no page found saying it
    does, unverified either way**; JetBrains maintains *"a custom fork of LLDB
    9 with hundreds of patches"* and a natvis evaluator rewritten for it, while
    upstream LLDB 21 on Linux and macOS uses *"the standard LLDB expression
    evaluator"*:
    https://blog.jetbrains.com/dotnet/2026/07/06/natvis-comes-to-linux-and-macos-in-rider-2026-2-debugger/
    **verified**. D's own debug project marks LLDB on Windows *"broken (LLDB not
    working with D Windows binaries)"*: https://github.com/Pure-D/dlang-debug/blob/master/README.md
    **verified**.
23. **`nim-gdb` is a bash script, unusable on Windows without bash**: a search
    summary of a page that returned 404 when opened, **unverified**.

## argument

Every project whose formatter is a file found by path or loaded by a command
paid for it: Go's path broke twice, Zig's editor plugin ignores `.lldbinit` and
its Python broke under Xcode's interpreter, Windows `lldb.exe` dies without its
Python DLL, and Nim's name map beside the build went unread. Swift 6.0 and LLDB
moved the other way: data embedded in the binary, loaded by `ModulesDidLoad`
with no Python and no setting, summaries mapped for Mach-O, ELF and PE/COFF.
Spickett's objection, that library authors will not write a compiler, does not
bind a compiler (inference). LLDB's removed Go and Java plugins warn against
teaching it the language. So C plus E carries the precedent; B and D carry
mostly its costs.

## condition

My reading changes if: (a) the lldb on the Linux image, in CI, or behind
lldb-dap predates the `FormatterSection` loader (its first release is
unverified here), which would make C dark exactly where B works; (b) the
bytecode cannot reach a `[T]`'s element type with no symbol lookup and no
loop, which pushes the shape into the emitted C's types (route H) rather than
into a formatter; (c) the Windows link Heroes uses truncates or drops a
14-character section name, an 8-character COFF limit being a question for the
C-boundary seat, unrun; (d) a page shows upstream LLDB reading natvis, which
would make natvis in the PDB the Windows answer.

## prediction

A `__lldbsummaries` record the build emits for one Heroes record type is
applied by this Mac's `lldb-2103.0.34.103` with no `command script import`
and no setting changed. Scored by a new case of the `lldb` suite
(`tests/harness/suite_lldb.hero`) in M-typed-inspection. The same record on the
Linux arm64 image: no prediction, its lldb version being unmeasured.

## what this seat could not run

No build, no lldb, no gdb: the seat has no shell. The critic measured
`__lldbformatters` and `__lldbsummaries` among the strings of this Mac's LLDB
framework (critic-briefs § 5); that is the critic's measurement and not this
seat's. The first LLVM release with the loaders, the CI's lldb build, the
Windows linker's treatment of long section names, and whether `strip` keeps the
sections are unrun questions.
