- [ ] **202 — a body written at its head's margin is told twice at its first line, and once more for each line after** | `function main()` over `print(1)` at column 0: `missing_body` and `expected_declaration`, both at 2:1, for the one indentation left out; `function f(x: i64) -> i64` over `y = x + 1` and `return y` at column 0: a third, `expected_declaration` at the second line; the same under a `test` and a `constant` | `selfhost/parse/top_level.hero:82` (`expected_declaration`, at the line the missing body was just told at) · `selfhost/parse/opening.hero:274` (`absent`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside panel 187's R5, 2026-10-03, on the head's compiler and on `eddb0a7c` (`scratchpad/lane-rec187/pass1/v5/v24*.hero` and `k-v24.txt`, 2026-10-03). No golden pins the two at one place (every `check` case's `.expected` read for a `missing_body` and an `expected_declaration` at one line and column), and the recovery instrument plants no body dedented to its head (its operators in `scratchpad/instrument/tool/ops.py`, read 2026-10-03), so no count has seen it.

    **Why it is a defect.** One mistake, the body's indentation, told twice at one place (design.md §4.17).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, class (a); both messages true.
