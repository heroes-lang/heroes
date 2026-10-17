# Panel 219: the seed is the compiler's emission with a re-anchoring line joined to its anchor and a file name written where it changes, for every program

**Convened 2026-10-17 from 02:37, synthesised the same day from 10:40**, on issue 2086
(https://github.com/heroes-lang/heroes/issues/2086), a task of M-bounded-seed, by the author's
answer of the night of 2026-10-16 accepting the coordinator's recommendation, *convene the sitting*
(the milestone's panel gate). **Seats**: compiler-engineer, historian, then a completeness critic.
**Resolution: provisional, author ratification pending.** Brief: `docs/panel/219-briefs/00-shared.md`.
Reports: `docs/panel/219-reports/` (`compiler-engineer.md`, `historian.md`, `critic.md`). Every
number below is a seat's, with its command in that seat's report.

## The proposal, verbatim (the shared brief's question)

> The seed, `seed/heroes.c`, is the compiler's own emitted C that a clean checkout builds with one
> clang line; nobody steps through it in a debugger, and half of it is `#line` directives. **Should
> the seed be written in an emission profile of its own, and which: without `#line`, with the line
> number alone (C11 § 6.10.4), with a bootstrap root that leaves out `mutate`, `probe` and
> `measure`, or another shape; and how is that profile asked for, given that CLAUDE.md § 10
> refuses `--no-line` as a flag?** The milestone's target is a number: a seed under 10 MB.

Shapes s0 to s2 and the roots were the brief's; s3 and s4 the compiler-engineer's; the frozen
runtime beside the seed the historian's precedent, priced by the critic.

## What was measured

- **Where the 48% comes from** (compiler-engineer): not one `#line` per Heroes line, but a restore
  before every C line after the first that one Heroes line lowers to, since `#line N` anchors one
  line (design.md §3.1). One writer emits them all, `selfhost/emit/writer.hero`.
- **The shapes** (compiler-engineer, every size reproduced by the critic):

  | shape | bytes | `xz -9` | object without `-g` | line table under `-gline-tables-only` |
  |---|---|---|---|---|
  | s0, as written | 51,514,006 | 3,983,808 | — | 1,234,797 rows, 654 files |
  | s1, no `#line` | 26,803,431 | 2,504,280 | identical | the C file only |
  | s2, `#line N` with no name | 32,868,681 | 3,512,328 | identical | **false**: `s2.c:76` for `lexer.hero:76` |
  | s3, the name where it changes | 33,207,910 | 3,541,656 | identical | identical, 0 of 1,234,797 addresses |
  | **s4, s3 and the restores joined** | **25,638,593** | **2,655,336** | identical | **identical**, 0 of 1,335,312 addresses |

- **s4 built in the writer** (compiler-engineer, re-run by the critic): `selfhost/main.hero` emits in
  25,649,440 bytes (the critic: 25,649,882 with his build of the patch, a 442-byte gap unexplained);
  the fixpoint by `cmp`; `lldb` 14 and 0, `determinism`, `wholes`, `descriptors`, `full`,
  `unsupported`, `run` green; `lines` 532 and 0 once taught a bare `#line N` and an entry ending
  `goto bb0; bb0:`; `emit` and `emission` red only on *no longer emits what it was blessed to
  emit*, 636 files to bless again; 6 of the compiler's own tests expecting the old restore's text.
  `--emit-c` of the compiler, warm, still machine: `user` +1.5% to +2.0% (the seat), +1.1% to 1.2%
  with `user` plus `sys` about 1% lower (the critic). The writer reads 355 code lines, past § 11's
  300.
- **What s4 changes for an author** (the critic): a debugger and a sanitizer name the same `.hero`
  file and line; a column is a C column under s0 and s4 alike and moves; on the exit-2 path clang's
  quoted excerpt becomes the whole Heroes line's C (code lines over 500 bytes, 2,540 under s0 and
  10,330 under s4); `llvm-cov` ignores `#line` under both and only C-line coverage coarsens.
- **A defect of the trunk, found here**: R3's readers find a name in the line clang quotes, and
  Apple clang 21 quotes none past 4,096 bytes, so a header macro that breaks the program's C is
  `ffi_header_refused` at exit 1 up to 88 chained bindings and an internal error at exit 2 from 89,
  on s0 and on s4 alike (the critic; reproduced by the coordinator on the trunk's compiler, filed
  as defect 2117, `blocking`). s4 lengthens every prologue line by its signature and so moves the
  cliff toward smaller functions.
- **The bootstrap roots** (compiler-engineer): without `mutate`, `probe` and `measure`, 13.6% less;
  a root that only builds, 19.4% less, and it breaks the site's start page, which runs
  `./heroes run` after the clang line.
- **10 MB** (the critic, as a floor): every copy, every inline bounds check, every `#line`, all
  indentation and the 954 functions nothing calls removed leave 18,831,319 bytes; the build-only
  root keeps about 15.2 MB of that; identifier text is 69.7% of s4, and no seat measured a change to
  it. A compressed seed in the tree is refused by § 10 and by `seed/README.md`, and costs more
  history per regeneration than raw s4 (`xz -9` about 2,612 KiB against 2,440).
- **History** (compiler-engineer, three consecutive seeds in a throwaway repository): per
  regeneration s0 4,034 KiB, s4 2,440 (60%), s1 2,322 (58%).
- **The rhythm, for issue 2087** (the critic): `HERO_RUNTIME_ABI` moved 30 times, 8 in the 30 days
  since 2026-09-17, and the seed's `_Static_assert` refuses a moved header, so a seed refreshed only
  when the two-stage build goes red would be refreshed about 8 times a month at September's pace.

## Verdicts

| seat | verdict | ground |
|---|---|---|
| compiler-engineer | **adopt s4 for every program, no profile**; adopt s3 as its half; object s0, s1 as a seed-only profile, both roots at this milestone, every way of asking (flag, subcommand, environment, pipe); **veto** s2 (a false line table), a declaration in the root (the ceiling), the output path (§3.1 *Determinism*) | design.md §3.1; CLAUDE.md § 10; `.claude/rules/cli-surface.md` |
| historian (advisory) | **approve** a seed without `#line` (Nim's csources and Gambit's committed C carry none), the bootstrap root on the brief's condition (Zig's `dev = .bootstrap`), one fixed regeneration command and both rhythms; **object** to `#line N` alone (no precedent) and to a general flag | every precedent sourced and dated in the report |
| completeness critic | s4 holds on every number re-run but one size; the excerpt cliff is the trunk's, not s4's; 10 MB unreachable as a floor | conditions: file the excerpt defect first, report the cost in both columns, re-emit the patch at landing, say in design.md §3.1 and `generated-c.md` what an author sees move |

