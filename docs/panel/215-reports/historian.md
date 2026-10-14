# Panel 215, historian seat (advisory, no veto)

Written 2026-10-14. **Every source below was read this session, 2026-10-14**, at the URL given.
**verified** means I read the primary text, or a named secondary that quotes it. **unverified**
means I could not confirm it. *(inference)* marks reasoning from a source, not a repetition of it.
No claim rests on *Heroes of code*.

This seat has no shell. Its tools are Read, Write, WebSearch and WebFetch. So it measured nothing in
the tree, ran no probe, and claims no costs. In particular it **did not count the group records**
the shared brief asks every seat to count: that count is unrun here. Numbers about Heroes are the
shared brief's and are cited as the brief's.

Brief files read: `docs/panel/215-briefs/00-shared.md` and `historian.md`; also
`docs/panel/178-reports/historian.md`, `docs/panel/194-reports/historian.md`,
`docs/panel/178-reports/completeness-critic.md` (§ 1), `docs/roadmap/milestones/M-buildable-structs.md`,
and `spec/heroes-spec.md` lines 336 to 399. All are in the lane-panel-215 worktree, read only.

## verdict

**Advisory.**

**Q1, padding across a call by value: approve A as the specification's position, together with an
unlisted route F, with E as F's remedy. Object to B, and to D in its struct-copy form.**
- **A**: the specification promises nothing about padding. That is C's position since 2004, and
  Rust's and Zig's.
- **F** (unlisted): refuse to hand C a group record **that has padding** as bytes, at a `void *`
  lend. This is how Rust's byte-view crates close the class.
- **E**: name the padding as a field. This is CERT's compliant solution, cgo's generated layout and
  zerocopy's own advice.
- **C** is precedented for size, never for padding. It cannot reach a C function that takes the
  record by value.

**Q2, a record C forbids to copy: approve B, widened to moves and to any value holding the record.
Seed it by name from POSIX's own list (a form of C). Object to A and A2 as the answer.**
- The **mark** has 40 years of precedent: Ada 83's `limited`, which GNAT has used for
  `pthread_mutex_t` since at least its GCC 4.3.0 sources; C++11's `= delete`; D; Nim; Swift.
- The **raw-copyable** layers (Rust's `libc`, Swift's C import, cgo) are safe only because a
  wrapper sits above them. Heroes binds pthread directly, with no standard library between.
- Zig is the one counter-precedent: on 2026-04-21 it closed its *pinned structs* proposal in
  favour of run-time checks in user code.

**Q3, R1's payment: approve A with D. Object to B.**
- Precedent cannot score a reader test.
- What it can say: Rust's own `c_char` is `u8` on Linux arm64, which is the mistake R1's objection
  condition counts.
- I found no language that withdrew a marked zero-the-rest form.

## precedents

### Q1. What a by-value copy does to padding

**C, verified. The rule is 22 years old and was written down on purpose.**
- WG14 N1060, *Technical Corrigendum 2*, 2004-03-31, from **DR 222**, sets C99 §6.2.6.1p6 to
  *"When a value is stored in an object of structure or union type, including in a member object,
  the bytes of the object representation that correspond to any padding bytes take unspecified
  value"*. Its footnote reads *"Thus, for example, structure assignment need not copy any padding
  bits."* (https://www.open-std.org/JTC1/SC22/wg14/www/docs/n1060.htm)
