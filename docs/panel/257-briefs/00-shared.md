# Panel 257, shared brief: the sentence the spec owes the refusals of a jump out of a handler

Written 2026-11-23 (the first commands at 00:37, the last at 00:50 by this Mac's clock, `date`) by the sitting's
coordinator. Every number, path and count below was produced by a command run while this brief was written,
and the command is named beside it; a fact carried without running it says **carried** and from where.
Read this file whole, then your own brief. **A brief is the one document nobody was assigned to check**: the
completeness critic reads this one before any seat starts, and you are asked to re-run what bears on your
verdict and say which fact was wrong.

## 1. The tree, the lane, and what this sitting is

- **Lane**: branch `lane-p257`, worktree `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p257`,
  at `828567a30a4018e74605378c3572249b2bed6d85` (`git log -1 --format=%H`), the head of `rebel-round` the
  day panel 255 was ratified. **`rebel-round` has moved since**: its head read `22710bc62` when the coordinator resumed the sitting, before 11:08 by `date` (`git log
  --oneline -1 rebel-round`), `828567a30` an ancestor of it (`git merge-base --is-ancestor`), and `git diff --stat
  828567a30 rebel-round -- spec seed runtime/parts runtime/runtime.c runtime/heroes_runtime.h tests/golden/run
  .claude/agents .claude/skills | wc -l` printed **0**: what moved is the UI runtime (`runtime/hero_ui.h`,
  `runtime/platform/hero_ui*`), panel 256's blind-seat report and the lanes' merges, nothing the sentence or
  the builds read. The sitting stays at `828567a30`; the coordinator merges the lane. Its only change, while the briefs were written: the untracked folder
  `docs/panel/257-briefs/` and its reports folder (`git status --short`). **The tree is frozen** from the
  moment the briefs go out until the synthesis: nothing under `spec/`, `runtime/`, `selfhost/` or `tests/`
  moves. Every seat's copy is `git worktree add --detach <its own directory> 828567a30` (below, § 2).
- **Defects and issues** (`gh issue view --repo heroes-lang/heroes`, read only, 00:52): 2701, panel 255's ratification,
  CLOSED; **2702**, *"A Heroes function runs inside a signal handler that switches contexts ..."*, OPEN, the context
  class R3 files.
- **The question.** The author answered *1a* and *2a* to panel 255's ratification on 2026-11-23 (issue 2701);
  the words below are **the recording session's *"meant as"* gloss of those two letters**, under
  `## Author's verdict` of `docs/panel/255-*.md`, not a quotation of the author: *"the words of the refusals R4
  names are owed to the spec, a sentence written in a small sitting with the spec-warden"*. **Write ONE sentence (or, if the
  sitting finds it cannot be one, the fewest) of `spec/heroes-spec.md` that tells a reader of the spec which
  correct C programs a Heroes program's jump out of a handler refuses, and what the program is told; true on
  both platforms** (this Mac, Darwin arm64, and Linux, which the CI runs as `ubuntu-26.04` and
  `ubuntu-26.04-arm`: `grep -n -E 'ubuntu-26|macos-26' .github/workflows/ci.yml`, carried for the names). It
  extends the rule on spec lines 427 and 428, § 13.
