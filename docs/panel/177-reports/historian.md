<!-- Written out by the coordinator from the seat's final message, 2026-09-24:
the historian has no write tool by its own definition. The one mechanical
change: the transport escaped `->`, `<` and `>` as HTML entities, and they are
unescaped here. Nothing else is altered. -->

# Panel 177: historian report (advisory seat, no veto)

**Marks.** **verified**: I fetched the primary page, file, paper or commit, and the words are quoted or closely paraphrased from it. **verified-weak**: I saw it only in a search engine's summary or in a secondary source. **UNVERIFIED**: I could not confirm it. **unrun**: it needs a shell or a build.

**What this seat did not do.** I have no shell and no write tool. So I built no compiler and ran none of the reproducers, although the shared brief asks every seat to build one; every Heroes number below comes from the shared brief. I did not use *Heroes of code*, as a route or as evidence. I used panel 176's historian report only as an index: everything taken from it was fetched again, or is marked.

---

## verdict (advisory)

**Question 1, route by route**

- **M, a moved binding refused at check time: approve, on one condition.** The condition is a rule for where branches meet, and one for loops.
  - Every static precedent is flow-sensitive: Cyclone, Swift `consume`, clang-tidy, Clang `-Wconsumed`, Nim, SPARK.
  - Every one either states or shows that a copy made before the consuming call escapes. Swift says so in its own proposal.
- **R, a liveness check at every handle-taking call: object, as stated.**
  - The only R precedent I found is Vulkan's object tracker. Its own documentation says it needs unique handle aliases (S) to work where handles repeat.
  - It is for development only.
  - As stated, R also refuses every correct borrowed handle. That count is the brief's, and I did not re-run it.
- **P, poisoning the name at run time: approve, as the run-time half.**
  - The poison value should be one that no null test reads as "absent". That is MSVC's reason for choosing 0x8123 (verified-weak).
  - No precedent reaches a copy made before the call.
