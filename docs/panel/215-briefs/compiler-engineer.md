# Panel 215, the compiler-engineer's brief

Read `00-shared.md` first. Your question is the ceiling and the soundness of each route in the live
compiler (`selfhost/`, `runtime/`).

1. **Q1**: run panel 178's critic's padding probe in the working copies this sitting's critic repaired
   (`docs/panel/215-reports/critic-work/pad/`; the originals no longer build) 
   with this tree's compiler on Linux x86-64 (emulated, `heroes-linux`) and arm64 under
   MemorySanitizer, and on Windows if the box has it, at `-O0` to `-O3`; then B is measured not to close it (`00-shared.md`); price C, D, E and any route you find in
   `selfhost/emit/` (where a group record is constructed, copied and passed: read `construct.hero`,
   `zeroed.hero` and the call lowering), build the route you adopt far enough that the probe reads
   clean, and measure its cost on a cold `--emit-c` of the compiler (CPU time; say the load).
2. **Q2**: price a mark that refuses `=` and a by-value parameter for a group record (B) against the
   same decided from the header (C): where the checker would refuse it (`selfhost/check/`), what the
   copy-free uses must still allow (a cell passed by `@`, a field lend), and what the 878 programs with
   an `extern` would meet (build them with the prototype: none may move unless it copies such a
   record). Run issue 1335's measurement yourself first.
3. **Q3**: panel 194's second objection condition, *a nonzero byte under MemorySanitizer after `rest:
   zero`*: run it on Linux (x86-64 emulated and arm64), a union and padding included; and what
   withdrawing R1 would cost the tree (the cases, `zeroed.hero`, the emitter's paths).

Veto on soundness: a route that leaves a wrong value or uninitialised bytes reaching C where it claims
to close them, measured.
