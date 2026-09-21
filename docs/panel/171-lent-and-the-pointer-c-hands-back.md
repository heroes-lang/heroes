# Panel 171 — `lent`, and the pointer C hands back

Convened 2026-09-20, at M-declared-extents, on **the word for a parameter that
does not keep, under a default that assumes it does**. Full panel: five seats
and a completeness critic, six briefs written to disk before any seat started,
the critic run **before** this synthesis, and the working tree frozen throughout.

**The DIRECTION was not on the ballot.** The author ruled it that evening, after
making one condition and having it measured
(`docs/records/log/2026-09-20-1930-heroes-must-be-robust-and-the-default-flips.md`,
`docs/measurements/038-…`): *a C pointer parameter is assumed to KEEP what it is
handed unless its declaration says otherwise, so a lend may reach only a
parameter so marked.* On that half this file is a **retro-record**: it records
the seats' real objections and stages no dissent. On the word, the exact rule
and the landing it is an ordinary sitting.

## The question, verbatim

> The word says *this parameter does not keep what it is handed* and is read on
> `cstr` and `ptr`. What is it; what exactly does the rule refuse and admit, and
> at which stage; what does the diagnostic say; how does it land without
> breaking the compiler that compiles itself; and what does it cost the corpus?

## The resolution, in one line

**The word is `lent`, in its own grammar slot beside `counted_by`.** A lend
reaches **only** a parameter declared `lent`, and only on an `extern` function —
so a Heroes function taking `cstr` can never receive a lend, which is the
soundness clause the engineer's veto turned on and which the spec sentence
carries by construction. **It lands in two commits or the bootstrap breaks**,
tested. It touches **27 parameters on 19 functions across three trees**, two of
which nobody had counted and either of which, forgotten, stops every program or
every suite. And **the flip closes defects 066 and 068 and leaves two shapes it
never claimed: defect 070, and a lent pointer that C hands BACK** — found by the
critic, older than the flip, and written down here rather than promised away.

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **verdict** | **approve**, veto if the rule admits a lend into a Heroes `cstr` parameter | **approve**, no ABI break, veto if the word ever reaches emitted C | **approve**, measured | **approve `lent`**, object `reads` and `borrows` | **approve**, advisory |
| **section** | §1.7, Part 5; §1.12, §4.19; §4.17 | §1.11; §4.19's retention paragraph | §1.6, §1.0 closure list, §1.12, §1.2 | — | — |
| **cost** | **+113 code lines, eleven files, frontend only**; zero in IR, descriptors, ownership, emitter; emitted C byte-identical; `check selfhost/main.hero` 19.52 → 19.41 s | none at run time; 24 probes byte-identical | **+13 real / +9 vendored** for the sentence and the slot, under `DELTA_GATE` | — | — |

**No budget veto.** Baseline 8201 real; the dearest draft on the table 8247.

## THE WORD, and four seats reached it from four inputs

**The llm-ergonomist**, reading nothing but the spec, ran three bindings rather
than the brief's two — adding `read(2)`, which does not keep but **writes**
through the lend, the shape beside the pair where a word can break. With the
proposed sentence open, all four candidates score 3/3. **Cold, word alone:**

| word | cold | where it fails |
|---|---|---|
| **`lent`** | **3/3** | nowhere |
| `transient` | 3/3 | second; wants +7 words of gloss |
| `reads` | 1/3 | marks `keep_label` because it *reads*, leaves `read(2)` unmarked because it *writes* |
| `borrows` | 0/3 | marks `keep_label` because *"the call keeps"* |

In both failures the silent cell is **the same program**: the mark admits the
lend, C keeps a pointer it does not own, and it compiles — the exact program the
ruling exists to refuse. `lent` is the participle of the verb § 13 already spends
three times, and it matches `owned`'s form. Its sentence was 39 words.

