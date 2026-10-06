---
kind: task
area: records
milestone: M-issue-files
filed: 2026-10-06
commit: none
github: none
---

- [ ] **M-issue-files** | panel 178 (2026-09-25, *the rest is zero where it is written*, M-buildable-structs' sitting) and what it filed live only on the branch `lane-panel-178`, never merged: its sitting, briefs and reports, its four defects 091 to 094 (the trunk's defect numbers go from 090 to 095) and row 63's description (`e597cfcd`); its ratification is in no list, so row 63 cannot open on it. It is sat again on the trunk as it is after batch 12, every premise and measurement of 178 re-run there, and then brought in: the old sitting kept as the record it is, the new one's resolution and its ratification issue, each of 091 to 094 re-run and filed by what it is today | `git log lane-panel-178`, `git show lane-panel-178:docs/panel/178-the-rest-is-zero-where-it-is-written-and-c-says-which-value-is-valid.md` · `docs/ROADMAP.md` row 63 · `.claude/skills/panel/SKILL.md`

    **Origin:** the author's doubt of 2026-10-06, meant as: *I suspect what you are doing is the roadmap's next step, the one about arrays; check*. Checked by the coordinator from 11:12: the next step, row 63, builds a C struct with long fixed arrays by naming the fields that matter and ending with `rest: zero`; batch 12's lane ir12 repairs the language's own arrays, `[T]` (defects 381 and 382), and the two meet only in `selfhost/emit/container.hero`, different functions. The check found panel 178 off the trunk: `git merge-base --is-ancestor lane-panel-178 main` false, no `issues/` file names `panel 178`, no defect file numbered 091 to 094.

    **The author's answer**, 2026-10-06, meant as: *I agree with your proposal, but there have been a thousand changes since that sitting, so the panel should perhaps be regenerated, or at least updated, and then you can bring it in, so we lose nothing.* Taken after batch 12 closes, by the coordinator of this session.

    **The blind seat's paid run is approved**, 2026-10-06, the author's words meant as: *OK to the blind reader with `claude -p`; for now I am not paying for it anyway*. It runs as `.claude/skills/panel/SKILL.md` says, one fresh session per task outside the repository, its budget bound named in its brief.

    **The author's notes for the step**, given 2026-10-06 as written, from an earlier attempt on an older HEAD; every one is re-measured before it is relied on, and each goes into the new sitting's briefs as a question rather than a premise:

    - Sample the real headers, not the repository. Run clang's AST census on all three legs. Last time: 42 public structs on Darwin, 29 on Linux arm64, 23 on Linux x86-64 with an array longer than 8; the repository's longest was 8.
    - There are three kinds of array: bytes the program never touches, buffers C fills, and bytes the program writes. The same struct differs by platform (`utsname` is 5×[256] on Darwin, 6×[65] on Linux).
    - The one form that paid for itself: `rest: zero` at the end of a group record's construction. Every field it does not name is zero. It applies to group records only, and `missing_fields` stays as it is everywhere else.
    - Promise nothing about padding. C11 §6.2.6.1p6 does not guarantee it, and on x86-64 padding is lost when a record is passed by value (MemorySanitizer, with a control).
    - Treat zero as bytes, never as a claim of validity. A "zero is valid" mark on the record guards a spelling only, because `partial` already zeroes undeclared fields. Zero-validity also differs by platform: a zeroed mutex locks on Linux and returns EINVAL on Darwin.
    - Let C say which value is valid, through the header's own `*_INITIALIZER` bound as a group constant. That needs a struct-initialiser constant to compile, which last time failed with an internal error.
    - Text into a fixed field already works: `memcpy` into `field.ptr()`, declared `ptr counted_by <n> lent`. A `to_fixed()` value form would need storage for `T[N]` (see `emit/ctype.hero`).
    - `[x; N]` writes a platform's length into every construction.
    - Refuse a zero default for every type: an all-zero `str` aborts, and a forgotten field becomes a silent zero.
    - The largest first-try failure among blind readers was C's `char` written as `u8` (22 of 50). § 13 should say it is `i8`.
    - Candidate defects to re-check: an element write into a fixed-array field is check 0, then dies in `hero_unreachable`; a group record lent to `void *` with a C count overflows the stack; a `str` with a zero byte from `read_file` reaches C cut short through `.cstr()`; a struct-initialiser `constant` stops the build.
    - `missing_fields` had no test: land a golden for it before any construction form. The `rest: zero` prototype moved three DECIDED layout rows (`ast.hero`, `check/walk.hero`, `print/fmt.hero`); name them first.
    - Judge readers by compiling their programs, not by stated beliefs. Price spec text with `--refresh` on the base you land on.
