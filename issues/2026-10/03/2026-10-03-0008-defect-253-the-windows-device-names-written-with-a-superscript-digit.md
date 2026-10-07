---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: aac84d00583ed90939f30e0460e0f5612407a504
github: none
---

- [x] **253 — the Windows device names written with a superscript digit, `COM¹` to `COM³` and `LPT¹` to `LPT³`, and `CONIN$` and `CONOUT$`, are outside the device rule, and what NTFS does with them is unmeasured** | `unwritable` refuses CON, PRN, AUX, NUL and COM0 to COM9, LPT0 to LPT9 by an ASCII digit (`selfhost/head_windows.hero:82` to `:92`), so `COM¹.h` (`43 4f 4d c2 b9`) passes on every platform; Microsoft's list of reserved names, which batch 8's FFI lane recalled and did not read, adds the superscript forms and the console's two (its report's finding 5, 2026-10-03) | `selfhost/head_windows.hero` (the device names) · the Windows box · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 5, a question).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on the box; no program measured wrong.

    **2026-10-07, lane b14-box**: Repaired at `aac84d00`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The device rule refuses `COM¹` to `COM³`, `LPT¹` to `LPT³`, `CONIN$` and `CONOUT$` in any ASCII case, on the part before its first `.` with its ending spaces dropped, and admits `COM0` and `LPT0`, as Microsoft's *Naming Files, Paths, and Namespaces* (read that day) and the Windows box (10.0.26100, each bare name the device by `RtlIsDosDeviceName_U` and `GetFullPathNameW`, `COM0` and `LPT0` files) read them; its case `fixedbugs-253-a-device-named-by-a-superscript-digit-or-the-console` holds ten refusals and five passes, and the compiler's 1,358 tests passed.

## The repair

**2026-10-07, lane b14-box**: Repaired at `aac84d00`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The device rule refuses `COM¹` to `COM³`, `LPT¹` to `LPT³`, `CONIN$` and `CONOUT$` in any ASCII case, on the part before its first `.` with its ending spaces dropped, and admits `COM0` and `LPT0`, as Microsoft's *Naming Files, Paths, and Namespaces* (read that day) and the Windows box (10.0.26100, each bare name the device by `RtlIsDosDeviceName_U` and `GetFullPathNameW`, `COM0` and `LPT0` files) read them; its case `fixedbugs-253-a-device-named-by-a-superscript-digit-or-the-console` holds ten refusals and five passes, and the compiler's 1,358 tests passed.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
