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

- [ ] **096 — inside an `extern` group, a remark followed by a blank line before the next member makes `fmt` refuse the file** | the group printer never emits the blank line for a continuing member, so the remark becomes the next member's doc, the self-check sees a different tree and `fmt` exits 2 on a program `check` accepts | `selfhost/print/fmt.hero` (the group's member walk, the `continues` branch)

    **Origin:** the skeptic seat over defect 095's repair, 2026-09-25, attacking
    the repair at the shapes beside it; measured on the seed compiler of
    `0a8fd346` and on the repaired one alike, so it is older than both.

    **The reproducer.** A group whose first member is followed by a remark, a
    blank line, and `record Ob tag ob`:

        extern "x.h"
            function ob_put(o: Ob consumes)
            # a remark about what follows

            record Ob tag ob

    `check` exit 0; `fmt` exit 2, *`fmt` changed the TREE … a DIFFERENT
    PROGRAM*, and the file is not touched. With no line-broken signature
    anywhere, so it is not 095's shape: the blank line is what is lost.

    **Why it is a defect.** A correct program is refused by a tool that must
    be idempotent on every program the parser accepts (design.md §4.15); the
    exit-2 guard stops the silent form, which would attach the remark to the
    record as its doc. The repair keeps the blank line between a remark and
    the member it does not document, inside a group as outside one.

- [ ] **099 — a comment after the last item of a block leaves the block when `fmt` runs** | a `#` remark written at the block's indent after the last member of a group, the last statement of a body or the last field of a record is printed by the NEXT declaration's walk, at column 0, with a blank line added under it: exit 0, the tree the same, and the comment now sits at file level above the next function | `selfhost/print/fmt.hero` (`format_file`'s walk, `comments_before` at the next item's indent)

    **Origin:** the parser seat that repaired defect 096, 2026-09-25, measuring
    the shapes beside its reproducer; reproduced by the coordinator the same
    day on the trunk's compiler at `0029567d`, so it is older than that repair.
    Searched `docs/work/`, `docs/records/done/` and `docs/learn/` for "last
    statement", "end of the body", "after the last member", "after the last
    field", "trailing remark": not filed.

    **The reproducer**, three shapes, each `fmt` exit 0 and each output a
    fixpoint:

        function f() -> i64
            x = 1
            return x
            # a remark after the last statement of the body

        function main()
            print(f())

    comes back with the remark at column 0 under `return x`'s block, above
    `function main()`, a blank line under it; the same for a remark after the
    last member of an `extern` group and after the last field of a record.

    **Why it is a defect.** `fmt` moves a comment out of the block the author
    wrote it in, silently: the tree does not change, so the self-check that
    stops defect 095's and 096's shapes cannot see it, and a remark about the
    end of a body now reads as a remark about the function below it. Defect
    095's record calls the silent move the worse form of the two. The repair
    prints a comment indented inside a block, after the block's last item and
    before the dedent, at the block's own indent.

    **And without the blank line it is a refusal, measured 2026-09-25** by the
    skeptic seat over defect 096's repair and re-run by the coordinator on the
    trunk's compiler: the same remark written directly above the next
    declaration, no blank line between (a body's last statement then
    `function main()`, a group's last member, a record's last field inside a
    group), is `check` 0 and `fmt` **2**, *changed the TREE*, because the
    remark printed at the next declaration's column becomes its doc. One root,
    two outcomes: with the blank line the move is silent, without it the
    self-check catches it.

- [ ] **100 — inside an `extern` group, a comment at another column than the member below it, or directly under a second head of the same group, becomes that member's doc when `fmt` runs** | `fmt` re-indents a column-0 comment between two members to the members' indent, and merges two `extern` heads with the same header into one group while keeping a remark that sat directly under the second head, so in both the comment ends up at the member's column, directly above it, and the re-parse takes it as the member's doc: `check` 0, `fmt` exit 2 | `selfhost/print/fmt.hero` (the group walk, `continues`, `extern_head_once`)

    **Origin:** the skeptic seat over defect 096's repair, 2026-09-25, beside
    its reproducer; re-run by the coordinator the same day on the trunk's
    compiler at `5c3a6e39`, so older than that repair.

    **The reproducers.** A group `extern "x.h"` with `record Ob tag ob`, then
    a comment at column 0, then `    record Pool tag pool`: `fmt` 2. And two
    groups `extern "x.h"` one under the other, the second holding
    `    # remark under a second head` above `    record Pool tag pool`:
    `fmt` 2. `take_docs` in the parser takes a comment as a doc only at the
    member's own column and directly above it, which the source did not have
    and the output does.

    **Why it is a defect.** A correct program is refused by a tool that must
    be tree-preserving on every program the parser accepts (design.md §4.15).
    Beside it, the skeptic found one shape the repair of 096 in its first form
    broke: a doc above the second of two same-head groups, `fmt` 0 on the
    trunk, lost at the merge in the repair's first form; the repair of this
    defect and of 096 land together.

- [ ] **101 — `fmt` moves a comment silently at three more places: off a block-opening line, out of a bracket at a body's end, and away from a blank line it adds between match arms** | a trailing comment on the line that opens a block (`if x > 0  # note`) is printed on the block's first line; a comment trailing the last element of a bracketed value at the end of a body leaves the function and becomes a column-0 remark above the next declaration; and a comment between two `match` arms gains a blank line under it the source does not have. Each at `fmt` exit 0 with the tree the same, so the self-check cannot see it | `selfhost/print/fmt.hero` (the block openers, the bracketed-value printer, the match-arm walk)

    **Origin:** the skeptic seat over the repair of defects 096, 099 and 100,
    2026-09-25, beside its reproducers; reproduced by the coordinator the same
    day on the trunk's compiler at `3b40c60d`, so it is older than that
    repair.

    **The reproducers.** `if x > 0  # only the positive case` over a block:
    `fmt` 0, the comment printed as the block's first line. `return [` then
    `1,` then `2  # the last element` then `]` as a body's last statement:
    `fmt` 0, the elements printed one per line and the comment at column 0
    under the function, above `function main()`, a blank line under it. A
    `# the other values` between the two arms of `return match x`: `fmt` 0, a
    blank line added under the comment.

    **Why it is a defect.** The first two move a comment away from the code
    the author wrote it beside, silently, which defect 095's record calls the
    worse form of a formatter's mistake; the third breaks the rule defect
    096's repair wrote down, that a blank line is printed only where the
    source has one. None changes the tree, so no instrument sees any of them.
    The repair keeps a trailing comment on the line it trails, a comment
    inside a bracket inside the bracket, and prints a blank line between arms
    only where the source has one.

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

*******************************************************************************
