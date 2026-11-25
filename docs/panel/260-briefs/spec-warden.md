# Panel 260, the spec-warden's brief

Read `00-shared.md` first, whole (§ 1 the author's principle, § 2 the questions, § 3 the tree and the language today and
the spec's counts, § 7.1 where a declaration could live), then, if the coordinator's prompt says it exists, the critic's
first reading (`docs/panel/260-reports/critic-briefs.md`). **Your agent file is the base's**
(`.claude/agents/spec-warden.md` of your copy); design.md §1.2 and §1.6 are reached by `grep -n`, never from memory.
Under the author's amendment of 2026-11-22 (CLAUDE.md § Precedence: *"the most robust solution now always wins, a new
form included and at the cost of the spec's size"*) **the budget is a cost to be measured and written, never a veto**;
Principle 0's burden (CLAUDE.md § 2) is yours to read against the author's principle, which is the proposal. Your copy is
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-spec-warden`, a detached worktree of the base, `c3885b81b`; build its
compiler from the seed and stage 2 there (`00-shared.md` § 10). Your own texts go beside your report as
`docs/panel/260-reports/spec-w*.txt`, each with its diff `spec-w*.diff` against the base's `spec/heroes-spec.md`.

## The counts

- **Before**, read at the base `c3885b81b` by the brief's repair (`00-shared.md` § 3.3, `heroes measure
  spec/heroes-spec.md` from the tree's root with the base's compiler, no `--refresh`, 11:33; the clerk read the same at
  `80e2e39ad`, 10:33, and the spec did not move between the two): claude-legacy **9,877**, cl100k_base **10,054**, **real 13,185**
  (`claude-opus-5`, the pin of 2026-11-23), headroom **3,199** against **16,384**, the FFI floor mortgaging 60. The
  file: 516 lines, 34,437 bytes, sha256 `c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee`. **Re-read
  them yourself** and say if any differs.
- **Run `measure` from your copy's root**: it reads `vendor/tokenizers/` from the folder it runs in (panel 259's brief
  recorded *"cannot read `vendor/tokenizers/claude-legacy.json`"* and exit 2 run elsewhere, *carried*), and for any file
  but the pinned spec it prints the two vendored rows and **no `real` row**. **A vendored delta is not a price**
  (`.claude/rules/spec-shape.md` § How a change to the document is made): the real count needs `./heroes-next measure
  spec/heroes-spec.md --refresh`, one request to the count-tokens endpoint each, **only where the coordinator's prompt
  authorises it, and then at most four**: `cp spec/heroes-spec.md spec.orig` and its `shasum -a 256`, apply the text,
  `set -a; . /Users/joseph/Temp/heroes/heroes-lang/.env; set +a` (print `${#ANTHROPIC_API_KEY}` alone, never the value),
  `--refresh`, then restore with `cp spec.orig spec/heroes-spec.md` and compare the `shasum` before any other command.
  Without that authorisation every count is written **a lower bound, vendored only**, in those words.

## What you judge

1. **The sentence each home of § 7.1 owes, written and measured.** No candidate text is drafted for you: write, for each
   home the sitting weighs (a line in the package's face module, a fifth kind of top-level declaration in § 4; a mark on
   the group in § 13; a key of the package's own catalogue in § 1; a manifest file per package), the smallest sentence
   that lets a reader write a package that binds the layer and declares it, and predict which programs the compiler
   refuses; and for the three homes the first list lacked (`00-shared.md` § 7.1: the `link` word the group already has,
   which panel 234's R6 refused, *"no `link` word or flag in a package"*; the floor's own file name; a `use` of the
   layer in the binding module), say what each would cost the text. **A catalogue key meets
   `.claude/rules/cli-surface.md:73-76`** (*"a library or package of spec § 13 ... a mode ... is refused by the
   reader, never ignored"*, `00-shared.md` § 3.2): say what of it a catalogue home changes. Merge where you can
   (`spec-shape.md:67-69`, *merging beats appending*): § 13's line 401 already says
   *"sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names one"*,
   which the principle falsifies for an undeclared group; § 1's lines 42 to 44 already name *"the package's own
   catalogue"*. Each text, its diff and its vendored delta, and the real one where authorised.
2. **One home for the rule** (`spec-shape.md` § Where a rule lives): the declaration's form and the refusal that holds it
   live in the section of the operation they govern; say which, and whether a pointer elsewhere is owed.
3. **What a reader must be able to predict** from the text alone: which of their packages must declare (one that names a
   member's header in a group; one whose own header includes one, `00-shared.md` § 6.2; a face whose floor binds; a
   package binding `hero_os.h` alone, which is the runtime's), what an undeclared one gets, and what a declaration that
   binds nothing gets (§ 7.4). Does the text need the list of the layer's headers, or a rule a reader applies to a
   header's name (`hero_<member>.h`, while `hero_os.h` and `hero_compiler.h` are the runtime's)? A closed list of
   headers is a list a new member falsifies: say what holds it. **The spec names three such headers today**,
   `hero_os.h`, `hero_net.h` and `hero_time.h`, at lines 401 and 428 and nowhere else (`grep -noE
   '[a-z_]*hero_[a-z_]*\.h' spec/heroes-spec.md` at the base), so the 39 other member headers are named by no text a
   reader holds; the blind run's W4, over `hero_activation.h`, and its rule D1c measure exactly this question
   (`llm-ergonomist.md`). And a reading that is not a header's name at all (a group naming
   `platform/hero_audio_arm.h`, `shapes/platformhdr/`; a group binding only a constant, `shapes/constonly/`;
   `00-shared.md` § 6.2) is a case the sentence must decide too.
4. **The instrument**, design.md §1.6's payment rule: an addition owes a named removal or a registered prediction
   naming an instrument that exists (grep the `spec`, `unseen` and `records` suites, `tests/harness/suite_spec.hero`).
   Which instrument holds the sentence's words to what the compiler refuses, as `tests/harness/spec_refusals.hero`
   holds the signal sentence's?
5. **The importer's side**: the spec says today that *"a program that uses no package has no catalogue"* (§ 1, lines 35
   to 36); under each home, what does a program that binds the layer itself, with no package, write?

## Predictions registered before the blind run

**Your report must hold the heading `## Predictions registered before the blind run`**, with your predictions under it,
**before any blind session starts** (the runner copies your report, hashed, as it starts). The run is **authorised by
the author and started by the coordinator after the synthesis** (`llm-ergonomist.md`); you predict before either arm's
text exists, so **predict for each of your own texts** (`spec-w*.txt`, each named by its home), each a number and a
range: the D1, D1c, D2 and D3 counts and D4's report of that file's rules, under that text as `adopted` and under
`now`, and the spend. Your predictions are read by `blind/registration.md` § How a prediction is read: an arm is read
against your text whose diff is byte for byte the arm's; else against your text of the same home, every line the
arm's diff has and yours does not named in the scoring; else the arm is *unpredicted*, neither right nor wrong. Say
which of your texts you expect to be adopted, and which the rival.

End with the verdict structure of your agent file, then one line per numbered question: the text you would adopt (its
count, real or *a lower bound, vendored only*), and the one measurement that would change your mind.
