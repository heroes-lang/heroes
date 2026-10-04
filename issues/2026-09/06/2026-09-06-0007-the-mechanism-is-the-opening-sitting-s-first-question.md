---
kind: task
area: cli
milestone: M-typed-inspection
filed: 2026-09-06
commit: none
github: none
---

- [ ] **M-typed-inspection** | the mechanism is the opening sitting's first question | `CLAUDE.md` §10 · `selfhost/cli/table.hero` § run_flags · `docs/ROADMAP.md` § M-vscode-extension

    **Origin:** measured 2026-09-06, the cheap route RUN rather than argued.
    §10's stopping rule is asked before it.

    **Thirty lines of lldb Python turned an opaque array into its elements with
    no compiler change and no runtime change.** It read `len` out of the header,
    resolved the `elem` descriptor pointer to the symbol `hero_desc_str`, found
    the C type `HeroStr` and printed `"ada"` and `"grace"`; `nm` shows a user
    type links as `_h_desc_Room_desc`, so **the descriptor's own symbol name is
    the type name `HeroDesc` does not carry**, which is `runtime/parts/sort.c`'s
    pointer-identity trick one level up. So a `name` field on `HeroDesc` is
    refused before it is proposed: `HERO_RUNTIME_ABI` +1 to buy a string the
    linker already holds.

    What the sitting must rule: whether CLAUDE.md §10's *"never a script"*
    forbids a formatter the BUILD emits and nobody types (Rust ships
    `rust-lldb`, a wrapper script, which is exactly what §10 refuses); whether
    the surface is `heroes run --debug` (a flag: same input, same question,
    different how, and at `-O0` because `run` defaults to `-O2` and the locals
    are gone there), a new verb (which §10 admits only with a proven overload),
    or **nothing** (the conservative reading, refused on §12: two invocations
    cannot guarantee the formatter and the binary came from one build, and a
    stale formatter shows the wrong variable's name at exit 0).

    Not this milestone's: calling a generated `to_str` inside the stopped
    process, which allocates on that process's heap on whatever thread lldb
    picks, after M-isolated-threads gave every thread its own.

    **Where to look also:** `design.md:594`.
    **Why it matters:** the mechanism decides whether the compiler changes at
    all, and three of the four routes leave it untouched.

    **Re-verified 2026-09-10: STILL OPEN, no sitting held, no surface landed.**
    `grep -rn '"--debug"' selfhost/` is empty and `selfhost/cli/table.hero:90-97`'s
    `run_flags` are unchanged. One pointer moved: *"No typed variable inspection in
    v1"* is `design.md:613`, not `:594`.
