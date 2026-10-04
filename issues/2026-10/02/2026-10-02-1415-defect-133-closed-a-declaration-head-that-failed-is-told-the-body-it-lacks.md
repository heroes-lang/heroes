---
kind: defect
area: compiler
milestone: none
filed: 2026-09-29
commit: 435201935c2adf4afdd5610b10335b213e8c1239
github: none
---

# Defect 133 closed: a declaration head that failed, or ends in a refused word, is told the body it lacks in the first run

- [x] **133 — after a declaration head the lexer refuses as a foreign word with a certain swap, the declaration's own mistakes are reported only once the swap is applied** | `const MAX = 5` costs `reserved_word` alone, and `check --apply` writes `constant MAX = 5`, which then costs `expected_constant_type`; `const MAX: i64 = 5` then costs `missing_body`, `fn main() {` then `missing_body`, `struct Point {` then `empty_record`: two turns for one habit | `selfhost/scan.hero` (the foreign word emitted as an `error` token) · `selfhost/keywords.hero` (`foreign_word`) · `selfhost/parse/decl.hero` · `cursor.recover_to_next_decl` · **closed 2026-10-02**

    **Origin:** lane 129's agent, 2026-09-29, reading `check/reserved-words`'s
    applied text while building the `fixes` suite's judgement of cases with no
    `.fixed` (`85066233`, reported and not changed); widened by the
    coordinator on the trunk's compiler at `ee1b648b` to every declaration
    keyword a swap names (`const.hero`, `const_typed.hero`, `fn_brace.hero`,
    `struct_brace.hero` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/d133/`, 2026-09-29).
    Beside it and not of it: every foreign word the LEXER refuses is reported
    in the first run (`fn add(a: int, b: int) -> int` costs four, clean after
    `--apply`), and `let`, whose repair is guidance, has no certain swap.
    The cause is read and unrun: the lexer emits the word as an `error` token
    (`state.error_token`), and at a top-level line's head the parser's
    recovery drops the whole declaration, its body included.

    **Why it is a defect.** The same as 130's: a mistake present and
    unreported is a silence where a message is owed, and design.md §4.17 asks
    that a model fix a program in one turn. `const MAX = 5`, `fn main() {`
    and `struct Point {` are the habits of the three languages a model has
    read most.

    **Widened 2026-09-29 by lane 133's agent, after its repair landed at
    `6f3d31d6`**: (i) a certain swap that is wrong where it stands, which
    `--apply` writes into a program still refused: `function fn(` to
    `function function(`, `record struct`, `const x = 5` in a body to
    `constant x = 5`, `fn` or `struct` nested in a body, `enum` in a group, a
    same-line `record P fn`, and `include "stdio.h"` to `use`, whose message
    then points at `stdio.h.hero` (defect 015's class of the byte after a
    word, one position further); (ii) `elif` is not read as `else if` (it
    costs `reserved_word`, `unexpected_block` and `expected_expression`) and
    `switch` not as `match`; (iii) `fn(i64) -> i64` without its parentheses
    is silent until the swap; reproducers `shapes/`, `more/`, `a/*swap*`,
    `final/elif_ok.hero`, `shapes/elif_brace.hero`,
    `shapes/switch_expr.hero`, `more/fntype_noparens.hero` in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/lane-133/`, 2026-09-29.

    **Batch gate 2026-09-30 00:07** (`53a5e0a9`, lanes X3 and X4 beside the trunk's
    129 to 133, X1, X2 and Y): the full net, every suite at 0 failed. The shapes
    still open are the next batch lane's items, found beside the repairs, and
    this item stays open until they are repaired and a batch gate reads them.

    **2026-10-02, lane recovery-b7, a declaration whose head failed, or holds
    a word the lexer refused, is told the body or the members it lacks in the
    same run** (the audit's 133-a-fswap2, 133-m-const_alone, 133-m-fn_alone;
    beside them `$`, `0X` or `let` after a head, `record P fn`, `extern "m.h"
    fn`, `if true fn` over its margin, and `function`, `function f`,
    `constant MAX`, `test`, `record`, `variant` that failed over nothing):
    repaired at `97bbdc68`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **The repair**, lane recovery-b7, `97bbdc68`. **The class**, wider than
    the audit's three rows (`fswap2`, `const_alone`, `fn_alone`): a head whose
    line ends in a token the lexer refused (`$`, `0X`, `let`, `record P fn`,
    `extern "m.h" fn`, `if true fn`), and native heads that fail with nothing
    below (`function`, `function f`, `constant MAX`, `test`, `record`,
    `variant`), hid the missing body or members. Now a failed declaration
    head, a head whose line ends in a refused token, and a refused word alone
    are told the body or members they lack, in the first run
    (`tails.owed_body`, `members_below.here_or_told`, `decl.bodiless_word`,
    `refused_heads.declares`); not where the value is already on the line,
    nor where the head's recovery took braces across lines; an extern group's
    missing block is told at the group's head. **Cases**:
    `check/fixedbugs-133-a-declaration-head-that-failed-over-no-body` and
    `-a-refused-word-heading-a-declaration-over-no-body`.

    **The gates.** Each repair by its cases and the compiler's own tests in
    its own lane, and the census against the compiler before it. **The
    round's one gate**, the first under the author's instruction of
    2026-10-02 (one gate per round, on this Mac): lanes ffi-macro, ci-probe,
    arm and recovery-b7 merged into `lane-round1002` (`c0f0f627`
    fast-forwarded, then `e8e4eaf6`, `76777908`, `ccba1df5`, one conflict, in
    item 135's body, every line of both lanes kept), closed at `e2d59fdb`:
    the seed regenerated once, 36,141,732 bytes, SHA-256 beginning
    `e6d520df7426008b`, its fixpoint by `cmp`; the compiler's own tests 1,018
    and the net's own 187, all passed; the full net, 25 suites three at a time
    and `cache` alone, 4,449 passed and 0 failed, one floor raised as the
    suite asked (`annotations`, 2,148 to 2,779), nothing blessed; the census
    of `check --brief` over 1,720 files, two exits moved, lane arm's two
    predicted ones, every moved output attributed to one lane by re-running
    the census with each lane's own compiler; the `--emit-c` census, 0 bytes
    of C moved; the site's build green. The trunk took the round at
    `415c0a14` on 2026-10-02, with only panel 186's sitting between, and
    `records` read 24 passed, 0 failed after it. **Owed before the push**:
    Linux arm64 and the Windows box, by the same instruction.
