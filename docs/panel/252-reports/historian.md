# Panel 252, the historian's report (advisory)

Written as I go, from 2026-11-19 (the session's date; this seat has no shell, so it read no clock and ran no
command but the web tools and file reads). The order of reading was `252-briefs/00-shared.md` whole, its
§ Repairs after the critic's first reading, then `252-briefs/historian.md`, `.claude/agents/historian.md:40-49`
(the output structure), then the sittings the brief names: panel 031 (`:95-209`), panel 135 (`:80-224`),
panel 029 (`:100-129`), panel 061 (`:270-289`), panel 035 (`:165-184`), panel 121 (`:40-59`, `:140-159`,
`:255-294`), panel 203's historian report (whole), design.md `:1745-1774` and `:1965-2023`. The
compiler-engineer's and spec-warden's reports in `252-reports/` were read for what they say about other
languages (§ Other seats' claims).

I have no shell: **nothing here was compiled or run by me**. Where a sentence says what another
language's compiler does with a program, it is the page's statement, quoted, or it is marked **unrun**.
Every page below was fetched or searched on **2026-11-19**. Pages went through the fetch tool's
summarising model (except the Oberon-07 report, read as page images), so a quotation should be re-read at
its source before it enters a `## Prior art` line. *Heroes of code* was not consulted and nothing below
rests on it.

**Where fetched pages landed, for the coordinator to remove**: one file outside the repository's root.
Fetching Wirth's Oberon-07 report (`https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf`) saved the
PDF to
`/Users/joseph/.claude/projects/-Users-joseph-Temp-heroes-heroes-lang/9d1670cb-db5d-4975-9d5b-1db104217e45/tool-results/webfetch-1795104601865-kkk1yg.pdf`
(68.6 KB), written by the fetch tool, not by me. I read it from there and wrote nothing else.

**Links to another project's GitHub issue or pull request** are written in the candidate `## Prior art`
lines through `redirect.github.com`, as `.claude/rules/records.md` § The issues asks; in the precedents they
are the plain URL I read.

## Verdicts at a glance (advisory; the reasons are in each block)

| question | approve | object |
|---|---|---|
| Q1 | **Q1-R**, the departure from Go named, with Go's main reason (entanglement) weighed by the sitting, not retired by the census | Q1-C on the record panel 031 kept, which quoted Go's lesser reason. Q1-R2 has the most precedent and Go declined it; Q1-A has precedent and a known documentation cost. Neither is objected to. |
| Q2 | **Q2-R** | **Q2-C**: it writes into the specification what Rust removed as *"accidental"* in 2026 |
| Q3 | **Q3-R as the direction** precedent points; Go's declaration-time rule for the nested shape | none on precedent. Principle 0's ticket is not this seat's |
| Q4 | **Q4-C** | Q4-R and Q4-R2: no compiler I read refuses a byte where it becomes text; the trap is closed elsewhere at the index |
| Q5 | **Q5-R** with Java's range refusal; **Q5-U** | Q5-P; Q5-C's *"no exponent"* half |

## Premises of the briefs found false or overstated

1. **design.md §4.12's historical note** (`docs/design.md:2022-2023`) says: *"Go shipped without user
   generics for a decade precisely because the three built-in containers covered most needs."* **Go's FAQ
   gives a different reason.**
   - The FAQ in Go's tree at tag `go1.12` (https://go.googlesource.com/go/+/refs/tags/go1.12/doc/go_faq.html,
     read through the fetch tool's summary):
     - The reason: generics add complexity to the type system and run time, and the team had not found a
       design whose value justified it.
     - Built-in maps and slices *with the empty interface* let programmers write many of the same things,
       *"less smoothly"*. That is a "meanwhile" mitigation, not the reason.
   - Today's FAQ (https://go.dev/doc/faq, *"Why was Go initially released without generic types?"*):
     *"Polymorphic programming did not seem essential to the language's goals at the time, and so was
     initially left out for simplicity."* It does not mention the containers at all.
   - The note's *"~90% of real generic use"* has no source I could find.
   - It is for the sitting to correct beneath, with its date, if it leans on the note.
2. **The historian brief, Q4**: *"where Python 2's `"use"[0]` was `"u"`"*.
   - In Python 3 `"use"[0]` is also a one-character string (*"indexing a string produces strings of length
     1"*, https://docs.python.org/3/library/stdtypes.html).
   - The change is that Python 3's **`bytes`** index gives an `int`. The docs' own sentence on that was past
     the fetched text, so that half is **unverified at the source**.
   - The brief's `f"{b"use"[0]}"` reuses the outer quote inside a hole, which parses only from Python 3.12
     (PEP 701, read for panel 121, not re-read today: **carried**).
3. **The historian brief, Q3**: *"Go shipped neither for a decade and both together in 1.18"*. True of
   generic functions and types. **But Go 1.18 refused generic methods, and Go 1.27 (August 2026) added
   them** (§ Question 3, precedent 3), so Go's own history is one of half generics widened twice.
4. **Panel 031's R6** cites Go's design paper for *"the cost it names (incremental builds)"*. The paper
   names that cost first, and then a cost it calls *"More important"*: entanglement
   (§ Question 1, precedent 2). **Incomplete, not false.**
5. **The historian brief, Q5**: *"Go's `strconv.FormatFloat` ... JSON's number grammar: does each promise
   that what it prints is a literal of the language"*. JSON prints nothing. Its grammar admits `1e-06` and
   excludes `Infinity` and `NaN`, which is the spec-warden's `inf`/`nan` point arrived at from the other
   side (§ Question 5, precedent 4). Not false: a question that needed its frame turned.

---

## Question 1: a private module that cannot use its own face

### verdict

**approve (advisory) Q1-R over Q1-C**, with the departure from Go written down as deliberate, and with
one correction to the record it rests on: **panel 031's R6 recorded Go's lesser reason, not its main
one.** The compiler-engineer's census answers the cache reason. Nothing a census can run answers the
entanglement reason. The precedent is densest on **Q1-R2's line**: cycles inside the unit that ships
together, refused between units. Go proposed that exact line in 2019 and declined it. **Q1-A**: the
precedent is strong, and Go wrote down its documentation cost. **Q1-C**: Go's choice, kept for a reason
the sitting has not weighed.

### precedents

1. **Go refuses every import cycle, in the specification.** *"It is illegal for a package to import
   itself, directly or indirectly, or to directly import a package without referring to any of its
   exported identifiers."* (The Go Programming Language Specification, § Import declarations,
   https://go.dev/ref/spec). **verified**.
2. **Go's reasons, in its design paper** (Rob Pike, *Go at Google: Language Design in the Service of
   Software Engineering*, SPLASH 2012 keynote of 2012-10-25, § 7 *Dependencies in Go*,
   https://go.dev/talks/2012/splash.article). **verified**:
   - *"The language defines that there can be no circular imports in the graph, and the compiler and
     linker both check that they do not exist."*
   - *"They require the compiler to deal with larger sets of source files all at once, which slows down
     incremental builds."*
   - Then: *"More important, when allowed, in our experience such imports end up entangling huge swaths
     of the source tree into large subpieces that are difficult to manage independently, bloating
     binaries and complicating initialization, testing, refactoring, releasing, and other tasks"*.
   - *"The lack of circular imports causes occasional annoyance but keeps the tree clean, forcing a clear
     demarcation between packages."*
3. **Go declined Q1-R in 2019** (golang/go #30247, *"proposal: Go 2: allow import cycle"*, opened and
   closed 2019-02-15, https://github.com/golang/go/issues/30247). **verified**.
   - Ian Lance Taylor: *"Sorry, this kind of change simply isn't going to happen."*
   - Rob Pike gave the main argument the same day: the lack of cycles *"forces programmers to think more
     about their dependencies"*; *"Import cycles can be convenient but their cost can be catastrophic.
     They should continue to be disallowed."*
4. **Go declined Q1-R2's exact shape three months later** (golang/go #31833, *"proposal: allow cyclic
   imports within one module (or within the same direct parent package)"*, opened 2019-05-04, closed
   2019-05-28, https://github.com/golang/go/issues/31833). **verified**.
   - The proposal: *"we can limit cyclic imports within one module, or even more limited, within the same
     direct parent package (and one module)."*
   - Robert Griesemer, closing it: *"Sorry, but no. We have deliberately chosen to disallow import cycles
     both for human understanding and implementation simplicity."*
   - The proposer then asked: *"Is there a plan to solve the bloating package problem?"* The page shows
     no reply. That is the question Rebel's `sql/build` and `math/linear` raise (`00-shared.md`
     § Question 1).
5. **Go's `internal` rule does not itself refuse the ring.** *"a package `.../a/b/c/internal/d/e/f` can be
   imported only by code in the directory tree rooted at `.../a/b/c`"* (Go 1.4 release notes, *Internal
   packages*, https://go.dev/doc/go1.4). **verified**. So `a/internal/b` may import `a`. The ring is
   refused by the cycle rule (1), as in Heroes, where the spec-warden measured the same (`sw/probes/q1-sibling`).
6. **Go named Q1-A's use in 2016, in the general alias proposal that was held back.** Proposal 16339,
   *Alias declarations for Go* (Griesemer and Pike, last updated 2016-07-18,
   https://go.googlesource.com/proposal/+/refs/heads/master/design/16339-alias-decls.md). **verified**:
   - § Other use cases: *"Alias declarations facilitate the construction of larger-scale libraries or
     "components"."*
   - It says aliases let a top-level package *"pull out"* the relevant declarations from its sub-packages,
     and *"The other packages can be organized in an "internal" sub-directory"* (the surrounding sentence
     paraphrased by the fetch tool).
   - § Motivation: *"There is no work-around for types, nor is possible to define a named type T in L1 and
     re-export it in L"*.
7. **Go 1.9 kept that use for type aliases.**
   - The design that shipped, 18130 (Cox and Griesemer, last updated 2016-12-16,
     https://go.googlesource.com/proposal/+/master/design/18130-type-alias.md), names *"allowing large
     packages to be split into multiple implementation packages with a single top-level exported API"*
     beside the primary use, *"moving a type from one package to another"*. It calls itself *"a
     replacement for the generalized alias proposal originally targeted for, but held back from, Go
     1.8."* **verified**.
   - Go 1.9's notes: *"Go now supports type aliases to support gradual code repair while moving a type
     between packages"*; *"both `T1` and `T2` denote the same type"* (https://go.dev/doc/go1.9).
     **verified**.
   - Panel 135's historian had already read 1.9's warrant as refactoring (`docs/panel/135-*.md:90`, `:136`).
     That reading stands. The top-level API split is a second, named use, not the warrant.
8. **Q1-A's recorded cost in Go: the documentation points into a package the reader cannot import.**
   golang/go #44905 (*"proposal: go/doc: inline type alias documentation"*, opened 2021-03-10, declined
   2021-04-28, https://github.com/golang/go/issues/44905). **verified**:
   - Readers of an alias to an internal type must move between packages to read its fields and methods,
     with no link back from the internal type to the alias.
   - Russ Cox declined the request: inlining is not always an improvement, needs qualified names, and
     `go doc` does not track dependency type information.
9. **Rust: `pub use` re-exports, even from a private path; a cycle of re-exports is an error.** The Rust
   Reference, *Use declarations* § `use` Visibility (https://doc.rust-lang.org/reference/items/use-declarations.html).
   **verified**:
   - *"Such a `use` declaration serves to re-export a name. A public `use` declaration can therefore
     redirect some public name to a different target definition: even a definition with a private
     canonical path, inside a different module."*
   - *"If a sequence of such redirections form a cycle or cannot be resolved unambiguously, they represent
     a compile-time error."*
10. **Rust: modules inside a crate may form a cycle; packages may not.**
    - Roman Kashitsyn, *Rust at scale: packages, crates, and modules* (2022-01-20, updated 2023-06-16,
      https://mmapped.blog/posts/03-rust-packages-crates-modules), a secondary source: *"modules in the
      same crate can form a dependency cycle"* and *"In contrast, the package dependency graph must be
      acyclic."* His fix: *"move common definitions into a new package or merge some packages."*
      **verified (secondary)**.
    - Cargo's message, quoted in a users' forum thread of 2020-11-12: *"error: cyclic package dependency:
      package `kernel v0.1.0 (...)` depends on itself."* The answers recommend extracting an interface crate
      (https://users.rust-lang.org/t/how-to-resolve-cyclic-dependency/51387). **verified (forum)**.
    - **What the Rust Reference itself says of module cycles: not found** (§ search).
11. **Java: circular references across compilation units are the rule; a module's cycle is an error.**
    JLS SE 25, chapter 7 (https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html). **verified**:
    - § 7.3: *"Classes and interfaces declared in different ordinary compilation units can refer to each
      other, circularly. A Java compiler must arrange to compile all such classes and interfaces at the same
      time."*
    - § 7.7.1: resolution fails, a compile-time error, when *"the current module directly or indirectly
      expresses a dependence on itself."*
    - The chapter states no prohibition on cycles among packages.
12. **Haskell: the report allows mutually recursive modules; GHC makes the programmer break each cycle.**
    - Haskell 2010 Report, chapter 5: *"Modules may be mutually recursive."* and *"Explicit type
      signatures for all exported values may be necessary to deal with mutual recursion."*
      (https://www.haskell.org/onlinereport/haskell2010/haskellch5.html). **verified**.
    - GHC User's Guide § 5.8.10, *Mutually recursive modules and hs-boot files*: *"Every cycle in the
      module import graph must be broken by a `hs-boot` file."*
      (https://downloads.haskell.org/ghc/latest/docs/users_guide/separate_compilation.html). **verified**.
    - The page does not say why GHC needs it.
13. **Python allows cycles at run time, and whether a cycle works depends on the form of the import.**
    The Programming FAQ quotes Gordon McMillan (https://docs.python.org/3/faq/programming.html).
    **verified**:
    - *"Circular imports are fine where both modules use the "import <module>" form of import. They fail
      when the 2nd module wants to grab a name out of the first ("from module import name") and the import
      is at the top level."*
    - It also records Guido van Rossum's advice to avoid `from <module> import ...`.
    - Python 3.7 relaxed one case: *"Circular imports involving absolute imports with binding a submodule
      to a name are now supported."* (What's New in Python 3.7, bpo-30024,
      https://docs.python.org/3/whatsnew/3.7.html). **verified**.
14. **Nim: cycles allowed; the unannotated form now deprecated for an explicit, checked one.** The Nim
    Manual, version 2.4.2 (header read today), § Modules, *Cyclic imports*
    (https://nim-lang.org/docs/manual.html). **verified**:
    - *"Recursive module dependencies are allowed, but are slightly subtle unless they are declared as
      cyclic imports."*
    - An import may carry a `cyclic` pragma. The modules connected by such imports *"form a cycle group
      that is checked together, phase by phase"*: imports, type sections, leading declarations, then the
      remaining statements.
    - *"An import cycle without a cyclic import is deprecated"*. With `--warning:ImplicitCyclicImport:on`
      the compiler lists the cycle's modules.
    - **Which release introduced `cyclic`: unverified.** A changelog search found nothing.
    - nim-lang/RFCs #6 (*"Cyclic imports and symbol dependencies"*, opened 2016-03-11, open,
      https://github.com/nim-lang/RFCs/issues/6) holds Araq, 2021-02-22: *"Now that IC is slowly beginning
      to work, we are looking into how to support recursive modules."* **verified**.
15. **Swift: inside a module every file sees every other; between packages, a cycle was refused and then
    relaxed in 2024.**
    - TSPL, *Access Control*: *"A module is a single unit of code distribution"*; *"Internal access
      enables entities to be used within any source file from their defining module, but not in any
      source file outside of that module."*
      (https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/AccessControl.md).
      **verified**.
    - swift-package-manager PR #7530 (*"Allow cyclic package dependencies if they don't introduce a cycle
      in a build graph"*, opened 2024-05-06, merged 2024-05-08,
      https://github.com/swiftlang/swift-package-manager/pull/7530): *"It should be possible for packages to
      depend on each other if such dependence doesn't introduce cycles in the build graph."* It also
      *"Adds detection of target cycles across packages"*. **verified**.
    - The follow-up limiting it to manifests of tools version 6.0 or later (#7579) is **unverified**: I
      have only the fetch tool's summary of a commit text, not the page.
16. **Ada 2005 added a form to allow mutually dependent packages.** Ada 2012 Reference Manual 10.1.2(31/2):
    *"The limited_with_clause may be used to support mutually dependent abstractions that are split across
    multiple packages."* (https://docs.adacore.com/live/wave/arm12/html/arm12/arm12-10-1-2.html). **verified**.
    That it entered in Ada 2005 is **unverified**: a search summary of a GNAT paper says so, and I did not
    read the paper.
17. **Delphi: circular unit references allowed only through an implementation-section `uses`.** Error
    F2047 (*"Circular unit reference"*). **unverified**: Embarcadero's pages returned 403. Only a search
    summary describes them.
18. **Oberon-07's report says nothing about import cycles.** The report is Wirth, *The Programming
    Language Oberon*, revision 1.10.2013 / 3.5.2016, the PDF above. **verified** (read as page images):
    - § 11 *Modules* (p. 14) defines `ImportList` and says what an import makes visible, and nothing of
      cycles.
    - Its introduction: *"What remains unsaid is mostly left so intentionally, either because it is
      derivable from stated rules of the language, or because it would unnecessarily restrict the freedom
      of implementors."*
    - Whether Oberon compilers refuse a cycle: **unverified**. The BlackBox archive's *"RE: Cyclic
      Imports?"* (2001) returned 403.
19. **JavaScript re-exports through a "barrel module".** MDN, *export*: *"A module can also "relay" values
    exported from other modules ... (usually called a "barrel module")"*, through `export { ... } from`
    (https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/export). **verified**.
    The page does not state that a re-exported binding is the same binding, except *"If both resolve to the
    same binding, there is no ambiguity"*.
20. **OCaml: link order.** The manual: *"a given x.cmx file must come before all .cmx files that refer to
    the unit x"*, and *"compilation units are initialized in that order at run-time"*
    (https://ocaml.org/manual/native.html). **verified**. That this refuses cycles between compilation
    units is **my inference**: the page *"says nothing about cyclic references between compilation units"*.

### argument (Q1, 119 words)

Every language I read draws the line at the unit that ships together. Rust's modules within a crate,
Java's classes, Swift's files and Haskell's modules may cycle. Cargo's packages, Java's modules and
Swift's targets may not. Go refuses everywhere, and in 2019 it declined both Q1-R and Q1-R2's exact
shape, *"for human understanding and implementation simplicity"*. Panel 031's R6 quoted Go's
incremental-build reason. The paper's *"more important"* reason was entanglement and bloated
packages, which is Rebel's `sql/build` question. No census can retire that reason. Only the sitting can
judge it, so the departure should be named. Nim kept cycles but made them declared and checked by
phase. SwiftPM relaxed package cycles in 2024. Q1-A has Rust's and Go's precedent, and Go's
documentation cost.

### condition (Q1)

My reading moves on any of these:

- **Toward Q1-C**: a language that allowed cycles and later refused them for a measured cost. I found
  none. Nim's deprecation is of the *unannotated* cycle, replaced by a declared one, not a refusal.
- **Toward Q1-C**: a record of a codebase entangled by allowed cycles, with numbers. I found Pike's
  *"in our experience"* only.
- **Toward Q1-R2**: the compiler-engineer's untried two-package ring (`ce` § What I cut) failing to
  place or build. That would make Cargo's, JPMS's and SwiftPM's target line the Heroes line too.
- **Against Q1-A**: a language with a transparent re-export that removed it. Panel 135 already searched
  for a shipped language that removed its transparent synonym and found none (`docs/panel/135-*.md:90`).

### search (Q1)

- **Queries**:
  - `Go language specification import cycle "illegal for a package to import itself"`
  - `Go standard library type alias re-export type from internal package "= internal" exported alias`
  - `Cargo "cyclic package dependency" error ...`
  - `Rust reference modules within a crate ... circular use ...`
  - `OCaml manual compilation units circular dependency ...`
  - `OCaml manual "module rec" ... "safe module" ...`
  - `Python "partially initialized module" circular import ...`
  - `JLS 7.7.1 module declaration "dependence on itself ..."`
  - `Ada 2005 "limited with" rationale ...`
  - `Swift Package Manager "cyclic dependency declaration found" ...`
  - `Nim "cyclic" pragma import "cycle group" changelog ...`
  - `Oberon-07 report Wirth module import "cyclic" OR "circular" ...`
  - `Modula-2 Wirth "definition modules" circular import ...`
  - `Delphi "Circular unit reference" error implementation section ...`
  - `golang/go proposal allow import cycles ... declined ...`
- **Pages read**: those cited above.
- **Unreadable** (403, or not readable as text):
  - Embarcadero's F2047 and *Programs and Units*;
  - the BlackBox 2001 archive;
  - Scribd's Oberon-07 copy (the ETH PDF was read instead).
- **Not found, and where it was looked for**:
  - a Go standard-library package that re-exports an `internal` package's type by alias. Looked for in
    the search above and in #44905, whose examples are `ent` and a hypothetical one. **A question, not a
    finding that none exists.**
  - the Rust Reference's own sentence on module cycles inside a crate. Looked for in the use-declarations
    page and by search.
  - a Modula-2 report's rule on circular imports. One search; the PIM text was not reached.
  - the Nim release that introduced `cyclic`. Looked for in a changelog search and the RFC thread.
- Panel 135's Go alias history (`docs/panel/135-*.md:90`, `:135-142`) was not re-searched beyond what
  Q1-A asks.

### candidate `## Prior art` lines (Q1)

Each line names the resolution it would serve. Only the adopted resolution's lines are used.

- Q1-C: `- **Reached first by Go**: [The Go Programming Language Specification, Import declarations](https://go.dev/ref/spec): "It is illegal for a package to import itself, directly or indirectly".`
- Q1-C: `- **Reached first by Go**: [Go at Google: Language Design in the Service of Software Engineering, § 7 (Pike, 2012)](https://go.dev/talks/2012/splash.article): the language defines that there can be no circular imports, and the compiler and linker both check that they do not exist.`
- Q1-R: `- **Reached first by Java**: [JLS SE 25 § 7.3](https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html): classes and interfaces declared in different ordinary compilation units can refer to each other circularly, and the compiler compiles them at the same time.`
- Q1-R: `- **Reached first by Haskell**: [Haskell 2010 Report, chapter 5](https://www.haskell.org/onlinereport/haskell2010/haskellch5.html): modules may be mutually recursive.`
- Q1-R: `- **Reached first by Nim**: [Nim Manual 2.4.2, Cyclic imports](https://nim-lang.org/docs/manual.html): recursive module dependencies are allowed; modules connected by imports marked `cyclic` form a cycle group checked together, phase by phase.`
- Q1-R2: `- **Reached first by Java**: [JLS SE 25 §§ 7.3, 7.7.1](https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html): classes in different compilation units may refer to each other circularly, while a module that directly or indirectly expresses a dependence on itself is a compile-time error.`
- Q1-A: `- **Reached first by Rust**: [The Rust Reference, Use declarations](https://doc.rust-lang.org/reference/items/use-declarations.html): a public use declaration re-exports a name, even a definition with a private canonical path inside a different module.`
- Q1-A: `- **Reached first by Go**: [Proposal 16339, Alias declarations for Go (Griesemer and Pike, 2016)](https://go.googlesource.com/proposal/+/refs/heads/master/design/16339-alias-decls.md): alias declarations let a top-level package present a component's public API while the other packages sit in an "internal" sub-directory.`

**Declined proposals are not prior art** in either allowed form. Go's #31833 is evidence for the sitting's
file, not an issue line.

---

## Question 2: a type parameter not taken from a map literal's key

### verdict

**approve (advisory) Q2-R; object (advisory) to Q2-C.** Rust's compiler had this shape: a generic call's
letter resolved from an array literal's first element. It was ruled an accident and repaired by a final
comment period merged on 2026-01-30. Every other language I read infers from every element.

### precedents

1. **Rust fixed this shape in 2026 and called it an accident.**
   - Issue #136420 (*"Rust fails to coerce to a fn pointer when passing an array as an argument to a
     generic function."*, opened 2025-02-02 by theemathas, closed 2026-01-30,
     https://github.com/rust-lang/rust/issues/136420). **verified**:
     - Of six lines, *"only the `f([foo, bar])` line fails"*, where `fn f<T>(_: [T; 2])`. The same pair
       passes as `vec![foo, bar]` to `e<T>(_: Vec<T>)`, and in `if`/`else`.
     - The assigned contributor, adwinwhite: *"`f`'s type parameter is resolved during the coercion of the
       first element, rather than based on the final type of the array."*
   - The repair, PR #140283, *"Fix accidental type inference in array coercion"*. Merge commit
     `35a31ba763976907cd38ba88743a3aefbaf8ffff`, 2026-01-29 21:36 UTC
     (https://rust.googlesource.com/rust/+/35a31ba763976907cd38ba88743a3aefbaf8ffff; the PR page,
     https://github.com/rust-lang/rust/pull/140283). **verified**:
     - *"If the expectation of array element is a type variable, we should avoid resolving it to the first
       element's type and wait until LUB coercion is completed."*
     - oli-obk proposed the FCP on 2025-06-03; the text reads *"we typeck the array expression normally,
       lubbing the element expressions"*.
     - The commit says much code depends on the first element's type as the expectation elsewhere, so the
       `if`/`match` behaviour could not simply be copied to arrays.
   - What stays open: issue #145048 (*"Nested coercion in array results in type mismatch, inconsistent with
     `if` and `match`."*, opened 2025-08-07, open, https://github.com/rust-lang/rust/issues/145048).
     **verified**: `check_expr_array` *"uses a type variable as the expectation during element typecking"*,
     but *"the type variable is unified with the first element's type during coercion."*
   - **That nested case is the compiler-engineer's `[1, b]` / `[b, 1]` shape in another type system**
     (`252-reports/compiler-engineer.md` § Q2, `x07`/`x08`). That likeness is my reading.
2. **C# 12 infers from each element of a collection expression.** *Collection expressions*, § Type
   inference (https://github.com/dotnet/csharplang/blob/main/proposals/csharp-12.0/collection-expressions.md).
   **verified**:
   - *"for each Eᵢ: If Eᵢ is an expression element, then an input type inference is made from Eᵢ to Tₑ"*,
     and likewise an output type inference.
   - *"The empty literal `[]` has no type."*
   - Its example `AsListOfArray([[4, 5], []])` gives `List<int[]>`. The reversed order is not shown: what
     C# does with `[[], [4, 5]]` is **unrun**. By the rule as written, no element is singled out (my
     reading).
3. **C++ deduces from every element of a braced list, and fails on a conflict.** cppreference, *Template
   argument deduction* (https://en.cppreference.com/w/cpp/language/template_argument_deduction.html).
   **verified**:
   - *"deduction is performed for every element of the initializer list"*.
   - *"if different pairs yield different deduced template arguments ... compilation fails"*.
   - The example: `f({1, "abc"})` fails as ambiguous.
4. **TypeScript computes the element type from every element.** Handbook, *Type Inference*, § Best common
   type (https://www.typescriptlang.org/docs/handbook/type-inference.html). **verified**:
   - *"To infer the type of `x` in the example above, we must consider the type of each array element."*
   - *"The best common type algorithm considers each candidate type, and picks the type that is compatible
     with all the other candidates."*
5. **Kotlin**: *"Type inference is a type constraint problem, and is usually solved by a type constraint
   solver."* (Kotlin specification, *Type inference*, https://kotlinlang.org/spec/type-inference.html).
   **verified**. The page does not say how a generic call's arguments combine, nor whether their order
   matters. What Kotlin does with `mapOf("a" to emptyList())` passed to `<K> count(g: Map<K, List<K>>)`
   is **unrun**.
6. **Swift**: `docs/TypeChecker.md` (https://github.com/swiftlang/swift/blob/main/docs/TypeChecker.md)
   gives a literal *"a fresh type variable"* and a literal constraint. It does not describe array or
   dictionary literals element by element. **verified** (that the page is silent). Swift's verdict on
   `["a": []]` passed to `[K: [K]]` is **unrun**.
7. **Panel 203's historian** read Pierce and Turner's warning on order: Cardelli's greedy algorithm's
   instantiations *"are highly sensitive to the precise order in which constraints are encountered"*
   (`docs/panel/203-reports/historian.md:30`). **carried, not re-read today**.

### argument (Q2, 89 words)

The only shipped compiler I found that typed a literal's letter from its first element is rustc. Its
own repair, by a final comment period, calls that *"accidental type inference"* and replaces it by
inference over the whole literal. C#, C++ and TypeScript state rules that read every element. C++ fails
on a conflict instead of choosing the first. Q2-C would write into Heroes' specification the behaviour
Rust's type team removed as an accident. Rust's open #145048 shows that the nested case stays hard,
which is the compiler-engineer's unbuilt `x07`.

### condition (Q2)

- **Toward Q2-C**: a language that specifies first-element typing on purpose, in its specification. I
  found none.
- **Against the advice**: Rust's later regressions from #140283, if a crater run or a revert shows the
  order-free route broke correct programs at scale. The merged PR names inference regressions its author
  could not fix (*"I can't fix the inference regressions for now"*); their size is **unread**. The
  compiler-engineer's census (149 roots, no Q2 move outside two `fixedbugs`) is the Heroes answer to the
  same question.

### search (Q2)

- **Queries**:
  - `rust array literal coercion depends on order of elements first element Box dyn Trait ...`
  - `Kotlin specification type inference constraint system ...`
- **Fetched**: TypeScript's handbook, C#'s collection-expressions proposal, cppreference, Swift's
  `TypeChecker.md`, Kotlin's specification, and rust-lang/rust #136420, #140283 and #145048 (with the
  merge commit).
- **Not done**: Java's JLS 18 was not re-read. Panel 203 covered JEP 101 and 215 for argument-position
  inference, not literals.
- **Not found**: a language whose specification makes a literal's type its first element's. Looked for in
  the pages above. The Rust forum statement that *"type inference looks at the first element of the
  Vec"* (search summary, `users.rust-lang.org/t/.../93849`) is about the same rustc behaviour and was not
  read.

### candidate `## Prior art` lines (Q2)

- Q2-R: `- **Reached first by Rust**: [rust-lang/rust PR 140283, "Fix accidental type inference in array coercion"](https://redirect.github.com/rust-lang/rust/pull/140283): if the expectation of an array element is a type variable, it is not resolved to the first element's type, and the compiler waits until the coercion of every element is completed.`
- Q2-R: `- **Reached first by C#**: [C# 12 collection expressions, Type inference](https://redirect.github.com/dotnet/csharplang/blob/main/proposals/csharp-12.0/collection-expressions.md): for each element of a collection expression, a type inference is made from that element to the element type.`
- Q2-R: `- **Reached first by C++**: [cppreference, Template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction.html): deduction is performed for every element of a braced initializer list, and compilation fails when two elements deduce different arguments.`

(The csharplang link is a file, not an issue or pull request. `redirect.github.com` is used for uniformity
and can be dropped.)

---

## Question 3: records with no type parameter

### verdict

**approve (advisory) Q3-R as the direction precedent points; no position on Principle 0's ticket, which
is not this seat's.** I found no shipped language that stayed with generic functions and no generic
types. Go's 2021 design rejected that split in so many words. Dart shipped the other half first and
added the missing one. Go refused generic methods in 1.18 and added them in 1.27, four years later, and
its own blog calls the function it had used meanwhile *"a workaround"*. The shape beside, `Nest<[T]>`,
has Go's precedent, refused at its declaration. Rust refuses it only at use, and an issue open since 2017
calls that a fault. Q3-C's shape also has a living precedent, Go's `container/heap`, which outlived
generics for a reason of its own.

### precedents

1. **Go's generics design rejected generic functions alone.** *Type Parameters Proposal*, Ian Lance Taylor
   and Robert Griesemer, 2021-08-20
   (https://go.googlesource.com/proposal/+/refs/heads/master/design/43651-type-parameters.md). **verified**:
   - *"We want more than just generic functions: we also want generic types."*
   - *"We suggest that types be extended to take type parameters."*
2. **Go's 2018 problem statement said the same against the opposite reduction.** *Generics — Problem
   Overview*, Russ Cox, 2018-08-27
   (https://go.googlesource.com/proposal/+/HEAD/design/go2draft-generics-overview.md). **verified**:
   - *"Polymorphic programming is about more than data containers."*
   - It describes the common view as one where *"generics means parameterized data structures, or
     containers, ignoring parameterized functions."*
3. **Go refused one half again, and widened it in 2026.**
   - The FAQ (https://go.dev/doc/faq, *"Why does Go not support methods with type parameters?"*) still
     reads, today: *"We do not anticipate that Go will ever add generic methods."* **verified**.
   - Go 1.27's release notes (https://go.dev/doc/go1.27): *"Go 1.27 now supports generic methods: a method
     declaration may declare its own type parameters."*, with *"methods of interfaces may not declare type
     parameters"*. **verified**. The specification marks the feature `[Go 1.27]` (https://go.dev/ref/spec,
     § Method declarations). **verified**.
   - The Go blog, *Generic Methods*, Mark Freeman, 2026-08-26
     (https://go.googlesource.com/website/+/refs/heads/master/_content/blog/generic-methods.md). **verified**:
     - *"Because Go 1.18 didn't support generic methods, we needed a generic function as a workaround."*
     - The earlier view: *"methods are primarily the means of implementing an interface"*. The new one:
       *"Methods enable organizing functionality around types."*
   - The FAQ and the release notes now disagree on go.dev. That is Go's record, read today, not mine to
     reconcile.
4. **Dart shipped generic classes first and added generic methods in 1.21.** *Dart 1.21: Generic Method
   Syntax*, 2016-12-07 (https://news.dartlang.org/2016/12/dart-121-generic-method-syntax.html). **verified**:
   - *"Until now, Dart's generic support was limited to classes, such as List<T>."*
   - *"Dart 1.21 introduces syntax allowing type arguments on methods and functions."*
   - *"type arguments are not reified"*.
   - (Corrected 2026-11-19, while writing: an earlier draft of this line said "five years later". That
     interval came from no page and is struck.)
5. **Zig makes a generic type a generic function.** Zig Language Reference (master), § *Generic Data
   Structures* (https://ziglang.org/documentation/master/). **verified**:
   - *"Zig uses comptime capabilities to implement generic data structures without introducing any
     special-case syntax."*
   - Of `fn List(comptime T: type) type`: *"It's a function that returns an anonymous `struct`."*
   - Zig's answer needs types as compile-time values. Heroes has none, so it is a precedent for the
     equivalence, not a route (my reading).
6. **Odin has both.** The overview's contents list *"Procedures using explicit parametric polymorphism
   (parapoly)"* and *"Data types using explicit parametric polymorphism (parapoly)"*
   (https://odin-lang.org/docs/overview/). **verified (headings only)**: the sections' bodies were past
   the fetched text.
7. **Java 5** added *"generic types and methods"* together, per JSR 14's 2003 draft as a search summary
   gives it (https://jcp.org/en/egc/view?id=14 and the academia.edu draft in the search). **unverified**:
   neither page was fetched.
8. **The nested shape, `Nest<[T]>`: Go refuses it at the declaration.** The Type Parameters Proposal
   (as 1). **verified**:
   - *"A generic type can refer to itself in cases where a type can ordinarily refer to itself, but when it
     does so the type arguments must be the type parameters, listed in the same order."* This *"prevents
     infinite recursion of type instantiation."*
   - Its example `type P[T1, T2 any] struct { F *P[T2, T1] }` is marked *"INVALID"*.
   - The authors add that *"it may be possible to relax this rule to permit some cases that use different
     type arguments."*
   - **Go's rule is stricter than the compiler-engineer's**: Go refuses any other arguments, not only
     growing ones. `ir/mono.hero:19-28`'s rule refuses growth (`252-reports/compiler-engineer.md` § Q3).
9. **Rust refuses it only at use, and the refusal has been called a fault since 2017.**
   - Error index E0320 (https://doc.rust-lang.org/error_codes/E0320.html). **verified**:
     - *"Recursion limit reached while creating drop-check rules."* Its example is
       `enum A<T> { B, C(T, Box<A<(T, T)>>) }`, refused at `A::<T>::B;`.
     - *"It is not possible to define a type with recursive drop-check rules. All such recursion must be
       removed."*
   - Issue #44933 (*"overflow during drop-check on a recursive type"*, opened 2017-09-29, open,
     https://github.com/rust-lang/rust/issues/44933). **verified**:
     - The reporter: *"If the type isn't used, the program compiles fine."*
     - Ixrec, 2017-10-15: *"It also seems bad that we don't emit this error until the type gets used
       somewhere."*
   - Panel 029's R4 read Rust's function case, *"reached the recursion limit while instantiating"*, as *"a
     warning rather than a model"* (`docs/panel/029-*.md:125-128`). **carried**.
10. **C++ bounds it by a depth limit.** GCC's `-ftemplate-depth=n`
    (https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html). **verified**:
    - *"A limit on the template instantiation depth is needed to detect endless recursions during template
      class instantiation."*
    - *"The default value is 900, as the compiler can run out of stack space before hitting 1024 in some
      situations."*
11. **MLton** (panel 029's R4): SML's lack of polymorphic recursion leaves *"only a finite number of
    instances of such types in any given program"* (`docs/panel/029-*.md:119-122`). **carried, not re-read
    today.**
12. **Q3-C's shape, a library's algorithm over the caller's own collection, is Go's `container/heap`, and
    it outlived generics.**
    - The package documentation (https://pkg.go.dev/container/heap). **verified**:
      - *"Package heap provides heap operations for any type that implements heap.Interface."*
      - *"Init is idempotent with respect to the heap invariants and may be called whenever the heap
        invariants may have been invalidated."*
    - Cox's 2018 overview (as 2): container/heap *"provides heap-maintenance algorithms as ordinary
      functions"*, and *"most programmers who want a priority queue don't want to implement the underlying
      storage for it"*. **verified**.
    - golang/go #47632 (*"proposal: container/heap: add Heap, a heap backed by a slice"*, opened
      2021-08-11, open, https://github.com/golang/go/issues/47632). **verified**:
      - The proposer's own review of real uses: the simplified generic `Heap` *"would cover fewer than half
        of those real-life use cases"*.
      - *"Many real uses of `container/heap` need the index-based methods `Fix` or `Remove`"*.
    - **A difference that matters, my reading**: Go's heap keeps no state apart from the caller's
      collection. Its invariant lives in the slice's own order. Rebel's `deque`, `cache`, `indexed` and
      `ordered` keep a shape record apart from the program's lists, and that second piece of state is what
      the compiler-engineer found mismatched in silence.
    - **What users got wrong with the caller-owned shape: no record found.** Heap's page states the caller's
      obligation (call `Init` or `Fix` after a change) and nothing about misuse.

### argument (Q3, 118 words)

Every language I read that shipped half of generics later shipped the rest. Dart's classes got methods
in 1.21. Go's functions and types came together, because *"we also want generic types"*. Go's
methods, refused in 1.18 with *"We do not anticipate"*, arrived in 1.27. Its blog calls the generic
function used meanwhile *"a workaround"*, which is the shape `containers` was written in. design.md
§4.12's Go note does not say what Go's FAQ says (§ Premises). On the nested shape Go is the model: a
refusal at the declaration. Rust's refusal at use has had an open complaint since 2017. Go's
caller-owned heap survived generics, but it keeps no state apart from the caller's slice. Rebel's
shapes do, and that state is where they break.

### condition (Q3)

- **Toward Q3-C as permanent**: a shipped language that kept generic functions without generic types
  and recorded why. I found none (§ search).
- **Toward Q3-C as permanent**: a caller-owned container design with separate state that has a record
  of working without mismatches. None found. Go's heap has no separate state.
- **Against Q3-R's nested-shape rule as the compiler-engineer wrote it**: a program that needs a non-growing
  self-reference at other arguments, which Go's stricter rule would refuse. I have no instance.

### search (Q3)

- **Queries**:
  - `Dart 1.21 generic method syntax ...`
  - `ziglang documentation "Generic Data Structures" ...`
  - `Rust error E0320 ...`
  - `golang proposal generic container/heap ...`
  - `JSR 14 "Add Generic Types To The Java Programming Language" ...`
  - `Go 1.27 generic methods proposal accepted ...`
- **Fetched**: Go's FAQ (today's and the `go1.12` tree's), the Type Parameters Proposal, the 2018 overview,
  the Go 1.27 notes and blog, Dart's announcement, Zig's reference (from offset 250,000), Odin's overview
  (first 100,000 characters), Rust's E0320 and #44933, GCC's dialect options, `container/heap` and #47632.
- **Unreadable**: `web.archive.org` (the fetch tool refuses it). The Go tree's FAQ at `go1.12` was read
  instead.
- **Not found**:
  - a shipped language with generic functions and no generic types. Looked for in Go's three design
    documents and through the brief's list (Zig, Odin, C, Java 5). C has neither, so it is no instance.
  - a record of users misusing a caller-owned container API (looked for in `container/heap`'s page and
    #47632).
- **Not read**: Haskell's nested datatypes (Bird and Meertens). **Unverified**, named only.

### candidate `## Prior art` lines (Q3)

- Q3-R: `- **Reached first by Go**: [Type Parameters Proposal (Taylor and Griesemer, 2021)](https://go.googlesource.com/proposal/+/refs/heads/master/design/43651-type-parameters.md): "We want more than just generic functions: we also want generic types."`
- Q3-R (the nested shape): `- **Taken from Go**: [Type Parameters Proposal (Taylor and Griesemer, 2021)](https://go.googlesource.com/proposal/+/refs/heads/master/design/43651-type-parameters.md): a generic type may refer to itself only with its own type parameters as arguments, in the same order, which prevents infinite recursion of type instantiation.` (Use **Taken from** only if the sitting adopts Go's rule; else **Reached first by**.)
- Q3-C: `- **Reached first by Go**: [Package container/heap](https://pkg.go.dev/container/heap): heap operations for any type that implements heap.Interface, the caller keeping the elements.`

---

## Question 4: a byte in `f"..."` written as its number, with no word from the compiler

### verdict

**approve (advisory) Q4-C; object (advisory) to Q4-R and Q4-R2 on precedent.** Whether the thesis
warrants a departure is the blind seat's measurement, not mine.

- In Go, in Python 3, and in the direction C++ is moving, a byte where text is written gives its number.
- The languages that closed the character-or-number trap closed it **at the index**: Rust refuses `s[0]`
  on a string, and Swift cannot index a string by an integer.
- **No compiler I read refuses a single byte at the place where it becomes text.** The nearest are Zig,
  which refuses a byte *slice* formatted with no specifier, and a clang-tidy check, a linter not a
  compiler, aimed at the mirror-image trap.

### precedents

1. **Go: a string's index is a `byte`, and a `byte` prints its number; vet warns only on the mirror trap.**
   - The specification, § Index expressions: *"`a[x]` is the non-constant byte value at index `x` and the
     type of `a[x]` is `byte`"* (https://go.dev/ref/spec). **verified**.
   - `fmt`'s default formats: *"uint, uint8 etc.: %d, %#x if printed with %#v"*; `%c` is *"the character
     represented by the corresponding Unicode code point"* (https://pkg.go.dev/fmt). **verified**.
   - Go 1.15's vet (https://go.dev/doc/go1.15). **verified**:
     - *"The vet tool now warns about conversions of the form `string(x)` where `x` has an integer type
       other than `rune` or `byte`. Experience with Go has shown that many conversions of this form
       erroneously assume that `string(x)` evaluates to the string representation of the integer `x`."*
     - The check is on by default under `go test`.
     - The page says a future release may prohibit the conversion.
   - That is the **mirror** of Heroes' trap (a number written as a character), warned on by a tool, and
     `byte` is exempt.
2. **Rust refuses the index itself, and says why.** *The Rust Programming Language*, § 8.2 *Indexing into
   Strings* (https://doc.rust-lang.org/book/ch08-02-strings.html). **verified**:
   - `s1[0]` is `error[E0277]: the type `str` cannot be indexed by `{integer}``, with the note *"you can use
     `.chars().nth()` or `.bytes().nth()`"*.
   - *"to avoid returning an unexpected value and causing bugs that might not be discovered immediately,
     Rust doesn't compile this code at all and prevents misunderstandings early in the development
     process."*
   - *"it's not clear what the return type of the string-indexing operation should be: a byte value, a
     character, a grapheme cluster, or a string slice."*
   - *"The best way to operate on pieces of strings is to be explicit about whether you want characters or
     bytes."*
   - **A Clippy lint for a `u8` printed as a number: not found** (search summary; the nearest,
     `char_lit_as_u8`, is about the cast `'x' as u8`).
3. **Swift: no integer index on a string.** TSPL, *Strings and Characters*: *"Swift strings can't be
   indexed by integer values."* The `utf8` view is *"a collection of unsigned 8-bit (`UInt8`) values"*
   (https://raw.githubusercontent.com/swiftlang/swift-book/main/TSPL.docc/LanguageGuide/StringsAndCharacters.md).
   **verified**.
4. **Python 3: a `str` index is a string; the run-time `BytesWarning` is being retired.**
   - *"Since there is no separate 'character' type, indexing a string produces strings of length 1."*
     (https://docs.python.org/3/library/stdtypes.html). **verified**.
   - That a `bytes` index gives an `int` is in the docs' *Bytes Objects* note, past the fetched text:
     **unverified at the source**.
   - The `-b` option (https://docs.python.org/3/using/cmdline.html). **verified**:
     - *"Issue a warning when converting `bytes` or `bytearray` to `str` without specifying encoding or
       comparing ... `bytes` with `int`."*
     - *"Deprecated since version 3.15 ... schedule them to become no-op in Python 3.17. These were
       primarily helpers for the Python 2 -> 3 transition. ... use a type checker instead."*
5. **C++ is moving toward the number.** C++ has the mirror trap: `uint8_t` is a character type and
   iostreams print it as a character.
   - WG21 **P3154R3**, *Deprecating signed character types in iostreams*, Elias Kosunen, 2026-06-09,
     audience LWG (https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3154r3). **verified**:
     - The behaviour *"is unexpected, especially when using the aliases `int8_t` or `uint8_t`"*.
     - It proposes deprecating the iostreams overloads for `signed char` and `unsigned char`, noting that
       `std::format` already treats them as integers.
     - A study of about 1.5 million lines found *"Only four instances of use"*, one probably a bug.
   - clang-tidy's **`bugprone-unintended-char-ostream-output`**
     (https://clang.llvm.org/extra/clang-tidy/checks/bugprone/unintended-char-ostream-output.html).
     **verified**; the version that added it is not on the page:
     - *"Normally, when `unsigned char (uint8_t)` or `signed char (int8_t)` is used, it is more likely a
       number than a character."*
     - Its fix states the intent: a cast to `unsigned int`, a unary `+`, or a cast to `char`.
6. **Zig refuses a byte *slice* with no specifier.** The commit *"require specifier for arrayish types"*
   (marler8997, committed by andrewrk, https://github.com/ziglang/zig/commit/1480c428065c01c6feff22ce84021c2e0e30aa9b)
   adds *"cannot format slice without a specifier (i.e. {s} or {any})"*. **verified**.
   - Its date: January 2021 per a search summary, **unverified** (the page shows none).
   - For a single byte, zig.guide (Zig 0.15.2) says only *"`{c}` formats a byte into an ascii character"*
     (https://zig.guide/standard-library/formatting-specifiers). What `{}` does with one `u8` is
     **unverified**.
7. **Java, C#, Kotlin**: that a string's index yields a character type, not a byte, is **unverified**
   today; no page was fetched.
8. **Panel 061** measured the same printing in Heroes (*"`b: u8 @ 'a'` prints 97"*) and ruled on another
   sentence (`docs/panel/061-the-fields-nobody-named.md:279-284`). **carried**.

### argument (Q4, 116 words)

Go, Python 3 and the direction of C++ (`std::format`, P3154) write a byte as its number. clang-tidy calls
the number the likelier meaning, which matches the compiler-engineer's census: 222 of 226 sites mean
the number. The languages that closed the trap closed it at the index. Rust refuses `s[0]` on a string
*"to avoid returning an unexpected value"*, and Swift cannot index a string by an integer. No compiler I
read refuses one byte where it becomes text. Zig refuses a byte slice with no specifier, and Python is
retiring its run-time `BytesWarning` in favour of type checkers. Go's vet warns on the mirror trap and
exempts `byte`. Q4-R and Q4-R2 have no precedent. A departure may be right, and only the blind seat
can show that.

### condition (Q4)

- **Toward Q4-R**: a compiler, not a linter, that refuses a single byte in a format or interpolation
  position, with its record. I found none.
- **Toward Q4-R2**: the same, refusing an index into a string where text is written. I found Rust's
  refusal of the index everywhere, which is wider and is not Heroes' design (`s[i]` yields `u8` on
  purpose, spec § 10).
- **Toward Q4-R in general**: P3154's adoption would move C++ toward Q4-C, not away. The blind seat's
  task 4 is the measure that can outweigh precedent here.

### search (Q4)

- **Queries**:
  - `Zig std.fmt "cannot format slice without a specifier" ...`
  - `C++ std::cout uint8_t prints as character ...`
  - `linter warns printing string index byte as integer instead of character ...`
  - `clippy lint u8 printed as number instead of char ...`
- **Fetched**: Go's spec, `fmt` and 1.15 notes; Rust's book § 8.2; TSPL's *Strings and Characters*;
  Python's `stdtypes` (first 100,000 characters) and command line; clang-tidy's check; P3154R3; the Zig
  commit; zig.guide.
- **Not found**: a compiler refusing a byte at a text position; a lint flagging `s[i]` inside a format
  (looked for in the searches above; the Xygeni detector in the search flags printing a byte *slice*,
  and was not read).
- **Not done**: `go vet`'s printf check for `%s` with a byte was not established. `fmt` documents
  `%!verb(type=value)` for a wrong type; whether vet flags `%s` with a `byte` is **unverified**.

### candidate `## Prior art` lines (Q4)

- Q4-C: `- **Reached first by Go**: [Package fmt](https://pkg.go.dev/fmt): the default format of %v for uint8 is %d, its number.`
- Q4-C: `- **Reached first by C++ (WG21)**: [P3154R3, Deprecating signed character types in iostreams (Kosunen, 2026)](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p3154r3): proposes deprecating the iostreams overloads that print signed char and unsigned char as characters, std::format already treating them as integers.`
- Q4-R: `- **Reached first by Clang-Tidy**: [bugprone-unintended-char-ostream-output](https://clang.llvm.org/extra/clang-tidy/checks/bugprone/unintended-char-ostream-output.html): finds unsigned char and signed char written to an ostream, whose output is a character where a number is more likely meant, and suggests a cast that states the intent.`
- Q4-R2: `- **Reached first by Rust**: [The Rust Programming Language, § 8.2](https://doc.rust-lang.org/book/ch08-02-strings.html): indexing a string by an integer does not compile, so that the operation never returns an unexpected value.` (At the index, not at the hole. Cite it only if the sitting reads it as the same refusal.)

---

## Question 5: `print` writing `1e-06` where a literal with an exponent is refused

### verdict

**approve (advisory) Q5-R, with the range refusal the compiler-engineer priced**, whose precedent is
Java's specification. **Approve Q5-U.** **Object to Q5-P.** **Object to Q5-C's *"no exponent"* half**,
which has no precedent I could find. Its *"name the reader"* half I leave to the spec-warden: Python names
its own `eval`, and no language I read names C's `strtod`.

### precedents

1. **Python prints the same form and promises it reads back.**
   - The tutorial, *Floating Point Arithmetic: Issues and Limitations*
     (https://docs.python.org/3/tutorial/floatingpoint.html). **verified**:
     - Any of the decimal strings sharing one binary approximation *"could be displayed while still
       preserving the invariant `eval(repr(x)) == x`"*.
     - *"Starting with Python 3.1, Python (on most systems) is now able to choose the shortest of these"*.
   - The format mini-language (https://docs.python.org/3/library/string.html). **verified**:
     - For a float, *"the exponent always contains at least two digits"*.
     - With no presentation type, a float is formatted like `'g'`, and *"m <= exp < p, where m is -4 for
       floats"* decides fixed-point.
   - So Python writes the two-digit `e-06` form that Heroes' `print` writes, and reads it back. That
     `str(0.000001)` is exactly `1e-06` is my reading of those two rules: **unrun**.
2. **Go reads the form, refuses overflow, and canonicalizes the letter in its formatter.**
   - The specification (https://go.dev/ref/spec). **verified**:
     - `decimal_float_lit` includes `decimal_digits decimal_exponent`.
     - `decimal_exponent = ( "e" | "E" ) [ "+" | "-" ] decimal_digits`.
     - § Representability: `1e1000` for `float64` *"overflows to IEEE +Inf after rounding"* and is not
       representable, while `-1e-1000` *"rounds to IEEE -0.0 which is further simplified to 0.0"* and is.
     - § Constants: every implementation must *"Give an error if unable to represent a floating-point or
       complex constant due to overflow."*
   - Go 1.13's notes (https://go.dev/doc/go1.13). **verified**:
     - *"`gofmt` ... now canonicalizes number literal prefixes and exponents to use lower-case letters"*;
       `1.2E3` becomes `1.2e3`.
     - Heroes' built `exponent_case` refuses `E` with a `certain` fix instead, as `base_prefix_case`
       refuses `0X` (`252-reports/compiler-engineer.md` § Q5). Same canonical spelling, a different
       mechanism.
3. **Java refuses a literal that is not its value, both ways.** JLS SE 25 § 3.10.2
   (https://docs.oracle.com/javase/specs/jls/se25/html/jls-3.html). **verified**:
   - *"It is a compile-time error if a non-zero floating-point literal is too large, so that on rounded
     conversion to its internal representation, it becomes an IEEE 754 infinity."*
   - *"It is a compile-time error if a non-zero floating-point literal is too small, so that, on rounded
     conversion to its internal representation, it becomes a zero."*
   - A subnormal result is not an error.
   - The grammar admits `Digits ExponentPart` (e.g. `1e137`), and underscores *"between digits that denote
     the whole-number part, and between digits that denote the fraction part, and between digits that
     denote the exponent."*
   - **This is the compiler-engineer's range refusal** (`1e400` reaching `inf`, `1e-400` reaching `0.0`,
     and the 401-digit literal he found printing `inf` today). The sentences are the current
     specification's. Since when Java has had them is **unverified**.
4. **JSON admits the form and excludes infinity and NaN.** RFC 8259 § 6
   (https://www.rfc-editor.org/rfc/rfc8259#section-6). **verified**:
   - `exp = e [ minus / plus ] 1*DIGIT`, so `1e-06` is a JSON number.
   - *"Numeric values that cannot be represented in the grammar below (such as Infinity and NaN) are not
     permitted."*
   - That is the spec-warden's finding that `inf` and `nan` stay unreadable under Q5-R, met elsewhere by
     exclusion (my reading).
5. **JavaScript prints positionally in a band and with an exponent outside it.** MDN,
   *Number.prototype.toString*: *"Scientific notation is used if radix is 10 and the number's magnitude
   (ignoring sign) is greater than or equal to 10^21 or less than 10^-6."*
   (https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/toString).
   **verified**.
6. **Erlang refuses `1e6` and prints with its required point, so its output reads back.** *Data Types*
   (https://www.erlang.org/doc/system/data_types.html). **verified**:
   - *"floating-point numbers must start with a digit, and must contain a `.`"*.
   - *"literals such as `.01` and `1e6` are not allowed, and must be written `0.01` and `1.0e6`,
     respectively."*
   - `36028797018963968.0` prints as `3.602879701896397e16`.
   - `_` between digits: `1_234.333_333`.
   - EEP 51 (Sergey Prokhorov, created 2019-10-07, implemented in OTP 23, https://www.erlang.org/eeps/eep-0051).
     **verified**:
     - `1.0e1_23` is valid, and `_` may not stand next to `.` or `e`.
     - Its motivation: separators *"make it easier to visually spot typos like `MicroSecond = Second *
       100000`"*.
7. **Pascal (ISO 7185) reads both forms and leaves the output's letter to the implementation.**
   standardpascal.org's OCR text of ISO 7185 (https://standardpascal.org/iso7185.html; the page warns of
   recognition errors). **verified**:
   - § 6.1.5: `unsigned-real = digit-sequence '.' fractional-part [ 'e' scale-factor ] | digit-sequence 'e'
     scale-factor`.
   - `scale-factor = [ sign ] digit-sequence`, with `sign = '+' | '-'`.
   - The standard's own examples include `1e10`, `5e-3` and `87.35E+8`.
   - § 6.1.1: a letter's case is *"insignificant"*.
   - Annex E.11, E.13, E.14: the default `TotalWidth` for real-type, `ExpDigits`, and *"the exponent
     character ('e' or 'E') used on output"* are implementation-defined.
8. **Oberon-07 (Wirth, revision 2016) requires a point and an upper-case `E`.** § 3, p. 2. **verified**,
   page image:
   - *"A real number always contains a decimal point. Optionally it may also contain a decimal scale
     factor."*
   - `real = digit {digit} "." {digit} [ScaleFactor]` and `ScaleFactor = "E" ["+" | "-"] digit {digit}`.
   - *"Capital and lower-case letters are considered as being distinct."*
   - So `1.E-6` and `1.0E-6` are Oberon reals, and `1E-6` and `1.0e-6` are not.
9. **Rust reads the form; its `Display` is positional; its `Debug` moved to exponents in 1.58.**
   - The Reference, *Tokens* (https://doc.rust-lang.org/reference/tokens.html). **verified**:
     - `FLOAT_LITERAL → DEC_LITERAL ( . DEC_LITERAL )? FLOAT_EXPONENT SUFFIX? | ...`.
     - *"All number literals allow `_` as a visual separator: `1_234.0E+18f64`"*.
   - The Rust Internals forum, *Format specifier for non-scientific notation?*
     (https://internals.rust-lang.org/t/format-specifier-for-non-scientific-notation/16005), 2022-01-22.
     **verified (forum)**:
     - jtempest: *"In Rust 1.58, the debug output format for float primitives changed. Now, if they are
       above or below a certain threshold (very small or very large), they are written using scientific
       notation."* His example: `0.00000023841858` became `2.3841858e-7`.
     - steffahn: *"`Display` hasn't changed"* and *"uses no scientific notation."*
   - The PR that made the `Debug` change and its stated motivation: **unverified**. I fetched two candidate
     PRs (#78618, #90297) and neither is it.
10. **Digit separators in a float** (Q5-U):
    - **PEP 515**: Final, Python 3.6, created 2016-02-10 (https://peps.python.org/pep-0515/). **verified**:
      - `digitpart: digit (["_"] digit)*` and `exponent: ("e" | "E") ["+" | "-"] digitpart`.
      - Underscores *"can aid readability of long literals"*.
      - Its survey of other languages, paraphrased by the fetch tool, says of Julia: *"not inside a float's
        exponent"*.
    - **Go 1.13** (https://go.dev/doc/go1.13): *"The digits of any number literal may now be separated
      (grouped) using underscores, such as in `1_000_000`, `0b_1010_0110`, or `3.1415_9265`."* **verified**.
    - **Java**, **Rust** and **Erlang**: as 3, 9 and 6 above.
11. **The one refusal I found**: Apache Spark's SPARK-7099, *"Floating point literals cannot be specified
    using exponent"* (created 2015-04-23, resolved *"Not A Problem"* 2015-10-14,
    https://issues.apache.org/jira/browse/SPARK-7099). **verified**:
    - Description: *"Floating point literals cannot be expressed in scientific notation using an exponent,
      like e.g. 1.23E4."*
    - The page carries no comment saying why, nor what Spark printed. It is a query dialect, not a
      language of this kind. **It does not answer the brief's question**, and I name it so the search is
      honest.
12. **A defect caused by a dropped or added zero in a long decimal literal: no record found.** The nearest
    are EEP 51's typo example (6) and PEP 515's readability rationale (10).

### argument (Q5, 116 words)

Every language I read that prints an exponent also reads one. Python writes the same two-digit `e-06`
form and promises `eval(repr(x)) == x`. Go, Java, Rust, Pascal and JSON's grammar all accept `1e-06`.
Erlang refuses `1e6` but prints with its required point, so its output still reads back. The one
refusal I found, Spark SQL's 2015 report, was closed without a reason. Java's specification refuses a
non-zero float literal that rounds to infinity or to zero. Go refuses only overflow. That precedent is
the compiler-engineer's range refusal, owed today whatever Q5 decides. Float separators are
near-universal. Rust's `Display` prints positionally, yet Rust moved `Debug` to exponents in 1.58. Q5-P
takes the road Rust's `Debug` left.

### condition (Q5)

- **Toward Q5-C**: a language that refuses exponent literals while printing them, with its stated reason.
  Spark's page gives none.
- **Toward Q5-P**: the motivation of Rust 1.58's `Debug` change, if it shows that positional output was
  kept somewhere for a reason that applies here. Unread.
- **Against the range refusal**: a record of Java's two compile-time errors refusing correct programs.
  Not looked for.

### search (Q5)

- **Queries**:
  - `Rust Display f64 never uses scientific notation ...`
  - `Rust 1.58 release notes Debug floats "scientific notation" ...`
  - `"digit separators" proposal rationale "count the zeros" ...`
  - `programming language float literals "no scientific notation" OR ...`
  - `Gleam float literal scientific notation ...`
- **Fetched**: Python's tutorial and `string` docs, RFC 8259, MDN, Erlang's data types and EEP 51, ISO
  7185 (three reads), the Oberon-07 PDF, Rust's tokens reference and the internals thread, Go's spec, 1.13
  notes and representability, JLS SE 25 chapter 3, PEP 515, SPARK-7099, and rust-lang/rust #78618 and
  #90297 (neither was the `Debug` change).
- **Not found**:
  - a language refusing exponent literals while printing exponents. Looked for in the search above and
    in every page here.
  - Gleam's float syntax by its own documentation. Search summaries only, not cited.
  - C++14's N3781 rationale. Its PDF was not fetched.
  - a defect record for a long literal's lost zero.

### candidate `## Prior art` lines (Q5)

- Q5-R: `- **Reached first by Python**: [Floating Point Arithmetic: Issues and Limitations](https://docs.python.org/3/tutorial/floatingpoint.html): repr chooses the shortest of the decimal strings sharing a float's binary approximation, preserving eval(repr(x)) == x.`
- Q5-R: `- **Reached first by Go**: [The Go Programming Language Specification, Floating-point literals](https://go.dev/ref/spec): a decimal float literal may be decimal digits followed by an exponent, ("e" | "E") ["+" | "-"] decimal_digits.`
- Q5-R (the range refusal): `- **Taken from Java**: [JLS SE 25 § 3.10.2](https://docs.oracle.com/javase/specs/jls/se25/html/jls-3.html): a non-zero floating-point literal that rounds to an IEEE 754 infinity, or to zero, is a compile-time error.`
- Q5-R (the letter): `- **Reached first by Go**: [Go 1.13 Release Notes](https://go.dev/doc/go1.13): gofmt canonicalizes number literal prefixes and exponents to lower-case letters; 1.2E3 becomes 1.2e3.`
- Q5-U: `- **Reached first by Java**: [JLS SE 25 § 3.10.2](https://docs.oracle.com/javase/specs/jls/se25/html/jls-3.html): underscores are allowed between the digits of a floating-point literal's whole-number part, its fraction part and its exponent.`
- Q5-U: `- **Reached first by Python**: [PEP 515](https://peps.python.org/pep-0515/): a single underscore may stand between digits in a float's digit parts and its exponent.`
- Q5-U: `- **Reached first by Go**: [Go 1.13 Release Notes](https://go.dev/doc/go1.13): the digits of any number literal may be separated by underscores, such as 3.1415_9265.`

---

## Other seats' claims about other languages

- **The compiler-engineer's report** (`252-reports/compiler-engineer.md`) makes no claim about another
  language's behaviour. Its precedents are Heroes' own (`base_prefix_case`, panel 029's R4 and R5).
  Nothing to verify. Two of its findings meet precedents above:
  - **Its range refusal is Java's § 3.10.2** (§ Question 5, precedent 3).
  - **Its nested-shape refusal is narrower than Go's** (§ Question 3, precedent 8).
- **The spec-warden's report** (`252-reports/spec-warden.md`) makes no claim about another language.
  - Its finding that `inf` and `nan` stay unreadable under Q5-R meets JSON's grammar, which excludes
    them (§ Question 5, precedent 4).
  - Its call to name C's `strtod` has no precedent I read: Python names its own `eval`. That is a
    finding about where I looked, not that none exists.
- **`00-shared.md`** names no other language's behaviour as fact. It runs Python's `Decimal` as an
  instrument (§ Question 5's Q5-P) and asks the rest as questions (§ Questions no one has run).

## What I did not do

- No paid run, no credential, no GitHub write, no commit, no sub-agent.
- No file written but this report. One file was written outside the root by the fetch tool (§ top).
- Nothing read under `rebel-scratch/252-llm-ergonomist/` or `rebel-scratch/253-llm-ergonomist/`.

Report closed 2026-11-19. This seat read no clock; the coordinator's `date` stands in for the hour.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/252-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
