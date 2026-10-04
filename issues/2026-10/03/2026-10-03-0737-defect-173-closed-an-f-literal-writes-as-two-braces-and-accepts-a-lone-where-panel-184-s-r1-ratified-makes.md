---
kind: defect
area: compiler
milestone: none
filed: 2026-10-01
commit: fc8687de2b2516fe8180d996faf83b2ad0c88910
github: none
---

# Defect 173 closed: an `f` literal writes `}}` as two braces and accepts a lone `}`, where panel 184's R1, ratified, makes `}}` one brace and a lone `}` an error

- [x] **173 — an `f` literal writes `}}` as two braces and accepts a lone `}`, where panel 184's R1, ratified, makes `}}` one brace and a lone `}` an error** | `print(f"{{a}}")` prints `{a}}` and `print(f"a}b")` prints `a}b`, both at exit 0; under R1 the first prints `{a}` and the second is refused | `selfhost/lex_interp.hero` · panel 184's R1 (`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md:149-150`, ratified at `:248`, *R1 and R2 land as ratified* at `:284`), R8 putting its sentence in spec § 2 (`:223-224`) · spec `:49-52` (*`{{` writes one brace*, no `}}`, no lone `}`) · **class: blocking** · **closed 2026-10-03**

    **Origin:** panel 184, ratified 2026-10-01 at 22:24 (`docs/records/done/2026-10-01-2224-panel-184-ratified-a-brace-both-ways-a-statement-after-a-jump-refused-and-a-floor-for-depth.md`); its landing found unwritten by the coordinator's file-queue agent, 2026-10-02 (`scratchpad/file-queue/unlanded-184/`, 2026-10-02), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). No list held the landing: the sitting's item was ticked at its ratification. The spec does not say it either; the landing writes both.

    **2026-10-02, lane fbrace, R1 in its two stages**: repaired at `f6fdda79`, the seed regenerated between the stages at `11b40220` with its fixpoint verified by `cmp`, and R8's sentence in spec § 2 at `8fc6e206` (2026-10-03), gated by its own cases; the rest is owed at the round's gate.

    **2026-10-03, the round's gate, lane round1003a**: two of the lane's misses redone in the round's tree, gated there: the probe's pinned count of the formatter's fixture directories, which `f6fdda79` grew to 12 and left its test at 11 (`5f022fa2`), and the site's four figures cut from `examples/gallery/12-interpolation.hero`, whose lines `f6fdda79` and `a5fc53de` moved by three (`b025a473`). Owed before the push: the strings chapter's paragraph on braces, in both editions, still states the language before R1 and R7.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): by the ratified language, a wrong value (`{a}}`) and a wrong program accepted (`a}b`).

    **Closed 2026-10-03** at the round of 2026-10-03's gate, `b43d224d` (lanes fbrace, flow4, rec187 and cb4), on this Mac: the seed regenerated once, 39,266,477 bytes, SHA-256 beginning `bdf53918a7df9c36`, its fixpoint by `cmp`; the compiler's own tests 1,079 and the net's own 200, all passed; the full net, 26 suites, 4,901 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,876 files and of `--emit-c` over 576, every move attributed to its lane; the site's build green. The trunk took the gate's tree by fast-forward. Its repair, lane fbrace's `f6fdda79`, panel 184's R1 in its two stages, the seed regenerated between them at `11b40220`; its own cases re-run on the trunk at `b43d224d` between 07:26 and 07:36 by `date`: `check` 1 of 1, `annotations` 1, `fixes` 1, `run` 2 of 2 (the regex binding among them), all passed. Not a C-boundary defect, so it closes at the Mac's gate (`.claude/rules/verification.md` § The batch); Linux arm64, the Windows box and Linux under clang 18 run before the push.
