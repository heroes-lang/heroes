---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: ba8e6f67f3d30d31034a922d5e5ead07a16d6555
github: none
---

- [x] **285 — a `pkg-config` that answers in CRLF lines has each line's carriage return shown as `<U+000D>` in the package's notes** | a stand-in `pkg-config` answering *Package zz9 not found* in CRLF lines: the package's message carries `note: Package zz9 not found<U+000D>`, the answer trimmed once before its lines are split (lane b9-harness, measured on this Mac, 2026-10-04, `<scratchpad>/batch9/harness/`) | `selfhost/cli/libraries.hero:318` (`strings.trimmed` of the whole answer, then `shell_split.shown` by line) · **class: adjacent**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*, measured), in the notext lane's file, so reported rather than repaired; read at `703af779` by the coordinator.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be, a line end shown as a code.

    Repaired at `ba8e6f67`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `ba8e6f67`. A `pkg-config` writing `\r\n`, as one built for Windows does, had its complaint split with the return left on the last note, and the same return on the answer's last flag: `-lz\r` sent the linker after `z\r`, exit 2, and `-DSEVEN=7\r\n` warned of an embedded newline in each of three compiles of a correct program. `package_answer` now reads `\r\n` as `\n` where both files enter, as `run_clang` reads clang's; a lone return stays one, written by its code. Measured by hand with stand-in `pkg-config`s on the PATH; the Windows box has no `pkg-config` to run it on. Its case is a compiler test over both files written so.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
