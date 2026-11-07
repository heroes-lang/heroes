# Panel 235, the ffi-pragmatist's brief

Read `00-shared.md` first, then `proposal.md`, `probe/README.md` and the
survey's rows on fetching, fingerprints and indexes (2.1, 2.2, Table 3). You
judge the founding constraint (design.md §1.11, §4.19): everything comes from
C, and a binding is verified by clang against the header it names. You write
and compile the C each route implies, or show why it cannot be written. You
have a veto on ABI breakage.

**Your worktree** is `.claude/worktrees/235-ffi-pragmatist/`, your scratch
`.claude/worktrees/235-ffi-pragmatist-scratch/`, your report
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/pm-opening/docs/panel/235-reports/ffi-pragmatist.md`.

## Your questions

**A fetched package that binds C.** Write one: a package fetched into a
program's tree whose module declares an `extern "x.h"` group over a header the
package ships, and a second whose group names a system library
(`link "sqlite3"` or `package "<name>"`). Show where clang finds the shipped
header from the consumer's build (today `-I` is the compiled file's
directory, design.md §4.19), what the program's link line gains, and what a
fingerprint must cover for the build to be reproducible (the header, a C
file, a prebuilt object). Compile and run both with the compiler you build;
where the route needs a compiler change you do not make, say so and stop at
the C.

**13, the C under the first version.** The first version fetches over C:
by running programs (`git`, and `curl` for an index, as `heroes measure
--refresh` already runs `curl`, probe) or by binding libraries (libgit2,
libcurl) into the `heroes` binary. For each: what the seed and its one clang
line become, what each of the four CI platforms must have installed, and what
each does to M-standalone-compiler (row 83, `heroes` needing nothing else
installed). Measure what you can on this Mac (the programs' versions, a
`git clone --depth 1` of a public repository into your scratch, bounded by
`timeout`) and write the other platforms as unrun.

**5 and 8, fingerprints and the index.** Over which bytes a fingerprint is
computed for a git source and for an index archive (the survey: Go hashes file
names and contents, Cargo the archive, Zig the selected files); whether the
compiler's own SHA-256 can serve when a fetched file is not text, and what the
runtime would need to read bytes; and whether a static index (files served by
a site, which Go's protocol states it allows) can be read by the first version
with `curl` and checked with that fingerprint.

**Robustness at the boundary.** Whether fetching or building a dependency
ever runs that dependency's code under each route (a header is compiled, a
`package` group asks `pkg-config`, which is the machine's program and not the
package's), and what a hostile package could make the consumer's build do.

## And from the critic's first pass

- **18, the network**: whether `build`, `check` or `run` may ever fetch, or
  only the verb that adds or fetches, against
  `selfhost/cli/refresh.hero:4-10`, *the network is admissible only where
  the answer is pre-recorded*.
- **5, whose fingerprint is trusted**: the reader test states the sum as *the
  publisher's fingerprint*; say what each base does (Go's checksum database,
  Cargo's index `cksum`, a first fetch recorded) and what a hostile index or
  a moved git tag can make a consumer's build do under each.
- **15, with the historian**: what row 78, M-package-index, must deliver
  (publishing, names, signing, a withdrawn version), and what of it the first
  version's catalogue must already respect.
