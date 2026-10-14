# Panel 214, the compiler-engineer's brief

Read `00-shared.md` first. Your question is the ceiling and the soundness of each route in the
live compiler (`selfhost/`, `runtime/`, never `archive/`).

1. **Read the readers** named in `00-shared.md` § The evidence and say, with line numbers, what
   rule each one actually applies, and which of them a general rule (B, C, E, F or one you find)
   would delete, keep or rewrite. Price every route, A to K, in lines, by module, built or estimated
   (say which); D (isolation) and H (the runtime read after the groups) change the unit's shape, so
   say what they move in `selfhost/emit/` and in the emission corpus.
2. **The false blame, both ways, run**: find or write the shapes where a rule would tell a
   compiler bug as the author's header (a defect of the emitter whose clang error lands at, or
   whose note names, the program's header) and where it would tell an author's mistake as the
   compiler's. For each route, which of them it gets wrong. A rule that turns a real emitter defect
   into `ffi_header_refused` at exit 1 hides the compiler's bug from its author: weigh it.
3. **Build the route you adopt** in your copy, far enough that its cases run: the 125 cases marked
   `#~ ffi_header_refused` (`grep -rl` in `00-shared.md`) and the six reproducers with their
   before and after messages, red where they should be. If it deletes readers, show the cases still
   pass.
4. **The census**: the 878 tracked programs with an `extern` outside `docs/` (`00-shared.md` gives
   the command), BUILT with this tree's compiler and with yours, exit code and message compared;
   none may move for the worse. Bound each build in time (`timeout` on the binary, never on
   `heroes run`). And the other units that hold a group's header (`00-shared.md`): what your rule
   does there.

Veto on soundness: a route that can tell a wrong party at exit 0 or 1 where today's tells the right
one, measured.
