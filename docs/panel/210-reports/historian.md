# Panel 210, historian (advisory, no veto)

Written as I went. This seat has no shell, so there is no `date` reading
and no build: the instrument is web search and fetch, and every claim below
carries the URL fetched in this sitting (2026-10-11). A claim I could not
confirm on a fetched page is marked **unverified**, and a negative sentence
is a question naming what was searched. *Heroes of code* was not opened this
sitting; Wirth's Zurich room (Pascal, Oberon) is the one route it would have
pointed to, and the sources there are primary.

The question is the decision issue
`issues/2026-10/10/2026-10-10-1737-an-at-parameter-the-callee-never-writes-is-accepted.md`.
The Heroes facts compared against are read from the spec, not run: spec § 9
(`@` is copy in, copy out, marked in the signature and at the call site;
top-level functions are values), spec § 5 (an unused parameter is an error, a
read is a use), spec § 3 (no aliasing among the values the language owns; a
`ptr` is a copied address; the production `TypeArg = Type | ident ":" Type`
has no `@`).

## Precedents

### Rust: clippy `needless_pass_by_ref_mut`, the closest modern analogue (a lint, softened within one release)

- **What it is**: *"Check if a `&mut` function argument is actually used
  mutably"*, suggesting `&T`; its doc warns *"Be careful if the function is
  publicly reexported as it would break compatibility"*. Version attribute
  `1.73.0`, group `nursery` today:
  https://raw.githubusercontent.com/rust-lang/rust-clippy/master/clippy_lints/src/needless_pass_by_ref_mut.rs
  (verified).
- **Added in Rust 1.73 (2023-10-05)**, the changelog's *New Lints*:
  https://raw.githubusercontent.com/rust-lang/rust-clippy/master/CHANGELOG.md
  (verified, offset 100000).
- **Moved out of the default set**: PR #11596, *"Move
  `needless_pass_by_ref_mut`: `suspicious` -> `nursery`"*, merged 2023-10-02,
  body *"has been released with some important bugs (notably having a lot of
  reported false positives and an ICE). So it may not be really ready for being
  in stable"*: https://github.com/rust-lang/rust-clippy/pull/11596 (verified).
  `suspicious` is **warn** by default and `nursery` **allow**:
  https://raw.githubusercontent.com/rust-lang/rust-clippy/master/README.md
  (verified). The changelog lists *"Moved [`needless_pass_by_ref_mut`] to
  `nursery` (Now allow-by-default)"* under 1.74 (2023-11-16) and again under
  1.75 (2023-12-28) (the CHANGELOG URL, verified). Whether stable 1.73 itself
  shipped it at warn is an inference from those three facts, not a page I
  read: **unverified**.
- **Still nursery about three years on** (the source above, verified as to
  today; the duration is arithmetic). In August 2025 a contributor called it
  *"just the Nursery lint"* and proposed enabling it more widely:
  https://github.com/rust-lang/rust-clippy/pull/15512 (verified).
- **Its false positives, by class**, from the issue search
  https://github.com/rust-lang/rust-clippy/issues?q=is%3Aissue+needless_pass_by_ref_mut
  (verified; 25 of 31 matches shown):
  - **the function used as a value** whose type demands `&mut`: #11182
    (opened 2023-07-18, `bar(foo)` with `bar(_: impl Fn(&mut i32) -> i32)`),
    https://github.com/rust-lang/rust-clippy/issues/11182, and #11199 (opened
    2023-07-20, `type ActionFn = fn(&mut Context)`),
    https://github.com/rust-lang/rust-clippy/issues/11199 (both verified;
    both closed by PR #11207). The source now records every function named
    outside a direct call and skips it, comment *"#11182; do not lint if
    mutability is required elsewhere"* (source URL, verified);
  - **a platform-gated body** that writes on one target and only reads on
    another: #11185 (opened 2023-07-19), answered with a note rather than a
    suppression, PR #11226:
    https://github.com/rust-lang/rust-clippy/issues/11185 (verified);
  - **closures and async bodies** capturing the parameter: #11216, #11299,
    #11380, #11545, #11561, #11610, #11620 (the issue list, verified as to
    titles and labels): the largest class;
  - **unsafe code and raw pointers**: #11586 (closed), #12905 and #13172 (open
    since 2024); the source now returns early on unsafe functions and non-Rust
    ABIs, *"We don't check unsafe functions"* (issue list and source,
    verified);
  - **exported API**: skipped while `avoid-breaking-exported-api` is set; #11374
    was the lint failing to honour it (source and issue list, verified);
  - **trait implementations**: skipped, *"Exclude non-inherent impls"* (source,
    verified).
