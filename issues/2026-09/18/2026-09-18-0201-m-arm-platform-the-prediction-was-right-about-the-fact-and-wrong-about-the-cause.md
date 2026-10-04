- [ ] **M-arm-platform walkthrough** | A prediction was registered so it could be scored: *plain `char` is unsigned on the ARM ABI and signed on x86-64*. The new leg found the divergence. **Before reading: this Mac is an arm64 machine. What does it read, and what does your answer do to the word "ARM" in that sentence?** | `docs/work/milestones/M-arm-platform.md` § What warrants it, and the SCORED paragraph under it

    The three machines, measured with a three-line C program:

    ```
    Linux arm64     CHAR_MIN 0     CHAR_MAX 255    UNSIGNED
    Linux x86-64    CHAR_MIN -128  CHAR_MAX 127    SIGNED
    Darwin arm64    CHAR_MIN -128  CHAR_MAX 127    SIGNED     <- this Mac
    ```

    **Where to look after answering:** AAPCS64's Table 3, which does say plain
    `char` is an unsigned byte, and the single line directly under it: *"A
    platform ABI may specify a different combination of primitive variants but we
    discourage this."* Then Apple's own arm64 document, which says in as many
    words that `char` is signed.

    **Why it matters.** The prediction was right that a divergence existed and
    wrong about what it belonged to. Three legs of CI had all been signed-`char`
    platforms, so none of them could have shown it, and a fourth leg chosen for
    its **architecture** alone could have been an arm64 Darwin and shown nothing.
    What made this leg informative is the pair *arm64 AND Linux*, which nobody
    argued for by that name when the row was scheduled.

    **The question to carry away.** The milestone row said where to look as well
    as what to expect: *"a divergence in the corpus's 20 `extern` programs of
    55"*. The corpus reads 53 passed, 0 failed on the new machine. Ask what that
    means about a prediction's search location, and whether a prediction that is
    wrong about where to look is still a good prediction.
