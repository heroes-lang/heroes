# Panel 230, the historian's report

Seat: historian, advisory, no veto. Written as it goes, 2026-10-26. Every precedent below was
looked up on the web in this session and its page re-read before its line was written; each is
marked `verified` (the page was read and states it), `secondary` (a page about it states it) or
`unverified`. Each line says what the page itself states and nothing of what Heroes would do.
*Heroes of code* was not used as a citation anywhere in this report. Pages were read through a
fetching tool that summarises long pages: every quotation below is what that reading returned as
the page's words, and a number or quotation that came back only paraphrased is marked so.

## Where this report starts (the project's own historians, re-read, not copied)

- **Panel 159's historian** (`docs/panel/159-reports/historian.md`): RFC 1937 verified there
  (Motivation *"a trip hazard for new users"*; `Termination::report` returning an `i32`; for
  `Result<T, E>` the error printed to stderr); the Rust release that shipped it left
  **unverified**; Go's refusal verified as the compiler's text *"func main must have no arguments
  and no return values"* (issues 34673 and 44750), **its reason not retrieved**. Both are re-read
  below (question 5), and both gaps are now closed.
- **Panel 117** (`docs/panel/117-both-sides-of-an-assert-when-a-side-is-an-aggregate.md`): its
  historian's row (`:75`) predicted *if a renderer reachable only from `assert` lands, the first
  non-test consumer adds a second printer rather than reusing it*; JEP 306, N1426 and P2186R2 are
  named in its part 2 (`:214-217`) as the shape of a narrowing with its restoration condition
  written down. Its file carries the names and not the links; the three are re-read at the end.
- **Panel 131** (`docs/panel/131-the-refusals-were-sound-and-their-reasons-were-not.md:265-271`):
  corrected 117's Zig reading: `std.testing.expectEqual` recurses through struct fields to a
  scalar leaf; on optionals it prints the whole payload, on slices it compares pointer and length,
  and Zig ships `expectEqualDeep`. Re-read below against the source (question 3).

## 1. A place passed, not computed (L1)

- **Wirth, Project Oberon: the position is written into the trap instruction**, verified,
  https://people.inf.ethz.ch/wirth/ProjectOberon/Sources/ORG.Mod.txt (*"MODULE ORG; (* N.Wirth,
  16.4.2016 / 4.4.2017 / 31.5.2019 Oberon compiler; code generator for RISC*)"*): `PROCEDURE
  Trap(cond, num: LONGINT); BEGIN Put3(BLR, cond, ORS.Pos()*100H + num*10H + MT) END Trap;`, called
  for the index check (`Trap(10, 1)`), a NIL pointer (`Trap(EQ, 4)`), a division (`Trap(LE, 6)`), a
  type guard (`Trap(NE, 2)`), `ASSERT` (`Trap(cond, 7)`). The handler, verified,
  https://people.inf.ethz.ch/wirth/ProjectOberon/Sources/System.Mod.txt (*"NW 20.6.2016"*): reads
  the trapping instruction, `w := v DIV 10H MOD 10H; (*trap number*)`, writes `"  pos "`, `v DIV
  100H MOD 10000H`, `"  TRAP"`, the number, `" in "`, the module's name found by walking
  `Modules.root`, `" at"` and the address, then `Oberon.Reset`. The position is the scanner's,
  verified, https://people.inf.ethz.ch/wirth/ProjectOberon/Sources/ORS.Mod.txt: `PROCEDURE Pos*():
  LONGINT; BEGIN RETURN Texts.Pos(R) - 1 END Pos;`, a character offset, which the compiler's own
  error messages print the same way, `Texts.WriteString(W, "  pos "); Texts.WriteInt(W, p, 1)`.
- **Rust, RFC 2091 `track_caller`**, verified,
  https://raw.githubusercontent.com/rust-lang/rfcs/master/text/2091-inline-semantic.md: start date
  *"2017-07-31"*; the function *"accepts the caller's location as an additional argument"*; *"the
  panicking branch will need to allocate space to store the varying caller location, so the number
  of instructions per `unwrap()`/`expect()` will increase"*; *"The `caller_location()` intrinsic
  returns the `Location` structure which encodes the file, line and column of the callsite"*; its
  example prints *"thread 'main' panicked at 'nothing to see here, move along', 3.rs:12:29"*
  (file, line, column). The page as it stands today also says *"An unstable flag `-Z
  location-detail` is added to `rustc`"*, with a redacted form *"<redacted>:192:0"*.
- **Rust 1.46.0 stabilised it**, verified, https://blog.rust-lang.org/2020/08/27/Rust-1.46.0/
  (Aug. 27, 2020): *"Back in March, the release of Rust 1.42 introduced better error messages when
  `unwrap` and related functions would panic ... Rust 1.46 stabilizes this feature. This attribute
  is called `#[track_caller]`, which was originally proposed in RFC 2091 way back in July of
  2017!"*; a panic hook *"can use the caller method on `std::panic::Location`"*.
- **What it cost, measured by Rust**, verified, https://github.com/rust-lang/rust/issues/70579
  (*"Measure binary size impact of implicit caller location"*, opened Mar 30, 2020, open): *"rustc
  with `#[track_caller]` in the standard library uses ~0.68% of its binary size on the static
  `Location` allocations and their filenames"*; *"`Location`s are 24 bytes when
  `size_of::<usize>() == size_of::<u64>()`"*; on an embedded kernel (Tock Imix, 2021-10-13), from
  175,304 bytes: *"all Panic locations fused: 158,356 bytes (-16,948 bytes / -9.6%)"*, *"keeping
  filenames but setting all line/column numbers to 0: 163,752 bytes (-11,552 bytes / -6.6%)"*,
  *"keeping line numbers + column numbers, setting filenames to "": 172,856 bytes (-2,448 bytes /
  -1.4%)"*, *"track_caller completely disabled: 157,204 bytes (-18,100 bytes / -10.3%)"*.
- **The knob Rust added for that cost**, verified,
  https://doc.rust-lang.org/beta/unstable-book/compiler-flags/location-detail.html: *"Option `-Z
  location-detail=val` ... allows users to control what location details are printed as part of
  panic messages, by allowing them to exclude any combination of filenames, line numbers, and
  column numbers"*; *"This option is intended to provide users with a way to mitigate the size
  impact of `#[track_caller]`"*; tracking issue 70580 (verified,
  https://github.com/rust-lang/rust/issues/70580: *"Mitigate binary size impact of implicit caller
  location"*, opened March 30, 2020, closed by PR 89920 on October 23, 2021). Still unstable on the
  page read.
- **Rust 1.73 changed what the place line looks like**, verified,
  https://blog.rust-lang.org/2023/10/05/Rust-1.73.0/: *"The output produced by the default panic
  handler has been changed to put the panic message on its own line instead of wrapping it in
  quotes"*, before ``thread 'main' panicked at 'oh no! "ferris.txt" not found!', src/main.rs:3:5``,
  after ``thread 'main' panicked at src/main.rs:3:5:`` then the message on the next line; *"This is
  especially useful when the message is long, contains nested quotes, or spans multiple lines."*
  The place line names the thread. In both of the post's examples the column named (`:3:5`,
  `:2:5`) is where the `panic!` or `assert_eq!` invocation begins, as the reading returned the
  examples' code indented by four spaces.
