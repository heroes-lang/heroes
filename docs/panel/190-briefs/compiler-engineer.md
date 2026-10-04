# Panel 190, the compiler-engineer's brief

Read `00-shared.md` first; it binds you. You judge the ceiling and the cost
(your charter, `.claude/agents/compiler-engineer.md`), and you build.
Repaired after the critic's first pass (the text it read is
`compiler-engineer-before-the-critic.md`): the files a `return` passes
through were named wrong, and only one route was to be built.

## What to build and run, in `<scratchpad>/190-compiler-engineer/`

1. **Reproduce** the shared brief's table on your own build of `703af779`
   (lines of C and `hero_str_decref` calls at N = 100, 200, 400; the 800 row
   from `<scratchpad>/190-facts/sr800.c`), and read the path a `return` takes,
   each with its lines: `selfhost/ir/flatten.hero` (where a `return` and a
   `?` become terminators, `:136` to `:148` and `:470` to `:480`; 1,150 of its
   `DECIDED` 1,150), `selfhost/ir/own.hero` (its second walk appends the exit
   sweeps; rules 1 to 6; 288 of 300 in the instrument's unit),
   `selfhost/ir/emissions.hero:38` to `:43` (the copy-out, from four call
   sites), `selfhost/ir/verify.hero:215` (`check_copy_out`),
   `selfhost/ir/released.hero`, `selfhost/ir/layout.hero`, and the emitter's
   writing of an exit (`selfhost/emit/`). Settle each file's room with
   `layout` itself, never `wc -l`.
2. **Build the shared exit** in your copy, **and at least one other route**
   of the shared brief's list far enough to compare it on the same shapes
   (one sweep instruction expanded once is the nearest; a cleanup ladder the
   one design.md once promised); price the rest, each with what would make it
   wrong. The room is the ceilings': a route that needs more says what it
   moves along which seam.
3. **Q1's cases**, each a program built and run on each route you built, its
   output compared with the trunk's and run under `--sanitize` (ASan here);
   the shared brief lists the shapes, the critic's `--dump-ir` shapes in
   `<scratchpad>/190-critic/beside/` among them. Add any shape you find; say
   which break the trunk and which a route.
4. **Q3 and Q4**: the instruments the shared brief names, whole, with the
   harness's command, output to a file read whole; how many blessed
   emissions and `ir` goldens move and whether every move is the exit's
   shape alone (read them; never bless, and never edit an `ir` golden: list
   the edits a route would owe); the seed's own lines before and after; the
   shapes at 100 to 800; the frame of the 800 shape (`-fstack-usage`).

Report as the shared brief says, each route's diff saved as
`<scratchpad>/190-compiler-engineer/<route>.diff` and named in your report.
