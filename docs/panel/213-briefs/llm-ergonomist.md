# Panel 213, the llm-ergonomist's brief

**You are run as a subagent of the coordinator's session, so you are NOT blind**:
the project's `CLAUDE.md` and memory index may reach your context. The skill's
blind route (a fresh `claude -p` session outside the repository) is a paid run
that writes outside the repository's root; the author chose on the evening of
2026-10-12 to run this seat as panel 212 did, non-blind and recorded. So say in
`context` whether any project rule, contract or memory reached you and what it
told you.

**Read only these files**, copies kept away from the tree's `spec/` so its rule
files do not load, by absolute path:

1. `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-213/llm-ergonomist/heroes-spec.md` (the language);
2. `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-213/llm-ergonomist/symptom.txt`;
3. `.../llm-ergonomist/frame-1.txt`, `frame-2.txt`, `frame-3.txt` in the same folder:
   three renderings of ONE stop of ONE program, label-stripped, in no meaningful order.

Do not read any other file of the project. Your method and output structure are
your agent definition's (`verdict`, `experiment`, `hesitation_points`,
`argument`, `prediction`, `condition`, `context`).

**The task**, once per frame and before you compare them: from the symptom and
that frame alone, say what is wrong in the program and which value shows it,
how many lines of the frame you read to find it, and what you had to guess
(a name, a type, a value's shape). Then compare: which rendering lets a reader
of the spec find it soonest and surest, which line of each misled you or cost
you, and what a rendering would need to drop or add. Write your report to
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-213/docs/panel/213-reports/llm-ergonomist.md`.
