# Panel 187, the historian's report

Written as it goes, 2026-10-02 (the session's date), against the trunk at
`07ccb72a` as `00-shared.md` describes it. Seat: historian, advisory, no veto.

**What I built and ran**: nothing. No copy, no compiler, no instrument, no paid
run. I had no shell, so `<scratchpad>/187-historian/` was never made. In the
trunk I read only my brief, its pre-critic text, `00-shared.md` and the critic's
report on the briefs; §4.17 is quoted from `00-shared.md` Q5, not read.
design.md's historical appendix, which my mandate names, was **not read** for
the same reason. **Cost**: web search and fetch only; two PDFs fetched and read
whole or in part with the file reader (Diekmann and Tratt 2020; de Jonge and
Visser 2012).

**Marking**: **verified** means I read the claim in the cited source in this
session; **unverified** means I did not, and nothing below rests on it. A page
fetched today is true of today's page; where it carries a version I give it. No
claim here rests on *Heroes of code*; I did not consult it for this sitting.
External source files are named by their URL, not as repository paths.

## Status

Complete. Sections 1 to 8 are the precedents; the verdict, argument,
prediction and condition follow them, then what I could not verify.

## 1. rustc

**1.1 Follow-on errors are silenced by an error value, and the project says
why.** The rustc-dev-guide's chapter on types
(<https://rustc-dev-guide.rust-lang.org/ty.html>, fetched 2026-10-02): *"There
is a `TyKind::Error` that is produced when the user makes a type error. The idea
is that we would propagate this type and suppress other errors that come up due
to it so as not to overwhelm the user with cascading compiler error messages."*
Its invariant: the compiler *"should never produce `Error` unless we know that an
error has already been reported to the user"*, enforced by `ErrorGuaranteed`
(<https://rustc-dev-guide.rust-lang.org/diagnostics/error-guaranteed.html>: *"if
your compiler code ever encounters a value of type `ErrorGuaranteed`, the
compilation is statically guaranteed to fail"*). **Verified.** Since when:
**unverified**.

**1.2 Identical diagnostics are deduplicated by default.** The Unstable Book
(<https://doc.rust-lang.org/unstable-book/compiler-flags/deduplicate-diagnostics.html>):
*"deduplicate identical diagnostics (default: yes)"*, with *"no tracking issue"*.
**Verified.** It removes only an identical message; a second, different message
for one mistake (this sitting's class (a)) is untouched by it.

**1.3 The parser's own bound: stop after unbalanced delimiters, and what it
cost.** PR [#108297](https://github.com/rust-lang/rust/pull/108297), *"Exit
when there are unmatched delims to avoid noisy diagnostics"*, by chenyukang,
merged 2023-03-01, milestone 1.69.0. **Verified** (title, author, date,
milestone; its rationale text did not show in the fetch). Before it, PR
[#104012](https://github.com/rust-lang/rust/issues/104012) (opened 2022-11-05)
had described one extra `{` producing several cascading messages, the first
pointing at an unrelated block, and reported only the most relevant delimiter
candidate. **What happened next**: issue
[#108608](https://github.com/rust-lang/rust/issues/108608), opened the day of
the merge, *"help: `}` may belong here" no longer reported*: the change that cut
the noise cut a useful hint with it. **Verified.** The nearest precedent to the
instrument's `bracket-open` pairs: after a bracket error, a production parser
chose zero further parse messages, and paid one suggestion for it on day one.

**1.4 `-Z treat-err-as-bug` is a debugging flag.** The Unstable Book
(<https://doc.rust-lang.org/unstable-book/compiler-flags/treat-err-as-bug.html>):
*"This flag converts the selected error to a `bug!` call, exiting the compiler
immediately and allowing you to generate a backtrace of where the error
occurred."*; the dev guide's debugging chapter
(<https://rustc-dev-guide.rust-lang.org/compiler-debugging.html>): `=n` panics on
the nth error, default 1. **Verified.** The brief's premise that it is part of
how rustc bounded recovery is **false**.

**1.5 No error budget found.** The one source: mcy, 2020-02-06, *"This may be
just a symptom of rustc not having something like the go compiler's 'too many
errors' error"*
(<https://internals.rust-lang.org/t/poll-how-much-context-to-give-for-async-error-messages/11753/14>).
**Verified as a quotation.** The absence is a negative claim on my searches
(`error limit`, `error-limit`, `too many errors`, with `rustc`): a question, not
a fact.

**1.6 rustc never declared recovery done; it files what stays, by kind.** Label
`D-verbose`: *"Diagnostics: Too much output caused by a single piece of
incorrect code."* (<https://github.com/rust-lang/rust/labels?q=verbose>), this
sitting's class (a) by name; `D-incorrect`: *"Diagnostics: A diagnostic that is
giving misleading or incorrect information."*
(<https://github.com/rust-lang/rust/labels?q=D->), class (c). **Open `D-verbose`
issues on 2026-10-02: 87**; the oldest open,
[#30418](https://github.com/rust-lang/rust/issues/30418), 2015-12-16; the
newest listed, [#162656](https://github.com/rust-lang/rust/issues/162656),
2026-09-11 (<https://github.com/rust-lang/rust/issues?q=is%3Aissue%20state%3Aopen%20label%3AD-verbose%20sort%3Acreated-asc>).
**Verified.** Whether that count ever fell is **unverified** (I read today's
count only).

**1.7 Pins of every message, and of known-wrong output.** The dev guide's UI
tests chapter (<https://rustc-dev-guide.rust-lang.org/tests/ui.html>, fetched
2026-10-02): `.stderr` snapshots made with `--bless` and reviewed, and
*"`ERROR` and `WARN` kinds are required to be exhaustively covered by line
annotations `//~` by default"*. Its `known-bug` directive, *"for tests that
demonstrate a known bug that has not yet been fixed"*, kept as *"a sentinel that
will fail if the bug is incidentally fixed"*; *"Do not include error annotations
in a test with `known-bug`. The test should still include other normal
directives and stdout/stderr files."* **Verified.** It entered with
[#93953](https://github.com/rust-lang/rust/pull/93953), *"Add the `known-bug`
test directive, use it, and do some cleanup"*, jackh726, merged 2022-02-19,
milestone 1.60.0: *"the current output is a bug"* is pinned as current output.
**Verified.** Route (1f)'s shape, in production for four and a half years.

## 2. Swift and TypeScript

**2.1 SwiftParser documents best-effort recovery.** swift-syntax `main`,
SwiftParser.md
(<https://raw.githubusercontent.com/swiftlang/swift-syntax/main/Sources/SwiftParser/SwiftParser.docc/SwiftParser.md>,
fetched 2026-10-02): *"The parser will attempt to recover from syntax errors,
maintaining as much of the program structure as is feasible."* Input matching no
grammar is kept in "unexpected" nodes, required syntax that is absent becomes
"missing" tokens, and the tree round-trips byte for byte *"regardless of whether
the input text was well-formed Swift code"*. **Verified.**

**2.2 How Swift measured progress: a fixed list from the old parser's tests.**
Alex Hoppen, 2022-10-27
(<https://forums.swift.org/t/october-update-on-the-new-swift-parser/61071>): the
previous parser's test suite imported as 1821 diagnostic TODOs, *"1469 of these
have been resolved, leaving 352 remaining"*; *"40 hand-crafted diagnostics were
introduced to the parser"*; most missing- or unexpected-token diagnostics
generated automatically with Fix-Its. Douglas Gregor, 2022-09-23
(<https://forums.swift.org/t/update-on-the-new-swift-parser/60470>): tested by
round-trip and by valid-parse over about 15,000 compiler tests; in a reply,
*"the goal is that it's very easy to go add a new special-case diagnostic when
we see a new weird case."* **Verified.** Whether the 352 reached zero:
**unverified**. This is the nearest precedent to route (1a), and its list was
fixed at the start, not grown by discovery.

**2.3 Two parsers, the second silenced.** swiftlang/swift
[#62629](https://github.com/swiftlang/swift/pull/62629), *"Experimentally emit
diagnostics from the new Swift parser"*, DougGregor, merged 2022-12-16: it
*"emits diagnostics from the new Swift parser first for a source file. If that
produces any errors, we suppress any diagnostics emitted from the C++ parser."*
**Verified.** Whether it became the default, and in which release:
**unverified**.

**2.4 Swift's written rule tying recovery to the fix.** swiftlang/swift `main`,
`docs/Diagnostics.md`
(<https://raw.githubusercontent.com/swiftlang/swift/main/docs/Diagnostics.md>,
fetched 2026-10-02): *"If a fix-it is placed on an error or warning, it must be
the single, obvious, and very likely correct way to fix the issue."* and
*"Ideally, the compiler or other tool will recover as if the user had applied
the fix-it."* Its verifier: *"If the `-verify` frontend flag is used, the Swift
compiler will check emitted diagnostics against specially formatted comments in
the source"*, used *"extensively throughout the test suite"*. **Verified.**
Whether an unexpected diagnostic fails a Swift test is not said in the fetched
text: **unverified for Swift** (Clang's, § 7.2, is verified).

**2.5 TypeScript: one parse error per position.** `parseErrorAtPosition`, in
the lines microsoft/TypeScript
[#43460](https://github.com/microsoft/TypeScript/pull/43460/files) changed
(*"Only issue matching token errors on non-dupe locations"*, sandersn, merged
2021-03-31): `// Don't report another error if it would just be at the same
position as the last error.` above `if (!lastError || start !==
lastError.start)`. **Verified.** The rule stands in the removed lines too, so it
predates 2021-03-31; its origin is **unverified**.

**2.6 TypeScript: no type error while a syntax error stands.**
`emitFilesAndReportErrors`, `watch.ts` on branch `release-5.0`
(<https://raw.githubusercontent.com/microsoft/TypeScript/release-5.0/src/compiler/watch.ts>):
syntactic diagnostics are added first, and `getSemanticDiagnostics` is added
only `if (allDiagnostics.length === configFileParsingDiagnosticsLength)`, the
same condition guarding the options and global diagnostics. **Verified** at that
branch. A phase gate: while any syntactic diagnostic stands, tsc's command line
reports no type error at all.

**2.7 TypeScript's pins, and its corpus run.** CONTRIBUTING.md on `release-5.0`
(<https://raw.githubusercontent.com/microsoft/TypeScript/release-5.0/CONTRIBUTING.md>):
compiler tests write an `.errors.txt` baseline among others into
`tests/baselines/reference`, accepted with `hereby baseline-accept`; *"Be sure to
validate the changes carefully -- apparently unrelated changes to baselines can
be clues about something you didn't think of."* **Verified** (on `main` the
guide now names a `tsc/testdata` layout, fetched 2026-10-02). The TypeScript bot
wiki, last edited 2026-02-06
(<https://github.com/microsoft/TypeScript/wiki/Triggering-TypeScript-Bot>):
`user test this` *"runs the nightly-tested `user` suite against the PR and
against main (this takes around 30 minutes). The bot will post a summary comment
comparing results from the two."* **Verified.** A whole-output corpus run,
compared between two compilers on one corpus at one moment, run nightly and on
request rather than on every change: the shape route (1b)'s four facts ask for.

## 3. Go

**3.1 The limit, its number and its flag, since Go 1.** `cmd/compile`,
go1.27.1 (<https://pkg.go.dev/cmd/compile>): `-e` *"Remove the limit on the
number of errors reported (default limit is 10)."* At the `go1` tag, the C
compiler's doc.go
(<https://raw.githubusercontent.com/golang/go/go1/src/cmd/gc/doc.go>): *"normally
the compiler quits after 10 errors; -e prints all errors"*. Go 1 was released
2012-03-28; the newest listed, go1.27.0, 2026-08-19
(<https://go.dev/doc/devel/release>). **Verified.** The coordinator's memory
(10, `-e`) holds.

**3.2 One syntax error per line, since Go 1.** At `go1`, the C compiler's
subr.c (<https://raw.githubusercontent.com/golang/go/go1/src/cmd/gc/subr.c>):
`// only one syntax error per line` above `if(lastsyntax == lexlineno) return;`,
and `if(nsavederrors+nerrors >= 10 && !debug['e'])` printing `"%L: too many
errors\n"`. On `master` today, the Go compiler's print.go
(<https://raw.githubusercontent.com/golang/go/master/src/cmd/compile/internal/base/print.go>):
*"only one syntax error per line, no matter what error"* (`if
sameline(lasterror.syntax, pos) { return }`), *"only one of multiple equal
non-syntax errors per line"*, and `if numErrors >= 10 && Flag.LowerE == 0`
before `"%v: too many errors\n"`. **Verified.** The same two rules, from the C
compiler of 2012 to the Go compiler of 2026.

**3.3 The library parser says why.** go/parser's parser.go on `master`
(<https://raw.githubusercontent.com/golang/go/master/src/go/parser/parser.go>):
*"If AllErrors is not set, discard errors reported on the same line as the last
recorded error and stop parsing if there are more than 10 errors."*, with
`return // discard - likely a spurious error`; and a guard on recovery itself,
`syncPos` and `syncCnt`, *"used to limit the number of calls to parser.advance
w/o making scanning progress - avoids potential endless loops across multiple
parser functions during error recovery"*. The package doc (go1.27.1,
<https://pkg.go.dev/go/parser>): on syntax errors *"the result is a partial AST
(with ast.Bad* nodes representing the fragments of erroneous source code)"*.
**Verified.**

**3.4 `go vet` does not bear on recovery.** Its doc (go1.27.1,
<https://pkg.go.dev/cmd/vet>): *"Vet examines Go source code and reports
suspicious constructs"*, and *"it can find errors not caught by the
compilers"*. Its framework (golang.org/x/tools v0.50.0, 2026-09-08,
<https://pkg.go.dev/golang.org/x/tools/go/analysis>): `RunDespiteErrors`
*"allows the driver to invoke the Run method of this analyzer even on a package
that contains parse or type errors"*. **Verified.** That an analyzer is
therefore not run on such a package by default is my inference from the flag's
wording, not a sentence I read. vet runs after the parser; the brief's pairing
of it with the error limit is a **false premise**.

## 4. GCC, Clang, javac

**4.1 GCC: no limit by default, an opt-in cap since 4.6, a stop-at-first since
earlier.** The patch, Nathan Froyd, 2010-11-09, *"fix PR 44782, implement
-fmax-errors for C-family languages"*, the name chosen *"for compatibility with
the Fortran front end's"* option
(<https://gcc.gnu.org/legacy-ml/gcc-patches/2010-11/msg00897.html>): *"Limits the
maximum number of error messages to n, at which point GCC bails out rather than
attempting to continue processing the source code. If n is 0 (the default),
there is no limit on the number of error messages produced."* GCC 4.6's changes
(<https://gcc.gnu.org/gcc-4.6/changes.html>): *"The `-fmax-errors=N` option is
now supported."*; GCC 4.6.0 released 2011-03-25
(<https://gcc.gnu.org/releases.html>). Today's manual carries the option
(<https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html>, fetched 2026-10-02).
`-Wfatal-errors`: *"This option causes the compiler to abort compilation on the
first error occurred rather than trying to keep going and printing further error
messages."*, already in the GCC 4.4.3 manual, where `-fmax-errors` is absent
(<https://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Warning-Options.html>).
**Verified.** What PR 44782 asked for: **unverified** (the bugzilla refused the
fetch).

**4.2 Clang: a cap of 20.** User's Manual, in progress
(<https://clang.llvm.org/docs/UsersManual.html>, fetched 2026-10-02) and at
Clang 3.8 (<https://releases.llvm.org/3.8.1/tools/docs/UsersManual.html>), the
same words: *"Stop emitting diagnostics after 123 errors have been produced. The
default is 20, and the error limit can be disabled with -ferror-limit=0."* The
limit existed by 2011-07-29: Douglas Gregor on cfe-dev, *"The -ferror-limit logic
is clipping notes when it shouldn't"*, under a report whose output ends *"fatal
error: too many errors emitted, stopping now [-ferror-limit=]"*
(<https://lists.llvm.org/pipermail/cfe-dev/2011-July/016421.html>).
**Verified.** Its first release: **unverified**.

**4.3 Clang's recovery, as documented.** Internals Manual
(<https://clang.llvm.org/docs/InternalsManual.html>, fetched 2026-10-02):
*"Clang produces an AST even when the code contains errors. Clang won't generate
and optimize code for it, but it's used as parsing continues to detect further
errors in the input."* Of fix-its: *"Since they are automatically applied if
`-Xclang -fixit` is passed to the driver, they should only be used when it's very
likely they match the user's intent. Clang must recover from errors as if the
fix-it had been applied."*, and *"Fix-it hints on a warning must not change the
meaning of the code."* `RecoveryExpr`
(<https://clang.llvm.org/doxygen/classclang_1_1RecoveryExpr.html>): *"clang does
not report most errors on dependent expressions, so we get rid of bogus errors
for free"*, and *"One can also reliably suppress all bogus errors on expressions
containing recovery expressions by examining results of
`Expr::containsErrors()`."* **Verified.** Its motive was tooling: Sam McCall's
RFC, cfe-dev, 2019-05-09, *"IDE tools like clangd see a lot of broken code, and
rely on the resulting AST"*
(<https://lists.llvm.org/pipermail/cfe-dev/2019-May/062262.html>); the commit
*"[AST] Add RecoveryExpr to retain expressions on semantic errors"* (hokein,
Differential Revision D69330, a mirror at
<https://github.com/supython-coder/llvm-project/commit/733edf9750a4893d5f50329ad68b3901935303a9>):
*"clang can produce some new diagnostics now and we aim to suppress bogus ones
based on Expr::containsErrors"*. **Verified**; its date (2020-03-19, a search
snippet) **unverified**.

**4.4 "In file included from" is context, not recovery.** *"When gcc displays an
error message or warning, it also displays (for the first such message occurring
in an include file) the inclusion stack."* (Bruno Haible, gcc-bugs, 1999-02-03,
<https://gcc.gnu.org/ml/gcc-bugs/1999-02n/msg00084.html>). **Verified.** It is
where a message happened, so it bears on §4.17's *"carries all the context
needed to fix it"*, not on cascades. The brief's premise that it bounds
recovery is **false**.

**4.5 javac, not in the brief: the same rule as TypeScript's.** Log.java on
openjdk/jdk `master`
(<https://raw.githubusercontent.com/openjdk/jdk/master/src/jdk.compiler/share/classes/com/sun/tools/javac/util/Log.java>,
fetched 2026-10-02): `shouldReport`, *"Returns true if an error needs to be
reported for a given source name and pos."*, over a `recorded` set of (file,
position); `getDefaultMaxErrors()` returns 100; past it, `nsuppressederrors++`.
The JDK 21 man page
(<https://docs.oracle.com/en/java/javase/21/docs/specs/man/javac.html>):
`-Xmaxerrs` *"Sets the maximum number of errors to print."* JDK-8066843
(created 2014-12-06, fixed in 9, <https://bugs.openjdk.org/browse/JDK-8066843>):
two errors at one position printed only the first. **Verified.**

## 5. Elm, and a language that stops at one

**5.1 Elm on cascades.** *Compilers as Assistants*, Evan Czaplicki, 2015-11-19,
Elm 0.16
(<https://raw.githubusercontent.com/elm/elm-lang.org/master/pages/news/compilers-as-assistants.elm>):
"So with many compilers a single mistake can lead to 3 or 4 different error
messages, leaving the programmer to figure out which one is the real problem."
and "Well, there are no more cascading errors in Elm 0.16 thanks to Hacker
News!" The comment it links (Animats, 2015-06-30,
<https://news.ycombinator.com/item?id=9808317>): *"Whenever a type problem was
detected, the error was reported and the type of the failed object was changed
to an internal error type."* **Verified.** rustc's mechanism (§ 1.1), on types;
the post does not say which phase it covers.

**5.2 Elm on syntax.** *The Syntax Cliff*, 2019-10-21, Elm 0.19.1
(<https://raw.githubusercontent.com/elm/elm-lang.org/master/pages/news/the-syntax-cliff.elm>):
*"Elm has a rule that any definition must be defined on a fresh line. It cannot
have any spaces in front of it. One benefit of this rule is that the compiler
can always pinpoint the particular definition that contains a syntax error. No
more errors at the end of the file!"* **Verified**: a rule of the language,
chosen in part for where a syntax error is told. **That Elm reports one syntax
error at a time, and states it as a design choice: unverified.** I found no such
sentence, only examples worded as where the parser *"got stuck"*. Elm users who
needed recovery wrote it: the-sett/parser-recoverable, *"The aim is to help
create parsers that will tolerate syntax errors and get back on track and allow
parsing to continue"*, for *"writing an interactive editor"*
(<https://github.com/the-sett/parser-recoverable>, README, no version shown).
**Verified.**

**5.3 CPython stops at the first syntax error, by design.** InternalDocs
parser.md on python/cpython `main`
(<https://raw.githubusercontent.com/python/cpython/main/InternalDocs/parser.md>):
*"When a pegen-generated parser detects that an exception is raised, it will
automatically stop parsing, no matter what the current state of the parser
is."*; a second pass with the `invalid_` rules, *"By design this attempt cannot
succeed"*, to *"give to the invalid rules a chance to detect specific situations
where custom, more precise, syntax errors can be raised"*. **Verified.** The PEG
parser is PEP 617, *Python-Version: 3.9* (<https://peps.python.org/pep-0617/>).
Users who needed more built another parser: parso, *"a Python parser that
supports error recovery and round-trip parsing ... Parso is also able to list
multiple syntax errors in your python file."*
(<https://raw.githubusercontent.com/davidhalter/parso/master/README.rst>).
**Verified.**

**5.4 The trade-off every suppression pays, in a textbook.** Robert Nystrom,
*Crafting Interpreters*, chapter *Parsing Expressions*, on panic mode
(<https://craftinginterpreters.com/parsing-expressions.html>, fetched
2026-10-02): "Any additional real syntax errors hiding in those discarded tokens
aren't reported, but it also means that any mistaken cascaded errors that are
side effects of the initial error aren't falsely reported either, which is a
decent trade-off." **Verified.** This sitting's class (b) and class (a), named
against each other: a rule that removes one buys the other.

## 6. Measured recovery quality

**6.1 Ripley and Druseikis 1978.** G. David Ripley, Frederick C. Druseikis, *A
statistical analysis of syntax errors*, Computer Languages 3(4):227-240, 1978
(<https://ftp.math.utah.edu/pub/tex/bib/idx/complngs/3/4/227_240.html>; DOI
10.1016/0096-0551(78)90041-3 resolves to Elsevier's pii 0096055178900413, which
refused the fetch). **Verified** (the record). Its abstract, as quoted in John R.
Levine's comp.compilers post of 1986-03-21
(<https://compilers.iecc.com/comparch/article/86-03-005>): *"A study of errors
made by Pascal programmers is described. The results of this study are
discussed in relation to compiler syntax error recovery procedures. It is found
that syntax errors made in practice are quite simple and occur relatively
infrequently (generally at most one per sentence of the language). Also a few
types of errors account for most occurences."* **Verified as that post quotes
it.** The size, 589 programs: **unverified** (a search summary only). Its
programs were sought as a benchmark for years: *"if anyone has or knows where I
can find the Ripley-Druseikis(%) test suite, please let me know"* (Dave
Schaumann, comp.compilers, 1992-09-02,
<https://compilers.iecc.com/comparch/article/92-09-021>). **Verified.** Bearing:
*"generally at most one per sentence"* is the oldest empirical ground under the
instrument's one planted mistake per program.

**6.2 Pennello and DeRemer's rating: a missed error counts against recovery.**
As reported by de Jonge and Visser 2012 (§ 6.3): *"A recovery is rated excellent
if it is the one a human reader would make, good if it results in a reasonable
program without spurious or missed errors, and poor if it introduces spurious
errors or if excessive token deletion occurs."*, citing Pennello and DeRemer,
*A forward move algorithm for LR error recovery*, POPL 1978, 241-254.
**Verified as reported; the POPL paper not read.** Medeiros and Mascarenhas
(SAC 2018, <https://ar5iv.arxiv.org/html/1806.11150>) adapted it to 180
hand-written invalid Lua programs, each written to raise one specific label:
excellent 100 (about 56%), good 63 (about 35%), poor 17 (about 9%), failed 0.
**Verified.** A rate over a rated single-mistake corpus is a definition of
quality with a 1978 ancestry.

**6.3 De Jonge and Visser 2012: the instrument's direct ancestor.** Maartje de
Jonge, Eelco Visser, *Automated Evaluation of Syntax Error Recovery*, ASE 2012,
322-325 (<https://eelcovisser.org/publications/2012/JongeV12.pdf>, read whole):
test files *"generated by a mutation based fuzzing technique that applies
knowledge about common syntax errors"*, each with an oracle AST, scored by a tree
difference; accidentally legal mutants *"filtered out by parsing them with error
recovery turned off"* (the instrument's LEGAL); validated on *"135 erroneous Java
programs"* against Pennello and DeRemer's criteria (a difference of at most one
goes with excellent, above 20 with poor). Its purpose, in its words: *"An
objective and automated evaluation method is essential to do benchmark
comparisons between existing techniques, and to detect regression in recovery
quality due to adaptations of the parser implementation."* And: *"According to
our knowledge, test generation techniques have not yet been applied to recovery
evaluation."* **Verified.** What happened next: Diekmann and Tratt call it
*"some early work in this area"* in 2020 (§ 6.4). **I found no evidence that it,
or anything like it, ran for years inside a production compiler's tests**; a
negative claim on my searches (`mutation`, `error recovery`, `regression`,
`benchmark`).

**6.4 Diekmann and Tratt 2020: cascades counted on a corpus.** *Don't Panic!
Better, Fewer, Syntax Errors for LR Parsers*, ECOOP 2020, LIPIcs 166, article 6,
DOI 10.4230/LIPIcs.ECOOP.2020.6, read in arXiv 1804.07133v4 of 2020-07-03, which
carries the ECOOP footer (<https://arxiv.org/abs/1804.07133>). On *"a corpus of
200,000 real-world syntactically invalid Java programs"* (Blackbox), CPCT+
repairs *"98.37%±0.017% of files within a timeout of 0.5s"* and *"reports
435,812±473 error locations to the user, reducing the cascading error problem
substantially relative to the 981,628±0 error locations reported by panic
mode"*; their Figure 11 adds Corchuelo et al. at 374,731±26 with a 5.54% failure
rate. **Verified** (a search engine's summary gave 98.38% and 435,824±480, not
what the PDF says). Five sentences that bear on this sitting, all verified:

- p. 6:15, the limit of their measure: *"the number of error locations only
  allows relative comparisons. Although we know that the corpus contains at
  least 200,000 manually created errors (i.e. at least one per file), we cannot
  know if, or how many, files contain more than one error. Since we cannot know
  the true number of error locations, we are unable to evaluate algorithms in an
  absolute sense."*
- p. 6:22, on mutation: *"One solution is to mutate correct source files (e.g.
  randomly deleting tokens), thus obtaining incorrect inputs which we can later
  test: however, it is difficult to uncover and then emulate the numerous,
  sometimes surprising, ways that humans make syntax errors"*.
- p. 6:2, what users do: *"Programmers quickly learn that only the location of
  the first error in a file"*, *"not the reported repair, nor the location of
  subsequent errors"*, *"can be relied upon to be accurate."* And on cost, the
  Eclipse IDE's hand-written Java recovery is *"5KLoC long"*.
- p. 6:19, what happened to the algorithms: *"Although several members of the
  Fischer et al. family were implemented in parsing tools of the day, to the
  best of our knowledge none of those implementations have survived."*
- p. 6:23 to 6:24, a route nobody listed: noncorrecting recovery (Richter,
  TOPLAS 7(3), 1985), whose suffix errors *"are guaranteed to be genuine syntax
  errors"* at the price that *"some genuine syntax errors are missed"*, not
  adopted for two reasons they give.

Bearing on (1b): the instrument plants a known number of mistakes, so its ONE
is an absolute count where theirs was relative; the threat they name, mutants
unlike real mistakes, is the one it carries.

**6.5 Exchanges measured, never used as a gate.** RustAssistant (Deligiannis,
Lal, Mehrotra, Poddar, Rastogi, ICSE 2025, April 2025,
<https://www.microsoft.com/en-us/research/publication/rustassistant-using-llms-to-fix-compilation-errors-in-rust-code/>):
an LLM with iteration reaches *"an impressive peak accuracy of roughly 74% on
real-world compilation errors in popular open-source Rust repositories"*. Seo,
Sadowski, Elbaum, Aftandilian, Bowdidge, ICSE 2014
(<https://research.google/pubs/programmers-build-errors-a-case-study-at-google/>):
*"26.6 million builds produced during a period of nine months"*, with the effort
to resolve each kind of error. **Verified** (abstracts).

## 7. Pinning every message a broken program gets

- **7.1 rustc**: § 1.7 (`.stderr`, every `ERROR` annotated, `known-bug`).
- **7.2 Clang `-verify`**: the Internals Manual: the diagnostics buffer
  *"captures and remembers the diagnostics as they fly by. Then `-verify`
  compares the list of produced diagnostics to the list of expected ones. If they
  disagree, it prints out its own output."*; expectations are comments such as
  `// expected-error {{use of undeclared identifier 'B'}}`
  (VerifyDiagnosticConsumer.h at `release_90`,
  <https://llvm.googlesource.com/clang/+/refs/heads/release_90/include/clang/Frontend/VerifyDiagnosticConsumer.h>).
  **Verified.**
- **7.3 Swift `-verify`**: § 2.4.
- **7.4 Go**: go/parser's error_test.go
  (<https://go.dev/src/go/parser/error_test.go?m=text>): *"Expected errors are
  indicated in the test files by putting a comment of the form /* ERROR "rx" */
  immediately following an offending token. The harness will verify that an
  error matching the regular expression rx is reported at that source
  position."*; an unannotated error fails with `"%s: unexpected error: %s"`, a
  missing one with `"%d errors not reported:"`. **Verified.** Go's
  `test/fixedbugs` existed at the `go1` tag
  (<https://github.com/golang/go/tree/go1/test/fixedbugs>): the name of this
  tree's `fixedbugs-NNN` cases has that precedent (whether it was borrowed:
  **unverified**).
- **7.5 TypeScript**: § 2.7.
- **7.6 The mechanical loop of route (1g)**: `cargo fix`, cargo's module docs
  at 1.101.0-nightly
  (<https://doc.rust-lang.org/nightly/nightly-rustc/cargo/ops/cargo_fix/index.html>):
  rustfix applies rustc's suggestions; *"If rustfix fails to apply any
  suggestions (for example, they are overlapping), but at least some suggestions
  succeeded, it will try the previous two steps up to 4 times as long as some
  suggestions succeed."*; then *"rustc is run again to verify the suggestions
  didn't break anything. The change will be backed out if it fails (unless
  `--broken-code` is used)."* rustc's suggestions carry a confidence *"from high
  (`Applicability::MachineApplicable`) to low (`Applicability::MaybeIncorrect`)"*
  (<https://rustc-dev-guide.rust-lang.org/diagnostics.html>): the precedent of
  this tree's `certain` and `guess`. **Verified.**

## 8. Bearing on Q2 to Q5 (precedents only)

- **Q2** (an open `[`, then a `)`): rustc tells a closer of another kind in ONE
  message naming both, `error: mismatched closing delimiter`, a label
  *"unclosed delimiter"* on the opener and *"mismatched closing delimiter"* on
  the closer, then *"aborting due to 1 previous error"* (rust-lang/rust at
  `97b1c314`, parser-recovery-2.stderr,
  <https://rust.googlesource.com/rust/+/97b1c314892ef4497c0ce5656daa3a54c4e052d3/tests/ui/parser/parser-recovery-2.stderr>;
  the commit's date not shown). **Verified as fetched.** The reworded-message
  route has this precedent, and § 1.3's exit after it.
- **Q3** (should the parser know where the lexer joined lines): in both designs
  I read, the lexer's line decisions reach the parser as tokens. Go inserts a
  semicolon *"into the token stream immediately after a line's final token"*
  when it is one of a listed kind (the Go spec, *Language version go1.27 (May
  26, 2026)*, <https://go.dev/ref/spec>); Python ends a logical line with
  `NEWLINE`, and *"There is no NEWLINE token between implicit continuation
  lines"*, with `INDENT` and `DEDENT` from a stack (the Python 3.14.8 reference,
  <https://docs.python.org/3/reference/lexical_analysis.html>). **Verified.**
  That a recovery skipping "to the end of the line" then skips joined lines with
  it is my inference, unmeasured.
- **Q4** (defect 172, a `certain` fix `check` cannot know): Swift asks a fix-it
  on an error to be *"the single, obvious, and very likely correct way to fix the
  issue"*, Clang *"very likely they match the user's intent"*, because `-fixit`
  applies them (§ 2.4, § 4.3). By that standard a fix `build` refuses where the
  header's type is 64 bits is not certain; my reading.
- **Q5** (per error or per program): practice reads per error (§ 6.4, only the
  first error is trusted); the rating literature reads per program, a missed
  error making a recovery not good (§ 6.2). Every recovery rule I sourced is
  written in a compiler's documentation or source comments (Go), a contributor
  guide (Clang's Internals Manual, Swift's Diagnostics.md, the rustc-dev-guide)
  or an issue label (rustc), none in a language reference; a negative claim on
  the two references I opened (§ 8 Q3), so a question.

## verdict

**approve (advisory)**, of a composite: (1f) as the definition, (1d) as the
repair, (1b) in its differential form as the instrument, (1c) as the filing,
(1g) as the measure. Per route:

| route | verdict | rests on |
|---|---|---|
| (1a) every audit row closed | **object** | no production compiler found that declared recovery done by emptying a list it kept discovering (§ 1.6: rustc's class-(a) label, oldest open 2015-12-16); Swift's nearest case was a list fixed at the start, its end unverified (§ 2.2). Fine as the inner rule of (1f) |
| (1b) the instrument's totals, ratcheted in the net | **approve in a differential form; object to a ratchet on raw totals** | de Jonge and Visser built this instrument for *"regression in recovery quality"*, as research (§ 6.3); counts on a corpus *"only allow relative comparisons"* (§ 6.4); TypeScript compares PR against main on one corpus, about 30 minutes, nightly and on request (§ 2.7). Differential: base and head compiler on the same frozen corpus at the round's gate, a moved pair being the finding. No ratchet on totals found in any compiler's CI |
| (1c) the classes alone | **approve, with a warning** | the longest-running precedent (§ 1.6, rustc's D-labels, ten years, 87 open today). The `adjacent` clock is a deliberate departure with no precedent I found; expect it to fire |
| (1d) a structural change | **approve** | Go's one syntax error per line and limit of 10, the same in 2012 and 2026 (§ 3.2); one per position in TypeScript and javac (§ 2.5, § 4.5); error types in rustc, Elm, Clang (§ 1.1, § 5.1, § 4.3); *"recover as if the fix-it had been applied"*, written in Clang and Swift (§ 4.3, § 2.4). Its cost is § 5.4's trade and § 1.3's lost hint |
| (1f) pin what stays | **approve** | rustc's `known-bug` since 1.60.0, 2022-02-19, *"a sentinel that will fail if the bug is incidentally fixed"* (§ 1.7); every `ERROR` annotated in rustc, Clang, Go (§ 7) |
| (1g) §4.17's count of exchanges | **approve as the measure, not as the sole gate** | `cargo fix` runs the loop in production (§ 7.6); exchanges measured in research (§ 6.5); none found as a compiler's acceptance gate, so its baseline is measured before any threshold |
| (1h) the seven rulings, adopted or reopened | **approve** | where precedent accepts an extra or withheld message, the ruling is written beside the code that applies it: *"discard - likely a spurious error"* (§ 3.3), *"Don't report another error if it would just be at the same position as the last error"* (§ 2.5) |
| (1e) unlisted: the differential corpus run | **approve** | § 2.7 (it answers (1b)'s live-corpus fact: both compilers plant the same mutants at one moment) |
| (1e) unlisted: first parse error only | **object** | CPython stops at the first by design (§ 5.3); its users who needed more built a second parser (parso), which CLAUDE.md § 10's one-binary rule refuses here |
| (1e) unlisted: an error cap | **object as a definition** | Go 10, Clang 20, javac 100, GCC none (§ 3.1, § 4.1, § 4.2, § 4.5): a backstop on volume for fourteen years and more, closing neither (a) nor (b) |

**Which routes ran for years, which have none, and what users did.** Ran for
years: (1c), (1d), (1f), and the caps. None found as a definition of done: (1a),
(1b) as a ratchet, (1g) as a gate. When recovery fell short, users trusted only
the first error (§ 6.4), asked the compiler to stop (`-Wfatal-errors`,
`-fmax-errors`, § 4.1), built a second tolerant parser for their tools
(rust-analyzer's, whose docs make *"Parsing is resilient"* a design goal,
<https://git.dreamy.place/mirrors/rust/plain/src/tools/rust-analyzer/docs/dev/syntax.md?h=1.77.1>;
parso; parser-recoverable; swift-syntax; Clang's `RecoveryExpr` for clangd), and
filed issues that are still open (§ 1.6). All verified at the sections cited.

**The brief's premises, checked** (the critic's § 5 list): Go's 10 and `-e`
hold, since Go 1; `go vet` does not bear on recovery (§ 3.4). GCC's and Clang's
options hold, defaults none and 20; the include chains are context (§ 4.4).
rustc's suppression is real and documented (§ 1.1); `treat-err-as-bug` is a
debugging flag (§ 1.4); no budget found (§ 1.5). Elm's *"no more cascading
errors"* holds for 0.16, by an error type (§ 5.1); *"one error at a time, by
design"* is unverified (§ 5.2). Swift's best-effort recovery holds (§ 2.1);
follow-on suppression holds for TypeScript (§ 2.5, § 2.6) and, experimentally in
2022, for Swift (§ 2.3).

## argument

No production compiler I could source ever declared its parser's recovery
finished. Each bounded it instead, and the bounds ran for years: one syntax
error per line and a limit of 10 (Go, from Go 1 in 2012 to Go 1.27.1), one error
per position (TypeScript, javac), an error type that silences what follows
(rustc, Elm 0.16, Clang), a cap (GCC since 4.6, Clang's 20), a filed backlog
(rustc's D-verbose: 87 open, the oldest from 2015), and pins of known-wrong
output (rustc's `known-bug` since 1.60). Done, by precedent, is a bound plus a
pin. Every suppression trades a second message for a hidden mistake, and rustc's
delimiter exit lost a hint the day it merged.

## prediction

**P1, on the instrument** (unrun; the coordinator's to run, about 14 minutes at
`--jobs 3` by F4). If a (1d) rule of Go's shape lands (after a failed construct,
nothing more is told until the next statement or line), then on the frozen
corpus, in one run, EXTRA falls below 439 **and** HIDDEN parse-stage rises above
0 of 14,684 pairs. If EXTRA falls while HIDDEN stays 0, the trade every
precedent pays (§ 5.4, § 3.3, § 1.3) does not bind this parser and my warning on
(1d) is wrong. **A question first**: whether any of the 14,684 pairs puts both
mistakes in one statement or line. If none does, P1 cannot fail, and the
instrument cannot see the cost the precedents name.

**P2, on the list.** Under (1c) without (1d), at least one of the cluster's
`adjacent` items (177 to 182, or 131's rows refiled) is still open when the
second milestone tag after its filing is placed, and `records/tagged` counts it
`blocking`. The base rate is § 1.6.

## condition

- **(1b)**: a production compiler whose CI gates on a recovery-quality total,
  over a mutation or real-error corpus, for years. I searched (mutation, error
  recovery, regression, benchmark; de Jonge and Visser's future work;
  TypeScript's `user` suite, which compares two compilers rather than holding a
  total) and found none. One would turn my objection to the raw-total ratchet
  into approval.
- **(1a)**: evidence that Swift's 352 (2022-10-27) reached zero and its team
  called the recovery done: a closable list that closed, and I would approve
  (1a) over a frozen list.
- **(1d)**: evidence that Go's per-line rule was reverted, or drew a standing
  class of issues for hiding real errors, in its fourteen years: my approval
  would become conditional.
- **(1c)**: a project whose class-(a) backlog drained under an aging clock like
  `adjacent` becoming `blocking`: I would drop the warning.

## Not verified, and not relied on

- Ripley and Druseikis: the 589 programs; the abstract read only as quoted in
  1986 (§ 6.1).
- Pennello and DeRemer's scale: read only as reported in 2012 and 2018; the
  POPL 1978 paper not read (§ 6.2).
- Elm reports one syntax error at a time, by design (§ 5.2).
- Swift: whether `ParserDiagnostics` became the default, and when; whether the
  352 reached zero; whether `-verify` fails on an unexpected diagnostic (§ 2.2
  to 2.4).
- rustc: since when `TyKind::Error` suppresses; whether the D-verbose count ever
  fell; the absence of an error limit (§ 1.1, § 1.5, § 1.6).
- Clang: the release that first carried `-ferror-limit`; `RecoveryExpr`'s date
  (§ 4.2, § 4.3).
- GCC: what PR 44782 asked (§ 4.1).
- TypeScript: the origin of the same-position rule (§ 2.5).
- Whether this tree's `fixedbugs-` names were borrowed from Go (§ 7.4).
- design.md's historical appendix: not read. Whether it already names any of
  these precedents, or departs from one of them in silence, is a question for
  the coordinator, who can read the trunk.
