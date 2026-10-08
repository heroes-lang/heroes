# Panel 199, the shared brief: a function whose every path calls itself

Written by the coordinator on 2026-10-07 from 23:43 by the clock, on the tree
frozen at `56def9b4` (worktree `lane-panel-199`). **A full panel**: a new
diagnostic is what the checker DOES (CLAUDE.md § 4): the compiler-engineer,
the spec-warden, the historian, the blind seat as fresh `claude -p` sessions
outside the repository (paid, **6 USD in all, approved by the author at about
23:4x on 2026-10-07**), and the completeness critic before the seats and
after. Convened for defect 457 (`systemic`), which blocks the milestone's tag.

## The question

Defect 457 (`issues/2026-10/07/2026-10-07-1837-defect-457-*.md`): a function
whose every path calls itself, `function forever(n: i64) -> i64` returning
`forever(n)`, passes `check` at exit 0; `build` prints clang's *warning: all
paths through this function will call itself [-Winfinite-recursion]* **over the
generated C** (`HeroArrayHeader * h_shadow_bytes(HeroStr h0_s) {` in the
shape the coordinator ran, `<scratchpad>/batch14/shadow/`, before 16:48), and
the run aborts *panic: stack exhausted*, exit 134. The shape an author meets:
a function of the program's own named like a built-in method whose body calls
that method by UFCS, `function bytes(s: str) -> [u8]` returning `s.bytes()`
(defect 455's symptom; `x.f(y)` is `f(x, y)`, spec `:267`). **What does the
compiler tell a function whose every path calls itself, and where?**

Read in the tree: the spec says *Recursion too deep aborts* (`spec/heroes-spec.md:280`)
and nothing of a function calling itself unconditionally (`grep -n -i recurs`,
three lines); **whether Heroes has warnings, or only errors, is for the seats
to establish by grep** (the coordinator found no warning class in the spec,
`grep -n -i warning spec/heroes-spec.md` empty, a negative to verify).

## The routes, a list to be widened

- **(A)** a checker rule: every path of a function's body reaches a call to
  the function itself with no path that returns or aborts first: an error
  naming the call and the function, with what to write instead;
- **(B)** the rule on the IR after lowering (the control-flow graph is there),
  told in the compiler's words;
- **(C)** clang's `-Winfinite-recursion` read by the build and told in the
  compiler's words at its `.hero` line;
- **(D)** clang's warning silenced, the run's abort the only witness;
- **(E)** the UFCS shape alone: a function named like a built-in method that
  calls that method on its first parameter told as the shadowing it is.

## The rules every seat works under

- **Your own copy**: `cp -R` the frozen tree to
  `<scratchpad>/199-<seat>/tree` (the scratchpad is
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`),
  `rm -rf build` in the copy, your compiler from its seed. Never build, run
  or write in the frozen tree, the trunk or any `lane-*` worktree.
- Counts only, never durations (a dozen lanes run). No paid run but the blind
  seat's, which the coordinator runs. Times only from `date`. Your final reply
  is the record; the coordinator copies it into `docs/panel/199-reports/<seat>.md`.
