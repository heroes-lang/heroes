# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 47**

- [ ] **177 — a `match` whose arms fall inside a bracket left open has each arm told again after the bracket's own message** | `return match scores[name` over `.ok v  => v.to_str()` and `.err e => e.code`: `unclosed_bracket` at the `[`, then `line_end_before_continuation` at each arm, three messages for one missing `]`; `x = match (n` over `.ok v => 1` the same; `y = match n` below `x = [n, 1` gets `expected_end_of_line` at each arm's `=>` | the reach of a bracket left open (panel 183's R1 and R2) over a `match`'s arms · `selfhost/parse/line_end.hero:242` · **class: adjacent**

    **Origin:** lane 135c's report and lane recovery-b4's (*one extra message per arm*), queued under recovery-b5, 2026-10-02 (`scratchpad/lane-135c/shapes/P6/`, 2026-10-02); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/open-bracket-arms/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The certain fix lane recovery-b4 saw inside a `[` left open is a guess today, and `check --apply` leaves the text as it is.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-arms-inside-a-bracket-left-open-are-told-again.hero`, a known cost under the sitting's R1**, not repaired inline: where the reach of a bracket left open ends over a match's arms is panel 183's R1 and R2, ratified, which the lexer and `parse/unclosed` apply. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake; no wrong value, no false message, no certain fix. The recovery cluster's, beside 130 and 131.

- [ ] **178 — a separator habit on every line of a block is told once per line: a `,` after each statement or each arm, a `;` after each field** | `x = 1,`, `y = 2,`, `print(x + y),` in a function: three `expected_end_of_line`; arms `0 => 1,`, `1 => 2,`, `_ => 3,`: three; `record P` over `x: i64;`, `y: i64;`, `z: i64;`: three `unexpected_character`; a `,` after every member of a declaration is one message since `68e46a13` | `selfhost/parse/member_lines.hero` (the members' rule, `68e46a13`) · the line end of a statement and of an arm · the lexer's `;` · **class: adjacent**

    **Origin:** lane recovery-b4's report (*the per-line `,` and `;` habits, one message per run with a certain deletion*), queued under recovery-b5, 2026-10-02; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/habit-every-line/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-separator-habit-on-every-line-is-told-once-a-line.hero`, a known cost under the sitting's R1**, not repaired inline: one message for a run of lines is a reading of its own, as ruling 5's is for an indentation habit, and the `;` is the lexer's (`selfhost/scan.hero:299`). The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **179 — `@return 1` costs two `expected_expression`, one at the `@` and one at `return`** | `function f() -> i64` over `@return 1`: `check` exit 1, *expected an expression, found `@`* at 2:5 and *expected an expression, found `return`* at 2:6, neither naming the sigil | `selfhost/parse/at_prefix.hero` (a sigil before a name is one message since `4441148b`; before a keyword it is not) · **class: adjacent**

    **Origin:** lane recovery-b4's report (*`@return 1`: two messages*), queued under recovery-b5, 2026-10-02; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/at-return/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-sigil-before-return-is-told-twice.hero`, a known cost under the sitting's R1**, not repaired inline: the statement's start is `selfhost/grammar_expr.hero`'s, at its `DECIDED` ceiling of 1085 with no line of room, so a repair first moves code out of that knot. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **180 — a range written `1..2` is told twice as a field access** | `x = 1..2`: `expected_field_name` at 3:11, *found `.`*, and again at 3:12, *found a number (`2`)*; `x = 0x1..5` the same at 3:13 and 3:14 | `selfhost/grammar_expr.hero:343` · **class: adjacent**

    **Origin:** lane arm's first pass for defect 154, 2026-10-02 (`scratchpad/lane-arm/pass1/n154/e20.hero`, 2026-10-02), queued as *a range habit, recovery's file*; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/range-dots-twice/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The recovery instrument counts its `range-dots` operator at 12 EXTRA.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-range-written-with-two-dots-is-told-twice.hero`, a known cost under the sitting's R1**, not repaired inline: its site is `selfhost/grammar_expr.hero`'s `after_dot`, at its `DECIDED` ceiling of 1085 with no line of room, and the range's right operand can be read only inside that knot, so no module of `parse/` can carry the repair; a `for i in 0..10` head costs the same two. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **181 — a decimal number with a second point, `1.5.2`, is told as a field access** | `x = 1.5.2`: `expected_field_name` at 3:13, *expected a field or function name after `.`, found a number (`2`)* | `selfhost/grammar_expr.hero:343` · `selfhost/number.hero` · defect 154's closed record (*Left, queued*) · **class: adjacent**

    **Origin:** lane arm's first pass for defect 154, 2026-10-02 (`scratchpad/lane-arm/pass1/n154/e08.hero`, 2026-10-02), named *Left, queued* in `docs/records/done/2026-10-02-1415-defect-154-closed-a-based-literal-with-a-fraction-is-told-as-the-number-it-is.md` and on no open list; reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/float-second-point/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). Lane arm: no existing code covers it, so a new one is a sitting's.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-a-number-with-a-second-point-is-told-as-a-field.hero`, a known cost under the sitting's R1**, not repaired inline: its site is `selfhost/grammar_expr.hero`'s `after_dot`, beside 180's, at its `DECIDED` ceiling with no line of room. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be. The recovery cluster's.

- [ ] **182 — an operator alone on a line deeper than a `match`'s arms costs `continuation_outside_brackets` and `unexpected_block`** | `k = match n` over `1 => "one"`, then a line holding only `+` (or `-`) one level deeper, then `_ => "many"`: two messages on line 5; the `certain` deletion of the operator, applied, checks clean | `selfhost/open_line.hero` · `selfhost/sign_above.hero` (the deletion, defect 165's `8cb4ba6c`) · the orphan block's `unexpected_block` · **class: adjacent**

    **Origin:** lane h158's pass for defects 165 and 166, 2026-10-02 (`scratchpad/lane-h158/d166/q1_plus_deeper.hero`, `q2_minus_deeper_wild.hero`, 2026-10-02); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/operator-deeper-line/`), where the one fix was a guess, and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), where it is the `certain` deletion 165's repair brought; the second message stands.

    **2026-10-03, lane rec187, panel 187's R5: measured as filed on the lane's compiler at `97008231` (V1, V5 and R6 landed), its reproducers reading as on the head's; pinned by `tests/golden/check/panel-187-an-operator-alone-deeper-than-the-arms-costs-two.hero`, a known cost under the sitting's R1**, not repaired inline: the deeper margin is laid out by the lexer before `selfhost/sign_above.hero` leaves the operator out of the stream, in `selfhost/open_line.hero`, two lines under its ceiling, and only the lexer's indent stack can take the block back. The item stays open.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake. The recovery cluster's.

