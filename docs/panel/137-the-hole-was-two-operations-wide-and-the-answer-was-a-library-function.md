# Panel 137 — traits: the hole was two operations wide, the answer was a library function, and the promise about copies was already false

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 3 · **status**
`provisional — author ratification pending`

**Lane: full five seats**, on **two items at once** — Part 7 item 8 (traits) and
Part 8 wart 11 (*"No user-extensible iteration, since there are no traits"*),
which is the same question from the other end. Ruling one and not the other would
leave the second standing as a promise the first had already broken. The gate
CLAUDE.md § 4 asks once per milestone was given at step 1; this sitting convenes
without asking again, as § 4 directs.

## The two items, as design.md carries them

`:2865-2867`: *"Traits / interfaces — genuinely useful, but instance resolution
is expensive. Their absence means there is no user-extensible iteration protocol:
`for x in ...` stays a compiler special case for arrays, maps and ranges."* And
`:3091`: *"No user-extensible iteration, since there are no traits."*

## The fact that reframed the sitting before the briefs went out

**A generic sort with a comparator compiles and runs today, with no trait.** The
coordinator wrote `sort_by<A>(xs: [A], before: (function(a: A, b: A) -> bool)) ->
[A]` over a `[Point]`: it printed `1 2 3`, exit 0. Generics are unconstrained and
top-level functions are values, so **passing the operation explicitly already
expresses everything a trait would dispatch**. That makes
`selfhost/modules.hero:159-190` — twenty-two lines of hand-written stable
insertion sort over `[diag.Diagnostic]`, written because `sort` takes *"a number,
`str` or `bool`, never a type parameter"* — evidence that nobody wrote `sort_by`,
and not evidence that traits are needed.

So the brief put **two routes** to the seats, and the second is in neither the
item nor the wart:

- **Route A, nominal traits**: `trait Ordered` naming signatures, `Point is
  Ordered` supplying them, `<A is Ordered>` constraining a type parameter.
  **+106** vendored.
- **Route B, a structural iteration rule and no trait at all**: `for x in v` also
  accepts a value whose module declares `function next(@it: T) -> E?`, which
  `for` calls until it fails. **+28** vendored.

**The seats found a third, and it is not a language change at all**: add
`sort_by` to the library. `selfhost/library_source.hero:74-125` already ships six
generic higher-order functions — `map`, `filter`, `fold`, `find`, `any`, `all` —
and `sort_by` is the seventh row of an existing table.

## What the coordinator measured, 2026-09-13

| fact | number |
|---|---|
| generic functions in `selfhost/` (59,511 lines, 204 modules) | **0** |
| generic functions in `examples/` | **17** |
| `for x in` sites in `selfhost/` | **883** |
| spec today | 5655 vendored · 7531 real · 601 headroom net of the FFI floor |
| route A draft | **5761**, +106 · real unrun |
| route B draft | **5683**, +28 · real unrun |

## The three measurements that decided it

**One — the hole is two operations wide, not one feature wide** (compiler seat,
eight cases compiled). A generic needing two operations on one type parameter
works; `==` on a type parameter over a user record works; a `{A: i64}` map key
works; a recursive generic works; a cross-module generic `sort_by` works. Only
three things are refused: `<` on a type parameter, `sort([A])`, and
`print`/`to_str` on one. The reason is Part 5's descriptor pass, which already
generates `copy`, `drop`, `eq` and `hash` per reachable type — **`ord` and `show`
are simply not in that set**. So what looked like a missing language feature is a
missing pair of descriptors, and the corpus wants one of the two.

**Two — the corpus does not want it** (warden and FFI seat, independently). All
**26** `sort(` sites in `selfhost/` sort `[str]` coming from `keys(m)` and **not
one** sorts a record. `function next(` appears **zero** times in the tree. And the
brief's own premise — *"every binding in `examples/` writes a `while` loop by
hand"* — is **false**: the FFI seat enumerated every `while` in all 22 files
containing an `extern`, found about seventy, and **exactly two advance a C handle
until it stops**. Route B's whole saving across the tree is **about eight lines at
two sites**.

