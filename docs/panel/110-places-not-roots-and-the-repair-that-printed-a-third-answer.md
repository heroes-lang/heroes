# Panel 110 — places, not roots, and the repair that printed a third answer

**Sat 2026-09-04. Landed 2026-09-05** (the build crossed midnight; every
measurement on this page was taken on the 4th, in the session that wrote it).
**Full panel, five seats.** Convened on four spec silences and one diagnostic
tag; it ended up ruling on six questions, two of which the sitting itself
found.

Every seat worked from a frozen snapshot (43 MB, `seed/ runtime/ selfhost/
spec/ tests/ examples/ docs/ design.md CLAUDE.md DESIGN-LOG.md`, no `build/`,
no `.git`) so the repository could be edited while they measured — CLAUDE.md
§15's *"a frozen snapshot every seat copies from"*, after panels 087 and 088.
The seed built there in **2.66 s**. One defect in the snapshot is recorded
here because a seat lost time to it: `vendor/tokenizers/` was left out, so
`heroes measure` exited 2 until the spec-warden copied it in.

## The proposal, verbatim

> Spec diff, measured 3750 → 3825 tokens (+75), headroom 346 → 271:
>
> ```
>   Types table, after the `T?` row:
> + | `()` | nothing: what a function returns when it returns no value; `ok()` is the `()?` |
>
>   ## Failure: `T?`, after "No exceptions exist.":
> + A `T?` is never fallible twice: `T??` is a compile error, `[T?]?` is not.
>
>   ## Top-level declarations, extending "Constants use SCREAMING_CASE":
> + , and a `constant`'s body computes over literals and other constants: it calls nothing.
> ```
>
> - **Q4** — a parameter list may NOT break across lines, and `heroes fmt`
>   keeps a long signature on one line. Proposal: refuse the form on
>   Principle 0, no spec token.
> - **Q5** — panel 010's refusal of two `@` arguments sharing a root ships a
>   repair that `design.md:1288` calls **`certain`**. Proposal: it is a
>   **`guess`**, and design.md is corrected.

Two more questions were added while the seats were sitting, both from
measurements taken during the sitting and sent to the seats that could rule on
them:

- **Q6** — `T??` is refused when WRITTEN and the compiler CONSTRUCTS it.
- **Q7** — `function f(_: ())` reaches clang as `int64_t h_f(void h0_)`, exit
  2 (defect 012, found by the ffi-pragmatist while measuring the `()` row).

