---
name: p
description: How far along the work of THIS chat is — a percentage, done, remaining, two plain sentences — in the chat's own language. `/p` once; `/p 60` again every 60 seconds until the task ends or `/p stop`. Invoked by the author only, never by the model.
disable-model-invocation: true
---

# /p — how far along is the work of THIS chat

Author instruction 2026-09-12. The author types one letter and gets, in a few
lines, how far the running session is with the task it was given: a
percentage, what is done, what remains, and two or three sentences in plain
words. The periodic form, `/p 60`, was the author's addition the same evening.

**This is the chat, not the project.** `/where` (`.claude/skills/where/SKILL.md`)
orients the author in the Heroes project as a whole and reads the repository
to do it. `/p` reads nothing but this conversation and answers in seconds. The
two never overlap: a question about the roadmap is `/where`; a question about
what this session has done with what it was asked is `/p`.

## Output language: the language this chat is being held in

This file is English, as every rule in the repository is (CLAUDE.md § 11). The
ANSWER is rendered in the language the author is speaking in this conversation,
every label of the shape below included. When the chat is in Italian the whole
glance is Italian; if a chat is ever held in English, the glance is English.
Nothing in this file is pasted verbatim into the answer.

## Arguments

`$ARGUMENTS` is read as a whole:

| typed | meaning |
|---|---|
| `/p`, `/p 0` | once, now. The default. No tool call. |
| `/p 60`, `/p 180`, `/p 300` | once now, then again every N seconds until the task ends or `/p stop` |
| `/p stop` | the periodic glance ends; the standing rule of Step 4 is dropped |
| `/p <anything else>` | once, narrowed to the activity named |

## Step 1 — Find the plan this chat set for itself

The plan is whichever of these the session is working from, outermost first
when they nest: the active todo list; the `/step` chain with its steps and
sub-steps; a plan the author approved; the author's request broken into its
parts. Report the outermost one the author asked for, unless `$ARGUMENTS` names
an activity, in which case report that one alone.

**Read nothing outside this conversation.** No repository file, no `git`, no
suite, no command, no tool call. The todo list is already in context. A glance
that goes and measures the tree is `/where`, and it takes minutes.

## Step 2 — Count, never feel

CLAUDE.md § 3 asks every update to carry a percentage AND the parts that
produce it (CL-038). So:

- **done** = finished AND verified. A step whose check has not run is not done.
- **in progress** = started and not finished. It counts as NOT done, and it is
  named on its own line.
- **remaining** = everything else in the plan.
- **percentage** = done / total, as an integer, never rounded up.

No explicit plan in this chat: say so in one line, still count what was done,
and mark the percentage as a guess. Never invent a plan in order to have
something to count.

## Step 3 — The shape, at most eight lines

Shown here in English; rendered in the chat's language:

```
[████████░░░░] 62% — 5 done, 1 in progress, 2 remaining (of 8)
Done: <the last two or three, comma-separated>
Now: <what is running, or what I am stuck on, or "nothing running">
Remaining: <the rest in order, the next one first, at most four>
In plain words: <two or three sentences, no jargon: what I did, what I am about to do, whether anything waits on you>
```

The bar is twelve cells, `█` filled and `░` empty. If the session is waiting on
the author — a question, an approval, a push to authorise — that goes on the
`Now:` line and is said first, because it is the one thing that changes what
the author does next.

## Step 4 — The periodic form, `/p N`

`/p 60` makes the glance a **standing rule for the rest of the task**, and the
mechanism rests on a measured fact: a scheduled prompt fires between turns,
never inside one — if the session is busy it waits for the turn to end (Claude
Code's own documentation on scheduled tasks, read 2026-09-12). A long `/step`
chain is one turn, so during the work the author cares about, nothing from
outside can speak. The session itself has to.

- After each tool call, read the clock with one `date +%s`. When N seconds or
  more have passed since the last glance, emit the shape above before
  continuing. This is CLAUDE.md § 3's own rule — never let three minutes pass,
  every two minutes when the author is following — with N chosen by the author
  (CL-034, CL-045). It has no floor: `/p 30` works.
- `date +%s` is the one tool call the periodic form allows. The clock is read,
  not felt (CLAUDE.md § RUN IT).
- When the turn ends, emit one closing glance.
- **No scheduled loop is created, on purpose.** It could not fire during the
  work, and once the chat is idle it would repeat "waiting on you" every N
  seconds. A heartbeat across idle turns is what `/loop 60s /p` already does,
  with the scheduler's one-minute floor, cancelled with `CronDelete` or Esc.
  Considered and refused on the fact above, and written down so the author can
  choose it.
- `/p stop` drops the rule. It does not cancel a `/loop` the author started; if
  one is running, the glance names its task id.

## What this skill never does

Recap the whole conversation. Teach. Put more than four items on a line. Edit a
file. Start or resume work. It is a glance, then back to work.
