# Panel 214, the spec-warden's brief

Read `00-shared.md` first. Your question is the indicator and Principle 0.

1. **Does a route owe the specification a sentence?** Route C does by definition; B, E and F may
   (an author must know that a header may not declare certain names, or that a refusal names the
   compiler). Write each candidate sentence, measure it: `./heroes measure spec/heroes-spec.md`
   before and after in your copy (offline only: the vendored counts; the `real` count is the pinned
   10,048, and a `--refresh` is a paid run you do not start), and say where in § 13 it would stand.
   The budget: 10240, the FFI floor's 60 held back (`.claude/rules/spec-shape.md`, read it).
2. **Principle 0** (CLAUDE.md § 2, design.md §1.0): no language form enters by these routes; say
   whether a rule about which C names a header may declare is a language rule or a toolchain fact,
   and which document owns it (the spec, design.md §4.19, `.claude/rules/c-boundary.md`).
3. **The exit-code contract** (`.claude/rules/cli-surface.md` § One command: exit 1 the input has
   diagnostics, exit 2 the tool could not run): read each route against it.

Veto on a budget breach with no named removal.
