# Panel 163 — historian

**Written out by the coordinator, verbatim, because this seat has no write tool.**

**Advisory, no veto.** Every claim below carries a URL fetched in this session, on 2026-09-18. Where I could not source something I say what I searched for. **I had no Bash tool in this seat**, so I could not grep `design.md §4.9` or `spec § 5`; nothing below rests on the repository's own text.

## verdict

**approve (advisory) — with a standing objection to route 3.**

Routes 1 (in the corrected shape below) and 2 both have long-running precedent at exactly this boundary. **Route 3 is the one route a major language shipped and then withdrew**, and it withdrew it in writing.

## precedents

### Route 3 — an out-parameter that does not require an initialised value

**1. Rust: `mem::uninitialized`, shipped 1.0, deprecated 1.39.0. VERIFIED.**

`MaybeUninit<T>` stabilised in **Rust 1.36.0, 2019-07-04**: *"The type `MaybeUninit<T>` has been stabilized. The Rust compiler will understand that it should not assume that a `MaybeUninit<T>` is a properly initialized `T`."* The same announcement calls the thing it replaces *"an incredibly dangerous operation that essentially cannot be used correctly as the Rust compiler assumes that values are properly initialized"*, and says *"Starting with Rust 1.39, the function `mem::uninitialized` will be deprecated."*
Source: https://blog.rust-lang.org/2019/07/04/Rust-1.36.0/

The deprecation notice itself, fetched today, reads **"Deprecated since 1.39.0: use `mem::MaybeUninit` instead"**, and gives the reason in words that are directly admissible here:

> "The function basically cannot be used correctly: it has the same effect as `MaybeUninit::uninit().assume_init()`. […] Truly uninitialized memory like what gets returned here is special in that the compiler knows that it does not have a fixed value. This makes it undefined behavior to have uninitialized data in a variable **even if that variable has an integer type**. […] Therefore, it is **immediate undefined behavior to call this function on nearly all types, including integer types and arrays of integer types, and even if the result is unused**."

Source: https://doc.rust-lang.org/std/mem/fn.uninitialized.html

The `MaybeUninit` docs restate the same rule from the other side: *"uninitialized memory is special in that it does not have a fixed value […] Reading the same uninitialized byte multiple times can give different results. This makes it undefined behavior to have uninitialized data in a variable even if that variable has an integer type, which otherwise can hold any fixed bit pattern."*
Source: https://doc.rust-lang.org/std/mem/union.MaybeUninit.html

**What it cost Rust.** Not a deprecation alone. A new library type (`MaybeUninit`), a new discipline (`assume_init`), a rewrite of every call site in the ecosystem, and a compiler lint — `invalid_value`, warn-by-default, which checks *"improper use of `mem::zeroed`, `mem::uninitialized`, `mem::transmute`, and `MaybeUninit::assume_init` that can cause undefined behavior"* and emits *"this code causes undefined behavior when executed / help: use `MaybeUninit<T>` instead"*.
Source: https://doc.rust-lang.org/rustc/lints/listing/warn-by-default.html

**The bearing on this sitting is exact.** `i8[256]` is an *array of an integer type*. Rust's published position is that an uninitialised one is immediate UB even if never read. Route 3 as written in `00-shared.md` is `mem::uninitialized` with a different spelling, and the brief's own note that **ASan cannot see an intra-object overflow in a C struct at all** means Heroes would ship it with strictly less instrumentation than Rust had when it withdrew it.

**2. Zig: `undefined`, still shipping. VERIFIED — and it is the mitigated form, not the bare one.**

Zig's reference: `undefined` means *"Not a meaningful value. Using this value would be a bug. The value will be unused, or overwritten before being used."* Reading one is illegal behaviour. And: *"In debug and safe mode, Zig writes 0xaa bytes to undefined memory"*, with the caveat that this *"is only an implementation feature, not a language semantic, so it is not guaranteed to be observable to code."*
Source: https://ziglang.org/documentation/master/#undefined

