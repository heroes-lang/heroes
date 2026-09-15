# Panel 150 — brief for the compiler-engineer

Read `docs/panel/150-briefs/00-shared.md` first, including its three required
sittings. Do not repeat a measurement already there; extend it.

Your seat: design.md §1.1, §1.7, Part 5. Implementation cost and core-versus-sugar.
You have a veto on soundness.

## What to read

- `selfhost/check/acquiring.hero` — the rule as it now stands, both sides.
- `selfhost/check/reaches.hero` — the walk, its visited set, its termination
  argument in two halves. New at panel 149.
- `selfhost/handles.hero` — `is_handle`, and the counts `handles_handed_over`
  and `handles_taken_back` that replaced two booleans.
- `selfhost/check/ffi.hero` — `crosses_the_boundary`, `ffi_field`.
- `selfhost/emit/ops.hero:158-180` — one increment per MARK since panel 149.
- `runtime/parts/alloc.c` — the counter and its two-cause message.

## Q1 — when must a `ptr` producer be marked

1. **Price the type-keyed rule**, mirroring the handle rule: where a group
   consumes a `ptr`, every `extern` handing a `ptr` back says which it is. How
   many lines on top of what ships? The shared brief measured the blast radius at
   one file; verify that yourself.
2. **The hard part, and say it plainly: a `ptr` has no identity.** Two handle
   types are two Heroes types and one C type, and `one_tag_one_type` refuses
   that. Every `ptr` in a program is the SAME Heroes type, so a group that
   consumes one `ptr` would demand a mark on every `ptr` producer in that group,
   however unrelated. Is that a rule or a nuisance? Write the binding where it is
   a nuisance and compile it.
3. **What happens to a `ptr` inside a record or a fixed array** — does the walk
   in `check/reaches.hero` need to see `ptr` at all, and what does that cost?
4. **The alternative nobody has priced**: make the demand PER GROUP rather than
   per program, or per header. Panel 148's compiler seat priced a group-keyed
   rule at ~90 lines over a key the language does not define, and said why. Does
   that argument still hold now that `reaches.hero` exists?

## Q2 — the per-element release

5. **Price the two routes panel 149's critic named**, and say which the compiler
   can actually check without flow analysis, which `check/leasing.hero` and
   `check/consuming.hero` both state it does not have:
   - refuse the element release — what is the check, and what does it rest on?
   - let the mark say WHOLE versus PER-ELEMENT — parser, AST, checker, emitter.
6. **The third route, which you should test before dismissing**: leave the count
   alone and fix only the MESSAGE, adding this shape as a third cause. What does
   that cost, and what does it fail to catch?
7. **Whatever you propose, say what it does on the CONSUMER's side.** That is the
   half panel 149 nearly missed.

## Predict something falsifiable

One prediction, with the milestone and the command that would settle it.
