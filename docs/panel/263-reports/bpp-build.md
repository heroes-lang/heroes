# Panel 263: route (b'') built and measured, lane bpp's report (2026-11-27, about 22:10)

Written by the coordinator from lane bpp's final message, word for word in substance, because the lane's own `report.md` could not be written from its session. The lane built (b'') on branch `lane-bpp` (commits `c1abdf30a` 217a, `f8a97d996` 217b, `02e5de670` 217c, `f7065a6b0` 217d, `35d801625` 217e, `c1b5d733d` 217f, `2dd6d086f` 217g; none merged) by the author's answer *1a* of 2026-11-27 about 18:35, after the brief `.claude/worktrees/rebel-scratch/coord/bpp-brief.md`. Its run files are under `.claude/worktrees/lane-bpp-scratch/out/` (370 MB, not committed).

## Two choices the sitting left open, the lane's

A runtime stack is sized like the C stack the callback came from: the thread's own size, or a coroutine's range, at least 64 KiB. A guarded reserve on C's known stack range: 1 KiB, 4 KiB under the sanitizer. Every Heroes frame runs on a runtime stack with a guard below and above, and every bound C call runs below the thunk on C's stack. Nested callbacks continue on the caller's runtime stack only when that stack has at least C's room left. A callback counts as nested only if the frame records above it reach an open door's switch record and that door holds no other callback; addresses are never compared by order.

## The vetoes

- **ffi-pragmatist, `csh.hero`: lifted.** It prints `both: 10022002`, 3 of 3, at -O0 and -O2 on both platforms; under `--sanitize` every build ends as the base's does. The alternation half of its condition: it never writes below C's stack; at -O2 it runs right to 3,262 levels on a 256 KiB malloc stack, against the base's 4,093 (Mac) and 4,094 (Linux), and past that it is refused with 0 bytes, each level taking 80 bytes of C's stack against the base's 64 (within the synthesis's "twice 64", but refusing correct programs in that band); under `--sanitize -O2` it runs to 2,014 (Mac) and 2,302 (Linux) against 3,270 and 3,272; at -O0 it goes deeper than the base, 1,165 against 908 and 1,254 against 961.
- **spec-warden, § 9: stands for C's frames only.** Every Heroes frame is guarded (0 bytes in every count). Shape D writes the base's bytes exactly: 81,424 at exit 0 at -O0 n 4,000 on this Mac, 41,957 on Linux. § 9 stays false for C's own frames unless a sentence says they are C's.
- **compiler-engineer, the alternation class: lifted for bytes** (0 where (b') wrote 258,243 at exit 0 at -O2 n 2,000); the refusal band above is what remains. Shape D is back to (c2)'s numbers, which is the run its approval of (b) turned on.

## Bytes written below the stack (counting canary; (b'') against the base)

| shape | this Mac | Linux arm64 |
|---|---|---|
| canary, malloc and static array, all four builds | 0 every run, a told abort; the base wrote up to 262 KB at exit 0, or hung, or gave the false thread message | the same; the base wrote up to 219 KB at exit 0, or exited 135 |
| shape D | the base's bytes, above | the same; at n 6,000 a false "null pointer" message until 217g put a guard above each runtime stack; now told after 68,242, as the base |
| alternation at n 1,000, 2,000, 3,000, 100,000 | 0 at every build | 0, except the reserve's own stop at `--sanitize -O0` writes 368 bytes (writer not identified) |
| reproducers: malloc, mmap with and without a guard, static array | told at -O0 and -O2; ASan's stack-overflow under `--sanitize`; the base's malloc exited 138 or hung, its static array hung | told at -O0 and -O2, as the base, except the static array, which the base let die silently (139, 135) |
| `helper.hero` | 0, told; the base wrote 57,921 at exit 0 | 0, told; the base wrote 43,441 at exit 0 |

Shape D at -O2 n 100,000 on this Mac also ends in a false message about the lend record after 139,200 bytes (the base hangs): C's recursion overwrites the runtime's thread-local words.

## The signal guard's cases

The four families hold 187 distinct cases, not 175, at the run form's four builds. On each platform 183 pass and 4 fail, all four the same way: the room's abort counts a different number of callbacks not returned (2,031 where 2,047 is expected), because held runtime stacks now count against the 1 GiB room. Two sanitizer failures (2704, 2756) were repaired in 217e. The Mac run is at 217f; the Linux run used the 217f compiler with the 217g runtime change copied in mid-run.

## The conditions

- Spawn doors do not switch: built. `spawn.hero` behaves as the base at both levels; the compiler built with (b'')'s thunks checks a 13,000-term sum and refuses 14,000, as the base.
- ASan fibre annotations: built, with jump unpoisoning added in 217e.
- CFI on both switches: built. The DWARF unwinder and the frame records cross both switches and reach `main`, on both platforms.
- Panel 258's R2 stopping by frame identity: not built (step 214d not on `main` when the lane ran); (b'')'s own release on a jump finds frames by identity.
- A bound on held stacks: built. The 7,282nd suspended callback is refused on this Mac (7,944th on Linux); the base ran 9,000; at 8 MiB per stack the refusal came at the 127th.
- What a reader of § 13 needs: not written or counted. w1's text is owed, and so is `frame.c`'s abort wording; the reserve's refusal is a new told end.

## The costs (this Mac, instructions retired, best of five, measured at 217d; Linux unmeasured, the image has no instruction counter)

| | -O2 | -O0 |
|---|---|---|
| per callback | +310 (+184.5%) | +598 |
| per callback from a coroutine stack | +316 | not read out |
| per C call inside a callback | +109 (+82.6%) | +217 |
| per C call outside every callback | +8.7 (+6.6%) | +28.9 |
| Heroes calls, panel 104's recursion | 0.0% | 0.0% |

Not optimised; above both earlier prototypes ((b) +284, (b') +133). The compiler's own tests passed, 2,369 of 2,369, at 217e and 217f; the seed is not regenerated.

## The lane's own slips

It used `pkill -P <pid>` once, on the parent id of its own harness process, to stop that process's child; the brief bars `pkill`.
