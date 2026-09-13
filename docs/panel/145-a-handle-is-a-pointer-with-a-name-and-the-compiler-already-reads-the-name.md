# Panel 145 — a handle is a pointer with a name, and the compiler already reads the name

**Sat** 2026-09-13 · **milestone** M-handle-verdict, step 2 · **status**
`provisional — author ratification pending`

**Lane: five seats plus a completeness critic, and for the first time every
brief is on disk** (`panel-145/briefs/`, six files, written before any seat
started, which panel 140 asked for and the author made the rule the same
evening). The historian's report reached the critic verbatim inside its prompt,
so the seat with no file write was audited for the first time since the critic
existed. **The critic changed the resolution, for the seventh sitting in seven.**

## The question, and the three things it is

Every C pointer that is not a `cstr` is one Heroes type, `ptr` (spec § 3). Three
defects, re-run at `481ec6ae` the evening before, are the three things that type
does not carry:

| defect | lacks | measured |
|---|---|---|
| **029** | a **pointee type** | `sqlite3_step(db)` for `sqlite3_step(statement)`: build exit 0, zero diagnostics, **exit 139** on Darwin, **`rows: -1` at exit 0** on Linux |
| **030** | **identity** | two copies of one record advance one C cursor: `a sees 1`, `b sees 2`, `same handle: true`. Closed as filed at step 1, hours earlier |
| **031** | **ownership** | one copy `free`s what every other copy holds: build 0, run 0, `heap-use-after-free, WRITE of size 8` under `--sanitize` |

The sitting had to say which of the three it answers. **It answers 029 with a
form whose spelling the critic corrected, 030 with a refusal whose home already
exists and whose one competing form nobody priced, and 031 with a wart it
declines to close the defect on, because a guard is cheap enough that robustness
says build it first.**

## What was measured, and all of it was compiled

**The grammar's only nominal shape refuses to be a handle.** Three seats
independently compiled `record Db tag sqlite3 partial` with no fields inside an
`extern` group: **`error[empty_record]`**, from `selfhost/parse/tails.hero:243-252`,
before the checker sees it — and the caret lands on the **next** declaration.
Reproduced by the coordinator.

**Give it one dummy field and the checker does the whole job.** The engineer's
`rec_field_swap.hero`: `sqlite3_step(db)` → **`error[type_mismatch]: expected
Stmt, found Db`** at the line that is `main.hero:74`. No new rule fired;
`Ty.named`'s nominal equality (`check/table.hero:76-79`) did it. **029's class
dies in `heroes check` the moment the parameter type is nominal.** Reproduced.

**What breaks is the representation, not the type.** A group record IS the
header's struct by value (`emit/ctype.hero:367`), so the correct program dies in
the emitter: `@out: Db` emits `struct sqlite3 *` where the header wants
`sqlite3 **`, `db: Db` emits `struct sqlite3 h0_db` **by value**, and the probe
reads `sqlite3_close((struct sqlite3){0})` — 20 clang errors, all *incomplete
definition of type*. The FFI seat measured it on Darwin and Linux. **The seam is
exactly between checker and emitter.**

**The C side, hand-written against the real `sqlite3.h` under the flag this
project already carries** (`-Werror=incompatible-pointer-types`,
`selfhost/cli/flags.hero:51`). Reproduced by the coordinator:

| spelling of the handle | correct program | `sqlite3_step(db)` |
|---|---|---|
| `typedef sqlite3 *h_Db;` — the header's own type name, starred | compiles, `rows: 3` | **clang error**: *incompatible pointer types passing `struct sqlite3 *` to `sqlite3_stmt *`* |
| `typedef void *h_Db;` — today | compiles, `rows: 3` | **compiles**, exit 139: C cannot tell them apart |
| `typedef struct sqlite3 h_Db;` — what a group record means today | never compiles: *incomplete type* | — |

So a handle that can be told from another is **a pointer with a Heroes name,
never a struct**. Spelled as the header's own type name, clang becomes a second
judge of the swap at zero cast and zero shim — and that type text,
`sqlite3_stmt *`, is what `selfhost/cli/pointee.hero` **already fetches** from
clang's AST dump for every extern parameter and discards at line 115 (*"not
numeric"*).

