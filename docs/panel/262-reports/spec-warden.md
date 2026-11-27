# panel 262, spec-warden: report (started 2026-11-26 21:46)

Written as it goes, in the copy `.claude/worktrees/262-spec-warden` (detached at the base `87003cb42`, `git log -1`:
`87003cb42946dc0f99dbab36582693c4afb3777a 2026-11-26 20:44:29 +0100`). Every number carries the command that produced
it, run in this session; what was not run says *unrun*. `TMPDIR` is `tmp/` inside the copy.

## 0. What I read, and the ground as it stands now

- design.md §1.6 by `grep -n '^### 1.6'` (line 255): **the ceiling is 16384** real tokens, `claude-opus-5` through
  `POST /v1/messages/count_tokens`, and the payment rule is unconditional; §1.2 at line 192 (real cost = tokens × (1 +
  rewrite rate)). Under CLAUDE.md § Precedence as amended 2026-11-22 the budget and Principle 0 are costs written here,
  never a veto; my veto is on the soundness of a text.
- The seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in the copy, `real 111.25 user 22.27`
  (load 37 to 85, `uptime`: no duration here is a measurement), `./heroes --version`: `heroes 0.6.0`, **`sources
  1e1eb23a1927d35b`**, the brief's. **Stage 2**: `./heroes build selfhost/main.hero -o heroes-next`, `real 714.05 user
  185.47 sys 36.74`, exit 0, `./heroes-next --version`: **`sources 6ce3f9e116d22e3a`**, the brief's
  (`build-spec-warden.log` beside this report). Every count and run below is this `heroes-next`'s.
- **The usage limit stopped this seat at about 22:00**; resumed 22:15 on the coordinator's word, the copy and this
  report found as left (the build had finished, `exit 0`). `main` read `deef27472` (22:15:39) on resuming, and `git log
  --oneline c2b7574bf..main -- spec runtime tests/harness/spec_refusals.hero tests/harness/suite_spec.hero` printed
  nothing: the ground below stands.
- **`main` has moved past the brief's `0bde00d84`**: `git log -1 main` reads `c2b7574bf` (2026-11-26 21:38:52 +0100),
  and `git log --oneline 87003cb42..main -- spec runtime` names four commits: step 203f (panel 261's spec text), step
  199e (`01706218e`, **defect 2853**), the merges, and **step 199f (`8d8263f15`, 21:38:42, defect 2852, class blocking:
  *"the signal guard's walk of frame records ends at the top of the coroutine's stack it runs on, so a fibre's callback
  is not refused for a delivery left above it"*)**, `git show -s 8d8263f15`. So **the lane's G and J have landed on
  `main`**: the one program the brief names as refused with no handler of C's (`mj_carved`, `--sanitize`, this Mac) is
  the case 199f's body says now passes (*"the fibres from exit 134 at `--sanitize -O0` ... to passed"*, the lane's run,
  not mine; my own second column is below). `git diff --stat 87003cb42 main -- runtime spec ...`: `sigguard.c` +23 −2,
  `sigentries.c` +8, `rebind_entries.c` +9, `rebind_jump.c` +28 −1, `sigjump_save.c` +6, the spec +18 −8,
  `suite_spec.hero` 8 lines, design.md 47.
- **The spec `main` holds** (`git show main:spec/heroes-spec.md`, kept as `programs/spec-warden/spec-main-c2b7574bf.md`):
  526 lines, 36,120 bytes (`wc -l -c`), sha256 `9ff7d2a1bfda4323e54de004b27ef522bd86885a3fa249eb20406fb2a2942fc3`
  (`shasum -a 256`), **byte-equal to the brief's `ground/spec-at-0bde00d84.md`** (`cmp`: no output); the handler
  sentence on line 438 (`grep -n 'left a handler'`). The base's: 516 lines, 35,246 bytes, sha256 `3df14254...d12d93f`,
  the sentence on line 428. **Both diffs of every text are written: against `main`'s spec (priced) and against the
  base's**, because the physical line that holds the sentence differs between the two at its head (panel 261's *"with
  neither mark,"*), so no one hunk can apply to both (item 8 below says what `patch --dry-run` read).

## 1. The counts (item 1 of the brief's counts; every number `heroes-next measure`, this copy, no `--refresh`)

**No `--refresh` was authorised**: the coordinator's prompt names none, and the launch instructions say *"Never: ... a
paid run"*. So **every candidate's count below is a lower bound, vendored only**, in those words, and no real count of
a text is written here (`.claude/rules/spec-shape.md` § How a change to the document is made).

- **Before, the base's spec** (`./heroes-next measure spec/heroes-spec.md`, `programs/spec-warden/measure-base.txt`):
  claude-legacy **10,067**, cl100k_base **10,245**, **real 13,439** (the pin, `claude-opus-5`, 2026-11-26), headroom
  2,945, 13,499 after the FFI floor's 60: **byte-equal to the brief's § 3.7**.
- **Before, `main`'s spec** (`measure-main.txt`): claude-legacy **10,318**, cl100k_base **10,507**; no `real` row (a
  path other than the pinned one). `main`'s `suite_spec.hero` pins `REAL_TOKENS` 13776, **read, not measured** by me.
- **The pieces** (`programs/spec-warden/texts/`, cut by `apply.py`'s constants and measured): the handler sentence
  **88 and 88**, the room's sentence **90 and 91**, line 437's head *"A callback is a **parameter**, ... nor called by
  one;"* 21 and 21. **The clerk's 88, 90 and 91 re-read, the same.**

| text | candidate | `main` + text (legacy, cl100k) | delta, vendored | the text alone | lines | instrument |
|---|---|---|---|---|---|---|
| `spec-w1` | (a) two sentences by kind | 10,381, 10,571 | **+63, +64** | 151, 152 | 526 | opening kept |
| `spec-w2` | (b) the property | 10,500, 10,689 | **+182, +182** | 270, 270 | 526 | opening moved |
| `spec-w3` | (c) one clause | 10,327, 10,516 | **+9, +9** | 97, 97 | 526 | opening kept |
| `spec-w4` | (d1) no list | 10,298, 10,487 | **−20, −20** | 68, 68 | 526 | opening moved |
| `spec-w5` | (d6) merged into the room's sentence | 10,338, 10,527 | **+20, +20** | 197, 198 | 526 | opening moved |
| `spec-w6` | (d7) line 437's home | 10,289, 10,478 | **−29, −29** | 79, 79 | 526 | opening moved |
| `spec-w7` | (d2) a promise per platform | 10,366, 10,556 | **+48, +49** | 136, 137 | 526 | opening moved |
| `spec-w8` | (d5) the words alone, no sentence | 10,231, 10,420 | **−87, −87** | 0 | 526 | spec side retired |
| `spec-w9` | (d8) abort at the exit | 10,318, 10,507 | **0, 0** | 88, 88 | 526 | opening moved |

The same deltas on the base's spec (`measure-texts.txt`: base + w1 10,130 and 10,309, ...; w2 10,249 and 10,427), every
one equal to `main`'s. **The real price of each is unmeasured.** As an inference only, the base's real-to-cl100k ratio
(13,439 / 10,245 = 1.31) puts w2 near +240 real and w1 near +85: a guess at a price, never a price. Line counts: every
text keeps the spec at 526 lines on `main` and 516 on the base (`wc -l`), so `claims.ts`'s *"fewer than 600 lines"*
moves by none.

**The diffs** (`spec-wN.diff` against `main`'s spec, `spec-wN-base.diff` against the base's; `apply.py` writes both
from the one text, asserting each replaced span occurs once): `patch --dry-run` (`programs/spec-warden/patch-dry-run.txt`)
reads, for every text, **the `main` diff applies to `main` and is refused by the base, the base diff applies to the
base**: the physical line holding the sentence opens with panel 261's *"with neither mark,"* on `main` alone. **I
priced `main`'s**; the deltas are the same on both.

**The hashes** (`shasum -a 256`, first 16 digits): texts w1 `a4bb3995ecc59bbf`, w2 `61ca48f7c78699e6`, w3
`d3d3985cefa8157f`, w4 `2b063d4a2551990e`, w5 `59cddfce849f033a`, w6 `bd6fa6873a76f926`, w7 `0ba4c98dd1ee631c`, w9
`62e90d1f39e846b2`; `main` with the text applied (`programs/spec-warden/applied/main-wN.md`): w1 `7c57d49f6d48bb7c`,
**w2 `b1e76202bfc13a7c`**, w3 `27f8780323806867`, w4 `84388e86b53155da`, w5 `32084a25e02cbb24`, w6 `653e8667fb951e2f`,
w7 `1431de9bcf6f8aa2`, w8 `75ff8f1c508a1a70`, w9 `c23670c522df0a1b`.

## Predictions registered before the blind run

Registered 2026-11-26 about 22:40, before any blind session (none had started: the run waits for the synthesis,
`blind/registration.md`'s head). **Read against an arm whose diff is byte for byte `spec-wN.diff` applied to the landed
spec** (`main`, sha256 `9ff7d2a1...2fc3`); else *unpredicted*. Counts are of 5 readers per program, as the rules count.
**I expect `spec-w2` (b) adopted, and would make `spec-w1` (a) the `rival`**: w2 removes the list and says what ends a
handler; w1 keeps the list and its *may*, adding the no-handler clause, so the two arms separate the narrowing (D2) from
the widening (D1).

| rule (programs) | `now` (the landed spec) | `spec-w2` as `adopted` | `spec-w1` as `rival` or `adopted` | `spec-w3` | `spec-w4` |
|---|---|---|---|---|---|
| **D1** build-right `RUNS` of 5, each of `h1`, `h3`, `h13`, `n1`, `n2`, `n3` | `h3` 5, `h13` 4, `h1` 3, `n1` 3, `n2` 2, `n3` 2 (range 1 to 5); `NOT PROMISED` 2 to 3 on `n1` to `n3` | **`h1` 4, `h3` 5, `h13` 5, `n1` 5, `n2` 4, `n3` 4** (range 3 to 5); `NOT PROMISED` at most 1 each: **passes** | `h1` 3, `h3` 5, `h13` 5, `n1` 5, `n2` 4, `n3` 4; `h1` the one at risk (no word says a jump out ends the handler) | `n1` to `n3` 3 to 4, `h1` 3: **fails on one at least** | `h1` 4, `n1` to `n3` 4: passes |
| **D2** `ABORTS` of 5, each of `h7`, `h9`, `h10`, `h11` | `h7` 2, `h9` 1, `h10` 2, `h11` 1 (range 0 to 3) | **0 or 1 each: passes** | `h9` 0; `h7`, `h10`, `h11` 1 to 2 each (the list's *may* on *"a stack once given to `makecontext`"*): **fails on one at least, 60 %** | `h9` 0 to 1, the rest 1 to 2: fails, 60 % | 0 or 1 each: passes |
| **D3** build-right or text-right of 5, each of `h2`, `h5`, `h6`, `h8`, and the words | 5 each (range 4 to 5); words right on every `ABORTS` | 5, 5, 5, 4 (`h8`: *"a coroutine the signal did not interrupt"* read once as the interrupted one); words right | 5 each | 5 each | `h2` 4, `h5` 4, `h6` 5, `h8` 4 (no examples): passes, closer |
| **D5** `RUNS` of 5 on `h12` | 2 (range 1 to 4: L) | **0** (range 0 to 1): passes | 0 to 1: passes | 2 (silent, as `now`): fails | 0 to 1 |
| **D4** labels over the arm's wrong answers (both tasks) | W 8 to 14, H 4 to 10 (with W), L 2 to 4, T 1 to 3, N 2 to 6, I 1 to 3, P 0 to 2, R 1 to 4, S 1 to 4 | W 0 to 2, N 0 to 2, L 0 to 1, T 0, I 0, H 0 to 1, **P 1 to 4** (*"where this runtime can tell"* read as a platform split, on `h1`, `h7`, `h10`, `h11`), R 1 to 3, S 0 to 3 | W 0 to 2, **N 3 to 6**, L 0 to 1, T 0, I 0, P 0 to 2, R 1 to 3 | W 3 to 8, N 2 to 6, L 2 to 4, T 0 to 1, I 0 | W 0 to 3, P 1 to 3, R 1 to 3 |
| `h4` (D4) | no text-right; `ABORTS` 3 to 5 | `ABORTS` 4 to 5 (*"from inside the handler ... may abort"* text-right) | `ABORTS` 3 to 5 | as `now` | `ABORTS` 3 to 5 |
| `n4` (D4) | `ABORTS` 1 to 3 (no sentence) | **`ABORTS` 4 to 5**, words the thread's (w2 alone says it) | 1 to 3 | 1 to 3 | 1 to 3 |

**The spend** (the registration's caps, 1.00 per t1 session, 0.60 per t2, 1.50 the verdict; its inference 13 to 18 USD
for 31): **31 sessions about 14.5 USD (range 12 to 18), 21 sessions about 10 USD (range 8.5 to 13)**, under the 25.00
ceiling, no session cut by its cap (an inference from 259's 0.43 mean t1 cost scaled by input bytes, the spec 1.1 KB
longer under w2).

**What would falsify my recommendation**: w2 failing D1 on `h1` (2 or more `NOT PROMISED` on it, read as P), or D3 on
`h8`; w1 passing D2 on all four (then the list's *may* does not invite N, and w1, 119 vendored tokens cheaper, is the
better text).

## 2. The second column: `main`'s runtime, this Mac (the decisive rows for every positive clause)

`git archive main runtime` (main `deef27472e915dc50f839edfd34e955e5fa34473`, 22:15) into `tmp/main-rt/` of this copy;
`diff -r runtime tmp/main-rt/runtime` names five files (`rebind_entries.c`, `rebind_jump.c`, `sigentries.c`,
`sigguard.c`, `sigjump_save.c`: steps 199e and 199f). Built by this copy's `heroes-next` with
`HEROES_RUNTIME=tmp/main-rt/runtime`, plain and `--sanitize`, three runs each under `timeout 60`, by
`programs/spec-warden/key-col.sh` (`blind/key.sh`'s loop and normalisation over the programs named), the blind
programs and the clerk's probes copied unchanged. Output `programs/spec-warden/col-main-deef27472.txt`, 40 lines, every
cell **1 distinct of 3**:

| program | plain | `--sanitize` |
|---|---|---|
| **`mj_carved`** (no handler; defect 2852's probe) | exit 0, its lines | **exit 0, its lines** (`fibres carved, jumped into: 7`) |
| `escape_carved`, `escape_heap` (no handler) | exit 0 | exit 0 |
| `n1`, `n2`, `n3` (no handler) | exit 0, `answer ran for 7`, `result 70` | exit 0, the same |
| `h13` (the program counts the signal) | exit 0, `watch 0`, `answer ran for 7` | exit 0 |
| `h1`, `h3`, `h7`, `h9`, `h10`, `h11` | exit 0, `answer ran for 7` (`h9`: `left on the thread: 1` first) | exit 0, the same |
| `h2`, `h4`, `h6`, `h12` | exit 134, STANDING's words | exit 134, STANDING's |
| `h5` | exit 134, SWITCHED's | exit 134, SWITCHED's |
| `h8` | exit 134, ONSTACK's | exit 134, ONSTACK's |
| `n4` (a thread C made) | exit 134, the thread's words | exit 134, the thread's |

**And the first column, the base's runtime, for the one row that differs**: `mj_carved --sanitize` with this copy's
own `runtime/` (`col-base-mj.txt`): **exit 134, 1 distinct of 3, `panic: mjcarved.run was called from inside a
signal handler: C called it while handling a signal`** (CALLED's words), plain exit 0; both first outputs kept beside
this report (`col-main-out-...`, `col-base-out-...`). So **a program whose C installs no signal handler is told a
handler's words at the base and runs on `main`, on this Mac**: step 199f (defect 2852) is what every positive
no-handler clause below waits for, and it has landed on `main` (its issue open until the batch's gate, the brief's
§ 3.5). Every other cell equals the repair's key at the base (`blind/registration.md` § The key). **Linux arm64, the same
column** (one container, `p262-spec-warden`, `docker run --pull never --rm`, `heroes-linux-arm64:latest`
`338a1044ac90`: Debian 13, `ldd (Debian GLIBC 2.41-12+deb13u3) 2.41`, Debian clang 22.1.8; over `git archive
87003cb42` with `main`'s runtime at `/w/main-rt/runtime`; the seed **`sources 1e1eb23a1927d35b`**, stage 2 **`sources
6ce3f9e116d22e3a`**, exit 0 each; `programs/spec-warden/linux-arm64/`): **32 cells, each 1 distinct of 3, every verdict
this Mac's**: `mj_carved`, `escape_carved`, `escape_heap`, `n1`, `n2`, `n3`, `h13`, `h1`, `h3`, `h7`, `h9`, `h10`, `h11`
exit 0 plain and `--sanitize`; `h12` (STANDING) and `h8` (ONSTACK) exit 134 both; `n4` exit 134 in the thread's words
both (`W-main.txt`; `h2`, `h4`, `h5`, `h6` not run there). **Unrun by me**: Windows, route A (forced or Darwin x86-64),
the x86-64 image.

## 3. The texts, one per candidate, and where each clause is true (item 1)

**Facts every row below rests on** (each read or run in this session): **F1** the fourth question, the walk and the
exact test exist on Linux and Darwin only; Windows's `hero_callback_entered` *"counts and returns"* (`sigguard.c`'s
`#else` branch, line 714 on at the base, read): every *aborts* of a handler's is false on Windows, every *may* true.
**F2** the thread refusal stands on every platform: the thunk asks the key on POSIX (`sigguard.c:672-675`) and every
platform reaches `hero_thread_guard` at the function's own entry (`thread.c:145-148`, no `_WIN32` around it, `grep -n _WIN32 runtime/parts/thread.c`: none;
`callback.c:72`); run: `n4` exit 134 in its words on this Mac (column above), Windows unrun. **F3** route A (glibc's
other architectures, a moved glibc, a forced build, Darwin x86-64: `sigjump.c:45-53`, `rebind_decode.c:314-315`) holds
every range `makecontext` named (rule 7) and trusts a jump's buffer only on the thread's stack, its alternate stack or a
segment of an image loaded before any constructor (`sigjump_record.c:15-27`): a correct program a route C build runs
is refused there (`h7`'s shape by rule 7; `b_0`, `cs_0` by `d-lane-sig-family-1`). **F4** panel 258's stop is POSIX
only (`#if !defined(_WIN32)`, `sigguard.c:194`), and a jump its hooks never see leaves the bracket for guard D at the
next use (`sigguard.c:189-193`, `alloc.c:670-678`, `lend.c:216-225`); in no golden by its words (`grep -rlF 'jumped out
of the code it interrupted' tests/ | wc -l`: 0), guard D in 2. **F5** the no-handler class: false at the base on this
Mac under `--sanitize`, true on `main` (§ 2).

- **`spec-w1`, (a) two sentences by kind.** Sentence 1 is `now`'s list with *"a signal handler on its thread"*, *"or
  into a coroutine the signal did not interrupt"* (`h12`, D5) and *"a jump into the handler's own frames"* (the
  pronoun gone): every clause a *may*, true on every platform, route and thread. Sentence 2, *"On a thread where no
  signal handler of C's has run (counting a signal with `hero_signal_watch` runs none), no coroutine switch or jump of
  C's makes a callback abort in these words"*: **true on Darwin arm64 at `main`** (§ 2: `n1` to `n3`, `mj_carved`,
  `escape_*`, `h13`, plain and `--sanitize`), **false at the base** (`mj_carved --sanitize`, CALLED), **and on Linux arm64 at
  `main`** (§ 2, the same programs, both builds); true on Windows by F1, on route A an inference (every arm of the
  fourth question names a handler the runtime saw begin). **Waits for defect
  2852** (step 199f, on `main`, issue open until the gate). *"In these words"* keeps it true of `n4` (the thread's
  words) and of the room's two refusals with no handler (their own words).
