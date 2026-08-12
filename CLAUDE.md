# CLAUDE.md — operating contract for the Heroes project

Heroes is a small compiled language designed so that **every plausible LLM
mistake is a compile error**. Bootstrap compiler in Rust, backend emits C11
compiled by clang, self-hosting is the v1 finish line (fixpoint on generated
C). `design.md` is the source of truth for the language; the milestone chain
lives at `docs/ROADMAP.md`; the process lives in the skills (`/step`,
`/debrief`, `/panel`, `/where`) — each rule is written in exactly one place,
everything else cites it. One person is learning compilers through this
project; comprehension is the objective, but it runs **on the author's
clock, never as a gate** (rule 3).

## 1. Re-read protocol — what never to trust from memory
- Read `spec/heroes-spec.md` in full at the start of every session (budget
  4096 tokens, measured — never estimated; that is the point of the budget).
- Reach `design.md` **by grep**, never from a remembered summary. Any asserted
  design rule must cite its section; an uncitable rule is a guess.
- Session start: `git log --oneline -10`, DESIGN-LOG tail, ROADMAP status.

## 2. Principle 0 (necessary-not-sufficient)
The language is finished for v1 when it can compile itself. A form enters v1
if the compiler needs it (the closure list) **or** it provably serves the
thesis (measured Part 11 effect, or a §1-derived argument the panel accepts).
Neither → it waits, regardless of elegance.

## 3. Process: implement first, understand on the author's clock
The assistant implements autonomously and never stops mid-step to ask.
Everything that once gated progress (predictions, spot-checks, golden
ratification, failure diagnosis, drills) becomes an entry in
`docs/debrief/QUEUE.md`, processed in `/debrief` sessions when the author
chooses. Learn-first (questions before implementing) only when the author
explicitly asks before a step. The executable protocol lives in `/step` —
its only home. Lessons stay impersonal: shapes and rules, never scores.

## 4. Panel — path-based triggers, asynchronous
Convene `/panel` before changing the *language*: `spec/**`, design.md Parts
1–11, surface syntax or semantics (`crates/heroes/src/{lexer,syntax,types}/`
behaviour, not internals), a diagnostic *class*, or architecture (backend,
IR, tool surface). The teaching process (design.md Part 0, the skills) is
amended by author instruction, no panel. The panel never blocks: the
synthesis adopts the most conservative resolution `provisional — author
ratification pending` and queues the decision; the author's verdict is
appended when given. No design change lands without `docs/panel/NNN-*.md` +
a DESIGN-LOG line + its own commit citing the verdict.

## 5. The Heroes subset of Rust — the Cyclone rule
References only as function parameters, never in structs or return types;
owned data everywhere, indices for links; `BTreeMap`/`BTreeSet` only;
iterator/`Option` closures fine, *stored* closures not. Enforced by
`clippy.toml` (the reasons live there) + `#![forbid(unsafe_code)]`. Every
necessary violation carries `// PORT-DEBT: <reason>` — the count is the
distance from self-hosting and must not ratchet up.

## 6. Nim: copy the surface, never the implementation
`importc`-style FFI, per-module cache, `nim r` → `heroes run`: yes.
Macros, templates, effect systems, style-insensitive identifiers, a separate
package binary: never.

