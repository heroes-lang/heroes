# Panel 230, the evidence

`evidence.tar.xz` beside this file, 1,280,304 bytes, holds the sitting's evidence as the author's
instruction of 2026-10-12 asks (task 2135): **every text file under 2 MiB** of the folders its
reports cite, **2,655** of them under `tree/`, and **the prototypes' changes** under `patches/`.
Made with `COPYFILE_DISABLE=1 tar --no-mac-metadata --no-xattrs -cJf`, **read back byte for byte**
(extracted into a scratch folder and `diff -r` against what went in: identical, 2,660 files), with
no `._` entry, and **scanned for keys** before it was made (`grep -rlE` for `sk-ant-`, `gh[pousr]_`,
`github_pat_` and an `ANTHROPIC_API_KEY=` with a value: nothing).

## What it keeps, by the path under `.claude/worktrees/`

| folder | what it is |
|---|---|
| `panic-location-scratch/before/` | the eight programs of the "before" and their stderr |
| `panic-location-scratch/defects/` | the reproductions of defects 2224, 2225 and 2226 |
| `panic-location-scratch/blind230/` | the blind seat: `items/` (the eight family P programs, their T and P stderr, the references `ref/` and their outputs), `m/` (the six tasks' cases, `run-cases.sh`, the references in today's shape `ref/` and in M1's `refp/`, the four mutants `mut/`), `spec-T.md` and `spec-P.md`, `fill.py`, `run_one.sh`, `score.py`, `scores.tsv`, `joined.txt`, `sealed/key.tsv`, `answers/` (the 84 answers whole, family P's `choice_points` among them) and `work/` (each answer's program as built, and each case's line) |
| `panic-location-scratch/timing/` | `window.sh` and every timed run's `/usr/bin/time -lp` log |
| `panic-location/build/b230/` | the 84 folders as the sessions read them (`heroes-spec.md`, `brief.md`, `task.md`) |
| `panic-location/build/b230-out/` | the 84 scored transcripts (`<n>.jsonl`, `<n>.err`) and the first run's set-aside 63 under `first/` |
| `230-critic-scratch/` | the critic's programs of both passes |
| `230-compiler-engineer-scratch/` | the seat's logs, tables, probes and the clock's commands (`timing/`), its compilers and seeds left out (below) |
| `230-ffi-pragmatist-scratch/` | the seat's C (`l1/`, `bench/`, `merge/`, `dsym/`), its `cc.sh`, and its emission logs |
| `230-spec-warden-scratch/` | `prices.tsv`, every draft priced (`drafts/`), the `u8` probe |
| `230-coordinator-scratch/` | the program that showed `()?`'s C shape |

**`patches/`**: `230-compiler-engineer.diff` (1,420 lines: the seat's prototype in its worktree,
twenty files changed and four new, `emit/place.hero`, `emit/sides.hero`, `runtime/parts/place.c`,
`runtime/parts/show.c`), `230-coordinator.diff` (52 lines: the M1 prototype `heroes-m1` that judged
family M's P variant, `check/decls.hero` and `emit/c_main.hero`); the ffi-pragmatist's, the
spec-warden's and the critic's worktrees hold no change (`git diff` empty; the ffi-pragmatist built in
its scratch, kept above).

## What it leaves out, and why

- **448 binaries** (compilers, programs, objects; a NUL byte in their first 8 KiB, or a Mach-O or ELF
  magic): rebuildable from the patches and the tree at `595d92d88`, and not text.
- **43 files over 2 MiB**: the seeds the seats emitted (`seed_L1.c`, `seed_L1a.c`, the
  compiler-engineer's `seed-after.c` and its siblings, each 27 to 29 MB), the compilers' own
  emissions, and the largest logs; their sizes are in `timings.md` and the seats' reports.
- **Every `build/` folder and every copy of the repository** (a folder holding `seed/` or `selfhost/`,
  such as the compiler-engineer's `clean/`): caches and copies of tracked files.

Nothing the reports cite was already gone when this was made. The folders themselves stay where they
are until the author removes them (their instruction of 2026-10-12).