- **Swift, `fatalError`, `precondition`, `assert`**, verified,
  https://raw.githubusercontent.com/swiftlang/swift/main/stdlib/public/core/Assert.swift: each
  takes `file: StaticString = #file, line: UInt = #line` as default arguments, **no column**;
  `precondition` in `-O`: *"If `condition` evaluates to `false`, stop program execution"*; `assert`
  in `-O`: *"`condition` is not evaluated"*.
- **Swift SE-0274, why the path left `#file`**, verified,
  https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0274-magic-file.md:
  *"Today, `#file` evaluates to a string literal containing the full path to the current source
  file"*; *"It can inadvertently reveal private or sensitive information. The full path to a source
  file may contain a developer's username, hints about the configuration of a build farm"*; *"most
  uses of `#file` are in default arguments, which makes this information capture invisible at the
  use site"*; *"It bloats binaries ... a shorter `#file` string reduced code size by up to 5%"*;
  *"several taking 22% less time"*; *"It introduces artificial differences between binaries built
  on different machines"*; the new string *"`<module-name>/<file-name>`"*; the full path kept as
  `#filePath`; *"The change to the behavior of `#file` was deferred to the next major language
  version (Swift 6)"*.
- **Swift SE-0285, `#fileID`**, verified,
  https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0285-ease-pound-file-transition.md:
  *"Status: Implemented (Swift 5.3)"*; *"Swift will support `#fileID`. This new magic identifier
  will generate the same module-and-filename string SE-0274 specified for `#file`"*; in a future
  mode *"`#file` will generate the same string as `#fileID`, and `#fileID` will be deprecated"*.
  **Amended**: verified, https://forums.swift.org/t/file-vs-fileid-in-swift-6/74614 (Frederick
  Kellison-Linn, September 24, 2024, reporting the Language Steering Group): *"the LSG determined
  that there was no longer consensus that this remains the correct path forward"*; *"The status
  quo will be that `#fileID` remains supported."*
