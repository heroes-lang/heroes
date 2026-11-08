# Panel 237, the historian's report

Seat: historian (advisory, no veto). Brief: `docs/panel/237-briefs/historian.md`, after
`00-shared.md` and `proposal.md`; also read: `237-reports/critic-briefs.md` and
`237-briefs/blind/README.md` (lane `p237`), and, in this worktree at `163912031`,
`spec/heroes-spec.md` lines 380 to 479 and `selfhost/cli/shadowed.hero` lines 225 to 254. Every page
cited below was read on the web on **2026-11-08** while this report was written. `verified` means the
page was opened that day and the line says only what it states; `unverified` means it could not be
read, and nothing rests on it. A sentence of mine that goes beyond a page is marked *my reading*.

## 1. Header search, and a file standing in for a library header

### The C standard, the oldest primary source

- **ISO C, N1570 (the C11 committee draft, 2011-04-12), §7.1.2 paragraph 3**: "If a file with the
  same name as one of the above < and > delimited sequences, not provided as part of the
  implementation, is placed in any of the standard places that are searched for included source
  files, the behavior is undefined." The list above it is the standard headers, `<stdint.h>` and
  `<stdbool.h>` among them. https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf (PDF page
  199, printed page 181), read 2026-11-08. **verified**. N1570 is the committee draft, not the
  published standard.
- **The same draft, §7.1.3 paragraphs 2 and 3**: "If the program declares or defines an identifier in
  a context in which it is reserved (other than as allowed by 7.1.4), or defines a reserved
  identifier as a macro name, the behavior is undefined." and "If the program removes (with #undef)
  any macro definition of an identifier in the first group listed above, the behavior is undefined."
  The first group is identifiers that begin with an underscore and either an uppercase letter or
  another underscore. Same URL, printed page 182, read 2026-11-08. **verified**. This is item 2's
  ground in the standard.
- **SEI CERT C, PRE04-C, "Do not reuse a standard header file name"**: "If a file with the same name
  as a standard header is placed in the search path for included source files, the behavior is
  undefined", citing C11 subclause 7.1.2; the compliant solution is "giving the local library a
  unique name", which makes "it apparent that the library used is not the original". Risk row:
  "PRE04-C Low Unlikely Yes No P2 L3".
  https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/recommendations/preprocessor-pre/pre04-c,
  read 2026-11-08 (the old wiki URL now redirects there). **verified**.
- **Static checkers that implement PRE04-C**: MathWorks Polyspace, "The issue occurs when you include
  a user-defined header file that has the same name as one of the standard library" headers,
  https://www.mathworks.com/help/bugfinder/ref/certcrec.pre04c.html; Axivion, message key
  `reused_std_name`, "User include files should not reuse the names of standard headers.",
  https://doc.qt.io/axivion/reference/analysis_configuration/CertC-PRE04.html. Both read 2026-11-08.
  **verified**.

### The compilers' search orders, and what they were for

- **GCC, Directory Options**: for the quote form, the current file's directory, then `-iquote`
  directories; then `-I` directories left to right; then `-isystem`; then the standard system
  directories; then `-idirafter`. You can "use -I to override a system header file, substituting
  your own version", since those directories are searched before the system header directories. A
  standard system directory also given with `-I` has "the -I option ... ignored", and is searched at
  its normal system position. https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html, read
  2026-11-08. **verified**. *My reading*: the override item 1 refuses is the documented purpose of
  `-I`, not a defect of it.
- **GCC, Environment Variables**: `CPATH` is treated as if given with `-I`, searched after the command
  line's `-I`; `C_INCLUDE_PATH`, `CPLUS_INCLUDE_PATH`, `OBJC_INCLUDE_PATH` are treated as `-isystem`,
  each for its own language, after the command line's `-isystem`. Command-line options "take
  precedence over environment variables, which in turn take precedence over the configuration of
  GCC." https://gcc.gnu.org/onlinedocs/cpp/Environment-Variables.html, read 2026-11-08.
  **verified**.
- **clang's man page, ENVIRONMENT**: `CPATH` "specifies additional (non-system) header search paths",
  "searched after paths specified with the -I<directory> option, but before any system header search
  paths"; `C_INCLUDE_PATH`, `OBJC_INCLUDE_PATH`, `CPLUS_INCLUDE_PATH`, `OBJCPLUS_INCLUDE_PATH`
  "specify additional system header file search paths". The page does not name
  `CCC_OVERRIDE_OPTIONS`. https://clang.llvm.org/docs/CommandGuide/clang.html, read 2026-11-08.
  **verified**.
- **clang's Command Line Reference**: `-idirafter` "Add directory to AFTER include search path";
  `-iquote` "Add directory to QUOTE include search path"; `-isystem` "Add directory to SYSTEM include
  search path"; `-isystem-after` "Add directory to end of the SYSTEM include search path".
  https://clang.llvm.org/docs/ClangCommandLineReference.html, read 2026-11-08 (its first 100,000
  characters). **verified**.
