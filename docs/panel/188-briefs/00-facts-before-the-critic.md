# Panel 188, the facts measured before the briefs

Measured by the coordinator on 2026-10-03 between 14:50 and 15:50 by `date`.
Each names its command; F1 was run on the trunk's compiler at `e339ece9`,
whose compiler files `826ddc2f` (the frozen head) carries unchanged (`git diff
--stat e339ece9 826ddc2f -- selfhost seed runtime tests spec` is empty).

## F1. What a header name does today, character by character (this Mac)

The generator wrote, in a folder of its own under the scratchpad
(`<scratchpad>/188-facts/case-*/`), one header of the given name holding
`static inline int32_t seven(void) { return 7; }` over `<stdint.h>`, and a
program `p.hero` binding it, `extern "<name>"` over `function seven() ->
i32`, `main` printing `seven()`; then `heroes check p.hero`, `heroes build
p.hero -o p` (with `HEROES_RUNTIME` set to the trunk's `runtime/`) and `./p`.
The name is written in the Heroes string with `\\`, `\"` and `\n` escapes
where it holds them.

| the header's name | `check` | `build` | what happens |
|---|---|---|---|
| `ab.h` | 0 | 0 | prints `7` |
| `a b.h` (a space) | 0 | 0 | prints `7` |
| `a'b.h` | 0 | 0 | prints `7` |
| `d//b.h` (the header in directory `d`) | 0 | 0 | prints `7` |
| `e/*b.h` (the header `*b.h` in directory `e`) | 0 | 0 | prints `7` |
| `a??)b.h` | 0 | 0 | prints `7` (defect 207's line splice) |
| `a>b.h` | 0 | **2** | *internal error: compiling the generated C failed*, clang's `#include <a>b.h>` and *'a' file not found*, then *error: clang refused the generated C* (defect 216) |
| `a\b.h` | 0 | 1 | `ffi_missing_header`: *`a\\b.h` is not on this machine's include path, clang looked and did not find it*, **while the file is there** |
| `a"b.h` | 0 | 1 | `ffi_missing_header`, the same words, **while the file is there** |
| `a` + line end + `b.h` | 0 | 1 | `ffi_missing_header`, the same words, **while the file is there** |

So on this Mac the class has two faces: an exit 2 with an internal error
(`>`), and a **false message** (`\`, `"`, a line end): `build` says the header
is missing where it exists. Both are `blocking` by the classes of
`.claude/rules/verification.md` § Bounded discovery.

Unrun: the same table on Linux and on the Windows box (offline since about
14:47, Tailscale reading it *last seen 54m ago* at 15:41); a name holding a
NUL (whether a Heroes string can carry one is a question); a name holding a
byte above ASCII.

## F6. C's angled spelling written inside the quotes (this Mac, 15:58)

Measured the same way as F1, binding `function puts(s: cstr lent) -> i32` and
calling it, with no header written beside the program (`<scratchpad>/188-facts/case-angle-*/`):

| written | `check` | `build` |
|---|---|---|
| `extern "<stdio.h>"` | 0 | **2**, *internal error: compiling the generated C failed* |
| `extern "stdio.h>"` | 0 | **0**: a malformed header name builds and the program runs |
| `extern "<stdio.h"` | 0 | 1, `ffi_missing_header`, *`<stdio.h` is not on this machine's include path* |

The first is C's own `#include <stdio.h>` carried into the string, the
spelling a model fluent in C reaches for; the second is a wrong program
accepted. Both are `blocking` by the classes of
`.claude/rules/verification.md` § Bounded discovery. Unrun: whether the
second's build drew a clang warning, and on which platform it differs.

## F2. Where the name is read, written and judged (`grep`, `sed`, at `826ddc2f`)

- `selfhost/parse/group_head.hero` (164 lines by `wc -l`): `header` at line 30
  bumps the string and calls `machine_locked` (its comment from line 60, the
  function at line 72), panel 055's
  refusal of an absolute path: a leading `/`, `\` or `:`, or a second byte `:`
  (a drive letter), code `machine_locked_path`, no fix (*"a certain fix that
  rewrote the string would be a lie"*). Nothing else judges the header's
  characters before `build`.
- `selfhost/emit/c_text.hero`: `includes` at line 99 writes each header's
  `#include <...>` line; since defect 207 a header name that would hold a
  trigraph is split by a line splice (its comment, lines 15 and 110).
- `ffi_missing_header` is built at `selfhost/emit/ffi_build.hero:205` and
  `selfhost/emit/header_reach.hero:96`.

## F3. The budgets the repair meets

- **The parser's line budget** (panel 187's R9): `tests/harness/suite_layout.hero`
  line 490, `"selfhost/parse/ 8696"`. Counted with the scratchpad's mirror of
  that suite's `code_lines` (`<scratchpad>/184-compiler-engineer/code_lines.py
  selfhost/parse/*.hero`, summed): **8,689 lines over 54 files, 7 of room**.
  The suite is the judge and its `budget` check passed at the sixth gate
  (`da3e29af`). `layout/budget`'s own message: *a repair pays for its lines by
  deleting or moving others, and a `blocking` one that cannot raises the row
  with its reason*.
- **The specification** (`heroes measure spec/heroes-spec.md` at `826ddc2f`):
  `cl100k_base` 7,117, `claude-legacy` 6,990, **real 9,392** (`claude-opus-5`,
  recorded 2026-10-03). design.md §1.6's payment rule binds an addition.

## F4. What the specification says today (`spec/heroes-spec.md`, § 13 FFI)

*"A group names its header, and `link` a library when the symbols need one."*
and the production `Extern = "extern" string [ ( "link" | "package" ) string ]
NEWLINE INDENT { Member } DEDENT .` No sentence says which characters a
header's name may hold, nor that an absolute path is refused (`grep -n -i
'absolute\|machine' spec/heroes-spec.md` finds neither in § 13).

## F5. Panel 036's R2 (`docs/panel/036-the-ffi-ladder.md`, line 147)

*"`<header.h>` + `-I<source dir>`, never the quoted form"*, held because a
quoted include takes a decoy: the sitting planted a `sqlite3.h` beside the
generated unit in `build/<hash>/` and `#include "sqlite3.h"` compiled against
it (line 48 onward).
