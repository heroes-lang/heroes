# Panel 212, the llm-ergonomist's brief

Your input is ONLY these five files, by their absolute paths (you cannot list a directory, so here they are):

1. /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-212/docs/panel/212-briefs/samples/01-a-defect.md
2. /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-212/docs/panel/212-briefs/samples/02-a-decision-with-prior-art.md
3. /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-212/docs/panel/212-briefs/samples/03-a-closed-task.md
4. /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-212/docs/panel/212-briefs/samples/04-four-defects-in-one-card.md
5. /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-212/docs/panel/212-briefs/samples/05-an-old-log-row.md

and this page. **A first run of this seat read none of them** (the brief named a folder and the seat
cannot list one); it also reported that the project's `records.md` and `verification.md` reached its
context as system context, and `records.md` describes the rendering under test, so say again in
`context` what reached you and whether it told you what a good issue should look like. **Do not read `00-shared.md`, design.md, the issue files or
any project document.** Your method and output structure are the ones your agent definition gave you
(`verdict`, `experiment`, `hesitation_points`, `argument`, `prediction`, `condition`, `context`); this
brief adapts the task, not the discipline.

**You are run as a subagent of the coordinator's session, so you are NOT blind**: the project's
`CLAUDE.md` and memory index reached your context. The skill's blind route (a fresh session outside the
repository, `claude -p`) is a paid run that writes outside the repository root, both the author's to
allow, and neither was asked. So **say in `context` whether any project rule, contract or memory
reached you and what it told you**: the synthesis records it, and a yes narrows what this report can
claim.

**The task.** Each sample is a GitHub issue as it would appear: a title, labels, a body, and the comment
that closes it. The files are produced by a **sketch** of the renderer, not by a finished one: a
`> sketch note` line tells you where the sketch had to guess, and you should treat those notes as the
sketch's own comments. Read each sample as **a stranger who found it in a search and has no access to
the repository**. For each, answer in your own words and then mark *found*, *not found* or *wrong*
against the text:

- (a) what was the problem; (b) what was the solution; (c) **in which commit** (quote the hash you
  read, or say there is none); (d) **was anything credited to somebody who solved it before, and who**;
  (e) what is noise to you or hides the answer.

Then, across all five: **the changes to the rendering you would make**, ranked, each tied to the line
of a sample that shows the need. Say what in a title would let you find the issue by searching, and what
in a body would make you stop reading. Sample 02's title is a path and sample 05's title is a log row:
say whether a reader could use either.

Your report is `docs/panel/212-reports/llm-ergonomist.md` in the lane tree.
