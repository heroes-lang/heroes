- [ ] **270 — a call to `sqrt` with no binding is asked *did you mean `sort`?*, where a note offering the C function would point to the repair** | `print(sqrt(2.0))` with no `extern`: `check` exit 1, `unknown_name` *nothing named `sqrt` is in scope, did you mean `sort`?*, *fix (guess): rename to `sort`* (batch 8's round compiler at `1eb854c3`, 2026-10-04, `tests/golden/check/fixedbugs-220-sqrt-is-not-sort.hero`), a guess since defect 220 and still the only route the message names | `selfhost/rename_fit.hero` and the resolver's did-you-mean (`selfhost/resolve/`) · **class: improvement**

    **Origin:** batch 8's source lane, 2026-10-03 (its report's *Found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a guess that is true and points away from the repair; no program refused or wrong.
