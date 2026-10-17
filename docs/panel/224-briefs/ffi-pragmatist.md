# Panel 224, ffi-pragmatist

Read `00-shared.md` first. Your seat: the C a real binding needs (veto on ABI breakage).

- Measure item 2 on the box: compile, with and without `-fno-wrapv`, a unit that includes the
  headers the Windows CI leg's examples bind and the UCRT's, at `-O2`, and say whether any inline
  function's code changes (`-S` and a diff, or `llvm-objdump`), naming the function.
- Say whether `-fno-wrapv` is what the C standard means on every target the CI runs (the four legs),
  and whether a header written for MSVC can rely on wrapping in a way a Heroes binding would
  reach (MSVC does optimise on signed overflow: the critic measured `cl.exe` 14.44 at `/O2`
  folding `(x * 2) / 2` to `x`).
- Verdict per route (approve / object / veto), the condition, one falsifiable prediction.
