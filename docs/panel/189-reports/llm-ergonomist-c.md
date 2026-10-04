# Panel 189, the llm-ergonomist's blind arm C

Run by the coordinator on 2026-10-04 between 00:12:35 and 00:15:12 by the run log, four fresh `claude -p` sessions from `<scratchpad>/189-llm-ergonomist/c1/` to `c4/`, outside any git tree (every parent walked), each folder holding `spec.md` (the specification at `7d9f2e8f`, `cmp`), `p.hero` (the program's own bytes, `cmp` against `189-blind-src/p.hero`) and `brief.md` (`docs/panel/189-briefs/blind/c-brief.md`): the draft refusal at exit 1 with the code `not_text`. The command of `docs/panel/189-briefs/llm-ergonomist.md`, `--max-budget-usd 0.25`. Each `c.hero` built by the coordinator with the trunk's compiler (sha256 `958320f39dee0b9e`) and run.

| session | model (CLI) | cost USD | turns | `c.hero` built | it printed | one-turn repair |
|---|---|---|---|---|---|---|
| c1 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1629 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| c2 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1665 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| c3 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1719 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| c4 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1662 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |

## Session c1

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` encodes `é` as the single Latin-1 byte 0xE9, which is not valid UTF-8, so the file is not the UTF-8 text the language requires.

**reading**: "the byte 0xE9 at 2:15 is not UTF-8, and a `.hero` file is UTF-8 text" told me the problem is the file's encoding and not the program's logic: 0xE9 is `é` in Latin-1, so I rewrote the same line with `é` encoded as UTF-8. The caret at column 15 and the replacement character in the echoed line `print("caf�")` confirmed that the bad byte is the one meant to be `é` in `café`.

**argument**: Yes. The message names the byte, its position, the rule it breaks (a `.hero` file is UTF-8), and the caret points at it. A model that knows 0xE9 is Latin-1 `é` repairs it at once, and the "what it should do" (print `café`) removes any doubt about the intended character. One weak spot: the message does not say "re-save as UTF-8" or name Latin-1, so a model that only edits text, and cannot see bytes, might copy the line back unchanged and think it already fixed it. Another is that the spec has no `\u` or `\x` escape, so the fix cannot be done in ASCII alone.

**prediction**: 90 of 100 models produce a one-turn repair that compiles and prints `café`. Failures would come mostly from tooling that re-writes the file in a non-UTF-8 encoding, or from models trying a `é` escape the spec forbids.

**context**: Only the files in this directory reached my context: `brief.md`, `spec.md` and `p.hero`, plus the harness's system prompt and an automatically attached user email address, neither of which bore on the task.

## Session c2

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` encoded the `é` of `café` as the single Latin-1 byte 0xE9, which is not valid UTF-8, and a `.hero` file must be UTF-8 text.

**reading**: `the byte 0xE9 at 2:15 is not UTF-8, and a .hero file is UTF-8 text` named the exact byte, its position (line 2, column 15, the character after `caf`), and the rule it broke. 0xE9 is `é` in Latin-1, which matches the intended word `café`, so the edit was to re-encode that one character as UTF-8 and leave everything else unchanged. Section 1 of the spec ("strings any [UTF-8] but a raw carriage return or line end") confirmed that a UTF-8 `é` inside a string literal is legal, and section 2 confirmed there is no `\u` or `\x` escape to use instead.

**argument**: Yes. The message names the offending byte in hex, its line and column, and the rule ("a .hero file is UTF-8 text"). A model that knows 0xE9 is Latin-1 `é` knows what to do at once. The fix is an encoding change, not a code change. The weak spot: the source excerpt shows the replacement glyph `�`, so a model reading only the excerpt might not know which letter was meant, though the task statement gives `café`. Another risk is that the message never says how to put the character in: it does not say to save the file as UTF-8, and the spec has no `\u` escape. A model might try to add an escape that does not exist.

**prediction**: 90 of 100 models produce a one-turn repair that compiles and prints `café`. Most of the failures will come from emitting an invalid escape such as `é`, or from a tool path that writes something other than UTF-8, not from misreading the message.

**context**: Only this directory's files reached my context: `brief.md`, `spec.md` and `p.hero`, plus the harness's system context (environment details and the user's account email), which had no bearing on the repair. I read no other file and used no tool other than reading and writing files here.

## Session c3

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` encoded `é` as the single Latin-1/Windows-1252 byte 0xE9 instead of UTF-8, and a `.hero` file must be UTF-8 text.

**reading**: The line `the byte 0xE9 at 2:15 is not UTF-8, and a .hero file is UTF-8 text` told me the exact byte, its position (the fifth character inside the string, where `é` belongs) and the rule. 0xE9 is `é` in Latin-1, which matches the intended word `café`. So the edit was to re-encode that one character as UTF-8 (0xC3 0xA9), leaving the program text unchanged. The caret under column 15 and the replacement glyph in the excerpt confirmed which character. Spec section 1 ("strings any [UTF-8] but a raw carriage return or line end") confirmed that a UTF-8 `é` inside a string literal is allowed, so no escape was needed (and section 2 offers no Unicode escape anyway).

**argument**: Yes. The message names the byte, its hex value, line and column, and the rule violated, which together identify the Latin-1 `é` uniquely. A model only has to re-encode one character. Where it falls short: it does not say "re-save the file as UTF-8" or suggest that 0xE9 is Latin-1 `é`, so a model must infer the intended character; the excerpt shows a replacement glyph, which some models could copy literally (U+FFFD) instead of `é`. A model might also hunt for a nonexistent `\u` escape or replace `é` with `e`, which compiles but prints the wrong word. Still, the task statement says `café`, so the inference is easy.

**prediction**: 92 of 100 models produce a one-turn repair that compiles and prints `café`. Falsified if fewer than 82 or more than 99 succeed. Expected failures: writing `cafe`, copying U+FFFD, inventing a `\u` escape, or using decomposed `é`.

**context**: Nothing other than this directory's files (`brief.md`, `spec.md`, `p.hero`) reached my context, besides the system and environment information supplied by the harness (working directory, date, and the user's account email, which I did not use).

## Session c4

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` stored the `é` as the single Latin-1 byte 0xE9, so the file was not valid UTF-8.

**reading**: The line `the byte 0xE9 at 2:15 is not UTF-8, and a .hero file is UTF-8 text` gave me the byte, where it was and the rule it broke. 0xE9 is `é` in Latin-1, and the brief says the program should print `café`. So I made one edit: I re-encoded that one character as UTF-8 and changed nothing else. The caret under column 15 and the `�` in the echoed source line confirmed that the bad byte was the one inside the string literal. Spec section 1 says strings may hold any UTF-8, which confirmed the literal is legal once it is encoded correctly.

**argument**: Yes. The message named the exact byte (0xE9), the exact position, and the rule (`.hero` files are UTF-8). Together with the intended output, that pins the fix down completely: 0xE9 is Latin-1 `é`. The message did not say "this looks like Latin-1 `é`; save the file as UTF-8", so a model has to make that inference itself. Most models can. The weak point is not the message but the medium: a model that copies the displayed `�` (U+FFFD) into its answer would compile cleanly and print the wrong text. The message does not warn about that trap.

**prediction**: 90 of 100 models repair it in one turn (falsifiable: fewer than 80 would refute this). The failures would mostly be models that echo U+FFFD or keep the Latin-1 encoding.

**context**: Nothing beyond this directory's files (`brief.md`, `spec.md`, `p.hero`) and the brief's quoted compiler output reached my context, apart from the harness's system prompt and an automatically attached user email. Neither had anything to do with the task.

