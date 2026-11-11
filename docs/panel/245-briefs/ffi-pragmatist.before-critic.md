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
