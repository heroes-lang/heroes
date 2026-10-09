# Panel 198, spec-warden

Copied by the coordinator at 08:24 on 2026-10-08 (`date`) from the seat's
reply, verbatim apart from this header: the harness refused the seat's Write of
its `report.md`, and it did not route around the refusal. Run 08:11:27 to
08:22:57 by the seat's own `date`, in its copy
`<scratchpad>/198-spec-warden/tree` of the frozen tree at `56def9b4`, with a
compiler built from that tree's seed; no paid run (`heroes measure` without
`--refresh`, `ANTHROPIC_API_KEY` unset). Its drafts and note texts stay in
`<scratchpad>/198-spec-warden/drafts/` and `notes/`, made by
`make_drafts.py` there.

---

The harness refused my Write of `report.md` (a subagent may not write report files; the critic's report records the same refusal), and I did not route around it with the shell. So the whole report is in this reply.

**Clock** (`date`): started 08:11:27; copy made 08:11:58 at `.../scratchpad/198-spec-warden/tree` (cp -R, `rm -f tree/.git`, `rm -rf tree/build`); compiler built from its seed at 08:13:32, exit 0; last reading 08:22:57. No paid run: `heroes measure` was run without `--refresh`, with `ANTHROPIC_API_KEY` unset.

## Verdict

- **verdict**: no route owes the spec a sentence. I **veto, provisionally**, restoring any sentence to § 13 (r1 or any draft below) while Principle 0's burden is unmet. On spec grounds I **approve at 0 spec tokens** the routes where each word is either passed or refused by Heroes' own message: A, B-ELF, B-asym, C, F, J. I **object** to B, D, E, G and H on what their note becomes.
- **section**: design.md §1.6 (the payment rule, and *every addition still carries §1.0's burden of proof*), §1.2, §1.0.
- **spec_token_delta**: 0 recommended. The frozen spec, re-measured at 08:14: legacy 7337, cl100k 7467, real 9831 (claude-opus-5, 2026-10-07, not stale). Headroom is 409, less the 60 mortgaged by the FFI floor, so **349 spendable**. Every draft below is a vendored lower bound. Even the largest (l2, at 1.5× its vendored delta) fits, so **there is no budget veto**.
- **removal**: nothing, and none is needed at a delta of 0. One problem if r1 is restored: `d0409cc5`'s body says R8 was *"Paid by r1 and by panel 192's registered predictions P7 and P1"*. Restoring r1 takes back a removal already spent on panel 192 R8's sentences, so it owes a payment of its own, and nothing on this sitting's table is offered as one.
- **needed_for_self_hosting**: no. `grep -rn -E '^extern .* package "' selfhost` gives 0.
- **argument**: r1's premise holds on every route where Heroes passes or refuses each word with its own message. The note fires whether or not the spec says anything, because the refused word comes from the machine's `.pc` file and no program text can avoid it. So a sentence adds about 15 to 33 real tokens to every prompt and removes no round-trip (§1.2): a reader who knows the rule still writes `package`, still gets refused, still repairs with `link`. The list itself went false in the spec within a day of entering it (`ad088ce5`: *"read under §12 the spec made `package "sdl2"` illegal"*). Under CLAUDE.md §12 a stale list in the spec makes the compiler refuse correct programs, which is the defect class this sitting was convened for. What needs work is the note, not the spec.
- **prediction**: two (P-tok and P-rw, below).
- **condition**: the veto lifts if the blind seat measures that readers with m2 in the spec reach a running program in fewer builds than readers without it (median at least 1 fewer, on the refused `package "sdl3"` build), or write `link` before the refusal. It also lifts if the panel accepts a Part 1 argument that rebuts the §1.4 test used in panel 188.

## What was found

1. **The list has been in the spec once, and it went false within a day.** Panel 050 wrote it in (`a48ddc1d`, 2026-08-14): *"Only `-I`, `-L`, `-l`, `-F` and `-framework` are accepted back"*. `ad088ce5` (2026-08-15) replaced it with r1's sentence, because panel 055 had already widened the compiler's list. r1 then removed that sentence on 2026-10-05. Panel 050's *"the list is named in the spec, because the list is the safety"* has stood unrevised for less than a day of its life.
2. **The note's reason clause is false, on its plain reading, of the word defect 444 refuses.** It says *"everything else is a flag a package file could use to run code during the build"*. The list's own test (`libraries.hero:11-13`) is *does it name a file, load anything, or write anything at build time?* Run at 08:21:
   - GNU ld's `--help` in `sdl3-b14`: `--enable-new-dtags` takes no argument.
   - `clang -### -pthread` there turns into `-lpthread` at the link, and `-l` is already accepted.

   That these words run no code at build time is an inference from what the drivers print, not a proof. If the coordinator reads it as *a false message*, it is a `blocking` candidate for the coordinator to class. A rewording that keeps the existing test needles (read, not run) costs **+6 on the note, 0 on the spec**: *"everything else is refused, because a word the list does not name could be one that runs code during the build (Go's CVE-2018-6574)"*.
3. **The note is now the rule's only statement, and nothing checks it against the code.** The comment at `:200-201` claims *"word for word"*. The tests (`:333-400`) exercise each accepted spelling and pin fragments of the note's text, but nothing compares the note's list with `filter_words`. design.md §1.6 has a rule that a figure may live in a second place only if it is checked or dated (panel 086), and the note is exactly such a second place. Whichever route widens the list owes an executor that compares the two.
4. **Precedent for route G:** the spec names no command-line flag, verb or environment variable (grep: 0 hits). `heroes build` already has `--include <path>` and `--library <path>`, and those live in `heroes help`.
5. **Claimants on the 349 spendable:** a grep finds 11 open issues that name spec work, among them defects 467 and 488 (`improvement`) and M-core-packages items. None is costed. There is no open `blocking` defect; 2 are `systemic`.

## Per route (spec cost and note cost, vendored maximum, lower bounds)

| route | verdict (my seat) | spec | note (124 today) | why |
|---|---|---|---|---|
| A | approve | 0 | +6 (reason rewording), +22 with the `--include`/`--library` repair | the premise holds; the repair must be run where the refusal fires |
| B | object | 0 | — | on macOS ld64's *"unknown options"* fires instead of Heroes' message (M1), stating neither the rule nor the repair; lld-link warns (M2) |
| B-ELF | approve | 0 | about +19 (not measured with both words) | the premise holds; the platform question belongs to the compiler-engineer |
| B-asym | approve | 0 | +19 | if admitted everywhere, a Mac `.pc` answering the word reaches ld64. None does: 0 of 499 Mac packages, 0 in the Debian 13 census |
| C | approve while the list fits in the note | 0 | grows per word | putting the list in the spec instead (l2, 18 words) costs **+127**, predicted +127 to +191 real: 36 to 55% of the 349 |
| D | object | d1 +20 to stay honest | — | a message on a build that succeeds counts as a warning on a correct program; libpsx changes meaning silently; no sentence repairs either |
| E | object | 0 | the note can no longer list what is accepted | it becomes a shape test; design.md:2640-2642 says the list holds *because it names what is permitted* |
| F | approve, spec silent | 0 | none fires | F makes r1, m1, m2 and l1 false; the only true sentence is f1 (+36), linker detail no reader can act on |
| G | object | 0 (g1 would be +30, the spec's first command-line fact) | +21 | either the note names the allowance, and a model's handed repair becomes widening the security list, or it doesn't, and part of the rule is stated nowhere a reader meets it |
| H | object as Go's list wholesale | 0 | the note cannot hold it | a short filtered list is C |
| J | approve | 0 | 0 (it is today's repair) | unrun on the source-build machine; its run does `sudo ldconfig` (`ci-step2.sh:20`), so the run-time half is plausible |

## Principle 0

The compiler needs no route. On thesis effect, no measurement exists. The only Part 1 argument on the record (panel 188's warden's §1.4 test, used by panel 192 for r1) points against a sentence. The blind seat's readings are the measurement, so the thesis half of this ruling is provisional until they report. Principle 0 does not bar a change to the list itself: a list word is not a form of the language and costs no spec token.

## Predictions

- **P-tok:** if any sentence lands, its claude-opus-5 delta falls between 1.0 and 1.5 times its vendored delta: r1 +22 to +33, m1 +21 to +32, m2 +15 to +23, d1 +20 to +30, g1 +30 to +45, f1 +36 to +54, l1 +61 to +92, l2 +127 to +191. This is calibrated on ledger rows read today: 1.32, 1.16, 1.39, 1.00 and 1.23. The instrument is `--refresh`, scored at the landing commit.
- **P-rw:** on the blind seat's arms, readers with m2 need the same median number of builds to a running program as readers without it, and none writes `link` before the refusal. It is falsified by at least 1 fewer build in the median.

## Drafts for the blind seat

All are written whole at `.../198-spec-warden/drafts/spec-<name>.md` (script `.../198-spec-warden/make_drafts.py`). Each replaces the end of line 446, *"…and what else it needs."*. Deltas are the vendored maximum, measured 08:18:03 to 08:18:47:

- **r1, +22**: `…what else it needs. A package answering with anything this compiler does not pass on is refused, naming what it said.`
- **m1, +21**: `…what else it needs, and is refused, naming the word, when the answer holds one this compiler does not pass on.`
- **m2, +15** (the one I would admit, if the condition above is met): `…what else it needs; a word of the answer this compiler does not pass on is refused.`
- **f1, +36** (true under F only): `…A package answering with anything this compiler does not pass on is refused, naming what it said, but the words choosing an rpath's tag, which it drops.`
- **d1, +20** (true under D only): `…what else it needs, and leaves out, naming it, any word of the answer this compiler does not pass on.`
- **g1, +30**: `…is refused, naming what it said, unless the person building admits that word.`
- **l1, +61** (true only for today's list): `Only `-D`, `-U`, `-I`, `-L`, `-l`, `-F`, `-framework`, `-Wl,-framework,` and `-Wl,-rpath,` are passed on; a package answering with anything else is refused, naming what it said.`
- **l2, +127**: l1 widened by `-isystem`, `-pthread`, `-Wl,--enable-new-dtags`, `-Wl,--as-needed`, `-Wl,--no-as-needed`, `-Wl,--export-dynamic`, `-Wl,-z,relro`, `-Wl,-z,now`.

r1, m1 and m2 are true under A, B, B-ELF, B-asym, C, E, G, H and J (under G an admitted word is passed on), and false under D and F.

The note drafts are at `.../198-spec-warden/notes/n-*.txt`. `--allow-package-word` in `n-G.txt` is a placeholder name, not a proposal.

## The paid run

My verdict needs **zero** `--refresh` calls. If the sitting adopts a sentence anyway, **one** call at the landing, on the adopted draft. Ranking r1, m1 and m2 on the real scale would take **three** (apply, refresh, revert each) to separate drafts 7 vendored tokens apart; I advise against it, because the choice between them is not a token question.

## What I could not run

- Any real count (`--refresh`, which the brief forbids).
- The compiler's own tests with the reworded note: I read the needles, I did not run them.
- Route J's build and run on the source-build machine.
- The blind seat's readings, on which the thesis half of my Principle 0 ruling waits.
- A check that the note's list equals `filter_words`: none exists, and I did not write one.
- `report.md`, refused by the harness.
