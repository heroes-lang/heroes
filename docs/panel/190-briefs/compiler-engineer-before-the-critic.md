# Panel 190, the compiler-engineer's brief

Read `00-shared.md` first; it binds you. You judge the ceiling and the cost
(your charter, `.claude/agents/compiler-engineer.md`), and you build.

## What to build and run, in `<scratchpad>/190-compiler-engineer/`

1. **Reproduce** the shared brief's table on your own build of `703af779`
   (lines of C and `hero_str_decref` calls at N = 100, 200, 400; at 800 if it
   finishes), and read where the product comes from: `selfhost/ir/own.hero`
   (its second walk appends the exit sweeps, its rules 1 to 6),
   `selfhost/ir/lower.hero` (where a `return` becomes a terminator),
   `selfhost/ir/released.hero`, `selfhost/ir/layout.hero` and the emitter's
   writing of an exit edge (`selfhost/emit/`). Name each with its lines.
2. **Build the shared exit** in your copy, the route the proposal names, and
   price the others Q2 lists (a liveness-pruned sweep, a C-only cleanup
   label) far enough to say what each costs and what would make it wrong.
3. **Q1's cases**, each a program built and run on your route, its output
   compared with the trunk's and run under `--sanitize` (ASan here): an early
   return of a slot's value; an `@` parameter written then returned early; a
   return inside a loop, inside a `match` arm, inside an `if` used as a value;
   a `.must()` and a `?` on a return path; a slot written on one path and not
   another (panel 106's frame); a function whose every path returns; a
   function returning nothing; a generic function and its instances. Add any
   shape you find; say which break the trunk and which your route.
4. **Q3 and Q4**: the compiler's own tests; `check`, `run`, `emission`,
   `determinism`, `wholes`, `descriptors` and `lines` whole (with the
   harness's command, output to a file read whole); how many blessed
   emissions in `tests/emission/` move and whether every move is the exit's
   shape alone (read them; never bless); the seed's own lines before and
   after; the shapes at 100 to 800.

Report as the shared brief says, with your route's diff saved as
`<scratchpad>/190-compiler-engineer/route.diff` and named in your report.
