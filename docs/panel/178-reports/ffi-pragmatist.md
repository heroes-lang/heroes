<!-- Written out by the coordinator from the seat's final message, 2026-09-24:
the harness refused the seat's write to REPORT.md. One mechanical change: this
comment. The text below is the seat's, unchanged. The probes it names are in
the seat's scratchpad directory; the ones the sitting's findings rest on are
copied beside this file, in `ffi-pragmatist-work/`. -->

# Panel 178 — ffi-pragmatist

**REPORT.md was not written.** The harness refused the write ("Subagents should return findings as text, not write report files"), so the full report is this message. Every probe and every output file below is under `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/07a257f7-6606-4944-b558-0be3c97d5c11/scratchpad/178-ffi-pragmatist/`. Nothing was written outside that directory. I did one read-only `git -C <trunk> show 57679005:selfhost/emit/container.hero` to diff my prototype against the original.

**One row:** R1 **approve, amended** · R0 **approve, but only one of R0 and R1** · R2 **object** · A **object** · Z1 **object** · Z2 **approve** · T **approve on three conditions; veto if its terminating zero is dropped** · L **approve, lands whatever else is chosen** · S **object as the answer** (it is safe, and stays the fallback) · D **object**.
- Sections: design.md §1.11, §1.12, §4.19.
- **No veto on the proposal as written.**
- Prediction: a Z1 prototype leaves all three measured silent zero-value failures at `check` 0.
- What would change my verdict: a Z1 that also governs `partial` and whose truth can be checked per platform.

Costs are `heroes measure`'s `maximum` row (cl100k_base, **a lower bound**), measured on the Darwin sources.

