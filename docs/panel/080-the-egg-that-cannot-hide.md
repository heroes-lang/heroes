# Panel 080 — the egg that cannot hide

**Date**: 2026-08-16 · **Session type**: retro-record (author instruction
2026-08-16, *"inseriscila nel linguaggio allora"* — the verdict was given before
the sitting; the sitting records real objections and real measurements, and
stages no dissent) · **Lane**: reduced, and the deviation is recorded here
rather than smoothed over. Neither named lane fits a tool-surface easter egg:
the soundness lane's second seat (ffi-pragmatist) had no C to compile, and the
full panel's other three seats had empty briefs — no spec token (delta measured
0 by the coordinator: `spec/heroes-spec.md` is untouched), no `.hero` surface
for the llm-ergonomist to A/B, no count for the spec-warden to judge. The two
seats with falsifiable content sat: **compiler-engineer** (cost, measured in
the tree) and **historian** (precedent, verified with sources). Panel 037's
lesson — the instrument was right and the gear was wrong — is why this sitting
is two seats and not five.

## Proposal (verbatim)

Add a `heroes this` subcommand: an easter egg printing "The Zen of Heroes"
(twenty ASCII laws, a David Bowie tribute, in the tradition of Python's
`import this`) to stdout, exit 0. No `.hero` surface, no diagnostic, no spec
token, no flags, no operand. Tension: §10's stopping rule (panel 016) admits a
capability only if a harness must type it or it has a measured Part 11 effect —
an easter egg has neither. Author instruction 2026-08-16 overrides for this one
verb. Bowie rule (CLAUDE.md §11): prose and packaging yes, error text and
library names never — the stdout artifact of a dedicated subcommand is
packaging.

## Verdict table

| Judge | Verdict | Rests on | Cost / delta | Prediction | Condition |
|---|---|---|---|---|---|
| compiler-engineer | approve-with-conditions (no veto: not a language form, §1.1 ceiling untouched) | design.md §1.1, Principle 0 via CLAUDE.md §10 (panel 016) | ~55–65 lines, all in `crates/heroes-cli`; **zero** in `crates/heroes/src/` | landing diff ≤65 lines in `heroes-cli` incl. test, 0 under `crates/heroes/src/`; the fixpoint diff never mentions the command | the containment sentence below, carried verbatim |
| historian (advisory) | approve | Python 2002 · Zig `zen` · rustc's ICE joke — third instance of a proven form | — | ≤1 maintenance commit per year (baseline: `this.py`, 5 commits in 24 years) | flips on a sourced case of a toolchain *removing* its egg for stated cost, or a hidden-vs-listed divergence causing a real defect |

## The containment sentence (compiler-engineer's condition, verbatim)

> record a one-off author exception in the class `--version` already occupies —
> zero input, constant output, exit 0 — never citable for any capability that
> reads a file, a flag, or the environment.

Carried as written, so the engineer's verdict is **approve**. The stopping
rule's refusals (`--no-line`, panel 020; `outline`/`explain`/`doc`, help.rs)
remain the surface's immune system; this file is not a precedent any of their
successors can cite.

## Findings

- **The egg cannot hide, and that is the better tradition.** The one-table
  design (panel 016; `table.rs:7` — "a new command touches exactly one of the
  three") means a `this` entry appears in `--help` and in the unknown-command
  enumeration. Hiding it would add a third pre-table special case. Zig lists
  `zen` in its man page; Python hides `this.py`. Heroes' own architecture
  forces Zig's side, which the historian's record says is the better one.
- **The historian corrected the brief**: the easter egg *predates* PEP 20 by
  two years, not the reverse. Zen posted by Tim Peters 1999-06-04
  (comp.lang.python); `this.py` landed 2002-02-08, commit `63cd9bf4`, Fred
  Drake, *"Python 10 was a success, commemorate it!"* (IPC 10's T-shirt read
  "import this"); first release 2.2.1, 2002-04-10; PEP 20 is 2004-08-19. The
  Warsaw attribution is secondary-source only; the commit of record is Drake's.
- **`this.py` stores its text ROT13-encoded, and the one de-obfuscating commit
  was reverted to protect the joke.** Heroes stores its Zen in plain text —
  law 4 of the text itself says what the eye can see is all there is.
- **Counter-precedent, reported as a failed search and not an impossibility**:
  no `go` subcommand printing the Go Proverbs was found (Pike, Gopherfest 2015;
  they live on a website instead).
- **All eight Bowie references verified, 8/8** ("Heroes" 1977 · Sound and
  Vision, *Low* 1977 · Starman, *Ziggy Stardust* 1972 · Changes, *Hunky Dory*
  1971 · Oh! You Pretty Things, *Hunky Dory* 1971 · Ashes to Ashes, *Scary
  Monsters* 1980 · Under Pressure, with Queen, 1981 · The Man Who Sold the
  World, 1970).

## The text of record

```
The Zen of Heroes

 1. First of all and freely, this language is a tribute to the great David Bowie.
 2. A modern language made for LLMs, yet plain to human eyes.
 3. The machine has read everything, yet sees only the page before it.
 4. What the eye can see is all there is, nothing but sound and vision.
 5. One way to say each thing, and every program sings it in the same voice.
 6. Although that way is not obvious at first, the Starman waiting in the sky already knows it.
 7. The semantics you already know, a syntax nobody has seen before.
 8. Turn and face the strange, for every strangeness here is deliberate.
 9. Nothing changes behind your back, every change signs its name where it happens.
10. Every plausible mistake becomes a compile error before the program ever runs.
11. Unless you confess it honestly and write ??? in its place.
12. An honest hole in the program beats a confident guess every time.
13. A compile time error is an answer, a run time error is an ambush.
14. An error tells you how to fix the program, anything less is a complaint.
15. A repeated word is cheap, a forgotten one costs the whole program.
16. The specification is small and lives under pressure, every word must earn its place.
17. Nothing crashes and nothing leaks, ashes to ashes, what it takes it returns.
18. Where the pretty things and the robust disagree, the robust wins every time.
19. There is no standard library, everything comes from C, the man who sold the world.
20. We can be heroes, just for one day.
```

Twenty laws, ASCII only, eight songs. Laws 5–6 and 10–11 are deliberate
contradiction pairs, the Zen of Python's own device ("Errors should never pass
silently. / Unless explicitly silenced."). Law 20 is the language's name.

## Predictions to score

- compiler-engineer, half 1 — **scoreable at the landing commit**: `git diff
  --stat` ≤65 lines added in `crates/heroes-cli` (test included), 0 under
  `crates/heroes/src/`.
  **Scored at the landing commit, same day**: the count half is **false** — 95
  insertions, measured with `git diff --cached --stat` — and the containment
  half is **true**, 0 lines under `crates/heroes/src/`. The overage is the
  byte-exact test carrying all twenty laws (the engineer priced a ~12-line
  structural test; the text being the artifact, the landing chose §9's
  deliberate redundancy instead) plus the module doc. The prediction failed on
  its own terms and the record keeps it failed.
- compiler-engineer, half 2 — at **M-selfhost-fixpoint**: the fixpoint diff
  never mentions the command.
- historian — at each yearly audit: ≤1 maintenance commit per year touching
  `commands/this.rs`.

## Verdict

**Adopted** — author instruction 2026-08-16, given in advance of the sitting
(retro-record; no ratification pending). One-off exception to §10's stopping
rule under the containment sentence above. Implementation lands in its own
commit citing this file.
