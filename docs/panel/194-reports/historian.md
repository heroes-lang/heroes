# Panel 194, historian seat (advisory, no veto)

Written 2026-10-06. Every claim carries its URL and **every source was read on
2026-10-06** unless the line says otherwise. **verified** means I read the
primary text this session, or a named secondary that quotes it; **unverified**
means I could not confirm it; *(inference)* marks reasoning from a source rather
than a repetition of it. No claim rests on *Heroes of code*. This seat measures
nothing in the tree and claims no costs; numbers about Heroes are the brief's
(`00-shared.md`) or lane b12-ffi13's (`docs/panel/194-evidence/092-routes.md`),
cited as theirs.

Two naming collisions: 178's **A** is `[x; N]` and its **D** is a zero default
for every type; defect 092's lane also has routes named A to E. I write the
lane's as **092-A** to **092-E**.

## verdict

**approve (advisory)**:
- R1, defined for unions and with no padding promise;
- Z2 read as bytes only;
- Z3;
- T and A waiting, with A's form named `[x; _]` if it returns;
- C's `char` said to be `i8`;
- for defect 092, 092-A with the unit rule of route C, 092-B as its remedy, and
  092-D as the direction most precedent takes.

**object**: R2, D, Z1, a separate R0, 092-C′ and 092-E.

**Changes from my 178 report**, each named:
- Z1 moves from approve to object: 178's measurements decided it, and the
  precedent below agrees.
- R0 is absorbed into `T(rest: zero)`.
- A moves from approve to wait. Principle 0's count reads 0 (the brief), and
  Rust now has `[x; _]`.

## precedents

### Q1. Building a C struct with long arrays without writing every element

**C11, verified, re-read this session** (N1570, committee draft 2011-04-12, page
images of https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf):
- §6.7.9p10 gives padding *"initialized to zero bits"* only to an object
  *"that has static or thread storage duration"* that is *"not initialized
  explicitly"*.
- §6.7.9p19: *"all subobjects that are not initialized explicitly shall be
  initialized implicitly the same as objects that have static storage
  duration."*
- §6.7.9p21: *"the remainder of the aggregate shall be initialized implicitly
  the same as objects that have static storage duration."*
- Footnote 149: *"After a union member is initialized, the next object is not
  the next member of the union"*.
- §6.7.9p9: *"Unnamed members of structure objects have indeterminate value
  even after initialization."*

**C23, verified** (N3096, working draft 2023-04-01, page images of
https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf):
- §6.7.10p11 brings in *default initialization* for an object initialised with
  `{}`. Its aggregate bullet reads *"every member is initialized (recursively)
  according to these rules, and any padding is initialized to zero bits"*. Its
  union bullet reads *"the first named member is initialized (recursively)
  according to these rules, and any padding is initialized to zero bits."*
- §6.7.10p20: *"all subobjects that are not initialized explicitly are subject
  to default initialization."*
- §6.7.10p22: *"the remainder of the aggregate is subject to default
  initialization."*
- §6.7.10p10: *"Unnamed members of structure objects have indeterminate
  representation even after initialization."*
- *(Inference)* A long array left out of a designated initialiser is zero in
  C11 and C23 alike, which is R1's semantics, with no new emission. Neither
  standard says the outer object's padding is zero once one member is named;
  only C23's `{}` with nothing inside reaches p11 for the whole object.

**§6.2.6.1 is unchanged in C23, verified.** N3096 §6.2.6.1p6:
- *"When a value is stored in an object of structure or union type, including
  in a member object, the bytes of the object representation that correspond
  to any padding bytes take unspecified values."*
- Footnote 56: *"structure assignment need not copy any padding bits."*
- p7: *"When a value is stored in a member of an object of union type, the bytes
  of the object representation that do not correspond to that member but do
  correspond to other members take unspecified values."*
- §6.2.6.2p3: *"For any integer type, the object representation where all the
  bits are zero shall be a representation of the value zero in that type."*

