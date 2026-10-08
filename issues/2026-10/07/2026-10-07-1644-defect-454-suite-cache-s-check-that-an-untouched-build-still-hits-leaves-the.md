---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: 2e03165d105f59d24d005f4eed38e7f6576974df
github: none
---

- [ ] **454 — `suite_cache`'s check that an untouched build still hits leaves the runtime object out** | the check excludes `runtime-` objects, so nothing at the filesystem witnesses defect 435's reading that a warm build compiles no runtime (lane b14-cli, 2026-10-07) | `tests/harness/suite_cache.hero` · defect 435 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*decisions* 3).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): coverage.

    Repaired at `2e03165d`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests; the net is owed at the batch's close. `objects_newer_than` asks every object under the build directory, the runtime's among them, so an untouched build compiling no runtime and a header edit moving no runtime are witnessed at the filesystem; the build directory is a parameter, so a stand-in that rewrites a `runtime-` object at every build is told by its name (red on the base, whose check read it green), and `cache`, run alone, reads 7 passed, 0 failed.
