# The answers, never given to a reader

**Today** is the runtime of the base `3ec775b66` (`00-shared.md` § 2), run by the repair of the briefs on the two graded
platforms, this Mac (Darwin arm64) and the Linux arm64 image: on each, every program built by the repair's stage 2 built
there (`heroes-next build qN.hero -o bin-qN`, in a copy of `programs/` under
`.claude/worktrees/panel259-repair-scratch/{mac,linux-arm}/blind/`) and run three times as `timeout 120 ./bin-qN > out
2>&1`, the exit `$?` (this Mac 06:47 to 06:49 UTC; the arm64 image 07:09 to 07:12 UTC, in the container
`p259-repair-1`); the three outputs of each program are equal by `cmp` on each platform, and one of each is in
`../runs/<platform>/out-qN.txt`. **The Linux x86-64 image (emulated) is the critic's, at `2cf85ea64`** (F25: every
output equal by `cmp` to its arm64 output, three of three); its re-run at this base is the ffi-pragmatist's. The clerk's
Mac runs (06:20 to 06:23 local) and the critic's on all three (F13, F14, F25), at `2cf85ea64`, read the same as this
base's, row for row.

**The grading** is `../llm-ergonomist.md` § The key: the landed build's own run, three of three, **on this Mac and in
the Linux arm64 image**, the two platforms `task-t1.md` names; x86-64 is recorded here and never graded. **The verdict
of a run** is read by one rule: `WRONG` where any printed line is not what a correct reading of the program's own code
prints, whatever follows; else `ABORTS` where it ends before its end saying why; else `PRINTS` with its lines. A run
ending by a signal with nothing said is none of the four. **A program whose two graded platforms give different
verdicts has no key under that arm** and is named, out of every rule that reads it. The adopted resolution decides what
q4 and t2's design A do, so their key under `adopted` is **UNRUN** until that build exists.

| program | what C does | today, this Mac, 3 of 3 | today, Linux arm64, 3 of 3 (x86-64 at `2cf85ea64`: the same bytes, F25) | key under `now` | spec claim it tests |
|---|---|---|---|---|---|
| q1 | `makecontext` coroutines on `malloc`'s stacks, both resume orders | `order 0 1: 30 32`, `order 1 0: 30 32`, exit 0 | the same lines, exit 0 | `PRINTS` those lines | the control |
| q2 | the same, stacks carved from `co_host`'s array on the thread's own stack | `body holds 7` twice, `body holds 7 coroutines 30 32`, `carved: 7`, exit 0 | the same lines, exit 0 | `PRINTS` those lines | a switch the runtime sees, on a carved stack |
| q3 | `sw.h`'s own switch on `malloc`'s stacks | as q1, exit 0 | the same lines, exit 0 | `PRINTS` as q1 | an unseen switch off the thread's stack |
| q4 | `sw.h`'s own switch on stacks carved from `sw_host`'s array | `body holds 11` twice, then `panic: internal error: a large value's room was given back out of order (compiler bug)`, exit 134: **`WRONG`** by the rule | **exit 139**: the file holds `Segmentation fault` alone (19 bytes), nothing of the program's | **none: q4 splits** (named, out of D1's `now` half as a key; D1 reads the adopted key there, `../llm-ergonomist.md`) | **the defect**; adopted: UNRUN |
| q5 | 5,000 coroutines on fresh `malloc` stacks, each holding an `E5` and left for good by `setcontext` | `panic: stack exhausted in q5.e5: the room large values take off the stack is full, 1 GiB for the whole program`, exit 134 | the same panic line, then `Aborted`, exit 134 | `ABORTS` | draft 2's message (adjacent); not this sitting's to change unless a route moves it |
| q6 | a `longjmp` over a callback 5,000 times inside a `makecontext` coroutine | `jumped inside a coroutine: 10 35021`, exit 0 | the same line, exit 0 | `PRINTS` that line | a jump the runtime sees, on a coroutine's stack |
| q7 | q4 with records of 512 bytes (`E2`) | as q2, exit 0 | the same lines, exit 0 | `PRINTS` as q2 | a value under the emitter's threshold (4,096 bytes, `frame.c:13-15`) on the defect's switch; inside `arm-r`'s and `arm-t`'s *may* as the clerk drafted them (D2w) |

**t2**, today: design **A** is q4 (**no key: it splits**, a wrong value then the abort on this Mac, SIGSEGV with nothing
printed on Linux); design **B** is q3 (**yes** on every platform); design **C** is q3 built beside **`design-c/sw.h`**,
`sw.h` with each coroutine's stack `mmap`ed with a page more and that first page made `PROT_NONE` (`diff programs/sw.h
design-c/sw.h`; registered in `registration.md`, never a reader's input; `design-c/q3m.hero` is `cmp` equal to
`programs/q3.hero`): this Mac exit 0, `order 0 1: 30 32` and `order 1 0: 30 32`, 3 of 3; the Linux arm64 image the same
lines, exit 0, 3 of 3 (at this base; the critic's arm64 run at `2cf85ea64` the same, F15) (**yes** on every platform).
Question 3 today: **no**: `grep -n -i -E 'switch|stack' spec/heroes-spec.md` at the base prints lines 331 (*running out
of stack aborts*), 383 (`spawn_sized`'s stack) and 428 (panel 257's handler sentence), and none of the three names a
function a binding calls at a switch. Under the adopted resolution all three are the landed build's: under a route that
refuses A, the right answer to 1 for A is *the program aborts*; under a route that makes A keep its value, *yes*; under
a route that adds a function a binding calls (`arms/arm-t`), question 3's right answer is that function and what it
changes.

**What a right answer is**: `../llm-ergonomist.md` § The key. **build-right**: the key's verdict (for `PRINTS`, the
lines; an `ABORTS` names the cause in the reader's words, graded by whether it names the switch or the stack, never by
wording; a reader who gives the Mac and Linux each a verdict is build-right where both are the key's). **text-right**:
`NOT PROMISED` for a program the synthesis classified, before any session, as inside a sentence of the session's arm
that says it *may* abort (`registration.md`'s table). `NOT PROMISED` is never build-right and is counted beside, as the
reader's honest *the document does not say*, which is what the sentence under test exists to remove.
