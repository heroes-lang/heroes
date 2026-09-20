# Panel 170 — historian

**Seat:** historian. **Advisory, no veto.** Every claim carries a source.
**Repository facts are taken from the shared brief and were NOT re-run by this
seat** — it had no shell. Route note: *Gli eroi del codice* pointed this seat at
the Apple/LLVM and Cyclone rooms; nothing below rests on it and it is cited
nowhere as evidence.

**Written out by the coordinator**, verbatim from the seat's report, because this
seat has no write tool in this configuration.

## verdict

**approve, with one correction to the proposal's default and one to its
definition.**

The mark has ancestry, it works, and the asymmetry the spec-warden objects to is
universal and openly documented by everyone who ships one. But two things in the
proposal as briefed are departures from precedent that look accidental rather
than deliberate:

1. **The polarity of the default.** Swift is the closest precedent and Swift's
   **foreign** default is the *opposite* of the one proposed. That, not the mark,
   is where the spec-warden's objection is answered.
2. **Whether freeing counts as retention.** The mark the proposal most resembles,
   Clang's `noescape`, explicitly says it does **not**. Under that definition
   defect 070 stays open.

## A. The exact shape — a mark on the callee's declaration, refused at the caller

### 1. Swift `@escaping` + the Clang importer. VERIFIED. This is the precedent.

SE-0103 *"Make non-escaping closures the default"*, author Trent Nadeau, review
manager Chris Lattner, status **Implemented (Swift 3.0)**; Swift 3.0 shipped
2016-09-13. The clause the sitting needs is in the proposal's own § *Imported
C/Objective-C APIs*, verbatim:

> "Per the Core Team, most Cocoa closure/block parameters are escaping (e.g.,
> delegates). As such the Clang importer will automatically add the `@escaping`
> annotation to closure/block parameters encountered in imported Objective-C
> APIs **unless they are explicitly marked with the Clang `((noescape))`
> attribute.** This will also be done with imported C APIs with function pointer
> or block parameters."

So: the mark lives in the **C header**, on the **callee's parameter**, and the
refusal happens in the **Swift caller**, at compile time. **That is panel 170's
shape, shipped for ten years.**

**What the reversal bought.** Lattner's rationale, quoted in the proposal: most
pure-Swift algorithms *"are naturally noescape"*, so the default removes
boilerplate; the compiler *"can provide a fixit that suggests adding
`@escaping`"*; and *"noescape closures have also always been the preferred
default, since they eliminate a class of retain cycle issues."*
**What it cost:** a source break — *"Existing code using the `@noescape`
attribute will need to be migrated to remove the attribute"* — and an escape
hatch that is **dynamically checked**, `withoutActuallyEscaping`, of which the
proposal says *"The helper should verify that the closure has not actually
escaped and trap if it does."*
**Collateral:** SE-0073 `@noescape(once)` was **rejected for Swift 3**, partly
because the surface syntax and the default were still unsettled — *verified-weak*,
read via search summary rather than fetched in full.

**THE LESSON, AND IT IS THE DEFAULT, NOT THE MARK.** Swift enforces `@escaping`
**on both sides when it owns both sides**: the caller cannot pass a non-escaping
value where escaping is required, *and* the callee's body is checked. At the C
boundary Swift keeps only the caller half **and flips the default to
pessimistic**. Optimistic where it can read the body; pessimistic where it
cannot. **Panel 170's proposal is the optimistic default applied to the one
region Swift refused to apply it to.**

### 2. Clang `__attribute__((ownership_holds))`. VERIFIED. Exact semantics, analyser-only enforcement.

Clang Static Analyzer source annotations, verbatim: *"Use this attribute to mark
functions that **take ownership of memory and will deallocate it at some
unspecified point in the future.** Takes two arguments: the type of the
allocation … and the index of the parameter that is being held."* Siblings
`ownership_returns` and `ownership_takes`. The doc distinguishes them: *"using
taken memory is a use-after-free error, while using held memory is assumed to be
legitimate."*

