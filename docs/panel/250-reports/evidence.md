# Panel 250's evidence

`evidence.tar.xz` beside this note holds the text files of the sitting's scratch folder,
`.claude/worktrees/p250-scratch/`, as they stood on 2026-11-15 at 09:22, after the grading of the
blind seat's 24 sessions and the completeness critic's second pass over the reports: the seats'
probes, logs, tables and notes, the grading setup's programs and runs, every route's diff and every
blind session's folder, each under 2 MiB, **1,631 files and 35,598,947 bytes**, plus one manifest;
2,327,144 bytes after `xz -9e` (`sha256` `06d629442f1e4772...`). Packed by panel 246's
`synth/pack.py`, its rules unchanged, which is in the archive. Two of its lines differ from the copy
panel 247 packed with: its header, and the folder it writes `DUPLICATES.tsv` into, `synth-work/`
instead of the scratch root, because this packer wrote nowhere else in the folder. The manifest went
from there to the archive's root.

**One copy of each content.** 7,231 more files of the folder, 42,720,219 bytes, are byte copies of a
file the archive keeps (2,060 of them under `grade/runs/`, and the 24 blind sessions' identical
briefs, tasks and specifications, 68 files, among them). Each is named in `DUPLICATES.tsv` at the
archive's root beside the kept path that holds the same bytes, so the folder can be rebuilt whole.

**Left out**, counted by `pack.py`:
- **the copies of the repository and of its folders**: 7 whole trees, 9 copies of `runtime/`, 11 of
  `selfhost/` and 3 of `packages/` (`synth/copies.txt`). Each is kept instead as a unified diff
  against the base, `48f53b3437c6cd46aba5923f4f4780ac7b347283`, or against the base's folder for a
  folder copy, in `synth/diffs/`: 30 diffs (`build/`, `tmp`, `.git`, the `zig` caches, objects and
  `.dSYM` folders left out of each; a binary file named by `diff`'s one line). The `selfhost/` and
  `packages/` copies are new to this list. Each is a whole copy of that folder of the repository,
  no base file missing. Kept file by file, they would have stored the first `selfhost/` copy whole
  and hidden each route's change among its 849 files. The diffs of the four untouched trees at the
  base read two lines each, the compilers `heroes` and `heroes-next`. The 13 copies of `runtime/`,
  `packages/` and `selfhost/` identical to the base give empty diffs. `briefs/base/tree` and
  `critic/tree` are byte copies of `722ae76bf`, the commit the sitting was first written over
  (`250-briefs/00-shared.md`), plus the same two compilers, as measured against that commit. So
  their diffs, 10,250,176 and 10,237,126 bytes and kept although over 2 MiB, carry the changes from
  `722ae76bf` to the base, reversed. The partial trees the probes built (`briefs/partial/`,
  `briefs/partial48/`, `critic/partial/`, `250-spec-warden/probe/`, and the compiler-engineer's
  `unreached/`, `pruned/`, `mrun/`, `fals2/` and `addfirst/`) hold some of `examples/`, `packages/`
  and `tests/` and no `seed/heroes.c`, `selfhost/` or `runtime/`. They are kept file by file;
- **53 binary files**, 8,029,957 bytes (a NUL byte, or an executable, archive or PDF header);
- **11 files of 2 MiB or more**, 306,333,864 bytes. Nine are compilers:
  `250-compiler-engineer/routeA/ce-a2` and `ce-a3`, `250-compiler-engineer/routeM/ce-m2` and
  `ce-m2-v1`, `250-ffi-pragmatist/linux/heroes-next-linux`, `grade/{m,w}/heroes` and
  `critic/p2/ergo/grade/{m,w}/heroes`. The other two hold one emitted C text twice,
  `250-compiler-engineer/a2.c` and `a3.c`, the same 35,574,612 bytes. `pack.py` counted 13 files
  at 326,821,166 bytes: the other two are the first-base diffs above, archived with `synth/`;
- 180 `build/` folders and 9 `tmp` folders.

**Read back and scanned.** Read back into `synth-check/r1/`, the archive gives 1,632 files, each
`cmp`-identical to its source. Every one was scanned for keys and credentials: an
`ANTHROPIC_API_KEY=` with a value, `sk-ant-`, `ghp_`, `github_pat_`, `gho_`, a private key's
`BEGIN` line, an `Authorization:` header with a value, then a wider sweep for `x-api-key`,
`Bearer`, `AKIA` and `sk-` shapes. There were three matches, and none is a credential. `run.sh:12`
tests that `${ANTHROPIC_API_KEY:-}` is not empty. `250-compiler-engineer/mrun/tests/harness/thesis_wire.hero:15`
and `:83`, a file byte-identical to the base's, are a comment and a curl header template that
names a variable. The wider sweep found only that file's `x-api-key` lines, templates of the same
kind. The blind sessions' 24 `run.json`, their 24 `run.err` (empty) and `run.log`, which gives the
key's length alone, hold none. No file was removed.

No issue draft and no decision draft stood at the folder's root when it was packed (no `defects/`,
no `decision-*` file).
