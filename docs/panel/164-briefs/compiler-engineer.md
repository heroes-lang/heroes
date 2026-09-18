# Panel 164 — compiler-engineer

Read `00-shared.md` first. Veto on soundness.

## What you are asked

Price the four routes to IMPLEMENT, and say what a fixed byte field at a call
argument position emits today and would emit under each.

1. **Where the mismatch is decided.** `slot_len(s.name)` is `type_mismatch` at
   the call. Find the comparison in `selfhost/check/` and say what admitting a
   `.fixed` of 8-bit elements against `.cstr` or `.ptr` costs there — and
   whether the same site already admits anything else by shape rather than by
   identity (a `str` against `cstr`? no — measure it).
2. **What the emitter would write.** At a call, `emit/ops.hero` renders
   arguments; a fixed value has no temporary (`emit/storageless.hero`). Say
   whether `fixed_text` at an argument position gives `h0_s.h_name`, which C
   decays, or whether something must be written. Then say what
   `guard_arguments` does with it — the null guard exists for a `cstr`
   argument and a field is never null, so is the guard wrong, wasted, or right?
3. **The lend's position rule.** `check/lending.hero` refuses `s.cstr()`
   anywhere but as a call argument (panel 122 R2). Does that rule transfer to a
   field lend unchanged, and does `check/leasing.hero` need to know?
4. **The route nobody listed**, especially: is there a way the CHECKER can know a
   field is terminated? (Panel 162 measured the runtime cannot without a scan.)

Copy, seed, never `selfhost/` rebuild, never `archive/`. Every number from a
command you name.