**And `struct <tag> *` is the wrong spelling for two of the three handle types in
this tree** — the critic's finding, reproduced: on this Mac's SDK `curl/curl.h:112`
reads **`typedef void CURL;`** (a struct only under `CURL_STRICTER`, `:109`), and
`FILE` is a struct with a different name on Darwin and on glibc (the critic's
reading, not re-run here). `record Curl tag Curl_easy` has no struct to name and
`record File tag __sFILE` is not portable. **The portable name is the one the
header gives, verbatim** — `CURL`, `FILE`, `sqlite3` — which is exactly the text
the pointee instrument reads. Every seat built the form for one handle type of
three and none said so.

**`owned` on a `ptr` parameter parses and is misread.** `function
sqlite3_finalize(statement: ptr owned sqlite3_finalize)`: the checker retypes
**any** `owned` parameter to `str?` (`check/freer.hero:90-93`) and answers
`error[type_mismatch]: expected str?, found ptr` plus `owned_freer_called` — two
diagnostics about a string on a program that has none. Three seats found it; the
coordinator reproduced all three spellings.

**§ 9 extended across calls would catch nothing** — the critic compiled the
question nobody asked: `sqlite3_step(statement: db)` passes `heroes check` at
exit 0 (reproduced). A name protects the position, never the value; the
ergonomist's H3 said so from the text alone.

