# Panel 256, the spec-warden

Written 2026-11-22 from 22:14, as it went (`date` at the first write: 22:14:27). Every command ran in my copy,
`.claude/worktrees/p256-scratch/spec-warden/` (`git -C <copy> log -1 --format=%H`:
`8aca3cc432a6007daaf416ecf3b1b8564472fa09`, the frozen commit), or read the briefs folder; the briefs were not
edited. Read first, whole: `256-briefs/spec-warden.md`, `00-shared.md`, `256-reports/critic-briefs.md`,
`256-briefs/llm-ergonomist.md`, the four tasks of `256-briefs/blind/`, the four arms' diffs
(`arms/spec-*.diff`), the frozen `.claude/agents/spec-warden.md` and CLAUDE.md § Precedence in my copy.

**The ceiling, by grep** (`grep -n 16384 docs/design.md` in my copy, line 257): *"must fit in 16384 tokens,
measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*. **The contract that binds me** is the
frozen one: *"**Neither veto holds against the most robust resolution** (CLAUDE.md § Precedence, the author's
instruction of 2026-11-22): there you report its cost, measured, and the ceiling is raised to fit it"*
(`.claude/agents/spec-warden.md` in my copy), and CLAUDE.md § Precedence in my copy holds *"Amended by the
author 2026-11-22: the most robust solution now always wins ... the spec-warden's vetoes yield to it"*. So no
budget or Principle 0 veto is mine to cast here; each is a cost, measured.

## Predictions registered before the blind run

Registered 2026-11-22 at 22:14 (`date`), after reading `llm-ergonomist.md`, the three tasks and the four arms,
**before** any of my measurements (my compiler was still building; no `measure` run). Each prediction is on the
arm's RULE as `llm-ergonomist.md` states it (its rows as repaired after critics 16, 20 to 23), a number and a
range. **Calibration, read before writing them**: panel 254's scored table
(`docs/panel/254-reports/llm-ergonomist-blind.md:155-171` in my copy) says *"every count of right sessions the
warden missed, it missed low"*: four of its nine counts outside their ranges, every one below what Opus 5.5
readers did. So I predict 5 of 5 wherever the arm's text states the row's rule in words a reader meets, and
lower only where the text is ambiguous (critic 21) or where the row and the text disagree (the `now` cell by
the compiler, critic 16).

