# Panel 254, spec-warden's report

Started 2026-11-22 02:05 CET (the clock read 02:10:59 when this file was first written). Written as I go;
everything below the predictions is filled in after them. Read before writing the predictions:
`00-shared.md`, `spec-warden.md`, `llm-ergonomist.md`, `blind/task-t1.md`, `task-t2.md`, `task-t3.md`,
`blind/brief-session.md`, `arms/spec-m.diff`, the added lines of `arms/spec-m0.diff`, spec-now's § 5
discard rule (lines 170-182 of `coord/arms/spec-now.md`) and § 13's handle-life paragraph, and
panel 251's blind-seat table (`docs/panel/251-reports/llm-ergonomist.md:59-67`). No build, no count and
no session had run when they were written.

## Predictions registered before the blind run

Registered 2026-11-22 02:11 CET. Each is a count out of 5 counted sessions per cell, graded by the rows
of `llm-ergonomist.md` as worded; a range is given beside each point and the point is what scores.

- W1, t1 `now`, by the seven reader's rows: **2 of 5 right** (range 1 to 3). Why: the added sentence
  ("any character, a newline and a `|` among them") pushes readers to an unconfusable encoding, but the
  spec has no `split` and no text-to-number parse (`to_i64` takes a number, not a `str`), so a
  length-prefixed or escaped encoding both ways is a long hand-written program, and rows (1), (2), (6)
  and (7) must all hold at once.
- W2, t1 `now`, mechanical (built, run 5 times, S1 to S4): **2 of 5 right** (range 1 to 3); reported
  beside, decides nothing.
- W3, t1 `m`, by the seven rows: **4 of 5 right** (range 4 to 5). The likely miss: a `(thread T)` kept in
  a list and waited through a `for`, or a missing answer to `spawn`'s `?`.
- W4, **D1: E1 = 4 - 2 = 2, so D1 does NOT hold** (the record crossing is not read as serving the thesis
  by the registered count E1 >= 3). Confidence about 55 percent; E1 >= 3 needs `now` at 1 or fewer.
- W5, t2 `m`, per program: **p1 5, p2 5, p3 4, p4 4, p5 2** (ranges p1 4-5, p2 4-5, p3 3-5, p4 3-5,
  p5 1-3). p5 is the hard one: the send is under an `if` so the checker cannot refuse `free(b)`, and a
  reader must also decide that a `(mailbox Block)` handed to `spawn` without `@` is not "a value holding"
  a handle; p4's likely miss is an abort that also names the mailbox, or a refusal of `_ =` on a thread.
- W6, t2 `m0`, per program: **p1 0, p2 0, p3 2, p4 0, p5 0** (ranges p1 0-1, p2 0-1, p3 0-3, p4 0-1,
  p5 0-1). Why: m0's `spawn` gives `(thread T)`, not a `T?`, so `spawn(...).must()` in p2, p4 and p5
  reads as refused; m0's mailbox is a handle whose `close` ends its life, so p1's `m.send(3)` after the
  close reads as a read where every path ended it (a compile error); m0 leaves `send`'s type unsaid.
- W7, **D2: d >= 3 on p1 (5), p2 (5) and p4 (4), three programs, so D2 HOLDS** (the proposal's sentences
  read as needed); d is 2 on p3 and p5. **The text suffices (right(m) >= 4) for p1 to p4 and not for p5.**
- W8, t3 `m`: **4 of 5 right** (range 3 to 5). The likely miss is row (6): the split 402/400/398 is
  arithmetic done by hand by the reader; second, a keeper whose loop main ends before waiting the workers.
- W9, **D3 HOLDS** (right(m) >= 4).
- W10, voiding: **0 sessions voided** by the scan, `reads.py` or the model check (panel 251 voided none).
- W11, the verdict session (spec-A = `now`, spec-B = `m`, task t1): **approve**.

Predictions on the instrument (scored by my own refreshes below, registered now, before any count):

- W12, real `m` minus real `now` (`claude-opus-5` through count_tokens): **+610** (range +560 to +680),
  the vendored +466 times the 11788/9022 ratio the pin and the vendored count of `now` give.
- W13, real `m0` minus real `now`: **+245** (range +215 to +290).
- W14, real `now`: **11788**, the pin, unchanged (the frozen spec is the pinned one).

## The refreshes (log)

