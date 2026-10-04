---
kind: defect
area: golden
milestone: none
filed: 2026-09-27
commit: 23bf73edbb1e1d86bf94f228ed0d1b96cbc470a9
github: none
---

# Defect 115 closed: on Windows the runtime says where in the dead region C read, as it does on Linux and the Mac

2026-09-27, M-agreed-retention step 22, in lane A (`7bcd7cc2`), merged `f08b192d`.
Found by the coordinator running the `run` suite on the Windows box for lane
105's merge; repaired and measured on the box by lane A's agent.

- [x] **115 — on Windows, the runtime's report of a dead C handle read inside C omits where in the page it was read** | `tests/golden/run/dead-handle-read-by-c-through-a-cell-it-should-only-write.hero` must abort saying `a dead C handle was used inside C, at offset 0x0`, and on the Windows box it says `a dead C handle was used inside C` with no offset, so the `run` suite reads 199 passed and 1 failed there; Linux x86-64, Linux arm64 and the Mac print the offset | `runtime/` (the Windows path of the poisoned page's fault handler) · the golden above · **closed 2026-09-27**

    **Origin:** the coordinator, 2026-09-27, running the `run` suite on the
    Windows box for lane 105's merge (the trunk at `09acdabc`), and the same
    golden run there on the trunk at `2b1a1f24`, before that merge: the same
    text, so it is older than the lane. Panel 177, which landed the poisoned
    page, records Windows as unrun for every row; the formatter lanes ran
    only their own suites on the box.

    **Why it is a defect.** A sentence the runtime owes is missing on one
    platform, and the net is red there; the Windows exception record carries
    the address a fault touched, so the offset can be said on every platform.

## What it was

`runtime/parts/stack.c`'s vectored exception handler found the dead region
from `ExceptionInformation[1]`, computed the offset into it, and then wrote a
fixed line with no offset. The comment above the arm gave the null arm's
reason for having no FRAME (no frame walk on Windows) as its reason for
having no OFFSET, and marked the arm unrun. The digits the null arm writes
were a hand copy of the POSIX `hero_stack_say_hex`, so the offset was one
copy away and not taken.

## The repair

- **One hex writer above the platform split**, `hero_stack_hex`, which
  formats an address into a caller's buffer with no call at all. The POSIX
  `hero_stack_say_hex` and the Windows `hero_stack_veh_say_hex` both use it,
  and the Windows null arm's hand copy of the digits is gone: three callers
  on two platforms, one loop.
- **The Windows dead-handle arm says the POSIX sentence**, `panic: a dead C
  handle was used inside C, at offset 0x…`, then the same tail. No `called
  from`, for the frame walk's reason at the head of the file, which the
  POSIX sentence makes optional.
- **Every Windows arm writes through `hero_stack_veh_say`**, which loops
  `_write` until every byte is out, as `hero_stack_say` loops `write(2)` on
  POSIX: a short write used to cut the sentence silently.
- `HERO_RUNTIME_ABI` does not move: no declaration in `heroes_runtime.h`
  changed.

## The shapes beside it, and three goldens that pin them on every platform

The golden only ever asked for offset 0, a read. Three `run` goldens beside
it ask for what the arithmetic must get right on all three platforms:

| golden | C does | must say |
|---|---|---|
| `dead-handle-read-by-c-at-a-field-past-the-first` | reads a field 72 bytes in | `at offset 0x48` |
| `dead-handle-written-by-c-through-a-cell-it-should-only-write` | WRITES a field 8 bytes in, reading nothing first | `at offset 0x8` |
| `dead-handle-read-by-c-at-the-last-word-of-the-dead-region` | reads the last word of a 64 KiB object | `at offset 0xfff8` |

| golden | offset | Mac | Linux arm64 | Linux x86-64 | Windows |
|---|---|---|---|---|---|
| `dead-handle-read-by-c-through-a-cell-it-should-only-write` | 0x0 | said | said | said | said, where the trunk left it out |
| `dead-handle-read-by-c-at-a-field-past-the-first` | 0x48 | said | said | said | said |
| `dead-handle-written-by-c-through-a-cell-it-should-only-write` | 0x8 | said | said | said | said |
| `dead-handle-read-by-c-at-the-last-word-of-the-dead-region` | 0xfff8 | said | said | said | said |

The Mac's column read by the coordinator on the trunk at `f08b192d` (`heroes
run` on each golden, the offset in its output); the others by the `run`
suite on the lane's tree on each platform, which compares each golden's
whole output, the offset included: 206, 206 and 204 passed, 0 failed.

## What it does not change

A read past the dead region's 64 KiB is still not named as a dead handle,
on any platform: the region is `HERO_DEAD_SPAN` bytes and the handlers ask
that range. Windows still prints no `called from`, since it has no frame
walk.

## The measurements

Three `run` goldens with their blessed C pin the offset on every
platform: `dead-handle-read-by-c-at-a-field-past-the-first` (0x48),
`dead-handle-written-by-c-through-a-cell-it-should-only-write` (0x8) and
`dead-handle-read-by-c-at-the-last-word-of-the-dead-region` (0xfff8).
The Windows box, the lane's final tree built there: the compiler's 778
tests and `run` 204 passed and 0 failed, where the trunk read 199 and 1.
The lane's gate and platforms are its commit's body (`7bcd7cc2`): the
compiler's 778 tests, the net's own 172, every suite 0 failed, Linux arm64
and x86-64 and the Windows box at run 206, 206 and 204 with 0 failed. On
the trunk after the merge (`f08b192d`): the seed emitted again by the
trunk's compiler, fixpoint by `cmp`; the compiler's 782 tests, the net's
own 172; the full net 2741 passed and 1 failed, the annotations floor the
two lanes moved together, raised in the merge, annotations 199 and 0.
