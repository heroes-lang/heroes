# Panel 227, shared brief: the specification's `Built-ins:` sentence and the message for a `slice` with nothing to slice

**Convened 2026-10-21** in M-thesis-harness, after its step 11 (measurement 050), on defects 2174
and 2175 (https://github.com/heroes-lang/heroes/issues/2174,
https://github.com/heroes-lang/heroes/issues/2175), at the author's request of that day in this
session, meant as *do point 1*, point 1 being these two defects through a sitting as the coordinator
had proposed. **Lane: full panel less the ffi-pragmatist**: the proposal changes a sentence of the
specification and a diagnostic's message, and touches no C, no ABI and no binding, so that seat's
input would not differ (CLAUDE.md § 4, *choosing only the seats whose input differs*); a
completeness critic read these briefs before any seat started (its report is
`docs/panel/227-reports/critic-briefs.md`, and § Repaired below says what it changed) and reads
the reports after.

**The tree**: the lane `thesis` at `63c78c54b` (`git log -1`, run 2026-10-21 at 12:05), `git status
--porcelain` empty but for this sitting's own two folders, frozen from the moment these briefs exist
until the synthesis is written. **No seat writes outside the repository's root** (CLAUDE.md § Hard
stops, the author's rule of 2026-10-08): a seat that builds works in its own detached worktree under
`.claude/worktrees/`, named in its brief; no seat reads, builds or runs in another's; **no seat
prints a credential**, and none starts a paid run its brief does not name.

## The proposal (route A), as a spec diff

`spec/heroes-spec.md` § 11, lines 316 and 317 (`grep -n 'Built-ins:' spec/heroes-spec.md` → 316):

```
-Built-ins: `print(...)` · `len` · `push` · `slice(from:, to:)` ·
-`chars` · `keys` · `join(xs, sep)` · `repeat(s, n)` · `sort` (ascending: …) ·
+Built-ins: `print(...)` · `len` · `push` · `xs.slice(from:, to:)` ·
+`chars` · `keys` · `join(xs, sep)` · `repeat(s, n)` (`n` a `u64`) · `sort` (ascending: …) ·
```

and in the checker: **a `slice` call whose first argument is missing is told as that**, one
message naming the value it slices, `xs.slice(from: a, to: b)`, and no `wrong_label` whose guess
writes `from:` a second time.

## What is true today, each with the command that settles it

- **The built-in's parameters**: `selfhost/inventory.hero:245`, `"slice" => ok(Labelled(name: name,
  params: ["", "from", "to"], shared: "i64"))`: an unlabelled first parameter, the value sliced, then
  `from:` and `to:`, both `i64`. `repeat` takes a `str` and a `u64` and nothing else:
  `selfhost/check/builtins.hero:267`, `allowed: "`str` and `u64`"`, ratified by panel 054
  (`docs/panel/054-a-cost-with-no-exit.md`, lines 103 to 108: *`repeat(s: str, n: u64) -> str`*, a
  computed count written `repeat(" ", col.to_u64().must())`).
- **The sentence** writes `slice(from:, to:)`, no first argument, and `repeat(s, n)`, no type, beside
  `join(xs, sep)`, which shows its first argument (`sed -n 316,322p spec/heroes-spec.md`). § 9 says
  `x.f(y)` is sugar for `f(x, y)` (`grep -n sugar spec/heroes-spec.md` → line 268), and the spec
  nowhere names `slice`'s first parameter (`grep -n slice spec/heroes-spec.md` → 313, 316, 322).
- **The messages today**, `heroes-next check` (the lane's stage-2 compiler, `sources
  79fe404274caacd0`) on `line = "hello world"`, `print(line)`, `part = slice(from: 0, to: 5)`,
  `print(part)` in a `main`:

  ```
  error[builtin_shape]: `slice` does not take 2 argument(s) of those types
    at slice_bare.hero:4:12
  error[wrong_label]: `slice` has nothing called `to` at this position — it is `from`
    at slice_bare.hero:4:27
    fix (guess): write `from:`
  ```

  the same two for an array (`xs = [1, 2, 3, 4]`, `head = slice(from: 0, to: 2)`); `--brief` drops
  the `fix (guess)` line, and **`--permissive` prints `builtin_shape` alone, since `wrong_label` is
  a thesis rule** (`selfhost/diag.hero:210`) that the control arm drops. On `n: i64 = 3`,
  `print(repeat("ab", n))`: `error[type_mismatch]: expected `u64`, found `i64`` at the count, with no
  note. The programs are in `.claude/worktrees/thesis-drafts/227/probe/`.
- **The shapes beside it** (the critic's, `check --brief` on each): `slice(from: 0)`, `slice(to:
  5)`, `slice()` and `slice(line, from: 0)` each draw `builtin_shape ... does not take N
  argument(s) of those types` and nothing naming what `slice` takes; `slice(0, 5)` draws
  `builtin_shape` and `needs_label`; `slice(from: 0, to: 5, line)` draws four. **A user function
  with the same mistake draws one diagnostic**: `function cut(s: str, from: i64, to: i64)` called
  `cut(from: 0, to: 2)` gives `error[wrong_arity]: `cut` takes 3 argument(s), found 2` with `note:
  declared at line 1: function cut(s: str, from: i64, to: i64) -> str`, and no `wrong_label`; so
  does `range(to: 3)`. **And a label naming nothing on a built-in's unlabelled parameter is
  accepted**: `len(foo: line)`, `repeat(s: "ab", n: 3)`, `join(xs: ["a"], sep: ",")` and
  `slice(xs: line, from: 0, to: 5)` exit 0, where `f(b: 1)` on `function f(a: i64)` is refused
  `wrong_label`; no issue on it was found by `gh issue list --state all --search 'label in:title'`,
  a question since that search is one vocabulary.
- **What the trial measured** (measurement 050 § 2; the strict arm's 90 turn-1 answers under the
  specification, `build/thesis-runs/00fb571c-v3` in the lane, each first fenced block put through
  `check --brief` by the critic): Haiku 5.5 refused 13 of 30, **9 holding a `slice` with nothing to
  slice, 7 of them with nothing else** and 2 (task `6f236494`, samples 1 and 2) with `repeat`'s
  count too; Sonnet 5.5 refused 11 of 30, 6 for `repeat`'s count and nothing else, none for
  `slice`; Opus 5.5 refused 7 of 30, 5 for `repeat`'s count and nothing else, none for `slice`.
  **18 of those 20 went green at turn 2**; 2 never did (`6f236494`, Haiku sample 2 and Sonnet
  sample 3), which wrote the conversion, `to_u64(...).must()`, at turn 2 and ran wrong. **The 13
  `repeat` refusals come from 2 of the 10 tasks** (`6f236494` 8, `c7a30e5b` 5), the 9 `slice` ones
  from 6. And the suite is the assistant's, whose numbers *never enter a published number*
  (`docs/metrics/tasks.md` § Two suites).
- **Two other faults of the same sentence in the same 90 answers** (the critic's): 3 Haiku refusals
  (`c7a30e5b` samples 2 and 3, `d37cfed1` sample 3) use `to_i64(...)` as an `i64`, where the
  sentence writes `to_i64` bare and line 330 says it gives an `i64?`; 2 answers define functions
  named `all` and `ok`, names the sentence lists (`builtin_name_taken`).
- **The price of route A**, counted 2026-10-21 on two copies of the specification
  (`.claude/worktrees/thesis-drafts/227/spec/before.md`, `cmp`-identical to the spec, and
  `after-a.md`), each request body and the API's answer kept beside them (`*.count.json`,
  `*.count.out`): `count_tokens` on `claude-opus-5`, the file as one user message, **10,292 before
  and 10,305 after**; the request's wrapper probed the way `heroes measure --refresh` probes it
  (`selfhost/cli/refresh.hero`'s `probed`: `x` answered 7 and `x x x x x` 11, four apart, so the
  wrapper is 6), so **in the ledger's unit 10,286 before, the stored figure, and 10,299 after,
  +13**. `heroes-next measure` on the copies: `claude-legacy` 7,708 → 7,718 and `cl100k_base` 7,846
  → 7,856. `heroes-next measure spec/heroes-spec.md` prints `real 10286 claude-opus-5, 2026-10-18 —
  the binding number`, a headroom of 6,098 against the 16,384 ceiling, of which the FFI floor
  mortgages 60.
- **The budget that binds before the ceiling**: design.md §1.6 (`sed -n 290,296p docs/design.md`)
  lets the consolidation block, the rows from M-buildable-structs to M-compatibility-promise,
  M-thesis-harness among them, spend at most **1,500** real tokens *before the thesis harness has
  run*, and the ROADMAP's § Where we are reads **238** of them spent. Whether measurement 050 is
  *the harness has run* is a question for the sitting, not a premise.
- **What the specification refused before**: panel 120 (`docs/panel/120-*.md`), *the spec buys
  prose and not signatures* (`.claude/rules/spec-shape.md` § What was refused, line 112).

## The routes the sitting judges

- **A**, the proposal: the two spellings in the sentence, and the `slice` message.
- **B**: every built-in in the sentence written with its parameters and types, `print(...)` to
  `range`; a larger rewrite of § 11, unpriced, and the route panel 120 refused in kind.
- **C**, conservative: the message alone, the specification untouched, 0 tokens. It cannot move
  turn 1, which sees no message; at turn 2 today's message already led all 9 `slice` samples to a
  receiver.
- **D**, not proposed: `repeat` taking an `i64` count, reopening panel 054. A negative literal is
  refused today (`int_out_of_range`) and so is an `i64` value (`type_mismatch`), but `repeat("-", 0
  - 1)` checks at exit 0 and aborts at its `u64` subtraction at run time
  (`tests/golden/check/repeat-count-is-unsigned.hero`'s header, panel 206's R3).
- **E**, the critic's: a built-in called with the wrong count told the way a user function's is,
  one `wrong_arity` with the call's shape as its note, an existing code reaching every arity at
  once, `slice(from: 0)` and `slice()` included.
- A route nobody listed is welcome, with what would make it right.

The repair of `repeat`'s message, a note naming the conversion beside `expected u64, found i64`,
is the shape next to 2175's and is open to the compiler-engineer.

## Repaired after the critic's first pass

The critic's first pass (`docs/panel/227-reports/critic-briefs.md`) found, and this brief now says:
the § 9 sentence is at line 268, not 271 to 272; the Haiku `slice` refusals are 9 with 7 alone,
not 9 alone; 2 of the 20 samples never went green; route D's negative count can reach run time;
the `count_tokens` figures had no stored answer (now kept, and re-measured) and were in another
unit than the ledger's (now both); the control arm drops `wrong_label`; and it added the shapes
beside the proposal, route E, the label a built-in accepts, panels 054 and 120, §1.6's budget, the
two tasks the `repeat` evidence comes from and the other two faults of the sentence.
