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

- [ ] **103 — the lexer is quadratic in the size of a file: it copies the token array at every line end and the rest of the file at every escape** | `layout.hero` appends the line-end, indent and dedent tokens as `l.tokens @ l.tokens.push(…)`, a field place the in-place store never fires on (design.md Part 8 wart 8), so every line copies the whole token array and resets its capacity, and the next in-place push reallocates it; and `literals.escape_step` reads one character as `text.slice(from: …, to: text.len()).chars()[0]`, a copy of the rest of the file per escape, as the char-literal loop and the unexpected-character path do per character | `selfhost/layout.hero:112,119,135` · `selfhost/literals.hero:30,159` · `selfhost/scan.hero:98,215` · `selfhost/state.hero:97,105` · `selfhost/lexer.hero:123,125`

    **Origin:** lane g's agent, 2026-09-26, timing its own round (*parsing
    walk.hero four times over takes 6.3 s against 0.16 s for one copy*);
    measured by the coordinator the same day on the trunk's compiler at
    `bdf430f1`, and profiled with `sample`, which put the time in
    `hero_array_push` under `layout.maybe_terminator` and `layout.line_start`
    and in the Token copies inside `hero_array_push_owned` under `state.emit`.

    **The measurements**, `heroes lex` on a file of 500 functions and its 2, 4
    and 8 copies, `user` seconds: with a string holding escapes in each, 1.32,
    5.13, 20.67, 91.05; with a plain string, 0.03, 0.12, 0.48, 2.58; with no
    string at all, 0.13, 0.49, 1.89 at 2, 4 and 8. Four times the time for
    twice the file, every row; 210 KB of source takes a minute and a half to
    lex.

    **Why it is a defect and not the wart.** Design.md Part 8 wart 8 stays a
    wart on panel 144's ruling that the cost of a push through a field is in
    the spelling, and names the cheap one: lend the field to an `@`
    parameter. `state.push_token` is exactly that, written for this lexer on
    2026-08-26 with a comment saying so, and the lexer calls it from one path
    of the eight that append a token. The repair is the spelling the ruling
    names at every append, a character read by its own width, and a test
    that fails if the slow spelling comes back.

*******************************************************************************
