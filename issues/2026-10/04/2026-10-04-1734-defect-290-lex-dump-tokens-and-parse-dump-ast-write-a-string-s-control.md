---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 28cef56dba0b4e354a723301b7efbe9016241649
github: none
---

- [x] **290 — `lex --dump-tokens` and `parse --dump-ast` write a string's control characters raw, so a file's escape sequences reach the terminal** | a string literal holding ESC and `[2Jboom`: the two dumps write the bytes to stdout as they are, a terminal's clear-screen among them; their `--json` forms escape it (lane b9-notext's compiler, 2026-10-04) | `selfhost/cli/lex.hero`, `selfhost/print/dump.hero` · defect 244, the same bytes through a diagnostic's excerpt, repaired in batch 9 · **class: blocking**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 1).

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): 244's reason, what a terminal shows is no longer what the compiler wrote; the lane left the class to the coordinator, a dump being an artifact as well as a message.

    Repaired at `28cef56d`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    Repaired at `2ef4d589`, 2026-10-04, lane b10-ir, its `build --dump-ir` half (`selfhost/ir/print.hero`, the same cause: the string table and a test's title written raw), gated by its case and the compiler's own tests; lane b10-cli's line names the dumps' half; the net is owed at the batch's close.

## The repair

Repaired at `28cef56d` for the token and syntax-tree dumps and at `2ef4d589` for `build --dump-ir`. Each wrote a string literal's ESC to the terminal as it was, so `[2J` cleared the reader's screen; each now writes every control character but a line end and a tab as a diagnostic does since defect 244, `<U+001B>`, and the JSON form writes DEL and C1's CSI as `\u007f` and `\u009b`. `print/guard` still compares the raw syntax tree, since the shown form is not injective. Measured base against repair over ESC, BEL, DEL, a C1 CSI, CR and NUL in a string, a comment, a char literal, an f-string, a constant and a test title: one control byte each before, none after; a tab and a plain string byte for byte. Its cases are compiler tests: `lex`, `dump`, `json_text` and `ir/print`.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
