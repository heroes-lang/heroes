---
kind: task
area: records
milestone: M-thesis-harness
filed: 2026-08-03
commit: none
github: none
---

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

    **Correction, 2026-09-28 (panel 181's completeness critic, four counts):**
    the settled paragraph above is false as measured on this date.
    (1) The form compiled: `y = a +` over `1` at the statement's margin ran at
    exit 0 until this date (defect 116), and so did a break across a blank
    line, a pattern broken after `|` and `for x in` over `xs`. (2) The rule is
    design.md §4.15's *Continuation lines* bullet (`:1939-1948` at `0fc98107`),
    not `:1800-1804`; the re-verification of 2026-09-10 moved the pointer and
    did not run the claim. (3) `ends_the_expression` refuses one shape, a
    binary operator heading the line after a control form's block, and its
    postfix neighbour compiled (defect 119). (4) That repair was defect 005's
    `/decide` answer `5a` of 2026-08-28, not panel 095, which is the blank-line
    sitting. Since panel 181 (provisional) the form is refused in both
    directions, by the lexer at the line break (`continuation_outside_brackets`,
    `selfhost/open_line.hero`), and the sitting adopted a spec sentence for it
    (its item 7), which lands in a commit of its own; what this item still owns
    is the scoring of panel 007's two predictions, which metric 2 alone can do.