One shell, `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`, `echo ${#ANTHROPIC_API_KEY}`
printed **108** (the length; the value never printed). Each arm `cp` over my copy's `spec/heroes-spec.md`,
`./heroes-next measure spec/heroes-spec.md --refresh` (stage 3, `sources 6e891e681c14bca0`), then
`git checkout -- spec/heroes-spec.md`; after the fourth, `cmp spec/heroes-spec.md coord/arms/spec-now.md`
printed nothing (equal) and `git status --short` named only my untracked `p254/`. Four refreshes, the
brief's maximum; each counts with **`claude-opus-5`** (`selfhost/cli/count_tokens.hero:41-42`,
`PINNED_MODEL`, the model `pinned.hero:58` and `:85` name), its request offset probed and subtracted
(`selfhost/cli/refresh.hero:212-221`).

| time (CET) | arm | sha256 (first 16) | real, `claude-opus-5` | real delta on `now` | vendored max | vendored delta |
|---|---|---|---|---|---|---|
| 02:20:28 | `now` | `ffd687d6dd6b6b54` | **11788** | 0 | 9022 | 0 |
| 02:20:29 | `m` (the proposal) | `b008c3078855228f` | **12454** | **+666** | 9488 | +466 |
| 02:20:30 | `m0` (panel 251's text) | `1dc1446d4fc99a03` | **12071** | **+283** | 9208 | +186 |
| 02:20:32 | `w` (my text, below) | `fa063601e2e58b58` | **12548** | **+760** (+94 on `m`) | 9557 | +535 (+69 on `m`) |

The vendored column is `./heroes-next measure <file>` run from my copy's root, each the maximum of
`claude-legacy` (8863, 9315, 9040, 9382) and `cl100k_base`. `spec-w-drain.md` (the S7 variant below) was
not refreshed (no fifth refresh): 9556 vendored, one below `w`, so its real count is **unrun**; a lower
bound by the vendored instrument only.

**Whether `claude-opus-5` is the reader's tokeniser**: the blind readers run `claude-opus-5-5`
(`blind/run.sh:98`, `--model claude-opus-5-5`) and the refresh asks `claude-opus-5`. Whether the two
count this document alike stays **a question**: answering it needs a count_tokens call naming
`claude-opus-5-5`, which no refresh makes and my brief allows no other API call. What is known is
design.md §1.6's own line that *"one pinned model id binds and moves only by author decision"* (panel
123 R3), so the binding number is `claude-opus-5`'s whatever the readers run; the gap is owed to a run
that names both ids (a count_tokens call each, free of charge per request but an API call).

My registered instrument predictions, scored: **W14 hit** (11788 exactly); **W12 held by its range**
(+666 in +560 to +680; the point +610 was 56 low); **W13 held by its range** (+283 in +215 to +290;
the point +245 was 38 low). The real-to-vendored ratio of the added text is 1.43 for `m` (666/466)
and 1.52 for `m0` (283/186), above the whole document's 1.31 (11788/9022): new prose in backticks
costs the reader's tokeniser more than the document's average.

## Working notes (as I go; the sections after these are the report)

- 02:11 build started in my copy (`p254-scratch/spec-warden`, `89f155330`, `git status` clean):
  stage 1 `real 13.55 user 11.93`, stage 2 `real 163.56 user 118.83 sys 32.46`; load 17.97 at 02:11,
  5.31 at 02:16 (`uptime`); not a timed result, no figure here is a speed claim.
- Read from the commit: `git show 691531908` (CLAUDE.md § Precedence's amendment, the definition's
  "Neither veto holds against the most robust resolution", the contract pin 8461 to 8611 real).
- design.md §1.6 (`grep -n '^### 1.6'`: line 255): ceiling **16384** real, `claude-opus-5` through
  count_tokens; the 1,500-real cap binds "the rows from M-buildable-structs to M-compatibility-promise"
  (the consolidation block), not M-rebel-library: the ledger's last row to count it is row 7856
  (251 of 1,500 spent); rows 8386 to 9022 (M-core-packages, M-rebel-library) price against 16384
  alone (`docs/measurements/010-spec-budget-ledger.md`, `awk` over its rows through line 254, its
  newest row; the file holds 308 lines, `wc -l`; corrected 02:28, the first note said *254 lines read*).
- Names, re-run on my copy, every count equal to `00-shared.md` § 5 item 9 and 10: 3 declarations of
  `spawn|spawn_sized|mailbox`; top-level `wait` 10, `send` 26, `receive` 13, `close` 84; 30 extern
  members named `close`; 73 files name `hero_thread_spawn`, 69 declare a member; 13 `use <rebel>/threads`.

## The verdict, in the structure of `.claude/agents/spec-warden.md`

- `verdict`: **approve route B; object to the text `m` as written**, and adopt it with ten text
  edits for nine defects (W-a to W-j below, the whole text `p254/spec-w2.md` of my copy). No veto: the measured
  budget fits 16384 (below), and since `691531908` neither the budget nor Principle 0 vetoes the most
  robust resolution; no defect of the text below is a soundness veto (none corrupts memory), and two of
  them (W-f, W-j) are robustness objections whose repair the synthesis owes.
