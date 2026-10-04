- [ ] **316 — a `HEROES_COMPILER` that is UTF-8 and names nothing is passed over in silence for `./heroes`, so the net judges a compiler nobody named** | `HEROES_COMPILER=/nonexistent/heroes` and no compiler named after `--`: the harness's probe, built from lane b9-annot's committed code, printed `compiler ok ./heroes`; since defect 243's harness half (`c539f58d`) a value that is not UTF-8 is refused, and one that names nothing is not | `tests/harness/shell.hero` (`some_compiler`, its candidates after `HEROES_COMPILER`) · defect 276, the runtime's twin of this cause · **class: adjacent**

    **Origin:** lane b9-annot, 2026-10-04, each reproduced on its worktree's harness (its final reply's *Found beside*; scratch `<scratchpad>/batch9/annot/`).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a hint that names nothing passed over in silence; the lane's reading, as for 276.

    Repaired at `31aaf512`, 2026-10-04, gated by its cases and the net's own tests; the net is owed at the batch's close.
