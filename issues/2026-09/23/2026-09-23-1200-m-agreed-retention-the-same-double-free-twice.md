---
kind: learn
area: runtime
milestone: M-agreed-retention
filed: 2026-09-23
commit: none
github: none
---

- [ ] **M-agreed-retention walkthrough** | The same program was run twice: a pointer C made, handed twice to a C function that frees it. Written with a raw `ptr`, Darwin kills it with signal 5 and **zero bytes** of explanation. Written with `record Box tag box`, `acquires box_close` and `consumes`, it dies at exit 134 with 396 bytes saying *given back that were never taken*. **Before reading: the C code is identical. Which of the two words makes the difference, and at what moment of the second call does the runtime speak — before C's `free`, or after it?** | `runtime/parts/alloc.c`'s `hero_handle_consumed` · `docs/roadmap/milestones/M-agreed-retention.md`, first item

    **Where to look after answering:** the handle's declaration is what gives
    the pointer a place in the runtime's set, and the set is asked BEFORE the
    call hands the address to C. A raw `ptr` has no declaration that could
    arm that question, which is why the milestone asks about the `ptr` that
    could have been a handle rather than about a missing mechanism.

    **The question to carry away.** Defect 075 is the same set failing the other
    way: `popen` then `fclose` balances it perfectly. Say what the set remembers
    about each handle, and what one more fact per entry would let it refuse.