- **`spec-w2`, (b) the property** (the one I recommend): (i) *ends ... when it returns*: every platform. (ii) *"or when
  C jumps out of it by `longjmp`, `siglongjmp`, `_longjmp` or a `setcontext` to a context `getcontext` took, back to a
  call the signal interrupted, where this runtime can tell that the jump left it"*: on route C by rules 4 and 5 (`h1`,
  `h7`, `h10`, `h11` run, § 2, both builds, this Mac; the repair's key on Linux arm64 at the base); **on route A the
  scope is the runtime's own words** (HELD and UNFOLLOWED say *"whether the jump left the handler is not known"*), so
  the clause promises nothing it refuses there; Windows by F1. **It meets every falsifier of x1** (item 2). (iii) every
  other exit, *may*: true everywhere. (iv) the allocator jump, *aborts, at the jump or where it next uses that code*:
  Linux and Darwin at the jump for the jumps the hooks see, everywhere else at the next use by guard D (F4); **an
  inference from the code, no golden and no run of mine** (the watchdog is the compiler-engineer's). (v) the
  no-handler clause, as w1's sentence 2: waits for 2852. (vi) the thread: every platform by F2, run on this Mac.
- **`spec-w3`, (c) one clause**: `now` with *", a signal handler on its thread,"* and *"within the handler"* for
  *"within it"*: every clause a *may*, true everywhere and at the base; fixes T, H and I, leaves W (no positive
  clause), L (`h12` silent) and N (the *may* on `h7`, `h10`, `h11`).
- **`spec-w4`, (d1) no list**: one *may* over *"a signal handler of C's that C has not left by its return, or by a jump
  of the C library's back to a call the signal interrupted that this runtime can tell left it"*: true everywhere, at
  the base too; no example, no positive clause.
- **`spec-w5`, (d6) merged**: the room's sentence and w4's condition joined by *"; and where it leaves a signal handler
  of its own any way but ..."*: true everywhere (the room's part unchanged). One sentence of 197 and 198 tokens, the
  longest of the paragraph; the instrument then reads the room's sentence too.
- **`spec-w6`, (d7) line 437's home**: *"nor called by one, nor on its thread before C leaves it by its return or a jump
  of the C library's back to a call the signal interrupted that this runtime can tell left it, where a call may
  abort, ..."*, the list dropped: true (a *may*), but **it turns a description into the language's rule**: `h2`'s or
  `h5`'s call, correct C, becomes a call the line forbids, and *"its parameters follow the same rule"* now follows the
  new clause, so *the same rule* has two antecedents. Its diff touches panel 261's landed words' line.
- **`spec-w7`, (d2) a promise per platform: false as written.** *"On Linux with glibc on 64-bit Arm or x86 and on a Mac
  with Apple silicon, when C jumps out of it ... back to a call the signal interrupted"* (1) leaves out panel 258's stop
  (F4), so a watchdog whose signal lands in the allocator is promised a run on both graded platforms; (2) promises route
  C on every glibc, where `sigjump.c:45-53` puts a glibc whose layout moved on route A; (3) names systems the spec has
  never named (`LC_ALL=C grep -c -i -E 'linux|darwin|windows|macos'` of `main`'s spec: 0; *platform* twice, lines 340 and 411). Its *"elsewhere (an Intel Mac, another C library or system, Windows) ... may
  abort"* is true and promises musl and FreeBSD nothing.
- **`spec-w8`, (d5) the words alone**: nothing false; the spec then names neither claim, the instrument's spec side is
  retired, and panel 257's R1, which the author asked for (*"the author asked for the words"*, 257's R2 note), is
  reversed.
- **`spec-w9`, (d8) abort at the exit: false today on every platform.** *"C leaving a signal handler of its own by a
  `swapcontext`, a `setcontext` or a jump into a coroutine the signal did not interrupt aborts there"*: the runtime
  aborts at the next callback (`h5` and `h8`, § 2: the abort is the callback's, in SWITCHED's and ONSTACK's words); and
  the runtime change it needs would refuse correct C that never calls back after (the brief's `h5` twin): a widening of
  refusals I price at 0 vendored tokens and do not offer.
- **(d3) a runtime change so the sentence can be simpler**: its first half has landed: G (199f) is what makes w1's and
  w2's no-handler clause true (§ 2). Its second half, a runtime that refuses every exit it does not follow so that *may*
  becomes *aborts*, would refuse `h7`, `h10` and `h11`, which run: correct programs refused, the `blocking` class
  (`.claude/rules/verification.md` § Bounded discovery). Would have to be true: none of those programs is correct C. No
  text.
- **(d4) a told end**: would have to be true: a C library that calls a `hero_os.h` function when its handler is over;
  none does, a binding author would wrap every handler, and panel 259 refused a told switch as *"a `hero_os.h` promise
  for ever"* (`docs/panel/259-*.md:200-203`, re-read: *"under E a tell buys nothing a program needs ... and would be a `hero_os.h`
  promise for ever"*). No text.
- **(d9) a told route**: would have to be true: `heroes doctor` printing the route and a public declaration of
  `hero_signal_jump_route`. Neither is: `grep -n route selfhost/cli/doctor.hero` prints nothing, and `grep -rln
  hero_signal_jump_route runtime selfhost` names only `runtime/parts/sigjump.c`, `rebind_decode.c` and `rebind.c`; the
  route case declares it itself (`fixedbugs-2679-the-runtime-says-which-route-decides-a-jump.h:7`). With both, w2's
  *"where this runtime can tell"* could add *"(`heroes doctor` says where)"*: a tool-surface change, a panel's.

## 4. Items 2 to 5 and 7 to 10 of the brief (item 6, the instrument, is § 5)

**Item 2, x1 is false as written; w2 reuses its exits only under every falsifier.** x1's *"carries the thread out of
it onto the thread's own stack, and a callback then runs"* is false at the base and on `main` for `fixedbugs-2705` and
the own-stack coroutine (ONSTACK: `h8`'s cell in § 2 is that shape), for panel 258's stop (F4), on route A (F3), and
it scopes the thread only in its second half. w2 holds each: *"back to a call the signal interrupted"* excludes a jump
into a coroutine the signal did not interrupt (2705, 2702's own-stack coroutine, `h8`, `h12`: all in its *may*);
(iv) says the allocator jump aborts; *"where this runtime can tell that the jump left it"* is route A's own words;
*"for the callbacks of its thread"* and *"on that thread"* scope it (`h9` runs). **w1, w3, w4, w5 and w6 promise no run
after a jump at all**; w7 reuses x1 unscoped and is false (§ 3).

**Item 3, the word *may*.** A clause can say *aborts* only where every platform aborts: the thread's (F2) and the
allocator jump (F4, at the jump or at the next use). The handler's own aborts cannot: Windows refuses none (F1), and
route A refuses what route C runs (F3). So *may* stays in every text, and w2 says what it is in the spec's own
precedents' shape, a condition of the runtime stated beside the abort, as line 435's *"aborts, or fails the call where
the kernel writes"* and line 497's *"unless C has since handed its address out again"* do: the handler ends *"where this
runtime can tell that the jump left it"*, and a callback before then *may* abort. **A reader who takes that for a
platform split (P) reads route A right**, so under w2 P is not a misreading; under `now` it was a misreading only where
the platforms agree. Panel 257's two measurements: Darwin's *"until 2702"* has moved (`h5`, `h8` abort the same on both
arm64 platforms in the repair's key, and `sigentries.c:9-15`); Linux's depth test is **unrun by me**, and no text here
depends on it (every unfollowed exit stays a *may*).

**Item 4, the word *handler*.** w1, w2 and w3 say *signal handler* where `now` says *handler*, and w2 never uses the
bare word for anything but the signal handler it opens with. **The program's C or the process**: *"of C's"* is C code
the program links, never this runtime's: w1 and w2 name the one runtime handler a reader writes a call for (*"counting
a signal with `hero_signal_watch` runs none"*, `h13`). The runtime's own SIGSEGV and SIGBUS handler (`stack.c:1269-1282`)
is not named: it either aborts on a stack overflow or chains to a handler C installed before it, which is then C's
running. Rebel's `os.ledger:454` quotes the remedy, which no text touches.

**Item 5, the pronoun and the thread.** w1 and w2 write *"a jump into the handler's own frames"*, w3 *"within the
handler"*: no *it* whose antecedent is *a stack*. Thread: w1 and w3 *"a signal handler on its thread"*, w2 *"for the
callbacks of its thread"* and *"on that thread"*, w4 *"on the thread of a signal handler"*, w5 *"on that handler's
thread"*, w6 *"on its thread"*.

**Item 7, what a reader of the whole paragraph meets.** The order stays: line 437's rule, the room's sentence (its own
abort and words), the handler text, the remedy. w2 adds the two aborts of a handler's doing that no sentence names
(panel 258's stop, the thread's), each beside the callback, their home (`.claude/rules/spec-shape.md`: *"each site that
aborts says so beside its operation"*), and lists no other abort (panel 087). **§ 9's *"running out of stack aborts"* on
a C coroutine's stack** (the critic's C28) is unrun by me and touched by no text: a label S answer is reported, not
graded. **Merging** (w5, +20) is the shape the rule prefers and here the costlier one per reader: one 197-token
sentence with two unrelated aborts; I do not recommend it.

**Item 8, panel 261's text.** Landed (step 203f); `main`'s spec is byte-equal to `0bde00d84`'s and to `deef27472`'s
(`git log c2b7574bf..main -- spec`: nothing). Every diff is written against it and priced there; each has a base twin
(§ 1). w6 alone changes line 437, the line panel 261's words share.

**Item 9, the record a text falsifies or supersedes.** Under w2: panel 257's R1 sentence is **replaced** (its two
claims kept); R2's x1 is **superseded** (its exits reused under the falsifiers; R2's *"it lands in a later amendment
when"* is answered by this sitting, not by x1); the eight arms move by **no word** (both claims kept, the instrument
reads them, § 5); `sigguard.c`'s module docs that quote the sentence (REENTERED's comment, `sigguard.c:557-567` at the
base: *"spec § 13's sentence on a callback called after a jump says a refusal says ..."*) stay true word for word;
`spec_refusals.hero`'s head comment moves with its opening (the instrument diff); Rebel's `os.ledger:454` is untouched.
**design.md owes no line**: it refuses coroutines in the language (`main`'s `docs/design.md:3611`, `:3642`,
`:4184-4190`, `grep -n -i coroutine`), and every text speaks of C's coroutines, as the room's sentence already does; the
guard's rules keep their home in the runtime's module docs.

**Item 10, what the landing owes, per text.** The four pins of the `spec` suite and the ledger row (every text), from
one `--refresh` at the landing; **the `run` goldens: none move by form for w1 to w7** (no arm's words change; the 91
claim-holding `run` cases and the `check` case `package-a-file-source-in-no-repository` stay as they are, and a `grep
-rl` rewrite of the latter would be wrong); the compiler's test of 2533 holds the remedy, which every text leaves
byte-equal and after it; the instrument diff (w2, w4, w5, w6, w7, w9; w1 and w3 keep the opening); **w2 owes a golden of
panel 258's stop** if (iv) is to be held by a run (0 today, F4) and one of the no-handler class on this Mac under
`--sanitize` (2852's own `fixedbugs-2852-*`, landed with 199f, is that case); **the site's build and its publication**
(the site serves the spec verbatim; every text keeps 526 lines, under `claims.ts`'s 600).

## 5. Item 6: the instrument first, then the text, each run

**The instrument's change** (`instrument-wN.diff`, made by `programs/spec-warden/instrument.py` from the base's
`tests/harness/spec_refusals.hero`, sha256 `f9fc613e...86cf`, `git show 87003cb42:` and the saved copy equal): **(1)**
`SENTENCE_OPENS` set to the text's opening where the text moves it (w2 and w7 *"A signal handler of C's ends, for the
callbacks of its thread"*, w4 *"A callback called on the thread of a signal handler of C's"*, w5 *"Where C leaves a
callback's frames"*, w6 *"A callback is a **parameter**"*, w9 *"C leaving a signal handler of its own"*; w1 and w3
keep it); **(2)** its own test's synthetic sentence built as `"text " + SENTENCE_OPENS + " by a jump, ..."`, so the
test stops hard-coding an opening a text may move (in every diff, w1's and w3's too); **(3)** its head comment. Each
opening occurs once in the spec it reads (`grep -c -F`, 1 in every `applied/` file). `CLAIMS`, `EXEMPT` and the
extractor are untouched: **no arm moves, no claim moves**.

**Run** (`programs/spec-warden/judge.sh`: the base's spec in this copy replaced by `applied/base-wN.md`, the
instrument by `instr/spec_refusals-wN.hero`, then `./heroes-next run tests/harness/main.hero -- ./heroes-next spec`
and `./heroes-next test tests/harness/spec_refusals.hero`, then both restored and compared by `shasum`; outputs
`programs/spec-warden/judge/`, summary `judge-summary.txt`, last line *"restored: 3df14254...d12d93f
spec/heroes-spec.md, f9fc613e...86cf tests/harness/spec_refusals.hero"*, exit 0):

| text | `spec` suite | which fail | the instrument's own tests |
|---|---|---|---|
| w1, w2, w3, w4, w5, w6, w7 | **32 passed, 4 failed** each | `spec/budget`, `spec/ledger`, `spec/real`, `spec/spendable`: the four pins a landing moves, as panel 257's sentence did | **14 tests, all passed** each |
| w9 | 34 passed, 2 failed | `spec/real`, `spec/spendable` (88 = 88 vendored, so the vendored pins hold; the digest moves the real one) | 14, all passed |
| w8 (no sentence, instrument unchanged) | 31 passed, **5 failed** | the four pins **and `spec/refusal-words`**: *"spec § 13 holds no sentence opening ..."* | **14 tests, 1 failed**: its test reads the real spec |

So **`spec/refusal-words` holds every text but w8**, both claims found in the sentence the instrument cuts, and the
eight arms each keep one. **w2 was judged in its first wording** (its second sentence then said *"stops the program, at
the jump or at the next allocation"*; the instrument reads the first sentence alone, which did not move); **the final
w2, re-judged** by `judge-net.sh` (`judge-net-summary.txt`, `judge/spec-w2-final.txt`): **`spec` 32 passed, 4 failed**,
the four pins (*"the spec measures 10427 and the record says 10245 — a delta of 182"*), **the instrument's own tests 14,
all passed**, both files restored by `shasum`. **The net's own tests whole (`./heroes-next test
tests/harness/main.hero`, w2 and its instrument applied) are unrun**: started 22:56, they had printed 6 `ok` lines in
about 16 minutes at a load of 125 to 200 (`uptime`), against 913 `test` blocks in `tests/harness/*.hero` (`grep -c
'^test "'`, summed), so I stopped that one process (`kill 94966`, its `cwd` this copy, `lsof`), and the script restored
both files (*"restored: 3df14254...d12d93f, f9fc613e...86cf"*, `git status --short`: only the untracked report folder
and `tmp/`). What they would add over the runs above is every other test of the harness that reads the real spec; the
one test that reads this sentence is the instrument's own, run per text. **A split sentence needs nothing more**: the
instrument reads to the first `". "`, and in w1 and w2 that is the sentence holding both claims; w1's second sentence
and w2's second carry no claim, the room's and the thread's words being their own. **The three arms in no golden**
(CALLED, HELD, UNFOLLOWED) matter to w2 alone: HELD's and UNFOLLOWED's *"whether the jump left the handler is not
known"* is the scope of its (ii), held by the instrument as text and by no run on a graded platform (neither is route
A). **The two stops with neither claim**: panel 258's (0 goldens) is a claim w2 makes, (iv), and owes a golden at the
landing; guard D's (2) is its *"where it next uses that code"*.

## 6. Verdict (the agent file's structure)

- `verdict`: **approve `spec-w2` (b), provisional** (every count a lower bound, vendored only: no `--refresh` was
  authorised). **Veto `spec-w7` (d2) and `spec-w9` (d8) on soundness**: each says what the runtime does not do on a
  platform it claims (§ 3). Object to `spec-w8` (d5) and `spec-w6` (d7). w1, w3, w4 and w5 are sound and weaker.
- `section`: design.md §1.6 (the budget: a cost written, never a veto, under CLAUDE.md § Precedence as amended
  2026-11-22), §1.2 (the rewrite rate), and CLAUDE.md § 2 (Principle 0: met by the thesis, a reader's prediction, the
  blind run being the measured argument the panel accepts); the soundness veto stands on design.md §1.12 (robustness)
  and CLAUDE.md § 12 (*a refusal is held to the same standard as a feature*).
- `spec_token_delta`: `main`'s spec 10,318 and 10,507 → **10,500 and 10,689, +182 and +182 vendored (a lower bound)**;
  the base's 10,067 and 10,245 → 10,249 and 10,427. **Real: unmeasured** (the base's pinned real is 13,439; `main`'s
  pin 13,776 read, not measured); about +240 real by the base's 1.31 ratio, an inference.
- `removal`: the handler sentence itself (88 and 88) is replaced; net +182. **The payment is the registered
  prediction**, not a removal: D1, D2 and D5 of `blind/registration.md` under `adopted`, scored by the blind run that
  exists and is authorised (§ Predictions above).
- `needed_for_self_hosting`: **no**.
- `argument`: `now` names five mechanisms and readers fill the rest four ways (W, N, L, T). w2 states the property the
  runtime tests: a signal handler of C's ends, for its thread, at its return or at a jump back to a call the signal
  interrupted that the runtime can tell left it; before then a callback *may* abort, the one honest word while Windows
  has no probe (F1) and route A refuses what route C runs (F3). It promises runs only where run: no handler on the
  thread (§ 2, `main`, both arm64 platforms, both builds), and `h1`, `h7`, `h10`, `h11`. It names the two aborts of a handler's doing no
  sentence names, panel 258's stop and the thread's. It meets every falsifier of x1.
- `prediction`: under `adopted` = w2, D1 holds for all six programs (each at least 4 of 5 `RUNS`), D2 0 or 1 `ABORTS`
  each, D5 `h12` 0 or 1 `RUNS`; the landing's `--refresh` reads w2's real delta between +210 and +270.
- `condition`: (1) a no-handler program refused in these words on a graded platform at `main` (§ 2 read none, on
  either arm64 platform, both builds): then w2's clause (v) waits for that defect; (2) w1 passing D2 on all four while w2 fails D1 on `h1`: then w1, 119 vendored
  tokens cheaper; (3) a watchdog whose signal lands in the allocator running past the jump on Linux or Darwin: then
  (iv)'s *"at the jump"* goes.

## 7. One line per numbered item: the text I would adopt, and the one measurement that would change my mind

1. **The texts**: `spec-w2` (b), **+182 and +182 vendored on `main`, a lower bound, vendored only**; the measurement:
   a `--refresh` of w2 and w1 at the landing showing w2's real delta above +300 while the blind run passes w1 on D2.
2. **x1**: not adopted; its exits survive only inside w2's (ii), held to all five falsifiers; the measurement: a
   program jumping *back to a call the signal interrupted*, on the thread's own stack, refused on route C at `main`.
3. ***may***: kept for the handler's aborts, *aborts* for the thread's and the allocator jump's (w2); the measurement:
   a Windows run of `h4` refused (then F1 is wrong and *may* could narrow there).
4. ***handler***: *signal handler of C's* throughout, `hero_signal_watch` named as none (w2); the measurement: D4's H
   label on 2 or more of w2's wrong answers.
5. **pronoun and thread**: *"a jump into the handler's own frames"*, *"for the callbacks of its thread"* (w2); the
   measurement: D4's I or T label on any w2 answer.
6. **the instrument**: `instrument-w2.diff` (opening moved, the test built on `SENTENCE_OPENS`), `spec` 32 and 4 with
   the four pins alone failing, its own 14 tests green; the measurement: the net's own tests whole (unrun here) red on
   anything but a pin.
7. **the paragraph**: w2 as its own two sentences, not merged (w5); the measurement: the blind run's R label on 2 or
   more of w2's `n2`/`n3` answers (then the room's sentence and the handler text want one home, w5).
8. **panel 261**: every diff against `main`'s landed spec, a base twin each, w2's `main` diff priced; the measurement:
   none (a fact of `patch`).
9. **the record**: R1 replaced, x1 superseded, no arm, module doc or Rebel page moved, design.md owes no line; the
   measurement: a `git grep` at the landing finding the old sentence quoted outside spec, instrument and ledger.
10. **the landing**: the four pins, the ledger row, the instrument diff, a golden of panel 258's stop, the site's build
    and its publication, 526 lines; the measurement: the site's build refusing the landed spec.

## 8. What stays unrun

Every real count (no `--refresh` authorised); the net's own tests whole with any text (stopped, § 5); Windows, route A
(forced or Darwin x86-64) and the x86-64 image for every row; panel 258's stop (no program of mine; w2's (iv) rests on
the code); Linux's depth test of panel 257 (no text depends on it); § 9's stack abort on a C coroutine's stack (C28);
`h2`, `h4`, `h5`, `h6` on Linux arm64 at `main` (run on this Mac at `main`); the blind run (not mine).
