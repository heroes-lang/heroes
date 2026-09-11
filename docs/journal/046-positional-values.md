# 046 — M-positional-values

Closed 2026-09-11, **untagged**: defect 026 is narrowed rather than closed, and
CLAUDE.md § Verification tags a milestone only over a clean list. One sitting,
panel 128, five seats, on a defect the milestone before it had filed and could
not repair.

## Goal

Repair defect 026 at the author's instruction. A call made through a function
type escapes the rule that two same-typed arguments must be named, because a
function type names no parameters: `selfhost/check/walk.hero:1541` reads the
label to expect from a **declaration**'s parameter span and
`(function(A, B) -> C)` has none at any of its sites.

## What surprised

**Three of the four routes were vetoed, each by a different seat, and each on
soundness rather than on price.** That has not happened before in this project's
sittings, and it is why the resolution is smaller than the subject. The
positional sentence was false as balloted; refusing the type forecloses three
steps of the FFI ladder and deletes a comparator that compiles today; names
inside the type are core by Part 5's test, eleven modules, two of them at their
decided ceiling.

**Names on peers buy a label, not a check.** The ergonomist wrote the comparator
task under both texts and found it first: `better(a: best, b: x)` compiles
exactly as `better(best, x)` does and returns the smallest where it promised the
largest. The label rule catches a **reordering**; it cannot catch a reader who
does not know which operand is the candidate. Swift's own guideline says the
same thing from the other end, *omit all labels when arguments can't be usefully
distinguished*, with `min(number1, number2)` as its example.

**A veto lifted on a measurement, and that is the shape worth keeping.** The
warden refused the document's positional sentence because a running program
contradicted it: an invented label was accepted and ignored. It stated its own
lift condition rather than a preference. The compiler seat then built the route
nobody had listed, twenty-five diff lines, and that program became an error. The
sentence was true before it was written, and the veto let it in by its own terms.

**The compiler's note was obedient, not complicit.** It prescribes
`less: (function(A, A) -> bool)` because design.md §4.12 says *functions only*
and *if an operation on `T` is needed, pass it as a parameter*. The seat then ran
the alternative and found the negative answer that matters: **the language has no
way to write a generic comparison the label rule can see.** A concrete record
carrier scores 100% on the instrument; a generic one cannot exist, because
`record Pair<A>` is refused.

**At the boundary the swap reaches memory, three times measured.** SQLite's
collation callback with its names swapped: exit 0 both ways, and a
heap-buffer-overflow under the sanitizer. zlib's `free_func`: exit 133, an
invalid free. The same swap in the shape a real user picks: **exit 0 in
silence**, and a heap-use-after-free. And C cannot help: `_Generic` says the two
parameter orders are the same C type, so the checker would be the whole defence.

**Ada answered in 1995 the question the sitting thought was new**, and Swift
answered it the other way and then reversed itself. Names in a function type
outside the type's identity is Ada's access-to-subprogram design, and Go's and
TypeScript's; names inside the identity is what Swift removed in Swift 3 for
simplification, with the cost recorded within seven months and a door left open
that nobody has walked through in ten years. **The two seats disagreed on the
record and the sitting took robustness**, which is § Precedence rank 3, and put
Ada's shape beside it as what the author can choose.

**The defect does not ship where the defect said.** It ships inside `fold`: a
program hands the library a function and the library calls it positionally with
no name in the chain, printing `cba` for `abc` at exit 0. `fold`'s type has two
distinct letters, so **no route the sitting considered reaches it**. Five golden
sites had implied coverage of the class and the record now says they do not.

## What broke and why

- **The golden went in without its expected file** and the whole `check` suite
  answered *the check/ suite could not run*, which is the right shape for a
  missing oracle and a blunt one to read. The expected text was built from the
  compiler's own output rather than typed.
- **The `.fixed` twin must be what `--apply` writes**, annotations and all: I
  stripped the annotation on the reasoning that the fixed program provokes no
  diagnostic, and `fixes` refused it, because the file is a mechanical output and
  `--apply` does not touch comments.
- **The ceiling moved five lines, exactly as the seat predicted before building**,
  and this time the table and the assert sixty lines below it moved together. The
  day before they did not, and the CI went red on all three legs.
- **A blanket revert undid work it was not aimed at.** `git checkout --` on one
  file to undo a bad edit also took the row count with it, because both edits
  were in that file. Redone together.

## What landed, and what carried forward

**The refusal.** `error[label_on_function_value]`, with a note that says §4.9's
rule reads a declaration's names and a value of that type has none, and a
`certain` fix that removes the label and leaves the value where it is. Twenty
files' worth of nothing else: the compiler's non-comment change is 25 diff lines
in two modules, and the emitted C does not move.

**The note.** `value_errors.hero:94` says to call the comparison **by position**
and names the two roles, `less(candidate, best)`.

**The document.** § 3's function-type row says the call is positional and a label
there is an error: 5624 real tokens, +25, digest `222ad91e3a6b7aa3`, ledger row
65, which records that nothing paid for it and why CLAUDE.md §12 carries it.

**What carried forward, in one number.** `heroes mutate --operator swap-args`
reads **50%** on a call through a function value, before and after. The inversion
is still silent. Defect 026 is narrowed to it, the conservative route is
scheduled at M-check-completeness with the measurement that lifts its veto, and
`fold`'s shape is named as owing an answer of its own.

### The chain entry

| 47 | **M-positional-values** | done 2026-09-11 | **untagged**, defect 026 | [046](journal/046-positional-values.md) | defect 026 narrowed: a label at a call through a function value is refused, the note stops prescribing the hole in silence, and `spec § 3` says the call is positional — a sentence vetoed as false at the ballot that the refusal made true · **§1.2** |
