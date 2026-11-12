# Panel 246's evidence

`evidence.tar.xz` beside this note holds the text files of the sitting's scratch folder,
`.claude/worktrees/p246-scratch/`, as they stood on 2026-11-12 at 07:23, after the critic's second
pass and the two Linux arm64 legs: every route's diff, probe program, header, driver, log, table,
blind session's folder and saved output under 2 MiB, **3,684 files and 4,647,796 bytes**, plus
one manifest; 759,272 bytes after `xz -9e` (`sha256` `b5836e953ddef1ac...`). Packed by the
synthesis's `synth/pack.py`, which is in the archive.

**One copy of each content.** 258,746 more files of the folder, 23,215,456 bytes, are byte copies
of a file the archive keeps: the 3,007-file package index every case of Part A copied into its own
repository (228,525 of them under `246-compiler-engineer/runs/`), the twenty blind sessions'
identical briefs and arm copies, and the like. Each is named in `DUPLICATES.tsv` at the archive's
root (258,747 lines, 36,615,386 bytes, its first line a header), beside the kept path that holds the
same bytes, so the folder can be rebuilt whole; each pair compared equal by `cmp`.

**Left out**, counted by `pack.py`:
- **the copies of the repository and of its `runtime/`**, 12 whole trees and 8 runtime copies:
  each is kept instead as a unified diff against the commit it was copied from, `ef50a7478` (the
  critic's first-pass tree against `b8c8240d4`), in `synth/diffs/`, 20 diffs (`build/`, `.git`
  and `tmp` left out of each), with the other files of each runtime copy's folder listed beside it
  (`*-other-files.txt`). The diffs of the six untouched bases read two lines each, the compilers
  built there;
- **102 binary files**, 38,169,924 bytes: compilers, probe programs, objects, the Windows `.exe`
  and `.pdb` of `a-win/`;
- **4 files of 2 MiB or more**, 406,847,488 bytes: the FAT and exFAT disk images the two Part A
  seats made to ask `flock` there (`246-ffi-pragmatist/a/img/fat32.dmg`, `fat32b.dmg`,
  `exfat.dmg`; `246-compiler-engineer/fat/fat.dmg`);
- 48 `.git` folders, the repositories each case made around itself with `git init -q`; 8 `tmp`
  folders; the `zig` caches (`zig-cache`, `zcache`, `zlocal`, 2 each);
- one symbolic link, `critic2/nocurl/bin/git` (the critic's `PATH` holding `git` alone), and two
  FIFOs, `246-ffi-pragmatist/b/out/fifo-0` and `fifo-1`;
- the issue drafts of the synthesis (`defects/`, `decision-*.md`), since an issue keeps no copy of
  itself beside it.

**What was already gone**: every `build/` folder of the scratch folder, which stood at a listing taken
after 07:09 (`ls`) and had been removed by about 07:15 by another session (`a-probes/` modified then); caches
the rule leaves out anyway. Every one of the 93 paths under `p246-scratch/` the briefs and reports
cite, and the seats' folder-relative ones checked beside them, still stood at the walk. Never
written: a `report.md` in any blind session's folder (each gave its report as its final message,
kept in its `run.json`).

**Read back** after packing: the archive extracted from the `.xz` file itself, and all 3,685
entries compared equal with their sources by `cmp`. **Scanned for credentials** over the extracted
files (`sk-ant-`, `ghp_` and its kin, `github_pat_`, `AKIA`, a private key's header, `xox`, `AIza`,
`Bearer`, and `ANTHROPIC_API_KEY=` with a value): none; the key's name stands in four files, each
time passed as `"$ANTHROPIC_API_KEY"` and never as a value.

What is where:
- `246-compiler-engineer/`: Part A's phases (`parta2.sh`, `runs-base2.log`, `runs-d.log`,
  `runs-a2.log`, `runs/lsof/`), the fixture `fx-src/`, both Linux legs (`linux-a.log` with `curl`,
  `linux-a.no-curl.log` the void first run); Part B's probes `b1/`, `b1s/`, `b2/`, `b3/`, `b3n/`;
  the gates' logs (`suite-fetch-a2.log`, `suite-runtime-rt.log`, `test-a.log`).
- `246-ffi-pragmatist/`: the lock probes and their logs (`a/`, `a/proto/stress.log`), the door bound
  from Heroes (`a/door/`), the stop probes (`b/`, `b/b2-darwin.log`), the program headers
  (`b/hdr-*/`), the carry header (`b/carry/`), its Linux leg.
- `246-spec-warden/`: its drafts and prices (`drafts/`, `tools/`), its nine thread probes and the
  stop probes (`probes/`).
- `246-llm-ergonomist/`: the twenty sessions' folders with their `run.json`, the arms and the
  runner.
- `critic/` and `critic2/`: the two passes' probes; `critic2/lockrace/` (the lock orders and the
  shared lock), `critic2/probes/acc-worker/` and `handler/`, `critic2/grade/` (each blind answer
  as built), `critic2/t3-runs.log`.
- `a-pid/`, `a-probes/`, `a-win/`, `probe-cross/`, `probe-const/`, `price2/`, `tools/`, `issues/`:
  the coordinator's measurements of the briefs.
- `synth/`: the diffs, the scripts that made them and the archive, and the seven prior-art pages as
  `curl` fetched them on 2026-11-12 at 07:09 (`synth/prior/`).