**`heroes check` cannot see a wrong tag**: `record Stmt tag no_such_struct
partial` checks at exit 0 (the critic's `bogus_tag.hero`, reproduced). Every
header fact is build-time in this compiler, and a handle's tag is one more;
`heroes mutate`, whose fate is `checker.check`, will never see one.

**The count the milestone file said was owed.** The FFI seat enumerated every
call of the 27 `ptr`-naming externs rather than trusting the census: **27
`ptr`-passing sites in `examples/`, 30 in the tree, 0 in `selfhost/`**. Sites
passing a handle from a different producer than the header expects: **0 of 27**.
Sites where 029's mutant can be *written*, a second handle of another pointee in
scope: **5 of 27, 18.5%**, all in `first_int` (`sqlite/main.hero:63,74,75,76`)
and `prepared` (`ledger/db/sqlite.hero:266`). The engineer's independent awk
found the same two functions. The ledger already writes the nominal handle by
hand — `record Db`/`record Statement` at `sqlite.hero:194-198` — at the price of
**13 `.handle` unwraps**, and inside the binding module those 13 sites are
unprotected.

**The coordinator's census was wrong in the direction that understates
exposure, and two seats plus the critic corrected it.** It said *four
out-parameters including `fopen`'s shape*: `fopen` is in no `examples/` file and
returns `FILE *`, it is no `@`; measured, **3** out-parameter shapes on 4 extern
lines. It said *two pointee types in `examples/sqlite`*: `@tail` is `const char
**` and `sqlite3_exec`'s `callback`, `context`, `error` are three more C types,
so **six** collapse to `ptr` in that one file. It said *six hand-on sites*:
`main.hero:63` is a **seventh**. It said *zero `ptr` in `selfhost/`, so Principle
0's first branch is unmet*: true at the `extern` keyword, and it hides where the
compiler's handles went — `runtime/parts/*.c` holds **5** lines of `FILE *`,
`DIR *` and `pid_t` (the critic said 17; the coordinator's grep with the critic's
own pattern reads 5), so the compiler's own handle-bearing code was written **in
C, where C's type system tells a `FILE *` from a `DIR *`**. Branch one is unmet
because the compiler took the exit the thesis says a Heroes program should not
need.

**The spec, priced alone per route** (warden; vendored, `real` estimated at
1.33): the nominal handle merged into § 13's `record` paragraph **+62**, with the
§ 13 example rewritten to use it **+69**; a WART clause in § 3's `ptr` row
**+27**; `owned` on a consuming parameter **+65**. Two removals found: § 13's
package-paragraph reason clause **−17**, and § 3's *"and only a group's `record`
may hold one"* **−14** — **which the critic showed is not safe**: that sentence
reconciles § 13:349, where `cstr` is a legal group-record field, with § 13:356,
*"no record holds one"*. So R2 is refused and ENTERS with the example and R1
alone is **+52 net → 5774**, estimated real ~7680 against 8192. **The budget veto
fires for no verdict this sitting can reach.**

**The reader wrote the bug without hesitating** (ergonomist, spec only): its
first-try program declares `db: ptr` and `statement: ptr`, and its hesitation
table has exactly **one silent row, H4**: `sqlite3_step(db)` under today's text.
Under the fieldless-record text the swap becomes a type error at the call —
**local**, read from the call line and the signature, no veto. It found **one
silent re-opening, H9**: § 13's own fence still teaches `@out: ptr` and
`sqlite3_close(db: ptr)`, so a model copying it writes `Stmt` only for the new
functions and `sqlite3_close(statement)` is accepted by *"a `ptr` still takes
any"*. **The warden reached H9 from the other side**: an opt-in form with the
example unchanged buys nothing under §1.2, because the program a model writes
from the prompt never reaches for it. And it found **H8, which the critic traced
to the coordinator**: `partial` was planted by the brief's own spelling, § 13:351
makes `==` on a `partial` record a compile error, so `db == nullptr` would be
refused. Two seats dropped `partial` silently, one kept it and hit H8, one
replaced it with `handle`; **four spellings of one form** and nobody said the
brief's spelling was what they were varying.

**Precedent is unanimous in direction and split only on who pays** (historian,
every claim sourced and dated). Swift's `OpaquePointer` is Heroes' exact choice,
one type for every incomplete struct, and its designers wrote it down twice as a
regret: Weiss 2018, *"less typesafe than C"*; Rose 2021, *"Swift Regret"*, kept
only because fixing it is ABI-breaking. Every FFI that **reads the header** types
per struct (cgo's `*C.sqlite3`, Zig's `opaque {}` since 0.7.0, bindgen); every
hand-written one sells a one-line type per handle (Nim's `distinct pointer`,
Haskell's `Ptr Sqlite3`); the ones that kept one type check at **runtime** (Lua
light userdata, Java FFM) or wrote regret. **Wirth kept `ADDRESS` in Modula-2 and
deleted it in Oberon-07.** For ownership the compile-time refusal has ancestors
without a wrapper (Cyclone's unique pointers, Rust's moves), and the one language
that lets a binding author mark the imported function's *parameter*, Nim's
`sink`, **copies instead of refusing** — the exact wrong turn. Identity has no
ancestor at the raw handle: everyone wrapped.

## Verdicts

| seat | 029 pointee | 030 identity | 031 ownership |
|---|---|---|---|
| `compiler-engineer` | **ENTERS as route A only** — fieldless group record spelled as a pointer, 60-90 lines, **0 `Ty` cases**; **veto** on a `Ty` payload, on the pointee instrument feeding the checker, and on panel 109's counted `ptr` | **WART**; the § 3 sentence is the ruling; an affine handle is core, veto if proposed | **WART**, remedy the consume mark with rule (i) first, 120-200 lines in a new module, two files at ceiling |
| `ffi-pragmatist` | **ENTERS** as a nominal handle spelled as a **pointer**; **veto** on the group record if it means a struct by value | **REFUSED**, Part 6; a refcounted box is an **ABI break, veto** | **WART**, remedy one word on the C parameter, zero C-side consequence |
| `spec-warden` | **object** to opt-in with the example unchanged; **approve** in the header-checked form with the fence rewritten; else WART. No budget veto | **approve** the sentence as the ruling, 0 tokens | **object** to the consume clause at +65 unpaid; **WART** with `--sanitize` |
| `llm-ergonomist` | **object** to today's text; **adopt-with-condition** the fieldless record — local, no veto; conditions: `nullptr` its null, `Fields` optional, the fence rewritten | **stays-a-wart**, the sentence is the ruling | **defer**; the document is silent and the grammar over-offers; `owned` on a `ptr` must be a loud refusal |
| `historian` | **approve** ENTERS | **approve** REFUSED | **approve ENTERS** as a flow rule, with Cyclone's price named |

## Where they disagree, unsmoothed

**"Veto on the group record as the handle" against "ENTERS as the fieldless group
record".** The FFI seat vetoes the record; the engineer adopts it. To the letter
they are one verdict: the FFI veto is against the record meaning **a struct by
value** (its route C, which never compiles), and the engineer's route A spells
the fieldless record **as a pointer**, which is the FFI seat's route A. The
disagreement is about one word, *record*, meaning two representations — and the
critic asked the question nobody put: **may one word mean by-value with fields
and pointer without?** The resolution answers it by fact rather than by decree: a
fieldless group record **cannot** mean the struct by value, because an
incomplete type has no size and clang refuses it (measured, 20 errors), so the
pointer meaning is the only one the shape can carry. No seventh contextual mark
is spent on a shape with one meaning.

**Whether the consume mark refuses `uaf.hero`.** The engineer: rule (i), *consume
through a borrowed parameter is refused*, refuses the program **as written** at
`free(h.cell)` inside `closed(h: Holder)`, because `h` is a plain parameter. The
FFI seat and the warden: the mark **cannot** refuse it, because the free is
through copy `b` and refusing the write at line 22 needs inference through a
Heroes body. **Both are right about different lines, and the critic showed the
two seats ruled against a rule they had not considered**: the warden priced only
same-name liveness and the FFI seat only the direct shape. Rule (i) refuses line
14 and the program dies there; nothing refuses line 22, and the engineer says so
itself — repaired to `closed(@h)`, `closed(@b)` is legal and ASan still fires.
**It catches the program, not the class; the class is the copy.**

**And the engineer's own number for rule (i) is falsified by reading.** It
predicted *0 of 17 shipped sqlite calls refused*. The critic found two:
`ledger/db/sqlite.hero:226 closed(db: Db)` calls `sqlite3_close(db.handle)` and
`:271 finalized(statement: Statement)` calls `sqlite3_finalize(statement.handle)`,
both through a non-`@` parameter. Reproduced. **Rule (i) refuses 2 of 17, and the
repair is the `@` step 1 already applied to `stepped`** — which is also exactly
the measurement Part 6's borrow-checker row names as its falsifier.

**WART or REFUSED for 030.** Two seats say wart, three say Part 6 row. The
warden's point decides the words: the Part 6 borrow-checker row, corrected at
panel 139, **already carries identity and names its falsifier** — *an annotation
with a measured refusal rate below 13 of 17*. **And the critic connected what no
seat did**: the consume mark refuses **2 of 17**, so adopting 031's remedy
**dissolves the ground of 030's refusal by that row's own words**. The row stays
a refusal and its ground moves from *cannot be written from the headers* to
*cost*, which is a fact the row must carry when the mark lands.

**The historian's ENTERS on 031 against three WARTs.** The historian has the
ancestors right and names the price: *a consumed handle may not be aliased*, and
Heroes copies records freely. That price is what the engineer measured as the
class rule (i) cannot reach. The sitting takes the ancestors as the reason the
return condition is a **prototype** and not a sentence.

**The engineer and the FFI seat agree on the measurement of `@out: Db` and
disagree on the price**, the critic found: the engineer's 60-90 lines never touch
`emit/extern_probe.hero:202 is_cast_out` (returns false for `.named`) or
`emit/assert_spelling.hero:188`, which spells a `.named` argument's probe zero as
`(struct X){0}` and for a handle must spell `(X *)0`. Both files are in the
price now.

## The critic's answer, and what it changed

Beyond the corrections woven in above, four findings changed the resolution:

- **A route nobody listed for 029: spell the handle by the header's own type
  name, not by `struct <tag>`.** Every seat built `struct sqlite3 *` and it
  reaches one of three handle types in the tree. The form adopted below spells
  `<tag> *` verbatim, which is portable because it is the header's spelling, and
  is the text the pointee instrument already reads.
- **A route nobody listed for 030: the affine handle**, the only rule that gives
  a `ptr` identity and refuses `uaf.hero` as written (`b: Holder @ a` a move when
  the record holds a `ptr`). Named twice, priced by nobody. **It stays unpriced
  tonight and is owed a count at the prototype step**: the copy sites in the
  checker (assignment, argument, construction, `push`, `return`, `@` copy-out) and
  the `copy` descriptor per type. **And the reserved question — the type's name,
  `unsafe_ptr` or a family, which panel 139 reserved for this sitting by name —
  was never handed to any seat.** Zero mentions in six briefs and five reports.
  **It is not decided tonight and the sitting says so** rather than deciding it by
  omission; it goes to the landing step as an open question.
- **"Did the sitting answer all three, or one?"** 029: answered, for one handle
  type of three until the spelling above. 030: **answered by inheritance** — the
  brief offered two options with one already landed, and all five took it; the
  affine handle went unpriced. 031: **every remedy catches a program other than
  the filed one**, and the filed class, free through a copy, is 030's affine type;
  **so 031's class is refused by transitivity and no row said so until this
  paragraph.**
- **"Is a Part 8 wart admissible for a §1.12 violation at exit 0 in the default
  build?"** Nobody asked. The answer the sitting gives: **only where design.md
  already places the operation outside the guarantee** — §4.10 says in so many
  words that *§4.19's `ptr`/`cstr` sits outside the guarantee*, and §4.19 says
  *treat every `ptr` as opaque and free it explicitly* — **and only while the
  cheapest guard is being built rather than argued about**, because CLAUDE.md
  § Precedence ranks robustness above Principle 0 and says a guard that closes a
  corruption class lands with its cost measured. The wart is therefore adopted as
  the language's honest present-tense statement, **the defect stays open**, and
  rule (i)'s prototype is the next step, not a return condition for someone else.
- **"Should the mutate operator be built first?"** Not as a way to move the
  ENTERS argument (the before-number is 0% by construction) but because **the
  sitting's central number is unrun while its instrument sat priced**: four seats
  disagreed on the denominator (5, 6, 7, ~14) and the warden said the whole §1.2
  case rests on an after-number nobody has. **The operator lands before the
  form**, so both numbers are measured.

## Provisional resolution

**029 ENTERS — a fieldless `extern` record is a handle: a pointer to a type the
header names and leaves opaque, spelled as the header's own type name.** The
form the grammar almost has: `Member`'s `Fields` becomes optional for a group
record that carries `tag`; a group record with no fields means **`<tag> *`**,
the tag written **verbatim as the header spells the type** — `record Db tag
sqlite3` is `sqlite3 *`, `record Curl tag CURL` is `CURL *` — and never the
struct by value, which no incomplete type can be; **`partial` is not part of the
form** (it means *names only some fields* and forbids `==`); `nullptr` is its
null; construction `Db()` is refused; `==` compares the address as it does for
`ptr` today; `@out: Db` emits the address of the cell, which under the header's
own type needs **no cast**; the `_Static_assert` probe compares `sqlite3_stmt *`
against the header, **and clang refuses a swap on its own** under a flag already
in `FLAGS`. **Cost**: 60-90 code lines plus the two probe files the critic
added, **zero new `Ty` cases**, `check/table.hero` and `check/walk.hero`
untouched — the engineer's prediction, scored at the landing step. **Spec**: § 13's
`record` paragraph gains one sentence and § 13's fence is **rewritten to use
it** — H9 and the warden's §1.2 argument are one finding from disjoint inputs.
**Paid** by R1, −17, and the registered predictions; R2 is refused on the
critic's reading. **Owed by the same step, because the route exposed them**:
`ffi_tag.hero:64-80` conflates *incomplete* with *absent*; `freer.hero:90-93`
retypes any `owned` parameter to `str?`; `empty_record`'s caret; and CL-036's walk
of every tool that re-prints a program — formatter, dump printers, `mutate`, the
diagnostics that quote a program, `measure`, the TextMate grammar and the site's
highlighter — which the engineer named two of and nobody enumerated. **What
conservative would have been**, recorded so it can be chosen: the ledger's
hand-written idiom written into § 13 as a sentence, a WART at +27, leaving 3 raw
producer cells of the 5 swappable sites and nothing checked against the header.

**Vetoes standing, all three honoured**: no payload on `Ty.ptr`
(interning-as-identity, `check/table.hero:5-9`); the pointee instrument does not
feed the checker (types would depend on clang at build time, `heroes check`
never runs clang, so `mutate` would never count a kill); panel 109's counted
`ptr owned` stays refused.

**030 REFUSED as a form, and the refusal already has its home.** Identity for a
foreign address is alias tracking; design.md Part 6's borrow-checker row carries
it with its falsifier, and spec § 3 states the behaviour since step 1. A
refcounted box is an ABI break (FFI veto); an affine handle is core (engineer
veto) **and is the one route that would close 031's class, so it is owed a price
at the prototype step rather than a veto by silence**; no ancestor gives the raw
handle identity. **Zero words added to the spec.** One dated line goes under the
Part 6 row: that this sitting confirmed it as 030's home, and that the consume
mark, if it lands, turns the row's ground from *unwritable from headers* to
*cost* by its own falsifier. **Not decided**: the type's name.

**031 WART adopted, defect NOT closed, prototype scheduled.** Part 8 gains an
entry stating the cost in the present tense: *one copy of a value holding a
`ptr` can free what every other copy holds, at exit 0; `--sanitize` names the
line; the class is 030's, refused by transitivity.* The remedy is a **consume
mark on a C parameter** — one word that is **not** `owned`, because `owned`
means *C hands you this and the compiler frees it*, the opposite direction —
with rule (i), *consume through a borrowed parameter is refused*, which refuses
the shipped reproducer as written at the line that frees and refuses **2 of 17**
correct calls in the shipped SQLite binding, both repaired by `@` as step 1
repaired `stepped`. Priced at 120-200 lines in a new checker module with
`ast.hero` (500/505) and `fmt.hero` (1147/1150) at ceiling, **zero C-side
consequence**. **The defect closes when one of two things is measured**: the
prototype at ≤ 150 code lines refusing `uaf.hero` in `heroes check`, or the
author ratifying the wart as the final answer with the prototype's measured cost
in front of them. **What conservative would have been**: the wart with
`--sanitize` alone and 031 closed on it tonight. Refused because §1.12 is a goal
of the language and a class this cheap to prototype is not closed by a paragraph.
**Filed beside it as a `cli-surface` question**: whether `--sanitize` should be
the default of `heroes run` and `heroes test` for a program whose `extern` group
frees.

**The order of the steps, and why**: **step 3 the `swap-ptr` operator** (~50-70
lines in a new `mutate/handles.hero`, ~14 mutants on `examples/`, **0 killed**
today by construction, measured rather than asserted); **step 4 the handle
form** (the operator's kill rate becomes the form's measurement, ≥ 12 of 14 the
engineer predicts once `examples/sqlite` is rewritten); **step 5 rule (i)'s
prototype and the affine handle's count**. RUN IT, or say it is unrun: the
instrument lands before the number it measures.

## Found beside the sitting

1. **`empty_record`'s caret lands on the following declaration**
   (`parse/tails.hero`, `record_tail`, `here_or` after `skip_terminators`).
2. **`ffi_unknown_tag` says a header *declares no `struct X`* when it declares it
   opaque** (`emit/ffi_tag.hero:69` reads clang's *incomplete definition* as
   *absent*), and `ffi_parameter_type`'s note steers the author into the by-value
   record, which never compiles.
3. **`owned` on a non-`cstr` parameter is retyped to `str?` in silence**
   (`check/freer.hero:90-93`); it should be refused at the declaration by the
   declared type's name.
4. **The census a coordinator hands a sitting is a measurement and it was
   wrong four ways** — recorded under the coordinator's name, and the shared brief
   stays on disk uncorrected so the error can be read beside the correction.

## Author's verdict

**Ratified 2026-09-13: all three verdicts as adopted, and the undecided question
answered by scheduling a sitting rather than a name.** The author ratified 029
ENTERS, 030 REFUSED as a form with its home the Part 6 borrow-checker row, and
031 a WART **with the defect held open** until the consume-mark prototype is
measured — which is the point the sitting refused to take the cheap resolution
on, and the author took the robust one.

**On the type's name**, which panel 139 reserved for this sitting and which no
seat received: the author declined both standing options and **ordered a sitting
of its own, after the form lands**, so that it argues over how many bare `ptr`
remain in the tree rather than over how many it imagines. That is a stronger
answer than the coordinator's recommendation, which was to keep `ptr` and treat
a rename as optional: the question is not closed, it is given an instrument and
a date. It is filed as `M-handle-verdict`'s fourth item and it may not sit before
step 4.

**What the yes settles**: the form's shape and spelling, the refusal's home and
its zero words, the wart's text, and the order of the steps — the operator
before the form, the form before the prototype. **What it does not settle**: the
form's measured cost, which is step 4's diff; whether the consume mark ever
enters, which its prototype decides; and the name.

## Predictions to score

| origin | prediction | instrument | scored at |
|---|---|---|---|
| engineer | the landing diff ≤ 120 code lines, `layout` green, no `DECIDED` row raised, `check/table.hero` untouched | `git diff --stat`, `-- ./heroes layout` | step 4 |
| engineer | `swap-ptr`: 0 of ~14 die today; ≥ 12 of 14 once `examples/sqlite` uses `Db`/`Stmt` | `heroes mutate` | steps 3 and 4 |
| engineer, **corrected by the critic** | rule (i) alone refuses `uaf.hero` as written and **2 of 17** shipped sqlite calls, `closed` and `finalized`, both repairable by `@` | a prototype in `heroes check` | step 5 |
| ffi | `sw.hero` retyped `Db`/`Stmt` fails `heroes build` naming line 74; the correct program compiles under the fourteen flags with zero new diagnostics on Darwin and Linux and prints `rows: 3`; §4.19 ladder step 3 needs no shim | `heroes build`, both platforms | step 4 |
| ffi | with the consume mark on the four consuming functions in `examples/`, `uaf.hero` is still not refused at line 22 | the prototype | step 5 |
| warden | at the landing, the spec measures ≤ 5775 vendored and ≤ 7700 real; `sw.hero` in the handle form builds exit ≠ 0 with one diagnostic naming line 74 | `heroes measure`, `heroes build` | step 4 |
| warden | § 3's `ptr` sentence byte-identical at this milestone's close; `two.hero` still prints `a sees 1`, `b sees 2`, `same handle: true` | `git diff`, a run | the close |
| ergonomist | `swap-ptr` over `examples/sqlite`: 0 refused today, all refused under the form with both handles declared; **some leak back if the § 13 fence is left as it is** | `heroes mutate` | steps 3 and 4 |
| historian | every defect filed against 030's class through the next three milestones is a 029 or a 031 in disguise | re-reading `DEFECTS.md` | each close |
| historian | the 2 curl sites compile after renaming to the header type — **scorable only under the verbatim-tag spelling**, unscorable under `struct <tag>` | `heroes build` | step 4 |

## Scored at step 3, 2026-09-13, and the prediction is corrected underneath

The sitting ordered the `swap-ptr` operator to land before the form. It landed
the same evening, and the number it returns is not the one the sitting expected.

**The engineer predicted 0 of ~14 killed today.** Measured over 120 programs:
**15 mutants, 8 killed, 53%** (`docs/measurements/029`). The mechanism the
prediction named is right and its number is wrong, for a reason worth more than
the number: **not one of the eight dies because the compiler can tell one handle
from another.** They die on `error[unused_binding]` — `db: db` → `db: statement`
leaves the parameter `db` unread — or on `error[aliased_mutable_arguments]` —
`statement: @statement` → `@tail` beside `tail: @tail` puts two `@` on one place.
Both were run. Both fire on the **shape of the edit**, never on the identity of
the value. **Where the swap is a pure use, which is defect 029's own shape, the
program compiles every time: 0 of 7 killed, and one of the seven is the filed
defect verbatim.**

**The denominator nobody had: 15.** The FFI seat said 5, the ergonomist 6, the
census 7, the engineer ~14 — and the FFI seat was counting *sites* where the
engineer was counting *mutants*, with neither naming its unit, which is CL-017's
shape and panel 144's own finding one sitting earlier. Twelve come from
`examples/sqlite/main.hero`, whose `first_int` holds **three** handles at once;
three from `examples/ledger`; **zero from `examples/curl`**, so the historian's
curl prediction has no site to be scored at.

**So the sitting's own headline number must be read in halves**, which is panel
011's rule arriving in a new place: when the form lands, the total will read 15
of 15 and the honest half is **7 of 7**. This paragraph exists so that nobody
reads the eight accidents as a defence that was already there.

## What seats could not run

- **Every seat's cost is unrun as a prototype**: the 60-90 lines are the
  engineer's grounded estimate from the files it read, not a compiled diff, and
  its spec delta was a guess (+30-40) against the warden's measured +62. Step 4 is
  the measurement.
- **The historian ran no compiler** and says so; every repository number in its
  report is inherited. **It also read outside its brief** — design.md §4.10 and
  § 3's type table, where it was permitted § 13 and §4.19 only — which the critic
  caught; those two citations are treated as uncitable here.
- **The warden's `real` deltas are estimated** at 1.33; the binding number needs
  `--refresh`, which the author authorises.
- **The FFI seat's *8-byte object pointers in one register class, measured* has
  no artefact** under `exp/`; it is recorded as **unrun**. Its *17 of 18 ledger
  sites unswappable* is read, not run; the operator settles it.
- **Windows is unrun** for every measurement above; Darwin and Linux were both run.
- **The ergonomist compiled nothing**, by its rules, and reported **four files
  loaded into its context unasked** — `CLAUDE.md`, two rules files and
  `MEMORY.md` — which it refused as evidence. Third sitting in a row.
- **The critic's own count of C-side handles, 17, did not reproduce**: the
  coordinator's grep with the critic's pattern over `runtime/parts/*.c` reads
  **5**. The point stands and the number is the coordinator's.
