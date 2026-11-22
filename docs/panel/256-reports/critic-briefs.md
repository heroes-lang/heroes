# Panel 256, the completeness critic, first reading: the briefs before any seat

Written 2026-11-22 from 21:45, as it went. Every command ran in my copy,
`.claude/worktrees/p256-scratch/critic/` (detached at `8aca3cc432a6007daaf416ecf3b1b8564472fa09`,
`git -C <copy> log -1 --format=%H`), or read the briefs folder; the briefs were not edited. No verdict on
any route.

## What was re-run and agreed (no finding)

- The arms: `python3 -I make_arms.py ../../spec/heroes-spec.md` in a fresh folder of my copy wrote the
  five specs, four diffs and `arms.sha256`; `cmp` against the briefs' files printed nothing for all eleven.
  `spec-now.md` is byte-identical to the frozen `spec/heroes-spec.md` (`cmp`, and both sha256
  `15804f3f...c427`).
- t1's two tasks: `python3 -I make_t1.py` in a fresh folder; `cmp` with both briefs' files printed nothing;
  `diff task-t1-w.md task-t1-m.md` prints `19,22c19,21` and nothing else, as `llm-ergonomist.md` says.
- The compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`real 11.24`), its
  `--version` `sources 05186f2b38e77a97`; stage 2 `./heroes build selfhost/main.hero -o heroes-next`,
  exit 0, `real 166.51`, `user 121.19`, `sys 35.92` (a waiting run, no duration); `heroes-next
  --version` reads `heroes 0.6.0` / `sources 031aaabb358218fd`, the coordinator's. `grep -c mailbox
  seed/heroes.c`: 0.
- The probes: `bash run.sh ../../heroes-next` from a copy (`zcritic/q2/`); `diff` of its output against
  `coordinator-run.txt`, process ids masked: **exit 0, every line the same**. The six t2 programs, copied
  with `db.hero`, checked, built and run under `timeout -s KILL 20`: the same six outcomes as
  `t2-programs/coordinator-run.txt`; each code block of `task-t2.md` byte-identical to its `progN.hero` and
  `db.hero` (a `python3 -I` comparison of the seven blocks), and `task-t3.md`'s `db.hero` identical too.
  `blind/brief-session.md` and `brief-verdict.md` are `cmp`-identical to panel 254's.
- Every line citation of § 3 and § 4 read with `sed -n` at the frozen commit, and each holds what the brief
  says: `mail.c` 836 lines, `:234-239` (the POSIX condition), `:456-473`, the three wakers at `:432`
  (send), `:540` (close), `:552` (stop), so *"nothing else can wake it"* holds; `thread_ops.hero` 245,
  `named` 57, `crosses` 99-100, `handle_into_spawn` 101-108, `marked` 175; `handed.hero` 50, `:44`;
  `crossing.hero` 171, `:31`, `:107`, `operation_names` 113-146; `key_pointer.hero:53`; the three
  `instantiations[span.end]` writes (`walk.hero:1790`, `function_value.hero:211`, `waiting_names.hero:126`);
  `mono.hero` 553, its head 18-28; `hero_poll.c` 1531, `:37-42`, `:62-96`; `hero_net.c` 4650, `:44-69`,
  `nudge_all` 897, `hero_mail_hear_stop()` 904; `poll.hero:286`, `:290`, `:372`; `poll.ledger:145`;
  `os.hero:790`, `:807`; `ui.hero:36-38`, `:1647`; `hero_ui.h:331`; `hero_ui_win32.c:2436`,
  `hero_ui_gtk.c:2277`, `hero_ui_appkit.swift:1280`; `ui.ledger:92-98`; the catalogue's TH01 row at 377 and
  its section at 463-474, copied verbatim; `readers.hero:11-15`, `serving.hero:157-171`,
  `workers.hero:11-16`, `:64-68`; `lock_copies.hero:306-332`; `design.md:1982-2040` (the quoted rule at
  2004) and `:3639`; measurement 034's 100 lines and both quoted shapes; panel 155's R2, R3 and 284 lines.
- The counts: `git grep` gave 5 files for `doorbell|net\.pair\(` outside `/demo/`, 41 stop sites in 31
  files, 1 `os.count_signal(`, `poll.make(` in 21 files, `ui.wait(` in 5; the census awk's four lines over
  8,802 tracked `.hero` files; 852 generic functions; 51 files under `selfhost/check/` naming `.generic`.
- The issues (`gh issue view <n> --json ...`, read only): 2693, 2660, 2695, 2688, 2675 open, labels and
  filing days as stated, no comments; every quotation found in its body. Commit `691531908` holds the
  quoted CLAUDE.md sentence and is an ancestor of the frozen commit (`git merge-base --is-ancestor`).
- The spec counts: `heroes-next measure` printed 9,585 / 9,761 / real 12,809 for the frozen spec and the
  maxima 9,812, 9,812, 9,847, 9,824 for `q1`, `gc`, `gi`, `gr`, as the briefs say; Docker answered
  `29.9.3 linux/arm64` and both named images stand.

## Findings

### A. What the seats will be handed that is not the frozen commit

**1. The seats' own definitions and contract are `main`'s, not the frozen commit's.** The main checkout
is on `main` at `17c50eba6` (`git rev-parse --abbrev-ref HEAD` in the main checkout: `main`; `git branch
-v`: `rebel-round 8aca3cc43`). `git merge-base --is-ancestor 691531908 HEAD` there: **not an ancestor**,
so the CLAUDE.md the seats load as their contract lacks the 2026-11-22 sentence § 1 calls *the rule that
decides*. `git diff HEAD 8aca3cc43 --stat -- .claude/agents/`: `ffi-pragmatist.md` and `spec-warden.md`
differ; the frozen `spec-warden.md` adds *"Neither veto holds against the most robust resolution"*, which
`main`'s lacks, so a spec-warden launched from the main checkout carries a budget veto in its own
definition that 00-shared § 1 says it does not hold. `.claude/skills/panel/SKILL.md` differs by 44 lines
(the 2026-11-19 amendment for defect 2589, the blind run's leak protections, is in the frozen copy, `grep
-n 2589` line 193, and absent from `main`'s). Which file a subagent loads is an inference (the project's
`.claude/agents/` from the session's checkout), unrun by a launch; the difference of the files is measured.

**2. The Windows box is not "unreachable from here" by the frozen commit's own rule.**
`.claude/rules/platforms.md:100-103` at the frozen commit: *"A session that needs it starts it itself:
`python3 -I docs/platforms/windows/power-on.py`"* (author, 2026-11-20); `docs/platforms/windows/power-on.py`
stands in the copy (`ls`). `ffi-pragmatist.md` item 3 prices Windows "from the code, the box being
unreachable", item 4 Win32 "from Microsoft's pages". 00-shared § 2 forbids *"no cloud run of your own"*,
and the box is *"Paid by the hour"* (`platforms.md:99`). The two rules disagree; the brief must say which
binds this seat. Windows is where P1's open halves are (item 2's set with no handle, item 3's
`add_signals` `unsupported`), so the choice decides whether they are measured or priced.

### B. Framing facts that read otherwise than the brief says

**3. `poll.add_signals` has no program caller; the "5 sites" are comments and ledger lines.** `git grep -n
'poll\.add_signals' | grep -v 'files/poll'`: `os.hero:870`, `os.hero:907`, `process.hero:1012` (all
`#`), `os.ledger:433`, `:437`. `git grep -n 'add_signals('` outside comments: poll's own definition and
tests (`poll.hero:290,292,507,520`), its binding, the runtime, and `tests/golden/run/the-signal-door.h`.
The one `os.count_signal(` is `examples-rebel/shell/interactive/repl.hero:35`, `.interrupt` in a REPL.
So P1 item 3 (*"graceful shutdown counts SIGTERM ... both shipped"*) rests on a mechanism no server in
the tree uses, and which is `unsupported` on Windows (`poll.hero:286`). Shipped is true; used is zero.

**4. e2 is panel 155's measured case, not a widening found tonight.** `docs/panel/155-...md:47-53`:
*"Seven programs ... through a holder used as a map key — are all `check` 0, `build` 0, run 134 ... The
reason is architectural ... the refusal lives inside the partial type's generated `_eq`/`_hash`"*, read
there as *"consistency not safety"*; and `:144`: *"the same rule aimed at a type parameter used as a map
key deletes two goldens"*. Issue 599 (`gh issue view 599`): the abort is *"panel 061's own deliberate
`hero_panic`"*. So e2's run-time abort is a designed guard, ruled on, and 155's R2 closed `==` alone.
§ 4.2's *"the same hole one rule over"* puts a guarded class beside an unguarded one (finding 5). Whether
the 2026-11-22 rule reopens panel 155's ruling on generic map keys (R2's scope, R3's float key) is the
question to put, with 155's measurement in front of the seat.

**5. Of the `ptr` probes, two cross a thread and two do not.** `p5_ptr_written_generic` and
`p5c_inferred_nongeneric_send` make, send and receive on `main`'s thread alone (read: no `spawn` in
either); what they show is the written-type rule (*"a `(mailbox T)` whose `T` holds a `ptr`"*) not
applied, not a crossing. `p7` (spawn) and `p8` (a thread's answer) are the crossings. § 4.2's *"a `ptr`
through a generic crosses threads with no check at any stage (p5, p7, p8 ...), and so does ... (p5c)"*
should name p7 and p8 as the crossings, which are the shapes the ffi-pragmatist's question 6 needs.
And the handle half: `p3` exits 0 because the spawn's crossing function hands the holder over
(`--emit-c` of p3: `h_0put_23a049fc` calls `hero_cross_put_handle` in `start<Boxed>`); a probe of mine,
`zcritic/q2/k3d_spawn_generic_kept_unclosed.hero` (`main` keeps its copy, the thread never closes it,
`main` then closes), exits **134** with *"a C handle reached the argument `f` of `fclose` on a thread
that does not hold it"*. So the brief's *"stopped at run time by panel 254's holder check"* holds, but
`p3b`'s abort is the consumed-address check, not the holder check; `k3d` is the holder check's witness.

**6. The census's "unrun" is now run: no instance in the tree holds a handle or a `ptr`.** `heroes-next
build main.hero --emit-c` for `examples-rebel/certs` and `examples-rebel/iothub` (exit 0 each):
`grep -c hero_cross_put_handle` prints **0** in both; the instances are `start<Setup>` ×4 (certs:
`nameserver.Setup`, `responder.Setup`, `ca.Setup`, `https.Setup`, whose C structs hold `HeroStr`,
arrays, `i64`, an `Instant`, a two-case `Store` of `str` and a `HeroMail *`) and `start_thread<Start>` ×2
plus `start_thread<(mailbox Told)>` (iothub, `amqp_broker.Start` = mailbox + `Fault`, `hub.loop.Start` =
mailbox + `str`). The two goldens run at `i64` and `()`. So the four functions are live and every
instance is harmless today. By each arm's text (read, not built): `gr` refuses all four (a letter's
value spawned, answered or sent); `gc` refuses the golden's `relay<A>`, whose body sends
`into.send(value)` without `@` (`threads-handles-in-lists-maps-and-generic-functions.hero:13-14`), and
its two calls at `i64` would need a cell and `@`; `gi` refuses none. `Fault`'s fields were not read.

**7. The census misses no generic that names a mailbox or thread type in its signature.** `git ls-files
'*.hero' | xargs grep -n -E '^function [a-z_0-9]+<[^>]*>.*\((mailbox|thread) '`: three lines, all in the
census's four (`stage.hero:137`, `world.hero:244`, `threads-handles-...hero:13`). This matters for `gr`,
whose sentence refuses a `(mailbox T)` naming a letter whether or not the body sends.

**8. Units.** *"`(mailbox `: 148 occurrences in 49 files"*: `git grep -c` sums **148 lines**; `git grep -o
| wc -l` counts **151 occurrences** (49 files either way). *"The lock order, one everywhere: the layer's
lock, then the queues' list lock, then a queue's lock (`mail.c:48-50`)"* drops the fourth rung the cited
lines hold, *"then the handle set's or the thread table's"*; `hero_mail_send` hands a handle over under
the queue's lock (`mail.c:424`, `sent(v)`, reaching `hero_handle_hand_over` at `:748`), so a ring outside
every runtime lock sits after `hero_mail_drop` at `:433`, which the brief's three rungs do not show.
*"0.188959 to 0.347129 each"* for 254's 26 sessions is the 25 graded rows (`grep` of the report's table);
the verdict session cost 6.445551 − 6.077117 = **0.368434**, by subtraction from the report's own sum.

**9. Panel 155's measured obstacle to the call-site route still stands, and the brief does not hand it
on.** 155 (`:150-158`): *"`instantiations` does not record who was called, so the pass needs a second
span-keyed table ... it needs a node-to-declaration map the compiler does not have ... which is 150 of its
284 lines"*. Today: `selfhost/check/state.hero:86` `instantiations: {i64: [i64]}` (a span's end to its
bound types, no callee); `grep -rn "decl_of\|owner_decl\|decl_for\|node_decl" selfhost/`: 2 hits, both
`cli/supplied_paired.hero`, none in `check/`. `compiler-engineer.md` item 3 says *"checked at each
recorded instantiation"* as if the table answered which body's obligations apply. It does not; that is
the larger half of 155's price.

### C. The negative sentences and the option set

**10. "No package can hand a mailbox to C today": held for the two shapes I tried.** A record holding a
`(mailbox i64)` as an `extern` parameter: `error[ffi_type]: \`Holder\` cannot cross the FFI boundary`; a
mailbox passed where `ptr` is asked: `error[type_mismatch]` (`zcritic/q2/n1_*.hero`, `n2_*.hero`, `check`).
Searched only those two; a global is excluded by spec § 4 (*"There are no mutable globals"*) and a
capturing callback by § 9 (*"no anonymous functions"*).

**11. "No `poll.add_mailbox` can be written in Rebel without a change to the language": true of P1's
exact face, false of a face of another shape.** A **forwarder per mailbox**, written in Rebel today: a
thread that receives from `m`, sends each value on into a second mailbox the program drains, and rings a
doorbell the set holds after each one, the library and never the program doing the ring. It is chat's
and notes' hand-written workaround (§ 3.2) packaged once; it needs no spec token. What would have to be
true for it, and what it costs, read and not built: a thread per watched mailbox (panel 254 measured *"the
257th `spawn` exits 134"*, `docs/panel/254-...md:184`, `spawn.c:520-523`), each value queued twice, the program reading the
forwarded mailbox rather than `m`, and the bell read **before** the drain or a wake is lost (the order the
library must own). And for a `T` that holds a handle, the forwarder's own `send` is a send of a letter's
value inside a generic: **defect 2695's shape**, so this route exists only under a Q2 ruling that lets a
generic hand a handle over (`gc` or `gi`, not `gr`). Unlisted in § 3.3; unbuilt by me.

**12. Unlisted: the mailbox exposes a descriptor, as `fs/watch` already does.** `packages/rebel/0.1.0/files/
fs/watch.hero:298-330`, `add_to(set, w)`: *"a wait on the set tells the key readable while an event waits
in the watch"*, through *"a pipe the watch rings on Darwin and Linux, and ... a datagram socket of the
loopback on Windows"*, added with `poll.add_stream`. That is a C-side queue joined to a set,
level-triggered on the queue, **shipped**, by another package than `poll`. As a route for P1 item 1 it
reverses the direction question: the runtime would hand a descriptor out and the layer add it as a stream,
with no watch the runtime calls back into, so R6's *"the runtime never calls the layer"* and the lock
order are not touched. Neither § 3.1 nor § 3.3 cites it; it is the nearest precedent in the tree.

**13. A shape beside the Windows ring, read in the code and unrun on Windows: an outside datagram stops
every `poll.wait`.** `hero_poll.c:369-397`: the Windows bell is a UDP socket **bound to the loopback at a
port the system chooses and never connected**; `:1294-1305` places it in every set's array as entry 0;
`:1477-1479` takes any readiness of entry 0 as `stopped = true`, and `:1504` fails the wait `stopped`
without asking `hero_net_stop_asked()`. Nobody reads the bell (`:37-42`), so one datagram from any process
on the machine to that port would fail every later wait of the program `stopped`. `fs/watch.hero:307-309`
states the same exposure for its own bell (*"a datagram from another program can make the key readable
with no event waiting"*), harmless there. P1 item 3 (*"Windows' `add_signals` made to work by the stop's
own loopback mechanism"*) and 2693's waker (*"a loopback socket on Windows"*) both build on this
mechanism. An inference from the code; the box would settle it (finding 2). `gh issue list --state all
--search` for *loopback datagram*, *bell datagram*, *stop bell Windows* and *datagram from another
program* printed nothing; a search, not proof that it is unknown.

**14. Other routes nobody listed, priced as far as reading goes.**
- **The one wait in the runtime** (Go's network poller sits under its scheduler): the readiness wait
  becomes the runtime's, a mailbox an entry by construction, and R6 stops being the constraint because no
  layer is on the other side. Its price is where `hero_poll.c`'s 1,531 lines live and what a program that
  links no layer member then carries; panel 254 measured mail.c's own addition to `print(1)` at 25 symbols
  and 4,080 bytes (`254-...md:416`). Unpriced.
- **A stop with a grace, the process's own** (for 2660): the stop fails only the waits begun before it,
  or fails waits only after a deadline the program set, so a WebSocket's closing exchange can run after
  SIGTERM with no token and no scoped mailbox. Panel 254's *"a stop drains then holds"* is already the
  mailbox's half of that. Unpriced.
- **A run-time guard for a `ptr` or a `cstr` crossing in an instance** (Q2): the emitter already writes a
  crossing function per instance type, and for `Pointed` it writes `hero_cross_put_raw(b, p,
  sizeof(h_kinds_Pointed))` (`--emit-c` of `p7`), so it knows, after `mono`, that the instance holds a
  `ptr`. That is both the cheapest static check after `mono` (the walk exists:
  `selfhost/emit/crossing.hero:60,77`, `.ptr | .cstr` copied raw) and the guard panel 155's veto condition
  asks to stand behind any give-up. Measurement 034's *"a check of any size goes in a new module"* was
  about `mono.hero`; the emitter's crossing walk is a different seam.
- **A blocked call outside every set** (asked of the ffi-pragmatist in item 3, not of the routes): under
  each route, what ends a worker's `recv` on a socket no set holds; the stop token reaches it, the scoped
  mailbox does not.

**15. "Everything a program waits on" is narrower in § 3.1 than in the tree.** The table names four
waits. `grep -n '^function wait\|^function accept' packages/rebel/0.1.0/files/*.hero` also gives
`process.wait`, `wait_or_stop`, `wait_or_signal` (a child's end; `poll.ledger:154-155`
`poll/process-and-signal-filters` is **refused**), `ipc.accept(within)`, `tls.accept`, `fs/watch.wait`
(which joins a set, finding 12), and the language's own `t.wait()`, a thread's end, which no set takes and
no mailbox reports unless the program sends on one (`threads.hero:24-27`). The seats should be told
whether a child's end and a thread's end are inside the question or named outside it.

### D. Q2's framing, and the instrument that would price it

**16. The frozen text is not silent on a generic; the frozen compiler disagrees with it.** Spec § 11 at the
frozen commit (`spec-now.md:383`): *"a handle, or a value holding one, crosses only through a mailbox,
written `m.send(@v)` ... Anything else is a compile error"*. Nothing there exempts a value whose type a
call gave a letter. Defect 2695 says the same of itself: *"so the rule of spec § 11 is decided for no
instance"*. Read as written, the frozen text refuses programs 3 and 5 and accepts program 2, which is
`gi`'s reading for those three (program 4, `@` on an `i64`, the frozen text does not decide). Two
consequences the briefs do not draw: (a) `llm-ergonomist.md:84-96` grades `now` by the compiler and
calls the text *"silent on a generic"* (`:117-119`), so a session that follows the frozen text is graded **wrong** on
programs 2, 3 and 5; the cell measures agreement with defect 2695, not the text's silence. (b) CLAUDE.md
§ 12, *"Spec beats compiler: the compiler has the bug"*: the question *is `gi` a clarification of what §
11 already says, and only `gc` and `gr` changes to the language?* belongs in front of every seat, since
it decides whether Q2 needs a spec token at all.

**17. The cost of a call-site route measured on `heroes check selfhost/main.hero` would measure almost
none of it.** `git ls-files '<area>/*.hero' | xargs grep -c -E '^function [a-z_0-9]+<'`, summed:
`selfhost` **1**, `packages` 555, `tests` 223, `examples-rebel` 24, `examples` 21. `compiler-engineer.md`
item 7 times the route on the compiler, which instantiates one generic; the instrument is inherited from
measurement 034 (`:86-90`), written when *"a compiler that declares no generics"* (`:95`) made it the only
one at hand. A route whose work scales with instances wants a second clock on a program of Rebel's
(my `--emit-c` of `examples-rebel/certs/main.hero` read `user 100.27`, a build, not a `check`, and on a
machine I did not hold still).

**18. Two defects the probes found have no issue.** `p6b_unit_generic`: the **compiler** aborts at build,
`panic: .must() on an error: none: no descriptor for this type` at `emit/threads.hero:51` (exit 134 of
`heroes build`); `p7`/`p8`: a `ptr` crossing a thread through a generic, no check at any stage. `gh issue
list --state all --search` for *emit/threads.hero*, *unit mailbox*, *ptr crosses*, *never crosses
generic* (since 2026-11-15) printed 2692 and 2695 alone, neither of which names a `ptr` or a unit. Both are
`blocking` by `.claude/rules/verification.md` § Bounded discovery (*"a crash"*, *"a wrong one accepted"*);
the brief folds them into 2695's class without saying whether they are filed, owed, or the sitting's.

### E. The blind seat: what it measures, and what it cannot yet be checked for

**19. The runner, the registration and the tools are not in the briefs folder, so this reading cannot
check them.** `ls blind/`: `brief-session.md`, `brief-verdict.md`, `make_t1.py`, the four tasks and
`t2-programs/`; no `run.sh`, `registration.md`, `RUN.md`, `preflight.sh`, `tools/make_sessions.py`,
`check_folders.py`, `check_area.py`, `scan.py`, `reads.py`, `spent.py`, `order.txt`, `key.txt`; and the
area `p256-scratch/256-llm-ergonomist/` does not exist (`ls p256-scratch/`: `compiler-engineer coord
critic ffi-pragmatist spec-warden`). So `CAP=1.00`, `CEILING=19.50`, the second gate, the 46 folders, the
label draw and the voiding tools are described, not readable. Panel 254's `run.sh` (read in my copy) is
the base; the edits are unread by anyone but their writer.

**20. The rows are narrower than the arms' texts in two places, which grades a legal program wrong.**
(a) t3 under `gc`, row (1): *"one generic `deliver` whose value parameter is `@`"*. `gc`'s text asks only
that a letter's value be *sent* `m.send(@v)`; `deliver<T>(m: (mailbox T), v: T)` with `local @= v` and
`m.send(@local)` is legal under it, closes the `db.Db` once and passes rows (2) to (5), and row (1)
fails it. (b) t3 under `gi` (and `gc`): one generic that takes the send as a parameter serves both kinds
under either text (it sends no letter's value itself); `gi`'s row (1) demands *"says one generic cannot
serve"*, so that program is graded wrong under `gi`, while `gr`'s row names *"takes the send as a
parameter"* as right (under `gr` only where no `(mailbox T)` is written, e.g. a second letter for the
mailbox). Spec § 9 forbids a function value with an `@` parameter, so the delegated send takes `v` by
copy; I did not build either program.

**21. `gc`'s sentence reads two ways, and the rows took one before the seat that judges the text spoke.**
*"a value whose type names a type parameter is always sent `m.send(@v)`"* is a rule on the writing
(`m.send(v)` refused, the row of program 1) or a rule on the meaning (every send of such a value empties
it, program 1 accepted, printing `false`, `7`). `spec-warden.md` item 1 asks for repaired texts in
`spec-w*.md`; the arms are hashed and the rows registered already, and no sentence says whether the run
uses the repaired text, nor whether the rows are re-graded by the compiler-engineer's builds of `gc`, `gi`
and `gr` (item 3 runs the six programs under them) where a build and a row disagree.

**22. `gr`'s cell is easier by construction.** Its six rows are all *refused*, and *"a refusal is right
whatever line or reason it names"* (`llm-ergonomist.md:97-98`); a session answering *refused* six times
scores 6 of 6 whatever it read. `gc` and `gi` hold three and four accepted programs whose every line must
be predicted. D2's *"read as written iff it suffices for at least five of the six programs"* is then not
the same bar across arms. Grading a refusal's reason (does it name the arm's rule) or reporting it beside
the count would make the cells comparable.

**23. The thresholds have no rule for a short cell, and D1's difference is within chance.** *"`right >=
4` of 5"* (D2, D3) is undefined for a cell of four counted sessions, which a voided session or the
ceiling's stop mid-round five makes (*"read on what it has"* says nothing of the bar). For D1, a one-sided
Fisher exact test (`python3`, hypergeometric) gives p = 0.22 for 5 of 5 against 3 of 5, 0.26 for 4 against
2, 0.08 for 5 against 2 and 0.024 for 5 against 1: *"read as the form readers get right more often iff
`E1 >= 2`"* reads a difference chance gives one time in four.

**24. The spend leaves nothing for the resolution arm the rules anticipate.** `CEILING=19.50` of the
author's 20 for the first run; `llm-ergonomist.md:129-132` registers *"any resolution arm the synthesis
adds"*, and panel 254 needed one (its arm-r, 15 sessions, 3.370710 USD, its report `:210`). At the brief's
own upper estimate, 16.0 USD, a 15-session resolution arm at 254's rates (3.37) passes 20. Whether the 20
USD covers it, and what the first run's ceiling should then be, is unsaid.

**25. The spec-warden's four refreshes are the four arms; none is left for the text the sitting adopts.**
`spec-warden.md:16-20`: *"At most four refreshes"*; the arms are `q1`, `gc`, `gi`, `gr`. Item 1 asks for
repaired texts, and the adopted resolution will hold Q1's and Q2's sentences together, whose real count is
not the sum of two arms' (the vendored maxima are not additive either: `q1` +51 and `gc` +51 share the
same paragraph). The resolution's real count would go unmeasured or need a fifth.

**26. No reference program exists for t1 or t3 under any arm.** Panel 254 graded its t1 with
`grade-t1-now.py` (`ls docs/panel/254-briefs/blind/`). Here t1's `poll.add_waker`, `poll.wake` and
`poll.add_mailbox` exist in no package (`grep -n '^function add_' poll.hero`: `add_conn`, `add_listener`,
`add_udp`, `add_stream`, `add_timer`, `add_signals`), so no row of t1 or t3 has been shown attainable by
a program anything compiles. Each is a plain function a stub in Heroes could stand for, so the language
half of rows (5) and (7) (a record holding a mailbox and a `Waker` handed to `spawn`, each worker waited
for once) could be compiled by the frozen stage 2 before sessions are graded against it.

**27. No leak found in what a session's folder holds; the session can still name the briefs.** `brief.md`
is panel 254's, `cmp`-identical; t2's and t3's tasks are one text for every arm; t1's two differ only at
lines 19-22 (finding above); the label alphabet `bdfhjkpqsvxyz` holds no letter of `now`, `gc`, `gi`,
`gr`, `w`, `m`. The area sits under the repository's root, so a session's `Read` of
`.../lane-p256/docs/panel/256-briefs/llm-ergonomist.md` would put every row in its context; that is
caught after the spend by `reads.py` and voided, not prevented (panel 254: 0 of 27 transcripts named a path
outside their folder, its report `:26`).

### F. Two smaller readings of P1

**28. P1 item 1's ring is a call from the runtime into the layer's code, which R6's words forbid and its
measured reason may not.** R6 (`254-...md:413-416`): *"the runtime never calls the platform layer"*,
measured as *"A `print(1)` program ... links no `hero_net` code"*. A watch the layer registers and the
send rings is the runtime calling layer code through a pointer at run time; linkage is kept, the words
are not, and R6's lock rule (*"no queue operation ever takes the layer's lock"*) then binds the callback's
body, which § 3.3 states as a hope (*"whose ring takes no lock of the layer's"*). Finding 12's descriptor
route does not raise the question. Whether R6 is read by its words or by its measurement is for the
seats to say, not to assume.

**29. `q1`'s fifth operation refuses no existing function.** `git ls-files '*.hero' | xargs grep -n -E
'^function number(<[^>]*>)?\('`: 55 definitions; read by eye, none takes a mailbox, a thread or a bare
letter first (the two generic ones, `graphs/internal/layout.hero:30` and `paths.hero:45`, take a map of
`K`). Two already name a handle's crossing number: `net/hero_net.hero:168 number(@s: Socket)` and
`ui/hero_ui.hero:259 number(app: App) -> i64`, the app's number TH01's `hero_ui_wake(app_number)` would
take, so the word matches the tree's usage.

## What the coordinator must repair before any seat starts

1. **Say which contract binds the seats** (finding 1): the main checkout is on `main`, which lacks
   `691531908` and the frozen `spec-warden.md` and `ffi-pragmatist.md`; quote the frozen texts into each
   seat's prompt (or launch from the frozen tree) and tell the spec-warden in so many words that its
   budget veto does not hold here.
2. **Put finding 16 in front of every seat**: the frozen § 11 already refuses programs 3 and 5 and
   accepts 2 for a value holding a handle, generic or not; ask whether `gi` clarifies what § 11 says and
   only `gc` and `gr` change the language, and grade the `now` cell by the text as well as by the
   compiler, or stop calling it *"silent"*.
3. **Settle the rows before the run** (findings 20, 21, 22): which of `gc`'s two readings the rows
   grade, whether the spec-warden's repaired texts replace the arms (and re-hash) or are recorded beside
   them, that the compiler-engineer's builds re-grade a row they disagree with, t3's rows widened to
   every program the arm's text allows, and a refusal's reason graded or reported beside the count.
4. **Put the runner, `registration.md`, `RUN.md`, `preflight.sh` and the tools in the briefs folder** and
   have them read before the preflight (finding 19); today the paid run's mechanics are unread.
5. **Decide the Windows box** (finding 2): `platforms.md:100-103` lets a session start it; § 2 forbids a
   cloud run. Say which binds the ffi-pragmatist, since P1's open halves are Windows'.
6. **Hand finding 13 to the ffi-pragmatist** (an outside datagram stopping every `poll.wait` on Windows,
   read in `hero_poll.c:369-397`, `:1477-1479`, `:1504`), since P1 item 3 and 2693's waker build on it;
   file it or say why not.
7. **Give Q2's cost a second clock** (finding 17): `selfhost` declares 1 generic of 852; time the route
   on a Rebel program too.
8. **Tell the compiler-engineer what panel 155 measured against the call-site route** (finding 9): the
   instantiations table records no callee and no node-to-declaration map exists, 150 of 155's 284 lines.
9. **Reframe e2 as panel 155's ruling** (finding 4), with 155's measurement and issue 599, and ask
   whether the 2026-11-22 rule reopens it.
10. **Correct P1 item 3's footing** (finding 3): no program calls `poll.add_signals`; the "5 sites" are
    comments and ledger lines.
11. **Say whether the two unfiled defects are filed now or are the sitting's** (finding 18: the
    compiler's panic on `(mailbox T)` at `T = ()`, the `ptr` crossing through a generic).
12. **Register what the decision rules do with a cell under five, and what E1 means at n = 5** (finding
    23); **reserve or re-state the budget for a resolution arm** (finding 24); **give the spec-warden a
    refresh for the adopted text** (finding 25).
13. **Add the unlisted routes to § 3.3** (findings 11, 12, 14: the forwarder per mailbox and its
    dependence on Q2, `fs/watch`'s descriptor as the shipped precedent, the one wait in the runtime, a
    stop with a grace, a run-time guard after `mono` in the emitter's crossing walk) and **say whether a
    child's end and a thread's end are inside "everything a program waits on"** (finding 15).
14. **Write a reference program per t1 and t3 arm** against stubs in Heroes, compiled by the frozen stage
    2 where the language half allows it (finding 26).
15. **Small corrections**: p5 and p5c cross no thread, p7 and p8 do, and `k3d` is the holder check's
    witness (finding 5); the census's instances measured harmless, and `gc` refuses the golden's
    `relay<A>` (finding 6); the lock order's fourth rung, *lines* not occurrences, the verdict session's
    0.368434 (finding 8); R6's words against its measurement for P1's ring (finding 28).