**Three — the item overstates its own cost by two thirds** (compiler seat). It
calls `for` a special case *"for arrays, maps and ranges"*. It is **arrays only**:
`range` is a library function returning `[i64]`, and a map is `not_iterable`.

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| `compiler-engineer` | **VETO on route A**, object on route B; ranks the library route first | §1.7, §1.1, Part 5; for route B **§4.8 `:1306`** | eight cases compiled, above. Route A is **900–1400 lines**, two `DeclKind` arms against **219 enumeration sites in 53 modules** — panel 119's closure shape at 219/53 — and **coherence has no home**: the name table is keyed `{module: {name: decl}}` precisely because two modules may each declare `Point`, and `mono.hero`'s `Instance` has no witness field. Route B is ~120–180 lines but **overrules design.md §4.8 by accident**: *"UFCS does not apply when the first parameter is `@`"*, and `for v in c` calls `next(@c)` with no `@` written anywhere — the compiler already refuses the neighbouring shape by name. Run anyway, hand-desugared: it terminates and sees its own writes, but **the author's own value stays at 0**, so the loop consumes a copy the reader cannot see. **Subtraction: both routes zero.** The four hand-written sorts, counted precisely rather than by the coordinator's loose grep, are 24+20+22+19 lines, and one is a plain oversight because `sort([3,1,2])` compiles today | adding `sort_by<A>(xs: [A], lt: (function(a: A, b: A) -> bool)) -> [A]` to the library costs **≤ 20 added lines** and **deletes ≥ 60** across the four sorts, with **no change to any compiler module** and three suites green. If any compiler module must change, route A's cost case reopens and this veto is wrong | the veto drops on a compiled closure-list program a passed function cannot express — heterogeneous dispatch over a collection whose element types are unknown where it is built, shown not to be carriable by a variant |
| `llm-ergonomist` | **refuse route A as written**; **VETO route B as written**, liftable by three sentences | § 4, § 8, § 9 | counted by hand from the three blind texts. **The shortest correct program is identical under all three** — 132 characters — and route A **lengthens** the sorting task rather than shortening it, because it leaves § 11's *"never a type parameter"* and § 7's *"a number only"* untouched, so the built-in `sort` still refuses an ordered type and `a < b` on a type parameter is still an error: **A buys the constraint, not the sort**. Route B saves 64 characters, 17%, on one shape and nothing else. **Five silences in A**, including a grammar-name collision (`Param` defined twice, in § 4 and § 9) and an `ident "is" ident` production with no qualification, which **silently forbids the cross-module case** a real program needs. Closing them costs ~388 characters | four, all scorable at a harness run: under B, asking for the stdout of two successive loops over one value gives a **non-degenerate split, neither reading below 20%**; under A, asking ten models to write `trait Ordered` gives **at least three distinct member names and none above 50%**, because A names none; **at least 60%** of first attempts at one sort for two types call the built-in `sort` on a type parameter or write `a < b` on one; and **no first-try improvement from A** on any task without two orderable user types | B's veto lifts on **248 characters, three sentences in § 8**: that the loop advances its own copy so the value it was given is unchanged; that a value which is none of these is a compile error naming the `next` the module would need; and that **the loop ends when `next` fails with code `done` and any other error aborts** — because as written **the loop drops a `T?` on every exit**, against § 5's *"Answer the error"* in the plainest words the document has |
| `ffi-pragmatist` | **defer both**; ranks **B over A decisively**; no veto, since neither route touches the C ABI | §1.11, §4.19; §1.12 and CLAUDE.md §12; spec § 3, § 13 | the census above, and **route B compiled against a real in-memory database**: a `record Statement` holding a `ptr`, a `next` calling `sqlite3_step`, and the `while` loop `for r in st` would desugar to — `sum of n over 4 rows: 100`. The emitted C shows `@it` as a plain pointer to a one-field struct with **no boxing and no glue**, and the `importc` verification survives untouched. **Two of the brief's three named iterators do not fit**: `curl_multi_perform`'s termination is an out-parameter with no element, so `next(@it) -> E?` has neither an `E` nor a failure — *"it is a pump, not an iterator"* — and `getchar` has no handle at all, needing an invented record and an invented field. **Finalisation is not reached by any vocabulary the language has**: `owned` covers a `cstr` result or a `char **` out-parameter, `lease` covers copied `str` bytes, and neither reaches a handle — measured with `break`, SQLite itself answered `statements still open: 1` and `sqlite3_close says BUSY: true` | `examples/ledger/main.hero`'s `queried_balances` is the **only** site in the tree where a `for` over a handle replaces a hand-written loop; a milestone implementing route B finds **≤ 2**, and the second needs an invented record. And SQLite needs no shim under either rule, which the compiled case already shows | route B moves to adopt-with-condition when **spec § 3 says what is true** about a copied address, and when the iteration rule **names what it does not cover**. Route A moves to object unless a seat produces a C binding a trait expresses and a module-level `next` cannot |
| `spec-warden` | **object — defer both**, provisional (every draft's real row is unrun); no veto. Ranks **A above B on soundness, B above A on cost**: *"if the sitting wants a refusal, refuse B — its defect is §1.3, not a price"* | §1.0 `:120-126`, `:136-147`; §1.2 `:190-204`; §1.3 `:211-217`; §1.6 `:283`, `:305-318` | **both coordinator figures are undercounts, because neither draft answers its own silences.** A drafted +106, **A closed +157**; B drafted +28, **B closed +86** — so a minimal closure puts **both** over `DELTA_GATE` 50. R1 alone measures **−55** on today's spec, not panel 135's −43, and for route A it is **repair rather than payment**: A adds `trait` and `is` to § 4's `Declaration`, which makes the kinds sentence stale, so A must spend it and still lands +102. **`is` is a legal identifier today** — `is = 3` prints 3. **Route A as drafted never touches § 8, so it does not close wart 11 at all** | at M-core-packages' close, any landing of B answering its three silences reads **≥ +80** vendored; `grep -rn 'function next('` over `selfhost/` still returns **0**; generic call sites in `selfhost/` still **< 3** | a metric 2 instrument existing and showing the trait spec beating the non-trait spec by ≥ 1 task in 20, or the closure list gaining a row that needs it |
| `historian` | **object** (advisory): refuse traits, adopt a structural rule as its own item, and **rewrite wart 11, whose causal clause is false** | Part 7 item 8, Part 8 wart 11 | **the item merges three costs that are not one.** Resolution and coherence at compile time is Rust's recursive instances and Haskell's `UndecidableInstances`; monomorphisation codegen Heroes **already pays**; dynamic dispatch it structurally cannot have. **Swift's famous compile-time disaster is not conformance checking** — it is the constraint solver over overload sets and literal protocols, 42 s for a twelve-line string concatenation, and Heroes has no overloading, no operator overloading and no implicit literal conversion. **And the causal claim is falsified six times**: C++ range-for looks for `begin`/`end` by name, Python duck-types `__iter__`, **Go shipped `range` over a plain function in 1.23 and declined the interface proposal on its own table**, Lua uses a metamethod, **Nim rewrites `for` to a named `items` call resolved by ordinary overload lookup** — which is the machinery UFCS already is — and Zig refuses both independently, which is the conflation the item makes. **No public measurement isolates trait selection as a bottleneck**, searched and not found. The strongest argument for traits, coherence, **evaporates here**: with unconstrained generics there is no site where a container calls a user operation behind the programmer's back. And the reversal precedent is C++0x concepts, **voted in 2008, voted out 2009, returning eleven years later deliberately minimal** | at the close of whichever milestone implements `for` over a user type: a structural rule needs **zero new resolution passes**, and the `for` lowering ends with **one** desugar site replacing today's special cases, countable by grep | four, any one reopening it, the last being **a decision to add constrained generics** — the day a generic function can call an operation on `T`, the coherence argument becomes live |

## Where they disagree, unsmoothed

**On route A there is no disagreement at all**: a veto from the seat whose
mandate is cost, a refusal from the seat that read only the text, a defer from
the budget, an object from precedent, and a defer-ranked-last from the boundary.
Five seats, five noes, for five different reasons.

**On route B the disagreement is real and it is about what kind of fault it
has.** The reader vetoes it on locality and says three sentences lift the veto.
The compiler seat objects on a different ground entirely — that it **overrules
§4.8 by accident**, a rule the compiler already enforces by name — and that
ground is not in the reader's list and is not lifted by any sentence. The warden
would refuse it outright under §1.3. The historian would adopt it. The FFI seat
would defer it and ranks it first of the two. **The resolution below takes the
compiler seat's objection as decisive over the reader's**, because §4.8 is an
existing written rule of the language and a new form that contradicts one is a
different act from a new form with a silence in it.

**A disagreement of method, recorded because it recurred from panel 136.** The
reader counts characters, the warden counts tokens, and neither saw the other's
number: the reader prices route A's silences at ~388 characters, the warden at
**+51 tokens**. Tokens are the unit §1.6 judges, so the warden's is operative and
the reader's is what a reader of the section pays.

## The resolution adopted, provisionally

**Three verdicts, because the sitting was asked two questions and found that the
answer to both is a third thing.**

1. **Route A, nominal traits: REFUSED, as a Part 6 row with its falsifier.** Two
   vetoes are not engaged — only the compiler seat's is — but no seat supports
   it, the subtraction test returns zero, and the strongest argument in its
   favour was measured not to apply here. The row's falsifier is the compiler
   seat's own condition: **a compiled program on the §1.0 closure list that a
   passed function cannot express** — heterogeneous dispatch over a collection
   whose element types are unknown where it is built, shown not to be carriable
   by a variant. Refusing is held to a feature's standard (CLAUDE.md § 12), and
   that is the program that would make the row wrong.
2. **Route B, a structural iteration rule: DEFERRED, with a return condition, and
   the reason is §4.8 rather than any of its silences.** *"UFCS does not apply
   when the first parameter is `@`. `l.advance()` would hide the mutation, which
   is the only thing `@` exists to make visible"* — and `for x in v` calling
   `next(@v)` writes no `@` at all. A form that contradicts an existing rule
   returns only with that rule amended or with a spelling that obeys it, and
   **the sitting deliberately does not design one**. Its return condition is the
   FFI seat's, because the corpus is what is missing: **five or more loops in the
   tree advancing a C handle until it stops**, against two today.
3. **What is adopted instead is not a language change**: `sort_by<A>(xs: [A], lt:
   (function(a: A, b: A) -> bool)) -> [A]` joins `selfhost/library_source.hero`'s
   six generic higher-order functions, and **it is scheduled at M-core-packages
   rather than landed here**, because this milestone's steps are sittings and
   nothing lands in the compiler that a sitting adopts. **Two seats named
   M-generics-library and it closed on 2026-08-11** — the coordinator caught that
   when filing, because `records/homes` fails on an open item whose milestone has
   a closed chain row, and it is corrected here rather than quietly: the library
   grows at the milestone that builds packages over it. The compiler seat's
   prediction is its acceptance test: ≤ 20 added lines there, ≥ 60 deleted across
   the four hand-written sorts, no compiler module changed, three suites green.
4. **Part 8 wart 11 is rewritten rather than dated**, because its stated cause is
   false: *"since there are no traits"* is refuted by six languages that extend
   `for` by a name and a shape. The wart itself stands — iteration is not
   extensible — and its cause is stated correctly: `for` is a compiler special
   case **for arrays alone**, and the two forms that would open it are the one
   §4.8 refuses and the one no corpus asks for.
5. **The item's own arithmetic is corrected**: `for` is a special case for arrays,
   not for *"arrays, maps and ranges"*.

**What conservative would have been, so the author can choose it**: defer route A
too, with a return condition, rather than refusing it. The sitting takes refusal
because a Part 6 row carries a falsifier a reader can check, where a fifth
deferral carries a promise — and this ledger exists to end promises.

## Found beside the sitting, and reproduced before it was written down

**Spec § 3's promise about copies is already false, through a `ptr` field, in
shipped code.** The FFI seat made two copies of a record holding a `sqlite3_stmt *`
and advanced each once:

```
a sees id 1   b sees id 2
a.handle == b.handle after the copy: true
```

`b` was copied **before** `a` advanced and `b` sees row 2. Hand-written C under
`-Wall -Wextra` gives the identical answer at zero warnings. The specification
says *"Every value behaves as an independent copy: after `b = a`, mutating `b`
never changes `a`. **No aliasing exists anywhere**"*, and
`examples/ledger/db/sqlite.hero:287` ships a function taking that wrapper **by
value** and mutating the C cursor through it. The sentence is true of
Heroes-owned memory and false of a copied address, and the document does not say
so. Filed as **defect 030**. It is independent of traits and is owed whichever
way this panel goes; the repair is a sentence in the specification, which is a
panel question because it states what a value is.

**And a leak the vocabulary cannot reach.** Breaking out of a handle-advancing
loop leaves the statement open: SQLite itself answered `statements still open: 1`
and `sqlite3_close says BUSY: true`. `owned` covers a `cstr` result or a `char **`
out-parameter, `lease` covers copied `str` bytes, and **neither reaches a
handle**. Route B would not create that hole, but it would put a `break` closer
to it. Recorded for **M-cleanup-verdict**, whose sitting is the ruling on a
scope-bound release, rather than filed as a second defect: it is a known absence
with a scheduled home, not a wrong answer.

## Process note — a seat refused to run, correctly

**The reader's seat was briefed before its files existed, and it returned `UNRUN`
rather than a verdict.** The coordinator wrote the brief naming four paths and
created them after the seat had started, so the seat opened six names, found none,
and reported the absence with the names it had searched for. It refused three
tempting repairs by name: substituting the repository's own specification,
inferring the two changes from the brief's framing, and producing a partial
verdict. Its own words: *"an unrun sitting reported as unrun is recoverable in one
message; a contaminated sitting reported as a verdict is not recoverable at all,
because nothing downstream can see the contamination."* It then wrote the check
that would catch it having cheated — compare its cited section numbers against the
three texts once they exist — and the sitting records that, because a seat that
arms the instrument against itself is the behaviour this panel is built on. The
files were written and the seat re-run with its context intact.

## Author's verdict

Pending — queued in `docs/work/DECIDE.md` as `panel 137`.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| engineer: `sort_by` costs ≤ 20 added library lines and deletes ≥ 60 across the four sorts, no compiler module changed | `git diff --stat`, the three suites | M-core-packages (the seat said M-generics-library, closed 2026-08-11) |
| FFI: at most 2 sites in `examples/` ever take a `for` over a module-declared `next`, and the second needs an invented record | `grep` after any such change | whichever milestone lands iteration |
| warden: a landing of route B answering its silences reads ≥ +80 vendored; `function next(` still 0; generic call sites still < 3 | `heroes measure`, `grep -c` | M-core-packages (the seat said M-generics-library, closed 2026-08-11) |
| ergonomist: under a route-B text, two successive loops over one value give a non-degenerate split, neither reading below 20% | metric 2's runner | M-thesis-harness |
| ergonomist: under a route-A text, ten models give ≥ 3 distinct trait member names, none above 50% | metric 2's runner | M-thesis-harness |
| historian: a structural rule needs zero new resolution passes and one desugar site | `grep` at that close | whichever milestone lands iteration |
| the return conditions' own clock: five loops advancing a C handle; a closure-list program a passed function cannot express; constrained generics entering | `grep`, the sitting that proposes either | every later sitting of this ledger |

## What the seats could not source or could not run

The compiler seat rebuilt nothing from `selfhost/` (the twenty-minute rebuild a
sitting forbids), so its route figures are counted estimates by analogy; the eight
compiled cases, the arm counts and the hand-desugared semantics are real. The
warden could not run the real tokeniser on a draft. The FFI seat's `real`
projection is an inference and it says so, having run `heroes measure` on the
checked-in spec only; its finalisation finding on the shipped tree is **read, not
run**. The reader has no shell, so every character count is hand-counted and it
asks that the ratios, not the counts, be treated as load-bearing. The historian
marks unverified: Lua 5.4's manual wording on `__ipairs`, C11's `_Generic` as a
trait substitute (**not searched**), any language publishing a measured deletion
count after adding traits (searched, not found), any statically typed language
removing shipped traits outright (searched, not found), and a designer's quotation
on Go's typeclass route, which is a secondary-source paraphrase; it ran no command
against this repository.
