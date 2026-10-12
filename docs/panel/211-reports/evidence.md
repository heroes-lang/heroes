# Panel 211: the evidence its seats' working folders held

The reports and briefs in this folder cite their seats' working folders under
`.claude/worktrees/`, which git ignores, so they were never published. The
author had them removed on 2026-10-12, meant as: *put the evidence in the
panel's result, then throw the folders away*.

**What is kept** is `evidence.tar.xz`, beside this file: 3503 files, every small
text file of the cited folders (outputs, probes, census rows, diffs, programs),
at its path relative to `.claude/worktrees/`, so a path a report cites still names
its file: `tar -xJf docs/panel/211-reports/evidence.tar.xz -C .claude/worktrees` puts
them back. The folders it holds, removed 2026-10-12:

- `scratch-b15/211-compiler-engineer/` (removed 2026-10-12)
- `scratch-b15/211-critic/` (removed 2026-10-12)
- `scratch-b15/lane-inferred-cell/` (removed 2026-10-12)

**What is not kept**: the copies of the repository the seats worked in and their
builds, regenerable from git at the commit each report names.
