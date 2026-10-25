# Panel 229: the probe ends; the formatter asks a chain once, and the run splits across processes

**Sat 2026-10-25** in M-rechecked-issues, on defect 2139, convened without asking again: the author's
yes for a sitting this milestone was given the same morning, for panel 228 (CLAUDE.md § 4, *ask once
per milestone*). **Seats whose input differs**: the compiler-engineer and the historian, with a
completeness critic before the seats and after them. Not seated, and why: the llm-ergonomist and the
spec-warden, since `heroes probe` stands in no sentence of the specification and no reader of the
language meets it; the ffi-pragmatist, since no route reaches the C boundary. The briefs are
`docs/panel/229-briefs/`, repaired after the critic's first pass and before any seat started; the
reports `docs/panel/229-reports/`, its evidence `evidence.tar.xz` and `evidence.md` beside them.
**Resolution provisional — author ratification pending**, its decision issue https://github.com/heroes-lang/heroes/issues/2210.

## The proposal, verbatim

From `229-briefs/00-shared.md`, § The question: *"how the run the rule owes becomes one that ends, on
this Mac, in a bounded wall time, without the probe losing the class of finding it exists for"*, the run
`.claude/rules/verification.md` § The formatter's probe owes where `selfhost/print/` moved, `single`
and `bracket` over `selfhost`, `tests` and `examples`; defect 2139, filed 2026-10-18: *`heroes probe
tests --family bracket` cannot end*. The routes as the brief listed them after the first pass: (A) a
bound per seed, (B) a bounded family, (C) the rule names the stride, (D) the judges made cheaper, (E)
the formatter made near-linear on a comment inside a long chain, (F) the run spread over the cores.

## The verdicts

| seat | verdict | section | cost or delta | predictions | condition |
|---|---|---|---|---|---|
| compiler-engineer | **approve (E) + (F) with an offset**; refuse (B); (A) and (C) not adopted; (H), a route it found, to a sitting of its own; no veto | §1.1, §3.5, Part 5, §1.12 | (E) +64 −26 in `print/page.hero`, `print/bare.hero`, `print/brackets.hero`; the offset +63 −34 in `cli/probe.hero`, `probe/judge.hero`, `cli/table.hero` | seven, below | — |
| historian | **approve (advisory) (E) for CPU with (F) split inside a seed for wall; object (advisory) to a bare stride**, (A) or (C) as written | precedent | — | three, below | a precedent bounding such a generator by a bare stride without loss; a cost with no bounded-fit form; a sound (E) changing an output |
| completeness critic | no verdict; first pass 15 findings, every one taken into the briefs; second pass 15 | — | — | — | — |

## Where the seats disagree, and which side the world takes

- **Where 259's time went.** The first brief read the judges as the cost (*"6.32 s a variant against
  the formatter's 0.86 s"*) and the depth as its shape. The critic's first pass ran `heroes fmt` on
  259's variants: **31.28 s on one comment after the 1,000th `[` of a 2,000-long chain written as one
  statement, 0.07 s on the file itself, 0.069 s a variant on the 2,000-deep nest**, and a `sample` put
  the time in `selfhost/print/`. The world took the critic's side, and the briefs were repaired.
- **Whether every failing place fails in all three forms.** The compiler-engineer found it true of
  every refusal it ran (defect 503's 17 places, panel 179's 4 and 5); the critic ran defect 506 on the
  day's compiler (`56def9b4`): **it fails in `T` and `B` and holds in `O`**. The world took the
  critic's side: a stride that is a multiple of 3 can lose a finding (R3).
- **Whether to split inside a seed.** The historian's case rested on the brief's 52 CPU hours for 259;
  after (E) 259 is about 2.6 (the critic, finding 6), and the offset buys 4.6 hours of wall down to 3.8
  to 4.0 (the compiler-engineer's passes, eight processes). No measured fact divides them; the sitting
  adopts the offset for what it guarantees, every variant once whatever seed sets the wall (R2).
- **The compiler-engineer's cost model** (0.85 µs a visit) does not fit as written (the critic,
  finding 3); nothing rests on it.

## The resolution, provisional — author ratification pending