- [ ] **183 — a `.pc` whose `prefix` holds an unescaped space is refused naming `space/include` as the flag, and not the package file's line that splits it** | `prefix=/opt/with space`, `Cflags: -I${prefix}/include`: `pkg-config --cflags s7` prints `-I/opt/with space/include`; `build` exit 1, `ffi_package`, *the package `s7` answered with `space/include`, which this compiler does not pass on* | `selfhost/cli/shell_split.hero` (the word splitter, defect 162's `b37bfce1`) before `filter_words` (`selfhost/cli/libraries.hero:83`) · **class: adjacent**

    **Origin:** lane h158 beside defect 162, 2026-10-02 (`scratchpad/lane-h158/d162/pc/s7.pc`, 2026-10-02, *true, could say more*); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/pc-prefix-space/`, run with `PKG_CONFIG_PATH` naming that folder) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), where only the note's list of accepted flags is worded otherwise.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

- [ ] **184 — `check` takes time quadratic in the `for` heads it refuses: 3,000 `for n > 0` lines cost 16 s** | a function of N loops `for n > 0` over `n @ n - 1`, `check --brief`: 0.44 s of user time at 500, 1.74 at 1,000, 3.87 at 1,500, 15.92 at 3,000, `real` within 0.16 s of `user`; 3,000 `while n > 0,` cost 0.15 s and 3,000 `n @ 0X1` 0.06 s | `selfhost/grammar_expr.hero:899` (`for_stmt`) · `selfhost/parse/loop_habit.hero` (`refuse`), the cause unrun · **class: adjacent**

    **Origin:** the coordinator's file-queue agent, 2026-10-02, measuring lane arm's item on field-place pushes, on `62d65e48` (2026-10-02, `scratchpad/file-queue/for-habit-quadratic/`), the machine at load 2 to 5; re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), the times above being that run's, on a busy machine and in the same order as the first. Two candidates were measured out over the same 3,000: `loop_habit.hero:62`'s append made in place, and `for_stmt`'s trial parse removed.

    **Why it is a defect.** Defect 146's class: a file of N mistakes costs N² work.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): real, found beside the work, no wrong value and no crash.

- [ ] **185 — the emitted C's line restores after a fixed-array field's assertion name a line one too low per such field** | `heroes build tests/golden/run/ffi-a-char-field-becomes-text.hero --emit-c`: line 20 is `#line 19 "ffiacharfieldbecomestext.c"`, so line 21 is reported as 19, and every later restore is 2 short; over `tests/emission`, 261 of 36,781 restores in 19 files are 1 to 10 short, each file's shortfall equal to its number of two-assertion lines | `selfhost/emit/extern_field.hero:158`, `:161` (a `"\n             _Static_assert(` the printer does not count) · `.claude/rules/generated-c.md:27-29` · **class: adjacent**

    **Origin:** lane round1002b at its gate, 2026-10-02 (*not chased*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/line-restore/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a place less exact than it could be in every clang note, sanitizer frame or debugger line past such a field; no value moves.

    **2026-10-03, lane b8-emit, each assertion of an array field is its own counted line, and the `lines` suite asks every restore of the generated file for its own line**: repaired at `1a4bb344`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. All 261 short restores in 19 traces were the 40 continuation lines above them, none from another writer; a C-boundary file (`extern*`), so it closes after the push's platform legs.

- [ ] **187 — four parser diagnostics are still appended through a field place, against defect 146's rule and `cursor.hero`'s own comment** | `git grep -n 'c.diagnostics @ c.diagnostics.push' -- selfhost/parse/` prints `loop_habit.hero:62`, `line_end.hero:251`, `type.hero:257`, `type.hero:320`, while `selfhost/cursor.hero:272` says *Every parser module appends through this* | the four lines · `cursor.push_diagnostic` (`selfhost/cursor.hero:273`) · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/field-place-push/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), which finds three more modules appending the same way, their cost unmeasured. No cost is measured for any: at `loop_habit.hero:62` the append is not what makes defect 184 slow.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form nobody needs to be right.

