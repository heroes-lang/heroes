# Panel 215, the compiler-engineer

Written 2026-10-14 from 08:31 to 10:30 in the copy `.claude/worktrees/scratch-panel-215/compiler-engineer`
at `7e5c02048`, its compiler built from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`, real 9.00, user 8.59, `heroes 0.4.0`; load 2.36 at the start, 4 to 10 for most of the
session, another lane's full net running). Every probe, script, output and the prototype's diff are
kept in `docs/panel/215-reports/compiler-engineer-work/` (`prototype.diff`, `padding.hero`,
`pinned.hero`, `probes/`, `pin/`, `msan/`); the copy is removed.

## The verdict

- **verdict**: **object**, with four soundness vetoes on named routes and three routes approved.
  Q1: **veto** B, C and D as closures of issue 1334, and **veto** route G in its naive form;
  **adopt G with the header's membership, plus H**. Q2: **adopt B**, refuse A, A2, C (as the
  decider) and D. Q3: **adopt A with D**, refuse B.
- **section**: design.md §1.7 and Part 5 (no adopted route adds a core construct: G is the emitter
  and the runtime, B is the checker, both erased before the IR); §1.12 *"check rather than assume
  ... at the boundary even where the value is known to be good"* for where Q1's remedy goes; spec
  § 13 *"C receives the place itself"* (panel 196's R1) for Q1's veto of D. **design.md does not
  cover padding reaching C**: it names padding only for `eq` and `hash` (§4.20, line 2909), and §1.12
  names segfaults and memory corruption, not a leak. The Q1 objection is the CLAUDE.md § Precedence
  rule (*the most robust resolution*), not a design.md section.
- **implementation_cost** (prototype, built and run; `git diff --numstat` and `wc -l` in the copy):
  Q1's G: `selfhost/emit/padding.hero` new, 83 lines (54 of code); `selfhost/emit/ops.hero` (441)
  +2; `runtime/parts/lend.c` +20, `runtime/heroes_runtime.h` +2, one word in each of the three guard lists; the rounds
  carrying the screen's verdict, `selfhost/cli/layout.hero` (621) +19 -5, `selfhost/cli/probing.hero`
  (262) +9 -1, `selfhost/cli/header_types.hero` +2. **0 lines in `selfhost/check/`, `selfhost/ir/`,
  the lowering.** Q2's B: `selfhost/check/pinned.hero` new, 186 lines (132 of code, the shape of
  `check/leasing.hero`, 311); `selfhost/parse/records.hero` (208) +10 -1; `selfhost/ast.hero` +3;
  `selfhost/checker.hero` +4; five test constructions. **0 lines in the IR, the emitter, the
  runtime.** Owed and unbuilt: Q1's G for `partial` records and records with anonymous members or
  bit-fields (from the dump's rows, `emit/layout_text.hero` already reads them); Q2's B for maps,
  `T?`, variant cases and generics, and the re-printing walk (**the formatter deletes the word**,
  measured), the diagnostics' fixes and goldens. For scale: `selfhost/` is 633 files and 165,631
  lines (`find selfhost -name '*.hero' | xargs cat | wc -l`), `check/` 21,424, `emit/` 31,397,
  `cli/` 28,779, `runtime/` 14,291; Pascal-P4's 4,000 is far behind, and neither route moves that.
- **needed_for_self_hosting**: **no**, for all three. The compiler binds 15 `extern` groups and **no
  group record** (`grep -rn "^extern " selfhost` 15; the awk over their members finds no `record`),
  and its own emitted C is byte-identical under the prototype (below).
- **argument**: On x86-64 a padded group record's padding is lost in a by-value call or return and
  reaches C at **every level, `-O2` included**, through a field lend; the critic's clean `-O1` to
  `-O3` is the zeroed lend region's accident. B, C and F cannot reach a record C itself returns by
  value; D hands C another address. Zeroing, at the lend, exactly the bytes no C member holds closes
  every shape measured on x86-64, costs nothing measurable in the compiler, moves 7 of 1,265 blessed
  emissions, but is unsound if the membership is the declared fields (a union arm corrupted). Q2's
  mark is checker-only and moved 0 of 905 files.
- **prediction**: at M-buildable-structs' close, route G landed with the header's membership is at
  most **250 lines** across `selfhost/emit/`, `selfhost/cli/` and `runtime/`, **0** in
  `selfhost/check/` and `selfhost/ir/`; `emission` moves **at most 10** blessed emissions and `run`
  none; and MemorySanitizer on `heroes-linux` reads **0 reports** for `fieldlend.hero`, `cret.hero`
  and `rzpad.hero` at `-O0` to `-O3`. Route B landed is at most **450 lines** across `check/`,
  `parse/`, `print/` and the AST, and moves **0** of the 905 `extern` files under `check`.
- **condition**: Q1 turns to approve-as-is (A) if the loss is shown never to reach C above `-O0`
  on a place outside a zeroed region (my `fieldlend.hero` says it does, at `-O2`); my G turns to
  veto on any leg where it writes a byte a C member holds, or where a cold `--emit-c` of the
  compiler costs over 1% CPU. Q2 turns from B to C if clang exposes a C attribute meaning *not
  copyable* that glibc, Darwin and Windows headers use (I found none, unsearched in clang's
  attribute list: a question). Q3 turns to withdraw on any leg showing a field or a union none named
  nonzero after `rest: zero`.

## Q1, padding across a call by value (issue 1334)

### What was measured

MemorySanitizer, every probe at `-O0` to `-O3` and at `-O1` to `-O3` with `-fno-inline` (a real
call, as between two modules: one `.c` per module, `selfhost/emit/signature.hero:83-84`), the real
build's flag list, Debian clang 22.1.8 in both containers. Scripts `compiler-engineer-work/probes/
msan_ce.sh` and `leg.sh`; outputs `compiler-engineer-work/msan/`.

| program | `heroes-linux` x86-64, today | arm64, today | x86-64, prototype G | arm64, prototype G |
|---|---|---|---|---|
| positive control, 178's `ctl.h` | 1 at all 7 | 1 at all 7 | 1 at all 7 | 1 at all 7 |
| critic's `small.hero` | **3 at `-O0`**, line 29 | 0 | 0 | 0 |
| `small_rz.hero` (route B's mechanism) | **3 at `-O0`**, line 29 | 0 | 0 | 0 |
| `cret.hero`: C returns the record by value | **3 at `-O0`**, line 13 | 0 | 0 | 0 |
| `fieldlend.hero`: `@x.h`, a field of a Heroes record | **5 at `-O0`, 2 at `-O1`, `-O2`, `-O3` and with `-fno-inline`**, lines 26 and 28 | 0 | **0 at all 7** | 0 |
| `rzpad.hero`: `rest: zero`, returned, read whole | **4 at `-O0`**, line 24 | 0 | 4 at `-O0` (unbuilt half) | 0 |
| `uni.hero`: union arm C wrote | 0, prints `1234605616436508552` | same | same value | same value |
| five `ffi-rest-zero-*` goldens | all `.expected`; partial-record 2 at `-O0` (the runtime's `hero_lend_sum` reading the region) | all `.expected`, 0 | all `.expected`; partial-record still 2 at `-O0` (a `partial` record: unbuilt half) | all `.expected`, 0 |

Windows: `clang -fsanitize=memory` on the box answers *unsupported option '-fsanitize=memory' for
target 'x86_64-pc-windows-msvc'* (clang 23.1.1), so that leg has no instrument; whether its ABI
loses padding is **unrun** (an inference from the Windows x64 convention that it does not).

**The drop from four levels (2026-09-25) to one (2026-10-14), measured.** Panel 178's original
probe with the compiler of `14a2a8591` (2026-10-06, the last seed before panel 196's R7,
`40c055a33`, 2026-10-07; an ancestor of it, `git merge-base --is-ancestor`): its C holds no lend
region (`hero_lend_local` 0 times) and MemorySanitizer reports at `small.hero:29` at **`-O0`, `-O1`,
`-O2` and `-O3`**, 1 each (`compiler-engineer-work/probes/prer7-run.sh`, run in `heroes-linux`). So
R7's zeroed region is what took the upper three levels away, not a repair of the loss.

**What it says.** The loss is real and x86-64 SysV's: an eightbyte holding a member and padding is
passed or returned as the member alone. The critic's `-O1` to `-O3` zeros are an accident: a local
lent whole lives in a region `hero_lend_local` zeroes (`runtime/parts/lend.c:452-470`), and above
`-O0` clang stores into it field by field. **A field lend has no such region, and loses the bytes
at every level, `-O2` included**, where `heroes run` compiles: `fieldlend.hero:26` after a Heroes
by-value call, `:28` after C's own by-value return.

### The routes

- **A, nothing**: refused. The loss reaches C at `-O2` (above). design.md does not rule on it
  (above), so the refusal is § Precedence's *most robust resolution*, given a route at the cost below.
- **B, every construction zeroes**: **veto as a closure**, measured twice: the critic's 3 reports
  stand with `rest: zero` at the same line, and `cret.hero` and `fieldlend.hero:28` lose bytes C
  produced, which no construction of the program's touches.
- **C, a group record crosses a call by address**: **veto as a closure**, by `cret.hero`: the bytes
  are lost in C's own by-value return, with no Heroes function between, so no Heroes calling
  convention reaches them. Priced, unbuilt: `emit/signature.hero` (165) `one_param`,
  `emit/heroes_call.hero` (123), every parameter read, returns through an out-pointer across
  `ir/exits.hero` and `ir/one_exit.hero`, function-value typedefs and callback thunks: more lines
  than G for less coverage.
- **D, a copy into a zeroed temporary before C**: **veto on soundness**. C would be handed an address
  other than the place, against spec § 13 (*"C receives the place itself, which it may keep only
  while the place does not move"*) and panel 196's R1. The tree's own
  `fixedbugs-396-a-local-lent-in-place-is-guarded` binds a `struct keeper` holding `self`, its own
  address, set by C: under D it would point into a dead temporary. Reasoned on that case; D unbuilt.
- **E, padding named as a field nobody reads**: impossible as stated, since a group's record IS the
  header's struct (§ 13) and Heroes cannot add a member to it; what E reaches for, the bytes no
  member holds, is G's input.
- **F (critic's), `memcpy` from a zeroed callee-side slot**: the bytes are gone in registers before
  the callee runs, so F needs C's address-passing, and still misses `cret.hero`. Refused.
- **G, zero at the boundary, in place: adopted, with the header's membership.** Before every
  `extern` call, each group record it lends (a whole cell, a field, an `@` parameter handed on) has
  the bytes **no C member holds** written zero, in place, so the address C sees is the place's.
  Built: `emit/padding.hero` writes `{ static const size_t hero_pad_spans[] = {offsetof(T, f),
  sizeof(((T *)0)->f), ...}; hero_pad_zero(&place, sizeof(T), hero_pad_spans, n); }`, C's own
  arithmetic, identical on every target.
  - **Its naive form is vetoed, measured**: membership taken from the declared fields zeroes the
    other half of a union arm C wrote. `uni.hero` (`struct s { int32_t kind; union { int32_t fd;
    void *h; }; }`, bound with `kind` and `fd`, which § 13 allows) printed **`1432778632`**
    (`0x55667788`) where today prints `1234605616436508552`: a wrong pointer at exit 0.
  - **The sound form**: only records the layout screen vouches for, whose fields are exactly the
    struct's members with no anonymous member and no bit-field (`emit/layout_screen.hero:14-26`),
    and none `partial`. The verdict is carried from `cli/layout.hero`'s `answered` to the emitter on
    the pointee answers (`lend_units`, key `%vouched <decl>`). With it, `uni.hero` keeps its value
    and nothing is written for it; the probes read clean (table).
  - **Unbuilt half, measured as such**: `rzpad.hero`'s `PU` (an anonymous union) and `PP`
    (`partial`) are skipped, and keep their 4 reports at `-O0`. They need the dump's rows, which
    the screen already asks for such records (`cli/layout.hero:179-185`).
  - **Reach**: `emission` with the prototype reads **1,258 passed, 7 failed** of 1,265, each failure
    only the zeroing lines (`struct linger` before `getsockopt`, `struct rlimit` before
    `getrlimit`, the self-pointing `struct keeper`, and `ffi-counted-by-a-stated-extent`,
    `ffi-lent-on-an-out-parameter` and two more `fixedbugs-092-*`); `run` **518 passed, 0 failed**:
    no program's output moved, those seven included.
    Census of `check` over the 905 files: **0 lines differ**. The 21 `examples/` programs with an
    `extern` (`grep -rl '^extern' examples --include='*.hero'`) emit **byte-identical** C under the
    prototype (`cmp`, 21 of 21): no example lends a group record the screen vouches for.
  - **Compiler cost**: a cold `--emit-c` of the compiler from an empty build directory, two runs
    each, alternating, user+sys: today **90.01, 90.19 s**; prototype **89.18, 91.06 s** (load 4.7 to
    5.5). The prototype emits the pristine compiler source **byte-identical** to today's, 123,660,697
    bytes (`cmp`): a program that lends no group record gets today's C.
  - **Run-time cost** at a hot lend, 50 million lends of `struct cd` to an inline C function at
    `-O2`: today 0.19 s user; the prototype's out-of-line helper 0.94 to 0.95 s (about 15 ns a
    lend); the helper inline, which clang folds to two stores of the seven bytes (`stur wzr, [x0,
    #1]` and `str wzr, [x0, #4]`), 0.45 s (about 5 ns). Load 7.6 to 8.9: noisy, reported, not a
    verdict. The landing should take the inline form and write nothing for a record whose fields'
    sizes sum to its size.
  - **Residual, reasoned and unrun**: Heroes programs run threads (`hero_thread_spawn`, bound by
    `tests/golden/run/fixedbugs-two-threads-over-one-allocator-give-back-their-own-handles.hero`).
    G never writes a byte a member holds, so a mutex another thread is locking keeps its bytes; a
    padding byte written while another thread copies the whole struct is a race on bytes no member
    holds. The landing states it in H's sentence rather than leaving it to be found.
- **H, the guarantee owned by `.claude/rules/generated-c.md`**: adopted **with G**, beside *"eq or
  hash walks fields, never bytes"*. Whether § 13 says one clause is the spec-warden's.

## Q2, a record C forbids to copy (issue 1335)

**Measured first** (`compiler-engineer-work/pin/mx.hero` over `mx.h`, `struct box { pthread_mutex_t
m; long n; }` bound `partial` with `n`): `m2: Box @= m` after `lock m` reads `trylock m2` **16**,
then `unlock m2`, `lock m2`, `unlock m`, `unlock m2` each **0**, both destroys 0, exit 0, on this Mac
and on glibc 2.41 (arm64). Two locks where the program wrote one, and no message.

- **A**: refused, a wrong program accepted at exit 0 (`.claude/rules/verification.md`'s `blocking`
  class).
- **A2** (`partial`, the mutex undeclared): refused; the critic measured the copy copies the bytes,
  and so does my run above, which is exactly that shape.
- **B, a mark on the group record: adopted.** Prototype, placeholder spelling `pinned` after
  `partial`'s place (a contextual word, as `partial` is, `parse/records.hero:88-92`), and
  `check/pinned.hero`, the lease's clause-1 shape: an expression of a pinned type (marked, or holding
  one by value, directly or through a fixed array) stands only as an `@` argument, the base of a
  field or index, or a construction or group constant initialising a cell or a construction's field;
  a parameter that is not `@` and a result of a pinned type are refused. Measured:
  - refused: issue 1335's `m2: Box @= m` (1); the critic's `copy.hero` (10, each a real copy: the
    by-value parameter, the result, `d: Counter @= c`, `return d`, and per call its argument and its
    returned value; a landing says the root once); a Heroes holder's copy `h2 = h`; an element `[m]`;
  - legal and unchanged: the critic's correct `b.hero`; a field lend `@h.b` of a pinned record built
    inside a Heroes record; an `@` parameter handing it on; field reads and writes (`legal.hero`
    prints `1`, then `6 1`); `fixedbugs-094` (initialised from a group constant) and
    `ffi-a-typedef-the-note-names-binds-by-its-own-name`, both marked, build and print their
    `.expected` byte for byte;
  - census: `heroes check` on the 905 files (`git grep -l -E '^[[:space:]]*extern ' 7e5c02048 --
    '*.hero' ':!docs'`), today against the prototype, eight at a time: **0 lines differ** (732 exit 0
    and 173 exit 1 for both). Population: 3,495 record lines inside a group in 392 files, 3,001 of
    them in three thousand-deep cases, 37 `partial`; 4 files name a lock or atomic type, 2 hold a
    mutex in a record, neither copies it.
  - **Not core** (§1.7, Part 5): a restriction asked in the checker and gone before lowering.
  - **Owed by a landing** (CL-036): `heroes fmt` on a marked record returns `record Box tag box
    partial`, **the word deleted at exit 0**; the dump printers, `heroes probe`'s reader, `mutate`,
    the editor grammar and the site's highlighter, the `grammar` suite's contextual words, the
    spec sentence, `Fix`es and annotated goldens; maps, `T?`, variant cases and generics, which the
    prototype does not walk.
