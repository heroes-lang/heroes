# Panel 214, the historian's report: who is told when the C compiler refuses a header

Seat: historian (advisory, no veto). Written 2026-10-14 from the two briefs
(`214-briefs/00-shared.md`, `214-briefs/historian.md`). Every precedent below carries the URL I read
in this session, and each is marked **verified** (the page I read says it, today) or **unverified**
(I could not read a page that says it). Where a sentence is my reading of a verified page rather
than the page's own words, it says *reading*. *Heroes of code* was not used for any claim here.

## verdict

**approve (advisory)**: approve the sitting replacing route A with a general rule. Precedent
supports a rule made of pieces, each of which has run elsewhere for years:

- **Location** decides whose text an error is in (B). Cgo has asked only where an error is since
  2013.
- **Every region the compiler writes is labelled** with a `#line` file name of its own, so that a
  location is a party. Cgo has done this since 2016, after its own bug was told at a user's line.
- **A name collision is decided by a reserved set of names** (C, spelled per F). C11 7.1.3 and
  Python's `Py`/`_Py` rule are the precedents.
- **What is left is settled by a probe** that gives the header exactly the unit's context (E/I).
  Cgo has probed since 2014, and corrected the probe's context in 2022.

I **object** to A continuing as the rule. Cgo read the C compiler's message text and stopped in
October 2013 because clang's wording churned (issue 6596). I also object to E or I without the
unit's full context: cgo's own probe of that kind told correct preambles they were broken until 2022
(issue 50710).

## precedents

### Route A, reading the C compiler's words shape by shape: tried and abandoned

