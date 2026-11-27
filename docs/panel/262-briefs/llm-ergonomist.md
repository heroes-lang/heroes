# Panel 262, the llm-ergonomist: REGISTERED, NOT RUN, ITS PAID RUN NOT AUTHORISED

**Amended 2026-11-26, about 21:45, by the author's word** (*ok a 25 dollari*, in conversation): the paid run **is authorised, with a ceiling of 25.00 USD for the whole run**, never more. The ceiling of 25.50 USD proposed below for the 31 sessions is **lowered to 25.00 USD** (the runner stops before a session whose cap plus margin would pass it; the 21-session plan, 17.50 USD at the caps, stays within it). The lines below that say the run is not authorised, or that the ceiling is proposed, are history and stay; this paragraph governs. **Still owed before any session**: the labels' reliability, two independent readers on panel 259's wrong answers (the repair measured kappa 0.02 on W and 0.07 on H against its own keyword rule: `blind/labels-259/`), and the synthesis.

**Nothing in this file has been run, and no session of it is started by a seat, by the critic, by the clerk, by the
repair or by the synthesis's writer.** **The paid run is not authorised**: the author authorised 20.00 USD for panels
259, 260 and 261 only (the coordinator's brief to the clerk, 2026-11-26). **The run waits for the author's yes, which the
coordinator asks for**, with the size, the per-session caps and the ceiling below; until that word, the sitting's
resolution is provisional on a reading that has not been made, and the synthesis says so. Written 2026-11-26 by the
sitting's clerk at the base `aded91388`; **repaired the same day by the repair, at the base `87003cb42`** (through `c18a9a9a3`), from the
critic's first reading (its C13 to C19 and C24, its repairs 3 and 7: `00-shared.md`, the head); its inputs, their
hashes, the key, the labels and the decision rules are `blind/registration.md`.

## What it measures, and why the sitting wants it

The sitting was convened by a reader's finding, not a builder's: panel 259's blind run found 3 of 5 `now` readers of
t1 and 4 of 5 of t2 reading the handler sentence's *"may abort"* as covering **every** switch C makes, and panel 257's
found 10 of 10 readers of its `adopted` and `rival` taking *"may abort"* as *"aborts"* (`00-shared.md` § 2 question
1). **Two opposite misreadings of one word**: the first makes a reader give up on a coroutine library that is
promised to run; the second makes a reader predict a correct program refused. **And at least four more** the repair's
registration can now see: the list read as exhaustive (L, defect 2848's), the sentence read across threads (T), the
pronoun (I, 2 of 5 of 259's `now` t1 readers, measured in its archive), and *may* read as a platform split (P); with
the room's abort (R) and § 9's stack (S) as reasons readers already give. Only a reader can measure which a text
leaves, and no builder seat can. The seat reads **only a copy of the specification**, the task and its programs
(panel 175 item 7: never `design.md`, never the repository).

## The arms (`blind/registration.md` § The arms)

`now` (the spec at `87003cb42`, the same bytes as at `aded91388`, sha256 `3df14254...d93f`; **panel 261's text landed
after the base**, so unless the coordinator names the base's spec, `now` is the landed one, 526 lines, sha256
`9ff7d2a1...2fc3` at `main` `0bde00d84`: `blind/registration.md` § The arms), `adopted` and `rival` (the
synthesis's two texts, each written with its diff and hash into the registration **before any session**). Three,
because a text that cures one misreading may invite another; two (`now`, `adopted`) if the author authorises less.

## The tasks (each written or assembled by the clerk and the repair, label-stripped; hashed in `blind/inputs.sha256`)

- **t1, thirteen programs whose C handles a signal** (`blind/task-t1.md`, amended by the repair; `programs/prelude.h`,
  `sw.h`, `h1.h` to `h13.h` and their `.hero`): a handler left by `siglongjmp` (`h1`), by a jump in assembly (`h2`,
  its first line no longer quoting `now`), by its return (`h3`); a handler that calls the function (`h4`); a handler
  that `swapcontext`s away for good (`h5`); a jump within the handler (`h6`); a `siglongjmp` onto memory a returned
  `makecontext` task once used (`h7`, **inside the sentence's third exit, and it runs**); a handler's `siglongjmp` into
  a live `makecontext` coroutine (`h8`); **the repair's five**: a handler left by assembly on a thread `spawn` started,
  the callback on the program's own thread (`h9`, **inside the first exit by its letter, and it runs**: T); a handler on
  an alternate stack left by `siglongjmp` into a `makecontext` coroutine (`h10`, 2707's, **runs**); a handler on the
  coroutine's own stack left by `setcontext` to the coroutine (`h11`, 2702's, **runs on route C**); a handler's
  `siglongjmp` into a live coroutine of an assembly switch never given to `makecontext` (`h12`, **none of the five
  exits, and it aborts**: L's program); and a signal the program counts with `hero_signal_watch`, then a callback
  (`h13`, the remedy's own handler, **runs**). Six of t1's programs are panel 257's registered or folder programs, five
  byte for byte and `h2` but its first line, so the two runs compare.
- **t2, four programs whose C switches with no signal handler at all** (`blind/task-t2.md`; `co.h`, `sw.h`, `esc.h`,
  `th.h`, `n1` to `n4`): a `makecontext` coroutine (`n1`), an assembly-switched coroutine leaving for good by `longjmp`
  on heap stacks (`n2`) and on carved stacks (`n3`, defect 2855's program, refused by defect at the clerk's base and
  **running at this one**, step 199d having repaired 2855), and a callback on a thread C made (`n4`, refused in words no sentence names). **t2 is where
  259's widening shows**: a reader who takes the sentence as covering every switch answers `NOT PROMISED` for `n1` and
  `n2`, which run.

Each answer is a verdict (`RUNS`, `ABORTS`, `NOT PROMISED`), the abort's sentence and its remedy, a confidence, the
sections it rests on, and `choice_points` (`blind/brief-session.md`).

## The key (`blind/registration.md` § The key)

**This Mac's rows are run** (the repair's stage 2 at `87003cb42`, `sources 6ce3f9e116d22e3a`, the plain build's key,
three runs of three, plain and `--sanitize`, every cell one output of three; the clerk's 72 cells at `aded91388`
reproduced byte for byte at `c18a9a9a3`, and every cell the same at `87003cb42` but `n3`'s): `h1`, `h3`, `h7`, `h9`,
`h10`, `h11`, `h13`, `n1`, `n2`, `n3` run; `h2`, `h4`, `h6`, `h12` abort in STANDING's words, `h5` in SWITCHED's, `h8`
in ONSTACK's, `n4` in the thread's. **The Linux arm64 rows were run by the repair at the base**, 34 cells, every one
this Mac's (`blind/registration.md` § The
key on Linux arm64). **Both are owed again on the run's day**, under that day's build: `main` will have moved (the lane
sig-family has 2852 and 2853 left). **A program whose two platforms or two builds give different verdicts has no
key.**

## The decision rules (registered there, before any session)

**D1, no widening**: under `adopted`, `h1`, `h3`, `h13`, `n1`, `n2`, `n3` each at least 4 of 5 build-right and at most 1 of 5
`NOT PROMISED`. **D2, no narrowing, its set fixed before any session and the same under every arm**: `h7`, `h9`, `h10`,
`h11`, each at most 1 of 5 `ABORTS` under `adopted` (the clerk's set was `h7` alone and depended on the arm's
classification, so a silent text emptied it: the critic's C15). **D3, the aborts and their words**: under `adopted`,
`h2`, `h5`, `h6`, `h8` each at least 4 of 5 build-right or text-right, every abort naming the handler not seen to end
or a call from inside one. **D5, the list not read as exhaustive** (the repair's): under `adopted`, `h12` at most 1 of
5 `RUNS`. **D4, reported**: `h4`, `n4`, and **every wrong answer's labels** (W, N, L, H, T, I, P, R, S, O,
several per answer, a wrong `RUNS` included), graded by two graders apart, their agreement per label. D1 or D2 failing
for `adopted` and holding for `rival` moves the resolution to `rival`; failing for both reopens the text; D3 or D5
failing amends it. The spec-warden's predictions are copied and hashed as the run starts.

**The labels were tried on panel 259's archived answers, free** (`blind/registration.md` § The labels, tried): 48
wrong answers in 13 sessions, a reader (the repair) against a rule registered before it read them: same label set on 2
of 48, kappa 0.02 on W, 0.07 on H, 0.00 on S. **A label is a reader's**: the registration now grades the reason that
decided an answer, and **two readers apart are owed before the paid run, free**, the repair having had no second reader
to give.

## The size, the spend and the cap (for the author's yes; nothing here is authorised)

- **Size**: **31 `claude -p` sessions** (t1 and t2, three arms, five each, and one verdict), or **21** for two arms.
- **The caps per session** (repaired for thirteen t1 programs): **1.00 USD** each t1 session, **0.60 USD** each t2
  session, **1.50 USD** the verdict, margin **0.10**; at the caps the 31 cannot pass **25.50 USD**, the 21 cannot pass
  **17.50 USD**.
- **The ceiling proposed**: **25.50 USD** (31 sessions) or **17.50 USD** (21), the caps' own bound, the runner stopping
  before any session where `spent + cap + margin` would pass it; **the author's to set**.
- **The price, an inference, unrun**: panel 257's t1 sessions cost 0.16 to 0.24 on 6,485 bytes of t1 input, panel
  259's 0.34 to 0.47 on 17,929 (its verdict 0.93); this run's t1 inputs are **23,864 bytes** and thirteen programs, t2's
  9,522 (`wc -c`): **about 13 to 18 USD for the 31, 9 to 13 for the 21**.

## The area, and every session

`blind/registration.md` § The area, the runner and every session: the folder
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/262-llm-ergonomist/`, drawn at the synthesis and not before,
an empty git repository of its own; the runner of panels 259 and 261, adapted by whoever starts the run with every
change registered as a diff before any session; the command of `.claude/skills/panel/SKILL.md` § Procedure with
`--bare` and `--model claude-opus-5-5`, `< /dev/null`, `HOME`, `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside the area, the
free preflight first, the three success fields, the scan. `claude --version`: `2.1.323 (Claude Code)` on this Mac
(the clerk, about 14:50; carried).

## What every session must say (the brief asks it, in `blind/brief-session.md`)

Under `answer`, each program's `verdict`, `abort`, `confidence` and `sections`; whether any project rule, contract or
memory reached its context (`context`; a yes voids the session); and **every place where the specification left it a
choice, the choice made, and what the other would produce** (`choice_points`), never its hesitations or its tools.
