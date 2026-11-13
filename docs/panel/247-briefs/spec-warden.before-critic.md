# Panel 247, spec-warden

Read `00-shared.md` first, whole. You judge what the specification must and must not say for each
route, and Principle 0's burden for any sentence. Your folder is
`.claude/worktrees/p247-scratch/247-spec-warden/`, your tree `tree/` in it, a `git archive` of
**`ae5e2cedd`** (tree `3e22d53467b606946a70a6f9dfdea6f46ea47612`), your compiler built there from the
seed in two stages (`sources 7cc70d136b590d46`). Your report is `docs/panel/247-reports/spec-warden.md`
in `.claude/worktrees/p247`, in your definition's output structure, written as you go. Reach design.md
by grep before you cite it (`grep -n '^### 1.6' docs/design.md`: 253; `grep -n 'payment'
docs/design.md`: 282, 301, 324, 328, 330 and 4053; Principle 0, `grep -n '^### 1.0' docs/design.md`:
112).

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
use it; a different figure is a finding.

## The candidates, priced on the vendored tables at the base

Each is the base's spec with the sentence appended, one leading space before it, after the one
occurrence of `` `atexit(f: (function() -> ()))`. `` (spec line 404, § 13's callback sentence). The
coordinator's drafts are `.claude/worktrees/p247-scratch/price/spec-<name>.md`, made by
`p247-scratch/price/drafts.py` from the base's spec; rebuild them in your folder from this table's
words rather than reading the coordinator's. **They are drafts written to be priced and tested, never
a ruling.**

| draft | the sentence | claude-legacy | cl100k_base | maximum | delta |
|---|---|---|---|---|---|
| x | (the base, no sentence) | 8340 | 8490 | 8490 | |
| hr | *"A function given to `signal` stops the program the first time a signal calls it: no code of the program runs inside a signal handler."* | 8369 | 8519 | 8519 | +29 |
| hrr | hr, its last clause continued: *"... inside a signal handler, so write the handler in C, in a header the program ships."* | 8383 | 8533 | 8533 | +43 |
| ha | *"A function is never given to `signal`, `sigset` or `bsd_signal`: the compiler refuses it, since no code of the program runs inside a signal handler."* | 8376 | 8526 | 8526 | +36 |
| he | *"A function given to `signal` runs after the signal, on a thread of the runtime's own, never inside it."* | 8365 | 8515 | 8515 | +25 |

hr and hrr describe route C, ha route A, he routes E and F; route D alone needs no sentence (it stops a
program that was already corrupting itself). **The real delta of a draft is unmeasured**: it needs
`heroes measure --refresh`, an API call this brief does not name, so every verdict of yours on a draft
is `provisional` and every vendored delta a lower bound, said in those words; the real count, if one
decides your verdict, is owed to the coordinator, who asks for it. For scale, never a measurement of
these drafts: the whole spec's real-to-vendored ratio at the base is 11108 / 8490 (`python3 -I -c
'print(round(11108/8490,4))'`: 1.3084).

## The questions

1. **Does § 13 owe a sentence at all**, for each route? The callback sentence today (lines 403-404)
   promises a function can be a parameter C calls, and the spec names no signal and no handler
   (`grep -c -i 'signal\|handler' spec/heroes-spec.md`: 0). Under C and A a program the spec allows
   is refused; under E and F what `signal` does changes. Which of those is a reader entitled to read in
   the spec, and which is the runtime's or the compiler's message alone (design.md §4.17: a
   diagnostic carries what is needed to fix the mistake it names).
2. **Panel 246's R4 kept a stop sentence out** (*"the spec-warden's veto stands, Principle 0's burden
   unmet, within its seat"*, `docs/panel/246-*.md:302`). Does the same burden fall on hr, hrr, ha
   and he, or is a refusal sentence of another kind, a rule a reader must know so as not to write a
   corrupting program (CLAUDE.md's first line: *every plausible LLM mistake is a compile error*)?
3. **What a reader of the base's spec plausibly writes**, run in your tree: a Heroes handler given
   to `signal` that sets nothing (Heroes has no mutable global: spec § 4, line 135, `grep -n -i 'no mutable globals'
   spec/heroes-spec.md`), one that calls C
   (`t3x1/t3x1.hero`'s `close`), one that prints, and a handler written in C in a header the program
   ships; build each and run it once with one SIGINT and once under the lane's storm
   (`b34-scratch/b34-signals/r2494/storm.py`, copied). Which mistake each candidate sentence prevents
   and which error each program meets without it.
4. **hrr's remedy names a header the program ships**: panel 114's R2 sentence (*"The header may be
   one the program ships beside its own file ..."*) never entered the spec (`grep -n -i ships
   spec/heroes-spec.md` prints nothing at the base: run it). Whether hrr leans on a rule the spec
   does not state, and what it would then owe.
5. **The payment**: for each draft you would admit, the named removal or the pre-registered
   falsifiable prediction with its instrument (the blind seat's reading, `llm-ergonomist.md`, is one
   if the author allows its run). For the one you would refuse, the program or fact that would make
   the refusal wrong (CLAUDE.md § 12).
