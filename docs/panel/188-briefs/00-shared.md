# Panel 188, the shared brief: what a header's name may hold, and what the compiler makes of it

Written 2026-10-03 by the coordinator on the trunk at `826ddc2f`, frozen from
the seats' launch to the synthesis, then repaired after the completeness
critic's first pass (`docs/panel/188-reports/completeness-critic-briefs.md`;
the text it read is `00-shared-before-the-critic.md`). Every number names the
command or the file that produced it; `00-facts.md` holds the facts, cited as
F1 to F8. What was not run says so.

## Why this sitting

Defect 216 (`blocking`, `docs/work/DEFECTS.md`): a header whose name holds a
`>` passes `check` at exit 0 and stops `build` at exit 2 with an internal
error and clang's text. A refusal at `check` is a diagnostic class and so a
sitting's (CLAUDE.md § 4). The author convened this one in full on
2026-10-03, its blind seat's paid sessions capped at 3 USD in all (the
author's answer *1a*,
`docs/records/log/2026-10-03-1627-the-author-answers-1a-2a-3a-panel-188-convened-the-x86-containers-and-18-lane-worktrees-removed.md`).

Measured before and by the critic (F1, F6, F7, F8), **the class has three
causes**:
- **what `#include <...>` cannot carry**: `>` (exit 2, an internal error,
  `extern "<stdio.h>"` among it), a line end, a NUL byte (exit 2), and the
  empty name (a false `ffi_unknown_name`);
- **escapes the compiler does not decode**: neither reader of a header string
  turns `\\`, `\"` or `\n` into their characters, so `extern "a\\b.h"` is
  written `#include <a\\b.h>`, two backslashes, and `build` says falsely that
  the header is missing; clang carries `\` and `"` in an angled include on
  this Mac;
- **a malformed name accepted**: `extern "stdio.h>"` builds at exit 0.

Six names work on this Mac (a space, `'`, `//`, `/*`, a trigraph, `<`). The
question is the class, not the one character.

## What C says, as the coordinator recalls it (a question, not a premise)

C11 6.4.7 (Header names) is recalled as making a header name between `<` and
`>` any characters but a line end and `>`, and the behaviour **undefined**
where `'`, `\`, `"`, `//` or `/*` occur in it. The historian verifies the
text, its paragraph, and whether C17 or C23 changed it; until then every seat
treats it as recalled.

## The questions

**Q1. The class: what a header's name may hold, and what the compiler writes
for it.** Routes the coordinator and the critic can name, none built:
- **(1a)** refuse at `check` what `#include <...>` cannot carry: `>`, a line
  end, a NUL, the empty name;
- **(1b)** refuse also what C leaves undefined between `<` and `>` (as
  recalled above): `'`, `\`, `"`, `//`, `/*`;
- **(1c)** a positive rule: a relative path of letters, digits and a stated
  few marks, every other character refused; its cost is the real headers it
  would refuse, which the ffi-pragmatist surveys;
- **(1d)** refuse nothing at `check`, and make `build` say true things;
- **(1f)** write the string's VALUE into the `#include`, its escapes decoded
  as spec § 2 defines them, so that a name means what the string says,
  whatever else is refused;
- **(1g)** refuse any escape in a header's string;
- **(1h)** a combination: a refusal at `check` together with (1d) or (1f);
- **(1i)** hand the header to clang by its `-include` flag instead of an
  `#include` line (recalled by the critic, unrun);
- **(1e)** a route nobody listed.

*Corrected 2026-10-03, after the seats: (1a)'s list is complete for a byte in
the middle of a name (the ffi-pragmatist's sweep of every ASCII byte on three
clangs adds only CR, a line end's second byte) and incomplete for its last
byte: a `\` before the closing `>` sends clang down its token path and binds
another file (the critic's second pass § 3). (1h)'s "a refusal" left the set
open, and four seats filled it four ways (the same pass, § 2).*

**Q2. The message and its fix.** What the refusal says (the character, why C
cannot carry it, what to write instead), its code (a new one, or an existing
one widened, `machine_locked_path` beside it), and whether any fix is
`certain` by `.claude/rules/diagnostics-and-goldens.md` (*a `certain` fix
repairs the defect the diagnostic names*): C's `<name>` written inside the
string, and a `\` written for a `/`, which is a separator on Windows and a byte
of a name elsewhere.

**Q3. Where it lives and what it costs.** Beside `machine_locked` in
`selfhost/parse/group_head.hero` (F8), against the parser's budget of 8,696
lines with 7 of room and panel 187's registered prediction that it holds at
the next tag (F3), or elsewhere; its lines, its cases, and the tools that
re-print a program (`.claude/rules/diagnostics-and-goldens.md` § A new
surface form, if any part of this is one).

**Q4. The specification.** § 13 says nothing of what a name may hold; panel
055's absolute-path sentence was in it and was removed on 2026-09-05 because
the compiler's message states the rule (F4). Is a sentence owed now, and at
what price (F3, design.md §1.6's payment rule), against that precedent.

**Q5. The shapes beside.** A `link` and a `package` string reach a linker and
`pkg-config`, not an `#include`: does any character of theirs fail the same
way; does `machine_locked` cover every absolute form it means to; and does
any other string the compiler hands to C carry undecoded escapes.

## The frozen tree and your copy

`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`.
Your copy is `<scratchpad>/188-<seat>/`, made with `git -C
/Users/joseph/Temp/heroes/heroes-lang archive 826ddc2f | tar -x -C <your copy>`;
build your compiler inside it from the seed, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (the seed's sha256 begins `2c809845ed0f7ba8`, the
trunk's compiler built from it `afc05be6b2c50184`: check yours). Rebuild the
compiler from `selfhost/` after an edit with `./heroes build selfhost/main.hero
-o heroes`. F1's and F6's cases are `<scratchpad>/188-facts/` (read only; copy
what you need); the critic's are `<scratchpad>/188-critic/`. Never build, run
or read inside another seat's copy or a lane's worktree, and inside the trunk
nothing but your own report. **No paid run**: the llm-ergonomist's two
sessions, at most 1.5 USD each, are the coordinator's; no seat runs `heroes
measure --refresh` (an API call). At most three processes at once, no timing.
**Docker**: one container at a time on this Mac, `docker ps -q` empty first,
and the coordinator's own Linux runs may be using it (wait and re-check every
minute). The Windows box is offline (F1): what needs it is unrun, said so.
Your report is `docs/panel/188-reports/<seat>.md` in the TRUNK, written as you
go: a verdict per route, what you built and ran, your cost, a falsifiable
prediction, the condition that would change your verdict. English, no em
dashes.
