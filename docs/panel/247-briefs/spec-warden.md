# Panel 247, spec-warden

Read `00-shared.md` first, whole: **R4 is a ratified rule, no Heroes code runs in a handler, and this
sitting rules how it is enforced or honoured, never whether**. You judge what the specification must
and must not say for the route adopted, and Principle 0's burden for any sentence. Your folder is
`.claude/worktrees/p247-scratch/247-spec-warden/`, your tree `tree/` in it, a `git archive` of
**`ae5e2cedd`** (tree `3e22d53467b606946a70a6f9dfdea6f46ea47612`), your compiler built there from the
seed in two stages (`sources 7cc70d136b590d46`), then D applied and stage 2 built again
(00-shared.md § D, the floor; `sources 560c95206c908459`). Your report is
`docs/panel/247-reports/spec-warden.md` in `.claude/worktrees/p247`, in your definition's output
structure, written as you go. Reach design.md by grep before you cite it (`grep -n '^### 1.6'
docs/design.md`: 253; `grep -n 'payment' docs/design.md`: 282, 301, 324, 328, 330 and 4053;
Principle 0, `grep -n '^### 1.0' docs/design.md`: 112).

## The count before, measured at the base

`./heroes-next measure spec/heroes-spec.md` from the base's root (it reads the vendored tables from
the tree's root):

| row | count |
|---|---|
| claude-legacy | 8340 |
| cl100k_base | 8490 |
| maximum (vendored) | 8490, *"a lower bound, not the reader's tokeniser"* |
| real | **11108**, *"claude-opus-5, 2026-11-13 — the binding number"*, the instrument's pin, no `STALE` |
| headroom | 5276 against 16384; the FFI floor mortgages 60, so 11168 is what the ceiling judges |

The ceiling is 16384 (`tests/harness/suite_spec.hero:163`). Run it again in your own tree before you
use it; a different figure is a finding. D changes no spec token (`git diff --stat ae5e2cedd 932876f95`
names `runtime/parts/alloc.c` and three files of its case, none under `spec/`).

## The candidates, priced on the vendored tables at the base

Each is the base's spec with the sentence appended, one leading space before it, after the one
occurrence of `` `atexit(f: (function() -> ()))`. `` (spec line 404, § 13's callback sentence). The
coordinator's drafts are `.claude/worktrees/p247-scratch/price/spec-<name>.md`, made by
`p247-scratch/price/drafts.py` from the base's spec (re-run after `hs` was added: every hash below
reproduced); rebuild them in your folder from this table's words rather than reading the
coordinator's. **They are drafts written to be priced and tested, never a ruling.**

| draft | the sentence | claude-legacy | cl100k_base | maximum | delta |
|---|---|---|---|---|---|
| x | (the base, no sentence) | 8340 | 8490 | 8490 | |
| hr | *"A function given to `signal` stops the program the first time a signal calls it: no code of the program runs inside a signal handler."* | 8369 | 8519 | 8519 | +29 |
| hrr | hr, its last clause continued: *"... inside a signal handler, so write the handler in C, in a header the program ships."* | 8383 | 8533 | 8533 | +43 |
| ha | *"A function is never given to `signal`, `sigset` or `bsd_signal`: the compiler refuses it, since no code of the program runs inside a signal handler."* | 8376 | 8526 | 8526 | +36 |
| he | *"A function given to `signal` runs after the signal, on a thread of the runtime's own, never inside it."* | 8365 | 8515 | 8515 | +25 |
| hs | *"A function given to `signal` runs after the signal, never inside it: on the thread the signal interrupted, once that thread is back in the program's own code."* | 8375 | 8525 | 8525 | +35 |

hr and hrr describe route C, ha route A, he route E (and F with E's delivery), hs route S (and F with
S's delivery); G, a door for other signals, would be a sentence or a card's line of its own, unpriced
here; D needs none. **The real delta of a draft is unmeasured**: it needs `heroes measure
--refresh`, an API call this brief does not name, so every verdict of yours on a draft is
`provisional` and every vendored delta a lower bound, said in those words; the real count, if one
decides your verdict, is owed to the coordinator, who asks for it. **For scale, never a measurement
of these drafts**: the whole spec's real-to-vendored ratio at the base is 11108 / 8490 (`python3 -I
-c 'print(round(11108/8490,4))'`: 1.3084); and the nearest measured marginal ratio of a § 13
sentence, panel 246's T1, which landed at +94 real and +73 vendored (`git -C
.claude/worktrees/b34-round diff ae5e2cedd HEAD -- 'docs/panel/246-*.md'`: *"**W3 held**, T1 priced
+94 real on `heroes measure spec/heroes-spec.md --refresh`, 11,014 to 11,108 on claude-opus-5, and
+73 vendored, 8417 to 8490"*), is 94 / 73 (`python3 -I -c 'print(round(94/73,4))'`: 1.2877).

## The questions

1. **Does § 13 owe a sentence for the route adopted?** The callback sentence today (lines 403-404)
   promises a function can be a parameter C calls, and the spec names no signal and no handler
   (`grep -c -i 'signal\|handler' spec/heroes-spec.md`: 0), while R4, ratified, says no Heroes code
   runs in a handler. Under A and C a program the spec allows is refused; under E, S and F what
   `signal` does changes. Which of those is a reader entitled to read in the spec, and which is the
   runtime's or the compiler's message alone (design.md §4.17: a diagnostic carries what is needed to
   fix the mistake it names; D's words, 00-shared.md, are such a message).
2. **Panel 246 kept a stop sentence out** (*"the spec-warden's veto stands, Principle 0's burden
   unmet, within its seat"*, `docs/panel/246-*.md:302`), and its veto of S1 to S3 compels *"no stop
   sentence in the specification and no stop door in `hero_os.h` or among the built-ins"* (351-355).
   Does the same burden fall on hr, hrr, ha, he and hs, or is a sentence that states R4 to a reader of
   another kind, a rule the reader must know so as not to write a program the runtime refuses or one
   that dies in silence (CLAUDE.md's first line: *every plausible LLM mistake is a compile error*)?
3. **What a reader of the base's spec plausibly writes**, run in your tree with D applied: a Heroes
   handler given to `signal` that sets nothing (Heroes has no mutable global: spec § 4, line 135,
   `grep -n -i 'no mutable globals' spec/heroes-spec.md`), one that calls C (`t3x1/t3x1.hero`'s
   `close`), one that prints, one that formats a number, and a handler written in C in a header the
   program ships; build each and run it once with one signal and once under a storm of **that
   handler's own signal**, with the shared driver, which takes the signal's name (`python3 -I
   storm.py <binary> SIGINT 4 0.3`, `p247-scratch/tools/storm.py` copied, run unwrapped; a program
   that took no handler for the signal sent dies at the first one, which measures nothing), and its
   crash reports read (`p247-scratch/tools/ips.py`). Which mistake each candidate sentence prevents
   and which error or silent death each program meets without it.
4. **hrr's remedy names a header the program ships**: panel 114's R2 sentence (*"The header may be
   one the program ships beside its own file ..."*) never entered the spec (`grep -n -i ships
   spec/heroes-spec.md` prints nothing at the base: run it); D's words give the same remedy, and panel
   246's critic measured its two-module trap (00-shared.md). Whether hrr leans on a rule the spec does
   not state, and what it would then owe.
5. **The payment**: for each draft you would admit, the named removal or the pre-registered
   falsifiable prediction with its instrument (the blind seat's reading, `llm-ergonomist.md`, is one
   if the author allows its run). For the one you would refuse, the program or fact that would make
   the refusal wrong (CLAUDE.md § 12).
