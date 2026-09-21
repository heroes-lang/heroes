# Panel 172 — the report comes from the crash, and not from the declaration

2026-09-21, M-declared-extents, after step 24 (`abf9a17e`). **Full panel**, five
seats and a completeness critic run **before** this synthesis. Convened on
defect 070, the one open defect of the milestone, without asking: the
milestone's one ask was spent at panel 167 (CLAUDE.md § 4).

Briefs: `docs/panel/172-briefs/` (`00-shared.md`, one per seat, and the six
reproducers beside them). Reports: `docs/panel/172-reports/`, the historian's
transcribed verbatim by the coordinator because that seat has no write tool.

**Procedure, recorded because it happened.** Four seats died on a session rate
limit after the llm-ergonomist reported; relaunched under the author's second
account, three then died on a 600-second watchdog while waiting on background
builds, and were relaunched a second time on their own copies with the
instruction never to wait in a polling loop. The tree was frozen from the
briefs to this synthesis and `git status` shows nothing but the sitting's own
directories. The coordinator ran three measurements of its own during the
sitting, each written below with its command, and re-ran the critic's
prototype before resting anything on it.

## The question, verbatim from the shared brief

Defect 070: *a lease handed to a C function that frees it dies with an empty
stderr and an unstable exit code.* Measured at the brief: `check` 0, `build` 0,
ten runs `133/0 133/0 133/0 133/0 134/0 133/0 133/0 134/0 133/0 133/0`
(exit code / stderr bytes), `--sanitize` *attempting free on address which was
not malloc()-ed*; the control, a lease never ended with C not freeing, exits 134
with 111 bytes, `panic: 1 lease(s) never ended`. Three routes were on the
table: **A**, `consumes` gains a meaning on a `cstr`/`ptr` parameter and
silence still means *keeps*; **B**, three words and silence admits nothing
Heroes made; **C**, write the limit down. The seats were asked to add a fourth.

## The resolution, in one line

**The repair is a run-time report and not a word.** The runtime already keeps
the number that answers the question and already speaks from a signal handler;
fifty lines wire the two together and the filed reproducer goes from **zero
bytes to 250 naming the lease**, ten runs of ten, control unharmed. The word
stays refused where design.md Part 6 refused it six days ago, because the seat
that built it measured that it closes **one of seven** shapes and not the filed
one, and three of the seven free the lease through a function that has **no
parameter to mark**. The spec writes the limit with its falsifier, and the
mechanism goes to a soundness-lane sitting before it lands.

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **verdict** | **object**, no veto: route A is sound, 96 lines, every suite green, and it moves one of 070's four shapes, the one that already failed to compile | **VETO on route B**; **object** to route A as written; **VETO** on any landing that emits address-keyed runtime bookkeeping for a `ptr`/`cstr` | **object**; **veto** if any draft lands without a dated correction under design.md:2667; **veto** on B2 (`DELTA_GATE`) | **approve variant Q** (three words), no veto | **approve** (advisory): `consumes` for the freer, never `borrows` for the keeper; the default undecided by precedent |
| **section** | §1.1 against §1.12 and §1.7 | §1.11, §4.19, design.md:2337-2341 | §1.6, §1.2, Part 6 :2667, CLAUDE.md §13 | spec only | — |
| **cost** | +96/−6 compiler lines, five files, one diagnostic; `suite_layout` largest 218 of 300 | zero bytes of C for a compile-time mark; two `hero_handle_consumed` lines if the handle machinery is reused, exit 134 on a correct `free` | A1 +42 real, A1' +53, B1 +64 (+49 vendored, one under the gate), **B2 +70 (+54 vendored, breaches)**, B3 +42 | — | — |
| **prediction** | at the close, under A as briefed, `lease070.hero` still `check` 0 | under A the ledger needs zero words and `corpus` stays green; under B ten words, `bind_text`'s unwritable | A1' lands at 8269 / `7ddda6086951f675`; `unread-mark.hero:26` green on a lie if unmoved | Q refuses F1 naming `s`, `lent`, `borrows`, `consumes`; P builds and dies; F5 (`lent` on `putenv`) builds under both | of the first five `consumes` marks on a `cstr`/`ptr`, at least one on a non-freer |
| **condition** | approve if A refuses a lease at an UNMARKED parameter too, or 070 is re-filed as the shape A closes | B: one keeps-word true at a declaration where a lease is legal; A: compile-time only, `--emit-c` byte-identical, the three-question note | Part 6 correction + A1' + `unread-mark.hero:26` repaired, in one commit | move to P if Q's wrong-word rate exceeds P's omitted-word rate | a shipping compiler that refuses an unannotated pointer parameter |

