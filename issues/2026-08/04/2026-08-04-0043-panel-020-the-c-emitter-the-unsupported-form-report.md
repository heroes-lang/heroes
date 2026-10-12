---
kind: decision
area: process
milestone: none
filed: 2026-08-04
commit: ac8c34cc7d48cd0faf5eacd59c10fc67ce327606
github: https://github.com/heroes-lang/heroes/issues/78
---

2026-08-04 | **Panel 020 — the C emitter.** The unsupported-form report is a `Diagnostic` **kind** (`unsupported`), not a parallel type, and it exits **1**: GCC's `sorry()` has been a kind in `kinds.def` beside `error` since GCC 2.5.8 and exits `FATAL_EXIT_CODE` = 1, GHC's `Sorry` carries this proposal's own reasoning in a comment and travels the ordinary failure path, and CLAUDE.md §9's `#~ <code>` invariant keys off diagnostic codes — a type outside the diagnostic system would be a test class outside the invariant. The message names the **capability**, never the milestone (§4.17: nothing that resolves only in another file). A holed `build` exits 1 ending in `no binary: N hole(s)` — the ergonomist measured exit 0 producing a `build && ./artifact` loop that runs yesterday's binary | ergonomist veto on exit 0 and on `(M5b)`; historian's two independent precedents; the exit code read to the end lands on 1 from GCC's own source | §3.1, §4.16, §4.17 | 020 |

## Prior art

Panel 020 (`docs/panel/020-the-c-emitter.md`): each line says what the cited page itself states, the sitting's own file says what was taken from it, and every link was re-read 2026-10-11.

- **Taken from GCC**: [GCC diagnostic kinds (diagnostic.def, DK_SORRY)](https://github.com/gcc-mirror/gcc/blob/releases/gcc-13/gcc/diagnostic.def): DK_SORRY, 'sorry, unimplemented', is a diagnostic kind of its own beside DK_ERROR and DK_WARNING
- **Taken from GHC**: [GHC.Utils.Panic, GhcException Sorry](https://raw.githubusercontent.com/ghc/ghc/master/compiler/GHC/Utils/Panic.hs): Sorry is a GhcException constructor beside Panic: something known not to work yet, which is not counted as a bug
- **Reached first by GCC**: [GCC Installation, Building (bootstrap comparison)](https://gcc.gnu.org/install/build.html): if the comparison of the stage2 and stage3 compilers fails, the stage2 compiler has normally compiled GCC incorrectly
