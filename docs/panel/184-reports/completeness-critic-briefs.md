# Panel 184, the completeness critic's first pass: the briefs

Written 2026-09-30 from 16:19, before any seat starts, by the completeness
critic, per `docs/panel/184-briefs/completeness-critic.md`. It gives no verdict.
It lists every framing fact of `00-shared.md` and the seats' briefs that a
command of mine did not confirm, and the ones it did, each with the command and
what it printed.

**My copy.** `<c>` below is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-critic/`,
made with `git -C /Users/joseph/Temp/heroes/heroes-lang archive a294a6ff | tar
-x -C <c>`, `rm -rf build`, `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes` (Apple clang 21.0.0, clang-2100.3.34.2, arm64-apple-darwin25.6.0).
`<p184>` is the coordinator's scratch. My compiler is the coordinator's
`<p184>/heroes-a294a6ff` apart from the link UUID: with both code signatures
removed, `cmp -l` counts 16 differing bytes, at offsets 1449 to 1464, and
`dwarfdump --uuid` prints two different UUIDs. So a count of mine and the
coordinator's are counts of the same code.

**Status: first pass complete, 16:50.** Sections A to E hold each check with
its command and output; the list below is what the coordinator repairs.

## What to repair before the seats start

**False as written** (each measured below):

1. The chain's panic function (00-shared.md:173): *`checkwalk.synth`; at
   10,000 `resolvewalk.expr`*. It is `resolvewalk.expr` from 600 through
   10,000, in the coordinator's own `<p184>/deep/binary-chain-600.out` to
   `-2000.out` and in mine (C1).
2. *Its fixed corpus: the 635 programs, 55,226 lines* and *13,827 sites*
   (00-shared.md:66-67, :130). The tree is fixed; which files are mutated is a
   wall-clock cut (`recovery.py:186`, `:205`). The same command, run by me at
   load 16, read 639 programs, 62,797 lines, 11 left out, 15,726 `over-indent`
   sites. The class counts and the 23 silent sites reproduced exactly (A1).
3. `selfhost/cursor.hero:269` (00-shared.md:134) is c85bccb8's line. In the
   seats' copy at a294a6ff that line is a `]` in a test, and the site is
   `:237` (A3). The other twelve cited files are identical at both commits.
4. The compiler-engineer's example `panic` is not a Heroes name:
   `error[unknown_name]: nothing named \`panic\` is in scope`. The spec's
   non-returning calls are `exit(code:)` (:319) and `.must()` (:161) (B3).
5. *A flat chain of n terms* has n+1 terms, and *n `else if` arms* are n-1 (C3).
6. The probe paths in 00-shared.md and two briefs do not exist in a seat's
   copy (untracked, 0 files at a294a6ff), and the briefs forbid reading the
   working tree they are in (B4).
7. A slip: of the three silent sites said to go into *the loop or `else`
   above*, all three go into a loop body; the `else` belongs to the `continue`
   (A6).

**Missing, measured, and changing what a seat decides:**

8. (2a)'s *a call that never returns* collides with `missing_return`, which
   today demands a statement after `exit(code: 3)`, after `assert false`, and
   after `while true` (exit 1 without it, exit 0 with it). Counting those as
   leaving for one rule and not the other leaves a function with no legal
   spelling. The log of 2026-08-04 and panel 021:362 recorded the `while true`
   half (B1, B2). The blind variant R leaves this case out (D2).
9. Panel 020 R4 item 2 already rules that the emitter omits blocks no edge
   targets. Measured: the emitted C of `after-return.hero` holds no
   `print(2)` (B1).
10. The walks that fail are at least eight, not five. Beyond the brief's five
    come `resolvenames.bare_name`, `cursor.pass_comments` and
    `resolvequalified.qualified` (C4).
11. Some shapes are not in the table. A method chain aborts from 200. The
    unary chains (250) and the method chain are neither counted by (3a)'s
    counter nor named by its exemption, and the blind variant Y's wording
    promises them no limit (C5).
12. The 17 silent `forget-f` mutants sit in 4 files, 13 of them in one golden
    file. Two of the 17 are one source line in two copies (A5).
13. The abort depth moves with `ulimit -s` on one machine, and Linux's 8 MB is
    a README sentence, not a measurement (C7).

**Leaks and unverified isolation in the blind seat:**

14. Task 3's brief says *implements X or Z*. Tasks 1 and 2 say *one of the
    three*. Task 2's brief text states a run that only Q allows (D3).
