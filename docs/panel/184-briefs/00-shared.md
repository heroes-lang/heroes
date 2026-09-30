# Panel 184, shared brief: a forgotten `f`, a statement after a jump, and how deep a source may nest

Written 2026-09-30 between 15:50 (the first measurement) and 16:20 by the
coordinator, for every seat, and **repaired between 16:51 and 17:00 on the
completeness critic's first pass**
(`docs/panel/184-reports/completeness-critic-briefs.md`, its sixteen items,
each re-measured by the coordinator before it was written here). Every number
and path below names the command that produced it, run while this brief was
written. **The tree the seats copy is commit `a294a6ff`** (`git log -1 --format='%h %ci'
a294a6ff` prints `a294a6ff 2026-09-30 15:49:24 +0200`); the trunk's working
tree may move during the sitting, so no seat builds or runs in it: each copies
the commit with `git -C /Users/joseph/Temp/heroes/heroes-lang archive a294a6ff
| tar -x -C <its own directory>`, then `rm -rf build`, then builds its own
compiler from the seed, `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes` (a few seconds). A seat that must prototype in `selfhost/` rebuilds
with `./heroes build selfhost/main.hero -o heroes-next` (about a minute).

**What a seat reads in the repository's working tree: this sitting's briefs
directory and nothing else**,
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/`. It is not in
the commit the seats copy (it is committed with the sitting), so its programs
are copied from there by absolute path: the five jump programs,
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/probes/after-*.hero`,
and the nesting generator,
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/probes/depth.py`
(`python3 depth.py <dir> <n>...`). The coordinator's scratch is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/p184/`
(below: `<p184>`): the compiler that measured, `<p184>/heroes-a294a6ff`, built
from that commit's seed with plain `clang` at 15:51; the nesting programs and
their outputs, `<p184>/deep/` and `<p184>/deep2/`; the recovery instrument's
run, `<p184>/instrument-a294a6ff/`. Two of the five jump programs in the
briefs directory were formatted after `<p184>/jump/`'s copies were measured
(`after-break.hero`, `after-continue.hero`: two blank lines added, both exit 0).

**The lane is the full panel** (`.claude/skills/panel/SKILL.md` § Two lanes):
each question is a diagnostic class and each may cost a spec sentence. Seats:
compiler-engineer, ffi-pragmatist, spec-warden, historian, and the
llm-ergonomist as fresh sessions outside the repository; the completeness
critic read these briefs before any seat started and reads the reports after.

**No paid run beyond what a brief names** (author instruction 2026-09-30,
`.claude/skills/panel/SKILL.md` § 3c). The sitting's paid runs are the
llm-ergonomist's three `claude -p` sessions and one `claude -p` probe the
coordinator ran before them to settle what a fresh session receives, each
capped by `--max-budget-usd` (`llm-ergonomist.md`). The historian's web
searches are that seat's method inside its own session, not a separate run. A
seat that finds another run worth making says so in its report, with its size,
and the coordinator decides.

**Every seat writes its report as it goes**, to
`docs/panel/184-reports/<seat>.md`, English, no em dashes, and ends it with
`verdict` (approve, object or veto, per question), `section` (the design.md
section it rests on), `cost` (measured), `prediction` (falsifiable, with when
it becomes checkable) and `condition` (what result would change the verdict).

## Why the sitting is owed

