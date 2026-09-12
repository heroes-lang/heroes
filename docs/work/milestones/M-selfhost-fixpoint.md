# M-selfhost-fixpoint — the fixpoint and the seed · **v1** *(closed 2026-08-18)*


A builds `B.c`, B builds `C.c`, `diff B.c C.c` empty — generated C, not binaries,
with the clang version pinned and recorded. The seed came with it:
`seed/heroes.c`, one clang line, no flags, tested from `git archive HEAD` because
a test run in the working tree proves nothing about a *checkout*.

**The archive left this milestone** (panel 085 B4) and became the two rows after
it. Three reasons, all measured: the port read its standard library from
`crates/` (archived 2026-08-19) at run time, so the self-hosted compiler was already broken outside
this repository and blamed the author's line for it; `tests/differential.rs` —
the instrument that found that and two more — has the **bootstrap** as its
expectation, so archiving it removes the only thing that can ask whether the two
compilers agree; and `heroes measure` was not in the port, while design.md §1.6
cited the bootstrap's `measure/gate.rs` as the spec budget's live enforcer.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
