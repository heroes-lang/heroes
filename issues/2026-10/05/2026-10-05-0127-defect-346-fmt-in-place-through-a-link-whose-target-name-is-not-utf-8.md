---
kind: defect
area: runtime
milestone: none
filed: 2026-10-05
commit: 805bae193f20f37e40b5f6f27ef80a3f0187d76d
github: none
---

- [x] **346 — `fmt --in-place` through a link whose target name is not UTF-8 says *the operating system's own reason is 84*, a number the runtime set** | on Linux, `heroes fmt` over a link whose target's name holds a byte that is not UTF-8: the message gives *the operating system's own reason is 84*, and 84, `EILSEQ`, is set by the runtime itself (`runtime/parts/replace.c:94`), not by the system (lane b11-windows, Linux arm64, 2026-10-05, the lane's report) | `runtime/parts/replace.c:94` (the `errno` it sets) · the compiler's wording of a refusal's reason · **class: blocking**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 238's link check (its final report, *Found beside*), read `adjacent` by the lane.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a false message, the reason attributed to the operating system being the runtime's own, which `blocking`'s list names; the lane read it `adjacent`.

    **2026-10-05:** Repaired at `805bae19`, gated by its case red first (1,218 tests, 1 failed) and the compiler's own tests on this Mac and on Linux arm64 (1,218, all passed each), its reproducer through `fmt --in-place` on Linux now telling the runtime's reason; the net is owed at the batch's close, and the Windows box its case.

## The repair

Repaired at `805bae19`, its case at `f28107e2`. The runtime's own refusals before the system was asked (an empty path, a path past the runtime's limit, a link chain past forty, a link target that is not UTF-8, and on Windows a name the wide calls cannot take) were told as *the operating system's own reason*; the runtime stores its own reasons as negative numbers, which no system returns (`hero_fs_why`), and the compiler tells *the runtime refused it before the system was asked, for the reason the system numbers N*. Its cases are compiler tests on this Mac and Linux.

**Closed 2026-10-05**, after batch 11's platform legs, each on `2dd5611c`, its tree as pushed at `61e085ae` but for records: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,233, all passed, and 22 suites, 5,244 passed and 0 failed, each time; the Windows box under clang 23.1.1, lane b11-windows' three trees, each with its compiler built from the seed and then from `selfhost/`: in `land`, the round's own runtime, the net's own tests 280, all passed, the compiler's own tests 1,233 with one failed, defect 337's, and 21 of 22 suites 0 failed, `unsupported` 130 passed and 1 failed, 337's other case; in `base`, the runtime as it stood at `7c615049`, the compiler's own tests failing 337's, 345's, 346's and 239's, and the net's own failing 12, 238's eleven and 239's walk; `heroes doctor` exit 0 in `land` with its MSVC row, where batch 10's compiler on the same box says *FAIL cc not found* with Linux's advice at exit 2. The CI after the push, at `61e085ae`: Darwin arm64, Linux arm64 and Linux x86-64 green; Windows x86-64 one of the net's own tests failed, defect 238's W16 and W21, its fixture's `clang -shared` exiting 1120 under clang 20.1.8, a row of 238, which stays open.