- `section`: design.md §1.6 (the ceiling, its instrument, the FFI floor, the payment rule), §1.2 (real
  cost), CLAUDE.md § Precedence as amended at `691531908`, § 2 (Principle 0 and the 2026-10-29 rule);
  the spec's § 3, § 5, § 9, § 11 and § 13 for the text; design.md §4.10 item 2 (`docs/design.md:1827`)
  and Part 7.13 for W-f.
- `spec_token_delta` (every number this session's command's; real is `claude-opus-5` by `--refresh`):

  | text | real | real delta on `now` | vendored max | vendored delta |
  |---|---|---|---|---|
  | `now` | 11788 | 0 | 9022 | 0 |
  | `m` | 12454 | +666 | 9488 | +466 |
  | `m0` | 12071 | +283 | 9208 | +186 |
  | `w` (W-a to W-g) | 12548 | +760 | 9557 | +535 |
  | `w2` (W-a to W-j, the text I would adopt) | **unrun** (four refreshes spent) | lower bound: `w`'s +760, `w2` being +10 vendored on `w` | 9567 | +545 |

- `removal`: the proposal's own, § 13's doors sentence and its `threads, `: **-143 vendored** measured
  alone on today's spec (9022 to 8879, `p254/now-minus-doors.md`; real unrun), and my W-g, § 11's names
  sentence (inside `w`'s count). They do not pay +666 real: **"nothing more, and under `691531908` that is
  a cost reported, not a debt"**.
- `needed_for_self_hosting`: **no**. The compiler starts its one thread through the doors today
  (`selfhost/main.hero:88-113`, `hero_thread_spawn_sized`) and exchanges nothing with it.
- `argument`: The route fits: 12,548 real plus the FFI floor's 60 leaves 3,776 under 16384, so the
  ceiling needs no raise. The text has nine defects; among them: § 3's "no aliasing"
  becomes false; "taken" is said of three built-ins when the compiler takes every built-in's name; a
  thread is a handle whose `nullptr` the text leaves; S11's `spawn(f, @x)` hands a handle to a parameter
  the spec's own rules forbid consuming (probed); a queue can hold a copy of itself and leak in silence
  (design.md §4.10); `m.send(v)` and a program's `send(m, v)` can silently differ; the refusal site is
  unsaid; `closed` against `stopped` is unordered. Each has a measured sentence (+79 vendored on `m`).
  (119 words.)
- `prediction`: see § Predictions for the landing, below (each with when it is checkable).
- `condition`: I would withdraw W-f if the compiler-engineer shows a program the closure list, a Rebel
  application or the gateway shape needs whose message type holds its own mailbox type; W-j if the
  synthesis rules that a spawned function's parameter owns what `spawn(f, @x)` hands it (then the text
  must say so: the sentence is priced below as W-j'); and my approval of `m`'s semantics rows if the
  blind run's t2 shows a row I adopt as written read wrong by 2 or more of 5 readers (program 5's
  2 of 5 is already my registered expectation, W5).

## 1. The cost, and what the ceiling must be

