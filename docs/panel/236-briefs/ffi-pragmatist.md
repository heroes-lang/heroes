# Panel 236, ffi-pragmatist's brief

Read `00-shared.md` and `proposal.md` first, then the record they name: your predecessor's report
`docs/panel/233-reports/ffi-pragmatist.md` § 2 (*R7*, veto (a)) and § 7 first, then § 3b; and the
critic's first pass, `236-reports/critic-briefs.md`, on which these briefs were repaired. Your
worktree is `.claude/worktrees/236-ffi-pragmatist/`, your scratch
`.claude/worktrees/236-ffi-pragmatist-scratch/`, your report `docs/panel/236-reports/ffi-pragmatist.md`
in your worktree, written as you go.

**Your task: the C each route implies at the boundary, written and compiled against real headers.**

1. **Veto (a), again, for group records** (question 3): `extval` (an `extern` returning `Held` and a
   Heroes function in one function type, handed to C), `cbparam`, `cbresult` and `cbnamed`, in
   `00-shared.md`'s `probe/`. Write by hand, and compile with the build's own words
   (`selfhost/cli/flags.hero`'s `flags()`) at `-O0` and `-O2`, the C of **the proposal as written**
   (values take the out-parameter; an `extern` used as a value wrapped in an adapter; a C-convention
   adapter where a Heroes function reaches C) and of **R7b** (values and thunks keep C's convention,
   only a call by name takes `hero_out`); run each against a C function that calls the callback, and
   report: the bytes C receives match what the Heroes side wrote; the adapter's frame
   (`-fstack-usage`); the typedef C sees, byte for byte against the base's `--emit-c`; and the
   per-thunk `_Static_assert(__builtin_types_compatible_p(...))` refusing your predecessor's R7c shape
   in each. Your verdict on ABI breakage rests here.
2. **An `extern`'s by-value parameter or result** of such a record (question 4, `grpextonly`): what a
   program meets today at both levels (the run and its message) for a parameter, a result and both,
   and what C fact any rule rests on. A `partial` record's size is the header's, not Heroes'. If you
   find a refusal or a message the most robust answer, report it as the full panel's, with the
   program that shows it; this lane rules the emitted C alone.
3. **Targets** (question 1): which of the routes' rules hold the same C on `arm64-apple-darwin`,
   `x86_64-pc-linux-gnu`, `aarch64-unknown-linux-gnu` and `x86_64-pc-windows-msvc`, by compiling for
   each with `-target` where clang can (panel 233's critic: `sizeof` is a target's fact, LLP64 against
   LP64); the Windows box (`ssh win`, `docs/platforms/windows/WINDOWS-MACHINE.md`) answers today and
   may run your boundary C, nothing on it changed but a scratch folder of yours. Windows links each
   program with a 64 MiB stack (`fixedbugs-2253-held.h`'s comment).
4. **Panel 215's R1** at the boundary: where a record crosses through `hero_out` or a pointer and C
   then reads it, its padding is zero; write the C that shows it holds or does not.
5. **Part (a)** from the C side (question 6), on the 64 KiB chains only (the 1 MiB chains are the
   compiler-engineer's to run): your predecessor measured `noinline` on the builders at 1 MiB taking
   clang to 0.49 billion instructions; say whether the routes make that unnecessary, and what remains
   on the frame beside it.

Give **one falsifiable prediction** (a number with the instrument that reads it), your verdict (adopt,
adopt with conditions, or veto on ABI breakage, naming the C that breaks), the route you would adopt
between question 3's two, and the platform runs the landing owes on the four CI legs.
