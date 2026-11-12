# Panel 246, spec-warden

Read `00-shared.md` first, whole. You judge **Part B only**; Part A is the soundness lane and changes
no spec token. Your folder is `.claude/worktrees/p246-scratch/246-spec-warden/`, your tree `tree/` in
it at `b8c8240d4`, your compiler built there from the seed in two stages. Your report is
`docs/panel/246-reports/spec-warden.md` in `.claude/worktrees/p246`, in your definition's output
structure, written as you go. Reach design.md §1.6 by grep before you cite it (`grep -n '^### 1.6'
docs/design.md`: line 253; the ceiling's sentence at 255; the payment rule at 282, 301-302 and
324-334, `grep -n 'payment' docs/design.md`).

## The count before, measured while writing

`./heroes-next measure spec/heroes-spec.md` from `.claude/worktrees/p246`'s root (it reads the
vendored tables from the tree's root and refuses from another folder: *"cannot read
`vendor/tokenizers/claude-legacy.json`"*, measured from `p246-scratch/price/`):

| row | count |
|---|---|
| claude-legacy | 8236 |
| cl100k_base | 8386 |
| maximum (vendored) | 8386, *"a lower bound, not the reader's tokeniser"* |
| real | **10973**, *"claude-opus-5, 2026-11-11"*, *"the binding number"*: the instrument's pin for these bytes, no `STALE` printed |
| headroom | 5411 against 16384; the FFI floor mortgages 60, so 11033 is what the ceiling judges |

The ceiling is 16384 (design.md:255; `tests/harness/suite_spec.hero:162-163`). Run it again in your
own tree before you use it; a different figure is a finding.

## The candidates, priced on the vendored tables

Each is the spec at the base with the sentence in place, measured the same way (the drafts are in
`.claude/worktrees/p246-scratch/price/`, made by `p246-scratch/tools/drafts.py`; rebuild them in your
own folder from 00-shared.md's text rather than reading mine):

| draft | where | claude-legacy | cl100k_base | maximum | delta (max) |
|---|---|---|---|---|---|
| base | | 8236 | 8386 | 8386 | |
| T1 | § 13, after line 377's first sentence | 8309 | 8459 | 8459 | +73 |
| T2 | same | 8251 | 8401 | 8401 | +15 |
| S1 | same | 8304 | 8456 | 8456 | +70 |
| S2 | same | 8299 | 8451 | 8451 | +65 |
| S3 | § 11, after `exit(...)` | 8294 | 8444 | 8444 | +58 |
| T1 + S1 | § 13 | 8377 | 8529 | 8529 | +143 |
| T1 + S3 | § 13 and § 11 | 8367 | 8517 | 8517 | +131 |

**The real delta of a draft is unmeasured**: it needs `heroes measure --refresh`, a paid call this
brief does not name, so every verdict of yours on a draft is `provisional` and every vendored delta a
lower bound, said in those words. Two figures for scale, neither a measurement of these drafts: the
real-to-vendored ratio of the whole spec today is 10973 / 8386 = 1.3085 (`python3 -I -c`), and panel
234's F3 was priced **+37** real against **+29** vendored (`sed -n 347p
docs/panel/234-reports/spec-warden.md`). If a draft's real count decides your verdict, name the
`--refresh` run, its size and why, and the coordinator asks the author inside the sitting's 10 USD.

## The questions

1. **B1.** Does § 13 owe the rung? A reader holding today's spec is told threads come from
   `hero_os.h` (`spec/heroes-spec.md:377`) and nothing of their names, signatures or what crosses. Run
   what such a reader would plausibly write and what the compiler does with it: a guessed name; a
   `pthread_create` bound from `pthread.h`, whose Heroes body on a thread C made is stopped by
   `runtime/parts/thread.c`'s guard at run time (panel 114's R4 recorded *"panic: main.worker ran on a
   thread this program did not start"*, exit 134, `grep -n 'R4' docs/panel/114-*.md`: run it today);
   a `str` body (00-shared.md: refused at build by clang's header). T1 against T2 against nothing: the
   mistake each prevents and the error that mistake meets without it.
2. **B1 and F3 meet in one sentence.** Feature 2295's landing writes panel 234's F3 into the same
   sentence of line 377 (*"...; threads, processes and files from the language's `hero_os.h`, and
   sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group
   names one."*, `sed -n 347p docs/panel/234-reports/spec-warden.md`), in lane `core-platform`, in
   flight. Price B1 merged with F3 against appended to it, and say which order of landing the
   sentence survives.
3. **B2.** Principle 0 (CLAUDE.md § 2; design.md §1.0): the compiler does not need a stop to compile
   itself; what measured thesis effect, or what pre-registered prediction whose instrument exists
   today, pays for S1, S2 or S3, if any; and whether a stop belongs in the spec at all, or in a
   package's card, or in `hero_os.h`'s comments alone. S3 is a built-in: say what the `spec` suite's
   built-ins table and `selfhost/inventory.hero` then owe.
4. **The promise.** Panel 232's paragraph (`docs/panel/232-*.md:272-274`) promises *"the functions
   `hero_os.h` declares at 1.0.0, their names, their types and what each result hands its caller"*:
   T1 writes `hero_thread_spawn`'s signature into the spec, and S1 or S2 would add doors to that
   header. For each candidate, which promise the author takes on, and what a 1.x could no longer change.
5. **B3.** Whether anything of copy-on-write's window owes a spec token; design.md Part 7 item 13 calls
   § 2's *"There are no mutable globals"* *"the spec-side twin"* and says it *"stays as written"*
   (`grep -n 'stays as written' docs/design.md`: 4079; the spec's sentence is at line 135 today).
6. **The payment**: for each candidate you would admit, the named removal or the pre-registered
   falsifiable prediction, its instrument named.
