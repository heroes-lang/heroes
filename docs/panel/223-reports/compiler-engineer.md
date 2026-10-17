# Panel 223, compiler-engineer: the padding bits beside a named bit-field

Seat: implementation cost and soundness (veto). Work copy:
`.claude/worktrees/223-compiler-engineer/` (below, `<copy>`), made 2026-10-17 by `git -C <lane>
archive 3c478500b | tar -x`, stage 1 built there by `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (`real 7.94` by `/usr/bin/time -p`), `./heroes --version` printing
`heroes 0.4.0` / `sources 5f45d6514b22d650`. Scripts and probes are under `<copy>/w/`, git-ignored,
inside the repository's root. On the Windows box, `/c/w/223-compiler-engineer/` only. Written
against the coordinator's deadline of 19:00; what that left unrun is § 6.

**Summary.** The defect reproduces (1 MemorySanitizer report at `-O0`, byte 1 of `struct bf`). Route
A as listed, a plain `byte &= mask`, **changes the values of bit-fields that C writes under its own
lock**: measured in C on three legs (430,340, 1,293,211 and 867,513) and through a Heroes program
the landed compiler accepts (1,764 and 6,252 on this Mac, 3,849,257 and 3,877,881 on the Windows
box, two runs each; the landed compiler 0). I veto it on design.md §1.12. The same
route with an **atomic `and`** per shared byte changed 0 values on every run, reads 0 MemorySanitizer
reports at all five levels, keeps every member's bytes in a value probe of six records, and costs
+92 code lines over five `selfhost/emit/` modules, +58 runtime lines, and +6 ns a lend of a
bit-field record on this Mac, about +8 ns on the box (0 for any other); it moves 1 blessed emission
of 1,299 beyond the key line every runtime change moves. I built it and approve it. R1 as landed already writes inside
C's access unit, on the MSVC leg (measured on the box) and on every leg for a shape beside it; it
never changes a member, because a `memset` reads nothing.

## 1. The defect on the landed compiler (item 1)

**Instrument**: `<copy>/w/scripts/msan.sh`, run as `docker run --rm -v <copy>:/src:ro
heroes-linux-arm64:latest bash /src/w/scripts/msan.sh seed/heroes.c runtime w/p1`. In the
container it builds the compiler from `seed/heroes.c` with `clang -O1`, writes each probe's C with
`hc build <p>.hero --emit-c`, compiles it beside `runtime/runtime.c` with today's
`selfhost/cli/flags.hero` list plus `-D_GNU_SOURCE -g -fsanitize=memory -fsanitize-recover=memory
-fsanitize-memory-track-origins` at five levels, runs it with `MSAN_OPTIONS=halt_on_error=0` under
`timeout 120`, and counts `WARNING: MemorySanitizer`. Debian clang 22.1.8, aarch64. The probe is
215's `pn.hero` and `pn.h`, copied unchanged.

| probe | `-O0` | `-O1` | `-O2` | `-O3` | `-O2 -fno-inline` | printed |
|---|---|---|---|---|---|---|
| `pn.hero`, landed compiler | **1**, `pn.hero:76` | 0 | 0 | 0 | 0 | `40 32 24 16 24 24 2.0 2.0 2.0` |

The report reads *Uninitialized bytes in write at offset 1 inside [..., 16)*: byte 1 of `struct
bf`, from `w_bf` (`pn.h:21`) called at `pn.hero:76`, its origin the `memcpy` of the store at
`pn.hero:75`. The same count, line and levels as 215's route-G column for this probe
(`docs/panel/215-reports/compiler-engineer-build.md`, arm64 table, row `pn`). Output:
`<copy>/w/p1/msan-landed-arm64.txt`.

## 2. The race, per access unit (item 2)

### 2a. Where C's access unit lies, per target

`clang -target <t> -fsyntax-only -Xclang -fdump-record-layouts` and `clang -target <t> -O0|-O2 -S
-emit-llvm` of `void set_c(struct bf *p, unsigned v) { p->c = v; }` (Apple clang 21.0.0 on this
Mac; the box's own clang 23.1.1 for MSVC gives the same dump and `load i32` / `store i32`):

| target | `struct bf`: `b`, unnamed, `c` | `set_c` reads and writes | R1 writes (its gaps) |
|---|---|---|---|
| `x86_64-unknown-linux-gnu`, `aarch64-unknown-linux-gnu`, `arm64-apple-macos14` | `1:0-2`, `1:3-4`, `1:5-8` | `i16` at offset 1: bytes 1, 2 | 3 to 7: **outside** the unit |
| `x86_64-pc-windows-msvc` | `4:0-2`, `4:3-4`, `4:5-8` | `i32` at offset 4: bytes 4 to 7 | 1 to 3 and **6, 7: inside** the unit |

**Measured on the box with the landed compiler**, not only read: `heroes.exe` built there from the
seed (`clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o heroes.exe`,
`--version` `sources 5f45d6514b22d650`, the trunk's key), `<copy>/w/p4/vp.hero` (C returns each
record with every byte first set to `0xff`, a Heroes record's field holds it, C prints what it is
lent) prints `bf 09 00 00 00 5d ff 00 00 00 ...`: R1 wrote bytes 1 to 3 and **6 and 7**, and left 4
and 5 as C made them. The critic's item 9 holds on the MSVC leg.

**And a shape beside it on every target** (CL-061): `struct gap { unsigned x : 1; unsigned : 20;
unsigned y : 1; char z; }` dumps `x` `0:0-0`, the unnamed `0:1-20`, `y` `2:5-5`, so byte 1 is held
by the unnamed bit-field alone and is padding to R1 (`membership.hero:38-39`); `set_y` reads and
writes an **`i24` at offset 0 (bytes 0 to 2)** on the three non-MSVC targets at `-O2`, `i32` on
MSVC. So R1 as landed writes inside C's access unit on all four CI legs, for that shape. Files:
`<copy>/w/p2/au.c`, `gap.c`.

### 2b. What a write inside the unit does to a member, measured

`<copy>/w/p2/race.c`: a C thread writes `b` and `c` of a `struct bf` (and `x`, `y` of a `struct
gap`) under its own lock and re-reads, under the lock, what it wrote last; the main thread, with no
lock (Heroes knows of none), does 50,000,000 times one of: **mode 0**, R1's write, `memset` of each
byte no member holds (struct gap's byte 1, and on MSVC 1 3 5 6 7); **mode 1**, route A as listed,
`byte &= mask` on each byte a named bit-field shares, the masks found by assigning all-ones to each
named bit-field of a zeroed object (route A's own method); **mode 2**, the same with
`__atomic_fetch_and(..., __ATOMIC_RELAXED)`. Counted: member values the writer finds changed by
somebody else.

| leg (command) | mode 0, R1 | mode 1, A plain | mode 2, A atomic |
|---|---|---|---|
| this Mac, arm64, `clang -O2` (`./race <m> 50000000`) | 0 of 551,185 rounds | **430,340** of 3,320,655 | 0 of 35,832,483 |
| Linux arm64, `heroes-linux-arm64:latest`, `clang -O2`, 8 cores | 0 of 256,177 | **1,293,211** of 3,460,528 | 0 of 22,987,212 |
| Windows box, x86-64 MSVC, clang 23.1.1 `-O2`, 2 cores | 0 of 6,508,499 | 0 of 3,570,754 | 0 of 15,246,489 |
| Windows box, the same at **`-O0`** (`heroes build`'s level) | 0 of 49,830,476 | **867,513** of 14,238,480 | 0 of 44,717,176 |

Why x86-64 `-O2` read 0 for mode 1: `clang -O2 -S` shows the plain `&=` became one unlocked
`andb %r13b, (%r12,%r10)`, a read-modify-write inside one instruction whose window the run never
hit; at `-O0` it is a load, an `and` and a store, and the window is hit 867,513 times. The atomic
form is `lock andb` there. The masks route A's own method found agree with the dump's bit column on
all four targets (`1&0xe7 2&0x01` on the three, `4&0xe7 5&0x01` on MSVC, from `1:0-2`, `1:5-8` and
`4:0-2`, `4:5-8`). Mode 0 cannot change a member by construction: a `memset` reads nothing and
writes no member's bit. What it can lose under a concurrent writer is its own zero, which C's
read-modify-write of the unit puts back as it read it: no member's value. That a concurrent whole
store by C undoes R1's zeros for any padding byte today is a reading, not run.

### 2c. A Heroes program reaches it

`<copy>/w/p3/race.hero` over `race.h`: `record Bf tag bf partial` (`a`, `d`); `bf_start(@p: Bf)`
keeps the address (no `lent`: spec § 13, *a parameter, `@` or not, is taken to keep what it is
handed unless declared `lent`*, and *C receives the place itself, which it may keep only while the
place does not move*, which a local lent whole does not, panel 196's R7) and starts a thread
writing `b` and `c` under the library's lock; `bf_peek(@p: Bf)` reads `c` under the same lock and
is called 20,000,000 times. It is race-free C, since every access to `b` and `c` holds the lock. The
landed compiler accepts it (`heroes build race.hero`, exit 0). No rule of the spec refuses it:
`grep -n -i thread spec/heroes-spec.md` matches nothing, and a Heroes program can start a thread of
its own through `extern "hero_os.h"` (`hero_thread_spawn`, which `selfhost/cli/link.hero:290`
binds), so the writer need not be C's own thread.

| compiler (this Mac, `heroes build`, its default level) | printed: started, sum, writer rounds, **member values changed** |
|---|---|
| landed (`3c478500b`) | `0 150007393 3691393838 0`, `0 153241617 4160958469 0` |
| **the route, plain `&=`** (route A as listed: the route's runtime with `hero_pad_clear`'s atomic replaced by `b[...] &= mask`, `w/rt_plain`) | `0 149603035 1393511537 1764`, `0 148638726 2541533697 6252` |
| **the route, atomic `and`** (as built) | `0 149918540 6142748366 0`, `0 150005255 6216622398 0` |
| Windows box, x86-64 MSVC: landed (`heroes.exe` from the seed) | `0 149806760 27742066 0`, `0 149886652 27076775 0` |
| Windows box: **the route, plain `&=`** (`HEROES_RUNTIME=C:/w/223-compiler-engineer/t/w/rt_plain`) | `0 150043171 33193783 3849257`, `0 149942976 33261628 3877881` |
| Windows box: **the route, atomic `and`** (stage 2 built there, `sources 313991fe36d061f4`) | `0 150609132 63977085 0`, `0 150226148 57099227 0` |

So the answer to item 2: a program exists, it is legal Heroes today, and the race is the route's,
not the program's. R1's whole-byte rule avoided it on the three non-MSVC legs for `struct bf`, and
on MSVC and for `struct gap` it writes inside the unit with no harm to a member, because a write
that reads nothing cannot put a stale value back. A route that reads a byte a member holds and
writes it back must do so atomically.

## 3. The route built: A, masks from the dump, cleared atomically (item 3)

**What it does.** For a record whose layout dump names a bit-field, `membership.encoded` writes `n
<byte> <lo> <hi>` (was `n <byte> <bytes>`), the bit range read by a new `layout_rows.bit_bounds`
from the dump's `byte:lo-hi` column, which `layout_rows.last_byte` already parsed for its last
byte. `padding.members` keeps those spans apart from the whole members' (`Held.bitspans`) and
keeps each bit-field's `(at) + byte, lo, hi` (`Held.bits`). The unit's table is written spans first,
bit spans after, so R1's gaps are unchanged; a record with bits gets a table `hero_pad_b_<d>`, room
`hero_pad_m_<d>[2 * Σ(hi/8+1)]`, and one constructor line `hero_pad_mn_<d> = hero_pad_masks(table,
<whole spans>, bits, n, masks, room)`, which ORs each named bit-field's bits into its bytes, **drops
any byte a whole member covers** (an overlapping union arm), and drops a byte all of whose bits are
named. Each lend, and each whole store (2112's site, through `padding.of_place`), writes
`hero_pad_fill` as today and then `hero_pad_clear`, one `__atomic_fetch_and(..., __ATOMIC_RELAXED)`
per shared byte. A unit writing a bit table asserts `_Static_assert(__BYTE_ORDER__ ==
__ORDER_LITTLE_ENDIAN__, ...)` once: the dump's numbering is the memory's on a little-endian target,
all four legs are, and a target that is not fails to build rather than masking the wrong bits
(`.claude/rules/module-shape.md`, a premise with a test that fires when it dies). Emitted for
`struct bf` on this Mac: `hero_pad_b_0[] = {(0) + 1, 0, 2, (0) + 1, 5, 8}`, `hero_pad_m_0[6]`,
`hero_pad_masks(hero_pad_t_0, 2, ...)`, and `hero_pad_clear(&h0_s, sizeof(struct bf), 1,
hero_pad_m_0, hero_pad_mn_0);` after each fill.

**Core or sugar** (design.md Part 5, §1.7): neither: no construct. The checker, the IR and the
lowering are untouched; the change is the layout round's text, the emitter's tables and two runtime
functions, erased below the frontend like R1. No spec token and no diagnostic.

### 3a. Lines, and where they land

`wc -l` and `diff` against the lane at `3c478500b`; code lines by the `layout` suite's own count,
`.claude/hooks/ceiling.py`'s `code_lines` imported (`<copy>/w/scripts/codelines.py`):

| file | `wc -l` | `+` / `-` | code lines (ceiling) |
|---|---|---|---|
| `selfhost/emit/layout_rows.hero` | 239 -> 266 | +27 -0 | 166 -> 182 (300) |
| `selfhost/emit/layout_text.hero` | 376 -> 382 | +7 -1 | 278 -> 283 (300) |
| `selfhost/emit/membership.hero` | 100 -> 130 | +35 -5 | 75 -> 98 (300) |
| `selfhost/emit/padding.hero` | 351 -> 369 | +31 -13 | 284 -> **300 (300)** |
| `selfhost/emit/padding_tables.hero` | 141 -> 180 | +40 -1 | 116 -> 148 (300) |
| `selfhost/emit/guarded_names.hero` | 203 -> 203 | +1 -1 | 193 (300) |
| `runtime/parts/lend.c` | 695 -> 746 | +51 -0 | |
| `runtime/heroes_runtime.h`, `heroes_guard_open.h`, `heroes_guard_close.h` | | +3, +2, +2 | |

`selfhost/emit/inst.hero` (327 of its 350) and `selfhost/cli/layout.hero` do not move. In all,
**+92 code lines in five `selfhost/emit/` modules and +58 runtime lines**: one person's afternoon on
a Pascal-P4 scale, no new pass. `padding.hero` sits **at** its ceiling after `heroes fmt` broke one
line; a landing owes it a split at a seam (the five table-name functions to `membership.hero`), never a
raised ceiling.

### 3b. The defect closed, MemorySanitizer

`<copy>/w/scripts/msan2.sh`, run as `docker run --rm -v <copy>:/src:ro heroes-linux-arm64:latest
bash /src/w/scripts/msan2.sh w/p1 w/p4`: the route's compiler built in the container in two stages
(seed with `clang -O1`, then `selfhost/main.hero`; `--version` `sources 313991fe36d061f4`), each
probe at five levels with the runtime as built (atomic) and with the plain `&=` variant. Output
`<copy>/w/p1/msan-route-arm64.txt`.

| probe | landed | route, atomic | route, plain `&=` |
|---|---|---|---|
| `pn.hero` (215's) | 1/0/0/0/0, `pn.hero:76` | **0/0/0/0/0** | **0/0/0/0/0** |
| `vp.hero` (mine, below) | (no uninitialised byte by design) | 0 at every level | 0 at every level |

**One limit of the instrument, measured**: MemorySanitizer marks every byte an atomic
read-modify-write touches as initialised. `<copy>/w/p2/ms.c` in the container, `clang -O0 -g
-fsanitize=memory`: an uninitialised byte `&= 0xff` (every bit kept), then branched on, reports
**1**; the same through `__atomic_fetch_and(..., 0xff, __ATOMIC_RELAXED)` reports **0**. So the
atomic column alone cannot tell a right mask from a wrong one. The plain column can: there MSan's shadow for `&` is exact, so a padding bit a mask kept
would have been reported, and none was. The value probe is the second judge.

### 3c. The bytes C receives, six records

`<copy>/w/p4/vp.hero` over `vp.h`, this Mac, `heroes build` (landed) and `heroes-next build`
(route); every member checksum is the same under both, `9510 743981 104 5167680 30510 1032`:

| shape | bytes, landed | bytes, route |
|---|---|---|
| `struct bf`, b=5 c=10 (bytes 1, 2) | `5d ff` | `45 01` (exactly b's and c's bits) |
| a union whose `unsigned char y[2]` arm overlaps an arm of bit-fields `x : 3`, `: 5`, `w : 4` | `ab cd` | `ab cd` (a byte a whole member covers is never masked) |
| `struct gap` (unnamed interior byte), bytes 0 to 2 | `ff 00 df` | `01 00 00` |
| `mid : 10` across a byte boundary, signed `s : 3`, `_Bool f : 1`, byte 2 | `da` | `1a` |
| `struct bf` nested in a group record | `5d ff` | `45 01` |
| a fixed array `struct bf e[2]` in a group record, elements 0 and 1 | `5d ff`, `7d fe` | `45 01`, `65 00` |

On the Windows box (MSVC layout), stage 2 of the route built there by `heroes.exe build
selfhost/main.hero -o heroes-next.exe` (exit 0, `sources 313991fe36d061f4`), the same probe prints,
landed then route: `bf` bytes 4, 5 `5d ff` then `45 01`; the union `ab cd` both; `gp` bytes 0 to 3
`ff 00 df 00` then `01 00 00 00`; the `md` record's `_Bool` unit, a unit of its own at byte 8 under
MSVC, `fe` then `00`; the nested and array cases as on the Mac; the same checksums under both.
Output `<copy>/w/p4/win-route.txt`.

### 3d. Run time

`<copy>/w/p5/`: 20,000,000 lends to a C function that reads one member (`touch_*(@p: T lent)`),
`heroes build` (its default level), this Mac, three runs each interleaved, `/usr/bin/time -p`, load
average 1.71; run 1 discarded (`real` above `user`), runs 2 and 3 have `real` equal to `user`:

| record | landed | route | per lend |
|---|---|---|---|
| two members, no bit-field (`struct two { char c; int64_t n; }`) | 0.28, 0.28 s | 0.28, 0.27 s | **0** |
| `struct bf` | 0.26, 0.27 s | 0.38, 0.39 s | **+6 ns** (one call, two `and`s) |

Beside the landed R1's +10 ns of `8e4636161`. **Windows box**, the same probes built there, three
runs each by wall clock (`date +%s%N` around the run; no `user` time on that shell, so a weaker
reading): two members 252, 294, 263 ms landed against 285, 264, 233 ms route (no difference above
the noise); `struct bf` 373, 287, 334 ms against 499, 535, 487 ms, about **+8 ns** a lend for the
two `lock andb`.

### 3e. Suites

On the route's compiler (`heroes-next`, `sources 313991fe36d061f4`), this Mac. To let the hook in
front of the net judge the copy as its own tree (a `git archive` copy has no `.git`, so the hook
resolved it to the trunk and refused, correctly, a compiler built from other sources), `git init`
was run inside `<copy>` alone, no commit.

- **The compiler's own tests**, `./heroes-next test selfhost/main.hero`: **1,720 tests, all
  passed**, exit 0 (`<copy>/w/selftest.txt`).
- **`emission`**, `./heroes-next run tests/harness/main.hero -- ./heroes-next emission`: **689
  passed, 610 failed**, exit 1 (`<copy>/w/emission.txt`). Every program's C carries the key of the
  runtime it links (panel 222's R4), and the route changes `runtime/parts/lend.c`, so every
  emission's `hero_program_sources` line moves; the rule's own exception reads *identical except
  that line* (`.claude/rules/verification.md` § The formatter's probe). Each of the 610 re-emitted
  by `<copy>/w/scripts/reemit.sh` (`heroes-next build <case> --emit-c`, then `cmp` with `grep -v
  'hero_program_sources(void) {'` on both sides; `xargs -P 6`, 36 s): **609 identical but the key
  line, 1 moved**, `tests/emission/run-fixedbugs-156-records-beside-bit-fields-build.c`, the one
  blessed emission of the 89 calling `hero_pad_fill` whose span table holds a bit-field's span
  (`grep -l -E` below, § 5). Its every line: **+8** (the bit table with its one-name macro guard,
  4; the mask room and its count, 2; the `_Static_assert`, 1; the constructor's `hero_pad_masks`, 1)
  and **2 changed** (the store's `hero_pad_clear` after its fill, one `#line`), 0 other. Its `BF`'s
  `flag : 1` and `rest : 31` fill their unit, so the route finds no byte to mask there and clears
  nothing at run time: a table and a call that do nothing, the price of deciding at run time.
