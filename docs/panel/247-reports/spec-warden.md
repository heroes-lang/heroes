# Panel 247, spec-warden's report

Written as the sitting goes (started 2026-11-13 04:40 by `date`). Every number carries the command
that produced it; every count of tokens on a draft is the **vendored** maximum, a **lower bound, not
the reader's tokeniser**, because the real count of a draft needs `heroes measure --refresh`, an API
call this seat is not given (the brief; `.claude/rules/spec-shape.md` § How a change to the document
is made). **So every verdict below is `provisional`.**

## 0. What I checked first, and where the briefs fall short

- **The ceiling, reached by grep today**: `grep -n '^### 1.6' docs/design.md` reads 253 in my tree
  (the base), in `.claude/worktrees/p247` and on the trunk; line 255-256 in all three: *"must fit in
  16384 tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*. The round's
  head (`git -C .claude/worktrees/b34-round show HEAD:docs/design.md | sed -n '253,256p'`) says the
  same. The payment rule: design.md §1.6, lines 323-339 (*"the payment rule is unconditional"*, the
  two payments *"held to the same standard"*, a prediction admissible only if its instrument exists
  on the day of registration). Principle 0: design.md §1.0, line 112.
- **The base**: `git -C .claude/worktrees/b34-round rev-parse ae5e2cedd 'ae5e2cedd^{tree}'` printed
  `ae5e2cedddee93e0d53715a5b5ae6d7833c569df` and `3e22d53467b606946a70a6f9dfdea6f46ea47612`; D's
  commit `932876f9585817ec888d1ba875c880f2f1a4b3e3`, its parent the base; the round's head
  `2a3a37fc3be275bfa03062b59a1185f6161f8ab8`, as the brief says.
- **The spec at the base, in my tree**: `wc -l -c spec/heroes-spec.md`: 492 lines, 28770 bytes;
  `shasum -a 256`: `41ad6690571bd27b3185ab5be4a0850e998832b6a0f723b506b3843cfc6f763a`, the brief's.
