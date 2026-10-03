# Panel 188, the historian's report

Seat: historian (advisory, no veto; charter `.claude/agents/historian.md`).
Written 2026-10-03 as I went; this is its final form. **Every source below
was read on 2026-10-03.**

**How the sources were read.** I read the C drafts as page images of the PDFs
themselves (the fetch tool saved them locally), so their quotations are the
firmest evidence here. Everything else came through the fetch tool, which
returns its own extraction. On its first attempt that tool **invented a
quotation of 6.4.7**: a "paragraph 3" that appears in no draft. A second pass
showed the page it had read was cut off before 6.4.7. I discarded that text
and it appears nowhere below. Source code was read from raw files. Where I say
what a compiler *does* on the strength of its source, the sentence says **read
in source, unrun**. The author's book was not used: no claim below rests on
it.

**What I built and ran, and my cost.** I built and ran nothing. This seat's
tools are Read, Write, WebSearch and WebFetch, with no shell, so I made no
copy of the tree and started no process, container or paid session. The cost
was about eighty web searches and fetches, plus PDF page reads.

## verdict

**approve (advisory): (1h), in the form (1f) + (1a) + (1b), with every
refusal judged on the string's DECODED value.** Go's type checker has used
this exact shape for import paths: decode the literal, then refuse named
characters in the value. It keeps a Heroes program out of C's undefined
behaviour and away from a platform's accident, without the widening history
that positive rules carry.

Per route:

- **(1a) approve.** C's own grammar already excludes these: `h-char` is *"any
  member of the source character set except the new-line character and `>`"*,
  and `h-char-sequence` has no empty alternative (N1570 6.4.7p1). Precedent
  for a front end refusing them: gc refuses an empty import path, a NUL and
  every control character; Rust refuses an empty link name (E0454). **A shape
  beside it:** gc refuses *every* control character (`r < 0x20 || r ==
  0x7f`), and F7 shows `\t` and `\r` fail the same way `\n` does. So (1a)'s
  "a line end" should read "a control character".
- **(1b) approve.** These five are C's own list, unchanged from C11 through
  the C2y working draft of 2026-01-25. C++ allows an implementation to make
  them "an error" (CWG 787). GCC and clang lex a `\` inside `<...>`
  differently (read in source, unrun). MSVC treats `\` as a separator, while
  GCC treats it as an ordinary character. go/types already refuses four of the
  five in a file-naming string: `'`, `"`, `\`, and `*` (which covers `/*`).
  The cost is F1's working rows for `'`, `//` and `/*`; no precedent I found
  obliges anyone to keep them.