**What it cost Zig, and what it means here.** Zig pays for route 3 three times: a **mandatory keyword at the binding** (you cannot omit the initialiser, you must write `= undefined`, so the reader sees it), **runtime poisoning** in Debug and ReleaseSafe, and a named category of illegal behaviour the compiler catches only sometimes. Route 3 survives in Zig because it is explicit and poisoned. A Heroes route 3 with neither would be the Rust version, not the Zig version.

### Route 2 — a zero default for a fixed field in an `extern` record

**3. C: `= {0}` since C89, `= {}` since C23. VERIFIED.** *"All members that are not initialized explicitly are empty-initialized"*; `struct {int n;} s = {}` is an error until C23 and valid since.
Source: https://en.cppreference.com/w/c/language/struct_initialization

**4. Go: the zero value, and `unix.Utsname` is the motivating struct itself. VERIFIED.** The Go spec: *"If a variable has not yet been assigned a value, its value is the zero value for its type."*
Source: https://go.dev/ref/spec
And the struct, fetched from `golang.org/x/sys` today:

```go
// unix/ztypes_darwin_arm64.go
type Utsname struct { Sysname [256]byte; Nodename [256]byte; Release [256]byte; Version [256]byte; Machine [256]byte }
// unix/ztypes_linux.go
type Utsname struct { Sysname [65]byte; Nodename [65]byte; Release [65]byte; Version [65]byte; Machine [65]byte; Domainname [65]byte }
```
Sources: https://raw.githubusercontent.com/golang/sys/master/unix/ztypes_darwin_arm64.go · https://raw.githubusercontent.com/golang/sys/master/unix/ztypes_linux.go

**Cost: none reported.** Go has shipped `var uts unix.Utsname; unix.Uname(&uts)` for over a decade with no per-field ceremony.

**5. Swift: the closest analogue in the room, and it chose route 2. VERIFIED.** Swift has value semantics and mandatory initialisation, and imports C structs with *"a synthesized default initializer (that sets all properties to zero), and an elementwise initializer"*; and *"C's fixed-size arrays are imported as Swift tuples"*, which the same document concedes is ergonomically poor because *"Swift tuples cannot be accessed through an index that only becomes known at runtime."*
Source: https://github.com/swiftlang/swift/blob/main/docs/HowSwiftImportsCAPIs.md

**What it cost Swift, measured by a third party on this exact struct.** Ole Begemann's write-up uses `uname`: `char[256]` becomes a 256-element tuple of `Int8`.
Source: https://oleb.net/blog/2017/12/swift-imports-fixed-size-c-arrays-as-tuples/
Community reporting on the same issue puts the cost at roughly 1,600 lines of generated Swift, ~8 s compile and ~750 KB of binary for one such tuple, plus an import size ceiling regression between Xcode 10 and 11.2 — I am citing this at **one remove**, from a search summary of the Apple Developer Forums thread and Michael Tsai's blog rather than from a page I fetched, so **mark those numbers unverified**: https://forums.developer.apple.com/thread/125614 · https://mjtsai.com/blog/2018/01/31/swift-imports-fixed-size-c-arrays-as-tuples/

**The load-bearing point: Swift paid its whole bill in the READ direction, and panel 162 already paid that one here** with `validated_bytes()`. Heroes is one decision away from the Swift shape with Swift's cost already avoided.

### The brief's second question: has a zero default at an FFI boundary ever been WRONG?

**Yes — and the shape of the answer narrows route 2 rather than refuting it. VERIFIED, documentation-level.**

- **`GetVersionEx`**: *"Before calling the GetVersionEx function, set the dwOSVersionInfoSize member of the structure as appropriate"*, and *"The function fails if you specify an invalid value for the dwOSVersionInfoSize member."* Microsoft's own example is `ZeroMemory(&osvi, sizeof(OSVERSIONINFO)); osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);` — **zero everything, then overwrite the one field the callee reads.**
  Source: https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getversionexa
