# Panel 212: the evidence its seats' working folders held

The reports and briefs in this folder cite their seats' working folders under
`.claude/worktrees/`, which git ignores, so they were never published. The
author had them removed on 2026-10-12, meant as: *put the evidence in the
panel's result, then throw the folders away*.

**What is kept** is `evidence.tar.xz`, beside this file: 9 files, every small
text file of the cited folders (outputs, probes, census rows, diffs, programs),
at its path relative to `.claude/worktrees/`, so a path a report cites still names
its file: `tar -xJf docs/panel/212-reports/evidence.tar.xz -C .claude/worktrees` puts
them back. The folders it holds, removed 2026-10-12:

- `scratch-gate-close/bodies-before.jsonl/` (removed 2026-10-12)
- `scratch-panel-212/ffi-pragmatist/` (removed 2026-10-12)

**What is not kept**: the copies of the repository the seats worked in and their
builds, regenerable from git at the commit each report names.

It also holds `scratch-gate-close/bodies-before.jsonl` (removed 2026-10-12): every
issue body as it stood before the link pass of 2026-10-12, R9 item 6, the way
back if a body ever has to be restored.

**Cited and already removed before 2026-10-12**, the seats' worktrees taken away
at the sitting's close, their reports being the files of this folder:

- `scratch-panel-212/critic2/` (absent 2026-10-12)