- **rustc itself** warns by default on a `mut` binding nothing mutates,
  `unused_mut`, *"detects mut variables which don't need to be mutable"*, help
  *"remove this `mut`"*:
  https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html (verified,
  offset 200000). Whether rustc has any lint for a `&mut T` parameter never
  written is a question: I read that listing for `unused_mut` only.

### Ada and GNAT: `in out` never assigned, a warning since 2007, narrowed the same week

- **Added 2007-08-14** (ChangeLog date; posted 2007-08-16 by Arnaud Charlet):
  *"if an in out parameter is read but never assigned, then under control of
  -gnatwk, a warning is given that the mode could be in"*, output *"formal
  parameter "b" is not modified, mode could be "in" instead of "in out""*:
  https://gcc.gnu.org/legacy-ml/gcc-patches/2007-08/msg00991.html (verified).
- **Narrowed in the same batch**: *"This patch suppresses the warning for
  unmodified in-out parameters with -gnatwk if the subprogram in question has
  its address taken, or is used as a generic actual"*, the warning delayed
  *"until the whole unit has been processed, since the access or generic use
  will appear after the body has been processed"*:
  https://gcc.gnu.org/legacy-ml/gcc-patches/2007-08/msg00985.html (verified).
  This is clippy's #11182 found sixteen years earlier.
- **Its exceptions on GCC trunk today**, `sem_warn.adb`: address taken,
  generic actual (*"the generic may be forcing IN OUT"*), `Warnings_Off`,
  `pragma Unmodified`, a dispatching operation, a composite type holding
  access values, a private type declared in another unit, a trivial
  subprogram; messages `"?k?formal parameter & is not modified!"` and
  `"\\?k?mode could be IN instead of `IN OUT`!"`:
  https://gnu.googlesource.com/gcc.git/+/refs/heads/trunk/gcc/ada/sem_warn.adb
  (verified, first 100000 characters).
- **Severity**: a warning, off by default, on under `-gnatwa`. GCC 15.1's
  manual on `-gnatwk`: *"The default is that such warnings are not given"*;
  `-gnatwk` is absent from the list of switches `-gnatwa` does not turn on:
  https://gcc.gnu.org/onlinedocs/gcc-15.1.0/gnat_ugn/Warning-Message-Control.html
  (verified; the manual's `-gnatwk` entry speaks of variables and does not
  mention `in out` formals, the page as fetched).
- **SPARK** flow analysis *"warns when an `in out` parameter isn't modified or
  when its initial value isn't used"*:
  https://learn.adacore.com/courses/intro-to-spark/chapters/02_Flow_Analysis.html
  (verified; a warning, in the language whose whole point is proof).
- **`out` never set**: GNAT has `"?v?OUT parameter& not set before return"`, a
  warning (`sem_warn.adb` URL, verified). Whether the Ada RM leaves an
  unassigned `out` legal: **unverified** this sitting.

### C#: `out` must be assigned, an error; `ref` need not be, by design

