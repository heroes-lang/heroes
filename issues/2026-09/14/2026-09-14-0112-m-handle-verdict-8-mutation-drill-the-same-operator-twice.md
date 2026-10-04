- [ ] **M-handle-verdict 8** | Run `./heroes mutate examples --operator swap-ptr` today and write down the three numbers. Then build the compiler as it was before the form — `git show e0f7b84f~1:seed/heroes.c > /tmp/before.c && clang -I runtime /tmp/before.c runtime/runtime.c -o /tmp/before` — check out `examples/` as it was at that commit into a scratch directory, and run the same operator there. Write down those three numbers too. **Before comparing them, say what would have to be true for the two rates to be comparable at all.**

    **Where to look:**
    `docs/measurements/029-the-sixteenth-operator-and-a-kill-rate-that-means-the-opposite.md`;
    `selfhost/mutate/handles.hero`; § Scored at step 3 of
    `docs/panel/145-a-handle-is-a-pointer-with-a-name-and-the-compiler-already-reads-the-name.md`.

    **Why it matters:** the operator was built **before** the form it exists to
    judge, deliberately and against the obvious order, and the reason is the
    whole exercise. A mutation operator finds its sites by reading the *written*
    type. Retype the bindings first and every site vanishes — the operator finds
    nothing to mutate, reports a perfect rate over an empty denominator, and the
    form is scored against **silence**. Building the operator first kept the
    denominator fixed at 15 across the change, which is the only reason the two
    rates mean anything side by side. That failure mode was written into the
    measurement's own prediction before it could happen.

    **The second half is the number itself.** Before the form: 15 mutants, 8
    killed, **53%** — and **0 of the 7** that were the class the operator exists
    to measure. After: 15 of 15, and 7 of 7. A headline that read *the language
    already catches half of these* was reporting eight deaths from rules about
    orphaned names and doubled `@` marks, neither of which can tell one C pointer
    from another. Ask, as you read the survivors: **what is the smallest change
    to an operator that would make its rate honest without changing a single
    thing about the language?**