- **C, decided from the header**: refused as the decider, for four reasons, two measured here. The same mutex is
  spelled `struct _opaque_pthread_mutex_t` in Darwin's record-layout dump and `pthread_mutex_t`
  (a union of `struct __pthread_mutex_s` and `char[48]`) in glibc's, so the rule is a per-libc name
  list, a premise about the world that goes silent on every type not on it
  (`.claude/rules/module-shape.md` § A narrowing asks the value); header facts reach `heroes build`
  and not `heroes check` (the critic's `v2.hero`: `check` exit 0, `build` exit 1
  `ffi_incomplete_record`), so the copy would be told only at build; and it needs a dump of every
  group record, which the screen exists to avoid (`emit/layout_screen.hero:5-12`, cited, not re-run
  by me). Kept as a later
  *note* that suggests the mark where a dump already ran.
- **D, a check at run time**: refused, reasoned and unrun: the group record is the header's struct,
  with no room for an identity, and a copy has the original's bytes.

## Q3, R1's payment

- **Panel 194's second condition is not met**: no field and no union none named is nonzero after
  `rest: zero`, on either Linux leg, at any of seven levels (the five `ffi-rest-zero-*` goldens
  `expected:same`; `rzpad.hero` counts ` 2 2 1 1` nonzero bytes over the WHOLE object, exactly the
  named fields'). **Padding** is lost through a by-value return on x86-64 at `-O0` (`rzpad.hero:24`,
  4 reports): R1 never promised padding; that is Q1's class, and G with the dump's rows closes it.