**No budget veto**: worst draft 8286 real against 10240. **One veto that
binds**: the ffi seat's, on address-keyed bookkeeping — measured, `veto2.c`,
exit 134 on a correct program that also holds one real handle. **One that would
have bound**: the warden's, on Part 6, moot because the word does not land.

## What the seats found, by input

### The compiler-engineer built route A and it does not close the defect

96 insertions, five files, one new diagnostic (`consumed_by_c`), 676 own tests
and `check` 135, `annotations` 174, `layout` 2, `canonical` 2, `run` 132, all
green in its copy. Then the four shapes, both compilers:

| shape | shipped | route A |
|---|---|---|
| `lease070.hero`, **the filed reproducer**, no word on the callee | exit 0 | **exit 0, unchanged** |
| `lend070.hero`, a freer whose binding falsely says `lent` | exit 0 | **exit 0, unchanged** |
| `consumes070.hero`, the word written | 1 `unread_mark` | 1 `consumed_by_c` |
| `lend070b.hero`, a bare lend into an unmarked freer | 1 `lend_kept` | 1 `lend_kept` |

*A route that reads a word cannot close a defect whose reproducer writes none.*
Three more findings: `selfhost/check/consuming.hero:45` reads `p.consumes` with
no type test, so route A makes `consumed_borrowed_handle` load-bearing on `cstr`
programs with a code word that is then false; the pair `lent consumes` is refused
twice by the prototype and should be `lent_shape` alone; and a C-made pointer
handed twice to a freeing C function is exit 0 at `check` today with no word at
all, so the double give-away is neither opened nor closed by any route here.

### The ffi-pragmatist compiled the world and vetoed route B

Nine ownership shapes compiled against this Mac's SDK (`classify.c`, exit 0;
SQLite 3.51.0, libcurl 8.7.1). `consumes` is true of `free`, `sqlite3_free`,
`freeaddrinfo`; **false** of `putenv` (keeps forever, never frees — and no
keeps-word helps, because nothing ever frees it) and of `strdup`; **undecidable
at the declaration** for `sqlite3_bind_text`, whose third parameter is consumed,
kept or copied by its **fifth** argument (`sqlite3.h:4888-4903`), and
**unwritable** for `curl_easy_setopt`, whose value arrives through `...`
(`easy.h:42`). The one declaration-level keeper found in either header,
`sqlite3_bind_pointer`'s `T`, demands *static storage duration*, which a lease
does not have — so route B's keeps-word would admit exactly the bug it exists to
prevent. design.md:2337-2341 already records panel 124's measurement, *0 of 71
`cstr` parameters in this tree are decidable from a header*; **it re-measures
true**. And ownership is invisible to the C type (`indistinguishable.c`: a freer,
a keeper and a reader of one `typedef`, assigned to each other, clang exit 0), so
it is invisible to the header verification this project guards: a wrong TYPE is
exit 1 with a header as judge, a wrong ownership WORD is exit 0 with no judge.

The seat also measured that **defect 070's shape has a correct spelling today**:
measurement 037's disposer route, re-run at `check` 0, `run` 0, **zero**
AddressSanitizer lines — and that today's `unread_mark` already prescribes it,
in the sentence route A would delete: *the word could be false without anything
noticing*.

### The spec-warden priced every draft and found the sitting inside Part 6

Every price on the real instrument, applied and reverted in its copy: baseline
8216 / 6172 / `2b1556634e73455a` confirmed; A1 +42 real, A2 +46, A1' +53, B1 +64,
B2 **+70 and +54 vendored against `DELTA_GATE`'s 50**, B3 +42. Merge beat append
by four real tokens, again. Then the finding no brief carried: **design.md:2667
is inside Part 6, Rejected permanently** — *a mark (`acquires`, `borrows`,
`consumes`) on a `ptr`, or on any result or parameter whose type reaches no
handle*, panel 150 R3, ratified 2026-09-15 — and CLAUDE.md § 13 says not to go
there. Both routes are that row. The coordinator's brief listed what was not on
the ballot and omitted it. The warden also read `spec/heroes-spec.md:65-67` and
`:370` with its own eyes and found **no contradiction**: reach in space over C's
memory against reach in time over Heroes' bytes, one syntactic position, and
*every draft on the table says "C frees what it is handed", which is the
licence :65-67 already grants* — so none of them names whose memory is at
stake. Its own A1' does, at +11 real over A1.

