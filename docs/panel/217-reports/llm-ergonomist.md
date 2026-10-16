# Panel 217, the llm-ergonomist's seat: the blind arms, run by the coordinator

Everything below is read from the arms' outputs in `llm-ergonomist-work/`, each program built and
run on this Mac.

## Arm R2, the second try under panel 215's R2 (run 2026-10-16, 22:06:15 to 22:07:35)

**How it was run.** Ten `claude -p` sessions by `llm-ergonomist-work/armR2/run_one.sh` (panel 215's
launcher, `--bare --restricted --strict-mcp-config --tools "Read,Write"`, `--max-budget-usd 0.7`,
with `--model claude-opus-5-5` added), five at a time, each in its own folder with `git init` under
the coordinator's scratchpad, outside the repository. A first launch at 22:05:39 ended in two
seconds at no cost: in `--bare` mode the CLI reads no stored login, and every session answered *Not
logged in* (`run.json`'s `result`); the second launched with `ANTHROPIC_API_KEY` from the
repository's `.env` in the environment, as panel 215's had. Each folder held the specification
`m-g-at` (sha256 `ac4ff3e2…`), `counter.h`, task C's words (`task.md`), the reader's own first
program from panel 215's re-run (`previous.hero`, from `215-reports/llm-ergonomist-work-2/r<NN>/c.hero.txt`)
and the lane's compiler's whole message on it (`compiler.txt`, its paths renamed from `c.hero` to
`previous.hero` and nothing else changed). As in panel 215's runs the Write tool did not reach the
sessions; each gave `c.hero` and `notes.md` in its answer, extracted unchanged (eight under a
`### \`c.hero\`` heading, r04 and r09 under `**c.hero**`). **Cost: 1.4139 USD** (the ten
`total_cost_usd`, 0.12 to 0.16 each).

**Context.** Seven notes say `context: yes`, each naming the environment's preamble alone (an agent
identity, the working directory, the date); three say `context: no`. None names a rule, memory or
repository status. The reading stands, as panel 215's did on the same answers.

**Judged** with the lane's compiler (R1 and R2, built from `selfhost/` at `f57af53da` plus R1's
uncommitted step), each `c.hero` built and run under a 20 s timeout:

| reader | builds | prints `6`, `4`, `5` | `doubled`'s signature | copies a lock |
|---|---|---|---|---|
| r01 to r10 | 10 of 10 | **10 of 10** | **`doubled(@c: Counter)` in all ten** | none (R2 would refuse it, and all ten build) |

**What it scores.** Panel 215's llm-ergonomist registered *with R2 and its message shown, at least 8
of 10 readers correct on task C's second try*: **10 of 10, held.**

**What it reads.** Every reader took the message's `guess` fix and the note's `@c`, the signature
nine of ten had refused as false on the first try. The notes say why in their own words: *the spec
has no read-only place lend, so `@` was the only option*; *Heroes has no word for const*; *nothing in
the spec expresses const*; and some justify the `@` by C's parameter, *the `@` does not change the
counter: `counter_value` takes a `const struct counter *`*. So R2 closes the copy, measured, and
leaves the program saying a change that does not happen, which is the sitting's question.

## Arm F1, the first try under the form (run 2026-10-16, 22:28:59 to 22:30:59 by the shell's clock)

**How it was run.** The same launcher, ten sessions, five at a time, each folder holding the
spec-warden's F1 draft over `m-g-at` (sha256 `8106db1e…700c50`, the draft the spec-warden's report
names, 10,227 real by its `--refresh`), `counter.h`, task C's words and the first-try brief (panel
215's, cut to the one program). Launched after the coordinator built the compiler-engineer's F1 key
with the prototype `heroes-f1` and it printed `6/4/5`. **Cost: 1.7700 USD.** The two arms together:
**3.1839 USD** of the evening's 10.

**Extraction.** Nine answers gave `c.hero` and `notes.md` under a heading as arm R2's did (r05
under `### c.hero`, without the code span). **r09 gave a first `c.hero` block of one line, `use
counter_h_unused_placeholder`, then wrote *Correction — that line is not part of the program. The
real file is:* and the program**: judged on the program the reader names as the real file, the
coordinator's choice, written here because panel 215's re-run had no correction to rule on.
Judged on the first block, r09 fails at `unknown_module`.

**Context.** All ten say `context: yes`, each naming the environment's preamble; none names a rule,
memory or repository status. **One limit, measured**: the working directory's path, which every
session is told, names the project and the seat, `…/-Users-joseph-Temp-heroes-heroes-lang/…/217-llm-ergonomist/armF1/r02`,
and r02 says so (*the path looks like a repository scratchpad*); panel 215's readers ran under
`.claude/worktrees/readers2/` inside the repository, whose path named it too. A path is no rule,
and the language is called Heroes in the specification itself; whether a reader who knows it is in
an experiment writes differently is unmeasured.

