# Panel 230, the compiler-engineer's brief

Read `00-shared.md` beside this file first; it is the proposal and the facts, each with its command,
and its last section says what the critic's first pass changed. You judge the ceiling (design.md
§1.1, §1.7, Part 5): what each route costs in `selfhost/` and `runtime/`, core against sugar, and
whether it can be made to crash. You hold a veto.

**Where you work**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/230-compiler-engineer`,
detached at `595d92d88`. Build its compiler there, `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes` (about 8 s) and then `./heroes build selfhost/main.hero -o heroes-next` (about 86 s); judge
with `heroes-next`. Prototype in that worktree, never in the lane. Scratch:
`.claude/worktrees/230-compiler-engineer-scratch/`. Your report is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panic-location/docs/panel/230-reports/compiler-engineer.md`,
**written as you go, from your first finding**: the critic's first pass wrote nothing until the end,
was stopped by a rate limit, and would have lost everything. No file outside the repository's root,
no credential printed, no paid run. **Start no clock**: build what is to be timed and write the exact
commands in your report; the coordinator runs them on a machine the other two sessions hold still.

**What you are asked, each answered with the command that settles it:**

1. **The census of the runtime's aborts in three kinds** (`00-shared.md` § Where an abort is
   written): which a program's own operation reaches through a runtime call (kind 2), by which entry
   point; which are raised where no call of the program stands (kind 3: the fault handlers, the exit
   check, the lease handlers, `lend.c`'s past-the-buffer report); which are invariants no correct
   compiler reaches. Include the 10 `panic:` lines outside the `hero_abort()` census, and say which
   of the 59 `hero_unreachable` emitter lines a program's own operation can reach.
2. **The place record**: where the emitter has a site's line, column and function when it writes the
   call (every `Inst` has a `span`; `writer.at_generated`; `ir.Run`); the column convention of
   `00-shared.md` (the start of the expression whose evaluation stopped the program); the file named
   by the program's module paths, not the invocation path (finding 6), and what a module of the
   library (`<heroes library>`) is called; the function in a generic instance, a `test` block, a
   function value through a parameter, and `main`.
3. **Build the route you recommend**, in your worktree, far enough that the numbers are real: the
   place record for the emitter-side sites (index in its three forms, `array_read.hero:37`, `:62`,
   `:65`; overflow; division; `.must()`; `assert`), one kind-2 class by L1a, L1b or L1c, whichever you
   recommend, and **what you recommend for kind 3** (L4's `hero_stack_names`, L5, or a narrower
   sentence), each built far enough to print on the programs of
   `.claude/worktrees/panic-location-scratch/before/` (copy them) and on a thread that indexes past
   an end (`runtime/parts/spawn.c`).
4. **The cost, prepared for the coordinator's clock**: two compilers, before and after your route,
   each built from the same tree but for the route; the commands that time `heroes-next build
   selfhost/main.hero --emit-c -o /dev/null` and the whole build with each; the binary's text and
   read-only data (`size`); and **`seed/heroes.c`'s bytes** before and after (the after one by
   emitting the seed with your compiler), against M-bounded-seed's 10 MB, and what that profile
   (which drops `#line`s) should do with the places. Say what L1a's argument costs on
   `hero_array_at_mut` (`:62`), the form taken on every impure indexed write.
5. **S1's renderer**: where it lives (`emit/structural.hero` and its ceiling,
   `tests/harness/suite_layout.hero`'s `DECIDED`), its code lines; whether ordering map entries by
   their rendered text, with a tie-break you name, answers panel 117's map clause (the two maps of
   `00-shared.md`) without a key order on records; whether the depth bound closes panel 076's frame
   cliff; `0.0` and `-0.0` as keys, `nan`; that it never reads through a `ptr`, a `cstr` or a handle;
   **its notation for a `str`** (`\n`, `\u{1b}`: the spec's escapes, as the lldb formatter writes
   them, against the failure printer's `<U+001B>`), which defect 2225's repair needs for `.must()`'s
   line too; how it and `editors/lldb/heroes_lldb.py` are held to one notation (one corpus of values
   both render, compared by a test, or one generated from the other); and what
   `tests/harness/suite_special.hero:308-317` becomes. Build it far enough to print `rects`' two
   sides (`.claude/worktrees/panic-location-scratch/blind230/items/rects.hero`) and an array of
   records.
6. **M1 and its neighbours**: what `main -> ()?` costs in the checker (`check/decls.hero:96-100`)
   and in the emitted `main` (`emit/c_main.hero`); `?` leaving `main`; what § 13's exit check does
   when `main` returns a `fail` with a lease live (measure both orders); M5's built-in (`hero_write_err`
   exists, `runtime/hero_os.h:108`); M7's obligation (can the checker tell a `main` whose `.err` arm
   leaves normally, without false alarms on a program that handled the error on purpose?); and
   `exit(code:)`'s range (defect 2224): a constant outside 0 to 255 refused at compile time, an
   abort otherwise. Build M1 far enough to run.
7. **The landing**: the seed's route for an ABI step (`abi-put-back`), what enters `selfhost/` only
   after the refresh whose seed knows it, and the order of the commits; the goldens owed (a
   `main_returns` case, none exists today; a `fixedbugs` case per abort class; defects 2224 and 2225).
8. **`-O2`**: the items built at `-O2` (`heroes run`'s level) under L1 and under L4: does inlining
   change the function either names?

**Your output**: `verdict` (approve | object | veto) per route, with the design.md section you stand
on; `cost` (code lines per module, the runtime's lines, the ABI step, the seed's bytes);
`prediction` (falsifiable, checkable at a named milestone); `condition` (what would change your
verdict); and every number with its command.
