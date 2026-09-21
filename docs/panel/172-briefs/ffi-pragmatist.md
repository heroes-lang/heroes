# Panel 172 — brief for the ffi-pragmatist

Read `docs/panel/172-briefs/00-shared.md` first. You judge the founding
constraint — everything comes from C (design.md §1.11), the boundary is §4.19 —
and you hold a veto on ABI breakage.

## Your axis

Does a `consumes` on a `cstr` or `ptr` parameter say something TRUE of real C
functions, can a binding author tell when to write it, and does it change one
byte of emitted C? And under route B: what does it cost to bind a real library
when every pointer parameter must carry a word before it takes a lease?

## Build, in a copy

    cp -r /Users/joseph/Temp/heroes/heroes-lang <scratch>/ffi-pragmatist-172
    cd <scratch>/ffi-pragmatist-172 && rm -rf build
    clang -I runtime seed/heroes.c runtime/runtime.c -o heroes      # ~3 s

The shipped compiler refuses `consumes` on a `cstr` today (`unread_mark`), so
you cannot compile route A's declarations; you can compile everything around
them and read the C.

## Tasks

1. **Classify real functions from the world**, by writing and compiling C
   against the SDK on this Mac: which take a `char *`/`void *` and FREE it or
   take ownership? Candidates: `free`, `sqlite3_free`, `putenv` (POSIX: *shall
   become part of the environment* — keeps, never frees; glibc frees on a later
   `setenv`?), `sqlite3_bind_text` with `SQLITE_STATIC` versus a real
   destructor (keeps, then frees with the destructor), `CFRelease`,
   `g_free`/`g_strdup`, `freeaddrinfo`, `execv`'s argv. For each: does
   `consumes` describe it truthfully? Where the truth is *keeps and later frees
   with a function I name* (bind_blob with a destructor), which word is true?
2. **The give-away route that exists**, `docs/measurements/037-the-give-away-case-was-writable-the-moment-the-callback-was.md`
   (R5: the author allocates through a bound allocator, C frees with the
   disposer it is handed). Re-run its program in your copy. Under route A, does
   anything in it change? Under route B?
3. **The ABI question**: read the emitted C for a call where the parameter
   would carry `consumes` (write the declaration WITHOUT the word, since the
   compiler refuses it today, and `--emit-c`); the word must reach no C. Say
   what would be a veto.
4. **Route B's cost on a real header**: count `const char *` and `void *`
   parameters in `sqlite3.h` and `curl/curl.h` on this SDK
   (`grep -c` with the command shown), and estimate how many a binding of the
   ledger's 18 functions would have to word. Is the keeps-word ever TRUE of a
   `const char *` parameter in those two headers, or is every keeper decided
   per call (bind_text's fifth argument, setopt's option), in which case route
   B's keeps-word cannot be written truthfully anywhere it is needed?
5. **What a binding author would write wrongly**: for `strdup(s: cstr)`, the
   golden `tests/golden/check/unread-mark.hero:26` says `consumes` — false.
   For `free(p: ptr)` — true. How would a reader with the man page tell? Say
   whether the diagnostic's notes should carry a test (*if the man page says
   the pointer is freed or owned by the callee*).

## Report

`docs/panel/172-reports/ffi-pragmatist.md`: verdict · section · cost · one
falsifiable prediction naming an instrument that exists · veto condition · the
C you compiled, with the commands and their exit codes.
