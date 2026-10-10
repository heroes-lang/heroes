---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: 33d199dc3bd7447cc55c8bb0ce5a2f6461ba59c4
github: none
---

- [ ] **566 — records a thousand deep holding a `str` take past the watchdog to build at `-O2` on Windows, and the CI's Windows leg is red** | batch 17's push (`65b78f2e`), run 37993565819: the Windows x86-64 leg reads 7,143 passed and 4 failed, `run` and `warnings` on `fixedbugs-539-records-875-deep-holding-a-str-build` and `fixedbugs-539-records-a-thousand-deep-holding-a-str-build` at `-O2`, each `build` ending at exit 124, the harness's watchdog of 120 s (`tests/harness/shell.hero:283`); on this Mac at 00:37, load 3.35, the thousand-deep case builds in 21.79 s at `-O2` against 2.74 s at `-O0`, `real` near `user`, and its C is 4,599,048 bytes against 3,742,441 at 875 deep, 1.23 times the bytes for 1.14 times the depth | the C defect 539's repair emits for a deep record's slots and their release, `selfhost/emit/`, and what clang's optimiser does with it; the shapes beside: the 875 case, an `i64` chain at 7,000 deep, `-O1`, a record held in an array · defect 539 · **class: blocking**

    **Origin:** filed by the coordinator at 00:39 on 2026-10-10 from the CI's Windows x86-64 leg of batch 17's push, its job log read through the API (kept under `.claude/worktrees/scratch-b15/gate17/post/ci-win.log`, ignored by git), as `.claude/rules/verification.md` § The optimistic chain item 5 asks; the Mac's two builds and the emitted C's sizes measured by the coordinator with the trunk's compiler built from the seed at `86189b2b`.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI, and a program that builds on one platform and not, within the bound, on another.

    Repaired at `33d199dc`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Two causes, each read in clang's `-ftime-report` under the build's own words: on this Mac a chain's per-type functions inlined into each other, a thousand inlined frames per release, the assembly printer and two debug-value analyses 54e9 of 143e9 instructions, so past `typeorder`'s depth they carry `nodebug`; on the Windows box (clang 23.1.1) `MemCpyOpt`'s stack-move held 120.6 of 129 s, so a counted struct's byte copy passes both addresses through `hero_slot_escape` (`HERO_COPY_HELD`), a struct holding nothing counted keeping the plain copy. clang `-O2` on this Mac at 250, 500 and 1,000 levels: 14.64, 40.58 and 143.32e9 instructions before, 12.08, 27.26 and 65.39e9 after; `heroes build -O2` of the thousand-deep case 26.06 s to 8.05 s here, and 11.8 s on the box, the 875-deep 12.2 s, every output as expected. The commit's subject says *linear time*: what the body measures is about 2.3 to 2.4 times the instructions per doubling of the depth, the growth `-O0` already has, not linear. Five blessed emissions moved, read by kind; `run` narrowed 9, `warnings` 499, `determinism` 473, the compiler's 1,546 tests, 0 failed.