- **S, a serial carried by the value: approve.** Among the value-semantic routes, it is the only one with precedent that also closes 077 through copies. It is also a departure, and I name it:
  - Every S precedent either owns the handle table itself (NT window handles, Erlang, slotmap, Go) or unwraps the handle at the boundary during development only (Vulkan's layers).
  - I found no always-on production system that carries a serial beside a raw C pointer.
- **G, a generation kept in the live set only: object (refuse).** Every generational scheme I found keeps the generation in the handle itself.
- **A, the affine handle: approve as the complete answer.**
  - Cyclone, Hylo, Swift `~Copyable`, SPARK and Rust's `OwnedFd` all shipped it.
  - SPARK ships the exact shape of Part 6's falsifier: a move for every type that contains a pointer. So "nobody has" is true of this repository, not of the world. What remains open is the price.
- **D, the documented limit: object, as the answer on its own** (§1.12).
- **A route the brief does not list, B, a shared box** (.NET `SafeHandle`, Lua's io library). Panel 145 already refused it (design.md:2663). Precedent narrows where its ABI cost falls; see §1d.

**Question 2**

- **A transfer that happens only on success, naming the success value: approve, and it must be checked.**
  - The placements with precedent are one condition per function (SAL `_Success_`, Clang `TRY_ACQUIRE`), or a condition on the parameter keyed to the one result (libkern's `_ON_ZERO` / `_ON_NONZERO`).
  - I found no C function that transfers two parameters under different success values.
- **A transfer names the releaser it hands the life towards (V1c):** unchanged from panel 176.
- **`retains` on a result: approve** (`g_object_ref`, `ns_returns_retained`).
- **`retains` on a parameter: approve, as a deliberate departure.**
  - I found no checked annotation, writable by a binding author, for "this parameter gains a reference". The departure is ROADMAP row 56's refusal of a releaser keyed on the type.
  - Adders that take the object as a parameter can fail (`X509_up_ref`, `kref_get_unless_zero`), so the success clause reaches `retains` too.

## section

The ground is precedent (the historical appendix this seat is responsible for). It bears on three places:
- the Part 6 borrow-checker row, `/Users/joseph/Temp/heroes/heroes-lang/docs/design/design.md:2663`;
- Part 8 wart 20, `:3592`;
- §1.12, for D.

## prediction (falsifiable)

1. Build route P exactly as the shared brief states it. All five rows of the brief's table then abort before C on every platform leg, the two 077 rows included. The reason is that every reproducer re-uses the same binding.
2. Now take each row and change one line: copy the handle into a second binding (`c = a`) before the consuming call, and make the stale use go through the copy. Every row returns to today's number:
   - `u1_static` and `reuse_malloc` exit 0;
   - `read_after_consume` exits 0, with `heap-use-after-free` under `--sanitize`;
   - the two rows that abort today still abort.

This is Stroustrup's `T* q = p; delete p; delete q;` played out on Heroes' own programs. It is falsified if any original row still reaches C under P, or if P refuses any copy variant. **Unrun.**

## condition: what would change my reading

1. **M:** I drop the join-rule condition if someone finds a shipped static use-after-consume check that ignores branches and loops without refusing correct programs or missing a consuming path.
2. **R:** I lift the objection on an always-on production system that checks liveness at every call over raw, reusable handles, without wrapping them, and still catches reuse.
3. **S:** I drop the departure note on an always-on production system that carries a serial beside a raw C pointer across a C ABI, handles inside C structs included. I turn to objecting if such a system turns out to have been withdrawn for its cost.
4. **A:** my reading weakens if a value-semantics language shipped move-on-copy for pointer-bearing types, as SPARK does, and later withdrew it.
5. **Where the success clause goes:** it moves to one condition per parameter if a C function reachable from `examples/` or a core package transfers two parameters under different result values.
6. **Parameter `retains`:** the departure note goes if someone finds a checked parameter annotation, writable by a binding author, that adds a reference.
7. **P's poison value:** that advice goes if MSVC's reason for avoiding NULL (verified-weak) turns out to be misreported.

## argument (108 words)

Precedent I found closes this class three ways:
- the copy itself consumes (Cyclone, Hylo, Swift's `~Copyable`, SPARK for pointer-bearing types, Rust's `OwnedFd`);
- the value carries a generation checked against a table (NT window handles, Erlang, slotmap, Vulkan's layers);
- or every copy shares one box (.NET `SafeHandle`, Lua), which was refused here at panel 145.

Poisoning the name (MSVC `/sdl`, `FREE_AND_NULL`, the kernel's `retain_and_null_ptr`) is simple and common. No source claims it reaches an earlier copy, and Stroustrup shows the copy defeating it. So M plus P has precedent and leaves wart 20's half open; S or A closes it. A success condition has precedent where it is checked; CPython's unchecked one was replaced.

---

## precedents

### 1a. Moved-from values (static checks)

**Rust, across FFI. Verified.**
- Raw pointers implement `Copy` ([pointer](https://doc.rust-lang.org/std/primitive.pointer.html)).
- E0382 is *"A variable was used after its contents have been moved elsewhere"*. `Copy` types are *"trivial to duplicate"*, so they never raise it ([E0382](https://doc.rust-lang.org/error_codes/E0382.html)).
- So Rust's move check says nothing about a C handle held as a raw pointer. The Nomicon: *"Foreign libraries often hand off ownership of resources to the calling code. When this occurs, we must use Rust's destructors"* ([FFI](https://doc.rust-lang.org/nomicon/ffi.html)).
- **What happened.** RFC 3128, I/O safety (start date 2021-05-24), describes 077's shape for file descriptors: close a file, open an unrelated one, and *"Further uses of `raw_fd` … could lead to it accidentally aliasing other otherwise encapsulated `File` instances"* ([RFC 3128](https://rust-lang.github.io/rfcs/3128-io-safety.html)).
- The fix was `OwnedFd`. It implements neither `Copy` nor `Clone`, and *"can be used in FFI in places where a file descriptor is passed as a consumed argument"* ([OwnedFd](https://doc.rust-lang.org/std/os/fd/struct.OwnedFd.html)).
- It shipped in Rust 1.63, 2022-08-11 (verified-weak: [announcement URL](https://blog.rust-lang.org/2022/08/11/Rust-1.63.0.html), [LWN](https://lwn.net/Articles/904486/)).
- **The lesson:** a language with a borrow checker still had the reused-handle bug for as long as the foreign handle was a copyable integer.

**clang-tidy `bugprone-use-after-move`. Verified.**
- Static, within one function, *"flow-sensitive but not path-sensitive"*.
- *"A warning is only emitted if the use can be reached from the move."*
- The page says nothing about copies made before the move ([doc](https://clang.llvm.org/extra/clang-tidy/checks/bugprone/use-after-move.html)).

**Clang consumed typestate (`consumable`, `callable_when`, `param_typestate`, …; `-Wconsumed`). Verified.**
- Static. Its diagnostics include *"state of variable '…' must match at the entry and exit of loop"* ([DiagnosticsReference](https://clang.llvm.org/docs/DiagnosticsReference.html)).
- **What happened.** The Clang 3.5 documentation says *"the implementation for these annotations is currently in development and are subject to change"* ([3.5](https://releases.llvm.org/3.5.0/tools/clang/docs/AttributeReference.html)). Today's page still says so ([current](https://clang.llvm.org/docs/AttributeReference.html)).
- That it is off by default is verified-weak.

**Swift `consume` (SE-0366). Verified.**
- Status *"Implemented (Swift 5.9)"*. Static and flow-sensitive; applies to local `let`/`var` and to parameters.
- Its own limit, in its own words: *"consume operates on bindings, not values. If we declare a constant `x` and another local constant `other` with the same value, we can still use `other` after we consume the value from `x`"* ([SE-0366](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0366-move-function.md)).
- That is wart 20's half, stated as intended behaviour. It is safe in Swift because `other` keeps the value alive; it would not be safe for a C address.

**Swift `~Copyable` (SE-0390). Verified.**
- *"Implemented (Swift 5.9)"*. The motivating example is a C handle: `struct FileDescriptor: ~Copyable { private var fd: Int32 … deinit { close(fd) } }`.
- `discard self` hands back the raw `Int32` without running `deinit` ([SE-0390](https://github.com/swiftlang/swift-evolution/blob/main/proposals/0390-noncopyable-structs-and-enums.md)).
- This is route A, applied to the types the author chooses.

**Cyclone unique pointers. Verified from the paper's pages 3 and 4** ([PDF](https://homes.cs.washington.edu/~djg/papers/cyc_mm_experience.pdf); ISMM 2004 venue verified-weak, [ACM](https://dl.acm.org/doi/10.1145/1029873.1029883)).
- *"an intraprocedural, flow-sensitive, path-insensitive analysis to track when a unique pointer becomes consumed, in which case the analysis rejects a subsequent attempt to use the pointer."*
- The copy half is route A: *"a copy of a unique pointer (e.g., in an assignment or function call) is treated as consuming the pointer."*
- The rule where branches meet: *"At join points in the control-flow graph, our analysis conservatively considers a value consumed if there is an incoming path on which it is consumed."*
- On P: *"In most systems, reading a unique pointer is treated as a destructive operation that overwrites the original copy with NULL."* Cyclone, whose pointer types may exclude NULL, used an explicit swap `:=:` instead.
- **What happened:** *"Cyclone is no longer supported … (Several of Cyclone's ideas have made their way into Rust.)"* ([site](https://cyclone.thelanguage.org/)).

**Hylo `sink`. Verified.**
- *"the argument becomes inaccessible in the caller after the callee is invoked"*, with *"error: v was consumed in the previous line"* and the repair *"pass v.copy()"* ([functions](https://docs.hylo-lang.org/language-tour/functions-and-methods)).
- *"assigning into a `var` binding consumes the source of the assignment"* ([bindings](https://docs.hylo-lang.org/language-tour/bindings)).
- The value-semantics language in Heroes' ancestry closes the class by making every copy explicit.

**Nim `sink` / `=wasMoved`. Verified.**
- A value moves on its last read; otherwise it is copied with `=dup`.
- The analysis is *"a static control flow analysis"*, *"limited and only concerned with local variables"*.
- `=wasMoved` resets the moved-from location, which is a form of P. But no later read ever sees the reset, because a read that is not the last one gets a copy instead ([destructors](https://nim-lang.org/docs/destructors.html)).

**SPARK (Ada). Verified.**
- Ownership arrived with pointers, *"As in Rust"* ([UG](https://docs.adacore.com/spark2014-docs/html/ug/en/source/access.html)). A preview shipped in GNAT+SPARK Community 2019 (Claire Dross, 2019-06-06, [blog](https://www.adacore.com/blog/using-pointers-in-spark)). SPARK 20: *"SPARK now supports pointers … through the restrictions provided by ownership rules"* ([notes](https://docs.adacore.com/live/wave/spark2014-release-notes/html/spark2014_release_note/release_notes_20.html)).
- A move happens on an assignment whose target is *"of a type containing subcomponents of a named access-to-variable type"* ([LRM 3.10](https://docs.adacore.com/spark2014-docs/html/lrm/declarations-and-types.html)). So a record that holds a pointer moves, and any other record copies.
- An annotation extends this to private types: *"the package `Text_IO` defines a limited type `File_Descriptor` and uses ownership annotations to force GNATprove to verify that all file descriptors are closed"* ([annotations](https://docs.adacore.com/spark2014-docs/html/ug/en/appendix/additional_annotate_pragmas.html)).
- The release that added the private-type annotation is UNVERIFIED.

### 1b. Generation-counted handles (dynamic checks; the generation lives in the value)

**slotmap. Verified.**
- Each slot is *"a `(value, version)` tuple. After insertion the returned key also contains a version"*.
- *"Only when the stored version and version in a key match is a key valid."*
- After 2^31 reuses of one slot, *"the version wraps around and such a spurious reference could potentially occur"* ([docs.rs](https://docs.rs/slotmap/latest/slotmap/)).

**Catherine West, RustConf 2018 closing keynote, 2018-09-14. Verified.** `GenerationalIndex { index, generation }`; a stale lookup fails because *"the generation must match"* ([post](https://kyren.github.io/2018/09/14/rustconf-talk.html)).

**Andre Weissflog, 2018-06-17. Verified.**
- A 16-bit handle of *"10 bits index + 6 bit 'unique pattern'"*, compared at every conversion from handle to pointer.
- The November 2018 update disables a slot whose counter overflows.
- It comes with a discipline: *"pointers should never be stored anywhere"* ([post](https://floooh.github.io/2018/06/17/handles-vs-pointers.html)).

**Vale generational references. Verified.**
- *"a pointer with a 'remembered generation' number next to it"*, checked at every dereference.
- Measured at *"10.84% overhead"* against unsafe code, and reference counting at 25.29% (post updated 2023-07-09, [post](https://verdagon.dev/blog/generational-references)).
- C memory is kept separate rather than generation-checked, as far as the page says: *"Separate the safe memory from the unsafe memory (such as the memory managed by C)"* ([page](https://vale.dev/memory-safe)).
- **What happened:** *"Vale is archived and no longer being worked on"* ([repo](https://github.com/ValeLang/Vale)).

**Windows window handles (HWND). Verified.**
- Windows NT *"used the top bits as a 'uniquifier'"*: table slot 0x0124 first gives 0x00010124, then 0x00020124.
- The reason was *"programs that send messages to windows that have already been destroyed"*.
- 16-bit programs got no uniquifier (Raymond Chen, 2007-07-17, [post](https://devblogs.microsoft.com/oldnewthing/20070717-00?p=25983)).

**Windows kernel handles are reused. Verified.**
- *"handle recycling means that any invalid handle can suddenly become valid again (but refer to an unrelated object)"* (Chen, 2012-09-26, [post](https://devblogs.microsoft.com/oldnewthing/20120926-00/?p=6493)).
- *"the operating system could mitigate this by handling out random values for handles, but in general the operating system does not always do this for performance reasons"* (MSDN blog, 2009-09-08, [post](https://learn.microsoft.com/en-us/archive/blogs/mattn/tracking-handle-misuse-using-application-verifier-and-windbg)).
- Application Verifier's Handles check catching calls with closed handles: verified-weak. How it does so when values are reused: UNVERIFIED.

**Erlang process and port identifiers. Verified.**
- Built *"by writing the index to the least significant set of bits, and the 'wrapped counter' to the most significant"*.
- *"If previously used identifiers are reused too quick, identifiers originating from terminated processes will refer to newly created processes, and mixups will occur"* ([PTables.md](https://github.com/erlang/otp/blob/master/erts/emulator/internal_doc/PTables.md)).
- The exact `badarg` wording for a closed port is UNVERIFIED.

**Go `runtime/cgo.Handle`, Go 1.17** ([notes](https://go.dev/doc/go1.17)). **Verified.**
- New handles come from `h := handleIdx.Add(1)`, with `panic("runtime/cgo: ran out of handle space")` on wrap.
- A deleted handle gives `panic("runtime/cgo: misuse of an invalid Handle")` ([source](https://go.dev/src/runtime/cgo/handle.go)).
- Numbers are never reused, so a stale handle is always detected. It runs in the opposite direction from Heroes: Go values handed to C.

**Vulkan non-dispatchable handles. Verified.**
- The spec text quoted in [Vulkan-Docs #1349](https://github.com/KhronosGroup/Vulkan-Docs/issues/1349) (2020-08-15): *"a 64-bit integer type whose meaning is implementation-dependent … Objects of a non-dispatchable type may not have unique handle values"*. Invalid use is *undefined behavior* ([guide](https://docs.vulkan.org/guide/latest/validation_overview.html)).
- The validation layer is R plus S:
  - *"As objects are used the layer verifies they exist in the data structure and output errors for unknown objects"*;
  - and because *"The Vulkan specification allows objects to have non-unique handles. This makes tracking object lifetimes difficult"*, handle wrapping, on *"as it is by default"*, aliases every object *"with a unique object representation"* ([README](https://android.googlesource.com/platform/external/vulkan-validation-layers/+/HEAD/layers/README.md), [handle_wrapping.md](https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/main/docs/handle_wrapping.md)).
- **What happened:** it stays a development tool. *"Applications should also never ship the Validation Layers … as they noticeably reduce performance"* ([guide](https://docs.vulkan.org/guide/latest/validation_overview.html)).
- UNVERIFIED: how the unique ids are issued, and how the layer unwraps handles nested inside structures.

### 1c. Invalidating the name at run time (route P), and what each gives up

**C++ `delete`. Verified.**
- *"C++ explicitly allows an implementation of delete to zero out an lvalue operand, and I had hoped that implementations would do that, but that idea doesn't seem to have become popular with implementers."*
- What it cannot reach: `delete p+1;`, and *"two pointers to an object: `T* p = new T; T* q = p; delete p; delete q; // ouch!`"* (Stroustrup, [FAQ](https://www.stroustrup.com/bs_faq2.html)).

**MSVC `/sdl`. Verified.**
- *"Does limited pointer sanitization. In expressions that don't involve dereferences and in types that have no user-defined destructor, pointer references are set to a non-valid address after a call to delete."*
- *"By default, /sdl is off"* ([Learn](https://learn.microsoft.com/en-us/cpp/build/reference/sdl-enable-additional-security-checks?view=msvc-170)).
- In Visual Studio 11 Beta the value was 0x8123. `delete [] arr[0]` twice went unsanitized because *"the expression arr[0] passed into the delete call is dereference"* (2012-05-02, [post](https://reversingonwindows.blogspot.com/2012/05/example-when-sdl-flag-doesnt-work-in.html)).
- Why not NULL: an existing NULL check *"could fortuitously hide a genuine memory safety issue"* (Tim Burrell, Microsoft, 2012-04-24, [post](https://www.microsoft.com/security/blog/2012/04/24/guarding-against-re-use-of-stale-object-references/)). Verified-weak: the page would not load for me.

**C conventions. Verified.**
- Git: `#define FREE_AND_NULL(p) do { free(p); (p) = NULL; } while (0)` ([git-compat-util.h](https://github.com/git/git/blob/master/git-compat-util.h)).
- GLib `g_clear_pointer`, since 2.34 ([docs](https://docs.gtk.org/glib/func.clear_pointer.html)).
- CERT MEM01-C: *"set pointers to `NULL` after they are freed"* ([CERT](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/recommendations/memory-management-mem/mem01-c/)).
- None of the three says anything about a second copy of the pointer.

**Linux kernel, 2025. Verified.**
- `retain_and_null_ptr()`: *"Only for situations where an allocation is handed in to another function and consumed by that function on success"*, used as `if (!ret) retain_and_null_ptr(f);`, and *"After retain_and_null_ptr(f) the variable f is NULL and cannot be dereferenced anymore"* ([cleanup.h](https://github.com/torvalds/linux/blob/master/include/linux/cleanup.h)).
- Commit 092d00ead733, Thomas Gleixner, 2025-03-19 ([commit](https://lkml.iu.edu/hypermail/linux/kernel/2504.1/03755.html)).
- It is P and a success condition in one macro, written at the call site.

**C++ `std::unique_ptr`. Verified.**
- The move constructor *"stores the null pointer in `u`"* ([cppreference](https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr)).
- That a pointer taken earlier with `get()` is left untouched is my reading of the page, not a quote from it.

**Reaching every copy costs tracking every pointer.**
- DangNull (NDSS 2015) *"automatically nullifies all pointers when the target object is freed"* ([NDSS](https://www.ndss-symposium.org/ndss2015/ndss-2015-programme/preventing-use-after-free-dangling-pointers-nullification/)). Verified. Its overheads, 4.8% on JavaScript and 53.1% on rendering in Chromium, are verified-weak.
- Chromium's `raw_ptr` quarantines freed memory *"as long as any dangling `raw_ptr<T>` pointing to it exists"*. It covers only memory from PartitionAlloc (Chromium's allocator), and not *"Pointers in local variables and function parameters and return values"* ([raw_ptr.md](https://chromium.googlesource.com/chromium/src/+/main/base/memory/raw_ptr.md)). Verified. Its launch in Chrome 102 is verified-weak.

**Debug allocators poison the memory, not the name, and avoid reuse by not reusing. Verified.**
- MSVC's `_CRTDBG_DELAY_FREE_MEM_DF` keeps freed blocks filled with 0xDD *"To check that freed memory isn't written to"* ([Learn](https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-debug-heap-details?view=msvc-170)).
- Zig's DebugAllocator *"Never reuses memory addresses"* ([commit](https://github.com/ziglang/zig/commit/cd99ab32294a3c22f09615c93d611593a2887cc3)).
- Heroes does not choose C's allocator, which is why 077 needs S.

### 1d. One route, for foreign handles only, without a borrow checker for the program's own values

Yes. Each of these took one route:
- **Swift:** A, static, for types the author marks.
- **SPARK:** A with flow analysis. Its rules are a borrow checker, but one scoped to pointer-bearing and annotated types, which is exactly the scope Part 6's falsifier names.
- **Vulkan's layers:** S plus R, development only.
- **Go:** handle numbers that are never reused.
- **The Linux kernel:** P at call sites.
- **.NET `SafeHandle`: a shared box, in production. Verified.**
  - It exists because *"Windows aggressively recycles handles, a handle could be recycled and point to another resource … This is known as a recycle attack"*.
  - *"Platform invoke operations automatically increment the reference count of handles encapsulated by a SafeHandle and decrement them upon completion"* ([API](https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.safehandle?view=net-9.0)). The page's version list begins at .NET Framework 2.0.
  - It stops at C structs: *"with structs, the use of SafeHandle is not interchangeable with IntPtr"*, and source-generated P/Invoke rejects such a struct with SYSLIB10 (Matt Weber, 2023-02-07, [post](https://badecho.com/index.php/2023/02/07/safehandles-in-structs/)). Verified from a practitioner's post, not from Microsoft.
- **Lua's io library: a shared box, poisoned. Verified.**
  - `aux_close` does `p->closef = NULL;  /* mark stream as closed */`, and every later use goes through `tofile`, which raises *"attempt to use a closed file"* ([liolib.c](https://www.lua.org/source/5.4/liolib.c.html)).
  - Every alias sees the poison because the box is shared.

**On route B, the shared box.** Panel 145 refused it with *"a refcounted box breaks the ABI"*. `SafeHandle` suggests the break is confined:
- it keeps each C function's signature by unwrapping at every call;
- it fails at a handle inside a C struct, the same place S fails.

What B would change is Heroes' value semantics, because copies would share one cell. S does not change that.

### 2a. Conditional transfer: where the success condition lives

| system | conditional? | where the condition lives | checked? |
|---|---|---|---|
| SAL `_Success_(expr)` | yes | on the **function**, one per function; per-parameter `_Always_` / `_On_failure_` override it | `/analyze` |
| Clang thread-safety `TRY_ACQUIRE(bool, …)` | yes | on the **function**, as the first argument | `-Wthread-safety` |
| libkern `*_RETAINED_ON_ZERO` / `_ON_NONZERO` | yes | on the **parameter**, keyed to the result, with a default taken from the result's type | analyzer |
| `CF_CONSUMED` / `ns_consumed` / `os_consumed` | no | unconditional | analyzer, ARC |
| Clang `ownership_takes` / `ownership_holds` | no | no condition documented | analyzer |
| GI `(transfer …)` | no | nowhere | no |
| OpenSSL `set0` / `add0` | yes in the code, silent in the pages | only in the name | no |
| BoringSSL | "typically only on success" | each function's prose | no |
| Linux kernel | yes | at the **call site**, and in prose | the macro NULLs the name |
| CPython `PyModule_AddObject` | yes | prose | no; replaced |

- **SAL. Verified.**
  - Under `_Success_(expr)`, *"each annotation (`anno`) on the function and in post-condition behaves as if it were coded as `_When_(expr, anno)`. The `_Success_` annotation may be used only on a function, not on its parameters or return type. There can be at most one `_Success_` annotation on a function."*
  - *"`NTSTATUS` and `HRESULT` already have a `_Success_` annotation built into them"* ([Learn](https://learn.microsoft.com/en-us/cpp/code-quality/annotating-function-behavior?view=msvc-170)).
  - `_COM_Outptr_` *"carries an `_On_failure_` post-condition that the returned pointer is null"* ([Learn](https://learn.microsoft.com/en-us/cpp/code-quality/annotating-function-parameters-and-return-values?view=msvc-170)).
  - SAL's consume mark is `_Post_ptr_invalid_`, commented `// e.g. void free( _Post_ptr_invalid_ void* pv );` ([sal.h](https://github.com/tpn/winsdk-10/blob/master/Include/10.0.16299.0/shared/sal.h)).
  - That `_Post_ptr_invalid_` then holds only on success unless wrapped in `_Always_` is my reading of the rule, not a documented example.
- **Clang thread-safety analysis. Verified.** *"The first argument must be true or false, to specify which return value indicates success"* ([TSA](https://clang.llvm.org/docs/ThreadSafetyAnalysis.html)).
- **libkern (Apple's kernel C++ runtime), in the Clang analyzer. Verified.**
  - *"a retained object is written into if and only if the function returns a zero value"*.
  - The default: *"For functions returning `OSReturn` or `IOReturn` … success is defined as having an output of zero … For all others, success is non-zero"* ([Annotations](https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html)).
  - The attributes appear in Clang 8's attribute reference and not in Clang 7's ([8](https://releases.llvm.org/8.0.0/tools/clang/docs/AttributeReference.html), [7](https://releases.llvm.org/7.0.0/tools/clang/docs/AttributeReference.html)). LLVM 8.0.0 came out 2019-03-20 (verified-weak, [announce](https://lists.llvm.org/pipermail/llvm-announce/2019-March/000082.html)).
- **Consuming marks are unconditional. Verified.**
  - `cf_consumed`: *"a `release` message is implicitly sent to the parameter upon completion of the call"* ([Annotations](https://clang.llvm.org/docs/analyzer/user-docs/Annotations.html)).
  - ARC: *"ARC releases the argument at the end of the function"* ([ARC](https://clang.llvm.org/docs/AutomaticReferenceCounting.html)).
- **GI. Verified.**
  - The annotations page says nothing about failure ([GI](https://gi.readthedocs.io/en/latest/annotations/giannotations.html)).
  - GLib: *"If a `GError` is reported, out parameters are not guaranteed to be set to any defined value"*, and the function *"did not complete whatever it was supposed to do"* ([GLib](https://docs.gtk.org/glib/error-reporting.html)).
  - Reading that as "a `(transfer full)` in-parameter was not taken" is my inference.
- **OpenSSL. Verified.**
  - The page says the values *"should not be freed by the caller after this function has been called"*, and that the function returns 1 on success or 0 on failure ([pod](https://raw.githubusercontent.com/openssl/openssl/master/doc/man3/RSA_get0_key.pod)).
  - The code returns 0 before taking `n`, `e` or `d` ([rsa_lib.c](https://raw.githubusercontent.com/openssl/openssl/master/crypto/rsa/rsa_lib.c)). Read literally, the page tells a caller whose call failed to leak.
  - `EVP_PKEY_assign_*` has no failure clause either ([pod](https://raw.githubusercontent.com/openssl/openssl/master/doc/man3/EVP_PKEY_set1_RSA.pod)).
  - `SSL_set0_rbio` is `void` and *"cannot fail"* ([pod](https://raw.githubusercontent.com/openssl/openssl/master/doc/man3/SSL_set_bio.pod)).
- **BoringSSL. Verified.** *"functions in BoringSSL typically only transfer on success"* ([conventions](https://boringssl.googlesource.com/boringssl/+/HEAD/API-CONVENTIONS.md)).
- **Linux kernel. Verified.**
  - kref rule 1: *"you must increment the refcount with `kref_get()` before passing it off"*. The page's example drops that reference again when `kthread_run` fails ([kref](https://docs.kernel.org/core-api/kref.html)).
  - `device_register`: *"Never directly free dev after calling this function, even if it returned an error! Always use put_device() to give up the reference initialized in this function instead"* ([driver API](https://docs.kernel.org/driver-api/infrastructure.html)). The releaser changes from `kfree` to `put_device` whether or not the call succeeds.
  - A series converting callers' `kfree(dev)` to `put_device()` was still being posted on 2026-09-04 (verified-weak, [mirror](https://ratatoskr.run/lkml/2026/09/17515932/t)).
- **CPython. Verified.**
  - `PyModule_AddObject` steals *"on success (if it returns `0`)"*; *"it is easy to introduce reference leaks by misusing"* it; it has been soft-deprecated since 3.13.
  - Its replacements are unconditional: `PyModule_AddObjectRef` (3.10) never steals, and `PyModule_Add` (3.13) steals *"(even on error)"* ([docs](https://docs.python.org/3/c-api/module.html)).
  - bpo-26871, opened 2016-04-27 by Serhiy Storchaka: *"only in few places in the stdlib a reference is decrefed explicitly after PyModule_AddObject() failure"*. It was closed in 2021 by adding `PyModule_AddObjectRef` ([bpo](https://bugs.python.org/issue26871)).
  - **This is the one conditional transfer I found with a recorded outcome.** Its condition lived in prose and nothing checked it; its designers replaced it with two unconditional functions.

### 2b. A reference added through a result or a parameter

**On a result. Verified.**
- ARC `ns_returns_retained` (*"retains the value at the point of evaluation of the return statement"*).
- GObject `g_object_ref` returns *"The same `object`"*, and *"The caller of the method takes ownership of the returned data"* ([docs](https://docs.gtk.org/gobject/method.Object.ref.html)). So the brief's `(transfer full)` premise holds.
- Swift `SWIFT_RETURNS_RETAINED` ([swift.org](https://www.swift.org/documentation/cxx-interop/)).

**On a parameter.** The shape is `int X509_up_ref(X509 *a)`, which *"returns 1 for success and 0 for failure"* ([pod](https://raw.githubusercontent.com/openssl/openssl/master/doc/man3/X509_new.pod)). Verified.
- **CPython's `refcounts.dat`** has rows per argument, such as `Py_INCREF:PyObject*:o:+1:`. The file itself says the argument information *"is confusing! Much more useful would be to indicate whether the function 'steals' a reference"* ([file](https://raw.githubusercontent.com/python/cpython/main/Doc/data/refcounts.dat)). Verified. Whether any checker reads the argument rows is UNVERIFIED.
- **Clang's retain-count checker** gives CoreFoundation functions whose names start or end with "retain" an `IncRef` effect on argument 0. It does this by name, with `AllowAnnotations = false` ([RetainSummaryManager.cpp](https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/Analysis/RetainSummaryManager.cpp)). Verified.
- **Swift** names the retain function on the type: `void retainImage(Image *img);` with `SWIFT_SHARED_REFERENCE(retainImage, releaseImage)`. Verified.
- **The kernel's `kref_get_unless_zero`** is `__must_check` and returns *"non-zero if the increment succeeded"* ([kref.h](https://raw.githubusercontent.com/torvalds/linux/master/include/linux/kref.h)). Verified.
- **What happened:** OpenSSL's own `X509_chain_up_ref` ignored `X509_up_ref`'s result (issue #8592, 2019-03-27, [issue](https://github.com/openssl/openssl/issues/8592)). Verified.
- **Suppose `retains` is written unconditionally on an adder that can fail.** Then the runtime's set of live handles would record a reference that C never added, so a later release would pass the set while C's own count is already zero. This is an inference, **unrun**.

---

## Named as unverified or unrun

- **UNVERIFIED:**
  - SPARK: the release that added the private-type ownership annotation.
  - Vulkan: how the layer issues its unique ids, and how it unwraps handles nested inside structures.
  - Application Verifier: how it catches closed handles when values are reused.
  - Erlang: the exact `badarg` text for a closed port.
  - Whether any checker reads `refcounts.dat`'s argument rows.
- **verified-weak:**
  - MSVC's reason for 0x8123 instead of NULL.
  - DangNull's overheads; MiraclePtr in Chrome 102.
  - The LLVM 8 and Rust 1.63 release dates.
  - SAL's `_Frees_ptr_` expansion.
  - `-Wconsumed` being off by default.
  - The kernel patch series of 2026-09-04.
  - The ISMM 2004 venue for the Cyclone paper.
- **Carried from panel 176, not re-run:** json-c's success value 0 and cJSON's `true`.
- **Unrun:** every Heroes reproducer, the prediction, and the unconditional-`retains` inference.

## Repository files read

- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/177-briefs/historian.md
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/177-briefs/00-shared.md
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/177-briefs/u1_static.hero
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/177-briefs/reuse_malloc.hero
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/177-briefs/read_after_consume.hero
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/177-briefs/helper_consume_then_use.hero
- /Users/joseph/Temp/heroes/heroes-lang/docs/design/design.md (lines 2655 to 2674 and 3585 to 3624)
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/176-a-transfer-names-where-the-life-goes-and-a-reference-joins-the-life-it-finds.md (lines 1 to 120)
- /Users/joseph/Temp/heroes/heroes-lang/docs/panel/176-reports/historian.md

Sources: every URL is given inline beside the claim it supports.
