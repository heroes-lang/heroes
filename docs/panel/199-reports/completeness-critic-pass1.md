# Panel 199, completeness critic, first pass: the briefs before any seat runs

Copied by the coordinator on 2026-10-08 from the critic's reply (a subagent's
Write of a report file is refused). Run from 01:44 to 02:03 CEST on 2026-10-08
(`date`), on the frozen tree `lane-panel-199` at `56def9b4`, everything run in
its own copy `<scratchpad>/199-critic/tree` with a compiler built from the seed
(Apple clang 21.0.0); probes in `<scratchpad>/199-critic/w/`, every one that
might not end built first and its binary run under `timeout` 5 or 10 s, output
cut by `head -c`; `pgrep` after found nothing. No paid run.

## Four findings that change what the sitting is about

1. **`heroes run` builds at `-O2` by default, and at `-O2` the shape hangs with
   no message.** Levels in `selfhost/cli/verbs.hero`: build `-O0` (`:36`), run
   `-O2` (`:61`), test `-O0` (`:110`). At `-O2`, `forever` is `b
   _h_forever_forever` and runs until `timeout`: exit 124, nothing printed. The
   same for mutual recursion `ping`/`pong`, a self-call through a function
   value, the exit-path shape, and a UFCS wrapper `function count(xs: [i64]) ->
   i64` returning `xs.count()`. Only p1 and p2 still abort 134 at `-O2`. So
   spec `:280` (*Recursion too deep aborts*) is false at the default level of
   the command a model uses, and route (D)'s witness does not exist there.
2. **Clang's warning can be false on a correct program**:

   ```
   function serve(n: i64) -> i64
       if n > 3
           exit(code: 0)
       print(n)
       return serve(next(n))
   ```

   `build` prints *all paths through this function will call itself*; the run
   prints 1, 2, 3 and exits 0. `exit(code: 1)` becomes `h_library_exit(t4); goto
   bb1;` and nothing marks the call as never returning. By the class list a
   `blocking` defect (a clang warning on a correct program), unfiled
   (`grep -rl 'all paths through this function' issues`: 455 and 457 only).
   Route (C) would pass this false message on.
3. **Clang does not warn on p2.** `n - 1` is checked for overflow and calls
   `hero_panic_overflow()` (`_Noreturn`, `runtime/heroes_runtime.h:61`), so
   clang sees a path that ends without calling `fact`. (C) misses p2; (A) as
   worded misses it too if a hidden abort counts as a path end. The checker's
   own rule (`check/path_end.hero`) counts three written statements as path
   ends: `exit(code:)`, `assert false`, a `while true` with no `break`.
4. **Heroes has a written ruling against warnings.** `docs/design.md:3725`
   (Part 8): *this language has no warning level: a diagnostic is exit 1 or
   nothing, so "just warn" is not available.* `selfhost/diag.hero:23-27`:
   `variant Kind` with `error_kind` and `unsupported_kind` only (panel 020). The
   one non-error report is a hole (spec `:341-343`).

## Each framing fact

