# Defect 174 closed: a statement after `return`, `break` or `continue` in the same block compiles, where panel 184's R4, ratified, makes it a compile error

- [x] **174 — a statement after `return`, `break` or `continue` in the same block compiles, where panel 184's R4, ratified, makes it a compile error** | `function f() -> i64` over `return 1` and then `print(2)`, `main` printing `f()`: `run` exit 0, prints `1` | the checker's walk of a block · panel 184's R4 (`docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md:181-182`; *the spec states the return rule for the first time*, `:186-190`; *R4 lands as it stands*, `:276-277`) · spec § 8 `:229-230`, its only sentence on a jump · **class: blocking** · **closed 2026-10-03**

    **Origin:** panel 184, ratified 2026-10-01 at 22:24; its landing found unwritten by the coordinator's file-queue agent, 2026-10-02 (`scratchpad/file-queue/unlanded-184/after_return.hero`, 2026-10-02), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **2026-10-03, lane flow4, panel 184's R4 whole with panel 185's R5**:
    repaired at `2a03b5e6` (`selfhost/check/path_end.hero`, called by
    `check/walk.hero` and `check/decls.hero`; the seal in `ir/flatten.hero`,
    `ir/lower.hero`, `ir/emissions.hero`), gated by its own cases, two
    `fixedbugs-174-*` goldens under `check/` and two under `run/`, and the
    two defect-139 goldens moved by hand; spec § 8's sentence landed in the
    lane's spec commit; the rest is owed at the round's gate.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): by the ratified language, a wrong program accepted.

    **Closed 2026-10-03** at the round of 2026-10-03's gate, `b43d224d` (lanes fbrace, flow4, rec187 and cb4), on this Mac: the seed regenerated once, 39,266,477 bytes, SHA-256 beginning `bdf53918a7df9c36`, its fixpoint by `cmp`; the compiler's own tests 1,079 and the net's own 200, all passed; the full net, 26 suites, 4,901 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,876 files and of `--emit-c` over 576, every move attributed to its lane; the site's build green. The trunk took the gate's tree by fast-forward. Its repair, lane flow4's `2a03b5e6`, panel 184's R4 with panel 185's R5; its own cases re-run on the trunk at `b43d224d` between 07:26 and 07:36 by `date`: `check` 2 of 2, `annotations` 2, `fixes` 2, `run` 2 of 2, all passed. Not a C-boundary defect, so it closes at the Mac's gate (`.claude/rules/verification.md` § The batch); Linux arm64, the Windows box and Linux under clang 18 run before the push.
