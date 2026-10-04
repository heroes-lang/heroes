---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 4370bd60df08fb7114bbf4812e7385000ef94ee1
github: none
---

- [ ] **323 — a `pkg-config` that is on the PATH and cannot start is told *pkg-config is not on this machine*, a false message** | a `pkg-config` on the PATH whose file the machine refuses to start (mode 644, the operating system's reason 13, `EACCES`): `build` of a group naming a `package` is told *pkg-config is not on this machine*, which is false; the program is there and could not be started (lane b10-cli's compiler at `86b29733`, 2026-10-04, found beside defect 249) | `selfhost/cli/package_answer.hero:88` (the message, written for every start failure) · `selfhost/cli/toolchain.hero:195`, where `run_clang` already asks the operating system's own reason · defect 249's repair · **class: blocking**

    **Origin:** lane b10-cli, 2026-10-04, found beside 249 and reproduced on its compiler; filed by the coordinator for the same lane, being blocking and in its files.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message about the machine (`.claude/rules/verification.md` § Bounded discovery's *a false message*); a cause apart from 249's, which was the bare line's missing code, place and route.

    Repaired at `4370bd60`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
