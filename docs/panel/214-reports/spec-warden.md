# Panel 214, the spec-warden's report

Written 2026-10-14 from 01:04 in my copy `.claude/worktrees/scratch-panel-214/spec-warden`, detached
at `8c8783ac7`, its compiler built there from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`). Every count below names its command. Offline only: every
`heroes measure` here ran with `env -u ANTHROPIC_API_KEY`, so no `--refresh` and no paid run.
Written as I go; the verdict block is at the end.

## 0. The ceiling, read today

`sed -n 253,410p docs/design.md` (§1.6, reached by `grep -n "^###" docs/design.md`): **10240**,
measured by `claude-opus-5` through `POST /v1/messages/count_tokens`; the payment rule
unconditional (a named removal, or a registered prediction naming an instrument that exists).

## 1. The baseline, measured in my copy

`env -u ANTHROPIC_API_KEY ./heroes measure spec/heroes-spec.md`:

```
claude-legacy      7519
cl100k_base        7655
maximum            7655   a lower bound, not the reader's tokeniser
real              10048   claude-opus-5, 2026-10-13 — the binding number
Headroom: 192 against the 10240 ceiling — but the FFI floor mortgages 60 of it (panel 030 R3),
so what is measured against the ceiling is 10108 and the check goes red at 10240.
```

So the real room under the ceiling is **132** (10240 - 10048 - 60), as the shared brief says. That room
is not an entitlement: design.md §1.6's payment rule is unconditional, so a sentence inside the 132 owes a
named removal or a registered prediction exactly as one past it would. `DELTA_GATE` is 50 vendored tokens
per commit (`tests/harness/suite_spec.hero:200`).

## 2. What the record already says (searched, so a silence is named)

- The spec: `grep -n -i "internal error\|blame\|hero_\|reserved\|exit 1\|exit 2" spec/heroes-spec.md`
  prints nothing. The spec states no exit code and no diagnostic class anywhere; every refusal it states
  is *a compile error*. Who is told is not a kind of content the document carries today.
- design.md §1.6 defines the document as *syntax, semantics, built-in library*. Exit codes are the tool's
  (`.claude/rules/cli-surface.md` § One command: exit 1 *the input has diagnostics*, exit 2 *the tool
  could not run*); the exit-1 class of a clang failure is `.claude/rules/c-boundary.md` § A clang failure
  that the author's own extern caused, twelve members, with its narrowing at line 133.
- Prior sittings: `grep -l -i "collid.*runtime\|runtime's names\|header may not declare\|may not declare"
  docs/panel/*.md` finds only panel 035 (`main` may not declare a result, unrelated);
  `grep -rn "7\.1\.3" docs/panel/ docs/design.md` finds nothing. No sitting I could find with those
  words reserved the compiler's C names to it. Issue 2046 (`gh issue view 2046`, read only) is open,
  `class:improvement`, `area:spec`: *a rule nobody needs to be right today*.

## 3. The finding that prices route C: the tree already declares the compiler's prefixes, and builds

The tracked program-own headers: `git ls-files '*.h' | grep -v -E '^(runtime|seed|archive|docs)/'`, **447**
(446 under `tests/golden/`, 1 under `examples/gallery`). Of those, **59** mention an identifier matching
`\b(hero_[a-z0-9_]+|Hero[A-Z]\w*|HERO_[A-Z0-9_]+|h_[a-z0-9_]+)\b`; they mention **82** distinct such
names, of which **59 names appear nowhere under `runtime/`** (`grep -r -q -w <name> runtime/`): the
program's OWN C names, carrying the compiler's prefix. The headers holding one: **37**, of which
**11 under `unsupported/`** (the planted collisions and typos of 2045, 2049, 2052) and **26 that the tree
expects to compile**: 12 `tests/golden/run/`, 12 `tests/golden/check/`, 2 `tests/golden/surface-fixtures/`.

Examples, read: `fixedbugs-094-mutex.h` defines `typedef struct {...} hero_mutex;`, `#define
HERO_MUTEX_INIT`, `hero_lock`, `hero_unlock`; `fixedbugs-143-*` define `hero_fd_new`, `hero_bits_set`,
`hero_wexitstatus`; the handle cases define `h_open`, `h_close`, `h_close_v2`, `h_value`, `h_ref` (`h_open`
and `h_close` in 11 headers each).

Built in my copy, `./heroes build <case> -o build/b_<case>`: `fixedbugs-094-the-systems-mutex-initialiser-
builds-a-record`, `fixedbugs-168-a-whole-record-beside-a-same-named-header`, `handle-set-any-releaser-named-
ends-the-life`, `fixedbugs-143-system-macros-through-functions-of-the-programs-own`, `fixedbugs-568-the-first-
group-s-header-is-read-before-the-c-library`, `ffi-owned-cell-is-freed`: **six of six exit 0**.

Two readings, both measured:

1. **The mistake route C names is plausible, and its shape is in this tree**: twelve `run` headers,
   written in this project, where the assistant writes the cases (CLAUDE.md § 3), over many defects (094,
   143, 152, 168, 411, 427, 568, ...), gave their own C helpers the `hero_`, `HERO_` or `h_` prefix. None
   collides, because none chose a name the runtime or the emitter actually declares.
2. **A prefix reservation would make working programs illegal**: the 12 `run` programs at least (six
   built above), and the 12 `check` and 2 fixture headers by rule (`check` compiles no header, so whether
   a refusal would reach them depends on where it is enforced; unrun). Enforced at build (route J's
   scan), it refuses a correct program, which `.claude/rules/verification.md` § Bounded discovery classes
   `blocking`. Not enforced, it is a prohibition in *the prompt* that no instrument checks, against the
   thesis's own words (CLAUDE.md, line 3: *every plausible LLM mistake is a compile error*).

## 4. Every candidate sentence, measured

Each draft is the spec with one edit in § 13's first paragraph, made by a script asserting its anchor
matches once (`build/drafts/` in my copy), measured by `env -u ANTHROPIC_API_KEY ./heroes measure
build/drafts/<draft>.md`; an unmodified copy reads 7519 legacy / 7655 cl100k, identical to
`spec/heroes-spec.md`. Where it stands: after *one defining `_GNU_SOURCE` first.* (C1, C2, B1, E1, F1);
C3 is merged into the order sentence itself, as `.claude/rules/spec-shape.md` § Where a rule lives
prefers (*merging beats appending*).

| draft | route | the sentence | legacy | cl100k | real, ESTIMATED |
|---|---|---|---|---|---|
| C1 | C | *A header shares its unit with the compiler's own C names, `hero_*`, `Hero*`, `HERO_*` and `h_*`, and may declare none of them.* | +39 | +39 | 46 to 51 |
| C2 | C, with the critic's residue (N9) | *A header shares its unit with the compiler's own C names, `hero_*`, `Hero*`, `HERO_*`, `HEROES_*`, `h_*` and `main`, and with `<stdint.h>` and `<math.h>` read after it: it may declare none of the first, and the rest only as C does.* | +78 | +75 | 88 to 99 |
| C3 | C, merged | *C reads a module's headers after the compiler's own C names, `hero_*`, `Hero*`, `HERO_*` and `h_*`, which none may declare, and in the order its groups are written: ...* | +33 | +33 | 39 to 43 |
| B1 | B | *A header clang refuses is the program's error, told at its group; a refusal only in the compiler's own C is the compiler's.* | +31 | +31 | 37 to 41 |
| E1 | E | *A header that compiles alone and is refused beside the compiler's own C names is told as a collision, both declarations named.* | +27 | +27 | 32 to 35 |
| F1 | F (a pointer only; the rename itself needs none) | *A header may not declare a name C reserves.* | +10 | +10 | 12 to 13 |
| none | A, D, H, I, J, K | no sentence | 0 | 0 | 0 |

**The real column is an inference, not a measurement**: `--refresh` is a paid run no seat starts
(`.claude/rules/spec-shape.md` § How a change to the document is made: the vendored figure is written
as a **lower bound, in those words**). The range is the vendored cl100k delta times two ratios this
tree carries: the whole document's 10048 / 7655 = 1.31, and the one priced delta I could find, panel
204 R1's § 13 order sentence, *+34 cl100k, +40 real* on its approved `--refresh` (`git show 5e6a8c7f4`,
its body), 1.18.

**Against the ceiling**: every draft fits the 132 real tokens of room, C2 the worst at 33 to 44 left.
**No budget veto is available on any of them.** C2's +75 cl100k passes `DELTA_GATE` (50), so its commit
body would owe what paid for it.

**Against the spec's own instruments**: each draft put in place of `spec/heroes-spec.md` in my copy,
`./heroes run tests/harness/main.hero -- ./heroes spec` into `build/spec-<draft>.txt`, then the
identical base copied back (`git status --porcelain spec/` empty after). Base: **31 passed, 0 failed**.
Every draft: **27 passed, 4 failed**, the four being `budget`, `spendable`, `real` and `ledger`, the
pinned count and its digest that any edit moves (read: *the spec measures 7665 and the record says
7655*). `shape`, `named`, `rejected`, `inventory` and `anchors` pass for all six: no draft breaks a
form check, so none is priced up for that.

## 5. Principle 0, and which document owns the rule (brief item 2)

**No form enters by any route**: not one draft adds syntax, a keyword or a built-in. So the only
question Principle 0 asks here is whether a SENTENCE serves the thesis (design.md §1.0: *a measured
effect on the Part 11 metrics, or an argument derived from Part 1 that the panel accepts*). The brief
pre-answered the first half; the critic flagged it (N16.4). I answer it from the documents:

- **Which C names collide is a fact of the implementation, not of the language.** The runtime's names
  are design.md §3.1's mangling bullet (*runtime names are `hero_*`; ... The rule lives in one place
  (the mangler)*) and §4.20's; the unit's layout, *its prefix* of the compiler's own headers before the
  groups, is design.md §4.19 (*A module's unit opens with the compiler's own headers, its prefix*;
  read at lines 2477 to 2525). Written into the spec, those names would become language: a renamed
  runtime function would be a spec change, and by `.claude/rules/records.md` § Release tags a `Y`
  release (`git diff vA vB -- spec/heroes-spec.md` not empty). Route F, renaming them into C's reserved
  space, would falsify C1 to C3 the day it landed.
- **What the language does own is already said**: § 13 says a group names a header, that *C reads a
  module's headers in the order its groups are written*, and that clang checks the bindings against
  it. A header C refuses refuses the program; nothing new needs stating for that to be true.
- **Who is told is a diagnostic's class and an exit code**, which the spec carries for no other
  refusal (section 2 above). B1 would be the document's only sentence about blame: a new kind of
  content outside design.md §1.6's *syntax, semantics, built-in library*.
- **Owners**: the class membership and who is told, `.claude/rules/c-boundary.md` § A clang failure
  that the author's own extern caused (the sitting's rule is a thirteenth member or a replacement of
  several); the unit's order and any move of the runtime (H) or isolation (D), design.md §4.19, which
  already says *a change to it changes what programs mean, so it is a sitting's*; the message's content,
  design.md §4.17 (*every error carries all the context needed to fix it*), which is where a reader meets
  the runtime's conflicting declaration, at the moment it matters and at zero prompt cost.

**Route C fails Principle 0 on a measurement, not an opinion** (section 3): its sentence, read under
CLAUDE.md § 12 (*Spec beats compiler: the compiler has the bug*), makes every tracked program whose
header declares its own `hero_*`, `HERO_*` or `h_*` name a wrong program the compiler accepts, a
`blocking` defect by `.claude/rules/verification.md` § Bounded discovery: **12 `run` programs at least,
six of them built at exit 0 in my copy today**. Enforcing it refuses them; not enforcing it leaves the
prompt stating a rule the compiler breaks. Narrowing it to the names the runtime actually declares is
unlistable in the document: 197 declarations and 40 macros (the critic's N9 count, not re-run here).

**A reading for route D from my seat** (an inference, D is not built): § 13's sentence *one that needs
another's names comes after it* (defect 563, panel 204 R1, priced +40 real) and six `run` cases of
defect 563 (`ls tests/golden/run | grep fixedbugs-563`, e.g. *two defaults mean what their written order
says*, where `K` is 5 or 9 by order) hold only if a later group's shim still reads every earlier
group's header. A shim per group that does not would falsify a spec sentence and owe a new one.

## 6. The exit-code contract, route by route (brief item 3)

`.claude/rules/cli-surface.md` § One command, read: *exit 0 clean, exit 1 the input has diagnostics,
exit 2 the tool could not run.* Three exits, no fourth.

- **A, C, E, F, H, J** end at exit 1 or 2 and can each honour it; C does so by decree, at the price in
  section 5.
- **B** reads *whose text it is*, which `c-boundary.md:133` refuses as the narrowing; and through
  `#line` a location in the program's own `.hero` is the emitter's (the critic's N11, run), so B can
  answer exit 1, *the input has diagnostics*, for a refusal of the compiler's own C: the contract broken
  in the false-blame direction the brief weighs most.
- **I** is the one route whose exit 2 means what the contract says by construction: the unit refused
  without the header too is the tool's failure, whatever clang's location says (the critic ran three
  ablations at exit 0).
- **K** cannot escape the binary: a message that assigns no blame still exits 1 or 2, so K moves the
  blame from the words into the exit code rather than removing it.
- **D** removes the collision, so a refusal left in a shim is the header's alone, exit 1, and the
  contract holds, at the cost section 5 names.

## 7. §1.2: does any sentence pay for itself?

A sentence costs its real tokens on **every** prompt, FFI or not; it saves a correction round
(500 to 2000 tokens, design.md §1.2) only on a program that writes its own header AND declares a name
the compiler's C declares. Break-even for C1 at 46 to 51 real: a collision in **2.3% to 10.2%** of
prompts. What the tree shows (an inference from its population, not a rewrite rate, which needs metric
2 and a model in the loop):

- 54 example programs (`git ls-files 'examples/*/main.hero'`), 21 `.hero` files with an `extern`, **one**
  program-own header (`examples/gallery/13-lease.h`), declaring no compiler-prefixed name;
