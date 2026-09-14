# Panel 147 — historian

**Written to disk by the coordinator**, verbatim from the seat's returned report:
the historian seat has Read, WebSearch and WebFetch and no write tool, and
panels 143 and 144 both recorded that its report could not be audited for
exactly this reason. The text below is the seat's own.

---

**Seat**: historian, advisory, no veto.
**Tree**: not touched, not built. Nothing in this report was run against the repository; every number below is either from my own enumeration of sources or quoted from the shared brief and attributed as such.
**design.md sections I rest on**: Part 6 (the refusal row and its falsifier discipline) and Part 8 wart 20, **as quoted in `00-shared.md`**. I did not grep design.md myself — my mandate is precedent — so treat those section numbers as the brief's citation, not mine.

## verdict

**approve** — advisory. A form should enter. Precedent favours **Route A** over Route B *specifically for a language that emits C*, and Route C is the one route whose precedent is actively expiring as of 2026.

I also record a **fourth route nobody listed** (§ Route D below) and the reason it failed the last time somebody tried it in a language without a GC.

## The measurement I can offer: an enumeration, and what it counts

Per CL-057 the list is itself a measurement, so here is mine, with its vocabulary named.

**Searched terms** (WebSearch, 2026-09-14): `annotate foreign function ownership release`; `free_function attribute binding C type`; `type names its releaser no destructors`; `value semantics no destructors C FFI handle release scope exit`; `language without destructors without defer C FFI handle leak compile error linear type`; `FinalizerPtr`; `custom_operations finalize`; `enif_open_resource_type destructor`; `%newobject %delobject`; `transfer full transfer none`; `cleanup variable attribute`; plus per-language searches for Go, Swift, Zig, D, Odin, Hare, Nim, Vala, Austral, Vale, Cyclone, Vault, Hylo, Splint.

**Found: 12 systems in which a FOREIGN function is named as the releaser of a foreign resource, rather than a native destructor being written.** Of those 12:

- **2 fire the releaser at lexical scope exit**: Vala's `free_function`, GCC's `__attribute__((cleanup))`.
- **1 of those 2 attaches the releaser to the TYPE** rather than to the variable: **Vala**.
- **9 fire at reclamation** (GC, refcount, or a binding generator's own bookkeeping), not at scope exit.

So the honest answer to your brief's item 2 — *which of these annotate a foreign function rather than defining a native destructor* — is **most of them do**; the annotate-a-foreign-function idiom is the dominant idiom for foreign resources and has been for two decades. The rare part is not the annotation. **The rare part is firing it at scope exit.** Exactly one shipped language does the combination Route A proposes, and it is Vala.

## precedents

### Route A — the type names a foreign function as its releaser

**A1. Vala — VERIFIED, and it is the bullseye.** Vala compiles to C, uses the GObject type system, binds C libraries through `.vapi` files, and its `[Compact]` classes carry a `free_function` naming a C function. The shipped GNOME binding for the exact library in your census reads:

```
[Compact]
[CCode (free_function = "sqlite3_finalize", cname = "sqlite3_stmt", cprefix = "sqlite3_")]
```

and, in the same file, `free_function = "sqlite3_close"` on `Database`, `sqlite3_mutex_free` on `Mutex`, `sqlite3_backup_finish` on `Backup`. Fetched 2026-09-14 from `https://raw.githubusercontent.com/GNOME/vala/master/vapi/sqlite3.vapi`.

Semantics, fetched 2026-09-14 from the GNOME wiki: Vala uses "automatic reference counting instead of tracing garbage collection"; a reference is unreferenced "when a reference variable leaves its scope"; compact classes are "classes that are not registered with Vala's type system", typically from non-GObject C libraries, and they "do not support reference counting by default", so **only one strong reference can exist** and additional references must be `unowned`.

**What happened to it**: Vala is alive. Jürg Billeter and Raffaele Sandrini finished it in May 2006; self-hosting at 0.1.0 in July 2007 (Wikipedia, secondary). GNOME Discourse carries "Vala Documentation Updates — September 2026", so it is maintained twenty years on. The *known cost*: Vala's own documentation says you can form reference cycles and must break them with weak references, and that "many types in the Vala binding APIs are falsely marked weak even though they should actually be unowned" — the `owned`/`unowned` distinction is the tax that `free_function` levies, and Vala pays it in binding-file churn, not in compiler soundness.

**Read this as**: the single-strong-reference rule that Vala imposes on compact classes is *the same rule Heroes already has* — value semantics, no aliasing. Vala needed it precisely because it names a foreign releaser. That is a precedent landing on Heroes' existing premise rather than against it.

**A2. Swift C++ interop, `SWIFT_SHARED_REFERENCE(retain, release)` — VERIFIED.** From `https://www.swift.org/documentation/cxx-interop/`, fetched 2026-09-14: "This macro expects two arguments: a retain and release function. These functions must be global functions that take exactly one argument and return void. The argument must be a pointer to the C++ type (not a base type)." Swift then "call[s] these custom retain and release functions where it would otherwise retain and release Swift classes." Fires at **refcount zero**, not scope exit. Producing functions must additionally be annotated `SWIFT_RETURNS_RETAINED` or `SWIFT_RETURNS_UNRETAINED` — i.e. the type-level annotation was **not sufficient on its own**; a per-function ownership annotation was also required. That is the most directly transferable warning in this report (see my prediction).

**A3. GLib boxed types — VERIFIED.** `GType g_boxed_type_register_static (const gchar *name, GBoxedCopyFunc boxed_copy, GBoxedFreeFunc boxed_free);` — "Boxed types can be used for wrapping structures defined in non-GObject based libraries." Shipped in GLib 2.x since 2002 (secondary for the date), still current at `https://docs.gtk.org/gobject/func.boxed_type_register_static.html`.

**A4. Haskell `ForeignPtr` — VERIFIED.** `newForeignPtr :: FinalizerPtr a -> Ptr a -> IO (ForeignPtr a)`; the finalizer is a *foreign function pointer*, imported as e.g. `foreign import ccall "notmuch.h &notmuch_database_destroy"`. From `hackage-content.haskell.org/package/base-4.22.0.0/docs/Foreign-ForeignPtr.html`, fetched 2026-09-14: "There is no guarantee of promptness, however the finalizer will be executed before the program exits." **Reclamation-timed, not scope-timed** — and Heroes' census problem (SQLite says BUSY *at close time*) is exactly the failure a non-prompt finalizer does not fix.

**A5. OCaml custom blocks — VERIFIED.** `void (*finalize)(value v)` — "called when the block becomes unreachable and is about to be reclaimed" (`ocaml.org/manual/5.2/intfc.html`, fetched 2026-09-14). Reclamation-timed.

**A6. Erlang NIF resources — VERIFIED.** `typedef void ErlNifResourceDtor(ErlNifEnv* caller_env, void* obj);` registered through `enif_open_resource_type`; "A resource object is not deallocated until the last handle term is garbage collected by the VM and the resource is released with `enif_release_resource`" (`erlang.org/doc/apps/erts/erl_nif.html`, fetched 2026-09-14). Reclamation-timed.

**A7. GCC `__attribute__((cleanup(f)))` — VERIFIED, and it is the near-miss.** From `gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/Common-Variable-Attributes.html`, fetched 2026-09-14: "The `cleanup` attribute runs a function when the variable goes out of scope. … The function must take one parameter, a pointer to a type compatible with the variable. … When multiple variables in the same scope have `cleanup` attributes, at exit from the scope their associated cleanup functions are run in reverse order of definition." **Scope-timed — but attached to the VARIABLE, not the type.** WG14 N2895 (Gustedt & Seacord, 2021-12-31) lists its shortcomings verbatim: callbacks are "necessarily associated to exactly one resource", the cleanup function must be "defined as a separate function, usually far from its use", and the feature "cannot be easily lifted into C23 as a standard attribute, because the `cleanup` feature clearly changes the semantics of a program".

**A8. SWIG `%newobject` / `%delobject` — VERIFIED** (`swig.org/Doc4.0/Customization.html`). Ownership annotations on foreign functions; without `%newobject` SWIG "is conservative and will never delete objects unless it knows for certain that the returned object was newly created" — i.e. the default is the silent leak your census measured.

**A9. Splint `/*@only@*/`, `/*@owned@*/`, `/*@dependent@*/`, `/*@keep@*/`, `/*@temp@*/` — VERIFIED** (`splint.org/manual/html/sec5.html`). Declares `/*@only@*/ /*@null@*/ void *malloc (size_t size);` and `void free (/*@only@*/ /*@out@*/ /*@null@*/ void *ptr);` — annotating the *foreign* allocator and releaser. **What happened**: stable release 3.1.2, 12 July 2007; main development stopped in 2010 (Wikipedia, secondary). The idiom survived; the tool did not. Static checking with no inserted call did not carry a language.

**A10. Clang ARC `cf_returns_retained` / `cf_consumed` / `ns_consumed` — VERIFIED** (`clang.llvm.org/docs/AutomaticReferenceCounting.html`). Annotates plain C functions with ownership transfer. Note the standing carve-out: "ARC does not manage Core Foundation objects automatically." The releaser is *fixed* (`CFRelease`), not named per type — so this is ownership-annotation without releaser-naming.

**A11. gobject-introspection `(transfer full)` / `(transfer none)` — VERIFIED** (`gi.readthedocs.io/en/latest/annotations/giannotations.html`). Twenty years of C functions annotated with ownership so that binding runtimes "can manage memory correctly".

**A12. Go `runtime.AddCleanup` (Go 1.24) — SECONDARY** (search results cite `tip.golang.org/doc/go1.24` and `go.dev/blog/cleanups-and-weak`; I did not fetch the release note itself). Attaches a cleanup function to an object; "not guaranteed to run immediately"; supersedes `SetFinalizer`. Reclamation-timed. **Marked secondary — do not rest anything on it.**

### Route B — a scope-bound statement

**B1. Go, 2009/2010 — VERIFIED, and it is the cautionary one.** Go blog "Defer, Panic, and Recover", Andrew Gerrand, **4 August 2010**, fetched 2026-09-14. The three rules verbatim: "A deferred function's arguments are evaluated when the defer statement is evaluated"; "Deferred function calls are executed in Last In First Out order **after the surrounding function returns**"; "Deferred functions may read and assign to the returning function's named return values."

**The function-scoped choice is the documented bug source.** Because Go's `defer` fires at *function* return and not at block exit, `defer` inside a loop accumulates. JetBrains ships an inspection for it (`jetbrains.com/help/inspectopedia/GoDeferInLoop.html`, "checks if a defer is called inside a for loop (which could potentially cause a resource leak)"); GitHub ships a linter package `deferinloop` that "flags defer statements placed directly inside for or range loop bodies" (`pkg.go.dev/github.com/github/gh-aw/pkg/linters/deferinloop`). Go also had to spend a release buying the cost back: Go 1.14 release notes, fetched 2026-09-14 — "This release improves the performance of most uses of `defer` to incur almost zero overhead compared to calling the deferred function directly."

**B2. Every later copy went block-scoped — VERIFIED as a pattern.** D's `scope(exit)` (dlang.org/spec/statement.html, fetched 2026-09-14: executes "when the scope exits normally or when it exits due to exception unwinding"), Swift's `defer` (fires when the enclosing *scope* exits, LIFO — official book section "Specifying Cleanup Actions": "Deferred actions are executed in the reverse of the order that they're written in your source code"; **quotation reached via search results, marked secondary** because my fetch of `docs.swift.org` returned only a stub), Zig's `defer` (ziglang.org/documentation/0.13.0/, fetched 2026-09-14: "defer executes an expression at the end of the current block"; inside a loop it runs each iteration), Odin's (odin-lang.org/docs/overview/, via search: "defers the execution of a statement until the end of the scope it is in", and explicitly "differs from Go's defer, which is function-exit"), Hare's (harelang.org, via search: defers "to the end of the current scope"). **That is five independent redesigns all correcting Go's one choice.** If Route B enters Heroes it must be block-scoped; the historical record on this is unanimous and the only dissenter is the originator.

**B3. Zig split `errdefer` out of `defer` — VERIFIED that the split exists** (ziglang.org/documentation/0.13.0/, fetched 2026-09-14: "errdefer is similar to defer, but only executes if the block returns an error"). The *reason* for the split as stated by Zig's maintainers is **unsourced** — my fetch of ziglang/zig issue #5605 returned only the opening post, not the maintainer replies. What I can source is the *shape of the need*: partial initialisation, where a later failure must undo an earlier acquisition but a success must not. Heroes has `?`, which is precisely that shape, so **if Route B enters, expect the same split to be demanded**, doubling its spec cost from the measured +108.

**B4. C itself is leaving Route C for Route B — VERIFIED and this is the newest fact in the report.** Jens Gustedt, "Defer available in gcc and clang", **15 February 2026**, fetched 2026-09-14: the feature is specified in **TS 25755**, edited by JeanHeyd Meneide, which "is now complete and moves through ISO's complicated publication procedures"; it is available in **Clang 22 and later** with `-fdefer-ts`, and via a macro fallback on GCC 9+. Caveats verbatim: "You'd always have to use the `defer` feature with curly braces to ensure that the gcc fallback works smoothly"; "There is no fallback for older clang version".

### Route C — refuse, and tell users to do it by hand

**C1. C, 1972–2026 — VERIFIED, and the precedent just expired.** C held Route C for over fifty years with `goto cleanup` as the answer. As of TS 25755 and Clang 22 it no longer does (B4). **This is the single strongest fact against Route C in this report**: the language Heroes emits, whose austerity is the usual justification for refusing, has itself concluded that the refusal did not hold. A Part 6 row refusing a scope-bound release in 2026 would be refusing something C adopted this year.

**C2. Objective-C/ARC kept a partial refusal — VERIFIED.** "ARC does not manage Core Foundation objects automatically"; CF objects stay manual, crossed with `__bridge` casts. A deliberate, documented, *scoped* refusal that has held since 2011 — evidence that a refusal can be stable **when it is a refusal about one clearly-fenced class of object**, not about the whole mechanism.

### Route D — the fourth route nobody listed: make the miss a compile error, insert nothing

This is your "a fourth that nobody has listed is exactly what this sitting wants to hear about". **Heroes is one annotation away from it**, because `consumes` (panel 145) is half of a linear type already.

**D1. Vault, Microsoft Research, PLDI 2001 — VERIFIED (secondary for the summary; paper at `microsoft.com/en-us/research/wp-content/uploads/2001/05/pldi01.pdf`).** DeLine & Fähndrich. "Vault enforces statically that resources cannot be leaked"; validated against the Windows 2000 kernel/driver interface. **What happened to it**: it did not ship as a product language.

**D2. Cyclone, ISMM 2004 — VERIFIED, and this is the most decision-relevant quotation in the report.** Hicks, Morrisett, Grossman, Jim, *Experience With Safe Manual Memory-Management in Cyclone*; I read the PDF directly (`homes.cs.washington.edu/~djg/papers/cyc_mm_experience.pdf`, fetched 2026-09-14). Verbatim, §3:

> "At join points in the control-flow graph, our analysis conservatively considers a value consumed if there is an incoming path on which it is consumed. For instance, if p is not consumed and we write: `if (rand()) free(p);` then the analysis treats p as consumed after the statement. In this situation, we issue a warning that p might leak, since the type-states do not match. Fortunately, we can link in the garbage collector to ensure the object is reclaimed. **We considered making this an error as in Vault [9], but found that exception handlers and shared unique pointers (described below) generated too many false alarms. Thus, we settle for a warning and can rely upon the GC as a safety net.**"

Also verbatim: "Unique pointers make it easy to support explicit deallocation, but they often force awkward coding idioms just to maintain uniqueness"; "Unique pointers are not a novel idea, but we found many challenges to implementing them in a full-scale safe language, as they interact poorly with other features such as exceptions, garbage collection, type abstraction, the address-of operator, undefined evaluation order"; and "placing unique pointers in non-unique objects can lead to subtle 'leaks.'"

Measured cost, Table 1 (non-comment lines; C baseline, then port delta, then the additional delta for manual mechanisms): Boa 5217 → +284 (5%) → +91 (1%); BetaFTPD 1146 → +191 (16%) → +225 (21%); Epic 2123 → +217 (10%) → +114 (5%); Kiss-FFT 453 → +73 (16%) → +20 (4%).

**What happened to Cyclone**: version 1.0 released 8 May 2006; "no longer supported… the core research project has finished and the developers have moved on" (cyclone.thelanguage.org / Wikipedia, secondary).

**The load-bearing caveat for Heroes**: Cyclone backed off from *error* to *warning* **because it had a GC to fall back on**. Heroes has none. But note what actually generated Cyclone's false alarms: **exception handlers** — invisible non-local exit. Heroes has no exceptions; its non-local exit is `?`, which is written in the source and already has a slot-table exit sweep behind it (`selfhost/ir/own.hero` rule 5, per the brief). That is a material difference, and I flag it as a **hypothesis, not a finding**: nobody has published the experiment "linear handle checking against explicit error-as-value propagation", and I could not find one.

**D3. Austral — VERIFIED.** Fernando Borretti, "Introducing Austral", **28 December 2022**, fetched 2026-09-14: "There are no exceptions and no stack unwinding and no destructors"; "there are no implicit function calls. If it's not in the source code, it's not happening"; "A value of a linear type must be used once and only once. Not *can*: *must*." Checker size: "Austral's equivalent of a borrow checker is less than 600 lines of code, including the implementation of borrowing and other ergonomic features." Austral's FFI pattern, from the spec (`austral-lang.org/spec/spec.html`, fetched 2026-09-14): the raw C handle is a "plain old fashioned (unrestricted, non-linear) file handle" wrapped inside a linear type, destructured to call C, then reconstructed. **Austral is the closest language in the world to Heroes' stated premises and it chose Route D.**

**D4. Borretti, "Linear Types and Exceptions", 7 August 2021 — VERIFIED, fetched 2026-09-14.** Verbatim: "A linear type system guarantees all resources allocated by a terminating program will be freed, and none will be used after being freed. This guarantee is lost with the introduction of exceptions: we can throw an exception before the consumer of a linear resource is called, leaking the resource." His two recommended escapes: affine types with implicit destructors (Rust), **or** dividing errors into terminating and recoverable-as-values. **Heroes already took the second escape.** The historical obstacle to Route D is the one obstacle Heroes has already removed.

**D5. Vale's "Higher RAII" — VERIFIED (primary: Evan Ovadia is Vale's author).** `verdagon.dev/blog/higher-raii-uses-linear-types`, 14 May 2024, fetched 2026-09-14: "Higher RAII = Linear types + whitelisted destroyers"; a linear struct leaving scope undestroyed yields "ERROR: Undestructed linear `x`". On the comparison your sitting cares about: "defer can make sure something happens at the end of your function, but RAII can make sure that something happens *even past* the end of your function."

