# Panel 153 — brief for the ffi-pragmatist

Read `docs/panel/153-briefs/00-shared.md` first. Your report goes to
`docs/panel/153-reports/ffi-pragmatist.md` — write it in your COPY and hand the
text back in your final message; the coordinator writes the file.

## What you judge

The founding constraint (design.md §1.11, §4.19): there is no standard library
and everything comes from C. **Write and compile the C each route implies**,
and say which binding a real program could not write under each. You have a
veto on ABI breakage.

## The four programs to write, in C first and then as the Heroes each route would emit

1. **`getaddrinfo` and the walk.** Resolve `"127.0.0.1"` with `hints` set to
   `AF_UNSPEC`/`SOCK_STREAM`, walk `ai_next`, print each `ai_family`, free the
   list. Under **Route A** the Heroes is a handle with fields read through the
   pointer (`ai->ai_family`, `ai->ai_next`); under **Route B** it is a fielded
   record copied out of the handle (`struct addrinfo a = *ai;`) and a pointer
   field walked. **Compile both C shapes under the compiler's own flags**
   (`selfhost/cli/flags.hero`, `flags()` — read the list, it includes
   `-Werror=incompatible-pointer-types`), on this Mac.
2. **`hints` by pointer.** `const struct addrinfo *hints` given the address of a
   Heroes value. `@hints: AddrInfo` is copy-in/copy-out: the emitter passes
   `&local` and copies back after the call. Compile `struct addrinfo *` against
   `const struct addrinfo *` under the flags; then say what the copy-back COSTS
   and RISKS when C promised not to write (it did not write; is the copy a
   correctness hazard or a wasted memcpy?).
3. **`lstat` and `struct stat`.** `int lstat(const char *, struct stat *)`: the
   struct is an OUT-parameter by pointer and its fields are read afterwards.
   Which route binds it, and does either need the other's construct? Note
   `examples/ctime/main.hero` already reads `struct tm` fields by value through
   `tag tm partial`, so the by-value read exists; what is missing is the
   pointer-returned case.
4. **The null read.** `struct addrinfo *ai = NULL; ai->ai_family;` — run it and
   record the signal. design.md §1.12 says a Heroes program must not segfault:
   what does the emitter have to write before every read through a handle, and
   what does it cost? Measure a read with and without the guard on a loop of a
   million reads at `-O0` and `-O2`.

## Then the shipped bindings

`examples/sqlite/main.hero`, `examples/ledger/db/sqlite.hero`,
`examples/curl/main.hero` declare five handle types. **Does either route change
one byte of the C those emit?** Build each in your copy before and after
reasoning about it; `HEROES_RUNTIME=<copy>/runtime`. If a route needs a change
to any of the five, that is ABI breakage and you say so.

## Q2 is yours too

Compile and run the corrected fence in your copy against the SDK's own
`sqlite3.h`:

```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
    function sqlite3_open(path: cstr, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64
```

with `tests/harness/suite_special.hero:330`'s `main`. Then write the program a
reader who COPIED THE CURRENT FENCE would write — open, no close — and record
what the compiler and the runtime say about it (exit code, stderr), because the
defect claims *nothing*.

## What your report must carry

Every compile's exit code and the first line of stderr where it failed; the C
you wrote, verbatim, for the four programs; the five example bindings before
and after; one prediction with the command that scores it. Run it, or say in
your own words that it is unrun.
