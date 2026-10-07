---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 8b151864380455e5b280467fff56b30e41ed6b2d
github: none
---

- [ ] **434 — a comment in `cli/artifact.hero` is false for program builds** | *`emit_maybe` with the learned tags is exactly what `assemble.fused` compiles* holds for `--emit-c`, whose artifact is the fused text, and not for a program build, whose rounds compile the per-module units | `selfhost/cli/artifact.hero` · **class: improvement**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-run400's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false comment, no program judged wrong.

    Repaired at `8b151864`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The comment now says that `--emit-c` is a program build, whose rounds compile one unit per module, so no clang run of that build reads the artifact's bytes, and that only `heroes test` compiles the fused unit; measured by hand to write it, clang read the artifacts of the 372 `run` programs under the build's flags with no error and no warning.