- **clang's `CCC_OVERRIDE_OPTIONS`**: read by the driver under the comment "Handle
  CCC_OVERRIDE_OPTIONS, used for editing a command line behind the scenes."
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang/tools/driver/driver.cpp, read
  2026-11-08. **verified**. Its edits (`#` silence, `^FOO` prepend, `+FOO` append, `s/XXX/YYY/`,
  `xOPTION` and `XOPTION` remove, `Ox`) are documented in the commit that moved it into clangDriver,
  Dave Lee, 2024-03-15 (PR 85425), which calls it "an escape hatch when there are issues that can be
  fixed by adding or removing clang flags."
  https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20240311/559241.html, read 2026-11-08.
  **verified**.
- **`#include_next`, GCC's own word**: "Include the _next_ file with this name"; "The use of
  #include_next can lead to great confusion. We recommend it be used only when there is no other
  alternative."; it "should not be used in the headers belonging to a specific program", only "to
  make global corrections along the lines of fixincludes."
  https://gcc.gnu.org/onlinedocs/cpp/Wrapper-Headers.html, read 2026-11-08. **verified**. The
  brief's refused folder and the critic's environment probe both use it.
- **A correction with a date, GCC 4.0**: "-I- has been deprecated. -iquote is meant to replace the
  need for this option." https://gcc.gnu.org/gcc-4.0/changes.html, read 2026-11-08. **verified**.
  The quote-only path is itself the repair of a coarser split.

### How build systems and languages that compile C meet the clash

- **CMake keeps the compiler's own directories off the command line**:
  `CMAKE_<LANG>_IMPLICIT_INCLUDE_DIRECTORIES`, "CMake does not explicitly specify these directories
  on compiler command lines for language <LANG>. This prevents system include directories from being
  treated as user include directories on some compilers, which is important for C, CXX, and CUDA to
  avoid overriding standard library headers."
  https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_IMPLICIT_INCLUDE_DIRECTORIES.html, read
  2026-11-08. **verified**. *My reading*: CMake protects the system directory's position; the page
  says nothing about a project file of a library header's name.
- **pkg-config strips the system directory**: `PKG_CONFIG_ALLOW_SYSTEM_CFLAGS`, "Don't strip
  -I/usr/include out of cflags." (Red Hat's man page as mirrored)
  https://www.unix.com/man_page/redhat/1/pkg-config, read 2026-11-08. **verified** for the line; the
  page gives no reason, and the reason is **unverified** (freedesktop's guide and Kitware's issue
  25228 both answered 403).
- **Meson**: `implicit_include_directories` "Controls whether Meson adds the current source and build
  directories to the include path" (since 0.42.0, default `true`); a dependency's `include_type` is
  `'preserve'` (the default), `'system'` or `'non-system'`, "System dependencies may be handled
  differently on some platforms", for example "using -isystem instead of -I"; `include_directories`
  with `is_system: true` uses `-isystem` and reverses the order.
  https://mesonbuild.com/Reference-manual_functions.html, read 2026-11-08. **verified**.
