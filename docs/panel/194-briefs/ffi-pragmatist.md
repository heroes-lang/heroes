# Panel 194, ffi-pragmatist

Read `00-shared.md` first, then panel 178's sitting whole and its
ffi-pragmatist report (`docs/panel/178-reports/ffi-pragmatist.md`), then
defects 092 and 094's issue files, in your copy.

Your seat judges the C boundary and the platforms (design.md §1.11, §1.12,
§4.19) and holds a veto on ABI breakage.

1. **Write and compile the bindings** R1 serves and the C each implies, on
   this Mac (Apple clang 21.0.0, `-I/opt/homebrew/include` for OpenSSL) and
   in Docker's `heroes-linux-arm64`: `utsname` (`uname`), `sockaddr_un`
   (`connect` to a path), a `pthread_mutex_t` set up correctly, and one
   OpenSSL `SHA256_CTX`. Report what compiles, what runs, what each costs in
   tokens of `.hero` (`heroes measure <file>`, the vendored row, said as such)
   with and without R1.
2. **Zero as bytes**: re-run 178's zero-validity table on today's code and
   both legs here (a zeroed mutex, spinlock, barrier); x86-64 in the `heroes-linux` image, which is amd64 and runs without
   `--platform` (the critic).
3. **Padding**: re-run 178's by-value finding where you can (MemorySanitizer
   is Linux only: in Docker's image if `clang -fsanitize=memory` builds
   there, with a control), and say what § 13 may and may not promise.
4. **094**: read lane b12-ffi13's branch when you write; check its lowering
   against `PTHREAD_MUTEX_INITIALIZER` on Darwin and Linux arm64.
5. **092**: read lane b12-ffi13's evidence (`<scratchpad>/batch12/ffi13/`)
   when it exists; for each route, the C a binding author writes and what a
   wrong count does on each leg.
6. **A shim header is allowed** where C's own declaration cannot be bound
   (`connect` takes `const struct sockaddr *`): say so where you use one.
   `SHA256_Init` is deprecated in OpenSSL 4.0.3 and still links; prefer
   what a binding would use today.
7. **The Windows box** (`ssh win`, clang 23.1.1, 2 cores; **a Windows leg of
   batch 12 may be running in `/c/w/b12-*`: never touch it, keep your load
   light**; **the headers only: no tree copied, no build**, three
   lanes share its two cores; your folder `/c/w/p194-ffi-<pid>`, never remove
   anything): which of the census headers exist there, and
   whether a binding of `sockaddr_un` or `utsname` means anything on Windows;
   unrun if the box does not answer.

Verdict per route: approve, object or veto, with its section, its cost, a
falsifiable prediction and its condition. Report:
`<scratchpad>/p194/reports/ffi-pragmatist.md`, written as you go.