- **(1c) object (advisory) as the primary rule.** Every positive rule I found
  was either narrower than real use or had to be widened repeatedly:
  - C's own guaranteed set (6.10.2p5) does not even include `/`.
  - Haskell 2010's `chname` grammar admits no digit (`chchar → letter |
    ascSymbol⟨&⟩`, with `letter → ascSmall | ascLarge | _`), so `sqlite3.h`
    is not a `chname` by the letter of the Report. GHC did not hold programs
    to it.
  - go/build's character allowlist was widened five times between 2013 and
    2019, by its own source comment's count. At least four of those widenings
    followed an issue from a user who had been refused.
- **(1d) object, as the whole answer.** It is the majority practice: Zig,
  Nim, Cython, Vala, Swift's bridging header and cffi check nothing in the
  code or documentation I read. But those tools treat the string as C text
  (Nim even emits a string starting with `#` verbatim as a preprocessor
  line). Heroes deliberately does not (panel 036's R2). (1d) alone leaves
  F6's `"stdio.h>"` accepted, much as GHC accepted `"some.rubbish"`.
- **(1f) approve.** GCC and MSVC both document that escapes are not decoded
  inside a header name, and MSVC prints the trap side by side: one backslash
  in an `#include`, two in a string literal. So the front end is the only
  place a Heroes string's escapes can be decoded. Go decodes before it checks.
  clang's module-map strings were decoded in LLVM 15 and are no longer
  decoded since a 2025 refactor, which shows a case must pin the decoded
  meaning.
- **(1g) no objection, not needed.** I found no precedent that refuses an
  escape as such; Go decodes and then judges the value. The escapes F1 and F7
  exercise (`\\`, `\"`, `\n`, `\t`, `\r`) all decode to a character that
  (1b) or a widened (1a) refuses, so for those (1g) changes only the wording
  of the message. Whether spec § 2's five escapes are exactly these I did not
  read: this seat stays out of the trunk beyond its report.
- **(1h) approve**, in the form above.
- **(1i) object (advisory).** bindgen does pass headers this way, but GCC's
  `-include` searches *"the preprocessor's working directory"* first and then
  the quoted chain. clang implements it by writing `#include "` + File + `"`
  into its predefines buffer, unescaped. So (1i) moves the class from `>` to
  `"` and brings back the quoted-form decoy that panel 036's R2 refused.
- **(1e) two unlisted routes have precedent; I recommend neither.**
  - **GHC's route:** stop including the header. GHC included it up to 6.8.3
    and then stopped, giving up, in its own words, the C compiler's check
    that "the C function being called via the FFI was being called at the
    right type".
  - **The C-file route** (Zig 0.16's `addTranslateC`, cgo's preamble, cffi's
    `set_source`, D's ImportC): the user writes the `#include` lines in C,
    and the language names the C side by an identifier or a file. For Heroes
    this would mean a second input class.

Positions on Q2 to Q5 are under *What it means for Q1 to Q5*, below.

## precedents

### Question 1. C's header-name rule (verified, primary documents)

**C11, N1570** (WG14, *Committee Draft, April 12, 2011*, ISO/IEC 9899:201x),
<https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf>. Subclause
**6.4.7** *Header names* is on printed pages 73 to 74 (PDF pages 91 to 92):

- **Paragraph 1 (Syntax)**: `h-char`: *"any member of the source character
  set except the new-line character and `>`"*; `q-char`: *"any member of the
  source character set except the new-line character and `"`"*.
  `h-char-sequence` is `h-char` or `h-char-sequence h-char`, so it has no
  empty alternative.
- **Paragraph 2**: *"The sequences in both forms of header names are mapped
  in an implementation-defined manner to headers or external source file
  names as specified in 6.10.2."*
- **Paragraph 3**, the one the brief asks for, verbatim: *"If the characters
  `'`, `\`, `"`, `//`, or `/*` occur in the sequence between the `<` and `>`
  delimiters, the behavior is undefined. Similarly, if the characters `'`,
  `\`, `//`, or `/*` occur in the sequence between the `"` delimiters, the
  behavior is undefined.81) Header name preprocessing tokens are recognized
  only within `#include` preprocessing directives and in implementation-defined
  locations within `#pragma` directives.82)"*
- **Footnote 81**: *"Thus, sequences of characters that resemble escape
  sequences cause undefined behavior."*

The same document, **6.10.2** *Source file inclusion*, printed pages 164 to 165:

- **Paragraph 1 (Constraints)**: *"A `#include` directive shall identify a
  header or source file that can be processed by the implementation."*
- **Paragraph 2**: *"How the places are specified or the header identified is
  implementation-defined."*
- **Paragraph 3**: a failed quoted search is reprocessed as an angled one
  *"with the identical contained sequence (including `>` characters, if any)
  from the original directive."* So C itself contemplates a `>` that only the
  quoted form can carry.
- **Paragraph 5**, the only names C guarantees: *"The implementation shall
  provide unique mappings for sequences consisting of one or more nondigits or
  digits (6.4.2.1) followed by a period (`.`) and a single nondigit. The first
  character shall not be a digit. The implementation may ignore distinctions
  of alphabetical case and restrict the mapping to eight significant
  characters before the period."*

The same document, **5.1.1.2** *Translation phases* (printed pages 10 to 11),
and **5.2.1** *Character sets* (printed pages 22 to 23):

