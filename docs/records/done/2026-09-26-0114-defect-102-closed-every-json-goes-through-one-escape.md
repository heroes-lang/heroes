# Defect 102 closed: every JSON the compiler writes goes through one escape, and a character the reader cannot see is named by its code

2026-09-26, M-agreed-retention step 18, in lane `b75b908f`, merged `5e691890`.
Found by the skeptic seat over lane g's repair of defects 096 to 101, as a
finding unrelated to that lane.

- [x] **102 — `lex --dump-tokens --json` and `check --json` write JSON that is not JSON when a token or a message holds a tab or a control byte** | both writers escape a backslash, a quote and a newline and nothing else, so a tab inside a string literal, a carriage return, or any byte below 0x20 goes into the output raw, which JSON forbids; and the text diagnostic for a stray control byte prints the byte itself between its backquotes, which the reader cannot see | `selfhost/cli/lex.hero` (`escape`) · `selfhost/cli/check.hero` (`quote`) · `selfhost/scan.hero:224` · **closed 2026-09-26**

    **Origin:** the skeptic seat over lane g's repair of defects 096 to 101,
    2026-09-26, as a finding unrelated to that lane; reproduced by the
    coordinator the same night on the trunk's compiler at `4f52c303`, where
    the seat's citation of `selfhost/cli/compile.hero:82` turned out to be
    the loop of `escape` in `selfhost/cli/lex.hero`.

    **The reproducers.** `print("a<TAB>b")` with a real tab inside the
    literal, a program `check` accepts: `lex --dump-tokens --json` exits 0 and
    its output holds a raw 0x09, *Invalid control character* to Python's
    `json.loads`. A stray 0x01 byte in a body: `lex --dump-tokens --json` and
    `check --json` both write it raw, both invalid JSON, and the text form
    reads `` `<0x01>` is not part of the language's syntax ``. A CRLF file is
    valid JSON, because the carriage return is not inside any token's text.

    **Why it is a defect.** A flag that promises JSON (schema 1, for
    `check`) writes something no JSON reader accepts, at exit 0 for a correct
    program, and the tools that consume it are exactly the ones that cannot
    look at the bytes. `selfhost/cli/count_tokens.hero` already escapes every
    byte below 0x20, so the repair is one escape for all three writers, not a
    third copy, and a diagnostic that names an invisible character by its
    code rather than by itself.

## The repair

- **One escape, three writers.** `selfhost/cli/json_text.hero` holds the one
  function that makes a `str` the inside of a JSON string: the quote and the
  backslash, the five whitespace controls C's own escapes name, and every
  other byte below 0x20 as `\u00XX`, the only form JSON has for it. It was
  `cli/count_tokens.hero`'s, complete, and moved rather than copied;
  `lex --dump-tokens --json` and `check --json` dropped their own, which
  escaped three of the thirty-four characters JSON forbids raw.
- **A character is named so the reader can find it** (`selfhost/shown_char.hero`).
  The lexer's *is not part of the language's syntax* shows a visible ASCII
  character as itself, as it always did; names a control character, below
  0x20 or from 0x7f to 0x9f, by its code alone, *the control character
  U+0001*, since showing it sends the raw byte to the terminal; names a
  character a terminal shows as nothing or that reorders the rest of the line
  (the spaces other than the space, the zero-width characters, the
  bidirectional marks, embeddings, overrides and isolates, the variation
  selectors, the byte order mark, the tag characters) by its code alone, *the
  invisible character U+202E*; and shows any other character above ASCII
  with its code, *`€` (U+20AC)*, which is what an editor's search finds.

## What it does not reach

A byte below 0x20 other than the five whitespace controls cannot be written in
a Heroes literal, whose escape set is frozen, so no unit test can build one: its
`\u0001` is pinned where such a byte can arrive, a file, by the surface suite's
rows over `tests/golden/surface-fixtures/json102/`. And no diagnostic today
carries a tab from the program into its message or its fix, so `check --json`'s
half of the escape is pinned by a unit test on `quote` and by the control byte
arriving there named; a file name with a tab would be the other route, and
Windows refuses one.

## The measurements

| input | before (trunk `8680f5ff`) | after (lane `b75b908f`) |
|---|---|---|
| a real tab inside a string literal, a program `check` accepts | `lex --dump-tokens --json` exit 0, a raw 0x09, *Invalid control character* to Python's `json.loads` | exit 0, valid JSON, the tab as `\t` |
| a stray 0x01 in a body | `lex --dump-tokens --json` and `check --json` both invalid JSON, the byte raw; the text reads `` `<0x01>` is not part of the language's syntax `` | both valid JSON; *the control character U+0001 is not part of the language's syntax* |
| a right-to-left override, U+202E | printed raw between backquotes, reversing the rest of the line | *the invisible character U+202E …* |
| a zero-width space, U+200B | an empty pair of backquotes | *the invisible character U+200B …* |
| DEL, 0x7f | printed raw | *the control character U+007F …* |
| a euro sign | `` `€` is not part of … `` | `` `€` (U+20AC) is not part of … `` |
| a CRLF file | valid JSON | valid JSON, unchanged |

Gate in the lane: the compiler's 726 tests; surface 121, annotations 191,
fixes 25, check 150, unsupported 15, canonical 2, layout 2, order 3, records
24, lines 207; the net's own 167; the seed regenerated and the fixpoint held.
Surface, annotations and check on Linux x86-64 (121, 191, 150), Linux arm64
(121, 191, 150) and the Windows box (114, 191, 150, the seven rows it skips
needing libraries it does not have, none of them these). On the trunk after the
merge `5e691890`: the index equal to the lane's tree, the compiler's 726 tests.
