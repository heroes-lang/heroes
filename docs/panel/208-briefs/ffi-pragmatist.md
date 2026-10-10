# Panel 208, ffi-pragmatist

Read `00-shared.md` beside this file first. Your directory:
`.claude/worktrees/scratch-b15/208-ffi-pragmatist/` (its `tree/` is
`391628b6`). Your ground is the founding constraint (design.md §1.11, §4.19:
there is no standard library, everything comes from C); you hold a veto on ABI
breakage and on a route that leaves a needed library unbindable.

1. **What a real program meets.** Count, with commands named: the functions
   this Mac's SDK marks deprecated that a Heroes program plausibly binds
   (`sprintf`, `vsprintf`, `mktemp`, `tmpnam`, `tempnam`, `getcontext`,
   `gethostbyname`, `system` on iOS-like targets, `syscall`, `sem_init`,
   ...; derive the list from the headers, not from memory), the same on Linux
   arm64 in the Docker image `heroes-linux-arm64:latest`
   (`docs/platforms/linux/LINUX-MACHINE.md`; glibc marks few), and a library
   whose current version deprecates heavily (OpenSSL 3's `RSA_*`, `SHA256_Init`;
   use what the machine has, `pkg-config --list-all`). Which of the 21
   `examples/` files with an `extern` call a deprecated name on either
   platform, and what each of (R), (S), (M), (F) does to them.
2. **Write and compile the binding C each route implies** for the shapes in
   the shared brief: a deprecated function called, a deprecated record in a
   signature, a deprecated enumerator, `warning(...)`, `unavailable`,
   `availability(macos, deprecated=...)`, `#pragma clang deprecated` on a
   macro, `[[deprecated]]`. Say on which platform each fires, what clang
   prints, and whether a Heroes program under (R) could still be written for
   the job (a non-deprecated replacement exists and binds) or would be left
   with no route at all.
3. **Explain, from the C side, why `sprintf`'s call is silent** with lane
   b18-guard's compiler while `twice`'s is not (the shared brief's
   asymmetry), measured on the emitted unit; the compiler-engineer measures
   the same from the compiler's side, so your two answers are checkable
   against each other.
4. Report to `docs/panel/208-reports/ffi-pragmatist.md`: the counts with
   their commands, a table of shapes by platform and route, your verdict per
   route, your recommendation, and one prediction someone can score at the
   landing or at the CI's legs.
