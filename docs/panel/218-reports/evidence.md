# Panel 218: the evidence its seats' working folders held

The reports and briefs in this folder cite the seats' working folders: a
session's scratch folder under `/private/tmp`, written `<scratchpad>/…`, which
the system empties at a reboot, and folders under `.claude/worktrees/`, which
git never saw. Kept here on 2026-10-18, as 212's and 217's are, by the author's
instruction of 2026-10-12, meant as: *put the evidence in the panel's result,
then throw the folders away* (task 2135, M-readonly-lend).

**What is kept** is `evidence.tar.xz`, beside this file: 14,010 files, every
text file under 2 MiB of the folders below, a scratch entry's at its path
relative to the scratch folder and a worktree folder's at its path relative to
the repository's root, so a path a report cites still names its file:
`tar -xJf docs/panel/218-reports/evidence.tar.xz -C <folder>` puts them back
under `<folder>`. Each file was read back byte for byte against its original on
2026-10-18, and none holds a key or a token (scanned for Anthropic, GitHub and
AWS keys and for private keys). The entries, with the files each keeps:

- `<scratchpad>/218-ce/`, 3,445;
- `<scratchpad>/218-critic/`, 3,305;
- `<scratchpad>/218-ffi/`, 7,201;
- `<scratchpad>/win216/`, 59.

**What is not kept**: the copies of the repository, 13,863 files at paths git
tracks, regenerable from git at the commit each report names; every `build/`,
`.git` and `node_modules`; 5,189 binaries, the compilers and programs the seats
built among them; 68 files of 2 MiB or more.

**Cited and already gone on 2026-10-18**, found in no scratch folder or
worktree of this machine that day (a name ending in `-` is a family of
entries, none present):

- `.claude/worktrees/218-`;
- `.claude/worktrees/218-compiler-engineer`;
- `.claude/worktrees/218-critic`;
- `.claude/worktrees/218-ffi-pragmatist`.
