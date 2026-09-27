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
**OPEN: 11**

- [ ] **104 — the spec says a NEWLINE inside brackets may fall between any two tokens, and the compiler refuses a break before an operator, a `:` or a `,`** | spec § 0 ends *Inside `(` `[` `{` a NEWLINE never ends a statement: where it separates, a production writes it; elsewhere it may fall between any two tokens*, but the lexer plants a terminator after a line-ending token inside brackets too (design.md §4.15, *terminators, which are inserted unchanged everywhere, brackets included*, which §4.9's one-element-per-line literals rely on), so `x = (1` then `+ 2)` is `expected_group_close`, while `x = (1 +` then `2)` checks clean; a reader who follows the spec writes a program the compiler refuses | `spec/heroes-spec.md:11-12` · `docs/design/design.md:1939-1946` · `selfhost/grammar_expr.hero:161` (`ends_the_expression`)

    **Origin:** panel 179's completeness critic, 2026-09-27, measuring which
    variants of its seats' generators fail to parse; reproduced by the
    coordinator the same night on the trunk's compiler at `fa813325`.
    Searched `docs/`, `spec/` and the records for the sentence and for a
    ruling on a break before an operator: the only other hit is the critic's
    own report. The sentence entered the spec at `e497646a`, 2026-09-12,
    M-stated-grammar step 4, and nothing has tested it since.

    **The reproducers**, each a four- or six-line program, `heroes check` at
    `fa813325`: `x = (1` / `+ 2)` exit 1, `expected_group_close`;
    `print(f(a` / `: 1, b: 2))` exit 1, `expected_args_close`;
    `xs = [1` / `, 2]` exit 1, `expected_expression`; `x = (1 +` / `2)` exit 0.

    **Why it is a defect, and on which side.** The spec is the language as a
    reader gets it, and this sentence tells the reader something the language
    does not do; design.md, the source of truth, and the compiler agree with
    each other, so the false party is the spec's sentence, not the parser.
    CLAUDE.md § 12's *spec beats compiler* does not apply where the spec
    contradicts design.md, which the spec only restates. The repair is the
    sentence saying what is true, at a sitting, priced on the reader's
    instrument; changing the lexer to allow a break before an operator would
    reverse §4.15's deferral of Nim's continuation rule and is a different
    question.

- [ ] **106 — inside a list whose elements a NEWLINE separates, a line that begins with `- b` is a new element, so a subtraction broken before its operator runs with one element too many** | the lexer ends the line after `a` (Go's last-token rule, design.md §4.15) and the literal's `Sep` takes that NEWLINE as a separator, so `xs = [a` / `- b]` is `[a, -b]` at exit 0; `[base * qty` / `- discount]` and `{1: 10` / `- 2: 20}` the same; PEP 8 and Black break BEFORE a binary operator, so a model's natural spelling compiles to a different program, and `heroes fmt` then prints the line as `-b`, erasing the space that showed the intent | `selfhost/grammar_expr.hero` (`separator`, `array_literal`, `map_literal`) · `docs/panel/180-a-line-inside-brackets-breaks-by-how-it-ends-and-a-list-refuses-a-subtraction-it-would-split.md`

    **Origin:** panel 180, 2026-09-27: the llm-ergonomist's `deltas` column
    and the historian's P1, run by the coordinator on the trunk's compiler at
    `29ed5601` (`heroes run` prints 2 for each of `[a` / `- b]`,
    `[base * qty` / `- discount]`, `[a` / `(b)]`, `[a` / `!b]`), mapped and
    censused by the compiler-engineer: of the four tokens that both continue
    a line and begin an element (`-`, `(`, `[`, `.`) only `-` is plausible
    and silent, and none of the 1447 element lines in the tree begins with
    one. The repair is the sitting's R2.

    **Why it is a defect.** A plausible mistake that compiles to a different
    program is the one class the thesis exists to refuse (design.md §1.4).

- [ ] **107 — `heroes fmt` refuses a program with a comment after `.` or `::` where the only bracket is the author's parentheses, because it drops them and the line break lands at depth 0** | `y = (xs.  # c` / `len())` parses, and `fmt` exits 2 with *produced source that does not parse* (`expected_field_name`), the file untouched; the same for `(p.  # c` / `x)`, `(Point::  # c` / `x)`, `(a + (b.  # c` / `c))`, and for a variant case's leading `.`, a binary's operand, a unary's operand, a `return` value and an `if` condition (`indentation_jump`); inside any other enclosing bracket the same comment formats | `selfhost/print/breaks.hero` (`restored`) · `selfhost/print/around.hero` · `selfhost/print/groups.hero`

    **Origin:** panel 180's compiler-engineer, 2026-09-27, reproducers in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/compiler-engineer/map/fmtdefect/`,
    widened by the critic (five more members), re-run by the coordinator on
    the trunk's compiler at `29ed5601`: `parse` exit 0 and `fmt` exit 2 on
    ten shapes (`expected_field_name` on seven, `expected_field_name_after_colons`,
    `expected_case_name`, `indentation_jump`), `fmt` exit 0 on the same
    comment inside a call, an array, an index and a type. The neighbour of lane g's fifth round (defect 101's record, F1 to
    F3), which kept the author's parentheses around a comment inside an
    index and did not reach these.

    **Why it is a defect.** A correct program has no canonical form (design.md
    §4.15, *exactly one correct way to write any program*); the guard stops
    the corruption, so it is a refusal and not a silent move.

- [ ] **108 — a comment inside an `extern` member's releaser set becomes part of the next releaser's name** | `acquires h_close |  # either one` / `h_close_v2` fails `heroes check` with `unread_releaser` for `#eitheroneh_close_v2`: `releasers` splits the set's source text and drops only whitespace, and the same reading feeds the checker, the emitter and `fmt`; without the comment the program runs | `selfhost/handles.hero:191` (`releasers`)

    **Origin:** panel 180's completeness critic, 2026-09-27, reproducer in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/critic/probes/releaser/`,
    beside the compiler-engineer's `rel_pipe` shape; loud, not corrupting, as
    far as the critic ran; re-run by the coordinator on the trunk's compiler
    at `29ed5601`: `heroes check` exit 1 with that message, and the control
    without the comment runs (*ended by h_close_v2*). The neighbour of panel
    176's releaser set.

    **Why it is a defect.** A comment changes a program's meaning, here into
    a refusal naming a function nobody wrote; a comment must be inert
    wherever the grammar lets it stand.

- [ ] **109 — every verb that reads a program panics on an empty file** | `heroes check`, `lex`, `parse` and `build` on a file of zero bytes exit 134 with `panic: string index out of range`: `source.from_files` reads the last byte of the first file, which has none; `fmt` exits 0 on the same file | `selfhost/source.hero:109` (`from_files`)

    **Origin:** lane 105's agent, 2026-09-27, met it while repairing
    `from_files` and kept it unchanged, the lane being a refactor; re-run by
    the coordinator the same day on the trunk's compiler at `2b1a1f24`:
    `check`, `lex`, `parse` and `build` exit 134, `fmt` exit 0.

    **Why it is a defect.** The compiler must not crash on an input
    (design.md §1.12); an empty file is a program with no `main`, and owes
    the diagnostic any file without one gets, or a clean exit where the verb
    has nothing to say.

- [ ] **110 — building a program whose interpolated string holds a character above ASCII before a hole panics** | `print(f"é {x}")` is `check` 0, and `heroes build` or `run` exits 134 with `panic: string slice splits a character`: `lex_interp.piece_text` slices one byte of a two-byte character | `selfhost/lex_interp.hero:104` (`piece_text`)

    **Origin:** lane 105's agent, 2026-09-27, kept unchanged in that
    refactor; re-run by the coordinator on the trunk's compiler at
    `2b1a1f24`: `check` exit 0, `run` exit 134. Beside defect 103's
    `bytes.char_at`, which reads one character by its lead byte.

    **Why it is a defect.** A program the checker accepts crashes the
    compiler; the robustness goal (design.md §1.12) is the first thing it
    breaks, and the spec lets a string hold any UTF-8.

- [ ] **111 — `heroes check` is quadratic in a program's calls and declarations, and one scan is most of the compiler checking itself** | `check/freer.marked_as_freer` reads every declaration and every parameter of the program at every call to a user function, asking whether one names it as a freer, and `resolved.declare_top` copies a module's whole map of names at every declaration: `check` on generated programs of 250, 500 and 1000 units reads 0.56, 1.45 and 4.31 s user, and on `selfhost/main.hero` the scan dominates the profile | `selfhost/check/freer.hero:40` (`marked_as_freer`) · `selfhost/resolved.hero:302` (`declare_top`)

    **Origin:** lane 105's agent, 2026-09-27, on its ladders (0.53, 1.35,
    3.92, 12.73 s at 250 to 2000 units, and 13,715 of 15,411 samples of
    `check selfhost/main.hero` in the scan); re-run by the coordinator on the
    trunk's compiler at `2b1a1f24`, the ladder above at a load near 4 and a
    `sample` of `check selfhost/main.hero` with the scan on the stack in most
    of its 10,208 samples.

    **Why it is a defect.** Defect 103's reasoning: a pass read per item where
    it is read once, in the compiler's own code, and the cost every `check`
    and every build pays grows with the square of the program.

