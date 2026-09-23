# Panel 175: completeness critic

Not a judge and no verdict. Everything below was run on 2026-09-23 in
`<scratchpad>/175-completeness-critic/`, a `git archive` of `64c92654` plus the
untracked briefs, with its own compiler built from the seed (`clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, 3.27 s). Route A was rebuilt from the
compiler-engineer's own `prototype-A.diff`: the patched `handle_traffic.hero` and
`alloc.c` are byte-identical to that seat's (`cmp`), and `heroes-A` built in
74.94 s real, 70.68 user. E2 is `prototype-E2.diff` applied to my copy of the
runtime (`cmp` identical to the seat's `runtime-e2/parts/os.c`). The ffi seat's
`runtimeA`, `runtimeA2` and `runtimeE`, and the other seats' probes, were copied
into my directory and rerun there. Linux runs are `heroes-linux` (x86-64) and
`heroes-linux-arm64`, with my directory mounted read-only, copied inside and built
from the seed there. Windows is **UNRUN** throughout.

"Three runs" and "five runs" mean the binary was run that many times and every
run gave the number shown.

---

## 1. Contradiction one, settled: route A aborts an ownership transfer, and no declaration rule separates it from the crossing

**Route A aborts a transfer.** The llm-ergonomist argued shape S from the
document. Nobody ran it. I wrote it three ways. `xfer_cj.hero` is the
ergonomist's program over a 12-line header with cJSON's contract.
`xfer_jsonc.hero` uses real json-c 0.19, whose header says
`json_object_object_add` works *"in effect transferring ownership that object to
`obj`"*. `xfer_ssl.hero` uses real OpenSSL 3, whose `SSL_set_bio(3)` page says
`SSL_set0_rbio` *"transfers ownership of rbio to ssl. It will be automatically
freed … when the ssl is freed"*. Each is `check`, `build -O0`, then runs:

| program | today's compiler | route A (compiler-engineer's prototype) |
|---|---|---|
| `xfer_cj` | 0, 24 B stdout, 0 B stderr, 5 of 5 | **134, 0 B stdout, 240 B**, 5 of 5: *given back to `cJSON_AddItemToObject` … marked `acquires cJSON_Delete`* |
| `xfer_jsonc` | 0, prints `add 0 fields 1` / `put 1` (json-c freed the root), 5 of 5 | **134, 244 B**, 5 of 5 |
| `xfer_ssl` | 0, 3 of 3, and `leaks --atExit` reads **0 leaks** | **134, 228 B**, 3 of 3 |

**Under route A there is no spelling of the transfer that runs.** The two
escapes the surface offers, over json-c, three runs each:

- dropping `consumes` on `val` gives 134 and 166 B, *1 C handle(s) never given
  back*;
- `borrows` on the creator gives 134 and 396 B, the stray report.

The only spelling that runs today is `consumes`. **Panel 147's ffi seat had
already said that is how a transfer is marked**
(`docs/panel/147-reports/ffi-pragmatist.md:241`: *"any C function that takes
ownership (`xmlAddChild` adopts its node; `curl_slist_append` adopts its head)
must be marked, or the sweep double-frees"*). No 175 report cites it.

**What rule admits the transfer and still refuses `popen`→`fclose`?** I built
three, each a variant of the prototype's `consumer()` in
`emit/handle_traffic.hero`, and each was a full compiler build:

- **T1**, the llm-ergonomist's rewording (*"…another that an `acquires` names"*):
  a consumer is held to the mark only if some `acquires` in the program names it.
- **T3**: a consumer that also takes a non-consumed parameter reaching a handle
  is a transfer into it, and is admitted.
- **T4**: T3, but only when that parameter has the same written type as the
  consumed one.

`build -O0`, three runs, exit and stderr bytes (`work/q1/matrix2.txt`):

| program | today | A | T1 | T3 | T4 |
|---|---|---|---|---|---|
| `popen_darwin` (brief) | 0 | 134 | **0** | 134 | 134 |
| `oneacq`, `outacq` (brief) | 0 | 134 | **0** | 134 | 134 |
| `popen_fopen`: the same, with `fopen … acquires fclose` also declared | 0 | 134 | 134 | 134 | 134 |
| `xfer_cj`, `xfer_jsonc` (same-type transfer) | 0 | **134** | 0 | 0 | 0 |
| `xfer_ssl` (a `Bio` into an `Ssl`) | 0 | **134** | 0 | 0 | **134** |
| `vk_cross`: `buf_destroy(d: Dev, b: Buf consumes)` crossed with `buf_destroy_pooled`, Vulkan's shape | 0 | 134 | 0 | **0** | 134 |
| `close_v2` (real sqlite3, two valid releasers) | 0 | **134** | 0 | 134 | 134 |
| `peek_other` (a `borrows` result consumed by the other releaser) | 0 | 134 | **0** | 134 | 134 |
| `font_part` (a record) | 134, 396 B, after the fact | 134, 232 B, before C | 134, **396 B** | 134, 232 B | 134, 232 B |
| `popen_freopen` | 0 | 134 | 134 | 134 | 134 |
| `freopen`, `transfer` (realloc), `xacquires` | 0 | 0 | 0 | 0 | 0 |

So T3 and T4 both answer the coordinator's question: each refuses
`popen`→`fclose` and admits the cJSON transfer. **Neither is right on every
row.**

- T3 lets a crossing between two releasers that take a context through
  (`vk_cross`).
- T4 aborts OpenSSL's correct, leak-free cross-type transfer.
- T1 admits every transfer, and **refuses none of the brief's three
  single-module reproducers**. It catches `popen` only when `fopen` is also
  declared, so under T1 the abort depends on a third declaration. **That is the
  llm-ergonomist's own veto condition** (*"the abort turns out to depend on
  anything besides the handle's acquiring declaration and the ending call"*),
  met by the ergonomist's own proposed wording: `popen_darwin` at 0 against
  `popen_fopen` at 134.

**Why no such rule can work.** A script over each program's `extern` group lists
the consuming functions that no `acquires` names. In all nine programs they are
exactly the functions in question: `fclose`, `h_close2`, `h_close2`,
`cJSON_AddItemToObject`, `json_object_object_add`, `SSL_set0_rbio`,
`buf_destroy_pooled` and `sqlite3_close_v2`. `popen_fopen` has none. The mistake
and the correct transfer have the same standing in the declarations. The
distinction lives in the C library's documentation, which is
`check/acquiring.hero:257`'s own sentence about `acquires` and `borrows`: *"No C
header states which, so the binding author states it."*

## 2. Contradiction two, settled: route E's goldens do need a harness change, and panel 173's line already did

`suite_run.hero:151-189` judges a case without `!sanitizer:` against the same
`!panic:` text under `--sanitize`. A case with `!sanitizer:` needs ASan's
stderr to carry the needle. The handler is compiled out under the sanitizer
(`os.c:186`, `HERO_STACK_GUARD_YIELDS_TO_ASAN`).

I wrote seven cases into `tests/golden/run-e/` of my copy and ran the real
`run` suite over them. The only change is `RUN_DIR` and `LEAST` in a copy of
the harness (`tests/harness-e/`). The command was `heroes run
tests/harness-e/main.hero -- ./heroes run`, once per runtime:

| case (`ec.h`: `abort()`, `__builtin_trap()`, `malloc`/`free`) | today, Darwin | E2, Darwin | E2, Linux x86-64 | E2, Linux arm64 |
|---|---|---|---|---|
| `control` | pass | pass | pass | pass |
| `c-trap-with-a-lease-live`, `!panic: … lease(s) still live` | **fails at `--sanitize`** | fails at `--sanitize` | fails at **-O0** (132, 0 B: SIGILL) | fails at `--sanitize` |
| the same with `!sanitizer: AddressSanitizer` | **fails at `--sanitize`** | fails | fails at -O0 | fails |
| `c-abort-with-a-lease-live` | **fails at `--sanitize`** | fails at `--sanitize` | fails at `--sanitize` | fails at `--sanitize` |
| `e-c-abort-no-lease`, `!panic: which this runtime did not raise` | fails at -O0 (no E) | **fails at `--sanitize`** | fails at `--sanitize` | fails at `--sanitize` |
| `e-c-trap-no-lease` | fails at -O0 | **fails at `--sanitize`** | fails at **-O0** | fails at `--sanitize` |
| `e-double-free-no-lease`, `!sanitizer: double-free` + `!panic:` | fails at -O0 | **pass** | pass | pass |
| total | 1 passed, 6 failed | **2 passed, 5 failed** | 2 / 5 | 2 / 5 |

Under `--sanitize`, a C `abort` and a C trap write **0 bytes** on Darwin (exit
134 and 133) and **0 `AddressSanitizer` lines** on both Linux legs.

- **The compiler-engineer's claim holds only where ASan has something to say.**
  That is the double free. It fails for every other death route E exists to
  name.
- **The gap is older than route E.** Panel 173's shipped lease line, for a C
  `abort` or trap with a lease live, cannot be pinned by any expectation the
  harness reads today, on any of the three platforms. Panel 173 shipped only
  `c-frees-a-lease-*` cases, which are exactly the ones ASan reports as
  `bad-free`.
- **A remedy exists, and it is measured.** With
  `ASAN_OPTIONS=handle_abort=1:handle_sigtrap=1:handle_sigill=1`, ASan reports
  `ABRT on unknown address`, `TRAP on unknown address` (Darwin 134, arm64 exit
  1) and `ILL on unknown address` (x86-64, exit 1). The trap's name differs by
  architecture, so a portable needle is `on unknown address`.
- **Where to set it is a change either way.** It could go in the harness, in
  `run --sanitize`, or in the runtime's sanitizer arm as `__asan_default_options`.
  The last is **UNRUN**.

## 3. The other contradictions between seats, each checked

1. **The spec-warden's registered prediction is already false against the
   prototype.** It says A takes `xacquires` from 0 to 134. Under the
   compiler-engineer's A it reads 0, 0 B, five of five, and 0 under T1, T3 and
   T4 as well. The compiler-engineer is right: nothing crosses at run time.
   **The prediction's other half, that `u1_handle_reuse` stays at 0, is not a
   function of the route.** Its emitted C is byte-identical under A, T1 and T3
   (`diff` empty), and so are the text sections (`otool -tV` diff, 0 lines).
   The binaries differ only in `LC_UUID` and code signature (1099 bytes), and
   that decides whether libmalloc hands the freed 8-byte block back.
   - The A binary prints `same address: true` and exits 0, six of six.
   - The T1 and T3 binaries print `false` and abort at 134, 396 B, six of six.
   - Copying one binary under the other's name follows the bytes, and
     `MallocNanoZone=0` changes nothing.

   A reproducer that does not depend on the allocator is `u1_static.hero`, over
   the compiler-engineer's static cell. It reads 0 with 0 B under today's
   runtime, ffi-A and ffi-A2, three of three. That is the one to file with F1.
2. **The ffi seat's route A does not pass the `runtime` suite.** Each runtime
   ran in its own tree with `heroes run tests/harness/main.hero -- heroes
   runtime`. `runtimeA` reads **6 passed, 2 failed** (allocation floor 10
   against 7; `runtime/threads` names `hero_handle_by` and `hero_handle_from`).
   `runtimeA2` also reads 6 and 2, with the floor at 11. The compiler-engineer's
   pair-entry A reads 8 and 0, the same as today's. "56 + 7 lines" does not
   carry this.
3. **Each seat's route E prototype fails on the other seat's shape.**
   - **E2 says `in e.main` where the call was in `e.run`.** That is the
     ffi-pragmatist's `-O2 -fomit-frame-pointer` library, a library `abort` and
     a library `assert`, on Darwin. Its *"true on every path I could
     enumerate"* was run on header functions called straight from `main`.
   - **The ffi-pragmatist's `runtimeE` prints *"the process is dying of SIGABRT,
     raised by this process …"* in a process that recovers and exits 0.** That
     is `e_recover` with `E_CTOR=1`: 0, 265 B, three of three. It is the
     compiler-engineer's E1 defect, because the ffi line is the `else` of panel
     173's branch and speaks before the earlier handler is called.
   - A sound route E needs both halves: E2's ordering and the ffi-pragmatist's
     `under`, SIGILL and `si_pid`. **No prototype is both.** The shipped lease
     line keeps its `in e.main` under both runtimes (Darwin and Linux arm64,
     rerun).
4. **The two route A prototypes disagree on one shape, and neither seat
   compared them.** On `fixed_unseen` the compiler-engineer's A reads 0 (newest
   mark wins). The ffi seat's `runtimeA` and `runtimeA2` read 134 and 276 B,
   three of three: they keep the first mark and name `f_open … acquires
   f_close`.
5. **The spec-warden's candidate 2, "nothing", rests on a sentence that already
   refuses the transfer.** § 13 says *`consumes` … says the call ends that
   value's life* and *`acquires` … names **the one** that ends it*. Read
   together, a json-c add cannot be written, so *"the repair makes the compiler
   meet the document"* means aborting three correct programs that run today
   (§ 1). And *"under route A the abort's own text carries the fix (§4.17)"* is
   false for this shape. A's text tells the reader the named call is *the one
   call that ends this handle's life*, and following it is the llm-ergonomist's
   repair chain.
6. **E2's third sentence repeats wording the spec-warden measured false.** It
   says *"a pointer the program must give back is a handle, and a handle given
   back twice is caught"*, which F3 (`int *`, `char **`) and F1 (reuse)
   falsify. That runtime text needs the same pricing as candidate 3.
7. **The ffi seat's A2 trades one hazard for another, and the second is
   §1.12's shape.** A getter wrongly marked `acquires` and a refcount `ref` are
   the same declaration, `f(x: T) -> T acquires rel`, returning the pointer it
   was given. Three runs each (`work/a2/`):

   | program | today | ffi-A | ffi-A2 |
   |---|---|---|---|
   | `refcount` (correct) | 134, 396 B | 134, 396 B | **0** |
   | `getter_wrong` (should be `borrows`) | 0 | 0 | 134, 166 B, *never given back* |
   | `getter_wrong_repaired` (the second close that message invites) | **134, 396 B, caught before C** | 134, 396 B | **133, 0 bytes**: a raw libmalloc double free |

8. **libmalloc's signal is not stable for one binary.** The ffi seat's library
   double free (24 B, inside an `-O2` dylib) exits 133 sixteen times and 134
   four times in twenty runs, with 0 B every time. The brief's `p10` and
   `b_out` read 133 twenty of twenty. The crash report reads *BUG IN CLIENT OF
   LIBMALLOC: not an allocated block*. The historian's *pointer being freed was
   not allocated* line appeared in none of my runs. So a route-E golden must not
   pin the signal name.

**Four shipped defects the seats found, not in the coordinator's list of
three.** I reran each.

- **The lease line is said in a process that exits 0** (compiler-engineer):
  today's runtime gives 0 and 277 B, three of three.
- **The lease line names `in e.main` for a call made in `e.run`** (ffi): Darwin
  and Linux arm64.
- **A library trap with a lease live on Linux x86-64 is 132 and 0 B** (ffi):
  `heroes-linux`, because SIGILL is not installed.
- **A correct refcounted binding aborts** (ffi): 134 and 396 B, with both the
  `acquires` and the `borrows` spelling.

The coordinator named F1, F4 and panel 170's item 8. These four are defects in
what ships, found in reports, and each needs filing or a written reason not to.

## 4. The shapes each recommended route was run against, and the ones it was only argued against

**Route A, the compiler-engineer's pair entry:**

- a record: **run** (`font_whole`, `font_part`; rerun);
- two modules: **run** (`xmod`; rerun, 0);
- a `borrows` result consumed: **run** (`peek_same` 0, `peek_other` 134);
- a reused address: **run** on a static cell. Over malloc it depends on the
  allocator (§ 3.1);
- a transfer: only **run** where the transfer also acquires (`freopen`,
  `realloc`). The transfer that does not acquire was **never run until § 1**,
  and it aborts;
- two valid releasers: **run**, aborts.

**A′ / A2, the ffi-pragmatist's (a set of names, and a count per address):**

- a record: **not run**;
- two modules: **argued** from `tu_a.c` and `tu_b.c` in C, not run as a Heroes
  program;
- a `borrows` result consumed: **not run**;
- a reused address: not run by the seat. I ran `fixed_unseen` (134, first
  mark kept) and `u1_static` (0, silent);
- a transfer: not run by the seat. I ran `xfer_cj` on `runtimeA` and
  `runtimeA2`: 134 and 306 B;
- two valid releasers: **run** on three platforms, 0 with both names;
- refcount: **run**, 0 under A2;
- a wrongly marked getter: I ran it, and the repair becomes 133 with 0 B
  (§ 3.7).

**B, inside D (ffi-pragmatist, historian):**

- B was prototyped only on the compiler-engineer's A: 2 of 4 reproducers, not
  `font_part`, not `peek_other`;
- **A′ plus B, which is D, was never built as one compiler.**

**E2 (compiler-engineer):**

- run on Darwin only, with header functions called from `main`;
- an `-O2` library: I ran it, and `in` is false;
- x86-64: I ran it, and the no-lease trap is 132 with 0 B, failing at `-O0`;
- goldens: I ran them, 5 of 7 fail on three platforms;
- the recover shapes: **run**.

**Route E, the ffi-pragmatist's:**

- run on three platforms against an `-O2` library;
- a recovering handler: I ran it, and the line is false;
- goldens: **not run**;
- Windows: written and **UNRUN**.

**F:** the `tag void` spelling **run** by two seats. The sentence was priced.
Any effect on a reader is **UNRUN**.

**Routes each seat listed and nobody ran:**

- the spec-warden's per-acquisition serial;
- the historian's H;
- the compiler-engineer's *one C name, one mark, program-wide*.

## 5. Routes nobody listed

- **The binding author states what a consuming call does, as panel 148 made the
  author state what a producing call does.** Three measured facts point here:
  - § 1: release against transfer;
  - § 3.7: a new life against an added reference;
  - T1, T3 and T4 each fail a real library.

  The shapes it could take are unpriced surface: a second consuming word, or A′
  with transfers inside the set. Over json-c the second means 12
  `json_object_new_*` creators, each naming `json_object_put` plus up to 5
  adding functions (counted by name in `json_object.h`; I read the ownership
  text for `object_add` and `new_object` only). OpenSSL's headers carry 130
  declarations named in its `set0`/`add0`/`push0`/`own0` convention (a name
  count, not each verified as a transfer). The ffi census could not see this
  class, because its pattern matches releaser names.
- **Let `acquires` and `consumes` reach a `ptr`.** `ptr` binds an `int *`
  result today (`work/ptr/ip.hero`: check 0, build 0, runs). The marks on it are
  refused by `unread_mark` (*"`ptr` reaches no handle"*). That refusal is the
  whole of F3's gap for `int *`, and the brief took it as fixed. Unpriced and
  **UNRUN** beyond that.
- **Make the sanitiser speak rather than the golden silent** (§ 2). This is
  measured on three platforms, and it is where route E's landing condition
  lives.
- **A declaration rule that refuses a consuming extern no `acquires` names.**
  It is the check-time image of T1. In the tree it would meet 7 such functions
  in 5 golden files and 0 in `examples/` (a script over single-line signatures).
  It would refuse the three reproducers, the three transfers and
  `sqlite3_close_v2` alike. That is § 1's point again, and it is **UNRUN** as a
  rule.

## 6. Claims checked here, beyond the above

- **Route A leaves the shipped examples alone.** `corpus` under A reads **55
  passed, 0 failed** (real 346 s against 172 user, so no timing is claimed). No
  seat ran it, and `.claude/rules/verification.md` asks for it when a refusal
  widens.
- **Lane 076's merge does not meet either route.** Its two new `run` cases pass
  under E2 and under A, each combined with the lane's `str.c`: 2 passed, 0
  failed.
- **The brief's note on lane 076 is incomplete.** It says the lane touched
  `str.c` *only*. The lane also adds those two goldens and changes `ROADMAP.md`
  and `DEFECTS.md` (`git diff --stat 64c92654`, 10 files). So `run` will count
  138 where the reports say 136.
- **The historian's two recent web claims hold.** GCC's `sm-malloc.cc` comment
  is verbatim as quoted. The win32metadata plan (PR #2295, opened 2026-08-27,
  merged 2026-09-15) contains both quoted sentences. It is a specification with
  a five-phase implementation plan, not implemented behaviour, and the report
  already says so.

## 7. The procedural fault

**No number I reran was bent.** Of the compiler-engineer's figures I reran the
four reproducers under A (219, 222, 222 and 0 B, identical), the E cases and the
`runtime` suite, and each matched. Of the ffi-pragmatist's I reran the refcount
program, the `-O2` library cases, the SIGILL hole and `runtimeA`'s `runtime`
result, and each matched, except the double-free signal in § 3.8. The
spec-warden's prices are **UNRUN** here (they need the API key).

**The llm-ergonomist's file in the worktree is harmless.** `heroes-lane-076` is
at `4324daab`, which descends from `64c92654`, and `git diff --stat 64c92654 --
spec/` there is empty. So its input bytes were the same wherever it read them.

**A separate breach, and it is structural rather than the worktree's.** The
seat reports that `CLAUDE.md` and three `.claude/rules/` files arrived in its
context. The `paths:` frontmatter shows that reading `spec/**` loads
`verification.md` and `spec-shape.md`, and reading `docs/**` loads
`verification.md` and `records.md`. That is three files, and `spec-shape.md`
carries token counts. So this happens whenever the blind seat reads its input
from inside the repository, not only in this sitting (an inference from the
frontmatter).

**The remedy is a question, not a measurement.** Hand the blind seat a copy
outside the tree. Whether rules then stay unloaded is **UNRUN**.

## 8. The questions the sitting should have asked

1. **Is `consumes` one thing, and is `acquires`?** A consuming call ends a life,
   passes it into another handle, or drops one of several references. An
   acquiring call begins a life or adds a reference. Route A makes that
   difference load-bearing, and no declaration Heroes has carries it (§ 1,
   § 3.7). Panel 148 split the producer side with `borrows`. Nobody asked
   whether the consumer side needs the same split before the runtime starts
   refusing on it.
2. **Can a golden pin it?** That should have been asked beside *"can the line
   be true?"* for route E. It would have found that panel 173's line for a
   non-free death has no golden on any platform (§ 2).
3. **Is candidate 2 free?** Only if the synthesis accepts that § 13 as written
   refuses json-c, cJSON and OpenSSL ownership transfers that run correctly
   today.

## Unrun

- Windows, everything.
- UCRT `fclose` on a `_popen` stream.
- Real Vulkan (only its declaration shape was run).
- The spec-warden's prices.
- Any reader-side effect.
- `__asan_default_options`.
- D as one compiler.
- The serial route, H, and *one C name, one mark*.
- The ergonomist's second repair (adding `cJSON_Delete(child)`), which is
  inferred to be a C double free.
- Darwin x86-64.

## Files

All under `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/175-completeness-critic/`:

- **Compilers:** `heroes`, `heroes-A`, `heroes-T1`, `heroes-T3`, `heroes-T4`,
  and their sources `A/`, `A-T1/`, `A-T3/`, `A-T4/`.
- **Runtimes:** `runtime-e2/`, `runtime-ffiE/`, `runtime-076e2/`,
  `runtime-076A/`.
- **Transfers and the rule matrix:** `work/q1/` (`matrix.sh`, `matrix2.sh`,
  `matrix2.txt`, `xfer_cj.hero`, `cj.h`, `xfer_jsonc.hero`, `xfer_ssl.hero`,
  `vk.h`, `vk_cross*.hero`, `popen_fopen.hero`).
- **A and A2 shapes:** `work/a2/` (`a2.py`, `getter_wrong*.hero`, `g.h`,
  `u1_static.hero`).
- **Route E goldens:** `tests/golden/run-e/` and `tests/harness-e/`, with the
  results in `work/e-suite-stock.txt` and `work/e-suite-e2.txt`.
- **Linux runs:** `work/linux_critic.sh`, `work/linux_asanopt.sh`.
- **Library and recover cases:** `work/e/`, `work/rec/`.
- **The rest:** `work/ptr/`, the `runtime` suite trees `rt-{stock,ffiA,ffiA2,ceA}/`,
  and `work/corpus-A.txt`.
