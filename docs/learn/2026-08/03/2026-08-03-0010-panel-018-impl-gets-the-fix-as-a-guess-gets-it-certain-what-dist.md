---
kind: learn
area: none
milestone: none
filed: 2026-08-03
commit: none
github: none
---

- [ ] **panel 018 impl** | `for i < 3` gets the `while` fix as a Guess; `for !done` gets it Certain. What distinguishes the two parse states, given the message is identical?

    **Where to look:** syntax/stmt.rs for_stmt
    **Why it matters:** certainty is a property of the parse state, not of the message
