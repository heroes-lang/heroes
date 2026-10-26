# Panel 230, shared brief: what a failing program tells whoever ran it

**Convened 2026-10-26** in M-panic-location (row 72), on task 812, features 1001, 1155 and 2083
(https://github.com/heroes-lang/heroes/issues/812, /1001, /1155, /2083) and on two defects the
critic's first pass found and the coordinator reproduced and filed that day, **2224** (`exit(code:)`
with a multiple of 256 tells the shell the program succeeded) and **2225** (a line feed in a
program's own text forges lines of an abort's report). By the author's yes of that day, asked once
for the milestone (CLAUDE.md § 4), with these answers: **the full panel**; **issue 1001 in the same
sitting, with a resolution of its own**, so either can be ratified without the other; **issue writes
for the session**; and **the blind seat as 84 short sessions, at most 12 USD in all** (first asked
as four sessions of at most 3 USD; changed on the critic's finding that one session a cell can show
no difference). Five seats and a completeness critic, which read these briefs before any seat
started (`docs/panel/230-reports/critic-briefs.md`, 40 findings) and reads the reports after.

**The tree**: the lane `panic-location` at `595d92d88` (step 0, which changed `ROADMAP.md` and the
milestone's page and nothing the compiler reads), whose `selfhost/`, `runtime/` and `seed/` are the
trunk's `1b3bb242f` (`git diff --stat 1b3bb242f 595d92d88 -- selfhost runtime seed` prints nothing).
**Each seat that builds works in its own detached worktree at that commit**,
`.claude/worktrees/230-<seat>/`, made by the coordinator, and builds its own compiler there (`clang
-I runtime seed/heroes.c runtime/runtime.c -o heroes`, 7.97 s in the lane and 7.88 s in the critic's;
then `./heroes build selfhost/main.hero -o heroes-next`, real 86.08 s and 87.16 s; `heroes-next
--version` prints `sources 413da33e7969760a`); it reads nothing of the lane but its brief and writes
nothing there but its report, `docs/panel/230-reports/<seat>.md`, **written as it goes**: the critic's
first pass was stopped by a rate limit and had written nothing, so a stop cost its whole work until
it was resumed. **No seat writes outside the repository's root** (CLAUDE.md § Hard stops; a seat's
scratch is its worktree or `.claude/worktrees/230-<seat>-scratch/`); no seat reads, builds or runs in
another's worktree; **no seat prints a credential** (`gh auth status`, never `gh auth token`); **none
starts a paid run** (the blind sessions are the coordinator's); `heroes measure --refresh` calls the
token-counting endpoint every landing in the spec's ledger has called, and the spec-warden is named
for it.

**This machine is shared today** with two other sessions (`heroes-lang-19`, Dependabot and CodeQL
repairs in the lane `deps-codeql`; `heroes-lang-3f`, a probe of an embedded C compiler in the lane
`standalone-compiler`). Pass/fail work runs beside theirs. **No seat starts a clock**: a seat builds
what is to be timed and writes the exact commands in its report; the coordinator agrees a still
machine with the two sessions and runs them (`.claude/rules/verification.md` § What may run beside a
gate: parallel work is free on correctness and forbidden on duration).

The programs below are in `.claude/worktrees/panic-location-scratch/before/`, `.../defects/` and
`.../blind230/`, and the critic's in `.claude/worktrees/230-critic-scratch/`; copy them, never write
there.

## The proposal, as a spec diff, and what it changes besides

Three sentences of the specification, the proposal's draft, each a question the sitting rules:

```
§ 1   - One file is one module; the file you compile holds `function main()`, which
-       takes nothing and produces nothing.
+       takes nothing and produces nothing or a `()?`: a `fail` it returns is written
+       on stderr, and the program exits 1.

§ 6   -An abort ends the program at once, saying why; no `T?` carries one.
      +An abort ends the program at once, saying why and where: the file, line and
      +function; no `T?` carries one.

§ 12  -`assert` failure shows the source expression, and both sides where `print`
      -takes them; where a side is an aggregate it shows the expression alone.
      +`assert` failure shows the source expression, and both sides of a comparison,
      +written as this document writes a value.
```

**They are not the whole of what the proposal changes** (the critic, findings 36 and 24): § 13
says *a lease nobody ends, like a handle nobody consumes, aborts when `main` returns, saying how
many* (`spec/heroes-spec.md:426-427`), measured at exit 134 with `panic: 1 lease(s) never ended`,
and a `main` that returns a `fail` has returned, so § 1's *exits 1* and § 13's abort disagree unless
the proposal says which wins; and § 11's `exit(code: i64)` *(ends the program)* states no range,
while defect 2224 measures 256 → status 0. **A § 1 sentence about `main`'s result is a reopening**:
it was written by panel 159's R3 (2026-09-16, ratified that day **by delegation and not by reading**,
its own foot says so), on the question *may `main` be `-> ()?`* (below).

