---
kind: task
area: cli
milestone: M-inferred-cell
filed: 2026-10-10
commit: none
github: none
---

- [ ] **M-inferred-cell** | `heroes check <root> --apply --in-place` applies the certain fixes of the root module only and prints *N certain fix(es) are in another module and were not applied*, and a nested module cannot be checked alone (its `use` lines resolve from its own directory), so the migrator the sitting adopted for the tree's rewrite, `check --apply` over the tree, does not exist as one command: step 1 applied its 391 `@` to `=` fixes from `check --json`'s byte spans (`file`, `byte_start`, `byte_end`, `replacement`, `text`) with a one-off in the lane's scratch, which is not a project instrument (CLAUDE.md § 10). The landing's second commit, 1671 files from `v: T @ e` to `v: T @= e`, needs the instrument: `--apply --in-place` writing every module of the program whose fixes are certain, or a word that names them (`--all-modules`, a flag since it changes how one question is answered about the same input, `.claude/rules/cli-surface.md`), and a decision whether an invocation may write files it did not name | `selfhost/cli/check.hero:108-121` and `selfhost/cli/apply_answer.hero` (`hand_back`, the note about another module); `selfhost/cli/served.hero`; the sitting's § The landing; the lane's one-off, `.claude/worktrees/scratch-b15/lane-inferred-cell/apply_json_fixes.py` (ignored by git)

    **Origin:** found by the landing lane at 19:13 on 2026-10-10, running `./heroes check selfhost/main.hero --apply` and reading its note; the engineer's report had priced the route and marked the pass over the tree unrun, and the critic's second pass listed it among the claims asserted and not measured.
