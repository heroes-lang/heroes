# Panel 192, the llm-ergonomist's blind arm D: route (d)'s words as R8 gives them (`blind/spec-d.diff`)

Run and copied by the coordinator on 2026-10-05, from 16:48:01 to 16:51:35 by the clock, from
`<scratchpad>/192-llm-ergonomist-2/d1` to `d5`, funded by the author's ratification of that day.
Each session ran as a fresh `claude -p` session in its own folder outside any git tree, with the
command of `docs/panel/192-briefs/llm-ergonomist.md` (`--restricted --safe-mode --strict-mcp-config`,
tools `Read` and `Write` only), `--model claude-opus-5-5`, one after another, each capped at the
smaller of 0.25 USD and what was left of 1.0965 less 0.03 (`blind/run-d.sh`). Its brief is
`blind/brief-d.md`, the arms' brief with only the task changed; its specification the base's with
`blind/spec-d.diff`. Scored by `blind/score-d.py`, written before any session ran, its prediction
registered in it, and tested on four programs of known answer before the five were scored.
The model the CLI reports and the cost are `run.json`'s; the scorer's line is quoted.
**Every raw U+200B is shown here as `<U+200B>`**: the files hold the character itself
(`e2 80 8b`), and a document of this repository holds no invisible character (defect 356).

Five sessions, 0.9317 USD by the CLI's report.

## Session d1

- **cost**: 0.1925 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: PASS, é *é raw*, U+200B *U+200B raw*, build exit 0, run exit 0, output `63 61 66 c3 a9 e2 80 8b 21 0a`

### `c.hero`

```
function main()
    print("café<U+200B>!")
```

### `report.md`

# experiment

```
function main()
    print("café<U+200B>!")
```

The string literal holds, raw and in UTF-8: `c` `a` `f`, U+00E9 (bytes C3 A9), U+200B (bytes E2 80 8B), `!`. The zero-width space sits between `é` and `!` and cannot be seen above.

# reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" (section 1) gave the declaration with no `->`.
- "exactly 4 spaces per level" gave the indentation of the body.
- "Syntax is ASCII-only, but comments and strings may hold UTF-8; a string holds no raw control character and is one line" (section 1) told me I could put `é` and U+200B straight into the string.
- "`\u{...}`, a code point in lowercase hex ... for one it holds no other way (`\u{1b}` is ESC)" (section 2) told me that `\u{e9}` and `\u{200b}` are for characters that cannot be written raw. Since both can be written raw, I read the escape as not allowed for them, and wrote them raw.
- "`print` writes its values with no separator and exactly one trailing newline" (section 11) gave one `print` call with one `str`, which ends the line.
- "A literal without the `f` is an error where its braces ... would be a hole" does not apply, because the string has no braces.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone" (section 5): `print` returns `()`, so the call stands alone with no `_ =`.

# confidence

High that it does what it should, moderate that it compiles. The risk is the zero-width space. The spec forbids a "raw control character" in a string and does not define the term. U+200B is general category Cf (format), not Cc (control), so I read it as allowed. A compiler that counts every non-printing or format character as "control" would refuse the raw form. Then it would accept `\u{200b}`, which my reading of "no other way" rules out. The `é` is safe as raw UTF-8. A second, smaller risk: the file must keep U+00E9 precomposed and not normalized to NFD by the editor or filesystem.

# argument

For the most part, yes. The program is one `print` of one string literal, and the spec says plainly that strings may hold UTF-8 and that `print` adds exactly one newline. The weak point is how two sentences fit together. "No raw control character" leaves "control" undefined. "For one it holds no other way" makes `\u{...}` a fallback whose scope depends on that same undefined term. For an invisible character like U+200B, a model has to guess whether the raw form or the escape is the legal one, and the two readings exclude each other. One example of a format character, or a sentence saying which categories count as control, would remove the guess.

# choice_points

