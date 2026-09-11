# 128 — the type that names nothing

Convened and closed 2026-09-11 · M-positional-values · **full lane, five seats**
· status: **ratified as adopted, 2026-09-11**
(`docs/work/DONE.md`)

Defect 026, filed at M-labelled-builtins' close, and the author asked for the
sitting in five words.

**Three of the four routes were vetoed, each by a different seat and each on
soundness rather than on price.** That is the shape of this sitting and the
reason its resolution is smaller than its subject.

## The defect

`spec § 9` says that when two parameters of a signature share a type the
arguments are named at the call site. Panel 127 made the rule reach the two
built-ins that escaped it. It cannot reach a call made through a **function
type**, because a function type names no parameters at any of its sites:
`selfhost/check/walk.hero:1541` reads the label to expect from a
**declaration**'s parameter span, and `(function(A, B) -> C)` has none.

Measured before the sitting, and every witness re-run in the landing session:

| what | the answer |
|---|---|
| `f("msg_first", "code_second")` through `(function(str, str) -> str)` | printed the swap, exit 0 |
| `f(nonsense: "a", rubbish: "b")` through the same | printed `a/b`, exit 0 — an invented label accepted and ignored |
| a C callback's parameter names swapped, body untouched | printed `-7` where the names promise `7`, no diagnostic |
| function types in the tree with two parameters of one type, as written | 5, all in `tests/golden/` |

## The seats, and what each measured that nobody else had

**llm-ergonomist — object, and it moved the sitting first.** It wrote the
comparator task under both texts and found that **names on peers buy a label,
not a check**: `better(a: best, b: x)` compiles exactly as `better(best, x)`
does, and prints the smallest where it promised the largest. The label rule
catches a *reordering*; it cannot catch a reader who does not know which operand
is the candidate. It preferred the positional row and its own hesitation list
names the real gap: *neither text says which operand of a comparison is the
candidate*.

**spec-warden — veto on the positional row as balloted.** Not on price: the
sentence was **false**, because invented labels were accepted, so adopting it
would have made a compiler defect wearing documentation's price tag
(CLAUDE.md §12). It also measured the pair that is the whole sitting: the same
mechanical inversion is killed **100%** through a declaration and **0%** through
a value, and the compiler's own note steers readers to the 0% side. And it
answered its own question: **the document never shows a function-typed parameter
being called**, anywhere, so all four readings of the rule fit today's text and
the compiler implements the only one that cannot fail loudly.

**ffi-pragmatist — veto on refusing the type, and three measurements on
memory.** SQLite's collation callback with its names swapped: exit 0 both ways,
and under the sanitizer a heap-buffer-overflow. zlib's `free_func` swapped: exit
133, an invalid free. The same swap in the shape a real user picks: **exit 0 in
silence**, and a heap-use-after-free. So at the boundary the swap reaches memory
rather than an answer. Refusing the type would foreclose §4.19's ladder at three
steps, `curl_write_callback`'s two `size_t` among them, and would delete a
comparator that compiles today. It also measured that **C cannot help**:
`_Generic` says the two parameter orders are the same C type, so the checker
would be the whole defence.

**compiler-engineer — veto on names in the type.** It is core by Part 5's
mechanical test: parsed, resolved, interned, substituted by the monomorphiser,
re-printed by the formatter, typedef'd by the emitter — **eleven modules**, two
of which sit at their decided ceiling with zero headroom. It would also **invert
a shipped `certain` fix**, `parse/type.hero:221`'s *a function type lists types,
not names*. And it gave the sitting's deepest finding, below.

**historian — approve names in the Ada shape, advisory.** Verified: Swift had
identity-significant labels in function types and **removed them** in Swift 3
(SE-0111) for simplification, with the cost recorded within seven months and a
door left open that nobody has walked through in ten years. Ada has done exactly
what route A proposes since 1995: an access-to-subprogram type carries parameter
names, a call through the value may use them, and assignment requires only
subtype conformance, which **excludes** the names. TypeScript and Go allow names
in the type and exclude them from identity. **Refusing a same-typed function
type has no precedent in anything it searched.** A swapped comparator in a
shipped system: a null answer, said plainly, with the precision that a swapped
comparator misorders rather than corrupts — the corruption path is same-typed
pairs with asymmetric roles, destination and source.

## The finding that no route reaches

**The defect does not ship where the sitting thought.** It ships inside `fold`.
A program hands `fold` a function, and the library calls it positionally with no
name anywhere in the chain: the compiler seat ran it and `fold` at `A := B` with
the callback's roles inverted prints `cba` instead of `abc`, exit 0.
`selfhost/library_source.hero:76-83` says in writing that this exact failure is
what its body was written to prevent, and it is prevented for the library's own
call and not for the callback the program supplies. `fold`'s type is
`(function(B, A) -> B)`, two distinct letters, so **no route in this sitting
reaches it**: refusing same-typed types does not fire, and mandatory names on a
same-typed pair do not either.

