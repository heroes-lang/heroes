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
**OPEN: 2**

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

*******************************************************************************