1. **`é` raw or `\u{e9}`.** I chose raw. If "no other way" is enforced, `\u{e9}` is a compile error, because `é` can be held raw. If the compiler is lax, it would print the same bytes.
2. **U+200B raw or `\u{200b}`.** I chose raw, reading "control character" as Unicode Cc. If the compiler counts Cf as control, the raw form is refused, and `\u{200b}` would be the only form that compiles. Under my reading, `\u{200b}` is a compile error.
3. **One `print` or several pieces.** `print("caf", "é", ...)` with several arguments would print the same thing, since there is no separator. I chose a single literal for simplicity. Concatenation with `+` would also give the same output.
4. **Plain or `f` literal.** There are no braces, so I chose plain. An `f` literal would print the same.
5. **Hex digit case and leading zeros for the escape**, if the escape were chosen: lowercase with no leading zero (`\u{200b}`, not `\u{200B}` or `\u{0200b}`). Any other form is a compile error.

# prediction

About 55 of 100 one-turn programs compile and do what they should. The split is between models that write raw characters and models that reach for `\u{e9}` / `\u{200b}`. Many models escape invisible characters by habit. Under the "no other way" rule, that escape is refused. A smaller share will fail on NFD `é`, uppercase hex, or `\u{00e9}`.

# context

Only `brief.md` and `spec.md` from this directory, plus the harness's system prompt, which includes an automatically attached user email address that I did not use. I read no other files and used no tools other than reading and writing files here.

## Session d2

- **cost**: 0.1940 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: FAIL, é *é raw*, U+200B *\u{200b}*, *refused under (d): \u{200b}: a string holds U+200B raw (guess fix to the raw character)*

### `c.hero`

```
function main()
    print("café\u{200b}!")
```

### `report.md`

# experiment

```
function main()
    print("café\u{200b}!")
```

The `é` in the file is the single raw character U+00E9 (UTF-8 bytes C3 A9).

# reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" led to the single `function main()` declaration with no `->`.
- "exactly 4 spaces per level" set the body indentation.
- "Syntax is ASCII-only, but comments and strings may hold UTF-8" led to writing `é` raw inside the string.
- "`\u{...}`, a code point in lowercase hex with no leading zero, never 0 or a surrogate, for one it holds no other way" led to `\u{200b}`: lowercase, no leading zero, not a surrogate.
- "a string holds no raw control character" is why I did not paste U+200B raw. I read an invisible character as the kind of thing the escape exists for.
- "`print` writes its values with no separator and exactly one trailing newline" means one `print` of one `str` writes exactly the required bytes plus `\n`.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone" means the `print(...)` call stands alone as a statement.
- The `f` literal rule: a non-`f` literal is an error only where a brace, read as a hole, would name something in scope. `{200b}` names nothing, and it is part of an escape anyway, so the plain literal should be fine.

# confidence

Moderate, about 70 percent. Everything else is trivial. The risk is the one decision about U+200B. The spec says that `\u{...}` is "for one it holds no other way", and that strings refuse only a "raw control character". If the compiler reads "control" as Unicode category Cc, then U+200B (category Cf) can be held raw, and `\u{200b}` might be rejected as an unneeded escape. If the compiler reads it more broadly (Cc plus Cf, or "not visible"), my program is right and the raw form would fail. The `é` written raw is safe under either reading. A smaller risk is that `\u{e9}` might also have been allowed, but I avoided that question.

# argument