### The llm-ergonomist, reading only the document, wanted three words

Cold, on `free`, `putenv`, `strdup`: `consumes`, nothing, `lent` — and on
`putenv` its second candidate was `consumes`, the trap the historian then
explained. Under three words it wrote `borrows` on `putenv` by direct lookup.
Ranked cold for the freeing case: **`frees` > `consumes`** > `takes` > `owns` >
`given`, because `frees` is the C documentation's own verb and excludes `putenv`
and `strdup` without reading a sentence. Its argument for three words is the one
the sitting has to answer: *the program that kills the process is exactly the
program a reader writes before thinking, and under one word it compiles*. It
found hesitation 8, which the coordinator measured (below), and read § 3 against
§ 13 as a contradiction, which the warden and the coordinator both settled as
two memories in one position.

### The historian found the word's family and no precedent for the default

Verified, with URLs and read dates in the report: GObject-Introspection's
`(transfer none)` default on in-parameters and its recorded bill, **GLib #1373**,
a parameter that frees under a default that says it reads — defect 070's shape
from the bindings side, repaired by one word on one parameter; Clang's 2010
`ownership_takes`/`ownership_holds`, **two words for the callee side from day
one**, analyzer-only and opt-in; SAL's `_Frees_ptr_` and C28197; Swift's
`ns_consumed`/`cf_consumed` and SE-0377's `consuming`, one word for keeps and
frees because the Swift caller retains no claim; Rust's `into_raw`/`from_raw`,
convention with no annotation; Nim's `sink`, D's `scope` (lent-polarity), Vala's
`owned` with its maintainers' own sentence about wrong words. **No shipping
binding system refuses an unannotated pointer parameter**; the one polarity flip
that shipped, Swift 6.2's SE-0458, is opt-in and warns. Two corrections to the
brief: GI's `container` is not a keeps/frees split, and **`borrows` means *does
not keep* in every precedent**, so using it for *keeps* would be an accidental
departure.

## The coordinator's three measurements, run during the sitting

**1. `lent consumes` is refused today by the rule the routes remove.**
`function f(s: cstr lent borrows)` and `function f(s: cstr lent consumes)` are
both `error[unread_mark]` at `check`, caret on `cstr`; on a handle,
`function f(h: H lent consumes)` is `error[lent_shape]`. The formatter re-prints
the pair unchanged. So the day either handle word means something on a `cstr`,
the contradictory pair parses, both words are read, and only an explicit refusal
stops it — a fact any future widening owes a golden for, and one more reason the
word does not land tonight.

**2. The two sentences are about two memories.** `spec/heroes-spec.md:65-67`
governs what a callee may do to **C's** bytes through an address the program
holds; `:370` governs how long a callee may keep **Heroes'** bytes handed to it.
The document is consistent, and a reader with only the document could not tell,
which is the ergonomist's finding stated as what it is.

**3. The one measurement the critic named as missing.** In a clean copy of
`abf9a17e`: `examples/ledger/db/sqlite.hero:115` rewritten to `text: cstr lent`
and `bind_text` returned from its lease to a plain `text.cstr()`, then
`./heroes check examples/ledger/main.hero` (no diagnostic) and `./heroes run
tests/harness/main.hero -- ./heroes corpus`: **55 passed, 0 failed**. The
engineer is right that the site wanted a lend; the ffi seat is right that this is
a fact about the PROGRAM, which always passes `SQLITE_TRANSIENT`, and not about
the header, where the same declaration with `destructor: free` at another call
would be a lie the compiler believes. **The ledger keeps its lease**, which is
safe under every value of the fifth argument, and its comment already says why.

## The completeness critic, and it changed the resolution

Seven findings, each run (`docs/panel/172-reports/completeness-critic.md`):

