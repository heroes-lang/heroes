# Panel 178 — spec-warden

Read `00-shared.md` first. Your directory is `<scratchpad>/178-spec-warden/`, a
`git archive` of HEAD `57679005`; build the compiler there from the seed. You
judge the indicator (design.md §1.2, §1.6) and Principle 0's burden of proof,
with veto on a budget breach. Write your report to `REPORT.md` in your
directory.

## The instrument, and it must be the real one

`./heroes measure spec/heroes-spec.md` in the lane read, while this brief was
written: `cl100k_base` **6282**, `real` **8361** (*claude-opus-5, 2026-09-23 —
the binding number*), headroom **1879** against the ceiling of **10240**, the
FFI floor mortgaging 60 of it. **Price every draft on the real instrument**:
apply it to `spec/heroes-spec.md` in your own copy, run `./heroes measure
spec/heroes-spec.md --refresh`, record the number, revert. `--refresh` needs
`ANTHROPIC_API_KEY`: `. /Users/joseph/Temp/heroes/heroes-lang/.env` (print
`${#ANTHROPIC_API_KEY}` if you must check it loaded, never the value). If it
exits 2, the number you have is a lower bound, and you say so in those words
(`.claude/rules/spec-shape.md` § How a change to the document is made).

## What to price

1. The shared brief's proposal text as written, and each clause alone: R1,
   R1 + Z1, R1 + Z2, T.
2. R2 (panel 163 measured +32 real then, at a baseline of 8030), A (`[x; N]`,
   one clause in § 13 or § 10), and R0.
3. **Principle 0.** The spec-warden of panel 163 set the withdrawal condition
   for R2: *three `examples/` programs declare a fixed array longer than 8, and
   a named removal is measured in the same commit*. Measure the condition today
   (`00-shared.md` measurement 1 says the repository has none longer than 8)
   and say whether the header census, a measurement of the libraries
   M-core-packages will bind, is a *measured argument the panel accepts*
   (CLAUDE.md § 2) in its place, or not, and why.
4. **A named removal, if one exists**: whether any sentence of § 13 becomes
   redundant under a route (for example *build one with `[a, b, c, d]`, as many
   elements as the type says*, or the `partial` sentence's construction half).

A verdict per route, a prediction of the real count of the adopted text, and
the condition that would change your verdict.
