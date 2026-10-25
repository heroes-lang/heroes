# Panel 228, the spec-warden's brief

Read `00-shared.md` in this folder first. This brief adds what only you are asked.

**Your worktree**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/228-spec-warden`,
detached at `9a38f1b26`, made by the coordinator; build your own compiler there with `clang -I
runtime seed/heroes.c runtime/runtime.c -o heroes` if you need one. No file outside the repository's
root, no credential printed, no paid run beyond one `heroes measure <file> --refresh` per draft you
price (it calls `count_tokens`, which the author's key pays; `. /Users/joseph/Temp/heroes/heroes-lang/.env`
loads it, and print `${#ANTHROPIC_API_KEY}` to know it loaded, never the value).

**The count today**, `./heroes measure spec/heroes-spec.md` in the lane, 2026-10-25: `claude-legacy`
7717, `cl100k_base` 7854, `real 10297 claude-opus-5, 2026-10-24 — the binding number`, headroom 6087
against 16384, of which the FFI floor mortgages 60. Reach design.md §1.6 by grep and read the
consolidation block's budget there yourself; the ROADMAP's § Where we are cell *The spec* says what
was spent and when.

**What you are asked**:

1. **The price of each route in spec tokens.** The coordinator's reading is that no route needs a
   word of the specification, since § 12 says only that *the compiler reports what belongs there (the
   expected type, what is in scope)*, and `--json` appears nowhere in it (`grep -n -- '--json'
   spec/heroes-spec.md` prints nothing; `grep -n json` prints line 448, `retains json_object_put`); confirm or refute it with the command, and price any sentence a route
   would owe on the real instrument, `--refresh` on the real path and reverted, or as a lower bound
   in those words (`.claude/rules/spec-shape.md` § How a change to the document is made).
2. **The cap's sentence** (question 6): design.md §4.16 says the cap of five *is stated in the spec*,
   and no revision of the spec stated it, by the critic's searches the shared brief names. Rule what
   the repair is: the spec states it (priced), or §4.16's sentence is corrected (no spec token), and
   what a model writing a program loses or gains either way.
3. **Principle 0 and the stopping rule** (`.claude/rules/cli-surface.md` § One command), with the
   two issues' own words the shared brief quotes (2158: *the instrument that found it has another
   route*; 2091: *this needs no new verb*): whether this is a capability that must earn its entry (the fixpoint, the golden harness or the Part 11
   harness must type it, or a measured Part 11 effect), or the completion of what `--json` already
   promises, *how to print, never what*; and what each route's answer would be.
4. **Where the document's shape is written**, so a model meets it: `site/src/llms.txt:61-66` and the
   errors pages hold the schema to the constant through `site/src/lib/claims.ts`; say whether a
   route owes those pages a sentence and whether the site's build would catch one missing.

**Output** as `.claude/agents/spec-warden.md` asks. **Write
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues/docs/panel/228-reports/spec-warden.md`**
as you go.
