# Panel 188, the facts measured before the briefs

Measured by the coordinator on 2026-10-03 between 14:50 and 15:50 by `date`,
then corrected and widened after the completeness critic's first pass
(`docs/panel/188-reports/completeness-critic-briefs.md`; the text it read is
`00-facts-before-the-critic.md`). Each fact names its command; F1 and F6 ran on
the trunk's compiler at `e339ece9`, whose compiler files `826ddc2f` (the frozen
head) carries unchanged (`git diff --stat e339ece9 826ddc2f -- selfhost seed
runtime tests spec` is empty).

## F1. What a header name does today, character by character (this Mac)

The generator wrote, in a folder of its own (`<scratchpad>/188-facts/case-*/`),
one header of the given name holding `static inline int32_t seven(void) {
return 7; }` over `<stdint.h>`, and a program `p.hero` binding it, `extern
"<name>"` over `function seven() -> i32`, `main` printing `seven()`; then
`heroes check p.hero`, `heroes build p.hero -o p` (with `HEROES_RUNTIME` set to
the trunk's `runtime/`) and `./p`. The name is written in the Heroes string
with `\\`, `\"` and `\n` escapes where it holds them.

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

**Why the last three fail (the critic's finding, F8)**: not clang. The
compiler writes the string's SOURCE spelling into the `#include`, its escapes
undecoded: `extern "a\\b.h"` becomes `#include <a\\b.h>`, two backslashes,
where the name has one. This Mac's clang compiles `#include <a\b.h>` and
`#include <a"b.h>` with the file found and no warning under `-Wall -Wextra`
(the critic, in its copy). So the table has three causes, not two: what
`#include <...>` cannot carry (`>`, a line end), the escapes the compiler does
not decode (`\\`, `\"`, `\n`), and the false message both produce.

Unrun: F1 on Linux and on the Windows box (offline since about 14:47,
Tailscale reading it *last seen 54m ago* at 15:41; still offline at 16:12, the
critic).

*Run 2026-10-03 by the ffi-pragmatist (§ 1.3): F1 in the Linux arm64 image,
identical to this Mac row for row and message for message. The Windows box
stayed offline through the sitting.*

## F6. C's angled spelling written inside the quotes (this Mac, 15:48)

Measured as F1, binding `function puts(s: cstr lent) -> i32` and calling it,
with no header written beside the program (`<scratchpad>/188-facts/case-angle-*/`,
written at 15:48:48 by their files' times):

| written | `check` | `build` |
|---|---|---|
| `extern "<stdio.h>"` | 0 | **2**, *internal error: compiling the generated C failed* |
| `extern "stdio.h>"` | 0 | **0**: a malformed header name builds and the program runs |
| `extern "<stdio.h"` | 0 | 1, `ffi_missing_header`, *`<stdio.h` is not on this machine's include path* |

The first is C's own `#include <stdio.h>` carried into the string. **Whether
models write it is a question, not a premise**: none of the 832 `extern`
lines in the tree puts brackets inside the quotes (the critic's count). The
second is a wrong program accepted. Both are `blocking` by the classes of
`.claude/rules/verification.md` § Bounded discovery. Unrun: whether the
second's build drew a clang warning, and on which platform it differs.

*Answered 2026-10-03 by the seats: the second's build prints clang's
`-Wextra-tokens` twice, once for the pointee probe's unit and once for the
program's, pointing at the generated C (the spec-warden, the
compiler-engineer § 1), the same under Debian clang 22.1.8 and 18.1.8 and
through the compiler in the Linux arm64 image (the ffi-pragmatist § 1.1 and
§ 1.3); Windows unrun. And whether models write the first was
measured: 0 of 13 fresh sessions (`docs/panel/188-reports/llm-ergonomist-gen.md`).*

## F7. More of the class, run by the critic (its copy, 826ddc2f's compiler)

- A raw NUL byte in the name: `check` 0, `build` **2**, an internal error.
- `extern ""`: a false `ffi_unknown_name` (*clang read the header*), where
  clang said *empty filename*.
- `\t` and `\r` in the name fail as `\n` does (F1).
- `a<b.h` checks, builds and prints `7`: `<` is carried.

*Corrected 2026-10-03, after the seats (the ffi-pragmatist § 1.4, re-run by
the critic's second pass § 5): the NUL row above holds only where the part
before the NUL names no file. Where it does, `build` is 0 and the program runs
on that file: `extern "stdio.h<NUL>x"` binds the system's `stdio.h` on this
Mac and in the Linux arm64 image. And a shape the facts did not hold (the
critic's second pass § 3): a `\` as a name's last byte, before the
closing `>`, sends clang down its token path, so an angled include does not
carry it as the name it is either.*

## F8. Where the name is read, written and judged (`grep`, `sed`, at `826ddc2f`)

- `selfhost/parse/group_head.hero` (164 lines by `wc -l`): `header` at line 30
  bumps the string and calls `machine_locked` (its comment from line 60, the
  function at line 72), panel 055's refusal of an absolute path: a leading
  `/`, `\` or `:`, or a second byte `:` (a drive letter), code
  `machine_locked_path`, no fix (*"a certain fix that rewrote the string
  would be a lie"*). The same function judges the library string of a `link`
  or a `package` (`what` is `"library"` there). Nothing else judges the
  header's characters before `build`.
- **Neither reader of a header string decodes its escapes**:
  `parse/module_text.unquote` and `emit/externs.unquoted` cut the quotes and
  keep the source spelling, though spec § 2 defines five string escapes (the
  critic).
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
  (`da3e29af`). **Panel 187 registered a prediction that `selfhost/parse/`
  stays at most 8,696 lines at the next `m-*` tag**: raising the row fails it.
- **The specification** (`heroes measure spec/heroes-spec.md` at `826ddc2f`):
  `cl100k_base` 7,117, `claude-legacy` 6,990, **real 9,392** (`claude-opus-5`,
  recorded 2026-10-03). design.md §1.6's payment rule binds an addition.

## F4. What the specification says today, and what it said

§ 13: *"A group names its header, and `link` a library when the symbols need
one."*, and the production `Extern = "extern" string [ ( "link" | "package" )
string ] NEWLINE INDENT { Member } DEDENT .` No sentence says which characters
a header's name may hold. **Panel 055's absolute-path sentence was in the
spec and was removed on purpose on 2026-09-05** (`aab44f9b`), on the grounds
that the compiler's message already states the rule (the critic).

## F5. Panel 036's R2 (`docs/panel/036-the-ffi-ladder.md`, line 147)

*"`<header.h>` + `-I<source dir>`, never the quoted form"*, held because a
quoted include takes a decoy: the sitting planted a `sqlite3.h` beside the
generated unit in `build/<hash>/` and `#include "sqlite3.h"` compiled against
it (line 48 onward).