- **`MEMORYSTATUSEX.dwLength`**: *"The size of the structure, in bytes. You must set this member before calling GlobalMemoryStatusEx."*
  Source: https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-memorystatusex
- **`sigaction.sa_mask`** — the one case that cuts the other way. POSIX: *"either sigemptyset() or sigfillset() must be called prior to any other use of the signal set, even if such use is read-only."* A zero-filled `sigset_t` is therefore **not** a sanctioned value, whatever glibc's layout happens to make it. (That `sigset_t` is internally an array is something I did **not** source — unverified.)
  Source: https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigemptyset.html
- **`getaddrinfo` hints — zero is the CORRECT default**, explicitly: *"All the other fields in the structure pointed to by hints must contain either 0 or a null pointer, as appropriate."*
  Source: https://man7.org/linux/man-pages/man3/getaddrinfo.3.html

**Not found: a filed defect report.** I searched the Go issue tracker via general web search for a cgo/FFI bug where a Go zero value was the wrong value for a field the C side reads first, and found only unrelated zero-*sized*-field issues (golang/go #26335, #20275, #25941). So the honest statement is: **the failure mode is real and documented, and it is guarded by documentation rather than by a bug trail I could reach.**

**And the pattern in that list is the finding.** Every field where zero is wrong is a **scalar** — `dwOSVersionInfoSize`, `dwLength`, `cbSize` — or a hand-built opaque set. The 256-byte character arrays in `utsname`, `stat`, `getrusage` are pure out-parameters. Rust draws the same line in its own type system: `mem::uninitialized` is UB for arrays of integers, while `mem::zeroed` remains legal and is warned only for types whose validity invariant excludes zero (*"references must be non-null"*). **A `[i8; 256]` holds zero legitimately.**

### Route 1 — a call typed by context. The precedent says: don't make it a call.

**6. Rust's array repeat expression `[expr; N]` is a LITERAL FORM, not a context-typed call. VERIFIED.** *"`[0; 128]; // array with 128 zeros"*. The length operand *"must either be an inferred const, or a constant expression of type `usize`"*, and if it is greater than 1 the repeat operand must be `Copy`, a const block, or a path to a constant.
Source: https://doc.rust-lang.org/reference/expressions/array-expr.html

This matters directly to the brief's finding that *"a call typed by context does not exist here"* and that `walk.hero` has six context-typed sites, **all of them literal forms**. Rust needed no context-typed call: the length lives **inside the form**, and only the element type flows in from context — which is precisely what a literal already does in Heroes. **Route 1 restated as a literal, `[0; 256]` against an expected `i8[256]`, is the precedent-backed version of panel 162's resolution and does not require inventing a new checking direction.** Whether it fits `storageless.hero` I cannot judge from this seat.

**7. A context-typed constructor ADDED after shipping: Swift SE-0287, implicit member chains, Swift 5.4, status Implemented.** Motivation: *"This error breaks the mental model that many users likely have for implicit member syntax, which boils down to a simple lexical omission of the type name in contexts where the type is clear."* The cost shows in its own rejected-alternatives section: restricting to homogeneously-typed chains was abandoned because *"the implementation complexity would have been greatly increased."*
Source: https://github.com/swiftlang/swift-evolution/blob/main/proposals/0287-implicit-member-chains.md

**8. Rust hit a long-array wall of its own and needed const generics to leave it. VERIFIED.** Before 1.47.0 (2020-10-08), *"arrays only have std trait implementations for lengths 0..=32"*; lifting that required the const-generics machinery, and the same post says *"We do not have a current estimated date for the stabilization of const generics."*
Source: https://blog.rust-lang.org/2020/10/08/Rust-1.47/
Read as: a long fixed array is where type systems reliably run out of road, and the fix is usually a **general mechanism**, not a special case.

Ada's `(others => 0)` and Zig's `**` array-repetition operator are **unverified** — I did not fetch either this session.

### Route 4 — a language that REFUSED

**I could not find one.** I searched for languages that forbid constructing a C out-parameter struct and require it from the library, via the Haskell FFI marshalling docs and general search on Swift/Go/Rust C interop. The nearest thing is **Haskell**, where you do not build a C struct as a value at all: you allocate raw bytes and `poke` fields. But the documentation does not state that `alloca`/`mallocBytes` memory is uninitialised; it only documents the zeroing variant, `callocBytes` — *"memory is filled with bytes of value zero"* — from which non-zeroing for the others is an **inference, not a quote**.
Source: https://hackage-content.haskell.org/package/base-4.22.0.0/docs/Foreign-Marshal-Alloc.html

So, in the brief's requested words: **I could not find a language that refused to let such a struct be built, and I searched Haskell's `Foreign.Marshal.Alloc`, Swift's C-import documentation, Go's `x/sys/unix`, and Rust's array and `MaybeUninit` documentation.** Route 4 stands or falls on Heroes' own grounds, without precedent behind it.

## argument

Rust shipped route 3 and withdrew it. `mem::uninitialized` is deprecated since 1.39.0 because an uninitialised value is *immediate* UB "on nearly all types, including integer types and arrays of integer types, and even if the result is unused". Rust's replacement is a type, not a rule. Route 2 is the line Rust kept: `mem::zeroed` is still there, warned only where zero is not a valid bit pattern; a `[i8; 256]` is. Go, Swift and C all take route 2 for exactly `uname`. The zero-is-wrong FFI cases are scalar length fields (`dwOSVersionInfoSize`, `dwLength`), never array fields — so scope the default to fixed arrays and leave scalars required.

## falsifiable prediction

**If route 2 is adopted and scoped to fixed-array fields only, with every scalar field of an `extern` record still required, then no C API in the POSIX or Win32 surface Heroes binds will be broken by the default** — because the fields whose pre-call value the callee reads are scalars, and those stay mandatory. This is falsified by exhibiting **one** C function that reads a fixed-array struct field before writing it and for which all-zero is the wrong value. `sigaction`'s `sa_mask` is the live candidate: POSIX forbids relying on an uninitialised set, and if `sigset_t` is laid out as an array on a platform Heroes binds, the prediction fails there and route 2 must carry an explicit exception for it.

## what I found misstated or under-stated in `00-shared.md`

1. **Under-stated, and it strengthens the brief.** The brief says `struct utsname` *"needs a per-platform source file whatever this sitting decides"* because of the array lengths. It is worse than that: Linux's `Utsname` has **six** fields and Darwin's has **five** — Linux carries `Domainname`, Darwin does not (both quoted above from `golang.org/x/sys`). The per-platform split is over the **field set**, not only over `65` versus `256`.
2. **Route 1's framing is narrower than the precedent requires.** The brief presents route 1 as *"a call typed by context"* and then correctly shows that no such thing exists in Heroes. Rust's `[0; 128]` shows the same capability delivered as a **literal form with the length inside it** — which is the category Heroes already supports. The sitting should weigh `[0; 256]`-as-a-literal separately from `repeat(0, 256)`-as-a-call; they are not the same proposal and the brief treats them as one.
3. **No misstatement found** in the Rust, Go, Swift or C claims — the brief makes none. I could not check the `design.md §4.9` and `spec § 5` citations: this seat had no shell.

## condition — what would change my reading

- **Against route 2**: one C function, in an API Heroes actually binds, that reads a **fixed-array** struct field before writing it and for which all-zero is wrong. `sigaction`'s `sa_mask` on a platform where `sigset_t` is an array would do it, and I did not manage to source that layout today.
- **For route 3**: a language that shipped an implicit uninitialised out-parameter, with no keyword at the binding and no runtime poisoning, and kept it. Zig is not that language — it has both. Rust was, for four years, and stopped.
- **Against route 4**: nothing needed; I already found no precedent for it, so it enters on Heroes' own argument.
