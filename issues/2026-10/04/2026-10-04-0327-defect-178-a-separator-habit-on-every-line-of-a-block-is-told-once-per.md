---
kind: defect
area: parse
milestone: none
filed: 2026-10-02
commit: 20f824b37acfc7f000688ab8084ca54aa2737836
github: none
---

- [x] **178 — a separator habit on every line of a block is told once per line: a `,` after each statement or each arm, a `;` after each field** | `x = 1,`, `y = 2,`, `print(x + y),` in a function: three `expected_end_of_line`; arms `0 => 1,`, `1 => 2,`, `_ => 3,`: three; `record P` over `x: i64;`, `y: i64;`, `z: i64;`: three `unexpected_character`; a `,` after every member of a declaration is one message since `68e46a13` | `selfhost/parse/member_lines.hero` (the members' rule, `68e46a13`) · the line end of a statement and of an arm · the lexer's `;` · **class: adjacent**

    **Origin:** lane recovery-b4's report (*the per-line `,` and `;` habits, one message per run with a certain deletion*), queued under recovery-b5, 2026-10-02; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/habit-every-line/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-separator-habit-on-every-line-is-told-once-a-line.hero`, a known cost under the sitting's R1**, not repaired inline: one message for a run of lines is a reading of its own, as ruling 5's is for an indentation habit, and the `;` is the lexer's (`selfhost/scan.hero:299`). The item stays open.

    **2026-10-03, lane b8-recovery: the members' reading of defect 132 carried to statements, arms and the lexer's `;`: the first separator ending its line is told, its deletion certain, and every later one in the declaration, with no other report between, is one more certain deletion of that report** (`selfhost/separator_runs.hero`, called by `parse/statement_end.close` and `scan.punct`; the deletions appended in place, linear; the pin and four cases of defects 130, 131, 132 and `unterminated` moved with a dated line each): repaired at `20f824b3`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

## The repair

Repaired at `20f824b3` (with 2de630dc and 465153ce). Defect 132's reading of a declaration's members carried to statements, arms and the lexer's `;`: the first separator ending its line is told, its deletion certain, and every later one in the declaration with no other report between is one more certain deletion of that report (`selfhost/separator_runs.hero`): 3 messages against the trunk's 10 on one shape, 7 against 12 on another. Its pin, `panel-187-a-separator-habit-on-every-line-is-told-once-a-line`, moved from 9 messages to 3; R2 read 151 single mutants' words move to *no `,` ends a statement's line* and 155 to the `;` sentence. Where the run's later fixes apply is told neither in words nor in `--json`, filed apart as defect 271.

**Closed 2026-10-04** with batch 8 (lanes b8-ffi, b8-source, b8-recovery, b8-emit and b8-defects, merged into one round tree), its closing gate run on `921dc61e` with the seed regenerated: 40,628,892 bytes, SHA-256 beginning `2d55c5ff8309b812`, its fixpoint by `cmp`; the compiler's own tests 1,158, all passed; the net's own tests 210, all passed; the full net, 26 suites, 5,177 passed and 0 failed. The census at the batch's first gate, the trunk's compiler at `7d9f2e8f` against the round's over the tree's tracked files: `check --brief` over 1,954, 30 moved, and `build --emit-c` over the 1,228 holding an `extern`, 44 moved, every one the batch's own (its new refusals, its words, its `#line` before a fixed array field's assertion). Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 12 fewer messages and none more; 15,842 pairs, one told second now carried in the first message's fixes (defect 271).