- **What withdrawing R1 costs the tree**: four modules of its own, 560 lines
  (`parse/rest_words.hero` 65, `resolve/rest_zero.hero` 259, `check/rest_zero.hero` 43,
  `emit/zeroed.hero` 193), hook lines in 19 more, 24 golden cases plus a harness line
  (`git grep -l "rest: zero" 7e5c02048 -- 'tests/**/*.hero'`), spec lines 383 and 389; it landed as
  +456 -79 in `selfhost/` and +2,360 in `tests/` (`838d27198`). No program outside `tests/` and the
  compiler uses it (the critic's grep, 0).
- **Adopt A with D**: R1 stands, judged on panel 194's conditions; 178's condition recorded void as a
  superseded sitting's. **Refuse B**: no measured failure of R1's promise, and its cost is paid.
  Panel 194's first condition, the reader test, is the paid run's and unscored by me.

## Not run

The Windows box's padding behaviour (no MemorySanitizer target there, measured); route C of Q1 and
route D of Q1 and Q2 (priced by reading, not built); G's dump-fed half for `partial` and anonymous
members; B's containers and the re-printing walk; the full net with either prototype (`emission`, `emit`, `check`, `run` and the compiler's own tests
only); the reader test (paid); any sentinel-value probe without MemorySanitizer.