- [ ] **188 — two texts still state the arm rule panel 185's R3 replaced** | `selfhost/check/lending.hero:108-109` quotes the spec as *An arm that does nothing is a block holding `_ = 0`*, where the spec now reads *An arm that does nothing holds `_ = 0`*; `tests/golden/check/fixedbugs-135-a-discard-that-is-the-line-s-one-reading.hero:8` reasons *`_ = ` on the arm's own line is `declaration_in_arm`*, false since R3 | the two lines · **class: improvement**

    **Origin:** lane arm's report, 2026-10-02; read by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/stale-arm-texts/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). design.md §4.7 owes nothing: R3 brought the spec to it.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): no program moves.

- [ ] **189 — `typeorder.visit`'s walk without recursion has no witness** | the record chains the run cases emit are 1,000 deep (`run/fixedbugs-140-records-` and `-extern-records-a-thousand-deep-build`), and at 1,000 and 5,000 lane ci140's mutant, `visit` restored to its recursion, emits the trunk's bytes (666,776 and 3,358,806, `cmp` equal) | `selfhost/emit/typeorder.hero:130` and its one test at `:206` · **class: improvement**

    **Origin:** lane ci140's report, 2026-10-02 (`scratchpad/lane-ci140/mut/emit-typeorder/heroes-mut`, 2026-10-02, *the mutant that no case catches*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/typeorder-witness/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). At 20,000 deep the mutant reached clang, which died (defect 170), so a unit test is the witness the lane names.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): coverage.

- [ ] **190 — `records/lists` reads an item's class token and not the rest of its first line, so an item that lost its title and its *where* field reads as whole** | item 130's first line as `8349d264` left it, with no `**` closing its title and no ` | ` at all, before its class token: `records` 24 passed, 0 failed, after that commit and after every one until the item was restored at `50644159` | `tests/harness/suite_records.hero` (`list_offences`; the class rule, about `:4448-4610`) · **class: improvement**

    **Origin:** the coordinator, 2026-10-02, on the damage panel 187's completeness critic found (`docs/records/log/2026-10-02-2141-item-130-cut-in-8349d264-and-restored-what-cut-it-is-unknown-the-commit-did-not-read-its-diff.md`). It would have caught the cut in the item's line, not the one in its body.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): hardening of an instrument; nothing a program does moves.

- [ ] **193 — an arm whose pattern failed hides the mistakes in its body: on one line in both arms, and, joined below the failed line, under `--permissive` since panel 187's V1** | `k = match n` over `x | 2 => f(1 +)`: `expected_pattern` at the `x` and the `1 +` untold, in both arms, on the head's compiler and on `29425af6`; `x |` over `2 => f(1 +)`: the normal arm tells the `1 +` from the lines apart, and `check --permissive` told it, `expected_expression`, until `29425af6` and not since; the same over `1 | +`, `x ==`, `1 -> 2 |` and an arm one level deeper | `selfhost/grammar_expr.hero` (`arms_of`'s `.err` branch: the failed arm's line goes with `cursor.drop_rest_of_line`, its body with it) · panel 187's R4 · **class: adjacent**

    **Origin:** panel 187's compiler engineer, 2026-10-02, on its V1 probes `h01`, `h05`, `h14`, `h16`, `h17` and their one-line twins `h02`, `h06` (`scratchpad/187-compiler-engineer-work/probes2/`, 2026-10-02); filed by lane rec187 at the sitting's R4, reproduced on the head's compiler and on `29425af6` (2026-10-03, `scratchpad/lane-rec187/pass1/k-166-control.txt` and `v1-vs-k.txt`). No golden form runs `--permissive` (the sitting's R3), so the control arm's half cannot be pinned; the one-line half can.

    **Why it is a defect.** A mistake told only once another is fixed: design.md §4.17's measure counts an exchange more.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only once another is fixed; no false message, no wrong certain fix.

