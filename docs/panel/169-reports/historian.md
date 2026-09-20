# Panel 169 — historian

**Seat:** historian (advisory, no veto). Every claim checked by web search on
2026-09-20. Claims that could not be sourced are marked **unverified** in those
words. Where a search returned nothing the seat says so and names the vocabulary
searched.

**Written out by the coordinator**, verbatim from the seat's report, because this
seat has no write tool in this configuration — the shape panels 143, 144 and 145
recorded.

*Route note, not evidence:* the project author's *Heroes of code* is what sent
this seat back to the Zurich room (Wirth's `SYSTEM` module) and to Cyclone. The
citations below are the Oberon Report and the primary sources; the book is never
the citation.

## verdict

**approve, with one correction.** Approve R2 on precedent. Object to 068 having
been filed as unsolvable on the panel 167 finding — that finding was about the
**callee**, and 068's corrupting line is the caller's. The correction: the
precedent for R2 is excellent for *references* and **does not exist for raw
pointers**, because every surveyed language exempts them by name. That makes R2 a
deliberate departure, not a port, and the sitting should record it as one.

## The five things, compressed

1. **The brief's reading is correct, but the rule has a different name.** 068 is
   not escape analysis — it is **exclusivity**: no write to a location while a
   reference into it is live. Exclusivity ships **statically and caller-side** in
   Rust (E0506), Swift (SE-0176, Swift 4.0), Ada (limited types), Austral, Nim.
   **None was ever withdrawn.** The panel 167 finding was about the *callee*;
   applying it to 068 is a category error.
2. **The correction: every surveyed language exempts raw pointers by name.**
   SE-0176 verbatim: *"Unsafe pointers will not use any active enforcement."*
   Rust's borrow checker does not track `*mut`. So R2 is a **deliberate
   departure**, justified because in Rust and Swift the raw pointer *is* the
   escape hatch and in Heroes `.ptr()` is the only road to C.
3. **FFM is the sharpest precedent and it confirms panel 168's split exactly.**
   Java enforces the caller-side half at run time (`IllegalStateException` after
   `Arena.close()`) and refuses the foreign half outright — javadoc: the runtime
   *"has no insight into the lifetime intended for said region of memory by the
   foreign function."* Cost: 10 JEPs, roughly five years, lifetime model
   redesigned twice.
4. **The negative the brief asked for: no withdrawal was found.** Nobody shipped
   R2's rule and removed it. What they did instead: Rust *relaxed the duration*
   (NLL), Swift *strengthened* it (warning to error, 4.2), GCC *added an escape
   hatch* (`[[gnu::no_dangling]]`), Linux *disabled the consumer-side warning*
   (Torvalds, `-Wdangling-pointer`).
5. **Route ranking: R2 best precedent, R1 second** (run-time everywhere except
   Ada), **R3 precedented as a marker and not as a proof** (Oberon `SYSTEM`,
   Fortran `TARGET`), **R4 still none.**

## Q1 — is the brief's reading of the 167 survey correct?

**Yes, but the rule 068 needs has a different name than the one 167 was about.**
167 surveyed *escape analysis across a foreign boundary*. 068 needs
**exclusivity**: no write to a location while a reference into it is live. That
is a cheaper rule and a different literature, and it ships.

### 1. Rust — `E0506`, "cannot assign to a borrowed value". VERIFIED.

The error-code documentation's own example is 068's shape with the C removed:

> `let fancy_ref = &fancy_num;`
> `fancy_num = FancyNum { num: 6 };`
> `// error: cannot assign to fancy_num because it is borrowed`

Explanation verbatim: *"Because `fancy_ref` still holds a reference to
`fancy_num`, `fancy_num` can't be assigned to a new value as it would invalidate
the reference."* On by default, no feature flag. (That it has existed since 1.0,
2015-05-15: **unverified** — the seat read the current error index, not the 1.0
one.) Source: <https://doc.rust-lang.org/error_codes/E0506.html>

