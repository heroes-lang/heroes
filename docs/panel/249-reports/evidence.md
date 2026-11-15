# Panel 249's evidence

`evidence.tar.xz` beside this note holds the text files of the sitting's scratch folder,
`.claude/worktrees/p249-scratch/`, as they stood on 2026-11-15 at 09:16, after the critic's review of
the reports and the decision's draft: every text file under 2 MiB of the two seats' folders
(`249-compiler-engineer/` 2,092 files, `249-ffi-pragmatist/` 722), the critic's (`critic/` 65) and the
coordinator's `briefs/` (335), with the packing's own `synth/` (17), **3,231 files and 20,489,648
bytes**, plus one manifest; 1,748,096 bytes after `xz -9e` (`sha256`
`877ee1445d651338a42add7b085acbc45384c1ebe6900319c393847b699bf582`). Packed by panel 246's
`synth/pack.py`, its rules unchanged, which is in the archive. Read back into a fresh folder, every
one of the 3,232 entries is `cmp`-equal to its source, 21,021,733 bytes, and the folder holds nothing
the list does not name.

**One copy of each content.** 3,417 more files of the folder, 24,232,752 bytes, are byte copies of a
file the archive keeps (3,161 of them under the three `inputs/` folders of `249-compiler-engineer/`,
`249-ffi-pragmatist/` and `briefs/`, which hold the same files; the seats' Go homes, among others),
each named in `DUPLICATES.tsv` at the archive's root beside the kept path that holds the same bytes,
so the folder can be rebuilt whole. `synth/pack.py` is one of them, the bytes of
`249-compiler-engineer/inputs/ev247/synth/pack.py`, the seats' copy of panel 247's archive; it is
packed at `synth/` as well, beside `pack.py`'s list, so the packer stands where this note says.

**Left out**, counted by `pack.py`:
- **the copies of the repository and of its `runtime/`**, 5 whole trees and 10 runtime copies
  (`synth/copies.txt`): each is kept instead as a unified diff against the base,
  `722ae76bf693bebdb7a5cca37052ed92646753c0`, its `runtime/` for a runtime copy, in `synth/diffs/`,
  15 diffs (`build/`, `tmp`, `.git`, the `zig` caches, objects and `.dSYM` folders left out of each;
  a binary file named by `diff`'s one line). The diffs of the three untouched trees
  (`249-ffi-pragmatist/tree`, `briefs/base/tree`, `critic/reports/tree`) read two lines each, their
  `heroes` and `heroes-next`; `249-compiler-engineer/base-rt/runtime`'s is empty, the base's runtime
  byte for byte; the three copies of panel 247's `critic2/work/rep/runtime-c`, in the seats'
  `inputs/ev247/`, read against this base, so they carry every difference between that copy and it
  beside their own, 4,509 lines each (its `parts/lock.c`, `parts/sigdoor.c` and `parts/sigguard.c`
  absent, as they were from 247's folder);
- **714 binary files**, 135,993,792 bytes (a NUL byte, or an executable, archive or PDF header);
- **10 files of 2 MiB or more**, 71,153,424 bytes, every one a binary, an `ar` archive: six Go
  static libraries (`249-compiler-engineer/go/answer/libgoanswer.a`, `.../go/census/libgocensus.a`,
  `249-ffi-pragmatist/c/go/{,golib/,golibt/}libfpgo.a`, `briefs/go/src/libgoanswer.a`) and four
  entries of the Go build cache (`gocache/62/6237d415...-d` in the three Go homes,
  `gocache/d4/d4c6296c...-d` in the ffi-pragmatist's);
- 19 `build/` folders and 6 `tmp` folders;
- the two files of the sitting's decision draft at the folder's root, `decision-249.md` and
  `decision-249.draft.out`, since an issue keeps no copy of itself beside it.

Not counted by `pack.py`: `evidence-tool/`, the coordinator's extraction of the packer for this
packing, one file byte-identical to `synth/pack.py`, which a wrapper took out of the walk's first
level before running `pack.py`'s own file.

**Scanned for keys**, every kept file, for an `ANTHROPIC_API_KEY=` with a value, `sk-ant-`, `ghp_`,
`github_pat_`, `gho_`, a private key's `BEGIN` line and an `Authorization:` header with a value: four
lines match and none holds a credential. Line 94 of `249-compiler-engineer/ci-132585763737.log` and
of `briefs/ci/job-132377462166.log` is a CI run's `git config ... extraheader` whose
`AUTHORIZATION: basic` is followed by GitHub's mask, `***`; line 25 of
`249-compiler-engineer/inputs/ev247/tools/blind-one.sh` and of `blind-one3.sh` is
`ANTHROPIC_API_KEY="$ANTHROPIC_API_KEY"`, an expansion. Nothing was removed.

**What was already gone.** Every path of the folder the five reports cite is in it, searched by the
folder's names and the reports' `F/` and `R/`; `runtime/parts/siginstall.c`, which the
compiler-engineer's candidate adds, is in `synth/diffs/249-compiler-engineer_tree.diff`. What ran
outside the folder left nothing else: the seats' and the critic's Docker containers left the logs
their reports cite in the folder, the compiler-engineer's and the critic's through a mount and run
with `--rm`, and none of them stands on the machine (`docker ps -a`, 2026-11-15: no `ce-249`,
`fp-249` or `critic-249` container, running or stopped); the Windows box was off and nothing ran
there; the historian's seat read pages on the web, each URL in its report, and had no folder.