- **The lane**: the author's *"a small sitting with the spec-warden"* is followed by seating the warden first and
  smallest; the **full panel** is the skill's rule for a spec token and is the coordinator's reading, which the
  sitting says it follows (the critic's finding B16): the proposal is a spec token (`.claude/skills/panel/SKILL.md` §
  Two lanes: *"anything the spec must state"*). Seated: `spec-warden`, `compiler-engineer`,
  `ffi-pragmatist`, and the completeness critic on the briefs and the reports. **Not seated, and what each
  gives up**: the **historian** (no precedent is claimed for the sentence; `records.md` § And the record says
  whose idea it was writes a `## Prior art` only where the historian sat); the **llm-ergonomist** is
  **registered and NOT RUN** (`257-briefs/llm-ergonomist.md`, `257-briefs/blind/`): no paid run is authorized
  (the author's order for this sitting), so no blind reader has said which program a reader of the new
  sentence expects to run, and **the resolution is provisional on that reading**. A seat that thinks a paid
  run would change its verdict says so in its report, with the run's size, and the coordinator decides.
- **Where the sentence goes, and the test that holds the spec's tail** (the critic's B1 and B2, each by its
  command in `257-reports/critic-briefs.md`): the compiler's own test *"every signal refusal ends with spec
  section 13's own sentence, byte for byte (defect 2533)"* (`selfhost/emit/callback_thunk.hero:354`) holds the
  text from `No function of the program is a signal handler` to the end of line 428 equal to
  `HERO_SIGNAL_REMEDY` (`runtime/parts/sigguard.c:110-113`), the last words of every signal refusal, pinned by 22
  `run` goldens. **A sentence appended after `its last call` would be held to the runtime's words and printed at the
  end of every refusal; so every arm of this brief goes BEFORE `No function of the program is a signal handler`**,
  between `-> i32`. and that sentence (measured for one arm: the same vendored count, the test's result unchanged),
  and line 428 keeps its present end, which has no full stop. **That test is already RED at the frozen tree**,
  before any arm: `c0476dff3` put the mailbox clause (*"or a mailbox's `receive` that finds it empty"*) into the
  spec and the runtime does not print it. A seat that runs the compiler's own tests sees that red and must not
  read it as its arm's; **the runtime lane's landing owes `sigguard.c`'s constant and the 22 goldens**, whatever
  text wins, and the sitting says so.
- **What is already ruled and is not this sitting's** (`docs/panel/255-*.md`, read its R1, R3, R4 and
  `## Author's verdict`): R1, route C on both platforms (the decode: a jump of the `setjmp` family reads ONE
  word of its target and decodes the stack pointer; no buffer a save filled is read at any other moment);
  R2, the copied buffer runs and the refill stays refused; R3, a context switched inside a handler is
  **defect 2702's** repair (its design is Darwin's C4r carried to Linux); R4, **G refused on both platforms**,
  *"told in words true of each"*. **The runtime lane** `lane-p255-impl` builds R1 to R4's refusals in
  parallel and will tell the final words: at 00:47 its worktree held four modified files
  (`runtime/parts/rebind.c`, `sigguard.c`, `sigjump.c`, `runtime/runtime.c`) and one new
  (`runtime/parts/rebind_decode.c`), uncommitted, and its HEAD was `828567a30` (`git -C .../lane-p255-impl
  status --short`, `log --oneline -1`). **Nothing of it is a premise**: this sitting's builds are panel 255's
  routes (§ 4), and the sentence is **checked again against the lane's final words after it lands** (owed,
  named in the sitting).

## 2. Rules for every seat

- **Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p257-scratch/<seat>/`, made by
  the coordinator: `git worktree add --detach .../p257-scratch/<seat> 828567a30`. **Build your own
  compiler inside it from the seed** (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, 44 s real
  on this Mac at load 43 to 71, read by `time` while this was written; indicative, never a number to quote),
  and stage 2 (`./heroes build selfhost/main.hero -o heroes-next`) where you need `measure` or the harness.
  `rm -rf build` after any copy. **Never use the trunk's `./heroes` or another seat's, and never read, build
  or run inside another seat's directory.** The panel-255 evidence is tracked:
  `docs/panel/255-reports/evidence.tar.xz` (4,510 files, `tar -tJf ... | wc -l`); **extract it into your own
  copy** (`tar -xJf`, into an empty folder of your own), never from the coordinator's.
- **Your report**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p257/docs/panel/257-reports/<seat>.md`,
  **written as you go**, so a killed seat leaves what it had. It ends with the verdict structure of your agent
  file (`.claude/agents/<seat>.md` in your copy), then one line per question of your brief: the answer, and
  the one measurement that would change it.