**R1. (E) is the repair of defect 2139's cause, and it changes no item of panel 179.** `brackets.spread`
asks `bare.needs_group` at every part it descends into, and each answer asks the part below to the
bottom of the chain, rendering and aligning that part's whole text: the square of a chain's length.
`needs_group` is asked once per value and place, its answers kept on the page with all it reads that
changes (`page.Asked`: the next comment, and the place's `needed`, `open` and `taken`; the critic read
every function `bare.fresh` reaches and found nothing else of `f` or the place read, so rustfmt's
failure, a memo keyed on too little, does not apply); and `page.trim_end` cuts by bytes. Two tests
land with it: one that **counts the work** a long chain with a comment costs, so taking the memo out
turns it red (the historian's (K), Black's shape), and one that **pins the memo's premise**, one tree,
one comment list and one `lines` to a page (the critic, finding 1; `.claude/rules/module-shape.md`).
Measured at `-O2`: 259's variant with a comment after the 1,000th `[` of line 18, `heroes fmt` 9.85 s
to 0.09 s; 20 judged variants of 259, 45.70 s to 2.45 s; 264 at stride 100, 62.70 s to 1.39 s;
169-chains, 65.16 s to 2.30 s; **every one of the 4,126 tracked `.hero` files the same stdout,
stderr and exit**, and 3,427 `bracket` variants where the memo answers the same bytes (the critic,
finding 12); the owed run's work, about 70 cool CPU hours to about 15 (×100 of stride-100 sweeps, an
inference the critic held in instructions within 9% on five seeds run whole, finding 11).

