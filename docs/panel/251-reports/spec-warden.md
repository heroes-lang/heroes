# Panel 251, spec-warden's report

Sitting A of M-rebel-library (threads and signals), 2026-11-19. Written as I go; every number carries
its command. My copy is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-spec-warden/`
(called `sw/` below), `git archive 3a5ee269117a31780ef1f96d0305452630ac4d83 | tar -x`, 10:08 CET.

## Setup, as measured

- `find sw -type f -not -path 'sw/tmp/*' | wc -l` before the `.git` file was written: **20022**, the
  writer's count.
- `cmp bw/arms2/spec-now.md sw/spec/heroes-spec.md`: equal. `wc -l -c`: 492 lines, 29407 bytes; `shasum -a
  256`: `1d532d514831aa70234ef0eb8ad77850ebd4c2e390234f22e973a363ed757001`, the brief's.
- Every arm of `bw/arms2/` re-hashed (`shasum -a 256 | cut -c1-16`, `wc -l`): all nine equal the brief's
  table (`now 1d532d514831aa70 492`, `q2-r 49cf54954dcb84e5 492`, `q2-r-after-t1 c011a88408c14129 492`,
  `q2-e 92c44fc0cb999390 492`, `q1-k f7484374bdec1123 492`, `q3-b 562b5a7980d0d36b 496`, `q3-m
  0fc6600ea429ba9f 498`, `proposal-b 9d30660e27e983a4 496`, `proposal-m 5b2d48d35e0607d2 498`).
- design.md §1.6, `grep -n '^### 1.6' docs/design.md` in my copy: **line 253**. The ceiling it states
  today: **16384**, *"measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*; the
  consolidation block's **1,500** real tokens *"before the thesis harness has run"*; the payment rule
  unconditional at every level.
- `selfhost/cli/refresh.hero` read: one `--refresh` makes three requests (two probes, `x` and `x x x x
  x`, then the document), curl reading the key from the environment by `--variable`; nothing in it asks
  for `.git`.

## Refresh log

Stage 2 built in my copy: seed `clang -I runtime seed/heroes.c runtime/runtime.c -o sw-heroes`, exit 0,
`real 53.41`; `./sw-heroes build selfhost/main.hero -o sw-heroes-next`, exit 0, `real 558.02 user 302.91
sys 66.63` (load 12 to 15, other sessions building; discarded as a duration); `./sw-heroes-next
--version`: `heroes 0.6.0`, **`sources 23c7d2a626b29fd5`**, the writer's, so I am on the frozen commit.
`./sw-heroes-next measure` on every arm of `bw/arms2/`: all nine vendored rows equal the brief's table.

**Whether `--refresh` runs in a copy with no `.git`**: it does (the brief's open question, answered). The
key was loaded by `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` in the one shell that
refreshed, and `echo ${#ANTHROPIC_API_KEY}` printed **108**, its length alone. Each arm was copied over
`sw/spec/heroes-spec.md`, refreshed by `./sw-heroes-next measure spec/heroes-spec.md --refresh`, and the
copy's spec restored from `bw/arms2/spec-now.md`, `cmp`-equal after every one (script
`sw/sw-tools/refresh_arms.sh`, its log `sw/tmp/refresh-log.txt`). **Eight refreshes, the brief's limit,
three requests each** (two probes and the document, `selfhost/cli/refresh.hero`); `q2-r-after-t1` not
refreshed, as the brief says. Each printed the four constants and the paste note on stderr, and nothing
was pasted into any tree.

| time (CET) | arm | sha256, first 16 | `SPEC_REAL_TOKENS` | `SPEC_REAL_MODEL` | `SPEC_REAL_TAKEN` | `SPEC_DIGEST` | real delta | vendored delta |
|---|---|---|---|---|---|---|---|---|
| 10:20:41-43 | `now` | `1d532d514831aa70` | **11384** | `claude-opus-5` | `2026-11-19` | `583fa94c01eddbf8` | 0 | 0 |
| 10:20:46-48 | `q2-r` | `49cf54954dcb84e5` | 11393 | `claude-opus-5` | `2026-11-19` | `4673f22c6de83944` | **+9** | +5 |
| 10:20:50-52 | `q2-e` | `92c44fc0cb999390` | 11416 | `claude-opus-5` | `2026-11-19` | `3f814bf04c0a8ec3` | **+32** | +27 |
| 10:20:54-56 | `q1-k` | `f7484374bdec1123` | 11414 | `claude-opus-5` | `2026-11-19` | `79e9c72460c6bca2` | **+30** | +26 |
| 10:20:58-21:00 | `q3-b` | `562b5a7980d0d36b` | 11634 | `claude-opus-5` | `2026-11-19` | `22204864307fbaa9` | **+250** | +162 |
| 10:21:02-04 | `q3-m` | `0fc6600ea429ba9f` | 11666 | `claude-opus-5` | `2026-11-19` | `24d9bf7a120b0f98` | **+282** | +185 |
| 10:21:06-08 | `proposal-b` | `9d30660e27e983a4` | 11644 | `claude-opus-5` | `2026-11-19` | `5c3ccb7723dd5a94` | **+260** | +168 |
| 10:21:10-13 | `proposal-m` | `5b2d48d35e0607d2` | 11676 | `claude-opus-5` | `2026-11-19` | `37da33783517965f` | **+292** | +191 |

