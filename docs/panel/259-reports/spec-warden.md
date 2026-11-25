# Panel 259, the spec-warden's report

Started 2026-11-25 08:22 (`date`), in the detached worktree `.claude/worktrees/259-spec-warden` at `3ec775b66`
(`git rev-parse HEAD` printed `3ec775b664c9cf0d6b8bae8c94f407f8a934e3a2`), long output in
`.claude/worktrees/259-spec-warden-scratch/`. Written as I go; sections still empty say so.

**The count instrument this sitting**: the coordinator's prompt forbids every API call, so no `measure --refresh` runs
here. **Every count below is a lower bound, vendored only**, in those words (`.claude/rules/spec-shape.md` § How a
change to the document is made). The real row read for the unmodified spec is the pin `measure` prints, not a request.

## Read, not run

- My agent file (`.claude/agents/spec-warden.md` of my copy); design.md §1.6 reached by `grep -n '^### 1\.'` (heading at
  line 255): *"must fit in 16384 tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"* (line
  257 to 258); the payment rule, *"Every amendment owes a named removal or a registered prediction naming an instrument
  that exists today"* (lines 326 to 328), and the prediction's admissibility, instrument and milestone (lines 330 to
  341); §1.2 at line 192, round-trip 500 to 2000 tokens, and line 391 to 394: metric 2's first run read the round-trip's
  median at 3,619 tokens (measurement 052).
- The spec, whole, in my copy (516 lines).

## Run, by me

