# Panel 244's evidence

`evidence.tar.xz`, packed 2026-11-10 at the synthesis from a staging folder and read back byte for
byte (`diff -r` of the extracted archive against the staging folder, silent; each of the 1,305
source files `cmp`-equal to its copy), scanned for keys before packing (none found). 1,307 files:

- every text file under 2 MiB of the folders the reports and the briefs cite, at its path relative
  to `.claude/worktrees/`, so a path a report names still names its file: `tar -xJf
  docs/panel/244-reports/evidence.tar.xz -C .claude/worktrees` puts them back. That is
  `p244-scratch/` (1,257 files: the compiler-engineer's 317, its `part*-*.txt` runs, its probes
  `c*` and `git/`, and `qstub/qstub.diff`; the ffi-pragmatist's 302, its probes `e1/` to `e13/`, the
  C of feature 2358 in `c2358/` and its drafts A, B and C; the spec-warden's 174, its probes and
  `drafts/` with `S1f.diff` and the price tables; the critic's 456, its first-pass and `second/`
  probes with the price folder `second/price/`, whose `set-S1c2_d03_C2_d08d.md` is the specification
  R1 lands; and the coordinator's 8, the `draft` and `file` logs of defects 2458 and 2459 and feature
  2460, whose drafts `file` removed, and the comment on 2446), and the coordinator's reproductions of
  defects 2442, 2443 and 2446, `b30-scratch/repro/b5/`, `repro/b10/` and `repro/d2/` (48);
- `evidence/compiler-engineer-worktree.diff` and `.status`, the Q-stub prototype as the seat left
  it uncommitted in `.claude/worktrees/244-compiler-engineer` against the base. The other seats'
  worktrees hold no change.

Left out: the seats' worktrees themselves, 6 binaries, every `build/` cache, and the system's and
Homebrew's headers, which the probes read where they stand. The folders are the author's to
remove.
