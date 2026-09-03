# 103 — the FFI pointer verdict goes strict, and the compiler learns the header's parameter types

**Convened 2026-09-03** by M-robustness-guards step 3 (`/decide` of the same day,
author instruction *"ratifica tutto"* and four criteria: robust over cheap,
consolidation over shortcuts, history over the token count, every platform and
never a silenced error). Full panel, five seats, differentiated by input; the
historian's weight raised and the spec-warden's token veto lowered by author
instruction. **Status** `provisional — author ratification pending`. Two author
instructions arrived during the sitting and are recorded where they bind: the
clang floor (below) and, from the same day's plan, that no `-Wno-` flag is an
option.

## The proposal, verbatim

1. `selfhost/cli/flags.hero::flags()` gains `-Werror=incompatible-pointer-types`
   (the 14th flag; the 13th is its sibling `…-discards-qualifiers`), so the
   verdict on a pointer mismatch is the same on every clang.
2. The emitter passes `(void *)&x` for an `@x: ptr` argument to an `extern`
   callee — C converts `void *` to any object pointer silently, `void **` to
   none — so `spec:215`'s own example, `sqlite3_open(path: cstr, @out: ptr)`
   against `sqlite3 **`, stays legal everywhere.
3. A numeric `@x: T` parameter of an `extern` is checked against the header's
   pointee by width and sign, the way a by-value parameter already is: `@n: i32`
   for `size_t *` is refused, `@n: u64` is accepted. This needs the compiler to
   LEARN the header's parameter types (mechanism: this sitting's to price).
4. `spec:209-211` narrows the exemption from *what a pointer points at* to *what
   a `ptr` points at*.
5. Panel 096's Q3 rides along: a clang complaint about the CALL of an
   author-declared `extern` — `printf(format: cstr)` used variadically,
   `strtok(target: cstr, sep: cstr, pad: str)` — becomes exit 1 on the `.hero`
   line under CLAUDE.md §7's `declaration()` narrowing, minting the 21st `ffi_*`
   code.
6. Q4, put as a question and not a premise: should `-Werror` be the rule for ALL
   generated C, with attribution (5) deciding exit 1 from exit 2?

## What was measured for the brief, and during the sitting

Default verdicts of four clangs on three pointer mismatches (`-std=gnu11 -c`):