**What a program shows under the proposal** (hand-written; nothing prints it yet):

```
panic: array index out of range
  at report.hero:13:44, in report.windows

assert failed: normalized(drawn[i]) == expected[i]
  at rects.hero:35:16, in rects.main
  left:  Rect(x0: 2, y0: 1, x1: 5, y1: 1)
  right: Rect(x0: 2, y0: 1, x1: 5, y1: 4)

error: no_key: port            (a `main` that returned `fail(code: "no_key", msg: "port")`; exit 1)
```

The first line of every abort is today's, unchanged; the place is a line beneath it, `at
<file>:<line>:<column>, in <module>.<function>`. **The column is the start of the expression whose
evaluation stopped the program** (`xs[i + 2]`, `prices[item].must()`, `h * 31`, and an `assert`'s
asserted expression, not its keyword). **The file is the path a program's modules name it by**: the
root's file name, a `use`d module's path from the root's directory, never the path the build was
invoked with, which today varies the text by where the build ran (finding 6, measured: `at
/Users/.../before/endless.hero:2:12` against `at before/endless.hero:2:12`, and two emissions that
differ at every `#line`).

**Its mechanism, the proposal's (L1)**: every abort the program's own operation raises passes a
pointer to a static constant record of its place, `{file, line, column, function}`, written by the
emitter beside the function; the runtime's entry points that abort take it and print it; nothing is
computed on the path that does not abort. `HERO_RUNTIME_ABI` 30 → 31, by the route `seed/README.md`
§ When this file must be regenerated calls `abi-put-back`. A side of an `assert` is written by a
renderer the compiler emits per type that stands as a side, called on the failing path alone,
bounded, in one notation with the lldb formatter, never a `HeroDesc` member and never anything a
program can call. **Aborts L1 cannot reach** (below, the third kind) name what the frame walk names.

## What is true today, each with the command that settles it

Programs built with the lane's `heroes-next` and their binaries run directly, on **Darwin arm64**
(`uname -sm`); no Linux or Windows status was measured in this sitting.

- **The "before"** (`./<f>.bin >/dev/null 2><f>.err; echo $?`):

  | the abort | stderr | exit |
  |---|---|---|
  | `xs[i]` past the end | `panic: array index out of range` | 134 |
  | `i64` max plus one | `panic: integer overflow` | 134 |
  | `.must()` on an error | `panic: .must() on an error: odd: not divisible by two` | 134 |
  | `"caffè".slice(from: 0, to: 5)` | `panic: string slice splits a character` | 134 |
  | `assert a == b`, two `i64` | `assert failed: a == b`, `  left:  3`, `  right: 4` | 134 |
  | `assert p == q`, two records | `assert failed: p == q` | 134 |
  | a recursion whose base case is never reached | `panic: stack exhausted in deep.down` | 134 |
  | `main` matching a `fail` and printing it | (stdout: the error) | **0** |
  | `exit(code: 256)`, `512`, `-256` (defect 2224) | nothing | **0** |
  | an `assert` side `"a\n  right: a"` (defect 2225) | `left:  a`, `right: a`, `right: a` | 134 |
  | a `.must()` message `"x\npanic: integer overflow"` (2225) | two `panic:` lines | 134 |

  No line names a `.hero` file or a line; the stack guard alone names a function. A recursion with
  no base case is refused at compile time (`error[endless_recursion]`). `heroes test` on two
  failing tests prints `FAIL "<title>"` on stdout and `assert failed: ...` on stderr for each, exit
  1: two streams, tied by their order alone.