design.md §1.6, read today (`grep -n '^### 1.6' docs/design.md`: 255): *"must fit in 16384 tokens,
measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*; `heroes measure` judges
`REAL_TOKENS + FFI_FLOOR >= CEILING` (the `spendable` check's own words, `p254/suite-m-spec.out`), the
FFI floor being **60** (panel 030 R3; `./heroes-next measure spec/heroes-spec.md` printed *"the FFI floor
mortgages 60 of it, so what is measured against the ceiling is 11848"*).

- `m`: 12454 + 60 = 12514; **16384 - 12514 = 3870 free**.
- `w`: 12548 + 60 = 12608; **16384 - 12608 = 3776 free**.
- `w2`: unrun real; vendored +10 on `w`; were its real delta 2 to 1 on the vendored one (the added text's
  measured ratio is 1.43 to 1.52), 3776 - 20 = **about 3756 free**, an inference, not a measurement.

**The ceiling the route needs is the one that stands: 16384, raised by nothing.** The rule of
`691531908` raises it *"to fit"*, and the route fits with 3,776 real to spare. The 1,500-real cap of
§1.6 binds only the consolidation block (M-buildable-structs to M-compatibility-promise), which ended
before M-rebel-library (the ledger, row 7856 the last to count it). And `DELTA_GATE` is **50** vendored
(`tests/harness/suite_spec.hero:215-216`): the landing commit moves the spec by +545 vendored and so says
in its body what paid (CLAUDE.md's opening; `spec/budget`'s own text, `p254/suite-m-spec.out`).

## 2. The text, sentence by sentence (`arms/spec-m.diff`, read in `coord/arms/spec-m.md` whole)

Every repair below is an edit of `p254/make_w.py` or the `w2` step, each asserted to match once; the
priced figure is vendored (`./heroes-next measure`), and `w`'s real is refreshed.

**W-a, § 3's table, `(thread T)` "a handle (section 13)"** (critic's finding 33). § 13 defines a handle
as a group's record with a `tag` and says *"`nullptr` is its null and `==` compares the address; a map
key is an error"*. Imported whole, a thread has a null: `t: (thread R) @= nullptr` reads as legal and
its `wait` as unsaid. The `==` (§ 7, now amended) and the map key (an error) agree. Repair, kept short:
*"a handle (section 13) with no null, which its `wait` ends"*.

**W-b, § 3, "No aliasing exists among the values this language owns"** (my brief's question). False
once a `(mailbox T)` is a value the language owns and two copies reach one queue: `b = a`, `b.send(1)`,
and `a.receive` gives it. The address sentence after it explains the mechanism but leaves the first
sentence false. Repair: *"No aliasing exists among the values this language owns but a mailbox's
queue."*

**W-c and W-g, the names** (S13). The proposal's § 11 line, *"The names `spawn`, `spawn_sized` and
`mailbox` are taken"*, is **the first sentence of the spec that says a name is taken** (`grep -n -i -E
'\btaken\b' spec/heroes-spec.md`: one hit, line 460, *"is taken to keep"*, unrelated), while the
compiler takes **every** built-in's name: 28 of 28 names probed as a local refused with
`error[builtin_name_taken]: ... the name is taken everywhere` (`p254/taken_*`: `len`, `push`, `slice`,
`chars`, `keys`, `join`, `repeat`, `sort`, `to_str`, `to_f32`, `to_f64`, `to_i8`, `to_u64`, `map`,
`filter`, `fold`, `find`, `any`, `all`, `range`, `print`, `read_file`, `write_file`, `args`,
`args_checked`, `exit`, `skip_test`, `end_lease`), and as a top-level function (`len`), a record
(`keys`), a parameter and a `for` variable; **accepted** as a field and as a case (`p254/taken_field`,
`p254/taken_case`, `check` silent). Said of three names only, the sentence teaches the other built-ins'
names are free, which the compiler refuses. Repair: say the rule once, in § 5 beside shadowing, *"and a
built-in's name (section 11) is taken in every file, a field's or a case's aside"* (W-c), and remove § 11's
names line (W-g), its other half moving to § 9 (W-d).

**W-d, § 9's UFCS sentence.** Read by the critic's example: `m.drain()` is not one of the four, so it
is `drain(m)`, the program's: the sentence reads one way, and *"There are no other methods"* no longer
contradicts it. What it left: the four names' freedom was said in § 11 a second time (two homes for one
rule, `.claude/rules/spec-shape.md`). Repair: *"written only after a dot and taking no name"*.

**W-h, § 9, a silent divergence the exception opens.** A program may declare `function send(m:
(mailbox Job), j: Job) -> ()?` (the name stays free), and then `m.send(j)` is the mailbox's and
`send(m, j)` the program's: two different calls on the same arguments, both accepted, a mistake the
build cannot see. Repair: *"though no function so named may take a thread or a mailbox first"* (+14
vendored on `w`). Nothing in the tree is refused by it (no thread or mailbox type exists today).

**W-e, § 11, `mailbox()` "its type written where it is bound"** (critic's finding 33d). Not a
contradiction of § 5's *"inference is local only"* (an annotation is local), but narrower than the
language: § 9 types a type parameter *"from the type the context asks for"*, so `Req(reply: mailbox())`
and `spawn(f, mailbox())` read as legal by § 9 and illegal by this clause. Repair, by the existing
parallel: *"typed as an empty `[]` is (section 10)"*.

**W-f, § 11, S5 and S9: a queue that holds a copy of itself** (the critic's finding 29, design.md
§4.10 item 2, `docs/design.md:1827`: *"references cannot be stored in variables or object fields, hence
all values form disjoint topological trees"*). Under S5 a queue *"goes with its last copy"*; a record
`Sub` holding `subs: [(mailbox Sub)]` (legal by § 3: *"contain itself only through `[T]`"*) sent into
its own mailbox puts a copy of the mailbox in its own queue, so the last copy is never dropped and,
since S5 tells nothing for a mailbox, the leak is silent. It is decidable on types: a runtime cycle of
queues needs a type that reaches its own mailbox type through fields, elements, payloads or mailbox
element types. Repair, a compile error: *"and so is a `(mailbox T)` whose `T` holds a `ptr`, a `cstr` or
that mailbox's own type, in a field, an element, a payload or another mailbox's `T`, so no queue ever
holds itself"*. Its cost to programs: a reply-to of the message's own type is refused (a hub whose
`Event` carries `(mailbox Event)` must split `Command` from `Event`); the gateway's `Request` holding
`(mailbox Reply)` is untouched. **Unrun**: no compiler half exists to build the refusal; the rule's
check at a generic function's instantiation is the compiler-engineer's to price. The same sentence
answers S10's *"where"* (*"anything else is a compile error"*) and makes `(mailbox ptr)` a compile error
rather than a mailbox nobody can use across a thread.

**W-f also, "which crosses as itself whatever it carries".** Program 5's `spawn(keep, m)` with `m:
(mailbox Block)`: is a mailbox of handles *"a value holding one"*, so that `m` must cross written `@m`
(emptying `main`'s copy)? The proposal's row says accepted without `@`; the text left both readings
(my registered W5 names it as p5's likely miss). The clause settles it.

**W-j, S11's `spawn(f, @x)`: a hand-over the spec's own rules make unusable.** A function value has no
`@` parameter (§ 9; probe `p254/hvalue`: `error[mutable_parameter_as_value]: ... no function type carries
an @`), so `spawn`'s `f` takes the handle as a borrowed parameter, and a borrowed handle cannot be
consumed (§ 13; probe `p254/hborrow`: `error[consumed_borrowed_handle]: free ends the life of what it is
given, and b was borrowed`). By the text, a handle handed in by `spawn(f, @x)` can only come back as
`wait`'s answer; anything else ends at `main` as *"one nobody consumes"*. A handle received from a
mailbox into a local is consumable (probe `p254/heq`: `b = malloc(16)` then `free(b)` builds and prints
`freed`, so no cell is needed, the shared brief's question 5). Repair W-j: *"crosses only through a
mailbox, written `m.send(@v)`, which empties it, its life going with it, or as the answer `wait` gives"*
(`w2`; with W-i, -4 vendored on `w-h`). The alternative, if the synthesis keeps `spawn(f, @x)`, is W-j',
a rule the compiler-engineer must build: *"whose `f` then owns it as a local"*, unpriced.

**W-i, S7: `closed` against `stopped`.** A stop asked on a closed, empty mailbox satisfies both clauses;
the text orders neither. Repair: *"or `stopped`, before all else, once a stop is asked"* (with W-j, in
`w2`).

**Read and found sound, no repair:** § 7's `==` (critic's 33a, now amended: a thread and a mailbox
compare as an address); `spawn_sized`'s result (*"does the same"*); § 13's first paragraph with
*"threads, "* and the doors gone reads whole (*"processes and files from the language's `hero_os.h`"*);
the stop sentence's *"or a mailbox's `receive`"* agrees with § 11's *"even with values in it"* (the
receive is named beside, not under, *"a waiting call"*); the lease sentence's *"a thread nobody waits
for"* agrees with S3; S3 against § 13's every-path compile error: *"as a call marked `consumes` ends a
handle's (section 13)"* imports both halves, a second `wait` where every path waited a compile error and
elsewhere an abort, which a reader must map from *"handing it to C again"*; it reads, at a stretch the
blind run's t2 can show. **Two consequences the text implies and a reader must derive**, each measured
on today's handles: a helper that waits a thread it was handed must take it `@t` (the
`consumed_borrowed_handle` rule), and threads kept in a list are waited through `for t in ts` (probe
`p254/hlist`: `for b in xs` then `free(b)` builds and prints `freed 2`).

**One the text cannot close, recorded (S5, the shared brief's question 1):** a worker blocked in
`receive(within_ms: -1)` on a mailbox nobody closes, waited by `main`, hangs; neither S5 (a value) nor
251's handle life tells it, since `main` never reaches its end. A hang, not a corruption; no sentence
of either text reaches it.

**S7's stop against a drain (questions 2 and 12), a question for the semantic seats, priced:**
*"even with values in it"* makes a reply already queued unreachable for ever once the process-wide,
permanent stop is asked (finding 32). The drain text, *"or `stopped` once a stop is asked (section 13)
and it is empty"*, costs **-1 vendored** on `w` (`p254/spec-w-drain.md`, real unrun), but a worker
already waiting on an empty reply mailbox at the stop still answers `stopped`, so it saves only replies
queued before the stop and lets an owner apply requests whose workers gave up. Neither text keeps every
reply; which is more robust is the compiler-engineer's measurement on the gateway shape, not a text
question. I keep S7 as proposed in `w2`.

## 3. Each row, S1 to S14

| row | verdict | text, where amended |
|---|---|---|
| S1 `spawn` | adopt as written | |
| S2 `spawn_sized` | adopt as written | |
| S3 `t.wait()` | adopt, amend the table | W-a |
| S4 `mailbox()` | amend | W-e |
| S5 life | adopt (a value), amend | W-b, W-f |
| S6 `send` | adopt as written (its unbounded queue is a semantic question I do not price) | |
| S7 `receive` | adopt, amend the order | W-i |
| S8 `close` | adopt as written | |
| S9 what crosses | adopt, amend | W-f (*"crosses as itself"*) |
| S10 never crosses | amend: the refusal site said | W-f (*"anything else is a compile error"*, `(mailbox ptr)` refused) |
| S11 a handle | amend: through a mailbox only | W-j (or W-j', the synthesis's ruling) |
| S12 the stop | adopt as written | |
| S13 names | amend | W-c, W-d, W-g, W-h |
| S14 the doors | adopt the removal from the text (-143 vendored); and the doors leave `hero_os.h`'s program surface in the same landing, by CLAUDE.md § 2's rule of 2026-10-29 (*"no old spelling kept beside the new"*), every caller moved; no spec sentence is needed: a member its header does not declare is refused at build, measured on probe `p254/nodecl` (`extern "stdlib.h"` binding `hero_no_such_door`: `error[ffi_unknown_name]: stdlib.h declares no hero_no_such_door — clang read the header and could not find it`, exit 1, no binary) | |

S14's reach, counted (`git grep -l -E 'hero_thread_spawn' -- '*.hero' ':!**/heroes.packages/**'`):
**73 files**, 53 `tests/golden`, 12 `examples`, 5 `selfhost`, 1 each `examples-rebel`, `packages`,
`tests/harness`; 6 `selfhost` files name some `hero_thread_` door (`git grep -l 'hero_thread_'`:
`cli/link`, `emit/callback_guard`, `emit/guarded_names`, `main`, `mutate/cut`, `mutate/recovery`). The
compiler's own `main.hero` can move to `spawn_sized` only after the seed refresh that knows it
(`.claude/rules/verification.md` § The batch, *"a new form enters `selfhost/` only after the refresh
whose seed knows it"*), so the doors leave the header in the landing after that refresh: a sequence the
lane is held to, not a sentence.

## 4. The names, the grammar's instruments, and spelling B with names nothing declares

**Taken**: `spawn`, `spawn_sized`, `mailbox` are declared 3 times today, all in Rebel
(`packages/rebel/0.1.0/files/threads.hero:61`, `:77`, `mail/internal/address.hero:141`), and by the
compiler's rule a built-in's name is refused for a function, a record, a binding, a parameter and a `for`
variable (§ 2's W-c probes), so all three are refused on landing and renamed (CLAUDE.md § 2, 2026-10-29:
no form enters to keep an older program compiling). A field named `mailbox` (`dns.hero`'s SOA case) stays
legal (the field probe). **Free**: `wait` 10, `send` 26, `receive` 13, `close` 84 top-level declarations
and 30 extern members named `close`, all untouched under M, and the compiler must not enter the four in
its `Builtin(name: ...)` table, or `builtin_name_taken` refuses all 133.

**The instruments the landing owes**, measured by running the `spec` and `grammar` suites on my copy with
each text laid over `spec/heroes-spec.md` (`./heroes-next run tests/harness/main.hero -- ./heroes-next
<suite>`, outputs `p254/suite-<arm>-<suite>.out`, the spec restored and `cmp`-equal after):

| text | `spec` | `grammar` |
|---|---|---|
| `now` | 35 passed, 0 failed | 12 passed, 0 failed |
| `m` | 30 passed, 5 failed: `budget`, `spendable`, `real`, `ledger` (the pins), `offered` | 12 passed, 0 failed |
| `w` | 30 passed, 5 failed, the same five | 12 passed, 0 failed |

`spec/offered` names what the compiler lacks: *"the spec offers these and the compiler has no such
built-in: spawn spawn_sized mailbox"*, so the lane owes three rows in `selfhost/inventory.hero` before the
spec lands; `named`, `rejected` and `fences` pass both texts. `grammar/productions` passes the new
`Prefix` alternative (every name it uses is defined, the production reachable from `File`). `heroes grammar`
and `grammar/contextual` owe nothing until the compiler declares the words; once `thread` and `mailbox`
are contextual words after a type's `(`, the suite asks three things (`tests/harness/suite_grammar.hero:118-180`):
`WORD_THREAD` and `WORD_MAILBOX` constants in `selfhost/keywords.hero`, the same two in its `contextual()`
list (what `heroes grammar` prints), and each read under `selfhost/parse/` through the constant, never a
literal. The landing commit also writes `SPEC_TOKENS`, the real pins in both files and the ledger row
(`budget`, `real`, `ledger`, `spendable`).

**Spelling B with names nothing declares** (panel 251's critic's *"a spelling nobody priced"*). Counted on
my copy for each candidate (top-level functions, extern members outside `docs/`, bindings, parameters and
fields): **`await` 0, 0, 0, 0** (and not a foreign word, `grep -i await selfhost/foreign_words.hero`: no
hit), and the prefixed `thread_wait`, `mail_send`, `mail_receive`, `mail_close` **0 each**; every natural
single word tried is declared (`finish` 44, `put` 65, `take` 28, `post` 6, `fetch` 6, `collect` 11,
`seal` 5, `shut` 5, `deliver` 3). So such names exist. Priced as `p254/b-free.md` (`w` respelled with the
four prefixed names, § 9 back to *"There are no methods"*, the Built-ins sentence +4 names): **9554
vendored, -3 on `w`** (real unrun). It keeps § 9 whole, has no W-h divergence class, and the
ffi-pragmatist's veto of B (its names collide with the tree's and C's `close`) does not reach it: 0
collisions. I do **not** recommend it over `w2`: its names depart from every reader's prior (`send`,
`recv`, `close` in Rust and Go), so a reader writes `m.send(v)` and is refused, one round trip of 500 to
2,000 tokens (§1.2) each time; that rate is unmeasured, a question rather than a premise, and W-h closes
M's only extra class for +14 vendored.

## 5. Principle 0, and panel 251's R3 reason 1

Panel 251's spec-warden made W4 (`docs/panel/251-reports/spec-warden.md:92`) the price of Q3's tokens:
each spelling right on task 2 at least `now` + 3, *"if it fails for a spelling, ... Principle 0's burden
for it is unmet"*; its condition (`:217-222`): *"W4 failing for both spellings: veto"*. It failed (B 0,
M 0 over `now`) and R3 recorded the veto. **Under `691531908` that veto is a Principle 0 veto and yields**:
it is recorded as a cost (the effect then measured: none on task 2), not a refusal. Its text conditions are
met by `m`: `spawn_sized` fallible, the copy after `close` told (`send` fails `closed`), § 9 amended and
its price refreshed (12454). The effect R3 named as where it would live, a separator in a document, is
this sitting's t1, and my registered W4 predicts D1 does **not** hold (E1 = 2).

**Whether I read the author's instruction as the ruling on the need: yes.** 2026-11-22: I read the
author's instructions 1 to 3 of that day (`00-shared.md` § 1) with the contract at `691531908` as the
ruling on the need for route B; the blind run's D1 to D3 stay the thesis measurement, recorded beside it,
and they decide which sentences are worth their tokens, not whether the route enters.

## 6. The payment (design.md §1.6, `grep -n 'named removal' docs/design.md`: 304, 326, 331, 377)

The payment rule is unconditional in §1.6's text (*"Every amendment owes a named removal or a registered
prediction naming an instrument that exists today"*, 326-328), and `691531908` makes the budget yield,
§1.6 *"keeps its text until a sitting reconciles it"*. So, for the record and not as a gate: **a payment
is still written in the ledger row**, and what I offer is (i) the named removal, § 13's doors sentence,
-143 vendored, measured alone; (ii) my registered predictions W1 to W11, which name an instrument that
exists today (the blind runner `run.sh` and `llm-ergonomist.md`'s rows) and their scoring point (this
sitting, M-rebel-library), paying only where they hold, the row to say which; (iii) W12 to W14, scored
above (W14 hit, W12 and W13 held by range). The rest of +666 real is the route's cost under the author's
rule, written as such. **The reconciliation of §1.6 with `691531908`** (*"design.md §1.6 keeps its text
until a sitting reconciles it"*) is owed to a sitting and not done here; I name it because the next
amendment's warden will meet the two texts disagreeing.

## 7. The conservative route, A

From the spec's side A costs **0 tokens**: a platform member and Rebel's package, the doors sentence
standing, the rung one `i64` or one `str` each way. Its price lands on the reader: a richer value must
be serialised into text, which is t1's shape, where panel 251's critic measured all five `now` programs
silently wrong with a separator in a document (`00-shared.md` § 4, R3). **Verdict on A: approve as the
recorded conservative route; not adopted**, the author's ruling being on B. If D1 holds (E1 >= 3, against
my W4), A's zero tokens are also the more expensive route by §1.2; if it fails, the thesis case for B rests
on t2 and t3 and on the author's ruling.

## 8. Predictions for the landing (registered 02:29, after the four refreshes; they score nothing paid)

- L1: the landed `w2` (or `w2` with W-j') refreshes to **12,560 real** (range 12,553 to 12,575),
  checkable at the landing commit's `--refresh`.
- L2: on the landed text with the lane's compiler, `spec` reads **0 failed** only once inventory holds the
  three rows and the four pins move; with the pins moved and the rows absent, **1 failed, `offered`**.
  Checkable at the lane's gate.
- L3: `grammar` reads **0 failed** with `thread` and `mailbox` declared as `WORD_` constants read by the
  parser, and **at least 1 `grammar/contextual` failure** if they are read as literals. Checkable at the
  lane's gate.
- L4: W-f refuses **0** tracked programs at landing (no mailbox type exists today) and **0** of the four
  Rebel applications rewritten on the route (gateway, registry, chat, notes), none of whose message types
  needs its own mailbox type. Checkable when those rewrites build.

## 9. Premises of the briefs found false, and corrections of my own

- **S11's row says *"a handle in a list goes as the list's elements do"*; the proposal's text does not
  say it** (`coord/arms/spec-m.md:382`, read whole). It is covered by *"a value holding one"*, so no
  sentence is owed, but the row is not the text.
- **S11's `spawn(f, @x)` is offered as a hand-over the receiving thread cannot complete** by the spec's
  own rules (probes `p254/hvalue`, `p254/hborrow`; § 2's W-j). The brief's premise that the critic's
  finding 30 repair made the hand-over whole holds for the sender only.
- **S13 treats "taken" as news for three names**; the compiler already takes every built-in's name and
  the spec says so nowhere (§ 2's W-c, 28 of 28 probed).
- **Question 5's premise that consuming a handle might need a cell**: it does not, `b = malloc(16)` then
  `free(b)` builds and runs (`p254/heq`), so `t = spawn(f, x).must()` then `t.wait()` needs no cell.
- Checked and true: `design.md:1827` (`grep -n`; my first `sed` read put it at 1826, my error); the
  refresh's path refusal (`selfhost/measure/pinned.hero:106-112`, its tests at 138-148); the FFI floor
  60; every count of `00-shared.md` § 5 items 9 and 10 that I re-ran; the vendored 9022, 9488 and 9208;
  stage 3's `sources 6e891e681c14bca0`.
- Mine: the working note *"254 lines read"* (corrected above at 02:28); W12's point (+610) was 56 below
  the measured +666, W13's (+245) 38 below +283, both inside their registered ranges.

## 10. Unrun, owed

- The real count of `w2` (and of `w-h`, `b-free`, `spec-w-drain`): four refreshes spent; owed to the
  landing's `--refresh` (L1) or a fifth refresh the coordinator may grant.
- Whether `claude-opus-5` and `claude-opus-5-5` count the spec alike: one count_tokens call per id.
- W-f's refusal, built: no compiler half here; the compiler-engineer prices its check, generics included.
- S6 bounded against unbounded, and S7's stop against a drain on the gateway shape: semantic seats'.
- The blind run's scores of W1 to W11: the coordinator's runner.

## 11. Commands, with their count lines (all from my copy, stage 3 `./heroes-next`, `TMPDIR` inside it)

- `./heroes-next measure <arm>`: `now` 8863 / 9022, `m` 9315 / 9488, `m0` 9040 / 9208, `w` 9382 / 9557,
  `w-h` 9396 / 9571, `w2` 9392 / 9567, `b-free` 9381 / 9554, `spec-w-drain` 9381 / 9556,
  `now-minus-doors` 8720 / 8879 (legacy / cl100k).
- `./heroes-next measure spec/heroes-spec.md --refresh`, four times: 11788, 12454, 12071, 12548.
- `shasum -a 256` of the arms: `ffd687d6...`, `b008c307...`, `1dc1446d...`, each equal to `arms.sha256`;
  `spec-w.md` `fa063601e2e58b58...`, `spec-w2.md` `0c06f838c0841643...`.
- Suites: `now` spec 35/0, grammar 12/0; `m` and `w` spec 30/5, grammar 12/0.
- Probes (`./heroes-next check` or `build`): `taken1`, `taken2`, `taken_*` (28 names, plus record,
  parameter, `for`: `builtin_name_taken`; field, case: accepted); `heq` builds, prints `freed`; `hlist`
  builds, prints `freed 2`; `hborrow` `consumed_borrowed_handle`; `hvalue` `mutable_parameter_as_value`;
  `nodecl` `ffi_unknown_name`.

The texts and scripts are in my copy, untracked: `p254/make_w.py`, `p254/make_variants.py`,
`p254/spec-w.md`, `p254/spec-w2.md`, `p254/w2-vs-m.diff`, `p254/w2-vs-now.diff`, `p254/b-free.md`,
`p254/w-h.md`, `p254/spec-w-drain.md`, `p254/now-minus-doors.md`.
