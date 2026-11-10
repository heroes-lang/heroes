# Panel 242's evidence

`evidence.tar.xz`, packed 2026-11-10 at the synthesis from a staging folder and read back byte for
byte (`diff -r` of the extracted archive against the staging folder, silent; each of the 204 source
files `cmp`-equal to its copy), scanned for keys before packing (none found). 212 files:

- `p242-scratch/`, every text file under 2 MiB the reports cite, at its path relative to
  `.claude/worktrees/`, so a path a report names still names its file: `tar -xJf
  docs/panel/242-reports/evidence.tar.xz -C .claude/worktrees` puts them back. That is the seats'
  logs, scripts and probe programs, the critic's `pathprobe/` and `vararg/` (defect 2421's
  reproducer), the ffi-pragmatist's `route-e.diff` and its packages, and the compiler-engineer's
  `routeA-final.diff` with its instruction counts (`instr-check.txt`, `instr-halves.txt`);
- `evidence/compiler-engineer-routeC.diff` and `.status`, route (C) at `selfhost/`'s root as the
  seat left it uncommitted in its worktree, and `evidence/compiler-engineer-routeD-harness.diff`,
  route (D)'s copy of `tests/harness/` against the base's;
- `evidence/ffi-pragmatist-rt-e.diff` and `-rt-put.diff`, the two scratch runtimes against the
  base's `runtime/`;
- `evidence/spec-warden-design-W1.diff`, `-W2.diff` and `-W3.diff`, the three priced wordings
  against the base's `docs/design.md`.

Left out: the seats' worktrees (`.claude/worktrees/242-*`), the copies of the tree under
`compiler-engineer/routeA-tree/`, `routeC/` and `routeD/` (their diffs above), the scratch runtimes
`rt-e/` and `rt-put/` (their diffs above), the four full copies of design.md (their diffs above),
libcurl 8.23.0's headers the ffi-pragmatist copied (`inc823/`, third-party code, in Homebrew's
`curl` 8.23.0), every binary and every `build/` cache. The folders are the author's to remove.
