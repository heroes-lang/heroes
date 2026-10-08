# Panel 199, historian

Read `00-shared.md` first, whole. You have web search: every external
precedent cited with its source and date, or marked unverified. Your running
notes go to `<scratchpad>/199-historian/report.md` as you go. Repaired
2026-10-08 on the critic's first pass; the version it read is
`first-pass/historian.md`.

1. **How other compilers tell a function whose every path calls itself**:
   rustc's `unconditional_recursion` (warn by default? since which version?),
   Swift's *all paths through this function will call itself*, clang's and
   GCC's `-Winfinite-recursion` (GCC since 12?), Go (`go vet`: a check or
   not?; staticcheck's SA5007, *infinite recursive call*, named by the critic
   and unverified), Kotlin, C#, Java (javac?), Haskell (GHC: no?), Zig. Error
   or warning, on by default or not, and what each misses (mutual recursion,
   generics, calls through a value, a path ended by a call that never
   returns, an abort the program does not write).
2. **Any language that made it an ERROR, and why; any that removed it, and
   why.** And whether any compiler states that a self-call in tail position
   is turned into a jump at some optimisation level, so that an unbounded
   recursion stops aborting and runs for ever, and what it says about it.
3. **The UFCS shape**, restated: not a function named like a built-in (here a
   built-in name is already refused, `builtin_name_taken`), but a free
   function whose own body calls itself through method syntax on a name the
   author believes is a method (`function bytes(s: str) -> [u8]` returning
   `s.bytes()`). D's UFCS, Nim's method call syntax, any other: known
   pitfalls, documented.
4. **This repository's own precedents**, to read whole in the frozen tree
   `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-panel-199`
   (read only):
   - `docs/panel/020-the-c-emitter.md`: why a report that is not an error is a
     kind of diagnostic and not a parallel type, and GCC's `sorry()`;
     `selfhost/diag.hero:17-27` holds the result, two kinds and no warning;
   - `docs/panel/070-the-stack-nobody-was-counting.md` (2026-08-16):
     *it depends on the optimisation level* scored falsified for its shape
     (`:141-144`);
   - `docs/panel/097-the-platform-arm-belongs-to-the-runtime.md`: named by
     the critic; its text holds no line on the stack, recursion, a guard,
     overflow or `-O2` (`grep -c -i 'guard\|overflow\|stack\|recurs\|-O2\|sibling\|tail'`,
     0), so say whether and why it bears;
   - `docs/panel/104-deep-recursion-stops-with-a-word.md` (2026-09-03): the
     guard-page handler that prints *stack exhausted*, and a depth counter
     rejected, partly because it blocks `-O2`'s recursion-to-loop
     transformation (`:64-68`);
   - `docs/panel/184-a-brace-is-written-both-ways-a-statement-after-a-jump-is-refused-and-depth-is-the-compilers-to-hold.md`
     and `docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md`:
     the written path ends, `exit(code:)`, `assert false`, a `while true`
     with no `break`, and why they are read by syntax and never by value;
   - `docs/design.md:3725` (Part 8): *this language has no warning level*.

Advisory, no veto. Write what was searched where you found nothing.