- the 12 `run` headers that DO carry the compiler's prefixes collide with nothing (section 3);
- the five collision defects of the night, 2044, 2045, 2049, 2051 and 2052, were each *found by the
  agent repairing* the previous one (`gh issue view <n>`, the Origin of each, read only): shapes
  attacked beside a repair, not met in a program written for its own purpose.

No natural collision is measured in this tree; the break-even is not shown to be reached. And the
sentence's preventive power is partial at best: C1 covers 2045, 2049's typo and emitter-name shapes and
2052's suggestion; 2044, 2049's `<math.h>`, 2051 and `main` are C's own rules (C11 7.1.3 reserves the
library's external names), which a writer of C is held to whatever this document says. Meanwhile the
diagnostic names the runtime's declaration at exit 1, run in my copy on `hs` (copied from
`.claude/worktrees/scratch-typed-inspection/rep/`, `./heroes build build/rep/hs.hero`): exit 1,
`error[ffi_header_refused]`, *clang adds, at the Heroes runtime's own `hero_str_len`, which every unit of
this program declares before any group's header*, and `fix (guess): rename hero_str_len in hs.h, a name
the Heroes runtime declares first in every unit`. That is C1's content, delivered to the one program
that needs it at the moment it needs it, at zero prompt cost; the round the sentence would save is a
round §4.17 already makes short.

## 8. The route I would adopt from this seat, and the one I refuse

- **Adopt**: any of I, E (once its probe is defined, the critic's N4), H or D that the compiler-engineer
  and ffi-pragmatist find sound, **ruled at 0 spec tokens**, the rule written in
  `.claude/rules/c-boundary.md` and, where the unit's layout moves, design.md §4.19; the collision's
  message carrying both declarations under design.md §4.17. My seat prefers I's discrimination for the
  contract's sake (section 6), and has no ground to rank I, E, H and D on cost.
- **Refuse**: route C as a spec sentence, in all three drafts, refused by the 12 tracked `run`
  programs (six built at exit 0 here) it would make wrong under CLAUDE.md § 12. B1 and E1 are refused
  on Principle 0: no measured thesis effect, and a kind of content the spec carries nowhere else. F1 is
  not owed: C's own reservation binds a header whatever the spec says.
- **Issue 2046**: settled by the ruling as *the specification stays silent; the diagnostic says it*,
  rather than by a sentence.

## 9. What I could not run

- **The real price of any draft**: `--refresh` is a paid run, not started. The real column of section 4
  is an inference from two ratios.
- **The rewrite rate**: §1.2's second factor needs metric 2 with a model in the loop; no paid run. The
  break-even of section 7 is arithmetic over an unmeasured rate.
- **Whether a model reading `hero_*` in the spec would try to bind the runtime's own names**, as ten
  examples do through `extern "hero_os.h"` (`hero_thread_spawn`, which the spec never names): a question,
  unrun.
- **The 12 `check` and 2 fixture headers under an enforced reservation**: where enforcement would sit
  decides whether `check` sees them; unrun.
- **Routes D, H, I, J as built compilers**: not built in my copy; my readings of them are of the
  documents and the critic's runs.

## Verdict

- `verdict`: **veto** of route C's sentence (C1, C2, C3); **object** to B1, E1 and F1; **approve** a ruling
  that lands at 0 spec tokens. Provisional only in its token prices (vendored measured, real estimated);
  the veto rests on no count.
- `section`: design.md §1.0 (Principle zero's burden), §1.2, §1.6 (the payment rule); CLAUDE.md § 12;
  `.claude/rules/cli-surface.md` § One command for the exit codes.
- `spec_token_delta`: measured cl100k 7655 before; after C1 7694 (+39), C2 7730 (+75), C3 7688 (+33),
  B1 7686 (+31), E1 7682 (+27), F1 7665 (+10); legacy 7519 before, +39/+78/+33/+31/+27/+10. Real 10048
  pinned before; after, ESTIMATED 46-51, 88-99, 39-43, 37-41, 32-35, 12-13 more. Recommended: 0.
- `removal`: none owed at 0. Were a sentence adopted, nothing named: and that would be a problem.
- `needed_for_self_hosting`: no.
- `argument`: The compiler's C names are the implementation's (design.md §3.1, §4.19, §4.20); written
  into the prompt they become language, and under CLAUDE.md § 12 a prefix reservation makes 12 tracked
  `run` programs, six built at exit 0 here today, wrong programs the compiler accepts. I found no
  collision outside attacked shapes (54 examples, 12 prefixed `run` headers searched); the night's five
  were found beside repairs. The spec states
  no exit code for any other refusal, so B1 adds a content kind §1.6 does not define. The diagnostic
  already names the runtime's declaration (§4.17). Every route but C rules at zero tokens; C cannot pay
  its burden.
- `prediction`: (1) The sitting's ruling lands with `heroes measure spec/heroes-spec.md` reading cl100k
  **7655** and the `spec` suite **31 passed, 0 failed** at the landing batch's gate. (2) The 12 `run`
  cases of section 3 build at exit 0 under whichever route lands, scored by the `run` suite at that gate;
  a route that turns one red has made a prefix a rule the tree already breaks. (3) If any draft lands
  anyway, its `--refresh` real delta is between 1.15 and 1.35 times its cl100k delta (C1: 45 to 53),
  scored by `heroes measure --refresh` in its landing commit.
- `condition`: a measured natural collision rate above C1's break-even (2.3% of prompts at 2000 tokens a
  round), from metric 2 or the task suite, AND a reservation that leaves the 12 `run` programs legal;
  or an author decision that the runtime's namespace is part of the language (for instance, `hero_os.h`
  documented in § 13), which would give the names a home there before any rule about them.

My copy `.claude/worktrees/scratch-panel-214/spec-warden` was removed at the end of this pass. Its
`build/drafts/` are reproduced from section 4's sentences at the anchors named there; `build/rep/hs.*`
from `.claude/worktrees/scratch-typed-inspection/rep/`.
