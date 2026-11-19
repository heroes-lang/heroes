# Panel 253, historian (advisory)

Written as it goes, 2026-11-19, by the historian seat. **This seat has no shell**: its tools are Read,
Write, WebSearch and WebFetch alone, so it ran no `git`, `ls`, `grep`, `date` or build, and every
repository fact below is a file it opened with Read, by path and line. Every page marked `verified` was
fetched today with WebFetch and its words are quoted beside the claim; anything not read today is marked
`unverified`. All six blocks are finished.

**One file landed outside the repository's root, written by WebFetch and not by this seat**: fetching
https://www.cl.cam.ac.uk/~rja14/Papers/whatyouc.pdf saved its binary to
`/Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/9d1670cb-db5d-4975-9d5b-1db104217e45/tool-results/webfetch-1795104584839-vk3usk.pdf`
(305.6 KB). The coordinator may remove it. Nothing else was written but this report.

Read first, whole and in order: `docs/panel/253-briefs/00-shared.md` (821 lines, its § Repairs after the
critic's first reading included), `docs/panel/253-briefs/historian.md` (144 lines), this seat's definition
`.claude/agents/historian.md`; then for their claims about other languages the spec-warden's,
compiler-engineer's and ffi-pragmatist's reports (the last read twice, its Q3 landing between the readings)
and the critic's first reading `critic-briefs.md`. Repository files were read in the `rebel-round`
worktree, not in a `git archive` of `181555e9a` (no shell); the critic found `181555e9a..f4e3594d5` moves
only panel 251's two evidence files, so `docs/design.md` there is the frozen one unless an uncommitted
edit stands, which this seat cannot ask.

## Premises of the briefs found false

1. **`historian.md` Q5, *"Go: what a panic prints (every goroutine's stack, by default)"***: false. Go's
   runtime page (https://pkg.go.dev/runtime): *"By default, a failure prints a stack trace for the current
   goroutine, eliding functions internal to the run-time system, and then exits with exit code 2"*;
   *"GOTRACEBACK=single (the default)"*; all goroutines only *"if there is no current goroutine"* or with
   `GOTRACEBACK=all`.
2. **`historian.md` Q2, *"Go's stated refusal of `@param`-style tags (its *Go Doc Comments* page,
   dated)"***: the page (https://go.dev/doc/comment) states no refusal of `@param` and never names it, and
   it carries no date of its own. What it says is that *"Named parameters and results can be referred to
   directly in the comment, without any special syntax like backquotes."* Go's choice is a convention, not a
   stated refusal.
3. **`historian.md` Q1, Deno *"a standard library published as packages a program takes or leaves (its
   own words on why)"***: Deno's own post (https://deno.com/blog/std-on-jsr) gives no words on optionality
   or on why JSR; its stated *why* is for independent versioning only (*"driven by the varied maturity,
   complexity, and usage of its components"*). The *takes or leaves* reading is the shape of `deno add`,
   not Deno's words.
4. **`historian.md` Q6 lists Boost's `bcp` beside the robust spelling's question**: not false, but `bcp`
   is the other route. It finds what to place by scanning: *"C++ source files are scanned for #includes"*
   (https://raw.githubusercontent.com/boostorg/bcp/develop/doc/bcp.qbk), panel 250's M at Boost's scale, not
   a list the program writes.
5. **This seat's own first writing of Q5, corrected before the block closed**: it said the
   compiler-engineer *measured* (W) at zero happy-path instructions; his report says *"0 instructions on a
   program's happy path by construction"* (`compiler-engineer.md:526`). And it quoted Rust's
   `std::backtrace` page as if it spoke of a panic's default output; it speaks of `Backtrace::capture`.

Nothing in the spec-warden's report claims a fact about another language. The compiler-engineer's two
(automake's status 77 as a skip; `#[track_caller]`'s shape) hold against their pages (Q4, Q5). The
ffi-pragmatist's (MemorySanitizer unsupported on this Mac's target) agrees with Mbed TLS's page (Q3).

## Question 1: an optional broad library beside a language

- `verdict`: **approve** the robust arm (`design-r`), with the ffi-pragmatist's repairs; **object**
  (advisory) to the conservative, whose standing table is the shape every precedent below moved away from.

- `precedents`:
  1. **Python, PEP 594, removing "dead batteries"** (`verified`,
     https://peps.python.org/pep-0594, read 2026-11-19). Status *"Final"*, by Christian Heimes and Brett
     Cannon; *"Starting with Python 3.11, deprecated modules will start issuing `DeprecationWarning`"*,
     removal in 3.13. Rationale, verbatim: *"Any additional module increases the maintenance cost for the
     Python core development team"*; *"Python's standard library is piling up with cruft, unnecessary
     duplication of functionality, and dispensable features"*; and *"The removal of an unmaintained stdlib
     module increases the chances of a community-contributed module to become widely used."* Python 3.13's
     What's New says *"The remaining 19 'dead batteries' ... have been removed"* (search result's quotation
     of https://docs.python.org/3/whatsnew/3.13.html; **the What's New page itself not fetched: the count
     19 is `unverified`** by this seat). **What happened**: a broad library shipped with the interpreter
     was, thirty years on, paid down module by module.
  2. **Python, Amber Brown at the 2019 Language Summit** (`verified`,
     https://pyfound.blogspot.com/2019/05/amber-brown-batteries-included-but.html, post dated 2019-05-18):
     *"the standard library is where code sometimes goes to die"*; she proposed *"We should embrace PyPI"*
     and a smaller standard library *"as Rust has"*; the post records it as the day's most controversial
     talk. A witness that the cost was named from inside the project, not a ruling.
  3. **Haskell Platform** (`verified`, https://www.haskell.org/platform/): *"The Haskell Platform is
     deprecated since 2022 and is no longer the recommended way of installing Haskell."* The page gives no
     reason. Its repository's issue 350, *"Archive this repository"*, opened 2022-02-01 (`verified`,
     https://github.com/haskell/haskell-platform/issues/350): *"Since the HP is riding into the sunset (in
     favor of GHCUP)"*; closed the next day. **What happened**: a curated distribution beside the compiler
     was retired for an installer of the compiler alone. The stated reason is that one clause; anything more
     is `unverified`.
  4. **Rust, the "Rust Platform" proposal of 2016** (`verified`,
     https://internals.rust-lang.org/t/follow-up-the-rust-platform/3782, aturon, 2016-07-29): the proposal
     was *"a kind of 'extended standard library' made up of curated crates from the ecosystem"*; it
     *"generated a lot of feedback, much of it negative"*; one concern, *"The platform would introduce a
     sharp cliff between the crates it includes and the rest of the ecosystem"*; commenters cited the
     Haskell Platform as a cautionary example; the follow-up calls the idea *"perhaps an over-reaching
     one"* and turns to crates.io's discoverability. **Not a formal rejection**: the page states none.
  5. **D, two standard libraries and a shared runtime** (`verified`,
     https://dlang.org/changelog/2.020.html): DMD 2.020, *"released Oct 20, 2008"*: *"Based on Sean
     Kelly's hard work, Phobos has been split into two libraries, druntime.lib and phobos.lib"*, which
     *"will enable better integration with Tango."* A community witness of the split's weight (`verified`
     as what the post says, https://lists.puremagic.com/pipermail/digitalmars-d/2008-August/040826.html,
     2008-08-13, subject *"The Death of D. (Was Tango vs Phobos)"*): *"I see the D community heading for a
     split"*, D1 with Tango on one side and *"D2+phobos"* on the other. **What happened**: two libraries
     over two incompatible runtimes fragmented D1; the repair was to make the runtime the one shared piece
     beneath both. That any later D2 port of Tango coexisted with Phobos is `unverified` (forum posts only).
  6. **D, the compiler kept off its own library** (`verified`,
     https://forum.dlang.org/post/ultcue$1gv0$1@digitalmars.com, thread *"D is our last hope"*,
     2023-12-19). Walter Bright: *"In order for our ease of debugging, we keep dmd's dependence on other
     code at a minimum"*; *"It's hard to port dmd to a new platform when it relies on Phobos and Phobos is
     not working on the new platform"*; *"There also isn't much that Phobos can add to the compiler"*;
     *"Some things that did help were just incorporated into the compiler source."* Steven Schveighoffer:
     *"it's better to have a *subset* of library that we can maintain separately, and with the front end
     ... bootstrapping is much faster, and more reliable."* **And its cost, stated in the same thread**:
     FeepingCreature, *"it's insane that the premier project in the language refuses to use its stdlib."*
     Earlier in the same practice (`verified`, https://forum.dlang.org/post/trrmir$1ikl$1@digitalmars.com,
     2023-02-06): Bright, *"At least (mostly) the compiler test suite does not depend on a functioning
     Phobos"*, and Paul Backus, *"actually there are a bunch of tests in the compiler's test suite that rely
     on Phobos."* **This is decision 6 reached first**, with both its reason (debugging, porting,
     bootstrapping) and its price (no dogfooding; the separation leaks in tests) on record.
  7. **Go, the `golang.org/x` repositories** (`verified`, https://go.dev/wiki/X-Repositories): *"The
     `golang.org/x/...` repositories are part of the Go Project but outside the main Go tree"*; *"They are
     developed under looser compatibility requirements than the Go core"*; *"In general, they will support
     the previous two releases and tip"*; *"These repositories should have no third-party dependencies apart
     from other golang.org/x/… repositories."* The page does not say how they relate to the standard
     library; Go's own standard library is broad and ships with the toolchain (`unverified` here, not read
     today).
  8. **Deno, the standard library moved to JSR** (`verified`, https://deno.com/blog/std-on-jsr, 2024-05-14):
     *"has officially moved to JSR"*, *"organized into 37 distinct packages, such as `@std/fs` and
     `@std/path`"*, added by `deno add @std/fs @std/path`; *"By adopting independent versioning for each
     package, we can stabilize mature packages promptly"*. **The page does not say the library is optional
     or unbundled**, nor why JSR beyond the move itself; a reading of Deno as "a standard library a program
     takes or leaves" is the shape of `deno add`, not the post's words.
  9. **OCaml, Jane Street's Base** (`verified`, https://github.com/janestreet/base): *"Base is a standard
     library for OCaml"*; *"Base is meant to be used as a wholesale replacement of the standard library
     distributed with the OCaml compiler."* A second standard library offered by a third party beside the
     compiler's own. Whether Base, Core or Batteries became a de facto standard is `unverified` (no page
     read states it).
  10. **Odin's three collections** (`verified`, https://pkg.odin-lang.org/): `base`, *"The base every Odin
      program builds on: the runtime, built-in procedures, and compiler intrinsics"*; `core`, *"The core
      library: formatted printing, strings, memory and allocators, containers, math, encoding"*; `vendor`,
      *"Bindings to popular third-party libraries ... all shipped with the compiler."* Only `vendor`'s line
      says *shipped with the compiler*. design.md §1.11 already cites Odin (`docs/design.md:535-536`, read).

- `argument`: The record runs one way. Libraries shipped with a language became its maintainers' debt
  (PEP 594: *"Any additional module increases the maintenance cost"*); a blessed set beside a compiler was
  retired (Haskell Platform, 2022) or refused as a *"sharp cliff"* (Rust, 2016); two libraries over two
  runtimes split D until druntime became the one shared floor. DMD reached decision 6 first, keeping the
  compiler off Phobos for debugging and bootstrapping, and recorded its price: no dogfooding, and tests
  that leak. So the robust arm's claim is precedented. Its one lesson from D: the runtime is the shared
  floor, and the ffi-pragmatist measured 39,110 lines of platform C grown under Rebel, so *"nothing in the
  runtime grows with Rebel"* is the sentence history warns against.

- `condition`: a language that offered one optional broad library at one version beside a minimal runtime
  and recorded a second rival library fragmenting its programs despite the shared runtime; or a record
  that a compiler kept off its library paid more than DMD's stated price (a bug in the library the
  compiler's own use would have found first, measured).

- **Candidate `## Prior art` lines** (each from the page's own words; the session filing the decision
  re-reads them):
  - `- **Reached first by D (DMD)**: [Walter Bright, "D is our last hope", 2023-12-19](https://forum.dlang.org/post/ultcue$1gv0$1@digitalmars.com): "we keep dmd's dependence on other code at a minimum"; "There also isn't much that Phobos can add to the compiler."`
  - `- **Reached first by Go**: [Go Wiki: X-Repositories](https://go.dev/wiki/X-Repositories): "The golang.org/x/... repositories are part of the Go Project but outside the main Go tree"; "They are developed under looser compatibility requirements than the Go core."`
  - `- **Reached first by Python**: [PEP 594](https://peps.python.org/pep-0594): "Any additional module increases the maintenance cost for the Python core development team."`
  - `- **Reached first by D**: [DMD 2.020 changelog](https://dlang.org/changelog/2.020.html): "Phobos has been split into two libraries, druntime.lib and phobos.lib", which "will enable better integration with Tango."`

- `search`: WebSearch, 2026-11-19, queries: *PEP 594 removing dead batteries ... 3.13 removed modules*;
  *Haskell Platform deprecated announcement GHCup reason*; *D language Phobos Tango split two standard
  libraries history druntime D2*; *Deno standard library JSR @std packages stabilized announcement
  independent versions*; *golang.org/x repositories "part of the Go Project but outside the main Go tree"*;
  *Rust "stdx" brson blessed crates Rust Platform proposal 2016 aturon rejected*; *Jane Street Base
  "alternative to the OCaml standard library" Core Batteries*; *dmd frontend does not use Phobos policy
  ... ddmd*; *Odin language library collections "core" "vendor" base packages*; *Python "batteries
  included" "dead batteries" Amber Brown 2019*. WebFetch of the ten pages cited above, plus
  `opensource.janestreet.com/base` (a redirect to GitHub, followed). **Not found or not read**: the
  original 2016 Rust Platform post (only its follow-up read); OCaml Batteries' own page; a primary record
  of Tango's D2 port; Nim's and Zig's statements on shipping a library with the compiler (searched for
  neither by name today; the brief's Zig/Nim bullet is unanswered here, a question rather than a finding).
  The book *Heroes of code* was not used for any claim.

## Question 2: a documentation verb, and examples that are tests

- `verdict`: **approve** (advisory) the verb reading the compiler's own parse and check. Two of the
  refusals the seats built were reached first by named tools; the third that the seats ask for, a missing
  parameter line, was **not** refused as an error by any tool read today, so adopting it is a departure the
  record should state as deliberate.

- `precedents`:
  1. **Go, `go doc`, a subcommand of the one command** (`verified`,
     https://pkg.go.dev/cmd/go#hdr-Show_documentation_for_package_or_symbol): *"Doc prints the
     documentation comments associated with the item identified by its arguments"* (a package, const, func,
     type, var, method, or struct field), argument forms `go doc encoding/json`, `go doc json.Number.Int64`;
     flags *"-all"*, *"-src"*, *"-u"*. A documentation verb inside one command, not a second binary.
  2. **Go, doc comments** (`verified`, https://go.dev/doc/comment): *"Named parameters and results can be
     referred to directly in the comment, without any special syntax like backquotes"*; *"doc comments for
     types start with complete sentences naming the declared symbol"*; *"The standard formatter gofmt
     reformats doc comments to use a canonical formatting"*; *"`go doc` and `pkgsite` rewrap doc comment
     text when printing it."* **The page never mentions `@param` or any per-parameter list**, and gives no
     date of its own (*"The # syntax was added in Go 1.19"*). Go chose prose naming the parameters; Heroes'
     `` - `name`: `` list per parameter is the opposite choice, closer to Javadoc's `@param`.
  3. **Go, testable examples attached by name** (`verified`, https://pkg.go.dev/testing): *"The naming
     convention to declare examples for the package, a function F, a type T and method M on type T are:"*
     `func ExampleF() { ... }`, `func ExampleT_M() { ... }`; *"Multiple example functions ... may be
     provided by appending a distinct suffix to the name"*; *"Example functions may include a concluding
     line comment that begins with "Output:" and is compared with the standard output of the function when
     the tests are run"*; *"Example functions without output comments are compiled but not executed."* The
     example is a test of the package, attached to its function **by the function's name**, which is
     Rebel's *"title opening with the function's name and a colon"* (§ A7).
  4. **Go, `vet`'s `tests` analyzer** (`verified`,
     https://pkg.go.dev/golang.org/x/tools/go/analysis/passes/tests): it checks *"Test, Benchmark, Fuzzing
     and Example functions checking malformed names, wrong signatures and examples documenting non-existent
     identifiers."* The compiler-engineer's `doc_example_unknown` reached first. (Whether `go test` runs
     this analyzer by default is `unverified`: the page read does not say.)
  5. **D, documented unittests** (`verified`, https://dlang.org/changelog/2.063.html, *"released May 28,
     2013"*): *"Added documented unittest feature for verifiable code example generation"*; *"Documented
     unittests which follow any symbol declarations are now used to generate example sections for the
     symbol"*; *"This allows the implementor of the function to keep their examples always up-to-date."*
     The spec (`verified`, https://dlang.org/spec/unittest.html): *"If a declaration is followed by a
     documented unittest, the code in the unittest will be inserted in the **example** section of the
     declaration"*; documented unittests *"will be attached to the last non-unittest declaration"*;
     *"A unittest which is not documented, or is marked as private will not be used to generate code
     samples."* Its origin (`verified`, the archived Bugzilla issue 2630,
     https://digitalmars.com/d/archives/digitalmars/D/bugs/Issue_2630_New_ddoc_should_be_able_to_document_unittests_16108.html,
     Jan 28 2009, reported by Andrei Alexandrescu's address): *"keeping them in sync with the library and
     its unittests."* **D attaches by position, Go by name**; Heroes' shape is Go's attachment with D's
     documented test block.
  6. **Rust, doctests and `# Examples`** (`verified`): rustdoc *"supports executing your documentation
     examples as tests"*, *"This makes sure that examples within your documentation are up to date and
     working"* (https://doc.rust-lang.org/rustdoc/write-documentation/documentation-tests.html), the
     example living in the comment and the test made from it, **the reverse arrow panel 136 refused**. RFC
     1574 (https://rust-lang.github.io/rfcs/1574-more-api-documentation-conventions.html, *"Start Date:
     2016-03-31"*): *"Everything should have examples"*; *"use the plural form: 'Examples' rather than
     'Example'. Future tooling is easier this way"*; *"the first line should be a single-line short
     sentence providing a summary"*; it says nothing of parameters. The API guidelines' C-EXAMPLE
     (https://rust-lang.github.io/api-guidelines/documentation.html): *"Every public module, trait, struct,
     enum, function, method, macro, and type definition should have an example"*. **And what Rust enforces**
     (https://doc.rust-lang.org/rustdoc/lints.html): `missing_docs` *"is allowed by default"*;
     `missing_doc_code_examples` *"is allowed by default and is nightly-only"*; `broken_intra_doc_links`
     *"warns by default"*. Rust has no per-parameter syntax to check.
  7. **Javadoc's DocLint, the closest precedent for the refusals** (`verified`,
     https://docs.oracle.com/en/java/javase/21/docs/specs/man/javadoc.html, the JDK 21 manual page):
     *"By default, the `-Xdoclint` option is enabled."* Group `reference`: *"a bad name is given for
     `@param` or `@throws`. Issues are typically reported as errors because while the issue may not cause
     problems in the generated files, the author has likely made a mistake"*. Group `missing`: *"a missing
     comment on a class declaration, or a missing `@param` or `@return` tag ... Issues related to missing
     items are typically reported as warnings because they are unlikely to be reported as errors by
     downstream validation tools"*. **Javadoc draws the exact line the seats found in Rebel**: a wrong name
     refused, a missing line warned. JEP 172 (DocLint) and its JDK release could not be read (openjdk.org
     answered 403): `unverified`.
  8. **Byte-identical documentation, as an option** (`verified`, the same Javadoc page): `-notimestamp`
     *"Suppresses the time stamp ... useful when you want to run the `javadoc` tool on two source bases and
     get the differences between `diff` them"*; `--date` *"Using a specific value means the generated
     documentation can be part of a reproducible build. If the option is not given, the default value is the
     current date and time."* So Javadoc's default output differs run to run, and determinism arrived as
     options. The release that added `--date` is `unverified` (its JBS issue, JDK-8272984, answered 403).
     No tool read today promises the same bytes by default.

- `argument`: `go doc` is the precedent for a verb inside one command printing a package's reference.
  Examples as named tests are Go's (`ExampleF`, attached by name) and D's (documented unittests, 2.063,
  attached by position); Rebel's title prefix is Go's attachment. Two refusals were reached first:
  Javadoc's `reference` group errors on a bad `@param` name, Go's vet on *"examples documenting
  non-existent identifiers"*. The missing line, Rebel's commonest departure (626 by the census, 661 by the
  compiler-engineer's reading), is where every tool read stops: Javadoc warns, rustdoc allows by default.
  Refusing it fits the thesis, and is a departure; the record should say so. Same bytes by default goes
  further than Javadoc, which made it an option.

- `condition`: a documentation tool that refused a missing parameter description as an error and retreated
  to a warning for a stated reason (that would argue for a warning class here); or a record that
  attaching examples by name broke under renames where attaching by position did not.

- **Candidate `## Prior art` lines**:
  - `- **Reached first by Go**: [go doc](https://pkg.go.dev/cmd/go#hdr-Show_documentation_for_package_or_symbol): "Doc prints the documentation comments associated with the item identified by its arguments".`
  - `- **Reached first by Go**: [package testing, Examples](https://pkg.go.dev/testing): "The naming convention to declare examples for the package, a function F, a type T and method M on type T are: func ExampleF() ..."`
  - `- **Reached first by D**: [DMD 2.063 changelog](https://dlang.org/changelog/2.063.html): "Documented unittests which follow any symbol declarations are now used to generate example sections for the symbol"; "This allows the implementor of the function to keep their examples always up-to-date."`
  - `- **Reached first by Java (javadoc)**: [javadoc, JDK 21](https://docs.oracle.com/en/java/javase/21/docs/specs/man/javadoc.html): DocLint's reference group reports "a bad name is given for @param" as an error; its missing group reports "a missing @param" as a warning.`
  - `- **Reached first by Go (vet)**: [the tests analyzer](https://pkg.go.dev/golang.org/x/tools/go/analysis/passes/tests): it checks "examples documenting non-existent identifiers".`

- `search`: WebFetch of go.dev/doc/comment, pkg.go.dev/testing (its first 100,000 characters; the
  Examples and Skipping sections fell inside them), pkg.go.dev/cmd/go (the `go doc` section),
  pkg.go.dev/golang.org/x/tools/go/analysis/passes/tests, dlang.org/spec/unittest.html,
  dlang.org/changelog/2.063.html, the Bugzilla 2630 archive, rustdoc's documentation-tests,
  what-to-include and lints pages, RFC 1574, the Rust API guidelines' documentation page, Oracle's JDK 21
  javadoc page. WebSearch: *DMD changelog documented unittests ddoc examples introduced 2.063*; *javadoc
  doclint "@param name not found" error -Xdoclint parameter check*; *javadoc --date option reproducible
  builds notimestamp*. **Refused by the server (403)**: openjdk.org/jeps/172, bugs.openjdk.org/browse/
  JDK-8272984, mail.openjdk.org's integration message. **Not searched**: pkgsite's own determinism,
  rustdoc's reproducibility, Sphinx or Doxygen; a sentence that no tool promises the same bytes by default
  rests on these four tools alone.

## Question 3: cryptography where the platform's own library lacks it

- `verdict`: **approve (S) now** (advisory), the route .NET took in 2022 and Apple's own engineers
  recommend; **approve (H) as the direction only behind one of the three enablers every precedent read
  used** (an explicit wrapping operator, as Swift and Zig have; overflow checks proved away, as SPARK does;
  or constant-time operations owned by the compiler, as Go's intrinsics are), and **object to writing
  ciphers in Heroes under today's emission**; (V) holds no small library read today with all four
  primitives in portable C.

- `precedents`:
  1. **Compilers break constant-time source, on record.** (a) **Clangover** (`verified`,
     https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/hqbtIGFKIpU, Antoon Purnal, 2024-06-03,
     *"Compiler-introduced timing leak in Kyber reference implementation"*): *"recent versions of Clang
     (15-18) can produce a secret-dependent branch in the Kyber / ML-KEM reference implementation"*, in
     `poly_frommsg`, at `-Os`, `-O1`, and `-O2`/`-O3` with `-fno-vectorize`; its proof of concept
     (`verified`, https://github.com/antoonpurnal/clangover): *"ISA: x86"*, and on an i7-13700H with Clang
     16.0.6 *"it takes between 5-10 minutes to leak the entire ML-KEM 512 secret key"*. (b) **"Breaking
     Bad"** (`verified`, https://arxiv.org/abs/2410.13489, Schneider, Lain, Puddu, Dutly, Čapkun, submitted
     2024-10-17, revised 2025-09-02): *"trace-based dynamic analysis on 44,604 different targets"* over
     *"x86-64, x86-i386, armv7, aarch64, RISC-V, and MIPS-32"*; *"Several compiler-induced secret-dependent
     operations occur within some of the most highly regarded hardened cryptographic libraries"*; current
     defensive techniques are *"inadequate, incomplete, and bound to fail"*. The abstract page names no
     compiler, level or pass (the paper's body not read: `unverified`). (c) **Simon, Chisnall, Anderson,
     "What you get is what you C"** (`verified`, https://www.cl.cam.ac.uk/~rja14/Papers/whatyouc.pdf, read
     through its first two pages): *"A compiler upgrade can suddenly and without warning open a timing
     channel in previously secure code. This arms race is pointless and has to stop"*; *"there is no way to
     express this in C"*; they *"implement constant-time selection and register/stack erasure in
     Clang/LLVM"*. Its venue, EuroS&P 2018, is the search result's (`unverified` from the pages read).
  2. **Go writes its cryptography in Go, and how** (`verified`). Its FIPS module (https://go.dev/blog/fips140,
     2025-07-15): *"The FIPS 140-3 validated Go Cryptographic Module now underlies Go's built-in crypto
     libraries"*, v1.0.0 *"included in Go 1.24"*, its packages *"involve no cgo"*; the post reports CAVP
     certificate A6650 and CMVP *"submitted"*, so a CMVP certificate is `unverified` here. **Constant time
     owned by the compiler**: `crypto/internal/constanttime` (https://go.dev/src/crypto/internal/constanttime/constant_time.go):
     *"The functions in this package are compiler intrinsics for constant-time operations"*; `Select`
     *"is intrinsicified on arches with CMOV"*; `boolToUint8` *"is a compiler intrinsic"*, its Go body
     `panic("unreachable; must be intrinsicified")`; `crypto/subtle` forwards to it *"for intrinsified"*
     (https://go.dev/src/crypto/subtle/constant_time.go). The release that added this package is
     `unverified`. **Hardware**: `subtle.WithDataIndependentTiming`, *"Added in go1.24.0"*: *"On Arm64
     processors with FEAT_DIT, WithDataIndependentTiming enables PSTATE.DIT"*, and on all other
     architectures it *"executes f immediately with no other side-effects"* (https://pkg.go.dev/crypto/subtle).
     **Generated field arithmetic**: `p256_fiat64.go` opens *"Code generated by Fiat Cryptography. DO NOT
     EDIT."* (https://go.dev/src/crypto/internal/fips140/nistec/fiat/p256_fiat64.go). **Review rules**:
     *"We prefer portable Go, not assembly"*; *"Code in assembly means (N packages * M architectures) to
     maintain"* (https://go.dev/wiki/AssemblyPolicy). **No page read says Go runs a constant-time checker on
     its binaries**: a question, searched only as far as these pages.
  3. **Constant time where overflow is checked.** **SPARKNaCl** (`verified`,
     https://github.com/rod-chapman/SPARKNaCl): a *"SPARK 2014 re-implementation of the TweetNaCl crypto
     library"*, a *"completely automated static proof of type-safety"* ruling out *"numeric overflows"*;
     *"The code retains the "constant time" algorithms from TweetNaCl"*; BSD 3-clause. How its arithmetic
     lives with checked overflow (https://www.adacore.com/blog/proving-constant-time-crypto-code-in-sparknacl,
     Rod Chapman, 2020-04-02): limbs of 16 bits held in 64, *"(64 bits are used so these digits can
     "overflow" during intermediate computations...)"*, ranges held by subtypes with `Dynamic_Predicate`,
     and modular types used for bitwise work. And what happens to the checks
     (https://www.adacore.com/blog/performance-analysis-and-tuning-of-sparknacl, 2021-02-09): *"SPARK offers
     static type safety, so programs can be compiled and run with no runtime type checking at all."* **The
     check is proved away, not kept**; Heroes keeps it as an abort, which is the `b.vs` per secret sum the
     ffi-pragmatist measured (336 per ChaCha20 block under MemorySanitizer). **Languages that trap by
     default and offer an explicit wrapping operator**: Swift (`verified`,
     https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/AdvancedOperators.md):
     *"by default Swift reports an error rather than allowing an invalid value to be created"*, and
     *"Swift provides three arithmetic overflow operators that opt in to the overflow behavior"*, `&+`,
     `&-`, `&*`; Zig (`verified`, https://ziglang.org/documentation/master/): *"Operators such as `+` and
     `-` cause Illegal Behavior on integer overflow"*; *"`+%` and `-%` perform wrapping arithmetic"*.
  4. **Constant-time checkers** (`verified`). **dudect** (https://eprint.iacr.org/2016/1123, Reparaz,
     Balasch, Verbauwhede, *"DATE 2017"*): *"a tool to assess whether a piece of code runs in constant time
     or not on a given platform"*, *"around 300 lines of C"*, *"runs on the target platform"*, *"requires
     no modeling of hardware behavior"*: a clock-based test, which this sitting's rule owes to a still
     machine. **Valgrind as a taint checker** (BoringSSL commit `a6a049a6`, Adam Langley, 2018-12-06,
     https://boringssl.googlesource.com/boringssl/+/a6a049a6fb51a052347611d41583a0622bc89d60):
     *"Valgrind's checking of uninitialised memory behaves very much like a check for constant-time code:
     branches and memory indexes based on uninitialised memory trigger warnings"*; *"This was the idea
     behind https://github.com/agl/ctgrind"*; `CONSTTIME_SECRET` and `CONSTTIME_DECLASSIFY` *"are no-ops
     unless the code is built with |BORINGSSL_CONSTANT_TIME_VALIDATION| defined, which it isn't by
     default."* **Which platforms**: Mbed TLS's page (https://mbed-tls.readthedocs.io/en/latest/kb/testing/testing-constant-flow/)
     says MemorySanitizer works only with clang on Linux, FreeBSD and NetBSD, and Valgrind memcheck costs
     about 20 to 30 times the run (the fetch returned its sense, not a verbatim sentence: the words are
     `unverified`); the ffi-pragmatist's run agrees for this Mac (*"unsupported option '-fsanitize=memory'
     for target 'arm64-apple-darwin27.2.0'"*). Valgrind's own platform list could not be read
     (https://valgrind.org/info/platforms.html answered 503 twice): **whether any taint checker runs on
     macOS arm64 or Windows is `unverified`**.
  5. **Apple** (`verified`). CryptoKit is Swift's alone: Quinn "The Eskimo!", Apple DTS, Nov 2021
     (https://developer.apple.com/forums/thread/695447): *"The only public API for AES-GCM is the one
     provided by Apple CryptoKit. That is only available in Swift. If you want to use it from Objective-C,
     create a Swift wrapper that publishes the necessary functionality to your Objective-C code."* On
     CommonCrypto's GCM in `CommonCryptorSPI.h`, Apr 2023: *"No. This is likely to get your rejected by the
     App Store ingestion process but, even if you get past that, you run the risk of binary compatibility
     problems down the line."* Jul 2023, the options: *"Bridge over to the Apple CryptoKit AES-GCM
     implementation"* or *"Write or acquire your own AES-GCM code."* **swift-crypto**
     (https://github.com/apple/swift-crypto): *"an open-source implementation of a substantial portion of
     the API of Apple CryptoKit"*; on Apple platforms it *"compiles its entire API surface down to nothing
     and simply re-exports the API of CryptoKit"*; elsewhere *"A vendored copy of BoringSSL's libcrypto"*;
     Apache-2.0. **Apple's own library uses the hardware bit**: Apple Security Research, 2026-05-22
     (https://security.apple.com/blog/formal-verification-corecrypto): corecrypto's guidelines *"take
     advantage of hardware features like Data Independent Timing (DIT)"*, and *"our approach assumes that
     the compiler ... behaves correctly."* **`swiftc` in the Command Line Tools**: no Apple page read says
     so or the opposite; the one Apple answer found (DTS, Jul 2021,
     https://developer.apple.com/forums/thread/684252) offers *"a package of just the command line tools"*
     when asked for a Swift compiler without Xcode, and does not name `swiftc`. The critic's measurement on
     this Mac is the only evidence; the page asked for is `unverified` (not found).
  6. **Route (S), reached first by .NET** (`verified`). Commit `e0294993`, *"Implement AES-GCM with CryptoKit
     on macOS"*, by vcsjones (https://github.com/dotnet/runtime/commit/e0294993ba0c384a9ab5a778c2ba96731511bf9d):
     *"With this change, GCM on macOS no longer uses OpenSSL, but routes into the CryptoKit library"*; its
     `AesGcm.macOS.cs`: *"CryptoKit added AES.GCM in macOS 10.15, which is our minimum target for macOS"*.
     The Swift file it adds, as of that commit
     (https://raw.githubusercontent.com/dotnet/runtime/e0294993ba0c384a9ab5a778c2ba96731511bf9d/src/native/libs/System.Security.Cryptography.Native.Apple/pal_swiftbindings.swift):
     `import CryptoKit`, `@_cdecl("AppleCryptoNative_ChaCha20Poly1305Encrypt")`, `...Decrypt`,
     `AppleCryptoNative_AesGcmEncrypt` and `...Decrypt`, **the `@_cdecl` member the brief's route (S) builds**.
     Its pull request, 76490, merged 2022-11-08 (https://github.com/dotnet/runtime/pull/76490), notes a
     dependency that was *"bumping the native macOS toolchain"*. Microsoft's page today
     (https://learn.microsoft.com/en-us/dotnet/standard/security/cross-platform-cryptography): *"On Apple
     platforms, the tag size is limited to 128-bit (16-byte) due to limitations of the CryptoKit
     framework."* **And what changed since**: the same file on `main` today binds by `@_silgen_name` in
     Swift's calling convention, not `@_cdecl` (fetched today); a route Heroes, which speaks the C ABI only
     (design.md §1.11 point 1), cannot take.
  7. **Windows CNG** (`verified`). Named curves
     (https://learn.microsoft.com/en-us/windows/win32/seccng/cng-named-elliptic-curves, `ms.date`
     2024-10-30): *"Beginning in Windows 10, CNG provides support for the following named elliptic
     curves"*, among them `BCRYPT_ECC_CURVE_25519`, *"curve25519"*, 255 bits, *"Object identifier: None"*,
     used through `BCRYPT_ECDSA_ALGORITHM` or `BCRYPT_ECDH_ALGORITHM`. Algorithm identifiers
     (https://learn.microsoft.com/en-us/windows/win32/seccng/cng-algorithm-identifiers, `ms.date`
     2025-05-27): `BCRYPT_CHACHA20_POLY1305_ALGORITHM`, *"Support for this algorithm begins with Windows
     10"* (.NET's page says *"Windows 10 Build 20142+"*); **no EdDSA or Ed25519 identifier stands on the
     page**. So X25519 by ECDH on curve25519 is documented since Windows 10, and Ed25519 has no CNG
     identifier as of that page. X25519's key-blob byte order is on no page read: `unverified`, owed to the
     box.
  8. **Small constant-time C libraries** (route V). **Monocypher** (`verified`, https://monocypher.org/):
     *"counts under 2000 lines of code"*; *"dedicated to the public domain (CC0-1.0, alternatively 2-clause
     BSD)"*; XChaCha20 and Poly1305, X25519, EdDSA with Ed25519 (SHA-512) as an option, BLAKE2b, Argon2i;
     **no AES**. Audit (`verified`, https://monocypher.org/quality-assurance/audit): *"Monocypher has been
     audited by Cure53 in June 2020"*, the Open Technology Fund thanked, *"no issues of High or Critical
     severity could be spotted"*. Whether its ChaCha20-Poly1305 is RFC 8439's 96-bit-nonce form is
     `unverified` (the page names XChaCha20). **libsodium**: audited by *"Dr. Matthew Green of Cryptography
     Engineering"*, *"v1.0.12 and v1.0.13"*, *"no critical flaws or vulnerabilities"* (`verified`,
     https://www.privateinternetaccess.com/blog/2017/08/libsodium-audit-results/); its AES-256-GCM
     (`verified`, https://doc.libsodium.org/secret-key_cryptography/aead/aes-256-gcm) *"requires the Intel
     AES-NI extensions, or the ARM Crypto extensions"*, and *"There are no plans to support non
     hardware-accelerated implementations of AES-GCM"*; its licence and size are `unverified` (not read).
     **TweetNaCl**: its pages (tweetnacl.cr.yp.to) failed to load three times: everything about it is
     `unverified`.

- `argument`: The record says three things. Compilers turn constant-time source into secret branches
  (Clangover, 2024; 44,604 targets in "Breaking Bad"). Go, the precedent decision 29 names, keeps constant
  time with compiler intrinsics, Arm's DIT bit and generated field code; SPARK keeps it under checked
  arithmetic by proving the checks away; Swift and Zig trap by default and wrap on request. Heroes has
  none of the three, and the ffi-pragmatist measured the consequence: a branch per secret sum. So (H) is
  precedented only behind one of those enablers, a language or compiler decision first. (S) is exactly
  what .NET shipped in 2022 and Apple recommends; its price is `swiftc` on every Mac.

- `condition`: a language that wrote constant-time ciphers with trapping arithmetic, **no** wrapping
  operator, checks kept in the binary, and a checker that declassified the abort edge (that would make (H)
  precedented under today's emission); or a record of .NET's Swift member breaking a build on a Mac
  without Xcode (that would price (S) higher).

- **Candidate `## Prior art` lines** (each only if the synthesis adopts the matching route):
  - `- **Reached first by .NET**: [dotnet/runtime e0294993, "Implement AES-GCM with CryptoKit on macOS"](https://github.com/dotnet/runtime/commit/e0294993ba0c384a9ab5a778c2ba96731511bf9d): "GCM on macOS no longer uses OpenSSL, but routes into the CryptoKit library"; its pal_swiftbindings.swift exports @_cdecl("AppleCryptoNative_AesGcmEncrypt").`
  - `- **Taken from Apple (Developer Technical Support)**: [How to use aes-256-gcm in objective-c](https://developer.apple.com/forums/thread/695447): "The only public API for AES-GCM is the one provided by Apple CryptoKit. That is only available in Swift."`
  - `- **Reached first by Go**: [crypto/internal/constanttime](https://go.dev/src/crypto/internal/constanttime/constant_time.go): "The functions in this package are compiler intrinsics for constant-time operations."`
  - `- **Taken from BoringSSL**: [commit a6a049a6](https://boringssl.googlesource.com/boringssl/+/a6a049a6fb51a052347611d41583a0622bc89d60): "Valgrind's checking of uninitialised memory behaves very much like a check for constant-time code: branches and memory indexes based on uninitialised memory trigger warnings."`
  - `- **Taken from PQShield (Clangover)**: [pqc-forum, 2024-06-03](https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/hqbtIGFKIpU): "recent versions of Clang (15-18) can produce a secret-dependent branch in the Kyber / ML-KEM reference implementation."`

- `search`: WebSearch, 2026-11-19: *Go 1.24 FIPS 140-3 Go Cryptographic Module*; *dudect "Dude, is my code
  constant time?"*; *BoringSSL CONSTTIME_SECRET valgrind*; *"Breaking Bad: How Compilers Break
  Constant-Time Implementations"*; *Clangover Antoon Purnal 2024*; *"What you get is what you C"*;
  *CommonCrypto CCCryptorGCM private SPI*; *Apple Command Line Tools ... swiftc*; *CNG named elliptic curves
  BCRYPT_ECC_CURVE_25519 ... Ed25519*; *Windows CNG Ed25519 support ... SymCrypt*; *SPARKNaCl Rod
  Chapman*; *Monocypher audit Cure53*; *libsodium security assessment ... aes256gcm*; *Go cryptography
  principles*; *Apple ... data independent timing DIT*; *dotnet runtime pal_swiftbindings.swift
  CryptoKit*. WebFetch of every page cited above. **Failed**: valgrind.org's platform page (503, twice),
  tweetnacl.cr.yp.to (three fetches, a parse error), developer.apple.com/documentation/cryptokit (no body
  returned), nvd.nist.gov's CVE-2024-37880 (no body). **Not searched**: RustCrypto's or the `subtle`
  crate's practice; ct-verif, FaCT, Jasmin; Windows' AES-GCM chaining mode in CNG.

## Question 4: a test that cannot run here

- `verdict`: **approve** (advisory) a named third outcome, the best-precedented thing this sitting asks
  for; **approve** the compiler-engineer's channel owned by the runner over an exit code (automake's 77 is
  the oldest route, and it reads any program's exit 77 as a skip, the forgery he measured as `FAIL` under
  his channel); **approve** a run of nothing but skips failing, which Catch2 reached first; and the name
  `skip_test`, whose nearest spelling is Python's `skipTest`. No runner read calls its skip a bare global
  `skip`.

- `precedents`:
  1. **Go** (`verified`): Go 1.1's notes (https://go.dev/doc/go1.1): *"a new Skip method of testing.B and
     testing.T to simplify skipping an inappropriate test"*. The package (https://pkg.go.dev/testing):
     *"Tests or benchmarks may be skipped at run time with a call to T.Skip or B.Skip"*; *"SkipNow marks
     the test as having been skipped and stops its execution by calling runtime.Goexit"*; *"Short reports
     whether the -test.short flag is set."* A control-flow signal inside one process.
  2. **Python** (`verified`, https://docs.python.org/3/library/unittest.html): the skipping section is
     *"Added in version 3.1"*; *"calling TestCase.skipTest() within a setUp() or test method, or raising
     SkipTest directly"*; **its own example runs four tests, all skipped, and ends `OK (skipped=4)`**: a run
     of nothing but skips is green there. pytest (https://docs.pytest.org/en/stable/how-to/skipping.html):
     *"skip imperatively during test execution or setup by calling the pytest.skip(reason) function"*;
     *"Detailed information about skipped/xfailed tests is not shown by default"*, `-rs` shows reasons.
     Its exit codes (https://docs.pytest.org/en/stable/reference/exit-codes.html) name code 5, *"No tests
     were collected"*, and none for a run whose tests all skipped (which code such a run returns is not on
     the page: `unverified`).
  3. **JUnit 5** (`verified`, https://docs.junit.org/current/writing-tests/assumptions.html): a failed
     assumption *"throws an exception of type org.opentest4j.TestAbortedException to signal that the test
     should be aborted instead of marked as a failure."* A third outcome named *aborted*.
  4. **Zig** (`verified`, https://ziglang.org/documentation/master/): *"To programmatically skip a test,
     make a test return the error error.SkipZigTest"*; *"the default test runner will consider the test as
     being skipped"*; its example ends *"0 passed; 1 skipped; 0 failed."*
  5. **Catch2** (`verified`,
     https://raw.githubusercontent.com/catchorg/Catch2/devel/docs/skipping-passing-failing.md):
     `SKIP`, *"Introduced in Catch2 3.3.0"*; *"This test case is then reported as skipped instead of passed
     or failed"*; and **when every test case of a run is skipped, Catch2 *"returns a non-zero exit code,
     same as it does if no test cases have run"***, overridden by *"--allow-running-no-tests"*. The floor
     both seats adopted, reached first.
  6. **TAP 14** (`verified`, https://testanything.org/tap-version-14-specification.html): *"If the
     directive starts with # SKIP, the test is counted as a skipped test"*; *"Harnesses must not treat
     failing SKIP test points as a test failure"*; *"Harnesses should report SKIP test points found as a list
     of items that were not tested"*; *"A plan line of 1..0 indicates that the test set was completely
     skipped."*
  7. **The exit-code route** (`verified`). GNU Automake
     (https://www.gnu.org/software/automake/manual/html_node/Scripts_002dbased-Testsuites.html): *"When no
     test protocol is in use, an exit status of 0 from a test script will denote a success, an exit status
     of 77 a skipped test, an exit status of 99 a hard error, and any other exit status will denote a
     failure."* Meson (https://mesonbuild.com/Unit-tests.html): *"the GNU standard approach in this case is
     to exit the program with error code 77"*; *"Meson will detect this and report these tests as skipped
     rather than failed"*, since 0.37.0. CMake's CTest
     (https://cmake.org/cmake/help/latest/prop_test/SKIP_RETURN_CODE.html): `SKIP_RETURN_CODE`, *"Return
     code to mark a test as skipped. Valid values are in the range of 0 to 255"*, marking the test *"Not
     Run"*. **Three process-per-test runners read a skip from a status any program can give**; Heroes'
     `exit(code:)` gives every one of 0 to 255 (the compiler-engineer's reading of spec § 11), which is why
     his channel departs from them.
  8. **Rust, the holdout** (`verified`, https://doc.rust-lang.org/reference/attributes/testing.html):
     `#[ignore]` tells *"the test harness to not execute that function as a test"*; *"Ignored tests are
     still compiled when in test mode, but they are not executed"*; a reason by `#[ignore = "..."]`. A choice
     made when the program is written, not at run time. A pre-RFC (`verified`,
     https://internals.rust-lang.org/t/pre-rfc-skippable-tests/14611, mathstuf, 2021-05-02) asked for the
     third outcome: *"If a test ends up skipped, its result is not considered to be either a pass or a
     fail"*, citing TAP, Automake's 77 and Autoconf's `AT_SKIP_IF` as prior art; the page shows no team
     decision. Whether any run-time skip has landed in libtest is `unverified` (no tracking issue found by
     the search below).

- `argument`: A third outcome is the norm: Go since 1.1, Python since 3.1, JUnit 5's *aborted*, Zig,
  Catch2 3.3.0, TAP, Automake, Meson, CTest. Rust is the holdout, and its users wrote a pre-RFC asking for
  exactly this. The channel splits by architecture: in-process runners use control flow (`Goexit`, an
  exception, an error value); process-per-test runners read exit 77, a status any program can give, the
  forgery the compiler-engineer refused. On a run of only skips the record splits too: Python prints
  `OK (skipped=4)` and TAP's `1..0` is a skip, while Catch2 returns non-zero. The floor has Catch2's
  precedent and the harness's own.

- `condition`: a process-per-test runner that carried a skip through a channel the test cannot forge and
  recorded it as costly or fragile; or a record that Catch2's all-skipped failure was reverted, with its
  reason.

- **Candidate `## Prior art` lines**:
  - `- **Reached first by Catch2**: [Skipping, passing and failing](https://raw.githubusercontent.com/catchorg/Catch2/devel/docs/skipping-passing-failing.md): when every test case of a run is skipped, Catch2 "returns a non-zero exit code, same as it does if no test cases have run".`
  - `- **Reached first by Go**: [Go 1.1 release notes](https://go.dev/doc/go1.1): "a new Skip method of testing.B and testing.T to simplify skipping an inappropriate test".`
  - `- **Reached first by TAP**: [TAP 14](https://testanything.org/tap-version-14-specification.html): "Harnesses should report SKIP test points found as a list of items that were not tested".`
  - `- **Reached first by Python**: [unittest, Skipping tests](https://docs.python.org/3/library/unittest.html): "calling TestCase.skipTest() within a setUp() or test method" (only if the name skip_test lands).`

- `search`: WebFetch of the pages cited above; pkg.go.dev/testing's Skipping section read in Q2's fetch.
  WebSearch: *automake parallel test harness exit status 77 skip 99 hard error*; *Rust libtest skip test at
  runtime issue "#[ignore]" dynamic ignore tracking issue*. The Automake page answered 429 twice before it
  answered. **Not read**: JUnit's console launcher's options (its page did not render the section);
  Rust's later *"Skippable Tests"* thread and cargo-nextest; Go proposals to fail a run on skips; any CI
  convention turning skips red beyond Catch2's default.

## Question 5: the stack of calls at an abort

- `verdict`: **approve (W) with (A)** (advisory), each half reached first by a named runtime; **object to
  (P) made general**, which Rust's own RFC declined beyond `unwrap` and `expect` for binary size; on
  x86-64, `-fno-omit-frame-pointer` has two distributions' measured precedent.

- `precedents`:
  1. **Go: one goroutine by default, a table in every binary** (`verified`, https://pkg.go.dev/runtime):
     *"By default, a failure prints a stack trace for the current goroutine, eliding functions internal to
     the run-time system, and then exits with exit code 2"*; *"GOTRACEBACK=single (the default)"*;
     *"GOTRACEBACK=all adds stack traces for all user-created goroutines."* The table it reads (`verified`,
     Russ Cox, *"Go 1.2 Runtime Symbol Information"*, July 2013,
     https://docs.google.com/document/d/1lyPIbmsYbXnpNj57a261hgOYVpNRcgydurVQIyZOz_o/pub): kept for
     crash-time stack traces, and *"In addition to stack traces, the stack walking ability is now used for
     profiling and garbage collection"*; for godoc, the old tables took about 2.1 MB in the file, the new
     ones just under 1 MB (the fetch's summary of the figures, `verified` as the page's sense). Its cost,
     named by the Go team (`verified`, https://github.com/golang/go/issues/36313, *"runtime: pclntab is too
     big"*, Rob Pike, 2019-12-30): Austin Clements, *"The pclntab is large, a significant fraction of Go
     binaries."* No overall percentage is on the thread. **This is route (W) with a table of the
     compiler's own**, paid in every binary because Go's collector needs it too; Heroes' (W) reads frame
     records and the line tables every build already carries instead.
  2. **Rust: no captured backtrace by default, and the caller's place for two functions** (`verified`).
     `std::backtrace` (https://doc.rust-lang.org/std/backtrace/index.html), of `Backtrace::capture`:
     *"Capturing a backtrace can be a quite expensive runtime operation, so the environment variables allow
     either forcibly disabling this runtime performance hit or allow selectively enabling it"*; *"If neither
     of the above env vars are set, then Backtrace::capture will be disabled"*; *"For most platforms a
     backtrace with a filename/line number requires that programs be compiled with debug information"*;
     *"no guarantees are provided about the exact accuracy of a backtrace."* What the default panic hook
     prints is not on that page (`unverified` here). `#[track_caller]`, RFC 2091
     (https://rust-lang.github.io/rfcs/2091-inline-semantic.html, start date 2017-07-31): for *"accurate
     caller location reporting during panic in {Option, Result}::{unwrap, expect}"*; the body moved into a
     closure that receives the caller's location as an extra argument (the fetch's summary); its cost, the
     instructions per `unwrap()` grow and the optimizer can no longer merge jumps to a shared panicking
     branch; and for the rarely-panicking functions, *"Inlining them would bloat the binary size without
     much benefit."* **Route (P) made general is what Rust declined**, on binary size.
  3. **A symboliser outside the dying process** (`verified`). AddressSanitizer
     (https://clang.llvm.org/docs/AddressSanitizer.html): *"online symbolization can be force disabled by
     setting ASAN_OPTIONS=symbolize=0"*; *"you can use a separate script to symbolize the result offline"*
     (`asan_symbolize.py`); and *"Note that on macOS you may need to run dsymutil on your binary to have the
     file:line info in the AddressSanitizer reports"*, the critic's Darwin finding reached first.
     `dsymutil` (https://llvm.org/docs/CommandGuide/dsymutil.html): *"dsymutil links the DWARF debug
     information found in the object files for an executable by using debug symbols information contained
     in its symbol table. By default, the linked debug information is placed in a .dSYM bundle"*. Swift's
     backtracer (https://raw.githubusercontent.com/swiftlang/swift/main/docs/Backtracing.rst):
     *"swift-backtrace is a program that gets invoked when your program crashes"*, which symbolicates so the
     crashing process is not disturbed; on Linux enabled by default, on macOS *"The backtracer is not active
     by default on macOS"* (`tty` from macOS 26, `no` before), on iOS and tvOS not at all. The release that
     brought it is not on the page (`unverified`). **Route (A)'s two halves exist**: a runtime that writes
     raw addresses, and a second process that resolves them; neither page read has the program's *parent*
     doing it, so that detail is Heroes' own.
  4. **Zig** (`verified`): its std, where symbols are stripped, prints *"Unable to dump stack trace: debug
     info stripped"* (https://ziglang.org/documentation/0.11.0/std/src/std/debug.zig.html, under
     `if (builtin.strip_debug_info)`); *"Error Return Traces are enabled by default in debug builds and
     disabled by default in fast, safe and small builds"* (https://ziglang.org/documentation/master/, read at
     offset 200,000). Whether a panic's own stack trace prints in ReleaseFast is not on the pages read
     (`unverified`).
  5. **Windows' DbgHelp** (`verified`). `SymInitialize`
     (https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-syminitialize): with no search
     path it searches *"The current working directory of the application"*, `_NT_SYMBOL_PATH` and
     `_NT_ALTERNATE_SYMBOL_PATH`; *"Note that the directory that contains the executable file for the
     process is not automatically part of the search path"*; *"All DbgHelp functions, such as this one, are
     single threaded. Therefore, calls from more than one thread to this function will likely result in
     unexpected behavior or memory corruption. To avoid this, call SymInitialize only when your process
     starts"*. `StackWalk64`
     (https://learn.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-stackwalk64): *"Using the
     StackWalk64 function is recommended over writing your own function because of all the complexities
     associated with stack walking on platforms"*; its function-table callback *"is required because the
     StackWalk64 function does not have access to the process's run-time function table"*. **Two facts for
     the runtime's own note at `stack.c:1231`** (*"a PDB beside the binary"*): beside the binary is not
     searched unless the program adds it, and the calls are single-threaded, the Windows shape of the
     `dladdr` loader-lock question the compiler-engineer left open.
  6. **Frame pointers turned back on, with their cost measured** (`verified`). Fedora
     (https://fedoraproject.org/wiki/Changes/fno-omit-frame-pointer): *"Fedora will add
     -fno-omit-frame-pointer and -mno-omit-leaf-frame-pointer to the default C/C++ compilation flags"*, for
     *"Fedora Linux 38"*, to *"improve the effectiveness of profiling and debugging tools"*; its authors
     measured a kernel build *"2.4% slower with frame pointers"*, Blender 2%, CPython 1 to 10% by benchmark,
     openssl, botan, zstd and Redis not significantly affected. Ubuntu (2023-12-13,
     https://ubuntu.com/blog/ubuntu-performance-engineering-with-frame-pointers-by-default): its GCC *"will
     enable frame pointers by default for 64-bit platforms"* from 24.04 LTS; Brendan Gregg quoted,
     *"typically less than 1%"*; kept off *"where the impact is high (such as the Python interpreter)"*.

- `argument`: Go prints the current goroutine's stack at every failure and pays a table in every binary
  (*"a significant fraction of Go binaries"*). Rust captures no backtrace by default (*"quite expensive"*)
  and passes the caller's place only to `unwrap` and `expect`, declining more as *"bloat"*. So (P) made
  general has an explicit refusal behind it. (W) with (A) has its halves on record: ASan writes addresses
  and symbolises offline, warning that macOS needs `dsymutil`; Swift resolves in a second process. The
  compiler-engineer's (W) costs no happy-path instruction by construction, which neither Go's nor Rust's
  choice offered. Frame pointers on x86-64 cost Fedora about 2% typically, 10% at worst.

- `condition`: a runtime that printed a whole trace at every abort by default and withdrew it (for cost, a
  hang in the loader's lock, or paths leaked into logs); or a language that made the caller's place general
  and measured it cheap.

- **Candidate `## Prior art` lines**:
  - `- **Reached first by Clang (AddressSanitizer)**: [AddressSanitizer, Symbolizing the Reports](https://clang.llvm.org/docs/AddressSanitizer.html): "online symbolization can be force disabled by setting ASAN_OPTIONS=symbolize=0"; "you can use a separate script to symbolize the result offline"; "on macOS you may need to run dsymutil on your binary to have the file:line info".`
  - `- **Reached first by Swift**: [Backtracing](https://raw.githubusercontent.com/swiftlang/swift/main/docs/Backtracing.rst): "swift-backtrace is a program that gets invoked when your program crashes."`
  - `- **Taken from Rust**: [RFC 2091](https://rust-lang.github.io/rfcs/2091-inline-semantic.html): caller location for unwrap and expect only; for the others, "Inlining them would bloat the binary size without much benefit." (only if the synthesis refuses P on it)`
  - `- **Reached first by Go**: [package runtime, GOTRACEBACK](https://pkg.go.dev/runtime): "By default, a failure prints a stack trace for the current goroutine, eliding functions internal to the run-time system, and then exits with exit code 2."`
  - `- **Taken from Fedora**: [Changes/fno-omit-frame-pointer](https://fedoraproject.org/wiki/Changes/fno-omit-frame-pointer): "Fedora will add -fno-omit-frame-pointer and -mno-omit-leaf-frame-pointer to the default C/C++ compilation flags" (only if the x86-64 flag lands).`

- `search`: WebFetch of pkg.go.dev/runtime (first 100,000 characters, GOTRACEBACK inside them), Russ
  Cox's Go 1.2 symbol document, golang/go issue 36313, Rust's `std::backtrace` and RFC 2091, clang's
  AddressSanitizer page, LLVM's dsymutil page, Swift's `docs/Backtracing.rst`, Zig 0.11.0's `debug.zig`
  and the master manual at offset 200,000, Microsoft's `SymInitialize` and `StackWalk64`, Fedora's change
  page, Canonical's blog. WebSearch: *Go runtime pclntab size ... "Go 1.2 Runtime Symbol Information"*;
  *Zig error return traces stack trace Debug ReleaseSafe ReleaseFast*; *Ubuntu 24.04 frame pointers
  enabled by default*. **Not found**: a test runner or launcher that receives a crashed child's raw
  addresses and resolves them as its parent (searched only through the pages above; Go's `go test` and
  Rust's harness were not read on this point), and a primary figure for pclntab's share of a Go binary.

## Question 6: an umbrella at one version, placed package by package

- `verdict`: **approve** (advisory) one version and a list the program writes, `umb-r`'s shape with the
  compiler-engineer's and spec-warden's repairs: Maven's and Compose's bills of materials and Cargo's
  feature lists reached it first. **Object to (M')**, placement computed from imports: Go's vendoring and
  Boost's `bcp` both place that way and both record panel 250's two findings. (E), per-package versions,
  is the road Deno, Compose and the AWS SDK for Go v2 took, and its cost is on record in Deno's own
  tracker.

- `precedents`:
  1. **One version-set, each library declared by the program** (`verified`). Maven
     (https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html): *"it has
     sometimes been difficult to keep the versions in the project using the artifacts in synch with the
     versions distributed in the library. The pattern below illustrates how a "bill of materials" (BOM) can
     be created"*; *"It defines the versions of all the artifacts that will be created in the library. Other
     projects that wish to use the library should import this POM into the dependencyManagement section"*;
     its example's consumer then lists `project1` and `project2` without versions. Jetpack Compose
     (https://developer.android.com/develop/ui/compose/bom): *"The Compose Bill of Materials (BOM) lets you
     manage all of your Compose library versions by specifying only the BOM's version"*; *"the BOM doesn't
     actually add those Compose libraries to your app"*; *"you must declare each library as a separate
     dependency line"*. **The shape of `[dependencies.rebel] version` with `packages = [...]`**, one
     version and a declared subset, reached first. **One difference**: under Compose's BOM, *"Going forward,
     Compose libraries will be versioned independently ... The latest stable releases of each library are
     tested together"*; the BOM maps one number onto many, where Rebel keeps one number for all.
  2. **A list of parts inside one package's entry, and its weakness** (`verified`, Cargo,
     https://doc.rust-lang.org/cargo/reference/features.html): *"Cargo "features" provide a mechanism to
     express conditional compilation and optional dependencies"*; *"Features for dependencies can be enabled
     in the dependency declaration in Cargo.toml"*. And: *"When a dependency is used by multiple packages,
     Cargo will use the union of all features enabled on that dependency when building it"*, so *"features
     should be additive"*. **What a program lists is not then what it gets**: the page does not say a crate
     may compile only because another enabled a feature (that is an inference, the fetch noted it as one);
     the compiler-engineer's prototype, which judges each program's own `use` lines against its own list
     whatever the union placed (`compiler-engineer.md:155-158`), is the guard against that reading.
  3. **Placement computed from imports** (`verified`). Go (https://go.dev/ref/mod, Vendoring): *"go mod
     vendor ... constructs a directory named vendor ... containing copies of all packages needed to build
     and test packages in the main module"*; *"If go.mod has changed since vendor/modules.txt was
     generated, the go command will report an error. go mod vendor should be run again to update the vendor
     directory."* A module at one version, placed package by package by what imports reach, and a placement
     that is a function of when the command last ran: **panel 250's M and its order finding, reached
     first**. What a new import missing from `vendor` gives is not on the page (`unverified`). Boost's `bcp`
     (https://raw.githubusercontent.com/boostorg/bcp/develop/doc/bcp.qbk): *"The bcp utility is a tool for
     extracting subsets of Boost"*; *"C++ source files are scanned for #includes"*, recursively; its
     caveats, *"bcp doesn't know what compiler you're using, so it follows all possible preprocessor
     paths"*, a *"fat"* list, and a scan that *"may not contain all the headers needed"* when a compiler
     switch changes. **Both of M's failure modes, over-placing and under-placing, in one tool's manual.**
  4. **Boost, one release number over many repositories** (`verified`,
     https://www.boost.org/releases/1.56.0/, *"August 7th, 2014"*): *"Boost version control has migrated to
     a system using git submodules"*; *"This shouldn't make too much of a difference to users"*; *"All
     header paths should remain the same."* The distribution kept its one number (1.56.0) over modular
     repositories. Whether individual Boost libraries are released apart from the distribution is on a
     Trac page the search quoted and this seat did not read (`unverified`).
  5. **The opposite choice: a version per package** (`verified`). Deno (https://deno.com/blog/std-on-jsr,
     2024-05-14): *"By adopting independent versioning for each package, we can stabilize mature packages
     promptly"*. Its cost, said inside Deno's tracker (https://github.com/denoland/deno_std/issues/4600,
     *"The Road to Stabilizing the Standard Library"*): KnorpelSenf, 2024-05-12, *"it adds a fair bit of
     accidental complexity to let the combinations"* of modules and versions *"explode like that"*; kt3k,
     2024-05-21, *"right now we need to split it into pieces to make the stabilization process going
     forward in a reasonable manner"*, a single version left for later. The request for one package over
     all of std (https://github.com/denoland/std/issues/4327, dsherret, 2024-02-13, *"JSR package for
     depending on all of std?"*): jtoppine, *"Higher amount of manual update work would possibly discourage
     the practice"*; iuioiua, 2024-04-12, *"Packages within the Standard Library will have separate
     versions, so there's no manageable way to have this done"*; **still open** at its last comment,
     2025-09-11. AWS SDK for Go v2 (https://aws.amazon.com/about-aws/whats-new/2021/01/aws-sdk-for-go-version-2-now-generally-available/,
     *"Jan 19, 2021"*): a *"modular architecture that allows customers to model service dependencies in
     their application"* and *"independently control service client updates using Go modules"*.
  6. **A diamond inside one vendor's family that drove it to one version**: not found. Angular's releases
     page (https://angular.dev/reference/releases) says only *"As of Angular version 7, the major versions
     of Angular core and the CLI are aligned"* because *"the version of @angular/core and the CLI need to be
     the same"*; a Babel 7 statement on fixed versioning was searched for and not found. The author's
     reason (item 35, *"the diamond conflicts of crates and npm"*) therefore stands on no page this seat
     read.

- `argument`: One version over many libraries, each placed by a line the program writes, is Maven's and
  Compose's bill of materials: the BOM *"doesn't actually add those Compose libraries"*, and *"you must
  declare each library as a separate dependency line"*. Cargo's features are the same list inside one
  entry, with one recorded weakness: the build takes *"the union of all features enabled"*, which the
  compiler-engineer's per-program check answers. Placement computed from imports (Go's `go mod vendor`,
  Boost's `bcp`) records 250's findings: stale until re-run, *"fat"*, or missing headers. Independent
  versions cost Deno combinations its own contributors called accidental complexity, and a request for one
  version stays open there.

- `condition`: a project that placed a subset by a declared list and recorded the list drifting from what
  the imports needed as its chief failure (that would argue for checking the list against the `use` lines
  at `add` or `fetch`, not only at build); or a record that one version over many packages stalled a
  release train (Boost's distribution, if its record says so).

- **Candidate `## Prior art` lines**:
  - `- **Reached first by Jetpack Compose**: [Use a Bill of Materials](https://developer.android.com/develop/ui/compose/bom): "The Compose Bill of Materials (BOM) lets you manage all of your Compose library versions by specifying only the BOM's version"; "the BOM doesn't actually add those Compose libraries to your app"; "you must declare each library as a separate dependency line".`
  - `- **Reached first by Maven**: [Introduction to the Dependency Mechanism](https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html): a bill of materials "defines the versions of all the artifacts that will be created in the library."`
  - `- **Reached first by Cargo**: [Features](https://doc.rust-lang.org/cargo/reference/features.html): "Features for dependencies can be enabled in the dependency declaration in Cargo.toml"; "Cargo will use the union of all features enabled on that dependency when building it."`
  - `- **Taken from Go** (only if M' is refused on it): [Go Modules Reference, Vendoring](https://go.dev/ref/mod): "If go.mod has changed since vendor/modules.txt was generated, the go command will report an error. go mod vendor should be run again to update the vendor directory."`
  - `- **Taken from Boost** (only if M' is refused on it): [bcp](https://raw.githubusercontent.com/boostorg/bcp/develop/doc/bcp.qbk): "bcp doesn't know what compiler you're using, so it follows all possible preprocessor paths."`

- `search`: WebFetch of the pages cited above, and of boost.org's bcp pages (`latest` and `1_86_0`, both
  rendered as navigation only, so the source `bcp.qbk` on GitHub was read instead), the AWS SDK for Go v2
  README (no word on modules), its GA blog post (no body returned) and migration guide (404). WebSearch:
  *Boost modular git repositories first release modularized 1.56*; *AWS SDK for Go v2 "modularized"
  service clients*; *Babel 7 "fixed" versioning all @babel packages same version*. **Not read**: Qt's
  module versioning, Spring Boot's BOM (Maven's page read in its place), npm's or crates.io's records of a
  diamond conflict inside one family, tokio's feature list.
