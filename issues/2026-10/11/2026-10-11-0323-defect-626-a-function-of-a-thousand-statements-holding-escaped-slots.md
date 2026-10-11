---
kind: defect
area: emit
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **626 — a function of a thousand statements holding escaped slots costs ASan's stack instrumentation most of a `--sanitize -O2` build** | defect 539's thousand-deep case after defect 622's repair: Apple clang 21 on this Mac, its unit at `-O2` with the build's words, in clang's own process: 65.5e9 instructions alone, 356.5e9 under `-fsanitize=address`, 358.4e9 under both sanitizers; `-mllvm -asan-stack=0` (a diagnostic, never a build's word) took ASan's time from 38.2 s to 9.1 s, and `-ftime-report` gave the Greedy Register Allocator 24.8 s, the Machine Instruction Scheduler 11.3 s and instruction selection 4.5 s of 45.6 s, the middle end under 1 s. The chain's `top` builds a thousand levels in one body and holds 2,002 slots and about 4,000 temporaries, each a stack object ASan pads and poisons, since `HERO_COPY_HELD` hands every slot's address to `hero_slot_escape` (defect 566); the growth is past linear in the statements: 35.2e9, 79.6e9 and 358.4e9 under both sanitizers at 250, 500 and 1,000 levels, against 12.1e9, 27.4e9 and 65.5e9 without | the slots and temporaries `selfhost/emit/` gives a construction statement past `typeorder`'s depth, and `selfhost/emit/byte_copy.hero`'s `HERO_COPY_HELD`, which keeps every one of them on the stack; what ASan's stack instrumentation then costs the backend. Routes measured and refused, each on this Mac: `-asan-stack=0` drops stack-buffer-overflow detection; copies that do not escape bring back defect 566's MemCpyOpt stack-move on the Windows box's clang 23 (unrun here); `noinline` on the per-type functions took 311.1e9 instructions but multiplied a sanitised program's run time by 16 (defect 622) · defects 539, 566, 605, 622 · **class: improvement**

    **Origin:** measured by lane b21-misc at 03:23 on 2026-10-11 while repairing defect 622, whose per-type share (UBSan's member-access checks compounding through the chain's inlined functions) that repair removed; this is the share that stayed, the program's own function and not its types' (`.claude/worktrees/scratch-b15/b21-misc/d622/`, ignored by git: the units, the `-ftime-report`s, the instruction counts by `/usr/bin/time -l` with `-fintegrated-cc1`).

    **Class: improvement**, 2026-10-11: nothing is wrong at exit 0; the case builds in every configuration and prints `x` and `yz`, under defect 605's watchdog by the measurement that set it; on the CI's clang 18 in a container of four CPUs on a loaded Mac the unit took 105.4 and 106.3 s of user time at `--sanitize -O2` after defect 622 against 128.6 and 121.4 s before, interleaved. A cost, not a refusal (CLAUDE.md § 13).
