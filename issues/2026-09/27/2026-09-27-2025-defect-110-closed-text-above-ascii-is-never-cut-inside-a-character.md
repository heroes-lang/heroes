# Defect 110 closed: text above ASCII is cut beside an ASCII byte or not at all, in the interpolated string and in eight other places

2026-09-27, M-agreed-retention step 22, in lane A (`7bcd7cc2`), merged `f08b192d`.
Found by lane 105's agent; repaired by lane A's agent, and the eight places
beside it found by an audit of every `.slice(from:` in `selfhost/` run in the
same lane.

- [x] **110 — building a program whose interpolated string holds a character above ASCII before a hole panics** | `print(f"é {x}")` is `check` 0, and `heroes build` or `run` exits 134 with `panic: string slice splits a character`: `lex_interp.piece_text` slices one byte of a two-byte character | `selfhost/lex_interp.hero:104` (`piece_text`) · **closed 2026-09-27**

    **Origin:** lane 105's agent, 2026-09-27, kept unchanged in that
    refactor; re-run by the coordinator on the trunk's compiler at
    `2b1a1f24`: `check` exit 0, `run` exit 134. Beside defect 103's
    `bytes.char_at`, which reads one character by its lead byte.

    **Why it is a defect.** A program the checker accepts crashes the
    compiler; the robustness goal (design.md §1.12) is the first thing it
    breaks, and the spec lets a string hold any UTF-8.

## What it was, and what it was beside

`piece_text` decoded a piece of an interpolated string by slicing it one
byte at a time, so the first byte of `é` was a slice that splits a
character. It aborted without a hole too: `print(f"é €")` died the same way.

The class is a string cut at a byte offset that can fall inside a
character. Every `.slice(from:` in `selfhost/` (186 lines in 92 files) was
read in context and every site judged unsafe was run; eight more were
reached with the trunk's compiler, each `panic: string slice splits a
character`, exit 134:

| where | the cut | what reached it |
|---|---|---|
| `check/builtins.hero` `rfind` | a slice per offset, walking back | `x = 5.default("é")`, `x = must("é")`: `check` aborted |
| `print/sheet.hero` `tail` | the page's last two bytes | a comment ending in `é`, `€` or `😀` closing a body or a record with a declaration after it: `fmt` aborted on a program `check` accepts |
| `cli/mutate.hero` `ends_hero` | the path's last five bytes, under a comment saying it compared bytes | a file named `é.txt` in the corpus directory |
| `cli/header_types.hero` `locate_text` | a slice after one byte was compared | an `@` parameter whose header type is named above ASCII, `typedef int 漢;`: `build` aborted on a binding `check` accepts |
| `cli/header_types.hero` `strip_word` | the last bytes of a pointee type | a pointee named `aébbbbbbb` |
| `cli/clang_floor.hero` `text_after` | a slice per offset | a `clang --version` line with a character above ASCII before `version `: every `build` and `doctor` |
| `cli/runtime_key.hero` `included_names` | a line counted in characters against a length in bytes, then sliced | a `runtime.c` line whose tenth byte falls inside a character: every `build` of every program |
| `cli/runtime_key.hero` `included_names` | a byte at a time for the closing quote | `#include "café.h"` in the runtime |

Beside them, three sites the audit could not reach and that are repaired
for the same reason: `module/diagnostics.hero` `suggestion`, which only
sees identifier text today; and in the harness `suite_lines.unquoted`,
which copies a `#line` name a byte at a time while the emitter writes a
path's own bytes there, and `shell.ends_with`.

One site is **not** repaired here: `handles.releasers`, defect 108's
function, which lane B is changing. A comment holding a character above
ASCII inside an `acquires a | b` set aborts `check` and `fmt` there with
the same panic; defect 108's repair takes comments out of the set, and
the lexer refuses a name above ASCII before the set is read.

## The repair

- **`piece_text` cuts only beside a doubled brace**, one byte never inside
  another character, and copies the runs between them whole.
- **`bytes.bytes_at(text:, at:, needle:)`**, whether a needle's bytes stand at
  an offset, asked a byte at a time and never by slicing. A match found this
  way starts on a character boundary, since a well-formed needle's first
  byte is never a continuation byte, and ends on one. `rfind`,
  `locate_text`, `strip_word`, `text_after`, `ends_hero` and
  `included_names` ask it, and `sheet.tail` became `sheet.ends_with`, the
  question its one caller asked.
- **`included_names` reads bytes throughout**, which also drops the
  `chars()` of the whole runtime at every build.
- `suggestion` and `suite_lines.unquoted` cut beside their ASCII separator;
  `shell.ends_with` asks the harness's byte-comparing `strings.ends_with`.

## The pins

Seven compiler tests, one per reader the audit reached or its neighbour: a
needle found by its bytes at any offset, a character's middle included; the
last occurrence and the removal found by their bytes past characters above
ASCII; the dump searched by its bytes with a type named above ASCII read
whole; a corpus file known by its last five bytes; the includes read by
their bytes; a character above ASCII copied whole before, after and between
holes, and beside a doubled brace and an escape; a suggested name keeping
every character whole. The `run` golden
`fixedbugs-an-interpolated-string-holds-characters-above-ascii` with its
blessed C, printing the characters byte for byte; and two `surface` rows
over `surface-fixtures/nonascii110/`, comments ending in characters of two,
three and four bytes printed back by `fmt`, and a call that unwraps what
cannot fail with such a character in it refused by `check`, where each
aborted.

## The measurements

The lane's gate and platforms are its commit's body (`7bcd7cc2`): the
compiler's 778 tests, the net's own 172, every suite 0 failed, Linux arm64
and x86-64 and the Windows box at run 206, 206 and 204 with 0 failed. On
the trunk after the merge (`f08b192d`): the seed emitted again by the
trunk's compiler, fixpoint by `cmp`; the compiler's 782 tests, the net's
own 172; the full net 2741 passed and 1 failed, the annotations floor the
two lanes moved together, raised in the merge, annotations 199 and 0.
