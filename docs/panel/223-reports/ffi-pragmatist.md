# Panel 223, ffi-pragmatist

Seat: the C a real binding needs, veto on breaking the C ABI (design.md §1.11, §4.19). Written as
the work goes; every number carries the command that made it, anything not run says *unrun*.

**Where I worked**: my copy `.claude/worktrees/223-ffi-pragmatist/` (`git -C <lane> archive
3c478500b | tar -x`), stage 1 built there, `/usr/bin/time -p clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`: `real 7.99`; `./heroes --version`: `heroes 0.4.0`, `sources
5f45d6514b22d650`. Probes in that copy's `ffi223/` (the harness `routes.c`, the record headers
`rec_*.h`, the census `census.py`, the MSan probe `msan_lend.c`, a Heroes program `hbf/`). The
Windows box: `/c/w/223-ffi-pragmatist`, the same tree's `seed runtime selfhost` and my probes,
stage 1 built there with `seed/README.md`'s line (`clang -I runtime seed/heroes.c runtime/runtime.c
-Wl,/STACK:67108864 -o heroes.exe`, `real 0m25.179s`, `sources 5f45d6514b22d650`).

**Compilers**: this Mac, Apple clang 21.0.0 (`arm64-apple-macos`); `heroes-linux-arm64:latest`
(`338a1044ac90`), Debian clang 22.1.8, aarch64; `heroes-linux:latest` (`6f78598a4eba`, amd64,
emulated on this Mac), Debian clang 22.1.8, x86_64; the Windows box, clang 23.1.1,
`x86_64-pc-windows-msvc`. "The project's flags" below is `flags()` of `selfhost/cli/flags.hero`
copied word for word into `ffi223/flags.txt`, and every compile adds `-Werror`, so a single clang
warning anywhere fails the build.

## 1. The bytes C itself writes when it stores a bit-field (item 2, per access unit)

`ffi223/bf_store.c`, `struct bf` and `void set_c(struct bf *p) { p->c = 2; }`, `clang -target <t>
-O2 -S -o -` (Apple clang 21.0.0):

| target | `set_c` | bytes C reads and writes back |
|---|---|---|
| `x86_64-unknown-linux-gnu` | `movzwl 1(%rdi)` … `movw %ax, 1(%rdi)` | 1, 2 |
| `aarch64-unknown-linux-gnu` | `ldurh w8, [x0, #1]` … `sturh w8, [x0, #1]` | 1, 2 |
| `arm64-apple-macos14` | `ldurh w8, [x0, #1]` … `sturh w8, [x0, #1]` | 1, 2 |
| `x86_64-pc-windows-msvc` | `andl 4(%rcx), %eax` … `movl %eax, 4(%rcx)` | **4, 5, 6, 7** |

**The landed compiler on the Windows box writes inside that unit.** `heroes.exe build
ffi223/hbf/main.hero --emit-c` there (a `partial` record over `struct bf`, from `mk_bf()` by value
into a field, lent): `hero_pad_t_0[] = {offsetof a, sizeof a, (0) + 4, 1, (0) + 4, 2, offsetof d,
sizeof d}`, against `(0) + 1, 1, (0) + 1, 2` from the same command on this Mac. Over that table the
runtime's own `hero_pad_gaps` (verbatim in `ffi223/r1_verbatim.inc`, `sed -n 502,529p
runtime/parts/lend.c`) gives `(1,3) (6,2)` on the box (`routes_w.exe`, below). So **R1 as landed
zeroes bytes 6 and 7 on `x86_64-pc-windows-msvc`, inside clang's 32-bit read-modify-write of `b`
and `c`**. The critic's item 9, measured on the platform.

**A second CI leg does the same for a `volatile` bit-field.** `ffi223/units.c`, store width of
`p->x = 5` (`clang -target <t> -O2 -S -emit-llvm`, `bash units.sh`; Apple clang 21.0.0 and Debian
clang 22.1.8 agree on every cell):

| target | `{unsigned x:3; int z;}` | `{volatile unsigned x:3; int z;}` | same, `unsigned :0` after `x` | `{unsigned x:3; char k; int z;}` |
|---|---|---|---|---|
| Linux x86-64 | i8 | volatile i8 | volatile i8 | i8 |
| **Linux arm64** | i8 | **volatile i32** | **volatile i32** | i8 |
| Darwin arm64 | i8 | volatile i8 | volatile i8 | i8 |
| Windows MSVC | i32 | volatile i32 | volatile i32 | i32 |

On Linux arm64 a `volatile` bit-field is stored with its declared type's width (the AAPCS rule;
clang's `-faapcs-bitfield-width`, my reading of the name, its default not asked), bytes 0 to 3,
and bytes 1 to 3 are whole padding R1 zeroes. **So the reason R1 kept to whole bytes, a lend that
never writes inside a C store, is false today on two of the four CI legs, before any route.**

What that write can and cannot do, single thread or not: R1 writes only bits no bit-field holds. A C
thread's read-modify-write racing it can bring back the old padding (R1's zero lost); it cannot
lose a bit-field's value, because R1 never writes one. That asymmetry is the whole of my item 2
answer below.

## 2. The real headers this touches (census)

`ffi223/census.py` (`python3 -I census.py clang <include root>`): every header under the root with a
line matching `:[[:space:]]*[0-9]+[[:space:]]*;` (the critic's spaced pattern, item 16), included
alone, `-fdump-record-layouts-complete`, each complete record's named bit-field bits counted per
byte. A header that does not compile alone is skipped, so both counts are floors.

| C library | headers matched | compiled alone | records with a bit-field | with a byte shared by named bits and unheld bits |
|---|---|---|---|---|
| macOS SDK `usr/include` (Apple clang 21.0.0), 1.19 s | 54 | 34 | 68 | **3** |
| `heroes-linux-arm64:latest` `/usr/include` (clang 22.1.8), `real 0m3.345s` | 282 | 125 | 145 | **29** |

Named in the output (`ffi223/census-macos.txt`, `census-linux-arm64.txt`): **`struct
curl_hstsentry`** (`curl/curl.h`, both platforms, byte 16: `includeSubDomains:1` and seven unheld
bits), `struct apr_uri_t` (`apr-1/apr_uri.h`, byte 82, three one-bit flags), **`struct obstack`**
(glibc `obstack.h`, byte 80: `use_extra_arg:1`, `maybe_empty_object:1`, `alloc_failed:1`), `struct
tcp_info` (`linux/tcp.h`, byte 7), eight `linux/cdrom.h` DVD structs, `snd_ctl_elem_value` and three
more ALSA records, `bpf_prog_info`. **2099 is not a probe's shape; libcurl, glibc and APR ship it.**

## 3. The C each route writes, and the test that every named bit-field is kept

The harness is `ffi223/routes.c` over a record header naming the record's non-bit-field members and
its named bit-fields (`rec_bf.h`, `rec_hsts.h` over the real `<curl/curl.h>`, `rec_obstack.h` over
the real `<obstack.h>`), the two lists the layout round already reads from the dump
(`emit/membership.hero`). What each route writes, as the emitter would:

- **R1 as landed**: the table of `offsetof`/`sizeof` spans plus one `n` span per named bit-field,
  `hero_pad_gaps` once before `main`, `hero_pad_fill` at the lend (verbatim).
- **Route A**: one function per record the emitter writes, run once before `main` beside
  `hero_pad_gaps`, `{ R m; memset(&m, 0, sizeof m); m.F--; keep[i] |= bytes of m }` for each named
  bit-field `F`; a runtime function keeping the bytes whose mask is neither 0 nor 0xff (`at[]`,
  `mask[]`); at the lend, `hero_pad_fill` then `b[at[i]] &= mask[i]`.
- **Route B**: `R t; memset(&t, 0, sizeof t);` then `memcpy(&t.M, &p->M, sizeof t.M)` for each
  member that is not a bit-field (an array such as `expire[18]` cannot be assigned) and `t.F = p->F`
  for each bit-field, then `memcpy(p, &t, sizeof t)`.
- **Route B-run** (the critic's narrowing): R1's fill, then the same temporary with only the
  bit-fields copied in, and only the run's bytes copied back.

The test: 200,000 objects of random bytes per route (a fixed xorshift seed), the route applied,
then for every byte: a member's byte unchanged, the named bits (`keep`) unchanged, the unheld bits
zero; and every named bit-field read back `==` its value before. The mask is checked against a
second oracle that **writes no bit-field**: each bit set alone in a zeroed object and every named
bit-field read (`keep_by_reading`). Command, per record: `clang <the project's flags> -Werror -O0 -g
[-Iinc] -DREC='"rec_X.h"' routes.c -o routes && ./routes`.

| platform | record | compiled under the project's flags + `-Werror` | R1 alone: unheld bits left set (byte-instances) | A | B | B-run | `m.F--` mask = read-only oracle |
|---|---|---|---|---|---|---|---|
| Darwin arm64 | `struct bf` | yes, 0 diagnostics | 348,625 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Darwin arm64 | `curl_hstsentry` (curl 8.7.1, SDK) | yes | 198,443 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Linux arm64 | `struct bf` | yes | 348,625 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Linux arm64 | `curl_hstsentry` | yes | 198,443 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Linux arm64 | `struct obstack` (glibc) | yes | 193,862 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Linux x86-64 | `struct bf` | yes | 348,625 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Linux x86-64 | `curl_hstsentry` (the image's own header) | yes | 198,443 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Linux x86-64 | `struct obstack` | yes | 193,862 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Windows MSVC | `struct bf` | yes | 348,435 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |
| Windows MSVC | `curl_hstsentry` (SDK header shipped) | yes | 198,443 | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 | yes |

`0 / 0 / 0` is named bits changed, bit-field values changed, member bytes changed; for A, B and
B-run unheld bits left set was 0 in every row. The masks each platform printed: `struct bf` keep
`1:0xe7 2:0x01` (Itanium) and `4:0xe7 5:0x01` (MSVC); `curl_hstsentry` `16:0x01` everywhere, R1's
gaps `(35,5)` (Itanium) and `(17,3) (38,2)` (MSVC: the `unsigned:1` takes a 4-byte unit there and
`expire` moves to 20); `obstack` `80:0x07`, gaps `(52,4) (81,7)`.

**MemorySanitizer**, `heroes-linux-arm64:latest`, `ffi223/msan_lend.c` (panel 215's probe flow in
C: `mk_R()` returns a record whose members alone it wrote, stored in a holder's field, lent to
`write(2)` on `/dev/null`), `clang <the project's flags> -Werror <O> -g -fsanitize=memory
-DREC=… msan_lend.c && ./m <route>` (`bash linux_run.sh` with `RECS=none`):

| record | level | no zeroing | R1 | A | B | B-run |
|---|---|---|---|---|---|---|
| `struct bf` | -O0 | 1 report | **1 report, `offset 1 inside [.., 16)`** | 0 | 0 | 0 |
| `struct bf` | -O2 | 0 | 0 | 0 | 0 | 0 |
| `curl_hstsentry` | -O0 | 1 | **1, `offset 16 inside [.., 40)`** | 0 | 0 | 0 |
| `curl_hstsentry` | -O2 | 1 | 1 | 0 | 0 | 0 |

Defect 2099 reproduced in C over libcurl's own struct, at both of `heroes`' levels; every route
closes it in the instrument that found it. (`struct bf` at -O2 reports nothing even unzeroed; not
pursued.)

## 4. Can writing back a byte a bit-field holds change what C reads of it? (`volatile`, `_Atomic`)

**On one thread, no, on all four CI legs, if the mask is clang's own.** Section 3's table is the
measurement: 200,000 dirty patterns per route, platform and record, 0 named bits changed. The mask
must come from clang's codegen and not from the dump's bit column. `ffi223/bf_static.c`, `const
struct bf mb = { .b = 7 }, mc = { .c = 15 }`, `clang -target <t> -S -emit-llvm` (Apple clang
21.0.0): every little-endian target puts `b` at `0x07` and `c` at `0xe0`/`0x01`, and
**`powerpc64-unknown-linux-gnu` puts `b` at `0xe0` and `c` at `0x07`/`0x80`, while its dump still
prints `b 1:0-2`, `c 1:5-8`**. A mask read from the dump's column (the critic's first unlisted
route) would therefore AND byte 2 with `0x01` there and clear `c`'s bit `0x80`. That is arithmetic
over two measured masks; the run on a big-endian machine is unrun, and no CI leg is big-endian.

**Which spelling the emitter can write.** `ffi223/spellings.sh`, seven spellings of "every bit of
named bit-field F set" over nine kinds (`unsigned:3`, `signed:3`, `int:3`, `int:1`, `_Bool:1`,
`unsigned char:2`, `unsigned long long:33`, unsigned and signed `enum:2`), the project's flags,
Apple clang 21.0.0. `m.F--;` and `m.F = 0; m.F -= 1;` give **0 diagnostics**. `m.F = -1;` gives 4
(`-Werror=sign-conversion`), `m.F = ~0u;` 5, `m.F = ~m.F;` 4 (sign-conversion,
`-Wbool-operation`), and both `__typeof__` forms are refused (`invalid application of 'typeof' to
bit-field`). The static-object route needs an initialiser, and an initialiser converts as an
assignment does, so I expect it to hit the same sign-conversion errors. That is an inference: the
initialiser forms are **unrun**.

**`_Atomic`**: C11 6.7.2.1p5 (N1570, fetched from open-std.org into `ffi223/n1570/`): *It is
implementation-defined whether atomic types are permitted.* `ffi223/qual.sh` (`QUAL=1`) on Apple
clang 21.0.0, Debian clang 22.1.8 and the box's clang 23.1.1: `bit-field 'x' has non-integral type
'_Atomic(unsigned int)'`, refused by all three. No header clang reads can hold one.

**`volatile`**: `QUAL=5`, both routes compile on all three clangs. But C11 6.7.3p6: *If an attempt
is made to refer to an object defined with a volatile-qualified type through use of an lvalue with
non-volatile-qualified type, the behavior is undefined.* Route A's `b[at] &= mask` through `unsigned
char *` is such a reference when the byte holds a `volatile` bit-field, and so is route B's final
`memcpy`. R1's writes reach only padding, which is no member's object. **So route A must write
through `volatile unsigned char *`** (the cost is nil in an out-of-line runtime function, by
reading; unmeasured).

**`const`** (not asked, found beside): `QUAL=2,3,4`. `m.F--` and `t.F = p->F` are refused on all
three clangs (`cannot assign to non-static data member 'x' with const-qualified type`), and `struct
q m;` alone draws `-Wdefault-const-init-field-unsafe`. So both routes as written turn a correct
binding's build into a clang refusal. The writes are UB as well, by 6.7.3p6's first sentence. The
census (`census.py`, its `Q` lines, the detector checked on a planted header that it found both
fields of) reads **0 named bit-fields declared `const` or `volatile`** in both C libraries'
headers, as a floor. The guard costs a line all the same: the dump prints the qualifier, so the
emitter can leave such a byte unwritten (R1's behaviour, said) or find the mask with the read-only
oracle.

**Concurrency**: C11 3.14, *memory location: either an object of scalar type, or a maximal sequence
of adjacent bit-fields all having nonzero width*. Its NOTE 2 adds: *It is not safe to concurrently
update two non-atomic bit-fields in the same structure if all members declared between them are
also (non-zero-length) bit-fields*. `b`, the unnamed `:2` and `c` are one memory location, and 5.1.2.4p25
makes two conflicting non-atomic actions in different threads a data race and UB. **Route A writes
that memory location; R1 writes none**, which is the asymmetry of section 1. A racing C store can
undo R1's zero, but it can lose a bit-field's value only to A. Heroes has no thread of its own
(`grep -n -i 'thread\|concurren\|atomic\|volatile' spec/heroes-spec.md` prints nothing), so the race
needs a C library that keeps a Heroes record's address (defect 2112's class) and writes one of its
bit-fields from its own thread while the program lends the record again. I found no such library
among the 32 records the census named, reading their headers only, so that is **a question, not a
premise**. Every lend under such a library would race on any member C reads, not only this one.

**What a route takes from a newer library** (not in the brief; design.md does not cover it, so I
say so). Libraries grow new named bits into a byte's unheld bits. Linux uapi `tcp.h` at v5.4
(`curl raw.githubusercontent.com/torvalds/linux/v5.4/include/uapi/linux/tcp.h`) has `__u8
tcpi_delivery_rate_app_limited:1;` in byte 7. The image's header (`LINUX_VERSION_CODE 396388`,
6.12) has `__u8 tcpi_delivery_rate_app_limited:1, tcpi_fastopen_client_fail:2;`. (v4.9 and v5.5
were refused with `429`, unread.) A C caller never writes bits its header leaves unheld, so a binary
built on the older header keeps the newer library's bit between calls; under A, a Heroes binary
zeroes it before each lend. For `tcp_info`, an output-only struct, that costs nothing. For a struct
a library keeps state in across calls it would, and I found no such case (**unrun**). R1 already
does the same to whole padding bytes, so A does not open the class; it moves it to where libraries
actually grow. The rule's sentence should say so.

## 5. Cost

`ffi223/bench.c`, 20 million lends of `struct bf` per route (the route, then a `noinline` C call
reading a byte), `/usr/bin/time -p ./bench<O> <route>`, this Mac, load 3.31 (`uptime`), one run
each, so noisy:

| level | R1 | A | B | B-run |
|---|---|---|---|---|
| -O0 | 0.23 s | 0.34 s (**+5.5 ns/lend**) | 0.15 s | 0.31 s |
| -O2 | 0.08 s | 0.10 s (+1 ns/lend) | 0.04 s | 0.13 s |

The no-route runs read `real 0.33 / user 0.05` and `0.19 / 0.02`, waiting, and are discarded by the
ratio. B is cheapest only because my harness inlines its per-record function while R1 and A call
the runtime's loops; that is not a property of the route. Lines in `selfhost/`: **unrun** (the
compiler-engineer's).

## 6. Verdicts

| route | verdict | the C it rests on |
|---|---|---|
| **A**, the mask from clang's own bits (`m.F--` on a zeroed object before `main`) | **approve, with conditions 1-4** | section 3: 0 named bits changed in 2,000,000 trials over 10 platform-record pairs, MSan 0 where R1 reports, 0 diagnostics under the flags |
| A, masks from the dump's `byte:lo-hi` column | **veto** | section 4: on big-endian `powerpc64` the dump's numbering and clang's bits differ, so the mask clears `c`'s bit. It rests on a premise about the world (CLAUDE.md § 11, *a narrowing asks the value, never the world*) where clang's own answer costs nothing more |
| A, masks from a static object | object | no generic all-ones initialiser passes `-Werror=sign-conversion` by the assignment measurement; the initialiser itself unrun |
| **B**, rebuilt through a zeroed temporary, every byte copied back | **object** | it writes every member's bytes (a mutex's, an atomic's), which is exactly what R1 refused (`emit/padding.hero` lines 7-9); `const` members refused by clang; `t.M = p->M` cannot copy an array (`expire[18]`), so it needs `memcpy` per member; a flexible array member's in-`sizeof` bytes would be overwritten (by reading, unrun) |
| B narrowed to the run | approve as equivalent | the same writes as A on the same bytes (section 3), but it needs every bit-field's C name, macro-guarded, at each lend site; its cost is A's within noise (0.31 s against A's 0.34 s at -O0, 0.13 against 0.10 at -O2, one run each) |
| **C**, left open | **object** | libcurl, glibc, APR and Linux ship the shape (census: 3 records on macOS, 29 on Linux, floors), MSan reports R1's lend at offset 16 of `curl_hstsentry` at both levels, and no rule stops 2099 counting (critic item 12) |
| **D**, refused | **veto** | it would refuse lending `curl_hstsentry`, `obstack`, `apr_uri_t`, `tcp_info` and the rest, which spec line 383 tells the reader to bind `partial`, so each needs a C shim: bindings categorically harder (design.md §1.11, §4.19) |
| zeroing at the source | object | it misses a C callee storing the whole struct through the `@` pointer from its own uninitialised local, and a kept address (2112's class); by reading, unrun |
| one route at the lend, another at 2112's store | approve A at both | at the store Heroes already writes every member byte, so A there adds nothing |

**Conditions on A** (each a property of the emitted C I could write and compile today):

1. The mask is clang's answer at run time, `m.F--` on a zeroed object or the read-only oracle,
   **never the dump's bit column**. The `n` byte spans the dump gives stay as they are; they are
   endian-agnostic.
2. A byte holding a `const` named bit-field gets no `m.F--` and no write (the dump prints the
   qualifier). Otherwise the build of a correct binding is a clang refusal.
3. The lend's AND goes through `volatile unsigned char *` (6.7.3p6).
4. `.claude/rules/generated-c.md`'s sentence says what changed: the lend and 2112's store now write
   inside a byte a named bit-field holds, putting the named bits back as they were. It also names
   the residue: a C thread writing a bit-field of the same run through a kept address races
   (5.1.2.4p25), and a newer library's bits in an older header's unheld bits are zeroed. And it
   records that R1 already writes inside C's access unit on MSVC and, for `volatile` bit-fields, on
   Linux arm64 (section 1). It also catches up to 2112's store site (critic item 4).

**What would change my verdict on A**: a C library that keeps a Heroes record's address and writes
a bit-field of the shared byte from its own thread (then A's lend writes become a lost update that
R1 never caused, and I would move to B-run at the stores alone plus a stated residue); any named bit
changed on a CI leg under the conditioned route; or a bit-field kind in a real header for which the
conditioned route emits C that clang refuses or warns on.

## 7. Prediction (falsifiable)

**Under route A with conditions 1-4, a Heroes program binding libcurl's `struct curl_hstsentry`
(`partial`, naming `name`, `namelen`, `expire`) that stores one C returned by value in a field and
lends it to C runs MemorySanitizer-clean at `-O0` and `-O2` in `heroes-linux-arm64:latest`, with no
line of its `extern` group changed and no C shim. Under the landed R1 the same program reports
`use-of-uninitialized-value` at offset 16 of 40.** I measured this in C (section 3, MSan table); in
Heroes it is unrun. Also, on the Windows leg, the same program's lend writes bytes 17 to 19 and 38
to 39 whole and ANDs byte 16 with `0x01`, and `includeSubDomains` reads back unchanged.
