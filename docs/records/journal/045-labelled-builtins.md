# 045 — M-labelled-builtins

Closed 2026-09-11, **untagged**: defect 026 was open when the work finished, and
CLAUDE.md § Verification tags a milestone only over a clean list. One sitting,
panel 127, five seats, on a defect the milestone before it had filed four hours
earlier.

## Goal

Repair defect 025. `spec § 9` has said since v0 that when two parameters of a
signature share a type the arguments are named at the call site, and
`fail("code_here", "message here")` was accepted while a user function with two
`str` parameters was refused twice. The author read the finding and answered in
one word.

The mechanism, read at the line before anything was written:
`selfhost/check/walk.hero:1541` gets the label to expect from
`fd.value.params[position].name`, a span into a **declaration**, and a
`.runtime` built-in has no declaration at all — `selfhost/inventory.hero` gave
it a name and a tier. `range` was checked because it is written in Heroes.

## What surprised

**The hole was not that the label is optional. It is that the label was
unverified.** The compiler seat ran the shapes next to the one that provoked the
defect, which is what CLAUDE.md § RUN IT asks for, and found worse:
`fail(nonsense: "a", garbage: "b")` was accepted and the labels ignored, and
`xs.slice(to: 3, from: 1)` **panicked**, because the values were used in the
order written while the labels claimed otherwise. A declared function refuses
both.

**The obvious repair would have killed nothing.** The sitting's strongest fact
was 159 swapped `fail(` calls surviving the mutation operator, and
`selfhost/mutate/edits.hero:29` says that operator moves *label and value
together*. So after a missing-label-only repair every swap is a **labelled**
call in the wrong order, and the order half of the rule is what kills it. A
route shipping only the first half would have moved a thousand call sites,
regenerated a 22 MB seed, and left the survivor count exactly where it was. The
seat measured that on a one-mutant probe: one mutant, zero killed.

**The sentence that admitted the hole cost thirty-two tokens and the repair that
closed it cost one.** The spec-warden priced both routes and then **vetoed** the
cheap one for a better reason than price: *the built-ins the compiler provides
are the one exception* is a false sentence, because `range(1, 4)` and
`write_file("out.txt", "hello")` are built-ins the rule already refuses. The
truthful wording, naming the two built-ins rather than the class, costs +32
real. Route C, which landed, is +8 for § 6's shape line and example and −7 for
§ 11's duplicated `slice(from:, to:)`, which over-pays.

**A certain fix was keeping the defect it named.** `copy_text(to: "b", from:
"a")` was refused `wrong_label` with a fix marked `certain`; the seat applied it,
ran the result, and the program printed the other answer, because renaming a
label leaves the values where they are. The rule that a fix which leaves the
defect standing is a `guess` was written on 2026-09-08 and had no executor. It
has one now, and the deciding fact is a fact about the call rather than a
premise about the world: does the written label name **another** position of
this callee?

**The precedent runs one way, and the closest one runs the other.** The
historian verified that Objective-C cannot express the exemption, because the
label is the selector; that Swift exempts by semantics and never by library
membership, and rewrote its own standard library rather than exempt it; and that
**PEP 570 is the opposite case**, where CPython's built-ins were *stricter* than
user code and the language changed to let user code declare the same strictness.
`gofix` and `cargo fix` are why a thousand mechanical call sites is routine, and
`2to3` is why the rewriter must be the compiler's own parser. The class has a
CVE, 2006-7049, `strstr` and `strrpos` with their arguments swapped and remotely
exploitable, and a name, CWE-683; Google's study of 200 million lines found one
that had stood thirteen years.

**One of the 1022 sites was already malformed**, which is the historian's
prediction scored by grep: `fail("compiler bug", …)` in `selfhost/emit/ctype.hero`,
where § 6 says a code is a stable snake_case string. Nothing matched it. No site
was **transposed**, and that is what made the mechanical repair safe: labelling a
call that is already swapped would have cemented it, so the sweep was gated on
that measurement rather than on hope.

## What broke and why

- **The first build said `internal error`**, not a diagnostic. The repair fires
  inside `selfhost/library_source.hero`, the library the compiler carries as
  embedded text and compiles for every program, and a diagnostic there is
  reported as a compiler bug. Its seven `fail(` calls were the first thing the
  repair had to relabel, by hand, because there the string **is** the program.
- **Ten more sites live inside the compiler's own test programs**, also as
  strings, and nine tests went red until they were relabelled. One of the ten
  had to be put **back**: `check/labels.hero`'s own test asserts that a
  POSITIONAL call is refused, so its text must stay positional, and the span
  arithmetic in the assertion says so.
- **`heroes check --apply --in-place` cannot do this job**, measured by the
  compiler seat: it applies fixes to the root module only, and 663 of the
  compiler's 706 one-line sites live in 102 nested modules. The sweep is a
  script over whole-file offsets, and the compiler is what judges it: after the
  sweep, zero `needs_label` anywhere.
- **`heroes fmt` re-wraps the lines that pass 120 columns**, so five decided
  file ceilings rose by their measured lines. The seat predicted ten rows and
  the instrument says five; the gap is in the sitting's file rather than tidied
  away.
- **Eight emission traces moved.** The ffi seat predicted two, and predicted
  correctly that every difference would be a `#line` number: proved by hashing
  all eight with the `#line` rows stripped, before and after, and finding them
  identical. They were re-blessed with `UPDATE_EMISSION=1`, which is the one
  suite in the harness that writes into the repository.

## What landed, and what carried forward

**The rule.** `selfhost/check/labels.hero`, 47 code lines, both halves, called
from the two paths that type a built-in call; the two signatures carry their
parameter names as literals in `selfhost/inventory.hero`, beside the table that
already enumerates every built-in, with the premise that they are the only two
written as a falsifiable claim. The emitted C is byte-identical and the runtime
ABI stamp is untouched: this is a frontend change, and the ffi seat proved it by
hashing the C for a positional and a labelled call.

**The number.** `heroes mutate --operator swap-args --survivors`, same corpus,
before and after:

| | before | after |
|---|---|---|
| mutants | 1910 | 1910 |
| killed by `heroes check` | 1441, 80% | **1733, 96%** |
| survivors | 367 | **75** |
| survivors that are a swapped `fail(` | 159 | **0** |

**The document.** 5599 real tokens, +1, digest `17553bf94f6feb87`; § 6 shows the
labels § 9 always required and § 11 says `slice` once instead of twice.

**What carried forward.** Defect 026, which is what leaves this milestone
untagged: a function type names no parameters, so the same swap is still silent
through a function value and through a C callback, both re-run in the landing
session rather than taken from the seat's report. `panel 127` in
`docs/work/DECIDE.md`. And the rule that labels are checked **in order**, which
the compiler enforces and no sentence of the document states: the warden priced
it at +7 and recommended filing rather than buying, and it is in defect 026's
neighbourhood rather than in the document.

### The chain entry

| 46 | **M-labelled-builtins** | done 2026-09-11 | **untagged**, defect 026 | [045](journal/045-labelled-builtins.md) | defect 025 repaired: the same-typed-argument rule reaches the two built-ins it did not, both halves of it, 1022 call sites relabelled, and the classic argument inversion drops from 367 survivors to 75 with the swapped `fail(` at zero · **§1.2** |
