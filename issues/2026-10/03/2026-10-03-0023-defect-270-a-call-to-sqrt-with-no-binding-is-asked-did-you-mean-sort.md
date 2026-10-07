---
kind: defect
area: golden
milestone: none
filed: 2026-10-03
commit: 2bed5f3146fcd4aa662ef5be60da7ea7cd3f7142
github: none
---

- [ ] **270 — a call to `sqrt` with no binding is asked *did you mean `sort`?*, where a note offering the C function would point to the repair** | `print(sqrt(2.0))` with no `extern`: `check` exit 1, `unknown_name` *nothing named `sqrt` is in scope, did you mean `sort`?*, *fix (guess): rename to `sort`* (batch 8's round compiler at `1eb854c3`, 2026-10-04, `tests/golden/check/fixedbugs-220-sqrt-is-not-sort.hero`), a guess since defect 220 and still the only route the message names | `selfhost/rename_fit.hero` and the resolver's did-you-mean (`selfhost/resolve/`) · **class: improvement**

    **Origin:** batch 8's source lane, 2026-10-03 (its report's *Found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a guess that is true and points away from the repair; no program refused or wrong.

    Repaired at `2bed5f31`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A name nothing binds that is a call's callee, and the name after a dot, now carry a note saying how a function of C is bound, *if `sqrt` is a function of C, an `extern` group in this file binds it*, with the group's line and the signature's shape in the name as written (§4.19); it says *if*, so it rests on no knowledge of which names are C's, a name only read is told nothing of C, and the rename stays a guess; no sitting rules the words (design.md §4.17, §4.19 and `docs/panel/` grepped). `full` 20, `check` 585, `permissive` 10, `unsupported` 188, the compiler's own tests 1,359, all passed.
