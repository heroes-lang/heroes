# Panel 169 — brief for the compiler-engineer

Read `docs/panel/169-briefs/00-shared.md` first. It carries the question, the
four routes, the measured state and four things a verdict must not assume.

## Your axis

The ceiling and the mechanism: design.md §1.1, §1.7, Part 5. What does each route
cost in the live compiler, and which of them can be built at all?

## Where the machinery already is, read rather than remembered

- `selfhost/check/marks.hero` — **115 lines**. `marks_are_read` at `:39`,
  `refuse_unread` at `:90`, `unread_mark` at `:103`. This is what refuses
  `borrows` on a `cstr` today, and its note tells the author to use a handle.
- `selfhost/check/acquiring.hero` — **278 lines**. `consumed_types` `:84`,
  `handle_behind` `:106`, `taker_of` `:130`, `bindings_say_which` `:146`,
  `releaser_reads` `:194`. This is the pass that makes `acquires`/`consumes`
  work.
- `selfhost/handles.hero` — **223 lines**.
- `runtime/parts/alloc.c:368-430` — `hero_handle_slot`, `hero_handle_acquired`,
  `hero_handle_consumed`: open addressing, linear probing, the live set that
  aborts at exit with a count and an address.
- `selfhost/check/lending.hero` — **386 lines** by `wc -l`. `argument_of` `:118`,
  `keeps_its_argument` `:177`, `lent_only_into_c` `:194`, `lends_a_place` `:285`,
  `rooted_at_a_name` `:293`. R2's machinery is here.
- `selfhost/emit/inst.hero` — the 069 gate, `.extern_fn | .builtin_fn |
  .indirect => hero_unreachable(); /* the gate refuses this form */`, and the
  call emitted four lines later regardless.

## The ratchets are yours and they must be RE-measured

Panel 168 measured **`selfhost/ir.hero` 310 of 310, `emit/inst.hero` 350 of 350,
`emit/ctype.hero` 395 of 395 — zero headroom on three** — with an awk replica of
`suite_layout.hero:526-560` validated against four independent anchors. **Do not
carry those.** Re-measure with the instrument, and note that panel 167 named
three *different* files a day earlier.

**And read § 4 of the shared brief before you use a ceiling in a verdict.** A
ceiling is CLAUDE.md § 11, rank 6; a route that closes a memory-corruption class
is rank 3, and CL-012 names compiler size among what robustness beats. If a route
needs a file split, **price the split** — that is what
`.claude/rules/module-shape.md` prescribes — rather than refusing the route.

## The questions

1. **R1, the handle route.** Can a lend or a lease be made to reach the handle
   live set, or must the binding author declare a handle instead? What does the
   compiler need in order to refuse a lend at a parameter that retains — is it a
   mark, an inference, or nothing it can have? Price it in the files above.
2. **R2, the caller-side rule for 068.** *A binding whose field's address has
   been lent in this function is not re-assigned in this function.* Build it far
   enough to price it, **in your copy**, and say what it costs, what it
   over-refuses, and whether `check/lending.hero`'s existing parent map is
   enough. Then run `tests/golden/` against it and say how many programs it
   breaks. **This is the question most likely to close a defect this milestone,
   so spend your time here.**
3. **069's repair.** The gate at `emit/inst.hero` refuses a form and then emits
   the call anyway. Is the repair a checker diagnostic, an emitter refusal at
   exit 2, or the form being supported? Say which, and price it.
4. **Is there a route nobody listed?** Two sittings have now missed one that was
   in the language. Ask what would have to be true for a fifth to exist.
5. **Register a falsifiable prediction** with an instrument that exists today.

## Working rules

- `cp -r` the tree to your scratchpad, then `rm -rf target build` in the copy.
- A compiler in 3.80 s:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. **Never rebuild
  from `selfhost/`** — twenty minutes, and the watchdog kills you.
- Never `archive/bootstrap-rs/`.
- Report to `docs/panel/169-reports/compiler-engineer.md`. Veto on soundness.
