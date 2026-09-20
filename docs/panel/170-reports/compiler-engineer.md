# Panel 170 — compiler-engineer

**Every number below was produced by a command run 2026-09-20, Darwin arm64, in
a copy at `/private/tmp/.../scratchpad/tree` built from `git ls-files` at
`65e10d28` (step 16), compiler `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`, 4.6 s.**

**A correction I owe first.** My first copy was `cp -r` of the working tree and
its `.git` read `7267db07` (step 10), six commits behind. Every reproduction was
re-run against a step-16 compiler after `seed/heroes.c` was diffed byte-identical
to main. This is `.claude/rules/verification.md` § *The compiler that judges is a
build artifact* arriving in a panel seat.

---

- **verdict**: veto
- **section**: design.md §1.7 (core plus elaboration) for the cost; design.md
  §4.17 (*"every error carries all the context needed to fix it"*, line 2001) for
  the veto.
- **needed_for_self_hosting**: no (Principle 0). `grep -c 'extern "' selfhost/*.hero`
  is the closure question and no compiler module binds a retaining C parameter.
  It enters, if it enters, on the design.md Part 11 limb.

## implementation_cost

The word is **contextual, not a keyword** — measured, and it removes the largest
line item the brief assumed. `grep -n "borrows\|acquires\|consumes"
selfhost/keywords.hero` returns **nothing**, and so does `counted_by\|owned`.
**Lexer cost: 0 lines.** `ast.Param` is a record, not a variant, and it has
**two** construction sites (`selfhost/ast.hero:653`, `selfhost/parse/members.hero:222`),
both single lines: **exhaustive-match cost: 0 lines.**

