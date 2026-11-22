# Panel 254, spec-warden

Read `00-shared.md` first, whole (repaired after the completeness critic's first reading,
`docs/panel/254-reports/critic-briefs.md`; its findings 9, 13, 17, 28, 33 and 36 are in this brief). You judge the indicator (design.md §1.6, `grep -n '^### 1.6'
docs/design.md`: line 255; the ceiling *"must fit in 16384"*, line 257) and Principle 0's burden
(CLAUDE.md § 2), **under the rule committed at `691531908`**, which you read from the commit (`git -C
/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p254 show 691531908`) and which says, in
CLAUDE.md § Precedence:

> **Amended by the author 2026-11-22: the most robust solution now always wins**, a new form included and
> at the cost of the spec's size: Principle 0, the spec's budget (design.md §1.6) and the spec-warden's
> vetoes yield to it, and the ceiling is raised to fit.

and in your own definition, `.claude/agents/spec-warden.md`, at your vetoes: *"**Neither veto holds
against the most robust resolution** (CLAUDE.md § Precedence, the author's instruction of 2026-11-22):
there you report its cost, measured, and the ceiling is raised to fit it."* **So you report the route's
cost, measured, and what the ceiling must become; you do not veto it on the budget or on Principle 0.**
Where the text is wrong (a sentence that contradicts another, a rule a reader cannot apply), that is not
a budget matter, and you say so as an objection with the text that repairs it.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p254-scratch/spec-warden/`, a
detached worktree at `89f155330` (its `CLAUDE.md` and your definition are the frozen ones; the rule above
is read from the commit). Build its compiler as `00-shared.md` § 7 says. Read `.claude/rules/
spec-shape.md` there. **Your report**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p254/
docs/panel/254-reports/spec-warden.md`, written as you go. No paid run but the refreshes below; no
credential printed.

## FIRST: your registered predictions, before the blind run

**The coordinator starts the blind seat's runner only after your report holds your registered
predictions, and the runner checks it**: `run.sh` (`.claude/worktrees/p254-scratch/254-llm-ergonomist/
run.sh`, its warden gate) refuses to start unless your report holds, on a line of its own, exactly

```
## Predictions registered before the blind run
```

**with at least one non-empty line under it before your next `## ` heading**, and it copies your report
as it stands then to `254-llm-ergonomist/warden-at-start.md` and logs that copy's sha256: **only the
predictions in that copy score**. So write this section **first**, before any build, from this brief,
`llm-ergonomist.md` (its arms, rows and decision rules) and the tasks themselves,
`docs/panel/254-briefs/blind/task-t1.md`, `task-t2.md` (its five programs) and `task-t3.md`, each prediction a count the rules
score: for each cell (t1 `now`, t1 `m`, t2 `m` and `m0` per program, t3 `m`), how many of five sessions
you predict right, and which decision rule you predict holds. Panel 251's were written 13 seconds after
its runner started and were admitted only by a departure (`docs/panel/251-...md:253-258`).

## The measurements

The arms, made by `docs/panel/254-briefs/arms/make_arms.py` from the frozen spec (`00-shared.md` § 5):
`spec-now.md`, `spec-m.md` (the proposal), `spec-m0.md` (panel 251's text ported), at
`.claude/worktrees/p254-scratch/coord/arms/`, their sha256 in `docs/panel/254-briefs/arms/arms.sha256`.
**Vendored**, measured by the coordinator at about 01:42 with the frozen commit's stage 3 (`heroes-next
measure <arm>`, `--version` `sources 6e891e681c14bca0`): `now` **9022**, `m` **9488** (+466; the arm as repaired after the critic's
first reading, 9422 before), `m0` **9208** (+186), each the maximum of `claude-legacy` and `cl100k_base`; and `heroes-next measure
spec/heroes-spec.md` printed **real 11788** (`claude-opus-5, 2026-11-20`, the pin, carried and not
refreshed), headroom 4596 against 16384, *"the FFI floor mortgages 60 of it"*. Re-run each yourself.

**The real counts are yours to take**: **at most four refreshes**, `now`, `m`, `m0`, and the text you
would adopt if it differs from `m`. `--refresh` refuses every path but `spec/heroes-spec.md` and
`CLAUDE.md` (panel 253's brief, carried; check it), so copy each arm over your copy's
`spec/heroes-spec.md`, refresh, and restore it (`git -C <copy> checkout -- spec/heroes-spec.md`, then
`cmp` against `spec-now.md`). The key: `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` in
the one shell that refreshes, then `echo ${#ANTHROPIC_API_KEY}`, **its length and never its value**. Log
each refresh in your report: the time, the arm, the real count. **And say which model each refresh counts
with** (`selfhost/measure/pinned.hero:58`, `:85` name `claude-opus-5`) **and whether that is the
reader's tokeniser** when the blind readers run `claude-opus-5-5` (`run.sh`'s `--model`): unrun by the
critic, since it needs an API call; your refreshes may answer it, or say it stays a question.

## Your questions, in order

1. **The cost**: each arm's real and vendored count, the delta of `m` against `now`, and **what the
   ceiling must be raised to** for the route to fit with the headroom design.md §1.6 keeps (read it), as
   the rule of `691531908` asks. Say it as a number with its arithmetic.
2. **The text, sentence by sentence** (`arms/spec-m.diff`): does any sentence contradict another of
   the spec, the new UFCS sentence of § 9 against its own *"There are no methods"* first among them
   (panel 251's spec-warden found its M text contradicted itself there; read it by the critic's example,
   whether `m.drain()` calls the program's `drain(m)`), and the critic's findings 17 and 33: S3's wait
   against § 13's compile error for an end on every path; § 7's `==` (now amended); `(thread T)` called a
   handle while § 13 defines a handle as a group's record with a `tag` (`spec:442-447`), so its null, its
   `==` and a map key of it; `spawn_sized`'s result (now said); *"its type written where it is bound"*
   against § 5's *"inference is local only"*; does § 3's *"No aliasing
   exists among the values this language owns"* stay true with the address sentence amended; does § 13
   read whole with *"threads, "* and the doors' sentence gone, and with the stop sentence's *"or a
   mailbox's `receive`"*; does the lease sentence's *"a thread nobody waits for"* agree with S3; is
   each of S1 to S14 (`00-shared.md` § 5) said by the text, said wrongly, or unsaid. For each defect,
   the sentence that repairs it, measured.
3. **The names**: `spawn`, `spawn_sized`, `mailbox` taken (3 declarations today, `00-shared.md` § 5 item
   9); the words `thread` and `mailbox` as contextual words after a type's `(`; whether `heroes grammar`
   and the `grammar` suite (`.claude/rules/verification.md`'s map: the spec row) owe the `Prefix`
   production. And **spelling B with names nothing declares** (panel 251's critic's *"a spelling
   nobody priced"*): whether such names exist, counted, and what they would cost the text.
4. **Principle 0 and R3's reason 1**: R3's spec-warden veto had the condition W4, stated at
   `docs/panel/251-reports/spec-warden.md:92` and its veto condition at `:217-222` (the sitting's file
   only names it, `docs/panel/251-...md:270-272`); say what it is under the rule of `691531908`, and **whether you read the author's
   instruction of 2026-11-22 as the ruling on the need**, in one line, dated.
5. **The payment** (design.md §1.6's payment rule, `grep -n 'named removal' docs/design.md`): under the
   rule the ceiling is raised; say whether a removal or a registered prediction is still owed for the
   record, and which of yours pays.

## Your output

`.claude/agents/spec-warden.md`'s structure, and `00-shared.md` § 10: the measured deltas, real where
refreshed and vendored where not, each with its command; the predictions registered first; every premise
of these briefs you found false.