### The value-semantics comparison (your item 4)

**Hylo (ex-Val) — VERIFIED**, and it is the one that should give the panel pause. Hylo is the flagship *mutable value semantics* language, with no aliasing among its own values — the same premise Heroes names. From the Hylo specification (`github.com/hylo-lang/specification/blob/main/spec.md`, fetched 2026-09-14): "A function declaration at type scope declared with `deinit` is called a deinitializer declaration. It introduces one sink method"; "The lifetime of an object of type `A` ends when a bound call to any of its sink methods has returned or when the memory location that it occupies has reach the end of its lifetime." **The closest relative to Heroes' design premise added a deinitializer.** I found **no statement about C or foreign function interop** in that specification — a genuine silence, not a claim about absence in the implementation.

**The measured silence you asked for.** Searching the terms listed at the top, I found **no language that has value semantics, no destructors, and a C FFI, and that answered the handle-release question with anything other than a scope-bound `defer`.** The four languages that best fit the premise — **Zig, Odin, Hare, Go** — all chose Route B, all four have no destructors, and the three post-Go ones all made it block-scoped. **Not one of them attaches a releaser to a type.** The reason is structural and worth saying plainly: *none of them has a per-type hook at all*, so Route A was never on their table. Heroes does have one (`record … tag …`, per the brief's spelling), which is why its option set is wider than theirs. **That is the finding**: Heroes is not choosing between Route A and Route B on the same terms those languages did; it has a door they did not.

## argument (≤120 words)

Route A is not novel and not exotic: naming a foreign function as a type's releaser is the dominant idiom for foreign resources, found in twelve systems I verified. What is rare is firing it at scope exit — exactly one shipped language does it, **Vala**, which compiles to C and whose GNOME binding reads `free_function = "sqlite3_finalize"` on `sqlite3_stmt`, the literal proposal. Vala has survived twenty years on it. Route B's precedent is broader but its originator, Go, shipped the loop leak, and five successors corrected it; Zig then needed `errdefer` too. Route C's one long holder, C, abandoned it in 2026: TS 25755, Clang 22. Refusing now refuses what C adopted.

## falsifiable prediction

**Route A's per-type annotation will prove insufficient on its own, and a per-FUNCTION opt-out will be required within the same milestone.** Two independent precedents predict this: Swift requires `SWIFT_RETURNS_RETAINED`/`SWIFT_RETURNS_UNRETAINED` on producing functions *in addition to* `SWIFT_SHARED_REFERENCE`; Vala requires `unowned` on non-owning references *in addition to* `free_function`. Both discovered that "this type is released by F" does not say "this particular call did or did not hand you ownership."

**Concretely checkable against the census, and cheap**: of the twelve acquire-and-release pairs read by hand across `examples/` and `selfhost/`, **at least one of the five handle types has at least one C function that returns an instance the caller must NOT release** (SQLite's `sqlite3_errmsg` and `sqlite3_column_*` return memory owned by the connection or statement; `sqlite3_next_stmt`, which the census itself used, returns a borrowed statement). **Falsifier**: all five handle types, across every producing function bound in `examples/`, return owned instances only — in which case `released` alone suffices and my prediction is wrong. This is **unrun**; it is a read of the twelve pairs, not a build.

Second, weaker prediction: **if Route B enters instead, `errdefer`'s split will be demanded within two milestones**, because Heroes' `?` is the partial-initialisation shape that forced Zig's split. Falsifier: a milestone of `?`-heavy `examples/` in which no site wants release-on-error-only.

## condition — what precedent would change my reading

Three, in descending force.

1. **A language with no GC that made an unreleased handle a hard compile error and kept it.** Cyclone tried, and its own words are that it backed off to a warning "and can rely upon the GC as a safety net." If somebody shows me a GC-free language that held the error — Austral shipping a real C binding under linear types would do it, or ATS — then **Route D outranks Route A**, because it costs zero spec tokens at the exit sweep and turns the silent leak into a refusal, which is what this project prefers. I could not find that evidence; I searched the terms listed above and found linear-type languages with published designs and no published field experience on foreign handles.

2. **Evidence that Vala's `free_function` on compact classes leaks on a non-local exit path.** I verified the annotation and the ownership model; I did **not** verify, by reading Vala's generated C, that the free is emitted on every exit path of a block including early return. If it is not, Route A's closest precedent is weaker than I have written it. **Marked unverified, and it is the first thing I would check with another hour.**

3. **A primary statement from Zig's maintainers on why `errdefer` was split out.** I could source that the split exists but not why. If the reason turns out to be something other than partial initialisation, my second prediction weakens.

## provenance note

*Heroes of code* / *Gli eroi del codice* is by this repository's author and is therefore **not a citation here**; per the instruction to this seat, naming it is naming a route. It told me which room to walk into — Cyclone's, and the Wirth-to-Oberon line behind the no-destructors premise. The evidence in this report is the ISMM 2004 PDF, the GNOME Vala `.vapi`, the WG14 documents and the vendor manuals, each with the URL and the date I fetched it. No `unverified` in this report was promoted by the book.

**Items marked secondary or unsourced, collected so the completeness critic does not have to hunt**: Swift `defer` book quotation (secondary — fetch returned a stub); Go 1.24 `runtime.AddCleanup` (secondary); Vala 2006/2007 dates and Cyclone 1.0 date and Splint's end-of-maintenance (secondary, Wikipedia); Zig's stated *reason* for splitting `errdefer` (**unsourced**); Zig's "Zen" text (**dropped — could not fetch primary**); Vala's emission of the free call on early-return paths (**unverified**); the GLib boxed-types 2002 date (**secondary**).