| route | verdict | section | cost | what it rests on |
|---|---|---|---|---|
| **R1** | approve, amended | §1.11, §4.19 | `utsname` **4067 → 198**; `statfs` **3313 → 172**; `sockaddr_un` with L **985 → 654** | Its C (memset the cell, then store the members) compiled with 0 warnings on 3 legs at `-O0` and `-O2`. Every real call returned 0. |
| **R0** | approve, but only one of R0 and R1 | §1.11 | 194 / 170 / 622 (with T) | Same C as R1. Needs 091's lowering, which I prototyped at +20/−1 lines. |
| **R2** | object | §1.12 through the thesis (§4.9 is another seat's) | about R1 minus `rest: zero` | A forgotten `ai_socktype` gives **2 results where 1 is right on Darwin** and **6 where 2 is right on Linux**, at exit 0. |
| **A** | object | §4.19, §1.11 | 244 / 206 / 679 | It writes the platform's N into every construction. A non-zero `x` needs a GNU range, which is **6 errors** under `-std=c11 -pedantic-errors`. |
| **Z1** | object | §4.19, §1.12 | +1 word per record, and it refuses zero-then-init | It closes **0 of 3** measured silent zero-value failures: all three are reachable through `partial` today. |
| **Z2** | approve | §1.12 | 0 | All-zero bytes are a defined value (NULL, +0.0, false) of every field type a group record holds, on 3 legs. |
| **T** | approve on conditions | §1.12, §4.20 | `sockaddr_un` **618** | It must copy bits into `i8`, refuse a zero byte inside the text, and keep the terminating zero. |
| **L** | approve, lands regardless | §1.12, spec § 5 line 121 | +20/−1 lines in `selfhost/emit/container.hero`; the compiler's 676 tests pass | It is the only route for bytes that are not text. |
| **S** | object as the answer | §1.11 | 4067 / 3313 / 985. One `utsname` emits **5555 lines of C with 1280 temporaries**. | Safe and working on 3 legs; stays the fallback. |
| **D** | object | §1.11 | — | Gives the C boundary nothing that R1 plus Z2 do not already give. |

---

- **`verdict`**: approve R1 (amended), R0 (only one of R0 and R1), Z2, T (on conditions) and L. Object to R2, A, Z1, S as the answer, and D. No veto on the proposal as written. The two conditional vetoes are under `condition`.

- **`section`**:
  - **§1.11**: *"FFI ergonomics rank alongside comprehension, not below it. When a choice makes bindings harder, that is a serious cost."*
  - **§4.19**: *"a wrong type in an `extern` is a compile error"*. This is clang checking every binding against the real header.
  - **§1.12**: *"must not segfault and must not corrupt memory … any C library must be bindable."*
  - **design.md says nothing on padding bytes, and nothing on whether all zeros is a valid value of a library's type.** I grepped §4.19, §4.20 and Part 6. On those two questions this report rests on measurement, not on a section.

- **`experiment`**, in short (details in §1 to §6 below):
  - Defect 091's lowering, prototyped in my copy of the compiler.
  - Every route's C for `utsname`, `sockaddr_un`, `sockaddr_storage`, `addrinfo`, `statfs`/`statvfs` and `pthread_mutex_t`, each handed to its real call.
  - Route S written in Heroes for all six and run on Darwin arm64, Linux arm64 and Linux x86-64.
  - Padding measured with MemorySanitizer, an instrument whose control does fail at `-O2`.
  - Zero-validity measured on three legs, taken from the census headers themselves.
  - **clang accepted every route's C** under the emitter's own flags (`selfhost/cli/flags.hero:91-110`), 0 errors and 0 warnings. The one exception is A's range designator under `-pedantic-errors`.
  - **Windows: unrun for everything.** The Heroes source for R1, R0, A and T is **unrun**: today's compiler refuses it, and §6 gives the diagnostics.

- **`argument`** (about 115 words): Every route's C is what a C binding author writes: memset, member stores, memcpy, a guarded subscript. clang accepted all of it against the real headers on three legs, with no ABI change and no marshalling. So what decides is the author's cost, and R1, R0 and T cut a `utsname` from 4067 tokens to about 198. Z1 is the wrong guard. Whether zero is valid is the library's rule, and clang cannot see it. Two Linux legs with the same glibc disagree on it (spinlock). Darwin's manual page is silent about it. `partial` already reaches every zero Z1 would refuse. And Z1 refuses the correct pattern, zero then init. A writes the platform's N into every construction. L repairs a defect and lands regardless.

- **`prediction`**: a Z1 prototype, however it is spelled, leaves all three of these programs at `heroes check` 0, because `partial` zeroes every member it does not declare (the C11 designated-initialiser rule; shared brief measurement 4). **Falsified if the prototype refuses any of the three.**
  - `p/s/sha_S.hero`: a zero `SHA256_CTX`. Update and Final both return 1, and the digest's first byte comes out 0 where 186 is right, on 3 legs.
  - `p/s/flock_S.hero`: a zero `struct flock`. `fcntl(F_SETLK)` returns 0 **and takes a read lock** on both Linux legs.
  - `p/s/mutex_S_darwin.hero`: a zero mutex. Lock returns 22.

- **`condition`**:
  - **Z1.** I withdraw the objection if the mark also governs `partial`, so the three programs above become compile errors, while zero-then-init stays legal (`pthread_mutex_init` on a zeroed cell, the second half of `mutex_S_darwin.hero`). Its truth would also have to be checked per platform against something clang can read, not asserted by the author.
  - **R1's padding sentence.** Keep *"and so is every byte between fields"* only if a harness case runs a `rest: zero` record through `write()` under MSan on the Linux leg. Otherwise drop the sentence.
  - **A.** I move to approve if the header-length route lands **and** A gets a spelling that needs no length. Or if someone measures a non-FFI need for non-zero repetition.
  - **Veto 1, on T:** if the terminating-zero clause is dropped, a C `strlen` over the field reads past it (§1.12).
  - **Veto 2, on R1 and R0:** any emission that fills an uninitialised cell by member stores alone. That is §3's CONTROL shape, which MSan reports at `-O0` through `-O3` on both Linux legs.

---

## 1. Defect 091's lowering (route L), prototyped and run

**The change.** It is in `selfhost/emit/container.hero`, function `write_element`, in the index step: +20/−1 lines. When the step's type is `.fixed x`:
- it appends the read path's own bounds guard to the lvalue: `[((uint64_t)(i) >= UINT64_C(N) ? (hero_panic("index out of range for a fixed array"), (int64_t)0) : (i))]`;
- it stores if this is the last step, and otherwise descends into `x.element`;
- it unshares nothing, because a C array member is an lvalue in place.

**Build and tests.**
- `./heroes build selfhost/main.hero -o heroes-next`: real 88.06 s, from the given seed. I do not know what optimisation level that seed was built at.
- `./heroes-next test selfhost/main.hero`: **676 tests, all passed** (real 71.50).
- `--emit-c` of `selfhost/main.hero` gives `p/seed-next.c` (26,412,053 bytes). The Linux legs build their compiler from it: 4 s on arm64, 7 s on x86-64.
- **The net (`tests/harness`) is unrun with the prototype.**

**What it does.**
- `elem_min.hero` now prints **72 and exits 0**; before the prototype it exited 134.
- `p/shapes.hero` tries the neighbouring shapes: a fixed array inside a nested record (`o.inn.b[2] @ 200`), a field of a record inside a fixed array (`o.pts[1].y @ 40`), and a whole element (`o.pts[0] @ Pt(...)`). It prints 200, 40 and 10.
- An out-of-range index (`o.inn.b[3]`) gives `panic: index out of range for a fixed array`, **exit 134**.

## 2. Route S in Heroes on three legs (`p/s/`)

Outputs are in `p/linux-arm64-S.txt` and `p/linux-x86-S.txt`; the Linux legs use `p/run_linux.sh`.

| program | Darwin arm64 | Linux arm64 | Linux x86-64 |
|---|---|---|---|
| `uname_S_*`: the full record, not `partial`, plus a producer spelling 1280 zeros (Darwin) or 390 (Linux) | 0, Darwin, arm64, 0, `true` | 0, Linux, aarch64, 0, `true` | 0, Linux, x86_64, 0, `true` |
| `sun_SL_ascii_*`: `partial`, 104 or 108 zeros, L's loop, then bind / listen / connect / getsockname on a socket in `/tmp`, and a `sockaddr_storage` through getsockname | 0 0 0 0, path read back, 0, family 1, **exit 0** | same | same |
| `sun_SL_utf8_*`: writes each byte with `to_i8().must()` | **134** `does_not_fit` | 134 | 134 |
| `sun_SL_utf8wrap_*`: a hand-written two's complement (6 lines, +108 tokens) | exit 0, `/tmp/hèroes-178-s.sock` | exit 0 | exit 0 |
| `addrinfo_S`: `getaddrinfo("localhost","80")` | 0 | 0 | 0 |
| `statfs_S_darwin`: 1024-byte arrays the program *reads* | 0, apfs, / | — | — |
| `statvfs_S_linux` | — | 0, 4096, 255 | 0, 4096, 255 |
| `mutex_S_darwin`: `tag _opaque_pthread_mutex_t`, field `__sig` | zero then lock **22**; zero then init 0, lock 0, unlock 0 | — | — |
| `mutex_S_linux`: `tag pthread_mutex_t`, field `__align` | — | **ffi_unknown_tag** plus 3 ffi_parameter_type | same |
| `sha_S`: `Sha256(num: 0)` through `partial` | Update 1, Final 1, first digest byte **0**; after Init **186** | same | same |
| `flock_S`: `Flock(l_start: 0)` through `partial` | **−1** (EINVAL) | **0, a read lock taken** | **0** |

**What route S costs in emitted C.** `uname_S_darwin` emits **5555 lines** of C. Its one construction line names 1280 temporaries, `t1` to `t1284`.

**Can a UTF-8 path be written?** Yes, on all three legs, once 091 is repaired. But only with a hand-written two's complement: the spelling a model reaches for first, `to_i8().must()`, panics at 134. That failure is loud, not corruption. T's memcpy writes the same path with no arithmetic (§3).

**Needed under every route, found while writing the bindings:**
- **Every `sockaddr` call needs a C cast shim.** `@addr: SockaddrUn` against `const struct sockaddr *` is `ffi_parameter_type` (`p/sun_bind_direct.hero`). `p/s/un_shim.h` holds 4 `static inline` casts. No route in this sitting removes a line of it.
- **glibc's `pthread_mutex_t` cannot be held by value.** It is a typedef of an anonymous union, so the compiler spells it `struct pthread_mutex_t` and reports `ffi_unknown_tag`. The error's note suggests a handle instead. Following that note (`p/s/mutex_handle_linux.hero`) gives `panic: a null pointer was read through, at offset 0x10, called from pthread_mutex_lock`, exit 134: loud, thanks to defect 090. So the mutex behind measurement 7 cannot even be declared by value on Linux, and holding one there needs a shim that allocates it. That is a completeness gap under §1.12. I infer, and have **not run**, that the other pthread typedefs of anonymous unions have the same gap.

## 3. Each route's C against the real headers (`p/r/routes.c`)

The flags are the emitter's own, verbatim (`p/r/flags.txt`). The stack is dirtied with 0xAA before every construction. Outputs are in `p/r/routes-{darwin,linux-arm64,linux-x86}.txt`.

**On all three legs, at `-O0` and `-O2`:**
- **0 errors and 0 warnings.**
- `uname`: R1 and A both return 0.
- `sockaddr_un`: bind, listen, connect, getsockname and the `sockaddr_storage` getsockname all return 0, under R1+L, R1+T and A+L. **The UTF-8 path works under R1+L and R1+T.**
- `getaddrinfo`: S and R1 both return 0.
- `statfs` or `statvfs`: 0.
- **0 bytes of 0xAA** remain in any constructed struct.
- Mutex: R0 then lock returns 22 on Darwin and 0 on Linux. R0, then `pthread_mutex_init`, then lock returns **0 on all three legs**.

**A's GNU range designator** (`[0 ... N-1] = x`) is accepted under `-std=gnu11`. Under `-std=c11 -pedantic-errors` it gives 6 errors.

**R0's two possible spellings**, `(T){0}` and `memset`, were emitted for every struct and union the census headers define (`p/r/zeroall.sh`): 250 records on Darwin, 157 on Linux arm64, 153 on Linux x86-64. Under the flags plus `-Wextra`: 0 errors, 0 warnings.

## 4. Padding, with an instrument that works at `-O2` (`p/r/padmsan.c`)

**The method.** MemorySanitizer reports any uninitialised byte in a buffer handed to `write()`. I built each shape, then wrote `&cell` to `/dev/null`. Clang is 22.1.8. Outputs are in `p/r/msan-arm64.txt` and `p/r/msan-x86.txt`.

**The controls fire, so the instrument measures:**
- *every field assigned, no initialiser*: reported at `-O0` through `-O3`, on both legs;
- *a union written through its 4-byte member*: reported at every level;
- *a value with undefined padding copied over a zeroed cell*: reported at `-O0`, **clean at `-O1` to `-O3`**. That is clang using C11's latitude that a structure assignment need not copy padding. I recall this as C11 6.2.6.1p6 and its footnote, **and have not verified the citation**.

**Every route's shape is clean at every level on both legs.** The shapes tested:
- today's S: literal into a temporary into the cell, as in `p/s/addrinfo_S.c:282-284`;
- R1 as a literal;
- R1 as a memset of the cell;
- R1 as a memset of a temporary, then a copy;
- `= {0}`;
- R0 as `(T){0}` followed by stores;
- a union, zeroed both by `{0}` and by memset;
- `termios`, both as a literal and by memset.

**Darwin: MSan is not available there, so unrun.**

**Census.** `-Wpadded`, forced by a `sizeof` of every struct (`p/r/padded.sh`), finds public padded structs:
- Darwin: **69 of 245**;
- Linux arm64: **62 of 154**;
- Linux x86-64: **57 of 150**.

**Census APIs that compare struct bytes.** None of them reads padding:
- `IN6_ARE_ADDR_EQUAL`: memcmp over `in6_addr`, which has no padding;
- `IN_ARE_ADDR_EQUAL`: bcmp over `in_addr`, no padding;
- `sqlite3_snapshot_cmp`: the bytes are SQLite's own;
- `pthread_equal`: compares pointers.

**Do callees check bytes the program never writes?**
- `bind(127.0.0.1)` with `sin_zero` filled with 0xAA returns 0 on Darwin and on Linux arm64 (`p/r/sinzero.c`).
- getaddrinfo with garbage in `ai_addrlen`, `ai_canonname`, `ai_addr` and `ai_next` returns 0 on all three legs.
- **A Linux abstract socket compares every byte of `sun_path` up to `addrlen`.** One stray byte gives `connect` **ECONNREFUSED** (`p/r/abstract.c`). So "the rest zero" matters to the kernel for a field, not for padding.

**The one way padding leaves the process** is F1 (§8).

## 5. Zero-validity, from the world (`p/zinit.c`, `p/ctors.sh`)

**Where I looked:**
- every `*_init`, `*_Init` and `*emptyset` function in the census headers, from clang's AST;
- the `*_INITIALIZER` and `*_INIT` macros, and `FD_ZERO`;
- the `fcntl.h` and `signal.h` constants;
- Darwin's `pthread_mutex_init(3)` manual page. It says **nothing** about zero or about the static initialiser.
- Linux manual pages are not installed in the images: **unrun**.

Each probe runs in a child process with a 3-second alarm. Outputs are in `p/zinit-darwin.txt` and `p/zinit-linux.txt`.

| type, zeroed | Darwin arm64 | Linux arm64 | Linux x86-64 |
|---|---|---|---|
| mutex, cond, rwlock | **22** from lock, signal, broadcast, timedwait, wrlock | 0 (the initialiser is all zero) | 0 |
| attr, mutexattr, rwlockattr, passed on | **22** | 0 | 0 |
| `pthread_once_t` | works, runs once | works | works |
| `pthread_barrier_t`, then wait | — | **blocks forever** | **SIGFPE** |
| `pthread_spinlock_t` | — | free | **LOCKED**: unlocked is 1, trylock gives EBUSY |
| `SHA256_CTX` | returns 1, **wrong digest**; Final writes 0 of 32 bytes | same | same |
| `struct flock` with `F_SETLK` | EINVAL (`F_RDLCK` = 1) | **0, a lock taken** (`F_RDLCK` = 0) | **0** |
| `struct sigevent` | means `SIGEV_NONE` | means **`SIGEV_SIGNAL`** | means **`SIGEV_SIGNAL`** |
| `sigset_t`, `fd_set`, `in6_addr`, `mbstate_t`, `SIG_DFL` | valid | valid | valid |

`p/r/zbits.c` confirms that all-zero bytes read as NULL, +0.0 and false on all three legs.

**Can a binding author know Z1 holds everywhere? No:**
- Darwin's manual page is silent;
- the fact lives in a macro expansion on one platform and only in a run on another;
- two Linux legs on the same glibc 2.41 disagree on the spinlock;
- Windows is unrun.

**What a false claim costs:**
- an error code, loud only if the program checks it (the Darwin mutex returns 22 from both lock and unlock);
- a silent wrong answer with success returns (SHA);
- a lock taken without asking (flock on Linux);
- a deadlock or SIGFPE inside libc (barrier, spinlock). These last are not reachable by value today, since the glibc typedefs have no spelling, and would not become reachable under R0 or R1.

**Why Z2 over Z1.** An out-parameter needs a defined initial buffer. That is exactly what panel 163 vetoed route 3 for lacking, and all-zero is one on every leg. Whether the value is valid *for the library* is the library constructor's job. It belongs in the binding module that wraps the library, for example a `sha256_new()` that calls `SHA256_Init`.

## 6. Cost per route, and how today's compiler refuses each (`p/cost/`)

`heroes-next check` on each route's source:
- `Utsname(rest: zero)`: `unknown_name` for `zero`. Note that it parses, with `rest` read as a field label, so a C field named `rest` would collide with the spelling. The census has none; I grepped the AST.
- `.zero()`: `unknown_function`.
- `[0; 256]`: `unexpected_character`.
- `to_fixed`: `unknown_function`.

| binding | S | R1 | R0 | A | with T |
|---|---|---|---|---|---|
| `utsname` | 4067 | 198 (199 with Z1's mark) | 194 | 244 | — |
| `statfs` | 3313 | 172 | 170 | 206 | — |
| `sockaddr_un` | 985 (S+L) | 654 (R1+L) | — | 679 (A+L) | 618 (R1+T), 622 (R0+T) |

## 7. The census route: a field whose length is the header's

This is **the only route that makes `utsname` a single binding for every platform**, because all its fields are arrays. It cannot do the same for `sockaddr_un`, because `sun_family` is `u8` on Darwin and `u16` on Linux (measured). It composes with R1, R0 and T, and not with A or S. It strengthens §4.19, provided the length is read from clang the way today's `_Static_assert` checks already are.

## 8. Found while measuring: to file, and not this sitting's question

- **F1 (§1.12): a whole group record lent to a `void *` parameter has no bound on C's count.**
  - `read(fd: 0, buf: @h, n: 4096)` into a 48-byte record: `check` 0, run **exit 138**, and `--sanitize` reports **stack-buffer-overflow WRITE of size 4096** (`p/rec_overwrite.hero`).
  - Through `write`, the same shape leaks stack addresses (`p/rec_overread.hero`).
  - It is defect 010's sibling: `@value: i32` against `void *` is refused (`tests/golden/fixedbugs/ffi-pointee-void.hero`), but a record is not.
- **F2 (§1.12, completeness): glibc's `pthread_mutex_t` cannot be declared by value** (§2).
- **F3: a zero byte inside the text reaches C through `.cstr()` and is truncated silently.**
  - `read_file` of `ab\0cd` gives `len` 5 but `strlen(s.cstr())` 2, exit 0 (`p/nul.hero`).
  - design.md's escape freeze (around line 1012) refuses `\0` because an interior NUL *"silently truncates every C call"*. But `read_file` produces one with no escape at all.
  - I searched `docs/work/`, `docs/records/log/` and panels 083 and 164 for "interior" and did not find this. That is a question, not proof it is unrecorded.
  - This is also why T must refuse a zero byte inside the text.

## Files (all under the seat directory above)

- **Prototype**: `selfhost/emit/container.hero`, `p/seed-next.c`, `p/run_linux.sh`.
- **Route S programs**: `p/s/` (all the `.hero` files, `un_shim.h`, `sha_shim.h`, `mutex_handle_linux.hero`, `sha-x86.txt`).
- **Route C and padding**: `p/r/` (`routes.c`, `padmsan.c`, `sinzero.c`, `abstract.c`, `forgot.c`, `zbits.c`, `padded.sh`, `zeroall.sh`, `build_routes.sh`, `run_msan.sh`, and every `*.txt` output).
- **Zero-validity**: `p/zinit.c`, `p/ctors.sh`, `p/zinit-darwin.txt`, `p/zinit-linux.txt`.
- **Costs**: `p/cost/`.
- **Findings**: `p/rec_overwrite.hero`, `p/rec_overread.hero`, `p/nul.hero`, `p/shapes.hero`, `p/sun_bind_direct.hero`, `p/selftest.txt`.

2026-10-06, defect 404: `p/nul.hero` is `ffi-pragmatist-work/nul-byte.hero` in this tree from this day, and the completeness critic's `w/ffi/nul.hero` is `w/ffi/nul-byte.hero`, both renamed because Windows reserves `nul` as a device's name with any extension and the CI's checkout refused them.
