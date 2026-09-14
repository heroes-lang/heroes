# Panel 147 — llm-ergonomist

**Do NOT read `00-shared.md`, and do not open the repository.** Your verdict is
an experiment, and it is only worth having if you know nothing the reader of
this language would not know. Your whole world is the three specification
variants named below, and the tasks.

You judge the objective: **can a competent writer, given only the
specification, write the right program — and does a plausible mistake become a
compile error or a silent wrong answer?** You have a veto on non-local
constructs.

## Your inputs, and there are no others

Three candidate specifications of the same language, label-stripped:

- `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/d2c6e340-fa30-4088-9630-1b1df8597855/scratchpad/panel-147/variants/variant-1.md`
- `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/d2c6e340-fa30-4088-9630-1b1df8597855/scratchpad/panel-147/variants/variant-2.md`
- `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/d2c6e340-fa30-4088-9630-1b1df8597855/scratchpad/panel-147/variants/variant-3.md`

They differ in a small number of places. **Part of your job is to find where**,
and to say whether a reader would notice without being told.

Do not try to work out which is the current language and which are proposals.
If you form a guess, say so and say what gave it away — that is itself a
finding about how the document reads.

## The situation the language is in

A program written in this language gets everything beyond the specification
from C libraries. Some C libraries hand you a thing you must later hand back —
open a database, prepare a query, later close and finalise them. Nothing hands
it back for you.

## Your tasks — write real programs, from each variant in turn

**Task 1.** Under each variant, write a function that opens a database, runs one
query, reads every row, and leaves everything it opened closed — **including
when reading a row fails partway through**. Use only what the variant's own text
licenses. Write it three times, once per variant.

**Task 2.** Under each variant, take this shape and say what happens to the
prepared statement when the marked line fails:

```
function rows(db: Db) -> [Row]?
    statement: Stmt @ prepared(db, "select ...")?
    out: [Row] @ []

    while stepped(@statement)?
        out @ out.push(cell_of(statement, column: 0)?)   # <- this one

    _ = finalized(@statement)
    return ok(out)
```

Answer from the variant's text alone. If the text does not settle it, **say that
the text does not settle it** — that answer is worth more than a guess.

**Task 3.** For each variant, name the **most plausible mistake** a writer makes
about releasing a C resource, and say whether that variant turns it into a
compile error, a loud runtime abort, or a silent wrong answer. Rank the three
variants by that alone.

## What your report must contain

Your verdict on each variant; which you would give a writer and why; the
specific sentence in each that carried the most weight; any construct you would
veto as **non-local** — meaning a reader must hold something from elsewhere in
the file, or in another file, to know what a line does. Say plainly if a variant
made a task harder rather than easier. A finding that all three are equal is a
real finding and this sitting wants it if it is true.
