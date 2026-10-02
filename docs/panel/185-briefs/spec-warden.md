# Panel 185, the spec-warden's brief

Read `00-shared.md` in this directory first; it holds the five questions,
their routes and every measured fact, and the rules of your directory
(`<scratchpad>/185-spec-warden/`, a copy of `03e70520`, your compiler built
inside it from the seed). Write your report as you go to
`docs/panel/185-reports/spec-warden.md`.

**Your instrument**: `./heroes measure spec/heroes-spec.md` in your copy, the
vendored counts (6,838 maximum at 00:41 on `03e70520`) and the `real` row,
recorded at 9,060 on 2026-09-28 against a ceiling of 10,240, of which the
FFI floor mortgages 60 (panel 030 R3), a headroom of 1,120, and re-taken only
by `heroes measure --refresh`, a paid call this sitting does not make: so
price each route's sentence on the vendored maximum, before and after, by
writing the sentence into your copy and measuring, and say that the real
delta is the landing's to take.

**What you judge**, per question: the spec sentence or production each route
needs, its tokens, and Principle 0's burden (CLAUDE.md § 2: a form enters if
the compiler needs it or a measured thesis effect is shown). In particular:
- Q1 (1a): spec § 13's sentence on what a macro-only binding's parameters
  and result are held to, beside its pointee sentence, an FFI sentence; and
  for (1b) and (1d), design.md's four promises of macro reachability (`:565`,
  `:645`, `:2265-2266`, `:2401-2402`), which design.md prices nowhere but a
  reader of it meets;
- Q2: whether `Inline` is to say what design.md §4.7 says, and the
  production's text and price under (2a) and (2c);
- Q3: whether either route needs a spec sentence (panel 184's ratified R4
  sentence, *A statement after a jump, in its block, is a compile error ...*,
  is not yet in the spec: `grep -n 'after a jump' spec/heroes-spec.md` is
  empty at 00:41);
- Q4: design.md §4.15's premise sentence under each route; and whether spec
  § 8's `Pattern` admitting `[ "-" ] string` is to be tightened, or is a
  grammar looser than its checker by design;
- Q5: each route's sentence in spec § 2, next to R1's ratified one.

**You hold a veto on a budget breach** (design.md §1.6).
