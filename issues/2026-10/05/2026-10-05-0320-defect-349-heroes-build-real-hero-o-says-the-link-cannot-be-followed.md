---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **349 — `heroes build real.hero -o ""` says the link cannot be followed for *the operating system's own reason is 0*, where there is no link and no reason** | `heroes build real.hero -o ""`: *cannot write : the link cannot be followed: the operating system's own reason is 0*; `argv` accepts an empty `-o` value and the empty landing path is read as a failed link (lane b11-windows, this Mac, 2026-10-05, the lane's report) | `selfhost/cli/argv.hero` (an empty `-o` accepted) · the landing of a build's output (`selfhost/cli/publish.hero`) · defect 346, the runtime's reasons told as the system's, a different cause · **class: blocking**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 346's repair (its final report, *Found beside*).

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a false message, a link and a reason named where there is neither; never deferred.
