# Panel 199, llm-ergonomist: the blind seat, run by the coordinator

Written 2026-10-08 from 07:57 by `date`, by the coordinator's hand, on the
completeness critic's first pass (its § *The blind seat's materials*). The
sessions never read this file. Run as fresh `claude -p` sessions outside the
repository, never as a subagent (`.claude/skills/panel/SKILL.md` § 2, the
paragraphs of 2026-09-30 and 2026-10-01), one session per task and variant so
no reading sees another.

**The approval**: the author approved the blind seat's paid sessions, **6 USD
in all**, between 23:40 and 23:43 on 2026-10-07 by the clocks read before and
after (carried from the coordinator).

## The folders

Under `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/199-blind/`,
one folder per task and variant, its label drawn at random at 07:57
(`random.SystemRandom().shuffle` per task, printed and copied here):

| folder | task | variant | holds |
|---|---|---|---|
| `r1-m` | 1, `p1.hero` | B | `spec.md`, `brief.md`, `p1.hero`, `B-PENDING.txt` |
| `r1-n` | 1, `p1.hero` | A | `spec.md`, `brief.md`, `p1.hero`, `output.txt` |
| `r2-m` | 2, `p2.hero` | B | `spec.md`, `brief.md`, `p2.hero`, `B-PENDING.txt` |
| `r2-n` | 2, `p2.hero` | A | `spec.md`, `brief.md`, `p2.hero`, `output.txt` |
| `r2-o` | 2, `p2.hero` | B, second reading of question 2, only if the sitting keeps two | `spec.md`, `brief.md`, `p2.hero`, `B-PENDING.txt` |
| `r3-m` | 3, `p3.hero` | A | `spec.md`, `brief.md`, `p3.hero`, `output.txt` |
| `r3-n` | 3, `p3.hero` | B | `spec.md`, `brief.md`, `p3.hero`, `B-PENDING.txt` |

`src/` beside them holds the sources (`spec.md`, `p1.hero` to `p3.hero`,
`p1-A.txt` to `p3-A.txt`) and is in no session's folder, nor is `runA/`, the
first version's build. Read at 07:57: `git -C 199-blind rev-parse` answers
*fatal: not a git repository*; no `CLAUDE.md`, `CLAUDE.local.md`, `.claude` or
`.git` in any ancestor up to `/` (`[ -e ]` on each); each `spec.md` equals
`git show 56def9b4:spec/heroes-spec.md` (`cmp`, seven of seven). A brief is
the same for a task's variants, so a session cannot tell which it holds.

