# Defect 143 closed: a function-like macro cannot be bound

- [x] **143 — a function-like macro cannot be bound** | `extern "sys/wait.h"` with `function WEXITSTATUS(status: i32) -> i64`: `build` exit 1, *`sys/wait.h` declares no `WEXITSTATUS` — clang read the header and could not find it*, though the header defines it as a macro; design.md §1.11 says *Macros, `inline` functions and `#define` constants are now reachable directly* | the `extern` probe's parenthesized call (panel 092's `(fn)(...)`), which no function-like macro expands · `selfhost/emit/` (the probe) · **class: blocking** · **closed 2026-10-03**

    **Origin:** panel 184's ffi-pragmatist, 2026-09-30 (a header of its own
    and `sys/wait.h:144-146`); reproduced by the coordinator at 20:44 on the
    trunk's compiler at `a294a6ff` (`scratchpad/p184/ffi-side/macro.hero`);
    again at 00:17 on 2026-10-01 on `3cc3b553`, exit 1 and `ffi_unknown_name`.

    **Why it is a defect.** A program calling `waitpid` needs `WEXITSTATUS`,
    and the design says it is reachable; the FFI is to be complete (CL-028).

    **Widened 2026-10-02** by panel 185's ffi-pragmatist: glibc's `FD_ZERO`,
    a statement macro (`do { ... } while (0)`), takes the same false *declares
    no* (`scratchpad/185-ffi-pragmatist/p185/`); on macOS `WEXITSTATUS`,
    `htonl`, `FD_ISSET` and `WIFEXITED` are macro-only names alike. Panel 185
    sat on the route (its Q1); the seats measured that a call-form probe holds
    none of a macro's parameters, a `u8` declaration reading out of bounds.

    **2026-10-02, lane ffi-macro, panel 185 R1, a macro-only name is
    `ffi_macro_name`, its note drafting a function of the program's own with
    a placeholder for every C type**: repaired at `357589d6`, gated by its
    cases and the compiler's own tests; the net is owed at the batch's close,
    and so are Linux x86-64 (the gate's container), Linux arm64 and the
    Windows box, R1 being at the C boundary.

    **2026-10-03, its cases read one by one, held for the Windows box**: on this Mac (00:30 to 00:42 by `date`) and in the Linux arm64 container (one run, 00:36 to 00:54, Debian clang 22.1.8), with a compiler built from the trunk's seed at `2620bed1`, all twelve passed, `run/fixedbugs-143-*` 2 of 2 and `unsupported/fixedbugs-143-*` 10 of 10. Four of them bind `sys/wait.h` or `sys/select.h` (`run/fixedbugs-143-system-macros-through-functions-of-the-programs-own`, `unsupported/fixedbugs-143-a-macro-declared-at-a-wrong-width-drafts-no-width`, `-a-system-macro-is-named-as-a-macro` and `-the-fd-set-macros`), which the Windows box lacks (this list's item 158, whose origin is the first of them red there at `2bb45a96`): there they are told `ffi_missing_header` and skipped, and the leg of `6bec7c8c` read `run` 234 and `unsupported` 113 at 0 failed, short of the Mac's 242 and 119 by exactly the cases that can skip there, these four among them (deduced from the totals; the box did not answer on 2026-10-03). A case skipped on a platform does not close the item: it waits for those four to run on Windows, which needs headers the platform does not have, or for a ruling that a case on a POSIX header is judged where the header exists.

    **2026-10-03, lane ffimsg**: cases at `391c04d4` on headers of the tree's own for the three shapes only the four system cases witnessed, a statement macro told as a macro and reached through a function of the program's own behind a handle, and a macro declared at a wrong width drafted with no width (`tests/golden/run/fixedbugs-143-statement-macros-through-functions-of-the-programs-own.hero`, `tests/golden/unsupported/fixedbugs-143-statement-macros-of-the-programs-own-header.hero`, `tests/golden/unsupported/fixedbugs-143-a-macro-of-the-programs-own-header-at-a-wrong-width.hero`), gated by their own cases on this Mac; the rest is owed at the round's gate. The item closes when they run on the Windows box.

    **2026-10-03, read one by one at `02e507bc` with the Windows box answering, and the author's answer *A***: all twelve passed on this Mac and in the Linux arm64 container, under clang 22.1.8 and again under 18.1.8; on the Windows box (clang 23.1.1) eight passed and the four named above were skipped, each built alone there reading `ffi_missing_header` on `sys/wait.h` or `sys/select.h`, which the paragraph held for the Windows box had deduced from the totals. The author's answer *A* (`docs/records/log/2026-10-03-1123-the-author-answers-a-a-case-a-platform-cannot-run-is-judged-where-its-header-is.md`): those four are judged where their headers are, and the item closes once its twin, lane ffimsg's in-tree macro case, has run on all three.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a false message
    (*declares no*) on a header that defines the name; repaired and gated,
    closes after the push's platform legs.

    **Closed 2026-10-03** after the push of `e339ece9`. Repaired as the lines above record, and given its twins on headers of the tree's own by lane ffimsg at `391c04d4`, it entered the trunk at the round of 2026-10-03's sixth gate, `da3e29af` (lanes warn, ffimsg, depth, twin156 and win214): the seed regenerated once, 39,690,755 bytes, SHA-256 beginning `2c809845ed0f7ba8`, its fixpoint by `cmp`; the compiler's own tests 1,099 and the net's own 200, all passed; the full net, 26 suites, 5,033 passed and 0 failed; the censuses of `check --brief` over 1,905 files and of `--emit-c` over 595, every move attributed to its lane. Its cases read one by one at `da3e29af`, whose compiler `e339ece9` carries unchanged (no file under `selfhost/`, `seed/`, `runtime/` or `tests/` moved between them): on this Mac (Apple clang 21), in the Linux arm64 container under Debian clang 22.1.8 and again under 18.1.8, and on the Windows box (clang 23.1.1): `run/fixedbugs-143-*` 3 of 3 and `unsupported/fixedbugs-143-*` 12 of 12 on this Mac and in both Linux arm64 runs; on the Windows box 2 of 3 and 9 of 12, the four cases on `sys/wait.h` or `sys/select.h` skipped, each built alone there reading `ffi_missing_header`, and the three twins passing there. By the author's answer *A* (`.claude/rules/platforms.md`), those four are judged where their headers are, green on this Mac and under both Linux clangs. The push's legs: Linux arm64, its suites four at a time, the compiler's own tests 1,099 all passed and 20 suites at 0 failed under each clang; the Windows box, the compiler's own tests 1,099 all passed and nine of its 20 suites at 0 failed before it went offline at about 14:47 (Tailscale, read at 15:41: last seen 54 minutes before), every case above already read there; and the CI's run 37124159403 on `e339ece9`: Linux x86-64 (Ubuntu clang 18.1.3), the compiler's own tests 1,099 all passed and 26 suites, 4,993 passed and 0 failed; Windows x86-64 (clang 20.1.8), 1,099 and 26 suites, 4,932 passed and 0 failed, the box's eleven unrun suites among them; Linux arm64 and Darwin arm64 green. Which golden cases those jobs ran their logs do not say, so the readings above are the measurement of them, and x86-64's an inference from Linux arm64 under the same clang major. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
