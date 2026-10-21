# Panel 227, completeness critic, first pass: the briefs

Read 2026-10-21, 12:21 to 12:31, in the lane `thesis` at `63c78c54b`, every command run from the
lane's root with its `./heroes-next` (`--version`: `sources 79fe404274caacd0`). Read-only: the one
file written is this one (`find . -newer docs/panel/227-briefs/critic.md -type f` printed nothing
before it). No credential printed, no paid run, no API call. Programs probed beside the briefs'
were piped to `./heroes-next check /dev/stdin`, so no probe file was written.

## 1. The framing facts

### Shared brief (`00-shared.md`)

| # | fact | command | verdict |
|---|---|---|---|
| S1 | the lane at `63c78c54b`, `git status --porcelain` empty | `git log -1`; `git status --porcelain` | **partly**: HEAD holds; porcelain prints `?? docs/panel/227-briefs/` (the briefs themselves, and this folder from now on) |
| S2 | defects 2174 and 2175 | `gh issue view 2174` / `2175 --json number,title,state,labels,body` | **holds**: both OPEN, `kind:defect`, `class:adjacent`, `area:spec` and `area:compiler` |
| S3 | the milestone's once-per-milestone question asked at panel 226; the author's *do point 1* | `sed -n 1,20p docs/panel/226-*.md` | **partly**: 226 says it was convened on the author's answers of 2026-10-19 and instruction of 2026-10-20; no line of it says *once per milestone*. *Do point 1* is in no file: not checkable by command |
| S4 | `grep -n 'Built-ins:'` → 316; the diff is lines 316 and 317 | that grep; `diff before.md after-a.md` | **holds** (`316,317c316,317`, nothing else) |
| S5 | `selfhost/inventory.hero:245`, `"slice" => ok(Labelled(... params: ["", "from", "to"], shared: "i64"))` | `sed -n 245p selfhost/inventory.hero` | **holds**, verbatim |
| S6 | `repeat` takes `str` and `u64` and nothing else, `selfhost/check/builtins.hero:267` | `sed -n 260,275p selfhost/check/builtins.hero` | **holds**: line 267 inside the `given.len() == 2` arm at 262 |
| S7 | § 9's `x.f(y)` sugar at `sed -n 271,272p` | `sed -n 271,272p spec/heroes-spec.md`; `grep -n sugar spec/heroes-spec.md` | **does not hold as cited**: 271-272 print the `@` parameter bullet; the sentence is **line 268**. The claim holds |
| S8 | the spec nowhere names `slice`'s first parameter | `grep -n slice spec/heroes-spec.md` → 313, 316, 322 | **holds** |
| S9 | today's messages on the three probes | `./heroes-next check` on `slice_bare.hero`, `slice_bare_array.hero`, `repeat_i64.hero` | **holds** byte for byte, `type_mismatch` at 3:24 with no note. Not said: `--brief` drops the `fix (guess)` line, and `--permissive` prints `builtin_shape` alone, since `wrong_label` is a thesis rule (`selfhost/diag.hero:210`) |
| S10 | the compiler is the tree's, `sources 79fe404274caacd0` | `./heroes-next --version`; `python3 -I -B -c '... provenance.tree_sources(<lane>)'` | **holds**: the mirror reads `79fe404274caacd0` for the tree, though the binary (Oct 20 12:46) predates `9ca6f1dfe` (13:33), the newest commit under `selfhost/` |
| S11 | 050 § 2: Haiku 13, Sonnet 11, Opus 7 refusals at turn 1, strict; green@3 28, 29, 30 | Python tally of `build/thesis-runs/00fb571c-v3/verdicts.jsonl` | **holds**: `compile_error` 13, 11, 7 (Haiku's 14th failure is a `timeout`); `green_turn` 1 to 3: 28, 29, 30 |
| S12 | Haiku 9 *slice and nothing else*; Sonnet 6 and Opus 5 *repeat and nothing else*, none `slice` | each of the 90 turn-1 answers (`generations/*.sN.json`, first fenced block) piped to `./heroes-next check --brief /dev/stdin`, each diagnostic classed by its message and source line | **partly**: Haiku **9 hold a receiverless `slice`, 7 with nothing else, 2 (task `6f236494`, s1 and s2) with `repeat`'s count too**: these are 050's *2 `repeat`'s count*. Sonnet 6 and Opus 5 hold |
| S13 | every one repaired later except as 050's table says | the same tally, `turns` and `green_turn` per key | **partly**: 18 of the 20 went green at turn 2. **2 never did**: `6f236494` Haiku s2 and Sonnet s3 compiled at turn 2 with `repeat(letter, to_u64(count).must())` and `to_u64(cnt).must()` and ran wrong (`compiled_wrong`). They are 2 of the 3 strict samples that never went green, and the silent@3 of Haiku and Sonnet |
| S14 | the copies: `before.md` is the spec, `after-a.md` is route A | `cmp before.md spec/heroes-spec.md`; `shasum -a 256` | **holds**: identical; the spec's sha256 `21226405...` is also the trial's `context_sha` (`provenance.json`) |
| S15 | vendored counts 7708 → 7718, 7846 → 7856 | `./heroes-next measure` on both copies | **holds** |
| S16 | `count_tokens` 10,292 → 10,305 | none possible here (no API call) | **unrun by this critic**. The request bodies `before.count.json` and `after-a.count.json` hold each copy byte for byte (Python compare: `content == file` True, both), and no stored response holds either number (`grep -rn 10292\|10305` hits the briefs alone) |
| S17 | `measure spec/heroes-spec.md`: real 10286, headroom 6098 against 16384 | `./heroes-next measure spec/heroes-spec.md` | **holds**. Not said: the same output says the FFI floor mortgages 60 of it (10346 against the ceiling) |
| S18 | route D: *a negative count needs a run-time answer the `u64` refuses at compile time* | `repeat("ab", -1)`, and `repeat("-", 0 - 1)` piped to `check` | **partly**: a negative literal is refused (`int_out_of_range`), an `i64` value too (`type_mismatch`), but `repeat("-", 0 - 1)` checks at exit 0 and aborts at its `u64` subtraction at run time (`tests/golden/check/repeat-count-is-unsigned.hero`'s header, panel 206 R3) |

### Compiler-engineer's brief

| # | fact | command | verdict |
|---|---|---|---|
| C1 | the paths: `check/labels.hero`, `label_errors.hero`, `contextless_errors.hero`, `check/builtins.hero` | `ls`; `grep -rn builtin_shape selfhost`; `grep -n 'wrong_label\|labelled_of' selfhost/check/labels.hero` | **partly**: all four exist; `wrong_label` goes `labels.hero:42` (`inventory.labelled_of`) → `:76` → `label_errors.hero:42`. But `builtin_shape` is pushed at **`selfhost/check/walk.hero:1283`** (built at `contextless_errors.hero:173`), which the list does not name |
| C2 | `to_u64` gives a `u64?` | `sed -n 322,333p spec/heroes-spec.md` | **holds**: line 330, *`to_i8` … `to_u64` give a `T?`* |
| C3 | *a new code is a new diagnostic class* | `grep -rn -i 'diagnostic class'` over `.claude/skills .claude/rules CLAUDE.md docs/records/contract/case-law.md`; `docs/design.md` | **not backed by a search**: only the panel skill's trigger lines 3 and 26, and CLAUDE.md § 4, name the class; nothing defines it as one code. A question, not a premise |
| C4 | the forms named: `check`, `layout` | `grep -n '"check"\|"layout"' tests/harness/main.hero` | **holds** (`main.hero:600`) |
| C5 | the worktree route from the trunk | `git -C <root> cat-file -t 63c78c54b` → `commit`; `ls -d .claude/worktrees/227-*` → none | **holds** |

### Spec-warden's brief

| # | fact | command | verdict |
|---|---|---|---|
| W1 | +13 on `count_tokens`, +10 on each vendored table | as S15 and S16 | vendored **holds**; real **unrun here** |
| W2 | the ledger, `docs/measurements/010-spec-budget-ledger.md`, and panel 012's rule | `ls`; `grep -n 'named removal\|pre-registered' docs/panel/012-*.md` | **holds** (012, lines 45-46) |
| W3 | `after-b.md` under `thesis-drafts/227/spec/` is inside the root and ignored | `git -C <root> check-ignore -v` | **holds** (`.gitignore:34`, `.claude/worktrees/`) |

### Historian's brief

| # | fact | command | verdict |
|---|---|---|---|
| H1 | Python's `str.split(sep=None)` | `python3 -I -c 'help(str.split)'` (3.15.0) | **holds**: `split(self, /, sep=None, maxsplit=-1)` |
| H2 | Rust `E0061`, `E0308` | `rustc --explain E0061`, `E0308` (1.90.0) | **hold**: *an invalid number of arguments*, *expected type did not match* |
| H3 | Rust's `str::repeat` takes a `usize` | `grep -n 'pub fn repeat' ~/.rustup/.../alloc/src/str.rs` | **holds**: `pub fn repeat(&self, n: usize) -> String` (line 531) |
| H4 | Swift's *missing argument for parameter*, Go's `builtin`, Elm, Kotlin | none | **the historian's own to verify; unrun here** |

### Blind folders (`blind/README.md`)

| # | fact | command | verdict |
|---|---|---|---|
| B1 | `p` is route A, `q` today; they differ in lines 316-317 alone | `cmp p/heroes-spec.md after-a.md`; `cmp q/... before.md`; `diff q p` | **holds** |
| B2 | brief and tasks the same in both folders and in `blind/` | `cmp` of each | **holds** |
| B3 | `GIT_CEILING_DIRECTORIES` hides the repository | `GIT_CEILING_DIRECTORIES=<227-blind> git rev-parse --show-toplevel` in `p` | **holds**: `fatal: not a git repository`; without it, the root |
| B4 | `--restricted`, `--safe-mode`, `--strict-mcp-config` do what the README says | `claude --help` (2.1.304, no session started) | **holds**, word for word. The one `CLAUDE.md` above the folders is the root's (`ls` up the chain), so `--safe-mode` is what keeps it out |
| B5 | *where they ran* | `ls -la p q` | **not yet**: each holds its four inputs and no `run.json` or `report.md` |
| B6 | *inside the author's budget of 2026-10-21* | 050's header | **unverified**: 050 records $80 for the trial ($43.54 spent by its § 1 table); no file read names panel 227's sessions inside it |

## 2. The gaps

**G1. The blind seat runs the one model that never made the `slice` mistake.** The README runs
`--model claude-sonnet-5-5`. In 050's 90 answers every receiverless `slice` is Haiku's (S12):
Sonnet's 0 of 30 says a Sonnet session at n = 1 per variant cannot tell `p` from `q` on `slice`.
It can on `repeat`. A Haiku session per folder is the route that measures 2174's `slice` half.

**G2. The `repeat` evidence is two tasks, not thirteen samples.** By task (S12's run), the 13
`repeat` refusals are `6f236494` (8) and `c7a30e5b` (5), 2 of the 10 tasks; the 9 `slice` ones are
spread over 6 tasks. 050 § 3 reads the within-task ICC at 0.614 for Sonnet and 0.477 for Opus. And
the suite is `assistant` (`provenance.json`), whose numbers *never enter a published number*
(`docs/metrics/tasks.md` § Two suites). The spec-warden's prediction, scored on *the next run of
the trial's turn 1*, would be scored on the same ten tasks, in sample. No brief says so.

**G3. Route C cannot move turn 1, and in this trial it had nothing left to move at turn 2.** Turn
1 sees no message, so only the spec can move 050's turn-1 refusals. And today's message already
worked: all 9 `slice` samples wrote a receiver at turn 2 (their turn-2 receivers are `line`,
`text`, `s`, `key`, `ln`, `stack`), and both `repeat` samples that never went green had already
written the conversion (S13). Nobody is asked what a measured effect of the message route could
be, or which instrument would see it.

**G4. Which arm sees the new message decides what Part 11 can see.** `wrong_label` is a thesis rule
(`selfhost/diag.hero:210`) and `--permissive` drops it; `builtin_shape` and `type_mismatch` it
does not (S9). Whether route A's message replaces a thesis rule, a non-thesis one or neither
decides whether it moves the control arm too, and so whether it can move strict minus control,
050's primary. The compiler-engineer is asked to choose the code, not its thesis status, and is
not pointed to the `annotations` suite, which needs a witness for every thesis rule.

**G5. A route nobody listed, with the code already there.** A user function with the same shape
(`function cut(s: str, from: i64, to: i64)` called as `cut(from: 0, to: 2)`) draws one
diagnostic, `error[wrong_arity]: `cut` takes 3 argument(s), found 2`, with `note: declared at
line 1: function cut(s: str, from: i64, to: i64) -> str`, and no `wrong_label`. So does
`range(to: 3)`. Telling a built-in's wrong count the way a user function's is told, with the
call's shape as the note, uses an existing code and reaches every arity at once.

**G6. The shapes beside the one the proposal names, unlisted** (all piped to `check --brief`):

- `slice(from: 0)`, `slice(to: 5)`, `slice()` and `slice(line, from: 0)` each draw
  `builtin_shape ... does not take N argument(s) of those types` and nothing naming what `slice`
  takes;
- `slice(0, 5)` draws `builtin_shape` plus `needs_label`;
- `slice(from: 0, to: 5, line)` draws four: `bad_operand`, `wrong_label`, `needs_label` and
  `type_mismatch`;
- **a label naming nothing on a built-in's unlabelled parameter is accepted**: `len(foo: line)`,
  `repeat(s: "ab", n: 3)`, `join(xs: ["a"], sep: ",")`, `slice(xs: line, from: 0, to: 5)` and
  `slice(s: line, ...)` all exit 0, while `f(b: 1)` on `function f(a: i64)` is refused
  `wrong_label`. `gh issue list --state all --search 'label in:title'` shows defect 025 (1126,
  closed: the `needs_label` half) and no issue on this one. A question, since that search is my
  vocabulary.

**G7. Prior rulings no brief cites** (`grep` of the briefs for `054`, `120`, `1,500`,
`consolidation`, `permissive`, `held-out` and `wrong_arity` prints nothing):

- **panel 054**, which ratified `repeat(s: str, n: u64)` and the idiom
  `repeat(" ", col.to_u64().must())` (lines 103-108, 149). Route D reopens it.
- **panel 120**, *the spec buys prose and not signatures*, listed in `.claude/rules/spec-shape.md`
  § What was refused. Route B, and route A's `(`n` a `u64`)`, meet it.
- **design.md §1.6's consolidation block** (lines 290-294): rows 66 to 74, M-thesis-harness (70)
  among them, may spend at most 1,500 real tokens before the thesis harness has run; the ROADMAP
  (line 23) reads 238 spent. Nobody is asked whether 050 counts as *the harness has run*, nor to
  price +13 against that budget rather than against the 16384 ceiling alone.

**G8. Two units of `real` side by side.** The brief sets 10,292 (the file as one user message) next
to the pinned 10,286. `selfhost/cli/refresh.hero:153-186` subtracts the request's wrapper,
probed at 6 on `claude-opus-5`, and 10,292 − 10,286 = 6. So after route A the ledger's `real`
would read about **10,299, not 10,305**. That is an inference from reading the code, not a run.
The warden is told it needs no API route; the row it owes is in the subtracted unit.

**G9. The spec suites the change owes are given to no seat.** `.claude/rules/verification.md`'s map
sends `spec/heroes-spec.md` to `spec`, `special`, `grammar`, `fixes` and `unseen`. By reading
`tests/harness/spec_text.hero:146-176` (not run): `spec/offered` skips any fragment holding a
space, so `slice(from:, to:)`, `join(xs, sep)`, `repeat(s, n)` and route A's
`xs.slice(from:, to:)` are never checked as names. It strips prose parentheses, `(`n` a `u64`)`
included. And a route-B fragment with no space, such as `len(xs)`, would be read as the name
`len(xs)` and turn `offered` red. Only the compiler-engineer has a worktree where route A's text
could stand at `spec/heroes-spec.md` and those suites run.

**G10. The other entries' fault is already in the data, and the warden is not handed it** (its Q3).
Among the same 90 answers: 3 Haiku refusals (`c7a30e5b` s2 and s3, `d37cfed1` s3) use
`to_i64(...)` as an `i64`, though line 330 says it gives `i64?`. The sentence writes `to_i64`
bare. 2 more define `all` and `ok`, names the sentence lists (`builtin_name_taken`).

**G11. Smaller points.**

- **`xs.` names an array** by the spec's own habit (`join(xs, sep)`, `xs.push(4)`), but all 9 Haiku
  turn-2 repairs slice a receiver named like text (`line`, `text`, `s`, `key`, `ln`), two of them
  also `stack`; names read, types not checked. Nobody is asked whether
  `xs.slice` reads as arrays only. The blind tasks both slice a `str`, so they would show it, at
  n = 1.
- **The blind brief asks the reader to quote, for every built-in call, the sentence it took the
  shape from.** That points it at the very line under test, which the trial's prompt
  (`docs/metrics/first-try.md`) does not. The two measure different things.
- **No `check` golden holds the receiverless shape.** `grep -rln 'slice(from:' tests/golden`
  finds none under `check/`, and the four goldens that pin a `slice` `wrong_label` all have a
  receiver. The case is owed. Panel 187's R2 corpus (`tests/golden/recovery/`) holds
  `slice(from:` lines and would read the message's change; nobody names it.
- **Who writes the llm-ergonomist's verdict?** Of the four seats in the lane, it has no brief of
  its own, and its veto has no author: the blind sessions write programs, not a verdict.
