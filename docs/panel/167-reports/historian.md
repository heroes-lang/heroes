# Panel 167 — historian's report

**Written to disk by the coordinator, verbatim from the seat's own final
message**, because this seat has no write tool — the procedure's rule since
panel 145, after panels 143 and 144 both recorded that the historian could not
be audited.

**Seat:** historian (advisory, no veto). **Question:** how have other languages
answered *how long may a foreign function keep a pointer we gave it?*

Every claim below was checked by web search on 2026-09-20. Claims I could not
source are marked **unverified** in those words.

---

## verdict

**approve, with a redirection.** Approve landing a rule; object to landing
**only one** of A, B, C or D. The survey says the shipped answer is never one of
them — it is A **and** B **and** a run-time instrument, three parts, in every
survivor.

---

## precedents

### The headline finding, stated first because it decides the sitting

**No language in this survey enforces foreign retention statically. Not one.
The two that came closest either died or are still behind a preview flag.**
Every survivor states the rule in prose and checks it at run time, or not at
all.

### 1. Go — cgo. The closest match, and it is prose plus a partial dynamic check

The rule, verbatim from the `cmd/cgo` documentation today:

> "C code may not keep a copy of a Go pointer after the call returns, unless the
> memory it points to is pinned with runtime.Pinner and the Pinner is not
> unpinned while the Go pointer is stored in C memory."

Enforcement, verbatim:

> "These rules are checked dynamically at runtime. The checking is controlled by
> the cgocheck setting of the GODEBUG environment variable. The default setting
> is GODEBUG=cgocheck=1, which implements reasonably cheap dynamic checks."

**What happened to the strict mode.** Go 1.21 (2023) *removed* `cgocheck=2` as a
runtime option; it is now a build-time experiment. The thorough check got
*harder* to enable, not easier. `cgocheck=1` has been the default since Go 1.6
(2016-02-17) and still is — the brief's question *was it ever turned off by
default* answers **no**.

**The admission that matters.** Ian Lance Taylor's own design proposal, October
2015: *"It is still possible to violate the invariant on the C side. There is
little we can do about this."* And on the exact shape of defect 066: *"A
particular unsafe area is C code that wants to hold on to Go func and pointer
values for future callbacks from C to Go. This works today but is not permitted
by the invariant. **It is hard to detect.**"*

Go, with a garbage collector, a runtime and write barriers available to it,
wrote the rule down and declared the C side undetectable. Heroes has strictly
less machinery.

