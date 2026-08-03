# Panel 000 — adversarial review of the bootstrap plan (revision 1 → revision 2)

Date: 2026-08-03. Five independent reviewers, differentiated by lens, each
instructed to find the strongest reason the plan was wrong.

| Reviewer | Verdict | Single most important change demanded |
|---|---|---|
| [Language design](language-design.md) | **object** | Replace implicit `T`→`T?` promotion with explicit `ok(x)`; drop Principle 0's destructive second question |
| [Compiler engineering](compiler-engineering.md) | approve-with-changes | Add the ownership + type-descriptor passes between lowering and emission; split M5b around them |
| [Simplicity](simplicity.md) | **object** | Cut ceremony; first milestone must end with tokens on screen, not a directory tree |
| [LLM ergonomics](llm-ergonomics.md) | **object** | Land spec + harness in M0 with a `--permissive` control arm; grade on tests passing; take the pre-amendment baseline |
| [Didactics](didactics.md) | approve-with-changes | Make the author's prediction uncued and falsifiable (separate file, committed first) |

## Disposition

All five verdicts were folded into **revision 2** of the plan (approved by the
author 2026-08-03), with two rejections recorded:

- **Cutting the mandatory panel** (simplicity reviewer): REJECTED — author's
  explicit decision. Adopted instead: the LLM-ergonomics redesign
  (differentiated inputs, falsifiable predictions, path-based triggers) and a
  large ceremony reduction (2 skills, 2 golden dirs, no ROADMAP, 5-section
  journal).
- **Cutting the LSP** (simplicity + didactics reviewers): REJECTED — author's
  explicit decision. Moved off the critical path; blocks nothing.

The plan itself (revision 2) is archived with the repo's planning records; the
authoritative copy that was approved lives in the Claude Code plans directory
and its content is reflected in CLAUDE.md and this docs tree.
