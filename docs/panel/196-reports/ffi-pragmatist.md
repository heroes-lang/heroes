# Panel 196, ffi-pragmatist

Copy: `<scratchpad>/196-ffi-pragmatist/`, `git archive 39935f7c`, compiler built
from its seed at 20:56:06 (by `date`; seed SHA-256 opens `1eeade6039fee357`);
the same seed built for Linux arm64 inside `heroes-linux-arm64` as
`heroes-lxa64` (21:09). The sitting's evidence copied from the lane's untracked
`docs/panel/196-evidence/` into `p196ffi/evidence/`; every probe of mine is
under `<scratchpad>/196-ffi-pragmatist/p196ffi/`. No paid run, no timing; times
are `date` readings. Work ran 20:55 to 21:21, stopped by a session limit, and
resumed 22:44 to 22:52. Nothing in the repository, any worktree or another
seat's copy was touched; the compiler-engineer's prototypes (S1, S2, S7) are
cited from their report, never run.

## The seat's answer, in the shape the seat owes

- `verdict`: **veto** on any route (S4 or S7) that routes a RECORD lend through
  a temporary; **approve** U, S7 with a guard page, S2 as S7's spelling of a
  stated extent, S5 as a complement, S4 in its guard-page form for scalar cells
  and emitter-owned temporaries; **object** to S1, S1', S3 (both), S6 as the
  resolution, S5' and nothing. Per route in the table below.
