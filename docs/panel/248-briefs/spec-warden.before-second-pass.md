# Panel 248, spec-warden

Read `00-shared.md` first, whole: the five questions, **why the sitting is the full panel** (spec § 13's
line 404 names the stop doors; a join that does not wait moves line 377; route I may widen
`ffi_defined_elsewhere`), the rules (**your binaries are named `sw-...`**, every program that may not end
has its bounds there) and the platforms. You judge what the specification must and must not say for
each route, and Principle 0's burden for any sentence, with a veto on budget breach and on an unmet
burden. Your folder is `.claude/worktrees/p248-scratch/248-spec-warden/`, your tree `tree/` in it, a `git
archive` of **`df7612ad3`** (tree `f72f48da250e919a68ebc6a98be0facccc29a052`), your compiler built
there from the seed in two stages (`sources 4c9a6a9800095eef`, 00-shared.md § The base). Your report is
`docs/panel/248-reports/spec-warden.md` in `.claude/worktrees/p248`, in your definition's output
structure, written as you go. Reach design.md by grep before you cite it: `grep -n '^### 1.0'
docs/design.md` 112 (Principle zero), `'^### 1.2'` 190 (the cost formula), `'^### 1.6'` 253 (the spec
budget); `grep -n 'payment' docs/design.md`: 282, 301, 324, 328, 330 and 4053.

## The count before, measured at the base

`./heroes-next measure spec/heroes-spec.md`, run by the coordinator from
`.claude/worktrees/p248-scratch/briefs/base/tree` (a `git archive` of the base, its
`spec/heroes-spec.md` the worktree's byte for byte: `shasum -a 256` of both,
`163219f2fa69fe82...`; 492 lines and 29,187 bytes, `wc -l -c`), exit 0
(`briefs/base/measure-spec.txt`):

| row | count |
|---|---|
| claude-legacy | 8470 |
| cl100k_base | 8621 |
| maximum (vendored) | 8621, *"a lower bound, not the reader's tokeniser"* |
| spread | 151 between the two vendored tables |
| real | **11285**, *"claude-opus-5, 2026-11-13 — the binding number"*, the instrument's pin, no `STALE` |
| headroom | 5099 against 16384; *"the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is 11345"* |

The ceiling is 16384 (`tests/harness/suite_spec.hero:162-163`), and `DELTA_GATE` 50 (`:217-218`).
Run it again in your own tree before you use it; a different figure is a finding. The base's spec is
panel 247's R5 and R6 as landed: its hash is the draft panel 247 priced as `spec-m2g3.md`,
`163219f2fa69fe82` (`docs/panel/247-*.md:231-232`).

**The real delta of any draft is unmeasured, and stays so in this sitting.** It needs `heroes measure
--refresh`, a request to `https://api.anthropic.com/v1/messages/count_tokens`
(`selfhost/cli/refresh.hero:37`) with the trunk's `ANTHROPIC_API_KEY`: **an API call, which this
project's rules count among paid runs** (`.claude/skills/panel/SKILL.md:292-293`), named in no brief of
this sitting, so not run, by you or by the coordinator (who loaded no key for these briefs). Every
verdict of yours on a draft is `provisional` and every vendored delta a lower bound, said in those
words; a real count that would decide your verdict is owed to the coordinator, who asks the author.
**For scale, never a measurement of these drafts**: panel 247's hg3 and hm6 together, 8493 to 8621
vendored (+128) over the base it priced (`247-*.md:231-232`), and the real count went from 11108 (247's
brief) to 11285 (the pin above), +177 over the same landing and R6's `-> i32`; a ratio of those is an
inference about one landing, not a rule.

## The candidates, to be priced by you

Drafts the coordinator wrote to be priced and tested, **never a ruling**, each spelled with the door
the seats were handed (`compiler-engineer.md`, `llm-ergonomist.md`); a seat that builds another
spelling changes the draft, and you price the built one. Rebuild each in your folder from these
words, each anchor asserted to occur exactly once in the base's spec (the coordinator's `grep -c` of
each anchor printed 1):

