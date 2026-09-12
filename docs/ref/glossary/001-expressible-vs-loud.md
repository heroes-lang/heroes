# 001 — Expressible vs loud: the two jobs a language rule can do

**Origin.** Debrief of 2026-08-04, on panel 008 (escape sequences). The
question was which half of the amendment made `print("a\nb")` print two
lines — the five escapes, or reserving the backslash. The instructive
answer is that they are *different jobs that arrived in the same commit*,
and conflating them makes it impossible to argue about either.

## The distinction

A change to a language can do one of two independent things.

**Make something expressible.** It widens what the language *can say*.
Before: no program could put a newline inside a string literal. After: it
can. This is governed by Principle 0 (§1.0) — the compiler needs it, or it
provably serves the thesis. The failure it fixes is *impossibility*.

**Make a mistake loud.** It narrows what the language *accepts*. Before:
`"\d+"` was four silent bytes. After: a compile error carrying its repair.
This is the thesis itself (§1.1: every plausible mistake is a compile
error). The failure it fixes is *silence*.

They point in opposite directions — one adds power, the other removes
permission — which is why a single amendment can carry both and still need
two separate arguments.

## The four worlds

Escapes make the grid concrete:

|                          | no escape set | escape set |
|--------------------------|---------------|------------|
| **backslash ordinary**   | `"a\nb"` is four characters, silently (Heroes until 2026-08-04) | C: `"a\nb"` works, but `"\q"` silently means `q` |
| **backslash reserved**   | `"a\nb"` is a compile error — loud, but a newline stays unexpressible | Heroes today: `"a\nb"` works, `"\d+"` errors |

The panel voted the two axes separately, which is the proof they are
separable: the ffi-pragmatist approved the escape set but **objected to
reserving the backslash as an endpoint** (bottom-left leaves `printf("%d\n")`
unexpressible, so §4.19's ladder fails); the spec-warden wrote the opposite,
"approve D even if C were somehow contested", because reservation is the
half that does thesis work.

## Why it matters when arguing about a proposal

Ask which job a change is doing before pricing it:

- An **expressiveness** change is priced against Principle 0's burden of
  proof and against the spec budget. Silence about it is *incompleteness*.
- A **loudness** change is priced against §1.2's cost formula and the
  measured error classes. Silence about it is a *silent-wrong-output class*,
  which never triggers a correction round-trip at all — it just ships.

Panel 009 turned this into an allocation rule for the spec budget: spend
tokens where a wrong guess is **silent**, not where the compiler already
makes the mistake loud.

## The boundary

Loudness can only be applied to what is *syntactically* wrong. `"C:\temp"`
compiles today and means `C:<TAB>emp`, because `\t` is a legal escape and
no lexer can read intent. That is not a gap in the chosen set — enlarging or
shrinking the set cannot reach it. Catching it needs a third mechanism
entirely (raw string literals: Go's backquotes, Rust's `r#"…"#`), which is
not v1 material. Recorded as design.md Part 8, wart 15.

**Rule of thumb:** if a proposal claims to do both jobs, make it say which
one each half does. Two jobs, two arguments, two prices.

## See also

- `docs/panel/008-escape-sequences.md` — the session, and the correction
  that found the boundary.
- `docs/panel/009-spec-budget-2000.md` — the allocation rule derived from
  this distinction.
- design.md §1.1 (the thesis), §1.0 (burden of proof), §1.2 (cost formula).
