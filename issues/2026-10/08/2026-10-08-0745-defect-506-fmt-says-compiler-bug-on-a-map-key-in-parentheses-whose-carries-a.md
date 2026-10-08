---
kind: defect
area: print
milestone: none
filed: 2026-10-08
commit: none
github: none
---

- [ ] **506 — `fmt` says *compiler bug* on a map key in parentheses whose `(` carries a comment** | `m = {` / `(  # c1` / `"ziggy"  # remark` / `): 8` / `}`: `check` exit 0; `fmt` exit 2, *`fmt` would move the comment on line 3 …* and *this is a compiler bug — … was NOT changed* (lane b15-parse's reproducer `<scratchpad>/batch15/parse/s504/cm/bind_paren_c.hero`, run by the coordinator before 07:45 on 2026-10-08 on the trunk's compiler); the base at `56def9b4` fails the same way; `heroes probe` on lane b15-print's v25 shape (a lone `{` over a parenthesised key) refuses 20 of 152 parsing variants for this cause | `selfhost/print/`, a comment on a parenthesised map key's `(` · defects 503 and 504 · **class: blocking**

    **Origin:** filed by the coordinator at 07:45 on 2026-10-08 from lane b15-parse's message (*found beside* 504, not its cause), reproduced before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program, told as a compiler bug.
