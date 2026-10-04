# Panel 192, the llm-ergonomist's blind arm B: B, route (b): `\u{...}` (`blind/spec-b.diff`)

Copied by the coordinator from `<scratchpad>/192-llm-ergonomist-2/`. Each session ran as a fresh
`claude -p` session in its own folder outside any git tree, with the command of
`docs/panel/192-briefs/llm-ergonomist.md` (`--restricted --safe-mode --strict-mcp-config`, tools
`Read` and `Write` only), its cap `--max-budget-usd 0.24` for the trial `a1` and `0.25` for the
rest (see `llm-ergonomist.md`). Scored by `<scratchpad>/192-blind-score.py`, written before
any session ran. The model the CLI reports and the cost are `run.json`'s; the scorer's line
is quoted whole.

## Session b1

- **cost**: 0.1742 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *\u{1b}*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*

### `c.hero`

```
function main()
    print("\u{1b}[31mERROR\u{1b}[0m")
```

### `report.md`

# experiment

```
function main()
    print("\u{1b}[31mERROR\u{1b}[0m")
```

#### reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" (section 1): the single declaration `function main()` with no `->`.
- "exactly 4 spaces per level" (section 1): the body is indented by four spaces.
- "Seven escapes, and no others: ... `\u{...}`, a code point in hex that is not 0 or a surrogate" (section 2): ESC (0x1b) has no named escape, so it is written `\u{1b}`. 0x1b is neither 0 nor a surrogate.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole naming something ... in scope" (section 2): the only braces in the string belong to the `\u{...}` escape, and `1b` names nothing in scope, so a plain literal is used rather than an `f` literal.
- "`print` writes its values with no separator and exactly one trailing newline, and takes ... `str`" (section 11): one `print` call writes the whole sequence and ends the line, so no `\n` is added.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone" (section 5): `print` returns `()`, so the call stands alone without `_ =`.

#### confidence

