# Variant contexts of the thesis instrument

Two files that stand where the specification stands in a prompt, each passed
to `tests/harness/thesis.hero` by path as `--context <file>`
(`tests/harness/thesis_prompt.hero`, `context_of`), so that a run beside the
`spec` context scores what one part of the document buys. They are measurement
inputs, never the language: `spec/heroes-spec.md` is the specification, and
neither file is judged as one. Both were made on 2026-10-24 from the
specification at `a4e09e061`, whose sha256 is
`4d8e5251e78dee55f8c9a563a0e98e50dfac83b5d00c95cde4703ccd2a3e9cc7`, the
digest the held-out run's pre-registration names.

**Which run scores them**: panel 226's R6 asks the variants *at the held-out
run on ten tasks with Haiku and Sonnet*, so the scoring run is the author's,
over the held-out suite's tasks 01 to 10 opened in `build/opened/ten/`, in a
shell no model reads. The run over the assistant's ten tasks (measurement 051)
is its pilot: what the three contexts cost and how long they take, and enters
no score.

## `spec-without-productions.md`

What panel 133's third prediction compares the specification against (issue
1139): the llm-ergonomist's *spec plus grammar beats the spec alone by at most
+1 task in 10, and the silent-divergence delta is exactly 0*. It is the
specification less its grammar, and nothing else:

- every line indented four spaces outside a fence, which is what a production
  is by the one extractor that exists, `heroes grammar`'s
  (`selfhost/cli/grammar.hero`, `production_block`): 65 lines holding 44
  productions, each opening with a name, spaces and `=` (R6 wrote 46 of the
  specification of 2026-10-20);
- the opening paragraph's two sentences that define the notation, from *A
  production is written in Wirth's notation* to *come from the indentation.*;
  the layout rules after them, from *NEWLINE ends a statement*, stay, being
  the language's whatever notation states it, and so does their one clause
  naming *a production*;
- the blank lines a removal left doubled, made single.

sha256 `6e4979e404d25efd185c1de0a058656de546a0789605b413b5e650757ac7da48`;
6,698 tokens on the vendored `cl100k_base` table against the specification's
7,856, a lower bound in both cases (`heroes measure`; the reader's own count is
each request's `usage`, which the run records).

## `spec-with-one-worked-diagnostic.md`

What issue 1005 asks to measure: the specification shows its reader no
compile error, so a model cannot predict what it is about to be told. It is
the specification with one paragraph added in section 8, after the bullets on
`match`: a program whose `match` misses a case, what `heroes check` prints for
it, and the arm that repairs it. The program and the message are the
compiler's, run that day with the compiler of sources `616f959062622f6a`:
`check` exits 1 with that message, and the repaired program checks clean and
prints `4`.

**Why `non_exhaustive` and not the error the trial met most.** At turn 1 in
the strict arm of the trial's `spec` run (`build/thesis-runs/00fb571c-v3`,
`docs/measurements/050-...`), the code most samples met was `type_mismatch`,
13 of 90, and all 13 were one mistake: `repeat` given an `i64` count where it
takes a `u64`. A worked diagnostic showing that would be measured on the same
ten tasks that produced it, and a gain would read as the document teaching
those tasks' answer rather than as the effect issue 1005 names, a reader who
has seen the shape of a diagnostic. So the diagnostic is one those ten tasks
never drew at turn 1, and the one the thesis exists for: a `match` that is not
exhaustive.

sha256 `b36c80dd616737bd4ad15c8472dae9cdbc776924cc0372e3d98d63b7791318e2`;
8,015 tokens on the vendored `cl100k_base` table, 159 more than the
specification.

## `spec-and-cards.md`

The package arm of the paired run's instrument (task 2300), which panel 234's
R14 asked at M-core-packages' close and which went, with that row retired, to
M-rebel-library's decision 15 (the register,
https://github.com/heroes-lang/heroes/issues/2610, item 7): the
specification, then the heading *The packages beside the program*, one
sentence saying that the program is compiled beside a catalogue holding the
packages whose cards follow, then the card of every face of the tree its first
line names, in E2's call form and in path order, a floor's card
left out (R5: only a program that reaches the floor reads it). It is written by
a command, never by hand:

    heroes run tests/harness/thesis.hero -- cards --packages build/thesis-packages -o tests/thesis/contexts/spec-and-cards.md  # none on 2026-11-20

Its first line, `packages <tree> <digest> spec <digest>`, names the tree and the
specification it was written from. The instrument drops that line from the
prompt, places the tree beside every answer of this context and of no other,
and refuses the file before any request where the tree's digest, the
specification's, or any byte of the file differs from what `cards` writes now
(`tests/harness/thesis_cards.hero`), so the run's two arms hold one
specification and the cards describe the packages the answers compile against.
The tree is a folder git ignores, placed by the compiler from the repository's
own index:

    mkdir -p build/thesis-packages
    heroes add rebel --index file://../../packages -C build/thesis-packages

Run with Rebel on 2026-11-20, it answers `error[absent]`: the index publishes
no version of Rebel, no `packages/rebel/versions` on 2026-11-20, and the
instrument takes only entries a `sum` pins, so a run publishes Rebel's version
there first. The tree's digest leaves out the catalogue's `index` line and the
key ending a placed folder's name, so one version placed from any spelling of
the index is one tree (`tests/harness/thesis_packages.hero`).

**No such file stands today.** The one written on 2026-11-15 held the cards
of the packages the index held then, and was removed on 2026-11-20 with them
(M-rebel-library's decision 8): no index places that tree again, so its
first line named a tree no run could place. A run of the instrument writes the file from the tree it places, before the run
is registered, and commits it; the file's own first line then names that tree
and that specification.
