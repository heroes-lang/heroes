# Panel 257, the completeness critic, first reading: the briefs

Read 2026-11-23 from 10:12 by this Mac's clock (`date`), in the detached copy
`.claude/worktrees/p257-scratch/critic/` at `828567a30` (`git log -1 --format=%H` printed it, `git status --short`
printed nothing). Written as I go; a finding is numbered `B<n>`, its command and result beside it. A command that
ran in my copy says `[copy]`, one in the lane `[lane]`. Load at the start: `uptime` read load averages 12.37 7.40
7.57 (indicative only). I gave no verdict on any arm. **24 findings, B1 to B24**, the end of this file orders what the coordinator
must repair before any seat starts; no paid run, no credential printed, nothing outward, no file outside the repository root (my one stray log, a `/tmp` file of a first build
attempt, was removed before I read anything from it: `ls /tmp/critic*` prints no match).

## What I re-ran and found TRUE (the framing facts that hold)

Each by its command; `[lane]` is `.claude/worktrees/lane-p257`, `[copy]` my detached copy at `828567a30`.

- Tree: `git log -1 --format=%H` [copy] = `828567a30a4018e74605378c3572249b2bed6d85`; the lane's `git status --short`
  printed `?? docs/panel/257-briefs/` alone. `git merge-base --is-ancestor 828567a30 rebel-round` [lane] = yes;
  `git diff --stat 828567a30 rebel-round -- spec seed runtime/parts runtime/runtime.c runtime/heroes_runtime.h
  tests/golden/run .claude/agents .claude/skills | wc -l` = **0** (rebel-round's head now reads `20cfda06b`, not the
  `22710bc62` of the brief; the brief says it moved, and the 0 holds at the newer head).
- Spec: `wc -l -c spec/heroes-spec.md` = 516 lines, 33272 bytes; `shasum -a 256` begins `15804f3fb52a62c208bc59ef`;
  line 428 ends `call\n` (`sed -n 428p | tail -c 5 | od -c`); line 202 is the quoted abort sentence;
  `grep -n -i -E 'setjmp|longjmp|jmp_buf|getcontext|swapcontext|signal handler|handler|fingerprint|save point'
  spec/heroes-spec.md` prints lines 427 and 428 only; `grep -o -i 'this runtime\|the runtime' | uniq -c` = `1 the runtime`
  (line 383, about threads, not aborts).
- Budget: stage 2 built in my copy (`./heroes build selfhost/main.hero -o heroes-next`, 7:44 real at load ~20, indicative);
  `./heroes-next measure spec/heroes-spec.md` = claude-legacy 9585, cl100k_base 9761, maximum 9761, **real 12809**
  (claude-opus-5, 2026-11-22), headroom 3575, the FFI floor mortgaging 60 (12869): identical to § 3 of the shared brief.
- Arms: `patch --dry-run -p1 < arm-eN.diff` [copy], four times, each printed `patching file 'spec/heroes-spec.md'`;
  each applied copy differs from the original by exactly one line (`diff | grep -c '^[<>]'` = 2) and its line 428 ends with
  the arm's `.txt` byte for byte. Vendored maxima (`./heroes-next measure build/arms/<e>/spec/heroes-spec.md`):
  e1 **9859**, e1b **9851**, e2 **9818**, e3 **9800**: all four equal the brief's.
- Issues, read only (`gh issue view --repo heroes-lang/heroes --json ...`): 2701 CLOSED (2026-11-22T23:35:46Z, title *Panel
  255: Defect 2679: ...*), 2702 OPEN, `kind:defect`, `class:blocking`, `area:runtime`, title as quoted.
- Commit `691531908` (`git show -s`): *"Contract: the most robust solution always wins, a new form and the spec's size
  included (author, 2026-11-22)"*; `.claude/skills/panel/SKILL.md:26` holds *"anything the spec must state"*.
- The evidence archive: `tar -tJf docs/panel/255-reports/evidence.tar.xz | wc -l` = 4510; the three route files:
  `wc -l`/`md5 -q` = 671 `ba8487af...` (C2679 sigjump.c), 1438 `2730f161...` (C4r rebind.c), 557 `3c603aff...` (C4r sigguard.c):
  all three equal the table of § 4. The frozen mechanism: 283 / 523 / 362 / 727 lines (`wc -l runtime/parts/{sigjump,sigguard,siginstall,rebind}.c`).
  The ffi-pragmatist's report lines 197, 237, 575 read 1,393, 1,403, 1,438 (`sed -n`).