**My compilers** (in my copy, `.claude/worktrees/259-spec-warden`): `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes` then `./heroes build selfhost/main.hero -o heroes-next`, both exit 0 (`259-spec-warden-scratch/build.log`);
`--version` prints `sources 05186f2b38e77a97` and `sources 117362877b136f67`, the brief's two keys (§ 3). Timings
(`real 64.62 user 23.72`, `real 608.82 user 239.39`) waited on a loaded machine and enter nothing. A usage limit
stopped this seat after 08:33 until 09:17 (the coordinator's message); nothing was half-written.

**The base, re-read** (`./heroes-next measure spec/heroes-spec.md` from my copy's root, no `--refresh`,
`259-spec-warden-scratch/measure-base.txt`): claude-legacy **9,877**, cl100k_base **10,054**, spread 177, **real
13,185** (*"claude-opus-5, 2026-11-23 — the binding number"*: the pin the binary carries, no request made), headroom
**3,199** against **16,384**, the FFI floor's 60, **13,245** measured against the ceiling. `wc -l -c`: 516 lines, 34,437
bytes; `shasum -a 256`: `c915f0f2...6372bee`. **No figure of the brief differs.**

**The three arms** (each `arms/arm-<x>.diff` applied by `patch -p1` to a copy in `259-spec-warden-scratch/arms/<x>/`,
`grep -c -F -f arm-<x>.txt` printing 1 each; `./heroes-next measure <copy>` from my copy's root,
`259-spec-warden-scratch/measure-arms.txt`):

| text | bytes | claude-legacy | cl100k_base | vendored delta (max) |
|---|---|---|---|---|
| base | 34,437 | 9,877 | 10,054 | 0 |
| `arm-p` | 34,575 | 9,909 | 10,086 | **+32** |
| `arm-r` | 34,704 | 9,945 | 10,123 | **+69** |
| `arm-t` | 34,723 | 9,954 | 10,131 | **+77** |

**A lower bound, vendored only**: no `--refresh` (the coordinator's prompt forbids every API call). The ratio of the
pinned real count to the vendored maximum on the base is 13,185 / 10,054 = 1.311; carried to a delta it would put the
three near +42, +90 and +101 real, **an inference, not a measurement**, and `.claude/rules/spec-shape.md` says why such a
carry can be wrong by a third on a short span.

**A shape beside `arm-r`'s list, run by me** (`259-spec-warden-scratch/shapes/own_spawned.hero`, `own_carved.hero` with
`main` replaced by `spawn_sized(carve, 7, bytes: 16777216).must()` and `t.wait()`, `carve` calling `ow_host`; `diff`
shows those lines alone): the same two coroutines on `ow_host`'s array, now in the frame of a thread whose stack the
runtime asked of `pthread` (`runtime/parts/spawn.c:661`, `pthread_attr_setstacksize`). Built with my stage 2, run
`timeout 120 ./bin-own_spawned`, this Mac: **exit 134, 3 of 3, equal by `cmp`**: `body holds 11`, `body holds 11`,
the *compiler bug* panic, `called from 0x10002f70f, in own_spawned.run`. Linux: **unrun** by me.

**My own texts** (each beside this report, `spec-w*.txt` and its `spec-w*.diff`; each diff passes `patch --dry-run
-p1` in my copy; each applied copy keeps line 428's tail from `No function of the program is a signal handler` to the
line's end byte for byte, the text `callback_thunk.hero:408` holds; measured from my copy's root,
`259-spec-warden-scratch/measure-mine.txt`):

| text | what | where | bytes | claude-legacy | cl100k_base | vendored delta (max) |
|---|---|---|---|---|---|---|
| `w1` | the promise, repaired | its own sentence, after `atexit(...)` and before the handler sentence | 34,524 | 9,895 | 10,072 | **+18** |
| `w1a` | `w1` merged | at the end of line 428's handler sentence (*", and no switch ..."*) | 34,528 | 9,896 | 10,073 | **+19** |
| `w1b` | `w1` merged | at the end of line 427's callback sentence, after `atexit(...)` | 34,528 | 9,896 | 10,073 | **+19** |
| `w2` | the told refusal, repaired (route A) | where `arm-r` stands | 34,744 | 9,954 | 10,132 | **+78** |
| `w3` | the told switch, repaired (route D), its refusal given `w2`'s threshold clause at 09:23 (without it, measured first at 34,769 / 9,969 / 10,146, **+92**, it refused q7, which prints today) | where `arm-t` stands | 34,843 | 9,987 | 10,165 | **+111** |

**A lower bound, vendored only.** Merging does **not** beat appending here: both merges cost one token more than the
sentence alone, and each changes what a reader reads into its host (below, question 4).

**`spec/refusal-words` over every text, run**: a copy of `tests/harness/spec_refusals.hero` and `strings.hero` in
`259-spec-warden-scratch/verdict/`, beside a five-line `check.hero` calling `spec_refusals.verdict(spec:, c:)` on a
spec and my copy's `runtime/parts/sigguard.c`, built with my stage 2: **`clean` for the base, `arm-r`, `arm-t`,
`arm-p`, `w1`, `w1a`, `w1b`, `w2` and `w3`** (`verdict/out.txt`). So the merge into the handler sentence keeps the
check green, `HERO_SIGNAL_HELD`'s words keep *"seen to end"* (`sigguard.c:607-612`), and **no text's new claim is held
by anything**: the check reads the handler sentence alone (`SENTENCE_OPENS`, `CLAIMS`, `spec_refusals.hero:33-42`).

**The compiler needs none of it**: `grep -rn -l -E 'makecontext|swapcontext|setcontext' selfhost/` lists 0 files.

**A confound in the blind programs, run by me** (`259-spec-warden-scratch/blind/`). Every program holds `E5` records,
8^5 x 8 = **262,144 bytes**, on coroutine stacks of `256u << 10` = **262,144 bytes** (`co.h:15`, `sw.h:9`), and
`task-t2.md` names the size (*"a record of 262,144 bytes"*). The spec never says where a large value lives, says a record
holds its fields *by value* (§ 3) and that *running out of stack aborts* (§ 9, line 331): a reader reasoning in C's
layout predicts an abort for q1, q2, q3 and q6, which print. Does the defect need values that large? q4 and q3 with
`E4` (32,768 bytes, past the emitter's 4,096 and an eighth of the stack: `e5(` to `e4(` from `run` down and one level
off each field path, `diff` shows those lines alone), built with my stage 2, run three times on this Mac: **`q4e4` exit
134, 3 of 3, equal by `cmp`: `body holds 11` twice and the *compiler bug* panic, `in q4e4.run`**; **`q3e4` exit 0, 3 of
3: `order 0 1: 30 32`, `order 1 0: 30 32`**. So the defect reproduces at 32 KiB and the registration can drop the
confound. Linux: **unrun** by me. Why it matters: the key grades an `ABORTS` *"by whether it names the switch or the
stack"*, so under a refusal key a reader who predicts a plain stack overflow on q4 is build-right, and under a
value-keeping key the same reader is build-wrong: **the confound lifts D1 for a refusal text and caps it for a
promise**, whatever the sentence says.

## Predictions registered before the blind run

Registered 2026-11-25 09:25 (`date`), **before any blind session is drawn; none is drawn and none runs in this
sitting**. Read for them: `llm-ergonomist.md`, `blind/answers.md`, `blind/registration.md`, both tasks, `q4.hero`,
`sw.h`, the `diff` of q4 against q7, the sizes in `co.h`. **Each number is mine, by reading, never by a run of a
reader.** The key under a text is the build that text describes, since `adopted`'s key is UNRUN: `arm-p` and `w1`, a
route that keeps the value (q4 `PRINTS` the three `body holds 7` lines and `carved: 7`); `arm-r` and `w2`, a refusal
(q4 `ABORTS`, q7 `PRINTS`); `arm-t` and `w3`, a told switch whose untold binding is refused (q4 `ABORTS`, since `sw.h`
tells nothing). Under `now`: q1, q2, q3, q6, q7 `PRINTS`, q5 `ABORTS`, q4 no key.

**t1, build-right of 5, point (range)**, with the inputs as registered (`E5`):

| text | q1 | q2 | q3 | q4 | q5 | q6 | q7 |
|---|---|---|---|---|---|---|---|
| `now` | 3 (1-5) | 3 (1-5) | 3 (1-5) | no key; `PRINTS` 3 (1-4) | 2 (0-4) | 3 (1-5) | 4 (2-5) |
| `arm-p` | 3 (1-5) | 3 (1-5) | 3 (1-5) | 3 (1-5) | 2 (0-4) | 3 (1-5) | 4 (2-5) |
| `w1` | 3 (1-5) | 3 (1-5) | 3 (1-5) | 3 (1-5) | 2 (0-4) | 3 (1-5) | 4 (2-5) |
| `arm-r` | 3 (1-5) | 4 (2-5) | 4 (2-5) | 2 (0-4) | 2 (0-4) | 3 (1-5) | 2 (0-4) |
| `w2` | 3 (1-5) | 4 (2-5) | 4 (2-5) | 4 (2-5) | 2 (0-4) | 3 (1-5) | 4 (2-5) |
| `arm-t` | 3 (1-5) | 2 (0-4) | 3 (1-5) | 2 (0-4) | 2 (0-4) | 3 (1-5) | 2 (0-4) |
| `w3` | 3 (1-5) | 3 (1-5) | 3 (1-5) | 4 (2-5) | 2 (0-4) | 3 (1-5) | 4 (2-5) |

**t1, `NOT PROMISED` of 5, point (range)**:

| text | q1 | q2 | q3 | q6 | q7 |
|---|---|---|---|---|---|
| `now` | 0 (0-1) | 1 (0-2) | 1 (0-2) | 1 (0-2) | 0 (0-2) |
| `arm-p`, `w1` | 0 (0-1) | 0 (0-1) | 0 (0-1) | 0 (0-1) | 0 (0-1) |
| `arm-r` | 0 (0-1) | 0 (0-2) | 0 (0-2) | 1 (0-2) | **3 (1-5)** |
| `w2` | 0 (0-1) | 0 (0-2) | 0 (0-2) | 1 (0-2) | 1 (0-2) |
| `arm-t` | 0 (0-1) | **3 (1-5)** | 1 (0-2) | 1 (0-2) | **3 (1-5)** |
| `w3` | 0 (0-1) | 1 (0-2) | 1 (0-2) | 1 (0-2) | 1 (0-2) |

(`arm-t`'s q2: its untold clause, *"on a stack the program was not told of"*, holds q2's carved stack, which nobody
told; question 2 below.)

**t2, build-right of 5, point (range)**: question 1 for A, B, C; question 3.

| text | A | B | C | Q3 |
|---|---|---|---|---|
| `now` | no key | 3 (2-5) | 3 (2-5) | 5 (4-5), *no* |
| `arm-p`, `w1` | 3 (1-5) | 4 (2-5) | 4 (2-5) | 5 (4-5), *no* |
| `arm-r` | 1 (0-3) | 4 (2-5) | 4 (2-5) | 5 (4-5), *no* |
| `w2` | 4 (2-5) | 4 (2-5) | 4 (2-5) | 5 (4-5), *no* |
| `arm-t` | 1 (0-3) | 4 (2-5) | 4 (2-5) | 4 (2-5), the function |
| `w3` | 4 (2-5) | 4 (2-5) | 4 (2-5) | 4 (2-5), the function |

**The decision rules**, as I predict them:

| text | D1 | D2 | D2w | D3 |
|---|---|---|---|---|
| `arm-p` | **fails** (q4 3 of 5 under it; under `now` 3 already give `PRINTS`, past the *at most 2*) | holds | holds | fails narrowly (A 3 of 5) |
| `w1` | **fails**, as `arm-p` | holds | holds | fails narrowly, as `arm-p` |
| `arm-r` | **fails by construction** (*may*) | at risk on q7 (`ABORTS` +1, range 0-3) | **fails on q7** (+3) | **fails** (A 1 of 5) |
| `w2` | holds, if route A's detector exists | holds | holds | holds |
| `arm-t` | **fails by construction** (*may*) | at risk on q2 and q7 | **fails on q2 and q7** (+2, +3) | **fails** |
| `w3` | holds, if a detector for the untold case exists | holds | holds | holds |

What the promise's D1 failure would mean, said before the run so it cannot be read after it: **`now` already teaches
q4's value** (§ 3), so a promise has nothing to move, and the stack confound caps the `adopted` half besides. A D1
that fails for `w1` is evidence for adopting **no sentence**, not for a refusal.

**The spend**, if run as registered: 31 sessions, **8.3 USD (6.0 to 11.5)**, under the 15.00 ceiling; at most 1 of the
30 graded sessions stops at its 0.60 cap.

**The text I expect adopted**: **no sentence** (a route that keeps the value lands, `w1` the rival the run measures),
about 60 in 100; `w2` with a refusal route, 30; `arm-t` or `w3`, 10. **A registration repair owed before any session**:
replace `E5` by `E4` in the seven programs and the t2 task (q4 and q3 at `E4` run on this Mac as their `E5` selves,
above), re-key on Linux, re-hash.

## The six questions

**1. Is a sentence owed, and by which route?** The spec already promises q4 its value: § 3, *"Every value behaves as an
independent copy"* and *"No aliasing exists among the values this language owns"*, and §1.12, *"must not corrupt
memory"* (design.md line 649); the only sentence that names a C switch (line 428) is about a handler, and the critic's
twelve words and the clerk's found none saying a switch may change a value (§ 4.4, carried; I read line 428 in my copy).
So `body holds 11` is the runtime breaking a written promise, and CLAUDE.md § 12 says where the bug is: *"Spec beats
compiler: the compiler has the bug."* **A route that makes the value keep on every platform (E, or C if an identity no
switch counterfeits exists, B kept for the jumps it sees) owes no sentence**: what a reader predicts from the spec is
then what the program does, and nothing is lost; `arm-p` would restate § 3 in one place, inviting a reader to doubt
the places it is not restated. **Route A owes one**, because it turns a promised program into a refused one, and it
must say *aborts* with a class a reader applies (`w2`). **Route D owes one for its form, and cannot stand alone**: its
sentence must say what an untold binding gets, which is A's refusal or E's keeping; leaving it the defect is the
sentence §1.12 forbids (shared brief § 6 F). B and E alone change only the runtime, but for the room's bound a
leak can reach (q5's *stack exhausted*, draft 2's, adjacent).

