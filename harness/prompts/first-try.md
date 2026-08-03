# Frozen prompt template — metric 2 (first-try rate)

Single turn. Context = this template + `spec/heroes-spec.md` + ONE task.
Never include design.md, the repo, or prior conversation. Any change to this
file is a new prompt sha and a new measurement series.

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
