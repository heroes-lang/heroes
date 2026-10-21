# Panel 227, compiler-engineer

Read `docs/panel/227-briefs/00-shared.md` first. Your question is the cost, and whether the message
route builds.

**Your copy**: create it yourself with `git -C /Users/joseph/Temp/heroes/heroes-lang worktree add
--detach .claude/worktrees/227-compiler-engineer 63c78c54b`, and work only there. Build the
compiler from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a few
seconds) and stage 2 (`./heroes build selfhost/main.hero -o heroes-next`, about a minute), and
judge with `heroes-next`. Never use the lane's compiler or write in the lane, the trunk or any
other worktree; never write outside the repository's root. A program that may not end is run as
its built binary under `timeout`, its output bounded (`.claude/rules/verification.md` § A run that
may not end).

**Measure, and write each number with the command that produced it:**

1. Where a receiverless `slice` is told today: `builtin_shape` is pushed at
   `selfhost/check/walk.hero:1283` and built at `selfhost/contextless_errors.hero:173`;
   `wrong_label` goes `selfhost/check/labels.hero:42` (`inventory.labelled_of`) → `:76` →
   `selfhost/label_errors.hero:42` (the critic's reading; confirm or correct), and the built-in
   typing is `selfhost/check/builtins.hero`.
2. **Prototype route A's message in your worktree**: a `slice` call whose unlabelled first argument
   is missing draws one diagnostic naming the value sliced and the call's shape, `xs.slice(from: a,
   to: b)`, and not the `wrong_label` whose guess writes `from:` twice. Choose the code (an existing
   one or another) and say why; whether a new code is a new *diagnostic class* in CLAUDE.md § 4's
   sense is a question nothing in the rules defines (the critic's search), so answer it rather than
   assume it. Say whether any `Fix` is `certain` (design.md §4.17 and
   `.claude/rules/diagnostics-and-goldens.md`: a `certain` fix must repair the defect the
   diagnostic names) or `guess`, or none.
3. **Prototype route E beside it**: a built-in called with the wrong count told as a user
   function's is, one `wrong_arity` with the call's shape as its note (`cut(from: 0, to: 2)` on a
   user function is the model, the shared brief quotes it). Say which of `slice(from: 0, to: 5)`,
   `slice(from: 0)`, `slice()`, `slice(line, from: 0)`, `slice(0, 5)` and `slice(from: 0, to: 5,
   line)` each route tells in one diagnostic, with the text of each.
4. **Which arm sees the new message.** `wrong_label` is a thesis rule (`selfhost/diag.hero:210`) and
   `--permissive` drops it; `builtin_shape` and `wrong_arity` the critic did not find dropped. Say,
   for each route, whether its message is a thesis rule's, what `check --permissive` prints on the
   same programs, and what that means for Part 11's strict-minus-control contrast; and whether the
   `annotations` suite (every thesis rule a witness) moves.
5. **Run what the change owes**: the golden form `check` (`./heroes-next run
   tests/harness/main.hero -- ./heroes-next check`), `full`, `permissive` and `fixes`; the
   compiler's own tests (`./heroes-next test selfhost/main.hero`); `layout` narrowed to the files you
   touched, with the line counts against their `DECIDED` rows; and **with route A's sentence in your
   worktree's `spec/heroes-spec.md`** (copy
   `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis-drafts/227/spec/after-a.md` over
   it), the suites the map sends a spec change to: `spec`, `special`, `grammar`, `fixes`, `unseen`
   (the critic read `tests/harness/spec_text.hero:146-176` as skipping a fragment with a space and
   reading one without as a name, unrun). Panel 187's R2 corpus, `tests/golden/recovery/`, holds
   `slice(from:` lines: say what the message change moves there.
6. The shape beside it: `repeat`'s `type_mismatch` on an `i64` count. Is a note naming the
   conversion (`to_u64` gives a `u64?`, spec line 330) within §4.17's promise, and what does it
   cost? Note the 2 samples that wrote the conversion and still never went green.
7. Route D: what `repeat` taking an `i64` would cost, and what a negative count would do, against
   panel 054's ruling.
8. **The label a built-in accepts**: `len(foo: line)`, `repeat(s: "ab", n: 3)`, `join(xs: ["a"],
   sep: ",")`, `slice(xs: line, from: 0, to: 5)` exit 0 while a user function refuses `f(b: 1)`.
   Is it a defect, and of which class (`.claude/rules/verification.md` § Bounded discovery)? Do not
   repair it; say what you would file.

Write your report to `docs/panel/227-reports/compiler-engineer.md` as you go: verdict (approve /
object / veto) on each route, the section of design.md each rests on, the cost (lines, files,
ceilings), a falsifiable prediction, and the condition that would change your verdict. Leave your
worktree in place for the coordinator; do not commit.