- **`determinism`**, the same invocation: **574 passed, 0 failed**, exit 0 (`<copy>/w/determinism.txt`).
- **`emit`**: **1 passed, 12 failed**, exit 1 (`<copy>/w/emit.txt`); the 12 re-emitted and compared
  the same way: **12 identical but the key line**, 0 moved.

## 4. Verdicts

Sections stood on: **design.md §1.12** (*A Heroes program must not segfault and must not corrupt
memory*) for the vetoes; **§1.1** (simplicity sets the ceiling), **§1.7** and **Part 5** (core
plus elaboration) for the cost. design.md does not speak of threads C runs over a place it kept; the
spec's § 13 lets C keep the place (§ 2c), and that is the ground the race stands on.

| route | verdict | why, on what was run | the condition that turns me |
|---|---|---|---|
| **A as listed**: masks found by assigning all-ones, plain `byte &= mask` | **veto** (§1.12) | It reads a byte a named bit-field holds and writes it back. Measured: 430,340, 1,293,211 and 867,513 bit-field values changed in C on this Mac, Linux arm64 and the box at `-O0`; through Heroes, a program the landed compiler accepts with 0, the route plain changed 1,764 and 6,252 here and 3,849,257 and 3,877,881 on the box. The program is race-free C; the race is the route's. | The write made atomic (the next row). |
| **A, masks from the dump's bit column, cleared by an atomic `and`** (built, § 3) | **approve** | 0 values changed in every run on three legs; MSan 0/0/0/0/0 on `pn.hero` (landed 1 at `-O0`); the bytes C receives keep every member and clear exactly the rest in six records on two layouts; +92 code lines in five `emit/` modules, +58 runtime; +6 ns a lend on this Mac and about +8 ns on the box for a bit-field record, 0 for any other; no construct, no spec token, no diagnostic. | A leg where the atomic form changes a member (the race program printing a non-zero last number), or a target whose dump numbers bits otherwise (its build fails on the `_Static_assert`, loudly; then a mask found by C itself, by name, is owed). |
| A, masks by assigning all-ones to each named bit-field, before `main` (the listed method, written atomically) | object, not veto | Its masks equal the dump's on all four targets (§ 2b), so it is the same route at a higher price: each bit-field's name in emitted C, through the macro guard, and an all-ones constant per field type: measured on this Mac, `clang -fsyntax-only -Wall -Werror=sign-conversion` (the build's word) refuses both `p->b = -1` and `p->b = ~p->b` into an `unsigned b : 3`, *implicit conversion changes signedness*, so each field type needs its own spelling of all-ones (`<copy>/w/p2/wb.c`). | A big-endian leg joining the CI. |
| A from a static object holding all-ones | object | Unrun. The same names and constants as above, at file scope; no gain over the dump. | A measurement that the dump's column is unreadable where a static object's is. |
| **B**, rebuilt through a zeroed temporary, every byte written | **veto** (§1.12) | It writes back every member's bytes from values read before, so a C thread's write between the read and the write is lost. On `struct bf` its write of bytes 1 and 2 is mode 1's write; mode 1 measured above. B itself unbuilt: an inference from that measured mechanism, said so. It also writes a lock's bytes, the class R1 kept to whole padding bytes to avoid (215's seat, reasoned there). | B done with atomic writes, which is A with masks of all-ones. Then A is cheaper. |
| B over the bit-field run alone | **veto** (§1.12) | The same write-back over the run's named bit-fields, inside C's unit by definition; unbuilt, same inference. | As B. |
| **C**, left open and said | object | 2099 is reproduced (§ 1). No rule lets it stop counting (the critic's item 12); a route exists at the cost in § 3. *Left open* holds the tag for no saving. | A rule making an open `adjacent` defect not count, which no seat may write here. |
| **D**, refused at build time | object; outside this lane | A diagnostic class; spec line 383 tells the reader to bind such a record `partial`, so D refuses a program the spec tells them to write. Not needed: the atomic route keeps every member at the measured price. | Only if the atomic route failed on a leg. |
| At the source: zeroing where a value enters a Heroes place from C | object | It needs no atomic (a fresh temporary). But C's own whole store through a kept address and a Heroes-to-Heroes return by value (046 shape 5's mechanism on x86-64) bring dirty bits too; so every source site, more sites than the lend and the store. A reading, unrun. | A census showing those two sources cannot carry a bit-field record. |
| Per site: one rule at the lend and at 2112's store | approve one mechanism at both | The store writes every byte non-atomically already, so the atomic `and` adds no race there and costs one call; one mechanism keeps one sentence of the rule. As built, both sites reach `padding.of_place`. | None. |

**My conditions on the route I approve**, each for the landing, not this sitting:

1. `selfhost/emit/padding.hero` split at a seam to below 300 code lines (it sits at 300 as
   built). No ceiling raised, no `DECIDED` row.
2. The value probe (`vp.hero`, six records) lands as a `run/` case whose `.expected` holds the
   bytes, the box's MSVC layout as its second blessing where it differs. The race program stays a
   measurement and not a golden: it is a lower bound, never a proof.
3. Every line the re-blessed emissions move is classified by script, as `8e4636161` did, and 0
   lines fall outside: bit tables, mask rooms and counts, `hero_pad_masks` lines, `hero_pad_clear`
   calls, one `_Static_assert` per unit, re-ordered span tables, and `#line`s.
4. `.claude/rules/generated-c.md` § Arithmetic and memory reads, after the route (it already
   lags 2112, the critic's item 4):

> **And C never reads a group record's padding as it was left** (panel 215's R1, ratified
> 2026-10-16; panel 223): before each `extern` call, and after every store of a group record whole
> (defect 2112), the bytes no C member holds in every group record the call lends through `@`, a
> whole cell, a field, an element, an `@` parameter handed on and the elements of an array's
> buffer, are written zero in place, never a byte a member holds and never inside a union; and in
> a byte a named bit-field shares, the bits no named bit-field holds are cleared by an atomic
> `and`, so no bit a named bit-field holds is ever written, even while a thread C runs writes it
> (`emit/padding.hero`). Every span is clang's `offsetof` and `sizeof` of a member, and every
> bit-field's bits its layout dump's, in a table each unit writes once behind an assertion of a
> little-endian target, a member's own name guarded from the header's macros; the gaps and the
> masks are found once, before `main`, and a lend only writes them. [measured-before sentence as
> today] What it does not promise: a zero C itself writes over while it runs beside the lend, and
> five shapes read and not yet run (issue 2100).

**The strongest objection to my own approval**, said so the synthesis can weigh it: an atomic
read-modify-write beside C's plain write of the same bit-field is still a data race by C11's own
definition (two conflicting accesses, one not atomic). It changes no member on either ISA the CI
runs, measured above; the instruction is `lock andb` on the box (`clang -O2 -S`), `ldclrb` on this
Mac and a call to `__aarch64_ldclr1_relax` on Linux arm64 (clang's outline atomics, LSE or an
exclusive loop chosen at run time) (`objdump -d` of `runtime.c` built at `-O0` on each), but it rests on the
hardware and not on the language standard; no route that writes inside a bit-field's byte can rest
on the standard. And MemorySanitizer cannot judge the atomic form's masks alone (§ 3b).

## 5. Prediction

**When panel 223's route lands, the re-blessed `emission` suite moves exactly the blessed emissions
whose span tables today hold a bit-field's span (a `(…) + <byte>, <bytes>` pair inside a
`hero_pad_t_` table), no other emission, and in each moved file 0 lines outside condition 3's
classes.** Instrument: `heroes run tests/harness/main.hero -- <compiler> emission` before the
re-blessing, its failed labels against `grep -l -E 'hero_pad_t_[0-9]+\[\] = \{[^}]*\) \+ [0-9]+, [0-9]+'
tests/emission/`, and the landing's classifying script. Scored at the landing commit of panel
223's route. **The number: 1 of 1,299 (`run-fixedbugs-156-records-beside-bit-fields-build.c`)**,
already true of the prototype (§ 3e), so the prediction is that the landing, split and all, keeps
it: a landing that moves a second emission beyond its key line, or any line outside condition 3's
classes, falsifies it. A second, cheaper to score: no module of `selfhost/emit/` over its ceiling
at the landing, by the `layout` suite, with no `DECIDED` row added.

## 6. What stayed unrun

- The suites the map names for `selfhost/emit/**` beyond the compiler's own tests, `emission`,
  `determinism` and `emit`: `wholes` and `descriptors` (they read `seed/heroes.c`, which was not
  regenerated), `lldb`, and `runtime` for `runtime/**`; the `layout` suite (the hook's mirror was
  used); the full net.
- MemorySanitizer on Linux x86-64; on the box there is none (046 § 5).
- The race program at `-O2` on the route, and on Linux arm64 through Heroes (the C stress ran there).
- A seed regenerated from the route, and its fixpoint.
- Linux x86-64 for the race and the value probe (the box is the x86-64 leg that ran them).
- The suppression of the table and the call for a record whose bit-fields fill every byte they
  touch (`fixedbugs-156`'s `BF`): decided at run time as built, so a call of zero masks stands.

## 7. The route as the coordinator adopted it, built and run (appended 2026-10-17, 19:15)

The coordinator adopted route A with the atomic `and`, provisionally, with three changes from the
critic's second pass (`docs/panel/223-reports/critic.md` § 3): (1) the masks come from C's own
bits, `o.F--` on a zeroed object for each named bit-field, in the constructor before `main`, its
names guarded as a member's are, the runtime reading the object's bytes; the dump's bit column and
the little-endian `_Static_assert` go; (2) the clear goes through `volatile unsigned char *` with
`__atomic_fetch_and`; (3) a byte a `const` bit-field holds is left alone. **§ 3's mechanism (masks
from the dump's `byte:lo-hi`) and § 5's classes *bit tables* and *one `_Static_assert` per unit*
are superseded by this section**; what they measured stands at its time. The dump route's files are
kept at `<copy>/w/built-dump-route/`.

### 7a. What was built

- `selfhost/emit/membership.hero`: a named bit-field's entry is `n <byte> <bytes> <path>`, its C
  name added; one whose dump type opens `const ` is `c <byte> <bytes>`, held whole (the dump prints
  the qualifier first: `0:0-2 | const unsigned int k`, measured on this Mac); `bit_entry` reads
  both. **The split's seam**: `guarded`, the guard that spells a dump's names, moved here from
  `padding.hero` with its test, since this module already reads those names (`names_in`).
- `selfhost/emit/padding.hero`: `Held.bits` holds each named bit-field's C name on the outer type;
  a `c` entry's bytes join the whole spans, so no mask reaches them and R1's gaps are unchanged.
- `selfhost/emit/padding_tables.hero`: per record with bits, at file scope a mask room
  `hero_pad_m_<d>[2 * Σ bytes]` and a count; in `hero_pad_init`, a block `{ T hero_pad_o;
  __builtin_memset(&hero_pad_o, 0, sizeof hero_pad_o); hero_pad_o.b--; hero_pad_o.c--;
  hero_pad_mn_<d> = hero_pad_masks(<table>, <whole spans>, &hero_pad_o, sizeof hero_pad_o,
  hero_pad_m_<d>, <room>); }`, the `--` line between `push_macro`/`#undef`/`pop_macro` of each name
  its paths spell. No bit table, no `_Static_assert`.
- `runtime/parts/lend.c`: `hero_pad_masks` reads the object's bytes, skipping a byte no named
  bit-field touches, one all of whose bits are named, and one a whole span covers;
  `hero_pad_clear` writes through `volatile unsigned char *` with `__atomic_fetch_and(...,
  __ATOMIC_RELAXED)`. `runtime/heroes_runtime.h` declares both, the object as `const void *`;
  `heroes_guard_open.h`, `heroes_guard_close.h` and `selfhost/emit/guarded_names.hero` list the two
  names.
- **Found by the compiler's own tests on the way**: declaring the object `const unsigned char *`
  in `heroes_runtime.h` failed *the list holds every word the runtime's headers write*
  (`missing: unsigned`), 1,719 of 1,720; declared `const void *`, 1,720 of 1,720.
- `layout_rows.hero` and `layout_text.hero` are back to `3c478500b` (the dump column is not read).

| file | `+` / `-` against `3c478500b` | code lines (ceiling), `layout`'s count by the hook's mirror |
|---|---|---|
| `selfhost/emit/padding.hero` | +38 -52 | 284 -> **283** (300) |
| `selfhost/emit/padding_tables.hero` | +49 -2 | 116 -> 153 (300) |
| `selfhost/emit/membership.hero` | +139 -6 (of which the guard and its test moved in, and a 52-line test `heroes fmt` broke per field) | 75 -> 129 (300) |
| `selfhost/emit/guarded_names.hero` | +1 -1 | 193 (300) |
| `runtime/parts/lend.c`, `heroes_runtime.h`, `heroes_guard_open.h`, `heroes_guard_close.h` | +41, +3, +2, +2 | |

**+90 code lines net** over three `selfhost/emit/` modules (the guard's move is net zero) and
**+48 runtime lines**; no module over its ceiling, no `DECIDED` row; `layout` 6 passed, 0 failed.

### 7b. The `run/` case

`tests/golden/run/ffi-the-bits-beside-a-bit-field-are-zero.{hero,h,expected}`: six records C
returns with every bit set, held in a field of the program's own record and lent; C compares what
it receives with the same record built from zero bytes and the same members, and prints 0 per
record where every byte agrees, so the line is the same on every layout. Its header comment says
what it measures and what the compiler before printed, as the run form's cases do (no `#~`: it
provokes no diagnostic).

| compiler | this Mac | the box (MSVC layout) |
|---|---|---|
| landed (`3c478500b`) | `2 0 1 3 10 2` | `5 0 1 6 13 5` |
| the route | **`0 0 0 0 0 0`** | **`0 0 0 0 0 0`** |

**The `const` shape could not go in, and why**: a record holding a `const` member, bit-field or
not, is refused by the **landed** compiler before any route. `<copy>/w/p6/cq.hero` (`struct cq {
const unsigned k : 3; unsigned j : 2; int64_t n; }`, a field of the program's record) and
`cq2.hero` (a lent local built `rest: zero`) both stop at `internal error: compiling the generated
C failed`, clang's *cannot assign to variable 't1' with const-qualified data member 'k'*, plus
`-Wdefault-const-init-field-unsafe` warnings: exit 2 on a correct binding. **A defect found beside
the work, not filed here**; until it is repaired no program reaches change (3), so (3) is held by
the compiler's own test `a named bit-field gives its bytes and its C name, a const one its bytes
alone` (`encoded` over a `const unsigned int` member gives `c 0 1`), not by a run.

### 7c. Measured on the final sources

The route's compiler `heroes-next`, `--version` `sources 661c18886e93f5b8`, built on this Mac by
`./heroes build selfhost/main.hero -o heroes-next` (exit 0); the same key built in the container and
on the box from the same files.

| instrument (command) | result |
|---|---|
| the compiler's own tests, `./heroes-next test selfhost/main.hero` | **1,720 tests, all passed** |
| `run`, filtered: `... -- ./heroes-next run ffi-the-bits-beside` and `... run fixedbugs-156` | **1 passed, 0 failed**; **1 passed, 0 failed** |
| `layout` | **6 passed, 0 failed** |
| `wholes`, `descriptors` (on `64610a1f57f15711`, the build before the `const void *` repair, which moved only the header's parameter type and one cast in the constructor; neither suite reads either) | **630 / 0**, **630 / 0** |
| `determinism` | **575 passed, 0 failed** |
| `emission` | 690 passed, 611 failed: 1 is the new case, *nothing blessed* (the patch carries no `tests/emission/`, as asked); the 610 re-emitted (`<copy>/w/scripts/reemit.sh`, `xargs -P 6`) are **609 identical but the key line** (the runtime moved, panel 222's R4) and **1 moved**, `run-fixedbugs-156-records-beside-bit-fields-build.c` |
| that file's every changed line, `python3 -I <copy>/w/scripts/classify.py <blessed> <new>` (`difflib` over both sides without the key line, each line matched to one class) | **+16 -2, 0 other**: `+6` macro guard of a bit-field's name, `+2` block braces, `+1` scratch object, `+1` scratch zeroed, `+1` decrement line, `+1` masks found, `+2` mask room and count, `+1` the store with its `hero_pad_clear`, `-1` the store before it, `+1 -1` `#line` |
| MemorySanitizer, `docker run ... heroes-linux-arm64:latest bash /src/w/scripts/msan2.sh w/p1` (two stages in the container, `sources 661c18886e93f5b8`) | `pn.hero` **0/0/0/0/0** with the runtime as built and **0/0/0/0/0** with the plain `&=` variant; landed 1/0/0/0/0 (§ 1). Output `<copy>/w/p1/msan-final-arm64.txt` |
| the lost-update race through Heroes, `race.hero`, this Mac, two runs each | atomic through `volatile`: **0**, **0**; plain: **40,814**, **53,734** |
| the same on the box, stage 2 built there (`sources 661c18886e93f5b8`) | atomic: **0**, **0**; plain: **1,304,216**, **1,562,472** |
| the value probe `vp.hero`, Mac and box (on `64610a1f57f15711`) | bytes identical to the dump route's on both layouts (§ 3c), every checksum `9510 743981 104 5167680 30510 1032` |

### 7d. The patch

`.claude/worktrees/223-compiler-engineer/route.patch`: `diff -ruN` of `selfhost/`, `runtime/` and
`tests/golden/` between a `git archive 3c478500b` and the copy, `-p1`; eleven files, +344 -55
lines; no `tests/emission/`, no seed. `sha256 0dc4a371d79faffccc2c0763931c3073e0587966d813ce20e0a17eb4d60a7aea`.
`git apply --check -p1` on a fresh `git archive 3c478500b`: applies cleanly. Applied there, stage 1
from the seed and stage 2 (`./heroes build selfhost/main.hero -o heroes-next`, exit 0) report
**`sources 661c18886e93f5b8`**, the key every measurement of 7c names: the patch is what was run.

### 7e. What the verdict and the conditions become

My verdict on the adopted route is **approve**, unchanged; the change to C's own bits removes the
one premise my route carried (the dump's numbering) and the `_Static_assert` with it, which is
better than what I built. Conditions 1 to 3 of § 4 are met in the patch (283 of 300; the case,
layout-independent, so no second blessing; every moved line classified, 0 other). Condition 4's
sentence reads, with the three changes: *... and in a byte a named bit-field shares, the bits no
named bit-field holds are cleared by an atomic `and`, so no bit a named bit-field holds is ever
written, even while a thread C runs writes it, and a byte a `const` bit-field holds is never
written (`emit/padding.hero`). Every span is clang's `offsetof` and `sizeof` of a member, and every
bit-field's bits C's own, `--` on each of a zeroed object, in a table each unit writes once, a
member's own name guarded from the header's macros; the gaps and the masks are found once, before
`main`, and a lend only writes them.* **§ 5's prediction, restated for this route**: at the landing
commit, `emission` moves exactly 1 blessed emission beyond its key line (`fixedbugs-156`), and the
classifying script reads 0 lines outside 7c's classes.

**Unrun**: `emit`, `lldb`, `runtime` and the full net on the final sources (`emit` read 12 failed,
all key-line only, on the dump route, § 3e); a seed regenerated and its fixpoint; Linux x86-64;
the per-lend time of the final route (the clear's body moved to `volatile`; not re-taken, the
Mac's load average reading 8.50 at 19:17, other sessions' work, which no clock may run beside);
the `const` exception through a program (unreachable, 7b).
