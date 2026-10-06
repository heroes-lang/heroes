<!-- Written out by the coordinator from the seat's final message, 2026-09-24:
the historian has no write tool by its definition. One mechanical change: this
comment. The text below is the seat's, unchanged. -->

# Panel 178, historian seat (advisory, no veto)

This seat has no measuring instrument, so it claims no costs. Every precedent below has its URL. **verified** means I read the primary text in this session (or a named secondary that quotes it). **unverified** means I could not confirm it. *(inference)* marks where I am reasoning from what a source says rather than repeating it. No claim here rests on *Heroes of code*. Design.md sections are the ones the shared brief and panel 163 cite; I did not re-read design.md.

## One-row verdict per route

| route | verdict | precedent it rests on | falsifiable prediction | condition that would change it |
|---|---|---|---|---|
| **R1** `rest: zero` (§4.9 untouched) | **approve**, with an **objection to one clause**: *"and so is every byte between fields"* | Marking the omission at the site has lasted: Ada 83 `others =>`, where every component must still be covered once; Rust `..` struct update; the Linux kernel's `..Zeroable::zeroed()` (2023), which is R1 plus Z1 almost exactly; Rust RFC 3681's `..`. The padding clause is unsupported: C11 §6.2.6.1p6 and fn. 51, and CERT DCL39-C calls memset-then-assign noncompliant. | No seat will cite a C11 or C23 paragraph that keeps padding zero after a member store. The clause will rest on clang's behaviour, and GCC 15 changed that behaviour for `{0}` on unions in 2025. | Such a paragraph is found; or the clause is restated as "zeroed at construction, not promised after a store"; or padding becomes explicit fields (CERT's compliant solution). |
| **R0** `T.zero()` then `@` stores | **approve**, gated by Z1 and after 091 is repaired | This is what Rust std (`mem::zeroed()` then stores, with a `SAFETY:` comment), Swift (`sockaddr_un()` then stores) and Go (`var` then stores) actually do. | The first Heroes `sockaddr_un` binding in M-core-packages will be zero-then-store, as all five builders I read are (Rust std, Go Linux, Go BSD, Zig, swift-nio). None builds it in one literal. | A precedent that builds `sockaddr_un` from a runtime path in a single literal. |
| **R2** omitted fields zero, no mark | **object** | Silent omission (C99 designated initialisers, Go keyed literals, Odin, D) led to Go's `exhaustruct` linter (golangci-lint v1.46.0), Odin's `#complete` request (2020, unresolved), and Rust RFC 3681 choosing an explicit `..`. Counter-precedent: Zig translate-c has done exactly R2's scope since 0.12 (PR #17196, 2023). | If R2 lands, a request to mark a group record "every field required" arrives before M-core-packages closes. | Evidence that Zig's translate-c zero defaults produced no forgotten-field defects. I found none either way. |
| **A** `[x; N]` | **approve** | Rust `[operand; length]`, Ada `(others => x)`, Zig `**` and `@splat`, Extended Pascal `otherwise`, Cyclone `{for i < n : e}`. Swift 6.2 has `init(repeating:)` and deferred a `[5 of 99]` literal. Where trait machinery was used instead of a literal, it stalled at 32 elements (Rust #61415, open since 2019), which broke bindgen (#2803). | No language I searched shipped a repetition literal and then withdrew it. | One that did. |
| **Z1** claimed on the record | **approve** | bytemuck and pin-init `Zeroable`: an `unsafe` trait that the implementer asserts. The kernel derives `MaybeZeroable` only when every field is Zeroable. | Any structural rule Heroes could compute (no pointers, only integers and arrays) accepts Darwin's `pthread_mutex_t`, so Z cannot be derived and must be claimed. | A system that states zero-validity per platform (I found none), or a rule that checks the claim against the header's `*_INITIALIZER` macros. |
| **Z2** assumed | **object** | Swift's structural zero `init()` gives Darwin a mutex that "won't be a valid lock" (Mike Ash, 2017). D's druntime shipped an all-zero `PTHREAD_MUTEX_INITIALIZER` on OS X until 2015-05-25 (issue 14617). bindgen's zeroing `Default` is UB for enums (#2974, 2024). Nim's `memset` broke Qt's `QList` (#5140, 2016). | Z2 reproduces measurement 7 on the first Darwin program that zero-builds a pthread record. | None found. |
| **T** `to_fixed()`, fallible | **approve**, with one gap | Rust std (`>=` then error; also refuses interior NUL), Zig (`len + 1 >` gives `NameTooLong`, then memset and memcpy, which is T's own semantics), Go BSD (`>=` gives `EINVAL`), swift-nio (`<= 103`), Oberon-07 §9.1. The truncating copies (strncpy, strlcpy) have been removed from the kernel. | A `str` with an interior zero byte passes `to_fixed()`, and C then reads a shorter path. One run settles it. | T refuses an interior zero, or a `str` cannot hold one (a question; I did not check the spec). |
| **L** repair 091, then loop | **approve** (spec §5 line 121 already admits the write) | Oberon-07 has no aggregate expressions at all (§8.1). Go's `sockaddr_un` builders copy byte by byte in a loop. | L becomes T's lowering (or a memcpy), not T's rival. | None. |
| **S** status quo | **approve as the floor, object as the answer** | Go's `x/sys/unix` is generated per GOOS/GOARCH (`cgo -godefs`) and matches the census exactly. Hand-written bindings drift: Zig `std.c.pthread_mutex_t` was 32 bytes against a real 40 (#21229, 2024); D 14617. | S stays the per-platform layout route whatever else lands. | The header-length route gets a price. |
| **D** zero default everywhere | **object** | D chose non-zero defaults on purpose, "to expose bugs" (D FAQ). Nim's `requiresInit` retrofit leaked for years (#13808 in 2020, #20253 in 2022). Go and Odin users asked for the completeness check that Heroes already has. | If D lands, the thesis harness finds a forgotten-field program that exits 0. | Evidence from a zero-default language that forgotten-field bugs are rare. |

## verdict

**approve (advisory)** of the composition R1 + Z1 + T, plus A. There is one standing objection to R1's padding clause, and I object to Z2, R2 and D.

## precedents

### 1. Building a C struct without naming every field

- **Swift, verified.** SE-0189 (Jordan Rose, implemented in Swift 4.1) says: *"Most C structs also have a no-argument initializer that fills the struct with zeros unless one of the members is marked `_Nonnull`"* and *"all C structs are imported with a memberwise initializer anyway."* [SE-0189](https://github.com/apple/swift-evolution/blob/master/proposals/0189-restrict-cross-module-struct-initializers.md). The zero is written at the site as `T()`, and the only refusal is structural (`_Nonnull`).
  - C fixed arrays import as tuples, and *"Swift tuples cannot be accessed through an index that only becomes known at runtime"* ([HowSwiftImportsCAPIs.md](https://github.com/apple/swift/blob/main/docs/HowSwiftImportsCAPIs.md)).
  - SE-0453 `InlineArray` was accepted 2025-02-05 ([announcement](https://forums.swift.org/t/accepted-with-modifications-se-0453-inlinearray-formerly-vector-a-fixed-size-array/77678)) and implemented in Swift 6.2, with `init(repeating:)` ([SE-0453](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0453-vector.md)).
  - SE-0483 (`[N of T]`, Swift 6.2) defers a `[5 of 99]` value literal as a future direction ([SE-0483](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0483-inline-array-sugar.md)).
  - Importing C arrays as `InlineArray` was only pitched on 2026-07-29 (Becca Royal-Gordon, [pitch](https://forums.swift.org/t/pitch-modernize-imported-c-arrays/88633)).
  - Bears on R0, Z2, A and the header-length route.
- **Zig, verified.** `std.mem.zeroes` ([source](https://raw.githubusercontent.com/ziglang/zig/master/lib/std/mem.zig)):
  - The doc comment says: *"If you are performing code review and see this function used, examine closely - it may be a code smell."*
  - It refuses, with these messages: *"Only nullable and allowzero pointers can be set to zero."*, *"Can't set a sentinel slice to zero…"*, and *"Can't set a <T> to zero."* for non-extern unions.
  - An extern struct is simply `@memset` to zero without recursing into its fields. *(Inference, unrun: an extern struct holding a non-allowzero pointer is zeroed without a compile error.)*
  - `zeroInit` *"Initializes all fields of the struct with their default value, or zero values if no default value is present"*, which is R1 as a function.
  - translate-c zero defaults: issue [#8165](https://github.com/ziglang/zig/issues/8165) (2021-03-06) led to [PR #17196](https://github.com/ziglang/zig/pull/17196) (merged 2023-09-19, milestone 0.12.0). The PR says: *"Omitted fields are implicitly initialized to zero. Some C APIs are designed with this in mind."* That is R2 for C-derived structs.
  - `**` is comptime-only, and `@splat(0)` is typed by its result ([0.16.0 docs](https://ziglang.org/documentation/0.16.0/), [master docs](https://ziglang.org/documentation/master/)).
  - A "Faulty Default Field Values" section exists in the table of contents. Its text as I have it came from a search snippet, so the verbatim wording is **unverified**.
- **Rust, verified.**
  - Struct update `..` must come last ([Book](https://doc.rust-lang.org/book/ch05-01-defining-structs.html)).
  - RFC 3681 (2024-08-22): *"By writing `Foo { .. }`, there is explicit indication that default values are being used; this enhances local reasoning further"* ([RFC](https://rust-lang.github.io/rfcs/3681-default-field-values.html)). The [tracking issue #132162](https://github.com/rust-lang/rust/issues/132162) was open, nightly-only, when I read it.
  - `mem::zeroed`: *"the padding byte in `(u8, u16)` is not necessarily zeroed"* and *"There is no guarantee that an all-zero byte-pattern represents a valid value"* ([doc](https://doc.rust-lang.org/std/mem/fn.zeroed.html)).
  - bytemuck `Zeroable` is an `unsafe` trait: *"Your type must be allowed to be an 'all zeroes' bit pattern (eg: no NonNull<T>)"*. Without a feature flag, arrays implement it only for lengths 0-32, 48, 64, 96, 128, 256, 512, 1024, 2048 and 4096 ([source](https://raw.githubusercontent.com/Lokathor/bytemuck/main/src/zeroable.rs)). Its derive requires every field to be Zeroable ([derive](https://github.com/Lokathor/bytemuck/blob/main/derive/src/lib.rs)).
  - Kernel pin-init, Benno Lossin, 2023-07-19: `..Zeroable::zeroed()`, *"these fields will be initialized with 0x00 set to every byte"*, and *"Only types that implement the `Zeroable` trait can utilize this"* ([LKML](https://lkml.iu.edu/hypermail/linux/kernel/2307.2/04643.html)). The current source spells it `..Zeroable::init_zeroed()`, and omitting a field without it is a compile error ([pin-init](https://raw.githubusercontent.com/Rust-for-Linux/pin-init/main/src/lib.rs)).
  - Kernel commit 4846300ba8f9 derives `MaybeZeroable` for all bindgen output: Zeroable only when all fields are ([commit](https://github.com/torvalds/linux/commit/4846300ba8f9)). The v3 posting date of 2025-08-14 comes from the lkml.org index only.
  - bindgen's zeroing `Default` is UB for enums with no 0 value ([#2974](https://github.com/rust-lang/rust-bindgen/issues/2974), 2024-11-11, open).
  - `[T; N]: Default` exists only up to 32 ([#61415](https://github.com/rust-lang/rust/issues/61415), 2019-05-31, open). This broke bindgen with *"the trait bound `[u8; 40]: Default` is not satisfied"* ([#2803](https://github.com/rust-lang/rust-bindgen/issues/2803), 2024-04-09).
  - The repeat expression requires `Copy` or a const ([Reference](https://doc.rust-lang.org/reference/expressions/array-expr.html)).
- **Go, verified.**
  - Spec: *"Omitted fields get the zero value for that field"* ([spec](https://go.dev/ref/spec), quoted via [boldlygo](https://boldlygo.tech/archive/2023-09-26-struct-literals/)). Also *"A literal may omit the element list; such a literal evaluates to the zero value for its type."*
  - `sync.Mutex`: *"The zero value for a Mutex is an unlocked mutex"* ([pkg.go.dev](https://pkg.go.dev/sync#Mutex)).
  - "Make the zero value useful" is from Rob Pike, Gopherfest SV 2015 ([proverbs](https://go-proverbs.github.io/)).
  - `exhaustruct`, *"Checks if all structure fields are initialized"*, arrived in golangci-lint v1.46.0 ([docs](https://golangci-lint.run/docs/linters/configuration/)).
  - That `var x C.struct_foo` is zero follows from the spec *(inference)*. I found no cgo sentence saying it.
- **C23, verified.** N2900 (JeanHeyd Meneide, 2022-01-01) ([paper](https://thephd.dev/_vendor/future_cxx/papers/C%20-%20Consistent,%20Warningless,%20and%20Intuitive%20Initialization%20with%20%7B%7D.html)). N3096 §6.7.10p11: *"…or any object is initialized with an empty initializer, then it is subject to default initialization… if it is an aggregate, every member is initialized (recursively)… and any padding is initialized to zero bits"* ([N3096](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf)). The zero padding is promised only for `{}` with nothing named.
- **Ada, verified.**
  - Ada 83 LRM §4.3: *"Each component of the value defined by an aggregate must be represented once and only once in the aggregate"*. `others` must come last and cover components of one type ([LRM](https://people.cs.kuleuven.be/~dirk.craeynest/ada-belgium/docs/rm83/lrm-04-03.html)).
  - Ada 2012 §4.3.1: `<>` initialises *"by default as for a stand-alone object"* ([RM](https://www.adaic.org/resources/add_content/standards/12rm/html/RM-4-3-1.html)). *(Inference: for a scalar with no default, that is not zero.)*
  - Ada 2012 §4.3.3: an array `others` needs an applicable index constraint ([RM](https://www.adaic.org/resources/add_content/standards/12rm/html/RM-4-3-3.html)).
- **D, verified.**
  - Struct literal: *"Any field not covered… is default initialized"*, and `@disable this();` refuses default construction ([spec](https://dlang.org/spec/struct.html)).
  - `char.init` is `'\xFF'` and `float.init` is `float.nan` ([types](https://dlang.org/spec/type.html)).
  - The FAQ: *"The default initializer value is not meant to be a useful value, it is meant to expose bugs"* ([FAQ](https://dlang.org/articles/faq.html)).
- **Odin, verified.** *"Variables are initialized to zero by default"*. `---` means uninitialised, and named-field literals zero whatever they omit ([overview](https://odin-lang.org/docs/overview/)). The `#complete` request (2020-10-07) got the reply *"the zero value is meant to be useful in Odin"* and was left unresolved ([discussion](https://github.com/odin-lang/Odin/discussions/962)).
- **Nim, verified.**
  - [PR #13808](https://github.com/nim-lang/Nim/pull/13808) (merged 2020-04-01) *"plugs a great number of holes"* for `not nil` and `requiresInit`.
  - [#20253](https://github.com/nim-lang/Nim/issues/20253) (2022-08-21) found `new` and `setLen` still bypassing it. The reporter's words: *"requiresInit is pretty useless"*.
  - [#5140](https://github.com/nim-lang/Nim/issues/5140) (2016-12-22): a `memset` on an importcpp result broke `QList`.
  - The posix bindings are per platform ([linux_amd64](https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/posix/posix_linux_amd64.nim): `array[65, char]`; [other](https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/posix/posix_other.nim): `array[0..255, char]`). Which file Linux arm64 uses, and what follows from that, is **unverified**.

### 2. Padding (verified from the [N1570 PDF](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf))

**The coordinator remembered correctly.**

- §6.2.6.1p6: *"When a value is stored in an object of structure or union type, including in a member object, the bytes of the object representation that correspond to any padding bytes take unspecified values."*
- Footnote 51: *"Thus, for example, structure assignment need not copy any padding bits."* So today's `t = (T){…}` is itself a store.
- Footnote 52: `x == y` *"does not imply that memcmp(&x, &y, sizeof (T)) == 0"*.
- §6.7.9p19: *"all **subobjects** that are not initialized explicitly shall be initialized implicitly the same as objects that have static storage duration."* Padding is not a subobject.
- §6.7.9p10 gives zero padding only to objects with static storage duration. §6.7.9p21 initialises *"the remainder of the aggregate"*.
- Jens Gustedt (2012-10-24) reads it as *"some padding in a structure is guaranteed to be zero-bit initialized, some isn't"* ([blog](https://gustedt.wordpress.com/2012/10/24/c11-defects-initialization-of-padding/)).
- CERT DCL39-C labels memset-then-assign **noncompliant**, citing CVE-2010-4083 ([CERT](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/declarations-and-initialization-dcl/dcl39-c)).
- Memfault (2022-03-01) found clang 13 at `-O1` leaving padding set under `{0}`. The brief did not measure `-O1` ([article](https://interrupt.memfault.com/blog/c-struct-padding-initialization)).
- GCC 15: *"`{0}` initializer… for unions no longer guarantees clearing of the whole union"* ([changes](https://gcc.gnu.org/gcc-15/changes.html)). GCC 15.1 was released in April 2025 ([announce](https://gcc.gnu.org/pipermail/gcc-announce/2025/000185.html); the date comes from the search listing). Linux perf adopted `-fzero-init-padding-bits=all` on 2025-03-20 ([LKML](https://lkml.rescloud.iu.edu/2503.2/06697.html)).
- I read §6.2.6.1 only in N1570, not in C23.

### 3. Who declares that all-zero is valid

- **Assumed:** Go; Zig translate-c; bindgen; Swift, which refuses only structurally.
- **Asserted once on the type:** bytemuck and pin-init `Zeroable`, plus Rust std's use-site comment *"SAFETY: All zeros is a valid representation for `sockaddr_un`"* ([addr.rs](https://github.com/rust-lang/rust/blob/master/library/std/src/os/unix/net/addr.rs)).
- **Refused:** Zig `zeroes` on non-allowzero pointers; Nim `requiresInit`; D `@disable this()`.
- **Per platform: I found no system that states zero-validity per platform.** What exists per platform is initializer constants:
  - Apple defines `_PTHREAD_MUTEX_SIG_init 0x32AAABA7`, and `PTHREAD_MUTEX_INITIALIZER {_PTHREAD_MUTEX_SIG_init, {0}}`, and the same shape for cond, rwlock, once, recursive and errorcheck ([impl.h](https://raw.githubusercontent.com/apple-oss-distributions/libpthread/main/include/pthread/pthread_impl.h), [pthread.h](https://github.com/apple-oss-distributions/libpthread/blob/main/include/pthread/pthread.h)).
  - D issue 14617 (Martin Nowak, 2015-05-23; fixed in druntime PR #1285 on 2015-05-25) ([thread](https://forum.dlang.org/thread/bug-14617-3@https.issues.dlang.org/)).
  - Mike Ash: *"that value won't be a valid lock"* ([2017](https://www.mikeash.com/pyblog/friday-qa-2017-10-27-locks-thread-safety-and-swift-2017-edition.html)).
  - POSIX does not require zero ([pthread_mutex_init](https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_mutex_init.html)).
- **An observation, from those six Apple macros only:** the one non-zero value in each is the scalar `__sig`, and the array is `{0}`. So panel 163's historian prediction 4 (route 2 scoped to fixed arrays) is **not** falsified by measurement 7. An arrays-only zero is a route the brief's list does not have.

### 4. A string into a fixed byte array

- **strncpy.** The C99 Rationale §7.21.2.4: *"strncpy was initially introduced into the C library to deal with fixed-length name fields in structures such as directory entries… strncpy is not by origin a 'bounded strcpy'"* ([Rationale V5.10](https://www.open-std.org/jtc1/sc22/wg14/www/C99RationaleV5.10.pdf)).
- **strlcpy.**
  - Introduced in OpenBSD 2.4. It returns *"the total length of the string they tried to create"*, and *"Neither function zero-fills unused bytes"* ([USENIX 1999](https://www.usenix.org/legacy/publications/library/proceedings/usenix99/full_papers/millert/millert_html/index.html)). A search summary said the opposite; the paper settles it.
  - Drepper, 2000-08-08: *"horribly inefficient BSD crap. Using these function only leads to other errors"* ([libc-alpha](https://sourceware.org/legacy-ml/libc-alpha/2000-08/msg00053.html)). LWN's 2014-09-17 account is [here](https://lwn.net/Articles/612244/).
  - Added in glibc 2.38 (2023) ([Phoronix](https://www.phoronix.com/news/GNU-C-Library-glibc-2.38)). POSIX: *"First released in Issue 8"* ([strlcpy](https://pubs.opengroup.org/onlinepubs/9799919799/functions/strlcpy.html)).
- **The kernel.**
  - strlcpy removed: *"The kernel is now free of the strlcpy() API!"* (pull request, 2024-01-19, [archive](https://www.mail-archive.com/linux-bcachefs@vger.kernel.org/msg01005.html)).
  - strncpy's last use removed: Kees Cook's pull of 2026-06-15 ([mirror](https://ratatoskr.run/lkml/2026/06/17135801/t)). deprecated.rst says strncpy is *completely removed*, and `strscpy` returns a negative errno on truncation ([doc](https://www.kernel.org/doc/html/latest/process/deprecated.html)).
- **Rust.**
  - std's `sockaddr_un`: *"paths must not contain interior null bytes"*, and `>=` gives *"path must be shorter than SUN_LEN"*.
  - `CString::new` errors on an internal 0 byte ([doc](https://doc.rust-lang.org/std/ffi/struct.CString.html)).
  - `copy_from_slice` panics on a length mismatch ([doc](https://doc.rust-lang.org/std/primitive.slice.html#method.copy_from_slice)).
- **Go.** Linux: `n > len` gives `EINVAL`, and a full-length path is allowed only for abstract addresses ([syscall_linux.go](https://github.com/golang/go/blob/master/src/syscall/syscall_linux.go)). BSD: `n >= len || n == 0` gives `EINVAL` ([syscall_bsd.go](https://github.com/golang/go/blob/master/src/syscall/syscall_bsd.go)). Both copy in a byte loop.
- **Zig 0.14** `initUnix`: *"Add 1 to ensure a terminating 0 is present…"*, returns `error.NameTooLong`, then memset and memcpy ([net.zig](https://github.com/ziglang/zig/blob/0.14.0/lib/std/net.zig)).
- **swift-nio**: `<= 103` on every platform, with no comment explaining it ([source](https://github.com/apple/swift-nio/blob/main/Sources/NIOCore/SocketAddresses.swift)).
- **Oberon-07** §9.1: *"Strings can be assigned to any array of characters, provided the number of characters in the string is less than that of the array. (A null character is appended)"*. §8.1 shows there are no aggregate operands ([report, rev. 3.5.2016](https://people.inf.ethz.ch/wirth/Oberon/Oberon07.Report.pdf)).
- **Also read:**
  - Kernighan, 1981-04-02: *"Because arr10 and arr20 have different types, it is not possible to write a single procedure that will sort them both"* ([essay](https://www.cs.virginia.edu/~evans/cs655/readings/bwk-on-pascal.html)).
  - Extended Pascal's `otherwise` in array values ([GPC](https://www.gnu-pascal.de/gpc/otherwise.html)).
  - Cyclone's `new {for i < 100 : 2*i}`, with definite initialisation only for data that contains pointers ([manual](https://www.cs.cornell.edu/projects/cyclone/online-manual/main-screen002.html)).
- **Per-platform generated types.** Go's generated types match the census: Darwin `Utsname` is `[256]byte` ×5 and `Path [104]int8`; Linux is `[65]byte` ×6 and `[108]int8` ([darwin](https://github.com/golang/sys/blob/master/unix/ztypes_darwin_arm64.go), [linux](https://github.com/golang/sys/blob/master/unix/ztypes_linux.go)). Zig's hand-written mutex drifted in size ([#21229](https://github.com/ziglang/zig/issues/21229)).

**Searched for and not found:** any per-platform zero-validity marker; any documented cgo bug with a zero Darwin mutex; any defect report against Zig's translate-c zero defaults beyond a union compiler crash; any language that withdrew a repeat-array literal.

## argument (≤120 words)

Where omission is marked at the construction (Ada's `others`, Rust's `..`, the kernel's `..Zeroable::zeroed()`), it has lasted. Where omission is silent (Go, Odin, Nim), users later built linters, asked for `#complete`, or retrofitted `requiresInit`, and that retrofit leaked for years. Zero-validity cannot be read off a type's structure: Swift's rule and D's druntime both gave Darwin an all-zero mutex that does not lock. Copies that fail when a string does not fit (Rust, Zig, Go) have replaced copies that truncate; the kernel has now removed strncpy and strlcpy. The one clause with no precedent is R1's promise about padding. C11 §6.2.6.1p6 makes padding unspecified after any store, and CERT calls memset-then-assign noncompliant.

## condition

My reading changes if any of these turns up:

1. A C11 or C23 paragraph that guarantees padding stays zero after a member store. That lifts the objection to R1's clause.
2. A system that states zero-validity per platform. That makes Z1's "on every platform" checkable rather than taken on the author's word.
3. Evidence that Zig's translate-c zero defaults (0.12, 2023) have gone without forgotten-field defects. That softens my objection to R2.
4. A language that shipped a repetition literal and withdrew it. That turns A to object.

One question for the ffi seat, not a premise: can a `partial` record that declares no fields compile over `pthread_mutex_t` on both Darwin and Linux? If it can, Z1's cross-platform claim is where measurement 7 would bite.

Brief files read: /Users/joseph/Temp/heroes-lane-panel-178/docs/panel/178-briefs/00-shared.md, /Users/joseph/Temp/heroes-lane-panel-178/docs/panel/178-briefs/historian.md, /Users/joseph/Temp/heroes-lane-panel-178/docs/panel/163-the-wall-was-not-there-and-the-brief-said-it-was.md
