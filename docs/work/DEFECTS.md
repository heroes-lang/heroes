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
**OPEN: 6**

- [ ] **123 — a `match` arm whose pattern begins with a `-` set apart from its operand compiles, where the spec refuses it** | `k = match n` with the arm `- 1 => 10` is `check` 0 and prints 10, as `-1 => 10` does; spec § 0 reads *where a NEWLINE separates without a `,`, the next line may not begin with a `-` that does not touch its operand*, and a `match`'s arms are separated by NEWLINE; panel 180's `spaced_minus_element` refuses the shape in a literal and panel 181's refusal at depth zero, and an arm is the one NEWLINE-separated context left | `selfhost/parse/list_line.hero` (`spaced_minus_element`) · `selfhost/grammar_expr.hero` (`arm`)

    **Origin:** lane 181's agent, 2026-09-28, beside panel 181's landing,
    left unchanged and reported; reproduced by the coordinator on the trunk's
    compiler at `28d7129c` the same morning (`arm_minus.hero` in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect123/`).

    **Why it is a defect.** The spec states the rule without the bracket
    limit design.md's panel 180 bullet gives it, and CLAUDE.md § 12 reads the
    disagreement as the compiler's: a reader of the spec predicts a refusal
    the compiler does not make. The shape has one reading, a negative
    pattern, so the refusal loses no program and its fix, the `-` touching
    its operand, is the spelling `fmt` prints.

    **Corrected 2026-09-28, by lane 123's agent:** `fmt` prints no spelling
    of a negative pattern, it exits 2 on every one, the unspaced `-1 =>`
    included (defect 125), so the last clause above was false when written.

- [ ] **124 — after a `-`, a `match` pattern reads a whole expression, so a pattern can name a variable or call a function** | `grammar_expr.pattern` reads `pattern_operand`, which is `unary`, so after a `-` any postfix expression parses, and `check/walk.literal_pattern` compares only its type: `-m => 10` with `m: i64 = 1` and `n = -1` is `check` 0 and prints 10, the pattern compared against a runtime name, and `-one() => 10` runs `one` inside the match, printing its 99 and then 10; also `-(1)`, `-xs[0]`, `--1`, `- -1`, and `-1.5` on an `f64` where `1.5` is `expected_pattern`; spec § 8 reads `Pattern = ... | [ "-" ] ( integer | string | character )` | `selfhost/grammar_expr.hero` (`pattern`, `pattern_operand`) · `selfhost/check/walk.hero` (`literal_pattern`)

    **Origin:** lane 123's agent, 2026-09-28, attacking the shapes beside
    defect 123 (reproducers in `/Users/joseph/Temp/heroes-lane-123-scratch/shapes/`);
    reproduced by the coordinator on the trunk's compiler at `aee8b01e` the
    same morning (`name_pattern.hero`, `call_pattern.hero` in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect124/`).

    **Why it is a defect.** A pattern is a constant the spec names, and the
    compiler admits a name, which silently reads a value at run time, and a
    call, which runs code with its effects inside a match, both at exit 0; and
    a float after a `-` where a float is not a pattern at all.

    **Widened 2026-09-28, by lane 123's agent:** it is wider than the `-`.
    With no sign at all a pattern reads a literal's suffixes, `"ab".len() =>
    10` compiles and prints 10, `'a'.to_i64().must() =>` compiles, and so does
    `0 | -m`: a pattern's literal is one token, optionally after one `-`.

- [ ] **125 — `fmt` accuses itself on every negative literal pattern** | `k = match n` with the arm `-1 => 10` is `check` 0, and `heroes fmt` exits 2 with *`fmt` produced source that does not parse* (`expected_pattern`, found `(`) and *this is a compiler bug*: `print/bodies.render_pattern` renders a literal pattern through `render_expr`, whose unary case prints `(-1)`, and a parenthesis opens no pattern; also on `return match n` and on a statement `match` | `selfhost/print/bodies.hero` (`render_pattern`)

    **Origin:** lane 123's agent, 2026-09-28, attacking the shapes beside
    defect 123 (reproducers `shapes/u01` to `u03` in
    `/Users/joseph/Temp/heroes-lane-123-scratch/`); reproduced by the
    coordinator on the trunk's compiler at `62425393` the same morning
    (`neg_pattern.hero` in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect125/`).

    **Why it is a defect.** A verb that says *this is a compiler bug* is one;
    and no suite saw it because no fixture and none of the probe's families
    holds a negative pattern.