15. Neither the path of the folders nor the user settings is named as a
    channel. The path names `heroes-heroes-lang`, `184` and
    `llm-ergonomist`. The settings carry `language: italian` and give extra
    directories `/tmp` and `~/.claude`. `claude --help` offers `--restricted`
    and `--setting-sources`. What a fresh session actually receives is
    unrun: one capped paid run would settle it, and the coordinator decides
    (D5).
16. Three claims about the `task<n>` folders are unverified by me, since my
    brief forbids reading them: that they are outside any git tree, that each
    `spec.md` differs from a294a6ff's by its marker alone, and that their
    files are the copies in `blind/`. My own environment block says *Is a
    git repository: true* for `.../task3` (D4).

## A. The recovery instrument (00-shared.md, questions 1 and 2)

**A1. The corpus is not fixed; its membership is a wall-clock cut. The brief
calls it fixed.** 00-shared.md:66 says *over its fixed corpus: the 635
programs, 55,226 lines*. I ran the brief's own command against my compiler,
into my own folder (`python3 <scratchpad>/instrument/tool/recovery.py
--compiler <c>/heroes --out <c>/inst-ff-oi --only forget-f,over-indent
--no-pairs --jobs 2`, 16:24 to 16:26; the load average read 16.33 at 16:22,
just before). It printed:

```
16:26:32 baseline: 650 accepted, 0 not clean beside themselves, 11 dearer than 0.8s
16:26:42 models: 639 programs lexed by .../instrument/tree/heroes
16:26:42 candidates: 15751 sites over 2 operators
```

and its report's Corpus table reads **639 programs, 62797 lines**, left out
as dear **11** (the coordinator's 15 without `selfhost/literals.hero`,
`selfhost/scan.hero`, `tests/harness/suite_records.hero`,
`tests/harness/suite_spec.hero`), and `over-indent` **15726 sites**, where
the brief says 13,827. The reason is in the tool:
`instrument/tool/recovery.py:186`, `cost = time.time() - t0  # scheduling
only: which files are dear`, and `:205`, `corpus = sorted(f for f, r in
ctx.baseline.items() if r["clean"] and r["cost"] <= ctx.dear)`. The tree is
fixed; which of its files are mutated depends on the machine's load. A seat
that re-runs the command will not get 635, 55,226 or 13,827 unless its load
matches. **What did reproduce, exactly**: `forget-f` 25 sites, 25 planted, 17
SILENT, 1 ONE, 4 EXTRA, 3 ELSEWHERE, control 23 SILENT; `over-indent` 150
planted, 6 SILENT, 135 ONE, 2 EXTRA, 7 ELSEWHERE, the same in the control arm,
and the same 23 SILENT sites line for line (both reports' silent tables are
identical). Repair: say *fixed tree, load-dependent membership*, and give the
coordinator's numbers as that run's, beside `--dear` as the way to pin it.

**A2. The corpus tree is c85bccb8: verified.** The tool says so
(`recovery.py:13-14`). I extracted `git archive c85bccb8` into `<c>/c85` and
compared: `git ls-tree -r --name-only c85bccb8 | grep '\.hero$' | wc -l` prints
1350, the tool's `tracked-c85bccb8.txt` holds the same 1350 paths (`diff`
empty), and `cmp -s` of every one of them against `<scratchpad>/instrument/tree`
reported no difference. The census reproduced: `census: {'refused': 430,
'accepted': 650, 'nested': 270}` in both runs.

**A3. A line the brief cites is c85bccb8's, and at a294a6ff it is another
line.** The seats copy a294a6ff; the instrument's sites are c85bccb8's. `git
diff --stat c85bccb8 a294a6ff --` over the thirteen files the brief and the
report cite for questions 1 and 2 prints one file: `selfhost/cursor.hero | 159
+++++++++------------------------------------------` (two defect 130 commits,
`52b2d378`, `41807577`). So **`selfhost/cursor.hero:269`** (00-shared.md:134)
is, at a294a6ff, a `]` inside `test "bump stops at eof forever"`; the site,
`i @ i + 1` under `return true` in `refused_since`, is **`selfhost/cursor.hero:237`**
at a294a6ff (`awk 'NR>=228 && NR<=240'` prints the function at 231 to 239).
The other twelve files are byte-identical between the two commits, so their
lines stand for both. Repair: cite `:237` for a294a6ff, or say the line is
c85bccb8's.