What the refreshes say beyond the brief's table:

- **The pin holds**: `now` reads 11384 on 2026-11-19, the carried pin of 2026-11-15, digest equal to
  `SPEC_DIGEST` (`selfhost/measure/pinned.hero:63`). Proposal B's digest is the `5c3ccb7723dd5a94` the
  writer's in-place measure named, so the in-place file and my arm are one text.
- **The vendored tables under-price Q3 by half**: +250 real against +162 vendored for Q3 B (1.54), +282
  against +185 for M (1.52); R's +9 against +5 (1.8); E's +32 against +27 and K's +30 against +26 (1.2).
  `.claude/rules/spec-shape.md` § How a change to the document is made, measured again.
- **M costs 32 real more than B** (Q3 alone, 282 against 250; with R, 292 against 260).
- **The ceiling is not in question**: the largest arm, proposal M, is 11676, plus the FFI floor's 60 is
  11736 against **16384** (design.md §1.6, line 253): 4648 free. No veto on the budget for any arm.
- **The arms of my own, vendored only (lower bounds, never refreshed: the brief's eight were spent)**,
  each edit asserted to match once (`sw/sw-tools/make_sw_arms.py`), counted by `./sw-heroes-next measure
  sw/sw-arms/spec-<arm>.md`: `q1-aborts` (*"nor called by one, which aborts;"*) **+4**; `q2-r-waits` (R
  with Q3's *"waits for"*) +6; `q3-s3-only` (the § 3 address sentence alone) **+9**; `names-s5` (*"and
  nothing takes a built-in's (section 11)"* after § 5's shadowing sentence) **+13**;
  `proposal-b-no-sized` 8836, so `spawn_sized`'s words cost **34**; `proposal-b-sized-fails`
  (`spawn_sized` giving a `(thread T)?` failing `no_thread`) 8886, **+16** over proposal B.

## Registered predictions, finished at 10:14:06 CET by `date`, before my refreshes

Whether the coordinator's `run.sh` had started by then I did not read: its folder is the blind seat's,
which my brief keeps me out of. The coordinator holds the runner's first `run.log` line and can say
which came first.

Registered here under design.md §1.6's payment rule as amended by panel 046: the instrument is the blind
seat's runner and grading (`251-briefs/llm-ergonomist.md` § The grading and § The decision rules), which
exist today (`run.sh` written, `bash -n` clean, its rules registered); scored at this sitting, in
M-rebel-library, on the counts as the coordinator grades them. Each is worded so the counted rules score
it with no reading of mine.

- **W1, task 1, R.** `q2-r` right **≥ 4 of 5** by its row; `now`'s five re-graded by `q2-r`'s row **≤ 1**;
  so R is read **needed** (difference ≥ 3). Falsified if the difference is under 3. **Pays R's tokens.**
- **W2, task 1, E.** `q2-e` right **≥ 4 of 5** by its row; `now` re-graded by `q2-e`'s row **≤ 1**; E read
  needed. Falsified if the difference is under 3.
- **W3, task 1, the default.** `now`'s recorded ending classes: **normal in at least 3 of 5** (a reader of
  today's spec takes the thread to be cut off with the process, not to abort as § 13's lease sentence
  does). Falsified at 2 or fewer normal. Scores nothing paid; it says whether R surprises a reader.
- **W4, task 2, Q3.** `now` right **≤ 2 of 5**; `proposal-b` right **≥ now + 3** and `proposal-m` right
  **≥ now + 3** (each spelling *serving the thesis* by the rule). Falsified, per spelling, where its right
  count is under `now` + 3. **Pays Q3's tokens; if it fails for a spelling, that spelling's tokens are
  unpaid and Principle 0's burden for it is unmet** (it is not compiler-need, below).
- **W5, tasks 2 and 4, B against M.** Over the two tasks (10 sessions each), `|B - M| < 2`: *the spelling
  does not decide*. Falsified where one leads by 2 or more.