## 7. Generated-C rules
C11; `int64_t`/`double`/`bool`; `#include "heroes_runtime.h"` (clang
type-checks every runtime call) and one `_Static_assert` on
`HERO_RUNTIME_ABI`, so a decoy `runtime/` cannot silently replace the
contract. `#line` when an instruction's line differs from the **current
effective line** (`#line N` anchors the *next* line), restored to the
generated file around synthetic code — the restore carries the printer's own
output line count. Emitter debugging is the test helper's job: `--no-line`
is refused by §10's stopping rule (panels 016, 020). Arithmetic aborts via
`__builtin_*_overflow` — never C UB — and `%` is guarded like `/`
(`INT64_MIN % -1` does not trap on arm64). `INT64_C(n)` for every `int`
literal. One `goto`+label per basic block, explicit entry `goto bb0` (spike 2
finding), a label **only where an edge targets it**, all locals hoisted to the
prologue, and a unit-typed temporary never declared at all (`void t0;` is a
hard error). An `@` parameter is a pointer parameter (§4.8's copy-out is
`*p_l = l;`). `hero_unreachable()` at every type-system-proven-unreachable
point. Compile flags: `-Wall -Werror=return-type -Werror=uninitialized
-Werror=format -Werror=conditional-uninitialized -fno-strict-aliasing`; a clang
failure is exit 2 and says the *compiler* is wrong, **with one named exception**
(author instruction 2026-08-12, panel 036): the `_Generic` return assertion §4.19
emits per `extern` exists to fail when the *author's* declaration disagrees with
the real header, so it is exit 1 and a `ffi_return_type` diagnostic on the
`.hero` line — `emit/ffi.rs` matches only the assertion messages this emitter
writes, and every other verdict on generated C is still the compiler's.
**A refcounted slot is the
one exception to the no-initialisation rule** — zero-initialised so cleanup is
unconditional, with `ptr == NULL` as the non-value every runtime entry point
rejects (panel 021). `--sanitize` adds `-fsanitize=address,undefined`, which
catches use-after-free and double-free; **leaks are caught by
`hero_runtime_check_leaks()`**, because ASan's leak detector does not exist on
Darwin arm64. A generated `eq` or `hash` walks **fields, never bytes** (padding
makes two equal records hash differently, silently), `hash` is never null, and
COW is **one unshare per step** of a mutated place — one at the primitive lets a
nested store alias, measured, with every instrument in this project reporting
success (panel 022). Every name through the
mangler (`h_<module>_<name>[_<typehash>]`; fields, variant cases and labels
too; the module component sanitised to `[A-Za-z0-9]` so the first `_` ends it;
`extern` FFI names pass through unmangled by design, and `extern` reaches no
binary before M-ffi-ladder because only the `#include` verifies it). **The double-emit
determinism test stays green at all times**, and the emitted C never mentions
the output path.

## 8. Error discipline
Every `Diagnostic` carries `Fix`es tagged `certain | guess`; only `certain` is
machine-applicable. Golden convention: `x.hero` + `x.expected` (+ `x.fixed`
where a certain fix exists — CI asserts the applied fix compiles). Errors are
a deliverable, not plumbing: they carry everything needed to fix the program
without opening another file (design.md §4.17).

## 9. Golden discipline
`UPDATE_GOLDEN=1` never turns a red test green without the diff being read and
quoted in the commit body. It is **forbidden in `tests/golden/check/`** and in
`tests/golden/ir/`. The assistant writes all cases; each milestone's 5
adversarial cases stay marked `# UNVERIFIED — pending debrief` until ratified in
`/debrief`; bulk regression cases are labelled as such.