**A4. Question 1's reading of the 8 reported mutants: verified.** From
`singles.jsonl` (both runs identical): the 6 `unused_binding` ones are
`tests/golden/ir/interpolation-desugar.hero` site 11 (diagnostics at 9, 10),
`tests/golden/run/fixedbugs-an-interpolated-string-holds-characters-above-ascii.hero`
site 13 (at 12), `examples/gallery/12-interpolation.hero` site 18 (three at
17), `tests/golden/run/interpolation-holes-and-braces.hero` site 20 (at 18,
19), and two in `tests/golden/surface-fixtures/comments101/holes.hero`, sites
8-11 (at 7) and 14-17 (at 13); none stands on a site line. The other 2 are in
`holes.hero`: ONE, `unclosed_bracket` at 14; EXTRA, `expected_end_of_line` at
10 and `expected_expression` at 11. The codes over all 25 are exactly
`unused_binding` (10 diagnostics), `unclosed_bracket` (1),
`expected_end_of_line` (1), `expected_expression` (1). I printed every
message: none names an `f`, a hole or interpolation (the twelve distinct
messages are `` `(` opened here is never closed ``, *expected the end of the
line, found a string*, *expected an expression, found `)`*, and nine
`unused_binding` texts, five of them about a PARAMETER: *the parameter `at` is
never read, remove it from the signature, or write `???`...*). So *no one of
the 25 is told at the literal that an `f` is missing* holds.

