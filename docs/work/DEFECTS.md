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
**OPEN: 8**

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

- [ ] **117 — `heroes mutate <file>` says it cannot read a file it reads** | `heroes mutate examples/adventure/main.hero` answers `error: cannot read examples/adventure/main.hero` at exit 2: `mutate` takes a directory of programs (`no_corpus`), and the message names the wrong cause | `selfhost/cli/mutate.hero:86,99`

    **Origin:** lane A's sweep of every verb on degenerate files, 2026-09-27,
    where `mutate` on each of the 15 files read the same before and after the
    repair of defect 109; re-run by the coordinator on a real program on the
    trunk at `f08b192d`.

    **Why it is a defect.** A diagnostic carries what is needed to fix the
    program without opening another file (design.md §4.17), and this one sends
    the reader to look for a permission or a missing file that is not there.

*******************************************************************************
