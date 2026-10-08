---
kind: defect
area: harness
milestone: none
filed: 2026-10-07
commit: a61521bf3bc97d87ee3c3e877d466f7be6229e50
github: none
---

- [ ] **450 — three suites spell a missing header's tail themselves instead of reading it from `absence`** | `suite_annotations`, `suite_corpus` and `suite_special` spell *clang looked and did not find it* themselves; it matches the compiler today, and `absence.HEADER_LOOKED` holds it since defect 328 (lane b14-harness-a, 2026-10-07) | the three suites · defect 328 · **class: improvement**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-harness-a's final report (*found beside*).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): copies that can lag the compiler, defect 328's own cause; no case reads wrong today.

    Repaired at `a61521bf`, 2026-10-08 (lane b15-harness), gated by its cases and the net's own tests; the net is owed at the batch's close. The six copies of the three suites read `absence.HEADER_LOOKED`, and `suite_golden`'s three copies of a group's owner, a package's close and pkg-config's note (the same cause, found beside) read absence's pieces, each string byte-identical; a test of `absence.hero` reads every other module of the net for fifteen of the compiler's pieces, red on the copies (59 tests, 1 failed) and green without them.
