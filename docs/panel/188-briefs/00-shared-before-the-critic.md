# Panel 188, the shared brief: what a header's name may hold

Written 2026-10-03 by the coordinator on the trunk at `826ddc2f`, frozen from
the seats' launch to the synthesis. Every number names the command or the
file that produced it; `00-facts.md` in this directory holds the facts
measured before the briefs, cited as F1 to F5. What was not run says so.

## Why this sitting

Defect 216 (`blocking`, `docs/work/DEFECTS.md`): a header whose name holds a
`>` passes `check` at exit 0 and stops `build` at exit 2 with an internal
error and clang's text. Its repair is a refusal at `check`, a diagnostic class
and so a sitting's (CLAUDE.md § 4). The author convened this one in full on
2026-10-03, with the blind seat's one paid session capped at 3 USD (the
author's answer *1a*).

Measured before the briefs (F1), the defect is a class with two faces: `>`
gives an exit 2 and an internal error; `\`, `"` and a line end give a **false
message**, `ffi_missing_header` saying the header is not on the include path
while it is there. Six other names work on this Mac (a space, `'`, `//`, `/*`,
a trigraph, a plain name). **And the plausible mistake is in the class** (F6):
`extern "<stdio.h>"`, C's `#include <stdio.h>` carried into the string, stops
`build` at exit 2 with an internal error, and `extern "stdio.h>"` builds at
exit 0, a malformed name accepted. The question is the class, not the one
character.

## What C says, as the coordinator recalls it (a question, not a premise)

C11 6.4.7 (Header names) is recalled as making a header name between `<` and
`>` any characters but a line end and `>`, and the behaviour **undefined**
where `'`, `\`, `"`, `//` or `/*` occur in it. The historian verifies the
text, its paragraph, and whether C17 or C23 changed it; until then every seat
treats it as recalled.

## The questions

**Q1. The class: what a header's name may hold.** Routes the coordinator can
name, none built:
- **(1a)** refuse what this Mac's clang fails on: `>`, a line end, `\`, `"`
  (F1);
- **(1b)** refuse what C makes impossible or undefined between `<` and `>`:
  a line end and `>`, and `'`, `\`, `"`, `//`, `/*` (as recalled above),
  whatever one clang does with them;
- **(1c)** a positive rule: a header's name is a relative path of letters,
  digits and a stated few marks, every other character refused; its cost is
  the real headers it would refuse, which the ffi-pragmatist surveys;
- **(1d)** refuse nothing at `check`, and make `build` say true things: the
  `>` an error on the author's line, the false `ffi_missing_header` true;
- **(1e)** a route nobody listed.

**Q2. The message and its fix.** What the refusal says (the character, why C
cannot carry it, what to write instead), its code (a new one, or an existing
one widened, `machine_locked_path` beside it saying *where the machine keeps
it*), and whether any fix is `certain`: a `\` written for a `/` on Windows
reads as a separator there and as a byte of a name elsewhere, and
`.claude/rules/diagnostics-and-goldens.md` says a `certain` fix repairs the
defect the diagnostic names.

**Q3. Where it lives and what it costs.** Beside `machine_locked` in
`selfhost/parse/group_head.hero` (F2), against the parser's budget of 8,696
lines with 7 of room (F3), or elsewhere; its lines, its cases, and the tools
that re-print a program (`.claude/rules/diagnostics-and-goldens.md` § A new
surface form, if any part of this is one).

**Q4. The specification.** § 13 says nothing of what a name may hold, nor of
the absolute paths panel 055 refuses (F4). Is a sentence owed, which, and at
what price (F3, design.md §1.6's payment rule).

**Q5. The shapes beside.** A `link` and a `package` string reach a linker and
`pkg-config`, not an `#include`: does any character of theirs fail the same
way; and does `machine_locked` cover every absolute form it means to.

## The frozen tree and your copy

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/188-<seat>/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 826ddc2f | tar -x -C <your copy>`;
build your compiler inside it from the seed, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (the seed's sha256 begins `2c809845ed0f7ba8`, the
trunk's compiler built from it `afc05be6b2c50184`: check yours). Rebuilding
the compiler from `selfhost/` after an edit, `./heroes build selfhost/main.hero
-o heroes`, takes about a minute. F1's cases are `<scratchpad>/188-facts/`
(read only; copy what you need). Never build, run or read inside another
seat's copy or a lane's worktree, and inside the trunk nothing but your own
report. **No paid run**: the llm-ergonomist's one session is the coordinator's;
no seat runs `heroes measure --refresh` (an API call). At most three processes
at once, no timing. The Windows box is offline (F1): what needs it is unrun,
said so. Your report is `docs/panel/188-reports/<seat>.md` in the TRUNK,
written as you go: a verdict per route, what you built and ran, your cost, a
falsifiable prediction, the condition that would change your verdict. English,
no em dashes.
