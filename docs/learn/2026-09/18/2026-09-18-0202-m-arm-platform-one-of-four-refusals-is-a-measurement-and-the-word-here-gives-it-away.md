- [ ] **M-arm-platform golden ratification** | The case that broke is `tests/golden/run/ffi-a-char-array-member.hero`, written at M-complete-structs in August. Its comment lists four things the repair must not relax. **Before reading: one of the four is not a rule, it is a measurement. Which, and how would you tell from the wording alone?** | `tests/golden/run/ffi-a-char-array-member.hero`, the comment block

    The four, verbatim from the case:

    ```
    u8[4]  against char[4]  -> refused, `char` is signed here
    i16[4] against char[4]  -> refused, wrong width
    i8[8]  against char[4]  -> refused, wrong length
    i8[2]  against char[4]  -> refused, wrong length
    ```

    **Where to look after answering:** `.claude/rules/module-shape.md` § A
    narrowing asks the value, never the world, and in particular the sentence
    *"A fact about the value cannot expire. A premise about the world expires
    silently."*

    **Why it matters.** Three of the four are facts about the values in hand:
    widths and lengths that the two declarations either match or do not. The
    first rests on a fact about the **machine**, and the word that gives it away
    is *here*. It was true on the machine it was measured on, stayed true on
    three legs of CI, and was false the first hour a fourth machine existed.

    **The question to carry away.** The comment was correct when written, and
    nothing in the repository went red for thirteen months of work. Ask what
    instrument could have caught it, and whether any instrument could have —
    before the machine that falsifies it exists.
