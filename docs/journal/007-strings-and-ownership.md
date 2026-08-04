# 007 — M5b: strings, and where a reference lives

Milestone M5b · 2026-08-04/05 · panel 021.

## 1. Goal

`str` reaches C, values acquire lifetimes, and the one number panel 006 left
undecided gets decided. `crates/heroes/src/own.rs` is the first **IR→IR** pass —
`incref`/`decref` as real instructions, visible in `--dump-ir` — and
`Program::phase` lets the verifier name which pass to blame.

368 tests (was 363): 331 crate, 13 golden harnesses over 68 cases, 24 surface. Every
`run/` case now runs in **three** configurations (`-O0`, `-O2`, and
`--sanitize`). The runtime went from 50 lines to 285.

Three outcomes:

1. **The leak counter caught a real defect on its first run**, which is the entire
   reason it exists: `panic: 3 heap blocks still live at exit`. All three traced to
   one missing row — `+` on a `str` is an `Op::Binary`, it reads like arithmetic and
   it is a constructor, and only the result type tells them apart.
2. **The phase-indexed verifier refuted the ownership pass's design twice**, on real
   programs, within an hour of the invariant being written — before any test did.
3. **The `f64` rendering came out better than the panel expected**, because two
   judges' findings compose: with the historian's subnormal branch, the
   spec-warden's own counterexample to "shortest" disappears.

## 2. What surprised

**The invariant was wrong twice, and both times it was too strong rather than too
weak.** Panel 021 handed over a wording — "a refcounted temporary is read only in the
block that defines it" — and it fired within minutes on `assert s == "x"`, whose
operands legitimately cross into the abort block: harmlessly, because a string
literal is a static block nothing ever releases. Rewritten as "a *released* temporary
never crosses a block", it fired on `name + " scored " + got.must().to_str()`, where
`.must()` opens a block in the middle of an expression. That second one was not a
corner case. `?`, `&&` and `if`-as-an-expression do the same thing, so *any* expression
containing a fallible call has a value crossing an edge.

**And the second refutation killed the design, not just the wording.** Releasing an
owning temporary at the end of its defining block cannot work when expressions
straddle blocks, and the alternative on the table was liveness — the analysis that
choosing slots over phi nodes was supposed to avoid. The answer was to cash panel
019's decision properly: **an owning temporary is *moved* into a synthetic slot in the
block that defines it.** Nothing is owned by a temporary past its own instruction,
nothing owned crosses an edge, every release is a slot's, and a loop does not
accumulate because the slot's overwrite releases. One sentence then makes the whole
scheme uniform: every value that reaches a store is borrowed.

**Position is what distinguishes a sweep from a store.** The check that a returning
block releases every slot it owns went through three forms. Counting `Decref`s was a
proxy and stopped meaning anything the moment the pass changed shape. Pairing a
`Decref` with a `Load` of the same slot could not tell the two apart either — a store
to a counted slot *also* loads the old value and decrefs it, and that pair is
byte-identical to a sweep pair. What separates them is that the store's load
necessarily precedes the store and the sweep's necessarily follows it. Found by
deleting a sweep decref by hand and watching the check stay silent.

**Two judges' findings composed into a better answer than either had.** The
spec-warden's objection to the word "shortest" rested on `5e-324` rendering
`4.94065645841247e-324` under the ladder. The historian, separately, found that
gnulib's shipped `ftoastr` starts the precision at **1** for a subnormal rather than
at 15. With that branch, `5e-324` renders `5e-324` and `1e-323` renders `1e-323` —
matching Python. The counterexample dissolved; the correction to the *wording* stands
anyway, because round-trip-exactness is what is guaranteed and shortest-ness is not.

**"The runtime never calls `setlocale`" was worth exactly nothing**, and the
demonstration is better than the argument. Under `de_DE.UTF-8` the ladder emits
`0,1.0` — malformed, because the decimal-point rider looks for a `.`, finds none, and
appends one — and the round-trip check *cannot see it*, because `strtod` reads the
same locale and is wrong consistently. PEP 331 is the shape: CPython never called
`setlocale`, GTK+ did, CPython broke anyway. The fix is a `uselocale` window, which
covers `snprintf` and `strtod` at once.

**`str` by value is an FFI decision, not an ergonomic one.** Written as a pointer, the
wrong `str`→`cstr` conversion compiles clean *with an explicit cast* and passes a
refcount word to `sqlite3_open`. Written by value it is `error: operand of type
'HeroStr' where arithmetic or pointer type is required` — inexpressible rather than
wrong, which is what §4.19 promises. And the eight-byte magic word in the header is
there because `HeroStr fake = {sqlite3_column_text(st,0), n}` compiles with zero
warnings under `-Weverything` and corrupts the library's own heap, with ASan silent.

**AddressSanitizer is not a leak detector on this platform.** `detect_leaks is not
supported on this platform`, exit 134 if you ask for it, and a program leaking 999
blocks exits 0 in silence. Two judges measured it independently. The milestone that
introduces reference counting would have shipped with no leak gate at all, behind a
configuration that looked exactly like one.

## 3. What broke and why

**Symptom** · `panic: 3 heap blocks still live at exit`, on the first program
containing a string. **Cause** · `own.rs::allocates` classified by defining
operation, and `+` on `str` is an `Op::Binary` — §4.14's table puts it in the same row
as `+` on `int`. **Fix** · one row; `Binary` allocates, and it can only be reached
when the result is counted. `tests/golden/run/fixedbugs-concat-leaked.hero` is named
after it.

**Symptom** · `bb4: reads $14, which bb1 releases`. **Cause** · the design, not a
typo: end-of-block release cannot survive an expression that straddles blocks. **Fix**
· move ownership into slots (above).

**Symptom** · `returns without releasing $own7, a slot it owns`. **Cause** · the pass
created synthetic slots *and* consumed the sweep list in one walk, so a block that
returned early swept only the slots invented before it. **Fix** · two walks, and the
module doc says the order is the correctness argument rather than a detail.

**Symptom** · a deliberately broken sweep produced no diagnostic. **Cause** · the
check could not distinguish a sweep pair from a store's old-value pair. **Fix** ·
position (above).

**Symptom** · `assert sentence_of([…]) == "one two"` rejected by the verifier.
**Cause** · panel 021 predicted precisely this and scheduled the repair for M6 with
`Abort::Assert`; the invariant arrived first. **Fix** · `ir/asserts.rs` routes a
counted operand through a synthetic slot — the panel's own prescription, two
instructions on a path that is about to abort the program.

**Symptom** · the spec's own tests failed twice on a two-line spec addition.
**Cause** · a prose parenthesis inside the built-ins list read as a built-in name
(`to`), and the list's extraction ran to the blank line, so "An `f64` always prints…"
made `f64` a built-in. **Fix** · skip parentheses outside code spans, and end the list
at its own terminating sentence.
