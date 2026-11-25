# Panel 259: no large value or lend is judged given back by its frame's address; what a callback C left held stays held, to a bound a spec sentence tells; and HELD stands

**Resolution adopted: provisional — author ratification pending.** Full panel: the compiler-engineer, the ffi-pragmatist,
the spec-warden, the historian, the completeness critic twice (on the briefs, then on the reports); the llm-ergonomist
registered and not run, its paid run authorised by the author at 09:25 with a ceiling of 20.00 USD
(`259-briefs/llm-ergonomist.md`) and waiting, by its own registration, for the key this synthesis names. Convened
2026-11-25 on lane coroutine-room's `systemic` draft (`d-lane-coroutine-room-1`, *"a repair needs a ruling no rule
reaches"*). Briefs: `docs/panel/259-briefs/` (written at `2cf85ea64`, repaired from the critic's first
reading and re-frozen at `3ec775b66`); reports: `docs/panel/259-reports/`, every seat's programs beside them under
`programs/` (the compiler-engineer's copied from its own tree before this synthesis: `diff -r` against
`.claude/worktrees/259-compiler-engineer/docs/panel/259-reports/programs/compiler-engineer` prints nothing, 53 files).
**Every cell below is at `3ec775b66`**; the synthesis's own runs are this Mac's, with a stage 2 built from that base's
seed (`sources 117362877b136f67`), in `.claude/worktrees/panel259-scratch/synth/`.

## The proposal, as put to the seats

Panel 258's R1 (provisional, issue 2758, built on `main`) heals the large-value room (`frame.c`) and the lend regions
(`lend.c`) by frame ADDRESS: a take, a lend or a thunk's entry gives back every entry of a lower frame on the thread's
own stack. Step 181a (`f767e0d4e`) judges a frame on another stack only where the runtime can name the stack (a
`makecontext` range); a frame inside the thread's measured stack and outside every named range is the thread's own
(`stack_lane.c:51-55`). A C library's own switch onto memory carved from the thread's stack makes that order lie:
`own_carved` prints `body holds 11` for 7 then *compiler bug* on this Mac, and dies by SIGSEGV with nothing printed on
Linux arm64 and x86-64. **The question**: what may the runtime assume about a switch it cannot see, and what is a program
told or promised when C makes one (routes A to G, `00-shared.md` § 6); and step 179a's `HERO_SIGNAL_HELD`, a refusal
about a stack no switch was seen to enter, ratified or refused.

## What the sitting found before judging, and beside it

