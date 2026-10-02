# Defect 135 closed: every certain fix the audit found with two readings now writes the one meant, or is a guess

- [x] **135 — a `certain` fix chosen from one reading, where another is as likely, writes a program that means something else or is refused anew** | `print("\(n)")` costs `unknown_escape` with the certain fix `\\(`, which checks clean and prints the hole's text where Swift's author meant its value; `True` costs `unknown_name` with the certain rename to the one in-scope name within two edits, `run` or `Value`, which then costs `type_mismatch`; a `,` left out before a mutable argument, `hints: nullptr @res`, costs `misplaced_mutable_marker` with the certain `: ` that writes `nullptr : @res`, refused anew | `selfhost/literals.hero:60` (the escape's fix) · `selfhost/resolve/errors.hero` (`suggest`, `nearest`) · `selfhost/grammar_expr.hero` (`misplaced_mutable_marker`) · **closed 2026-10-02**

    **Origin:** the coordinator's measurement lane, 2026-09-30, the parser
    recovery instrument over 641 programs at `c85bccb8`
    (`scratchpad/instrument/baseline-c85bccb8/`): of 2,630 texts `--apply`
    changed, 17 checked clean and meant something else and 23 carried a code
    the first run did not report. The first three shapes below reproduced by
    the coordinator at 03:48 on the trunk's compiler; the fourth is read from
    the instrument's record and not re-run.

    **Measured.** `interp-swift`: 17 of 25 mutants apply into a program that
    checks clean and prints `\(` and the hole's text, and 6 into one refused
    anew. `python-bool`: 13 of 152 renames of `True` or `False` pick a record, a
    variant or a function (`did you mean run?`, `fix (certain): rename to
    run`, then `expected bool, found (function(str) -> ())`). `missing-comma`:
    3 of 153. And one rename of a one-letter name to the only other one-letter
    name in scope (`p` to `s`), where a line over-indented had taken `p` out of
    scope, which then costs `not_mutable`.

    **The cause, read and not yet proved by a repair.** Each fix encodes one
    reading of the mistake where another is as likely, so it is not certain in
    the sense `.claude/rules/diagnostics-and-goldens.md` gives the word, a fix
    that repairs the defect the diagnostic names, compiling not being the bar.
    `\(` followed by a bracketed expression reads as Swift's interpolation as
    much as a literal backslash; `nearest` counts any one candidate within two
    edits (one below four letters) as certain, and the literals `true` and
    `false` are not among its candidates, so `True`, two edits from `run`,
    renames to it; a name and `@` with no `,` between them
    reads as a missing `,` as much as a misplaced `@`.

    **Why it is a defect.** `check --apply` machine-applies a certain fix, so a
    wrong one is written into the author's program without a reader, and in
    the first shape the result compiles and prints the wrong thing: defect
    120's class (a certain `_ = ` that compiled and printed 100 where 93 was
    meant), and defect 015's (a context-blind certain replacement).

    **Unrun, questions rather than premises**: every other `certain` fix in
    `selfhost/` measured by the same instrument's `--apply` pass beyond these
    96 operators; whether a rename's certainty can ask the candidate's kind
    (a value where a value stands, a type where a type does) at the resolver,
    which does not know types.

    **2026-09-30, lane recovery-b3, an escape the language does not have
    keeps its certain `\\` only where it reads as nothing but a backslash**
    (the instrument's `\(`, and beside it an escape by a character's code, one
    written bare, one in a character literal): repaired at `455940f5`, gated
    by its cases and the compiler's own tests; the net is owed at the batch's
    close.

    **2026-09-30, lane recovery-b3, a rename is certain only where the name's
    own letters allow the one candidate, and `True` is `true`** (the
    instrument's `python-bool`, and `p` renamed to `s`): repaired at
    `4e5ef22e`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-09-30, lane recovery-b3, a value then `@` in an argument list reads
    as a named mutable argument only where the value is a bare name, and as a
    `,` left out always** (the instrument's `missing-comma`): repaired at
    `83e77694`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-09-30, lane recovery-b3, a record's or a variant's type parameters
    end its head's line, told once at the `<`, and its members are read**
    (lane recovery-b2's report, its first shape): repaired at `042a14a4`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close.

    **Widened 2026-09-30 by the coordinator's certain-fix audit**, a measuring
    lane at `6d781be1` that enumerated every site able to write a `certain`
    fix (the 31 lines building one with `.certain`, the 78 `Fix(`
    constructions and the helpers traced to their callers, and the 12 swap
    rows of `keywords.hero`'s table: 42 sites) and probed each with a second
    reading, about 165 probes (`scratchpad/certain-audit/`; each breach's
    reproducer, what `--apply` wrote and the program meant, under
    `findings/<site>/`). **16 of the 42 breach the rule**, 11 of them writing a
    program that checks clean and means something else. The discard's `_ = `
    (`discard_errors.hero:79`) is certain on `push(xs, 4)`, `b.items.push(4)`,
    `grid[0].push(1)`, `sort(xs)` and a bare `x == 5`, since
    `returns_its_receiver` reads only the dotted form with a bare receiver,
    and in a `test` block it turns a comparison that lost its `assert` into a
    passing test (reproduced by the coordinator at 10:53 on the trunk's
    compiler: `add(a: 1, b: 2) == 3` over an `add` that returns 4 draws
    `discarded_value`, `--apply` writes `_ = add(a: 1, b: 2) == 3`, and
    `heroes test` reads `1 test, all passed`, where the `assert` meant reads
    `1 failed`). The positional labels of `needs_label` and `missing_label`
    (`data_errors.hero:193`, `:204`) certify the order written: Go's
    `copy(dst, src)` order becomes `blit(from: screen, to: sprite)`, and
    `Size(640, 480)` over fields `height, width` prints 480 where 640 is
    meant. The rename's one slip between two meaningful names
    (`resolve/errors.hero:187`: `printf` to `print`, `origin_y` to `origin_x`,
    `line2` to `line1`), and the module's near name with no such guard at all
    (`module/diagnostics.hero:85`: `calc.sub` to `calc.sum`, `calc.median` to
    `calc.mean`). `::` for a dot on a record's name when a value of that name
    is in scope (`check/access.hero:101`). Python's `\N{...}` and an
    apostrophe meant as `'\''` (`escape_report.hero:43`, `literals.hero:204`).
    Refused anew: the `@` of `marker_mismatch` on a place that cannot take it
    (`data_errors.hero:219`), `while` for a `for` over an array or a string
    (`parse/loop_habit.hero:52`), the arm sign on a string or a character
    (`parse/arm_line.hero:50`), the token moved up in `(a` over `(b))`
    (`parse/line_end.hero:221`), the nested bare function type
    (`parse/type.hero:255`), `.must()` and `.default()`'s text found by the
    last `.default(` in the whole call (`check/builtins.hero:413`), `0X` with
    no digits (`number.hero:80`), and `int` inside an `extern` group made
    `i64` where C's `int` is the spec's `i32` (`scan.hero:152`'s row). The
    three sites the audit calls a wrong reading still refused
    (`literals.hero:89`, `data_errors.hero:89`, `parse/signature.hero:116`)
    and the one it found unreachable (`data_errors.hero:177`) are in its
    table. What overlapping certain fixes do to `--apply` itself is not this
    defect's: it is 137.

    **Batch gate 2026-09-30 09:38** (`77b8ca98`, the merge gate of lane
    recovery-b3's batch, closed at `46846f97`, beside defect 134): the full
    net 3907 passed and 0 failed with no suite re-run, the census moving the
    lane's ten new cases and nothing else, and the recovery instrument over
    its 13,594 planted mistakes reading ONE 11,933, EXTRA 1,326, APPLY-NEW 0,
    APPLY-OTHER 0 and 17 hidden second mistakes where the baseline read 153;
    then Linux x86-64, 930 tests and 19 suites at 0 failed by 11:40, and the
    Windows box at `6d781be1`, the same code, 930 and 19 at 0 failed by 11:42.
    Before the push, Linux arm64 at `ec1fd0a9`, the same code again, 930 and
    19 at 0 failed by 14:46.

    **2026-09-30, lane 135b, a dropped value takes a certain `_ = ` only where
    the discard is its line's one reading** (the audit's C2: a place of the
    value's own type in any spelling, a `bool`, a line in a `test`, the end of
    a function that returns a value, an arm's inline body): repaired at
    `8c0f80e7`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-09-30, lane 135b, a missing label is certain only where the
    argument can stand nowhere else** (the audit's C6 and C7: one argument
    alone, a type only one position takes, a marker only one carries; a
    label goes before an argument's `@`): repaired at `5ea6b86f`, gated by
    its cases and the compiler's own tests; the net is owed at the batch's
    close.

    **2026-09-30, lane 135b, a rename is certain only for a slip inside a
    word of the name, and a module's near name asks the same** (the audit's
    R2 and R3: both ends kept, no digit, a part of three letters or more):
    repaired at `6eda1b3a`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-09-30, lane 135b, `::` for a dot on a record's name is certain
    only where the position asks for a field's name** (the audit's C10: a
    `str` asked for and no value spelled like the record bound in the
    declaration): repaired at `045e6519`, gated by its cases and the
    compiler's own tests; the net is owed at the batch's close.

    **2026-09-30, lane 135b, the `@` a marked parameter asks is certain only
    before a place that can take it** (the audit's C8: a place rooted in a
    cell or an `@` parameter; elsewhere no `@`, and a note): repaired at
    `8ee4efeb`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **Batch gate, lane 135b, 2026-09-30 21:32** (`f3942bac`, the trunk
    `cf943ddf` merged at `827dcd92`): the seed regenerated once, the fixpoint
    by `cmp`; the compiler's own tests 939 and the net's own 184, all passed;
    the full net 3,952 passed and 0 failed; the census over 1,411 files, no
    exit moved, 8 texts reworded by the batch; the certain-fix audit's probes,
    167 certain fixes in 136 probes before and 123 in 105 after, none into a
    new refusal; Linux x86-64 on `f3942bac`, 939 tests and 19 suites at 0
    failed. **Integrated** at `3cc3b553` (defect 136's record carries the
    integration). The item stays open for the next batch's sites: P4 to P7,
    C9's span, L3 to L5, and L6.

    **2026-10-01, lane 135c, a token moved up to the line above is certain
    only where going on is its one reading** (the audit's P6, a `(` or `[`
    below a value in a group or an index; and lane recovery-b4's, a line that
    can stand on its own inside a bracket no closer closes): repaired at
    `19fdef87`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close. P7 no longer breaches on the base of this
    lane, `9d1c209d`: defect 137's rounds write its nested types right (the
    audit's three probes and eighteen beside them).

    **2026-10-01, lane 135c, removing a `.must()` or `.default()` that
    unwraps nothing keeps the call's own receiver, and is certain only where
    it drops nothing that runs** (the audit's C9, the receiver read from the
    call's tokens; beside it, a `.default(v)` whose `v` is not a literal):
    repaired at `b788f716`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-01, lane 135c, *use `while`* for a `for` is certain only where
    its condition is a `bool` by its shape** (the audit's P4, and beside it
    `for 3`, `for ("abc")`, `for {"a": 1}`): repaired at `aa22c907`, gated
    by its cases and the compiler's own tests; the net is owed at the
    batch's close.

    **Batch gate, lane 135c, 2026-10-01 21:14** (`8025ad15`, P6, C9 and P4;
    P7 found closed by defect 137's rounds; defect 141 closed by the same
    gate): the seed regenerated once, the fixpoint by `cmp`; the compiler's
    own tests 973 and the net's own 184, all passed; the full net 4,207
    passed and 0 failed; the census over 1,509 files, every move the batch's
    own; the certain-fix audit's 168 probes, certain fixes 124 to 118 and
    applied texts carrying a new code 11 to 4; Linux x86-64 18 of 19 suites at
    0 failed, `probe` on the emulation's timeout and 24 and 0 on the trunk
    that carries the batch; Windows 973 tests and 19 suites at 0 failed. The
    item stays open for P5, L2, L3, L4, L5 and C3, each measured wider than
    the audit found (`scratchpad/lane-135c/next/first-pass.md`), P5 waiting
    on panel 185's ruling on design.md §4.15's premise.

    **2026-10-02, lane literals, an escape that names a character by its
    name, its code or its letter, or one written bare, takes no certain
    backslash** (the audit's L4, `\N{…}`, `\x{…}`, `\u41`, `\cA`, `\%`; and
    beside them `\o{…}`, `\U41`, `\C-a`, `\M-A`, `\E`, `\#`, `\@`, `\8`):
    repaired at `1d134613`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-02, lane literals, an uppercase base prefix is lowered with
    certainty only where a digit follows it or its digits already faulted**
    (the audit's L5, `0X`, `0B`, `0O` with no digit after them): repaired at
    `065b0fef`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-10-02, lane literals, the carriage return that begins a CRLF line
    end ends a literal, and is never written into it** (the audit's L2, and
    beside it a backslash before that line end): repaired at `f6fd5e48`,
    gated by its cases and the compiler's own tests; the net is owed at the
    batch's close. Left open: an `f"…"` literal open at a CRLF end with no
    backslash before it, whose loop is `lex_interp.hero:67`.

    **2026-10-02, lane literals, `'\'` is a backslash or an apostrophe, both
    guesses, and a later quote on its line leaves it whole** (the audit's
    L3; beside it `c == '\' || c == '"'` read as one wider literal):
    repaired at `8012b34c`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close, and so is the deletion of
    `fixedbugs-131-a-literal-that-never-closed-takes-its-line.applied`,
    which pinned the old certain fix.

    **2026-10-02, lane checker, a label that names nothing is renamed by its
    position only where the argument can stand there alone** (C3, and beside
    it two labels naming nothing in each other's places, which applied into
    a program that checks clean): repaired at `1ef5c8ca`, gated by its cases
    and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-02, lane arm, panel 185 R6: a spaced `-` opening an arm is
    deleted with certainty before a string, and is two guesses, the sign and
    the deletion, before an integer or a character** (P5; beside it, a tab
    setting the `-` apart, which the sign wrote into the margin): repaired at
    `b76f705e`, gated by its cases and the compiler's own tests; the net is
    owed at the batch's close.

    **2026-10-02, lane arm, beside P5 (panel 185's t1 and t2): a `-` before a
    case or `_` opening a pattern is told at the `-`, where it was told at the
    pattern after it, and deleted with certainty, since no `-` signs either**:
    repaired at `45aad99f`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-02, lane recovery-b7, a result written after `:` or `=>` is a
    certain `->` only where a body follows the head or none is owed** (the
    audit's P8, `function stub(): pass` and `=> value` over no body; beside
    them a stub whose word is a type, `(): i64` over nothing; certain still
    over an indented, tabbed or braced body and in an `extern` group):
    repaired at `2e0c53f5`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-02, lane recovery-b7, `int` inside an `extern` group is C's,
    and its certain swap writes `i32`** (the audit's L6, `t_int_extern_param`
    and `t_int_extern_field`; beside them a result, a callback's parameter and
    a field two levels under the head; outside a group `int` is `i64` still):
    repaired at `c61a1d04`, gated by its cases and the compiler's own tests;
    the net is owed at the batch's close.

    **2026-10-02, lane recovery-b7, an `f"…"` literal left open at a CRLF
    line end is told unterminated once, the line end never written into it**
    (the site the L2 line above left open, `lex_interp.hero`'s `scan_piece`;
    lane literals' probes `r2g` and `s2i`, and beside them a later piece, a
    doubled brace and a call): repaired at `664b283b`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

    **The last sites, 2026-10-02**, each told in a dated line above: P5, lane
    arm `b76f705e` (panel 185's R6, a spaced `-` before a string deleted with
    certainty, before an integer or a character two guesses) and `45aad99f`
    (a `-` before a case or `_`); P8, lane recovery-b7 `2e0c53f5` (a result
    after `:` or `=>` certain only where a body follows); L6, `c61a1d04`
    (`int` in an `extern` group is `i32`, C's `int`, measured 4 bytes here,
    and held at every platform gate by the run golden
    `ffi-pointee-at-the-headers-width`); and the CRLF site the L2 line left
    open, `664b283b`. With the earlier batches' P4, P6, P7, C9 (lane 135c),
    L2 to L5 (lane literals) and C3 (lane checker), no site the audit or a
    lane named is open. **Cases**: `check/fixedbugs-135-p5-*` (three),
    `fixedbugs-135-p8-a-result-after-a-colon-over-no-body`,
    `fixedbugs-135-l6-int-in-an-extern-group`, and
    `fixedbugs-135-crlf-an-f-literal-open-at-a-crlf-line-end` with its CR
    bytes kept by a `.gitattributes` line.

    **The gates.** Each repair by its cases and the compiler's own tests in
    its own lane, and the census against the compiler before it. **The
    round's one gate**, the first under the author's instruction of
    2026-10-02 (one gate per round, on this Mac): lanes ffi-macro, ci-probe,
    arm and recovery-b7 merged into `lane-round1002` (`c0f0f627`
    fast-forwarded, then `e8e4eaf6`, `76777908`, `ccba1df5`, one conflict, in
    item 135's body, every line of both lanes kept), closed at `e2d59fdb`:
    the seed regenerated once, 36,141,732 bytes, SHA-256 beginning
    `e6d520df7426008b`, its fixpoint by `cmp`; the compiler's own tests 1,018
    and the net's own 187, all passed; the full net, 25 suites three at a time
    and `cache` alone, 4,449 passed and 0 failed, one floor raised as the
    suite asked (`annotations`, 2,148 to 2,779), nothing blessed; the census
    of `check --brief` over 1,720 files, two exits moved, lane arm's two
    predicted ones, every moved output attributed to one lane by re-running
    the census with each lane's own compiler; the `--emit-c` census, 0 bytes
    of C moved; the site's build green. The trunk took the round at
    `415c0a14` on 2026-10-02, with only panel 186's sitting between, and
    `records` read 24 passed, 0 failed after it. **Owed before the push**:
    Linux arm64 and the Windows box, by the same instruction.