- `section`: design.md §1.11 (points 2 and 3, and level 3's *unbox-in/box-out*),
  §1.12, §4.19; design.md is silent on a lend's count and on a lend's address
  as C's identity.
- `experiment`: `p196ffi/routes/routes_c.c` (each route's call against the real
  `openssl/sha.h`), `s4/s4_guard.c`, `s4/s4_neigh.c`, `s7g/s7_guardpage.c`,
  `s7g/s4g.c`, `ident/zident.c`, `s5/redecl*.c`, `win/*.c`: clang accepted all
  with 0 warnings on Darwin arm64 (Apple clang 21.0.0) and Linux arm64 (clang
  22.1.8), and the Windows files under clang 23.1.1; `s5/redecl.c` is refused
  on purpose (*conflicting types*, both platforms), `routes/s2_mismatch.c` too
  (*static assertion failed*). Heroes programs on the frozen compiler in
  `r2/`, `r2lx/`, `ident/`.
- `argument`: No route breaks the ABI: S1, S1', S2 and S6 emit identical C at
  the call. Where the header states no extent (OpenSSL: 0 of its digest
  outputs, on two versions), every static route makes 32 the binding's word,
  and a wrong word corrupts at exit 0 on both platforms. S7 is the only route
  binding `SHA256_Final`, `pipe` and `gethostname` with no C by hand; placed
  against a guard page, an understated extent becomes a named abort (or a
  refused syscall), measured on three platforms. Copying a record is
  categorically wrong: zlib refuses a moved stream, and today's element lend
  already moves it (leak, exit 0).
- `prediction`: under any route that copies a lent record, `ident/z_elem.hero`'s
  zlib calls keep returning -2 and LeakSanitizer keeps reporting 268,096 bytes
  on Linux arm64; under a guarded S7, `@md: [u8] counted_by 16` with SHA-256
  aborts naming `SHA256_Final`'s `md` on Darwin and Linux arm64.
- `condition`: the veto lifts for a record whose C never keeps its address, if
  any header could say so (none does); S7's approval turns to object if its
  buffer is bare on the stack, or if a run-time count is honoured by copying
  rather than by lending in place.

## Verdicts

Sections quoted from my copy's `docs/design.md`: §1.11 (line 467): *"FFI
ergonomics rank alongside comprehension, not below it. When a choice makes
bindings harder, that is a serious cost"*; its point 2, *"A thin C shim remains
standard practice ... shims are for the hard cases only"*; its point 3, *"a
wrong type in an `extern` is a compile error, which is this project's thesis
applied to the boundary"*; its level 3, refused because *"every C call
require[s] unbox-in/box-out, which is precisely the glue we refused to write"*.
§1.12 (line 575): *"must not segfault and must not corrupt memory ... any C
library must be bindable"*, and *"check rather than assume ... one predictable
branch per FFI argument, and it is paid deliberately"*. §4.19 (line 2322): *"a C
out-parameter is an `@` parameter, which §4.8 already compiles to a pointer"*.
**design.md does not cover** how many elements an `@` lend promises, a stated
extent, `counted_by` (no text at all), or what a lend's address is worth to C
(identity); on those this seat stands on measurement, not on a section.

**No route changes the C ABI**: at the call S1, S1', S2 and S6 emit the same
machine code, `SHA256_Final(&field, &ctx)` (§2). **One veto**, on any form of
S4 or S7 that copies a RECORD lend: it breaks zlib on two platforms (§4).

| route | verdict | section | cost, measured | falsifiable prediction | condition |
|---|---|---|---|---|---|
| **U**, the ruling: an unmarked `@x: T` promises ONE `T` | **approve** | §4.19; design.md silent on a count | 0 at the boundary; the single-cell scalar out-parameter is the commonest writable typed pointer after records on the SDK and in Homebrew, and second to `T **` on Linux: 2,754 in 2,052 functions on the SDK, 6,957 in 4,918 in Homebrew, 1,980 in 1,614 on Linux arm64 | in the 84 SDK headers that declare `_LIBC_SINGLE_BY_DEFAULT()`, the unannotated writable typed pointers (537 today) stay at least 4 times the stated extents (118 today) in any later SDK | a header family where unannotated means *many* by the vendor's own rule |
| **S6**, the sentence alone (a buffer through a shim) | **object** as the resolution | §1.11 point 2 | `pipe`, the most basic POSIX call, has **no correct binding without a C function** today (`pipe_whole.hero` `ffi_parameter_type`, `pipe_ptr.hero` `lent_shape`, both platforms); S6 writes that down as the language's answer. A hand-written wrong shim corrupts as silently (`s6_16.hero`: Mac 134 empty stderr, Linux 0) | under S6, `p196ffi/r2/pipe_cell.hero` stays `check` 0, `run` 0, `after` 4, on both platforms | approve its first half (U) apart |
| **S1**, `@md: u8[32]` | **object**: dominated by S7 | §1.11 points 2 and 3 | the call is the field's address (identical C, §2); the pointee check stays typed; but the place must be a group record's field, so a hand-written header struct (the critic; CE) | an S1 compiler without S0 binds `SHA256_Final` correctly only with a header the program writes, on both platforms | approve if S7 is refused AND S0 (a local fixed array lent to C) lands with it |
| **S1'**, a one-field record lent whole to `T *` | **object** | §1.11 point 3 | needs today's pointee refusal (`ffi_parameter_type`, *a different kind of thing*) relaxed for records of one array field; the same header struct | none | none: S7 covers it without relaxing a check |
| **S2**, `counted_by` a constant | **approve as the spelling S7 shares**; object as a separate route | §1.12, §4.19 | the constant extent is checked against the place (`_Static_assert`, refused at compile time on a `u8[16]` field, both platforms, §2); an understated constant passes and corrupts (`counted_by 16` over `u8[16]`: `after` overwritten, exit 0, both) | under S2, `counted_by 32` over a `u8[16]` field is refused at build on Darwin, Linux arm64 and Linux x86-64 (CE measured `field_lend_extent`; my C on two) | its emitter half lands with it (CE: a check-side S2 alone emits no check); **no `guess` fix that rewrites the stated extent to the field's length** (CE's finding: that fix writes the lie) |
| **S7**, `@md: [u8] counted_by N`, an emitter-owned buffer | **approve, conditioned** | §1.11 (no C by hand), §1.12 | the only route after which `SHA256_Final`, `pipe` and `gethostname` bind with no C (CE, built); two copies of N per call (CE's emission: one `hero_array_at` per element copied in and one `hero_array_push_owned` per element copied out, up to 2N runtime calls); an understated N overruns the emitter's stack buffer at exit 134, silent under `--sanitize` (CE). **The guard page closes that**: the buffer placed to end where a `PROT_NONE` page begins (§4): extent 32 correct, 16 and 1 `panic: C wrote past the bytes lent to SHA256_Final's md`, exit 134, Mac and Linux | a guarded-S7 compiler running `@md: [u8] counted_by 16` with SHA-256 prints a panic naming `SHA256_Final`'s `md` and exits 134 on Darwin and Linux arm64, and on Windows a user-mode writer aborts the same way while a kernel writer (`GetKeyboardState`) returns failure, `ERROR_NOACCESS` (998), with nothing corrupted | (a) the buffer lives against a guard page, never bare on the stack; (b) **a record is never copied** (veto, below); (c) a count-carrying buffer (`read`, `recv`, `gethostname`'s sibling) is NOT copied N bytes per call: that is §1.11 level 3's *unbox-in/box-out*; it is lent in place (S0's `[u8]` through `.ptr()` with route C's run-time check). S7 with a sibling count by copy is **object** |
| **S3 widened** (every unmarked one-cell lend refused) | **object** | §1.11 | a word on the commonest correct C shape: the 2,754 / 6,957 / 1,980 scalar out-parameters above, 34 live lends in the tree (12 the compiler's); Apple's audited headers say the default is one (537 unstated against 22 literal and 96 named in the same 84 files) | none | none from this seat; the spec-warden's veto stands on Principle 0 |
| S3 narrow (byte pointees) | object | §1.11 | refuses 163's correct `char *`; misses `pipe` (`int`) | none | none |
| **S4**, a run-time guard | **approve in its guard-page form** for every emitter-owned temporary (S7's buffer, the element temporary of `ir/inout.hero`) and for an unmarked scalar cell copied to the guarded page; **object to the canary form as a promise**; **VETO any form that copies a record lend** | §1.12; §1.11 (*any C library must be bindable*) | canary, 64 bytes a side: catches 32 into 1, 4,096 into 1 (`read` of `/dev/zero`), `pipe`'s second `int`, `gethostname`, SHA-512's 64 into 32, on both platforms; misses a write into the heap (`victim[8]` 66, exit 0); +53 static instructions at the call site at `-O2` (57 against 4). Guard page: no fill, no compare; catches any forward overrun; on Linux `pipe` into a one-`int` cell at the page's end returns -1 (the kernel's `EFAULT`), nothing corrupted. **A copied `z_stream`**: `deflate` returns `Z_STREAM_ERROR` (-2), both platforms | (veto) an S4 or S7 that routes a record lend through a temporary turns `p196ffi/ident/zident.c`'s `copy` case into a binding that cannot work: `deflate` -2 on every call, Mac and Linux arm64 | lifted only for records the emitter can show C never keeps the address of, which no header states |
| **S5**, the header's own extent | **approve as a complement**, never the sole route | §4.19, §1.11 point 3 | reaches what the header states: 96 parameters in 90 functions (SDK), 173 in 149 (Homebrew), 116 in 107 (Linux arm64), `pipe` and `uuid_generate` among them; **7 of 278 digest-like byte outputs, 0 in OpenSSL** (§3). Mechanism: the header's spellings or filtered `-ast-print`; **never a redeclaration in Heroes' C spellings** (conflicting types on correct bindings, both platforms, §1) | a prototype reading `-ast-print` refuses `pipe(@fds: i32)` (`critic/pipe_cell.hero`) on Darwin and Linux arm64 and builds `md_scalar.hero` (the miss, stated) | glibc's `__attr_access` and Windows' SAL are invisible to clang (0 survive preprocessing, measured): a sentence saying *the header says* must say *declares an array of*, unless a reader of those texts is built |
| S5', `-fbounds-safety` for the unit | object | §1.11 | Mac only; one-cell lends to `pipe`, `uuid_generate`, `ctime_r` compile silently and trap 133 with no message | none | none |
| Nothing | object | §1.12 | 396 stays silent at exit 0 on four platforms (Windows: `GetKeyboardState`, §5) | none | none |

**The strongest reason against what I approve, from this seat**: S7 is
marshalling. It is bounded where it is right (fixed extents: digests 16 to 64
bytes, `uuid_t` 16, `pipe` 8; the SDK's largest stated extent is 4,096), and it
becomes the Lua lesson the moment it carries a run-time count, which is why
condition (c) is not optional. And S5, which keeps §1.11 point 3's promise at
the boundary, reaches 0 of OpenSSL's digest finals: for `SHA256_Final` the 32 is
the binding's word under every static route, and only the guard turns a wrong
word into a named abort.

## 1. The census of the fault's shape

### This Mac, 21:02 to 21:12

**Instrument** (`p196ffi/census/dump.py`, `classify.py`, `report.py`): for every
header, a translation unit holding only `#include <header>`, compiled with
`clang -std=gnu11 -fsyntax-only -fbounds-safety -Xclang -ast-dump` (Apple clang
21.0.0), its top-level `FunctionDecl`s and their `ParmVarDecl` types recorded,
functions deduplicated by name and kept only where the name's file lies under
the set's root. **Why `-fbounds-safety`**: in that mode clang itself turns a C
array parameter (`int [2]`, `uuid_t`) and the SDK's `_LIBC_COUNT(n)` into
`__counted_by(n)` in the parameter's type, a named count into
`__counted_by(<param>)`, and leaves an unannotated pointer `__single` (one
element), so the header's own statement of an extent is read by clang, not by
my regex. The AST is printed even where the mode raises errors in a header's
inline bodies (OpenSSL's `safestack.h`), so those are kept. A **writable typed
pointer** is a `T *` whose `T` (typedefs resolved from the same dumps) is not
`const`, not `void`, not a function. Sets: the SDK's `usr/include` less `c++/`
(1,671 headers; 76 fatal, 329 with non-fatal errors) and `find -L
/opt/homebrew/include -name '*.h'` (2,891 headers; 1,307 fatal, mostly C++ or
needing their own `-I`; 462 with non-fatal errors).

| | macOS SDK | Homebrew |
|---|---|---|
| functions | 15,965 | 38,233 |
| writable typed pointer parameters | 16,284 in 10,283 functions | 38,416 in 25,568 |
| **the header states a literal extent, no count parameter** (`__counted_by(26)`, `int [2]`, `uuid_t`) | **96 in 90 functions** | **173 in 149** (nettle 60, libavutil 32, OpenSSL 25, lcms2 14, ...) |
| the header names a count parameter (`__counted_by(__namelen)`) | 103 in 102 | 0 |
| NUL-terminated / explicitly unbounded | 96 / 25 | 122 / 0 |
| **the header says nothing** | **15,964** | **38,121** |
| of which a byte pointee (`char`, `signed char`, `unsigned char`) | 1,036 in 932 functions; 375 with no integer parameter at all | 1,971 in 1,748; 453 |
| of which another scalar pointee (the single-cell out-parameter a route must keep: `int *` 1,496, `unsigned int *` 428, `unsigned long *` (`size_t *`) 423 on the SDK) | **2,754 in 2,052 functions** | **6,957 in 4,918** (`int *` 4,157, `size_t *` 982, `float *` 441) |

The SDK's 96 literal extents include exactly the brief's examples and the
critic's: `pipe` and `pipe2` (`__counted_by(2)`), `encrypt` (64), `ctime_r`
and `asctime_r` (26), `strmode` (12), `uuid_generate` and its family (16),
`uuid_unparse` (37), `setjmp` (48), `erand48`/`seed48` (3), `tmpnam` and
`ctermid_r` (1024), `getpwuuid` and `mbr_uid_to_uuid` (16), and the digest
finals of the SDK's own libraries: `apr_md5_final` and `apr_md4_final` (16),
`apr_sha1_final` (20), `_sasl_MD5Final` (16), `httpMD5Final` (33).
(`asctime_r` was missed by my first classifier, a `restrict` stripping bug,
fixed before these numbers; the SDK's `inet_ntop` names its count, `__size`;
`gets` is `__unsafe_indexable`; both as the critic says.)

**OpenSSL 4.0.3 states 25 extents, all of them `ivec[16]` or `ecount_buf[16]`
in `modes.h`, `camellia.h`, `seed.h`**, and **0 for any digest output**. Of its
423 unstated writable byte pointers, 79 sit in a function with no integer
parameter at all; the digest finals among them are `MD4_Final`, `MD5_Final`,
`MDC2_Final`, `RIPEMD160_Final`, `SHA1_Final`, `SHA224_Final`, `SHA256_Final`,
`SHA384_Final`, `SHA512_Final`, `WHIRLPOOL_Final`, plus the one-block ciphers
`AES_encrypt`, `AES_decrypt`, `Camellia_encrypt`, `Camellia_decrypt`,
`IDEA_ecb_encrypt`, and `DES_fcrypt`. **39 of the 79 are the critic's
out-count twin**: a scalar pointer sits beside the bytes (`EVP_DigestFinal_ex`,
`HMAC_Final`, `X509_digest`, `CMAC_Final`, the `EVP_*Final*` family,
`EVP_PKEY_derive`). The header cannot tell in from out: `EVP_PKEY_derive`'s
`size_t *keylen` is read as the buffer's size and then written,
`EVP_DigestFinal_ex`'s `unsigned int *s` is only written (OpenSSL's
documentation, not the header). With an integer parameter that counts
something else, `SHA256(d, n, md)`, `MD5`, `HMAC`, `EVP_Digest`, `EVP_Q_digest`
hide the same shape behind an integer that counts the input, so a search for
*no count beside it* cannot see them and the list above is **a floor**.
**How I searched, so the negative is mine**: clang's own types for the
header's words, then by name for byte parameters called `md`, `digest`, `out`,
`mac`, `hash`, `tag`, `key`, `iv`, `buf` (212 more in OpenSSL), then by
function name (`final`, `digest`, `_sum`, `_hash`, §3); a function whose array
has another name or a non-byte pointee is in none of these lists.

**The SDK already rules what an unannotated pointer means**: 84 SDK headers
declare `_LIBC_SINGLE_BY_DEFAULT()` / `__ptrcheck_abi_assume_single()`; in them
537 writable typed pointers are unannotated and therefore, by Apple's own
statement, **one element**, against 22 literal and 96 named extents in the same
files. That is U, written by a platform vendor: unstated means one, and an
array says its extent. GMP's idiom says it in the type: nettle's `mpz_t`
parameters are `__mpz_struct[1]`, a stated extent of one.

**What clang can check at a call without `-fbounds-safety`** (`p196ffi/s5/s5_clang.c`,
`-Wall -Warray-parameter`): a redeclaration `int pipe(int fds[1]);` warns
*mismatched bound*; `int SHA256_Final(unsigned char md[16], ...)` is silent; a
declaration `[static 32]` warns *array argument is too small; contains 16
elements, callee requires at least 32* when handed a `u8[16]` array, and is
**silent** when handed a one-byte cell (`&cell` or `(void *)&b.m`, today's
emitted shape). **With `-fbounds-safety`** (`s5_bs.c`): a one-cell lend to
`pipe`, `uuid_generate`, `ctime_r` compiles with **no diagnostic** and traps at
run time, exit 133 at `-O0` and `-O2`: a run-time guard, Mac only.

### Linux arm64 (`heroes-linux-arm64`, Debian clang 22.1.8, OpenSSL 3.5.7), 21:12 to 21:18

Same instrument without `-fbounds-safety` (upstream clang has no such mode
for parameters): `dump_lx.py` adds a filtered `-ast-print` pass, which keeps
the header's spelling (`int __pipedes[2]`; a typedef of an array resolved from
the dump), because the AST dump decays the bound. 5,098 headers under
`/usr/include` less `c++/` (2,246 fatal: kernel UAPI and C++ headers; 323 with
non-fatal errors); 17,303 functions; 18,575 writable typed pointers in 11,409.

- **Stated extent, no count parameter: 116 in 107 functions**: `pipe` `[2]`,
  `socketpair` `[2]`, `erand48`/`seed48` `[3]`, `lcong48` `[7]`, `tmpnam`
  `[20]`, `if_indextoname` `[16]`, the `setjmp` family, nettle's
  `rsa.h`/`dsa.h`/`pkcs1.h` 56 (`mpz_t`, `[1]`), gnutls key ids `[8]` (8), and
  **OpenSSL 3.5.7's 18, every one `ivec[16]`/`ecount_buf[16]` in `modes.h`**;
  44 more are `[]` with no number.
- **Digest outputs with a stated extent in OpenSSL: 0** (`SHA256_Final`'s `md`
  unstated, as on the Mac).
- Unstated scalar pointee (the single-cell out-parameters): **1,980 in 1,614
  functions** (`int *` 871, `unsigned int *` 458, `unsigned long *` 448);
  unstated byte pointers 1,290 in 1,096 functions, 348 with no integer
  parameter.
- **glibc's own annotations are invisible to clang**: `grep` counts
  `__attr_access ((__write_only__, N, N))` 78 times and `((__write_only__, N))`
  4 (`if_indextoname`, `getwd`, two in `stdio.h`), but `cdefs.h:831` defines
  the macro only under `__GNUC_PREREQ (10, 0)`, and `clang -E` of `unistd.h`,
  `stdio.h`, `net/if.h` holds **0** `__access__` attributes. Only the C array
  spelling reaches clang on Linux.

### What a header-reading S5 must not do: redeclare in Heroes' spellings (`s5/redecl.c`, `redecl2.c`), 21:20

The compiler-engineer's report names *S5's redeclaration* as what would catch
`@fds: i32[1]`. A redeclaration written in the C types the emitter uses for
Heroes types (`int32_t`, `int8_t`, `uint64_t`, `int64_t`) **refuses correct
bindings**: `int gethostname(int8_t a0[64], uint64_t a1);` and `char
*ctime_r(const int64_t *a0, int8_t a1[26]);` are `error: conflicting types`
on this Mac and on Linux arm64 under `-std=gnu11`, the emitter's dialect
(`selfhost/cli/flags.hero:93`; `-std=c11` hides both declarations in glibc, so
that run was vacuous and is not counted). Written in the **header's own
spellings**, as `-ast-dump=json` of `__typeof__(f)` prints them (`int
gethostname(char a0[64], size_t a1);`), the same probe is silent on the right
bindings and warns `-Warray-parameter` only on `int pipe2(int a0[1], int a1)`
(Mac; glibc declares `pipe2` only under `_GNU_SOURCE`). This is §4.19's
*"re-declaring gives conflicting types five times out of five"* met again; the
probe already reads those spellings, so the cost is a new ask, not a parser.
Filtered `-ast-print` (`-Xclang -ast-print -Xclang -ast-dump-filter=pipe`)
prints `int pipe(int[2])` on both platforms and is the other mechanism (a
typedef such as `uuid_t` needs its type resolved, which the dump gives).

## 2. `SHA256_Final` under each route, and the neighbours, 21:10 to 21:20 and 22:44 to 22:50

**Two kinds of evidence, said apart.** (a) Heroes programs on the frozen
compiler (`p196ffi/r2/`, Mac; `p196ffi/r2lx/`, Linux arm64), judged by `check`
and `run`: today's forms and S6. (b) For S1, S1', S2, S4, S7, which the frozen
compiler does not build and whose prototypes live in another seat's copy:
**the C each route's emitter writes at the call** (`p196ffi/routes/routes_c.c`,
S1's call as the compiler-engineer's report quotes its emission,
`SHA256_Final((void *)&h1_d.b, &h0_c)`; route C's check as its
`_Static_assert` in `sizeof(*(P)0)` units), built `-std=gnu11 -fsigned-char
-Wall` at `-O0` and `-O2` against the real headers and libcrypto, each case run
alone. **0 warnings on both platforms; every row identical at `-O0` and
`-O2`, on Darwin arm64 and Linux arm64.** Group record `Box { m: u8[32],
after: i64 }` built with `after: 7`; the digest of `abc` opens 186, closes 173.

| route | binding | right extent (32) | wrong extent (16) |
|---|---|---|---|
| **today** `@md: u8`, lent `@b.m` of `{m: u8, rest: u8[7], after}` (`md_field.hero`) | none states 32 | Mac and Linux: `check` 0, `run` 0, `after` 2531777658719584577 (bytes 8 to 15 of the digest) | (always wrong) |
| **today, an element**: `@md: u8` lent `@b.m[0]` of `{m: u8[32], after}` (`today_elem32.hero`) | the storage IS 32 | `check` 0; **Mac `run` 0, Linux `run` 135 (empty stderr)**; prints `186 0 7`: `m[31]` is **0**, not 173: `ir/inout.hero` copies an element into a one-byte C temporary (emitted: `h2_e0 = t13; SHA256_Final((void *)&h2_e0, &h0_c); ... h1_b.m[...] = t15;`), so C writes 31 bytes past a temporary and the array gets one byte | same |
| **S1** `@md: u8[32]`, lent `@b.m` | the type | C: `186 173 after 7`, both platforms (CE: `fixed_param.hero` builds and prints 173) | binding `@md: u8[16]` over a `u8[16]` field: C writes 32, `after` -7171393520880516176 (bytes 16 to 23), exit 0, both platforms. A `u8[16]` field lent to `@md: u8[32]` is `type_mismatch` (CE) |
| **S1'** a one-field record `{d: u8[32]}` lent whole | the record | C: `186 173 after 7` | the same lie with a 16-byte record |
| **S2** `md: ptr counted_by 32 lent`, `b.m.ptr()` | a constant | C: `186 173 after 7` | `counted_by 16` over a `u8[16]` field: the static check passes (16 <= 16), `after` overwritten, exit 0. `counted_by 32` over a `u8[16]` field: **compile error**, *static assertion failed ... past the field*, both platforms |
| **S6** a shim `hero_sha256_final32(struct box32 *)` (`s6_32.hero`) | C written by hand | Heroes: `check` 0, `run` 0, `186 173 7`, both platforms | `s6_16.hero` (the shim over a 16-byte struct): `check` 0, **Mac `run` 134 with an empty stderr, Linux `run` 0**, `after` -7171393520880516176 on both |
| **S7** `@md: [u8]` with extent 32, through an emitter temporary of 32, copied back | the emitter's | C: `186 173 len 32` (CE, built: `1 1 1 32 186 173`, `--sanitize` 0) | extent 16, an unguarded 16-byte temporary: the neighbour slot overwritten, exit 0 (CE, built: 134, silent under `--sanitize`). **Against a guard page: exit 134, `panic: C wrote past the bytes lent to SHA256_Final's md`, both platforms** (§4) |
| **S4** today's `@md: u8`, cell guarded by 64 bytes of 0xA5 each side | none | C: **exit 134, `panic: C wrote past the one u8 lent to SHA256_Final's md`**, both platforms | S7's 16-byte temporary canary-guarded: **exit 134**, the same panic |

**What the table says, from this seat**: at the call, S1, S1', S2 and S6 emit
the **same machine code** (`SHA256_Final(&field, &ctx)`): no route changes the
ABI or adds marshalling, except S7 (two copies of N per call) and S4 (a copy of
the cell, a 129-byte fill and compare). **Where the header states no extent,
every static route turns 32 into the binding's word, and a wrong word corrupts
at exit 0 on both platforms**; only a guard turns it into a named abort. S2's
route-C check catches a field shorter than the stated extent (the place's
mistake), never a stated extent shorter than what C writes (the binding's
mistake), which is 396.

**The neighbours** (`r2/`, `r2lx/`, `s4/s4_neigh.c`; S1, S2, S7 cells are the
compiler-engineer's measurements on their prototypes, cited):

| neighbour | today | S1 | S2 | S7 | S6 | S4 (canary, C) | S5 (header's words) |
|---|---|---|---|---|---|---|---|
| `pipe` (`int [2]` in both headers) | `@fds: i32` lent `@b.fd`: `check` 0, `run` 0, `0 3 4`, `after` 7 became 4, both platforms. **No correct binding without a hand-written C function**: `{fd: i32[2], after}` lent whole is `ffi_parameter_type` at `run` (`pipe_whole.hero`, both); `fds: ptr lent` is `lent_shape` (`pipe_ptr.hero`) | `@fds: i32[2]`: `0 1 7` correct; `@fds: i32[1]`: `after` 4 | no spelling: `.ptr()` takes only `i8[N]`/`u8[N]` (`bad_operand`) | `@fds: [i32] counted_by 2`: `0 2 1 0`, correct, no C | `hero_pipe(struct fds2 *)`: `0 3 4 7`, both | `panic: C wrote past the one i32 lent to pipe's fds`, 134, both | **catches it**: `[2]` is in the header on both platforms |
| `gethostname` (count present) | `@name: i8`, `namelen: 64`: Mac `run` 134 with the false *null function pointer* panic; **Linux `run` 0, `after` 1650537270 (host name bytes), silent**. Correct today with no C function: `name: ptr counted_by namelen lent`, `b.n.ptr()` of an `i8[64]` field (`host_ok.hero`: `0 118 7` Mac, `0 54 7` Linux), but the field needs a header struct; `namelen: 72` is refused `field_lend_extent` (`host_past.hero`) | `@name: i8[64]` does not tie `namelen` to it | route C, landed, already right | `counted_by 256`, `namelen: 256`: correct; `counted_by 4`, `namelen: 256`: 134 with the false panic (the constant ties nothing to the sibling) | not needed | `panic: C wrote past the one i8 lent to gethostname's name`, 134, both | Mac only (`__counted_by(__namelen)`); glibc says nothing clang reads |
| `EVP_DigestFinal_ex`'s out-count | `counted_by s`, `s: 32`: SHA-256 correct, SHA-512 `run` 134 Mac and **135 Linux** after printing `after` -6286656575195475423 | `@md: u8[32]`, `@s: u32` a plain out-parameter; SHA-512 still writes 64 (the binding's word) | `counted_by EVP_MAX_MD_SIZE` into a `u8[32]` field: refused at build; into `u8[64]`: `1 1 1 64 7`, correct | `counted_by EVP_MAX_MD_SIZE`, SHA-512: `s` 64, correct, `--sanitize` 0 | the shim checks `n == 32` after the fact (`sha_shim.h`) | S7's 32-byte temporary, SHA-512: `panic: ... EVP_DigestFinal_ex's md`, 134, both | nothing: the header cannot say in from out |

**Correction, 22:50**: the table first written at 21:19 said S2 on the EVP
twin was *a word nobody checks*; the compiler-engineer's prototype measures
`counted_by EVP_MAX_MD_SIZE` refused into 32 and correct into 64, so S2
answers the twin by spelling, and the critic's `counted_by s` (an out-count)
still builds and corrupts. The S4 column was an inference at 21:19 and is
measured now (`s4_neigh.c`).

## 3. S5 on real headers: how many say the extent

Counted by the census of §1 and by `grep -R` (following Homebrew's symlinks):

- **`[static N]`**: **0** files in the SDK's `usr/include`, in
  `/opt/homebrew/include`, and in Debian's `/usr/include`.
- **clang's `counted_by`/`sized_by` written out**: the SDK 55 spellings (the
  `ptrcheck.h`-style macro definitions and a few direct uses); Homebrew 0;
  Linux 0. The SDK's annotations proper are `_LIBC_COUNT` (161) and
  `_LIBC_SIZE` (91), which become `__counted_by`/`__sized_by` only under
  `-fbounds-safety`, and which the census reads: 96 literal and 103 named
  parameters.
- **A C array with a length in a parameter**: SDK 96 parameters in 90
  functions (array syntax and `_LIBC_COUNT` together), Homebrew 173 in 149,
  Linux arm64 116 in 107.
- **Digest outputs**: of the byte-output functions whose name says
  *final*/*digest*/*hash* (`re.I`), the SDK has 16, **6 with a stated extent**
  (`apr_md4_final`, `apr_md5_final`, `apr_sha1_final`, `_sasl_MD5Final`,
  `_sasl_hmac_md5_final`, `httpMD5Final`); Homebrew 143, **1**
  (`av_murmur3_final`); Linux arm64 119, **0**. **7 of 278; OpenSSL 0 on both
  versions.**
- **Windows**: the SDK states extents in SAL 5,828 times, and clang sees none
  (§5).

So S5 is right where it reaches (`pipe`, `uuid_t`, `ctime_r`, `ivec[16]`,
`setjmp`) and reaches almost no digest.

## 4. S4's guard in C, and its guard-page form

**The canary form** (`p196ffi/s4/s4_guard.c`, `s4_neigh.c`): the lent cell
copied into the middle of `struct { uint8_t pre[64]; T cell; uint8_t post[64];
}` filled with 0xA5, C handed `&g.cell`, both guards compared after the call,
the cell copied back. Built `-O0` and `-O2`, run on Darwin arm64 and Linux
arm64 (22:45), identical:

| case | result |
|---|---|
| `SHA256_Final`, 32 into 1 | `panic: C wrote past the one u8 lent to SHA256_Final's md`, 134 |
| `read` of 4,096 bytes of `/dev/zero` into 1 (far past the 64-byte guard) | the same panic for `read`'s `buf`, 134: the write smashed the frame above, and the check ran before the function returned. Caught **because the stack had room**: a write past the stack's top would fault inside C first |
| 32 zero bytes into 1 | caught (the canary is not zero) |
| `pipe`, `gethostname`, SHA-512's 64 into a 32-byte temporary | caught, 134 each |
| C writing into the **heap** through another pointer | **not seen**: `victim[8]` 66, exit 0 |

Not seen either, by construction: a read past the cell; a write made later
through a pointer C kept; a write that lands beyond the guard without touching
it. **Cost**: `final_today` is 4 instructions at `-O2`, `final_guarded` 57
(`otool -tV`, static count; the dynamic count is unrun: no instruction counter
on this Mac or in the image).

**The guard-page form** (`p196ffi/s7g/s7_guardpage.c`, `s4g.c`;
`p196ffi/win/s7g_win.c`, `s7g_win2.c`): the runtime maps two pages once and
makes the second `PROT_NONE`; the emitter's temporary of N is placed to END at
the second page, so the first byte past N faults. A `SIGSEGV`/`SIGBUS`
handler that finds `si_addr` in that page names the call. No fill, no
compare: two copies of N.

| platform | case | result |
|---|---|---|
| Darwin arm64 (page 16,384) | `SHA256_Final`, extent 32 / 16 / 1 | `186 ... 173`, 0 / `panic: C wrote past the bytes lent to SHA256_Final's md`, 134 / the same, 134 |
| Linux arm64 (page 4,096) | the same | the same three results |
| Darwin arm64 | `pipe` into one `int` at the page's end (`s4g.c`) | `panic: C wrote past the one cell lent to pipe's fds`, 134 (libc's wrapper writes in user mode) |
| Linux arm64 | the same | `pipe rc -1`: the kernel refused the copy (`EFAULT`), nothing written past, exit 0 |
| Windows (page 4,096) | `GetKeyboardState` (`_Out_writes_(256)`), extent 256 / 16 | `ok 1` / `ok 0`, `GetLastError()` **998** (`ERROR_NOACCESS`), exit 0, nothing corrupted |
| Windows | `PathBuildRootA` (`_Out_writes_(4)`, user mode), extent 4 / 2 | `C:\`, 0 / `panic: C wrote past the bytes lent to PathBuildRootA's pszRoot`, 134 (vectored exception handler) |

So a guarded temporary turns every forward overrun into a named abort where C
writes in user mode, and into a failed call where the kernel writes, on all
three platforms run. The runtime already owns a `SIGSEGV`/`SIGBUS` handler
with a *two witnesses* discipline (`runtime/parts/stack.c:29`, `:402` to
`:447`); a scratch page would be its third witness. That is a runtime ABI
change and a design, unbuilt in Heroes; and under `--sanitize` ASan owns the
handler (`stack.c:38`), so the fault would be named by ASan, not by us.

**What no guard may do: copy a record** (`p196ffi/ident/zident.c`, and
`z_local.hero`, `z_elem.hero`). zlib keeps the stream's address
(`state->strm`) and refuses a stream that moved. In C, `deflate` on a
`z_stream` copied into an S4-shaped temporary returns **-2
(`Z_STREAM_ERROR`)**, in place it returns 1 (`Z_STREAM_END`), Darwin and Linux
arm64. It is **live today** through the element lend's temporary (`ir/inout.hero`),
found beside, below.

## 5. The Windows box (`ssh win`, `/c/w/p196-ffi-44891`, hand-written C only), 20:56:56 to about 21:02, and 22:46 to 22:47

The box answered at 20:56:56 (MINGW64, clang 23.1.1, `C:` 21G free). No clang
was running (`tasklist`, both times); each command ran one clang at a time.
Nothing removed. Files: `bcr.c`, `bcr2.c`, `sal_pp.c`, `gks.c`, `s7g_win.c`,
`s7g_win2.c` (copies in `p196ffi/win/`).

- **`BCryptFinishHash` takes a count**: Windows SDK 10.0.26100.0,
  `shared/bcrypt.h:1545`: `BCryptFinishHash(_Inout_ BCRYPT_HASH_HANDLE hHash,
  _Out_writes_bytes_all_(cbOutput) PUCHAR pbOutput, _In_ ULONG cbOutput, _In_
  ULONG dwFlags)`. Called right (`bcr.c`, `-lbcrypt`): status 0, digest of
  `abc` opens 186 and closes 173. **Called with `cbOutput 1` into a one-byte
  cell of a `{m, pad[7], after}` struct** (`bcr2.c`): status `0xc000000d`
  (STATUS_INVALID_PARAMETER), the cell 0, `after` 7: **the library checks its
  count and refuses**, which OpenSSL's `SHA256_Final` cannot do (no count).
- **The SAL words are invisible to clang**: `clang -E` of `bcrypt.h` prints the
  declaration as `BCryptFinishHash( BCRYPT_HASH_HANDLE hHash, PUCHAR pbOutput,
  ULONG cbOutput, ULONG dwFlags);` (`sal_pp.c`): under the default macro set the
  annotation expands to nothing, so an S5 reading clang's AST sees nothing on
  Windows; reading SAL would mean parsing header text (unrun, and a second
  parser of C is what §4.19's *"no libclang, no external tool"* refused).
- **A digest API without a count**: `grep -l -E 'MD5Final|MD4Final|A_SHAFinal'`
  over `um/*.h shared/*.h` of the SDK: none. **The shape itself is on
  Windows**: the SDK's 1,814 headers in `um`, `shared`, `ucrt` carry
  `_Out_writes*(` 5,828 times; 141 of those annotations state the extent as a
  number (`_Out_writes_(1)` 48 of them); among the numbered ones
  `WinUser.h:5939` `GetKeyboardState(_Out_writes_(256) PBYTE lpKeyState)`,
  `Shlwapi.h:538` `PathBuildRootA(_Out_writes_(4) LPSTR, int)`, `sql.h:698`
  `SQLError(..., _Out_writes_(6) SQLCHAR *Sqlstate, ...)`, `ucrt/mbstring.h:977`
  `_mbccpy(_Out_writes_bytes_(2) unsigned char *, ...)`.
- **`GetKeyboardState` through one cell, the C a `@ks: u8` lend emits**
  (`gks.c`, `-luser32`, the cell followed by 511 bytes of 0xAA): `ok 1`,
  **255 guard bytes changed, the last at index 254**, exit 0. 256 bytes through
  a one-byte cell, on the fourth platform, silent.
- The guard-page form of S7 and S4 on Windows: §4's table.

**Correction, 21:12**: the third bullet first said *"141 with a literal extent
and no count parameter"*; 141 counts the numbered annotations, and whether
each function also has a count parameter was not counted.

## Found beside (to file; the synthesis's to class)

1. **An `@` lend of an array ELEMENT hands C a temporary, so a C library that
   keeps the address breaks, and a buffer correctly sized gets one byte.**
   `ir/inout.hero` copies `@xs[i]` into a C local and stores it back
   (*copy in, copy out*). zlib: `deflateInit_(@zs[0], ...)` then
   `deflateReset(@zs[0])` and `deflateEnd(@zs[0])` print `0 -2 -2`
   (`Z_STREAM_ERROR`) where the same calls on a local print `0 0 0`
   (`p196ffi/ident/`), `check` 0 and `run` 0 on Darwin arm64 and Linux arm64;
   on Linux `run --sanitize` exits 1 with **LeakSanitizer: 268,096 byte(s)
   leaked in 5 allocation(s)** (that they are zlib's deflate state is an
   inference: the report's stacks were not read), the local 0 with no report. And `@md: u8` lent `@b.m[0]` of a `u8[32]` field leaves `m[31]` 0:
   C's 31 bytes go past the one-byte temporary (Mac 0, Linux 135). A wrong
   result and a leak at exit 0 is `blocking` by
   `.claude/rules/verification.md`'s list. Searched `issues/` for *zlib*,
   *z_stream*, *element lend*, *WriteBack*, *temporary*, *identity*,
   *address*: 091 (a fixed-array element written through a cell) and 011 (two
   arguments of one place) are other shapes; a question, not proof it is
   unfiled.
2. **Whether 396 is seen at all is the platform's accident**: `host_one.hero`
   is 134 with a false panic on the Mac and **0, silent, `after` 1650537270 on
   Linux arm64**; `s6_16.hero` 134 on the Mac, 0 on Linux; `today_elem32.hero`
   0 on the Mac, 135 on Linux.
3. **The `guess` fixes write the fault** (the compiler-engineer's finding,
   cited, not re-run here): the pointee reader proposes `@md: u8` for
   `unsigned char *md`; under S2 it proposes rewriting a stated extent to the
   field's length. Any route owes the fix the extent, or no fix.
