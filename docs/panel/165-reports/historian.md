# Panel 165 — historian's report

**Seat:** historian (advisory, no veto). **Date:** 2026-09-19.
**Question:** who has let a foreign-function declaration state a C array
parameter's **extent** and had the binding language **check** it — and what
happened to them?

**Written out verbatim by the coordinator**, because this seat has no write tool
(panels 143 and 144 both recorded that it could not be audited for exactly that
reason). One `unverified` item was settled by the coordinator after the report
arrived and is marked in place.

**Method note.** Every row below was reached by web search or by reading a file
on this Mac, and each carries its source URL. Where a source was summarised
rather than quoted, or where the fetcher returned something internally
inconsistent, the cell is marked `partially-verified` and says why. Where I could
not confirm a thing, the word is `unverified`.

## verdict

**`approve` (advisory)** — route 6 has a long, working precedent and its one
recorded disaster is a disaster about *meaning*, not about *checking*, and Heroes
has already closed that door. Two conditions below.

## precedents

| # | Language / binding | Is there a place to write `N`? | Checked, or decoration? | Caller who gets `N` wrong | Marker |
|---|---|---|---|---|---|
| 1 | **Rust — `bindgen`, default** | **No.** `void my_func(uint8_t arr[20]);` → `pub fn my_func(arr: *mut u8);` | N/A — extent discarded at generation | nothing is checked | **verified** ([#1561](https://github.com/rust-lang/rust-bindgen/issues/1561)) |
| 2 | **Rust — `bindgen --array-pointers-in-arguments`** | **Yes, opt-in**: "Translate arrays `T arr[size]` into array pointers `*mut [T; size]`". Merged 2019-05-22 | **Checked, by ordinary Rust type identity** — `*mut [u8; 8]` ≠ `*mut [u8; 20]` | compile error at the call | **verified** for flag/default/date ([docs.rs](https://docs.rs/bindgen/latest/bindgen/struct.Builder.html#method.array_pointers_in_arguments), [PR #1564](https://github.com/rust-lang/rust-bindgen/pull/1564)); **partially-verified** for the diagnostic |
| 3 | **Rust — `libc` (hand-written)** | not for parameters; for **constants**, one literal per platform module | kept honest by an external instrument: `ctest` generates a C file including the real headers and validates the Rust side against it | a wrong constant is caught by ctest on that platform, not by the compiler | **verified** ([1024](https://docs.rs/libc/latest/aarch64-apple-darwin/libc/constant.L_tmpnam.html), [20](https://docs.rs/libc/latest/x86_64-unknown-linux-gnu/libc/constant.L_tmpnam.html), [ctest](https://github.com/rust-lang/libc/tree/main/ctest)) |
| 4 | **Zig — `@cImport` + `extern fn`** | **No, and deliberately removed.** #6535 "Disallow array parameters in C calling convention functions", PR #6582 merged 2020-10-08. Reason: `[5]u8` reads as by-value, and "the C code is free to modify the contents of the array, which is against Zig rules" | N/A — spelling refused | compile error on the *declaration* | **verified** ([#6535](https://github.com/ziglang/zig/issues/6535), [#6582](https://github.com/ziglang/zig/pull/6582)) |
| 5 | **Go — `cgo`** | **No.** *"In C, a function argument written as a fixed size array actually requires a pointer to the first element… C compilers are aware of this calling convention and adjust the call accordingly, but Go cannot. In Go, you must pass the pointer to the first element explicitly: `C.f(&C.x[0])`."* | N/A | caller writes `&C.x[0]`; length is the programmer's problem | **verified** ([cmd/cgo](https://pkg.go.dev/cmd/cgo)) |
| 6 | **D — hand-written `extern(C)`** — **the cautionary tale** | **Yes, and it means something different from what C means.** D passes the static array **by value** | neither checked nor decoration: a **silently wrong ABI** | **Segfault.** Bug 8887, reopened P1 critical: *"This is regression from D1 behaviour that makes all of our existing extern(C) bindings segfault when compiled with D2 compiler."* | **verified** for behaviour and quote ([forum mirror](https://forum.dlang.org/thread/bug-8887-3@https.issues.dlang.org/)); **unverified**: final disposition |
| 7 | **D — `ImportC`** | **No.** applies the C adjustment itself, citing C11 6.7.6.3 | N/A | N/A | **partially-verified** ([spec](https://dlang.org/spec/importc.html)) |
| 8 | **Odin — `foreign` / `core:c/libc`** | **No**; uses a **multi-pointer**: `tmpnam :: proc(s: [^]char) -> [^]char ---`. `[^]T` is explicitly unbounds-checked | decoration-free: there is no `N` | programmer's problem | **verified** for the signature ([stdio.odin](https://raw.githubusercontent.com/odin-lang/Odin/master/core/c/libc/stdio.odin)) |
| 9 | **Swift — ClangImporter, classic path** | **No place for a parameter extent.** For *variables* it does carry it: "C's fixed-size arrays are imported as Swift tuples" | for variables: tuple arity is a Swift type, so checked. For parameters: nothing | — | **verified** for the tuple rule ([HowSwiftImportsCAPIs](https://github.com/swiftlang/swift/blob/main/docs/HowSwiftImportsCAPIs.md)); **unverified** for parameters |
| 10 | **Swift — `SafeInteropWrappers`** | **Yes, on a pointer, via `__counted_by`/`__sized_by`** → `UnsafeBufferPointer`, `Span<T>`, `RawSpan`. Behind `-enable-experimental-feature SafeInteropWrappers` | **checked at the interop boundary**; with `-fbounds-safety` on the C side too | trap at the boundary rather than a silent overrun | **verified** ([swift.org](https://www.swift.org/documentation/cxx-interop/safe-interop/)); **unverified** whether still experimental today |
| 11 | **Ada — `Interfaces.C` + `pragma Import (C, …)`** — **the working precedent** | **Yes.** the formal may be a *constrained* array subtype; C still receives `t*` | **Checked.** a view conversion to the formal's nominal subtype "(which might raise Constraint_Error)"; for a constrained array target "a check is made that the length of each dimension… equals the length of the corresponding dimension of the target subtype" | **`Constraint_Error`** — statically diagnosable when lengths are static | **verified** for [B.3(70)](https://www.adaic.org/resources/add_content/standards/05rm/html/RM-B-3.html) and [4.6(37)](https://ada-lang.io/docs/arm/AA-4/AA-4.6/); **partially-verified** for 6.4.1's paragraph number |
| 12 | **Nim — `importc` / `c2nim`** | **No.** *"C conflates pointers with arrays, Nim does not."* `#isarray` gives `ptr UncheckedArray[T]` — a type whose name says it is unchecked | decoration-free | programmer's problem | **verified** ([c2nim](https://github.com/nim-lang/c2nim/blob/master/doc/c2nim.rst)) |
| 13 | **Pascal (Wirth lineage) — the ancestor** | **Yes, and it was the original rule.** the array parameter's type includes its index type; ISO 7185 added the conformant array parameter | checked | compile error / bounds failure | **partially-verified** ([ACK](https://tack.sourceforge.net/olddocs/pascal.html)) |

**Tally.** Of the ten binding paths a generator or hand-writer actually uses today
(1, 2, 4, 5, 6, 7, 8, 9, 11, 12), **eight discard the extent by default**. **Two
carry and check it: Ada always, bindgen on request.** Exactly one — D — carried
the *spelling* without the *meaning*, and that one segfaulted.

## Thread A — platform-varying constants (`L_tmpnam` 1024 Darwin, 20 glibc)

**Family 1 — bake the number in, one per platform, and verify it with an
instrument.** Rust `libc` ships `L_tmpnam = 1024` for aarch64-apple-darwin and
`= 20` for x86_64-unknown-linux-gnu in separate per-target modules — **identical
to this panel's own measured values** — and what keeps them honest is `ctest`,
not review. Odin hand-writes `:: 15` (Windows, comment `"\\" + 12 + NUL`),
`:: 20` (Linux), `:: 1024` (Darwin), and **does not define it at all** on
JS/OpenBSD/NetBSD/FreeBSD. **Note the third value: Windows is 15**, and this
panel's Windows column is absent — Odin's own binding is evidence that a fourth
platform carries a *third* number. Go's `x/sys` generates
`zerrors_${GOOS}_${GOARCH}.go` by compiling and running a C program per target.

**Family 2 — do not bake the number in; let the C compiler resolve it.** Nim
writes the constant as an imported symbol:
`IF_NAMESIZE {.importc: "IF_NAMESIZE", header: "<net/if.h>".}: cint`. The number
never appears in the binding.

**What this says for Heroes.** The shared brief measured that Heroes is in
**family 2** and that it works on both platforms. That is the Nim shape and it is
the better half of the split: family 1 needs a whole second instrument to stay
true, and Odin's table has already gone stale on four targets.

**And here is the collision the panel should price.** Route 6 checks an extent
*at check time*. Family 2 knows the portable number only *at C-compile time*.
**No language in this survey does both at once.** Ada checks a number its own
compiler knows; Nim knows the portable number and checks nothing; bindgen's
`*mut [T; N]` takes `N` from the local header at generation time and is therefore
family 1 with all of family 1's staleness. If route 6 admits only a literal
extent it cannot express `char[L_tmpnam]` — **the exact glibc spelling of the one
function in this census that matters** — and the shared brief already found that
**all 14 Linux `NAMED` extents are macro constants, not literals**.

## Thread B — annotation-carried extents (`__counted_by` / `_LIBC_COUNT`)

**Does this route exist? Yes. Is it live? Barely — one consumer.**

- The attributes are real: `__counted_by`, `__sized_by`, `__ended_by` and others
  in `clang/lib/Headers/ptrcheck.h`, enabled per-file by `-fbounds-safety`
  ([adoption guide](https://clang.llvm.org/docs/BoundsSafetyAdoptionGuide.html)) — **verified**.
- **Exactly one binding language reads them today: Swift**, experimental — **verified**.
- **bindgen does not**: no CHANGELOG entry for `counted_by`, `sized_by`,
  `bounds safety` or `ptrcheck` across every documented version — **verified as a
  search of that document**, which is a claim about its vocabulary, not about
  what bindgen can do.
- **Zig does not**: [#2457](https://github.com/ziglang/zig/issues/2457) open, no
  milestone — **partially-verified** (the fetch returned an inconsistent page).
- **Odin, D, Go, Nim, Ada:** no evidence found. **unverified** — a negative
  resting on my search vocabulary, not on the world.

**The measurement that matters most**, read on this Mac,
`MacOSX.sdk/usr/include/_bounds.h:29-65`:

```c
#ifdef __LIBC_STAGED_BOUNDS_SAFETY_ATTRIBUTES /* compiler-defined */
#define _LIBC_COUNT(x)		__counted_by(x)
#else
#define _LIBC_COUNT(x)
```

So on Darwin `_LIBC_COUNT(L_tmpnam)` expands to nothing unless the compiler
defines that macro. Whether Apple clang does was **unverified** from this seat —
*"one command settles it; I have no shell"*.

> **SETTLED BY THE COORDINATOR, 2026-09-19, after this report arrived.**
> `clang -dM -E -x c /dev/null | grep -i LIBC_STAGED` returns **nothing**: the
> macro is **not defined by default**. And preprocessing the real header,
> `clang -E -P` over `#include <stdio.h>`, yields line 203:
> `char * tmpnam(char *);` — **the annotation is gone.** So on Darwin the
> extent is invisible to any tool that merely preprocesses, and Thread B is a
> direction rather than an available source of extents today.

One more from Swift's page, worth holding beside route 6's shape: **"Bounds
annotations on global variables or struct fields are ignored: only parameters and
return values are considered."** The shared brief established that route 6's
argument in Heroes can *only* be a record field. These are not the same
restriction, but a panel deciding where extents are honoured should notice that
the one shipping implementation drew a line in roughly this neighbourhood.

## argument

Route 6 is not novel and is not discredited. Ada has shipped it for decades: a
constrained array subtype as the formal of a Convention-C import, passed to C as
`t*`, length-checked on entry, `Constraint_Error` when it disagrees. bindgen
ships the same idea as an opt-in flag emitting `*mut [T; N]`. Eight of eight
default generators throw `N` away, but that is caution about headers they did not
write, not a verdict on a hand-written binding. D allowed the spelling without
fixing its meaning and segfaulted real bindings; Zig banned the spelling in 2020
instead. Heroes already refuses the decay, so D's trap is shut. The live risk is
scope, not soundness.

## condition

1. **A precedent that checks an extent it does not know at check time.** If a seat
   finds a C-binding language where the declared extent may be a header-resolved
   constant (`char[L_tmpnam]`) *and* is still checked, Thread A's tension
   dissolves and route 6 should be adopted with named extents from day one. I
   searched and found none; that negative rests on my vocabulary. **Absent such a
   precedent: adopt route 6 with literal extents only, and write into §4.19 that
   the extent is a compile-time check and that what crosses to C is a pointer** —
   the sentence D never wrote.
2. **Evidence that the annotation route has a second consumer.** One shipping
   consumer behind an experimental flag is a direction, not a precedent.

A third, weaker one: a documented case of Ada's constrained-array C bindings
*failing in the field* would turn my approve into an object. I looked and did not
find it. **unverified.**

## falsifiable prediction

**Within two milestones of route 6 being adopted, this tree will contain zero
`extern` array-parameter declarations whose extent is stated as an `extern`
`constant` rather than as an integer literal.** Scored by grep over `selfhost/`,
`tests/golden/` and `examples/` for an `extern` parameter of the form
`: iN[<identifier>]`. **Falsified by a single `function f(s: i8[L_tmpnam])` that
compiles.**

## What I could not verify, stated as such

- **Swift's treatment of a C parameter written `char buf[8]`.** No primary source.
  Do not let this report be cited for it.
- **Whether `SafeInteropWrappers` is still experimental today.**
- **The disposition of D Issue 8887.**
- **ISO 7185's conformance levels**, and any verbatim statement of Pascal's
  strict array-parameter matching rule.
- **That no language other than Swift reads `__counted_by`.** A negative resting
  on my searches, not on the world.
- **`ImportC`'s exact source comment**, which came through a summary.
