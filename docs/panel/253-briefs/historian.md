# Panel 253, historian (advisory)

Read `00-shared.md` first, whole: the six questions, what the brief measured, and the resolutions put to
the seats. Your question for each: **who has done this before, and what happened to them?** Every claim
carries its source, verified by a page you read this session, or is marked `unverified`. You have no veto.

**Your report** is `.claude/worktrees/rebel-round/docs/panel/253-reports/historian.md`, written as you go,
in `.claude/agents/historian.md`'s structure (`verdict`, `precedents` each marked verified or unverified,
`argument`, `condition`, `search`), **one block per question**. You read the web and the repository; you
build nothing and run no paid call. **A precedent you mark `verified` may become a line of the sitting's
decision issue's `## Prior art`** (`.claude/rules/records.md` § And the record says whose idea it was),
written only from what the page itself states: panel 251's synthesis narrowed two of its historian's lines
to their pages (`docs/panel/251-*.md:287-289`), and a count of 2026-10-11 found 33 of 90 such lines saying
more than the page (`records.md`, carried). So quote the page's words beside each claim.

**Earlier sittings already read some of this; re-read rather than carry** (`grep -rl -i -E` over
`docs/panel/*-reports/historian.md`, re-counted after the critic's finding 17): Deno in 12 reports (153,
158, 179, 185, 194, 209, 217, 230, 234, 235, 238, 250); `golang.org/x` at 161 to 250 (Go's vendor and module
rules, 250's above all); D's Phobos at 212 and 227; `RUST_BACKTRACE` at 230; Go's documentation pages
(`go doc`, `godoc` or `pkg.go.dev`) in 51 reports from 147 on, mostly as citations; rustdoc at 153, 189,
193. Nobody read dudect, constant-time checkers, CryptoKit, Boost, `t.Skip`, PEP 594, Go's pclntab or
automake's exit 77 (`grep -l -i -F` of each over the same files prints nothing). **The sitting is whole,
six questions** (the coordinator's decision, `00-shared.md` § One sitting, six questions): write one block
per question, whole, before the next.

## Question 1: a language that imposes no standard library and offers an optional one

The author's sentence: *"Heroes imposes no standard library and offers one that is optional"*, Rebel, one
package among packages, never used by the compiler. Find who did it and what followed:

- **D's two standard libraries**, Phobos and Tango, and whether the split was a cost the D community wrote
  down (dates, the D2 decision, druntime's separation);
- **OCaml**: Stdlib beside Jane Street's Base and Core and the community's Batteries; whether any became
  the de facto standard library and how;
- **Haskell**: `base` beside the Haskell Platform, and the Platform's end (its date and stated reason);
- **Python's batteries** and PEP 594's removal of *dead batteries* (which release, what it removed, the
  stated maintenance argument), the opposite direction from the author's;
- **Go's `golang.org/x`** repositories: libraries kept outside the standard library on purpose, versioned
  apart (what Go's own documents say they are for);
- **Deno's `@std`** on JSR: a standard library published as packages a program takes or leaves (its own
  words on why, and when it moved there);
- **Rust**: a small `std` by design, and the community's refused or abandoned proposals for a blessed set
  of crates (an RFC or a team statement, dated);
- **Zig, Odin, Nim**: a library shipped with the compiler; Odin's `core:` and `vendor:` collections (§1.11
  cites Odin already).

What the sitting needs from you: **does an optional broad library beside a language succeed where it is
one, or fragment where there are two**, and does any project record what happened when the compiler's
own tree stayed off its library (the author's decision 6).

## Question 2: a documentation verb, and examples that are tests

The robust route: `heroes doc` prints a module's reference from its comments, the same bytes every time,
with each function's example taken from a documented `test` block (Go's and D's shape, panel 136's word,
design.md:2601-2604), and refuses a parameter line naming no parameter. Find:

- **Go**: `go doc` and `godoc`/`pkgsite`; testable examples (`func ExampleFoo`, `// Output:`), how an
  example is attached to its function by name, and Go's stated refusal of `@param`-style tags (its
  *Go Doc Comments* page, dated);
- **D**: `ddoc` and *documented unittests* (a `///` comment above a `unittest` block shown as the
  example), the release that added them;
- **Rust**: rustdoc's `# Examples` convention and doctests (the reverse arrow panel 136 refused), and
  whether rustdoc checks that a documented parameter exists (`missing_docs`, intra-doc links' checks);
- **Javadoc** `@param` checks (`-Xdoclint`), and any tool that refuses a parameter named in a comment but
  absent from the signature;
- **a documentation tool that promises byte-identical output** across runs and platforms, and why.

## Question 3: cryptography where the platform's own library lacks it

The routes: CryptoKit through a Swift member on macOS; the primitives written in Heroes under a
constant-time discipline a tool checks on the binary; AES-CTR from CommonCrypto with GHASH beside it; a
small constant-time C library in the platform layer. Find:

- **Go**: its cryptography written in Go, how it keeps constant time (`crypto/subtle`, generated field
  arithmetic such as fiat-crypto, its review rules), whether any tool checks it, and the FIPS 140-3 module
  (release and date);
- **constant-time checkers**: dudect (the paper, year, what it measures), valgrind-based taint as BoringSSL
  uses it (`CONSTTIME_SECRET` or ctgrind, BoringSSL's own page), and any published finding that a C
  compiler turned constant-time source into secret-dependent branches (paper, year, compiler, flags);
- **Apple**: whether CryptoKit has any C API, whether CommonCrypto has ever exposed GCM publicly, and
  `swift-crypto`'s design (CryptoKit's API over BoringSSL off Apple's platforms, its repository's words);
  whether Apple documents `swiftc` in the Command Line Tools (this Mac's CLT holds it, reached through
  `xcrun`, `00-shared.md` § Question 3: a page saying so, or the opposite, is what is asked);
- **constant time in a language with checked arithmetic**: any language or library that writes
  constant-time code where every overflow aborts and no wrapping operator exists (Heroes' spec lines 210
  and 220), and how it wrote modular arithmetic;
- **Windows CNG**: whether any Windows release added Ed25519 or a documented X25519 key blob, with the
  page and date;
- **small constant-time C libraries**: Monocypher, TweetNaCl, libsodium: licence, size, audits (by whom,
  when), which of the four primitives each holds.

## Question 4: a test that cannot run here

The robust route: a third outcome, `skip(reason:)`, printed with its reason, counted apart, failing no
run. Find, each with its words and its release:

- **Go**'s `t.Skip` and `-short`; **Rust**'s `#[ignore]` and whether a test can skip itself at run time
  (an accepted RFC or a stated refusal); **Python**'s `unittest.SkipTest` and `pytest.skip`; **JUnit 5**'s
  assumptions (*aborted*, not skipped); **Zig**'s `error.SkipZigTest`; **Catch2**'s `SKIP`; **TAP**'s
  `# SKIP` directive;
- **automake's exit status 77** for a skipped test (the manual's words), and whether another runner reads
  an exit code as a skip;
- **a test runner that turns skips red** (a flag, a CI convention) or counts a run of nothing but skips as
  a failure, as this project's harness does (`tests/harness/report.hero:49-53`).

## Question 5: the stack of calls at an abort

The routes: a frame walk naming each frame and its line; the caller's place passed to every call (panel
230's library route made general, Rust's `#[track_caller]` its shape); the call into a package alone.
Find:

- **Go**: what a panic prints (every goroutine's stack, by default), and what the runtime keeps to print it
  (the function and line table, its stated size cost);
- **Rust**: `RUST_BACKTRACE` off by default (the stated reason), `#[track_caller]` (its RFC, what it costs
  per call), and the backtrace's symbolisation at run time;
- **a parent that symbolises a child's addresses** (route (A) of `00-shared.md` § Question 5): any test
  runner or launcher that receives raw return addresses from a crashed program and resolves them against
  its binary and line tables (Go's `go test`? Rust's test harness? `llvm-symbolizer` driven by a runner),
  and macOS's own split between a binary and its `.dSYM` or debug map;
- **Swift**: the runtime backtracer that prints on a crash (the release, Linux and macOS, opt-in or not);
- **Zig**: the stack trace a debug build prints, and what a release build keeps;
- **Windows**: `StackWalk64` and `SymFromAddr` and their needs (a PDB beside the binary), from Microsoft's
  documentation.

## Question 6: an umbrella at one version, placed package by package

The author's choice: Rebel keeps one version, tested and published as one, and a program places only the
packages it uses with their Rebel dependencies; independent versions per package refused. Find:

- **Boost**: one release number over many libraries, its modular repositories (since which release), and
  `bcp`, its tool that extracts a subset with its dependencies (its documentation's words);
- **Qt**'s modules at one version; **Jakarta EE**'s or **Spring Boot**'s bill of materials (a version set
  a build pins); **Jetpack Compose**'s BOM;
- **the opposite choice and what it bought**: Deno `@std` versioned per package on JSR, the AWS SDK for Go
  v2's per-service modules, `golang.org/x` modules apart; and any record of a diamond conflict inside one
  vendor's family that drove a project to one version (crates or npm, with the issue or post);
- whether any of them places only the packages a program uses **from a list the program writes** (the
  robust spelling) or **from what its imports reach** (panel 250's M), and what went wrong with either.

## What you are asked to say

Your definition's structure per question; every precedent with its page, the page's own words beside it,
and `verified` or `unverified`; what you searched and where, so a sentence about something not found names
what was looked for (the `search` field).
