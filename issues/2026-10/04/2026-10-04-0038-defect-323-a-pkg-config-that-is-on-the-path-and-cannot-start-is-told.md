---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 4370bd60df08fb7114bbf4812e7385000ef94ee1
github: none
---

- [x] **323 — a `pkg-config` that is on the PATH and cannot start is told *pkg-config is not on this machine*, a false message** | a `pkg-config` on the PATH whose file the machine refuses to start (mode 644, the operating system's reason 13, `EACCES`): `build` of a group naming a `package` is told *pkg-config is not on this machine*, which is false; the program is there and could not be started (lane b10-cli's compiler at `86b29733`, 2026-10-04, found beside defect 249) | `selfhost/cli/package_answer.hero:88` (the message, written for every start failure) · `selfhost/cli/toolchain.hero:195`, where `run_clang` already asks the operating system's own reason · defect 249's repair · **class: blocking**

    **Origin:** lane b10-cli, 2026-10-04, found beside 249 and reproduced on its compiler; filed by the coordinator for the same lane, being blocking and in its files.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message about the machine (`.claude/rules/verification.md` § Bounded discovery's *a false message*); a cause apart from 249's, which was the bare line's missing code, place and route.

    Repaired at `4370bd60`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `4370bd60`. Every start failure of `pkg-config` was told *pkg-config is not on this machine*, a file of mode 644 on the PATH among them, which the machine refuses with 13 and which is there. The message now asks the operating system's own reason, as `run_clang` asks it of clang: 2 alone keeps *not on this machine*, and any other reason is *pkg-config could not be started*, with the number, both the package's `ffi_package` at its string. Measured by hand on this Mac: a mode-644 `pkg-config` and a directory of that name, each 13; none on the PATH, 2, unchanged. Its case is a compiler test starting a mode-644 file by its path.

**Closed 2026-10-05**, after batch 10's platform legs, each on `4c3524fb`, the batch's closing tree: Linux arm64 in its container under Debian clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,213, all passed, and 22 suites, every one 0 failed, each time; the Windows box under clang 23.1.1, the compiler's own tests 1,213, all passed, and 21 of its 22 suites 0 failed in the leg's folder, `unsupported` reading 127 passed and 2 failed there, two `fixedbugs-157-*` cases told `internal error` over three NUL bytes, because the box's crash at 18:46 on 2026-10-04 had left 11 files of that folder's build cache as zeros (defect 357); `unsupported` from the same archive with the same `heroes.exe`, in a fresh folder, 129 passed and 0 failed.
