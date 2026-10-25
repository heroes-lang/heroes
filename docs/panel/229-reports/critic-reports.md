# Panel 229, the completeness critic, second pass: what the reports leave out

Written 2026-10-25 from 13:05, as I go, by the completeness critic, after the
historian's and the compiler-engineer's reports (the latter stopped at an API
limit before its verdict and predictions). I give no verdict. Every finding
carries its command and output; my files are in
`.claude/worktrees/rechecked-issues-scratch/229-critic/` (`m2-*` and `m2/`).

## The state of the machine when this pass began

At 13:08 `uptime` read a load of 58 / 71 / 54, and `ps` showed eight
processes left by the compiler-engineer's seat after it stopped:
`../m/heroes-s3 probe <259> --family bracket --stride 8 --offset 0..7`, each
under `timeout 2400` and `/usr/bin/time -p`, started by its
`tools/split259.sh s3 8` at 12:49:27 (its log: `load before { 3.15 3.18 5.44 }`).
That is the seat's direct run of all 84,027 of 259's `bracket` variants in
eight offset parts, the measurement its x100 inference needed, which its report
does not mention. I left it running and timed nothing of mine until it ended
(CL-025); its result is finding 8.

## Findings

### 1. The memo of route (E) step 1: its key is complete, by reading every function it reaches

**The question** (the historian's 1.3: rustfmt's memo of `format_expr` was
reverted because *"the result is not a function of just `shape` and `expr`"*):
does `bare.needs_group`, memoised on `(value, f.next_comment, at.needed,
at.open, at.taken)`, read anything of `f`, `at` or `c` beyond that?

**Read in the seat's patched copy** (`229-compiler-engineer/src/selfhost/print/`,
`m/route-e.patch` applied there; I read, I did not write):

- `bare.fresh` (the old body, `bare.hero:66-142`) reads `at.taken` (`:73`),
  `at.open` (`:83`) and `at.needed` (`:84`, `:93`, `:127-142`), and never
  `at.all` or `at.enclosed`; `around.Place` has those five fields and no
  other (`around.hero:28-33`).
- It reads `f` only through `elements.holds`, `elements.waits`,
  `elements.alone` (`elements.hero:253-302`), `around.just_inside`
  (`around.hero:46-49`, which is two `waits`), and `one_line` ->
  `breaks.loose` -> `breaks.plan` -> `breaks.restored`
  (`breaks.hero:56-117`, `:185-209`). `grep -n "f\.\(next_comment\|lines\|printed\|last_line\|dropped\|moved\|asked\)"`
  over `elements.hero`, `around.hero`, `breaks.hero`, `aligned.hero` and
  `parens.hero` prints `f.next_comment` in `holds`, `alone`, `waits`, `loose`
  and `restored`; `f.lines` only in `around.reach` (`:38-42`), which `fresh`
  does not call; and the writes `f.last_line @`, `f.next_comment @` only in
  `breaks.pieces` (`:177-178`), which `fresh` does not reach (`loose` calls
  `plan`, never `pieces`). Nothing else of `f` is read.
