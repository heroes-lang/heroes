# M-thesis-harness — the thesis, measured


**Scheduled by author instruction 2026-09-03**, and it is the second milestone
after the fixpoint with a warrant: **§1.1**, comprehension measured rather than
asserted, and design.md Part 11, whose metrics 2 and 4 have never run
(`docs/measurements/007:24-25`; M-publication-gate's checklist says so in its own
words). Everything this project claims about first-try rates rests today on
metric 3 alone.

**What it delivers is the instrument**, written in Heroes and run by the one
command: Part 11's protocol — spec-only context, single turn, frozen and hashed
prompt templates, two gradings (compiles · tests pass), Wilson intervals, one
non-Anthropic model as robustness, provenance on every run (spec sha, compiler
sha, model id, prompt sha, suite sha) — and metric 4's turns-to-green over broken
programs, capped at 5. It reaches the model over HTTPS, so it depends on
M-core-packages' `net/http` client (libcurl, step 11) and reads its key from the
environment, one of §10's three input classes.

**The tasks, and a decision of the same night.** Metric 2's held-out tasks must
be author-written or they measure the assistant's priors (panel 011; author
decision 2026-08-24, the M-guide-book item in `SCHEDULED.md`). That decision
stands: the bulk of the set is written at M-guide-book, where the author's own
work *is* writing Heroes. What this milestone adds, by author decision
2026-09-03, is a **small seed of tasks written by the author at its opening**, so
that the instrument runs once on real input before the book grows the set, and
so that the number exists before the books state it.

**Why here.** After the packages that give it a client; before the tools, whose
value it does not need; before the books and the gate, which will print the
number. Corpus material is labelled and never enters the held-out set
(CLAUDE.md §9).

*******************************************************************************
**OPEN: 4**

- [ ] **M-thesis-harness** | should the spec exhibit one worked diagnostic, when today it exhibits zero | `spec/heroes-spec.md` · `design.md` Part 11 · `docs/measurements/007-two-predictions-collected.md:24-25`

    **Origin:** the llm-ergonomist, panel 118, 2026-09-08, the other half of the
    same appetite. `/decide` 2026-09-08 split the two and refused to settle this
    one by a date: it is **decided by an instrument**, which is what CLAUDE.md §2
    asks of anything entering on the thesis rather than on compiler need.

    **The claim, in the seat's own terms**: a language whose thesis is that every
    plausible mistake is a compile error shows its reader **zero** compile errors,
    so a model cannot predict what it is about to be told. That is a falsifiable
    claim about metric 2 — first-try completion under a spec-only context — and
    metric 2 has never run.

    **Why the instrument and not a date.** A worked diagnostic is dearer than the
    **40** tokens free after M-closures-verdict, so it cannot be bought on an
    appetite; Principle 0 admits it on a **measured design.md Part 11 effect**,
    and this milestone is what produces one. The sitting that buys or refuses it
    is therefore the one that can read the number, and a refusal then owes a
    Part 6 row carrying its falsifier (CLAUDE.md §12).

    **What it has to beat, in the same currency.** Panel 120 measured one
    silent-class fact at **+30** where four signature drafts cost +63 to +86 and
    bought nothing for the class the sitting was convened about. The exhibit's own
    price is owed measured rather than estimated before it is compared with
    whatever clause the same budget could buy instead.

    **Why it matters:** the one thing this spec never shows is the thing the whole
    language is for, and whether that costs a reader anything is a measurement
    nobody has taken.

    **Re-verified 2026-09-10: STILL OPEN in substance, and one sentence is now
    FALSE.** The substance holds — `grep -c 'error\[' spec/heroes-spec.md` is **0**,
    so the spec still shows a reader no diagnostic at all. What is false is the
    pricing: *"dearer than the 40 tokens free"* rests on a headroom figure
    `docs/measurements/010-spec-budget-ledger.md` **voids in its own closing
    section**, this one by name. Measured here instead, from
    `tests/harness/suite_spec.hero`: ceiling **6144**, `REAL_TOKENS` **5378**,
    `FFI_FLOOR` **60**, so **706** free — not 40. The comparison against panel 120's
    `+30` is in the same voided currency and needs the new instrument before the
    sitting reads it.

    **Both figures moved again, 2026-09-12, and this line is the correction
    rather than an edit to the one above**: the ceiling is **8192** by author
    decision that day (design.md §1.6, panel 133), and `REAL_TOKENS` is **5716**,
    so the free figure this item prices against is neither 40 nor 706. Re-measure
    it when the sitting reads it; the currency has now been voided twice.