**2. Each draft's text.** `arm-r`: *"may abort"* fails D1 by registration and D2w on q7 (q7 is a callback on a stack
switched by code of its own inside the thread's stack, in its class, and prints); *"not by `swapcontext` or
`setcontext`"* is false at `3ec775b66` on Linux, where `unseen_carved` switches by the runtime's `swapcontext` and
aborts (the brief's runs, carried); *"is followed"* is a list (question 3); *"saying the switch was not seen"* is held
by nothing (run above). *"Inside the stack of the thread that called C"* a reader can apply where the C is visible
(`own_carved`, `below`, `own_spawned` all hold an array in a C frame) and cannot for a library that carves inside a
binary. `arm-t`: **its untold class holds every ordinary callback**: *"a callback C runs, inside the stack of the thread
that called C, on a stack the program was not told of"* is true of the spec's own example two sentences earlier,
`atexit(f)`, run on the thread's own stack that nobody tells; *"C that switches stacks ... tells the program"* reads as
a fact about C (Boost tells nothing) where a duty is meant; a Heroes signature names a function C calls at a switch;
nothing says when a told range stops being one (a range carved from `sw_host`'s frame outlives `sw_host`: what the
runtime does then is a question, unrun); *"saying so"* gives an instrument nothing to hold. `arm-p`: *"A value a
function holds keeps it"*, a reader cannot be sure what *it* is; false today on every platform run (`own_carved`,
`below`); *"by `swapcontext` or by code of its own"* is a two-item list inside *"every switch"*, and a reader asks where
`setcontext` stands. **Repaired**: `w1` (a promise, one claim, no list), `w2` (a refusal: *aborts*, a class by
location, the threshold that keeps q7 out, words an instrument can hold), `w3` (a told switch with its pair and `w2`'s
refusal for the untold). **q7**: inside `arm-r`'s and `arm-t`'s *may*; inside `arm-p`'s and `w1`'s promise, which it
keeps; outside `w2` and `w3` by their threshold. The price of that threshold: **the spec would promise readers the
emitter's 4,096 bytes** (`frame.c:13-15`), a fact it has never named; without it a refusal text either hedges (D2w
fails) or refuses q7 (D2 fails, and a correct program that runs today stops).

