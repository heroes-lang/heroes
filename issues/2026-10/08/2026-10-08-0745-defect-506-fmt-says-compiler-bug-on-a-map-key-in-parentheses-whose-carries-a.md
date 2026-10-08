---
kind: defect
area: print
milestone: none
filed: 2026-10-08
commit: 044764780e258bc61c864e84de8849bab942a93f
github: none
---

- [ ] **506 — `fmt` says *compiler bug* on a map key in parentheses whose `(` carries a comment** | `m = {` / `(  # c1` / `"ziggy"  # remark` / `): 8` / `}`: `check` exit 0; `fmt` exit 2, *`fmt` would move the comment on line 3 …* and *this is a compiler bug — … was NOT changed* (lane b15-parse's reproducer `<scratchpad>/batch15/parse/s504/cm/bind_paren_c.hero`, run by the coordinator before 07:45 on 2026-10-08 on the trunk's compiler); the base at `56def9b4` fails the same way; `heroes probe` on lane b15-print's v25 shape (a lone `{` over a parenthesised key) refuses 20 of 152 parsing variants for this cause | `selfhost/print/`, a comment on a parenthesised map key's `(` · defects 503 and 504 · **class: blocking**

    **Origin:** filed by the coordinator at 07:45 on 2026-10-08 from lane b15-parse's message (*found beside* 504, not its cause), reproduced before filing.

    **Class: blocking**, 2026-10-08 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 on a correct program, told as a compiler bug.

    Repaired at `044764780e258bc61c864e84de8849bab942a93f`, 2026-10-08, gated by its cases and the compiler's own tests; the net is owed at the batch's close. `print/around.starts`, the line an element begins on, asked for the author's parentheses around the entry's value, and a map's entry begins with its key, so a `(` alone on the key's line was read as before the entry and the comment after it printed above it; it asks the key's now. With 504 merged, `heroes probe` over the v25 file in every family leaves 3 of 147 parsing variants refused on the tree without this repair and 0 of 147 with it, and the reproducer's 109 of 109 go to 0; its case is `tests/golden/run/fixedbugs-506-a-comment-on-a-map-key-s-parenthesis` and two compiler tests, red on the tree without it.
