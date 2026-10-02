# Panel 187, the llm-ergonomist's brief: how it is run

Repaired 2026-10-02 after the completeness critic's first pass
(`docs/panel/187-reports/completeness-critic-briefs.md` § 3 and § 7 item 8);
the text it read is `llm-ergonomist-before-the-critic.md`.

The blind seat runs as fresh sessions outside the repository, never as a
subagent (`.claude/skills/panel/SKILL.md` § 2): **one session per program**,
so that no program's message is seen while another is repaired. Each runs in
a folder of its own, `<scratchpad>/rb1/` to `<scratchpad>/rb5/`, outside any
git tree, with no `CLAUDE.md` in or above it, holding `spec.md` (the trunk's
spec at the sitting's head, `cmp`-identical), and `brief.md` instantiated
from `blind/brief.md` in this directory with one program, what it should do
and what the sitting's compiler printed for it. The folder's path still
carries the scratchpad's own name, which encodes the repository's path; the
names `rb1` to `rb5` carry no sitting number.

The five programs, each with the class of open row it stands for
(`00-shared.md`, the classes (a) to (f)):

| | program (`blind/pN.hero.txt`) | what it should do | stands for |
|---|---|---|---|
| p1 | a C `for` header over `print(i)` | print 0, 1 and 2 | class (a): several messages for one habit (rows 131-33a, 54a, 54b, the instrument's `c-for`) |
| p2 | `function seven(): i64 )` over its body | print 7 | class (b), its easiest case: the hidden `)` is on the line the message points at |
| p3 | `function sum() -> i64` without a body inside `record Point` | print 3 | the shape `g2/r09`: a second message whose excerpt and caret are `function main()`, which has a body (the critic's (c) reading, Q4) |
| p4 | `x = [1, 2` over `print(x.len()) )` | print 2 | class (b), row 130-34a's shape, with an intent that settles Q2's two readings |
| p5 | `print(totl)` | print 3 | a control: one mistake, one message, a `certain` fix |

`p3` replaced, on the critic's finding that it was a second control,
`record Point )` over its fields, which is kept as
`blind/p3-before-the-critic.hero.txt` with its message.

**The task**: repair the program in ONE turn, from the message and the spec
alone, into `c.hero`. **The measurement is the coordinator's**: each
corrected program built and run on the sitting's compiler; a one-turn repair
is one that checks clean, builds, and prints what the program should.

**The paid run, named**: five `claude -p` sessions, each with the command of
`.claude/skills/panel/SKILL.md` § 2, `--model claude-opus-5-5`, each capped
at 3 USD by `--max-budget-usd 3`. Panel 186's three blind readings cost
0.48, 0.38 and 0.25 USD (each one session, one task), so the five are
expected near 2 USD in all; the bound is 15.
