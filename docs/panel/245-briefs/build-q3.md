# Panel 245, the build of Q3's P1 with P4's note: the compiler-engineer

Written 2026-11-11 by the coordinator after the critic's second pass
(`docs/panel/245-reports/critic-reports.md`, read whole before you start, its section C above all).
`/panel` 3c: a route that does not build is not adopted, and a seat with Bash builds the adopted
route in its own copy and runs the cases the sitting named before the synthesis is written. The
critic names P1 the most robust route for Q3 and finds it unbuilt in the compiler (its "what it
rests on"). This brief asks you to build it in your worktree, `.claude/worktrees/245-compiler-engineer`
(clean at `7fb6868b2`), and to run it.

## What to build

**P1, a refusal by provenance judged per supplied name, beside P4's note naming both halves.** For
every name a group binds that a library supplies, the header that declares it and the library file
the linker loads for it come both from folders the build names, or neither. Every clause below is
the critic's, each to be built and shown:

1. **per supplied name**, the declaring header of each name and never the group's own header alone
   (item 22): this is what keeps `examples/curl`, whose `curl_bound.h` stands under `--include
   vendor`, passing. Say what gives you each name's declaring file (`cli/header_types.hero:9`'s
   `-ast-dump=json` is the critic's question, unmeasured);
2. **the folders the environment names count as named**: `LIBRARY_PATH` read from clang's link job,
   its empty entry `.` among them (item 19; the author's profile sets `LIBRARY_PATH=:/opt/homebrew/lib`
   on this Mac, so every plain build reads it);
3. **a refusal, not a warning** (design.md's *exit 1 or nothing*, the critic's citation);
4. **P4's note**, which you built (`routes/P4/head_names.hero`, +1/−1 at `selfhost/head_names.hero:222`)
   and which moves the three goldens that pin the old words: the remedy names both halves, the
   header's folder and the library's, or `pkg-config` answering both.

## What to run, with the compiler you build

- The critic's reproducers, copied first into your own scratch folder: `q3/rl` (readline: plain,
  `--library`, `LIBRARY_PATH`, `pkgconfig`, and `--library` with `--sanitize`), `q3/shim` (curl
  through `pkgconfig "libcurl"`), `q3/lzma` (with and without `LIBRARY_PATH`), `q3/dot` (a library
  in the build's folder), all under `.claude/worktrees/p245-scratch/critic/second/`; and the
  ffi-pragmatist's eleven rows (`p245-scratch/ffi-pragmatist/`, its `pairing.tsv` and the builds it
  names). For each: exit, what it prints, what P1 says.
- `examples/curl`, `examples/sqlite` and every tracked program under `examples/` and
  `tests/golden/run/` that links a library, built by the base compiler and by yours: the names of
  the ones that move.
- The `unsupported`, `run` and `check` forms narrowed to the cases your change moves, by the
  harness's third word, the three goldens P4 moves among them.
- How many lines the change adds under `selfhost/`, in `code_lines`' unit (the `layout` suite's).

## How

- Your worktree, your compiler: rebuild stage 2 from the seed's stage 1 after each change. Never
  build or run in another seat's worktree or folder.
- Batch 30's Linux leg and its last suites may still be running on this Mac: one heavy command at a
  time, no whole suite or form. A program that may not end is built and its binary run under
  `timeout 20`, its output to a file through `head -c 200000`.
- No paid run, no credential printed, no GitHub write, no file outside the repository's root
  (`/tmp` included: write to `.claude/worktrees/p245-scratch/compiler-engineer/build/`).
- Write your report as you go, `docs/panel/245-reports/build-q3.md` in the worktree
  `.claude/worktrees/p245` (that file only; nothing else there), each item the fact, the command,
  what it printed. End with the diff of your worktree against `7fb6868b2`, saved as
  `.claude/worktrees/p245-scratch/compiler-engineer/build/route.diff`, and its line counts.
