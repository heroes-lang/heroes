# Panel 237, the evidence kept

`evidence.tar.xz` beside this file, written 2026-11-08 by the coordinator: 3,643 files, 325,016
bytes, read back whole (`tar -tJf` lists 3,643 entries, no AppleDouble entry). It holds every text
file under 2 MiB (`file --mime-encoding` not `binary`) of the folders the sitting's reports cite:

- `237-compiler-engineer/build/ce/`: the seat's programs, scripts and logs (`routes/`, `m2/`, `logs/`,
  `envmatrix.sh`, `groupmatrix.sh`, `sweep2.sh`, `cases2333b.sh`, `cost2.sh`), and **of its two variant
  compilers only the modules that differ from the tree at `163912031`** (10 files under
  `variant/selfhost/` and `variant2/selfhost/`): the routes R2 adopts, as built;
- `237-ffi-pragmatist/build/ffi/`: the hand-written C of each search order, the packages' answers, the
  SQLite programs and the Linux runs' work folders (`lx/s3/`, `lx/work/`);
- `237-critic/build/critic/`: both passes' programs and outputs (`cpath/`, `cpath2/`, `pcmain/`,
  `fences/`, `grade/`, the shapes of 2343);
- `237-spec-warden/build/sw237/`: the drafts it priced and its refresh records;
- lane `p237`'s `build/b237/`: the blind seat's folder and its `run.json`, the 288 reader sessions'
  `task.md`, `run.json` and `run.err`, the graded programs with their build and run output
  (`graded/`, `graded/table.tsv`) and the `lent`-deleted rebuilds (`refix/`);
- `p237-scratch/blind237/`: the runner `blind237.py`, the shim's source, the reference programs
  (`refs/`), the store and journal trials, the run logs and the grader's outputs; and the
  coordinator's reproductions (`t2/`, `t2seat/`, `viagroup/`, `envdoor/`, `pcmain/`, `drafts/`,
  `shadow1/`, `noshadow/`).

**Left out**: every binary (compilers, objects, built programs, `shim.dylib`), every `build/` cache
inside a program's folder, the ffi-pragmatist's copy of the repository sent to the Linux container
(`lx/tree/`, 14,300 files), the unchanged modules of the two variant compilers (copies of
`selfhost/`), and **the 289 copies of the specification the sessions read**: each is one of five, and
`arms-spec-sha256.txt` in the archive gives their sha256 (A is `spec/heroes-spec.md` at `163912031`,
`fea65798...`; B, C and D are it with the fence's lines as `blind/README.md` gives them; the seat's is
A with the marker line), every session's copy checked against its arm's by `shasum -a 256` before the
archive was written. Scanned before writing: no string shaped like an Anthropic or GitHub key, no
`ANTHROPIC_API_KEY=` with a value, and no address at the author's domain. The seats' worktrees
(`.claude/worktrees/237-<seat>/`) hold no committed change. The folders are the author's to remove.