1. **The empty stderr was never written.** Five bad-free shapes in plain C on
   Darwin 25.6.0 — interior free, double free, a stack address, garbage, a large
   block's interior — all die with zero bytes of allocator text; `MallocNanoZone`,
   `MallocScribble`, `MallocStackLogging`, `MallocDebugReport` change nothing; no
   crash report is written. *Pointer being freed was not allocated* is a
   sentence from an older macOS. And **133 is SIGTRAP, 134 is SIGABRT**: the
   "unstable exit code" in the defect's title is two platform paths, not
   randomness. The defect's entry is corrected on both counts below.
2. **THE ROUTE NOBODY LISTED, and it works.** `runtime/parts/stack.c:582-596`
   already installs a `sigaction` handler for stack exhaustion and writes from
   inside it with `write(2)`; `runtime/parts/alloc.c:98` already keeps
   `hero_live_held`, the live lease balance. Fifty lines — 48 in `os.c`, 2 in
   `alloc.c`, a SIGTRAP/SIGABRT handler that reads the counter and a flag the
   exit sweep sets — and the FILED reproducer goes from `133/0` to **`133/250`**:

       panic: a C function freed bytes this program still leases — 1 lease(s)
       were live when the allocator refused the free.
         the bytes of a `.lease()` are the program's, freed by `end_lease`,
         and the callee that was handed one frees what it is handed

   Control unchanged at 111 bytes; composes with ASan (both reports print). Two
   measured exceptions: a Heroes panic while a lease is live prints a second,
   false line (the flag covers only the exit sweep), and the handler is silent
   where no lease is live, which is correct and a class boundary. **The
   coordinator rebuilt the prototype's runtime and re-ran it: ten of ten at 250
   bytes, control at 111.**
3. **A negative claim falsified.** The engineer's *the runtime cannot catch it,
   control never returns* cited `str.c:430-444`, which is about READING a freed
   block. The prototype reads a counter in static storage, from a signal, after
   the allocator has refused. Control does return to the runtime, through the
   mechanism the runtime already installs.
4. **Three shapes beside the four, all `check` 0 under both compilers, all
   133/0.** A callback C invokes with the leased pointer (measurement 037's own
   R5 construct with a lease in place of the `malloc`), **a later call that
   frees what an earlier one stashed** — `later_free()` has no pointer parameter,
   so there is nowhere to write any word — and an `@out: cstr` C fills and
   frees. The prototype names the lease on the first two and is silent on the
   third, whose pointer is C's. **`b_later.hero` decides the vocabulary
   question**: a vocabulary on parameters cannot reach it. And `b_never.hero`, a
   consuming parameter of a function nobody calls, is `unread_mark` today and
   **exit 0 under route A**: strictly less checking than today.
5. **The engineer's approve-condition (1) is route B's price under A's name.**
   Not one of the parameters the 12 shipped lease sites reach carries any word,
   so refusing a lease at an unmarked parameter refuses all twelve, `bind_text`'s
   included — the parameter the ffi veto is written about.
6. **Re-measurements**: the four shapes reproduce; 037 reproduces exactly and
   route A leaves it at 0; the spec baseline reproduces; A1' repriced at **8268**
   real with digest **`34270e866110cfde`** ≠ the warden's `7ddda6086951f675` —
   so that prediction names the wrong instrument, because the digest follows the
   line wrapping and the count does not, and the digest is what `suite_spec`
   goes STALE on.
7. **The contradiction** between engineer and ffi on `bind_text`, with the
   measurement that settles it named — which the coordinator then ran (above).

Its one question: *three of seven measured shapes free a leased pointer through a
function with no pointer parameter to mark, all `check` 0 under the route being
adopted, while a fifty-line runtime prototype names the lease in every one where
a lease is live — what is the measured reason the report has to come from the
declaration rather than from the crash?* **The sitting has no such reason.**

## The disagreements, stated plainly

**Two seats approved a word and two objected to it, and the objections are the
ones with measurements.** The ergonomist's case for three words is real — the
unthinking program compiles under one word — and it is answered not by a better
word but by the runtime: the unthinking program still compiles and now dies
saying what happened, in every shape, including the three no word can reach.
The historian approved `consumes` for the freer and its own table shows why the
seat that compiles objected: in every precedent the word means *takes
ownership*, keeps included, so the reader the historian describes writes it on
`putenv`, and the ffi seat measured that a false ownership word has no judge.

