# Panel 184, how the llm-ergonomist sits

The seat runs as three fresh sessions outside the repository, one per
question, so no task sees another (`.claude/skills/panel/SKILL.md` § 2, the
paragraph of 2026-09-30). This file is the record of what each session was
given; the sessions themselves never read it. **Repaired before any session
ran**, on the completeness critic's first pass (items 14 to 16 and D2 of
`docs/panel/184-reports/completeness-critic-briefs.md`): the folders renamed,
the leaks in the briefs removed, the variants R and Y brought to 00-shared.md's
routes, and the run's flags closing the channels the critic found; the first
versions the critic read are the ones its report quotes, and the versions
below are the ones that ran (`docs/panel/184-briefs/blind/`).

**The folders**, with neutral names (the first ones named the sitting and the
seat in their path, which a session is told):
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/rdr/t1/`,
`t2/` and `t3/`, and the probe's `p0/`. Read at 16:55: `git -C <folder>
rev-parse --show-toplevel` answers *fatal: not a git repository* for all four;
`CLAUDE.md`, `CLAUDE.local.md` and `.claude` exist in none of their ancestors
up to `/` (`[ -e ]` on each of eight directories); each `spec.md` differs from
`git show a294a6ff:spec/heroes-spec.md` by its marker line alone (`diff`).

**What each folder holds**: `spec.md` with one marker: task 1 replaces
`A literal without the `f` is unchanged.` (§ 2) with `[RULE 1]`; task 2 adds a
line `[RULE 2]` after § 8's `Loops:` line; task 3 adds a bullet `- [RULE 3]`
after § 9's `Recursion too deep aborts.`. Then `brief.md`, the seat's method
and output structure without the project's names, which asks for the code
written under each variant BEFORE any output file is opened, and the task's
programs and outputs:

| task | programs | outputs, from `<p184>/heroes-a294a6ff` | the variants, label-stripped |
|---|---|---|---|
| 1 | `task1.hero`: two literals whose `f` is missing, one whose hole's name is bound only for it | `o1-check.txt`: one `unused_binding` at `count`, exit 1; the other literal silent | K = (1b), L = today's sentence, M = (1a) |
| 2 | `task2.hero`: `i @ i + 1` under `return i` in a `while`, and a `print` under `break` | `o2-check.txt`: exit 0, nothing printed; `o2-run.txt`: built and run, nothing printed and still running at 5 s (`perl -e 'alarm 5; exec @ARGV'`, exit 142) | P = (2b), Q = today's silence, R = (2a), the one predicate |
| 3 | `task3a.hero`, a 300-term `+` chain; `task3b.hero`, `step(` nested 100 deep | `o3a-check.txt`: `panic: stack exhausted in checkwalk.synth`, exit 134; `o3b-check.txt`: exit 0; `o3b-build.txt`: `panic: stack exhausted in checklower.named`, exit 134 | X = today's silence, Y = (3a) with every chain held at any length, 256 as the rule's example, Z = today's behaviour stated |

One more measurement stands behind task 1 and is not given to the seat:
`task1.hero` with the line `count = rows.len()` removed, which is what
`o1-check.txt`'s *remove the binding* asks for, checks at exit 0, builds, and
prints `{count} rows` and `total {total} over {rows.len()} rows`
(`<p184>/blindprep/task1-advice-taken.hero`; the critic reproduced it).

**The probe, the coordinator's one paid run before the seat** (the critic's
item 15, *the coordinator decides*): in `p0/`, a brief asking the session what
it was told and which tools it has, and to try two reads outside its folder,
run at 16:56 with the flags below except `--tools`, capped at 0.5 USD, cost
**0.34 USD** by the CLI's JSON report, 5 turns. Its report,
`docs/panel/184-reports/llm-ergonomist-probe.md`: its working directory the
folder; **no memory, no project rules, no `CLAUDE.md`, no output style and no
extra directories**; the only language instruction the run's own *Answer in
English*; both reads outside the folder **refused**, *`--restricted` confines
the file tools to the working directory*. It also had tools the seat does not
need, `ListAgents` and `SendMessage` among them, which could reach another
session on this machine; so the seats' runs name their tools with `--tools`,
and their `context` heading asks for the tools each had.

**The run**, per folder, from inside it:

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --restricted --safe-mode --strict-mcp-config \
  --tools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 3 --output-format json > run.json 2> run.err
```

`--restricted` ignores the user, project and local settings files (the
critic found `"language": "italian"` and 22 extra directories in the user's)
and confines the file tools to the working directory; `--safe-mode` disables
`CLAUDE.md`, skills, hooks, output styles and the rest of the customisations;
`--strict-mcp-config` loads no MCP server (`claude --help`, 2.1.274).

**The first attempt, 17:05, gave no reading.** Tasks 1 and 2 were stopped by
the API itself, *Opus 5 (1M context)'s safeguards flagged this message*,
details `[reasoning_extraction]`, after 4 and 5 turns (0.17 and 0.18 USD);
task 3 by the account's session limit after 8 turns (0.88 USD); no report was
written. The cause of the first two is read and unrun: the briefs asked the
session to note *every point where you hesitated or guessed* and, new since
panel 183, *the names of the tools you have*, which read as a request for its
own reasoning and context. So the second attempt's briefs ask instead for
*every place where the specification left the author a choice, the choice you
made, and what the other choice would produce*, under the heading
`choice_points` in place of `hesitation_points`, and drop the tools question
(the probe had measured the confinement). A run with `--model claude-opus-5-5`
was refused by the CLI at no cost (*Claude Code 2.1.274 does not support this
model; version 2.1.280 or newer is required*), and the CLI was not updated,
since that changes a tool installed on the author's machine; the second attempt
runs on the default model. The versions in `docs/panel/184-briefs/blind/` are
the second attempt's; the first attempt's run files are
`<scratchpad>/rdr/attempt1/` and `attempt2/`.

**The sitting's paid runs are the probe and these sessions and no other**,
each capped by the CLI's own `--max-budget-usd`: 0.5 USD for the probe (0.34
spent), 3 USD for each session; the first attempt spent 1.23 USD, so the whole
seat is bounded by 0.34 + 1.23 + 3 × 3 = 10.57 USD.

**The second attempt, 17:25 to 17:37, gave three readings**, at 1.75, 1.66
and 2.22 USD by the CLI's JSON reports (11, 12 and 13 turns, none an error):
**7.20 USD for the whole seat**. One defect of the run itself, the
coordinator's: `--tools "Read,Write"` names the tools a session HAS, and the
probe's `--allowedTools "Read,Write"`, which APPROVES them, was dropped when
`--tools` was added, so every `Write` into the folder waited for a permission
print mode cannot give. Each session said so, and wrote its steps 1 and 2 (before
opening any output, by their files' times) and its report to its own session
scratchpad instead, `/private/tmp/claude-501/-private-tmp-claude-501--Users-joseph-Temp-heroes-heroes-lang-edfda945-b355-427c-a48e-2113b1040b67-scratchpad-rdr-t<n>/<session>/scratchpad/`;
the coordinator copied both files of each into
`docs/panel/184-reports/llm-ergonomist-task<n>.md`. A run of this seat names
both flags.

**Two further readings were prepared and not run** (folders `rdr/t4` and
`rdr/t5`, 21:45): the critic's § E items 6 and 7, task 1 with (1b) amended
and the symmetric escape stated, and task 2 with (2d) as a fourth variant. The
synthesis puts both to the author with the ratification, sized at one session
and a 3 USD cap each. The
coordinator copies each `report.md` to
`docs/panel/184-reports/llm-ergonomist-task<n>.md` with a header saying how it
was run, what it cost by the CLI's report, and its `context` answer; a *yes*
voids that reading until it is re-run.
