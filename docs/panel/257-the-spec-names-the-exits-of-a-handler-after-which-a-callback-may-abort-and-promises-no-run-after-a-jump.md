# Panel 257: the spec names the exits of a handler after which a callback may abort, says what the program is told, and promises no run after a jump

**Sat** 2026-11-23, from 00:37 to about 16:55 by this Mac's clock (`date`), interrupted by two API limits.
**Lane**: the **full panel** seated as the spec-warden, the compiler-engineer (Linux arm64), the ffi-pragmatist
(Darwin and real C), and the completeness critic on the briefs and on the reports; the llm-ergonomist
**registered and not run**, the historian not convened. **Tree**: `lane-p257` at `828567a30` (`rebel-round`'s head the
day panel 255 was ratified); `rebel-round` moved while it sat (head `22710bc62`, then `523d35537` with the runtime
lane's landing, then `b9129d41b` and `cab9f3cf4`), and **the seats re-ran their decisive rows against the landed runtime**
(`523d35537`) beside panel 255's two routes. **Status: provisional — author ratification pending**, and
**provisional on the blind reading**, which is owed. Briefs `257-briefs/`, reports `257-reports/`, evidence
`257-reports/evidence.tar.xz`.

## The question, and why the full panel

Panel 255's author's answer (2026-11-23, issue 2701, recorded as the session's *meant as* gloss of *1a* and *2a*):
*the words of the refusals R4 names are owed to the spec, a sentence written in a small sitting with the
spec-warden*. **Write the sentence that tells a reader which correct C programs a Heroes program's jump out of a
handler refuses, and what the program is told, true on both platforms**; it extends spec lines 427 and 428 (§ 13).
**Why the full panel**: the sentence is a spec token (`.claude/skills/panel/SKILL.md`: *"anything the spec must
state"*), so the soundness lane was not an option; the author's *small sitting with the spec-warden* is followed by
seating the warden first and the briefs smallest (three seats, a critic). **What it gave up**: no blind reader said
which program a reader of the sentence expects to run, because **no paid run is authorized**
(`257-briefs/llm-ergonomist.md`, `blind/registration.md`); no historian sourced a precedent, so there is **no
`## Prior art`** (none is claimed).

## The proposal, verbatim

`257-briefs/00-shared.md` § 5, the coordinator's drafts from panel 255's rows, issued twice. **First issue**
(`arms/v1/`, appended to the end of line 428): e1, e1b, e2, e3. **Second issue** (`arms/arm-f*.diff`, after the
critic's first reading, before `No function of the program is a signal handler`): **f1**, *"A handler of C's ends, for
the program, where this runtime sees it return or sees `longjmp`, `siglongjmp` or `_longjmp` take the thread out
of it; a handler left any other way (a jump no function of the C library makes, a switch of context never switched
back) or jumped within is held running, and a callback called while one is held running aborts, saying the
handler was not seen to end."*; **f1b**, f1 without the switch of context; **f2**, what runs first, then the
abort; **f3**, *"A callback called while a handler of C's has not been seen to end, by return or by `longjmp`,
`siglongjmp` or `_longjmp` out of it, aborts, saying so."* Vendored counts (`heroes-next measure`): e1 9,859, e1b
9,851, e2 9,818, e3 9,800, f1 9,854, f1b 9,846, f2 9,848, f3 9,805, against 9,761.

## The critic's first reading, and what it changed before any seat

`257-reports/critic-briefs.md`, 24 findings, every repair made before a seat started (`00-shared.md` names them *(B n)*).
What changed the sitting: **the placement** (B1, B2): the compiler's own test of defect 2533
(`selfhost/emit/callback_thunk.hero:354`) holds the end of line 428 to `HERO_SIGNAL_REMEDY`, the last words of every
signal refusal and 22 goldens, so a sentence appended there would be printed at the end of every refusal; **every
arm of the second issue goes before `No function of the program is a signal handler`**, and that test is **already
red at the frozen tree** (`c0476dff3`'s mailbox clause is in the spec and not in the runtime); **measured falsifiers
of the first issue** (B22, B23: the critic's p8, p9, p10 built and run on both platforms and both adopted builds):
*"a jump to a buffer the handler refilled"* is false (p10 leaves the handler and runs), E2's *"a buffer a save
filled"* is false (p3, p10), p7 (`swapcontext` away) splits (Darwin C4r refuses, Linux C2679 runs), the class the
runtime acts on is **a jump that lands within the handler**; the blind registration was not what the brief said
(B3 to B8, B18, B20) and was repaired; the platform scope (Windows has no probe, B12; the adopted Linux build was
never run on the CI's glibc 2.43, B13).

## What the seats measured (each in its report; the decisive rows)

- **The abort after an exit the runtime does not follow is a stack-depth test on Linux** (compiler-engineer § 4,
  3 of 3 on four builds): a callback called from the raise site runs (`p4s`), one about 5 KiB deeper aborts
  (`p4d` PAD 5,120 and up), a later handler clears it (`p4x`), another thread's runs (`b1`, `b2`). **On Darwin it is
  not** (ffi-pragmatist, § 4b): every refusal holds at every depth from the raise site's own level to 256 KiB below
  (2,460 runs on today and C4r, 810 on the landed runtime), **except `fp3`**, refused at `-O0` and `--sanitize`, run
  at `-O2`. So **no sentence can say *"a callback called from deeper than the handler"*; *"may abort"* is the only
  true word on both.**
- **`setcontext` out of a handler is not one rule**: to a context `getcontext` took it runs on every build
  (`b_0`, `b_1`, `p7s`, the critic's `n2`); a one-way `setcontext` onto a `makecontext` stack (`sc1`) is refused on
  Darwin on all three builds. **A switch of context *never switched back* is not the class**: `sched` (a green-thread
  scheduler) and a Boost.Context fiber switch back and are refused on Darwin.
- **A correct jump between two stacks the program made aborts on Linux** by mapping order (`cs2` ORDER=1; panel 255's
  `c_1` and `cs_1` on glibc 2.43: 18 of 20 runs), today, C2679 and landed alike, because `siginstall.c:150-160`
  compares raw addresses of stacks it does not know (compiler-engineer § 10).
- **The positive clause is false on Darwin by plain C**: `fp3` and the critic's `n6` (a `siglongjmp` onto the thread's own
  stack, the frame in memory once given to `makecontext`: refused at `-O0` **and** `-O2` once the signal is 260 KiB or
  more below the save point; runs at every depth on Linux), and on **route A** forced on both (jumps through heap,
  thread-local and late-loaded buffers; `fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`'s accessor).
- **The words** (critic § 4, 91 Darwin lines and every Linux S and K line): S is true of `p4`, `p6`, `p8` and the 2494
  golden and **its three causes are false of `fp3`, `as_0`, `n6` and `sched`**; U still prints on C4r and landed for
  `rr_0`, `rr_1`, `as_1` (a finding against R2's promise); **K** (*"called it while handling a signal"*) is printed for
  `as_1` on every Linux build; the lane's new SW words (*"had switched to another context ... had not been seen to
  switch back or to end"*, `HERO_SIGNAL_SWITCHED`) are true of what they print for, and the lane **did not argue** S's
  words true of Darwin `fp3` and `as_0`: **they are not, and a reader of them is misled; a sentence cannot repair
  printed words, it can only not promise what they contradict.**
- **The landed tree builds no Heroes program in `heroes-linux-arm64:latest`**: `runtime/platform/hero_ui_print_gtk.c:609`
  fails `-Werror=conditional-uninitialized`, on the landed tree and on `rebel-round`'s head (compiler-engineer § landing;
  re-run by the critic, § 1; whether the CI's clang refuses it is unrun). **It is not the sentence's; it is the
  coordinator's to carry to the lane that owns the file.**

## The verdict table

| seat | text | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|---|
| spec-warden | its `w26` (`spec-w7.txt`), approve; f1, f1b, f2, f3 object | **approve w26 only**, no veto | design.md §1.6, §1.2; CLAUDE.md § 2, § 12 | real **12,947 (+138)**; f3 +58, f1b +111, first pass +126 (four counts, `--refresh`) | the landed spec reads 12,947 (digest `5d451c5c...`); blind `now` p4 1 of 5 (0 to 2), `w26` 4 (3 to 5) | defect 2702 open at the landing: land the fallback `w27`; a blind run failing D2 withdraws it |
| compiler-engineer | w26, w27 | **object, no veto**; would approve w27 repaired (R-1, R-3) | design.md §1.12, §4.17 | the sentence 0 lines; the mechanism 3,289 lines of C | `cs2` ORDER=1 reads S at the landing if the sentence promises a run | (a) `cs2` runs or the sentence scopes it, (b) K named or only S promised, (c) *until 2702*, (d) Windows scope, (e) the landed tree builds in the CI's Linux image |
| ffi-pragmatist | w26, w27, the arms | **object, no veto**; nothing breaks the C ABI; none of eight real C libraries stops working; 422 `.dylib`s searched | design.md §1.12, §4.19 | none | the landed runtime leaves `sqlite3_progress_handler`'s binding without a shim | approve if (a) no *"though C is correct"* and no depth, (b) `setcontext` by its target, (c) no `__builtin_longjmp`, (d) *"whatever memory"* scoped |
| critic, second reading | x1, x2, w26, w27 | **would print none of the four unchanged**: the positive clause, falsified by `n6` and `fp3` on Darwin; *"A sentence that keeps x1's refusal side and says nothing positive has no program of mine against it"* | | | | |

## Where the seats disagree, stated plainly

- **The warden's w26 against both builders.** The warden approved its text, which lists `setcontext` as an exit the
  runtime follows and the depth as the abort's condition; the engineer measured the depth on Linux and the
  ffi-pragmatist found **no depth on Darwin and `setcontext` onto a `makecontext` stack refused** (`sc1`). Both objected,
  each on a different row; the critic's `n6` and `cs2` stopped w26 and w27 on both platforms. **The sitting goes with
  the measurement: w26 and w27 are not adopted.**
- **Whether the sentence may promise a run.** The warden, x1 and x2, and e1 to f2 promise that a callback runs after
  a `longjmp` out of a handler; the engineer's own cells hold on Linux (`every jump on the thread's own stack ran`),
  and the ffi-pragmatist's hold on Darwin **except `fp3`**; the critic's `n6` falsifies it on Darwin and the forced
  route A on both. **Not a disagreement left open: a promise a running program falsifies is not printed.**
- **The context clause.** The first issue's *"a switch of context never switched back"* is false of `sched` and the
  Boost fiber (Darwin) and true only in part of Linux until 2702; the warden kept `swapcontext` and the `makecontext`
  stack; the engineer wanted *"until 2702"*. **The sentence says *"may"*, which is true while Linux runs them and
  Darwin refuses, and stays true when 2702 lands.**

## The resolutions (provisional — author ratification pending)

### R1. The sentence, which says what may abort and promises nothing about what runs

Before `No function of the program is a signal handler`, in § 13 (diff: `257-reports/spec-sentence.diff`, **not
applied**; text: `spec-sentence.txt`):

> A callback called after C left a handler of C's by a jump no function of the C library makes (written in assembly,
> say), by a `swapcontext` away from it, by a jump or a `setcontext` onto a stack once given to `makecontext`, or by a
> jump that lands within it, may abort, saying the handler was not seen to end, or that C called it from inside one.

**Cost, measured** (`heroes-next measure spec/heroes-spec.md --refresh` on the sentence applied to a copy of the
frozen spec, 2026-11-23 at about 16:42 by this Mac's clock, **one count by the coordinator**, beside the warden's four; the key
counts tokens and does nothing else, the author's rule of 2026-09-09): **before real 12,809, after real 12,920, +111**
(claude-legacy 9,585 to 9,672, cl100k_base 9,761 to 9,848, both +87); headroom 3,464 against 16,384, 3,404 after the FFI
floor's 60; digest `0c7e93971e6440d6`. **The `spec` suite on the sentence applied** (`run tests/harness/main.hero -- <compiler> spec`
in a scratch copy): **31 passed, 4 failed**, the four pins the landing moves (`spec/budget`, `spec/spendable`,
`spec/real`, `spec/ledger`; 35 and 0 before), no check of the document's shape trips. Restored and compared by
`shasum` after each count. **At the landing the count is the landing's own**: `rebel-round`'s spec carries panel 256's
mailbox sentences (`git diff 828567a30 rebel-round -- spec`, 2 lines), so 12,920 is for the frozen file.

**What the sentence says and what it leaves out, each by a measured row**: it names **five exits** and promises only
*may*: the inline or assembly jump (`p4`, S at every depth on Darwin, at depth on Linux), the `swapcontext` away
(`p7`, `watchdog-b`: refused on Darwin, run on Linux until 2702; `sched` is refused on both landed builds), a jump or `setcontext` onto a stack once given
to `makecontext` (`sc1`, `as_0`, `fp3`, `n6`, `cs2`), a jump that lands within the handler (`p8`: line 427's
*called by one*). **It does not say**: that a callback runs after a `longjmp` (n6, fp3, route A); that the abort is
conditional on depth (Linux's and not Darwin's); that C is correct (the standards leave an asynchronous call undefined,
recalled, only this Mac's man pages read); that the words are true of every refusal (they are not); anything about
Windows (it has no probe, so *may* is not false there) or x86-64 (unrun).
**What it is told**: *"saying the handler was not seen to end, or that C called it from inside one"* is true of
every S, SW and K line the seats kept (critic § 4, 91 of 91 on Darwin, every S and K on Linux).

### R2. The positive clause is withheld; its text is recorded; it lands when the runtime makes it true

The sentence a reader most wants, *"a handler that C leaves by `longjmp`, `siglongjmp` or `_longjmp` onto the thread's
own stack is over and its callbacks run"*, is **false on Darwin today** (`fp3`, `n6`) and on route A, and unread on the
CI's Linux x86-64 leg (the calibration of panel 255's R1 has never been run there). **Recorded as the rival**: `spec-x1.txt`
(+142 vendored), the blind reading's `rival` arm. **It lands in a later amendment when** the runtime lane's Darwin
repair of the `fp3` class lands (a defect drafted apart, R4), and `ubuntu-26.04` has read the accessor. *What conservative
would have been*: **no sentence**, the runtime's words alone as ruling B's were (0 tokens; the author asked for the words).

### R3. What the landing owes

`HERO_SIGNAL_REMEDY` (`sigguard.c:110-113`) and the 22 `run` goldens that end with it, and the compiler's own test of
defect 2533 (red at the frozen tree, `c0476dff3`): **owed whatever text wins** (the sentence goes before the remedy sentence, so
it adds nothing to that test's red). **The four pins** of the `spec` suite and the ledger row. **A check that holds the
sentence's nouns to the words** (`grep -rn -F 'seen to end'` finds `sigguard.c:443, 449` and 17 goldens and no test of
the spec's side; the warden's draft is about 20 lines of Heroes, unrun): **recommended**, so the sentence and the runtime's
words cannot drift again (the lane's new SW words already differ from S's). **Goldens by platform**: `p4s` runs on
Linux and aborts on Darwin on every build, so the warden's registered *"goldens that show `p4s` running"* cannot be one
`run` golden (critic § 9, 3). The words the lane printed are re-checked against the sentence when 2702 lands.

### R4. Findings beside the sentence, filed apart

Drafts, unfiled, `.claude/worktrees/rebel-scratch/coord/issues/p257-defect-*.md` (the coordinator opens them): **the false *"compiler
bug"* abort** after libpng's own error path over a Heroes frame (`pngread` 27 of 27, `abandon`, `watchdog_h2`, all six builds, both
platforms); **`cs2`**, a correct jump between two stacks the program made refused on Linux by mapping order
(`siginstall.c:150-160`); **`n6` and `fp3`**, a correct `siglongjmp` refused on Darwin by where the signal was raised; **the
words**: S's causes false of `fp3`, `as_0`, `n6`, `sched`, U still printed for `rr_*` and `as_1`, K for `as_1` on Linux only.
**Not drafted, the coordinator's**: the landed tree's build in the Linux image (`hero_ui_print_gtk.c:609`).

### R5. The blind reading is registered and not run

`257-briefs/llm-ergonomist.md` and `blind/registration.md`: arms `now`, `adopted` (R1), `rival` (x1), by sha256; t1 seven
programs, t2 three designs; D1 to D3; **31 sessions, 0.60 USD a session, 1.00 the verdict, 12.00 USD the ceiling, an
expectation of 7.0 (5.2 to 11.3) from panel 256's measured run**; the area drawn empty, **no session, no key, no run**.
The coordinator's predictions are in the registration: **D3 fails for `adopted` and `rival` alike**, the sentence being
silent on blame, and that is recorded now as what it does not teach.

## What conservative would have been

**No sentence** (R2's note), or **w27 with R-1 and R-3**, the engineer's approval, which promises a run on the thread's
own stack the critic's `n6` falsifies on Darwin. Recorded so the author can choose either.

## The prior art, per resolution

None: the historian did not sit, and no precedent is claimed.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| spec-warden | the landed spec of its `w26` reads exactly 12,947 real; blind `now` p4 1 of 5 (0 to 2), `w26` p4 4 (3 to 5); at the landing `run fixedbugs-2522` reads 25 and 0 with goldens showing `p4s` running and `p4d` at 5,120 aborting (**the critic: not one golden on both platforms**) | the landing; the blind run |
| compiler-engineer | at the landing, `cs2` ORDER=1 reads S (exit 134) under any runtime whose `siginstall.c` is the frozen file | the landing |
| ffi-pragmatist | the landed runtime leaves the `sqlite3_progress_handler` binding of §4.19's SQLite fence without a shim | the landing |
| coordinator | `heroes measure --refresh` of the sentence applied to `rebel-round`'s spec at the landing reads 12,920 plus panel 256's delta, and the four pins are the only red of `spec`; the blind run: D1 and D2 hold, D3 fails for `adopted` and `rival` alike; 31 sessions, 7.0 USD (5.2 to 11.3) | the landing; the blind run |

## What this sitting files and owes

- **A decision issue**, `panel 257`, drafted unfiled at `.claude/worktrees/rebel-scratch/coord/issues/p257-decision.md`
  and checked by `heroes run tests/harness/issues.hero -- draft`: R1 to R5 for the author's reading.
- **Four defect drafts**, unfiled (R4), and the Linux build of the landed tree, which is the coordinator's.
- **Owed to the author**: the blind reading (R5), by their word and a figure; the sentence's real count at the landing.
- **Defect 2702** (R3 of panel 255) is where the context clauses become true on Linux; **the sentence does not promise
  its refusals as final** (the coordinator's instruction of 2026-11-23), and *"may"* stays true after it lands.

## How the sitting went, for the next

- **The first issue of the arms was falsified by the first reading before any seat ran**, on a placement the compiler's own test
  holds and on clauses the critic measured on both builds in 53 minutes (24 findings): the briefs' arms were rewritten, the
  registration repaired, and **the sitting's strongest result came from the critic reading the briefs first** (CL-079).
- **The seats held one platform each and the critic held both**: the two depth rules and `setcontext`'s two behaviours
  were found because the critic ran each seat's programs on the other platform (C10 measured, 70 rows, 14 to 21 differ).
- **A runtime landed while the sitting sat** (`523d35537`): the briefs named panel 255's routes as the build, and the
  seats' decisive rows were re-run against the landing after the fact (the resumed seats' `## Against the landed runtime`),
  not at the start: **a sitting that tests a sentence against a route a lane is landing owes the lane's tree as a second column
  from the first brief.**
- **Two API limits** stopped both builders mid-report (about 11:50 and again); each was relaunched **from its own report
  file**, not from the other's findings, and the resume brief (`257-briefs/resume.md`) is on disk.
- **Three times a seat wrote scratch files into `/tmp`** (the critic's first reading a log, the engineer six files, the critic's
  second reading one empty file), each removed in the same session and disclosed in its report: **the seat brief should name `TMPDIR`
  inside the copy**.
- **Counts**: five `--refresh` requests in all (the warden's four and the coordinator's one), each a token count; no
  `claude -p`, no paid run; every container removed (`docker ps -a | grep p257` empty, read by the engineer and the critic).
- **The sentence is shorter than the reports behind it**: a refusal-side sentence with *may* and four named exits, after a
  family of arms that each promised more than a running program allowed. Each promise was falsified by a program, never by
  an argument.

## Author's verdict

*Pending.*
