# Panel 194, spec-warden

Read `00-shared.md` first, then panel 178's sitting and its spec-warden report
(`docs/panel/178-reports/spec-warden.md`), then `spec/heroes-spec.md` § 13 and
design.md §1.6 and §1.2 (grep them), in your copy.

Your seat judges the indicator (design.md §1.2's cost formula, §1.6's budget)
and Principle 0's burden of proof, and holds a veto on budget breach.

1. **Price every sentence the routes add on the real instrument**, never the
   vendored one (`.claude/rules/spec-shape.md` § How a change to the document
   is made): apply the draft to `spec/heroes-spec.md` in your copy, run
   `./heroes measure spec/heroes-spec.md --refresh` with `.env` sourced from
   `/Users/joseph/Temp/heroes/heroes-lang/.env` (print `${#ANTHROPIC_API_KEY}`,
   never the value), read the `real` row, revert, next draft. **This paid run
   is approved by the author for this sitting; at most eight `--refresh` runs
   in all**, the first of them on the unchanged base as a control. Today's base
   reads 9,518 real (pinned). The drafts: 178's R1
   sentence (*End a construction with `rest: zero` and every field it does not
   name is zero; only a group's record has it.*), merged where it is cheapest, and with a clause saying what it does to a
   union;
   a sentence saying C's `char` is `i8`; and the sentence 092's adopted route
   would need, if any.
2. **Principle 0** (CLAUDE.md § 2): does R1 serve the compiler or a measured
   thesis effect? 178's lapse condition was *fewer than 3 bindings construct
   such a struct*: count today's with `git -C /Users/joseph/Temp/heroes/heroes-lang ls-tree -r
   --name-only 7a26a0a6` (read-only; `git ls-files` fails in an archive copy),
   a construction of a group record holding an array longer than 8; the critic
   read 0 outside `tests/`, `docs/` and `archive/`.
3. **Score 178's predictions that are checkable today** (its § Predictions to
   score), each with its command.

Verdict per route: approve, object or veto, with its section, its real-token
cost, a falsifiable prediction and its condition. Report:
`<scratchpad>/p194/reports/spec-warden.md`, written as you go.
