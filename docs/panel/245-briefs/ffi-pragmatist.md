# Panel 245, ffi-pragmatist

Read `00-shared.md` first. Your worktree `.claude/worktrees/245-ffi-pragmatist` (detached at
`a4b89740b`); build its compiler from the seed in two stages; Docker stays off. Your question is the C
each route makes a program write, compiled against real headers on this Mac: Q1, a door over `sprintf`,
`gets`-like and `__attribute__((warning))` functions in the SDK and in Homebrew's headers, what clang
prints and at what exit under each route; Q2, `printf`, `fprintf`, `snprintf`, `syslog` and a libcurl
or sqlite format function (`curl_mfprintf`, `sqlite3_mprintf`) bound directly and through a door, a
run-time format with and without conversions, and what a program that only prints a run-time string
(`puts`, `fputs`, a `%s` door) writes under each route; Q3, the curl and readline mismatches under
`CPATH` and `--include`, what `pkg-config`, Homebrew's own build and clang itself pair, and whether a
general pairing rule exists that a build can check without knowing the library (say what would have
to be true for one to exist). Compile the C a route needs, or show why it cannot be compiled.

## After the critic's first pass

Your base is `7fb6868b2` (00-shared.md § The base). Q3 runs in both directions (`--library` or `LIBRARY_PATH` alone pairs an old header with a new library); bring one measured wrong value or memory fault from a mismatch (a record's layout, a constant, a macro the two releases spell apart), or say none was found and what you tried. Q2: compile `open`, `fcntl`, `ioctl`, `execl` and `sqlite3_config` bound with nothing past `...` and through a door, and the door forwarding a format with no data arguments under `--sanitize`. Q1: run every default-on warning the critic lists under each route, at `-O0` and `-O2`.
