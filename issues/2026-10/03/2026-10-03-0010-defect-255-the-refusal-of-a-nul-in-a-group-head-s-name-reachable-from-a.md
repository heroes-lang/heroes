---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: 5215222f6a5e8af628d445af3729681b6d41543d
github: none
---

- [x] **255 — the refusal of a NUL in a group head's name, reachable from a program, has no golden case** | `extern "a<NUL>b.h"`, a raw NUL in the string: `check` exit 1, `unwritable_name` *this header's name holds a NUL byte, and C reads a name only up to its first NUL* (batch 8's round compiler at `1eb854c3`, 2026-10-04, `<scratchpad>/filings-b8/probe/nul.hero`); `grep -rl 'NUL byte' tests/golden` finds none, the rule's witnesses being unit tests in `selfhost/head_names.hero` and `selfhost/cli/units.hero` | `tests/golden/check/` (a `fixedbugs-216-` case holding the raw byte, annotated) · `selfhost/head_names.hero:121` to `:144` · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 8); the shape run by the coordinator, 2026-10-04.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a witness owed (`.claude/rules/diagnostics-and-goldens.md`: every diagnostic annotated in the source that provokes it); no program moves.

    Repaired at `5215222f`, 2026-10-05, gated by its case and the compiler's own tests; the net is owed at the batch's close. Its case's name is `fixedbugs-255-`, where the item said `fixedbugs-216-`, the number of the head's own rule.

## The repair

Repaired at `5215222f`. `unwritable_name`'s refusal of a NUL in a group head's name, reachable from a program, had unit tests alone; a `check` case now holds a raw NUL in each of the head's three strings, the header's, a library's and a package's, each told once, the head's. Found beside it and repaired with it: `\u{0}` in a head's string was told `unknown_escape` (a link) or `escape_in_header_name` (a header), the decoder leaving a NUL by code undecoded; `escape.encoded(0)` is now the NUL's one byte, so the head refuses the NUL the escape writes, in one message. Its case is `check/fixedbugs-255-a-nul-in-a-group-head-s-name-is-the-head-s-to-tell`, its three raw lines green on the base, the witness owed, and its two `\u{0}` lines red; the item had named it `fixedbugs-216-`, the number of the head's own rule.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
