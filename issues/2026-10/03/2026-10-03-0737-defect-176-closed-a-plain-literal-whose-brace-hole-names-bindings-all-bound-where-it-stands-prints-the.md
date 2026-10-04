---
kind: defect
area: records
milestone: none
filed: 2026-10-02
commit: fc8687de2b2516fe8180d996faf83b2ad0c88910
github: none
---

# Defect 176 closed: a plain literal whose brace hole names bindings all bound where it stands prints the braces, where panel 185's R7, decided by the author, refuses it with the `f` as its fix

- [x] **176 — a plain literal whose brace hole names bindings all bound where it stands prints the braces, where panel 185's R7, decided by the author, refuses it with the `f` as its fix** | `x = 1` over `print("{x}")` and `print(x)`: `run` exit 0, prints `{x}` and `1`; under R7 `check` refuses the literal | spec `:52` (*A literal without the `f` is unchanged*), the opposite of the landing · panel 185's R7 (`docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md:201-203`; decided 2026-10-02, `:264-268`, *route (5b) lands after panel 184's R1*), so after defect 173 · **class: blocking** · **closed 2026-10-03**

    **Origin:** panel 185's R7, decided by the author 2026-10-02 at 07:05 (`docs/records/done/2026-10-02-0705-panel-185-r7-decided-the-forgotten-f-refused-where-the-braces-hold-names-all-bound.md`); its landing found unwritten by the coordinator's file-queue agent, 2026-10-02 (`scratchpad/file-queue/unlanded-184/forgot_f.hero`, 2026-10-02), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **2026-10-03, lane fbrace, route (5b) after R1**: repaired at `a5fc53de`, its sentence in spec § 2 at `8fc6e206`, gated by its own cases; the rest is owed at the round's gate.

    **2026-10-03, the round's gate, lane round1003a**: the site's figures this item's `a5fc53de` moved, with defect 173's `f6fdda79`, are re-cut at `b025a473`. Owed before the push: the strings chapter's *A string without the `f` is exactly what it was*, in both editions.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): by the decided language, a wrong program accepted.

    **Closed 2026-10-03** at the round of 2026-10-03's gate, `b43d224d` (lanes fbrace, flow4, rec187 and cb4), on this Mac: the seed regenerated once, 39,266,477 bytes, SHA-256 beginning `bdf53918a7df9c36`, its fixpoint by `cmp`; the compiler's own tests 1,079 and the net's own 200, all passed; the full net, 26 suites, 4,901 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,876 files and of `--emit-c` over 576, every move attributed to its lane; the site's build green. The trunk took the gate's tree by fast-forward. Its repair, lane fbrace's `a5fc53de`, panel 185's R7; its own cases re-run on the trunk at `b43d224d` between 07:26 and 07:36 by `date`: `check` 1 of 1, `annotations` 1, `fixes` 1, all passed. Not a C-boundary defect, so it closes at the Mac's gate (`.claude/rules/verification.md` § The batch); Linux arm64, the Windows box and Linux under clang 18 run before the push.
