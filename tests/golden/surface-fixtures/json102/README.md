# Defect 102: JSON that is JSON, and a character the reader can find

- `tab.hero` is a correct program with a real tab inside a token's text:
  `lex --dump-tokens --json` writes it as `\t`, where it wrote the byte raw
  and no JSON reader accepted the output. The tab stood in a string literal
  until panel 192 refused a raw tab there (`raw_tab`, 2026-10-05, its one
  spelling being `\t`); it stands in a comment since, where a tab is legal.
- `control.hero` holds a stray 0x01 byte: both JSON writers write it as
  `\u0001`, and the diagnostic names it *the control character U+0001*, where
  it printed the byte itself between two backquotes.
- `unseen.hero` holds a right-to-left override (U+202E), a zero-width space
  (U+200B) and a euro sign (U+20AC): the first two are named by their code
  alone, since a terminal shows nothing for one and reorders the rest of the
  line for the other; the third is shown and named.

These files hold the bytes on purpose, so no editor should normalise them:
`tests/harness/suite_surface.hero`'s rows over them are what fails if one does.