- **Go cgo read gcc's and clang's error text until October 2013, and stopped. Verified.** Issue
  6596, "cmd/cgo: do not look at error message text", was opened by rsc on 2013-10-16 and closed on
  2013-10-18 by revision 06ad3b2 (https://github.com/golang/go/issues/6596): "cgo currently looks at
  the error message text from a trial compilation to deduce whether a name C.foo refers to a
  function, type, variable, or constant". Its fix needs "only the presence or absence of an error on
  a particular line", which is "much less dependent on the compiler details". The commit, "cmd/cgo:
  stop using compiler error message text to analyze C names"
  (https://github.com/golang/go/commit/06ad3b2de1c6608d3ec3139f7daddb67dc03e1cc), says: "As compilers
  change their errors or drop warnings, cgo breaks." "Clang in particular (now required on OS X) has
  a significant churn rate." The page does not show the commit's date, so the date is the issue's
  close. When cgo began reading the text is **unverified**.
- **A clang upgrade broke it that week. Verified.** Issue 6612, "'could not determine kind of name'
  with Xcode 5", was opened on 2013-10-17 against Apple LLVM 5.0 (clang-500.2.75). The same package
  had built with clang 4.2, and the issue was closed by the same 06ad3b2
  (https://github.com/golang/go/issues/6612).
- **The comment cgo still carries. Verified**, `src/cmd/cgo/gcc.go` on master
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/cgo/gcc.go): "We used to look at
  specific warning or error messages here, but that tied the behavior too closely to specific
  versions of the compilers. Instead, arrange that we can infer what we need from only the presence
  or absence of an error on a specific line." It still reads notes, by location only: "newer GCC
  versions will sometimes emit an error on a macro #define with a note referring to where the
  expansion occurs. We care about where the expansion occurs, so in that case treat the note as an
  error." *Reading*: that is defect 2040's shape, a macro-expansion note, which cgo met by asking
  where the note is and not what it says.
- **What A's readers have cost here.** The shared brief's table lists ten repairs, defects 2034 to
  2053, each a reader of clang's words or a warning region. They include about 100 lines of reader
  code for 2052's *did you mean* note, and a Windows-only wording for 2053. Those are the brief's
  measurements, not mine. *Reading*: it is the slope cgo described in 2013.

### Route B, the error's location decides: cgo's answer, with the compiler's text labelled

- **Location as meaning. Verified**, `src/cmd/cgo/doc.go`
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/cgo/doc.go). Cgo works "by feeding
  carefully constructed programs into the system C compiler". It puts each question under its own
  `#line 1 "not-declared"`, `#line 1 "not-type"` and so on: "An error on not-type:1 indicates that
  foo is not a type".
- **"A location is not a party" (the critic's pass 1) was cgo's bug in 2016, and cgo repaired it
  with labels. Verified.** Issue 15836, "cmd/cgo: cgo error in generated wrappers points at
  unrelated source line", was opened on 2016-05-25 (https://github.com/golang/go/issues/15836). A C
  compiler error in cgo's generated wrapper for `mpfr_get_prec` was reported at `test.go:49`, a
  comment line of the user's file. rsc suggested a `#line` directive with a different file name. It
  was fixed by commit b2290229 (CL 32613) on 2016-11-03, milestone Go1.8. The wrapper really was
  cgo's own bug: issue 17537, "cmd/cgo: cannot wrap mpfr_get_prec from libmpfr", says it was
  "caused by the `(void*)` cast introduced by" an earlier CL, because "the call is actually a
  preprocessor macro, and the cast causes the macro to fail". It was closed by fb8c896 on 2016-11-11
  (https://github.com/golang/go/issues/17537). *Reading*: a header's macro took part in that error,
  the error was the tool's, and its location in the tool's own code was the right witness once that
  code had a name of its own.
- **Today every region cgo writes is labelled. Verified**, `src/cmd/cgo/ast.go`
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/cgo/ast.go): the preamble is written
  as `#line %d %q` (the `.go` file's absolute path and line), then the user's C, then
  `#line 1 "cgo-generated-wrapper"`. `out.go`
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/cgo/out.go) opens `builtinProlog` with
  `#line 1 "cgo-builtin-prolog"` and `gccProlog` with `#line 1 "cgo-gcc-prolog"`.
- **Location-based policy in the C toolchain itself. Verified**, GCC's preprocessor manual,
  "System Headers" (https://gcc.gnu.org/onlinedocs/cpp/System-Headers.html): "All warnings, other
  than those generated by ‘#warning’" "are suppressed while GCC is processing a system header", and
  `#pragma GCC system_header` "tells GCC to consider the rest of the current include file a system
  header, no matter where it was found". *Reading*: the guard regions this tree opens around a
  group's header (`heroes_guard_open.h`, the `#pragma clang diagnostic` lines at the top of
  `tests/emission/examples-sqlite-main.c`, which I opened) belong to this lineage.
- **Swift's ClangImporter, location of the compiler's own buffers. Verified**,
  `lib/ClangImporter/ClangDiagnosticConsumer.cpp`
  (https://raw.githubusercontent.com/swiftlang/swift/main/lib/ClangImporter/ClangDiagnosticConsumer.cpp).
  `isInSwiftBuffers` asks whether a diagnostic's buffer is `moduleImportBufferName` or
  `bridgingHeaderBufferName`. If it is, the location is replaced with an invalid one, and
  `note_module_import_here` and `err_module_not_built` located there are dropped.

### Route C, the language reserves the compiler's names: decades old, necessary but not sufficient

- **C itself, C11 7.1.3. Verified**, N1570
  (https://port70.net/~nsz/c/c11/n1570.html): "All identifiers that begin with an underscore and
  either an uppercase letter or another underscore" are "always reserved for any use"; "All
  identifiers that begin with an underscore are always reserved for use as identifiers with file
  scope". And: "No other identifiers are reserved. If the program declares or defines an identifier
  in a context in which it is reserved", the behaviour is undefined.
- **C++: the reservation exists for the implementation's generated names. Verified**, CWG issue 1882,
  "Reserved names without library use", 2014-02-26, status CD4
  (https://cplusplus.github.io/CWG/issues/1882.html): "implementations rely on this restriction for
  mangling, even when no standard library facilities are used." **Unverified**: that cfront's
  C-emitting mangling is where the C++ rule came from. I could not reach a primary cfront source,
  because the Dr. Dobb's host did not resolve.
- **Python's C API. Verified**, "Include Files" (https://docs.python.org/3/c-api/intro.html): "All
  user visible names defined by Python.h (except those defined by the included standard headers)
  have one of the prefixes `Py` or `_Py`." "User code should never define names that begin with
  `Py` or `_Py`. This confuses the reader, and jeopardizes the portability of the user code to
  future Python versions".
- **Where that rule did not reach. Verified.** In bpo-34657, message of 2018-09-13
  (https://bugs.python.org/msg325235), Windows `PC/pyconfig.h` maps `timezone`, `daylight` and
  `tzname` to their underscore names. A user header with a struct field `timezone`, included before
  `Python.h`, failed with "function cannot be member of struct". A capi-sig thread started on
  2020-04-09 says Python.h's `snprintf` macro "collides with std::snprintf"
  (https://mail.python.org/archives/list/capi-sig@python.org/thread/5NXBZWKBMAPJJLNIXASSAYRIAP2OHJ53/).
  *Reading*: a prefix rule covers the runtime's own spellings and not the C library names the
  runtime's headers bring in. That is the critic's finding that C must reserve "more than four
  prefixes", and the shape of defects 2044, 2049 and 2051.
- **CHICKEN Scheme. Verified**, "Module (chicken foreign)"
  (https://wiki.call-cc.org/man/5/Module%20(chicken%20foreign)): `foreign-declare` will "Include given
  strings verbatim into header of generated file", and the runtime's names begin with `C_`: "It is
  advisable to use a different naming scheme for your own code to avoid name clashes." This is
  advice, not a rule with a diagnostic.

### Route D, isolation: the only family where blame is clean, and what leaving the unit cost GHC

- **Zig `@cImport`. Verified.** The 0.14.0 language reference
  (https://ziglang.org/documentation/0.14.0/): it "parses C code and imports the functions, types,
  variables, and compatible macro definitions" into "a new empty struct type", and `@cInclude`,
  `@cDefine` and `@cUndef` append to "a temporary buffer that is parsed as C". A failure is told at
  the `@cImport` call. A user's post of 2026-03-21 on Zig 0.15.2
  (https://ziggit.dev/t/why-cannot-zig-find-standard-c-header/14671) shows
  `foo.zig:2:11: error: C import failed`, a note, and then clang's own error located in the import
  buffer `cimport.h:1:10`. Issue 20630, "move `@cImport` to the build system", was opened on
  2024-07-14 and closed as completed on 2026-04-20; it removes "one dependency on libclang inside
  the Zig compiler executable" (https://github.com/ziglang/zig/issues/20630).
- **Swift. Verified**, `DiagnosticsClangImporter.def`
  (https://raw.githubusercontent.com/swiftlang/swift/main/include/swift/AST/DiagnosticsClangImporter.def):
  `clang_cannot_build_module`, "could not build %select{C|Objective-C}0 module '%1'", and
  `bridging_header_error`, "failed to import bridging header '%0'". The consumer emits the first
  when clang's `err_module_not_built` names the module being imported (ClangDiagnosticConsumer.cpp,
  URL above). Isolation leaks where it is partial: a forum post of 2022-02-04 on SR-15822
  (https://forums.swift.org/t/sr-15822-clangimporters-bug-why-does-it-import-separately-clang-modules-and-bridging-c-headers/55091)
  says that the bridging header shares the modules' one `clang::Preprocessor`, so include guards
  hide declarations from the bridging header's lookup table.
- **Clang modules. Verified**, Clang's "Modules" page (https://clang.llvm.org/docs/Modules.html):
  textual inclusion is "subject to any active macro definitions at the time of inclusion", and name
  collisions "can break the library API or cause compilation failures in the library header itself".
  Under modules, "Each module is parsed as a standalone entity, so it has a consistent preprocessor
  environment."
- **Rust bindgen. Verified**, `bindgen/lib.rs`
  (https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen/lib.rs): every diagnostic
  with `d.severity() >= CXDiagnostic_Error` is gathered into `BindgenError::ClangDiagnostic`, "Clang
  diagnosed an error" (https://docs.rs/bindgen/latest/bindgen/enum.BindgenError.html), and anything
  milder is printed as `clang diag:`. `BindgenError` itself came with PR #2125, merged 2021-12-29
  (https://github.com/rust-lang/rust-bindgen/pull/2125). **Unverified**: which variants it carried
  then, and that bindgen adds no text of its own to the unit it parses.
- **GHC took the headers out of its generated C in 6.10.1. Verified**, release notes § 1.4.1
  (https://downloads.haskell.org/ghc/6.10.1/docs/html/users_guide/release-6-10-1.html): "FFI change:
  header files are now not used when compiling via C." Among its reasons: "we don't need to worry
  about the interactions between header files, or CPP options necessary to expose certain functions
  from the system header files (this was becoming quite a headache)." What it cost: "One downside is
  that you don't get a warning from the C compiler when you call a function via the FFI at the wrong
  type"; varargs calls "may now really cause a crash on certain platforms"; and a macro needs "a C
  wrapper". The users' guide of 7.0.1
  (https://downloads.haskell.org/ghc/7.0.1/docs/html/users_guide/ffi-ghc.html) says "Earlier versions
  of GHC (6.8.3 and earlier) #included the header file in the C source file generated from the
  Haskell code".
- **GHC's way back is a shim per import, the route-D shape. Verified.** `CApiFFI`, "Since: 7.6.1"
  (https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/ffi.html): "we instead call f using
  the C API defined in the header header.h". Ben Gamari, 2021-08-02
  (https://well-typed.com/blog/2021/08/capi-usage): "GHC will construct a C source file which
  `#include`'s `stdio.h`" "and defines a stub function which performs the call", so "the calling
  convention is decided by the C compiler using the signature provided in the indicated header file".
  **Unverified**: that the stub's file holds nothing else of GHC's.
- *Reading*: in every system I found that compiles the program's header apart from its own
  generated code (Zig, Swift, bindgen), a refusal is told as the import's or the header's, with no
  case analysis. Of the systems that compile the header inside their generated C, Nim and Vala print
  the C compiler's words and classify nothing (route K, below), and cgo built the pieces this report
  describes. How Cython and GHC up to 6.8.3 told a refusal I did not find, so it is **unverified**
  either way. The enumeration comes from the brief's list plus what the searches surfaced; it is
  not a survey.

### Route E and I, a probe of the program's C: cgo did it, and its probe false-blamed for eight years

- **The probe. Verified.** Issue 8442, "cmd/cgo: request for better error messages", was opened on
  2014-07-29 and closed on 2014-08-14 by ba30c08 (https://github.com/golang/go/issues/8442). The
  reporter asked that cgo say "the C files it compiles with your snippet don't work *at all*". Ian
  Lance Taylor called the old message "correct in a sense" but "in context it is very unhelpful".
  Today's code (gcc.go, URL above) reads: "Check if compiling the preamble by itself causes any
  errors, because the messages we've printed out so far aren't helpful to users debugging preamble
  mistakes. See issue 8442." It runs `p.gccErrors([]byte(builtinProlog + f.Preamble))` and prints
  `"\n%s errors for preamble:\n%s"`.
- **The probe's false blame. Verified.** Issue 50710, "cmd/go: reports errors for a correct preamble
  when it contains a builtin C type and a `C.*` name can't be resolved", was opened on 2022-01-20
  and closed by 4f45ec5 on 2022-06-25, milestone Go 1.19 (https://github.com/golang/go/issues/50710):
  "`_GoString_` is defined in the buildin prolog but we are not adding the prolog to the preamble
  when checking if the preamble has any error." *Reading*: a probe that gives the header less than
  the unit gives it tells a correct program it is wrong. That is the critic's objection to
  `cli/header_alone.hero` as route E's probe, met by cgo and repaired eight years after the probe
  landed.

### Route F, the compiler's names in C's reserved space: what three toolchains do

- **Cython. Verified**, `Cython/Compiler/Naming.py`
  (https://raw.githubusercontent.com/cython/cython/master/Cython/Compiler/Naming.py): "Prefixes for
  generating C names." "Collected here to facilitate ensuring uniqueness." `pyrex_prefix = "__pyx_"`,
  plus `__cyt_` for temporaries and `__PYX_HAVE_` for guards. One generated name that escaped the
  prefix was filed as a bug: Launchpad 188154, "Module cleanup function name not mangled", reported
  on 2008-02-01, status Fix Committed (https://bugs.launchpad.net/bugs/188154). Its worry was a user
  "using a C library that defines a function called cleanup". Cython places a `#include` of the
  named header in its generated C, and "Cython does not itself read the C header file"
  (https://cython.readthedocs.io/en/latest/src/userguide/external_C_code.html). All of this is
  verified.
- **Cgo. Verified**, `src/cmd/cgo/main.go`
  (https://raw.githubusercontent.com/golang/go/master/src/cmd/cgo/main.go): "Need a unique prefix for
  the global C symbols that we use to coordinate between gcc and ourselves. We already put _cgo_ at
  the beginning, so the main concern is other cgo wrappers for the same functions." The code is
  `cPrefix = fmt.Sprintf("_%x", h.Sum(nil)[0:6])`, over the import path and the files. The one
  collision I found is cgo with itself: issue 23555, opened on 2018-01-25, "multiple definition of
  `_cgo_05dc84caff82_Cfunc_sysconf'", for one package vendored twice. It was repaired by hashing the
  import path too, CL 118739, Go 1.11 (https://github.com/golang/go/issues/23555). The other side,
  also verified: `builtinProlog` declares unprefixed names in the user's space (`intgo`, `GoString`,
  `GoBytes`, `CString`, `CBytes`; out.go, URL above), and issue 27019, "conflicting types for
  '_GoString_'", 2018, was a collision of cgo's prolog with itself through an exported header
  (https://github.com/golang/go/issues/27019).
- **Go's runtime in 2010. Verified**, commit 68b4255a by Russ Cox, 2010-11-04
  (https://go.googlesource.com/go/+/68b4255a9644a838898bfc6567bae97585c2e871): "Prefix all external
  symbols in runtime by runtime·, to avoid conflicts with possible symbols of the same name in
  linked-in C libraries. The obvious conflicts are printf, malloc, and free, but hide everything to
  avoid future pain."
- **What F costs. Verified.** C11 7.1.3 makes a program that declares a reserved identifier
  undefined (above). Clang 13 added "`-Wreserved-identifier` emits warning when user code uses
  reserved identifiers"
  (https://releases.llvm.org/13.0.0/tools/clang/docs/ReleaseNotes.html). **Unverified**: whether
  `-Wall` or `-Wextra` enables it. The documentation page was cut before that entry each of the
  three times I read it.
- *Reading*: the searches that found the collisions above found no report of a user header
  colliding with a `__pyx_` or `_cgo_<hash>_` name. That rests on the words searched ("`__pyx_`
  conflict", "`_cgo_` prefix conflict"), and it is a question, not a finding.
- **Nim, the opposite choice. Verified.** The 0.16.0 `nimbase.h`
  (https://web.mit.edu/nim-lang_v0.16.0/nim-0.16.0/lib/nimbase.h) defines unprefixed `NI`, `NU`,
  `likely`, `unlikely`, `NAN`, `INF` and `string`, and turns off a list of clang warnings for
  generated files, `-Wmacro-redefined` among them.

### Route G, structured diagnostics: in-process works; the command-line formats churn

- **The in-process consumers. Verified**: Swift's consumer receives a `clang::Diagnostic` and
  switches on its ID (URL above), and bindgen reads libclang's severity (URL above).
- **Command-line formats. Verified.** GCC 9 introduced "-fdiagnostics-format=json"
  (https://gcc.gnu.org/gcc-9/changes.html). GCC 13 added SARIF
  (https://gcc.gnu.org/gcc-13/changes.html). In GCC 16: "The so-called "json" format for
  -fdiagnostics-format= has been removed in this release." "Users seeking machine-readable
  diagnostics from GCC should use SARIF." (https://gcc.gnu.org/gcc-16/changes.html). Clang's manual
  (https://clang.llvm.org/docs/UsersManual.html): "The SARIF diagnostic format is currently
  unstable." *Reading*: a structured command-line format has its own churn record, a format removed
  seven major versions after it arrived, which is the cost cgo left message text to escape.

### Route H, the runtime read after the program's headers: cgo moved this way in 2013

- **Verified.** Issue 6558, opened on 2013-10-10 and closed on 2013-10-15 by 5feb155
  (https://github.com/golang/go/issues/6558). A new `#include <sys/types.h>` in cgo's `builtinProlog`
  was read before the preamble's `_GNU_SOURCE` and `_FILE_OFFSET_BITS`, so those switches did
  nothing. The commit, "cmd/cgo: print the builtin prolog after the per-file preamble"
  (https://github.com/golang/go/commit/5feb15508e9cefa06f7d109da8233c91e69937fa), says: "The preamble
  may want to #define some special symbols and then #include <sys/types.h> itself." This tree met
  the same thing as panel 205's R3 and defect 568 (`runtime/heroes_unit.h`'s comment, which I
  opened).
- **The unrepaired instance. Verified.** Issue 35315, "cmd/cgo: inject preamble before other include
  directives", was opened on 2019-11-02 and is open, milestone Unplanned
  (https://github.com/golang/go/issues/35315). `_cgo_export.c` reads `stdlib.h` before the preamble,
  and Python.h says "you must include Python.h before any standard headers are included"
  (https://docs.python.org/3/c-api/intro.html, verified). aclements wrote in 2020-12: "currently we
  don't have a plan to address this."
- **The other side of H. Verified**: cgo's preamble may not hold definitions when `//export` is used
  in the file, because it "is copied into two different C output files" (https://pkg.go.dev/cmd/cgo;
  issue 10982, 2015-05-29, closed as working as intended,
  https://github.com/golang/go/issues/10982). *Reading*: when the program's C reaches several units,
  each with its own order, the rule has to be stated per unit. That is the brief's list of
  `two_ways_ask`, `label_dump`, `defined_twice`, `reads` and `header_alone`.

### Route J, scanning the header's names first

- **Verified**, cgo doc.go (URL above): cgo first runs the C compiler on the preamble with
  `gcc -E -dM` to learn its simple `#define`s, and reads the DWARF of `__cgo__N` to learn each
  name's type. *Reading*: J is how cgo learns names. It is not how cgo assigns blame.

### Route K, telling what was established and blaming no one: what the C-emitting family does by default

- **Nim. Verified**, `compiler/extccomp.nim`
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/extccomp.nim): Nim prints the C
  compiler's own output and reports "execution of an external compiler program '...' failed with
  exit code: ". It parses none of the output.
- **Vala. Verified**, vala-list, Abderrahim Kitouni, 2010-10-04
  (https://mail.gnome.org/archives/vala-list/2010-October/msg00003.html): "if valac generates invalid
  C code it's a bug, either in valac or (more likely) in the bindings."
- *Reading*: K alone is what cgo's issue 8442 asked to be rescued from (above).

## argument

Where the header is compiled apart from generated code (Zig, Swift, bindgen), the refusal is told as
the import's, without case analysis. Where the two mix, Nim and Vala blame nobody; cgo converged on
three things. It reads where an error is, never what it says (2013, after clang's churn). It labels
each region it writes with its own `#line` file (2016, after its own bug surfaced at a user's line).
It probes the user's C alone, which false-blamed until it got the full prolog (2022). A is cgo
before 2013. B with labels and a full-context E is cgo after, and C is Python's rule for collisions.
D alone is clean, and GHC paid C-compiler type checking for it.

## condition

My reading would change on any of these four findings:

- A text-reading attribution layer that stayed stable across clang versions for five years or more:
  that would rehabilitate A.
- A report of a user header colliding with `__pyx_`- or `_cgo_`-style reserved names: that would
  weaken F.
- A mixing toolchain whose location rule wrongly blamed a user for an error in labelled generated
  code: that would weaken B.
- Evidence that GHC's CApiFFI stubs, or the C file of bindgen's `wrap_static_fns`, carry collision
  bugs of their own: that would weaken D as the clean ending. The `wrap_static_fns` C file, with its
  default suffix `__extern`, is verified at https://docs.rs/bindgen/latest/bindgen/struct.Builder.html.

## The route I would adopt, and the route I refuse

**Adopt: B as the first test, made trustworthy by labels, with C/F for collisions and E/I with the
unit's full context for the remainder.** In detail, by precedent:

1. An error located in the group's header, or in a file it reaches that is neither the compiler's
   nor the C library's, is the header's (B).
2. An error located in text the compiler wrote (the runtime, the unit's own lines, the emitted body
   behind a `.hero` `#line`) is the compiler's. The exception is a note of the same diagnostic that
   lies in the header, at its declaration of a reserved identifier. That collision is the header's by
   rule (C), and the message names both sides.
3. Any remaining case is settled by a probe that compiles the unit as written with the bindings'
   uses replaced (I), or the header after exactly the unit's preamble (E). Cgo's 50710 is the reason
   for "exactly".

What precedent does not settle: whether the `.hero` `#line` mapping needs cgo's
`cgo-generated-wrapper` treatment. That treatment puts a `#line` back to a compiler-owned name
around every region the emitter writes, so that the location alone is a party. `generated-c.md`
says the `#line` is "restored to the generated file around synthetic code". Whether that already
covers it is a question for the compiler-engineer and has not been run.

**Refuse: A as the standing rule.** The fact that refuses it is cgo's 2013 record: the commit that
ended it says "As compilers change their errors or drop warnings, cgo breaks", and "Clang in
particular ... has a significant churn rate". This tree's own instance is defect 2053, whose words
differ on Windows (brief).

**Not refused, recorded as the conservative alternative**: D, isolation by a shim per group, GHC's
CApiFFI shape. It is the only precedent with clean blame. It costs what GHC 6.10.1 lists, or what
CApiFFI pays to get the C compiler's check back. It is an architecture change, beyond this seat's
evidence.

## prediction

1. **If A stays the rule, I predict that the first change of clang version on any CI leg (Apple
   clang on macOS, the distribution clang on Linux, clang 23.1.1 on the Windows box per the brief)
   will move at least one of the 55 `#~ ffi_header_refused` cases or the five `run` cases that must
   keep building, in exit code or message.** Instrument: the `unsupported` and `run` golden forms on
   the CI legs. When: the first CI run after a toolchain bump. Ground: cgo issue 6612, a clang
   upgrade (clang 4.2 to Apple LLVM 5.0) that broke cgo's reading, closed by the commit that stopped
   reading text. This is a forecast. I did not count the cases; the 55 is the brief's.
2. **If E or I is adopted with a probe that gives the header less than the unit does, I predict
   that the census the brief names (the 878 tracked programs with an `extern`, built with this
   tree's compiler and with the route's, exit code and message compared) will show at least one
   correct header told refused.** Instrument: that census. When: at the lane's gate. Ground: cgo
   issue 50710.

## What I could not run or read

- No compiler, no suite, no census: this seat's tools are reading and web search. Nothing above
  measures this tree. The tree facts I cite are the brief's, or what the two files I opened say
  (`runtime/heroes_unit.h`, `tests/emission/examples-sqlite-main.c`).
- Pages I could not read:
  - The GHC GitLab commit c245355e: an Anubis block.
  - go.googlesource.com for 06ad3b2d: HTTP 503.
  - The Gambit manual: the host did not resolve, so its `___` convention stays unverified and
    uncited.
  - Dr. Dobb's on cfront: the host did not resolve.
  - Clang's DiagnosticsReference entry for `-Wreserved-identifier`: the page was cut before the
    entry.
  - Zig's `zirCImport` source: the file was cut before the function.
- I did not grep `design.md` or `docs/panel/` for earlier sittings' precedents on this question.
  This seat's tools have no grep, so an earlier historian's finding on the same ground may exist
  unread.

## Candidate `## Prior art` lines (for the session that files the sitting, re-read before use)

Each line below says only what its page states. Re-read every page before filing, as
`.claude/rules/records.md` § And the record says whose idea it was requires.

- **Reached first by Go (cgo)**: [golang/go#6596](https://github.com/golang/go/issues/6596): cgo
  stopped deducing what a C name is from the compiler's error message text, needing "only the
  presence or absence of an error on a particular line".
- **Reached first by Go (cgo)**: [golang/go#15836](https://github.com/golang/go/issues/15836): a C
  compiler error in cgo's generated wrapper was reported at an unrelated line of the user's Go file,
  and a `#line` directive with a different file name was proposed.
- **Reached first by Go (cgo)**: [golang/go#50710](https://github.com/golang/go/issues/50710): cgo's
  check of a preamble compiled alone reported errors for a correct preamble because the builtin
  prolog was not added to it.
- **Reached first by Go (cgo)**: [golang/go#6558](https://github.com/golang/go/issues/6558): an
  include in cgo's builtin prolog, read before the preamble's feature-test macros, left those macros
  without effect.
- **Reached first by GHC**: [GHC 6.10.1 release notes](https://downloads.haskell.org/ghc/6.10.1/docs/html/users_guide/release-6-10-1.html):
  header files are not used when compiling via C, one reason being the interactions between header
  files and CPP options.
- **Reached first by Python**: [Python/C API, Include Files](https://docs.python.org/3/c-api/intro.html):
  user code should never define names that begin with `Py` or `_Py`.
