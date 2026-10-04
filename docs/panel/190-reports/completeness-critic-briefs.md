# Panel 190, the completeness critic's first pass: the briefs

Begun 2026-10-04 at 07:42:59 (read from `date`), by the completeness critic,
who gives no verdict. Over every file of `docs/panel/190-briefs/` as they
stood at 07:42 (`00-shared.md`, `compiler-engineer.md`, `ffi-pragmatist.md`,
`completeness-critic.md`). My copy: `<scratchpad>/190-critic/`, made with
`git -C /Users/joseph/Temp/heroes/heroes-lang archive 703af779 | tar -x -C
<copy>`. Seed sha256 read in the copy: `2d55c5ff8309b812087f...` (the brief's
prefix holds). Compiler built in the copy from the seed with `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, exit 0.

Written as I go; each section names the command that settles it.

## 1. The shared brief's table: holds, every cell

Written 07:53:43 (`date`). Shapes copied read-only from
`<scratchpad>/batch8/emit/shapes/` into `<copy>/shapes190/`, run one at a
time (`./heroes build shapes190/slots-returns-N.hero --emit-c > sr<N>.c`,
then `wc -l` and `grep -o 'hero_str_decref(' | wc -l`):

| N | lines of C | `hero_str_decref(` | exit, stderr | against the coordinator's file |
|---|---|---|---|---|
| 100 | 97,108 | 30,603 | 0, empty | identical after the source path is normalised (`sed` of the shapes path, then `cmp`) |
| 200 | 374,008 | 121,203 | 0, empty | identical, same way |
| 400 | 1,467,808 | 482,403 | 0, empty | identical, same way |
| 800 | 5,815,408 (read from `<scratchpad>/190-facts/sr800.c`, not emitted) | 1,924,803 | file complete through `main` | `cmp` with the emit lane's `<scratchpad>/batch8/emit/sr800.c`: identical |

The raw files differ only in the source path, which the header and every
`#line` carry (`diff` lines all of the form `#line N "<path>"`).

- **The ratios hold**: lines x3.8515, x3.9245, x3.962; calls x3.9605, x3.9801,
  x3.990 (`python3`, from the cells above).
- **The structure, derived from the counts, not stated in the brief**: the C's
  `hero_str_decref(` count is exactly 3(N+1)^2 at all four N (30,603 =
  3 x 101^2, and so on). It is not the sweep alone: at N = 400 it is 401
  returns x 1,200 swept slots (481,200) plus rule 3's 1,200 store decrefs, all
  in `f`, plus 2 in `main` and 1 in a generated release helper of an optional
  (`h_0opt_f87774a_release`), counted per C function with `awk` over my
  `sr400.c`. A seat predicting the route's count should start from that split,
  not from the total.
- **The IR claim, carried in the brief, now run**: `./heroes build
  shapes190/slots-returns-400.hero --dump-ir` (exit 0), `grep -c decref_slot`
  reads **481,201**, `grep -c return` reads 402 (f's 401 and `main`'s 1), and
  the slot line lists `s0`..`s399` and `$own401`..`$own1200`, 1,200 owed
  slots. 481,201 = 401 x 1,200 + 1 (`main`'s one slot). Holds.
- **A date**: the brief calls the 800 row "the emit lane's own count of
  2026-10-03". That file, `<scratchpad>/batch8/emit/sr800.c`, was written at
  2026-10-04 00:04 (its `.err` at 2026-10-03 23:55), and defect 231's body line
  carrying 5,815,408 is dated 2026-10-04 (`ls -la`; the defect file, line 7).
  The count holds; the day is the next one.

## 2. Missing from the shared brief: clang at `-O0` already FINISHED the 800 shape on this Mac

Q4 asks "whether clang finishes `slots-returns-800` on this Mac", and the
ffi-pragmatist's item 3 asks the same. **The coordinator's own 800 run already
answers it at `-O0`**, and no brief says so:

- `heroes build --emit-c` compiles every unit with clang, without linking,
  before it writes the artifact: `selfhost/cli/produce.hero:243-272` (*"`--emit-c`
  WRITES AFTER THE ROUNDS, WHICH COMPILE AND DO NOT LINK FOR IT"*),
  `selfhost/cli/assemble.hero:48-54` and `:214-217`, at the build verb's default
  level `-O0` (`selfhost/cli/verbs.hero:31`).
- Seen in my copy: each of my three runs left `build/tu-*/slotsreturnsN.c` and
  a clang object beside it (`slotsreturns400-2fcae25217f20f29.o`, 12,500,904
  bytes, 07:46).
- Seen in the trunk's cache, read only (`find
  /Users/joseph/Temp/heroes/heroes-lang/build -maxdepth 2 -name
  'slotsreturns*'`): `build/tu-1ef8254750e64675/slotsreturns800.c` published
  at **07:33** and its object `slotsreturns800-2fcae25217f20f29.o`,
  **49,024,896 bytes, written at 07:41**, the minute `sr800.c` was written. A
  new object, so no cache hit.

So on the trunk's compiler, **clang at `-O0` finishes `slots-returns-800` on
this Mac**, after more than 300 s by the files' times (07:33 to 07:41, beside
four lanes, so no duration). That does not contradict lane irverify's *"at
800 clang does not finish in 300 s"*, whose verb and level neither the brief
nor defect 231's item names (I did not open `<scratchpad>/lane-irverify/`:
the shared brief forbids reading a lane's folder). **What is still unrun** is
`-O2` (`heroes run`'s default, `selfhost/cli/verbs.hero:56`) and
`--sanitize`, on every platform. Q4 should name the level it asks about.

## 3. The ceilings: `own.hero` has 12 lines, and `flatten.hero` has none

The shared brief says *"`selfhost/ir/own.hero` is 534 lines, its ceiling
`tests/harness/suite_layout.hero`'s"*. 534 is `wc -l`; the instrument counts
non-blank lines outside `test` blocks (`code_lines`,
`tests/harness/suite_layout.hero:785-800`), and `own.hero` has no `DECIDED`
row, so its ceiling is `CEILING` = 300 (`suite_layout.hero:44-45`).

Settled by the instrument, not by my replication of it: in my copy I put 13
comment lines at the top of `own.hero` and one at the top of
`selfhost/ir/flatten.hero`, ran `./heroes run tests/harness/main.hero --
./heroes layout selfhost/ir/` (output to a file, read whole), and restored
both files (`cmp` against `git show 703af779:<path>`: identical). The
instrument said:

```
selfhost/ir/flatten.hero: 1151 lines of code, past the 1150 it measured when this check was written [the line goes on]
selfhost/ir/own.hero: 301 lines of code, past §11's 300
```

So at `703af779` **`own.hero` counts 288 of 300 (12 lines of headroom)** and
**`flatten.hero` counts 1,150 of its `DECIDED` 1,150 (none)**. The same run on
the unmodified file, `layout selfhost/ir/flatten.hero`, reads 1 passed, 0 failed. `flatten.hero`
matters because it is where a `return` statement and a `?` become
terminators (next section), and no brief names it. By my replication only
(unrun by the instrument), the other files a route touches: `build.hero` 291
of 300, `released.hero` 285 of 300, `verify.hero` 276 of 300,
`emit/inst.hero` 344 of its `DECIDED` 350, `emissions.hero` 205 of 300,
`lower.hero` 185 of 300.

## 4. The compiler-engineer's file list: right files, two missing, one pointer off

Written 08:01:18 (`date`). The brief names `own.hero` ("its second walk
appends the exit sweeps, its rules 1 to 6"), `lower.hero` ("where a `return`
becomes a terminator"), `released.hero`, `layout.hero` and the emitter's
writing of an exit edge. Read in my copy:

- **Holds**: `own.hero`'s second walk is lines 214-292 (rule 4's incref at
  :261-271, one `decref_slot` per swept slot at :285-289); rules 1 to 6 are
  its header, lines 10-39; rule 4 is *"A returned value is increfed BEFORE the
  sweep"* (:24-25). `released.hero` holds the verifier's reading of the same
  rules (`owed_slots`, `released_on_return`, :20, :52-80), and `own.hero:217-218`
  says the two readings must agree. `layout.hero:153-154` is
  `is_refcounted`, which reads the checker's `c.counted`
  (`selfhost/check/counted.hero:58-91`). The emitter: a return is
  `selfhost/emit/term.hero:52-62`, a `decref_slot` is
  `selfhost/emit/inst.hero:173` through `reference_named` (:340-379), a
  copy-out `inst.hero:47-62`.
- **Off**: the `return` STATEMENT becomes a terminator in
  `selfhost/ir/flatten.hero:136-148` (`returned`), and the early return a `?`
  lowers to in `flatten.hero:470-480`. `lower.hero` holds three other return
  terminators: a constant's body (:91), a test's (:131) and falling off the end
  (`close`, :192-200). Found with `grep -rn '\.return_term(' selfhost/`.
- **Missing**: `selfhost/ir/emissions.hero:38-43`, the `@` copy-out, which
  LOWERING writes into every returning block from four call sites
  (`flatten.hero:137, 145, 478`, `lower.hero:199`), before the ownership pass
  runs. The proposal's *"the exit block copies out the `@` parameters"* moves
  work out of lowering as well as out of `own.hero`. And
  `selfhost/ir/verify.hero:215` (`check_copy_out`, per returning block), the
  verifier's second per-return check. `flatten.hero` is at its ceiling
  (section 3).

## 5. Panels 021 and 106 as the shared brief states them

Read whole in my copy: `docs/panel/021-strings-ownership-and-how-a-float-prints.md`
and `docs/panel/106-the-frame-is-the-sweeps-own-temporaries.md`.

- **021 point 2 and R3: holds, with one attribution to repair.** The italic
  words are point 2 of the PROPOSAL (021:26-33), which R3 (021:162-180) adopts
  ("Point 2 is adopted") and qualifies with a rider the brief leaves out:
  `ptr == NULL` is the one non-value every runtime entry point rejects, and a
  phase-`Owned` invariant that every slot is stored before it is loaded on
  every path, *"the fixpoint stays in the verifier, not in the pass"*. The
  null no-op holds in the runtime for `str`, arrays and maps
  (`runtime/heroes_runtime.h:197, 466, 616`).
- **021's refusal of liveness, the "why" Q2 asks for**, is in three places,
  not one: point 2 (*"the alternative is a liveness pass, and a decref of an
  uninitialised local is undefined behaviour rather than a wrong answer"*),
  the disagreement section (021:82-89, *"the whole point of choosing slots was
  that cleanup be a table walk, and a liveness pass inside the ownership pass
  would spend that"*, panel 019's reason), and R3's last sentence. Panel 019
  is cited by none of the briefs.
- **"Its cost was named then as over-retention": does not hold as written.**
  Neither sitting contains the word (`grep -n -i 'over-retention\|over-retain'`
  over both: nothing). It was named the next day,
  `docs/records/log/2026-08-05-0000-the-ownership-pass-moves-it-does-not-release-an.md`,
  and in `own.hero:41-45`, as the cost of a DIFFERENT decision: rule 5, every
  owning temporary moved into a synthetic slot of its own and held until the
  function returns, which also refuted 021 R2's rule 5. That decision is half
  of this defect's product: at N = 400, 800 of the 1,200 swept slots are
  rule 5's `$own` slots (the `--dump-ir` slot line).
- **"kept by panel 106": holds.** 106's resolution 5 (*"Panel 021 R3 stands.
  Nothing here makes the exit sweep liveness-directed; it makes the sweep
  cheaper to emit"*) and its ratification of 2026-09-04 (*"whether the exit
  sweep should ever become liveness-directed, which is panel 021 R3 and stays
  ratified as it stands"*).
- **"panel 106's frame" (Q1, and the compiler-engineer's item 3): unclear.**
  106's frame is the C stack frame, and its refused proposal was slot sharing.
  It has no case of *"a slot written on one path and not another"*; the
  sentence that bears on Q1 is 106:93-99, *"`own.hero:257-262` reads every
  swept slot at every returning block, so no two of them are ever disjoint"*.
  The premise Q1 names is design.md Part 5's (*"the sweep releases it whether
  the path that stored it ran or not"*, `docs/design/design.md:2748-2749`).
- **Not carried, and 106 asked for it**: 106:281-285, *"It is written here so
  that the next sitting about lifetimes starts from it"*: `--sanitize` HIDES a
  use-after-free when the read happens inside an uninstrumented C library (a
  `str` bound with `SQLITE_STATIC`, `length: 0` at exit 0, `length: 53` under
  ASan). Q1 and the compiler-engineer's item 3 lean on ASan. A shared exit
  moves no release earlier, so the hazard is probably not reopened, but that
  is my inference and the seats should say it from the C.

## 6. Missing: the design.md sentence the route rewrites, and panel 182

- design.md Part 5 (`docs/design/design.md:2752-2758`, Part 5 opens at :2708)
  reads today: *"**Every block that returns**, a `return` and the early return
  a `?` lowers to, performs the `@` copy-out, retains what it returns, and
  releases every local and synthetic slot"*. A shared exit falsifies that
  sentence, so the route amends design.md Part 5, which CLAUDE.md § 4 says
  lands with its own log entry and commit. No brief names it.
- That sentence was written by **panel 182** (2026-09-28, item 6,
  `docs/panel/182-a-value-is-never-zeroed-a-slot-is-and-a-definition-is-whole.md:253-256, 276-279`),
  replacing a false one, kept in design.md beneath it (:2760, :2771-2774): *"The
  pass builds a cleanup-label chain per function so every exit edge
  (`return`, `?`, `break`, `continue`, `panic`, match fallthrough) releases
  live locals and performs `@` copy-out"*. That sentence entered design.md on
  2026-08-03, the day before panel 021 (`git log -S 'cleanup-label chain per
  function'`: `8cacab7a`, *"design: apply all decided amendments"*), and was
  never built. So **the design document promised a per-function cleanup chain
  for eight weeks**; the sitting should know whether it is restoring a design
  or changing one, and no brief cites 182.
- Panel 182 also deferred, *"deferred and not refused"*, *"one slot for the
  synthetic slots of mutually exclusive arms"* (its premise unrun) to
  `docs/work/milestones/M-deployable-binary.md:57-77`, while panel 106 refused
  slot sharing *"permanently and in every form"*. A seat reaching for the
  slot factor will meet both rulings.
- The spec's one sentence on this (`spec/heroes-spec.md:268-269`, *"copy-out
  always happens, including on early return and `?`"*) stays true under a
  shared exit, so the soundness lane holds for that route
  (`.claude/skills/panel/SKILL.md:18-34`: no surface, no diagnostic, no spec
  token). It would NOT hold for Q2's *"leaving the product and bounding the
  inputs"* if bounding means refusing a function past a size: that is a
  diagnostic, a full-panel matter by the same lines.

## 7. Instruments the briefs leave out

Written 08:02:56 (`date`). Q3 names the verifier, `wholes`, `descriptors`,
`determinism`, `lines`, ASan and the leak gate; the compiler-engineer's item
4 names `check`, `run`, `emission`, `determinism`, `wholes`, `descriptors`
and `lines`. Every one of those is a registered selector (`grep` of
`tests/harness/main.hero`). Missing, each with what settles it:

- **`ir`, the 23 IR goldens, which are hand-edit only.** `tests/golden/ir/`
  holds 23 `.expected` files (`ls`), 15 of them carry `decref_slot` (`grep -l`),
  and several hold 3 to 5 returns (`adversarial-try-copies-out` 5,
  `sugar-try` 4). The `ir` form compares `--dump-ir` with them
  (`tests/harness/suite_golden.hero:108-120`), and CLAUDE.md § Hard stops
  forbids `UPDATE_GOLDEN=1` there outright; 021 R8 already called each one
  *"hand-edited labour"*. Q2's cost column counts `tests/emission/` and not
  these.
- **`emit`**, 7 goldens of emitted C in `tests/golden/emit/`
  (`suite_golden.hero:122-134`).
- **`layout`**, given section 3, and the rest of the `selfhost/**` row of
  `.claude/rules/verification.md` (`canonical`, `order`, `records`).
- **`warnings` and two flags of the list.** A non-counted return value
  carried to one exit is a slot written on every path and read at a join;
  `flags()` holds `-Werror=uninitialized` and
  `-Werror=conditional-uninitialized` (`selfhost/cli/flags.hero:91-109`). The
  `if`-as-value join slot is the same shape and builds today (design.md
  Part 5's sugar table), so this is probably safe, but it is an inference;
  `warnings` and the clang compile are what would say it, and no brief lists
  them.
- **The frame.** A return slot per function is one more prologue local in
  every function with a value; panel 106's instrument was `-fstack-usage` and
  the interpreter's nesting ceiling. No brief asks for either.
- **The compiler's speed.** CLAUDE.md § Verification, *"Never slow the
  compiler down"*, owes a `/usr/bin/time -p` before and after; the shared
  brief forbids timing beside four lanes and says "say so where one is owed".
  The specific timing owed (the compiler building itself before and after the
  route) should be named, so the synthesis cannot adopt without saying it is
  unrun.

## 8. The ffi-pragmatist's brief

- **Holds**: `flags()` is `selfhost/cli/flags.hero:91-109`; Apple clang
  21.0.0 here (`clang --version`); the image `heroes-linux-arm64:latest`
  exists (`docker images`, no container started); Debian clang 22.1.8 and
  18.1.8 are on record in that image
  (`docs/records/log/2026-10-03-1123-the-author-answers-a-a-case-a-platform-cannot-run-is-judged-where-its-header-is.md:7`);
  the box's clang 23.1.1 is on record
  (`docs/records/done/2026-10-04-0409-defect-185-...md:13`), carried, not run
  by me; `<scratchpad>/platforms/windows2-u.sh` exists (`ls -la`; I neither
  read nor ran it).
- **The pointer for item 2 does not hold**: `.claude/rules/c-boundary.md` (81
  lines) contains neither `acquires`, nor *the handle set*, nor a leak gate
  (`grep -n -i 'acquir\|handle'`: one unrelated hit at :38). The handle set is
  `runtime/heroes_runtime.h:219-271` and `runtime/parts/alloc.c`.
- **Item 2's premise needs stating**: the exit sweep never releases a handle.
  It covers counted types only (`own.hero:238`; `selfhost/check/counted.hero:58-91`:
  `str`, arrays, maps, every `T?`, failures, records and variants with a
  counted field; never `ptr`, `cstr` or a handle), handles are ended by the
  binding author's own `consumes` call (`heroes_runtime.h:219-221`), and both
  the handle set's and the leases' gates run once at process exit, from
  `main` (`hero_runtime_check_leaks()`, `selfhost/emit/decls.hero:232, 248`;
  `tests/golden/run/abort-lease-never-ended.hero`). So *"is each path's
  handle released exactly once under one exit"* tests something no route
  moves; it is worth keeping as a negative control (does the route change
  anything the set sees?), named as one.
- **A lean**: the seat is asked for the proposal's C only. The C-only
  cleanup label is a C shape it could write by hand at the same cost, beside
  the proposal's, and compare.

## 9. Shapes beside the defect (CL-061) that no brief measures

Built in my copy under `beside/`, `--dump-ir` only, no clang:

- **`@` parameters times returns**: the copy-out is written into every
  returning block (section 4), so it is the same product. 20 `@` parameters
  and 21 returns (`beside/params-returns-20b.hero`): **420 `copyout`** in the
  IR, exit 0. The emit lane's `many-params-N` has one return and does not show
  it.
- **A `?` chain**: each `?` is an early return that sweeps every slot. 20 of
  them (`beside/try-chain-20.hero`): **2,610 `decref_slot`** in the program,
  21 of whose 24 returns are the chain's own function's, exit 0. The compiler's own source holds about 64 lines ending in a `?`
  (`find selfhost -name '*.hero' -exec grep -hE '\)\?[[:space:]]*$' {} +`
  without signatures or comments, 61, plus 3 bare `value?` lines, over 403
  modules), so it is not the compiler's dominant shape.
- Returns of counted types other than `str` (an array, a map, a record or
  variant with a counted field, a `T?`): the return value's slot under the
  proposal ranges over all of them (`counted.hero`), and Q1's list names
  `str`, `?` and unit only.

Q4 sizes only `slots-returns-N` and the seed.

## 10. Routes nobody listed (each a question, none run)

- **One sweep instruction per exit edge** (021 R3 kept to the letter, the IR
  linear), emitted as one call over the function's counted slots held in one
  addressable block. What would make it wrong: slots that are not
  address-taken today (`hero_str_decref(h3_s)` takes a value) become pinned
  to the frame, which is panel 106's measured cost in another form.
- **A cleanup ladder in initialisation order** (a return jumps to the label
  that releases what may be written by then): fewer releases at run time,
  but it needs a may-be-written analysis and, in C, labels that fall through,
  which `selfhost/emit/term.hero:4-9` refuses.
- **Cutting the slot factor**: 800 of the 1,200 slots at N = 400 are rule 5's
  `$own` slots (section 5). It leaves the product quadratic and meets 106's
  permanent refusal and 182's deferred coalescing (section 6).
- **The shared exit only past a threshold** (for functions with more than one
  exit, or above some returns x slots): fewer blessed emissions move, at the
  price of two exit shapes to keep correct. Probably the "conservative"
  resolution the synthesis must record beside the robust one (CLAUDE.md § 4).
- **The C-only cleanup label** is constrained by more than
  `generated-c.md:33`: panel 021 R1 (*"the emitter stays a printer"*, the dump
  shows the refcount ops) and R8 (*"`--dump-ir` means the IR the emitter
  sees"*), and `own.hero:4-8`. Q2 cites only the first.

## 11. Leans

- The compiler-engineer BUILDS one route and PRICES the others "far enough";
  its parenthesis names two of Q2's three other routes and drops *"leaving
  the product and bounding the inputs"*. Only the proposal will arrive with
  measured numbers.
- *"about 1,600 instructions instead of 481,201 at 400"* is labelled derived,
  and it is optimistic: 1,200 `decref_slot` plus 401 rule 4 increfs is 1,601,
  with no store of the returned value into the slot the exit reads. With that
  store it is about 2,000, and about 2,800 if the store takes rule 3's load
  and decref of the old value (my arithmetic, unrun). The same split for the
  C (section 1): today 3(N+1)^2; under one exit about 6N+3, plus N+1 if rule 3
  applies to the return slot.
- Q1 states the soundness argument as its premise ("Every path's slots ... are
  released exactly once ... a decref of null is a no-op"), and the proposal
  says *"The table walk, the zero-initialisation and the unconditional sweep
  stay"* without naming the design.md sentence it rewrites (section 6). Both
  are plausibly true; both should be questions the seats answer.
- The shared brief is otherwise even: it names four routes "even to be
  refused" and asks for any other, and it says which numbers were derived.

## 12. Questions the sitting should ask and does not

- **What acceptance the repair owes**: which N must build, at which level
  (`-O0`, `-O2`, `--sanitize`), on which platforms, within what bound named
  in advance. Section 2 shows `-O0` already finishes at 800 here; panel 184's
  R6 floor is about depth and is not in the spec at `703af779` (`grep -n -i
  'depth\|floor' spec/heroes-spec.md`: only *"Recursion too deep aborts"*).
- **Whether the route changes run-time cost**: one exit executes the same
  releases per return as today plus a store and a jump, so by construction
  only code size and compile time move (an inference, unrun).
  CLAUDE.md § Precedence (2026-09-27): where robustness is not at stake, take
  the route fastest at run time. No brief asks.
- **Which functions the route touches**: all, or only those with a sweep and
  more than one exit. It decides how many of the 359 blessed `.c` files and
  the 23 IR goldens move.

## 13. For the coordinator: what to repair before a seat starts

Written 08:05:19 (`date`). In order of weight:

1. **Q4 and the ffi-pragmatist's item 3 ask an answered question.** Clang at
   `-O0` finished `slots-returns-800` on this Mac in the coordinator's own run
   (the trunk's `build/tu-1ef8254750e64675/slotsreturns800-2fcae25217f20f29.o`,
   07:41; section 2). Name the level asked about; `-O2` and `--sanitize` are
   the unrun ones.
2. **`flatten.hero` is at its `DECIDED` 1,150 and `own.hero` has 12 lines**
   (section 3, the instrument's own words). Replace "534 lines" with the
   instrument's count and add `flatten.hero`.
3. **The compiler-engineer's file list**: the `return` statement and `?` are
   `flatten.hero:136-148` and `:470-480`, not `lower.hero`; add
   `emissions.hero:38-43` (copy-out, four call sites in lowering) and
   `verify.hero:215` (section 4).
4. **"Its cost was named then as over-retention"**: not in 021 nor 106; it is
   `docs/records/log/2026-08-05-0000-...md` and `own.hero:41-45`, about rule
   5's synthetic slots, which are 800 of the 1,200 slots at N = 400 (section 5).
5. **design.md Part 5's sentence the route rewrites** (`:2752-2758`) and
   **panel 182**, which wrote it and recorded the never-built *"cleanup-label
   chain per function"* of 2026-08-03 (section 6).
6. **The `ir` form** (23 IR goldens, 15 with `decref_slot`, hand-edit only),
   `emit`, `layout`, `warnings` and the frame are missing from Q3 and from the
   compiler-engineer's item 4 (section 7).
7. **The ffi-pragmatist's item 2 cites a file that does not hold it** and
   rests on a premise the code refutes: the sweep releases no handle (section 8).
8. **"panel 106's frame"** in Q1 and the compiler-engineer's item 3 points at
   no such case in 106; 106's warning for the next lifetimes sitting (ASan
   blind to C reading freed memory) is not carried (section 5).
9. **Shapes beside** (`@` parameters times returns, a `?` chain, non-`str`
   counted returns) and **routes nobody listed** (sections 9, 10); the
   bounding route would leave the soundness lane (section 6).
10. **Leans**: one route built and the rest priced; *"about 1,600"* omits the
    per-return store (about 2,000 to 2,800); the 800 row's date is 2026-10-04
    (sections 1, 11).
