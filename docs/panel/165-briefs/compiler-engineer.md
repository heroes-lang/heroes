# Panel 165 — brief for the compiler-engineer

**Read `docs/panel/165-briefs/00-shared.md` first.** It carries the question, the
census, and the two refusals the compiler raises today. Everything in it was run.

## Your question

Panel 164 asserted that route 6 *"needs no new expression, no lend, no built-in
and no position rule — one widening of §4.19's parameter list, with ordinary type
identity doing the matching."* **Price that sentence, and say whether it is
true.** If it is false, say what it leaves out and what that costs.

## Where the refusals live, located while writing this brief

```
$ grep -rn "ffi_type" --include='*.hero' selfhost/ | head
selfhost/ffi_errors.hero:21   function ffi_type(name, what, span) -> diag.Diagnostic
selfhost/check/ffi.hero:108   refused(@c, s, decls, ty: written.must(),
                                what: "an `extern`'s parameter", span: …)

$ grep -rln "fixed_outside_a_group" --include='*.hero' selfhost/
selfhost/ffi_errors.hero
selfhost/check/ffi.hero
selfhost/check/ffi_sweep.hero
selfhost/check/decls.hero

$ wc -l selfhost/check/ffi.hero selfhost/check/ffi_sweep.hero \
        selfhost/ffi_errors.hero selfhost/check/lend_types.hero
     384 selfhost/check/ffi.hero
     121 selfhost/check/ffi_sweep.hero
     213 selfhost/ffi_errors.hero
     138 selfhost/check/lend_types.hero
```

`check/ffi.hero` is **384 lines**, already above `.claude/rules/module-shape.md`'s
~300 threshold. Confirm what `layout` says about it and whether a route 6 landing
raises a `DECIDED` row. Panel 164's engineer predicted route 3 at 90–150 selfhost
lines with **no zero-headroom file touched**; score that prediction if you can —
route 3 has shipped, so `git show --stat` on M-readable-bytes settles it.

## Things to price, each of them a question and not a claim

1. **Type identity on a fixed array.** Does the checker already compare two
   `i8[N]` types by extent, for the field case? If it does, route 6 is a
   widening; if extents are compared somewhere else or not at all, it is not.
   Name the function.
2. **The argument shape.** The shared brief measured that `i8[8]` cannot be a
   local binding (`fixed_outside_a_group`), so a route 6 argument is a record
   field. Does anything in the checker or the emitter assume a parameter's
   argument can be a local? What breaks if route 6 admits only a field?
3. **The emitter.** Route 3's repair at M-readable-bytes is instructive and is
   in `selfhost/emit/field_lend.hero` and `selfhost/emit/unread.hero`: the first
   implementation handed C the address of a **copy**, which read correctly and
   lost every write in silence at exit 0. Does route 6 pass by value, by address,
   or by decay — and which of those three does the current emitter already do for
   a field?
4. **A new refusal reaches further than its own file.**
   `.claude/rules/verification.md` records that a change to what the checker
   refuses invalidates programs anywhere in `tests/golden/**` and `examples/**`.
   Route 6 *widens* what is accepted rather than narrowing it, so say whether
   that rule bites here at all — and if it does not, say so plainly, because a
   seat asserting a gate it did not need is the shape this panel keeps finding.

## Build guidance

`cp -r` the tree to your scratchpad and `rm -rf target build` before you build.
The seed builds in about 3 s:

```
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
```

Rebuilding from `selfhost/` is ~20 minutes and will kill you on the watchdog.
**Never `archive/bootstrap-rs/`** — nothing builds it.

## What your verdict must carry

A verdict (approve / object / veto), the design.md sections it rests on, a **line
count** for the implementation with the files named, and **one falsifiable
prediction** with the milestone at which it becomes checkable.
