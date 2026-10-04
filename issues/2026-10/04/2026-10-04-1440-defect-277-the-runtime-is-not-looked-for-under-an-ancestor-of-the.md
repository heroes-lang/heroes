---
kind: defect
area: runtime
milestone: none
filed: 2026-10-04
commit: 7f875977bc876ab30fc23f7d2fbd985cc0feb62f
github: none
---

- [x] **277 — the runtime is not looked for under an ancestor of the executable, the third place panel 020 ruled, so an installed compiler finds none without `HEROES_RUNTIME`** | a compiler copied to `prefix/bin/heroes` with the runtime at `prefix/runtime/`, run from another folder with `HEROES_RUNTIME` unset: `build p.hero` exit 2, *cannot find the Heroes runtime (heroes_runtime.h and runtime.c). looked in: the given hint and ./runtime* (batch 8's round compiler, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/inst277/`) | `selfhost/cli/toolchain.hero:77` (`locate_toolchain`, two candidates, the hint and `runtime`) · design.md §3.1, line 709: *`$HEROES_RUNTIME`, then `runtime/` under the working directory, then `runtime/` under an ancestor of the executable*, ruled by panel 020 (`docs/panel/020-the-c-emitter.md:85` and `:328`) for *an installed compiler has no repository above it* · **class: adjacent**

    **Origin:** the coordinator, 2026-10-04, reading `locate_toolchain` for defect 276 against design.md's search order, and running the installed shape.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a ruled place the compiler does not search; its message is true, it names the two places it looked and the variable that reaches the third, and an installed compiler is not yet a shipped artifact.

    Repaired at `7f875977`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `7f875977`. The runtime is looked for under every ancestor of the compiler, the third place design.md §3.1 and panel 020 rule: the runtime answers the executable's own path (`hero_exe_path_shown`: `GetModuleFileNameA` on Windows, `_NSGetExecutablePath` then `realpath` on macOS, `/proc/self/exe` elsewhere), and where none holds a runtime the places looked in are named.

**Closed 2026-10-04** after batch 9's platform legs ran its cases on the tree that closes (`38d6c6b1`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8 and the same image under clang 18.1.8, the compiler's tests 1,190, all passed, and its 21 suites at 0 failed under each; the Windows box, clang 23.1.1 on Windows Server 2025 at code page 1252, the compiler's tests 1,190, all passed, and its 21 suites at 0 failed, `annotations` read again on `0470afd5`'s file after the floor's revision (its first read, 678 passed and 1 failed, was the floor counting the asked marks alone, six stepped aside there for headers the box lacks). The batch's gate on this Mac is the closing commit's body and the twenty-two records closed with it.
