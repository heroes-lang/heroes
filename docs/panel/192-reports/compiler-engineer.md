# Panel 192, the compiler-engineer: what a string or a comment may hold, and how a refused character is written

Started 2026-10-04 18:15:51 (from `date`). Written as I go. I take Q1, Q2, Q3
and Q7, and I build what I recommend (`/panel` § 3c). My brief is
`docs/panel/192-briefs/compiler-engineer.md`, read with `00-shared.md`,
`00-facts.md` and the critic's first pass
(`docs/panel/192-reports/completeness-critic-briefs.md`). **The account's
session limit stopped me at about 18:30; I resumed at 21:07:45 by `date`**,
from this file and my copy, which the stop left as it was.

**My copy**: `<scratchpad>/192-compiler-engineer/`, from `git -C <trunk>
archive 4c3524fb | tar -x`, made 18:16:10. Its seed's sha256 begins
`c79ffd5ad005c301`; the compiler built from it (18:16:14 to 18:16:19) has
sha1 `8084f018f53875362048cc0230d23207976d8b00`. Both match `00-facts.md`'s
base. My scripts and cases are in `<scratchpad>/192-ce-work/`.

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.

## 1. F5 counted by context, with the compiler's own lexer

Run 18:21:49 to 18:21:56 by `date`: `192-ce-work/bycontext.py` reads each of
the 2,014 tracked `.hero` files at `4c3524fb` (`git ls-tree -r --name-only
4c3524fb`, filtered to `.hero`), finds every character of the classes below,
and places it in the token `heroes lex <file> --dump-tokens --json` says
holds it (the dump's `col` counts code points: measured on `t2.hero`, where
`é` and U+200B each moved the next token by one). Output:
`192-ce-work/base-counts.txt` and `base-where.txt`.

