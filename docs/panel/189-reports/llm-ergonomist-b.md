# Panel 189, the llm-ergonomist's blind arm B

Run by the coordinator on 2026-10-04 between 00:12:35 and 00:15:12 by the run log, four fresh `claude -p` sessions from `<scratchpad>/189-llm-ergonomist/b1/` to `b4/`, outside any git tree (every parent walked), each folder holding `spec.md` (the specification at `7d9f2e8f`, `cmp`), `p.hero` (the program's own bytes, `cmp` against `189-blind-src/p.hero`) and `brief.md` (`docs/panel/189-briefs/blind/b-brief.md`): the draft refusal at exit 1 with the code `unexpected_character`. The command of `docs/panel/189-briefs/llm-ergonomist.md`, `--max-budget-usd 0.25`. Each `c.hero` built by the coordinator with the trunk's compiler (sha256 `958320f39dee0b9e`) and run.

| session | model (CLI) | cost USD | turns | `c.hero` built | it printed | one-turn repair |
|---|---|---|---|---|---|---|
| b1 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1757 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| b2 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1738 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| b3 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1791 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| b4 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1742 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |

## Session b1

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` held the `é` as the single Latin-1 byte 0xE9, which is not valid UTF-8, and a `.hero` file must be UTF-8 text.

**reading**: The text "the byte 0xE9 at 2:15 is not UTF-8, and a `.hero` file is UTF-8 text" told me the exact byte, its position (the character after `caf` inside the string), and the rule broken. 0xE9 is `é` in Latin-1, which matches the intended word `café`. So the edit was to re-encode that one character as UTF-8 (0xC3 0xA9), leaving the rest of the program unchanged. The caret under column 15 and the replacement character `�` in the echoed line confirmed it was the `é` and nothing else.

**argument**: Yes. The message names the offending byte value, its line and column, and the rule ("a `.hero` file is UTF-8 text"), which together state both the problem and the repair: save the character as UTF-8. Recognising 0xE9 as Latin-1 `é` is common knowledge, and the stated intent `café` removes any doubt. The weak spot is that the fix is an encoding change, not a visible text change: a model that rewrites the line "as it looks" succeeds only if its tooling writes UTF-8, and the message gives no hint about tooling. It also does not mention that the spec has no `\u` escape, so a model might try one and fail; the message could suggest "re-save as UTF-8".

**prediction**: About 90 of 100 models produce a one-turn repair that compiles and prints `café`. Failures would come from attempting a `é` or `\xe9` escape (refused by section 2), choosing a decomposed form, or tooling that writes non-UTF-8 bytes.

**context**: Only this directory's files (`brief.md`, `spec.md`, `p.hero`) reached my context, plus the harness's standard system context (environment details and an account email), none of which bore on the repair.

## Session b2

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` was saved in Latin-1 (or Windows-1252), so the `é` in the string was the single byte 0xE9. That byte is not valid UTF-8, and the spec (section 1) requires UTF-8 strings.

**reading**: The message `the byte 0xE9 at 2:15 is not UTF-8, and a .hero file is UTF-8 text` gave both the byte and its position. 0xE9 is `é` in Latin-1, and 2:15 is the fourth character of `caf�`. So the edit was to re-encode that one character as UTF-8 and change nothing else. The caret under the replacement character confirmed the column.

**argument**: Yes, a one-turn repair was likely. The message names the exact byte, its line and column, the rule it breaks, and the encoding the file needs. 0xE9 is well known as Latin-1 `é`, and the intended word `café` is given, so the fix follows directly. One weak spot: the snippet shows U+FFFD in place of the byte, so a model working only from that text cannot see the original character. Another: the message says "not UTF-8" without naming Latin-1. Both are minor. The bigger risk is environmental. A model that edits with a byte-preserving tool, or that copies the corrupted text, could write the bad byte or U+FFFD back into the file.

**prediction**: 90 of 100 models produce a correct one-turn repair. The failures I expect are a few that write U+FFFD or keep the 0xE9 byte through their tooling, a few that try a `\u` escape, and a rare one that uses the decomposed form.

**context**: Nothing beyond this directory's `brief.md`, `spec.md` and `p.hero` reached my context, apart from the harness's system context (environment details and the user's account email), which played no part in the repair.

## Session b3

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` was saved in Latin-1 (or Windows-1252), so the `é` in the string literal was the lone byte 0xE9, which is not valid UTF-8, and the spec requires a `.hero` file to be UTF-8 text.

**reading**: The phrase "the byte 0xE9 at 2:15 is not UTF-8, and a `.hero` file is UTF-8 text" told me two things. First, the program text was right and only the encoding was wrong. Second, 0xE9 is `é` in Latin-1, which matches the intended word `café`. So my edit was to write the same characters again with `é` encoded as UTF-8. The caret under column 15, inside the string, plus spec section 1 ("strings any [UTF-8] but a raw carriage return or line end"), confirmed that a non-ASCII character is allowed inside a string literal. That meant no escape or workaround was needed. The spec has no `\u` or `\x` escape anyway (section 2 lists six escapes and says any other is an error).

**argument**: Yes. The message names the offending byte in hex, gives its exact position, states the rule ("a `.hero` file is UTF-8 text"), and the caret lands inside the string literal. A model that knows 0xE9 is Latin-1 `é` sees at once that it only needs to re-encode. Two weak spots. First, the rendered source line shows a replacement character, so the message never says which character was meant. The brief supplies that ("café"), not the compiler. Second, it does not say a UTF-8 `é` is allowed in a string. A cautious model might wrongly reach for an escape or an ASCII spelling, and the spec has no escape that could produce `é`.

**prediction**: About 92 of 100 models repair it in one turn. The failures would come mostly from models that spell it `cafe`, invent a `é` or `\xe9` escape, or use a decomposed `é`.

**context**: Nothing beyond this directory's files (`brief.md`, `spec.md`, `p.hero`) reached my context, apart from the harness's system prompt and an automatically attached user email address, which played no part in the repair.

## Session b4

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` was saved in Latin-1 (or Windows-1252), so the `é` in the string was the single byte 0xE9, and that is not valid UTF-8.

**reading**: The headline `the byte 0xE9 at 2:15 is not UTF-8, and a .hero file is UTF-8 text` told me what to change: the encoding of that one character. It did not tell me to change the program's logic. 0xE9 is `é` in Latin-1, and the expected output `café` agrees with that. The caret under column 15, inside the string literal, confirmed that only the fourth character of `"caf?"` had to change. So I re-encoded that character as UTF-8 and left the source otherwise the same. Spec section 1 ("strings any [UTF-8] but a raw carriage return or line end") confirms that a non-ASCII character is allowed inside a string literal.

**argument**: Yes. The message names the exact byte, its position, the rule it breaks (UTF-8 required) and the file-level cause. A model that knows 0xE9 is Latin-1 `é` repairs this in one turn. The weak point is that the snippet shows a replacement glyph `?`, so the model never sees the intended character. It has to infer it from the byte value or from the task description. The message also does not say "re-save as UTF-8" or name the likely source encoding (Latin-1). A model might reach for an escape such as `é` or `\xe9`, which spec section 2 forbids, because only six escapes exist. Here the stated goal, `café`, removes most of the doubt.

**prediction**: About 85 of 100 models repair this in one turn. The failures I expect: using a forbidden `é` or `\xe9` escape (about 8), using the decomposed form or some other non-matching bytes (about 3), and tooling that writes a non-UTF-8 file or changes the output text, for example to `cafe` (about 4).

**context**: Only this directory's files reached my context: `brief.md`, `spec.md` and `p.hero`. The harness also attached a system note with the user's email address and environment details, and I did not use them. No other files or tools were used.