- C11 keeps the paragraph, with the footnote numbered 51 (N1570 §6.2.6.1p6,
  http://port70.net/~nsz/c/c11/n1570.html).
- **A call is an assignment.** C11 §6.5.2.2p4: *"each parameter is assigned the value of the
  corresponding argument"*. Footnote 93: *"A function may change the values of its parameters,
  but these changes cannot affect the values of the arguments."* (same URL)
- *(inference)* So C allows the loss the critic measured at `cd_through`, at every optimisation
  level. A route that keeps a by-value C parameter cannot promise padding.

**Why x86-64 loses the bytes and arm64 keeps them: clang's own source, verified.**
- **x86-64.** `clang/lib/CodeGen/Targets/X86.cpp`, `GetINTEGERTypeAtOffset`:
  *"If we have a 1/2/4-byte integer, we can use it only if the rest of the goodness in the source
  type is just tail padding."* It then returns `i8`/`i16`/`i32` when
  `BitsContainNoUserData(SourceTy, SourceOffset*8+BitWidth, SourceOffset*8+64, ...)` holds.
  `BitsContainNoUserData` returns true when the bit range is *"off the end of the specified type or
  being in alignment padding"*
  (https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/CodeGen/Targets/X86.cpp).
- clang's own test does this to a 16-byte struct: `struct s81 { double d; int : 32; }` is passed as
  `(double %a.coerce0, i32 %a.coerce1)`
  (https://raw.githubusercontent.com/llvm/llvm-project/main/clang/test/CodeGen/X86/x86_64-arguments.c).
- **AArch64.** `Targets/AArch64.cpp`: *"Aggregates <= 16 bytes are passed directly in registers or
  on the stack"*, and *"We use a pair of i64 for 16-byte aggregate with 8-byte alignment."*
  (https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/CodeGen/Targets/AArch64.cpp)
- *(inference, unrun)* `{char c; double d}` crosses x86-64 as `i8, double`, so bytes 1 to 7 never
  travel. On arm64 it crosses as `[2 x i64]`, and they do. That matches the critic's split, x86-64
  reported and arm64 clean.
- **Windows x64.** Microsoft's convention says *"Structs or unions of other sizes are passed as a
  pointer to memory allocated by the caller"*, and *"16-byte arguments are passed by reference"*
  (https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention, updated 2026-05-21).
  Whether the caller's copy keeps the padding is **unverified**, and unrun on the box.

**CERT DCL39-C, verified** (https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/declarations-and-initialization-dcl/dcl39-c):
- The rule says *"initializing the members of the structure does not guarantee initialization of
  the padding bytes"*.
- **memset-then-assign is a noncompliant example.** This is what route B is, and the critic
  measured that it does not close the loss.
- Its compliant solutions are:
  1. copy each field into a zeroed byte buffer (serialisation);
  2. *"Padding bytes can be explicitly declared as fields within the structure"*, called
     non-portable and checked with `static_assert` (route E);
  3. packing, with GCC's attribute or MSVC's `#pragma pack`.
- It cites CVE-2010-4083, CVE-2010-3881 and CVE-2010-3477, all kernel or QEMU-KVM leaks.
- Its risk rating: severity **Low**, likelihood **Unlikely**, **P2 L3**.
- On copies it says only that *"structure and union assignment may or may not copy any padding
  bits"*. It says nothing about passing by value.

**Rust, verified.**
- The Unsafe Code Guidelines glossary models padding as secret `Pad` fields: *"Copying `Pad`
  ignores the source byte, and writes any value to the target byte"*. It also says *"the moment you
  perform a typed copy out of that reference you will have uninitialized padding bytes in the
  copy"* (https://rust-lang.github.io/unsafe-code-guidelines/glossary.html).
- The Reference: *"An integer ... must be initialized, i.e., must not be obtained from
  uninitialized memory"* (https://doc.rust-lang.org/reference/behavior-considered-undefined.html).
- **The byte-view crates refuse padded types outright.**
  - zerocopy 0.8.63's `IntoBytes` requires a type that *"has no padding bytes"*. Its fix list
    begins with *"adding explicit struct fields where those padding bytes would be"*
    (https://docs.rs/zerocopy/latest/zerocopy/derive.IntoBytes.html).
  - bytemuck 1.25.2's `NoUninit`: *"The type must not contain any uninit (or padding) bytes, either
    in the middle or on the end"* (https://docs.rs/bytemuck/latest/bytemuck/trait.NoUninit.html).
- These two are the precedent for **route F**.

**Go and cgo, verified. This is route E, shipped, and it shows E's limit.**
- The cgo documentation: C fields Go cannot express are *"omitted in the Go struct, replaced by
  appropriate padding to reach the next field or the end of the struct"*
  (https://pkg.go.dev/cmd/cgo).
- The generated types name that padding as blank fields. In `x/sys/unix`'s
  `ztypes_linux_amd64.go`, `Flock_t` ends `Pid int32; _ [4]byte`, made by
  `cgo -godefs`
  (https://raw.githubusercontent.com/golang/sys/master/unix/ztypes_linux_amd64.go).
- The Go spec, go1.27 (May 26, 2026): *"Two struct values are equal if their corresponding
  non-blank field values are equal"* (https://go.dev/ref/spec). So `==` ignores the named padding,
  as Heroes' field-walking `==` already does (the shared brief).
- **The limit.** Go issue #26167, *"cmd/compile: msan cannot handle structs with padding"*, was
  opened 2018-07-01 and is **still open** (https://github.com/golang/go/issues/26167):
  - when **C** wrote the struct, *"Only bytes 0-5 are marked as initialized by C (bytes 6-8 are
    padding)"*, and Go's whole-object copy reads all 8;
  - Ian Lance Taylor confirms *"cmd/cgo does add padding fields to the Go version of C structs"*.
  - *(inference)* E repairs records the program built. It does not repair records C filled.

**Zig, verified in part.**
- On Ziggit, 2025-05-17, the question *"Is the compiler allowed to clobber the padding of a
  struct?"* got this answer from `andrewrk`: *"Yes it is allowed and in fact is already done for
  example for storing type safety information"*. The same answer says *"If you want to reserve space
  within struct padding, simply declare the data as a field along with the others"*
  (https://ziggit.dev/t/is-the-compiler-allowed-to-clobber-struct-padding/10164).
- That `andrewrk` is Zig's lead is **unverified on that page**. Zig's language reference was not
  read for this point.

**Nim, verified. This is route C, for size and never for padding.**
- `compiler/ccgutils.nim`, `ccgIntroducedPtr`, passes an object larger than
  `conf.target.floatSize * 3` by hidden pointer
  (https://raw.githubusercontent.com/nim-lang/Nim/devel/compiler/ccgutils.nim).
- The manual's `bycopy` and `byref` pragmas exist *"for interop"*: *"The Nim compiler automatically
  determines whether a parameter is passed by value or by reference"*
  (https://nim-lang.org/docs/manual.html).
- *(inference)* A 16-byte `{char; double}` falls under Nim's 24-byte threshold, so it would still
  cross by value.
- I found no language that passes records by address in order to keep their padding.

### Q2. A C record that must not be copied

**POSIX, verified. The rule is about the address, not only about copying.**
- `pthread_mutex_init`: *"this volume of POSIX.1-2024 does not define assignment or equality for
  this type"* (https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_mutex_init.html).
- XSH §2.9.9, *Synchronization Object Copies and Alternative Mappings*: for a process-private
  object, *"only the synchronization object at the address used to initialize it can be used for
  performing synchronization"*, and using a copy is undefined
  (https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html).
- §2.9.9 names the whole set: barriers, condition variables, mutexes, read-write locks, spin locks,
  semaphores, and ISO C `mtx_t` and `cnd_t` (same page).
- Detection is not required; only resource errors are.

**Ada and GNAT, verified. Route B, in 1983, applied to `pthread_mutex_t`.**
- Ada 83 LRM §7.4.4: *"A limited type is a type for which neither assignment nor the predefined
  comparison for equality and inequality is implicitly declared"*
  (https://people.cs.kuleuven.be/~dirk.craeynest/ada-belgium/docs/rm83/lrm-07-04.html).
- GNAT's runtime binds `type pthread_mutex_t is limited private;`. Its full view is
  `Data : char_array (1 .. OS_Constants.PTHREAD_MUTEX_SIZE)`
  (https://gnu.googlesource.com/gcc.git/+/refs/heads/master/gcc/ada/libgnat/s-oslock__posix.ads).
  That is A2's opaque bytes, made limited.
- `pthread_cond_t` and `pthread_rwlock_t` are also `limited private`
  (https://raw.githubusercontent.com/gcc-mirror/gcc/master/gcc/ada/libgnarl/s-osinte__linux.ads).
- The same `limited private` declarations stand in GCC 4.3.0's `s-osinte-linux.ads`, copyright
  1995-2007
  (https://raw.githubusercontent.com/gcc-mirror/gcc/releases/gcc-4.3.0/gcc/ada/s-osinte-linux.ads).
  GCC 4.3.0's release date is **unverified**.

**C++, verified.**
- N2346, *Defaulted and Deleted Functions* (Lawrence Crowl, 2007-07-19), replaced *"a private copy
  constructor and a private copy assignment operator"* with `= delete`
  (https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2346.htm).
- `std::mutex` *"is neither copyable nor movable"*, since C++11
  (https://en.cppreference.com/w/cpp/thread/mutex).

**D, verified.** *"Disabling struct postblit makes the object not copyable."* Its example shows the
rule is transitive: *"uncopyable member makes S also not copyable"*
(https://dlang.org/spec/struct.html).

**Nim, verified.** *"The =copy proc can be marked with the {.error.} pragma. Then any assignment
that otherwise would lead to a copy is prevented at compile-time"*
(https://nim-lang.org/docs/destructors.html).

**Swift, verified. The mark exists, C structs cannot carry it, and a mark alone does not pin the
address.**
- SE-0390, noncopyable structs and enums, *"Implemented (Swift 5.9)"*. A parameter of such a type
  *"must declare whether the parameter uses the `borrowing`, `consuming`, or `inout` convention"*
  (https://github.com/swiftlang/swift-evolution/blob/main/proposals/0390-noncopyable-structs-and-enums.md).
- SE-0433, `Mutex`, *"Implemented (Swift 6.0)"*, is declared `struct Mutex<...>: ~Copyable`. It
  wraps `os_unfair_lock`, futex or `SRWLOCK`, not `pthread_mutex_t`
  (https://github.com/swiftlang/swift-evolution/blob/main/proposals/0433-mutex.md).
- SE-0410's `Atomic` lives at *"a single, stable memory location"*, and *"It is a compile-time error
  to have a `var`"* of it
  (https://github.com/swiftlang/swift-evolution/blob/main/proposals/0410-atomics.md).
- **From the header.** *"C++ structures and classes with a deleted copy constructor are represented
  as non-copyable Swift types"*, and `SWIFT_NONCOPYABLE` marks others
  (https://www.swift.org/documentation/cxx-interop/). The page covers C++ only.
- For a **C** type, a forum answer of 2024-03-12 offers only a Swift wrapper
  (https://forums.swift.org/t/is-there-a-way-to-declare-import-c-type-as-a-noncopyable-struct-in-swift/70611).
- On 2026-04-08 a `z_stream` inside a `~Copyable` Swift struct still changed address between calls.
  John McCall: *"The problem is precisely that C structs are formally copyable"*. The poster moved
  to `calloc`
  (https://forums.swift.org/t/c-struct-in-a-swift-struct-has-an-unstable-address/85894).
- Mike Ash, 2017-10-27: *"If you copy one of the `pthread` types, the copy will be unusable and may
  crash"*. The pthread functions *"assume that the values are at the same memory addresses as where
  they were initialized"*
  (https://www.mikeash.com/pyblog/friday-qa-2017-10-27-locks-thread-safety-and-swift-2017-edition.html).

**Rust, verified. The raw layer copies; the safe layer boxes or pins.**
- `libc` 0.2.190 gives `pthread_mutex_t` the traits `Clone`, `Copy`, `Eq` and `Hash`
  (https://docs.rs/libc/latest/libc/struct.pthread_mutex_t.html).
- bindgen derives `Copy` by default (0.73.2, https://docs.rs/bindgen/latest/bindgen/struct.Builder.html).
  Its `nocopy` page uses this example: *"this struct is not safe to move because pthread mutexes
  can't move in memory!"*. The annotation goes in a header comment, `<div rustbindgen nocopy></div>`,
  or by regex, `--no-copy` (https://rust-lang.github.io/rust-bindgen/nocopy.html).
  *(inference)* Removing `Copy` still lets Rust move the value.
- Rust 1.0's `Mutex` boxed the native lock: *"Once a native mutex has been used once, its address
  can never change (it can't be moved) ... we box the inner lock to give it a constant address"*
  (https://doc.rust-lang.org/1.0.0/src/std/sync/mutex.rs.html).
- Rust 1.62.0 (blog of 2022-06-30): the pthread mutex *"is 40 bytes and cannot be moved. This forced
  the standard library to allocate a `Box`"*; Linux moved to futexes
  (https://blog.rust-lang.org/2022/06/30/Rust-1.62.0/).
- Tracking issue #93740, opened 2022-02-07: the other Unix platforms kept pthread locks, lazily
  boxed (https://github.com/rust-lang/rust/issues/93740).
- In Linux's Rust, *"Instances of [`Mutex`] need a lock class and to be pinned"*
  (https://github.com/torvalds/linux/blob/master/rust/kernel/sync/lock/mutex.rs).

**Go, verified. Route D at run time, plus a lint.**
- `sync.Cond` carries a `copyChecker` that *"holds back pointer to itself to detect object copying"*
  and calls `panic("sync.Cond is copied")`
  (https://raw.githubusercontent.com/golang/go/master/src/sync/cond.go).
- `go vet`'s `copylocks` warns that *"Inadvertently copying a value containing a lock ... may cause
  both copies to malfunction"*. It finds a lock **by method set**: *"a pointer to this type is a
  sync.Locker, but a value is not"*
  (https://raw.githubusercontent.com/golang/tools/master/go/analysis/passes/copylock/copylock.go).
- *(inference)* A cgo `C.pthread_mutex_t` has no methods, so `vet` does not see it.

**Zig, verified. The counter-precedent.**
- Proposal #7769, *Pinned Structs*, by SpexGuy, was opened 2021-01-13 to catch *"accidentally
  make a value copy"* (https://github.com/ziglang/zig/issues/7769).
- On 2025-06-10 it was unaccepted.
- On 2025-08-20 `mlugg` argued that a user-code `AddrLock` *"is able to catch these cases at
  runtime with an assertion"*.
- On 2026-04-21 `andrewrk` closed it *"in favor of userland safety alternatives which keep the
  language simpler"*.
- `AddrLock`'s code was not read: **unverified**.

### Q3. R1's payment

- **Rust's `c_char` is `u8`** on `aarch64` and nine other architectures, except on Windows, Apple
  and Vita (`core/ffi/primitives.rs`, https://doc.rust-lang.org/src/core/ffi/primitives.rs.html,
  verified).
  - *(inference)* A reader trained on Rust's Linux arm64 bindings writes `u8` for `char`, which is
    exactly what R1's objection condition counts.
  - So the reader test is the right instrument. Precedent cannot score it.
- **RFC 3681's marked `..`** is still open and unstable (#132162). Its most recent visible comment
  is of 2026-08-03 (https://github.com/rust-lang/rust/issues/132162, verified).
  - A marked zero-the-rest form is still being adopted.
  - I found no language that withdrew one. That rests on what I searched, so it is a question,
    not a finding.
- Panel 178's condition (its spec-warden's) and panel 194's are both process facts in this
  repository. No outside precedent bears on which of them is void.

## Each route, what precedent says

| Q | route | reading | precedent for | precedent against |
|---|---|---|---|---|
| 1 | **A** nothing | approve as the spec's position | C since DR 222 (2004) and C11 fn 51; Rust's typed copies; Zig per `andrewrk`. None of them promises padding | CERT names the leak class, but rates it P2 L3 |
| 1 | **B** zero at construction | object | none | CERT calls memset-then-assign noncompliant; C11 §6.5.2.2p4 with §6.2.6.1p6; the critic measured it |
| 1 | **C** by address | approve only as how Heroes calls Heroes; never as a promise | Nim (size threshold); Windows x64 for 16-byte structs | clang's x86-64 lowering drops padding at any **C** by-value parameter, and a C prototype decides that call |
| 1 | **D** zeroed temporary | approve only as CERT's serialisation: zero the bytes, then copy each field's bytes | CERT's first compliant solution | as `tmp = src` it copies unspecified bytes; as memset then member stores it is CERT's noncompliant example |
| 1 | **E** padding as a field | approve as the remedy | CERT (padding bytes); cgo `-godefs` `_ [N]byte`; zerocopy's first fix; `andrewrk`'s advice | Go #26167, open since 2018: E does not help records C filled. *(a question)* E needs Heroes to emit its own layout rather than the header's struct |
| 1 | **F** (unlisted) refuse a padded record lent as bytes | approve | zerocopy `IntoBytes`, bytemuck `NoUninit` | a correct program that sends a padded struct to `write()` must spell its padding (E) first |
| 2 | **A** nothing | object | Rust `libc` (`Copy`), Swift's C import, cgo | each has a safe layer above it: Rust's `Box`/`Pin`, Swift's `Mutex`, Go's `sync.Mutex`. Heroes has none (CLAUDE.md § 13: no standard library) |
| 2 | **A2** `partial`, field undeclared | object as the answer | GNAT's opaque `char_array` full view; bindgen `opaque_type` | GNAT makes its opaque view **limited**; opacity alone copies (the critic measured this, and so does bindgen's default `Copy`) |
| 2 | **B** a mark | approve, if it refuses **moves** (`=`, by-value parameter, return, array element) and covers **any value holding it** | Ada 83 `limited` and GNAT's `pthread_mutex_t`; C++ `= delete` and non-movable `std::mutex`; D (transitive); Nim `{.error.}`; Swift `~Copyable` | Zig closed *pinned structs* (2026) for language simplicity; Swift's 2026 thread shows a non-copy mark that still moves |
| 2 | **C** from the header | approve by name, seeded from POSIX §2.9.9's list; no C attribute precedent found | Swift reads C++ deleted copy constructors; bindgen reads a header comment or a regex | Swift has no route for a **C** struct (2024, 2026); the transitive case (a struct holding a mutex) needs D's rule |
| 2 | **D** run-time check | approve only as a second layer | Go `copyChecker`; Zig's `AddrLock` | needs a self-address the program owns, and a header's struct has no room for one *(inference)* |
| 3 | **A** R1 stands, reader test | approve | Rust's `c_char = u8` on arm64 Linux is the mistake the test counts | none |
| 3 | **B** withdraw R1 | object | none found | every FFI layer panel 194 read has a zero-the-rest form; RFC 3681 still advancing |
| 3 | **C** shorten | neutral | none | none |
| 3 | **D** 178's condition void | approve (procedural) | none | none |

## argument (≤120 words)

C has told compilers since 2004 (DR 222) that assignment, and so every by-value call, need not copy
padding. clang's x86-64 lowering acts on it, and arm64's does not. No language I read promises
padding. The ones that hand records to C as bytes either refuse padded types (zerocopy, bytemuck) or
name the padding as fields (cgo, CERT). For locks, the mark has 40 years of precedent: Ada 83, then
GNAT's `limited private` `pthread_mutex_t`, C++11, D, Nim, Swift. Rust and Swift teach the second
half: POSIX binds the lock to its **address**, so a mark that refuses copies but allows moves still
breaks. Zig refused the mark for simplicity. Heroes has no wrapper layer to hide the copy, so the
mark belongs in the language.

## condition

My reading changes if any of these turns up:

1. **A C paragraph, C2y included, or a clang guarantee that padding survives assignment or a
   by-value call.** That would revive B, or make A a promise. I found none: one search turned up no
   2025 or 2026 WG14 paper.
2. **Zig's `AddrLock`, or Go's `copyChecker`, applied to a C-defined lock** with no field the
   language owns. That would make D carry Q2 alone, as Zig chose.
3. **A language with a non-copy, non-move mark on C records that withdrew it** as unusable. That
   would turn B to object.
4. **Evidence that Heroes emits the header's struct type rather than a layout of its own.** If it
   does, E costs a second layout, and F alone becomes the route. This is a question for the
   compiler-engineer, unread by this seat.

## prediction

1. **Q1.**
   - **The prediction:** with E applied to the critic's `{char c; double d}` probe, MemorySanitizer
     on `heroes-linux` (x86-64) reads **0** reports at `-O0` to `-O3`. A probe where **C** fills the
     record (`read`), Heroes copies it by value, and then lends it to `write()` reads reports again,
     as Go #26167 predicts.
   - **Instrument and when:** the critic's `msan.sh`, at M-buildable-structs' landing of Q1.
2. **Q2.**
   - **The prediction:** a mark that refuses `=` and by-value parameters but not return or array
     growth will show what Swift's 2026 thread showed. A mutex returned from a function, or held in
     a `[T]` that grows, reaches `pthread_mutex_lock` and `pthread_mutex_unlock` at two different
     addresses.
   - **Instrument and when:** a probe printing the address C receives at each call, on Linux arm64
     and Darwin, when B is built.
3. **Q3.**
   - **The prediction:** fewer than 3 of 10 readers under R1 with K write `u8` for a C `char`, so R1
     stands.
   - **Instrument and when:** the paid reader test this sitting names, scored by the coordinator.

## what I could not run

- I had no shell, so there is no count of group records and no probe.
- The Windows x64 padding of a caller's copy: unrun.
- Facts left **unverified** above:
  - GCC 4.3.0's release date;
  - `andrewrk`'s identity as shown on Ziggit;
  - Zig's `AddrLock` code;
  - whether `SWIFT_NONCOPYABLE` applies to a C struct.
- Panel 194's Odin `#all_or_none` and kernel pin-init sources were not re-read and are not used here.
