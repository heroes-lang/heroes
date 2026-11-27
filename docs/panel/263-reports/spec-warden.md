# Panel 263, the spec-warden's report

Written as it went, 2026-11-27 from 11:14, in my copy `.claude/worktrees/263-spec-warden` (`git worktree add --detach
.claude/worktrees/263-spec-warden f9f42fcc4`, 11:14, `git status --short` empty), scratch
`.claude/worktrees/263-spec-warden-scratch/` (`TMPDIR` there). No sub-agent, no commit, no GitHub write, no credential
printed, nothing outside the repository's root; one paid request, the count below. I ran no reproducer program: every
outcome of `malloc.hero`, `static.hero`, the canary and the 2871 cases this report names is the brief's or the critic's
run, cited, never mine.

## What I ran, and what I read, apart

**Ran** (this Mac; `uptime` 66.37 / 59.20 / 42.82 at 11:18 and 50.66 at 11:22, 8 cores, other seats building: every
duration below is one nobody should quote):

- **the seed's compiler**, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `real 56.58 user 23.75`, exit 0,
  **`sources 1e1eb23a1927d35b`**, the brief's; **stage 2**, `./heroes build selfhost/main.hero -o heroes-next`: `real
  465.82 user 215.15 sys 45.70`, exit 0, **`sources 50e8403d5a885325`**, the critic's;
- `wc -lc spec/heroes-spec.md`: **526 lines, 36,316 bytes**; `shasum -a 256`: **`736b534c852e39af…efd3`**: the brief's;
- `./heroes measure spec/heroes-spec.md` (seed's): **10,374 / 10,563** and `STALE … 24de425e6c28e69b … 25b307910fc3ea19`,
  the brief's; **`./heroes-next measure spec/heroes-spec.md`** (stage 2, no request): the same two rows and **`real 13849
  claude-opus-5, 2026-11-26`** read from the pin, and the line *"Headroom: 2535 against the 16384 ceiling — but the FFI
  floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is 13909"*: **headroom 2,475**, as the
  critic printed. The stage-2 build prints otherwise nothing the brief did not say;
- every text below applied to a copy of the spec in my scratch (`texts/apply.pl`: each anchor asserted to match exactly
  once, `grep -c` 1 for each of the six), and measured offline by the seed's compiler: the vendored columns below;
- **the one paid request, 11:23:45** (`cp spec/heroes-spec.md ../263-spec-warden-scratch/spec.orig`, its `shasum -a 256`
  `736b534c…efd3`; `spec-w1.md` copied onto `spec/heroes-spec.md`, `f372f420b838493f…9d435`; `set -a; .
  /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` inside a subshell, **`key length 108`** printed and nothing else of
  it; `./heroes-next measure spec/heroes-spec.md --refresh`, exit 0; `spec.orig` copied back, `shasum` **`736b534c…efd3`**,
  equal, before any other command; `git status --short` then: `?? docs/panel/263-reports/` alone). It printed:
  **`SPEC_REAL_TOKENS 13859`, `"claude-opus-5"`, `"2026-11-27"`, `SPEC_DIGEST "4bf3cc3c13e0df6e"`**
  (`../263-spec-warden-scratch/refresh-w1.txt`). **No text for 199j's stop was in it**: my answer there is *no sentence*
  (item 4), so the request priced the main text alone, as the coordinator's call of 10:57 says;
- **the `spec` suite with w1 applied** (`./heroes-next run tests/harness/main.hero -- ./heroes-next spec`, the spec
  restored after, `shasum` equal): **32 passed, 4 failed**, `real 200.10 user 98.02`, exit 1; the four are `budget`
  (*"measures 10569 and the record says 10563 — a delta of 6"*), `spendable` (*"is 13909"*), `real` (*"13849
  claude-opus-5, 2026-11-26"*) and `ledger` (*"newest row says 10563 … measures 10569"*): **the four pins and the
  ledger's row, which a landing's paste moves, and nothing else** (`refusal-words`, `shape`, `anchors`, `offered`,
  `named`, `rejected`, `inventory` among the 32 green; `../263-spec-warden-scratch/spec-suite-w1.txt`);
- **the net's own tests with w1 applied** (`./heroes-next test tests/harness/main.hero`): **5 of 918 `ok`, 0 failed,
  then stopped by me by process id** (`kill` of the test binary, its child compiler and `heroes-next`, 11:32; `real 262.43`,
  exit 143), the sixth test building for over two minutes at load 50 beside a gate's own net tests
  (`gate-push10`, never touched) and the other seats' runs; the spec restored by the same command chain, `shasum`
  `736b534c…efd3`. **Unrun past 5 of 918**: no harness file moves under w1, and the `spec` suite's 32 other checks
  are green with it, which is an inference about the rest and not a run (`../263-spec-warden-scratch/nettests-w1.txt`);

**Read** (not run): design.md §1.2 (`docs/design.md:192`), §1.6 (`:255`, the ceiling **16384** on `claude-opus-5`
through `POST /v1/messages/count_tokens`, `:257-258`; the payment rule `:320-341`), §1.12 (`:646`), Part 6's Coroutines
row (`:3611`, `:3642`) and Part 7's (`:4184-4192`); the spec whole; `.claude/rules/spec-shape.md`; panel 232's K1 and R4
(`docs/panel/232-*.md:171-174`, `:244-251`); panel 258's R1 (`docs/panel/258-*.md:38-47`); panel 259's R1 and R2
(`docs/panel/259-*.md:63-80`, `:128-156`); panel 262's findings, table, R2, landing list and verdict
(`docs/panel/262-*.md:78-97`, `:145-165`, `:215-240`, `:270-278`, `:433`, `:575-591`) and its adopted text;
`runtime/parts/sigguard.c:590-622` (199j's stop), `runtime/parts/thread.c:128-150` (the thread refusal),
`runtime/parts/frame.c:81`, `:128-146` (the 1 GiB room and its words); `tests/harness/spec_refusals.hero:1-80`;
`tests/golden/run/fixedbugs-2871-*` (the header's seven shapes and the seven `.expected`), `fixedbugs-2870-*.h:35-37`;
step 199j's commit body; the critic's first reading.

## Where the briefs, and the record they cite, are false or silent

1. **Panel 262's verdict says *"R2's three repairs stand on `main`: … `so` by step 199i (`94c2266a4`)"***
   (`docs/panel/262-*.md:588-589`). **It is true of half of `so`.** 262's `so` was a 256 KiB `makecontext` stack *"from
   `malloc` or `mmap` with a guard page"* (`:95-97`; `:433`, *"`malloc` and `mmap` exit 138"*); step 199i's four goldens
   are all on `fixedbugs-2870-a-coroutines-guarded-stack.h`, an `mmap` with `mprotect(map, page, PROT_NONE)` (`:35-37`),
   and this sitting's defect is `so`'s `malloc` half, open. So **R2's condition (a) is unmet for `so`** (item 5). The
   brief cites the verdict and does not say so.
2. **Item 4 says the stop has *"no callback in it"***: true of the stop (it is no callback's abort), but the seven 2871
   cases do call one: each `.expected` that runs prints *"answer ran for N"*, the header saying *"No function of the
   program runs in a signal; it is called after, on the thread's own stack"*. It matters to item 4: the refused cases sit
   under § 13's handler sentence's *"may abort"*, by its letter (below).
3. **Silent: the room's abort words.** w1 puts a stack into the 1 GiB count, and the words that abort at the bound today
   are *"stack exhausted: the room large values take off the stack is full, 1 GiB for the whole program"*
   (`runtime/parts/frame.c:128`, `:136`). Under w1 they name no stack, so a program stopped by its held stacks would be
   told a cause it does not have (design.md §4.17). The landing owes the words, not only the sentence.
4. **Silent: lane land262 and any text here meet on one physical line.** Line 438 holds both panel 259's held-memory
   sentence (w1's) and panel 257's handler sentence (262's R1 replaces it, w5's neighbour); git merges by line, so
   whichever lands second re-applies its text by its anchor, never by a merge's resolution by hand.
5. **Silent: § 1.6's consolidation budget** (*"spend at most 1,500 real tokens before the thesis harness has run"*,
   `docs/design.md:293-296`): whether it still binds, the harness having landed at M-thesis-harness, is a question I did
   not settle; it changes no verdict here (+10 real).
6. The brief's *"headroom 2,475"* and its *"13,849 is a pin read"*: both stand, run above.

## The texts, and their prices

Each is beside this report as `spec-w<n>.txt` (the sentence as it would stand) and `spec-w<n>.diff` (against
`spec/heroes-spec.md` at `f9f42fcc4`). Base: claude-legacy 10,374, cl100k_base 10,563, real 13,849 (the pin of
2026-11-26 for digest `25b307910fc3ea19`, which is this file's: stage 2 prints the row, not `STALE`).

| text | the route it answers | where, and what it says | claude-legacy | cl100k_base | real, `claude-opus-5` |
|---|---|---|---|---|---|
| **w1** | **(b)**, and (g) | § 13, panel 259's held-memory sentence: *"…, or holding a `partial` record, a buffer they lent to C **and a stack this runtime gave them** stay held until the program ends, counted with the live ones against 1 GiB …"* | 10,380 (+6) | 10,569 (+6) | **13,859 (+10)**, counted 2026-11-27 |
| w1b | (b), its stacks bounded apart | § 13, the same sentence's tail: *"…, and a callback C calls where the system has no stack to give it aborts, saying so."* | 10,394 (+20) | 10,583 (+20) | a lower bound, vendored only |
| w2 | (a), (a4), (c), (e), (n): stacks the runtime saw made | § 9: *"running out of stack aborts, but a callback C calls on a stack this runtime did not see C make may first write over what lies below that stack."* | 10,399 (+25) | 10,588 (+25) | a lower bound, vendored only |
| w3 | (l), (q), (m), and (d), (i), (o), (p) alone: the writes stay | § 9: *"…, but a callback C calls on a stack of C's with nothing of no access below it may first write over what lies below, and the program may run on."* | 10,407 (+33) | 10,596 (+33) | a lower bound, vendored only |
| w4 | (f): a new abort | § 13, after the `atexit` example: *"A callback C calls on a stack with no page of no access below it that this runtime can see (one C took from `malloc`, say) aborts before it runs, saying so."* | 10,413 (+39) | 10,602 (+39) | a lower bound, vendored only |
| w6 | (h) | § 13, after the held-memory sentence: *"A stack C hands `makecontext` is swapped for one of this runtime's own of its size, held until C frees that block where this runtime sees it and else until the program ends, counted against the same 1 GiB."* | 10,422 (+48) | 10,611 (+48) | a lower bound, vendored only |
| w7 | (b), if shape C breaks | § 13, after the `atexit` example: *"C calls a callback on a stack of this runtime's own, so C that reads its stack inside one (to copy or scan it, say) reads that stack."* | 10,408 (+34) | 10,597 (+34) | a lower bound, vendored only |
| w5 | step 199j's stop (not adopted) | § 13, after the handler sentence: *"Where a signal handler of C's interrupted a `swapcontext` or `setcontext` before it loaded its target, a switch the handler makes naming either context of that one aborts the program before it switches, saying so, where this runtime saw both."* | 10,426 (+52) | 10,615 (+52) | a lower bound, vendored only |

**w1's +10 real is against a pin of yesterday, not a count of today**: the base was not re-counted (one request), so the
delta rests on the endpoint giving one count for one content a day apart, an inference. Measured against the ceiling as
the instrument does it, w1 reads **13,919 with the FFI floor's 60, headroom 2,465**.

## 1. § 9's *"running out of stack aborts"*, route by route

- **(b)**: **true as written, on one condition, and no word of § 9 moves.** Every frame of the program, and every C frame
  a callback calls, then runs on a stack the runtime made with its own guard, whatever stack C entered on: `malloc`, a
  static array (shape A), one no wrapper saw (boost.context, a hand switch), and a recursion through C (shape D), which
  after its first entry stays on runtime stacks. What C's own frames do on a stack of C's is C's, as any C overrun is: §
  13 promises an abort of C's reach only past a lend (`counted_by`). **The condition** (the critic's item 6): the thunk
  switches before it writes to C's stack anything but a bounded frame of its own. Today `hero_callback_entered` →
  `hero_callback_asked` → `hero_sig_inside` / `hero_rebind_inside`, and a refusal's `hero_signal_stop`, run on C's stack
  before anything (`sigguard.c:912-990`, read by the brief); left so, a callback entered with fewer bytes left than those
  frames take writes below C's stack before any guard, and § 9 is false for that entry. A nested entry must know it is on
  the runtime's stack already. **And Windows**: § 9 names no platform, so (b) owes Windows a switch too, or the sentence
  is false there for a callback on a hand-switched block; as recalled, not read, a switch that leaves the thread's stack
  limits unmoved breaks `__chkstk`, so Windows' (b) would be a fibre (a question for the ffi-pragmatist; Windows by
  reading, a choice).
  **And shape C, which (b) may break where today runs** (`00-shared.md` § 5 item 6, read, unrun by anyone): a copy-stack
  library (libaco, libco's shared stack, greenlet, as recalled) saves a coroutine by copying its block from the stack
  pointer up; a callback that yields through it under (b) leaves that pointer on the runtime's stack, outside the block,
  so the library copies an extent it never owned; a collector scanning its thread's stack meets the same. Unrun: the
  ffi-pragmatist's to run. If it breaks, (b) corrupts a program that runs today, and either the route leaves such a
  stack alone (then § 9 needs w2 for it) or the spec tells binders where a callback runs, **w7**, +34 vendored, a lower
  bound.
- **(g)**: true only where (f)'s judgement is: a page below that is unmapped at the entry and mapped later (shape B,
  `00-shared.md` § 2.6) passes the probe and is written. **w2's qualifier plus a word for a mapping made after the
  entry**, or (g) widened to (b).
- **(h)**: reaches the stacks made through the wrapper; a static array handed to it is reached, one no wrapper saw is not:
  **w2**, and (h) owes w6 besides (item 2).
- **(a), (a4)**: seen stacks only, and only where a whole page lies inside the block (a 16 KiB block on this Mac's 16 KiB
  pages may hold none): **w2**, plus *"or one too small to hold a page of its own"*; a static array handed to the wrapper
  is reached (its lowest whole page made `PROT_NONE` is the array's own).
- **(c)**: Heroes frames on seen stacks; the C frames a callback calls run below the last check (shape D): **w2**, plus a
  word for C called from a callback. **(e)** alone closes nothing; **(n)** reaches bounded trees only: each **w2** at
  least.
- **(f)**: § 9 true by refusing (w4), **except shape B** as for (g): the probe at the entry is a premise about a page the
  next mapping may take.
- **(l), (q), (m)** alone (and (d), (i), (o), (p) alone): **only w3 is true**, and w3 says in the spec that a program may
  write over memory it does not own and run on: a confession of §1.12, rank 3 of CLAUDE.md § Precedence.
- **Every route that leaves a stack a callback can recurse on unguarded, with § 9 unmoved: veto**, on soundness: the
  sentence promises what the defect's own program falsifies (`malloc -O0`, `CO_N` 30,000, exit 0 with about 5.24 MiB
  written below, `00-shared.md` § 2.2; `static.hero`, exit 139 in Linux arm64, § 2.5). Panel 232's R4 landed the sentence
  on *"a frame past the guard may write rather than die"* being closed; it binds the same here.

## 2. § 13's held-memory sentence

Under (b) a runtime stack is held for each callback suspended or abandoned; none of the three things panel 259's sentence
names. **I adopt w1** (13,859 real): one bound, one abort, one told end, as 259's amendment made the lends join the room
(*"the abort that ends a loop of abandonment is the told one whatever was abandoned"*, `docs/panel/259-*.md:73-75`). *"a
stack this runtime gave them"* says, at the price of the article, that C calls back on a stack of the runtime's own where
it gives one (the fact a binder of a stack-scanning C library needs), and is true of a hybrid (b) that switches only off
stacks it cannot guard. **Its cost, unmeasured**: counted by mapped bytes, the bound refuses a correct program holding more
than 1 GiB ÷ S callbacks suspended at once (S the stack's size: 4,096 at 256 KiB, 128 at 8 MiB), live or abandoned, which
runs today; so the count, S, and how many suspended callbacks a correct program reaches are the seats' to run. **w1b**
instead bounds them apart with no count, at a told abort when the system has no stack to give, which a lazily committed
`mmap` may never meet before an untold kill (Linux's OOM killer, as recalled, a question): less robust, so second. **Under (h)**: w6, a stack per
context, not per callback, held where C's `free` is unseen: it refuses a correct program that makes more than 1 GiB ÷ S
contexts over its life, which runs today; **I object to (h)** on that and on C reading its own block. **And the words**:
w1 owes `frame.c`'s abort words a stack (§ Where … silent, 3).

## 3. A new abort: route (f), and (e) where it refuses

**w4**, +39 vendored, a lower bound. Home: § 13's callback paragraph, after the `atexit` example, on `main`; once lane
land262 lands, merged into 262's thread clause, which is the cheaper home (*merging beats appending*): *"and a callback C
calls on a thread of its own, or on a stack with no page of no access below it that this runtime can see, aborts, saying
so"*. It refuses every callback C calls on such a stack, recursing or not: the reproducer's `malloc` at every `CO_N` that
returns today (`CO_N` 1 included), the canary, `static.hero` (the image's data below is readable), and OpenSSL's default
fibre if it is still a `malloc`'d block (unrun on 4.1.0, `00-shared.md` § 2.7); the count of correct programs is the
ffi-pragmatist's. **Object**: each such program is a correct one refused, `blocking` by § Bounded discovery; (b) refuses
none; and w4 is still false in shape B (item 1).

## 4. Step 199j's stop: no sentence owed, and the reason

**Answer: none, today.** The stop (`sigguard.c:600-622`) ends a race in C's own code with C's own contexts: a handler of
C's that interrupted C's `swapcontext` or `setcontext` and switches naming either of its contexts. The stop's own words
call neither call async-signal-safe (recalled as POSIX's, not verified by the historian: a question), and step 199j's
commit says *"C alone dies too"* (forced, `Bus error` on this Mac, `SIGTRAP`, 3 of 3). **Panel 258's stop is the
contrast, and it is why 258's got a sentence**: a jump out of a handler that interrupted **this runtime's** allocator
(`docs/panel/258-*.md:45`) hurts state a C author cannot see from C; 199j's hurts C's own contexts, which C alone hurts
the same way. So it is a fact the spec leaves to the runtime, like every abort no section lists (`spec-shape.md` § Where a
rule lives: *no section lists every abort*). **No sentence is falsified by it**: on `main`, the handler sentence's *"by a
`swapcontext` away from it … may abort"* covers shapes 0, 1 and 6 by its letter, and its *"saying …"* belongs to a
callback's abort, while the stop aborts before any callback runs; under 262's adopted text the *"may"* list holds *"a
`setcontext`, a `swapcontext"*, so shape 2 too. **Price if adopted: w5, +52 vendored, a lower bound**; home: § 13 after
the handler sentence on `main`, or after 258's stop clause in 262's adopted text once landed (*"…, and else where the
program next uses that code;"*, then w5's clause). **Its instrument, if adopted**: none reads these words today
(`spec_refusals.hero` reads `hero_signal_refuse`'s arms only, `:6-8`); a check pairing w5's *"before it switches"* with
the stop's *"so the program stops here, before the switch"* (`sigguard.c:615`), as `spec_refusals.hero` pairs `CLAIMS`
with the arms, plus the four refused 2871 goldens. **What would change it** (the condition below): a correct program the
stop refuses (one that keeps the signal blocked around every switch, as the stop's own words advise, or a handler naming a
context the interrupted switch had finished with, the ffi-pragmatist's and compiler-engineer's shape to run): that is a
runtime defect first, and a sentence only if the stop cannot be narrowed. PARKED is R1's first sentence's, not mine.

## 5. Panel 262's thread clause, read against `malloc.hero`'s runs (the brief's and the critic's, not mine)

**No clause of the adopted text is falsified by its letter**: (i) the end-clause and its *"may abort"*: no handler of
C's runs in `malloc.hero` (`co.h` installs none), so it claims nothing; (ii) 258's stop: the same; (iii) the no-handler
clause, *"no coroutine switch or jump of C's makes a callback abort in **these words**"*: the runs printed the thread's
words, a hang, exit 138, `stack exhausted` or nothing at exit 0, never the handler's words; **but it is at risk under
this defect, unrun**: the recursion writes `hero_signal_stack` 48,872 bytes below `lo` (§ 2.3), the record a callback's
question reads, so a later callback may abort in the handler's words on a thread where no handler ran; (iv) the thread
clause states a sufficient condition (*on a thread of its own → these words*), and the runs print its words on the
program's own thread, which its letter does not forbid: **the runtime's message is false** (§4.17, `blocking`), **the
clause is not**. A reader reading the words in a terminal reads the converse, which is why the message, not the clause,
is the defect. **What its landing owes this defect**: R2's condition (a), *"with `so` … re-run … each as a `run`
golden"* (`:228-230`), and the compiler-engineer's 262 veto lifted only by *"repairs of `fm_top`, `wide` and `so` landed
and re-run"* (`:133`, `:341`): **`so`'s `malloc` half is this defect, so R1 lands after this sitting's repair and its
`malloc` goldens, judged by the canary's count**, or the verdict's *"`so` by step 199i"* is corrected underneath, with its
date, and the veto's condition read again by the author.

## 6. design.md's refusal of coroutines

(b) and (h) make the runtime switch to, or hand out, stacks of its own: **the stack switching the Part 6 row names as the
step change** (`docs/design.md:3642`, refused *"on cost alone"*, `:3611`). Neither is a coroutine: nothing in a Heroes
source suspends or resumes, nothing is scheduled, and Part 7's *"do not build a scheduler"* (`:4192`) stands. **A line is
owed under the row, dated, by CLAUDE.md § 12** (a refusal names the fact that would make it wrong), proposed: *"Read again
2026-11-27 (panel 263): where route (b) lands, the runtime switches to a stack of its own at a callback C calls, for
§1.12; that is no coroutine, and the row's ground narrows from the switch to what a coroutine adds over it, a suspension
no call site shows and the scheduler that resumes it."* `:4184-4192` owes nothing. design.md carries no token budget.

## 7. The instrument and the landing, for w1

- **Instruments first**: no instrument reads the held-memory sentence (`grep -rln "4,096 bytes\|have not returned\|1
  GiB" tests/harness`: only `thesis_sandbox.hero`'s `ulimit` comment); its abort's words are held by `run` goldens
  (`grep -rl "have not returned" tests/golden`: 14). w1's landing owes one more: callbacks left suspended in coroutines
  never resumed until their runtime stacks pass the bound, ending in the words, and `frame.c`'s words naming the stacks.
  `spec_refusals.hero`'s `SENTENCE_OPENS` (`:39`) and `CLAIMS` (`:32`) are unmoved: its sentence opens after w1's.
- **The `spec` suite with w1 applied**: 32 passed, 4 failed, the four being the pins and the ledger (above).
- **The net's own tests with w1 applied**: 5 of 918 `ok`, stopped; the rest unrun, owed at the landing.
- **The four pins and the row**: `SPEC_TOKENS` 10563 → **10569** (`tests/harness/suite_spec.hero:114-115`), `REAL_TOKENS`
  13849 → **13859** (`:175-176`) with its date **2026-11-27**, `SPEC_DIGEST` → **`4bf3cc3c13e0df6e`** (`:227`), and
  `selfhost/measure/pinned.hero:55-64` the same, in one commit with the ledger's row
  (`docs/measurements/010-spec-budget-ledger.md`); **re-counted at the landing**, since 262's text lands on the same
  line.
- **The site**: `spec/heroes-spec.md` is `.github/workflows/deploy-site.yml:73`'s, so the push that carries w1 publishes
  the site, and is asked of the author (CLAUDE.md § Hard stops).

## 8. Principle 0 and the payment rule, as costs

- **w1**: not compiler-need (`needed_for_self_hosting: no`); it is the price of the robust route told, panel 259's R2
  reasoning (*"without them each program above is a correct program that runs on `main` and stops, `blocking` … with
  them its end is the promised one"*, `docs/panel/259-*.md:128-131`). **Named removal: none.** **Payment: the prediction
  below**, registered with its instrument (the blind reading's protocol, run by panel 262, and a `run` golden) and its
  milestone (the landing's); if the blind run is not authorised, it is an observation and pays nothing, written so.
- **w2, w3, w4, w6, w1b, w5**: not adopted; each would owe the same, and none is a veto by its price.

## Predictions registered before the blind run

Per text I would adopt, which is **w1 alone** (w5 not adopted; § 9 unmoved). w1 changes nothing a reader reads about § 9,
so for the brief's programs the predictions are those of the spec as it stands, and they are what route (b) must make
true:

| program | a reader of the spec with w1 predicts | registered, of 10 readers |
|---|---|---|
| `reproducer/malloc.hero`, default `n` (10,000,000) | *aborts*, naming the stack | ≥ 8 *aborts*, ≥ 6 name the stack; 0 to 1 *runs* |
| the same sized by `CO_N` to return (1,600 to 30,000) | *runs*, printing the answer, or *may abort* for stack (§ 9 promises no depth) | ≥ 7 *runs* or *may abort*; **0** predict memory written below the stack |
| `canary.hero` at each `CO_N` | 0 bytes below written, or an abort before | ≥ 8 |
| `static.hero` | *aborts*, naming the stack | ≥ 8 |
| OpenSSL's default ASYNC fibre, a recursing job | *aborts*, naming the stack | ≥ 7 |
| 2871 shapes 0, 1, 2, 6 (refused) | *may abort* (on `main`, the handler sentence's *"a `swapcontext` away from it"*); the words *"not seen to end"* or *"from inside one"* | ≥ 6 *may abort* or *aborts*; ≤ 2 name a switch inside a switch as the cause (no sentence states it) |
| 2871 shape 3 (handler switches nothing, runs) | *runs* | ≥ 8 |
| 2871 shapes 4, 5 (handler swaps away and back, runs) | split: *runs*, or *may abort* by *"a `swapcontext` away from it"* | 3 to 7 *may abort* on `main`; ≤ 3 under 262's adopted text, where the handler ends at its return |
| **w1's own row**: 10,000 callbacks left suspended in coroutines C never resumes, nothing large held | with w1, *may abort past a bound, saying how many callbacks have not returned*; without it, *runs* | **≥ 5 with w1, ≤ 1 without**: the payment |

## Verdict

- `verdict`: **approve** route (b) with **w1** (provisional on the seats' runs of item 1's condition and item 2's count);
  **object** to (a), (a4), (c), (e), (n), (h) alone (each needs w2, and (h) w6), to (l), (q), (m) alone (only w3 is
  true, a confession of §1.12), and to (f) (w4 refuses correct programs and stays false in shape B); **veto**, on
  soundness, any route that leaves a callback's stack unguarded with § 9 unmoved; **no sentence** for 199j's stop.
- `section`: design.md §1.6 (`:255`), §1.2 (`:192`), §1.12 (`:646`); `.claude/rules/spec-shape.md` § Where a rule lives.
- `spec_token_delta`: **w1 measured: real 13,849 (pin, 2026-11-26) → 13,859 (2026-11-27), +10**; vendored 10,374 →
  10,380 and 10,563 → 10,569, +6 each. Every other text: a lower bound, vendored only (table above).
- `removal`: nothing; a registered prediction instead (w1's row), which pays only if the blind run is authorised.
- `needed_for_self_hosting`: no.
- `argument`: § 9's *"running out of stack aborts"* is false today on every stack C made without a page of no access
  below; the defect's own program returns at exit 0 with megabytes written. Only (b) makes it true with no word moved,
  because it alone reaches stacks the runtime never saw and the image's own data; every other route must write w2 or
  w3 into the spec, telling a reader their program may write over memory it does not own, which §1.12 refuses. (b)'s
  real cost to the spec is ten tokens: its held stacks, told by w1. 199j's stop guards C's own race, which C alone dies
  of: the spec leaves it to the runtime.
- `prediction`: w1 lands at +10 ± 4 real over the pin of the base it lands on; with (b) landed, `malloc.hero` at
  `CO_N` 1,600 to 30,000 and the canary write **0** bytes below the stack on both arm64 platforms, judged by the
  canary's count, never by `--sanitize`.
- `condition`: (b)'s thunk shown, by an emission and a run, to write on C's stack nothing but a bounded frame before
  the switch, and Windows given a switch, or § 9 owes a qualifier; a copy-stack library or a stack-scanning collector shown
  broken by a callback yielding on the runtime's stack (shape C), which turns my approval of (b) into an objection
  unless the route leaves such stacks alone (and § 9 takes w2 for them) or w7 lands; the seats' count of suspended callbacks a correct
  program reaches below 1 GiB ÷ S, or w1b instead; a correct program 199j's stop refuses, which makes w5 owed (after a
  runtime repair is ruled out).

**One line per item**:

1. § 9: no word moves under (b) (+0); w2 (+25 vendored, a lower bound) under any route reaching only seen stacks; w3
   (+33, a lower bound) under (l), (q), (m) alone. *Would change my mind*: a run of a callback entered on C's stack with
   fewer bytes left than the thunk's pre-switch frames take, ending at exit 0 under (b).
2. Held memory: **w1, 13,859 real (+10)**. *Would change my mind*: the seats' measured S and a correct program's count of
   suspended callbacks passing 1 GiB ÷ S, which makes w1b (+20, a lower bound) the text.
3. New abort: w4 (+39, a lower bound), objected. *Would change my mind*: the ffi-pragmatist's count of correct programs
   it refuses at 0.
4. 199j's stop: no sentence (+0); w5 +52, a lower bound, if owed. *Would change my mind*: one correct program the stop
   refuses, run on either arm64 platform.
5. 262's thread clause: not falsified by its letter; its landing owes `so`'s `malloc` half (R2 (a)). *Would change my
   mind*: a run of `malloc.hero` aborting in the handler's words, which would falsify the no-handler clause.
6. design.md: one dated line under the Coroutines row (no token cost). *Would change my mind*: a route adopted that
   switches no stack.
7. Instruments: none moves for w1; the `spec` suite 32 and 4, the four the pins; the net's own tests 5 of 918, stopped, the rest owed; the site is
   asked. *Would change my mind*: any check but the four red with w1 applied.
8. Payment: no removal; w1's registered prediction. *Would change my mind*: the blind run refused, which leaves w1 an
   observation and its +10 unpaid, written so in the landing's commit.

## Where this stood when copied

2026-11-27 at about 11:35: every item answered; **the one paid request made** (w1, 13,859); the `spec` suite run with
w1; the net's own tests stopped at 5 of 918; my copy clean but this folder (`git status --short`: `?? docs/panel/263-reports/`),
its spec `736b534c…efd3`; nothing of mine running. Texts beside this file: `spec-w1`, `w1b`, `w2`, `w3`, `w4`, `w5`,
`w6`, `w7`, each `.txt` and `.diff`. **Not done**: no reproducer run of mine (every outcome cited is the brief's or the
critic's); Windows by reading, a choice; the seats' runs, which items 1 and 2 are conditional on, not yet read.
