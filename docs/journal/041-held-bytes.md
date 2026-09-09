# 041 — M-held-bytes

Closed 2026-09-09. Two steps, two sittings on the same day, one defect closed
that the milestone before had to leave open, and a capability where every
candidate had been a refusal.

## Goal

Close defect 024: a `.cstr()` lend that a C function RETAINS past the call is
accepted at exit 0 with zero diagnostics and is a heap-use-after-free under
`--sanitize`. `M-cstr-lifetime` had closed the lend that escapes its frame and
filed this half against design.md §4.19's reserved keyword, *"a borrowed
pointer you must not touch"*, because no rule about where a Heroes expression
stands can see it: the retention decision is the C function's own argument.

## What surprised

**The answer was not a keyword that refuses, and every keyword that refuses was
refused.** Three candidate rules went to panel 124 and each came back with a
veto. A fourth, the coordinator's, was raised when the first three had fallen
and was vetoed by the seat whose own sentence had suggested it. What closed the
class was a capability: bytes Heroes makes and frees which C may read for as
long as the program says. The ffi seat found it by correcting its own report,
which had said *"A's only remedy"* on the strength of having tried `strdup`
(answers `cstr`) and not `malloc` (answers `ptr`). One line of difference
between a class that cannot close and one that can.

**The header is why no declaration-site mark can work, and two seats read it
independently.** `sqlite3.h:4888`: the FIFTH ARGUMENT *"controls or indicates
the lifetime of the object referenced by the third parameter"*, and its three
options are exactly §4.19's three reserved cases, on the same parameter of the
same declaration, chosen at runtime. `curl_easy_setopt` repeats it in its
second argument. So a mark on the parameter is either a false refusal or an
empty comment, and **0 of 71 `cstr` parameters in this tree are decidable from
a header**, because `const` promises no write, not no retention. Value-dependent
retention appears to be unprecedented, sourced as an absence over thirteen
systems; what Vala shipped for this exact function is two declarations for one
C symbol.

**Holding is O(1) and the compiler deciding is O(n), on the same program.**
Today's lend: 1,425,408 bytes at a million trips, flat across a tenfold change.
The coordinator's duration rule: 81,903,616. A lease released inside the loop:
1,441,792, which is +1.1%. The 57x was never the price of holding bytes; it was
the price of the compiler choosing when.

**The runtime guard the coordinator wrote read freed memory, and both seats of
the soundness lane measured it further than the brief had.** A magic word eight
bytes before the pointer is undefined on three of the four inputs its own
message named — a C literal, a C `malloc`, a stack buffer — because it must
dereference before it can validate; and a double release cannot be told from a
never-held pointer by any such read, because the block is gone. The comment
above the function promised three refusals where the code had two. Five
unsanitized runs printed the wrong message five times out of five.

**The rule that made a second release impossible is three clauses, and the
third was found by running a seat's own table.** The compiler seat measured
what a `cstr` cell may do today and concluded the hole was *"exactly and only
the two cell forms"*. But `g = c`, then `end_lease(@c)`, then a read of `g`
through C is a use-after-free two clauses cannot see. So a lease's name stands
only as an argument of a call — the lend's own position rule applied to a
cell — nothing but its own `end_lease` writes the cell, and `end_lease` takes
only such a cell.

**The spelling the coordinator chose collided 29 times in 11 files**, measured
by the reservation mechanism itself: `release` is a top-level function inside
the compiler, and both words live in five shipped examples. Panel 121's
precedent applied — refuse the spelling, keep the form — and `lease` /
`end_lease` cost zero by the same measurement.

**And a rider that binds whatever the lifetime rule is.** Until this milestone
NOTHING that ran lent a computed string inside a loop; the three computed-lend
goldens were `check`-only negatives refused before the emitter. So every one of
panel 124's candidates could have shipped green. `tests/golden/run/lend-inside-a-loop.hero`
exists because of that sentence.

## What broke and why

**The ABI stamp moved before the seed did, and CLAUDE.md § Commands' first line
went red in the working tree.** The stamp is written by the COMPILER
(`selfhost/emit/decls.hero:79`), not by the header, so moving the header alone
left the committed seed asserting `21 == 22`. Both seats caught it. The order
that works keeps a compiler alive across the bump: header back, emitter's line
forward, build `heroes-next`, header forward, `heroes-next --emit-c` into the
seed, clang, `cmp`. 224 lines in 222 files carry the stamp and all landed in one
commit. And `seed/README.md` had promised a test that asserts the two numbers
agree; no such test exists outside its prose, and the sentence now says so.

**The layout suite refused the first shape twice, and both times the seam it
asked for was one the module doc had already named.** `count_tokens.hero` at
303 lines split into the half that opens no socket and the half that does;
`lending.hero` at 366 split into the lend and the lease, whose rule is the
lend's applied to a cell. Neither split was a line-count cut.

**Two emission suites went red for the right reason.** `emit`'s six goldens and
214 `tests/emission/` traces carry the ABI stamp; `unsupported`'s eight carry
the note that lists every emitted built-in, which gained two names. Every one
was regenerated only after its diff was read and found to be exactly that line
and nothing else.

**And the resolver stops the file before the lease sweep is reached.** A copied
lease that nobody reads is `unused_binding`, and the check ends there; the
golden that pins `lease_escapes` reads its copies on purpose, and says why.

## Predictions scored