**The engineer and the ffi seat disagreed about `bind_text`** and both were
right, on different subjects: a program fact against a header fact. Settled by
running it (coordinator's measurement 3).

**The engineer and the critic disagreed about the runtime**, and the critic ran
it. The engineer's report is otherwise the sitting's spine: it built the route
the brief proposed, measured that it does not close the defect, and said so
against its own work.

**The warden's price prediction named the wrong instrument**, the critic found,
and it is recorded here because a prediction that can pass on tokens and fail on
the digest is exactly the kind CL-076 warns about — an instrument watching
itself. The warden's Part 6 finding stands untouched by it and was the sitting's
most consequential correction to the coordinator.

## The resolution — `provisional — author ratification pending`

1. **The runtime names the live lease when a C function frees it.** The
   critic's fifty lines, re-run by the coordinator: a SIGTRAP/SIGABRT handler on
   the mechanism `runtime/parts/stack.c` already ships, reading
   `hero_live_held`, guarded by a flag the exit sweep sets, chaining to the
   previous disposition. It reports the LEASE class — every shape where a lease
   is live at the crash, including the callback and the later call — and says
   nothing where no lease is live, because that pointer is C's. **It lands after
   a soundness-lane sitting** (panel 173, `compiler-engineer` and
   `ffi-pragmatist`) on four questions the critic left unrun: the panic path
   (a Heroes panic with a live lease must not print the second line), the
   interaction with `stack.c`'s handler, **Linux** (the signal and glibc's own
   message are platform facts and are run on the platform, `.claude/rules/platforms.md`),
   and whether `hero_stack_blame` can name the C function as well as the lease.
   Each shape gets a `tests/golden/run/` case; the Linux leg runs under
   `--sanitize` (CL-055).
2. **The word does not enter today.** `consumes` stays refused on a `cstr` or
   `ptr` parameter, and design.md Part 6 :2667 stands as written. Measured
   reasons, in the order of the contract's precedence: the compiler does not
   need it (Principle 0: no binding the compiler makes hands a lease, and no
   `consumes` sits on a non-handle in `selfhost/`); it closes one of seven
   measured shapes and not the filed one; three shapes have no parameter to
   mark; it makes `tests/golden/check/unread-mark.hero:26` a legal false
   declaration with no judge; it removes a check (`b_never.hero`); and the seat
   that compiles objects that it deletes a true sentence to admit an
   uncheckable one. **What would change this**: Part 6's own falsifier — a
   program that is correct, needs the mark, and has no other spelling. Defect
   070's reproducer is a program that must be refused, and the correct program
   beside it has a spelling today (item 3).
3. **The spec writes the limit, naming whose memory.** § 13 gains one sentence
   after the lease sentence, priced at landing on the real instrument and held
   under the warden's A1' ceiling of +53 real, merged rather than appended,
   saying: a lease reaches any parameter because C may read it or keep a copy; a
   C function that FREES what it is handed frees the program's bytes, which no
   declaration can say where the disposer is an argument, so the runtime names
   the live lease at the crash; and bytes C must own are allocated through the
   header's own allocator and handed over with their disposer. Its falsifier,
   CL-005's requirement, is the ffi seat's first question: *does the
   documentation name an argument that disposes of this pointer?* — if it does,
   no word on the parameter is true.
4. **Defect 070 closes when item 1 lands, as filed and corrected.** As filed:
   *an empty stderr and an unstable exit code* become 250 bytes naming the lease
   and two named signal paths. Corrected underneath, under its date: the
   allocator on this platform prints nothing for any bad free; the class is
   seven shapes and not four; three of them are unreachable by any word on a
   parameter; the runtime's report covers the lease class and not a C-made
   pointer freed twice, which is filed (item 6). The record follows CL-078: the
   defect is what the finder saw, and the shapes beside it are where what it is
   became visible.
5. **design.md is corrected once, underneath :2340**: a lease into a freer is a
   FIFTH shape — the fourth case's memory reaching the third case's word, bytes
   Heroes made handed to a C function that frees them — and the runtime's report
   is its instrument. No correction under :2667, because the row stands.
