# Panel 253, ffi-pragmatist

Seat folder `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-ffi-pragmatist/`
(called `fp/` below), a `git archive` of `181555e9a32d0b18878f9f10e48d2eadd939c285`, **20,037 files** by
`find $F -type f -not -path "$F/tmp/*" | wc -l` (the brief's count). Started 16:40 CET (`date`), load
8.20 (`uptime`). Written as it goes; every block below is whole.

## Premises of the briefs found false

Each with the command that shows it; the block that uses it says more.

1. **The compiler-engineer's *"An inlined frame at `-O2` is lost, measured: `middle` and `outer` inlined
   into `main` ... `atos -i` over the same address gives `deep.hero:14` and no inlined frame, so
   `-gline-tables-only` as built gives no inline chain here"*** (`compiler-engineer.md` § Question 5).
   At `-O2` `main` calls `outer` (`bl 0x1e634 <h_deep_outer>` in Linux arm64, `llvm-objdump -d`; four
   `T` symbols in this Mac's binary, `nm`); `pick` and `middle` are inlined **into `outer`**. At the
   abort site inside `outer`, `xcrun atos -i` and `llvm-symbolizer --inlines` both give `deep.hero:3`,
   `:6`, `:10`. The chain is in the line tables; (W) loses it by skipping the frame that holds the abort,
   and `main`'s address was the wrong one to ask. (Q5.1)
2. **The compiler-engineer's *"`dladdr` taking the loader's lock ... Unmeasured"***: measured. glibc 2.41's
   `dladdr` waits while another thread is inside `dlopen` (still waiting after 5 s, exit 3); Darwin's
   returns at once (`fp/lx/dladdr/hang.c`). (Q5)
3. **`00-shared.md` § Question 5, *"`aarch64-linux-gnu` `-mframe-pointer=non-leaf`"*** holds for this
   Mac's driver only; the Linux arm64 image's clang 22.1.8 says **`non-leaf-no-reserve`** (`clang -### -O2
   -c f.c`). The records in non-leaf functions are kept either way, and `-fno-omit-frame-pointer` there
   gives a byte-identical binary (`cmp`). (Q5.1)
4. **`00-shared.md` § Question 3's *"What would have to be true ... a check that tells a secret-dependent
   branch or memory index in the emitted binary ... and a compiler that keeps constant-time source
   constant-time at `-O2` ... Neither is measured"***: both measured now. The check exists on Linux
   (MemorySanitizer over the build's own words, a control reported at every byte) and not on Darwin
   (*"unsupported option '-fsanitize=memory'"*); the compiler keeps a masked source free of secret
   branches at `-O2` (0 reports, 0 `b.vs`) and does not at `-O0`, where Rebel's own `word.shift_right`
   branches on a secret sign bit (`tbz x8, #0x3f`). (Q3, H)
5. **The milestone page's measurement of route (S)** (decision 29, *"the Ed25519 public key of RFC 8032's
   first vector equal, a ChaCha20-Poly1305 seal opened back"*, carried in `00-shared.md` § Question 3)
   checked a public key and a round trip, never a signature against RFC 8032: CryptoKit's Ed25519
   signatures are not deterministic (two differ; neither is RFC 8032's), which Rebel's
   `keypairs.hero:96` and its test at `:485` assume. (Q3, S)
6. **The robust arm of Q1** (`bw/arms/design-r.md`), as the brief hands it: its new *"nothing in the
   compiler or the runtime grows with Rebel"* (39,110 lines of `runtime/` since `89a33de42^`, `git diff
   --shortstat`), its kept line 573 (*"every one of those needs comes from C"*), and its table's
   preface against its own `SQL` row (`sql/sqlite` binds `libsqlite3`). (Q1)
7. **`ffi-pragmatist.md` Q3.2, *"run a valgrind taint in Linux arm64 if the image has valgrind"***: it
   has none (`command -v valgrind`: nothing); MemorySanitizer took its place, and clang 22's default
   eager checks had to be turned off (`-fno-sanitize-memory-param-retval`) for its reports to be
   branches rather than returned values. Not false; a condition the image does not meet.
8. **`ffi-pragmatist.md` Q1's account of where Rebel's system calls stand** (the runtime's `hero_os.h`, the
   layer, four system headers; asked as a question): there is a fourth, C loaded by `dlopen` inside the layer on Linux, typed by hand, about 151 function-pointer
   slots in four members, checked against no header. (Q1)
9. **`00-shared.md` § Where each seat works: *"Every cost is asked in instructions retired: `/usr/bin/time -l
   <command>` prints `instructions retired`"*** says what it prints and not whose instructions: **the
   process it starts, not that process's children**. Measured on this Mac: one `clang -c` of
   `hero_crypto.c` run directly, 75,322,656; the same run forked by `sh -c "...; true"`, 25,843,208 (the
   shell alone); exec'd by `sh -c "exec ..."`, 96,868,264; and clang's driver figure (about 75 M) is the
   same whether the compile succeeds or fails at once, its `-cc1` (1,231,662,691 counted alone) being a
   child. So a `heroes build` or `heroes test` figure counts the compiler's own work and none of the
   clang compiles and links it runs; a seat comparing a build with a compile must count each process on
   its own. My own first reading of route (S)'s cost fell into it and is withdrawn in Q3.

## Question 1: design.md §1.11's sentence, my charter

**Built**: stage 2 in `fp/`, `./heroes-next --version` prints **`sources 23c7d2a626b29fd5`**, the frozen
key (`fp/tmp/stage2.log` ends `wrote heroes-next`).

**The experiment, a real binding under the new rule.** Rebel's own SQLite floor
(`packages/rebel/0.1.0/files/sql/sqlite/hero_sqlite3.hero`, `extern "sqlite3.h" link "sqlite3"`, 42
functions and 43 constants) reached by a program through a `path` entry (`fp/q1/good/`, the program
`examples-rebel/reach_sql_sqlite.hero` copied): **builds, exit 0, prints `1 Ziggy`, `2 Aladdin`,
`false`**. The same tree cloned (`fp/q1/rebel-mut/`) with one declaration made wrong,
`function sqlite3_column_count(statement: Compiled) -> u32` where the SDK's `sqlite3.h:5201` says
`int`: **refused, exit 1**:

```
error[ffi_return_type]: `sqlite3_column_count` returns `int` in `sqlite3.h`, not `u32` — clang read the header
  at ../rebel-mut/sql/sqlite/hero_sqlite3.hero:114:5
  fix (guess): declare the result `i32`
```

So a package's binding is held to its header exactly as a program's is, and neither arm moves a byte
of the C boundary: no layout, no ownership mark, no `str` termination, no refcount, no marshalling.
That is why this block is not a veto.

**Rebel is the FFI's largest client, counted** (`grep -rln '^extern '` over Rebel's files: **23 files**;
an `awk` over each group, `fp/tmp/q1-extern-counts.txt`): **33 `extern` groups declaring 433 C
functions and 453 constants**. 86 of the functions bind C the system ships: `sqlite3.h` 42 and
`string.h`'s `memcpy` (`sql/sqlite`), `math.h` 41 (`math/libm`), `stdlib.h`'s `malloc`/`free`
(`threads/internal/lives`). The other 347 bind the project's own headers, `hero_os.h` (the runtime)
and 15 headers of the platform layer (`runtime/platform/`, 16 `.c` members).

**Where every system call Rebel makes stands, answered as the question it is** (by the `grep`s above
and `selfhost/cli/layer.hero:1-23`, read): **three homes**, never a package's own C (Rebel ships none,
`find ... ! -name '*.hero' ! -name '*.ledger'` empty): (i) the runtime's `hero_os.h`, process, files,
directories, spawning and threads (`layer.hero:21-23`); (ii) the platform layer's 16 members, sockets,
clock, TLS, cryptography, ODBC, serial, keyring, watch, ipc, poll, terminal, disk, sys; (iii) two C
libraries bound directly by an `extern` group, SQLite and libm, plus three libc functions. **And a fourth
the brief does not name**: inside the layer, on Linux, **C loaded at run time by `dlopen` and typed by
hand**, `libcrypto.so.3`/`.so.4` (`hero_crypto.c:2398`), `libssl` (`hero_tls.c:2441`), `libodbc`
(`hero_odbc.c:557`), `libsecret-1.so.0` (`hero_keyring.c:1207`): **about 151 function-pointer slots**
(crypto 49, TLS 45, ODBC 38, keyring 19, by a `grep -c` of slot declarations), most typed `void *`
(`hero_crypto.c:2323-2375`), checked against no header. design.md §1.11's point 3 (*"a wrong type in an
`extern` is a compile error"*) covers `extern` groups; **the document does not cover a member's
run-time-loaded library**, and no sentence of either arm does. Not this sitting's to repair; named
because Q3's routes (S) and (V) are judged on it below.

**The robust arm, read against the tree** (`bw/arms/design-r.md`):

- **Its new sentence *"nothing in the compiler or the runtime grows with Rebel"* is false at the frozen
  commit.** `git diff --shortstat 89a33de42^ 181555e9a -- runtime/` (from M-rebel-library's step 3,
  *"the platform layer under them"*): **36 files, 39,110 insertions**. Every member but `hero_net.c` and
  `hero_time.c` (M-core-packages step 14, `85d6f1000`) was born in M-rebel-library's steps 3, 6 and 23
  (`git log --diff-filter=A`), written so a Rebel package could stand on it; the layer is 42,290 lines
  with the headers (`wc -l`). **And every program's build compiles it**: `print(1)` built by stage 2
  (`fp/q1/hello/`) writes `build/layer-....a`, **947,776 bytes, 17 archive entries**, and the binary
  holds **0** layer symbols (`nm | grep -c`): the linker keeps nothing a program does not call, as
  `layer.hero:12-16` says, so *"a program that takes none of it carries none of it"* holds of the
  binary and not of the build. What the sentence can truthfully say: *the compiler takes none of
  Rebel; Rebel's platform C is the runtime's layer, C of the project's own, compiled with every build
  and linked into a program only where it calls it.*
- **Line 573 keeps *"The rule above says every one of those needs comes from C"***, which the arm's own
  new box falsifies (`grep -n` of `design-r.md`).
- **The table's preface, *"written in Heroes where it reads data from outside and over the system's own
  library where the platform has one (decision 24)"***, is false on its own `SQL` row: `sql/sqlite`
  binds `libsqlite3` by `link "sqlite3"` (Rebel's README `A1` row 5: *"SQLite over its library"*), C
  that reads a database file from outside, not written in Heroes; decision 24 already has a measured
  exception in Rebel. The preface should say Rebel's rule as Rebel's: *"over the system's library or a
  C library where Rebel's README names one"*.
- **Decision 24 must stay Rebel's, never a program's.** The arm writes it into the table's preface
  about Rebel's packages (good); the register and Part 2's bullet must not turn it into *"a program
  never binds a C library that reads data"*. A program binding cJSON, PCRE2 or zlib itself is §4.19's
  own fence and stays first-class. **That is the one line on which I would veto**: a rule refusing a
  program's binding is a binding made categorically harder.
- **`FFI ergonomics rank alongside comprehension, not below it`** stands unchanged in both arms
  (`design-r.md:598`, `design-c.md:593`), and it should: its premise was *"everything comes from C"*,
  and the measured premise now is *"every package that reaches the platform reaches it through the
  FFI"*, 433 declarations of Rebel's own. The words *"from a package it carries or from a C library
  through the FFI"* demote nothing if the box adds that clause; without it, a reader of the box sees
  two doors where there is one door and a package standing behind it.

**The conservative arm** leaves *"That is the whole standard library ... expected to arrive via FFI"*
over a table naming PCRE2 where Rebel's `regex` is Heroes (`[ -f regex.hero ]`), and the core
paragraph's `use <core>/strings` for a package decision 8 deletes: sentences a reader then finds false.
It is cheaper (+88 against +333 vendored, carried from `00-shared.md`) and keeps those false sentences; robust keeps three too, all
repairable in place.

**My definition's first sentence under the robust arm** (the author's to amend,
`.claude/agents/ffi-pragmatist.md:3`): *"Panel judge for the founding constraint (design.md §1.11,
§4.19: Heroes imposes no standard library; what a program needs comes from a package it carries or
from a C library through the FFI, and every package that reaches the platform reaches it through the
FFI, held by clang to the real header). Input MUST include the C a real binding would need under the
proposal; compiles it or shows why it can't. Has veto power on ABI breakage."* And its first paragraph's
quotation of the old box replaced by the new one.

- `verdict`: **approve** the robust arm **with four repairs** (the leverage sentence, line 573, the
  table's preface, a clause in the box naming the FFI as every package's door to the platform);
  **object** to the conservative arm (its standing table and core paragraph are false); **veto
  reserved** for any wording of decision 24 that reaches a program's own binding.
- `section`: design.md §1.11 (the box, point 3 of *"The tension ... is resolved"*, *"FFI ergonomics
  rank alongside comprehension"*), §4.19 (the `extern "sqlite3.h"` fence); the document does not cover
  a platform member's `dlopen`ed library.
- `experiment`: Rebel's `sql/sqlite` floor built through a `path` entry, exit 0, the right rows; its
  clone with `sqlite3_column_count -> u32` refused `error[ffi_return_type]` by clang reading
  `sqlite3.h`, exit 1 (`fp/q1/good/build.out`, `fp/q1/bad/build.out`). Clang accepted the true binding
  and refused the false one.
- `argument`: Rebel moves no byte of the C boundary: its 33 `extern` groups, 433 functions and 453
  constants, are checked against their headers exactly as a program's are, measured by a wrong
  `sqlite3_column_count` refused inside the package. So the box may name a package beside the FFI
  without demoting it, provided the box says the package reaches the platform through the FFI. What
  the robust arm gets wrong is the runtime: 39,110 lines of platform C were written under Rebel, and
  every build compiles them; the arm's *"nothing in the runtime grows with Rebel"* is false today.
- `prediction`: at the landing of the robust arm, `git diff --stat` of the landing commit names no file
  under `selfhost/`, `runtime/` or `seed/`; and Rebel's `sql/sqlite` with any one of its 42 `sqlite3_*`
  results declared one integer width narrower than `sqlite3.h` says is refused `ffi_return_type`, as
  `sqlite3_column_count` was, with no shim written.
- `condition`: a measurement that a fetched (not `path`) package's `extern` group escapes the header
  check (then the box's *"verified ... by clang against the header it names"* is false for packages and
  I object to the arm); or a register row turning decision 24 into a refusal of a program's own
  binding (veto).

## Question 3: the system's cryptography on macOS

**The instrument, since the image has no valgrind** (`command -v valgrind` in `heroes-linux-arm64:latest`:
none): **clang's MemorySanitizer in the Linux arm64 image** (Debian clang 22.1.8), the secret bytes
poisoned through an `extern` door of my own (`fp/lx/taint/fp_secret.{h,c}`: `fp_secret(@data: [u8]
counted_by n lent, n: u64)` calls `__msan_poison`), the program's emitted C compiled with the build's own
words plus `-fsanitize=memory -fsanitize-recover=memory -fno-sanitize-memory-param-retval` (the last
because clang 22's default eager checks report a poisoned value *returned*, not a branch; with them on, a
report fell on `lend.c:346:5`, which is `return sum;`), every report kept and counted by its
innermost frame (`fp/lx/msan.sh`). MemorySanitizer reports a branch, an address or a system call
computed from a poisoned bit; it instruments LLVM's IR, not the machine code, so the machine code is read
beside it (`otool -tV` here). Linux stage 2 in the container prints `sources 23c7d2a626b29fd5`.
**A control first**: a comparison that leaves at the first differing byte, in the same program, is
reported at every byte (8 reports at `-O0`, 7 at `-O2`, `main.hero:19`), so the poison reaches the bytes.

**H.1, `crypto.equals_constant_time`, the function already in the tree: holds, at both levels, on both
machines read.** Emitted C (`fp/q3/ct/ct.c:9073`): `to_i64(u8)` is lowered to an option whose tag is the
constant 0 (`{.tag = INT64_C(0), .as.ok = (int64_t)t16}`), so each `.must()` tests a constant; the bounds
check of `a[at]` and the overflow check of `at + 1` read `at` and the lengths, which are public. This
Mac's `-O2` machine code: every conditional branch of the loop tests a pointer, `at` against a length, an
element size or an option's tag word stored as `xzr`; the bytes pass through `ldrb`, `eor`, `orr` only,
and the answer is `cmp x22, #0; cset w19, eq`. `-O0`: the same, through `memcpy`s of the option.
**MemorySanitizer in Linux arm64: 0 reports in `compare.equals` at `-O0` and at `-O2`** (`./prog abcdefgh
abcdefgh constant`). So the frozen tree's one constant-time function keeps its word, and no defect of it
is found.

**But the FFI door itself is not silent**: both levels report the runtime's lend guard,
`hero_lend_audit` at `runtime/parts/lend.c:358:63`: a buffer lent to C through `counted_by ... lent` is
summed (`hero_lend_sum`, a multiply-xor over its bytes) and the sum compared with the one kept, at the
lend's end and at each audit. With a secret in the buffer, that comparison is computed from the secret.
Its direction is fixed in a correct run (it differs only when C wrote after the lend ended), so it times
nothing; but **every key Rebel hands the platform layer through `[u8] counted_by n lent` meets it**, and
a checker reports it.

**H.2, a primitive written in Heroes: ChaCha20** (`fp/lx/chacha/chacha.hero`, 86 lines (`wc -l`), every 32-bit
word an `i64` masked with `LOW32` after each sum, since overflow aborts and no wrapping operator exists,
spec lines 210 and 220): **its two RFC 8439 vectors (2.3.2's key-stream block, 2.4.2's sunscreen text)
equal OpenSSL 3.5.7's own `openssl enc -chacha20` in the image byte for byte** (`cmp` of
`fp/lx/chacha/openssl-out.txt` against the Mac's output), at `-O0` and `-O2`.

- **Emitted C** (`fp/q3/chacha/cc.c`): `quarter` holds 4 `__builtin_add_overflow` on two secret words and
  24 index checks on public indices; `rotl` lowers `x >> n` as **`(x < 0) ? ~(int64_t)((~(uint64_t)x) >>
  n) : (int64_t)((uint64_t)x >> n)`**, a test of the word's sign (`selfhost/emit/operator.hero:172`);
  `block`'s output bytes pass `to_u8(...)`, a range test.
- **This Mac's `-O2` machine code**: each sum is `adds x8, x9, x8` then **`b.vs`**, a conditional branch on
  the overflow flag of two secret words, 4 per quarter round; `rotl`'s sign test became one `asr`, its two
  branches testing the public count; the `to_u8` range tests folded away in `block` (the `& 0xff` lets
  clang prove the range), one `cmp #0xff; b.hi` left in `xor_stream`.
- **MemorySanitizer, Linux arm64**: in the cipher, **every report is an overflow check on a sum of two
  secret words**: `quarter`'s four sums (lines 22, 24, 26, 28, 240 reports each over the run's 3 blocks)
  and `block`'s 16 final sums (line 62, 48), **336 per 64-byte block, the same at `-O0` and `-O2`**. The
  sign test, the rotations and the `to_u8` tests draw none: the masked words' high bits are defined zeros,
  and MemorySanitizer tracks bits, so they depend on no secret bit. (The other reports, `hex` and
  `str.c`, are my test program printing the secret key stream through a table lookup, not the cipher.)

**Is an overflow check on a secret a secret branch? Measured, two answers.** To the checker, yes: all 336
per block are reported, and the machine code holds the `b.vs`. To the clock, no: each sum is of two words
below 2^32, so the branch is never taken and its direction does not move with the key. **No checker run
here can tell the two apart**, so a constant-time discipline "the tools check" over today's emission
needs either a checker taught that an edge into `hero_panic_overflow_at` declassifies only *whether the
program aborted* (which an observer sees anyway), or a wrapping add the spec does not have (a language
change, a panel's).

**H.3, Rebel's digests and password hashes owe the same reading, and fail it at `-O0`.** The password
marked secret (`fp/lx/hashes/main.hero`):

| primitive | `-O0` reports, by place | `-O2` reports |
|---|---|---|
| `crypto.sha512` | `word.shift_right` (`word.hero:23`) 1,943; `word.add` 3 sites × 327; `word.add4` 7 sites × 129 to 143 | **0** |
| `crypto.blake2b` | `word.shift_right` 1,168; `word.add` 3 × 186; `word.add4` 7 × 183 to 184 | **0** |
| `crypto.argon2` (argon2i, 64 KiB, 1 pass) | stopped by me at 313,108 reports, its stderr past the rules' bound (below): `shift_right` 106,043, `word.add` 3 × 24,470, `word.add4` 7 × 11,840 to 12,090, `argon2.multiply` 8 × 6,191 | **126,977**, every one in `argon2.multiply`'s overflow checks (`argon2.hero:37`, `:39`, 4 sites × 31,744) |

(Each row also draws the lend guard's one report, above.) **At `-O2` the two digests are clean and Argon2 is not**, because
Rebel writes each 64-bit sum as halves masked at the add site (`word.hero:38-40`), which lets clang prove
no overflow, and clang folds the sign test into `asr`; `argon2.multiply` (`argon2.hero:32-40`, a product of two 32-bit words by 16-bit halves) receives words masked by its caller, so clang, compiling it apart, cannot prove its sums in range and keeps every check. **At `-O0` two kinds remain, and one is a true
secret branch**: `shift_right` (`return x >> n & ~(-1 << 64 - n)`, `word.hero:23`) on a 64-bit word
whose top bit is secret, lowered by `operator.hero:172` to a test of that sign: **this Mac's `-O0`
machine code holds `tbz x8, #0x3f` on `x`** (`otool -tV -p _h_1rebelcryptointernalword_shift_right
fp/q3/ct/ct-O0`), two instruction paths chosen by a secret bit. `-O0` is the level `heroes build` and
`heroes test` compile at by default (`selfhost/cli/verbs.hero:38` and `:136`; `heroes run` `-O2`, `:69`). **That is a defect of the frozen
tree, for the coordinator to file**: every `>>` of a value whose sign is secret compiles at `-O0` to a
branch on that sign, in SHA-512, BLAKE2b and every password hash over BLAKE2b.

**Its repair is one expression of the emitter, compiled here** (`fp/q3/shr/shr.c`): `m = 0 - ((uint64_t)x
>> 63)`, then `(int64_t)((((uint64_t)x ^ m) >> n) ^ m)`, defined C11 for every `x` and `n` in 0..63:
**equal to today's lowering and to clang's `x >> n` on 10,000,000 random pairs, 0 mismatches**, and **0
conditional branches at `-O0` against today's 1** (0 and 0 at `-O2`). Not built into the emitter (a
`selfhost/emit/` change moves every emission's `>>` and owes `emission`, `emit`, `determinism`, `wholes`;
`.claude/rules/verification.md`'s map).

**H.4, the source discipline that reaches `-O2`, measured.** The same ChaCha20 with each operand masked at
its add site, `(s[a] & LOW32) + (s[b] & LOW32) & LOW32`, Rebel's own style (`fp/lx/chacha2/`, five lines
changed): **the same two vectors; MemorySanitizer 0 reports in the cipher at `-O2`, and 336 per block at
`-O0`**; this Mac's `-O2` machine code of `quarter` and `block` holds **0 `b.vs` and 0 `adds`**, its 36
and 32 remaining branches the index checks of public indices. So **Heroes as the compiler emits it today
holds a constant-time discipline at `-O2` only, and only under a discipline of the source that no tool
checks at the source**: a mask forgotten at one add, or a helper compiled apart from its callers (Argon2's
`multiply`), leaves a branch on a secret that the build accepts in silence. At `-O0`, the level `heroes
build` and `heroes test` compile at, it holds for nothing but `equals_constant_time`.

**A note on my own run, owed to the rules**: the Argon2i `-O0` taint wrote MemorySanitizer's reports to an
unbounded file, 531,195,298 bytes when I found it; I killed the run in the container and its loop, kept
the per-place counts (`fp/lx/hashes/argon2i-O0-partial.txt`) and deleted the file, and every run after it
counts reports from a pipe, bounded at 120 s and 65,536 bytes (`fp/lx/msan.sh`). The disk read 64 GiB free
after (`df -h`).

**What (H) owes before a cipher can be written in Heroes**, priced from the above: (1) the `>>` lowering
of H.3, one expression of `operator.hero:172`, an emitter change owing `emission`, `emit`,
`determinism`, `wholes`; (2) a checker that reads the build's own output, MemorySanitizer at the level
the program ships, with an edge into `hero_panic_*` declassified, or a wrapping add in the language, which
is a spec change (spec lines 210, 220) and a panel; (3) the checker on every platform: MemorySanitizer
runs on Linux alone (this Mac's Apple clang 21: *"unsupported option '-fsanitize=memory' for target 'arm64-apple-darwin27.2.0'"*, run), so
macOS and Windows machine code are read by hand or by a binary census, and a timing test of dudect's kind
is **owed to a still machine**; (4) the lend guard's comparison at `lend.c:358` taught to the checker or
skipped for a buffer marked secret. Rebel's `crypto.hero:15-21` refusal stands on all four.

**S, a member in Swift over CryptoKit, built as a library and run, not built into the layer.**
`fp/q3/swift/hero_crypto_swift.swift` (113 lines, `wc -l`): the four primitives the macOS arm lacks, seven
`@_cdecl` functions (`hero_crypto_swift_seal`, `_open`, `_ed25519_public`, `_ed25519_sign`,
`_ed25519_verify`, `_x25519_public`, `_x25519_agree`), with a hand-written C header
`hero_crypto_swift.h`. Built by `xcrun swiftc -sdk "$(xcrun --show-sdk-path)" -parse-as-library
-emit-library -static -O -emit-clang-header-path gen-header.h` (`HOME` and `TMPDIR` in my folder, the
module cache in `fp/tmp/`): exit 0, `libherocryptoswift.a` 21,192 bytes, `nm` shows the seven `T`
symbols.

- **The header check, my standing concern, measured.** `swiftc -emit-clang-header-path` writes C
  declarations of the seven functions (`gen-header.h:370-382`, `Int` as `NSInteger`), **but inside `#if
  defined(__OBJC__)`** (`:369`). So a C compile of the hand header beside the generated one checks
  nothing: with one parameter made wrong (`int32_t text_count` where Swift says `Int`), `clang -x c
  -fsyntax-only` **exits 0**. **Compiled as Objective-C**, the true header passes (exit 0) and the wrong
  one is refused: *`error: conflicting types for 'hero_crypto_swift_seal'`*, exit 1. So route (S) keeps
  *"a wrong FFI signature is a compile error"* **only if the layer adds a step**: the generated header
  written beside the object and one check unit compiled as Objective-C against the hand header. Without
  it, the Swift member is the layer's second hand-typed boundary after Linux's `dlsym` table (Q1).
- **The vectors, against an independent reference.** One harness, `vectors.c`, over two backends: the
  Swift member on this Mac, OpenSSL 3.5.7's EVP in the Linux image (which itself reproduces RFC 8439's
  tag `1ae10b594f09e26a7e902ecbd0600691`, RFC 8032 test 1's signature and RFC 7748 6.1's keys and
  secret). **`diff` of the two outputs: identical in every line but two**: ChaCha20-Poly1305's and
  AES-256-GCM's texts and tags over RFC 8439 2.8.2's inputs, both opens, both bad tags refused (0),
  Ed25519's public key, all three X25519 values. **The two: CryptoKit's Ed25519 signatures are not
  deterministic**: two signatures of one message differ (`two-signatures-equal 0`), neither equals RFC
  8032's (`equals-rfc8032 0`); each verifies, RFC 8032's verifies, a flipped bit is refused. **That
  breaks a promise Rebel already writes**: `crypto/keypairs.hero:96` *"Ed25519, of the message itself;
  deterministic"*, and its test `keypairs.hero:485-496` asserts RFC 8032's TEST 1 to 3 signature bytes,
  which would fail on macOS under (S). Whether CryptoKit documents its signatures as randomized is the
  historian's to source.
- **Under `--sanitize`'s words**: the harness built `-O0 -g -fsanitize=address,undefined` against the
  Swift archive links and runs, exit 0, the same outputs, 0 bytes on stderr. The link needs no flag:
  `otool -L` names CryptoKit, Foundation and nine `/usr/lib/swift/libswift*.dylib` beside libSystem,
  pulled by the Swift object's own autolink entries, as the writer's probe found.
- **What it costs every program, because the layer compiles every member for every build.** Measured
  in instructions retired, **each compiler process counted on its own**: `/usr/bin/time -l` counts the
  process it starts and not that process's children (shown: one `clang -c` direct, 75,322,656; the same
  forked by a shell, 25,843,208; exec'd by it, 96,868,264), so a driver's figure is not its compile's.
  Counted that way (`fp/q3/layercost/count.sh`, each `-cc1` run as its own process with the build's words
  at `-O0`, one run after a discarded warm-up): **the layer's 16 C members 10,001,977,325** (`hero_crypto.c`
  1,231,662,691, the largest `hero_watch.c` 1,577,551,163), **`runtime.c` 2,068,904,472**; the compiler's
  own process in a cold build of `print(1)`, **3,881,321,213** (best of 3, the first of 4 discarded,
  `fp/q3/hellocold*/`); so a cold `print(1)` build retires **at least 15.95 G** (the unit's compile and
  the link not counted). **`swift-frontend` compiling the 113-line Swift member**, its command taken from
  `xcrun swiftc -###` and run as its own process (best of 3, the first of 4 discarded): **1,494,617,649
  with a warm module cache, 13,476,283,683 cold**. So the Swift member adds **at least 9% (warm) and up to
  84% (cold) to every new program's cold build on a Mac**, `print(1)` included, more than its C
  neighbour `hero_crypto.c` (1.21 times it warm, 10.9 times cold), because `layer.hero:12-16` compiles
  every member and lets the linker choose (*"no predicate decides here which program needs which
  member"*). Cold is once per module cache: per machine if the member uses Swift's shared cache, per
  program folder if the layer keeps the cache in `build/` as it keeps every other output. The object is
  18,888 bytes against `hero_crypto.c`'s 107,928. (My first reading, 65% to 373%, set `xcrun swiftc`'s
  process, 2,518,938,968 and 14,478,761,199, against a build count that omits its clang children; it is
  withdrawn.)
- **What `layer.hero`, the key, `heroes doctor` and the link owe** (read in `selfhost/cli/layer.hero`,
  472 lines; priced, not built): `swiftc` found through `xcrun` (bare `swiftc` fails under the CLT
  alone, the critic's finding 10) and named by a `doctor` row; the member compiled with
  `-parse-as-library -emit-object -module-name ... -module-cache-path <build>/...` and the SDK path; the
  key holding `swiftc --version`'s answer and the SDK's path beside clang's answer; the generated header
  and the Objective-C check unit above; nothing on the link line. Either every Mac build pays the
  compile above, or the layer gains the first predicate of which program needs which member, which
  `layer.hero`'s own header refuses. The CI's `macos-26` image: `ci.yml:490-491` says its lldb *"comes
  with the image's Xcode"*; `swiftc` there is unrun, owed to the CI's first run.
- **What it leaves**: Windows' Ed25519 (CNG names none, `hero_crypto.c` note 3), X25519 there
  unmeasured (note 4), every Windows line owed to the box; Linux's 49 hand-typed OpenSSL slots as they
  are.

**C, AES-CTR from CommonCrypto with GHASH beside it: priced, not built.** Its premise compiled and run:
`kCCModeCTR` through `CommonCryptor.h` under `-Werror=deprecated-declarations`, the zero block under
the zero 256-bit key enciphered to `dc95c078a2408989ad48a21492842087` (`fp/q3/routec/ctr.c`, exit 0);
`grep -c -i gcm` of that header prints 0. It closes **one primitive of four** (AES-GCM); ChaCha20-Poly1305,
Ed25519 and X25519 have no CommonCrypto half at all. The constant-time half is then GHASH, a
carry-less multiply in GF(2^128) written in C of the project's own (or in Heroes, where H's findings
apply in full), checkable by the same MemorySanitizer taint on Linux and by reading on macOS. Not built:
H's answer and S's and V's coverage make it the narrowest route.

**V, a small constant-time C library vendored into the layer: priced, not built, none at hand.** No
candidate is on this Mac (`find` over `/opt/homebrew`, `/usr/local`, `~/go`, `~/Library/Caches`,
`~/src`, `~/Developer`, `~/Temp` for Monocypher, TweetNaCl, libsodium, BearSSL, HACL, donna: none
outside the seats' folders) nor in the image but Nettle 3.10.1 (`dpkg -l`), and I downloaded nothing.
**Candidates for the historian to source** (licence, audit, size; from my knowledge, unread here):
Monocypher (ChaCha20-Poly1305 in its IETF form, X25519, Ed25519 with its optional SHA-512 file; CC0 or
BSD-2-Clause; an audit by Cure53), with BearSSL's constant-time AES and GHASH (`aes_ct64`,
`ghash_ctmul64`; MIT) for AES-GCM, which Monocypher lacks; Nettle is LGPL or GPL, which the runtime
exception's terms would have to be read against. **What V keeps that S does not**: C, compiled by clang
as every member is, its header checked by the member's own include (`hero_crypto.c:13-15`), the same
bytes on three platforms (closing Windows' Ed25519 too), a deterministic Ed25519 (RFC 8032's), no new
toolchain, a build cost of the order of the C members' 0.13 to 1.58 G each (an inference: unbuilt) rather than
Swift's 1.49 to 13.48 G, and checkable by the same MemorySanitizer taint as H on Linux. **Decision 24 reaches it only as it
already reaches the layer**: `hero_crypto.c` already reads PKCS #8 and SPKI DER from outside in C
(`hero_crypto.c:47-53`, *"this member reads and writes PKCS #8 and SPKI itself"*), so the layer is a
standing exception to *"never bound from a C library"*, and V is one more member of it.

**The conservative, run on this Mac** (`fp/q3/conservative/main.hero`, one call each, built `-O0`, exit
0): `seal.is_available(aes_256_gcm)` **false**, `(chacha20_poly1305)` **false**, `seal.generate(aes_256_gcm)`
**err unsupported**; `keypairs.is_available(ed25519)` **false**, `(x25519)` **false**;
`keypairs.parse_public_raw(ed25519)`, the door `x509.verify_signature` and `webauthn` read an Ed25519
key through, **err unsupported**; `jose.is_available(eddsa)` **false**; `nats.nkey_sign` **err
unsupported**; `webauthn.parse_public_key` of an EdDSA COSE key (RFC 8032 test 1's key) **err
unsupported**. `web.seal_value` needs a `seal` key, which `seal.generate` refuses, so it is unreachable
(its test returns early, `web.hero:3030-3031`); `keyring/protect` has no function to call
(`keyring.ledger:69-75`, *pending*). **And Rebel's tests of the gap pass on macOS while testing
nothing**: `nats.hero:1648-1651` (*"`.err e => assert e.code == "unsupported"`"*, run alone with `--only`:
`ok`, 796,081,706,537 instructions in the compiler's own process, its clang children uncounted, one run) and `web.hero:3030` (`if !seal.is_available(...)
return`), question 4's silent pass in the shape a platform gives it.

**TLS over a stream the program holds**: no route above closes it; it is Network.framework's
connection model (`tls.ledger:20`, `:47`), not a missing primitive, so only a TLS engine would, which
the author refused in Heroes (2026-11-16) and none of S, C or V supplies.

- `verdict`: **object** to (S) as the route; **object** to (H) as the route today; **approve** (V) as the
  route to build, provisional on the historian's licence and audit; the conservative stands until V is
  built and measured (it keeps 0 lines and an honest `unsupported`). No veto: no route breaks the C ABI.
- `section`: design.md §1.11 (*"FFI signatures are verified by clang against the real header"*, point 3
  of *"The tension ... is resolved"*; the box's *"the C ABI ... Swift `@_cdecl`"*, line 499), §4.19;
  §1.12 for the abort edges. The document does not cover a platform member written in a language other
  than C, nor a constant-time discipline.
- `experiment`: Swift member over CryptoKit, seven `@_cdecl` functions, `swiftc` accepted it; its hand
  header held to Swift's by clang **only compiled as Objective-C** (C: a wrong header exit 0; ObjC:
  `conflicting types`, exit 1); vectors equal to OpenSSL's but Ed25519 signatures (not deterministic);
  ASan+UBSan link clean; `swift-frontend` 1.49 G (warm) to 13.48 G (cold) instructions against the
  layer's 16 C members' 10.00 G and a cold `print(1)` build's 15.95 G or more. ChaCha20 written in Heroes equal to OpenSSL on RFC 8439's vectors, 336 secret branches per block
  by MemorySanitizer at both levels, 0 at `-O2` with the operands masked; `equals_constant_time` 0 at both
  levels; SHA-512 and BLAKE2b 0 at `-O2`, and at `-O0` a true branch on a secret sign bit
  (`operator.hero:172`'s `>>`), whose branch-free C11 repair compiled to 0 branches and matched 10 M pairs.
- `argument`: (S) works but buys a second hand-typed boundary unless an Objective-C check is added, an
  Ed25519 that breaks Rebel's written *"deterministic"* and its RFC 8032 test, and 9% (warm) to 84% (cold)
  more instructions in every Mac program's cold build, `print(1)` included, while leaving Windows' Ed25519
  open. (H) is not holdable today: at `-O0`, `heroes build`'s level, Rebel's own hashes branch on secret
  bits, and at `-O2` only an unchecked source discipline keeps a cipher clean, checkable on Linux alone.
  (V) keeps the boundary C and clang-checked, the same on three platforms, deterministic, and cheap.
- `prediction`: (a) at the landing of a Swift member, Rebel's `keypairs.hero:485` test fails on macOS
  (an Ed25519 signature unequal to RFC 8032's TEST 1) unless its assertion is weakened; (b) with the Swift member in the layer, a
  cold `heroes build` of `print(1)` on a Mac runs a `swift-frontend` process retiring at least 1.4 G
  instructions, beside the 16 `-cc1` processes of today's layer; (c) the emitter's `>>` written as `fp/q3/shr/shr.c`'s
  `shr_flat`, MemorySanitizer's `-O0` reports at `word.hero:23` over SHA-512 of a secret fall from 1,943
  to 0, and the two-stage fixpoint holds.
- `condition`: for (S): CryptoKit offering a deterministic Ed25519 (or Rebel ruling its *"deterministic"*
  per platform) **and** the layer gaining a member compiled only for programs that take it, measured;
  for (H): the `>>` repair landed, a checker that reads the shipped level with abort edges declassified,
  and the discipline checked at the source; for (V): a licence or audit the historian finds unfit, or a
  candidate's AES failing MemorySanitizer's taint in the image.

## Question 5: the platforms of a stack of calls

Linux arm64 in `heroes-linux-arm64:latest` (native; stage 2 there `sources 23c7d2a626b29fd5`), the
x86-64 rows in `heroes-linux:latest` (emulated, not the CI's leg), each its own container in turn
(`fp-253-1`, then `fp-253-2`), only `fp/lx/` mounted, each binary under `timeout -s KILL 20` and its output
under `head -c`. The compiler-engineer's (W) taken from its folder: its `runtime/parts/stack.c` whole
(+64 lines) and, of its `panic.c`, the two lines of (W) alone (its question-4 lines left out,
`fp/lx/w/panic.c`, `diff` against the frozen file: those two lines).

**5.1, frame records and (W)'s rows.**

| platform, clang | `-O2` default (`clang -### -O2 -c`) | `deep` `-O0` | `deep` `-O2` | `-O2` with `-fno-omit-frame-pointer` |
|---|---|---|---|---|
| Linux arm64, Debian clang 22.1.8 | **`-mframe-pointer=non-leaf-no-reserve`** (not this Mac's driver's `non-leaf` for `aarch64-linux-gnu`) | `middle`, `outer`, `main` | `main` alone | **byte-identical binary** (`cmp`), the same walk |
| Linux x86-64 (emulated), Debian clang 22.1.8 | `-mframe-pointer=none` (the compiler-engineer's reading, re-run) | `middle`, `outer`, `main` | **no `called from` line at all** | `main` alone |
| Darwin arm64, Apple clang 21 | `non-leaf` (the brief's) | the compiler-engineer's: `middle`, `outer`, `main` | its: `main` alone | not run |

`deepmust` names `deepmust.main` at both levels and the Rebel case **`main.swap_words` and `main.main` at
both levels** in Linux arm64, as on this Mac. (W) adds **768 bytes of `.text`** there at both levels
(`size -A`: 128,748 to 129,516 at `-O0`, 96,764 to 97,532 at `-O2`). So on x86-64 (W) needs
`-fno-omit-frame-pointer`, one word of `flags.hero`; on aarch64 Linux the word changes no byte, so it
can be every target's at no cost there.

**What an inlined call leaves: the whole chain, in the line tables; (W) drops it.** At `-O2` `main` does
not inline `outer`: Linux arm64's `main` holds `bl 0x1e634 <h_deep_outer>` (`llvm-objdump -d`), and this
Mac's binary keeps all four symbols. `pick` and `middle` are inlined **into `outer`**, so the frame that
holds the abort is `outer`'s, and (W) skips it as *"the one whose place the report already names"*
(`stack.c`, its comment). Asked at that frame's own abort site, the call to `hero_panic_index_at` inside
`outer`: **Linux, `llvm-symbolizer --inlines`: `deep.hero:3` (`pick`), `:6` (`middle`), `:10`
(`outer`); Darwin, `xcrun atos -i`: the same three**, and the walk's `main` adds `:14`. Every call is
recoverable at `-O2` on both platforms; the repair is (W) telling the abort's own frame's address too.

**5.2, the lines at run time, and what each way adds.**
- **Linux**: the ELF carries `.debug_line` (`deep` 68,316 bytes at `-O0`, 60,045 at `-O2`;
  `readelf -S`), `.debug_info` with **1,209** inline records at `-O2` (`llvm-dwarfdump | grep -c
  DW_TAG_inlined_subroutine`), and `.symtab`. `llvm-symbolizer` and `addr2line` resolve (W)'s addresses
  to `deep.hero:6`, `:10`, `:14`, and the Rebel case's to **`main.hero:6`** (the program's call into
  Rebel) and `:9`. They are tools outside the binary (present in the image); the runtime reads no DWARF.
  A **stripped** binary (265,440 bytes from 475,928) still names every frame through the `.dynsym`
  `-rdynamic` exports, and has no line left.
- **Darwin**: no line table in the binary (the critic's and the compiler-engineer's reading). On a fresh
  binary never symbolised (`fp/q5mac/fresh/`), `atos -i` with the build's objects moved away gives
  `h_deep_outer + 248`, no line; **with the objects put back it still gives that**, so atos keeps an
  answer per binary, a failed one included (that it is a cache keyed by the UUID is an inference); with a
  `.dSYM` beside, made by `dsymutil`, **214,556 bytes of DWARF for a 249,112-byte binary**, the whole
  inline chain with its lines. So a parent that resolves on Darwin must ask atos only while the objects
  exist, or write the `.dSYM` at link, which the link does not do today.
- **Adds over today's binary**: (W) 756 to 768 bytes of `.text`; (A) nothing in the binary, the parent's
  60 to 90 lines (the compiler-engineer's price) and, on Darwin, a `.dSYM` of the order of the binary if
  written; a table the emitter writes per call site cannot be keyed by a return address in portable C
  (the C has no address of a return until clang compiles it), so the line tables already are that table.
- **(A) needs from (W)**: the abort frame's own address (above), and an image base that is not
  `dladdr`'s on Linux (below).

**`dladdr` and the loader's lock, the compiler-engineer's open question, measured.** (W) calls `dladdr`
for every frame it tells, on Linux as on Darwin, for the image's base and name (`stack.c`, its
`hero_stack_say_calls`). A C probe (`fp/lx/dladdr/`): thread B `dlopen`s a library whose constructor
blocks; a second later the main thread calls `dladdr`, under a five-second alarm. **Linux arm64, glibc
2.41: `dladdr` was still waiting after 5 s, exit 3. Darwin: it returned at once** (`dladdr returned 1`).
So on Linux an abort beside a thread inside `dlopen` (Rebel's crypto, TLS and ODBC members open their
libraries at first call, Q1) would write its report and then hang instead of ending, against spec § 6's
*"An abort ends the program at once"*. Repair: on Linux the executable's base from `parts/symbols.c`'s
own reading or `getauxval(AT_PHDR)`, never `dladdr` in the walk. **And a question for the coordinator**:
the frozen stack guard's `hero_stack_symbol` (`stack.c:585-593`) falls back to `dladdr` on Linux for an
address outside the executable, inside a signal handler; the same wait is possible there (an inference
from the probe, unrun on the guard).

**5.3, Windows**: owed to the box. What dbghelp's `StackWalk64` and `SymFromAddr` need is the historian's
to source from Microsoft's pages (unread here; I fetched nothing). Two things I can say from the tree:
the link writes a PDB only where its line carries `-g` (`flags.hero:210-213`), so (A) has its table there
by the debug word every level already takes; and (P) and (B) need nothing of any platform. Whether a walk
on Windows x64 can stand on the unwind tables every x64 image carries, rather than on frame pointers, is
a question for the box.

**5.4, a stop inside a C library reached by a callback** (`fp/lx/cb/`: `main` → `outer` → C's
`through_c`, in a separately built library, → the Heroes `pick`, which aborts):

| C library built | program | Linux arm64 | Linux x86-64 (emulated) |
|---|---|---|---|
| keeping frame records (arm64 clang's default) | `-O0` | `main.pick` (the callback's C-callable wrapper), `main.outer`, `main.main` | n/a |
| | `-O2` | wrapper, `main.main` | n/a |
| `-fomit-frame-pointer` (arm64) / its clang's `-O2` default (x86-64) | `-O0` | wrapper, `main.main`: **`outer` lost** | wrapper, `main.main`: **`outer` lost** |
| | `-O2` | wrapper alone: **`outer` and `main` lost** | nothing (plain); wrapper alone (with the flag) |

A C frame without a record hides every Heroes caller above it from (W). The image's own distribution
C keeps records in what I sampled (`qsort_r`, `bsearch`, `sqlite3_exec`: one `x29` setup each); a C
library anyone builds with `-fomit-frame-pointer` keeps none, on Darwin arm64 too (`through-mac.o`: 0).
(W) also tells the callback's wrapper as `called from main.pick`, a line that reads as a second `pick`: it is `h_0cb_main_pick` in the emitted C (`fp/lx/amd/cb.c`), which calls `hero_callback_entered` and then `h_main_pick`; `outer`'s call into C records no frame of its own (its emitted call, `cb.c:200-201`), so nothing lets a walk step over a C frame that keeps no record.
(P) passes a place only through Heroes calls, so a C frame ends its chain as well, unless the place is
kept in a per-thread chain the C frame cannot break, which is a cost on every call (unpriced here).

- `verdict`: **approve** (W) with (A) **on three repairs, each measured**: the abort's own frame told
  (or its address handed to the parent), so `-O2`'s inlined calls are recovered from the line tables on
  both POSIX platforms; no `dladdr` in the walk on Linux (it waits on a held loader); and
  `-fno-omit-frame-pointer` among the compile words on x86-64 (without it (W) finds nothing at `-O2`), on
  every target if the latency suite agrees, since aarch64 Linux emits the same bytes with it. Windows
  owed to the box. **Object** to (P) on the compiler-engineer's ground (a cost on every call); on the C
  boundary (P) changes nothing a C caller sees, a callback reaching C through its wrapper. No veto.
- `section`: design.md §1.12 (the stop final, and a hang is not a stop); §4.19 (a callback is a binding's
  shape); spec § 6's *"at once"*. The document does not cover a walk through a C library's frames.
- `experiment`: (W) built in Linux arm64 and x86-64 from the compiler-engineer's files, the three
  programs and a C callback at both levels, with and without `-fno-omit-frame-pointer`, C libraries with
  and without frame records; `llvm-symbolizer --inlines` and `atos -i` at the abort site; the `dladdr`
  probe in C, clang accepted it on both, glibc hung it, Darwin did not.
- `argument`: The frame records and the line tables already hold the whole stack on both POSIX
  platforms, inlined calls included; the prototype loses them in three measured places: it skips the
  frame holding the abort, it asks `dladdr`, which glibc makes wait on another thread's `dlopen`, and on
  x86-64 it has no records without one word. Fix those and a walk plus the parent's symboliser names
  every `.hero` line, at 768 bytes and no happy-path cost. A C library built without records still hides
  the Heroes calls above it, a limit to state, not a reason for (P).
- `prediction`: with (W) telling the abort's own frame, `heroes run` (`-O2`) of `bw/q5/deep.hero` in Linux
  arm64, resolved by `llvm-symbolizer --inlines`, names `deep.hero:3`, `:6`, `:10` and `:14`; and on Linux
  a program aborting while another thread is inside a `dlopen` whose constructor blocks reaches exit 134
  only once `dladdr` is out of the walk.
- `condition`: a Linux run where `dladdr` does not wait (another glibc), which removes the second
  repair; or the latency suite measuring `-fno-omit-frame-pointer` past its floor on x86-64, which leaves
  that platform's walk to the parent's unwinder.

## What this seat leaves owed, and what it did not do

- **Route (S) not built into the layer**, so Rebel's own `crypto/seal` and `crypto/keypairs` tests under
  it were not run, at either level or under `--sanitize`; the member ran as a library against the
  vectors instead (Q3). Its first landing owes that run, and `keypairs.hero:485` is predicted to fail.
- **Route (V) not built**: no candidate at hand, nothing downloaded; its licence and audit are the
  historian's, its build and its MemorySanitizer taint the landing's.
- **The `>>` repair not built into the emitter** (`operator.hero:172`); its C is compiled and checked
  (`fp/q3/shr/shr.c`). The `-O0` branch on a secret sign bit in `word.shift_right` is a defect of the
  frozen tree for the coordinator to file.
- **(W)'s three repairs not built** (the abort's own frame told, no `dladdr` in the Linux walk,
  `-fno-omit-frame-pointer` on x86-64); each is measured as needed (Q5). The frozen stack guard's
  `dladdr` inside a signal handler on Linux is a question for the coordinator, unrun on the guard.
- **Windows**: every Windows row owed to the box (dbghelp, CNG's X25519, a walk on x64 unwind tables).
- **A still machine**: a timing test of dudect's kind for any cipher, and `-fno-omit-frame-pointer`'s
  instructions on x86-64.
- **No paid run, no GitHub write, no commit, no sub-agent**; containers `fp-253-0`, `fp-253-1` and
  `fp-253-2` stopped and removed (`docker ps -a --filter name=fp-253`: none), no process of mine left
  (`pgrep -fl 253-ffi-pragmatist`: none). Finished 17:28 CET (`date`).