**What happened:** it has existed for over a decade and was only *documented* in
January 2025 (LLVM PR #121759). It is enforced by the analyser, never by the
compiler, and is not on by default. **This is the literal "C keeps it" mark, and
nobody promoted it to an error.**

### 3. Microsoft SAL `__drv_aliasesMem`. PARTIALLY VERIFIED.

Meaning per secondary sources: applied to a parameter whose pointer the function
*saves*, so Code Analysis stops reporting a leak; mutually exclusive with
`__drv_freesMem`. The Microsoft *Understanding SAL* page confirms SAL's model and
that *"Microsoft public headers are already annotated"*, but **does not define
this annotation**. Searched: `__drv_aliasesMem`, "SAL aliasesMem", "SAL 2
annotations for Windows drivers". **Unverified on a primary source.**

### 4. GCC 11 `__attribute__((malloc(deallocator[, ptr-index])))` + `-Wmismatched-dealloc`. VERIFIED. This is the answer to defect 072.

GCC 11.1 released 2021-04-27. Per the GCC 11 changes page, the malloc attribute
*"has been extended so that it can be used to identify allocator/deallocator API
pairs"*, and `-Wmismatched-dealloc` is **enabled by default** and *"warns about
calls to deallocation functions with pointers returned from mismatched allocation
functions"*. GCC's worked example is `popen`/`fclose`.

**A declaration-site mark, diagnosed at the call site, in a production C
compiler, on by default, for exactly 072's shape** — and a *separate* mechanism
from any escape or retention annotation. **Precedent says two mechanisms, not
one.**

### 5. Fortran `ASYNCHRONOUS` via MPI-3. VERIFIED. A retention mark that binds the caller's compiler.

MPI-3.1 §17: in `mpi_f08`, nonblocking routines declare their choice buffer dummy
argument `ASYNCHRONOUS`, which *"informs the compiler that any statement in the
scoping unit may be executed while the buffer is affected by a pending
asynchronous … communication"* and *"protects the buffer accesses from
optimizations through code movements across routine calls."*

**And the humility worth copying:** MPI ships a compile-time constant,
`MPI_ASYNC_PROTECTS_NONBLOCKING`, *"set to `.TRUE.` if the ASYNCHRONOUS attribute
was added to the choice buffer arguments of all nonblocking interfaces **and** the
underlying Fortran compiler supports the ASYNCHRONOUS attribute … otherwise it is
set to `.FALSE.`"* — **a standard that states in-band whether its own guarantee
is in force.**

## B. Marks enforced on both sides — and why that does not transfer

**6. Hylo `let` / `inout` / `sink` / `set`.** VERIFIED-WEAK. *"The `let`
convention does not transfer ownership of the argument to the callee, meaning
that without first copying it, a `let` parameter can't be returned, or stored
anywhere that outlives the call."* `sink` *"indicates a transfer of ownership"*.
Same declaration-site vocabulary Heroes is reaching for — **and the compiler
checks the callee body, because the callee is Hylo.**

**7. C# `scoped` / `[UnscopedRef]`** (C# 11, 2022). PARTIALLY VERIFIED. Both
sides are C#.

**8. Nim `sink` / `lent`** under ARC/ORC. VERIFIED-WEAK. Native move semantics,
not an FFI mechanism.

**9. Cyclone regions** (PLDI 2002). VERIFIED-WEAK. Porting legacy C required
altering ~8% of the code, ~6% region annotations. The ancestor of the family —
and **its annotation burden was measured and reported, which is the standard this
sitting should hold itself to.**

## C. The sentence this sitting asked for — languages that say out loud that nobody audits the mark

All four verified.

**Rust, RFC 3484 "unsafe extern blocks" (Rust 2024) — the best quote, because it
is about a foreign declaration:**

> "When we declare the signature of items within `extern` blocks, we are
> asserting to the compiler that these declarations are correct. **The compiler
> cannot itself verify these assertions.** If the signatures we declare are in
> fact not correct, then using these items may result in undefined behavior."

and the resolution it drew:

> "It's *unreasonable* to expect the *caller* … to have to prove that the
> signature is valid. Instead, it's the responsibility of the person writing the
> `extern` block to ensure the correctness of all signatures within."
> "Once unsafely declared, a `safe` item within an `unsafe extern` block may be
> used directly from safe Rust code."

**Rust's answer to the asymmetry is not to reject the mark; it is to make the act
of declaring the foreign signature itself the attributable unsafe step, after
which callers may be safe.** Heroes' FFI binding is structurally the same object.

**Clang, `AttrDocs.td` for `noescape`** (verified twice, LLVM PR #117344 and the
LLVM RFC):

> "`noescape` placed on a function parameter is used to inform the compiler that
> the pointer cannot escape … **Users are responsible for making sure parameters
> annotated with `noescape` do not actually escape. Calling `free()` on such a
> parameter does not constitute an escape.**"

and the same RFC states the interop consequence in words that read like defect
070's docket entry: *"a parameter with `noescape` can be passed from a safe
language without violating lifetime safety, **but only if that function does not
free the memory**."*

**D, language spec, § Functions:** *"scope escape analysis is only done for
`@safe` functions"* — *"**For other functions scope semantics must be manually
enforced.**"* — and the check is still gated behind `-preview=dip1000`.

**Java FFM, `Linker`, § Safety considerations:** *"Creating a downcall method
handle is intrinsically unsafe … As a consequence, **the linker runtime cannot
validate linkage requests.**"*

## D. What everyone does instead of auditing the mark: a dynamic check

**Go. VERIFIED, and the strongest counter-proposal on the table.** `cmd/cgo`,
§ Passing pointers: *"**C code may not keep a copy of a Go pointer after the call
returns**, unless the memory it points to is pinned with `runtime.Pinner`"* and
the Pinner remains active. Enforcement is dynamic and partial:
`GODEBUG=cgocheck=1` is the default, `cgocheck=0` disables it, and
`GOEXPERIMENT=cgocheck2` gives *"complete checking of pointer handling, at some
cost in run time."* Go 1.21 added `runtime.Pinner`, which *"allows C memory to
safely retain that Go pointer even after the cgo call returns, provided the
object remains pinned."*

**Go states Heroes' § 13 rule almost word for word, and answers the retention
case with a caller-side object with an explicit lifetime — no mark on the C
declaration at all.** Heroes' existing `acquires` live set is the same family.

**Java FFM `Linker.Option.critical`.** VERIFIED. *"valid for the duration of the
function call"*, stated by the caller and trusted.

**Clang `-fbounds-safety`.** VERIFIED. *"If a bounds check fails, the program
will deterministically trap"*. Note for Heroes: `counted_by` is already in
`spec § 13`'s grammar, and upstream the same word is an author's promise Apple
chose to back with a **trap**, not with trust.

**Swift.** `withoutActuallyEscaping` — a runtime trap.

## E. The negative — was a retention mark ever shipped and withdrawn?

**No foreign-retention mark found withdrawn.** Vocabulary searched:
*withdrawn / removed / deprecated* against `noescape`, `@escaping`,
`ownership_holds`, `__drv_aliasesMem`, `cf_consumed`/`ns_consumed`, D's `scope`,
`lifetimebound`, `restrict`, "escape annotation FFI". **If a seat knows of one,
this finding is the thing to attack.**

Three near-misses:

- **C++ `[[carries_dependency]]`. VERIFIED.** Standardised in C++11. P0371R1
  (2016) recorded that *"all current compilers essentially map it to
  `memory_order_acquire`"* and that the specification *"requires numerous
  `[[carries_dependency]]` annotations throughout code"* with *"no performance
  advantage"*. P3475R2 deprecates for C++26, and **LLVM PR #219912 removed it
  from Clang on 2026-09-01** — nineteen days before this sitting — because the
  implementation *"never did anything with it"*. **Not a retention mark, so an
  analogy and flagged as one.** But its failure mode is the spec-warden's
  objection at scale: **an annotation that only pays off when written on every
  link in the chain loses its guarantee at the first unannotated boundary, and
  then nobody writes it.** Fifteen years from standard to removal.
- **Swift `@noescape`. VERIFIED.** The *mark* was withdrawn in Swift 3; its
  *meaning* became the default. **A mark retired by inverting the default is the
  healthiest withdrawal in this file.**
- **D `scope`. VERIFIED.** Specified long before it was enforced, and still
  enforced only under `@safe` plus a preview switch.

## F. Correction to the record, panel 169

This seat's FFM finding was carried further in that synthesis than it was taken.
Stated precisely: FFM's `IllegalStateException` is about **temporal validity of a
memory segment's scope** — access after the owning arena has closed, or from the
wrong thread for a confined arena. It is **not** an exclusivity or aliasing
check, so it is not defect 068's shape. What FFM *does* support is the § Safety
considerations sentence: **the linker cannot validate linkage requests at all.**

## G. Two findings that bear directly on the four questions

**On question 1, what the mark is.** Every enforced-at-the-caller mark in this
file is stated in the **negative polarity** — `noescape`, `scoped`, `let`,
`@escaping`'s absence. The **positive** polarity, *this parameter is retained*,
exists in exactly two places found, `ownership_holds` and `__drv_aliasesMem`, and
**both live in static analysers, neither in a compiler.** Not an argument against
`keeps`; a note that **Heroes would be the first to make the positive polarity a
hard compiler refusal, and first is a cost the sitting should price rather than
discover.**

**On question 2, the load-bearing definitional trap.** Clang's `noescape` says in
its own documentation that *"Calling `free()` on such a parameter does not
constitute an escape"*. **A Heroes mark modelled on escape analysis would leave
defect 070 outside its own definition.** Whatever word is chosen, `spec § 13`
must say in one sentence whether freeing is retention. **Precedent shows that if
it does not say, the answer defaults to "no", and the defect stays open with a
green `check`.**

**On question 3, the unmarked case.** The symmetry with `owned`, `consumes` and
`acquires` is real and every ecosystem here relies on it. But the symmetry does
not by itself answer the objection, because **precedent says the mark's value is
set by the default it sits against, not by the mark.** Swift is the only
precedent that had to choose a default for *foreign* parameters and it chose
**pessimistic**. If Heroes' unmarked foreign pointer parameter keeps meaning *a
lend is fine*, then in all three of 066's reproducers the outcome is unchanged
and the sitting will have shipped a word for the case that was already safe.

**On question 4, 072.** No, and GCC 11 is the evidence: allocator/deallocator
pairing is a separate declaration-site mechanism from retention, shipped
separately, diagnosed separately, on by default. **Two mechanisms.**

## argument, 120 words

Swift is the precedent, and the lesson is the default, not the mark. Swift
enforces `@escaping` on both sides where it owns both; at the C boundary it keeps
the caller half and **assumes every imported block escapes unless the header says
`noescape`**. Heroes proposes the optimistic default in the one place Swift
refused it, so the spec-warden's objection survives the mark. The asymmetry
itself is acceptable: Rust, Clang, D and Java all say in their own documentation
that nobody audits the foreign side, and Rust's answer — make declaring the
binding the attributable unsafe act — fits Heroes. Two departures need deciding
out loud: the default's polarity, and whether `free()` counts as retention. Clang
says it does not; defect 070 says it must.

## condition — what would change this reading

1. **A compiler, not an analyser, that makes the positive polarity a hard error
   at the caller.** Only `ownership_holds` and `__drv_aliasesMem` were found, both
   analyser-only.
2. **A language that took the optimistic default at a foreign boundary and
   measured that it was enough.** Swift went pessimistic; Go went pessimistic plus
   dynamic checks; FFM declared the problem out of scope.
3. **Any shipped-and-withdrawn foreign retention mark.**
4. **A primary Microsoft page defining `__drv_aliasesMem`.**
5. **Evidence that Heroes' existing `borrows` already carries the freeing case.**
   This seat did not run the compiler and cannot say whether 070 is a missing word
   or a missing refusal on a word that already parses.

## Sources

SE-0103 · the SE-0103 acceptance announcement · SE-0073 · Swift 3.0 release ·
Swift C++ interop safety docs · LLVM Discourse RFC on `noescape` semantics ·
LLVM PR #117344 · clang-tidy `bugprone-no-escape` · Clang Static Analyzer source
annotations · LLVM PR #121759 · Clang `-fbounds-safety` docs · GCC 11 changes ·
the GCC 11.1 announcement · OpenSSF compiler-annotations guide · Rust RFC 3484 ·
the D specification § Functions · the D blog on DIP1000 part 2 · Go `cmd/cgo` ·
Go 1.21 release notes · `java.lang.foreign.Linker` (JDK 24) · `Linker.Option` ·
Java restricted methods · MPI-3.1 nodes 408 and 427 · Microsoft *Understanding
SAL* · an OSR thread on `__drv_aliasesMem` (secondary) · P0371R1 · P3475R2 ·
LLVM PR #219912 · the Hylo language tour · the Cyclone regions paper (PLDI 2002)
· an endjin article on C# 11 `scoped` · the Nim destructors manual · Clang ARC.
