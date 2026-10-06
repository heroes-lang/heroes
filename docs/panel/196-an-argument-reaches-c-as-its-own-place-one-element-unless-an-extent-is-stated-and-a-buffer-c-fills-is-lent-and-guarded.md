# Panel 196: an `@` argument reaches C as its own place, one element unless an extent is stated, and a buffer C fills is lent and guarded

Convened 2026-10-06 by the coordinator for defect 396, `systemic`, filed by
panel 194 as *a ruling no rule reaches*, under the author's standing goal of
that day (meant as: *within 24 hours, close every defect that is not an
improvement*). A full panel: the compiler-engineer, the ffi-pragmatist, the
spec-warden, the historian, the blind seat as nine fresh `claude -p` sessions,
and the completeness critic before and after. The tree frozen at
**`39935f7c`** (batch 13's round with lanes c382, bs and unit, its seed at
ABI 28). Briefs written 20:18 to 20:23 and repaired on the critic's first pass
(20:23 to 20:49); the seats from 20:55, stopped by an API session limit about
21:20 and resumed at 22:44, done by 23:03; the blind sessions 20:55 to 20:58;
the critic's second pass 23:08 to 23:31; this synthesis from 23:41, every time
read from `date`. Briefs in `196-briefs/` (the blind ones in `blind/`), reports
in `196-reports/`, evidence in `196-evidence/`.

## The question, verbatim from the brief