| clang | `int *` → `long *` | `void **` → `char **` | `uint64_t *` → `size_t *` | `-ast-dump=json` shape for `getline` |
|---|---|---|---|---|
| Debian clang 18.1.8 (`silkeh/clang:18`; the CI's Ubuntu leg is 18.1.3) | warning | warning | ok | `size_t *restrict`, no `desugaredQualType` |
| Apple clang 21.0.0 (the Mac) | warning | warning | **warning** (`unsigned long long *` vs `unsigned long *`) | same |
| Debian clang 22.1.8 (the Linux image) | **error** | **error** | ok | same |
| clang 22.1.8 (the Windows box, `x86_64-pc-windows-msvc`) | **error** | **error** | ok | same, function type carries `__attribute__((cdecl))` |

The historian found the reason: upstream PR #157364, merged 2025-09-15, made
`-Wincompatible-pointer-types` an error by default in **Clang 22.1.0**
(2026-02-24); GCC 14 (2024-05-07) had done it a year and a half earlier after
Fedora rebuilt the distribution and fixed **381** packages rather than silence
the warning. The patch flipped only that diagnostic, so the 13th flag stays
necessary.

Live programs, measured by the seats: `getline(@line: ptr, @n: i32, stream:
ptr)` builds **exit 0 with five warnings** on the Mac and the binary prints
`read: 3 cap: 4` while `getline` writes 8 bytes into a 4-byte slot — and
**`--sanitize` is silent**. `@n: u64` and `@n: i64` build the same way. `time(@t:
i64)` against `time_t *` builds today at exit 0 with three warnings and would be
refused **on macOS alone** under the strict flag without a cast (`long *` against
`long long *`), exit 0 on Debian and Windows. Of the 35 `extern` programs in the
repository, 19 are clean under the 13 flags today, and the strict flag newly
refuses **exactly 2**: `examples/sqlite/main.hero` (8 sites, all `void **`
against `T **`) and `tests/golden/fixedbugs/ffi-out-parameter-guard.hero` (4); both
are repaired by item (2). Numeric `@` parameters in `examples/`: **0**. The
compiler's own source has one `@` at the C boundary, `hero_run_go(@status: i64)`
against `int64_t *`, exact; the warden's self-build in a copy: exit 0, zero clang
warnings, 56.6 s, so item (1) cannot break the fixpoint.

The spec: **3685** today. Wordings measured (cl100k_base): w2 3705 (+20), w2b
(`ptr` or a `cstr`) 3711 (+26), w5 **3718 (+33 net: +46 against the −13 of
relocating `spec:237`'s sentence)**, w4b 3721, w1 3726, w4 3732, w3 3733.

## The verdict table

| judge | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **APPROVE-WITH-CONDITIONS; object on (2)'s scope and on (6).** (1) approve conditional on (2)+(3) in the same step; (2) approve, **scope widened to every `@`**; (3) approve-with-conditions, ship the JSON dump + `_Static_assert`; (5) approve, cheaper than panel 096 priced; (6) object | design.md §1.1, §1.7/Part 5 (nothing here is a construct: a backend verification pass), §1.12, §4.19 | **~400–550 code_lines** (suite_layout's rule), all under `selfhost/emit/` and `selfhost/cli/`; 0 IR ops, 0 checker rules; no touched file crosses 300 and `inst.hero` (DECIDED 324) is not touched — the cast lands in `ops.hero:213-260`'s per-slot `guard_cstr_arguments`, `extern_probe.hero:162/:205`, `assert_spelling.hero:191-197`; (3): new `emit/pointee.hero` ~120–160 + reader ~60–90 + `produce.hero`/`units.hero` +20–30, **one clang process per cold unit, 0.02 s** (`extern __typeof__(<name>) hero_ty_<name>;` per numeric-`@` extern, `-ast-dump-filter=hero_ty_`, 6–8 KB per unit against 2.8 MB unfiltered); (5): new reader `emit/ffi_call.hero` ~70–100, because the probe reproduces the call's mistake at the DECLARATION line. Verified: the seed at 7666b0f emitting `&h0_bs` is exit 1 under the 14th flag, error at the CALL line, unclaimed → exit 2, the compiler's — correct. **Measured the brief's own hole**: under the strict flag alone, `getline(@n: u64)` is exit 1 on macOS with a FALSE `ffi_parameter_type` (the probe's `uint64_t *` against `size_t *`) and exit 0 on Linux; with (2) for `ptr` only, the call line stays an unclaimed error → exit 2 for a correct program. Tried and rejected: pure C11/clang tricks (no parameter-position `__typeof__`), a `transparent_union` redeclaration (uniform and free, but `conflicting types` the moment a parameter is `ptr`), DWARF (`llvm-dwarfdump` absent on the CI's Linux leg, CodeView on Windows) | at M-robustness-guards close, (1)+(2)+(3)+(5) add **350–550 code_lines** under `selfhost/emit/` + `selfhost/cli/`, `inst.hero` still reads 324, no touched file enters DECIDED; `examples/sqlite` exit 0 on Debian 22; `heroes build selfhost/main.hero` cold time grows < 2%. Wrong if the delta exceeds 700 or a DECIDED entry is needed | OBJECT if (1) lands without (2)-for-all-`@` and (3) in the same step; VETO only if (3) needs a new IR op or a checker rule; reprice if the JSON extraction needs a general parser (> 150 lines) or one process per extern rather than per unit |
| ffi-pragmatist | **object as written; APPROVE with three conditions.** (1) APPROVE-WITH-CONDITIONS — alone a VETO; (2) APPROVE; (3) APPROVE-WITH-CONDITIONS; (4) APPROVE; (5) APPROVE-WITH-CONDITIONS; (6) APPROVE-WITH-CONDITIONS at `-Wall` | design.md §4.19:1996-1997, §1.11:443; CLAUDE.md §12 | `(void *)&out` into `sqlite3_open` exit 0 on all three clangs under the strict flag, `&out` exit 1; `-ast-dump=json` **24 ms** per run against 22.5 ms for the plain syntax check, same shape on three clangs; DWARF works on Mac and Debian but the **Windows box has no `llvm-dwarfdump`**; the check is two `_Static_assert`s in the probe TU (`sizeof`, and `((T)-1 < 0)` for sign), measured refusing `i32` and `i64` for `size_t *` and `u64` for `time_t *`, accepting `u64` and `i64` respectively, identically on Mac and Debian; Q4: today's emitted C for six examples, the seed and the runtime recompiled with `-Werror`: 0 errors on Mac and Debian except sqlite's 8, all item (2)'s | under (1)+(2)+(3): `examples/sqlite` exit 0 with 0 warnings on all three platforms and `nm` still lists panel 099's 7 symbols; `getline_i32` exit 1 on `.hero:2` on all three with the width message, `getline_i64` exit 1 with the sign message, `time64` and `getline_u64` exit 0 on all three — any platform difference falsifies the mechanism | (1) never lands alone: same commit as (2) and as the **numeric-`@` cast**; (2) reaches all three spellings — the call (`inst.hero:178`), the probe function (`extern_probe.hero`), the `_Static_assert` (`assert_spelling.hero:190-193`); (3) checks width AND sign and passes `(void *)&n` after the check, type text from the JSON dump (exact `name` + `FunctionDecl`, tolerate redeclarations, strip `*`/`restrict`/`const`/`__attribute__`), never a width table; Q3 mints **two** texts (arity; `-Wformat-security` carrying clang's own fix) and a variadic bound at fixed arity stays green (`printf(format: cstr, value: i64)` builds and prints today, as does `examples/curl`); Q4 at `-Wall` only, `-isystem` for `package` headers if a level above it is ever proposed. **Veto if** (1) ships without the numeric cast or a width table replaces `sizeof` |
| spec-warden | **APPROVE-WITH-CONDITIONS.** w3 **VETO** (a false sentence), w1 OBJECT, w2 APPROVE, recommendation **w5**, w4b fallback; the ergonomist's `ptr` or a `cstr` **refused** | design.md §1.6, §1.2, §1.4, §1.3; CLAUDE.md §12; Part 6 preamble :2369-2378 | **3685 → 3718 (+33 net)**, paid as a §12 repair (today's spec PERMITS `@n: i32` for `size_t *`; the compiler will refuse it) plus a registered prediction; ledger row 49; `SPEC_TOKENS` 3685 → 3718 and `LEDGER_ROWS` 48 → 49 in the same commit; with w5 in a copy the harness reads `spec: 6 passed, 2 failed` on the two pins and `spec/rejected` green. Truth table over seven probes: `@lineptr: cstr` vs `char **` is **already exit 1 today** (`ffi_writable_parameter`), which is why `cstr` does not join the exemption; `@e: i16` vs `int *` builds today with three warnings and (1) refuses it, which w1's "checked the same way" would wrongly permit | (i) `heroes measure` reads **3718** at the landing commit; (ii) at M-robustness-guards close the pointee check has produced **zero false fires**: no `extern` in `examples/` or `tests/golden/` that built at exit 0 on 2026-09-03 is edited to pass it except one at the wrong width or sign; `examples/sqlite` exit 0 on the Debian image; the `@n: u64` `run/` golden exit 0 on three legs | (1) **VETO the spec text if it lands before** item (3) refuses `@n: i32` on all three platforms and item (2) makes `@out: ptr` clean — panel 060's false-by-fiat class, and this clause's third size in nine days (+12, +32, +33), which the ledger row states; (2) veto item (1) unless the pragmatist puts panel 092:277's falsifier on the record (done: `sqlite3_open` clean under the flag, three platforms) and `@n: u64` still builds on macOS; (3) plain APPROVE once a `check/` golden for `@n: i32` vs `size_t *` (exit 1, `#~`) and a `run/` golden for `@n: u64` (exit 0) exist and fire; Q3 and Q4 need **zero** spec tokens |
| llm-ergonomist | **APPROVE-WITH-CONDITIONS** on B's sentence; no veto (the construct is local: the line plus the header it names) | the thesis: a model reading the spec alone | blind A/B over three tasks: `size_t *` out-parameter `@n: u64` at 90 under A and 95 under B; the sqlite example 92/90; task 3 (`@n: i32` for `size_t *`) **accepted at 78 under A, refused at 92 under B**. B removes three silent mistakes (`@n: i32`, `@n: i64` for `size_t *`; `@w: i64` for `int *`) and adds none; B does not close the forgotten-`@` case (`db: ptr` against `T **`, compiles under both) | the silent-wrong rate on a `size_t *` pointee moves from **25–40% under A to 0% under B**; task 3 predicted correctly by ≥ 90% under B against ≤ 70% under A; the `T **` case moves by ≤ 5 points | (1) name `cstr` beside `ptr` — **refused by the warden on a measurement**, recorded here as a disagreement; (2) the refusal is a `.hero`-line diagnostic with a fix naming the header's type (`@n: i32` → `@n: u64`), not a raw clang error; (3) record that B does not close the forgotten-`@` case |
| historian (advisory, weight raised) | **ADVISE FOR** (1)–(5); (6) **SPLIT, leaning against a blanket** | the record | GCC 14 and Clang 22.1.0 flipped this diagnostic after distribution-wide rebuilds ("easily ignored, resulting in difficult to diagnose bugs"); cffi API mode is the one FFI that CHECKS a user-declared pointer parameter against the header, by letting the C compiler diagnose the generated call; cgo learns types from a probe's DWARF (`__typeof__(name) *__cgo__N;`, `-gdwarf-2`); GHC `capi` passes every pointer as `void*`; Microsoft's 64-bit checklist: a cast on an out-parameter is how a width bug passes the compiler and overwrites the next 32 bits; PyPy #5016 (2024): generated C broken by GCC 14's default — the emitter's class, like step 1's `&h0_bs` | the first Apple clang rebased on upstream after 2025-09-15 rejects `uint64_t *` → `size_t *` on macOS by default with no flag change from Heroes — so item (3)'s cast is needed regardless of (1) | none binding (advisory); would change the reading: a production tool parsing `-ast-dump=json` across clang majors without breakage (none found); evidence that Apple clang 21 is based on LLVM ≥ 22 |

## Disagreements, stated plainly

**DWARF or the JSON dump.** The historian's record favours DWARF: cgo, the only
production tool that learns C types without libclang, reads it, and nobody ships
a parser of `-ast-dump=json`. The pragmatist measured both on the three machines
and the record loses to the machine: the Windows box has no `llvm-dwarfdump`,
DWARF costs two processes and a second tool, and the JSON dump has the same
shape on clang 18, 21 and 22 (measured on all four, this sitting) at 24 ms
against 22.5 for the syntax check that already runs. **The resolution takes the
JSON dump**, and the author's instruction received mid-sitting is what makes the
historian's objection answerable: *"se serve puoi richiedere una versione minima
di clang recente a piacere"* — a floor turns "parse every clang ever" into
"parse the clangs at or above N", and the floor is set from measured versions.

**`cstr` in the exemption.** The ergonomist, reading the spec alone, asked for
*what a `ptr` or a `cstr` points at*. The warden measured that a `cstr`'s pointee
IS checked today — `@lineptr: cstr` against `char **` is `ffi_writable_parameter`
at exit 1 (panel 058, `spec:235-236`) — so exempting it would write a false
sentence. The warden's reading stands; the ergonomist's condition is recorded as
what a blind reader wanted and why the document must not say it.

**A blanket `-Werror`.** The pragmatist measured it free today at `-Wall` (0
errors over six examples, the seed and the runtime, Mac and Debian, once
sqlite's 8 are repaired by item 2) and approved with the condition `-Wall` only.
The historian's record says every compiler that tightened did so one named
diagnostic at a time after measuring a corpus, and leaned against the blanket.
**The resolution takes the record's shape and the pragmatist's measurement
together**: no blanket; the strict flag is named; `-Werror` on all generated C
is not adopted by this sitting and returns, if ever, one diagnostic at a time
with the corpus measured.

## Resolution — `provisional — author ratification pending`

**Items (1), (2), (3), (4), (5) land, together, in one step, under every
condition below. Item (6) does not.** Robust over cheap, as instructed; and one
piece the brief did not know it needed — the numeric cast — is what the seat
that compiles found.

1. **The flag**: `-Werror=incompatible-pointer-types` is the 14th flag in
   `selfhost/cli/flags.hero::flags()`, with its measured reason, and its test
   pins fourteen. Never a `-Wno-` flag (author instruction).
2. **The cast, on EVERY `@` argument to an `extern`, in all three spellings**:
   `(void *)&x` at the call — in `emit/ops.hero`'s extern arm, whose
   `guard_cstr_arguments` already matches the callee's declared slot type per
   argument, so `emit/inst.hero:178` (DECIDED 324) is not touched — in the
   probe function (`emit/extern_probe.hero:162`, `:205`) and in the
   `_Static_assert` (`emit/assert_spelling.hero:191-197`, a bare `0` as `ptr`
   already gets). The probe and the call disagree today. The numeric `@x: T`
   is cast too, **after** (3) has checked it, because `long *` and `long long *`
   are the same 8 bytes and different types on macOS (`time(@t: i64)`,
   `getline(@n: u64)` measured by both compiling seats; the engineer measured
   the strict flag alone turning `getline(@n: u64)` into a FALSE
   `ffi_parameter_type` on the Mac and a clean build on Linux). Blessed
   emissions move with it: 172 `tests/emission` files carry the library's
   `(int64_t *)0`, re-blessed alone, as one commit.
3. **The check, the compiler's own**: **one clang process per cold unit** (the
   engineer's shape, 0.02 s measured against the syntax check that already
   runs): a dump TU declaring `extern __typeof__(<name>) hero_ty_<name>;` for
   every `extern` function of the unit with a numeric `@` parameter, run as
   `clang -fsyntax-only -Xclang -ast-dump=json -Xclang -ast-dump-filter=hero_ty_`
   (6–8 KB per unit against 2.8 MB unfiltered); the k-th `ParmVarDecl`'s
   `qualType` is the header's own text for the parameter, stripped of `*`,
   `restrict`, `const` and `__attribute__` — the same shape on clang 18, 21 and
   22, measured. The verdict is not read from the JSON: it is asked of clang in
   the probe block that already exists, two `_Static_assert`s at the
   DECLARATION's `#line` — `sizeof(<header type>) == sizeof(<T>)` and
   `((<header type>)-1 > 0) == ((<T>)-1 > 0)` — so `extern_at_line` claims the
   failure and it comes out as `ffi_parameter_type` at exit 1 on every clang,
   naming the written type and the header's, with a `guess` fix spelling the
   header's width (`@n: i32` → `@n: u64`). Width and sign by the header's own
   text, **never a width table**; the dump's exit code never fails the build
   (a macro-only name is the probe's to refuse); cached under the unit's key.
   Tried and rejected by the seats: a `transparent_union` redeclaration
   (uniform and free, but `conflicting types` the moment any parameter is
   `ptr`), DWARF (two tools, one absent on the CI's Linux leg and CodeView on
   Windows), and every pure-C11 trick (no parameter-position `__typeof__`).
4. **The spec**: w5 lands in the same commit as (2) and (3), never before
   (warden's condition 1):

   ```
   -and what a pointer points at:
   +and what a `ptr` points at. A C out-parameter is an `@` parameter, and what it
   +points at is held to the same width and sign — `@n: u64` where it says
   +`size_t *`:
   ...
   -`str` to C to read and `c.validated()` copies one back as a `str?`. A C
   -out-parameter is an `@` parameter.
   +`str` to C to read and `c.validated()` copies one back as a `str?`.
   ```
   3685 → **3718**, +33 net; ledger row 49 says *a move, not a saving* for the
   −13 and names this as the clause's third size in nine days; `SPEC_TOKENS` and
   `LEDGER_ROWS` move in the same commit.
5. **Attribution (Q3)**: a clang complaint about the CALL of an author-declared
   `extern` is exit 1 on the `.hero` line — the 21st `ffi_*` code, minted for
   two texts: arity (`too many arguments to function call`, attributed from the
   probe-mapped line, the call-line duplicate swallowed, the raw `.c` line
   ignored) and `-Wformat-security` (carrying clang's own *treat the string as an
   argument*). A variadic bound at fixed arity — `printf(format: cstr, value:
   i64)`, `examples/curl`'s `curl_easy_setopt` — stays green and is the golden.
6. **Q4 is not adopted.** `-Werror` on all generated C is free today at `-Wall`
   and the record says tighten one named diagnostic at a time; this sitting
   named one. If a level above `-Wall` is ever proposed, `package` include paths
   go in as `-isystem`, which is not a `-Wno-` flag.
7. **A clang floor** (author instruction, this sitting): the compiler requires
   **clang 18 or newer** — the oldest version measured, the CI's Ubuntu leg —
   enforced by `heroes doctor` and by the build, exit 2 naming the version found
   and the floor. Measured: 18.1.8, 21.0.0 and 22.1.8 give the same
   `-ast-dump=json` shape; the flag's default flips only at 22, so the floor
   does not make (1) redundant and (1) does not make the floor redundant. A
   higher floor today would break the CI's Linux leg for nothing.
8. **Goldens and platforms**: `check/` for `@n: i32` against `size_t *` (exit 1,
   `#~`) and for `@n: i64` (the sign case); `run/` for `@n: u64` against
   `size_t *` and for `time(@t: i64)` (exit 0); `examples/sqlite` at 0 warnings
   on a cold cache; all measured on the Mac, the Linux image and the Windows
   box before the commit.

## What a veto would compel

A veto of (1) leaves the verdict to the host clang — today two of the three
measured platforms refuse the spec's own example and the third accepts a write
out of bounds with a warning. A veto of (3) leaves `getline(@n: i32)` writing 8
bytes into 4 at exit 0 under `--sanitize`, the memory-safety hole §12 names
first. A veto of (2) with (1) kept breaks `examples/sqlite` and every `T **`
out-parameter in the corpus. A veto of the JSON mechanism in favour of DWARF
compels a second tool on a platform that does not have it.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| ffi-pragmatist | under (1)+(2)+(3): `examples/sqlite` exit 0, 0 warnings, 7 `nm` symbols, on three platforms; `getline_i32` exit 1 on `.hero:2` with the width message on three; `getline_i64` exit 1 with the sign message on three; `time64` and `getline_u64` exit 0 on three | M-robustness-guards step 3's landing |
| spec-warden | `heroes measure` reads 3718 at the landing commit; zero false fires of the pointee check over `examples/` and `tests/golden/` by the milestone's close; `examples/sqlite` exit 0 on the Debian image | landing commit · M-robustness-guards close |
| llm-ergonomist | silent-wrong rate on a `size_t *` pointee 25–40% under A → 0% under B; task-3 prediction ≥ 90% right under B | the next Part 11 harness run over FFI tasks |
| historian | the first Apple clang rebased on upstream after 2025-09-15 rejects `uint64_t *` → `size_t *` by default with no flag change from Heroes | the next Apple clang major |
| compiler-engineer | (1)+(2)+(3)+(5) add 350–550 code_lines under `selfhost/emit/` + `selfhost/cli/`; `inst.hero` still 324; no new DECIDED entry; `examples/sqlite` exit 0 on Debian 22; cold self-build < 2% slower | M-robustness-guards close |

## Corrections to the brief, which the coordinator wrote

- The brief cast only `ptr`. The pragmatist showed that the strict flag then
  refuses `time(@t: i64)` and `getline(@n: u64)` on macOS alone — uniform by
  platform lottery. The numeric `@` is cast too, after the compiler's own check.
- The brief said the flag makes the verdict uniform; it never checks a pointee's
  **sign** (`-Wpointer-sign` is a warning on all three clangs). Item (3) is not
  redundant with (1) on any platform.
- The brief offered `ptr` or `cstr` as a wording; a `cstr`'s pointee is already
  checked for writability (exit 1 today), so the sentence would be false.
- The brief's "Ubuntu clang 18 warns" was cited from `LINUX-MACHINE.md`; it is
  now measured directly on 18.1.8 in the `silkeh/clang:18` image, warning on both
  mismatches, and its JSON dump shape matches 21 and 22.

## Author's verdict

**Pending.** The item that asks for it is open in `docs/work/DECIDE.md` and names
this sitting as `panel 103`; work proceeds on the provisional default (CLAUDE.md
§4 — a panel never blocks). Two instructions were given during the sitting and
are recorded above: the clang floor is the coordinator's to choose from measured
versions, and no `-Wno-` flag is an option.
