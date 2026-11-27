# Panel 262, the completeness critic: first reading, the briefs before any seat starts

Started 2026-11-26 15:07, in the detached copy `.claude/worktrees/262-critic` at `aded91388`, scratch in
`.claude/worktrees/262-critic-scratch/`. The machine read `load averages: 99.59 67.89 50.89` at 15:07 and 109 to 138
through the reading (`uptime`), so **no duration below is a measurement**. Written as it goes; a section marked *in
progress* was not finished. Every finding is numbered **C<n>**; the last section orders the repairs.

## 0. What was run, and on what

- **This Mac** (Darwin arm64): the seed built here, `sources 05186f2b38e77a97` (the clerk's, `heroes --version`);
  stage 2 (§ 4 says what it read).
- **Linux arm64**, `heroes-linux-arm64:latest` (`338a1044ac90`), one container `p262-critic`, `--rm --pull never`,
  over a `git archive aded91388` of this copy with `blind/` and `ground/` added (`262-critic-scratch/linux-tree/`):
  Debian 13, glibc 2.41, clang 22.1.8 (`linux-run.out`). Seed, stage 2, `blind/key.sh`, `ground/probes.sh`, and the
  route accessor's case; then a second container, my three programs and what the image holds of the ffi-pragmatist's
  libraries. **The image has no `/usr/bin/time`** (my first container exited at once on it; the next dropped the
  timing). No container is left (`docker ps -a | grep p262`: none).
- **Read-only GitHub** (`gh issue view`, `gh run view --log`), saved under `262-critic-scratch/issue-*.json` and
  `ci-job-*.log`.

## 1. Framing facts re-run by command

| fact (where the brief states it) | command | result |
|---|---|---|
| base `aded91388584da...` 2026-11-26 14:11:41 (`00-shared.md` head) | `git log -1 --format='%H %ci'` | same |
| spec 516 lines, 35,246 bytes, sha256 `3df14254...d93f` (§ 3.1) | `wc -l -c`, `shasum -a 256` | same |
| last three spec commits 197d, 192c, 126a (§ 3.1) | `git log -3 -- spec/heroes-spec.md` | same |
| the sentence on line 428; the only *may abort* (§ 3.1, q4) | `grep -n 'left a handler'`; `grep -n -o -i '[^.]*may abort[^.]*'` | line 428; one sentence (but see **C6**) |
| no system named; *platform* twice, lines 330 and 401 (q3) | `grep -n -i -E 'platform\|linux\|darwin\|windows\|macos'` | lines 330 and 401 only: same |
| the thread: only line 383 (q8) | `grep -n -i 'thread C\|C made\|thread of its own'` | line 383 only: same (see **C11** for wider words) |
| `spec_refusals.hero` 222 lines; `SENTENCE_OPENS` 39-40; first `". "` 68; `CLAIMS` 32-36; `EXEMPT` 43-44 (q6) | `wc -l`, `Read` | same |
| `spec/refusal-words` at `suite_spec.hero:1638-1648` (q6) | `sed -n 1630,1652p` | same |
| the 2533 test at `callback_thunk.hero:400-413` (q6) | `sed -n 395,429p` | the comment 400-406, the test 407-429: same |
| the map's 23 line counts (`compiler-engineer.md` § The map) | `wc -l` of every file named | all 23 equal |
| `sigguard.c` pointers: 49-60, 61-74, 110-115, 229, 576-622, 637, 642, 663-668, 679-688, 692, 694 | `Read` whole | all as stated |
| `thread.c:128-139` the thread's refusal | `sed -n 100,144p` | same at the base; **at `main` 132-143** (step 200a, **C1**) |
| `sigentries.c` rules 27-61, the life of a range 103-126, route A 127-133 | `Read` 1-140 | same |
| `sigjump.c` routes 27-58, residues 68-81, `HERO_JMP_NO_DECODE`/`HERO_JMP_MAKE_IN_C` "130-141" | `Read` 1-145 | same; the defines are 130-132 and 134-145 |
| goldens per family "62, 1, 2, 4, 10 and 6 by `ls \| grep -c`" (`compiler-engineer.md` item 2) | `ls \| grep -c` per prefix | **133, 2, 5, 9, 21, 13**: the brief's numbers count `.hero` files only (`grep '\.hero$'` gives 62, 1, 2, 4, 10, 6). The command named does not give them |
| the census (§ 3.3, § 3.4) | `sh ground/census.sh` in my copy | **byte-equal** to `ground/census.txt` (`diff`) (but see **C5**) |
| blind inputs by hash (`registration.md` § The inputs) | `shasum -a 256 -c inputs.sha256` | 29 of 30 OK; the 30th line names `arms/spec-now.md (spec/heroes-spec.md at aded91388)`, no such file, so `-c` fails it; its hash equals the spec's above |
| issue states (§ 3.5) | `gh issue view <n> --json state,...` | 2706 closed 2026-11-23T21:04Z; 2848, 2850, 2851, 2852, 2853, 2855 open; 2849, 2861 open `adjacent`; **2854 open, repaired by step 200a at `2978e6aed`** (**C1**) |
| 2848's Origin quotation (q1) | `gh issue view 2848` | quoted correctly |
| panel 257's lines 56-62, 65-66, 67-69, 106-109, 113-151, 134, 180-184, 244-331, 281-283, 329-331 | `Read` whole | each says what the brief says it says |
| panel 259's lines 39-44, 128-136, 141-142, 200-205, 303-306, 315-323 | `sed -n` | each as stated |
| x1 and x2 quoted (§ 3.2) | `cat 257-reports/spec-x1.txt spec-x2.txt` | as quoted (x1 elided with `(...)`) |
| what touched the guard since panel 257 (§ 3.3): 120d *"+63"* each, 199a *"+154 −118"*, 192a *"`callback.c` −186 net"* | `git show --numstat` | 199a as stated; 120d's *"+63"* are changed lines (`sigentries.c` and `rebind_entries.c` `+56 −7` each, `rebind_jump.c` `+11 −1`, `sigjump_save.c` `+10`); **192a's `callback.c` is `+9 −177`, net −168**: the 186 is the changed lines, not the net |
| step 199a's body (§ 3.3, q3) | `git show -s 65da4688c` | as quoted; it also says **"13 rows keep route A's own UNFOLLOWED (filed apart)"**, which no brief carries (**C8**) |

## 2. Findings

**C1. The lane's state moved, and `main` with it (asked 15:09).** `git log --oneline main..lane-sig-family`: nothing.
But **the lane's branch head is no longer `aded91388`**: it is `6079623b6` (14:44:58, *"Panel 261: the blind reading
..."*), a commit of `main` (`git merge-base --is-ancestor lane-sig-family main`: true), so the lane merged `main` as its
plan said (*"commit S, merge main, then E"*) and has committed none of E, G or J. **`main` is 15 commits past the base**
(`git log --oneline aded91388..main | wc -l`: 15, to `648b76602` at 15:04), among them **step 200a** (`2978e6aed`,
defect 2854: `runtime/parts/callback.c` +84 −28, `thread.c`, `spawn.c`, `os.c`, `frame.c`; and step 198d, `cd001bf18`, `replace.c` +20 −13, comments of the Windows link arms; `git diff --stat aded91388
main -- runtime/`: 6 files, +128 −49). It touches **the door every callback crosses** (`hero_callback_in`, the count
the room's abort reads) and moves `hero_thread_refuse` to `thread.c:132-143`. No spec change since the base (`git diff
--stat aded91388 main -- spec/`: empty). **Repair**: the shared brief's *"its branch head `aded91388`, `main`'s"* and
*"2854 ... lane room-mixedown"* are stale; say that `main` moved, that 200a changed the callback door and the room's
count (not the guard's questions), and that the landing meets `main` at 200a or later, so a seat runs its decisive
rows on `main`'s runtime as a second column (panel 257's own lesson, *"a sitting that tests a sentence against a route a
lane is landing owes the lane's tree as a second column from the first brief"*).

**C2. Darwin x86-64 stands on route A, and no brief says so.** § 3.3's *"which route stands where"* quotes `sigjump.c`
(Linux): route A on *"glibc's four other architectures, a glibc whose layout moved, a build forced to it"*. But
`runtime/parts/rebind_decode.c:314-315` says *"Darwin x86-64: no decode is read here (no machine to calibrate it on),
so both families stay on route A, by construction"*, and the route accessor's golden says the same
(`fixedbugs-2679-the-runtime-says-which-route-decides-a-jump.hero`: *"A platform whose layout does not verify answers
lower ... (glibc's other architectures, Darwin x86-64)"*). **An Intel Mac is a real platform a reader calls "a Mac",
route A by construction, unrun, and in no CI leg** (`ci.yml`, `rebel.yml`: `ubuntu-26.04`, `ubuntu-26.04-arm`,
`macos-26`, `windows-latest`). Route A refuses 2706's two programs (199a's body) and holds every `makecontext` range
(rule 7), so **`h7`'s key and x1's promise are, by the runtime's own text, false on an Intel Mac** (an inference from
the code and 199a's simulation, unrun on such a machine). The blind tasks say *"a 64-bit Arm machine, a Mac or Linux"*,
so the readers are scoped to arm64; **a spec sentence is not**. **Repair**: § 3.3 and q3 name Darwin x86-64 beside
glibc's other architectures; (d2) *"a promise per platform"* must say what it promises an Intel Mac; the warden's texts
say on which architectures each run-promise holds, or promise nothing positive.

**C3. x1's second condition is no longer unrun.** q3: *"the CI's glibc 2.43 reading the accessor, is unrun by the
clerk (a CI run's log; the critic's)"*. CI run `45654270318` (push of `4a5e585a9`, 2026-11-25, conclusion `failure`
for other reasons), its Linux x86-64 jobs on `ubuntu-26.04` (*"Included Software: ... images/ubuntu/Ubuntu2604"*,
`Ubuntu clang version 21.1.8`) and its Linux arm64 jobs (`ubuntu-26.04-arm`): the three `run` shards of each leg name
six `FAIL run/` lines (`abort-calls-*` at `-O2` and `--sanitize`, two header-macro cases) and one `SKIP run/` of the
2679 family (`libpng-decodes-...`), and **neither names `fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`**,
whose `.expected` is `route 3` (`gh run view --job <id> --log`, jobs `139346835548`, `139346835603`, `139346835562`
(x86-64) and `139346835834`, `139346835543`, `139346836292` (arm64)). So the case passed on both CI Linux legs, an inference from its absence from the failures (the
log prints failures and skips by name, passes by count). The `emission` form there failed it as *"nothing blessed"*,
which is the emission's, not the route's. The glibc version on that image I did not read (panel 257 says 2.43;
carried). **Repair**: q3 says the condition is met at `4a5e585a9` by that log, the pre-199a runtime; 199a moved no
calibration (`git show --stat 65da4688c` names `sigjump.c`, `sigjump_make.c`, `sigjump_save.c`, `sigjump_record.c`,
`sigguard.c`, `rebind_jump.c`, `sigentries.c`, `rebind_entries.c`, and no `sigjump_decode.c`, `sigjump_probe.c` or
`rebind_decode.c`), so the condition stands unless a later CI run says otherwise.

**C4. x1 is falsified at the base by programs the brief itself runs, whatever its two conditions.** q3 and § 3.2
present x1 as waiting on two conditions (2706's repair, met; the CI's accessor, **C3**: met). But x1 promises *"a
handler of C's is over where ... `longjmp`, `siglongjmp`, `_longjmp` ... carries the thread out of it onto the thread's
own stack, and a callback then runs"*, and at the base on this Mac (the clerk's own § 3.4 table):
`fixedbugs-2705` (a handler's `siglongjmp` into a live coroutine **on the own stack**) is refused in ONSTACK's words;
`2702-a-handler-jumps-into-a-coroutine-on-the-own-stack-which-calls-the-function-is-refused` likewise; and **any
asynchronous handler that `siglongjmp`s out of code inside the runtime's allocator stops the program at the jump**
(panel 258's R1, `sigguard.c:175-212`, `hero_signal_jump_checked`, **C7**). A coroutine carved from the thread's own
stack is "the thread's own stack" by address; x1's reader cannot tell it is excluded. On route A (**C2**) the two 2706
programs are refused too (199a's body). **Repair**: q3 and q11 must not read as *"x1 lands when its conditions hold"*:
the conditions R2 named are met and x1 is still false as written; the warden's (c) variant that reuses x1 must be held
to 2705, the own-stack coroutine and **C7** before it is offered.

**C5. "85 files" holding a claim are 84, and the 85th is no refusal.** § 3.3 and q13 (*"the goldens' words where a
claim moves (85 files)"*). `grep -rlE 'seen to end|from inside' tests/golden --include='*.expected'` lists 84 `run`
cases and `check/package-a-file-source-in-no-repository.case/main.expected`, whose `from inside` is *"read only from
inside the program's own repository"*. The arms' own counts agree with 84: STANDING 26 + SWITCHED 41 + REENTERED 5 +
ONSTACK 12 = 84 (§ 3.3's table). So *from inside* is 31 refusal goldens, not 32. **Repair**: say 84 and name the
outlier, so a landing that re-words a claim by `grep -rl` does not rewrite a `check` golden.

**C6. *"The spec's only may abort"* is true of the phrase and not of the class.** `grep -n -o -i 'may [a-z]*'`
finds one *may abort* (line 428). But the spec already states, in other words, aborts a reader cannot predict from
the program: § 9's *"running out of stack aborts"* (line 331, no depth given); line 487's handle given back *"aborts,
... unless C has since handed its address out again or a million other lives have ended"* (a stated exception);
line 425's *"C reaching past them aborts, or fails the call where the kernel writes"*; the room's bound (line 428,
a count the run decides). **Repair**: q4 should hand the warden these as the spec's own precedents for wording a
conditional abort (line 487's *"unless"* states the exception rather than a *may*), so *may* is weighed against how the
document already words the same kind of outcome.

**C7. A refusal of a handler's jump that no brief names, and which falsifies every run-promise.** `sigguard.c:175-212`
(panel 258's R1, defect 2755, landed): *"a jump out of a handler that interrupted this runtime's allocator"* stops
the program **at the jump**, in words of its own (*"panic: a signal handler jumped out of the code it interrupted while
that code was inside this runtime's allocator ... so the program stops here, before the jump"*), which hold neither
claim, are no arm of `hero_signal_refuse` (so `spec_refusals.hero` never reads them), stand in **no golden** (`grep -rlF
'jumped out of the code it interrupted' tests/`: nothing) and in no spec sentence (panel 258's R1: *"no spec sentence"*).
Beside it, the allocator's guard D (`alloc.c:673`, *"a signal handler used this runtime's allocator while the code it
interrupted was inside it ... a Heroes function given to C as a signal handler, or called from one"*, ending in
`HERO_SIGNAL_REMEDY`) is what a handler installed by a raw system call meets (2 goldens,
`fixedbugs-2522-a-handler-installed-by-a-raw-system-call-*`). **A watchdog** (`SIGALRM`, a handler that `siglongjmp`s
out of whatever it interrupted) is the C design whose outcome depends on where an asynchronous signal lands, which no
reader can see. q9 lists two aborts beside the sentence (the room's, § 9's stack); **there are at least four**, these two
added, plus the thread's (q8) and DELIVERED (line 427). **Repair**: q9 and § 5 add the watchdog shape and both words;
every candidate that promises a run after `siglongjmp` (x1, (b)'s *"known to have ended"*, (d1)) must be scoped to a
signal that interrupted C and not the runtime, or say that a jump out of the runtime's own code stops the program.

**C8. A seventh guard defect stands drafted and unfiled, and it moves q4's split.** `.claude/worktrees/rebel-scratch/
coord/issues/d-lane-sig-family-1.md` (14:11 today, `github: none`, *"filed apart"* in 199a's body as *"13 rows keep
route A's own UNFOLLOWED"*): **on route A** a handler's `setcontext` to a context kept on the heap is never trusted, so
`fixedbugs-2702-a-handler-left-by-setcontext-to-the-runner-runs` (`b_0`) and `...-to-its-coroutine-runs` (`cs_0`),
**correct programs, exit 134 in UNFOLLOWED's words**, and ten rows that must be refused are refused in those words where
route C says SWITCHED or REENTERED. **Class blocking**, and the draft says where route A stands: *"glibc's riscv64,
loongarch64 and 32-bit arm, on Darwin x86-64 and in a forced build; no CI leg runs it"* (confirming **C2** from the
lane's own text). So q4's *"a `setcontext` out of a handler to its coroutine runs with the handler on the thread's own
stack"* is **route C's alone**, x1's *"a `setcontext` to a context `getcontext` took ... a callback then runs"* is false
on route A, and UNFOLLOWED's *"0 goldens"* is a Mac-and-arm64 count, the arm route A prints most. **Repair**: § 3.5 gains
this draft as a seventh row (with whatever number it takes), and q4's split is stated per route.

**C9. 2855's class has recurred, and the briefs give one member of three.** Defect **2831** (closed; *"On Linux a
`makecontext` reached through `dlsym` is glibc's, so the coroutine's stack is never named and a correct program is
refused in the words of a signal handler it does not have"*, panel 259's critic's `unseen_carved`, repaired by step
185a, `a4bdf4257`), then 2852 and 2855: **three no-handler programs told a handler's words, all three filed on 2026-11-26** (GitHub's
`createdAt`: 2831 12:37:45Z, closed at once; 2852 12:52:13Z; 2855 12:53:36Z), by three
mechanisms (an unseen `makecontext`, rule 6 on a carved stack, the walk on Darwin's fiber frame pointer). § 3.6 lists
neither 2831 nor 185a. **Repair**: § 3.5 and § 3.6 name 2831; it is the measured base rate behind (a) and (d3)'s
question *"can a sentence promise that a program with no handler is never told these words?"*: it has been false three
times by three routes, and the compiler-engineer's item 3 (*"find a third"*) should start from the three.

**C10. design.md speaks on coroutines, and the brief says it does not speak.** § 3.6: *"design.md names neither
`swapcontext`, `makecontext`, *seen to end* nor *handler of C's*"* (true, re-run: `grep -n -i -E` of the four:
nothing), and *"its signal lines ... none about the guard"* (true). But `docs/design.md:3628` (Part 6, the refusals
table): *"| Coroutines | require stack switching (`ucontext` or stack copying) — a step change in complexity |"*, and
`:4170-4174`: *"Threads, not green threads, not coroutines ... Coroutines are already refused"*. Candidate (a) names
*"a coroutine switch"* as a kind of its own; the spec already says *"a coroutine it never resumes"* (the room's
sentence), so the word is not new, but a sentence that promises what a **C** coroutine library is told sits beside a
design row refusing coroutines **in the language**. **Repair**: § 3.6 hands the warden these two rows for its item 7
(*"say whether a text owes design.md a line"*).

**C11. § 3.8's negative is false by its own command, and the wider words find more.** `git grep -l -i -F` of *seen to
end* over `rebel/` finds `rebel/library/0.1.0/files/process.hero` (*"the child was not seen to end"*, a process's,
unrelated) and `process.ledger`; of *may abort*, *handler of C*, *left a handler* over `docs/` outside `docs/panel/`
finds `docs/measurements/010-spec-budget-ledger.md:259-260` (the ledger's rows naming panels 257 and 259, a file the
landing writes anyway). Wider words: `rebel/library/0.1.0/files/os.ledger:454` quotes the remedy (*"No function of the
program is a signal handler (spec § 13)"*) and `term.ledger:137` speaks of *"the layer's `atexit` and signal
handlers"*; `examples/server` and two Rebel files call `hero_net_catch_stop`. None quotes the handler sentence itself.
**Repair**: § 3.8 says *"nothing quotes the handler sentence outside `spec/`, the runtime, the instrument and the
ledger"*, names the ledger, and names `os.ledger:454` as Rebel's one reader-facing page about signals, which a text on
*handler* (q5) must not contradict.

**C12. The runtime itself runs a signal handler on an alternate stack in every program, and no brief says so.**
`runtime/parts/stack.c:1265-1281` installs `hero_stack_handler` for SIGSEGV and SIGBUS with `SA_SIGINFO | SA_ONSTACK`
once per process, called from `hero_args_set` (`os.c:703`), on an alternate stack it registers per thread (`:1235`),
and chains to the handler it found (`hero_stack_prev_segv`, `:804-806`); under `--sanitize` the guard yields to ASan
(`HERO_STACK_GUARD_YIELDS_TO_ASAN`, `:112-117`; what it then installs I did not trace). So **every plain-built Heroes
program has a handler and an alternate stack before C does anything**, a C handler installed earlier runs called by the runtime's, and the golden
`fixedbugs-2494-a-heroes-function-a-c-handler-calls-on-the-runtimes-alternate-stack-is-refused` exists. It bears on
three things the briefs ask: whether *"a program whose C installs no signal handler"* (d3) is the right class (the
program always has one, the runtime's); the signal stack trick (a library that calls `sigaltstack` replaces the
runtime's alternate stack on that thread, and what § 9's *"running out of stack aborts"* then does is unrun); and
rule 4's *"the alternate stack registered now"*, which on every thread is the runtime's until C registers its own.
**Repair**: § 3.3 names it; the ffi-pragmatist's signal-stack-trick build reports what happens to the runtime's
alternate stack.

## 3. The blind registration, read for what it measures

**C13. The registration cannot measure the third reading, L, at all.** q1 names three readings (W, N, and L, 2848's
*"exhaustive list promising every other exit"*) and `llm-ergonomist.md` says *"Only a reader can measure which a text
leaves"*. But § The grades labels **only** *"each `NOT PROMISED` or wrong `ABORTS` on a program the key runs"*. An L
reader's error is the other kind: **a `RUNS` on a program the key aborts**, whose exit no list names. On the graded
platforms (route C), the programs that abort by an exit outside `now`'s five are: `n3` (no handler; out of every gate,
a defect's), `n4` (the thread; D4 only), and none in t1 (`h4` is line 427's call, not an exit). So **no gate and no label
can see L**; a text that invites it (any text that keeps a closed-looking list, (c) above all) passes every rule.
**Repair**: label every wrong verdict, a wrong `RUNS` included (L, or a new label for *"silence read as a promise"*,
which `n4`'s `RUNS` answers are); and add, keyed on both platforms, a program whose C leaves a handler by an exit no
text lists and aborts there: candidates, each **unbuilt**: a handler left by `_Unwind_ForcedUnwind` called from C (the
residue `sigjump.c:72-74` names, *"a C++ exception or `_Unwind_*`"*, STANDING's words), or by `siglongjmp` from inside
a system library's own frames. If none can be keyed, the registration says L is unmeasured, in those words.

**C14. W and H are one finding in the only data there is, so two graders will split them.** `259-reports/llm-ergonomist-
blind.md:157-161`: the four `NOT PROMISED` readers of t1 gave **one** reason, *"the handler sentence's 'may abort' covers
any frame C leaves ... whatever a handler is"*, `t1-u-5` naming its cause *"I took it to mean a callback frame"*: that is
H producing W. The labels are defined as alternatives (*"labelled by the reason ... W ... or H"*), with no rule for an
answer that gives both, which is what 259's answers are. And `t2-h-2`'s reason (*"a stack outside the thread's that the
runtime's 'running out of stack aborts' may misjudge"*, `:179-181`) is a fourth, § 9's abort read onto a coroutine's
stack, which only O catches. **Repair**: let an answer carry every label its `choice_points` support, or fix a
precedence (H before W where the reader says *handler* means a frame); add S (§ 9's stack read onto a switched stack)
and R (the room's abort read as reached); and **try the labels on 259's archived answers first**
(`259-reports/llm-ergonomist-blind-answers.tar.xz`, free, no session), two graders apart, so the disagreement rate is
known before a cent is spent.

**C15. D2 rests on one program, and a text can empty it by silence.** D2's set is *"each program the key runs and the
arm's classification puts inside a *may* or promises"*, and under `now` that is `h7` alone. So (i) one reader moves
D2 by 20 %; (ii) **the set is the arm's classification, written by the synthesis per arm**: a text that names no exit
`h7` falls in (and promises nothing for it) leaves D2 with no program, and it passes vacuously, which is exactly the
*"text that fixes one misreading by inviting the other"* the shared brief's § 4 forbids. 257's narrowing was measured
on programs that **do** abort (its 10 of 10 were build-right but for p7's Linux row, since gone with 2702), so the error
N has been seen once on a running program. **Repair**: fix D2's set before any session, the same under every arm: every
program the key runs on both platforms and that `now` puts in a *may*. At the base on this Mac those include `h7` and,
from the goldens, `2707`'s `siglongjmp` into a coroutine from a handler on an alternate stack (runs, 2 cases),
`2702`'s `setcontext` to its coroutine or the runner from the own stack (runs, 2), and 2848's `getcontext`/`setcontext`
jump on the own stack (runs): add two or three of them to t1, re-keyed on both platforms (route C only, **C8**); and
`programs/critic/other.hero` (**C24**), run: a handler left by assembly on a `spawn`ed thread, the callback on `main`,
inside `now`'s first exit by its letter, and it runs.

**C16. The classification of `now` misses the room's sentence and line 427 where they bite.** Program by program
against lines 427 and 428:
- `h1`, `h3`, `h2`, `h5`, `h7`, `h8`: as the clerk says (each read against the five exits).
- `h4`: as the clerk says, and the registration should say what is text-right for it (nothing is defined, so D4's
  report cannot grade it).
- `h6`: the clerk writes *may* by the fifth exit; but `h6.h`'s handler **calls the function itself** after its jump
  (*"then calls the program's function itself"*), which is `h4`'s line-427 case too. So `h6` is gated (D3) and `h4`,
  the same call without the jump, is not: say why, or class them alike.
- `n2` and `n3`: *"silent: no handler"*. But `leave`'s frames are left by `esc_leave`'s `longjmp`, so **the room's
  sentence applies** (*"Where C leaves a callback's frames without their returning (by a jump ...)"*), its abort not
  reached (no value over 4,096 bytes, no lend, one callback). A reader who answers `NOT PROMISED` or `ABORTS` for `n2`
  from the room's sentence is not W, and would be labelled W. **Repair**: the classification notes the room's sentence
  for `n2`, `n3`; the labels get R (**C14**).
- `n1`: silent, as the clerk says.
- `n4`: silent, as the clerk says; and line 383's *"reaching C through it on a thread that does not hold it aborts,
  naming the call"* (a handle's) is the nearest sentence a reader may borrow.

**C17. `n3`'s exclusion is sound in intent and wrong in its condition.** The rule: *"`n3` is out of every gate below
unless its key is one verdict on both graded platforms under the build of the run's day"*. At the base `n3` **is** one
verdict on both (REENTERED, this Mac by the clerk and by me; Linux arm64 by 2855's body and by my run, § 4), so by the
letter it is in; but no gate lists it (D1 `h1 h3 n1 n2`, D2 the *may* set, D3 `h2 h5 h6 h8`), so *"unless"* admits it to
nothing; and its key at the base is a defect's false message, which a gate must never reward. **Repair**: `n3` is graded
only once its key is what a text promises (2855's repair landed and the key re-run), and reported otherwise.

**C18. The key does not say what happens when plain and `--sanitize` disagree.** *"A program whose two graded
platforms give different verdicts has no key"*; the two **builds** are silent. 2852's mechanism (Darwin's fiber keeps
`getcontext`'s frame pointer; the walk climbs into gone frames; *"under `--sanitize` the layout puts such a word on
the chain, plain it does not"*, an inference in its body) is **`n1`'s shape**: a `makecontext` fiber on Darwin that
calls a callback. `n1 --sanitize` runs today (this Mac, 3 of 3, mine and the clerk's), by the layout's luck; step 200a
changed `callback.c` on `main` (**C1**). **Repair**: name the key's build (plain), or make a plain/`--sanitize` split
leave no key as a platform split does; re-run `n1 --sanitize` on Darwin under `main`'s runtime on the run's day.

**C19. Two smaller faults in the inputs.** (i) `task-t1.md` says each header *"includes `programs/prelude.h`"*;
`h2.h` and `h7.h` do not (`grep -L prelude.h programs/h?.h`: `h2.h`, `h7.h`). (ii) **`h2.h`'s first line quotes the
`now` sentence's first exit word for word**: *"a handler leaves by a jump no function of the C library makes"* (spec:
*"by a jump no function of the C library makes"*). It is panel 257's `p4` byte for byte, kept for comparison, and in
257 it matched the **adopted** arm's words; here it matches `now`'s, so under an `adopted` or `rival` that drops the
phrase, the program carries `now`'s vocabulary into the other arm. My `grep -n -i -E` of every input for the project,
a defect, the runtime, a panel, the spec's claim words and four-digit numbers found nothing else. **Repair**: (i) say
*"most include"*; (ii) either strip that comment's wording to the mechanism (*"a handler leaves by an inline jump
written in assembly"*) for every arm alike, accepting the break with 257's bytes, or register that `h2`'s comment cues
`now`.

## 4. Run again: this Mac, and Linux arm64, which the clerk did not run

**Compilers.** This Mac: seed `sources 05186f2b38e77a97`, stage 2 **`sources 7205f5b93a1fc8f1`** (`build-stage2.log`;
`real 719.25 user 220.71`, a load of about 120, not a duration). Linux arm64 (`p262-critic`, Debian 13, glibc
2.41, Debian clang 22.1.8): seed `05186f2b38e77a97`, stage 2 **`7205f5b93a1fc8f1`**, the same keys.

**§ 3.7.** `heroes-next measure spec/heroes-spec.md` (no `--refresh`): claude-legacy 10,067, cl100k_base 10,245,
real 13,439, headroom 2,945, the FFI floor 60: **byte-equal to `ground/measure-spec.txt`** (`diff` after trailing
blanks). The seed prints `STALE: the recorded count is for 4dced6767460e590 and this file is 24de425e6c28e69b`;
`pinned.hero:55` `13439`, `:64` `"24de425e6c28e69b"`. The cut parts: the sentence 88 and 88 (its file is the
sentence's 346 bytes plus a newline), the room's 90 and 91, lines 427-428 371 and 373: the same. The `spec` suite, `heroes-next run tests/harness/main.hero -- heroes-next spec`:
**`spec: 36 passed, 0 failed`**, the clerk's line (`diff` of the count lines against `ground/suite-spec.txt`: equal).

**The blind key, `sh blind/key.sh heroes-next <out>` from each copy's root, three runs of three per cell.**
- **This Mac**: 72 outputs, **0 differ** from the clerk's `key-mac/` after `key.sh`'s own normalisation (pid, addresses,
  `(N)`) (`diff` per file).
- **Linux arm64**: every one of the 24 cells one distinct output of three, and **every verdict and every refusal's
  words the same as this Mac's** (the first run of each cell `diff`ed against the Mac's, ASan's warning lines and the
  shell's `Aborted` set aside: 0 of 24 differ). `h1`, `h3`, `h7`, `n1`, `n2` run; `h2`, `h4`, `h6` STANDING; `h5`
  SWITCHED; `h8` ONSTACK; `n3` REENTERED (2855 on Linux, as its body says); `n4` the thread's. **So `h7`'s key holds on
  Linux arm64 at the base, plain and `--sanitize`, and no program of t1 or t2 splits between the graded platforms.**
  Owed again on the run's day, under that day's build, as the registration says (and under `main`'s runtime, **C1**).

**The probes, `sh ground/probes.sh heroes-next <out>`.**
- **This Mac**: `escape_carved` REENTERED plain and `--sanitize`; `escape_heap` `escaped: -1 then 42` both; `mj_carved`
  plain its lines, `--sanitize` **CALLED** (*"mjcarved.run was called from inside a signal handler"*): the clerk's
  table, every cell.
- **Linux arm64**: `escape_carved` **REENTERED plain and `--sanitize`** (2855 holds on Linux); `escape_heap` runs both;
  **`mj_carved` runs plain and `--sanitize`** (`body holds 7`): 2852 is Darwin's alone, as its body says.

**The negatives of § 3.4, re-run wider.** *"33 install none ... none in the handler sentence's words"*: my census under
`sh` (`262-critic-scratch/census2.sh`) follows every `#include "..."` a named header makes, looks for `sigaction`,
`signal(`, `sigset(`, `bsd_signal`, `sigvec` in the whole chain and for `hero_signal_watch` or `hero_net_catch_stop` in
the `.hero`: **still 33, still 2 aborting, both the room's**; one of the 33 names `hero_os.h`
(`fixedbugs-coroutines-on-a-thread-spawn-started-each-give-their-own-room-back`, for `hero_thread_spawn`), which
installs nothing. And the converse the brief infers (*"the handler sentence's aborts are reached only by programs with
a handler"*), asked of **every** golden holding a claim rather than of names holding *coroutine*: all 84 refusal
goldens install a handler in their header chain (**C5**'s 85th is the `check` outlier). The inference holds for the
tree's goldens. (A first pass of mine in `zsh` read 81 of 85 as installing none, because `zsh` does not split an
unquoted variable; discarded and re-run under `sh`.)

**The narrowed `run` cases** (`probes.sh`'s second half, the `run` form with its third word, each case plain and
`--sanitize` and held to its one `.expected`): **this Mac**, the twelve count lines `diff`-equal to the clerk's
`ground/run-cited.txt` (`grep -c '0 failed'`: 24); **Linux arm64**, the same twelve count lines, `diff`-equal to the
clerk's Mac file, 24 `0 failed`: every row of § 3.4's first table holds on Linux arm64 at the base (2707 4, 2706 2,
2848 6, 2705 1, the thread 1, 2702's `setcontext` 2 and 1, swapped away 6, 2522's `siglongjmp` 2 and inline 1, 2494
1, the own-stack coroutine 1). And **the route case on Linux arm64**: `run (only
fixedbugs-2679-the-runtime-says-which-route): 1 passed, 0 failed`, so `route 3` under Debian's glibc 2.41 (`L-2679.txt`).

**My programs on Linux arm64** (a second container, `linux-run2.sh`, three runs each): `other` prints `left on the
thread: 1`, `answer ran for 7`, `result 70`, exit 0, plain and `--sanitize`; `same` aborts in STANDING's words, exit
134, both; `watched` prints `watch 0`, `answer ran for 7`, `result 70`, `counted 1`, both. The same as this Mac.

**What the Linux arm64 image holds of the ffi-pragmatist's libraries** (`find /usr/include /usr/local/include`, `find /
-xdev -name '<lib>*.so*'`, `dpkg -l`): **Boost absent** (no `boost/version.hpp`, no `libboost_context`,
`_coroutine`, `_fiber`); **OpenSSL present**, `libssl-dev` 3.5.7 (`openssl/async.h`, `libcrypto.so.3`), not 3.5.9;
**libuv's runtime alone**, `libuv1t64` 1.50.0 (`libuv.so.1.0.0`) and **no `uv.h`**, so nothing binds it there without
a download; **curl present**, `libcurl4-openssl-dev` 8.14.1 (`curl/curl.h`, `libcurl.so.4`); no GNU Pth (`pth.h`
absent; `libnpth0t64` is GnuPG's nPth, another library), no libcoro.

## 5. Which seat was handed what it cannot check, and what a seat needs and was not handed

**C20. The ffi-pragmatist builds the signal stack trick from a recollection the historian is sourcing at the same
time.** `ffi-pragmatist.md` item 4 describes it as *"saving a buffer inside the handler and leaving it, then jumping
back into that frame"*; the seat has no web, and the historian (`historian.md` item 6) sources it in parallel. The
runtime's answer turns on two details the recollection leaves open: **whether the handler returns** after its save
(seen to end, an entry popped) **or leaves by a jump**, and **whether the library restores the previous alternate
stack** after the creation (rule 4 reads *"the alternate stack registered now"*; rule 6 re-enters on *"a jump that
lands on the alternate stack from outside it"*; and every plain-built thread already has the runtime's own, **C12**).
**Repair**: the ffi-pragmatist builds the four variants (returns or jumps; restores or leaves the alternate stack), and
names which one the historian's page describes once it is read. Name also libcoro's `CORO_SJLJ` (Marc Lehmann's,
used by Perl's Coro, as I recall it: **unverified**, the historian's), a second library on the trick.

**C21. The ffi-pragmatist's Linux column may not exist without a download.** Its seven libraries are on this Mac:
`/opt/homebrew/Cellar/boost/1.92.0_1` (`BOOST_LIB_VERSION "1_92"`; `libboost_context`, `_coroutine`, `_fiber` under
`/opt/homebrew/lib`), `openssl@3/3.5.9` and `openssl@4/4.1.0` (**`/opt/homebrew/lib/libcrypto.dylib` links to
`openssl@4/4.1.0`**, whose `nm -u` imports the same five: `__longjmp __setjmp _getcontext _makecontext _setcontext`),
`libuv/1.53.0` (`ls -d`). Whether the Linux arm64 image holds their headers and libraries the brief does not say, and
the seat may not download (*"no download (use what is on this disk)"*), nor `apt-get` inside the container (a
download). **Measured (§ 4)**: the image holds OpenSSL 3.5.7 and curl 8.14.1 with their headers, libuv 1.50.0 without
`uv.h`, and no Boost: **Boost.Context's and libuv's Linux cells cannot be built** under the seat's rules, and
OpenSSL's is 3.5.7, not the Mac's 3.5.9 or 4.1.0. **Repair**: the brief says so, and that those cells are **unrun**,
never inferred from Darwin's; the hand-written shapes (items 3 to 5, 7) need no library and run on both.

**C22. Facts the seats need and were not handed** (each found above): `main` moved and 200a changed the callback door
(**C1**: all builders); Darwin x86-64 on route A (**C2**: compiler-engineer, warden); x1's second condition answered
(**C3**: warden, compiler-engineer); 2705 and the own-stack coroutine falsify x1 at the base (**C4**: warden); panel
258's jump refusal and guard D's words (**C7**: all); the seventh guard defect, drafted (**C8**: all); 2831 (**C9**:
compiler-engineer); design.md's coroutine rows (**C10**: warden); the runtime's own handler and alternate stack
(**C12**: compiler-engineer, ffi-pragmatist); the spec's own conditional aborts (**C6**: warden, historian).

**C23. The historian's list misses the two closest precedents.** Both are my recollection, **unverified**, each a
question for the historian: (i) **JNI**, whose specification has native code on a thread the VM did not start call
`AttachCurrentThread` before calling Java, and whose HotSpot documentation describes *signal chaining* (`libjsig`) for
a VM that installs its own handlers beside a native library's: the precedent for `n4`'s thread and for **C12** (a
runtime with handlers of its own); (ii) **Lua**, whose manual says a coroutine cannot yield across a C call that does
not allow it and whose error reads *"attempt to yield across a C-call boundary"*: a language document wording a switch
refused across C frames, the nearest thing to (a)'s *coroutine switch* sentence. Python's `signal` module (*"a Python
signal handler does not get executed inside the low-level (C) signal handler"*, as recalled) is the remedy's own
precedent, which `rebel/library/0.1.0/files/os.ledger:454` already cites by analogy. **Repair**: add the three to
`historian.md` as recollections.

## 6. Routes, shapes and readings nobody listed

**C24. A fourth reading: the thread.** `now`'s sentence has no *"on its thread"*: *"A callback called after C left a
handler of C's ... may abort"*. The runtime's entries are per thread (`HeroSignalStack` is `_Thread_local`,
`sigguard.c:431`, reached through the thread key), so a callback on thread B after C left a handler by assembly on
thread A **runs**. **Run, this Mac** (`docs/panel/262-reports/programs/critic/`: `tl.h` is `h2.h`'s inline jump split
into `tl_leave()` and `tl_call(cb)`; `other.hero` calls `tl_leave` inside a `spawn`ed function, waits, then has C call
`answer` on `main`; `same.hero` does both on `main`): **`other` prints `left on the thread: 1`, `answer ran for 7`,
`result 70`, exit 0, 3 of 3 plain and 3 of 3 `--sanitize`; `same` aborts in STANDING's words, exit 134, 3 of 3 each**.
Panel 257 saw the same on Linux (`b1`, `b2`: *"another thread's runs"*, `docs/panel/257-*.md:58`); **Linux arm64 at this
base, the same**: `other` runs, `same` STANDING, 3 of 3 each, plain and `--sanitize` (§ 4). A reader of `now`
predicts *may abort* for it; x1 says *"on its thread"* and `now` does not. Neither blind run nor 2848 shows this,
because **no program of either run has two threads and a handler**. Beside it, two more readings no run shows: (i)
**the pronoun**: in *"by a jump or a `setcontext` onto a stack once given to `makecontext`, or by a jump that lands
within it"*, the nearest antecedent of *it* is *a stack*, not *a handler*, so a reader may read the fifth exit as *a
jump that lands within a `makecontext` stack* (and then class `h6` by line 427 alone); (ii) **the remedy's own
handlers**: the sentence after says to count signals with `hero_signal_watch`, whose handler is the runtime's C; a
reader may take it for *"a handler of C's"* and predict that a callback after a counted signal may abort. It runs:
`programs/critic/watched.hero` with `rw.h` (`hero_signal_watch(SIGUSR1)`, then C raises SIGUSR1 and calls `answer`)
prints `watch 0`, `answer ran for 7`, `result 70`, `counted 1`, exit 0, 3 of 3 plain and `--sanitize`, on this Mac and
in Linux arm64. And *may* read as **a platform split** (P), which is
how panel 257 came to the word (`:56-62`, `:106-109`), for which the labels have only O. **Repair**: add `other.hero` to t1
(its Heroes code differs from the task's shared `main`, so the task's sentence *"Every program's Heroes code is the
same"* must change), keyed on both platforms: under `now`'s letter it sits **inside a *may*** (the first exit) and
runs, so it is also a second D2 program (**C15**); one that counts a signal with `hero_signal_watch` and then has C call
the callback; labels T (thread), I (the pronoun), P (platform).

**C25. Shapes nobody listed** (beyond § 5's), each **unbuilt by me**, each with the seat that owns it:
- **A watchdog**: a `SIGALRM` handler of C's that `siglongjmp`s out of whatever it interrupted, the interrupted code a
  Heroes loop that allocates (**C7**): the outcome is where the signal lands, invisible to a reader. Compiler-engineer,
  both platforms, many runs (a rate, not a verdict). A real library shape: libcurl's synchronous resolver timeout
  (`alarm` and `siglongjmp` around `getaddrinfo`, as I recall it, **unverified**), the ffi-pragmatist's: curl is in the Linux arm64
  image (`libcurl4-openssl-dev` 8.14.1, § 4), none under `/opt/homebrew/Cellar` (`ls`).
- **A handler installed by a raw system call** that calls the function, on **Linux x86-64** with a restorer of its own:
  the delivery's address learned is glibc's `__restore_rt`, the restorer is the program's, so neither the exact test
  nor the walk should find it, and guard D alone stands (**an inference** from `sigguard.c:29-37` and `:61-66`, unrun;
  the `run` goldens of 2522's raw-system-call rows, a restorer of their own on Linux, `platform_skips.hero:129`, are
  allocator-and-lend shapes). A text that says *"aborts"* for a call from inside a handler (d3's stricter runtime,
  (b) without *may*) is false there. Compiler-engineer, the x86-64 image (recorded, not graded).
- **A handler left by `_Unwind_ForcedUnwind` from C**, the residue *"a C++ exception or `_Unwind_*`"*, and L's
  falsifier (**C13**); on Darwin `_Unwind_*` lives in libSystem, so *"a jump no function of the C library makes"*
  is itself ambiguous there. Compiler-engineer.
- **POSIX's own callbacks on threads the C library makes**: `timer_create` with `SIGEV_THREAD`, `mq_notify`, glibc's
  `getaddrinfo_a`: a binding author who never wrote `pthread_create` meets the thread's refusal (q8's *should a text
  name a callback on a thread C made*). Ffi-pragmatist.
- **Systems a reader calls Linux that the runtime was never built on**: musl (no `makecontext` in libc; `libucontext`),
  FreeBSD (*"every POSIX system but Darwin"*, `sigjump.c:14`): what a run-promise or *"may"* means there is unrun, and
  (d2) must say. Warden, as a question.

**C26. Routes nobody listed**, each a question with what would have to be true:
- **(d6) Merge, not append**: one sentence on *how C leaving a callback's frames or a handler costs the program*,
  joining the room's sentence (which already says *"by a jump, an exception or a coroutine it never resumes"*) and the
  handler sentence, by `.claude/rules/spec-shape.md`'s *"merging beats appending"*. True if one class of exits serves
  both aborts; the room's is every frame left without returning, the handler's a subset of switches, so it needs the
  two classes said once each inside one sentence.
- **(d7) Line 427's home**: the rule is about what a callback is (*"never a result or a signal handler, nor called by
  one"*); extend that clause with its consequence (*"... nor called while one is running on its thread, which C's
  leaving by a way the runtime does not follow leaves running for good: such a call aborts, saying ..."*) and drop
  the list. True if the instrument reads the claim there (`SENTENCE_OPENS` moves) and line 427's `atexit` example
  survives.
- **(d8) Abort at the exit, not at the next callback**, as panel 258 already does for a jump out of the allocator
  (**C7**): for the exits the runtime **sees** (a `swapcontext` away, a `setcontext` onto a coroutine's stack, a jump
  into a live coroutine), stop the program there, so the sentence says *aborts* with no *may* and no *later*. True only
  where the exit is seen; it refuses programs that never call a callback after (`h5`'s twin with no callback), so it
  is a widening of refusals the warden prices and the engineer counts.
- **(d9) A told route**: the spec says *may abort where this runtime cannot follow C's exits, which
  `hero_signal_jump_route()` and `heroes doctor` name on a machine*; the accessor exists
  (`fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`, `route 3`). True if `doctor` prints it (unread) and the
  spec may name a machine-dependent fact through a command, which § 13's *"platform layer"* sentence does not yet do.

**C27. *"Nothing in ... `site/`"* hides that the site serves the spec and a landing publishes it.** q13 says the
landing owes nothing in `site/`. But `.github/workflows/deploy-site.yml`'s `paths:` holds `spec/heroes-spec.md`,
`tests/harness/suite_spec.hero` and `runtime/**`; `site/src/lib/markdown.ts:1-10` serves *"the one markdown document
this site serves verbatim: the specification"*, byte for byte, asserted at build time; and `claims.ts:895-915` holds
the README's *"fewer than 600 lines"* to the spec's line count (516 at the base, 527 with 261's text). So **the landing
owes the site's build, and its push publishes the site** (CLAUDE.md § Hard stops: asked for, with the count of site
commits), as would any lane's `runtime/**` repair (E3, G). **Repair**: q13 names the site's build and the publication.

**C28. The third abort beside the sentence may not be one on a C coroutine's stack.** q9 names § 9's *"running out of
stack aborts"* as an abort a coroutine program can meet. The runtime's stack guard is a fault on a guard page
(`stack.c`, `hero_stack_foreign`, `:856-866`, a fault off the thread's stack passed to the handler before it); a
coroutine stack from `malloc` (co.h's, sw.h's, every `n*` and `h8`) has **no guard page**, so a callback that recurses
past its end writes into the heap and nothing faults: whether that aborts or corrupts memory is **unrun** (no golden by
name holds it: `ls tests/golden/run | grep -i -E 'overflow|exhaust|deep'` joined with *coroutine|context|stack*: none;
no issue by `gh issue list --search`). 259's `t2-h-2` raised exactly this as its reason for `NOT PROMISED`, so it may
be no misreading. A robustness question (design.md §1.12) beside the sentence, not inside it. **Repair**: § 5 adds it
for the compiler-engineer (a callback recursing past a `malloc` stack and past an `mmap` stack with a guard page, both
platforms), and the labels do not grade S as a misreading until it is run.

## What the coordinator must repair before any seat starts

Most important first. Each names the finding it rests on; none is a verdict on a candidate.

1. **Stop presenting x1, and every run-promise, as waiting on conditions** (**C4**, **C8**, **C2**, **C3**). Both of
   R2's conditions are met (2706 closed; the CI's two Linux legs passed the route case, **C3**), and x1 is still false
   as written: at the base on route C by `fixedbugs-2705` and the own-stack coroutine (ONSTACK), on route A by the
   drafted seventh defect (`b_0`, `cs_0`, correct programs, UNFOLLOWED) and by 2706's two programs (HELD, ONSTACK); and
   route A is **Darwin x86-64** as well as glibc's other architectures. Hand the warden and both builders these
   falsifiers, state q4's `setcontext` split as route C's alone, and add the drafted defect to § 3.5.
2. **Add panel 258's refusal of a jump out of the runtime's allocator to q9 and § 5** (**C7**): it stops a program
   at a handler's `siglongjmp`, in words no golden, no arm of the instrument and no spec sentence holds, and it
   falsifies any promise of a run after `siglongjmp` for an asynchronous handler (a watchdog). Every run-promise
   candidate must say what it promises there.
3. **Repair the registration before any session can be drawn** (**C13**, **C15**, **C24**, **C14**): label every wrong
   verdict, a wrong `RUNS` included, or L is unmeasurable by construction; fix D2's program set before the run, the
   same under every arm, with more than `h7` in it (`other.hero`, run; the `2707` and `2702` runs; route C only);
   add `other.hero` (the thread, the fourth reading, run on this Mac and Linux arm64) and labels T, I, P, R, S; allow several labels
   per answer and try them on 259's archived answers, free, two graders apart.
4. **Re-anchor the ground on `main`** (**C1**): the lane's head is `6079623b6` (a `main` commit), `main` is 15
   commits past the base with step 200a changing `callback.c`, the door every thunk crosses, and 2854 repaired; a seat's
   decisive rows run on the base **and** on `main`'s runtime, as panel 257 learned.
5. **Name what the landing owes outside the spec** (**C27**, **C5**): the site's build and a publication (the site
   serves the spec verbatim; `deploy-site.yml` paths hold the spec, `suite_spec.hero` and `runtime/**`); 84 refusal
   goldens, not 85, the 85th a `check` case a `grep -rl` would rewrite; and the net's own tests, which run
   `spec_refusals.hero`'s test against the real spec (`spec_refusals.hero:213-222`).
6. **Hand the seats the facts they were not handed** (**C22**): 2831 (**C9**); the runtime's own SIGSEGV handler and
   alternate stack in every plain build (**C12**); design.md's coroutine rows (**C10**); the spec's own conditional
   aborts (**C6**); § 3.8 corrected (**C11**); the § 9 abort on a C coroutine's stack, unrun (**C28**).
7. **The registration's smaller faults** (**C16** to **C19**): the room's sentence on `n2`, `n3`; `h6` against `h4`;
   `n3`'s condition; the key's build when plain and `--sanitize` split (`n1` is 2852's shape on Darwin); `prelude.h`
   in `task-t1.md`; `h2.h`'s comment quoting `now`; `inputs.sha256`'s first line, which `shasum -c` cannot read.
8. **The seats' instructions** (**C20**, **C21**, **C23**, **C25**, **C26**): the signal stack trick in four variants;
   which libraries the Linux image holds (§ 4); JNI, Lua and Python's `signal` for the historian; the shapes of **C25**
   to their owners; the routes (d6) to (d9) beside (d1) to (d5) as questions.
9. **Small framing corrections**: the goldens-per-family numbers count `.hero` files, not `ls | grep -c` (§ 1's table);
   `sigjump.c`'s defines are at 130-132 and 134-145; 192a's `callback.c` is `+9 −177` (net −168, not −186); § 3.4
   quotes *"the room large values take off the stack is full ... while 2047 callbacks C called have not returned"* as
   *"panel 259's sentence"*: those are the runtime's words (the spec's says *"saying how many callbacks have not
   returned"*), and the second such golden reads 2046.
