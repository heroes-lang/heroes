---
kind: defect
area: check
milestone: none
filed: 2026-10-11
commit: 7c01e6f96b9dd80678938e5a1eaf0c4747589eb6
github: none
---

- [ ] **629 — a handle consumed through the UFCS receiver of a parameter without `@` is accepted, where the named call is refused** | `function finish(n: Node)` whose body is `n.node_free()`, `node_free` a C function whose parameter `consumes`, is accepted, and a later use of the caller's handle aborts 134 at run time; the named call `node_free(n)` in the same body is refused `consumed_borrowed_handle`, which the checker asks only on the named-call path (`selfhost/check/walk.hero:1202`); `h.h_into(...)` for `transfers` the same | `selfhost/check/walk.hero` and the UFCS call's lowering of a consuming parameter; panel 210 R4 · **class: blocking**

    **Origin:** filed by the coordinator at 03:05 on 2026-10-11 from panel 210's completeness critic (`docs/panel/210-reports/critic.md`), its builds under `.claude/worktrees/scratch-b15/210-critic/` (ignored by git), the seat's measurement on the tree frozen at `1dd890751`, not re-run by the coordinator.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted (memory stays safe: the abort is the runtime's).

    Repaired at `7c01e6f96b9dd80678938e5a1eaf0c4747589eb6`, 2026-10-11 (lane b21-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The dotted call hands its receiver as the first argument (`moved_calls.all_arguments`, the moved rule's own reading) to `consuming.refuse_borrowed_dotted`, asked in `check/walk.hero`'s `method` once the field half has declined, so a function-typed field is never judged as the extern; `ufcs_hides_the_marker` moved to `check/markers.hero` to pay walk.hero's line (1870, its `DECIDED` ceiling). Reproduced first on the lane's compiler at `d56e14429`: the critic's `c04b` and `c07b` exit 0, `c05b` and `c07c` refused. Shapes measured, each now as the named call: refused, the receiver, a field of it (`o.h.h_close()`), inside a binding, inside an argument, a `when 0` result, a `transfers` at the receiver and one behind a receiver (`parent.h_adopt(child: h)`); left alone in both spellings, an `@` parameter whole or by field (the critic's `c04`, `c07`, `c09`), a local, `retains`, `borrows`, a receiver that is a call's result. A module-qualified call cannot reach an extern (`extern_across_modules`), so that path needs nothing. Two `tests/golden/check/fixedbugs-629-*` cases; check, annotations and fixes 2/0 narrowed, layout over `selfhost/check` 4/0, own tests 1607 passed; the 214 tracked files naming `consumes` or `transfers` check alike on trunk and lane; instructions retired within run-to-run spread (the commit body's numbers).
