---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: a82dd2a11432cb5de134383432ef3c16a3ad3814
github: none
---

- [ ] **416 — `build` and `test` disagree on a handle the program build never reaches** | `record UH tag utag` over a union's tag, unused or bound only in a test: `build` and `run` exit 0 while `test` refuses `ffi_tag_is_a_union`, even with no test block; the same for a misspelled tag (`ffi_unknown_name`); each message is true, and the verbs disagree on whether it is asked | the records the pointee and tag questions reach per verb, `selfhost/cli/` · **class: adjacent**

    **Origin:** lane b13-run400, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a mistake told by one verb and not another; both messages true.

    Repaired at `a82dd2a1`, 2026-10-07 (lane b13-run400): every verb asks the header about every group record a module declares (`emit/members.hero`'s `group_records`, wanted by declaration in `emit/unit.hero`), so `build`, `run`, `test` and `--emit-c` tell a refuted handle alike; the header probe asked once per build and its digests seeding each round's memo (`cli/assemble.hero`, `cli/produce.hero`), a warm `run` of `print(1)` from 2.58 to 2.09 billion instructions (the lane's count); cases `unsupported/fixedbugs-416-*` and `run/fixedbugs-416-handles-nothing-reaches-run`; gated by its cases, the compiler's own tests and the lane's run, corpus, emission, determinism, lines, warnings, layout, order, canonical and records; a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.