- **CS0177**, *"The out parameter 'parameter' must be assigned to before
  control leaves the current method"*:
  https://learn.microsoft.com/en-us/dotnet/csharp/misc/cs0177 (verified).
  The standard, § 9.2.7: *"Every output parameter of a function member,
  anonymous function, or local function shall be definitely assigned (§9.4)
  before the function member, anonymous function, or local function returns
  normally"*, and after the call *"each variable that was passed as an output
  parameter is considered assigned"*:
  https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/variables
  (verified). So CS0177 is a **soundness** rule, the caller reading the variable
  as assigned; it is not a rule against a false promise.
- **`ref`**: *"The method can assign a new value to the parameter, but it isn't
  required to."* Both the definition and the call say `ref`; `in` and `ref
  readonly` exist for the read-only case:
  https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/method-parameters
  (verified). The direction Heroes' `@` shares with `ref` is the one C# leaves
  unchecked.
- **ReSharper once offered "Change to value parameter" for a `ref` never
  assigned in a private method, and JetBrains dropped it because a method may
  rely on the caller's variable being changed by another thread or by a call
  it makes**: the thread
  https://resharper-support.jetbrains.com/hc/en-us/community/posts/206049739/comments/205921445
  returned HTTP 403 to two fetches, so this rests on a search engine's summary:
  **unverified**. Its reason, if true, is aliasing, which spec § 3 says the
  values Heroes owns do not have.

### Fortran: `intent(out)` unset warned by three compilers; `intent(inout)` unmodified, nothing found

- gfortran: *"Dummy argument 'x' at (1) was declared INTENT(OUT) but was not
  set [-Wunused-dummy-argument]"*; Intel's warning #6843 and NAG's
  *"INTENT(OUT) dummy argument X never set"* also quoted (thread of
  2024-07-17): https://fortran-lang.discourse.group/t/how-to-detect-uninitialized-arguments/8354
  (verified). When the flag was split out in 2010, Tobias Burnus: *"not
  setting INTENT(OUT) variables indicates an logical error in the program"*:
  https://gcc.gnu.org/pipermail/gcc-patches/2010-May/284017.html (verified).
- Narrowed 2009-11-03 by Steven G. Kargl: *"Silence intent(out) warning for
  derived type dummy arguments with default initialization"*:
  https://gcc.gnu.org/legacy-ml/gcc-patches/2009-11/msg00158.html (verified).
- Does any Fortran compiler or linter warn that an `intent(inout)` could be
  `intent(in)`? A question: two searches (`"intent(inout)" never modified
  "could be intent(in)"`, and the gfortran `-Wunused-dummy-argument` query)
  and the thread above found none.

### Swift: `inout` never mutated, declined in 2019 for want of a silencer, still open

- SR-9883, now swiftlang/swift#52289, *"Unmutated "inout" should warn like
  unmutated "var""*, opened 2019-02-07. Jordan Rose (@belkadan), 2019-02-08:
  *"Sometimes you _have_ to put `inout` because you're, say, satisfying a
  protocol requirement"*, and he *"wouldn't want to add a warning unless we had
  a way to silence it"*: https://github.com/apple/swift-issues/issues/9883
  (verified, rendered as #52289). **Open**, labels `bug` and `compiler`, last
  comment 2019-02-24: https://github.com/swiftlang/swift/issues/52289
  (verified). That @belkadan is Jordan Rose is my identification, not the
  page's: **unverified**.

### Zig: errors for the neighbours, nothing found for the pointer parameter

- 0.9.0: *"It is no longer possible to have a local variable or parameter that
  is not referenced"*, discarded with `_ = x;`:
  https://ziglang.org/download/0.9.0/release-notes.html (verified; its date is
  not on the part of the page read: **unverified**).
- 0.12.0, *"Unnecessary Use of var"*: *"error: local variable is never
  mutated"*, *"note: consider using 'const'"*:
  https://ziglang.org/download/0.12.0/release-notes.html (verified; date not on
  the page as read: **unverified**). First met in dev build
  `0.12.0-dev.1664+8ca4a5240` on 2023-11-21, with complaints of friction while
  exploring (*"I wish I could tell the compiler to "leave me alone" for
  now"*): https://ziggit.dev/t/error-local-variable-is-never-mutated/2238
  (verified).
- **Why it is syntactic**: Andrew Kelley, 2026-06-26, the AST-level check
  avoids false positives from conditional compilation, *"the syntax guarantees
  the property"*; and a `var` pointer whose target is mutated through it counts
  as mutated: https://ziggit.dev/t/i-can-use-var-even-if-the-pointer-is-never-mutated/16357
  (verified).
- Does Zig refuse a `*T` parameter where `*const T` would do? A question: the
  0.12.0 notes and that thread were read for it and say nothing.

### Go: no warnings by philosophy; unused locals refused, unused parameters not; vet has no such check

- FAQ: Go *"refuses to compile programs with unused variables or imports"*;
  *"the Go compiler does not report warnings, only errors"*; *"if it's worth
  complaining about, it's worth fixing in the code"*; and on receivers, *"If
  some of the methods of the type must have pointer receivers, the rest should
  too"*, a pointer that does not write recommended for consistency:
  https://go.dev/doc/faq (verified).
- `go vet`'s 35 analyzers as listed name none about a parameter or receiver
  never written: https://pkg.go.dev/cmd/vet (verified as to the list).
- gopls `unusedparams` skips *"functions that are used as a value rather than
  being called directly"* and exported functions, which *"may be address-taken
  in another package"*; its own fix only renames to `_`, while removing the
  parameter *"along with all corresponding arguments at call sites"* is a
  separate refactoring the user invokes:
  https://pkg.go.dev/golang.org/x/tools/gopls/internal/analysis/unusedparams
  (verified).

### V: the one language found with call-site marks and a never-changed error, which exempted arguments and then softened

- Commit *"do not allow declaring a mutable variable if it's never
  modified"*, condition `... && !var.is_arg && ...`, message *"`$var.name` is
  declared mutable, but it was never changed"*:
  https://github.com/vlang/v/commit/9ccd3bde0134911de88081c9e3aef098070ea5ae
  (verified; date not shown: **unverified**, a search summary says January
  2020).
- Then *"make the unchanged mutable variable error a warning in non-prod
  builds"*, `p.warn_or_error`, with carve-outs `var.typ != 'T*' && p.mod !=
  'ui' && var.typ != 'App*'` in the context line:
  https://github.com/vlang/v/commit/126289c19bb2ff5687817d201ca35aa311c5d9b2
  (verified; date not shown). V marks mutable arguments at the call
  (*"mutable args have to be marked on call"*, quoted by a search summary of
  https://docs.vlang.io/functions-2.html, not fetched: **unverified**).

### Pascal and Oberon: Wirth removed the reason for an unwritten `VAR` instead of refusing it

- Kernighan, 1981-04-02: Pascal *"arrays are passed by value by default"* and
  *"every array parameter is declared 'var' by the programmer more or less
  without thinking"*: https://www.lysator.liu.se/c/bwk-on-pascal.html
  (verified; the fragments read do not name copy cost as the reason).
- Oberon-07, revision 1.10.2013 / 3.5.2016, § 9.1: *"If a value parameter is
  structured (of array or record type), no assignment to it or to its elements
  are permitted"*, so the compiler may pass it by address:
  https://www.miasap.se/obnc/oberon-report.html (verified; the
  pass-by-address consequence is my reading, not the report's words).
- What this asks of Heroes, as a question for the compiler-engineer, unrun by
  me: is any `@` in `selfhost/` there to avoid a copy rather than to write? A
  rule refusing it would then be paid for in copies, and Wirth's answer was to
  make the value parameter cheap, not to keep the `VAR`.

### C lineage: clang-tidy `readability-non-const-parameter`

- *"finds function parameters of a pointer type that could be changed to point
  to a constant type"*, and declines where a write goes through a pointer the
  parameter holds (*"making *p const could be misleading"*):
  https://clang.llvm.org/extra/clang-tidy/checks/readability/non-const-parameter.html
  (verified). An opt-in check, not a compiler diagnostic.

## What the record shows, in one table

| system | diagnostic | level | softened? |
|---|---|---|---|
| C# `out` | CS0177, unassigned | error (soundness) | none found |
| C# `ref` | none, by design | none | n/a |
| GNAT `in out` | `-gnatwk` *mode could be IN* | warning, off by default | narrowed the same week (2007) |
| SPARK `in out` | flow warning | warning | none found |
| gfortran/ifx/nagfor `intent(out)` | unset | warning | gfortran narrowed for default-initialised derived types (2009) |
| Fortran `intent(inout)` | none found | | |
| clippy `&mut` | `needless_pass_by_ref_mut` | warn, then allow | within one release (2023) |
| Swift `inout` | declined | none | open since 2019 |
| Zig `*T` | none found; unused params and never-mutated locals are errors | | |
| Go | no warnings; vet none | | |
| V `mut` | never-changed error skipped arguments | error, then warning outside `-prod` | yes |

**No language or tool I found makes an in-out parameter the callee never
writes a compile error.** That is a negative claim, so it goes out as a
question: the searches were the systems above plus Mojo, Hylo and Nim (three
searches, none naming such a diagnostic).

## verdict

**approve** (advisory), for refusing the `@` parameter nothing writes, under
the conditions below.

## argument

Every attempt met the same false-positive classes: a signature forced from
outside (a function value, a generic actual, a trait, a protocol), closures and
async, raw pointers and FFI, conditional compilation, exported API. Spec § 9
and § 3 remove the first three by construction (no anonymous functions, no
traits, and `TypeArg` has no `@`, which the compiler-engineer should confirm by
running it), and § 3 removes ReSharper's reported aliasing reason. What remains
is the C boundary and the exported function, the issue's own two carve-outs. Go
and Zig show a language without warnings refusing the neighbours as errors and
keeping them. C#'s CS0177 is the wrong precedent: it guards soundness, not a
false promise.

## condition

My reading changes if: Heroes lets a function with an `@` parameter be a value
or a generic's argument (then GNAT's 2007 suppression is owed); a write through
an `extern` pointer is not counted as a write (clippy's #12905 shape); Heroes
has or gains platform-conditional bodies (clippy #11185; Zig's syntactic
answer); the fix is `certain` for a function a package exports (gopls and
clippy both refuse that); or `@` is used in `selfhost/` to avoid copies
(Kernighan's Pascal, Wirth's Oberon-07 answer). And if any source shows an
error-level version of this rule that was reverted, that outweighs everything
above.

## Routes the record supports

- **R1, refuse, `certain` only over the whole compiled set, C writes counted:**
  supported, provided the conditions above hold. The fix's multi-site edit has
  a precedent only as a user-invoked refactoring (gopls), never as an
  automatic one, so `guess` wherever a caller may be outside the set.
- **R2, accept as today:** what C# and Swift do for this exact shape, and, as
  far as was found, Fortran and Zig. The reasons on record are Swift's protocol
  requirement and missing silencer and ReSharper's aliasing (unverified);
  Heroes has neither a protocol nor aliasing among owned values (spec § 3,
  § 9), so the record gives R2 no reason that applies here.
- **R3, a warning or lint:** what GNAT, SPARK and clippy did, and clippy's
  warning lasted one release. Whether Heroes has a warning channel for it to
  land in is the spec-warden's question; I found none in the sections read
  (§ 3, § 5, § 9), and the project's thesis is a compile error per mistake.

**Prediction to score**: if R1 lands, no defect filed against the rule in its
first two milestones will be a closure, trait, async or function-value false
positive (Heroes has none of those forms); any false positive filed will be at
the C boundary or on a library function a package exports. Scored by reading
`issues/` for defects naming the rule.