**Judged** with the prototype `heroes-f1`, each `c.hero` built and run under a 20 s timeout, every
signature and call read:

| reader | builds, prints `6`, `4`, `5` | `doubled`'s signature | its call | `counter_value` | copies an initialised lock |
|---|---|---|---|---|---|
| r01, r02, r04 to r08, r10 | yes | `doubled(c: Counter lent)` | `doubled(s.first)`, unmarked | `c: Counter lent` | no |
| r09 | yes, on its corrected program | the same | unmarked | the same | no |
| r03 | **no**: `ffi_lock_copied` at its set-up helper's `c @ Counter(n: 0, rest: zero)`, a fresh construction stored into the caller's place, which R2's positions refuse | the same | unmarked | the same | no |

**The spec-warden's two extra columns** (its report: a C function that writes, bound read-only,
builds under the prototype, p13 and p15): every reader bound `counter_init`, `counter_lock` and
`counter_unlock`, the three that write, with `@` (seven `@c: Counter lent`, three `@c: Counter`),
so **0 of 10** bound a writing function read-only; and **0 of 10** wrote `@c: Counter lent` on a
Heroes helper.

**Judged again with `heroes-f1f`** (sha256 `fe0128507a63…`), the compiler-engineer's last prototype,
which closes three holes `heroes-f1` has (a nested `@` in one call, a writable C header lent
read-only, a lock through a generic): the same ten verdicts, nine printing `6`, `4`, `5` and r03
refused at the same line, and the key still printing `6`, `4`, `5`.

**What it scores.**

- Panel 215's llm-ergonomist, *with a read-only lend, at most 2 of 10 copy the lock first try*:
  **0 of 10 copy an initialised lock; held.** (Counting r03's refused store as a copy, 1 of 10:
  held either way.)
- The spec-warden's (this sitting), *at least 8 of 10 build and print 6, 4, 5; at least 8 of 10
  write `doubled(c: Counter lent)`; at most 1 of 10 copies a lock*: **9, 10 and at most 1; held.**
  (8 of 10 build if r09's correction is not taken: still held.)
- The historian's (this sitting), *at least 7 of 10 write the read-only lend with an unmarked
  call, at most 2 of 10 copy the lock*: **10 and 0; held.**

**Against arm R2.** The same task, the same launcher and model: under R2 alone every reader reached
a correct program only on its second try, and every one through `doubled(@c: Counter)`, a
signature saying the function changes the counter; under F1 nine of ten reached it on the first
try, every one writing the signature that says it does not, and none copied a lock.

## Arm C, the control: the F1 specification, task C without the words that define the form (run 2026-10-16, 23:41:21 to 23:43:25)

**Why.** The critic's first pass found that task C's words, *a function that gives a counter's
value doubled, using `counter_value`, without changing the counter*, restate F1's own definition,
and its second pass named the control arm as what the resolution would rest on that nobody ran.
So the coordinator ran it, inside the evening's budget: the same F1 draft (sha256 `8106db1e…`),
`counter.h`, the first-try brief, and task C with *, without changing the counter* removed and
nothing else changed (`armC/task.md`, `diff` one line). **Cost: 1.8363 USD**; the three arms
together **5.0202 USD** of the evening's 10.

**Extraction.** Every answer gave one block per file; no correction.

**Judged** with `heroes-f1f`:

| | |
|---|---|
| builds, prints `6`, `4`, `5` | **9 of 10** |
| `doubled(c: Counter lent)` | **10 of 10** |
| its call unmarked, `doubled(s.first)` | **10 of 10** |
| copies an initialised lock | **0 of 10** |
| the three writing C functions declared `@c: Counter lent` | **30 of 30** (arm F1: 21 of 30) |
| refused | r06, `ffi_lock_copied` at its set-up helper's `c @ Counter(n: 0, rest: zero)`, the shape of arm F1's r03 |
| context | `yes` from all ten, each naming the environment's preamble alone |

**What it reads.** Without the words that define the form, every reader still reached for `lent`
on the read-only helper and wrote the call unmarked: the form is found in the specification, not in
the task. Two readers of twenty (arm F1's r03, this arm's r06) wrote a set-up helper that stores a
fresh construction into the caller's place, which R2's positions refuse with *this copies a
`Counter` … which C forbids to copy*; true of C, and silent on the repair, which is to build the
record where it lives and initialise it there. That message is owed a note of its own (design.md
§4.17). And the drop of `lent` from the writing C functions that arm F1 showed (three readers) did
not recur here (none), so it stands as a reading of one arm, not a measured shift of the word.
