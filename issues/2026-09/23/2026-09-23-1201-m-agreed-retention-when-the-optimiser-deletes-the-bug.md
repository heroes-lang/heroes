---
kind: learn
area: cli
milestone: M-agreed-retention
filed: 2026-09-23
commit: none
github: none
---

- [ ] **M-agreed-retention walkthrough** | A probe freed one `malloc` twice through two `static inline` C functions in a local header. `heroes build` ran it at exit 133; `heroes run` ran the SAME file at exit 0 and printed its last line, five times out of five. Marking the two functions `noinline` made both verbs die. **Before reading: the two verbs differ in one default. Which one, and why can a double free vanish without anybody fixing it?** | `selfhost/cli/compile.hero`'s `level_from` · C11 on undefined behaviour

    **Where to look after answering:** `heroes run` defaults to `-O2` and
    `heroes build` to `-O0`. At `-O2` clang sees the allocation and both frees
    inside one function, and a program whose every path is undefined may be
    compiled to anything, including nothing. The program did not become
    correct; the evidence was deleted.

    **The question to carry away.** A reproducer that goes green under
    optimisation is measuring the optimiser. Say what a probe of a C library
    must keep out of the compiler's sight to measure the library instead.
