- [x] **068 — a record rewritten under C's held address, which no sanitizer can see** | **closed 2026-09-21**, M-declared-extents step 24 | **The lend is refused where it is made, one line before the assignment that corrupted it**: `k_register` says no `lent`, so `lend_kept` fires at `s.name.ptr()` and the write two lines below never reaches C's copy of the address | `selfhost/check/lend_landing.hero` · `tests/golden/check/fixedbugs-a-record-rewritten-under-a-held-address.hero` | 171

    **Origin:** panel 167's completeness critic, 2026-09-20, separating defect
    066 into the two defects it is.

    **How it differs from 066, and why the difference matters.** 066 is the frame
    DYING under a held address, which a sanitizer can see when the retainer is
    instrumented. Here nothing is freed and no frame dies: the storage is alive
    and its contents change. **No sanitizer can ever see it**, on any platform,
    because no memory rule is broken — only the program's meaning is.

    **What is owed.** Nothing decides this one from a declaration, a header or an
    argument: panel 167 measured that retention is per-call and that a run-time
    `bool` can pick it. It is filed so that the route adopted for 066 is judged
    against it too, and so that a later sitting does not discover it as new.

    **THE LAYER WAS WRONG, and panel 168's completeness critic found it**,
    2026-09-20. This entry files 068 with 066 as *foreign retention*, where panel
    167's historian proved static enforcement exists in none of ten ecosystems.
    That is true of 066 and **false of 068**: in the reproducer the wrong answer
    is produced by a **Heroes assignment**, two lines below the lend, in the same
    function, to the very binding whose field's address was lent. No C code
    participates in the corruption; C only observes it. That is **caller-side**,
    which is exactly where the historian found static enforcement does exist
    everywhere, and `field_lend_escapes` and `field_lend_needs_a_place`
    (`selfhost/check/lending.hero:218-228`) already relate a lend to its root
    binding. **068 has never been searched at the layer where its defect
    happens**, and *"nothing decides this one from a declaration, a header or an
    argument"* above is an argument about the C side of a line the Heroes side
    writes.

    ## The repair

    Defect 066's, and the entry moved here from `docs/work/DEFECTS.md` says why
    it is the same one: under *a lend lives for its call and no longer*, the
    Heroes assignment two lines below the lend is LEGAL and C is the one
    retaining. What was owed was a way to declare that a parameter retains and
    a refusal of a lend that reaches one, and `lent` with the pessimistic
    default is both. The caller-side rule panel 168's critic sketched — *a
    binding whose field's address has been lent is not re-assigned in this
    function* — was not needed: it would refuse a program that is sound the
    moment the parameter says `lent`, which is what `k_sum` in the golden
    shows quiet.

    ## The measurements

    | | before | after |
    |---|---|---|
    | the reproducer, `k_register(p: s.name.ptr(), n: 8)` then `s @ KSlot(...)` | `check` **0**, prints 72 then 1 at exit 0, **zero** AddressSanitizer lines on any platform | **`check` 1, `error[lend_kept]`** at the lend |
    | the same lend into `k_sum(p: ptr counted_by n lent, n: i64)`, with the rewrite after it | `check` 0 | **`check` 0, unchanged**: C kept nothing, so the rewrite is nobody's defect |

    ## The gates

    Defect 066's, in the same commit.
