---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: 805a6974f55ce1c24cda9c2e5cfb133530d734e6
github: none
---

- [ ] **381 — `xs @ xs.push(e)` copies the whole array when `e` branches, against spec § 10's _grows in place_** | 40,000 pushes onto a plain name: `bytes @ bytes.push(text[at].to_i64().must())` retires 48,118,251,823 instructions, the same value bound to a name on the line above 44,782,121; `--dump-ir` shows a `load` of the array before the `must:` branch and `call builtin push` after it, where the named form has `push_owned`. In the pushed value `.must()`, `.default(0)`, `?`, `&&` and `||` each lose `push_owned`; `/` and `%`, which abort without a branch, keep it (the round's compiler at `ca7cf585`, measured by the coordinator at 07:33 and 09:48 on 2026-10-06) | `selfhost/ir/place_store.hero`, `pattern`: the rewrite asks for `load p` and `store p` in ONE block, and a branching value puts them in two · spec § 10 · **class: blocking**

    **Origin:** lane b12-cli12, 2026-10-06 (its final report; reproducers in its scratchpad, lost at the machine's restart that morning); reproduced, its shapes probed and filed by the coordinator at 09:49.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): spec § 10 says *`xs @ xs.push(4)` grows in place when `xs` is a plain name*, and for these shapes the compiler copies every element on every push, so a loop written as the specification teaches runs in quadratic time; the specification beats the compiler (CLAUDE.md § 12), and a sentence of it made false is truth failing. Lane cli12 proposed `adjacent`; the coordinator took `blocking` on that sentence.

    The reproducer, whole:

        function fill(text: str) -> i64
            bytes: [i64] @ []
            at: i64 @ 0

            while at < text.len()
                bytes @ bytes.push(text[at].to_i64().must())
                at @ at + 1

            return bytes.len()

        function main()
            print(fill("ab".repeat(20000)))

    `place_store.hero`'s own header names the shape it keeps classic on purpose, *anything that could mutate p while the pushed value is computed (`f(@p)` in an argument, a block re-entering p)*; a branch to an abort or a short circuit writes nothing, so what the repair must keep is that reason, not the one-block premise standing in for it.

    Repaired at `805a6974`, 2026-10-06 (lane b12-ir12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The title's closing read `*grows in place***` until then, which `records/lists` refused (26 passed and 1 failed at 10:51 on the lane's tree, this file as it was filed); the italic is written `_grows in place_` since, and `records` reads 27 and 0.
