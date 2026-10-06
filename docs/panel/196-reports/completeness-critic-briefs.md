# Panel 196, completeness critic, first pass (the briefs)

Work copy: `<scratchpad>/196-critic/`, `git archive 39935f7c`, compiler built from
its seed at 20:23 (by `date`). My probes are in `<scratchpad>/196-critic/probe196/`
(outputs under `probe196/out/`); every number below was produced there between
20:23 and 20:48 on this Mac (Apple clang 21.0.0, OpenSSL 4.0.3 from Homebrew),
plus two reads of the `heroes-linux-arm64` image. No paid run. Nothing in the
repository or any worktree was edited.

## 1. Verified (re-run on the frozen tree, true as written)

- **The frozen tree**: `39935f7c` carries lanes c382, bs and unit as merges
  `2ea1ea00`, `3e2a5035`, `225d11c2`; no change to `seed/`, `runtime/` or
  `selfhost/` between `14a2a859` and `39935f7c`; `seed/heroes.c` is 38,114,106
  bytes, SHA-256 `1eeade6039fee357...`; `HERO_RUNTIME_ABI 28`
  (`runtime/heroes_runtime.h:58`); **fixpoint**: `heroes build
  selfhost/main.hero --emit-c` from the seed-built compiler, then `cmp` against
  `seed/heroes.c`: identical.
- **The fault**: `md_field.hero` `check` 0, `run` 0, printing `1 1 1 186
  2531777658719584577`; the last is bytes 8 to 15 of SHA-256("abc") read
  little-endian (Python `hashlib`). `md_scalar.hero` `check` 0, `run` 0, prints
  `1 1 1 186`; `run --sanitize` exit 0, 0 `ERROR` lines, at `-O2` (the `run`
  default) **and at `-O0`**. `md_scalar_canary.hero` `run` 0, canaries
  `1111111111` and `2222222222`.
