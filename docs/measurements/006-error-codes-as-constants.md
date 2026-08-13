# 006 — the error code as a `constant`: the operator lost its sites rather than passing them

Date: 2026-08-13 · during M-program-corpus · **the awkward outcome panel 034 R4
named in advance, measured.**

## Provenance

| what | value |
|---|---|
| compiler | `9e5c258` plus this commit |
| spec | sha256 `8825dc10f94b678d…` — measured max **2974**, unchanged by this run |
| corpus | `examples/` — **44** programs (the twelve-file gallery, and the nine program directories: `adventure`, `assembler`, `calculator` ×5, `curl`, `json` ×3, `logs` ×3, `markdown` ×4, `maze` ×3, `spreadsheet` ×3, `sqlite`, `todo` ×2) |
| operators | `harness/mutations/operators.md`, twelve — unchanged |
| arms | `check` and `check --permissive` — unchanged |
| command | `heroes mutate` |

## The question

Panel 034 R4 asked for one change and one measurement. The change: **error codes
become `constant`s across the corpus**, compared as `store.ERR_UNKNOWN_ITEM`
rather than as bare literals — zero spec tokens and zero compiler lines, because
a typo in a name is `error[unknown_name]` today, locally, with the owning module
named. The measurement: *re-run `heroes mutate` and say what happened to
`typo-code`'s row, **including the awkward possibility that it loses its sites
rather than passing them***.

It lost its sites. That is the whole result, and the panel is owed the credit for
predicting it before anyone ran anything.

## Result

| row | before | after |
|---|---|---|
| `typo-code` mutants | **88** | **3** |
| `typo-code` killed | 0 (0%) | 0 (0%) |
| `typo-ident` mutants | 3216 | **3343** |
| `typo-ident` killed | 3203 (100%) | 3330 (100%) |
| total mutants | 5549 | 5591 |
| total killed (`check`) | 5127 (**93%**) | 5254 (**95%**) |
| total killed (`--permissive`) | 4541 (83%) | 4668 (84%) |

**85 sites moved from an operator that catches 0% to one that catches 100%.**
`typo-code` mutates a string literal in `fail("code", …)` or in the `e.code ==
"code"` that reads it back; once the code is a `constant`, there is no literal at
either end, so the operator finds nothing to do and `typo-ident` — which mutates
a *name* — finds two more sites per code instead. The mistake class did not
become detectable. It became a **different mistake class**, one the compiler
already refuses.

The two-point rise in the headline is real and is not the point; per panel 011 a
per-operator rate is the measurement and the headline is never pooled. The row to
read is `typo-code`'s, and the honest reading of `3 mutants, 0 killed` is *the
hole is still there and almost nothing is standing in it any more*.

## The three that are left, and why they stay

All three are in `examples/gallery/03-fallible.hero`, which is the file that
teaches `fail`/`ok`/`?`/`.must()`. The gallery is one idea per file — that is
what it is *for* — and putting a `constant` in front of the code would put a
second idea into the file a reader meets `fail` in. So the last three sites stay
literal on purpose, and `typo-code` keeps a place to fire.

It is worth being exact about what that leaves: the gallery is a **teaching**
corpus and the nine program directories are the **program** corpus, and this
change is about the second. A program that fails is a program whose codes another
program reads back; a nine-line example that fails is a sentence.

## What the change cost

Nothing the compiler could see. **Zero spec tokens, zero compiler lines**, and no
test changed behaviour — 116 corpus tests green before and after, in all three
configurations. What it cost in the source is 61 `constant` declarations across
18 modules, and three of them are read **across a module boundary**:
`program.ERR_BAD_REGISTER`, `program.ERR_BAD_NUMBER`, `sheet.ERR_BAD_RANGE`,
`parse.ERR_UNEXPECTED_END`, `parse.ERR_UNCLOSED_PAREN`.

That is worth recording because those five sites **did not compile four hours
earlier**. A qualified `constant` read was refused with *"`program` names a
module, not a value"* until it was fixed in this same milestone (step 6,
`fixedbugs_a_constant_is_read_across_a_module`). Panel 034 R4 was written on
2026-08-12 naming `store.ERR_UNKNOWN_ITEM` as the shape it wanted, and the shape
did not work; nobody found out until a program needed it.

## What this does not settle

The hole is still open, and it is the same hole `004` measured with 25 mutants
and reported at zero. A code the *runtime* produces (`missing_key`) and a code a
handler compares against are still two strings that agree by convention. What
changed is that in the corpus the convention now has a name, and a name is
something the resolver checks.

The remaining question is whether the language should carry a typed error
(design.md Part 8 wart 5) — and `004`'s conclusion stands: it loses on §4.12's
positive rule as well as on simplicity, and the answer is a `constant`, not a
feature. This measurement is that answer applied, and its price is now known:
five qualified reads and a compiler defect that had to be fixed first.
