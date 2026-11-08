# Panel 236, the evidence kept

`evidence.tar.xz` beside this file, written 2026-11-08 by the coordinator: 2,883 files, 2,017,432
bytes, read back whole (`tar -tJf` lists 2,883 entries, no AppleDouble entry). It holds every text file
under 2 MiB (`file --mime-encoding` not `binary`) of the four folders the sitting's reports cite:

- `236-coordinator/`: the brief's own runs (`run/`), `grpret` and `grpwrap` at both levels and the two
  64 KiB chains' units and clang counts;
- `236-compiler-engineer-scratch/`: the seat's routes as patches (`route-final.patch`, R7b with rule B;
  `route-rp-Bip.patch`, the proposal as written; `rp_patch.py`), its programs (`mine/`) and outputs
  (`out/`);
- `236-ffi-pragmatist-scratch/`: the boundary's hand-written C of each route, its targets, its frames and
  its library (`q4/`), and its tools;
- `236-critic-scratch/`: both passes' programs and outputs (`r2/`), the probe programs copied into
  `236-briefs/probe/`.

**Left out**: every binary (compilers, objects, archives, built programs), every `build/` folder, the
extract of panel 233's archive under `236-coordinator/ev233/` (it is that sitting's `evidence.tar.xz`),
and every file of 2 MiB or more (the emitted C of the 1 MiB chains among them). Scanned before writing:
no string shaped like an Anthropic or GitHub key, no `ANTHROPIC_API_KEY=` with a value, and not the
author's email. The seats' worktrees (`.claude/worktrees/236-<seat>/`) held no committed change; the
compiler-engineer's route lives in its patch above. The folders are the author's to remove.
