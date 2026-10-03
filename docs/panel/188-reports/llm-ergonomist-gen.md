# Panel 188, the llm-ergonomist's generation arm: does a reader of the spec write C's brackets inside the string

Run by the coordinator on 2026-10-03, after the seats reported, to score the
spec-warden's P3 (`spec-warden.md` § Predictions), which the critic's first
pass had also named as the measurement the A/B could not make (*"whether models
write `extern "<math.h>"` at all"*). A paid run the spec-warden's report named
with its size and left to the coordinator, inside the author's cap for this
sitting's blind seat, 3 USD in all (the author's answer *1a*).

## How it was run

Thirteen fresh `claude -p` sessions, each from its own folder under
`<scratchpad>/188-llm-ergonomist/gen/` (`s01` to `s10`, `u1` to `u3`), outside
any git tree (every parent walked: no `CLAUDE.md`, `.git` or `.claude`; `git
rev-parse` fails), each folder holding `spec.md`, the specification at
`826ddc2f` byte for byte (`cmp`), and `brief.md`. The command is the
llm-ergonomist brief's (`--model claude-opus-5-5 --restricted --safe-mode
--strict-mcp-config`, tools Read and Write) with `--max-budget-usd 0.25` per
session. The CLI 2.1.285 reported `claude-opus-5-5` (and
`claude-haiku-4-5-20251001` for its own use) in every `run.json`, 5 turns each,
no error.

- **`s01` to `s10`, P3 as registered**: the brief is
  `docs/panel/188-briefs/blind/gen-brief.md` (written 17:23:24 by its file's
  time, the sessions from 17:23:45 to 17:24:22), its task *"Write a program
  that binds the C function `sqrt` from the header `math.h` and prints the
  square root of 2."* It names the header in prose without brackets, which
  leans toward the bare spelling.
- **`u1` to `u3`, the task naming no header**: the brief is
  `docs/panel/188-briefs/blind/gen-unprimed-brief.md` (written 17:25:29, the
  sessions from 17:25:37 to 17:26:12), identical but for the task, *"Write a
  program that prints the square root of 2 using the C standard library's
  `sqrt`."* (`diff` of the two: that line alone). Three sessions and not ten
  because of the cap: 0.25 USD each at worst kept the sitting under 3 USD.

The coordinator built `s01`'s `c.hero` with the trunk's compiler (sha256
`afc05be6b2c50184`, built from the seed at `826ddc2f`), which builds every
session's, the thirteen files being one text (below).

## What they wrote

**All thirteen `c.hero` files are byte-identical** (`md5`: one value,
`b0ac376034319a5fd182575b52188b25`, thirteen times):

```
extern "math.h" link "m"
    function sqrt(x: f64) -> f64

function main()
    print(sqrt(2.0))
```

`check` 0, `build` 0, it printed `1.4142135623730951`. **0 of 13 wrote C's
brackets inside the string**: 0 of 10 with the header named, 0 of 3 with no
header named.

**Where the spelling came from**: every `reading` section cites § 13's
example `extern "sqlite3.h" link "sqlite3"` beside the `Extern` production
and *"A group names its header, and `link` a library when the symbols need
one"*.

**Whether the bracketed spelling was considered** (`grep -n -i -E
'bracket|<math|angle|include'` over the reports): in none of the ten
sessions given the header's name; in **all three** sessions given none, each
listing it under `choice_points` and choosing against it by the example:
`u1`, *"`math.h` or `<math.h>` in the string. I chose `"math.h"`, as in the
`"sqlite3.h"` example. Brackets would probably be passed through as a
different include path or refused."*; `u2`, *"Header `"math.h"` versus
`"<math.h>"` or another spelling. Chosen: `"math.h"`, mirroring
`"sqlite3.h"`. Angle brackets would likely be passed through literally and
fail."*; `u3`, *"Header `"math.h"` versus `"<math.h>"`: I followed the
example's bare form."*

**Their predictions** (first-try programs that compile and do what the task
asks, of 100): `s01` 70, `s02` to `s05` 85, `s06` 90, `s07` to `s10` 85; `u1`
85, `u2` 85, `u3` 80.

**Their `context` sections**: all thirteen say only `brief.md` and `spec.md`
were read, beside the harness's system prompt, its environment details and an
attached account email; none names a project rule, contract or memory.

## Cost

`total_cost_usd` from each `run.json`: `s01` to `s10` **1.7441 USD**, `u1` to
`u3` **0.5233 USD**; with the A/B's 0.3477, **the sitting's paid runs total
2.6151 USD of the author's 3**. No further paid run.

## What it scores

- The spec-warden's **P3**, *"ten fresh sessions given the status-quo spec
  and asked to bind `sqrt` from `math.h` write the brackets inside the string
  in at most 1"*: **held, 0 of 10**. Its second half (*"with a6, in 0"*) is
  unrun: no session was given a6.
- The spec-warden's condition *"a6 is owed under either standard if P3's run
  finds 2 or more of ten bracketed"*: **not met**.
- The historian's third prediction, *"in sessions that bind a system header
  with no Heroes `extern` example in view, a model writes C's brackets inside
  the string at least once"*: **not scored**, since § 13's example was in
  view in every session; nothing here falsifies or supports it.
- What it does not measure: another model, another header, a reader with
  less of the spec in view than all of it, and a session given a6.
