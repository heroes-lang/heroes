---
name: graph
description: Draw the plan THIS chat is working from as a graph of its steps, every leaf under its step with its state read from the repository and GitHub, publish it as a private Artifact and keep it updated in place. Use when the author asks for the graph or to update it (they ask in Italian: "grafo", "aggiorna grafo", "dammi il grafo", "stampami il grafo", "fammi vedere il grafo").
---

# /graph: the plan of this chat, drawn

Author instruction 2026-11-09, meant as: *I like this graph; save it as a skill
I can call in the other chats too*. It was drawn by hand that day, for the plan
that closed M-package-manager, and published again each time the author asked
("aggiorna grafo", "dammi il grafo"); this skill is what that page learned.

**Where it sits beside the other two.** `/p` (`.claude/skills/p/SKILL.md`) is a
glance at this chat's progress in eight lines, read from the conversation alone,
in seconds. `/where` (`.claude/skills/where/SKILL.md`) orients the author in the
project as a whole. `/graph` is the chat's plan **drawn**: every step a node,
every nameable thing a step produced or owes a leaf, and **every state read
from the world before it is drawn**, which is what makes it slower than `/p` and
worth the wait.

## Output language: the language this chat is being held in

This file is English (CLAUDE.md § 11). The PAGE is written in the language the
author speaks in this chat, every label included; the template's labels are
Italian because the author's chats are. Plain words, no em dashes (CL-029).

## Step 1: Find the plan, outermost first

The plan this chat works from: the active todo list, the `/step` chain, a plan
the author approved (the plan file, when plan mode wrote one), or the author's
request broken into its parts. The outermost one the author asked for is the
graph; a named activity in `$ARGUMENTS` narrows it to that activity.

**A step is a node; a leaf is a nameable thing**: a commit, an issue, a
resolution, a check that ran, an answer the author owes. Leaves do not weigh the
same, so the percentage counts things and not hours, and the page says so.

## Step 2: Read every state from the world

CLAUDE.md § RUN IT binds every leaf. Before a leaf is `ok`, the command that
settles it has been run in this turn: `git log` or `git branch -r --contains`
for a commit (pushed or still a lane's), `gh issue view` or
`heroes run tests/harness/issues.hero -- open` for an issue, the gate's own log
for a suite, `gh run view` for a CI leg. The six states:

| state | meaning | counted |
|---|---|---|
| `ok` | done and verified | yes |
| `run` | started, not finished; the note says what is running | yes |
| `you` | waits on the author: a reading, an answer, a yes | yes |
| `todo` | not started | yes |
| `ko` | a failure observed; its repair is a leaf elsewhere | no |
| `skip` | skipped or overtaken, the note saying why | no |

A node takes its state from its leaves: `run` if any runs, else `you` if any
waits on the author, else `ok` when all are done, `part` when some are, `todo`
otherwise. A percentage is done over counted, floored, never rounded up. A
short hash a lane holds and `origin` does not goes into `LOCAL`, so the page
shows it grey instead of linking to a commit GitHub does not have.

## Step 3: The file

**One file per plan, under `.claude/worktrees/plan-graph/`**, which git
ignores: inside the repository's root, as CLAUDE.md § Hard stops asks of every
file, and outside the tracked tree, since a chat's plan is not a record.

- **The first time**, copy `.claude/skills/graph/template.html` there under a
  short slug (`piano-<slug>.html`), and set its `<title>` to a name of two to
  four words.
- **Edit only between `DATA BEGIN` and `DATA END`**: `META` (the words of the
  page, the map's `view` and its band labels), `REPO`, `LOCAL`, `PH` (the
  nodes), `EDGES` and `PATH` (what stands between now and the goal, in order).
  The comments there give each shape. The renderer below the marker is shared
  by every graph; a change to it is a change to this skill's template, made
  there.
- **The layout**: a node is 172 by 66; columns at `x = 20 + 200·k`, six across
  a `view` 1212 wide; the done band's rows at `y = 40` and `136`, the to-do
  band's at `290` and `392` (a `view` 474 high), the two band labels above
  each. An edge runs right side to left side, or bottom to top in one column;
  anything else names its sides and waypoints, `{ from: "B", to: "L", via:
  [[x, y]] }`, so that no edge crosses a node.
- **`toGoal: true`** on every node up to the goal the author set (a tag, a
  push, a deadline): the first figure counts those, the second the whole plan.
- **The footer says when and from what it was read**: the clock, then each
  source (`git log` of which branches, `gh issue list`, which CI run, which
  gate's logs). A page that does not say when it was read is a claim with no
  date.
- After the artifact's first publish, the file's first line holds its link,
  `<!-- artifact: https://claude.ai/... -->`, so a later chat finds it.

## Step 4: Look at it before the author does

Render it headless and read the screenshot. Chrome's profile goes inside the
scratch folder, never in the home directory:

```
{ printf '<!doctype html><html lang="it"><head><meta charset="utf-8">\n'; cat <file>; printf '</html>\n'; } > <scratch>/preview.html
"/Applications/Google Chrome.app/Contents/MacOS/Google Chrome" --headless=new --disable-gpu \
  --user-data-dir=<scratch>/chrome-profile --no-first-run --virtual-time-budget=3000 \
  --window-size=1280,1400 --screenshot=<scratch>/preview.png file://<scratch>/preview.html
```

`<scratch>` is a folder under `.claude/worktrees/`. A blank page is a script
error: the renderer throws on an edge that names a missing node, and `--dump-dom`
in place of `--screenshot` shows how far it got. Look for an edge through a
node, a title cut by its box, and a band label under a node.

## Step 5: Publish, and keep the same link

- **First time**: the Artifact tool, `file_path` the file, `icon` `map`, a
  one-sentence `description`; then write the link into the file's first line.
- **In the same chat**: publish the same `file_path` again; the link stays.
- **In another chat, over a graph that exists**: take the link from the file's
  first line, `read` it with the Artifact tool, then publish with that `url`.
- Give the author the link in one line, with what moved since the last
  version, and nothing else: the page is the answer.

An Artifact is private until the author shares it, so publishing one is not
an outward-facing act in CLAUDE.md § Hard stops' sense; pushing the file, or
anything that names it, is.

## What this skill never does

Draw a state it did not read (Step 2). Put the graph into a tracked path or a
commit. Write the page's numbers into an image: they stay text, as CLAUDE.md
§ 11 asks of every measured number (CL-035). Stop the work: a graph asked for
mid-task is drawn, published, and the task resumes in the same turn, as `/p`
does.