**The spec-warden** priced the same field: one-token against two-token words
differ by exactly +3 real; **`reads` contradicts the document's own line 369**,
which says C writes back through a `@` lend, so `ptr counted_by n reads` would
deny the sentence beside it; `lent` needs no gloss because the paragraph uses
*lend* seven times.

**The historian** found the exact-shape precedent. D's druntime binds libc in D
files and writes, today:

    char* getenv(scope const char* name);
    double atof(scope const char* nptr);
    FILE* fopen(scope const char* filename, scope const char* mode);

and leaves **unmarked** the functions that keep — `atexit`, `setvbuf`, `putenv`.
**A word in the binding file, written by the binding author about a library they
did not write, read at the caller, negative polarity, pessimistic default.**
Swift's API notes do the same for blocks only; for `const char *` Swift has no
word and documents the undefined behaviour instead.

**And `borrows` is a trap rather than a non-local construct**, so no veto: the
ergonomist found it is **already in `CParam`** for `@` out-parameters, so its
second meaning would sit on the same production, told apart by the `@`.

## THE SENTENCE, priced on the instrument that judges it

The warden's cheapest complete draft, **M2**, applied to the real path with
`--refresh` and reverted, in its copy:

> *A lend lives for its call and no longer: a parameter is taken to keep what it
> is handed unless declared `lent`, and a lend reaches only one so declared.*

**8201 → 8214 real, +13; 6159 → 6168 vendored, +9.** It spends the clause the
flip makes false — *and nothing checks it*, worth −8 alone — and **merging won by
72%** against appending, because the joined sentence loses two clauses. The
warden's rule from two sittings' data: *merging wins when the joined sentence
loses a clause, loses when it gains a qualifier.*

**The critic reproduced the three prices and settled that they are three
sentences**: the engineer's +46 is its own draft, called *a draft for the
warden*; the ergonomist's +42 carries a gloss and the clause *takes a lease*,
which the warden's condition (iv) flags as premature. **M2 is the sentence that
lands.**