- **`c` is the page's own in production.** `grep -rn "elements.Code("`
  prints five constructions: `brackets.hero:54`, `:68` and `breaks.hero:21`,
  each `Code(tree: tree, s: s, comments: comments, lines: f.lines)`, and
  `breaks.hero:272` and `aligned.hero:234`, both inside `test` blocks
  (`grep -n '^test "'`: `breaks.hero:267`, `aligned.hero:230`). Every
  production caller of `brackets.broken`, `brackets.led` and
  `breaks.led_text` (`fmt.hero:455, 550, 729, 748, 763, 1003, 1010, 1051`,
  `headline.hero:137, 162, 232`) passes `tree, s, comments` by those names,
  which `fmt.format_lexed` builds once (`fmt.hero:125-136`); `f.lines` is
  written once per page, at `fmt.hero:175`, before anything is printed, and
  `page.blank_page()` is called once per format (`fmt.hero:126`; the others
  are tests and `margins.hero`'s tests). `grep` for a page compared or copied
  (`Fmt ==`, `= f$`) prints nothing.
- So within one page `fresh`'s answer is a function of exactly the key, and
  the memo is the gofmt shape (historian 1.5: the key is the whole of what the
  answer depends on), not rustfmt's. **What the reading cannot exclude**: a
  later change that makes `fresh` read another field of `f` or `at`, or hands
  `needs_group` a second tree on one page. Nothing in the patch guards that
  but `Asked`'s doc comment; the run in finding 12 is what tests the outputs.
- **What the landing owes for it, by `.claude/rules/module-shape.md` § A
  narrowing asks the value, never the world**: the memo's correctness rests
  on a premise about the callers (one tree, one comment list, one `lines`
  per page, and `fresh` reading only `next_comment` and three fields of the
  place), not on the value in hand. That section owes such a premise *"a
  falsifiable claim"* and *"a test that fires when it dies"*. `grep -c
  "^+test\|^-test" m/route-e.patch` prints 0: the patch adds no test and no
  case, and its eight touched tests only call `page_at`. A test that, over a
  fixture with comments inside chains, asks `fresh` beside every answer the
  memo gives and asserts they agree would fire when either premise dies; so
  would a key that holds the premise itself. The project's own precedent for
  this class, defect 393's case (`tests/golden/run/fixedbugs-393-…`, *"No
  golden form holds a cost, so this case holds the three loops' output"*),
  pins the output and not the cost; nothing in the net would notice the
  square coming back except this run, hours long, after a push (the
  historian's route (K), Black's test counting calls, is the cost pin no seat
  took up).

### 2. The rule conflict: five places state when the probe is owed, and they disagree

```
$ grep -n "formatter's probe\|probe by hand\|run by hand\|Before the push\|After the push\|Nothing else\|Layer 4\|push only over exit 0" .claude/rules/verification.md
94:  | `selfhost/print/**` ... and the run by hand below before a push |
297: ## The formatter's probe, by hand, before a push that touches `selfhost/print/`
475: 1. **Before the push**: gate A ... and gate B ...
482:    and 604). Nothing else. ...
488: 3. **After the push, beside the CI**: ... the formatter's probe where
491:    `selfhost/print/` moved. What they find is filed by its class; nothing waits for them.
758: 4. the formatter's probe by hand if `selfhost/print/` moved, the site's build if ...
903: - **Layer 4, around the push**: the site's build before it; the census, R2, the probe and the CI's legs after it
```

and at the origin, **panel 179's item 7** (its file, lines 162-166): *"The
full single-comment and bracket-break run over the tree stays a run by hand,
**required before a push that touches `selfhost/print/`**, written in
`.claude/rules/verification.md`."* The compiler-engineer found two of the five;
the table row (94) and § The batch's gate item 4 (758, in a list whose item 5,
the Linux x86-64 container, § The optimistic chain item 4 also removed) are
the other two stale ones.

**Which stands, by four measurements, not by preference:**

- **Dates.** Panel 179 and line 297's section: 2026-09-28. § The optimistic
  chain: the author's instruction of 2026-10-09, which names the probe in its
  item 3 and says of the time before the push *"Nothing else"*; Layer 4 at
  line 903 restates it with that date. The later author instruction, naming
  this run by name, is the one the author gave last.
- **Practice.** `git log origin/main --since=2026-10-09 -- selfhost/print/`
  prints 8 commits (`dddea0cf3` to `7ce66022e`, M-inferred-cell to
  M-witnessed-seed), all pushed while the probe they owed stood unfinished
  (ROADMAP.md line 17).
- **The record calls it the post-push probe.** `grep -n -i probe
  docs/records/journal/06[2-9]*.md`: journal 063 line 118-119 *"After the
  push, as the optimistic chain says: the formatter's probe"*; 065:116,
  066:104, 067:147 *"owed after the push by the optimistic chain"*; 068:58
  *"M-readonly-lend's post-push probe"*.
- **The author's amendment of 2026-10-12** (lines 313-314, *"are we wasting three
  hours on a comment?"*) was given on that post-push run (journal 063
  lines 118-128), and it narrows the run further; it says nothing to bring it
  back before the push.

**What the sitting should say**, as I read it: § The optimistic chain stands;
the run is owed after the push, beside the CI, its findings filed by class,
and nothing waits for it; the words *"required before a push"* in panel 179's
item 7 are read as amended by the author's instruction of 2026-10-09 (the
sitting's file says so beneath, since 179's file is a record), and the four
stale places of `verification.md` (lines 94, 297 and 306-312, 758; line
197-198's *"as a `selfhost/print/` change goes through the probe by hand"* is
neutral) are amended in the landing, each with its date. **What it changes in
the sitting's question**: the wall time blocks no push. It blocks a tag only
through 2139 itself (a run that cannot end; the next tag cannot stand over it,
decision 2200) and through whatever the run files as `blocking`. So *"in a
bounded wall time"* has no number behind it (finding 9), and route (F)'s
offset, priced by the seat at 4.6 -> 3.8-4.0 hours, buys under an hour of a
run nobody waits for.

### 3. The compiler-engineer's cost model does not fit as written

Report lines 90-96: *"about (n-k)^2/2 level visits plus (n-k) renders ... the
1st pays the visits (2 million) ... The model fits the four timings within 10%
(a = 0.85 us per visit, b = 9 us per index rendered and aligned)."*

```
$ python3 -I -c '...n=2000, a=0.85e-6, b=9e-6, measured {1:3.41, 200:6.07, 1000:9.85, 1990:0.28}...'
k=    1 measured  3.41  model (n-k)^2/2:  1.72 (0.50x)  model (n-k)^2:  3.41 (1.00x)
k=  200 measured  6.07  model (n-k)^2/2:  4.62 (0.76x)  model (n-k)^2:  5.99 (0.99x)
k= 1000 measured  9.85  model (n-k)^2/2:  9.43 (0.96x)  model (n-k)^2:  9.85 (1.00x)
k= 1990 measured  0.28  model (n-k)^2/2:  0.18 (0.64x)  model (n-k)^2:  0.18 (0.64x)
```

With the visits it states, (n-k)^2/2, the model is off by half at the 1st;
it fits three timings within 1% only with (n-k)^2 visits (or a = 1.7 us),
and the 1,990th only once the file's own 0.07 s is added (0.25 against
0.28). A fit, said to be one, whose text and numbers disagree by a factor of
2; nothing rests on it since step 1 was measured directly (0.08 to 0.10 s).

### 4. Route (D) was priced before (E) and never again; after (E) the judges are half of what is left

The shared brief prices (D) as *"measured not to be where 259's time is"*,
true of the compiler before (E). The compiler-engineer's own profile after
step 1 (`229-compiler-engineer/m/sample-s1-records.txt`, the probe on
`suite_records.hero`, 6,007 worker samples), split by direct callee with my
`m2/tools/kids.py`:

```
$ python3 -I m2/tools/kids.py .../m/sample-s1-records.txt h_probejudge_judged
h_probejudge_judged 5997:  5254 h_printguard_checked   623 h_probereader_compare   92 h_source_new_source
$ ... h_printguard_checked
h_printguard_checked 5254: 3106 h_printguard_judged_output   1204 h_parse_parse_source   938 h_printfmt_format_lexed
$ ... h_printguard_judged_output
h_printguard_judged_output 3106: 943 h_printfmt_format_lexed   855 h_parse_parse   638 h_printanchors_first_moved_read   519 h_printdump_dump_ast   135 h_source_new_source
```

Of a variant's 5,997 samples: the formatter twice, 1,881 (31%); the two
parses, 2,059 (34%); the guard's anchors and tree dump and the reader, 1,780
(30%). So after (E) the judges proper (re-parse, dump, anchors, reader:
2,635, 44%) cost more than the formatter, on the seed that is the tree's
third dearest. No seat priced (D) on step 1's compiler; at most it halves the
run (the formatter and the input's parse stay), it touches the guard every
`heroes fmt` runs, and like (E) it changes no item of panel 179. One profile
of one seed, read by me from the seat's file: not a measurement over the tree.

### 5. A route nobody listed: (L) the run in the CI, sharded, after the push

**What would have to be true for it to exist**: that the run is owed after the
push (finding 2 says it is), and that something other than this Mac can run
it. The shared brief rules *"on this Mac"* into the question (§ What this
sitting rules), so no seat looked. Read in the lane:

```
$ ls .github/workflows/   ->  ci.yml codeql.yml deploy-site.yml release-site.yml
$ grep -n "runs-on\|matrix\|heroes probe\|run tests/harness" .github/workflows/ci.yml
177:    runs-on: ${{ matrix.os }}        291: matrix: include: Linux x86-64 (ubuntu-latest), Linux arm64 (ubuntu-24.04-arm),
                                              Darwin arm64 (macos-26), Windows x86-64 (windows-latest)
