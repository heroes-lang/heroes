---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: none
github: none
---

- [ ] **454 — `suite_cache`'s check that an untouched build still hits leaves the runtime object out** | the check excludes `runtime-` objects, so nothing at the filesystem witnesses defect 435's reading that a warm build compiles no runtime (lane b14-cli, 2026-10-07) | `tests/harness/suite_cache.hero` · defect 435 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*decisions* 3).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): coverage.
