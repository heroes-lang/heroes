---
kind: defect
area: runtime
milestone: none
filed: 2026-10-07
commit: ec6a9e24adac4724e3555e42a68a49afc9fde9b4
github: none
---

- [ ] **463 — a file replaced on Linux can change its SELinux label in silence** | the stage skips any `security.*` attribute it cannot set, so a replaced file could silently change label; no SELinux in the container to test (lane b14-runtime, unrun) | `runtime/parts/replace.c`, `runtime/parts/write.c` (batch 14's round, unmerged on 2026-10-07) · defect 438 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on a Linux with SELinux.

    Repaired at `ec6a9e24`, 2026-10-08 (lane b15-runtime), gated by its hand-written case and the Mac's `run fixedbugs-438`; the net is owed at the batch's close, and a C-boundary item closes only after the push's legs. Measured on Fedora Cloud 44 aarch64 under qemu, SELinux enforcing, a process confined as `user_u:user_r:user_t:s0` rewriting a file labelled `unconfined_u:object_r:user_tmp_t:s0` left `user_u:object_r:user_home_t:s0` through `write_file` and the publish route in silence, where an in-place write keeps the label: now the label is carried, or the new file already holds it, or the stage answers attributes (EACCES) and removes what it made, so `write_file` writes in place (label and inode kept) and the publish route refuses with the file untouched; `security.capability`, `.ima` and `.evm`, about the bytes, are not carried (as root the stage had been giving a file capability to the new bytes, which an in-place write drops). No golden form has SELinux: the case is `docs/platforms/linux/selinux-labels.c` with its machine and rows in its head.
