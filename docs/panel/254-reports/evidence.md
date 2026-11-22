# Panel 254's evidence

`evidence.tar.xz` (223,212 bytes, sha256 `e0b8ed859baa2a4d8603a0ac96d5d8e621111a21cbe00629e0f9cf5ad69707ba`),
made 2026-11-22 at 04:06 by the coordinator with `coord/tools/evidence.py` (inside the archive), as
SKILL.md § 3c asks: every text file under 2 MiB of the folders the sitting's reports cite, **read back
byte for byte (738 of 738 equal)** and scanned for keys.

## What it keeps, by folder

| folder in the archive | from | files |
|---|---|---|
| `254-compiler-engineer/` | `p254-scratch/compiler-engineer/p254/`: the runtime half (`q2/mail.c`), the cycle, the stress and gateway emulations and their logs, the `Ty` counts (`q1/*.out`), the probes | 123 |
| `254-ffi-pragmatist/` | its `p254/` (the C hand-overs `c/`, route A's gateway `gwa/`, A0 `a0p/`, the holder check `q11/rt/parts/alloc.c`, `nm/`, `q2/`), and under `tree/` the nine files it changed or added in its copy's tracked tree for route A (`runtime/hero_mail.h`, `runtime/platform/hero_mail.c`, `hero_stream.c` and `.h`, `hero_handle_tag.h`, `runtime.c`, three Rebel bindings) | 365 |
| `254-spec-warden/` | its `p254/`: `spec-w.md`, `spec-w2.md`, the variants and their maker, the suites' outputs, the probes | 58 |
| `254-critic/` | its `p254/`: the first reading's probes, A0, the S11 shapes (`s11/`), the stop's behaviours and behaviour 3 (`s7/`, `s7/m3/`) | 72 |
| `254-llm-ergonomist/` | the blind area: `run.sh`, `preflight.sh`, `tools/`, `arms/`, the 26 drawn folders, `key.txt`, `order.txt`, `run.log`, `warden-at-start.md`, the preflight's own files | 109 |
| `coord/` | the coordinator's arms and tools (`make_arms.py`, `make_resolution.py`, this archive's maker), the build log, the probes of t2's programs 4 and 5 | 11 |

## What it leaves out, and why

- **5,536 files that are copies of the frozen tree**: a file under a folder named as one of the tree's
  own (`runtime`, `selfhost`, `packages`, `tests`, ...) or `rt`, byte-equal to the frozen commit's file at
  the same path. A file a seat changed there is kept (for example the holder check's `alloc.c`).
- **233 files that are not text**: the seats' binaries and objects.
- **13 files over 2 MiB**: emitted C of whole programs (`a0p.c`, `gwa.c`) and binaries.
- **26 links**: the symlink roots two seats made to run the `runtime` suite on a prototype runtime.
- **Every `build/`, `tmp/` and `home/` folder**: the compilers' caches, the sessions' temporary folder,
  and the blind area's `home/` (the CLI's transcript of the preflight; the scans' readings of it are in
  `run.log`).

## The key scan

No file was left out for a key-shaped string. The archive holds four `sk-ant-` strings, all one
string, `sk-ant-api03-refused-by-design-panel-preflight`, the preflight's refused key, which the API
refuses by design and is no credential (`tar -xOJf evidence.tar.xz | grep -a -o -E
'sk-ant-[A-Za-z0-9_-]{20,}' | sort | uniq -c`).

## What is not here

The seats' working copies themselves (`.claude/worktrees/p254-scratch/<seat>/`, detached worktrees of
`89f155330`), which are the author's to remove; the compiler binaries; and no paid session's output, none
having run.
