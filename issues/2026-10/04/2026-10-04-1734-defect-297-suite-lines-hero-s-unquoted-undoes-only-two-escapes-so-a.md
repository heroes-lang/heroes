---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: f6c9d74d589c9795c264eb3f8e483c8830f2edba
github: none
---

- [x] **297 — `suite_lines.hero`'s `unquoted` undoes only two escapes, so a `#line` name holding a line end would be misread** | `tests/harness/suite_lines.hero:242` reads a `#line` name back undoing `\\` and `\"` alone; since defect 240's repair the emitter also writes `\n`, `\r` and `\?` in such a name, and before it this reader misread every name above ASCII; no tracked path holds those bytes today (lane b9-emit, 2026-10-04, a reading) | `tests/harness/suite_lines.hero:242` · defect 240's spelling, `selfhost/emit/c_text.hero` · **class: improvement**

    **Origin:** lane b9-emit, 2026-10-04 (its reply's *found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument that would misread a name no tracked path holds; no program moves.

    Repaired at `f6c9d74d`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.

## The repair

Repaired at `f6c9d74d`. `suite_lines` undid three of the five escapes the emitter writes in a `#line` name and read `\n` and `\r` as letters, and a claim on a file the walk could not read was passed over in silence, which is how the misreading hid. The five are undone, any other escape and a backslash ending the name are an offence naming the name, and a claim on a file the walk cannot read is an offence once per run of claims on it. Its case is in `suite_lines.hero`: each escape, the ones not written, a trailing backslash, and a claim on a file that is not there.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
