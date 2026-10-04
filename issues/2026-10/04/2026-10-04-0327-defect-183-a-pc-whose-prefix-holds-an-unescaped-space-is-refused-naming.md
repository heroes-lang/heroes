---
kind: defect
area: cli
milestone: none
filed: 2026-10-02
commit: e2f98e174ad84adc0ecd66b1f75569f8001bdc43
github: none
---

- [x] **183 — a `.pc` whose `prefix` holds an unescaped space is refused naming `space/include` as the flag, and not the package file's line that splits it** | `prefix=/opt/with space`, `Cflags: -I${prefix}/include`: `pkg-config --cflags s7` prints `-I/opt/with space/include`; `build` exit 1, `ffi_package`, *the package `s7` answered with `space/include`, which this compiler does not pass on* | `selfhost/cli/shell_split.hero` (the word splitter, defect 162's `b37bfce1`) before `filter_words` (`selfhost/cli/libraries.hero:83`) · **class: adjacent**

    **Origin:** lane h158 beside defect 162, 2026-10-02 (`scratchpad/lane-h158/d162/pc/s7.pc`, 2026-10-02, *true, could say more*); reproduced by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/pc-prefix-space/`, run with `PKG_CONFIG_PATH` naming that folder) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`), where only the note's list of accepted flags is worded otherwise.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    **2026-10-03, batch 8's FFI lane**: repaired at `e2f98e17`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The refusal of a word that is no flag names the line `pkg-config` printed and the word before it; the package file's own line is not read, since finding it would rest on each `pkg-config`'s `.pc` syntax.

## The repair

Repaired at `e2f98e17`. A word of a package's answer that is no flag is refused with the line `pkg-config` printed and the word before it, and a note that a path holding a space is one word only where `pkg-config` prints it escaped or quoted. Accepted as repaired by the author's *3a* of 2026-10-04 (`docs/records/log/2026-10-04-0150-the-author-answers-1a-2a-3a-4a-5a.md`): naming the `.pc` file's own line would rest on each `pkg-config`'s syntax. Its witnesses are the unit tests of `shell_split.no_flag` and of `filter_words`' whole message.

**Closed 2026-10-04** with batch 8 (lanes b8-ffi, b8-source, b8-recovery, b8-emit and b8-defects, merged into one round tree), its closing gate run on `921dc61e` with the seed regenerated: 40,628,892 bytes, SHA-256 beginning `2d55c5ff8309b812`, its fixpoint by `cmp`; the compiler's own tests 1,158, all passed; the net's own tests 210, all passed; the full net, 26 suites, 5,177 passed and 0 failed. The census at the batch's first gate, the trunk's compiler at `7d9f2e8f` against the round's over the tree's tracked files: `check --brief` over 1,954, 30 moved, and `build --emit-c` over the 1,228 holding an `extern`, 44 moved, every one the batch's own (its new refusals, its words, its `#line` before a fixed array field's assertion). Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 12 fewer messages and none more; 15,842 pairs, one told second now carried in the first message's fixes (defect 271).
