# Panel 241's evidence

`evidence.tar.xz`, packed 2026-11-10 at the synthesis from a staging folder and
read back byte for byte (`diff -r` of the extracted archive against the staging
folder, silent; each of the 253 source files `cmp`-equal to its copy), scanned
for keys before packing (none found). 259 files:

- `p241-scratch/`, every text file under 2 MiB the reports cite, at its path
  relative to `.claude/worktrees/`, so a path a report names still names its
  file: `tar -xJf docs/panel/241-reports/evidence.tar.xz -C .claude/worktrees`
  puts them back. That is the compiler-engineer's logs and scripts (`body.sh`,
  `move_calls.py`, `pairs.sh`, `paired.py`, the latency and instruction logs,
  `copies.txt`, the `crash/`, `cat/`, `r13/` and `routec/` probes), the
  ffi-pragmatist's packages under `ffi/index/` with their programs, the
  Linux and Windows scripts and logs and `data/shasum.txt`, and the critic's
  probes of both passes, `pass2/slice-lines.txt` and the third panic's
  catalogue (`pass2/edit/heroes.toml.before`) among them;
- `evidence/compiler-engineer/route1.diff`, route 1 as the seat left it
  uncommitted in its worktree (12 files, +131 −49 by `git diff --stat`: the 10
  modules of `selfhost/fetch/` and the hook mirror patch with its test), and
  `route1-heroes.toml`, the compiler's catalogue it wrote; the placed tree
  itself is `packages/core/0.2.0/files/` of the index at the base;
- `evidence/*_runtime-*.diff`, each scratch runtime against the base's
  `runtime/` (`diff -ru`): route (c)'s door by the ffi-pragmatist and by the
  critic, the `_wputenv_s` line, and the four-door move into `hero_os.h`.

Left out: the seats' worktrees (`.claude/worktrees/241-*`), the copies of the
repository under `compiler-engineer/base/` and `basefull/`, every binary, every
`build/` cache, the random data files (`ffi/data/*.dat`, regenerable only in
kind, their digests kept in `shasum.txt`), and the whole scratch runtimes,
whose differences are the diffs above. The folders are the author's to remove.