**Variant B is not written.** It is the compiler-engineer's prototype's real
output on the program (its brief's item 6), in the compiler's own format, the
transcript stopping at `check` exit 1, no `certain` fix, and `spec.md` taking
the spec-warden's sentence or staying unchanged. Each B folder holds
`B-PENDING.txt` saying so; **it is deleted before that folder runs**, since it
names the sitting and the seat. If the adopted rule does not refuse p2 (question
2), `r2-m`'s output would be `r2-n`'s and it is not run.

**One residue this repair could not remove**: the parent's name, `199-blind`,
names the sitting and the seat, and a session is told its working directory;
panel 184 renamed its folders for exactly that (`docs/panel/184-briefs/llm-ergonomist.md`,
*The folders*). This repair was confined to that folder, so the coordinator
moves the run folders under a parent whose name says nothing of the sitting
before the run, or records why not.

**Moved at 08:11 by `date`**: the seven run folders are now
`<scratchpad>/readings/<folder>`, `src/` and `runA/` left in `199-blind/`,
which no session reads. Re-read the same minute: each `spec.md` equals
`199-blind/src/spec.md` (`cmp`, seven of seven), no `CLAUDE.md`,
`CLAUDE.local.md`, `.claude` or `.git` in any ancestor of `readings/r1-m` up
to `/`, and `git -C readings rev-parse` answers *fatal: not a git repository*.

## The tasks

Each states the program's intent; the program is the one the reader is told
was run, and `output.txt` is what was printed.

1. **p1**, *`bytes(s)` gives the bytes of the string `s`, in order, as an
   array, and `main` prints how many bytes `"hi"` has*: the self-call through
   UFCS on a name the author believes is a method. A: clang's warning in C
   words at `p1.hero:1:44`, the build at exit 0, `panic: stack exhausted in
   p1.bytes`, exit 134.
2. **p2**, *`fact(n)` gives the factorial of `n`, and `main` prints the
   factorial of 5*: the missing base case, and the discriminating case: clang
   is silent (the overflow check is a path that does not call `fact`), and
   whether the rule refuses it is question 2. A: the build silent at exit 0,
   `panic: stack exhausted in p2.fact`, exit 134.
3. **p3**, *`count(xs)` gives the number of elements of the array `xs`, and
   `main` prints how many elements `[4, 5, 6]` has*: the UFCS shape at `-O2`,
   `heroes run`'s level. A: clang's warning at `p3.hero:1:45`, `heroes build
   -O2` at exit 0, the binary printing nothing, still running after 10 s,
   stopped from outside, exit 124.

**How the A transcripts were made**, 07:53:11 to 07:53:30 by `date`: in
`<scratchpad>/199-briefs-work/blindrun/`, each program under its own file
name, by `t.sh` there with the frozen tree's compiler built from its seed
(the trunk's seed is the same file, `cmp`), Apple clang 21.0.0: `heroes check`,
`heroes build` (p3: `heroes build -O2`), the binary under `timeout 10` through
`head -c 4096`, each `(exit N)` the command's own status; the script's own
stderr was empty, so no harness line reached a transcript. The first
versions the critic read, `(exit )` empty, a `bash: ... Abort trap: 6` line
and `main.hero` for the file name, are `first-pass/p1-A.txt` and `p2-A.txt`.
In p3's transcript the last line, `(exit 124)`, is written in words, since 124
is `timeout`'s and not the program's.

## The run

From inside each folder, the skill's command with three departures named:

```
claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." \
  --restricted --safe-mode --strict-mcp-config --model claude-opus-5-5 \
  --tools "Read,Write" --allowedTools "Read,Write" \
  --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" \
  --max-budget-usd 0.85 --output-format json > run.json 2> run.err < /dev/null
```

- **`--model claude-opus-5-5`**, the sitting's model (the skill: *the `claude`
  command must support the sitting's model, or the seat runs on another*).
  The critic wrote that 2.1.285 accepting it was untested. Read between
  07:54 and 07:57 by the clocks before and after:
  `claude --version` is 2.1.285, `/opt/homebrew/bin/claude` a link to
  `Caskroom/claude-code/2.1.285`, dated 1 October; panel 196's script
  (`<scratchpad>/p196/blind-run.sh`) passed this flag on 2026-10-06 and
  `/tmp/b196-t1-A/run.json`'s `modelUsage` names `claude-opus-5-5`. So the
  same install ran it two days ago; today's run is unrun until it runs.
- **`--max-budget-usd 0.85`** in place of the skill's 3: **at most seven
  sessions at 0.85 USD, 5.95 USD, inside the 6 USD approved**; six without
  `r2-o`, 5.10; five if `r2-m` is not run either, 4.25. What it rests on:
  panel 196's nine sessions, one program each over the same spec, cost 0.17
  to 0.52 USD by each `/tmp/b196-t*/run.json`'s `total_cost_usd` (5 to 7
  turns, every one `success`), read in the same minutes; panel 184's three heavier
  sessions, three variants written in each, cost 1.66 to 2.22 USD (its brief).
  A session that meets its cap stops, and its `report.md` is what it had
  written, which is why the brief says *as you go*.
- **`< /dev/null`**, as panel 196's script had it, so no session waits on a
  terminal.

At most three at a time; no session starts before its `B-PENDING.txt` is
gone and its `output.txt` is in place. The coordinator copies each `report.md`
to `docs/panel/199-reports/llm-ergonomist-<folder>.md` with a header saying
how it ran, what it cost by its `run.json`, and its `context` answer; a *yes*
voids that reading until it is re-run.

## What is scored

From each `report.md` (`program`, `choice_points`, `confidence`, `context`,
panel 196's headings without `files`, since no task needs a C header):

- **the program**, run by the coordinator in a copy: `check`, then `build` at
  `-O0` and at `-O2`, each binary under `timeout 10` through `head -c 4096`;
  correct when `check` is 0 and both binaries print the key's line at exit 0;
- **whether `confidence` names the part of `output.txt` that told the reader
  what to change**, and which part;
- **`context`**: a *yes* voids the reading.

The keys, each run in `blindrun/` at 07:53 (`lv.sh`) with the frozen
compiler, `check` 0, `build` 0 with no warning, the binary at exit 0 at both
levels:

```
function bytes(s: str) -> [u8]
    out: [u8] @ []
    i: i64 @ 0

    while i < s.len()
        out @ out.push(s[i])
        i @ i + 1

    return out

function main()
    b = bytes("hi")
    print(b.len())
```

prints `2` (its canonical form by `heroes fmt`, re-run after the format);

```
function fact(n: i64) -> i64
    if n <= 1
        return 1
    return n * fact(n - 1)

function main()
    print(fact(5))
```

prints `120`;

```
function count(xs: [i64]) -> i64
    return xs.len()

function main()
    print(count([4, 5, 6]))
```

prints `3`.

## The copy

`docs/panel/199-briefs/blind/` holds `brief-task1.md` to `brief-task3.md`
(each identical to its task's folders' `brief.md`), `p1.hero` to `p3.hero`
and `p1-A.txt` to `p3-A.txt`; the B transcripts join it when written. Once
committed, those three programs are tracked `.hero` files a rule of this
sitting may refuse on purpose, as `docs/panel/173-briefs/`' two probes are;
the census at the landing reads them as such.