And the negative answer underneath it, run: **the language has no way to write a
generic comparison the label rule can see.** A concrete record carrier scores
100% on the instrument; a generic one cannot exist, because `record Pair<A>` is
refused — design.md §4.12 is *functions only*. Which means
`value_errors.hero:94` was never complicit: it is **obedient** to §4.12's own
escape hatch, *if an operation on `T` is needed, pass it as a parameter*.

## Resolution adopted, provisional

Three routes are vetoed and a veto is a refusal rather than a price
(CLAUDE.md §4), so what lands is the pair that is measured and built:

1. **A label at a call through a function value is refused**, the route nobody
   had listed, built by the compiler seat and landed from its own copy: 25 diff
   lines, `error[label_on_function_value]` with a note that says why and a
   `certain` fix that removes the label and leaves the value where it is.
2. **The compiler's own note stops prescribing the hole in silence**:
   `value_errors.hero:94` now says to call the comparison **by position** and
   names the two roles, `less(candidate, best)`, instead of `a, b`.
3. **The document says it**, and this is where a veto lifted on a measurement.
   § 3's function-type row gains *a call through one is positional, since the
   type names no parameters, and a label there is an error*. The warden had
   vetoed that sentence as false and stated its own lift condition, a running
   program showing the sentence is the whole truth; point 1 made that program an
   error, so the sentence became true before it was written. **+25 real tokens,
   paid by nothing, and ledger row 65 says so rather than pretending.**

**What this closes and what it does not.** It closes the invented label: a label
at such a call was not even a claim and is now an error. It closes the silence:
a reader is told the call is positional. **It does not close the inversion**, and
the instrument says so in one number: `heroes mutate --operator swap-args` reads
**50%** on this shape before and after, measured on the corpus the compiler seat
wrote for it.

**What conservative would have been**, recorded so the author can choose it:
names in the type, mandatory where two parameters share a type, part of the
type's identity — the ffi seat's condition and the historian's Ada precedent,
which the compiler seat vetoed on cost and whose veto lifts on a measurement it
named: build it, show the two ceilings raised and `swap-args` at 100% under about
150 code lines with the emission, descriptor and determinism suites green.

## Predictions to score

| seat | prediction | state |
|---|---|---|
| compiler-engineer | if the label refusal lands alone, `swap-args` still reads 50% and `check/walk.hero` measures 1751 | **HELD, both halves**: the suite named 1751 and the metric is unmoved |
| compiler-engineer | if names in the type ever land, the net names both `check/walk.hero` and `ast.hero`, two or more ceilings rise, and walk exceeds 1780 | open, scored at that landing |
| ffi-pragmatist | `atexit` and `sqlite3_busy_handler` need no new spelling, their text unchanged | **HELD** by construction here: neither has a same-typed pair and neither route touched them |
| ffi-pragmatist | under names-in-the-type, `curl_write_callback` would be declared with `size` and `nitems` and the checker would accept them transposed, because nothing reads those names out of `curl.h` | open, and it is the sharpest argument against the conservative route |
| historian | with names outside identity exactly the same-typed sites need editing and no new mismatch diagnostic appears | open |
| historian | searching swift-evolution in a year still finds no proposal reintroducing labels into function types | open |
| llm-ergonomist | its four programs compile or fail exactly on the quoted lines | **HELD**, measured: the two named-type programs are refused `named_parameter_in_function_type`, the two positional ones run |

## Author's verdict

**Ratified as adopted, 2026-09-11** (the author's word, `ratifico i panel`, given
the same day the sitting closed). The small half stands: the label at a call
through a function value is an error, the note says the call is positional and
names the roles, and § 3's row says it. **The conservative route is not built
now**; it stays scheduled at M-check-completeness with the measurement that lifts
its own veto, and defect 026 stays open and narrowed to the inversion.

**So the three vetoes stand as refusals rather than as prices**, which is what
they were entered as, and the record keeps all three arguments unsmoothed: the
warden's sentence was false until a program made it true, refusing the type would
delete a program that works, and names inside the type are a rebuilding of eleven
modules rather than a repair. The one number the ratification does not change:
the inversion is still measured at 50%.

## What this sitting did not do

It did not close defect 026, which stays open and is narrowed to the inversion
it still admits. It did not reach `fold`, where the defect actually ships, and it
says so rather than letting five golden sites imply coverage. It did not measure
a reader: the ergonomist's rates need Part 11's metric 2. And it did not build
the conservative route, whose cost is priced at eleven modules and whose veto
lifts on a measurement nobody has taken.
