# Defect 129 closed: the joins of one statement take one certainty, and a join is certain only where what it writes can compile

- [x] **129 — a line holding `-` alone draws two certain joins, and applied together they write a program `check` refuses** | in `function main()`, `x = a` over a line holding `-` alone over `print(x)` costs two `continuation_outside_brackets`, and `check --apply` writes `x = a -` over `print(x)`, which `check` refuses again; in a `match`, `0 => 5` over `-` over `"=>"` has both joins applied, `0 => 5 - "=>"`, which is `bad_operand` | `selfhost/open_line.hero` (`goes_on_at_head`, `refuse_the_end`) · **closed 2026-09-30**

    **Origin:** lane 123's agent, 2026-09-28, attacking the positions beside
    defect 126 (a statement where 126 has an arm, and a string where it has a
    pattern; reproducers `shapes/x04_stmt_minus_alone.hero` and
    `shapes/x07_arrow_in_string.hero`, applied in `shapes/apbase/`, in
    `/Users/joseph/Temp/heroes-lane-123-scratch/`); reproduced by the
    coordinator on the trunk's compiler at `6b81e993` at 18:14
    (`stmt_minus_alone.hero`, `x07_arrow_in_string.hero` and their applied
    files in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect129/`).

    **Why it is a defect.** Panel 181's item 3: a `certain` fix that fails to
    compile is a defect of the landing. Each join is certain alone; the line
    holding `-` alone draws one at its head and one at its end, and nothing
    judges the two together. Defect 126 is the same shape on an arm; this is
    it at the statement and before a value that is not a pattern.

    **Carried to the next milestone**, by the author's instruction of
    2026-09-28 16:40 (`docs/records/log/2026-09-28-1640-m-agreed-retention-closes-over-the-defects-found-after-its-last-eight.md`):
    found after the eight M-agreed-retention repairs before its tag, and
    not repaired in it.

    **Widened 2026-09-28 at 23:00 by the coordinator, on the trunk's compiler
    at `79aeeffa`, to two classes, neither of them the `-`'s.** The
    directories cited above were gone by 22:55, removed by nobody in this
    conversation; `stmt_minus_alone.hero` and `x07_arrow_in_string.hero` were
    copied at 22:47 and the shapes below written beside them
    (`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/d129/`, 2026-09-28).
    (A) **The joins of one statement broken at two or more line ends are
    judged one at a time**, so a `certain` one applied without the others
    writes a line the same code refuses: `+`, `&&` and `+ b *` alone on the
    line do what `-` does (`p1`, `p4`, `p3`), and so does a statement with no
    line holding an operator alone, `x = y +` over `z +` over `print(x)`,
    joined to `x = y + z +` (`q12`), `x = y` over `+ z +` (`q14`), and a
    chain whose second break carries a comment and so no fix (`q15`); where
    every join is `certain`, `x = a` over `-` over `b`, the three-line join
    is right and compiles (`x05`, `p2`, `q13`). (B) **A `certain` join writes
    an operator against a literal it can take under no type**: `x07`'s
    `5 - "=>"`, `x = a` over `- "s"` (`q10`), `x = a &&` over `1` (`q11`), and,
    beside defect 126, the sign join `-"a" => 10` on a `str` match (`p5`);
    `-'a'` on an `i64` match compiles (`p6`), since a character is an integer.

    **Corrected 2026-09-28 at 23:17**: the directories were removed by the
    author, who took them for old lanes, and restored from the trash the
    same evening, so the paths above resolve again; the copies in the
    scratchpad are what the lanes read.

## The repair

In lane 129 (`fbf949f2`, `85066233`, `199fa845`, the trunk merged into it at
`2fcd9e7b`), merged into the trunk at `f8a4dba0`. The defect as filed was one
witness of two classes, both of panel 181 item 3's own criterion, a `certain`
join that writes a program `check` refuses.

**(A) The joins of one statement broken at several line ends are one repair,
and take one certainty, the weakest among them.** `open_line` records every
refused break (`state.Break`), and `line_joins.weigh` judges them once the file
is lexed. A break that offers no join, a comment between or a line the lexer
ends rather than refuses (`x = a +` over `b +` over `return x`), makes every
other join of its statement a `guess`. The statement's chain crosses the blocks
it opens itself (`x = a +` over `a + if a > 0`, its blocks, `else`, and `- a`)
and ends only at a terminator planted at its own level. The diagnostics stay
one per break (panel 181 item 2); only certainties move.

**(B) A join is `certain` only where the expression it writes can compile
under some typing.** `operand_families.hero` holds what each operator takes,
measured on the checker at `a6eab736` over every operator against every
literal kind and matching spec § 7, and a test in `cli/grammar.hero` holds the
lexer's copy of the binding powers to the parser's. `literal_operands.hero`
reads the operand a join writes, a literal, a prefix run, a parenthesised run
or a run binding tighter than the join, and a name, which spec § 7 puts in the
family of the literal beside it. The whole expression a join writes is judged
in its context, so a join that re-associates the rest of its line
(`x = v ==` over `1 == 2`), or changes the operand of a looser operator above
it, or puts a number where a condition stands, is a `guess`.

**The lexer's limit**, run and recorded, where the certainty stays what the
break gives: a family resting on names alone (`x = a +` over `s` with
`s: str`), a type written on another line (`x: str =` over `1`, `x @` over
`"s"`), `match` arms of another family, and a family an operator implies but
no literal names.

**The instrument, landed first so it judged the rest** (`85066233`):
`tests/harness/suite_fixes.hero` skipped every case with no `.fixed`, so no
check asserted that such a case had no `certain` fix, and a join turned
`certain` again would have passed green (the coordinator's finding on
`fbf949f2`). Every `check/` case and fixture root now has one answer: a
`.fixed` that checks clean, an `.applied` pinning the bytes `--apply` writes
where a diagnostic no certain fix repairs remains, or none, where `--apply`
writes the source back. Its first run turned 43 cases red, every diff read:
four missing `.fixed` added (`deleted-int`, `raw-carriage-return`,
`typo-not-a-new-binding`, `applyx/geom`), 39 `.applied` written, no certain fix
that lies. `fixes` went from 32 cases to 442.

**Measured**, panel 181 item 3's instrument: over the coordinator's twenty
probes, a certain fix wrote a refused program on 13 on the trunk and none on
the lane; over 3,169 generated shapes, 2,457 on the trunk and 9 on the lane,
each classified (two statements in one probe, and the type residue above).

**Found beside it and filed**: defect 132 (a join into a line holding an
arm's `=>` costs a second diagnostic) and defect 133 (after a foreign
declaration head, the declaration's own mistakes wait for the swap).

## The gate

In lane 129, on the compiler built from its regenerated seed, the fixpoint by
`cmp`, one suite at a time: check 199, annotations 248, canonical 2, fixes 442,
surface 334, probe 24, layout 4, order 3, records 24, grammar 9, run 211,
emission 638, determinism 241, corpus 55, warnings 272, lines 212, each 0
failed; the compiler's own tests 839, the net's own 181. At the coordinator's
gate of the merge (`2fcd9e7b`, landed as `f8a4dba0`): the fixpoint, the whole
net 3618 passed and 0 failed over 26 suites, and a census of `check` over the
1239 tracked `.hero` files in both arms, trunk against merge, moving none, the
lane moving certainties, which `fixes` judges, and never the count of messages.

On the platforms, at the trunk `9fd7ef70`, which holds it: Linux arm64 (the
`heroes-linux-arm64` image) the compiler's 852 tests and surface 334,
canonical 2, annotations 270, check 221, fixes 464, layout 4, order 3, lines
208, run 207, emission 626, determinism 237, wholes 303, descriptors 303,
cache 6, grammar 9, spec 20, probe 24, corpus 53, warnings 266, each 0
failed; the Windows box, one archive whose sha256 matched on both sides
(`99157710ccf4ef8c`), the compiler's 852 tests and surface 327, canonical 2,
annotations 270, check 221, fixes 464, layout 4, order 3, lines 206, run 205,
emission 604, determinism 235, wholes 303, descriptors 303, cache 6, grammar
9, spec 20, probe 24, corpus 50, warnings 261, each 0 failed.

And at the batch gate of lanes X3 and X4 (`53a5e0a9`), the first under
CL-079: the compiler's own tests 896, the net's own 184, the full net six
suites at a time with `cache` alone after, every one of the 26 at 0 failed
once `layout`'s floor was raised as it asked; the defect's twenty probes
re-run there, and no `certain` fix brings a code the first run did not
report.