- **Machine and clocks**: the Mac is shared with other lanes (`uptime` read load averages 43.77, 58.49,
  72.12 at 00:45). **A duration you measure is indicative and goes in no verdict**; read `real` against
  `user` plus `sys`. A program that may not end is built with `heroes build` and its BINARY run under
  `timeout`, **both streams** bounded in bytes (`2>&1 | head -c 100000 > o.txt`, the exit read from `PIPESTATUS`;
  panel 255's model script bounds stdout only, the critic's B21), never `timeout heroes run`; **after a timeout
  look for the built binary** (`pgrep -f build/`) and stop it (`.claude/rules/verification.md` § A run that may
  not end). Every container you start is yours to remove (`docker rm -f`).
- **No paid run.** No `claude -p` session, no cloud run, no API call of your own. The one exception is the
  spec-warden's **token counts** (its brief): `heroes measure spec/heroes-spec.md --refresh` is one request to
  `https://api.anthropic.com/v1/messages/count_tokens` (`selfhost/cli/refresh.hero:37`): **the key counts tokens and does nothing else**, by the author's instruction of
  2026-09-09 (`.claude/hooks/guard_bash.py:1006-1013`, read by the critic: *"Counting is a read. Inference is
  spending somebody's money"*). The warden has **at most four**, each a count, and prints the key's length,
  never its value. Whether the account is billed for a count request is not settled by any file read here.
- **No credential is printed**: not `gh auth token`, not `.env`'s content, not an environment dump; `gh auth
  status` masks, and `.claude/hooks/guard_bash.py` refuses the other.
- **Nothing outward**: no push, no merge into `rebel-round`, no GitHub write, no file outside the repository's
  root, no `git add -A`, `.`, `-u` or `git stash`. You commit nothing; the coordinator commits the sitting by
  pathspec.
- **Every number in your report is a command's, with the command beside it**; a negative sentence (*the
  runtime does not*, *no program*) is run or goes out as a question naming what was searched.

## 3. The facts, each by its command

**The rule the sentence extends** (`sed -n 427,428p spec/heroes-spec.md`): *"A callback is a **parameter**,
never a result or a signal handler, nor called by one; its parameters follow the same rule and `()` is
`void`: `atexit(f: (function() -> ())) -> i32`. No function of the program is a signal handler: after
`hero_net_catch_stop() -> i64` (`hero_net.h`), Ctrl-C, SIGTERM and `hero_net_ask_stop() -> ()` only make
`hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer, or a mailbox's `receive` that finds
it empty, answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts any other signal, and
`hero_signal_take(sig: i32) -> i64` gives the count since its last call"* (line 428 ends with no full stop,
`sed -n 428p ... | tail -c 5 | od -c`). **No other line of the spec says anything of the mechanism**:
`grep -n -i -E 'setjmp|longjmp|jmp_buf|getcontext|swapcontext|signal handler|handler|fingerprint|save point'
spec/heroes-spec.md` prints lines 427 and 428 only. **The word the sentence must reuse**: § 6, line 202,
*"An abort ends the program at once, saying why; no `T?` carries one."* (`sed -n 202p`); `grep -o -i 'this
runtime\|the runtime' spec/heroes-spec.md | sort | uniq -c` prints `1 the runtime`: the document speaks of
*the program* aborting, not of a runtime. **Two rules of the document's shape that bind the sentence**
(`.claude/rules/spec-shape.md`): *no closed list of aborts* (panel 087's warden *"vetoed a closed list and
falsified it with a running program"*) and *merging beats appending*.

**The budget, before** (`./heroes-next measure spec/heroes-spec.md` from the lane, stage 2 built in it at
00:45): claude-legacy **9,585**, cl100k_base **9,761**, vendored maximum **9,761**, **real 12,809**
(`claude-opus-5`, the pin of 2026-11-22), headroom **3,575** against the **16,384** ceiling, the FFI floor
mortgaging 60 of it (12,869 measured against the ceiling). The file: 516 lines, 33,272 bytes (`wc -l`,
`wc -c`), sha256 `15804f3fb52a62c208bc59ef...` (`shasum -a 256`), the same prefix panel 256 recorded.
**The rule that prices a change** (`691531908`, 2026-11-22, `git show -s`: *"Contract: the most robust solution
always wins, a new form and the spec's size included"*): the cost is **measured and written, never a veto**;
the real count, never the vendored one, is the price (`spec-shape.md` § How a change to the document is made).

**The arms** (§ 5, **second issue**, after the critic's first reading): `257-briefs/arms/arm-f1.diff`, `arm-f1b.diff`,
`arm-f2.diff`, `arm-f3.diff`, each a diff of `spec/heroes-spec.md` that applies (`patch --dry-run -p1 < arm-fN.diff`,
four times, each printing `patching file`) and puts the sentence **before** `No function of the program is a signal
handler`; their **vendored claude-legacy and cl100k_base counts** by `heroes-next measure` on each applied copy
(the maximum is the larger): f1 9,678 and **9,854** (+93 against 9,761), f1b 9,670 and **9,846** (+85), f2 9,672
and **9,848** (+87), f3 9,629 and **9,805** (+44). **Their real counts are the warden's** (its brief). The first
issue's four arms, `arms/v1/arm-e*.diff`, **appended** to the end of line 428 and are kept as the critic read
them; the critic measured clauses of them false (§ 5.2) and that is why they were reissued.

**Today's words, the four kinds a refusal prints** (`tests/golden/run/fixedbugs-2522-*.expected`; the kind
letters are panel 255's ffi-pragmatist's own, `docs/panel/255-reports/evidence.tar.xz`,
`ffi-pragmatist/p255/r2/run.sh` lines 5 to 7): **S** *"... had not been seen to end: either C called it from
inside that handler, or C left the handler in a way this runtime does not follow (an inline
`__builtin_longjmp`, a C++ exception, a jump inside a system library). A handler not seen to end is held to be
running, where neither this runtime's memory nor the C library's is safe to use."*; **U** *"... C, inside a
signal handler, jumped to a buffer this runtime holds no record of as it now stands (copied with `memcpy`,
refilled where the runtime does not look, or saved before more save points than it keeps), so whether the
jump left the handler is not known"*; **K** *"C called it while handling a signal"*; **D** *"ran as a signal
handler"* (K and D read from that script's `sed`, the full text is the runtime's). Every one ends with
`HERO_SIGNAL_REMEDY`, the spec's tail **as it stood before `c0476dff3`** (the mailbox clause is missing, above). **R2 of panel 255 moves the refill
case from U to S and lets the copied buffer run**; **R4 keeps G refused, in S words**.

**The cases that exist**: `ls tests/golden/run | grep '^fixedbugs-2522' | grep -c '\.hero$'` prints **25**
programs (twelve `a-callback-after-*` that RUN, one `a-correct-inline-jump-out-of-a-handler-is-refused-in-words-never-false`,
the copied-buffer and refill pair, the `dlsym`-found jump, the handlers installed by raw system calls). **No
case and no shape of panel 255's throws a C++ exception**:
`grep -l -E 'throw|__cxa|_Unwind' tests/golden/run/fixedbugs-2522*.h ...2558*.h ...2571*.h` prints nothing,
and over the extracted evidence prints 17 files, all named `sigjump.c`, 8 distinct by `md5` (the critic's
`grep -rl -E 'throw|__cxa|_Unwind' <evidence> | wc -l`), where the word is in a comment of the runtime's own source. So **the S words' *C++ exception* is a statement of the words, never measured**; `g++` is
clang on both platforms (`g++ --version` on the Mac: Apple clang 21.0.0; `readlink -f $(which g++)` in the container:
`/usr/lib/llvm-22/bin/clang`) and **there is no GCC** (`gcc` is not found in the container), so a C++ program has
one front end here, and *"clang refuses `__builtin_longjmp` on arm64"* is the only compiler this sitting runs.

**The inline jump is not one spelling on both platforms** (`sed -n 1,16p` and `grep -n arm64` of
`tests/golden/run/fixedbugs-2522-an-inline-jump-out-of-a-handler.h`): *"clang refuses `__builtin_longjmp` on
arm64 (*not supported for the current target*, Linux arm64 at panel 251), so there the same jump is written by
hand"*, in assembly. So **an arm names `__builtin_longjmp` as the example of a jump no C library function makes
and it does not compile on arm64 clang**; the golden's own description of the class is *"a jump no function of
the C library makes, so this runtime sees no event for it"*. The wording of that class is a finding to repair.

**What a refill is, as the goldens' own comments say it** (`fixedbugs-2522-a-handler-of-cs-jumps-and-its-deep-callback.h`
and the `...refills-by-memcpy...` golden's header comment): the handler copies **its own** save into a buffer
the program filled earlier at a frame that has since returned, and jumps to that one: **the jump lands inside
the handler**, so the callback that follows is called by the handler, which is spec line 427's rule and not a
cost. Under R1's decode (a jump reads the stack pointer of its target) the sitting's R2 says this case *stays*
refused in the *"had not been seen to end"* words. **Whether any correct program that really LEFT the handler is refused because its buffer was refilled** was
the question the first issue left to the seats; **the critic measured it**: p10 (the HANDLER `memcpy`s a save the
main line made into a second buffer and `siglongjmp`s to the copy, so it leaves) **RUNS on both adopted builds**
(3 of 3 each) and ABORTS in the U words today on both (`critic-briefs.md` B22). The class the runtime acts on is
**a jump that lands within the handler** (p8), not a refill; the seats re-measure it.

**The seven programs of the registered blind reading, today** (`257-briefs/blind/programs/`, built by the
lane's stage 2 and each run once under `timeout 20`, Mac, plain, the coordinator at 00:57; `answers.md` holds
the table): p1 `siglongjmp` static buffer RUNS, p2 heap buffer RUNS, p3 a whole copy ABORTS (words *"holds no
record of"*), p4 the golden's inline jump ABORTS (S words), p5 a handler that returns RUNS, p6 a handler that
calls the function itself ABORTS (S words), p7 a handler that `swapcontext`s away never resumed RUNS (exit 0). **p4's header comment and p6's ending were changed after the critic's B5 and B8** (p4's comment was the golden's own, project-internal; p6 now returns what the callback returned, so a run that is not refused prints `result 70`), and the coordinator re-ran p4, p6, p8, p9 and p10 on the changed files by `date` 11:08: p4, p6 and p8 ABORT in the S words, p9 RUNS, p10 ABORTS in the U words (Mac, plain, today).
They are a data point, not a seat's measurement: the seats build and run them again on their platform and
build.

**Where the mechanism lives, at the frozen tree** (`wc -l`): `runtime/parts/sigjump.c` 283 lines (Linux and
every POSIX but Darwin), `sigguard.c` 523, `siginstall.c` 362, `rebind.c` 727 (Darwin).

## 4. The two builds the sentence is checked against (panel 255's routes)

Panel 255 adopted, provisionally and by the author's reading, **Linux: *C, 2679 alone*** and **Darwin: C4r**.
Both are files in the tracked evidence, each applied over `runtime/parts/` of a copy:

| build | files, under the extracted archive | lines, md5 (`wc -l`, `md5 -q`) |
|---|---|---|
| **Linux** | `compiler-engineer/p255/r2/routes/C2679/sigjump.c` | 671, `ba8487af034ea2b7dd80413bab0f2884` |
| **Darwin** | `ffi-pragmatist/p255/r2/C4r/rebind.c` | 1,438, `2730f1617d4d1ff35d9e5074f209c2da` |
| **Darwin** | `ffi-pragmatist/p255/r2/C4r/sigguard.c` | 557, `3c603aff7998aa786c4d01970b7fe2cb` |

The route is built as panel 255's seats built it: **a copy of `runtime/` with those files replaced**, and a
program built by *your* stage 2 with **`HEROES_RUNTIME=<that copy>`** (`selfhost/main.hero:293` names the
variable; the ffi-pragmatist's own `r2/run.sh` is the model: each shape built plain and `--sanitize`, run
under `timeout 20`, its output through `head -c`, the refusal kind read from stderr). **Today** is the same
with the frozen `runtime/` untouched. **The ffi-pragmatist's report of panel 255 says `rebind.c` was 1,393,
then 1,403 lines at C4 and 1,438 at C4r** (`ffi-pragmatist-r2.md` lines 197, 237 and 575, `grep -n`): the archive's file is the
build, and a number that disagrees with it is the seat's to reconcile in its report.

**What the CI also runs, and the sentence's scope** (the critic's B12 and B13). The CI's third leg is **Windows**
x86-64 (`.github/workflows/ci.yml:373`, `windows-latest`), and in the frozen source the runtime has **no probe
there**: `runtime/parts/sigguard.c:506-521`, the `_WIN32` branch, says *"Windows: no probe and no walk (panel 247's
R10) ... whether UCRT's `raise` leaves a delivery's address to learn is the box's question, unrun"* and
`hero_callback_entered` is empty there; `sigjump.c:69` is `#if !defined(_WIN32) && !defined(__APPLE__)`. So on
Windows no callback is refused for being called by a handler, line 427's own rule included: **the sentence is
scoped to the platforms that have signals (Linux and Darwin), or it is knowingly Windows-false, and the warden
says which and prices the words.** Windows is unrun here (there is no Windows machine in this sitting). **And
the adopted Linux build was never run on the CI's C library**: the CI's Linux legs run Ubuntu 26.04, glibc 2.43,
and panel 255's Ubuntu runs are of C4, which was not adopted on Linux (`compiler-engineer/p255/r2/out/` holds
`C2679 C4 C4fail png png2 today ub-C4 x86-C4`, no `ub-C2679`): the engineer runs `C2679` through
`compiler-engineer/p255/r2/ubuntu-bins.sh` with `VARIANT=C2679` and `ubuntu-run.sh` (arm64 only), and puts
**the accessor's reading (`decoding: N`) beside every row**; a clause that holds only where the decode
verifies (route A refuses a jump through heap, thread-local or late-loaded buffers) says so.