**3. The closed list.** `arm-r`'s *"a stack from `malloc` or `mmap`, or one given to `makecontext`, is followed"* is a
list of causes, and a running program falsifies each part: **`own_spawned`, run by me**, an array on a thread's stack
that `pthread` gave (`spawn.c:661`), harmed exactly as `own_carved` (exit 134, 3 of 3), which the list's *"from
`mmap`"* and the scope's *"inside the stack of the thread"* classify both ways (that the C library maps a thread's stack
is a reading, unrun); **`makecontext` reached by `dlsym(RTLD_NEXT, ...)`** on Linux at `3ec775b66` (`unseen_carved`
134, `ph` `state 10`, the brief's runs, carried), not followed; and a stack from an allocator the list does not name
(`posix_memalign`, a pool) a reader cannot classify at all (a question, unrun). **A class survives the next program; a
list does not**: the harm follows the stack's place and whether the switch is seen, never the allocator. `w2` keeps
one exception, `makecontext`'s stacks, a list of one tied to a door the `dlsym-make` lane is moving; `w1` has none.

**4. Where, and merging.** § 13 at line 428 is the home (a Heroes function runs on C's switched stack only as a
callback); § 9's line 331 is about depth, lines 465 to 466 about lends. **Merged, measured**: `w1a` (end of the
handler sentence) **+19**, `w1b` (end of line 427's sentence) **+19**, `w1` alone **+18**: merging does not beat
appending here, and each merge gives the promise a second reading (`w1a`: scoped to the handler case; `w1b`: an
afterthought to the `atexit` example). Keep it a sentence of its own, **before** the handler sentence, the general rule
then its exception. Every text leaves `spec/refusal-words` clean and line 428's tail byte-equal (both run above).

**5. Principle 0, Part 6, the payment.** No text is needed by the compiler (0 files of `selfhost/`); no thesis effect is
measured (the blind run is registered, unrun). So Principle 0's burden is unmet for every sentence today; CLAUDE.md §
Precedence (2026-11-22) lets the most robust resolution override it, and **the most robust resolution is the runtime
keeping the value, which owes no sentence**: a refusal sentence is the price of a less complete route, not of
robustness. Part 6's row (line 3624) refuses coroutines as a form of the language; a sentence naming C's switches is a
fact of the C boundary (§1.11, §1.12) and line 428 names `swapcontext` already, so no contradiction. `arm-t`'s entry
point is runtime support for C's coroutines, the row's *"step change in complexity"* landing in the runtime; **design.md
does not cover a runtime entry point for C's coroutines**, and I say so. Payment: `arm-t`/`w3` add a form, owing a
named removal (none is named) or a registered prediction naming an instrument that exists: the blind runner of panel
256 on t2's question 3, and `spec/refusal-words` **widened first** (instruments first, `spec-shape.md`) with the new
sentence's opening and claim; **no check holds a `hero_os.h` signature the spec states to the header** (searched:
`"runtime/`, `hero_os.h`, `hero_net.h` in `suite_spec.hero` and `spec_text.hero`, and `hero_signal_take`,
`hero_net_stop_asked`, `hero_net_ask_stop` across `tests/harness/` and `selfhost/`: only `guarded_names.hero:105`, a
list of names), so that check is owed before the text. **Step 179a's `HELD`**: its words keep *"seen to end"* and
name a cause the program has (a jump inside a handler onto a stack `makecontext` was given, line 428's own door);
`spec/refusal-words` reads clean on every text: **ratify it from the spec's side**. No sentence of this sitting changes
what it must say; a route-A refusal of an unseen switch needs **words of its own**, never a handler's, which are what
Linux prints today for `unseen_carved` (2705's and 2755's class).

**6. Scope.** `w1` is one property of the runtime, *no judgment by an address a switch can counterfeit*, and its text
names no platform, handle or allocator: true when the route lands on all three, false until then on every one.
**Scoping it** (*"on Linux and macOS"*, about five tokens) **would write a hole into §1.12** for Windows, where an own
assembly switch leaves the thread block unchanged (`stack_lane.c:89-100`, read, unrun): refuse the scoping, land the
route on all three first. `arm-r` and `w2` are **knowingly false on Linux at `3ec775b66`** for the `RTLD_NEXT` and C
library handles until the `dlsym-make` lane lands, and through `dlvsym` after it (`sigjump.c:69-70` says unanswered;
a question, unrun); on Windows a fiber's switch is the system's code, which a reader may or may not read as *"code of
its own"*, a second reading. `arm-t`/`w3` add a Windows question nobody can run here: whether a told range is honoured
where the runtime reads the thread block.

## Verdict

- `verdict`: **object**, **provisional** (every count vendored only; the blind reading unrun)
- `section`: design.md §1.6 (the payment rule, lines 319 to 341), §1.2, §1.0; §1.12 with CLAUDE.md § 12
- `spec_token_delta`: before **10,054** vendored maximum (real **13,185**, the pin of 2026-11-23); after, vendored
  maximum delta: `arm-p` **+32**, `arm-r` **+69**, `arm-t` **+77**; mine `w1` **+18**, `w1a` **+19**, `w1b` **+19**,
  `w2` **+78**, `w3` **+111**; no sentence **0**. **A lower bound, vendored only.**
- `removal`: nothing, and that is a problem for every drafted sentence; under the route I recommend nothing is owed
- `needed_for_self_hosting`: no
- `argument`: The spec already promises q4 its value: § 3's independent copies, §1.12's no corruption, and no word
  that a C switch changes a value. q4 printing 11 is the runtime breaking a written promise, and CLAUDE.md § 12 puts
  that bug in the compiler. The robust route repairs the runtime and owes readers nothing. Every draft is defective as
  text: `arm-r`'s list is falsified by `own_spawned` (run) and Linux's `RTLD_NEXT` door; `arm-t`'s untold class holds
  every ordinary callback, the spec's own `atexit` included; `arm-p`'s *"keeps it"* has no sure antecedent and is
  false today. Both *may* texts fail D1 by registration and D2w on q7. No text names a removal or an instrument holding
  its words.
- `prediction`: under `now`, at least 3 of 5 t1 readers answer `PRINTS` for q4; `arm-r` and `arm-t` each raise q7's
  `NOT PROMISED` by at least 2; `w1`, if ever landed, measures at most **+25** real by `heroes measure
  spec/heroes-spec.md --refresh` in the commit that lands it.
- `condition`: a blind run where `now` gives q4 `PRINTS` at most 2 of 5 and `w1` at least 4 (a measured thesis effect:
  I approve `w1` at its real count); or a measurement that no route keeps the value on some platform (then a refusal
  owes `w2`, which I approve at its real count once `spec/refusal-words` holds its claim).

## One line per question

1. **No sentence** under a route that keeps the value (0 tokens); `w2` (+78, *a lower bound, vendored only*) only if a
   refusal route lands. Would change it: a blind `now` arm giving q4 `PRINTS` at most 2 of 5.
2. **`w1`** (+18, *a lower bound, vendored only*) is the only repaired text I would let a reader see. Would change it:
   a `w1` reader who answers anything but `PRINTS` on q2 or q7 because of it.
3. **No list**: `w1` (+18) carries none; `w2` keeps one exception. Would change it: a carved-stack program that is
   harmed or spared by its allocator and not by its place.
4. **`w1` alone, before the handler sentence** (+18 against the merges' +19). Would change it: a `--refresh` putting a
   merge at or below `w1`'s real count.
5. **No form**: `arm-t`/`w3` (+77/+111) wait for an instrument that holds a `hero_os.h` signature; **ratify `HELD`**.
   Would change it: a binding whose library cannot keep its stacks off the thread's stack and that no route keeps right.
6. **`w1` unscoped** (+18), landing only when the route holds on all three platforms. Would change it: a Windows run of
   `own_carved`'s shape that the thread block already keeps right.

Finished 2026-11-25 09:26 (`date`), no container started, no request to any API made, every file I wrote is under
`.claude/worktrees/259-spec-warden-scratch/` and beside this report (`spec-w1`, `w1a`, `w1b`, `w2`, `w3`, each `.txt`
and `.diff`).
