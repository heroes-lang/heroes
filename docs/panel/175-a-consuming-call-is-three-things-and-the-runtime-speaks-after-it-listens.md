# Panel 175 — a consuming call is three things, and the runtime speaks only after it has listened

2026-09-23, M-agreed-retention, after step 2 (`64c92654`). **Full panel**: both
questions have a sentence of `spec § 13` behind them. Five seats and a
completeness critic. Briefs and every reproducer: `docs/panel/175-briefs/`.
Reports: `docs/panel/175-reports/`, the historian's and the spec-warden's
written out verbatim by the coordinator because those seats have no file write.

**The author's words for this sitting**, both given in conversation on
2026-09-23 and recorded here in English as meant: *proceed, always favouring the
most solid solutions*, and, on the spec's price, *pay all the tokens, without
economising*. The first is CLAUDE.md § Precedence as already written; the second
is its application to this sitting's text, and it removes the race to the
cheapest draft. It does not remove design.md §1.6's payment rule, which asks for
a named removal or a registered prediction per row, and every row below names
its prediction.

**Procedure, recorded because it went wrong.** For a stretch after the seats
launched at 10:24, the coordinator switched its own session into a git worktree
(`heroes-lane-076`, where defect 076 was being repaired). Claude Code's isolation
then refused the running seats' writes to the trunk and some of their shell
calls: the llm-ergonomist wrote its report into the worktree and it was moved,
and the spec-warden, the compiler-engineer and the ffi-pragmatist each report a
stretch of refused calls they did not work around. **The critic reran the
numbers each seat produced and found none bent**, and the worktree's `spec/` is
byte-identical to `64c92654`. The coordinator left the worktree as soon as the
first report said what had happened. Recorded as the coordinator's fault, not a
seat's.

## The question

**Question 1 — defect 075.** `acquires pclose` on `popen`, then `fclose`, is
`check` 0 and `run` 0 on all four platforms: spec § 13 says the call *names the
one that ends it, which the program owes it*, and nothing holds the program to
it. Routes listed in the brief: **A**, the runtime set remembers the releaser;
**B**, the checker refuses the crossing where it can see it; **C**, two Heroes
types over one tag; **D**, A with B or with C.

**Question 2 — the milestone's first item.** A pointer C made, given back twice
through a `ptr`, dies with zero bytes on Darwin and Windows. Routes: **E**, the
crash handler speaks with no lease live; **F**, a sentence in § 13; **G**,
nothing.

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **verdict** | approve A, E2, F · object B, C, G · no veto | object A (one name) · approve **A′** (a set of names, a count per address), D = A′+B, E with three conditions, F · **VETO C** · object G | approve "nothing" for Q1 under A · object the appended clause · **veto the Q2 `ptr` wording** · approve T1 and R1c | object, no veto: Y turns the silent crossing loud and names the `ptr` truth, but *another* aborts an ownership transfer and *is a handle* cannot be spelled for every pointer | approve D with A keyed on a SET, approve E with F · object C, G |
| **section** | §1.7, §1.1, §1.12 | §1.11, §4.19, §1.12, §4.20, Part 6 *function overloading* | §1.6, §1.2, §1.0, §1.12 | the spec alone | precedent, advisory |
| **cost** | A: 18 compiler + 2 stamp, 41 runtime code lines, ABI 22→23, 227 + 7 blessed files and the seed move · B: 91 lines · C: 1 line · E2: 19 runtime lines | A′: 56 + 7 runtime lines, about 10 ns per pair · C: a 26-line module per acquirer class, 56–62 `FILE *` functions | candidate 1 +10 real · candidate 3 +40 · T1 +4 · R1c −1 · M1 +2 | — | fdsan, ASan, Valgrind, AppVerifier: one tag per live object |
| **prediction** | at A's landing `xacquires` still exits 0, 0 B; `selfhost/` +≤30 lines; `emission` 227 failed before blessing | `sqlite_v2.hero` 134 under a one-name mark, 0 under a two-name mark, three POSIX legs | A takes the four reproducers to 134 and leaves `u1` at 0 | ≥25% silent under X, 0% under Y, first-try within 10 points | the first `examples/` pair of consuming functions over one handle type will be interchangeable |
| **condition** | object A if two valid releasers are common; then `acquires a \| b` priced first | drop the set if no needed interchangeable pair; lift C's veto for one declaration taking both types with no wrapper | approve the clause if a reader effect is measured and it lands merged | approve when *another* spares a transfer and *is a handle* is limited to what can be spelled | A with one name is enough only if no bound library has two valid releasers |

## What the sitting measured, and what changed its mind

**Two valid releasers for one acquisition are common, so a mark naming one is
wrong.** The ffi-pragmatist ran nine interchangeable pairs clean under ASan on
two platforms — `sqlite3_close`/`sqlite3_close_v2`, `gzclose`/`gzclose_r`, the
two zstd frees, `pthread_join`/`pthread_detach`, `CGContextRelease`/`CFRelease`,
`closedir`/`fdclosedir`, two OpenSSL pairs, and glibc's `fclose`/`pclose` — and
FFmpeg's `AVIOContext` carries both kinds on one type. Both A prototypes abort
`sqlite3_open` then `sqlite3_close_v2`, a correct program. The historian found
the same lesson shipped in every tool of this class: none keys on one function,
and GCC's analyzer says why in its source.