- **Go's cgo**: "The cgo tool will always invoke the C compiler with the source file's directory in
  the include path; i.e. -I${SRCDIR} is always implied." https://pkg.go.dev/cmd/cgo, read
  2026-11-08. **verified**. Nothing on that page refuses a package file named like a libc header
  (a claim about the page, not about cgo's code).
- **Zig's build system**: `addIncludePath` is `-I`, `addSystemIncludePath` `-isystem`,
  `addAfterIncludePath` `-idirafter`, and a dependency step's installed headers (`.other_step`) `-I`,
  in `appendZigProcessFlags`. https://raw.githubusercontent.com/ziglang/zig/master/lib/std/Build/Module.zig,
  read 2026-11-08. **verified**. Zig issue 13528 (on `zig.h` sitting beside C language headers):
  **unverified**, two fetches timed out; nothing rests on it.
- **Rust's `cc` crate**: `include()` "Add a directory to the `-I` or include path for headers"; the
  `Build` page names neither `-isystem` nor `-idirafter`. It reads `CC`, `CFLAGS`, `CXXFLAGS`,
  `CRATE_CC_NO_DEFAULTS` and their target-scoped forms from the environment.
  https://docs.rs/cc/latest/cc/struct.Build.html and https://docs.rs/cc/latest/cc/, read
  2026-11-08. **verified**.
- **Python, the same class one level up** (a script's folder before the standard library):
  3.11 added "a -P command line option and a PYTHONSAFEPATH environment variable, which disable the
  automatic prepending to sys.path of the script's directory", which "avoids unintentionally or
  maliciously shadowing modules" (gh-57684, Victor Stinner), an opt-in reorder with the default
  unchanged. https://docs.python.org/3/whatsnew/3.11.html, read 2026-11-08. **verified**.

*My reading of this subsection, read not run*: of the eight systems read, none refuses a project's
or package's file named like a library header, and none moves the project's folder after the
system's by default. Every reorder read is either a filter on the system's own directory (CMake,
pkg-config) or an option the user turns on (Meson's `include_type`, Zig's `addAfterIncludePath`,
Python's `-P`). Item 1's refusal departs from all of them, and stands with C11 §7.1.2p3 and CERT
PRE04-C.

## 2. A package's compiler flags held to a list

- **Go, CVE-2018-6574**: "The "go get" command with cgo is vulnerable to remote command execution by
  leveraging the gcc or clang plugin feature." Fixed in 1.8.7 and 1.9.4.
  https://go.googlesource.com/vulndb/+/703236d8e94eeb3dbc7521f7bd113b9c0d1df43a/data/reports/GO-2022-0201.yaml,
  read 2026-11-08. **verified**.
- **The fix commit**: "cmd/go: accept only limited compiler and linker flags in #cgo directives",
  Russ Cox, authored 2018-02-01, committed 2018-02-07; "Both gcc and clang accept an option
  -fplugin=code.so to load a plugin"; the restrictions "can be adjusted by the environment variables
  CGO_CFLAGS_ALLOW, CGO_CFLAGS_DISALLOW"; "Fixes #23672, CVE-2018-6574."
  https://go.googlesource.com/go/+/1dcb5836ad2c60776561da2923c70576ba2eefc6, read 2026-11-08.
  **verified**. (Issue 23672 itself: **unverified**, GitHub answered 502 and timed out.)
- **Go 1.10's notes**: "Options specified by cgo using #cgo CFLAGS and the like are now checked
  against a list of permitted options. This closes a security hole in which a downloaded package uses
  compiler options like -fplugin to run arbitrary code on the machine where it is being built."
  https://go.dev/doc/go1.10, read 2026-11-08. **verified**.
- **Go wiki, InvalidFlag**: the safelist "is new in releases 1.8.7, 1.9.4, and 1.10, and all
  subsequent releases"; a blocklist was refused because "all existing Go releases will become
  immediately vulnerable". https://go.dev/wiki/InvalidFlag, read 2026-11-08. **verified**.
- **cmd/cgo**: "For security reasons, only a limited set of flags are allowed, notably -D, -U, -I,
  and -l."; and, for the environment: "Flags obtained from environment variables are not subject to
  the security limitations described above." https://pkg.go.dev/cmd/cgo, read 2026-11-08.
  **verified**.
- **What Go allows of `-D` and `-U`**: `validCompilerFlags` holds
  `-D([A-Za-z_][A-Za-z0-9_]*)(=[^@\-]*)?` and `-U([A-Za-z_][A-Za-z0-9_]*)`; pkg-config's output is
  checked by `checkPkgConfigFlags` against `validPkgConfigFlags`, the environment overrides off.
  https://go.dev/src/cmd/go/internal/work/security.go, read 2026-11-08. **verified**. *My reading of
  the two regexps, not run*: they bound the name to C's identifier shape and the value away from `@`
  and `-`, and name no identifier, so `-D_FORTIFY_SOURCE` matches them. Go judges a flag's shape;
  item 2 judges a name, a different axis.
- **SwiftPM**: `unsafeFlags`, "Swift Package Manager can't safely determine if the build flags have
  any negative side effect on the build"; "As some build flags can be exploited for unsupported or
  malicious behavior, you can't use products with unsafe build flags as dependencies in another
  package." (PackageDescription 5.0)
  https://developer.apple.com/documentation/packagedescription/cxxsetting/unsafeflags(_:_:).md, read
  2026-11-08. **verified**. Its `define` "Defines a value for a macro" and states no restriction on
  the name. https://developer.apple.com/documentation/packagedescription/csetting/define(_:to:_:).md,
  read 2026-11-08. **verified**. SE-0238 (Implemented, Swift 5.0): "Products that contain a target
  which uses an unsafe flag will be ineligible to act as a dependency for other packages."
  https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0238-package-manager-build-settings.md,
  read 2026-11-08. **verified**.

*My reading*: both precedents restrict **what a dependency may hand the C compiler**, and Go states
the line in so many words: a package's flags are checked, the environment's are not. Neither judges
a `-D`'s name; C11 §7.1.3p2 is the only source read that does.

## 3. SQLite's word on closing, and how bindings bind it

- **SQLite, `sqlite3_close` and `sqlite3_close_v2`**: "If the database connection is associated with
  unfinalized prepared statements, BLOB handlers, and/or unfinished sqlite3_backup objects then
  sqlite3_close() will leave the database connection open and return SQLITE_BUSY." `sqlite3_close_v2`
  "returns SQLITE_OK regardless, but instead of deallocating the database connection immediately, it
  marks the database connection as an unusable "zombie"". "The sqlite3_close_v2() interface is
  intended for use with host languages that are garbage collected, and where the order in which
  destructors are called is arbitrary." https://www.sqlite.org/c3ref/close.html, read 2026-11-08.
  **verified**.
- **When `_v2` came**: SQLite 3.7.14, 2012-09-03, "Added the sqlite3_close_v2() interface."
  https://www.sqlite.org/releaselog/3_7_14.html, read 2026-11-08. **verified**.
- **Python's `sqlite3` (CPython `Modules/_sqlite/connection.c`)**: `connection_close` calls
  `(void)sqlite3_close_v2(db);` under the comment "The v2 close call always returns SQLITE_OK if
  given a valid database pointer (which we do), so we can safely ignore the return value".
  https://raw.githubusercontent.com/python/cpython/main/Modules/_sqlite/connection.c, read
  2026-11-08. **verified**.
- **Go's `mattn/go-sqlite3`**: `func (c *SQLiteConn) Close() error` calls
  `rv := C.sqlite3_close_v2(c.db)` and returns `lastError(c.db)` when `rv != C.SQLITE_OK`.
  https://raw.githubusercontent.com/mattn/go-sqlite3/master/sqlite3.go, read 2026-11-08.
  **verified**.
- **Rust's `rusqlite`**: `InnerConnection::close` calls `ffi::sqlite3_close(self.db)` and nulls the
  handle only `if r.is_ok()`.
  https://raw.githubusercontent.com/rusqlite/rusqlite/master/src/inner_connection.rs, read
  2026-11-08. **verified**. The public `Connection::close` is
  `pub fn close(self) -> Result<(), (Self, Error)>`, "functionally equivalent to the Drop
  implementation for Connection except that on failure," it returns the connection with the error;
  "Will return Err if the underlying SQLite call fails." (rusqlite 0.41.1)
  https://docs.rs/rusqlite/latest/rusqlite/struct.Connection.html, read 2026-11-08. **verified**.
- **Node's `better-sqlite3`**: `Database::CloseHandles` closes every statement and backup, then
  `int status = sqlite3_close(db_handle); assert(status == SQLITE_OK);`.
  https://raw.githubusercontent.com/WiseLibs/better-sqlite3/master/src/objects/database.cpp, read
  2026-11-08. **verified**.

*My reading*: the two bindings in garbage-collected hosts call `_v2`, the host SQLite's page names;
the two that order the end themselves call `sqlite3_close`, one after finalizing everything, one
(rusqlite) by **consuming the connection and handing it back on failure**, which is fence B's
`consumes ... when 0` in another language's words.

### A close that can refuse, with no `_v2` beside it

- **Rust std, `BufWriter::into_inner`**: `pub fn into_inner(self) -> Result<W,
  IntoInnerError<BufWriter<W>>>`; `IntoInnerError` is "An error returned by BufWriter::into_inner
  which combines an error that happened while writing out the buffer, and the buffered writer object
  which may be used to recover from the condition." https://doc.rust-lang.org/std/io/struct.BufWriter.html
  and https://doc.rust-lang.org/std/io/struct.IntoInnerError.html, read 2026-11-08. **verified**.
  And `File`: "Errors detected on closing are ignored by the implementation of Drop. Use the method
  sync_all if these errors must be manually handled."
  https://doc.rust-lang.org/std/fs/struct.File.html, read 2026-11-08. **verified**.
- **The opposite shape, in the C standard: `fclose`**: "Whether or not the call succeeds, the stream
  is disassociated from the file and any buffer set by the setbuf or setvbuf function is
  disassociated from the stream"; it "returns zero if the stream was successfully closed, or EOF if
  any errors were detected." N1570 §7.21.5.1, printed page 304, same URL as §1, read 2026-11-08.
  **verified**.
- **And in POSIX on Linux: `close(2)`**: "Retrying the close() after a failure return is the wrong
  thing to do"; "on many implementations, close() must not be called again after an EINTR error, and
  on at least one, close() must be called again"; "the Linux kernel always releases the file
  descriptor early in the close operation". https://man7.org/linux/man-pages/man2/close.2.html,
  read 2026-11-08. **verified**.
- **`pthread_mutex_destroy`** (named in the critic's finding 13): POSIX Issue 8 (IEEE Std
  1003.1-2024) lists no `[EBUSY]`; "Attempting to destroy a locked mutex, or a mutex that another
  thread is attempting to lock, results in undefined behavior."; Issue 7's change history: "The
  [EBUSY] error for a locked mutex ... is removed; this condition results in undefined behavior." The
  rationale still recommends failing with `[EBUSY]`.
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_mutex_destroy.html, read
  2026-11-08. **verified**. A task built on it would teach a binding of undefined behaviour; the
  reader test's R2 uses a store library of its own instead (`blind/README.md`).

*My reading, for question 7*: whether a failed close leaves the handle alive is a fact of each
function's own documentation, and the documents read disagree: `sqlite3_close` and rusqlite's
`close` keep it; `fclose` (C11) and `close(2)` on Linux end it whatever they return; one function
(`close(2)`) differs by platform. So `when 0` is right for `sqlite3_close` and wrong for `fclose`.
The spec's sentence (line 473) defines `when 0` as "the result that means the call did what it was
handed"; `fclose` returning `EOF` is a documented case where the call failed and still ended the
handle, so that wording does not say which side `fclose` falls on. That is a question for the
spec-warden, not a finding about Heroes' compiler.

## 4. Diagnostics that name a file standing in

- **Python 3.13**: "A common mistake is to write a script with the same name as a standard library
  module." The message: `AttributeError: module 'random' has no attribute 'randint' (consider
  renaming '/home/me/random.py' since it has the same name as the standard library module named
  'random' and prevents importing that standard library module)` (gh-95754, Shantanu Jain).
  https://docs.python.org/3/whatsnew/3.13.html, read 2026-11-08. **verified**. It names the file by
  its full path, the module it stands in for, the effect, and the repair in prose; it is told when
  the shadowing has already caused an error, not before.
- **clang, `-Wshadow-header`**: `warn_header_shadowing`, "multiple candidates for header '%0' found;
  directory '%1' chosen, ignoring others including '%2'", in group `ShadowHeader`, `DefaultIgnore`.
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/DiagnosticLexKinds.td,
  read 2026-11-08. **verified**. Merged 2025-12-03 (PR 162491,
  https://github.com/llvm/llvm-project/pull/162491, read 2026-11-08, **verified**); Clang 22.1.0's
  notes: "A new warning -Wshadow-header has been added to detect when a header file is found in
  multiple search directories (excluding system paths)."
  https://releases.llvm.org/22.1.0/tools/clang/docs/ReleaseNotes.html, read 2026-11-08.
  **verified**. The test says "System headers are ignored."
  https://llvm.googlesource.com/llvm-project/clang/+/27d389b54701f2e9a03e4f87c3f3de42a0c28d85/test/Preprocessor/header-shadowing.c,
  read 2026-11-08. **verified**. It names both directories and offers no repair. Whether it fires
  for an `-I` file shadowing an SDK header is **unrun** (this seat has no shell); the release note's
  "excluding system paths" suggests not, a question for the compiler-engineer.
- **CERT's checkers** (§1): Axivion's "User include files should not reuse the names of standard
  headers." **verified**.
- **GCC**: a search found no GCC warning for a same-name header; GCC bug 16358 surfaced and was not
  opened, so it is **unverified**.

*My reading, for question 4*: the shadowing messages read name the file and what it hides. Python's
alone gives a repair, and it gives it as prose ("consider renaming"), not as an edit a tool applies.
None of them offers a machine-applicable fix, which matches the landed refusal's note and no `Fix`.

## Candidate lines for the sitting's `## Prior art`

Each line says only what its page states. Whether a line is `Taken from` or `Reached first by` is the
sitting's call: items 1 and 2 landed at `96d42c250` before this report, and item 3's reading is
still open.

- **Reached first by ISO C (N1570)**: [§7.1.2p3](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): a file with a standard header's name, not provided by the implementation, placed in the standard places searched for included source files, is undefined behaviour.
- **Reached first by SEI CERT C**: [PRE04-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/recommendations/preprocessor-pre/pre04-c): do not reuse a standard header file name; the compliant solution gives the local library a unique name.
- **Reached first by ISO C (N1570)**: [§7.1.3p2-3](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): defining a reserved identifier as a macro name, or removing with `#undef` a macro of the first reserved group, is undefined behaviour.
- **Reached first by Go**: [cmd/cgo](https://pkg.go.dev/cmd/cgo): the flags a package's `#cgo` directives pass are limited to an allowed set for security reasons, and flags from environment variables are not subject to that limit.
- **Reached first by Swift Package Manager**: [unsafeFlags](https://developer.apple.com/documentation/packagedescription/cxxsetting/unsafeflags(_:_:).md): products with unsafe build flags cannot be used as dependencies in another package.
- **Reached first by Python 3.13**: [What's New](https://docs.python.org/3/whatsnew/3.13.html): an error message names the script that has a standard library module's name and says to consider renaming it.
- **Taken from (or Reached first by) SQLite**: [sqlite3_close](https://www.sqlite.org/c3ref/close.html): with unfinalized statements `sqlite3_close` leaves the connection open and returns `SQLITE_BUSY`; `sqlite3_close_v2` is intended for garbage-collected host languages.
- **Reached first by rusqlite**: [Connection::close](https://docs.rs/rusqlite/latest/rusqlite/struct.Connection.html): `close(self) -> Result<(), (Self, Error)>` gives the connection back with the error when the SQLite call fails.

## argument

No build tool read (CMake, Meson, Zig, Go's cgo, the `cc` crate) refuses a project file named like a
library header, and none moves a project's folder after the system's by default. The refusal departs
from them deliberately, on older ground: C11 §7.1.2p3 calls such a file undefined behaviour, and CERT
PRE04-C says rename. Go draws its flag line at who supplies the flag (a package's flags are checked,
the environment's trusted); Heroes draws it at the effect, a departure §4.19 should state. SQLite's
page makes line A false for `sqlite3_close` and intends `_v2` for garbage-collected hosts. `fclose`
and Linux `close(2)` end the handle even when they fail, so `when 0` is each function's own fact.

(116 words, counted by hand.)

## search

How it was done: WebSearch for discovery, then WebFetch of every page cited, each on 2026-11-08; the
N1570 PDF's pages 199 to 201 and 322 to 324 were read as images. No shell ran in this seat: no `grep`
of `docs/panel/` or of the issues was made here, so a sitting before this one that already ruled on
these precedents is not excluded by this report.

- **Queries made**: `clang documentation CPATH C_INCLUDE_PATH environment variables "-idirafter"
  "-iquote" search order`; `CCC_OVERRIDE_OPTIONS clang driver environment variable documentation`;
  `"applyOverrideOptions" clang Driver.cpp "CCC_OVERRIDE_OPTIONS"`; `SEI CERT C PRE04-C "Do not reuse
  a standard header file name"`; `clang "-Wshadow-header" warning multiple candidates for header`;
  `clang release notes "-Wshadow-header" added diagnostic`; `CVE-2018-6574 cmd/go arbitrary code
  execution go get cgo flags Go 1.9.4 1.8.7 security fix`; `PKG_CONFIG_ALLOW_SYSTEM_CFLAGS "Don't
  strip -I/usr/include" pkg-config man page`; `zig build addAfterIncludePath addSystemIncludePath
  addIncludePath idirafter std.Build`; `zig cImport user header same name as libc header shadows
  include path issue github ziglang`; `nim C backend nimcache generated C file name collides with
  system header issue github nim-lang`; `better-sqlite3 database.cpp CloseHandles sqlite3_close
  finalize statements source`; `swift package manager unsafeFlags "ineligible for use by other
  packages"`; `"unsafeFlags" "As the usage of the word" Swift Package Manager`; `gcc warning user
  header shadows system header same name "include" diagnostic option`.
- **Swept and read**: ISO C N1570 (§7.1.2, §7.1.3, §7.21.5.1); GCC's Directory Options, Environment
  Variables, Wrapper Headers, 4.0 changes; clang's man page, Command Line Reference, `driver.cpp`,
  `DiagnosticLexKinds.td`, PR 162491, the header-shadowing test, 22.1.0 release notes, the
  cfe-commits mail of PR 85425; CERT PRE04-C, Polyspace, Axivion; CMake; pkg-config (Red Hat man
  page); Meson; Go (cmd/cgo, `security.go`, go1.10 notes, wiki InvalidFlag, vulndb GO-2022-0201, fix
  commit `1dcb5836`); Zig's `std/Build/Module.zig`; Rust's `cc` crate; SwiftPM (`unsafeFlags`,
  `define`, SE-0238); Python 3.11 and 3.13 What's New; SQLite (`close`, `finalize`, release 3.7.14);
  CPython `connection.c`; `mattn/go-sqlite3`; rusqlite (`inner_connection.rs`, docs.rs); better-sqlite3
  `database.cpp`; Rust std (`BufWriter`, `IntoInnerError`, `File`); Linux `close(2)`; POSIX
  `pthread_mutex_destroy`.
- **Looked for and not read**: Go issue 23672 (GitHub 502, then timeout; the fix commit and cmd/cgo
  stand in); Zig issue 13528 (two timeouts); pkg-config's reason for stripping `-I/usr/include`
  (freedesktop guide 403, Kitware issue 25228 403); GCC bug 16358 (not opened); Apple's HTML page for
  `unsafeFlags` (rendered empty; its `.md` form read). **Nim**: the one query above found no Nim issue
  or page on a generated or user file named like a C library header, so Nim is not in this report.
  That says nothing about whether such a page exists.
- *Heroes of code* was not used as a source; no line here rests on it.
- `sqlite3_finalize`'s page (https://www.sqlite.org/c3ref/finalize.html) was read: it says the
  function is "called to delete a prepared statement" and returns the most recent evaluation's error
  code. It does not say in so many words that the statement is deleted whatever is returned, so I
  do not cite it as a fails-yet-ends case.

## verdict, and what it rests on

- **verdict: approve (advisory)**. Item 1 as a **refusal**, not a reorder, and not both. Item 2 as
  landed. Item 3 as **B or D, not A and not C**. On the environment: GCC's page treats `CPATH` as
  `-I` and `C_INCLUDE_PATH` as `-isystem`, both ahead of the standard system directories in its
  order; clang's page puts `CPATH` "before any system header search paths" (§1). *My reading*: by
  the compilers' own documents a `CPATH` folder is a folder named for the build, inside the line
  item 1 draws. `CCC_OVERRIDE_OPTIONS` is a driver escape hatch documented only in source, which is
  Go's "who set it" line. Whichever way the sitting rules, §4.19 should say which line it drew,
  since Go drew the other one.
- **design.md**: §1.12 (robustness: the program the refusal stops has undefined behaviour by C11
  §7.1.2p3); §4.17 (the words, measured against Python 3.13's and clang 22's); §4.19 (the C
  boundary and its copy of the fence).
- **cost or delta measured**: none. This seat ran no command on the tree, only web reads, and rests
  no verdict on a number of the brief's. The readline and poppler collision behind "not a reorder"
  is the critic's finding 11, **not re-run here**.
- **prediction**: in this sitting's reader test, **arm C is not preferred over A on R2** (the store
  library with no `_v2`) under the registered rule (silent lower at one-sided Fisher p < 0.05, correct
  not lower), so the runner reports C as *a lesson that does not carry*, whatever C scores on R1.
  Ground: the two bindings read that avoid the refusing close (Python's and Go's) do it by calling
  `_v2`, and R2 has no `_v2` to call. Scored by the reader test's R2 sessions under arms A and C (18
  each), `blind/README.md` § The decision rule. Falsified if C is preferred over A on R2.
- **condition**: any of these would change my reading, so the coordinator knows what to look for.
  For item 1, a language or build tool that once refused or reordered such a file by default and gave
  it up for a measured cost. For item 3, an ownership-tracking binding that chose `_v2` for a stated
  reason other than garbage collection, or a SQLite page recommending `_v2` to all new code. For the
  environment, a tool that clears `CPATH` for its compiler and kept doing so.
- **a paid run worth naming, for the coordinator to decide**: R1 and R2 test the carry to a close
  that refuses. Nothing tests over-application to a close that fails and still ends, the `fclose`
  shape above. A third task of that shape, 4 arms × 6 sessions × 1 model, is at most 24 × 0.45 =
  10.80 USD at the per-session cap. Not run, and not asked for by this seat's brief.
- **housekeeping**: a stray file, `docs/panel/237-reports/historian.argument.tmp` in this worktree,
  was written by mistake while drafting and holds one placeholder line; this seat has no tool that
  deletes, so the coordinator should not copy it and should remove it.
