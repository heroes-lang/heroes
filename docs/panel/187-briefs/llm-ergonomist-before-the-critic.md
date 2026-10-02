# Panel 187, the llm-ergonomist's brief: how it is run

The blind seat runs as a fresh session outside the repository, never as a
subagent (`.claude/skills/panel/SKILL.md` § 2): a folder of its own,
`<scratchpad>/187-blind/`, outside any git tree, no `CLAUDE.md` in or above
it, holding `spec.md` (the trunk's spec at the sitting's head), five short
programs of the open rows' shapes (`p1` the C loop header, class (a); `p2` a
function head `(): i64 )` over its body, two mistakes hidden behind one
message, class (b); `p3` `record Point )` over its fields; `p4` the shape of
row 130-34a; `p5` a control, one mistake and one message), each with what it
should do and what the trunk's compiler printed for it, and `brief.md` (a
copy is `blind/brief.md` in this directory, the programs as `blind/p*.hero.txt`
and the messages as `blind/p*.messages.txt`).

**The task**: repair each program in ONE turn, from the message and the spec
alone, into `c1.hero` to `c5.hero`. **The measurement is the coordinator's**:
each corrected program built and run on the trunk's compiler, a one-turn
repair being one that checks clean and prints what the program should. That
is what the routes of Q1 and Q5 disagree about: whether a hidden mistake
(class (b)) costs a model a second turn where a second message (class (a))
costs it nothing but noise.

**The paid run, named**: one `claude -p` session with the command of
`.claude/skills/panel/SKILL.md` § 2, `--model claude-opus-5-5`, capped at 3
USD (the sitting's two earlier readings cost 0.25 to 0.48 USD).
