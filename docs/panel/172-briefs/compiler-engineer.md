# Panel 172 — brief for the compiler-engineer

Read `docs/panel/172-briefs/00-shared.md` first. You judge the mechanism, its
cost, and its soundness, and you hold a veto on soundness.

## Your axis

design.md §1.1, §1.7, Part 5; §1.12 (a Heroes program must not corrupt
memory); §4.19. The question you alone can settle by building: **what does each
route refuse and admit, at `check`, on the programs beside the brief and on the
corpus?**

## Build, in a copy

    cp -r /Users/joseph/Temp/heroes/heroes-lang <scratch>/compiler-engineer-172
    cd <scratch>/compiler-engineer-172 && rm -rf build
    clang -I runtime seed/heroes.c runtime/runtime.c -o heroes      # ~3 s

Then prototype **route A** far enough that the numbers are real, with this
compiler as the seed for your rebuild (`./heroes build selfhost/main.hero -o
heroes-next`, about a minute and a half):

1. `selfhost/check/marks.hero`: admit `consumes` on a by-value `cstr` or `ptr`
   PARAMETER of an `extern` function (not `borrows`, not `acquires`, not on a
   result, not on an `@` parameter); everything else stays `unread_mark`.
2. `selfhost/check/lend_landing.hero`: `Landing` carries `consumes`; a lend
   landing on a consuming parameter is refused with a new diagnostic. Decide
   and say WHERE in the order (header, extent, `lent`) the question sits, and
   whether a parameter may carry both `lent` and `consumes` (it cannot be
   true; is it `lent_shape`, or the new code?).
3. `selfhost/check/leasing.hero`: a lease cell's name as an argument of a call
   whose parameter at that position consumes is refused with the same code.
4. `tests/golden/check/unread-mark.hero:18,26` change verdict; say to what.
5. Run `./heroes-next test selfhost/main.hero` and the `check`, `annotations`,
   `run`, `layout`, `canonical` suites with `heroes-next` and report the counts
   and which goldens moved.

Measure **route B** too, without necessarily building it: with A built, count
how many of the 12 lease sites in the shared brief reach an unmarked parameter
today — that is B's refusal count — and how many of those parameters could
truthfully say a keeps-word.

## Questions you must answer

- **Is refusing a lend and a lease at a `consumes` parameter sufficient for
  soundness of the WORD?** In this language, which expressions of type `cstr`
  or `ptr` are Heroes-made? (`.cstr()`, `.ptr()`, a lease cell — anything
  else? `validated_bytes`? an `@` cell?) Every other value is C's, so what
  reaches a consuming parameter is C's to free. Write the enumeration, and
  attack it at the shapes beside it (CL-061): a name bound to a lend
  (`cstr_escapes` refuses it already?), a lease copied (`lease_escapes`?), a
  lend through a wrapper (`lend_needs_a_header`?), a `cstr` field of a group
  record, an `@out: cstr` C filled.
- **The double give-away.** A C-made pointer consumed twice is a double free
  in C. Is that this language's business (design.md §1.12) or C's? The handle
  live set (`runtime/parts/alloc.c`, `hero_handle_acquired/consumed`) keys on
  a handle's declaration; is there a sound analogue for raw pointers, and what
  would it cost? A measured answer or *not this sitting's*, in those words.
- **The residual under A.** A freer whose binding forgets `consumes` is 070
  unchanged. Is there a mechanism that catches it at run time — the lease block
  carries a header (`hero_held_release` reads it); can the runtime detect that
  C freed it? Panel 168 measured that a trailing header does not survive C's
  free; what about the leading one that ships? Run it or mark it unrun.
- **Route B's soundness and cost**: does refusing a lease at an unmarked
  parameter close the residual, and what does it break in `selfhost/` (the
  compiler's own bindings hand no lease today — verify with grep) and in the
  corpus?
- **Layout**: `lend_landing.hero` is at 239 by `wc` and `marks.hero` at 115;
  what does `suite_layout` read after your prototype?

## Report

`docs/panel/172-reports/compiler-engineer.md`: verdict · section · cost (code
lines, files, `check selfhost/main.hero` NOT timed — the machine is shared) ·
one falsifiable prediction naming an instrument that exists · veto condition if
any · what you built and what moved, with the commands.