- [ ] **126 — panel 181's join is certain on an arm whose `-` is split from its operand by a line end or a comment, and writes a program that does not parse** | `match n` with `0 => 5` over a line holding `-` alone over `1 => 10` costs three diagnostics, and `check --apply` writes `0 => 5 - 1 => 10`, refused with `expected_end_of_line`; with `-  # the sign` the join keeps the comment inside the line; on the first arm the certain join writes `- 1 => 10`, which defect 123's repair refuses; and the parenthesised `guess` would wrap a pattern's value, and a pattern takes no parentheses | `selfhost/open_line.hero` (`goes_on_at_head`, `refuse_the_end`) · `selfhost/parse/wrap_break.hero` (`spanning`)

    **Origin:** lane 123's agent, 2026-09-28, attacking the separators beside
    defect 123 (a line end or a comment where 123 has a space; reproducers
    `shapes/s07` to `s10` and `applied/` in
    `/Users/joseph/Temp/heroes-lane-123-scratch/`); reproduced by the
    coordinator on the trunk's compiler at `3bde187f` the same morning
    (`arm_split.hero` in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect126/`).

    **Why it is a defect.** Panel 181's item 3: a `certain` fix that fails to
    compile is a defect of the landing, and `heroes check --apply` would write
    it into a file.

- [ ] **127 — a literal pattern on an integer type other than `i64` is refused, so a `match` over a `u8` can only be `_`** | `255 =>` in a `match` over a `u8` is `type_mismatch: expected u8, found i64`, and `1 =>` over an `i32` likewise: `check/walk.literal_pattern` synthesises the literal's type, `i64`, instead of checking the literal against the scrutinee, where spec § 2 reads *a literal takes the type its context asks for*; and `x: i64 = -9223372036854775808` compiles while the same spelling as a pattern is `int_out_of_range` | `selfhost/check/walk.hero` (`literal_pattern`)

    **Origin:** lane 123's agent, 2026-09-28, attacking the shapes beside
    defect 124 (reproducers `shapes/v05`, `v06`, `v12`, `v15`, `v17` in
    `/Users/joseph/Temp/heroes-lane-123-scratch/`); reproduced by the
    coordinator on the trunk's compiler at `5fdd0edd` the same afternoon
    (`u8_pattern.hero` in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect127/`).

    **Why it is a defect.** A program the spec admits, a `match` over a byte
    by its values, is refused, and the only spelling left is `_`; the
    refusal's message names a type the author never wrote.

- [ ] **128 — two literal arms with the same value compile, while two arms naming the same case are refused** | `match n` with `0 => 10` and a later `0 => 30` is `check` 0 and prints 10, the second arm never reachable, and `-0` beside `0` likewise; the same shape over a variant, `.dot` twice, is `duplicate_arm` (*`.dot` is already covered by an earlier arm*, `selfhost/data_errors.hero:254`) | `selfhost/check/walk.hero` (the arm walk that calls `duplicate_arm` for a case and not for a literal)

    **Origin:** lane 123's agent, 2026-09-28, as a question beside defect 124
    (`shapes/v07`, `v14`); the coordinator found the existing refusal for
    cases and reproduced both shapes on the trunk's compiler at `5fdd0edd`
    (`dup_literal.hero`, `dup_case.hero` in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect127/`).

    **Why it is a defect.** The compiler already names an arm an earlier arm
    covers as a mistake, for cases; for literals it keeps the unreachable arm
    in silence, the plausible slip of a copied line whose value was not
    changed.

*******************************************************************************
