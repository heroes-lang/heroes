# Panel 189, the historian's report

Seat: historian (advisory, no veto). Brief: `docs/panel/189-briefs/historian.md`,
read with `00-shared.md` and `00-facts.md`. Panel 087, the critic's first pass
and the llm-ergonomist's brief were read for the repository's own lineage and
for the experiment my prediction is scored by.

**The clock.** This seat has Read, Write, WebSearch and WebFetch and no shell,
so no time below was read from `date`. Every source is dated **2026-10-04**,
the session's own date as the environment states it; no time of day is
claimed for any reading. The brief's rule (*every time you write is read from
`date`*) cannot be met by this seat, and this paragraph says so rather than
estimating around it.

**The discipline.** Every precedent carries the URL I fetched or the search
result I read, marked **verified** (I read the words in the page, the source
file, or the PDF page image) or **unverified**. A quotation appears only where
I saw its words. The fetch tool summarises pages before I see them; where its
summary drew a conclusion the quoted code did not support, I say so and keep
the code. **The author's book was not used in this sitting, as a route or
otherwise.**

**Corrected before submission**, found on rereading my own draft. Each is
repaired below.

- Its argument counted 123 words while saying 117.
- It gave Unicode's Tables 3-8 to 3-11 as 34 bytes; they are four rows of
  nine, 36.
- It cited F8 for two details that are the critic's report (its item 17, its
  Q6 table).
- It stated a negative (*no flag disables it*) as a fact.
- It called Erlang's false release-candidate message a cost of the retry
  route, which came after it.
- It gave clang's installed-base reason to GCC as well, whose reason P2295
  calls unknown.
- It described Swift as choosing a *replacement* rule. What I read shows how
  far Swift advances past a bad sequence, not what it substitutes.
- It gave a reason for the lineage languages it did not search.

## For the synthesis (the charter's structure)

- **verdict: approve** (advisory), with four conditions drawn from precedent:
  the message's words name UTF-8 and the byte; no route hands a replaced text
  to a writer; UTF-16 is named only where the bytes prove it; the new
  offset-reporting check is tested against an independent validator.
- **precedents** (each in § The evidence, with its source):
  - Rust 1.86, 2025: the first bad byte, one error per file (verified).
  - CPython: the first bad byte, one error per file, PEP 263 named in the
    message (verified).
  - Go (gc and `go/scanner`), Swift, javac: every occurrence reported, each
    deciding where one bad sequence ends (verified).
  - clang and GCC: a bad byte in a comment accepted by default (verified).
    clang's stated reason is disruption of existing code; GCC's is unknown
    even to P2295 (verified).
  - C++23's P2295: always diagnose, comments included; SG16 polled it
    8-2-0-0-0 (verified).
  - Zig: validation removed in 2024 after a fuzzer crashed it (verified).
  - grep: *binary* for encoding errors drew a bug titled *wrongly classifies
    a valid C source file* (verified).
  - Rust's `VarError`: not Unicode is a different state from not present
    (verified).
- **argument** (119 words, counted by hand): The proposal is the shape the most
  recently revised front ends share: Rust 1.86 and CPython refuse the file at
  its first bad byte, naming byte and position; Go, Swift and javac report
  more, each choosing where one bad sequence ends, a rule Unicode documents as
  practice, not requirement. The departures are deliberate and precedented:
  `not_text` shares the word a program sees, as rustc's headline shares
  `read_to_string`'s, because a self-hosted compiler reads through its own
  library. The comment exemption (clang's, by its own commit) serves an
  installed base Heroes lacks (0 of 1,910 files), against SG16's 8-2-0-0-0
  poll. The words must name UTF-8: every encoding refusal I read names the
  encoding it expected, and grep's *binary* drew a bug.
- **condition**: the precedents that would change my reading are listed under
  each question and gathered in § What would change this reading.

## Per question

### Q1. The code and its class

**Verdict (advisory): approve `not_text` as the code, on condition that the
words say UTF-8; the refusal covers comments; it is not a thesis rule.**

- **No compiler I read names this state *not text*.** By the bytes or the
  encoding:
  - gc: *invalid UTF-8 encoding*; `go/scanner`: *illegal UTF-8 encoding*.
  - Swift: *invalid UTF-8 found in source file*.
  - clang: *source file is not valid UTF-8*.
  - rustc: *stream did not contain valid UTF-8* and *byte `193` is not valid
    utf-8*.
  - CPython: *Non-UTF-8 code starting with ...*.
  - GHC: *lexical error (UTF-8 decoding error)*.
  - Ruby: *invalid multibyte char (UTF-8)*.
  - javac: *unmappable character (0xE9) for encoding UTF-8*.
  - MSVC's C4828: *illegal in the current source character set (codepage
    65001)*.

  The internal names are by the bytes: Swift `lex_invalid_utf8`, clang
  `err_invalid_utf8` and `-Winvalid-utf8`, GCC `-Winvalid-utf8`. So arm D's
  `invalid_utf8` is the field's usual identifier.
- **Naming by the file has one compiler precedent, and a cautionary one
  beside it.** C# CS2015, *"'file' is a binary file instead of a text file"*,
  fires on two consecutive NULs, not on an encoding error. GNU grep 2.21
  (2014-11-23) began treating a file *"improperly encoded for the current
  locale"* as binary. Red Hat bug 1219141 (2015-05-06) is titled *"grep
  wrongly classifies a valid C source file as a binary file"*: `file` called
  it *"C source, ISO-8859 text"*. Eric Blake's answer is the naming lesson in
  one line: *"A text file, by POSIX definition, is one in which ALL bytes
  comprise valid characters in the current locale encoding. A file may be
  text in one locale, and binary in another."*

  *Not text* is true relative to an encoding and reads as false to an author
  whose file is text in another one. Heroes has one encoding and no locale,
  so the code is unambiguous inside the language; the message is where the
  relativity must be spelled out. The blind arms' drafted words already do
  (*"is not UTF-8, and a `.hero` file is UTF-8 text"*).
