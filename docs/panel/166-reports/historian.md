# Panel 166 — historian's report

**Seat:** historian (advisory, no veto). **Date:** 2026-09-19.
**Rule applied:** every claim carries a source URL and a marker. Nothing here was
run on this machine; this seat has no shell. Where a claim needs a command, it
says so and names the command.

**Written out verbatim by the coordinator**, because this seat has no write tool.
Its falsifiable prediction was settled by the coordinator after the report
arrived and is marked in place.

## verdict

**approve, with one objection** — approve routes **B + D + E** as the combination
with the strongest precedent; approve **C** only in the narrowed form below;
**object to G standing alone**, because it is the one route in this survey with
no precedent anywhere.

## Thread A — the counted pointer at an FFI boundary

Can the binding declare that a length parameter belongs to a pointer parameter,
and does anything check it?

| Language | Can the relation be declared? | Is it checked, and when? | Marker |
|---|---|---|---|
| **Rust** | No. `p.as_mut_ptr()` and `p.len()` are two independent expressions the caller writes. Inbound, the relation is a prose safety contract. | Never by the compiler: *"Behavior is undefined if… `data` must be non-null, valid for reads for `len * size_of::<T>()` many bytes"* ([from_raw_parts](https://doc.rust-lang.org/std/slice/fn.from_raw_parts.html)) | **verified** |
| **Rust / bindgen** | The brief asked what `bindgen` does with `__counted_by`. **Nothing found** — [an issue search for `counted_by`](https://github.com/rust-lang/rust-bindgen/issues?q=counted_by) returns *"No results."* | n/a | **verified** as a negative about that tracker, with the search term named (CL-018) |
| **Zig** | No. A slice `[]T` *is* a pointer+length pair and is bounds-checked, but **a slice may not cross the C ABI** ([#12930](https://github.com/ziglang/zig/issues/12930)); at the boundary you pass `[*]T`, "many-item pointer to **unknown number of items**", plus a `usize`. | Nothing checks the pair. Bounds checking exists up to the `extern fn` and stops. | **verified** |
| **Go** | No. cgo declares C types; there is no size annotation. Its rules are about **pointer lifetime and the GC**, not extent. | The rules that exist *are* checked — *"checked dynamically at runtime… controlled by the cgocheck setting of GODEBUG"* ([cmd/cgo](https://pkg.go.dev/cmd/cgo)) — but extent is not among them. | **verified** |
| **D** | No. The spec gives D's `T[]` **"no equivalent"** in C ([interfaceToC](https://dlang.org/spec/interfaceToC.html)). | Nothing. | **verified** |
| **Odin** | No, but it *names* the hazard in the type: `[^]T`, *"the main purpose of this type is to aid with `foreign` code"*, with *"index (without any bounds checking)"*. | Partially by construction, on the inbound slice only. | **verified** |
| **Swift** | **Yes — the closest working system.** The importer reads `__counted_by` and synthesises `UnsafeBufferPointer`/`Span` overloads. | Yes, at runtime. **Still experimental**, by the page's own words: *"behind an experimental feature flag on the Swift 6.2 release branch"*, and the [Swift 6.3 announcement](https://www.swift.org/blog/swift-6.3-released/) does **not** mention it at all. | **verified**, both the flag text and the absence from 6.3 |
| **Swift — the struct-field carve-out** | *"Bounds annotations on global variables or struct fields are ignored: only parameters and return values are considered."* **The page gives no reason.** | — | statement **verified**; the *reason* **unverified** |
| **Ada** | No. RM B.3[70/5]: an array parameter is passed as `t*`; [62.a/3] *"the bounds can simply be dropped"*. | Nothing. Ada forbids the case where bounds would be needed rather than checking them. | **verified** ([RM B.3](https://ada-lang.io/docs/arm/AA-B/AA-B.3/)) |
| **Nim** | No automatic pointer+length; [#5437](https://github.com/nim-lang/Nim/issues/5437) asked for a library type. | Nothing. | **partially verified** |
| **C# / P-Invoke** | **Yes, syntactically — and this is the punchline for route C.** `[MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]`, *"similar to `size_is` in COM"*. | **In the direction Heroes cares about it is not checked — it is not even used**: *"The `MarshalAsAttribute` has **no effect** on marshalling managed arrays to unmanaged code. In that direction, the array size is determined by examination."* Inbound it is **trusted**: *"the marshaller checks the `MarshalAsAttribute`… If the array size is not specified, only one element is marshalled."* | **verified** ([arrays](https://learn.microsoft.com/en-us/dotnet/standard/native-interop/default-marshalling-for-arrays), [SizeParamIndex](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.marshalasattribute.sizeparamindex)) |
| **C# — a contradiction worth knowing** | The `SizeParamIndex` page says it *"supports managed-to-unmanaged and unmanaged-to-managed calls"*; the arrays page says the attribute has *no effect* managed-to-unmanaged. Two Microsoft pages disagree. | — | contradiction **verified**; its resolution **not** |
| **C# — what *is* validated** | The declaration's **shape**, at marshal time: an out-of-range index throws *"Array size control parameter index is out of range"*; a non-integral one throws `MarshalDirectiveException`. | Marshal time, never against the actual length. | **partially verified** ([116154](https://github.com/dotnet/runtime/issues/116154), [7315](https://github.com/dotnet/runtime/issues/7315)) |

### The C side, because Heroes emits C that clang compiles

**This is the table that actually prices routes C and E, and nobody in the panel
had read it.**

| Mechanism | What it declares | What checks it | What happened to it | Marker |
|---|---|---|---|---|
| **C99 `[static N]`** | a **minimum** extent, in the prototype, as part of the type | **clang diagnoses the call site at compile time**: `array argument is too small; contains 9 elements, callee requires at least 10` | Alive and being adopted now — [LWN, 1 Dec 2025](https://lwn.net/Articles/1046840/) on using it for kernel crypto parameters. Its documented limit: *"will only generate a warning… not catch all mistakes, though it is sufficient to prevent memory-safety and swapped-argument problems."* | **verified** (spec text, clang test file, LWN) |
| **GCC `__attribute__((access(write_only, 1, 2)))`** | exactly route C: this pointer, that size, and the direction | **GCC checks the call site at compile time**: `warning: 'init_array' accessing 128 bytes in a region of size 32`. GCC 10. | **This is the cautionary tale.** Linux disabled `-Wstringop-overflow` kernel-wide for GCC in **6.8** (Torvalds, Feb 2024: *"It turns out it was never just gcc-11 that was broken"*). The declaration was cheap; the **inference engine behind the check** produced false positives the largest C codebase in the world would not live with. | semantics and origin **verified**; the 6.8 disabling **verified** ([Phoronix](https://www.phoronix.com/news/Linux-Drop-GCC--Wstringop-of)) |
| **Does clang implement `access`?** | — | — | A kernel patch states it is *"supported since gcc >= 11 but not currently supported by clang"* ([LKML](https://lkml.iu.edu/2209.2/09080.html)) | **partially verified — this is my prediction below** |
| **clang `-fbounds-safety` / `__counted_by`** | the full route C, as a C language extension | *"the compiler inserts a bounds check before `p` is dereferenced"*; *"deterministically trap before out-of-bounds memory is accessed"* | Two facts in tension on one page: *"adopted on millions of lines of production C code"* — and *"NOTE: This is a design document and the feature is not available for users yet."* | **verified** ([BoundsSafety](https://clang.llvm.org/docs/BoundsSafety.html)) |
| **Microsoft SAL** — `_Out_writes_(n)` | the same relation, in the header, since the 2000s | not by the compiler — by a **separate static analyser**, `/analyze` | shipped across the Windows headers; never became a compile-time language rule | **partially verified** |

**The pattern across those five, and it is the finding of this report.** Declaring
the relation is the easy half and has been done five separate times over
twenty-five years. Every system that then tried to *check* it landed in one of
three places: **check only what is a compile-time constant** (C99 `[static N]`,
which is why it survives), **check at runtime with a trap** (clang
`-fbounds-safety`, Swift's importer), or **infer the object's size and warn**
(GCC `access`, SAL) — which is the one that got switched off by its biggest
customer.

## Thread B — writing through a value the language calls immutable

The question is not *do they refuse it*; C can always cast. It is **where the
line is drawn and whether the language reference draws it.**

| Language | Where the line is drawn | Reference or folklore? | Marker |
|---|---|---|---|
| **Rust** | In the **reference**, as UB: *"The bytes owned by an immutable binding or immutable `static` are immutable, unless those bytes are part of an `UnsafeCell<U>`"*, and a mutation is *"any write of more than 0 bytes which overlaps"* | **Reference**, and reachable only from inside `unsafe` | **verified** |
| **D** | In the **spec**, with the example: *"`*q = 3; // allowed by compiler, but result is undefined behavior`"*, and *"one must assume the responsibility to ensure the immutability of the data"* | **Spec**, attached to an explicit `cast` | **verified** ([const3](https://dlang.org/spec/const3.html)) |
| **Go** | In the **library reference**, on the call that produces the pointer: *"Since Go strings are immutable, the bytes returned by StringData must not be modified."* `SliceData` carries no such sentence. | **Reference**, reachable only through `unsafe` | **verified** |
| **Ada** | **In the type system, and visible in the C prototype**: an array parameter is passed as `t*` *"with the const modifier if the Ada mode is in"*. The only language here where **the mode of the binding changes the C signature the callee sees.** | **Reference** | **verified** |
| **Nim** | Historically in a **named operator**, `unsafeAddr`, for exactly this. [RFC #369](https://github.com/nim-lang/RFCs/issues/369) removed the distinction — Araq: *"The distinction between unsafeAddr and addr makes little sense."* Nim 2.0 deprecated it. **A language that had this exact distinction and deliberately dropped it.** | the obligation moved into a doc comment, then the operator carrying the warning was deprecated | RFC and deprecation **verified**; the doc-comment wording **partially verified** |
| **Zig** | In the type system up to the boundary (`*T` coerces to `*const T`, *"not vice versa"*), then `@constCast`, with no reference-level statement about mutation | **Folklore.** A Ziggit thread has no core-developer answer and no reference citation; behaviour differed between 0.11 and 0.12 | coercion **verified**; the UB status **unverified** |
| **Swift** | **At escape, not at mutation**: *"Escaping the pointer to use after the execution of the function is undefined behavior."* `UnsafeMutablePointer(mutating:)` appears with **no prohibition on writing attached** | **Reference** — but the line is lifetime, not mutability | **verified** |

### The two rows that bear directly on routes A and B

| System | What it does | Marker |
|---|---|---|
| **Swift `inout`** | **Marks the write at the call site and refuses constants outright**: *"You place an ampersand (`&`) directly before a variable's name… to indicate that it can be modified"*, and *"You can only pass a variable… You can't pass a constant or a literal value, because constants and literals can't be modified."* | **verified** |
| **Hylo** | The same rule, stated as a rule about the **marker** rather than about addresses: *"arguments to `inout` parameters must be mutable and marked with an ampersand (`&`) at the call site."* In Hylo `&` does not mean "address of"; it marks a mutation. | **verified** |
| **Oberon (Wirth)** | The ancestral form of A/B: all unsafe operations live in a pseudo-module `SYSTEM` whose *"name would appear in the prominently visible import list of every module making use of such low-level facilities"*. Unsafety is not forbidden — it is made **impossible to reach without writing its name**. | **verified** |

## Advisory verdict on the seven routes

| Route | Precedent | Reading |
|---|---|---|
| **A** refuse `.ptr()` from an immutable binding | Swift refuses a constant to an `inout` parameter, verbatim | **Sound but blunt.** Every system that drew this line drew it **per direction**, not per lend: GCC's `read_only`/`write_only`, SAL's `_In_reads_`/`_Out_writes_`, Ada's `in` meaning `const t*`. A route refusing the **read** as well has no precedent here. |
| **B** panel 164 R2, `@`-marked `ptr` takes `@field` | **The strongest precedent of the seven.** Swift and Hylo both require an explicit call-site marker and both refuse constants there; Ada does it in the type system | **Approve.** The mainstream answer, twice independently arrived at, and it closes 065 at the declaration rather than at the binding. |
| **C** the counted parameter | Done five times: C# (declared, **not used** outbound), SAL (separate analyser), GCC (checked, **then switched off by Linux 6.8**), clang `-fbounds-safety` (traps, "not available yet"), Swift (runtime, experimental, **ignores struct fields**) | **Approve narrowed.** Adopt the declaration; check **only where both sides are compile-time constants** and stay silent otherwise — the C99 `[static N]` discipline, the one member of the family nobody had to turn off. **Do not build size inference**; that is the exact thing Torvalds disabled. |
| **D** `len()` on a fixed field | Universal: D `.length`, Zig `.len`, Odin `len`, Rust `.len()` | **Approve.** Its absence is the anomaly, not its presence. |
| **E** a named extent `i8[SL_NAME_LEN]` | Ada declares bounds by named constant as a matter of course; C's `[static N]` is exactly a named-or-literal extent in the type | **Approve**, and note it is the enabler for C and D being *portable* rather than tied to a literal. |
| **F** refuse the lend, back to the copy | Go pins and forbids retention but **still lets you pass the pointer**. **No language here removed a raw lend once it had it.** | **Object, advisory.** Withdrawing a two-day-old capability has no precedent, and it leaves `getcwd` unwritable — the gap it was opened for. |
| **G** write the hole into the specification | What Rust, D, Go, Nim and Swift all do — **and every one writes it next to a construct the programmer had to spell**: `unsafe`, `cast(int*)`, `unsafe.Pointer`, `addr`, `UnsafeMutablePointer` | **Object, and this is my one real objection.** Heroes' hole is reachable from `t = sl_make()` + `sl_fill(p: t.name.ptr(), n: 8)` — no marker anywhere. I found **no precedent for documenting a memory-corruption hole reachable from unmarked, ordinary-looking code.** G is admissible only as the *residue* left after B puts a marker in the program. |

## argument

Five systems declared a pointer/length relation before this one. C# declares it
and, outbound, does not use it. SAL needed a separate analyser. GCC checked it
and Linux switched the warnings off in 6.8. Clang's `-fbounds-safety` traps at
runtime and is still "not available for users". Swift's importer works, is
experimental, and ignores struct fields — Heroes' exact case. The survivor is
C99 `[static N]`: check only compile-time constants, stay silent otherwise. On
immutability, Swift, Hylo, Ada and Oberon all agree — mark the mutation where a
reader sees it. Every language documenting a hole documents it next to a word the
programmer typed. **Route G alone documents a hole with no word.**

## condition

My reading changes if the panel finds any of these:

1. **A language that removed a raw FFI lend after shipping it** without
   regressing its C bindings. That would make route F precedented; I found none.
2. **A system that checks a declared pointer/length relation by inferring object
   sizes and is still switched on in a large C codebase** — a counterexample to
   the Linux 6.8 story. That would let route C be ambitious instead of narrow.
3. **A stated reason from Swift for the struct-field carve-out.** If the reason is
   *"a field's count expression names another field, which a parameter-level
   wrapper cannot see"*, that is a direct warning about Heroes' case and
   strengthens route E over route C.
4. **A language reference documenting an immutability hole reachable without any
   marker in the program text.** That would retire my objection to G.

## one falsifiable prediction

**Clang on this Mac does not implement `__attribute__((access(...)))`:
`__has_attribute(access)` expands to `0`, and a TU declaring
`void sl_fill(char *p, long n) __attribute__((access(write_only, 1, 2)));` and
calling it as `sl_fill(s.name, 64)` on an 8-byte field compiles with no
diagnostic at `-Wall -Wextra -O2`.**

**Why this and not another.** It is the single fact that most changes route C's
price. If I am **wrong**, route C costs almost nothing at the checker — the
emitter writes one attribute and clang does the call-site checking `selfhost/`
cannot. If I am **right**, route C's check must be written in Heroes, and the
`[static N]` / `_Static_assert` discipline is the only mechanism on the table a C
compiler will enforce for free — **which makes route E load-bearing rather than
cosmetic.**

> **SETTLED BY THE COORDINATOR, 2026-09-19, and the prediction is CORRECT.**
>
> ```
> $ printf '#if __has_attribute(access)\n#error HAS_ACCESS\n#endif\n' | clang -x c -fsyntax-only -
> (no output — the #error did not fire, so __has_attribute(access) is 0)
>
> $ clang -c acc.c -o /dev/null -Wall -Wextra -O2
> acc.c:2:46: warning: unknown attribute 'access' ignored [-Wunknown-attributes]
> ```
>
> No call-site diagnostic; the attribute is ignored outright. **So route C's
> check must be written in Heroes, and route E is load-bearing.**
