# Panel 243's evidence

`evidence.tar.xz`, packed 2026-11-10 at the synthesis from a staging folder and read back byte for
byte (`diff -r` of the extracted archive against the staging folder, silent; each of the 686 source
files `cmp`-equal to its copy), scanned for keys before packing (none found). 690 files:

- every text file under 2 MiB of the folders the reports and the briefs cite, at its path relative
  to `.claude/worktrees/`, so a path a report names still names its file: `tar -xJf
  docs/panel/243-reports/evidence.tar.xz -C .claude/worktrees` puts them back. That is
  `p243-scratch/` (618 files: the seats' logs, scripts and probe programs, the compiler-engineer's
  route patches and `combined.py`, the ffi-pragmatist's `proto/prototype-step3.diff` and its door
  and danger probes, the critic's first-pass and `second/` probes, the spec-warden's priced
  wordings, the coordinator's decision draft and the `draft` and `file` logs of defects 2447 and
2448, whose drafts `file` removed); lane b29-boundary's Q3 shapes,
  `b29-scratch/boundary/2420/shapes/`, and Q2's reproducer, `b29-scratch/coord-curlwrap/undef/`
  (48); Q1's reproducer, `p242-scratch/coord-vararg/` (3); and the coordinator's reproductions of
  defects 2447 and 2448, `b30-scratch/repro/q3/` and `repro/mismatch/` (17);
- `evidence/compiler-engineer-worktree.diff` and `.status`, route (L+S) as the seat left it
  uncommitted in `.claude/worktrees/243-compiler-engineer`, against the base;
- `evidence/ffi-pragmatist-worktree.diff` and `.status`, Q2's step 3 as the seat left it in
  `.claude/worktrees/243-ffi-pragmatist`, against the base.

Left out: the seats' worktrees themselves (their diffs above), 134 binaries (the seats' route
compilers `heroes-r0`, `heroes-q2a`, `heroes-q2b`, `heroes-q3L`, `heroes-q3LS`, `heroes-proto`, and
every built probe), every `build/` cache, and the system's and Homebrew's headers, which the probes
read where they stand. The folders are the author's to remove.