- **Zig, `@src()`'s record**, verified at the canonical repository,
  https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/lang.zig (verified,
  https://ziglang.org/news/migrating-from-github-to-codeberg/, November 26, 2025: *"the canonical
  origin/master branch of the main Zig project repository is
  `https://codeberg.org/ziglang/zig.git`"*, and *"I have made ziglang/zig on GitHub read-only"*;
  the archival GitHub `lib/std/builtin.zig` carries the same doc comments): `SourceLocation` is
  `module` (*"The name chosen when compiling. Not a file path."*), `file` (*"Relative to the root
  directory of its module."*), `fn_name`, `line: u32`, `column: u32`; *"used by the Zig language
  code generation and therefore must be kept in sync with the compiler implementation"*. The
  language reference, verified, https://ziglang.org/documentation/master/: `@src()
  std.lang.SourceLocation`, *"Returns a `SourceLocation` struct representing the function's name
  and location in the source code. This must be called in a function."*
- **C's `assert`**, verified, C89 draft, http://port70.net/~nsz/c/c89/c89-draft.html § 4.2.1.1:
  *"the assert macro writes information about the particular call that failed (including the text
  of the argument, the name of the source file, and the source line number ... on the standard
  error file in an implementation-defined format"*, then calls `abort`; no function name. C11
  draft N1570, http://port70.net/~nsz/c/c11/n1570.html § 7.2.1.1p2: *"writes information about the
  particular call that failed ... on the standard error stream in an implementation-defined
  format"*, naming *"the values of the preprocessing macros __FILE__ and __LINE__ and of the
  identifier __func__"*; footnote 191: *"Assertion failed: expression, function abc, file xyz,
  line nnn."* No column in either. That `__func__` arrived **in C99** is **secondary**
  (https://en.cppreference.com/w/c/error/assert marks it *"(since C99)"*); the C99 text itself was
  not read.
- **C++20, P1208R6, *Adopt source_location for C++20***, verified,
  https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1208r6.pdf (2019-07-19; Douglas,
  Jabot, Krügler, Sommerlad): `current()` gives `line_`, `column_`, `file_name_`,
  `function_name_`; *"The value should be affected by #line in the same manner as for __LINE__ and
  __FILE__"*; `column_` is *"an implementation-defined value denoting some offset from the start of
  the line ... Column numbers are presumed to be 1-indexed; however, an implementation is
  encouraged to use 0 when the column number is unknown"*; `function_name_` *"a name of the
  current function such as in __func__ ... if any, an empty string otherwise"*; *"[Note: When used
  as a default argument ..., the value of the source_location will be the location of the call to
  current at the call site.]"*; *"The intent of source_location is to have a small size and
  efficient copying"*; and of its Cologne revision: *"we discussed if the functions in
  source_location are signal safe as with initializer_list, but did not dare to specify it at this
  point."*
- **GHC, `HasCallStack`**, verified,
  https://hackage-content.haskell.org/package/base-4.22.0.0/docs/GHC-Stack.html: *"A function can
  request its call-site with the HasCallStack constraint"*, *"just an alias for an implicit
  parameter ?callStack :: CallStack"*; *"If there is a CallStack in scope -- i.e. the enclosing
  function has a HasCallStack constraint -- GHC will append the new call-site to the existing
  CallStack"*; `HasCallStack` *"Since: base-4.9.0.0"*; `SrcLoc` carries package, module, file, start
  line and column, end line and column. The user's guide, verified,
  https://downloads.haskell.org/ghc/latest/docs/users_guide/exts/callstack.html, prints
  `CallStack (from HasCallStack):` then `error, called at <interactive>:25:1 in
  interactive:Ghci5`; *"GHC will never infer a HasCallStack constraint, you must request it
  explicitly"*; it *"does not interact with the RTS and does not require compilation with -prof"*;
  *"The CallStack will only extend as far as the types allow it."* Which GHC release introduced it
  is on neither page: **unverified**.
- **OCaml, `Assert_failure`**, verified, https://ocaml.org/manual/latest/core.html: *"exception
  Assert_failure of (string * int * int)"*, *"The arguments are the location of the assert keyword
  in the source code (file name, line number, column number)"*; `Match_failure` the same of *"the
  match keyword"*. An uncaught one, verified,
  https://raw.githubusercontent.com/ocaml/ocaml/trunk/runtime/printexc.c: `fprintf(stderr, "Fatal
  error: exception %s\n", msg);` then `exit(2);` (`abort();` when abort-on-uncaught is set).
- **Which print a column, and in what form** (from the lines above, nothing added): Rust
  `src/main.rs:3:5`, at the macro's start in the post's examples; GHC `<interactive>:25:1`; OCaml a
  triple, the column of the `assert` **keyword**; Zig and C++20 carry a column field (C++20's
  *"implementation-defined"*, *"1-indexed"*, 0 when unknown); Wirth's Oberon a character offset in
  the module, no line; Swift's three functions and C's `assert` a file and a line and **no
  column**.

## 2. A place found at the stop (L2, L4), or resolved later (L5)

- **Go, the runtime's traceback**, verified, https://pkg.go.dev/runtime (`GOTRACEBACK`): *"By
  default, a failure prints a stack trace for the current goroutine, eliding functions internal to
  the run-time system, and then exits with exit code 2"*; *"The failure prints stack traces for all
  goroutines if there is no current goroutine or the failure is internal to the run-time"*; `none`
  *"omits the goroutine stack traces entirely"*; `crash` *"crashes in an operating system-specific
  manner instead of exiting"*; `wer` *"doesn't disable Windows Error Reporting (WER)"*. And
  https://pkg.go.dev/os#pkg-variables, verified: *"Note that the Go runtime writes to standard
  error for panics and crashes; closing Stderr may cause those messages to go elsewhere, perhaps to
  a file opened later."* The exact text of a Go traceback was not on a page read: **unverified**.
- **Go's line table travels in the binary**, verified, https://pkg.go.dev/debug/gosym: *"Package
  gosym implements access to the Go symbol and line number tables embedded in Go binaries
  generated by the gc compilers"*.
- **Go's specification names no status**, verified, https://go.dev/ref/spec (read whole, in four
  chunks): an unrecovered panic, *"the program is terminated and the error condition is reported,
  including the value of the argument to `panic`"*; a run-time panic is *"equivalent to a call of
  the built-in function `panic` with a value of the implementation-defined interface type
  `runtime.Error`"*; no sentence naming an exit status or code was found in any chunk. The 2 is the
  `runtime` package's documentation, above.
- **Go took the builder's paths out on request**, verified, https://go.dev/doc/go1.13: *"The new
  `go` `build` flag `-trimpath` removes all file system paths from the compiled executable, to
  improve build reproducibility"*; https://pkg.go.dev/cmd/go: *"the recorded file names will begin
  either a module path@version (when using modules), or a plain import path (when using the
  standard library, or GOPATH)"*.
- **GCC's remapping of `__FILE__`**, verified,
  https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html: `-fmacro-prefix-map=old=new`,
  *"When preprocessing files residing in directory old, expand the __FILE__ and __BASE_FILE__
  macros as if the files resided in directory new instead"*; it *"can be used to change an absolute
  path to a relative path by using . for new which can result in more reproducible builds that are
  location independent"*. Which release added it: the GCC 8 changes page does not name it;
  **unverified**.
- **Nim keeps a frame per procedure at run time**, verified, https://nim-lang.org/docs/system.html:
  `TFrame` holds `prev` (*"Previous frame; used for chaining the call stack"*), `procname` (*"Name
  of the proc that is currently executing"*), `line` (*"Line number of the proc that is currently
  executing"*), `filename`; `PFrame` *"Represents a runtime frame of the call stack; part of the
  debugger API"*. The compiler's guide, verified, https://nim-lang.org/docs/nimc.html: with
  `--stackTrace`, *"the generated C contains code to ensure that proper stack traces are given if
  the program crashes or some uncaught exception is raised"*; `--lineTrace` *"implies the
  stackTrace option"* and adds *"line number information"*; `--excessiveStackTrace:on|off`,
  *"stack traces use full file paths"*. What these cost, and their defaults, are not on the page:
  **unverified**.
- **Rust's backtrace**, verified, https://doc.rust-lang.org/std/backtrace/index.html (the module
  since 1.65.0): *"For most platforms a backtrace with a filename/line number requires that
  programs be compiled with debug information. Without debug information filenames/line numbers
  will not be reported"*; *"no guarantees are provided about the exact accuracy of a backtrace.
  Instruction pointers, symbol names, filenames, line numbers, etc, may all be incorrect when
  reported"*; *"Some platforms simply do nothing when capturing a backtrace"*; *"Capturing a
  backtrace can be a quite expensive runtime operation"*, so `Backtrace::capture` captures nothing
  unless `RUST_LIB_BACKTRACE` or `RUST_BACKTRACE` says so.
- **binutils' `addr2line`**, verified,
  https://sourceware.org/binutils/docs/binutils/addr2line.html: *"addr2line translates addresses
  or symbol+offset into file names and line numbers. Given an address or symbol+offset in an
  executable or an offset in a section of a relocatable object, it uses the debugging information
  to figure out which file name and line number are associated with it."*
- **Apple's `atos` and the `.dSYM`**, verified,
  https://developer.apple.com/documentation/xcode/adding-identifiable-symbol-names-to-a-crash-report
  (read through its JSON,
  https://developer.apple.com/tutorials/data/documentation/xcode/adding-identifiable-symbol-names-to-a-crash-report.json):
  *"Replace hexadecimal addresses in a crash report with function names and line numbers that
  correspond to your app's code"*; *"An unsymbolicated crash report displays only hexadecimal
  addresses in the backtraces, with no function names"*; *"an unsymbolicated crash report is rarely
  useful"*; the `.dSYM` must be *"the one that matches the binary image's build UUID"*; the
  example `atos -arch arm64 -o TouchCanvas.app.dSYM/Contents/Resources/DWARF/TouchCanvas -l
  0x10459c000 -i 0x1045a4610`, the load address read from the report's list of binary images.
- **Breakpad**, verified,
  https://chromium.googlesource.com/breakpad/breakpad/+/HEAD/docs/getting_started_with_breakpad.md:
  *"a library and tool suite that allows you to distribute an application to users with
  compiler-provided debugging information removed"*, which can *"produce C and C++ stack traces
  from these minidumps"*; the symbol dumper *"should be run on your binary before it's stripped"*,
  and *"the symbol files need to be stored somewhere that the minidump processor can find"*.
  Crashpad was not read: **unverified**.
- **What a signal handler may call (the shared brief's question 5)**, verified,
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html § 2.4.3 (IEEE Std
  1003.1-2024): *"The following table defines a set of functions and function-like macros that
  shall be async-signal-safe"*; the table holds `write`, `_exit`, `_Exit` and `abort`, and not
  `printf`, `fprintf`, `malloc`, `backtrace` or `dladdr` (the string `dladdr` is not in the part of
  the page holding the table); a handler that calls a function outside it is the undefined case
  the section lists. C++20 left `source_location`'s signal safety open (P1208R6, question 1).

## 3. Both sides of a failed comparison (S1, S3), and how a text side is written

- **Rust, `assert_eq!`**, verified, https://doc.rust-lang.org/std/macro.assert_eq.html (since
  1.0.0): *"On panic, this macro will print the values of the expressions with their debug
  representations."* Its form since 1.73, verified, https://blog.rust-lang.org/2023/10/05/Rust-1.73.0/:
  ``assertion `left == right` failed: ferris is not a fish``, then a `left:` line and a `right:`
  line, each `str` side written between double quotes (the post's two sides are emoji, so this line
  paraphrases them); before 1.73 each side was also wrapped in backticks inside the quoted message.
- **Rust, how `Debug` writes a text**, verified, https://doc.rust-lang.org/std/primitive.char.html:
  `escape_debug` *"will escape the characters similar to the Debug implementations of str or
  char"*, `'\n'.escape_debug().to_string()` is `"\\n"`; `escape_default`: *"Tab is escaped as
  `\t`"*, *"Line feed is escaped as `\n`"*, *"Double quote is escaped as `\"`"*, *"Backslash is
  escaped as `\\`"*, printable ASCII left as itself, *"All other characters are given hexadecimal
  Unicode escapes"* (`escape_unicode`, the `\u{...}` form). So a line feed inside a `str` side is
  written as two characters, `\n`, and not as a new line.
- **pytest, assertion rewriting**, verified, https://docs.pytest.org/en/stable/how-to/assert.html:
  *"Reporting details about a failing assertion is achieved by rewriting assert statements before
  they are run"*; *"pytest rewrites test modules on import by using an import hook to write new pyc
  files"*; it shows *"the values of the most common subexpressions including calls, attributes,
  comparisons"*; *"comparing long strings: a context diff is shown"*, *"comparing long sequences:
  first failing indices"*, *"comparing dicts: different entries"*. **Its bound**, verified,
  https://raw.githubusercontent.com/pytest-dev/pytest/main/src/_pytest/assertion/truncate.py:
  *"Current default behaviour is to truncate assertion explanations at terminal lines, unless
  running with an assertions verbosity level of at least 2 or running on CI"*, the notice
  `...Full output truncated, use '-vv' to show`; and
  https://raw.githubusercontent.com/pytest-dev/pytest/main/src/_pytest/assertion/_typing.py:
  `DEFAULT_MAX_LINES: ClassVar[int] = 8`, `DEFAULT_MAX_CHARS: ClassVar[int] = DEFAULT_MAX_LINES *
  80`, *"Default limits applied when the corresponding ini option is left unset"*, `0` *"leaves it
  unbounded"*.
- **Python `unittest`, a diff with a bound**, verified,
  https://docs.python.org/3/library/unittest.html: `maxDiff` *"controls the maximum length of diffs
  output by assert methods that report diffs on failure. It defaults to 80*8 characters"*;
  *"Setting `maxDiff` to `None` means that there is no maximum length of diffs"*; *"Added in version
  3.2"*; `assertMultiLineEqual`: *"When not equal a diff of the two strings highlighting the
  differences will be included in the error message"*.
- **Zig, `expectEqual`, re-read for panel 131**, verified at the canonical repository,
  https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/testing.zig: doc comment *"When the
  two values are not equal, prints diagnostics to stderr to show exactly how they are not equal"*;
  a struct: `inline for` over its field names, `try expectEqual(@field(expected, field_name),
  @field(actual, field_name))`, a recursion; an optional: one side null prints `"expected {any},
  found null\n"` with the other payload whole, both present **recurse** on the payloads; a slice:
  `"expected slice ptr {*}, found {*}\n"` then `"expected slice len {}, found {}\n"`, pointer and
  length and not contents; a scalar: `"expected {any}, found {any}\n"`. Panel 131's reading holds
  for structs and slices; on optionals the source today prints the payload only when the other
  side is null and otherwise recurses.
- **Zig, `expectEqualDeep`**, verified, same file: *"Pointer values are deeply equal if values they
  point to are deeply equal"*; a slice `"Slice len not the same, expected {d}, found {d}\n"`, then
  `"index {d} incorrect. expected {any}, found {any}\n"`; a struct `"Field {s} incorrect. expected
  {any}, found {any}\n"`.
- **Zig, `expectEqualSlices`, a first-difference window with a bound**, verified, same file:
  `"slices differ. first difference occurs at index {d} (0x{X})\n"`; `const max_lines: usize =
  16;`, `const max_window_size: usize = if (T == u8) max_lines * 16 else max_lines;`, the window's
  start aligned just before the first difference; beyond it `"... truncated, remaining items: "
  ... " ...\n"`.
- **Jest's `pretty-format`**, verified for what its README states,
  https://raw.githubusercontent.com/jestjs/jest/main/packages/pretty-format/README.md:
  `escapeString`, default `true`, *"escape special characters in strings"*; `maxDepth` and
  `maxWidth` default `Infinity` (*"levels to print in arrays, objects, elements, and so on"*,
  *"number of elements to print in arrays, sets, and so on"*).

## 4. A map shown in a fixed order

- **Go 1.12, `fmt` sorts a map by key**, verified, https://go.dev/doc/go1.12: *"Maps are now
  printed in key-sorted order to ease testing. The ordering rules are:"* *"When applicable, nil
  compares low"*, *"ints, floats, and strings order by <"*, *"NaN compares less than non-NaN
  floats"*, *"bool compares false before true"*, *"Complex compares real, then imaginary"*,
  *"Pointers compare by machine address"*, *"Channel values compare by machine address"*, *"Structs
  compare each field in turn"*, *"Arrays compare each element in turn"*, *"Interface values compare
  first by `reflect.Type` describing the concrete type and then by concrete value as described in
  the previous rules"*; and *"When printing maps, non-reflexive key values like `NaN` were
  previously displayed as `<nil>`. As of this release, the correct values are printed."*
- **Python's `pprint`, sorted by key with a fallback**, verified,
  https://raw.githubusercontent.com/python/cpython/main/Lib/pprint.py: `PrettyPrinter.__init__(...,
  sort_dicts=True, ...)`, while `pp(object, *args, sort_dicts=False, **kwargs)`; `items =
  sorted(object.items(), key=_safe_tuple)`; `_safe_key`, *"Helper function for key functions when
  sorting unorderable objects"*, *"(sorting first comparing the type name and then by the obj
  ids)"*, the code comparing `(str(type(self.obj)), id(self.obj))` when `<` raises `TypeError`.
  The documentation, verified, https://docs.python.org/3/library/pprint.html: *"Changed in version
  3.8: Added the sort_dicts parameter"*; `depth`: *"If the data structure being printed is too
  deep, the next contained level is replaced by `...`"*.
- **Node's `util.inspect`, ordered by the rendered text**, verified,
  https://nodejs.org/api/util.html: `sorted`, *"If set to `true` or a function, all properties of
  an object, and `Set` and `Map` entries are sorted in the resulting string"*, history *"v10.12.0 —
  The `sorted` option is supported now"*; and its source, verified,
  https://raw.githubusercontent.com/nodejs/node/main/lib/internal/util/inspect.js: `const
  comparator = ctx.sorted === true ? undefined : ctx.sorted;` then `ArrayPrototypeSort(output,
  comparator);`, `output` being the array of entries **already formatted**; and for a `WeakMap`
  or `WeakSet`, each entry formatted as `key => value` and then *"Sort all entries to have a
  halfway reliable output (if more entries than retrieved ones exist, we can not reliably return
  the same output) if the output is not sorted anyway."*
- **Jest's `pretty-format`**, verified for what its README states (link above): `compareKeys`,
  default `undefined`, *"compare function used when sorting object keys, `null` can be used to skip
  over sorting"*; the README does not say in words what the default order is.

## 5. `main` that fails (M1, M2, M4, M6)

- **Go's specification, in words close to § 1's**, verified, https://go.dev/ref/spec, *Program
  execution*: *"The main package must have package name `main` and declare a function `main` that
  takes no arguments and returns no value."*, `func main() { … }`.
- **Go's refusal, and now its reason** (panel 159's first condition), verified. Issue 27328,
  https://github.com/golang/go/issues/27328 (*"proposal: allow main.main to optionally return an
  error"*, nigeltao, 2018-08-29, closed 2018-09-19): rsc, 2018-09-19,
  https://github.com/golang/go/issues/27328#issuecomment-422888852: *"This seems like a clear "no"
  to me."* *"I see very little compelling benefit here and I strongly disagree with establishing a
  standard handling"* *"of what happens when main returns an error."* *"Your example of by default
  printing the error and exit 1 is one possibility but there are plenty of others."* *"Better to
  let main be in charge of that."* *"More generally, we've already made the decision about what
  main is."* *"I don't see a strong reason here to reopen it."* Issue 34673,
  https://github.com/golang/go/issues/34673: mvdan, 2019-10-03, *"I think this complicates the
  language ... for very little benefit - just saving yourself four lines of code"*, *"Why a
  `log.Fatalln` and not something else? With what logger? What if the main function panics before
  returning?"*; bcmills, *"This was proposed (and rejected) in #27328, and as far as I can tell
  this report does not add any new motivating data"*. Issue 44750,
  https://github.com/golang/go/issues/44750#issuecomment-814443887: ianlancetaylor, 2021-04-06,
  citing rsc, *"Nothing seems to have changed since then, and this issue does not seem to have
  much support"*, *"likely decline"*; 2021-05-04, *"Further comments but no real change in
  consensus."* (Read through GitHub's API, `api.github.com/repos/golang/go/issues/.../comments`;
  rsc's sentence came back in two pieces, quoted as two.)
- **Rust 1.26 shipped `main -> Result`**, verified, https://blog.rust-lang.org/2018/05/10/Rust-1.26/
  (May 10, 2018), *"`main` can return a `Result`"*: the pattern it replaced is shown as
  `println!("Application error: {}", e);` then `process::exit(1);`, *"We need to make this second
  function because `main` can't return a `Result`"*; *"If `main` returns an error, this will exit
  with an error code, and print out a debug representation of the error."* (This closes panel
  159's unverified release.)
- **What RFC 1937 asked for, re-read**, verified,
  https://raw.githubusercontent.com/rust-lang/rfcs/master/text/1937-ques-in-main.md (*"Start Date:
  2017-02-22"*): *"This proposal selects Display, largely because application error types are not
  obliged to implement Error"*; *"it is guaranteed to write diagnostics to `io::stderr` that include
  the `Display` text"*; and for the status, *"Therefore I am proposing that all the standard impls'
  `report` functions should use 0 for success and 2 for failure"*, *"It is important not to use 1,
  even though `EXIT_FAILURE` is usually 1, because some existing programs (notably `grep`) give 1 a
  specific meaning; as far as I know, 2 has no specific meaning anywhere"*; `sysexits.h` constants
  *"beyond the scope of this RFC"*; *"on "most Unix-like" operating systems, only the low 8 bits of
  this value are significant."*
- **Both were amended before it shipped**, verified. The tracking issue,
  https://github.com/rust-lang/rust/issues/43301: ketsuban, Jul 18, 2017, *"the least surprising
  thing is to return 1"*; liigo, Aug 11, 2017, *"Many people agree that the exit code should be `1`
  on failure (instead of `2`)"*; exit codes moved to a separate issue, 48711 (not read). The
  stabilisation issue, https://github.com/rust-lang/rust/issues/48453 (*"Stabilize `main` with
  non-() return types"*, closed Sep 17, 2018): *"we modified the `Result` impl to use `Debug`"*, its
  reason not given on the page; ExpHP, Feb 23, 2018, *"The RFC says `E: Display` instead of `E:
  Debug`, and tbh, Display would have been my first choice as well"*; the codes *"are defined as
  `libc::EXIT_SUCCESS` for "Ok" and `libc::EXIT_FAILURE` for "Err""*.
- **What ships today**, verified,
  https://raw.githubusercontent.com/rust-lang/rust/master/library/std/src/process.rs: `Err(err) =>
  { io::attempt_print_to_stderr(format_args_nl!("Error: {err:?}")); ExitCode::FAILURE`; the trait,
  https://doc.rust-lang.org/std/process/trait.Termination.html (since 1.61.0): *"The C-main
  function only supports returning integers. So, every type implementing the `Termination` trait
  has to be converted to an integer"*, `impl<T: Termination, E: Debug> Termination for Result<T,
  E>`. Rust 1.61, verified, https://blog.rust-lang.org/2022/05/19/Rust-1.61.0/ (May 19, 2022): the
  `Termination` trait *"finally stable"*, with `ExitCode`; its example, as the reading paraphrased
  it, is a `git bisect run` result type mapping its cases to 0, 1, 125 and 255 through `fn
  report(self) -> ExitCode`. `ExitCode`, verified,
  https://doc.rust-lang.org/std/process/struct.ExitCode.html: *"Constructs an `ExitCode` from an
  arbitrary u8 value"*; `FAILURE`, *"If you're only returning this and `SUCCESS` from `main`,
  consider instead returning `Err(_)` and `Ok(())` respectively, which will return the same codes
  (but will also `eprintln!` the error)"*.
- **Zig, `pub fn main() !void`**, verified,
  https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/start.zig: `const bad_main_ret =
  "expected return type of main to be 'void', '!void', 'noreturn', 'u8', or '!u8'";`; on an error,
  `std.log.err("{t}", .{err});`, then `std.debug.dumpErrorReturnTrace(trace)` (not on freestanding
  targets), then `return 1;`. Its default log, verified,
  https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/log.zig: *"It outputs to stderr"*, the
  line `level.asText()`, `": "`, the message, `.err => "error"`.
- **Swift, a throwing top level or `main`**, verified,
  https://raw.githubusercontent.com/swiftlang/swift/main/stdlib/public/core/ErrorType.swift:
  `_errorInMain`, *"Invoked by the compiler when code at top level throws an uncaught error"*,
  `fatalError("Error raised at top level: \(String(reflecting: error))")`; SE-0281, verified,
  https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0281-main-attribute.md
  (*"Implemented (Swift 5.3)"*): *"The `main()` method can either be declared as `throws` or not;
  errors thrown from a `main()` method will have the same behavior as errors thrown from top-level
  code."* The status a Swift `fatalError` exits with is on neither page: **unverified**.
- **Python's `sys.exit`**, verified, https://docs.python.org/3/library/sys.html: *"If another type
  of object is passed, `None` is equivalent to passing zero, and any other object is printed to
  `stderr` and results in an exit code of 1. In particular, `sys.exit("some error message")` is a
  quick way to exit a program when an error occurs"*; and for an integer, *"Most systems require
  it to be in the range 0–127, and produce undefined results otherwise"*, *"Unix programs generally
  use 2 for command line syntax errors and 1 for all other kinds of errors"*.
- **D**, verified, https://dlang.org/spec/function.html: `main` may return `void`, `int`,
  `noreturn` or `auto`; *"If main returns void, the OS will receive a zero value on success"*; *"If
  main returns void or noreturn, the OS will receive a non-zero value on abnormal termination,
  such as an uncaught exception."* The page does not say what is printed.
- **C**, verified, N1570, http://port70.net/~nsz/c/c11/n1570.html: § 5.1.2.2.3, *"reaching the }
  that terminates the main function returns a value of 0"*, *"If the return type is not compatible
  with int, the termination status returned to the host environment is unspecified"*; § 7.22.4.4p5,
  *"If the value of status is EXIT_FAILURE, an implementation-defined form of the status
  unsuccessful termination is returned. Otherwise the status returned is implementation-defined."*
- **Reversed after shipping**: none found among Rust (1.26 to today's `process.rs`), Zig (today's
  `start.zig`) and Swift (5.3 to today's `ErrorType.swift`), the three read that ship a failing
  `main`; Rust's shape was amended before it shipped, not after.

## 6. A way to write on stderr (M5)

- **Rust added one after shipping without it**, verified. RFC 1869,
  https://raw.githubusercontent.com/rust-lang/rfcs/master/text/1869-eprintln.md (*"Start Date:
  2017-01-23"*): *"This RFC proposes the addition of two macros to the global prelude, `eprint!` and
  `eprintln!`. These are exactly the same as `print!` and `println!`, respectively, except that they
  write to standard error"*; it sets `println!("out of cheese error: {}", 42);` against
  `writeln!(stderr(), "out of cheese error: {}", 42).unwrap();`; *"all of the tutorial
  documentation for the language uses `println!` for error messages, which teaches bad habits."*
  Shipped in 1.19, verified, https://blog.rust-lang.org/2017/07/20/Rust-1.19/ (July 20, 2017):
  *"The largest new library feature is the `eprint!` and `eprintln!` macros."* Rust 1.0, verified,
  https://blog.rust-lang.org/2015/05/15/Rust-1.0/, is dated May 15, 2015; the two years between
  the dates are this seat's arithmetic.
- **Go**, verified, https://pkg.go.dev/builtin: `print` and `println` *"writes the result to
  standard error"*; *"Println is useful for bootstrapping and debugging; it is not guaranteed to
  stay in the language."* And https://pkg.go.dev/os#pkg-variables: *"Stdin, Stdout, and Stderr are
  open Files pointing to the standard input, standard output, and standard error file
  descriptors"*.
- **Python**, verified, PEP 3105, https://peps.python.org/pep-3105/ (*"Status: Final"*,
  *"Python-Version: 3.0"*, *"Created: 19-Nov-2006"*): `def print(*args, sep=' ', end='\n',
  file=None)`; *"A call like: print(a, b, c, file=sys.stderr) will be equivalent to today's: print
  >>sys.stderr, a, b, c"*.
- **Zig**, verified, https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/debug.zig: `pub fn
  print(comptime fmt: []const u8, args: anytype) void`, *"Writes to stderr, ignoring errors"*, *"a
  low-level debugging primitive"*, *"Alternatively, use the higher-level `std.log`"*.
- **Swift, asked for and not added**, verified. The 2016 pitch,
  https://lists.swift.org/pipermail/swift-evolution/Week-of-Mon-20160704/023831.html (Saagar Jha,
  Jul 8, 2016, *"Addition of a standardError OutputStream"*): *"I was wondering if a standardError
  OutputStream could be added to the standard library,"*, *"Currently, it’s rather annoying to print
  to standard error, requiring either something low-level like fputs."* Nine years later,
  https://forums.swift.org/t/how-to-print-to-stderr-swift-6-1/79863 (May 13, 2025): Itai Ferber,
  *"I'm not aware of a more succinct way of getting access to `stderr` for use with `print`"*, with
  a wrapper over `FileHandle.standardError`; AlexanderM, *"Who wants to write up a PR for `struct
  _Stderr`?"*. No accepted Swift Evolution proposal for one was found by the searches below, which
  is a statement about the searches.

## 7. The statuses

- **Rust, a panic**, verified, https://doc.rust-lang.org/std/macro.panic.html, under its *Current
  implementation* heading: *"If the main thread panics it will terminate all your threads and end
  your program with code `101`."* The Reference, verified, https://doc.rust-lang.org/reference/panic.html:
  *"`unwind` — unwinds the stack and is potentially recoverable"*, *"`abort` –– aborts the process
  and is non-recoverable"*, *"The default for most targets is `unwind`"*; no exit status is stated
  there and 101 does not appear.
- **Go 2 and OCaml 2**: the runtime documentation and the runtime's source, question 2 and
  question 1; Go's specification states none.
- **C's `abort`**, verified, N1570 § 7.22.4.1: *"an implementation-defined form of the status
  unsuccessful termination"* is returned *"by means of the function call raise(SIGABRT)"*.
- **What a shell reports**, verified. POSIX,
  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html § 2.8.2: *"Otherwise,
  if the command terminated due to the receipt of a signal, the shell shall assign it an exit status
  greater than 128. The exit status shall identify, in an implementation-defined manner, which
  signal terminated the command."* Bash,
  https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html: *"When a command terminates
  on a fatal signal whose number is N, Bash uses the value 128+N as the exit status"*; *"Exit
  statuses fall between 0 and 255, though, as explained below, the shell may use values above 125
  specially."* SIGABRT is 6 in POSIX's `kill` table,
  https://pubs.opengroup.org/onlinepubs/9799919799/utilities/kill.html. 134 = 128 + 6 under Bash is
  this seat's arithmetic.
- **Windows**, verified. `abort`,
  https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/abort: *"If a non-default
  signal handler is set, abort calls raise(SIGABRT)"*; *"By default, in non-debug builds of desktop
  or console apps, abort then invokes the Windows Error Reporting Service mechanism"*; *"If the
  Windows error reporting handler isn't invoked, then abort calls _exit to terminate the process
  with exit code 3"*; the page does not name `__fastfail`. `__fastfail`,
  https://learn.microsoft.com/en-us/cpp/intrinsics/fastfail: *"User-mode fast fail requests appear
  as a second chance non-continuable exception with exception code 0xC0000409"*; *"no exception
  handlers are invoked because the program is expected to be in a corrupted state"*; *"Support for
  the native fast fail mechanism began in Windows 8."* `GetExitCodeProcess`,
  https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getexitcodeprocess:
  the status is a `DWORD`, one of *"The exit value specified in the ExitProcess or TerminateProcess
  function"*, *"The return value from the main or WinMain function of the process"*, *"The exception
  value for an unhandled exception that caused the process to terminate"*; *"an application should
  not use STILL_ACTIVE (259) as an error code"*. That the CRT's `abort` reaches `__fastfail` on its
  reporting path is on neither page: **unverified** here (the project's record, panel 174's brief,
  measured `0xC0000409` on the author's box).
- **A status past 255**, verified. POSIX `exit`,
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/exit.html: *"The value of status can be
  0, EXIT_SUCCESS, EXIT_FAILURE, or any other value, though only the least significant 8 bits (that
  is, status & 0377) shall be available from wait() and waitpid(); the full value shall be
  available from waitid() and in the siginfo_t passed to a signal handler for SIGCHLD."* Rust,
  https://doc.rust-lang.org/std/process/fn.exit.html: *"On most Unix-like platforms, only the eight
  least-significant bits are considered"*, and of `process::exit(0x0100);`, *"the exit code for this
  example will be `0` on Linux, but `256` on Windows"*; `ExitCode` (question 5): *"Numeric values
  used in this type don't have portable meanings, and different platforms may mask different
  amounts of them"*, built from a `u8`. Go, https://pkg.go.dev/os#Exit: *"For portability, the
  status code should be in the range [0, 125]"*, nothing about Windows or values outside it (when
  that sentence was added: a search found it in go1.16.1's source, **secondary**; its origin
  **unverified**). Erlang, https://www.erlang.org/doc/apps/erts/erlang.html, `halt/2`: *"On many
  platforms, the OS supports only status codes 0-255. A too large status code is truncated by
  clearing the high bits."* Python (question 5): *"Most systems require it to be in the range
  0–127, and produce undefined results otherwise"*. Zig (question 5): `main` returns `u8` or `!u8`.
  ShellCheck, https://www.shellcheck.net/wiki/SC2242: *"Can only exit with status 0-255. Other data
  should be written to stdout/stderr."*, *"Note in particular that `exit -1` is equivalent to `exit
  255`"*.
- **`sysexits.h`**, verified, https://man.freebsd.org/cgi/man.cgi?query=sysexits&sektion=3:
  *"This interface has been deprecated and is retained only for compatibility. Its use is
  discouraged."*; *"Error numbers begin at EX__BASE to reduce the possibility of clashing with other
  exit statuses"*; BUGS: *"This interface is not portable."*, *"The choice of an appropriate exit
  value is often ambiguous."*; HISTORY: *"The sysexits file first appeared in 4BSD."*
- **Whether a specification states what an abort exits with**: among the pages read, none fixes a
  number in a language's specification. C leaves it *"implementation-defined"*; Go's
  specification is silent and its runtime documentation says 2; Rust's Reference is silent and its
  library documentation says 101 under *Current implementation*; D's specification says *"a
  non-zero value"*. OCaml's manual was not searched for a status; its runtime's source says 2.

## 8. Bounds on a rendering

- **GDB**, verified, https://sourceware.org/gdb/current/onlinedocs/gdb.html/Print-Settings.html:
  `set print elements`, *"When GDB starts, this limit is set to 200."*; `set print characters`,
  *"When GDB starts, this limit is set to `elements`."*; `set print repeats`, *"The default
  threshold is 10."*; `set print max-depth`, *"Set the threshold after which nested structures are
  replaced with ellipsis"*, `{...}`; its default is not on the page: **unverified**.
- **LLDB**, verified,
  https://raw.githubusercontent.com/llvm/llvm-project/main/lldb/source/Target/TargetProperties.td:
  `max-children-count`, `DefaultUnsignedValue<24>`, *"Maximum number of children to expand in any
  level of depth."*; `max-children-depth`, `DefaultUnsignedValue<5>`, *"Maximum depth to expand
  children."*; `max-string-summary-length`, `DefaultUnsignedValue<1024>`, *"Maximum number of
  characters to show when using %s in summary strings."* **The depth bound moved**, verified,
  https://llvm.googlesource.com/llvm-project/+/1e7ec351c40af981e299743a955d71c7d70753a8%5E%21
  (Michael Buch, Jul 17, 2025): *"Deeply nested structs can be noisy, so Apple's LLDB fork sets the
  default to `4`"*, the diff `-DefaultUnsignedValue<0xFFFFFFFF>` `+DefaultUnsignedValue<4>`; and
  https://lists.llvm.org/pipermail/llvm-bugs/2026-April/139962.html (bug 192057, Apr 14, 2026),
  title only: *"Change to default value of `target.max-children-depth` setting broke repeating
  `memory read` commands"*. When it became 5: **unverified**. That `max-children-count` was once
  256: **secondary** (a search's reading of an LLVM cross-reference that refused the connection).
- **Node's `util.inspect`**, verified, https://nodejs.org/api/util.html: `depth` *"Default: `2`"*;
  `maxArrayLength` *"Default: `100`"*, *"v6.1.0 — The `maxArrayLength` option is supported now; in
  particular, long arrays are truncated by default"*; `maxStringLength` *"Default: `10000`"*;
  `breakLength` *"Default: `80`"*.
- **Python's `reprlib`**, verified, https://docs.python.org/3/library/reprlib.html: *"a means for
  producing object representations with limits on the size of the resulting strings"*; `maxlevel`
  *"The default is `6`"*; *"The default is `4` for maxdict, `5` for maxarray, and `6` for the
  others"*; `maxstring` *"The default is `30`"*; `maxlong` *"The default is `40`"*; `maxother` *"The
  default is `20`"*, while the constructor's signature on the same page says `maxother=30`.
- **pytest's bounded printer**, verified,
  https://raw.githubusercontent.com/pytest-dev/pytest/main/src/_pytest/_io/saferepr.py: *"repr.Repr
  that limits the resulting size of repr() and includes information on exceptions raised during
  the call"*; `DEFAULT_REPR_MAX_SIZE = 240`; a `__repr__` that raises is shown as
  `<[{exc_info} raised in repr()] {type(obj).__name__} object at 0x{id(obj):x}>`.
- **pytest's second printer** (panel 117's prediction), verified,
  https://raw.githubusercontent.com/pytest-dev/pytest/main/src/_pytest/_io/pprint.py: *"This module
  was imported from the cpython standard library (https://github.com/python/cpython/) at commit
  c5140945c723ae6c4b7ee81ff720ac8ea4b52cfd (python3.12). It has since been adapted to emit its
  output lazily as a stream of string chunks ... This lets ``pformat_lines`` stop formatting as
  soon as a line/char budget is reached, so a huge collection a caller is going to truncate anyway
  is never fully built. As a result this copy has diverged structurally from upstream and is no
  longer kept in sync with it."*
- **The test frameworks' bounds** (pytest 8 lines and 640 characters, `unittest` 640 characters,
  Zig's 16-row window) and Python `pprint`'s `depth`: questions 3 and 4.

## Panel 117's three, re-read

- **JEP 306**, verified, https://openjdk.org/jeps/306: *"JEP 306: Restore Always-Strict
  Floating-Point Semantics"*, *"Closed / Delivered"*, release *"17"*; *"This will restore the
  original floating-point semantics to the language and VM, matching the semantics before the
  introduction of strict and default floating-point modes in Java SE 1.2"*; the split *"stemmed
  from a bad interaction between the original Java language and JVM semantics"* and the *"x87
  floating-point co-processor instruction set"*; *"the technical motivation for having a default
  floating-point semantics different than strict is no longer present."*
- **N1426**, verified, https://www.open-std.org/JTC1/SC22/wg14/www/docs/n1426.pdf: WG14's
  *"Editor's Report"*, *"November 25, 2009"* (Larry Jones), listing under *"Newly Applied Content"*
  *"N1420 On The Removal of gets()"*.
- **P2186R2**, verified, https://open-std.org/JTC1/SC22/WG21/docs/papers/2021/p2186r2.html:
  *"Removing Garbage Collection Support"*, 2021-04-16, JF Bastien and Alisdair Meredith; *"removing
  (not deprecating) C++'s Garbage Collection support"*; *"the authors are not aware of any
  implementations of the strict pointer safety facility"*; EWG: *"Remove (not deprecate) garbage
  collection support in C++23."*
- None of the three is about what a failing program prints; they bear on § 12's history alone (a
  narrowing written with its condition, and the condition met in JEP 306's case).

## verdict

**approve** (advisory, no veto), with the three cautions of the argument and the departures
below named so that each is taken on purpose.

## argument

Each direction the proposal takes has shipped, kept precedent. A place written at the site:
Wirth's Oberon encodes it in the trap instruction; Rust since 1.46, C++20, Zig and GHC pass it. A
file named by its module, not the builder's path: Swift reversed its full path for privacy, size
and reproducibility; Zig's record is relative to its module. Sides written by the general printer
with escapes: Rust's `Debug`. A failing `main` written on stderr at status 1: Rust (its RFC's 2
amended to 1), Zig, Python's `sys.exit`. Cautions: Rust measured the passed place's size and added
a knob to strip it; pytest's private printer diverged from its source; Go refused a standard
handling as one choice among many.

## departures from precedent, to be taken deliberately

1. **The column of an `assert`**: the proposal names the asserted expression's start; OCaml names
   the `assert` keyword, and Rust's post shows the column where the macro begins. Nothing read
   names the expression.
2. **The status of a failing `main`**: 1 is what Rust shipped and what Zig and `sys.exit` give;
   RFC 1937's author argued for 2 (`grep` gives 1 a meaning) and lost before shipping. A status
   carried by the failure (M6) has Rust 1.61's `Termination` example as its precedent.
3. **A map in the order of its rendered text**: Node sorts whole rendered entries; Go sorts by
   key value and orders pointers and channels *"by machine address"*; Python's `pprint` sorts by
   key and falls back to *"the type name and then ... the obj ids"*. An order by a rendered *key*
   needs the tie-break the brief names; an order by the whole rendered entry, Node's, puts equal
   texts side by side, where their order cannot show. That last sentence is this seat's
   inference, unrun.
4. **`exit(code:)` past 255**: Rust, Erlang and POSIX document the truncation; Go and Python
   advise a range; Zig types `main`'s status as `u8` and Rust's `ExitCode` takes a `u8`;
   ShellCheck refuses it statically in shell. Among those read, none aborts at run time on an
   out-of-range code: that route would be new.
5. **An abort's status in the specification**: no specification read fixes a number (C, Go,
   Rust's Reference, D). Writing 134 or any number into the spec would be the first among them.

## prediction

**Routes someone reversed, and why** (all verified above): Swift reversed `#file`'s full path
(privacy, *"up to 5%"* code size, reproducible builds), and its Language Steering Group in 2024
reversed SE-0285's planned deprecation of `#fileID`; Rust amended RFC 1937's status 2 to 1 and its
`Display` to `Debug` before shipping; Rust 1.73 took the panic message out of quotes and onto its
own line, *"especially useful when the message is long, contains nested quotes, or spans multiple
lines"*; Go 1.12 replaced an unordered map print with a sorted one *"to ease testing"*; LLDB moved
its depth bound from unbounded to 4 in 2025, a bug was filed against the change in 2026, and it
reads 5 today; FreeBSD deprecated `sysexits`. **Not reversed after shipping, among those read**: a
passed place (Rust since 1.46, C++20, Zig, GHC, Wirth's Oberon) and a failing `main` (Rust since
1.26, Zig, Swift since 5.3).

**This seat's registered predictions, each scorable:**

1. **If L1 lands, the per-site records, not the file names, carry its size.** Measured at the
   landing commit by `seed/heroes.c`'s bytes three ways: without places, with them, and with every
   line and column written as 0 (Rust's Tock figures run that way: zeroing line and column saved
   6.6%, blanking file names 1.4%). And within two milestones of that landing, M-bounded-seed's
   profile is asked to drop or fuse the places, as Rust added `-Z location-detail`. Wrong if the
   file names are the larger share, or if no such request is filed by then.
2. **If S1 lands as a renderer emitted beside the lldb formatter with no single corpus both are
   tested on, the two notations differ again within two milestones** (panel 117's prediction,
   with pytest's fork of `pprint`, *"no longer kept in sync"*, as its precedent; finding 9 measured
   one difference in this tree already). Wrong if a value rendered by both reads the same at that
   close.
3. **M1, if adopted, is not reversed; it is extended.** The extension asked for is a status other
   than 1 carried by the failure (M6), as Rust stabilised `Termination` with `ExitCode` four years
   after 1.26. Wrong if M1 is withdrawn, or if no program asks a second status through it by the
   next sitting on `main`.

## condition

What would change this reading: a primary page showing a language that **shipped a passed place
and withdrew it** on its cost (Rust kept `track_caller` and made only the detail optional, behind
an unstable flag), or that **shipped a failing `main` and withdrew it**; a test framework whose
**assert-only renderer stayed one notation with its debugger's formatter** for five years (panel
117's own condition, unmet by anything read here); a **language specification that fixes an
abort's numeric status** (none found); and Rust's written reason for printing a failing `main`'s
error with `Debug` rather than `Display` (issue 48711 and PR 49162 were not read), which bears on
whether a failure line uses the escaping notation S1 proposes for sides.

## search

Every page above was fetched in this session (`WebFetch`), most of them after a `WebSearch` that
named them; quotations are what the fetch returned. The queries, in order: a stray first query,
`placeholder`, issued by mistake and disregarded; `"Announcing Rust 1.46.0" track_caller
Location::caller`; `rust "location-detail" unstable flag tracking issue binary size panic location
file line column`; `"Announcing Rust 1.73.0" cleaner panic messages "panicked at" assert_eq left
right`; `SE-0274 "#fileID" accepted with modifications Swift 5.3 core team decision`; `OCaml manual
uncaught exception "Fatal error: exception" exit code 2 Printexc`; `pytest "truncation_limit_lines"
"truncation_limit_chars" default 8 640 assertion output truncated`; `ziglang.org news Zig migrating
from GitHub to Codeberg`; `"Announcing Rust 1.26" "main can return a Result"`; `"Announcing Rust
1.0" blog.rust-lang.org 2015/05/15`; `Swift evolution pitch print to standard error stderr
TextOutputStream standardError stdlib`; `Rust panic process exit code 101 documented ...`; `lldb
"max-children-count" default changed 256 to 24 reason review`; `"P2186R2" removing garbage
collection support C++23`; `WG14 OR WG21 "N1426" paper`; `golang os.Exit "For portability, the
status code should be in the range [0, 125]" issue CL added`; `Project Oberon 2013 trap source
position encoded in instruction ORG.Mod Trap ORS.Pos ...` (it found nothing; Wirth's sources were
then read at ETH directly). The panel's own files were located through
`api.github.com/repos/heroes-lang/heroes/contents/docs/panel`, since this seat has no directory
listing tool.

**Swept, by language and source**: Rust (RFCs 1869, 1937, 2091; blog posts 1.0, 1.19, 1.26,
1.46, 1.61, 1.73; issues 43301, 48453, 70579, 70580; std docs for `assert_eq!`, `panic!`, `char`,
`backtrace`, `process::exit`, `ExitCode`, `Termination`; the Reference's panic chapter; the
unstable book; `library/std/src/process.rs`); Swift (SE-0274, SE-0281, SE-0285, the forum's
`#fileID` thread, `Assert.swift`, `ErrorType.swift`, the 2016 standard-error pitch, the 2025
forum thread); Zig (`lang.zig`, `builtin.zig` archival, `start.zig`, `log.zig`, `debug.zig`,
`testing.zig`, the language reference, the Codeberg announcement); Go (the specification whole,
`runtime`, `os`, `builtin`, `debug/gosym`, `cmd/go`, the 1.12 and 1.13 release notes, issues 27328,
34673, 44750 and their comments); C (C89 draft, C11 draft N1570, cppreference's `assert`); C++
(P1208R6, cppreference's `source_location`, P2186R2); GHC (`GHC.Stack`, the user's guide); OCaml
(the core library chapter, `printexc.c`); Python (`sys`, `unittest`, `pprint` docs and source,
`reprlib`, PEP 3105); pytest (`assert.html`, `truncate.py`, `_typing.py`, `saferepr.py`,
`pprint.py`); Node (`util` docs, `inspect.js`); Jest (`pretty-format` README); D (the function
chapter); Nim (`nimc`, `system`); Erlang (`erlang:halt/2`); Wirth (Project Oberon's `ORG.Mod`,
`ORS.Mod`, `System.Mod`); POSIX (`exit`, § 2.4.3, § 2.8.2, `kill`); Bash; Microsoft (`abort`,
`__fastfail`, `GetExitCodeProcess`); FreeBSD (`sysexits`); ShellCheck (SC2242); GCC (preprocessor
options, the GCC 8 changes page); binutils (`addr2line`); Apple (the crash-report symbolication
article); Breakpad; LLDB (`TargetProperties.td`, commit 1e7ec35, bug 192057); GDB (print
settings); Java (JEP 306); WG14 (N1426).

**Looked for and not found, or not read** (each a statement about this search, not about the
world): the exact text of a Go traceback; the exit status of a Swift `fatalError`; the GHC release
that introduced `HasCallStack`; the GCC release that added `-fmacro-prefix-map`; GDB's
`max-depth` default; the origin of LLDB's `max-children-count` of 24 and of `max-children-depth`
5; the origin of Go's `[0, 125]` sentence; Rust's written reason for `Debug` (issue 48711 and PR
49162 not read); whether the Windows CRT's `abort` reaches `__fastfail`; Crashpad; an accepted
Swift Evolution proposal for a standard-error stream; any language that aborts at run time on an
out-of-range exit code; any language specification that fixes an abort's status; any language
that reversed a passed place or a failing `main` after shipping. **Not searched at all**: Java's
and .NET's uncaught-exception statuses, Kotlin, Elixir, Haskell's `exitWith`, Pascal-P4, Cyclone,
Hylo, Idris, K/APL and cfront, whose ancestry this mandate names and which this question did not
reach.
