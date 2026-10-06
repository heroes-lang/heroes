---
kind: defect
area: records
milestone: none
filed: 2026-10-06
commit: self
github: none
---

- [x] **404 — three names Windows reserves reached the trunk, and the CI's Windows leg failed at its checkout** | batch 12's push, `6a03c488`, carried `docs/panel/178-reports/completeness-critic-work/w/ffi/nul.hero`, `docs/panel/178-reports/ffi-pragmatist-work/nul.hero` and `tests/golden/surface-fixtures/notext357/nul.txt`; Windows reserves `NUL` as a device's name with any extension, so the CI's `git checkout` refused the first of them (`error: invalid path`, exit 128) fourteen seconds into the job and no test ran there | `tests/harness/portable.hero`; `records/portable` in `tests/harness/suite_records.hero` · **class: blocking**

    **Origin:** the coordinator, 2026-10-06, reading CI run 37503830990's Windows x86-64 job after batch 12's push: `##[error]error: invalid path 'docs/panel/178-reports/completeness-critic-work/w/ffi/nul.hero'` at 17:28:50 UTC, on `windows-2025-vs2026` image `20260925.250` with `git version 2.55.0.windows.5`. Git stops at the first path it refuses, so it named one; `git ls-tree -r --name-only 6a03c488` filtered for the reserved names found the three.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): the one platform leg the CI runs on Windows ran nothing, so every Windows fact a push asserts went unjudged there, defect 238's half on clang 20.1.8 among them.

    **Measured beside it, 2026-10-06**: the same image and the same git read the tree of `d8a9913e` green on 2026-10-04 (run 37240247061, `Updating files` to 7983) and of `61e085ae` and `ca5fa51e` past their checkout on 2026-10-05, red there on defect 238 alone. None of the three paths was in those trees: panel 178's files were written on its lane at `d267b56a`, 2026-09-25, and reached the trunk at `3088caa7`, 2026-10-06 11:52, its merge into batch 12's round; the fixture was written at `31ddec2b`, defect 357's repair, on lane cli12 the evening before. Batch 12's Windows box leg did not see it, because that leg ships the tree by `tar` and never runs `git checkout`; whether the box's `tar` wrote those three files is **unrun**. Nothing in the net looked at a path's name before this.

## The repair

Repaired in the commit that files this issue, `lane-winpaths` from `6a03c488`. The three files are renamed by `git mv`, `nul.hero` to `nul-byte.hero` twice and `nul.txt` to `one-nul.txt`, and the five lines of `selfhost/cli/kept.hero` and `selfhost/cli/produce.hero` that name the fixture follow it; the records that cite the old names, defects 093 and 357 and panel 178's ffi-pragmatist report, each carry a dated line underneath saying where the file is now. **And the class is closed by an instrument**, `records/portable` (`tests/harness/portable.hero`): every path git names as the project's, tracked or untracked and not ignored, is refused when a part of it is a device name Windows reserves in any case and with any extension, ends in a space or a dot, or holds `<`, `>`, `:`, `"`, `|`, `?`, `*`, a backslash or a control character. Its three unit tests are the net's own, 286 to 289, all passed; `records` read 28 passed, 0 failed on the lane's tree, and 27 passed, 1 failed with an untracked `tests/con.txt` planted, naming `con.txt`.
