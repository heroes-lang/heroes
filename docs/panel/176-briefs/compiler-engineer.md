# Panel 176 — compiler-engineer

Read `00-shared.md` first. Your seat judges implementation cost and
core-versus-sugar (design.md §1.1, §1.7, Part 5), with a veto on soundness.
Work in `<scratchpad>/176-compiler-engineer/`, built from the seed at `a747e5a2`.

## The pointers

- Route A as panel 175 fixed it is your own prototype:
  `<scratchpad>/175-compiler-engineer/prototype-A.diff` (pair entries in
  `runtime/parts/alloc.c`, the releaser passed from
  `selfhost/emit/handle_traffic.hero`'s `for_call` and the consumer from
  `before_call`). Panel 175's critic rebuilt it byte-identically as
  `<scratchpad>/175-completeness-critic/heroes-A` with `…/A/runtime`, and its T1,
  T3 and T4 variants beside it.
- The marks today: `selfhost/check/marks.hero` (the handle-only sweep),
  `selfhost/check/acquiring.hero` (`releaser_reads` `:194`, `unread_releaser`
  `:269`, the header comment at `:257` quoted in the shared brief),
  `selfhost/check/consuming.hero`, `selfhost/parse/members.hero` for the grammar
  of `Member` and `CParam`.
- Program-wide rules already exist: `one_tag_one_type`,
  `selfhost/check/decls.hero:334`, and `bindings_say_which`.
- The emitted C for a mark is `hero_handle_acquired(t, …)` after the call and
  `hero_handle_consumed(t, …)` before it; `HERO_RUNTIME_ABI` is 22 at
  `runtime/heroes_runtime.h:37`, and your panel 175 report measured what moves
  with it (227 blessed files in `tests/emission/`, 7 in `tests/golden/emit/`, the
  seed).

## What to measure

1. **Price V1, V2, V3 and R1** (shared brief) in compiler and runtime lines, on
   top of route A with a set-valued mark, and say what each does to the grammar,
   the formatter, `heroes grammar`, `check/marks.hero` and the ABI.
2. **Prototype the one you would land** far enough that the six programs of
   Question 1 give the right verdict: `popen_fopen` and `vk_cross` refused before
   C runs, `xfer_cj`, `xfer_jsonc` and `xfer_ssl` at 0, `refcount` at 0, and
   `getter_wrong_repaired` still caught before C. Say what each prints.
3. **Question 2 as a checker rule.** Price *one C name, one set of marks,
   program-wide*: which parameters and results are compared, how the `printf`
   two-arity fixture stays legal, and what the rule says when two modules name
   two different sets for one acquisition. Run it against `xmod-*/` and against
   every program in `tests/golden/` and `examples/`.
4. **Question 3 against your rule.** Whatever answers Question 2 must either
   leave one module per mode of `sqlite3_bind_text` legal or give the three
   modes another spelling that is true of each call. Price the spelling you
   would pick.
5. **Question 4.** Read how `owned` on an `@` cell is emitted and why the
   qualifier is dropped, and price the repair both ways: emit the header's own
   qualifier, or refuse the mark with an `ffi_` diagnostic at exit 1.
6. **The time.** If a route puts work on every call rather than only marked
   ones, time `./heroes check selfhost/main.hero` before and after,
   `/usr/bin/time -p`, three runs each, with nothing else running.

Give a verdict, the cost, one falsifiable prediction with the milestone at which
it is checkable, and the condition under which you would change your mind.
