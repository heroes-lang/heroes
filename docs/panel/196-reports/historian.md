# Panel 196, historian (advisory, no veto)

Written 2026-10-06. Every URL below was read on **2026-10-06**; a claim I could
not confirm says `unverified`, and an inference from a verified rule says
`inference` and was not run. *Heroes of code* was not opened this session and no
claim here rests on it.

## verdict

**approve (advisory)** of the direction that puts the extent **in the binding,
checked at compile time**: S1 (`@md: u8[32]`) or S2 (a constant extent), with
S6's sentence as the stated default (`@x: T` promises exactly one `T`).
**object (advisory)** to S6 alone, to S3 keyed on byte pointees, and to S5 as the
only mechanism. Between S1 and S2 the closest analogue is D (value-semantics
fixed arrays lent to C by `ref`), which is S1, and it presupposes the critic's
S0: a fixed array that can live in a local, which D has and Heroes refuses today
(`fixed_outside_a_group`). Reasons and sources below.

## precedents

### A. The default "a pointer reaches one object" is what the traditions write down

- **GCC `access` attribute**: *"When no size-index argument is specified, the
  pointer argument must be either null or point to a space that is suitably
  aligned and large enough for at least one object of the referenced type"*; the
  size-index is *"the position of a function argument"*, never a constant.
  https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html - **verified**.
- **glibc** `misc/sys/cdefs.h`, under `__GNUC_PREREQ (10, 0)`: the access
  attribute designates *"size-index elements ... or at least one element when
  size-index is not provided"*.
  https://raw.githubusercontent.com/bminor/glibc/master/misc/sys/cdefs.h -
  **verified** (GitHub mirror; sourceware.org refused the fetch).
- **Cyclone**: `@numelts(e)`, *"e must be a static expression ... an upper bound
  on the number of objects that the pointer refers to"*, and *"If omitted on a
  @thin pointer, then @numelts(1) is inserted by default"*; *"a
  T\*@numelts(42) pointer can be passed anywhere a T\*@numelts(30) pointer is
  expected"*. https://www.cs.cornell.edu/Projects/cyclone/online-manual/main-screen003.html
  - **verified**. That the reverse (1 to 32) is refused is not stated on the page:
  **inference**.