High. The program uses only `main`, one `print` and one string literal. The one non-obvious need, the ESC byte, is covered by the `\u{...}` escape, which the specification states explicitly with its constraints. The residual risk is the brace rule for plain literals: if the compiler scanned `\u{1b}` as a potential hole before handling the escape, it would still not name anything in scope, so the rule does not fire. Output is ESC[31m, ERROR, ESC[0m, newline, which an ANSI terminal shows as red `ERROR` followed by a reset.

#### argument

Yes, largely. The task needs four facts: the shape of `main`, indentation, how to write a byte with no named escape, and that `print` ends the line. Each is stated in one place and in plain words. The `\u{...}` escape is the crux, and its rule (hex, not 0, not a surrogate) leaves no doubt that `\u{1b}` is legal. The only friction is the brace rule for non-`f` literals: a reader may briefly wonder whether `\u{1b}` collides with interpolation. The rule's wording ("a hole naming something ... in scope") resolves it, but a model that skims might reach for `f"..."` and then need `{{`, or add a stray `\n`, producing a blank line.

#### choice_points

- Reset code: `ESC[0m` (reset all attributes) versus `ESC[39m` (default foreground only). Both return the colour to the terminal default; `39m` would leave other attributes untouched, of which there are none here. Output looks identical.
- Red code: `31` (normal red) versus `91` (bright red). `91` would print a brighter red, still red but less conventional.
- Escape spelling: `\u{1b}` versus `\u{1B}` or `\u{001b}`. The specification says only "in hex"; I chose lowercase, minimal digits. Other spellings would presumably compile identically, though case and leading zeros are not explicitly confirmed.
- One `print` with one string versus several arguments (`print(esc, "[31m", ...)`) or a constant `ESC`. Same output; more surface for error.
- Plain literal versus `f` literal: an `f` literal would also work since no braces form a hole, but adds nothing.
- Not appending `\n`: adding it would print an extra blank line, since `print` already ends the line.

#### prediction

90 of 100 one-turn programs compile and do what they should. Expected failures: a trailing `\n` giving a blank line, an unsupported escape such as `\x1b` or `\e` or `\033` (compile error), or a mistaken `_ = print(...)`.

#### context

Only `brief.md` and `spec.md` from this directory were read. The system context also supplied the user's email address and environment details (working directory, platform, date), none of which bear on the program.

## Session b2

- **cost**: 0.1789 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *\u{1b}*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*

### `c.hero`

```
function main()
    print("\u{1b}[31mERROR\u{1b}[0m")
```

### `report.md`

# experiment

```
function main()
    print("\u{1b}[31mERROR\u{1b}[0m")
```

#### reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" (section 1) and the `Declaration` production with an optional `-> Type` led to `function main()` with no arrow.
- "exactly 4 spaces per level" (section 1) led to the four-space indent of the body.
- "Seven escapes, and no others: ... `\u{...}`, a code point in hex that is not 0 or a surrogate" (section 2) led to writing ESC as `\u{1b}`. It is the only way to put byte 0x1B in a string; there is no `\e` or `\x1b`, and "Any other escape is a compile error" ruled those out.
- "`print` writes its values with no separator and exactly one trailing newline, and takes ... `str`" (section 11) led to a single `print` call that also ends the line, with no `\n` written.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone" (section 5) confirmed that `print(...)`, which returns `()`, stands as a bare statement with no `_ =`.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole naming something ... in scope" (section 2): the only braces are the escape's own, and `1b` names nothing (an identifier cannot start with a digit), so the plain literal is legal.

#### confidence

High. The program is one statement using only `print`, a string literal and a documented escape. The one residual risk is how the lexer orders the `\u{...}` escape against the brace-hole check of section 2; the spec lists `\u{...}` as an escape inside strings, so I expect the escape to be consumed first. At run time the bytes are ESC `[31m` `ERROR` ESC `[0m` newline, which is red ERROR, reset, end of line on an ANSI terminal.

#### argument

Yes, likely. The spec names `\u{...}` explicitly with an example and forbids every other escape, so the only real hazard, writing `\x1b`, `\e` or `\033` from habit in other languages, is called out as a compile error a careful reader avoids. `print` adding exactly one newline is stated, so no double newline. `main` with no arrow is stated. The weak spot is the rule about plain literals whose braces would be holes: it does not say outright that braces inside a `\u{...}` escape are exempt, so a model could worry and reach for an `f` literal or a constant, which still works. A model skimming may write `\x1b`.

#### choice_points

- Reset sequence: chose `ESC[0m` (reset all attributes). `ESC[39m` resets only the foreground to default; on this output both look identical, since only the colour was set.
- One string versus pieces: chose one literal. `print("\u{1b}[31m", "ERROR", "\u{1b}[0m")` prints the same bytes, as `print` uses no separator; `+` concatenation also gives the same.
- Hex case and padding in the escape: chose `1b`. `\u{1B}` or `\u{001b}` presumably produce the same code point; the spec does not say whether case or leading zeros are allowed, so lower case and minimal digits are safest.
- Newline: relied on `print`'s trailing newline. Adding `\n` would print an extra blank line.
- Constant for ESC: chose inline. A `constant ESC: str` with body `"\u{1b}"` would work too but adds surface for error (constant body rules).
- Plain versus `f` literal: chose plain. An `f` literal would need `\u{1b}` to survive hole parsing, which is less clear.

#### prediction

85 of 100 models produce a program that compiles and prints red ERROR, reset, newline. Failures mostly from `\x1b`, `\033` or `\e` escapes, a stray `\n` giving an extra line, or a `->` on main.

#### context

Only `brief.md` and `spec.md` from this directory, plus the harness's system prompt and an automatically attached user email address, which played no part in the program.

## Session b3

- **cost**: 0.1766 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *\u{1b}*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*

### `c.hero`

```
function main()
    print("\u{1b}[31mERROR\u{1b}[0m")
```

### `report.md`

# experiment

```
function main()
    print("\u{1b}[31mERROR\u{1b}[0m")
```

#### reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" (section 1): the single declaration `function main()` with no `->`.
- "exactly 4 spaces per level" (section 1): the body is indented four spaces.
- "Seven escapes, and no others: ... `\u{...}`, a code point in hex that is not 0 or a surrogate" (section 2): ESC (0x1b) is written `\u{1b}`. There is no `\e` or `\x1b`, and the sentence says any other escape is a compile error, so `\u{1b}` is the only way to put ESC in a string literal.
- "strings any but a raw carriage return or line end" (section 1): the whole sequence sits on one line.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole naming something and only what is in scope there" (section 2): the braces in `\u{1b}` belong to the escape, and `1b` names nothing in scope anyway, so the plain literal is legal without `f` or doubled braces.
- "`print` writes its values with no separator and exactly one trailing newline, and takes ... `str`" (section 11): one `print` of one `str` writes the bytes and ends the line, so no `\n` is needed.
- "`()` line ... stands alone" (section 5): `print(...)` returns `()`, so it stands as a statement with no `_ =`.

#### confidence

High. The program uses one declaration, one built-in call and one escape form, and the spec states each one explicitly. The only risk I can see is whether the lexer reads `\u{1b}` correctly before it checks for interpolation holes. Section 2 lists it as an escape, and `1b` is not an identifier, so even a hole check run first would find nothing in scope. Output bytes: ESC `[31m` `ERROR` ESC `[0m` newline.

#### argument

Yes. Section 2 lists every escape, so it is clear at once that ESC has to be `\u{1b}`, and it rules out the usual guesses `\e`, `\x1b` and `\033` by making any other escape a compile error. Section 11 says `print` adds exactly one newline, which removes the risk of a doubled line end. The one weak spot is the rule against braces in non-`f` literals. A model may wonder whether `\u{1b}` trips it and add an `f` or `{{`, either of which would break the escape. The spec never says outright that escape braces are exempt. That follows only from the "naming something in scope" wording.

#### choice_points

- Reset code: I chose `\u{1b}[0m` (reset all attributes). The alternative `\u{1b}[39m` resets only the foreground to the default. On this output both look the same.
- Red code: I chose `31` (standard red). `91` (bright red) would print a lighter red, and `38;5;1` or truecolour would also show red on capable terminals.
- Line end: I relied on `print`'s own newline. Adding `\n` would print an extra blank line.
- One `print` or concatenation: I chose one literal. `print("\u{1b}[31m", "ERROR", "\u{1b}[0m")` gives the same bytes, because print uses no separator. Making ESC a `constant ESC: str` would also work but adds a declaration.
- `f` prefix: I did not use one. Writing `f"\u{1b}..."` could make the lexer treat `{1b}` as a hole, which would probably be a compile error.

#### prediction

90 of 100 models produce a one-turn program that compiles and prints red `ERROR`, resets the colour and ends the line. I expect the failures to come mostly from `\x1b`, `\e` or `\033` escapes, from an extra `\n`, or from 8-space or tab indentation.

#### context

Only `brief.md` and `spec.md` from this directory reached my context, plus the harness's system prompt (tool descriptions, environment details and the user's account email). I read no other files and used no other tools.

## Session b4

- **cost**: 0.1738 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *\u{1b}*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 33 39 6d 0a`, *pass*

### `c.hero`

```
function main()
    print("\u{1b}[31mERROR\u{1b}[39m")
```

### `report.md`

# experiment

```
function main()
    print("\u{1b}[31mERROR\u{1b}[39m")
```

#### reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" gave the only declaration.
- "exactly 4 spaces per level" gave the body indentation.
- "Seven escapes, and no others: ... `\u{...}`, a code point in hex that is not 0 or a surrogate" gave `\u{1b}` for ESC (0x1b). It is the only way to write ESC, since `\e`, `\x1b` and `\033` are excluded by "Any other escape is a compile error".
- "`print` writes its values with no separator and exactly one trailing newline" gave the line end with no explicit `\n`.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone" told me a bare `print(...)` call is a valid statement.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole naming something ... in scope" made me check that `{1b}` names nothing (it is not an identifier or valid expression), so the plain literal is allowed.

#### confidence

High. The program is one call with one string. The one real risk is the braces rule: the spec does not say whether `\u{...}` is read as an escape before brace checking. Read literally, `{1b}` names nothing in scope, so it should not be an error, and `\u{...}` is listed as an escape *in a string*, so it must be meant to work in plain literals. Output: ESC[31m, ERROR, ESC[39m, newline.

#### argument

Yes, it was likely. The spec lists every escape, so ESC can only be written as `\u{1b}`. It also says print adds exactly one newline. What it left open is how the `\u{...}` braces interact with the braces-as-hole rule for non-`f` literals. A careful reader can settle that, but the spec states no order between escape lexing and the hole check. The spec gives no example of a non-ASCII control code. A model trained on C or Python will reach for `\x1b`, `\033` or `\e` first. Each of those is a compile error here, and only a model that reads section 2 closely avoids it.

#### choice_points

- Reset code: I chose `ESC[39m` (default foreground). `ESC[0m` resets all attributes. Both make the colour the terminal default and print the same text.
- One string vs. several print arguments: one string. Writing `print("\u{1b}[31m", "ERROR", "\u{1b}[39m")` gives the same bytes, because print uses no separator.
- Newline: I let print supply it. Adding `\n` would print an extra blank line.
- Red code: `31` (standard red). `91` (bright red) would show a different shade.
- Hex case: `1b` and `1B` are presumably equivalent. The spec shows only lowercase.

#### prediction

80 of 100 models produce a program that compiles and prints correctly. Most failures will come from using `\x1b`, `\033` or `\e`, or from adding a redundant `\n`.

#### context

Only `brief.md` and `spec.md` from this directory, plus the harness system prompt (which includes the user's email address, unused here). No other files or sources.

## Session b5x

- **cost**: 0.1782 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: pass `True`, spelling *\u{1b}*, build exit 0, output `1b 5b 33 31 6d 45 52 52 4f 52 1b 5b 30 6d 0a`, *pass*
- **stopped**: b5 wrote nothing, stopped by the account's limit; this is its re-run

### `c.hero`

```
function main()
    print("\u{1b}[31mERROR\u{1b}[0m")
```

### `report.md`

# experiment

```
function main()
    print("\u{1b}[31mERROR\u{1b}[0m")
```

#### reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" (section 1): the single declaration `function main()` with no `->`.
- "exactly 4 spaces per level" (section 1): the body is indented by four spaces.
- "Seven escapes, and no others: ... `\u{...}`, a code point in hex that is not 0 or a surrogate" (section 2): ESC (0x1B) has no named escape, so it is written `\u{1b}`. This is the only way to get a control character into a string, and 0x1B is neither 0 nor a surrogate.
- "`print` writes its values with no separator and exactly one trailing newline, and takes ... `str`" (section 11): one `print` call with one `str` writes the sequence and ends the line, so no `\n` is needed.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone" (section 5): `print` returns `()`, so the call stands alone with no `_ =`.
- "A literal without the `f` is an error where its braces, read with an `f`, would be a hole naming something ... in scope" (section 2): checked against `{1b}`. Inside `\u{...}` the braces belong to the escape, and `1b` is not a name in scope anyway, so a plain literal is correct.

#### confidence

High. The program is one call with one string literal. Its only risk is the escape: `\u{1b}` is explicitly allowed (hex, not 0, not a surrogate). The brace rule for non-`f` literals cannot fire, because `1b` names nothing and starts with a digit. `print` adds the newline. A terminal following ANSI shows `ERROR` in red, then `ESC[0m` sets attributes, foreground colour included, back to default.

#### argument

Yes, the specification made a one-turn program likely. The task depends on three facts: how to write a byte with no named escape, what `print` appends, and what `main` looks like. Each is stated plainly in one sentence. The `\u{...}` rule names its exclusions (0, surrogates), so it is easy to check that ESC is allowed. The weak spot is the brace rule for non-`f` literals: it does not say whether braces inside a `\u{...}` escape count as a potential hole. A careful reader may hesitate or reach for `f"..."` and doubled braces. Here `1b` names nothing, so the question does not come up in practice. Nothing about ANSI is in the specification, and nothing needs to be.

#### choice_points

- Reset sequence: I chose `ESC[0m` (reset all attributes). The alternative `ESC[39m` resets only the foreground to default. Both produce the same visible result here; 39 is narrower, 0 also clears any other attributes.
- Hex case and width in `\u{...}`: `1b` versus `1B` or `001b`. The specification says "in hex" without fixing the case or a digit count. I chose lowercase, as in its `\u{e9}` example. Uppercase or padded forms would probably also compile, but the specification does not say so.
- One `print` with one string versus several arguments (`print("\u{1b}[31m", "ERROR", "\u{1b}[0m")`): `print` takes several values with no separator, so the output is identical. I chose one literal.
- Newline: rely on `print`'s trailing newline instead of adding `\n`. Adding `\n` would print an extra blank line.
- `f` literal versus plain literal: a plain literal. An `f` literal would also work, unless its escape braces were read as a hole, which the specification does not settle.

#### prediction

About 85 of 100 models produce a one-turn program that compiles and does what it should. The failures I expect: writing `\x1b` or `\033` or `\e` out of C/Python habit (forbidden escapes, a compile error), adding a `\n` that doubles the newline, or wrapping `print` in `_ =`.

#### context

Only this directory's files: `brief.md` and `spec.md`. The system context also held the session's environment details and a user email address. I did not use them, and they did not affect the program.

