- [ ] **M-interpolated-strings, golden ratification** | Five cases were written and marked nothing; read `interpolation-holes-and-braces` and predict its output line by line before opening the `.expected`, then say why `{{braces}}` prints `{braces}}` | `tests/golden/run/interpolation-holes-and-braces.hero` · `tests/golden/ir/interpolation-desugar.hero` | only `{{` is doubled, because a `}` in text mode is text — Python doubles both for a grammar this lexer does not have

    **Origin:** M-interpolated-strings close, 2026-09-09.