6. **Filed**, in `docs/work/milestones/M-declared-extents.md`: a C-made pointer
   handed twice to a freeing C function is `check` 0 today (the engineer's
   `p10`, the critic's `b_out`); whether § 13 owes the sentence that a pointer C
   made is C's to free once, and whether the runtime's report can reach it, is
   unmeasured.
7. **The ledger keeps its lease** at `examples/ledger/db/sqlite.hero:355`, and
   the finding that `lent` plus a lend is `corpus`-green there is recorded as a
   program fact and not a header fact.

**What conservative would have been, recorded so the author can choose it**
(CL-040): **route C alone** — the limit written into § 13, no runtime change,
defect 070 closed as a documented limit with 037's disposer route as the
correct spelling. Zero lines of C, one sentence, and the filed reproducer still
dies at 133 with nothing on stderr. It is refused because the fifty lines are
measured, cheap, reach the three shapes no sentence reaches, and turn a silence
into a report, which is what CLAUDE.md § Precedence rank 3 buys before anything
else. **And what the brief proposed, refused on measurement**: route A, the
word, 96 compiler lines closing one shape of seven, with a legal falsehood and
a Part 6 amendment attached.

## Predictions to score, at the M-declared-extents close

| seat | prediction | how it is scored |
|---|---|---|
| compiler-engineer | at the close, under route A as briefed, `CPATH=docs/panel/172-briefs ./heroes check docs/panel/172-briefs/lease070.hero` exits 0 | route A did not land; scored on the route that did: the command exits 0 (the program is not refused) and the binary prints 250 bytes. **Correct in substance**: the defect is not closed at `check` |
| ffi-pragmatist | under A the ledger needs zero words and `corpus` stays green; under B ten words and `bind_text` unwritable | neither landed; the coordinator's measurement 3 scores the `bind_text` half: a word IS writable for this program (`lent`, `corpus` 55/0) and is a program fact — **partly falsified, as the seat's own falsifier names** |
| spec-warden | A1' lands at 8269 real / `7ddda6086951f675` | not landed; the critic's repricing already read 8268 / `34270e866110cfde`, so the digest half would have failed — **lapsed, and the instrument named was the wrong one** |
| spec-warden | `unread-mark.hero:26` green on a lie if route A lands unmoved | route A did not land; the line stays refused. **Moot, and true as a conditional** |
| llm-ergonomist | Q refuses F1 naming the three words; P builds and dies; F5 (`putenv(string: cstr lent)` fed a lend) builds under both | Q and P did not land; F5 is scored on today's compiler at the close: it builds, and the runtime's report is silent because no lease is live. **The residual class the seat named survives, as it said** |
| historian | of the first five `consumes` marks on a `cstr`/`ptr`, at least one on a non-freer | no such mark can be written: **lapsed**, the route was not taken |
| completeness critic (its prototype's numbers, carried to panel 173) | the filed reproducer at 133/250 ten of ten; control at 134/111; a Heroes panic with a live lease prints a second line until the panic path sets the flag | scored at the landing of item 1 |

## Author's verdict

**RATIFIED 2026-09-21**, in one act with panels 171 and 173, in the author's
words: *then ratify all the DECIDE items.* Recorded as a ratification given in
conversation on the coordinator's summaries in that session, not as a reading
of this file, and **not `by delegation`** (CL-058).

**So the word does not enter the language and design.md:2667's Part 6 row
stands as panel 150 wrote it.** What replaces it is the run-time report of
item 1, judged by panel 173 and landed at step 28, and the sentence of item 3
carrying its own falsifier. The conservative option this sitting recorded —
route C alone, the limit written and no runtime change, with the filed
reproducer still dying in silence — was **not** taken.

**CORRECTION, 2026-09-21, by the author, within the hour.** The author read
this file. The paragraph above is wrong where it says *not as a reading*:
CLAUDE.md § 4's default is that the author reads every sitting, and a
ratification is recorded as a reading unless they say otherwise. The
coordinator read CL-058 as a rule about humility, which is the reading that
paragraph exists to forbid.

What this section said while the item was open, kept because a record is never
rewritten: *Pending: `docs/work/DECIDE.md` carried this sitting as `panel 172`.
The direction — a run-time report rather than a word, the word left refused,
the limit written — was the sitting's provisional resolution and work proceeded
on it: panel 173 in the soundness lane, on the fifty lines.*
