# Panel 163 — compiler-engineer

Read `00-shared.md` first. This file is your input only. You have a veto on
soundness.

## What you are asked, and it is unusual

**Price each route to IMPLEMENT, in the files it lands in, and say which one you
would write.** Panel 162's resolution was corrected three times at
implementation — it called a library function a built-in, it assumed a name
could be reserved that `ir/owned_release.hero` depends on, and it priced this
half as a widening of `repeat` that cannot be written. Your seat is the one that
can stop a fourth.

## The four questions, in order of what they decide

1. **Route 1, a call typed by context.** `selfhost/check/walk.hero`'s `check`
   matches on expression kind against an `expected` type; count the sites and
   say what a `.call`/`.method` arm costs there. Then the other half:
   `selfhost/emit/storageless.hero` has one producer of a braced literal and an
   arm that refuses `.call` as *not a producer of a fixed value*. What does a
   third producer cost, and **does the lowering even keep the call**, or does it
   evaluate it into a value that has no storage?

2. **Route 2, a zero default.** design.md §4.9 says no default values.
   `selfhost/emit/assert_spelling.hero:170`'s `zero_of` already emits
   `(Color){0}` — panel 162's compiler seat reported that and called
   `selfhost/check/walk.hero:616` the single wall. **Verify that yourself**; if
   the mechanism is already there, this route is a checker rule and not an
   emitter change, and that would make it the cheapest sound route rather than
   the cheapest unsound one.

3. **Route 3, an uninitialised `@` out-parameter.** `spec § 5` says all bindings
   are initialised. What breaks if one `@` argument is exempt — name the hole,
   because the reason that rule exists is that a read of an unassigned slot is a
   LOUD panic (panel 021's zero-initialiser) and an exemption may make it quiet.

4. **The route nobody listed.** Consider in particular whether a header record
   with a long array field could be obtained from a **function the group
   declares**, so nothing in the language changes at all.

## How to work

`cp -r` to your scratchpad, `rm -rf build`, seed with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. **Never rebuild
from `selfhost/`.** **Never `archive/bootstrap-rs/`.** Every number from a
command you name.

## Deliver

Verdict · the design.md section it rests on · the implementation cost per route
with the files · a falsifiable prediction with its milestone · any veto as a
refusal rather than a price.
