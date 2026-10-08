# Panel 199, the shared brief: a function whose every path calls itself

Written by the coordinator on 2026-10-07 from 23:43 by the clock, on the tree
frozen at `56def9b4` (worktree `lane-panel-199`). **Repaired on 2026-10-08
from 07:47 to 08:05 by `date`**, by the coordinator's hand, on the completeness
critic's first pass (`docs/panel/199-reports/completeness-critic-pass1.md`):
every repair that report lists is applied here and in the seats' briefs. Every
number, path and line below was produced by a command run while repairing,
in `<scratchpad>/199-briefs-work/`: a copy of the frozen tree, `tree/`, its
compiler built from its own seed with Apple clang 21.0.0 (the trunk's
`seed/`, `selfhost/` and `runtime/` are `56def9b4`'s, `git diff --stat
56def9b4 HEAD -- seed selfhost runtime` empty and the seed equal by `cmp`); the probes in
`probes/` and `blindrun/`, every one that might not end built first and its
binary run under `timeout 5` or `timeout 10`, output cut at 4096 bytes by
`head -c`, `pgrep` after each batch finding nothing. A figure not run there is
carried, and says from where. The versions the critic read are kept in
`first-pass/`.

**A full panel**: a new diagnostic is what the checker DOES (CLAUDE.md § 4):
the compiler-engineer, the spec-warden, the historian, the blind seat as fresh
`claude -p` sessions outside the repository, and the completeness critic
before the seats (done) and after them. **The author approved the blind
seat's paid sessions, 6 USD in all, between 23:40 and 23:43 on 2026-10-07 by
the clocks read before and after** (carried from the coordinator): at most
seven sessions at `--max-budget-usd 0.85` each, 5.95 USD
(`llm-ergonomist.md`). **No other paid run is approved.**

Convened for defect 457 (`systemic`), which blocks the milestone's tag. Open
at `56def9b4` by the class on each open defect's line (`grep -rh '^- \[ \]
\*\*[0-9]' issues/`): 2 `systemic` (444, 457), 0 `blocking`, 4 `adjacent`, 59
`improvement`. **Two `blocking` defects from the critic's first pass were
filed by the coordinator at 07:48 on 2026-10-08 (their `**Origin:**` lines), on
the trunk and uncommitted when this was repaired** (`git status` on the trunk,
read between 07:47 and 07:50 by the clocks before and after), so a copy of
`56def9b4` does not hold them: **507**, clang's warning
false on a correct program (`serve`, below), and **508**, the shape running for
ever at `-O2` (below). Both are `issues/2026-10/08/2026-10-08-0748-defect-50{7,8}-*.md`
on the trunk; 508's card says its route is this sitting's to choose.

## The question

Defect 457 (`issues/2026-10/07/2026-10-07-1837-defect-457-*.md`): a function
whose every path calls itself passes `check` at exit 0 and runs until
something stops it. **The shape an author meets is a self-call through UFCS
on a name the author believes is a method**: `function bytes(s: str) -> [u8]`
returning `s.bytes()` (defect 455's symptom). `x.f(y)` is `f(x, y)` (spec
`:267`), so inside `function bytes` the call `s.bytes()` is `bytes(s)`, the
function itself. **`bytes` is no built-in**: § 11 (spec `:308-314`) lists none,
and with no function of that name `"hi".bytes()` is
`error[unknown_function]`, *no function named `bytes`*, exit 1 (probe
`nobytes.hero`). So the author's own function is what makes the call legal,
and what makes it endless. A name that IS a built-in cannot take the shape:
`function len(xs: [i64]) -> i64` is `error[builtin_name_taken]`, exit 1 (probe
`lenname.hero`), a thesis rule (`selfhost/diag.hero:141-165`). Defect 455's
own correction says the same (`issues/2026-10/07/2026-10-07-1648-defect-455-*.md`,
*Corrected 2026-10-07*). **What does the compiler tell a function whose every
path calls itself, and where?**

### What the compiler does today, measured

| shape (probe) | `check` | `build`, clang 21.0.0 | run, `-O0` | run, `-O2` |
|---|---|---|---|---|
| `forever(n)` returning `forever(n)` (`forever.hero`) | 0 | 0, clang's warning at `forever.hero:1:41` | `panic: stack exhausted in forever.forever`, 134 | nothing printed, stopped by `timeout 5`, 124 |
| p1, `bytes(s)` returning `s.bytes()` | 0 | 0, warning at `p1.hero:1:44` | `... in p1.bytes`, 134 | `... in p1.bytes`, 134 |
| p2, `fact(n)` returning `n * fact(n - 1)`, no base case | 0 | 0, **no warning** | `... in p2.fact`, 134 | `... in p2.fact`, 134 |
| p3, `count(xs: [i64])` returning `xs.count()` | 0 | 0, warning at `p3.hero:1:45` | `... in p3.count`, 134 | nothing printed, stopped by `timeout 10`, 124 |
| `ping` returning `pong(n)`, `pong` returning `ping(n)` (`pingpong.hero`) | 0 | 0, **no warning** | `... in pingpong.pong`, 134 | nothing, 124 |
| `go(n)`: `f = go`, `return f(n)` (`fnvalue.hero`) | 0 | 0, **no warning** | `... in fnvalue.go`, 134 | nothing, 124 |
| `same<T>(x: T) -> T` returning `same(x)` (`gen.hero`) | 0 | 0, warning at `gen.hero:1:55` | `... in gen.same_1b9a87`, 134 | nothing, 124 |
| `serve`, below, **a correct program** | 0 | 0, **warning at `serve.hero:4:37`, false** | prints 1, 2, 3, exit 0 | the same |
| `grow<T>(x: T, n: i64)` calling `grow([x], n - 1)` (`polyrec.hero`) | 0 | **1, `error[polymorphic_recursion]`** at `polyrec.hero:4:12` | | |

The probes are in `<scratchpad>/199-briefs-work/probes/` and p1 to p3 in
`blindrun/`; the `-O0` column is `heroes build`'s default, the `-O2` column
`heroes build -O2`.

- **`heroes run` builds at `-O2` by default**: `selfhost/cli/verbs.hero:36`
  (`build`, `default_level: "-O0"`), `:61` (`run`, `"-O2"`), `:110` (`test`,
  `"-O0"`), the flag read by `compile.level_from`
  (`selfhost/cli/compile.hero:63-76`). `heroes build -O2` builds what `run`
  builds, the same `Options` with the level, **read in `verbs.hero` and not
  run**, since `heroes run` on a program that may not end is never run (the
  bounds, below). So at the level of the command a model uses, spec `:280`
  (*Recursion too deep aborts*) is false for every shape above but p1 and p2:
  clang turns the self-call into a jump, and route (D)'s witness does not
  exist there (defect 508). Panel 104 measured that transformation
  (`docs/panel/104-deep-recursion-stops-with-a-word.md:64-68`: *at `-O2` ...
  the plain recursion costs 0.000 s, because a counter with a side effect
  blocks the transformation that turns the recursion into a loop*); panel 070
  scored *it depends on the optimisation level* falsified for its own shape,
  *the tail call clang was supposed to be making is one the C standard does
  not permit it to make* (`docs/panel/070-the-stack-nobody-was-counting.md:141-144`).
- **Clang's warning is false on a correct program**:

  ```
  function next(n: i64) -> i64
      return n + 1

  function serve(n: i64) -> i64
      if n > 3
          exit(code: 0)
      print(n)
      return serve(next(n))
  ```

  `build` prints *all paths through this function will call itself*; the run
  prints 1, 2, 3 and exits 0 at both levels. `exit(code: 0)` is emitted as
  `h_library_exit(t4);` then `goto bb1;`, and the library declares `void
  h_library_exit(int64_t h0_code);`, nothing marking it as never returning
  (the probe's `build/tu-*/library.c`, `grep -c _Noreturn` 0). Defect 507,
  `blocking`. A route that passes clang's warning on, (C) or (K), passes this
  false message on until (F) lands.
- **Clang is silent on p2**, and on `ping`/`pong` and the function value. In
  p2's C, `n - 1` is `if (__builtin_sub_overflow(t2, t3, &t4))
  hero_panic_overflow();` before `t5 = h_p2_fact(t4);`, and
  `hero_panic_overflow` is `_Noreturn` (`runtime/heroes_runtime.h:61`), so
  clang sees a path that ends without calling `fact`.
- **The warning's place is already the `.hero` line**, through `#line`
  (`p1.hero:1:44`); only its column and its excerpt are the C's. `-Wall` is
  `selfhost/cli/flags.hero:97`; `selfhost/cli/clang_told.hero` (238 lines) and
  `selfhost/emit/clang_place.hero` (388) read clang's diagnostics back.

### Warnings: a written ruling, not an open negative

The first version left *whether Heroes has warnings* for the seats to grep.
It has a ruling: `docs/design.md:3725` (Part 8, *Known warts*): *this language
has no warning level: a diagnostic is exit 1 or nothing, so "just warn" is not
available.* `selfhost/diag.hero:22-27`: `variant Kind` holds `error_kind` and
`unsupported_kind` and nothing else (its comment at `:17` cites panel 020).
The spec has no warning class (`grep -n -i warning spec/heroes-spec.md`, no
line); its one report that is not an error is a hole (`:341-343`). So a route
that would *warn* is an error, or a new kind, and a new kind is a ruling of
its own.

### The spec today

`:280` *Recursion too deep aborts.* `grep -n -i recurs` gives three lines,
`:113` (mutual recursion needs no forward declaration), `:193` (`==`
recursively) and `:280`, none about a function calling itself on every path.
The written path ends are `:238-240`: `exit(code:)`, `assert false` and a
`while true` with no `break` of its own; an overflow, an index out of bounds
and a `.must()` abort, and none of them is written as a path end. The spec
names no diagnostic code (`grep -c 'error\[' spec/heroes-spec.md`, 0), and
`polymorphic_recursion`, a build-time refusal of a recursion shape, has no
spec sentence (`grep -c polymorphic`, 0).

## The routes, a list to be widened

- **(A)** a checker rule: every path of a function's body reaches a call to
  the function itself with no path that returns or ends first: an error
  naming the call and the function, with what to write instead. **Open: does
  an abort the program does not write end a path** (question 2)? If it does,
  (A) misses p2 as clang does; if only the three written ends count, as
  `selfhost/check/path_end.hero` reads them (by syntax, never by value), (A)
  refuses p2;
- **(B)** the rule on the IR after lowering (the control-flow graph is
  there), told in the compiler's words; there the overflow check is a branch
  to an abort, so (B) answers question 2 by construction unless it says
  otherwise;
- **(C)** clang's `-Winfinite-recursion` read by the build and told in the
  compiler's words: an error or a new kind, never a warning (above); false on
  `serve` until (F); silent on p2, `ping`/`pong` and a function value;
- **(D)** clang's warning silenced, the run's abort the only witness: at
  `-O2`, `heroes run`'s level, there is no abort to witness;
- **(E)** *withdrawn*: it rested on *a function named like a built-in
  method*, and `bytes` is no built-in, while a function named like one is
  already refused (`builtin_name_taken`). Its sound half is (H);
- **(F)** mark `exit(code:)` as never returning in the emitted C, which (C)
  and (K) need before they are truthful (defect 507);
- **(G)** make `:280` true at `-O2` (`-fno-optimize-sibling-calls` or its
  equal), or have the spec name the level; its cost, a correct deep tail
  recursion that runs at `-O2` today would abort; panel 104 rejected a depth
  counter (`:64-68`) (defect 508);
- **(H)** the provably infinite subset: a self-call passing the function's
  own parameters unchanged (`forever(n)`, `s.bytes()`, `xs.count()`); misses
  p2;
- **(I)** a whole-program rule over the call graph (`ping`/`pong`);
- **(J)** the status quo, named, and weighed against the `-O2` hang;
- **(K)** `-Werror=infinite-recursion`, false on `serve` until (F);
- **(L)** a spec sentence near `:267`: inside `function f`, `x.f()` is `f`
  itself;
- **(M)** a better run-time message: `panic: stack exhausted in
  gen.same_1b9a87` names a mangled instance (`gen.hero`).

(F) to (M) are the critic's. The routes combine, and (F) and (G) answer two
`blocking` defects whatever else is adopted.

## The questions the sitting asks

The critic's ten, with what was run since:

1. Is (C) an error or a new kind (design.md:3725 rules out a warning)?
2. Does a hidden abort end a path (p2; `docs/panel/173-briefs/so_lease.hero:4-5`,
   `return down(n: n + 1) + 1`; `docs/panel/173-briefs/q5_overflow.hero:3-5`)?
3. `heroes run` hangs on the shape at `-O2`: filed as defect 508.
4. The false warning on `serve`: filed as defect 507, `blocking`.
5. Should `check` see the rule? `polymorphic_recursion` is the precedent of a
   recursion rule `check` does not see: `check` 0, `build` 1
   (`selfhost/ir/mono.hero:374`).
6. A thesis rule (`is_thesis_rule`, `selfhost/diag.hero:141-165`, which
   `check --permissive` drops), with a `permissive` case?
7. Evidence an LLM makes the mistake: defect 455 came from lane
   b14-runtime's final report, *found beside* (its `**Origin:**`); whether it
   was a model's own mistake is unrun.
8. What the rule refuses in tracked programs: the census over the 2,980
   tracked `.hero` files at `56def9b4` (`git ls-tree -r --name-only 56def9b4
   | grep -c '\.hero$'`). The critic's textual approximation, carried and not
   re-run: 2 of 10,590 top-level functions, both deliberate probes in
   `docs/panel/173-briefs/`; 35 bodies hold `.NAME(` inside `function NAME`,
   each a same-named function of another module or a variant constructor, so
   the rule works on resolved names.
9. A fix, `certain` or `guess` (none looks certain).
10. Every probe at `-O2` may never end: the bounds below.

## The rules every seat works under

- **Your own copy**: `cp -R` the frozen tree to
  `<scratchpad>/199-<seat>/tree` (the scratchpad is
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad`),
  `rm -rf build` in the copy, your compiler from its seed (`clang -I runtime
  seed/heroes.c runtime/runtime.c -o heroes`). Never build, run or write in
  the frozen tree, the trunk, any `lane-*` worktree or another seat's
  directory.
- **Your report as you go**: running notes in
  `<scratchpad>/199-<seat>/report.md` from your first finding, not only at the
  end, so a stopped seat leaves what it had (`.claude/skills/panel/SKILL.md`
  § 3c). Your final reply is the record as well; the coordinator copies the
  file into `docs/panel/199-reports/<seat>.md`.
- **Every run that may not end is bounded in time and in bytes**
  (`.claude/rules/verification.md` § A run that may not end): every shape of
  this sitting may never end at `-O2`. Build the program with `heroes build`
  (`-O2` for `run`'s level), run the BINARY under `timeout 10`, its output to
  `/dev/null` or through `head -c 4096` into a file; **never `timeout heroes
  run`**, which leaves the program running (defect 425); after each batch
  `pgrep -f` your probe directory and stop what it finds before you write a
  verdict.
- Counts only, never durations: `git worktree list` gives 16 entries on this
  machine, the trunk's among them. No paid run but the blind seat's, which the
  coordinator runs; a seat that finds one worth running says so, with its
  size. Times only from `date`.
