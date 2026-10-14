# Panel 215, the ffi-pragmatist's brief

Read `00-shared.md` first. Your question is the C each route implies, compiled on the three platforms.

1. **Q1**: which real C APIs read a struct's padding (write, send, a hash of the bytes, `memcmp`):
   write the bindings and show, under MemorySanitizer on Linux, whether each route (B, C, D, E) closes
   what reaches C; and whether C (by address) or D (a zeroed temporary) changes the ABI of an `extern`
   that takes a record by value (`HERO_RUNTIME_ABI`, a callback's signature).
2. **Q2**: the C types that must not be copied, on the three platforms: `pthread_mutex_t`,
   `pthread_cond_t`, `FILE`, `va_list`, `jmp_buf`, atomics, and whatever C marks so (an attribute? a
   documented rule only?). Can route C decide from the header alone, and how often would it be wrong?
   Bind and run a copy of each on this Mac, Linux arm64 and the Windows box.
3. **Q3**: the bindings measurement 044 names (`sockaddr`, `SHA256_CTX`, `fd_set`) written with `rest:
   zero` and without it: what the program writes in each case, compiled.

Veto on ABI breakage, and on a route that cannot bind a header today's compiler binds.
