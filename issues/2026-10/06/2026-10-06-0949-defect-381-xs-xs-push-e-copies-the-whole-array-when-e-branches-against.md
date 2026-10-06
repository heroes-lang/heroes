---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: 805a6974f55ce1c24cda9c2e5cfb133530d734e6
github: none
---

- [x] **381 — `xs @ xs.push(e)` copies the whole array when `e` branches, against spec § 10's in-place growth** | 40,000 pushes onto a plain name: `bytes @ bytes.push(text[at].to_i64().must())` retires 48,118,251,823 instructions, the same value bound to a name on the line above 44,782,121; `--dump-ir` shows a `load` of the array before the `must:` branch and `call builtin push` after it, where the named form has `push_owned`. In the pushed value `.must()`, `.default(0)`, `?`, `&&` and `||` each lose `push_owned`; `/` and `%`, which abort without a branch, keep it (the round's compiler at `ca7cf585`, measured by the coordinator at 07:33 and 09:48 on 2026-10-06) | `selfhost/ir/place_store.hero`, `pattern`: the rewrite asks for `load p` and `store p` in ONE block, and a branching value puts them in two · spec § 10 · **class: blocking**

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

## The repair

Repaired at `805a6974`. `xs @ xs.push(e)` copied the whole array at every push when `e` held `.must()`, `.default()`, `?`, `&&` or `||`, because the place store asked for the load of `xs` and its store in one block, a premise standing in for its reason. It now asks the reason itself, whether anything can write `xs` on a path from the load to the store (`selfhost/ir/between.hero`, a walk backwards from the store's block that stops at the load's). The issue's reproducer, 40,000 pushes, from 48,135,983,642 to 44,771,220 instructions at -O0 and from 19,298,562,339 to 23,367,200 at -O2. Its cases are `ir/fixedbugs-381-a-branching-pushed-value-is-push-owned`, red on the base, and three `run/fixedbugs-381-*` programs over the five operators, two in one value, a nested call, a match arm, a loop, a generic and the shapes that must keep copying; defect 387's repair moved the IR case and one run case, their receiver load now retained.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
