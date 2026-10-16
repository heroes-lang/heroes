# Panel 215: a group record's padding is zeroed where C receives it, a record holding a lock is refused as a value, and `rest: zero`'s sentences say what C reads

**Convened and written 2026-10-14, on the tree frozen at `7e5c02048`**, by the author's answer of
that morning at 08:13, meant as *I ratify everything and accept all your suggestions, proceed*,
where the coordinator's suggestion was to convene sittings for M-buildable-structs' issues 1334 and
1335; the coordinator put both into one sitting with the question measurement 044 opened. **Full
panel**: the answer may be a refusal, a diagnostic class and a sentence of the specification
(CLAUDE.md § 4). Seats: compiler-engineer, ffi-pragmatist, spec-warden, historian, llm-ergonomist,
and a completeness critic twice; then a **second round** in which three seats built or drafted the
routes the critic's pass 2 left standing, and the llm-ergonomist's experiment ran again on the
repaired text. **The llm-ergonomist ran blind and paid**, on the author's budget of that morning
(meant as: *you can run paid tests, I give you ten dollars*): twenty `claude -p` readers in two runs,
4.1950 USD. **Resolution: provisional, author ratification pending.**
Briefs: `docs/panel/215-briefs/` (the shared brief, five seats, the critic, and the three
second-round briefs). Reports: `docs/panel/215-reports/` (the five seats, `critic-briefs.md`,
`critic-reports.md`, `compiler-engineer-build.md`, `ffi-pragmatist-build.md`,
`spec-warden-drafts.md`, `llm-ergonomist.md` with its dated correction, `llm-ergonomist-rerun.md`,
and each seat's `-work/` folder of sources, scripts and outputs, no binaries).

## The proposal, verbatim (the shared brief's three questions)

- **Q1, padding across a call by value** (issue 1334): a `{char c; double d}` record copied by value
  hands C uninitialised bytes at offset 1 on Linux x86-64. Routes A nothing, B zero at every
  construction, C a group record crosses a call by address, D a copy into a zeroed temporary before
  C, E the padding named as a field.
- **Q2, a record C forbids to copy** (issue 1335): `m2 = m` copies a `pthread_mutex_t`; a copy of a
  locked mutex reads locked, and both lock at exit 0. Routes A nothing, A2 `partial` with the lock
  undeclared, B a mark on the record, C the same decided from the header, D a check at run time.
- **Q3, R1's payment**: panel 194 ratified R1 (`rest: zero`) with a reader test registered at its
  landing and never scored, and measurement 044 met panel 178's old condition. Routes A R1 stands
  on 194's conditions, B R1 withdrawn, C its sentence shortened, D 178's condition void.

## What the sitting found before it judged

- **The critic's pass 1**: route B does not close Q1 (the bytes are lost in the by-value call, not at
  the construction); R1's payment is panel 194's, not 178's (178 is superseded).
- **The first paid run** (`llm-ergonomist.md`): A1 (a path into `char[108]`) 10 of 10, `u8` 0; A2 (a
  record holding a union) **1 of 10**; B (a counter holding a mutex) 10 of 10, no copy, on a task
  whose verb only `@` can write.
