# Panel 232, the historian's report

Written as it went, 2026-10-29, by the historian seat. Every precedent below
was read this session by a web fetch or search run by this seat; its URL and
the words the page states are given, and it is marked `verified` only where the
page itself was read. A page that could not be opened is not cited as
evidence and is marked `unverified`. Each entry says what the page states and
nothing of what Heroes did or should do about it; the seat's own advisory
reading is kept apart, at the end. The book *Heroes of code* was not used as a
source for any line below.

Method note: pages were read through the WebFetch tool, which returns a
model's extraction of the page asked for verbatim quotations. Where a quotation
was load-bearing it was asked for twice, in two differently worded fetches,
and the two readings are said to agree where they did. Some extractions were
cut by the tool at about 125 characters; a quotation ending in `...` or
marked *fragment* is a cut sentence, exact as far as it goes. Three documents
the tool could not turn into text (P0145R3, N1570 and Armstrong's thesis, all
PDFs) were saved by it and read here page by page as images; their quotations
are transcribed from those page images.

## Item 1. The promise and its mechanism

### 1.1 Go 1: *Go 1 and the Future of Go Programs*

- URL: https://go.dev/doc/go1compat, read 2026-10-29, twice. Page title:
  *Go 1 and the Future of Go Programs*; its top-level headings, in order:
  *Introduction*, *Expectations*, *Sub-repositories*, *Operating systems*,
  *Tools*. **verified**
- The release it governs: Andrew Gerrand, *Go version 1 is released*, 28 March
  2012, https://go.dev/blog/go1, read 2026-10-29: *"a stable foundation for
  creating reliable products, projects, and publications"* (fragment); *"those
  programs will continue to compile and run without change"* (fragment).
  **verified**
- **What it promises**, in its words: *"Go 1 defines two things: first, the
  specification of the language; and second, the specification of a set of
  core APIs, the "standard packages" of the Go library."* And: *"It is
  intended that programs written to the Go 1 specification will continue to
  compile and run correctly, unchanged, over the lifetime of that
  specification."* And: *"At some indefinite point, a Go 2 specification may
  arise, but until that time, Go programs that work today should continue to
  work even as future "point" releases of Go 1 arise (Go 1.1, Go 1.2, etc.)."*
  And: *"The APIs may grow, acquiring new packages and features, but not in a
  way that breaks existing Go 1 code."*
- **The list's preamble**: *"Although we expect that the vast majority of
  programs will maintain this compatibility over time, it is impossible to
  guarantee that no future change will break any program. ... There are a
  number of ways in which a program that compiles and runs today may fail to
  do so after a future point release. They are all unlikely but worth
  recording."*
- **The eight listed exclusions**, each the page's heading and words:
  1. *Security*: *"A security issue in the specification or implementation
     may come to light whose resolution requires breaking compatibility. We
     reserve the right to address such security issues."*
  2. *Unspecified behavior*: *"The Go specification tries to be explicit about
     most properties of the language, but there are some aspects that are
     undefined. Programs that depend on such unspecified behavior may break in
     future releases."*
  3. *Specification errors*: *"If it becomes necessary to address an
     inconsistency or incompleteness in the specification, resolving the issue
     could affect the meaning or legality of existing programs. We reserve the
     right to address such issues, including updating the implementations.
     Except for security issues, no incompatible changes to the specification
     would be made."*
  4. *Bugs*: *"If a compiler or library has a bug that violates the
     specification, a program that depends on the buggy behavior may break if
     the bug is fixed. We reserve the right to fix such bugs."*
  5. *Struct literals*: unkeyed literals of another package's struct *"would
     fail to compile"* after a field is added; *"code that uses keyed literals
     ... will continue to compile"*.
  6. *Methods*: an added method may conflict with an embedded type's; *"We
     cannot protect against this rare case and do not guarantee compatibility
     should it arise."*
  7. *Dot imports*: *"using it may cause a program to fail to compile in
     future releases."*
  8. *Use of package `unsafe`*: *"Packages that import unsafe may depend on
     internal properties of the Go implementation. We reserve the right to make
     changes to the implementation that may break such programs."*
- **The list is closed by**: *"Of course, for all of these possibilities,
  should they arise, we would endeavor whenever feasible to update the
  specification, compilers, or libraries without affecting existing code."*
- **Exclusions the page makes OUTSIDE the eight-item list** (the brief asked
  for every one, and the critic's F27 counted the list's eight headings only):
  - *Source, not binary*: *"Compatibility is at the source level. Binary
    compatibility for compiled packages is not guaranteed between releases.
    After a point release, Go source will need to be recompiled to link
    against the new release."*
  - *One direction only*: *"code that runs under Go 1.2 should be compatible
    with Go 1.2.1, Go 1.3, Go 1.4, etc., although not necessarily with Go 1.1
    since it may use features added only in Go 1.2"*.
  - *Unreleased features*: *"No promise of compatibility is made for software
    using such features until they have been released."*
  - *Performance*: *"No guarantee can be made about the performance of a given
    program between releases."*
  - *Sub-repositories*: *"may be developed under looser compatibility
    requirements."*
  - *Operating systems*: *"It is impossible to guarantee long-term
    compatibility with operating system interfaces, which are changed by
    outside parties. The syscall package is therefore outside the purview of
    the guarantees made here."*
  - *Tools*: *"the Go toolchain (compilers, linkers, build tools, and so on) is
    under active development and may change behavior. This means, for instance,
    that scripts that depend on the location and properties of the tools may be
    broken by a point release."*
- **Whether a program declares a version**: asked of the page directly, it
  does not mention GODEBUG, `go.mod`, a `go` line, or a program declaring a Go
  version (second fetch, *"Not present"*). The mechanism that came later is
  §1.2 below.
- **Against the critic's F27**: the eight headings and their order match the
  page as read here. The page's other out-of-scope statements (source-level
  only, one direction, performance, the syscall package, the tools) are not
  among the eight and were not in F27.

### 1.2 Go after Go 1: the `go` line becomes a declared version (what happened to them)

