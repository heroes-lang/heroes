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
  ~2000 tokens, measured — never estimated; that is the point of the budget).
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
The assistant writes all cases; each milestone's 5 adversarial cases stay
marked `# UNVERIFIED — pending debrief` until ratified in `/debrief`; bulk
regression cases are labelled as such.

## 10. One command
Any new capability is a `heroes` subcommand or flag. Never a second binary,
never a script, never a Makefile. Declared exception with an expiry date:
`cargo build`/`cargo test` build the compiler until the fixpoint (M8c).

## 11. Language and conventions
**Everything written is English** — code, comments, docs, commits, verdicts.
Conversation with the author is Italian. `Heroes` in prose, `heroes` for the
binary, `.hero` for files. ASCII-only syntax. Bowie belongs in prose and
packaging, never in error text or library names; the site's register and its
rules live in `site/README.md` § Style guide.
**Code is written to be read** (author instruction 2026-08-04): files stay
short and single-concern — split a module before it passes ~300 lines; every
file opens with a module doc stating its role and citing its design.md
sections; comments teach the invariant and the why, never the diff. The
author must be able to open any file and read it without drowning.

## 12. Precedence when artifacts disagree
Spec beats compiler (the compiler has the bug). Measurement beats opinion —
including the author's and the panel's: comprehension is the objective (§1.1)
and it is measured, not asserted.

## 13. Where not to go
Performance (a non-goal, never a justification). A standard library. Anything
in design.md Part 6. Anything in Part 7 before the closure list compiles
itself.

## 14. Documentation duty + git
A step is not done without a commit (`M<n> step <k>: <what>`). Per milestone:
journal (3 sections) + one story beat in `docs/book/beats.md` + a tag (pushed
`--follow-tags`) + ROADMAP status. Per decision: a DESIGN-LOG line. The
milestone-close checklist lives in `/step` — its only copy. The repo pushes
to `origin` (github.com/giuseppearici/heroes-lang). Hard stops that remain:
publishing the site or anything else outward-facing, and destructive ops.

## Commands
```
cargo build && cargo test        # build the compiler, run all tests
cargo clippy                     # Cyclone-rule enforcement
./target/debug/heroes doctor     # toolchain check
./target/debug/heroes <cmd>      # the one command (grows per milestone)
```