**t1** (5 sessions per arm, the reader's rows):

| id | what | point | range |
|---|---|---|---|
| S1 | `right(w)` | 4 | 3 to 5 |
| S2 | `right(m)` | 5 | 4 to 5 |
| S3 | `lost(w)` | 0 | 0 to 1 |
| S4 | `lost(m)` | 0 | 0 to 0 |
| S5 | `E1 = right(m) - right(w)` | 1 | 0 to 2 |

Reason for S1 below S2: under `w` the reader owns two acts (the send and the ring) and a drain; under `m` one.
The task text (*"Rings made before that wait are told once together"*, *"prints every result that has
arrived"*) names the trap, so I predict no session loses a wake; what I expect to cost `w` its one session is
row (7), a slip of the language in a longer program (an unused listener key, a `poll.close(@set)` on a
binding that is no cell), not row (2).

**t2** (5 sessions per arm; right of 5 per program, programs 1 to 6):

| id | arm | p1 | p2 | p3 | p4 | p5 | p6 |
|---|---|---|---|---|---|---|---|
| S6 | `now`, by the compiler | 5 (4-5) | 0 (0-1) | 0 (0-1) | 0 (0-2) | 0 (0-1) | 5 (4-5) |
| S7 | `now`, by the frozen text | 5 (4-5) | 5 (3-5) | 4 (3-5) | 5 (4-5) | 4 (3-5) | 5 (4-5) |
| S8 | `gc` | 4 (2-5) | 5 (4-5) | 4 (3-5) | 5 (4-5) | 5 (3-5) | 5 (4-5) |
| S9 | `gi` | 5 (4-5) | 5 (4-5) | 5 (3-5) | 4 (3-5) | 5 (3-5) | 5 (4-5) |
| S10 | `gr` | 5 (4-5) | 5 (4-5) | 5 (4-5) | 5 (4-5) | 5 (4-5) | 5 (4-5) |

Reasons: `now` by the compiler is defect 2695 as a reader meets it: no reader of the frozen text predicts
`marker_mismatch` on programs 2 and 4 (nothing in § 11 or § 5 refuses an `@` at a send of a value holding
no handle, `grep -n 'does not change'` in the frozen spec: nothing), nor an acceptance and a run-time abort on
programs 3 and 5, which the text calls compile errors. `gc`'s program 1 carries critic 21's two readings
(a rule on the writing or on the meaning), so it is the one cell I put at 4 with a range down to 2;
program 3 under `gc` is refused by both readings for most readers (a handle at the call). `gi`'s program 4
asks a reader to refuse an `@` on an `i64`, the least intuitive of its refusals. `gr` refuses all six, the
easiest cell by construction (critic 22), even with the refusal's reason graded.

**t3** (5 sessions per arm, `right` per arm):

| id | arm | point | range |
|---|---|---|---|
| S11 | `gc` | 5 | 3 to 5 |
| S12 | `gi` | 4 | 3 to 5 |
| S13 | `gr` | 4 | 2 to 5 |

Reason for `gr` lowest: its text ends *"take the operation as a parameter instead"*, which invites a generic
taking a send function; spec § 9 forbids a function value with an `@` parameter (`grep -n 'Top-level functions
are values'`, line 321), so a reader who follows the invitation for a `db.Db` meets the trap critic 20 names,
and a reader who writes `(mailbox T)` in that generic's signature is refused by `gr`'s own last clause.

**The decision rules**:

| id | rule | predicted |
|---|---|---|
| S14 | D1, *"the waker admits a mistake the build accepts iff `lost(w) >= 1`"* | **does not hold** (`lost(w) = 0`) |
| S15 | D1, *"the mailbox in the set closes it iff `lost(m) = 0`"* | holds |
| S16 | D1, *"read as the form readers get right more often iff `E1 >= 2`"* | **does not hold** (`E1 = 1`) |
| S17 | D2 for `gc` (text suffices for >= 5 of 6 programs) | holds, 6 of 6; its weakest program p1 |
| S18 | D2 for `gi` | holds, 6 of 6 |
| S19 | D2 for `gr` | holds, 6 of 6 |
| S20 | D3 for `gc`, `gi`, `gr` (`right >= 4`) | holds for all three; `gr` the likeliest to fall |
| S21 | voided sessions | 0 (0 to 1) |

**And my own counts, registered before I run them** (the vendored deltas of `00-shared.md` § 4.5 times the
frozen spec's real-to-vendored ratio, 12,809 / 9,761 = 1.312, an inference until measured):

| id | arm | vendored delta | real delta predicted | range |
|---|---|---|---|---|
| S22 | `q1` | +51 | +67 | +55 to +80 |
| S23 | `gc` | +51 | +67 | +55 to +80 |
| S24 | `gi` | +86 | +113 | +95 to +130 |
| S25 | `gr` | +63 | +83 | +70 to +100 |

*Added 2026-11-22 at 22:23 (`date`), after my six counts and after writing the repaired texts (what was added: S26 to
S28, predictions for resolution arms the synthesis may add, on rules no arm above states):*

| id | what | predicted |
|---|---|---|
| S26 | a resolution arm on `spec-wadopt.md` (`gi`'s rows, R-base stated): t2 right per program 1 to 6 | 5, 5, 5, 5, 5, 5 (each 4 to 5); t3 5 (3 to 5); D2 and D3 hold |
| S27 | a resolution arm on `spec-wcq.md` (`gc`'s rows, its two readings repaired): t2 program 1 | 5 (4 to 5), one more than S8's 4: the repair removes the meaning reading |
| S28 | either arm above against its unrepaired one, t2 program 4 | no lower: R-base says the rule program 4 tests |

## 1. The arms' texts, judged (item 1, 2b and 4.6)

Each sentence read against the frozen spec in my copy; every quotation `grep`ped there or in `arms/spec-*.diff`.

**T0, a gap under every arm, older than this sitting: the spec states half of the send's marker rule.** Spec
§ 11 (`spec/heroes-spec.md:383`) says a handle, or a value holding one, *"crosses only through a mailbox,
written `m.send(@v)`"*; it never says the other half, that a value holding none is written `m.send(v)` and an
`@` on it is refused. The checker does refuse it: `selfhost/check/thread_ops.hero:175-178`, *"A send's `@`
(panel 254's S11): written where the value holds a C handle or a thread, which the send hands over and
empties, and nowhere else, `@` on a copy being a claim about the call that is false"*, its diagnostic
`marker_mismatch`; panel 254's synthesis lands it as *"S11's `@` by `marker_mismatch`"*
(`docs/panel/254-*.md:525`). `grep -n 'does not change'` and every `m.send(@` in the frozen spec: one
occurrence, the handle's. **So the frozen text does not decide `t2`'s program 4 for a non-generic `i64`
either**, not only through a generic, and `gi`'s sentence (*"those that hold none, written `m.send(v)`"*)
and `gc`'s exception (*"always sent `m.send(@v)`"*) both lean on a rule the document never states. Every
repaired text below carries that half, in five words, as clause **R-base**; the synthesis can take it whatever
route it adopts, and it is a clarification (the compiler already does it), not a change.

**T1, `gc`: its sentence reads two ways (critic 21), and the text says the WRITING more than the meaning,
weakly.** *"a value whose type names a type parameter is always sent `m.send(@v)`"*: a code span after
*"sent"* is how this document writes a required spelling (§ 11: *"crosses only through a mailbox, written
`m.send(@v)`"*), so the stronger reading is the rule on the writing (`m.send(v)` refused, program 1 refused,
the row's reading); but *"is always sent"* is the indicative the document uses for what a program DOES (*"`m.send(v)`
adds a copy of `v`"*), so a reader may take it as the meaning (every send of such a value empties it,
program 1 accepted). The document's own word for a rule on the writing is *"written"*, and its word for a
refusal is *"is a compile error"*; `gc` uses neither. Repaired in `spec-wc.md`: *"is sent only as
`m.send(@v)`, and `m.send(v)` of it is a compile error"*.

**T2, `gc`: *"which empties it whatever that type holds"* gives a meaning to an emptied number the document
never defines.** The frozen spec defines emptying for a handle (§ 11, *"which empties it, its life going with
it"*; § 13, *"reading it where every path has ended it is a compile error; elsewhere handing it to C again or
`==` on it aborts"*) and for a lease (§ 13, *"`end_lease(@x)` frees it and empties the cell"*), and for nothing
else (`python3` over every `empt` in the frozen spec: three sentences, all three those). What `n` reads after
`hand(m, @n)` at `T = i64` is unsaid: the old value, a zero, a compile error, an abort. Against § 5
(*"an `@` parameter nothing writes or ends (section 13), an `@` argument counting"*): the send's `@` counts, so
`hand<T>`'s `@v` is legal in every instance, and § 5 gives no answer to the read after either. **The
repair that defines every point** (`spec-wc.md`): the function reads `v` no more after the send, whatever the
type (one rule, checkable in the body once), and the caller's cell is *"emptied where the type holds a handle
and left as it was where it holds none"*, which is § 11's own two meanings, *"empties it"* and *"`v` then left
as it was"*, and invents no emptied number. **And T2's sharper half: `gc` makes one writing mean two things by
where it stands.** With R-base stated, `m.send(@n)` of an `i64` is refused outside a generic (the checker today)
and REQUIRED inside one; a reader who learns the writing in one place writes it wrong in the other. That is
the §1.2 cost this seat exists to price: a rule whose spelling depends on context it does not show.

**T3, `gi`: *"refused at a call that gives it the other kind"* has an antecedent a reader can take two ways.**
*"the other kind"* is meant as *"the kind its send's writing does not serve"*; read by position it is the
kind named last, *"those that hold none"*. And *"a type parameter's value"* is narrower than `gc`'s and `gr`'s
*"a value whose type names a type parameter"*: `xs: [T]` sent into a `(mailbox [T])` is not a type parameter's
value, so `gi`'s second clause would not reach it while its first (*"judged at each call"*) would. Repaired
in `spec-wi.md`: *"a send of a value whose type names a type parameter accepts, written `m.send(@v)`, only
types that hold a handle, and written `m.send(v)` only types that hold none; a call giving another is a
compile error, told at that call"*.

**T4, `gi` is what § 11 already says (critic 16, the brief's § 4.6), save two things.** Read as written, the
frozen § 11 refuses programs 3 and 5 (a handle sent without `@`, a handle into `spawn`: *"Anything else is a
compile error"*), and accepts program 2: nothing in it exempts a value whose type a call gave a letter. What
the frozen text does NOT say, and `gi` adds: **where** the error is told (at the call), and program 4's
answer, which is T0's missing half and is missing outside generics too. So **`gi` is a clarification of
§ 11 plus R-base, not a change to the language**; by CLAUDE.md § 12 (*"Spec beats compiler: the compiler has
the bug"*), defect 2695 is a compiler defect against the frozen text, and `gc` and `gr` are the two arms that
change the language: `gc` by accepting program 4's writing inside a generic, `gr` by refusing programs 1 and 6,
which the frozen text accepts.

**T5, `gr`: *"take the operation as a parameter instead"* collides with design.md §4.12 and with spec § 9.**
§4.12's *"If an operation on `T` is needed, pass it as a parameter"* (`docs/design.md:2004`) means a function
value; but a send is no function value (spec § 9: *"a mailbox's `send` ... written only after a dot and taking
no name"*), so the reader must write a function of their own that sends, and for a value holding a handle
that function would need an `@` parameter, which § 9 forbids a function value (*"Top-level functions are
values, but not one with an `@` or `lent` parameter"*, `spec/heroes-spec.md:321`). The advice is therefore
followable for a handle only by a copy (`mine @= v; m.send(@mine)`), which leaves the caller holding a copy of
the handle whose life went with the send: program 5's shape (`close_it`), stopped only at run time by § 13's
guard. **So `gr`'s own advice leads a reader back into the class `gr` closes**, one function over. And
*"is not sent, handed to `spawn` or answered by a thread"* is the indicative again, where the document says
*"is a compile error"*. Repaired in `spec-wr.md`: the refusal said as a compile error, and the advice replaced by
the plain statement that a send is written where its type is known.

**T6, `q1`: *"once the queue has gone it names nothing"* is a promise about the runtime, and the document's
word for the queue going is another.** § 11 says the values are *"dropped with its last copy"*; *"has gone"*
appears nowhere else in the frozen spec (`grep -c 'has gone'`: checked below). *"names nothing"* promises that
a stale number never names a LATER queue, which holds only if the runtime's table never reuses a slot without
a generation; that is the compiler-engineer's to show (P1 item 1 cites `hero_net_number`), and the text
should say it in words a test can hold: *"names no queue once the mailbox's last copy is dropped, never
another"*. And *"for every copy, the same number"* is implied by *"names its queue"* (copies share one queue,
*"closes it for every copy"*) and costs four words to say. The UFCS sentence's fifth operation refuses no
function in the tree (critic 29, 55 `number` definitions, none taking a mailbox, a thread or a bare letter
first), and matches the tree's usage (`net`'s `number(@s: Socket)`, `ui`'s `number(app: App)`). Repaired in
`spec-wq.md`.

**T7, two rules on a letter the checker holds and the spec never states** (found while asking item 3's
question; neither is any arm's defect). Every sentence of the frozen spec naming a type parameter (`python3`
over `type parameter` and `a letter`): three, § 5's `_` rule (line 184), § 9's inference (line 325), § 11's
*"`sort` ... never a type parameter"* (line 362). So panel 155's R2, `==` on a bare letter refused in the body
(`generic_equality`, `selfhost/value_errors.hero`, `check/ops.hero`), has no sentence, and § 7 says the
opposite to a reader: *"`==` is structural equality on any two values of one type"*. Named for the synthesis;
not repaired here, since no arm touches § 7.

## 2. The counts, measured (item 2)

My compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 25.41`, `sources
05186f2b38e77a97`), then `./heroes build selfhost/main.hero -o heroes-next` (`real 341.72`, `user 203.38`,
`sys 49.36`: a waiting run at a load of 31 to 90, `uptime`, so no duration), `--version` **`heroes 0.6.0` /
`sources 031aaabb358218fd`**, the coordinator's. The arms' hashes: `shasum -a 256 -c` against
`arms/arms.sha256`, five `OK`. Vendored: `heroes-next measure <file>`, offline. Real: the file copied over
`spec/heroes-spec.md` in my copy, `heroes-next measure spec/heroes-spec.md --refresh` (key from the main
checkout's `.env`, **length 108** printed, never its value), then the frozen bytes copied back from
`zwarden/spec-frozen.md` and `cmp`ed, after every one. **Six refreshes, the brief's ceiling**; after the
sixth, `git diff --stat spec/` printed 0 lines and `shasum` read `15804f3fb52a62c2`.

| text | what it is | legacy | cl100k | vendored max (delta) | **real** (delta) | real ÷ vendored delta | refresh, digest |
|---|---|---|---|---|---|---|---|
| `now` | the frozen spec | 9,585 | 9,761 | 9,761 | **12,809** (pinned) | | `measure`, no refresh |
| `q1` | arm | 9,636 | 9,812 | 9,812 (+51) | **12,875 (+66)** | 1.29 | 1, `08f9110a3483d6e6` |
| `gc` | arm (P2) | 9,635 | 9,812 | 9,812 (+51) | **12,875 (+66)** | 1.29 | 2, `2becc83b2381399e` |
| `gi` | arm | 9,669 | 9,847 | 9,847 (+86) | **12,915 (+106)** | 1.23 | 3, `5ed9336d18f55b98` |
| `gr` | arm | 9,648 | 9,824 | 9,824 (+63) | **12,898 (+89)** | 1.41 | 4, `787b0cd02fe4c01a` |
| `spec-wadopt.md` | my route: `wq` + R-base + § 9's sentence + `gi`'s send clause | 9,796 | 9,976 | 9,976 (+215) | **13,083 (+274)** | 1.27 | 5, `34183a4e26226f49` |
| `spec-wcq.md` | the proposal as repaired: `wq` + R-base + `wc`'s sentence | 9,789 | 9,966 | 9,966 (+205) | **13,074 (+265)** | 1.29 | 6, `74ac4b3e1487d7bb` |
| `spec-wq.md` | `q1` repaired (T6) | 9,650 | 9,826 | 9,826 (+65) | unmeasured; **about +84 by the ratio, an inference** | | |
| `spec-wc.md` | `gc` repaired + R-base | 9,724 | 9,901 | 9,901 (+140) | unmeasured; about +181, an inference | | |
| `spec-wi.md` | `gi` repaired + R-base | 9,717 | 9,894 | 9,894 (+133) | unmeasured; about +172, an inference | | |
| `spec-wr.md` | `gr` repaired + R-base | 9,695 | 9,871 | 9,871 (+110) | unmeasured; about +142, an inference | | |
| `spec-w9.md` | R-base + § 9's sentence + `gi`'s send clause, no `q1` | 9,731 | 9,911 | 9,911 (+150) | unmeasured; about +194, an inference | | |

(Each digest is line 8 of `zwarden/refresh-<n>-<text>.txt` in my copy, the refresh's own output.)
The hashes of my texts are `256-reports/spec-warden/spec-w.sha256`, written by `zwarden/make_w.py` in my copy,
which builds each from the frozen spec by edits asserted to match exactly once.

**My registered count predictions, scored**: S22 `q1` +67 (55 to 80), counted **+66**, hit; S23 `gc` +67,
counted **+66**, hit; S24 `gi` +113 (95 to 130), counted **+106**, inside; S25 `gr` +83 (70 to 100), counted
**+89**, inside. The ratio real ÷ vendored on a marginal sentence ran 1.23 to 1.41 against the whole
document's 1.31: **a vendored delta ranks two drafts wrongly when they are close** (`gi` +86 against `gr` +63
vendored is a gap of 23; real, 17).

**Against the ceiling** (`docs/design.md:257`, 16384): my route at 13,083 leaves **3,301**, the FFI floor's 60
(`measure`'s own line: *"the FFI floor mortgages 60 of it"*) included, 3,241. No text here comes near it.
**The consolidation block's 1,500** (§1.6, *"spend at most 1,500 real tokens before the thesis harness has
run"*): what that block has spent so far is **unrun by me**; the ledger `docs/measurements/010-*` would say,
and the 2026-11-22 rule makes it a cost in any case.

## 3. Principle 0 and §1.2, as costs (item 2)

- **The compiler needs none of it.** `selfhost` declares one generic function (`git ls-files
  'selfhost/*.hero' | xargs grep -c -E '^function [a-z_0-9]+<'`, summed: **1**, `fetch/verb_place.hero:95
  unit_of<T>(answer: T?)`, which sends nothing). Whether any of the 11 `selfhost` files that name `mailbox()`
  or `spawn(` waits on a mailbox and a socket at once is unread. So Q1's `m.number()` and every Q2 arm enter
  by robustness (CLAUDE.md § Precedence, rank 3, and the 2026-11-22 amendment), never by § 2's closure list.
- **Q2 serves robustness, measured by others**: the coordinator's and the critic's probes (`00-shared.md`
  § 4.2, re-run by the critic) show a handle through a generic accepted and stopped only at run time (`p3b`,
  `k3d`) and a `ptr` crossing a thread with no check at any stage (`p7`, `p8`). Its token price is **+66 to
  +106 real** for the arms as written and **+265 to +274** for the repaired texts with Q1; the difference is
  R-base, the repairs' plainer words, and § 9's sentence.
- **Q1's `m.number()` costs +66 real that every reader of the spec pays, for a form only a package's author
  writes**: t1's two tasks never show it to the reader (the package hides it behind `poll.add_mailbox`). Under
  §1.2 that is a token with no rewrite-rate return in any program a reader writes; under rank 3 it is what
  lets a wait be rung by the send itself, so no ring is forgotten (P1 item 1), against the waker's two acts.
  Recorded as a cost, as the 2026-11-22 rule asks.
- **No named removal is offered by any arm, and none by me**: under the payment rule (§1.6; panel 012) every
  addition owes one or a registered prediction. S1 to S28 above are that registration for this sitting's
  texts; the removal I would name is none, and I say so.

## 4. Where the generic rule belongs (item 3)

**The principle belongs in § 9, the send's `@` in § 11.** *"Judged at each call, on the types that call
gives"* is a property of generics, not of threads: § 9's generics bullet is where a reader looks to learn what
a type parameter means, and § 11 already holds a sentence about a letter only for `sort`. **One sentence can
say it for every rule on what a type holds** (`spec-w9.md`, in § 9: *"A rule on what a type holds (sections
11 and 13) is judged at each call of a generic function, on the types that call gives its type parameters,
and told at that call"*); its vendored price over saying it in § 11 alone is **+17** (`spec-w9.md` +150
against `spec-wi.md` +133), about +22 real by the ratio, an inference. **What it reaches beyond threads**:
§ 13's *"Comparing a `partial` record ... and using it as a map key are compile errors, for it and for any
value holding it"* and *"a map key is an error"* for a handle, so § 4.2's `e2` becomes a compile error at the
call. **That reopens panel 155's ruling** (its R2 closed `==` on a letter alone, its R3 left the float key
through a generic waiting, and its call-site pass was refused under a soundness veto for a nesting give-up):
the sentence costs the spec 17 vendored tokens and the compiler whatever the instance-complete check costs,
which is the compiler-engineer's to price; **the text does not need two sentences to say one principle**.
What it must NOT reach: `sort` and (once stated, T7) `==` on a bare letter, which are refused in the body, a
stricter rule; § 9's sentence speaks of rules on what a type HOLDS, which neither is.

## 5. For the blind seat and the synthesis

- **Each repair keeps the rule its rows grade** (the brief's condition for my predictions): `spec-wc.md`
  keeps the rule on the writing (program 1 refused) and changes nothing a t2 or t3 row reads (no program
  reads `n` after the send); `spec-wi.md` keeps `gi`'s per-kind rule and states program 4's half (R-base);
  `spec-wr.md` keeps every refusal and drops the advice T5 found misleading, and t3's `gr` row already allows
  both shapes. **If the run uses the arms unrepaired, S8's program 1 is where `gc`'s two readings show.**
- **t1's row (7) meets T0**: *"nothing the spec refuses ... a missing `@`"*; a worker that writes
  `m.send(@r)` for a `Result` (a record holding no handle) is refused by the compiler (`marker_mismatch`) and
  by NO sentence of the frozen spec both t1 arms read. The row should say which it grades (the text, as the
  row's own words say), so a grader does not mark a program wrong for a rule its reader was never shown.
- **The `now` cell by the text** (critic 16): T4 agrees: the frozen text refuses programs 3 and 5 and accepts
  2, and decides program 4 neither way, outside a generic as inside one (T0).

## Verdict

- `verdict`: **object** (not provisional: every count above is `claude-opus-5`'s through `--refresh`), to
  P2 as worded (`gc`) and to `gr`; **approve `gi` repaired with R-base and § 9's sentence** (`spec-wadopt.md`),
  and Q1's `m.number()` repaired (`spec-wq.md`). No veto: the frozen agent file says neither veto holds
  against the most robust resolution, and none of these texts passes the ceiling.
- `section`: design.md §1.2 (a writing whose meaning depends on where it stands, `gc`'s T2), §1.6 (the costs,
  measured), §4.12 (`gr`'s advice, T5); CLAUDE.md § 12 for T4.
- `spec_token_delta`: real 12,809 → `q1` 12,875 (+66), `gc` 12,875 (+66), `gi` 12,915 (+106), `gr` 12,898
  (+89); my route 13,083 (+274); the proposal repaired 13,074 (+265). All six measured.
- `removal`: nothing, and that is a problem: paid by registered predictions S1 to S28 under the 2026-11-22
  rule, not by a removal.
- `needed_for_self_hosting`: no (one generic in `selfhost`, which sends nothing).
- `argument`: `gi` is what § 11 already says (T4): the frozen text refuses programs 3 and 5, so defect 2695
  is the compiler's, and `gi` adds only where the error is told and the marker rule's missing half (T0),
  which the spec lacks outside generics too. `gc` changes the language and makes `m.send(@n)` of an `i64`
  required inside a generic and refused outside it (with R-base stated): one writing, two meanings by place,
  the §1.2 rewrite cost this seat prices, and an "emptied number" the document never defines (T2). `gr`
  refuses programs the text accepts and its advice leads a handle back to program 5's run-time abort (T5).
  § 9's one sentence closes `e2` too, for 17 vendored tokens.
- `prediction`: S26: a resolution arm on `spec-wadopt.md` scores 5 of 5 on each of t2's six programs (each 4
  to 5) and 5 on t3; and S8: `gc` unrepaired scores at most 4 on program 1.
- `condition`: the compiler-engineer showing that no instance-complete check exists (no seam after `mono`,
  a nesting give-up left, panel 155's veto condition): then `gr`, sound in the body by construction, is the
  robust route and I would adopt `spec-wr.md`. Or the blind run's `gi` cell failing D2 (its text read right
  on fewer than five programs) where `gc`'s passes.

**Q1, the route I would adopt**: P1 item 1, the mailbox a source the wait takes, its language half
`spec-wq.md` (+66 real as `q1`, about +84 repaired, an inference), over the waker, whose ring can be forgotten
and is accepted by the build. **The measurement that would change my mind**: a `poll.add_mailbox` written in
Rebel with no language form (critic 11's forwarder, under `gi` two of them, one per kind) stressed to zero
lost wakes, which would make the 66 tokens buy nothing a package cannot; or the runtime showing that a
number's slot is reused with no generation, which would make *"never another"* a promise the text cannot keep.

**Q2, the route I would adopt**: `gi`, repaired, with R-base and § 9's sentence (`spec-wadopt.md`, 13,083
real with Q1). **The measurement that would change my mind**: the compiler-engineer's, that the per-instance
check cannot be made complete; then `gr` repaired (`spec-wr.md`).
