# Panel 189, the llm-ergonomist's blind arm D

Run by the coordinator on 2026-10-04 between 00:12:35 and 00:15:12 by the run log, four fresh `claude -p` sessions from `<scratchpad>/189-llm-ergonomist/d1/` to `d4/`, outside any git tree (every parent walked), each folder holding `spec.md` (the specification at `7d9f2e8f`, `cmp`), `p.hero` (the program's own bytes, `cmp` against `189-blind-src/p.hero`) and `brief.md` (`docs/panel/189-briefs/blind/d-brief.md`): the draft refusal at exit 1 with the code `invalid_utf8`. The command of `docs/panel/189-briefs/llm-ergonomist.md`, `--max-budget-usd 0.25`. Each `c.hero` built by the coordinator with the trunk's compiler (sha256 `958320f39dee0b9e`) and run.

| session | model (CLI) | cost USD | turns | `c.hero` built | it printed | one-turn repair |
|---|---|---|---|---|---|---|
| d1 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1636 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| d2 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1747 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| d3 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.168 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| d4 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1623 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |

## Session d1

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` was saved in Latin-1 (or Windows-1252), so its `é` was the single byte 0xE9, and a `.hero` file must be UTF-8.

**reading**: The phrase "the byte 0xE9 at 2:15 is not UTF-8, and a `.hero` file is UTF-8 text" names the exact byte and the rule it breaks. 0xE9 is `é` in Latin-1, and the brief asks for `café`. So my one edit was to re-encode that character as UTF-8 and leave everything else as it was. The caret under column 15 and the `�` in the echoed line confirmed that the bad byte is inside the string literal, where spec section 1 allows any UTF-8.

**argument**: Yes, a one-turn repair was likely. The message names the byte value, gives its position, states the rule, and points the caret at the right column. Together with the stated goal (`café`), that makes 0xE9 easy to read as Latin-1 `é`. The message does not say which encoding the byte probably came from, and it does not say that the fix is to re-encode rather than delete. A model that does not know 0xE9 is Latin-1 `é` might drop the byte or swap in an ASCII `e`. That would compile but print `caf` or `cafe`. The `�` in the echoed line hides the original character, though the goal recovers it.

**prediction**: 90 of 100 models repair it in one turn, so the falsifiable band is 80 to 97. Most failures would come from replacing `é` with `e`, or from a tool that writes a non-UTF-8 encoding.

**context**: Nothing reached my context besides this directory's `brief.md`, `spec.md` and `p.hero`, apart from the harness's system prompt and an automatically attached user email address, which I did not use.

## Session d2

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` encoded `é` as the single Latin-1 byte 0xE9, which is not valid UTF-8, and the spec requires `.hero` source to be UTF-8 text.

**reading**: The phrase "the byte 0xE9 at 2:15 is not UTF-8, and a `.hero` file is UTF-8 text" told me the literal's content was right but its encoding was wrong. 0xE9 is `é` in Latin-1, and column 15 is the fourth character inside the string, so I re-encoded that one character as UTF-8 and changed nothing else. Spec section 1 ("strings any [UTF-8]") confirmed a non-ASCII character is allowed inside a string. Section 2 lists only six escapes and none for Unicode, so writing the character directly is the only way.

**argument**: Yes. The message names the exact byte, its position and the rule it breaks. That is enough for an encoding fix. The caret row shows `�`, so it does not show what the byte was meant to be. A model has to know that 0xE9 is Latin-1 `é` or work it out from the stated goal, `café`. Here the brief supplies that goal. The message never says "re-save as UTF-8" or "write é as UTF-8", so a model may think the language forbids non-ASCII in strings. It might then try an escape like `é`, which section 2 makes an error. The bigger risk is in the tooling. A model that writes files as text can only produce UTF-8, so the fix is easy. A model that has to output exact bytes might not manage it.

**prediction**: 90 of 100 models repair it in one turn (falsifiable: fewer than 80 or more than 97 would refute it). Most failures would be escape attempts like `é`, or tooling that writes Latin-1 again.

**context**: Besides this directory's files (`brief.md`, `spec.md`, `p.hero`), my context also held the harness system prompt with environment details (working directory, platform, date, model identity) and a system reminder with the user's account email. None of it was about Heroes, and none of it affected the repair.

## Session d3

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` held the `é` as the single Latin-1 byte 0xE9, which is not valid UTF-8, and the specification requires a `.hero` file to be UTF-8 text.

**reading**: The line `the byte 0xE9 at 2:15 is not UTF-8, and a .hero file is UTF-8 text` named both the offending byte and the rule; 0xE9 is `é` in Latin-1, and the intended output `café` confirmed it, so the edit was to re-encode that one character as UTF-8 and change nothing else. The caret at column 15 pointed at the replacement glyph inside the string, ruling out any other location.

**argument**: Yes. The message names the byte in hex, its position, and the rule it breaks, which together make the fix nearly mechanical: re-encode `é` as UTF-8. It does not say which encoding the byte came from or suggest the correct bytes, but 0xE9 plus the stated intent leaves little doubt. The weak point is not the message but the medium: a model reading `p.hero` sees only a replacement glyph, and a model whose output is itself re-encoded, or that answers by editing bytes it cannot see, could reproduce the bad byte. Nothing in the message tells a model that the language has no `\u` or `\x` escape, so a model might try one and hit the escape error instead.

**prediction**: About 90 of 100 models produce a one-turn repair that compiles and prints `café`; the failures come from escape attempts (`é`) and from tools or pipelines that write non-UTF-8 bytes. Falsified if fewer than 80 or more than 98 succeed.

**context**: Only this directory's files reached my context: `brief.md`, `spec.md` and `p.hero`, plus the harness's system prompt and an automatically attached user email, which played no part in the work.

## Session d4

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` stored `é` as the single Latin-1 byte 0xE9 instead of its UTF-8 encoding, so the file was not valid UTF-8 text.

**reading**: The message `the byte 0xE9 at 2:15 is not UTF-8, and a .hero file is UTF-8 text` gave me the byte and its position. 0xE9 is `é` in Latin-1 / Windows-1252, and the source shows `caf` just before the caret, so the intended word is `café`. That led me to re-encode that one character as UTF-8 and leave everything else unchanged. The spec (section 1: "strings any [UTF-8] but a raw carriage return or line end") confirms that a UTF-8 `é` is allowed inside a string literal.

**argument**: Yes, the message made a one-turn repair likely. It names the exact byte (0xE9), its position, and the rule it breaks (a `.hero` file is UTF-8), and the caret points at the character. A model that knows 0xE9 is Latin-1 `é` can infer the intended text. Two things work against the repair. First, the message does not say what the byte probably meant or how to write it in UTF-8 (C3 A9). Second, the snippet shows the replacement glyph `�`, which hides the original character. Also, a model's repair depends on its tooling writing UTF-8, and the message cannot control that. The spec has no `\u` escape, so a model that reaches for one would fail.

**prediction**: 90 of 100 models repair it in one turn. The failures I expect: models that use a `é` escape, models that strip the accent, and models whose write path does not produce UTF-8.

**context**: Nothing reached my context except this directory's `brief.md`, `spec.md` and `p.hero`, plus the system's default environment information (working directory, platform, date, and an account email that I did not use).

