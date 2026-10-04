---
kind: defect
area: emit
milestone: none
filed: 2026-10-03
commit: b9c4bda012c30cbd042b906d88ca63d8f08901f4
github: none
---

- [x] **226 — `ffi_package` points at the group's first member, not at the `package` string it is about** | `extern "ab.h" package "zz9nothere"` over `function seven() -> i32`: `build` 1, *the package `zz9nothere` is not installed on this machine*, `at p.hero:2:5`, the member's line, where the string is on line 1 (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro188/pkg-span/`) | `selfhost/emit/ffi_build.hero` (where `ffi_package` takes its span) · **class: adjacent**

    **Origin:** panel 188's spec-warden, 2026-10-03; reproduced by the coordinator the same day.

    **Class: adjacent**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    **2026-10-03, batch 8's FFI lane**: repaired at `b9c4bda0`, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `b9c4bda0`. `ffi_package` points at the `package` string it is about. The census read five messages move from the group's first member to the string, `unsupported/fixedbugs-226-a-package-not-installed-is-told-at-its-string` among them.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.

**Corrected 2026-10-04 at 09:53, read against the legs' own counts: on Linux arm64 the legs did not judge this item's case, and on the Windows box whether they did is unrun.** The `unsupported` form of that day counted a case it skipped as neither passed nor failed, and named none below its one-third floor (defect 246). It read 133 passed on Linux arm64 under both clangs and 129 on the Windows box, against 137 on this Mac at the same code (`703af779`, whose harness, cases, compiler, runtime and seed equal tree `7ec19cb7`'s, read by lane b9-harness, `<scratchpad>/batch9/harness/base-unsupported.txt`). Run again in the same image (Debian clang 22.1.8, pkgconf), in a run that ended at 09:51:52 (`<scratchpad>/probe-b9-unsupported/arm64-c22.log`), the trunk's compiler differs from four expectations of 136, this item's case among them. For this case the difference is only `pkg-config`'s note, which pkgconf words *Package '...', required by 'virtual:world', not found*. The headline and its place, which are what the repair moved, read as expected. In the same run, batch 9's form, which judges that note against the machine's own answer, judged this case and passed it: 136 passed, 0 failed, one case stepped aside by name for `raylib`. The Windows box's eight skips were never named, and the box has not answered since about 09:03; batch 9's Windows leg names every case it steps aside.