| prediction | verdict |
|---|---|
| ffi-pragmatist, panel 124: `examples/ledger/db/sqlite.hero` ships TRANSIENT at +2 -1, comment deleted, `main.expected` unchanged, `--sanitize` clean on this Mac and the Linux leg, no shim, no `heroes cc` | **CONFIRMED on the Mac half**: +2 -1 in code, byte-identical output, ASan and UBSan clean, comment deleted and corrected. **The Linux leg is unrun** and is owed before the push |
| ffi-pragmatist, panel 124: no declaration-site annotation lands on `sqlite3_bind_text`'s `text` parameter | **CONFIRMED**: none did |
| ffi-pragmatist, panel 124: with the capability spelled as a reserved word, both residues build at exit 0, are clean on Mac and Linux, and a forgotten release is a diagnostic or a named panic accusing the program | **CONFIRMED on the Mac half**: `lease-c-keeps-the-pointer` 13/13/13, `lease-tail-points-into-the-bytes` 7, both clean; `abort-lease-never-ended` is `!panic: 1 lease(s) never ended`. Linux unrun |
| ffi-pragmatist, panel 124: any implementation extending a lend to the frame's exit gives `x9.hero` a peak RSS above 50 MB | **MOOT AND CONFIRMED IN PASSING**: no such implementation landed, and the seat's own measurement of it read 81.9 MB |
| compiler-engineer, panel 124: any A or B shipped without a relay clause leaves a compile-clean use-after-free; `lending.hero` above 260 lines, `ast.hero` >= 472 | **MOOT on the first half**: neither A nor B shipped. **FALSIFIED on the second, in the seat's favour**: `lending.hero` landed at **234** in the suite's unit and `ast.hero` gained no field |
| compiler-engineer, panel 125: the feature lands with `variant Ty` at 17 cases, `DECIDED` at 16 entries, `lending.hero` <= 275 | **CONFIRMED**: 17, 16, and 234 |
| ffi-pragmatist, panel 125: the ledger CAN ship a per-column held helper keeping its `fail(...)` with no shim | **UNSCORED HERE**: the ledger ships TRANSIENT by panel 124 R7 and no held helper was written for it; the shape is proven by `lease-c-keeps-the-pointer.hero` instead. Not lapsed, because its instrument exists and a later milestone may write that helper |
| ffi-pragmatist, panel 125: `hero_held_release` never appears as a C destructor argument | **CONFIRMED**: it cannot, clang refuses the signature, and no example tries |
| spec-warden, panel 124: marks needed to hold `./heroes build selfhost/main.hero` at exit 0 are exactly 0 in `selfhost/` and 1 in `examples/` | **MOOT**: no mark landed; `selfhost/` needs 0 leases and `examples/` uses none |
| spec-warden, panel 124: both reproducers move to a compile-time refusal | **FALSIFIED, and the record says why**: the class is undecidable from any declaration, so no refusal was possible; what landed is the sound spelling and a refusal of its misuse |
| historian, panel 124: a one-value-per-parameter annotation will not bind `sqlite3_bind_text` in a single declaration | **CONFIRMED by absence**: no annotation landed at all |
| llm-ergonomist, panel 124: on 10 keeper and 10 reader tasks under the three candidates | **LAPSES to M-thesis-harness**: needs a task set that does not exist |

## What landed, and what carried forward

§4.19's fourth case as three built-ins' worth of surface — `s.lease()`,
`end_lease(@x)` — with zero new types, zero new IR instructions, two runtime
entry points, a fourth allocation counter whose exit message accuses the
program, and three checker clauses in a module of their own. The spec says it in
four lines at **+103** real tokens (5128 to 5231 on `claude-opus-5`, 853 free net
of the FFI floor), which is 32 more than the cheapest statement of the
capability alone, because the two clauses that make a second release impossible
are in the document. Seven goldens: three `check` refusals with their
annotations, three `run` shapes including the forgotten release, and the rider.
Defect **024 closed**, with its limit named: a lease C retains past the
`end_lease` the program wrote is a use-after-free no rule can see, and the Linux
`--sanitize` leg is its instrument.

**Carried forward**: the Linux and Windows legs, run once before the push by
author instruction; panel 125's ratification; the ffi seat's unscored
prediction about a held helper in the ledger; and the interpolation
implementation, which is unblocked and next.

**Appended 2026-09-09, the two other platforms run before the push.** The ffi
seat's two predictions that were confirmed "on the Mac half" above are now
confirmed on all three. **Linux**, in the container `docs/environment/linux/`
describes: the seed builds from C alone, the compiler's 618 tests pass,
`examples/ledger` with `SQLITE_TRANSIENT` is byte-identical to `main.expected`
under `--sanitize` with **LeakSanitizer** on, which exists on that leg and on no
other; `lease-c-keeps-the-pointer`, `lease-tail-points-into-the-bytes` and
`lend-inside-a-loop` are clean there, and `abort-lease-never-ended` exits 134
with its message and no sanitizer report. **Windows**, on the box: the seed
builds with `seed/README.md`'s stack flag, 618 own tests and 125 harness tests
pass, the three run goldens print their expected output, and the abort case
exits 127, which is what the shipped `abort-null-cstr-into-c` exits there too.
**And Windows found a defect the other two could not**: the golden's C side
declared `stash_len` as `unsigned long`, which is 32 bits on Windows and 64 on
the others, and the FFI's own `_Static_assert` refused it against the `.hero`'s
`u64`. It answers `size_t` now, which is what `strlen` answers on all three.