- **.NET interop marshaller**: *"If the array size is not specified, only one
  element is marshalled"* (unmanaged to managed); the size may be a parameter
  (`SizeParamIndex`) **or a constant** (`SizeConst=128`); managed to unmanaged,
  *"the array size is determined by examination"*.
  https://learn.microsoft.com/en-us/dotnet/framework/interop/default-marshalling-for-arrays
  and https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.marshalasattribute.sizeconst
  (that page's version list starts at .NET Framework 1.1) - **verified**.
- **Microsoft SAL** separates `_Out_` (*"scalars, structures, pointers to
  structures"*) from `_Out_writes_(s)` (*"a pointer to an array of s elements"*),
  and capacity from written count in `_Out_writes_to_(s,c)`.
  https://learn.microsoft.com/en-us/cpp/code-quality/annotating-function-parameters-and-return-values
  - **verified**.

So S6's sentence is not a departure; it is the tradition. What differs is
whether the tradition then lets the binding state N.

### B. FFIs that put N in the binding's type, so a one-cell lend is refused

- **D**: the spec, *"In D, static arrays are passed by value, not by reference.
  Thus, the function prototype must be adjusted to match what C expects"*, with
  `extern (C) void foo(ref int[3] a);` for C's `void foo(int a[3])`.
  https://dlang.org/spec/interfaceToC.html - **verified**. druntime today:
  `int pipe(ref int[2]) @trusted;` beside `int gethostname(char*, size_t);` (not
  `@trusted`):
  https://raw.githubusercontent.com/dlang/dmd/master/druntime/src/core/sys/posix/unistd.d
  - **verified**. **How D got there was a fault**: Shin Fujishiro, 2010-05-17,
  issue 4199, *"in D2, static arrays are passed by value!"* and *"The pipe() must
  be declared like: int pipe(ref int[2])"*, the posix bindings segfaulting:
  https://forum.dlang.org/post/hsrglc$25l4$1@digitalmars.com - **verified**;
  the bugzilla resolution and its date (issues.dlang.org returned HTTP 522 three
  times) - **unverified**.
- **Zig, hand-written**: `pub extern "c" fn pipe2(fds: *[2]fd_t, flags: u32) c_int;`
  https://github.com/ziglang/zig/blob/f16f25047c511cc5f468da57292a968555c4b791/lib/std/c/openbsd.zig
  - **verified**; an older `pub fn pipe(fd: *[2]i32) usize` in issue #1804
  (2018-11-28) https://github.com/ziglang/zig/issues/1804 - **verified**; the
  trunk's `pipe` line today - **unverified** (the fetch truncated). `*[N]T`
  coerces to `[*]T` and to slices: https://ziglang.org/documentation/master/
  (0.17.0) - **verified**.
- **Zig, translated**: `[*c]T` *"coerce[s] to single and multi item pointers"*
  (zig.guide, 0.15.2) https://zig.guide/working-with-c/c-pointers - **verified**;
  the language reference's *"The only valid reason for using a C pointer is in
  auto-generated code from translating C code"*, with the reason that
  translation cannot tell one item from many, read as quoted on Ziggit
  (2023-12-16) https://ziggit.dev/t/using-zig-to-call-c-code-strings/2470 -
  **verified through a secondary only**. Zig names Heroes' ambiguity exactly.
- **Nim**: `proc pipe*(a: array[0..1, cint]): cint {.importc, header: "<unistd.h>".}`,
  `socketpair*(..., a4: var array[0..1, cint])`, `getgroups*(a1: cint, a2: ptr array[0..255, Gid])`.
  https://raw.githubusercontent.com/nim-lang/Nim/devel/lib/posix/posix.nim -
  **verified**. A Nim `SHA256_Final` binding to quote - **unverified** (none found).
- **Ada** (2012 RM B.3, paragraphs 68 and 70 as the page numbers them): an
  `out`/`in out` elementary T and an array of component T **both** reach C as
  `t*`. https://www.adaic.org/resources/add_content/standards/12rm/html/RM-B-3.html
  - **verified**; a constrained `array (1 .. 32)` binding refusing a scalar -
  **inference**.
- **Checked C**: `int f(_Array_ptr<int> p : count(5));`, *"Function taking
  pointer to 5 integers"*
  https://github.com/checkedc/checkedc-fork/wiki/Bounds-declarations - **verified**;
  bounds-safe interfaces give existing C functions checked types
  https://github.com/Microsoft/checkedc/wiki/Bounds-safe-interfaces - **verified**;
  the sentence *"The code is rejected by the compiler"* on that page I hold only
  as the fetch summarised it, its exact context **unverified**.
- **MIDL**: fixed arrays in signatures (`short asNumbers[10]`), and `size_is`
  (allocation) kept apart from `length_is` (elements transmitted).
  https://learn.microsoft.com/en-us/windows/win32/midl/length-is - **verified**.

### C. FFIs that accept a one-cell lend silently

- **Rust**: `openssl-sys` binds `SHA256_Final(md: *mut c_uchar, c: *mut SHA256_CTX) -> c_int`
  https://raw.githubusercontent.com/sfackler/rust-openssl/master/openssl-sys/src/handwritten/sha.rs
  - **verified**; the 32 lives only in the safe wrapper, `MaybeUninit::<[u8; 32]>`
  https://raw.githubusercontent.com/sfackler/rust-openssl/master/openssl/src/sha.rs
  - **verified**; `libc::pipe(fds: *mut c_int)` (0.2.190)
  https://docs.rs/libc/latest/libc/fn.pipe.html - **verified**; `bindgen`
  0.73.2's `array_pointers_in_arguments` turns `T arr[size]` into
  `*mut [T; size]`, off by default, so only where the header writes a bound
  https://docs.rs/bindgen/latest/bindgen/struct.Builder.html - **verified**.
  Whether Miri or a sanitizer catches the one-cell lend - **unverified**.
- **Swift**: one `UnsafeMutablePointer<Int>` parameter takes `&value` (one Int)
  and `&numbers` (an array); *"Swift's type safety guarantees that you can only
  pass a pointer to the type required"*: the type, never the count.
  https://developer.apple.com/documentation/swift/unsafemutablepointer.md -
  **verified**. Bounded `Span` overloads come only from `__counted_by`, with the
  experimental `SafeInteropWrappers` (Apple DTS, 2025-11-21)
  https://forums.swift.org/t/c-annotations-counted-by-and-noescape-not-producing-span-consuming-function-signatures/83333
  - **verified**.
- **Go cgo**: *"a function argument written as a fixed size array actually
  requires a pointer to the first element"*; `cgocheck` checks pinning and Go
  pointers, not sizes. https://pkg.go.dev/cmd/cgo - **verified**.
- **OCaml ctypes** 0.24.0: `allocate` (one) and `allocate_n ~count` both return
  `'a ptr`; arrays cannot be function arguments.
  https://ocaml.org/p/ctypes/latest/doc/Ctypes/index.html - **verified**.
- **Python ctypes** 3.14.8: for a `POINTER(c_int)` argument *"an object of the
  pointed type (c_int in this case) can be passed ... ctypes will apply the
  required byref() conversion ... automatically"*, and arrays are accepted too.
  https://docs.python.org/3/library/ctypes.html - **verified**.

This column is today's Heroes rule (the pointee's type is checked, the count is
not). Section E is what it cost in Rust; for Swift, Go, OCaml and Python I
searched for no CVE of this shape and claim none.

### D. What C and its headers say (S5)

- **C99 6.7.5.3**: with `static` the argument *"shall provide access to the first
  element of an array with at least as many elements as specified"*; Clang 21
  warns *"array argument is too small; contains 6 elements, callee requires at
  least 7"*, GCC 15 *"accessing 7 bytes in a region of size 6
  [-Wstringop-overflow=]"* (Jason A. Donenfeld's patch, 2025-11-23)
  https://lkml.iu.edu/hypermail/linux/kernel/2511.2/04258.html - **verified**.
  GCC 11's `-Warray-parameter` *"also enables the detection of the likely out of
  bounds accesses in calls to such functions with smaller arrays"*
  https://gcc.gnu.org/gcc-11/changes.html - **verified**.
- **Linux, 2025, on this very function**: Eric Biggers, 2025-11-22,
  `void sha256_final(struct sha256_ctx *ctx, u8 out[SHA256_DIGEST_SIZE]);`
  became `u8 out[at_least SHA256_DIGEST_SIZE]` (`at_least` is `static`), *"This
  causes clang to warn when a too-small array of known size is passed"*
  https://lkml.iu.edu/2511.2/04375.html - **verified**; merged by Linus Torvalds
  2025-12-02 (sha2, sha1, poly1305, md5, curve25519, chacha, chacha20poly1305)
  https://code.googlesource.com/linux/torvalds/linux/+/906003e15160642658358153e7598302d1b38166
  - **verified**; the pull request indexed as *"'at_least' array sizes for 6.19"*
  https://lkml.iu.edu/2511.3/04228.html - **verified** (title as indexed).
- **clang `-fbounds-safety`**: `__counted_by(N)`, N *"a simple reference to
  declaration, a constant ..., or an arithmetic expression"*; a failed check
  *"will deterministically trap"*; the upstream page says *"the feature is not
  available for users yet"* https://clang.llvm.org/docs/BoundsSafety.html -
  **verified**; Apple ships it in Xcode (WWDC25 session 311, *"You can now use
  it in Xcode"*) https://developer.apple.com/videos/play/wwdc2025/311/ -
  **verified**. Upstream clang's plain `counted_by` on **parameters** was left to
  *"future patches"* in PR #90786 (2024-05-17)
  https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20240513/579144.html
  - **verified for that date; its status today unverified**.
- **Apple Libc** `_bounds.h`: `_LIBC_COUNT(x)` is `__counted_by(x)` and
  `_LIBC_SIZE(x)` is `__sized_by(x)` under `__LIBC_STAGED_BOUNDS_SAFETY_ATTRIBUTES`,
  empty otherwise https://cs.iossec.tech/Libc/latest/source/include/_bounds.h -
  **verified** (third-party code browser).
- **glibc** `posix/unistd.h`: `pipe (int __pipedes[2])`, `gethostname (...)
  __fortified_attr_access (__write_only__, 1, 2)`
  https://raw.githubusercontent.com/bminor/glibc/master/posix/unistd.h - **verified**.
- **Windows SDK** 10.0.16299.0: `PathCombineW(_Out_writes_(MAX_PATH) LPWSTR pszDest, ...)`,
  `PathBuildRootA(_Out_writes_(4) LPSTR pszRoot, int iDrive)`: constant extents,
  no count parameter, which is SHA256_Final's shape annotated
  https://raw.githubusercontent.com/tpn/winsdk-10/master/Include/10.0.16299.0/um/Shlwapi.h
  - **verified** (mirror). Its hash API takes a count instead,
  `BCryptFinishHash(..., _Out_writes_bytes_all_(cbOutput) PUCHAR pbOutput, _In_ ULONG cbOutput, ...)`
  https://raw.githubusercontent.com/tpn/winsdk-10/master/Include/10.0.16299.0/shared/bcrypt.h
  and fails with STATUS_INVALID_PARAMETER where *"cbOutput is not the same size
  as the fixed size output of the hash"*
  https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptfinishhash
  - **verified**.
- **OpenSSL master** `sha.h`: `int SHA256_Final(unsigned char *md, SHA256_CTX *c);`,
  no bound, no bounds attribute
  https://raw.githubusercontent.com/openssl/openssl/master/include/openssl/sha.h
  - **verified**. `EVP_DigestFinal_ex`'s `s` is **written, never read**: *"the
  number of bytes of data written ... will be written to the integer at s"*
  https://raw.githubusercontent.com/openssl/openssl/master/doc/man3/EVP_DigestInit.pod
  - **verified**; the critic's `evp_outcount_512` lie, confirmed by C's own docs.

### E. Faults of this shape on the record

- **CVE-2026-41681 / GHSA-ghm9-cr32-g9qj, rust-openssl**: *"EVP_DigestFinal()
  always writes EVP_MD_CTX_size(ctx) to the out buffer. If out is smaller than
  that, MdCtxRef::digest_final() writes past its end, usually corrupting the
  stack. This is reachable from safe Rust."* Affected 0.10.39 to 0.10.77, fixed
  in 0.10.78 by a pre-call check, `if self.size() > len as usize { return Err(ErrorStack::get()); }`,
  tested with a 16-byte buffer for SHA-256.
  https://advisories.gitlab.com/cargo/openssl/CVE-2026-41681/ (published
  2026-04-22, CVSS 9.8), https://osv.dev/vulnerability/CVE-2026-41681 (says
  2026-04-24, CVSS v4 8.1),
  https://github.com/rust-openssl/rust-openssl/commit/826c3888b77add418b394770e2b2e3a72d9f92fe
  - **verified**; the sources disagree on date and score, both given. **This is
  defect 396's shape in the most-used Rust binding of the same library.** Its
  fix needed the extent at run time, which `EVP_MD_CTX_size` supplies;
  `SHA256_Final` offers no such query (**inference** from the header above).