- [ ] **194 — C's three-clause loop header costs three messages: the lexer tells each `;` and the loop habit the `for (`** | `for (i = 0; i < 3; i++)` over its body: `unexpected_character` at each `;` and `for_missing_in` at the `(`, three messages for one habit; `for (;;)` the same; a `{` after the header adds `missing_body`, ruling 4's | `selfhost/scan.hero:299` (the lexer's `;`) · `selfhost/parse/loop_habit.hero:58` (`for_missing_in`) · pinned by `tests/golden/check/panel-187-a-c-style-for-header-is-told-by-the-lexer-and-by-the-loop.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A1 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-33a, 131-54a and 131-54b and the recovery instrument's `c-for`, 52 of its 439 EXTRA; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

- [ ] **195 — a line a recovery hands on to the statement reader is told a second time, by the expression's `primary`** | an arm's `0` over a `=> 5` two dedents out: `expected_arm_arrow` at the `0` and `expected_expression` at the `=>`; `use geom` over an indented `function g()`: `expected_declaration` at the block and `expected_expression` at the `function` | `selfhost/grammar_expr.hero:429` (`primary`'s message), after `selfhost/parse/broken_arm.hero:55` or `selfhost/parse/top_level.hero:82` · pinned by `tests/golden/check/panel-187-a-line-a-recovery-hands-on-is-told-again-by-the-expression.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A2 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-55a and 131-55b and lane recovery-b8's shape `g1/a11`; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

- [ ] **196 — the colon habit is told once a line, and the next head's `:` on the same line is named a missing body** | `if n > 0: if n > 1: print(1)`: `trailing_colon` at the first `:`, whose `certain` fix rewrites the whole line and checks clean, then `missing_body` at the second, *found `:`*; three heads on one line cost the same two | `selfhost/parse/colon_habit.hero:98` · `selfhost/parse/opening.hero:274` (`absent`) · pinned by `tests/golden/check/panel-187-the-colon-habit-is-told-once-a-line.hero` and its `.fixed` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A3 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-56a and 131-56b; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

- [ ] **197 — one `)` left out of a function type costs four messages: the lexer names two openers, and the type reader asks for the `)` and the `->` it was owed** | `function f(g: (function(i64 -> i64)` over its body: `unclosed_bracket` at each `(`, *never closed* at the file's end and *still open at line N* where a declaration below ends the reach, then `expected_function_type_params_close` at the `->` and `expected_function_type_arrow` at the line's end | `selfhost/closers.hero` (`never_closed` at `:151`, `still_open` at `:169`) · `selfhost/parse/type.hero:307` and `:229` · pinned by `tests/golden/check/panel-187-a-closer-the-lexer-pairs-with-another-opener.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause A4 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's row 131-41a; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03). The row's own file ends below the head, so it reads *never closed* (`scratchpad/audit-130-133/cases/131-41a/`, 2026-09-30, re-run by the lane on `29425af6`); the pin has a declaration below, so *still open*.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a second message for one mistake, class (a).

- [ ] **198 — a head whose line goes on past it is told by one message and the rest of its line dropped, so a stray closer on it is told only once the first is repaired** | `variant T )` and `record Point )` with nothing below: `empty_variant` and `empty_record`, the `)` untold; `function f(): i64 )` over its body: `expected_end_of_line` at the `:`, the `)` untold until `->` replaces the `:`; with no body, `missing_body` (found `:`) and the `)` untold | `selfhost/parse/members_below.hero:87` (`skip_line` after a record's or a variant's head) · `selfhost/parse/opening.hero:192` (`drop_rest_of_line` after a function's) · pinned by `tests/golden/check/panel-187-a-heads-line-that-goes-on-is-told-once.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B1 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's rows 131-16a, 131-16b, 131-53a and 131-53b; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

- [ ] **199 — a head that refused something drops the rest of its line as debris, a stray closer in it with it** | `if f(1 +) )` over its body: `expected_expression` at the call's `)`, and the stray `)` after it told only once the operand is written | `selfhost/parse/opening.hero:147` (`drop_rest_of_line` after a failed head) · pinned by `tests/golden/check/panel-187-a-failed-heads-rest-is-dropped-as-debris.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B2 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on lane recovery-b8's shape `g4/ti`, one of the six beside item 130; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

