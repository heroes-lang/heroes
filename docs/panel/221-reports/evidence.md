# Panel 221: the evidence its seats' working folders held

The reports and briefs in this folder cite the seats' working folders: a
session's scratch folder under `/private/tmp`, written `<scratchpad>/…`, which
the system empties at a reboot, and folders under `.claude/worktrees/`, which
git never saw. Kept here on 2026-10-18, as 212's and 217's are, by the author's
instruction of 2026-10-12, meant as: *put the evidence in the panel's result,
then throw the folders away* (task 2135, M-readonly-lend).

**What is kept** is `evidence.tar.xz`, beside this file: 1,846 files, every text
file under 2 MiB of the folders below, a scratch entry's at its path relative to
the scratch folder and a worktree folder's at its path relative to the
repository's root, so a path a report cites still names its file:
`tar -xJf docs/panel/221-reports/evidence.tar.xz -C <folder>` puts them back
under `<folder>`. Each file was read back byte for byte against its original on
2026-10-18, and none holds a key or a token (scanned for Anthropic, GitHub and
AWS keys and for private keys). The entries, with the files each keeps:

- `<scratchpad>/221-ce/`, 1,720;
- `<scratchpad>/221-ffi/`, 126.

**What is not kept**: every `build/`, `.git` and `node_modules`; 309 binaries,
the compilers and programs the seats built among them; 3 files of 2 MiB or more.

**Cited and already gone on 2026-10-18**, found in no scratch folder or
worktree of this machine that day (a name ending in `-` is a family of
entries, none present):

- `.claude/worktrees/221-`;
- `.claude/worktrees/221-ce-nobranch`;
- `.claude/worktrees/221-ce-s2`;
- `.claude/worktrees/221-compiler-engineer`;
- `.claude/worktrees/221-ffi-pragmatist`;
- `.claude/worktrees/lane-land215`.