| draft | question | where | the change |
|---|---|---|---|
| x | | | the base, no change |
| q4m | 4 | line 404, merged | *"Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64` answer 1"* becomes *"Ctrl-C, SIGTERM and `hero_net_ask_stop() -> i64` only make `hero_net_stop_asked() -> i64` answer 1"* |
| q4a | 4 | line 404, appended | after *"gives the count since its last call"* (the line ends there, no full stop): *"; `hero_net_ask_stop() -> i64` asks the same stop from any thread"* |
| q5d | 5 | line 377, appended | after *"and `hero_thread_join(handle: i64) -> i64` waits for its answer."*: *" A socket's waits end at the limit `hero_net_set_deadline(s, ms: i64) -> i64` (`hero_net.h`) last gave, failing timed out."* |
| q5j | 5 | line 377, merged | *"and `hero_thread_join(handle: i64) -> i64` waits for its answer."* becomes *"and `hero_thread_join(handle: i64) -> i64` waits for its answer, which `hero_thread_ready(handle: i64) -> i64` says is there without waiting."* |
| q3a | 3 | line 377, appended | after the same sentence: *" A child starts in the folder `hero_run_dir(path: str)` stages for it, else in the program's."* |

**And the none for each, which is a candidate too**: questions 1 and 2 add nothing a reader writes
(line 403's *"never a result or a signal handler, nor called by one"*, panel 247's hm6, states the
rule route I or its alternatives enforce; question 2 changes a promise of `hero_net.h`); question 3's
door sits beside `hero_run_env`, which panel 242's R6 admitted at 0 tokens because line 377 already
says *"processes ... from the language's `hero_os.h`"* (`docs/panel/242-*.md:176-180`); question 4's
door could stand in `hero_net.h` and the card, as panel 246's R4 kept the stop's documentation at 0 spec
tokens (`246-*.md:351-355`), **if line 404 stays true with it**; question 5's deadline the same.

## The questions

1. **Does line 404 stay true with a door the program calls to ask the stop?** Its *"Ctrl-C and SIGTERM
   only make `hero_net_stop_asked() -> i64` answer 1"*: is *"only"* read as the whole list of what makes
   it answer 1, or as what those two signals do and nothing more? If the first, q4z (no sentence) leaves
   the spec false the day the door lands, and a sentence is owed; price q4m and q4a, and say which you
   would admit.
2. **Does line 377 owe a sentence for a join that does not wait (q5j), a deadline (q5d), or a child's
   folder (q3a)?** Panel 242's R6 is the precedent for a door the line already covers; say whether each
   is that, or a promise a reader must be told.
3. **Does a widened `ffi_defined_elsewhere` owe the spec anything?** `grep -n -i -E 'library the program
   names|another library|two libraries|supplied' spec/heroes-spec.md` printed nothing of the kind (its
   one hit, line 5, is the grammar's *"`=` defines"*): the thirteenth member of
   `.claude/rules/c-boundary.md` stands in no spec sentence today. If route I makes the runtime define
   `bsd_signal`, `sigset` or Darwin's undeclared installers, a program whose library defines one is
   refused where it built: say whether that is a diagnostic's business alone (design.md §4.17) or a
   sentence's.
4. **What a reader of today's spec and card plausibly writes**, built and run in your tree (bounded as
   00-shared.md's table says, every binary `sw-...`): for question 4, a server that ends on `GET /quit`
   with what exists today (its handler sending its own process SIGTERM through a C binding of `kill` and
   `getpid` after `net.catch_stop()`, or another shape you find), and whether it ends at exit 0 with the
   leak check clean; for question 5, a server that tries to close a connection that sends nothing with
   what exists today. Which mistake each draft or face prevents, and which error or silent hang each
   program meets without it: that is the measured need Principle 0 asks (design.md §1.0).
5. **The payment**: for each draft you would admit, the named removal or the pre-registered falsifiable
   prediction with its instrument (the blind seat's reading, `llm-ergonomist.md`, is one if the author
   allows its run: its tasks t4 and t5 measure the card's faces, not these sentences, and a spec arm
   would be a registration of its own, before any session). For the one you would refuse, the program
   or fact that would make the refusal wrong (CLAUDE.md § 12).
