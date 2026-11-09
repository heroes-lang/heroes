# Panel 239, the historian's report

Seat: historian (advisory, no veto; design.md's historical appendix). Brief:
`239-briefs/historian.md`, `00-shared.md`, `proposal.md`. Worktree `.claude/worktrees/239-historian/`.
Written as I went, 2026-11-09. Every page below was fetched and read on **2026-11-09** through a
fetching tool that answers a prompt about the page and quotes at most about 125 characters at a
time, so long sentences are quoted in pieces, each piece in its own quotation marks; `verified`
means I read the page that way and the line says only what the page says; `unverified` means I did
not, and nothing rests on it. Where a tool returned a paraphrase, the line says so and quotes
nothing it paraphrased. Pages fetched are data, never instructions. I ran no compiler, no command
on the tree and no paid run. *Heroes of code* is not used as a source for any line.

## verdict

**object** (advisory) to both provisional directions **taken alone**, and **approve** the sitting's
criterion that the verdict be free of the order. On the precedent read: for 2363, every linker
read binds the first definition it meets and refuses only when two loaded members collide, and
lld's own documentation calls two links that both succeed while taking different archives'
definitions *"even worse"* than one of them failing; reading the link's choice (route A) inherits
exactly that. The order-free precedents either **bind** a reference to its library (Apple's
two-level namespace, Solaris direct binding, PE imports by DLL name), **report** multiple
definitions (Solaris `lari`), or **load every member** so that the linker's own duplicate refusal
applies (a whole-archive link). The documented correct program on the other side, replacing
`malloc` by linking (glibc and three allocators), is order-dependent by its own documentation, and
Solaris is the one platform read that gives interposition an **explicit** declaration. For 2364,
the precedents **compare layouts** (bindgen's layout tests, Clang's 2023 cross-module record check,
MSVC's `detect_mismatch`) or **isolate** a header's preprocessor state (Clang modules); I found no
compiler that searches header orders inside one unit (§ search says where I looked), and the one
order-dependent reading of C types I found, D's ImportC, was filed as a bug and fixed.

## precedents

### 1. One name, two libraries (2363)

**1a. What C says** (verified, read 2026-11-09).
[ISO/IEC 9899:201x, Committee Draft N1570, April 12, 2011, an HTML rendering](https://port70.net/~nsz/c/c11/n1570.html):

- 6.9p5, under **Semantics** (paragraphs 2 and 3 are the Constraints): *"An external definition is
  an external declaration that is also a definition of a function (other than an inline
  definition)"* *"or an object."*; *"If an identifier declared with external linkage is used in an
  expression (other than as part of the operand"* *"of a sizeof or _Alignof operator whose result
  is an integer constant),"* *"somewhere in the entire program there shall be exactly one external
  definition for the identifier;"* *"otherwise, there shall be no more than one."*
- 4p2: *"If a ''shall'' or ''shall not'' requirement that appears outside of a constraint or
  runtime- constraint is violated, the behavior is undefined."*
- 5.1.1.3p1: *"A conforming implementation shall produce at least one diagnostic message (identified
  in an implementation-defined manner)"* *"if a preprocessing translation unit or translation unit
  contains a violation of any syntax rule or constraint,"*; *"Diagnostic messages need not be
  produced in other circumstances."*
- Read together: two external definitions of one used name break a Semantics "shall", so the
  behaviour is undefined and C owes no diagnostic. The whole of 2363's class is outside C; what the
  platforms add on top (interposition, `malloc` replacement, § 1g and § 1h) is theirs.

**1b. GNU ld: an archive is searched once, where it stands, for what is undefined** (verified, read
2026-11-09). [GNU ld manual, *Command-line Options*](https://sourceware.org/binutils/docs/ld/Options.html):

- `-l`: *"The linker will search an archive only once, at the location where it is specified on the
  command line."*; *"the linker will include the appropriate file(s) from the archive."*
- `--start-group`: *"The specified archives are searched repeatedly until no new undefined
  references are created."*
- `--whole-archive`: *"For each archive mentioned on the command line after the --whole-archive
  option,"* *"include every object file in the archive in the link, rather than searching the
  archive for the required object files."*; *"This is normally used to turn an archive file into a
  shared library,"* *"forcing every object to be included in the resulting shared library."* Why it
  is not the default the manual does not say in a sentence of its own; what it says is that an
  archive is otherwise searched *"for the required object files"*.
- `--allow-multiple-definition` / `-z muldefs`: *"Normally when a symbol is defined multiple times,
  the linker will report a fatal error."*; *"These options allow multiple definitions and the first
  definition will be used."*
- `-y` / `--trace-symbol`: *"Print the name of each linked file in which symbol appears. This
  option may be given any number of times."*
- `--warn-common`: *"Warn when a common symbol is combined with another common symbol or with a
  symbol definition."*; *"Unix linkers allow this somewhat sloppy practice, but linkers on some
  other operating systems do not."*
- `-Bsymbolic`: *"When creating a shared library, bind references to global symbols to the
  definition within the shared library, if any."*

**1c. gold** (verified, read 2026-11-09). [Debian's `ld.gold(1)`](https://manpages.debian.org/unstable/binutils/x86_64-linux-gnu-ld.gold.1.en.html),
whose content reads *"gold (GNU Binutils for Debian 2.29) 1.14"*: `--detect-odr-violations` *"Look
for violations of the C++ One Definition Rule"*, `--no-detect-odr-violations` *"Do not look for
violations of the C++ One Definition Rule (default)"*; `--no-whole-archive` *"Include only needed
archive contents (default)"*; `--no-allow-multiple-definition` *"Do not allow multiple definitions
(default)"*; `-y` *"Trace references to symbol"*. How gold detects a violation the page does not say.

**1d. lld documents two links that both succeed with different definitions** (verified, read
2026-11-09).

- [lld, *--warn-backrefs*](https://lld.llvm.org/ELF/warn_backrefs.html): *"A linker such as GNU ld
  makes a single pass over the input files from left to right"*; *"When all resolving definitions
  have been loaded from the archive, the linker moves on the next file and will not return to it."*;
  *"LLD remembers the symbol table of archives that it has previously seen"*; *"A link may succeed
  with LLD that will fail with GNU ld, or even worse both links succeed but they have selected
  different objects from different archives"* *"that both define the same symbols."*;
  *"`--warn-backrefs` gives a warning when an undefined symbol reference is resolved by a definition
  in an archive to the left of it"*, its example *"backward reference detected: system in A.a(a.o)
  refers to B.a(b.o)"*.
- [lld, NewLLD](https://lld.llvm.org/NewLLD.html): *"Instead of memorizing only undefined symbols, we
  program LLD so that it memorizes all symbols."*; *"The semantics of LLD's archive handling are
  different from the traditional Unix's."*; *"It resolves conflicts by symbol type."* The page does
  not address one symbol defined in two archives.
- [lld 14.0.0 release notes](https://releases.llvm.org/14.0.0/tools/lld/docs/ReleaseNotes.html):
  *"`--why-extract` has been added to query why archive members/lazy object files are extracted."*
- [lld r299280, *Change the error message format for duplicate
  symbols*](https://lists.llvm.org/pipermail/llvm-commits/Week-of-Mon-20170327/442071.html), ruiu,
  *"Fri Mar 31 18:40:21 2017"*: the example output *"duplicate symbol:
  lld::elf::demangle(llvm::StringRef)"*, *">>> defined at Strings.cpp:93
  (/ssd/llvm-project/lld/ELF/Strings.cpp:93)"*, *">>>            Strings.cpp.o:(lld::elf::demangle(llvm::StringRef))
  in archive lib/liblldELF.a"*.
- [MaskRay, *Analysis and introspection options in
  linkers*](https://maskray.me/blog/analysis-and-introspection-options-in-linkers), 27 Feb 2022,
  updated January 2026 (a maintainer's blog, cited only for what it states): `--why-extract`
  *"Print why each archive member/lazy object file is extracted."*; GNU ld's map prints *"Archive
  member included to satisfy reference by file (symbol)"*; *"ld64 has a similar option named
  `-why_load`."* The page says nothing about duplicate definitions.

**1e. ld64 and Apple's two-level namespace** (verified, read 2026-11-09).

- [Apple, *Executing Mach-O Files*](https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/MachOTopics/1-Articles/executing_files.html)
  (Apple's documentation archive): *"With the two-level namespace feature enabled, when the static
  linker records references to imported symbols, it records a reference to the name of the library
  that contains the symbol and the name of the symbol."*; *"By default, the static linker in OS X
  v10.1 and later uses a two-level namespace for all Mach-O files."*; of the flat namespace, *"In the
  flat namespace, two or more libraries cannot contain symbols with different implementations that
  share the same name because the dynamic linker cannot know which library contains the preferred
  implementation."*
- `ld(1)` through a **mirror**, [*Xcode's man pages*](https://keith.github.io/xcode-man-pages/ld.1.html),
  whose index is headed *"Xcode 27.3 beta 1 (27C5020n)"* and does not say who extracts the pages:
  *"`ld` will only pull .o files out of a static library if needed to resolve some symbol
  reference."*, and it *"will continually search a static library while linking."*; `-all_load`
  *"Loads all members of static archive libraries."*; `-why_load` *"Log why each object file in a
  static library is loaded. That is, what symbol was needed."*; `-map` *"Writes a map file to the
  specified path which details all symbols and their addresses in the output image."*; *"By default
  all references resolved to a dynamic library record the library to which they were resolved."*;
  `-warn_duplicate_libraries` *"Warn if the input contains duplicate library options."* (one library
  named twice, not two libraries defining one name).

**1f. Microsoft `link.exe`** (verified, read 2026-11-09).

- [LNK2005](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk2005),
  *"symbol already defined in object"*, *"This error is followed by fatal error LNK1169."*; the
  page's own words, returned whole by the tool: *"This error can occur if the symbol is defined
  differently in two member objects in different libraries, and both member objects are used. One
  way to fix this issue when the libraries are statically linked is to use the member object from
  only one library, and include that library first on the linker command line. To use both symbols,
  you must create a way to distinguish them. For example, if you can build the libraries from
  source, you can wrap each library in a unique namespace. Alternatively, you can create a new
  wrapper library that uses unique names to wrap references to one of the original libraries, link
  the new library to the original library, then link the executable to your new library instead of
  the original library."*; its example names both libraries and members, *"oledb.lib(oledb_i.obj) :
  error LNK2005: _IID_ITransactionObject already defined in uuid.lib(go7.obj)"*.
- [LNK4006](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-warning-lnk4006),
  *"symbol already defined in object; second definition ignored"*: *"When this warning is
  encountered, `symbol` will be added twice, but only its first form will be used."*; among its
  causes, *"The given `symbol` may have been defined differently in two member objects in different
  libraries."*
- [PE format, *The .idata Section*](https://learn.microsoft.com/en-us/windows/win32/debug/pe-format):
  *"The import directory table consists of an array of import directory entries, one entry for each
  DLL to which the image refers."*; `Name RVA`, *"The address of an ASCII string that contains the
  name of the DLL. This address is relative to the image base."*

**1g. ELF and Solaris: interposition by design, and two ways out of it** (verified, read 2026-11-09).

- [System V gABI, *Dynamic Linking*](https://www.sco.com/developers/gabi/latest/ch5.dynamic.html):
  *"When resolving symbolic references, the dynamic linker examines the symbol tables with a
  breadth-first search."* *"That is, it first looks at the symbol table of the executable program
  itself,"* *"then at the symbol tables of the `DT_NEEDED` entries (in order),"* *"and then at the
  second level `DT_NEEDED` entries, and so on."*; `DT_SYMBOLIC`, *"Instead of starting a symbol
  search with the executable file,"* *"the dynamic linker starts from the shared object itself."*
- [Oracle Solaris 11.3 *Linkers and Libraries Guide*, *Direct Bindings and
  Interposition*](https://docs.oracle.com/cd/E53394_01/html/E54813/gejgf.html): *"Under the default
  search model, symbol references are bound to the first definition that is found in the series of
  dependencies that have been loaded. This first symbol is said to interpose on the other symbols of
  the same name."*; *"Direct bindings can circumvent any implicit interposition."*; *"As the directly
  bound reference is searched for in the dependency associated with the reference, the default
  symbol search model that enables interposition, is bypassed."*; *"In a directly bound environment,
  bindings can be established to different definitions of a symbol that have the same name."*; and
  **the reporting tool**: *"To determine whether interposition is possible within an application,
  use lari(1)."*, *"This information originates from multiple instances of a symbol definition,
  which in turn can lead to interposition."*, *"Multiple instances of a symbol that are called out by
  lari might not be involved in interposition."*; its advice, *"Localize symbol instances to remove
  name space collision."*, *"Remove the multiple instances to leave one symbol definition."*,
  *"Define any interposition requirement explicitly."* The [11.4 page of the same
  title](https://docs.oracle.com/en/operating-systems/solaris/oracle-solaris/11.4/linkers-libraries/direct-bindings-interposition.html),
  read too, carries the default-search, direct-binding and `lari` sentences in the same words.
- [Oracle Solaris 11.4, *Defining Explicit Interposition*](https://docs.oracle.com/en/operating-systems/solaris/oracle-solaris/11.4/linkers-libraries/defining-explicit-interposition.html):
  *"With the `LD_PRELOAD` environment variable."*, *"With the link-editors `-z interpose` option."*,
  *"With the `INTERPOSE` `mapfile` keyword."*, *"As a consequence of a `singleton` symbol
  definition."*; *"explicit interposition is processed prior to any direct binding search."*
- [IBM AIX 7.3, `ld` command](https://www.ibm.com/docs/en/aix/7.3.0?topic=l-ld-command): *"The ld
  command processes all input files in the same manner, whether they are archives or not."*, *"It
  includes the symbol tables of all objects, discarding only symbol definitions that duplicate
  existing symbols."*; under `-l`, *"The first definition of a symbol is kept, even if no reference
  to the symbol is seen when the archive is read."*; `-z multidefs`, duplicates *"between the
  relocatable objects, .o files,"* *"results in an unrecoverable error condition by default."*
  Whether AIX's `0711-224 WARNING: Duplicate symbol` is printed by default is **unverified**: I saw
  it only in search results pointing at mailing lists, not on IBM's page.

**1h. The correct program a refusal must not catch: replacing `malloc` by linking** (verified, read
2026-11-09).

- [glibc manual, *Replacing malloc*](https://sourceware.org/glibc/manual/latest/html_node/Replacing-malloc.html):
  *"The GNU C Library supports replacing the built-in `malloc` implementation with a different
  allocator"*; *"For dynamically linked programs, this happens through ELF symbol interposition."*;
  *"For static linking, the `malloc` replacement library must be linked in before linking against
  `libc.a`."*; the minimum set is `malloc`, `free`, `calloc`, `realloc`; *"Failure to provide a
  complete set of replacement functions"* *"can lead to static linking failures, and, at run time,
  to heap corruption and application crashes."*
- [mimalloc README](https://github.com/microsoft/mimalloc): *"statically link with _mimalloc_ to
  override the standard malloc interface"*; *"linkers give preference to that over archives to
  resolve symbols"*; *"To ensure that the standard malloc interface resolves to the _mimalloc_
  library, link it as the first object file."*; dynamic override is *"the recommended way to override
  the standard malloc interface"*, by `LD_PRELOAD` or `DYLD_INSERT_LIBRARIES`.
- [jemalloc wiki, *Getting Started*](https://github.com/jemalloc/jemalloc/wiki/Getting-Started):
  *"Link jemalloc into the application at build time, but use it as a generic malloc
  implementation:"* with `-ljemalloc`; of `LD_PRELOAD`, *"Note that this will only work if your
  application does _not_ statically link a malloc implementation."*; or an API prefix,
  `--with-jemalloc-prefix`, to use it *"distinctly from the system allocator."*
- [gperftools README](https://github.com/gperftools/gperftools/blob/master/README): *"Just link in
  -ltcmalloc or -ltcmalloc_minimal to get the advantages of tcmalloc -- a replacement for malloc and
  new."*

What these pages say together: replacing a C library name by linking a library that defines it is
documented and supported by glibc and by three allocators, and two of the pages make the outcome
depend on **order** (glibc: *"linked in before"*; mimalloc: *"link it as the first object file"*).
Solaris is the one platform read that gives interposition a **declaration** of its own
(`-z interpose`, `INTERPOSE`); none of the allocator pages read mentions one.

### 2. How languages that link C for a user meet it

**2a. Rust's Cargo refuses two packages declaring one native library** (verified, read 2026-11-09).

- [Cargo Book, *The `links` Manifest Key*](https://doc.rust-lang.org/cargo/reference/build-scripts.html):
  *"The `package.links` key may be set in the `Cargo.toml` manifest to declare that the package links
  with the given native library."*; *"Primarily, Cargo requires that there is at most one package
  per `links` value. In other words, it is forbidden to have two packages link to the same native
  library. This helps prevent duplicate symbols between crates."*; *"Common dependencies on
  `foo-sys` alleviates the rule about one package per value of `links`."*
- [Cargo Book, *Dependency Resolution*](https://doc.rust-lang.org/cargo/reference/resolver.html):
  *"The `links` field is used to ensure only one copy of a native library is linked into a
  binary."*; *"The resolver will attempt to find a graph where there is only one instance of each
  `links` name."*; *"If it is unable to find a graph that satisfies that constraint, it will return
  an error."*
- [RFC 0403, *cargo build command*](https://rust-lang.github.io/rfcs/0403-cargo-build-command.html),
  *"Start Date: 2014-10-30"*: *"The key `links` declares that the package will link to and provide
  the given C libraries."*; *"Cargo will impose the restriction that the same C library _must not_
  appear more than once in a dependency graph."* The RFC does not mention duplicate symbols.
- The refusal's words, [Cargo's `src/resolver/errors.rs` on Google's Gitiles
  copy](https://rust.googlesource.com/cargo/+/refs/heads/master/src/resolver/errors.rs) (the
  repository's description *"Cargo downloads your Rust project's dependencies and compiles your
  project."*; [GitHub's `rust-lang/cargo` `src/`](https://github.com/rust-lang/cargo/tree/master/src)
  shows the same `resolver` folder), the `ConflictReason::Links` branch, in pieces: *"` links to the
  native library `"*, *"`, but it conflicts with a previous package which links to `"*, *"` as
  well:\n"*, then the dependency path; *"note: only one package in the dependency graph may specify
  the same links value to ensure that only one copy of a native library is linked in the final
  binary"*; *"for more information, see
  https://doc.rust-lang.org/cargo/reference/resolver.html#links"*; *"help: try to adjust your
  dependencies so that only one package uses the `links = \""*.
- [Rust internals, *Idea to disallow multiple imports of a `-sys` crate*](https://internals.rust-lang.org/t/idea-to-disallow-multiple-imports-of-a-sys-crate/7999),
  2018-07-19, matklad: *"`links` prevents you from having two major versions of a `-sys` crate in
  the crate graph"*, *"it does not prevent you from using `-sys` in several places."*
- So Cargo's rule is about a library's **declared identity**; none of the pages read describes a
  check of which symbols two **different** libraries define.

**2b. Go's cgo** (verified, read 2026-11-09).

- [`cmd/cgo`](https://pkg.go.dev/cmd/cgo): *"All the LDFLAGS directives in any package in the program
  are concatenated and used at link time."*; of `//export`, a preamble holding definitions means
  *"the two output files will produce duplicate symbols and the linker will fail."*
- [golang/go issue 16463, *cmd/go: diamond CGO dependency produces a symbol
  collision*](https://github.com/golang/go/issues/16463), opened by colinmarc 2016-07-22, closed as
  completed: *"duplicate symbol _snappy_compress in:"*; rsc, *"I think the problem is the CGO_LDFLAGS
  being set"*; Ian Lance Taylor, *"I removing .o and .a files from `CGO_LDFLAGS` in
  `builder.collect` makes sense."* (sic); CL 32642. A duplicate the linker refused, repaired by not
  passing one archive twice.

**2c. Zig** (verified, read 2026-11-09). [Ziggit, *Duplicate symbol
definition*](https://ziggit.dev/t/duplicate-symbol-definition/13365), 2025-12-05: Zig's linker
printed *"error: duplicate symbol definition: AES_cbc_encrypt"* with two *"note: defined by"* lines,
each naming `libopenssl.a` and a member; vulpesx: *"Looks like you have two versions of openssl in
your dependency tree."*, and, 2026-03-29, *"zig has not yet taken advantage of semver to deduplicate
dependencies, so your only solution is:"* (aligning the two hashes). A forum thread, cited only for
the message's shape.

**2d. Swift and Xcode** (verified, read 2026-11-09).

- [Swift forums, *Swift Packages in multiple targets results in "This will result in duplication of
  library code." errors*](https://forums.swift.org/t/swift-packages-in-multiple-targets-results-in-this-will-result-in-duplication-of-library-code-errors/34892),
  Jeff Kelley, 2020-03-28, *"When building with Xcode 11.4."*: *"error: Swift package product
  'Alamofire' is linked as a static library by 'EmojiClub' and 'EmojiClubMessages'."*, *"This will
  result in duplication of library code."* A refusal of one library twice, by package identity.
- [Swift forums, *Duplicate symbols - runtime crash with local SwiftPM package
  dependencies*](https://forums.swift.org/t/duplicate-symbols-runtime-crash-with-local-swiftpm-package-dependencies/73991),
  Peter Kaminski, 2024-08-16: a warning *"Class CommonGraphQL19MyMutation is implemented in both"*
  beside a crash *"Could not cast value of type 'Swift.AnyHashable'"*; Boris Buegling: the two
  products *"are linking "CommonGraphQL" statically"*; the reporter's fix, *"marking everything as
  `type: .dynamic` seems to resolve the duplicate symbols issue!"*
- [Apple Developer Forums, *objc: Class MyClass is implemented in both
  dylibs*](https://developer.apple.com/forums/thread/734878), Aug 2023: the runtime's words as the
  question quotes them, *"objc[15078]: Class MyClass is implemented in both
  /path/to/libdynamicLib_A.dylib (0x104f03f90) and /path/to/libdynamicLib_B.dylib (0x10dbf3f48). One
  of the two will be used. Which one is undefined."*; Quinn "The Eskimo!" (DTS): *"Objective-C
  classes live in a single flat namespace."*, *"The best way to avoid this warning is to rework how
  you do your linking, so that you only have one instance of the class."*, *"The alternative is to
  build the static library twice, with different names in each case."*
- [Swift.org, *Wrapping C/C++ Library in Swift*](https://www.swift.org/documentation/articles/wrapping-c-cpp-library-in-swift.html):
  *"Swift can't import header files directly."*; a module map may carry *"additional directives,
  like `link "sqlite3"`"*, *"to indicate to the autolink mechanism that it should automatically link
  against the sqlite3 library."*

**2e. Haskell: GHC's runtime linker refuses, naming both files** (verified, read 2026-11-09).
[`rts/Linker.c`](https://raw.githubusercontent.com/ghc/ghc/master/rts/Linker.c), function
`ghciInsertSymbolTable`, its format string in pieces: *"GHC runtime linker: fatal error: I found a
duplicate definition for symbol\n"*, *"whilst processing object file\n"*, *"The symbol was
previously defined in\n"*, *"This could be caused by:\n"*, *"   * Loading two different object files
which export the same symbol\n"*, *"   * Specifying the same object file twice on the GHCi command
line\n"*, *"   * An incorrect `package.conf' entry, causing some object to be\n"*, *"     loaded
twice.\n"*. Two tickets carry its title: [#11748](https://mail.haskell.org/pipermail/ghc-tickets/2016-April/038281.html),
*"Mon Apr 18 06:50:05 UTC 2016"*, and [#14053](https://mail.haskell.org/pipermail/ghc-tickets/2017-July/055303.html),
*"Runtime linker failed due to a duplicate symbol definition"*. Cabal I did not read.

**2f. Nim and LuaJIT: a name imported from a named library** (verified, read 2026-11-09).

- [Nim manual 0.16.0, FFI, on MIT's copy](https://web.mit.edu/nim-lang_v0.16.0/nim-0.16.0/doc/manual/ffi.txt)
  (an old version; the current manual's section was not reached by the tool): *"With the
  ``dynlib`` pragma a procedure or a variable can be imported from"* *"a dynamic library (``.dll``
  files for Windows, ``lib*.so`` files for UNIX)."*; *"The non-optional argument has to be the name
  of the dynamic library:"*
- [LuaJIT, *ffi.* API](https://luajit.org/ext_ffi_api.html): `ffi.load` *"loads the dynamic library
  given by name and returns a new C library namespace which binds to its symbols."*; `ffi.C` *"binds
  to the default set of symbols or libraries on the target system."*

**2g. Corrections after a reported crash.** No page I read dates a toolchain or language change as
the answer to a crash from one name two libraries define. The nearest is § 2d's Swift thread of
2024, a crash beside a duplicate-class warning, whose fix was the user's own link change. This is a
question, not a finding: the search terms are in § search.

### 3. One record, two layouts in two units (2364)

**3a. What C says** (verified, read 2026-11-09). [N1570](https://port70.net/~nsz/c/c11/n1570.html),
6.2.7p1: *"Moreover, two structure, union, or enumerated types declared in separate translation units
are compatible"* *"if their tags and members satisfy the following requirements"*; 6.2.7p2: *"All
declarations that refer to the same object or function shall have compatible type; otherwise, the
behavior is undefined."* Not a constraint, so by 5.1.1.3 (§ 1a) no diagnostic is owed.

**3b. A macro that must come before every header, by documentation** (verified, read 2026-11-09).
[`feature_test_macros(7)`](https://man7.org/linux/man-pages/man7/feature_test_macros.7.html): *"In
order to be effective, a feature test macro _must be_ _defined before including any header files_."*;
*"The requirement that the macro must be defined before including any header file exists"* *"because
header files may freely include one another."*; `_FILE_OFFSET_BITS=64` *"automatically converts
references to 32-bit functions and data types"* *"related to file I/O and filesystem operations into
references to their 64-bit counterparts."*

**3c. A pack state one header leaves set, warned by two compilers** (verified, read 2026-11-09).

- [MSVC C4103](https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4103),
  level 1: *"'filename' : alignment changed after including header, may be due to missing #pragma
  pack(pop)"*; *"Packing affects the layout of classes, and commonly, if packing changes across header
  files, there can be problems."*; *"Use #pragma pack(pop) before exiting the header file to resolve
  this warning."*
- [Clang r309386, *Recommit r308327 3rd time: Add a warning for
  missing...*](https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20170925/204993.html),
  *"Author: arphaman"*, *"Date: Fri Jul 28 07:41:21 2017"*: *"This commit adds a new -Wpragma-pack
  warning."*; *"When leaving an included file that changed the state of the current alignment
  value."*; texts *"the current #pragma pack aligment value is modified in the included file"* (sic),
  *"non-default #pragma pack value changes the alignment of struct or union members in the included
  file"*, *"unterminated '#pragma pack (push, ...)' at end of file"*; the suspicious-include half is
  `DefaultIgnore`, the modified-after-include and unterminated warnings are not (the tool's reading
  of the diff). Clang's current *Diagnostics Reference* could not be read that far (§ search), so
  whether the group is on by default in clang 21 and 22 is **unverified**.

**3d. MSVC: a configuration that changes layouts, compared at the link** (verified, read 2026-11-09).

- [`detect_mismatch` pragma](https://learn.microsoft.com/en-us/cpp/preprocessor/detect-mismatch):
  *"Places a record in an object. The linker checks these records for potential mismatches."*; *"the
  linker throws a LNK2038 error if the project contains two objects that have the same name but each
  has a different value. Use this pragma to prevent inconsistent object files from linking."*
- [LNK2038](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk2038):
  *"mismatch detected for 'name': value 'value_1' doesn't match value 'value_2' in filename.obj"*;
  *"Visual Studio defines the following symbols to prevent the linking of incompatible code, which
  can cause run-time errors or other unexpected behavior."*; of `_ITERATOR_DEBUG_LEVEL`, *"These
  features can change the representation of certain C++ Standard Library objects and thereby make
  them incompatible with those that use different security and debugging features."*; *"Ensure that
  the values of these symbols are consistent throughout the projects in your Visual Studio
  solution"*.

**3e. Clang modules: one preprocessor state per module, and records compared across modules**
(verified, read 2026-11-09).

- [Clang, *Modules*](https://clang.llvm.org/docs/Modules.html): the problem, *"`#include` directives
  are treated as textual inclusion by the preprocessor"* *"are therefore subject to any active macro
  definitions at the time of inclusion."*; the answer, *"Preprocessor definitions that precede the
  import declaration have no impact on the API provided by `std.io`,"* *"because the module itself
  was compiled as a separate, standalone module."*; `config_macros` *"specifies the set of
  configuration macros that have an effect on the API of the enclosing module."*, *"A translation
  unit shall not import the same module under different definitions of the configuration macros."*,
  a changed one warned *"under the control of `-Wconfig-macros`"*; and *"Clang currently only
  performs minimal checking for violations of the One Definition Rule."*
- [Clang commit, *[ODRHash] Hash `RecordDecl` and diagnose discovered
  mismatches.*](https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20230116/456465.html),
  Volodymyr Sapsai, *"2023-01-19T15:57:48-06:00"*: *"When two modules contain struct/union with the
  same name, check the definitions are equivalent and diagnose if they are not."*; the release note,
  *"Behavior in C and Objective-C language modes now is"* *"the same as in C++."*
- [Clang test `Modules/compare-record.c`](https://llvm.googlesource.com/llvm-project/clang/+/refs/heads/main/test/Modules/compare-record.c):
  *"'CompareFieldPresence1' has different definitions in different modules;"* *"first difference is
  definition in module 'First.Hidden' found field"*, with the note *"but in 'Second' found end of
  class"*.

**3f. Checkers that compare across units** (verified, read 2026-11-09).

- [GCC, *Warning Options*](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html):
  `-Wno-lto-type-mismatch`, *"do not warn about type mismatches in global declarations from different
  compilation units"*, *"Requires -flto to be enabled."*, *"Enabled by default."*; `-Wno-odr`, *"Warn
  about One Definition Rule violations during link-time optimization."*, *"Enabled by default."* The
  part read does not say whether the first applies to C records.
- [AddressSanitizer, *One Definition Rule violation*](https://github.com/google/sanitizers/wiki/AddressSanitizerOneDefinitionRuleViolation):
  *"In various other cases (e.g. when shared libraries are involved) the linker remains silent."*;
  ASan *"is able to detect most of the ODR violations at process startup."*, *"ERROR:
  AddressSanitizer: odr-violation"*; level 2 *"To find even more ODR violations (where the object
  sizes are equal)."*; *"Currently it requires a special run-time switch"*. About globals, not record
  layouts.
- gold's `--detect-odr-violations`, § 1c: C++ by its own words, off by default.

**3g. Binding generators** (verified, read 2026-11-09).

- [`cmd/cgo`](https://pkg.go.dev/cmd/cgo): *"Cgo translates C types into equivalent unexported Go
  types."*; *"Because the translations are unexported, a Go package should not expose C types in its
  exported API:"* *"a C type used in one Go package is different from the same C type used in
  another."*
- [Zig 0.13.0 documentation](https://ziglang.org/documentation/0.13.0/), `@cImport`: *"Usually you
  should only have one `@cImport` in your entire application"* *"because it saves the compiler from
  invoking clang multiple times"* and *"prevents inline functions from being duplicated."*; reasons
  for more than one: *"To avoid a symbol collision, for example if foo.h and bar.h both `#define
  CONNECTION_COUNT`"* and *"To analyze the C code with different preprocessor defines"*. The master
  documentation read the same day holds no `@cImport` section in the parts read; whether it was
  removed is **unverified**.
- [zig.guide, *cImport*](https://zig.guide/working-with-c/c-import), *"Version: Zig 0.15.2"*, a
  community guide: *"the types generated within one cImport will not be equivalent to those
  generated in another."*
- [bindgen, `Builder`](https://docs.rs/bindgen/latest/bindgen/struct.Builder.html), `layout_tests`:
  *"Set whether layout tests should be generated."*, *"Layout tests are generated by default."* What
  a layout test asserts the page does not say (size and alignment in generated code seen in search
  results only; offsets **unverified**).

**3h. D's ImportC: C files compiled together, order-dependent, filed as bugs and fixed** (verified,
read 2026-11-09, through the bug tracker's mailing-list copies; `issues.dlang.org` and its archive
answered HTTP 522).

- [Issue 24280, *ImportC: forward reference error when compiling multiple
  files*](https://lists.puremagic.com/pipermail/digitalmars-d-bugs/2023-December/134415.html),
  2023-12-12, reported by dkorpel: *"I have two single-file c libraries, and dmd can compile either
  one just fine,"* *"but not at the same time."*; *"dmd b.c a.c // works"*; *"dmd a.c b.c //
  fails:"*, *"b.c(13): Error: forward reference to type `timespec`"*.
- [Walter Bright's comment](https://lists.puremagic.com/pipermail/digitalmars-d-bugs/2023-December/134576.html),
  2023-12-26: *"The source of the trouble is dmd assigns each type a unique "deco" string."*, *"C
  structs defined in different .c files are considered to be the same if they have the same "deco"
  name."*; [the fix](https://lists.puremagic.com/pipermail/digitalmars-d-bugs/2023-December/134585.html),
  2023-12-27: *"dlang/dmd pull request #15954"*, commit *"e590fba9d81c6060f806a52c11c030161780484d by
  Walter Bright"*, status *"RESOLVED"*, *"FIXED"*.
- [Issue 24306, *ImportC: same name structs in separate C files interfere when compiled
  together*](https://lists.puremagic.com/pipermail/digitalmars-d-bugs/2023-December/134617.html),
  2023-12-28: *"dmd a.c // works"*, *"dmd b.c // works"*, *"dmd a.c b.c // fails with:"*, *"Error:
  struct `a.S` unknown size"*.
- [Issue 23387](https://digitalmars.com/d/archives/digitalmars/D/bugs/Issue_23387_New_ImportC_identical_structs_defined_in_two_C_files_130613.html),
  Oct 04 2022: *"ImportC: identical structs defined in two C files lead to duplicate .init symbol on
  macOS"*.
- [digitalmars.D, *ImportC: On the issue of using a type from two different C
  files*](https://lists.puremagic.com/pipermail/digitalmars-d/2022-August/329700.html), TheGag96,
  2022-08-27: *"a type imported from two different .c files aren't compatible with each other."*

### 4. Error messages: what each names and what it tells the user to do

All from the pages above, verified 2026-11-09.

| tool | names | tells the user |
|---|---|---|
| GHC runtime linker (§ 2e) | the symbol, the object being loaded, the object that defined it first | three causes, *"Loading two different object files which export the same symbol"* first |
| Objective-C runtime (§ 2d) | the class, both images and addresses | *"One of the two will be used. Which one is undefined."*; Apple DTS: one instance, or build the library twice *"with different names"* |
| MSVC LNK2005 (§ 1f) | the symbol, both libraries and members (*"oledb.lib(oledb_i.obj) ... already defined in uuid.lib(go7.obj)"*) | one library first; or *"wrap each library in a unique namespace"*, or *"a new wrapper library that uses unique names"* |
| lld (§ 1d) | the symbol, each definition's source line, object and archive | nothing beyond the locations |
| Zig's linker (§ 2c) | the symbol, each definer as archive and member | nothing beyond the definers |
| Cargo (§ 2a) | the package, the library, the earlier package, the path between | a `note:` saying why, a link, and a `help:` to adjust the dependencies so that one package uses the `links` value |
| Solaris guide (§ 1g) | (a guide, not a message) | *"Localize symbol instances"*, *"Remove the multiple instances"*, *"Define any interposition requirement explicitly."* |
| Clang modules (§ 3e) | the record, both modules, the first differing member | nothing beyond the difference |
| MSVC LNK2038 (§ 3d) | the key, both values, the object | *"Ensure that the values of these symbols are consistent"* |
| MSVC C4103 (§ 3c) | the header that left the state set | *"Use #pragma pack(pop) before exiting the header file"* |

## argument

C makes two definitions of one used name undefined behaviour with no diagnostic owed (6.9p5 under
Semantics; 4p2; 5.1.1.3). Every linker read binds the first definition and refuses only when two
loaded members collide; lld's own page calls two links succeeding with different archives'
definitions "even worse" than one failing. Route (A) alone inherits that order. The order-free
precedents bind or report: Apple's two-level namespace and Solaris direct binding tie a reference to
its library, Solaris's `lari` reports multiple symbol definitions, Cargo refuses by declared
library. Deliberate replacement is documented and order-dependent (glibc, mimalloc); Solaris
declares interposition explicitly (`-z interpose`). For 2364 the precedents compare layouts
(bindgen, Clang 2023, LNK2038) or isolate (Clang modules); I found none that searches orders.

## condition

- **For 2363's (A) alone**: a language or toolchain that binds a foreign name by reading which
  library the link took, with the link's order left free, and reports no order-dependent outcome
  over years of use. lld's page (§ 1d) is the opposite reading; one such page would make (A) alone
  defensible on precedent.
- **Against an order-free listing (B, D)**: a toolchain that refused by default a name two
  **archives** define where one member alone loads, and withdrew the refusal for false positives
  (glibc's or the platform's own pairs). I found no toolchain that refuses it by default (§ 1b to
  § 1g; AIX's warning is unverified); one that tried and retreated would argue for the narrower (A)
  joined to (C).
- **On the deliberate replacement**: a platform other than Solaris that declares interposition by a
  word of the link (rather than by order or `LD_PRELOAD`) would strengthen a Heroes word for it; an
  allocator documented as replaceable **only** by order, on a platform where order is not a lever,
  would argue for telling it by its symbol set (glibc's minimum four) instead.
- **For 2364's (A)**: a compiler or binding generator that searches the order of headers inside one
  translation unit to make it compile, shipped and kept. D's ImportC met order-dependent readings
  and repaired the cause (§ 3h); Clang chose isolation (§ 3e). One such page would make panel 238's
  direction a precedent rather than a first.

## search

How the search was done: one query or fetch per claim, every precedent read on its own page, the
tool asked for verbatim quotations; where a page was long, read at successive offsets until the
section was found.

- **C**: `port70.net/~nsz/c/c11/n1570.html` at offsets 0 (twice: 4p2; then 5.1.1.3 and the title),
  100000 (6.2.7), 300000 (6.9p5), 400000 (not there: 6.9.2's tail onward).
- **Linkers**: GNU ld's *Options* page (offsets 0, 100000, 110000); Debian's `ld.gold(1)`; lld's
  `warn_backrefs.html`, `NewLLD.html`, the 14.0.0 release notes, the r299280 commit mail; MaskRay's
  blog; Apple's *Executing Mach-O Files*; the `ld(1)` mirror and its index; Microsoft's LNK2005,
  LNK4006, LNK2038, C4103, `detect_mismatch`, and the PE format (offsets 0, 55000, 100000); the gABI
  at `sco.com` (the `refspecs.linuxbase.org` copy answered with an access-denied page); Oracle's
  Solaris 11.3 page (twice) and two 11.4 pages; IBM's AIX 7.3 `ld` (twice). Queries: *gold linker
  "--detect-odr-violations" ...*; *Xcode 15 release notes new linker "duplicate libraries" warning
  ld* (its results concern one library named twice and are not cited); *Oracle Solaris Linker and
  Libraries Guide "direct bindings" ...*; *Solaris lari(1) ...*; *AIX ld "0711-224 WARNING:
  Duplicate symbol" default warning*; *lld release notes "--why-extract" ...*; *lld ELF "duplicate
  symbol:" ">>> defined at" ...*.
- **Allocators**: glibc's *Replacing malloc* (the `gnu.org` URL redirects to sourceware.org), the
  mimalloc README, jemalloc's *Getting Started*, the gperftools README.
- **Languages**: Cargo's build-scripts and resolver pages, RFC 0403, the internals thread, Cargo's
  `src/resolver/errors.rs` and `mod.rs` and the repository's front page on Gitiles
  (`raw.githubusercontent.com` and `doc.rust-lang.org/nightly-rustc` answered 404 for the old
  `src/cargo/core/resolver` path, and GitHub's `src/cargo/core` listing 404; GitHub's `src/` listing
  read to confirm the new one); `cmd/cgo`, golang/go 16463; Ziggit's thread; the two Swift forum
  threads, Apple's forum thread 734878 (the objc4 test file at `git.saurik.com` answered 503 and is
  not cited), Swift.org's wrapping article; GHC's `rts/Linker.c` and three ticket mails (GitLab
  answered with an access-denied page); Nim's current manual at five offsets (the `dynlib` section
  never reached) and its 0.16.0 copy at MIT; LuaJIT's FFI API. Queries: *golang/go issue cgo
  "duplicate symbol" two packages ...*; *rust-lang issue two -sys crates bundle same C library
  ...*; *cargo error "Only one package in the dependency graph may specify the same links value"
  ...*; *GHC runtime linker "I found a duplicate definition for symbol" ...*; *Swift Package
  Manager duplicate symbols two packages same C library ...*; *objc4 source "is implemented in
  both" ...*; *zig linker "duplicate symbol definition" ...*; *"With the dynlib pragma, a procedure
  or a variable can be imported from a dynamic library" nim manual*.
- **2364**: `feature_test_macros(7)`; Clang's *Modules* page (twice); Clang's *Diagnostics Reference*
  at offsets 0, 200000, 235000, 262000, 275000 (the tool's view stopped inside `-Wodr` every time,
  so `-Wpragma-pack` was read in its commit instead); the 2017 and 2023 commit mails and the r295293
  revert mail (read, not cited); `compare-record.c`; GCC's *Warning Options* (offsets 0, 100000); the
  ASan wiki; Zig's master documentation (offsets 0, 330000, 430000) and 0.13.0 (offsets 280000,
  380000); zig.guide; bindgen's book front page and `Builder`; D's mailing-list copies and the
  digitalmars archive. Queries: *clang "-Wpragma-pack" "modified in the included file" ...*;
  *"Usually you should only have one @cImport in your entire application"*; *bindgen layout tests
  ...*; *clang modules error "has different definitions in different modules" ...*; *clang release
  notes modules ODR checking C structs ...*; *D language ImportC "same struct" two C files ...*;
  *"Issue 24280" "New:" ImportC ...*.
- **Looked for and not found, as questions**: a toolchain that refuses by default one name two
  archives define where only one member loads (GNU ld, gold, lld, ld64's mirror, `link.exe`, AIX's
  page: none says so; AIX's `0711-224` warning only in search results); a page dating a toolchain or
  language change as a correction after a crash from such a name (none among the pages above); a
  compiler or binding generator that searches header orders within one unit (Clang modules, GCC,
  Zig, cgo, bindgen, Swift, D: none of the pages read does); whether GCC's `-Wlto-type-mismatch`
  covers C records (the entry read does not say); whether bindgen's layout tests assert offsets (not
  on the pages read); whether clang 21 and 22 keep `-Wpragma-pack` on by default (the reference page
  not reached); Zig master's `@cImport` (not in the parts read); Nim's current `dynlib` text (not
  reached); an Apple-hosted `ld(1)` (not searched for; the mirror is cited as a mirror); what
  `lld-link` reports for one name two `.lib` files define (no page read; a question for the
  ffi-pragmatist).
- Also read, not cited: panel 238's historian report, to avoid repeating its prior art (it covers
  formatters and C's include-order breakages).

## Closing items (`00-shared.md` § Rules)

- **Verdict**: object (advisory) to both provisional directions taken alone; approve the order-free
  criterion (§ verdict).
- **design.md section**: §1.12, robustness (*"a Heroes program must not segfault and must not corrupt
  memory"*, as CLAUDE.md § Precedence states it), which 2363's `sig_sorted` and 2364's three silent
  members break; and §4.17 (*"a diagnostic carries everything needed to fix the mistake it names
  without opening another file"*, CLAUDE.md § 8), against which § 4's table is the precedent. I did
  not re-read design.md this session; both sections are cited as CLAUDE.md cites them.
- **Cost or delta measured**: none on the tree; I ran no command there. What I counted, by hand from
  this session's own tool calls: **104 fetches**, **91** returning a page and **13** not (five 404s,
  four 522s, one 503, two access-denied pages, one redirect not followed); **68 distinct pages**
  read, of which those linked above are cited; **22 web searches**.
- **One falsifiable prediction**: clang's `-Wpragma-pack` sees exactly **one** of 2364's three
  silent members. Compiling the root unit's C of `repro/critic/s2364x/pragma/pk_first`, as Heroes
  emits it, with clang's default warnings (no `-w`) prints a warning tagged `[-Wpragma-pack]` for
  `pk.h` (on this Mac's Apple clang 21.0.0 and the container's Debian clang 22.1.8); the root units
  of `repro/s2364/pack/pk_first` (a balanced `push`/`pop` under a macro) and
  `repro/critic/s2364x/partial/pw_first` (no pragma) print none. Scored by one clang run per unit,
  by the compiler-engineer seat or the lane; falsified if `pragma`'s unit is silent or either of the
  others warns. If it holds, an existing clang warning closes one member at no cost, and the other
  two still need a layout comparison or isolation (§ 3d, § 3e).
- **The condition that would change the verdict**: § condition above, its first and last bullets
  foremost.

## Candidate prior-art lines

For the decision issue's `## Prior art`, its first line naming `docs/panel/239-*.md`, this report
and the day the links are re-read. Each line quotes its page as read 2026-11-09 and says only what
the page states. Which form a line takes is the synthesis's call by what the resolution rests on: I
propose **Taken from** where a rule the sitting may adopt would rest on that page's evidence (C's
rules, the deliberate replacement as the correct program, an explicit replacement word, the header
warnings, the fix by unique names), and **Reached first by** where the sitting reached its finding
by its own measurement (the repros) and found it already there. A line whose rule the sitting does
not adopt is dropped, not rewritten.

**2363**

- **Taken from C (ISO/IEC 9899:2011, draft N1570)**: [6.9p5](https://port70.net/~nsz/c/c11/n1570.html): an identifier with external linkage that is used has exactly one external definition in the whole program. *"somewhere in the entire program there shall be exactly one external definition for the identifier;"*
- **Taken from glibc**: [*Replacing malloc*](https://sourceware.org/glibc/manual/latest/html_node/Replacing-malloc.html): replacing `malloc` by linking another allocator is supported, through interposition when dynamic and by linking it before `libc.a` when static. *"The GNU C Library supports replacing the built-in `malloc` implementation with a different allocator"*
- **Taken from mimalloc**: [README](https://github.com/microsoft/mimalloc): a static override is made by linking the allocator first. *"To ensure that the standard malloc interface resolves to the _mimalloc_ library, link it as the first object file."*
- **Taken from Oracle Solaris**: [*Defining Explicit Interposition*](https://docs.oracle.com/en/operating-systems/solaris/oracle-solaris/11.4/linkers-libraries/defining-explicit-interposition.html): interposition can be declared explicitly to the link-editor. *"With the link-editors `-z interpose` option."*
- **Taken from Microsoft (LNK2005)**: [*Linker Tools Error LNK2005*](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk2005): a program needing two libraries that define one symbol distinguishes them by unique names. *"To use both symbols, you must create a way to distinguish them."*
- **Reached first by LLVM (lld)**: [*--warn-backrefs*](https://lld.llvm.org/ELF/warn_backrefs.html): two linkers can both succeed while taking different definitions of one symbol from different archives. *"or even worse both links succeed but they have selected different objects from different archives"*
- **Reached first by Apple (two-level namespace)**: [*Executing Mach-O Files*](https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/MachOTopics/1-Articles/executing_files.html): a reference to an imported symbol records the library that contains it. *"it records a reference to the name of the library that contains the symbol and the name of the symbol."*
- **Reached first by Oracle Solaris (direct bindings)**: [*Direct Bindings and Interposition*](https://docs.oracle.com/cd/E53394_01/html/E54813/gejgf.html): a directly bound reference is looked up in the dependency it is associated with, bypassing first-found interposition. *"As the directly bound reference is searched for in the dependency associated with the reference, the default symbol search model that enables interposition, is bypassed."*
- **Reached first by Oracle Solaris (`lari`)**: [*Direct Bindings and Interposition*](https://docs.oracle.com/cd/E53394_01/html/E54813/gejgf.html): a tool reports multiple instances of a symbol definition in an application. *"This information originates from multiple instances of a symbol definition, which in turn can lead to interposition."*
- **Reached first by Rust (Cargo)**: [*The `links` Manifest Key*](https://doc.rust-lang.org/cargo/reference/build-scripts.html): two packages may not declare the same native library. *"In other words, it is forbidden to have two packages link to the same native library. This helps prevent duplicate symbols between crates."*
- **Reached first by Haskell (GHC)**: [`rts/Linker.c`](https://raw.githubusercontent.com/ghc/ghc/master/rts/Linker.c): a symbol defined twice is a fatal error naming the object being loaded and the one that defined it first. *"GHC runtime linker: fatal error: I found a duplicate definition for symbol"*
- **Reached first by Apple (Objective-C runtime)**: [Apple Developer Forums 734878](https://developer.apple.com/forums/thread/734878): one class defined in two loaded images is told at run time, the choice undefined. *"One of the two will be used. Which one is undefined."*

**2364**

- **Taken from C (ISO/IEC 9899:2011, draft N1570)**: [6.2.7p2](https://port70.net/~nsz/c/c11/n1570.html): declarations that refer to one object or function have compatible types. *"All declarations that refer to the same object or function shall have compatible type; otherwise, the behavior is undefined."*
- **Taken from Linux man-pages**: [`feature_test_macros(7)`](https://man7.org/linux/man-pages/man7/feature_test_macros.7.html): a feature test macro is defined before every header. *"In order to be effective, a feature test macro _must be_ _defined before including any header files_."*
- **Taken from Microsoft (C4103)**: [*Compiler Warning (level 1) C4103*](https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4103): a header that changes packing and does not restore it is warned. *"alignment changed after including header, may be due to missing #pragma pack(pop)"*
- **Taken from Clang (`-Wpragma-pack`)**: [r309386](https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20170925/204993.html): leaving an included file that changed the packing state is warned. *"When leaving an included file that changed the state of the current alignment value."*
- **Reached first by Clang (modules)**: [*Modules*](https://clang.llvm.org/docs/Modules.html): a module is compiled on its own, so macros defined before its import do not change it. *"because the module itself was compiled as a separate, standalone module."*
- **Reached first by Clang (ODRHash for C records)**: [*[ODRHash] Hash `RecordDecl` and diagnose discovered mismatches.*](https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20230116/456465.html): a C struct or union two modules define is compared and a difference diagnosed. *"When two modules contain struct/union with the same name, check the definitions are equivalent and diagnose if they are not."*
- **Reached first by Microsoft (`detect_mismatch`)**: [*detect_mismatch pragma*](https://learn.microsoft.com/en-us/cpp/preprocessor/detect-mismatch): objects carrying one key with two values are refused at the link. *"Use this pragma to prevent inconsistent object files from linking."*
- **Reached first by Rust (bindgen)**: [`Builder::layout_tests`](https://docs.rs/bindgen/latest/bindgen/struct.Builder.html): generated bindings carry layout tests by default. *"Layout tests are generated by default."*
- **Reached first by Go (cgo)**: [`cmd/cgo`](https://pkg.go.dev/cmd/cgo): a C type is a different type in each Go package that uses it. *"a C type used in one Go package is different from the same C type used in another."*
- **Reached first by Zig (`@cImport`)**: [Zig 0.13.0 documentation](https://ziglang.org/documentation/0.13.0/): one header's macro colliding with another header's is a reason for separate imports. *"To avoid a symbol collision, for example if foo.h and bar.h both `#define CONNECTION_COUNT`"*
- **Reached first by D (ImportC)**: [Issue 24280](https://lists.puremagic.com/pipermail/digitalmars-d-bugs/2023-December/134415.html): two C files that each compiled alone failed together in one order and compiled in the other, reported as a bug. *"dmd a.c b.c // fails:"*

Not proposed: gold's and GCC's ODR options and ASan's check (C++ or globals by their own words, so a
comparison, not a borrowing); the ld64 mirror (not Apple's page); the Zig and Swift forum threads (a
message's shape, not a rule); Nim and LuaJIT (dynamic loading by name, a different binding model the
sitting compares itself with at most); the PE format (true of DLL imports, not of static `.lib`
resolution); Cargo's error text (a message's shape, read on a Gitiles copy).