- [ ] **112 — emission is quadratic in the size of a function** | `emit/unread.assigned` scans every instruction of a function for each temporary it declares, so `build --emit-c` less `check` reads about 3.5, 7.7 and 19.7 s at 250, 500 and 1000 units of a generated program whose `main` makes one call per unit; the lowering's `ir/flatten.call` and `ir/owned_release.library_validated` lead the rest | `selfhost/emit/unread.hero:84` (`assigned`)

    **Origin:** lane 105's agent, 2026-09-27 (2,196 of 6,633 samples of
    `build --emit-c` at 1000 units in `assigned`); the ladder re-run by the
    coordinator on the trunk's compiler at `2b1a1f24`, `build --emit-c` 4.01,
    9.12 and 24.02 s user at 250, 500 and 1000 units against `check` 0.56,
    1.45 and 4.31, at a load near 4.

    **Why it is a defect.** The same shape as 111 in the back end: one pass
    per temporary where one pass per function answers every temporary.

- [ ] **113 — the holes report reads every local and every top-level name of the program once per hole** | `check/holes.in_scope` and `nearby` walk all locals and all names for each `???`, so `check` on 200, 400 and 800 holes reads 0.13, 0.33 and 1.01 s user; what the report prints per hole is capped (design.md §4.16), what it reads is not | `selfhost/check/holes.hero:84,158` (`in_scope`, `nearby`)

    **Origin:** lane 105's agent, 2026-09-27 (0.05, 0.12, 0.31, 0.91 s at
    100 to 800 holes); re-run by the coordinator on the trunk's compiler at
    `2b1a1f24`, the numbers above at a load near 4.

    **Why it is a defect.** A report whose work grows with holes times names
    is the shape 105 closed for printed artifacts, left in a reader.

