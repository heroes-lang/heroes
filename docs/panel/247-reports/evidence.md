# Panel 247's evidence

`evidence.tar.xz` beside this note holds the text files of the sitting's scratch folder,
`.claude/worktrees/p247-scratch/`, as they stood on 2026-11-13 at 12:12, after the critic's second
pass and the blind seat's third follow-up: every route's diff, probe program, header, driver, log,
table, blind session's folder and saved output under 2 MiB, **1,727 files and 12,183,478 bytes**,
plus one manifest; 754,656 bytes after `xz -9e` (`sha256` `509669bf2b6149c1...`). Packed by
panel 246's `synth/pack.py`, its rules unchanged, which is in the archive.

**One copy of each content.** 1,143 more files of the folder, 6,289,970 bytes, are byte copies of a
file the archive keeps (the 96 blind sessions' identical briefs and specifications, among them), each
named in `DUPLICATES.tsv` at the archive's root beside the kept path that holds the same bytes, so the
folder can be rebuilt whole.

**Left out**, counted by `pack.py`:
- **the copies of the repository and of its `runtime/`**, 11 whole trees and 14 runtime copies
  (`synth/copies.txt`): each is kept instead as a unified diff against the base,
  `ae5e2cedddee93e0d53715a5b5ae6d7833c569df`, its `runtime/` for a runtime copy, in `synth/diffs/`,
  25 diffs (`build/`, `tmp`, `.git`, the `zig` caches, objects and `.dSYM` folders left out of each;
  a binary file named by `diff`'s one line). The diffs of the two untouched bases read two lines each;
  the copies of the lane's runtime at `62ce17563` read against the base, so they carry batch 34's
  changes beside their own;
- **2,215 binary files**, 171,960,840 bytes (a NUL byte, or an executable, archive or PDF header);
- **9 files of 2 MiB or more**, 189,637,458 bytes, every one a binary: seven compilers
  (`grade/{today,dg,c,f}/heroes`, `critic2/work/{today,E}/heroes`, `critic2/work/rep/heroes-c`) and
  two `libmingw32.lib` of the ffi-pragmatist's `zig` cache;
- 250 `build/` folders and 10 `tmp` folders.

The issue drafts of the sitting were never in the folder (`.claude/worktrees/b35-scratch/`), since an
issue keeps no copy of itself beside it.