**And M2 covers the engineer's veto clause by construction.** `function wrap(s:
cstr lent)` is refused at parse — a Heroes parameter cannot carry the word — so
*a lend reaches only one so declared* excludes every Heroes parameter without a
second clause. The critic confirmed it: yes, by construction.

## THE RULE AS BUILT, and the engineer built it

Refused at `check`, on the AST, a lend expression — `s.cstr()`, `cstr(s)`,
`f.ptr()` — standing as an argument of a call whose callee is:

1. **not an `extern` function** → `lend_needs_a_header`, new for `cstr`; the
   `ptr` twin is panel 166's. **This is the soundness clause (b10).** Panel 122
   admitted a lend into a Heroes `cstr` parameter *because legality depended on
   what another file declares*; under the flip that is true of every lend, so
   the ground is spent. `wrap(s: cstr)` forwarding to an unmarked C parameter,
   called with `"12".cstr()`, hands C a kept pointer through a route the literal
   rule cannot see. Built: exit 1.
2. **an `extern` whose parameter at that position lacks `lent`** →
   `lend_kept`, new, **no `Fix`**, two notes both `guess`: adding the word is a
   claim about C nothing here can check, and a lease is wrong where C frees.

**Admitted at an unmarked parameter, each run**: a lease name, `nullptr`, a
`cstr` C handed back, any name. **The word is refused where nothing reads it**:
`lent_shape` on an `@` out-parameter, on an `i64`, on a handle. **Not reachable
and untouched**: a lend in an `@` position is `not_a_place`, at a handle
`type_mismatch`, bound to an `@` cell `cstr_escapes`.

**Which layer**: `check`, on the AST, and panel 169's open-set worry does not
apply — the rule reads a declaration's parameter and an argument's kind, both
closed sets, and the IR has no `Param` to move it to.

## THE LANDING, and the count went 9 → 11 → 28 → 27 in one sitting

The brief said nine functions at twelve sites. The ffi seat found the embedded
library's two. **The engineer found the third tree and tested the plan**:

| tree | functions | `cstr` parameters | what happens if forgotten |
|---|---|---|---|
| `selfhost/cli/process.hero`, `emit/literal.hero` | 9 | 13 | the compiler does not compile itself |
| `selfhost/library_source.hero:152-153` | 2 | 2 | **every `heroes check` of every program exits 2**: *internal error: a diagnostic landed inside the Heroes library* |
| `tests/harness/shell.hero:44-57` | 9 | 13 | **`layout`, `order`, `canonical` all red**, so no suite of the net can run |

**The critic's census: 21 functions, 29 `cstr` parameters, no `ptr` parameter in
any of the three; required marks 27 on 19 functions.** The engineer's 28 includes
`hero_fs_remove` in `shell.hero`, declared and never called — truthful and
unrequired — and correctly leaves `hero_str_try_from_cstr(p: cstr)` unmarked,
because it receives a name and never a lend.

**Two commits, tested in the engineer's copy:**

- **commit A** — the word parsed, printed and formatted, the rule **not** yet
  flipped. Builds under today's seed in **73.79 s**; fixpoint **byte-identical**
  (76.37 / 3.16 / 76.44 s).
- **commit B** — the rule flipped, the 27 marks, the spec sentence, the
  example fence, the 35 goldens. Builds under commit A's seed in **61.11 s**;
  fixpoint **byte-identical**.
- **one commit** — breaks at `selfhost/cli/process.hero:42:40`,
  `error[expected_params_close]`, **no binary**. Precisely: CI's `cmp` fixpoint
  could still pass a squashed commit; what breaks is the bootstrap chain
  (`seed/README.md` § *the tags are the chain*), the rung before unable to build
  the rung after.

**And the spec moves once, in commit B.** The warden found that
`suite_special.hero:330` lends `":memory:".cstr()` into the spec's own
`sqlite3_open(path: cstr …)` fence, so under the flip **the document's runnable
example is refused** until its line 341 carries `lent`. A fence is compiled
before it is written, so the sentence, the slot and the fence move together, at
+13; an interim step-A sentence would have cost +44.

## THE CORPUS, measured with both compilers over 433 programs

**35 change verdict, no diagnostic is lost.** 64 `lend_kept` and 4
`lend_needs_a_header` on **50 distinct declarations**: `check` 8, `fixedbugs` 8,
`run` 11, `ir` 1, `unsupported` 1, `surface-fixtures` 3, and **`examples/` 3
programs at 8 sites** — panel 170 said eight and it is eight. Two shapes need
rewriting rather than marking: `surface-fixtures/twoarity`, whose
`as_text.say(format: cstr, value: cstr)` is the b10 wrapper, and
`check/fixedbugs-a-lend-laundered-through-a-helper.hero`, which gains one more
true refusal.

**The ffi seat classified every bound function from the world**, four ASan
programs handing each a freed argument: SQLite `open`, `exec`, `prepare_v2`
copy; `bind_text` with `SQLITE_STATIC` is heap-use-after-free and with
`SQLITE_TRANSIENT` silent; `*pzTail` aliases into `sql`; curl copies every
string but `CURLOPT_POSTFIELDS`; `getenv` does not alias `name`; all nine runtime
bindings silent. **`bind_text` admits no true word on its raw declaration**,
because it keeps or copies by its fifth argument: the ledger's wrapper rewritten
with a lease is `check` 0, output identical to `main.expected`, and its emitted
C differs only in `hero_str_cstr` → `hero_str_held` plus the release. A
three-line shim header makes the word true by construction and clang refuses a
wrong shim from the real header.

## THE POINTER C HANDS BACK — the critic's finding, and it is older than the flip

The engineer attacked the rule at b10 and closed it. The critic attacked it at
the shape beside b10 and found **the rule reads the lend EXPRESSION, and the
hazard is the POINTER.** C can hand a lent pointer back, and the rule then treats
it as C's own:

    keep(s: strchr(s: text.cstr(), c: 'x'))     # strchr(s: cstr lent) -> cstr

`strchr` returns an address **inside** the lent bytes; the rule admits a
C-returned `cstr` at an unmarked, assumed-keeping parameter; C keeps a pointer
into Heroes memory. **Five programs, three doors** — a result, a `@rest: cstr`
out-cell C fills with `s + 1`, a callback C invokes with the lent pointer whose
Heroes body forwards it — **two types, all `check` exit 0 under the rule and
use-after-free under `--sanitize`**, the `ptr` twin via `memchr` a
stack-use-after-scope that prints a plausible value.

**Three things about it that the resolution owes.**

- **The flip did not open it.** Today's compiler: same exit 0, same ASan. It
  left it, exactly as it found it.
- **The corpus already knows the shape.** `tests/golden/run/lease-tail-points-into-the-bytes.hero`
  is panel 124's defect 024, closed with a **caller lease** under the old
  *nothing checks it* sentence.
- **Every draft sentence over-promised about it.** The ergonomist's *a pointer C
  owns* is false of a C-returned value that points into Heroes bytes; the
  engineer's *takes a lease* is false for `nullptr` and for a C-returned value;
  **the warden's M2 is silent about the pointer's next step**, which is the
  honest position and the one that lands.

The critic named three routes and chose none: a word on a result or out-cell
meaning *points into what was lent* (no slot in the vocabulary); treating every
C-returned `cstr`/`ptr` as a lend (the ffi seat's condition objects, and
`getenv → puts` is `check` 0 today); or **writing the boundary down honestly**,
which is CL-005's requirement of a refusal and this sitting's choice. **The
sentence promises what the rule does and nothing more.**

## Three smaller gaps the landing commit owes, each run by the critic

1. **`lent` on an uncounted `ptr` is accepted silently** — dead text. It should
   be `lent_shape`, like `lent` on an `i64`.
2. **Two modules may carry two contradictory marks on one C function**, `check`
   0, runs. This is the historian's B.4 live in Heroes: upstream Clang's
   `noescape` broke on its first day because a third-party re-declaration
   disagreed with the SDK header. **Filed, not repaired here**: it needs a rule
   across modules that no sitting has priced.
3. **`lend_kept`'s notes for a `.ptr()` lend say `cstr` of a `ptr` parameter and
   offer a field lease that does not exist**; and following the first `guess`
   note on defect 070's fourth shape — add `lent` — yields `check` 0 and run 134,
   because the *if C frees, neither route works* sentence sits in the second
   note. **The notes are rewritten before the diagnostic ships.**

And the ergonomist's six inventions, checked against the built rule: same on
four, **different on two** — invention 2 (uncounted `ptr lent`, which the
ergonomist wanted loud and the rule leaves silent: gap 1 above) and invention 5
(the lease route offered for a field lend: gap 3). **Both are silences in every
draft sentence**, and both close in the landing.

## THE HISTORIAN, and the cost Swift never published

**No number exists.** Apple's release note is one sentence — *Introduction and
adoption of NS_NOESCAPE* — with no count, no list, no period. Lattner for the
core team called the burden *not overly burdensome* and left it to header
owners; SE-0012, which asked for the annotations, was **rejected**. The first
visible breakage was a third-party re-declaration disagreeing with the SDK header
(compiler-rt, 2017-09-19, reverted the same day).

**The coordinator ran the seat's unrun command**: on this Mac's SDK, **77 of
9,386 headers** carry a `noescape` mark today, ten years after the flip — under
one percent — and at libc level they are `dirent.h`, `glob.h`, `fts.h`,
`_stdlib.h`: `qsort`'s and `scandir`'s **function pointers**, not `const char *`.
Swift's flip produced no mass annotation; the few dozen places that mattered
were marked and the rest stayed pessimistic.

**The standard's own vocabulary is the polarity Heroes adopted.** C11 says
*keeps* in words wherever it holds — `setvbuf`'s footnote 273 (*the buffer has to
have a lifetime at least as great as the open stream*), `atexit`, `strtok`'s
¶2, POSIX `putenv` (*shall become part of the environment*) — and says
**nothing** where a function does not keep: `getenv`'s `name` is *the string to
match*. **Silence is the standard's word for does-not-keep, and a sentence is
its word for keeps.**

**And a binding author getting it wrong, live**: druntime's WASI branch declares
`putenv(const scope char*)` while wasi-libc's `putenv` **stores the pointer**.
The `.d` file says *does not keep* about a function that keeps. **This is why
`lend_kept` carries no `certain` fix**: the person adding `lent` is making the
claim D's WASI binding made.

**A correction the historian owes the brief**: the Clang `noescape` RFC is
**March 2026**, partially approved 2026-07-03, not 2025; and Vala's `owned`
parameter falsifies the brief's *positive polarity ships nowhere as a compiler
error*, recorded so the record is true. **Renames found, none a withdrawal**:
Vala `#` → `owned` (2008), Swift `@noescape` retired by inversion (2016), LLVM
`nocapture` → `captures(none)` (2025), Java `ResourceScope` → `MemorySession` →
`Arena` (JDK 18–21) — every one toward a plainer word.