- **The header**: `/opt/homebrew/include/openssl/sha.h:77` `SHA256_Final(unsigned
  char *md, SHA256_CTX *c)`, `:83` `SHA256(...)`, `:87` `#define
  SHA256_DIGEST_LENGTH 32`. On Linux arm64 (Debian's OpenSSL 3.5.7) the same
  declaration is line 76.
- **The census of 41**: reproduced byte for byte (`git ls-tree` of `39935f7c`,
  2,741 `.hero` files, **1,204** with a line opening `extern `, the one-line
  regex, `diff` against `scalar-lends.txt`: identical). The multi-line worry is
  measured empty: 25 extern signatures broken over lines, none holding a scalar
  `@` lend, so 41 is exact for scalars, not only a floor.
- **The compiler's own seven** are seven SIGNATURES carrying 12 lends, each
  against an `int64_t *` in `runtime/hero_os.h`; the runtime writes them only as
  `*x = ...` (44 such stores, no indexing): one value each.
- **The three probes**: `fixed_param.hero` `check` 1 `ffi_type` (*`u8[32]` cannot
  cross the FFI boundary*); `field_ptr.hero` `check` 1, `lent_shape` and
  `field_lend_uncounted`; `field_array.hero` `check` 0, `run` 0, `186` and `173`
  (SHA-256("abc")'s first and last bytes).
- **163**: `tests/golden/run/fixedbugs-163-...-as-i8.h` writes one `char`
  (`*p = (char)0xFF`), prints -1.
- **Spec lines**: 353-355 hold the out-parameter sentence (356 is the fence);
  396 the `f.ptr()` sentence; 447 `CParam`. `fixed_outside_a_group` is
  `selfhost/ffi_errors.hero:192`; defect 399 is open.
- **The spec's count is STALE**: `heroes measure` prints claude-legacy 7,135,
  cl100k_base 7,266, maximum 7,266, and `STALE: the recorded count is for
  7a1ea85e443d182b and this file is 3bb126c03750e011`, exit 1.
  `<scratchpad>/p196/refresh-approved` exists (20:31, four runs).
- **Paths the seats' briefs name exist**: `selfhost/emit/ffi_unit.hero`,
  `selfhost/emit/lend_count.hero`, the pointee probes
  (`selfhost/cli/pointee*.hero`, `header_*.hero`, `selfhost/emit/ffi_pointee.hero`,
  `extern_probe.hero`), `tests/harness/suite_layout.hero`,
  `.claude/rules/spec-shape.md`, panel 194's ffi-pragmatist report (its
  *Found beside* section, where 396 was found). The blind seat's command matches
  `<scratchpad>/p194/blind-run.sh` flag for flag; no `CLAUDE.md` exists in
  `/tmp`, `/private/tmp`, `/private`, `/` or `~/.claude/`.
- `inet_ntop` takes a count (`char *_LIBC_COUNT(__size), socklen_t __size`);
  `gets` takes none (`char *_LIBC_UNSAFE_INDEXABLE`).

## 2. False, or true only in part

1. **"lanes bs and unit changed § 13's text"** (`00-shared.md`, spec-warden
   item 4): only lane bs did. `git log` of the unit lane's five commits from its
   fork `7001dfb3` touches no `spec/` file; the spec's one change since the pin
   (`ca7cf585`, 9,518) is `54c5f618`, lane bs, +7/-3 lines. So panel 194's route
   C (`counted_by` on an `@` record lend) **landed with no spec sentence**: § 13
   mentions `counted_by` only at line 396 (`f.ptr()` to a `ptr`) and in the
   grammar. Any S2 sentence is written over a spec already silent on a form the
   compiler accepts.
2. **"each against a runtime declaration `int64_t *status` or `int64_t *marks`"**:
   `selfhost/main.hero:78` lends `@why` against `int64_t *why`
   (`runtime/hero_os.h:417`). Still one value; the sentence is wrong in its
   detail.
3. **"two that bind a C `char *` as one cell: 163 ... and the three 396
   programs"**: the 396 programs bind `unsigned char *`, 163 binds `char *`.
   This matters for S3's wording and for what follows in item 7.
4. **"A byte-typed pointee does not tell the two apart"**: true, and too narrow.
   **The fault does not need a byte pointee.** `pipe(@fds: i32)` against macOS's
   `int pipe(int [2]);` (`unistd.h:482`), lent `@b.fd` of a group record `{fd:
   i32, after: i32}` built with `after: 7`: `check` 0, `run` 0, printing `0 3
   4`: C wrote the second descriptor over `after`. **S3 as drafted (byte
   pointees only) does not reach it**; S5 does (item 3 of § 4 below).
5. **The shape is not "where the header gives no count"** (the question as
   framed). `gethostname(@name: i8, namelen: u64)`, the header's count present,
   lent `@b.n` of a 16-byte group record with `namelen: 64`: `check` 0, **`run`
   134**, printing `0`, `101` (`'e'`, the host name's second byte, in the next
   field), `0` (`after`, 7 before), then **`panic: a null function pointer was
   called`**, a false message: the stack was overwritten. A count in the header
   does not protect a one-cell lend, and the binding cannot tie one to it:
   `@name: i8 counted_by namelen` is `counted_by_shape` (*`counted_by` says how
   far C reads or writes through a `ptr`, or through a group's record lent
   whole*). **The blind seat's task 3 is therefore not a clean control**: its
   most natural wrong reading is the fault itself.
6. **"`md_scalar_canary.hero`: both canaries intact; where the other 31 bytes
   went is unrun"**: settled at `-O2` by `otool -tV` of the `-O2` build:
   `m` is `sp+0xf`, `c` (`SHA256_CTX`, 112 bytes) is `sp+0x10..0x7f`, so the 31
   bytes land in the first 31 bytes of `c`; and the two canaries are **never in
   memory** (`mov w0, #0x35c7; movk w0, #0x423a` is 1111111111 as an immediate),
   so at `-O2` the probe cannot see anything: its *intact* is vacuous, and
   panel 194's *"the 31 bytes landed in frame slots the program did not print"*
   is an inference this disassembly replaces. `md_field` at `-O2`: `Box` at
   `sp+0..0xf`, `c` at `sp+0x10`; the 32 bytes cover all of `Box` (16 bytes) and
   16 of `c`.
7. **"`--sanitize` is silent"** (defect 396's line; the brief's `run --sanitize`
   for `md_scalar`): true at both levels for `md_scalar` and `md_field`, **false
   for `md_scalar_canary` at `-O0`**: `heroes run md_scalar_canary.hero
   --sanitize -O0` exits **134**, `UndefinedBehaviorSanitizer: misaligned address
   0xad1500f261ff10a4 for type 'HeroStrHeader'` then `AddressSanitizer: SEGV ...
   in hero_str_hdr_checked`. `0xad1500f261ff10a4` is `0xad1500f261ff10b4`, bytes
   24 to 31 of SHA-256("abc") read little-endian, minus the header's 16: the
   digest's tail overwrote a `str` in the frame. A second run printed the same
   UBSan lines and then hung in the handler (state S, 0 CPU, killed after
   3:30). This is defect 390's shape (`run --sanitize` builds at `-O2`).
8. **"The one correct binding today needs C written by hand"**: true for
   `SHA256_Final`, but **the contrast it implies with S1 and S2 is false as they
   are drafted**. A fixed array exists only as a group record's field, a group
   record must be a struct the named header declares (a record over `struct
   digest32` in a group naming only `openssl/sha.h`: `check` 0, `run` 1,
   `ffi_unknown_tag`), and `.ptr()` takes only `i8[N]`/`u8[N]` (`buf.ptr()` on a
   `[u8]`: `bad_operand`). **S1's place and S2's field both need a hand-written
   header declaring a struct with a `u8[32]`**; S1, S2 and S6 differ in whether
   that header also holds a function, not in whether C is written by hand.
   Also: a route with **no hand-written function** exists today (§ 4, item 2).
9. **"`crypt`'s outputs"** (ffi brief, as an instance of the shape): `crypt`
   writes no parameter (`char *crypt(const char *, const char *)`, macOS
   `unistd.h:558`, Debian `crypt.h:63`); `crypt_r` writes one struct and
   `crypt_rn` takes a size. The right neighbour is `encrypt(char
   *_LIBC_COUNT(64), int)` (macOS `unistd.h:560`).
10. **The blind seat scores "whether it would `check`"**: `check` does not run the
    header probes. Measured: `frexp(x: f64, @e: i64)` `check` 0, `run` 1
    `ffi_parameter_type`; a record lent whole to `unsigned char *md` `check` 0,
    `run` 1 `ffi_parameter_type`; 163's `@p: u8` `check` 0, `build` 1. A
    reader's program must be judged by `build`/`run`, or a pointee refusal is
    scored as acceptance.
11. **"41 single-cell `@` lends in the tree"**: 41 LINES carrying 51 lends; of the
    lines, 8 are cases annotated `#~` (refusals the compiler is meant to make), 8
    are `docs/panel/` probes and 1 is `archive/`, which nothing builds. Live and
    accepted: **24 lines, 34 lends** (`selfhost/` 7, `tests/golden/` 12,
    `tests/harness/` 5); none in `examples/`. And the census counts only
    scalars: **509** record-typed `@` lends sit on one-line extern signatures
    (handles among them), and the sitting's question is the same for them.

## 3. Could not verify

- **The Windows box** (`ssh win`, clang 23.1.1, 2 cores): `ssh -o
  ConnectTimeout=10 win` timed out at about 20:39 (*connect to host
  apponfly-vps port 22: Operation timed out*). The ffi-pragmatist's item 5 is
  likely unrun unless it answers later.
- **`BCryptFinishHash` takes a count**: no `bcrypt.h` on this Mac or in the
  Linux image; the historian's or the box's to source.
- **Why `evp_outcount_512` (§ 4, item 2) exits 134**: the function carries
  `___stack_chk_guard` and the record is overrun by 24 bytes, so the stack
  protector is the likely cause; a backtrace (`lldb -b`) hung and was killed.
  An inference, not a measurement.
- **The real token count**: the spec-warden's paid call, not mine.

## 4. Routes and shapes the briefs did not list

What would have to be true for an unlisted route to exist? That the 32 bytes
can live somewhere other than a header struct, that a check can read something
other than the parameter's type, or that the extent can be owned by the
emitter rather than asserted by the binding. Each was probed.

1. **S0, where the bytes live (a prerequisite of S1 and S2, not an
   alternative)**: admit a fixed array as a LOCAL's type when it is lent to C
   (`md: u8[32] @ ...`, today `fixed_outside_a_group`, panel 062; defect 399
   sits on its note), or lend a `[u8]` through `.ptr()` with its length checked
   against the extent at run time (today `bad_operand`). Without one of them,
   S1 and S2 close 396 only for a programmer who also writes a C header.
2. **Today's forms already compose, without a shim function, and the
   composition is the same lie with a word**: `EVP_DigestFinal_ex(c: MdCtx, md:
   ptr counted_by s lent, @s: u32)`, `EVP_sha256() -> ptr`, `d.b.ptr()`, `s: u32
   @ 32` (`probe196/evp_outcount.hero`, header `evp_box.h` declaring only
   `struct digest32`): `check` 0, `run` 0, `1 1 1 32 186 173`, correct. But `s`
   is an OUT count C never reads: with `EVP_sha512()` into a `{b: u8[32], after:
   i64}` record (`evp_outcount_512.hero`): `check` 0, **`run` 134**, `after`
   printed `-6286656575195475423`, bytes 32 to 39 of SHA-512("abc"). **A
   `counted_by` naming a parameter C writes is 396 with an explicit word**; the
   header cannot tell `unsigned int *s` in from out. Any ruling on what an
   extent word promises reaches route C's `counted_by` too, or leaves this twin.
3. **S5 has a cheap mechanism, measured, and it does not reach 396's own
   reproducer**: redeclare the function in the probe unit with each `@`
   parameter as `T name[1]` under `-Werror=array-parameter` (the compiler
   already passes `-Wall`, `selfhost/cli/flags.hero:97`). Apple clang 21:
   `pipe(int fds[1])` warns *previously declared as 'int[2]'*;
   `uuid_generate(unsigned char out[1])` warns *previously declared as 'uuid_t'
   (aka 'unsigned char[16]')*; `SHA256_Final(unsigned char md[1], ...)` is
   **silent** (the header writes a pointer). The frozen compiler sees none of it
   (`pipe(@fds: i32)` builds). The headers' own words, measured: OpenSSL 4.0.3
   carries **0** files with `counted_by`, `sized_by`, an `access` attribute, SAL
   or `[static`, though `modes.h` declares `ivec[16]`; macOS's SDK carries
   `_LIBC_COUNT` 161 times and `_LIBC_SIZE` 91 in 30 files, among them
   `ctime_r(..., char *_LIBC_COUNT(26))`, `asctime_r` 26, `strmode` 12,
   `encrypt` 64; Debian's glibc carries `__attr_access` 78 times as
   `(__write_only__, N, N)` and 4 times as `(__write_only__, N)` with no size
   (`getwd`, `if_indextoname`, two in `stdio.h`), and leaves `ctime_r` bare.
4. **S5′, the platform's own bounds checker**: Apple clang 21 accepts
   `-fbounds-safety` and then defines `__LIBC_STAGED_BOUNDS_SAFETY_ATTRIBUTES`;
   `ctime_r(&t, &c)` with one `char` compiles and **traps at run (exit 133)**;
   a 26-byte buffer prints the date; a plain `char *` parameter passed on is a
   compile error, *count value of 26 always fails*. Mac-only, annotated headers
   only; not a route for OpenSSL.
5. **S1′, no new surface**: admit a group record of exactly one fixed-array field
   `T[N]` lent whole to a `T *` parameter (today: `@md: Digest32` against
   `unsigned char *md`, `check` 0, `run` 1 `ffi_parameter_type`, *a different
   kind of thing*). The extent is the record's type, the signature stays a
   record lend as route C made it, and the grammar does not move. It shares S0's
   need for a header struct.
6. **S7, an emitter-owned buffer by copy-in, copy-out**: § 9 already defines `@`
   as copy in, copy out; an `@md: [u8]` with a stated extent could be marshalled
   through a C temporary of exactly N bytes that the emitter allocates (with S4's
   guard around it if wanted) and copied back. No group record, no header, no
   place type: the extent is owned by the code that allocates. Unbuilt; named
   because it is the only listed or unlisted route that removes the hand-written
   C for `SHA256_Final`.
7. **S2's two spellings cost differently**: `counted_by SHA256_DIGEST_LENGTH`
   (a group `constant`) PARSES today and is refused by the checker
   (`counted_by_shape`, *names no other parameter*), so it needs no grammar
   change; `counted_by 32` is a parse error (`expected_extent`) and moves
   `CParam` (spec line 447) and with it the `grammar` suite. Note also that the
   language's own wording of `counted_by` says *how far C reads* in one message
   and *reads or writes* in another.
8. **S3 widened**: refuse every unmarked single-cell `@` lend to a typed pointer,
   not only a byte one. Cost on the tree: the 34 live lends above (12 of them the
   compiler's own, so the seed and its fixpoint move), against `pipe` and
   `gethostname` caught.

**A shape beside 396 that no brief names, and it is worse (CL-061, depth one)**:
**a `cstr` lent to a WRITABLE `unsigned char *`**. `ffi_writable_parameter`
(panel 058, `selfhost/emit/ffi_mutable.hero`) fires only when clang says
*discards qualifiers*; for `const char *` to `unsigned char *` clang says
`-Wpointer-sign` instead (measured on `ptrsign.c`), so the refusal never fires
(`gethostname(name: cstr)` against `char *` IS refused). Measured with
`SHA256_Final(md: cstr lent, ...)`:
- `s = "0123456789abcdef0123456789abcdef".repeat(1)`, `t = s`, then
  `SHA256_Final(md: s.cstr(), @c)`: `check` 0, `run` 0, and **`t[0]` prints
  186**: a copy made before the call changed, § 3's *no aliasing* false at exit
  0;
- `s = "a".repeat(1)`: `run` 0 and `run --sanitize -O0` 0 with no report, C
  writing 32 bytes into a 2-byte heap string;
- `s = "a"` (a literal): `run` **138** (SIGBUS);
- a lease, `x: cstr @ "a".lease()` passed as `md: cstr`: `run` 0, `--sanitize
  -O0` 0;
- the only sign is two clang `-Wpointer-sign` warnings in the build output.
This is the class panel 058 closed for `char *`, open for `unsigned char *` and
presumably `signed char *` (the latter unrun). It is `blocking` by
`.claude/rules/verification.md`'s list (a wrong program accepted, a memory
fault at exit 0) and should be filed whatever the sitting rules.

## 5. The question the sitting should be asking

The brief asks *how a binding says that C writes N*. Every probe above says the
extent of a C write through a typed pointer is something **the header cannot
state** for the case that matters (`SHA256_Final`, an out-count like
`EVP_DigestFinal_ex`'s `s`, a `cstr` handed to `unsigned char *`), so every
static route turns N into the binding author's assertion, a word like `@` or
`counted_by` is today. The questions the sitting is not asking:

1. **What does the compiler do when the binding says nothing?** Today an
   unmarked `@x: T` to a `T *` asserts *one*, silently, and the fault is
   measured with a byte (`SHA256_Final`), an `int` (`pipe`) and with a count
   present (`gethostname`). Should that default be refused unless stated (the
   thesis: the claim made local and visible, the `@one` or `[N]` written where a
   reader sees it), guarded at run time (S4), or left a lie (S6)? Whichever
   answer, does it cover the two other places the same assertion hides: a
   `counted_by` naming a count C writes rather than reads, and a `cstr` lent to a
   pointer C writes?
2. **Where does a buffer of N bytes live in a Heroes program without C written
   by hand?** Until a local fixed array, a `[u8]` lend or an emitter-owned buffer
   (S0, S7) exists, S1 and S2 move the hand-written C from a function to a struct
   and close 396 for nobody who will not write a header; the blind tasks 1 and 3
   have no correct one-file answer under any variant, so the coordinator must
   say before the runs what a reader's correct program is allowed to be (may it
   write a `.h`?) and score by `build`/`run`, not `check`.
