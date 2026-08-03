# CLAUDE.md — operating contract for the Heroes project

Heroes is a small compiled language designed so that **every plausible LLM
mistake is a compile error**. Bootstrap compiler in Rust, backend emits C11
compiled by clang, self-hosting is the v1 finish line (fixpoint on generated
C). `design.md` is the source of truth; the approved plan lives at
`docs/panel/000-plan-review/`. One person is learning compilers through this
project — the process rules below are teaching apparatus, not bureaucracy.

## 1. Re-read protocol — what never to trust from memory
- Read `spec/heroes-spec.md` in full at the start of every session (~1500
  tokens; that is the point of the budget).
- Reach `design.md` **by grep**, never from a remembered summary. Any asserted
  design rule must cite its section; an uncitable rule is a guess.
- Session start: `git log --oneline -10`, DESIGN-LOG tail, last journal entry.
  Session end: write open questions + next action into the journal entry.

## 2. Principle 0 (necessary-not-sufficient)
The language is finished for v1 when it can compile itself. A form enters v1
if the compiler needs it (the closure list in the plan) **or** it provably
serves the thesis (measured Part 11 effect, or a §1-derived argument the panel
accepts). Neither → it waits, regardless of elegance.

## 3. Part 0's three rules (teaching protocol — non-negotiable)
- **Predict before implementing, uncued.** The author's prediction goes in
  `docs/journal/NNN-prediction.md`, countable and falsifiable, **committed
  before any src/ change for that step**. The assistant states only the goal
  and input — never the expected output — until the prediction is committed.
- **The author writes the tests.** 5 adversarial golden cases per milestone
  come from the author. Assistant-proposed cases are marked
  `# UNVERIFIED — author must confirm` and bulk regression cases are labelled.
- **Author-first diagnosis.** On any failure, post the raw symptom (golden
  diff, clang error) and STOP until the author writes a hypothesis and names
  the file. Then explain before fixing.
- One function per milestone is written by the author; the assistant reviews.
- Each journal closes with the author's explain-it-back (≤10 lines, from
  memory) and 3 spaced questions from ≥2 steps back.

## 4. Panel triggers (path-based, mandatory)
Convene `/panel` before changing: `spec/**`, `design.md`, surface syntax or
semantics (`crates/heroes/src/{lexer,syntax,types}/` behaviour, not internals),
a diagnostic *class*, or architecture (backend, IR, tool surface). Everything
else is implementation and needs no panel. No design change lands without
`docs/panel/NNN-*.md` + a DESIGN-LOG line + its own commit citing the verdict.

## 5. The Heroes subset of Rust — the Cyclone rule
References only as function parameters, never in structs or return types.
Enforced by `clippy.toml` (no `Box`/`Rc`/`Arc`/`RefCell`/`HashMap`/`HashSet`)
and `#![forbid(unsafe_code)]`. `BTreeMap`/`BTreeSet` only — iteration order is
a fixpoint requirement. Iterator/`Option` closures are fine (expressions, not
stored state); *stored* closures are not. Owned data everywhere; indices, not
references, for links. Every necessary violation carries `// PORT-DEBT:
<reason>` — the count is the distance from self-hosting and must not ratchet up.

## 6. Nim: copy the surface, never the implementation
`importc`-style FFI, per-module cache, `nim r` → `heroes run`: yes.
Macros, templates, effect systems, style-insensitive identifiers, a separate
package binary: never. (Nim's compiler is ~150k lines — the face not to copy.)

## 7. Generated-C rules
C11; `int64_t`/`double`/`bool`; `#include "heroes_runtime.h"` (clang
type-checks every runtime call). `#line` on source-line *change*, restored to
the generated file around synthetic code; `--emit-c --no-line` for emitter
debugging. Arithmetic aborts via `__builtin_*_overflow` — never C UB. One
`goto`+label per basic block, explicit entry `goto bb0` (spike 2 finding), all
locals hoisted to the prologue. `hero_unreachable()` at every
type-system-proven-unreachable point. Compile flags:
`-Wall -Werror=return-type -Werror=uninitialized -fno-strict-aliasing`.
Every name through the mangler (`h_<module>_<name>[_<typehash>]`; fields,
variant cases and labels too; `extern` FFI names pass through unmangled by
design). **The double-emit determinism test stays green at all times.**

## 8. Error discipline
Every `Diagnostic` carries `Fix`es tagged `certain | guess`; only `certain` is
machine-applicable. Golden convention: `x.hero` + `x.expected` (+ `x.fixed`
where a certain fix exists — CI asserts the applied fix compiles). Errors are
a deliverable, not plumbing: they carry everything needed to fix the program
without opening another file (design.md §4.17).

## 9. Golden discipline
`UPDATE_GOLDEN=1` never turns a red test green without the diff being read and
quoted in the commit body. It is **forbidden in `tests/golden/check/`**.

## 10. One command
Any new capability is a `heroes` subcommand or flag. Never a second binary,
never a script, never a Makefile. Declared exception with an expiry date:
`cargo build`/`cargo test` build the compiler until the fixpoint (M8c).

## 11. Language and conventions
**Everything written is English** — code, comments, docs, commits, verdicts.
Conversation with the author is Italian. `Heroes` in prose, `heroes` for the
binary, `.hero` for files. ASCII-only syntax. Bowie belongs in prose, never in
error text or library names.

## 12. Precedence when artifacts disagree
Spec beats compiler (the compiler has the bug). Measurement beats opinion —
including the author's and the panel's: comprehension is the objective (§1.1)
and it is measured, not asserted.

## 13. Where not to go
Performance (a non-goal, never a justification). A standard library. Anything
in design.md Part 6. Anything in Part 7 before the closure list compiles
itself.

## 14. Documentation duty + git
A step is not done without: journal entry, DESIGN-LOG line, **story beat**
(one line in `docs/book/beats.md` — this project ends in a mini-book about
the journey, and the beats are its raw material; see `docs/book/README.md`),
commit (`M<n> step <k>: <what> (docs/journal/NNN)`). One tag per milestone
(pushed with `--follow-tags`). A milestone closes only when goldens pass
(ASan-clean where applicable), the determinism diff is empty, the prediction
predates the implementation, and the mutation drill ran. The repo pushes to
`origin` (github.com/giuseppearici/heroes-lang).

## Commands
```
cargo build && cargo test        # build the compiler, run all tests
cargo clippy                     # Cyclone-rule enforcement
./target/debug/heroes doctor     # toolchain check
./target/debug/heroes <cmd>      # the one command (grows per milestone)
```
