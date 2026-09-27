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
**OPEN: 3**

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

- [ ] **116 — a statement continued at its own margin after a trailing operator compiles, where design.md says a long expression at depth zero is broken inside parentheses or not at all** | `y = a +` over `1` at the statement's own indentation is `check` 0 and prints 6: the lexer plants no terminator after `+` (Go's rule) and the same margin plants no indent, so the statement goes on; one level deeper it is refused; design.md §4.15 reads *at bracket depth zero every line's indentation is structural: a long expression is broken inside parentheses or not at all*, and *trailing-operator continuation at depth zero (Nim's rule) was considered and deferred* | `selfhost/layout.hero` (`is_line_ender`, `maybe_terminator`) · `docs/design/design.md` §4.15

    **Origin:** lane B's agent, 2026-09-27, beside defect 107 (a comment on
    its own line in such a continuation is refused by `fmt` at exit 2, since
    the owner rule reads the continued line as a new logical line); searched
    `docs/work/DEFECTS.md`, `docs/records/` and `docs/panel/` for *same
    indentation*, *depth zero* and *Nim's rule* and found only panel 007's
    deferral and panel 180's reports quoting it. Re-run by the coordinator on
    the trunk at `f08b192d`: `y = a +` / `1` and `y = xs.` / `len()` at the
    same margin, `check` 0 and `run` printing 6 and 2; the continuation one
    level deeper, `check` 1.

    **Why it is a defect, and why it needs a sitting.** The compiler admits a
    form the source of truth says was deferred; the spec's own last-token
    rule reads as admitting it. Whether the repair refuses it, as design.md
    says, or design.md admits it, is what the lexer does: a panel path
    (CLAUDE.md § 4).

- [ ] **118 — `fmt` accuses itself on a `match` arm continued at its own margin** | `.dot => a +` over `1` at the arm's own indentation, inside `y = match s`, is `check` 0 and prints 6 (defect 116's shape), and `heroes fmt` on it exits 2 with *`fmt` is not a fixpoint on its own output* and *this is a compiler bug*; the same shape after a `.` at a statement's margin, `y = xs.` over `len()`, formats at exit 0 to `y = xs.len(` over a `)` of its own | `selfhost/print/fmt.hero` · `selfhost/layout.hero` (`maybe_terminator`)

    **Origin:** the coordinator, 2026-09-28, writing panel 181's brief on
    defect 116: the seventeen shapes of that brief run through `fmt` with the
    trunk's compiler built from the seed at `dfcac362`; five format at exit 0,
    the arm exits 2.

    **Why it is a defect, and why it has a number of its own.** A verb that
    says *this is a compiler bug* is one, whatever the input's standing. If
    panel 181 refuses the shape, the input stops parsing and this closes with
    116; if it admits it, the formatter is owed a repair of its own, and a
    defect hidden inside another's body would be forgotten by the route that
    does not close it.

*******************************************************************************