- **The critic's pass 2** (`critic-reports.md`), which changed every question: (1) the readers had
  no compiler, so A2's misses are the specification's, by their notes (§ 13's *named or anonymous*,
  R1's silence about what `partial` leaves out), not defect 2062's message, which is true of C; the
  miss meets panel 194's registered row *nothing becomes my verdict if the reader test fails*. (2)
  Q1 is one mechanism at every level: on x86-64 SysV the by-value call loses the bytes, and above
  `-O0` panel 196's zeroed lend region hides it; **`heroes build` and `heroes test` compile at `-O0`**,
  where two records `==` calls equal give different `memcmp` and SHA-256 answers from one run to the
  next, and C reads seven bytes of a dirtied stack; the Windows box keeps the bytes at every level.
  (3) The ffi-pragmatist's G was the vetoed naive form; the compiler-engineer's left a nested record
  and an array lend open. (4) Route C's "three spellings" objection goes once clang is asked by type
  identity; B with `z_stream` marked refuses defect 413's own repair. (5) The dispute on Q2 is
  CLAUDE.md's ranking against design.md §1.12's *does not suspend Principle 0*.

## Verdicts

| seat | verdict | section | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto** B, C, D and naive G on Q1; **adopt** G with the header's membership + H; Q2 **B**; Q3 **A + D** | CLAUDE.md § Precedence (design.md is silent on padding reaching C), §1.7 | first: G ≤ 250 lines, B ≤ 450; built second round: G recursive **+480 −11** | at M-buildable-structs' close MSan reads 0 on the named probes | — |
| ffi-pragmatist | Q1 **G**, **veto** E; Q2 **B**, C as a note; Q3 **A + D**; second round: **approve C** with three corrections, no veto | design.md §1.11, §1.12, §4.19, §4.17 | C: ~2.5% of a cold corpus build, 0 spec tokens | C moves 0 of 905 files at `--emit-c` on Darwin and glibc | any of the three corrections missing turns C to a veto |
| spec-warden | Q1 **A in the spec** (0 tokens, the rule in `generated-c.md`); Q2 **C** (0 tokens), object to B; Q3 **A + D**; second round: the repaired sentences, **no budget veto** | design.md §1.6, §1.2, Principle 0 | `m-g-at` **+60 real** (10,108, measured by `--refresh`) | the re-run: A2 ≥ 8, U ≥ 8 with `u8` ≤ 2, C ≥ 8 correct and ≥ 3 copying | — |
| historian (advisory) | Q1 **A** + an unlisted **F** (refuse a padded record lent as bytes); Q2 **B** widened to moves and arrays, C seeded by POSIX's list; Q3 **A + D** | precedent | — | Q3: `u8` < 3 of 10 | a language that withdrew a no-copy mark on C records turns B to object |
| llm-ergonomist (blind, paid) | first run: approve R1 *as landed*, **withdrawn** after the critic; re-run: **approve the repaired sentences**, **approve a refusal of copying a lock**, **object to landing it alone** | the thesis, as a reader | — | with a refusal and `@c` named, ≥ 8 of 10 correct on a second try | — |

## What the second round built and measured

- **Q1, G recursive** (`compiler-engineer-build.md`): before each lend to C (a whole cell, a field,
  an element, an `@` parameter passed on, the region `hero_lend_take` fills), the bytes no C member
  holds are zeroed in place, every span written as clang-checked `offsetof`/`sizeof` of a member the
  dump names, recursing into member records and array elements, and **nothing written inside a record
  whose C type is a union** (asked of C by `__builtin_classify_type`): building it found that the
  first prototype zeroed a union's wide arm when the binding named its narrow one (`0.0 1432778632`
  where today prints `2.5 1234605616436508552`, exit 0), the class the compiler-engineer had vetoed in
  the naive G. MemorySanitizer on x86-64 at `-O0`/`-O1`/`-O2`/`-O3`/`-O2 -fno-inline`: the critic's
  `pc` 6/5/5/5/6 → 0, `el` 1/1/1/1/1 → 0, `fieldlend` 5/2/2/2/2 → 0, `rzpad` (an anonymous union and a
  `partial` record, from clang's dump) 4/0/0/0/0 → 0, the six shapes beside (`pn`) 6/5/5/5/5 →
  1/0/0/0/0, the one left being padding bits inside a byte a named bit-field holds; union probes keep
  their values. With no sanitizer: `dv` `true 0 7` → `true 0 0`; the ffi-pragmatist's `main`
  `memcmp 0 0 0`, `sha true true true` at `-O0` and `-O2`, three runs each. `run` 518/0, `check`
  699/0, `emit` 12/0, the compiler's own tests 1,654; `emission` 26 moved, every changed line a zeroing
  statement, a brace or a `#line`; `layout` red on four modules past 300 lines; a cold `--emit-c`
  136.01 s against 136.75 s.
- **Q2, C by type identity** (`ffi-pragmatist-build.md`): built on this Mac, Linux arm64 and the
  Windows box, with three corrections the brief lacked, each found by running it: only a lock that is
  a record or a union is asked (glibc's `pthread_spinlock_t` and Darwin's `sem_t` are typedefs of
  `int`, and without the check `z_stream`'s `int data_type` was called a spinlock); the question
  includes no header but the program's own (`<windows.h>` after raylib's names failed); `extern`
  declarations are not judged by signature, a copy into C refused at the call's argument (the
  first build refused a binding today's compiler builds and runs). Refused: issue 1335's copy, the
  critic's `copy.hero` (10), a counter returned by a helper (2), a counter in a `[T]` (2), a Heroes
  record holding one copied (1, the chain of fields named), Windows' `CRITICAL_SECTION`, `SRWLOCK`
  and C11 `mtx_t` copies. Kept, outputs identical: every `@` lend, the critic's `b.hero` and
  `race.hero`, the first run's ten `b.hero`, defect 413's goldens, `examples/ctime`. Census of the
  905 files with an `extern`: 0 move at `check` and at `--emit-c`, on Darwin and glibc; one lock found
  in 135 questions (defect 094's `hero_mutex`, lent with `@`, still builds). The message:
  `error[ffi_lock_copied]: this copies a 'Box', whose C struct 'struct box' holds a 'pthread_mutex_t'
  at 'm', which C forbids to copy`, its note naming `@`. What C cannot see: `z_stream`, an empty
  `TAILQ_HEAD`, glibc's scalar spinlock, Darwin's `os_unfair_lock`, each building at exit 0.
- **Q3, the sentences** (`spec-warden-drafts.md`): every union shape built; one rule decides them,
  *a field is a name C reads as `s.name`*, which is what the emitted static assertions ask clang. The
  draft `m-g-at`, priced by the coordinator's `--refresh` at **10,108 real** (+60; spendable 132).
- **The re-run** (`llm-ergonomist-rerun.md`), ten blind readers on `m-g-at`, the first run's
  launcher, 2.2106 USD: **A2 10 of 10** (all `partial`), **U 10 of 10** with `u8` 0, **C 10 of 10
  correct and 10 of 10 copying an initialised lock**: every reader's read-only helper takes the
  counter by value and copies it again to lend it to C, because a Heroes parameter is `[ "@" ] ident
  ":" Type` (spec line 124), a place the function may change or a value, and *without changing the
  counter* reads to every reader as *not `@`*.

## Disagreements, stated plainly

- **Q2, B or C.** Three seats adopted the mark B; the spec-warden adopted C on Principle 0. **What
  decides it here**: C was built, refuses every copy shape on three platforms, moves 0 of 905 files,
  adds no word (so no formatter can drop it, the critic's § 3), and costs 0 spec tokens; B's word is
  a new form whose thesis effect beyond C's reach (`z_stream`, `TAILQ`) is unmeasured, whose
  prototype refused defect 413's repair once marked, and which owes CL-036's whole walk. The ranking
  question (CLAUDE.md's *robustness above Principle 0* against design.md §1.12's *does not suspend
  Principle 0*) is put to the author below; the sitting reads them as consistent, robustness deciding
  *that* the class closes and Principle 0 *how*.
- **Q1, which G.** The ffi-pragmatist's gap macro and the compiler-engineer's first prototype were
  each unsound on a shape the other had not built; the recursive G as built is sound on every shape
  measured, union records left whole, and names what stays open.
- **Q3, what the A2 miss means.** Panel 194's warden registered withdrawal on a miss; the critic read
  the miss as the specification's; the re-run measured it: the same tasks, launcher and compiler,
  and A2 went from 1 to 10 of 10 when the two sentences changed. R1 is kept.

## Resolution (provisional: author ratification pending)

**R1, Q1: route G recursive, the header's membership, before every lend to C.** As built in the
second round: the bytes of a group record that no C member holds are zeroed in place before each lend
to C, every span a clang-checked `offsetof`/`sizeof` of a member the dump names, recursing into member
records and array elements and the region `hero_lend_take` fills, nothing written inside a record
whose C type is a union. **No sentence of the specification** (the spec-warden's ruling): the
guarantee is a fact of the generated C, written in `.claude/rules/generated-c.md` § Arithmetic and
memory once the landing holds it (the compiler-engineer's H), naming what stays open. **The landing
owes**: the four modules past their ceiling split at their seams, never a ceiling raised to fit; the
26 moved emissions blessed after reading every line; the shapes measured open each filed as an issue
by its class (padding bits inside a named bit-field's byte; a C array the binding does not declare;
a record handed to C by value; bytes dirtied between calls through an address C kept; the size of a
deep `partial` record's dump); and the per-lend run-time cost measured.

**R2, Q2: route C, by type identity, with the ffi-pragmatist's three corrections.** A group record
whose C struct holds, at any depth, a member of a standard lock type that is a record or a union
(POSIX XSH 2.9.9's barriers, condition variables, mutexes, read-write locks, spin locks and
semaphores; C11's `mtx_t` and `cnd_t`; Windows' `CRITICAL_SECTION`, `SRWLOCK` and
`CONDITION_VARIABLE`; each asked only where the program's own headers declare it) is not a value:
a binding of it from a place, a by-value parameter, a return, an element of a `[T]`, and a Heroes
record or array holding one by value, copied, are refused at `build` with `ffi_lock_copied`, a new
diagnostic class, naming the record, the lock's C type and path, and the copy. **No sentence of the
specification**: the message carries the fix (design.md §4.17). What C cannot see is recorded, not
closed: `z_stream`, `TAILQ_HEAD`, scalar spinlocks, `os_unfair_lock`.

**R3, Q3: R1 (`rest: zero`) stands, panel 178's condition is void, and § 13's two sentences and
§ 9's `@` sentence become `m-g-at`.** The spec-warden's draft, word for word as the re-run's readers
read it (sha256 `ac4ff3e2...`, kept as `215-reports/llm-ergonomist-work-2/heroes-spec-m-g-at.md`):
§ 9's *Semantics: copy in, copy out (copy-out always happens, including on early return and `?`)*
becomes *Semantics: the callee works on the caller's place itself*; § 13's R1 sentence moves after
`partial` and reads *... every field sharing no byte with one it names is zero, those `partial`
leaves out included*; and the union sentence becomes *A field is a name C reads as `s.name`: an
anonymous union's or struct's member, or a header macro's. Where C reads `s.u.n`, `u` is the field,
typed by the record of its union's typedef, or left to `partial`. A union's members share its
bytes: ...*. Cost **+60 real**, 10,108 against 10,240, the FFI floor's 60 held. **The landing owes**,
by CLAUDE.md § 12 (*spec beats compiler*): the emitter reads a field through an `@` parameter in
place (today `t2 = (*ph0_c); t3 = t2.n;` copies the whole record at `-O0`, a held lock's bytes
included, the critic's § 4), so § 9's sentence is true of the emitted C; the ledger's row; the
`spec` suite's pins.

**R4, what M-buildable-structs owes**: issue 1334 is R1's landing, 1335 is R2's; issue 1333's route
T does not return (A1 read 10 of 10 first try, its condition *fails at least half*), so 1333 closes on
that measurement; defect 2062's note is batch 17's (the ffi lane), its premise corrected on its issue.

**R5, the routes refused**: Q1's B (does not close, measured), C (`cret.hero` loses the bytes in C's
own by-value return), D (C would receive another address, against spec § 13 and panel 196's R1),
the naive G and the gap macro (a union's arm zeroed, a wrong value at exit 0), E (a group record is
the header's struct and gains no field), F (refuses every correct binding of `write`, `send`,
`memcmp`, `SHA256_Update`); Q2's A and A2 (the copy happens), D (dominated by C, unbuilt); Q3's B
(R1's 560 lines and 24 cases, and the miss was the text's).

### The question to the author

1. **Which governs a new word: CLAUDE.md's ranking (robustness above Principle 0) or design.md
   §1.12's *it does not suspend Principle 0*?** *Default while unanswered*: the sitting's reading,
   both, robustness deciding that a class closes and Principle 0 how; so R2 lands as a refusal and
   the word B waits. *Recommendation*: confirm that reading, because it is what both texts say when
   read together (§1.12: *what §1.12 decides is the shape of a form already admitted*), and it let
   this sitting close the measured class without a word.
2. **A read-only lend.** The re-run measured why readers copy a lock: a Heroes parameter is a place
   the function may change or a value, and ten of ten chose the value to say *I do not change it*.
   R2's refusal tells them `@c`, the signature every reader rejected as false. *Default*: R2 lands
   with `@c` in its note. *Recommendation*: **convene a sitting on a read-only lend** (a parameter
   whose place C may read and the function promises not to change), with the re-run's task C as its
   instrument, since the thesis effect is measured (10 of 10) and the form is the language's.

### What the conservative resolution would have been, so the author can choose it

Q1 A (a sentence in `generated-c.md` that padding is not promised, nothing zeroed); Q2 A2 (the lock
left undeclared under `partial`, documented, no refusal); Q3 R1 kept with its old sentences and
panel 194's test marked lapsed. Each leaves a measured wrong value or a measured miss standing:
`memcmp` and SHA-256 changing between runs at `-O0` on x86-64, ten readers of ten copying a lock with
nothing said, nine of ten failing a union binding.

## Prior art

Panel 215 (this file, `docs/panel/215-reports/historian.md`), every link re-read 2026-10-14. The
sitting's decision issue carries the same lines.

- **Taken from POSIX**: [XSH 2.9.9, Synchronization Object Copies and Alternative Mappings](https://pubs.opengroup.org/onlinepubs/9799919799/functions/V2_chap02.html): the section covers barriers, condition variables, mutexes and read-write locks whose process-shared attribute is `PTHREAD_PROCESS_PRIVATE`, and extends its rules to spin locks, semaphores and ISO C's `cnd_t` and `mtx_t`; "only the synchronization object at the address used to initialize it can be used for performing synchronization", and "The effect of referring to a copy of the object when locking, unlocking, or destroying it is undefined."
- **Reached first by bindgen**: [nocopy](https://rust-lang.github.io/rust-bindgen/nocopy.html): bindgen "will attempt to derive the `Copy` and `Clone` traits on a best-effort basis", and the `nocopy` annotation in the C doc comment, `Builder::no_copy` or `--no-copy <regex>` prevents it for a type; the page's example is a struct holding a `pthread_mutex_t`, "not safe to move because pthread mutexes can't move in memory".
- **Reached first by Swift**: [Mixing Swift and C++](https://www.swift.org/documentation/cxx-interop/): "C++ structures and classes with a deleted copy constructor are represented as non-copyable Swift types (`~Copyable`)."
- **Reached first by the SEI CERT C Coding Standard**: [DCL39-C. Avoid information leakage when passing a structure across a trust boundary](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/declarations-and-initialization-dcl/dcl39-c): "When passing a pointer to a structure across a trust boundary to a different trusted domain, the programmer must ensure that the padding bytes and bit-field storage unit padding bits of such a structure do not contain sensitive information."

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | its first report's: G lands in at most 250 lines in `emit/`, `cli/` and `runtime/`, 0 in `check/` and `ir/`; `emission` moves at most 10; MemorySanitizer on `heroes-linux` reads 0 for `fieldlend`, `cret` and `rzpad` at `-O0` to `-O3`. **Scored by its own second-round build**: lines **falsified** (+480 −11, 0 in `check/` and `ir/` held), emissions **falsified** (26), MemorySanitizer **held** (0) | scored 2026-10-14 on the prototype; again at R1's landing |
| ffi-pragmatist | R2 with the three corrections moves 0 of 905 files at `--emit-c` on Darwin and glibc; on the Windows box the four copying programs are refused and `wkept.hero` prints `2 1 5 0 3`; raylib's and SQLite's emitted C stay byte-identical | R2's landing |
| spec-warden | the specification reads 10,108 real at R3's landing, `spec` green once its pins move; the re-run's thresholds (scored: A2 10, U 10, `u8` 0, C 10 correct and 10 copying) | scored 2026-10-14; the count at the landing |
| historian | `u8` under 3 of 10 under R1 with K | scored: 0 of 10, twice |
| llm-ergonomist | with R2 and its message shown, at least 8 of 10 readers correct on task C's second try; with a read-only lend, at most 2 of 10 copy the lock first try | a re-run at R2's landing; the read-only lend's sitting |

## Process notes

- **The paid reader runs were the sitting's decisive instrument, and the first was misread.** The
  coordinator attributed A2's misses to a compiler message the readers never saw; the critic's pass
  2 found it, the report carries a dated correction, and defect 2062's premise is corrected on its
  issue. Twenty readers, 4.1950 USD of the author's 10; the `--refresh` counts appear in no cost the
  CLI reports.
- **Every route that reached the build was corrected by building it**: the gap G, the first
  prototype G (a union clobbered), and C three times. None of these was visible in a report.
- **The session restarted at about 12:30** and stopped the three second-round seats and three lanes;
  each was resumed from its own files, nothing lost.
- **Slips outside the repository's root**, each removed at once and recorded in its report: the
  compiler-engineer's first pass (one file under `/tmp`), the ffi-pragmatist's first pass
  (`/tmp/dummy_never_used`), the spec-warden's second (three `heroes fmt` outputs in the session
  scratchpad).

## Author's verdict

Pending: https://github.com/heroes-lang/heroes/issues/2070 asks for it, naming the sitting as `panel 215`. A yes settles R1 to R5 as
written and the two questions above the way the author answers them.

**RATIFIED, 2026-10-16**, R1 to R5 as written above, the author answering `1a 2a 3a` in
conversation, between 17:31 (the compiler's build time, read before the question) and 19:46 (the
clock read after the answer), on the coordinator's `/decide` list, which put each route with its
measurement and a recommendation: *1a*, the five resolutions over the conservative resolution;
*2a*, question 1's reading, robustness deciding THAT a measured class closes and Principle 0 HOW,
so R2 lands as a refusal and the word B waits, written into CLAUDE.md § Precedence at `df89c0df2`;
*3a*, question 2's recommendation, a sitting on a read-only lend, the re-run's task C its
instrument, issue 2098 of M-buildable-structs, R2 landing with `@c` in its note until it sits.
Recorded as a reading (CLAUDE.md § 4). The author may overturn it.

**What the coordinator verified before asking**, on the tree at `3e389b024`: nothing of R1 to R3
has landed (`ffi_lock_copied` is in no file under `selfhost/`, and § 9 still reads *copy in, copy
out*); the specification has not moved since 2026-10-13 and differs from `m-g-at` in R3's three
places alone (`diff`); `heroes measure` reads 10,048 real, so R3's 10,108 stands against the
ceiling of 16,384 raised on 2026-10-14; and two `@` arguments reaching one place are refused
already (`aliased_mutable_arguments`, run on `f(a: @x, b: @x)` and `g(@p, @p.x)`), so § 9's new
sentence changes no program's meaning. **Two things the landing owes that the sitting did not
list**: the four messages and one note that still explain `@` by its copy-out
(`selfhost/resolve/errors.hero:202`, `resolve/writes.hero:307` and `:311`,
`resolve/written_nowhere.hero:79` and `:96`), to be reworded with R3's sentence; and
`aliased_mutable_arguments`'s first `fix (guess)`, which proposes the copy R2 refuses for a record
holding a lock (the spec-warden's own finding, `215-reports/spec-warden-drafts.md` § Found while
building the key). Both stand on the milestone's page.
