# Panel 230, the completeness critic, first pass: the briefs

Read: every file of `docs/panel/230-briefs/` (`00-shared.md`, the four seats' briefs, `critic.md`,
and `blind/` with `README.md`, `brief-p.md`, `brief-m.md`, `task-m.md`, `task-p-T.md`,
`task-p-P.md`), and the materials they name under `.claude/worktrees/panic-location-scratch/`,
copied (`rsync -a --exclude build/ --exclude '*.bin'`) into
`.claude/worktrees/230-critic-scratch/copy/`, where every program below was rebuilt and run. Nothing
was written in the scratch folder or the lane but this file.

The tree: my worktree `.claude/worktrees/230-critic`, detached at `595d92d88`. Its compiler: `clang
-I runtime seed/heroes.c runtime/runtime.c -o heroes` (real 7.88 s), then `./heroes build
selfhost/main.hero -o heroes-next` (real 87.16, user 65.91, sys 15.24, exit 0); `./heroes-next
--version` prints `heroes 0.5.0` and `sources 413da33e7969760a`, the key the shared brief names.
Every `heroes-next` below is that binary.

One disclosure: `blind230/key.txt` stands in the scratch folder beside the items, and I read it while
listing the folder (`1 M-T`, `2 P-T`, `3 M-P`, `4 P-P`). I score nothing, so it changes nothing of
mine; finding 21 is about where it stands.

Each finding: the fact as a brief states it, the command, what it printed, and what the brief should
say. A finding no command settles is written as a question. They are numbered in the order found;
the five that most change what the sitting would decide are named first, and the facts that hold are
tabled at the end.

## The five that most change what the sitting would decide

1. **M1 reopens a ratified sentence the brief says was never revisited** (finding 1). Panel 159 sat
   on *may `main` be `-> ()?`* on 2026-09-16, wrote § 1's *produces nothing* as its R3, and its
   compiler-engineer would veto `main() -> ()?` *without a specified exit code*; the brief's glob
   missed it. With findings 12 and 27: the compiler, the closure list's program, already writes its
   errors on stderr through the runtime's `hero_write_err` and ends with statuses M1 cannot give, so
   M1's ticket is the thesis alone, which family M cannot show (finding 14).
2. **The proposal is not three sentences** (finding 36): § 13 says a lease nobody ends *aborts when
   `main` returns*, measured at 134, while M1 says a returned `fail` *exits 1*; and `exit(code:
   256)` exits 0 (finding 24), a silent success inside issue 1001's own subject that the exit-status
   question has to rule on, and that reads as a `blocking` defect to file whatever is decided.
3. **L1 alone cannot make the proposed § 6 sentence true** (finding 4): fifteen `panic:` lines the
   census counts and ten it does not are written in fault handlers, at exit or in the lease reports,
   where no call site can pass a place; L2 on Darwin reads `build/`'s objects, not the binary (finding 5);
   L4's walk exists already (finding 8). The "where" decision needs a second mechanism or a narrower
   sentence.
4. **The blind seat cannot decide what the sitting asks it** (findings 14, 18, 28, with 17 and 39):
   family M's default is one case, a difference of at most one, which the README reads as none;
   family P's mistakes are visible without their stderr, against measured ceilings; no variant
   separates S1 from S2 or L1 from L4; and the case script passes a hang and a two-argument silent
   success.
5. **The payment named for the spec's sentences rests on an instrument that does not exist**
   (finding 32, with 26): no arm of Part 11's instrument varies what an abort says, the held-out
   run is not the 90 cases the briefs count, and its grader ignores stderr.

## Findings

### 1. The sentence the proposal amends was written by panel 159, which sat on `main -> ()?`; "the revisit never sat" is false

- **The brief** (`00-shared.md`, "`main` may not declare a result"): *`grep -l main_returns
  docs/panel/*.md` finds panel 035 alone, so the revisit never sat.*
- **Command**: `grep -rl main_returns docs spec selfhost tests`, then `grep -n -iE 'R3|main'
  docs/panel/159-the-document-was-not-the-only-thing-that-was-behind.md`, then `git log
  --format='%h %ad %s' --date=short -S'takes nothing and produces nothing' -- spec/heroes-spec.md`.
- **Printed**: `main_returns` is also named in `docs/panel/159-briefs/` (3 files) and
  `docs/panel/159-reports/` (4 files) and `201-reports/compiler-engineer.md`, which the brief's glob
  `docs/panel/*.md` does not reach. Panel 159 (2026-09-16, ratified the same day by standing
  delegation) was convened on three silences, the third *may `main` be `-> ()?`*. Its verdict table:
  the compiler-engineer *would veto accepting `main() -> ()?` without a specified exit code*. Its
  **R3**: *The sentence is the diagnostic's, `main` PRODUCES NOTHING*, held until defect 053 was
  repaired, home § 1's first bullet. The sentence the proposal amends was landed by
  `f20f03dce 2026-09-16 M-check-completeness step 11: three silences get sentences, and two of them
  were hiding defects`. Its llm-ergonomist registered *at least one in three first attempts at a
  fallible `main` writes `-> ()?`* (`docs/panel/159-...md:229`, *checkable today by compiling four
  lines*) and drafted, in `159-reports/llm-ergonomist.md:354-357`, the M1-shaped sentence *`function
  main()`, or `function main() -> ()?` whose error ends the program with its `code`, its ...*; its
  historian sourced Go's refusal of a result from `main` (`159-reports/historian.md:134`).
