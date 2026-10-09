# Panel 198, spec-warden

Read `00-shared.md` first, whole; then, in your copy
(`<scratchpad>/198-spec-warden/tree`, `rm -f tree/.git`, your compiler from
its seed), `spec/heroes-spec.md` § 13 whole, `.claude/rules/spec-shape.md`,
and design.md §1.2 and §1.6 reached by grep. Your running notes go in
`<scratchpad>/198-spec-warden/report.md`. Written on 2026-10-08 for the full
panel the author convened; this seat had no brief in the soundness lane.

Your seat judges the indicator (design.md §1.2 cost formula, §1.6 spec
budget) and Principle 0's burden of proof, and holds a veto on a budget
breach.

1. **The sentence that was removed.** Until 2026-10-05 § 13 read, after
   *asks the system where its headers and libraries are and what else it
   needs.*: *A package answering with anything this compiler does not pass on
   is refused, naming what it said.* Panel 192 R8 item r1 removed it
   (`d0409cc5`, `git log -S'does not pass on' -- spec/heroes-spec.md`), -22 on
   both vendored tables, on the ground that the message states the whole rule
   and the repair each time it fires (`docs/panel/192-reports/spec-warden.md:527-538`;
   panel 188's warden's §1.4 test, *what the machine answers has its home in
   the message*). Today `grep -n 'package' spec/heroes-spec.md` returns
   `:445` and `:448` only, and the spec names no `CPATH`, `LIBRARY_PATH` or
   `pkg-config`. Panel 050 had held the opposite: *The list is named in the
   spec, because the list is the safety*
   (`docs/panel/050-the-package-clause.md:65-66`). **Does the ruling owe the
   spec a sentence again?** Ask it of each route: one that changes which words
   a correct program's package may answer (semantics, CLAUDE.md § 4); one that
   adds an argv or environment allowance (G), which a reader cannot find in a
   message that has not fired; one that only rewords the note (A). r1's
   premise was that the message carries the rule; say whether each route
   leaves that premise true.

2. **The numbers, measured in a copy of the frozen tree** (between 07:52:39
   and 07:53:52 on 2026-10-08, `env -u ANTHROPIC_API_KEY ./heroes measure
   spec/heroes-spec.md`, outputs in `<scratchpad>/198-briefs-work/`):

   | file | claude-legacy | cl100k_base | maximum | spread | real |
   |---|---|---|---|---|---|
   | the frozen spec (456 lines, 25,247 bytes) | 7337 | 7467 | 7467 | 130 | 9831, *claude-opus-5, 2026-10-07* |
   | r1's sentence restored verbatim at `:446` | 7359 | 7489 | 7489 | 130 | STALE: *the recorded count is for 5f6c56b946f3fb85 and this file is 7a0bbed633ea823f* |

   The frozen spec's verdict line: *Headroom: 409 against the 10240 ceiling,
   but the FFI floor mortgages 60 of it (panel 030 R3), so what is measured
   against the ceiling is 9891.* So r1 restored costs +22 on each vendored
   table, which is **a lower bound, in those words**
   (`.claude/rules/spec-shape.md` § How a change to the document is made:
   *A vendored delta is not a price*). Run both again in your copy before you
   lean on them.

3. **Draft the sentence each route owes**, at its smallest honest wording,
   or write *none* and why; measure each draft with `heroes measure` on a
   copy of the spec in your directory, and write every delta as a lower
   bound. **The real count needs `heroes measure spec/heroes-spec.md
   --refresh`, a paid call this sitting's briefs do not name: do not run
   it.** Say which drafts it would decide between and how many calls that is,
   and the coordinator decides. Hand your drafts to the coordinator by
   writing them whole in `report.md` under a heading `drafts for the blind
   seat`: where a route owes a sentence, the blind seat's variant for that
   route reads the spec with it.

4. **Principle 0**: does the compiler need any route? It binds no package
   (`grep -rn -E '^extern .* package "' selfhost`: 0). Does a route serve the
   thesis by a measured effect? The blind seat's readings are that
   measurement where they exist (its brief); until then your ruling on that
   half is provisional, and says so.

5. **The note as the rule's only statement.** Every route changes the
   `ffi_package` note's text (the critic's § 1), and the note is pinned by
   the compiler's own tests (shared brief). Judge each route's note as you
   would a spec sentence: what it costs a reader who meets it once, and
   whether a reader who has not met it can write a correct program.

Verdict per route: approve, object or veto, with its token cost (a lower
bound where it is vendored), a falsifiable prediction and its condition.