**Every diagnostic is annotated in the source that provokes it** — `#~ <code>`
for this line, `#~v <code>` for the next — *in addition to* the `.expected`
snapshot. rustc's rule and rustc's reason: the redundancy exists because
snapshots are auto-generated and absorb mistakes, and because the annotation
shows where the span points without opening a second file. It is what turns the
paragraph above from a convention into an invariant: a regenerator can rewrite
`x.expected`, but it cannot invent an annotation in `x.hero`. A **fixed defect
gets a case named after it**, carrying symptom, cause and date (Go's
`test/fixedbugs`); every **verifier check has a test that makes it fire**
(LLVM's `test/Verifier`); and an invariant that must hold on every accepted
program is asserted over `heroes mutate`'s corpus rather than over cases somebody
thought of (`llvm-opt-fuzzer`'s `verifyModule`).

## 10. One command
Any new capability is a `heroes` subcommand or flag. Never a second binary,
never a script, never a Makefile. Declared exception with an expiry date:
`cargo build`/`cargo test` build the compiler until the fixpoint
(M-selfhost-fixpoint).

**The stopping rule** (panel 016): a capability enters the surface only if the
fixpoint invocation, the golden harness or the Part 11 harness must type it, or
it has a measured Part 11 effect. Its shape is then mechanical — a **subcommand**
if it answers a different question (a different artifact class), a **flag** if it
changes how one question is answered about the same input, and **nothing** if two
existing invocations already compose to it. A new top-level verb needs a *proven
overload* of an existing one, which is what every recorded split was (`git
checkout` → `switch`/`restore`, `go get` → `go install`) — never a new capability.
**The contract**: the artifact on stdout, diagnostics on stderr; exit 0 clean · 1
the input has diagnostics · 2 the tool could not run. Inspection is `--dump-<stage>`
(§3.5's own list); `--json` says *how* to print, never *what*; a mutating flag is
`--in-place`; `--emit-c` is an output, not a dump. One argv table parses and
prints the help, so they cannot disagree.

## 11. Language and conventions
**Everything written is English** — code, comments, docs, commits, verdicts.
Conversation with the author is Italian. **One declared exception** (author
instruction 2026-08-11): the two books, M-journey-book and M-guide-book,
are written in **Italian and English**, neither a machine translation of the
other. The author studies from the Italian, so where the two diverge the Italian
is fixed to be clearer rather than the English to be more faithful — and both are
in the plain register of the `/where` skill, which assumes zero compiler
knowledge. `Heroes` in prose, `heroes` for the binary, `.hero` for files.
ASCII-only syntax. Bowie belongs in prose and
packaging, never in error text or library names; the site's register and its
rules live in `site/README.md` § Style guide.
**Code is written to be read** (author instruction 2026-08-04): files stay
short and single-concern — split a module before it passes ~300 lines; every
file opens with a module doc stating its role and citing its design.md
sections; comments teach the invariant and the why, never the diff. The
author must be able to open any file and read it without drowning.

**A narrowing asks the value, never the world** (author instruction
2026-08-12, sweep 001). A filter, an allow-list of kinds, or a `_ =>` arm is a
decision, and its correctness rests on something. Rest it on a fact about the
value in hand — *this* expression's extent, *this* declaration's fields — never
on a premise about the world around it ("a multi-line list is the only value
that does not end where it started"; "nothing reads this payload"). A fact
about the value cannot expire. A premise about the world expires **silently**,
and the comment justifying it goes on reading as correct, because the argument
is still valid and only the premise died. Two of sweep 001's twenty were
exactly this, and both had been read and agreed with — which is why a
convention about comment style would not have caught either. Where a premise
is unavoidable, two things are owed: write it as a **falsifiable claim**, not a
justification; and give it **a test that fires when it dies**, whose failure
message names what depends on it (`a_declared_type_cannot_contain_a_type_parameter`
is the shape — one premise, three dependants in three modules, one test). And
put the fallback in the **loud** direction: `_ => hero_unreachable()` beat
`_ => false` by a whole class of defect at M5c, and D3 paid for the same lesson
twice.

## 12. Precedence when artifacts disagree
Spec beats compiler (the compiler has the bug). Measurement beats opinion —
including the author's and the panel's: comprehension is the objective (§1.1)
and it is measured, not asserted.

## 13. Where not to go
Performance (a non-goal, never a justification). A standard library. Anything
in design.md Part 6. Anything in Part 7 before the closure list compiles
itself.

## 14. Documentation duty + git
A step is not done without a commit (`M-<name> step <k>: <what>`). Per milestone:
journal (3 sections) + one story beat in `docs/book/beats.md` + a tag (pushed
`--follow-tags`) + ROADMAP status. Per decision: a DESIGN-LOG line. The
milestone-close checklist lives in `/step` — its only copy. The repo pushes
to `origin` (github.com/giuseppearici/heroes-lang). Hard stops that remain:
publishing the site or anything else outward-facing, and destructive ops.

**Milestone identifiers are names, not numbers** (author instruction 2026-08-12;
panel 030 R7 as amended — the argument lives there). This is the algorithm's only
home; `docs/ROADMAP.md` § The names carries the map and cites this.
- **Two words**, `M-<what-it-delivers>`, hyphenated and lowercase after the `M-`;
  the tag is the same string lowercased (`m-ffi-ladder`).
- **Name the deliverable, never the area** — the area must stay free for the second
  milestone that touches it, and one already exists: `M-module-namespace` and
  `M-separate-compilation` are both about modules. A one-word name appropriates a
  topic, and an identifier must make no claim a later milestone can falsify.
- Prefer a phrase the ROADMAP entry or the milestone's own journal slug already
  uses over an invented one.
- **An id is never renamed once it is in the record.** A milestone that changes
  shape gets a *new* id; the old one is retired in § The names.
- **Order lives in the ROADMAP's order table and nowhere else** — the id claims
  nothing about position. `git tag --list --sort=creatordate` gives the chronology.
- An id never reaches a diagnostic or any user-visible output (§8, asserted by
  `emit/tests/gate.rs`).
- **Appending to a dated record uses that record's vocabulary**, with the new name
  in parentheses on first use — `scored at M8a close (M-module-namespace)`. The
  record is never rewritten: `docs/panel/`, `DESIGN-LOG.md`, `docs/journal/`,
  `docs/measurements/`, `docs/defects/`, `docs/book/beats.md`, `tests/golden/`,
  every commit subject and the twelve legacy tags keep the numbers.

## Commands
```
cargo build && cargo test        # build the compiler, run all tests
cargo clippy                     # Cyclone-rule enforcement
./target/debug/heroes doctor     # toolchain check
./target/debug/heroes <cmd>      # the one command (grows per milestone)
```
