# The blind seat's folders, as they are handed to the llm-ergonomist

**What the seat measures**: whether the shape in which `heroes check --json` hands a model a hole
changes what the model does with it, the information held the same. Three tasks, each a program with
one or two `???` (`variants/t1-*.md` to `t3-*.md`), each in three shapes of the compiler's answer:

- **T**, today's: exit 0, the hole's report as text on stdout, the stderr document with
  `"diagnostics": []`;
- **A**, route A: exit 0, nothing on stdout, the stderr document with `"diagnostics": []` and
  `"holes"`;
- **B**, route B: exit 0, a document of `"kind": "holes"` on stdout, the stderr document unchanged.

**The information is the same in the three shapes by construction**: one table in
`.claude/worktrees/rechecked-issues-scratch/blind228/gen/variants.py` renders all nine. It is the
compiler's real report (the lane's stage 2, `sources 451802aa66692eb3`, each program's `check
--json` kept in `.claude/worktrees/rechecked-issues-scratch/blind228/src/`) **less the line defect
2203 adds**, the binding a hole's own initialiser defines listed in scope (`q: Point` in t1's first
hole, `d: i64` in its second, `book: Item` in t2's), removed from all three shapes alike, since the
seat measures the shape and not that defect. The text shape is the text report's own form; the JSON
shapes are the proposal's members as the shared brief names them, written by the coordinator, no
compiler emitting them yet.

**What a right answer is**, each a reference program run on 2026-10-25 by the lane's `heroes run`:
t1 prints `25` (`q = Point(x: 3, y: 4)`, `d = dist2(a: p, b: q)`), t2 prints `40` (`book =
Item(name: "book", price: 15, qty: 2)`), t3 prints `ada!` and `bob!` (`print(shout(n))`). Each
answer is scored on three things: `finished` answered `no`; the program compiles under `heroes
check` strict; it prints what the task says.

**The model is Haiku 5.5**, the smallest of the three the trial pins and so the likeliest to miss a
hole a shape hides. **Three sessions per task and shape, 27 in all**, one per folder, each bounded at
`--max-budget-usd 1` and the sitting at the author's 3 USD of 2026-10-25; at three samples a cell it
is a reading, not a rate.

**The folders**: `build/w228/1` to `build/w228/27` in the lane (inside the repository's root, under
`/build`, which git ignores), each holding `heroes-spec.md` (the specification at `9a38f1b26`,
`git show 9a38f1b26:spec/heroes-spec.md`), `brief.md` (this folder's) and `task.md` (one variant),
nothing else. **The key** (which folder holds which variant) is `key.txt` beside this file, written
when the folders are made and never inside one.

**The command**, panel 227's (`docs/panel/227-briefs/blind/README.md`), which that sitting measured
blind where the skill's own was not:

```
GIT_CEILING_DIRECTORIES=<build/w228> ANTHROPIC_API_KEY=<from .env> claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Answer in English." \
  --bare --system-prompt "You work in the directory <build/w228/N>. You read its files with the Read tool, by absolute path." \
  --model claude-haiku-5-5 --restricted --safe-mode --strict-mcp-config \
  --tools "Read" --allowedTools "Read" \
  --disallowedTools "Bash,Write,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 1 --output-format json > run.json 2> run.err
```

**The seat's verdict is the experiment's result**, written by the coordinator in
`docs/panel/228-reports/llm-ergonomist.md` from the 27 answers and the checks, every number with its
command. The coordinator holds no veto on the seat's behalf; one is recorded only where the
experiment shows a route's shape making a model take an unfinished program for finished, or write a
program the compiler refuses, more often than today's.