## Disagreements, stated plainly

- **No `#line` for the seed, or s4 for every program.** The historian approves a seed without
  `#line`, on two projects that commit emitted C with none. The compiler-engineer objects to it as
  a profile, a second emission mode whose determinism would be green in one mode only (panel 016's
  objection), asked for by a flag, a verb or a pipe that each costs surface or a script. **The
  measurement decides it**: s4 is smaller in the tree than s1 by 1,164,838 bytes, keeps the line
  table identical at every address, and needs no mode and no way of asking; s1's advantage is 118
  KiB a regeneration of history (4.8%).
- **A bootstrap root.** The historian approves it on Zig's precedent; the compiler-engineer objects
  at this milestone: it saves 13.6% and misses 10 MB anyway, and the clang-built compiler would stop
  being the whole one that 34 lines of 24 files print. Not adopted now; the rhythm's issue may
  weigh it again.

## Resolution (provisional: author ratification pending)

**R1, s4 for every program: the writer's one rule.** A `#line` that re-anchors the very line the
previous C line sits on is dropped, and its C line joined onto that one (never onto a directive, a
blank line, a preprocessor line, a line holding a newline or a `//`); and a `#line` names its file
only where the file clang presumes changes (C11 § 6.10.4). No profile, no flag, no verb: the seed
stays the compiler's emission of its own root. **Its conditions**:

1. **Defect 2117 repaired before s4 lands or with it**, since s4 moves the excerpt cliff toward
   smaller functions; its repair stops resting a blame rule on clang's quoted line (panel 214's R1:
   *decided by a clang run and by a name's form, never by clang's words*).
2. The join rule split out of `writer.hero`, each module below § 11's 300 by the `layout`
   instrument.
3. `suite_lines` taught a bare `#line N` and an entry ending `goto bb0; bb0:` in the same commit,
   its own tests with it.
4. `--emit-c` of `selfhost/main.hero` timed before and after on a still machine, `user` and `user`
   plus `sys` both reported.