Written 23:43 (`stat`), consistent. Frozen at `56def9b4`, verified. *6 USD
approved at about 23:4x*: **"23:4x" is a placeholder**; the session count and
per-session cap are not written (§ 3c). 457 `systemic` verified; open at
`56def9b4`: 2 `systemic` (444, 457), 0 `blocking`, 4 `adjacent`, 59
`improvement`. `forever`: `check` 0 and build 0 verified, run 134 **at `-O0`
only** (hangs at `-O2`). The warning *over the generated C*: half true, its
place is already `forever.hero:1:41` through `#line`; only the column and the
excerpt are C's. `<scratchpad>/batch14/shadow/`: `build.txt` exists, no exit
code recorded. **"a function named like a built-in method" false**: `bytes`
is no built-in (§ 11 `:308-314`; `"hi".bytes()` with no such function is
`error[unknown_function]`, exit 1); defect 455's correction says so; route (E)
rests on the false premise. `:267`, `:280`, the three `recurs` lines, the
empty `warning` grep: verified (the negative's answer is design.md:3725).
Copy rule fine. *A dozen lanes*: `git worktree list` 16. *The coordinator
copies your reply* departs from § 3c (each seat writes as it goes, to
`<scratchpad>/199-<seat>/report.md`). Engineer: **`check/flow.hero` is the
wrong module** (panel 177's lattice of moved values); the every-path logic is
`selfhost/check/path_end.hero` (267 lines, panels 184 R4, 185 R4/R5) and
`Outcome.leaves` in `check/walk.hero:98`; the IR's blocks verified
(`ir/graph.hero`, `ir/dominators.hero`, `ir/callees.hero`); **"an
`abort`/`panic` path": no such statement exists**, the written path ends are
spec `:238-240`, overflow and out-of-bounds aborts are not; census over 2,980
tracked files; clang versions in the records 18.1.3, 21.0.0, 22.1.8, 23.1.1,
only 21.0.0 run (warns); (C)'s *read clang's warning back to a `.hero` line*:
the line is already there (`-Wall` at `flags.hero:97`; `cli/clang_told.hero`,
`emit/clang_place.hero` parse clang's diagnostics). Warden: **"`measure
--refresh` is paid" false by Anthropic's documentation, not run**
(`selfhost/cli/refresh.hero:33` posts to `/v1/messages/count_tokens`, which
the bundled claude-api skill says *costs nothing and samples nothing*; it
needs the trunk's `.env`; `.claude/rules/spec-shape.md` records *always
measure with the real*); `heroes measure spec/heroes-spec.md` gives
claude-legacy 7337, cl100k_base 7467, maximum 7467 (*a lower bound*), real
9831 (pinned 2026-10-07), headroom 409 against 10240, 60 mortgaged to the FFI
floor; the spec names no diagnostic code (`grep -c 'error\['` 0;
`polymorphic_recursion` has no spec sentence). Historian: questions,
correctly so.

## Routes the list misses

(F) mark `exit(code:)` as never returning in the C (until then (C) is not
truthful). (G) make `:280` true at `-O2` or have the spec name the level
(`-fno-optimize-sibling-calls`; cost: a correct deep tail recursion that runs
at `-O2` would abort; panel 104 rejected a depth counter). (H) the provably
infinite subset: a self-call passing the function's own parameters unchanged
(`forever(n)`, `s.bytes()`, `xs.count()`); misses p2. (I) a whole-program rule
over the call graph (`ping`/`pong`). (J) the status quo, named, weighed
against the `-O2` hang. (K) `-Werror=infinite-recursion` (false on `serve`).
(L) a spec sentence near `:267`: inside `function f`, `x.f()` is `f` itself.
(M) a better run-time message (`stack exhausted in gen.same_1b9a87` names a
mangled instance).

## Questions the sitting should ask

1. Is (C) an error or a new kind (design.md:3725 rules out a warning)? 2. Do
hidden aborts end a path (p2; `docs/panel/173-briefs/so_lease.hero:4`,
`q5_overflow.hero:3`)? 3. What does `heroes run` do with the shape: it hangs;
file it? 4. File the false warning on `serve` (blocking)? 5. Should `check`
see the rule (`polymorphic_recursion`: `check` 0, `build` refuses,
`ir/mono.hero:374`)? 6. A thesis rule (`is_thesis_rule`,
`selfhost/diag.hero:94-141`, a `permissive` case)? 7. Evidence an LLM makes the
mistake (was 455's program a lane's real mistake)? 8. What the rule refuses in
tracked programs (textual approximation, unrun: 2 of 10,590 top-level
functions, both deliberate probes in `docs/panel/173-briefs/`; 35 bodies hold
`.NAME(` inside `function NAME`, each a same-named function of another module
or a variant constructor: the rule must work on resolved names). 9. A fix,
`certain` or `guess` (none certain). 10. Every probe at `-O2` may never end:
the brief owes a time and a byte bound (`.claude/rules/verification.md` § A
run that may not end). Shapes missing: `serve`; the right side of `&&` and
`||`; `for` and `while <cond>` bodies; a self-call inside a condition; a
same-named function of another module; a variant constructor; an `@` first
parameter; `test` blocks.

## The blind seat's materials

Isolation right (`spec.md` byte-identical, no git tree, no `CLAUDE.md` above);
`claude` 2.1.285 accepting `claude-opus-5-5` untested. p1's premise holds
(`s.bytes()` resolves only to the program's own `bytes`). p2 is the
discriminating case (finding 3). Transcript defects: both A files end
`(exit )`, true value 134; both carry the harness's `bash: line 1: <pid> Abort
trap: 6 timeout 30 ./main`; the transcripts say `main.hero` while the files
are `p1.hero`, `p2.hero`. Answer keys run: p1 a loop pushing `s[i]` prints 2;
p2 with `if n <= 1` returning 1 prints 120. Missing: a p3 such as `count(xs)`
returning `xs.count()`, which hangs silently at `-O2`. The blind brief owes:
one folder per task and variant, neutral labels; sessions and a per-session
`--max-budget-usd` adding to 6 USD at most; the skill's command verbatim;
panel 196's headings (`program`, `choice_points`, `confidence`, `context`);
each program's stated intent; the context question; a copy in
`docs/panel/199-briefs/blind/`. Variant B owes the diagnostic as the
engineer's prototype prints it, in the compiler's real format, a transcript
stopping at `check` exit 1, the spec-warden's sentence or *unchanged*, no
`certain` fix; p2's B waits on question 2, or two B variants.

## Repairs to the briefs

1. Shared: the shape as a self-call through UFCS on a name the author believes
is a method; drop (E) or restate it as (H); add (F) to (M); design.md:3725 and
`diag.hero:23-27` for the warning negative; the `-O2` and `heroes run` facts
and `serve`; the approval time and the paid run's size; seats write as they
go; the run bounds. 2. Engineer: `check/path_end.hero` and `Outcome.leaves`;
the three written path ends and the hidden-abort question; the missing shapes
and probes p2, `serve`, `ping`/`pong`, `count`, `-O2`; the `.hero` line already
reaches clang's warning; the census count 2,980; `polymorphic_recursion`. 3.
Warden: the measured numbers; `--refresh` allowed with the trunk's `.env` (a
free endpoint) or the price as the `maximum` row, a lower bound; Q3's answer.
4. Historian: panels 104, 070, 097, 184, 185, 020 and design.md:3725; Go
(staticcheck SA5007), unverified. 5. Blind: clean both A files; consider p3;
write B after question 2.
