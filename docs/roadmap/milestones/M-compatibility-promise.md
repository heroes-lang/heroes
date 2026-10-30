# M-compatibility-promise — the paragraph `1.0.0` rests on, and its instrument


**Scheduled 2026-09-10 by author decision**, from § What production-ready means
row 1. M-publication-gate owns the paragraph — *"Silence reads as a promise"* —
and `README.md:117` already says the version stays below `1.0.0` until it is
written; this row delivers the paragraph **and** the instrument, and discharges
that bullet.

**What warrants the instrument.** Go 1 published *Go 1 and the Future of Go
Programs*, Nim 1.0 promised that code compiling under 1.0 compiles under any 1.x,
and Rust 1.0 put *stability without stagnation* on a train where an unstable
feature is an error on stable. **In all three the sentence has a mechanism**, and
in a repository whose contract is that a claim is written down only after the
command that settles it has run, a paragraph alone is the one unrun claim left.

**What it delivers**, and both halves exist already: the corpus at a release tag
recompiled by today's compiler, so a program that stops compiling is a **broken
promise** rather than a discovery; and the trigger, since `git diff vA vB --
spec/heroes-spec.md` already moves `Y` and its emptiness moves `Z`
(`.claude/rules/records.md` § Release tags). No new verb is owed.

**The hardest sentence is a consequence of §10.** A program cannot declare the
language version it was written for — no fourth input class, and panel 056
refused a per-project file — so the promise is **one-directional**: the compiler
must not break old programs, and an old program cannot ask for an old compiler.
**And the paragraph says the deployment model out loud**: crash and be restarted,
because Part 6 refuses exceptions permanently and every abort `spec:166-169`
lists is uncatchable. Defensible as a model, indefensible as a surprise.

**Why here.** Immediately before the gate, which then publishes the `1.0.0` this
row earns.

**What it does not deliver**: the `1.0.0` tag itself, which is the author's act
on a clean `main` and never a milestone's.

**Re-ordered 2026-10-14, by the author's instruction**: from 79 to 73, closing
the consolidation block and ahead of the packages, so that the instrument this
page describes runs while the largest body of Heroes after the compiler is
written. The paragraph is still published at the gate. It carries one promise
more, written the day the history was rewritten once to keep the seed only at
the releases (M-bounded-seed): `main` and the published tags are never
rewritten again.

**Corrected 2026-10-15**: the history keeps the seed at every tag, not only at
the releases, by the author's decision of that day (M-bounded-seed's page);
the promise is unchanged.