**Platforms**: this Mac, Darwin arm64, Apple clang 21.0.0, macOS 27.2.1 (`clang --version`, `sw_vers
-productVersion`); Linux arm64 in the image `heroes-linux-arm64:latest` (`docker image ls`, ID `338a1044ac90`),
whose own `clang --version`, `ldd --version` and `uname -m` read Debian clang 22.1.8, glibc 2.41, aarch64.
**Linux x86-64 is unrun** here (carried: panel 255's `## What this sitting files and owes` leaves its rows to
the lane and the `ubuntu-26.04` leg), and **so is the CI's `macos-26`**; a claim that rests on either says so.
libpng 1.6.59 is on this Mac (`pkg-config --modversion libpng`); **the container has none on its
`pkg-config` path** (`docker run --rm heroes-linux-arm64:latest sh -c 'pkg-config --modversion libpng'` printed
*Package libpng was not found*), and has `clang++` and `python3` (`which`; its `g++` is clang). Panel 255's Linux scripts are
`compiler-engineer/p255/linux.sh` and `r2/leg2.sh` in the archive, the models for a container run.

## 5. The arms, and what separates them

### 5.1 The second issue (`arms/arm-f*.txt`; each goes before `No function of the program is a signal handler`)

**E0** is the status quo: no sentence, the runtime's words alone, as ruling B's were.

