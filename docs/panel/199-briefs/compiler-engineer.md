# Panel 199, compiler-engineer

Read `00-shared.md` first, whole. Your directory
`<scratchpad>/199-compiler-engineer/`, your copy `tree/` inside it with its
compiler from its seed, your running notes `report.md` beside it. Every probe
that may not end is bounded as `00-shared.md` says: built, the binary under
`timeout 10`, output capped, never `timeout heroes run`. Repaired 2026-10-08 on
the critic's first pass; the version it read is `first-pass/compiler-engineer.md`.

1. **Where the rule can live.**
   - **The checker.** The every-path logic is `selfhost/check/path_end.hero`
     (267 lines; panels 184 R4, 185 R4 and R5), read through `Outcome.leaves`
     (`selfhost/check/walk.hero:98-99`, `record Outcome` and its `leaves:
     join.Leaves`), the one predicate that decides a function's end
     (`missing_return`), a value arm and a value block. `check/flow.hero`,
     which the first version named, is panel 177's lattice of ended places
     (238 lines, its head comment), not this.
   - **The IR.** `selfhost/ir/graph.hero` (171 lines), `ir/dominators.hero`
     (455), `ir/callees.hero` (76).
   - **A recursion rule `check` does not see has a precedent**:
     `polymorphic_recursion` (`selfhost/ir/mono.hero:374`), `check` 0 and
     `build` 1 on `00-shared.md`'s `polyrec.hero`. Say whether this rule should
     be seen by `check`, and what it costs either way.
2. **What ends a path.** Three written ends: `exit(code:)`, `assert false`, a
   `while true` with no `break` of its own (spec `:238-240`), which
   `path_end.hero` reads by syntax and never by value (its head comment). No
   `abort` or `panic` statement exists: the first version's *an
   `abort`/`panic` path* named nothing. The aborts nobody writes are an
   overflow check (p2's `n - 1` is `if (__builtin_sub_overflow(...))
   hero_panic_overflow();`, `_Noreturn` at `runtime/heroes_runtime.h:61`), an
   index out of bounds, a `.must()`. **Does a hidden abort end a path for the
   rule** (question 2)? Answer it with both readings' cases: p2,
   `docs/panel/173-briefs/so_lease.hero:4-5` (`return down(n: n + 1) + 1`) and
   `docs/panel/173-briefs/q5_overflow.hero:3-5` (`return a[0] + down(n: n + 1)`).
3. **The shapes, each a case annotated** in your copy: a direct self-call in
   every arm of a `match`; a self-call behind `if` with no `else` (a path
   returns); a self-call through UFCS (p1, p3); a generic function calling its
   own instance (`gen.hero`); a call through a function value (`fnvalue.hero`);
   mutual recursion (`pingpong.hero`); a self-call after `?` (a path returns
   early); a `while true` loop that calls itself; **and the ones the first
   version missed**: `serve` (`exit` ends a path, so it is NOT refused); the
   right side of `&&` and of `||` (short-circuit: a path that never calls); a
   `for` body and a `while <cond>` body (a path that skips it); a self-call
   inside a condition (`if f(n) > 0`); a same-named function of another
   module (`m.f(x)` inside `function f` is not a self-call); a variant case
   named like the function (`.f(...)`); an `@` first parameter (UFCS does not
   apply, spec `:272-273`); a `test` block (no function to call itself).
4. **The probes, at both levels**: p1, p2, p3 (`<scratchpad>/199-blind/src/`),
   and `forever`, `serve`, `pingpong`, `fnvalue`, `gen` from
   `<scratchpad>/199-briefs-work/probes/` (read them, copy them into your own
   directory, run them there); `00-shared.md`'s table is what they gave on the
   frozen compiler.
5. **Build (A) or (B)** in your copy, or (H), or the route you find more
   robust: its cases, the compiler's own tests, `check` narrowed to the
   cases, and the census of `check` over every tracked `.hero` file, 2,980 at
   `56def9b4` (`git ls-tree -r --name-only 56def9b4 | grep -c '\.hero$'`),
   frozen compiler against prototype, every hit named and read (no correct
   program refused). The critic's textual approximation, carried and not
   re-run, is in `00-shared.md` question 8. And the compiler's own `check`'s
   cost before and after, in instructions.
6. **The prototype's output is the blind seat's variant B.** Run it on p1,
   p2 and p3 in your copy (`check`, then `build` and the bounded binary where
   `check` passes) and put each transcript in your report verbatim, in the
   shape of `<scratchpad>/199-blind/src/p1-A.txt`: the diagnostic in the
   compiler's own format, no `certain` fix. If p2 is not refused, say so; if
   you prototype both readings of question 2, give both transcripts.
7. **(C), (F), (G).** The `.hero` line already reaches clang's warning
   (`00-shared.md`); what (C) adds is the compiler's words and an exit code,
   and the ruling against warnings says which. Clang versions in the records:
   18.1.3 (Ubuntu, the CI's Linux x86-64), 21.0.0 (this Mac, the only one run
   here), 22.1.8 (Debian, Linux arm64), 23.1.1 (the Windows box,
   `docs/platforms/windows/WINDOWS-MACHINE.md`); whether each has
   `-Winfinite-recursion` is unrun, say what you can run. (F): what marking
   `exit` never-returning in `selfhost/emit/` takes, and whether `serve`'s
   warning goes (defect 507). (G): what `-fno-optimize-sibling-calls` or its
   equal costs, whether `forever` and p3 abort 134 at `-O2` with it (defect
   508), and what it does to a correct deep tail recursion.
8. **The diagnostic's class**: a thesis rule (`selfhost/diag.hero:141-165`,
   dropped by `check --permissive`) or a rule without which the program has no
   meaning; its fix `certain` or `guess`.

Verdict: the route, its cost, the other routes' costs. Veto on soundness only.