**The order of evaluation, the first item of the next sitting on the
specification** (the author's *4a* of 2026-10-25). The specification says
nothing of the order in which a call's arguments and an operator's operands
are evaluated, while the compiler lowers them left to right
(`selfhost/ir/flatten.hero:190`, `:619` and `:639`, `selfhost/ir/inout.hero:30`,
and a record's fields in source order, `selfhost/ir/containers.hero:223`); a
program whose output depends on the order means one thing today and is
promised nothing. Panel 010 found the word ambiguous once named arguments may
be written in any order, left to right of the call or of the signature
(`docs/panel/010-repeated-mutable-arguments.md:46-47`). The promise `1.0.0`
rests on states it, so the next sitting on the specification takes it first,
priced on the reader's tokeniser with its removal or prediction named
(design.md § 1.6). A comparison read the same day: Goose's specification fixes
it in one sentence, "Evaluation order is left-to-right everywhere (operands,
arguments, field initializers ...)" (github.com/aardappel/goose,
[goose/docs/goose_spec.md](https://github.com/aardappel/goose/blob/master/docs/goose_spec.md) § 2).

## Opened 2026-10-29

**Opened by the author's instruction of 2026-10-29**, meant as: *proceed with the
directions I gave you, and do the step at this point*; on the trunk and in no lane
yet, the opening writing records alone.

**Where the promise starts, the author's ruling of the opening day**, meant as:
*that is, we will guarantee this from 1.0.0 on*, given on their question why a
language still at `0.x` should owe compatibility at all. The promise, and the
suite that makes a broken one red, start at the tag `v1.0.0`: the corpus at that
tag, and at every later `v1.*` tag, is what a compiler on `main` must still
compile. Before `v1.0.0` the suite judges nothing: it reports how many programs
of a release's corpus today's compiler refuses, and whether their fixes are
`certain` or `guess`, and it is never red (the coordinator's proposal in the
same conversation, the author's answer being to proceed). **So *What it
delivers* above, *the corpus at a release tag*, reads *at a release tag from
`v1.0.0` on***, and the example issue 1066 gave, `examples/` at `v0.2.0`, is a
change `0.x` allows and not a broken promise; the issue's comment of the same day
carries the measurement
(https://github.com/heroes-lang/heroes/issues/1066#issuecomment-6503102842).

**What it starts from**, each number its command's, 2026-10-29:

- **five releases**, `v0.1.0` to `v0.5.0` (`git tag --list 'v*'`); between each
  pair the specification moved (`git diff --quiet vA vB --
  spec/heroes-spec.md` false four times of four) and `Y` moved with it, so the
  version rule of `.claude/rules/records.md` § Release tags held at every
  release, and no patch release exists;
- **the corpus the report reads first**, the files of `examples/` holding
  `function main(`: 67 at `v0.1.0` and `v0.2.0`, 69 at `v0.3.0` to `v0.5.0` and
  on `main` (`git grep -l`). Each tag's, extracted with `git archive`, asked
  `heroes check` by today's compiler (`0.5.0`, the tree at `de2353e17`, whose
  `selfhost/`, `runtime/` and `seed/` are unchanged since): **60 of 67 refused**
  at `v0.1.0` and at `v0.2.0`, 58 of them first on `old_cell_symbol` (panel
  209's `@=`, whose fix is `certain`), one `lend_kept` (`curl`) and one
  `callback_name` (`readings`); **1 of 69** at each of `v0.3.0`, `v0.4.0` and
  `v0.5.0`, `examples/interpreter/main.hero:63`, `exit(f.code)` with an `i64`,
  `type_mismatch`, broken after `v0.5.0` by M-panic-location's `exit(code: u8)`
  (737e99535, 2026-10-26) and the one break task 2250's `i64` is predicted to
  heal;
- **`v0.2.0`'s own compiler**, built by clang from that tag's seed, checks all 67
  of its own: the 60 refusals are the language moving, not a corpus that never
  compiled;
- **the open items**: 1066 (the paragraph, the suite, the sentence § 10
  forces), 2213 (the checker's rules by churn), 2245 (three sentences of the
  specification the compiler does not keep), 2250 (`exit` takes a computed
  `i64`, panel 230's fallback), 2237 (the runtime's old entry points, after the
  release whose seed knows the new ones); 2085 and 2217 closed, the second
  leaving the order of evaluation as the first item of the next sitting on the
  specification (above).

**The order of the steps**, the coordinator's, each a commit: **2250 first**, in a
lane, since it is ratified with its condition met (decision 2241) and needs no
sitting, and since it is the one break the report reads from `v0.3.0` on; then
**2213**, a measurement; then **the suite**, reporting; then **one full sitting on
the specification**, its gate asked once of the author (CLAUDE.md § 4): the order
of evaluation, 2245's three sentences, the paragraph's words and its home, the
sentence § 10 forces and the deployment model; then its landing. **2237 waits on a
release**, the author's act, and stays on this row until one is cut, or is
re-homed at the close.

**What the steps found for the sitting on the specification**, 2026-10-29,
each a question rather than a premise, beside the order of evaluation and
2245's three sentences above:

- **§ 11 says a code outside 0 to 255 aborts, and a literal one is refused**
  (step 1, task 2250, panel 230's fallback as ratified): the compiler is
  stricter than the sentence, in the safe direction, and the sentence the
  sitting priced at +15 does not say so. Whether it should, and at what
  price, is the sitting's.
- **Whether `1.0.0` promises a diagnostic's code** (step 2, measurement 060):
  its words are no program's meaning, and most of what moves is words; a
  code is what `heroes check --json` hands to an editor or a harness, a
  program of its own whose input a renamed code breaks. If it does, the 170
  codes that moved in none of eight weeks are where the list starts.
- **The suite's corpus** (step 3): `examples/` at each release tag, every
  file with a line opening `function main(`, 69 at `v0.5.0`. The golden
  `run/` programs are valid programs too, and are not read, since before
  `1.0.0` a case is changed with the language it tests; whether they join
  the corpus from `v1.0.0` on is the sitting's.
- **The CI must fetch the tags with their history before the version is
  `1.0.0`** (step 3): the matrix checks out one commit and no tag, which the
  suite reads as `0.x` and tells; from `1.0.0` it is red there until the
  workflow fetches them, on purpose, since a suite that found no release
  would judge none and pass. A change to `.github/workflows/ci.yml`, a push,
  and so the author's.

**What the suite tells before `v1.0.0`, by the author's answer of the same
day**, meant as: *report it, saying that the error is a known case, like the
one of `@=`, and so all is well*. A refusal every code of which names a change
`0.x` made on purpose, a row of the suite's `known` table with the commit that
made it, reads "all ok"; one that does not is named, worth a look, and is
never red before `v1.0.0`. Measured with it on `v0.2.0`'s 67 programs: 60
refused, every one by five such changes (`@=`, the label rule reaching
`fail`, `lent`'s default, a callback's names, `_ =` keeping a failure), so
"all ok"; the newest release, `v0.5.0`, refuses none.

**And the author's rule of the same day** (CLAUDE.md § 2), meant as: *I do
not want to complicate the language for backward compatibility I do not need
now; an absolute rule*. No form enters the language before `1.0.0` to keep an
older program compiling: no declared version, no mode, no old spelling kept
beside the new, no deprecated alias. The sitting's shared brief carries it as
a frame no seat may argue past, and the suite's `known` table is a list of a
test and no part of the language.

## Panel 232's resolution, provisional (2026-10-29)

**The sitting**, `docs/panel/232-what-1-0-0-promises-and-what-it-leaves-out-said-before-it-binds-and-ten-sentences-the-compiler-keeps.md`,
adopted its resolution provisionally, and the author ratified it the same evening, decision 2262
(https://github.com/heroes-lang/heroes/issues/2262). Its seats and its critic found nine defects
running the sentences' shapes, 2253 to 2261, six of them `blocking`.

**The paragraph `1.0.0` rests on, in the sitting's words** (R6), kept here until M-publication-gate
publishes it in `README.md` (task 2268), and in no sentence of the specification (the
spec-warden's veto):

> **What 1.0.0 promises.** A program that compiles under Heroes 1.0.0 compiles under every later
> 1.x compiler, and does what the specification says it does. Outside it: a program that relies on
> a defect, the compiler doing other than the specification says, which a 1.x release may repair;
> a repair that keeps a program from crashing or corrupting memory, which may refuse or stop a
> program that did; what the specification leaves open, such as how deep a program may call and
> how much memory it may hold; and an error in the specification itself, which a 1.x release may
> correct.
>
> **What it covers.** The language the specification describes and the functions it names; the
> functions `hero_os.h` declares at 1.0.0, their names, their types and what each result hands its
> caller, which a 1.x release may add to but never removes, narrows or changes; and the `heroes`
> command's exit codes (0 clean, 1 the input has a diagnostic, 2 the tool could not run), its
> diagnostics' codes and `heroes check --json`'s schema, never their words. Any other C a program
> binds is outside it: its header is read again at every build, so a change in a name, a type or a
> record's fields stops the program compiling where it binds it, and what no header shows is that
> library's to promise. So is a built program meeting a newer shared library, the runtime's own
> `heroes_runtime.h`, which a program may not bind, and every name the compiler's C takes, `h_`,
> `hero_`, `Hero`, `HERO_` and `HEROES_`, which a program's own C may not declare.
>
> **It runs one way.** A program declares no version: a newer 1.x compiler reads an older 1.x
> program, and a change of meaning, a new refusal, a new built-in or a new reserved word waits for
> 2.0.0.
>
> **A Heroes program stops rather than guesses.** There are no exceptions: an error a program does
> not handle ends it at once, saying why and where, with a status that is not 0, and nothing
> catches that stop. A program meant to run for a long time runs under something that starts it
> again.
>
> **The history stays.** `main` and the published tags are never rewritten again.
>
> **2.0.0 may break this promise**, and says which programs it breaks.

**What lands in this milestone**, in one landing after the batch's repairs merge:

- ten sentences of the specification, each with the repair that makes it true: E1, D1, § 10's index of a place written with `@`, B4r
  and L2 for the order of evaluation; S6 for `<<` and `>>`; H3 for a handle given back, with defect
  2255; K1 for the stack, with defect 2253 on Darwin and on Linux; X4 and N1 for `exit` and
  `does_not_fit`; 10,343 to 10,497 real by the parts the spec-warden measured, measured again as
  they land;
- the batch, every defect open: 2253 to 2261, 2211, 2212, 2242, 2251 and 2252, fourteen;
- task 2237, now (R8);
- design.md § 4.17: a `1.x` renames no diagnostic code and withdraws none (R9);
- `.claude/rules/records.md` § Release tags: `X` moves only with a release that breaks the promise
  and says which programs it breaks (R11);
- the compatibility suite on `build --emit-c`, its corpus `examples/` and the thesis references
  (R10).

**What M-publication-gate owes before the paragraph binds**: 2263 (`hero_os.h`'s functions and
what each result owns), 2264 (the compiler's name forms reserved, P1 with its refusal), 2265 (a
stop inside a bound function names its place), 2266 (the codes listed and held by the net), 2267
(the suite runs the corpus), 2268 (the paragraph published) and 2269 (the CI fetches the tags).

**The author's three choices, answered 2026-10-29** with the sitting's ratification (decision
2262): R7, rule (A), a `1.x` refusing no program its `1.0.0` compiled; R8, yes, the rule against a
deprecated alias reaching the runtime's C names; and R12, **insertion order**: `1.0.0` promises a
map's keys in the order they were first written, so the paragraph above no longer lists a map's
order among what the specification leaves open, and the runtime and § 10 owe it (feature 2274), **in this milestone**, by the
author's instruction of the same night, meant as: *it should be done before the publication
gate, since developing the packages' core without it would be a problem; let us do it now*.

**Closed 2026-10-30**, [073](../../records/journal/073-compatibility-promise.md): panel 232's
eleven sentences in the specification at 10,514 real, each with the repair that makes it true;
the batch of nineteen defects, feature 2274 and task 2237 gated once, the full net 10,545 and 0 on
34 suites; `heroes check` on the compiler 2.0% slower, owed by
https://github.com/heroes-lang/heroes/issues/2279 on M-latency-ceiling. What it carried forward
is the `adjacent` https://github.com/heroes-lang/heroes/issues/2275,
https://github.com/heroes-lang/heroes/issues/2277 and
https://github.com/heroes-lang/heroes/issues/2278, and the paragraph's publication in
`README.md`, task 2268 on M-publication-gate.