## The disagreements, stated plainly

**Three prices for what looked like one sentence.** +13 (warden), +42
(ergonomist), +46 (engineer). The critic measured all three on the real
tokenizer and they are three sentences, 31–33 tokens apart; the differences are a
gloss and a clause. M2 lands.

**Four counts for one landing.** 9/12 (brief), 11 (ffi seat), 20/28 (engineer),
**21/29 with 27 required** (critic). The last is the census and it is the number.
**The warden's prediction P2 — exactly 13 marks in `selfhost/` — will score
false for the wrong reason**: `library_source.hero` is a `selfhost/` file, so the
landing writes **15** there. Corrected here before it is scored.

**The engineer's veto condition and the warden's approval reconcile**, and the
critic confirmed how: the header clause the engineer built is what M2's
*only one so declared* already says.

## The resolution — `provisional — author ratification pending` on the word and the rule; the direction is the author's

1. **The word is `lent`**, in its own `CParam` slot beside `counted_by`, never
   inside the `consumes | acquires | borrows` alternation. Read on `cstr` and
   `ptr` parameters of `extern` functions; `lent_shape` anywhere else, **the
   uncounted `ptr` included**.
2. **The sentence is M2**, +13 real, landing in commit B with the fence:
   *A lend lives for its call and no longer: a parameter is taken to keep what it
   is handed unless declared `lent`, and a lend reaches only one so declared.*
