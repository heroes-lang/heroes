# Panel 181, brief for the spec-warden

Read `00-shared.md` first, in this directory. Your seat judges design.md
§1.2's cost formula, §1.6's spec budget and Principle 0's burden of proof,
with a veto on a budget breach.

**Your directory is `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/spec-warden/`,
and it is yours alone.** Make it with `git -C /Users/joseph/Temp/heroes/heroes-lang
archive 0fc98107 | tar -x -C <your directory>`, `rm -rf build` inside it, and
build your own compiler there from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, about 3 s). Never build or edit anything in the
trunk or in another seat's directory, and kill only processes you started, by
PID.

## What you decide

1. **The price of each wording.** The candidates are Variants Y and Z in
   `llm-ergonomist.md` (X adds nothing), and any shorter sentence with the same
   meaning you can find. In your copy, add each after `spec/heroes-spec.md`
   line 17 and run `./heroes measure spec/heroes-spec.md`: record the vendored
   rows and the delta from today's (6672 claude-legacy, 6794 cl100k_base,
   measured for `00-shared.md`). For the `real` row, `. /Users/joseph/Temp/heroes/heroes-lang/.env`
   in your shell and run `./heroes measure --refresh spec/heroes-spec.md` in
   your copy; print `${#ANTHROPIC_API_KEY}` if you must check it loaded, never
   the key. Today's `real` is 8999 against the 10240 ceiling.
2. **Whether the spec owes a sentence at all.** § 0 already says *NEWLINE ends
   a statement* and states the last-token rule for brackets only. Read it as a
   reader would and say whether, under route (a) or (b) of
   `compiler-engineer.md` (the compiler refuses the shape), the spec is already
   true without an addition, or whether the refusal is a rule a reader cannot
   infer and so must be stated; under route (c) (Nim's rule admitted), the
   sentence is owed by construction, so price it and say what it displaces.
3. **Principle 0's burden.** design.md §4.15 and panel 007 already ruled that
   depth-zero continuation is not in the language, ratified by the author. Say
   whether bringing the compiler to that rule owes a Part 11 effect or a
   compiler need, or is a repair that owes neither; and what route (c) would
   owe, since panel 007 set its condition (*enters only if the measurement
   baseline shows models actually produce that break shape*): name the
   instrument that could show it today, if one exists, and whether it has run
   (`grep` `docs/measurements/` and `docs/panel/` for the baseline panel 007
   meant).
4. **The diagnostic class.** A new refusal is a new diagnostic code; say where
   the codes are listed (`grep` the tree for the list a new code must join)
   and what else a new code owes (the `.expected` snapshot, the `#~`
   annotation, `surface` rows, the `check --permissive` question of
   `selfhost/diag.hero`'s `is_thesis_rule`).

## Prediction

Register one falsifiable prediction with an instrument that exists today,
scored at this milestone's close (M-agreed-retention).

Write your report to `<your directory>/report.md`, and copy it to
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/181-reports/spec-warden.md`
(the only file you write in the trunk). English, no em dashes.
