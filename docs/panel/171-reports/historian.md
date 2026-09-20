# Panel 171 — historian

**Seat:** historian. **Advisory, no veto.** Every claim carries a source and a
mark: **verified** (the primary page or file was fetched and the words quoted),
**verified-weak** (read through a search engine's summary), **unverified**, or
**unrun** (needs a shell this seat does not have). Repository facts are the
shared brief's and were not re-run here. Route note: *Gli eroi del codice*
pointed this seat at the Apple, Bell Labs and Digital Mars rooms; nothing below
rests on it and it is cited nowhere as evidence.

**Written out by the coordinator**, verbatim from the seat's report, because this
seat has no write tool in this configuration.

**One correction to the brief before anything else.** The brief says Clang's
`noescape` semantics *"were revised in 2025"*. The RFC *Updating the semantics
of the noescape attribute* was posted in **March 2026** and partially approved by
the Clang Area Team on **3 July 2026** (verified, § D.1). What happened in 2025
was one layer down: LLVM IR's `nocapture` was renamed `captures(none)` on
29 January 2025 (verified, § D.2). The year in the brief is the coordinator's and
it is wrong by one.

## verdict

**approve** (advisory), with three findings the sitting should carry into the
word, the rule and the diagnostic.

## A. Question 1: the word, on whose declaration, written by whom

Sorted by Heroes' shape — **a word in the binding file, written by the binding
author about a library they did not write, read at the caller**:

### A.1 D, druntime `core.stdc` and `core.sys.posix`: `scope`. VERIFIED. This is the closest precedent, and it is exact.

The D runtime's binding of libc is written by the D team in D files, and it reads
today:

    char* getenv(scope const char* name);        // core/stdc/stdlib.d
    double atof(scope const char* nptr);
    int atoi(scope const char* nptr);
    int system(scope const char* string);
    FILE* fopen(scope const char* filename, scope const char* mode);   // core/stdc/stdio.d
    int fputs(scope const char* s, FILE* stream);

and, **unmarked**, the functions that keep what they are handed:

    int atexit(void function() func);
    int setvbuf(FILE* stream, char* buf, int mode, size_t size);
    void setbuf(FILE* stream, char* buf);
    int putenv(char*);                            // core/sys/posix/stdlib.d, glibc branch

(`qsort`, `bsearch`, `free`, `malloc`, `tmpnam`, `fgets`, `mkstemp` are also
unmarked; the binding author marked only what they were sure of.) Sources: the
raw `stdlib.d`, `stdio.d` and posix `stdlib.d` in `dlang/dmd`'s druntime.

What the word means, from the D spec's storage-class table, verbatim: *"The
parameter must not escape the function call (e.g. by being assigned to a global
variable). Ignored for any parameter that is not a reference type."* And the
enforcement caveat, verbatim: *"scope escape analysis is only done for @safe
functions. For other functions scope semantics must be manually enforced."* and
*"Note: @safe escape analysis is only done with the -preview=dip1000 switch."*
**Negative polarity; the unmarked parameter is the one the compiler assumes may
store.** The D blog states the three cases: no attribute, *the function might
store the address of the argument somewhere*; `scope`, the address is not
stored; `return scope`, not stored other than in the return value
(verified-weak).

**When `scope` reached these bindings: unverified.** Vocabulary searched:
*druntime core.stdc scope dip1000 annotate pull request*, *changelog core.stdc
scope*. **Renamed? Unverified**, not searched exhaustively.

### A.2 Swift API notes: `NoEscape: true`. VERIFIED. Same shape, blocks only.

Clang's API notes exist for exactly Heroes' reason, verbatim: *"You have headers
you want to use, but you also want to add extra information to the API. You
don't want to put that information in the headers themselves, perhaps because
you want to keep them clean for other clients, or perhaps because they're from
some open source project and you don't want to modify them at all."* The
parameter key: *"NoEscape: Used only for block parameters. Equivalent to
`NS_NOESCAPE`."*

**So Swift's side-file word covers closures and not `const char*`.** For
pointers Swift has no word at all; it has a caller-side rule in the standard
library's own documentation, verbatim: *"The pointer created through implicit
bridging of an instance or of an array's elements is only valid during the
execution of the called function. Escaping the pointer to use after the
execution of the function is undefined behavior."* SR-2529, filed 2016-08-31,
asked the importer to keep `__attribute__((noescape))` on imported
function-pointer and block types; the page shows no resolution. **Swift's
pointer parameters are therefore Heroes' OLD default, and the hazard is
documented rather than refused.**

### A.3 Vala `.vapi` and `.metadata`: `owned`, positive polarity, optimistic default. VERIFIED. The counter-precedent.

The Vala binding guide, verbatim: *"All parameters are, by default, unowned,
unless marked with the `owned` keyword."* and *"If ownership semantics are not
correct, either a memory leak has been written or a double-free has been
written."* For gir-generated bindings the binding author writes the same facts
in a `.metadata` file: arguments `owned`, `unowned`, `scope` (*"Scope of the
delegate, in GIR terms"*).

**Renamed: yes.** Jürg Billeter, 2008-12-19: the `#` type modifier *"was very
unintuitive"*, replaced by `owned`, old syntax *"planned to be deprecated after
the release of Vala 0.5.4"*. **A one-character sigil for ownership lasted about
two years before it was replaced by an English word.** Note for the `borrows`
sub-question: Vala's `owned` carries a position-dependent default (parameters
unowned, results owned) and one meaning from the receiver's point of view, in
production since 2008. Heroes' `borrows` on a result and a hypothetical
`borrows` on a parameter would have the same receiver-relative reading.
Recorded as an observation, not a ranking.

### A.4 GObject Introspection: `(scope call)`, `(transfer none)`. VERIFIED.

Library-author side, negative polarity, optimistic defaults: scope `call` is the
default, *"Only valid for the duration of the call"*; in-parameters default to
`(transfer none)`. These live in the C source's doc comments, so they are the
library author's; Vala's `.metadata` is where the binding author overrides them.

### A.5 Cyclone: no word, a region variable, optimistic default. VERIFIED.

*"In function arguments, a fresh region variable is used."* A fresh region
variable on a parameter means the callee cannot store it anywhere longer-lived;
that reading is this seat's inference, not a quoted sentence. Cyclone 1.0
shipped 2006-05-08 and is no longer supported.

### A.6 Three caller-side lends with a documented hazard and no word. VERIFIED.

Haskell `withCString`: *"the pointer to the temporary storage must not be used
after this."* Nim manual: *"A Nim string is implicitly convertible to cstring
for convenience … The garbage collector does not consider a cstring to be a
root and may collect the underlying memory. For this reason, the implicit
conversion will be removed in future releases."* Rust `libc`:
`pub unsafe extern "C" fn getenv(s: *const c_char) -> *mut c_char`, no
lifetime, no doc.

**Tally on the default, at a foreign boundary, in a compiler:** pessimistic
(unmarked = may keep): Swift for blocks (2016), D under dip1000. Optimistic:
Vala, Cyclone, GI's `scope call`, and Swift itself for pointers. **The brief's
sentence that the positive polarity ships nowhere as a compiler error is
falsified by Vala's `owned` parameter**, which the Vala compiler enforces,
though as ownership transfer rather than retention in Heroes' sense; recorded so
the record is true.

## B. Question 2: what the pessimistic default cost Swift's binding authors

**B.1 Who was asked to pay. VERIFIED.** Chris Lattner for the core team,
2016-06-01: *"the core team did a quick study of the Cocoa APIs and found that
most closure/block parameters are escaping in practice. As such, the core team
feels that it isn't overly burdensome to ask that imported Objective-C APIs
annotate their semantically noescape block parameters with the clang
`__attribute__((noescape))` attribute."*

**B.2 The community had asked for the annotations first and was told it was not
the language's job. VERIFIED.** SE-0012 *Add @noescape to public library API*,
**Status: Rejected**. So the burden moved from the language to Apple's header
owners.

**B.3 What Apple did and did not publish. VERIFIED.** The macOS 10.12 Foundation
release notes carry one sentence: *"Introduction and adoption of NS_NOESCAPE to
mark blocks whose execution completes before the API the block is passed in as
an argument to."* **No count, no list, no period. The number the brief asks for
does not exist in any published source this seat found.** Vocabulary searched:
*NS_NOESCAPE introduced*, *CF_NOESCAPE*, *DISPATCH_NOESCAPE*, *noescape SDK
headers count*, *Swift 3 migration @escaping imported C complaint*. **Unrun, and
the coordinator can run it on this Mac:**

    grep -rlE 'NS_NOESCAPE|CF_NOESCAPE|DISPATCH_NOESCAPE|__attribute__\(\(__?noescape__?\)\)' \
      "$(xcrun --show-sdk-path)/System/Library/Frameworks" "$(xcrun --show-sdk-path)/usr/include" | wc -l

That counts headers today, not the 2016 delta; it is still the only number
available.

**B.4 One footprint that is dated. VERIFIED.** Upstream Clang's `noescape`
(D32210, landed 2017-09-19 as r313722) was reverted the same day: *"some of the
functions declared in the file do not match the ones in the SDK headers (which
are annotated with 'noescape')."* The fix, D38141, annotated `dispatch_sync`,
`dispatch_barrier_sync`, `dispatch_once`, `dispatch_apply`. **So by September
2017 Apple's shipped SDK already carried `noescape` on libdispatch, a year before
upstream Clang could parse it, and the first cost that surfaced was a
third-party re-declaration going out of step with the header.** That is the
mechanism to watch for in Heroes: **two declarations of one C function
disagreeing on the mark.**

**B.5 The escape hatch shipped a release later. VERIFIED.**
`withoutActuallyEscaping` is in the Swift 3.1 CHANGELOG, and the proposal says
*"The helper should verify that the closure has not actually escaped and trap if
it does."* Under a pessimistic default the pressure valve is a dynamic check, not
a second word.

**B.6 The D data point on cost, and why it only half transfers. VERIFIED.** D
Language Foundation meeting, December 2024: Átila Neves, *"It had been a disaster
trying to turn it on by default."*; Walter Bright on bootstrapping, *"We couldn't
seem to move to a more modern version of the compiler for bootstrapping."* D
2.101.0: scope semantics *"enforced in @safe code on pointers to stack memory, but
only as deprecation warnings"*. **What was a disaster was checking D function
BODIES with `return scope` inference. Heroes checks only the caller and never a
body, so the mechanism does not transfer; the part that does is the fixpoint
problem Walter names, which is the shared brief's own seed-first landing order.**

## C. Question 3: `getenv`, and what the C standard says about keeping

**C.1 `getenv` does not say it keeps, and it describes `name` as a lookup key.
VERIFIED, read from N1570 pp. 352-353.** 7.22.4.6 ¶2: *"The `getenv` function
searches an environment list, provided by the host environment, for a string
that matches the string pointed to by `name`."* ¶4: *"returns a pointer to a
string associated with the matched list member … may be overwritten by a
subsequent call to the `getenv` function."* POSIX says the same. **The standard
has no sentence either way about retaining `name`; the argument's role, a
string to be matched, is the only evidence, and D's binding authors read it the
same way (`scope`).** Not read this sitting: C11 §7.1.4 *Use of library
functions*, where a general clause would live if one existed.

**C.2 The standard DOES say it, in words, where a function keeps. VERIFIED. This
is the anchor the brief asked for.**

- `setvbuf` 7.21.5.6, footnote 273: *"The buffer has to have a lifetime at
  least as great as the open stream, so the stream should be closed before a
  buffer that has automatic storage duration is deallocated upon block exit."*
- `atexit` 7.22.4.2 ¶2: *"registers the function pointed to by `func`, to be
  called without arguments at normal program termination."*
- `strtok` 7.24.5.8 ¶2: *"The first call in the sequence has a non-null first
  argument; subsequent calls in the sequence have a null first argument."*
- POSIX `putenv`: *"the string pointed to by `string` shall become part of the
  environment, so altering the string shall change the environment."* and, under
  Application Usage, *"A potential error is to call `putenv()` with an automatic
  variable as the argument, then return from the calling function while `string`
  is still part of the environment."*

**So the standard's own vocabulary for *keeps* is a lifetime sentence per
function, and its vocabulary for *does not keep* is silence. That is the
polarity Heroes has now adopted: the marked case is the one somebody had to
write down.**

**C.3 A binding author getting it wrong, live, in the exact shape. VERIFIED,
with one inference flagged.** druntime's `core.sys.posix.stdlib` declares
`putenv` as `int putenv(char*);` for glibc and as
`int putenv(const scope char*);` in its `CRuntime_WASI` branch. wasi-libc's
`putenv.c` is musl's and stores the pointer without copying: `newenv[i] = s;`.
**The `.d` file says *does not keep* about a function whose implementation
keeps.** That a stack buffer would then be accepted under `@safe` is this
seat's inference and not something run. It is the strongest available argument
for what the shared brief's question 3 already suspected: **a `Fix` that adds
the mark to the declaration cannot be `certain`**, because the person adding it
is making the claim D's WASI binding made.

## D. Question 4: the negative

**D.1 No shipped binding-side does-not-keep mark was found withdrawn, and Clang
went the other way. VERIFIED.** The 2026 RFC's motivation: *"a parameter with
`noescape` can be passed from a safe language without violating lifetime
safety, but only if that function does not free the memory."* The Clang Area
Team on 2026-07-03 *"partially approved this RFC"*; the author reported *"one
function with `noescape` that frees the pointer"* in the codebases surveyed.
**After nine years the mark was tightened, not withdrawn, and the reason given
was interop with a safe language. Panel 170's definitional trap — freeing is not
an escape — has now been closed upstream too.**

**D.2 Renames found, none a withdrawal. VERIFIED.** LLVM IR `nocapture` to
`captures(none)`, 2025-01-29, *"intended to be essentially NFC"*. Swift
`@noescape` retired by inversion, 2016. Vala `#` to `owned`, 2008. Java FFM's
lend-scope object: `ResourceScope` (JDK 18) to `MemorySession` (JDK 19) to
`Arena` + `SegmentScope` (JDK 20) to `Arena` alone (JDK 21). **Three names in
three years for the object a caller uses to say how long a lend lasts. The
concept is stable; the noun is not, and every rename was toward a plainer
word.**

**D.3 A default flipped back: none found.** Vocabulary searched: *noescape
removed*, *noescape deprecated*, *scope parameter reverted*, *dip1000 enabled by
default reverted*, *implicit cstring conversion removed Nim*, *@escaping default
reverted*. Nim announced the removal of its implicit lend and has not executed
it. D's default enforcement was attempted and rolled back to warnings, which is
the nearest thing, and it concerned body checking.

## argument

Precedent says the binding-side word exists and works: D's druntime writes
`scope` on `getenv(scope const char* name)`, about a library D did not write,
read at the caller under `@safe`, and leaves `setvbuf`, `atexit` and `putenv`
unmarked, which keep. Swift's side-file `NoEscape` does the same for blocks only;
for `const char*` Swift has no word and documents the hazard. No count of headers
Apple annotated was ever published; the core team called the burden not overly
burdensome and left it to header owners. The standard itself distinguishes:
`getenv` searches for a match, `setvbuf`'s footnote says the buffer outlives the
stream, POSIX `putenv` says the string becomes part of the environment. Nobody
withdrew a does-not-keep mark; Clang tightened its own in 2026.

## condition

1. **A published count or migration thread showing SE-0103's foreign default
   cost Swift's binding authors materially.** None found; the unrun SDK grep in
   B.3 is the only number on offer.
2. **Evidence that Swift's importer honours `noescape` on `const char*`
   parameters.** SR-2529 shows no resolution.
3. **The date `scope` reached druntime's C bindings and any breakage it
   caused.**
4. **Any binding-side does-not-keep mark shipped and withdrawn**, under the
   vocabulary in D.3. Finding one would turn this approve into an object.
5. **A sentence in C11 §7.1.4 about argument retention.** Unread.

## Sources

SE-0103 and its acceptance · Lattner, core team on imported ObjC, 2016-06-01 ·
SE-0012 · Foundation release notes, macOS 10.12 · Clang API notes ·
`UnsafePointer.swift` · SR-2529 · D32210, its revert, D38141, LLVM Weekly #195 ·
the Swift 3.1 CHANGELOG · druntime `stdlib.d`, `stdio.d`, posix `stdlib.d` · the
D spec § Functions · the D 2.101.0 changelog · the D blog on DIP1000 · the D
Foundation meeting of December 2024 · wasi-libc `putenv.c` · the Vala vapi guide
and tutorial § Ownership · the Vala ownership syntax change of 2008-12-19 · the
Vala GIR metadata format · GI annotations · the Cyclone FAQ · `Foreign.C.String`
· the Nim manual and its 1.4.2 mirror · `libc::getenv` · N1570 · POSIX `getenv`
and `putenv` · cppreference `getenv` and `setvbuf` · the LLVM RFC on `noescape`,
2026, both pages · PR #123181 · JEPs 419, 424, 434, 442 · Clang ARC.