- [ ] **200 — a `function` among a variant's cases is told once and dropped with its block, so a mistake inside it waits until it moves out** | `variant V` over `red` and `function f()` over `print(1 +)`: `expected_case` at the `function`, and the `1 +` untold; among a record's fields the same function is read as the function it is since `26358f9c` | `selfhost/parse/member_lines.hero:205` (`skip_line`) and `:202` (`balanced_block`) · pinned by `tests/golden/check/panel-187-a-function-among-a-variants-cases-is-dropped.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B3 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on lane recovery-b8's shape `g2/r12`, one of the six beside item 130; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

- [ ] **201 — in a braced body, the rest of the line past an inner closing brace is not read** | `function main() {` over `do {`, a body and `} while (1 == 1)`: `missing_body` at the `{`, `expected_end_of_line` at the `do {` and in the body, and `while (1 == 1)` untold until the braces are gone | `selfhost/parse/braced_lines.hero:154` (`brace_habit.pass`, which passes the function's braces after its lines are read, the inner `}`'s line with them) · pinned by `tests/golden/check/panel-187-a-line-past-an-inner-closing-brace-is-dropped.hero` · **class: adjacent**

    **Origin:** panel 187's compiler engineer, its § 1's cause B4 (`docs/panel/187-reports/compiler-engineer.md`, 2026-10-02), on the audit's row 131-32, where `heroes lex --dump-tokens` shows the lexer hands the parser every token of that line; filed by lane rec187 under the sitting's R1 with its pin, which reads byte for byte the same on the head's compiler and on `29425af6` (2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery, panel 187's R1): a mistake told only once another is fixed, class (b).

- [ ] **202 — a body written at its head's margin is told twice at its first line, and once more for each line after** | `function main()` over `print(1)` at column 0: `missing_body` and `expected_declaration`, both at 2:1, for the one indentation left out; `function f(x: i64) -> i64` over `y = x + 1` and `return y` at column 0: a third, `expected_declaration` at the second line; the same under a `test` and a `constant` | `selfhost/parse/top_level.hero:82` (`expected_declaration`, at the line the missing body was just told at) · `selfhost/parse/opening.hero:274` (`absent`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside panel 187's R5, 2026-10-03, on the head's compiler and on `eddb0a7c` (`scratchpad/lane-rec187/pass1/v5/v24*.hero` and `k-v24.txt`, 2026-10-03). No golden pins the two at one place (every `check` case's `.expected` read for a `missing_body` and an `expected_declaration` at one line and column), and the recovery instrument plants no body dedented to its head (its operators in `scratchpad/instrument/tool/ops.py`, read 2026-10-03), so no count has seen it.

    **Why it is a defect.** One mistake, the body's indentation, told twice at one place (design.md §4.17).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, class (a); both messages true.

- [ ] **204 — inside a list whose `[` the lexer paired with a closer of another kind further down, a binding is read as an element and told without the `[`, and the stray closer waits for the `]`** | `x = [1, 2` over `print(x)` over `y = 3 )`: one message, `expected_separator` at the `=` two lines below the `[`, *found `=`*, naming no `[`; with the `]` written, the `)` is told, `expected_end_of_line`, on a second run | `selfhost/parse/unclosed.hero` (panel 183's R1, a binding below a `[` ends its reach only where the lexer named the `[` never closed) · `selfhost/closers.hero` (the closer of another kind paired with the `[`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside defect 203, 2026-10-03, on the head's compiler and on the lane's (`scratchpad/lane-rec187/pass1/r6/r16_closer_two_below.hero` and `r16b_bracket_written.hero`, 2026-10-03). Another cause than 203's: the found token is no closer, and the reach rule is not asked.

    **Why it is a defect.** The `]` left out is told nowhere and the stray `)` only on a second run (design.md §4.17's measure).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a mistake told only once another is fixed, class (b).

- [ ] **205 — a closer of another kind where a map entry's `:` goes is told without the `{` or its closer** | `m = {1: 2` over `print(x) )`: `expected_map_entry_colon` at the `)`, *expected `:` between a map's key and its value, found `)`*, naming no `{` and no `}`; `{1: 2` over `y]` the same | `selfhost/grammar_expr.hero:567` (`map_literal`'s `line_end.expect_after` for the `:`) · defect 203's message, the separator's, which names them (`selfhost/parse/list_line.hero`, `another_kind`) · **class: adjacent**

    **Origin:** lane rec187's first pass beside defect 203, 2026-10-03, on the head's compiler and on the lane's (`scratchpad/lane-rec187/pass1/r6/r04_map_paren.hero` and `r12_map_bracket.hero`, 2026-10-03). The same reading as 203's at another site: the line is read as the map's next key, and the closer stands where its `:` goes.

    **Why it is a defect.** As 203: one reading served, the other's repair a run more (design.md §4.17's measure).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

- [ ] **208 — a dead `break` after a `return` inside `while true` is told twice, `unreachable_statement` and `missing_return`** | `while true` over `if m > 3`, `return m`, `break`, in a `function f(n: i64) -> i64`: `unreachable_statement` at the `break` and `missing_return` on `f`, both gone once the `break` is deleted | `selfhost/check/flow.hero` (panel 184's R4: a `while true` with a `break` of its own does not end a path, read by syntax) · **class: adjacent**

    **Origin:** lane flow4's report, 2026-10-03 (`scratchpad/lane-flow4/w/w16-dead-break-under-return.hero`, 2026-10-03); measured by the coordinator on the trunk at `e5893696` (`scratchpad/file-r5/`, 2026-10-03).

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

- [ ] **209 — a `layout` run filtered to a name that matches no module reads 1 passed** | `./heroes run tests/harness/main.hero -- ./heroes layout <a name no module has>`: `1 passed, 0 failed`, because the harness's guard against an empty selection counts cases and the suite's file-wide checks are always one case | `tests/harness/suite_layout.hero` · the harness's selection guard · **class: improvement**

    **Origin:** the coordinator's agent finishing defect 167 in lane cb4, 2026-10-03, which then checked by hand that each of its six filters matched one module (`scratchpad/lane-cb4/progress.md`, 2026-10-03).

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument that can read green over nothing; no program moves.

- [ ] **210 — the recovery instrument lives in the scratchpad as a Python tool, where a reboot can lose it; its home is `heroes mutate`'s recovery arm, with its plan pinned** | `<scratchpad>/instrument/tool/` (3,618 code lines of Python, the compiler-engineer's count); panel 187's R2 reads it at every recovery round's gate, and the fourth round's reading had to subtract a corpus program the language changed under it by hand | `selfhost/cli/mutate.hero` and `selfhost/mutate/` · panel 187's R2 and R3 · **class: improvement**

    **Origin:** panel 187's R10 (filed beside the sitting), 2026-10-03, and the fourth round's differential reading (`scratchpad/inst-187/r4-differential.txt`, 2026-10-03): the port owes a frozen plan in the tree and the subtraction of an unmutated program's own messages.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument's home; no program moves.

- [ ] **211 — no golden form runs the control arm (`--permissive`), so a control-arm row cannot be pinned** | `tests/harness/suite_golden.hero` has no such word (the compiler-engineer's search); 131-22's open half and V1's five control-arm hides (defect 193) have no pin | `tests/harness/suite_golden.hero` · panel 187's R3 · **class: improvement**

    **Origin:** panel 187's R3 and R10, 2026-10-03.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): coverage.

- [ ] **212 — class (b), a mistake told only once another is fixed, has no measurement at the project's sizes** | the blind readings' two class (b) programs are 3 and 5 lines; panel 183's 20 of 20 were 31 to 105; the spec-warden's named run (rows 131-32 and `g2/r12`, ten sessions, about 4 to 5 USD) and the 300-line form panel 183's critic left owed are unrun | panel 187's R7 and R10 · `docs/panel/187-reports/spec-warden.md` § 8 · **class: improvement**

    **Origin:** panel 187's R10, 2026-10-03; deferred by the author's answer *7b* (*later, after the ratification*): a paid run the author funds.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a measurement nobody needs to be right today; it decides whether D1's stronger promise enters.

- [ ] **213 — the Linux image carries no SDL3, so defect 151's SDL3 event runs on this Mac alone** | `run/ffi-a-construction-polls-an-sdl3-event` in the arm64 container: *the package sdl3 is not installed on this machine*, skipped; the CI's Linux jobs and the Windows leg skip it too, by their totals | the `heroes-linux-arm64` image and the CI's install list · defect 151's closed record · **class: improvement**

    **Origin:** the coordinator's closings agent, 2026-10-03, and the author's answer *4a*.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): coverage of one platform.

- [ ] **215 — a text grown by `+` in a selfhost module is first seen by the whole `layout` at a gate: the write-time hook does not ask it, and `layout` narrowed to the file cannot** | lane win214's `b8ad7f9e` wrote `beside @ beside + ch` in a loop of `selfhost/cli/compiling.hero`'s test; `.claude/hooks/fmt_check.py` passed it (it asks parse, canonical form, the compiler's check and the line ceiling), and the lane's gate read `layout/concat` red at 11:06, four sites; `tests/harness/suite_layout.hero` asks `appends`, `concat` and `budget` only when `only == ""`, so a narrowed `layout` never asks them | `.claude/hooks/fmt_check.py`, `tests/harness/suite_layout.hero` (`GROWTH_ALLOWED`) · defect 209 · **class: improvement**

    **Origin:** the coordinator, 2026-10-03, at lane win214's gate: one run of 25 suites stopped at 20 to repair it, then run again whole.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument's coverage. § A suite is the last judge asks that what a hook can see on the touched file never wait for a suite, and the growth sites of one file against `GROWTH_ALLOWED`'s entries for that file are such a thing.

- [ ] **216 — a header whose name holds a `>` passes `check` and stops `build` at exit 2 with an internal error and clang's text** | `extern "a>b.h"` over `function seven() -> i32`, the header beside the program: `check` exit 0; `build` exit 2, *internal error: compiling the generated C failed*, clang's `#include <a>b.h>` and *'a' file not found*, then *error: clang refused the generated C*; the same program over `ab.h` builds and prints `7` (the trunk's compiler, `02e507bc`'s seed, 2026-10-03) | `selfhost/parse/group_head.hero` (what a group head refuses of its header's text, `machine_locked_path` its one refusal today) · the `#include` line the emitter writes · **class: blocking**

    **Origin:** lane warn's first pass beside defect 207, 2026-10-03, reported to the coordinator with its reproducer; reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 and an internal error where the author can be told. No angled `#include` can spell a `>`, and panel 036's R2 keeps the angled form and never the quoted one, since a quoted include takes a decoy planted beside the unit (`docs/panel/036-the-ffi-ladder.md`); so the repair is a refusal at `check`, a diagnostic class and so a sitting's (CLAUDE.md § 4) before a lane's.

    **Widened by panel 188**, 2026-10-03 (`docs/panel/188-a-group-heads-string-is-its-value-and-what-its-tool-cannot-carry-is-refused-on-its-line.md` R2, R12): the cause is the group head's string's, not the header's, a string its tool cannot carry as the one name it is, so the empty string, a NUL and a line end in a header, a `link` or a `package`, and `>` in a header, are 216's shapes. Measured on the trunk's compiler beside it: a NUL in a `link` or `package` string aborts the compiler (`build` 134, *an argument contains a NUL byte*), `link ""` hands the linker a bare `-l` that takes the output as an input (`build` 2), and a NUL in a header binds the part before it at exit 0 where that part names a file (`<scratchpad>/repro188/link-nul/`, `link-empty/`). Repaired by the sitting's stage E, which lands in batch 8's FFI lane; closes only after that batch's platform legs (a C-boundary defect).

- [ ] **219 — clang's debug information dies on a type chain between 3,000 and 5,000 nested variants on this Mac, by 10,000 in the Linux container** | past panel 184's R6 floor of 2,000: since `6c95f44a` (defect 170) the build says so in the compiler's words at exit 2 and leaves no crash files; clang's own stack raised to 64 MB compiled 10,000 in the lane's measurement, about eight times further, not built | `selfhost/cli/clang_died.hero` (lane depth's, at `6c95f44a`, 2026-10-03), `selfhost/emit/typeorder.hero` · **class: improvement**

    **Origin:** lane depth beside defect 170, 2026-10-03, reported to the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): beyond the floor panel 184's R6 sets, and told truly at exit 2 since defect 170's repair; a reach nobody needs to be right today.

- [ ] **220 — a call to a name nothing binds, close to a built-in, gets a `certain` rename to the built-in, and the program `--apply` writes is refused anew** | `function main()` over `print(sqrt(2.0))`: `check` exit 1, `unknown_name`, *nothing named `sqrt` is in scope, did you mean `sort`?*, `fix (certain): rename to sort`; `check --apply` writes `print(sort(2.0))`, which `check` refuses, `bad_operand`, *`sort` takes `[T]`, found `f64`* (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro220/`) | the `unknown_name` rename's certainty (`selfhost/resolve/`) · `.claude/rules/diagnostics-and-goldens.md` (*a `certain` fix repairs the defect the diagnostic names*) · **class: blocking**

    **Origin:** panel 188's completeness critic, 2026-10-03, in its first pass, beside the blind seat's own program with its `extern` line dropped; reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a `certain` fix that writes a program refused anew; and the name is C's `sqrt`, which a program binds from `math.h`, so the rename points away from the repair.

- [ ] **221 — no reader of a group head's string decodes its escapes, so `extern "a\\b.h"` asks clang for `a\\b.h` and `build` says a header present is missing** | `extern "a\\b.h"` over `function seven() -> i32`, the file `a\b.h` beside the program: `check` 0, `build` 1, `ffi_missing_header`, *`a\\b.h` is not on this machine's include path, clang looked and did not find it*, while the file is there; the emitted unit holds `#include <a\\b.h>`, two backslashes; the same for `\"`, `\n`, `\t`, `\r`, and for `link "a\\b"` with `liba\b.a` there (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/esc-bs/`) | `selfhost/emit/externs.hero` (`unquoted`), `selfhost/parse/module_text.hero` (`unquote`) · spec § 2's five string escapes, § 13's `Extern` production · panel 188 R1 · **class: blocking**

    **Origin:** panel 188's completeness critic, 2026-10-03, in its first pass over the briefs (the escape face of F1, read as clang's, was the compiler's); confirmed by `od -c` of the emitted units (the compiler-engineer) and for `link` (the ffi-pragmatist); reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a false message; filed apart from 216 by the rule (a reader that does not decode, where 216's is a string its tool cannot carry), landed in the same lane, since the refusal reads the decoded value.

- [ ] **222 — a `package` string beginning with `-` reaches `pkg-config` as an option: one is accepted with no package named, another called a package, a third told falsely as not installed** | `extern "ab.h" package "--atleast-pkgconfig-version=0"` over `function seven() -> i32`, `ab.h` a `static inline`: `check` 0, `build` 0, prints `7`, a package clause naming no package; `package "--version"`: *the package `--version` answered with `3.0.7`*; `package "-x"`: *the package `-x` is not installed on this machine*, where `pkg-config` said *unknown option -- x* (the trunk's compiler at `826ddc2f`, this Mac, 2026-10-03, `<scratchpad>/repro188/pkg-option/`, `pkg-opt--/`, `pkg-opt-x/`; the first also in the Linux arm64 image, the seats) | `selfhost/cli/libraries.hero` (the `pkg-config` argv; its comment at 302 to 304 says no `.hero` file can hand it an argument) · panel 188 R7 (b) · **class: blocking**

    **Origin:** panel 188's ffi-pragmatist and compiler-engineer, 2026-10-03, on Q5; reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a wrong program accepted and a false message; filed apart from 216 (the tool's parse of a whole string, not a string its tool cannot carry), landed in the same lane.

- [ ] **223 — a `package` string reaches `pkg-config`'s list and version grammar, which § 13 does not name, and a refused version is told as a package not installed** | `extern "ab.h" package "zlib >= 99"`: `build` 1, *the package `zlib >= 99` is not installed on this machine*, while zlib 1.2.12 is installed (this Mac; 1.3.1 in the Linux arm64 image, the ffi-pragmatist); `package "zlib sqlite3"`: `build` 0 (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/pkg-version/`, `pkg-list/`) | `selfhost/cli/libraries.hero` · spec § 13 (*"A group may name a **package**"*) · panel 188 R7 (c) · **class: blocking**

    **Origin:** panel 188's ffi-pragmatist and compiler-engineer, 2026-10-03, on Q5; the ruling that a package names one package is the sitting's (R7 (c)); reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a false message; filed apart from 216 (`pkg-config`'s grammar, not a string its tool cannot carry), landed in the same lane.

- [ ] **224 — the reader of the linker's *missing library* line cuts a library's name at a quote under ld64 and at whitespace under GNU ld, and `build` exits 2** | `extern "ab.h" link "a'b"`: `build` 2, *internal error: linking failed*, ld64's *library 'a'b' not found*, *clang refused the generated C* (this Mac); `link "a b"`: the same under GNU ld's *cannot find -la b* (Linux arm64, the seats) (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/link-squote/`) | `selfhost/emit/ffi_build.hero` (`missing_library`, `link_head`, `strip_colons`) · panel 188 R7 (a) · **class: blocking**

    **Origin:** panel 188's compiler-engineer and ffi-pragmatist, 2026-10-03, on Q5; reproduced by the coordinator on this Mac the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told; filed apart from 216 (the compiler's reader of the linker's text), landed with 221's repair, which widens it (a TAB in a `link` name once decoded).

- [ ] **225 — `machine_locked` reads a header's first bytes, so a `..` climb to the root names a header by where this machine keeps it, and a `package` naming a `.pc` file is read from the working directory** | `extern "../../(24 times)/<the absolute path without its leading />"` over `function seven() -> i32`: `check` 0, `build` 0, prints `7`, while the same path written absolute is refused `machine_locked_path` (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/climb/`); `package "./seven.pc"`, `"seven.pc"`, `"seven.PC"`, `".pc"` build from the `.pc`'s directory and are *not installed* from another (the ffi-pragmatist and the critic, pkg-config 3.0.7 and pkgconf 1.8.1) | `selfhost/parse/group_head.hero` (`machine_locked`; the leaf of panel 188 R9 once landed) · panel 055 · panel 188 R6, which refuses the `.pc` half · **class: adjacent**

    **Origin:** panel 188's ffi-pragmatist, 2026-10-03, on Q5, the `.pc` spellings widened by the critic's third pass; the climb reproduced by the coordinator the same day, the `.pc` path not.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): real, beside the work, a thesis rule's reach rather than a wrong program; the `.pc` half lands with panel 188's R6, and for the climb the value alone cannot tell a climb to the root from a climb to a sibling directory.

- [ ] **226 — `ffi_package` points at the group's first member, not at the `package` string it is about** | `extern "ab.h" package "zz9nothere"` over `function seven() -> i32`: `build` 1, *the package `zz9nothere` is not installed on this machine*, `at p.hero:2:5`, the member's line, where the string is on line 1 (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/pkg-span/`) | `selfhost/emit/ffi_build.hero` (where `ffi_package` takes its span) · **class: adjacent**

    **Origin:** panel 188's spec-warden, 2026-10-03; reproduced by the coordinator the same day.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

- [ ] **227 — a source file holding a byte that is not UTF-8 is answered `cannot read` at exit 2, where the file was read and the author can be told which line holds the byte** | `function main()` over a comment `# caf` and the byte 0xE9, over `print(1)`: `check` exit 2, *error: cannot read `p.hero`*; the same with the byte inside a string literal (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro-utf8/`) | the reading of a source file into a `str` · `.claude/rules/cli-surface.md` (*exit 1 the input has diagnostics, exit 2 the tool could not run*) · **class: blocking**

    **Origin:** panel 188's ffi-pragmatist, 2026-10-03, beside its sweep (its case `bad-utf8`); reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, and a false message (the file was read).

- [ ] **228 — the emitter finds each instruction's line by walking its source line by character, so a long line costs its length squared** | lane irverify's profile: `writer.at_span` → `source.locate`/`line_col`, 65% of `concat-chain-2000`'s build after defect 218's repair (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/emit/writer.hero`, `selfhost/source.hero` · **class: improvement**

    **Origin:** lane irverify, 2026-10-03, profiling the shapes of panel 184's R6 after defect 218's repair, reported to the coordinator; not yet run by the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): every one of R6's 25 shapes builds at 2,000 on this Mac and in the Linux arm64 container after 218's repair, so this is a cost past the floor, not a program that fails.

    **2026-10-03, lane b8-emit, a `#line` asks the source for its file and line and walks no line**: repaired at `17322f0e`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Counted on an instrumented copy of each compiler's own C, the characters the column walk covers in `build --emit-c`: concat-chain-2000 24,188,468 to 286, index-chain-2000 92,310,458 to 286, for-nested-1000 80,378,910 to 286; seven more emitter sites built `#line` text through `locate` and ask the same lookup.

- [ ] **229 — the emitter's type gate walks a deep type in full at each use** | lane irverify's profile: `emit/gate.check_type` → `check_element`, about 60% of `index-chain-2000`'s build after defect 218's repair (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/emit/gate.hero` · **class: improvement**

    **Origin:** lane irverify, 2026-10-03, the same profiles, reported to the coordinator; not yet run by the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a cost past panel 184's R6 floor, which the shape meets.

- [ ] **230 — the ownership pass asks of every counted load whether a write in its block lets it survive, a walk of the block each time** | lane irverify's profile: `ir/own.run` → `place_store.load_survives_write`, 33 to 35% of `one-return-many-800`, `many-params-800` and `record-literal-2000` (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/ir/own.hero`, `selfhost/ir/place_store.hero` · **class: improvement**

    **Origin:** lane irverify, 2026-10-03, the same profiles, reported to the coordinator; not yet run by the coordinator.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a cost past panel 184's R6 floor, which the shapes meet.

- [ ] **231 — every return sweeps every owned slot, so a function of many returns and many slots grows its IR and its C as their product** | lane irverify: `slots-returns-400` builds in 64.57 s after defect 218's repair, and at 800 clang does not finish in 300 s (`<scratchpad>/lane-irverify/`, 2026-10-03) | `selfhost/ir/` (the sweep at a return) · panel 106's design · **class: adjacent**

    **Origin:** lane irverify, 2026-10-03, beside defect 218, reported to the coordinator; not yet run by the coordinator.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): real, beside the work; an 800-return function is no shape of panel 184's R6, so no floor it sets is broken.

- [ ] **232 — the reader of clang's *file not found* line cuts a header's name at a quote and cannot read a code point clang prints as `<U+XXXX>`, so a missing header named with either stops `build` at exit 2** | `extern "a'b.h"` over `function seven() -> i32`, no header beside it: `check` 0, `build` 2, *internal error: compiling the generated C failed*, clang's *'a'b.h' file not found* not matched back to the group, where `extern "ab.h"` missing is `ffi_missing_header` at exit 1 (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/squote-missing/`); a private-use U+E000, an unassigned U+0378 or a noncharacter U+FFFE in a missing header's name the same, clang printing it `<U+E000>`, on both platforms (the compiler-engineer, stage E) | `selfhost/emit/ffi_build.hero` (`missing_header`, which takes the name up to the first quote and matches it whole) · **class: blocking**

    **Origin:** panel 188's completeness critic, 2026-10-03, in its third pass over stage D (F1 ran `a'b.h` only with its header present), and the compiler-engineer building stage E for the escaped code points; the quote reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told. Panel 188's R4 and R5 refuse `'`, U+2028 and U+2029 at `check`, so once they land a program `check` passes reaches this reader with a private-use, unassigned or noncharacter code point only; R4 is a thesis rule the author may turn, so the reader is repaired on its own, reading clang's line against each declared header as stage C did for the linker's.

*******************************************************************************
