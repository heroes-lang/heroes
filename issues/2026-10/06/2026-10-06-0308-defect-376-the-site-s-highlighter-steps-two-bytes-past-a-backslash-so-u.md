---
kind: defect
area: site
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **376 — the site's highlighter steps two bytes past a backslash, so `\u{1b}` in an `f` literal opens a hole at its `{`** | `site/src/lib/highlight.ts:89` advances `end += source[end] === '\\' ? 2 : 1`, so the escape panel 192's R5 adds, `\u{1b}`, is read as `\u` and then `{1b}`, which inside an `f` literal the highlighter reads as a hole: a correct program is shown wrong on the site (read by the coordinator at 03:08 on 2026-10-06; found by lane b12-str192, its site build unrun) | `site/src/lib/highlight.ts:89`, `:108` · `.claude/rules/diagnostics-and-goldens.md` § A new surface form lands in every tool that reads the language · **class: adjacent**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a correct program coloured wrong on a published page, a tool that re-prints the language not taught the new form.