**A5. What the brief does not say about the 25, and a seat pricing "17 of 25"
needs** (the spec-warden's task 2 takes it as the measured argument). The 25
sites are in **7 files**, and **14 of them in one**,
`tests/golden/run/fixedbugs-an-interpolated-string-holds-characters-above-ascii.hero`;
the 17 SILENT are in **4 files**, **13 of the 17 in that one file**, 2 in
`examples/gallery/13-lease.hero`, 1 in
`docs/panel/168-briefs/gallery-example-reordered.hero`, which is a copy of
`13-lease.hero:25`'s very line (`print(f"C still reads {kept_label_length()}
bytes")`, both rows of the report's silent table), and 1 in
`tests/golden/run/interpolation-holes-and-braces.hero`. So the 17 are not 17
independent programs. One of the 25 removes the `f` of a literal INSIDE a hole
of an `f` literal (`...above-ascii.hero:33`, `{m["ключ{x}"].must()}` inside
`f"..."`, SILENT), which is the compiler-engineer's task 1 last question,
already sampled once. (Command: a `collections.Counter` over `singles.jsonl`'s
`forget-f` rows by `file` and `class_normal`.)

**A6. Question 2's reading of the 6 silent `over-indent` sites: verified,
with one wording slip.** Each site printed from `<c>/c85` with nine lines
above: `selfhost/cursor.hero:269` (c85bccb8) goes under `return true` in the
`if` of a `while` ✓; `tests/harness/cases.hero:45` under `return fail(...)`
✓; `examples/checksum/base64.hero:282` into the `while i < 64` body ✓;
`examples/routes/main.hero:161` into the `while i < path.len()` body ✓;
`examples/maze/main.hero:72` from the `for row` body into the `for column`
body ✓; `tests/harness/suite_emission.hero:276`, a `continue` that closed an
`if`/`else` at its level, into the `else` branch ✓, so the `if` branch
(`report.keep_pass(@r)`, line 266) now falls through to `judge(...)` at 277.
The slip: 00-shared.md:136 says *3 put a statement into the body of the loop or
`else` above it*; all three go into a loop body, and the one that goes into an
`else` is the `continue`. Neither (2a) nor (2b) as worded reaches the four
that are not under a `return` (no jump precedes them in their new block), which
matches the spec-warden brief's *2 of those 6 after a `return`*. The unrun
claim (*that loop would not end*) is marked unrun in the brief and reads true:
with the `i @ i + 1` inside the `if`, an index holding a thesis-rule
diagnostic is never passed.

## B. Question 2's framing

**B1. The two greps: verified, and the search the brief hands me finds more.**
Both of 00-shared.md:112-119's commands, run in `<c>`, print what it says: the
spec grep prints nothing (exit 1); the second prints panels 029 (:148, :353),
067 (:89), 068 (:86), the log entries of 2026-08-11 and 2026-08-16, and this
sitting's own entry (:12, :26). With a wider vocabulary (`unreach`, `dead
(code|statement|branch|arm|line)`, `reachab`, `after (a |the )?return|break|continue`,
`never (runs|reached)`, `diverg|never returns|noreturn`, `always leaves`) over
design.md, DESIGN-LOG.md, `docs/panel/*.md`, `docs/records/log/*.md`,
`docs/work/*.md`, `docs/work/milestones/*.md` and the spec (963 files; run
under `bash`, since zsh does not split a quoted file list and my first attempt
printed 0 hits everywhere), three passages bear on the question. None rules
on a statement after a jump; the first is a ruling that the language REQUIRES
one kind of statement no path reaches:

- **`docs/records/log/2026-08-04-0050-lands-in-the-checker-not-the-lowering-the-it-needs.md`**,
  where `missing_return` landed: *"The rule over-rejects `while true` exactly
  as Rust's does, recorded with its one-line repair rather than special-cased"*.
  And **`docs/panel/021-strings-ownership-and-how-a-float-prints.md:362-363`**,
  its watch list: *"`while true` in a value-returning function still needs an
  unreachable `return`"*. So the language today REQUIRES, in one shape, a
  statement no path reaches. See B2.
- **`docs/panel/020-the-c-emitter.md:213-217`**, R4 item 2: *"A block is
  emitted only if some edge targets it ... Omitting unreachable blocks entirely
  keeps that warning alive"*. Measured today: `HEROES_RUNTIME=<c>/runtime
  <c>/heroes build --emit-c after-return.hero` exits 0 and the body of
  `h_afterreturn_f` is `t1 = INT64_C(1); return t1;` and nothing else (lines
  93-103 of the emitted C); `print(2)` is not in the C. That answers in
  advance half of the ffi-pragmatist's task 2 (*whether any flag makes clang
  see it*): there is nothing in the C for clang to see. `grep -n -i
  unreachable selfhost/cli/flags.hero` prints nothing.

**B2. What (2a)'s "always leaves" collides with, measured, and nobody lists
it.** (2a) names *a call that never returns* among what always leaves, for the
seats to settle. Today `missing_return` treats those calls as NOT leaving, so
it DEMANDS a statement after them, and (2a) would then refuse that statement:
a function with no legal spelling, unless the two predicates move together.
Files in `<c>/q2/`, `<c>/heroes check`:

```
exit-last.hero        (if n > 0: return 1; then exit(code: 3) last)   exit 1, missing_return at 1:23
exit-then-return.hero (the same, then return 0)                      exit 0
assert-false-last.hero (if n > 0: return 1; then assert false last)   exit 1, missing_return at 1:23
assert-false-then-return.hero (the same, then return 0)              exit 0
while-true-last.hero  (while true holding the only return)            exit 1, missing_return at 1:23
while-true-then-return.hero (the same, then return 0)                exit 0
```

`after-if-else-returns.hero` (exit 0 with `print(3)` last in an `i64`
function) shows that an `if`/`else` whose branches both return already counts
as leaving for `missing_return` (`Outcome.jumps`, `walk.hero:703-713`), and so
does a `match` whose arms all return (`<c>/q2/match-returns-last.hero`, the
`match` last in an `i64` function, exit 0; `match-returns-then-print.hero`,
the same with `print(3)` after it, exit 0 and silent). So (2a)'s first two
cases are consistent with today's predicate and the third is not. The seats should be told the question is one predicate for two rules.

**B3. The compiler-engineer's example `panic` is not a name in Heroes.** Its
task 2 asks *whether a call can be known never to return (`panic`, `assert
false`, ...)*. `<c>/q2/panic-last.hero` checks at exit 1 with
`error[unknown_name]: nothing named \`panic\` is in scope`. The spec's calls
that end a program are `exit(code: i64)` (*ends the program*,
`spec/heroes-spec.md:319`) and `.must()` on a failure (*extract or abort*,
`:161`); `panic: ...` is only the runtime's message. The ffi-pragmatist's
task 2 calls `exit` an `extern` function the checker cannot know; in Heroes
`exit` is also a built-in (`:319`), which the checker can know.

**B4. The five probes: verified, and not in the seats' copies.** Copied to
`<c>/probes/`, `<c>/heroes check` exits 0 with 0 bytes printed on all five.
Two of the five differ from the coordinator's measured copies in
`<p184>/jump/` (`after-break.hero`, `after-continue.hero`: `diff` shows two
added blank lines, nothing else); both versions exit 0. The coordinator's
`.out` files are 0 bytes each and do not record the exit code. **The probe
paths do not resolve in a seat's copy**: `git -C <repo> ls-tree -r --name-only
a294a6ff -- docs/panel/184-briefs | wc -l` prints 0, and `git status` lists
`?? docs/panel/184-briefs/`, so `docs/panel/184-briefs/probes/after-*.hero`
and `.../depth.py`, which 00-shared.md, the compiler-engineer's and the
ffi-pragmatist's briefs name as relative paths, exist only in the repository's
working tree, which those two briefs and the spec-warden's forbid reading
(*Never read, build or run in the repository's working tree*; taken to the
letter that also forbids reading the briefs, which live there). Repair: name
the absolute path to copy from.

**B5. The checker lines cited: verified.** `selfhost/check/walk.hero:690-720`
is `block` as described (sets `jumps` at 703-704, walks on with no message,
returns at 713 or 720); `:92`, `:713`, `:765`, `:842` are each a `return
Outcome(jumps: ...)`; `selfhost/flow_errors.hero:157` is `function
missing_return`. `spec/heroes-spec.md:227-231` makes a jump a valid arm body.

## C. Question 3's nesting table, cell by cell

**How I measured.** `python3 <c>/probes/depth.py <c>/deep 100 200 250 300 400
500 600 700 800 1000 2000 10000` (Python 3.14.7) wrote 120 programs, ten
shapes at twelve depths, 250 included for every shape; `<c>/heroes check` on
each, four at a time, `ulimit -s` 8176 (`sysctl -n hw.ncpu` 8, load average
16.33 at 16:22), exit code and output kept beside each file. All 30 exit-0
cells printed nothing else.

| shape | brief: 0 up to / from / function | mine: 0 up to / from | function, by depth (mine) |
|---|---|---|---|
| `((1))` closed | 500 / 600 / `grammarexpr.postfix` | 500 / 600 (550 exits 0) | `grammarexpr.postfix` 600 to 10000 |
| never closed | 500, exit 1 / 600 / same | 500, exit 1, `unclosed_bracket` ×n at every n to 500 / 600 | `grammarexpr.postfix` 600 to 10000 |
| `[[1]]` | 200 / 250 / `checkwalk.synth` | 200 / 250 | `checkwalk.synth` 250-500, `grammarexpr.postfix` 600 to 10000 |
| `g(g(1))` | 100 / 200 / `checktable.ty_key` | 100 / 200 | `checktable.ty_key` 200-500, **`resolvenames.bare_name`** 600, `grammarexpr.postfix` 700 to 10000 |
| `- - 1` | 200 / 250 / `checkwalk.synth` | 200 / 250 | `checkwalk.synth` 250-500, `resolvewalk.expr` 600-2000, **`cursor.pass_comments`** 10000 |
| `!!true` | 200 / **300** / `checkwalk.synth` | 200 / **250** | `checkwalk.synth` 250-500, `resolvewalk.expr` 600-2000, **`cursor.pass_comments`** 10000 |
| `1 + 1 + ...` | 200 / 250 / `checkwalk.synth`; **at 10,000** `resolvewalk.expr` | 200 / 250 | `checkwalk.synth` 250-500, `resolvewalk.expr` **600 to 10000** |
| `if true` nested | 100 / 200 / `checkwalk.synth` | 100 / 200 | `checkwalk.synth` 200-250, `grammarexpr.postfix` 300 to 10000 |
| nested `f"{...}"` | 200 / 250 / `checkwalk.synth` | 200 / 250 | `checkwalk.synth` 250-500, **`cursor.pass_comments`** 600 to 10000 |
| `else if` chain | 10,000 / none | 10,000 / none | |

**C1. One cell is false, and it is false in the coordinator's own outputs.**
00-shared.md:173 says the chain aborts in `checkwalk.synth` and *at 10,000*
in `resolvewalk.expr`. `<p184>/deep/binary-chain-600.out` to
`binary-chain-2000.out` each read `panic: stack exhausted in
resolvewalk.expr`, as mine do: the resolver's walk is the one that fails from
600, not only at 10,000.

**C2. Coarser than it reads, not false.** `!!true`'s *from 300* is the grid's:
the brief measured 250 only for four shapes (the coordinator's `c-*-250.out`
files are exactly those four), and at 250 `!!` aborts too (mine:
`not-chain-250`, exit 134, `checkwalk.synth`). The coordinator's
`c-paren-closed-550.out` reads `error: cannot read \`paren-closed-550.hero\``,
so it is not a measurement; mine exits 0 at 550.

**C3. The shape names are off by one in two rows.** *A flat chain of n terms*:
`depth.py`'s `binary-chain-n` holds n+1 terms (`grep -o '+ 1'
binary-chain-200.hero | wc -l` prints 200, and the built program prints 201).
*`if`, then n `else if` arms*: `else-if-chain-n` holds one `if` and n-1
`else if` (99 at n = 100) and a final `else`.

**C4. The walks that fail are more than the five the compiler-engineer's
brief names.** Its task 3 lists `grammarexpr.postfix`, `checkwalk.synth`,
`checktable.ty_key`, `resolvewalk.expr`, `checklower.named` (all five lines
verified: `grammar_expr.hero:279`, `check/walk.hero:96`, `check/table.hero:128`,
`resolve/walk.hero:107`, `check/lower.hero:66`). The table's own shapes also
abort in `resolvenames.bare_name` (`selfhost/resolve/names.hero:29`) and
`cursor.pass_comments` (`selfhost/cursor.hero:120`), and the shape of C5 in
`resolvequalified.qualified` (`selfhost/resolve/qualified.hero:35`). A route
that bounds or unrolls "every walk" is judged against at least eight.

**C5. Shapes the table does not hold, measured, and what (3a)'s words do with
them.** In `<c>/extra/`, at 100, 200, 250, 300, 600, 1000, 10000:

- `b = true && true && ...`: 0 to 200, 134 from 250 (`checkwalk.synth`),
  `resolvewalk.expr` from 600: like `+`.
- a method chain, `v.to_str().len().to_str()...` (n calls): 0 at 100, **134
  from 200** (`checkwalk.synth`), `resolvequalified.qualified` from 600. It
  opens no bracket that stays open, so a counter of open brackets sees depth 1.
- a block of n `print(i)` statements: exit 0 at every depth to 10,000.

(3a) counts *where the parser opens a bracket, a block or a hole* and exempts
*a flat operator chain* by making *its walks not recurse per operand*. The
unary chains (`- -`, `!!`, 250) and the method chain (200) open nothing the
counter counts and have one operand, so under (3a) as worded they are neither
limited nor promised; they would still abort. The blind variant Y promises
more: *a chain of operators, `a + b + c`, is not nesting and has no limit*
reads as covering them. The briefs should say which.

**C6. The build claims: verified.** `HEROES_RUNTIME=<c>/runtime <c>/heroes
build`: `g(g(...))` at 10 to 90 builds, and each binary prints `1` at exit 0;
at 100 `check` exits 0 and `build` prints `panic: stack exhausted in
checklower.named`, exit 134. Parens 500, `[[` 200, `- -` 200, `!!` 200, the
chain 200, `if` 100, nested `f` 200 and the `else if` chain 2000 each build
and run at exit 0, printing `1`, `1`, `1`, `true`, `201`, `1`, `1`, `1`.

**C7. The platform sentences.** `selfhost/cli/flags.hero:194` is
`["-Wl,/STACK:67108864", "-Wl,/INCREMENTAL:NO"]`, the value of `link_flags()`
when `process.exe_suffix()` is `.exe` (`:168-179`); `seed/README.md:17` is the
Windows command with the same flag and `:20` says *Windows gives a process's
main thread 1 MB of stack where Darwin and Linux give 8*. Darwin's 8 is
measured (`ulimit -s` 8176 here; panel 107:178 measured `main = 8,372,224`).
**Linux's 8 MB is the README's sentence, not a measurement of this brief**, and
the brief's own *the other platforms' are unrun* covers the depths but not
that number. One thing the brief leaves unsaid and a seat needs: the guard is
a guard-page handler (`runtime/parts/stack.c:21-25`), so the abort depth is
whatever the running thread's stack holds, and it moves on one machine with
`ulimit -s` (panel 107:104-106: `./heroes check selfhost/main.hero` exits 134
at `ulimit -s` 512, 640 and 768 KB and 0 at 896 KB). *The depths above are
this Mac's* is true for this Mac's default limit only.

**C8. The rest of question 3's citations: verified.** Panel 107 is ratified
2026-09-04 (`:6`); its `:10-11` says *no language specification states a
number*; its § The resolution (`:246-247`) says *would be false on the day it
landed, for a legal program and nearly for this compiler itself*; Darwin's
library threads at 512 KB are measured at `:178` (`default attr = 524,288`).
`spec/heroes-spec.md:270`, *Recursion too deep aborts.*, is in § 9
(246-275). `.claude/rules/cli-surface.md:40-42` is the exit contract.

## D. The blind seat: its inputs, and whether they leak

**D1. Every output the seat is given reproduces byte for byte.** I copied
`task1.hero`, `task2.hero`, `task3a.hero`, `task3b.hero` from
`docs/panel/184-briefs/blind/` into `<c>/blind/` and ran `<c>/heroes check` on
each, with the exit code appended: `cmp` against `o1-check.txt`,
`o2-check.txt`, `o3a-check.txt`, `o3b-check.txt` finds no difference, and
`build task3b.hero` against `o3b-build.txt` finds none. `task2.hero` builds
(exit 0) and `perl -e 'alarm 5; exec @ARGV' ./task2.bin` exits 142 with 0
bytes printed. The withheld measurement: `task1.hero` without line 6 (`sed
6d`) is byte-identical to `<p184>/blindprep/task1-advice-taken.hero`, checks
at exit 0, builds, and prints `row 3: row-3`, `row 5: row-5`, `row 8: row-8`,
`{count} rows`, `total {total} over {rows.len()} rows`. The program
descriptions hold: 300 numbers in `task3a.hero`; 101 `step(` in
`task3b.hero`, one the definition, so 100 nested.

**D2. The variants against 00-shared.md's routes.** K is (1b) and M is (1a),
in the routes' words; L is word for word the sentence the marker replaces
(`grep -c` finds it once in the spec, at :52, in § 2). P is (2b). **R is
narrower than (2a)**: it lists *a `return`, `break` or `continue`, an `if`
whose every branch leaves, or a `match` whose every arm leaves*, and not *a
call that never returns*, which 00-shared.md:152 puts in (2a) for the seats to
settle; so the blind seat and the other four judge different (2a)s, and B2's
collision is outside the blind seat's text. **Y is broader than (3a)** (C5).

**D3. What the briefs' own text gives away.** No project word:
`grep -n -i -E 'heroes|\bhero\b|panel|\b184\b|seat|coordinator|selfhost|a294|trunk|today|current|proposal|route|\(1a\)|\(2a\)|\(3a\)|design\.md|CLAUDE|bowie'`
over the three briefs, the programs and the outputs matches only the `.hero`
extension (and a `184` inside `task3a.hero`'s chain). The spec copy names the
language, *Report on the Programming Language Heroes* (`spec/heroes-spec.md:1`,
also :3 and :302), by design. The variant: task 1 and task 2 say *implements
one of the three* (`brief-task1.md:24`, `brief-task2.md:22`); **task 3 says
*implements X or Z* (`brief-task3.md:26`), which tells the seat that Y is not
the compiler's**. Task 2's text adds *Built and run, that compiler's program
... was still running after 5 seconds* (`brief-task2.md:23-24`), a fact only Q
allows, since under P or R `task2.hero` is a compile error and has no program;
tasks 1 and 2 then ask the seat which variant the output fits, so task 2's
answer is in its brief as well as in its output. The spec copy leaks none:
no plain literal holding a brace pair (`grep -n -E '(^|[^f])"[^"]*\{[^"]*\}[^"]*"'`
prints nothing), no statement after a jump in its examples (the two `return`
lines in its code, :94 and :168, are each last in their block), and on depth
only :270 and a sample `constant MAX_DEPTH: i64` of value 64 (:88-89).

**D4. The folders' isolation, as far as I may check it.** The three ancestors
the brief names have no `CLAUDE.md`, and neither do the other four outside the
seat's own folder: `CLAUDE.md`, `CLAUDE.local.md` and `.claude` are absent from
`/`, `/private`, `/private/tmp`, `/private/tmp/claude-501`,
`.../-Users-joseph-Temp-heroes-heroes-lang`, `.../edfda945-...` and
`.../scratchpad` (`[ -e ]` on each). `git -C <scratchpad> rev-parse
--show-toplevel` prints *fatal: not a git repository*. `~/.claude/CLAUDE.md`
does not exist, `~/.claude/settings.json` has no `hooks`, and
`~/.claude/projects/` had no folder named for a 184 path at 16:25 or at 16:49
(which I read as no blind session having started, an inference: a session's
transcript folder is named for its working directory). **Not checked by me**,
because my brief forbids reading the seat's folders: that they are outside any
git tree; that each `spec.md` is the spec at a294a6ff with one marker and
nothing else changed (settled by `diff <(git show
a294a6ff:spec/heroes-spec.md) <task folder>/spec.md`, expecting one line
replaced at :52 for task 1, one added after :235 for task 2, one added after
:270 for task 3); and that each `brief.md`, program and output there is the
copy in `docs/panel/184-briefs/blind/` (`cmp` each). One signal the
coordinator should settle with `git -C <task folder> rev-parse
--show-toplevel`: my own session's environment block names
`.../184-llm-ergonomist/task3` as its primary working directory (my shell
starts there, so every command I ran used absolute paths) and says *Is a git
repository: true*, against the brief's 16:18 reading. It may be a value
carried from the coordinator's start in the repository; I did not run it.

**D5. Channels into a fresh `claude -p` session that the brief does not name,
and flags that close them.** Measured facts: (a) the folder path is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-llm-ergonomist/task<n>/`,
which names the checkout (`heroes-heroes-lang`), the sitting (`184`) and the
seat (`llm-ergonomist`), and a Claude Code session is told its working
directory (my own environment block carries it); (b) `~/.claude/settings.json`,
which a session loads unless told not to, holds `"language": "italian"` where
the run's prompt says *Answer in English*, `"model": "opus"`, `"effortLevel":
"xhigh"`, and 22 `permissions.additionalDirectories`, 14 naming a heroes path
(none of those 14 exists on disk) and two that exist, `/tmp`, which contains
every seat's directory and `<p184>/blindprep/`, and `/Users/joseph/.claude`,
which holds `projects/-Users-joseph-Temp-heroes-heroes-lang/` (157 entries,
this project's transcripts); (c) `claude --help` (2.1.274, `claude --version`)
lists `--restricted`, which *ignores user, project and local settings files*
and *confines the file tools to the working directories*, and
`--setting-sources`; the brief's command uses neither, and `--allowedTools
"Read,Write"` names tools, not paths. **Unrun**: what a fresh session's prompt
actually carries of (a) and (b), and whether its `Read` of a path outside the
folder succeeds. It would be settled by one paid run, a `claude -p` session in
a folder of the same shape asked only to list its working directories and any
language instruction, capped with `--max-budget-usd` well under 1 USD; the
seat's own `context` answer is the free, weaker witness. The coordinator
decides.

## E. The other seats' briefs

- **spec-warden: verified.** `<c>/heroes measure spec/heroes-spec.md` (no key
  in the environment, `${#ANTHROPIC_API_KEY}` 0; the plain verb is offline,
  `selfhost/measure/pinned.hero:77`) prints `real 9060`, `claude-opus-5,
  2026-09-28`, *the binding number*, `maximum 6838`, and *Headroom: 1180
  against the 10240 ceiling ... 60 of it (panel 030 R3), so what is measured
  against the ceiling is 9120*, exit 0. Panel 107's quoted sentence: C8.
- **compiler-engineer.** `selfhost/scan.hero:69-70` hands `"` to
  `literals.string` and `:71-74` hands `f"` to `lex_interp.head` (`:81-83`,
  `lex_interp.piece` after a hole's `}`): verified. The certain-fix rule is
  `.claude/rules/diagnostics-and-goldens.md:22`, *A `certain` fix repairs the
  defect the diagnostic names*: verified. `runtime/parts/thread.c`,
  `runtime/parts/stack.c`, `tests/harness/suite_layout.hero` exist. The 2.8
  minutes: `<p184>/instrument-a294a6ff.stdout`, *this invocation took 2.8
  minutes*. Against it: `panic` (B3), the list of walks (C4), the cursor line
  (A3).
- **ffi-pragmatist.** `examples/gallery/13-lease.hero:23` is `label: cstr @
  f"row-{at}-payload".lease()` at both commits; § 13 is FFI
  (`spec/heroes-spec.md:334`); `.claude/rules/platforms.md` exists. Against it:
  `exit` is a Heroes built-in as well as a C function (B3); the emitted C holds
  no statement after a `return` (B1).
- **historian: verified.** `docs/panel/121-the-brace-was-already-taken.md:46-50`
  and `docs/panel/107-the-number-cannot-be-uniform-the-abort-can.md:10-11` say
  what the brief quotes. One wording clash with 00-shared.md: 00-shared.md:31-32
  says the three blind sessions *are the sitting's only paid runs*, and the
  historian's brief says *No paid run beyond your web searches*.
- **00-shared.md's remaining facts: verified.** `git log -1 --format='%h %ci'
  a294a6ff` prints `a294a6ff 2026-09-30 15:49:24 +0200`;
  `spec/heroes-spec.md:49-52` is the quoted text; panel 121's R2 is at `:265`
  (*Unanimous ... 211 literals in 39 of 190 modules stop parsing*) and its
  ratification at `:328` (*RATIFIED 2026-09-09*); the log entry of 2026-09-30
  10:38 names §4.17 as the questions' home (`:14`); `SKILL.md` § 3c holds the
  paid-run rule (`:264-271`) and § 2 the fresh-session paragraph (`:145-157`);
  `ulimit -s` prints 8176 and `sysctl -n hw.ncpu` 8. The load average the brief
  gives for its own runs is not re-derivable; mine read 16.33 14.82 13.92 at
  16:22.

