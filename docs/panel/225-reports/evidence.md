# Panel 225: the evidence its seats' working folders held

The reports and briefs in this folder cite the seats' working folders, folders
under `.claude/worktrees/` which git never saw, and the coordinator's own
measurements, in a session's scratch folder under `/private/tmp`, written
`<scratchpad>/…`, which the system empties at a reboot. Kept here on
2026-10-19, as 213's to 224's are, by the author's instruction of 2026-10-12,
meant as: *put the evidence in the panel's result, then throw the folders away*
(task 2135, M-readonly-lend).

**What is kept** is `evidence.tar.xz`, beside this file: 3,063 files, every text
file under 2 MiB of the folders below, a worktree folder's at its path relative
to the repository's root and a scratch entry's at its path under
`<scratchpad>/`, so a path a report cites still names its file:
`tar -xJf docs/panel/225-reports/evidence.tar.xz -C <folder>` puts them back
under `<folder>`. Each file was read back byte for byte against its original on
2026-10-19, and none holds a key or a token (scanned for Anthropic, GitHub and
AWS keys and for private keys). The entries, with the files each keeps:

- `.claude/worktrees/225-compiler-engineer/`, 2,251, most of them the fresh
  copies its table and its count of clang runs built every program in
  (`ce-work/table-work/`, 933; `ce-work/count/`, 853);
- `.claude/worktrees/225-critic/`, 186;
- `.claude/worktrees/225-ffi-pragmatist/`, 578;
- `.claude/worktrees/scratch-clean-list/repro/` and `repro-s/`, the
  reproducers the shared brief names, 9 and 7;
- `<scratchpad>/p225g/` (`marks.c`, the pragma measurement), `basefile/`,
  `mainmacro/`, `mainmacro2/` (defect 2146), `stale/`, `stale2/`, `stale3/` and
  `stalec2/` (defect 2147), 1, 3, 6, 5, 7, 4, 3 and 3: the coordinator's
  measurements in `coordinator.md`.

**What is not kept**: the copies of the repository, 37,361 files at paths git
tracks, regenerable from git at `7b97b9438`, the commit each seat's copy was
made from (each seat's own diff is kept, `ce-work/union.diff`, `ce-work/g.diff`
and `critic-work/pass2/ce.diff` and `fp.diff`); every `build/`, `.git` and
`node_modules`, 268 folders; 167 binaries, the compilers and programs the seats
built among them; 21 files of 2 MiB or more. And the Windows box's folders,
`/c/w/b225`, `b225r`, `b225w` and `p225g`, whose programs are the
compiler-engineer's `ce-work/union-src/` and whose output `coordinator.md`
quotes whole.
