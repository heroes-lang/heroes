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
**OPEN: 5**

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

- [ ] **105 — `heroes lex --dump-tokens` and `heroes fmt` build what they print by appending to one string, so both are quadratic in the size of their output** | every token line and every printed line is appended as `out @ out + …` or `f.out @ f.out + …`, and a string concatenation copies the whole string under value semantics (design.md Part 8 wart 8, which panel 144 left a wart because the cost is in the spelling, naming accumulation into an array joined once as the cheap one): the JSON token dump of `selfhost/check/walk.hero`, 108 KB, takes 3.67 s user, 14.09 at two copies and 53.18 at four, the text dump 0.58, 2.22 and 12.85, while `parse --dump-ast` reads 0.08, 0.17 and 0.33 | `selfhost/cli/lex.hero:36-71` (`json`, `text_dump`) · `selfhost/print/page.hero:70-82,294-299` (`put_line`, `blank_line`) · `selfhost/print/margins.hero:32-54` (`deepened`)

    **Origin:** the coordinator, 2026-09-27, timing lane g's merge for defect
    101's record: `fmt` on 1, 2, 4, 8, 16 and 32 copies of `walk.hero` read
    0.28, 0.58, 1.25, 2.80, 6.86 and 21.15 s user on the trunk's compiler at
    `316d974f`, and 0.21, 0.44, 0.96, 2.22, 5.53 and 19.12 on the one at
    `f37b01b3`, the same growth, so it is older than lane g. `sample` on the
    8-copy run put the copies under `put_line` at 780 of 1695 samples; the
    ladder then taken on `lex --dump-tokens` read 50.04 s at 8 copies and
    194.31 at 16. `selfhost/cli/lex.hero` is unchanged by lane g. The wart
    itself, re-measured the same day on a bare local and through a field of a
    record: 20,000, 40,000 and 80,000 appends of 22 bytes, 0.10, 0.32 and 1.17
    s user, the same for both, so here the place is not what costs.

    **Why it is a defect and not the wart.** The wart stays a wart on panel
    144's ruling, and names the cheap spelling; `parse --dump-ast` uses one and
    is linear. This is the compiler's own code using the spelling the ruling
    calls slow, as defect 103 was, and a dump a reader or a harness asks for
    on a large file takes minutes. The repair is owed at the class: every
    artifact a verb prints, measured on a ladder, not the two witnesses; and a
    check that fails if the slow spelling comes back where a check can read it.

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

*******************************************************************************
