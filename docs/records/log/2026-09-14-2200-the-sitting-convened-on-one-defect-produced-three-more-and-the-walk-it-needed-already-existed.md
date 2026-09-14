# The sitting convened on one defect produced three more, and the walk it needed already existed

2026-09-14, 22:00. Panel 149, convened by author instruction on
`docs/work/DEFECTS.md` 033: *fix the open defect with the panel*. Full panel,
five seats plus the completeness critic. The sitting is
`docs/panel/149-the-walk-already-existed-and-the-instrument-was-counting-calls.md`.

## What was decided

A handle is what a type **reaches**, through group-record fields and fixed-array
elements, to any depth, and the rule asks it of `consumes` parameters as well as
of results and `@` out-parameters. The diagnostic prints the dotted path. **No
refusal lands** for a record reaching several handles: one mark is one obligation
on the whole value, which is what C ships. The releaser a mark names must resolve
and must carry `consumes` on a parameter of the marked type or a handle it
reaches. Spec cost **+4 real**, 7974 → 7978 against a ceiling of 10240.

## Half the question did not exist

The defect asked *how deep: behind two records, behind an optional, behind a
list.* Measured before the briefs went out: **`-> Slot?` and `-> [Slot]` are
already `error[ffi_type]`**, and so is a result record declared outside the
group. The FFI vocabulary had answered two thirds of the question years of
sentences ago, and nobody had asked it.

What does exist is four shapes, all reaching the runtime and aborting 134: a
handle in a field, two records deep, inside a fixed array, and through an `@`
out-parameter.

## The surface was never the problem

`function pair_make(n: i64) -> Pair acquires slot_close` parses, checks, emits,
counts and exits **0** today. `-> Pair borrows` exits **0**. Nothing was missing
from the parser, the emitter or the runtime. **Only the rule failed to ask.**

## And the walk was already written, for a different rule

`selfhost/check/map_keys.hero:103-160` is a 58-line reachability walk that
returns a dotted path, ships with three tests, and already prints
`` `Conn.handle` ``. Found by the compiler-engineer. So the repair is a second
caller and not a second walk, and the diagnostic's path is free.

## Three defects the sitting produced while ruling on a fourth

- **034**, by the **spec-warden**, while pricing the spec cost of the repair it
  was asked about. The counter counts CALLS and not MARKS: two `acquires` on one
  call emit one increment, so **the correct program aborts at 134 and the leaking
  one exits 0.** Re-run by the coordinator with two distinct handle types before
  it was filed.
- **035**, by the **compiler-engineer**, while measuring the walk's termination.
  `reaches_handle` gives up at a depth bound, in silence, and the sentence
  licensing that bound sits on its float twin where it is true. The walk was
  copied and its premise was left behind on the original. Three constructions
  give three different boundaries, which is the finding.
- **036**, by the **completeness critic**, outside the sitting's question.
  `owned <freer>` after a non-`cstr` result checks clean and makes clang blame
  the compiler at exit 2. **The coordinator's first reproduction failed**: the
  defect needs the freer declared, and without it `unknown_freer` catches it
  correctly. The guard exists and watches the wrong half.

## What the critic changed, and it was everything

Step 3b again earned its place. It dissolved the flat contradiction between the
warden and the ergonomist — one measures what the text **licenses**, the other
what it **causes** — and then refuted the warden's decisive argument from the
document itself: `owned ident` sits in the identical grammatical position and is
narrowed in prose one line above `acquires`.

It found a seat's veto **lifted by that seat's own stated condition, inside the
sitting**: the compiler-engineer would withdraw on a real header returning a
struct by value carrying two pointers to release, and the ffi-pragmatist had
already compiled `LoadFont`. Neither noticed.

It found that **nobody cited panel 147** — zero hits across five reports and six
briefs — so the historian argued a route that sitting had refused on its own axis
and the author had ratified the same day. **The coordinator's shared brief is
where that citation was owed.**

And it asked the question that makes the repair work at all: **does the rule's
consumer side reach as deep as its producer side?** It does not.
`consumed_types` asks at one level, so `UnloadFont(font: Font consumes)` leaves
the map empty and **the rule as framed would have stayed silent on the only
shipped-library case while erroring on the contrived one.** Verified: the same
`Font` used as a map key errors with `` `Font.recs` is a handle ``, which proves
the walk sees it and the rule simply does not call it.

## The precedent reshaped the question rather than answering it

The historian's survey found the split is not how deep a rule looks but **where
the word is written**. Every system marking the FUNCTION stops at its own
signature — clang's `cf_returns_retained`, Swift's `SWIFT_RETURNS_RETAINED`,
GObject-Introspection, SAL, Core Foundation's Create Rule. Every system reaching
a field marks the FIELD or the TYPE — ARC's `__strong` plus a calling convention,
Splint, Vala. Heroes has ratified that it marks the call, so the exit the
industry took is closed to it.

Two warnings carried forward: **Swift shipped panel 148 R5's rule and retracted
it to a flag inside one release cycle, for noise**, and Cyclone's authors wrote
that their warnings *"were of little help, since there were too many false
positives"*.

## One prediction was run inside the sitting and confirmed on its own number

The historian predicted the fixed array would break the mark's **arity** before
anything else, and named the outcome: `+3` or abort 134. Run on receipt:
`Slot[4]`, one mark, four releases, **exactly +3, abort 134.**