- **One word for a program's failure and a compiler's diagnostic has a direct
  precedent, for the same structural reason.** rustc reads source through
  `file.read_to_string(...)` (`rustc_span/src/source_map.rs`, verified) and
  prints *couldn't read `{path}`: {e}*. A Rust program calling
  `read_to_string` on a non-UTF-8 file gets the same *"stream did not contain
  valid UTF-8"* (users.rust-lang.org, 2017-04-02, verified). Python separates
  the two: a program gets `UnicodeDecodeError`, the compiler `SyntaxError:
  Non-UTF-8 code ...`.

  Heroes' compiler, like rustc, reads through its own language's library,
  whose `read_file` already fails `not_text` (F2, F3). Sharing the word is
  the Rust shape, and it completes panel 087's reversal condition (*a named
  reader ... plus a golden that fires the arm*,
  `docs/panel/087-the-guard-that-could-not-fire.md`, § Author's verdict, read
  in the trunk).
- **The class: a refusal of the input, not a thesis rule.** Among the strict
  front ends I read (Go, Swift, Rust, CPython), I found no flag that disables
  the refusal. That is a negative from the code and documents read, not a
  search of every flag. CPython's way out is a declaration that changes the
  encoding, not a relaxation. The language documents state it as a condition
  of the input:
  - the Rust Reference: *"It is an error if the file is not valid UTF-8"*;
  - C++23: *"shall be a well-formed UTF-8 code unit sequence"*;
  - Unicode's C10: a process *"shall treat ill-formed code unit sequences as
    an error condition"*.

  The lenient compilers are lenient by declaring the file to be in another
  encoding (P2295's acknowledgments: *"invalid UTF-8 can be supported by
  being considered a different encoding than UTF-8"*), not by relaxing a
  check. Advisory reading: `--permissive` keeps it. Part 11's question is the
  thesis seats', not a precedent's.
- **Route listed and refused by precedent: accept a bad byte in a comment.**
  - Who does it: clang by default (an `Extension`, ignored unless
    `-pedantic`), GCC by default, Zig on master (by regression, against its
    own documented rule), and Nim (no validation anywhere).
  - Why clang does it, in its own commit: *"The warning is off by default as
    its likely to be somewhat disruptive otherwise"*, which is an installed
    base. F8 measures Heroes' as 0 of 1,910 files.
  - GCC's reason is not recorded where I looked. P2295 says of it, *"We
    don't know if this is intentional."*
  - Against it: P2295's argument, *"If the compiler detects invalid
    characters in comments, there may be undetectable mojibake in string
    literals, which would lead to runtime bugs"*, and SG16's poll for a mode
    that diagnoses ill-formed UTF-8 *"regardless of whether the
    ill-formedness is located in comments, header names or string
    literals"*, 8 strongly for, 2 for, 0 neutral, 0 against.
  - The one reason on record for this route is absent here.
- **Route listed and refused by precedent: an encoding declaration or a
  transcoding.**
  - Every declaration I read belongs to a language with sources in older
    encodings: PEP 263 (Python 2.3, 2001), Ruby's magic comment, Erlang's
    coding comment, javac's `-encoding`, csc's `-codepage`, GCC's
    `-finput-charset`.
  - The defaults have moved one way: Python 3.0 (PEP 3120), Ruby (*"Default
    encoding is UTF-8"*), Erlang/OTP 17.0, JDK 18 (JEP 400), and C++23
    mandating UTF-8 support.
  - Erlang's transition shows both costs. Its release candidates, with no
    retry, printed *"no module definition"* beside the real cause (F1's
    false `unknown_module` shape). OTP 17.0 then re-read undeclared files as
    Latin-1 with a deprecation warning, promising an error *"preferably in
    OTP 18"*; whether that came I could not verify.
  - Roslyn's permanent silent fallback to code page 1252 is the version
    with no exit.
  - Among the languages born UTF-8-only that I read (Go, Rust, Swift, Zig),
    none added a declaration. This is a negative from the searcher's list,
    not the world.

### Q2. The message and its fix

**Verdict (advisory): name the byte, its line and column, and UTF-8; name
UTF-16 only when the bytes prove it; no `certain` fix, in a string or a
comment.**

- **What precedents name.**
  - The byte: Python as `'\xe9'`, javac as `(0xE9)`, dmd as `\xNN`, rustc in
    decimal (`193`), MSVC as an offset (`0x2`).
  - The position: gc (line and byte column), rustc (line:col and a snippet),
    Python (the line).
  - The encoding expected: all of them.
  - The remedy: CPython links PEP 263 *in the message*; MSVC's C4819 says
    *"Save the file in Unicode format"*.
- **A likely encoding, only where the bytes decide it.**
  - Go issue 71950 (2025-02-25) is the record of what a generic message
    costs: UTF-16 files saved on Windows got *"unexpected NUL"*. `go/scanner`
    now says *"illegal UTF-8 encoding (got UTF-16)"*, and only when the file
    begins `FF FE` or `FE FF`.
  - Swift names UTF-16 on a `0xFF` or `0xFE` byte.
  - dmd goes furthest: it detects UTF-16 and UTF-32 without a byte order
    mark from zero bytes in the first character, and transcodes them.
  - For single-byte encodings no front end I read guesses. P2295 gives the
    reason in one example: *"in `windows-1251`, `0xC0` represents the
    cyrillic letter A"*. `file(1)` names only a family (*"ISO-8859"* against
    *"Non-ISO extended-ASCII"*, split on the 0x80 to 0x9F range).
  - So *Latin-1 or Windows-1252* in a message would be a guess, false for a
    file in windows-1251, P2295's own example. The critic's measured UTF-16
    cases (its item 17: 0xFF or 0xFE first with a mark, 0xE9 at line 2
    without one) are the one place where naming the encoding has proof
    behind it, and the second is exactly Go's misleading-message shape.
- **The fix.**
  - The only automatic fix I found is Swift's fix-it, replacing the bad
    bytes with a space, and only outside comments and strings.
  - No front end I read offers a transcoding fix. The byte's letter is not
    recoverable from the byte, so a transcoding fix is a `guess` by Heroes'
    own rule. Deleting a byte in a comment repairs the defect named, but
    destroys a letter the author meant (P2295's *"the name of a maintainer"*).
  - No precedent offers it; it is at most a `guess`, and none would be my
    reading.

### Q3. How many, what after, and what is written

**Verdict (advisory): one diagnostic per file, at the first bad byte, as
proposed; if more are ever wanted, Unicode's maximal subparts; no writer ever
runs on a file that is not text.**

- **One per file**: rustc (since 1.86, PR 135557: *"the first invalid UTF-8
  character"*), CPython (the loop breaks at the first bad byte), GHC (one
  *lexical error* in the transcript). **Per line**: gc, which keeps one of
  several equal messages per line and stops at ten. **Every occurrence**:
  `go/scanner`, Swift, javac, clang in code position.
- **What one-per-file buys, from the code**: the first bad byte's position is
  computed over the valid prefix only (`valid_up_to` in rustc), so it is
  exact under any rule for later bytes. Rust and Python never meet F6's
  per-byte-or-per-run question.
- **If the lexer goes on, a rule must be chosen, and several are in use.**
  - Go advances per byte (`DecodeRune` returns `(RuneError, 1)`).
  - javac replaces per malformed sequence of Java's decoder
    (`dest.put((char)0xfffd); // backward compatible`).
  - Swift reports and advances per sequence
    (`validateUTF8CharacterAndAdvance`); what it substitutes, if anything,
    I did not read.
  - Unicode documents the practice, *U+FFFD Substitution of Maximal
    Subparts*, *"Although the Unicode Standard does not require this practice
    for conformance"*. One U+FFFD per byte, except a truncated prefix of an
    otherwise valid sequence (Table 3-11: `E1 80` takes one, `F0 91 92` takes
    one).
  - For a Latin-1 byte followed by ASCII, all agree. They differ only on
    truncated multi-byte prefixes, which is the critic's item 5.
- **Cascades**: `go/scanner` consumes the whole input after a UTF-16 mark
  (*"to avoid error cascade"*); Swift jumps to the end of the buffer. One
  diagnostic for a UTF-16 file is the precedent.
- **Writers**: gofmt parses before it writes (`if err != nil { return err }`
  precedes `writeFile`), and `go/parser` adds every scanner error to its
  error list (`eh := func(...) { p.errors.Add(pos, msg) }`). So a file with a
  bad byte is never rewritten by gofmt (verified, the chain read in three
  files). That is F8's requirement, met by construction.
- **The cautionary side.** Code and documentation together show a replaced
  text reaching an output:
  - tsc decodes a file without a byte order mark by `buffer.toString("utf8")`.
  - Node's documentation says such decoding uses *"the Unicode replacement
    character `U+FFFD`"*.
  - javac's comment calls its U+FFFD *"backward compatible"*, from its
    warning era.

  That a warning-era build or a tsc emit carried U+FFFD into its output is my
  inference from code and documentation, not a reported case.

### Q4. Where it lives

**Verdict (advisory): the check at read time is the rustc shape; whatever
reports the offset must be tested against an independent validator, as
Zig's own condition for restoring its check says.**

- **Two architectures in the field.** Some validate at read: rustc via
  `read_to_string`, javac in its file manager, CPython per line read, Roslyn
  at decode. Others validate in the lexer: Go, Swift, clang, and Zig until
  2024. The proposal (a runtime check reporting the first offset) is the
  first kind.
- **Zig's history is the risk to plan for.** Commit 377e857, *"I pointed a
  fuzzer at the tokenizer and it crashed immediately. ... Removes UTF-8
  validation."* Its condition for bringing the check back: *"it must be
  fuzz-tested while checking the property that it matches an independent
  Unicode validation implementation on the same file."*
  - Heroes has `hero_utf8_valid` (F2), a `bool`. The new offset-reporting
    function can be held to Zig's property against it: it reports *no
    offset* exactly when the old one says valid, and the prefix before the
    offset validates.
  - Cases: Unicode 15's Tables 3-8 to 3-11 (four rows of nine bytes, counted
    from the page images) and Zig's eight `invalid utf8` rows.
- **Holding a case whose file is not UTF-8 (F8).** Three precedents keep such
  files in the tree:
  - clang's `comment-invalid-utf8.c` is *"purposefully encoded as
    windows-1252"* and carries its ASCII `expected-warning` annotations
    inside those bytes, so its harness reads annotations from a file that is
    not UTF-8.
  - CPython keeps `Lib/test/tokenizedata/badsyntax_pep3120.py`, 14 bytes
    with one bad byte, and its test only asserts `'utf-8' in msg`.
  - Rust keeps `tests/ui/macros/not-utf8.bin`, reached by `include!` from an
    ASCII `.rs`. Its error's primary span is the `include!` line, its note
    at the byte. That is also a precedent for the proposal's *a `use`d module
    the same*: rustc tells the including site and points into the file.
  - Heroes' run golden writes the bytes at run time instead (F8). Both shapes
    exist; clang's is the one that keeps § 9's annotation in the source.

### Q5. The specification

**Verdict (advisory): a sentence has the stronger precedent; its price and
payment are the spec-warden's.**

- One sentence each, verified:
  - the Rust Reference, *"It is an error if the file is not valid UTF-8."*;
  - the Python reference, *"If the text cannot be decoded, a SyntaxError is
    raised."*;
  - C++23 [lex.phases], *"it shall be a well-formed UTF-8 code unit
    sequence"*;
  - Zig's language reference, *"An invalid UTF-8 byte sequence results in a
    compile error."*
- None: the Go spec (*"Source code is Unicode text encoded in UTF-8"*, and
  nothing on a file that is not). Its compilers refuse anyway.
- **Zig is the case that cuts both ways.** Its sentence has been false
  against its own tokenizer since 2024. It is also what made the removal a
  named regression (*"a regression of #663"*) rather than a silent change of
  meaning, which is CLAUDE.md § 12's mechanism working in another project.

### Q6. The shapes beside

**Verdict (advisory): argv and the environment have precedents that tell
*not text* apart from *absent*; by the brief's own definition the
environment row may share 227's cause, a question for the seats who run.**

- **argv.**
  - Rust's `std::env::args` *"will panic during iteration if any argument to
    the process is not valid Unicode"*; `args_os` is the bytes route.
  - rustc itself panicked with an internal compiler error until PR 42092
    (cuviper, merged 2017-05-19, *"Give a nicer error for non-Unicode
    arguments to rustc and rustdoc"*). Now each bad argument is reported,
    *"argument {i} is not valid Unicode: {arg:?}"*, then a fatal error.
  - It names the index and the escaped value, and gives no advice for a
    program's author. The critic's Q6 table found Heroes' argv message does
    (its `args_checked()` note). That is a shape apart, not 227's cause.
- **The environment.** Rust's `VarError` has `NotPresent` and
  `NotUnicode(OsString)` (*"was found, but it did not contain valid unicode
  data"*): the type itself refuses to collapse *not text* into *absent*.
  - The critic measured `HEROES_RUNTIME` holding bytes that are not UTF-8 as
    read as unset (*runtime not found*). It classed this *no* for 227's
    cause.
  - 00-shared defines that cause as *"a read that is not text taken as
    unreadable or absent"*. By those words the row seems to share it. A
    question, not a finding: I ran nothing.
- **File names.** PEP 383 (Python 3.1, 2009) is the lossless route: *"File
  names, environment variables, and command line arguments are defined as
  being character data in POSIX; the C APIs however allow passing arbitrary
  bytes"*. Undecodable bytes go to U+DC80..U+DCFF and back. Go needs none:
  *"A string value is a (possibly empty) sequence of bytes."*

## The prediction (falsifiable)

**Registered, scored by this sitting's blind experiment
(`docs/panel/189-briefs/llm-ergonomist.md`):** arms B, C and D, whose
messages carry the same words and differ only in the code, will not separate.
Their one-turn repair counts will differ by **at most 1 of 4** from each
other.

The basis: the remedy travels in the words, and three front ends I read print
no code at all for this error (rustc's `.stderr` shows none; gc and
`go/scanner` emit a bare string). **Falsified** if any two of B, C and D
differ by 2 or more of 4. A session stopped by its cap is excluded from both
counts.

## What would change this reading

- A front end whose refusal names only the file (*not a text file*), with a
  record that its users repair as readily: Q1's condition on the words would
  weaken.
- A Rust or CPython issue showing that one-error-per-file cost users repeated
  turns, answered by reporting every bad byte: Q3 would lean to one per line.
- A language born UTF-8-only that later added an encoding declaration because
  its users needed one: the declaration route would reopen.
- A recorded reason for clang's or GCC's comment leniency other than an
  installed base: the comment route would reopen.
- Zig restoring validation in a different place (its file loader rather than
  its tokenizer), with its reason: Q4's placement would gain a data point.

## The evidence, per front end

### Go: two front ends, both report per occurrence and keep reading

- **The spec** (verified, https://go.dev/ref/spec, § Source code
  representation): *"Source code is Unicode text encoded in UTF-8."* Its two
  implementation restrictions are about NUL and the byte order mark; nothing
  on invalid UTF-8. § String types: *"A string value is a (possibly empty)
  sequence of bytes."*
- **gc** (verified,
  https://raw.githubusercontent.com/golang/go/master/src/cmd/compile/internal/syntax/source.go):
  `if s.ch == utf8.RuneError && s.chw == 1 { s.error("invalid UTF-8
  encoding"); goto redo }`. Comments are skipped through the same `nextch`
  (`syntax/scanner.go`: `skipLine` and `skipComment` call `s.nextch()`), so a
  bad byte in a comment is an error. Columns: `s.col += uint(s.chw)`, bytes
  (the fetch tool guessed characters; the code says bytes).
- **How many** (verified, `cmd/compile/internal/base/print.go`, `ErrorfAt`): a
  scanner message is not prefixed *syntax error*, so it takes *"only one of
  multiple equal non-syntax errors per line"*, and at `numErrors >= 10`
  without `-e` it prints *"too many errors"* and exits.
- **`go/scanner`** (verified,
  https://raw.githubusercontent.com/golang/go/master/src/go/scanner/scanner.go):
  *"illegal UTF-8 encoding"* per bad byte; at offset 0 before `FF FE` or
  `FE FF`, *"illegal UTF-8 encoding (got UTF-16)"*, then `s.rdOffset +=
  len(in) // consume all input to avoid error cascade`. `go/token`'s
  `Column` is *"column number, starting at 1 (byte count)"*.
  `unicode/utf8.DecodeRune` returns `(RuneError, 1)` for an invalid encoding
  (verified, https://pkg.go.dev/unicode/utf8).
- **What happened** (verified, https://github.com/golang/go/issues/71950,
  adonovan, 2025-02-25, *"go/scanner: improve error message when a Go file is
  encoded as UCS-2"*): Windows users saving UTF-16 got *"unexpected NUL"*;
  the proposal read *"encoding of file foo.go is UCS-2; Go requires UTF-8"*.
  Which release carried the shipped message: unverified.
- **The writer** (verified): `cmd/gofmt/gofmt.go`, `parse(...)` then `if err
  != nil { return err }` before `writeFile`; `go/parser/parser.go`, `eh :=
  func(pos token.Position, msg string) { p.errors.Add(pos, msg) }`.

### Rust: one error per file, the first byte, since 1.86

- **The message** (verified,
  https://raw.githubusercontent.com/rust-lang/rust/master/tests/ui/macros/not-utf8.stderr):
  *"error: couldn't read `$DIR/not-utf8.bin`: stream did not contain valid
  UTF-8"*, primary span at the `include!` line, then *"note: byte `193` is
  not valid utf-8"* at the `.bin`'s 1:1. No error code.
- **The code** (verified,
  https://doc.rust-lang.org/nightly/nightly-rustc/src/rustc_parse/lib.rs.html):
  read failures are told by kind (*couldn't find file*, *permission denied
  when opening file*, *is a directory*, otherwise *couldn't read*); the note
  is `byte{s} `{bytes}` {are} not valid utf-8` from `valid_up_to()` and
  `error_len()`, or `invalid utf-8 at byte `{start}`` for a sequence cut off
  at the end. The source is read by `file.read_to_string(&mut contents)?`
  (verified,
  https://raw.githubusercontent.com/rust-lang/rust/master/compiler/rustc_span/src/source_map.rs).
- **What happened** (verified, https://github.com/rust-lang/rust/pull/135557,
  *"Point at invalid utf-8 span on user's source code"*, estebank, opened
  2025-01-15, merged 2025-01-23, milestone 1.86.0). Its description adds
  *where*: *"we provide additional context about *where* the file has the
  first invalid UTF-8 character"*. Rust 1.0 shipped 2015-05-15 and 1.86
  2025-04-03 (the dates in the blog's own URLs,
  https://blog.rust-lang.org/2015/05/15/Rust-1.0/ and
  https://blog.rust-lang.org/2025/04/03/Rust-1.86.0.html, read through
  search results). Rust kept *couldn't read* as the headline and added the
  note beside it.
- **A program sees the same words** (verified,
  https://users.rust-lang.org/t/solved-stream-did-not-contain-valid-utf-8/10186,
  2017-04-02): `read_to_string` on the program's own binary, *"stream did
  not contain valid UTF-8"*. Where the standard library defines the string I
  did not find (two fetches of `library/std/src/io/` saw no such line).
- **The Reference** (verified, https://doc.rust-lang.org/reference/input-format.html):
  *"Each source file is interpreted as a sequence of Unicode characters
  encoded in UTF-8. It is an error if the file is not valid UTF-8."*
- **argv and environment** (verified): https://doc.rust-lang.org/std/env/fn.args.html;
  https://doc.rust-lang.org/std/env/enum.VarError.html;
  https://doc.rust-lang.org/stable/nightly-rustc/src/rustc_driver_impl/args.rs.html
  (`"argument {i} is not valid Unicode: {arg:?}"`, every argument collected,
  then fatal); https://github.com/rust-lang/rust/pull/42092 (cuviper, opened
  2017-05-18, merged 2017-05-19; before it, *"error: internal compiler error:
  unexpected panic"*).

### Python: one error, the first byte, and the route out in the message

- **The message** (verified,
  https://raw.githubusercontent.com/python/cpython/main/Parser/tokenizer/helpers.c,
  `_PyTokenizer_ensure_utf8`): *"Non-UTF-8 code starting with '\\x%.2x'%s%V
  on line %i, but no encoding declared; see https://peps.python.org/pep-0263/
  for details"*; the loop breaks at the first bad byte. A transcript
  (verified, https://mail.python.org/pipermail/tutor/2012-August/090756.html,
  2012-08-09): *"SyntaxError: Non-UTF-8 code starting with '\xd0' in file on
  line 30, but no encoding declared"*.
- **Comments are covered** (verified, a mirror of CPython's
  `Parser/tokenizer/file_tokenizer.c`,
  https://chromium.googlesource.com/external/github.com/python/cpython/+/6b9a6c6ec3bbc9795df67b87340e2ea58f42b3d4/Parser/tokenizer/file_tokenizer.c):
  `if (!tok->encoding) { /* The default encoding is UTF-8, so make sure we
  don't have any non-UTF-8 sequences in it. */ if
  (!_PyTokenizer_ensure_utf8(line, tok, lineno)) ...`, run on each line read
  before tokenizing.
- **The default** (verified, https://peps.python.org/pep-3120/, Final, 3.0):
  *"This PEP proposes to change the default source encoding from ASCII to
  UTF-8."* The reference (verified,
  https://docs.python.org/3/reference/lexical_analysis.html): *"If the text
  cannot be decoded, a SyntaxError is raised."*
- **The declaration** (verified, https://peps.python.org/pep-0263/, Final,
  Python 2.3, created 2001-06-06): a comment on line 1 or 2 matching
  `^[ \t\f]*#.*?coding[:=][ \t]*([-_.a-zA-Z0-9]+)`; encodings *"which use two
  or more bytes for all characters like e.g. UTF-16"* excluded.
- **The test case in the tree** (verified):
  https://raw.githubusercontent.com/python/cpython/main/Lib/test/tokenizedata/badsyntax_pep3120.py
  (14 bytes, `print("b` + one bad byte + `se")`) and
  https://raw.githubusercontent.com/python/cpython/main/Lib/test/test_utf8source.py
  (`self.assertTrue('utf-8' in msg)`).
- **File names, environment, argv** (verified, https://peps.python.org/pep-0383/,
  Final, Python 3.1, Martin von Löwis, created 2009-04-22): the
  `surrogateescape` handler.

### Swift: every occurrence, comments included, and a UTF-16 message

- (verified,
  https://raw.githubusercontent.com/swiftlang/swift/main/include/swift/AST/DiagnosticsParse.def)
  `lex_invalid_utf8`, *"invalid UTF-8 found in source file"*;
  `lex_utf16_bom_marker`, *"input files must be encoded as UTF-8 instead of
  UTF-16"*; `lex_nul_character`, a warning.
- (verified,
  https://raw.githubusercontent.com/swiftlang/swift/main/lib/Parse/Lexer.cpp)
  `lex_invalid_utf8` in line comments (`skipToEndOfLine`), block comments,
  string literals (`lexCharacter`) and code (`lexImpl`), one per bad
  sequence, lexing continues. Only the `lexImpl` one carries a fix-it,
  `.fixItReplaceChars(..., " ")`. The UTF-16 arm is `case (char)-1: case
  (char)-2:`, then `CurPtr = BufferEnd`; as quoted it is not limited to
  offset 0, so a stray `0xFF` where a token starts is also told *UTF-16*.
  Whether another check guards that: unverified.

### Zig: a documented rule, enforced, then removed after a fuzzer

- (verified, https://github.com/ziglang/zig/issues/663, thejoshwolfe,
  2017-12-23): *"It is a compile error for zig source to contain invalid utf8
  byte sequences."* The language reference: *"Zig source code is encoded in
  UTF-8. An invalid UTF-8 byte sequence results in a compile error."*
  (verified,
  https://github.com/ziglang/zig/commit/e6d4028a84d0e9b9835c38f57b5b0d4bbc101394,
  andrewrk; its date 2018-01-31 only from a mirror's listing in search
  results). Today's wording, *"Invalid UTF-8 byte sequences are not allowed
  anywhere"*, is a search summary's and unverified.
- (verified,
  https://github.com/ziglang/zig/commit/377e8579f9539c56fc5988c9a452a01438af89f3,
  andrewrk; its date 2024-07-31 only from a mirror's listing in search
  results): quoted under Q4.
- (verified, the canonical repository,
  https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/zig/tokenizer.zig;
  GitHub read-only since the move announced 2025-11-26,
  https://ziglang.org/news/migrating-from-github-to-codeberg/, through search
  results) `test "invalid utf8"`: `//\x80`, `//\xbf`, `//\xf8`, `//\xff`,
  `//\xc2\xc0`, `//\xe0`, `//\xf0`, `//\xf0\x90\x80\xc0` each tokenize to no
  token. Whether a later stage of the compiler refuses them: unverified.

### Java (javac): every occurrence, a replacement, and a warning that became an error

- (verified,
  https://raw.githubusercontent.com/openjdk/jdk/master/src/jdk.compiler/share/classes/com/sun/tools/javac/resources/compiler.properties)
  `compiler.err.illegal.char.for.encoding`, *"unmappable character (0x{0})
  for encoding {1}"*.
- (verified,
  https://raw.githubusercontent.com/openjdk/jdk/master/src/jdk.compiler/share/classes/com/sun/tools/javac/file/BaseFileManager.java)
  on a malformed or unmappable result: `%02X` of the bytes, `log.error(...)`,
  then `dest.put((char)0xfffd); // backward compatible`, decoding continues.
- (verified, https://mail.openjdk.org/pipermail/discuss/2009-August/001355.html,
  Joe Darcy, 2009-08-07) *"an encoding problem generates a warning with
  source 1.5 but is treated as an error with source 1.6."* And
  https://bugs.openjdk.org/browse/JDK-6893695 (created 2009-10-21): *"In the
  JDK6 javac reports about encoding warning however now it reports about
  error"*, a JDK test file that stopped compiling on it.
- (verified, https://openjdk.org/jeps/400, JDK 18) javac *"assumes that
  `.java` source files are encoded with the default charset, unless
  configured otherwise by the `-encoding` option"*; the default became UTF-8.

### C# (Roslyn): a silent transcoding, and one refusal that names the file

- (verified,
  https://raw.githubusercontent.com/dotnet/roslyn/main/src/Compilers/Core/Portable/EncodedStringText.cs)
  *"it will first try UTF-8 and, if that fails, it will try CodePage 1252. If
  CodePage 1252 is not available on the system, then it will try Latin1."*
  Microsoft's documentation agrees (verified,
  https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/compiler-options/advanced,
  § CodePage). Whether the fallback is announced by a diagnostic: unverified,
  I found none.
- (verified, https://learn.microsoft.com/en-us/dotnet/csharp/misc/cs2015)
  CS2015, *"'file' is a binary file instead of a text file"*; its test
  (verified,
  https://raw.githubusercontent.com/dotnet/roslyn/main/src/Compilers/Core/Portable/Text/SourceText.cs):
  two consecutive NULs, *"unlikely to appear in genuine text"*.

### clang: three tiers; comments accepted by default

- (verified,
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang/include/clang/Basic/DiagnosticLexKinds.td)
  `err_invalid_utf8`, *"source file is not valid UTF-8"*;
  `warn_invalid_utf8_in_comment`, an `Extension` in `invalid-utf8`;
  `warn_bad_string_encoding`, an `ExtWarn`, *"illegal character encoding in
  string literal"*. An `Extension` is *"ignored by default"*, an `ExtWarn`
  warns by default (verified, https://clang.llvm.org/docs/InternalsManual.html).
  P2295 (2022) describes clang's string-literal case as an error; the
  current `.td` says a default warning.
- (verified,
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang/test/Lexer/utf8-invalid.c)
  `extern int <bad byte>x; // expected-error{{source file is not valid
  UTF-8}}`, and none in `#if 0` or in preprocessor directives.
- (verified,
  https://github.com/llvm/llvm-project/commit/e3dc56805f1029dd5959e4c69196a287961afb8d,
  cor3ntin, https://reviews.llvm.org/D128059; its date, July 2022, from a
  search summary only) the comment warning, *"off by default as its likely to
  be somewhat disruptive otherwise"*, to conform to P2295R5. Its test,
  https://raw.githubusercontent.com/llvm/llvm-project/main/clang/test/Lexer/comment-invalid-utf8.c,
  is *"purposefully encoded as windows-1252"* with its annotations inside
  (read through the fetch tool's summary).

### GCC

- (verified, https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html)
  `-finput-charset`, *"The default is UTF-8"*; *"If the input character set
  is UTF-8, warnings about ill-formed code unit sequences are issued if
  -Winvalid-utf8 is enabled. Otherwise no diagnostics are issued when the
  input character set matches the execution character set."* The
  `-Winvalid-utf8` entry itself I could not reach (the warning page's fetch
  did not include it); a search summary placed it in GCC 13 with P2295,
  unverified. P2295 on GCC's UTF-8 input (verified, the PDF below): *"it is
  not decoded at all ... and so the input is not validated. ... We don't
  know if this is intentional."*

### MSVC (through P2295 and Microsoft's archive)

- C4828 as P2295 transcribes it (verified, the PDF below): *"main.cpp(1):
  warning C4828: The file contains a character starting at offset 0x2 that is
  illegal in the current source character set (codepage 65001)."*
  Microsoft's own C4828 page: not found (404).
- C4819 (verified,
  https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2013/ms173715(v=vs.120)):
  *"The file contains a character that cannot be represented in the current
  code page (number). Save the file in Unicode format to prevent data
  loss."*

### C++23's P2295R6

Verified, the PDF read whole,
https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2295r6.pdf
(*"Support for UTF-8 as a portable source file encoding"*, Corentin Jabot and
Peter Brett, 2022-07-01). Its § Invalid code units, § SG16 Polls (2021-04-14,
8-2-0-0-0), § Wording and § Acknowledgments are quoted under Q1 and Q2. Its
§ Input validation records 2022: GCC does not validate UTF-8 input; *"Clang
does not check invalid comments. By reading the source code this is very
intentional"*; MSVC warns even in comments. Whether R6 entered C++23 as
adopted: unverified here (its own text is a proposal; I read no plenary
minutes).

### GHC, Ruby, Erlang, Nim, D, TypeScript and Node

- **GHC** (verified, https://mail.haskell.org/pipermail/beginners/2016-May/016869.html,
  2016-05-08): *"baby.hs:6:42: lexical error (UTF-8 decoding error)"*.
- **Ruby** (verified): *"Default encoding is UTF-8"* and the magic comment on
  the first line, or the second after a shebang
  (https://docs.ruby-lang.org/en/master/syntax/comments_rdoc.html);
  *"invalid multibyte char (UTF-8)"* (https://bugs.ruby-lang.org/issues/20990,
  Ruby 3.4.1, 2024-12-28); *"blah.ruby:1: invalid multibyte char
  (US-ASCII)"* under the old default
  (https://markhneedham.com/blog/2013/01/27/ruby-invalid-multibyte-char-us-ascii,
  2013-01-27).
- **Erlang** (verified): UTF-8 the default *"In Erlang/OTP 17.0"*
  (https://www.erlang.org/doc/apps/stdlib/unicode_usage.html). Andreas
  Schumacher on erlang-questions, 2014-03-13
  (https://erlang.org/pipermail/erlang-questions/2014-March/078214.html):
  - the release candidates printed *"tst.erl:1: cannot parse file, giving
    up / tst.erl:1: no module definition / tst.erl:1: cannot translate from
    UTF-8"*;
  - 17.0 *"issues a deprecation warning, and processes the file again,
    assuming latin-1 encoding"*;
  - *"preferably in OTP 18, the deprecation warning will be turned into an
    error again"*.

  EDoc's notes say the same of its own retry, *"This workaround will be
  removed in a future release"* (OTP-12008,
  https://docs.huihoo.com/erlang/18/lib/syntax_tools-1.7/doc/html/notes.html).
  Whether OTP 18 made it an error: **unverified**. Neither the 18.0
  announcement (https://erlang.org/news/86, 2015-06-24) nor STDLIB 2.4 and 2.5's
  notes mention it.
- **Nim** (verified, https://nim-lang.org/docs/manual.html): *"All Nim source
  files are in the UTF-8 encoding (or its ASCII subset). Other encodings are
  not supported."*, yet `letter ::= 'A'..'Z' | 'a'..'z' | '\x80'..'\xff'`;
  the lexer's `SymChars` include `'\x80'..'\xFF'` and no validation was found
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/lexer.nim,
  through the fetch tool's reading).
- **D** (verified, https://dlang.org/spec/lex.html): ASCII, UTF-8, UTF-16BE,
  UTF-16LE, UTF-32BE, UTF-32LE; *"If the source file does not begin with a
  BOM, then the first character must be less than or equal to U+0000007F."*
  dmd's `processSource` (verified,
  https://raw.githubusercontent.com/dlang/dmd/master/compiler/src/dmd/dmodule.d)
  transcodes by mark or by zero bytes, else *"source file must start with BOM
  or ASCII character, not \\x%02X"*. D's tracker returned HTTP 522 for bug 430.
- **TypeScript** (verified, tag v5.8.3,
  https://raw.githubusercontent.com/microsoft/TypeScript/v5.8.3/src/compiler/sys.ts):
  UTF-16 decoded by either mark, a UTF-8 mark stripped, otherwise
  `buffer.toString("utf8")`. **Node** (verified,
  https://nodejs.org/api/buffer.html): *"the Unicode replacement character
  `U+FFFD` � will be used to represent those errors"*; its CommonJS loader
  reads `readFileSync(filename, 'utf8')` (verified,
  https://raw.githubusercontent.com/nodejs/node/main/lib/internal/modules/cjs/loader.js).

### The vocabulary: Unicode, POSIX, grep, file(1)

- **Unicode** (verified, Unicode 18.0.0 chapter 3,
  https://www.unicode.org/versions/Unicode18.0.0/core-spec/chapter-3/): C10,
  *"When a process interprets a code unit sequence which purports to be in a
  Unicode character encoding form, it shall treat ill-formed code unit
  sequences as an error condition and shall not interpret such sequences as
  characters."* The maximal-subpart section and Tables 3-8 to 3-11 read in
  Unicode 15.0's chapter 3 PDF, pages 127 to 128
  (https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf): *"An ill-formed
  subsequence consisting of more than one code unit could be treated as a
  single error or as multiple errors"*, and D93b's rule, *"The maximal
  subpart at that offset is replaced by a single U+FFFD."*
- **POSIX**: *"Character: A sequence of one or more bytes representing a
  member of a character set"* (verified, POSIX.1-2024 § 3.58,
  https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html).
  *Text File*, *"A file that contains characters organized into zero or more
  lines"*, is quoted from a mail (verified as quoted,
  https://lists.suckless.org/dev/1411/24639.html, Evan Gates, 2014-11-21);
  the standard's own page was truncated before it.
- **GNU grep** (verified, the manual for 3.12,
  https://www.gnu.org/software/grep/manual/grep.html): *"Non-text bytes
  indicate binary data; these are either output bytes that are improperly
  encoded for the current locale ..., or null input bytes"*.

  NEWS (verified,
  https://cgit.git.savannah.gnu.org/cgit/grep.git/plain/NEWS):
  - 2.21 (2014-11-23), files with improperly encoded data are treated as
    binary;
  - 2.23 (2016-02-04), *"Binary files are now less likely to generate
    diagnostics and more likely to yield text matches"*;
  - 3.5 (2020-09-27), reworded to *"grep: FOO: binary file matches"* on
    standard error.

  The bug (verified, https://bugzilla.redhat.com/show_bug.cgi?id=1219141,
  Kamil Dudka, 2015-05-06; Eric Blake's answer 2015-05-11) is quoted under
  Q1.
- **file(1)** (verified,
  https://raw.githubusercontent.com/file/file/master/src/encoding.c): the
  names *"Unicode text, UTF-8"*, *"Unicode text, UTF-16, little-endian"*,
  *"ISO-8859"*, *"Non-ISO extended-ASCII"*, the last for bytes 0x80 to 0x9F
  *"which ISO-8859 considers to be control characters but the IBM PC and
  Macintosh consider to be printing characters"*.

## The comparison, from the evidence above

| front end | the words | how many | goes on | the byte | the position | encoding named | a fix |
|---|---|---|---|---|---|---|---|
| Go gc | *invalid UTF-8 encoding* | one per line, ten then stop | yes | no | line:col (bytes) | no | no |
| `go/scanner` | *illegal UTF-8 encoding*, *(got UTF-16)* | each byte; UTF-16 one | yes | no | offset (bytes) | UTF-16 by mark | no |
| rustc 1.86+ | *couldn't read ...: stream did not contain valid UTF-8* + note | one per file | no | decimal | line:col | no | no |
| CPython | *Non-UTF-8 code starting with '\xe9' ... see PEP 263* | one per file | no | `\x` hex | line | no; the declaration route | no |
| Swift | *invalid UTF-8 found in source file*; *must be encoded as UTF-8 instead of UTF-16* | each; UTF-16 one | yes | no | location | UTF-16 on 0xFF/0xFE | a space, in code only |
| javac | *unmappable character (0xE9) for encoding UTF-8* | each sequence | yes, U+FFFD | hex | position | the one in use | no |
| clang | *source file is not valid UTF-8* (code) | each | yes | no | location | no | no |
| MSVC C4828 | *... starting at offset 0x2 ... (codepage 65001)* | unverified | yes, a warning | offset | line | code page | no |
| GHC | *lexical error (UTF-8 decoding error)* | one seen | unverified | no | line:col | no | no |
| Ruby | *invalid multibyte char (UTF-8)* | unverified | unverified | no | line | the one in use | no |
| Erlang 17-rc | *cannot translate from UTF-8* and two more | three at line 1 | no | no | line 1 | no | 17.0: Latin-1 retry |
| dmd | *must start with BOM or ASCII character, not \xNN* | one | no | `\x` hex | no | UTF-16/32 detected | transcodes |
| Roslyn | none; CS2015 for two NULs | none | n/a | n/a | n/a | assumes 1252 | transcodes |
| GCC, Zig master, Nim, tsc | none by default | none | n/a | n/a | n/a | n/a | n/a |

## What I ran, what it cost, what stays unverified

- **Run**: no command; this seat has no shell. Web searches: 27; fetches:
  about 114, both counted by hand from this session's own calls (a hand count,
  so an approximation). Three PDFs read from the fetch tool's saved copies.
  No paid session started. The session's own cost is not measured by this
  seat.
- **Not searched**: Pascal-P4, Oberon, Cyclone, Hylo, Idris and K/APL from
  the charter's lineage, and Elm, Gleam, OCaml and Kotlin. This report makes
  no claim about them.
- **Unverified, as marked above**:
  - which Go release carried *(got UTF-16)*;
  - the dates of Zig's two commits and of clang's (search listings only);
  - Zig's current reference wording;
  - Swift's UTF-16 arm beyond offset 0, and what Swift substitutes for a bad
    sequence;
  - whether Roslyn announces its fallback;
  - GCC's `-Winvalid-utf8` entry;
  - Microsoft's C4828 page;
  - P2295R6's adoption;
  - whether OTP 18 made Latin-1 an error;
  - where Rust's standard library defines its string;
  - GHC's and Ruby's counts.
