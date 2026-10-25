# Panel 229, the completeness critic, first pass: the briefs' framing facts

Written 2026-10-25, 06:08 to 06:30, by the completeness critic, before any
seat starts. I give no verdict.

**My copy**: `.claude/worktrees/rechecked-issues-scratch/229-critic/`, made by
`git -C <lane> archive a126f7a60 | tar -x`. **Two compilers**: the seed build
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 7.95`),
then stage 2 (`./heroes build selfhost/main.hero -o heroes-next`, `real 84.12`,
then `mv`), whose `--version` says `sources 1b247d9a35f7e25b`; and the `-O2`
build the shared brief names (`./heroes build selfhost/main.hero --emit-c >
m/stage.c`; `cmp m/stage.c ../p229/stage.c` prints nothing, so it is the
coordinator's C; `clang -O2 -w -I runtime m/stage.c runtime/runtime.c -o
heroes-o2`, `real 51.09`). Every timing below is `heroes-o2`, every run under
`timeout`, and every output is in `229-critic/m/`. The load average was 2 to 8
during this pass because other seats were building. So I give `real` beside
`user` for every timing, and a run whose `real` is far above `user` would be
discarded. None was.

Findings 1 to 4 change what the sitting is about. Findings 5 to 15 correct
numbers, citations and wording.

## Findings

### 1. The formatter IS what is slow on 259, and the depth is not why

**00-shared, § Measured today**: *"the formatter is not what is slow in 259
[...] Today's 6.32 s a variant against the formatter's 0.86 s for two passes
says the judges cost more than the formatter on that seed"*. **Also** *"(D) The
judges made linear"*, the cost table's label *"`fixedbugs-259-…` (depth
2,000)"*, and § *Why the depth is in the tree*. **compiler-engineer.md**,
task 1: *"the formatter, the guard (`judge.hero`), the independent reader"*.
**historian.md**: *"on a seed nested 2,000 deep both grow with the depth and
the run is about cubic"*.

(a) **`heroes fmt` is `guard.checked`, the same call the probe makes for each
variant**, so 2139's 0.33 s for formatting already includes the guard, one of
the two judges.

```
$ grep -n 'guard.checked' selfhost/cli/syntax_cmds.hero selfhost/probe/judge.hero
selfhost/cli/syntax_cmds.hero:47:    done = guard.checked(path: path, s: s)
selfhost/probe/judge.hero:72:    done = guard.checked(path: path, s: source.new_source(name: path, text: text))
```

`judge.hero` lines 2 to 4 say the same thing: *"formatted and guarded by
`print/guard.hero`'s `checked`, the one function `heroes fmt` calls"*. So the
guard is `selfhost/print/guard.hero`, not `judge.hero`. `judge.hero` is the
driver: it builds each variant's text, calls the guard and the reader, and
keeps the tallies.

(b) **I timed `heroes fmt` on 259 and on variants built by `tokens.hero`'s
rule.** The builders are `m/variants.py`, `m/variants2.py` and
`m/variants3.py`; each prints the line as `head` and a rest that starts 8
columns past the line's indent. Timings are `/usr/bin/time -p timeout 60
./heroes-o2 fmt <file>`. Every exit was 0 except where noted.

| variant of 259 | output bytes | user | real |
|---|---|---|---|
| 259 itself | 33,016 | 0.07 | 0.07 |
| line 16 (the 2,000-deep nest), T after the 10th / 1,000th / 1,999th `[` | 33,611 / 4,051,033 / 16,053,019 | 0.07 / 0.10 / 0.20 | 0.07 / 0.11 / 0.23 |
| line 16, T/O/B after the 1,400th `[` | 7.9 MB each | 0.14 each | 0.15 |
| line 16, T/O/B after the 10th and 800th `]` | 15.9 MB and 5.8 MB | 0.20 and 0.12 | 0.22 and 0.13 |
| line 25 (an index chain inside `print(`), T after the 10th / 1,000th / 1,990th `[` | 33,045 to 33,047 | 0.07 / 0.10 / 0.17 | 0.08 / 0.13 / 0.18 |
| line 25, T/O/B after the 1,000th `]` | (parser refuses: `line_end_before_continuation`, a discard) | 0.00 | 0.01 |
| **line 18 (`a: str = x[0][0]…`, an index chain 2,000 long at statement level), T after the 1st / 10th / 200th / 1,000th / 1,990th `[`** | 33,035 | **3.63 / 3.78 / 7.75 / 31.28 / 1.29** | 3.66 / 3.79 / 7.82 / 31.40 / 1.30 |
| line 18, O and B after the 200th `[` | 33,042 / 33,052 | 7.74 / 7.76 | 7.78 / 7.82 |
| line 18, T after the 1,000th `0` | 33,031 | 31.00 | 31.05 |

(c) **A profile.** I ran `sample` for 20 s on `heroes-o2 probe 259 --family
bracket --stride 4202`, output in `m/sample259.txt`. One worker thread does the
work; the main thread waits in `hero_thread_join` for 13,364 samples. The
worker's top-of-stack counts are `h_printaligned_written_pairs` **6,569**,
`h_printowners_index_at` 1,133, and then allocator frames, under a deep
recursion of `h_printbare_needs_group` / `h_printbare_part_needs`. **None of
the top frames is in `probe/reader.hero`.** `written_pairs`
(`selfhost/print/aligned.hero:145`) loops over `written` inside a loop over
`written`; its caller is `print/breaks.hero:192`, `restored`, which is the
formatter breaking *"a type, an index, a place"* after a comment. I read that
shape from the code; it is not a complexity measurement.

(d) **The 2,000-deep nest alone is cheap under the probe, both judges
included.** I wrote lines 14 to 17 of 259 plus `print(x.len())` into
`m/nest/nest-only.hero`, which parses, and ran `heroes-o2 probe <it> --family
bracket --stride 600`: 12,015 variants, 21 judged, `user 1.45`, `real 1.47`.
That is 0.069 s per judged variant, about 14 CPU minutes for the whole nest
(arithmetic, so an inference).

(e) **Where 259's variants are.** I applied `tokens.hero`'s rule to `heroes lex
--dump-tokens --json` (`m/bracket_by_line.py`; it gives 84,027 in total, the
probe's own count). Line 16 has 11,994 variants and line 20 has 12,000: these
are the two nests, 29%. Lines 18 and 22, the statement-level chains, have
12,000 each. Lines 25 and 28, the chains inside `print(`, have 18,006 each.
At the brief's stride of 4202, 6 of the 20 judged indices fall on lines 18 and
22: 12606, 16808 and 21010 on line 18 (indices 11,997 to 23,996), and 37818,
42020 and 46222 on line 22 (indices 35,997 to 47,996). **So the 6.32 s average
and the 147 hours rest on six samples of the statement-level chains.** The
other fourteen samples cost about 0.1 s each.

(f) **The other dear seeds have the same shape.** 264's line 24 is
`x = v.to_str().len().to_str()…`, 15,008 characters. 169-chains' line 23 is
`return v.inc().inc()…`, 12,012 characters. Both are 2,000-long chains at
statement level.

**Correction.** On 259, the cost is the formatter's (`guard.checked`, which is
what `heroes fmt` runs). It falls on a comment that breaks a 2,000-long index
chain at statement level: one such variant costs `heroes fmt` alone up to
31.28 s, against 0.07 s for the file unbroken. The 2,000-deep nest costs 0.069 s
per judged variant. The "deep" framing (the cube of the depth, *"a seed nested
2,000 deep"*, the label *"(depth 2,000)"*) describes the cheap part of 259.
**Route (D) as written ("the judges made linear") points away from the
measured hot spot**, which is in `selfhost/print/`: `aligned.written_pairs`,
`owners.index_at` and `bare.needs_group`. Which of these belong to the guard's
checks and which to printing is for the compiler-engineer's profile; I did not
measure that split.

### 2. A program a person could write takes `heroes fmt` 31 seconds

This follows from finding 1. `m/x/x-open1000.hero` is 259 with one comment
after the 1,000th `[` of line 18. It is 33 KB, and `heroes parse` takes it.
`/usr/bin/time -p timeout 60 ./heroes-o2 fmt m/x/x-open1000.hero` gives `user
31.28`, `real 31.40`, 33,035 bytes out, and nothing on stderr. That is a
`heroes fmt` cost on a correct program, the shape of defect 393 (*`heroes fmt`
costs the square of an array literal's length*, issue 1729, closed). **No
brief lists it as a route.** It would be a formatter repair under
`selfhost/print/`, filed as a defect by its class, with the probe and the
families unchanged, so panel 179's ruling is untouched. Because the repair
touches `selfhost/print/`, the same probe run would then be owed for it. I
filed nothing: classing it is the coordinator's.

### 3. The spec has no depth floor, and R6's N was never a spec sentence

**00-shared**: *"panel 184's R6 put a floor in the spec, not a ceiling (its
file, line 204): every nesting to 2,000 compiles. The cases that hold that
floor are required, and they are the seeds that do not end."*

```
$ grep -n -i 'nest\|depth\|deep' spec/heroes-spec.md
54: ... nested brackets and literals skipped;   94:constant MAX_DEPTH: i64
276: ... one nested in another argument.         287:- Recursion too deep aborts.
$ grep -rn -i '2,000\|2000\|floor\|nested up to\|its depth' spec/      (nothing)
$ sed -n 268,271p docs/panel/184-…md   →  "and R6's N, measured on this Mac, Linux x86-64 and the Windows box under R5 before its sentence is written"
$ sed -n 42,43p docs/panel/197-briefs/00-shared.md  →  "Panel 184's R6 floor of N = 2,000 was measured on this Mac and Linux arm64, Windows owed, and **is not a spec sentence** (critic 7)"
```

**Correction.** R6 *ruled* that the spec would state a floor once N had been
measured on three platforms. N = 2,000 was measured on this Mac and on Linux
arm64 (panel 197), and at `a126f7a60` no spec sentence states it. The cases
stay in the tree because `tests/golden/` is a record (`.claude/rules/records.md`
§ A record is never rewritten), not because the spec pins a depth. *"They are
the seeds that do not end"* holds for 259 alone. 264 and 169-chains take about
1.8 CPU hours each by the coordinator's sweep, and finding 1 shows that the
part that costs is chain length, not nesting depth. **Why it matters**: the
sentence makes any route that touches which seeds the probe reads look like a
spec question, and it is not one.

### 4. Defects 503 and 504: the wrong GitHub issues, and 504 is not the probe's find

**00-shared** and **compiler-engineer.md**: *"the formatter's probe found two,
503 and 504"*, *"the formatter defects the probe found (503 and 504, …)"*.

```
$ gh issue view 503 --json title  → "RATIFIED 2026-08-15 as it stands …" (panel 059's ratification)
$ gh issue view 504 --json title  → "ANSWERED 2026-08-15 …" (an FFI message)
$ gh search issues "Defect 503"   → 1849 Defect 503: `fmt` says *compiler bug* on a map constant broken after a key
                                    1850 Defect 504: a map constant whose body opens with `{` alone on its line is refused three times
                                    1852 Defect 506: `fmt` says *compiler bug* on a map key in parentheses whose `(` carries a comment
```

Defects 001 to 677 keep their own numbers (CLAUDE.md § 14, CL-083), so a seat
that runs `gh issue view 503` reads the wrong issue. Issue 1850's Origin says
504 was *"found beside defect 503 while reducing it"*: it is a parser defect,
`check` exiting 1 on a correct program, and no variant the probe made found it.
Issue 1849 says the probe's own find was 503: *"the `bracket` family over
`tests/golden/ir/fixedbugs-394-a-literal-s-arguments-grow-in-place.hero`, 51 of
276 variants refused by the guard"*. Issue 1852 says defect 506 was filed from
lane b15-parse's message, and that *"`heroes probe` on lane b15-print's v25
shape refuses 20 of 152 parsing variants for this cause"*: the probe confirmed
it, and verification.md's line 471 tally does not count it. Panel 179's 17
refusals (its lines 74 to 79: 12 in the `comments101` fixtures, *"T/O/B31,
T/O/B39, T/O/B59, T/O/B116"*, plus 5 bracket breaks of 225 in `walk.hero`) were
found by the seats' Python generators before `heroes probe` existed. Every
variant named there carries the bracket family's T/O/B tags.

**Correction.** Cite the defects as links: 503 is
https://github.com/heroes-lang/heroes/issues/1849, 504 is …/1850, and 506 is
…/1852. Say that 504 is the parser defect found by the reduction, not a probe
variant.

### 5. The seed count is 2,628, not 2,631

**00-shared**: *"2,728,284 `bracket` variants over 2,631 seeds; 487 files are
no seed"*. Those add up to 3,118, but the same sentence counts 3,115 files.

```
$ ./heroes probe <root> --family bracket --stride 1000000      (m/count-*.out; real 3.80 + 3.26 + 0.34)
probe: 684 seeds, and 0 files the parser refuses   (selfhost)   | bracket | 1546662 |
probe: 1824 seeds, and 487 files the parser refuses (tests)     | bracket | 1058277 |
probe: 120 seeds, and 0 files the parser refuses   (examples)   | bracket | 123345  |
$ wc -l p229/files.txt p229/counts.tsv  →  3115, 3118
$ grep -n -x '36\|39\|123' p229/counts.tsv  →  2348:36  2516:39  2519:123
```

**Correction.** 684 + 1,824 + 120 = **2,628 seeds**; 2,628 + 487 = 3,115. The
variant total of 2,728,284 is right. The extra 3 come from `count1.sh`. Its
awk has no `exit`, so for three files (`brackets180/shape075.hero`,
`paramcomment095/following.hero` and `paramcomment095/heroes.hero`) it also
matched the discards table's `| bracket | line_end_before_continuation |` row,
and each of those files wrote two lines. The stray halves `36`, `39` and `123`
are the *"three names that are no file"* the brief blames on the sweep's list.
The sweep did run those three seeds under their own names, so its sums stand:
I recomputed 405.03 s, 2,644,257 generated and 27,895 judged from
`sweep.tsv`. The same awk explains the stray `1`, `3`, `8`, `4`, `8` lines in
`cost.txt`: they are the discard counts of the 20 judged, and they show that
the "per variant" column divides by 20 including the variants the parser
refused.

### 6. "The largest" table is not the largest eight

**00-shared**, the table under *"The largest:"*.

```
$ awk -F'\t' 'NF==2 && $1 ~ /^[0-9]+$/' p229/counts.tsv | sort -t$'\t' -k1,1nr | head -14
84027 259 · 38658 477 · 36573 suite_records · 34728 walk · 33804 suite_surface · 32586 suite_spec
25368 selfhost/ir/flatten · 24264 selfhost/grammar_expr · 24051 539-a-thousand · 21051 539-875
19359 shell · 18936 records_github · 18036 169-brackets · 18012 thesis_run
(264 is 53rd with 6,195; 169-chains is 57th with 6,045)
```

**Correction.** The table leaves out the 6th to 12th largest and includes
264, which is 53rd. Retitle it *"the five largest and the deep cases"*, or
give the ranks.

### 7. The 2,000-deep or 2,000-long seeds are at least six, not three

**00-shared**: *"So the seeds 2,000 deep or long are three"*. The brief's own
table lists `fixedbugs-169-brackets-2000-deep`.

```
$ grep -i '2000\|two-thousand' p229/counts.tsv
84027 259 (two thousand deep) · 18036 169-brackets-2000-deep · 6195 264 (chain two thousand long)
6045 169-chains-2000-long · 57 489 (chain of records two thousand deep) · 48 169-one-operand-runs-2000-long
```

**Correction.** At least six by name. Three of them cost more than an hour
each: 259, 264 and 169-chains, and all three are long *chains* (finding 1).
169-brackets, a 2,000-deep nest, takes about 0.48 CPU hours (17.13 s × 100 in
the sweep). The other two are cheap.

### 8. "Toward the cube", and "about cubic", overstate 2139's numbers

**00-shared** carries *"toward the cube of the depth"* from 2139.
**historian.md** turns it into *"the run is about cubic"*. 2139's own numbers
(issue body, read today) are 0.84, 3.73 and 19.11 s at depth 100, 200 and 400:
each doubling costs 4.4 and then 5.1 times as much. Those are exponents of
log2(4.44) = 2.15 and log2(5.12) = 2.36; a cube would cost 8 times per
doubling. They are also measurements of a *nest*, which finding 1 shows is the
cheap shape on 259.

**Correction.** Say "between square and cube, 2.15 and then 2.36 per doubling,
for a nest". Do not carry that exponent to 259, whose cost is the chains'.

### 9. The six invocations do not spread the cost over the cores

**00-shared**: *"every other seed together about 7.7 [CPU hours] … which the
rule's six invocations side by side would spread over the Mac's cores"*.

The rule (verification.md, lines 306 to 308) runs one process per root and family, and
`sample` shows that a probe works on one thread (finding 1(c)). I split the
coordinator's `sweep.tsv` by root with `awk … sweep.tsv`:

| root | seeds | user at stride 100 | × 100 |
|---|---|---|---|
| tests | 1,823 | 296.88 s | 8.25 CPU h |
| selfhost | 684 | 104.89 s | 2.91 CPU h |
| examples | 120 | 3.26 s | 0.09 CPU h |

**Correction.** `tests` in `bracket` is one process. Even without 259 it is
about 8.25 hours of wall clock (an inference from stride 100), and no core
count shortens it. Journal 067 reads the same way: `selfhost`'s `bracket` took
14,034 s of CPU and `tests`' `single` 16,410 s, each in one process.

### 10. The briefs name no time bound and no byte bound

**00-shared**, § How every seat works: *"runs under `timeout`, its output to a
file or `/dev/null`"*. **compiler-engineer.md**: *"bounded by `timeout`"*. The
rule, verification.md § A run that may not end: *"a brief that asks a seat or
a lane for such a probe **names both bounds**, the time and the bytes"*, and
*"through `head -c <bytes>` into a file"*. Neither brief gives a number for
either bound, and *"to a file"* alone is not a bound. Formatter outputs here
are large: one variant formats to 16,053,019 bytes (finding 1).

**Correction.** Name a time limit (for example 300 s per run) and a byte limit
(`head -c`).

### 11. (D) may not change what panel 179 ruled

**00-shared**: *"None is chosen here; each changes what panel 179 ruled."*
Panel 179's items 1 to 8 (its lines 128 to 172) rule the verb, the in-process
calls, the families, the two judges and three rulings, the report, the exit
codes, the rows, and the order of landing. None of them rules how much a judge
or the formatter costs. A route that keeps every family and every verdict and
makes them cheaper, either (D) or finding 2's formatter repair, changes no item
of 179 as far as I read it. This is a question, not a measured fact: **which
item would (D) change?** If none, (D) is a defect's repair and not this
sitting's ruling, and saying *"each"* steers the sitting toward treating every
route as a change to the ruling.

### 12. "The formatter's 0.86 s for two passes" is not the probe's per-variant work

**00-shared**: 2139 timed `fmt` on the deepest variant (0.33 s) and then `fmt`
on that output (0.53 s). The probe calls `guard.checked` **once** per variant
(`judge.hero:72`) and never formats the output again. Today's compiler gives
`user 0.20` for that one variant (finding 1(b)). The comparison of 6.32
against 0.86 compares the average over six chain samples with one nest
variant.

### 13. "29 lines" understates 259's size

**historian.md**: *"147 CPU hours for one file of 29 lines"*.
`grep -c ''` gives 29 lines. `wc -c` gives 33,016 bytes, and the lines run to
4,007 to 6,036 characters. The lexer gives 32,068 tokens that are not comments
or terminators: 12,000 `[`, 12,000 `]` and 8,002 `int`. The `bracket` family
counts tokens, not lines.

**Correction.** "29 lines, 33,016 bytes, 32,068 tokens".

### 14. The waiver of 2026-10-24 is not cited

**00-shared**: *"counts as `blocking` since `m-thesis-harness` … so
M-rechecked-issues cannot be tagged while it is open"*. That much is true.
`gh issue view 2139` shows the label `class:adjacent`, created
2026-10-18T16:50:47Z. `m-witnessed-seed` is dated 2026-10-19 and
`m-thesis-harness` 2026-10-24 (`git for-each-ref`), so the second of them is
the second tag since the filing. But `m-thesis-harness` stands over 2139 by
the author's waiver, **decision 2200** (closed 2026-10-24, verdict *B*,
*"the next tag cannot stand over them"*), and its comment of 2026-10-24 says
so. Defect 2198 is OPEN (`gh issue view 2198`); its guard is in the tree
(`.claude/hooks/guard_bash.py:922`, `tagged.py`).

**Correction.** Cite 2200. It is the ruling that closes a second waiver as a
route.

### 15. Smaller wording issues

- **00-shared**: *"`lines.hero` 312 (`single`, `multi`)"*. `lines.hero` holds
  five families: `single`, `multi`, `ends`, `columns` and `texts` (its lines 7
  to 29; `judge.hero:24` to 25 lists all seven).
- **00-shared**: *"`bracket` (`tokens.hero:40`): inside a bracket, after every
  token…"*. The rule also never breaks beside a `str_piece`
  (`tokens.hero:57`), and it counts the break after an opening bracket.
- **compiler-engineer.md**: `gh search issues --repo heroes-lang/heroes
  "probe"` hits its 200-result limit (`m/probe-issues.json`). 46 of the 200
  titles name a pointee, layout, header or clang probe, which is a different
  instrument. `--label area:probe` gives one issue, 2139.
- **compiler-engineer.md**, (B)'s example: *"one per run of tokens that are
  the same kind at the same depth"*. I measured it with `m/runs.py` on the
  lexer's tokens. On 259 it collapses nothing: 84,027 variants before and
  84,027 after. On line 16 each `[` sits at a new depth, and on lines 18 to 28
  `[`, `0` and `]` alternate. It does collapse 264 (6,195 to 198) and
  169-chains (6,045 to 48). The example is offered as *"for instance"*, but on
  the file that costs 147 of the roughly 158 hours it bounds nothing.

## Framing that steers without a measurement

- The phrase *"Why the depth is in the tree"*, the label *"(depth 2,000)"*,
  the cube and the historian's *"nested 2,000 deep"* all point the sitting at
  nesting depth. Finding 1 measures chain length at statement level as the
  cost, so a seat would look for a rule about depth when the cost lies in long
  chains.
- *"the judges cost more than the formatter"* and route (D) point the profile
  at the judges and away from `selfhost/print/`. They also leave out the
  unlisted route of finding 2, a formatter repair that changes neither the
  probe nor 179.
- *"each changes what panel 179 ruled"* (finding 11) makes every route a
  sitting's change by assertion.
- *"a floor in the spec"* (finding 3) makes the deep seeds look untouchable by
  spec when no spec sentence exists.

## Facts verified true

- The lane is at `a126f7a60bed32b3d1eec34347a21b239e1a1c29`, branch
  `lane-rechecked-issues` (`git rev-parse`, `git branch --show-current`).
- `heroes --version` gives `sources 1b247d9a35f7e25b` for both stage 2 and
  `-O2`. The emitted C is byte-identical to the coordinator's `stage.c`.
- Line counts: `seed.hero` 214, `tokens.hero` 155, `lines.hero` 312,
  `judge.hero` 263, `reader.hero` 408, `reduce.hero` 135 (`grep -c ''`).
- `tokens.hero:40` is `function bracket`. `judge.hero:116` is the stride's
  comment. All variants are generated and counted before the stride test
  (`judge.hero:143` to 149).
- verification.md line 297 is the section heading. Line 471 says *"the
  formatter's probe 2 (503, 504)"*. The rule names six invocations and an
  `-O2` build.
- ROADMAP.md line 17 says what the brief says. Journal 067 (lines 206 to 220)
  backs it.
- Panel 179's item 3 is *"Every family the seats ran"*. Item 7 is quoted
  accurately. Item 8 is *"The 17 refusals found at `83ac68c1`"*, made of 12 + 5
  (its lines 74 to 79).
- Panel 184's line 204 is R6's heading.
- 3,115 tracked `.hero` files (`git ls-files`, which matches `files.txt`). 487
  files are refused: 403 under `check`, 53 under `surface-fixtures`, 15 under
  `full`, 13 under `permissive` and 3 under `control`. 2,728,284 variants.
- The cost table, re-timed one file at a time (`m/cost.txt`, `real` ≈ `user`,
  load 4 to 6), is within 6% on every row: 477 0.75 s, 539 0.71, 169-brackets
  2.02, walk 0.76, 264 19.87, 259 132.33 (the brief has 126.37). The
  arithmetic column follows from them.
- The sweep's sums match `sweep.tsv` exactly: 405.03 s user, 27,895 judged of
  2,644,257, `real 436.40`, `user 418.37`, `sys 11.24`. The heaviest-seeds
  table matches, with judged = ceil(variants / 100) on every row.
- 2139's numbers are carried accurately: 0.33 s, 16,053,010 bytes, 0.53 s,
  633 / 1,233 / 2,433 variants in 0.84 / 3.73 / 19.11 s. Routes A to C are the
  issue's three.
- 259 has 29 lines. The rule *"heroes probe"* applies to the fixtures, and the
  `FIXTURES` constant exists (`cli/probe.hero:54`).
- `.claude/skills/panel/SKILL.md` § 3c exists (its line 258), with the
  critic's first pass in it.
