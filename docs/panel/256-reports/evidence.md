# Panel 256, the evidence

`evidence.tar.xz` (809,576 bytes, 464 files, 5.5 MB unpacked), made 2026-11-22 by the sitting's coordinator
from the folders the reports cite, **every text file under 2 MiB** (`file` reading text, JSON, a script or
empty), **every `build/` and every binary left out**, and the copies of the repository left out but for what a
seat changed in one: read back byte for byte (`diff -r` of the folder against the archive unpacked: no
difference) and scanned for keys (`grep -rIl -E 'sk-ant-[A-Za-z0-9_-]{20,}|gh[opsu]_[A-Za-z0-9]{20,}|AKIA[0-9A-Z]{16}'`:
nothing).

| in the archive | from | what it holds |
|---|---|---|
| `evidence-256/coord-p256/` | `p256-scratch/coord/p256/` | the coordinator's probes and their runs (`q2/`, `q2run*`), the t2 programs and their runs (`t2/`), the reference programs (`ref/`), the draft checks |
| `evidence-256/compiler-engineer-ce256/` | `p256-scratch/compiler-engineer/ce256/` | its probes and outputs (`q2-gi/`, `gi2/`), the census (`census-golden.txt`, `census-rebel.txt`), the Q1 runtime copy's C test (`q1/`), the clocks it discarded (`clocks.txt`) |
| `evidence-256/compiler-engineer-copy.diff`, `compiler-engineer-copy-new/` | `git -C p256-scratch/compiler-engineer diff`, and its new `selfhost/check/instances.hero` | the `gi` build's changes to six checker files and the emitter guard, and its new module, against the frozen commit |
| `evidence-256/ffi-pragmatist-ffi256/` | `p256-scratch/ffi-pragmatist/ffi256/` | `wait_routes.c` and its runs (`darwin.txt`, `linux.txt`, `darwin_sparse.txt`), `kev0.c`, `bell_udp.c`, the UI programs, the `ptr` programs (`q2/real/`) |
| `evidence-256/spec-warden-zwarden/` | `p256-scratch/spec-warden/zwarden/` | its text generator and the six refreshes' outputs |
| `evidence-256/critic-zcritic/` | `p256-scratch/critic/zcritic/` | both readings' probes (`q2/`, `r2/`, `ffi-real/`) and rebuilt arms |

**Not in the archive**: the blind seat's area, `p256-scratch/256-llm-ergonomist/`, whose 46 sessions are drawn
and not run (`256-briefs/blind/RUN.md`): its `key.txt` stays out of the tree until the run is scored, and the
scorer archives the area then; the briefs folder already holds every file the area was made from
(`256-briefs/blind/`, `256-reports/resolution/`). The seats' and the critic's built compilers and every
`build/` directory. **The folders are the author's to remove** once the sitting is ratified and the blind run
scored: `.claude/worktrees/p256-scratch/` (`coord`, `compiler-engineer`, `ffi-pragmatist`, `spec-warden`,
`critic`, detached worktrees of `8aca3cc43`, and `256-llm-ergonomist`).