Sources: [cmd/cgo](https://pkg.go.dev/cmd/cgo) ·
[proposal 12416](https://go.googlesource.com/proposal/+/master/design/12416-cgo-pointers.md) ·
[Go 1.6](https://go.dev/doc/go1.6) · [Go 1.21](https://tip.golang.org/doc/go1.21)

### 2. Java / JNI — a lend, a lease, and a run-time instrument, shipped 1997

- **The lend.** Local references *"are valid only while the method that you
  invoke runs"*; storing one in a static *"is illegal and will lead to crashes."*
- **The lease.** `NewGlobalRef` / `DeleteGlobalRef`: *"guaranteed to be valid
  until you call DeleteGlobalRef."*
- **The copy-or-pin lease, verbatim** — exactly route A's shape:
  `GetStringUTFChars(JNIEnv *env, jstring string, jboolean *isCopy)` — *"This
  array is valid until it is released by `ReleaseStringUTFChars()`. If `isCopy`
  is not `NULL`, then `*isCopy` is set to `JNI_TRUE` if a copy is made."*
- **The instrument.** `-Xcheck:jni`, a **run-time** flag: *"Expect a performance
  degradation"* and *"not guaranteed to find all invalid arguments."*
- **An abort that counts** — the only one found. Android ART aborts with
  `JNI ERROR (app bug): local reference table overflow (max=512)`. That is an
  overflow guard, not an end-of-program audit.

**What happened to it.** JEP 472, delivered in JDK 24, *Prepare to Restrict the
Use of JNI*: *"Such problems cannot be prevented by the Java runtime, nor do
they provoke exceptions for Java code to catch."* Twenty-seven years of
documented-rule FFI, and the conclusion is to restrict it.

Sources: [JNI functions](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html) ·
[Android JNI tips](https://developer.android.com/ndk/guides/jni-tips) ·
[-Xcheck:jni](https://docs.oracle.com/javase/8/docs/technotes/guides/troubleshoot/clopts002.html) ·
[JEP 472](https://openjdk.org/jeps/472)

### 3. Java FFM (JEP 454) — the newest, best-resourced attempt, and it chose dynamic

Delivered in Java 22 (2024). Lifetime is an `Arena`: *"Attempts to access a
memory segment after its arena is closed will fail with an exception."* And the
boundary is still a boundary: *"Such failures cannot be prevented by the Java
runtime, nor be caught by Java code."* The state of the art in 2024, with an
entire JVM to spend: temporal safety on the managed side, enforced at run time;
nothing on the native side.

Source: [JEP 454](https://openjdk.org/jeps/454)

### 4. C# / .NET — two forms, one for the call and one for a lifetime, since 2002

- **`fixed`** pins for the statement's execution. Static restriction, but it is
  scoping rather than escape analysis: the docs themselves tell you to declare a
  second pointer variable, which can be stored anywhere.
- **`GCHandle.Alloc(obj, GCHandleType.Pinned)`** — the lifetime form, released
  with `Free()`. No abort on an unfreed handle; it leaks.
- **`Marshal.StringToHGlobalAnsi` / `FreeHGlobal`** — a literal
  copy-and-release pair, .NET Framework 1.0, still documented for .NET 10.

Sources: [fixed](https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/fixed) ·
[GCHandle.Alloc](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.gchandle.alloc) ·
[Marshal.StringToHGlobalAnsi](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.marshal.stringtohglobalansi)

### 5. Swift — documentation for nine years, then a type-system feature

`withUnsafePointer` ships a **prose** rule: *"do not store or return the pointer
for later use"*, *"misusing a temporary pointer by allowing it to outlive the
enclosing function call results in undefined behavior."* Not compiler-enforced.
Swift then spent a decade fixing it: SE-0103, *Make non-escaping closures the
default*, Swift 3 (2016) — route B with the polarity inverted, and it survived
ten years; then SE-0446 *Nonescapable Types*, accepted 2024, implemented in
Swift 6.2, with SE-0447 `Span` on top. The Language Steering Group noted SE-0446
*"in isolation is not sufficient"*. A multi-year type-system project, and even
now it governs Swift values rather than what a C function does with an address.

Sources: [withUnsafeMutablePointer](https://developer.apple.com/documentation/swift/withunsafemutablepointer(to:_:)) ·
[SE-0103](https://github.com/apple/swift-evolution/blob/master/proposals/0103-make-noescape-default.md) ·
[SE-0446](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0446-non-escapable.md) ·
[SE-0447](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0447-span-access-shared-contiguous-storage.md)

### 6. Ada — the one production language with a static escape refusal, and it does not reach C

Ada RM 3.10.2: *"In most cases, accessibility is enforced at compile time by
Legality Rules. However, run-time accessibility checks are also used, since the
Legality Rules do not cover certain cases."* Two caveats: even Ada could not make
it fully static, and *"the `Unchecked_Access` attribute may be used to circumvent
the accessibility rules"* — which is precisely where you reach when interfacing
with C. Ada's rule is **the caller-side check Heroes already has**:
`field_lend_escapes` and `field_lend_needs_a_place` are accessibility levels by
another name. Ada is not precedent for a rule about the callee.

Source: [Ada 2022 RM 3.10.2](http://www.ada-auth.org/standards/22rm/html/rm-3-10-2.html)

### 7. Fortran + MPI — the one shipped route-B mark for exactly this problem

MPI's non-blocking sends keep the caller's buffer past the call. The fix was a
declaration-site attribute: `ASYNCHRONOUS`, extended by Fortran TS 29113 so that
it *"also covers asynchronous communication occurring within library routines
written in C."* Standardised into MPI-3.0, still in MPI 5.0.

**What the mark does and does not do**: it *"informs the compiler that any
statement in the scoping unit may be executed while the buffer is affected by a
pending"* operation — it constrains *the caller's optimiser*. The other half
stays prose: *"a pending communication affector shall not be referenced."* The
MPI report's own worked example breaks the rule and nothing catches it. **This is
the honest price of route B: the mark buys the compiler a fact it can act on; it
does not buy verification.** It survived fourteen years in the most
performance-critical corner of computing.

Sources: [MPI-3.1 §19.1.17](https://www.mpi-forum.org/docs/mpi-3.1/mpi31-report/node427.htm) ·
[MPI-4.1 Fortran support](https://www.mpi-forum.org/docs/mpi-4.1/mpi41-report/node467.htm)

### 8. Clang — the binding-author's mark already exists in C, and almost nothing checks it

`[[clang::noescape]]` *"indicates that the function will not allow the parameter
to escape its scope."* Swift's ClangImporter reads it. **Its verification is one
clang-tidy check with a two-function scope**: `bugprone-no-escape` flags only
pointers captured by `dispatch_async()` and `dispatch_after()`. And clang's new
`-Wlifetime-safety-*` says of itself: *"designed for bug finding, not
verification… It does not guarantee the absence of all lifetime bugs"*, off by
default and intra-procedural.

**Bearing on route H's precedent**: route H worked because the header carried
`const`. `noescape` is the header's word for retention — and the panel's own
measurement says real headers do not carry it. `sqlite3_bind_text` does not.

Sources: [Clang AttributeReference](https://clang.llvm.org/docs/AttributeReference.html) ·
[bugprone-no-escape](https://clang.llvm.org/extra/clang-tidy/checks/bugprone/no-escape.html) ·
[Clang Lifetime Safety](https://clang.llvm.org/docs/LifetimeSafety.html)

### 9. D — the cautionary tale for route B as a checked mark

DIP 1000, *Scoped Pointers*: static escape analysis in a production systems
language, enabled by `-preview=dip1000`, **not enabled by default**. On
2019-03-07 it was marked **Superseded**, because *"the implementation supersedes
the DIP"* — specification and compiler had drifted apart. Forum discussion in
August 2024 still describes it as behind the preview switch. Seven years in
preview and counting, its own design document retired for drift. That is the cost
figure to hold against any route that tries to prove, rather than declare,
escape.

Sources: [D blog, DIP1000](https://dlang.org/blog/2022/06/21/dip1000-memory-safety-in-a-modern-system-programming-language-pt-1/) ·
[Superseded announcement](https://www.digitalmars.com/d/archives/digitalmars/D/announce/DIP_1000--Scoped_Pointers--Superseded_54701.html)

### 10. Cyclone — the ancestor that solved it statically, and stopped

Cyclone's region system statically prevents a pointer escaping its region (Jim,
Morrisett, Grossman, Hicks, Cheney, Wang, USENIX ATC 2002; PLDI 2002). The
project's own site: *"Cyclone is no longer supported; the core research project
has finished."* CLAUDE.md §5 already names the Cyclone rule as spent. The one
dialect that put full static region-escape checking into C-family syntax is dead,
and the descendants that lived — Rust, Swift, Go — all kept the static rule
**inside** the language and gave up at the foreign boundary.

Sources: [cyclone.thelanguage.org](http://cyclone.thelanguage.org/) ·
[Cyclone: A safe dialect of C](http://www.cs.umd.edu/projects/cyclone/papers/cyclone-safety.pdf) ·
[Region-based memory management in Cyclone](https://dl.acm.org/doi/10.1145/512529.512563)

### 11. Rust, Haskell, OCaml, Erlang, Nim — five more lend/lease pairs, all documented, none checked

| language | the call-length form | the lease, with its release | checked? |
|---|---|---|---|
| Rust | `&T` — a raw pointer at `extern "C"` | `Box::into_raw` / `Box::from_raw` | no; `unsafe` |
| Haskell | `withForeignPtr` | `newStablePtr` / `freeStablePtr` | no |
| OCaml | `CAMLparam`/`CAMLlocal`/`CAMLreturn` | `caml_register_global_root` | no |
| Erlang | `enif_inspect_binary`, transient | `enif_keep_resource` / `enif_release_resource` | no |
| Nim | — | `GC_ref` / `GC_unref` | no |

Haskell's is the sharpest-worded, with the reason given plainly: *"the compiler
can only track usage of the `ForeignPtr` object, not a `Ptr` object made from
it."* **That is defect 066 in one sentence, written by GHC's library authors.**

Sources: [Rustonomicon FFI](https://doc.rust-lang.org/nomicon/ffi.html) ·
[Foreign.ForeignPtr](https://hackage-content.haskell.org/package/base-4.22.0.0/docs/Foreign-ForeignPtr.html) ·
[OCaml manual ch. 23](https://ocaml.org/manual/5.5/intfc.html) ·
[erl_nif](https://www.erlang.org/doc/apps/erts/erl_nif.html) ·
[Nim memory management](https://nim-lang.org/1.4.8/gc.html)

### 12. OCaml, 2023 — what it costs when you do try to check the C side

Edwin Török, *Targeted Static Analysis for OCaml C Stubs*, OCaml Workshop 2023 @
ICFP, [arXiv:2307.14909](https://arxiv.org/pdf/2307.14909). OCaml C stubs benefit
*"from neither OCaml's nor C's type checking, and existing C static analysers are
not aware of the OCaml GC safety rules."* The tool is built on Goblint and checks
for retention without GC-root registration. **Twenty-seven years after OCaml's C
interface, the check exists — as a separate whole-program C analyser, outside the
compiler, targeted at one ecosystem's conventions.** Its precise bug counts:
**unverified**.

### 13. Hylo / mutable value semantics — the nearest ancestor to Heroes' own model

Racordon, Shabalin, Zheng, Abrahams, Saeta, *Native Implementation of Mutable
Value Semantics*, [arXiv:2106.12678](https://arxiv.org/abs/2106.12678), from the
abstract: *"references are second-class: they are only created implicitly, at
function boundaries, and cannot be stored in variables or object fields."* That
sentence **is** Heroes' `field_lend_escapes` clause. Hylo achieves lifetime
safety by never letting a reference become a value — which works exactly as far
as the language's own edge. Hylo's C interoperability is listed as ongoing
research, not a shipped rule; whether it addresses foreign retention:
**unverified**.

### 14. Zig — no lifetime tracking, run-time allocators instead

Zig has no borrow checker; Debug and ReleaseSafe insert run-time checks, and the
debug allocator catches use-after-free at run time. **Sources secondary**
(zig.guide, community blogs, ziglang/zig#2301) rather than the language
reference; treat the specifics as **unverified** and the direction — run-time,
not static — as consistent with everything above.

---

## The two questions the brief asked, answered directly

**Q. Is there a language that enforces this statically, and at what cost?**
**No.** Ten ecosystems surveyed. The static enforcement that exists is always on
the *caller's* side — Ada's accessibility levels, Rust's lifetimes, C#'s
`scoped`, Swift's `@escaping` and `~Escapable`, Hylo's second-class references,
Cyclone's regions — and **every one stops at the foreign boundary by
construction**, because the callee is not in the type system. Go said so in its
design document; JEP 454 and JEP 472 say so for the JVM; Haskell's own docs say
the compiler can only track the handle. The two attempts to push further cost:
Cyclone (dead) and D's DIP 1000 (seven years in preview, DIP superseded).

**Q. Has any language shipped a pin-and-release pair for exactly this?**
**Yes — nine of them, across eight ecosystems, and every single one still
ships.**

| pair | first shipped | copy or pin | survived? |
|---|---|---|---|
| JNI `GetStringUTFChars`/`ReleaseStringUTFChars` | JDK 1.1, Feb 1997 | **either, explicitly** | yes |
| JNI `NewGlobalRef`/`DeleteGlobalRef` | JDK 1.1, 1997 | handle | yes |
| .NET `Marshal.StringToHGlobalAnsi`/`FreeHGlobal` | .NET Fx 1.0, 2002 | **copy** | yes |
| .NET `GCHandle.Alloc(Pinned)`/`Free` | .NET Fx 1.0, 2002 | pin | yes |
| Haskell `newStablePtr`/`freeStablePtr` | Haskell FFI | handle | yes |
| OCaml `caml_register_global_root`/`caml_remove_global_root` | long-standing | root | yes |
| Erlang `enif_keep_resource`/`enif_release_resource` | ERTS NIF API | refcount | yes |
| Nim `GC_ref`/`GC_unref` | — | pin | yes |
| Go `runtime.Pinner.Pin`/`Unpin` | Go 1.21, 2023 | pin | yes, newest |

**Route A is the single most replicated construct in this survey.**

**One caveat on the abort.** Go's `Pinner` deliberately does not abort; .NET
leaks silently. The only abort-with-a-count found is Android ART's
local-reference-table overflow, an overflow guard rather than an exit audit. **A
precedent for aborting at exit with a count, in a shipping language's FFI, I did
not find** — I searched JNI, .NET, Go, Haskell, OCaml, Erlang and Nim. Heroes'
existing lease already does this, so the sitting is extending a local precedent,
not importing one.

---

## verdict per route

**Route A — the field lease. APPROVE, the best-attested route in the survey.**
Nine shipped pairs, oldest 1997, newest 2023, zero withdrawals. JNI's `isCopy` is
the precise ancestor. Heroes already has the `cstr` lease; closing the asymmetry
by symmetry is the move with the most precedent behind it.

**Route B — the binding author's mark. APPROVE as a companion, not a
substitute.** Precedent is good and not what people assume: MPI's `ASYNCHRONOUS`
is a declaration-site mark for exactly *the C library keeps this buffer*, and it
survived; Swift's `@escaping` is the same shape inverted. **But every one is
checked on the caller's side only.** That is enough for Heroes — *a frame-rooted
`.ptr()` may not reach a `keeps` parameter* is a caller-side refusal the checker
can make today. Do not let the mark be sold as a proof.

**Route C — write the hole into the specification. OBJECT, and the survey is
against it.** The only route with a clean negative outcome in the record. Java
documented the rule in 1997 and concluded in 2024 that the interface must be
restricted. Swift documented it in 2014 and spent 2024–25 making it a type fact.
Go documented it in 2015 and shipped a dynamic check the same release. **Every
ecosystem that wrote it down as prose alone later came back to add a mechanism.**

**Route D — withdraw the field lend. OBJECT.** No precedent. Not one surveyed
language removed the short-lived borrow after adding the long-lived one; all nine
kept both.

---

## the route nobody listed

**Route E — all three parts, because that is what every survivor ships.** The
four routes are posed as alternatives and the record has no single-mechanism
survivor in it. The recurring triple: a call-length form (Heroes has it), a
lifetime form with an owed release (route A), and a run-time instrument that
catches the violation (`-Xcheck:jni`, `cgocheck=1`, `IllegalStateException`).
Route B's mark is the **router** that decides which of the two a call site gets.
Route B alone leaves the program with nowhere legal to go; route A alone leaves
the binding author unable to say which parameters need it.

**Route F — put the mark where Fortran puts it: on the argument, not only on the
parameter.** MPI requires `ASYNCHRONOUS` on the actual buffer's declaration in
the caller's scoping unit, precisely so the caller's compiler and the caller's
reader both see it. The SQLite example is the argument for this: retention there
is decided by the **fifth argument's value**, so a declaration-only mark cannot
express it and a call-site form can. The precedent that this is the workable
placement is MPI's, and it is fourteen years old.

---

## argument

Ten ecosystems, one answer: **nobody checks the callee statically, because the
callee is not in the type system.** Go's own designer wrote *"there is little we
can do about this"* in 2015; Java concluded the same in 2024. The static
enforcement that exists is caller-side and stops at the boundary. Cyclone died;
D's DIP 1000 has been seven years in preview. What survived instead is a triple:
a call-length lend, a lease with an owed release, a run-time instrument. Nine
such lease pairs, 1997 to 2023, zero withdrawn. Heroes has two of three. Land A,
add B as the router, keep C out.

---

## condition

1. **A shipped language where the compiler refuses a call because the callee's
   body, in C, retains the pointer** — verification rather than declaration, in a
   production toolchain, on by default. I searched Go, Rust, Swift, C#, Java,
   Ada, Haskell, OCaml, Erlang, Nim, D, Zig, Cyclone, Hylo, and the
   FFI-verification literature. What I found were research artifacts and
   off-by-default bug-finders, never a default-on compiler refusal.
2. **Any ecosystem that shipped a lease-style pair and then removed it.** I found
   none; all nine still ship.
3. **A header convention that carries retention and is present in real C
   headers.** `[[clang::noescape]]` exists and Swift's importer reads it, but the
   headers Heroes binds do not carry it.

---

## a falsifiable prediction

**If Heroes ships route C alone — the hole written into the specification, no
mark and no lease — then within four milestones a diagnostic or a mechanism for
foreign retention will be proposed again by a seat other than this one, and it
will be proposed because a program was written that needed it, not because a
panel was reviewing the spec.**

The mechanism is the pattern in §§2, 3, 5 and 12: Java 1997 → `-Xcheck:jni` →
JEP 454 → JEP 472; Swift 2014 prose → SE-0103 → SE-0446/0447; Go 2015 prose →
cgocheck → `runtime.Pinner`; OCaml's C interface → a Goblint-based analyser.
Four independent ecosystems, each of which wrote the rule as prose first and came
back for a mechanism later, none of them faster than the problem arrived.

Falsified if four milestones pass with route C in the spec and nobody re-opens
it. The instrument is this repository's own record.

---

## files

Briefs read: `docs/panel/167-briefs/00-shared.md`,
`docs/panel/167-briefs/historian.md`. Repository facts relied on come from the
shared brief and are the coordinator's measurements, not this seat's: the
0-of-71 figure, the `--sanitize` reproduction, the 33 `.ptr()` occurrences, and
the −44 real price of route F at panel 166.