The author's decision of 2026-09-30, recorded in
`docs/records/log/2026-09-30-1038-four-answers-the-blind-seat-runs-as-a-fresh-session-and-one-sitting-takes-three-questions.md`:
one sitting, this one, takes three language questions, each a diagnostic class
(CLAUDE.md § 4). design.md §4.17 is their home (the log entry's table). The
sitting is not a retro-record: nothing has been decided.

## Question 1: a literal whose `f` was forgotten

**Today.** `spec/heroes-spec.md:49-52` (§ 2):

> An `f` before a literal's opening quote makes `{e}` write that value as
> `to_str` does: `f"line {n}: {word}"`. Any expression may stand there, and the
> hole ends at the `}` that closes it, nested brackets and literals skipped;
> `{{` writes one brace. A literal without the `f` is unchanged.

That last sentence is panel 121's R2
(`docs/panel/121-the-brace-was-already-taken.md:265`, ratified 2026-09-09 at
`:328`): *the brace is active only inside a gated literal*, unanimous, because
making a bare `{` active in every literal stopped 211 brace-bearing literals in
39 of `selfhost/`'s 190 modules from parsing, a count of that day's tree which
this brief did not re-run.

**What it costs, measured.** The recovery instrument's `forget-f` operator
removes the `f` from an interpolated literal, one mutant per site. Its TREE is
fixed, the trunk's committed tree at `c85bccb8` (1,350 tracked `.hero` files,
650 of them accepted alone by `check --brief`); **which of those it mutates is
not**: a file is left out when its own check took longer than `--dear`, 0.8 s
of wall clock (`instrument/tool/recovery.py:186` and `:205`), so the corpus
moves with the machine's load. The coordinator's run left 15 out and mutated
635 programs, 55,226 lines; the critic's, the same command at a load of 16,
left 11 out and mutated 639 programs, 62,797 lines. **The class counts below
reproduced exactly in both runs, and the silent sites are the same line for
line** (the critic compared the two reports' silent tables). A seat that needs
the corpus pinned passes a larger `--dear`.

```
python3 <scratchpad>/instrument/tool/recovery.py --compiler <p184>/heroes-a294a6ff \
  --out <p184>/instrument-a294a6ff --only forget-f,over-indent --no-pairs --jobs 2
```

`report.md` in that folder, the counts table: **25 sites, all 25 planted**;
under `check --brief`, **17 SILENT** (exit 0, the program prints the braces and
the hole's text), 1 ONE, 4 EXTRA, 3 ELSEWHERE; under `check --brief
--permissive`, 23 SILENT. Of the 8 the normal arm reports, read from
`singles.jsonl`: **6 are reported only by `unused_binding` at the binding the
hole used**, never at the literal, and 2, both in the multi-line fixture
`tests/golden/surface-fixtures/comments101/holes.hero`, by `unclosed_bracket`
or `expected_end_of_line` and `expected_expression`. **No one of the 25 is
told at the literal that an `f` is missing**: the codes printed are those four
and no other (the critic printed every message: none names an `f`, a hole or
interpolation).

**What the 25 are, and what a rate taken from them can claim** (the critic's
count over `singles.jsonl`): they are in **7 files, 14 of them in one**,
`tests/golden/run/fixedbugs-an-interpolated-string-holds-characters-above-ascii.hero`;
the 17 SILENT are in **4 files, 13 of them in that one**, 2 in
`examples/gallery/13-lease.hero`, 1 in
`docs/panel/168-briefs/gallery-example-reordered.hero` (a copy of
`13-lease.hero:25`'s very line) and 1 in
`tests/golden/run/interpolation-holes-and-braces.hero`. So they are 17 sites
and not 17 independent programs. One of the 25 removes the `f` of a literal
inside a hole of an `f` literal (`...above-ascii.hero:33`, SILENT). Two silent
ones, from the report's table:

```
examples/gallery/13-lease.hero:23   label: cstr @ "row-{at}-payload".lease()
examples/gallery/13-lease.hero:25   print("C still reads {kept_label_length()} bytes")
```

**What following today's message does, measured on the blind seat's task 1**
(`docs/panel/184-briefs/blind/task1.hero`): its one diagnostic is
`unused_binding` at `count`, *remove the binding, or read it*; with that line
removed the program checks at exit 0, builds, and prints `{count} rows` and
`total {total} over {rows.len()} rows` (`<p184>/blindprep/task1-advice-taken.hero`).

**The routes the coordinator can name**, for the seats to measure, refuse or
replace; **a route nobody listed is the finding the sitting most wants**:

- **(1a) the shape**: a literal without the `f` holding a `{` whose text up to
  the `}` that closes it would parse as a hole is a compile error; its fix, a
  `guess`, writes the `f`; braces meant as text in such a place are written
  `{{`...`}}` in an `f` literal. The rule reads the literal alone.
- **(1b) the shape and the scope**: the same, only where the hole's names are
  bound where the literal stands. Fewer false alarms, and a literal's legality
  then depends on the bindings around it.
- **(1c) the message, not the rule**: plain literals stay unchanged, and
  `unused_binding` on a name that a plain literal in the same function holds in
  braces says so, with the `f` as its fix. It reaches the 6 above and not the
  17.
- **(1d) today's rule**, which panel 121 adopted for the reason quoted above.

## Question 2: a statement after a jump

**Today.** The spec says nothing about a statement that follows `return`,
`break` or `continue` in the same block: `grep -n -i -E "unreachable|after a
jump|dead code|never executed" spec/heroes-spec.md` prints nothing, and
`grep -n -i -E "unreachable (code|statement)|statement after (a|the)
\`?(return|jump)|code after (a |the )?\`?return|after a jump|dead
statement|never executed"` over `docs/design/design.md`, `docs/panel/*.md` and
`docs/records/log/*.md` finds the word in other senses (`panic: entered
unreachable code`, in panels 029, 067 and 068 and in two log entries of
2026-08-11 and 2026-08-16) and this sitting's own log entry, and no ruling.
§ 8 (`spec/heroes-spec.md:227-231`) makes a jump a valid arm body. The
critic's wider search (`unreach`, `dead (code|statement|branch|arm|line)`,
`reachab`, `diverg`, `never returns`, `noreturn`, `always leaves` and more,
over 963 files) found three passages that bear on it and no ruling on the
language; two of them are the next paragraph's.

**What the language already demands, and what any rule here must agree
with.** `missing_return` counts an `if`/`else` whose branches both return as
leaving (`after-if-else-returns.hero` exits 0 with `print(3)` last in an `i64`
function), and it does NOT count `exit(code:)`, `assert false` or `while true`
as leaving: each of these, as the last statement of an `i64` function whose
other path returns, costs `missing_return` at 1:23, exit 1, and the same with a
`return 0` after it exits 0 (`<p184>/deep2/exit-last.hero`,
`assert-false-last.hero`, `while-true-last.hero`, `exit-then-return.hero`). So
**today the language requires, in those three shapes, a statement no path
reaches**, a cost recorded when the rule landed
(`docs/records/log/2026-08-04-0050-lands-in-the-checker-not-the-lowering-the-it-needs.md`,
*the rule over-rejects `while true` exactly as Rust's does*) and on panel
021's watch list (`docs/panel/021-strings-ownership-and-how-a-float-prints.md:362-363`).
A rule that counted those three as leaving while `missing_return` did not
would refuse the one statement `missing_return` demands: a function with no
legal spelling. **The question is one predicate for two rules.** And **the
spec states neither rule**: `grep -n -i -E "every path|must return|returns
nothing|missing" spec/heroes-spec.md` prints only `missing_key` (:294), so
`missing_return` lives in the compiler alone and a reader of the spec meets it
first as a message. And the
emitter already drops a block no edge reaches (panel 020 R4 item 2,
`docs/panel/020-the-c-emitter.md:213-217`): `--emit-c` of `after-return.hero`
gives `h_afterreturn_f` the body `t1 = INT64_C(1); return t1;` and no
`print(2)` (`<p184>/deep2/after-return.c`), so clang never sees the statement.
The calls that end a program in this language are `exit(code: i64)`, a
built-in (*ends the program*, `spec/heroes-spec.md:319`), and `.must()` on a
failure (*extract or abort*, `:161`); `panic` is not a name the language has
(`panic: ...` is the runtime's message).

Five programs,
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/probes/after-*.hero`,
each with a statement after a jump in the same block (`return` with a value, a
bare `return`, `break`, `continue`, and an `if`/`else` whose two branches
return): `<p184>/heroes-a294a6ff check <file>` exits **0 on all five**, with
nothing printed.

**What it costs, measured.** The instrument's `over-indent` operator moves one
statement a level deeper than its block: 150 planted (the instrument's default
per operator) of 13,827 sites in the coordinator's run and 15,726 in the
critic's (the load-dependent corpus of Question 1), **6 SILENT**, 135 ONE, 2
EXTRA, 7 ELSEWHERE, the same in both arms and both runs. Of the 6 silent, read
in the corpus beside each site: **2 put the statement under a `return` in the
block above it**, `selfhost/cursor.hero:269` in the instrument's tree at
`c85bccb8`, which is `selfhost/cursor.hero:237` in the seats' copy at
`a294a6ff` (`i @ i + 1` under `return true` inside a `while`, in
`refused_since`; the file moved 32 lines between the two commits, the other
sites' files did not), and `tests/harness/cases.hero:45` (the loop's
`cases.push(...)` under `return fail(...)`); **3 put a statement into the body
of the loop above it** (`examples/checksum/base64.hero:282` into `while i <
64`, `examples/routes/main.hero:161` into `while i < path.len()`,
`examples/maze/main.hero:72` from the `for row` body into the `for column`
body); **1 moves a `continue` into the `else` branch above it**
(`tests/harness/suite_emission.hero:276`), so the `if` branch falls through.
Neither jump route below reaches the four that are not under a jump. By
reading, and the critic read it the same way: the mutant at `refused_since`
no longer advances `i` when the `if` is false, so that loop would not end; the
blind seat's `task2.hero` holds that shape, and built and run it printed
nothing and was still running when killed at 5 s (`perl -e 'alarm 5; exec
@ARGV'`, exit 142).

**Where the checker stands.** `block` (`selfhost/check/walk.hero:690-720`)
walks a block's statements, sets `jumps` when one jumps, and walks the rest
with no message; `Outcome(jumps: ...)` is how a statement says it left
(`selfhost/check/walk.hero:92`, `:713`, `:765`, `:842`). `missing_return` is
`selfhost/flow_errors.hero:157`.

**The routes the coordinator can name**: **(2a)** one predicate, *this
statement always leaves its block*, shared by both rules: a statement after
one that always leaves, in the same block, is a compile error, and a function
none of whose paths reaches its end needs no `return` there; what *always
leaves* covers (a jump; an `if`/`else` or a `match` every branch of which
leaves; `exit(code:)`; `assert false`; `while true` with no `break`) is for the
seats to settle, and whatever it covers, both rules read the same answer;
**(2b)** the first half for the three jump words only, `missing_return`
unchanged, which leaves the three shapes above as they are; **(2c)** today's
rule.

## Question 3: how deep a source may nest

**Today.** A source nested past what the compiler's own stack holds aborts
`check`: `panic: stack exhausted in <function>`, exit 134, where the command's
contract is exit 0 clean, 1 the input has diagnostics, 2 the tool could not run
(`.claude/rules/cli-surface.md:40-42`). Measured on this Mac (`ulimit -s`
8176, 8 cores, the load average near 10 to 19 from three lanes and the
formatter's probe beside it; no run was repeated on a still machine),
`depth.py` programs at each depth, `check` with `<p184>/heroes-a294a6ff`; the
critic's run of the same programs at twelve depths, 250 among them for every
shape, gave the same cells except where the table says so:

| shape (one line of `main`, or its body) | exit 0 up to | aborts from | the function the panic names |
|---|---|---|---|
| `x = ((((1))))`, closed | 500 (the critic: 550 too) | 600 | `grammarexpr.postfix` |
| the same, never closed | 500 (exit 1, one `unclosed_bracket` per opener) | 600 | `grammarexpr.postfix` |
| `x = [[[[1]]]]` | 200 | 250 | `checkwalk.synth`; `grammarexpr.postfix` from 600 |
| `x = g(g(g(1)))` | 100 | 200 | `checktable.ty_key`; `resolvenames.bare_name` at 600; `grammarexpr.postfix` from 700 |
| `x = - - - 1` | 200 | 250 | `checkwalk.synth`; `resolvewalk.expr` 600 to 2000; at 10,000 `grammarexpr.unary` (the coordinator) or `cursor.pass_comments` (the critic) |
| `b = !!!true` | 200 | 250 | the same as `- -` |
| `x = 1 + 1 + ... + 1`, n `+` (n+1 terms) | 200 | 250 | `checkwalk.synth`; **`resolvewalk.expr` from 600** |
| `b = true && true && ...`, n `&&` | 200 | 250 | `checkwalk.synth`; `resolvewalk.expr` at 600 |
| `x = v.to_str().len()...`, n calls in one chain | 150 | 200 | `checkwalk.synth`; `resolvequalified.qualified` at 600 |
| `if true` nested n deep | 100 | 200 | `checkwalk.synth`; `grammarexpr.postfix` from 300 (the critic) |
| `f"{f"{f"{1}"}"}"` | 200 | 250 | `checkwalk.synth`; from 600 `grammarexpr.binary` (the coordinator) or `cursor.pass_comments` (the critic) |
| `if`, then n-1 `else if` arms and an `else` | 10,000 | none measured | |
| n `print(i)` statements in one block | 10,000 | none measured | |

The coordinator's depths were 100 to 800 by hundreds, 1000, 2000 and 10000 for
the first nine shapes, 250 for four of them, and the `&&` chain, the method
chain and the block at the depths shown (`<p184>/deep2/`); the critic's the
same twelve for every shape plus the three last (its report § C, C5). A cell
that differed was the coordinator's grid being coarser, or, for the chain's
function, the brief being wrong about its own outputs (the critic's C1).
**The function a panic names is where the stack ran out, and that is not a
fixed point**: the same programs, on two compilers built from one seed and
differing only in their link UUID, named different functions at 10,000 `- -`
and `!!` and from 600 nested `f` (the coordinator's `<p184>/deep/*.out` against
the critic's § C table).
**And `build` stops sooner than `check`**: `g(g(...))` at 90 builds and runs
(10 to 90 by tens, each built), at 100 `check` exits 0 and `build` aborts,
`panic: stack exhausted in checklower.named`, exit 134. Every other closed
shape builds and runs at the largest depth of those measured that `check`
accepted, the `else if` chain at 2,000 (10,000 not built)
(`HEROES_RUNTIME=<tree>/runtime <p184>/heroes-a294a6ff build <file> -o <bin>`,
`<p184>/deep/b-*.out`; the critic reproduced every one).

**What "deep" is, then: three kinds, and the table holds all three.** Nesting a
reader sees (brackets, blocks, holes); a chain of one-operand operators
(`- -`, `!!`), which opens nothing; and a chain a reader sees as FLAT (`+`,
`&&`, a method chain), which opens no bracket that stays open, so a counter of
open brackets sees depth 1 on the 200-call method chain that aborts. A route
says which kinds it limits, which it makes hold any length, and which it leaves
to abort.

**What is already ruled.** Panel 107 (`docs/panel/107-the-number-cannot-be-uniform-the-abort-can.md`,
ratified 2026-09-04) refused a depth number in the spec **for a program's
recursion at run time** and made the abort uniform instead, which is
`spec/heroes-spec.md:270`, *Recursion too deep aborts.* The compiler is a
Heroes program, so its own abort above is that sentence applied to it. Whether
a limit on the **source's** nesting is the same kind of number is this
question. **The abort depth is not a property of the compiler alone**: the
guard is a guard-page handler (`runtime/parts/stack.c:21-25`), so a check
aborts wherever the running thread's stack ends, and that moves with `ulimit
-s` on one machine (panel 107:105: the compiler checking itself exits 134 at
`ulimit -s` 512, 640 and 768). `selfhost/cli/flags.hero:194` links Windows
binaries with `-Wl,/STACK:67108864`, and `seed/README.md:17` builds the Windows
compiler the same way; `seed/README.md:20` says Darwin and Linux give 8 MB,
which is measured on this Mac (8176 KB) and **unmeasured on Linux by this
brief**. So the depths above are this Mac's, at its default limit, and the
other platforms' are unrun.

**The routes the coordinator can name**: **(3a)** a fixed limit on nesting,
counted where the parser opens a bracket, a block or a hole, told as a
diagnostic at the opener that passes it, exit 1, the same number on every
platform; it says nothing yet about the two chain kinds, which a counter of
openers does not see, and a seat that adopts it says what happens to them;
**(3b)** no limit, every walk made to hold any depth the input has (an
explicit stack, a stack grown on demand, or the compiler's passes run on a
thread whose stack it chooses); **(3c)** a limit on the depth of the tree the
parser builds, every kind counted, told at the node that passes it, with or
without the chains built so that no walk recurses per link; **(3d)** today's
abort.