- [ ] **114 — the emitted C zeroes every temporary of a function at its entry, so a lookup that returns early pays for every arm** | the emitter declares each temporary at the top of the C function with `= {0}`, and a `match` over a string of twenty arms declares them all: the emitted `h_keywords_keyword` zeroes 139 temporaries on every call to make 21 string comparisons, and on the default build line (plain `clang`, no optimisation, CLAUDE.md § Commands) every one is executed; `memset` was about a fifth of `fmt`'s samples after defect 105's repair | `selfhost/emit/` (the temporaries' declarations) · `seed/heroes.c` (`h_keywords_keyword`)

    **Origin:** lane 105's agent, 2026-09-27 (`memset` 240 of 1,113 samples
    of `fmt` on eight copies of `walk.hero`, under the lexer's keyword and
    punctuation lookups); the emitted function read by the coordinator in
    the trunk's seed at `2b1a1f24`: 941 lines, 139 `= {0}`, 21 string
    comparisons.

    **Why it is a defect.** Every Heroes program pays it, the compiler first:
    work the program never reads, done at each call. The repair keeps the
    guarantee the zeroing buys, every temporary initialised before any read,
    and moves the initialisation to where the temporary's life begins.

- [ ] **115 — on Windows, the runtime's report of a dead C handle read inside C omits where in the page it was read** | `tests/golden/run/dead-handle-read-by-c-through-a-cell-it-should-only-write.hero` must abort saying `a dead C handle was used inside C, at offset 0x0`, and on the Windows box it says `a dead C handle was used inside C` with no offset, so the `run` suite reads 199 passed and 1 failed there; Linux x86-64, Linux arm64 and the Mac print the offset | `runtime/` (the Windows path of the poisoned page's fault handler) · the golden above

    **Origin:** the coordinator, 2026-09-27, running the `run` suite on the
    Windows box for lane 105's merge (the trunk at `09acdabc`), and the same
    golden run there on the trunk at `2b1a1f24`, before that merge: the same
    text, so it is older than the lane. Panel 177, which landed the poisoned
    page, records Windows as unrun for every row; the formatter lanes ran
    only their own suites on the box.

    **Why it is a defect.** A sentence the runtime owes is missing on one
    platform, and the net is red there; the Windows exception record carries
    the address a fault touched, so the offset can be said on every platform.

*******************************************************************************
