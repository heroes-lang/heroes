# Panel 209: the evidence its seats' working folders held

The reports and briefs in this folder cite their seats' working folders under
`.claude/worktrees/`, which git ignores, so they were never published. The
author had them removed on 2026-10-12, meant as: *put the evidence in the
panel's result, then throw the folders away*.

**What is kept** is `evidence.tar.xz`, beside this file: 2 files, every small
text file of the cited folders (outputs, probes, census rows, diffs, programs),
at its path relative to `.claude/worktrees/`, so a path a report cites still names
its file: `tar -xJf docs/panel/209-reports/evidence.tar.xz -C .claude/worktrees` puts
them back. The folders it holds, removed 2026-10-12:

- `scratch-b15/209-coordinator/` (removed 2026-10-12)

**What is not kept**: the copies of the repository the seats worked in and their
builds, regenerable from git at the commit each report names.

**The 24 blind readers' reports** the briefs cite under `scratch-b15/209-blind/`
(removed before 2026-10-12) stand in this folder as `llm-ergonomist-<task>.md`, one
per task and variant; all 24 were found there.

**Cited and already removed before 2026-10-12**, the seats' worktrees taken away
at the sitting's close, their reports being the files of this folder:

- `scratch-b15/209-compiler-engineer/` (absent 2026-10-12)
- `scratch-b15/209-critic/` (absent 2026-10-12)
- `scratch-b15/209-ffi-pragmatist/` (absent 2026-10-12)
- `scratch-b15/209-spec-warden/` (absent 2026-10-12)
