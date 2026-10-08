---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: ec6a9e24adac4724e3555e42a68a49afc9fde9b4
github: none
---

- [x] **463 — a file replaced on Linux can change its SELinux label in silence** | the stage skips any `security.*` attribute it cannot set, so a replaced file could silently change label; no SELinux in the container to test (lane b14-runtime, unrun) | `runtime/parts/replace.c`, `runtime/parts/write.c` (batch 14's round, unmerged on 2026-10-07) · defect 438 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on a Linux with SELinux.

    Repaired at `ec6a9e24`, 2026-10-08 (lane b15-runtime), gated by its hand-written case and the Mac's `run fixedbugs-438`; the net is owed at the batch's close, and a C-boundary item closes only after the push's legs. Measured on Fedora Cloud 44 aarch64 under qemu, SELinux enforcing, a process confined as `user_u:user_r:user_t:s0` rewriting a file labelled `unconfined_u:object_r:user_tmp_t:s0` left `user_u:object_r:user_home_t:s0` through `write_file` and the publish route in silence, where an in-place write keeps the label: now the label is carried, or the new file already holds it, or the stage answers attributes (EACCES) and removes what it made, so `write_file` writes in place (label and inode kept) and the publish route refuses with the file untouched; `security.capability`, `.ima` and `.evm`, about the bytes, are not carried (as root the stage had been giving a file capability to the new bytes, which an in-place write drops). No golden form has SELinux: the case is `docs/platforms/linux/selinux-labels.c` with its machine and rows in its head.

## The repair

Repaired at `ec6a9e24`, 2026-10-08 (lane b15-runtime), gated by its hand-written case and the Mac's `run fixedbugs-438`; the net is owed at the batch's close, and a C-boundary item closes only after the push's legs. Measured on Fedora Cloud 44 aarch64 under qemu, SELinux enforcing, a process confined as `user_u:user_r:user_t:s0` rewriting a file labelled `unconfined_u:object_r:user_tmp_t:s0` left `user_u:object_r:user_home_t:s0` through `write_file` and the publish route in silence, where an in-place write keeps the label: now the label is carried, or the new file already holds it, or the stage answers attributes (EACCES) and removes what it made, so `write_file` writes in place (label and inode kept) and the publish route refuses with the file untouched; `security.capability`, `.ima` and `.evm`, about the bytes, are not carried (as root the stage had been giving a file capability to the new bytes, which an in-place write drops). No golden form has SELinux: the case is `docs/platforms/linux/selinux-labels.c` with its machine and rows in its head.

**Closed 2026-10-09**, after the push's platform legs, this defect being at the C boundary or changing what clang gets on every platform (`.claude/rules/verification.md` § The batch): batch 15 closed on this Mac alone (`811f8398`) and the CI's legs ran its cases afterwards, run 37844345400, created at 23:06 on 2026-10-08 and its Windows leg finished at 01:31 on 2026-10-09. Darwin arm64 read the net 6,960 passed and 0 failed, Linux arm64 and Linux x86-64 6,941 each, and on those three legs the compiler's own tests 1,478, the module's 269 and the net's own tests 318, all passed; the Windows leg read the compiler's own tests 1,478 and the module's 269, all passed, and the net 6,811 passed and 1 failed, defect 322's sanitiser case alone (defect 509), its net's own tests not reached. This defect's cases are not among any leg's SKIP lines, read from the four logs, and passed on every leg; defect 447's vcpkg step ran on the Windows leg and its SDL3 case ran there, where batch 14's leg had skipped it. Defect 463's case is the hand-written one its lane ran on a Linux with SELinux enforcing, which no leg is.
