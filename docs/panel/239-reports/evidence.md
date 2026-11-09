# Panel 239, the evidence kept

`evidence.tar.xz` beside this file, written 2026-11-09 at 03:19 by the coordinator with
`.claude/worktrees/p239-scratch/evidence.py` (its rule in its first lines): **4,023 files**,
6,494,593 bytes before compression, 356,492 after, read back whole (`tar -xJf` into a scratch
folder and every file compared by `cmp` with its source: 0 differing; `tar -tJf` lists 4,023
entries, no AppleDouble entry). It holds every file under 2 MiB whose type `file -b --mime-type`
reads as text, empty or JSON, of the folders the sitting's reports cite, by their paths under
`.claude/worktrees/`:

- `239-compiler-engineer/build/239/` (1,356 files): the 2364 programs and their outputs on both
  platforms (`repro/`, `shapes/`, `order/`, `out-*`, `emitc-*`, `tracked-emitc/`), the record-layout
  dumps in C (`cdump/`), the tracked roots' list and rows (`tracked/roots.txt`, `out-tracked-*`), the
  cost runs (`cost*`), the container's scripts and outputs (`linux/`), and of its variant compilers
  only the modules that differ from the tree (`sh-agree/`, the route's first version; `sh-agree2/`,
  its second; `sh-verify/`, route B alone; `sh-r2/` and `sh-agree-r2/`, panel 238's R2 with and
  without the route);
- `239-ffi-pragmatist/build/239/` (1,424 files): the binding C and its scripts (`c/`, `c2364/`, the
  libjpeg pair), the Heroes programs (`hero/`) and every run's words and outputs (`hero-runs/`,
  `runs*`, `hero-*.txt`), the route's versions, v1 to v6 (`route/`, `route6/` with `diff/route6.diff`,
  `routeg*`), the cost runs and their `clang` wrapper (`cost*/`), the static-library census
  (`v6c/dprobe-*.txt`), the container's scripts and outputs;
- `239-spec-warden/build/239/` (171 files): the drafts and their measurements (`drafts/make.py`,
  `make2.py`, `measured.txt`, `measured2.txt`), its copies of the reproducers and its re-run of the
  critic's `self_*`;
- `239-critic/build/239/` (824 files): both passes' programs and outputs (`c/`, `s2363x/`,
  `s2364x/`, `p2/`: the route and v5 re-run on both platforms, `self/`, `jpeg/`, `combo/` with the
  first versions combined with panel 238's two diffs, `order/`, `cost/`);
- `p239-scratch/` (248 files): the briefs' reproducers and their outputs on both platforms
  (`repro/`, `repro/mac.out`, `linux/repro.out`, `linux/pthread.out`, the build logs), the critic's
  shapes as copied for the seats (`repro/critic/`), the defect drafts (`defects/`) with their
  `draft` outputs, the decision draft (`decision-239.md`, `decision-239.out`), the landing list
  (`landing.md`), the synthesis's working parts, and this archive's script (`evidence.py`).

**Left out** (`p239-scratch/evidence-out/left-out.txt` lists every path and its reason): every file
whose bytes equal a file the tree tracks at the sitting's commit (16,798, the copies of `selfhost/`,
`runtime/`, `seed/`, `tests/` and `examples/` the seats built from or ran), every binary (729
Mach-O and 364 Linux executables, 131 archives, 85 objects, 12 shared libraries, 638 other binary
files), every inner `build/` cache (210 folders), 39 files of 2 MiB or more (emitted C among them),
the copies of the repository sent to the container (`tree/`, `ltree/`), and panel 238's evidence as
extracted by the seats (it is `238-reports/evidence.tar.xz`). Each is reproducible from the tree at
`1c4bc5197`, the scripts and the diffs above.

Scanned before writing: no string shaped like an Anthropic or GitHub key, no `ANTHROPIC_API_KEY=`
with a value, no address at the author's domain (`grep -rIlE` over the extracted copy). The seats'
worktrees (`.claude/worktrees/239-<seat>/`) hold no committed change (`git status`: only their own
`docs/panel/239-reports/` and `build/`, untracked). The folders are the author's to remove once the
worktrees are.
