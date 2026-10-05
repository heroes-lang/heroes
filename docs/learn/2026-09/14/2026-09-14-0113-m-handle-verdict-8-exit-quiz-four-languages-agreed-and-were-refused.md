- [ ] **M-handle-verdict 8** | Four questions, in order, and none of them needs the compiler. **(a)** Zig renamed `c_void` to `anyopaque`, Terra spells it `&opaque`, Odin spells it `rawptr`, Swift's word for *pointee unknown* is `Raw`. Heroes was offered `opaque`, it was the **cheapest candidate measured** (+0 tokens on the binding tokeniser, where `unsafe_ptr` and `raw_ptr` both cost +10), and it was refused. Give the two reasons. **(b)** The sitting kept the name `ptr`. Write down the one thing that would prove that decision wrong. **(c)** `@tail` was changed from `ptr` to `cstr`. The program's output did not change by a byte. Say what did. **(d)** Panel 140 measured `ptr` escaping an `extern` group **11 times in 2 files** and used that number to refuse a `raw` module. That number is now **0**. Does the refusal get stronger or weaker, and why?

    **Where to look:**
    `docs/panel/146-the-name-survives-and-the-sitting-convened-over-it-found-a-cast.md`,
    §§ 1a, 1b, 1c and 2; design.md Part 6's `raw` row and its dated correction;
    `examples/sqlite/main.hero:56`.

    **Why it matters:** (a) is the one where agreeing with everybody is the wrong
    answer. The specification already spends the word *opaque* on the handle, so
    `db: opaque` would read as though it already **were** one — and separately,
    the ffi seat measured that *unnameable pointee* is **false at 6 of the 9**
    surviving positions, where the header names the type perfectly well and only
    this language cannot spell it. Four languages converged on a word that is
    wrong here for two independent reasons, and precedent is advisory for exactly
    this kind of case.

    (c) is the one that matters most. `@tail: ptr` makes this compiler emit
    `void ** a4` and then write `(void *)a4` at the call — **a cast the compiler
    inserts on your behalf.** `@tail: cstr` emits `const char ** a4` and hands
    clang the exact type to check. Nothing about the program's behaviour moved;
    what moved is **who is checking**. A sitting convened to decide a word found
    that, and robustness outranks naming by the contract's own precedence order,
    so the word became the smaller half of its own meeting.