**R2. (F): the owed run is split across processes, and `--offset` lands beside `--stride`.** The run
is one `heroes probe <seed>` per seed and family, unstrided, at `-O2`, as many side by side as the
machine has cores (journal 061's run of 2026-09-28 went so, 2,273 jobs seven at a time); a seed whose
own job would set the wall is cut by `--stride N --offset K` for K from 0 to N−1, which judge every
variant of it once between them. Measured: `--stride 4` at offsets 0 to 3 judged 276 of defect 503's
seed and parsed 204, the unstrided run's 204; `comments101/parens.hero` 291 and 204 the same way. The
flag is tool surface, item 7's stride extended. Eight processes on this Mac, a fanless MacBook Air M3
with four performance and four efficiency cores, deliver about 3.9 cool cores: the owed run's wall,
eight wide with 259 cut in eight, read 3.8 to 4.0 hours by ×100 of stride-100 passes, a lower bound
(the seat's one sustained run, 259 whole in eight parts, fell short of its bound under a load up to 90
that was not its own).

**R3. A stride that is a multiple of 3 is refused.** `bracket` writes `T`, `O` and `B` at every place
and `paren` `A`, `B` and `C`, in that order, so every such family's count is a multiple of 3 and a
stride that is one judges one form of each place in every seed (the compiler-engineer, read and
counted; the historian's prediction 1, run by the critic at `56def9b4`: stride 3 judged `T` alone).
Defect 506 holds in its `O` form, so the blindness is not hypothetical. `heroes probe` refuses such a
stride at exit 2, saying why; `suite_probe`'s census stride, 9,999,999, becomes 9,999,998.

**R4. The rule: the run is owed after the push, beside the CI, and nothing waits for it.**
`.claude/rules/verification.md` § The optimistic chain, the author's instruction of 2026-10-09, names
this run among what follows the push and says of the push *"Nothing else"*; eight commits under
`selfhost/print/` were pushed since while the probe stood unfinished, and journals 063 to 068 call it
the *post-push probe* (the critic, finding 2). § The formatter's probe's *"before a push ... push only
over exit 0"* and the three other places that still say so (that file's lines 94, 758 and 903) are
corrected underneath; panel 179's item 7 is a record and stands, this sitting's R4 its amendment. The
section gains the run's shape, R2's.

**R5. What closes defect 2139** (the critic, finding 9): the owed run itself, ended at exit 0, or at
exit 2 only on failures an open defect names, with its wall and CPU written beside the ×100
prediction. Until it has ended, the defect stays open, and with it M-rechecked-issues' tag.

**R6. Refused and left.** (B) is refused: it changes item 3, and after (E) it saves 0.12 of 10.20 CPU
hours. (A) and (C) are not adopted: the run ends without them, both change item 7, and their strides
would be R3's to refuse. Recorded, with no issue, since the run ends without them: (D), the judges,
about 44% of a variant's samples on `suite_records.hero` after (E) (the critic, finding 4); (H), a
variant judged inside its own declaration and its neighbours, 13 times cheaper a variant in the one
window measured and defect 503's 51 refusals kept, which changes what a variant is and owes a sitting
of its own; (L), the run sharded in the CI after the push (the critic, finding 5), a workflow change
that is the author's to push; (G), a counted cap with a diagnostic (the historian), whose condition,
a cost with no bounded form, (E) removed.

**What conservative would have been**: (E) alone, the rule's six invocations unchanged (about 8.1
hours of wall, `tests --family bracket` alone setting it), no offset, no refusal of a stride.

## What the sitting found beside its question

- **The rule said the run two ways for sixteen days** (R4).
- **The probe's stride aliases with its own forms** (R3), unseen until this sitting, though the
  census row of `suite_probe` has run with a stride that is a multiple of 3 since defect 155's repair
  (`027b436ea`, 2026-10-02), harmless there since it judges one variant of each root.
- **A machine measured is a machine named**: this Mac slows under sustained load, `suite_records.hero`
  at stride 100 reading 28.5 s cool and 38.6 to 43.2 s forty minutes into a run, `real` equal to `user`
  throughout, and eight processes deliver about 3.9 cool cores.
- **`selfhost`'s count moved by 33 variants** between `a126f7a60` and `01c5efd75`, `aligned.hero`
  grown by `written_pairs`' repair, which the critic's first finding led to and which landed before
  the seats sat (`01c5efd75`).

## Prior art

Read 2026-10-25, from `docs/panel/229-reports/historian.md`; each page re-read by the coordinator the
same day.

- **Reached first by Black**: [psf/black#5270](https://github.com/psf/black/issues/5270): *"A single
  line consisting of a chain of subscripts takes quadratic time to format"*, 0.033 s at n=40, 1.142 s
  at 80, 7.098 s at 160; closed as completed by #5475.
- **Reached first by Black**: [psf/black#5475](https://github.com/psf/black/pull/5475): *"Black was
  performing up to 20 full `right_hand_split` calls per recursion level"*; *"Output formatting is 100%
  invariant across all cases"*; a test that wraps `right_hand_split`, counts its calls and asserts them
  bounded linearly, `call_count < 2 * n`, chosen over wall-clock timing.
- **Reached first by Go**: [code review 4274075](https://codereview.appspot.com/4274075), *go/printer,
  gofmt: avoid exponential layout algorithm*: *"Use memoization to avoid repeated recomputation of
  nested node sizes"*.
- **Taken from rustfmt**: [rust-lang/rustfmt#5403](https://github.com/rust-lang/rustfmt/pull/5403),
  *Revert "Memoize format_expr"*: *"Memoizing expressions lead to cases where rustfmt's stability
  guarantees were violated."*
- **Taken from GoogleTest**: [Advanced GoogleTest Topics](https://google.github.io/googletest/advanced.html),
  distributing test functions to multiple machines: *"Across all shards, each test function in the
  program will be run exactly once."*
- **Reached first by LLVM's lit**: [lit, the LLVM Integrated Tester](https://llvm.org/docs/CommandGuide/lit.html),
  `--num-shards` and `--run-shard`: *"Divide the set of selected tests into M equal-sized subsets or
  'shards', and run only one of them."*

## Predictions to score

| seat | prediction | checkable at | falsified by |
|---|---|---|---|
| compiler-engineer 1 | with (E), 259's variant with a comment after the 1,000th `[` takes at most 0.20 s `user` at `-O2` | the landing | `/usr/bin/time -p timeout 300 <compiler-O2> fmt <x-open1000.hero>` reading more |
| compiler-engineer 2 | every tracked file gives the same bytes, stderr and exit with and without (E) | the landing | the files formatted by both compilers differing anywhere |
| compiler-engineer 3 | in the rule's six invocations at stride 100, `tests --family bracket` takes at least 250 s `real` | the landing | its `tools/six.sh` reading less |
| compiler-engineer 4 | the owed run, per seed, eight wide, 259 in eight offset parts, takes more than 3.8 hours of wall and refuses nothing | R5's run | that run finishing faster or exiting 2 on a failure no open defect names |
| compiler-engineer 5 | every root's `bracket` count is a multiple of 3 | now | `heroes probe <root> --family bracket --stride 1000000` printing one that is not |
| compiler-engineer 6 | (B)'s rule keeps at least 99% of the places at the next tag | the next tag | the sum of `tools/lexone.sh` over the seeds |
| compiler-engineer 7 | defect 503's seed on `56def9b4` with (E) still gives 51 refused of 201 parsing | now | the day's probe printing another count |
| historian 1 | a stride divisible by 3 judges no `O` and no `B` variant of `bracket` | run by the critic, held | a strided run naming an `O<n>` or `B<n>` failure |
| historian 2 | runs split by offset judge exactly the unstrided run's variants and failures | the landing | any seed whose offsets' union differs |
| historian 3 | once (E) bounds the fit question, the four line-18 variants cost within a factor of 3 of each other and of 10 of 259 itself | the landing | either ratio on the landed compiler |

## Author's verdict

Pending: the decision issue `Panel 229:`, https://github.com/heroes-lang/heroes/issues/2210, holds the
ratification. A yes settles R1 to R6, the landing on them and the rule's amendment; it does not settle
the owed run's result, which R5 makes defect 2139's closing, nor (D), (H), (L) or (G), recorded for a
later sitting.

## Process notes

- **The critic's first pass changed what the sitting sat on**, before any seat started: the cost was
  the formatter's, on long chains, and `written_pairs`' quadratic, which it found, landed at
  `01c5efd75` as an ordinary repair the same morning; the briefs were repaired and re-frozen there.
- **The compiler-engineer stopped at the API's weekly limit** at 12:50 with its verdict unwritten,
  and finished on the author's new account, resumed from its own report (`.claude/skills/panel/SKILL.md`
  § 3c); the critic's second pass read the report before that ending, so its finding 7 is answered.
- **Writes outside the repository's root**: the compiler-engineer's stray `cp -R` of `selfhost/` to
  `/private/tmp/nonexistent-check` at 12:49, found and removed by the seat and recorded in its report;
  the harness's own files of the critic's background commands under `/private/tmp/claude-501/`, which
  the critic names and did not write.
- **Timings under others' load**: the seats' clocks ran beside each other and beside the coordinator's
  sweeps; every number here was taken with `real` beside `user`, and the one sustained run that fell
  short is said so (R2).

## The landing, on the provisional resolution

**R1 to R4 landed** at `01c5efd75` (`written_pairs`, the critic's first finding) and `9a627663a`
(the memo keyed on the value's span and counted, a test that counts the work and went red with the
memo's answers never taken, `--offset`, the refusal of a stride that is a multiple of 3, the census
stride 9,999,998, `.claude/rules/verification.md` amended), every one of the 4,126 tracked `.hero`
files formatting to the same output and exit as `01c5efd75`'s; the batch's gate green over the
merged lane at `3f52c6fea`, pushed the same afternoon.

**R5, as the author answered on 2026-10-25**: asked whether to close defect 2139 on its seed judged
whole and let the rest of the owed run follow the push, as § The optimistic chain has every
post-push check do, the author answered *yes, close now*, on the coordinator's recommendation. So
the defect closed on **259's `bracket` family judged whole**: eight parts at `--stride 8 --offset
0` to `7`, each at exit 0, 84,027 variants judged between them, 72,012 parsing and 72,012 held,
none refused by the guard or the reader, 15,654.78 s of `user` in all and 2,395 s of wall for the
longest part, under a load of 35 to 90 from two other sessions' gates beside it (their windows in
the run's own record). The rest of the owed run, 6,247 jobs over a copy of the lane at
`3f52c6fea` with the compiler at `-O2`, goes on as the post-push check; its result is written on
the defect's issue when it ends, and a refusal becomes a defect of its own. Its wall is not a
measurement: two sessions' gates ran beside it.

**Predictions scored at the landing**: the compiler-engineer's 1 **held**, 259's variant with a
comment after the 1,000th `[` of line 18 formatting in 0.14, 0.14 and 0.15 s of `user` at `-O2`
on the landed compiler, under a load of 62 to 78; its 2 **held**, the 4,126 files the same; its 5
**held**, every root's `bracket` count a multiple of 3 (`selfhost` 1,546,695, `tests` 1,058,277,
`examples` 123,345, 259 84,027); the historian's 2 **held** on three seeds, the offsets' judged
counts summing to every variant (503's seed 276, `comments101/parens.hero` 291, 259 84,027); the
historian's 1 and 3 and the compiler-engineer's 3 and 7 were run by the seats themselves and held
there. The compiler-engineer's 4 waits on the rest of the run, its wall not a measurement now; its
6 waits on the next tag.