**One slip of mine**: a canonical check of the kept programs wrote one temporary file under `/tmp`
(the formatter's output), removed by the same command; nothing else was written outside the
repository's root. That check found the seven kept programs carrying `pinned` not canonical, each by
the one line holding the word and nothing else: the formatter's deletion again, so they are kept as
written.

## Working log

### 08:36, Q1 on Linux arm64 under MemorySanitizer

My script (the critic's, with the real build's flag list from `selfhost/cli/flags.hero:129-149`,
each `.expected` compared by `cmp`, and `-O1`, `-O2`, `-O3` each with `-fno-inline`). Run as
`docker run --rm -v "$PWD":/src:ro heroes-linux-arm64:latest bash /src/build/pad/msan_ce.sh pad
<programs>`. The positive control reports 1 at every level; every other program 0 at every level,
every golden `expected:same`; `rzpad` prints ` 2 2 1 1` everywhere.

### 08:40, Q2, issue 1335's measurement

As in § Q2. A `partial` record with no field at all is refused (`empty_record`), so the mutex is
bound inside `struct box`, the shape a program writes.

### 08:42 to 09:40, Q1 on Linux x86-64 and the naive prototype

The x86-64 column of the table, today's compiler. Then the naive G: built, the probe's C carries
the zeroing before each lend, and `uni.hero` prints `1432778632`: vetoed.

### 09:40 to 10:00, the sound prototype

The screen's vouched set carried to the emitter (`prototype.diff`). Built with today's compiler
(`heroes build selfhost/main.hero`, real 34.95, user 34.09). `uni.hero` keeps its value; the
probe's six lends and `fieldlend.hero`'s three are zeroed. Both legs as in the table.

### 10:00 to 10:20, Q2's prototype, census and the suites

`heroes-g3` (both prototypes; real 63.83, user 53.28, load 6 to 10), each judged by itself
(`heroes-g3 run tests/harness/main.hero -- heroes-g3 <form>`): `emission` 1,258 passed, 7 failed
(`compiler-engineer-work/suite-emission-g3.txt`, the seven above); `emit` **12 passed, 0 failed**;
`check` **699 passed, 0 failed**; the compiler's own tests (`heroes-g3 test selfhost/main.hero`)
**1,653 tests, all passed**, exit 0; the census of 905 files, 0 lines differ; and `run` **518
passed, 0 failed**, two skipped for `sys/prctl.h`, which this Mac lacks, exit 0 (10:12 to 10:40,
load 5 to 10; `compiler-engineer-work/suite-run-g3.txt`).
