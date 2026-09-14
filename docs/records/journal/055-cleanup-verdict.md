# 055 — M-cleanup-verdict: the obligation is created by a call

## Goal

Rule on a scope-bound release — `defer` or another form — for the obligations
the FFI puts on every path out of a scope. A decision, not a feature, in
M-closures-verdict's shape: a form enters with its own milestone, or it lands as
a design.md Part 6 row naming the program that would make it wrong.

The milestone's own file forbade arguing either way without a count that did not
exist: *how many acquire-and-release pairs stand in `selfhost/` and under
`examples/`, and how many early returns and `?` operators sit between an acquire
and its release.* Taking that count was step 1, and it did not survive contact
with the tree in the shape anybody expected.

## What surprised

**The census rewrote its own question, twice.**

The milestone was scheduled on the sentence that `owned` and `lease` both put a
release obligation on **every path**. Measured: **`owned` puts none.** The
compiler frees the C string at the call, inside a null guard — visible in the
emitted C, and confirmed by running a program that returns early between the
acquire and the end of its function. **`lease` puts one and a miss is loud**:
exit 134, *"1 lease(s) never ended"*, saying how many. Two thirds of the premise
was false, which is M-deferral-ledger's finding of one milestone earlier
happening to the document that scheduled this one.

So the C handle is not a *third* obligation beside two others. It is the **only**
one that leaks in silence, and that is the whole subject.

Then the second narrowing, and it looked decisive: **the language already owns a
scope-bound release.** The exit sweep over the slot table releases on every path
including `?` — rule 5 of the ownership pass says it had to be built that way,
because `.must()`, `?`, `&&` and if-as-value all open a block in the middle of
an expression. A handle sits outside it not by policy but because it has nothing
to call: in one binding's emitted C the three `str?` slots each carry a
`_retain` and a `_release`, and the handle carries `_eq`, `_hash`, and nothing
else. The question stopped being *should this language have `defer`* and became
*can a C handle be given a release function and join a table that already
exists*.

**And that was still not narrow enough, which is the sitting's finding.**
Ownership is a property of the **call**, not of the type. The same
`sqlite3_stmt *` is handed back owned by one C function and **borrowed** by
another — compiled at the sitting, `borrowed == owned: true` — so a releaser
named on the type closes a connection it was lent. With the release in place a
program printed **0 rows where the table holds 3**, at exit 0, with the
sanitizer silent.

**The rule that settles it was already written.** This project has placed an FFI
lifetime fact four times and never once on a type: `owned` rides a result,
`lease` a declaration's initialiser, `consumes` a parameter position, the `cstr`
lend an expression's position. *A narrowing asks the value, never the world.*
Three seats reached the refusal the expensive way, each from its own mandate,
and the contract had the answer in one line.

**A shape worth keeping, about tests.** The one golden in the tree that compared
a handle asserted `s.handle == nullptr` against an expected `true` — and its
handle **is** null, so the right answer and a constant `true` are the same
value there. It passed for a whole day while handle equality was broken. A test
asserts a value; what you want to know is whether the code computes it, and the
two come apart exactly where the expected value coincides with what a broken
implementation returns by default.

## What broke and why

**Defect 032, and it was a §1.12 breach one day old.** `==` on a C handle
generated `return true;` — so `h == nullptr` was always taken and `h != nullptr`
never fired. The commonest line in any binding, the null guard after an init
that can fail, did not work, and a null handle sailed on to the next C call. The
wrong answer travelled through a record holding a handle, an array of them and a
fallible alike: five shapes measured, five wrong.

The cause is the shape this project has a rule against. The emitter's zero-field
arm rested on *a fieldless record is `error[empty_record]`* — a premise about
the **world**, true the day it was written, killed by the previous milestone when
a fieldless group record with a `tag` became legal. **When the premise died the
argument stayed valid**, so the comment went on reading as correct; and the
fallback sat in the quiet direction fifteen lines below a sibling arm that
answers `hero_panic` for the same *this cannot happen*.

**How it was found is the part worth repeating.** Not by using the language —
by a census probe answering `statements still open: 0` while saying
`sqlite3_close says BUSY: true` in the same breath. Two sentences that cannot
both be true. The same program in C answered 1 and true. `while s != nullptr`
was a loop that never ran.

**A corpus leak, beside the program that documents it.** `examples/sqlite/`
dropped the connection on a failed open, while its sibling carried seventeen
lines measuring on two platforms that a failed open still hands one back, and
closed it. The program a reader meets first taught the opposite of the program
next to it.

**And the sitting's own census was wrong four ways.** Written in the morning,
handed to five judges as fact, audited by nobody until the completeness critic:
exposure is 22 paths in one file rather than 23 in two; the pair list was short
by at least seven, so ≥19 and not 12; *"10 of 21"* is 8; and **all 22 paths end
in `exit(1)` or `abort`**, so no shipped program leaks a handle and then goes on
running. The headline *exit 0, in silence* was true of a synthetic reduction and
overstated the corpus. The class survives; the urgency does not. Corrected
underneath itself, and the conservative resolution that correction supports is
written into the sitting so the author can take it.

**A records check that had been wrong since it was written.** Keeping the
sitting's briefs beside it made twelve files that are not sittings get asked for
an author's verdict — because the predicate read every `.md` under the panel
tree as a sitting, and had done since a 2026-08 subdirectory was created, masked
only by a floor that skipped number 0. The first sitting numbered above the
floor with files under it made it answer out loud.

## What landed, and what carried forward

**The ruling.** A releaser named on a handle TYPE is refused to design.md
Part 6, on its axis and not its spelling, with a falsifier that can expire in
its turn: a header-derivable signal separating a C function that gives ownership
from one that lends it. A scope-bound statement does not enter as drafted — two
seats found the same `@`-cell defect from opposite ends — and would be
block-scoped if it ever did, five languages having corrected Go's one choice.
Route C, refusing outright, was **not** taken either: C itself adopted `defer`
this year, and the Part 6 row that would have hosted the refusal names a
falsifier that already fired when `consumes` landed.

**What enters is an axis.** The obligation is marked where it is created — on
the acquiring call — and **the compiler never picks the release call**, the one
clause every refusing seat and all three independently proposed fourth routes
agreed on. It takes `M-marked-acquisition`, entered in the chain at 57, whose
first act is to price two instruments the sitting deliberately would not choose
between: a `consumes`-shaped mark, and escape refusal, whose machinery ships
twice already and which the critic found had never actually been refused —
panel 122 refused an *inferred* release, which is a different form.

**Two predictions were scored inside the sitting that made them.** The
historian predicted a per-type mark would prove insufficient because some
producing function returns a borrowed instance, and named `sqlite3_next_stmt` —
the function this milestone's own census had used. The ffi seat collected it an
hour later. The spec-warden predicted a re-count would read 22 in one file
rather than 23 in two; the critic's audit found exactly that.

**And the briefs are in the record for the first time.** Panel 140's finding —
*the briefs are prompts; they vanish when the workflow ends* — was ratified in
2026-09-13 and no sitting had performed it, because what was implemented is a
session scratchpad that also vanishes. Which home the rule meant is the
author's to settle and is filed.
