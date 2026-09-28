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
**OPEN: 4**

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

    **What it actually is, read in the emitted C, 2026-09-28** (the
    coordinator, on the trunk's seed at `c90fd658`). The zeroing is not
    waste: it is the release's precondition. Each arm of the `match` writes
    its value into an owned variable of its own (`h3_own3` to `h25_own25` in
    `h_keywords_keyword`), each store releases the variable's old value
    first, and the function's exit releases all 23, the arms not taken
    included, so an unzeroed one would be released uninitialised. `= {0}` is
    emitted only on a refcounted type (`emit/body.hero`, `is_refcounted`),
    and `TokenKind?` is one because its failure half carries strings. The
    cost is in the lowering's shape, one owned variable per arm where the
    arms could share one destination, and changing how a `match` lowers is
    the IR's architecture: a panel path (CLAUDE.md § 4), convened when the
    seats can sit again. Removing the zeroing without it would trade a
    robustness guarantee for speed, which § Precedence refuses.

- [ ] **122 — a build's object cache ignores which C compiler and which flags built an object, so a changed compiler or flag reuses stale objects** | the unit key (`selfhost/cli/units.hero:82-84`) is the fingerprint, level, module, text, runtime and search paths, the fingerprint being `VERSION`, and the runtime object's (`selfhost/cli/toolchain.hero:154`) likewise: a program printing `__clang_major__` built under Apple clang 21 prints 21, built warm with Homebrew clang 22.1.8 first on the `PATH` still prints 21, and built cold prints 22; a compiler with one more flag in its list and the same `VERSION` reused every object (the critic) | `selfhost/cli/units.hero` · `selfhost/cli/toolchain.hero` · `selfhost/cli/clang_floor.hero` (which already writes `clang --version` on every build)

    **Origin:** panel 182's compiler-engineer, 2026-09-28, reading the key
    while probing a pattern-initialised build; run by the sitting's critic
    (clang 21 against 22 on a warm directory, and a flag added with the version
    unchanged), and reproduced by the coordinator the same morning in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect122/`.

    **Why it is a defect.** A build that silently links an object compiled by
    another compiler, or without a flag the compiler now passes, is a program
    other than the one its source and its compiler say, at exit 0; and it is
    the precondition for every new instrument leg the sitting names.

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

*******************************************************************************