**What does an `extern` function's `@x: T` parameter promise about how many
`T` C may write through it, and how does a binding say that C writes N of them
where the header gives no count?** Widened on the critic's first pass: *what
does the compiler do when a binding says nothing, and where does a buffer of N
bytes live without C written by hand?* And on its second pass, the question
the sitting should have asked: **what address does C receive for an `@`
argument, and for how long may C use it?**

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | historian | blind seat |
|---|---|---|---|---|---|
| **U, one element** | approve | approve | approve (+4 to +6 est.) | approve (GCC, glibc, Cyclone, .NET, SAL write the same default) | 6 of 9 read it (variants A, B) |
| **S7, the emitter's buffer** | approve, **veto without a guard** (built, `heroes-s7`, `heroes-s7g`) | approve on a guard page; **veto any copy of a record** | approve, provisional (+79 real) | — | no reader saw it |
| **S2, `counted_by` a constant** | approve as S7's spelling (built) | approve as S7's spelling | approve (+17 real) | approve (with S1) | t1-B wrote it; builds under `heroes-s2` (critic) |
| **S1, `@md: u8[32]`** | object (built) | object | **veto** | approve (advisory; D's `ref int[2]`, which needs S0) | t1-A wrote it; refused 3 times under `heroes-s1` (critic) |
| **S5, the header's extent** | approve as a complement, redeclaration form | approve as a complement, the header's own spellings | approve, provisional | object as the only mechanism | — |
| **S4, a run-time guard** | approve on single cells, no spec sentence | approve the guard page; **veto a copy** | object standalone | no precedent outside diagnostic modes | — |
| **S3 (default refusal), S6, S0, nothing** | object S3 (built, checks nothing), **veto S0**'s local fixed array | object | **veto** S3 widened; object S6 | object S6 alone, S3 | 3 of 3 bound `frexp` as an unmarked cell |
| **section** | §1.7, §1.12, Part 5 | §1.11, §1.12, §4.19 | §1.6, §1.2, Principle 0 | precedent | the spec alone |

## What the sitting measured

- **The fault is not about bytes, nor about a missing count** (the critic's
  first pass, re-run by the coordinator): `pipe(@fds: i32)` sets its
  neighbour to 4 at exit 0; `gethostname(@name: i8, namelen: 64)` exits 134
  with a false message; `EVP_DigestFinal_ex`'s out-count, read as an extent,
  exits 134 with SHA-512's bytes in the neighbour; and the ffi-pragmatist found
  it on the fourth platform (`GetKeyboardState`, 255 bytes, exit 0).
- **The header rarely says** (the ffi-pragmatist's census, clang reading the
  headers): a literal extent on 96 parameters of the macOS SDK, 173 of
  Homebrew's, 116 of Linux arm64's, against 2,754, 6,957 and 1,980 single-cell
  scalar out-parameters a route must keep; OpenSSL states no extent for any
  digest output in two versions.
- **The one correct binding of `SHA256_Final` today needs C written by hand**,
  and the blind seat measured what that costs: 0 of 9 readers wrote the
  one-cell fault, and 5 of the 6 buffer programs wrote a C header (the critic:
  0 of 6 bounds the fault's rate only below 39%; 6 of the 9 read U; none read
  S7; task 1's readers all chose the one-shot `SHA256`).
- **S7 builds and runs from one `.hero` file** (the compiler-engineer):
  `SHA256_Final`, `pipe` and `gethostname` with no header of the program's
  own; the frozen `selfhost/main.hero` emitted byte-identical to the seed; a
  `check` census of 2,740 tracked files moved by no route but the default
  refusal (59 files, all by its own diagnostic).
- **N is the binding's word under every static route**, and an understated N
  corrupts; a guard turns it into a named abort (the compiler-engineer's
  canary, the ffi-pragmatist's guard page, both built in C or Heroes).
- **Neither guard is robust as built** (the critic's second pass): the canary
  misses an overrun whose bytes equal its constant pattern (`fgets` of `0xA5`,
  no panic) and puts N on the stack (8 MiB and up exits 139 with no message);
  the one-region guard page gives 26 and 89 wrong digests under two threads at
  exit 0, and cannot hold N above a page.
- **S7's buffer is a lend that ignores § 13's lend rule** (the critic):
  `setvbuf` keeps it, and stdio then writes a dead frame at exit 0
  (`stack-use-after-scope` under `--sanitize`); `lent` on an `@` parameter,
  which would say *C keeps nothing*, is refused today (`lent_shape`).
- **One spelling, four addresses** (the critic): a local or a field reaches C
  as its place; an array's element as a temporary (defect 413, zlib `0 -2
  -2`); a Heroes function's own `@` parameter as the callee's copy
  (`selfhost/emit/body.hero:294-310`, the copy-in prologue: zlib `0 -2 -2`,
  libuv an endless loop at `-O2` and a silent SIGSEGV at `-O0`); and under S7
  or S4 the emitter's buffer. A library that keeps the address (zlib, libuv,
  stdio, a mutex) is correct only in the first.
- **Precedent** (the historian, every claim sourced): CVE-2026-41681, defect
  396's shape in rust-openssl's `digest_final`, CVSS 9.8, repaired in April
  2026 by a size check before the call; CVE-2021-45707, the out-count shape in
  `nix`; the FFIs that refuse the fault put N in the binding's type (D, Zig,
  Nim, Ada, Checked C); none found guards a lend at run time by default.

## Disagreements, stated plainly

- **S1**: the historian approves it on D's precedent, which presupposes a local
  fixed array (S0, vetoed by the compiler-engineer); on this tree S1 binds
  `SHA256_Final` from one file in no way (the compiler-engineer's prediction,
  the critic's run of the A reader's program). S1 is refused; the historian's
  reading holds for a language with S0.
- **The guard**: the compiler-engineer built a canary, the ffi-pragmatist a
  guard page; the critic measured each failing where the other holds, and the
  guard page as the one that can be made robust: per thread, per buffer,
  sized to N in pages, in stack order. Taken.
- **S4 on single cells**: approved as a backend check, approved only as a guard
  page, objected to as a spec promise. Resolved by R7: no copy (the veto), the
  local laid out in the guarded region, and § 13 promising only what the
  region does.
- **The sibling count**: by copy (the compiler-engineer) or in place (the
  ffi-pragmatist). Resolved by R3: a temporary reaches only a parameter
  declared `lent`, so a copy sized at the call is harmless where C keeps
  nothing, which is the ffi-pragmatist's own lifting condition.
- **The live scalar lends**: 34 against 36. The critic checked: 36, the
  library's two bindings held as string literals in
  `selfhost/library_source.hero` included.

## The resolution — ratified by the author (below)

The most robust and complete route at every disagreement (CLAUDE.md § 4,
CL-040); what conservative would have been is below the list.

1. **R1, the address.** An `@` argument that reaches C is its place's own
   address, never a copy: a binding, a field, an array's element (the array
   unshared first, then the element's own address), and through a Heroes
   function's own `@` parameter, which the emitter lowers by pointer so that
   the callee's place is the caller's. § 9's *copy in, copy out* stays the
   meaning inside Heroes, where an `@` place is exclusive (defect 011's rule:
   no two arguments of one place), which a lowering by pointer keeps (the
   critic's `alias.hero`); § 13 says what C receives: **the place itself,
   which C may keep only while the place does not move; a copy, a return and
   an array's growth move it**, the program's obligation, written where § 13
   writes C's other promises. Repairs defect 413 as widened. The mark that
   would let the checker hold the obligation (a group record that says it is
   not a value) is panel 178's open item
   (`issues/2026-09/25/2026-09-25-0003-m2-m-copies-a-pthread-mutex-t-which-posix-does-not-allow-and.md`),
   this sitting's measurements its evidence; not adopted here.
2. **R2, one element.** An unmarked `@x: T` on an `extern` promises ONE `T`,
   merged into § 13's out-parameter sentence (U).
3. **R3, `lent` on `@`.** Legal, and it says C keeps nothing of the address
   after the call; any temporary the emitter makes for a call (R4's buffer)
   reaches only a parameter so declared, while R1's place reaches either.
   `lent_shape` moves. Answers the critic's `setvbuf`.
4. **R4, the buffer C fills** (S7, complete). `@md: [T] counted_by E lent`, `T`
   a number, a group record (arrays of records: `poll`, `kevent`), or `u8`
   against a `void *` (the byte as the unit where the header gives none:
   `read`, `recv`, `arc4random_buf`); `E` a constant of the group, an integer
   literal, or a sibling parameter (a run-time count, sized at the call). The
   emitter owns the buffer: the array's first `min(len, E)` elements copied in,
   zeros after, exactly `E` copied out as the array's new value. **The buffer
   lives off the stack, in a per-thread guarded region**: created with the
   thread beside its alternate signal stack and released with it, sized to `E`
   rounded up to pages and grown when a larger `E` arrives, one region per
   buffer of the call, taken in stack order so that a callback into Heroes that
   makes another such call gets its own; a write past `E` faults on the guard
   page and the runtime's existing handler (`runtime/parts/stack.c`) names the
   call and the parameter; where the kernel writes, the call fails and returns
   its error, which § 13 says as such. A runtime ABI change.
5. **R5, the spelling of an extent.** `counted_by` names a sibling, a constant
   of the group, or an integer literal, on a field's `ptr` lend as on R4;
   `CParam` becomes `[ "@" ] ident ":" Type [ "counted_by" ( ident | integer )
   ] [ "lent" ]`. The checker half and the emitter half land in one commit (a
   checker-side extent alone checks nothing: the compiler-engineer). Panel
   194's route C, `counted_by` on a whole group record lent with `@`, gets its
   § 13 clause in the same sentence (it landed with none: the critic's first
   pass).
6. **R6, the header's own extent** (S5). A probe in a unit of its own reads an
   array parameter's declared bound in the header's own spellings (`int [2]`,
   `uuid_t`, `[static N]`, and `__counted_by` where clang exposes it) and
   refuses a lend whose extent is smaller, a single cell included. It catches
   `pipe`, `uuid_generate`, `encrypt` and `ctime_r`, and nothing in OpenSSL,
   which the record says and the spec does not promise.
7. **R7, a local lent in place, guarded.** A local lent through `@` to an
   `extern` call lives in the same per-thread guarded region for its
   function's life, in stack order, so C receives the local itself (R1) and a
   write past it faults by name. A field or an element keeps its record's or
   array's layout and cannot be guarded; for them the extent is the binding's
   word under R2, as every other C promise is.
8. **R8, the fixes and the messages.** `ffi_pointee`'s guess never drops an
   extent (it proposes `@md: u8` for an array today, the compiler-engineer);
   a `field_lend_extent` fix never rewrites a stated extent; `@md: [u8]
   counted_by n` where `n` names nothing is one message, not two.
9. **Refused**: S1 and S1′ (dominated by R4, the spec-warden's veto); S3
   narrow and widened (the default refusal checks nothing under `counted_by 1`
   and refuses the `frexp` control); S4 as a copy (the ffi-pragmatist's veto,
   joined by the compiler-engineer) and as a spec promise; S5′ (Apple's
   `-fbounds-safety`, one platform); S6 as the resolution; S0's local fixed
   array (the compiler-engineer's veto); nothing.

**What conservative would have been** (CL-040), the author's to choose
instead: R2 and S7 with constant extents only, a 16-byte canary with a
per-process random pattern on the stack, R1 limited to array elements and to
group records lent through a wrapper, no `lent` on `@` (a function that keeps a
buffer then has no binding but C), and R6 and R7 waiting.

**Defect 396** is ruled by R2 and repaired by R4 to R7 at the landing: its
reproducer `md_scalar.hero` (a local) faults by name under R7; `md_field.hero`
and the critic's `poll_cell.hero` (a field, a record cell) are the binding's
word under R2 unless R6 reads an extent, which the item records. It closes
after the landing's platform legs (a C-boundary defect). **Defect 413** is
widened by the critic's wrapper rows and repaired by R1 at the landing.

**The landing**, in three lanes whose files are disjoint: the address (R1:
`ir/inout.hero`, `emit/body.hero`'s prologue); the buffer, its region and its
spelling (R3, R4, R5, R7: `runtime/`, a new emitter module, `check/lend_extent`,
`parse/marks`); the header and the messages (R6, R8: `cli/pointee*`,
`emit/ffi_pointee`). Each repair's own cases on three platforms; the spec's
sentences priced by `--refresh` at the landing.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | the S7 part of the landing (R4, R5, the guard) reads 250 to 450 `selfhost/` insertions; with R1 out of it, its compiler emits the parent's `selfhost/main.hero` byte-identical to the parent seed; the `check` census moves 0 outcomes | the landing |
| ffi-pragmatist | under R4, `@md: [u8] counted_by 16` with SHA-256 aborts naming `SHA256_Final`'s `md` on Darwin and Linux arm64; under R1, `z_elem.hero` prints `0 0 0` on both | the landing |
| spec-warden | the composition (U, S5's clause, S7 with the sibling and the guard, S2's constant, route C's clause) reads 9,720 ± 8 real on this base (R1's and R3's clauses are unpriced, the landing's `--refresh` prices them); at the reader test (10 per arm, an S7 arm and a U-alone arm, every program built and run), 8 of 10 S7 readers build task 1 with no C file and 9 of 10 bind the `frexp` control correctly under U | the landing, the reader test |
| completeness critic | under R3 the critic's `setvbuf_keep.hero` is refused at `check`; under R4's per-thread region `gp_threads.c`'s shape gives 0 wrong digests; `poll_cell.hero` is unchanged at exit 0 (the binding's word) | the landing |

## Author's verdict

**RATIFIED, 2026-10-06**, R1 to R9 as written above, the author answering
through the question widget minutes after 23:45 by the clock read when it was
put (the next reading, 00:09, came after the disk was freed), choosing
*ratify* over the conservative alternative and over *I want to read it
first*, on the coordinator's summary of the nine points; recorded as a
reading (CLAUDE.md § 4). The ratification issue is
`issues/2026-10/06/2026-10-06-2341-panel-196-ratify-amend-or-overturn-r1-to-r9-an-argument-reaches-c-as.md`.
The author may overturn it (CLAUDE.md § 4).