- Phase 1 replaces trigraph sequences and phase 2 deletes *"each instance of
  a backslash character (`\`) immediately followed by a new-line character"*.
  Both happen before phase 3 forms the header name. 5.2.1.1 says trigraphs are
  replaced *"Before any other processing takes place"*; this is what defect
  207's line splice works around.
- 5.2.1p2: *"A byte with all bits set to 0, called the null character, shall
  exist in the basic execution character set"*. 5.2.1p3's list of the members
  of the basic **source** set does not include it.

**C17.** N2310 (WG14, *diff marks, November 6, 2018*),
<https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2310.pdf>, says on its
abstract page: *"Changes from the previous draft (ISO/IEC 9899:2018) are
indicated by striking out text that has been deleted and underlining text that
has been added."* Its 6.4.7, on printed page 53 (PDF 66), carries **paragraph 3
word for word as N1570 has it, with no diff mark on the page**; only the
footnotes are renumbered (83 and 84).

Caveat, verified: N2310 is **not C17's own draft**. Wikipedia,
<https://en.wikipedia.org/wiki/C17_(C_standard_revision)>, lists *"N2176
(final draft of C17 standard); WG14; 2017-10-09"* and *"N2310 (post-C17, very
early draft of what would become C23; WG14; 2018-11-11"*. So "C17 left it
unchanged" is read from N2310's unmarked page against its stated diff
convention. It is not a reading of N2176, which I could not fetch (the web
archive refused the tool): **unverified directly**.

**C23.** N3096 (*working draft, April 1, 2023*, ISO/IEC 9899:2023 (E)),
<https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf>, 6.4.7 on printed
page 69 (PDF 88). **The two undefined-behaviour sentences of paragraph 3 are
unchanged.** Only the sentence after them changed: *"Header name
preprocessing tokens are recognized only within `#include` and `#embed`
preprocessing directives, in `__has_include` and `__has_embed` expressions, as
well as in implementation-defined locations within `#pragma` directives."*
Footnote 92 keeps *"Thus, sequences of characters that resemble escape
sequences cause undefined behavior."* The syntax is unchanged.

Separately, N3096's 5.2.1.1 is *Multibyte characters* and there is no trigraph
subclause (printed pages 18 to 19), so C23 has no trigraphs. The paper behind
this is N2940, *Removing trigraphs* (Robert C. Seacord, 2022-03-02); its
author and date come from a search result, not from the paper. Heroes emits
C11, so defect 207's splice is still needed.

**C2y.** N3783 (*ISO/IEC 9899:202y (en), N3783 working draft*; dated
2026-01-25 in the WG14 document log,
<https://www.open-std.org/jtc1/sc22/wg14/www/wg14_document_log>),
<https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3783.pdf>:

- Header names is **renumbered 6.4.8** (printed page 70, PDF 87), and its
  paragraph 3 still reads *"If the characters `'`, `\`, `"`, `//`, or `/*`
  occur in the sequence between the `<` and `>` delimiters, the behavior is
  undefined."* Source file inclusion is now 6.10.3.
- Its abstract lists every paper integrated through the 2026 February meeting
  (PDF pages 1 to 3). They include *Slay Some Earthly Demons* I to XVII, but
  no paper on header names.
- Two later papers by Ryan Karl target header names, but by their titles they
  concern invalid multibyte characters:
  - N3572 (2025-08-17): *"Slay Some Earthly Demons XX: Remove undefined
    behavior if an identifier, comment, string literal, character constant,
    or header name contains an invalid multibyte character or does not begin
    and end in the initial shift state exceptions"* (title only, body
    unread).
  - N3808, read (dated 2026-02-20 on the paper, posted 2026-03-08 by the
    log): *"We propose making such occurrences constraint violations so
    compilers must diagnose them."* That is C2y's direction for lexical
    undefined behaviour, and it is the direction a refusal at `check` takes.
    It does not touch the five characters.

**C++, the neighbour that did change it.** The C++ draft's source,
<https://raw.githubusercontent.com/cplusplus/draft/main/source/lex.tex>,
[lex.header] paragraph 2: *"The appearance of either of the characters `'` or
`\` or of either of the character sequences `/*` or `//` in a q-char-sequence
or an h-char-sequence is conditionally-supported with implementation-defined
semantics, as is the appearance of the character `"` in an h-char-sequence."*
Its note: *"Thus, a sequence of characters that resembles an escape sequence
can result in an error, be interpreted as the character corresponding to the
escape sequence, or have a completely different meaning, depending on the
implementation."* The change from undefined is CWG issue 787, *"Unnecessary
lexical undefined behavior"*, submitted by the UK on 3 March 2009, status CD2,
<https://cplusplus.github.io/CWG/issues/787.html>.

### Question 2. What the compilers document

**GCC** documents both halves. The Include Syntax node,
<https://gcc.gnu.org/onlinedocs/cpp/Include-Syntax.html>, was checked against
its texinfo source,
<https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/doc/cpp.texi>:
*"However, if backslashes occur within file, they are considered ordinary text
characters, not escape characters. None of the character escape sequences
appropriate to string constants in C are processed. Thus, `#include
"x\n\\y"` specifies a filename containing three backslashes. (Some systems
interpret `\` as a pathname separator. All of these also interpret `/` the
same way. It is most portable to use only `/`.)"* Also: *"`#include <x/*y>`
specifies inclusion of a system header file named `x/*y`."* I found no
sentence on that page about `"`, `>` or a line end in a name.

The lexer, `lex_string` in
<https://raw.githubusercontent.com/gcc-mirror/gcc/master/libcpp/lex.cc> (read
in source, unrun):

- *"In #include-style directives, terminators are not escapable."*
- A line end inside `<...>` turns the `<` into a plain token: *"Unmatched
  quotes always yield undefined behavior, but greedy lexing means that what
  appears to be an unterminated header name may actually be a legitimate
  sequence of tokens."*
- A NUL draws *"null character(s) preserved in literal"*.

**MSVC** reads `\` as a separator and says escapes are not decoded. Its page
on the C mapping of source file character sequences (ANSI 3.8.2),
<https://learn.microsoft.com/en-us/cpp/c-language/character-sequences>:
*"Preprocessor statements use the same character set as source file
statements with the exception that escape sequences are not supported. Thus,
to specify a path for an include file, use only one backslash: `#include
"path1\path2\myfile"` Within source code, two backslashes are necessary: `fil
= fopen( "path1\\path2\\myfile", "rt" );`"*

Its `#include` page,
<https://learn.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp>,
says *"The syntax of the path-spec depends on the operating system on which
the program is compiled"*, and its example path is
`F:\MSVC\SPECIAL\INCL\TEST.H`. Neither page says anything about `"`, `>` or a
line end.

**clang**: I found no user documentation on these characters. What I found:

- **Its lexer.** `Lexer::LexAngledStringLiteral`,
  <https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/Lex/Lexer.cpp>
  (read in source, unrun), lexes `<...>` with *"Skip escaped characters"*: a
  `\` stops the next character from ending the name. A line end makes it
  give up and *"let the caller lex the '<' normally"*. A NUL is kept, with
  *"null character(s) preserved in string literal"*.
- **So GCC and clang disagree on `#include <a\>b.h>`**: GCC's name ends at
  the first `>` and clang's at the second (read in source, unrun).
- **Its messages**, from
  <https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/DiagnosticLexKinds.td>:
  `"empty filename"`, `"expected \"FILENAME\" or <FILENAME>"`, `"extra tokens
  at end of #%0 directive"`, `"'%0' file not found"`.
- **A new warning for backslashes.** `-Wnonportable-include-path-separator`
  came from PR #186770 by Rose Hudson (Sony), merged 2026-04-07,
  <https://github.com/llvm/llvm-project/pull/186770>, commit
  <https://llvm.googlesource.com/llvm-project/+/9a1860c45a915328b392fa1af0bbc75dbe96af7f>:
  - The PR says: *"Emit a warning when #include paths contain backslashes,
    with a fixit to convert them all to '/'. [...] The warning is off by
    default due to being noisy and not always desirable."*
  - The diagnostic reads *"non-portable path to file '%0'; specified path
    contains backslashes"*.
  - The added code (as fetched) checks only that the written name contains a
    `\`. Its test opens `// REQUIRES: system-windows`.
  - A revert (PR #190975, <https://github.com/llvm/llvm-project/pull/190975>)
    was closed without merging on 2026-04-09. A performance fix replaced it
    (`0ab4d85`, #191148, 2026-04-09,
    <https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20260406/832335.html>).
  - It ships in **Clang 23.1.0**:
    <https://releases.llvm.org/23.1.0/tools/clang/docs/ReleaseNotes.html>.
- **Its first casualty was a compiler that generates `#include` lines.**
  Swift PR #92509, by charles-zablit, merged 2026-09-22,
  <https://github.com/swiftlang/swift/pull/92509>. Swift's PrintAsClang wrote
  bridging-header paths into generated quoted includes with Windows
  backslashes. Swift's harness compiles generated headers with `-Weverything
  -Werror`, so the new warning failed them, and the fix normalises the paths
  with `convert_to_slash`.
- **The class has been in clang's sight since 2020.** Nico Weber's commit
  `b9d50bd` (2020-05-01),
  <https://www.mail-archive.com/cfe-commits@lists.llvm.org/msg180944.html>:
  *"Also, on Windows backslashes in include lines often end up escaped so
  that there are two of them."* *"Having backslashes in include lines is
  undefined behavior in most cases and implementation-defined behavior in
  C++20, but since clang treats it as normal repeated path separators, the
  diagnostic should too."* That first sentence describes F8's undecoded `\\`.
  (The commit says C++20; the C++ change is older: CWG 787, 2009.)

### Question 3. What other front ends do with the name a binding includes

The front ends the brief names. In the code paths I read, **none checks a
header name's characters before handing it to C.** That negative covers only
what I read; a check elsewhere in these tools is not excluded.

- **Zig `@cInclude`**, from the 0.13.0 docs,
  <https://ziglang.org/documentation/0.13.0/>: *"This function can only occur
  inside @cImport. This appends `#include <$path>\n` to the c_import temporary
  buffer."* I did not read Zig's compiler source, so whether anything checks
  the path there is unverified. The 0.16.0 release notes,
  <https://ziglang.org/download/0.16.0/release-notes.html>, say C translation
  moves to the build system and that `@cImport` *"is now deprecated"*. The
  `#include` lines then go into a C header the user writes.
- **Go's cgo preamble** is C that the user writes,
  <https://pkg.go.dev/cmd/cgo>: *"that comment, called the preamble, is used
  as a header when compiling the C parts of the package."* Go does check
  three other strings it hands to the C toolchain, and it checks its own file
  names:
  - **`#cgo` arguments** (`doc.go`,
    <https://raw.githubusercontent.com/golang/go/master/src/cmd/cgo/doc.go>):
    *"For security reasons, only a limited set of flags are allowed, notably
    -D, -U, -I, and -l."* and *"only a limited set of characters are
    permitted [...] Attempts to use forbidden characters will get a
    "malformed #cgo argument" error."* That allowlist (`safeString` in
    <https://raw.githubusercontent.com/golang/go/master/src/go/build/build.go>)
    was widened at least five times, each time cited in its source comment:
    - `$` for `-Wl,$ORIGIN`: issue 6038, opened 2013-08-04;
    - `@` for macOS loader paths: issue 13720, 2015-12-23;
    - `%` for Jenkins paths: issue 16959, 2016-09-01, refused as
      *"malformed #cgo argument: -I/somewhere/a%2Fb/include"*;
    - `!` for module paths: issue 26716, 2018-07-31 (its title concerns git
      submodules, so this tie rests on the comment);
    - `~` and `^` for sr.ht: issue 32260, 2019-05-26.
  - **pkg-config names** pass `load.SafeArg` or fail with *"invalid
    pkg-config package name: %s"* (`cmd/go/internal/work/exec.go`).
    `SafeArg`'s comment says *"args beginning with - are not safe (they look
    like flags)"* and *"args beginning with @ are not safe (they look like GNU
    binutils flagfile specifiers"*.
  - **Why Go checks at all**: CVE-2018-6574, *"cmd/go: arbitrary code
    execution during "go get""*, issue #23672 opened by rsc on 2018-02-02,
    <https://github.com/golang/go/issues/23672>. The attack was a `-fplugin=`
    flag in `#cgo` directives, fixed by an allowlist in Go 1.8.7, 1.9.4 and
    1.10rc2.
  - **Go's own file names**, the closest analogue to this sitting:
    - The spec gained one sentence in commit `ac4055b2` by Robert Griesemer,
      2012-02-22, *"go spec: import path implementation restriction"*:
      *"Implementation restriction: A compiler may restrict ImportPaths to
      non-empty strings using only characters belonging to Unicode's L, M, N,
      P, and S general categories (the Graphic characters without spaces) and
      may also exclude the ASCII characters"*, followed by a list of 25 ASCII
      characters (counted one by one), among them `"`, `'`, `\`, `<`, `>` and
      `*`
      (<https://go.googlesource.com/go/+/ac4055b2c5a81047271d8a0b830b657820a29698%5E%21/doc>).
    - gc's `checkImportPath`
      (<https://raw.githubusercontent.com/golang/go/master/src/cmd/compile/internal/noder/import.go>)
      says *"import path is empty"*, *"import path contains NUL"*, *"import
      path contains control character"*, **"import path contains backslash;
      use slash"**, and *"import path contains invalid character '%c'"*.
    - go/types' `validatedImportPath`
      (<https://raw.githubusercontent.com/golang/go/master/src/go/types/resolver.go>)
      **calls `strconv.Unquote` first** and then refuses, in the decoded
      value, the empty string (*"empty string"*), any non-graphic or space
      character, and every character in its `illegalChars`, with *"invalid
      character %#U"*. **This is (1h) exactly.**
- **Rust `bindgen`**, <https://raw.githubusercontent.com/rust-lang/rust-bindgen/main/bindgen/lib.rs>:
  every input header but the last goes to clang as `["-include", header]`,
  and the last is the positional input. The last one only is checked against
  the filesystem, failing with `BindgenError::NotExist`, `FolderAsHeader` or
  `InsufficientPermissions`. Their message texts I did not read. No character
  check.
- **The `cc` crate**, <https://docs.rs/cc/latest/cc/struct.Build.html>, names
  no header: `include` is *"Add a directory to the `-I` or include path for
  headers"*.
- **Nim's `header` pragma**: `generateHeaders` in
  <https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/cgen.nim>
  (read in source) handles the string three ways:
  - a string starting with `#` is emitted verbatim as a preprocessor line,
    with backticks turned into `"`;
  - one starting with `"` or `<` is written as given after `#include`;
  - anything else is wrapped in quotes.
  No character check. Nim's own library writes C's brackets inside the
  string: `proc c_isnan(x: float): bool {.importc: "isnan", header:
  "<math.h>".}`
  (<https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/pure/math.nim>).
  I could not reach the manual's own sentence: unverified.
- **Swift.**
  - The bridging header: `ClangImporter::importBridgingHeader`,
    <https://raw.githubusercontent.com/swiftlang/swift/main/lib/ClangImporter/ClangImporter.cpp>
    (read in source), builds `"#import \""` (or `"#include \""`) + header +
    `"\"\n"` into a memory buffer, with no escaping.
  - Module maps: clang parses the `header` string. In LLVM 15 it decoded
    escapes with `StringLiteralParser`
    (<https://raw.githubusercontent.com/llvm/llvm-project/llvmorg-15.0.0/clang/lib/Lex/ModuleMap.cpp>).
    In main it takes the raw spelling, with the quotes cut
    (<https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/Lex/ModuleMapFile.cpp>),
    which is the shape of Heroes' `unquote`. The change came with PR #119740,
    *"[clang][modules] Separate parsing of modulemaps"*, by Michael Spencer,
    merged 2025-02-26 (<https://github.com/llvm/llvm-project/pull/119740>).
    Its description, as the fetch tool rendered it, mentions no change to how
    strings are read. Whether a later step decodes them I did not search.
- **D's ImportC**, <https://dlang.org/spec/importc.html>: *"import hello;
  which will, if hello is not a D file, and has an extension .i, .h, or .c,
  compile hello with ImportC"*. The C file is run through the platform's C
  preprocessor. The D side names it by a module identifier, so no header
  string exists in D.
- **Python's cffi**, <https://cffi.readthedocs.io/en/stable/cdef.html>:
  `cdef()` source *"cannot contain `#include`"*. `set_source()`'s C code
  *"typically contains some `#include`"* and goes to the C compiler as
  written. No check. The error raised for an `#include` in `cdef()` was not
  found: unverified.

Beyond the brief, three more front ends that write `#include` lines, and two
that check a library name:

- **Cython**: `IncludeCode` in
  <https://raw.githubusercontent.com/cython/cython/master/Cython/Compiler/Code.py>
  turns a string with `include[0] == '<' and include[-1] == '>'` into
  `#include {}`, and anything else into `#include "{}"`. No check. Its own
  library writes `cdef extern from "<vector>" namespace "std" nogil:`
  (<https://raw.githubusercontent.com/cython/cython/master/Cython/Includes/libcpp/vector.pxd>).
- **Vala**: `CCodeIncludeDirective.write`,
  <https://raw.githubusercontent.com/GNOME/vala/main/ccode/valaccodeincludedirective.vala>,
  writes `<` + filename + `>` (or quotes) with no escaping or check.
- **Haskell 2010**, chapter 8,
  <https://www.haskell.org/onlinereport/haskell2010/haskellch8.html>:
  - The Report specifies a positive rule (section 8.5.1): `chname →
    {chchar} . h` and `chchar → letter | ascSymbol⟨&⟩`, with `letter →
    ascSmall | ascLarge | _` (section 8.3). So no digit is admitted.
  - It gives the reason for the suffix: *"chname must end in the suffix .h
    to make parsing of the specification of external entities
    unambiguous"*. It also says the admissible names are *"a subset of those
    permitted as arguments to the #include directive in C"*.
  - GHC accepted names outside that grammar: ticket #8650 (nh2, 2014-01-05,
    <https://mailman.haskell.org/archives/list/ghc-tickets@haskell.org/thread/GKTL5E6HGZTPLJH3QNKUIT6PBCOB6FTF>)
    reports that `"myheaderBLA.h myfunction"` and `"some.rubbish"` compile
    without error.
  - The GHC user guide,
    <https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/ffi.html>:
    *"Earlier versions of GHC (6.8.3 and earlier) `#include`d the header file
    [...] GHC no longer includes external header files when compiling via C,
    so this checking is not performed."*
- **Rust E0454**, <https://doc.rust-lang.org/error_codes/E0454.html>:
  *"`#[link(name = "")]` given with empty name"*.
- **Go's import paths**, above.

### What it means for Q1 to Q5

**Q1.** The coordinator's recollection is **right in substance and its
paragraph is 3**: 6.4.7p3 in C11, C17 (by N2310) and C23, and 6.4.8p3 in the
C2y draft.

- (1a)'s set is C's grammar, so a name holding it is not an angled header
  name at all.
- (1b)'s set is exactly paragraph 3's angled list, and footnote 81 gives the
  reason for it, escapes.
- (1c) cannot be "what C guarantees", because 6.10.2p5's guaranteed names do
  not include `sys/types.h`. Its width is a choice, and the ffi-pragmatist's
  survey must price it.

**Q2. The message.** gc's words are the precedent: name the character, and
for `\` name the replacement in words (*"import path contains backslash; use
slash"*), with no machine-applied fix.

**The `\` to `/` fix is `guess`.** Three precedents agree:

- GCC's manual makes `/` equivalent to `\` only on systems that *"interpret
  `\` as a pathname separator"*; elsewhere `\` is *"ordinary text"*.
- clang's own fix-it is off by default and tested only on Windows.
- gc states the fix in prose and does not apply it.

**C's brackets written inside the string** (`extern "<stdio.h>"`) support
`certain`, for the exact shape `"<" name ">"` with no other bracket inside.
Nim and Cython both define that spelling as `#include <name>`, which is the
only line Heroes writes, so removing the brackets leaves the C line byte for
byte the same. A lone bracket (`"stdio.h>"`) has no precedent that defines
it, so no fix there is certain.

**Whether models write it** (F6's question): it is the convention of Nim's
and Cython's own libraries, so it is in what models learn from. That is a
reason to expect it, not a measurement.

**Q3.** Precedent puts such a check early, in the front end: gc's importer,
go/types' resolver, and GHC's parser for the foreign-import string (behaviour
known from ticket #8650; the code I could not reach). So a check beside
`machine_locked` is consistent with precedent. The line budget is not this
seat's to judge.

**Q4.**
- **Go**: one sentence in its spec (2012, "Implementation restriction"), and
  the compilers' messages name each character. Both have stood since.
- **Haskell 2010**: a grammar stricter than real names, which GHC did not
  hold programs to and then stopped using.
- **C**: a paragraph.
- **This project**: panel 055's removal (F4) is the project's own precedent,
  and nothing above overturns it.

If a sentence is written, precedent says it should state **exactly** the set
the checker refuses (Go's shape), never a stricter one (Haskell's).

**Q5.** For `package`, Go refuses a pkg-config name with a leading `-` or `@`
(`SafeArg`) because it would be read as an option or a response file. For any
string that reaches a C toolchain, the cautionary precedent is CVE-2018-6574.
For an empty library name, Rust has E0454.

**Whether a Heroes `link` or `package` string can begin with `-` or `@` and
reach clang or pkg-config as an option is a question I cannot run.** The
compiler-engineer can.

Where else the class lives: clang's own `-include` and Swift's bridging header
both write a path into `#include "..."` / `#import "..."` unescaped. So any
Heroes string that becomes C text carries this class too.

## argument

C11 through the C2y draft leave `'`, `\`, `"`, `//` and `/*` undefined between
`<` and `>`, and their grammar excludes `>`, a line end and the empty name. GCC
and MSVC document that escapes are not decoded in a header name, and `\` is a
path separator on Windows (MSVC's own example) but an ordinary character to
GCC. A Heroes string's escapes can therefore be decoded only by the front end,
which is what Go's type checker does: unquote, then refuse named characters in
the value. Positive rules were narrower than real use (C's guaranteed set;
Haskell 2010's grammar, which GHC did not enforce) or widened five times in six
years (go/build). Hence (1h), and not (1c), (1d) or (1i).

## condition

My reading changes if any of these holds:

- **The ffi-pragmatist's survey finds widely used headers whose names hold
  `//`, `'` or `/*`.** Then (1b) costs real programs. Precedent would then
  support refusing only what go/types refuses (`'`, `"`, `\`, `*`) and
  leaving `//` alone, or only the escape-colliding pair `\` and `"` that
  footnote 81 is about.
- **The Windows box shows clang reading `\` inside `<...>` as an ordinary
  filename character, not a separator.** Then the platform argument for
  refusing `\` falls, and C's undefined behaviour is its only ground.
- **A front end turns up that checks header names with a positive rule and
  never widened it.** Then my objection to (1c) weakens.
- **The census finds an existing `extern` line that relies on today's
  undecoded spelling.** Then (1f) has a migration cost that precedent cannot
  price.

**Falsifiable predictions**, each unrun by me:

1. **(1f) without (1b) would make one program name two different files.**
   `extern "a\\b.h"`, written as `#include <a\b.h>`, builds on the Windows box
   against a tree holding only `a/b.h` (directory `a`, file `b.h`), and fails
   on this Mac against the same tree. This follows from MSVC's documentation
   and Weber's 2020 commit. It is falsified if clang on Windows does not find
   the header. Unrun: the box is offline (F1).
2. **GCC and clang end `#include <a\>b.h>` in different places.** GCC names
   `a\` and warns of extra tokens; clang names `a\>b.h`. One run of each on
   Linux falsifies it.
3. **For the llm-ergonomist, not mine to run:** in sessions that bind a
   system header with no Heroes `extern` example in view, a model writes C's
   brackets inside the string at least once. It is falsified if no session
   does.

**Unverified, said so:**

- C17's own draft N2176, not read.
- N3572's body, not read.
- N2940's author and date (from a search result).
- The Nim manual's sentence on `header`.
- GHC's `parseCImport` source (GitLab refused the fetch, and the mirrors cut
  the file off).
- Zig's compiler source for `@cInclude`.
- bindgen's error texts.
- cffi's error for `#include` in `cdef()`.
- Whether current clang decodes module-map strings after lexing.
- Whether Simon Tatham's 2018 RFC (llvm-dev, 2018-06-27,
  <https://lists.llvm.org/pipermail/llvm-dev/2018-June/124322.html>) landed.
- Issue 26716's tie to `!`.
- Which five escapes spec § 2 defines (this seat read nothing of the trunk
  but its brief).
