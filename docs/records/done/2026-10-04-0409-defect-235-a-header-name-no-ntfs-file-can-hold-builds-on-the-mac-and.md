- [x] **235 — a header name no NTFS file can hold builds on the Mac and Linux and is missing on Windows** | `*`, `<`, `?` and `|` in a header's name: `CreateFileW` refuses each (Win32 error 123), so on Windows `build` says *missing header*, true there, where this Mac and the Linux arm64 image build and run (the ffi-pragmatist, the same section, 2026-10-03) | the leaf's judgement (`selfhost/head_names.hero` after panel 188's landing) · panel 188 R4 (which already refuses `/*`) · **class: adjacent**

    **Origin:** panel 188's ffi-pragmatist, 2026-10-03, the same measurement; read by the coordinator from the seat's tables, not yet run by the coordinator.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a divergence between platforms whose messages are true on each; ruled with 234.

    **2026-10-03, batch 8's FFI lane**: repaired at `24cb4fd4`, gated by its cases and the compiler's own tests; the net is owed at the batch's close (the line added at the round's merge, the item having been filed on the trunk after the lane's base).

## The repair

Repaired at `24cb4fd4`. `*`, `<`, `?` and `|`, which no NTFS file can hold, are refused in a group head's name on every platform. Case: `check/fixedbugs-235-a-name-no-windows-file-can-hold`.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
