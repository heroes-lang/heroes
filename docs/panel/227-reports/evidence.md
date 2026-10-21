# Panel 227, the evidence beside the reports

`evidence.tar.xz`, made after the landing on 2026-10-21 by the coordinator's `227-evidence.py`
(kept in `.claude/worktrees/thesis-drafts/`, outside the tracked tree), from the folders the
reports and the synthesis cite: every text file under 2 MiB, scanned for a key-shaped string
before the archive was written (none found; the script refuses the archive otherwise) and read
back byte for byte after. Every binary, the compilers above all, is left out, as the sittings
since 198 leave them.

## Kept, 348 files, under `227-evidence/`

- `compiler-engineer/227out/`, 57 files: every suite's whole output for each route the seat built
  (A, E, E2, E4), the shapes' messages, and the arity probes.
- `compiler-engineer/227probe/`, 30 files: the programs the seat probed.
- `compiler-engineer/227variants/`, 31 files: each variant's files (`a`, `e`, `e2`, `e4`).
- `compiler-engineer/227orig/`, 16 files: the originals the variants changed.
- `compiler-engineer/227fx/log.txt`: the seat's fixpoint log (its two emissions, 26,759,615 bytes
  each, are over 2 MiB and left out).
- `blind/run-that-counts/`, 42 files: the six folders `build/w/1` to `6` as the sessions read
  them, with each session's `run.json` (the report is its `result`), `run.err` and exit; 2, 3 and
  5 route A's sentence, 1, 4 and 6 today's.
- `blind/void-first-run/`, 56 files: the first run of six, void (`context: yes`), in its folders
  `p1` to `q3`, and under `probes/` the two sessions that found the blind command (`bare-*` with
  `--bare`, `plain-*` without).
- `blind/programs-and-checks/`, 42 files: the twelve programs taken from the six answers, each
  with its `check --brief` and `check` output, and the six answers as `w<N>-report.md`.
- `spec-counts/`, 12 files: the two copies of the specification counted (`before.md`, `after-a.md`),
  the spec-warden's route-B draft (`after-b.md`), and the four `count_tokens` requests with the
  API's answers (`before`, `after-a`, `probe-one`, `probe-five`), from which 10,286 and 10,299 come.
- `r4-probe/shapes.hero`: the shapes defect 2185's repair was probed on.
- `gates/landing/`, 43 files, and `gates/spec-half/`, 18 files: the landing's two gates, every
  step's and suite's whole output and the census's two lists.

## Left out

- over 2 MiB: 4 (the two gates' fixpoint emissions, 26,763,178 bytes each).
- binary: 0 beyond those (the compilers stand outside the folders archived).