- **CVE-2021-45707 / RUSTSEC-2021-0119, `nix::unistd::getgrouplist`**: called
  libc *"with a length parameter greater than the size of the buffer it
  provides, resulting in an out-of-bounds write"*, reported 2021-09-27.
  https://rustsec.org/advisories/RUSTSEC-2021-0119.html - **verified**. A count
  C writes back, taken as the next extent: the `counted_by s` shape.
- **RUSTSEC-2024-0020 / GHSA-w5w5-8vfh-xcjq, `whoami`**: a stack buffer
  overflow because of *"an incorrect definition of the passwd struct"* on
  illumos, Solaris and older BSDs, issued 2024-03-05.
  https://rustsec.org/advisories/RUSTSEC-2024-0020.html - **verified**. The
  binding's pointee shorter than what C writes.
- **CVE-2026-41898 / GHSA-hppc-g8h3-xhp3, rust-openssl**: a closure's returned
  length forwarded to OpenSSL *"without checking it against the &mut [u8]"*,
  fixed in 0.10.78. https://github.com/advisories/GHSA-hppc-g8h3-xhp3 -
  **verified**; an adjacent shape (a read past a buffer, not a write). The dates
  disagree (GHSA 2026-04-19, SentinelOne 2026-04-30,
  https://www.sentinelone.com/vulnerability-database/cve-2026-41898/).
- **D issue 4199 (2010)**: section B, **verified** as a forum post.
- **PostgreSQL, 2021-02-15**: `pg_cryptohash_final()` gained a result-size
  argument because *"a careless use ... would result in an out-of-bound write in
  memory as the size of the destination buffer to store the result digest is not
  known to the cryptohash internals"* (Michael Paquier).
  https://www.postgresql.org/message-id/E1lBSdy-00065L-OY%40gemulon.postgresql.org
  - **verified**. C's own answer, like BCrypt's: give the callee the count.

### F. Run-time guards and emitter-owned temporaries (S4, S7)

- **HotSpot JNI**: arrays are *"Copy[ied] and wrap[ped] ... for bounds
  checking"* and the release reports *"failed bounds check"*, in `jniCheck.cpp`
  https://cr.openjdk.org/~dsimms/6311046/rev5/src/share/vm/prims/jniCheck.cpp.cdiff.html
  - **verified**; that this file implements `-Xcheck:jni`, a diagnostic mode,
  rests on a search summary (bugs.openjdk.org redirected to a maintenance page):
  **unverified first-hand**. Copy-in, guard, copy-out: S7 and S4 together, as a
  debugging aid.
- **SWIG** (Doc 4.2): `%typemap(in, numinputs=0) int *out (int temp) { $1 = &temp; }`
  (14.5.1) and `%typemap(in) float value[4] (float temp[4])` with a size check
  (14.6.1): wrapper-owned C temporaries. https://www.swig.org/Doc4.2/Typemaps.html
  - **verified**; the two combined for an output array - **inference**.
- **I found no FFI that guards a lend at run time by default.** Looked at: Rust,
  Swift, Go, OCaml, Python, .NET, JNI. A question, not a premise.

## argument

The default "a pointer reaches one object" is what GCC, glibc, Cyclone, .NET and
SAL write down, so S6's sentence is right. But the FFIs that leave N unstated
(Rust, Swift, Go, OCaml, Python) accept the lie, and Rust's has a record:
rust-openssl wrote past a short `EVP_DigestFinal` buffer until April 2026
(CVE-2026-41681), and `nix` the out-count variant (CVE-2021-45707). The FFIs
that refuse put N in the binding's type: D (`ref int[2]`, after a 2010
segfault), Zig's std, Nim, Ada, Cyclone, Checked C. S5 alone fails: OpenSSL's
header says nothing, though Linux put `[static]` on `sha256_final` in 2025. No
precedent for S3 keyed on pointees, nor for S4 outside diagnostic modes. SAL
and MIDL keep capacity apart from written count.

## condition

My reading changes if somebody finds: an FFI that refuses by **pointee type**
(S3) and kept the rule; one that guards lends at run time **by default** at a
stated cost (S4); evidence that fixed-array parameters (S1) caused recurring ABI
faults beyond D2's 2010 by-value mistake, which would tip S1 toward S2; or the
D 4199 bugzilla record showing that `ref` was later reverted.