**THE CAVEAT THAT MATTERS, AND IT IS THE CORRECTION THIS SITTING NEEDS.
VERIFIED.** Rust does **not** apply this to raw pointers. *"just like the borrow
checker does not do any tracking for raw pointers, Stacked Borrows also makes no
attempt to distinguish different raw pointers pointing to the same thing: raw
pointers are untagged."* Creating `*mut T` from `&mut T` and then reassigning the
root compiles; **Miri in strict mode** flags it as UB. So Rust's answer to 068's
exact shape — a *pointer*, not a reference — is a **run-time interpreter check**,
not a static one. Sources:
<https://rust-unofficial.github.io/too-many-lists/fifth-stacked-borrows.html> ·
<https://www.ralfj.de/blog/2018/08/07/stacked-borrows.html> ·
<https://www.ralfj.de/blog/2023/06/02/tree-borrows.html>

### 2. Swift — SE-0176, exclusivity. Static, caller-side, shipped 2017, strengthened not withdrawn. VERIFIED.

Status line: *"Implemented (Swift 4.0)"*. Enforcement split, verbatim:

> *"Local variables, inout parameters, and struct properties can generally
> enforce the rule statically."*
> *"Class properties and global variables will have to enforce the rule
> dynamically."*
> **"Unsafe pointers will not use any active enforcement; it is the programmer's
> responsibility to follow the rule."**

That last sentence is Swift declining R2 for exactly the pointer case, in
writing, in 2017. The proposal also concedes the over-approximation in its own
words: the rule is *"a conservative over-approximation: that is, there is code
which does not violate the NRR which will be considered ill-formed under the
NPCR."* And it accepted the cost: *"exclusivity will eventually demand a source
break."*

The direction of travel was **toward** the rule, not away: Devin Coughlin,
**2018-05-15**, announced the overlapping-access **warning becomes an error in
Swift 4.2** — *"This means that projects with this warning will fail to build
with the Swift 4.2 compiler."* Reported false positives (BJ Homer, 2018-06-05,
protocol-typed variables) were triaged by Joe Groff as *"This is a bug. It's
fixed in the master and 4.2 branches"* — **fixed as bugs, not treated as grounds
for retreat.** Swift 5 then turned on run-time enforcement in Release builds by
default. Sources:
<https://github.com/apple/swift-evolution/blob/master/proposals/0176-enforce-exclusive-access-to-memory.md>
· <https://forums.swift.org/t/upgrading-exclusive-access-warning-to-be-an-error-in-swift-4-2/12704>
· <https://www.swift.org/blog/swift-5-exclusivity/>

