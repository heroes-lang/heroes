# Frozen prompt template, metrics 2 and 4 (first-try rate, turns-to-green)

Turn 1 is the prompt below the first line of three dashes: this template +
`spec/heroes-spec.md` (or a variant by path, or the one sentence of the `none`
context) + ONE task, in one of two layouts the run's provenance names, a single
user message or a cached `system` block cut after `</specification>` with the
task as the user message (panel 226's R12, 2026-10-20). Never design.md, the
repository, or prior conversation. From turn 2 the conversation is append-only
and every arm receives the same feedback, one of the five fixed blocks below
the second line of three dashes, `{{MESSAGE}}` filled with the arm's compiler
message and `{{OBSERVED}}` with what the program did on the task's visible
example; the hidden cases are never returned. **Any change to this file is a
new template sha and a new measurement series**; the request body's shape is
the layout sha beside it, and a run differing in either is a different series.

---

You are given the complete specification of a small programming language
called Heroes, followed by one programming task. Write a single Heroes
program that solves the task. Output only the program, in one code block,
with no commentary.

<specification>
{{SPEC}}
</specification>

<task>
{{TASK}}
</task>

---

<feedback kind="refused">
The compiler refused the program:

{{MESSAGE}}

Fix the program. Answer with the whole program in one fenced code block and nothing else.
</feedback>

<feedback kind="no_program">
Your answer held no fenced code block holding `function main`. Answer with the whole program in one fenced code block and nothing else.
</feedback>

<feedback kind="extern">
The program may not declare `extern`: write it with the language's own built-ins alone. Answer with the whole program in one fenced code block and nothing else.
</feedback>

<feedback kind="example">
The program compiled. Run on the example given in the task, it did this:

{{OBSERVED}}

Fix the program. Answer with the whole program in one fenced code block and nothing else.
</feedback>

<feedback kind="truncated">
Your answer was cut off before it ended. Answer with the whole program in one fenced code block and nothing else.
</feedback>
