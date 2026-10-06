# Panel 178 — ffi-pragmatist

Read `00-shared.md` first. Your directory is `<scratchpad>/178-ffi-pragmatist/`,
a `git archive` of HEAD `57679005`; build the compiler there from the seed. You
judge the founding constraint (design.md §1.11, §4.19) and robustness (§1.12),
with veto on ABI breakage and on soundness. Write your report to `REPORT.md` in
your directory.

## What to build and run, on Darwin and both Linux legs

1. **The C each route implies, for real structs**: `utsname` (to `uname`),
   `sockaddr_un` (to `bind` and `connect` on a real socket in `/tmp`),
   `sockaddr_storage` (to `getsockname`), `struct addrinfo` hints (to
   `getaddrinfo("localhost", …)`), `statfs` on Darwin and `statvfs` on Linux,
   and `pthread_mutex_t` (to `pthread_mutex_lock`). For each, the Heroes a
   binding author writes today (route S) and under R1, R0, A and T, compiled
   and run; where a route cannot express it, say which line fails and how.
2. **Zero-validity, Z1 against Z2, from the world**: which of the census
   structs (`census-*.tsv`, and `initializers.sh`'s macro list) have a
   documented initialiser or constructor that is not all zeros, per platform,
   from the headers and their manual pages. Measurement 7 found the Darwin
   mutex; find the rest or say where you looked. A Z1 claim must be true on
   every platform: say whether a binding author can know that, and what a
   false claim costs at run time (an error code C returns, or silent loss of
   mutual exclusion).
3. **Padding, where it can matter**: find a C API in the census headers that
   compares, hashes or copies a struct's bytes including padding (`memcmp` on
   addresses, a kernel copy-out), and run whether today's emitter shape and
   each route's emission hand it deterministic bytes. `padding.c` here is the
   method; its control fails at `-O2`, so build a better one or say `-O2` is
   unmeasured.
4. **Defect 091 against real headers**: write `sun_path` through its repair's
   shape (a loop of element stores) and through T, and run `connect` with it.
   A path byte of 128 or more: `path[i]` is a `u8` and `sun_path` is `i8`, so
   `to_i8()` fails for it (spec § 11: *`to_i8` … give a `T?`, because the number
   may not fit*). Run `sunpath_*_utf8.hero`'s path once 091 is repaired in your
   copy, and say whether a UTF-8 path can be written at all.

Every figure with the command that produced it, and *unrun* where you could not
run it.
