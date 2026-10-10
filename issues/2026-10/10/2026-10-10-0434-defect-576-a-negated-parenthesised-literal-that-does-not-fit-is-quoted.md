---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: 79e557b5fde0d177c05576676e3b5192e2807d45
github: none
---

- [x] **576 — a negated parenthesised literal that does not fit is quoted without its closing parenthesis** | `x: i8 @ -(-128)` is refused *`` `-(-128` `` does not fit an `i8`*, the span's caret one character short under `-(-128)`; reproduced by the coordinator at 04:33 with the trunk's compiler (`.claude/worktrees/scratch-b15/r576/neg.hero`); the base before batch 18 the same | the span of a unary minus over a parenthesised operand, `selfhost/check/` and the parser's span of `-(...)` · **class: adjacent**

    **Origin:** found by lane b18-infer beside defect 564 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-infer/notes.txt`, ignored by git), filed by the coordinator at 04:34 on 2026-10-10.

    **Class: adjacent**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a true message whose quote and span are less exact than they could be.

    Repaired at `79e557b5`, 2026-10-10 (lane b18-infer), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Not a minus's alone: a parenthesised expression is its contents, and every operator built its span from its operands', so a `!`, a binary operator at either edge, and a field, an index, a call, a `?` or a `::` after a parenthesised base lost a parenthesis too; each now spans from the first token its form read to the last. `-(-128)` and `-((200))` are quoted whole, `(1 + 2) * 3` and `(p).x` underlined from the `(`. Cases `check/` and `full/fixedbugs-576-…`, three goldens told one column earlier and corrected beneath; `check` 653, `full` 34, `fixes` 937, `annotations` 945, `emission` 1096 with nothing moved, the compiler's 1,547 tests, 0 failed; no code moves in the census and the IR of the 1,643 programs both compilers accept is identical.

## The repair

Repaired at `79e557b5`, 2026-10-10 (lane b18-infer), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Not a minus's alone: a parenthesised expression is its contents, and every operator built its span from its operands', so a `!`, a binary operator at either edge, and a field, an index, a call, a `?` or a `::` after a parenthesised base lost a parenthesis too; each now spans from the first token its form read to the last. `-(-128)` and `-((200))` are quoted whole, `(1 + 2) * 3` and `(p).x` underlined from the `(`. Cases `check/` and `full/fixedbugs-576-…`, three goldens told one column earlier and corrected beneath; `check` 653, `full` 34, `fixes` 937, `annotations` 945, `emission` 1096 with nothing moved, the compiler's 1,547 tests, 0 failed; no code moves in the census and the IR of the 1,643 programs both compilers accept is identical.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
