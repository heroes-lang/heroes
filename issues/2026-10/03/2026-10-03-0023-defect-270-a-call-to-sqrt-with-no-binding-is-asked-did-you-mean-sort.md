---
kind: defect
area: golden
milestone: none
filed: 2026-10-03
commit: 2bed5f3146fcd4aa662ef5be60da7ea7cd3f7142
github: none
---

- [x] **270 — a call to `sqrt` with no binding is asked *did you mean `sort`?*, where a note offering the C function would point to the repair** | `print(sqrt(2.0))` with no `extern`: `check` exit 1, `unknown_name` *nothing named `sqrt` is in scope, did you mean `sort`?*, *fix (guess): rename to `sort`* (batch 8's round compiler at `1eb854c3`, 2026-10-04, `tests/golden/check/fixedbugs-220-sqrt-is-not-sort.hero`), a guess since defect 220 and still the only route the message names | `selfhost/rename_fit.hero` and the resolver's did-you-mean (`selfhost/resolve/`) · **class: improvement**

    **Origin:** batch 8's source lane, 2026-10-03 (its report's *Found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a guess that is true and points away from the repair; no program refused or wrong.

    Repaired at `2bed5f31`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A name nothing binds that is a call's callee, and the name after a dot, now carry a note saying how a function of C is bound, *if `sqrt` is a function of C, an `extern` group in this file binds it*, with the group's line and the signature's shape in the name as written (§4.19); it says *if*, so it rests on no knowledge of which names are C's, a name only read is told nothing of C, and the rename stays a guess; no sitting rules the words (design.md §4.17, §4.19 and `docs/panel/` grepped). `full` 20, `check` 585, `permissive` 10, `unsupported` 188, the compiler's own tests 1,359, all passed.

## The repair

Repaired at `2bed5f31`, 2026-10-07 (lane b14-resolve), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A name nothing binds that is a call's callee, and the name after a dot, now carry a note saying how a function of C is bound, *if `sqrt` is a function of C, an `extern` group in this file binds it*, with the group's line and the signature's shape in the name as written (§4.19); it says *if*, so it rests on no knowledge of which names are C's, a name only read is told nothing of C, and the rename stays a guess; no sitting rules the words (design.md §4.17, §4.19 and `docs/panel/` grepped). `full` 20, `check` 585, `permissive` 10, `unsupported` 188, the compiler's own tests 1,359, all passed.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
