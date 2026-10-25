# Panel 229, shared brief: the formatter's probe over `tests` must end

Written 2026-10-25 by the coordinator of M-rechecked-issues, on the lane
`lane-rechecked-issues`. **Repaired after the critic's first pass**
(`../229-reports/critic-briefs.md`, fifteen findings, every one taken; the
first version of this brief is that report's subject, kept in the git
history of this file). The frozen commit is now
**`01c5efd75`** (`git log -1 --format=%H`:
`01c5efd7580a81678b1bd90b9906921ebe4d2ea0`, `git status --short` clean but for this sitting's
own two folders), which holds one repair the critic's first finding led to (below).
Every number carries the command that produced it; a sentence that could not
be run says so.

## The question

**Defect 2139** (https://github.com/heroes-lang/heroes/issues/2139):
`heroes probe tests --family bracket` cannot end. `.claude/rules/verification.md`
§ *The formatter's probe, by hand, before a push that touches `selfhost/print/`*
(line 297) owes that run, `single` and `bracket` over `selfhost`, `tests` and
`examples` as six invocations side by side at `-O2`, before any push that
touches `selfhost/print/`; it has stood unfinished since 2026-10-11 for
M-inferred-cell's changes and since 2026-10-18 for M-readonly-lend's
(`ROADMAP.md` line 17: every other run those changes owed read exit 0,
journal 067 lines 206 to 220). Filed `adjacent` on 2026-10-18, it counts as
`blocking` since the second tag after it; `m-thesis-harness` stands over it by
the author's waiver, **decision 2200** (closed 2026-10-24, verdict *B*: *the
next tag cannot stand over them*), so **M-rechecked-issues cannot be tagged
while it is open**, and a second waiver is not a route.

