---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: 4de7cc2afc2cc759de82f0d99b6bdb3e20377e66
github: none
---

- [x] **241 — a `pkg-config` answer that is not UTF-8 is taken as no answer, so the package's flags are dropped and the build succeeds without them** | `extern "ab.h" package "u"` over `function seven() -> i32`, `u.pc` answering `Cflags: -DX=caf<e9> -I/nowhere` (`pkg-config --cflags u` prints those bytes): `build` exit 0, *wrote*, and neither `-DX` nor `-I/nowhere` reaches clang; the program prints 7 (batch 8's round compiler at `1eb854c3`, this Mac, Homebrew's `pkg-config`, 2026-10-04, `<scratchpad>/batch8/ffi/repro/r237/badutf.hero` with `pcbad/`) | `selfhost/cli/libraries.hero:286` (`resolve_packages`, the answer's read failing as not text and taken as no words) · panel 189's Q6 · **class: blocking**

    **Origin:** batch 8's FFI lane beside 237, 2026-10-04 (its report's addendum), handed to panel 189 as a shape of its Q6; reproduced by the coordinator on the round's compiler the same night.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a wrong build accepted at exit 0: the flags a package names dropped in silence.

    Repaired at `4de7cc2a`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `4de7cc2a`. A `pkg-config` answer that is not UTF-8 stops the build, named: the package's `ffi_package` at exit 1, its bytes quoted, each byte that is not UTF-8 written by its value; a complaint that is not UTF-8 is told with its bytes where it was told as nothing. Its cases are `cli/package_answer.hero`'s test over two committed answers under `surface-fixtures/notext241`.

**Closed 2026-10-04** with batch 9 (lanes b9-notext, b9-emit, b9-harness, b9-recovery and b9-annot, merged into one round tree with the trunk at `f6a3122e`), its closing gate run on the round's head from `2c58b28e` to `662870e6`, no line of `selfhost/`, `runtime/` or the seed moving between, with the seed regenerated: 41,364,146 bytes, SHA-256 beginning `26ccaa9d96478a20`, its fixpoint by `cmp`; the compiler's own tests 1,190, all passed; the net's own tests 246, all passed; the full net, 27 suites, 5,268 passed and 0 failed, `fixes` read alone after `662870e6`, which stopped that suite copying the byte fixtures of defects 227 and 241 as text. The census, the trunk's compiler at `703af779` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 1,993, 34 moved, and `build --emit-c` over the 621 holding an `extern`, 3 files of C and 22 of messages moved, every one the batch's own. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, 68 fewer messages in the normal arm and 71 in the control arm and none more; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked.