| class | in a string | in a comment | in a character literal | in an `error` token (code) | elsewhere |
|---|---|---|---|---|---|
| tab | 1 (`json102/tab.hero`) | 0 | 0 | 0 | 68 in 7 files (margins of cases 130, 131) |
| C0 but tab, CR and LF, and DEL | **4 in 3 files**: `compile.hero:83` (2), case 244 `:12`, case 289 `:8` | 0 | 0 | 15 in 2 files (case 282, `json102/control.hero`) | 0 |
| C1 | 0 | 0 | 0 | 0 | 0 |
| bidirectional controls | 0 | 0 | 0 | 1 (`json102/unseen.hero:2`) | 0 |
| U+2028, U+2029 | 1 (case 216 `:29`, a group head) | 0 | 0 | 0 | 0 |
| other Default_Ignorable | 1 (case 216 `:26`, a group head) | 0 | 0 | 1 (`json102/unseen.hero:3`) | 1 (case 242's BOM, `:1:1`) |
| `UNSEEN`'s spaces (NBSP, U+2000 to U+200A, U+202F, U+205F, U+3000) and U+FFF9 to U+FFFB | 0 | 0 | 0 | 0 | 0 |
| NUL | 0 | 0 | 0 | 0 | 0 |
| CR | 1 (`raw-carriage-return`) | 0 | 1 (the same case) | 1 (case 135, `crlf`) | 100 in 5 files: two CRLF cases (135) and three stray-CR cases |

Four files are not UTF-8 (defect 227's cases) and were not lexed.

**What this changes in F5**: the critic's approximation holds, and the
compiler's lexer sharpens two of its rows. **No tracked comment holds any of
these characters, of any class**: a refusal in comments moves no tracked
file. Cases 244 and 289's ESC sits inside a string, after a backslash, so it
is `unknown_escape` already and a raw-character refusal does not reach it
(the escape step consumes the byte after a backslash). The two strings of
case 216 are group heads' strings, which the lexer lexes as every string:
**a lexer refusal reaches them, and `unshowable_name` already refuses them**,
so the two must be made one message (section 4).

## 2. Where each reader is

- **The comment**: `scan.comment`, `selfhost/scan.hero:108`, found by `grep
  -n "^function comment\|comment(@l, text)\|comment_token(@l"`: called from
  `line_body` at `:76` on `#`, and from `foreign_comment` at `:194` for a
  `//` comment; a `/* */` comment's token is made at `:175`. It reads to the
  line feed and drops a trailing CR, and asks no byte anything else.
- **The string**: `literals.string`, `selfhost/literals.hero:96`, its raw
  byte branch at `:113` to `:120` (`raw_carriage_return` at `:119`).
- **The f-string's text**: `lex_interp.scan_piece`,
  `selfhost/lex_interp.hero:79`, its raw byte branch at `:102` to `:105`.
- **The character literal**: `literals.char_lit`, `selfhost/literals.hero:135`,
  its raw byte branch at `:160` to `:165`. It accepts one ASCII byte as one
  unit, so a raw ESC, a raw tab and a raw NUL are each a character literal
  of value 27, 9 or 0 today.
- **The decoders a new escape meets in the compiler**: `escape.unescape`
  (`selfhost/escape.hero:101`), called by lowering
  (`ir/flatten.hero:1159`), the checker's `literal_value`
  (`check/contextual.hero:154`, which reads a character literal's value as
  the decoded text's FIRST BYTE, `text[0]`), `check/reach.hero:100`, the
  group head's judge (`head_names.hero:28`) and its emitter
  (`emit/externs.hero:73`); and `lex_interp.piece_text`
  (`lex_interp.hero:146`), whose comment states a premise a braced escape
  breaks: *"no escape's second byte is a brace"*.
- **The harness's three named by the critic**: `suite_lines.hero:327`
  undoes the C escapes the emitter writes in a `#line` name, and
  `suite_records.hero:1919` strips quotes; **neither decodes a Heroes
  escape**, so neither is moved by one. `suite_spec.hero:1343`
  (`compiler_escapes`) is: it reads the escapes from `escape_text`'s
  `if after_backslash == "x"` rows, and the `spec/colouring` check holds them
  against the TextMate grammar.

## 3. A correction to the critic, measured

The critic's section 4, question 7, says *"`[1].validated_bytes().must()`
keeps the cache key's bytes on today's seed"*. Run at 21:07:55 by `date`
(`192-ce-work/a1/soh.hero`): **it does not check**,
`error[bad_operand]: validated_bytes takes a run of bytes, i8[N], u8[N] or
[u8], found [i64]`, since an array literal as a receiver takes `[i64]`. The
route needs a typed binding, `soh: [u8] = [1]` then
`soh.validated_bytes().must()` (`a1/soh2.hero`: checks at 0, the key's
length 6, its bytes 1 and `L`). That is two lines more than today's one, and
`selfhost/cli/compile.hero` stands at **300 of its 300** in the layout's unit
(`192-ce-work/code_lines.py`, a replica of `suite_layout.code_lines`; the
suite is the judge, run below).

## 4. What five other compilers on this Mac do, run rather than recalled

Run 21:09:18 to 21:09:26 by `date` (`192-ce-work/others/matrix.py`, one file
per language, character and context, each run alone; every output kept
beside its file as `.out`): rustc 1.90.0 (`--emit=metadata`), zig 0.15.2
(`ast-check`), go 1.27.1 (`go vet`), Apple clang 21 (`-fsyntax-only -Wall
-Wextra`), Python 3.14.8 (`compile`). Each holds the raw character in
`"a<X>b"`, and again in a line comment. `ERR` is a refusal, `warn` a
warning, `ok` silence.

| | NUL | ESC | TAB | DEL | NEL (U+0085) | LRM | RLO | RLI | U+2028 | ZWSP | ZWJ | VS16 | NBSP |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Rust, string | ok | ok | ok | ok | ok | ok | **ERR** | **ERR** | ok | ok | ok | ok | ok |
| Rust, comment | ok | ok | ok | ok | ok | ok | **ERR** | **ERR** | ok | ok | ok | ok | ok |
| Zig, string | **ERR** | **ERR** | **ERR** | **ERR** | ok | ok | ok | ok | ok | ok | ok | ok | ok |
| Zig, comment | **ERR** | **ERR** | **ERR** | **ERR** | ok | ok | ok | ok | ok | ok | ok | ok | ok |
| Go, both | **ERR** | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok |
| C (clang), string | warn | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok |
| C (clang), comment | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok |
| Python, both | **ERR** | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok | ok |

What the messages say: Rust's is the lint
`text_direction_codepoint_in_literal`, deny by default, *"unicode codepoint
changing visible direction of text present in literal"*, with the help *"if
you want to keep them but make them visible in your source code, you can
escape them"* and the rewrite `"a\u{202e}b"`; it does not cover LRM, an
implicit mark. Zig's is *"string literal contains invalid byte: '\t'"* and
*"comment contains invalid byte: '\x1b'"*. Go's is *"illegal character
NUL"*; clang's *"null character(s) preserved in string literal
[-Wnull-character]"*.

**Two readings of it, both measured.** No compiler of the five refuses ZWJ,
VS16, ZWSP or NBSP anywhere: panel 188's list in a string would refuse more
than any of them. And the union of what they refuse, Zig's controls and
Rust's explicit bidirectional controls, in strings and comments alike, is
nearly the route I recommend below, which takes the properties those two
lists are cut from.

## Status

In progress, 21:17.