773:        run: ./heroes${{ matrix.exe }} run tests/harness/main.hero -- ./heroes${{ matrix.exe }}
```

The CI already builds the compiler and runs the whole net (its `probe` suite,
the fixtures and `multi`) on four legs after every push; `heroes probe <root>
--family bracket|single` is never run there. **The route**: a workflow job,
after the push and beside the CI as § The optimistic chain puts the run,
building the `-O2` compiler once and running the six invocations split per
seed (or per stride offset) across a matrix of runners, each shard's report
an artifact, a refusal filing a defect by its class. It moves the run off a
fanless laptop the author works on (findings 8 and 9), and it gives the run
a second platform at no new cost in the tree.

**Its premises, unrun here**: how many cores a standard runner of a public
repository has, the per-job and per-workflow time limits, the concurrency a
free account allows, and whether a run of this size costs money (the brief
counts *"a cloud run"* as paid, and a CI job is a cloud run the project
already spends on every push). Those are the historian's to read on GitHub's
documentation, and the author's to decide: a workflow change is pushed, which
is asked every time (CLAUDE.md § Hard stops). It needs no new flag for a
split per seed (the seat's § Route (F): `heroes probe <one file>` already
exits 0, 1 or 2 by file), and an offset only for a seed that is itself the
longest job.

### 6. The historian's case for (F) inside a seed rests on a number (E) removed

The historian's § 5 (its lines 629-636): *"by the brief's own inference 259
alone was about 52 CPU hours after the repair, and a set of parallel jobs
cannot end before its longest job, so a per-seed `xargs -P` leaves 259 as the
run's floor whatever the core count; lit's and AFL's split by offset is the
precedent for splitting it."* The compiler-engineer's step 1 moved 259's
`bracket` family to **2.62 CPU hours** (stride 100 x 100, its line 153) or
2.9 (its 20-variant sample), and its own passes price what the offset then
buys on this Mac: `p8-whole-1` 164.13 s against `p8-split-1` 137.51 s and
`p8-split-2` 144.00 s, x100 **4.6 against 3.8-4.0 hours** (its lines 291-296),
because 259's whole job (`real` 164.10 there, 94.20 cool and alone) is still
that pass's longest. So the precedent stands and its premise does not: with
(E), splitting inside a seed buys 13 to 17% of a wall that, by finding 2,
nobody waits for, at the price of a flag (+63 -34 lines, tool surface, this
sitting's matter). The historian's own condition (its § 7: *"my reading
changes if ... the compiler seats measure that 259's remaining cost has no
bounded-fit form"*) and its prediction 3 (the dearest variants *"within a
factor of 3 of each other and within a factor of 10 of 259 itself"*) are both
met by step 1's table, 0.08 / 0.08 / 0.09 / 0.10 s against 0.07 s, though by a
memo and not by the bounded fit question the prediction named as its cause:
the base still renders the commented part whole, once per value instead of
once per level. **The two seats do not contradict each other on any measured
fact**; the historian's argument for the offset is the brief's 52 hours,
which no seat re-read after step 1.

### 7. The seat with a veto has no verdict, no refusals and no predictions

`tail -2 docs/panel/229-reports/compiler-engineer.md` reads *"(Verdict and
predictions below.)"* and nothing below it. Its brief (§ What your report
states) owes *"the route or combination you would adopt and why ...; the
routes you refuse with the measurement that refuses them (a veto is a refusal
on soundness, not a price); ... and your predictions, each with the command
that would falsify it"*. The report holds the measurements and prototypes
from which a verdict would be read, and two refusal-shaped findings (a stride
that is a multiple of 3 judges T alone, its lines 393-404 and 441-448; (C)
needs an exclusion the probe lacks, 451-454), but **no seat has said which
routes it vetoes**, and the compiler-engineer is a seat with veto power
(`.claude/agents/compiler-engineer.md`). Nothing in its files says it
intended to veto anything; the synthesis may not read a verdict into its
measurements. The seat can be resumed with its context, or the synthesis
says that this seat gave none. Its last measurement, still running when this
pass began (§ The state of the machine), is read in finding 8.

### 8. The seat's direct run of all of 259 under (E) did not end in 40 minutes on eight cores

**What ran** (the seat's `tools/split259.sh s3 8`, its `m/split259-s3-8.log`,
read at 13:29:31, after its bound):

```
load before { 3.15 3.18 5.44 } at 12:49:27
wall 2401 s, load after { 14.91 17.73 31.01 } at 13:29:28
part 0: real 2400.07 user 1973.33   ...   part 7: real 2400.07 user 1975.63
$ wc -c < split259-s3-<k>.out   ->  0 for k = 0..7
```

Every part was killed by its `timeout 2400` and printed no report: **15,808 s
of `user`, 4.39 CPU hours, spent without finishing 259's `bracket` family**,
which the seat priced at 2.62 CPU hours (x100) and 2.9 (x84,027/20). At 13:08
`ps` showed the eight at state `RN` (reduced priority) and 15 to 85% CPU each,
with the load at 58 to 71; my own commands, foreground and background, run at
nice 0 (`ps -o stat,nice -p $$`), so why the seat's ran niced is unexplained.

**The cool cost is what the seat said**, measured on my build of
`route-e.patch` (`sources 6772f37917366513`, the seat's `heroes-s2` key), one
run at a time, load 2.5 to 2.8:

```
$ tprobe.sh heroes-e-o2 bracket 97  <259>   ->  user 97.41  real 98.07  1,821.054 G instructions  84027 generated, 867 judged, 0 refused
$ tprobe.sh heroes-e-o2 bracket 100 <259>   ->  user 94.84  real 95.65  1,765.157 G instructions  84027 generated, 841 judged, 0 refused
$ tprobe.sh heroes-s3   bracket 400 <259>   ->  user 24.05  real 24.42    444.191 G   211 judged     (the seat's binary)
$ tprobe.sh heroes-e-o2 bracket 400 <259>   ->  user 23.97  real 24.16    442.915 G   211 judged
```

0.1124 and 0.1128 s, 2.10 billion instructions, a judged variant at a prime
stride and at 100: **259 is about 9,460 cool CPU seconds, 2.63 hours**, and
the seat's binary with the offset costs what mine does. Stride 97 walks every
residue mod 6, so the even strides (100, 4202) did not bias the sample; I
also timed all 24 forms around the 1st, 200th, 1,000th and 1,990th `[` and
`0` of line 18 under (E) (`m2/e-v18b.tsv`): 0.07 to 0.10 s, 1.32 to 1.86
billion instructions, every one.

**So what the run measured is the machine, not the formatter**: each part
needed about 22 trillion instructions (84,027 / 8 x 2.10 G), 1,194 s on a cool
performance core at 18.5 G a second (444 G in 24.0 s), and with 1,976 s of
CPU each it did not get there, so the eight parts ran at under 60% of that
rate per CPU-second for forty minutes, on four performance and four efficiency
cores of a fanless case, niced, beside a load of up to 71. The seat's own
passes (2 to 2.5 minutes each) read 3.8 to 3.9 cool cores; this 40-minute run
delivered **fewer than 3.94** (8 x 1,182 s of cool work not done in 2,400 s),
by how much it cannot say, having printed nothing. **The x100 wall figures
(3.8, 4.0, 4.6 and 8.1 hours) are cool-machine figures, and the only
sustained run of this sitting, under the conditions it had, fell short of
its cool prediction.** A run of hours on this machine is a question that only
running it settles (finding 9).

### 9. The question the sitting should have asked: what run closes 2139, and what bound is it held to?

*"In a bounded wall time"* (the shared brief's § What this sitting rules)
names no bound, and no seat asked for one. Three things make that the
sitting's missing question rather than a detail:

- **Every hour figure in both reports is an extrapolation.** The x100 of a
  stride-100 sample (the compiler-engineer's 10.3, 4.8, 15.0 CPU hours, its
  8.1 and 3.8-4.6 wall hours), the x84,027/20 of a 20-variant sample (2.9
  hours for 259), the brief's 11.3 and 147. Nobody has seen the owed run end
  since 2026-09-28 (issue 2139's Notes: *"The last complete run, 2026-09-28
  ... 2,273 jobs seven at a time in 57 minutes"*), when the tree held 1,136
  `.hero` files under the three roots against 3,115 today (`git ls-tree -r
  --name-only <tag> | grep '\.hero$'`, finding 10). Findings 8 and 11 test
  the multiplication where it can be tested inside 300 s; the run itself is
  the one test of the whole.
- **What closes 2139 is not written.** Its title is *"cannot end"*. Read
  plainly it closes when the run the rule owes ends, which after (E) it is
  expected to do in hours, with no offset, no bound and no new flag; read as
  *"ends within N hours on this Mac"*, N decides whether (F)'s offset or (A)
  is needed at all. By finding 2 nothing waits on that wall but the next tag.
- **Whose machine.** The run is hours of all eight cores of the author's
  fanless MacBook Air (the seat's lines 219-244: *"It slows under sustained
  load"*, 28.5 s becoming 38.6 to 43.2 s after forty minutes; background
  priority 5.4 times slower), and the author's own words on 2026-10-12 were
  *"are we wasting three hours on a comment?"*. Whether the author accepts an
  owed run of four to eight hours on that machine, once per milestone that
  moves `selfhost/print/`, is a question for the author with a recommendation,
  not a premise the seats could measure; route (L) (finding 5) is the answer
  if the answer is no.

**What the sitting can still do**: name the run that closes 2139 (the six
invocations and the fixtures, on the landed compiler, ended, their wall and
`user` recorded beside the x100 they were predicted by) as its own falsifier,
and ask the author the bound.

### 10. The run's growth: the tree's square jumped once and has grown 4.5% since

No seat asked whether the run that ends today ends next month. The rule's
own cost model (`verification.md` lines 335-340, 2026-09-28: 25 us per seed
line per variant, 8.6 variants per line, *"73.3 million for the sum of the
squared line counts of the 1,020 seeds, about 4.4 CPU hours"*), re-run on
each tag:

```
$ for t in <tags> 01c5efd75; do git grep -c '' $t -- 'selfhost/*.hero' 'tests/*.hero' 'examples/*.hero' | awk -F: '{s += $NF*$NF; n++} END {...}'; done
m-agreed-retention (2026-09-28)  files  1134  sum of squared lines   76.5 million
m-inferred-cell    (2026-10-11)  files  2698  sum of squared lines  406.0 million
m-readonly-lend    (2026-10-18)  files  2964  sum of squared lines  409.2 million
m-thesis-harness   (2026-10-24)  files  3096  sum of squared lines  421.5 million
01c5efd75          (2026-10-25)  files  3113  sum of squared lines  424.3 million
$ ... | sort -nr | head
47900241 tests/golden/full/fixedbugs-489-...two-thousand-deep...   39150049 tests/harness/suite_records.hero
25321024 tests/golden/run/fixedbugs-140-...a-thousand-deep-build   (and three more 140 cases at 25.2 million each)
16224784 tests/golden/run/fixedbugs-539-...a-thousand-deep...      13579225 tests/harness/suite_spec.hero
```

Read with the model, today's tree is about 25.3 CPU hours for both families
(the 487 files the parser refuses included, which cost almost nothing), the
same order as the compiler-engineer's 15.0 measured with step 1 (x100). The
square grew 5.3 times between 2026-09-28 and 2026-10-11, the thousand-deep
cases entering, and **4.5% in the fourteen days since**. So the routes that
buy a constant factor, (E), (D), (F), (L), keep their price on the scale of
months at the present pace; (A), (B) and (H) are the ones that bound growth,
and growth is not today's problem. The line model is coarse: 489's 47.9
million squared lines give 57 `bracket` variants, so the dear cases are dear
by tokens per line, not lines.

### 11. The x100 multiplication, tested on five seeds at stride 1: it holds in instructions, not always in seconds

Every CPU-hour figure of the compiler-engineer's (10.2, 4.8, 15.0) is a
stride-100 sweep times 100. I ran five seeds both ways under (E), one run at
a time, load 2.7 to 4.9 (`m2/x100-a.tsv`, `m2/x100-b.tsv`; `tprobe.sh` is
`timeout 300 /usr/bin/time -l heroes probe <seed> --family bracket --stride
<n>`, output through `head -c 50000000`):

| seed | stride 100: `user` s, G instr | x100 | stride 1: `user` s (`real`), G instr | x100 / stride 1, seconds; instructions |
|---|---|---|---|---|
| `tests/harness/suite_special.hero` (6,426) | 0.58, 9.515 | 58, 951.5 | 54.36 (54.72), 875.684 | 1.07; 1.09 |
| `tests/harness/cards.hero` (6,690) | 0.70, 10.923 | 70, 1,092.3 | 75.38 (75.87), 1,178.890 | 0.93; 0.93 |
| `selfhost/check/builtins.hero` (9,567) | 1.13, 18.380 | 113, 1,838.0 | 112.95 (114.51), 1,745.226 | 1.00; 1.05 |
| 264 (6,195) | 1.41, 23.175 | 141, 2,317.5 | 162.94 (172.34), 2,286.378 | **0.87**; 1.01 |
| 169-chains (6,045) | 2.37, 38.860 | 237, 3,886.0 | 233.39 (236.17), 3,828.192 | 1.02; 1.02 |

Every run read 0 refused. **The multiplication is sound in work**: within 9%
either side on the three ordinary seeds and within 2% on the two chain seeds,
so the seat's 10.2 + 4.8 CPU hours is a fair count of the work, and seeds
under 100 variants (where x100 overcounts) carry 0.24 s and 0.17 s of the
two sweeps (`awk` over the seat's `sweep-s1-*-100.tsv`), nothing. **It is not
sound in seconds over minutes**: 264's stride-1 run did the predicted work in
13.5% more CPU time, 14.0 billion instructions a CPU-second against 16.4 in
the short runs, alone on a quiet machine. Finding 8 is the same effect over
forty minutes and eight processes. So the sitting has a measured count of the
owed work (about 15 cool CPU hours with (E)) and **no measured wall** for it;
what the Mac delivers over hours is unrun.

### 12. The memo's outputs, compared where it works: 3,427 `bracket` variants, every one the same bytes

The compiler-engineer's equality runs were the 4,126 tracked files as
written (canonical files, few comments inside a value, where `fresh` returns
at its first test), 259's 20 judged variants, 503's seed on the day's tree,
and verdict counts (not bytes) over the stride-100 sweeps. The memo answers
from its table only where a comment stands inside a value no bracket holds,
which is what the `bracket` family makes. So I compared bytes there.

**How**: `m2/tools/brk.py`, my own port of `tokens.hero`'s `bracket` and
`seed.splice` over `heroes lex --dump-tokens --json` (a token whose JSON
text is not its source bytes is skipped: 136 places of `suite_records.hero`,
none elsewhere), its counts equal to the probe's own for every seed below;
`m2/tools/cmp1.sh`, each variant through `heroes-base-o2 fmt` (`sources
48ac4ee426d58db0`) and `heroes-e-o2 fmt` (`6772f37917366513`), each under
`timeout 300`, stdout through `head -c 50000000`, its exit and the `shasum`
of stdout and of stderr compared; four at a time (`xargs -P 4`), no timing.

```
d394       (503's seed, every variant)        276 variants, 276 same, 0 DIFF; base exits 204 x 0, 72 x 1
c101inner  (comments101/innermost.hero, all)  213 variants, 213 same, 0 DIFF; 180 x 0, 33 x 1
c101parens (comments101/parens.hero, all)     291 variants, 291 same, 0 DIFF; 204 x 0, 87 x 1
records    (suite_records.hero, stride 49)    738 variants, 738 same, 0 DIFF; 538 x 0, 200 x 1
walk       (selfhost/check/walk.hero, 47)     739 variants, 739 same, 0 DIFF; 580 x 0, 159 x 1
gexpr      (selfhost/grammar_expr.hero, 43)   565 variants, 565 same, 0 DIFF; 447 x 0, 118 x 1
s264       (264, stride 31)                   200 variants, 200 same, 0 DIFF; 199 x 0, 1 x 1
s169c      (169-chains, stride 31)            195 variants, 195 same, 0 DIFF; 195 x 0
s259       (259, stride 401)                  210 variants, 210 same, 0 DIFF; 180 x 0, 30 x 1
```

(Exit 1 is a variant the parser refuses, a discard; no variant reached exit
2 under either compiler.) Every stride is prime to 6, so every form and
token of a chain is in the sample. **3,427 of 3,427 the same**, 605 of them on
the three chain seeds. Together with finding 1's reading, the rustfmt
warning (historian 1.3) does not apply to this memo on any input run; the
shapes of finding 13 add their outputs.

### 13. The shapes beside the repaired one: (E) removes the square on every chain tried, and moves no byte

The compiler-engineer measured (E) on one shape directly (259's index chain,
a comment after the k-th `[`, n = 2,000) and on 264's and 169-chains' method
chains through the sweep. CLAUDE.md § RUN IT owes the shapes beside it.
`m2/tools/shapes.py` writes one statement per file, a chain n long broken by
the `bracket` T form (a trailing comment, the rest eight columns in) after
the middle unit (`one`), after a third and two thirds (`two`), or after every
100th unit (`many`), or not at all (`none`): `index` `x[0][0]…`, `method`
`v.f().f()…`, `binary` `(x + x + …)`, `args` `f(x, x, …)`, `array`
`[x, x, …]`, `mixed` `v.f(x)[0].g…`, at n = 500, 1,000, 2,000 (and 4,000 under
(E)). `m2/tools/tfmt.sh`: `timeout 300 /usr/bin/time -l heroes fmt`, one at
a time; `m2/shapes-b.tsv`, `m2/shapes-e.tsv`. Billions of instructions
retired, base -> (E):

| shape, comments | n = 500 | 1,000 | 2,000 | 4,000, (E) only |
|---|---|---|---|---|
| index, one | 11.315 -> 0.165 | 44.369 -> 0.310 | **178.753 -> 0.610** (10.06 s -> 0.03 s) | 1.236 |
| index, two | 9.694 -> 0.186 | 38.108 -> 0.352 | 153.458 -> 0.698 | 1.414 |
| index, many | 4.007 -> 0.221 | 8.607 -> 0.443 | 18.206 -> 0.904 | 1.954 |
| method, one | 0.713 -> 0.083 | 2.720 -> 0.151 | 10.741 -> 0.303 | 0.660 |
| method, many | 2.161 -> 0.086 | 9.605 -> 0.157 | 40.995 -> 0.312 | 0.671 |
| binary in `( )`, one | 0.856 -> 0.086 | 3.338 -> 0.154 | 13.420 -> 0.314 | 0.694 |
| binary, two | 0.426 -> 0.083 | 1.572 -> 0.151 | 6.146 -> 0.305 | 0.675 |
| mixed, one | 0.740 -> 0.087 | 2.861 -> 0.158 | 11.287 -> 0.307 | 0.636 |
| mixed, many | 1.375 -> 0.093 | 7.985 -> 0.178 | 38.234 -> 0.355 | 0.739 |
| args, array, any; every shape `none` | the same both ways, linear (0.06 to 0.27) | | | |

- **Before (E)** every chain with a comment grows about 4 times per doubling
  (index, one comment: 3.9 then 4.0); the binary chain inside parentheses,
  which no seed of the sitting holds at length, is the same square at a
  smaller constant. **Under (E)** every row about doubles (1.79 to 2.21 per
  doubling, the low end binary-one's 0.154 / 0.086, where the file's own
  constant still shows), so the square is gone for one, two and n/100 comments, and the
  index chain at 4,000 costs 1.24 billion instructions where the base would
  be about 700 by its own ratio (unrun at 4,000).
- **Output**: 72 files run both ways (n <= 2,000; `field` left out, below),
  the same exit and the same stdout `shasum` on every one.
- **What I could not make**: a comment in a field chain `v.g.g…` stands
  outside every bracket, and the parser refuses it (exit 1 under both,
  `m2/shapes-e.tsv`), which is why the `bracket` family never makes it.
  Unary chains and `?` chains were not tried.

### 14. Two runs on the day's compiler: a stride of 3 judges T alone, and defect 506 is a finding that depends on the form

The compiler-engineer could not run 506 (*"the file not being in either
tree"*) and wrote *"Every failing place I ran fails in all three forms ...
so on this record it would have cost nothing"*. Both are now run, with a
copy of its build of `56def9b4` (`m2/bin/heroes-56def9b4`, `shasum` equal to
`dl/56def9b4/heroes2`, `--version` 0.2.0), the tree on which issue 1852 says
506 *"fails the same way"*.

**The historian's prediction 1, run** (`m2/d503/`, 503's seed copied from the
day's tree):

```
stride 1 exit 2: | bracket | 276 | 276 | 201 | 150 | 51 | 0 |   refused: T, O and B at 76 77 78 80 81 82 84 ... (51)
stride 3 exit 2: | bracket | 276 |  92 |  67 |  50 | 17 | 0 |   refused: T76 T77 T78 T80 T81 T82 T84 T85 T86 T88 T89 T90 T92 T93 T94 T96 T97
```

At stride 3 every judged variant is a T and no O or B is named; 503 is still
caught, in its T form at all 17 places. Not falsified.

**Defect 506 by form** (`m2/d506/`): two seeds of the shape issue 1852 names,
comment-free, through the day's `heroes probe --family bracket` (`check`
exit 0 on both):

```
oneline.hero  first = {("ziggy"): 8} ...                       exit 0: | bracket | 147 | 147 | 114 | 114 | 0 | 0 |
lone.hero     first = {  /  ("ziggy"): 8  /  }   (v25's shape)  exit 2: | bracket | 129 | 129 |  96 |  92 | 4 | 0 |
              refused: T38 B38 T54 B54; first: "T38, the guard: `fmt` would move the comment on line 6 ... away from the code it was written beside"
$ heroes lex lone.hero --dump-tokens --json | (code tokens)  ->  38 lparen '(' line 6;  54 lparen '(' line 11
```

The comment trailing the key's `(` (T, and B which holds it) is refused at
both places; the same comment on a line of its own after the `(` (O38, O54)
is held. **So the record holds a finding that fails in two forms of three**,
and the seat's *"fails in all three forms"* is true of 503 and panel 179's
places, not of the record. A stride that is a multiple of 3 still catches
506 (its T fails, and a multiple of 3 always lands on a T); a defect of the
O form alone would not be caught, and 506 shows the forms do part. Both seats'
remedy stands and now rests on a run: a stride prime to 3, or a sample by a
stable hash of the place, never a bare multiple of 3. Panel 179's 17 at
`83ac68c1` were not re-run by me.

### 15. Three of the seat's claims checked by run, all holding

- **`m/route-e.patch` is what was measured.** Applied with `patch -p1` to a
  fresh `git archive 01c5efd75` (`m2/e/`), it builds (seed, stage 2 of the
  pristine tree, `--emit-c`, `clang -O2`) to `sources 6772f37917366513`, the
  seat's `heroes-s2` key; `cmp` of my patched `print/bare.hero`,
  `page.hero` and `brackets.hero` against the seat's `src/` prints nothing.
- **`m/route-e-and-offset.patch` applies to a pristine `01c5efd75`**:
  `patch -p1 --dry-run` in `m2/base/` patches its six files, exit 0.
- **The fixtures' unstrided run is small**, as the seat said without a
  number: `timeout 300 /usr/bin/time -l heroes-e-o2 probe` (no path, every
  family) in `m2/e/`: exit 0, `real 15.33 user 15.22`, 251.8 billion
  instructions; 67 seeds and 3 files the parser refuses; `single` 6,638,
  `multi` 603, `bracket` 9,210 (6,801 parsing), `paren` 2,253 (1,395), every
  parsing variant held (`m2/fixtures-e.out`).

## What is measured and what is not, by route

I give no verdict. This is what the reports and this pass leave measured,
for the synthesis to weigh.

- **(E), measured as a repair**: on every chain shape tried, one, two and
  n/100 comments, n to 4,000, the square is gone (finding 13); on 259's dear
  variants 3.4 to 9.9 s become 0.08 to 0.10 s (the seat; finding 8's 24 forms);
  the bytes are the same on 4,126 files (the seat), 3,427 `bracket` variants
  and 72 shapes (findings 12, 13); its memo's key is complete by reading
  (finding 1). It changes no item of panel 179 (the seat's item-5 table; the
  first pass's finding 11): a defect's repair, 393's class. **Unmeasured or
  missing**: a test that fires when its premise dies (finding 1, owed by
  `module-shape.md`), and the owed run on it, ended.
- **The owed work with (E)**: about 15 cool CPU hours (`bracket` 10.2,
  `single` 4.8, the fixtures 15 s), the x100 sound in instructions within 9%
  on five seeds (finding 11). **Its wall on this Mac is unmeasured**: the
  x100 walls (3.8 to 8.1 hours) are cool-machine figures, and the one
  forty-minute run of the sitting fell short of its cool prediction
  (finding 8).
- **(F) by the caller, per seed**: needs no new surface (the seat), and is
  `verification.md`'s text. **(F)'s offset**: measured to buy 4.6 -> 3.8-4.0
  hours on the seat's 2-minute passes; the historian's reason for it was a
  number (E) removed (finding 6), and by finding 2 nobody waits for that
  wall. It is tool surface, so this sitting's to admit or not.
- **(A), (B), (C)**: none is needed for the run to end after (E); (B) saves
  1.2% after (E) (the seat); (A) and (C) must never stride by a multiple of 3,
  now on a run (finding 14: 506 fails in T and B and holds in O).
- **(D)**: after (E), the judges are about 44% of an ordinary variant's
  samples on one profile (finding 4); unpriced over the tree.
- **(H)**: the seat's inference, 10.2 -> 4.9 CPU hours, unrun over the tree,
  and it changes what a variant is (the seat: a sitting of its own).
- **(L), the CI**: unlisted, its premises unrun (finding 5).
- **The rule**: § The optimistic chain stands by date, practice and record
  (finding 2); the sitting has to say so and have four places amended.
- **The missing question**: what run closes 2139, held to what bound, on
  whose machine (finding 9), and the veto seat's own verdict (finding 7).

No paid run and no credential printed. Every `fmt`, `probe`, `lex` and
build of mine ran under `timeout 300` (the `timeout 2400` above was the
seat's), every compiler output through `head -c` into a file of
`229-critic/m2/` (two `--version` checks ran bare). Nothing of mine was
written outside the repository's root; **the harness itself kept files
outside it**: the output files of my three background commands and one
monitor under `/private/tmp/claude-501/…/tasks/` (their printed lines: the
time, the seat's log, nothing else), and one tool result too large to show,
the seat's report read with `cat`, under `~/.claude/projects/…/tool-results/`.
I did not remove them; that is the coordinator's or the author's call.