3. **The rule**: a lend expression reaches only a `lent` parameter of an `extern`
   function — `lend_kept` at an unmarked one, `lend_needs_a_header` into a Heroes
   function. A lease, `nullptr`, a C-returned value and any name are admitted
   unmarked. **No `certain` fix**, and the two `guess` notes each carry the *if C
   frees, neither route works* sentence.
4. **Two commits, in this order, each with its seed regenerated and its fixpoint
   verified**: A, the word parsed and printed, rule unchanged; B, the rule, the
   **27 marks on 19 functions across `selfhost/cli`, `selfhost/emit`,
   `selfhost/library_source.hero` and `tests/harness/shell.hero`**, the spec
   sentence and slot, the spec's fence, and the 35 goldens — 33 re-blessed, two
   rewritten.
5. **The spec sentence promises what the rule does and nothing more.** It says
   nothing about a pointer C hands back, because the rule does nothing about it,
   and **that shape is written down as what the flip leaves standing**, with
   defect 024's caller lease as the route the corpus already uses.
6. **Defects 066 and 068 close with commit B**: both reproducers lend into an
   unmarked parameter, and that lend is refused. **Defect 070 stays exactly where
   it was**, measured: a lease into an unmarked freer is `check` 0, run 133,
   empty stderr. Its fourth shape becomes `lend_kept`.
