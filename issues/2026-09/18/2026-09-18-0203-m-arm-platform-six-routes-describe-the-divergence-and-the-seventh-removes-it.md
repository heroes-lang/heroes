- [ ] **M-arm-platform mutation drill** | Seven routes were argued for binding C's plain `char`. Six describe the divergence; one removes it. **Before reading: you have `i8` and `u8` and no third 8-bit type. Invent three ways to make one source file compile on a machine where `char` is signed and one where it is not, then rank yours by what a reader of the program can still tell about the header.** | `docs/panel/161-the-third-char-had-no-spelling-and-the-cheapest-answer-was-a-flag.md` § The routes, then § The resolution

    The seven, in the order they were named: a ninth integer type whose sign is
    the target's; a spelling legal only inside an `extern` group; accept either
    `i8` or `u8` everywhere; refuse plain `char` outright; bind it as `u8` on
    every target; split the rule by position; and pin the sign at the compiler
    invocation.

    **Where to look after answering:** the llm-ergonomist's report, which judged
    them as a reader of the specification alone and found that one of them is
    secretly two — with and without a sentence naming what the field READS as.

    **Why it matters.** The route adopted spends no specification token and adds
    no vocabulary. `spec § 13` already says a field is declared at *the header's
    own width and sign*; the flag makes that sentence true on every leg instead
    of weakening it. Six routes were ways of writing down that the machines
    disagree. The seventh makes them agree.

    **The question to carry away.** Ask which of your three the historian's
    precedent would have killed. One of them was proposed in Rust in 2018 and
    withdrawn by its own proposer in twenty-five hours; another shipped in Zig in
    2023 and had a cross-platform build break filed against it eleven months
    later; a third shipped in Swift and has carried a documentation comment that
    is publicly false since 2016.
