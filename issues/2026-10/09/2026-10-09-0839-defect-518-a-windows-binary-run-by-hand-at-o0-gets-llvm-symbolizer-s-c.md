---
kind: defect
area: cli
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **518 — a Windows binary run by hand at `-O0` gets llvm-symbolizer's `.c` line in a sanitiser's report** | defect 509's repair sets `external_symbolizer_path=` for `run` and `test`; a binary run by hand keeps llvm-symbolizer, which names the last file block's line (`.c:122`) for every address of a function whose line table spans the `.hero` and the `.c`, and clang 23's dynamic ASan runtime ignores `__asan_default_options` (lane b15-runtime, on the box) | `selfhost/cli/symbolized.hero`, the emitted program's start · defect 509 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-runtime's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true report naming a less exact line.

    Measured 2026-10-09 on the box (lane b16-tools, clang 23.1.1, lld-link; not a repair), the 322 case's emitted C built as the compiler builds it at `--sanitize -O0` and run by hand, llvm-symbolizer on the PATH: frame #1 names `.c:122` in all three stacks, and `.hero:21` with `ASAN_OPTIONS=external_symbolizer_path=`. **`__asan_default_options` is read, but not for the symbolizer**: defined in the program (exported or not) as `external_symbolizer_path=:print_summary=0:verbosity=1`, the report lost its SUMMARY line and still named `.c:122`, and nothing was printed at start; so it is applied after the symbolizer is chosen (an inference from those three readings), and the runtime lane's *not honored* was half the shape. **What a binary can carry instead**: DWARF beside CodeView (`-g -gcodeview -gdwarf-4` on the unit, linked `-g`): llvm-symbolizer reads the DWARF and names `.hero:21`, DbgHelp reads the PDB and names `.hero:21`; lld-link then warns of every debug section's long name, and with `-Wl,/DEBUG:DWARF` beside it does not, the binary 1,110,016 bytes to 1,960,448 (the PDB 7,413,760 both). Its price, measured: a frame in code without DWARF (the C runtime's start, ASan's malloc and free thunk) loses its line under llvm-symbolizer, and its name too without `/DEBUG:DWARF`. Unrun: the compile and link cost at three sizes, `-O2` under line tables, the runtime's own frames compiled with DWARF, beside 509's `/PDBALTPATH`, and the CI's clang 20; the box's C: held 8.03 GB free after these six binaries, at the 8 GB floor, so they wait on space only the author can free.