- Platforms: `clang --version` Apple 21.0.0, `sw_vers -productVersion` 27.2.1, `pkg-config --modversion libpng` 1.6.59 [Mac];
  the container `docker image ls heroes-linux-arm64` shows `latest 338a1044ac90`; inside it Debian clang 22.1.8, ldd 2.41,
  `aarch64`, `clang++ g++ python3` present, `pkg-config --modversion libpng` = *Package libpng was not found*.
  (Container named `p257-critic-probe`, removed: `docker rm -f` printed nothing, `docker ps -a` shows only another lane's.)
- Goldens: `ls tests/golden/run | grep '^fixedbugs-2522' | grep -c '\.hero$'` = **25**; of them 12 are `a-callback-after-*`
  (4 longjmp, 4 siglongjmp, 4 underscore-longjmp: read from the listing). `.github/workflows/ci.yml` names `macos-26`,
  `ubuntu-26.04`, `ubuntu-26.04-arm` (lines 368 to 373).
- Author's verdict: `docs/panel/255-*.md` line 294 `## Author's verdict` holds *1a and 2a* and the text the brief quotes,
  **inside a "meant as:" gloss written by the coordinator, not in the author's own words** (finding B16).
- The seven blind programs, built by MY OWN seed compiler (`./heroes build pN.hero -o pN.bin`, Mac, plain) and run three
  times each under `timeout 20`, output to a file inside my copy (never `timeout heroes run`): p1 exit 0 `answer ran for 7`
  / `result 70` (3 of 3); p2 the same; p5 the same; p7 the same (exit 0, 3 of 3); p3 exit 134, words *"was called after C,
  inside a signal handler, jumped to a buffer this runtime holds no record of..."* (3 of 3); p4 exit 134, the S words
  (3 of 3); p6 exit 134, the S words (3 of 3). **All seven match `answers.md`'s "today, Mac plain" column.** The cost: the
  seed compiler, not stage 2, built them; the runtime is the same file, so the C is the same.

- No paid run is under way and none was started by the briefs: `ls .claude/worktrees/p257-scratch` = `compiler-engineer coord critic ffi-pragmatist spec-warden` (no `257-llm-ergonomist` area, no `run.log`), the area's path does not exist (`ls -d` said so, B18),
  and `grep -rn -i -E 'sk-ant|api[_-]?key *=|bearer' docs/panel/257-briefs` prints nothing; the briefs name no path outside the repository root (`grep -rn -E '/tmp/|/private/tmp|~/' docs/panel/257-briefs` prints nothing).
- The 12 `a-callback-after-*` goldens are `.expected`-pinned as running (`grep -q '^!panic'` over the 12 `fixedbugs-2522-a-callback-after-*.expected`: none has such a line), and the `ffi-pragmatist-r2.md` lines 283 and 284 read `fp3` and `as_0` *"134 S: a cost"* (`sed -n`), as C8 says; `critic-r2-reports.md:91` has the `as_0` row C8 names.

## Findings

**B1. (blocks the placement of every arm) The end of line 428 is not free: the compiler's own test holds it, byte for
byte, to a C constant the runtime prints at the end of every signal refusal, and no brief says so.** All four arms
append to line 428 (`@@ -427,3 +427,3 @@`, the new text after `... since its last call`). But
`selfhost/emit/callback_thunk.hero:354-375` (the test *"every signal refusal ends with spec section 13's own sentence,
byte for byte (defect 2533)"*) reads the spec **from where `No function of the program is a signal handler` opens to
the end of its line** (`bytes.locate(text: spec, needle: "\n", from: opens)`) and asserts it `==` the string literals of
`#define HERO_SIGNAL_REMEDY` (`runtime/parts/sigguard.c:110-113`). That constant is the last words of **every** signal
refusal: `grep -rn HERO_SIGNAL_REMEDY runtime` [copy] = `sigguard.c:453`, `alloc.c:620`, `lend.c:194` (the K, D, allocator
and lend-tracking refusals as well as S and U), and 22 `run` goldens pin it (`grep -l 'No function of the program is a
signal handler' tests/golden/run/*.expected | wc -l` = 22 of 924 `.expected`). So an arm applied as drafted either turns
that test red, or, kept green, **puts the new sentence at the end of every refusal the runtime prints, including ones that
have nothing to do with a jump** (and in the S refusal itself, a message that says *"... aborts saying the handler was not
seen to end"* to a program that was just told so), costing `sigguard.c` and 22 goldens (`grep -l "a waiting call of that layer answer stopped" tests/golden/run/*.expected | wc -l` = 22; no file of `tests/emission` carries the sentence, `grep -l ... tests/emission/* | wc -l` = 0). The
shared brief § 3 says only *"Every one is followed by the spec's own rule"*; the warden's item 5 sends the warden to
`suite_spec.hero` and the goldens' `.expected`, not to this test. **The result of running it is in B2.**
**A route nobody listed, which this coupling makes the cheapest:** put the sentence **before** `No function of the program
is a signal handler`, between `-> i32`. and that sentence (line 428's first full stop is already there), so the line's tail
and the paragraph's no-full-stop end stay as they are, the REMEDY stays equal to the spec's tail, and the one token the
arms add (`. `) is not needed at all. (See also B14, the full stop.)

**B2. (a fact the sitting must know) That same test is RED at the frozen tree, before any arm: the runtime prints a stale
copy of line 428's tail, so the words a refusal ends with are not the spec's today.** Command [copy], stage 2 built from the
frozen tree, the spec unmodified (`shasum -a 256` began `15804f3f`):
`./heroes-next test selfhost/main.hero --only "byte for byte (defect 2533)"` -> `FAIL ... 1 test, 1 failed`, exit 1
(7:33 real at load ~20, indicative). Its own diagnostic: left (the runtime's `HERO_SIGNAL_REMEDY`) reads `... answer 1 and a
waiting call of that layer answer stopped; ...`; right (the spec's line 428) reads `... answer 1 and a waiting call of that
layer, or a mailbox's `receive` that finds it empty, answer stopped; ...`. Cause (`git log`): `c0476dff3` (2026-11-22,
*"panel 254's text for threads and mailboxes"*) put the mailbox clause into line 428 and is an ancestor of the frozen commit
(`git merge-base --is-ancestor`); `git show 828567a30:runtime/parts/sigguard.c | grep -c mailbox` = 0 and the same at
`rebel-round`'s head. The printed refusals show it: my p4 run of the seven programs ends `... a waiting call of that layer
answer stopped; ...` with no mailbox clause, and so does every `.expected` of the 22. `gh issue list --search HERO_SIGNAL_REMEDY`
finds only #2533 (CLOSED, the defect this test was written for); no open issue names the new drift (searched: the constant's
name, and *signal refusal sentence spec mailbox*). **What this changes for the sitting:** (i) the shared brief's *"Every one is
followed by the spec's own rule"* is false today at one clause; (ii) the sentence the warden writes lands in a spec whose
tail the runtime does not match, so the runtime lane's landing owes the REMEDY update and the 22 goldens whatever arm wins,
**and it must say so** (the brief says the lane *"will tell the final words"*, not that it owes this); (iii) a seat that runs
the compiler's own tests in its copy will see this red and must not read it as its arm's.

**B1, measured.** [copy] `cp` the arm's spec over `spec/heroes-spec.md`, the same command as B2, spec restored after each
(`shasum -a 256` began `15804f3f` again each time): **arm e3 as drafted (appended): the test's right-hand side now ends
`... since its last call. Where C leaves a handler other than by returning, `longjmp`, `siglongjmp` or `_longjmp`, the next
callback aborts, saying the handler was not seen to end`** (2:07 real), i.e. the arm is now part of "spec section 13's own
sentence" the runtime must print. **The same words placed before `No function of the program ...`** (a file built by a
five-line python replace in `build/arms/e3before/`, vendored maximum **9800**, the same as e3 appended): the right-hand side
ends `... since its last call` and `right.replace(mailbox clause, "") == left` is `True`: **the arm adds nothing to that
test's red** (2:41 real). Both placements cost the same vendored tokens (9800 against 9800).

**B3. (the blind registration) The runner is not "panel 256's, unchanged" with "`make_sessions.py` the one file changed": at least five of its pinned
files carry panel 256's constants, and the brief's own caps contradict `run.sh`.** `llm-ergonomist.md` says the runner is panel 256's
*"unchanged"*, *"pinned by hash"*, and that *"`make_sessions.py` is the one file changed"*. Read from `rebel-round`
(`git show rebel-round:docs/panel/256-briefs/blind/<file>`):
- `run.sh` lines 43-50: `CAP=1.00`, `CAP_VERDICT=1.50`, `CEILING=19.50`, `WARDEN=/.../lane-p256/docs/panel/256-reports/spec-warden.md`,
  `REGISTRATION_SHA=fe6e3cb3...`; the brief's run needs **0.60 / 1.00 / 12.00**, the lane-p257 path and a new hash (14 lines of
  `run.sh` name 46, 45, 256 or the caps: `grep -n -E '\b46\b|\b45\b|p256|256|heroes-spec|19\.50|1\.50|1\.00|lane-p256'`);
- `check_folders.py` line 12: `if len(order) != 46 or len(set(order)) != 46` and it compares `sorted(os.listdir(d))` with the key's flat names and `open(..., 'rb')`s each, so a session folder holding a `programs/`
  **subfolder** (the 257 sessions hold `programs/prelude.h`, `programs/pN.h`) either lists `programs` where the key holds `programs/p1.h` (*"holds other files"*) or, keyed as `programs`, is opened as a file and raises `IsADirectoryError`;
- `brief-session.md` names the two inputs *`heroes-spec.md`* and *`task.md`* and says *"Read no other file"*, while both 257
  tasks say *"the specification of the language Heroes (`spec.md`)"* and name `programs/prelude.h`, `programs/pN.h`;
  its report structure is `answer / choice_points / prediction / context` with *"prediction: every line the program writes"*,
  where t1 asks `verdict / the one sentence / confidence`;
- `make_sessions.py` copies exactly three flat files per folder (`assert sorted(os.listdir(d)) == sorted(files)`) and draws labels
  from `pool = list('bdfhjkpqsvxyz')` *"no letter of `now`, `gc`, ..."*: `d`, `p`, `v` are letters of **`adopted`** and **`rival`**;
- the **verdict session** of 257 *"reads all 30 reports"* (`llm-ergonomist.md`), so its folder cannot be made by `make_sessions.py`
  (the reports do not exist yet) nor held to registered hashes by `check_folders.py`; panel 256's verdict folder held `spec-A`,
  `spec-B`, `changes.diff` and a task, never reports.
The registration then pins files that are no longer 256's, and *"pinned by hash"* would hash the edited ones; the honest sentence is
*"panel 256's runner **adapted**, files changed: run.sh, check_folders.py, brief-session.md, make_sessions.py, preflight.sh*" (preflight.sh
embeds run.sh's text, 15 lines of the same constants), with `diff` against 256's committed bytes in the registration.

**B4. (the cap) The brief's spend arithmetic is the cost, the runner's rule is the cap, and at the top of the brief's own range the verdict does not run.**
`run.sh` stops *before* a session when `spent + cap + MARGIN <= CEILING` fails (`python3 -I -c "... $spent + $cap + $MARGIN <= $CEILING"`).
With the brief's 0.60 / 1.00 / 0.10 / 12.00: the verdict (cap 1.00) runs only if the 30 graded sessions spent **<= 12.00 - 1.00 - 0.10 = 10.90
USD, a mean of 0.363**; the brief's top of range is 30 x 0.377992 = **11.34** (`python3 -c "print(30*0.377992)"`), which stops the
verdict, while the brief says *"the ceiling above the 30 at that price (11.3 plus 0.3)"*. 256's measured mean of its graded sessions is
(10.775607 - 0.294765) / 45 = **0.233** (`llm-ergonomist-blind.md` lines 31-36 of `rebel-round`: the numbers 10.775607, 0.294765,
0.171964, 0.377992 and the author's 20 USD all read true), so the mean-case total is 30 x 0.233 = 7.0 and the verdict runs; the risk
is real only in the upper tail, and the brief should say *"the verdict needs a mean at most 0.363"* or set the verdict's cap aside
(its own 1.10, not inside the 12.00). (The brief's *11.7* upper figure is 11.34 + 0.295 = 11.63, rounded up.)

**B5. (a leak into the arm the blind reading is meant to separate) `blind/programs/p4.h` carries the golden's project-internal header, and it tells a `now` reader
what the runtime does.** `grep -n -i -E 'defect|panel|ruling|runtime|row [0-9]|absence|no event|2522|2026-' blind/programs/*.h`: p4.h lines 1-15 and 47
and 109, among them *"Defect 2522's row 2 (the author's ruling B of 2026-11-20): the program's own C for its case. A correct program: its
handler jumps OUT of itself by a jump no function of the C library makes, **so this runtime sees no event for it**, and only then, the
signal over, it calls the program's function from a frame far deeper than the handler's was"*, *"Linux arm64 at panel 251"*, `absence.hero`, and
a golden's file name, *`fixedbugs-2522-a-jump-out-of-a-handler-through-a-copied-buffer.h`* (the **p3** shape, named inside p4). The task
tells the session *"no other file of this folder"* and the area holds *"only its registered files"*, but **p4.h is a registered file**, copied whole from
`tests/golden/run`. p4 is one of the three programs D1 rests on (*p3, p4, p7*, `now` at most 2 of 5 on at least two of the three), and p3 may not be
one of the two (**an inference, unrun**: that a `now` reader's default for a `siglongjmp` through a copy is RUNS, which the key calls right; the blind reading is the only instrument that would say), in which case **D1 hangs on p4 and p7, and
p4's header answers p4 for the `now` arm** (also an inference about readers; what is measured is that the header is in the file, byte for byte the golden's: `diff tests/golden/run/fixedbugs-2522-an-inline-jump-out-of-a-handler.h blind/programs/p4.h` shows only the six lines of `run_case` added). A `now` that scores well on p4 reads as *the sentence did not move anyone* (D1 fails, *"the sitting is
reopened on the text"*) for a reason that is the header's, not the sentence's. Repair: p4.h with its comment replaced by one neutral line, hashed again
in `answers.md`; and every program's header read for the same (p1, p2, p3, p5, p6, p7 pass the `grep`; `prelude.h`'s *"from a frame 64 KiB deeper
... no signal running"* says depth matters and is shared by all arms, a mild cue, not an arm leak).

**B6. (the t2 words are not the words the runtime prints) `task-t2.md` says *"here are the words it prints (they stand for every abort of this kind)"* and quotes 337
characters of an 809-character message.** [copy] python replica on my p4 run (`build/blind/o4.txt`): the quote's two sentences are in the real text, but the real
text also holds the parenthetical *"(an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library)"* (absent from the quote) and the final sentence
*"No function of the program is a signal handler: ..."* (absent from the quote), neither marked by an ellipsis. The parenthetical is exactly the cue the arms' cause
lists repeat, so removing it may be deliberate, but nothing says so, and D3 (*"neither"* against *"the library's C is wrong"*) is then a reading of a message
that no build prints, whose last sentence (*"No function of the program is a signal handler"*, an instruction to the program) is the one most likely to
decide whose mistake it is. Either quote the message whole (and accept the cue, equal in all three arms), or say in the task *"the message is shortened"* and
register why.

**B7. (the t2 key was never run) `answers.md` grades t2 *A RUNS, B ABORTS, C RUNS* and no program of the three designs exists.** `ls blind/programs`:
`p1..p7` (`.h`, `.hero`) and `prelude.h`; none holds `run_with_deadline`, a timer (`setitimer`, `alarm`), or an asynchronous signal: p1, p5, p7
deliver by `raise()`. The t2 story is a **timer signal interrupting a long computation**, which differs from `raise()` in where the handler runs and on
what the thread was doing; today p7 (the B shape) RUNS on this Mac (my run: exit 0), so *B ABORTS* is the ratified rule's prediction (R4) and not a
measurement on either platform. The ffi-pragmatist's item 3(c) builds the three real handlers; the key must be marked *unrun* until it has, and `answers.md`
should say that t2 is graded by the landed build's run of those three programs (kept in `programs/`), as t1's is.
**B8. (a control that cannot be answered by the spec) p6 can never print `result 70`, so t1's own definition of RUNS cannot be met by it.** `task-t1.md`: *"`RUNS` if it
prints `answer ran for 7` then `result 70`, or `ABORTS` if the program ends before that"*. `blind/programs/p6.h`: the handler calls `f(7)` and
`run_case` ends `return 1;` (p1 to p5 and p7 return `call(cb)`, 70; p4's `inline_deep` returns `cb(7) + room[0] - room[...]` = 70). A p6 that is **not** refused
prints `answer ran for 7` then `result 1`: neither verdict. So a careful reader answers p6 from the C (the verdict is *not RUNS*), without the spec, and
p6's per-arm count measures reading, not the sentence, in both arms. Repair: `return call(cb)`-shaped ending, or `return f(7)`-shaped so a non-refused run
prints `result 70`; the answer key then holds.

**B9. (a shape the arms and the table never name) a longjmp that lands inside the handler is a longjmp, and the only goldens of it are refused.**
`ls tests/golden/run | grep 'jumps-within-itself'` = two (`...by-longjmp-then-calls-deep-is-refused`, `...by-siglongjmp-...`): the handler saves with
`_setjmp`/`sigsetjmp`, returns to itself by a helper's `longjmp`, then reaches the program's function; refused in the S words (`.expected` first line:
*"answer was called while a signal handler of C's that this runtime saw begin had not been seen to end ..."*). E1's *"C may leave a handler by `longjmp`,
`siglongjmp` or `_longjmp` and the program's callbacks run on"* and E2's *"C left it by `longjmp` ... to a buffer a save of that family filled"* are both
true of this program only through the verb *leave*, which the reader cannot test (whether a jump leaves depends on where its buffer was filled).
None of C1 to C10 and none of p1 to p7 is this shape as such; C6's *refill* golden is the **same class** (its header: *"This jump lands within the handler, so its entry stands and the function is refused"*, `sed -n 1,16p` of
`tests/golden/run/fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused.hero`), and B22 measures that *refilled* is not what the runtime acts on. A reader of E1 or E2 is not shown that *"a longjmp"* is not enough. My **p8** (below; the handler saves a buffer of its own, `siglongjmp`s to it, then calls the program's function, a neutral-comment version of the golden's shape, ending `return r;` so a non-refused run
prints `result 70`): **ABORTS in the S words on all four cells, 3 of 3 each** (Mac today, Darwin C4r, Linux today, Linux C2679, same method as the key table), so it is a graded program with a stable key. The seats should ask the clause
*"to a buffer saved outside the handler"* (or *"a jump that lands within the handler"*) against p8 and p10.
```
/* p8.h: the handler saves a buffer of its own, jumps back to it by siglongjmp (a jump that lands inside the handler), then calls the program's function itself */
#include "prelude.h"
static cb_t f;
static int64_t r;
static sigjmp_buf hbuf;
static void handler(int sig) {
    (void)sig;
    if (sigsetjmp(hbuf, 1) == 0) siglongjmp(hbuf, 1);
    r = f(7);
}
int64_t run_case(cb_t cb) {
    f = cb;
    if (on(SIGUSR1, handler) != 0) return -2;
    raise(SIGUSR1);
    return r;
}
```

**B10. (a pass criterion the adopted builds cannot meet on the frozen goldens) *"passed `... run fixedbugs-2522`"* reads 23 passed, 2 failed on both adopted builds, by panel 255's own gates.**
Both seat briefs: *"say first whether the adopted ... build built and passed `./heroes run tests/harness/main.hero -- ./heroes-next run fixedbugs-2522`
in the form panel 255 ran it"*, and *"A route that does not build is not adopted"*. Panel 255's readings of that very run: `compiler-engineer-r2.md:382`
`fixedbugs-2522 | 25 / 0 | **23 / 2** | 25 / 0 | 23 / 2` (columns today, C4, C4 forced, C 2679 alone: the adopted Linux build is the last), `ffi-pragmatist-r2.md:521`
(*"23 passed, 2 failed: the two goldens the brief names"*) and `:686-687` (C4r's own gate: *"2522 23 passed, 2 failed (the same two goldens: the refill case in S
words, the copied buffer running)"*; **C4r's gate was unrun until that late section**, `:584`). The two failures are R2's two `.expected`, untouched in the frozen tree. A seat
that reads *passed* as 25/0 reports a failure of the adopted route; one that reads 23/2 as a pass needs the brief to say so. Repair: the brief names
*"23 passed, 2 failed, exactly `...a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` and `...a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so`"* as the pass.

**B11. (small, checkable) Three counts and a path in § 3 are wrong.** (a) *"over the extracted evidence prints only three copies of the runtime's own `sigjump.c`"*:
`grep -rl -E 'throw|__cxa|_Unwind' build/evidence | wc -l` [copy, after `tar -xJf`] = **17**, all named `sigjump.c`, **8 distinct** by `md5` (the match is the comment at
line 65, *"a C++ exception or `_Unwind_*` out of a handler (no jump is called)"*, in the C2679 copy); the claim's substance (no case throws) holds, the number does not.
(b) C7's `g2_1`: not under the cited `ffi-pragmatist/p255/r2/sh/` (`find` there finds `g1_0 g1_1 g2 g3`, no `g2_1`); it is at `critic/p255/r3/fsh/g2_1.hero` (and `w-*-f/`).
(c) *"`clang++` and `g++` are on this Mac"* and *"are in the container"* (`compiler-engineer.md` item 3 too): **`g++` is clang on both**: `g++ --version` [Mac] =
Apple clang 21.0.0, `readlink -f $(which g++)` [container] = `/usr/lib/llvm-22/bin/clang` (same as `clang++`), and `gcc` is *not found* in the container. There is no GCC on
either platform, so the C5 program has one front end, and *"clang refuses `__builtin_longjmp` on arm64"* is the only compiler this sitting can run: I confirmed it on both
(`clang -c bl.c` -> *error: `__builtin_setjmp` is not supported for the current target*, Mac arm64 and container aarch64; g++ in the container the same). Whether GCC accepts
it on arm64 is unrun (recalled, not measured, that it does).

**B12. (the shared brief's platform scope) Windows is a third leg of the CI and a platform of the contract, § 4 does not mention it, and on it the runtime enforces none of what the arms say aborts.** `.github/workflows/ci.yml:373`
holds `Windows x86-64` (`windows-latest`); `.claude/rules/platforms.md` opens *"# The three platforms"*. Read in the frozen source (not run: there is no Windows machine in this sitting): `runtime/parts/sigguard.c:506-521`, the
`_WIN32` branch (the `#else` at line 506 of the `#if !defined(_WIN32)` opened at line 182), reads *"Windows: no probe and no walk (panel 247's R10) ... whether UCRT's `raise` leaves a delivery's address to learn is the box's question, unrun"*
and `hero_callback_entered` is an empty function (`(void)what; (void)ret; (void)frame;`); `sigjump.c:69` is `#if !defined(_WIN32) && !defined(__APPLE__)`. So on that leg a Heroes function a C handler calls, or a callback after a handler left by an inline jump,
**is not refused at all** (apart from the threads Windows makes and a fault's guard, per the same comment), which makes *"the next callback aborts saying the handler was not seen to end"*, and line 427's own *"never ... called by one"*, false there, in a document that is not platform-scoped. The brief
tests *"true on both platforms"* and lists as unrun *Linux x86-64* and *macos-26*; it never says what the sentence claims on Windows. Not for a seat to build; for the text: scope the sentence (*where the platform has signals* is
Linux and Darwin) or say in the synthesis that the clause is knowingly Windows-false and file it, and price the words that say so.

**B13. (R1's fallback makes *"whatever memory the buffer is in"* platform-conditional, and the adopted Linux build was never run on the CI's C library) The decode that makes it true is verified on three CPU/C-library pairs, one of them not the CI's.** Panel 255 R1:
*"Where it does not verify, route A (a fingerprint read only of a buffer on the thread's stack ...): glibc's four other architectures, a future C library, Darwin x86-64"*, and
`## What conservative would have been` lists what route A refuses (*"a correct jump out of a handler through a heap, thread-local or late-loaded buffer"*). E1's
*"whatever memory the buffer is in"* (E1b's the same words) is false on a route-A build. This sitting's two platforms are the two where the decode verified in panel 255
(Darwin arm64 `route 3`; Debian glibc 2.41 aarch64), and the CI's Linux legs run Ubuntu's glibc **2.43**. The runs of a *runtime* on Ubuntu 2.43 in the archive are of **C4**, which
panel 255 did *not* adopt on Linux, and the x86-64 runs are C4's and round one's route C (`find build/evidence -maxdepth 5 -type d \( -iname '*ub*' -o -iname '*x86*' \)` = `p255/out/x86-T`, `p255/out/x86-C`, `p255/r2/out/x86-C4`, `p255/r2/out/ub-C4`, `p255/r2/out-first/x86-C4`; `grep -rl 'GLIBC 2.43' build/evidence` = `probe/ubuntu2604-arm64.log`, `probe/ubuntu2604-x86.log` (round one's key-layout probes; `compiler-engineer-r2.md:502-503` says of Ubuntu x86-64 *"by the probe, not this calibration"*) and `r2/out/ub-C4/ubuntu.log`): `ls build/evidence/compiler-engineer/p255/r2/out` [copy] = `C2679 C4 C4fail png png2 today ub-C4 x86-C4`, **no `ub-C2679`, no `x86-C2679`**;
`compiler-engineer-r2.md:494-500` (*"C4's stage 2 built in the Debian image ... run in `ubuntu:26.04` ... `decoding: 3`"*), `:544`, and `ubuntu-bins.sh`'s default `VARIANT:-C4`.
The engineer's own prediction is falsified *"by the accessor printing `2` or `0` (the key or the field not verified against the runner's glibc 2.43 ...)"* (`compiler-engineer-r2.md:582`). The
brief says *"true on both platforms"* for a Linux build whose CI-library calibration is unmeasured, and names the container's glibc (2.41) without saying 2.43 differs.
Ask the engineer to run `C2679` through `ubuntu-bins.sh` with `VARIANT=C2679` and `ubuntu-run.sh` (arm64 only; x86-64 stays unrun), and to put the accessor's reading (`decoding: N`)
beside every row of the table.


**B14. (the full stop, for the warden's item 3) The paragraph of lines 427-428 is the only prose paragraph of the spec that ends without a full stop, and the arms' leading `. ` is not a neutral token.**
[lane] python over `spec/heroes-spec.md` (a paragraph = a block between blank lines outside fences, not opening with `#`, `|`, four spaces, `- ` or `* `): 14 such
paragraphs, **13 end in `.`, 1 does not: the one ending at line 428** (`... gives the count since its last call`). The omission is not an accident of style:
the runtime prints that line's tail as the last words of its message and the test of B1 holds the two equal, so a full stop added to the spec's line would have to be added
to `HERO_SIGNAL_REMEDY` and to 22 `.expected` files. Every arm's first token (`. `) is the stop that turns the tail into a non-tail; it belongs to this change **only**
in the appended placement, and **not at all** in the placement before the remedy sentence (B1), where the paragraph keeps its present end.

**B15. (a shape no arm, row or program names, and a split today) A jump to the first of many saved buffers is refused on Linux today and runs on Darwin today: U's third cause.**
The U words (today's, `sigguard.c:448`) end *"(copied with `memcpy`, refilled where the runtime does not look, or saved before more save points than it keeps)"*; the record is
`#define HERO_SIG_JUMPS 32` (`sigguard.c:317`, *"a full record overwrites its oldest pair"*, Linux and POSIX-but-Darwin). Not one of C1 to C10 or p1 to p7 saves more than a few
buffers. My p9 (below; `sed 's/p1\.h/p9.h/' p1.hero`): a recursion of depth 40 with a `sigsetjmp` in each frame, a handler that `siglongjmp`s to the **first**, then `call(cb)`.
Built by my seed compiler, `-O0`, three runs each: **Mac: exit 0, `answer ran for 7` / `result 70` (3 of 3); Linux arm64 container (Debian clang 22.1.8, glibc 2.41, the seed built inside it
from the frozen `seed/` and `runtime/`): exit 134, the U words (3 of 3)**. A correct program on both (every buffer lives in a live frame; the jump is a plain `siglongjmp`), refused with
the words *"holds no record of"* on one platform only: C10 fails today on it, and R2 promises the U words go (*"the true 'had not been seen to end'"*). Under R1's decode neither platform
should refuse it, and **both adopted builds run it** (key table below, 3 of 3 each); E1's and E1b's *"whatever memory the buffer is in"* does not say *however many*. Both seats should run it on the adopted builds (the engineer: the container; the
ffi-pragmatist: this Mac), plain and `--sanitize`.
```
/* p9.h: a handler leaves by siglongjmp to the FIRST of 40 buffers saved in 40 live frames */
#include "prelude.h"
#define N 40
static sigjmp_buf envs[N];
static volatile int fired;
static void handler(int sig) { (void)sig; siglongjmp(envs[0], 1); }
__attribute__((noinline)) static int rec(int n) {
    if (sigsetjmp(envs[n], 1) == 0) {
        if (n + 1 < N) return rec(n + 1);
        if (!fired) { fired = 1; raise(SIGUSR1); }
        return -4;
    }
    return 0;
}
int64_t run_case(cb_t cb) {
    if (on(SIGUSR1, handler) != 0) return -2;
    if (rec(0) != 0 && !fired) return -3;
    return call(cb);
}
```

**B16. (the author's words) The brief's "the author's" sentence is the coordinator's gloss, and the lane the author named is not the lane the brief ran.** `docs/panel/255-*.md:294-` (*Author's
verdict*): *"the answers *1a* and *2a* to the coordinator's recommendations, **meant as:** *ratify R1 to R5 as written; and the words of the refusals R4 names are owed to the spec, a sentence
written in a small sitting with the spec-warden*"*. `00-shared.md` § 1 puts that clause in quotation marks as *"The question (the author's, 2026-11-23 ...)"*; it is the recording
session's rendering of two letters. And *"a small sitting with the spec-warden"* became, in the same brief, **"the full panel"** (`SKILL.md` § Two lanes: *"full five judges"*) seated as
**three judges and a critic**, the historian and the llm-ergonomist unseated (the brief says what each gives up, which is the skill's duty). Not wrong, but the synthesis should
say which of the author's two words it follows, and `records.md` § *And the record says whose idea it was* reads: the question's wording is the author's *choice of 1a and 2a*, the text of
the sentence the sitting's.

**B17. (R4's list is the sentence's subject, and no arm names its programs) *"the refusals R4 names"* are mostly contexts and the arms are written in the vocabulary of jumps.**
`docs/panel/255-*.md` R4 (lines 217-228): *"On Darwin, C4r refuses `g1_0`, `g1_1`, `g2`, `g2_1` (G, a handler swapped away from and never resumed), `fp3` and `as_0` (and `cs_1`...); on Linux,
C 2679 alone refuses `g_3_0`; R3's Linux build would refuse every G form and `ub_*` (a callback above a handler left by an inline jump ...), and, where contexts fall to A, `b_*`"*. E1 names *"a switch
of context never switched back"*, E1b drops it, E2 and E3 say *"any other exit"* and never name a context; **no arm says what `fp3` and `as_0` are** (a jump *into* a coroutine, C8), and p7 is the only
context program of the blind reading. A reader of E2 or E3 who meets p7 (a `swapcontext` away, never back) must decide that a swap is *"an exit"*; t1's p7 then measures that inference, not the
sentence. (C8 and the ffi-pragmatist's item 7 ask the question; the arms are not yet written for it.)

**B18. (a registered check that cannot pass) *"It holds no `CLAUDE.md` in or above it; the sitting's coordinator checks that by `find`"* (`llm-ergonomist.md`, the area).** The area is
`.claude/worktrees/p257-scratch/257-llm-ergonomist/`, inside the repository: walking its ancestors with `dirname` (`for f in CLAUDE.md AGENTS.md MEMORY.md; do [ -e "$d/$f" ] && echo FOUND ...`) prints
**`FOUND /Users/joseph/Temp/heroes/heroes-lang/CLAUDE.md`** and **`FOUND /Users/joseph/Temp/heroes/heroes-lang/AGENTS.md`** (the area itself did not exist yet, `ls -d` said so). What kept the contract out of 256's
sessions was not the folder: `--bare` (run.sh line 110) and the area being the top of its own git repository, checked by the preflight's transcript and `scan.py` (`claude-md-or-memory=no`, 47 of 47 in
`llm-ergonomist-blind.md`). The registration should name those and drop the `find`, which would either fail forever or be ignored.

**B19. (provenance, low) The exception to *no paid run* is licensed by the author's rule of 2026-09-09, which the brief does not cite.** `00-shared.md` § 2 grants the warden up to four `--refresh` counts, *"which generates nothing"*, beside
*"No paid run"*. `.claude/hooks/guard_bash.py:1006-1013` (read): *"The Anthropic key counts tokens and does nothing else (author instruction 2026-09-09 ...) ... Counting is a read. Inference is spending somebody's money"*, and the
endpoint is a constant, `selfhost/cli/refresh.hero:37` (verified), `curl --version` here 8.7.1 against the 8.3.0 the file needs. `--refresh` leaves `build/measure-refresh/*-body.json` and `*-answer.json` in the warden's
copy (the file's own comment; the key is read by curl from the environment, in neither). So the exception is inside the rules; the brief should cite that rule instead of resting on *"generates nothing"*, which no command in it shows,
and I did not look up whether a count request is free of charge (no WebFetch: outward).

**B20. (the warden's predictions are due before the synthesis names the text they predict for)** `spec-warden.md`: *"Say which arm is the adopted one by the rule `llm-ergonomist.md` states (the text the synthesis names; you
propose it)"*; `llm-ergonomist.md`: `adopted` is *"the resolution's sentence"*, `rival` *"the second-ranked text the synthesis names"*, and the runner (`run.sh` of 256, `HEADING` and the `WARDEN` gate, lines 52-82) copies the warden's report as it stands
when it starts, so a prediction can only be for the warden's own proposed text, and the arms the run uses are fixed later by the coordinator. There is no *rule* in `llm-ergonomist.md` to apply, only a deferral. Ask for
predictions **per candidate** (e1, e1b, e2, e3 and the warden's own), so that whichever the synthesis hands to `adopted` and `rival` has its row, and for the warden to say which it expects to be adopted.

**B21. (the model script bounds stdout, not stderr) `.claude/rules/verification.md` § *A run that may not end is bounded in time and in bytes* asks for both, and the script the briefs name as the model does one and a half.**
`ffi-pragmatist/p255/r2/run.sh` (the *"model"* of `00-shared.md` § 4), lines 27-31: `timeout 20 "./bin-$n-$mode" 2> e.txt < /dev/null | head -c 100000 > o.txt`: **stdout** goes through `head -c`, **stderr** goes
to `e.txt` unbounded for up to 20 s. The refusals are on stderr and one of the rules' own incident (146 GB, `verification.md`) was an unbounded stream; a handler that loops and prints (a `fixedbugs`-style shape on a seat's own
programs) is bounded by 20 s and not by bytes. The brief says *"its output through `head -c`"*; say *both streams* (`2>&1 | head -c 100000 > o.txt`, the exit read from `PIPESTATUS`) and keep `e.txt`'s kind-grep on the merged file.

**B22. (measured: E1's and E1b's clause *"a jump to a buffer the handler refilled"* is false of the program it names, on both adopted builds) The refused refill is the one that lands INSIDE the handler; a handler that refills a
buffer and leaves runs.** The brief's § 3 asks the question (*"Whether any correct program that really LEFT the handler is refused because its buffer was refilled ... nobody has measured"*, C6). My p10 (below) is the
shape: the **handler** `memcpy`s a save the **main line** made into a second buffer and `siglongjmp`s to the copy, so the handler is left and the callback is called from the main line's frame. Same method as the key table below
(seed compiler, `-O0`, plain, 3 runs per cell; Mac and container; adopted runtimes by `HEROES_RUNTIME`): **Mac today ABORTS (U words, 3 of 3); Linux today ABORTS (U, 3 of 3); Darwin C4r RUNS (exit 0, `result 70`, 3 of 3);
Linux C2679 RUNS (3 of 3).** The golden the arms' clause comes from (`...a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused`, header *"the handler copies **its own** save into a buffer ... and jumps to that one: the jump
lands inside the handler"*, `00-shared.md` § 3) and the two `jumps-within-itself` goldens (B9) are **one class, a jump that lands within the handler**, which is what the sentence should name (and which makes a *longjmp*
that does not leave a handler refused in the same words); *"refilled"* is the cause the goldens' file name carries, not the one the runtime acts on. The seats measure it on their own builds and under `--sanitize`; the clause is false
on the two **adopted** cells I ran (today's two cells refuse it, for the reason R2 removes).
```
/* p10.h: the HANDLER refills a buffer by memcpy from a save the main line made, then jumps to the refilled buffer: it leaves the handler */
#include "prelude.h"
static sigjmp_buf saved, copy;
static void handler(int sig) { (void)sig; memcpy(copy, saved, sizeof saved); siglongjmp(copy, 1); }
int64_t run_case(cb_t cb) {
    if (on(SIGUSR1, handler) != 0) return -2;
    if (sigsetjmp(saved, 1) == 0) { raise(SIGUSR1); return -3; }
    return call(cb);
}
```

**B23. (the arms' clauses against the cells I ran: the answer to the warden's item 2, *which kind survives a falsifying program*, already has two measured falsifiers)** Cells are the adopted builds only (Darwin C4r / Linux C2679), `-O0`, plain,
3 of 3 each; the seats re-measure with `--sanitize` and their own shapes. `T` = held, `F` = falsified by the program named.
| clause (arms) | Darwin C4r | Linux C2679 | program |
|---|---|---|---|
| leaving by `longjmp`/`siglongjmp`/`_longjmp`, callbacks run, *whatever memory* (E1, E1b; E2's and E3's *runs*) | T | T | p1 static, p2 heap, p9 40 deep, p3 and p10 copied |
| an inline jump aborts the next callback (E1, E1b, E2, E3) | T | T | p4 (asm on arm64; the `__builtin_longjmp` spelling is x86-64 only, B11c) |
| a context switched away and never back aborts (E1; E2's and E3's *any other exit*) | T | **F** (RUNS) | p7 |
| *a jump to a buffer the handler refilled* aborts (E1, E1b) | **F** (RUNS) | **F** (RUNS) | p10 |
| *to a buffer a save of that family filled* (E2): any other exit aborts | **F** (p3, p10 RUN) | **F** (p3, p10 RUN) | p3 and p10 leave by `siglongjmp` to a buffer a `memcpy` filled |
| a handler that returns runs (E2) | T | T | p5 |
So **E2 is falsified by a program already in the registered blind set (p3), on both adopted builds, in its own words**; **E1 and E1b carry a clause (*refilled*) that p10 falsifies on both**; E3 is the only arm none of my programs
falsifies on Darwin, and on Linux it shares E1's and E2's context clause with p7. The context clause is true on Darwin and waits for defect 2702 on Linux (the brief's own expectation, now measured). Untested by me: the C++ clause (C5), `--sanitize`, x86-64,
glibc 2.43, a callback on another thread or from a second handler (the engineer's item 4).

**B24. (a hard stop the brief's last section does not mention) The sitting's own files are on the site's publish list, and so is the spec and the runtime.** `.github/workflows/deploy-site.yml` `paths:` (read, lines 63-82; `grep -n` prints `73:      - 'docs/panel/**'`) holds `'docs/panel/**'` (line 73),
`'spec/heroes-spec.md'`, `'runtime/**'`, `'selfhost/**'`, `'.claude/agents/**'`, `'tests/harness/suite_spec.hero'`; `site/src/lib/spec-page.ts:33` reads `spec/heroes-spec.md`. `CLAUDE.md` § Hard stops: *"A push touching a path in `deploy-site.yml`'s `paths:` ... publishes the
site. Say how many site commits would travel and wait for the yes **before committing**"*, and `.claude/rules/verification.md` names the site's build (`npm run build` in `site/`) as owed before such a push. `00-shared.md` § 7 says *"the sitting and the reports are committed
by pathspec in the lane"* and, of the sentence, *"enters the spec only through the runtime lane's landing"*: neither says that the commit of `docs/panel/257-*` (briefs, reports, synthesis, the adopted diff) is itself a site-publishing path, nor the count of site
commits that would travel, nor the author's yes. (The lane's branch is `lane-p257`; a push is the coordinator's and asked: the point is that the *commit's* wait is in the contract.) Add one line to § 7: the sitting is committed on the lane, **not pushed**, and the count and the ask are
made when `rebel-round` is pushed.

## The registered key, run on both platforms and on the adopted builds (the cells I could measure in this reading)

All `-O0` (the `heroes build` default), plain, 3 runs per cell under `timeout 20`, output to a file inside my copy, never `timeout heroes run`; built by **my own seed compiler**
(`./heroes build pN.hero -o pN.bin`, the runtime the same C file), the adopted runtimes by `HEROES_RUNTIME=<copy of runtime/ with the route's files over parts/>`. Mac: this Mac (Darwin arm64,
Apple clang 21.0.0). Linux: the container `heroes-linux-arm64:latest` (Debian clang 22.1.8, glibc 2.41, aarch64), the seed built inside it, containers `p257-critic-probe`, `-bl`, `-bl2`, `-1`, `-2`, `-3`, `-4`, all `--rm`, removed
(`docker ps -a --format '{{.Names}}' | grep p257` prints nothing, run again at the end). Files: `build/blind/`, `build/linux/{run.sh,run2679.sh,run10.sh,run8.sh}` and `build/linux-*.log` of my copy (`build/` removed at the end with `rm -rf build`; the two compilers remain in the copy's root, ignored by git: `heroes` from the seed, `heroes --version` sources `05186f2b38e77a97`, and `heroes-next` stage 2, sources `56db985736468b40`, so the second reading need not rebuild them; its `build/` caches are gone).
`md5 -q` of the route files used: Darwin `rebind.c` `2730f161...`, `sigguard.c` `3c603aff...` (the table of § 4); Linux `sigjump.c` `ba8487af...` (`md5sum` in the container).

| program | Mac today | Linux today | **Darwin C4r** (adopted) | **Linux C2679** (adopted) | `answers.md` adopted |
|---|---|---|---|---|---|
| p1 `siglongjmp`, static | RUNS | RUNS | RUNS | RUNS | RUNS |
| p2 heap buffer | RUNS | RUNS | RUNS | RUNS | RUNS |
| p3 whole copy | ABORTS (U: *holds no record of*) | ABORTS (U) | **RUNS** | **RUNS** | RUNS |
| p4 inline jump (asm on arm64) | ABORTS (S) | ABORTS (S) | ABORTS (S) | ABORTS (S) | ABORTS |
| p5 handler returns | RUNS | RUNS | RUNS | RUNS | RUNS |
| p6 handler calls it | ABORTS (S) | ABORTS (S) | ABORTS (S) | ABORTS (S) | ABORTS |
| **p7 G (`swapcontext` away, never back)** | RUNS | RUNS | **ABORTS (S)** | **RUNS** | ABORTS *(Linux only after 2702)* |
| p8 the handler jumps within itself, then calls (B9) | ABORTS (S) | ABORTS (S) | ABORTS (S) | ABORTS (S) | (none) |
| p9 jump to the first of 40 saved buffers (B15) | RUNS | **ABORTS (U)** | RUNS | RUNS | (none) |
| p10 the handler refills from an outside save and jumps (B22) | ABORTS (U) | ABORTS (U) | **RUNS** | **RUNS** | (none) |
Every cell 3 of 3 (exit 0 and the two lines `answer ran for 7` / `result 70`, or exit 134 and the words named). Plus, on this Mac, today, `-O2` and `--sanitize`: p1 to p7, 3 runs each (42 more runs), the verdicts of the
`-O0` column (p8, p9 and p10 only at `-O0`). **Conclusions that are measured:** (i) every cell of `answers.md`'s *adopted* column is what the adopted **Darwin** build does, for the seven programs; (ii) on the adopted **Linux** build **p7 RUNS**,
so *"G refused on both platforms"* is not what the builds that exist do (the brief expects this: *"where those differ the report says which clause waits for which defect"*); the registered grading for p7 and t2's design B is *"the landed build's own run on each platform"*, which on
today's builds is **a split**, and a run started before defect 2702 lands would grade p7 and B on Linux as RUNS; (iii) C7's cell *"S on both"* (`00-shared.md` § 6) is false of p7 on Linux C2679 (3 of 3 RUNS), true on Darwin
C4r (3 of 3 S); (iv) p9 shows C10 failing **today** on a correct program (Mac RUNS, Linux ABORTS in the U words) and closed by both adopted builds. Linux x86-64, glibc 2.43 and `--sanitize` on the adopted builds are unrun by me.

## What is missing from the briefs (not findings of fact; routes and checks nobody listed)

- **The placement of the sentence** (B1, B14): before `No function of the program is a signal handler`, not after `its last call`; and **a decision who owns the REMEDY update** (the runtime lane's landing carries
  `sigguard.c`, 22 goldens and the already-red test of B2, whatever arm wins). Measured: both placements 9800 vendored for e3 (build/arms/e3before).
- **The document's own instruments on each arm** (spec-shape: *"instruments first"*): none of the four briefs asks a seat to run the `spec` suite on its text. I did: `./heroes-next run tests/harness/main.hero --
  ./heroes-next spec` [copy], baseline **35 passed, 0 failed** (2:24 real); with each of e1, e1b, e2, e3 and the e3-before-the-remedy variant copied over `spec/heroes-spec.md` (restored each time, `shasum -a 256` began
  `15804f3f` after): **31 passed, 4 failed for all five, the same four: `spec/budget`, `spec/spendable`, `spec/real`, `spec/ledger`** (`grep -E '^(FAIL|  spec:)' build/spec-<arm>.log`). So no arm trips `named`,
  `rejected`, `inventory`, `anchors`, `offered` or `shape` (the code spans `_longjmp`, `__builtin_longjmp` and `siglongjmp` are admissible); the four pins are the landing's (`SPEC_TOKENS`, `REAL_TOKENS`, the ledger row,
  per `spec-shape.md` § How a change to the document is made).
- **`atexit`, the spec's own example of a callback (line 427), reached through `exit()` inside a handler:** the 2494 golden `...a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused` is
  refused today in the **S words** (`head -c 420 ...expected`: *"at_end was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that
  handler ..."*), though that handler never left; it is line 427's *called by one*, and the arms' *"the next callback aborts saying the handler was not seen to end"* is true of it only by the S words'
  accident (the handler was not seen to end because it did not). A reader told *"a handler left any other way ... is held running"* is not told that a handler that did not leave is refused in the same words.
- **A p8 for B9, a p9 for B15 and a p10 for B22** in the registered programs (or the seats' tables), each with the platform where it differs (p8: ABORTS on all four cells; p9: Mac and Linux differ today; p10: today refused on both, adopted run on both). The three are written and measured in this file.
- **The runtime lane's REMEDY drift is not the only moving word:** `git -C .claude/worktrees/lane-p255-impl diff HEAD -- runtime/parts/sigguard.c | grep '^[+-]' | grep -i -E 'record|seen to end|REMEDY|mailbox'` shows the lane
  already editing the wording around the record and *"handler not seen to end is held to be running"*, uncommitted; the brief says nothing of it is a premise, which is right, and the check against the final words is *owed*
  with no instrument named: the only instrument that exists is B1's test, and it reaches the last sentence only.
- **The `-O` level and `heroes run` against `heroes build`:** `heroes build` defaults to `-O0`, `heroes run` to `-O2` (`./heroes --help`: under `build` the line `-O0 ... (the default here)`, under `run` the line `-O2 ... (the default here)`); the brief's cells say *plain and
  `--sanitize`* and never an `-O`. I ran p1 to p7 at `-O0`, `-O2` and `--sanitize` on this Mac, 3 runs each (63 runs): **the verdicts did not move** (p1, p2, p5, p7 exit 0; p3, p4, p6 exit 134), so for these seven the
  level does not matter on Darwin; the seats' own shapes (the golden's header says SIGILL appeared at `-O2` under `--sanitize` on Linux arm64) may.
- **A POSIX reading of *though C is correct* on this Mac's own man page** (a datum for the ffi-pragmatist's item 4, measured not recalled): `man -P cat sigaction | sed -n 198,260p | col -b | grep -c -E 'longjmp|swapcontext|setcontext'`
  = **0**: the page's list of async-signal-safe functions holds neither `siglongjmp` nor `swapcontext`, and ends *"All functions not in the above lists are considered to be unsafe with respect to signals."* So
  E1's positive clause (*"C may leave a handler by `longjmp`, `siglongjmp` or `_longjmp`"*) is as unsupported by that page as the refused exits are; what makes the longjmp family *correct* is the standard's text, which no brief
  asks anyone to read.
- **t2's question 2** (*"which design do you ask for"*) has key *A or C* and no rule in D1 to D3 that uses it; it costs tokens in 15 sessions and measures nothing registered.
- **D1 counts p3 where p3 may not be able to move a reader** (an inference, unrun): if a `now` reader's default for a `siglongjmp` through a copy is RUNS, which the key calls right, p3 contributes only on the `adopted` side
  (where it detects E2's *"a buffer a save of that family filled"* read literally as an abort). Put p3 in D2's set too (*must not fall*), so that an arm that teaches ABORTS for it is withdrawn.
- **Routes nobody listed** (the arms are all *"append to line 428's end"*): (1) the sentence **before** `No function of the program is a signal handler` (B1, measured); (2) the sentence naming the **class**, *a jump that lands within the handler* (B9, B22),
  instead of a list of causes, which p10 falsifies one of; (3) the sentence **scoped by platform** (B12: *where the platform has signals*); (4) the sentence **being part of the runtime's refusal**: since B1's test holds the spec's last sentence to the
  runtime's words byte for byte, a sentence placed after `its last call` would be held to the runtime for free, at the price of printing it at the end of every signal refusal, K and D and the allocator's included: a route with its own
  cost, which no brief weighs; (5) E0 with a **golden that pins the S noun** (the warden's item 5 gestures at it; its price is the test of B1 over again for a second sentence: 22 lines of Heroes, a test beside `callback_thunk.hero:354`).

## Unrun by me, and said so

Linux x86-64 (`ubuntu-26.04`), the CI's `macos-26`, Windows (no machine), glibc 2.43 for the adopted Linux build (`ubuntu:26.04` is a local image, `docker image ls ubuntu` shows it, `ubuntu-bins.sh` takes `VARIANT=C2679`),
`--sanitize` of every adopted cell, the C++-exception clause (C5), another thread's callback and a second handler (the engineer's item 4), real watchdog / scheduler programs (the ffi-pragmatist's item 3), any `--refresh`
count, any paid run. Every cell of the key table is 3 runs at `-O0` plain; a count of 3 is a spot check of the brief's *"three runs a cell"*, not a rate.

## What the coordinator must repair before any seat starts

Most important first; each names the finding and what to write.

1. **Decide where the sentence goes, and tell every seat (B1, B2, B14).** All four arms append to the end of line 428, which the compiler's own test (`selfhost/emit/callback_thunk.hero:354`, defect 2533) holds byte for byte to the C constant
   `HERO_SIGNAL_REMEDY` that ends every signal refusal and is pinned by 22 goldens. Either issue arm variants with the sentence **before** `No function of the program is a signal handler` (measured for e3: same 9800 vendored tokens, the test's red unchanged
   by the arm) or say that the landing owes `sigguard.c` and 22 `.expected`. Put in every brief that **that test is already RED at the frozen tree** (the mailbox clause of `c0476dff3` is in the spec and not in the runtime), so a seat running the compiler's tests does not
   read it as its arm's, and that the shared brief's *"every one is followed by the spec's own rule"* is false at that clause today.
2. **Correct the pass criterion of the two builds (B10).** `run fixedbugs-2522` on an adopted build is **23 passed, 2 failed**, exactly the two goldens R2 moves (panel 255's own gates, both seats); write that as the pass, or a seat reports a failure of the route it was told to
   assume, or hides the two.
3. **Hand the seats the falsifiers already measured, and correct C7 (B22, B23, B9, B15, B17).** p10 (a handler refills a buffer from an outside save and jumps: RUNS on both adopted builds) falsifies *"a jump to a buffer the handler refilled"* (E1, E1b); p3 and p10 falsify E2's
   *"to a buffer a save of that family filled"*; p7 RUNS on Linux C2679 (3 of 3) against Darwin C4r (3 of 3 S), so C7's *"S on both"* is the expected split, not a result to hunt; p9 (40 live saves) splits **today** (Mac RUNS, Linux U) and both adopted builds run it. Add p8
   (the handler jumps within itself, the golden's shape), p9 and p10 to the programs the seats run on both platforms (the code of all three is in B9, B15 and B22). The class the runtime acts on is *a jump that lands within the handler*; the arms list causes.
4. **State the platform scope the sentence claims (B12, B13).** Windows is the CI's third leg and `sigguard.c:506-521` has no probe there (line 427's own rule is not enforced); the adopted Linux build was never run on the CI's glibc 2.43. Ask the engineer for `C2679` through
   `ubuntu-bins.sh`/`ubuntu-run.sh` and the accessor's reading beside every row; decide in the brief whether the sentence is scoped (*where the platform has signals*) or knowingly Windows-false.
5. **The blind registration is not what the brief says (B3, B4, B18, B20).** Name the files changed from panel 256's runner (`run.sh` constants and paths, `check_folders.py`'s 46 and its flat listing, `brief-session.md`'s input names and report structure, `make_sessions.py`, `preflight.sh`) with
   a `diff` against 256's bytes; the verdict session's folder is built after the 30 sessions, not by `make_sessions.py`; the verdict runs only if the 30 spent at most **10.90 USD** (mean 0.363) under the 0.60 / 1.00 / 0.10 / 12.00 rules; replace *"no `CLAUDE.md` in or above it, by `find`"* (it cannot pass: the repository's `CLAUDE.md` and
   `AGENTS.md` are above the area) with `--bare` plus the free preflight's transcript plus `scan.py`; ask the warden for predictions **per candidate text**.
6. **The blind material (B5, B6, B7, B8).** p4.h's comment is the golden's, project-internal, and tells a `now` reader what the runtime does: replace it by one neutral line and re-hash; `task-t2.md`'s *"the words it prints"* is 337 of 809 characters: quote whole or say shortened; the t2 key (A RUNS, B ABORTS, C RUNS)
   has no program behind it, mark it unrun until the ffi-pragmatist's item 3(c) programs (timer-driven, not `raise`) are kept in `programs/`; p6 ends `return 1;` and can never print `result 70`, so t1's definition of RUNS is unanswerable by it.
7. **One line for § 7 (B24):** the sitting's files (`docs/panel/**`), the spec and the runtime are on `deploy-site.yml`'s `paths:`: the sitting is committed on the lane and not pushed; the count of site commits and the author's yes belong to the push.
8. **Small corrections (B11, B16, B19, B21).** § 3's *"three copies"* is 17 files (8 distinct); `g2_1` is under `critic/p255/r3/fsh/`; `g++` is clang on both platforms and there is no GCC (so *"a C++ exception"* has one front end); the author's sentence is the coordinator's *"meant as"* gloss of *1a and 2a*, and the
   author asked for *"a small sitting with the spec-warden"*, which the brief turned into a four-seat sitting without saying which word it follows; cite the author's rule of 2026-09-09 (`guard_bash.py:1006-1013`) for the `--refresh` exception rather than *"generates nothing"*; bound **stderr** as well as stdout in the model script.
