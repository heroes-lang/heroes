# Panel 184, brief for the compiler-engineer

Read `/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/00-shared.md`
first: the sitting, its three questions, and every measurement they rest on,
with the commands. (Repaired on the completeness critic's first pass, 16:51
onward: `docs/panel/184-reports/completeness-critic-briefs.md`.)

**Your directory** is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-compiler-engineer/`:
`git -C /Users/joseph/Temp/heroes/heroes-lang archive a294a6ff | tar -x -C <it>`,
`rm -rf build`, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
Never build or run in the repository's working tree or another seat's
directory; in the working tree, read only this sitting's briefs directory,
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/184-briefs/`, and copy its
programs into your directory from there (`probes/after-*.hero`,
`probes/depth.py`). Rebuilding the compiler from `selfhost/` is `./heroes build
selfhost/main.hero -o heroes-next` (about a minute); prototype there. The
recovery instrument is the coordinator's and you may run it against your own
compilers, never into the coordinator's folders:
`python3 /private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/instrument/tool/recovery.py
--compiler <yours> --out <your directory>/inst-<name> --only <operators> --no-pairs
--jobs 2` (it reads its own fixed tree and writes only into `--out`; the
coordinator's run of both operators took 2.8 minutes of wall clock). Its
mutated corpus moves with the machine's load (00-shared.md § Question 1): pass
`--dear` large enough that your before and after runs mutate the same files,
and say what you passed. It is a local run, not a paid one.

## Your tasks, in order

1. **Question 1, the forgotten `f`.** Where does each of 00-shared.md's routes
   (1a) to (1d) live in the compiler, and what does it cost: `selfhost/scan.hero:71-83`
   lexes an `f` literal through `selfhost/lex_interp.hero`, and
   `selfhost/scan.hero:69-70` hands a plain one to `literals.string`
   (`selfhost/literals.hero`). Then the count that decides (1a) against
   (1b): **every plain literal in the tracked tree at `a294a6ff` that (1a)
   would refuse**, and every one (1b) would, with the real lexer and not a
   regular expression, each named with its file and line, split by what it is
   (diagnostic text naming a map type, a JSON or C format string handed to a
   binding, a test's expected output, a forgotten `f`). The coordinator did
   not count them. Prototype the route your counts favour in your copy, run the
   instrument's `forget-f` against it, and give the census: `check --brief` and
   its exit over every tracked `.hero` file, your prototype against
   `a294a6ff`'s compiler, both arms (`--permissive` too), every moved file
   named with why. Say whether the fix is `certain` or `guess` by the rule in
   `.claude/rules/diagnostics-and-goldens.md:22` (*a `certain` fix repairs the
   defect the diagnostic names*), and whether the rule reaches a literal inside
   a hole of an `f` literal (one of the 25 `forget-f` sites is one).
2. **Question 2, a statement after a jump, and `missing_return` with it.**
   00-shared.md § Question 2 measured that `missing_return` demands a `return`
   after `exit(code:)`, after `assert false` and after `while true`, and counts
   an `if`/`else` whose branches all return as leaving. So (2a) is ONE
   predicate, *this statement always leaves its block*, read by both rules.
   Prototype it in `block` (`selfhost/check/walk.hero:690-720`) and in
   `missing_return`'s walk together, from what the checker already knows
   (`Outcome.jumps`), and settle what it covers: the three jump words; an
   `if`/`else` or `match` every branch of which leaves; `exit(code:)`, the
   built-in that ends the program (`spec/heroes-spec.md:319`); `assert false`;
   `.must()` on a value the checker knows is a failure, if it can know one;
   `while true` with no `break` out of it; a call to a function every path of
   which does one of those, if the checker can know it (and if not, why not).
   (`panic` is not a name the language has: `panic: ...` is the runtime's
   message.) Run the five probes, the four `missing_return` programs of
   00-shared.md (`<p184>/deep2/exit-last.hero` and the three beside it), the
   instrument's `over-indent` with `--per-op` large enough to cover more than
   the 150 of 00-shared.md (say how many), and the same census as task 1: **a
   program in the tracked tree that the rule refuses, or that loses a `return`
   it no longer needs, is either a real defect of that program or a reason
   against the rule**, and you say which for each. Where does the message
   point (the first statement after the jump, or the jump), what does it say,
   and does the over-indented case earn a fix of its own (the statement one
   level deeper than the jump's block, moved back)? And (2b), for comparison:
   the three jump words alone, `missing_return` unchanged.
3. **Question 3, how deep a source may nest.** Which walks recurse once per
   level of each shape in 00-shared.md's table, with the frame each spends.
   The panics name at least eight: `grammarexpr.postfix`
   (`selfhost/grammar_expr.hero:279`), `grammarexpr.unary` (`:257`),
   `grammarexpr.binary` (`:201`), `checkwalk.synth`
   (`selfhost/check/walk.hero:96`), `checktable.ty_key`
   (`selfhost/check/table.hero:128`), `resolvewalk.expr`
   (`selfhost/resolve/walk.hero:107`), `resolvenames.bare_name`
   (`selfhost/resolve/names.hero:29`), `resolvequalified.qualified`
   (`selfhost/resolve/qualified.hero:35`), `cursor.pass_comments`
   (`selfhost/cursor.hero:120`), and past `check`, `checklower.named`
   (`selfhost/check/lower.hero:66`); the lowering and the emitter may have
   more, and the function named is where the stack ran out, which moves between
   builds (00-shared.md). Then: **the deepest nesting the tracked tree holds
   today** in each of the three kinds 00-shared.md names (nesting a reader
   sees, one-operand chains, flat chains: `+`, `&&`, method chains), measured
   (the margin between what programs use and where the compiler aborts is the
   number this question turns on); **whether the compiler ever parses or checks
   a module on a thread other than the main one** (`selfhost/cli/`,
   `runtime/parts/thread.c`), since a thread's stack is not the main thread's
   (panel 107 measured Darwin's library threads at 512 KB) and a limit must
   hold on the smallest stack the compiler runs on; **for (3a)**, one counter
   in the parser bounding every later walk, the number it would take, what
   makes that number the same on every platform and under any `ulimit -s` the
   compiler accepts, and what it does with the two chain kinds its counter
   does not see; **for (3c)**, the same for a limit on the depth of the tree
   the parser builds; **for the chains**, whether the parser can build `1 + 1 +
   ... + 1`, `- - - 1` and `v.a().b()...` so that no later walk recurses per
   link, or each walk can iterate over them, and at what cost in lines; **for
   (3b)**, what an explicit stack, a stack grown on demand, or the passes run
   on a thread of a chosen stack size cost in `selfhost/` and `runtime/`, and
   whether the stack guard (`runtime/parts/stack.c`) stays sound under each.
   Prototype the route you favour far enough that 00-shared.md's table
   re-measured under it is real, and give that table.
4. **Cost and speed**: lines per route against `tests/harness/suite_layout.hero`'s
   measure for the files each touches, and the compiler's own speed before and
   after on `./heroes build selfhost/main.hero`, timed with `/usr/bin/time -p`
   on a still machine or reported as unrun with why (the machine is shared:
   three lanes and the formatter's probe run beside the sitting, so `real`
   far above `user` plus `sys` says the run waited and is discarded).

Your verdict per question: approve, amend or refuse each route, with the
condition that would change it; your veto is a refusal on soundness, and says
which program it would break. Write your report to
`docs/panel/184-reports/compiler-engineer.md` in the repository AS YOU GO (the
one file you write there), so a stalled run leaves what it had. No paid run
(`claude -p`, an API call, a cloud run). English, no em dashes.