7. **One item filed**: two modules carrying contradictory marks on one C
   function, `check` 0 today, the historian's B.4 arriving in Heroes.
8. **Owed by the landing commit and unrun by this sitting** (CL-036): the two
   highlighters, `heroes mutate`, the full net, and the `.expected` regeneration
   that the engineer's corpus diff names but did not perform.

**What conservative would have been, so the author can choose it**: the same
word and rule with the ergonomist's longer sentence, +42, which says *takes a
lease or a pointer C owns* — clearer to a reader and **false of the pointer C
hands back**, which the critic measured. The sitting takes the shorter sentence
because it is the one that is true.

## Predictions to score, at the M-declared-extents close

- **compiler-engineer**: commit B's pathspec contains `selfhost/library_source.hero`
  and `tests/harness/shell.hero`, else `./heroes check tests/golden/run/ffi-cstr.hero`
  exits 2 and `layout` prints `lend_kept` at `shell.hero:526`; `DECIDED` reads
  `selfhost/ast.hero 527` (±1); `lease_errors.hero` measures 63 ± 10; the
  `check` form carries 8 ± 1 changed `.expected`.
- **ffi-pragmatist**: on commit B unmarked, `heroes check` gives exactly 8
  refusals in `examples/` at the lines it named; after marking six SQLite
  declarations and rewriting `bind_text`, the ledger matches `main.expected` and
  its 24 probes are byte-identical.
- **spec-warden**: the landing reads **8214 ± 3 real**, vendored ≤ 6172;
  `examples/` `.lease()` count stays 1. **Its P2 is corrected before scoring:
  15 marks in `selfhost/`, not 13**, because the library text lives there.
- **llm-ergonomist**: ten trials per word on three bindings, `lent` ≥ 9/10
  correct; on the bare grammar line, `lent` ≥ 8/10 answer *C does not keep it*.
- **historian**: no binding-side does-not-keep mark is found shipped and
  withdrawn; the contradictory-marks shape (B.4) is met in Heroes within two
  milestones by a program, not a panel.
- **completeness critic**: the C-hands-it-back shape is met in a program within
  two milestones and closed by a caller lease, not by a new word.

## Author's verdict

**RATIFIED 2026-09-21**, in one act with panels 172 and 173, in the author's
words: *then ratify all the DECIDE items.* Recorded as a ratification given in
conversation on the coordinator's summaries in that session, not as a reading
of this file, and **not `by delegation`** — the author's standing instruction of
2026-09-19 is that they read, and the coordinator does not know whether this
file was read (CL-058). The word, the rule, the sentence and the two-commit
landing are confirmed rather than authorised: the whole of them had already
landed at `83a8c92c` and `abf9a17e`, under CLAUDE.md § 3's rule that a
milestone asked for in one `/step` decides its delegated questions with the
recommended resolution as the default and says once which way it went.

**The direction was never on this ballot.** The author ruled the default
flipped on 2026-09-20, before the sitting, and the ratification does not
revisit it. The conservative sentence this sitting recorded, the ergonomist's
longer draft at +42 real, was **not** taken.

**CORRECTION, 2026-09-21, by the author, within the hour.** The author read
this file. The paragraph above is wrong where it says *not as a reading*:
CLAUDE.md § 4's default is that the author reads every sitting, and a
ratification is recorded as a reading unless they say otherwise. The
coordinator read CL-058 as a rule about humility, which is the reading that
paragraph exists to forbid.

What this section said while the item was open, kept because a record is never
rewritten:

> **The direction is the author's, given 2026-09-20 in conversation and recorded
> in the log entry above.** The word, the sentence, the rule and the landing are
> this sitting's provisional resolution and are **queued as `panel 171`** in
> `docs/work/DECIDE.md`. Work proceeds on them as the default: commit A next.