- **The class is wider than an unseen switch; its one cause is the heal by address.** With the three heal functions
  returning at once, eight corrupting cells run right on this Mac (`own_carved`, `below`, Boost.Context 1.92's
  `bfc_host` and `bfc_below`, `cpu_heap`, `cpo_host`, `cpo_host_told`, `cpo_heap_told`), and on Linux arm64 and x86-64
  where run (the ffi-pragmatist's no-heal table). `cpu_heap`, a copying design over `makecontext` and `swapcontext`,
  corrupts with **every switch seen** (counter `made 2 swap 7`), on all three platforms.
- **A real library and a shipped design reach it.** Boost.Context 1.92's own switch onto a carved stack prints `body
  holds 11` (`bfc_host`, 3 of 3); greenlet 3.5.6 starts each greenlet at a local's address on the thread's own stack and
  copies slices out and in (its source on this disk, read by the ffi-pragmatist; verified at its repository by the
  historian); Tencent libco copies stacks on `malloc`'s memory (historian, verified). One coroutine suffices (the
  critic's `one`: `body holds 10` for 7); so does a thread `spawn_sized` started (the warden's `own_spawned`). A runtime
  that keeps a fact about the thread's stack has met it before: Go 1.21's cgo callbacks broke on a coroutine library's
  stack manipulation, mended by refreshing the bounds at every callback (golang/go issue 62440, the historian, verified).
- **Two facts of the briefs were wrong.** Homebrew's libcrypto 3.5.9 imports `__longjmp __setjmp _getcontext
  _makecontext _setcontext` and **no `swapcontext`** (`nm -u`, the ffi-pragmatist first, the critic again): OpenSSL's
  fibers here switch by `_longjmp`, which mattered (below); Debian's x86-64 build switches by `swapcontext`, its arm64
  build by `_longjmp`. And on Linux the `makecontext` door is the handle, wider than the brief said: `RTLD_NEXT`, the C
  library's handle and **`dlvsym` by all three handles** answer the C library's `makecontext` (`dl3` `state 1`, both Linux
  images), so `unseen_carved` is refused there in `HERO_SIGNAL_REENTERED`'s words about a handler it does not have
  (traced by the critic: rule 6 on coroutine 0's yield), 2705's and 2755's class.
- **Rule 6 lies on this Mac too.** `escape_carved` (a coroutine on a carved stack leaving for good by a `longjmp` to its
  host) is refused in REENTERED's words, exit 134, 3 of 3, on this Mac and Linux arm64; its heap twin runs. No route of
  the room touches it (the compiler-engineer).

## Verdicts

| judge | verdict | section | cost named | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto of R1's heal by address**; would adopt **E with B and the owner's proof** (`rt-ebo6`), built | design.md §1.12 (*"must not segfault and must not corrupt memory"*; *"check rather than assume"*) | runtime only, +288 −7 lines, 0 of `selfhost/`; at `-O2` +1.0 instruction a callback, +958 a lend, about +1,940 a 32 KB take (`cost.sh`, base against `rt-ebo3`) | `own_carved`, `below`, `lend_carved`, `coval_carved`, `give_carved`, `copy_carved`, `copy_jump` as `run` goldens pass on darwin-arm64 and linux-arm64, and `cost_cb` at `-O2` stays within +1% | *"a program in which the owner's proof gives a live entry back (then B goes and E stays alone, with its leaks)"*; a fact that tells unseen abandonment from unseen suspension on every switch |
| ffi-pragmatist | **veto** of a range tell (`arm-t` as drafted, and any tell a binding's correctness depends on) and of any heal by address for a coroutine nobody named; **object** to `arm-r` and `arm-p` as worded | §1.12 (*"any C library must be bindable"*), §1.11, §4.19 | none at the ABI: a new `hero_os.h` entry would be additive (`HERO_RUNTIME_ABI` 32 stands, `heroes_runtime.h:1091-1093`) but a promise for ever (decision 2228) | `bfc.h` needs no added line: `bfc_host` and `bfc_host_01` print right, 3 of 3, plain and `--sanitize`; `cx13` at 5,000 throws then aborts with the room full unless a C++ unwind is made a seen event | a runtime under which every corrupting cell is right with every binding unchanged turns the veto into approve |
| spec-warden | **object**, provisional | design.md §1.6 (the payment rule), §1.2, §1.12 with CLAUDE.md § 12 | every count vendored only: `arm-p` +32, `arm-r` +69, `arm-t` +77, `w1` +18, `w2` +78, `w3` +111 | under `now` at least 3 of 5 t1 readers answer `PRINTS` for q4; `w1`, if landed, at most +25 real | no sentence under a route that keeps the value; `w2` only if a refusal route lands; **ratify `HELD`** from the spec's side |
| historian (advisory) | **approve** a resolution whose correctness never rests on an address order an unseen C switch can falsify; **object** to route C's frame walk as proof and to any route that detects heuristically and continues | precedent | none | none | a production runtime that judges its stack, untold, by a fact surviving linked frames and copy-stack libraries |

The completeness critic gives no verdict; its second reading (`critic-reports.md`) decides the route (below). The
llm-ergonomist's reading is registered (`259-briefs/blind/registration.md`), not run.

## The resolution adopted (provisional)

**R1, the room and the lends: route E alone.** No entry of the large-value room and no lend region is ever judged given
back by its frame's address: every take and every lend is a block or a region of its own, given back by its own give,
in any order and on any thread; no take, give, lend or thunk entry heals anything. It is the compiler-engineer's
`rt-e` (`programs/compiler-engineer/runtime-rt-e.diff`, 43 lines: both asides tagged 0, which `frame_aside.c:99` and
`lend_aside.c` never judge, and `hero_callback_heal` returning at once), **without B**. It replaces panel 258's R1 in its
healing half only (`frame.c`, `lend.c`, `callback.c`, and step 181a's lanes as a judge of what is gone); R1's other half,
the gates' true words, `wait` on a thread C ended and the allocator's signal stop, is untouched. **And every lend region
counts, by its mapped bytes, against the same 1 GiB bound as the room's values, live and held alike**, so the abort that
ends a loop of abandonment is the told one whatever was abandoned (the coordinator's amendment of the synthesis's draft,
after 17:30, CLAUDE.md § 4): **adopted, unbuilt**; the landing builds it and measures `lendjump` and a 20,000-jump lend
loop past the bound (its lend sized to pass it: `lendjump`'s 4 KiB lend maps about 32 KiB with its guard on this Mac's
16 KiB pages, some 640 MB for 20,000, under the bound; arithmetic, unrun). **Run, every cell three runs, plain unless
said, right where nothing else is said:**

- this Mac, the compiler-engineer: `own_carved`, `below`, `lend_carved`, `coval_carved`, `give_carved`, `copy_carved`,
  `copy_jump`, plain and `--sanitize`; `room_order` exit 0 (one run);
- the critic, `rt-e` on the three programs `rt-ebo6` corrupts: `aj_carved` (OpenSSL 3.5.9 through its own
  `ASYNC_set_mem_functions`), `mj_carved` and `cpu_heap`, right on this Mac and Linux arm64, and `mj_carved`, `aj_carved`
  and `cpu_heap` on x86-64 (emulated);
- **the synthesis, this Mac**: Boost.Context's `bfc_host`, `bfc_below`, `bfc_heap`, `bfc_host_01`, `bfc_host_told`; the
  copying designs `cpo_host` (greenlet's), `cpo_host_01`, `cpo_heap` (Tencent libco's), `cpo_heap_01`, `cpo_host_told`,
  `cpo_heap_told`; `aj_carved`, `aj_heap`; the critic's `one`, `nested`, `mj_carved`; the warden's `own_spawned`; the
  blind q1 to q4 and q7 at `E5` and q1 to q4 at `E4`, and t2's design C, each printing its right lines
  (`panel259-scratch/synth/drive1.log`, `drive2.log`). **Not run under E alone**: any Linux cell but the critic's four
  (above, and `qsort_jump` on x86-64); the eight coroutine goldens on Linux; the `--sanitize` cells of the synthesis's
  runs; the whole `run` form.

**Its cost, measured** (`cost.sh` of the base runtime against `rt-e`, `-O2`, four alternating rounds, the first
discarded, instructions retired, this Mac at load about 40, `synth/cost-O2-e.txt`): `cost_cb` 4,063,189,392 against
4,024,473,637, **−1.9 instructions a callback (−0.95%)**; `cost_lend` **+907 a lend (+32.8%)**; `cost_room` **+5,797 a
call of three 32 KB takes, about +1,932 a take (+32.1%)**: the asides' one process-wide lock, `malloc` and slab scan,
the prototype's and not the route's floor (a per-thread aside is unbuilt). The compiler takes no large value
(`grep -c 'hero_frame_take(' seed/heroes.c` prints 1, a string its emitter writes) and lends in 23 functions. **The
emitter owes nothing**: `--emit-c` with the base runtime and with `rt-ebo3` is identical bar `hero_program_sources`
(the compiler-engineer, three programs and the compiler's own); the compiler **running on** a route was measured by the
critic on `rt-ebo6` (`cmp` of its self-emission prints nothing, § A8), and **on `rt-e` by the synthesis**:
`HEROES_RUNTIME=rt-e ./heroes-next build selfhost/main.hero -o heroes-e` exit 0 (`sources 258fa2f04149ec64`), then
`heroes-e` and the base's stage 2 each `build selfhost/main.hero --emit-c`, both exit 0, both 40,230,893 bytes, and
`cmp` prints nothing (`synth/selfbuild-e.log`, 16:53 to 17:06, not timed).

**The price, measured: every abandonment keeps what it held, seen or not.** E alone heals nothing on any event, so a
loop of C leaving a callback's frames without their returning, over values past 4 KiB, fills the room's gibibyte and
aborts where it runs today: `asm_jump` (an unseen jump; the compiler-engineer and the synthesis), `deep_jump` (a **seen**
`longjmp` from 1,200 C frames down; the synthesis, 3 of 3), `jumpco` and `jumpco_own`, and **the two goldens step 181a
added** (`fixedbugs-a-c-longjmp-over-a-callback-inside-a-coroutine-gives-its-room-back`,
`fixedbugs-a-coroutine-left-for-good-holding-a-large-value-has-its-room-given-back-on-its-stack`: 6 of 8 goldens
`golden=equal` on this Mac, the compiler-engineer), `qsort_jump` on x86-64 (the critic), and the blind **q6 at `E5`**
(5,000 `longjmp`s inside a coroutine: base `jumped inside a coroutine: 10 35021`, exit 0, 3 of 3; `rt-e` `panic: stack
exhausted in q6.e5 ...`, exit 134, 3 of 3; the synthesis). Door 13 (`cx13` at 5,000 throws) and door 6 (`bl6_5000`,
x86-64) abort under the ffi-pragmatist's no-heal copy and under `rt-ebo6` (the critic, § A6 and § J); **under `rt-e`
itself they are unrun**, the same mechanism. **The count is readable at every such abort**: `hero_callbacks_unfinished()`
printed at the abort (the critic's `rt-ebo6-gate.diff` applied to `rt-e`) reads **1,261** for `asm_jump` and **2,047**
for q5 and q6 (the synthesis, one run each), as the critic read 1,261 to 2,047 under `rt-ebo6`; **`asm_jump` aborts near
its 1,262nd jump**, which `frame.c`'s own head predicts (*"about 1,260 jumps"*), never *"after about 3,700"*. **And the
lends: E alone reopens defect 2756's leak.** Panel 258's `lendjump` (20,000 jumps over a 4 KiB lend) at the base, exit 0,
3,948,544 to 3,964,928 bytes resident; under `rt-e`, exit 0, **209,207,296 to 336,756,736** (`/usr/bin/time -l`, 3 of 3,
the synthesis), panel 258's own measure of the defect (334,741,504). Under `rt-e` an abandoned lend region stays mapped
and nothing bounds it (`lend_aside.c` keeps a lent record until its give; `HERO_FRAME_ROOM` counts only takes): a loop
of it grows until the system refuses a mapping (`hero_lend_refuse`'s *"the system refused the memory for it"*) or, on
Linux, an out-of-memory kill ends it with no message (both inferences, unrun). **That is why R1 counts the lend regions
against the bound** (above): the end becomes the told abort.

**R2, the price told: a spec sentence and the abort's words.** Without them each program above is a correct program that
runs on `main` and stops, `blocking` by `.claude/rules/verification.md` § Bounded discovery the day E lands (the critic's
reading, § D); with them its end is the promised one. **The sentence** (`259-reports/synthesis-adopted.txt` and
`.diff`), in § 13 on line 428 after `atexit(f: (function() -> ())) -> i32`. and before the handler sentence:

> Where C leaves a callback's frames without their returning (by a jump, an exception or a coroutine it never resumes,
> say), a value they held of more than 4,096 bytes, or holding a `partial` record, and a buffer they lent to C stay held
> until the program ends, counted with the live ones against 1 GiB for the whole program, past which it aborts, saying
> how many callbacks have not returned.

**Its count: claude-legacy 9,966, cl100k_base 10,144, a vendored delta of +90 (base 10,054): a lower bound, vendored
only** (`./heroes-next measure` of the applied copy from the scratch tree's root; no `--refresh`, none authorised; the
draft before the coordinator's amendment, whose buffer *"stays lent"* outside the bound, read +92);
`spec/refusal-words` reads it clean (the warden's `check.hero` over `spec_refusals.hero`, `cmp` equal to the base's) and
line 428's tail from `No function of the program is a signal handler` stays byte-equal (`cmp`). It is the
compiler-engineer's +61 draft with the critic's three false clauses repaired: its class *"a jump or a switch this runtime
does not see"* (false: `deep_jump` is seen and held) is now every frame left without returning; *"stays held until the
program ends"* is true of E, which has no B to give back; *"aborts, saying so"* (false today) now says what the words
must say; its list *"(code of its own, a C++ exception, any jump on Windows)"* became examples, `spec-shape.md`'s closed
list being refused. **The threshold is its price** (the warden): the spec names the emitter's 4,096 bytes and its
`partial` rule (`selfhost/emit/large_value.hero:14-17`) for the first time, so moving either is a spec change. Its
lend clause is true of the amended R1 only, **unbuilt**: under `rt-e` a buffer stays held and no bound counts it.
**The abort's words, unbuilt**: today *"stack exhausted in `<fn>`: the room large values take off the stack is full, 1 GiB for
the whole program"*, which name no cause; where the count is above zero the landing says it, *the room ... is full ...
while N callbacks C called have not returned*, never `HERO_CALLBACKS_LEFT`'s clause as it stands, whose *"C jumped over
their frames ... or ended their thread"* names causes q5 and `cx13` do not have (the critic, § A7); at zero, today's
words stand (line 331, *"running out of stack aborts"*).

**R3, `HERO_SIGNAL_HELD` stands: ratified by reading by two seats, run by none.** The compiler-engineer reads its order
judgment as made only on a range `makecontext` named, so an own switch onto a carved stack cannot make it lie, and its
words (*"may still be a coroutine's"*) true of the one correct program it refuses that the seat could name; the warden
reads its words keeping line 428's *"seen to end"* and `spec/refusal-words` clean. The ffi-pragmatist had no shape that
jumps inside a handler onto a `makecontext` stack; the historian did not search. The only runs are the lane's own,
carried in `b0b2a975a`'s body (148 run goldens forced onto route A on both systems; *"the two fixedbugs-2706 programs now
refused"*).

## Not adopted

- **R1 as built, the heal by address**: the compiler-engineer's veto, measured on three platforms and by a real library
  (*What a veto would compel*).
- **E with B and the owner's proof (`rt-ebo6`), the compiler-engineer's built route.** B's two events hand a live value
  to another frame in three correct programs E alone runs right (the critic, § A1, § B, § J, traced on this Mac and
  arm64): its jump hook reads a C library's `_longjmp` **up** into a fiber carved from the thread's stack as abandoning
  the live frames below (`aj_carved`: `body holds 21`, then *compiler bug*, this Mac; SIGSEGV on Debian arm64; **right on
  x86-64**, whose OpenSSL build switches by `swapcontext`; `mj_carved`, the same switch with no library: wrong on all
  three); its `makecontext` event reads a named range as a stack that starts over while a suspended coroutine's value is
  still in it (`cpu_heap`: the false null-pointer panic, all three). That is the seat's own `condition`, met. Its other
  measured gap: a **seen** jump from more than 512 frame records or 64 KiB below the callback leaks (`deep_jump`,
  `deep600s`, `deep200b`, the hook's two bounds). **B proved by a fact other than an order of addresses** (the save
  point's own identity, which route A already records; the coroutine identity a `swapcontext` carries, which the
  ffi-pragmatist's context-identity copy read for `cpu_heap`) is unbuilt: the open question that would turn E's leaks
  back into heals.
- **B by frame alone (`rt-eb`)**: `copy_jump` exits 134 with *compiler bug*, 3 of 3 (two copied coroutines' frames share
  addresses).
- **Lends left unbounded** (the synthesis's measured draft, `rt-e` as built: its sentence said *"a buffer they lent to C
  stays lent"*, +92): `lendjump` holds 209 to 337 MB at 20,000 jumps and nothing stops the growth, so a loop that keeps
  abandoning lends ends where the system refuses a mapping or, on Linux, by an out-of-memory kill with no message
  (inferences, unrun): a silent death, which robustness does not leave open while the room's bound already gives a told
  one.
- **Route A, a told refusal** (the compiler-engineer's conservative): a detector exists (the asker's chain of frame
  records ends at the coroutine's first record, 5 or 6 steps against 8 or 9) and fires before any wrong value, but
  it refuses six correct programs E runs right (`own_carved`, `below`, `lend_carved`, `coval_carved`, `give_carved`,
  `copy_carved`), corosensei links a coroutine's first frame record to its resumer's on purpose (historian, verified:
  README and `x86_64.rs`, *"Set up the frame pointer to point at the parent link"*), a copying library started at the
  current stack position climbs into the thread's records (greenlet's design; read, unrun), and Windows x86-64 keeps no
  frame records (`callback.c:170-176`, read). The tools that detect a switch nobody told them of do it by a distance and
  write their false reports into their own words (AddressSanitizer past 64 MiB, *"False positive error reports may
  follow"*; Valgrind past 2,000,000 bytes, *"client switching stacks?"*: the historian, verified); a refusal resting on
  a heuristic is the shape the sitting declines.
  No seat built the refusal, so `w2` and every *"aborts, saying the switch was not seen"* clause have no cell.
- **Route C as an identity**: a detector, not an identity a heal can rest on (copying defeats it). **The hybrid** (R1's
  room for chain-climbing takes): cheaper, fooled where C is; unbuilt.
- **Route D, a told switch.** A range tell repairs separate stacks and, **over R1's lanes**, crashes shared ones
  (`cpo_heap_told`, `cpo_host_told`); over `rt-ebo6` and over `rt-e` the same told programs run right (the critic, § A2;
  the synthesis), so under E a tell buys nothing a program needs (the compiler-engineer) and would be a `hero_os.h`
  promise for ever. The sanitizer variant reaches `--sanitize` builds alone; an identity tell repairs told shapes over
  R1 and leaves the untold wrong. Telling is opt-in where precedent has it (Boost.Context tells sanitizers only from its
  `ucontext` backend, on request: historian, verified).
- **G1 retire-not-reuse, G2 a static tell, G5 a cookie**: read, unrun; G4 a hardware shadow stack: absent here, measured.
- **`arm-r`, `arm-t`, `arm-p` as drafted**: false clause by clause (`ffi-clauses.md`; the compiler-engineer's § 10).
  **`w1`** (*"A switch of stacks C makes changes no value a function holds, wherever the stacks lie"*, +18 vendored):
  true under `rt-e` for every shape run, silent on the price; § 3 already promises it (*"Every value behaves as an
  independent copy"*), so restating it invites doubt where it is not restated (the warden): it is the **rival**, before
  the adopted sentence. **`w2`, `w3`**: texts for routes not adopted. **The compiler-engineer's +61 draft**: three false
  clauses (the critic, § D), repaired above.

## Disagreements, stated plainly

1. **Which route.** The compiler-engineer would adopt E with B and the owner's proof; the critic's runs found B's two
   events corrupting three correct programs E alone runs right. **The synthesis takes E alone, by the seat's own
   condition**, and records B as what the compiler-engineer supports and the leaks it would have closed (the two
   goldens, `jumpco`, the loops of seen jumps).
2. **Whether a sentence is owed.** The warden: none under a value-keeping route, *"nothing is lost"*. The
   compiler-engineer: one, for the price. Measured (the critic, § D, and the cells above): under E correct programs that
   run today abort, and under `rt-e` the lends grow without a bound. **The synthesis adopts the sentence**: the robust choice tells a
   reader the end the program will meet; no sentence would leave each such program a `blocking` defect.
3. **The abort's words.** `adjacent` (draft 2; the compiler-engineer's *"true of the room"*) or a false message, 2705's
   class (the ffi-pragmatist). Under E the words reach every abort of the price, not door 8's alone, and the cause is
   countable at each: **they are rewritten whatever their class** (R2).
4. **The ffi-pragmatist's veto of a range tell** was measured against R1's lanes, not against E (the critic, § C); its
   other half, no heal by address for an unnamed coroutine, is what R1 above adopts.
5. **Whether the copying class needs a told identity.** The ffi-pragmatist: only an identity a switch carries separates
   two coroutines on one region. Under E nothing needs separating: `cpu_heap` and `cpo_host` run right untold and
   `cpo_heap_told` right though told (the critic and the synthesis), so the identity is B's open question, not R1's.

## What a veto would compel

**The compiler-engineer's veto of R1's heal by address stands, measured**: under a switch the runtime does not see onto
memory of the thread's stack, or a library that copies stacks, it prints another frame's bytes (`body holds 11` for 7,
`body holds 1001` for 8, `coroutine 7` for 10, this Mac; Boost.Context 1.92's `bfc_host`) or dies by SIGSEGV with nothing
said (Linux arm64 and x86-64). It compels the runtime to stop judging an entry gone by its address, which R1 above does;
**its built answer, `rt-ebo6`, is not adopted** (§ A1). **The ffi-pragmatist's veto** compels that no binding's
correctness depends on telling the runtime anything and that the runtime stop giving back by address and take
order-free (both R1 above, measured right with every binding unchanged on this Mac), true words for the abort (R2),
and, for door 13, the unwinder made a seen event (panel 258's R2, pending); **its range-tell half stands only as far as
the critic says**: a crash measured over a heal that judges by range, which E removes.

## The conservative resolution

**R1 as it is** (panel 258's heal by address with step 181a's lanes), no sentence, the lane's drafts filed, and binding
authors told outside the spec to give each coroutine a `malloc`'d stack of its own. **Robustness cannot take it**: the
vetoes measured it corrupting values on three platforms, through a real library (Boost.Context 1.92), through shipped
designs (greenlet's on the thread's own stack, which chooses its own stacks so no binding can avoid it; Tencent libco's
once told), and with every switch seen (`cpu_heap`), which design.md §1.12's first sentence forbids. The
compiler-engineer named route A as its conservative; it refuses six correct programs E runs right.

## What is open and unrun

- **The whole `run` form (1,044 cases) under E**, and the eight coroutine goldens on Linux under E: unrun; the two step
  181a goldens' `.expected` are rewritten by hand at the landing (they abort under E on this Mac).
- **The lends under the bound** (R1's amendment): unbuilt; the landing builds it and runs `lendjump` and a 20,000-jump
  lend loop past the bound on all three platforms. Two costs it owes a measurement of: a correct program whose **live**
  lends and large values pass 1 GiB together now aborts where it runs today (unmeasured; whether a program
  of the tree lends so much is unsearched); and the count is a region's mapped bytes, page-rounded with its guard page, which a reader of *"a buffer"*
  may not predict (the region's layout, `lend.c`, read).
- **E's single process-wide lock under an asynchronous jump** (a handler that jumps out while a thread holds it: a hang
  where today there is a refusal): a question, unrun (the critic, § H 2).
- **The abort's new words**: unbuilt; the count they read is measured (1,261 to 2,047).
- **Doors 6 and 13 under `rt-e` itself**, `deep600s` and `deep200b` under `rt-e`: unrun (no-heal and `rt-ebo6` cells).
- **Windows**: every cell of every route (no box). The historian's reading of Boost's Windows switch (the thread block's
  four stack fields saved and restored unconditionally) is at `develop`, not the 1.92.0 tag.
- **Real token counts**: none; every count here is vendored.
- **The blind reading**: registered with its two arms (`adopted`, this sentence; `rival`, `w1` before it), the warden's
  `E4` change (q5 kept at `E5`, the one program that reaches the bound), the *may* table (every cell out) and the
  20.00 USD ceiling (`259-briefs/blind/registration.md` § What the synthesis added). It **waits**, by that file's own
  rule, for the key's Linux arm64 rows under `now` and under `rt-e` or the landed build; this Mac's rows are run.
- **A per-thread aside** (E's cost without the lock and the `malloc`): unbuilt.
- **B by another fact** (a save point's identity, a context's): unbuilt.
- **Door 12 with a large value**, a shipped library of `cpu_heap`'s design (*cloudwu/coroutine*, the critic's memory,
  unread), the emitter's *"4,096 bytes as it counts them"* against a reader's count: unrun.
- **The held drafts re-measured under E** (`d-lane-coroutine-room-1` and `-2`, `d-lane-dlsym-make-1`,
  `d-lane-ctx-after-1`, `-3`, `-4`, `-5`, and `escape_carved`): the coordinator's, before any is filed or closed.

## Predictions to score

The seats' (table above), at the landing of R1: the compiler-engineer's shapes pass as `run` goldens on darwin-arm64 and
linux-arm64 and `cost_cb` stays within +1% (`rt-e` read −0.95% here); the ffi-pragmatist's `bfc_host` and `bfc_host_01`
right with `bfc.h` unchanged (right under `rt-e`, 3 of 3, plain, this Mac) and `cx13` aborting at 5,000 throws; the
warden's `now` arm giving q4 `PRINTS` at least 3 of 5. And the synthesis's falsifier of R1: **one correct program among
`aj_carved`, `mj_carved`, `cpu_heap`, `own_carved`, `below`, `one`, `nested`, `own_spawned`, `bfc_host`, `cpo_host`
printing another frame's bytes under the landed route, on any of the three platforms**, withdraws it.

## The blind reading, run 2026-11-25

Added the same evening, after this sitting's commit (`6da1ff4d7`): the registered reading ran on the author's
authorisation of 09:25 (ceiling 20.00 USD), its report `259-reports/llm-ergonomist-blind.md`, the sessions' answers
`259-reports/llm-ergonomist-blind-answers.tar.xz` (text only, 264 files), the runner as adapted, written before any
session, `259-briefs/blind/registration-runner.md`. **31 sessions, all counted, 10.393155 USD**; no session voided (no
project rule, contract or memory reached one; every one of the 315 registered files kept its hash). The key ran on this
Mac and Linux arm64 with no program splitting between them (q4 at `E4`: a wrong value then *compiler bug* under the
base, right under `rt-e`, on both); x86-64 recorded, never graded. **By the registration's own rules the resolution
stands on `adopted`**: D1 holds at its edge (4 of 5 `adopted` readers right on q4, 2 of 5 `now`), D2 (0 against 0 on
q1, q2, q3, q6, q7), D2w (`NOT PROMISED` from 3 to 1 on each of the five) and D3 (design A 4 of 5; no wrong value or
abort answered for B or C); `rival` holds the four too (q4 5 of 5). Scored predictions: the warden's for `w1` missed
on D1 and D3, both of which hold; this synthesis's *`now` gives q4 `PRINTS` at least 3 of 5* measured 2 of 5.
**What it found that it was not built to measure**: 3 of 5 `now` readers of t1 and 4 of 5 of t2 read spec § 13's
existing handler sentence (*"may abort"*) as covering every switch C makes, and answered `NOT PROMISED` throughout;
1 of 5 a task under `adopted`, 0 and 1 under `rival`; the verdict session names that sentence as the real ambiguity.
That is a question for a sitting on § 13's handler sentence, not a change of this resolution. The run's own limits,
in its report: the `adopted` key is the prototype `rt-e`, the landed build not existing yet; no session could write
`report.md` under the registered flags (as at panel 257), each answer kept as `result.md`; the scorer knew the arms'
labels, the grading mechanical and its files archived.

## Author's verdict

**Pending.** The decision's draft is `docs/panel/259-reports/decision-draft.md`, filed with this sitting as `panel 259`.
