# Panel 196, shared brief: what a lend of one cell promises when C writes an array through it

Convened 2026-10-06 by the coordinator, under the author's standing goal of
that day (meant as: *within 24 hours, close every defect that is not an
improvement, and advance the roadmap*), for defect 396, `systemic`, filed by
panel 194 as *a ruling no rule reaches*:
`issues/2026-10/06/2026-10-06-1826-defect-396-a-single-value-lent-through-to-a-pointer-c-writes-an-array.md`.
A full panel: the question is what an `@` parameter of an `extern` means, a § 13
sentence and possibly a diagnostic class (CLAUDE.md § 4). M-buildable-structs'
panel gate was asked at panel 194; this sitting convenes without asking again.

**This brief was repaired after the completeness critic's first pass**
(`docs/panel/196-reports/completeness-critic-briefs.md`, written 20:23 to
20:49), the false sentences corrected where they stood and its findings
collected in § What the critic's first pass found, below; the coordinator
re-ran `pipe_cell`, `host_one` and `evp_outcount_512` at 20:53.

**The frozen tree is `39935f7c`** on `lane-panel-196`: batch 13's round with
lanes c382, bs and unit merged (panel 194's R1 `rest: zero` and its route C for
defect 092 landed; panel 195's literal constants landed), the seed committed at
`14a2a859` (38,114,106 bytes, SHA-256 beginning `1eeade6039fee357`, the
runtime's ABI 28, its fixpoint by `cmp`). **Every fact below was produced by
the command named beside it, run by the coordinator on 2026-10-06 between 20:18
and 20:21 by the clock; a number from an earlier sitting is marked CARRIED and
is a question, never a premise** (CLAUDE.md § RUN IT; CL-077).

## The question

**What does an `extern` function's `@x: T` parameter promise about how many
`T` C may write through it, and how does a binding say that C writes N of them
where the header gives no count?** Today the spec says one thing (§ 13, lines
353-356 of `spec/heroes-spec.md`): *"A C out-parameter is an `@` parameter, and
what it points at is held to the same width and sign — `@n: u64` where it says
`size_t *`"*. It says nothing of a pointer C writes as an array.

**Widened after the critic's first pass, and the two questions it says the
sitting must answer**: (1) *what does the compiler do when a binding says
nothing?* Today an unmarked `@x: T` silently asserts ONE element, and the fault
is measured on a byte (`SHA256_Final`), on an `int` (`pipe`, below) and with a
count present (`gethostname`, below); is that default refused unless stated,
guarded at run time, or left as the binding author's lie, and does the answer
reach a `counted_by` naming a count C WRITES (`EVP_DigestFinal_ex`, below)?
(2) *Where does a buffer of N bytes live in a Heroes program without C written
by hand?* Until it has an answer, S1 and S2 move the hand-written C from a
function to a struct.

## What is measured today

- **The fault, re-run on the frozen tree** (`heroes check` and `heroes run` on
  `docs/panel/194-evidence/new-defects/single-cell/`, OpenSSL 4.0.3 from
  Homebrew, Apple clang, this Mac): `md_field.hero` binds
  `SHA256_Final(@md: u8, @c: Sha256Ctx)` and lends `@b.m`, a `u8` field of a
  group record `Box { m: u8, rest: u8[7], after: i64 }` built with `after: 7`:
  `check` 0, `run` 0, printing `1 1 1 186 2531777658719584577`. The last is
  `b.after`, 7 before the call, now bytes 8 to 15 of SHA-256("abc"): C wrote 32
  bytes through a one-byte cell. `md_scalar.hero` (a local `m: u8 @ 0`):
  `check` 0, `run` 0, printing `186`; `run --sanitize` exit 0 with 0 `ERROR`
  lines. `md_scalar_canary.hero` (two `i64` canaries around the cell): `run` 0,
  both canaries print intact (`1111111111`, `2222222222`), which proves
  nothing: at `-O2` they are immediates, never in memory (the critic, by
  `otool`); the cell sits at `sp+0xf` and the context `c` at `sp+0x10`, so the
  31 bytes land in `c`. **At `-O0`, `run --sanitize` of `md_scalar_canary`
  exits 134**: UBSan reports a misaligned `HeroStrHeader` and ASan a SEGV, the
  digest having overwritten a `str` in the frame (the critic; `--sanitize` is
  silent on `md_scalar` at both levels).
- **The header gives no count** (`grep -n SHA256_Final
  /opt/homebrew/include/openssl/sha.h`): line 77,
  `OSSL_DEPRECATEDIN_3_0 int SHA256_Final(unsigned char *md, SHA256_CTX *c);`;
  line 83, `unsigned char *SHA256(const unsigned char *d, size_t n, unsigned
  char *md);`. The 32 is in the library's documentation and in
  `SHA256_DIGEST_LENGTH`, never in the parameter.
- **Single-cell `@` lends in the tree** (`git ls-files '*.hero'` filtered to the
  1,204 files with an `extern` group, then lines matching an indented
  `function` with `@<name>: <i8..u64|f32|f64|bool>` on one line; the list is
  `docs/panel/196-evidence/scalar-lends.txt`; a signature broken over lines is
  missed, and the critic found none of the 25 such signatures holds a scalar
  lend): **41 lines carrying 51 lends**; live and accepted, 24 lines and 34
  lends (8 are `#~` refusal cases, 8 `docs/panel` probes, 1 `archive/`).
  The count is of scalars: **509 record-typed `@` lends** sit on one-line
  extern signatures, and the question reaches them too. Seven are the compiler's own
  (`selfhost/main.hero:78,81`, `selfhost/cli/process.hero:47,60,61`,
  `selfhost/cli/runtime_places.hero:17`, `selfhost/module/reading.hero:30`),
  each against a runtime declaration `int64_t *status`, `int64_t *marks` or
  `int64_t *why` (`grep` of `runtime/*.h`): seven signatures carrying 12 lends,
  the runtime storing one value through each (44 stores, the critic). The cases are `frexp(@e: i32)`,
  `time(@t)`, `getsockopt(@len: u32)`, `fill_len(@n: u32)`; one binds a C
  `char *` as one cell, `tests/golden/run/fixedbugs-163-a-plain-char-pointee-bound-as-i8.hero`,
  whose C writes ONE `char` (-1) and is correct, and the three 396 programs
  bind an `unsigned char *` C writes 32 through. **A byte-typed pointee does not tell the two apart.**
- **What a binding can write today for 32 bytes** (probes in
  `docs/panel/196-evidence/`, each `check`ed on the frozen tree):
  - `@md: u8[32]` as the parameter's type (`fixed_param.hero`): `check` 1,
    `ffi_type`, *`u8[32]` cannot cross the FFI boundary*;
  - `md: ptr lent` lent `d.b.ptr()`, `d` a group record with `b: u8[32]`
    (`field_ptr.hero`): `check` 1, `lent_shape` and `field_lend_uncounted`, a
    `ptr` lend needing `counted_by <parameter>`, and `SHA256_Final` has no
    count parameter;
  - a C shim taking `struct digest32 *` (`docs/panel/194-evidence/new-defects/single-cell/sha_shim.h`,
    `hero_sha256_final`) and `@d: Digest32` (`field_array.hero`): `check` 0,
    `run` 0, printing `186` and `173`, SHA-256("abc")'s first and last bytes
    (Python's `hashlib`). **The one correct binding today needs C written by
    hand**, and so do S1 and S2 as drafted below: a group record must be a
    header's struct (a record over `digest32` in a group naming only
    `openssl/sha.h`: `run` 1, `ffi_unknown_tag`), and `.ptr()` takes only an
    `i8[N]` or `u8[N]` field (on a `[u8]`: `bad_operand`), the critic's.
- **The grammar** (`spec/heroes-spec.md:447`): `CParam = [ "@" ] ident ":" Type
  [ "counted_by" ident ] [ "lent" ]`; `counted_by` names a parameter, and
  § 13 (line 396) reads it on a `ptr` lend of a field. A fixed array `T[N]`
  is a group record's field type only (`fixed_outside_a_group`,
  `selfhost/ffi_errors.hero:192`; defect 399 is open on its note).
- **The spec's count is STALE on this tree** (`heroes measure
  spec/heroes-spec.md`: vendored maximum 7,266, cl100k_base; the pin recorded
  for another hash): lane bs changed § 13's text (`54c5f618`; lane unit's
  route C for 092 landed with no spec sentence, the critic), and the real count
  needs `--refresh`, a paid call; whether the spec-warden may run it is in its
  brief.

## The routes on the table, each to be judged, none adopted

- **S1, a fixed-array out-parameter**: `@md: u8[32]` admitted in an `extern`
  signature, lent only a place of that exact type (a group record's `u8[32]`
  field), C handed its address; the extent is the type's.
- **S2, a counted lend with a constant extent**: `counted_by` taking a constant
  or a literal (`md: ptr counted_by SHA256_DIGEST_LENGTH lent`, or `32`), so
  `d.b.ptr()` is checked against it as a parameter-named count is today.
- **S3, a refusal at the boundary**: a single-cell `@` lend refused where the
  header's pointee is a byte type unless the binding says it is one (breaks
  163 as written; what it would cost is the question).
- **S4, a run-time guard**: the cell lent inside a region the emitter checks
  after the call, aborting where C wrote past it (what it costs per call, and
  what it misses).
- **S5, the header's own words**: an array parameter the header declares
  (`unsigned char md[32]`, `[static 32]`, clang's `counted_by` attribute)
  read by the header probes and checked; what fraction of real headers say it.
- **S6, a § 13 sentence alone**: `@` promises one element; a buffer is bound as
  a group record lent whole, through a shim where the header has no struct; the
  fault stays a binding author's lie, as every C lie is.
- **Nothing**, the stopping rule's third shape, if today's forms already
  compose to it. A route nobody listed is the critic's to name.

## What the critic's first pass found

Each re-run by the coordinator at 20:53 where it says so; the rest is the
critic's, with its commands in its report.

- **The fault needs no byte pointee** (re-run): `pipe(@fds: i32)` against
  `int pipe(int [2])`, lent `@b.fd` of a group record `{fd, after}`: `check` 0,
  `run` 0, `after` prints 4, the second descriptor
  (`docs/panel/196-evidence/critic/pipe_cell.hero`). S3 keyed on byte pointees
  misses it.
- **A count in the header does not protect a one-cell lend** (re-run):
  `gethostname(@name: i8, namelen: u64)` with `namelen: 64`, lent a field of a
  16-byte group record: `check` 0, `run` 134, `after` 7 becomes 0, then a false
  *panic: a null function pointer was called* (`critic/host_one.hero`); and
  `@name: i8 counted_by namelen` is refused `counted_by_shape`
  (`critic/host_cell.hero`), so the binding cannot tie the count to the cell.
- **Today's forms already compose, with the same lie under a word** (re-run):
  `EVP_DigestFinal_ex(c, md: ptr counted_by s lent, @s: u32)` with `s = 32` set
  before the call runs correctly for SHA-256; for SHA-512 into
  `{b: u8[32], after}`: `check` 0, `run` 134, `after` printing SHA-512 bytes
  (`critic/evp_outcount_512.hero`): `s` is an out-count C never reads, and
  `counted_by` takes it as the extent.
- **A `cstr` lent to an `unsigned char *` C writes is not refused**: filed as
  defect 411 (`blocking`) and given to lane b13-w411, not this sitting; it is
  the shape `ffi_writable_parameter` exists for, missed because clang words it
  `-Wpointer-sign`.
- **What real headers say** (S5): OpenSSL 4.0.3, nothing (0 files with
  `counted_by`, `sized_by`, an `access` attribute, SAL or `[static`); the macOS
  SDK, `_LIBC_COUNT` 161 times and `_LIBC_SIZE` 91 (`gethostname(char
  *_LIBC_COUNT(__namelen), size_t __namelen)`); Debian's glibc,
  `__attr_access` write-only with a size 78 times. A cheap S5 mechanism:
  redeclare with `T x[1]` under `-Werror=array-parameter`, which catches
  `pipe` (`int[2]`) and `uuid_generate` (`unsigned char[16]`) and is silent on
  `SHA256_Final`.
- **Routes the briefs did not list**: **S0**, where N bytes live (a local fixed
  array lent to C, today `fixed_outside_a_group`; a `[u8]` lent through
  `.ptr()` with a run-time length check, today `bad_operand`), a prerequisite
  of S1 and S2; **S1'**, a group record holding one `T[N]` field lent whole to a
  `T *` (today `run` 1, `ffi_parameter_type`); **S5'**, Apple's
  `-fbounds-safety`, Mac-only, which traps a one-`char` lend to `ctime_r`
  (exit 133); **S7**, an emitter-owned buffer by copy-in and copy-out: `@md:
  [u8]` with a stated extent passed through a C temporary of exactly N bytes,
  unbuilt, the one route that removes the hand-written C for `SHA256_Final`;
  **S3 widened** to every typed pointer, which costs 34 live lends, 12 of them
  the compiler's own (the seed moves), and catches `pipe` and `gethostname`.
- **S2's two spellings cost differently**: `counted_by SHA256_DIGEST_LENGTH`
  parses today and is refused by the checker; `counted_by 32` is a parse error
  (`expected_extent`), moving `CParam` and the `grammar` suite.
- **The blind seat is scored by `build` and `run`, never `check`**: `check`
  runs no header probe, so `frexp(@e: i64)` checks 0 and builds 1
  (`ffi_parameter_type`).

## How the sitting runs

Each seat works in its own copy, `<scratchpad>/196-<seat>/`, made by
`git -C /Users/joseph/Temp/heroes/heroes-lang archive 39935f7c | tar -x -C
<scratchpad>/196-<seat>`, its compiler built there: `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes` (a few seconds). Never another
seat's copy, never the repository. No timing: four lanes run on this machine;
an instruction count is allowed. **Paid runs**: the blind seat's `claude -p`
sessions, bounded in its brief; no other unless its brief names it. Write your
report as you go in `<scratchpad>/p196/reports/<seat>.md`; the coordinator
copies it into `docs/panel/196-reports/` with the sitting. `<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
