# Panel 229, the historian's report

Seat: historian (advisory, no veto). Written 2026-10-25, as the reading went,
on the lane `lane-rechecked-issues` at the frozen commit `01c5efd75`. Every
precedent below names the page it rests on and the day it was read
(2026-10-25 for every one), and is marked `verified` only where the page
itself was read today and says what the line says. What the page does not say
is not written here. No paid run, no credential, no compiler built or run:
this seat read pages and two files of the probe (section 4), and its
predictions at the end name the commands that would falsify them.

*Heroes of code* was not used as a source for any line here.

## 1. A formatter's cost on long or deep input

### 1.1 clang-format: a hard cap on the states its line search explores

- **What the source says**, `clang/lib/Format/UnwrappedLineFormatter.cpp` on
  `main`, read 2026-10-25
  (https://raw.githubusercontent.com/llvm/llvm-project/main/clang/lib/Format/UnwrappedLineFormatter.cpp):
  the `OptimizingLineFormatter` is documented as *"a variant of Dijkstra's
  algorithm"* over line-break states; inside `analyzeSolutionSpace` it reads
  `// Quit if we still haven't found a solution by now.` `if (Count >
  25'000'000) return 0;`, and a softer threshold `if (Count > 50'000)
  Node->State.IgnoreStackForComparison = true;` under the comment *"Cut off the
  analysis of certain solutions if the analysis gets too complex"*. When the
  queue empties with no solution: *"We were unable to find a solution, do
  nothing. FIXME: Add diagnostic?"*, which leaves the line as written.
  `verified`.
- **What happened**, the review that added the cap, D128574, *"[clang-format]
  Quit analyzing solution space for large state count"*, read 2026-10-25
  (https://reviews.llvm.org/D128574): *"When formatting
  compiler-rt/test/builtins/Unit/udivmodti4_test.c, which contains a nested
  braced init list with almost 2.5 million tokens, clang-format will run for
  hours and eventually be killed by the OS. Halving the size of the list would
  enable clang-format to format the file in a couple of minutes with Count
  being almost 25 million."* Created 2022-06-24, committed 2022-06-26.
  `verified`. The issue it fixed, llvm/llvm-project#56043, *"clang-format
  hangs on large nested braced initialization lists"*, opened 2022-06-15,
  closed by commit `664ce34` on 2022-06-26, read 2026-10-25
  (https://github.com/llvm/llvm-project/issues/56043). `verified`.
- **What it paid**: on such a line the formatter does nothing, silently (the
  `FIXME: Add diagnostic?` is the source's own words). The review gives no
  reason for 25 million beyond the halved input's count.
- **Routes**: a bound inside the formatter, on a count of work rather than on
  input size or on time. A production formatter met a superlinear line search
  on one dear test file of a compiler's own tree and answered with a cap, not
  a new algorithm. It bears on **(E)** and on route **(G)**, which nobody
  listed (section 5).

### 1.2 dart_style: an attempt cap, then the best solution so far

- **The author's account**, Bob Nystrom, *"The Hardest Program I've Ever
  Written"*, 2015-09-08, read 2026-10-25
  (https://journal.stuffwithstuff.com/2015/09/08/the-hardest-program-ive-ever-written/):
  *"the set of possible solutions is exponential in the number of rules"*; one
  example line has *"about 549 billion ways we could line break it"*; *"If the
  line splitter tries, like, 5,000 solutions and still hasn't found a winner
  yet, it just picks the best it found so far and bails. In practice, I only
  see it hit this case on generated code."*; *"There are still a few cases
  where the formatter takes a long time."* On comments: *"Comments were the
  one of the biggest challenges"*, the formatter having to be *"resilient in
  the face of comments and newlines appearing in all sorts of weird places.
  There's no single clean solution for this, just lots of edge cases and
  special handling."* `verified`.
- **The source then**, `lib/src/line_splitting/line_splitter.dart` at tag
  `1.3.7`, read 2026-10-25
  (https://dart.googlesource.com/dart_style/+/refs/tags/1.3.7/lib/src/line_splitting/line_splitter.dart):
  *"To ensure the solver doesn't go totally pathological on giant code, we cap
  it at a fixed number of attempts. If the optimal solution isn't found after
  this many tries, it just uses the best it found so far."* `const _maxAttempts
  = 5000;`, and `if (attempts++ > _maxAttempts) break;`. `verified`.
- **The source now**, the rewritten ("tall") solver,
  `lib/src/back_end/solver.dart` on `main`, read 2026-10-25
  (https://raw.githubusercontent.com/dart-lang/dart_style/main/lib/src/back_end/solver.dart):
  the same doc comment, `const _maxAttempts = 10000;`, the loop `while
  (_queue.isNotEmpty && attempts < _maxAttempts)`, and *"If we didn't find a
  solution without overflow, pick the least bad one."* `verified`.
- **What it paid**: on capped input the output is not the optimum; the source
  says so. A rewrite of the whole solver kept the cap (and doubled it) rather
  than removing it, which is the strongest evidence in this section that a
  cap and a better algorithm are complements, not alternatives.
- **Routes**: as 1.1; a bound inside the formatter, counted, not timed.

### 1.3 rustfmt: an exponential search, a memo that fixed it, a revert

- **The slowness**, rust-lang/rustfmt#5128, *"rustfmt is prohibitively slow
  for sufficiently nested structs"*, opened 2021-12-08, read 2026-10-25
  (https://github.com/rust-lang/rustfmt/issues/5128): *"Spent 0.020 secs in
  the parsing phase, and 444.417 secs in the formatting phase"* on a snippet of
  about 220 lines; with `max_width = 106` it read *"0.055 secs in the
  formatting phase"*, and at 105 minutes again. `verified`.
  rust-lang/rustfmt#6079, *"does not finish when trying to format a program
  with a ridiculous number of nested `match`"*, opened 2024-02-16: *"will not
  finish formatting even after 15 minutes"*, starting at about 16 nested
  `match`; **open**, labelled `P-low`, no maintainer reply, read 2026-10-25
  (https://github.com/rust-lang/rustfmt/issues/6079). `verified`.
- **The diagnosis and the repair**, PR #5139, *"Fix exponential execution time
  by memoizing format_expr"*, opened 2021-12-15, merged 2022-04-17, read
  2026-10-25 (https://github.com/rust-lang/rustfmt/pull/5139): *"when
  `max_width` is tight, rustfmt should check all possible formattings, which is
  exponential relative to input size. By memoization of `format_expr`
  function, worst case execution time becomes `O(n*max_width^3)`."*; the same
  author, after the revert: *"the result is not a function of just `shape` and
  `expr`"*. `verified`.
- **What happened to it**, PR #5403, *Revert "Memoize format_expr"*, opened
  and merged 2022-06-22, read 2026-10-25
  (https://github.com/rust-lang/rustfmt/pull/5403): *"Memoizing expressions
  lead to cases where rustfmt's stability guarantees were violated."*, fixing
  #5399, *"Recent nightly rustfmt broke compatibility with stable rustfmt"*.
  `verified`. The changelog, read 2026-10-25
  (https://raw.githubusercontent.com/rust-lang/rustfmt/main/CHANGELOG.md):
  1.5.0 (2022-06-14) *"Improved performance when formatting large and deeply
  nested expression trees, often found in generated code"* citing #5128,
  #4867, #4476, #5139; 1.5.1 (2022-06-24) opens *"A bug was introduced in
  v1.5.0/nightly-2022-06-15 which modified formatting ... since that bug has
  now been reverted."*; and 1.2.0 (2019-03-27) *"Fix bad performance on deeply
  nested binary expressions #3467"*. `verified`. **That the bug 1.5.1 names is
  the memo #5403 reverted is my inference from the dates and #5399's title; the
  changelog does not name the change: `unverified`.**
- **What rustfmt does instead of bounding**: its error kinds, read 2026-10-25
  (https://doc.rust-lang.org/stable/nightly-rustc/src/rustfmt_nightly/lib.rs.html):
  `LostComment`, *"If we had formatted the given node, then we would have lost
  a comment."*, message *"not formatted because a comment would be lost"*; and
  `LineOverflow`, *"line formatted, but exceeded maximum width"*. `verified`.
  Tracking issue #6687, *"[Tracking] Formatting failures with all-or-nothing
  fallback behavior"*, opened 2025-10-09, open, read 2026-10-25
  (https://github.com/rust-lang/rustfmt/issues/6687): rustfmt *"abandons
  formatting the entire statement instead of attempting partial formatting"*,
  affecting *"method chains (in src/chains.rs)"*, with triggers including
  *"problematic block comments"*. `verified`.
- **Routes**: a warning about **(E)**. A memo keyed on too little made the
  formatter fast and changed its output, and was reverted about ten weeks
  after it merged; its own author names the cause as a result that *"is not a
  function of just `shape` and `expr`"*. Heroes's repair at `01c5efd75` was
  checked byte for byte over 3,115 files (shared brief, line 97), which is the
  check that would have caught rustfmt's; any further step of (E) owes the
  same, and the probe run after it. rustfmt's `LostComment` is a precedent for
  the probe's guard: a formatter that refuses to emit rather than lose a
  comment.

### 1.4 Black: the same shape as 259's line 18, met and repaired this month

- **The report**, psf/black#5270, *"Quadratic formatting time for chains of
  subscripts (7s for one 160-subscript line; attribute/call chains stay
  linear)"*, opened 2026-07-29, read 2026-10-25
  (https://github.com/psf/black/issues/5270): *"A single line consisting of a
  chain of subscripts takes quadratic time to format."* For `a[0][1]…`:
  n=40 0.033 s, n=80 1.142 s, n=160 7.098 s; *"160 subscripts on one line
  takes ~7 s; 300 takes ~23 s."*; attribute chain n=160 0.024 s, call chain
  n=160 0.068 s; with `--line-length 100000`, n=160 0.020 s. A maintainer,
  JelleZijlstra, 2026-07-30: *"I'd prefer maintaining existing formatting;
  otherwise we would have to put the change in the preview style."* Closed
  2026-10-06 by #5475. `verified`.
- **The repair**, PR #5475, *"Avoid quadratic runtime in
  right_hand_split_with_omits on long lines (#5270)"*, opened 2026-10-03,
  merged 2026-10-06, read 2026-10-25 (https://github.com/psf/black/pull/5475):
  subscript chains *"have no intermediate delimiters"* and fall back to
  `right_hand_split_with_omits`; *"Black was performing up to 20 full
  `right_hand_split` calls per recursion level"*; the fix precomputes prefix
  lengths once, skips an omit whose bracket opens beyond `line_length`, and
  reuses a result instead of re-running the split; *"Output formatting is 100%
  invariant across all cases"*, by diff-shades on both styles. n=100 about
  2.8 s to 0.35 s, n=160 about 9.5 s to 0.35 s, n=300 over 30 s to about
  0.95 s. Its test asserts the number of `right_hand_split` calls stays under
  2n rather than timing the run, *"to avoid flaky CI timings"*. `verified`.
- **The changelog**, read 2026-10-25
  (https://black.readthedocs.io/en/stable/change_log.html): 26.10.1 *"Avoid
  quadratic runtime when splitting very long lines with many trailing bracket
  pairs (such as consecutive subscripts)"*; 26.10.0 lists, among others,
  *"Improve performance on deeply nested bracketed expressions"* (#5171,
  #5242), *"Improve performance on lists and subscripts holding one long
  expression without operators"* (#5239), *"Improve performance on deeply
  chained operations (e.g. a long `a ** b ** c ** ...` chain)"*. `verified`.
- **The probe's own class, in Black**, PR #5498, *"Avoid splitting trailers
  when earlier bracket split exceeds length due to comments (#3681)"*, merged
  2026-10-10, read 2026-10-25 (https://github.com/psf/black/pull/5498): a
  head line over length *"solely because of a trailing comment"* caused
  *"unrelated trailing indexing/subscripts (such as `[0]`) on later lines to be
  exploded across multiple lines"*; it fixes #3681, *"Addition of type ignore
  causes undesirable splitting on another line."* `verified`. A comment
  inside a bracket chain changing where lines break is the `bracket` family's
  shape; Black's was reported by a user, not found by a generator.
- **What it paid**: nothing in output, by its own diff-shades run; the cost was
  the report standing open about ten weeks (2026-07-29 to 2026-10-06).
- **Routes**: the strongest support found for **(E)**. A formatter whose
  per-level work on a delimiter-free chain re-ran a whole split per level was
  made linear in the calls it makes, with the output held identical over a
  corpus and a test that counts calls, not seconds. 259's profile (shared
  brief, lines 104 to 110: a recursion one level per index, each level
  rendering the whole subtree) reads as the same shape; that it is the same
  cause is a question for the compiler seats, not a finding of mine.

### 1.5 gofmt: a printer that measured by printing, memoised soundly, in 2011

- **The repair**, code review 4274075, *"go/printer, gofmt: avoid exponential
  layout algorithm"*, by gri, created 2011-03-21, read 2026-10-25
  (https://codereview.appspot.com/4274075): *"Use memoization to avoid
  repeated recomputation of nested node sizes. Speeds up testdata/slow.input by
  several orders of magnitude. Fixes issue 1628."* `verified`.
- **The source today**, `src/go/printer/nodes.go` on `master`, read 2026-10-25
  (https://raw.githubusercontent.com/golang/go/master/src/go/printer/nodes.go):
  `nodeSize` *"determines the size of n in chars after formatting. The result
  is <= maxSize if the node fits on one line with at most maxSize chars ...
  Otherwise, the result is > maxSize."*; inside it, *"nodeSize invokes the
  printer, which may invoke nodeSize recursively. For deep composite literal
  nests, this can lead to an exponential algorithm. Remember previous results
  to prune the recursion (was issue 1628)."*; the cache `p.nodeSizes[n]`,
  seeded with `maxSize + 1 // assume n doesn't fit` before the recursion; and
  *"nodeSize computation must be independent of particular style so that we
  always get the same decision; print in RawFormat"*. `bodySize` stops adding
  once `bodySize > maxSize`, *"no need to continue"*. `verified`.
- **What it paid**: a map per printer; the page states no cost.
- **Routes**: **(E)**, and the sound half of the rustfmt story. The memo is
  keyed on the node alone and made valid by computing the size in one fixed
  style, so the key is the whole of what the answer depends on, which is the
  condition rustfmt's memo broke. And the question asked is only *does it fit
  in maxSize*, never the full rendering. Whether 259's recursion (shared
  brief, lines 104 to 110) has the same structure is the compiler seats' to
  say; the page shows only that a printer which measures a subtree by printing
  it met the same exponential and how it was repaired.

### 1.6 Wadler and Oppen: the fit test looks no further than one line

- **The paper**, Philip Wadler, *"A prettier printer"*, read 2026-10-25 in
  the author's PDF (https://homepages.inf.ed.ac.uk/wadler/papers/prettier/prettier.pdf),
  page 10: `fits w x | w < 0 = False`, `fits w (i 'Line' x) = True`, so a fit
  test stops at the first line break or when the width runs out; *"It is
  essential for efficiency that the inner computation of best is performed
  lazily."* Page 11: *"It is reasonable to expect that pretty printing a
  document should be achievable in time O(s), where s is the size of the
  document"*. Page 17: *"say that it is bounded if it can make this choice
  after looking at no more than the next w characters, where w is the line
  width. Hughes notes that there is no algorithm to choose line breaks for his
  combinators that is optimal and bounded, while the layout algorithm
  presented here has both properties."*, and *"Oppen (1980) describes a
  pretty-printer with similar capabilities ... it is optimal and bounded: line
  breaks are chosen to avoid overflow whenever possible, and lookahead is
  limited to the width of one line."* `verified`. The paper's year and venue
  are not on the pages read: `unverified`.
- **Oppen's own abstract**, *"Prettyprinting"*, ACM TOPLAS, 1980: the ACM page
  refused the fetch (HTTP 403), so its complexity statement is `unverified`
  from the primary page; the line above is Wadler's account of it.
- **Routes**: **(E)** at its root. Since at least Oppen, the decision *does
  this group fit on the line* is answerable after reading at most the line's
  width. 259's base case renders the whole subtree (`parens.render`, shared
  brief line 107) to answer it; whether the Heroes formatter can ask the
  bounded question instead is the compiler seats' to measure.

### 1.7 Prettier: exponential where alternatives nest, repaired case by case

- **Its own documentation**, `commands.md` on `main`, read 2026-10-25
  (https://raw.githubusercontent.com/prettier/prettier/main/commands.md):
  `conditionalGroup` *"should be used as last resort as it triggers an
  exponential complexity when nested."* `verified`.
- **What happened**, prettier/prettier#4672, *"`prettier.format()` slow for
  deeply nested function calls in input string"*, opened 2018-06-12, about 25
  nested calls, output *"after 159 seconds"*, Prettier 1.13.5, closed
  2020-05-22 with a maintainer's *"#6229 fixed it most probably"*, read
  2026-10-25 (https://github.com/prettier/prettier/issues/4672). `verified`.
  PR #6229, *"Printing arguments: short circuiting some cases"*, merged
  2019-06-17, closing #4784 (*"Severe performance degradation on small code.
  5+ minutes to format"*); the sample then ran in 0.58 s; its author called
  the code *"one of the most sensitive parts"* and a reviewer did not
  *"understand this piece of code well enough to say whether this is safe"*
  but supported the risk, read 2026-10-25
  (https://github.com/prettier/prettier/pull/6229). `verified`. #4776,
  *"Performance on big files"*, opened 2018-06-30 by vjeux, open: *"As far as
  I know, outside of fixing exponential complexity, we haven't spent any time
  trying to optimize it."*, read 2026-10-25
  (https://github.com/prettier/prettier/issues/4776). `verified`.
- **Routes**: **(E)**, with its price named by the project itself: each
  superlinear case closed by a local short circuit in a part of the printer its
  maintainers call sensitive. No bound on input, no cap.

### 1.8 Biome: the one documented input-size cutoff found

- **Its configuration reference**, read 2026-10-25
  (https://biomejs.dev/reference/configuration/): `files.maxSize`, default
  1048576 bytes: *"Biome will skip files above this limit for performance
  reasons and emit a warning diagnostic."* `verified`.
- **Routes**: a bound on input, said aloud, which is **(A)**'s shape applied
  to the formatter rather than the probe. 259 is 33,016 bytes, far under such
  a cutoff; a bound in bytes would not reach it, so the bound (A) needs is on
  variants or on work, not on bytes.

### 1.9 The judges' precedents: Black and ocamlformat check their own output

- **Black**, *"The basics"*, read 2026-10-25
  (https://black.readthedocs.io/en/stable/usage_and_configuration/the_basics.html):
  *"By default, Black performs an AST safety check after formatting your
  code. The --fast flag turns off this check"*; its failure reads *"INTERNAL
  ERROR: Black produced code that is not equivalent to the source."*, exit
  123. `verified`.
- **ocamlformat**, `lib/Conf.ml` on `main`, read 2026-10-25
  (https://raw.githubusercontent.com/ocaml-ppx/ocamlformat/main/lib/Conf.ml):
  `max-iters`, default 10, *"Fail if output of formatting does not stabilize
  within N iterations."*; `comment-check`, default true, *"Control whether to
  check comments and documentation comments. Unsafe to turn off."* `verified`.
- **Routes**: none directly. A formatter judging its own output on every run,
  as `guard.hero`'s `checked` does, is ordinary practice; Black lets a user
  turn the judge off for speed, ocamlformat calls turning its comment check off
  unsafe. Route **(D)** was measured not to be where the time is, so neither
  precedent argues for it.

**What section 1 says as a whole** (my reading, not a page's): every
production formatter read here met superlinear cost on long or deep input
(clang-format, dart_style, rustfmt, Black, gofmt, Prettier); four repaired the
algorithm (gofmt, Black, Prettier, rustfmt once, reverted); two cap the work by
a count and keep the cap after rewrites (clang-format, dart_style); one bounds
input by bytes (Biome). **No formatter read here caps by wall time**, and the
two caps are counts, so their output stays the same on every machine.

## 2. Bounding or partitioning an exhaustive variant generator

### 2.1 AFL: exhaustive deterministic stages, an effector map, a split by checksum

- **The effector map**, `docs/technical_details.txt`, read 2026-10-25
  (https://raw.githubusercontent.com/google/AFL/master/docs/technical_details.txt):
  effector maps *"are local every queue entry and remain in force only during
  deterministic stages"*; they *"can reduce the number of execs by 10-40% or so
  without an appreciable drop in coverage. In extreme cases, such as normally
  block-aligned tar archives, the gains can be as high as 90%."* *"File size
  has a dramatic impact on fuzzing performance"*. `verified`. The source,
  `afl-fuzz.c`, read 2026-10-25
  (https://raw.githubusercontent.com/google/AFL/master/afl-fuzz.c): in the
  walking-byte stage *"we identify bytes that seem to have no effect on the
  current execution path even when fully flipped and we skip them during more
  expensive deterministic stages, such as arithmetics or known ints."*; *"If
  the effector map is more than EFF_MAX_PERC dense, just flag the whole thing
  as worth fuzzing, since we wouldn't be saving much time anyway."*
  `verified`. `config.h`, read 2026-10-25
  (https://raw.githubusercontent.com/google/AFL/master/config.h):
  `EFF_MIN_LEN 128` (*"Minimum input file length at which the effector logic
  kicks in"*), `EFF_MAX_PERC 90`, `EXEC_TIMEOUT 1000` (*"This is the upper
  bound, also used for detecting hangs"*), `MAX_FILE (1 * 1024 * 1024)`.
  `verified`.
- **Splitting the exhaustive stage over processes**,
  `docs/parallel_fuzzing.txt`, read 2026-10-25
  (https://raw.githubusercontent.com/google/AFL/master/docs/parallel_fuzzing.txt):
  *"the master instance will still perform deterministic checks; while the
  secondary instances will proceed straight to random tweaks"*; *"there is an
  experimental support for parallelizing the deterministic checks"*, `-M
  masterA:1/3`, *"the first value after ':' is the sequential ID of a
  particular master instance (starting at 1), and the second value is the
  total number of fuzzers to distribute the deterministic fuzzing across."*
  `verified`. The source, `fuzz_one` in `afl-fuzz.c`: *"Skip deterministic
  fuzzing if exec path checksum puts this out of scope for this master
  instance."* `if (master_max && (queue_cur->exec_cksum % master_max) !=
  master_id - 1) goto havoc_stage;` `verified`.
- **What happened to the exhaustive stage**, AFL++'s changelog, read
  2026-10-25 (https://aflplus.plus/docs/changelog/): 3.00c *"deterministic
  fuzzing is now disabled by default and can be enabled with -D. It is still
  enabled by default for -M."*; 3.14c *"removed implied -D determinstic from
  -M main"*; 4.10c *"better deterministic fuzzing is now available, benchmarks
  have shown to improve fuzzing. Enable with -D."*; 4.20c *"the new
  deterministic fuzzing feature is now activated by default, deactivate with
  -z. Parameters -d and -D are ignored."* `verified`. AFL++'s *Important
  changes* page, read 2026-10-25 (https://aflplus.plus/docs/important_changes),
  states the 3.00 change and **gives no reason for it**. `verified` (the
  absence is the page's, not a search's).
- **Routes**: three of them. **(F)**: AFL split one exhaustive stage across
  processes by a key modulo the count (`exec_cksum % master_max`), the same
  shape as a stride with an offset; AFL's own doc calls it experimental.
  **(B)**: the effector map is a class of equivalent places decided by observed
  behaviour (a flip that leaves the execution path unchanged), not by syntax;
  it saved 10 to 40%, never the orders of magnitude 259 needs. And AFL++'s
  history says an exhaustive stage that is too dear gets turned off by default
  and comes back only when made cheaper, which is **(E)**'s order of events,
  applied to the generator.

### 2.2 libFuzzer: a slow input is a finding, and parallel means processes

- **Its documentation**, read 2026-10-25 (https://llvm.org/docs/LibFuzzer.html):
  `-timeout`, *"Timeout in seconds, default 1200. If an input takes longer than
  this timeout, the process is treated as a failure case."*, exit code 70, the
  input saved as a `timeout-<sha1>` artifact; `-max_len`, *"If 0 (the
  default), libFuzzer tries to guess a good value based on the corpus"*, with
  the sample output *"libFuzzer will not generate inputs larger than 4096
  bytes"*; *"Each libFuzzer process is single-threaded, unless the library
  under test starts its own threads."*, *"it is possible to run multiple
  libFuzzer processes in parallel with a shared corpus directory"*; `-jobs`,
  `-workers` (*"min(jobs, NumberOfCpuCores()/2)"*), `-fork=N`. `verified`.
  Its flags source, `FuzzerFlags.def` on `main`, read 2026-10-25
  (https://raw.githubusercontent.com/llvm/llvm-project/main/compiler-rt/lib/fuzzer/FuzzerFlags.def):
  `report_slow_units`, default 10, *"Report slowest units if they run for more
  than this number of seconds."* `verified`.
- **Routes**: **(F)** (one process per core is the normal shape of a fuzzing
  campaign), and route **(J)** nobody listed (section 5): a variant whose
  formatting passes a bound is itself reported, as libFuzzer reports a slow
  unit, rather than its cost being absorbed by the run.

### 2.3 Mutant sampling and selective mutation: what a sample keeps

- **Offutt, Lee, Rothermel, Untch, Zapf**, *"An experimental determination of
  sufficient mutant operators"*, ACM TOSEM 5(2):99-118, 1996, abstract read
  2026-10-25 through OpenAlex's record (reconstructed from its inverted index
  of the abstract) (https://api.openalex.org/works/doi:10.1145/227607.227610):
  *"The principal expense of mutation testing is that many variants of the
  test program, called mutants, must be repeatedly executed"*; selective
  mutation reduces the mutants *"by an order of magnitude"*; *"in experimental
  trials selective mutation provides almost the same coverage as nonselective
  mutation with a four-fold or more reduction in the number of mutants."*
  `verified` (abstract only; the paper's body was not read).
- **Zhang, Hou, Hu, Xie, Mei**, *"Is operator-based mutant selection superior
  to random mutant selection?"*, ICSE 2010, pp. 435-444, record read
  2026-10-25 (https://api.openalex.org/works/doi:10.1145/1806799.1806863):
  *"Our empirical results show that operator-based mutant selection is not
  superior to random mutant selection."* `verified` (abstract only).
- **Gopinath, Alipour, Ahmed, Jensen, Groce**, *"How hard does mutation
  analysis have to be, anyway?"*, ISSRE 2015, pp. 216-227, abstract read
  2026-10-25 through OpenAlex (reconstructed from its inverted index)
  (https://api.openalex.org/works/doi:10.1109/ISSRE.2015.7381815): *"these
  strategies at best reduce the number of mutants required to a fraction of
  the overall mutants, which still grows with the program size"*; *"a small
  constant sample of mutants yields statistically similar results to running a
  full mutation analysis, regardless of the size of the program or similarity
  between mutants."* `verified` (abstract only). The concrete sample size
  and error bound in its body: `unverified` (the PDF at
  https://www.cefns.nau.edu/~adg326/issre15.pdf and a second copy both
  refused the fetch on their TLS certificates).
- **Wong and Mathur**, *"Reducing the cost of mutation testing: An empirical
  study"*, Journal of Systems and Software 31(3):185-196, 1995: the
  bibliographic record is `verified` (OpenAlex,
  https://api.openalex.org/works/doi:10.1016/0164-1212(94)00098-0, read
  2026-10-25); **its findings are `unverified`**: OpenAlex holds no abstract,
  ScienceDirect refused (HTTP 403), and two mirrors of a survey quoting it
  returned 404 or a certificate error. No number of theirs is used here.
- **What these measure, and what they do not** (my reading): every one judges
  a sample by a **score**, how adequate a test suite looks, not by whether
  each individual fault is still found. The probe's findings are individual
  defects of known density: 51 of 276 variants (defect 503), 20 of 152
  (defect 506), 5 of 225 (`walk.hero`, panel 179), all in the shared brief.
  For a defect refusing m of a seed's variants, a uniform 1-in-k sample misses
  it with probability about (1 - 1/k)^m: for 5 of 225, about 95% missed at
  k = 100 and about 24% at k = 4. That is arithmetic on the brief's numbers,
  not a measurement, and a stride is not a uniform sample (section 4).
- **Routes**: supports **(A)**, **(B)** and **(C)** for a question of score,
  and gives them no support for a question of *was this defect found*, which
  is the probe's. Gopinath's *"still grows with the program size"* is the
  warning for (A) and (C) as fractions: a 1-in-k stride over a growing tree
  grows with it.

### 2.4 A comment at every slot, sampled by a stable hash: the closest peer found

- **bynk**, issue #1834, *"fmt: inject a comment at every slot of every
  fixture"*, opened 2026-10-09, read 2026-10-25
  (https://github.com/accuser/bynk/issues/1834): each insertion point gets a
  line-end comment or a comment on its own line; the formatter must keep it
  exactly once with stable output that re-parses, or refuse with its named
  diagnostic; *"The test runs over the whole positive corpus within the normal
  `cargo test -p bynk-fmt` budget."* and *"If it can't, sample
  deterministically by seed and say so in the test's doc comment."*
  `verified`. PR #1861, merged 2026-10-09, read 2026-10-25
  (https://github.com/accuser/bynk/pull/1861): *"~7,000 refusals across ~21k
  placements"*; *"by default about one in four placements is checked, chosen
  by a stable FNV hash of (file, line, kind)"*; `BYNK_FMT_EXHAUSTIVE=1`
  checks all, which *"passes today"*; default run *"about 9s"*; *"checking
  everything takes well over half a minute in debug"*; three defects encoded as
  shape rules. `verified`.
- **gleam-parser**, PR #15, *"test(format): comment law at the hard positions
  inside expressions"*, merged 2026-10-16, read 2026-10-25
  (https://github.com/moonrockz/gleam-parser/pull/15): *"a comment before every
  `)`, `]`, `|>`, `->`, `|`, `as` and `..` of a generated module"*, the law
  *"is in the formatted output exactly once, and the output parses and is
  idempotent"*, which *"passes 12000 cases at two extra seeds; 150 in the
  suite."* `verified`.
- **Their standing**: both are a month old; neither page shows the project's
  size or users. They are peers met at the same time, not a precedent with a
  history, and are weighed as such.
- **Routes**: **(A)** and **(C)** in one form nobody listed, route **(H)**
  (section 5): the default run samples by a **stable hash of the place**, not
  by a stride, and the exhaustive run stays one switch away and is still
  owed. The bynk issue's own fallback is (A)'s *"say so"*.

## 3. Property-based testing and reduction

- **QuickCheck**, `src/Test/QuickCheck/Test.hs` on `master`, read 2026-10-25
  (https://raw.githubusercontent.com/nick8325/quickcheck/master/src/Test/QuickCheck/Test.hs):
  `stdArgs` sets `maxSuccess = 100`, `maxDiscardRatio = 10`, `maxSize = 100`,
  `maxShrinks = maxBound`; `computeSize`, *"e.g. with maxSuccess = 250,
  maxSize = 100, goes like this: 0, 1, 2, ..., 99, 0, 1, 2, ..., 99, 0, 2, 4,
  ..., 98."* The Hackage page, read 2026-10-25
  (https://hackage-content.haskell.org/package/QuickCheck-2.19.0.0/docs/Test-QuickCheck.html):
  `maxSize` *"Size to use for the biggest test cases"*. `verified`.
- **Hypothesis**, API reference, read 2026-10-25
  (https://hypothesis.readthedocs.io/en/latest/reference/api.html):
  `max_examples` *"The default max examples is 100."*, *"Once this many
  satisfying test cases have been considered without finding any failing test
  case, Hypothesis will stop looking."*; `deadline`, *"The maximum allowed
  duration of an individual test case, in milliseconds."*, *"The default
  deadline is 200 milliseconds. If running on CI, the default is None
  instead."*, *"We treat the deadline as a soft limit in some cases, where that
  would avoid flakiness due to timing variability."*; `DeadlineExceeded`,
  *"Raised when an input takes too long to run, relative to the
  settings.deadline setting."*; `derandomize`, *"If True, seed Hypothesis'
  random number generator using a hash of the test function"*, default
  `True` on CI. `verified`.
- **Hypothesis's reducer**, MacIver and Donaldson, *"Test-Case Reduction via
  Test-Case Generation: Insights from the Hypothesis Reducer"*, ECOOP 2020,
  LIPIcs 166, read 2026-10-25
  (https://drops.dagstuhl.de/entities/document/10.4230/LIPIcs.ECOOP.2020.13):
  reduction applied *"internally, to the sequence of random choices made
  during generation"*, so that *"any reduced test case is one that could in
  principle have been generated"*; evaluated *"on applications including
  Python auto-formatting, C compilers, and the SymPy symbolic math library."*
  `verified`.
- **C-Reduce**, Regehr, Chen, Cuoq, Eide, Ellison, Yang, *"Test-case reduction
  for C compiler bugs"*, PLDI 2012, pp. 335-346, abstract read 2026-10-25
  through OpenAlex (https://api.openalex.org/works/doi:10.1145/2254064.2254104):
  delta debugging *"typically yields test cases that are too large or even
  invalid"*; their best reducer's outputs are *"on average, more than 25 times
  smaller"*. `verified` (abstract only).
- **Delta debugging**, Zeller and Hildebrandt, *"Simplifying and isolating
  failure-inducing input"*, IEEE TSE 28(2):183-200, 2002, abstract read
  2026-10-25 through OpenAlex (https://api.openalex.org/works/doi:10.1109/32.988498):
  *"it simplified 896 lines of HTML to the single line that caused the
  failure. The study required 139 automated test runs or 35 minutes on a 500
  MHz PC."* `verified` (abstract only).
- **Routes**: the size parameter is **(A)** in the generator's own terms: the
  two property-testing tools bound *how many* and *how big*, and say what they
  stopped at; neither runs everything. Their reduction is the probe's
  `reduce.hero`'s ancestry and bears on no route of cost. Hypothesis's
  `deadline` is route **(J)** again (a slow example raises), with its own
  caveat: a time bound is turned off on CI by default because timing varies.

## 4. Spreading a deterministic run over cores or machines

- **LLVM lit**, its manual, read 2026-10-25
  (https://llvm.org/docs/CommandGuide/lit.html): `--num-shards M`, *"Divide the
  set of selected tests into M equal-sized subsets or "shards", and run only
  one of them."*, *"a coarse mechanism for partitioning large testsuites, for
  parallel execution on separate machines"*; `--timeout`, *"Maximum time to
  spend running a single test"*; `--max-time`, *"Spend at most N seconds
  (approximately) running tests and then terminate."* `verified`. Its source,
  `llvm/utils/lit/lit/main.py` on `main`, read 2026-10-25
  (https://raw.githubusercontent.com/llvm/llvm-project/main/llvm/utils/lit/lit/main.py):
  `test_ixs = range(run - 1, len(tests), shards)`, the note printing *"tests
  #({shards}*k)+{run}"*; the tests are ordered by `determine_order` before
  sharding, which may shuffle, sort by name, or order by earlier failures and
  elapsed times. `verified`. So lit's shard is **a stride with an offset over
  an ordered list**, and its reproducibility rests on the order.
- **GoogleTest**, *Advanced*, read 2026-10-25
  (https://google.github.io/googletest/advanced.html): `GTEST_TOTAL_SHARDS`
  *"must be the same for all shards"*, `GTEST_SHARD_INDEX`; *"Across all
  shards, each test function in the program will be run exactly once."*;
  *"GoogleTest reserves the right to change how the work is distributed across
  the shards."* `verified`.
- **Bazel**, *Test encyclopedia*, read 2026-10-25
  (https://bazel.build/reference/test-encyclopedia): *"the test runner is
  launched once per shard"*, `TEST_TOTAL_SHARDS`, `TEST_SHARD_INDEX`
  *"beginning at 0"*; *"Runners use this information to select which tests to
  run - for example, using a round-robin strategy."*; *"If tests are not
  properly hermetic then they do not give historically reproducible
  results."* `verified`.
- **AFL**, section 2.1: one exhaustive stage split by `exec_cksum %
  master_max`. `verified` above.
- **What the probe does today**, read in the lane at `01c5efd75` (a reading,
  not a run): `selfhost/probe/tokens.hero` lines 67 to 71 push, for every place
  a `bracket` break can stand, three edits in a fixed order, `T`, `O`, `B`
  (and `paren` pushes `A`, `B`, `C`, lines 115 to 121); `selfhost/probe/judge.hero`
  lines 143 to 148 count every edit of a family across the run and judge the
  edit whose index is a multiple of the stride. **So a stride divisible by 3
  judges only the first kind of every place, `T` in `bracket` and `A` in
  `paren`, and never an `O`, `B` or `C`.** On 259's line 18, `x[0][0]…`, the
  places are each `[` and each `0` (a `]` closes to depth 0 and is skipped,
  `tokens.hero` line 53), six variants per index, which agrees with the brief's
  12,000 for 2,000 indices; a stride divisible by 6 there judges one (token,
  kind) pair of the six. This is the periodicity risk of systematic sampling,
  stated on Wikipedia's *Systematic sampling*, read 2026-10-25
  (https://en.wikipedia.org/wiki/Systematic_sampling): *"The researcher must
  ensure that the chosen sampling interval does not hide a pattern. Any
  pattern would threaten randomness."*, with the example *"If we then sample
  every 10th household, our sample will either be made up only of corner
  houses"*. `verified` (the page; the application to the probe is mine).
  The strides the brief used, 100, 4202 and 1,000,000, are not multiples of 3;
  no route as the brief writes it forbids one that is.
- **Routes**: **(F)** has two precedents in exactly its shape: lit's stride
  with an offset and AFL's key modulo the count. Both keep a run reproducible
  only over a fixed order, which the probe has (*"counted across the run in
  the order the seeds are read"*, `judge.hero` line 117), and GoogleTest
  states the property the union must keep, *each exactly once*. **Against
  (A) and (C) as written**: a bare stride aliases with the probe's own
  periodic structure; the remedy has precedent in section 2.4 (a stable hash
  of the place) or is arithmetic (a stride coprime to 6).

## 5. Routes nobody listed

The shared brief asks what would have to be true for a route nobody listed to
exist. Each route below is named by a precedent above; none is measured here.

- **(G) The formatter caps its own work, by a count, and says so.**
  clang-format (25 million states) and dart_style (5,000, then 10,000
  attempts) bound the line search inside the formatter and keep the bound
  after rewrites (1.1, 1.2). It bounds `heroes fmt` for everybody, not only the
  probe; a 2,000-long chain in a user's program costs `heroes fmt` the same
  10 s it costs the probe. Its price, by both sources: on a capped line the
  output is not the formatter's best, and clang-format's silent *"do nothing"*
  is the shape to refuse (a diagnostic is owed). Whether a capped line's
  output can still be canonical and stable under the guard is the question
  this route must answer first.
- **(H) Sample by a stable hash of the place, not by a stride.** bynk's
  default (2.4): one in four, *"chosen by a stable FNV hash of (file, line,
  kind)"*, exhaustive behind a switch. Deterministic like a stride, and blind to
  the T/O/B period and the chain's period (section 4). Its cost: a hash
  sample of a fixed fraction still grows with the tree (Gopinath, 2.3).
- **(I) Classes of equivalent places decided by the formatter's behaviour,
  not by tokens.** AFL's effector map (2.1) skips a position whose change
  leaves the execution path unchanged. The critic's syntactic rule for (B)
  leaves 259 at 84,027; a behavioural key (for instance, what the formatter's
  plan decides for that place) might or might not collapse it. A question,
  not a finding; AFL measured 10 to 40% saved, not orders of magnitude.
- **(J) A variant over a bound is a finding, not a cost.** libFuzzer's
  `-timeout` (*"treated as a failure case"*) and `report_slow_units`,
  Hypothesis's `DeadlineExceeded`, AFL's `EXEC_TIMEOUT` *"used for detecting
  hangs"*, lit's `--timeout` (2.2, 3, 4). 259's 10 s variant would have been
  filed as a performance defect by any of them. Their own caveat: time is
  machine-dependent (Hypothesis turns its deadline off on CI), so the bound
  that stays deterministic is a count of work, as Black's test counts
  `right_hand_split` calls under 2n (1.4).
- **(K) Pin (E)'s gain with a test that counts work.** Black's #5475 test
  (1.4) keeps a complexity repair from regressing without a clock, which the
  probe's whole-tree run, by hand and hours long, cannot do between pushes.

**Which combination the precedents point to** (my reading): CPU and wall are
separate, as the brief says, and every project read here that met both used
one tool for each. For CPU, **(E)** first, in Black's terms (output held
identical over a corpus, the brief's 3,115-file check; a test that counts
work, (K)) and gofmt's (a memo whose key is the whole of what the answer
depends on). For wall, **(F)** split **inside** a seed, not only per seed: by
the brief's own inference 259 alone was about 52 CPU hours after the repair,
and a set of parallel jobs cannot end before its longest job, so a per-seed
`xargs -P` leaves 259 as the run's floor whatever the core count; lit's and
AFL's split by offset is the precedent for splitting it. (A) and (C) as the
run's main answer have the weakest precedent for the probe's question, and as
written carry the stride's aliasing.

## 6. Predictions a later run can falsify

Each under the brief's bounds: `timeout 300` (a longer run says why), output
through `head -c 50000000`, the `-O2` compiler of the seat's own copy.

1. **A stride divisible by 3 judges no `O` and no `B` variant of `bracket`.**
   Falsified by any `heroes probe <root> --family bracket --stride 3k` whose
   printed failures name an `O<n>` or a `B<n>` variant (`Failure.name` is the
   edit's name, `judge.hero` line 161). A reading of the code, not a run.
2. **A split by offset keeps the run exactly, as GoogleTest's shards do.** If
   (F) lands as a stride with an offset (each of N processes judging the
   indices `i, i + N, i + 2N, …` of the same ordered count), then for any
   seed the N processes' judged counts sum to the stride-1 run's and their
   failures, by name, are the stride-1 run's failures, no more and no fewer.
   Falsified on any seed whose stride-1 run ends under 300 s (for instance
   `tests/golden/ir/fixedbugs-394-a-literal-s-arguments-grow-in-place.hero`,
   which had 276 `bracket` variants when defect 503 was found) where the
   union differs.
3. **The dearest variant's cost tracks the chain, not the comment's place,
   once the fit question stops at the width.** If (E) makes the base case ask
   *does it fit in the width* rather than render the subtree (1.5, 1.6), then
   `/usr/bin/time -p heroes fmt` on 259's variants with the comment after the
   1st, 200th, 1,000th and 1,990th `[` of line 18 reads within a factor of 3 of
   each other and within a factor of 10 of 259 itself (0.07 s in the brief).
   Falsified by either ratio on the post-change compiler.

## 7. The seat's reading

- **verdict**: approve (advisory) of **(E)** for CPU together with **(F)**
  split inside a seed for wall; **object** (advisory) to a bare stride, (A) or
  (C) as written, being the run's answer.
- **precedents**: sections 1 to 4, each marked; `unverified` are Wong and
  Mathur's findings, Gopinath's concrete sample size, Oppen's own abstract,
  the year of Wadler's paper, and the tie between rustfmt 1.5.1's revert and
  #5403.
- **argument**: Six production formatters met superlinear cost on long or
  deep input. Black repaired this very subscript-chain shape in October 2026,
  held output identical over a corpus and pinned the repair by counting
  calls; gofmt memoised its measure-by-printing recursion in 2011 on a key
  holding all the answer depends on; rustfmt's memo, keyed on too little, was
  reverted. Wall is a separate bound: test runners and fuzzers split one
  ordered enumeration by offset (lit, AFL), each item exactly once. Mutant
  sampling (Offutt, Gopinath) keeps a score, not each finding; bynk samples by
  hash and keeps the exhaustive run one switch away; and the probe's stride
  aliases with its own T/O/B period.
- **condition**: my reading changes if a precedent shows a generator of this
  kind bounded by a bare stride without losing findings of its class; if the
  compiler seats measure that 259's remaining cost has no bounded-fit form
  (then (G), a counted cap with a diagnostic, rises above (E), as clang-format
  and dart_style chose); or if a sound (E) is shown to change output on any
  of the 3,115 files.