- **Russ Cox, *Backward Compatibility, Go 1.21, and Go 2*, 14 August 2023**,
  https://go.dev/blog/compat, read 2026-10-29. **verified**
  - *"Go 1.21 expands and formalizes the use of GODEBUG."* *"A program's
    GODEBUG settings are configured to match the Go version listed in the main
    package's go.mod file."*
  - *"There will not be a Go 2 that breaks Go 1 programs."* *"Go 2, in the
    sense of breaking with the past and no longer compiling old programs, is
    never going to happen."*
  - Changes the post itself names as having broken programs under the Go 1
    promise: the sort algorithm (Go 1.6), *"Changing sort algorithms often
    changes how equal elements are ordered, and that happened here."*;
    HTTP/2 (Go 1.6); SHA-1 certificates disabled by default (Go 1.18, *"with a
    GODEBUG setting to override that change"*); `net.ParseIP` (Go 1.17, *"we
    decided to change net.ParseIP in Go 1.17 to reject leading zeros
    entirely"*); `ParseInt` (Go 1.13, *"much later we heard from an external
    user whose code did break"*); `compress/flate` (Go 1.8, *"it broke a
    project inside Google that needed reproducible archive builds"*).
- ***Go, Backwards Compatibility, and GODEBUG***, https://go.dev/doc/godebug,
  read 2026-10-29. **verified**
  - *"If code depends on buggy (including insecure) behavior, then fixing the
    bug will break that code. ... These kinds of changes are unavoidable and
    permitted by the Go 1 compatibility rules. Even so, Go provides a mechanism
    called GODEBUG to reduce the impact such changes have on Go developers
    using newer toolchains to compile old code."*
  - *"When compiling a work module or workspace that declares an older Go
    version, the Go toolchain amends its defaults to match that older Go
    version as closely as possible."* *"As an exception, GODEBUGs introduced
    for security releases will have the new behavior apply to all versions."*
  - *"GODEBUG settings added for compatibility will be maintained for a minimum
    of two years (four Go releases). Some, such as http2client and http2server,
    will be maintained much longer, even indefinitely."*
  - *"starting in Go 1.21, a main package's source files can include one or
    more //go:debug directives at the top of the file"*.
- ***Go Toolchains***, https://go.dev/doc/toolchain, read 2026-10-29.
  **verified**
  - *"The go line declares the minimum required Go version for using the module
    or workspace."* *"The go line for each module sets the language version the
    compiler enforces when compiling packages in that module."*
  - *"The Go toolchain refuses to load a module or workspace that declares a
    minimum required Go version greater than the toolchain's own version."*
    With the default `GOTOOLCHAIN=auto`, *"When the go or toolchain line is
    newer than the bundled toolchain, the go command runs the newer toolchain
    instead."*
  - *"Go 1.21 changed the go line to be a mandatory requirement instead."*
    *"Before Go 1.21, Go toolchains treated the go line as an advisory
    requirement: if builds succeeded the toolchain assumed everything worked,
    and if not it printed a note about the potential version mismatch."* And
    the backport: *"Go 1.19 releases starting at Go 1.19.13 and Go 1.20
    releases starting at Go 1.20.8, refuse to load workspaces or modules
    declaring version Go 1.22 or later."*
- **The specification's own appendix, *Language versions*** (the Go
  specification, https://go.dev/ref/spec, headed *"Language version go1.27
  (May 26, 2026)"*, read 2026-10-29 from offset 200,000). **verified** *"The
  Go 1 compatibility guarantee ensures that programs written to the Go 1
  specification will continue to compile and run correctly, unchanged, over
  the lifetime of that specification. More generally, as adjustments are made
  and features added to the language, the compatibility guarantee ensures that
  a Go program that works with a specific Go language version will continue to
  work with any subsequent version."* And: *"Source code containing an integer
  literal such as 0b1011 will be rejected if the implied or required language
  version used by the compiler is older than Go 1.13."* Its Go 1.22 entry:
  *"In a "for" statement, each iteration has its own set of iteration
  variables rather than sharing the same variables in each iteration."*
- **A language meaning gated by the declared version**: David Chase and Russ
  Cox, *Fixing For Loops in Go 1.22*, 19 September 2023,
  https://go.dev/blog/loopvar-preview, read 2026-10-29. **verified** *"For
  Go 1.22, we plan to change for loops to make these variables have
  per-iteration scope instead of per-loop scope."* *"the new semantics will
  only apply in packages contained in modules that declare go 1.22 or later in
  their go.mod files."* On its trial at Google: *"in the past four months we
  have had zero reports of any problems in production code"*, and *"we did have
  to correct many buggy tests that were not testing what they thought they
  were."*
  - The Go 1.22 release notes, https://go.dev/doc/go1.22, read 2026-10-29
    (**verified**), state the change in their *Changes to the language*
    without a version condition (*"In Go 1.22, each iteration of the loop
    creates new variables, to avoid accidental sharing bugs."*); the `go.mod`
    condition appears there only in the `vet` section (*"When analyzing a file
    that requires Go 1.22 or newer (due to its go.mod file or a per-file build
    constraint)"*). The gate is stated by the blog post above and by the
    toolchain page's *"sets the language version the compiler enforces"*.

### 1.3 Rust: *Stability as a Deliverable*, 1.0, RFC 1122, and editions

- **Aaron Turon and Niko Matsakis, *Stability as a Deliverable*, 30 October
  2014**, https://blog.rust-lang.org/2014/10/30/Stability/ (the URL without
  the trailing slash redirects there), read 2026-10-29. **verified**
  - The phrase: *"...now widely used to provide stability without
    stagnation:"* (fragment, cut by the tool).
  - The promise: *"To put it simply, our responsibility is to ensure that you
    never dread upgrading Rust."* *"If your code compiles on Rust stable 1.0, it
    should compile with Rust stable 1.x with a minimum of hassle."* *"Our
    promise is that, if you are using the stable release of Rust, you will
    never dread upgrading to the next release."*
  - The mechanism, channels and gates: *"New features and new APIs will be
    flagged as unstable via feature gates and stability attributes
    respectively."* *"Unstable features and standard library APIs will only be
    available on the nightly branch"* (fragment). *"...stable means stable: the
    stable channel provides only stable features."*
  - What it keeps the right to break: *"We reserve the right to fix compiler
    bugs, patch safety holes, and change type inference"* (fragment). *"We do
    not expect any of these changes to cause headaches when upgrading Rust."*
- **The Rust Core Team, *Announcing Rust 1.0*, 15 May 2015**,
  https://blog.rust-lang.org/2015/05/15/Rust-1.0/, read 2026-10-29.
  **verified** *"The 1.0 release marks the end of that churn."* *"This release
  is the official beginning of our commitment to stability"* (fragment).
  *"From this point forward, breaking changes are largely out of scope"*
  (fragment), linking to rust-lang/rfcs pull request 1122 for *"some minor
  caveats apply, such as compiler bugs."* The phrase *stability without
  stagnation* does not appear on this page (the fetch, asked for it).
- **RFC 1122, *Language semver***, start date 2015-05-07,
  https://rust-lang.github.io/rfcs/1122-language-semver.html, read
  2026-10-29. **verified** *"This RFC has the goal of defining what sorts of
  breaking changes we will permit for the Rust language itself,"*. *"This RFC
  proposes that minor releases may only contain breaking changes that fix
  compiler bugs or other type-system issues"* (fragment). *"Primarily, this
  means soundness issues where "innocent" code can cause undefined behavior
  (in the technical sense),"*. And of the specification's gaps: *"There are a
  number of areas where the precise language semantics are currently somewhat
  underspecified."* *"Changes of this nature should be treated as soundness
  changes,"*. *"Some details of type inference may change."* How:
  *"The first step then is to evaluate the impact of the fix on the crates
  found in the crates.io website"*; *"Begin with a deprecation or other
  warning before issuing a hard error."*
- **RFC 2052, editions** (filed as *epochs*; the page as read today says
  *edition*), start date 2017-06-26,
  https://rust-lang.github.io/rfcs/2052-epochs.html, read 2026-10-29.
  **verified**
  - *"Each crate can declare an edition in its Cargo.toml like edition =
    "2019"; otherwise it is assumed to have edition 2015"*.
  - *"Rust compilers must support all extant editions, and a crate dependency
    graph may involve several different editions simultaneously."* *"Thus,
    editions do not split the ecosystem nor do they break existing code."*
  - *"There are only two things a new edition can do that a normal release
    cannot:"* *"Change an existing deprecation into a hard error."* *"Change an
    existing deprecation to deny by default, and leverage the corresponding
    lint setting"*. *"This is the only kind of breaking change a edition opt-in
    can make."*
  - *"TL;DR: Warning-free code on edition N must compile on edition N+1 and
    have the same behavior."* *"The Rust compiler supports multiple editions,
    but must only support a single version of "core Rust"."*
- **The Edition Guide, *What are editions?***,
  https://doc.rust-lang.org/edition-guide/editions/index.html, read
  2026-10-29. **verified** *"Rust uses editions to solve this problem. When
  there are backwards-incompatible changes, they are pushed into the next
  edition."* *"Each crate chooses its edition within its Cargo.toml file."*
  *"crates in one edition must seamlessly interoperate with those compiled with
  other editions."* *"changes found in new Rust editions tend to be 'skin
  deep'."* *"When creating a new crate with Cargo, it will automatically
  select the newest stable edition."*

### 1.4 Nim: *Version 1.0 released*, and after

- ***Version 1.0 released*, 23 September 2019**,
  https://nim-lang.org/blog/2019/09/23/version-100-released.html, read
  2026-10-29. **verified**
  - *"Version 1.0 marks the beginning of a stable base"* (fragment); *"future
    versions of Nim won't break the code you have written with the current
    version."* (fragment). *"Our goal is to make sure that code which compiled
    under Nim 1.0 continues to compile under any stable Nim 1.x version in the
    future."*
  - Its scope: *"Backwards compatibility covers only the stable fragment of the
    language, as defined by the manual."* *"The compiler still implements
    experimental features which are documented in the "experimental manual"."*
    *"These features are subject to changes which may be backwards
    incompatible"* (fragment). Unstable library modules are *"marked with an
    "Unstable API" in their docs."*
  - Its exclusions: *"We of course have to concede that there are
    exceptions."* *"if a security vulnerability is discovered in the standard
    library, we reserve the right to break code which uses it."*
  - Its support: *"The 1.0.x branch will receive bug fixes for as long as
    there is demand for them."*
- **A declared version, by compiler switch**: the Nim 1.0.0 *Nim Compiler User
  Guide*, https://nim-lang.org/1.0.0/nimc.html, read 2026-10-29
  (**verified**), lists *"--useVersion:1.0"*, *"emulate Nim version X of the
  Nim compiler"*. The library side, `std/private/since`,
  https://nim-lang.org/docs/since.html, read 2026-10-29 (**verified**):
  *"since is used to emulate older versions of nim stdlib"*, and *"The
  emulation cannot be 100% faithful and to avoid adding too much complexity,
  since is not needed in those cases:"* (a new module, an added overload, an
  extra parameter). The current user guide, https://nim-lang.org/docs/nimc.html,
  was asked the same question the same day and the fetch reported no such
  switch in the list it read (it named `--legacy` and `--experimental`); whether
  the switch survives in today's compiler is **unverified**.
- **What happened at 2.0**: *Nim v2.0 released*, 01 August 2023,
  https://nim-lang.org/blog/2023/08/01/nim-v20-released.html, read 2026-10-29.
  **verified** It ships *"ORC memory management as a default"* and a porting
  guide whose entries include *"This means that the following code now does not
  compile:"* (identifiers and underscores) and *"Several standard library
  modules have been moved to nimble packages"*. One old behaviour is kept by a
  switch: *"the old behavior is currently still supported with the command
  line option --jsbigint64:off"*. The page, asked, did not say how long 1.6 is
  supported.

### 1.5 TypeScript, for contrast: no semver, and what it calls breaking

- microsoft/TypeScript issue #14116, *Typescript should follow semantic
  versioning*, opened by jbgraug on 16 February 2017, closed 19 April 2017,
  https://github.com/microsoft/TypeScript/issues/14116, read 2026-10-29,
  twice. **verified** mhegazy, 16 February 2017: *"TypeScript never claimed to
  follow semantic versioning, in the sense that breaking changes imply major
  versions."* *"TypeScript, however, promises no breaking changes after a
  stable release."* RyanCavanaugh, 17 February 2017: *"If we followed semver
  rules exactly, literally every single release would be a major version
  bump."* *"Any time we produced the wrong type or emitted the wrong code or
  failed to issue a correct error, that's a breaking change"* (fragment).
  *"The middle digit just isn't useful for TypeScript in a strict semver
  interpretation."* And: *"We're not going to take a major version bump because
  there was a bug in the compiler"*.

### 1.6 What a specification leaves open: a map's order

- **Go 1 release notes, *Iterating in maps***, https://go.dev/doc/go1, read
  2026-10-29. **verified** *"In Go 1, the order in which elements are visited
  when iterating over a map using a for range statement is defined to be
  unpredictable, even if the same loop is run multiple times with the same map.
  Code should not assume that the elements are visited in any particular
  order."* *"This change means that code that depends on iteration order is
  very likely to break early and be fixed long before it becomes a problem."*
  Its *Updating* note: *"Note that it was already incorrect to depend on the
  iteration order, which was unspecified. This change codifies the
  unpredictability."*
- **Python 3.6, *What's New***, https://docs.python.org/3/whatsnew/3.6.html,
  read 2026-10-29. **verified** *"The order-preserving aspect of this new
  implementation is considered an implementation detail and should not be
  relied upon (this may change in the future, but it is desired to have this
  new dict implementation in the language for a few releases before changing
  the language spec to mandate order-preserving semantics for all current and
  future Python implementations; ...)"*.
- **Python 3.7, *What's New***, https://docs.python.org/3/whatsnew/3.7.html,
  read 2026-10-29. **verified** Its release highlights: *"the insertion-order
  preservation nature of dict objects has been declared to be an official part
  of the Python language spec."*

### 1.7 Item 1 answered from the pages above

- **Promises one way, with no declaration**: Go 1 as released on 28 March 2012
  (§1.1: *"not necessarily with Go 1.1 since it may use features added only in
  Go 1.2"*; no declared version on the page). Nim 1.0's announcement promises
  forward compilation (*"continues to compile under any stable Nim 1.x
  version"*); its 1.0.0 compiler's guide lists `--useVersion:1.0`, a
  declaration made by a switch, not by the program's text.
- **Lets a program declare the version it was written for**: Rust, per crate,
  from RFC 2052 on (*"Each crate can declare an edition"*, edition 2015 when
  absent); Go, per module, the `go` line, which *"Before Go 1.21"* was *"an
  advisory requirement"* and which Go 1.21 made *"a mandatory requirement"* and
  the selector of *"the language version the compiler enforces"*. In both cases
  the declaration with semantic force arrived after the 1.0 promise, not with
  it: Rust 1.0 is dated 15 May 2015 and RFC 2052 starts 2017-06-26; Go 1 is
  dated 28 March 2012 and the post naming Go 1.21's change 14 August 2023.
- **Exclusions shared**: compiler bugs (Go *Bugs*, Rust RFC 1122, Rust 2014
  post); security (Go, Nim); specification errors or underspecified semantics
  (Go *Specification errors*, RFC 1122 *"underspecified"*); unstable or
  experimental surface (Rust feature gates, Nim's experimental manual, Go's
  `unsafe`); operating-system interfaces (Go's `syscall`, *"changed by outside
  parties"*).
- **An order the specification leaves open but the implementation gives
  deterministically**: Go 1 made it unpredictable on purpose so that dependent
  code would *"break early"*; Python called it *"an implementation detail"* in
  3.6 and made it *"an official part of the Python language spec"* in 3.7.

## Item 2. Evaluation order

### 2.1 Go: *Order of evaluation* and *Assignment statements*

- The Go specification, https://go.dev/ref/spec, headed *"Language version
  go1.27 (May 26, 2026)"*, read 2026-10-29 in three parts by offset (the page
  is 264,235 characters). **verified**
- *Order of evaluation*, in full as to its rule: *"At package level,
  initialization dependencies determine the evaluation order of individual
  initialization expressions in variable declarations. Otherwise, when
  evaluating the operands of an expression, assignment, or return statement,
  all function calls, method calls, receive operations, and binary logical
  operations are evaluated in lexical left-to-right order."* For `y[f()], ok =
  g(z || h(), i()+x[j()], <-c), k()`: *"the function calls and communication
  happen in the order f(), h() (if z evaluates to false), i(), j(), <-c, g(),
  and k(). However, the order of those events compared to the evaluation and
  indexing of x and the evaluation of y and z is not specified, except as
  required lexically."* Its examples, with their comments:
  `x := []int{a, f()}  // x may be [1, 2] or [2, 2]: evaluation order between a
  and f() is not specified`; `m := map[int]int{a: 1, a: 2}  // m may be {2: 1}
  or {2: 2}: evaluation order between the two map assignments is not
  specified`; `n := map[int]int{a: f()}  // n may be {2: 3} or {3: 3}:
  evaluation order between the key and the value is not specified`.
- *Assignment statements*, the two phases: *"The assignment proceeds in two
  phases. First, the operands of index expressions and pointer indirections
  (including implicit pointer indirections in selectors) on the left and the
  expressions on the right are all evaluated in the usual order. Second, the
  assignments are carried out in left-to-right order."* Its examples include
  `x[1], x[3] = 4, 5  // set x[1] = 4, then panic setting x[3] = 5.` and
  `x[2], p.x = 6, 7  // set x[2] = 6, then panic setting p.x = 7`.
- **Against the critic's F29**: the page agrees; Go orders calls, receives and
  logical operations among themselves and leaves the rest unspecified, with
  the composite-literal and map-literal examples above. **Against F4 and
  F31**: the two-phase sentence and the `x[3]` example are on the page as F31
  quotes them.

### 2.2 Java: JLS §15.7 and §15.26.1

- *The Java Language Specification, Java SE 21 Edition*, chapter 15,
  https://docs.oracle.com/javase/specs/jls/se21/html/jls-15.html, read
  2026-10-29 at offsets 0 and 300,000. **verified**
- §15.7: *"The Java programming language guarantees that the operands of
  operators appear to be evaluated in a specific evaluation order, namely, from
  left to right."* *"It is recommended that code not rely crucially on this
  specification. Code is usually clearer when each expression contains at most
  one side effect, as its outermost operation, and when code does not depend on
  exactly which exception arises as a consequence of the left-to-right
  evaluation of expressions."*
- §15.7.1: *"The left-hand operand of a binary operator appears to be fully
  evaluated before any part of the right-hand operand is evaluated."* *"If
  evaluation of the left-hand operand of a binary operator completes abruptly,
  no part of the right-hand operand appears to have been evaluated."*
- §15.7.2: *"every operand of an operator (except the conditional operators
  &&, ||, and ? :) appears to be fully evaluated before any part of the
  operation itself is performed."* Of `/` and `%`: the `ArithmeticException`
  *"is thrown only after both operands of the binary operator have been
  evaluated and only if these evaluations completed normally."*
- §15.7.4: *"Each argument expression appears to be fully evaluated before any
  part of any argument expression to its right."*
- §15.7.5 names the expressions with their own order because they *"may raise
  exceptional conditions at times that must be specified"*, among them
  *"assignments involving array components (§15.26)"*.
- **§15.26.1, an array component assigned**, every step in order: *"First, the
  array reference subexpression of the left-hand operand array access
  expression is evaluated."* ... *"Otherwise, the index subexpression of the
  left-hand operand array access expression is evaluated."* ... *"Otherwise,
  the right-hand operand is evaluated."* ... *"Otherwise, if the value of the
  array reference subexpression is null, then no assignment occurs and a
  NullPointerException is thrown."* *"Otherwise, the value of the array
  reference subexpression indeed refers to an array. If the value of the index
  subexpression is less than zero, or greater than or equal to the length of
  the array, then no assignment occurs and an ArrayIndexOutOfBoundsException is
  thrown."* Then the component is selected, and for a reference type an
  `ArrayStoreException` check follows.

### 2.3 C#: the standard's order, and the compiler that does not keep it

- *C# language specification*, chapter *Expressions*,
  https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/expressions,
  read 2026-10-29 at offsets 0, 195,000 and 400,000 (the page is 503,116
  characters). **verified**
  - §12.4.1: *"Operands in an expression are evaluated from left to right."*
    *"Example: In F(i) + G(i++) * H(i), method F is called using the old value
    of i, then method G is called with the old value of i, and, finally, method
    H is called with the new value of i. This is separate from and unrelated to
    operator precedence."*
  - §12.6.2.3: *"the expressions or variable references of an argument list are
    evaluated in order, from left to right"*; *"The expressions of an argument
    list are always evaluated in textual order."*
  - §12.8.12.2, *Array access*, its steps: *"P is evaluated."*; each index
    expression *"in order, from left to right"*; *"The value of P is checked to
    be valid. If the value of P is null, a System.NullReferenceException is
    thrown"*; *"The value of each expression in the argument_list is checked
    against the actual bounds of each dimension of the array instance referenced
    by P. If one or more values are out of range, a System.IndexOutOfRangeException
    is thrown"*; then *"The variable reference of the array element given by the
    index expressions is computed, and this becomes the result of the array
    access."*
  - §12.24.2, *Simple assignment*: *"x is evaluated if it was not already."*
    *"If x is classified as a variable, y is evaluated and, if required,
    converted to T"*; then the store.
  - Read together (this seat's reading of the two sections, not a run), the
    standard's text puts the null and bounds checks inside the evaluation of
    `x`, before `y`.
- **dotnet/csharpstandard issue #169**, *12.7.7.2 and 12.18.2 Array access
  evaluation order for assignments*, opened by Jon Skeet on 24 April 2017,
  closed 7 October 2022, labels *meeting: discuss*, *resolved:
  known-deviation*, *status: gnarly*, *type: bug*,
  https://github.com/dotnet/csharpstandard/issues/169, read 2026-10-29.
  **verified** The opening post: *"the nullity check and array index check are
  only performed after y is evaluated."* Mads Torgersen, 4 May 2021: *"I think
  that the spec'ed semantics are the most elegant and orthogonal, and that the
  compiler behavior is essentially a bug."* and *"My inclination is to do
  nothing and live with the discrepancy."* Jon Skeet, 5 October 2022: *"We're
  not going to change the spec,"*. The label's own description: *"We know
  Roslyn deviates from the spec, but we don't want to change the spec."*
- **dotnet/csharpstandard issue #1478**, *Evaluation order of (simple)
  assignments (incl. field assignment, property setter, indexer setter, array
  element assignment, byref property, byref indexers)*, opened by GeeLaw on 23
  November 2025, open when read,
  https://github.com/dotnet/csharpstandard/issues/1478, read 2026-10-29.
  **verified** *"The current spec concerning simple assignment is inconsistent
  with the current Microsoft implementation of C#."* *"In particular, various
  null checks and index range checks happen in inconsistent order w.r.t. other
  side effects."* For a one-dimensional array: *"Implementation evaluates A to
  a, then I to i, then F to f, then checks for null and range and variance,
  then stores f in a[i]."* It cites #169: *"Last time it was discussed, the
  resolution was to put the deviation into documentation, instead of changing
  the spec."* No committee reply stood on the page as read.
- **Raymond Chen, *Why does the assignment operator in C# evaluate left to
  right instead of right to left?*, 18 July 2017**,
  https://devblogs.microsoft.com/oldnewthing/20170718-00/?p=96635, read
  2026-10-29. **verified** *"The C# order of evaluation guarantees that the
  left hand side is evaluated before the right hand side."* *"The C# language
  tries to avoid undefined behavior, so given that it must define a particular
  order of evaluation,"* (fragment). *"However, for some reason, Java has a
  special exception for direct (non-compound) assignment to an array
  element."* *"the range check on the index occurs after the right-hand side is
  evaluated"* (fragment).

### 2.4 C: unsequenced, except as specified

- *ISO/IEC 9899:201x, Committee Draft, April 12, 2011, N1570*,
  https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf, read 2026-10-29
  as page images (document pages 76, 82, 94 and 95). **verified**
- §6.5 ¶2: *"If a side effect on a scalar object is unsequenced relative to
  either a different side effect on the same scalar object or a value
  computation using the value of the same scalar object, the behavior is
  undefined. If there are multiple allowable orderings of the subexpressions of
  an expression, the behavior is undefined if such an unsequenced side effect
  occurs in any of the orderings."* Footnote 84: *"This paragraph renders
  undefined statement expressions such as i = ++i + 1; a[i++] = i; while
  allowing i = i + 1; a[i] = i;"*.
- §6.5 ¶3: *"The grouping of operators and operands is indicated by the syntax.
  Except as specified later, side effects and value computations of
  subexpressions are unsequenced."* Footnote 86: *"In an expression that is
  evaluated more than once during the execution of a program, unsequenced and
  indeterminately sequenced evaluations of its subexpressions need not be
  performed consistently in different evaluations."*
- §6.5.2.2 ¶10: *"There is a sequence point after the evaluations of the
  function designator and the actual arguments but before the actual call."*
  ¶12, EXAMPLE: *"In the function call (*pf[f1()]) (f2(), f3() + f4()) the
  functions f1, f2, f3, and f4 may be called in any order. All side effects
  have to be completed before the function pointed to by pf[f1()] is called."*
- A secondary statement of the same, cppreference, *Order of evaluation* (C),
  https://en.cppreference.com/w/c/language/eval_order, read 2026-10-29
  (**verified**, secondary): *"Order of evaluation of the operands of any C
  operator, including the order of evaluation of function arguments in a
  function-call expression, and the order of evaluation of the subexpressions
  within any expression is unspecified (except where noted below)."*

### 2.5 C++17: what changed and why

- **P0145R3, *Refining Expression Evaluation Order for Idiomatic C++*,
  Gabriel Dos Reis, Herb Sutter, Jonathan Caves, 2016-06-23**,
  https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0145r3.pdf, read
  2026-10-29 as page images, pages 1 to 7. **verified**
  - Abstract: *"This paper proposes an order of evaluation of operands in
    expressions, directly supporting decades-old established and recommended
    C++ idioms. The result is the removal of embarrassing traps for novices and
    experts alike, increased confidence and safety of popular programming
    practices and facilities, hallmarks of modern C++."*
  - The question it opens on: `m[0] = m.size(); // #1` and *"What should the
    map object m look like after evaluation of the statement marked #1?
    {{0, 0}} or {{0, 1}}?"*
  - Why, §2 *A corroding problem*: the chained
    `s.replace(0, 4, "").replace(s.find("even"), 4, "only").replace(s.find("
    don't"), 6, "")`, and *"This code has been reviewed by C++ experts
    world-wide, and published (The C++ Programming Language, 4th edition.) Yet,
    its vulnerability to unspecified order of evaluation has been discovered
    only recently by a tool."*
  - §3: *"The current rules have been in effect for more than three decades. So,
    why change them now?"* ... *"a living and evolving programming language
    cannot just hold onto inertia."*
  - §4, the fix: *"A simple solution would be to require that every expression
    has a well-defined evaluation order. That suggestion has traditionally met
    resistance for various reasons. Rather, this proposal suggests a more
    targeted fix: Postfix expressions are evaluated from left to right. This
    includes functions calls and member selection expressions. Assignment
    expressions are evaluated from right to left. This includes compound
    assignments. Operands to shift operators are evaluated from left to
    right."* And: *"the following expressions are evaluated in the order a,
    then b, then c, then d: a.b, a->b, a->*b, a(b1, b2, b3), b @= a, a[b], a <<
    b, a >> b"*.
  - §7, its measurement: *"We modified the Visual C++ compiler to measure the
    impacts in the worst case scenario."* *"We successfully built, installed,
    and booted the NT kernel. Then we built a large application code base, and
    ran "build, validation, test suites." That uncovered sources of potential
    bugs due to non-portability assumptions: one real-world-code test failed,
    out of 26."* Of the benchmarks: *"The variation is between -4% and +4%."*
  - §8: an alternative for function calls (the function first, the arguments
    *"indeterminately sequenced, but with no interleaving"*); *"At the Spring
    2016 meeting in Jacksonville, FL, EWG decided (via a poll) not to pursue
    this alternative evaluation order."*
- **P0400R0, *Wording for Order of Evaluation of Function Arguments*, William
  M. Miller, 2016-06-25**,
  https://open-std.org/JTC1/SC22/wg21/docs/papers/2016/p0400r0.html, read
  2026-10-29. **verified** *"In the 2016-06-24 plenary session of WG21, paper
  P0145R3 was adopted, except that the alternative wording for 5.2.2
  [expr.call] paragraph 4 describing the indeterminate sequencing of function
  argument evaluation was adopted instead of the wording from the primary
  proposal:"* *"This wording had not been considered by the Core Language
  Working Group during its review of the proposal."* Its wording: *"The
  postfix-expression is sequenced before each expression in the
  expression-list and any default argument."* *"The initialization of a
  parameter, including every associated value computation and side effect, is
  indeterminately sequenced with respect to that of any other parameter."*
- A secondary statement of the result, cppreference, *Order of evaluation*
  (C++), https://en.cppreference.com/w/cpp/language/eval_order, read
  2026-10-29 (**verified**, secondary): *"Order of evaluation of any part of
  any expression, including order of evaluation of function arguments is
  unspecified (with some exceptions listed below)."* Rule 14: *"In a function
  call, value computations and side effects of the initialization of every
  parameter are indeterminately sequenced with respect to value computations
  and side effects of any other parameter."* Rule 19: *"In every simple
  assignment expression E1 = E2 and every compound assignment expression E1 @=
  E2, E2 is sequenced before E1."* The page as read does not tag these rules
  *since C++17* and does not cite P0145; the C++17 attribution rests on the two
  papers above.

### 2.6 Rust and Python: the value before the place

- The Rust Reference, *Expressions*, *Evaluation order of operands*,
  https://doc.rust-lang.org/reference/expressions.html, read 2026-10-29.
  **verified** For a listed set of expressions (among them index, array,
  tuple, struct, call, method call and field expressions): *"The operands of
  these expressions are evaluated prior to applying the effects of the
  expression. Expressions taking multiple operands are evaluated left to right
  as written in the source code."*
- The Rust Reference, *Operator expressions*,
  https://doc.rust-lang.org/reference/expressions/operator-expr.html, read
  2026-10-29. **verified** *"Evaluating assignment expressions begins by
  evaluating its operands. The assigned value operand is evaluated first,
  followed by the assignee expression."* *"Note: This is different than other
  expressions in that the right operand is evaluated before the left one."*
  For compound assignment on primitives: *"the right hand side is evaluated
  first, the left hand side is evaluated next"*; and *"Avoid writing code that
  depends on the evaluation order of operands in compound assignments as it
  can be unusual and surprising."*
- *The Python Language Reference*, §6.16 *Evaluation order*,
  https://docs.python.org/3/reference/expressions.html, read 2026-10-29 at
  offset 100,000. **verified** *"Python evaluates expressions from left to
  right. Notice that while evaluating an assignment, the right-hand side is
  evaluated before the left-hand side."* Its example lines, *"evaluated in the
  arithmetic order of their suffixes"*, end with `expr3, expr4 = expr1,
  expr2`.

### 2.7 Item 2 answered from the pages above

- **Left to right in every position the page lists**: Java (operators, §15.7;
  arguments, §15.7.4), C# (operands, §12.4.1; arguments, §12.6.2.3), Rust (its
  listed expressions), Python (expressions).
- **Some positions only**: Go orders *"function calls, method calls, receive
  operations, and binary logical operations"* and says of the rest *"not
  specified"*; C++17 orders postfix expressions, shifts and (right to left)
  assignments, and leaves arguments *"indeterminately sequenced"*, the paper's
  own left-to-right argument order having been replaced at the plenary
  (P0400R0).
- **None beyond its sequence points**: C (*"unsequenced"*; f1 to f4 *"may be
  called in any order"*).
- **A write in two phases, the place's operands, then the value, then the
  write and its check**: Go in words (*"The assignment proceeds in two
  phases"*); Java's §15.26.1 (array, index, right-hand operand, then the null
  and bounds checks); C#'s compiler, by #169 and #1478, while C#'s standard
  places the checks before the value and its editors kept that text, labelling
  the compiler *"known-deviation"*.
- **The value before the place**: Rust (*"The assigned value operand is
  evaluated first"*), Python (*"the right-hand side is evaluated before the
  left-hand side"*), C++17 (*"Assignment expressions are evaluated from right
  to left"*).

## Item 3. A shift past the sign bit

### 3.1 Java

- JLS SE 21 §15.19 *Shift Operators*,
  https://docs.oracle.com/javase/specs/jls/se21/html/jls-15.html, read
  2026-10-29 at offset 200,000. **verified** *"If the promoted type of the
  left-hand operand is long, then only the six lowest-order bits of the
  right-hand operand are used as the shift distance. It is as if the
  right-hand operand were subjected to a bitwise logical AND operator &
  (§15.22.1) with the mask value 0x3f (0b111111). The shift distance actually
  used is therefore always in the range 0 to 63, inclusive."* *"The value of n
  << s is n left-shifted s bit positions; this is equivalent (even if overflow
  occurs) to multiplication by two to the power s."* *"The value of n >> s is n
  right-shifted s bit positions with sign-extension. The resulting value is
  floor(n / 2^s)."*
- JLS SE 21 §4.2.2 *Integer Operations*,
  https://docs.oracle.com/javase/specs/jls/se21/html/jls-4.html, read
  2026-10-29. **verified** *"The integer operators do not indicate overflow or
  underflow in any way."* The exceptions it lists an integer operator can throw
  are a `NullPointerException` on unboxing a null, an `ArithmeticException` for
  `/` and `%` by zero, and an `OutOfMemoryError` for `++` and `--` boxing; a
  shift is not among them.

### 3.2 Go

- The Go specification, https://go.dev/ref/spec (go1.27), read 2026-10-29 at
  offset 100,000. **verified** *"The right operand in a shift expression must
  have integer type [Go 1.13] or be an untyped constant representable by a
  value of type uint."* *"The shift operators shift the left operand by the
  shift count specified by the right operand, which must be non-negative. If
  the shift count is negative at run time, a run-time panic occurs. The shift
  operators implement arithmetic shifts if the left operand is a signed integer
  and logical shifts if it is an unsigned integer. There is no upper limit on
  the shift count. Shifts behave as if the left operand is shifted n times by 1
  for a shift count of n. As a result, x << 1 is the same as x*2 and x >> 1 is
  the same as x/2 but truncated towards negative infinity."*
- *Integer overflow*: *"For signed integers, the operations +, -, *, /, and <<
  may legally overflow and the resulting value exists and is deterministically
  defined by the signed integer representation, the operation, and its
  operands. Overflow does not cause a run-time panic. A compiler may not
  optimize code under the assumption that overflow does not occur."*

### 3.3 Rust, debug and release

- The Rust Reference, *Operator expressions*,
  https://doc.rust-lang.org/reference/expressions/operator-expr.html, read
  2026-10-29. **verified** *"Integer operators will panic when they overflow
  when compiled in debug mode. The -C debug-assertions and -C overflow-checks
  compiler flags can be used to control this more directly. The following
  things are considered to be overflow:"*; of shifts, the one entry: *"Using <<
  or >> where the right-hand argument is greater than or equal to the number of
  bits in the type of the left-hand argument, or is negative."* The shift
  footnote: *"Arithmetic right shift on signed integer types, logical right
  shift on unsigned integer types."*
- The Rust Reference, *Behavior not considered unsafe*, *Integer overflow*,
  https://doc.rust-lang.org/reference/behavior-not-considered-unsafe.html,
  read 2026-10-29. **verified** *"If a program contains arithmetic overflow,
  the programmer has made an error."* *"When the programmer has enabled
  debug_assert! assertions (for example, by enabling a non-optimized build),
  implementations must insert dynamic checks that panic on overflow. Other
  kinds of builds may result in panics or silently wrapped values on overflow,
  at the implementation's discretion."* *"In the case of implicitly-wrapped
  overflow, implementations must provide well-defined (even if still
  considered erroneous) results by using two's complement overflow
  conventions."* The section names no shift.
- RFC 560, *Integer overflow*, start date 2014-06-30,
  https://rust-lang.github.io/rfcs/0560-integer-overflow.html, read
  2026-10-29. **verified** *"Shift operations (<<, >>) on a value of width N
  can be passed a shift value >= N."* *"It is unclear what behaviour should
  result from this, so the shift value is unconditionally masked"* (fragment).
  Its updates: *"Shifts were specified to mask off the bits of over-long
  shifts."* Its rationale: *"However, wraparound arithmetic is almost never the
  intended behavior."*
- The standard library's `i64`, https://doc.rust-lang.org/std/primitive.i64.html,
  read 2026-10-29 (its first 100,000 characters). **verified**
  - `checked_shl`: *"Checked shift left. Computes self << rhs, returning None
    if rhs is larger than or equal to the number of bits in self."*, with the
    page's example `0x10i64.checked_shl(63) == Some(0)`.
  - `strict_shl`: *"panicking if rhs is larger than or equal to the number of
    bits in self"*.
  - `shl_exact`, marked nightly-only, feature `exact_bitshifts`, tracking
    issue #144336: *"Exact shift left. Computes self << rhs as long as it can
    be reversed losslessly."* *"Returns None if any bits that would be shifted
    out differ from the resulting sign bit or if rhs >= i64::BITS."*, with the
    page's example `0x1i64.shl_exact(i64::BITS - 1) == None`.

### 3.4 Swift

- *The Swift Programming Language*, *Advanced Operators*, read 2026-10-29 from
  the book's source,
  https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/AdvancedOperators.md
  (the rendered page,
  https://docs.swift.org/swift-book/documentation/the-swift-programming-language/advancedoperators/,
  returned no text to the fetch). **verified** *"Unlike arithmetic operators in
  C, arithmetic operators in Swift don't overflow by default. Overflow behavior
  is trapped and reported as an error."* Of shifts: *"Any bits that are moved
  beyond the bounds of the integer's storage are discarded."* And: *"When you
  shift signed integers to the right, apply the same rules as for unsigned
  integers, but fill any empty bits on the left with the sign bit, rather than
  with a zero."* *"This action ensures that signed integers have the same sign
  after they're shifted to the right, and is known as an arithmetic shift."*
- The standard library's source, `Integers.swift`,
  https://raw.githubusercontent.com/swiftlang/swift/main/stdlib/public/core/Integers.swift,
  read 2026-10-29. **verified** *"The << operator performs a smart shift, which
  defines a result for a shift of any value."* *"Using a negative value for rhs
  performs a right shift using abs(rhs)."* *"Using a value for rhs that is
  greater than or equal to the bit width of lhs is an overshift, resulting in
  zero."* And of `&<<`: *"Before shifting, the masking left shift operator
  masks the shift to this range."*

### 3.5 C#

- *C# language specification*, §12.14 *Shift operators*,
  https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/expressions,
  read 2026-10-29 at offset 295,000. **verified** *"When the type of x is long
  or ulong, the shift count is given by the low-order six bits of count. In
  other words, the shift count is computed from count & 0x3F."* *"The
  high-order bits outside the range of the result type of x are discarded, the
  remaining bits are shifted left, and the low-order empty bit positions are
  set to zero."* For `>>` on `int`, `nint` or `long`: *"the high-order empty
  bit positions are set to zero if x is non-negative and set to one if x is
  negative."* *"Shift operations never cause overflows and produce the same
  results in checked and unchecked contexts."*

### 3.6 Beside the brief's five: C, C++ and Zig

- **C**, N1570 §6.5.7, read as page images (document pages 94 and 95).
  **verified** ¶3: *"If the value of the right operand is negative or is
  greater than or equal to the width of the promoted left operand, the behavior
  is undefined."* ¶4: *"If E1 has a signed type and nonnegative value, and E1 ×
  2^E2 is representable in the result type, then that is the resulting value;
  otherwise, the behavior is undefined."* ¶5: *"If E1 has a signed type and a
  negative value, the resulting value is implementation-defined."*
- **C++, CWG issue 1457**, *Undefined behavior in left-shift*, submitted by
  Howard Hinnant, 2012-02-04, status CD3,
  https://cplusplus.github.io/CWG/issues/1457.html, read 2026-10-29.
  **verified** The wording then *"makes it undefined behavior to create the
  most-negative integer of a given type"* by shifting a signed 1 into the sign
  bit, which *"is not uncommonly done and works correctly on the majority of
  (twos-complement) architectures."* Its resolution: *"if E1 ⨯ 2E2 is
  representable in the corresponding unsigned type of the result type, then
  that value, converted to the result type, is the resulting value"*.
- **C++20, P0907R4, *Signed Integers are Two's Complement*, JF Bastien,
  2018-10-06**, https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0907r4.html,
  read 2026-10-29, twice. **verified** Abstract: *"There is One True
  Representation for signed integers, and that representation is two's
  complement."* Its changes: *"Left-shift on signed integer types produces the
  same results as left-shift on the corresponding unsigned integer type."*
  *"Right-shift is an arithmetic right shift which performs sign-extension."*
  Its SG6/SG12 polls at Jacksonville: *"Left shift should be the same for
  signed and unsigned (overrides previous poll)."* 6/5/5/0/1; *"Right shift on
  a signed integral type should be an arithmetic shift (which sign-extends)."*
  9/4/3/1/0. On overflow of `+`, `-`, `*`: *"The author had hoped to make this
  well-defined as wrapping."*, and the poll (several answers allowed) read
  *Wrap* 4, *Wrap or trap* 6, *Intermediate values are mathematical integers*
  5, *Status quo* (undefined) 14.
- cppreference, *Arithmetic operators*, bitwise shift,
  https://en.cppreference.com/w/cpp/language/operator_arithmetic, read
  2026-10-29 (**verified**, secondary). Until C++20, of a signed non-negative
  `a`: *"(this makes it legal to create INT_MIN as 1 << 31)"*; since C++20:
  *"The value of a << b is the unique value congruent to a * 2^b modulo 2^N
  where N is the number of bits in the return type (that is, bitwise left shift
  is performed and the bits that get shifted out of the destination type are
  discarded)."* *"The value of a >> b is a/2^b, rounded towards negative
  infinity (in other words, right shift on signed a is arithmetic right
  shift)."* In every version: *"If the value of rhs is negative or is not less
  than the number of bits in lhs, the behavior is undefined."*
- **Zig**, *Zig Language Reference* (master),
  https://ziglang.org/documentation/master/, read 2026-10-29 at offsets 0,
  300,000 and 400,000. **verified**
  - The operator table, `a << b`: *"Moves all bits to the left, inserting new
    zeroes at the least-significant bit."*
  - `@shlExact`: *"Performs the left shift operation (<<)."* For unsigned
    integers the result is *"undefined if any 1 bits are shifted out."*; for
    signed integers *"undefined if any bits that disagree with the resultant
    sign bit are shifted out."*
  - *Exact Left Shift Overflow*: its compile-time example gives *"error:
    overflow of integer type 'u8' with value '340'"*; its run-time example,
    built with `zig build-exe` and no mode flag, prints *"panic: left shift
    overflowed bits"* and *"(process terminated by signal)"*.
  - In its *Errors* section, of another operation: *"invokes safety-checked
    Illegal Behavior, so in debug and safe, triggers a safety panic by
    default."*

### 3.7 Item 3 answered from the pages above

- **None of the brief's five traps on a shift that loses a set bit or changes
  the sign**: Java (*"do not indicate overflow or underflow in any way"*), Go
  (*"Overflow does not cause a run-time panic"*), Rust in debug and release
  (its overflow list names, for a shift, only the count; the page's own
  example `0x10i64.checked_shl(63) == Some(0)` loses a set bit and is not
  refused), Swift (*"defines a result for a shift of any value"*), C# (*"never
  cause overflows ... in checked and unchecked contexts"*).
- **What does trap**: Go, a negative count at run time; Rust in debug, a count
  not below the width or negative; Zig's separate `@shlExact`, bits shifted
  out that *"disagree with the resultant sign bit"* (its documented run-time
  example panics). Rust's nightly `shl_exact` refuses the same condition by
  returning `None`, and its page gives `0x1i64.shl_exact(i64::BITS - 1) ==
  None`, a shift into the sign bit. Neither page describes a trap on a set bit
  lost while the sign is kept, as distinct from the sign condition, for signed
  integers; Zig's *"undefined if any 1 bits are shifted out"* is its rule for
  unsigned integers.
- **`>>` on a negative**: sign-copying in Java (*"with sign-extension"*), Go
  (*"arithmetic shifts if the left operand is a signed integer"*), Rust
  (*"Arithmetic right shift on signed integer types"*), Swift (*"known as an
  arithmetic shift"*), C# (*"set to one if x is negative"*) and C++20
  (*"arithmetic right shift"*); C leaves it *"implementation-defined"*.
- **A shift into the sign bit given a value on purpose**: C++'s CWG 1457 made
  it defined because it *"is not uncommonly done"*, and C++20 (P0907R4) made
  every signed left shift the unsigned one's result.

## Item 4. Diagnostic codes as a promise

### 4.1 rustc's `E0xxx`

- *Rust Compiler Development Guide*, *Error codes and explanations*,
  https://rustc-dev-guide.rust-lang.org/diagnostics/error-codes.html, read
  2026-10-29. **verified** *"We generally try to assign each error message a
  unique code like E0123."* *"Error codes are stored in
  compiler/rustc_error_codes."* *"All error codes have an associated
  explanation: new error codes must include them."* *"Note that not all
  historical (no longer emitted) error codes have explanations."* *"To create a
  new error, you first need to find the next available code."* Asked, the page
  states nothing on whether a code is removed, reused or renumbered, and
  nothing on the stability of codes or of wording.
- The error index, *Error code E0001*,
  https://doc.rust-lang.org/error_codes/E0001.html, read 2026-10-29.
  **verified** *"Note: this error code is no longer emitted by the
  compiler."*, with its explanation kept below.
- *The rustc book*, *JSON Output*, https://doc.rust-lang.org/rustc/json.html,
  read 2026-10-29. **verified** *"When parsing, care should be taken to be
  forwards-compatible with future changes to the format."* *"New fields may be
  added."* *"Enumerated fields like "level" or "suggestion_applicability" may
  add new values."* Of the code: *"The diagnostic code. Some messages may set
  this value to null."* and *"A unique string identifying which diagnostic
  triggered."* No sentence on the stability of `message` or `rendered`.
- *The rustc book*, *Warn-by-default Lints*, `renamed_and_removed_lints`,
  https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html, read
  2026-10-29 at offset 100,000. **verified** *"The renamed_and_removed_lints
  lint detects lints that have been renamed or removed."* Its example produces
  *"warning: lint `raw_pointer_derive` has been removed: using derive with raw
  pointers is ok"*. *"To fix this, either remove the lint or use the new
  name."*

### 4.2 Go's type checker

- `src/internal/types/errors/codes.go`,
  https://raw.githubusercontent.com/golang/go/master/src/internal/types/errors/codes.go,
  read 2026-10-29. **verified** Its header: *"Copyright 2020 The Go
  Authors."* *"This file defines the error codes that can be produced during
  type-checking."* *"Collectively, these codes provide an identifier that may be
  used to implement special handling for certain types of errors."* *"Error
  code values should not be changed: add new codes at the end."* *"Error codes
  should be fine-grained enough that the exact nature of the error can be
  easily determined, but coarse enough that they are not an implementation
  detail of the type checking algorithm."*

### 4.3 C#'s `CSxxxx`

- *Compiler warning waves*,
  https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/compiler-messages/warning-waves,
  read 2026-10-29. **verified** *"New warnings and errors can be introduced in
  each release of the C# compiler. When new warnings could be reported on
  existing code, those warnings are introduced under an opt-in system referred
  to as a warning wave. The opt-in system means that you shouldn't see new
  warnings on existing code without taking action to enable them. When
  <TreatWarningsAsErrors>true</TreatWarningsAsErrors> is specified, enabled
  warning wave warnings generate errors."* *"Warning wave 5 diagnostics were
  added in C# 9."* ... *"Warning wave 10 diagnostics were added in C# 14."*
  And: *"AnalysisLevel tracks the current TFM if not specified"*.
- Roslyn, `ErrorCode.cs`,
  https://raw.githubusercontent.com/dotnet/roslyn/main/src/Compilers/CSharp/Portable/Errors/ErrorCode.cs,
  read 2026-10-29 (its first 100,000 of 115,265 characters). **verified** No
  comment states a rule for adding, reusing or renumbering a code. Retired
  codes stand as comments: *"// WRN_OldWarning_UnsafeProp = 207,    // This
  error code is unused."*, *"//ERR_SameFullNameThisAggThisAgg = 468, no longer
  used in Roslyn"*; ranges are marked *"// unused 3029-3999"* and *"//
  available 8037-8039"*; and one note reuses a never-used number: *"CS0166 was
  originally reserved for ERR_SwitchFallInto in Dev10, but was never used."*

### 4.4 TypeScript

- TypeScript's `CONTRIBUTING.md`,
  https://raw.githubusercontent.com/microsoft/TypeScript/main/CONTRIBUTING.md,
  read 2026-10-29. **verified** Asked, it states nothing on how a diagnostic's
  code is chosen, reused or changed.
- Issue #14116 (§1.5) states the project's versioning and what it calls
  breaking, and nothing on codes.

### 4.5 Swift: SE-0443

- *SE-0443, Precise Control Flags over Compiler Warnings*, Doug Gregor and
  Dmitrii Galimzianov, status *"Implemented (Swift 6.1)"*,
  https://raw.githubusercontent.com/swiftlang/swift-evolution/main/proposals/0443-warning-control-flags.md,
  read 2026-10-29. **verified** *"A diagnostic group is a stable identifier for
  an error or warning."* *"This is necessary because diagnostics within the
  compiler may change, but we need to provide stable user-facing identifiers
  for them."* *"Diagnostic groups may expand over time, but they can never
  become narrower."* *"The internal structure of the graph may change to some
  extent. However, the set of diagnostics included in a diagnostic group
  (directly or transitively) should not shrink."* Asked, it does not say
  whether a group's name may be changed or removed.

### 4.6 SARIF 2.1.0

- *Static Analysis Results Format (SARIF) Version 2.1.0 JSON Schema*,
  https://docs.oasis-open.org/sarif/sarif/v2.1.0/errata01/os/schemas/sarif-schema-2.1.0.json,
  read 2026-10-29. **verified** `reportingDescriptor.id`: *"A stable, opaque
  identifier for the report."* `deprecatedIds`: *"An array of stable, opaque
  identifiers by which this report was known in some previous version of the
  analysis tool."* `result.ruleId`: *"The stable, unique identifier of the
  rule, if any, to which this result is relevant."* `version`: *"The SARIF
  format version of this log file."* The specification's normative prose was
  not read; only the schema's descriptions.

### 4.7 Item 4 answered from the pages above

- **A diagnostic identifier promised stable, in words**: Swift's SE-0443
  (*"a stable identifier for an error or warning"*, *"can never become
  narrower"*), which separates it from the diagnostics themselves
  (*"diagnostics within the compiler may change"*); SARIF's schema (*"A
  stable, opaque identifier"*, with `deprecatedIds` for the old ones); Go's
  type checker, as a rule for its maintainers in a file under `src/internal/`
  (*"Error code values should not be changed: add new codes at the end."*).
- **Retired codes kept, with no written promise read**: rustc (E0001,
  *"no longer emitted by the compiler"*) and Roslyn (*"This error code is
  unused."*); rustc warns on a renamed or removed lint name rather than
  refusing it.
- **No promise found**: TypeScript and C# (searches below).
- **Words**: none of the pages read promises a message's words; rustc's JSON
  page asks a parser to expect *"New fields may be added."*
- **A diagnostic added on existing code**: C# makes new warnings on existing
  code opt-in (*"warning waves"*).

## Item 5. No exceptions, so a stop and a restart

### 5.1 Erlang: *let it crash* and supervisors

- **Joe Armstrong, *Making reliable distributed systems in the presence of
  software errors***, a dissertation submitted to the Royal Institute of
  Technology, Stockholm, December 2003 (its title page: *"Final version (with
  corrections)"* and *"last update 20 November 2003"*),
  https://erlang.org/download/armstrong_thesis_2003.pdf, read 2026-10-29 as
  page images (title page, contents, pages 105 to 110). **verified**
  - §4.3.1, page 105: *"Thus under all circumstances, including hardware
    failure it is Pid2's job to correct the error. This is why I say "let some
    other process fix the error.""*
  - Page 106: *"This philosophy is completely different to that used in a
    sequential programming language where there is no alternative but to try
    and handle all errors in the thread of control where the error occurs."*
  - §4.3.2: *"One process, the worker process, does the job. Another process,
    the supervisor process. observes the worker. If an error occurs in the
    worker, the supervisor takes actions to correct the error."*
  - §4.4 *Let it crash*, page 107: *"The philosophy is let some other process
    fix the error, but what does this mean for their code? The answer is let it
    crash. By this I mean that in the event of an error, then the program
    should just crash."*
  - Page 108: *"All you can do is terminate the program."* Page 109: *"The
    defensive code detracts from the pure case and confuses the reader"*, and
    *"the diagnostic is often no better than the diagnostic which the compiler
    supplies automatically."*
- **Erlang/OTP, *Supervisor Behaviour***,
  https://www.erlang.org/doc/system/sup_princ.html, read 2026-10-29.
  **verified** *"A supervisor is responsible for starting, stopping, and
  monitoring its child processes."* *"The basic idea of a supervisor is that it
  is to keep its child processes alive by restarting them when necessary."* Its
  limit: *"If more than MaxR number of restarts occur in the last MaxT
  seconds"*, the supervisor *"terminates all the child processes and then
  itself"*. *"A permanent child process is always restarted."* *"A temporary
  child process is never restarted"*. *"A transient child process is restarted
  only if it terminates abnormally"*, *"with an exit reason other than normal,
  shutdown, or {shutdown,Term}"*.

### 5.2 Go: `panic` across goroutines, and the server that catches it

- The Go specification, *Handling panics* and *Run-time panics*,
  https://go.dev/ref/spec, read 2026-10-29 at offset 200,000. **verified**
  *"While executing a function F, an explicit call to panic or a run-time
  panic terminates the execution of F. Any functions deferred by F are then
  executed as usual. Next, any deferred functions run by F's caller are run,
  and so on up to any deferred by the top-level function in the executing
  goroutine. At that point, the program is terminated and the error condition
  is reported, including the value of the argument to panic. This termination
  sequence is called panicking."* And: *"Execution errors such as attempting to
  index an array out of bounds trigger a run-time panic equivalent to a call of
  the built-in function panic with a value of the implementation-defined
  interface type runtime.Error."*
- *Effective Go*, *Panic* and *Recover*, https://go.dev/doc/effective_go, read
  2026-10-29. **verified** *"If that unwinding reaches the top of the
  goroutine's stack, the program dies."* *"One application of recover is to
  shut down a failing goroutine inside a server without killing the other
  executing goroutines."* And: *"real library functions should avoid panic. If
  the problem can be masked or worked around, it's always better to let things
  continue to run rather than taking down the whole program."*
- `net/http`, `Handler`, https://pkg.go.dev/net/http, read 2026-10-29 at
  offset 100,000. **verified** *"If ServeHTTP panics, the server (the caller of
  ServeHTTP) assumes that the effect of the panic was isolated to the active
  request."* *"It recovers the panic, logs a stack trace to the server error
  log, and either closes the network connection or sends an HTTP/2
  RST_STREAM, depending on the HTTP protocol."* *"To abort a handler so the
  client sees an interrupted response but the server doesn't log an error,
  panic with the value ErrAbortHandler."*
- **What was said of it later**: grpc/grpc-go issue #441, *panic in rpc causes
  the whole server crash*, opened 10 November 2015, the comment by `dsymonds`
  of the same day, https://github.com/grpc/grpc-go/issues/441, read
  2026-10-29. **verified** *"On the other hand, quietly catching a panic might
  leave the server in an inconsistent or incorrect state, so while crashing
  sucks at least you'll find out and can go fix the panic."* *"We've come to
  regret the net/http panic catching after running Go in production for a
  while."* *"It sounded like a good idea at the time, but it has caused its own
  issues in practice."* Found through Nick Snyder, *When is it ok to recover
  from panics in Go?*, Sourcegraph, 6 March 2018,
  https://sourcegraph.com/blog/go-when-is-it-ok-to-recover, read 2026-10-29
  (**verified**), which names the commenter David Symonds. The same regret
  stated by Russ Cox or Brad Fitzpatrick: **unverified**, not found (search
  below); golang/go issue #25448, which that search offered, is *"spec:
  guarantee non-nil return value from recover"* and says nothing of the kind
  (read 2026-10-29).

### 5.3 Rust: `panic = "abort"`

- *The Cargo Book*, *Profiles*, `panic`,
  https://doc.rust-lang.org/cargo/reference/profiles.html, read 2026-10-29.
  **verified** *"The valid options are: "unwind": Unwind the stack upon panic.
  "abort": Terminate the process upon panic."* Both `dev` and `release`
  default to `panic = 'unwind'`. *"Tests, benchmarks, build scripts, and proc
  macros ignore the panic setting."*
- *The Rust Programming Language*, §9.1,
  https://doc.rust-lang.org/book/ch09-01-unrecoverable-errors-with-panic.html,
  read 2026-10-29. **verified** *"By default, these panics will print a failure
  message, unwind, clean up the stack, and quit."* *"Rust therefore allows you
  to choose the alternative of immediately aborting, which ends the program
  without cleaning up."* *"If in your project you need to make the resultant
  binary as small as possible, you can switch from unwinding to aborting upon a
  panic by adding panic = 'abort' to the appropriate [profile] sections in your
  Cargo.toml file."*
- `std::panic::catch_unwind`,
  https://doc.rust-lang.org/std/panic/fn.catch_unwind.html, read 2026-10-29.
  **verified** *"It is not recommended to use this function for a general
  try/catch mechanism."* *"This function might not catch all Rust panics. A
  Rust panic is not always implemented via unwinding, but can be implemented by
  aborting the process as well. This function only catches unwinding panics,
  not those that abort the process."*

### 5.4 Swift's rationale, both ways

- Swift, `docs/ErrorHandlingRationale.md`,
  https://raw.githubusercontent.com/swiftlang/swift/main/docs/ErrorHandlingRationale.md,
  read 2026-10-29. **verified** *"The final category is logic failures,
  including out of bounds array accesses, forced unwrap of nil optionals, and
  other kinds of assertions."* *"The programmer has made a mistake, and the
  failure should be handled by fixing the code, not by attempting to recover
  dynamically."* *"The correct handling of these error conditions is an open
  question and is not a focus of this proposal."* *"High-reliability systems
  may need some way to limp on even after an assertion failure."* *"Tearing
  down the process can be viewed as a vector for a denial-of-service attack."*
  *"However, an assertion failure might indicate that the process has been
  corrupted and is under attack,"* ... *"limping on anyway may open the system
  up for other, more serious forms of security breach."*

### 5.5 The thing that starts it again, outside the language: systemd

- *systemd.service*,
  https://www.freedesktop.org/software/systemd/man/latest/systemd.service.html,
  read 2026-10-29. **verified** `Restart=`: *"Configures whether the service
  shall be restarted when the service process exits, is killed, or a timeout is
  reached."* *"Takes one of no, on-success, on-failure, on-abnormal,
  on-watchdog, on-abort, or always."* *"If set to no (the default), the service
  will not be restarted."* *"If set to on-failure, the service will be
  restarted when the process exits with a non-zero exit code, is terminated by
  a signal"* (fragment). *"If set to on-abort, the service will be restarted
  only if the service process exits due to an uncaught signal"* (fragment).
  *"Setting this to on-failure is the recommended choice for long-running
  services, in order to increase reliability by attempting automatic recovery
  from errors."*

### 5.6 Item 5 answered from the pages above

- **Erlang**: the failing process crashes (*"the program should just crash"*)
  and another restarts it inside the runtime; the supervisor gives up past
  MaxR restarts in MaxT seconds and *"terminates all the child processes and
  then itself"*.
- **Go**: an unrecovered panic in any goroutine terminates the program (*"the
  program dies"*); its own HTTP server recovers per request, of which
  `dsymonds` wrote *"We've come to regret the net/http panic catching"*.
- **Rust**: `panic = "abort"` is *"Terminate the process upon panic"*;
  `catch_unwind` is *"not recommended ... for a general try/catch
  mechanism"* and catches no aborting panic.
- **Swift**: its rationale leaves logic failures' handling *"an open
  question"* and records both the *"limp on"* and the *"corrupted and is under
  attack"* arguments.
- **Outside the language**: systemd's `on-failure`, *"the recommended choice
  for long-running services"*, restarts on *"a non-zero exit code"* or on
  termination *"by a signal"*; its default is no restart.

## The seat's reading (advisory, no veto)

- **verdict**: approve (advisory).
- **argument** (116 words, counted by hand): The paragraph's three exclusions
  are Go 1's own headings (*Bugs*, *Unspecified behavior*, *Specification
  errors*), and Go restated that promise in 2023 (*"There will not be a Go 2
  that breaks Go 1 programs"*). Two departures should be chosen, not
  inherited. *It runs one way* as a `1.x` rule: Go began undeclared, then made
  the `go` line *"a mandatory requirement"* (1.21) and gated a meaning change
  on it; Rust added editions after 1.0; Nim 1.0's guide lists `--useVersion`.
  Promising `hero_os.h`: Go put `syscall` outside, *"changed by outside
  parties"*. A two-phase write has Go's and Java's words; C#'s standard says
  otherwise and lives with a *"known-deviation"*. Wrapping `<<` matches all
  five languages asked; none traps on lost bits.
- **condition**: the reading changes if any of these is found: a language that
  promised one way for a whole major version and kept it without later adding
  a declared version, an edition, a version switch or a per-version default
  (none among Go, Rust and Nim as read); a language that promised its
  operating-system functions across OS changes and kept it; a compiler that
  traps on a lost set bit while giving a shift into the sign bit a value (the
  two exact-shift precedents read, Rust's `shl_exact` and Zig's `@shlExact`,
  refuse both); a later C# decision bringing Roslyn to the standard's order or
  the standard to Roslyn's; or a language promising its diagnostics' words.

## Search: what was looked for, and where

**Web searches run (WebSearch, standard mode)**, each with what it returned:

1. `nim compiler "--useVersion" emulate Nim version`: pointed to the Nim
   1.0.0 user guide and the `since` module, both then read.
2. `C# array element assignment IndexOutOfRangeException thrown after
   right-hand side evaluated specification Roslyn stelem order`: Raymond
   Chen's post and csharpstandard #1478, both read; #1478 led to #169, read.
3. `P0400R0 "Wording for Order of Evaluation of Function Arguments" Oulu
   indeterminately sequenced`: P0400R0, read. The meeting's place was not
   confirmed and is stated nowhere above.
4. `TypeScript diagnostic error code numbers stable "breaking change"
   maintainers TS2345 codes not guaranteed`: nothing from the TypeScript
   project.
5. `TypeScript does not follow semantic versioning breaking changes every
   minor release github issue 14116`: secondary pages; #14116 then read
   directly.
6. `site:github.com microsoft TypeScript wiki "error codes" OR "diagnostic
   codes" stable reuse diagnosticMessages.json`: no policy page.
7. `SARIF 2.1.0 reportingDescriptor "id" "stable, opaque identifier" ruleId`
   and 8. `OASIS SARIF v2.1.0 specification reportingDescriptor id property
   "SHALL" stable identifier rule "3.49.3"`: vendor pages only; the OASIS
   schema was then read directly.
9. `Joe Armstrong thesis 2003 "Making reliable distributed systems in the
   presence of software errors" "let it crash"`: the thesis's mirrors; the
   erlang.org copy then read.
10. `golang net/http recover panics ServeHTTP "mistake" issue Brad Fitzpatrick
    Russ Cox in retrospect`: the Sourcegraph post (read) and golang/go #25448
    (read, unrelated); no statement by Cox or Fitzpatrick.
11. `Roslyn C# compiler diagnostic IDs "CS" error codes never reused or
    renumbered stable policy`: no Microsoft policy; Roslyn's `ErrorCode.cs`
    then read.

**Sources swept by item** (every one fetched and read as cited above):

- Item 1: go.dev (go1compat, blog/go1, blog/compat, doc/godebug,
  doc/toolchain, blog/loopvar-preview, doc/go1.22, doc/go1, ref/spec's
  appendix); blog.rust-lang.org (2014 *Stability*, 2015 *Rust 1.0*);
  rust-lang.github.io RFCs 1122 and 2052; the Edition Guide; nim-lang.org
  (1.0 and 2.0 announcements, 1.0.0 and current user guides, `since`);
  microsoft/TypeScript #14116; docs.python.org What's New 3.6 and 3.7.
- Item 2: the Go specification; JLS SE 21 ch. 15; the C# specification's
  *Expressions*; csharpstandard #169 and #1478; *The Old New Thing*; N1570;
  P0145R3; P0400R0; cppreference (C and C++ *Order of evaluation*); the Rust
  Reference; the Python Language Reference.
- Item 3: JLS SE 21 §15.19 and §4.2.2; the Go specification; the Rust
  Reference (two pages), RFC 560 and `i64`'s documentation; the Swift book's
  source and `Integers.swift`; the C# specification §12.14; N1570 §6.5.7; CWG
  1457; P0907R4; cppreference *Arithmetic operators*; the Zig Language
  Reference.
- Item 4: rustc-dev-guide; the error index (E0001); the rustc book (JSON,
  lints); Go's `codes.go`; Microsoft Learn *warning waves*; Roslyn's
  `ErrorCode.cs`; TypeScript's `CONTRIBUTING.md` and #14116; SE-0443; the
  SARIF 2.1.0 schema.
- Item 5: Armstrong's thesis; Erlang/OTP *Supervisor Behaviour*; the Go
  specification; Effective Go; `net/http`; grpc-go #441; the Sourcegraph post;
  golang/go #25448; the Cargo Book; the Rust Book §9.1; `catch_unwind`;
  Swift's `ErrorHandlingRationale.md`; `systemd.service`.

**Looked for and not found**, each a statement about this search and not about
the world:

- A written promise that TypeScript's numbered diagnostics are stable:
  searches 4 and 6, TypeScript's `CONTRIBUTING.md`, issue #14116.
- A written Microsoft policy that C#'s `CS` codes are never reused or
  renumbered: search 11 and Roslyn's `ErrorCode.cs` (which states no rule,
  and records one reuse of a never-used number).
- A statement on rustc's error codes' stability: the dev guide's page, asked
  directly.
- Whether today's Nim compiler still has `--useVersion`: the current user
  guide, asked directly, showed none in the list it read; **unverified**.
- The net/http regret stated by Russ Cox or Brad Fitzpatrick: search 10;
  **unverified**.
- SARIF's normative prose on identifiers: searches 7 and 8 did not reach it;
  only the schema was read.
- A language promising one way for a whole major version without ever adding
  a declaration: not searched as such; the three languages the brief named
  were read, and each added one (Go, Rust) or documented one at 1.0 (Nim).