*Partially verified:* that `-enforce-exclusivity=none` still keeps compile-time
enforcement (Xcode's "Compile-time Enforcement Only") appeared in a search
snippet attributed to swift.org but the seat did not re-read the sentence in the
fetched page body. **Treat as unverified.** What is verified is that
`-enforce-exclusivity=unchecked` disables the **run-time** checks and that doing
so *"may result in undefined behavior."*

### 3. Ada — limited types. The nuclear R2, static, since Ada 83. VERIFIED.

RM 7.5: *"If a view of the type makes it immutably limited, then no copying
(assignment) operations are ever available for objects of the type."* The
rationale text is the handle idiom in prose: *"A file name obtained from Open
acts as a kind of password; its internal properties are not known and clients of
the package cannot make copies of objects of that type."*

This is the only **compile-time** handle rule in the entire survey. Ada's answer
to *do not reassign the thing C is holding* is *the thing has no assignment
operation at all.* Source:
<http://www.ada-auth.org/standards/12rm/html/RM-7-5.html>

### 4. Austral — the borrow statement. Lexical, flow-insensitive, in a deliberately small language. VERIFIED.

*"For the duration of this block, the value `buf` is unusable, since it has been
borrowed."* And: *"Within the scope of the region statement, `R` is defined.
Outside the block, it isn't. That means you can't leak references."* The tutorial
gives no flow analysis and makes no claim to one. Sources:
<https://austral-lang.org/tutorial/borrowing> ·
<https://borretti.me/article/how-australs-linear-type-checker-works>

### 5. Hylo / mutable value semantics, and Cyclone

Carried unchanged from panel 167 and verified there on this same date: Hylo's
*"references are second-class: they are only created implicitly, at function
boundaries, and cannot be stored in variables or object fields"*
(<https://arxiv.org/abs/2106.12678>); Cyclone's region system statically prevents
escape and the project is dead (<http://cyclone.thelanguage.org/>). Neither
reaches a pointer held by C.

### 6. C++ — the lifetime profile, and it is the cautionary half. VERIFIED, with one secondary source.

P1179 (Sutter) has a partial MSVC static-analyzer implementation since 2018 and a
clang fork by Gehre and Horváth (`mgehre/llvm-project`) that was **never
upstreamed**. That the fork is *partial, not in mainline, last actively
maintained around 2020-2021* comes from a secondary blog and the repo
description: treat the *dates* as **unverified**; the fact that it is a fork
rather than mainline clang is verified by the repository's own self-description.
Sources: <https://github.com/mgehre/llvm-project> ·
<https://github.com/cplusplus/papers/issues/312> ·
<https://herbsutter.com/2018/09/20/lifetime-profile-v1-0-posted/>

**Answer to Q1, direct.** The reading is correct. The languages whose rule would
refuse 068's Heroes line are Rust (E0506), Swift (SE-0176), Ada (limited types),
Austral (borrow block), Nim (views). Four of the five are **static and
caller-side and cheap** — none needs a whole ownership system to state *"do not
assign to X while a reference into X is live"*; Rust and Austral get there via
ownership, Swift and Nim and Ada do not. **But all five exempt the raw-pointer
case, and Swift says so verbatim.** R2 is therefore a departure. Record it as
deliberate.

## Q2 — the smallest such rule that ever shipped, and what it over-refused

**The canonical answer is Rust's pre-NLL lexical borrow checker, and RFC 2094
documents the over-refusal in R2's exact shape. VERIFIED.** RFC 2094's Problem
case #1:

    let slice = &mut data[..];
    capitalize(slice);
    data.push('d'); // ERROR!

*"assigning a reference into a variable means that its lifetime must be as large
as the entire scope of that variable. In this case, that means the lifetime is
now extended all the way until the end of the block."* The RFC calls the
workaround *"artificial and also not an entirely obvious solution."*

**That program is R2's rule refusing a correct program.** Lend, call the
consumer, then mutate the root after the consumer is finished. If R2 is
whole-function and flow-insensitive, it refuses it.

What happened: Rust spent three years and an RFC replacing the over-approximation
with a liveness analysis over MIR. NLL became the default for the 2018 edition in
**Rust 1.31, 2018-12-06**, and reached the 2015 edition in **1.36 (2019)**, with
the newly surfaced errors first downgraded to warnings before being promoted.
Sources: <https://rust-lang.github.io/rfcs/2094-nll.html> ·
<https://blog.rust-lang.org/2018/12/06/Rust-1.31-and-rust-2018/> ·
<http://blog.pnkfx.org/blog/2019/06/26/breaking-news-non-lexical-lifetimes-arrives-for-everyone/>

**The smallest rule still shipping, and it names its own shape. VERIFIED.** Nim's
view types. The manual, verbatim: *"For the duration of the borrow operation, no
mutations to the borrowed locations may be performed except via the view that
borrowed from the location"* and **"The analysis is currently control flow
insensitive."** Since Nim **1.4 (2020-10-16)**, behind
`{.experimental: "views".}`, and **still experimental** in the current manual.
Its defect history includes issue #16132, *"The views feature is broken"* (opened
2020-11-25, filed against 1.5.1, milestoned 1.6.0, now closed).

**Reading:** a flow-insensitive borrow rule shipped, worked, and was never
withdrawn — and in six years never left the experimental flag either. Sources:
<https://nim-lang.org/docs/manual_experimental.html> ·
<https://nim-lang.org/blog/2020/10/16/version-140-released.html> ·
<https://github.com/nim-lang/Nim/issues/16132>

**What the over-refusal cost, when it was priced by consumers rather than
authors. VERIFIED.** GCC 12 shipped `-Wdangling-pointer`. Linus Torvalds disabled
it kernel-wide in commit **49beadbd47c2**, *"gcc-12: disable '-Wdangling-pointer'
warning for now"*: *"the gcc-12 implementation is not compatible with reality,
and results in false positives."* The two examples in the commit body are a
doubly linked list head on the stack, and deliberately stored addresses of locals
for stack traces. Torvalds calls the compiler's perspective *"entirely
reasonable"* and disables it anyway. (The postings read are the 5.15 and 5.18
stable backports; that the mainline release was **v5.19** is **unverified**.)

And the repair the ecosystem chose was not withdrawal but an **escape hatch**:
GCC 14 release notes, verbatim — *"-Wdangling-reference false positives have been
reduced. The warning does not warn about `std::span`-like classes; there is also
a new attribute `gnu::no_dangling` to suppress the warning."* Sources:
<https://lkml.iu.edu/hypermail/linux/kernel/2206.2/04076.html> ·
<https://gcc.gnu.org/gcc-14/changes.html>

**Answer to Q2, direct.** Yes, something that small shipped — twice. It
over-refused the *lend, use, then mutate the root* shape, which is RFC 2094's own
first example and is **two lines away from 068's reproducer**. The two historical
repairs are flow analysis (Rust, three years) and a named escape attribute (GCC
14, one release). Nobody removed the rule.

## Q3 — the handle route. Who else, and compile time or run time?

| system | what moved | when | enforced |
|---|---|---|---|
| .NET `SafeHandle` | `IntPtr` to handle object | .NET Framework 2.0 | **run time** |
| Python `PyCapsule` | `void*` (`PyCObject`) to a **named** capsule | Py 3.1 / 2.7 | **run time** |
| JNI `jobject` | opaque reference, never an address | JDK 1.1, 1997 | **run time** |
| Java FFM `Arena`/`MemorySegment` | pointer to scoped segment | JDK 22, 2024 | **run time** |
| Ada limited types | value to non-assignable value | Ada 83 | **compile time** |
| Oberon `SYSTEM` | unsafety to a name in the import list | 1990 | **compile time (visibility)** |
| Zig allocator passing | — | — | **unverified**, secondary sources only |
| OCaml custom blocks / registered roots | — | long-standing | run time (verified at 167) |

**.NET `SafeHandle` — this is the one that answers "did anybody move from a
pointer to a handle *because the pointer could not carry the rule*". YES, and the
docs say why. VERIFIED.**

> *"in some circumstances, finalizable objects can be reclaimed by garbage
> collection while executing a method within a platform invoke call. If a
> finalizer frees the handle passed to that platform invoke call, it could lead
> to handle corruption."*
> *"More critically, because Windows aggressively recycles handles, a handle
> could be recycled and point to another resource that might contain sensitive
> data. This is known as a recycle attack and can potentially corrupt data and be
> a security threat."*

And the mechanism is run-time refcounting wired into the call boundary:

> *"Platform invoke operations automatically increment the reference count of
> handles encapsulated by a `SafeHandle` and decrement them upon completion. This
> ensures that the handle will not be recycled or closed unexpectedly."*

**That is Heroes' `acquires`/`consumes` live set, twenty-one years earlier, at
the call rather than at exit.** Earliest moniker listed on the current page is
`netframework-2.0`. **No compile-time rule anywhere in it.** Source:
<https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.safehandle>

**Python `PyCapsule` — the second "pointer could not carry the rule" move.
VERIFIED.** PyCapsule replaced PyCObject in Python 3.1 and 2.7 because *"it
didn't permit distinguishing between valid CObjects, which allowed mismatched
CObjects to crash the interpreter, and some of its APIs relied on undefined
behavior in C."* The fix was a **name** attached to the pointer: *"PyCapsule
associates a name (char\*) to the capsule, instead of a pointer (void\*) in
PyCObject, and uses the name associated to the stored pointer in order to
retrieve the latter."* PyCObject was deprecated in 3.1/2.7 and **removed in
Python 3.3**.

**That is `unread_mark`'s note in Python's words**: *name the C type the pointer
stands for and mark that.* Sources:
<https://docs.python.org/3/c-api/capsule.html> ·
<https://py3c.readthedocs.io/en/latest/capsulethunk.html>

**Answer to Q3, direct.** The pointer-to-handle move is well attested and twice
documented with the reason Heroes has. **Every one of them enforces at run time
except Ada's limited types.** So R1's run-time half (the live set that already
ships) is the most replicated construct in the whole survey — unchanged from 167
— and R1's **new static half**, *a lend may not reach a parameter that retains*,
has precedent only as a *declaration*, never as a verification. That is 167's
headline finding, intact.

## Q4 — the FFM precedent. THIS IS THE SHARPEST ONE AND IT SETTLES THE SITTING'S FRAMING

**Is it the handle route under another name?** Partly, and it is stronger: the
lifetime lives on the **scope** (`Arena`) rather than on the object
(`MemorySegment`). The guarantee, verbatim from the `java.lang.foreign` package
summary:

> *"**Temporal safety**: to prevent a region of memory from being accessed after
> it has been deallocated (i.e. use-after-free), a segment is also validated
> (upon access) to make sure that the arena from which it has been obtained has
> not been closed."*
> *"Together, spatial and temporal safety ensure that each memory access
> operation either succeeds — and accesses a valid location within the region of
> memory backing the memory segment — or fails."*

Enforcement is **run time**. `Arena` javadoc: *"Closes this arena. If this method
completes normally, the arena scope is no longer alive, and all the memory
segments associated with it can no longer be accessed."* Access after close
throws `IllegalStateException`; closing a confined arena from a foreign thread
throws `WrongThreadException`. Sources:
<https://docs.oracle.com/en/java/javase/22/docs/api/java.base/java/lang/foreign/package-summary.html>
· <https://docs.oracle.com/en/java/javase/22/docs/api/java.base/java/lang/foreign/Arena.html>
· <https://openjdk.org/jeps/454> · <https://openjdk.org/jeps/442>

**What it cost the ecosystem. VERIFIED.** Ten JEPs across roughly five years, and
the **lifetime model was redesigned twice inside the preview window**:

| JEP | JDK | what |
|---|---|---|
| 370 | 14 | Foreign-Memory Access (incubator) |
| 383 | 15 | re-incubator |
| 393 | 16 | re-incubator |
| 389 | 16 | Foreign Linker (incubator) |
| 412 | 17 | FFM incubator |
| 419 | 18 | second incubator |
| 424 | 19 | first preview — `MemorySession` |
| 434 | 20 | second preview — `MemorySession` **split** into `Arena` + `SegmentScope` |
| 442 | 21 | third preview — lifetime management **re-centralised** in `Arena`, `SegmentScope` gone, `VaList` removed |
| 454 | 22 | final |

Plus an access-control regime on top: restricted methods warn by default,
`--enable-native-access` grants, and **JEP 472** (JDK 24) prepares to restrict
JNI the same way — *"In a future release, deny will become the default and allow
will be removed."* Plus a measured run-time cost the javadoc itself states: *"The
cost of providing this guarantee varies based on the number of threads… if an
arena is always created and closed by one thread… then ensuring correctness is
trivial. Conversely… much more complex."* Shared arenas use JEP 312 thread-local
handshakes and `Arena::close` is slower on shared than on confined (that *close
is slower on shared* is attested by the Panama design docs and the javadoc's cost
paragraph; the specific magnitude is **unverified**). Sources:
<https://openjdk.org/jeps/472> ·
<https://github.com/openjdk/panama-foreign/blob/foreign-memaccess+abi/doc/panama_memaccess.md>

**What it REFUSED to do — and this is the sentence for the synthesis. VERIFIED.**
`MemorySegment` javadoc, on pointers returned by or passed to native code:

> *"In addition to having no insight into the size of the region of memory
> backing a pointer returned from a foreign function, **it also has no insight
> into the lifetime intended for said region of memory by the foreign function
> that allocated it.**"*

And on the reverse direction, upcall stubs — the mechanism defect 069 is about:

> *"if the method handle associated with an upcall stub returns a memory segment,
> clients must ensure that this address cannot become invalid after the upcall is
> completed. **This can lead to unspecified behavior, and even JVM crashes.**"*
> *"`upcallStub` is a restricted method of the Java platform… Restricted methods
> are unsafe, and, if used incorrectly, might crash the JVM or result in memory
> corruption."*

And on re-attaching bounds to a foreign pointer: *"assigning a segment incorrect
spatial and/or temporal bounds could result in a VM crash when attempting to
access the memory segment."* Sources:
<https://docs.oracle.com/en/java/javase/22/docs/api/java.base/java/lang/foreign/MemorySegment.html>
· <https://docs.oracle.com/en/java/javase/22/docs/api/java.base/java/lang/foreign/Linker.html>

**Answer to Q4, direct, and it is the finding this seat most wants on the
record.** **Java's newest FFI draws the line in exactly the place panel 168's
completeness critic drew it.** It enforces the **caller-side** half — Java
touches a segment after the scope ends, exception — and it refuses the
**foreign** half outright, in its own javadoc, twice. **Defect 068 is FFM's
`IllegalStateException` case. Defect 066 is FFM's "no insight into the lifetime
intended" case. They are not one class, and the best-resourced FFI ever built
treats them as two.** Filing 068 with 066 as unsolvable is filing an enforced
class with an unenforceable one.

One difference Heroes should notice and not lose: FFM enforces the caller-side
half **at run time, on every access**, because Java's arena can be closed from
anywhere. Heroes' 068 reproducer is a **single function**, so Heroes can afford
the static rule Java could not.

## THE NEGATIVE THE BRIEF ASKED FOR

**Has any language shipped the caller-side rule for this exact shape and
WITHDRAWN it? None was found.**

Searched for, in these words and in variants: *removed / reverted / rolled back /
withdrawn* combined with *borrow check, escape analysis, exclusivity, lifetime
profile, scoped pointers, view types, dangling*; plus per-language: Rust E0506 and
AST-borrowck removal, Swift exclusivity rollback and `-enforce-exclusivity`, D
DIP 1000, Nim views, Ada limited types, GCC `-Wdangling-pointer` /
`-Wdangling-reference`, clang `-Wlifetime` / P1179, C++ Core Guidelines lifetime
profile.

**What the record shows instead — four distinct outcomes, none of them
withdrawal:**

| outcome | case | source |
|---|---|---|
| **relaxed in duration, kept in substance** | Rust: lexical borrowck to NLL, RFC 2094, default 1.31 (2018-12-06), all editions by 1.36 | RFC 2094; the 1.31 announcement |
| **strengthened** | Swift: overlapping-access warning to **error** in 4.2, 2018-05-15; run-time checks on in Release in Swift 5 | Swift Forums; swift.org |
| **kept, given an escape hatch** | GCC 14: `-Wdangling-reference` false positives reduced, `std::span`-like classes exempted, **new `gnu::no_dangling` attribute** | GCC 14 changes |
| **kept, never promoted** | Nim views: flow-insensitive, experimental since 1.4 (2020-10-16), still experimental. D DIP 1000: `-preview=dip1000`, DIP marked **Superseded** 2019-03-07 because *"the implementation supersedes the DIP"* | Nim manual; the DIP 1000 announcement |

**The nearest thing to a withdrawal is a consumer's, not a language's**: the Linux
kernel disabling `-Wdangling-pointer` (Torvalds, 49beadbd47c2). And one abandoned
*implementation*: the clang `-Wlifetime` fork for P1179 was never upstreamed.

**So the negative that would change this sitting does not exist in the record
this seat can reach.** State it that way in the synthesis, and note that a
negative rests on the searcher's vocabulary: the list above is what was searched
for.

## Which route has the best precedent, which has none

**R2 — the caller-side rule. BEST PRECEDENT IN THE SITTING, and it is not
close.** Five shipping languages state the rule statically and caller-side: Rust
(E0506), Swift (SE-0176, Swift 4.0), Ada (limited types, Ada 83), Austral (borrow
block), Nim (views). Zero withdrawals. **Two deliberate departures Heroes must
own:**

1. Every one of them **exempts raw pointers by name** — SE-0176 verbatim. Heroes'
   `.ptr()` is the only road to C and therefore cannot be an escape hatch the way
   `*mut T` is. The departure is justified; it must be *recorded* as one.
2. R2 as worded is whole-function and flow-insensitive. RFC 2094's Problem case
   #1 is a correct program of that exact shape, rejected. **Price the
   over-refusal on the corpus before landing, not after.** Historically this is
   repaired with flow analysis (three years) or an escape attribute (one
   release).

**R1 — the handle route. MOST REPLICATED SHAPE, but its new half is the
unprecedented one.** `SafeHandle` (2005), `PyCapsule` (2009/2010), `jobject`
(1997), FFM `Arena` (2024) all move pointer to handle, and two of them say in
their own docs that they did it *because the pointer could not carry the rule*.
That is strong. **But all four enforce at run time.** Only Ada's limited types
enforce at compile time, and Ada does it by removing assignment from the type,
not by checking calls. So R1's run-time half is the best-attested thing in two
sittings, and R1's **static half** is still the thing nobody verifies. 167's
headline stands for R1 and does **not** stand for R2.

**R3 — the group-level mark. Precedented as a MARKER, never as a proof, and the
ancestry is Wirth's.** Oberon Report: low-level facilities *"can break the data
type compatibility rules otherwise imposed by the language definition"*, and such
modules are *"easily recognized due to the identifier SYSTEM appearing in their
import lists."* Fortran 90's `TARGET` is the same device for aliasing: *"Modern
Fortran's pointers can't associate with arbitrary data. They can be pointed only
at objects that have the explicit `TARGET` attribute"*, and flang's own doc says
a non-`TARGET` variable *"is generally safe from aliasing with pointers."* Both
shipped, both survive, **and neither verifies anything** — Oberon's is visibility,
Fortran's is an optimiser fact. Same verdict as MPI's `ASYNCHRONOUS` at 167: the
mark buys the compiler a fact to act on; it does not buy verification. R3 is
legitimate and cheap **if sold as a marker.** Sources:
<https://people.inf.ethz.ch/wirth/Oberon/Oberon.Report.pdf> ·
<https://flang.llvm.org/docs/Aliasing.html>

**R4 — withdraw the field lend. STILL NO PRECEDENT.** Unchanged from 167: not one
surveyed language removed the short-lived borrow after adding the long-lived one.
All nine lease pairs still ship. Oberon is the only near-ancestor and its move was
not withdrawal — it was *relocation behind a named import*, which is R3.

## argument, 104 words

068 is exclusivity, not escape analysis, and exclusivity ships statically and
caller-side in Rust (E0506), Swift (SE-0176, warning to error in 4.2), Ada
(limited types, since 83), Austral and Nim. None was ever withdrawn; Rust relaxed
its duration, Swift strengthened it, GCC added an escape hatch. So filing 068
unsolvable on the 167 finding misreads it: that finding was about the callee. One
correction: every surveyed language exempts raw pointers by name — SE-0176 says
so verbatim. Heroes has no escape hatch, so the exemption costs it everything. R2
is a deliberate departure, which is Wirth's move: keep the road, mark it. Price
the over-refusal.

## condition

1. **A shipping language that applied an exclusivity rule to a pointer handed to
   foreign code, and then turned it off.** The vocabulary searched is listed
   under THE NEGATIVE. Nothing found. If a seat finds one, R2's departure stops
   being justified by absence of precedent and becomes a repeat of a known
   failure.
2. **A shipping language whose static caller-side rule reaches raw pointers at
   all** rather than only language-level references. Rust and Swift exclude them
   explicitly; Ada's `Unchecked_Access` is the documented hole. Find one and R2
   becomes a port rather than a departure, which strengthens it.
3. **A compile-time handle rule in an FFI other than Ada's limited types.** If one
   exists, R1's static half acquires the precedent it currently lacks and the
   routes stop competing.
4. **A measurement from this repository** — not a precedent, but it outranks this
   seat under CLAUDE.md §12: if R2 as worded refuses zero programs in `examples/`
   and `tests/golden/`, the over-refusal cost warned about here is hypothetical
   and the historical caution should be discounted accordingly.

## predictions

**1.** If R2 lands whole-function and flow-insensitive, **at least one program
currently green in `examples/` or `tests/golden/` will be refused and will need a
source edit.** The shape is RFC 2094's Problem case #1 and it is two lines from
068's own reproducer: lend, call the consumer, then reassign the root **after**
the consumer has finished — safe, and refused. The instrument is `./heroes check`
over the corpus plus the `corpus`, `emission` and `run` suites, run after the
rule lands and before it is called done. **Falsified** if the rule lands and the
whole corpus stays green with zero source edits, in which case the historical
caution in this report does not apply to Heroes' corpus and should be dropped
from the record rather than carried forward.

**2.** If R2 lands without a named escape, **one will be requested within four
milestones — by a program, not by a panel.** The mechanism is GCC 12 to GCC 14
(`gnu::no_dangling`), Rust 1.0 to 1.31 (NLL), and the Linux kernel's blanket
disable. Three independent ecosystems, each of which shipped an over-approximating
lifetime rule and within three years had either flow analysis or an opt-out.
Falsified if four milestones pass with R2 in and no escape requested.

## files

Briefs read: `docs/panel/169-briefs/00-shared.md`,
`docs/panel/169-briefs/historian.md`. Repository context read and **not measured
by this seat**: `docs/panel/167-reports/historian.md`, `docs/work/DEFECTS.md`.
**Every repository number in this report is the coordinator's, not this seat's.**
