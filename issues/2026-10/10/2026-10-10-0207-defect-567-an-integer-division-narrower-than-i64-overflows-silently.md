---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: 4a9ab236abb2a3395f6da9fb521fa752206cee0c
github: none
---

- [ ] **567 — an integer division narrower than `i64` overflows silently, and its guard warns at `heroes run`** | `x: i8 @ -128`, `y: i8 @ -1`, `print(x / y)` builds and prints `-128` at exit 0, where spec § 7 has an overflow abort at every width; at `heroes run` (`-O2`) clang warns on the correct program, *result of comparison of constant -9223372036854775808 with expression of type 'int8_t' ... is always false [-Wtautological-constant-out-of-range-compare]*, at the emitted `if (t3 == INT64_MIN && t4 == INT64_C(-1)) hero_panic_overflow();`; the same program at `i64` aborts 134, *panic: integer overflow*; measured by the coordinator at 02:06 with the trunk's compiler | the division and remainder guard, `selfhost/emit/operator.hero:148` (lane b18-ffi's reading), which compares with `INT64_MIN` and `-1` at every width; the shapes beside: `%`, `i16`, `i32`, the unsigned widths (`u8 x / 2` warns, the lane's `udiv`), a constant divisor, `-O0` · **class: blocking**

    **Origin:** found by lane b18-ffi at 01:35 on 2026-10-10 beside its own work (its notes, `.claude/worktrees/scratch-b15/b18-ffi/notes.txt`, ignored by git, reproducers `probes2/i8div.hero` and `probes2/udiv.hero`), reproduced and filed by the coordinator at 02:07 (`.claude/worktrees/scratch-b15/div567/`).

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0, and a clang warning on a correct program.

    Repaired at `4a9ab236`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The guard of `/` and `%` moved to `emit/division.hero` and takes the operands' own width: a signed width compares with its own `INTn_MIN` and `INTn_C(-1)`, an unsigned one takes no guard, `i64`'s text byte for byte what it was, and `%` at a narrow width says why it aborts, naming the width. Beside it, the same cause: at `u64` the old guard's constants converted to 2^63 and the maximum, so the correct `9223372036854775808 / 18446744073709551615` aborted *integer overflow*. Eight `run/fixedbugs-567-…` cases, each red on the base (six abort 134 with their panic where the base printed at exit 0 with the warning, two print where the base warned or aborted); `run` narrowed 8, `warnings` 498, `determinism` 472, `emission` 1104, the compiler's 1,546 tests, 0 failed.
