# 046: the padding C reads, under MemorySanitizer: five shapes, two reach it, and one is the x86-64 ABI

Measured 2026-10-17 by the session that landed panel 215, for task 2100
(https://github.com/heroes-lang/heroes/issues/2100): the five shapes panel
215's R1 left open as readings (`docs/panel/215-reports/compiler-engineer-build.md`
§ What stays open, items 1, 3, 4, 5 and 7), each run under MemorySanitizer.
Every number is a command's, run that night; what was not run is said in § 5.

## 1. The instrument, and that it fires

The trunk's compiler at `bf6a9c4eb`, built from its seed in the container,
writes each probe's C with `heroes build <p>.hero --emit-c`; clang builds it
beside `runtime/runtime.c` with `-std=gnu11 -fsanitize=memory
-fsanitize-memory-track-origins -fno-omit-frame-pointer` at `-O0` and at
`-O2`, and the binary runs. Linux arm64 is `heroes-linux-arm64:latest`, Linux
x86-64 `heroes-linux:latest`, both Debian clang 22.1.8. A probe's header hands
C a function that branches on every byte of the record it receives
(`all_bytes`), or prints the offsets MemorySanitizer holds unwritten without
stopping (`unwritten`, `__msan_test_shadow` over each byte).

- **The runtime is clean under it**: a program pushing to an array and
  joining two strings ran at exit 0 with no report, on arm64.
- **The instrument fires**: a C program that builds a `Gap` (`struct { uint8_t
  c; int64_t n; }`) by its two members and hands it to `all_bytes` reports
  `use-of-uninitialized-value` at the branch, on both legs.
- **The control**: a `Gap` lent through `@` to `peek_gap`, R1's own route, is
  clean on both legs at both levels.

## 2. The five shapes

| shape (item) | probe | arm64 -O0 / -O2 | x86-64 -O0 / -O2 | reading |
|---|---|---|---|---|
| a C array of structs no binding declares (1) | `record Outer partial` naming `id` over `struct { int32_t id; Cd a[2]; }`, built `Outer(id:, rest: zero)`, lent | clean / clean | clean / clean | every byte read written, the record built with `rest: zero` (which writes what it is the measurement does not say) |
| bytes past a union's widest arm (3) | `record U5` over `union { uint8_t c[5]; int32_t i; }`, built naming `c`, lent | clean / clean | clean / clean | every byte read written, bytes 5 to 7 included |
| a record handed to C by value (4) | `peek_gap_value(g: Gap)` | clean / clean | **reported** / clean | **the x86-64 ABI**: below |
| bytes dirtied after the zeroing, read through a kept address (5) | `keep_gap(@g)`, then `g @ made(n: 3)`, then `peek_kept()` | clean / clean | **reported** / clean | **a defect**, filed as 2112 |
| a record whose layout dump the reader cannot read (7) | five layouts, below | as below | as below | not reached |

**Shape 4 is no caller's to repair.** The report's origin is the callee's own
parameter, *created by an allocation of 'g' in the stack frame of
'peek_gap_value'*. A C program alone, `memset(&g, 0, sizeof g)` before the
members are set, then `peek_gap_value(g)`, reports the same on x86-64 at
`-O0` (two reports) and nothing on arm64: the x86-64 ABI passes
`{uint8_t, int64_t}` in two registers that hold no padding, so the callee's
copy has none to read whatever the caller wrote.

**Shape 5 reaches C.** The report's origin is the `memcpy` of the store
`g @ made(n: 3)` (`kept.hero:14`): `made` returned the record by value, which
on x86-64 carries no padding either, the store copied seven bytes nothing
wrote into the place C kept, and R1 writes the padding zero only before an
`extern` call that lends the place, which `peek_kept()` does not. Filed as
defect 2112 (`blocking`).

**Shape 7, five layouts tried, none unread.** Each record came from C by
value, so its padding was dirty at `-O0`, and was then lent through `@`:
a bit-field spanning a byte boundary before a `uint8_t` and an `int64_t`;
`__attribute__((packed, aligned(8)))`; an `int : 0` between two bytes;
`_Alignas(16)` on an `int64_t`; an anonymous struct member. In each, every
padding byte read written, so the reader read each dump and R1 zeroed it. The
one byte left unwritten anywhere was byte 1 of the bit-field record, the three
bits past `mid : 10` inside it: defect 2099 (`adjacent`), already filed.

## 3. Found beside shape 5: C may hold a local's address past its function

`keep_gap(@g: Gap)` was accepted for a local cell of `main`, and so it was for
a local of a function that returns: `stash()` hands C `@g` and returns, and a
later `peek_kept()` reads `g`. On this Mac the plain build printed `2` at
exit 0, bytes of a dead frame; under `--sanitize` with
`ASAN_OPTIONS=detect_stack_use_after_return=1` the program died, exit 134,
*AddressSanitizer: BUS on unknown address*. Panel 196's R1, ratified
2026-10-06, rules it: C receives *the place itself, which C may keep only while
the place does not move; a copy, a return and an array's growth move it, the
program's obligation*. So it is not filed as a defect; it is put to the author
as a question, since a measured class of memory faults at exit 0 is what
CLAUDE.md § Precedence, as the author read it on 2026-10-16, closes.

## 4. What it means for a program

On Linux arm64 and at `-O2` everywhere, every shape C can be handed reads
written bytes. On Linux x86-64 at `-O0`, the level `heroes build` compiles at,
a record that came back from a function by value and was stored into a place
C kept is read with seven bytes nothing wrote; the same record handed to C by
value is C's own ABI. The probes are in the coordinator's scratchpad of that
night, not kept: each is reproduced from the shapes above in a few lines.

## 5. Not run

macOS, which has no MemorySanitizer; Windows, whose clang has none; the
threads of item 6, which the task did not ask; a record whose layout dump the
reader cannot read, which none of five tries produced, so item 7 stands as a
reading for shapes not tried.
