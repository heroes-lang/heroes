# Defect 141 closed: check --apply hands back the author's bytes with its fixes, a last line left open included

- [x] **141 — `check --apply` writes bytes no fix proposed: the newline the loader adds to a root file's open last line** | `fn main()` over `    print(1)` with no final newline, 22 bytes: `check --apply` prints 29 where its one certain fix makes 28, the last byte a `\n`; `--apply --in-place` writes it into the author's file | `source.from_files` · `source_extent.user_text` · `selfhost/cli/check.hero` (what `--apply` prints) · **closed 2026-10-02**

    **Origin:** lane 136 at its close, 2026-09-30, on the trunk's compiler as
    on its own (`scratchpad/lane-136/r2/nl/`); reproduced by the coordinator
    at 00:05 on 2026-10-01 on `3cc3b553` (`scratchpad/p184/newdef/certain.hero`,
    `od -c` of the output). Beside it and to be judged with it: a refused
    file with no fix prints its text with the added newline, and a clean file
    prints nothing.

    **Why it is a defect.** What `--apply` writes must be exactly the fixes
    it applied, the rule defect 137's repair holds; here a byte no fix
    proposed reaches the author's file.

    **2026-10-01, lane 135c, what `check --apply` hands back is the author's
    bytes and its fixes, a last line left open included** (beside it, found
    in the lane's first pass: a stray `\r` ending a file or a module read as a
    line ending, a CRLF file's bare `\n`, nothing printed for a clean file
    and the holes report for one with a hole): repaired at `fa64c7da`, gated
    by its cases and the compiler's own tests; the net is owed at the batch's
    close.

    **The repair**, lane 135c, `fa64c7da`. **The class**: a byte the author
    never wrote reached what `check --apply` prints and writes, by two causes:
    `source.from_files` closed a file's open last line with a newline of its
    own, inside the file's text, and `cli/io.print_artifact` printed a text
    with no final newline through `print`, which adds one. Now the newline
    joining two files belongs to neither, a file's own text is its author's
    bytes, and an artifact is printed byte for byte, through the runtime's
    existing `hero_print_str`. **Beside it, repaired with it**, the shapes the
    entry asked to be judged with it: a stray `\r` ending a file or a module
    was read as a line ending (`check` passed what `fmt` refused), a CRLF file
    got a bare `\n`, and `--apply` printed nothing for a clean file (so
    `check --apply > file` emptied it) and the holes report for a file with a
    hole; `fmt` writing the final newline is the canonical form's by design
    (§4.15), not this defect.

    **Cases**: four `check/fixedbugs-141-*`, two with a `.fixed`; the fixture
    `surface-fixtures/apply141/` and five `suite_surface` rows (its table's
    count raised 151 to 156, dated); `source_extent`'s test over seven
    last-line shapes. The open-variant census of the 1,448 tracked `.hero`
    files, every final newline stripped: `--apply` equals the closed file's
    output less its last newline in all of them, and `check` moves in five,
    each an end-of-file caret moving from a line the file lacks to the end of
    its last line; on the tree as it is, `check` moves in none.

    **The gates.** Each repair by its cases and the compiler's own tests.
    **The batch gate**, lane 135c's closing commit `8025ad15` (with P6, C9
    and P4 of defect 135, which stays open), the trunk merged at `155c750a`
    and `cc97f96d`: the seed regenerated once, the fixpoint by `cmp`; the
    compiler's own tests 973 and the net's own 184, all passed; the full
    net, 25 suites four at a time and `cache` alone, 4,207 passed and 0
    failed, none red in the parallel pass; `tests/emission` unchanged; the
    census over 1,509 files, both arms, 3 exits moved (the three
    stray-carriage-return cases of this defect, 0 to 1) and 6 outputs, every
    one the batch's own. The trunk fast-forwarded to `8025ad15` at 21:47 on
    2026-10-01. **Linux x86-64** on `8025ad15`: the compiler's own tests 973,
    all passed, and 18 of the 19 suites 0 failed; `probe` read 21 and 1 on
    its `selfhost, multi` row's 120 s timeout under emulation, the row lane
    flow had timed the same on both trees that evening. It did not stand on
    the trunk that carries the batch: lane recovery-b5's leg on `a8b04ea8`,
    which holds `8025ad15`, read `probe` 24 and 0 with no container beside
    it, and its 19 suites at 0 failed. **Windows**, the box, on `b39ae3d5`
    (this batch with emit's and flow's): 973 tests, all passed, and the 19
    suites 0 failed, exit 0 at 23:06 on 2026-10-01.