## Verdicts

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **llm-ergonomist** (spec only, blind A/B) | accept-with-condition | the thesis | **7 forced guesses under today's spec against 3 under the amendment**, and it closed 5 of 7 sub-questions | under B ≥90% of samples write `-> ()?` + `return ok()`; the **silent**-error slice (a compiling program with a non-unit result) ≥20% under A and <3% under B | say whether a `()`-valued line is subject to the must-use rule — *"the new row makes the safe move (`_ =`) look unsafe"* |
| **spec-warden** | sentence 1 accept-with-condition · sentence 2 **VETO** · sentence 3 accept-with-condition at +9 | §1.2, §1.6, Principle 0 | confirmed +75 exactly, priced apart: `()` **+26**, `T??` **+29**, `constant` **+20** | at M-corpus-depth close `grep -rn 'ok(())'` ≤ 1 and `heroes measure` ≤ **3800** | the `()` row must buy the rule and not the gloss; the `constant` sentence must buy the direction the compiler cannot teach |
| **ffi-pragmatist** | sentence 1 accept-with-condition · sentence 2 accept · sentence 3 **VETO as worded** | §1.11, §4.19 | 14 `.hero` bindings written and compiled; its wording measures **3820, four tokens cheaper** than the tabled one | `extern "math.h" link "m"` with `constant INFINITY: f64` builds with no `.c` of this project's own — true today; and `SQLITE_TRANSIENT` needs no shim | the `constant` sentence's subject must be the **written body** |
| **compiler-engineer** | Q5 approve-conclusion/object-reasoning · Q2 approve-site/**object-shape** · Q3 object both · Q4 object · Q6 **VETO on flattening** | §1.7, §1.1, §1.12 | root comparison ~40 lines, path comparison ~75; `diag.hero` unchanged | no diagnostic ships two `certain` fixes by M-isolated-threads close; the `find`-over-`[i64?]` program still prints 1 at exit 0 unless Q6 gets its own sitting | sentence 2 scoped to the written form and Q6 docketed; the over-rejection widened or the comparison changed |
| **historian** (advisory) | object on item 3's **stated reason** and on item 4 | precedent | — | at M-check-completeness the first generic instantiated at `()` needs a value expression or a refusal; at M-guide-book a 72-80 column page cannot set a 120+ column signature | a shipped language where a parameter list cannot break, deliberately — *"the one thing on this page I most want to be wrong about"* |

## What the seats found that the brief did not have

**The `()` row's real warrant is not the row.** The brief said a reader meets
`()` three times; measured, the unit **type** appears in the spec **once**, at
`spec:191`'s `write_file(path: str, text: str) -> ()?` — the other two are
empty *parameter* lists. But the hole is wider than a missing row: `ok(())`
is refused (`()` is not an expression) and **`return ok()`, argument-less, is
the only way to build a `()?`**, while `spec:146` shows only `ok(v)`. So a
reader with the document alone cannot write a function of the exact shape the
document displays. The spec-warden measured the need — **`ok()` 19 times in
`selfhost/`, 62 across 587 `.hero` files** — and the blind seat measured the
cost: its most dangerous result was routing around the undefined type by
returning `str?`, *"which compiles and is a different API from the one
requested"*.

**The `constant` sentence's veto was earned by reading a real header.** With
`clang -E -dM` on this machine: `HUGE_VAL` is `__builtin_huge_val()` and `NAN`
is `(__builtin_nanf(""))`. Both bind as `constant …: f64` and print
`inf`/`nan`. So a Heroes `constant` whose value is a call already exists and
works, at §4.19's ladder rung 2, and *"a `constant`'s body calls nothing"*
would have read as illegalising it — provoking a 9-line C shim in two files,
which the seat wrote and compiled to price it. `spec:239` already says a
group's `constant` has **no body**, so the sentence presupposed what the
document denies one section below.

**And the compiler-engineer proved *"it calls nothing"* false in the costly
direction anyway.** `constant_body` refuses exactly four expression kinds, and
`[1, 2, 3]`, `{"a": 1}`, `XS[0]`, `if`/`else` and unary operators all run —
while `Point(x: 0, y: 0)` is refused **as a call**. A reader who accepts
`[1,2,3]` and reads *"it calls nothing"* writes `constant ORIGIN: Point` and
pays a round trip.

**Q5: the argument the convener did not make.** Both of the brief's premises
were wrong, and the seat said so: `apply` (`selfhost/cli/check.hero:200-259`)
collects every `certain` fix, sorts by span and applies them **in reverse**, so
a multi-span edit is already expressible as N `Fix` values; and the resolver's
lack of types does not block a repair keyed on a syntactic root. Then it built
the repair and ran it:

```
n: i64 @ 0
t: i64 @ n
shift(a: @n, b: @t)      # prints 1
```

The unrepaired program prints **10**. A reference reading gives **11**. The
`certain` repair prints **1** — a third answer, at exit 0, compiling clean.
CLAUDE.md §9's harness asks an applied `certain` fix to *compile*, not to mean
the same thing, so `--apply` would have turned a 10-program into a 1-program
with CI green. And the tree's own doctrine settles the tag: `d.fixes` is a list
of **alternatives** (`number.hero:105-108` ships two `guess` spellings of one
literal; `value_errors.hero:130` vs `:140-143` ships one `certain` when one
repair exists and two `guess` when two do), so two readings make two guesses.

**Q2: panel 010 R6's assignment does not survive contact with the code.** R6
sent the work to *"the routine that already builds the label→argument map for
the same-typed-argument rule"*. **There is no such map** — `user_call` builds
`ambiguous: [i64]`, a list of positions whose lowered types coincide, and
fetches each label on the fly. `resolve/walk.hero`'s `arguments` is the single
funnel for all four call shapes, has `arg.mutable` syntactically and
`resolved.Ref.local(at:)` for the root, and `check/walk.hero` is **2071 lines**
against `resolve/writes.hero`'s 99.

**Q2's shape is where this sitting changed its own mind, and it is the
resolution the title names.** design.md:1298 said *"comparing roots is a
complete alias test"*. The seat objected that a root test refuses two programs
design.md never authorised refusing, and both work today:

```
shift(a: @p.x, b: @p.y)        # distinct fields, one root
shift(a: @xs[0], b: @xs[1])    # distinct literal indices, one root
```

design.md:1299 authorises over-rejecting `f(a @ xs[i], b @ xs[j])` with
**variable** indices, and says nothing about fields. The convener's counter was
a memory-safety worry the seat had not weighed: two `@` pointers into one
COW buffer, where the second unshare could move it. **That was measured rather
than argued** — both programs, with the record and the array *shared*, print
`1 10` and are clean under `--sanitize`, because the first unshare takes the
refcount to 1 and the second moves nothing. So §12's robustness rule does not
protect the root test, and §12's *"a refusal is held to a feature's standard"*
condemns it. **Places, compared step by step.**

**Q6 is worse than the brief said and it belongs in its own sitting.** The
brief claimed no signature can name the doubly-fallible value; a **generic**
one can — `function wrap<T>(x: T) -> T?` over an `i64?` gives `i64??`, and
`wrap(wrap(a))` gives `i64???`, unbounded. The producer is not a contrivance:
the §1.11 built-in `find<A>(xs: [A], f) -> A?` over `[i64?]` yields `i64??`,
runs, and prints 1. The compiler also **prints a type it refuses to parse** —
`error[type_mismatch]: expected i64?, found i64??` — which is §4.17's own
failure. Four repairs were priced. **Flattening `m[k]` is VETOED on
soundness**: it collapses *key absent* and *key present, value failed*, so a
stored `fail("parse", …)` becomes indistinguishable from a missing key and the
program takes the wrong branch at exit 0. Refusing `{K: V?}` closes one of
three doors. Making written `T??` legal is the only option that closes all
three and the only one §1.7 favours, because it **deletes** a special case —
but it removes a catch the thesis may want, which is the ergonomist's and the
warden's call, not the engineer's. Docketed with what its sitting must measure
first: `--dump-ir` on `find` over `[i64?]`, whether `?` peels the outer level
in the emitted C, `hero_runtime_check_leaks()` on a nested fallible carrying a
`str`, and the full list of library functions that bind a parameter from an
argument and return it fallible (one of six measured: `find`).

**Q4's premise was false, and three independent measurements say so.** All
three multi-line signature shapes — parenthesis-then-indent, hanging
continuation, and four-space continuation — `check` and `run` at **exit 0**,
and `heroes fmt` joins them onto one line. `cursor.skip_terminators`, called
four times in `parse/members.hero`, is what admits the break. So *"refuse the
form"* was not declining a feature, it was proposing a new refusal of a working
one. Breakage if it shipped: **zero**, because `fmt` has normalised every
signature in the tree already. The historian found the refusal
**unprecedented** — Python exempts brackets entirely, **Nim's rule is the one
CLAUDE.md §6 says to copy the surface of**, Go permits it and requires the
trailing comma its semicolon insertion implies, Elm was asked for Heroes' rule
in 2015 (elm/compiler#978) and kept mandatory commas — and reported the search
as a *failed search* rather than an impossibility. The blind seat scored that
task a **null result**: 0 of 1 answerable under either variant. Signatures past
the formatter's 120 columns: **167 of 1444 (11.6%) in the compiler, 3 of 850
(0.4%) in `examples/`, 2 of 282 (0.7%) in the harness** — a factor of 29, so
the pressure is the compiler's alone, and the compiler is written.

## Disagreements, unsmoothed

- **The `constant` sentence's price.** The warden wanted it at **+9**
  (permissive half only) and said it would *object at +20* because *"the extra
  11 tokens restate the diagnostic's own first line"*. The engineer wanted the
  refusing half **corrected** rather than dropped (*"a record construction
  counts as a call"*, ~+26). The resolution takes the warden's price and the
  engineer's escape at once: the sentence states **only** the permissive half,
  so there is no false claim to correct, and the excellent
  `error[constant_body]` teaches the refusing half. **+13** measured.
- **The `()` row's width.** The warden measured its own widened row at the same
  price as the drafted one and told the sitting to *buy the rule and not the
  gloss*. The engineer refused to have a behavioural rule inside a Types-table
  row at all. The resolution splits them: the row is **76 characters**, in line
  with the table's other nine, and the must-use answer goes to `spec:100` where
  the must-use rule already lives.
- **The removal.** The ergonomist volunteered ~38 tokens of FFI package
  illustration. The warden measured it at **−49** and **declined both halves**,
  on a ground worth keeping: `error[ffi_missing_link]`'s note teaches `link` and
  **never says `package`**, so the spec's illustration is the only place in the
  system that tells a reader `package` exists — panel 089's condition inverted.
  It offered **−19** instead (`spec:176`'s quadratic-concatenation cost model,
  on the ground that §13 makes performance a non-goal) and named the honest
  counter itself. **The counter wins and the removal is declined**: §15's
  *never slow the compiler down* is a standing author instruction in capitals,
  the emitter is string-heavy, a concat loop draws **no diagnostic**, and the
  spec is that fact's only carrier. Buying 19 tokens of slack we do not need by
  deleting the one written warning is not a payment.
- **The historian against the `constant` rule's stated reason**, and it is
  right: *"a `constant` re-evaluated on every read"* is an argument for
  diagnosing an initialiser that is not a constant expression, not for
  forbidding calls. `len(args())` should be refused because `args()` reads the
  process. Forbidding calls is *sufficient*, not *necessary*, and every sourced
  neighbour has been relaxing (C++11 → C++20, Ada 2022, Rust `const fn`). What
  saves the rule is its own citation: **N3018, 2022-07-06**, where WG14 kept
  C23's `constexpr` to objects only *"to avoid undue burden on lightweight
  implementations"* — and Heroes emits C. So the restriction stands as an
  **implementation limit rather than a principle**, recorded here because
  `resolve/cycles.hero` is at its pinned ceiling with zero room and a third
  note on an already-good diagnostic is not where this belongs.
- **Two brief premises were false and both were caught by the seats**: Kotlin's
  `T??` is not refused, it *parses and collapses* (`T?? ≡ T?`); and the
  historian's own first fetch of the Go spec was wrong about the trailing
  comma, which it falsified itself and reported.

## The resolution

1. **The `()` row lands**, at 76 characters: *nothing: what a function with no
   `->` returns; `ok()` is the `()?`*. The *with no `->`* is not decoration —
   `heroes fmt` **deletes** a bare `-> ()` (`print/fmt.hero:521-523`), and
   under §4.15 a spec that teaches a non-canonical spelling is a contradiction.
   `spec:100` gains *which a `()` line refuses: it stands alone*.
2. **The `T??` sentence does not land.** Two vetoes, and the tabled wording was
   false. `tests/golden/check/a-fallible-type-is-never-written-fallible-twice.hero`
   is the whole record of the rule until Q6's sitting rules — the golden §9 owed
   and did not have.
3. **The `constant` sentence lands at +13**, permissive half only, subject the
   *written body*.
4. **Q4: no change and no spec token**, and the resolution is **not** *refuse*:
   the form is **accepted input with non-canonical output**, which is what the
   compiler does today. The trailing-comma steering the historian recommends
   (`zig fmt` and Black's magic comma converged on it independently) is a
   feature under Principle 0 with nothing blocked and 0.4% of real programs
   affected, and is docketed at M-guide-book with that seat's page-width
   prediction.
5. **Q5: `guess`.** design.md:1288 corrected, with the third answer as the
   recorded reason.
6. **Q6: its own sitting**, flattening vetoed, with the four measurements its
   brief owes. Noted for that sitting: `nested_fallible` is **absent** from
   `is_thesis_rule` (`selfhost/diag.hero:88-103`), so `--permissive` does not
   drop it.
7. **Q7: repaired in this commit**, `unsupported[unit_parameter]` beside
   `unit_field` and `unit_element`.

Spec measured **3750 → 3799, +49**, headroom **297** — one token inside the
warden's own registered bound, and 26 tokens below the tabled proposal that
carried two vetoed sentences.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| llm-ergonomist | ≥90% of samples given the amended spec write `-> ()?` with `return ok()`; the silent-error slice (a compiling program with a non-unit result) <3% against ≥20% on the old spec | M-thesis-harness |
| llm-ergonomist | the line-continuation task shows **no measurable difference** between the two specs; if the amendment scores better there, its reading of the diff was wrong | M-thesis-harness |
| spec-warden | `grep -rn 'ok(())' selfhost/ examples/ tests/` reads ≤ 1 (today **1**) | M-check-completeness close · see the note below |
| spec-warden | `heroes measure spec/heroes-spec.md` reads ≤ 3800 (this commit lands **3799**) | M-check-completeness close · see the note below |
| spec-warden | `grep -rn -- '?\]?' selfhost/ examples/ tests/` still reads **0**; above 0 falsifies its veto on sentence 2 | M-check-completeness close · see the note below |
| ffi-pragmatist | `extern "math.h" link "m"` with `constant INFINITY: f64` and `constant NAN: f64` builds and prints `inf`/`nan` with no `.c` file of this project's own; and `constant SQLITE_TRANSIENT: ptr` builds and runs | M-ffi-ladder libm rung |
| compiler-engineer | no diagnostic in the tree ships two `certain` fixes (23 `certain` today, 21 `guess`) | M-isolated-threads close |
| compiler-engineer | `heroes build` on the eight-line `find`-over-`[i64?]` program still prints 1 at exit 0, and `grep -c nested_fallible selfhost/` stays at 2, unless Q6 sits | M-isolated-threads close |
| compiler-engineer | `header_line`'s premise that *the line IS the signature* (`check/walk.hero:1413-1423`, repeated at `print/dump.hero:126`) stays true and untested | M-robustness-guards close |
| historian | the first generic instantiated at `()` needs either a value expression for `()` or a rule refusing `()` as a type argument — `map`/`fold` with a `()`-returning callback | M-check-completeness close |
| historian | the 11.6% figure **rises** rather than falls, because every new generic and every `@` parameter lengthens a signature and nothing shortens one | M-guide-book |

**The spec-warden's three predictions were written against *M-corpus-depth
close*, and that milestone closed on 2026-09-04 — the day of the sitting.** A
prediction cannot be scored at a close that has already happened, so the seat's
own words are kept above and the scoring milestone is re-homed to
**M-check-completeness** (row 42), which is open. The seat is not at fault: it
was reading a ROADMAP whose *Last closed* row moved while the sitting sat, which
is the hazard CLAUDE.md §15 names — *re-read the chain and the log immediately
before writing a scheduling fact*, and this sitting paid it at one remove.

**And M-check-completeness is where Q6 goes, because Q6 is the warrant that row
did not have.** `docs/ROADMAP.md:363` records it as *"the only one of the four
with **no warrant**"*. It now has one, measured: `heroes check` says 0 on
`m: {str: i64?}` and on `find` over `[i64?]`, both of which produce a value no
signature in the language can name, and it said 0 on `function f(_: ())` while
`heroes build` said 2. That is the row's own sentence — *what `heroes check`
accepts, `heroes build` compiles* — failing in both directions on the same day.

## Author's verdict

**Ratified 2026-09-05 by standing delegation**, and the delegation is quoted
because a quotation binds where a summary does not. The author's instruction
opening this session, in English as §11 now requires: *"Carry on until defects
and decide are at zero; you suggest the answers yourself."* That is the same
delegation §15 already records — *"carry on without asking me"*, and *"choose
the most robust and complete solutions over the cheaper ones"* — so the
resolution above is adopted in full rather than provisionally, and this sitting
opens no item in `docs/work/DECIDE.md`.

Two things the author may want to overturn, named here so that overturning them
costs one reading rather than an audit. **The `constant` sentence buys only the
permissive half**: the conservative resolution was the warden's +9 with no
change at all to the refusing side, and the robust one the engineer's +26 that
states the record-construction trap; what landed is neither, and the argument
is that a sentence which says nothing false needs no correction. **And the −19
removal was declined**, which leaves the spec 19 tokens heavier than the
warden's own package — the reason is §15's standing instruction never to slow
the compiler, against a spec sentence that is the only written carrier of the
quadratic-concatenation fact.
