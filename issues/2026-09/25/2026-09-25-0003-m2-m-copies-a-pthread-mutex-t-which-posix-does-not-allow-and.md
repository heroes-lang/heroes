---
kind: feature
area: records
milestone: M-buildable-structs
filed: 2026-09-25
commit: none
github: none
---

- [ ] **M-buildable-structs** | `m2 = m` copies a `pthread_mutex_t`, which POSIX does not allow, and nothing refuses it | `docs/panel/178-reports/completeness-critic.md` § 5 question 2

    **Origin:** panel 178's completeness critic, 2026-09-25: 19 of 50 blind
    readers raised copy-in/copy-out of the mutex on their own. The emitted C
    passes the cell's address (`pthread_mutex_lock(&h0_m)`), so a local cell
    does not move between calls, but a value copy of a locked mutex is
    **unrun**. Whether a group record can say it is not a value is the question.
