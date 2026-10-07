# Panel 196, ffi-pragmatist

Read `00-shared.md` first, then defect 396's issue file, panel 194's sitting
and its ffi-pragmatist report (`docs/panel/194-reports/ffi-pragmatist.md`, its
*found alongside* 1, where 396 was found) and `.claude/rules/c-boundary.md`,
in your copy.

Your seat judges the C boundary and the platforms (design.md §1.11, §1.12,
§4.19) and holds a veto on ABI breakage.

1. **The census of the fault's shape**: in the headers this Mac has
   (`/usr/include` through the SDK, `/opt/homebrew/include`) and in Docker's
   `heroes-linux-arm64`, find functions whose pointer parameter C writes as an
   array with no count parameter beside it (OpenSSL's digest finals,
   `SHA256`, `MD5_Final`, `encrypt(char *_LIBC_COUNT(64), int)` on this Mac (`crypt` writes no
   parameter, the critic), `inet_ntop` has a count, `gets`
   has none); say how you searched, since a negative is the searcher's
   vocabulary. Count, for the same headers, the single-cell out-parameters a
   route must keep (`int *`, `size_t *`, `char *` written once).
2. **Write and run each route's binding of `SHA256_Final`** on this Mac and on
   Linux arm64 (Homebrew's OpenSSL 4.0.3 here; Debian's in the image), with a
   group record `Box { m: u8[32], after: i64 }` whose `after` must survive:
   and the critic's three neighbours (`docs/panel/196-evidence/critic/`:
   `pipe`, `gethostname` with its count, `EVP_DigestFinal_ex`'s out-count) under
   each route as well;
   S1 and S2 as the compiler-engineer prototypes them (read their report when
   it exists, never their copy), S6's shim, and today's `@md: u8`. Say what a
   wrong extent (`u8[16]`) does under each route.
3. **S5 on real headers**: how many digest or buffer functions in the census
   declare their parameter as an array with a length (`md[32]`,
   `[static 32]`) or carry clang's `counted_by`/`sized_by` attributes.
4. **S4's guard**: write the C a run-time guard implies (a canary region around
   the lent cell, checked after the call), compile it, and say what it catches
   and what it cannot (a write far past the region, a write into the heap).
5. **The Windows box**, up since about 20:46 with 21 GB free (`ssh win`,
   clang 23.1.1, 2 cores; three lanes may use it, one clang at a time; your folder
   `/c/w/p196-ffi-<pid>`, headers and hand-written C only, never a tree copy,
   never remove anything): whether `bcrypt.h`'s `BCryptFinishHash` (which
   takes a count) and any digest API without one exist there; unrun if the box
   does not answer.

Verdict per route: approve, object or veto, with its section, its cost, a
falsifiable prediction and its condition. Report:
`<scratchpad>/p196/reports/ffi-pragmatist.md`, written as you go.
