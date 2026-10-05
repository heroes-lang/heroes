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

## Resumed 2026-10-05 00:39:27 by `date`

The author paused all work at 21:26; this file stood as committed at
`99a67630`. From here it is appended to only: a sentence I correct is
corrected underneath, with its time. My copy and its base `4c3524fb` are as
the pause left them: `selfhost/raw_chars.hero` written, `selfhost/escape.hero`
edited, nothing built yet.

## 5. What I built, and the landing order it took (written 00:56)

Built in my copy between 00:41 and 00:55 by `date`; the base's compiler is
kept as `192-ce-work/heroes-base`.

- **`selfhost/raw_chars.hero`** (new): the refusal. One predicate,
  `shown_char.unshowable`, asked of every byte of a string, an `f"…"`
  literal's text, a character literal and a comment, through one call per
  scanner (`raw_chars.past`, a byte test first so printable ASCII costs three
  comparisons). A run of refused characters is one message at its first, its
  certain fix writing every one of them. Codes: `invisible_character` (a
  control, a bidirectional control, U+2028 or U+2029), `raw_tab` (a tab in a
  string or a character literal), `nul_in_string`. `raw_carriage_return`
  moved here from `literals.hero`, unchanged, to keep that file under its 300.
- **`selfhost/code_escape.hero`** (new): route (d), `\u{hex}`, legal in a
  string or an `f` literal's text exactly for a character a string refuses
  written as itself, the NUL and the CR excepted; any other code is refused
  with its one spelling (`escape_not_needed`), the NUL with none
  (`nul_in_string`), a surrogate or a code past U+10FFFF as
  `unknown_escape`. Hex digits in either case, one to six of them.
- **`selfhost/heads_told.hero`** (new): defect 250's dedup, moved from
  `head_names.hero` (at 298 of 300) and widened, so a raw character in any of
  a group head's three strings is one message, the head's.
- **Edited**: `escape.hero` (decodes `\u{…}`; `end_of`, past an escape, for
  the three readers of a literal's braces), `literals.hero` (the escape step,
  the string's raw bytes, the character literal), `lex_interp.hero` (the
  `f` text's raw bytes; `piece_text` steps over an escape's braces),
  `scan.hero` (the comment), `escape_report.hero` (what `unknown_escape`
  says of an escape by code), `escape_readings.hero` and `plain_holes.hero`
  (each steps over an escape's braces), `shown_char.hero` (the predicate,
  and `visible` writes a bidirectional control and U+2028 by its code, so no
  excerpt of a refused line reorders the message that quotes it),
  `lexer.hero`, `refused_runs.hero` (two citations), `cli/compile.hero:83`.

**The landing order, run** (Q3's last question):

1. The base's compiler (today's seed) checks the tree with the escape and the
   refusal and `compile.hero:83` still raw: exit 0, and builds it as
   `heroes-a` (00:46:49 to 00:48:12).
2. With `:83` written `"\u{1}I" + … + "\u{1}L" + …`, **the base's compiler
   refuses the tree**: `unknown_escape` twice at `compile.hero:83`, exit 1.
   `heroes-a` checks it at exit 0. And `heroes-a` **refuses the tree of
   step 1**: `invisible_character` twice at `:83`, exit 1. So neither tree
   compiles under both compilers: the escape must reach a seed before the
   source may use it, and the refusal cannot land while `:83` is raw.
3. `heroes-a` builds the final tree as `heroes-b` (00:48:47 to 00:49:50);
   `heroes-b --emit-c` writes the seed (35,467,773 bytes, sha256 beginning
   `5f5e1e0eb1b38da2`); clang builds it as `heroes-c`, whose own emission is
   **byte for byte the same, by `cmp`** (00:55:22). The fixpoint holds in
   two generations.
4. The cache key keeps its bytes: the new seed holds
   `HERO_STR_STATIC(hero_str_cc, "\001I")` and `"\001L"` (lines 3233 and
   3234), as the base's seed holds `"\001I"`. Every existing build directory
   keeps its name.

So **one commit can carry all of it**, the seed included, for anyone who
builds from the seed; but whoever regenerates that seed builds twice, once
from the tree with `:83` raw (step 1) and once from the final tree. Route
(a′) at `:83` would avoid the two-step build, and costs two lines in a file
at 300 of its 300 (section 3), which would have to move something out.

My copy's `seed/heroes.c` and `heroes` are now generation B's (`heroes`
sha1 beginning `f1a4117b3d5dff35`).

## 6. Q1 and Q2 on the built route, run (00:58 to 01:00)

Cases in `192-ce-work/q/` and `q7/`, each a `function main()` program,
written byte for byte by a script. `base` is `192-ce-work/heroes-base`, the
route is my copy's `heroes`.

**Every refused shape, at `check`, `check --permissive` and `build`** (exit
codes; the base checks every one at 0 but the group head):

