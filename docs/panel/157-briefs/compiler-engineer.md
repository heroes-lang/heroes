# Panel 157 — compiler-engineer brief

Read `docs/panel/157-briefs/00-shared.md` first. **This is the SOUNDNESS LANE**:
you and the ffi-pragmatist only. The lane was chosen because `--emit-c` is a tool
surface with **no spec token** — the specification is the language and does not
mention the flag — and no diagnostic class. What the lane gives up is recorded in
the synthesis: the llm-ergonomist would have had only the spec to read, which is
silent here, and the warden a token delta of zero.

You judge the ceiling (design.md §1.1, §1.7) and implementation cost, and you
have a veto on soundness.

## Where everything is

| what | where |
|---|---|
| the `--emit-c` early return | `selfhost/cli/compile.hero:233-265`, and its own comment at `:240-241` |
| the round | `selfhost/cli/assemble.hero`, its head paragraph is the whole design |
| who asks for a second round | `selfhost/cli/produce.hero:197-222`, `struct_tags` |
| how a refusal is read as a tag request | `selfhost/emit/ffi_tag.hero`, `needs_struct` |
| the reproducer, already in the tree | `tests/golden/surface-fixtures/structtag/main.hero` |
| the suite whose premise is at stake | `tests/harness/suite_emission.hero:70`, `:190`, `:206` |

## What you are asked to price, measured and not estimated

1. **The round inside `--emit-c`.** How many lines, and what does it cost a
   caller who asked for text and did not want clang run? Note `heroes test` also
   takes the fused-text path (`wants_text` at `:233-238`), so a change there is
   not confined to one flag — say whether it reaches `test` and what that does.
2. **The refusal instead.** `--emit-c` exits 2 when the program declares a tagged
   handle, naming why. Cheaper, and it removes a capability — price both and say
   which you would take.
3. **A route nobody has listed.** CLAUDE.md § RUN IT says a recommendation is a
   claim about the option SET. Two the coordinator noticed and did not price, and
   neither is endorsed: emitting the C **twice** and writing whichever compiles,
   which needs clang anyway; and emitting a `#define` or typedef shim at the top
   of the artifact that makes both spellings legal, which needs no clang at all
   but adds a line to every emission. Ask what would have to be true for a fourth
   to exist.
4. **R3, and it is the one that decides the shape.** If `--emit-c` runs clang,
   what happens to `suite_emission.hero`'s fourteen wrong-FFI cases whose whole
   value is that they emit where a build refuses? Read that suite before
   answering; the premise is stated in its own comment.
5. **R4.** Nothing in the tree compiles what `--emit-c` wrote. What is the
   cheapest instrument that would have caught this, and where does it live —
   `emission`, `surface`, or somewhere else?

## The seed, which is why this is not only an artifact

`--emit-c` is how `seed/heroes.c` is made, and CI compares the committed seed to
what today's source emits on every push. `selfhost/` declares no
`record … tag` today, measured, so the seed is safe **now**. Say whether your
recommendation keeps it safe when that stops being true, because that is the day
this defect becomes a bootstrap failure rather than an artifact bug.

## Your verdict owes

A verdict per R1-R4, measured costs, a falsifiable prediction naming the
instrument that would score it, the condition under which you would change your
vote, what you left UNRUN, and whether you cast your veto.

Write your report to `docs/panel/157-reports/compiler-engineer.md` **first**.