- [ ] **M-thesis-harness** | what it depends on, and the author's decision on the seed tasks | `design.md` Part 11 · `docs/measurements/007` · `docs/panel/011`

    **Origin:** author instruction 2026-09-03, scheduled `DESIGN-LOG.md:539`.

    **Metrics 2 and 4 have never run** (`docs/measurements/007:24-25`;
    M-publication-gate's checklist), and the instrument that runs them is a
    Heroes program over HTTPS, so it waits for M-core-packages' `net/http`
    client (libcurl, step 11) and reads its API key from the environment, one of
    §10's three input classes. Part 11's protocol binds it whole: spec-only
    context, single turn, hashed frozen prompts, two gradings, Wilson intervals,
    one non-Anthropic model, provenance (spec sha, compiler sha, model id,
    prompt sha, suite sha). **Author decision 2026-09-03**: a small seed of
    tasks written by the author at this milestone's opening, the bulk still at
    M-guide-book as decided 2026-08-24 (the M-guide-book item above stands).
    Corpus material never enters the held-out set (CLAUDE.md §9).

    **Where to look also:** `design.md` Part 11 (`:2823-2860`) ·
    `harness/tasks/README.md`.
    **Why it matters:** a thesis with one of two factors audited is an opinion
    with a decimal point, which §12 forbids.

    **Re-verified 2026-09-10: STILL OPEN, and it points at a contradiction the
    ROADMAP now has with itself.** `harness/tasks/README.md` still says *"Status: 0
    tasks"* and still keys the bulk to M-guide-book. Part 11 is `design.md:3114-3166`,
    not `:2823-2860`. **And the dependency's number moved**: the `net/http` client is
    M-core-packages **step 5** after the 2026-09-10 reorder, while
    `docs/ROADMAP.md` § M-thesis-harness still says *"step 11"* — which is now the
    byte buffer. Whoever opens either milestone repairs that sentence to name the
    package rather than a number, on §14's own rule.

- [ ] **M-thesis-harness** | panel 007's two predictions contradict each other and nothing has scored them | `docs/panel/007-terminator-enders.md:72-85` · `design.md:1800-1809` · `docs/panel/046` R1

    **Origin:** panel 007's two registered predictions, deferred as *"007-bis"*
    on 2026-08-03 and never scored. Its home since 2026-09-04, when the panel
    watch list was retired: both predictions are scored against a metric-2 run
    and nothing else can score them.

    The ergonomist's: tasks forcing a depth-0 expression break lose **≥25pp**
    first-try under a spec that says nothing. The spec-warden's: **zero**
    baseline completions contain a depth-0 illegal break.
    `docs/panel/007-terminator-enders.md:82-85` says in its own words that
    *"these two cannot both hold"*.

    **What has been settled meanwhile, so the sitting is not asked the wrong
    question**: the *form* is refused and now enforced — `design.md:1800-1804`,
    and `grammar_expr.hero`'s `ends_the_expression` refuses a continuation at
    bracket depth zero after a defect that had it compiling and printing at exit
    0 (panel 095, ratified). What is open is only whether the **spec owes the
    ~28-word layout sentence**, and panel 007 deferred exactly that as one
    package with Nim's explicit continuator set. **The scoring rule is panel 046
    R1's**: an outstanding prediction is *re-decided, never renewed* — scored,
    or marked `lapsed` with its clause re-argued under the removal branch.
    `design.md:1807-1809` still carries the deferral.

    **Where to look also:** `docs/measurements/007`, which scored panel 046's
    six rows and not these two.
    **Why it matters:** a prediction nobody can score is a deferral wearing a
    measurement's clothes, and this pair has worn it for a month.

    **Re-verified 2026-09-10: STILL OPEN, and unscoreable for the reason it
    says.** `docs/panel/007:82-85` still carries both predictions and its own
    *"(these two cannot both hold)"*, and `selfhost/grammar_expr.hero:160` still
    defines `ends_the_expression`. Metric 2 has still never run, so neither
    prediction can be scored. **Both design.md pointers moved**: the
    statement-position sentence is `:1853` and the deferred trailing-operator
    continuation `:1861`; `:1800-1809` is now about literal patterns and
    `&&`/`||`.

- [ ] **M-thesis-harness** | the compiles-but-wrong-output bucket the harness never named | `docs/panel/009-spec-budget-2000.md:100-103` · `design.md:2851-2853` · `selfhost/mutate/score.hero`

    **Origin:** panel 009's llm-ergonomist, as a condition on its own vote
    2026-08-04. Its home since 2026-09-04, when the panel watch list was
    retired; the condition is the seat's own and it is a condition on the
    instrument, so it belongs to the milestone that builds it.

    **The harness must report a compiles-but-wrong-output bucket, separate from
    the compile-error bucket, and today nothing names one.** The seat's words:
    without it *"the prediction is untestable and the raise unjustified on this
    axis"*, and its scoring line adds *"only if the compiles-but-wrong bucket
    exists"*. **What exists instead, measured**: Part 11 states two gradings,
    *compile* rate and *tests-pass* rate (`design.md:2851-2853`) — which yields
    the bucket by subtraction and never names it, and `git log -S` dates that
    text to 2026-08-03, the day **before** the sitting, so it is not an answer
    to the condition. `selfhost/mutate/score.hero`'s `survived` is a
    compiles-but-presumed-wrong class over mechanical mutants, which is **metric
    3 and not this**: it never compares program output. The golden runner does
    compare output, so the capability is in the tree and the reporting class is
    not.

    **Also owed and cheap**: the sibling condition from the same vote — that
    Part 11 report *class-weighted* numbers — survives in **one** place in this
    repository, the sitting itself, which is the shape §11 calls a premise that
    expires in silence.

    **Where to look also:** `docs/panel/009-spec-budget-2000.md:23` ·
    `tests/harness/suite_run.hero`.
    **Why it matters:** the thesis's central claim is that this language deletes
    SILENT errors, and the instrument that would show it has no column for them.

    **Re-verified 2026-09-10: STILL OPEN, and its own one-place count is
    confirmed.** `class-weighted` survives in exactly **one** place, `docs/panel/009`,
    and *"compiles-but-wrong"* appears nowhere in any instrument — only in that
    sitting, in `docs/panel/010:118`, in the narration and in this item.
    `selfhost/mutate/score.hero`'s `.survived` is still over mechanical mutants
    alone. One pointer moved: Part 11's two-gradings sentence is
    `design.md:3136-3138`, not `:2851-2853`.

*******************************************************************************