The specification makes this program nearly certain except for one sentence. "For one it holds no other way" implies that an escape for a character a string could hold raw is an error. But the only rule saying what a string cannot hold raw is "no raw control character", and the spec never defines that term. A zero-width space sits exactly on that boundary: it is invisible but not a C0/C1 control. So a model has to guess whether U+200B is escapable or must be written raw. The task is built around this ambiguity. Everything else (main, print's single newline, raw UTF-8 for `é`) is stated plainly.

# choice_points

1. **How to write U+200B.** I chose `\u{200b}`. The alternative is the raw character in the string. If the compiler treats Cf as holdable raw, my choice is a compile error and the raw form works. If not, the reverse.
2. **How to write é.** I chose raw U+00E9. The alternative, `\u{e9}`, is likely a compile error, since a string holds `é` raw ("no other way" fails). Writing `e` plus U+0301 would compile but give the wrong output.
3. **One print or several.** I chose one string. Writing `print("café", "\u{200b}", "!")` gives the same output, because `print` adds no separator, and it carries the same escape risk. Several `print` calls would add extra newlines, which is wrong.
4. **Escape case.** I chose lowercase `200b`, as required. `200B` would be a compile error.

# prediction

55 of 100 models produce a program that compiles and prints the right bytes. Most will write `é` raw. They will split on U+200B, with a majority choosing `\u{200b}`, and a few will write `\u{e9}` for `é` or a decomposed `é`. Whichever reading the compiler implements, I expect between 35 and 75 of 100 to succeed.

# context

Only `brief.md` and `spec.md` from this directory reached my context, plus the harness's system prompt and an automatically attached user email address, which I did not use. I read no other files.

## Session d3

- **cost**: 0.1826 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: PASS, é *é raw*, U+200B *U+200B raw*, build exit 0, run exit 0, output `63 61 66 c3 a9 e2 80 8b 21 0a`

### `c.hero`

```
function main()
    print("café<U+200B>!")
```

### `report.md`

# experiment

```
function main()
    print("café<U+200B>!")
```

The string literal holds, as raw UTF-8: `c` `a` `f`, U+00E9 (bytes C3 A9), U+200B (bytes E2 80 8B, invisible here), `!`. The file is written that way in `c.hero`.

# reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" and the `Declaration` production with no `->`: the `function main()` header.
- "exactly 4 spaces per level": the body is indented by four spaces.
- "Syntax is ASCII-only, but comments and strings may hold UTF-8; a string holds no raw control character and is one line": I put `é` and the zero-width space into the string directly. U+200B is a format character (Unicode category Cf), not a control character (Cc), so I read it as allowed raw.
- "`\u{...}`, a code point in lowercase hex ..., for one it holds no other way": both characters can be held raw, so I read `\u{e9}` and `\u{200b}` as forbidden here, and did not use them.
- "`print` writes its values with no separator and exactly one trailing newline, and takes ... `str`": one `print` call with one `str` gives the text and the line ending.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone": `print(...)` returns `()`, so it stands alone with no `_ =`.
- Section 2 on braces: the literal has no braces, so the `f` question does not come up.

# confidence

Fairly high, not certain. The program is two lines and everything except the zero-width space is plain. The risk is whether the compiler counts U+200B as a "raw control character". If it uses a strict Cc definition (or only rejects C0/C1 and DEL), it accepts the raw character and the output is exactly `café`, U+200B, `!`, newline. If it rejects invisible or format characters in literals, compiling fails, and `\u{200b}` was the intended spelling. Another risk is outside the language: the tool that writes the file has to keep the invisible character. I wrote it deliberately, but I cannot read the bytes back to check.

# argument

The spec makes this likely to work for most of the program: `main`, `print` and its newline, and UTF-8 in strings are all stated clearly. The weak spot is the clause "for one it holds no other way" next to "no raw control character". It never says whether an invisible format character like U+200B counts as one the string "holds no other way". A model that reaches for `\u{200b}`, which is the natural move for an invisible character, risks a compile error if the compiler forbids that escape for characters that can appear raw. A model that writes `\u{e9}` probably fails for sure. The rule is short but easy to miss. Its scope is undefined for invisible characters.

# choice_points

1. **`é` raw or `\u{e9}`.** Chose raw. By the "no other way" rule, `\u{e9}` would be a compile error. Writing `e` plus a combining U+0301 would compile but print the wrong character.
2. **Zero-width space raw or `\u{200b}`.** Chose raw, treating U+200B as Cf and not a control character. If the compiler counts U+200B as needing an escape, the raw form fails to compile and `\u{200b}` compiles. Otherwise, `\u{200b}` is the compile error.
3. **One `print` call or several.** Chose one. `print` always ends the line, so splitting the text across calls would print extra newlines. Building the string with `+` from pieces would also work but adds nothing.
4. **Plain literal or `f` literal.** Chose plain. With no braces, the two behave the same.
5. **Hex case and leading zeros in `\u{...}`** (only relevant to the escape route): lowercase with no leading zero is required. `\u{00e9}` or `\u{200B}` would be compile errors.

# prediction

55 of 100 models produce a program that compiles and prints exactly the right bytes. The main loss is models that escape the zero-width space or `é` (the natural move for an invisible character), which the "no other way" rule probably rejects. Some others lose the invisible character when writing the file.

# context

Only `brief.md` and `spec.md` from this directory were read. The system context also held the harness instructions and an automatically attached user email address, neither related to the task. I used no other files or tools and drew on no outside knowledge of Heroes beyond general knowledge of Unicode categories.

## Session d4

- **cost**: 0.1789 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: PASS, é *é raw*, U+200B *U+200B raw*, build exit 0, run exit 0, output `63 61 66 c3 a9 e2 80 8b 21 0a`

### `c.hero`

```
function main()
    print("café<U+200B>!")
```

### `report.md`

# experiment

```
function main()
    print("café<U+200B>!")
```

The string literal holds, as raw UTF-8: `c` `a` `f`, U+00E9 (bytes C3 A9), U+200B (bytes E2 80 8B), `!`. No escapes are used.

# reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" led to `function main()` with no `->`.
- "exactly 4 spaces per level" set the body indent.
- "Syntax is ASCII-only, but comments and strings may hold UTF-8; a string holds no raw control character and is one line" led me to write both characters raw inside the string.
- "`\u{...}`, a code point ... for one it holds no other way (`\u{1b}` is ESC)" led me NOT to use `\u{e9}` or `\u{200b}`: both can be held raw, so I read the escape as refused for them.
- "`print` writes its values with no separator and exactly one trailing newline" covers ending the line.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone" confirms `print(...)` stands alone as a statement.
- No braces in the literal, so the `f`/brace rule does not apply.

# confidence

Moderate to high. The program is two lines and the only risk is lexical: whether the compiler treats U+200B as a "raw control character". By Unicode it is a format character (Cf), not a control character (Cc), so raw should be accepted, and then `\u{200b}` would be the error. If the compiler instead counts invisible or format characters as control characters, my raw ZWSP is refused. A second, smaller risk is that the file is saved with é decomposed; I wrote precomposed U+00E9.

# argument

Mostly yes. The program needs only `main`, `print` and a string literal, all clearly specified, and print's single trailing newline is stated exactly. The weak point is the escape rule: "for one it holds no other way" makes `\u{...}` legal only for characters that cannot appear raw, but "raw control character" is not defined. Whether U+200B is a control character decides between the raw form and `\u{200b}`, and exactly one of them compiles. A model's habit of escaping invisible characters for clarity is punished if Cf counts as holdable raw. Also, a model's output pipeline may silently drop or normalize the invisible character, which the spec cannot help with.

# choice_points

- é: raw U+00E9 (chosen) vs `\u{e9}`. The escape would, by my reading, be a compile error since é is holdable raw.
- ZWSP: raw U+200B (chosen) vs `\u{200b}`. If the compiler counts Cf as control, raw fails to compile and the escape is right; otherwise the escape fails to compile.
- Single literal vs concatenation (`"café" + "\u{200b}" + "!"`) or multiple `print` arguments (`print("café", "<U+200B>", "!")`): all print the same bytes since print uses no separator; I chose one literal for simplicity.
- Plain literal vs `f` literal: no braces, so identical output; plain chosen.

# prediction

55 of 100 models produce a one-turn program that compiles and prints the right bytes. Losses: models writing `\u{200b}` or `\u{e9}` (refused under the "no other way" rule), and models whose emitted raw ZWSP is lost or whose é is decomposed.

# context

Only `brief.md` and `spec.md` from this directory, plus the harness's system context (environment details and an account email, unused). No other files or tools beyond reading and writing here.

## Session d5

- **cost**: 0.1837 USD; **models**: claude-haiku-4-5-20251001, claude-opus-5-5; **ended**: `success`, 5 turns
- **scorer**: PASS, é *é raw*, U+200B *U+200B raw*, build exit 0, run exit 0, output `63 61 66 c3 a9 e2 80 8b 21 0a`

### `c.hero`

```
function main()
    print("café<U+200B>!")
```

### `report.md`

# experiment

```
function main()
    print("café<U+200B>!")
```

The string literal holds, as raw UTF-8: `c`, `a`, `f`, U+00E9 (bytes C3 A9), U+200B (bytes E2 80 8B), `!`. The zero-width space sits invisibly between `é` and `!`.

# reading

- "the file you compile holds `function main()`, which takes nothing and produces nothing" (section 1): the single declaration `function main()` with no `->`.
- "exactly 4 spaces per level" (section 1): the body is indented by four spaces.
- "Syntax is ASCII-only, but comments and strings may hold UTF-8; a string holds no raw control character and is one line" (section 1): I wrote `é` and U+200B raw inside the string. U+200B is a format character (Unicode Cf), not a control character (Cc), so I read it as allowed raw.
- "`\u{...}` ... for one it holds no other way" (section 2): I read this as a restriction. `\u{...}` is only for code points a string cannot hold raw, such as ESC. Both of my characters can be held raw, so I took `\u{e9}` and `\u{200b}` to be errors, or at least not the intended form.
- "A literal without the `f` is an error where its braces ... would be a hole" (section 2): there are no braces, so a plain literal is fine.
- "`print` writes its values with no separator and exactly one trailing newline" (section 11): one `print` of one `str` produces the text and a single line end, and nothing else.
- "A line that computes a value must use it ... which a `()` line refuses: it stands alone" (section 5): `print` returns `()`, so it stands alone as a statement with no `_ =`.

# confidence

Moderate to high. The structure is trivial and covered directly by the spec. The risk is all in the string literal. (1) If "for one it holds no other way" only means "for example, for things you cannot type" and the compiler actually rejects raw U+200B (some lexers reject invisible or format characters), my program fails to compile. (2) The program depends on the file really holding precomposed U+00E9 and the ZWSP bytes. A tool or editor that normalizes to NFD or strips the ZWSP would silently change the output. I wrote the bytes deliberately, but I cannot check them by running anything.

# argument

The spec makes a one-turn program likely: `main`, indentation, and `print`'s single trailing newline are stated plainly. The weak point is the escape rule. "for one it holds no other way" is a terse clause that could mean `\u{...}` is forbidden for printable characters, or could just explain why it exists. It also does not say whether a raw U+200B counts as holdable. "No raw control character" leaves format characters such as U+200B, U+200E, and U+FEFF undefined. A model that reaches for `\u{200b}`, the natural choice for an invisible character, may hit a compile error. A model that writes it raw risks losing it in transmission. Neither path is clearly blessed.

# choice_points

- Raw U+200B vs `\u{200b}`: I chose raw. If the compiler enforces "no other way" strictly, `\u{200b}` would be a compile error. If it instead treats U+200B as unholdable raw, my raw form would be the error and `\u{200b}` would compile and print correctly.
- Raw `é` vs `\u{e9}`: I chose raw. Under the strict reading, `\u{e9}` fails to compile. Under a loose reading it prints the same bytes.
- Decomposed `e` + U+0301 vs precomposed U+00E9: I chose precomposed. The other would compile but print the wrong bytes.
- One `print` of a whole string vs several arguments (`print("café", "\u{200b}", "!")`): I chose one. Several arguments print the same output, since there is no separator, but they still depend on the escape question.
- `print` vs `write_file` to stdout: `print` is the only stdout writer, so this was no real choice.

# prediction

About 60 of 100 models produce a program that compiles and does what it should. Most of the failures would come from writing `\u{e9}` or `\u{200b}` and having one rejected under the "no other way" rule, or from the ZWSP being dropped or the `é` being decomposed in the emitted file.

# context

Only `brief.md` and `spec.md` from this directory reached my context, plus the harness's system prompt and an automatically attached account email that I did not use. I read no other files.

