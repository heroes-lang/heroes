# Panel 102 — a local may take a module's name, and the wrong one compiles

Convened 2026-09-02, full panel, five seats. Opened by M-selfhost-nesting: the
milestone that drops `selfhost/`'s prefixes found the class the prefixes had
been hiding.

**RATIFIED the same day by author instruction** — *"non preoccuparti troppo dei
token, abbassa i suoi veti"* — which lowers the spec-warden's two vetoes and
overrules its zero-token clause. See § The author's verdict.

## The proposal, verbatim as briefed

A local binding may take a module's qualifier with no diagnostic. This program
compiles at exit 0 and prints `7` then `101`:

```
use mod/state          # mod/state.hero declares `function value(x: i64) -> i64`

record Box
    value: i64

function elsewhere() -> i64
    return state.value(1)      # the MODULE

function main()
    state = Box(value: 7)
    print(to_str(state.value))  # the LOCAL record's field
    print(to_str(elsewhere()))
```

`state.value` is the same text with two meanings in two functions of one file.
Both readings type-check, because a field of `i64` and a function returning
`i64` are indistinguishable at the use site. Two candidate resolutions:

- **(A) File-wide reservation.** A `use` binds a name for the whole file; no
  local binding, `@` cell, `for` name or parameter in that file may take it.
  Completes the rule the compiler already enforces for top-level declarations.
- **(B) Ambiguity at the point of use.** An error only where `X.y` is written
  and `X` is both a live local and a module binding of the file.

## What the sitting was wrong about before it started

**The brief told five judges this was an open question. It was not.** The
spec-warden found the ruling: panel 031 R3, ratified, `DESIGN-LOG:185`, which
chose the verb *binds* for the § Files and layout bullet precisely so that
`spec:99`'s "Shadowing is a compile error." would cover a local named after a
module — **for zero tokens**, in that entry's own words. And panel 031's page
carries the reductio:

> Module names enter the ordinary namespace, not a third one, otherwise
> `p = geom.Point(…)` followed by `geom = 3` is legal and `geom.f()` becomes
> ambiguous.

That sentence is this sitting's defect, written down before it was found. So the
question was never what the language should say. It is **CLAUDE.md §12**: the
spec was right and the compiler had the bug.

The convener grepped `docs/panel/` and `DESIGN-LOG.md` for `shadowed_binding`
and `module_binding_taken` and found only panel 101. Panel 031 discusses the
same thing in other words — `geom = 3`, "a third namespace" — and **a grep is
only as wide as the vocabulary of whoever runs it**. CLAUDE.md §1 already says
this about negative claims. It says it about searching the world; it is just as
true of searching the record, and this is the fifth entry in that section's
lineage.

**The compiler said it too, in two comments, for as long as anybody cared to
read them.** `module_errors.hero`'s declaration-direction diagnostic is
annotated *"Spec's shadowing rule, reached because `use` BINDS (panel 031 R3)"*.
And `selfhost/resolve_qualified.hero:96-98`, found by the ffi-pragmatist, states
the missing check as an accomplished fact:

> The second is not an ambiguity to resolve — a local named after a module is
> the shadowing error, reported where the local is bound — it simply means the
> dot beside it is UFCS.

Nothing reported it. CLAUDE.md §11's expiring premise in its worst form: this
premise never held at all.

**And a test asserted the defect.** `selfhost/resolve_qualified.hero:226`, "a
local wearing the module's name turns the dot back into UFCS", built the shape
by hand, threw away the answer `declare` gave it, and pinned the fallback as
correct. It contradicted a ratified ruling for as long as nobody read the two
together, and it is the one test (A) turns red. It is rewritten in place, under
its own history, rather than deleted.

## The verdict table