| case | base `check` | route `check` | `--permissive` | `build` |
|---|---|---|---|---|
| raw ESC in a string (`s-esc`) | 0 | 1 | 1 | 1 |
| raw RLO, LRM, U+2028, NEL (U+0085) in a string | 0 each | 1 each | 1 each | 1 each |
| raw tab in a string | 0 | 1 | 1 | 1 |
| raw NUL in a string | 0 | 1 | 1 | 1 |
| raw RLO, ESC, U+2028, NUL, lone CR in a comment | 0 each | 1 each | 1 each | 1 each |
| raw ESC, tab, NUL, DEL in a character literal | 0 each | 1 each | 1 each | 1 each |
| raw ESC in an `f` literal's text | 0 | 1 | 1 | 1 |
| raw ESC in a `test` title | 0 | 1 | 1 | 1 |
| raw ESC in an `extern` header and in a `link` string | 1 | 1, one message each, `unshowable_name` | **0**, as on the base | 1 |
| a tab in a comment; ZWJ in a comment | 0 | 0 | 0 | 0 |

So **no new refusal is dropped by `--permissive`**: each is kept, as panel
066's `raw_carriage_return` is. In a group head the head's thesis rule,
`unshowable_name`, stays the one message, and the control arm reads the head
as it did. The messages, `--brief`:

- `error[invisible_character]: this string holds the control character
  U+001B written as itself, which no screen shows and a terminal acts on: a
  string writes it by its code, as \u{1b}`, fix (certain) `\u{1b}`; a run
  `ESC BEL DEL` is one message, *"and 2 more after it"*, its fix
  `\u{1b}\u{7}\u{7f}`.
- `error[invisible_character]: this string holds the invisible character
  U+202E written as itself, which changes how its line is shown, so the line
  can read as another program: a string writes it by its code, as \u{202e}`.
- `error[raw_tab]: a tab written as itself inside a string: it reads as
  spaces, so the next reader retypes it as spaces, and a tab is written \t`,
  fix (certain) `\t`.
- `error[nul_in_string]: this string holds a NUL written as itself: no
  screen shows one, and C reads a string only to its first NUL, so a string
  cannot hold one`, fix (guess) *delete it*.
- `error[invisible_character]: this comment holds the invisible character
  U+202E written as itself, ...: write its name instead, <U+202E>`, fix
  (certain) `<U+202E>`: a comment's text changes, the program cannot.
- `error[invisible_character]: this character literal holds the control
  character U+001B written as itself, ...: a character literal is its
  character's code, so write the number, 27`, fix (certain) `27`.

**What real text writes under the route** (built and run, the output by
`od`): each is written as itself and prints exactly its bytes.

| program | prints |
|---|---|
| `print("👨‍💻 coder")`, ZWJ | `f0 9f 91 a8 e2 80 8d f0 9f 92 bb 20 ...` |
| `print("می‌خواهم")`, ZWNJ (Persian) | `d9 85 db 8c e2 80 8c d8 ae ...` |
| `print("⚠️ low disk")`, VS16 | `e2 9a a0 ef b8 8f 20 6c 6f 77 ...` |
| `print("🏴󠁧󠁢󠁳󠁣󠁴󠁿")`, Scotland's flag, tag characters | `f0 9f 8f b4 f3 a0 81 a7 ... f3 a0 81 bf` |
| `print("a<ZWSP>b")` | `61 e2 80 8b 62` (accepted: the residual below) |
| `print("a<NBSP>b<U+3000>c")` | `61 c2 a0 62 e3 80 80 63` |

**Under the proposal's list, panel 188's predicate**, the same characters in
a group head on the base (`y-*.hero`): ZWJ, ZWNJ, VS16, the tag U+E0067 and
ZWSP are each `unshowable_name`; NBSP and U+3000 pass. So the proposal would
refuse the first four programs above, correct text all four, and the
measured matrix of section 4 shows no other compiler of five that does.

**What the route leaves**: a zero-width space, a soft hyphen, a word joiner,
a mid-string BOM and the Hangul fillers stay legal written as themselves in
a string, invisible, so `"a<ZWSP>b"` and `"ab"` still look alike. That is
the price of not refusing what text spells with; carving Default_Ignorable
into what text needs and what it does not would be a judgement table, a
premise about the world (`.claude/rules/module-shape.md`), where the three
properties are facts about the code point.

**The escape, run**: `print("\u{1b}[31mERROR\u{1B}[0m")` prints `1b 5b 33 31
6d 45 52 52 4f 52 1b 5b 30 6d 0a`, F10's target byte for byte;
`print("ab\u{202e}cd")` prints `61 62 e2 80 ae 63 64 0a`; `x = 1` then
`print(f"\u{1b}[31m{x}\u{1b}[0m}}")` prints `1b 5b 33 31 6d 31 1b 5b 30 6d
7d 0a`, the hole read and the escape's braces never a hole.

## 7. Q3, writing what is refused: route (d), built, and what it says (01:00 to 01:09)

**What each spelling does, run on the route** (`192-ce-work/q/`, `q7/`):

