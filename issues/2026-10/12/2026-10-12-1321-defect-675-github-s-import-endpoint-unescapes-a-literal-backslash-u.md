---
kind: defect
area: harness
milestone: M-issue-files
filed: 2026-10-12
commit: self
github: https://github.com/heroes-lang/heroes/issues/2024
---

- [x] **675 — GitHub's import endpoint unescapes a literal backslash-u sequence of a control character a second time** | issues 1354 and 1643 hold `\^A` and `\^[` where their cards say `\u0001` and `\u009b`: the payload sent the backslash escaped, as JSON asks, and the import stored the text as though the escape were read twice and the control character then written in caret notation; `\u007f` in the second card and `\u200b` in a third (issue 1709) stayed as written, and the bodies of the other 2,018 issues match their cards beyond the `Record:` pin | `tests/harness/issue_push.hero`, which reads no body back after an import · `issues/2026-09/26/2026-09-26-0114-defect-102-closed-every-json-goes-through-one-escape.md` · `issues/2026-10/04/2026-10-04-1734-defect-290-lex-dump-tokens-and-parse-dump-ast-write-a-string-s-control.md` · **class: blocking**

    **Origin:** found 2026-10-12 by the coordinator, reading the report of `plan` run after the transcription: 2 of the 2,020 issues differed in body beyond the pin; the body GitHub holds, read back, against the payload sent, the first differing bytes `\u0001` sent and `\^A` held (issue 1354), `\u009b` sent and `\^[` held (issue 1643), three bytes shorter each.

    **Class: blocking**, 2026-10-12: two public issues say what their files do not, a false text in the record a reader is shown.

    **What a repair owes**: the two bodies made their cards' again by `repair --yes` (the author's answer of 2026-10-12) and read back; if the REST route stores them otherwise too, that is measured and said here, and another spelling of the same text is found; and every import of a later pass read back, so a body the endpoint changes is told the day it is made.

    **2026-10-12, the REST route measured**: `repair --yes` on the two cards sent each a `PATCH` of its body, answered 200, and read both back still holding `\^A` and `\^[`, the HTML GitHub serves its readers showing `<code>\^A</code>`; so GitHub stores such a sequence so on every route, and the import is not the whole of the cause. The oracle, `POST /markdown`, which writes nothing, shows `<code>&#92;u0001</code>` displayed as the code span `\u0001` is, and the rendering rule now spells a C0 or C1 control escape so (`.claude/rules/records.md` § The issues); whether GitHub stores that spelling as sent is the next `repair`'s read-back.

## The repair

The rendering rule spells a literal backslash and `u` before the four digits of a C0 or C1 control character with its backslash as `&#92;`, and a code span holding one as an HTML `<code>` element, every ASCII punctuation character of its text written as a character reference so that nothing inside reads as Markdown (`.claude/rules/records.md` § The issues; `tests/harness/issue_controls.hero`, four cases); the oracle shows the spelling displayed as the code span is. `records/rendered` refuses such a sequence still written bare in a body, a fenced block's included, which no card holds today (counted over 2,023 files: three hold one, all in single code spans). Rendered over the whole tree before and after, the bodies of the three cards holding one changed and no other byte of any card. And `push` reads back every issue it imports and tells one whose body GitHub holds otherwise, so the next such change is seen the day it is made.

Then `repair --yes` on the two cards, the second time: both bodies rewritten by a `PATCH` and read back equal to their cards, `repair: 2 issues compared, 2 repaired, 0 still differing or unread`; issues 1354 and 1643 show `\u0001` and `\u009b` again.

**Closed 2026-10-12** in the commit that writes this section.