- **Where an abort is written**, three kinds:
  1. **in the program's C**, under the author's `#line`: the overflow (`if
     (__builtin_add_overflow(t1, t2, &t3)) hero_panic_overflow();`), the division by zero, and the
     index check in its checked form, `(t1 == NULL || t2 < 0 || t2 >= t1->len) ?
     ((void)hero_array_at(t1, t2), hero_unreachable()) : ...`; and under the **generated file's**
     `#line` on purpose (`selfhost/emit/abort.hero:8-10`, *lldb must not blame their line for it*),
     `.must()` (`#line 139 "must.c"`) and `assert` (`#line 105 "assertint.c"`). Counted through
     `.claude/rules/verification.md`'s `code_of` (a module without its `test` blocks and comments),
     **19 lines in 7 modules** write a `hero_panic*` call (`emit/operator.hero` 5, `abort.hero` 5,
     `structural.hero` 3, `division.hero` 3, `places.hero`, `lend_extent.hero`, `c_main.hero` 1
     each; the brief's first count, 29, took 10 strings inside `test` blocks); the index check is
     written by `emit/array_read.hero:37`, `:62` and `:65`, and **`:62`, the form an impure place
     takes, calls `hero_array_at_mut` on every access**, which checks on the path that does not
     abort; `hero_unreachable` is written by 59 lines in 20 modules.
  2. **inside a runtime call on the path that does not abort** (a slice through a character, a
     `nan` sorted, `args()` over bytes that are not UTF-8, a handle given back twice): the runtime
     raises it and is told no place. `grep -cE 'hero_panic[a-z_]*\(|hero_abort\(\)'
     runtime/parts/*.c runtime/*.c` counts 145 lines in 19 files, definitions included (`alloc.c`
     20, `str.c` 20, `map.c` 13, `panic.c` 12, `stack.c` 11, `array.c` 10, `lend.c` 9, `cow.c` 8,
     `spawn.c` 7, `failure.c` 6, `f64.c` 5, `sort.c` 5, `map-write.c` 4, `codepage.c` 3, `os.c` 3,
     `run.c` 3, `text.c` 3, `dir.c` 2, `thread.c` 1). The entry points are named in the headers:
     `hero_str_slice` is `runtime/heroes_runtime.h:286`, `args()` reads `hero_args_count` and
     `hero_args_at` (`runtime/hero_os.h:110-118`).
  3. **where no call of the program stands** (the critic, finding 4): fifteen `panic:` lines inside
     the census are written in fault handlers (`stack.c:431`, `:617-755`, *stack exhausted*, *a null
     function pointer was called*, *a dead C handle was used inside C*, *a null pointer was read
     through*, and their Windows twins at `:884-961`) and at exit (`hero_runtime_check_leaks`,
     `alloc.c:272-404`, heap blocks, leases, handles); ten more are outside the census, ending by
     `return`, `raise(` or a bare `abort()` (`os.c:211-426`, the lease handlers; `lend.c:651-699`,
     *C reached past a buffer lent to it*). **L1 cannot locate this kind by construction**, so the
     proposed § 6 sentence is false for it under L1 alone.
- **The frame walk the runtime already has**: `hero_stack_blame` (`stack.c:374`) names the first
  Heroes frame by `dladdr`, and **`hero_stack_names`** (`stack.c:544-561`, `HERO_STACK_SEEN` 64)
  collects every Heroes function on the interrupted stack, callee first, written for *the recursion,
  not only the frame that ran out* (defect 521, panel 199's route (M)); *on x86-64 the faulting
  function's caller is never on the chain*; *Windows has no walk* (`stack.c:850-857`: dbghelp and a
  PDB unmeasured on the box).
- **The line table**: `-g` at `-O0`, `-gline-tables-only` otherwise
  (`selfhost/cli/flags.hero:169-172`). **On Darwin a built binary carries none** (the critic,
  finding 5: no `__DWARF` segment; `dsymutil -s` names the objects under `build/`, and no
  `dsymutil` call exists in `selfhost/`), so a line read at the stop there needs `build/` beside the
  binary or a `dsymutil` step. **`heroes run` builds at `-O2`** (`selfhost/cli/table.hero:159`, *the
  dev loop*), `heroes build` at `-O0`.
- **The type descriptors**: `HeroDesc` is five members, `size`, `copy`, `drop`, `eq`, `hash`
  (`runtime/heroes_runtime.h:503-509`). An `assert`'s side is rendered through
  `builtins.to_str_entry` (`selfhost/emit/builtins.hero:281-297`): the numbers, `bool`, and `str` as
  `hero_str_identity`, **unquoted**, so `assert s == ""` shows an empty side and defect 2225's line
  feed forges a line; every other type answers `none` and the assert shows its text alone
  (`abort.hero:99-156`). **Two notations for one character stand in the tree** (finding 9): the
  failure printer (`runtime/parts/failure.c:110-183`, panel 192's R7) writes a control character as
  `<U+001B>` and a line feed and a tab as themselves; the lldb formatter
  (`editors/lldb/heroes_lldb.py:20-40`) writes a `str` as a literal with the spec's escapes, `\n`,
  `\u{1b}`. Its other rules: a record as its construction, `[T]` as `[e0, e1, ...]`, `{K: V}` in
  bucket order, `T?` as `ok(v)` or `fail(code: "...", msg: "...")`, a variant `.case(field: v)`;
  bounds `SHOWN` 16, `TEXT_BYTES` 1024, `BUDGET` 256, `DEPTH` 6 (`:93-106`).
- **Two sides that render alike**: `assert x == x` with `x = 0.0 / 0.0` prints `left:  nan`,
  `right: nan` (finding 10); every bound adds such pairs (arrays differing past element 16, texts
  past byte 1,024), and a map ordered by rendered text needs a tie-break where two keys render alike
  within the bound.
- **What pins today's § 12 behaviour** is not under `tests/golden`: `tests/harness/suite_special.hero:308-317`
  asserts an aggregate side is ABSENT, *so defect 016 is repaired and the spec sentence panel 117
  narrowed has expired: read that sitting, unpin this row* (panel 117's *ratified tripwire*; defect
  016 is issue 991, closed); `:296-302` pins `left:  4` and `right: 5`. Under `tests/golden`:
  `grep -rl '^!panic:' tests/golden examples tests/thesis` finds 132 files (`run` 129, `control`
  3), matched by `strings.contains` (`tests/harness/expectation.hero:159-166`), so a line added
  beneath an abort's first line moves none; of 9 files naming `assert failed`, 5 are the frozen
  recovery corpus and one live case pins it, `run/fixedbugs-580-...expected`; no golden pins `!exit:
  134`; **`main_returns` has no golden at all** (finding 40: two unit tests, `check/decls.hero:371`
  and `:388`).
- **What Part 11's instrument does with an abort**: on `ran.code >= 128` with `panic:` in stderr,
  outcome `abort`, the stderr trimmed (`tests/harness/thesis_grade.hero:257-260`); **a failed
  `assert` has no `panic:` and is `crash`**, its stderr after *ended with status N*; a wrong status
  below 128 is `compiled_wrong`, its stderr after *its stderr was*. That message is turn 2's feedback
  (`thesis_run.hero:9-12`). **No arm varies what an abort says**: `ARMS` is `strict`, `brief`,
  `control` (`thesis_args.hero:50-53`), one `--compiler` a run, *the feedback symmetric across the
  arms*; and **the grader ignores stderr** (`docs/metrics/tasks.md:58-61`).
- **What a program that handles its own failure has**: no built-in writes to stderr (`grep -n -iE
  'stderr|eprint'` over the spec and `selfhost/inventory.hero`, nothing); **a binding does, in three
  lines**: `extern "hero_os.h"` `function hero_write_err(text: str)` (`runtime/hero_os.h:108`) wrote
  `error: no_key: port` at exit 1 in the critic's run (finding 12), and **the compiler itself writes
  its errors that way**, from 88 code lines in 21 modules (`selfhost/cli/io.hero:12-32`: it once
  bound POSIX's `write`, which MSVC and clang-cl do not ship); a POSIX `write` binding works on POSIX
  alone. The held-out tasks forbid `extern` (`docs/metrics/tasks.md:55`). The examples print the
  error on stdout and call `exit(1)` (`examples/interpreter/main.hero:42-49`,
  `examples/ledger/main.hero:36-51`): 11 of the 55 directories under `examples/` call `exit(`, as
  `exit(1)` 18 times, `exit(2)` 9 and `exit(f.code)` once (`grep -rnE '\bexit\('`, a comment
  excluded). The visible task suite, `tests/thesis/01` to `10`, has 90 cases, 40 of them `!exit:`
  (27 `!exit: 1`, 12 `!exit: 2`, 1 `!exit: 3`); its writing rules pin every task's error statuses in
  prose (`docs/metrics/tasks.md:58-68`), so the held-out run (`suite.enc`, sealed, 40 tasks) measures
  a stated status, never a default. The compiler's `main` ends by `exit(code:)` 14 times
  (`selfhost/main.hero:99-225`), with 0, 2 and the codes `cli_table.exit_code` gives: **a status M1
  cannot give**, so the compiler would not use M1. Whether a silent program of measurement 052 was
  an error path ending at 0 is a question: its runs are sealed and this brief did not open them.
- **`main` may not declare a result**, `error[main_returns]` (`selfhost/check/decls.hero:96-100`):
  *`main() -> i64?` returning fail(…) compiled, printed nothing and exited 0*.
- **The seed**: `seed/heroes.c` is 271,387 lines and 27,000,911 bytes, with 57,860 `#line`s and
  **17,297 abort call sites** (`hero_panic_overflow(` 7,570, `hero_array_at(` 6,828,
  `hero_panic_must(` 2,382, `hero_panic(` 278, `hero_array_at_mut(` 170, `hero_panic_assert_sides(`
  37, `hero_panic_assert(` 30, the two `unmatched` 1 each) beside 23,996 `hero_unreachable(`
  (finding 35, `grep -oE ... | sort | uniq -c`); **M-bounded-seed's target is a seed under 10 MB**
  (its page, `:26-34`), by an emission profile without `#line`s. L1 writes a place at each site.
- **The spec**: **7854** on the vendored maximum (`heroes-next measure spec/heroes-spec.md`, today)
  and **10,297** on the reader's own, the row pinned on 2026-10-24 (not re-measured here); the
  ceiling 16384, against which 10,357 is measured (the FFI floor's 60, panel 030 R3); the
  consolidation block's 1,500, of which the ledger's row 115 says 251 spent and row 116 is a -2 with
  *no addition, so no payment is owed* (`docs/measurements/010-spec-budget-ledger.md:231-232`);
  whether the block still binds after M-thesis-harness's run is a question (design.md binds it
  *before the thesis harness has run*). **The draft** reads 7881 vendored, 7743 legacy: +27, a lower
  bound.

## The history this sitting stands on

- **Panel 035's R3** (`docs/panel/035-...md:156-157`): *`main` may not declare a result. New
  diagnostic `main_returns`, zero spec tokens ... Revisit when M7 decides `exit(code)`.*
- **Panel 159** (2026-09-16, M-check-completeness; ratified the same day *by delegation and not by
  reading*): convened on *may `main` be `-> ()?`* among three silences. Its compiler-engineer *would
  veto accepting `main() -> ()?` without a specified exit code*; its **R3**: *the sentence is the
  diagnostic's, `main` PRODUCES NOTHING*, landed in `f20f03dce`; its llm-ergonomist registered *≥1
  in 3 first attempts at a fallible `main` writes `-> ()?`*, *checkable today by compiling four
  lines*, and drafted an M1-shaped sentence (`159-reports/llm-ergonomist.md:354-357`); its historian
  sourced Go's refusal of a result from `main` (`159-reports/historian.md:134`). **What changed since**,
  the sitting's to weigh: Part 11's instrument runs (M-thesis-harness, 2026-10-24) and its task
  rules ask statuses; defect 2224; issue 1001 re-homed here by M-rechecked-issues; and the author
  has not read 159.
- **Panel 117** (convened 2026-09-07, ratified that day), *both sides of an `assert`, when a side is
  an aggregate*: **(d), a sixth `HeroDesc` member, refused, double-vetoed** (lifted only for a
  construct that makes a five-member positional initialiser a compile error, or a NULL-render guard
  naming the type plus a proven two-stage bootstrap); **(c), per-type generation, vetoed by the
  compiler-engineer and approved by the ffi-pragmatist**, refused *for now because the map clause it
  would owe is unanswered* (two `==`-equal maps iterate `b a c` and `b c a`; the order dispatch is
  `hero_cmp_for`, `runtime/parts/sort.c:130-143` today, twelve scalar descriptors and no record) and
  for the ~52,200-frame cliff panel 076 measured. Its historian registered *if a renderer reachable
  only from `assert` lands, the first non-test consumer adds a second printer rather than reusing
  it*.
- **Panel 131** (ratified 2026-09-11): run-time reflection refused, a design.md Part 6 row; panel
  117's leaf walk withdrawn on its own measurements; its conservative variant, *records and variants
  only, no recursion through `[T]` or `{K: V}`*, recorded and not adopted. **Panel 132**: § 12's
  *until this language renders one* named a condition nothing worked toward, *`assert` has no program
  to ask*; the clause is gone.
- **Panels 104, 173, 199**: the stack guard (*deep recursion stops with a word*), *the runtime may
  say what it saw, and not why* (the lease handler's silence, `panic.c:25-31`), and
  `hero_stack_names`. **Panel 174**'s brief (2026-09-21) measured an abort on the author's Windows
  box exiting `0xC0000409` (`__fastfail`, kept by `os.c:241-247`), not re-run since. **Panel 192**'s
  R7, the failure printer. **Panel 222**, every program's sources key (`hero_program_sources`).
- **design.md §4.18** designed `assertion failed at line 12` above the expression and the sides
  (`docs/design.md:2460`); the line never landed. **design.md §1.12**, *what would make this wrong*:
  *an abort that stops a program at a place that tells the reader nothing*.
- **The milestone's page**, widened on 2026-10-14 in `44f98952b` by the coordinator, named *the
  runtime's type descriptors*, which is (d) above; step 0 corrected it beneath. Its rules stand: *the
  location is passed, never computed*, and *the corpus leg's time before and after is the
  measurement* (which, the critic measured, times the builds, finding 13; the path that does not
  abort is timed on the compiler itself, `heroes-next build selfhost/main.hero`, with and without
  `--emit-c`).

## The routes the sitting judges

**Where (the place)**

- **L0, the issues' own** (812, 1155): the entry points become macros reading `__FILE__`,
  `__LINE__` and `__func__`. Measured against it: two of the five emitter-side classes sit under the
  generated file's `#line` on purpose; `__func__` is the C name (`h_index_pick`); the file is the
  invocation path (finding 6).
- **L1, the proposal**: a static place record per site, passed on the failing path. For kind 2,
  **L1a** the entry point takes the place on every call (paid on every impure indexed write too);
  **L1b** the check moves to the program's C, as the index check's checked form already is, the
  runtime's check staying as an invariant behind it; **L1c** the entry point answers a status and the
  program's C aborts at the call with the place. Kind 3 is not reached.
- **L2**: the place found at the stop from the line table, read in-process; on Darwin, `build/` or
  a `dsymutil` step (above).
- **L3**: a thread-local cell of the current site, written before each call that can abort; computed
  on the path that does not abort, which the page's rule refuses, so judged on its measurement.
- **L4**: the frame walk, `hero_stack_names` extended from the stack guard to every abort: the chain
  of Heroes callers (POSIX; the x86-64 leaf; no Windows walk). It is what reaches kind 3.
- **L5, the place resolved off-line** (finding 37): the abort prints the site's image-relative
  address and the program's sources key; a `heroes` verb resolves it against the build's debug
  information. Nothing passed, nothing computed, printable from a signal handler; costs a verb (the
  stopping rule, `.claude/rules/cli-surface.md`), `build/` kept on Darwin, and an address a golden
  pins only as an offset of a reproducible link. Unrun.
- A combination is a route (L1 for kinds 1 and 2, L4 for kind 3), and so is a narrower § 6.

**What (the sides)**

- **S1, the proposal**: a renderer per side type, emitted, bounded as the lldb formatter, in one
  notation with it (held by a test over one corpus of values, or one generated from the other), a
  `str` quoted with the spec's escapes, map entries in the order of their rendered text with a
  stated tie-break, a `ptr`, a `cstr`, a function value and a handle written as an address and never
  read through.
- **S2**: S1 for records and variants of scalars alone; an array or a map by its length.
- **S3**: the first differing field or element alone, with its path (`left.y1: 1`, `right.y1: 4`).
- **S4**: today's sentence.

**What `main` tells the shell (issue 1001), and what `exit` tells it (defect 2224)**

- **M1, the proposal**: `main` may produce `()?`; a `fail` it returns is written on stderr, `error:
  <code>: <msg>`, through the printer defect 2225's repair settles, and the program exits 1; `?`
  leaves `main`; `exit(code:)` unchanged; `main_returns` narrows to every result but `()` and `()?`;
  the proposal says what § 13's leak check does then.
- **M2**: `main -> i64`, the status (C's shape). **M4**: M1 with the `fail`'s place.
- **M5** (finding 12): a built-in that writes a line on stderr, `main` unchanged, which the compiler
  itself would use; it gives a channel and changes no default.
- **M6** (finding 27): a status carried by the failure (`main -> i64?`, or a `fail`'s code mapped
  to a status), so a task asking 2 is served as one asking 1.
- **M7, the obligation** (finding 11): a `main` whose `.err` arm leaves it normally is told so.
- **M3**: today: `exit(code:)` is the route.
- **`exit(code:)`'s range** (defect 2224), whichever M is taken: refused at compile time where the
  code is a constant outside 0 to 255, an abort otherwise; or clamped to a failure; or stated as the
  low eight bits.

## The questions inside every route

1. **The cost**: the compiler's own path, `heroes-next build selfhost/main.hero --emit-c` and the
   whole build, timed by the coordinator before and after from the seat's two binaries; the
   binary's size; **`seed/heroes.c`'s bytes before and after**, against M-bounded-seed's 10 MB.
2. **The function's name** in a generic instance, a `test` block (its title?), a function value
   called through a parameter, the library's own Heroes (`map`, `find`), and `main`.
3. **The column** the IR holds (every `Inst` carries `span: token.Span`, `selfhost/ir.hero:333-337`).
4. **Windows**: what kind 3 prints there; `0xC0000409` re-run on the box is owed (the author starts
   it).
5. **The signal path**: what of a place a handler prints async-signal-safely.
6. **A side and the C boundary**: a group record's fields, a union, a `cstr` that may dangle.
7. **Determinism**: the same program prints the same abort text on every run and machine.
8. **The statuses**: an abort, a `main` that fails, `exit(code:)` past 255, a lease live when `main`
   returns a `fail` or a `?` leaves it.
9. **The landing**: the seed's route, and what enters `selfhost/` only after the refresh whose seed
   knows it (`seed/README.md`, 2026-10-18).
10. **A thread** (`runtime/parts/spawn.c`, the guard's bounds `_Thread_local`, `stack.c:276-277`):
    what an abort on a started thread prints, and where L4 stops.
11. **`-O2`**, the dev loop's level: inlining folds a function into its caller for L2 and L4, not
    for L1's static record.
12. **`heroes test`**: whether the place line ties a failure to its test.
13. **The control arm**: whether `--permissive`'s aborts (`hero_panic_unmatched_*`) carry a place,
    the arms being compared on the same feedback.
14. **A failure while saying where**: what prints if the renderer or the line reader faults,
    allocates past what is left, or finds `build/` gone, under panel 173's one-funnel rule
    (`panic.c:25-37`).
15. **The tripwire**: what `suite_special.hero:308-317` becomes under each S route.

## Repaired after the critic's first pass

`docs/panel/230-reports/critic-briefs.md`, 40 findings, every one taken. The proposal's text: the
column convention and `rects`' column (20), the file named by the program's module paths (6), §
13's lease sentence and `exit`'s range named as what the proposal also changes (36, 24), § 1 as
panel 159's reopening (1). Defects 2224 and 2225 filed (24, 33). The census's third kind (4), the
line table on Darwin (5), the index check's three sites and `:62` on the hot path, 19 lines in 7
modules, 59 `hero_unreachable` (3, 38), the walk that exists (8), the two notations (9), the ties
(10), the stderr binding and the compiler's own `hero_write_err` (12), the corpus leg's subject
(13), the instrument's outcomes (22), the held-out rules and the absent arm (26, 32), the seed's
sites and target (35), the tripwire (2), `main_returns`' missing golden (40), Windows' recorded
status (25), the corrections of 23 and 38. Routes added: L0 (7), L5 (37), M5 (12), M6 (27), M7
(11), and the range of `exit` (24); S1 held to one notation with the formatter (37). Questions 10
to 15 (34). The blind seat redesigned (14 to 21, 28 to 30, 39): `blind/README.md`.
