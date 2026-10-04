# Panel 185's R7 landed: a plain literal whose braces would be a hole naming only what is in scope is refused

2026-10-03 at 00:26 by the clock (`date`), lane fbrace records the landing of
panel 185's R7 as the author decided it on 2026-10-02, route (5b)
(`docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md`
§ Author's verdict), which defect 176 held, at `a5fc53de` (00:15), after
panel 184's R1 as the verdict asks; its sentence is in spec § 2 since
`8fc6e206`.

## The decision

| | |
|---|---|
| date | 2026-10-02 |
| decision | (5b) as built and as decided: a plain literal whose hole, read with an `f`, names at least one name and only names in scope where it stands (a local, a declaration its module sees, a built-in, a module it uses) is `hole_without_f` at its first such hole, `{i + 1}` included; every name its holes read counts as read, so the mistake is one message; two guesses, the `f` written and the braces kept as text, each doubling every other brace, `}` too; a thesis rule, which `--permissive` drops. Not adopted from the prototype: the refusal of a pattern's literal, `"{MAX}" =>`, which an `f` cannot spell, found in the lane's first pass |
| reason | `x = 1` over `print("{x}")` printed `{x}` at exit 0. Recounted on the lane's tree, `check --brief` before and after over the 154 tracked files that hold a `{` in a string literal: 6 files move, all under `docs/panel/`, 8 sites refused, each a forgotten `f` written into a panel's brief or probe on purpose (the sitting's 2 true sites among them), 0 false alarms, 3 files from exit 0 to 1, all panel 185's probes; nothing of the compiler, the harness, the examples or the goldens moves |
| design.md § | §1.3 (the author's ruling of 2026-10-01: its locality test speaks of meaning, not legality), §4.17 |
| panel | 185 |

## What it leaves open

- The compiler-engineer's census prediction, *exactly 1 file moves and 0
  change exit*, was made over `03e70520`; on this tree it reads 6 and 3, the
  difference being panel 185's own five briefs and probes, added after that
  census, each holding the mistake on purpose. Its `forget-f` prediction on
  the recovery instrument is owed at the round's gate, unrun in the lane.
- (5b)'s falsifier stands, a template whose placeholder names a binding in
  scope; `examples/template/main.hero` holds none.
- The round's gate, as for every repair of the lane.