I wrote the patch — `keeps` as a bool on `ast.Param`, a `borrows`-shaped marker,
two printer suffixes, a `retaining_landing` sibling of `uncounted_landing`, one
sweep clause, one diagnostic — and measured it in `tests/harness/suite_layout.hero`'s
own unit (`code_lines`, `suite_layout.hero:525`; my awk replica reproduces panel
169's 1174/524/298 exactly).

| file | before | after | ceiling | |
|---|---|---|---|---|
| `selfhost/keywords.hero` | 231 | 231 | 300 | contextual word, no lexer change |
| `selfhost/parse/members.hero` | 298 | 307 | 300 | **over +7** |
| `selfhost/ast.hero` | 524 | 527 | 525 | **over +2** (DECIDED row) |
| `selfhost/print/fmt.hero` | 1174 | 1178 | 1175 | **over +3** (DECIDED row) |
| `selfhost/print/dump.hero` | 207 | 211 | 300 | fits |
| `selfhost/check/lend_extent.hero` | 216 | 241 | 300 | fits |
| `selfhost/check/lending.hero` | 293 | 306 | 300 | **over +6** |
| `selfhost/lend_errors.hero` | 277 | 288 | 300 | fits |

**Total: 69 code lines, 7 files.** Spec: `heroes measure --refresh` on the draft
CParam plus two sentences reads **8201 -> 8263 real**, +62 of 2039 headroom
(draft reverted; `diff` against main identical).

**The ceilings are priced, not pleaded** (brief rule 3, CL-012). Two of the four
breaches vanish for free and two are a row edit:

- **`members.hero` 307 -> 210**, by moving the six marker functions into a new
  `selfhost/parse/marks.hero` at **111**. Measured: the block is 98 code lines in
  the instrument's unit. The seam names a concern — *what a declaration may say
  about a C parameter* — and it is the same seam `selfhost/check/lend_decls.hero`'s
  module doc already describes on the checker side.
- **`lending.hero` 306 -> 294**, by putting the clause in `lend_extent.hero`
  (241 -> 255) and calling it in one line. Both then fit under 300. No split at all.
- `ast.hero` and `fmt.hero` are **DECIDED** rows, and `suite_layout.hero:117`
  records four such rows moving before. 525 -> 527 and 1175 -> 1178, named in
  advance as that file asks.

**Which layer, and it is not panel 169's answer.** The AST, and the reason is
measured: `selfhost/ir.hero` and `selfhost/ir/*.hero` carry **zero** FFI mark
fields — all four grep hits for the mark words are English prose in comments, and
there is no `record Param` in the IR at all. Moving the rule to the IR would
first require plumbing six marks through lowering, which does not exist. Panel
169's worry was the AST's *write* set; this rule reads the *argument* set, and
`record_arguments` (`lending.hero:104`) is an **exhaustive match** over
`a.exprs[].kind` with every non-argument arm listed, so the language itself
closes it.

**Which stage.** `check`. `lending.lent_only_into_c` is called from
`selfhost/checker.hero:85`, and I ran the sibling clause that already exists:
`heroes check` on a `ptr` lend with no `counted_by` gives `error[field_lend_uncounted]`,
**exit 1**, no `build` needed.

**How many programs it refuses: zero**, and that is a deduction from the patch
rather than a suite run — `retaining_landing`'s first gate is `landed.keeps`, and
no declaration in the tree writes the word. Collision risk is nil: 315 `keeps`
occurrences in `.hero` files, every one prose or a test name, none in a parameter
mark position.

## By §1.7's own test, this is not core

design.md §1.7: *"Anything in the core must be implemented in the type checker
**and** the lowering **and** the backend. Anything that is sugar is erased by one
function on the way into the IR."* The IR carries no marks (measured above), so
`keeps` never reaches the lowering or the backend. It is a **frontend-only**
construct at 69 lines against Pascal-P4's ~4000. **The ceiling is not my
objection and I will not pretend it is.**

## argument

The mark is sound, cheap and frontend-only: 69 code lines, no lexer, no IR, no
backend. I veto the framing, not the word. Run today: **072 is already closed by
two tags and no new code** — distinct tags give two `type_mismatch` refusals — and
**a lend into a freeing callee exits 134 with empty stderr while `keeps` never
fires**. Worse, my own diagnostic's repair, *"lease instead"*, **is** defect 070
when the parameter frees: I ran both halves. One mark whose fix is another
defect's crash breaches §4.17. Split retention from give-away before adopting
either.

## What I ran, and it says the four defects are three things

| shape | check | build | run | stderr |
|---|---|---|---|---|
| **066**, `cstr` lend, C parks the address | 0 | 0 | 0, prints `112` from a dead frame | 0 B |
| 066 under `--sanitize` | 0 | 0 | 0, prints `112` | **0 B, no ASan line** |
| **070**, lease + C frees + `end_lease` | 0 | 0 | 133 | 0 B |
| 070 variant, lease + C frees, no `end_lease` | 0 | 0 | 134 | 0 B |
| **new shape**, `.cstr()` **lend** + C frees | 0 | 0 | 134 | 0 B |
| **072**, two families on `tag void` | 0 | 0 | 0, prints `done` | 0 B |
| 072 with **two distinct tags** | **1** | — | — | two `error[type_mismatch]` |
| control: lease never ended, C does not free | 0 | 0 | 134 | **111 B, names the count** |

`--sanitize` is real: on a deliberate `free(p); p[0]=1` it prints
`AddressSanitizer: heap-use-after-free`. On 066 it printed nothing.

Three findings the sitting does not have:

1. **072 is not about retention and needs no mark.** Give the two families two
   tags and the checker refuses the swap today. 072 is `one_tag_one_type`'s doing
   and nothing else: the tag decides the **C spelling** (`handles.hero:113-124`)
   and the **declaration** decides Heroes type identity, which is why the
   diagnostic names `Arena` and `Heap`. The cheapest close is a condition inside
   a **17-line function**, `one_tag_one_type` at `selfhost/check/decls.hero:313`,
   in a file at **219 of 300**. Priced, not built — I could not rebuild from
   `selfhost/`, and the one command that settles it is the full `check` suite
   after that condition lands.
2. **070's empty stderr is the language's own report being pre-empted.** The
   run-time lease check works — the control printed *"panic: 1 lease(s) never
   ended"*, 111 bytes — and produced **0 bytes** in 070 because C's `free` aborts
   before `main` returns. So 070 is not a missing diagnostic; it is a working
   diagnostic the corruption outruns. That is the strongest argument in this
   sitting for a **compile-time** refusal, and it is an argument for a *different*
   mark from `keeps`.
3. **A fourth shape nobody filed**: a plain `.cstr()` lend handed to a freeing C
   function. `check` 0, exit 134, stderr 0 B. `keeps` does not describe it — C
   does not keep those bytes, it frees them.

## Why this is a veto and not a price

`keeps` says *C retains what it is handed*. Its only honest repair is a lease,
and my draft note says so. On a parameter that **frees**, that repair is defect
070: I ran it, exit 133, stderr 0 bytes. So a single mark covering both facts
hands the author a `certain`-shaped fix that is the next crash — against design.md
§4.17 and against `selfhost/check/marks.hero:18`'s own quotation of design.md
Part 6, *a tag nobody reads is a comment that looks like a guarantee*. A mark that
**is** read and answers the wrong question is worse than one nobody reads.

The refusals differ in the direction that matters, which is why one word cannot
carry both:

- **retains** — refuse a lend, **admit a lease** (066, 068);
- **frees** — refuse a lend **and refuse a lease**; admit only bytes from the
  library's own allocator, which is the route
  `docs/measurements/037-...md` already measured working (070).

I am not asking the sitting to drop the mark. I am asking it not to adopt one
word for two facts, and not to count 072 among them.

## prediction

Falsifiable, at the close of the milestone that lands this sitting's resolution:
**if one mark is adopted for both facts, `docs/work/DEFECTS.md` will still carry
an open give-away defect** — 070 or its successor number — **and `heroes check`
on the six-line reproducer at `070`'s entry will exit 0.** If instead retention
and give-away get two marks, that same reproducer exits **1**. The instrument is
`./heroes check` on that file and the `**OPEN: N**` banner; both exist today.

Second, on the ceiling, so the sitting can hold me to a number:
**`tests/harness/suite_layout.hero`'s `DECIDED` table will read
`selfhost/ast.hero 527` and `selfhost/print/fmt.hero 1178`, `selfhost/parse/members.hero`
will be absent from the table at 210, and a new `selfhost/parse/marks.hero` will
measure 111 ± 15 in that suite's unit.**

## condition

The veto lifts on either of two showings:

1. **A run** in which one mark's diagnostic gives a repair that is safe on a
   freeing parameter and safe on a retaining one — `heroes check` exit 0 and the
   built program exit 0 with a clean `--sanitize` on **both** of my `g.hero` and
   `k1.hero` shapes. I could not construct it; I say that as a question about my
   own vocabulary, not a proof.
2. **A resolution that separates the two facts** and drops 072 from the sitting's
   scope. Then my verdict is **approve** at the cost above, because 69 frontend
   lines for a class with no run-time instrument at all is the cheapest thing on
   this table.

The one thing I could not run: the patched compiler against the golden trees.
Rebuilding from `selfhost/` is minutes and the brief forbids it. The zero-refusals
claim rests on the patch's first gate, not on a suite.