**What this sitting rules**: how the run the rule owes becomes one that ends,
on this Mac, in a bounded wall time, without the probe losing the class of
finding it exists for. Panel 179 ruled the probe's shape (item 3, *every
family the seats ran*; item 7, *the full single-comment and bracket-break run
over the tree stays a run by hand, required before a push that touches
`selfhost/print/`*). **Whether a given route changes any item of 179 is a
question for the seats, not a premise** (the critic's finding 11): no item of
179 rules what a judge or the formatter costs, so a route that only makes them
cheaper may be a defect's repair and not this sitting's ruling.

## What the probe is, at the frozen commit

- `selfhost/cli/probe.hero` (the verb, `--family`, `--stride`), and
  `selfhost/probe/`: `seed.hero` 214 lines, `tokens.hero` 155 (`bracket`,
  `paren`), `lines.hero` 312 (`single`, `multi`, `ends`, `columns`, `texts`),
  `judge.hero` 263 (the driver), `reader.hero` 408 (the independent judge),
  `reduce.hero` 135 (`grep -c ''` on each).
- **The guard is `selfhost/print/guard.hero`'s `checked`, the one function
  `heroes fmt` calls** (`cli/syntax_cmds.hero:47`), and the probe calls it once
  per variant (`probe/judge.hero:72`): timing `heroes fmt` on a variant times
  the formatter and the first judge together.
- **`bracket`** (`tokens.hero:40`): inside a bracket, after every token that
  another token follows on its line, never beside a piece of an interpolated
  string (`:57`), three variants: the line broken there with a comment
  trailing the token (`T`), on a line of its own (`O`), or both (`B`). Each
  variant is the whole file with that one line replaced.
- **`--stride N`** (`judge.hero:116`): judges every N-th variant of each
  family across the run, the first included; every variant is generated and
  counted either way (`:143` to `:149`).
- **One thread per invocation**: a `sample` of a running probe shows one
  worker thread doing the work and the main thread waiting on it (the
  critic's finding 1(c), and the coordinator's `sample` of `heroes fmt`).

## Why the dear seeds are in the tree, and what in them is dear

The spec states **no depth floor**: `grep -n -i '2,000\|2000\|floor' spec/`
prints nothing. Panel 184's R6 ruled that it would state one once N was
measured on three platforms; panel 197 recorded N = 2,000 measured on this Mac
and Linux arm64, *not a spec sentence* (`docs/panel/197-briefs/00-shared.md`
lines 42-43). The cases stay because `tests/golden/` is a record, never
rewritten. Six seeds are 2,000 deep or long by name (`grep -i
'2000\|two-thousand'` on the counts): 259 (84,027 variants), 169-brackets
(18,036), 264 (6,195), 169-chains (6,045), 489 (57), 169-one-operand-runs
(48).

**On 259 (29 lines, 33,016 bytes, 32,068 tokens) the cost is a comment
inside a 2,000-long chain written as one statement, not the nesting**
(critic's finding 1, its builder `m/variants*.py` in its directory): `heroes
fmt` on one variant of line 18, `a: str = x[0][0]…`, read `user` 3.63, 3.78,
7.75, **31.28** and 1.29 s with the comment after the 1st, 10th, 200th,
1,000th and 1,990th `[`, against 0.07 s for 259 itself; variants of the
2,000-deep nest (lines 16 and 20) take 0.07 to 0.20 s each, the nest alone
under the probe 0.069 s a judged variant. Lines 18 and 22 hold 12,000
variants each; 264's dear line is `x = v.to_str().len()…` (15,008
characters) and 169-chains' `return v.inc().inc()…` (12,012), the same shape.
2139's numbers for a nest (0.84, 3.73, 19.11 s at depth 100, 200, 400) grow
by the power 2.15 then 2.36 per doubling, not 3.

## Measured today

The measuring compilers: the lane's `selfhost/` emitted with `./heroes build
selfhost/main.hero --emit-c` and compiled with `clang -O2 -w -I runtime <that
C> runtime/runtime.c`, the build the rule names: **before** the repair below
at `a126f7a60` (`sources 1b247d9a35f7e25b`), **after** it at `01c5efd75`
(`sources 48ac4ee426d58db0`).

**The repair the critic's finding led to, landed at `01c5efd75`**:
`print/aligned.hero`'s `written_pairs` walked every written token again for
each one's partner; a map built once gives the same bytes over **all 3,115
tracked `.hero` files** (each formatted by both compilers, `shasum` compared)
and: `heroes fmt` on 259's variant with a comment after the 1,000th `[` of
line 18, `user` **29.79 → 9.89 s**; 20 judged `bracket` variants of 259
(`--stride 4202`), `user` **126.37 → 44.83 s** (6 of the 20 fall on lines 18
and 22, the other 14 cost about 0.1 s each). **Where the 9.89 s goes now**, a
5 s `sample` of that `fmt` (worker thread, 3,244 samples, cumulative):
`bare.needs_group`/`part_needs` 1,014, a recursion one level per index of the
chain, each level computing `elements.extent`, `around.untaken` and, at its
base, `parens.render` of the whole subtree; `brackets.spread` 498;
`breaks.loose`/`bare.one_line` 435; `parens.render` 216; `breaks.plan` 208;
`aligned.aligned` 196. A reading of those functions, not a complexity
measurement.

**The variants, counted** (`heroes probe <root> --family bracket --stride
1000000`): `selfhost` 684 seeds, 1,546,662 variants; `tests` 1,824 seeds and
487 files the parser refuses on purpose, 1,058,277; `examples` 120 seeds,
123,345: **2,628 seeds, 2,728,284 variants**. The largest by variants: 259
84,027; 477 38,658; `suite_records.hero` 36,573; `walk.hero` 34,728;
`suite_surface.hero` 33,804; `suite_spec.hero` 32,586; `ir/flatten.hero`
25,368; `grammar_expr.hero` 24,264.

**The whole tree without 259, before the repair** (one `heroes probe <seed>
--family bracket --stride 100` per seed, sequential, `/usr/bin/time -p` on
each; the loop `real 436.40`, `user 418.37`, the machine still): 405.03 s of
`user` for 27,895 variants judged of 2,644,257, which times 100 is about **11.3
CPU hours** (an inference from an even sample). By root: **`tests` 8.25 CPU
hours**, `selfhost` 2.91, `examples` 0.09. The dearest per judged variant:
169-chains 1.08 s and 264 1.02 s (about 1.8 CPU hours each), `suite_records`
0.078 s, 169-brackets 0.095 s, `walk.hero` 0.044 s. Not re-run after the
repair; 259 is the one seed timed both ways.

**So the run has two problems, not one.** CPU: 259 alone was about 147 CPU
hours by six samples, about 52 after the repair by the same six (an
inference). Wall: **`tests --family bracket` is one process on one thread**,
so even without 259 it is about 8.25 hours of wall clock as the rule runs it,
whatever the formatter costs; journal 067 reads the same, `selfhost`'s
`bracket` 14,034 s and `tests`' `single` 16,410 s, each one process.

## The routes, as listed, and the ones the first pass found

- **(A) A bound per seed**: the probe judges a seed whose variants pass a bound
  at a stride it chooses, and says so on stderr.
- **(B) A bounded family**: `bracket` breaks a line at fewer places, one per
  class of equivalent places. The critic measured one such rule, *one per run
  of tokens of the same kind at the same depth*: it leaves 259 at 84,027
  (each `[` of the nest at a new depth, `[` `0` `]` alternating on the
  chains) and collapses 264 to 198 and 169-chains to 48.
- **(C) The rule names the stride** for such files, the probe unchanged.
- **(D) The judges made cheaper**: measured not to be where 259's time is;
  `probe/reader.hero` appears in no top frame of either profile.
- **(E) The formatter made near-linear on a comment inside a long chain**, the
  probe and its families unchanged: its first step is `01c5efd75`; the rest is
  the recursion and the renders above, and the same probe run is owed for it
  as for any change under `selfhost/print/`.
- **(F) The run spread over the cores**: one seed's variants or one root's
  seeds split across processes (a stride with an offset, or the run per seed
  under `xargs -P`, as journal 061's run of 2026-09-28 went, 2,273 jobs seven
  at a time in 57 minutes), which no route above answers.

Ask what would have to be true for a route nobody listed to exist, and which
combination the run needs: CPU and wall are separate bounds.

## What the probe has found, the class it must not lose

- **Defect 503** (https://github.com/heroes-lang/heroes/issues/1849): the
  `bracket` family over `tests/golden/ir/fixedbugs-394-a-literal-s-arguments-grow-in-place.hero`,
  51 of 276 variants refused by the guard.
- **Defect 504** (…/1850): a parser defect found while reducing 503, not by a
  variant.
- **Defect 506** (…/1852): confirmed by `heroes probe`, 20 of 152 parsing
  variants refused; not in verification.md line 471's tally.
- **Panel 179's 17 refusals at `83ac68c1`** (its lines 74 to 79): found by the
  seats' Python generators before `heroes probe` existed, every one tagged
  T/O/B, the `bracket` family's shape: 12 in the `comments101` fixtures, 5 of
  225 bracket breaks in `walk.hero`.

Note: defects 001 to 677 keep their own numbers, so `gh issue view 503` reads
another issue; search `"Defect 503"`.

## How every seat works

- **Your own copy, never another seat's**: `git -C
  /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues archive
  01c5efd75 | tar -x -C <your directory>`, your directory
  `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues-scratch/229-<seat>/`,
  inside the repository's root (nothing is written outside it). Build there
  from the seed, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
  then `./heroes build selfhost/main.hero -o heroes-next` and `mv heroes-next
  heroes` as its own command; for timing, the `-O2` route above. Never the
  lane's `./heroes`. The critic's builders and outputs are in
  `…/rechecked-issues-scratch/229-critic/m/` (read, do not write).
- **Bounds, both named** (`.claude/rules/verification.md` § A run that may not
  end): **every run under `timeout 300`** (a longer one says why in your
  report and still has a bound), **every output through `head -c 50000000`
  into a file or to `/dev/null`**; one variant formats to 16,053,019 bytes.
  After a timeout, `pgrep -f` for what it left and stop it.
- **Timing**: `/usr/bin/time -p`, `real` beside `user`; other seats run on
  this Mac, so a run whose `real` is far above `user` is discarded and said.
- **No paid run** (`claude -p`, an API call, a cloud run) and **no credential
  printed** (`gh auth token` is refused; `gh auth status` and read-only `gh
  issue view` / `gh search issues` are fine).
- **Write your report as you go**, `docs/panel/229-reports/<seat>.md` in the
  lane, never only at the end; every number with its command, every negative
  sentence run or written as a question.
- **End with predictions** a later run can falsify, each with its command.
