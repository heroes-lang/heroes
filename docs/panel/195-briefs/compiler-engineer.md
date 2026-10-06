# Panel 195, compiler-engineer

Read `00-shared.md` first, then defect 382's issue file whole, then
`selfhost/ir/lower.hero` around line 77, `selfhost/emit/container.hero`'s
`build_array` and `selfhost/emit/construct.hero` in your copy.

Your seat judges cost and core against sugar (design.md §1.1, §1.7, Part 5) and
holds a veto on soundness (design.md §1.12).

1. **Build the route you judge most robust, in your copy, far enough that its
   numbers are real**: the emitted C of a constant's block, the lowering of a
   read, and the runtime lines it needs. Run the reproducer
   (`<scratchpad>/p195/k.hero`, and a `[str]` twin you write) and report
   instructions retired at `-O0` and `-O2`, against the landed route and the
   floor in `00-shared.md`.
2. **Rule the trade in `hero_array_decref`**: a guard that returns on a
   negative count silences a count driven negative by a release too many,
   which today reaches the drop list. Find every way a count goes negative
   today (`grep` the runtime and the emitter), say whether any program reaches
   one, and say what keeps a double release loud under the guard (an assert
   on a count below -1? a magic in the header, as strings carry? a separate
   flag?). A route that makes a memory error silent is refused by this seat's
   own veto unless it says what catches it instead.
3. **The shapes**, each built or refused with its reason: a constant of `[i64]`,
   `[str]`, `[[i64]]`, `[Record]` holding a `str`, `[Variant]`, a `{str: i64}`
   map, an empty `[]`, a constant whose body names another constant, a
   constant read by threads made with `hero_thread_spawn`, a constant of
   functions (`callback_guard.hero`'s guard), a map with a computed value, an
   `f"..."` element, and an index that aborts.
4. **The ABI**: does the route require a runtime that skips a static array
   block, and so a move of `HERO_RUNTIME_ABI` from 27? Name every word it
   adds to the runtime's headers and say whether `emit/guarded_names.hero`
   and the guard files must carry it (defect 361).
5. **Cost to the compiler**: the compiler's own `--emit-c` as instructions, on
   this branch and with your route, and the layout of every file you touch in
   the instrument's unit (`tests/harness/suite_layout.hero`'s `code_lines`).

Verdict: approve, object or veto per route, with the section, the cost, a
falsifiable prediction and its condition. Your report:
`<scratchpad>/p195/reports/compiler-engineer.md`, written as you go.
