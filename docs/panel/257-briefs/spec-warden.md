# Panel 257, spec-warden

Read `00-shared.md` first, whole (its § 1 on **where the sentence goes** and § 5 on the arms bear on you most),
then `docs/panel/257-reports/critic-briefs.md` (the critic's first reading, which the coordinator has answered).
**Your agent file is the frozen commit's** (`.claude/agents/spec-warden.md` in your copy); under the author's
rule of 2026-11-22 (`691531908`) the budget is a cost to be measured and written, **not a veto**. Your copy is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/spec-warden/`; build its compiler and its
stage 2 there (§ 2). Your report:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p257/docs/panel/257-reports/spec-warden.md`,
written as you go; your own repaired texts beside it as `docs/panel/257-reports/spec-w*.txt` (the sentence that
goes **before** `No function of the program is a signal handler`, as `arms/arm-fN.txt` is) with their diffs
`spec-w*.diff`.

## The counts

- **Before**, read by the coordinator's stage 2: claude-legacy 9,585, cl100k_base 9,761, vendored maximum 9,761,
  **real 12,809** (`claude-opus-5`, the pin of 2026-11-22), headroom 3,575 against 16,384, the FFI floor
  mortgaging 60. **Re-read it yourself** (`./heroes-next measure spec/heroes-spec.md`) and say if it differs.
- **The arms' vendored counts** (§ 3 of the shared brief): f1 9,854, f1b 9,846, f2 9,848, f3 9,805 (cl100k_base,
  the maximum). **Re-read them.**
- **Real counts: at most FOUR `--refresh`**, each one count request and never a `claude -p` session (the
  author's rule of 2026-09-09: *the key counts tokens and does nothing else*). How: `cp spec/heroes-spec.md
  spec.orig` (and its `shasum -a 256`), apply the text to be priced (`patch -p1 < <diff>`, or write the file
  from your own), `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` (print `${#ANTHROPIC_API_KEY}`
  alone, **never the value**), `./heroes-next measure spec/heroes-spec.md --refresh`, then **restore** with `cp
  spec.orig spec/heroes-spec.md` and compare the `shasum` before any other command. Choose which four: the arms
  you judge viable and **your own repaired text of the route you would adopt, which must be one of them**. A text
  not priced by `--refresh` is written *a lower bound, vendored only*, in those words.

## What you judge

1. **Every arm's text** (`arms/arm-f*.diff`): does each sentence say one thing, with the words the document
   already uses (*abort*, § 6 line 202; *callback*; *handler*)? **Is *held running* / *not seen to end* a rule a
   reader can apply to a program they have not run?** The document says *the runtime* once and never *this
   runtime*; what does *"this runtime sees it return"* promise, and to whom? A sentence a reader can take two ways
   is a defect of the text: name it, repair it in your own `spec-w*.txt`, measure it. **Take the seats' measured
   findings as they come** (they run each clause, § 6; the critic measured the first issue, § 5.2): your report
   names which finding made you change a clause. Read their reports when they exist; you may not re-run their builds.
2. **The closed list.** `spec-shape.md` refuses *a closed list of aborts* (panel 087) because a running program
   falsified one. For each arm say whether its list is open or closed, and for each clause **name the running
   program that would falsify it**: the critic's p1 to p10 (`blind/programs/`, § 5.2's table) already falsified
   two clauses of the first issue; which clause of f1, f1b, f2, f3 would the *next* program falsify (C5's C++
   exception, C11's other thread, C12's `exit`, C8's coroutine jumps)? **A class (*not seen to end*) against a list
   of causes: which survives a falsifying program?**
3. **Where it goes, and what it costs the runtime.** The sentence goes before `No function of the program is a
   signal handler` (§ 1); line 428 keeps its end. Say whether you agree, and **price the alternative** the
   critic listed (the sentence part of `HERO_SIGNAL_REMEDY`, printed at the end of every signal refusal, 22
   goldens, `sigguard.c`). **Merging beats appending**: can the sentence MERGE into line 427's first clause
   (*"nor called by one"*)? Write the merged text and measure it.
4. **Scope.** Windows has no probe in the runtime (§ 4): the sentence is scoped *where the platform has signals*
   (Linux and Darwin) or is knowingly Windows-false, as line 427 itself is. Which, and what do the scoping words
   cost (measure)? **A clause true only on a route-A-free build** (*whatever memory*, *wherever the buffer lives*,
   § 4's glibc and architecture note) says so or is cut.
5. **Principle 0 and the payment.** `design.md` §1.2's cost formula and §1.6's payment rule are reached by grep,
   never from memory. The sentence adds no form: what is its Part 11 effect, or is it the author's order
   (2026-11-23) alone? Name a **removal** if one exists, or **register a prediction naming an instrument that
   exists** (`grep` the `spec`, `unseen` and `records` suites): the compiler's own test `selfhost/emit/callback_thunk.hero:354`
   holds the spec's *tail* to the runtime's words and **not the new sentence**; which instrument, if any, holds
   the sentence's nouns (*not seen to end*) to the words the runtime prints, and what a **new check** would be,
   priced as the work it is. The critic measured the `spec` suite on every first-issue arm: 31 passed, 4 failed
   (`spec/budget`, `spec/spendable`, `spec/real`, `spec/ledger`), the four pins the landing moves; confirm on your
   text.

## Predictions registered before the blind run

**Your report must hold the heading `## Predictions registered before the blind run`**, with your predictions
under it, **before the blind run starts** (its runner refuses to start without it and copies your report,
hashed, as it starts). **The run is NOT RUN in this sitting** (no paid run is authorized) and is registered:
read `257-briefs/llm-ergonomist.md` (the arms `now`, `adopted`, `rival`, the rows, D1 to D3) and the tasks and
answers in `257-briefs/blind/`; you may read them, and never a session's folder (none exists). **Predict per
candidate text** (`arm-f1`, `f1b`, `f2`, `f3` and your own), each a number and a range: for t1, **the right count
of 5 on each of p1 to p7** under `now` and under that text; for t2, the right count on questions 1 and 3 for A, B
and C; whether D1, D2 and D3 hold; the spend. Say which text you expect to be adopted.

End with the verdict structure of your agent file, then one line per numbered question: the text you would
adopt (its real count), and the one measurement that would change your mind.
