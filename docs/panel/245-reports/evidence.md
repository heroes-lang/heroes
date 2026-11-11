# Panel 245's evidence

`evidence.tar.xz` beside this note holds the small files of the sitting's scratch folder,
`.claude/worktrees/p245-scratch/`, as they stood on 2026-11-11 after the two build seats: every
route diff, table, script, probe program, header and saved output under 2 MB, 28,508 files and
16.1 MB before compression, 1,361,360 bytes after. Left out: compilers, objects, and the compiler's
own caches inside a probe program's folder (`build/pragma-*`, `build/tu-*`, `build/harness-*`),
which a build rebuilds.

What is where:
- `critic/second/`: the critic's probes of its second pass, among them `q1/run.sh` and its 17-shape
  table, `q1curl/` (curl's width checker), `q2/` (the door the note describes), `q3/` (`rl`, `shim`,
  `lzma`, `dot` and `p1.py`).
- `compiler-engineer/`: its routes of the first round (`routes/`, E, *var*, P4), its census, and
  `build/` with P1's diffs (`route.diff`, `route-p1.diff`) and the Mac rows; `linux/` holds the four
  runs of the Linux arm64 tie, their scripts and tables.
- `ffi-pragmatist/`: its prototype (`proto/`), its Q2 and Q3 probes (`q3/pairing.tsv`), and `build/`
  with route (d) and S and D (`route.diff`), the level variant (`route-level.diff`), the census
  tables and the instruction counts (`cost/instr3.txt`).
- `spec-warden/`: its drafts and their prices.

Read back after packing: 28,506 of the 28,508 files compare equal by `cmp`. The two that do not are
`compiler-engineer/q2cases/open_door/made2.txt` and `critic/second/q2/ce-door/made2.txt`: each is
the empty file a door calling `open` with no mode created at mode `--wx------`, the fault the
sitting's Q2 rests on, so `cmp` cannot read it; both are 0 bytes in the folder and in the archive.
Scanned for credentials before packing: none.