- **Where the briefs fall short, first** (each run below): 00-shared.md lists three tracked programs
  that bind `signal`; at the round's head there are **four**, D's own case being the fourth (§ 9).
  And route A's names, `signal`, `sigset`, `bsd_signal`, are Darwin's: glibc's header, read, declares
  `ssignal` and `sysv_signal` too, and `bsd_signal` only under a feature set the compiler's
  `-std=gnu11` does not select (§ 9; ha's verdict). Nothing else in either brief differed from
  what I ran.


## 1. Setup, as run (my folder `.claude/worktrees/p247-scratch/247-spec-warden/`)

- `git -C .claude/worktrees/b34-round archive ae5e2cedd... | tar -x -C tree`; `git check-ignore -v tree`:
  `.gitignore:34:.claude/worktrees/`. `TMPDIR` set to `tmp/` beside `tree/` for every build and run.
- Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0. Stage 2: `./heroes build
  selfhost/main.hero -o heroes-next`, exit 0; `./heroes-next --version`: `heroes 0.6.0`, `sources
  7cc70d136b590d46`, **the base**. `./heroes-next doctor`: `ok runtime runtime` (the copy's own).
  That binary is kept as `tree/heroes-base` and prices every draft below.
- D: `git -C .claude/worktrees/b34-round diff ae5e2cedd 932876f95 -- runtime/parts/alloc.c > d.patch`
  (238 lines), `patch -p1`, exit 0; `shasum -a 256 runtime/parts/alloc.c` equals `git show
  932876f95:runtime/parts/alloc.c | shasum -a 256` (`5bea9640...`). Stage 2 again with D: below.
- Machine: `uptime` 4.46 at 04:40, 9.15 at 04:42; a round gate, a lane's tests and a Linux container
  ran beside (`ps`). No duration in this report is a measurement.

## 2. The count before, run again in my tree

`./heroes-next measure spec/heroes-spec.md` from my tree's root (base stage 2), exit 0: claude-legacy
**8340**, cl100k_base **8490**, maximum **8490** (*"a lower bound, not the reader's tokeniser"*),
spread 150, **real 11108** (dated *"claude-opus-5, 2026-11-13"* and called *"the binding number"*, no
`STALE`), *"Headroom: 5276 against the 16384 ceiling"*, the FFI floor mortgaging 60 of it (panel 030
R3), so *"what is measured against the ceiling is 11168"*. The brief's figures, every one: no finding.

## 3. Every draft priced, vendored only (each delta a lower bound on the real one)

Rebuilt by my own `tools/drafts.py` from `spec-warden.md`'s words, never reading the coordinator's
`price/` (`python3 -I tools/drafts.py tree/spec/heroes-spec.md drafts`): the sentence after the one
occurrence of `` `atexit(f: (function() -> ()))`. ``, asserted once, one space before it; a merged
draft replaces the one occurrence of § 13's callback sentence, asserted once. Hashes are `shasum -a
256`'s first 16 as the script prints them. Prices: `./heroes-base measure ../drafts/spec-<d>.md` from
the tree's root, each printing *"No ceiling judges ... a count and not a verdict"*.

| draft | sha256 (16) | claude-legacy | cl100k_base | maximum | delta |
|---|---|---|---|---|---|
| x, the base | `41ad6690571bd27b` | 8340 | 8490 | 8490 | |
| hr | `51ecfbc67c72bff2` | 8369 | 8519 | 8519 | +29 |
| hrr | `819cb24d7e9c0ec0` | 8383 | 8533 | 8533 | +43 |
| ha | `7e1a25c601977d20` | 8376 | 8526 | 8526 | +36 |
| he | `1868dce1e004ad46` | 8365 | 8515 | 8515 | +25 |
| hs | `f116bb024d10085f` | 8375 | 8525 | 8525 | +35 |
| **hn**, mine: *"No code of the program runs inside a signal handler."* appended | `717270c1c6181cad` | 8351 | 8501 | 8501 | **+11** |
| **hc**, mine: *"No code of the program runs inside a signal handler: C writes the handler."* appended | `45b7a30137cdf6d6` | 8356 | 8506 | 8506 | +16 |
| **hm**, mine, merged: the callback sentence's *"never a result;"* becomes *"never a result or a signal handler;"* | `8e7a4dbbe81f2920` | 8344 | 8494 | 8494 | **+4** |
| **hmc**, mine, merged: *"never a result, and never a signal handler, which C writes;"* | `b0a4522ffa6c0442` | 8350 | 8500 | 8500 | +10 |
| **hm2**, mine, merged: *"never a result, and never runs inside a signal handler;"*, true under every route that honours R4 | `1289a67a359b5f8c` | 8348 | 8498 | 8498 | +8 |
| **hm5**, mine, merged: *"never a result or a signal handler, and never called from one;"* | `f6ae572d3fa4c7d7` | 8350 | 8500 | 8500 | +10 |
| **hm6**, mine, merged: *"never a result or a signal handler, nor called by one;"*, the whole of R4 for a callback | `3335350f71b16ce0` | 8349 | 8499 | 8499 | **+9** |
| **hg**, mine, F3 merged: *"and sockets, clocks and signals from its platform layer's"*, were G to land | `37809500a355ac76` | 8342 | 8492 | 8492 | +2 |
| § 10's quadratic sentence out (panel 114's R2's own removal) | `b4599c9dfc10e1ee` | 8309 | 8459 | 8459 | **−31** |
| hr with that removal | `4121050f13da006b` | 8338 | 8488 | 8488 | −2 |
| hrr with that removal | `172239f516bc68a3` | 8352 | 8502 | 8502 | +12 |

**The brief's five reproduce to the token and to the hash** (hr, hrr, ha, he; hs `f116bb024d10085f`
as `llm-ergonomist.md` gives it). The removal reproduces panel 246's spec-warden's −31 to the token.

**Scale, never a measurement of these drafts**: real/vendored is 11108/8490 = 1.3084 on the whole
document and 94/73 = 1.2877 on T1's landing (the brief's two commands). Read so, the largest draft,
hrr, is on the order of +55 to +56 real, hm6 +12 and hm +5 (`python3 -I -c`, 43, 9 and 4 times each
ratio), against **5216 free** after the FFI floor (16384 − 11168, the instrument's own line).
**All five together, +168 vendored, are a thirty-first of that room (5216 / 168 = 31.0): the budget
vetoes nothing here.** What decides
each draft is §1.6's payment, §1.2's rewrite rate and Principle 0, below. Where two drafts differ by
fewer than ten vendored tokens I rank neither on price (spec-shape.md: a vendored delta is not a
price).

## 4. Question 3: what a reader of the base's spec plausibly writes, run with D

Stage 2 with D: `./heroes build selfhost/main.hero -o heroes-next` after `patch`, exit 0;
`./heroes-next --version`: `heroes 0.6.0`, **`sources 560c95206c908459`**, the line 00-shared.md gives
for D's stage 2. `heroes-next doctor` from my `probes/` folder: `ok runtime .../247-spec-warden/tree/runtime`
(D's). Seven programs under `probes/`, each `heroes-next fmt --in-place` (exit 0), `check` (exit 0) and
`build -o <name>` (exit 0, all seven). Every one does the same work, the lane's `storm.hero`'s: 200
rounds of 100,000 strings `f"word {r} {i}"`, so `main` is inside the allocator and inside `snprintf`
(`hero_int_to_str`) most of the time. **Every handler is the one a reader can write for SIGINT under
§ 4's *"There are no mutable globals"*** (spec line 135): a Heroes handler can reach `main` only
through C, a descriptor, the output or `exit`.

Runs: `python3 -I tools/one.py <binary> SIGINT 0.5` (my copy of the shared `storm.py` with the storm
loop replaced by one `kill`, same session, same bounds, same final group SIGKILL) and `python3 -I
tools/storm.py <binary> SIGINT 4 0.3` (the shared driver, copied, run unwrapped). `pgrep -fl
247-spec-warden/probes` after each batch: nothing. Crash reports: `python3 -I tools/ips.py
~/Library/Logs/DiagnosticReports swp1-nothing ... swp7-exit` (copied), `SHOW=1` for the stacks.

| probe | the handler | one SIGINT at 0.5 s | SIGINT storm | crash reports |
|---|---|---|---|---|
| swp1 | Heroes, `_ = 0`: sets nothing, there being nothing to set | exit 0, `built 20000000 words`: **Ctrl-C ignored** | exit 0, `built 20000000 words`, 1,094,323 sent | none |
| swp2 | Heroes, `_ = close(STOP_FD)`, the loop `dup`-probing between rounds (t3x1's shape, panel 246's one right base answer) | exit 0, `stopped after 9 rounds` | exit 0, `stopped after 12 rounds`, 28,910 sent | none |
| swp3 | Heroes, `print("caught a signal")` | exit 0, one `caught a signal`, `built 20000000 words` | **6 of 6** exit 0, 808,422 to 1,074,759 sent each | none |
| swp4 | Heroes, `print(f"caught {sig}")`: a number formatted | exit 0, `caught 2`, `built 20000000 words` | **6 of 6 died**: 5 exit -6 with D's words on stderr, **1 exit -9 with nothing on stderr** (run 4, pid 24170) | 5 SIGABRT `hero_alloc_reentered < hero_alloc_enter < hero_malloc_raw < ... < hero_int_to_str < h_main_on_int < h_0cb_main_on_int < _sigtramp < hero_malloc_raw < ... < h_main_round`; 1 SIGKILL `_os_unfair_lock_recursive_abort < ... < localeconv_l < __vfprintf < _vsnprintf < __snprintf_chk < hero_int_to_str < h_main_on_int < h_0cb_main_on_int < _sigtramp < localeconv_l < ... < h_main_round` |
| swp5 | C in a header the program ships, one module: a `static volatile sig_atomic_t` set by a C handler, read by `stop_asked()` between rounds | exit 0, `stopped after 10 rounds` | exit 0, `stopped after 12 rounds`, 4,635 sent | none |
| swp6 | **the same header bound by two modules**: `main.hero` calls `stop_install()`, `worker.hero` loops on `stop_asked()` | exit 0, **`stopped after 200 rounds`: Ctrl-C ignored in silence** | exit 0, **`stopped after 200 rounds`**, 1,283,449 sent | none |
| swp7 | Heroes, `print("stopping")` then `exit(code: 0)`: the plainest stop | exit 0, `stopping`, the round in hand dropped | **6 of 6** exit 0, `stopping`, 307 to 822 sent | none |

**Why swp6 ignores every signal**: `nm swp6-twomod/swp6-twomod | grep stop_seen` prints **two** local
`_stop_seen` (`b`) and one `_stop_on_int`; `nm` of swp5 prints one of each. Each module's C unit reads
the header and gets its own `static` flag: the handler `main`'s unit installs sets `main`'s copy, and
the worker reads its own, which nothing sets. Panel 246's critic measured this shape (`drained after
1000 requests`); here it is the remedy D's words and hrr give, built with D, and **no route of this
sitting sees it, since no code of the program runs in its handler**.

**What this says for the spec, measured and not inferred:**

- **The defect's class reproduces from a reader's first guess**: swp4, a handler that formats the
  signal's number, died 6 of 6 under the storm, **1 of 6 in silence** (SIGKILL, nothing on stderr,
  the lock), 5 of 6 by D's stop. With one signal it printed `caught 2` and ran clean: a reader's own
  test with one Ctrl-C passes.
- **Four of the five Heroes handlers survive today with D** (swp1, swp2, swp3, swp7: 14 storm runs
  of 14 at exit 0). **Every refusing route turns all five into refusals** (A at build, C at the first
  signal: by reading, unrun here), swp2 and swp7 being stops that work today. So a refusing route
  breaks programs that run correctly today, and the reader learns why only from the refusal or from a
  sentence, never from a crash they would otherwise have met.
- **A Heroes handler has nothing to say to `main`**: swp1 is what remains of a handler once R4 is
  obeyed in spirit and C is not used, and it ignores Ctrl-C. Under a deferring route (E, S) the same
  holds: deferral makes swp3 and swp4 safe and changes nothing of swp1, and swp7's `exit` still drops
  the round in hand (by reading).
- **The shapes R4 admits that a reader of this spec reaches are C's, or no handler at all.** The
  plain C one, swp5, works; the same code in two modules, swp6, ignores its signal with exit 0 and
  nothing said. And panel 246's t3-x-2, a reader of that sitting's base, blocked SIGINT and SIGTERM
  with `sigprocmask` and asked `sigpending` between steps, reasoning from § 4 that a handler *"has
  no mutable global to set"* (`246-reports/llm-ergonomist.md:1600` and its choice point 1), and
  was refused at build, `empty_record` (`docs/panel/246-*.md:213`). A pending signal is one bit,
  so that shape cannot count signals: t2's task still needs C or a door.

**What each sentence tells the reader of each shape** (a reading of the drafts' words against the
probes; *silent* means the sentence says nothing of that shape; *R4's clause* is the words *"no code of
the program runs inside a signal handler"*, which hr, hrr and ha each carry and which cover every
shape in general, as hn does):

| shape | hr | hrr | ha | he | hs | hm6 |
|---|---|---|---|---|---|---|
| a Heroes function given to `signal` (swp1 to swp4, swp7) | forbids: *"stops the program the first time a signal calls it"* | the same, and sends to C | forbids: *"the compiler refuses it"* | allows: runs it later | allows: runs it later | forbids |
| the same through `sigset`, `bsd_signal`, glibc's `ssignal` or `sysv_signal` | R4's clause | R4's clause | names the first two; R4's clause the rest | silent | silent | forbids (*"a signal handler"*) |
| a Heroes function installed through the program's own C (`wrap`) | R4's clause | R4's clause | R4's clause, false under A alone, which cannot see it | silent | silent | forbids |
| a Heroes function a C handler calls back | R4's clause | R4's clause, beside a remedy that has the reader write C handlers | R4's clause | silent | silent | forbids (*"nor called by one"*) |
| C in a shipped header, one module (swp5) | silent | prescribes | silent | silent | silent | silent |
| the same, two modules (swp6, the trap) | silent | **prescribes** | silent | silent | silent | silent |

So hr, hrr and ha reach every shape through R4's own words, and differ from hm6 in what they add:
a timing that one route keeps (hr), a remedy with a trap (hrr), a list the world falsifies (ha).

## 5. Question 4: hrr's *"a header the program ships"* leans on a rule the spec does not state

- **Run**: `grep -n -i ships spec/heroes-spec.md` in my tree prints nothing (exit 1), as the brief
  says; `grep -n -i "its own file\|program's own\|beside its\|header the program"` finds one line,
  470, *"the program's own name for it"*, about a handle, not a header. The spec says where a group's
  header is read (*"A group names its header"*, line 377) and never that the header may be the
  program's.
- **The rule it leans on is panel 114's R2**, ratified 2026-09-06, never landed: *"The header may be
  one the program ships beside its own file, so a difference between machines is written in C"*
  (`docs/panel/114-*.md:183-184`), gated by its own R3, *"Three instruments are owed, and R2 does
  not land without them"* (line 192). hrr would land R2's permission in a narrower form, for
  handlers, without that gate. The gate's second instrument, *"The compiler reports which `extern`
  declarations it did NOT verify on this machine"*: `/usr/bin/grep -rli --include='*.hero'` over
  `selfhost/` and `tests/harness/` for *not verified*, *did not verify*, *dead arm*, *not checked on
  this machine* finds no file; *unverified* finds `cli/unverified.hero` and its callers, which tell
  a package's `path` entry (its head, read), not a dead `#ifdef` arm. **A question, not an absence**
  (my vocabulary; panel 246's spec-warden found the same on 2026-11-12, `246-reports/spec-warden.md`,
  question 5). The third instrument exists, `selfhost/cli/header_refused.hero` (`ls`).
- **The route is real and the compiler already teaches it, at zero spec tokens.** design.md
  prescribes *"a header of the program's own"* as the door for a reserved C name, a macro, a
  variadic and a header order (`/usr/bin/grep -n -i "header of the program's own" docs/design.md`:
  lines 613, 2672, 2762, 2769, 2773, 3167, 4030), and the diagnostics carry it where the mistake is
  made (panel 246's spec-warden's q5: `raise` is `error[reserved_word]` naming `static inline int
  c_raise(int x)`). **No spec sentence names it, by a ratified gate.** hrr would be the first.
- **Its first use for a stop is two measured failures**: swp6 above (two modules, the signal ignored
  in silence, 2 of 2 runs, every route blind to it), and panel 246's q4-accept, not re-run here: a
  server whose flag, in its own header, is asked between connections (set there by a Heroes handler;
  a C handler's flag waits the same, by reading the base's `hero_net_accept`,
  `runtime/platform/hero_net.c:950-985`, which asks only the layer's stop and asks again after
  `EINTR`) was *"still in `accept` 3 s after SIGINT and
  58 s after SIGTERM"* until a client connected (`246-reports/spec-warden.md`, § A finding for B2),
  the failure panel 246's R4 put the stop in the platform layer to avoid. hrr tells every reader,
  servers included, to answer SIGINT with that shape.
- **What hrr would then owe**: R2's gate met or ruled by a sitting; swp6's trap told by the compiler
  (a `static` object of a header two modules bind: `cli/two_ways.hero` passes a `static`
  definition over by design, `.claude/rules/c-boundary.md`, and swp6 built at exit 0, so
  `ffi_defined_twice` does not tell it either) or the remedy worded to one module; and a reason a
  server's reader should not use R4's own door, which the spec may not name (panel 246's veto, *"no
  stop sentence in the specification"*).

## 6. Questions 1 and 2: what § 13 owes, route by route, and which burden falls where

**Two burdens, kept apart, each cited.**

- **Principle 0's** (design.md §1.0, line 120: *"The burden of proof for any proposed form"*) falls on
  a **new form**. It does **not** fall on a refusal: CLAUDE.md § Precedence, written in at the
  author's *2a* on panel 215 (ratified 2026-10-16, `docs/panel/215-*.md:244-246`): *"robustness
  decides THAT a measured class of wrong programs closes, Principle 0 HOW, so a refusal lands at once
  and a new form still needs § 2's ticket"*. So the **rule** (hr's and ha's first halves, hm6, hm, hn) is
  not Principle-0-gated; a **deferral** (E, S, F: a new meaning for a function given to `signal`,
  and a mechanism, a thread or compiled polls), a **door** (G) and **a road the spec would newly
  advertise** (hrr's remedy, panel 114's gated R2) are.
- **§1.6's payment** (design.md:323-326, *"the payment rule is unconditional. Every amendment owes a
  named removal or a registered prediction naming an instrument that exists today"*) falls on
  **every** sentence, the rule included. A rule is not exempt for being short.
- So **question 2's answer**: panel 246's veto of S1 to S3 was the first burden (two doors and a
  built-in the compiler did not need, promising a stop that was false for a server). A sentence that
  only states R4 is of the other kind: a refusal's statement, owing §1.6's payment and §1.2's case,
  not a ticket. A sentence that describes a deferral (he, hs) is of the first kind again.
- **The compiler needs none of it**: `/usr/bin/grep -rn --include='*.hero' 'signal(' selfhost`
  prints 5 lines: one test string, `selfhost/foreign_at.hero:409` (a fixture for the `int` foreign
  word), and four of `hero_run_signal`, a reader of a child's exit; `-E 'function
  (signal|sigset|bsd_signal|sigaction)\('` names that one file. The compiler hands C no handler.

**What § 13 owes, by route** (the spec's silence today: `grep -c -i 'signal\|handler'
spec/heroes-spec.md` is 0, the brief's command; design.md states no R4 either, `/usr/bin/grep -n -i
signal docs/design.md` finding only `raise`'s door and a lease reader):

| route | is a Heroes handler refused, deferred or run? | does the spec owe a sentence? |
|---|---|---|
| **D alone** | run, stopped only on the allocator (swp1, swp2, swp3, swp7 ran theirs 14 storms of 14) | **No true sentence exists**: *"no code of the program runs inside a signal handler"* is false under D (swp3's handler printed inside the signal, 6 storms of 6 at exit 0). D alone changes R4: the author's. |
| **A** (+ a backstop) | refused at build where the callee is `signal`, `sigset`, `bsd_signal` | **Yes, the rule.** § 13's callback sentence permits the refused program by its letter (a function handed to C as a parameter), and a reader wrote it (panel 246's t3-x-1; swp1 to swp4, swp7), and CLAUDE.md § 12, *"Spec beats compiler: the compiler has the bug"*, makes an unstated refusal the compiler's defect. The thread refusal is not a precedent against this: § 13 says where threads come from (*"threads ... from the language's `hero_os.h`"*, line 377) and nothing of where a handler comes from. |
| **C** (thunk form) | refused at the first signal | **Yes, more so**: a build and every run with no signal pass, so the spec is the one place the rule is learned before a signal comes (swp4's one-signal run printed `caught 2` and exited 0 today; under a refusal at the first signal the reader's signal-free tests pass the same way, by reading). |
| **E, S, F** (deferral) | run later, outside the signal | **Yes, the new meaning** (he, hs or hm2), since a reader's model of C's `signal` is otherwise false; but Principle 0's ticket is unmet (nothing built, no effect measurable: `llm-ergonomist.md` grades arm e *"by reading ... marked provisional"* unless a seat builds E; hs is in no arm), so it waits, sentence and route. |
| **G** (a door) | no Heroes handler needed | **No**, by panel 246's precedent: a facility of the platform layer is documented in `hero_net.h` and the `net` card at 0 spec tokens; hg (*"sockets, clocks and signals"*, +2) only if readers are measured not to find it. G owes Principle 0's ticket. |

**Where the remedy goes: the refusal's message, at 0 spec tokens**, as every other door of the
program's own header is taught today (design.md's seven lines in § 5 above; design.md §4.17: a
diagnostic carries what is needed to fix the mistake it names). The spec cannot name the stop door
(panel 246's veto), and the C-header remedy has swp6's trap, so a spec remedy is either forbidden or
measured to fail in silence.

**§1.2, as arithmetic and not a measurement of how often a program installs a handler**: one round
trip is 500 to 2000 tokens (design.md:197-198); a sentence is paid on every prompt. At the scale
ratios above, the share of prompts that must install a handler (and would have made the mistake) for
a sentence to pay is **hm 0.26% to 1.05%**, **hm6 0.58% to 2.36%**, hn 0.71% to 2.88%, hr 1.87% to 7.59%, ha 2.32% to 9.42%,
**hrr 2.77% to 11.25%** (`python3 -I -c`, cost / 2000 and cost / 500, each at both ratios). A clause
of the rule pays at a share of handler tasks the milestone's servers plausibly reach; a sentence with
a remedy needs one task in nine to thirty-six to be a handler task.

## 7. Question 5: the payment, and the predictions, registered 2026-11-13 at 05:05 CET

**Registered before any blind session exists**: `ls` of the twelve folders under
`p247-scratch/s/` at 04:59:35 and again at 05:04:55 (`date`) counted 3 files in each (`brief.md`,
`heroes-spec.md`, `task.md`), no `report.md` and no `run.json`; `247-reports/` held
`critic-briefs.md`, `historian.md` and this file, no `llm-ergonomist.md`. I have read no session.
Every instrument named exists today: the blind seat's run and its handler record
(`llm-ergonomist.md`, registered; *"its **handler** is recorded beside its class"*), `heroes
measure --refresh`, and my folder's `probes/` with `tools/one.py` and the shared `storm.py`.

- **SW1, the rule moves readers (hm6's payment, or hm's).** In this sitting's blind run as
  `llm-ergonomist.md` registers it, **the answers of arm x that install a function of the program as
  a signal handler** (given to `signal`, `sigset` or `bsd_signal`, or installed through C the program
  ships) **outnumber arm r's by at least 2** of 4, the blind seat's own margin. Falsified by a
  difference under 2. Scored in this sitting (M-core-packages) from the handler the grading records
  for each answer. What it shows if it holds: the spec's silence leads readers to the refused shape
  and a sentence stating the rule leads them off it. hrr carries the rule with a remedy, so SW1
  bounds hm6's effect from one side and no more (said, not hidden); and it can fail for a reason
  panel 246 already showed, a base reader who avoided `signal` altogether (t3-x-2, § 4). If it fails,
  or the author gives no yes for the run, the rule's remaining ground is CLAUDE.md § 12, which is no
  payment: hm6 is then paid by a removal the author names, or waits (below).
- **SW2, hrr's arm passes the blind seat's rule.** **Arm r gives at least two more `right` than arm x
  over t1 and t2, graded under C** (the registered decision rule). Falsified by r under x + 2. I
  register it against my own objection's comfort: if it holds, hrr's effect on single-module tasks
  is real, and my objection stands only on what such a task cannot see (SW4 and panel 114's gate).
- **SW3, he's arm does not.** **Arm e gives at most one more `right` than arm x**, graded under E
  (by reading, provisional, or a built E). Falsified by e at x + 2 or more; under a **built** E that
  would meet Principle 0's thesis branch and narrow my veto of he to its being unbuilt.
- **SW4, the remedy's trap survives every route.** `probes/swp6-twomod`, rebuilt with the compiler
  and runtime of the route this sitting adopts, prints **`stopped after 200 rounds`** at exit 0 after
  one SIGINT at 0.5 s (`one.py`) and under `storm.py <binary> SIGINT 4 0.3`, as it did twice here.
  Falsified by a stop before round 200 or a refusal at build. Scored at the adopted route's landing
  (M-core-packages). It is the falsifier of my objection to hrr and to D's words' remedy.
- **SW5, the clause's real price.** hm6 lands at **+8 to +15 real** on `heroes measure
  spec/heroes-spec.md --refresh` (vendored +9; scale 11.6 to 11.8), or hm at **+3 to +8** (vendored +4). Scored by the landing's `--refresh`, the one paid call any landing makes.

**The payment for each draft I would admit, and the falsifier for each I refuse** (CLAUDE.md § 12:
a refusal is held to the same standard as a feature):

| draft | admit? | paid by | what would make my refusal wrong |
|---|---|---|---|
| **hm6** (mine; hm its conservative twin) | yes, with the refusing route | SW1, priced by SW5; failing SW1, a removal the author names | n/a |
| hr | no, hm6 instead | (SW1 would pay it) | the sitting adopts C alone and C's refusal leaves its words on stderr in 50 of 50 storm runs of swp4 on Darwin: hr's timing clause is then true and tells a reader that a run with no signal passes |
| hrr | no | (SW2 would pay it) | SW4 falsified, or the remedy worded to one module and the trap told by the compiler; and panel 114's R3 gate met or ruled by a sitting |
| ha | no, hm6 instead | (SW1) | a backstop that refuses every installer on the four CI legs, `wrap`, `ssignal` and `sysv_signal` included: ha's clause is then true, at four times hm6's price (nine times hm's) for the same rule and a list |
| he | **veto** | none | E built, and arm e graded under it at x + 2 (SW3 falsified), the sentence also saying what a fault signal, `exit` and `SA_SIGINFO`'s pointers do |
| hs | **veto** | none | S built, an arm s graded under it at x + 2, and its latency measured on a thread blocked in C |

**The removal available, and whose it is.** § 10's quadratic sentence (line 330 of the base, the one
that opens *"`+` on `str` copies both sides"* and ends *"`join` and `repeat` build in one pass."*),
**−31** vendored (`spec-q-out`, above), is panel 114's R2's own named removal, ratified with R2 on 2026-09-06 on the
ground *"its whole content is performance advice"* and unspent since (line 330 of the base still
holds it). It would pay hm6 three times over, or hr outright (hr with it: −2), but it can be spent
once: **spending it here takes it from R2**, and the synthesis must say so if it does. I name it for
the author's choice and do not spend it.

**A paid run worth weighing, not run** (no paid run is mine): SW1 bounds hm6 only through hrr's
text. **An arm m, hm6's specification (`3335350f71b16ce0`), in place of arm e** would measure the
sentence the sitting can actually land, at the same twelve sessions and the same 4.80 USD bound by
caps (`llm-ergonomist.md`), since arm e can only be graded by reading unless a seat builds E. It
would have to be registered before any session; the coordinator's call.

## 8. Verdicts, one per draft, in my definition's structure

Every count below is the vendored maximum, **a lower bound, not the reader's tokeniser** (§ 3's
commands); the real delta of every draft is unmeasured and owed to the coordinator, who asks for
`--refresh`. So every verdict with a count in it is `provisional`; the two vetoes rest on no count.

### hr (route C's sentence)

- `verdict`: **object**, `provisional`
- `section`: design.md §1.6 (the payment, 323-339); §1.2 (190-204); CLAUDE.md § 12; spec § 6, *"An
  abort ends the program at once, saying why"* (line 183)
- `spec_token_delta`: **estimated, a lower bound**: 8490 to **8519 (+29)**, claude-legacy 8340 to 8369;
  real unmeasured (scale +37 to +38)
- `removal`: nothing named, and that is a problem; hm6 states the whole rule for +9, hm its direct half for +4
- `needed_for_self_hosting`: **no**
- `argument`: hr states R4, and a timing only route C keeps. Under A, or A with C behind it, a function given to `signal` is refused at build and never "stops the program"; under C alone the clause holds only once the refusal says why without dying in its own lock, which protoC did not (1 of 3 `quiet` runs, SIGKILL in its own `snprintf`, as 00-shared.md reads the lane's log), and spec § 6's abort promises *"saying why"*. A reader needs the rule, since swp1 to swp4 and swp7 are what it forbids; the whole rule costs +9 merged into the callback sentence (hm6). The other +20 buy one fact, true of one route.
- `prediction`: SW1 (would pay its rule)
- `condition`: the sitting adopts C alone, and C's refusal leaves its words on stderr in 50 of 50 storm
  runs of swp4 on Darwin; then hr's timing is true and worth its tokens, paid by SW1

### hrr (route C's sentence and its remedy; the blind seat's arm r)

- `verdict`: **object**, `provisional`
- `section`: design.md §1.6; §1.2; §1.12 and CLAUDE.md § Precedence rank 3 (a stop ignored in silence);
  panel 114's R3, a ratified gate (`docs/panel/114-*.md:192`); panel 246's R4 (the stop is the layer's)
- `spec_token_delta`: **estimated, a lower bound**: 8490 to **8533 (+43)**, claude-legacy 8383; real
  unmeasured (scale +55 to +56); with § 10's quadratic sentence out, +12
- `removal`: nothing named; the one priced, § 10's quadratic sentence (−31), is panel 114's R2's own
- `needed_for_self_hosting`: **no**
- `argument`: Its first clause is hr's. Its remedy names a road the spec has never stated: panel 114's R2, ratified 2026-09-06 and held by its R3, *"R2 does not land without them"*, whose dead-arm report nobody has found. Its plainest uses fail in silence: swp6, one header in two modules, ignored SIGINT in 2 runs of 2 at exit 0, a shape no route sees; and a server's own flag does not end a wait in `accept` (panel 246's run; the base's `hero_net_accept`, read). A single-module blind task sees neither. The remedy belongs in the refusal's message, where design.md already teaches the program's own header seven times, at zero spec tokens.
- `prediction`: SW2 (it holds: r at x + 2 or more under C) and SW4 (swp6 ignores its signal under every route)
- `condition`: SW4 falsified, or the remedy worded to one module and the two-module trap told by the
  compiler; panel 114's R3 gate met or ruled by a sitting; and SW2 holding

### ha (route A's sentence)

- `verdict`: **object**, `provisional`
- `section`: `.claude/rules/spec-shape.md` (no closed list: panel 087's warden); design.md §1.6; §1.2;
  CLAUDE.md § 12
- `spec_token_delta`: **estimated, a lower bound**: 8490 to **8526 (+36)**, claude-legacy 8376; real
  unmeasured (scale +46 to +47)
- `removal`: nothing named, and that is a problem
- `needed_for_self_hosting`: **no**
- `argument`: A closed list of installers, which the world already falsifies. The coordinator's `wrap` hands `sigaction` a Heroes function through the program's own header and printed `caught 30` (the coordinator's run, 00-shared.md). glibc's header as zig ships it (read, not run) declares `ssignal` under `__USE_MISC` and `sysv_signal` under `__USE_GNU`, and `bsd_signal` only without `__USE_XOPEN2K8`, so under `-std=gnu11` the list names one installer Linux will not bind and misses two it will. Its clause *"no code of the program runs inside a signal handler"* is then false under A alone; panel 087's warden refused a closed list of aborts for this shape. It also says the refusal twice, for +36.
- `prediction`: SW1 (would pay the rule it carries)
- `condition`: a backstop that refuses every installer on the four CI legs, and the list dropped for the
  rule, which is hm6

### he (route E's sentence; the blind seat's arm e)

- `verdict`: **veto**. Not provisional: no count enters it.
- `section`: CLAUDE.md § 2 and design.md §1.0 (Principle 0's burden unmet); CLAUDE.md § Precedence,
  *"a new form still needs § 2's ticket"* (panel 215's *2a*); design.md §1.3 (a call whose meaning a
  rule keyed on the callee's name sets)
- `spec_token_delta`: lower bound 8490 to **8515 (+25)**, claude-legacy 8365
- `removal`: nothing named, and that is a problem
- `needed_for_self_hosting`: **no** (5 lines name `signal(` in `selfhost/`, none a handler the compiler installs)
- `argument`: he describes a new meaning for a callback given to `signal`, and a mechanism, a thread of the runtime's, that nothing builds: the sentence is false at the base and stays false until E lands. Principle 0: the compiler needs no deferral, and no thesis effect can be measured, since `llm-ergonomist.md` grades arm e by reading, provisional, unless a seat builds E. Deferral does not make a handler useful either: with no mutable global (spec § 4), swp1's handler has nothing to tell `main`, deferred or not. It would rescue swp3 and swp4, and leave swp7's `exit` dropping the step in hand.
- `prediction`: SW3 (arm e at most x + 1)
- `condition`: E built, arm e graded under it at x + 2 or more, and the sentence also saying what a fault
  signal, `exit` and `SA_SIGINFO`'s two pointers do

### hs (route S's sentence)

- `verdict`: **veto**. Not provisional.
- `section`: as he
- `spec_token_delta`: lower bound 8490 to **8525 (+35)**, claude-legacy 8375
- `removal`: nothing named, and that is a problem
- `needed_for_self_hosting`: **no**
- `argument`: Everything said of he, on less evidence: hs is in no arm of the blind run (`llm-ergonomist.md`: *"Not in the twelve"*), so no measurement of it is even planned, and S's polls at loop heads and function entries are a cost on every program, unbuilt and unmeasured. Its clause *"once that thread is back in the program's own code"* promises a latency the spec cannot bound: a thread blocked in C, in `sleep` or `accept`, runs the handler only when C returns, which is Python's own caveat (the historian's verified quote, 00-shared.md).
- `prediction`: none: no arm reads it, so nothing scores it
- `condition`: S built, an arm s graded under it at x + 2 or more, and its latency measured on a thread
  blocked in C

### hm6, this seat's counter-proposal (hm its conservative twin)

- `verdict`: **approve**, `provisional` (a vendored lower bound; and only with a refusing route)
- `section`: CLAUDE.md § 12 (*"Spec beats compiler"*); design.md §1.6 (the payment); §1.2;
  `.claude/rules/spec-shape.md` (*"Merging beats appending"*; one home per rule, § 13 the FFI's)
- `spec_token_delta`: **estimated, a lower bound**: 8490 to **8499 (+9)**, claude-legacy 8340 to 8349
  (hm: 8494, +4); real unmeasured, SW5 registers +8 to +15 (hm +3 to +8)
- `removal`: none; paid by SW1; failing SW1, a removal the author names, the one priced being panel
  114's R2's own (−31), which R2 would then lose
- `needed_for_self_hosting`: **no**
- `argument`: R4 in its one home: *"A callback is a **parameter**, never a result or a signal handler, nor called by one; ..."*. It forbids exactly what a reader of today's spec writes (swp1 to swp4, swp7, panel 246's one right base answer) and the shape D's words name (*"or called from one"*); it makes the refusal the spec's, so a program the compiler still accepts is the compiler's defect, never the reader's guess (CLAUDE.md § 12). The remedy stays in the refusal's message, as every door of the program's own header is taught today: the spec may not name the stop door, and the C-header remedy fails in silence in two modules (swp6).
- `prediction`: SW1 and SW5
- `condition`: it lands in the commit that lands a refusing route (A, C or both), never before, since
  under D alone it describes a rule nothing holds; where the route refuses only what an installer is
  handed, the shape it leaves (a callback a C handler calls, accepted) is filed `blocking`, *"a wrong
  one accepted"*; under C alone the rule is held where a signal calls the handler, as an index is
  held where it is read, so the 2257 pair, whose handlers never run, stay accepted and are rewritten
  to the spec's word; **robust against conservative** (CLAUDE.md § 4): hm6 covers the shape D's
  words name, *"or called from one"*, for 5 more vendored tokens; hm (+4) is what the conservative
  resolution would be

## 9. Findings for the synthesis beside the drafts

- **The tracked programs are four at the round's head, not three.** `git -C .claude/worktrees/b34-round
  grep -l -E 'function (signal|sigset|bsd_signal|ssignal|sysv_signal)\(' <commit> -- '*.hero'`: at the
  base, the two 2257 cases, the 2496 case and `selfhost/foreign_at.hero` (a test string for the `int`
  foreign word, never built); at `2a3a37fc3` also **D's own case**,
  `fixedbugs-2494-a-handler-that-allocates-inside-the-allocator-stops-its-program-by-name.hero`, whose
  `on_signal` builds `[sig, sig, sig]` (`git show 932876f95:<it>`). hm6 makes all four programs the
  spec forbids. Under A each is refused at build, D's case included, so D's guard would no longer be
  reached by its own case; under C alone the two a signal calls change (2496 and D's case), and the
  2257 pair, whose handlers never run (by their names and 00-shared.md; unrun here), builds and runs
  as today against a sentence that forbids it, a refusal at run time that never fires.
- **Route A must not be spelled as a refused word.** `raise` is refused as a foreign word
  (`selfhost/foreign_words.hero:163`, `"try" | "catch" | "throw" | "raise"`), and the `spec` suite's
  `rejected` check fails when a word the lexer refuses appears in the spec's code
  (`tests/harness/suite_spec.hero:834-856`, read). Were `signal` refused that way, hr, hrr, ha, he
  and hs, which each hold `` `signal` `` in a code span, could not land, and a binding handing
  `signal` a C handler (R4's own shape) would be refused with it. A refusal of the call shape, a
  function of the program handed to the named installer, keeps both. **hm6 names no installer** and
  is indifferent to A's spelling. A reading, unrun.
- **D's words (00-shared.md question 6), from the spec's side**: they cost no spec token and are the
  right home for the remedy, but their remedy, *"a handler written in C in the program's own
  header"*, is swp6's shape, which ignored SIGINT in silence with D applied (2 of 2). Whoever amends
  them under the adopted route should qualify it (one module binding the header) or point at a door
  that holds one counter per process (G, if built). Under E, S or F they are false and must change.
- **Spec § 6's abort is a promise any run-time refusal inherits**: *"An abort ends the program at
  once, saying why"* (line 183). swp4's SIGKILL with nothing said (1 of 6, the lock) is not an abort
  but a crash, and a refusal that dies in its own `snprintf` (protoC, 1 of 3 on `quiet`, as
  00-shared.md reads the lane's log and crash report) is an abort that breaks § 6. So a route C,
  and hr's promise with it, is spec-true only where its refusal is async-signal-safe end to end.
- **What a Heroes handler can do at all, under any route**: § 4's *"There are no mutable globals"*
  leaves it four channels to `main`, C, a descriptor, the output and `exit` (swp1, swp2, swp3, swp7).
  A deferring route makes those safe and adds none; the counting and stopping tasks still need C or a
  door. This bounds what any deferral can buy a reader, by reading, and it is SW3's ground.
- **A spec sentence a blind reader misread, beside this sitting** (for the coordinator to weigh; I
  file nothing): § 11's *"`exit(code: i64)` (ends the program; a code outside 0 to 255 aborts, one
  written as a number is refused)"* was read by panel 246's t3-x-2 as refusing `exit(code: 0)`: *"so
  `exit(code: 0)` may be a compile error"* (`246-reports/llm-ergonomist.md:1676`), and it avoided
  `exit`. Measured here: swp7's `exit(code: 0)` passed `check` and `build` at exit 0 and ended its
  program with 0. *"one"* means a code outside the range; a reader took it for any code.
- **Linux, read and not run**: zig's glibc `signal.h` (`/Users/joseph/.zvm/0.15.2/lib/libc/include/
  generic-glibc/signal.h`, lines 75-127 and 367) declares `__sysv_signal`, `sysv_signal`
  (`__USE_GNU`), `signal` (`__USE_MISC`, else the SVID one), `bsd_signal` (`__USE_XOPEN_EXTENDED &&
  !__USE_XOPEN2K8`), `ssignal` (`__USE_MISC`, *"SVID names for the same things"*) and `sigset`. Darwin's
  SDK declares `bsd_signal` and `sigset` (`signal.h:82` and `102`) and `signal` (`sys/signal.h:392`),
  read with `grep`. Which of glibc's apply to a Heroes build: `echo | clang -std=gnu11 -dM -E -`
  defines no `__STRICT_ANSI__` (count 0; `-std=c11` gives 1), so glibc's `features.h` (same folder,
  lines 316-326, 382-385 and 430-431, read) takes `_DEFAULT_SOURCE`: POSIX.1-2008, hence
  `__USE_XOPEN2K8` and no `bsd_signal`, and `__USE_MISC`, hence `ssignal`. A list of installers by
  name is a list of what one C library declared one day.

## 10. What I did not do, and why

- **No `--refresh`, no API call, no paid run**: every count is vendored and every delta a lower
  bound; the real counts of any draft the synthesis keeps are owed to the coordinator.
- **No route built**: A, C's thunk form, E, S, F and G are the other seats'; my probes ran with D
  only, as my brief asks, and every sentence above about what a route would do to a probe is a
  reading, said so where it stands.
- **No Linux and no Windows run**: the glibc facts above are readings of a header zig ships; the
  Windows box is offline per 00-shared.md, and I read no Windows header.
- **No whole suite**: no draft was applied to a tree, so `spec`, `special`, `grammar`, `fixes` and
  `unseen` (the spec row of `.claude/rules/verification.md`) are owed at the landing of whichever
  clause lands, `rejected` and `named` among `spec`'s checks.
- **Every process I started was stopped**: `pgrep -fl 247-spec-warden/probes` after each batch printed
  nothing.