| judge | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **spec-warden** | **approve (A)** · **VETO (B)** · **VETO do-nothing** | design.md §1.3, §1.4, §1.6 · CLAUDE.md §12 · DESIGN-LOG:185, :53 | ten wordings measured; base **3663**, `w-zero` **+0**, its own `c3` **+4**, `c2` **+6**, the brief's `w-nothing-else` **+11**, `w-ambiguous` (B) **+21**, `w-explicit` **+31**. Removal available at **−11** and declined | the repair needs **exactly 1 rename across all 501 `.hero` files** and `measure` stays on its number, `heroes test selfhost/main.hero` green, **0** renames under `selfhost/` | withdraws the (A) approval and demands prose if a program is shown where `:99` plus *binds* does not decide the case; withdraws the (B) veto only if panels 031 R3 and 015 E are formally reopened |
| **llm-ergonomist** | **approve-with-conditions (A)** · object (B) · **VETO do-nothing** | the thesis; §1.3 locality | its own three tasks under each rule: (A) gives **4** diagnostics, one per binder, each on the line that introduced it; (B) gives **11 to 14** for the same four names, **2.75×–3.5×**, and not one lands on a name | 20 tasks whose natural local name is a module's last part: today **≥60%** bind the colliding name and **≥3/20** are exit-0 wrong-output with no diagnostic, at least one hanging; under (A) silent divergence **exactly 0**; under (B) **≥1**, and **≥30%** of first repairs edit the expression instead of the binder | (A) becomes a plain approve when the sentence names the **binding** namespace — declarations, parameters, `=` and `@` locals, `for` names, `match` payloads — and **flips to object if it reaches field names** |
| **ffi-pragmatist** | **approve (A)** · object (B) · **VETO do-nothing** | design.md §1.11, §4.19 · CLAUDE.md §8 | `--emit-c` md5 **identical** across baseline, (A) and (B): `1c273950f5331e18b45ed6f99deeb1d1`. Suites: baseline 535 all passed · (A) 535, **1 failed** (the test that blessed the defect) · (B) and (B') 535 all passed. Over 55 files in `examples/`: **2 files, 1 real site**; over `selfhost/`: **0** | splitting `examples/curl` into `ffi/curl.hero` plus a caller forces **zero** renames under (A), because libcurl's natural locals are `handle` and `slist`, never `curl` | withdraws the do-nothing veto if `needs_qualifying`'s fix drops to `guess` **and** a golden case with a `.fixed` sibling exists; withdraws (A) if a ladder rung needs the module's own name as a local with `as` unavailable |
| **historian** | **approve (A)** · object (B) · object do-nothing | precedent | Zig, C#, ISO 7185 Pascal for (A); C++, Nim, Ada 8.4(11), Swift for (B) | a (B) patch touching fewer than **three** dot-resolution sites ships at least one silent case — field access, UFCS call, and module qualification are three distinct resolutions of the same syntax (marked by the seat as an inference from the spec, not a measurement) | moves (B) to approve-with-conditions on any of: a primary-source Nim rule resolving this cleanly under UFCS; an accepted Zig proposal exempting `@import` bindings; or a `::`-free language that shipped (B) and kept it five years |
| **compiler-engineer** | **approve-with-conditions (A)** · **VETO (B)** · **VETO do-nothing** | design.md §1.7 · CLAUDE.md §11, §12 | (A) **3 files, 71 insertions, 1 deletion** — `resolve_state.hero` 548→601, `resolve_top.hero` 376→377, `module_errors.hero` 284→300; **55 of the 71 buy the line number, not the rule**, so (A)'s real range is **35–71** and the difference is one table on the `Resolver`. (B) **3 files, 38 insertions, 3 deletions** — cheaper in lines, and vetoed anyway | (A) is complete as landed and needs no second site: at the tag, its three modules total **≤1290** lines (measured 1278 under its own patch) and `resolve/qualified.hero` has gained **no** `used_module_named` call — `grep -c` expects 1, as on `main` today. Falsified either way, which would mean (B) arrived through the back door | **two, both measured, both honoured in what landed**: the stage must **report and then bind**, not refuse; and the diagnostic's home must not put `module_errors.hero` past 300 |

**The engineer's row above was appended after the synthesis**, which is what
panel 100 did for the same seat in the same situation — and this time the seat
was **killed by a stream watchdog at 600 s** while running the net, the failure
that cost panel 087 four of five seats. Its brief carried the cheap route and it
took the long one. It was resumed with one instruction — report what is in hand,
run nothing — and it delivered the sitting's three most useful measurements. The
lesson is the brief's, not the seat's: **a panel seat must be told what NOT to
run**, not merely offered a short path.

### The three things the engineer changed about the landing

**1. It must report and then BIND, not refuse.** Measured: refuse-and-return
leaves the name meaning the module, which is not a value, so
`examples/json/parse.hero` printed **five** diagnostics for one mistake — one
`shadowed_binding`, one `unknown_name`, three `module_is_not_a_value`. The
convener had already seen two of the five and had not drawn the conclusion.
Reporting and then binding takes it to **one**, and — the part worth the whole
row — the pre-existing test at `resolve_qualified.hero:226` then **passes
untouched**, so the repair needs no rewrite of the case that blessed the defect.
Its own reading of why the other three stages may refuse and this one may not:
the names they collide with are still values. `declare` gains a fourth arm of
the same shape as its three, which is what §11's threshold protects and is why
`resolve_state.hero` at 575 is legible.

**2. It corrected the brief's §11 argument.** The brief called
`resolve_state.hero` a knot whose seam the language forbids. Its own module doc
says the opposite — *"they are one concern… so they are one module WITHOUT a
cycle forcing it"* — so §11's language-forbids-the-seam clause does not reach it,
and the plainer half does.

**3. It found a live memory-safety defect on `main`, unrelated to this
sitting**, while building (B), and minimised it to 14 lines. `@` at a call site
on an **immutable by-value parameter** holding a refcounted field is accepted and
corrupts the heap: exit **134** with *"1 heap blocks still live at exit"*, a
two-call variant at **133** with no output, and under `--sanitize` an
**`AddressSanitizer: heap-use-after-free`** in `hero_array_decref`. Reproduced
independently by the convener the same hour. `spec:90` already forbids the
program — *"only a declared `@` name can be mutated"* — and the cause is one hop:
`resolve_walk.hero:231`'s `write_root` is the check, and `grep` gives it exactly
**one** caller, `place()` at `:206`, which handles a write **statement**. A call
argument's `@` marker never reaches it. §1.12 makes this a goal's failure rather
than a limitation, so it has its own item in `docs/work/DECIDE.md` and its own
step, because CLAUDE.md §1 says a repair is attacked at the shapes next to the
one that provoked it and here they are seven.

**And one more dead citation, inside the comment that justified the defect.**
`resolve_qualified.hero:113` cites `module_paths.last_parts_collide` as *"what
makes the answer unique"*. Panel 100 R3 deleted that rule; `module_paths.hero:90`
says so.

## Where the seats disagreed

**Only about (B), and the argument that settled it came from the seat with no
veto.** The historian found that **every language that shipped (B) also ships a
way to say "I meant the module"**: C++ and C# have `::`, Nim has always-available
`module.symbol`, and **Swift is adding `::` right now** — SE-0491, Module
Selectors, implemented in Swift 6.3 — whose motivation is this defect verbatim
in shape, with `import RocketEngine` beside a local `RocketEngine` making
`RocketEngine.Fuel` *"look for a nested type in the struct
`RocketEngine.RocketEngine`"*. Swift had also moved the **other** way in 2018
(swiftlang/swift#21378), replacing an ambiguity error with shadowing plus
qualification, because the ambiguity error broke working code. Heroes has no
`::`, so under (B) the repair is still renaming the local: **(A)'s cost, moved
from the binder to every use site**, and the spec-warden priced the wording of
that worse deal at **+21** against (A)'s **+0**.

**The brief's two precedents for (A) were both wrong and the historian struck
them.** Modula-3 §2.1 resolves by the smallest enclosing scope and Ada RM
8.4(10) lets a local homograph hide a use-visible name — both silent. The real
precedents are **Zig**, which is the closest case in existence because `@import`
binds a *value* read with `.`, whose shadowing prohibition is file-wide, and
which closed **the identical order-dependent hole as a bug** (ziglang/zig#9194);
**C#** (CS0135/CS0136/CS7039), twenty years old and preserved through a full
compiler rewrite, where the criticism that survives is about message quality and
never about the rule; and **ISO 7185 Pascal** §6.2.2, in Heroes' own ancestry,
which made "one name, two meanings in one region" impossible at block scope.

**Heroes' asymmetry is Go's asymmetry, exactly, and nobody has ever defended
it.** Go's spec states it flatly — an import name lives in the *file* block, a
top-level declaration in the *package* block, "no identifier may be declared in
both" — so a top-level clash is an error and an inner local is legal. The JLS
asserts the mirror image. Neither argues it. Both languages ship third-party
linters against the silent half, and **Go's own `vet -shadow` was withdrawn in
Go 1.12 for imprecision**, which is the strongest sourced argument against a
heuristic warning and therefore *for* an exact rule.

**What a reader expects is the losing side, and the sitting says so rather than
hiding it.** Of the five most-used general-purpose languages, four resolve this
silently in the local's favour; C# is the error. So (A) contradicts the
expectation a reader arrives with. The thesis is the reason expectation loses,
and CLAUDE.md §12's *"a refusal is held to the same standard as a feature"* is
why that had to be argued rather than assumed.

**One empty search, reported as one.** `stackoverflow.com` is unreachable from
this environment, so item 2 of the historian's brief — real questions with vote
counts — is **unmet**, and no vote count appears anywhere on this page. What
replaced it is linter-rule documentation, which is better evidence for the same
claim: pylint W0621 and W0416, Ruff A001-A004, ESLint `no-shadow` (not in
`recommended`, while `no-import-assign` **is**), go-critic's `importShadow`
(disabled by default, classed opinionated), and SEI CERT DCL51-J — a rule
written as an external standard for a language that would not enforce it.

## The finding that outlives the sitting

**`needs_qualifying`'s `certain` fix does not terminate**, and it is on the
FFI's most natural spelling. `ptr` has no fields, so `window: ptr` beside
`use ffi/window` sends every module call through a shadowed handle to that
diagnostic — whose fix prepends the qualifier the line already has. Reproduced
twice by the ffi-pragmatist and twice again by the convener:

| round | what `check --apply` writes |
|---|---|
| 1 | `window.window.destroy(handle: window)` |
| 2 | `window.window.window.destroy(handle: window)` |

CLAUDE.md §8 promises CI asserts an applied `certain` fix compiles. It does not
here, and the reason is structural: `tests/harness/suite_fixes.hero` says in its
own words *"Only `check/` cases carry `.fixed` files"*, a `check/` case is one
file, and **every** diagnostic about modules needs two files to provoke. Golden
cases naming `needs_qualifying` anywhere in `tests/golden/`: **0**, measured.
Panel 101 R4 had already ruled this class — *"a certain fix that does not
compile is the one thing a fix may never be"* — and it was live again one branch
over.

**(A) closes the reachable path**, because the local is refused at its binding
and the diagnostic is never reached; `--apply` then leaves the text unchanged.
The instrument's blindness is a decision and is filed as one in
`docs/work/DECIDE.md`.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| spec-warden | the repair needs **exactly 1** rename across all 501 `.hero` files, `heroes measure spec/heroes-spec.md` stays where the landing wording puts it, **0** renames under `selfhost/`, tests green | this milestone |
| llm-ergonomist | 20 tasks whose natural local name equals a module's last part: today **≥60%** bind the colliding name, **≥3/20** exit-0 wrong-output with no diagnostic, **≥1** hangs; under (A) silent divergence **0/20**, first-emission compile success falls **10-25 points**, and after one diagnostic round trip **≥95%** correct with the repair being `as` in **≥40%** | harness run |
| llm-ergonomist | under (B), **≥30%** of first repair attempts edit the expression rather than the binder, because the caret is on the use site and no disambiguating spelling exists | harness run, if (B) is ever built |
| ffi-pragmatist | `examples/curl` split into `ffi/curl.hero` plus a caller forces **zero** renames under (A) | M-corpus-coverage, or the next milestone that touches the FFI ladder |
| historian | a (B) patch touching fewer than **three** dot-resolution sites ships at least one silent case | if (B) is ever built |
| compiler-engineer | (A) is complete as landed: at the `m-selfhost-nesting` tag its three modules total **≤1290** lines and `selfhost/resolve/qualified.hero` has gained **no** `used_module_named` call (`grep -c` expects 1). Falsified either way, which would mean the reservation needed a use-site half after all | this milestone's close |

## What a veto would compel

The two live vetoes are both **against** the alternatives, so nothing is
compelled by them: doing nothing is vetoed by three seats and (B) by one.

Had (A) been vetoed, the compelled path was the spec-warden's own: reopen panel
031 R3 and panel 015 E by a panel rather than by a wording, because (B)
contradicts 015 E's ratified sequential-shadow rule — *"a line below must not
decide whether the line above compiles"* — which is a deliberate departure from
Java JLS §6.4 and C# CS0136 taken on §1.3 grounds.

**The llm-ergonomist's condition is the one that binds this landing**, and it is
met by construction rather than by promise: the reservation covers the binding
namespace and stops there. A record **field** named after a module stays legal,
because `b.geom` is decided by `b`'s declared type and a module never appears to
the right of a dot. That half is asserted by a test in
`selfhost/resolve/top.hero`, in the same case as the refusal, so the day it
starts refusing fields is the day the suite goes red. Its stated reason is worth
keeping: refusing a field name would make adding one `use` line edit every other
file that constructs the record — locality of reading bought by destroying
locality of repair.

## Author's verdict

**Ratified 2026-09-02**, in one instruction: *"non preoccuparti troppo dei token,
abbassa i suoi veti."*

Two effects, and the second is the one that changes the artifact. The
spec-warden's vetoes on (B) and on doing nothing are **lowered** — which changes
no outcome, since both point where the sitting was already going. Its **+0
clause is overruled**: the warden approved (A) at `w-zero`, writing nothing into
the spec because `:99` plus *binds* already forbids the program. Under the
instruction the answer inverts, and **the sentence is written** so that a reader
does not have to derive it. This is panel 100's precedent applied unchanged —
there the author said *"non mi importano i token"* and the landing form was the
dearest of six candidates, because the document teaches rather than glosses.

Three wordings measured by the convener from the repository root, base **3663**:

| after | delta | wording |
|---|---|---|
| 3669 | +6 | `Shadowing is a compile error, a `use` included.` |
| 3676 | +13 | `Shadowing is a compile error, and a `use` binds its name for the whole file.` |
| **3685** | **+22** | `Shadowing is a compile error: a `use` binds its name for the whole file, so nothing else in the file may take it.` |

**Taken: +22**, headroom 411. The `+6` figure independently confirms the
warden's `c2` measurement to the token. The reason for the dearest is the
llm-ergonomist's phase 0: reading the spec alone, it put itself at **60/40** on
whether the program was legal, and named the counter-reading available in the
same document — modules appear in type position and are never values, so a
reader may conclude two namespaces and no shadowing. A sentence that leaves a
fresh reader at 60/40 on the meaning of a two-token expression is a sentence
that has to say the rest out loud.

**What the yes does not settle**, stated rather than smoothed:

- The compiler-engineer's row and prediction, still pending, with its veto on
  implementation cost intact.
- Whether (B) was the better rule. It is refused on the historian's `::`
  argument and on the warden's §1.3 objection, and the refusal is cheap to
  overturn: **1** program in the whole repository is refused by (A), measured.
- The `.fixed` instrument's blindness to every module diagnostic, which is a
  decision with its own item and is not closed by this sitting.
