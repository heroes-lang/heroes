---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: e38dd76590525149f504cd1a0631ba661fe5a345
github: none
---

- [x] **436 — a program stopped by Ctrl-Z leaves `heroes` waiting for ever** | a child of `heroes run` stopped by Ctrl-Z (SIGTSTP) leaves `heroes` waiting, since `hero_run_go` waits for exits only; already so before defect 425's repair (the lane's reading) | `runtime/parts/run.c`, `hero_run_go`'s wait · **class: adjacent**

    **Origin:** filed by the coordinator at 04:40 on 2026-10-07, from lane b13-tmpl407's report (*found beside*); the lane's reading, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): the command stops answering where the terminal's job control should hand it back.

    Repaired at `e38dd765`, 2026-10-07 (lane b13-tmpl407): a stopped child is never read as an exit: stop reports are consumed and the wait goes on; with a terminal, Ctrl-Z takes the terminal back and stops `heroes` itself with `raise(SIGTSTP)`, `fg` handing it back and continuing the child; SIGCONT sent after 425's SIGTERM; on Darwin the base killed the stopped program (exit 137), worse than filed (`runtime/parts/run.c`, the POSIX side); case `run/fixedbugs-436-a-child-that-stops-is-waited-on-and-not-killed`, pty probes on this Mac and Linux arm64 (the lane's); a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.

## The repair

Repaired at `e38dd765`, 2026-10-07 (lane b13-tmpl407): a stopped child is never read as an exit: stop reports are consumed and the wait goes on; with a terminal, Ctrl-Z takes the terminal back and stops `heroes` itself with `raise(SIGTSTP)`, `fg` handing it back and continuing the child; SIGCONT sent after 425's SIGTERM; on Darwin the base killed the stopped program (exit 137), worse than filed (`runtime/parts/run.c`, the POSIX side); case `run/fixedbugs-436-a-child-that-stops-is-waited-on-and-not-killed`, pty probes on this Mac and Linux arm64 (the lane's); a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.

**Gated 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.

**Closed 2026-10-07** after the push's platform legs, a defect at the C boundary closing only after them (`.claude/rules/verification.md` § The batch), the last Windows leg sent to the CI by the author at 10:19 (`issues/2026-10/07/2026-10-07-1019-the-author-sends-batch-13-s-last-windows-leg-to-the-ci.md`): the CI's run 37603092655 on the pushed `34f95b71`, every job a success, read at 13:43; Linux x86-64 the compiler's own tests 1,357, the full net 6,536 passed and the net's own tests 289, 0 failed; Linux arm64 1,357, 6,536 and 289; Darwin arm64 1,357, 6,571 and 289; Windows x86-64 1,357, 6,422 and 289. The cases whose header Windows lacks were skipped there by name and run where the header is: defect 092's `getsockopt`, 396's POSIX and SHA-256 buffers and 436's stopped child on both Linux legs and Darwin, 413's zlib case on the three and its libuv case on Darwin alone, `pkg-config` on both Linux legs not knowing `libuv`.
