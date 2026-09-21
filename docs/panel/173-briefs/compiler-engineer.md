# Panel 173 — brief for the compiler-engineer

Read `docs/panel/173-briefs/00-shared.md` first. You judge the mechanism's
soundness and its cost, and you hold a veto on soundness. design.md §1.12 (a
Heroes program must not corrupt memory; here, must not LIE about why it died),
§4.17 (a diagnostic carries what is needed to fix the program), §1.1 (the
ceiling).

## Build

Fresh copy as the shared brief says; `git apply docs/panel/173-briefs/prototype.diff`;
rebuild; confirm the ten-run table on `lease070.hero` and the control before
anything else. Foreground only, generous timeouts, no polling loops.

## Questions you must answer, each with a command

1. **The panic path, and its sound shape.** Reproduce `panic_lease.hero`'s two
   lines. Then decide between: (a) a flag set at every runtime-initiated abort
   — count the sites (`grep -n 'abort()' runtime/parts/*.c`), say whether they
   already share one function or need one; (b) a handler that infers the
   origin from `siginfo_t` (`si_code`, `si_pid`: an `abort()` from this process
   against a SIGTRAP from the allocator's `__builtin_trap`); (c) something you
   find. Build the one you judge sound, re-run `panic_lease.hero`, `lease070`,
   `control`, `b_later`, `b_callback`, and a stack-overflow program with a live
   lease (a recursion that never returns, after `c: cstr @ x.lease()`), and
   report one table. Every runtime `abort()` you did not make a case for is a
   shape UNRUN, in those words.
2. **Composition with `stack.c`'s handler.** Read `hero_stack_handler`
   (`runtime/parts/stack.c`, around lines 280-330) to its last line: does it
   re-raise the fault or `abort()`? Install order: the prototype calls its
   installer after `hero_stack_guard_install()` in `os.c:140`; does either
   overwrite the other's disposition? Both use `SA_ONSTACK`: is the alternate
   stack `stack.c` maps big enough for both handlers' frames, and is it per
   thread (the comment at `stack.c:578-581` says the disposition is the
   process's and the stack the thread's)? The prototype saves the previous
   dispositions and never calls them: say what a correct handler owes them
   (chain, or restore-and-reraise as it does), and whether restore-and-reraise
   loses anything the previous handler would have said — including under
   `--sanitize`, where ASan installs its own SIGABRT handling.
3. **What the report can name.** From inside the handler, with `ucontext_t`
   in hand, can `hero_stack_blame` (or the walk it uses) name the Heroes
   function that was executing the C call? Try it in the copy; report the line
   the message would gain and the cost. If the runtime keeps no record of which
   lease is which, say so and say what one would cost (a name per lease is a
   pointer per `.lease()`).
4. **Cost.** `git diff --numstat runtime/` after your version; the runtime's
   size (`wc -l runtime/parts/os.c runtime/parts/alloc.c` before and after);
   whether `suite_layout` reads `runtime/` at all (`grep -n 'runtime' tests/harness/suite_layout.hero`);
   two `sigaction` calls at process start, stated as what they are and not
   timed.
5. **The goldens.** Write `tests/golden/run/` cases for `lease070`,
   `b_callback`, `b_later` (report), `b_out` (silent) and `panic_lease` (one
   line) in your copy, in the shape `tests/golden/run/abort-lease-never-ended.hero`
   and `.expected` use, and run the `run` suite with your rebuilt compiler.
   Report how the suite judges the exit code of an aborting case (read
   `tests/harness/suite_golden.hero`'s run form), because ours is 133 on the
   interior-free path and 134 on `abort()`.
6. **The seed.** Confirm with `cmp` that `seed/heroes.c` is unchanged by a
   runtime edit, or say what changed.

## Report

`docs/panel/173-reports/compiler-engineer.md`: verdict · section · cost · one
falsifiable prediction naming an instrument that exists (`tests/golden/run/`,
the `run` suite, the Linux leg of `.github/workflows/ci.yml`) · veto condition
if any · what you built and every table, with the commands. English only.
About 25 minutes of work; mark the rest UNRUN in those words.