| question | answer, run |
|---|---|
| a program printing a terminal's ESC | `print("\u{1b}[31mERROR\u{1B}[0m")` prints `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a` (section 6) |
| the NUL kept out | `"a\u{0}b"` is `nul_in_string`, *"`\u{0}` writes a NUL, and C reads a string only to its first NUL, so a string cannot hold one"*, no fix; a raw NUL is the same code |
| `fmt` printing it back | `fmt --in-place` over `print( "a\u{1B}b" )` writes `print("a\u{1B}b")`: the escape byte for byte, **its digits' case kept** (the gap below). Over a raw ESC it now refuses, *refusing to format a file with diagnostics*, the file untouched; on the base it wrote the raw ESC back, panel 066's trap for ESC |
| what `--dump-tokens` shows | the spelling, `2:11 str "ab\u{202e}cd"`; a raw override in a token, which the base dumped raw (`e2 80 ae`), is dumped `<U+202E>` |
| a character literal | no escape by code: `'\u{1b}'` is `unknown_escape`, *"a character literal is its character's code, an integer, and this language has no escape for a character by its code: write the character, or its number"*, fix (guess) `27`, as on the base. A raw ESC there is refused, fix (certain) `27`, the number it is (design.md §4.3). So design.md `:996`, *one ASCII character or one escape*, stands as written |
| a group head's string | `extern "a\u{1b}b.h"`: the escape decodes (`emit/externs.unquoted` and the head's judge both use `escape.unescape`), and the head refuses the value, `unshowable_name`, the one message, as it refuses `\t` there |
| every certain fix, applied | `check --apply` over 13 cases: each result checks at 0, and the seven that build print **the same bytes** as the base's build of the original (`cmp`), so each certain fix keeps the program's meaning |

