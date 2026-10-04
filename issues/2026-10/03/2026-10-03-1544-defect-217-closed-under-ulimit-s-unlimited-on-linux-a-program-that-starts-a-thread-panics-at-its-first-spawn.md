---
kind: defect
area: examples
milestone: none
filed: 2026-10-03
commit: ffaf9f4dc40f97c82379b5e16a0f84506c5e359c
github: none
---

# Defect 217 closed: under `ulimit -s unlimited` on Linux, a program that starts a thread panics at its first spawn

- [x] **217 — under `ulimit -s unlimited` on Linux, a program that starts a thread panics at its first spawn** | `examples/threads` as it stood at `02e507bc`, run in the Linux arm64 container with the stack limit unlimited: a panic at the first spawn, glibc reporting the main thread's stack as 93,823,035,207,680 bytes, from which `runtime/parts/spawn.c` asked a floor | `runtime/parts/spawn.c` · lane depth's commit `ffaf9f4d` · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane depth's first pass beside defect 169, 2026-10-03, reported to the coordinator (`<scratchpad>/lane-depth/linux2/`).

    **2026-10-03, repaired at `ffaf9f4d` in lane depth, beside defect 169**: under an unlimited stack limit no floor is asked. Owed at the round's gate, and on the platforms before the push, `runtime/` being the C boundary.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): a crash of a correct program in a setting Linux allows.

    **Closed 2026-10-03** after the push of `e339ece9`. Repaired at `ffaf9f4d` in lane depth, beside defect 169, it entered the trunk at the round of 2026-10-03's sixth gate, `da3e29af` (lanes warn, ffimsg, depth, twin156 and win214): the seed regenerated once, 39,690,755 bytes, SHA-256 beginning `2c809845ed0f7ba8`, its fixpoint by `cmp`; the compiler's own tests 1,099 and the net's own 200, all passed; the full net, 26 suites, 5,033 passed and 0 failed; the censuses of `check --brief` over 1,905 files and of `--emit-c` over 595, every move attributed to its lane. Its cases read one by one at `da3e29af`, whose compiler `e339ece9` carries unchanged (no file under `selfhost/`, `seed/`, `runtime/` or `tests/` moved between them): on this Mac (Apple clang 21), in the Linux arm64 container under Debian clang 22.1.8 and again under 18.1.8, and on the Windows box (clang 23.1.1): in the Linux arm64 container under clang 22.1.8 and again under 18.1.8, `examples/threads` built by the round's compiler ran at exit 0 under `ulimit -s unlimited`, its output printed, and matched `main.expected` at the default limit; the compiler itself, under the unlimited limit, checked `docs/panel/184-briefs/blind/task3a.hero` at exit 0. A Linux fact read on Linux; unrun on this Mac and the Windows box. The push's legs: Linux arm64, its suites four at a time, the compiler's own tests 1,099 all passed and 20 suites at 0 failed under each clang; the Windows box, the compiler's own tests 1,099 all passed and nine of its 20 suites at 0 failed before it went offline at about 14:47 (Tailscale, read at 15:41: last seen 54 minutes before), every case above already read there; and the CI's run 37124159403 on `e339ece9`: Linux x86-64 (Ubuntu clang 18.1.3), the compiler's own tests 1,099 all passed and 26 suites, 4,993 passed and 0 failed; Windows x86-64 (clang 20.1.8), 1,099 and 26 suites, 4,932 passed and 0 failed, the box's eleven unrun suites among them; Linux arm64 and Darwin arm64 green. Which golden cases those jobs ran their logs do not say, so the readings above are the measurement of them, and x86-64's an inference from Linux arm64 under the same clang major. It closes after the push's platform legs ran its cases (`.claude/rules/verification.md` § The batch).
