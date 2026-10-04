# Panel 189, the llm-ergonomist's blind arm A

Run by the coordinator on 2026-10-04 between 00:12:35 and 00:15:12 by the run log, four fresh `claude -p` sessions from `<scratchpad>/189-llm-ergonomist/a1/` to `a4/`, outside any git tree (every parent walked), each folder holding `spec.md` (the specification at `7d9f2e8f`, `cmp`), `p.hero` (the program's own bytes, `cmp` against `189-blind-src/p.hero`) and `brief.md` (`docs/panel/189-briefs/blind/a-brief.md`): today's output, `check` and `build` at exit 2 with *error: cannot read `p.hero`*. The command of `docs/panel/189-briefs/llm-ergonomist.md`, `--max-budget-usd 0.25`. Each `c.hero` built by the coordinator with the trunk's compiler (sha256 `958320f39dee0b9e`) and run.

| session | model (CLI) | cost USD | turns | `c.hero` built | it printed | one-turn repair |
|---|---|---|---|---|---|---|
| a1 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1729 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| a2 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1736 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| a3 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1732 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |
| a4 | claude-haiku-4-5-20251001, claude-opus-5-5 | 0.1794 | 6 | 0 | `63 61 66 c3 a9 0a` | yes, precomposed |

## Session a1

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` holds a byte after `caf` that is not valid UTF-8 (shown to me as the replacement character U+FFFD, most likely a lone Latin-1 `0xE9`), so the compiler refused to read the file as source.

**reading**: The message `error: cannot read `p.hero`` says nothing about the cause. I could not have fixed anything from it alone. What led me to the edit was reading `p.hero`: the string showed `caf` followed by a replacement character. Section 1 of the spec says strings may contain "any UTF-8". So I took "cannot read" to mean "not valid UTF-8" and not a missing file or a permissions problem. Since the program is supposed to print `café`, I rewrote the `é` as valid UTF-8.

**argument**: No. "cannot read" tells you neither the byte, the offset nor the rule broken. It does not even separate an I/O failure from an encoding failure. A one-turn repair happened only because the file is two lines long and the bad byte showed up as a visible replacement character when the file was read. In a longer file, or with a reader that hides invalid bytes, a model would be unable to locate the fault. It might also conclude the path was wrong and change nothing. A useful message would name the line, the column and the invalid byte, and say that source must be UTF-8.

**prediction**: 70 of 100 models repair it in one turn. That figure is falsifiable: most models will see the replacement character in `p.hero` and retype `café` correctly. The failures will come from models that trust the message, conclude the file is missing or unreadable, and resubmit unchanged or with irrelevant edits.

**context**: Nothing beyond this directory's `brief.md`, `spec.md` and `p.hero` reached my context, apart from the harness's environment details (working directory, platform, date, and a user email address I did not use).

## Session a2

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` contains a byte after `caf` that is not valid UTF-8 (it showed up as a replacement character when I read it, most likely a Latin-1 `0xE9`), so the compiler could not decode the file as text.

**reading**: The compiler printed only `error: cannot read `p.hero``. It gave no line, column, byte offset or reason. On its own it points to a missing file or a permissions problem. But I could open the file, and its one string showed a replacement character where `é` should be. Spec section 1 says strings may hold "any UTF-8". That told me the file was the wrong encoding, so I wrote `é` again as UTF-8 (`0xC3 0xA9`) and changed nothing else.

**argument**: The message made a one-turn repair possible but did little to make it likely. "cannot read" sounds like an I/O failure, a missing file or a permissions problem, not a decoding failure. It names no byte offset, no line and no encoding. A model gets the repair right only by looking at the bytes, or at how its reader shows them, and connecting a replacement character to the UTF-8 rule in section 1. A model that trusts the message might change nothing, or might decide the problem is outside the program. The fix itself is trivial once the cause is found. A message like "p.hero: invalid UTF-8 at line 2, column 15 (byte 0xE9)" would make this repair close to certain.

**prediction**: About 55 of 100 models repair it in one turn. Models whose file reader shows the replacement character will mostly get it right. Models that take "cannot read" to mean an I/O failure, or whose rewrite carries the bad byte over unchanged, will fail.

**context**: Only this directory's files, `brief.md`, `spec.md` and `p.hero`, plus the harness's system context (environment details and an account email, which I did not use). Nothing else reached my context.

## Session a3

`c.hero`:

```
function main()
    print("café")
```

**cause**: The `é` in `p.hero`'s string literal was not valid UTF-8 (my read tool showed it as the replacement character U+FFFD, which fits a lone Latin-1 byte 0xE9), so the compiler could not decode the file.

**reading**: All the compiler printed was `error: cannot read `p.hero``. It gives no line, no byte offset and no reason. It does not say whether the file was missing, unreadable, or badly encoded. Because I could read the file, I ruled out a missing file or a permission problem. The `�` I saw in the string, together with spec section 1 ("strings any [UTF-8]"), led me to the edit: write the same string again as valid UTF-8 (`é` as U+00E9, bytes C3 A9). Nothing in the message pointed at that line.

**argument**: No. "cannot read" sounds like an I/O failure, not an encoding error. It names no line, no byte and no cause, so a model could easily go looking for problems with the path, permissions or file existence. The repair was likely only because my file reader happened to show the bad byte as `�` and the spec says strings are UTF-8. A model whose tools quietly dropped or changed the invalid byte would get no help from the message at all. A message like "p.hero:2:16: invalid UTF-8 byte 0xE9" would have made the repair nearly certain. The program's stated purpose ("print café") supplied the intended character. The message did not.

**prediction**: About 55 of 100 models repair it in one turn. Most fail for one of two reasons: their tools hide the invalid byte, or "cannot read" leads them to change nothing or blame the environment.

**context**: Only `brief.md`, `spec.md` and `p.hero` from this directory, plus the system context of the harness (environment details and an account email, neither of which I used). I read no other files.

## Session a4

`c.hero`:

```
function main()
    print("café")
```

**cause**: `p.hero` has a byte that is not valid UTF-8 inside the string literal. It looks like a lone Latin-1 `é` (0xE9), which the Read tool showed as U+FFFD. The compiler therefore refused to read the file at all.

**reading**: The message `error: cannot read \`p.hero\`` gives no line, column, byte or reason. On its own it points to a missing file or a permissions problem, and neither one can be fixed by editing the program. What actually led me to the edit was opening `p.hero`: the replacement character in `"caf�"`, together with the stated intent ("print the word `café`") and spec section 1 ("strings any [UTF-8]"), pointed to an encoding fault. So I re-encoded the literal as valid UTF-8. Nothing the compiler printed contributed to that edit.

**argument**: No. The message names the file but not the problem, and "cannot read" suggests I/O, not encoding. A model that trusts the message might check the path, conclude the file is fine, and resubmit unchanged, or it might rewrite the program in ASCII (`cafe`), which compiles but fails the intent. The repair is likely only because the program is two lines long and the intent names the exact word, so the reader can compare the source with the goal. The message should say something like "invalid UTF-8 at line 2, column 15 (byte 0xE9)". With the current message, one-turn success depends on the model inspecting the bytes itself and on how its tools render them.

**prediction**: About 70 of 100 models repair it in one turn. Most will see the replacement character and rewrite the literal in UTF-8. The failures will be models that read "cannot read" as a path or permissions problem and resubmit an identical file, models whose tools silently round-trip the bad byte, and models that drop the accent to `cafe`. If fewer than 50 or more than 90 succeed, this prediction is falsified.

**context**: Besides this directory's files (`brief.md`, `spec.md`, `p.hero`), my context contained the harness system prompt, environment details (working directory, platform, date, model identity), and an automatically attached note with the user's email address. None of it concerned Heroes or this program, and none of it informed the repair.

