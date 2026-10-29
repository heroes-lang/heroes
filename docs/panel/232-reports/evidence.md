# Panel 232, the evidence

`evidence.tar.xz` beside this file, 2,405,380 bytes, holds the sitting's evidence as the author's
instruction of 2026-10-12 asks (task 2135): **3,521 text files**, each under 2 MiB, of the folders
the reports cite, by their paths under `.claude/worktrees/`. Made by
`232-coordinator/evidence.sh`, which is inside it, with `COPYFILE_DISABLE=1 tar --no-mac-metadata
--no-xattrs -cJf`, **read back byte for byte** (extracted into a scratch folder and `cmp` against
what went in: 3,521 files, 0 differ), with no `._` entry, and **scanned for keys** (`grep -rlE` for
`sk-ant-`, `gh[pousr]_`, `github_pat_` and an `ANTHROPIC_API_KEY=` with a value: nothing; a first
scan matched the script's own patterns, which it now puts together from pieces).

## What it keeps

| folder | files | what it is |
|---|---|---|
| `232-compiler-engineer-scratch/` | 360 | the seat's programs and their outputs: the order of evaluation (`ce/`, `ord/`), the shifts under the frozen compiler and the abort route (`shl/`, `shift/`, `bench/`), `exit`'s edges (`exit/`), the stack's shapes and their generator (`stack/`, `tools/gen_stack.py`), the codes read out of `selfhost/` (`codes/`, `reach/`), the route's patch, the stack's 12-line repair, and the logs of the route's tests; every program's printed output and build log beside it (`bin_<name>.out`, `bin_<name>.build.txt`) |
| `232-spec-warden-scratch/` | 132 | `price.py`, every draft priced (`drafts/`) and `prices.tsv`, the probes behind its wordings (`probes/`) |
| `232-ffi-pragmatist/ffi-scratch/` | 1,301 | the handle shapes and their headers (`handle/`), the bindings of each header (`bind/`), the ABI steps (`abi/`), the third-party header in nine versions (`thirdparty/`), the stop caught (`catch/`), the name a later runtime adds (`prefix/`), the costs (`cost/`), clang's declaration tables of both headers, the `run` form's outputs under route (a) |
| `232-critic/critic-scratch/` | 1,244 | both passes' probes (`second/` for the reports), the pages it read by `curl` (`second/pages/`), the blind join script |
| `232-coordinator/` | 244 | the blind seat whole: `run_one.sh`, `fill.py`, `make_p.py`, `score.py`, the two specifications, the sealed key, `scores.tsv`, every answer (`answers/`), the run's log; the coordinator's reproductions of 2254 to 2261 (`verify-ce/`, `verify-ffi/`, `verify-abi/`, `verify-adj/`); the critic's and the lanes' prompts, the pricing request, the synthesis's draft; and `runtime-diffs/`, eleven diffs |
| `compat-promise/build/b232/` | 240 | the 80 blind sessions' folders, each the brief, the task and the specification it was given |

**`runtime-diffs/`** holds each edited copy of the runtime as its `diff -ru` against the frozen
tree's `runtime/`, in place of the copy: the ffi-pragmatist's `runtime-eq` and `runtime-eq-cold`
(item 3's route (a)), `runtime-dfl` (the stop made final), `runtime-own` (a result's ownership
changed), `runtime-add` (a name a later runtime adds), `runtime-v0` to `runtime-v4` (the ABI steps),
and the coordinator's `runtime-v32` (defect 2259's reproduction).

## What it leaves out, and why

- **The copies of the repository**: the compiler-engineer's `corpus/` (the release's trees unpacked
  by `git archive`), `pristine/` and `frozen_rt/`, the critic's `compat/`, each made again by the
  command its report names; and the seats' worktrees themselves, detached at `afe983265`.
- **Every `build/` and every binary**: compilers, programs, objects; each made again from the
  sources kept and the frozen tree.
- **The edited runtimes' unchanged files**, kept as diffs (above).
- **The pages the coordinator re-read for the prior art**, fetched into the session's scratchpad
  outside the repository; their URLs and the words quoted are in the sitting's file.

**A first archive of the same day left out text files by their names** (a filter meant for
binaries dropped `heroes-spec.md` and the programs' `bin_<name>.out` outputs, 276 files);
the binaries are left out by their content alone, and the archive above is the second.