**What `unknown_escape` says under the route** (the brief's five):

| written | the message, after *"is not an escape sequence"* | fixes |
|---|---|---|
| `\x1b` | *a string writes the control character U+001B by its code, as `\u{1b}`; write `\\` for a literal backslash* | guess `\u{1b}`, guess `\\x1b` |
| `\u{1b}` | accepted: it is the escape | |
| `\u001b` | as `\x1b` | the same two guesses |
| `\e` | as `\x1b` | the same two guesses |
| `\033` | as `\x1b` | the same two guesses |

The base's sentence, *"a string holds its characters as themselves, and
this language has no escape for one by its code"*, pointed the writer at a
raw ESC, defect 251's trap; it is said now only of a code a string holds as
itself (`\x41`). **May the fix be certain? No, measured**: `print("bin\x86")`,
a Windows path's backslash before `x86`, reads as the code U+0086, a C1
control, so a certain `\u{86}` would write a control character into a path
with no reader between (`x-path.hero`; defect 135's rule, a reading with two
readings is a guess). Only the escape's own braced spellings are one
reading, and those are accepted as they stand.

**Route (d) against design.md `:994-995`'s one spelling**, held: the escape
names a character only where a string refuses it written as itself, so no
character has two spellings. `\u{41}` is `escape_not_needed`, fix (certain)
`A`; `\u{e9}`, `é`; `\u{a}`, `\n`; `\u{22}`, `\"`; `\u{5c}`, `\\`; `\u{7b}`
in an `f` literal's text, `{{`; `\u{200b}`, a character a string holds as
itself and no screen shows, fix (guess) the character, since a certain fix
would write an invisible character into the file unread. **One gap**: the
hex digits are read in either case and with leading zeros, as a number's
digits are (design.md §4.3, *"a mask copied out of a C header arrives as
`0xFF"*; a code copied from Unicode's charts arrives as `U+202E`), so
`\u{1B}` and `\u{1b}` both check, and `fmt` keeps each as written, where it
lowercases `0xFF`. Owed, unbuilt: `fmt`'s canonical digits for the escape,
and its dump with it (`print/bodies.hero`'s `canonical_int` is the
precedent). The alternative, refusing a capital digit with a certain fix,
is three lines in `code_escape.hero` and costs a compile error a model would
not have earned.

**The other routes, each against the same questions**:

- **(a′), `[u8]` and `validated_bytes()`**: no compiler line, a few spec
  words at `:384`, works on today's seed, cannot make a NUL. But it cannot
  be a refusal's fix: panel 066 refused the raw CR only once `\r` existed,
  *"the escape is what made the repair writable"* (design.md §4.3; the
  comment that stood above `raw_carriage_return`, now in `raw_chars.hero`),
  and a `[u8]` binding is two statements away from the literal, no certain
  fix can write it. It cannot stand in a `constant`'s body or a `match`
  pattern. And measured here, the one-line form the critic gave does not
  check (section 3). **Kept as what it is**: a way to build bytes at run
  time, which the spec should say a `[u8]` answers (Q6, the spec-warden's).
- **(b), `\u{…}` for every code point**: the same code as (d) less one
  predicate call, and a second spelling for every character, `\u{41}` beside
  `A`, against `:994-995` and `:2029`, unless `fmt` rewrote it, which it
  does not do for any string today.
- **(c), `\xNN`**: writes bytes, so `\xff` makes a `str` that is not UTF-8
  (spec `:64`); limited to `01`-`7f` it writes none of the bidirectional
  controls, U+2028 or C1, which a string refuses under Q1. Refused.
- **(e), a named constant**: cannot stand inside a literal, and adds names
  to a language with no standard library. Refused.
- **(f), `fmt` writing a refused character by its escape**: `fmt` refuses a
  file with diagnostics, so beside a refusal it never runs on one; the
  repair it would make is the refusal's certain fix, which `check --apply`
  already writes. Instead of a refusal, it is a trap for every file nobody
  formats. Not needed.

## 8. Q7, the shapes beside, each run on the base and on the route (00:58 to 01:09)

| shape | the base | the route | its cause |
|---|---|---|---|
| a character literal holding a raw ESC (`'<ESC>'`) | `check` 0, value 27 | `invisible_character`, fix (certain) `27` | **shared**, built |
| an `f` literal's text holding a raw ESC | `check` 0 | `invisible_character`, fix `\u{1b}` | **shared**, built |
| a `test` title holding a raw ESC | `check` 0, and `heroes test` prints `FAIL "title <ESC>[2J here"` raw (F11) | refused at `check`; a title written `\u{1b}` is printed by the runner **as spelled**, `FAIL "title \u{1b}[2J here"`, no ESC reaches the terminal | **shared**, closed by the refusal |
| a doc comment holding ESC and U+202E | `check` 0 | two `invisible_character`, fixes `<U+001B>`, `<U+202E>` | **shared**, built |
| a name | refused already, ASCII-only (`:35`): `unexpected_character`, the code named | unchanged | not this cause |
| a diagnostic quoting a line that holds U+202E | the excerpt of an `unused_binding` over `x = "ab<U+202E>cd"` writes `e2 80 ae` raw, so the message's own line is reordered | the line is refused, and the excerpt writes `<U+202E>`: `shown_char.visible` now writes a bidirectional control and U+2028 by code as it wrote a control (defect 244) | **shared**, built; it also reaches `--dump-tokens`, `--dump-ast` and `--dump-ir`, which share `visible` |
| `fmt --in-place` over a file holding a raw ESC | writes the ESC back byte for byte | refuses, *refusing to format a file with diagnostics*, the file untouched | **shared**, closed by the refusal |
| the runtime's panic and `assert` messages | a `.must()` message and an `assert`'s side holding ESC write it raw to stderr (F11) | **the same**: `fail(code: "e", msg: "m \u{1b}[2J here")` then `.must()` writes `033 [ 2 J` raw | **not this cause**: a value at run time, which can come from a file or an argument as easily as from a literal; the runtime's printer (`failure.c:116`, `:135`, `:145`). To be filed apart, `adjacent` |
| the editor's grammar (`editors/vscode/syntaxes/heroes.tmLanguage.json`) | `\u{1b}` painted `invalid.illegal.unknown-escape` | a pattern `\\u\{[0-9A-Fa-f]{1,6}\}` in the string and the `f` rules, tested with Python's `re` on `\u{1b}`, `\u{202E}` (match), `\u{}`, `\u{1234567}`, `\n` (no match) | owed by the escape, built |
| the site's highlighter (`site/src/lib/highlight.ts`) | `pieceEnd` stepped two bytes past a backslash, so `\u{1b}` in an `f` literal's text opened a hole at its `{` | `escapeEnd` steps past the `}`, as `escape.end_of` does; run with node on `f"\u{1b}[31m{x}"`: the piece ends at the hole's own `{` (13), and `f"a\nb"` at its quote | owed by the escape, built; the site's build (`npm`) unrun |

**Who refreshes `DEFAULT_IGNORABLE`** (read from Unicode 15.0.0): under this
route **the lexer never reads it**. The string's and the comment's refusal
rests on Cc, Bidi_Control and U+2028 and U+2029; whether Unicode keeps the
first and last fixed, as I recall its stability policy says, is the
historian's to read, a question rather than a premise. The table still
judges a group head's name (panel 188), and a refresh widens what `check`
refuses there, so it is judged as *a new checker rule*
(`.claude/rules/verification.md`): every golden tree and the census, the
table's count pinned by its own test (`DEFAULT_IGNORABLE.len() == 34`, which
moves with it). The proposal's list would have made every refresh a change to
what every string may hold.

## 9. What each tool that re-prints a program owes (the brief's item 5)

| tool | what it does with the escape or the refusal | owed |
|---|---|---|
| the formatter, `print/fmt.hero` | prints a string's text as written: `\u{1B}` stays `\u{1B}`; refuses a file with a refused raw character | **canonical digits** (`\u{1B}` to `\u{1b}`), unbuilt, with its dump (below) |
| the formatter's self-check, `cli/syntax_cmds.hero` | compares two dumps, and the dump shows a literal's spelling, so the escape survives both | nothing, until `fmt` rewrites digits; then the dump canonicalises them as `canonical_int` does for `0xFF` |
| `--dump-tokens`, `--dump-ast`, `--dump-ir` | spelling in the first two, the decoded value in the IR, each through `shown_char.visible`, which now writes a bidirectional control by code | built (`shown_char`) |
| `heroes mutate` | no operator reads a string's escapes (`grep -n escape selfhost/mutate/`: none) | nothing |
| `heroes probe`'s reader, `probe/reader.hero` | counts tokens; an escape is inside one | nothing |
| the fixes | every certain fix built here applies and checks clean (section 7); the `\u{1b}` guess of `unknown_escape` bypasses `escape_report.as_text`, which would double its braces in an `f` piece | built |
| the readers of a literal's braces | `lex_interp.piece_text`, `plain_holes.with_f`, `escape_readings.interpolated` each stepped two bytes past a backslash, so `\u{b}` read as a hole `{b}`; each now steps past an escape's `}` (`escape.end_of`) | built |
| `heroes measure` | spec text | the spec-warden's (Q6) |
| the two highlighters | section 8 | built; the site's build unrun |
| `tests/harness/suite_spec.hero` `compiler_escapes` | reads the escapes from `escape_text`'s `if after_backslash == "x"` rows, so it **cannot see** `\u{…}`, which `code_escape.hero` checks, and `spec/colouring` passes whether or not the grammar knows it | **owed, unbuilt**: a row for the escape by code on both sides |

## 10. The cost (measured 01:03 to 01:07)

**Lines**, `git diff --no-index --stat` of my copy against a second
`git archive 4c3524fb` (`<scratchpad>/192-ce-base/`):

- `selfhost/`: **15 files, 776 insertions, 105 deletions**. Of the
  deletions, 54 are `told_by_heads` and its test leaving `head_names.hero`
  and about 26 are `raw_carriage_return` leaving `literals.hero`, both moved,
  not removed.
- `tests/`: 15 files, 150 insertions, 4 deletions: the four new
  `tests/golden/check/` cases with their `.expected` and `.fixed` or
  `.applied`, the fixture `json102/tab.hero` and its README, one row's name in
  `suite_surface.hero`.
- outside the net: `editors/vscode/syntaxes/heroes.tmLanguage.json` +10,
  `site/src/lib/highlight.ts` +15.

**In the layout's unit** (`code_lines`, test blocks and blank lines out; my
replica, the suite's own run below): **the compiler, 78,278 to 78,762, +484,
0.62%**, 412 modules to 415. By module:

| module | base | route | what |
|---|---|---|---|
| `raw_chars.hero` (new) | | 235 | the refusal and its messages; about 22 of it is `raw_carriage_return`, moved |
| `code_escape.hero` (new) | | 83 | the escape's check |
| `heads_told.hero` (new) | | 63 | defect 250's dedup, moved and widened |
| `shown_char.hero` | 222 | 271 | the predicate (Bidi_Control's table), and `visible` widened |
| `escape.hero` | 113 | 183 | the decoder, `end_of`, the UTF-8 encoder |
| `escape_report.hero` | 139 | 160 | what `unknown_escape` says of a code |
| `head_names.hero` | 298 | 269 | `told_by_heads` out |
| `literals.hero` | 296 | 281 | `raw_carriage_return` out, three calls in |
| `lex_interp.hero` | 162 | 166 | one call, one escape step |
| `scan.hero` | 298 | **300** | one call; at its ceiling |
| `cli/compile.hero` | 300 | **300** | `:83` respelled in place |
| `plain_holes.hero`, `escape_readings.hero`, `lexer.hero`, `refused_runs.hero` | | +1, 0, 0, 0 | |

Attributed by piece, a reading of the modules rather than separate builds:
**the refusal about 300** (the new module less the moved CR, the predicate
and writer in `shown_char`, the dedup's widening, the calls), **the escape
about 175**. All of it in the lexer, its diagnostics and the decoder of a
literal: **no line in the checker's types, the lowering, the ownership pass,
the descriptors or the emitter**. The escape is gone once `escape.unescape`
has decoded the literal, which lowering already calls (`ir/flatten.hero:1159`),
so the IR holds the bytes, as it holds a `\n`'s.

**The seed's fixpoint**: two generations, `cmp` silent (section 5), and
again after the last `selfhost/` edit, a test's text in `lexer.hero`: the
seed emitted from the final tree (01:03:47 to 01:05:20) differed from the one
before it only in `#line` numbers and the order of interned strings, and the
compiler built from it emitted it again **byte for byte** (01:05:38 to
01:07:05): sha256 beginning `6311b452460f0db7`, 35,467,773 bytes (the base's
35,206,983, +260,790). My copy's `heroes` is built from it, sha1 beginning
`8ae46c61eda1d799`.

**The compiler's own tests**, the route's compiler judging itself: **1,220
tests, all passed** (00:58 to about 01:02), after one failure at the first
run, which was this route's to answer: the defect 250 test held `\u{41}` as
an escape the language does not have, which it now has. The test keeps its
claim with `\x{41}` and gains the new one, `\u{41}` in a header told by the
lexer as `escape_not_needed`, its fix `A`.

## Resumed 2026-10-05 02:40:14 by `date`, after a second stop at about 01:31

Sections 11 and 12 below record what ran between 01:09 and 01:31, before the
stop; their times are the commands' own.

## 11. The net over the route's tree (01:09:05 to 01:30:44)

All 26 names and forms of `tests/harness/main.hero`, three at a time, each by
the route's compiler judging its own copy (`192-ce-work/net.sh`, outputs in
`192-ce-work/net-route/`), then `cache` alone. A first attempt at 01:07:32
ran nothing (`xargs: command line cannot be assembled, too long`, macOS's
`-I` limit); its folder is kept as `net-route-xargs-failed`.

| suite | result | suite | result |
|---|---|---|---|
| annotations | 697 passed, 0 failed | probe | 27, 0 |
| canonical | 2, 0 | records | **exit 2**: *`git ls-files` could not list this project's files*, an archive copy has no `.git`; not run |
| corpus | 55, 0 | run | 270, 0 |
| descriptors | 363, 0 | runtime | 8, 0 |
| determinism | 302, 0 | spec | 21, 0 |
| emission | 759, 0 | special | 10, 0 |
| fixes | **759, 2 failed** | surface | 355, 0 |
| grammar | 9, 0 | units | 3, 0 |
| layout | 5, 0 | warnings | 331, 0 |
| lines | 271, 0 | wholes | 363, 0 |
| order | 3, 0 | check | **499, 4 failed** |
| ir | 26, 0 | emit | 8, 0 |
| unsupported | 137, 0 | permissive | 6, 0 |
| full | 6, 0 | cache | 7, 0 |

**What moves, every red line read**:
- `check`, four of defect 135's cases (`fixedbugs-135-an-escape-by-a-character-s-code`,
  `-l4-a-character-by-a-code-braced-or-short`, `-l4-a-character-by-its-name`,
  `-l4-a-control-character-by-its-letter`): their `.expected` pin the base's
  sentence, *"a string holds its characters as themselves, and this language
  has no escape for one by its code"*, which Q3 asks to change. Each moved
  line is the new sentence of section 7 and nothing else, but one:
  `\u{1F600}`, the language's own escape now, is `escape_not_needed` with a
  certain fix to `😀`. Owed at landing: the four `.expected` rewritten by
  hand, read line by line.
- `fixes`, two: the same 135 case, which gains that certain fix and so owes
  an `.applied`; and case 244, whose `.applied` held `"a\\<ESC>b"`, a raw ESC
  `check --apply` left in place, and now reads `"a\\\u{1b}b"`: the second
  pass writes the ESC by its code. A better text, owed by hand.
- `records`: not run, for want of git in the copy. **What it would judge**:
  the route moves line numbers in `literals.hero`, `head_names.hero`,
  `lexer.hero`, `escape.hero` and `scan.hero` that documents cite as
  `file:line`, which `records/positions` reads; this brief's own pointers
  (`literals.hero:83`, `head_names.hero:155`, `:191`) are among them. Unrun.
- The four new cases pass `check`, `annotations` and `fixes`; `surface`
  passes with the fixture's tab moved into a comment (section 12).

So **no program, golden or example of the tree is refused anew** by the route
but the fixture whose purpose it was to hold a raw tab in a string; every
other movement is a message this sitting changes on purpose.

## 12. The bidirectional algorithm, run with GNU FriBidi 1.0.17 (01:31)

`fribidi --ltr --nopad --nobreak` over four lines (`192-ce-work/bidi-in.txt`,
`bidi-out.txt`), the logical order against the visual order a screen draws,
each control written here by its code:

| logical | visual |
|---|---|
| `x = 1  # abc<U+202E>def` | `x = 1  # abc<U+202E>fed` |
| `ok = name == "user<U+202E>" \|\| is_admin(x)` | `ok = name == "user<U+202E>(x)nimda_si \|\| "` |
| `if a <U+200F>><U+200F> b` | `if a <U+200F><<U+200F> b` |
| `y = 2  # z<U+2067>w > v` | unchanged |

Three facts, measured:
1. **In a string, an override reverses the code after the string**, and the
   closing quote is drawn at the end of the line, so `|| is_admin(x)` reads
   as text inside the string: Trojan Source's *stretched string*, in this
   language's own syntax.
2. **An implicit mark, RLM, mirrors a neutral**: `a > b` with an RLM on each
   side of `>` is drawn `a < b`. Rust's lint and GCC's warning leave the three
   marks out (the historian's [S1], [S3]); Bidi_Control's twelve take them in,
   which is why the route's predicate is the property and not the nine.
3. **In a comment, an override reverses only the comment's text after it**:
   a `#` comment runs to its line's end, so the code before it on its line
   is not moved. So my comment message, *"so the line can read as another
   program"*, **is false for a comment** and is corrected in section 13; a
   comment's refusal stands on the reader of the comment and of the
   documentation it becomes, and, for U+2028 and the control characters, on
   the line an editor or a terminal draws after it.

## 13. Two corrections, underneath (02:41)

- **Section 6's comment message, corrected in the code.** *"which changes how
  its line is shown, so the line can read as another program"* is false of
  a comment (section 12, fact 3). `raw_chars.why` now takes what holds the
  character: a control, *"which no screen shows and a terminal acts on"*;
  U+2028 or U+2029, *"which a screen may draw as a line break"*; a
  bidirectional control in a comment, *"which reorders the rest of the
  comment as a screen draws it"*; in a string, *"which reorders the rest of
  its line as a screen draws it, so the line can read as another program"*,
  measured. The compiler is rebuilt and the seed regenerated for it (section
  14).
- **Section 7's sentence on route (a′), *"It cannot stand in a `constant`'s
  body or a `match` pattern"*, was written before it was run**; run at 02:41
  on the base (`192-ce-work/a2/`): `constant ESC: str` over
  `ESC_BYTES.validated_bytes().must()` is `constant_body`, *"may not contain
  a call written with `.`"*; a `match` arm over a name bound to the bytes is
  `expected_pattern`. Under route (d), `constant RED: str` over
  `"\u{1b}[31m"` and an arm `"\u{1b}[0m" =>` check at 0, and the program
  prints `033 [ 3 1 m r e s e t`. The sentence stands, now measured.

## 14. What the refusal cost the lexer, and how it was paid back (02:56 to 03:29)

Instructions retired (`/usr/bin/time -l`), never a duration: `heroes lex` of
each of the base's 412 modules in turn, summed, the base's compiler against
the route's, the machine otherwise idle but for my own runs.

| route as built | base | route | |
|---|---|---|---|
| section 5: `raw_chars.past(@l, …)` called for every byte of a string and a comment | 174.11 G | 193.42 G | **+11.1%** |
| a byte test before the call in a string, `clear` (no state) before it in a comment | 174.42 G | 179.47 G | +2.9% |
| the test as a call that also decodes, `stops` | 174.26, 174.16 G | 181.95, 182.00 G | +4.5%, worse, undone |
| the test written out, but read five times in `clear` | 174.24 G | 183.63 G | +5.4%, worse, undone |
| `clear` reading the byte once | 174.12 G | 178.32 G | +2.4% |
| **as it stands**: a printable byte answered in two comparisons in a string and three in a comment, an em dash decoded in `clear` with no call | **174.03, 174.00 G** | **176.82, 176.68 G** | **+1.6%** |

The cause of the first number: an `@` parameter is copied in and out at each
call (spec § 9), and the lexer's state was copied once a byte. **And the
whole check**: `heroes check selfhost/main.hero` over a third copy both
compilers accept (`<scratchpad>/192-ce-meas/`, the base's `selfhost/` with
`compile.hero:83` written by route (a′)), run twice each: base **81.33 G
and 81.35 G**, route **81.83 G and 81.83 G**, **+0.6%**.

Each step was rebuilt and its seed's fixpoint verified by `cmp`; after the
last, the compiler's own tests read **1,220, all passed** (03:23), and the
forms it can move, rerun over the route's tree (03:23 to 03:29), read as
section 11 did: `check` 499 and the same 4 failed, `fixes` 759 and the same
2, `annotations` 697, `surface` 355, `probe` 27, `layout` 5, `canonical` 2,
`order` 3, `lines` 271, each 0 failed. Every case of `q/` reads the exit
codes of section 6, and the four programs' bytes are those of section 6.

**The base, for what moves** (02:52 to 02:54, `192-ce-work/net-base/`, the
base's compiler over the base's copy): `check` 499 passed, `annotations`
693, `fixes` 757, `surface` 355, `layout` 5, each 0 failed. So the route
adds 4 cases to each of the first three, and moves the 4 and 2 named in
section 11.

**The census** (`192-ce-work/census-basefiles.py`, 02:55:38 to 02:56:28):
`check --brief` over the base's 2,014 tracked `.hero` files, the base's
compiler against the route's, three at a time. **Four move**:
`selfhost/main.hero`, 0 to 1, two `invisible_character` at
`cli/compile.hero:83` (what the route respells); `cli/compile.hero` checked
alone, which fails on its `use` lines either way and gains the same two;
`json102/tab.hero`, 0 to 1, `raw_tab` (the fixture the route moves); and
case 135's codes, `\u{1F600}` now `escape_not_needed`. No other program,
golden, example or harness file moves.

## 15. The cost as it stands (03:29), correcting section 10 underneath

Section 10's numbers were the route before section 14's rework; they stand
for that tree. **As it stands**: `selfhost/` **15 files, 821 insertions, 105
deletions**; `tests/` unchanged, 15 files, 150 and 4; `editors/` **+6**
(section 10's *+10* was an estimate, not a count: corrected), `site/` +16,
−1. **The compiler in the layout's unit: 78,278 to 78,798, +520 (0.66%)**,
412 modules to 415: `raw_chars.hero` 263, `literals.hero` 285,
`lex_interp.hero` 170, the rest as in section 10. `scan.hero` and
`cli/compile.hero` stand at 300 of 300; `layout` reads 5 passed, 0 failed.
The seed: sha256 beginning `12062b13f9ec44da`, 35,486,919 bytes, its
fixpoint by `cmp` (03:21:38); my copy's `heroes` sha1 beginning
`744c3197848b79d3`.

## 16. Verdicts

No veto: nothing here is core. Every line is in the lexer, its diagnostics,
the decoder of a literal and the writer of a message; the escape is gone
once `escape.unescape` has decoded it, before the IR, and no line reaches
the checker's types, lowering, ownership, the descriptors or the emitter
(design.md §1.7, Part 5).

### Q1, a string

- `verdict`: **object** to the proposal's list, panel 188's predicate;
  **approve** the narrower refusal, built: a control character (Cc, but the
  line feed; the CR keeps `raw_carriage_return`), Unicode's twelve
  Bidi_Control, U+2028 and U+2029, `invisible_character`; a raw tab,
  `raw_tab`, on one spelling; a NUL, `nul_in_string`, with no spelling.
- `section`: design.md §4.3 (`:994-995`, one spelling; `:1016-1018`, the
  freeze's NUL) and §1.10, which says strings are full UTF-8 and is amended.
  **design.md does not cover a character that reorders a line**; I say so
  rather than borrow a rationale.
- `implementation_cost`: about 300 lines in the layout's unit:
  `selfhost/raw_chars.hero` (new, 263, of which `raw_carriage_return`, 22,
  moved), `shown_char.hero` 222 to 271, `heads_told.hero` (new, 63) against
  `head_names.hero` 298 to 269, one call each in `literals.hero`,
  `lex_interp.hero`, `scan.hero`. +0.6% instructions on a whole `check`.
- `needed_for_self_hosting`: **no**: the compiler writes its controls by
  `[u8]` thirteen times, and `compile.hero:83` is respelled.
- `argument`: Panel 188's predicate was cut for names, which hold no text;
  a string holds text. Run on the route, ZWJ in 👨‍💻, ZWNJ in Persian, VS16
  in ⚠️ and a flag's tags print exactly their bytes; that predicate refuses
  each, and no compiler of five run here refuses any. Controls,
  Bidi_Control and U+2028 are what defects 251 and 283 name: three Unicode
  properties, no judgement table, and the census moves only
  `compile.hero:83` and one fixture. Kept by `--permissive`, as panel 066's
  CR is: Zig and Rust refuse these without the thesis. The residual, ZWSP
  and its kin, stays legal and named.
- `prediction`: at the gate of the batch that lands it, `check
  selfhost/main.hero` retires at most 1% more instructions than on that
  batch's base (+0.6% here) and `code_lines` over `selfhost/` grows by at
  most 600 (+520 here, the owed items below to come).
- `condition`: a measured program, in the tree or a blind arm's output,
  whose string hides a meaning behind a zero-width space or a joiner between
  ASCII letters; then the list widens to Default_Ignorable less Join_Control,
  Variation_Selector and the emoji tags, about fifteen lines.

### Q2, a comment

- `verdict`: **object** to the proposal's list as too narrow; **approve**
  the same predicate as the string's, the tab excepted, a refusal, its fix
  (certain) the character's name, `<U+202E>`.
- `section`: §4.15's canonical form, *"any textual difference between two
  versions is semantic"*, read the other way: two texts that look the same
  must be the same. **design.md does not cover a comment's characters past
  §1.10's *full UTF-8*.**
- `implementation_cost`: one call in `scan.comment` (`scan.hero` 298 to
  300), the rest shared with Q1.
- `needed_for_self_hosting`: **no**.
- `argument`: The proposal omits control characters, which Zig refuses in
  comments (run here) and which reach the terminal of whoever reads the
  file. One predicate for strings and comments moves no tracked file. A
  refusal, not an unbalanced-run rule: FriBidi draws `a <RLM>><RLM> b` as
  `a < b`, from a mark that pairs with nothing. Not showing alone: it never
  reaches an editor. Against it stands UTS #55, *"should not prohibit"*, and
  in a `#` comment an override reverses only the comment's own text, so the
  refusal rests on the reader of comments and documentation: a judgement,
  recorded as one.
- `prediction`: as Q1's.
- `condition`: a measured program whose right-to-left comment needs a raw
  mark; then a comment keeps Cc and U+2028/2029 refused and lets the
  bidirectional controls through, as UTS #55 advises.

### Q3, writing what is refused

- `verdict`: **approve route (d)**, `\u{hex}` for exactly the characters a
  string refuses written as itself, NUL and CR excepted; **object** to (b)
  and (c); (a′) stays a run-time route, not a refusal's spelling; (e)
  refused; (f) not needed.
- `section`: design.md §4.3, `:994-995` and `:1016-1018`, and the panel 066
  paragraph: *"the escape is what made the repair writable"*.
- `implementation_cost`: about 175 lines: `code_escape.hero` (new, 83),
  `escape.hero` 113 to 183, `escape_report.hero` 139 to 160, one line in
  `plain_holes.hero`; owed: `fmt`'s canonical digits and a row of
  `suite_spec.compiler_escapes`.
- `needed_for_self_hosting`: **no**.
- `argument`: The blind arms all read 5 of 5, so writability is not the
  reason. The refusal's certain fix is: it needs a spelling inside the
  literal, as `\r` gave the CR, and (a′) cannot be a fix, a constant's body
  or a pattern (run). Route (d) keeps one spelling per character and the NUL
  out, reuses the code reader `unknown_escape` already had, is decoded
  before the IR, and lands in two seeds' builds, measured. (b) gives every
  character a second spelling; (c) writes half a character. `unknown_escape`
  now points at `\u{1b}`, its fix a guess because `"bin\x86"` reads as a
  code.
- `prediction`: as Q1's.
- `condition`: a blind run in which writers given `\u{...}` fail where
  writers given (a′) pass, or a measured case where a certain `\u{…}` fix
  writes a program meaning something else.

### Q7, the shapes beside

- `verdict`: **approve**, built: every shape whose cause is a character a
  screen cannot show reaching a reader from the source (the character
  literal, the `f` text, the test title, the doc comment, the quoted line in
  a diagnostic and the dumps, `fmt`, both highlighters for the escape).
  **File apart**, `adjacent`: the runtime's panic and `assert` printer
  writes a value's ESC raw, a value that a file brings as easily as a
  literal.
- `section`: §4.17, a message carries what is needed to fix the mistake,
  and a message whose line is reordered carries less.
- `implementation_cost`: inside Q1's, `shown_char.visible` widened (about
  20 of `shown_char`'s 49).
- `needed_for_self_hosting`: **no**.
- `argument`: one predicate, asked by the lexer and by the writer of every
  message, so what the compiler refuses in a source and what it refuses to
  print raw are one list. `DEFAULT_IGNORABLE` stays the group head's alone,
  so a Unicode refresh no longer touches every string.
- `prediction`: as Q1's.
- `condition`: a character a screen cannot show that reaches a reader from
  the source through a path not listed here.

## 17. What the landing owes, built or not

- The four `.expected` of defect 135's cases and two `.applied` (135's code
  case, 244), rewritten by hand and read line by line (section 11).
- The seed regenerated twice: once from the tree with `compile.hero:83`
  raw, once from the final tree (section 5).
- `records/positions` over the line citations the route moves (section 11),
  unrun in an archive.
- `fmt`'s canonical digits for `\u{…}` and its dump; a `suite_spec` row for
  the escape by code; the site's build (`npm`), unrun.
- The spec's sentences, `:35-36` and `:47-48`, priced by the spec-warden.
- Filed apart: the runtime printer of section 8.

Status: complete at 03:30 by `date`.