5. design.md §3.1's `#line` bullet and `.claude/rules/generated-c.md` say that a re-anchoring line
   joins its anchor and a name is written where it changes, and what an author sees move: clang's
   C column and quoted excerpt on the exit-2 path, and C-line coverage by hand.
6. The emissions under `tests/emission/` and the `emit` goldens blessed again through the suites'
   own procedure, every moved file read; the committed writer re-emitting its own seed by `cmp`
   (the 442-byte gap closed or explained).

**R2, the 10 MB target is not reached by any emission profile**, measured as a floor; the shapes
that would reach it change the C every program gets (copy propagation in the lowering, bounds
checks out of line, the identifiers' mass), each a sitting and an ABI question of its own. The
milestone's target is the author's to rule (below).

**R3, the shapes and ways refused**, each with its measured reason: s0 (48% restores s4 replaces
losslessly); s1 as a seed profile (dominated by s4 in the tree, a second mode); s2 (vetoed, a false
line table); both bootstrap roots at this milestone; a flag, a verb, the environment and a pipe
(object: a second mode or a script in the bootstrap's trust path); a declaration in the root
(vetoed on the ceiling); the output path (vetoed, §3.1 *Determinism*); a compressed seed (refused by
§ 10 and `seed/README.md`, and worse per regeneration).

**R4, for issue 2087, the rhythm, measured here and not ruled**: refreshed only when red means about
8 refreshes a month while the runtime's interface moves at September's pace; a frozen copy of the
runtime beside the seed (33 files, 687,335 bytes) would make a refresh owed only when a new form
enters `selfhost/`, at the cost of a third bootstrap stage of about 70 s, priced and unbuilt.

### The question to the author

**M-bounded-seed's target was a seed under 10 MB. Should it become what the profile measures to
bound, the seed halved by s4 (about 25.6 MB, history 60% of s0's per regeneration) with part 2's
rhythm bounding how often it is paid, or stay 10 MB?** The recommendation is **amend it**: every
seat and the critic measured that no profile reaches 10 MB, and what would is lowering and emitter
work for every program's C, each with its own sitting; the milestone was opened for the history the
seed costs, which s4, the rhythm and the rewrite already made bound. The content changes are filed
as a task for a later milestone.

### What the other answer would be

The target stays 10 MB and M-bounded-seed stays open on sittings for copy propagation in the
lowering, bounds checks out of line in the runtime header and the identifiers' mass, each changing
every program's C, with no measurement yet that their sum reaches it (the critic's floor leaves
identifier text at 69.7% of s4).

## Predictions to score

| seat | prediction | instrument | when |
|---|---|---|---|
| compiler-engineer | with s4 the writer's one rule, the seed regenerated at M-bounded-seed's close measures 24,000,000 to 27,000,000 bytes and under 60,000 `#line`s; `lldb` and `lines` 0 failed; a regeneration costs at most 65% of an s0 one by the method of his § 7; no seed under 10,000,000 bytes unless a change to the C's content lands first | `wc -c`, `grep -c '^#line '`, the suites, a throwaway repository's `size-pack` | M-bounded-seed's close |

## Process notes

- **The brief's `git log -S` for the runtime's interface finds 1 commit and `-G` finds 30**: a value
  change keeps the string's count (the critic). Corrected here; the brief stands as written.
- **Found beside the sitting**: defect 2117 (filed 2026-10-17, `blocking`).
- **The historian sat**, so the ratification asks `## Prior art` of this resolution: it rests on no
  precedent the historian sourced (s4 was measured, not borrowed, and the precedents the historian
  approved, a seed without `#line` and a bootstrap root, were not adopted), so the ratification
  carries none (`.claude/rules/records.md` § And the record says whose idea it was).

## Author's verdict

Pending.

Asked for by https://github.com/heroes-lang/heroes/issues/2118, naming the sitting as
`panel 219`, with its one question and the recommendation.

**RATIFIED, 2026-10-17**, R1 to R4 as written above, the author answering in conversation that
morning on the coordinator's summary, which put this sitting's question and panel 218's, each with
its measurement and a recommendation: meant as *ok, I accept the recommendations*. So **the
question to the author is answered: M-bounded-seed's target is amended** from a seed under 10 MB
to what the sitting measured the profile to bound, the seed halved by s4 (about 25.6 MB, history
60% of the seed as written's per regeneration), with part 2's rhythm bounding how often it is
paid; the content changes 10 MB would need go to sittings of their own, outside this milestone.
The other answer is not taken. Recorded as a reading (CLAUDE.md § 4). The author may overturn it.
