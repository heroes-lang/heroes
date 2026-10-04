---
kind: decision
area: none
milestone: none
filed: 2026-09-27
commit: 29ed560108e1511efcf304e7432d30e911279818
github: none
---

# A block head printed across lines stands past the body's column

2026-09-27 | every line of a block head `fmt` prints across lines after its
first (an `if`, `else if`, `while`, `for`, `match`, a signature, a constant's
head) goes eight columns further in than a bracket's lines go elsewhere, its
closing line included, so the closing line stands four past the body's column
and nothing in the head stands at it (`print/margins.hero`, `deepened`) | at
the body's column a continuation read as the body's first line, and a `)` at
the head's own column as a statement the block then hung under (the third
skeptic seat's `m3` and `sh3` over lane g's fourth round, output that reads
wrong at exit 0); `fmt` prints a head across lines to keep a comment inside
it (`print/page.hero`'s list), and measured over the trunk's 886 `.hero`
files the rule changes the printed bytes of three, the `paramcomment095/`
fixtures | design.md §4.15 (*multi-line
calls, signatures and literals indent freely*; neither it nor the spec names
a column) | none: a column inside brackets is not a CLAUDE.md § 4 path, written
here so the author can send it to one
