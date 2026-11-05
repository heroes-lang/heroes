# strings: `use core/strings`

Text searched, trimmed and split: Go's `strings` package, written in Heroes.
Offsets are byte offsets.

## Types

`record Halves` with `before: str` and `after: str`: the text before a separator and the text after it.

## Functions

- `contains(s: str, substr: str) -> bool`: whether `substr` occurs in `s`; the empty text occurs in every text.
- `has_prefix(s: str, prefix: str) -> bool`: whether `s` begins with `prefix`.
- `has_suffix(s: str, suffix: str) -> bool`: whether `s` ends with `suffix`.
- `position(s: str, substr: str) -> i64?`: the byte offset where `substr` first occurs in `s`; fails `not_found`.
- `last_position(s: str, substr: str) -> i64?`: the byte offset where `substr` last occurs in `s`; fails `not_found`.
- `count(s: str, substr: str) -> i64`: how many times `substr` occurs in `s`, not overlapping.
- `trim_space(s: str) -> str`: `s` without the spaces, tabs and line ends at either end.
- `trim_prefix(s: str, prefix: str) -> str`: `s` without `prefix` where it begins with it; otherwise `s`.
- `trim_suffix(s: str, suffix: str) -> str`: `s` without `suffix` where it ends with it; otherwise `s`.
- `split(s: str, sep: str) -> [str]`: the parts of `s` between the occurrences of `sep`, empty parts kept.
- `split_once(s: str, sep: str) -> Halves?`: the text before the first `sep` and the text after it; fails `not_found`.
- `fields(s: str) -> [str]`: the runs of `s` between spaces, tabs and line ends, never an empty one.
- `split_lines(s: str) -> [str]`: the lines of `s`, each without its `\n` or `\r\n`; a text ending in a line end has no empty line after it.
- `replace_all(s: str, old: str, new: str) -> str`: `s` with every occurrence of `old` replaced by `new`, not overlapping.

## Codes

- `not_found`: the text searched for does not occur.