**Clang 20 promises more than C, verified, and this is new since 178.** Yabin
Cui's patch, reapplied 2024-09-25 as PR #110051
(https://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20240923/624596.html),
adds a LanguageExtensions section, *"Union and aggregate initialization in C"*:
- *"Clang supports initializer `= {}` mentioned above in all C standards."*
- *"When unions are initialized from initializer `= {}`, bytes outside of the
  first members of unions are also initialized to zero."*
- *"When unions, structures and arrays are initialized from initializer `= {
  initializer-list }`, all members not explicitly initialized in the
  initializer list are empty-initialized recursively. In addition, all padding
  bits are initialized to zero."*
- It applies to C only.

The heading is in clang 20.1.0's documentation
(https://releases.llvm.org/20.1.0/tools/clang/docs/LanguageExtensions.html) and
absent from 19.1.0's
(https://releases.llvm.org/19.1.0/tools/clang/docs/LanguageExtensions.html).
Which LLVM Apple clang 21.0.0 is built from is **unverified**.

*(Inference from §6.2.6.1p6.)* The guarantee covers initialisation, not a later
copy or store. So an emission `t2 = (struct utsname){...}` is an assignment
whose padding C does not promise, even where the compound literal's own padding
is zero. That fits 178's MemorySanitizer finding (padding lost by value on
x86-64). Whether Heroes' emission is a declaration-initialiser or an
assignment is the compiler-engineer's to read. I did not read it.

**GCC 15 promises less, verified** (https://gcc.gnu.org/gcc-15/changes.html):
*"{0} initializer in C or C++ for unions no longer guarantees clearing of the
whole union (except for static storage duration initialization), it just
initializes the first union member to zero."* It points to `{}` or
`-fzero-init-padding-bits=unions`. Kees Cook's kbuild patch of 2025-01-20,
*"kbuild: Use -fzero-init-padding-bits=all"*, has the kernel buy the old
behaviour back with a flag (https://lkml.iu.edu/hypermail/linux/kernel/2501.2/04577.html).

**Rust, verified.**
- `MaybeUninit::zeroed` (https://doc.rust-lang.org/std/mem/union.MaybeUninit.html)
  says: *"if `T` has padding bytes, those bytes are _not_ preserved when the
  `MaybeUninit<T>` value is returned from this function, so those bytes are not
  guaranteed to be zeroed"*. It also says: *"It depends on `T` whether that
  already makes for proper initialization."*
- `[T; N]: Default` still stops at 32. Issue #61415 (2019-05-31) is **open**
  (https://github.com/rust-lang/rust/issues/61415).
- bindgen #2803, *"Failed to generate `Default` impl for arrays larger than 32
  entries"* (2024-04-09), is **open**
  (https://github.com/rust-lang/rust-bindgen/issues/2803).
- bindgen #2974 (2024-11-11) is **open**: the derived `Default` is
  `ptr::write_bytes(s.as_mut_ptr(), 0, 1); s.assume_init()`, which is UB for an
  enum with no zero variant (https://github.com/rust-lang/rust-bindgen/issues/2974).
- RFC 3681's `..` is still **unstable**, behind `#![feature(default_field_values)]`
  (https://github.com/rust-lang/rust/issues/132162). Under that feature,
  E0063, the missing-fields error, stays an error and adds *"help: all
  remaining fields have default values, you can use those values with `..`"*
  (https://rust.googlesource.com/rust/+/HEAD/tests/ui/structs/default-field-values/non-exhaustive-ctor.enabled.stderr).
  That is the precedent for the critic's unlisted route, a missing-fields
  message that offers the explicit tail.
- Kernel pin-init (https://raw.githubusercontent.com/Rust-for-Linux/pin-init/main/src/lib.rs):
  `..Zeroable::init_zeroed()` at the end of an initialiser zeroes every field
  it does not name. The type must implement `Zeroable`, and leaving a field out
  without it is a compile error. This is R1's shape exactly: the completeness
  check stays, and an explicit tail asks for zeros.
- **New since 178: Rust 1.89.0 (2025-08-07) stabilised `_` as an array
  length.** Its example is `[false; _]` in a body, and `_` is refused *"when in
  a signature"* (https://blog.rust-lang.org/2025/08/07/Rust-1.89.0/). The
  Reference now admits *"an inferred const"* as the repeat length
  (https://doc.rust-lang.org/reference/expressions/array-expr.html).

**Zig, verified.** `lib/std/mem.zig`
(https://raw.githubusercontent.com/ziglang/zig/master/lib/std/mem.zig):
- `zeroes`' doc comment says: *"Generally, Zig users are encouraged to
  explicitly initialize all fields of a struct explicitly rather than using
  this function. However, it is recognized that there are sometimes use cases
  for initializing all fields to a "zero" value. For example, when interfacing
  with a C API where this practice is more common and relied upon. If you are
  performing code review and see this function used, examine closely - it may
  be a code smell."*
- `zeroes` refuses non-allowzero pointers, sentinel slices, and *"Can't set a
  <T> to zero."* for a non-extern union.
- An extern struct and an **extern union** are `@memset(asBytes(&item), 0)`,
  every byte. An array is `@splat` of its element's zero.
- `zeroInit`: *"Initializes all fields of the struct with their default value,
  or zero values if no default value is present."*

The language reference's *Faulty Default Field Values* section
(https://codeberg.org/ziglang/zig/raw/branch/master/doc/langref.html.in) reads:
*"Default field values are only appropriate when the data invariants of a
struct cannot be violated by omitting that field from an initialization."*
178 had this unverified. It is verified now.

translate-c's PR #17196 (jayschwa, merged 2023-09-19) reads: *"C99 introduced
designated initializers for structs. Omitted fields are implicitly initialized
to zero. Some C APIs are designed with this in mind."*
(https://github.com/ziglang/zig/pull/17196). GitHub lists no milestone, so
178's *"milestone 0.12.0"* is **unverified**.

**Go, verified.**
- The spec's struct-literal rules, quoted at
  https://boldlygo.tech/archive/2023-09-26-struct-literals/, say a list
  *"that does not contain any keys must list an element for each struct
  field"*, and with keys *"Omitted fields get the zero value for that field."*
- The spec, version go1.27 of 2026-05-26 (https://go.dev/ref/spec): *"A literal
  may omit the element list; such a literal evaluates to the zero value for its
  type."*
- cgo (https://pkg.go.dev/cmd/cgo) has no sentence on a C struct's zero value.
  That `var x C.struct_foo` is zero is an inference from the spec, as 178 said.

**Swift, verified.**
- SE-0189, implemented in Swift 4.1: *"Most C structs also have a no-argument
  initializer that fills the struct with zeros unless one of the members is
  marked `_Nonnull`"*
  (https://github.com/swiftlang/swift-evolution/blob/main/proposals/0189-restrict-cross-module-struct-initializers.md).
- C arrays still import as tuples, which *"cannot be accessed through an index
  that only becomes known at runtime"*, and a C union imports as a struct of
  computed properties over one storage
  (https://github.com/swiftlang/swift/blob/main/docs/HowSwiftImportsCAPIs.md).
- The pitch *"Modernize imported C arrays"* (Becca Royal-Gordon, 2026-07-29)
  is still a pitch, last post 2026-08-04, with no SE number found
  (https://forums.swift.org/t/pitch-modernize-imported-c-arrays/88633).

**Odin, verified, and changed since 178.**
- *"Variables are initialized to zero by default unless specified otherwise"*,
  and a named-field literal zeroes what it omits
  (https://odin-lang.org/docs/overview/).
- **New:** *"`struct #all_or_none` is a new struct directive where struct
  literals must have all or none of their fields set when declaring a compound
  literal with named fields"*. It appears in the newsletter for 2025 Q4 and
  2026 Q1, published 2026-02-11 (https://odin-lang.org/news/newsletter-2026-q1/).
- The `#complete` request of 2020-10-07 was answered *"Because the zero value
  is meant to be useful in Odin, in most cases #complete won't be necessary"*,
  and is still not marked answered
  (https://github.com/odin-lang/Odin/discussions/962).
- A completeness mark therefore shipped about five years after it was asked
  for, per type and opt-in. *(Inference)* It is R1 inverted: Odin defaults to
  partial-and-zero and asks for *all or none* on the type, while Heroes
  defaults to all (`missing_fields`) and asks for *the named ones, then zero*
  at the construction.

**Nim, verified in part.** The manual says *"not all fields need to be
mentioned"* in an object construction (https://nim-lang.org/docs/manual.html).
`requiresInit`'s #20253 (2022-08-21) is **now closed by PR #21174**
(https://github.com/nim-lang/Nim/issues/20253).

**D, verified.** *"The default initializer value is not meant to be a useful
value, it is meant to expose bugs."* (https://dlang.org/articles/faq.html)

**Ada, verified.** Ada 83 §4.3 says *"Each component of the value defined by an
aggregate must be represented once and only once in the aggregate"*, with
`others` last and over components of one type
(https://people.cs.kuleuven.be/~dirk.craeynest/ada-belgium/docs/rm83/lrm-04-03.html).
Ada 2012 §4.3.1 says `<>` gives a component its default expression or
*"initialized by default as for a stand-alone object"*, and `others => <>` is
legal (https://www.adaic.org/resources/add_content/standards/12rm/html/RM-4-3-1.html).

### Q2. Zero as a valid value

**POSIX, verified** (https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_mutex_init.html).
`PTHREAD_MUTEX_INITIALIZER` is the static route, and nothing promises zero.
Using an uninitialised mutex is undefined, and the rationale lets an
implementation detect it with `EINVAL`, without requiring it.

**The initialisers, read in each libc's own header:**

| libc | `PTHREAD_MUTEX_INITIALIZER` | source |
|---|---|---|
| Apple libpthread | `{_PTHREAD_MUTEX_SIG_init, {0}}`, with `_PTHREAD_MUTEX_SIG_init 0x32AAABA7`. The cond, rwlock, once, errorcheck and recursive initialisers have the same `{SIG, {0}}` shape, signatures `0x3CB0B1BB`, `0x2DA8B3B4`, `0x30B1BCBA`, `0x32AAABA1`, `0x32AAABA2` | https://raw.githubusercontent.com/apple-oss-distributions/libpthread/main/include/pthread/pthread.h, https://raw.githubusercontent.com/apple-oss-distributions/libpthread/main/include/pthread/pthread_impl.h |
| glibc | `{ { __PTHREAD_MUTEX_INITIALIZER (PTHREAD_MUTEX_TIMED_NP) } }`, where `PTHREAD_MUTEX_TIMED_NP` is the enum's first value and the x86-64 expansion is `0, 0, 0, 0, __kind, 0, 0, { NULL, NULL }`. `PTHREAD_COND_INITIALIZER` is all zeros. `PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP` uses `PTHREAD_MUTEX_RECURSIVE_NP`, non-zero | https://raw.githubusercontent.com/bminor/glibc/master/sysdeps/nptl/pthread.h, https://raw.githubusercontent.com/bminor/glibc/master/sysdeps/x86/nptl/bits/struct_mutex.h |
| musl | `{{{0}}}` for mutex, rwlock and cond | https://git.musl-libc.org/cgit/musl/plain/include/pthread.h |
| FreeBSD | `NULL` for mutex, cond and rwlock | https://raw.githubusercontent.com/freebsd/freebsd-src/main/include/pthread.h |

**Apple's own zero-valid lock still names its constant, verified, new.**
`os_unfair_lock` is `struct { uint32_t _os_unfair_lock_opaque; }` and
`OS_UNFAIR_LOCK_INIT` is `((os_unfair_lock){0})`. Yet the header says *"Must be
initialized with OS_UNFAIR_LOCK_INIT."*, available from macOS 10.12
(https://raw.githubusercontent.com/apple-oss-distributions/libplatform/main/include/os/lock.h).
*(Inference)* Even where zero is valid, the vendor's contract is the named
constant, not zero, which is Z3's reading over Z2-as-validity.

**What each language promised:**
- **Go**, on its own type: *"The zero value for a Mutex is an unlocked mutex."*
  and *"A Mutex must not be copied after first use."*
  (https://pkg.go.dev/sync#Mutex).
- **Swift**, structurally: SE-0189's zero `init()`, refused only for
  `_Nonnull`. Mike Ash, 2017-10-27: *"you can create a value using the empty
  `()` initializer, but that value won't be a valid lock."*
  (https://www.mikeash.com/pyblog/friday-qa-2017-10-27-locks-thread-safety-and-swift-2017-edition.html).
- **Rust**: nothing structural. `MaybeUninit::zeroed` leaves validity to `T`,
  and pin-init's `Zeroable` is a per-type claim.
- **Zig**: a binding whose default fields are the initialiser.
  `PTHREAD_MUTEX_INITIALIZER = pthread_mutex_t{}` and
  `PTHREAD_COND_INITIALIZER = pthread_cond_t{}` came in Andrew Kelley's
  2020-12-21 commit *"std.Mutex: integrate with pthreads"*
  (https://git.medv.io/zig/commit/4eb4d26fa14524652bed69325eb491f39701d995.html).
  The Linux type's field defaults to `[_]u8{0} ** data_len`
  (https://ziggit.dev/t/size-of-std-c-pthread-mutex-t/5783). Its size was wrong
  on x86-64 glibc, 32 against 40, issue #21229 (2024-08-28), closed by PR #21237
  (https://github.com/ziglang/zig/issues/21229). Whether the Darwin type
  defaults `sig` to `0x32AAABA7` is **unverified**: I could not reach the
  definition.
- **D**: issue 14617 (Martin Nowak, 2015-05-23) was fixed in druntime PR #1285,
  pushed to master 2015-05-25 and to stable 2015-06-17, with *"Define correct
  values for PTHREAD_MUTEX_INITIALIZER and PTHREAD_ONCE_INIT"*
  (https://forum.dlang.org/thread/bug-14617-3@https.issues.dlang.org/).
  Andrei Alexandrescu first proposed a struct default, `c_long __sig =
  0x32AAABA7;`. It was dropped for a build reason (`types.d` was not in the
  build) in favour of the constant. **178's dates hold.** That the value
  shipped before was all-zero is 178's reading; *(inference)* the fix setting
  `__sig` points that way.

**Searched for and not found, again:** any system that states zero-validity
per platform. The nearest is Zig's per-OS struct whose defaults are that OS's
initialiser. That states the valid value, not whether zero is valid.

### Q3. A whole struct lent to `void *` with a separate count (defect 092)

**C's own hardening checks a caller's count against the object, verified.**
- glibc's fortified `read` is `__glibc_fortify (read, __nbytes, sizeof (char),
  __glibc_objsize0 (__buf), ...)`
  (https://raw.githubusercontent.com/bminor/glibc/master/posix/bits/unistd.h).
- Its `poll` is `__glibc_fortify (poll, __nfds, sizeof (*__fds), __glibc_objsize
  (__fds), ...)`, so it counts **records** where `read` counts **bytes**
  (https://raw.githubusercontent.com/bminor/glibc/master/io/bits/poll2.h).
- `__read_chk` is in the `GLIBC_2.4` symbol block and `__poll_chk` in
  `GLIBC_2.16` (https://raw.githubusercontent.com/bminor/glibc/master/debug/Versions).
  Those releases were 2006-03-06 and 2012-06-30, dates from search listings
  only.
- glibc 2.40 (2024-07-22) says *"The fortify functionality has been
  significantly enhanced for building programs with clang"*
  (https://sourceware.org/pipermail/libc-alpha/2024-July/158467.html).
- *(A question, unrun)* Does Heroes' emitted C build with `_FORTIFY_SOURCE` on
  Linux? If it does, glibc would already catch the reproducer at run time.

**The C header declares the extent, verified.**
- glibc declares `read (int __fd, void *__buf, size_t __nbytes) ...
  __fortified_attr_access (__write_only__, 2, 3)` and `poll (struct pollfd
  *__fds, nfds_t __nfds, int __timeout) __fortified_attr_access
  (__write_only__, 1, 2)`
  (https://raw.githubusercontent.com/bminor/glibc/master/posix/unistd.h,
  https://raw.githubusercontent.com/bminor/glibc/master/io/sys/poll.h).
- GCC's `access` attribute, new in GCC 10 (*"to associate such arguments with
  integer arguments denoting the objects' sizes"*,
  https://gcc.gnu.org/gcc-10/changes.html), defines the unit: *"The size is the
  number of elements of the type referenced by ref-index, or the number of
  bytes when the pointer type is `void*`."* Without a size, *"the pointer
  argument must be either null or point to a space that is suitably aligned and
  large for at least one object of the referenced type."*
  (https://gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/Common-Function-Attributes.html).
- glibc emits the attribute only under `#if __GNUC_PREREQ (10, 0)`
  (https://raw.githubusercontent.com/bminor/glibc/master/misc/sys/cdefs.h).
  Clang poses as GCC 4.2.1 (https://lists.llvm.org/pipermail/cfe-dev/2019-July/062890.html,
  search listing). *(Inference)* So under clang these declarations expand to
  nothing, and Heroes' pointee check cannot read them.

**Bounded-C dialects use the same unit rule, verified.**
- Clang's `-fbounds-safety` (https://clang.llvm.org/docs/BoundsSafety.html)
  defines `__counted_by(N)` as *"`N` elements of pointee type"* and
  `__sized_by(N)` as *"`N` bytes"*. It also says: *"The `__counted_by`
  annotation cannot apply to pointers to incomplete types or types without size
  such as `void *`. Instead, `__sized_by` can be used to describe the byte
  count."*
- In the same dialect, *"All ABI-visible pointers are treated as `__single` by
  default unless annotated otherwise"*, meaning one object or null. The page
  still says *"This is a design document and the feature is not available for
  users yet."*
- Checked C (https://github.com/checkedc/checkedc-fork/wiki/Bounds-declarations)
  has `count(e)`, *"the number of array elements"*, and `byte_count(e)`, *"the
  number of bytes"*.
- Its bounds-safe interfaces redeclare `memcpy(void * restrict dest :
  byte_count(n), ...)` and `fread(void * restrict p : byte_count(size * nmemb),
  ...)` (https://github.com/Microsoft/checkedc/wiki/Bounds-safe-interfaces).
- Its 2021 to 2024 fork was archived 2024-09-30, *"The changes have been merged
  into the original Checked C repo"* (https://github.com/secure-sw-dev/checkedc).
  The main repository's activity today is **unverified**.
- CERT ARR38-C (https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/arrays-arr/arr38-c)
  names the class: *"C library functions that make changes to arrays or
  objects take at least two arguments: a pointer to the array or object and an
  integer indicating the number of elements or bytes to be manipulated."* Its
  noncompliant example is a unit confusion (`wmemcpy` given `sizeof`), rated
  severity High, likelihood Likely, P9 L2.

**The safe layers derive the count from the object, verified.**
- **Rust std** (https://doc.rust-lang.org/src/std/sys/net/connection/socket/mod.rs.html):
  `setsockopt` passes `size_of::<T>()`. `getsockopt` starts from
  `MaybeUninit::<T>::zeroed()` with `option_len = size_of::<T>()` and lets C
  fill it, which is the lane's route D for `getsockopt`, and kind 2's
  zero-then-C-fills.
- **bytemuck**: `bytes_of_mut<T: NoUninit + AnyBitPattern>(t: &mut T) -> &mut
  [u8]` (https://docs.rs/bytemuck/latest/bytemuck/fn.bytes_of_mut.html). A
  record lent as writable bytes must have no padding and must accept every bit
  pattern.
- **Zig std**: `read(fd, buf: []u8)` calls `system.read(fd, buf.ptr,
  @min(buf.len, max_count))`. `poll(fds: []pollfd, timeout)` passes `fds.len`,
  in records. `setsockopt(..., opt: []const u8)` passes `opt.ptr, opt.len`
  (https://codeberg.org/ziglang/zig/raw/branch/master/lib/std/posix.zig).
  `asBytes` returns `*const [@sizeOf(P.child)]u8` (mem.zig, above).
- **Go x/sys/unix**: `SetsockoptIPMreq` passes `SizeofIPMreq` and
  `GetsockoptInt` uses `_Socklen(4)`
  (https://raw.githubusercontent.com/golang/sys/master/unix/syscall_unix.go).
  `Poll(fds []PollFd, ...)` goes through `Ppoll(fds, ...)`
  (https://raw.githubusercontent.com/golang/sys/master/unix/syscall_linux.go).
  cgo bounds nothing: no sentence on how many bytes C may reach
  (https://pkg.go.dev/cmd/cgo).
- **Swift**: `withUnsafeMutableBytes(of:)` gives a buffer that *"is the size of
  the instance passed as `value`"*
  (https://developer.apple.com/documentation/swift/withunsafemutablebytes(of:_:).md).
  With `__counted_by` and `__noescape` in the C header and the experimental
  `SafeInteropWrappers`, WWDC25 session 311 shows `void invertImage(uint8_t
  *__counted_by(imageSize) imagePtr __noescape, size_t imageSize)` imported as
  `func invertImage(_ imagePtr : inout MutableSpan<UInt8>)`. In the session's
  words, *"the compiler will take care of unwrapping the Span, extracting the
  correct pointer and size, and pass them to the C function for me"*
  (https://developer.apple.com/videos/play/wwdc2025/311/). Apple DTS confirmed
  the flags on 2025-11-21
  (https://forums.swift.org/t/c-annotations-counted-by-and-noescape-not-producing-span-consuming-function-signatures/83333).

**Cyclone, verified in part.** It bounds a pointer by a constant,
`int *@numelts(4)`, and its fat pointers carry their own bound, *"you can get
the size of `argv` by writing `numelts(argv)`"*
(https://www.cs.cornell.edu/projects/cyclone/online-manual/main-screen002.html).
A WebFetch summary also offered a Cyclone example tying `@numelts` to another
parameter. Asked for it verbatim, the page had **none**. It is not cited.

### On C's `char` (178's point 5, and the critic's 0-token note route)

- **Go forces signed `char`, verified.** `x/sys/unix`'s generator sets
  `SignedChar: true` for arm64 and s390x and appends `-fsigned-char`
  (https://raw.githubusercontent.com/golang/sys/master/unix/linux/mkall.go). So
  `RawSockaddr.Data` is `[14]int8` on both linux/arm64 and linux/amd64
  (https://raw.githubusercontent.com/golang/sys/master/unix/ztypes_linux_arm64.go,
  .../ztypes_linux_amd64.go). This is Heroes' choice (the brief: the emitter
  passes `-fsigned-char` on every leg).
- **Rust follows each platform, verified.** `c_char` is `u8` on aarch64, arm,
  riscv and others except Apple, Windows and Vita, and `i8` elsewhere
  (https://doc.rust-lang.org/src/core/ffi/primitives.rs.html).
- *(Inference)* A reader trained on Rust's Linux arm64 rule writes `u8`, which
  fits 178's 22 of 50.

## Each route, what precedent says

| route | reading | for | against | what would change it |
|---|---|---|---|---|
| **R1** `rest: zero` | approve | kernel `..Zeroable::init_zeroed()` (omission without it a compile error); Rust `..` (still unstable); Ada `others => <>`; Odin's `#all_or_none` shows completeness returning where omission was silent | none against the mark; Principle 0 reads 0 (the brief), which precedent cannot settle | a language with a marked rest-zero that withdrew it (none found) |
| R1 on a **union** | define it, two cases | C's union initialisation covers one member (C11 fn 149; C23 §6.7.10p11). Whole-union zero comes from C23 `{}` and clang's extension, which clang accepts in every C mode. Zig memsets an extern union | GCC 15 withdrew whole-union `{0}`; §6.2.6.1p7 makes the other bytes unspecified after any member store | *(inference)* `U(rest: zero)` with no member named lowers to `{}`, every byte zero on clang. With a member named, only clang promises the rest; a refusal is the conservative option |
| R1 **padding** | do not promise (178's point 9) | clang 20's extension zeroes padding at initialisation | C23 kept §6.2.6.1p6 and footnote 56; GCC 15; CERT DCL39-C's memset-then-store is noncompliant | a C2y paragraph keeping padding zero after a store |
| R0 separate spelling | refuse; `T(rest: zero)` covers it | Rust std's `getsockopt` is zero-then-C-fills, with no second spelling | none | none |
| **Z1** claim mark | object | bytemuck and pin-init `Zeroable` claim validity per type | no per-platform statement exists anywhere I searched; Apple documents its zero-valid lock by the named INIT; 178 measured 0 of 10 false claims, with `partial` reaching the same state | a checkable per-platform zero-validity source |
| **Z2** as bytes | approve, no validity sentence | `MaybeUninit::zeroed` leaves validity to `T`; Zig `zeroes` on extern structs; C §6.7.9p10; C23 §6.2.6.2p3 | Swift's `T()` mutex (Mike Ash); D 14617; bindgen #2974 | a § 13 sentence implying validity would bring Swift's failure back |
| **Z3** header's initialiser | approve | D fixed 14617 with the right initialiser constant; Zig makes the initialiser the type's defaults; Apple's own lock asks for its INIT | Apple's six initialisers share the shape `{SIG, {0}}` and glibc and musl use all-zero lists, so a brace list fits several types. That is the lane's untyped-brace finding, explained by the headers themselves | none; the limit is structural and should be written down, as 178 said |
| **L** (091) | closed by defect 097 (the brief) | — | — | — |
| **T** | wait | the fallible copies of 178 (not re-checked) | — | 178's measurement conditions |
| **A** `[x; N]` | wait; if it returns, as `[x; _]` | Rust 1.89 (2025-08-07) shipped `[false; _]`, a length inferred from the type, which answers the author's *"writes a platform's length into every construction"* | Principle 0 reads 0; only a non-zero repetition is A's own case | a measured need for a non-zero fill |
| **R2** silent zero | object | Zig translate-c (2023) | Odin `#all_or_none` (2026); Go `exhaustruct`; Rust RFC 3681 chose an explicit `..` | evidence that Zig's zero defaults caused no forgotten-field bugs (none found either way) |
| **D** zero default for every type | object | Go, Odin | D's FAQ; Nim `requiresInit` (#20253, closed only by #21174); Odin and Go users asking for completeness | — |
| **`char` is `i8`** | approve saying it, where measurement decides | Go forces `-fsigned-char` like Heroes | Rust's `c_char` is `u8` on Linux arm64 | — |
| `missing_fields` offering `rest: zero` | approve as a help, a `guess` fix *(inference)* | rustc E0063 adds *"you can use those values with `..`"* and stays an error | a `certain` fix would write a forgotten field as zero | — |
| **nothing** | precedent does not support it | — | every FFI layer I read has a zero-the-rest form (C `{}`, Rust `..`/`zeroed`, Zig `zeroes`/`zeroInit`, Go keyed literals, Swift `T()`); I found none without one | a language binding long-array C structs with no such form |
| **092-A** declared count, run-time compare, undeclared `void *` refused | approve | glibc FORTIFY (`__read_chk` since GLIBC_2.4); GCC `access`; Checked C `byte_count`; `-fbounds-safety` refusing `__counted_by` on `void *` | E alone leaves a correct program unbindable | — |
| **route C** unit by pointee | approve | GCC `access`, `__counted_by`/`__sized_by`, Checked C `count`/`byte_count`, glibc `poll` (`sizeof (*__fds)`), all four agreeing; CERT ARR38-C rates unit confusion High | — | a precedent that counts typed pointers in bytes |
| **092-C′** refuse a count on a typed pointer | object | none found | every precedent above counts typed pointers in elements | — |
| gap 2: typed pointer, no declaration | precedent reads it as **one object** | GCC `access` without a size; `-fbounds-safety`'s `__single` default | nothing caller-side catches `poll(&one, 512)`; glibc catches it only because `poll` is annotated | a system that refuses an undeclared typed lend beside an integer (none found) |
| **092-B** `x.ptr()` lends the whole record | approve as the remedy | Zig `asBytes`; Swift `withUnsafeMutableBytes(of:)`; bytemuck `bytes_of_mut` | bytemuck needs `NoUninit + AnyBitPattern`, *(a question, unrun)*: a record holding a `ptr` field, lent to `read`, lets C write any pointer bits | — |
| **092-D** the compiler passes the size | approve as the direction | Rust std socket options; Go x/sys; Zig std.posix; Swift SafeInteropWrappers (experimental), which **removes the count from the call**, as D would | it cannot ask for fewer bytes than the record, so 092-A or B stays for `read` of part | — |
| **092-E** refusal alone | object | — | every language read offers a byte view of one record | — |

## 178's report, claim by claim

**Hold, re-checked this session:**
- SE-0189; `HowSwiftImportsCAPIs`; the Swift pitch (still a pitch).
- Zig `zeroes`, its refusals and `zeroInit`; PR #17196, with its milestone now
  **unverified**.
- RFC 3681, still unstable; `mem::zeroed` padding, via `MaybeUninit::zeroed`.
- bindgen #2974 and #2803 and Rust #61415, all still open.
- pin-init `..Zeroable::init_zeroed()`.
- The Go spec; `sync.Mutex`; the cgo inference.
- C23 §6.7.10p11; C11 §6.2.6.1p6 (as C23's identical text); footnote 51 (C23's
  footnote 56); §6.7.9p10, p19 and p21, re-read in N1570.
- CERT DCL39-C; GCC 15's union change.
- Apple's `SIG` constants and initialisers; D 14617 (both dates); Mike Ash;
  POSIX; the D FAQ; Ada 83 §4.3 and Ada 2012 §4.3.1.
- Zig #21229, now closed by PR #21237.

**Changed:**
- Odin: `#all_or_none` has shipped; the request is no longer unresolved in
  effect.
- Zig's *Faulty Default Field Values*: unverified then, verified now.
- Rust's repeat expression: `_` lengths since 1.89.
- Nim #20253: closed by #21174.
- New sources that 178 lacked: clang 20's padding extension; the kernel's
  kbuild flag.

**Not re-checked**, and not used above:
- bytemuck's Zeroable lengths; the kernel's `MaybeZeroable` commit
  4846300ba8f9; Nim #13808 and #5140; the Nim posix files.
- Gustedt; Memfault; GCC 15.1's date; perf's 2025-03-20 flag.
- Everything under 178's T section (strncpy, strlcpy, the kernel removals, Rust
  `sockaddr_un`, Go `syscall_*`, Zig `initUnix`, swift-nio, Oberon-07,
  Kernighan, Extended Pascal, Cyclone `for`).
- Go's Darwin and Linux `Utsname` and `Path`; SE-0453 and SE-0483; Ada §4.3.3;
  the D struct spec; Go's proverbs and `exhaustruct`'s version.

**178's predictions:**
- *No C paragraph keeps padding zero after a store*: still held. C23 kept the
  text, and clang's promise is about initialisation.
- *The first `sockaddr_un` binding is zero-then-store*: not scorable, since no
  binding exists (Principle 0's count, the brief).
- *Z2 reproduces measurement 7 on Darwin*: consistent with the critic's re-run
  of 22 on 22 (the brief's number, not mine).

## argument (≤120 words)

Marked omission has lasted: Rust's `..`, the kernel's `..Zeroable::init_zeroed()`, Ada's `others => <>`. Silent omission brought completeness back: Odin shipped `#all_or_none` by 2026, after a 2020 request. So R1, not R2 or D. Zero is bytes, never validity: Apple's zero-valid `os_unfair_lock` still says to use `OS_UNFAIR_LOCK_INIT`; Swift's `T()` and D's druntime gave Darwin a mutex that fails, and D fixed it with the right initialiser constant, Z3's shape. Padding stays unpromised: C23 kept §6.2.6.1p6 while clang 20 and GCC 15 moved opposite ways. For 092, every safe layer I read derives the count from the object; C's own hardening counts elements of the pointee type, and bytes for `void *`.

## condition

My reading changes if any of these turns up:
1. A C paragraph, C2y included, keeping padding zero after a store. That lifts
   the padding refusal.
2. A checkable per-platform statement of zero-validity. That revives Z1.
3. Evidence either way on Zig translate-c's zero defaults (2023). That moves
   R2.
4. A language that binds long-array C structs comfortably with no
   zero-the-rest form. That supports *nothing*.
5. For 092, a system that refuses an undeclared typed-pointer lend beside an
   integer parameter, or one that counts typed pointers in bytes. The first
   would argue for closing gap 2 by refusal; the second would revive 092-C′.
   I found neither.

Brief files read: `<scratchpad>/p194/briefs/00-shared.md`, `historian.md`;
`docs/panel/178-reports/historian.md`,
`docs/panel/178-the-rest-is-zero-where-it-is-written-and-c-says-which-value-is-valid.md`,
`docs/panel/194-evidence/092-routes.md`,
`issues/2026-10/06/2026-10-06-1118-panel-178-reaches-the-trunk-sat-again-on-the-trunk-of-today.md`
(all in the lane-panel-194 worktree, read only).