- **W6, task 2, names.** `proposal-b`'s task 2 answers declaring a name B takes (`spawn`, `spawn_sized`,
  `wait`, `mailbox`, `send`, `receive`, `close`): **at most 1 of 5**, so B is *not* recorded as "taking a
  name readers write". Falsified at 2 or more.
- **W7, task 4, the address sentence.** Right **≥ 4 of 5 in each spelling**: the amended § 3 sentence
  suffices. Falsified at 3 or fewer in either. **Pays the § 3 amendment's share.**
- **W8, task 3, K.** `now` right **≥ 2 of 5** and `q1-k` minus `now` **≤ 2**: K's sentence is *not* read as
  changing a reader's prediction. Falsified where the difference is 3 or more, and then the frozen spec
  under-tells the abort (Q1's block says what I would then adopt, and that it owes its own reading).

## Q1, a Heroes function a C handler reaches (defect 2522)

- `verdict`: **approve** route J at no spec token; **object** to route K's sentence. Provisional only on
  W8 (the counts are refreshed).
- `section`: design.md §1.6 (the payment rule: J spends nothing, K owes a payment it has not got) and §1.2;
  `.claude/rules/spec-shape.md` § Where a rule lives (*"Each site that aborts says so beside its
  operation"*). design.md does not define *"the binding's word"*; it is an idiom of spec § 13 (line 402),
  and my objection to K rests on that line, not on design.md.
- `spec_token_delta`: J: **11384 to 11384 real (0)**, 8702 vendored. K (`q1-k`): **11384 to 11414 real
  (+30)**, 8702 to 8728 vendored (+26). The told abort alone, *"nor called by one, which aborts;"*
  (`sw-arms/spec-q1-aborts.md`): **+4 vendored, a lower bound, not refreshed**.
- `removal`: J owes none. K: nothing, and that is a problem.
- `needed_for_self_hosting`: no.
- `argument`: Spec § 13's lines 403-404 forbid the shape outright, and a reader writing a program needs
  nothing more: J enforces them and tells no residue. K spends 30 real tokens telling where the rule
  stops being checked, which panel 247's R3 (never a known cost) and the author's no, for now refuse. It
  borrows line 402's the binding's word, which there names what a group declares (`counted_by`); no mark
  of a binding says a C function installs a handler, so K's residue is nobody's word. Where J errs, it
  refuses a correct program: a runtime defect under CLAUDE.md § 12, never a sentence. What 403 omits is
  only that a breach aborts rather than fails to compile. (117 words, `wc -w`.)
- `prediction`: **W8**: `now` right ≥ 2 of 5 and `q1-k` minus `now` ≤ 2 on task 3.
- `condition`: W8 falsified (the difference 3 or more): I adopt *"nor called by one, which aborts;"* at
  line 403 (+4 vendored, its real price owed by a refresh in the landing), and it owes its own reading
  before it is called sufficient. J's runs leaving an **accepting** residue (Heroes code reached inside
  a handler with no refusal): the residue is then a known cost the author refused, and no sentence may
  tell it as the binding's word; it goes back to the author with K's words beside it.

**Beneath it.** The home question: if anything is told it belongs at line 403, beside the rule it
qualifies, which is where K puts it; the place is right and the words are not. *"which aborts where the
handler was installed and left through the C library's own names and past them is the binding's word"*
reads to me as three clauses a reader must parse to learn one fact (a breach aborts), with the residue
named in a vocabulary (*"left through the C library's own names"*) the spec defines nowhere. J's
direction of error is the one §1.12 tolerates (a refusal, told), and 249's R3 already orders J built and
run first. **Unrun by me**: J itself (the building seats'); task 3 (the blind seat's).

## Q2, the end of `main` with threads running (defect 2552)

- `verdict`: **approve R merged into the lease sentence**; **object** to E, and to R as its own sentence
  after T1. Provisional on W1.
- `section`: design.md §1.6 and §1.12 (robustness: a told abort over a silent truncation, CLAUDE.md §
  Precedence 3); `.claude/rules/spec-shape.md` § Where a rule lives (*"Merging beats appending"* and
  *"the home is the section of the operation it governs"*, both § 13 here). Whether the silence is a
  ruling is not covered by design.md; my search is below.
- `spec_token_delta`: R merged (`q2-r`): **11384 to 11393 real (+9)**, 8702 to 8707 vendored (+5). R
  after T1: 8725 vendored (+23), **not refreshed, a lower bound**. E (`q2-e`): **11384 to 11416 real
  (+32)**, +27 vendored. R in Q3's words, *"a thread nobody waits for"*: +6 vendored alone; with Q3 it is
  proposal B less Q3 B, **+10 real** (11644 against 11634).
- `removal`: nothing, and that is a problem: R is paid by W1. **Under Q3 a removal appears**: a `(thread
  T)` is *"a handle (section 13) its wait consumes"* and *"a handle nobody consumes ... aborts when
  `main` returns"* already says it, so R's words become a restatement worth **10 real**; a named removal
  only once a reading shows readers derive it, which no task of this sitting measures.
- `needed_for_self_hosting`: no. The compiler joins its one thread (`selfhost/main.hero:112`).
- `argument`: The silence is an omission, not a ruling: no sitting rules it (searched), `hero_os.h:485-486`
  says an unjoined thread drops its answer with the process (E), Rebel's `threads` says it stops the
  program (R), and the runtime does neither: a false compiler bug, or nothing (5 of 5 each, my copy). R
  merges five words into the one sentence that already says what aborts when `main` returns: its home
  under today's doors and under Q3, where a sentence after T1 would move again. E needs no word the spec
  lacks (§ 6's saying why), but calls a worker cut mid-write a normal end, which §1.12 ranks below a told
  abort. `exit(code:)` owes nothing: the sentence already stops at `main` returning. (119 words.)
- `prediction`: **W1** (R needed by task 1's rule), with W2 and W3 beside it.
- `condition`: W1 falsified (`q2-r` minus `now` under 3 by R's row): R's 9 real tokens are unpaid, the
  runtime's true message carries the rule, and the silence is then recorded as a ruling. W2 holding
  while W1 fails would not move me to E: CLAUDE.md § Precedence puts robustness above the count.

**Beneath it.**

- **My runs** (`sw/repro/`, the writer's programs copied, built by `./sw-heroes-next build`, each binary
  run 5 times under `timeout -s KILL 20`, `pgrep` empty after): `unjoined` 5 of 5 exit 134, *"panic: 2
  heap blocks still live at exit (a missing decref) — this is a compiler bug"*; `unjoined_noheap` 5 of 5
  exit 0, stderr empty. So today one forgotten join is a false accusation and the other is silence.
- **The search for a ruling**: `grep -n -i -E 'unjoined|never joined|not joined|joined by nobody|nobody
  joins|forgotten join|left running' docs/panel/*.md` printed three lines, none on threads (panels 038,
  233); thread or join within a sentence of *main returns* over every sitting file naming a thread printed
  nothing. The record's readings disagree with each other: `runtime/hero_os.h:485-486` (*"A thread never
  joined drops its answer with the process"*) is E's semantics and is false of the runtime;
  `packages/rebel/0.1.0/files/threads.hero:1-20` (carried from the brief) is R's.
- **E's words**: *"the program says how many"* is § 6's register (*"An abort ends the program at once,
  saying why"*, line 183), which names no stream either, so E needs no word the spec lacks. What E leaves
  open is the status of a program that cut a thread: the spec gives a status only to `exit(code:)` (line
  362), so a reader of E assumes 0, a CI reads success, and the worker's half-written work is the cost.
- **`exit(code:)` under R**: the lease sentence is already scoped *"when `main` returns"*, and `exit`
  already skips that gate for leases on the record (`runtime/hero_os.h:145-151`: *"It bypasses
  `hero_runtime_check_leaks()`, deliberately"*). R inherits the same scope; it opens no hole the lease
  rule has not had since panel 036. No clause owed.
- **What R needs beneath it, no spec token**: the gate asks the thread table first, and a correction of
  `hero_os.h:485-486` in the same change, since R makes that comment false in so many words.

## Q3, threads that share nothing (decision 32)

- `verdict`: **object**, provisional on W4 and W7. Not a veto today: every arm sits far under the ceiling,
  and Principle 0's burden is registered (W4, W7), not yet failed. **It becomes a veto if W4 fails for
  both spellings**: no compiler need and no measured effect is CLAUDE.md § 2's *"it waits"*.
- `section`: CLAUDE.md § 2 (Principle 0) and design.md §1.6 and §1.2; `.claude/rules/spec-shape.md`
  § Where a rule lives.
- `spec_token_delta`: proposal B **11384 to 11644 real (+260)**, 8702 to 8870 vendored (+168); proposal M
  **11384 to 11676 real (+292)**, 8893 (+191); Q3 alone, B **+250 real** (+162), M **+282 real** (+185).
  All net of T1's removal.
- `removal`: T1 is the change's own replacement, already netted in every delta above, and pays nothing
  beyond them. The net +250 or +282 real is paid by W4 and W7, or it is unpaid: until they score,
  nothing, and that is a problem.
- `needed_for_self_hosting`: no. The compiler starts its thread through `hero_os.h` today
  (`selfhost/main.hero:86-101`) and compiles itself; `spawn_sized` becomes need only because Q3 takes that
  door away, and as written it does not carry what the compiler needs (below).
- `argument`: Not compiler-need, so Principle 0 asks a measured effect, and tasks 2 and 4 are it: until
  they score, 260 real tokens are unpaid. Three defects stand before any count. `spawn_sized` gives a
  `(thread T)` that cannot fail, so an operating system's refusal must abort where the compiler today
  exits 2 with the system's reason (panel 184's R5). B takes `close`, 77 of its 124 collisions and the
  core packages' own name for ending a connection, and leaves C's `close`, `send` and `wait` unbindable
  under their names behind a fix, pick another name, a C symbol cannot follow. M costs 32 more real tokens
  and leaves § 9's UFCS sentence contradicting its own: `m.send(v)` is sugar for which `send`? (119 words.)
- `prediction`: **W4** (each spelling's task 2 right ≥ `now` + 3, `now` ≤ 2), **W7** (task 4 ≥ 4 of 5 per
  spelling), with W5 and W6 beside them.
- `condition`: I approve a spelling when W4 and W7 hold for it **and** its text is repaired: `spawn_sized`
  fallible (+16 vendored, below), the copy after `close` told (+8 vendored, below), and, for B, `close`
  respelled so it takes no name the tree and C use (below), or for M, § 9's UFCS sentence amended with it
  and its price refreshed. W4 failing for both spellings: veto.

**The sub-judgments, each with what it rests on.**

1. **The compiler's own sized thread** (finding 8). `runtime/hero_os.h:469-472`: *"A refusal is answered
   rather than panicked, -1 with the operating system's own number in `*why`, so the caller can say what
   it needed the thread for"* (panel 184's R5); `selfhost/main.hero:100-107` turns it into exit 2 with the
   system's reason, and `runtime/parts/spawn.c:594` shows the plain door panicking on the same refusal.
   The proposal's `spawn_sized(f, x, bytes: n)` gives a `(thread T)`, so under it a refusal can only
   abort (134), breaking the compiler's exit-code contract (2, the tool could not run). **So under CLAUDE.md
   § 2's closure-list clause `spawn_sized` is owed, and owed fallible.** The words I would adopt, in §
   11's paragraph: *"`spawn_sized(f, x, bytes: n)` gives a `(thread T)?` whose stack holds at least `n`
   bytes, failing `no_thread` where the system starts none."* (`sw-arms/spec-proposal-b-sized-fails.md`,
   8886, **+16 vendored over proposal B**, a lower bound). `spawn_sized`'s words cost **34 vendored** in
   all (proposal B against `sw-arms/spec-proposal-b-no-sized.md`, 8870 against 8836). **A route nobody
   listed**: no `spawn_sized`, every thread reserving what the compiler's does (256 MiB is *"reserved
   address space and not memory"*, `main.hero:81-82`): 34 vendored tokens and one taken name less, at a
   per-thread reservation cost the compiler-engineer would have to measure; **unrun**. Running `main`
   itself on a created thread for every program is **refused already**, panel 107 (`main.hero:78-79`,
   `examples/sdl/` needs the process's own main thread).
2. **The names** (finding 2). **The spec states the rule nowhere**: `grep -n -i -E
   'reserved|taken|shadow'` prints line 167 (`use`) and line 442 (*"is taken to keep"*, another sense).
   The rule already binds the **24** names today's sentence offers (a port of `spec_text.offered_builtins`,
   `sw/sw-tools/offered.py`); B raises it to 31, M to 27. **What B takes, measured in my copy**
   (`grep -rn -E '^function (spawn|spawn_sized|wait|mailbox|send|receive|close)\('`, `heroes.packages/`
   and my own folders left out): **124**, of which **107** in `packages/rebel`, **5** in `packages/core`
   (`net.hero:172` `close(@c: Conn)`, `net/hero_net.hero:74` `send`, `:85` `close`,
   `net/http/internal/client.hero:20` `send`, `:53` `receive`), **4 in the compiler**
   (`arrow_inside.hero:36`, `ir/lower.hero:233`, `parse/statement_end.hero:85`,
   `emit/static_constant.hero:122`, all `close`), 7 in `docs/panel/` records, 1 golden fixture. **`close`
   alone is 77 of the 124.** And an `extern` member: `extern "stdlib.h"` binding `exit` is refused today
   with *"`exit` is a built-in of the language, so the name is taken everywhere — pick another name"*
   (`sw/probes/ext_exit.hero`, `./sw-heroes check`); `close` and `send` bound the same way pass today. So
   under B, by the same rule (an inference: no compiler has B, and I probed `wait` not at all), C's
   `close(2)`, `send(2)` and `wait(2)` are bound only through a header of the program's own that renames
   them, which the message does not say (design.md §4.17's promise, a gap B widens). **M**
   keeps every name free and pays 32 more real tokens; its arm amends § 9's *"There are no methods"* but
   leaves *"`x.f(y)` is sugar for `f(x, y)` (UFCS)"* standing, so a program declaring `send` makes
   `m.send(v)` two things by the text. **A spelling nobody listed**: B with its worst collision respelled
   in the family the spec already has for a built-in that ends a life, `end_lease(@x)`: `end_mailbox`,
   **0** declarations and 0 `extern` members in the tree (same grep), against `close`'s 77 and 30;
   `wait` 7, `send` 25 and `receive` 12 remain, and their respelling is the compiler-engineer's to price
   and the blind seat's to read. **Does either spelling owe the unstated rule a sentence?** Only if W6
   fails (2 or more of B's task 2 answers declare a name it takes); then, merged into § 5's shadowing
   sentence, *"..., so nothing else in the file may take it, and nothing takes a built-in's (section
   11)."* (`sw-arms/spec-names-s5.md`, **+13 vendored**, a lower bound).
3. **The two contradictions and the repair's answer** (finding 7). The amended § 3 sentence is the right
   one and the cheapest: **+9 vendored alone** (`sw-arms/spec-q3-s3-only.md`), merged into the one sentence
   that already tells a value whose copies share what they reach. Each alternative still owes it: `@m`
   leaves `m2 = m` reaching one queue, so § 3's *"No aliasing"* is still false without it; a move-only
   mailbox has one sender, which refuses the pool decision 32 asks for (`mail/smtp`, `mongodb`, `redis`,
   the census's rows, carried from `00-shared.md`); 7.13's queue per thread is reached through a `(thread T)`, whose copies then share a
   queue, so it owes the same sentence or a rule that a thread handle is never copied. **Two gaps in the
   text around it**: § 13 defines a handle as a group's record with a `tag` and no fields (line 424), which
   a mailbox is not, so *"a handle (section 13)"* in the table borrows the life rules under a definition
   it does not meet; and those rules say what aborts in C's terms (line 463, *"handing it to C again"*),
   so **a `send` through a copy after `close` is not derivable**: *"... which `close(m)` ends, as a
   handle's (section 13), a copy used after it aborting;"* (`sw-arms/spec-proposal-b-copy-after-close.md`,
   **+8 vendored** over proposal B). Task 4 does not reach that case.
4. **The doors** (finding 8's question). If `hero_os.h` stops declaring them, a program that binds one is
   refused **by an existing class**: `extern "hero_os.h"` binding an undeclared name passes `check` (exit 0)
   and is refused at build with `error[ffi_unknown_name]`, *"`hero_os.h` declares no ... — clang read the
   header and could not find it"* (`sw/probes/door_gone.hero`). **No new diagnostic class**; its note
   (*"check the spelling, or name the header that does declare"* it) is less exact than §4.17 asks for a
   door that left, and naming `spawn` there is the note's to add, no spec token. CLAUDE.md § 2 forbids
   keeping the doors beside the built-ins for older programs (Q5).
5. **The home and the instruments.** § 11 after `exit(code:)` is the right home: they are names the
   compiler offers, and a second fact goes in the prose after the sentence (spec-shape). **The sentence
   stays one sentence ending at `range`** in both arms, measured with the port of the reader (`now` 24
   names, B 31, M 27, last `range` in each); the `offered` check's last half passes only once the
   compiler has the names, the instruments-first order. **The sqlite example stays § 13's first fence** in
   both proposals (read by `python3 -I`, first line `extern "sqlite3.h" link "sqlite3"`). The `Prefix`
   alternative uses `Type` alone, which is defined, so `grammar/productions` holds by reading; the
   `grammar/contextual` check (`suite_grammar.hero:120-205`) will ask the compiler to declare `thread` and
   `mailbox` as `WORD_*` constants read under `selfhost/parse/`; `named` and `rejected` read
   `(thread T)` and `(mailbox T)` in the table's code spans. **None of the three suites was run** on an
   arm: the frozen compiler has no such built-ins and would fail them for that reason alone.
6. **The removal and the conservative.** T1 (64 words and 416 characters by my own split, `python3 -I`, equal to the brief's) leaves because its content is
   replaced; it is netted in the deltas and is not a named removal paying for the rest. The conservative,
   no change, costs no token; its price is the thesis's: **38 refused and 9 pending** of the census's 86
   ledger blocks (carried from `00-shared.md`), and the kept rows' workaround is a value written into
   text and parsed back by hand, which is exactly what task 2's `now` arm measures a reader doing. No row
   of the census names a crash or a corruption, so robustness does not decide this; Principle 0 does, on
   W4.
7. **Defect 2582 under Q3** (finding 12). § 11 already rules: *"`print` writes its values with no
   separator and exactly one trailing newline"* (lines 348-349). The torn line the writer recorded,
   `sum main done15//` (`t2-trap`, 1 of 30, carried), puts another print's bytes between one print's
   values, which is the separator the sentence says is absent. **Under CLAUDE.md § 12 the runtime has the
   bug and Q3 owes no sentence**; a lock around each `print` is the repair, no spec token. This is a
   reading of the sentence, not a run: no instrument of this sitting sees a torn line (the blind tasks
   print from `main` alone).

## Q4, the payment

design.md §1.6, lines 324-337: *"Every amendment owes a named removal or a registered prediction naming
an instrument that exists today"*, and a prediction pays only if it names the instrument and the
milestone at which it is scored. **What I register is above** (§ Registered predictions, W1 to W8, finished
at 10:14:06 CET, before my refreshes): the instrument is the blind seat's runner and its counted rules, which exist
today; the milestone is M-rebel-library, this sitting. What each pays:

| addition | real | vendored | paid by | if it fails |
|---|---|---|---|---|
| Q1, J | 0 | 0 | nothing owed | — |
| Q1, K (not adopted) | +30 | +26 | nothing: W8 predicts it changes no prediction | W8 falsified: *"which aborts"* (+4 vendored) instead, with its own reading |
| Q2, R merged | +9 | +5 | **W1** | unpaid; the runtime's message carries the rule, the silence recorded as a ruling |
| Q3, B (with R) | +260 | +168 | **W4** (B's half), **W7** (the § 3 sentence) | B's tokens unpaid; Principle 0 unmet for B |
| Q3, M (with R) | +292 | +191 | **W4** (M's half), **W7** | M's tokens unpaid; Principle 0 unmet for M |
| the repairs I name for Q3 | +16, +8 (and +13 if W6 fails) | vendored only | the same W4 and W7, as part of the clause they repair | — |

No named removal exists in this sitting's arms: T1 is replaced, not spent, and the 10 real tokens R's
words cost under Q3 are a removal only once a reading shows readers derive them from the handle rule.

**Does the consolidation block's 1,500 real tokens still bind? No, by its own words, measured.** design.md
§1.6 (lines 290-293) bounds *"the rows from M-buildable-structs to M-compatibility-promise, the
consolidation block"* to 1,500 real tokens *"before the thesis harness has run"*. Both conditions are
past: the block's rows are chain rows 66 to 74 (`ROADMAP.md:207-215`), M-compatibility-promise closed
2026-10-30, and they spent **10,048 to 10,514 real, +466** (`docs/measurements/010-spec-budget-ledger.md`,
rows at its lines 227 to 238); M-thesis-harness closed 2026-10-24 (*"Part 11's metrics 2 and 4 run for
the first time"*, `ROADMAP.md:211`) and `docs/measurements/052-the-first-held-out-run-...md` is that
harness's first held-out run. M-rebel-library is not a row of the chain at the frozen commit (`grep -n
'M-rebel-library' ROADMAP.md` prints nothing; its page exists). **What still binds from that raise** is
its other rule: the next raise rests on what the harness measures a token buys, and no arm comes near
asking for one (the largest, 11736 with the FFI floor, against 16384).

## Q5, the door's type before `1.0.0`

**CLAUDE.md § 2 (line 136) binds, and panel 232's promise does not yet.** The compiler is `0.6.0`
(`selfhost/main.hero:66-67`). Panel 232's promise covers *"the functions `hero_os.h` declares at 1.0.0"*
(`docs/panel/232-*.md:272-274`), and the same file says when it starts: *"before `1.0.0` no form enters
to keep an older program compiling, and the promise starts at `v1.0.0`"* (its lines 15-16). So
`runtime/hero_os.h:481-482`'s *"whose type does not move (panel 232's promise)"* reads the promise as
binding today, which its own sitting says it does not: a premise false at the frozen commit, owed a
correction in the change that moves a door. **What § 2 then asks of a route that changes or removes the
doors**: the change refuses the `0.x` programs that bind them and is told, never undone for them; no
door kept beside `spawn` as an alias for older programs, no mode, no old spelling (§ 2's own list). A door
kept for a capability the built-ins lack would not be an alias, but none is named: `spawn_sized`, made
fallible, carries the compiler's.

## Q6, the diagnostic class each route touches

- **Q1, J**: `ffi_defined_elsewhere`'s runtime arm (`selfhost/cli/supplied_words.hero:179-190`) fires on
  three more names, `setjmp`, `_setjmp`, `__sigsetjmp`, with its note *"rename it where library X is
  built, or link a library that does not define it"*. That note fits a library's own symbol and not a
  library that implements the setjmp family on purpose; a sanitizer runtime's interceptors of the same
  family are a question for the building seats (the brief's hole list names them). **Owed**: an annotated
  golden per name, and the census of every golden tree, since a refusal that fires where it did not is
  *"a change to what the checker refuses"* (`.claude/rules/verification.md`). **No spec token**: the
  spec names this class nowhere (`grep -n -i -E 'defines|defined' spec/heroes-spec.md` prints line 5,
  the notation, alone).
- **Q2, R**: not a compiler diagnostic but a runtime abort's words, which `run` goldens pin. **Owed**: a
  `run` golden for each shape the brief and I measured, each moving to a true message: a running thread
  holding a `str` (today the false *"compiler bug"*, 5 of 5 mine), one holding numbers only (today exit 0
  and silence, 5 of 5 mine), a finished thread never joined (carried: exit 0), and one holding a lease
  (carried: a false lease message). The spec's share is R's nine real tokens.
- **Q3**: (a) a value holding a `ptr` or `cstr` crossing, and a type form in the wrong place, are new
  checker refusals: new codes, annotated goldens under `tests/golden/check/`, the census; (b) a binding of
  a door that left: **no new class**, `ffi_unknown_name` measured above, its note owed the word `spawn`;
  (c) `builtin_name_taken` reaching seven more names under B (three under M): the class exists, but its
  fix cannot be followed for an `extern` member (measured on `exit` today), a §4.17 gap B widens to
  `close`, `send` and `wait`; under B its note owes the route a C name takes (a header of the program's
  own that renames it), **unrun**.

## Premises of the briefs I found false or incomplete

1. **"30 `extern` members named `close`, C's `close(2)` bound under its own name, which B would refuse"**
   (`00-shared.md:246-247`): true by count, misleading by weight. **26 of the 30 are in
   `docs/panel/178-reports/`**, a sitting's evidence no build reads; 4 are goldens (3 under
   `tests/golden/check/`, 1 under `tests/golden/run/`). Of B's 124 declarations, 7 are in `docs/panel/`
   records and **4 are the compiler's own** `close`.
2. **"the proposal still expresses what the compiler needs"** (`00-shared.md:211-212`): it does not.
   `spawn_sized` cannot fail in the proposal's text, and the compiler needs the refusal answered to exit 2
   (`hero_os.h:469-472`, `main.hero:100-107`).
3. **`runtime/hero_os.h:481-482`**, *"whose type does not move (panel 232's promise)"*: false today, by
   panel 232's own lines 15-16 (Q5). The brief posed it as a reading to confirm; I do not confirm it.
4. **`runtime/hero_os.h:485-486`**, *"A thread never joined drops its answer with the process"*: false of
   the runtime (my 5 of 5 aborts), and R would make it false by rule.
5. **"Whether `--refresh` runs in a copy with no `.git` is unrun"**: now run; it does, eight times.
6. **Proposal M's arm** amends § 9's *"There are no methods"* and leaves the UFCS sentence beside it
   unamended, so the arm as counted (+282 real) is short of the text M needs.
7. **Proposal B's arm** names a mailbox's after-close abort only by pointing at § 13, whose words are about
   C (line 463); the arm as counted is short of that clause too (+8 vendored).

Everything else I re-ran held: the nine arms' hashes and vendored counts, the pin's 11384, T1's 64 words
and 416 characters, the spec's 492 lines and 29407 bytes, `sources 23c7d2a626b29fd5`.

## In one line per question

- **Q1**: approve J, 0 tokens; object to K (+30 real). W8.
- **Q2**: approve R merged, +9 real (11384 to 11393); object to E (+32) and to R after T1. W1.
- **Q3**: object, provisional: B +260 real (11644), M +292 (11676), both under the ceiling; unpaid until
  W4 and W7; three text defects first (`spawn_sized` cannot fail, B's `close`, M's UFCS). Veto if W4
  fails for both spellings.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