- **The brief should say**: § 1's sentence is panel 159's R3, the revisit panel 035 asked for sat
  there and refused the shape by stating it, and this sitting reopens a ratified sentence, so it owes
  what changed since 2026-09-16 (the brief names nothing that did). Panel 159's compiler-engineer's
  conditional veto is answered by M1's *exits 1* and should be put to that seat in those words. Panel
  159's ergonomist's prediction is directly scorable by family M's T session (does it write `main ->
  ()?`, refused by `main_returns`?) and should be scored there (finding 15). The historian brief
  should start from 159's historian report rather than from memory. The general lesson is the
  brief's own instrument: a glob over `docs/panel/*.md` misses every sitting's `-briefs/` and
  `-reports/` folders; `grep -rl` over `docs/panel/` is the search.

### 2. S1, S2 and S3 each turn the `special` suite red on purpose: a ratified tripwire the brief does not name

- **The brief** ("What the goldens pin"): `grep -rlE 'assert failed' tests/golden finds 9`; *An abort
  is a code that is not 0, with its message, nothing more.*
- **Command**: `grep -rnE 'assert failed|"left: |"right: ' tests/harness/*.hero selfhost
  --include='*.hero'`, then `sed -n 244,325p tests/harness/suite_special.hero`; `gh issue list --repo
  heroes-lang/heroes --state all --search 'in:title "Defect 016"'`.
- **Printed**: `tests/harness/suite_special.hero:315-317` asserts the aggregate side is ABSENT: *if
  strings.contains(..., "left:  [1, 2]") ... "an aggregate side is rendered now, so defect 016 is
  repaired and the spec sentence panel 117 narrowed has expired: read that sitting, unpin this row,
  and take the `fixedbugs` case per shape §9 owes"*. Panel 117's file calls it *a ratified tripwire*
  (`docs/panel/117-...md:292`). Defect 016 is issue 991, closed. And of the brief's 9 files, 5 are
  frozen copies under `tests/golden/recovery/corpus/`, and of the 4 live ones only
  `tests/golden/run/fixedbugs-580-a-constant-whose-assert-fails-aborts-where-it-is-read.expected`
  pins an assert line (`!panic: assert failed: 1 == 2`); the other three name `assert failed` in
  comments. `suite_special.hero:296-302` also pins `left:  4` and `right: 5` by `contains`.
- **The brief should say**: the instrument that pins today's § 12 behaviour is
  `suite_special.hero:308-317`, outside `tests/golden`; S1 (and any route that writes an array side
  as `[1, 2]`) turns it red and owes its unpinning, a `fixedbugs` case per shape, and the closed
  defect 016 (issue 991) as the item it answers. S2 (an array by its length) and S3 (a first
  differing element) leave the row green while still rendering a side, so the row's own message
  would be false under them: the sitting owes a word on what the row becomes under each.

### 3. The "29 lines" of emitted panic calls are 19 of code and 10 of tests, and the index check's abort is not among them

- **The brief** ("The emitter"): *`grep -cE '"[^"]*hero_panic[a-z_]*\('` over `selfhost/` finds 29
  lines in 9 modules that write a panic's call*; and the compiler-engineer's brief item 3 lists
  *index* among *the emitter-side sites*.
- **Command**: `grep -rcE '"[^"]*hero_panic[a-z_]*\(' selfhost --include='*.hero'` (29 in 9,
  confirmed); the same through `.claude/rules/verification.md`'s `code_of` (a module without its
  `test` blocks and comments, defect 288's lesson), for `hero_panic*`, `hero_array_at(_mut)` and
  `hero_unreachable`; `grep -rn -E '"[^"]*hero_array_at(_mut)?\(' selfhost`; `sed -n 174,178p
  runtime/parts/array.c`.
- **Printed**: in code, `hero_panic*` is written by **19 lines in 7 modules** (11 `hero_panic(`, 3
  `hero_panic_overflow(`, one each of `_assert`, `_assert_sides`, `_must`, `_unmatched_case`,
  `_unmatched_value`); the other 10 of the 29 are expected strings inside `test` blocks, and
  `container.hero` and `body.hero` hold none in code. `hero_array_at(_mut)` is written by 3 code lines,
  all in `emit/array_read.hero` (`:37`, `:62`, `:65`); `hero_unreachable` by 59 code lines in 20
  modules. The index check's message is the runtime's, `array.c:176`, `if (index < 0 || index >=
  a->len) hero_panic("array index out of range");`. In the checked form (`:37`, `:65`) the call is
  on the failing branch alone; in `array_read.hero:62`, the form an impure place takes
  (`place(..., pure: false)`, tested at `:79`), `hero_array_at_mut` is called on every access and
  does the check itself, on the path that does not abort.
- **The brief should say**: 19 code lines in 7 modules write an emitted panic call; the index check
  is a further site whose text the runtime writes, inline on one path and inside
  `hero_array_at_mut` on the other, so it is both an emitter-side site (`:37`, `:65`) and L1a, L1b
  or L1c's question (`:62`), where L1a's one more argument is paid on every impure indexed write;
  and the 59 `hero_unreachable` lines need the census of which a program's own operation reaches.
  A count of literals should be taken by `code_of`, as the rule that owns the instrument says.

### 4. Aborts with no call site to pass a place from: faults, exit-time checks and the lease reports, ten of whose lines the census does not count

- **The brief** ("The runtime"): the census is `grep -cE 'hero_panic[a-z_]*\(|hero_abort\(\)'`, 145
  lines in 19 files; it splits them into *a program's own operation* and *the runtime's own
  invariants*.
- **Command**: `grep -nE '"panic: ' runtime/parts/*.c | grep -vE 'hero_panic[a-z_]*\(' | wc -l`,
  then for each line the function it sits in and the first of `hero_abort()`, `abort()`, `raise(`,
  `return` within the thirty lines after it.
- **Printed**: 37 lines write `panic:` outside a `hero_panic*` call. Of them 27 end in
  `hero_abort()`, so the census counts them through that line: four are the entry points' own
  bodies (`panic.c:41`, `:68`, `:76`, `failure.c:195`), eight the handle reports of `alloc.c`
  (raised at a call or an `==` on a handle, so a site exists), and **fifteen are raised where no
  program call site stands**: nine in the fault handlers, `stack.c:431` and `:617-755`
  (`hero_stack_asan_veh`, `hero_stack_handler`: *stack exhausted*, *a null function pointer was
  called*, *a dead C handle was used inside C*, *a null pointer was read through*) and their
  Windows twins in `hero_stack_veh`, `stack.c:884-961`; and six in the exit-time check
  `hero_runtime_check_leaks`, `alloc.c:272-404` (*heap blocks still live at exit*, *lease(s) never
  ended*, *C handle(s) never given back*). Ten lines end by `return`,
  `raise(` or a bare `abort()`, call no `hero_abort()`, and are not in the census at all:
  `os.c:211-426` (`hero_lease_say_leases_win`, `hero_lease_veh`, `hero_lease_abrt_win`,
  `hero_lease_last_chance`, `hero_lease_crash`) and `lend.c:651-699` (`hero_lend_tell`, *C reached
  past a buffer lent to it*).
- **The brief should say**: the census has a third kind beside the two it names, aborts raised in a
  signal or exception handler or at exit, which L1 cannot locate by construction (nothing on the
  program's side calls them); only L2 (the faulting address against a line table) or L4 (the frame
  walk) can, and only async-signal-safely (the ffi-pragmatist's item 2). Each route should say what
  it prints for that kind; the proposal's § 6 sentence (*An abort ends the program at once, saying
  why and where*) is false for every one of them under L1 alone, so either the sentence narrows or a
  second mechanism is in the proposal. The census command should also count the lines that print
  `panic:` and end without `hero_abort()`.

### 5. On Darwin a binary carries no line table: the DWARF stays in `build/`'s objects

- **The brief** ("The debug information every binary carries"): *so a line table exists in every
  binary on the platforms that keep it*; L2 reads *the line from the binary's own line table, read
  in-process*.
- **Command**: after `heroes-next build report.hero -o report.mybin`: `otool -l report.mybin | grep
  -E 'sectname __debug|segname __DWARF'` and `dsymutil -s report.mybin | grep N_OSO`; `grep -rn
  'dsymutil\|dSYM' selfhost --include='*.hero'`.
- **Printed**: no `__DWARF` segment and no `__debug_*` section; two `N_OSO` entries naming
  `.../items/build/runtime-...o` and `.../items/build/tu-40c5cdcc5bf34d01/report-45bb220d779523a8.o`;
  no `dsymutil` call in `selfhost/` (three comments name `.dSYM`).
- **The brief should say**: on Darwin the line table is in the object files under `build/`, reached
  through the binary's debug map, so L2 there reads files beside the binary, fails once `build/` is
  cleaned or the binary is copied, and needs a `dsymutil` step the build does not run today. The
  shared brief's sentence should name the platform rather than hedge, and L2's cost on Darwin
  includes that step or that dependence.

### 6. The file an abort would name is the build command's path: absolute when the build was given one

- **The brief**: *the file named as a diagnostic names it (`at deep.hero:2:12` ...)*; question 7,
  *Determinism: ... so a golden can pin it; an address cannot be*.
- **Command**: `heroes-next check $PWD/endless.hero` and, from the parent, `heroes-next check
  before/endless.hero`; `heroes-next build $PWD/index.hero --emit-c -o abs.c` against `heroes-next
  build index.hero --emit-c -o rel.c`, then `grep '#line ... index.hero'` and `cmp`.
- **Printed**: `at /Users/joseph/.../copy/before/endless.hero:2:12` and `at
  before/endless.hero:2:12`; `#line 1 "/Users/joseph/.../before/index.hero"` against `#line 1
  "index.hero"`, and the two emissions differ at line 1 (`/* Generated by heroes 0.5.0 from ...`)
  and at every `#line` naming the source.
- **The brief should say**: under L1 the place record's file is the invocation path, so (a) the
  same program prints a different abort text by where it was built from, which a golden cannot pin
  across machines or across a harness that builds by absolute path; (b) a shipped binary prints the
  builder's home directory; (c) the module path of a `use`d file needs the same answer. Question 7
  should name the path beside the address, and the compiler-engineer's item 2 (*what file name it
  writes, the `#line` name or the diagnostic's*) should know the two are the same today and both
  vary.

### 7. The route the issues themselves propose, `__FILE__` and `__LINE__` at the call, is not listed

- **The brief**: *a route that passes a location never reads `__LINE__`, and leaves that choice
  standing*; routes L1 to L4.
- **Command**: `gh issue view 812 --repo heroes-lang/heroes --json body` and the same for 1155;
  `grep -n -B3 'hero_panic_must(' before/must.c`, `grep -n -B3 'hero_panic_assert_sides('
  before/assertint.c`, `grep -n 'h_index_pick' before/index.c`.
- **Printed**: 812: *The generated C carries `#line`, so `__FILE__`/`__LINE__` at each aborting
  runtime call already resolve to the `.hero` position*; 1155: *the emitted C already carries
  `#line`, so `__FILE__` and `__LINE__` at the assert site resolve to the `.hero` position and the
  runtime is not told*. Measured against them: `hero_panic_must(t8);` sits under `#line 139
  "must.c"` and `hero_panic_assert_sides(...)` under `#line 105 "assertint.c"`; the C function is
  `h_index_pick`, so `__func__` would print the mangled name.
- **The brief should say**: list it as a route (call it L0: the entry points become macros reading
  `__FILE__`, `__LINE__` and `__func__`), with the three measured reasons it fails as it stands
  (two of five classes point at the generated file on purpose, `__func__` is the C name, the file is
  the invocation path of finding 6), so the synthesis refuses the issues' own proposal on its
  measurement rather than in a subordinate clause.

### 8. L4's frame walk already exists in the runtime, and three sittings that built the stack guard are missing from the history

- **The brief**: L4, *the chain of Heroes callers by the frame walk (names only, POSIX)*; the stack
  guard named as `hero_stack_blame`, `stack.c:374`.
- **Command**: `sed -n 525,561p runtime/parts/stack.c`; `ls docs/panel | grep -E '^(104|173|199)-'`.
- **Printed**: `hero_stack_names` (`stack.c:544-561`, `HERO_STACK_SEEN 64`) walks the interrupted
  stack and collects every Heroes function on it, callee first, written for *THE RECURSION, NOT
  ONLY THE FRAME THAT RAN OUT (defect 521, panel 199's route (M), 2026-10-09)*; *On x86-64 the
  faulting function's caller is never on the chain*; *Windows has no walk*. Panels 104 (*deep
  recursion stops with a word: the guard-page handler*), 173 (*the runtime may say what it saw, and
  not why*, the lease handler's silence rule cited in `panic.c:25-31`) and 199 exist.
- **The brief should say**: L4 is the extension of `hero_stack_names` from the stack guard to every
  abort, with panel 199's measured limits (the x86-64 leaf, no Windows walk); the history section
  should cite panels 104, 173 and 199 beside 117, 131 and 132.

### 9. S1's notation for a `str` collides with the failure printer's

- **The brief**: S1 renders *in the specification's notation, a `str` quoted*; the P sentence of
  § 12, *written as this document writes a value*; the program's texts *go through `failure.c`'s
  printer (panel 192's R7: by length, a control character as `<U+001B>`)*.
- **Command**: `sed -n 110,140p runtime/parts/failure.c`; `sed -n 18,42p
  editors/lldb/heroes_lldb.py`.
- **Printed**: the failure printer writes *every character a line cannot show ... by its code
  between angle brackets, `<U+001B>`*, and leaves a line feed and a tab as themselves; the lldb
  formatter writes a `str` *with the spec's escapes (`\n \t \r \\ \"`) and `\u{1b}`*.
- **The brief should say**: two notations stand in the tree for one character, and a side under S1
  meets both: is ESC in a side `"\u{1b}"` (the spec's) or `"<U+001B>"` (panel 192's), is a line feed
  inside a side `\n` or a new line, and do `.must()`'s code and message, unquoted today, change with
  it. The spec-warden's item 5 and the compiler-engineer's item 5 should both be asked it.

### 10. Two sides that render the same text: it happens today, and S1's bounds add more

- **The brief**: S1 is bounded (16 elements, 1,024 bytes, 256 values, depth 6) and orders a map's
  entries by their rendered text.
- **Command**: `assert x == x` with `x = 0.0 / 0.0`, built with `heroes-next` and run; `assert s ==
  ""` with `s = "a\"b"`.
- **Printed**: `assert failed: x == x`, `left:  nan`, `right: nan`, exit 134; and `left:  a"b`,
  `right: ` (an empty side, as the brief says).
- **The brief should say**: ask what each S route prints when the two sides render alike, which
  `nan` does today and which every bound adds (two arrays differing past element 16, two records
  past depth 6, two texts past byte 1,024); and how the map order breaks a tie between two keys
  whose rendered texts are equal (two keys sharing their first 1,024 bytes), since a tie broken by
  bucket order brings back the non-determinism panel 117 measured. S3 (a diff) answers the first;
  nothing listed answers the second.

### 11. Neither M1 nor M2 changes the program issue 1001 measured

- **The brief**: M1, *`main` may produce `()?`; a `fail` it returns is written on stderr ... and
  the program exits 1*; issue 1001 is *what `main` tells the shell*.
- **Command**: `gh issue view 1001 --repo heroes-lang/heroes --json body`; `heroes-next build
  before/handled.hero` and its run.
- **Printed**: 1001: *A program that receives a `fail`, matches it and prints it exits **0***, and
  *the language has a way to report failure and no way to oblige it*. `handled` prints `error:
  empty: no age given` on stdout and exits 0 today; M1's words change nothing for a `main` that
  declares no result, so by the proposal's text it would do the same (a reading, not a run: no
  compiler builds M1 yet).
- **The brief should say**: M1 adds a second way to report and still obliges nothing: the measured
  program keeps exiting 0. Whether a writer reaches for `-> ()?` is the whole effect, which is family
  M's task 2 (one case, finding 14). A route nobody listed is the obligation itself (a `main` whose
  `.err` arm falls off its end is told so), and another is a stderr writer with `exit(code:)`
  unchanged (M5, below); the sitting should say why each is or is not taken.

### 12. A route nobody listed for issue 1001: a way to write on stderr, `main` unchanged

- **The brief**: *no built-in writes to stderr* (`grep -n -iE 'stderr|eprint'` over the spec and
  `inventory.hero` find nothing).
- **Command**: the same greps (0 and 0, `library_source.hero` 0, confirmed); then a program
  binding `extern "unistd.h"` `function write(fd: i32, buf: cstr lent, count: u64) -> i64`, calling
  `write(fd: 2, buf: text.cstr(), count: to_u64(text.len()).must())` with `text = "no port\n"` and
  then `exit(code: 1)`, built with `heroes-next` and run; one binding `extern "stdio.h"` `function
  perror(s: cstr lent)`; one binding the runtime's own writer as the compiler does, `extern
  "hero_os.h"` `function hero_write_err(text: str)`; `sed -n 1,43p selfhost/cli/io.hero`; the
  `io.eprint(` and `io.eprint_raw(` lines of `selfhost/` through `code_of`.
- **Printed**: confirmed for the built-ins. The `write` program: `exit 1`, stdout empty, stderr
  `no port`. The `perror` one: `exit 1`, stderr `no port: Undefined error: 0`. The
  `hero_write_err` one: `exit 1`, stderr `error: no_key: port`. Without `lent` the `cstr` binding
  is refused, `error[lend_kept]`, and a literal passed as `cstr` is `error[type_mismatch]: expected
  cstr, found str`. `selfhost/cli/io.hero:12-32`: the compiler *used to bind POSIX's write(2)
  directly*, which put `#include <unistd.h>` in the seed, *and neither MSVC nor clang-cl ships that
  header*, so it binds `hero_write_err` of `hero_os.h` (`runtime/hero_os.h:108`), over C89's
  `fwrite(..., stderr)`; the compiler writes through it from 88 code lines in 21 modules
  (`io.eprint` and `io.eprint_raw`, counted by `code_of`, tests and comments left out), and its `main`
  ends by `exit(code:)` with 0, 2 and the codes `cli_table.exit_code` gives
  (`selfhost/main.hero:99-225`).
- **The brief should say**: no built-in writes to stderr, but a program does today in three lines
  of FFI, portably only through the runtime's `hero_write_err` (the `write` route is POSIX's alone,
  as io.hero measured), so *what a program that handles its own failure has* is stdout,
  `exit(code:)` and a binding of the runtime's header. That bears on Principle 0 twice: the
  closure-list program writes its errors on stderr and ends by `exit(code:)` with a status of 2 that
  M1 cannot give, so the compiler would not use M1, while it already uses what M5 would name; and
  what M1 adds over `hero_write_err` plus `exit(code: 1)` is the default and the shape, not the
  capability. The held-out tasks forbid `extern` (`docs/metrics/tasks.md:55`), so there the
  capability is absent. List M5: a built-in that writes a line on stderr (or `exit(code:, msg:)`),
  leaving `main` as it is, which gives task 1 and task 3's programs a place to say why without
  touching `main`'s shape and which the compiler itself would use; and say whether binding
  `hero_os.h` from a program is a supported route or an accident the sitting should close.

### 13. The corpus leg times the builds, not a program's path that does not abort

- **The brief** (question 1, compiler-engineer item 4): the cost is *a program's path that does not
  abort, measured ... and the corpus leg's time*, `./heroes run tests/harness/main.hero --
  ./heroes-next corpus`.
- **Command**: `sed -n 1,40p tests/harness/suite_corpus.hero`; `grep -n -E 'run_guarded|"build"'
  tests/harness/suite_corpus.hero`; then one example timed, `/usr/bin/time -p ./heroes-next build
  examples/interpreter/main.hero -o interp.bin` and `/usr/bin/time -p interp.bin
  examples/interpreter/scripts/tour.script` (its `main.args`), on a machine at load 3.7 with the
  other seats working, so a ratio and not a duration.
- **Printed**: the corpus runs `heroes test` in three configurations, `heroes build`, `heroes run`
  and `heroes run --sanitize` (`suite_corpus.hero:347-370`) over the examples. The one example:
  build `real 1.23 user 0.90 sys 0.20`; run `real 0.20 user 0.00 sys 0.00`, its output
  `main.expected`'s but for the `!exit: 0` line. Each example is built four times or more and run
  about twice, so the leg's time is the builds' (one example measured; the rest an inference).
- **The brief should say**: the corpus leg measures what the place records cost the build (more C,
  more read-only data), which is worth having; the path that does not abort needs a program named
  for it. The one with the most abort sites on a hot path is the compiler itself: `heroes-next build
  selfhost/main.hero` timed with the stage 2 of the tree before and after (CLAUDE.md, *never slow
  the compiler down*), which is also where L1a's one more argument on `hero_array_at_mut`,
  `hero_str_slice` and the rest is paid. Two numbers, not one: `--emit-c` alone times the compiler
  as a program on its path that does not abort, and the whole build adds clang over C that carries
  the place records (finding 35 counts the sites).

### 14. Family M: the one case that measures the default cannot show a difference by the README's own rule

- **The README**: *task 2 states none ... and is the one task that measures the default*; the
  silent count over *the eight cases whose expected status is not 0 (task 1's four, task 2's one,
  task 3's three)*; *a difference of one item is no difference this sample can show*; a veto at *two
  or more more silent cases than T*.
- **Command**: for each case of `blind230/m/cases/`, its `args`, `status` and `stdout`.
- **Printed**: `sum/3,4,5,6` nonzero (task 1, whose text says *exit with a status other than 0*),
  `capital/2` nonzero (task 2, the only one), `port/2,3,4` nonzero (task 3, *a script must be able to
  tell*). Counts as the README says.
- **The brief should say**: 7 of the 8 silent-case chances are tasks whose text states the status,
  so they measure instruction-following, not the default; the default is one case in one session per
  variant, its largest possible difference 1, which the README's rule reads as no difference. So
  family M cannot show issue 1001's effect, and its veto can be triggered only by the tasks that
  state the status. Either the default needs more than one case and one session (more task-2-shaped
  tasks inside the same four sessions, e.g. task 2's shape over three different programs), or the
  README should say before the run that family M reads instruction-following and that the default is
  unmeasured.

### 15. Family M: a T answer written `main -> ()?` does not build, is not silent, and is not reported as what it is

- **The README**: *A program that does not build counts as no case passed and is reported with its
  first diagnostic*; the silent count is the cases that *ended with status 0*.
- **Command**: `heroes-next check m1.hero` on `function main() -> ()?` returning `fail(...)`.
- **Printed**: `error[main_returns]`, *`main` produces nothing, and this one is declared `-> ()?`,
  a program reports failure by what it prints, not by what it returns* (the compiler's dash written
  here as a comma).
- **The brief should say**: a T session that reaches for M1 lowers T's silent count (it ends at no
  status) and T's passes, so the two scores move in opposite directions for the reason the proposal
  exists. Count `main_returns` refusals in each variant as a reading of their own, written before the
  run; it is panel 159's ergonomist's prediction (finding 1), and it is the most direct evidence the
  seat can give for M1.

### 16. Family M's P side has no compiler and no reference yet

- **The README**: P answers are built *by the prototype compiler that accepts `main -> ()?` (P; the
  compiler-engineer's, or the coordinator's where that seat's is not built when the answers
  return)*; *the coordinator's references pass them all*.
- **Command**: `heroes-next check` of the `-> ()?` program above (refused); `ls blind230/m/ref/`;
  the three references built and run by `run-cases.sh`.
- **Printed**: refused at `595d92d88`; `ref/` holds `sum.hero`, `capital.hero`, `port.hero`, each
  T-shaped (`exit(1)`, the capital's message on stdout), and they pass `sum: 7 of 7`, `capital: 3 of
  3`, `port: 4 of 4` (confirmed). No P-shaped reference exists, so no case has been run against a
  program that ends by returning a `fail` from `main`.
- **The brief should say**: name which compiler judges P's answers before the run and what happens
  if none exists when they return (the family's P side void, or built by hand to M1's words and
  said so); and run the cases once against a P-shaped reference on that compiler before the sessions
  start, as was done for T.

### 17. `run-cases.sh` passes a program that hangs

- **The README**: the cases judge the status; *the count that ended with status 0*.
- **Command**: a capital program whose `.err` arm loops (`while true`), built with `heroes-next`,
  then `sh run-cases.sh hang.bin capital < /dev/null` under `/usr/bin/time -p`.
- **Printed**: `capital case 2: exit 124, pass`, `capital: 3 of 3`, real 10.24 (the script's
  `timeout 10`); `pgrep -f hang.bin` found nothing afterwards.
- **The brief should say**: a timeout's 124 is counted as *nonzero*, so a program that never ends on
  an error passes and is not silent. The script should fail a case at 124 and report it apart, and
  the README should decide, before the run, whether an abort's 134 on a case that wants a status
  other than 0 passes (the script passes every status but 0, `run-cases.sh:10`, and the task text
  allows it).

### 18. Family P will most likely read 4 of 4 in both variants: every planted mistake is visible without its stderr

- **The README**: the two variants *differ in the stderr alone*; a veto at P *two or more items
  below T* on *fixed*.
- **Command**: the four items read against their references (`diff items/<f>.hero
  items/ref/<f>.hero`, one line each, confirmed); `ls docs/measurements | grep -E '^05[234]'`.
- **Printed**: the four planted lines are `while i + 2 <= xs.len()` (an off-by-one against an
  expected output of four windows), `item: "chese"` (line 26 of a 28-line file whose line 8 is
  `c["cheese"] @ 480`), `y1:
  lower(a: r.y0, b: r.y1)` (one line below `x1: upper(...)`), and `h @ h * 31 + v` under a header
  comment saying *modulo 1,000,000,007*. The recent blind readings ran at their ceiling: measurement
  053 *27 of 27 accepted at the first try*, 054 *60 of 60 programs accepted*, 052 *no discordant
  pair*.
- **A question, since no session is mine to run**: with the T variant's abort class narrowing each
  program to one plausible site, is there any outcome of family P other than 4 and 4 on *fixed*? If
  not, the family can neither support nor veto, and the four sessions' one family P pair buys
  nothing a reader of the items cannot say now. What would make it measure: items where the class
  names many plausible sites and only the line separates them (a longer program, several lookups by
  computed keys, two accumulators), or a reading that is not at ceiling (each session's tokens
  before its first `### Item`, which `run.jsonl` holds, per family).

### 19. Family P's *located* rule partly scores whether the model quotes the line the stderr names

- **The README**: *located* when the `cause` quotes the planted line: `report` the `while`
  condition, `basket` the call holding `"chese"`, `rects` the line computing `y1`, `checksum` the line
  `h @ h * 31 + v`.
- **Command**: the P locations counted against the item files (finding 20).
- **Printed**: P names line 13 for `report` (the planted line is 12), line 18 for `basket` (the
  planted line is 26, in `main`), line 35 for `rects` (planted 26), line 11 for `checksum` (the
  planted line). And `checksum`'s mistake is *the modulus applied after the loop* (the README's
  words), so `return h % MODULUS`, line 13, is an equally true *line that is wrong*, which the rule
  does not accept.
- **The brief should say**: per item, whether *located* measures that the location helps
  (`checksum`, where P's line is the planted one) or that it does not mislead (`report`, `basket`,
  where P points elsewhere); and accept `checksum`'s line 13 too, or score that item's *located* on
  the fix alone. As written, P gains on `checksum` and risks losing on `basket` for the same reason,
  the pointer, and the two cancel in a sum of four.

### 20. The hand-written P locations: the lines are right, the columns follow two conventions

- **The README**: P's stderr is hand-written, *nothing printing it yet*.
- **Command**: line and column counted in each item file; `heroes-next build checksum.hero --emit-c`
  and its `#line`s; the overflow found by `python3 -I -c` iterating `h = h * 31 + v` from 7 over
  1..39 against 2^63 - 1.
- **Printed**: `report.hero:13:44` is the first character of `xs[i + 2]`, the read that fails at
  `i = 4`; `basket.hero:18:12` the first character of `prices[item].must()`; `checksum.hero:11:13`
  the first character of `h * 31`, and the overflow is that multiplication at `v = 13` (*overflow at
  v= 13 in h*31*), the emitted line being `#line 10` plus one; but `rects.hero:35:9` is the `assert`
  keyword, where the expression starts at column 16. Read through the emitted C's `#line`s (a
  Python reader of `report.my.c`, `basket.my.c`, `rects.my.c`): `report.hero` line 13 holds three
  `hero_array_at(` calls, so only the column tells the failing read from the two that did not
  fail; line 23, `for w in windows(readings)`, holds a sixth index check the author never wrote and
  the README's grep does not count; `hero_panic_must(` sits under `basket.c` 158 and 184 and
  `hero_panic_assert` under `rects.c` 188, so P's lines 18 and 35 are what the IR would carry, not
  anything the C says today.
- **The brief should say**: the four lines agree with the programs; the column is the failing
  sub-expression's start in three items and the statement's start in the fourth. Question 3 of the
  shared brief (*which column*) is answered by the P variant two ways; say which one the proposal
  means before the run, so the session does not read a convention the compiler will not print.
  `report` is the item where a line without a column would name three reads, which is the
  measured case for the column; and a `for`'s own element read is a site the census and question
  2 should name (what place, what words, if it can fire at all).

### 21. The blind run: the working directory, the confound, and the key beside the items

- **The README**: the command line (no `cd` shown); the scoring *before the key is read*; *the two
  variants differ in the stderr alone*.
- **Command**: `claude --help` (2.1.308; no session started) for `--restricted`; `ls
  blind230/`.
- **Printed**: `--restricted` *confines the file tools to the working directories*; the README's
  command sets a system prompt naming `<build/b230/N>` and `GIT_CEILING_DIRECTORIES`, and shows no
  `cd`. `key.txt` stands in `blind230/` beside `items/` and `m/`. In family P the P variant changes
  the spec and the stderr together.
- **The brief should say**: the command is run from inside `build/b230/N` (a `cd` in the line), since
  from the lane's root `--restricted` would let a session read the lane, the context check would void
  it after the fact, and with one session per cell a void session loses its family. The key should
  stand where the scorer does not list it until scoring ends. Family P's *located* needs a reader
  (quote matching) who can see the variant in `task.md` itself, so its blindness rests on the rule
  being mechanical: say so. And a family P difference cannot be split between the two sentences of
  § 6 and § 12 and the stderr; say that the family reads the whole proposal.

### 22. Part 11's instrument names a failed `assert` a crash, not an abort

- **The brief**: *the whole of stderr, trimmed, on an abort (outcome `abort`)*.
- **Command**: `sed -n 250,264p tests/harness/thesis_grade.hero`; `sed -n 155,158p
  tests/harness/expectation.hero`.
- **Printed**: `abort` is `ran.code >= 128` with `panic:` in stderr; otherwise at 128 or above,
  `crash`, *"ended with status " + ran.code.to_str() + "\n" + ran.err*, untrimmed. A failed `assert`
  writes `assert failed:`, no `panic:`, so it is `crash`. A wrong status below 128 is
  `compiled_wrong`, whose text carries the stderr (`its stderr was:`).
- **The brief should say**: stderr reaches turn 2 on every non-zero exit, under three outcome names;
  S1's sides arrive as `crash`, and a prediction on turns-to-green over programs that abort should
  name both outcomes.

### 23. Smaller corrections, each measured

- *`exit(3)` once* (examples): the one match is a comment, `examples/interpreter/main.hero:214`
  (`grep -rnE '\bexit\(' examples | grep -E ':[0-9]+:\s*#'`); the calls are `exit(1)` 18, `exit(2)`
  9 and `exit(f.code)` 1.
- *48 lines naming `exit(code` across `selfhost/`*: 14 are the calls, all in `selfhost/main.hero`,
  20 are comments and 14 sit inside strings (`grep -rn 'exit(code' selfhost`, filtered).
- *Panel 117 (2026-09-06, ratified 2026-09-07)*: its file says *Convened 2026-09-07* (line 3) and
  it was added in `14eab40b0`, 2026-09-07.
- *`runtime/parts/sort.c:120-134` orders twelve scalar descriptors*: that is panel 117's citation;
  today the dispatch is `hero_cmp_for` at `sort.c:130-143`, still twelve descriptors and no record.
- *design.md §4.18 (`docs/design.md:2433`) ... showed `assertion failed at line 12`*: `2433` is the
  heading; the text is at `:2460`.
- *row 116 gave 2 back*: row 116 says *against 10,299 at row 115 (-2); no addition, so no payment is
  owed*; that the 2 return to the consolidation block is an inference. And design.md `:292-293`
  binds the 1,500 *before the thesis harness has run*; M-thesis-harness closed 2026-10-24 with
  measurement 052, the first held-out run. **A question for the spec-warden**: does the block's cap
  still bind this sitting, or has its condition passed and the next raise's rule taken over?
- *the ceiling 16384*: `heroes-next measure spec/heroes-spec.md` prints the FFI floor's 60 (panel
  030 R3) beside it, *what is measured against the ceiling is 10357*.
- `selfhost/emit/abort.hero:7` says a wrong entry point is *a wrong message, at exit 1*; every abort
  measured here exits 134. A comment, but question 8 (*the exit statuses*) meets it.

### 24. `exit(code: 256)` tells the shell the program succeeded: a silent success in issue 1001's own subject

- **The brief** (question 8, *the exit statuses*; M2, *`main -> i64`, the exit status*; M3, *`exit(code:)` is the route*).
- **Command**: four programs `print("before")` then `exit(code: N)` for N in 256, -1, 512, 1, built with `heroes-next`, each binary run and `$?` read.
- **Printed**: `exit(code: 256) -> status 0`, `exit(code: -1) -> status 255`, `exit(code: 512) -> status 0`, `exit(code: 1) -> status 1`; nothing on stderr for any of them. The spec says only *`exit(code: i64)` (ends the program)* (`spec/heroes-spec.md:337`).
- **The brief should say**: on POSIX a status is the code modulo 256, so a program that asks for a failure it numbers 256 or 512 reports success, silently: issue 1001's class (a failure told to the shell as success) reached through the route M3 keeps. It is a wrong value with no message, so by `.claude/rules/verification.md` § Bounded discovery it reads as a `blocking` defect to file, whatever the sitting decides. M2 inherits it (a `main` returning 256), and Windows, whose statuses are 32 bits wide, would differ (an inference, unrun). The sitting should rule what `exit(code:)` does with a code outside 0 to 255 (refuse at compile time where constant, abort otherwise, or clamp to a failure), since question 8 asks whether the specification says what each status is.

### 25. What an abort exits with on Windows was measured once, by panel 174's brief

- **The brief** (question 4): *What an abort exits with on Windows is a question (134 is SIGABRT on POSIX; nothing pinned)*; the ffi-pragmatist's item 5, *on Windows (a question unless the CI's log or the box says it)*.
- **Command**: `grep -rn -iE '0xC0000409|fastfail|STATUS_STACK_BUFFER_OVERRUN' .` (worktrees excluded); `sed -n 120,145p docs/panel/174-briefs/00-shared.md`; `sed -n 237,248p runtime/parts/os.c`.
- **Printed**: panel 174's brief (convened 2026-09-21), a probe on the author's box: `abort-array-index exited 0xC0000409` (`STATUS_STACK_BUFFER_OVERRUN`) and three heap-corrupting cases `exited 0xC0000374`. `os.c:241-247`: *The UCRT's `abort` calls `raise(SIGABRT)` only when a handler is installed, and then `__fastfail` ... the fail-fast the process always died of.*
- **The brief should say**: an abort on Windows exited `0xC0000409` on 2026-09-21 (panel 174), by `__fastfail`, which the runtime's handler of panel 175 keeps (`os.c:241-247`); a re-run on the box is owed before the number is written as today's, but the question has an answer in the record. The spec states no abort status (`grep -n -iE 'status|134|exit' spec/heroes-spec.md` prints the two `exit(code:)` lines alone), so 134 and `0xC0000409` are both platform facts and neither is the language's.

### 26. The held-out run is not the 90 cases the briefs count, and its grader ignores stderr

- **The spec-warden's brief** (item 2): *40 of the task suite's 90 cases demand an exit status that is not 0 (`00-shared.md`), so a prediction on silent error paths is scorable on the same run* (the next held-out run).
- **Command**: `find tests/thesis -name '*.expected' | awk -F/ '{print $3}' | sort | uniq -c`; `sed -n 1,25p tests/thesis/held-out/README.md`; `sed -n 40,90p docs/metrics/tasks.md`.
- **Printed**: the 90 cases are nine each of the ten visible tasks `01` to `10` (*the ten assistant-written tasks in clear*); the held-out run grades `suite.enc`, *40 tasks ... 4 to 8 hidden cases*, sealed. `docs/metrics/tasks.md:58-61`: *stderr is ignored*; every task has *one malformed with the exit code the prose names*; `:68`: *the argument count and an unreadable input, with their exit codes* are pinned in prose.
- **The brief should say**: the counts of `00-shared.md` describe the visible suite, not the run a prediction would be scored on; the held-out suite's statuses are sealed, but its writing rules say every task names its error statuses in prose, so the held-out run measures task 1's shape (a status stated), never task 2's (the default), and its grader never reads what M1 writes on stderr, which reaches a model only as turn 2's feedback. A prediction on *silent error paths* has to be written knowing that.

### 27. M1's one status does not serve the statuses the task suite asks for

- **The brief**: M1, *a `fail` it returns is written on stderr ... and the program exits 1*.
- **Command**: `find tests/thesis -name '*.expected' | xargs grep -h '^!exit:' | sort | uniq -c`.
- **Printed**: `27 !exit: 1`, `12 !exit: 2`, `1 !exit: 3`.
- **The brief should say**: 13 of the visible suite's 40 failing cases ask a status M1 cannot give, so a writer who takes `main -> ()?` still needs `exit(code:)` for them, and a model that reaches for M1 where the prose names 2 fails the case. A prediction for M1 should name both directions: fewer silent error paths, and possibly more wrong statuses where 2 or 3 is asked. M2 and a `fail` carrying its status (a route nobody listed: the code of the failure mapped to a status, or `main -> i64?`) are the shapes that serve both; the sitting should say why each is or is not taken.

### 28. The four sessions weigh the proposal against today, and no route against another

- **The README**: *whether the proposal's text changes what a model can do when a program has failed*; the shared brief lists L1 to L4, S1 to S4, M1 to M4.
- **Command**: `sed -n 5,9p blind230/items/rects.hero` (the record's fields); the P stderr of each item (`items/<f>.err.P`).
- **Printed**: `Rect` is four `i64` fields, so its sides render identically under S1 and under S2 (*records and variants of scalars alone*, panel 131's conservative variant); no item has an array or a map side, where S1 and S2 differ. P's four location lines are L1's alone; L4's caller line (`basket.main`, the function holding the planted `"chese"`) and S3's diff (`left.y1: 1`, `right.y1: 4`) appear in no variant; family M has M1 and M3 and no M2 or M4.
- **The brief should say**: the seat can say whether the proposal helps against nothing, not which route helps: it cannot separate S1 from S2, the choice panels 117 and 131 fought over, nor L1 from L4 on the one item (`basket`) where they print different functions. Either one item gets an aggregate side that S2 shows by its length (an array of records, a map), with S1 and S2 as two P variants inside the four sessions, or the sitting records before the run that the seat's result bears on the proposal as a whole and on no route against another.

### 29. The README's command writes its transcript into the session's own folder

- **The README**: the folder holds *`heroes-spec.md` (its variant's copy), `brief.md` (its family's) and `task.md`, nothing else*; the command ends `> run.jsonl 2> run.err`.
- **Command**: `sed -n 1,20p docs/panel/228-reports/blind/run_one.sh`, panel 228's script for the same command.
- **Printed**: panel 228's script does `cd "$d"` and writes `> "$root/b-out/$n.jsonl" 2> "$root/b-out/$n.err"`, outside the session's folder.
- **The brief should say**: run from inside `build/b230/N` (finding 21), the README's relative redirect creates `run.jsonl` and `run.err` in that folder before the session starts, so the folder holds five files, two of them the session's own transcript, which `Read` can open; write them to a sibling folder as panel 228 did, and show the `cd` in the command.

### 30. "Written before the run" has no trace yet

- **The README**: *Scored before the key is read, by the rules below, written before the run*.
- **Command**: `git status --short docs/panel/` in the lane; `shasum -a 256` of the briefs and the scratch's files.
- **Printed**: `?? docs/panel/230-briefs/` and `?? docs/panel/230-reports/`: the pre-registration is an untracked file, editable after the run without a record. At this reading: `blind/README.md` `832dc547a343e7f4d4e4739acff0d04ea9468d2b7f72c95e1914ba819a88f738`, `blind/task-p-T.md` `170078bc48cc9d93fbc54581a69f5a3769d0abff022aad824675f8576b802672`, `blind/task-p-P.md` `ef84d96860c84b783c12fa841d628b3369d57b3ec54ad196eda0c40ad45fbb73`, `blind/task-m.md` `59289438d5dfe976781fc840ec5ea22ed55165c69faf8721824340b0e20c4132`, `blind/brief-p.md` `59fd7a48a31e9040b1e3bc81cf35b0d3c82654c4255759d5393b30af6ba1fb47`, `blind/brief-m.md` `0b4299bd2a9039971d9c1d4a6def8c0b300b0f554079a374e9025e53d423097d`, `blind230/m/run-cases.sh` `d1ad714336c7e9caee489cda846eec5f288c9c95c9bd46daffbb6ea49e000ceb`, `blind230/spec-T.md` `5b69e6a8c6b327d4f47fd0c7ad327b489a93403d1a430dc2df486da5d3c2f3a1`, `blind230/spec-P.md` `aa11f940102ed438e68d9217c736fd969fa8880972e4fbc947ae59c600e0ff5e`.
- **The brief should say**: the scoring rules and the case script are fixed by a commit (or by digests the sitting's file carries) before the first session starts, and amended after it only underneath, dated; and since findings 14 to 19, 28, 29 and 39 ask for changes to them, the digests above are the version this pass read, not the version to run.

### 31. `flags()` carries no blanket `-Werror`

- **The ffi-pragmatist's brief** (item 1): *compiled under `selfhost/cli/flags.hero`'s `flags()` with `-Werror`*.
- **Command**: the `-W` words of `flags()` in `selfhost/cli/flags.hero` (the function at `:130`).
- **Printed**: `-Wall`, nine `-Werror=` words (`return-type`, `uninitialized`, `format`, `conditional-uninitialized`, `shorten-64-to-32`, `sign-conversion`, `incompatible-pointer-types-discards-qualifiers`, `incompatible-pointer-types`, `macro-redefined`) and `-Wno-infinite-recursion`; no `"-Werror"`.
- **The brief should say**: *under `flags()` as written, and again with `-Werror` added*, so a warning the tree's build would let through (and the `warnings` suite would judge) is told apart from one only the stricter build refuses.

### 32. No arm of Part 11's instrument varies what an abort says, so "scorable at the next held-out run" needs an instrument that does not exist yet

- **The spec-warden's brief** (item 2): *Part 11's instrument feeds stderr back at turn 2 ..., so a prediction on turns-to-green over programs that abort is scorable at the next held-out run*; design.md §1.6 asks *a registered prediction naming an instrument that exists*.
- **Command**: `sed -n 50,53p tests/harness/thesis_args.hero`; `grep -n -- '--compiler' tests/harness/thesis_args.hero`; `sed -n 5,14p tests/harness/thesis_run.hero`.
- **Printed**: `constant ARMS: [str]` is `["strict", "brief", "control"]`, three arms that differ in what the compiler says and builds, none in what a running program writes; a run takes one `--compiler`; *the feedback is symmetric across the arms (the sitting's R5)*.
- **The brief should say**: inside one run every arm gets the same abort text, so the location's effect on turns-to-green is not a difference any arm measures; it can be read only across two runs with two compilers (measurement 052's, before; the next, after), where run-to-run variation is unmeasured, or by a new arm (the place switched off for one arm, by a flag the runtime reads) or by replaying 052's kept turn-1 generations under the new compiler. Which of these is the instrument, and whether the instrument can replay generations under another compiler, is **a question** the payment rests on; until it is answered the prediction names an instrument that does not exist.

### 33. Today a `str` side holding a line feed forges a line of the report

- **The brief**: S1 writes a side *in the specification's notation, a `str` quoted*; the failure printer passes the line feed and the tab as themselves (finding 9).
- **Command**: `s = "a"`, `t = "a\n  right: a"`, `assert s == t`, built with `heroes-next` and run.
- **Printed**: `assert failed: s == t`, `  left:  a`, `  right: a`, `  right: a`, exit 134: two `right:` lines, and no way to tell which text was the side. The same with a tab: `right: r<TAB>q` written raw.
- **The brief should say**: this is the measured case for quoting a `str` side (S1) against panel 192's rule that leaves the line feed raw, and it decides finding 9's question in one direction for sides; whether the `.must()` line (`code: msg`, also the program's texts, also raw line feeds) follows is the same question. A report a program's own value can forge is a false message in design.md §4.17's sense, so the class reads as a defect whichever route is taken.

### 34. Questions the sitting should ask and does not

Each is unrun here; each names what would settle it.

- **A thread**: `runtime/parts/spawn.c` holds 7 abort lines and the stack guard keeps its bounds per thread (`stack.c:276-277`, `_Thread_local`). What does each route print for an abort on a thread a program started, and where does L4's walk stop? Settled by a program that starts a thread which indexes past an end, under the built route.
- **An optimised build**: the items were built at `-O0` (`build/words/*.rsp` holds `"-O0"`); at `-O2` (`-gline-tables-only`, `flags.hero:169-172`) inlining folds a Heroes function into its caller, so L2's line and L4's names can name the caller where L1's static record still names the site. Settled by building `report.hero` at `-O2` under each prototype.
- **`heroes test`**: the "before" shows the `FAIL "<title>"` lines on stdout and the `assert failed:` lines on stderr (`before/tested.hero`), two streams with nothing tying a failure to its test but their order. Does the place line tie them, and is a test's function its title (question 2)?
- **The control arm**: `hero_panic_unmatched_case` and `hero_panic_unmatched_value` exist only in a `--permissive` build (`panic.c:48-80`); each route should say whether the control arm's aborts carry a place, since the instrument's arms are compared on the same feedback.
- **A failure while saying where**: under L2 the reader of the line table (finding 5) and under S1 the renderer run on the failing path, after the flag `hero_runtime_spoke` is meant to be set (`panic.c:25-37`, panel 173: a second, false `panic:` line under a live lease); what prints if that code itself faults, allocates past what is left, or finds `build/` gone?

### 35. The seed carries 17,297 abort call sites, and its size has a target the brief's cost question does not name

- **The brief** (question 1; compiler-engineer item 4): *the binary's size of a program with many abort sites (the compiler itself, `heroes-next`, before and after)*; question 9, the seed's route.
- **Command**: `wc -lc seed/heroes.c`; `grep -c '^#line' seed/heroes.c`; `grep -oE 'hero_panic[a-z_]*\(|hero_array_at(_mut)?\(|hero_unreachable\(' seed/heroes.c | sort | uniq -c`; `sed -n 26,34p docs/roadmap/milestones/M-bounded-seed.md`.
- **Printed**: `271387 27000911 seed/heroes.c`; 57,860 `#line` lines; call sites `hero_panic_overflow(` 7,570, `hero_array_at(` 6,828, `hero_panic_must(` 2,382, `hero_panic(` 278, `hero_array_at_mut(` 170, `hero_panic_assert_sides(` 37, `hero_panic_assert(` 30, `hero_panic_unmatched_case(` 1, `hero_panic_unmatched_value(` 1, that is 17,297, beside 23,996 `hero_unreachable(`. M-bounded-seed's page: *The target is a number: a seed under 10 MB*, by an emission profile that writes the seed without `#line` directives.
- **The brief should say**: L1 writes a place record (or a reference to one) at each of these sites, so the seed, a tracked file with a size target, grows by a measured amount the sitting should know before it rules; and M-bounded-seed's profile, which drops the `#line`s because nobody steps through the seed, has to say whether it drops the places too (a seed-built compiler that aborts would then say less) or keeps them. The compiler-engineer's item 4 should name `seed/heroes.c`'s bytes before and after beside `heroes-next`'s.

### 36. Under M1 two sentences of the specification disagree about a `main` that returns a `fail` with a lease live

- **The brief**: *Three sentences, the whole of the proposal's text; everything else it changes is behaviour no sentence of the specification states.*
- **Command**: `grep -n -E '\bmain\b' spec/heroes-spec.md`; then two programs that take `x: cstr @= s.lease()` and never end it, one leaving by `exit(code: 1)`, one by returning from `main`, built with `heroes-next` and run.
- **Printed**: `spec/heroes-spec.md:426-427`, § 13: *a lease nobody ends, like a handle nobody consumes, aborts when `main` returns, saying how many.* The `exit(code: 1)` program: `exit 1`, stderr empty (the exit-time check is bypassed, as `tests/harness/suite_corpus.hero:28-31` says it is by design). The returning one: `exit 134`, `panic: 1 lease(s) never ended -- every .lease() owes one end_lease, and this program is missing that many`.
- **The brief should say**: § 13 states behaviour M1 changes: a `main` that returns a `fail` has returned, so § 13 says it aborts (134, a `panic:` line), while the P sentence of § 1 says it *exits 1* with the failure's line. The proposal has to say which (the check run first, its abort winning; or the failure's exit taken as `exit(code:)` takes it, the leak unreported), and the spec-warden's item 5 should price the words that say it. The same question stands for a `?` that leaves `main`.

### 37. Two more routes nobody listed

- **L5, the place resolved off-line.** The abort prints the image-relative address of the site (the entry point's `__builtin_return_address(0)`, or the faulting `pc` a signal handler already holds, `stack.c:544-561`) beside the binary's sources key (`hero_program_sources`, panel 222, which `--version` prints for the compiler), and a `heroes` verb resolves it against the build's debug information. Nothing is passed and nothing computed on the path that does not abort, no entry point changes shape, and printing a number is `write`, so it reaches finding 4's fault class async-signal-safely where L1 cannot. What it costs: a verb (`.claude/rules/cli-surface.md`'s stopping rule), `build/` kept on Darwin (finding 5), and an address a golden pins only as an offset of a reproducible link (`selfhost/cli/link_flags.hero:100-116`: `-reproducible` gave one binary for two links). Unrun; what would make it right is the offset measured equal across two builds of one program and the resolver measured on the three platforms.
- **S1 is a second printer, and the first already drifted.** The lldb formatter renders a value (`editors/lldb/heroes_lldb.py`), the failure printer renders the program's texts, and finding 9 measured them writing one character two ways. Panel 117's historian registered *if a renderer reachable only from `assert` lands, the first non-test consumer adds a **second** printer rather than reusing it* (`docs/panel/117-...md`, the verdict table). The sitting should say how S1's renderer and the formatter are held to one notation (one corpus of values both render, compared by a test), or which of them the other is generated from.

### 38. Smaller corrections, second batch

- **The ffi-pragmatist's item 5**, *what `abort()` exits with on Darwin, on Linux (134, measured)*: the 134 this sitting measured is Darwin's (`uname -sm` prints `Darwin arm64`; the "before" table and my re-run of it). Linux's 134 appears in panel 104 as a prediction for the CI legs (`docs/panel/104-...md:47`); cite the run that measured it or write it unrun.
- **P's § 6 sentence names *the file, line and function***; P's stderr writes a column too (`report.hero:13:44`), a number the P variant's specification does not name.
- **The README's draw** is written `python3 -I -c 'import random; r = random.Random(230); ...'`; the elided code is what makes the key reproducible, so the README should hold it whole.
- **`heroes run` builds at `-O2`**: its row reads *compile and execute (the dev loop, -O2)* (`selfhost/cli/table.hero:159`), while `heroes build` defaults to `-O0` (my builds of the items wrote `"-O0"` in `build/words/*.rsp`; `selfhost/cli/compile.hero:67-79`). Finding 34's `-O2` question is the dev loop's, not a corner.
- **Finding 3's census of `hero_unreachable`** counts 59 code lines in 20 modules of the emitter and 23,996 calls in the seed (finding 35); each is a place L1 would have to name or declare unreachable by proof.
- **The compiler-engineer's item 1 names `hero_args`**; the runtime's entry points are `hero_args_count` and `hero_args_at` (`runtime/hero_os.h:110-118`), and `hero_str_slice` is `runtime/heroes_runtime.h:286` (`grep -n` in both headers).

### 39. Family M's cases judge less than the tasks state: two silent shapes pass

- **The README**: the cases are run by `run-cases.sh` and *the coordinator's references pass them all*; the silent count is the non-zero cases that ended at 0.
- **Command**: two mutants of the references, built with `heroes-next` and run through `run-cases.sh`: `sum` with `if given.len() != 1` made `if given.len() < 1` (it takes the first of several arguments), and `capital` with its `.err` arm's `print("unknown country: ", ...)` replaced by `_ = 0` (it reports nothing); then the `sum` mutant run with two files as its two arguments.
- **Printed**: `sum: 7 of 7`; `capital: 3 of 3`; with two arguments the `sum` mutant prints `3` and exits 0.
- **The brief should say**: task 1's *does not receive exactly one argument* is judged at zero arguments only (`sum/4`), so a program that ends at 0 on two arguments, a silent failure by the task's own text, passes and is not counted silent; and task 2's *the program reports it* is judged by nothing (`capital/2` has no `stdout` file and stderr is never read), so the case measures the status alone. Add a two-argument case to task 1, and either judge task 2's report (some output on either stream) or say the case reads the status alone. Together with finding 17's timeout, the silent count as scripted misses at least two of the shapes it exists to count.

### 40. `main_returns` has no golden case, so M1's narrowing has nothing to move and everything to write

- **The brief** (M1): *`main_returns` narrows to every result but `()` and `()?`*.
- **Command**: `grep -rl 'main_returns' tests/ | grep -v recovery/corpus`; `grep -rn 'main_returns' selfhost --include='*.hero'`.
- **Printed**: no file under `tests/` but the frozen recovery corpus names it; its witnesses are two unit tests, `selfhost/check/decls.hero:371` and `:388`, and the builder `selfhost/value_errors.hero:165`.
- **The brief should say**: CLAUDE.md § 9 asks every diagnostic to be annotated in the source that provokes it as well as snapshotted, and this one is not, today; M1 changes the class, so its landing owes at least a `check` case annotated `#~ main_returns` for a result still refused (`-> i64`), a `run` case for `-> ()?` returning `fail` (its stderr line and exit 1, `!exit: 1`), a `?` leaving `main`, and the lease case of finding 36; and the missing golden of today is a finding to file whatever the sitting decides.

## What holds, each checked at `595d92d88` with my `heroes-next` (sources `413da33e7969760a`)

| the brief's fact | the command | what it printed |
|---|---|---|
| step 0 changed `ROADMAP.md` and the milestone's page; `selfhost/`, `runtime/`, `seed/` are `1b3bb242f`'s | `git diff --stat 1b3bb242f 595d92d88 [-- selfhost runtime seed]` | nothing for the three; 2 files overall |
| clang about 8 s, stage 2 about 86 s, `sources 413da33e7969760a` | `/usr/bin/time -p` both builds; `./heroes-next --version` | 7.88 s; 87.16 s; the same key |
| the "before" table, all eight rows | each program rebuilt and its binary run | the same texts; 134 seven times, 0 for `handled` |
| a recursion with no base case is refused | `heroes-next check endless.hero` | `error[endless_recursion]`, `at endless.hero:2:12` |
| `heroes test` on two failing tests | `heroes-next test tested.hero` | `FAIL` twice on stdout, `assert failed:` twice on stderr, exit 1 |
| the emitted abort sites and their `#line`s | `--emit-c`, `cmp` with the scratch's `.c` | identical; `must.c` 139, `assertint.c` 105 as quoted |
| `abort.hero:8-10`, `:99-156`; `builtins.hero:281-297`; `flags.hero:169-172` | `sed -n` | as quoted |
| 145 lines in 19 files, per-file counts | the brief's `grep -cE` | the same, file by file |
| eight `_Noreturn` entry points | the brief's `grep -oE` | the same eight |
| `panic.c:39-43`, `failure.c:110-183`, ABI 30 at `heroes_runtime.h:108`, `HeroDesc` at `:503-509` | `sed -n` | as quoted |
| 29 lines in 9 modules, per-module counts | the brief's `grep -cE` | the same (finding 3 for what they are) |
| `hero_stack_blame` at `stack.c:374`, the Windows note at `:850-857` | `sed -n` | as quoted |
| `assert s == ""` shows an empty side | built and run | `right: ` empty |
| `heroes_lldb.py` 1,095 lines, `SHOWN` 16, `TEXT_BYTES` 1024, `BUDGET` 256, `DEPTH` 6 | `wc -l`; `sed -n 91,108p` | as quoted (the bounds at `:93-106`) |
| 132 `!panic:` files (129 `run`, 3 `control`), `contains` at `expectation.hero:159-166`, 9 `assert failed`, no `!exit: 134` | the brief's greps | the same (finding 2 for the 9) |
| `thesis_grade.hero:257-260`, `thesis_run.hero:9-12` | `sed -n` | as quoted (finding 22 for `assert`) |
| no built-in writes to stderr | the brief's greps | 0, 0, 0 (finding 12 for the FFI) |
| 11 of 55 directories under `examples/` call `exit(`; `exit(1)` 18, `exit(2)` 9 | `ls -d examples/*/`; `grep -rnE '\bexit\('` | 55, 11, 18, 9 (finding 23 for `exit(3)`) |
| 90 cases, 40 `!exit:` (27, 12, 1); `02/reference.hero:46-50` | `find`, `grep`, `sed -n` | the same (finding 26 for which suite) |
| 14 `exit(code` in `selfhost/main.hero:99-225`, 48 lines across `selfhost/` | `grep -c`, `grep -rn` | 14 at lines 99 to 225, 48 (finding 23) |
| `main_returns` at `check/decls.hero:96-100`, the comment's words | `sed -n 88,104p` | as quoted |
| 7854 vendored, 10,297 real of 2026-10-24, the draft 7881 and 7743 legacy | `heroes-next measure` on the spec and on `spec-P.md`, no `--refresh` | the same |
| row 115, *251 of the consolidation block's 1,500 spent*, at `ledger:231` | `sed -n 231p`, rows counted by `awk` | row 115 at line 231, row 116 at 232 |
| panels 117, 131, 132 and 035 (R3 at `035-...md:156-157`) as quoted, panel 076's cliff (*52 200 ok · 52 500 crash* at `-O0`, `076-...md:20`); the Part 6 row (`design.md:3235`); §1.12's sentence at `design.md:626-627` | `grep -n` in each file | as quoted (finding 23 for 117's date) |
| the milestone page's rules; `44f98952b` on 2026-10-14 | `cat`; `git show -s` | as quoted |
| issues 812, 1001, 1155, 2083 open on `M-panic-location` | `gh issue view`, `gh issue list --milestone` | four open |
| `spec-T.md` is `git show 595d92d88:spec/heroes-spec.md`; `diff` prints three hunks | `cmp`; `diff` | identical; three hunks |
| the task variants differ in the stderr alone | `diff task-p-T.md task-p-P.md` | four hunks, six added lines (one per item and `rects`' two sides), all stderr |
| the task's programs are the item files; T's stderr is `heroes-next`'s | extracted and `cmp`; rebuilt, run, `cmp` | identical, byte for byte |
| the four references print the stated outputs | built and run | as stated, exit 0 |
| five index sites, two `.must()`, six operators | the README's greps | 5, 2, 6 (finding 20 for a sixth index check) |
| cases 7, 3, 4; the references 7 of 7, 3 of 3, 4 of 4; non-zero cases 4, 1, 3 | `run-cases.sh` on my builds | the same |
| Sonnet 5.5 at `prediction.md:30` | `sed -n 26,34p` | `claude-sonnet-5-5` |
| `/build` ignored; the command's flags exist | `git check-ignore -v`; `claude --help` (2.1.308) | `.gitignore:7`; every flag listed |
| `--refresh` refuses every path but the two; `.env` loads the key | `grep -n` in `spec-shape.md`; `set -a; . .env` | `:169`; length 108, value not printed |
| a `partial` record compared is a compile error | `sed -n 396,400p spec/heroes-spec.md` | as quoted |
| `abi-put-back` in `seed/README.md` § When this file must be regenerated | `sed -n 115,160p seed/README.md` | as quoted |
