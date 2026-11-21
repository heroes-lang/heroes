# Panel 252, compiler-engineer

Read `00-shared.md` first, whole: the frozen commit (`181555e9a32d0b18878f9f10e48d2eadd939c285`, stage 2
`sources 23c7d2a626b29fd5`), the five questions with their witnesses, arms and counts, and the rules. You
judge from the compiler's and the runtime's side, with a veto on soundness: a veto names the program that
dies in silence, is refused while correct, or corrupts memory under the route vetoed, **built and run**.

**Your folder** is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-compiler-engineer/`,
made by you as `00-shared.md` § Where each seat works shows (a `git archive` of the frozen commit, the
`.git` file, the seed in about 10 s and stage 2 in about 134 s for the writer). Binaries `ce-...`. **Your
report** is `.claude/worktrees/rebel-round/docs/panel/252-reports/compiler-engineer.md`, written as you go,
in `.claude/agents/compiler-engineer.md:36-44`'s structure (`verdict`, `section`, `implementation_cost`
with file citations, `needed_for_self_hosting`, `argument` of at most 120 words, `prediction`,
`condition`), **one block per question**, each after the runs it rests on. No paid run, no credential, no
`heroes test` over the compiler or a package (a test of a small program of your own is allowed), nothing
written outside your folder and your report. **The writer's programs are in `bw/`**
(`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-briefwriter/`), read-only:
copy what you run.

## The cheap route to a compiler with a change

Edit your copy's `selfhost/` and rebuild stage 2 from your seed (`./heroes build selfhost/main.hero -o
heroes-<route>`, 134 to 140 s for the writer at load 12 to 16), whose `sources` key then moves; say the
key of each compiler you report from. A form's cases run through the harness narrowed by its third word,
`./heroes-<route> run tests/harness/main.hero -- ./heroes-<route> <form> <substring>`, its output whole into
a file (the guard refuses a gate piped into `head` or `tail`). **A change to what the checker refuses is
judged by every golden tree** (`.claude/rules/verification.md` § the rule *a new checker rule*): `check`,
`run`, `emission`, `determinism`, `corpus` narrowed where you can say why, and the census below where you
cannot. **The route you would adopt, you build and run before the synthesis**; one that does not build is
reported as not built.

## The census each question may need

`heroes-<route> check <root>` over every root `.hero` (a file with `function main()`) under `selfhost/`
(`selfhost/main.hero` alone), `examples/`, `tests/golden/run/` and `tests/golden/check/`, and Rebel's
drivers `examples-rebel/reach_*.hero` (160 of them, `ls examples-rebel | grep -c '^reach_'`; each reaches its package through
`examples-rebel/heroes.toml`'s `path` entry), frozen stage 2 against your route,
`xargs -P 2` at most, every file whose verdict or message moves named. Say what one check costs before
running all of them. **The critic's count, carried** (`docs/panel/252-reports/critic-briefs.md:258-261`):
1,852 roots per compiler (`examples` 71, `tests/golden/run` 837, `tests/golden/check` 783, 160 drivers and
`selfhost/main.hero`), one Rebel driver checked in `real 0.71` at load 9.8, about 11 minutes of wall a census
at `-P 2`; **the frozen compiler's census is shared by every route, so run it once**.

## Question 1: lift `module_cycle`, and what rests on it

- **Build Q1-R** in your copy: `cycles` in `selfhost/modules.hero:280-334` removed or answering at once,
  and every place that then needs a word. The writer's two-line probe is `bw/q1/heroes-nocycle`
  (`sources 7023dd011002b5d5`, its patch `bw/q1/nocycle.diff`, the patched file `bw/q1/modules.hero.patched`;
  the critic rebuilt it from those two lines and read the same key; a probe, not a route): on `bw/q1/cycle/` it checks, builds by units, prints `25`, `--emit-c` writes 11,830 bytes, a warm
  rebuild after an edit inside the ring prints `26`, and a `test` across the ring passes.
- **What rests on acyclicity, read and then run**: the comments `grep -rn module_cycle selfhost` prints
  (`cli/digest.hero:6`, `parse/decl.hero:22-30`, `ir/flatten.hero:14-19`, `parse/use_line.hero:21`,
  `emit/probe_reply.hero:17`, `grammar_expr.hero:8`, `mutate/typo.hero:30`, `mutate/sites.hero:14`); the
  per-module build and its key (`cli/units.hero:7-20`: *"the TU's own emitted text"*, read by the writer
  and not measured), `cli/deps.hero`, `emit/provenance.hero` (the key of the modules a root reaches), a
  package's placement walk (`selfhost/module/`, panel 250's), `heroes doc`, the cards
  (`tests/harness/package_cards.hero`). Panel 031's R6 refused cycles *"where the per-module cache needs a
  topological order"* (`docs/panel/031-modules-the-namespace.md:193-198`): **does anything need that order
  today?**
- **The cases** (write each, run each under the frozen stage 2 and your route): the face and its private
  module (`bw/q1/cycle/`); a ring of three modules; a generic function on the ring called across it;
  constants on the ring (`selfhost/resolve/cycles.hero`'s `constant_cycle` across modules); a record held
  by value across two modules of a ring (`selfhost/check/sized.hero`'s `no_size` cycle, *"A holds a B,
  then B holds an A"*); an `extern` group in two modules of a ring (spec § 13: *"C reads a module's
  headers in the order its groups are written"*); a warm rebuild after an edit to each ring member; a
  package with a ring placed through a catalogue's `path` entry. **No golden holds `module_cycle`**
  (`grep -rl module_cycle tests/golden` prints three frozen corpus comments and no case); its test is
  `selfhost/modules.hero:431-432`.
- **Price Q1-A** beside Q1-R (the critic's finding 7; arm `bw/arms/q1-a.diff`, +54): the face names its
  private module's type as its own, by a transparent alias (`alias Point = types.Point`, panel 135's
  settled form, *"transparent, and *everywhere* reaches the constructor and `::`"*,
  `docs/panel/135-*.md:144`) or a re-export, against panel 031's R4, *"There is no re-export"*
  (`docs/panel/031-modules-the-namespace.md:178-182`): what the resolver, the checker, the printer, the
  cards and `heroes doc` owe, and whether it removes Rebel's mirrors as Q1-R would, at a price you count.
- **Price Q1-R2** beside Q1-R: a ring allowed only through a module and the modules under its own
  `internal/`, every other cycle refused, and what it costs the reader (a rule to state).
- **The compiler's own knots**: under Q1-R, can `ir/flatten.hero` (1,414 lines) or `grammar_expr.hero`
  (1,767) be split (decision 523)? A sentence, unbuilt, is enough.
- **What it would let Rebel delete**, counted where a command can count it: the mirror modules and the
  codes written twice (`00-shared.md` § Question 1's table; `wc -l` of each named file).

## Question 2: the key that is not read

- **Reproduce** `bw/q2/`'s **20** programs (copy them; the first brief said *sixteen* and held 15) with your
  stage 2: `a`, `k`, `o`, `g`, `l`, `p`, `q` refused with *"no other argument and no context says what `K`
  is"*, `i` naming `K` and `V`, `e` refused rightly, the other eleven accepted. **The order inside one
  literal decides** (measured after the critic's reading): `{"a": [], "b": ["c"]}` (`p`) and `[[], ["a"]]`
  (`q`) refused where `{"a": ["b"], "b": []}` (`n`) and `[["a"], []]` (`s`) are accepted: the literal is
  typed from its first element or value, and nothing of it is read when that one gives no type. **Find why**: `selfhost/check/generic_argument.hero:285` builds the message,
  `check/waiting.hero` (236 lines) holds panel 203's waiting arguments, and `check/walk.hero`'s
  `synth_argument` (1790) and `settle_waiting` (1806) are where they are tried (`grep -n '^function'`).
- **Build Q2-R**: the typed parts of a waiting literal (a key, any typed entry, in any position) settle the
  callee's letters before the forms that refuse themselves; then `a`, `k`, `o`, `g` (`K` an `i64` from the
  literal's default, by § 9's *"a literal among them"*), `l`, `p` and `q` accepted and run, every pair of
  orders one verdict, `i` refused for `V` alone, `e` still refused; panel 203's order pairs (`first(a: [1], b: [])` and its mirror, `{"k": 1}` with `{}`,
  `[[1]]` with `[[]]`) keep one verdict in either order. **The census**: the 12 `check` expectations
  holding `cannot_infer` (`grep -l cannot_infer tests/golden/check/*.expected | wc -l`) and the roots of §
  The census, every move named; a program that moves from refused to accepted is the defect's repair, one
  that moves the other way is a finding.
- **Say whether Q2 is a defect or a ruling**: if Q2-R builds and moves nothing else, it is a `blocking`
  defect under CLAUDE.md § 12, and Q2-C's sentence (+14, repaired to *"nor does a literal whose first element or value gives none"*, true
  of the 20) buys nothing; say so with the counts.

## Question 3: a record or a variant with a type parameter

- **Price Q3-R** across the compiler, by the method panels 119 and 251 used (*"a new `Ty` case is N arms
  in M files"*: `docs/panel/119-*.md:66`, `docs/panel/251-*.md:143`, carried; make the compiler count, do
  not estimate): the parser (`selfhost/parse/members_below.hero:223`, today's refusal; `parse/records.hero`
  206 lines, `parse/type.hero` 660, its `Prefix`), the type table, inference at a construction and in a
  `match` on a generic variant, field access under substitution, the monomorphiser over types, the mangler
  and the C emitted (one struct per instance), the descriptors the runtime frees by
  (`runtime/parts/desc.c`), the formatter, `heroes grammar`, the cards and `heroes doc`. **The record it
  extends** (the critic's finding 8): **panel 029**, whose **R4** refuses polymorphic recursion in the pass
  (*"`f<T>` calling `f<[T]>` instantiates forever"*, `docs/panel/029-generics-by-monomorphisation.md:110`) and
  whose **R5** names an instance by its canonical rendered type (`:180`); say how each reaches a type.
  **The shapes beside, each owed an answer** (design.md §1.12: refused at the declaration, or built, never
  a hang or a C stack overflow): `record Nest<T>` holding `inner: [Nest<[T]>]`, which instantiates without
  end as 029's `f<[T]>`; and a recursive generic variant, `variant List<T>` with a case holding a `List<T>`.
  **Build the smallest slice that runs** if it can be built in your folder in the sitting's time: the
  arm's own example, `record Pair<A, B>` holding `first: A` and `second: B` (arm `bw/arms/q3-r.diff`,
  repaired: its first example, a `Ring<T>`, was the blind task's answer), and the ring of the blind seat's
  task 3 (`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/252-llm-ergonomist/tools/task-t3.md`,
  read-only; the writer's reference under the frozen spec is `bw/ref/t3/main.hero`, `3 4 5 green blue`).
  **The blind grading no longer rests on it** (decision (c)): both arms of task 3 are graded by one
  written rubric; a slice that builds the `q3-r` answers is recorded beside the grade.
- **Principle 0** (CLAUDE.md § 2): does the compiler need it? Read `selfhost/` for record shapes written
  twice for two element types, or a shape and a list passed side by side (the `containers` pattern), and
  count them by a command you name. A form enters on compiler need **or** a measured thesis effect (the
  blind seat's task 3).
- **The silent mismatch, measured by the writer** (`bw/q3/`, `bw/tree/examples-rebel/bw_deque_long.hero`
  and `bw_deque_short.hero`): `deque.pop_front(@jobs_queue, @urgent)` is accepted and prints `taken 10`,
  exit 0, 3 of 3 each, where `containers/deque.hero:17-18` promises an abort. **Run the same shape on
  `cache`, `indexed` and `ordered`** (`cache.hero:109`, `indexed.hero:20`, `:75`, `ordered.hero:35` make the
  same promise), under `timeout -s KILL 20`, and say which keep it, **and whether any can** (the critic's
  finding 8): a shape record and the program's list share no identity, values being copies (spec § 3,
  lines 97-98), so what check could tell a list that is not the shape's own is Q3-C's open question.

## Question 4: a `u8` where a value becomes text

- **Build Q4-R**: a `u8` refused in an `f"..."` hole (`selfhost/check/interp.hero:26-31`) and as a `print`
  argument (`selfhost/check/builtins.hero:176-190`), a diagnostic class of its own with two `guess` fixes,
  the character `s.slice(from: i, to: i + 1)` where the hole indexes a `str`, the number `to_str(b)`.
  `bw/q4/bytes.hero` is the writer's measure (`[117]`, `117`, `117`, `[65]`, `[97]`, `[u]`).
- **The census** (§ The census each question may need): every hole and `print` argument the route refuses,
  each read and classed as **number meant** (a correct program the route refuses; its fix `to_str`) or
  **character meant** (a silent wrong program the route catches). Rebel's drivers first: this is Q4-R's
  price and its yield, and no other seat can count it.
- **Price Q4-R2 beside it** (the critic's finding 10; arm `bw/arms/q4-r2.diff`, +52): refuse only a hole or
  a `print` argument whose expression is itself an index into a `str` (`s[i]`, `word[0]`), so a `u8` bound
  by name prints its number with no `to_str`: the same census read for what Q4-R2 refuses, what it lets
  pass that Q4-R catches (`c = s[0]` then `f"{c}"`), and the correct programs neither touches.
- **The shape beside**: `c = 'a'` is an `i64` (spec § 2), so `f"{c}"` writes `97` under Q4-R too. What
  would a character literal defaulting to `u8` move (a census of the same roots)? Priced, not built.

## Question 5: an exponent the language prints and refuses

- **Build Q5-R**: `selfhost/number.hero:146-172` reads an exponent instead of refusing it, to the arm's
  repaired form (*"A float is digits, a point and digits, or digits with an exponent, or both"*, arm
  `bw/arms/q5-r.diff`, +41; its first wording gave `1e-06` as a float with no point, the critic's finding
  3), so `1e21`, with no point, is a float: say whether your build makes it one; the value's
  C (`%a`, `docs/design.md:810`) unchanged; what `heroes fmt` writes back for `1e-06`, `1E-6` and
  `1.0e-6` (one canonical spelling, or each kept as written; CLAUDE.md § 9's walk for a new surface form,
  `.claude/rules/diagnostics-and-goldens.md`); an `f32` context (`small: f32 = 1e-06`); a literal past
  `f64`'s range, `1e400` (refused, or `inf`, and which the reader is told); the goldens
  that hold `exponent_literal` (`tests/golden/check/exponent-literal.hero`,
  `check/fixedbugs-2391-*`, `full/fixedbugs-2390-*`, by `grep -rl exponent_literal tests/golden`).
- **A float's `_`** (the critic's finding 9): `tol = 0.000_000_001` and `1_000_000.5` are accepted today and
  print `1e-09` and `1000000.5` (`bw/q5/float-sep.hero`), though spec § 2 grants `_` to an `i64` alone;
  does `heroes fmt` keep a float's `_` as written, and does Q5-R's reader take `1_000e-3`? Arm
  `bw/arms/q5-u.diff`, +14, states it.
- **Price Q5-P** (`print` writes no exponent): `runtime/parts/f64.c` (341 lines, the ladder at 26-27)
  changed to a positional form that still reads back exactly; the `run` expectations that move (**8 of
  811** print an exponent today, `grep -l -E '(^|[^0-9A-Za-z_])[0-9](\.[0-9]+)?e[-+][0-9]{2,3}\b'
  tests/golden/run/*.expected | wc -l`, the writer's pattern, unread one by one); what `print` of the
  largest and the smallest positive `f64` writes, in bytes.

## What you are asked to say

Your definition's structure per question; each route built or priced with its files and lines; the
census counts with the command that made them; the route you would adopt per question, built and run on
the cases named here; and every premise of these briefs you found false.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/252-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
