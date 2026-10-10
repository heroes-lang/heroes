---
kind: defect
area: emit
milestone: none
filed: 2026-10-10
commit: ce51ad26442c7dffeaf9b1546f2e71e3a59dbe66
github: none
---

- [ ] **585 — a name hidden by the program's own stricter switch is told *declares no* without naming the switch** | under a header of the program's own defining `_POSIX_C_SOURCE 200112L` before `string.h`, binding `strlcpy` is refused *`string.h` declares no `strlcpy`*, true, where the message could name the switch that hid it (defect 568's message names `_GNU_SOURCE` when a name is declared only under a feature-test macro, the converse shape) | the hidden-name message of defect 568's repair, `selfhost/emit/` · **class: improvement**

    **Origin:** found by lane b18-guard beside panel 205's landing (its final reply and notes, `.claude/worktrees/scratch-b15/b18-guard/notes.txt`, ignored by git), filed by the coordinator at 13:56 on 2026-10-10.

    **Class: improvement**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `ce51ad26442c7dffeaf9b1546f2e71e3a59dbe66`, 2026-10-10 (lane b19-pack), gated by its cases and the compiler's own tests; the net is owed at the batch's close. After defect 568's switches a refused round asks two questions more (`selfhost/cli/unswitch_ask.hero`): which strict switch (`_POSIX_C_SOURCE`, `_POSIX_SOURCE`, `_XOPEN_SOURCE`, `_ANSI_SOURCE`, `_ISOC99_SOURCE`, `_ISOC11_SOURCE`) stands after each of the unit's headers, one that stands before the first being no header's; and, for each switch a header defined, the headers again with that switch undefined after every header, each name asked as 568's are. A name declared there is told *`string.h` declares no `strlcpy` under `_POSIX_C_SOURCE`, which `cfg.h` defines: without that switch it declares it*, every header after which the switch stands again named, with a note on where the switch stands and the two ways out (`selfhost/emit/ffi_switched.hero`, whose `Switch` record now carries both answers); a name no switch hides keeps *declares no*. Measured on this Mac: lane b18-guard's `sw` program, `cfg.h` defining `_POSIX_C_SOURCE 200112L` before `string.h`, is told so. One run of clang more and one per switch a header defines, only in a round already refused and only where a name is left unexplained. Cases `unsupported/fixedbugs-585-*` (3): the switch and its header, two headers that define it both named, and a name no switch hides still told *declares no*; each annotated `#~ ffi_unknown_name`.
