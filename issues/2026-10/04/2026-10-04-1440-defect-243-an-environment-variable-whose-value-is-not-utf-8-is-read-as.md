---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: ea5c22ca7f6989ab3585972bfcdcd8fc171f0820
github: none
---

- [x] **243 — an environment variable whose value is not UTF-8 is read as unset, so a `HEROES_RUNTIME` naming a real runtime is answered *looked in: the given hint* and told to set it** | Linux arm64, `HEROES_RUNTIME=/root/rt<e9>` naming a real runtime: `build` exit 2, *cannot find the Heroes runtime (heroes_runtime.h and runtime.c). looked in: the given hint and ./runtime. set HEROES_RUNTIME=<dir> to say where it is*, and `doctor` *not found* (the trunk's compiler at `7d9f2e8f`, panel 189's ffi-pragmatist, 2026-10-04); `env` answers `""` for a value that is not UTF-8, `validated(c: getenv(...)).default("")` (read by the coordinator, 2026-10-04), so the hint is never looked in; on Windows every accented value is one, through the narrow `getenv` (defect 238) | `selfhost/cli/process.hero:214` (`env`) · `selfhost/cli/toolchain.hero:95` (the message) · panel 189's Q6 · **class: blocking**

    **Origin:** panel 189's ffi-pragmatist, 2026-10-04 00:29 (`docs/panel/189-reports/ffi-pragmatist.md`, the Linux arm64 table); the message read against `env` by the coordinator; filed apart.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message, *looked in: the given hint* for a hint never read, with a route telling the author to set what they set. The seat read it `adjacent` on Linux; the coordinator classes it by the list's *a false message*.

    **2026-10-04, panel 189's compiler-engineer and the coordinator**: the hint read as unset falls through to a `./runtime` beside the program when one is there, so the build succeeds against a runtime the author did not name, no word said (its Q6; run by the coordinator with a hint that is not UTF-8 and a copy of the runtime beside `p.hero`, exit 0, `<scratchpad>/filings-b8/probe/rt243/`). The fall-through itself, for any hint that names nothing, is defect 276.

    Repaired at `ea5c22ca`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-04, batch 9's annot lane**: the harness's own `env` (`tests/harness/shell.hero`) repaired at `c539f58d`: a value that is not UTF-8 is named, so a `HEROES_COMPILER` in such bytes ends the run at exit 2 saying so where the net judged `./heroes`, and such a `HEROES_RUNTIME` refuses every skip where clang searched `runtime`. Gated by its cases (one runs the harness's own modules under a child given the bytes), the net's own tests and 13 suites whole (no `selfhost/` line moved); the net is owed at the batch's close.

## The repair

Repaired at `ea5c22ca`, the harness's own `env` at `c539f58d`. An environment value that is not UTF-8 is named, never read as unset: `HEROES_RUNTIME` so is exit 2 with its value quoted, `/nowhere/rt<0xE9>`, and what to set; `doctor` says FAIL with the same words; the API key is named and never quoted.

**Closed 2026-10-04** after batch 9's platform legs ran its cases on the tree that closes (`38d6c6b1`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8 and the same image under clang 18.1.8, the compiler's tests 1,190, all passed, and its 21 suites at 0 failed under each; the Windows box, clang 23.1.1 on Windows Server 2025 at code page 1252, the compiler's tests 1,190, all passed, and its 21 suites at 0 failed, `annotations` read again on `0470afd5`'s file after the floor's revision (its first read, 678 passed and 1 failed, was the floor counting the asked marks alone, six stepped aside there for headers the box lacks). The batch's gate on this Mac is the closing commit's body and the twenty-two records closed with it.

**Corrected underneath, 2026-10-04, panel 191** (`docs/panel/191-on-windows-a-name-reaches-the-runtime-through-a-utf-8-code-page-its-own-object-carries-and-a-program-refuses-to-start-without-it.md`, R7): on Windows the repaired message is false for a valid value. `HEROES_RUNTIME` naming a real runtime under `rté` makes `doctor` say FAIL, *`rt<0xE9>` is not UTF-8* (the compiler-engineer's row 8, the trunk's runtime at `7f4c0cc5` on the box): the value is valid Unicode, and the narrow `getenv` read it in code page 1252, which is defect 238, as this item's own line says of every accented value there. On Linux the message holds. 238's landing repairs it (panel 191's R1).
