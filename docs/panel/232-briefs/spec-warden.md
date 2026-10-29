# Panel 232, the spec-warden's brief

Read `00-shared.md` first, and the critic's findings it cites
(`docs/panel/232-reports/critic-briefs.md`). Your worktree is
`.claude/worktrees/232-spec-warden/`; your report
`docs/panel/232-reports/spec-warden.md`, written as you go.

**First, before any other work, the blind seat's predictions**, under a
heading `## Predictions for the blind seat`: for each of its three tasks (O,
S, X; `blind/README.md`, `blind/tasks/`), how many of the 14 sessions that
read P you predict right, and the share of the 14 that read T, each with one
line of why. The blind run waits for them, and the coordinator copies them
into the README before its first session.

**The instrument.** You are named for `heroes measure spec/heroes-spec.md
--refresh`, the token-counting endpoint the ledger's landings have called
since panel 123 (34 of its 121 rows name it, F30): load the trunk's `.env`
with `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a`, and never
print the key. `--refresh` refuses every path but the specification and the
contract, so a draft is priced by applying it to your worktree's
`spec/heroes-spec.md`, measuring, and restoring, or written down as a lower
bound in those words (`.claude/rules/spec-shape.md` § How a change to the
document is made).

**Today's count**, at `afe983265`: 10,343 real on `claude-opus-5` and 7,890
on the vendored maximum, against a ceiling of 16384, the headroom 5,981 once
panel 030's 60-token mortgage for the FFI floor is counted (F19); step 1's
§ 11 sentence spent +15 real, panel 230's own price to the token.

**Price every draft, alone and together**, real and vendored, and say for
each what pays for it (design.md § 1.6: a named removal or a registered
prediction naming an instrument that exists):

1. the order of evaluation: where it lives when it spans § 7's operators,
   § 9's calls and § 10's literals (`.claude/rules/spec-shape.md` § Where a
   rule lives: one home, F32), its cheapest TRUE wording once it says the
   write's two phases and the built-ins' calls (`00-shared.md` item 1), and
   whether § 9 owes *a direct call's labels keep the declared order*, half of
   which § 9 already says of a call through a value (F2);
2. `<<` and `>>`: the sentence that states what design.md § 4.14 rules (the
   wrap, the sign copied), or, if the compiler-engineer's lost-set-bit route
   holds, the sentence for it;
3. `==` on a handle given back: § 13's clause kept and the runtime made to
   abort, or the clause narrowed to what the runtime does;
4. § 9's *recursion too deep aborts* becoming *a call chain deeper than the
   stack aborts*;
5. § 11: whether *a literal one is refused* is owed, its price, and the
   edges F7 found;
6. the paragraph `1.0.0` rests on (`paragraph-draft.md`), in each home and
   at each home's cost: in the specification it would restate § 6's one
   definition of an abort, which `.claude/rules/spec-shape.md` refuses
   (F32); the README is held by the site's build (`checkReadme`,
   `site/src/lib/claims.ts`), bound by `site/CLAUDE.md`, and a push that
   touches it publishes the site (F18); design.md is the source of truth,
   and what a reader of the language is given is the specification
   (design.md § 1.6), not design.md.

**Principle 0** (CLAUDE.md § 2) judges each: a sentence the compiler needs
or one that provably serves the thesis. The blind seat's tasks O, S and X
measure the thesis half for items 1, 2 and 5.

**And the author's rule** (CLAUDE.md § 2, `00-shared.md`): a sentence that
adds a form to keep an older program compiling is refused before it is
priced.
