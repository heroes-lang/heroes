---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **416 — `build` and `test` disagree on a handle the program build never reaches** | `record UH tag utag` over a union's tag, unused or bound only in a test: `build` and `run` exit 0 while `test` refuses `ffi_tag_is_a_union`, even with no test block; the same for a misspelled tag (`ffi_unknown_name`); each message is true, and the verbs disagree on whether it is asked | the records the pointee and tag questions reach per verb, `selfhost/cli/` · **class: adjacent**

    **Origin:** lane b13-run400, 2026-10-06 (its report, *found beside*), filed by the coordinator at 23:03; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a mistake told by one verb and not another; both messages true.