**And a consuming call that TRANSFERS is indistinguishable from a wrong
release, in the declarations.** The llm-ergonomist argued it from the document
(cJSON's add-to-object); the critic ran it against real json-c 0.19 and real
OpenSSL 3 (`SSL_set0_rbio`, 0 leaks under `leaks --atExit`): all three run at 0
today and **abort at 134 under route A, with no spelling of the transfer that
runs**. Three rules were built to admit the transfer and still refuse
`popen`→`fclose`, each as a full compiler: the ergonomist's rewording catches
none of the brief's reproducers and makes the abort depend on whether `fopen` is
declared, which is the ergonomist's own veto; the other two each fail a real
library shape (a Vulkan-style crossing, OpenSSL's cross-type transfer). The
critic's script over every program's `extern` group found why: the wrong
releaser and the correct transfer are both *a consuming function no `acquires`
names*. `selfhost/check/acquiring.hero:257` already says what follows: *no C
header states which, so the binding author states it.* Panel 147's ffi seat had
said a transfer is marked `consumes` (`147-reports/ffi-pragmatist.md:241`), and
no 175 report cited it until the critic.

**So `consumes` is three things today**: it ends a life, it hands a life into
another value, or it drops one of several references (defect 079 below). Route A
makes the difference load-bearing, and no declaration Heroes has carries it.
Panel 148 split the producer side with `borrows`; nobody had asked whether the
consumer side needs the same split before the runtime starts refusing on it.
That is the question this sitting should have been asked, and the critic asked
it.

**`xacquires` is not a run-time fault at all.** Each module gives the handle
back to the call its own mark names, and nothing crosses: every route leaves it
at 0. Two declarations of one C function disagree, and only a declaration rule
sees that — which is the milestone's second item, exactly.

**Route E is sound only as a composite no seat built.** The compiler-engineer's
E2 speaks after the previous handler has been called, and says *in e.main* for a
call made in `e.run` against an `-O2` library. The ffi-pragmatist's line says
*under*, installs SIGILL and reads the sender, and speaks in a process that
recovers and exits 0. Each is false on the other's shape. The shipped lease line
has both faults already (defects 080 and 082 below).

**Route E's goldens need the sanitiser to speak.** The compiler-engineer said
no harness change was needed; the critic ran seven cases through the real `run`
suite on three platforms and five failed under `--sanitize`, because the handler
is compiled out there and a C `abort` or trap gives ASan nothing to report. With
`ASAN_OPTIONS=handle_abort=1:handle_sigtrap=1:handle_sigill=1` ASan reports on
all three, with `on unknown address` as the portable needle. The gap is older
than route E: panel 173's shipped lease line for a non-free death has no golden
that can pin it on any platform.

**The spec-warden's registered prediction was falsified at the sitting.** It
said A takes `xacquires` to 134; it stays at 0. Its other half, that `u1` stays
at 0, turned out to depend on the allocator rather than the route: byte-identical
C, two binaries differing only in their UUID and signature, 0 and 134. The
critic's `u1_static.hero` reproduces the reuse without the allocator.

## Six defects in what ships, found by the seats and reproduced before filing

Each was reproduced by the coordinator from the seat's own files and filed as
open in `docs/work/DEFECTS.md` with this sitting's commit or the one after it:
**077** a handle given back after C reused its address frees the new one in
silence (spec-warden); **078** `owned` on a `const char **` cell is `internal
error`, exit 2 (spec-warden); **079** a reference-counted handle aborts a correct
program in either spelling (ffi-pragmatist); **080** the lease line names the
function that called the caller (ffi-pragmatist); **081** on Linux x86-64 a C
trap with a lease live dies at 132 with nothing (ffi-pragmatist); **082** the
lease line says the process died above a process that exits 0
(compiler-engineer). And **panel 170's ratified item 8**, *spec § 13 owes the
handle-only rule*, never landed and has no ledger row (spec-warden).

## The resolution — `provisional — author ratification pending`

1. **Defect 075's repair is route A in the compiler-engineer's pair-entry
   shape, keyed on a SET, and it lands only with a consumer vocabulary that
   separates a release from a transfer.** The shape is fixed here: the releaser's
   identity is its C name compared by content (a function's address differs per
   translation unit, measured); it is folded into the set's own entries (a
   second table turned the `runtime` suite red); it is checked before C runs;
   the newest mark wins when C hands an address out again; a mark may name more
   than one releaser, `acquires sqlite3_close | sqlite3_close_v2`. **The spelling
   of the transfer, and of the added reference, is the next sitting's**, because
   no seat tested a word for it with a reader and the ergonomist's veto sits on
   exactly that. So **panel 176 widens**: what a declaration says a C call does
   with a handle — ends it (and with which releasers), hands it into another
   value, drops one reference — and whether two declarations of one C function
   must agree, which is the milestone's second and third items. Defect 075 stays
   open until 176 lands, and so does 079.
2. **Route C is refused on the ffi-pragmatist's veto** (§1.11): a type per
   acquirer class multiplies every `FILE *` binding into modules and wrappers,
   and Part 6's function-overloading row is what forces it. **Route B is not
   adopted**: it catches two of four, A catches everything B does before C runs,
   no Part 11 effect is measured, and D was never built as one compiler.
3. **Route E lands now, as the composite.** The line is written only after the
   previous disposition has been called and has not given control back (E2's
   ordering); it says *under* the Heroes function, never *in* or *called from*;
   SIGILL joins SIGTRAP and SIGABRT; it says who raised the signal from
   `si_code` and `si_pid`, and says nothing about the sender when `si_code > 0`;
   with no lease live it states the signal and that this runtime did not raise
   it, and names C's usual reasons as usual, never as the cause. The Windows arm
   says the same for `0xC0000374` with no lease and is measured on the box at
   the landing. **It closes defects 080, 081 and 082 and the milestone's first
   item's silence.** E2's third sentence (*a handle given back twice is
   caught*) does not land: F1 and F3 falsify it.
4. **The sanitiser is made to speak rather than the golden made silent.**
   `handle_abort`, `handle_sigtrap` and `handle_sigill` are set where a
   program's own `--sanitize` build receives them, `__asan_default_options` in
   the runtime's sanitizer arm, **if that is measured to work on the three POSIX
   legs at the landing**; otherwise in the harness's `run --sanitize`, which the
   critic measured. Either way the route-E goldens and a golden for panel 173's
   non-free lease death carry `!sanitizer: on unknown address`, and none pins a
   signal name (libmalloc's double free is 133 sixteen times and 134 four in
   twenty).
5. **§ 13 lands its true sentences in full**, on the author's word: T1, *two
   records may not name one tag but `void`*, which is the spelling a pointer C
   hands out for the program to give back can take; a sentence pointing the
   reader there, worded so it is true of what can be spelled (`void *`, not
   `int *`, F3), which answers the ergonomist's second condition; the
   reuse-honest wording of *giving one back twice aborts*, since defect 077 is
   open and the stronger sentence is false today; and **panel 170's item 8**, the
   handle-only rule. Each is priced on `claude-opus-5` at the landing and owes a
   ledger row with its prediction. The set grammar and the consumer vocabulary
   land with 176.
6. **Defect 078 is repaired in this milestone** as an ordinary defect: the
   author's own `extern` disagreeing with the header on a qualifier is an `ffi_`
   diagnostic at exit 1, `.claude/rules/c-boundary.md`'s named class.
7. **The blind seat is not blind, and that is a `/panel` amendment the author
   owns.** Reading `spec/**` from inside the repository loads rule files into
   the llm-ergonomist's context by their `paths:` frontmatter, one of them
   carrying token counts, in every sitting. The recommendation put to the author:
   hand that seat a copy of the specification outside the tree, and measure that
   no rule loads.

**What a veto compels.** No Heroes type per acquirer class as the carrier of a
releaser (the ffi-pragmatist on C). No `ptr` sentence worded *is a handle* while
`int *` cannot be one (the spec-warden on candidate 3).

**What conservative would have been** (CL-040): land the compiler-engineer's
route A now, keyed on one name, as prototyped — it passes `runtime` 8/0, `run`
136/0, `corpus` 55/0 and the compiler's 675 — and accept what it costs.
**Refused**, because what it costs is measured: three correct programs over real
libraries, cJSON's shape, json-c and OpenSSL, stop being writable, and a correct
`sqlite3_close_v2` aborts. That trades a silent wrong release for a loud false
refusal, which is §1.11's failure in exchange for §1.12's.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | at A's landing `xacquires` exits 0 with 0 B, five of five; `selfhost/` gains at most 30 non-test lines; `emission` reads exactly 227 failed before blessing | panel 176's landing, in M-agreed-retention |
| ffi-pragmatist | `sqlite_v2.hero` exits 134 under a one-name mark and 0 under a two-name mark on the three POSIX legs, and 0 on Windows | panel 176's landing |
| spec-warden | A takes `xacquires` from 0 to 134 — **FALSIFIED at the sitting** by the critic's rerun: 0, five of five. Its `u1` half is allocator-dependent and is not scored | scored here |
| llm-ergonomist | on Task 1 at ≥20 trials per variant, programs that run with the wrong closer fall from ≥25% under X to 0% under Y, and first-try correctness moves by at most 10 points | M-thesis-harness, the first harness that runs a model on the spec |
| historian | the first `examples/` program to declare two consuming functions for one handle type declares an interchangeable pair, not an exclusive one | the first milestone whose `examples/` gains one |

## Author's verdict

*Pending: `docs/work/DECIDE.md` carries this sitting as `panel 175`. Work
proceeds on the resolution: the landing of items 3 to 6, then panel 176.*
