# 246: a killed run is recovered under a lock beside its folder, an edit in a placed tree is never dropped in silence, the spec tells a thread its one i64, and a server stops inside the platform layer

**Status** a sitting in two parts, convened 2026-11-12 by the coordinator of M-core-packages on
the author's answer `1a 2a 3a 4a 5a` of 2026-11-12 at about 00:30 to the coordinator's batch
(`/decide`), given in the coordinator's conversation and on no issue when the briefs were written
(`246-briefs/00-shared.md`, its first paragraph; the critic's first pass, item 20): **`2a`**, defect
2453's repair through the soundness lane on route A; **`3a`**, a full sitting on the server step's
two questions, the blind seat the only paid run, with a ceiling of 10 USD for the whole sitting, the
author's. Held as one sitting in two parts, so the shared brief and the critic are written once. The
resolution is `ratified 2026-11-12` (§ Author's verdict).
**Seats** Part A, the soundness lane: compiler-engineer and ffi-pragmatist, with the historian's
precedent as input and no verdict. Part B, the full panel: compiler-engineer, ffi-pragmatist,
spec-warden, historian (advisory), llm-ergonomist (the blind seat, twenty sessions). The
completeness critic read the briefs before any seat (22 items; the briefs repaired on them, the first
versions kept as `246-briefs/*.before-critic.md`) and the reports after (16 items, the blind seat
graded by the rule registered before any session).
**Base** `ef50a7478c241f765c0b308cb55282f95d64053f`, the head of batch 33's round on `b33-round`, its
stage 2 printing `sources 796d1133542b1909`; each seat built its own in
`.claude/worktrees/p246-scratch/246-<seat>/tree/`. The critic's first pass ran at the first base,
`b8c8240d4` (`sources 017930ae6061c8b4`).
**Briefs** `docs/panel/246-briefs/`, **reports** `docs/panel/246-reports/`, **evidence**
`docs/panel/246-reports/evidence.tar.xz` (its note beside it).

## The proposal

Verbatim from `246-briefs/00-shared.md`, its lines 93 to 110 (Part A) and 182 to 245 and 269 to
289 (Part B).

### Part A, defect 2453

**The routes, A as lanes b31-pkg and b32-pkg wrote it** (the issue's two comments):

- **A.** Each run of `heroes add` or `heroes fetch` takes an exclusive lock the operating system
  releases when the process dies (`flock` on POSIX, `LockFileEx` on Windows) on a file inside its own
  `heroes.packages/.fetching-<pid>-<n>/`, holds it until the run ends, and writes its ledger there
  before each rename; a later run recovers every `.fetching-*` folder whose lock it can take (keeps a
  placed tree the catalogue names, removes any other tree the dead run placed, puts back what it moved
  aside, removes the folder) and leaves one whose lock it cannot take. Defect 2434's *leaves the
  program's folder as it found it* (`selfhost/fetch/staging.hero:98-108`), carried to a kill.
- **B.** Report the leftovers and remove nothing. **C.** Pin them as a known cost.
- **D** (the critic's, a route nobody listed): `staging.opened` never removes a standing
  `.fetching-<pid>-<n>`; it refuses it or takes the next serial, as `heroes new` already rules for its
  own staging name (`selfhost/cli/stub.hero:175-176`: *"A staged name that stands already, a run of
  this process number killed before it ended, is refused and never removed: it is not this run's."*,
  the check at 180-184).
- **B and C are false of the code as long as `staging.hero:48` stands**: `opened` removes a folder of
  its own name before it makes it (`written(process.remove_tree(folder))?`), so nothing is left alone
  and no pin holds. Every route carries D or says why it does not need it.

### Part B, the server step

**The proposal, as spec diffs and candidate spellings.** Every price is the vendored maximum of
`heroes-next measure`, run from the base's root on a copy of the base's spec with the sentence in
place (`p246-scratch/price2/`, made by `p246-scratch/tools/drafts2.py`); the real delta needs
`--refresh`, a paid call nobody is named for.

- **B1** (feature 1073). Spec § 13 opens, at the base, *"Anything beyond this document comes from
  packages or C libraries; threads, processes and files from the language's `hero_os.h`, and sockets
  and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names
  one."* (`spec/heroes-spec.md:377`, the one line `grep -n -i thread` finds; F3 as feature 2295 landed
  it, `docs/measurements/010-spec-budget-ledger.md` row 8417: +31 vendored and +41 real, 10,973 to
  11,014). Append after it:
  - **T1** (+73): *"A thread runs a function of one `i64` and gives back one, and nothing else crosses:
    `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` starts it and
    `hero_thread_join(handle: i64) -> i64` waits for its answer."*
  - **T2** (+15): *"Into a thread and back crosses one `i64` and nothing else."*

  **Two measurements, in their order, and no ruling between them found.** Panel 113's R5, 2026-09-06
  (`grep -n 'the scalars and' docs/panel/113-*.md`: 276; ratified *"in full"* at 410): the body
  signature *"admits only what is measured safe to cross. Today that is the scalars and **`str`**"*,
  on 250,000 concurrent operations on one `HeroStr`, 0 races. Journal 034 (`sed -n 79p
  docs/records/journal/034-isolated-threads.md`): *"Only an `i64` crosses a thread boundary"*, with no
  reason given for leaving `str` and the other scalars out. The critic searched journal 034, panel 113
  and design.md Part 7 item 13 for a ruling that narrowed R5 and found none: where `i64`-only was
  decided, a ruling or a gap between R5 and the code, is a question, and so is whether R5's *scalars*
  include `f64` and `bool`. Panel 111's *"9 reported `heap-use-after-free`"* for a shared `str`
  (`docs/panel/111-*.md:129`), which feature 1073 cites against widening, predates *"The refcount
  half is closed at M-isolated-threads step 3"* (design.md:4082). **`runtime/hero_os.h:430-434` is
  false today**: *"WHAT MAY CROSS IS DECIDED BY THE CHECKER ... a `[T]` or a `{K: V}` in that signature
  is `error[ffi_type]`"*, while a `str` body and a handle body pass `check` and are refused by clang at
  `build` (below). Panel 232's promise lets a 1.x add to `hero_os.h` and never narrow it, so the rung
  the spec writes now is the one promised at 1.0.0.
- **B2** (feature 1069). A way for a server to stop cleanly, never a mutable global (design.md Part 6,
  `grep -n 'Global mutable state' docs/design.md`: 3572). The candidates, each to be built or priced
  against the others:
  - **S1** (+70, § 13): *"`hero_stop_catch()` makes Ctrl-C and a request to end (SIGINT, SIGTERM) set a
    flag instead of ending the program, and `hero_stop_asked() -> i64` answers 1 once one has come: a
    loop asks between its steps, finishes what it holds and returns from `main`."*
  - **S2** (+65, § 13): *"From the first call of `hero_stop_asked() -> i64`, Ctrl-C and a request to end
    (SIGINT, SIGTERM) no longer end the program; it answers 1 once one has come, and a loop that asks
    between its steps finishes what it holds and returns from `main`."*
  - **S3** (+58, § 11, a built-in after `exit`, the table's ` · ` before it): *"`stop_asked() -> bool`
    (false until Ctrl-C or a request to end, SIGINT or SIGTERM, arrives; from its first call such a
    request no longer ends the program, so a loop that asks between its steps finishes and returns from
    `main`)."*
  - **(a)** the ending signals blocked with `pthread_sigmask` and asked with `sigpending` between
    steps: no handler, no Heroes code in one, no `SA_RESTART` question; **(b)** the stop as a
    descriptor (a self-pipe's read end, kqueue's `EVFILT_SIGNAL`, Linux's `signalfd`) that one `poll`
    waits on beside the listening socket, so a blocked `accept` wakes; **(c)** the stop inside the
    platform layer's own blocking calls, since panel 234's R3 makes the layer own `accept`. All three
    unrun.
  - **The program's own header**, feature 1069's re-check of 2026-10-25: a three-line `stopflag.h`
    (`static volatile sig_atomic_t`, two `static inline` functions) a program ships, open to a program
    by panel 114's **R2**, ratified 2026-09-06, whose spec sentence *"The header may be one the program
    ships beside its own file, so a difference between machines is written in C, where `#ifdef`
    already exists."* (`docs/panel/114-*.md:183-184`) **never entered the spec** (`grep -n -i ships
    spec/heroes-spec.md` prints nothing; the critic's `git log -S` over the spec found none), and
    closed to a **package** by panel 234's R6 (*"no package ships platform C of its own"*, `grep -n
    'ships platform C' docs/panel/234-*.md`: 421). Priced: R2's sentence appended after line 377,
    +31. **Its trap, measured at the base** (`p246-scratch/a-probes/stop1`, `stop2`, the critic's
    sources): handler and loop in one module printed `drained after 3 requests`, exit 0; the handler in
    a second module, each binding `stopflag.h`, printed **`drained after 1000 requests`** (the probe's
    cap), exit 0: the handler set its own unit's flag and the loop read the other's, so without the cap
    Ctrl-C is swallowed and the compiler says nothing.
  - T1 with S1 together: +143; T1 with S3: +131.

- **B3** (feature 925, with feature 893 and task 1072). **A connection is a handle, not an `i64`**:
  panel 234's R3, ratified 2026-11-06 (`sed -n 321,324p docs/panel/234-*.md`): *"a descriptor (a POSIX
  `int`, a winsock `SOCKET`) is wrapped in C as a struct behind a pointer so it carries the marks"*;
  at the base `runtime/hero_net.h:45-46` hands out `hero_net_socket *`, *"a group declares it as a
  handle, `record Socket tag hero_net_socket`"*, and promises at 33-36 *"never a read of freed memory
  and never another socket that took its number"*. So feature 1073's *"One connection per thread needs
  only the descriptor, which is an `i64`"* and feature 893's *"A descriptor is a number, so it crosses
  as the `i64`"* rest on a premise R3 overturned. Whether T1's rung can serve one connection per thread
  at all, and the three ways out, each priced: **a numbered handle** (the layer gives a socket a
  number and back, which R3's *never another socket that took its number* exists to refuse), **a rung
  that moves a handle** with its marks across the boundary, or **one thread**
  (`docs/roadmap/milestones/M-core-packages.md:35`, *"writable, one thread"*). And copy-on-write's
  `refcount == 1` test-then-mutate (`grep -n 'refcount == 1' runtime/parts/*.c`: `cow.c:44`, `:88`,
  `:93`, `map-write.c:128`): whether anything a program can make cross reaches it under the rung,
  including the shapes panel 113 did **not** try (feature 925: *"`sort` through a function pointer,
  the `eq` and `hash` descriptor walks, and the drop-list drain under contention"*) and panel 111's
  four classes (`docs/panel/111-*.md:125-170`: a shared `str`'s count, `==` on nested arrays,
  copy-on-write's test-then-mutate, the stack guard on a worker).

These spellings are the coordinator's drafts, written to be priced and tested, never a ruling; the
names `hero_stop_catch`, `hero_stop_asked` and `stop_asked` are candidates.

## The verdicts

| seat | part | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|---|
| compiler-engineer | A | **approve** route A with D inside it, as built in `tree-a/heroes-a2` (recovery once, before discovery) | design.md §1.12; §1.1, §1.7 | `selfhost/` +242 −7 (`staging.hero` +62 −7, `leftover.hero` 175, `packages.hero` +5); `runtime/` +115 (`parts/lock.c` 101, `hero_compiler.h` +10, `runtime.c` +4); route D alone +12 −2 | `tests/golden/fetch/` passes with no case text changed, a compiler test names 2453, +330 to +600 net lines at M-core-packages' tag; in Linux arm64, 0 `.fetching-*` after every later run under `heroes-a2` and the edit lost under the base in 1b and 1c | a Linux or Windows phase under `heroes-a2` leaving a dead folder, losing the edit or removing a live folder; `flock` refused outright on a supported file system (then the lockless fallback lands with A); the ledger ruled a fifth input class (then D alone, and A to a full sitting) |
| ffi-pragmatist | A | **approve** route A in its measured shape: the lock **beside** each `.fetching-*` name, `flock` with the identity check, `O_EXCL` births (D inside it), Windows' share-0 delete-on-close open; **object** to the lock inside the folder | §1.11, §4.19, §1.12 | `parts/lock.c` 139 lines, two declarations, a type and a constant in `hero_compiler.h`; no `static`, Rule 3 counting 109 objects as at the base | Linux A-4: 0 violations with the identity check, more than 0 without; A-1 equal to Darwin's on overlay and `/dev/shm` | object if Linux shows a violation with the check, or an `flock` surviving `kill -9`; D alone if the lock file is R12's fifth class, or a mount with local-only locks must never lose a live run's folder |
| compiler-engineer | B | **object**, no veto (no core construct) | §1.12; CLAUDE.md § 12; §1.7 and Part 5 for the veto unused | S1 `parts/stop.c` 46, `hero_os.h` +7, `runtime.c` +1, each guard header +2, `guarded_names.hero` 1; S2 48; S3 S2's runtime with `inventory.hero` +6 −2 and `library_source.hero` +6, a reserved word, 13 emitted lines moved in the smallest program; its (c) `hero_net.c` +2; the numbered handle 14; a `str` door 23 + 6 | without (c), a server blocked in `hero_net_accept` sent SIGTERM under `timeout -k 3 15` ends at 137 with no drain line on Darwin and Linux at the server step; with (c), its drain line and exit 0 | (c) not waking a thread blocked in `accept` (then route (b)); a stop sentence naming `accept`, `heroes run`'s grace and a child waited on |
| ffi-pragmatist | B | **object**, no veto (no ABI broken), to T1 and T2 as written and to S1 to S3 without the layer's own wait | §1.11, §4.19; §1.12 for the stop | `parts/stop.c` 127 lines with the layer's `accept` waiting on it; `spawn-str.c` 41; a program's carry header 4 | Linux: the three between-steps stops still running 2 s after SIGINT; the draft server ended within 3 tenths of a second of SIGTERM; `close-then-connect` refused | approve a T sentence saying what is true, and any S whose runtime part the layer's blocking calls wait on, an inherited ignore kept; for B3, a rung that moves a handle or one thread |
| spec-warden | B1, T1 | **object**, provisional | §1.6 (255, 324 to 330); §1.0; §1.2 | +73 vendored (8417 to 8490), a lower bound; merged into F3 (M1) +58; the clause *"and nothing else crosses"* +5 | W1, W2, W3 | approve when W2 holds and the decision issue names the clause a narrowing of R5 for the author; land arm y's text verbatim, not M1 |
| spec-warden | B1, T2 | **object**, provisional | §1.2; §1.6 | +15 (M2 +12) | W2's second half | arm z at x + 2 or more |
| spec-warden | B2, S1 to S3 | **veto**, not provisional | CLAUDE.md § 2 and design.md §1.0 (Principle 0's burden unmet); §1.2; §1.12 | +70, +65, +58; with T1 +143 and +131 | W4 | W4 falsified on Darwin and Linux, then a priced sentence for a `hero_os.h` door or a built-in; a stop inside the layer is documented in `hero_net.h` and the `net` card at 0 spec tokens |
| spec-warden | panel 114's R2 | **object**, no veto | §1.6; R3's gate; §1.12 | +31 alone, +0 with its named removal | none, a gate | R3's first and second instruments shown, and the two-module flag told or ruled out |
| spec-warden | B3 | **approve** no sentence | `.claude/rules/spec-shape.md`; Part 7 item 13 | 0 | the server step lands with 0 spec tokens on threads beyond B1's | a server that must cross a connection to a worker |
| historian | B, advisory | **object** to S1 to S3 alone and to a program's own header; would approve the flag paired with (b) or (c) | precedent | | | a flag-only stop serving a server blocked in a retried `accept`; a Darwin or Windows page saying a closed listener does not wake `accept` (moves its reading from (c) to (b)) |
| historian | A, input only | **approve** route A with D | precedent | | | a tool that lost a user's data by recovering under a lock; Cargo issue 2615 showing `LOCK_NB` blocking on NFS |
| llm-ergonomist | B1, B2, an experiment | graded by the critic: t1 and t2, x 0 of 4, y 4 of 4, z 0 of 4; t3, x 1 of 2, p, q and r 2 of 2 each | design.md Part 11 | 3.8216 USD | the decision rule registered before any session | a `context: yes` voids a session: none |

## What each seat measured, in short

- **compiler-engineer** (`246-reports/compiler-engineer.md`): the base loses the author's edit
  under a reused process number for `fetch` (1b) and `check` (1c); route D alone (+12 −2) stops
  exactly that and leaves every killed run's folder for ever; route A2 left 0 folders in every
  phase, kept a live run's folder, survived two recoverers 10 of 10 and moved no `fetch` golden
  (`fetch: 48 passed, 0 failed`); recovery inside `staging.begun` tells a false `package_unread`
  (finding A-3), repaired by running it once before discovery; its lock is held by `heroes` alone,
  not by its `git` child (`lsof`). For Part B: every non-`i64` body passes `check` and is refused at
  `build` by `ffi_callback_type`, so `hero_os.h:430-434` is false; `i64`-only was written at
  `ea4a6fda7` with no ruling found; a `str` door shared by count ran 0 ThreadSanitizer warnings with
  the instrument proved live; S1 and S2 as drafted run the leak gate where `exit` skips it, but a
  server in `accept` never stops (alive at `ELAPSED 04:25`), `heroes run` kills a drain at 2000 ms,
  arming takes INT and TERM out of defect 425's forwarding, and a second Ctrl-C does nothing; S3
  reserves a word and moves every program's C; the layer's handle is already a number with a
  generation, and four connections were served one per thread.
- **ffi-pragmatist** (`246-reports/ffi-pragmatist.md`): `flock`, `F_SETLK` and `F_OFD_SETLK` asked
  the same seven questions on APFS, FAT16, FAT32 and exFAT, `F_SETLK` wrong twice; the door bound
  from Heroes with its marks; route A's protocol under load, the lock beside the folder with the
  identity check 0 violations over 13,191 owners, 65 to 76 per 8 s without it, the lock inside 666
  births lost or 4 corpses left; every between-steps stop waits for a connection; `hero_net_close`
  does not wake a blocked `accept` on Darwin and the layer then accepts a connection on the closed
  listener; a runtime stop the layer's `accept` waits on ended a Heroes server 0.2 s after SIGTERM
  and its handle check ran; a `str` rung that copies, 20 of 20 plain and under `--sanitize`; a
  program's four-line carry header moves a lease past its end at exit 0 (ASan's
  `heap-use-after-free`), and moves a socket with its marks 10 of 10.
- **spec-warden** (`246-reports/spec-warden.md`): the base re-measured, 8417 vendored and 11014 real;
  thirteen drafts priced beside the base, vendored only; nine probes of what a reader writes for a thread, the type
  half told in one round by `ffi_callback_type` and the name half by no message; a server asking
  between connections alive 58 s after a SIGTERM its handler caught; S3's reservation refusing a
  program that ships `stop_asked`; panel 114's R2 at +0 with its own removal and its R3 gate
  standing; four predictions registered at 05:06 before any blind result.
- **historian** (`246-reports/historian.md`): every page read on 2026-11-12, each marked `verified`
  or not; seven bullets of the brief's lists found wrong (Erlang's `os:set_signal/2` cannot take
  SIGINT; PEP 703 says nothing of a count of one); the rows this synthesis rests on are re-read below.
- **llm-ergonomist** (`246-reports/llm-ergonomist.md`): twenty `claude -p` sessions run by the
  coordinator's hand, 3.8216 USD in all, every one `context: no`.
- **critic, second pass** (`246-reports/critic-reports.md`): the compiler-engineer's lock order
  loses births and live folders under load, the ffi-pragmatist's does not; three mechanisms carry the
  name "(c)"; the numbered handle and the handle-moving rung are the same two doors, and a number
  dropped leaks at exit 0; the blind seat graded; the base's own compiler tests hold one red test;
  one shared lock per program, run; a Heroes handler that allocates kills its program; and the
  question the sitting did not ask, whether an edit inside a placed tree is the author's work.

## The blind seat, graded by its registered rule

Graded by the completeness critic (`246-reports/critic-reports.md`, the grading table and items 13
to 15), each answer built with its arm's compiler: the base's stage 2 for x, y and z; for p and q
the compiler-engineer's `s1/heroes` and `s2/heroes` (the base's compiler beside the arm's runtime);
for r its `tree-s3/heroes-next` (`sources 06b60f830cca15c0`). Every t3 binary was run with SIGINT
and SIGTERM at their defaults and SIGKILL to its group as the bound, since a program catching SIGTERM
outlives a plain `timeout` (the spec-warden's finding).

| task | x (the base) | y (T1) | z (T2) | p (S1) | q (S2) | r (S3) |
|---|---|---|---|---|---|---|
| t1 | 2 stopped, exit 134, each its own header wrapping `pthread_create` | 2 right | 2 refused, `ffi_unknown_name` (`hero_thread`) | | | |
| t2 | 2 refused, `reserved_word` (C11 `thrd_create`'s `func`) and `ffi_unknown_name` | 2 right | 2 refused, `ffi_unknown_name` | | | |
| t3 | 1 right (a Heroes handler that calls `close`), 1 refused (`sigprocmask` by hand, `empty_record`) | | | 2 right | 2 right | 2 right |

**The registered rules, applied**: over t1 and t2, x 0 of 4, **y 4 of 4**, z 0 of 4: T1's arm is +4
against the rule's +2, so **T1 enters by the rule**, and T2's is +0, so T2 does not. Over t3, p, q
and r 2 of 2 each: **a three-way tie**, so the blind seat names no spelling; x's two answers are
what a reader writes today. Not provisional: every arm had a seat-built compiler. **The two sessions
that saw the folder's path stand**: t1-x-2 and t1-z-1 each answered `context: no`, each matches its
twin's class and error, and 17 of the 20 `context` answers name a working directory, so voiding
the two that said it aloud would leave the same exposure in fifteen; voiding both changes no
decision (x 0 of 3, z 0 of 3, y 4 of 4).

## The resolution adopted

**R1, Part A (defect 2453, the soundness lane): route A with route D inside it, in the
ffi-pragmatist's layout and order** (`ratified 2026-11-12`). Each run's lock file stands **beside**
its staging folder and is taken **before** the folder is made; the folder is removed **while the
lock is held**, and the lock dropped after. It is the one placement measured at 0 violations, 0
births lost and 0 folders left on this Mac (the ffi-pragmatist's `proto.c`, four runs; the critic's
run of it, one) and in Linux arm64 (two runs), the critic's item 1. **The identity check after
locking stays** (`fstat` of the descriptor against `stat` of the path): without it this Mac lost a
live run's folder 65 to 76 times in 8 s, while Linux arm64 read 0 without it, twice (the critic's
item 2), so its necessity is a Darwin measurement and is kept for the price of a `stat`. **A
standing folder name is never removed**, and the next serial is taken (route D, `heroes new`'s rule,
`selfhost/cli/stub.hero:175-176`). **Recovery runs once, before discovery asks a placed tree**, never
inside `staging.begun`, where `check` told a false `package_unread` about a tree the recovery had
just put back (the compiler-engineer's finding A-3). **Every writer of `.fetching-*` is named and
takes the lock**: `add`, `fetch`, and through `selfhost/module/packages.hero:121` (`unplaced.refusal`,
`staging.begun`, `opened`) every verb that resolves a `use <name>/...`, `check`, `build`, `run`,
`test` and `mutate`, with the three openers of a package's own staging folder
(`fetch/git_source.hero:48`, `fetch/index_source.hero:126`, `fetch/add_wanted.hero:87`). **The door
is compiler-private**: `runtime/parts/lock.c`, its declarations in `runtime/hero_compiler.h`, bound as
`selfhost/module/reading.hero` binds that header, so no guard header, no `guarded_names.hero` word and
no promise of panel 232 moves, and the lane holds; a door in `hero_os.h` would be a surface, outside
this lane. Rule 3 of `tests/harness/suite_runtime.hero` judges no new object, the descriptor living in
the block the door hands out or in the caller's ledger (both seats' runs, `runtime: 10 passed, 0
failed`). **The tree the compiler-engineer built, `tree-a`, is not the adopted layout**: its lock
stands inside the run's folder, is taken after the folder is made and is released before the folder
is removed, and the critic drove its `lock.c`, byte for byte, in that order under the
ffi-pragmatist's load: 178 to 233 births lost and 41 to 67 live folders gone with three recoverers,
with 1, 1 and 0 violations; 15 to 20 and 0 to 3 with each run's own sweep alone; the identity
check passing because the file had not yet been unlinked (item 1). The landing builds the
ffi-pragmatist's order into the compiler-engineer's recovery-once placement. **Unrun, and owed by
the landing**: a network file system
(Cargo's source, as the historian read it, says `flock`'s failure mode on NFS is blocking forever even
with the non-blocking flag, citing Cargo issue 2615, which nobody read: a question, which would hang a
`check` in recovery; and a mount whose locks are local, `locallocks` on Darwin and `local_lock=` on
Linux, where A can remove another machine's live run's folder and D cannot, read from manual pages);
the lockless fallback where `flock` is refused outright (owed, unbuilt); and every Windows arm, each
compiled with `zig cc -target x86_64-windows-gnu` and none run: the CI's Windows leg after the push,
the box being the author's.

**R2, the question the sitting should have asked (the critic's item 16)** (`ratified 2026-11-12`).
**An edit inside a placed package tree is the author's work and is never discarded in silence.**
Panel 235 says so of the build: *"The placed tree is checked against its fingerprint at every build,
since a tree inside the program is a folder anything can edit; question 17 counts as met only when
W4's cases pass: `sum`, `version`, `tag` and `path` mistyped, and the placed tree edited"* (its R7,
`docs/panel/235-dependencies-named-by-an-alias-in-angle-brackets-that-the-programs-catalogue-resolves-gos-modules-copied-whole-a-toml-catalogue-listing-every-package-and-no-fetched-code-run-at-build.md:644-646`;
W4 held with `package_unread` at its line 386). A plain `fetch` that ends drops such an edit at exit
0 with no word, on this Mac (the critic's item 16) and in Linux arm64 (the compiler-engineer's
control 1a, `linux-a.log` lines 20 and 28): a defect of its own, `blocking`, drafted for the
coordinator to file (§ Defects the sitting found, 1). Its repair is a refusal that names the edited
files, or a full sitting if it needs a new word: this synthesis does not design it. **So defect 2453
stays `blocking`**: its recovery puts an edit back (the compiler-engineer's phase (d)), and 2453's
landing waits for that defect's ruling on the fetch path alone.

**R3, Part B1 (feature 1073, all five seats)** (`ratified 2026-11-12`). **T1 enters spec § 13 in
the words the blind seat tested** (arm y), appended after F3's sentence at line 377 as tested and
not merged into it, the spec-warden's condition (*land arm y's text verbatim, not M1*): *"A thread
runs a function of one `i64` and gives back one, and nothing else crosses:
`hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` starts it and
`hero_thread_join(handle: i64) -> i64` waits for its answer."* The blind grade: T1's arm 4 of 4
right, the base 0 of 4, T2's 0 of 4 (the critic's items 13 and 14). Of the base's four, two built and
stopped at exit 134 with the C-thread panic (t1-x-1, t1-x-2) and two were refused at `build` (t2-x-1,
`reserved_word`; t2-x-2, `ffi_unknown_name`); the coordinator's ruling said each compiles and panics,
which is true of the first two alone. **Its clause states the rung the runtime holds today, one `i64`
each way**, because a specification that promised `str` would be false today: `check` passes a `str`
body and clang refuses it at `build`, `ffi_callback_type` (the coordinator's `s.hero`, the
compiler-engineer's B1 table, the spec-warden's q1d). **The widening goes to the author as a ruling of
its own**: the two `str` rungs both seats ran clean (the compiler-engineer's, sharing one `str` by its
atomic count, 32 threads and 256,000 count operations, 0 ThreadSanitizer warnings with the instrument
proved live; the ffi-pragmatist's, copying the bytes for the thread, 20 of 20 plain and 20 of 20 under
`--sanitize` on this Mac and in Linux arm64), and panel 113's ratified R5 (*"the scalars and
**`str`**"*, `docs/panel/113-*.md:276`) against journal 034's line 79 and commit `ea4a6fda7`, whose
body believed the checker decided what crosses while the header's prototype narrowed it (the critic's
item 6). Whether to widen the rung is that ruling's, and it changes T1's clause in the commit that
widens it. **Price**: +73 vendored as appended (8417 to 8490, the spec-warden's table), a lower bound;
the real count is owed at the landing (W3 predicts +80 to +110).

**R4, Part B2 (feature 1069)** (`ratified 2026-11-12`). **No stop sentence enters the
specification**: the spec-warden's veto stands, Principle 0's burden unmet, within its seat
(`.claude/agents/spec-warden.md:45-46`, *"or if Principle 0's burden of proof is unmet"*). **The blind
t3 grade is not read as a thesis effect** (each spelling 2 of 2, the base 1 of 2): the base's one
right answer, t3-x-1, is a program's own Heroes handler, the shape the critic measured killing its
program 5 of 5 when the handler allocates (item 12; t3-x-1's handler calls `close` alone), and t3 is
a `sleep` loop, the shape where every spelling's promise happens to be true, not the server the
feature is for; so t3 measured spellings of a route that is unsafe without the stop below. **The stop
lives in the platform layer, the ffi-pragmatist's mechanism**: the layer owns a stop pipe its signal
handler, written in C, writes one byte down; every blocking call of the layer (`accept`, `recv`,
`send`, `connect`) waits on it beside its socket, and returns a stop kind the program reads
(`HERO_NET_STOPPED`, 12, in the draft), so `main` returns and task 1072's leak check runs. It is the
only one of the three mechanisms named "(c)" measured to wake an `accept` blocked on any thread: with
`accept` on a worker and `main` in `hero_thread_join`, it ended at 0.00 s on SIGTERM and on SIGINT,
where the compiler-engineer's (c), `accept` returning on `EINTR` alone, was still running 10 s after
either signal; and closing the listener does not wake `accept` on Darwin (the critic's item 3; the
ffi-pragmatist's `close-then-connect`). One pipe woke four threads each blocked on its own listener
(`wakeall.c`, on this Mac and in Linux arm64). **No Heroes code runs in a handler**: Windows runs a
console control handler on a thread it creates for it (the historian's precedent, below), and a
Heroes handler that allocates kills its program (the critic's item 12). The door's spelling in
`hero_net.h` and its Windows arm (`WSAEventSelect` and `WaitForMultipleObjects` on an event the
console handler sets, compiled and unrun) are the landing's, a feature. **Recorded, not adopted**: S1
with the compiler-engineer's (c); the historian's (b) as a separate descriptor the program polls,
which a program cannot write today since panel 234's R3 keeps the descriptor inside
`hero_net_socket` (the compiler-engineer's B2).

**R5, Part B3 (feature 925)** (`ratified 2026-11-12`). **No sentence** (the spec-warden approved, 0
tokens). The compiler-engineer's run of shapes panel 113 did not try stands as evidence: 8 workers of
3,000 steps each comparing a nested `constant GRID: [[i64]]` with `==`, copying and writing a row of
it, `sort`ing constants and hashing words into a map of their own while the drop lists drained, 0
ThreadSanitizer warnings 3 of 3 and `--sanitize` 3 of 3. **The shapes both seats left untried are the
server step's cases**: `sort` through a function pointer and the `eq` and `hash` walks under
contention where C is the one calling (the ffi-pragmatist, unrun); a `str` shared by count beside one
copied, under ThreadSanitizer (the ffi-pragmatist, unrun); the stack guard on a worker (claimed in
`hero_spawn_enter`, not re-run by the compiler-engineer); and a socket two threads use at once,
which the layer's own `busy` count would serialise (the compiler-engineer, unrun).

**R6, one connection per thread** (`ratified 2026-11-12`). **A socket handle moves into a thread as
a number through two doors of `hero_net.h`**, the compiler-engineer's 14 lines (`hero_net_number`
and `hero_net_of`, 8 in `runtime/platform/hero_net.c` and 6 in `runtime/hero_net.h`): the "numbered
handle" and the ffi-pragmatist's "rung that moves a handle" are these doors (the critic's item 5).
The layer's handle is already `(life << 32) | (index + 1)`, never dereferenced, so panel 234's R3,
*never another socket that took its number*, holds through the number: a stale number is refused
`NOT_OPEN`, measured by both seats on this Mac and by the ffi-pragmatist in Linux arm64, plain and
under `--sanitize`. **The condition the landing owes**: each door marked so the leak check sees a
socket turned into a number and never taken back; the critic measured that case leaking at exit 0, plain and under
`--sanitize`, with nothing on stderr (item 5, `dropped.hero`).

## What a veto compels

- **The spec-warden's veto of S1 to S3** compels what R4 does: no stop sentence in the specification
  and no stop door in `hero_os.h` or among the built-ins; the stop the layer owns is documented in
  `hero_net.h` and the `net` package's card at 0 spec tokens, the spec-warden's own condition. A stop
  spelled in the language returns to a sitting with a thesis effect measured on a server-shaped task
  and its W4 falsified on Darwin and Linux.
- **No other veto was cast**: the compiler-engineer found no core construct in any candidate, the
  ffi-pragmatist no ABI broken; the historian has none, and the blind seat's reading is an experiment.
- **What the Part A seats' conditions would compel**: the ledger ruled a fifth input class under
  panel 235's R12, or a mount with local-only locks required never to lose a live run's folder,
  leaves route D alone as the lane's answer and sends A to a full sitting. Neither seat read the run's
  lock file as R12's class (each reading R12's *"a lock"* as Cargo's per-project lock file, which a
  build reads to choose what compiles), and the author's ratification takes R1 as adopted.

## Disagreements, not smoothed

- **Where the lock stands, and when.** The compiler-engineer's `tree-a` puts it inside the folder,
  after `mkdir`, released before removal; the ffi-pragmatist measured inside as losing births or
  leaving corpses, and read on Microsoft's pages that Windows removes no folder holding an open file. The critic ran the
  compiler-engineer's own `lock.c` in its own order, which no seat had measured: births and live
  folders lost. R1 takes the ffi-pragmatist's order; the compiler-engineer's Windows reason for
  releasing first is answered by the lock standing outside the folder.
- **Whether the identity check is owed.** 65 to 76 lost folders per 8 s without it on this Mac, 0
  in Linux arm64: the ffi-pragmatist's prediction, *more than 0 on both identity=0 runs*, is falsified
  on Linux. Kept as a Darwin measurement.
- **Three mechanisms under one name, "(c)".** The compiler-engineer's (`accept` returns where
  `EINTR` reaches its own thread), the ffi-pragmatist's (the layer waits on a stop descriptor), the
  historian's (the server closes its listener, Go's and Java's shape). The first left a worker's
  `accept` running 10 s after the signal, the third does not wake `accept` on Darwin; the second woke
  it at once. The compiler-engineer's own condition, *"a run in which (c) does not wake a thread
  blocked in `accept` ... which would send me to route (b)"*, was met on this Mac.
- **The numbered handle and the handle-moving rung.** The compiler-engineer says no handle rung is
  needed, the ffi-pragmatist approves a rung that moves a handle: the words differ and the doors do
  not. The spec-warden read a numbered handle as reopening what panel 234's R3 refuses; both seats
  measured a stale number refused, and the critic measured the danger that remains, a number dropped
  and never counted (R6's condition).
- **Two `str` rungs.** The compiler-engineer's shares by count, resting on `str` never being mutated
  in place, a property of today's runtime that no rule states (the critic: `runtime/parts/str.c`
  holds no uniqueness test); the ffi-pragmatist's copies, design.md Part 7.13's *"copying at the
  boundaries"*. Each ran clean on its own shape and never side by side: the author's ruling.
- **T1 appended or merged.** M1, T1 merged into F3's sentence, is 15 vendored tokens cheaper; the
  spec-warden reads it as two half-rules and the blind seat tested only T1 appended. R3 lands the
  tested text, CLAUDE.md § Precedence's *"at their cost too"*.
- **What t3 measured.** The spec-warden: no thesis effect, and t3 cannot see a server. The grade:
  every spelling one right answer above the base. R4 reads the grade as spellings of an unsafe route.
- **W2's argument.** The spec-warden predicted a T2 reader would still guess the function's name;
  all four z answers wrote `hero_thread_spawn` and `hero_thread_join` exactly and failed on an
  invented handle type, `record Thread tag hero_thread`, where the header answers `int64_t`. W2's
  numbers held and its reason did not: T2 leaves open the result's type, and the message a z reader
  meets does not say the handle is an `i64` (the critic's item 14).
- **What recovery owes an edit.** The compiler-engineer's route A puts a killed `fetch`'s edit back;
  the critic measured the next `fetch` dropping it anyway. R2 settles that the edit is the author's.
- **Route (a)**, the signals blocked and asked with `sigpending`: the compiler-engineer measured
  every child started deaf to TERM (`survived`), the ffi-pragmatist a blocked SIGINT discarded where
  the shell had left it ignored and a server in `accept` never reached. Adopted by no seat.
- **Panel 114's R2 sentence.** Ratified 2026-09-06 and never in the specification; the spec-warden
  prices it at +0 with its removal and objects on its standing R3 gate; the ffi-pragmatist and the
  critic measured its first use for a stop a trap across two modules and impossible on Windows. Not
  landed, and no blind arm carried it.

## Conservative alternatives, recorded

- **R1**: the critic's route nobody listed, **one shared lock per program**: every run holds a shared
  `flock` on `heroes.packages/.runs` for its life, taken before it makes any folder, and recovery
  runs only under an exclusive one taken without waiting (0 violations, 0 births lost, 0 folders
  gone over four runs of 3,172 to 4,092 owners; 0 left after one quiet sweep). Its cost: recovery
  waits for a moment no run is alive, so an editor that keeps a `check` always running defers it; it
  owes panel 235's R12 the same answer route A does. And, more conservative still, **route D alone**,
  +12 −2: the measured loss closed and every killed run's leftovers kept.
- **R2**: an edit inside a placed tree read as not the author's work; then 2453's loss is leftovers,
  `adjacent` or `improvement`, and M-core-packages' tag does not wait on it (the critic's other
  branch, item 16).
- **R3**: T1 without *"and nothing else crosses"* (+68; the clause is +5), or merged into F3 (+58),
  neither read by any reader.
- **R4**: S1 with the compiler-engineer's (c); the historian's (b) as a descriptor the program polls;
  and two routes nobody built, an `accept` with a deadline (the layer has no call with one, though
  `HERO_NET_TIMED_OUT` is a kind) and a `sigwait` thread (which meets the compiler-engineer's
  `child2`, a child started deaf, unless `run.c` clears the two signals before `exec`).
- **R6**: **one thread**, 0 lines, `docs/roadmap/milestones/M-core-packages.md:35`'s *"writable, one
  thread"*, which costs a server its concurrency and not its correctness.

## Prior art, per resolution

Every page below was re-read on 2026-11-12 at 07:09 by the synthesis's `curl`, each quoted sentence
found verbatim (`synth/prior/` in the evidence); the historian marked each `verified`. The lines are
the sitting's decision issue's `## Prior art`.

- **R1**: reached first by Nix (a per-process file whose non-blocking write lock succeeds only once
  its owner has died) and by Go's module cache (leftovers removed only under the lock that proves
  their writers gone); route D's refusal of a standing name reached first by git's `lockfile.h`.
  Beside it, recorded here and not as prior art (the historian's own note): Nix's
  `createTempRootsFile` removes a standing file of its own name because "It *must* be stale, since
  there can be no two processes with the same pid", the premise of `staging.hero:48`, applied by Nix
  to contents it no longer needs, where `replaced-N` holds the author's only copy.
- **R2**: none.
- **R3**: none. PEP 554's narrow first rung, widened by PEP 734, is a comparison the resolution does
  not rest on.
- **R4**: reached first by Python, PEP 475 (wrappers retried on `EINTR`, a wakeup descriptor the way
  to interrupt on some signals) and `signal.set_wakeup_fd` (a byte written to wake a `poll`); the
  refusal of Heroes code in a handler taken from Microsoft's HandlerRoutine page (a console handler
  runs on a new thread the system creates).
- **R5**: none.
- **R6**: reached first by Tcl's `thread::transfer` (a channel moved to another thread, the sender
  keeping no access).

The historian's search line asked a grep of earlier sittings for `signal-hook`, `set_wakeup_fd`,
`controlling_process`, `lockedfile` and `temproots` before any line is filed; the critic ran it,
and no earlier sitting names them (its item 9).

## Defects the sitting found

Drafted for the coordinator to file, each with the reproducer of the report that measured it, in
`.claude/worktrees/p246-scratch/defects/`; searched first in the open issues, and no open defect names
any of the five (each draft's `**Origin:**` gives the words searched):

1. `def-fetch-drops-edit.md`: a plain `fetch` drops an edit inside a placed tree at exit 0
   (R2; the critic's item 16), `blocking`.
2. `def-handler-allocates.md`: a Heroes function given to C's `signal` that allocates kills its
   program (the critic's item 12), `blocking`; feature 1069's re-check of 2026-10-25 had left it a
   question.
3. `def-signal-before-claim.md`: a signal reaching a freshly spawned thread before it claims itself
   is refused as foreign, exit 134 in 3 of 5 on this Mac and 10 of 10 in Linux arm64 (the
   ffi-pragmatist), `blocking`.
4. `def-header-object-undefined.md`: an object a header declares and no module defines is
   `internal error: linking failed` at exit 2 (the ffi-pragmatist), `blocking`.
5. `def-c-thread-panic-remedy.md`: the run-time panic for a Heroes function on a thread C made
   names no remedy (the spec-warden), `adjacent`.

**Found and not drafted here**, for the coordinator to weigh: the ffi-pragmatist's candidate defect
1, `hero_net_close` not waking an `accept` blocked on a listener on Darwin and the layer then
accepting a connection made after the close, against `runtime/hero_net.h:93`'s *"A call in flight on
it on another thread ends first"*; Darwin alone, since Linux arm64 read `refused` (the critic's item
3). Searched 2026-11-12 with `hero_net_close accept`, `close wake accept`, `listener closed accept`
and `accept closed listener`: only defect 2489 answers, about a library replacing a name the layer
calls, not this. And the base's own compiler tests: `2108 tests, 1 failed`, defect 2355's test
reading `selfhost/cli/doctor.hero: -dumpmachine`, a line `ef50a7478` itself added, the round's merge
of lane `core-platform` (the critic's item 8): owed to batch 33's gate, no part of this sitting.

## What the landing owes beside the resolutions

- **`runtime/hero_os.h:430-434`** says *"WHAT MAY CROSS IS DECIDED BY THE CHECKER"*, false today: the
  header's prototype decides and `build` tells it as `ffi_callback_type` on the author's line (the
  compiler-engineer and the spec-warden, each *"whatever this sitting rules"*).
- **For R4's stop**, measured false of the S1 draft by the compiler-engineer and owed an answer by the
  adopted mechanism: under `heroes run` a drain past `HERO_RUN_GRACE_MS`, 2000 ms, is killed (exit
  143); arming the stop takes SIGINT and SIGTERM out of defect 425's forwarding, so a child waited on
  runs on; a second Ctrl-C does nothing; and an inherited ignore is kept, as `run.c:518-525` keeps it
  (the ffi-pragmatist's draft keeps it). Two signals and not four, by the ffi-pragmatist's reading
  (`run.c`'s four are passed on to children; SIGQUIT asks for a core, SIGHUP is a hung terminal):
  a reading, not a measurement. Rule 3 owes the draft's two `SHARED_BY_DECISION` lines (the `runtime`
  suite never ran on `tree-rung`), and its Windows `CTRL_CLOSE_EVENT` wait of 4,500 ms of the system's
  5,000 is compiled and unrun.
- **For R1**: a compiler test for recovery over a planted dead folder; the `fetch` form's
  planted-folder cases, which the harness must grow (it has no kill step); the replay's six states, of
  which four were run.

## Left open, as questions

- `flock` on NFS and SMB, two PID namespaces on one bind mount, and every Windows run: unrun.
- Whether a `str` is ever mutated in place, the premise of a rung that shares by count: a property of
  today's runtime, written as no rule.
- Whether `f64` and `bool` are among panel 113's R5 *"scalars"*: no body of either was run on a
  widened door.
- S3's re-blessing of up to 861 emitted files and the `lldb` sessions that pin `#line`s: moot under
  R4, recorded unrun.
- The blind folders' names: every session folder sits under the root's own path by the hard stop,
  and a name saying less than `246-llm-ergonomist/` is a question for the next sheet (the critic's
  item 15).

## Predictions to score

- **compiler-engineer, Part A**: landed, `tests/golden/fetch/` passes with no case text changed, the
  compiler's own tests gain a case naming 2453, and `git diff --numstat ef50a7478..<landing> --
  selfhost runtime` reads +330 to +600 net; checkable at M-core-packages' tag, against R1's adopted
  shape. Its Linux half (0 `.fetching-*` after every later run under `heroes-a2`, the edit lost under
  the base in 1b and 1c): **held**, scored in this sitting from `linux-a.log` (the critic's
  postscript).
- **ffi-pragmatist, Part A**: Linux A-4, 0 violations with the identity check (**held**, twice) and
  more than 0 without (**falsified**, 0 twice); A-1 equal to Darwin's on overlay and `/dev/shm`
  (**held**). Scored in this sitting.
- **compiler-engineer, Part B**: a stop landed without a wait in the layer leaves a server blocked in
  `hero_net_accept` alive under `timeout -k 3 15` (exit 137, no drain line) on Darwin and Linux, and
  one with it prints its drain line and exits 0; checkable at M-core-packages' server step.
- **ffi-pragmatist, Part B**: in Linux arm64, the three between-steps stops still running 2 s after
  SIGINT, the draft server ended within 3 tenths of a second of SIGTERM, and `close-then-connect`
  refused: **held**, all three, scored in this sitting from its `linux-a.log`.
- **spec-warden**: W1, at most one `right` among x's t1 and t2 answers and none refused by
  `ffi_callback_type` alone: **held** (0 right; one failure, t2-x-1's `reserved_word`, in a class W1
  did not list). W2, y at x + 2 or more and z at most x + 1: **held** (y +4, z +0), its argument
  not. W3, T1 at +80 to +110 real on `heroes measure --refresh`: checkable at R3's landing, the one
  paid call it makes. W4, any of S1 to S3 with `hero_net.c:509-512` unchanged leaves the server alive
  3 s after SIGINT on this Mac and Linux: scored on this Mac by the compiler-engineer's `accept1`
  under S1 (alive at `ELAPSED 04:25`), Linux unrun; checkable at the server step. B3: the server step
  lands with 0 spec tokens on threads beyond B1's; checkable at M-core-packages' close in
  `docs/measurements/010-spec-budget-ledger.md`.
- **historian**: no prediction, by its seat.

## Process notes

- **The lanes.** Part A sat in the soundness lane by the author's `2a`: no spec-warden (R1 adds no
  spec token) and no blind seat (recovery prints nothing new: no message, code or exit moved in any
  phase, and `fetch: 48 passed, 0 failed`). What the lane gave up is a reader-facing question, and it
  came anyway, from the critic: whether an edit in a placed tree is the author's work (R2).
- **The one paid run**: the blind seat, twenty sessions, **3.8216 USD of the author's 10**, 0.1419 to
  0.3258 a session, every session capped at 0.45 and the runner's bound of 6.50 never reached. All
  twenty answered `context: no`; t1-x-2 and t1-z-1 named the folder's path and are not voided (above).
  No `heroes measure --refresh` ran, so every price above is vendored and a lower bound.
- **A void Linux leg.** The compiler-engineer's first Linux arm64 run (`linux-a.no-curl.log`) stopped
  every `fetch` and `add` at `error[no_curl]`, exit 2, before the phase it meant to judge: a `file://`
  index is read with `curl` (`selfhost/fetch/index_source.hero:1-3`), and the briefs said no case
  needs it. A framing error of the critic's first pass, its item 8, copied into
  `00-shared.md` § Part A and both seat briefs, and named by the critic itself (its second pass, item
  7). The coordinator re-ran the leg with `curl` in the container (`linux-a.log`, its first line
  `/usr/bin/curl`), and that run is the one cited here.
- **The seats' folders inside the repository.** The coordinator's first instruction named the
  session scratchpad under `/private/tmp`; CLAUDE.md § Hard stops writes no file outside the
  repository's root, so every seat worked in `.claude/worktrees/p246-scratch/246-<seat>/`, ignored
  by git (`00-shared.md` § Where each seat works).
- **The machine was loaded.** The reports read load averages from 4.96 (the critic, 00:58) to 69.30
  (the compiler-engineer, 04:57), the critic's second pass 21.52, 51.13 and 63.00; the coordinator's
  brief for this synthesis gives 6 to 110, its upper figure in no report. No duration in any report is
  a measurement of speed, which each says.
- **Slips each seat recorded**: the compiler-engineer stopped its own base run mid-way after editing
  the script under it and re-ran it whole, and its `.git`-less copy was judged by the write hook with
  the trunk's compiler (`.claude/hooks/trees.py` finds a tree by `seed/heroes.c` beside a `.git`), so
  it formatted with its own stage 2 and never ran the trunk's binary; the ffi-pragmatist ran `pkill
  -9 -f '^sleep 20$'`, a pattern that could have matched another session's `sleep 20` (`pgrep` found
  none of its own after), and its first stop driver read process numbers through `head -c`, so one
  probe outlived its `timeout 30` until `kill -9` and that run is discarded; the spec-warden's `pgrep
  -f` pattern matched its own `bash -c` script, so its first SIGINT reached the script, and the program
  then outlived `timeout 20` by 58 s; the historian's `WebFetch` saved a 627.2 KB PDF outside the
  repository's root by the tool's own behaviour, at the path its report names, and left it there,
  removal being the coordinator's or the author's to ask; the critic's first pass wrote the framing
  error above.
- **What moved under this synthesis**: the scratch folder's `build/` caches were removed by another
  session at about 07:15, before the evidence was packed (`evidence.md`).

## Author's verdict

**Ratified, R1 to R6, 2026-11-12**: the author's answer at about 07:12, meant as *ok, I ratify*,
to the coordinator's summary of the six resolutions in six lines, given in the coordinator's
conversation and relayed to this synthesis by the coordinator, recorded as a reading
(`.claude/rules/records.md` § And the record says whose idea it was). The sitting's decision issue,
`Panel 246:`, is filed as a decision taken: DECISION_URL. **What the yes settles**: R1 to R6 as
adopted; the five defects drafted above, to be filed; and R2's ruling that an edit inside a placed
tree is the author's work and is never discarded in silence, so defect 2453 stays `blocking`. **What
it does not**: whether the thread rung widens from one `i64` to one `str` (panel 113's R5 against
journal 034 and `ea4a6fda7`), which R3 left to the author and which stands as its own open decision:
RUNG_DECISION_URL; the predictions above, on their own clocks; and the conservative alternatives,
which stay the author's to choose.
