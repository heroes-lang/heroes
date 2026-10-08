# Panel 199, spec-warden

Copied by the coordinator at 10:04 on 2026-10-08 (`date`) from the seat's own file
`<scratchpad>/199-spec-warden/report.md`, unchanged below the rule: the seat's
running notes, written as it went, with its verdict at the end. The seat was
stopped once by the session limit at about 08:25 and resumed at 09:50 on a new
account. It priced every draft on the `maximum` row, a lower bound, and ran no
`measure --refresh` (the coordinator did not confirm it); its drafts, probes and
`scripts/draft.sh` stay in `<scratchpad>/199-spec-warden/`. Its reply's verdict:
**object**, no veto; budget and Principle 0 held apart; the one sentence it
would admit, conditional on a syntactic rule (A) that refuses `hidden2`, is
merged into the spec's `:280`.

---

# Panel 199, spec-warden, running notes

Copy: `tree/` here, rsync of the frozen `lane-panel-199` at `56def9b4`
(`.claude/worktrees` excluded), `build/` removed, compiler from its seed with
Apple clang (seed equal to the frozen tree's by `cmp`). Started 08:09 CEST
2026-10-08 by `date`. No paid run: the coordinator did NOT confirm
`measure --refresh`, so every draft below is priced on the `maximum` row, a
LOWER BOUND, its real count unrun and owed at the landing.

## 1. The baseline, re-measured (08:10:57 by `date`)

`./heroes measure spec/heroes-spec.md`, exit 0: claude-legacy 7337,
cl100k_base 7467, maximum 7467, spread 130, real 9831 (pinned claude-opus-5,
2026-10-07). Headroom 409 against 10240, 60 mortgaged to the FFI floor, so
9891 is what the ceiling judges: 349 free. **Agrees with the brief to the
token.** design.md §1.6 (`docs/design.md:255`) re-read by grep: 10240,
`claude-opus-5` through `POST /v1/messages/count_tokens`; payment rule
unconditional (`:311-314`), the two payments held to one standard (`:316-327`).

## 2. Probes, my copy, 08:13-08:16 by `date` (`probes/`, every binary under `timeout 10`, output cut by `head -c 4096`, `pgrep -fl` on the directory after: none)

| probe | `check` | build `-O0` | run `-O0` | run `-O2` |
|---|---|---|---|---|
| `deep.hero`: `count_to(n:, acc:)`, a CORRECT tail recursion, 10,000,000 deep | 0 | 0, no warning | `panic: stack exhausted in deep.count_to`, 134 | prints `10000000`, 0 |
| `hidden2.hero`: `serve` calls `stop_after(n)` (which holds `exit(code: 0)` when `n > 3`), prints, returns `serve(next(n))`: a CORRECT program, its end in a callee | 0 | 0, **clang warns `hidden2.hero:8:39` *all paths through this function will call itself*, false** | 1, 2, 3, exit 0 | the same |
| `hidden.hero`: the same with `serve(n + 1)` inline | 0 | 0, no warning (the inline overflow branch is a `_Noreturn` path, p2's effect) | 1, 2, 3, 0 | the same |
| `dieret.hero`: `f(n) -> i64` printing then calling `die()`, which is `exit(code: 3)` | **1, `error[missing_return]`** at `dieret.hero:4:23` | | | |

Three findings for the spec:

1. **`:280` is level-dependent in BOTH directions, not only for the mistake.**
   `deep` is a correct program: `heroes build` (default `-O0`) aborts it,
   `heroes build -O2` (what `heroes run` builds, `verbs.hero:61`, read not
   run) runs it. So today one program has two meanings by flag. design.md
   Part 3 (`docs/design.md:710`) already refuses that in kind: *a missing
   return after an exhaustive `match` must not become whatever `-O2` feels
   like*.
2. **Defect 507 has a shape beside it that route (F) does not repair.**
   `hidden2`: the program's end is `exit(code:)` inside a callee that is not
   `_Noreturn` and cannot be (it returns when `n <= 3`). Marking
   `h_library_exit` `_Noreturn` changes nothing in `serve`'s CFG, so clang's
   warning stays false on a correct program after (F) lands. (C) and (K) are
   therefore not made truthful by (F) alone (an inference from the C's shape;
   (F) is not built, so unrun).
3. **The spec's path-end rule is already syntactic and already refuses a
   correct program whose end is hidden in a callee** (`dieret`: `missing_return`
   on a function that can never fall off its end). So route (A) worded on the
   three written ends of `:238-240` is the exact dual of a rule the language
   already states, with the same accepted cost: `hidden2` would be refused,
   a correct program, rewritten as `while true` (the idiomatic shape) or with
   the `exit` written in `serve`. That is a correct program refused, which
   the class list calls `blocking` unless it is the LANGUAGE's rule, written
   where a reader can see it. **This is what decides question 4 below.**

## 3. The level, by hand (08:19-08:22 CEST, before the session limit)

The six emitted C files (`heroes build --emit-c`) compiled by hand with the
build's own `flags()` less its five `-Werror=` additions, at `-O2`, with and
without `-fno-optimize-sibling-calls`, each binary under `timeout 10` through
`head -c 4096`, `pgrep` after: none. Apple clang, one version only.

| shape | `-O2` plain | `-O2` + `-fno-optimize-sibling-calls` |
|---|---|---|
| `forever` | 124, nothing | 134, `stack exhausted in forever.forever` |
| `ping`/`pong` | 124 | 134, `... in pingpong.ping` |
| function value `go` | 124 | 134, `... in fnvalue.go` |
| generic `same<T>` | 124 | 134, `... in gen.same_1b9a87` |
| p3 `count(xs)` | 124 | 134, `... in p3.count` |
| `deep`, a CORRECT 10,000,000-deep tail recursion | 0, prints 10000000 | 134, `stack exhausted in deep.count_to` (what `-O0` does) |

So route (G) makes `:280` true for every shape at `-O2` and costs exactly the
program `deep`: a correct tail recursion that `heroes run` finishes today and
that `heroes build` (`-O0`) aborts today. The language then has ONE meaning at
both levels, which design.md `:710` (*must not become whatever `-O2` feels
like*) and `:724` (*one `.o` is correct across `-O0`, `-O2`*) ask of it. The
spec names no flag (`grep -c -e '-O[0-9]' spec/heroes-spec.md`, 0), so a spec
sentence that makes `:280` true by naming a level has no word to name it with:
the best draft (D4b below) must invent `-O2` for the reader.

## 4. Drafts, priced on the `maximum` row (offline, a LOWER BOUND; the real count of every one is UNRUN)

`scripts/draft.sh` replaces one anchored line of a copy of `spec/heroes-spec.md`
and prints `heroes measure` of the copy; base = `maximum 7467` (claude-legacy
7337). Re-run by anyone: `drafts/<name>.md` is each draft whole.

| draft | where | text | maximum | delta |
|---|---|---|---|---|
| D1c | `:280` | `- Recursion too deep aborts; a function calling itself on every path is a compile error.` | 7479 | +12 |
| D1r | `:280` | `- Recursion too deep aborts, and a function whose every path reaches a call of itself is a compile error.` | 7483 | +16 |
| D1rw | `:280` | D1r wrapped at 80 columns (two lines) | 7485 | +18 |
| D1rnw | `:280` | `... a call of\n  itself by name is a compile error.` | 7487 | +20 |
| D1rIw | `:280` | `... a call of itself, directly or through other functions, is a compile error.` (route I) | 7492 | +25 |
| D1a | `:280` | `... a function that calls itself on every path, before the path returns or ends, is a compile error.` | 7489 | +22 |
| D2 | `:240` | appended after `... end a path.` : `A function whose every path reaches a call of itself is a compile error.` | 7484 | +17 |
| D3b | `:267` | (L): `... (UFCS), so inside `f`, `x.f()` is `f` itself.` | 7483 | +16 |
| D3a | `:267` | `(UFCS), inside `f` too.` | 7473 | +6 |
| D4b | `:280` | (G by spec): `... aborts; built with `-O2`, a call in tail position may run for ever instead.` | 7484 | +17 |
| D4a | `:280` | `... aborts, except at `-O2`, where a call in tail position may run for ever instead.` | 7485 | +18 |
| D1rG | `:280` | `- Recursion too deep aborts, at every optimisation level.` (only true if (G) lands) | 7473 | +6 |

Against the ceiling: the binding number is the real, 9831 + 60 (FFI floor) =
9891 of 10240, 349 free. Every draft above costs at most 25 on the maximum row.
The ratio real/maximum on this document is 9831 / 7467 = 1.317, so IF it held
for a 16-token sentence the real cost would be about 21 (an INFERENCE, the
ratio is a property of the whole document and not of a clause); even at 2x the
largest draft is 50, inside 349. **No budget veto at any draft.**

## 5. The six questions (resumed 09:50 CEST after the session limit and the Mac's sleep)

**1. Does the rule need a spec sentence?** Principle 0 has two doors and
neither is open for the DIAGNOSTIC: (a) compiler need: no. The compiler
compiles itself with no such function (the critic's textual census, carried and
not re-run: 2 of 10,590 top-level functions, both deliberate probes of panel
173); `needed_for_self_hosting: no`. (b) a measured Part 11 effect: none yet;
`docs/metrics/operators.md` / `selfhost/mutate/ops.hero` holds sixteen operators
and none imitates *a function whose every path calls itself*; defect 455's
program was run by the coordinator, "first seen by lane b14-runtime" as a clang
warning, so whether any model wrote it is unrun (the card's own words). The
corpus census cannot supply the rate either: the shape aborts at the first run
at `-O0`, so it is fixed before it is committed (survivorship, an inference).
design.md has no sentence on a function that always recurses (grep `recurs`,
`infinite`, `terminat` over docs/design.md: nothing on this shape).

**The one door that opens a sentence is §12 / the spec being the prompt**: if
the rule is (A) read by the three written path ends of `:238-240`, it refuses a
program that RUNS: `hidden2.hero` (probes/): `serve` ends the program through
`stop_after`'s `exit(code: 0)`, prints 1, 2, 3 at exit 0 at both levels, and
`check` exits 0 today. `missing_return` already refuses the dual (`dieret.hero`,
`check` 1: a function that can never fall off its end because `die()` exits),
and states why at `:238-240`; so a refusal of a running program is NOT new, and
its vocabulary (*ends a path*) is already in the document. What is new is the
boundary of THIS refusal, which only the compiler would hold. One merged clause
at `:280` gives it (D1r, +16).

**The precedent that says no sentence** (question 4): `grep -c 'error\['
spec/heroes-spec.md` 0 and `grep -c polymorphic` 0, re-run in my copy, and
panel 029 R4 (`docs/panel/029-generics-by-monomorphisation.md:110-135`) took
that route on purpose: *refused by the pass, zero spec tokens*. **But its
refusal loses no running program** (a growing instantiation does not build);
(A) loses `hidden2`. That is the whole difference, and it makes the sentence
conditional on what the sitting does with `hidden2`, not unconditional.

**2. `:280` at `-O2`.** Section 3 above. The language makes the sentence true
(G): 0 spec tokens, `-fno-optimize-sibling-calls`, one flag in
`selfhost/cli/flags.hero`'s list; the spec naming the level costs +17 (D4b) AND
needs a word the document never uses, AND leaves two meanings for one program,
which design.md `:710` and `:724` refuse. What (G) costs is `deep`, measured.
**Not run: whether the compiler built with the flag still compiles itself** (a
cold `-O2` self-build under a load average of 65; started at 09:53 in
`treeg/`, status in `treeg/g.status`; see section 7). **Route (N), unlisted**:
guarantee tail calls at every level, so that `forever` loops BY DEFINITION and
`:280` is rewritten to except a call in tail position (about +10); then no abort
witnesses the shape ever and the checker rule is the ONLY witness. Not mine to
choose; I name it so the sitting sees what (G) forecloses.

**3. Is the shape a mistake an LLM plausibly makes?** The spec already tells the
reader the three facts that make it derivable: `:267` (*sugar for `f(x, y)`*,
*There are no methods*), `:309-314` (the closed `Built-ins:` list, `bytes` and
`count` not among them), `:113` (no forward declarations, so a function may
name itself). It does not tell the consequence, that inside `function f` the
call `x.f()` is `f`. The UFCS shape (p1, p3) needs a model to hold two beliefs
at once: *I must define `bytes`* and *`bytes` is a method*; weakly plausible.
**The plausible mistake is p2, the missing base case**, and it is the one the
UFCS framing hides: clang is silent on it, and (A) refuses it only because a
hidden overflow abort is not a written path end (`:240`; spec-shape.md:57-60,
panel 087: no list of aborts). So the Principle 0 case for the rule rests on p2,
not on `bytes`.

**4. Does a diagnostic need a spec row?** No by the precedent, and the
precedent holds for every rule that refuses nothing that runs. See question 1.

**5. Warnings.** `grep -n -i warning spec/heroes-spec.md`: no line; nothing in
the document implies a warning class. The nearest, `:341-343` (`???` *reports
what belongs there*), is a refusal to build ("produces no binary"), not a
warning. design.md `:3725` rules a warning level out. **Beside it, measured**:
today's build prints clang's *warning: all paths through this function will
call itself* in C words, so the language ships a warning in fact (defect 457's
status quo, route J) against its written ruling; and it is FALSE on `hidden2`
even after (F) marks `exit` `_Noreturn`, because the program's end is inside a
callee that returns when `n <= 3` (an inference from the C, (F) unbuilt).
Routes (C) and (K) are therefore not made truthful by (F) alone. A warning
class would cost a sentence defining it (not drafted; I would veto it without
a ruling of its own against `:3725`).

**6. The sentence for variant B: unchanged.** Reason: one change per variant.
B carries the diagnostic alone; a B that also carries D1r cannot say which of
the two repaired the program, and the budget (7 sessions, 5.95 USD) has no
cell for a third variant. If the sitting adopts D1r it is priced here and its
effect on a reader is unmeasured by this instrument.

## 6. §1.2 and the payment (design.md `:193-204` and `:311-327`)

Real cost = tokens x (1 + rewrite rate). D1r costs 16 on the maximum row; about
21 on the real IF the document's ratio 1.317 held (inference). One correction
round trip is 500-2000 tokens (design.md `:197`). The sentence pays for itself
when it prevents the mistake in at least 21/2000 = 1.05% to 21/500 = 4.2% of
the sessions that read it. The rate it is asked to beat is unmeasured: 0
tracked occurrences (survivorship-biased, above), one program authored by a
coordinator, no model run. The side that would be measured, the blind seat, has
one reading per task and variant: it can say a diagnostic repairs a program,
not a rate of 1% or 4%. So the benefit side is zero data and the cost side is
measured; that is the strongest reason the sentence is wrong TODAY, and it is
why my condition is `hidden2`, not taste.

**Payment** (panel 012/046, unconditional at every level): no removal named, on
purpose. A removal is a spec change that carries its own §1.0 burden and I will
not manufacture one to pay for a sentence I do not recommend unconditionally.
The payment I offer is a registered prediction, scorable by an instrument that
exists today (a compile, a diagnostic transcript, `heroes measure`):

- **P-A** (scored at this sitting's landing, by `heroes measure spec/heroes-spec.md --refresh`
  once the coordinator confirms it, and by `git diff --stat spec/`): D1r merged
  at `:280` costs 16 on the maximum row (repeatable now: `drafts/D1r.md`, 7483)
  and between 18 and 26 on the real count; above 30 the draft is re-argued.
- **P-B** (scored from the blind readings of panel 199, B variants of p1 and
  p3, spec unchanged, which are diagnostic transcripts): both are repaired to
  the key (`check` 0, both levels print the key's line at exit 0), and the
  `confidence` heading names the diagnostic's own text as what told the reader
  what to change. If either fails, the sentence's case opens at once.

## 7. Which wording, and what is still unrun (10:02 CEST)

**The sentence I would admit, and only under the condition of section 5 q1**
(rule (A) by the written path ends, refusing `hidden2`): D1rnw, merged into
`:280`, exactly

```
- Recursion too deep aborts, and a function whose every path reaches a call of
  itself by name is a compile error.
```

**+20 on the maximum row** (7487 against 7467; claude-legacy 7357 against 7337;
`drafts/D1rnw.md`), a lower bound, the real count unrun. D1r (+16) drops *by
name* and then claims `f = go; return f(n)` too, which a name-reading checker
does not refuse: *spec beats compiler*, so the 4 tokens buy the sentence being
true (CLAUDE.md § 12). It costs no code span, so none of the spec suite's
`named`, `rejected`, `inventory`, `offered` or `shape` checks can move (an
inference from what they read; the suite is unrun on the draft, the machine
being at load 60 and the harness build not started). `budget`, `recorded` and
`ledger` move by design and wait for the real count.

If the sitting narrows (A) so that it refuses nothing that runs (for instance
by exempting a body that calls another user function), the sentence is not
owed and the cost is 0, by the polymorphic_recursion precedent.

**Unrun, in full**: (1) `measure --refresh`, by the coordinator's instruction:
every draft's real count; (2) the compiler built with `-fno-optimize-sibling-calls`
compiling itself (`treeg/`, started 09:53 under a load average above 60: its
status file `treeg/g.status` is the answer, or the absence of one); (3) the
`spec` suite on any draft; (4) the flag on any clang but Apple clang, and
through `heroes build` itself (I compiled the emitted C by hand with the
build's flags less five `-Werror=`s); (5) whether any model ever wrote the
shape; (6) the census of tracked files (critic's textual number carried).

**Stopped at 10:04 by `date`**: the `treeg/` self-build was still on its FIRST
step (a cold `-O0` build of the compiler) after 10 minutes at a load average
above 60, and two `-O2` builds would follow; I stopped it and its processes
(`pgrep` after). So question (2) above is unrun, not negative.