- **f1**: *"A handler of C's ends, for the program, where this runtime sees it return or sees `longjmp`,
  `siglongjmp` or `_longjmp` take the thread out of it; a handler left any other way (a jump no function of the
  C library makes, a switch of context never switched back) or jumped within is held running, and a callback
  called while one is held running aborts, saying the handler was not seen to end."* A **class** (*held
  running*) with its causes.
- **f1b**: f1 without *"a switch of context never switched back"*: **only what is true on both of panel 255's
  adopted builds**, since p7 splits (§ 5.2); the context clause waits for defect 2702.
- **f2**: *"A callback called after C left a handler by `longjmp`, `siglongjmp` or `_longjmp` runs, wherever the
  buffer lives; one called while a handler is held running aborts, saying the handler was not seen to end, and a
  handler is held running if C left it any other way (a jump no function of the C library makes, a switch of
  context never switched back) or jumped within it."* What runs first, then the abort.
- **f3**: *"A callback called while a handler of C's has not been seen to end, by return or by `longjmp`,
  `siglongjmp` or `_longjmp` out of it, aborts, saying so."* The bare rule, no cause named.

**These are drafts by the coordinator and every clause of each is a claim to be tested, not a premise.** They
were written after the critic measured the first issue (5.2): the class the runtime acts on is **a handler not seen
to end** (a jump that lands within it is one), not a list of causes. The questions they put: does *"held
running"* / *"not seen to end"* read as a rule a reader can apply to a program (the warden)? is *"a callback
called while ..."* true of a callback on **another thread**, or of one **called from a second handler** (the
engineer's item 4)? does *"saying so"* (f3) say enough? is *"a switch of context never switched back"* a
class a C programmer recognises (the ffi-pragmatist's items 3d and 7)?

### 5.2 What the critic measured of the first issue (`arms/v1/`; `critic-briefs.md` B22, B23, key table)

Adopted builds only (Darwin C4r, Linux C2679), `-O0`, plain, 3 of 3 each, **the critic's own runs; the seats
re-measure them, with `--sanitize` and their own shapes**:

| clause of the first issue | Darwin C4r | Linux C2679 | program |
|---|---|---|---|
| leaving by `longjmp`, `siglongjmp` or `_longjmp`, callbacks run, *whatever memory* | held | held | p1 static, p2 heap, p9 (40 live frames), p3 and p10 copied |
| an inline jump aborts the next callback | held | held | p4 |
| a context switched away and never back aborts | held | **false: RUNS** | p7 |
| *a jump to a buffer the handler refilled* aborts (e1, e1b) | **false: RUNS** | **false: RUNS** | p10 (the handler refills and leaves) |
| *to a buffer a save of that family filled* (e2): any other exit aborts | **false** | **false** | p3 and p10 |
| a handler that returns runs | held | held | p5 |
| a jump that lands **within** the handler, then the callback: ABORTS (S) | held | held | p8 |

**Today** differs: p3 and p10 ABORT in the U words on both platforms (R2 moves them); **p9 splits** (Mac RUNS, Linux
ABORTS in U: *"saved before more save points than it keeps"*, `HERO_SIG_JUMPS 32`), and both adopted builds
run it. **p7 RUNS today on both platforms and on Linux C2679**, and ABORTS (S) on Darwin C4r: *"G refused on both
platforms"* is the ratified rule, and **defect 2702 is what makes Linux meet it**.

## 6. The claims, the table every seat that builds fills

Each row is an atom of the arms; the cell is **runs / S / U / K / D / other (exit, first line)** on each
build, plain and `--sanitize`, three runs a cell, with the program and the command. **Today** is a column too.
`p1` to `p10` are in `257-briefs/blind/programs/` (p1 to p7 are the registered reading's; **p8, p9 and p10 are
the critic's, added for the seats**).

| id | the claim | programs |
|---|---|---|
| C1 | a handler left by `longjmp`, `siglongjmp` or `_longjmp` to a stack or static buffer: the callback runs | p1; the twelve `a-callback-after-*` goldens |
| C2 | the same to a buffer in the heap, thread-local storage, a mapped page, a library loaded after start, or one of **40 live saves**: runs | p2, p9; `heap`, `mmap`, `handler_heap`, `handler_mmap`, `plain_heap`, `plain_mmap`, `x_heap`, `x_tls`, `x_dlopen`, `jo_heap`, `jo_tls`, `jo_lib`, `pngdemo` (libpng 1.6.59, `good.png`) of `ffi-pragmatist/p255/r2/sh/` |
| C3 | a whole copy of a saved buffer, and a buffer the **handler** refills from a save the main line made, then leaves by: runs (R2) | p3, p10; `gcopy` |
| C4 | an inline jump: aborts (S); one spelling on x86-64, assembly on arm64 | p4; the golden `...a-correct-inline-jump-out-of-a-handler-...` |
| C5 | a C++ exception through the handler: aborts (S) | **none exists**; write one in `clang++` or say it cannot be made correct |
| C6 | **a jump that lands within the handler**, then a callback: aborts (S), a longjmp included | p8; the `jumps-within-itself` goldens; the `...refills-by-memcpy...` golden (its jump lands within) |
| C7 | a context switched away from inside a handler and never switched back (G): aborts (S) | p7; `g1_0`, `g1_1`, `g2`, `g3` (`ffi-pragmatist/p255/r2/sh/`), `g2_1` (`critic/p255/r3/fsh/`), Linux's `g_3_0`. **Expected: Darwin refuses, Linux C2679 runs p7** (defect 2702) |
| C8 | a handler left by a jump into a coroutine (`fp3`, `as_0`) | Darwin C4r refuses both in S words, *"a cost"* (`ffi-pragmatist-r2.md` lines 283, 284); Linux's build unmeasured |
| C9 | the words each build prints say *the handler was not seen to end*, and not *C is wrong* | the first stderr line of every refusal |
| C10 | **every C-row has one verdict on both builds**: the critic reads it across the two tables | a row that splits is named with the build that departs |
| C11 | a callback called **on another thread** while the first thread's handler is held running; one **called from a second handler**; a callback **never called again** after the inline jump (exit 0?) | the engineer's item 4 |
| C12 | **`exit()` inside a handler reaching an `atexit` function** is refused in the S words though the handler never left (line 427's *called by one*) | the 2494 golden `...a-heroes-atexit-function-a-c-handler-reaches-through-exit-is-refused` |

**A row where the two builds differ is a finding, not a failure** (panel 255's own words, the sitting's `##
Where the seats disagree`: *"the context rows split as they split today, Darwin refusing where today's Linux
runs a Heroes function inside a signal, until R3 lands on Linux"*). The sentence must be true of the language the
lanes will have made, and of the builds that exist; where those differ the report says which clause waits for
which defect.

## 7. What the coordinator does with the reports

Synthesis: **the most robust and complete sentence**, never the cheapest and never a compromise
(`CLAUDE.md` § 4); where robust and conservative disagree the sitting takes robust and records what
conservative would have been. The adopted text is a **diff file in `257-reports/`, never applied to `spec/`**
(the sentence enters the spec only through the runtime lane's landing, after the author's ratification, which
also owes `sigguard.c`'s `HERO_SIGNAL_REMEDY` and 22 goldens (§ 1) and the four pins the `spec` suite reads,
`spec/budget`, `spec/spendable`, `spec/real` and `spec/ledger`, which the critic measured red for every
arm); its real count is the warden's; a decision issue is drafted unfiled; the sitting and the reports are
committed by pathspec in the lane. **The sitting's files, the spec and the runtime are on
`deploy-site.yml`'s `paths:`** (`'docs/panel/**'` is its line 73): the sitting is committed on the lane and
**not pushed**; the count of site commits and the author's yes belong to the push (`CLAUDE.md` § Hard stops).
