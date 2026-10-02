# Panel 185, the ffi-pragmatist's brief

Read `00-shared.md` in this directory first; it holds the five questions,
their routes and every measured fact, and the rules of your directory
(`<scratchpad>/185-ffi-pragmatist/`, a copy of `03e70520`, your compiler built
inside it from the seed). Write your report as you go to
`docs/panel/185-reports/ffi-pragmatist.md`.

**Q1 is yours first.** Write and compile the C each route implies, against
real headers on this Mac (Apple clang, macOS SDK) and, where you can, by
`clang -target x86_64-linux-gnu -fsyntax-only` against the Linux headers the
`heroes-linux` image carries (`docker run --rm heroes-linux ...`; never two
containers at once, and `docker ps` first: three repair lanes may run a leg).
The bindings a real program needs: `WEXITSTATUS`, `WIFEXITED`, `WTERMSIG` with
`waitpid`; `htonl`, `htons`, `ntohl` (a macro on macOS, a function on glibc, by
lane emit's reading, recorded); `FD_SET`, `FD_ISSET`, `FD_ZERO` (macros taking
a pointer); `errno` (an object-like macro over a function call); `isdigit`
(a function and a macro on some libcs). For each: what (1a) checks and what it
cannot, what (1b) tells the author, what (1d), a `static inline` wrapper in
a header of the program's own, checks (`probes/q1/shim/`, measured working),
and whether the emitted C of every binding that builds today stays
byte-identical: of the 412 tracked `.hero` files with an `extern` line, 206
emit under `build --emit-c` today and five are refused `ffi_unknown_name`,
none a macro (`probes/q1/extern-emit.tsv`); `check` cannot see Q1, since the
probe runs at `build`. Pointers: `selfhost/emit/ffi_declared.hero:159` (the
message), `selfhost/cli/assemble.hero:4-24` (the second clang round that
exists, for a handle's tag).
**You hold a veto on ABI breakage.**

**Defect 145's no-evidence shape**: the note gives two true readings where
clang showed no spelling of the type
(`tests/golden/unsupported/fixedbugs-145-a-typedef-with-nothing-else-said-of-it.hero`
and `-a-misspelled-tag`); the critic measured that one `(void)sizeof(name);`
query splits them; say whether it belongs to Q1's round or to its own.

**Q4, beside it**: whether a C or shell habit makes the bulleted reading of
`- 1 =>` likelier than the signed one, from code you can point at.
