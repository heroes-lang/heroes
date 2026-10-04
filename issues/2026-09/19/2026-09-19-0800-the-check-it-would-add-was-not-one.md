# The check it would add was not one

2026-09-19. Panel 165, at M-declared-extents step 1, on the one row of the chain
scheduled behind a measurement rather than behind a decision.

## The decision

| | |
|---|---|
| date | 2026-09-19 |
| decision | **route 6 is refused as spelled**, on two vetoes; the milestone's work becomes the repair of three defects the sitting found, and **route 12** — a header `constant` as the extent — is queued with its measurement already run |
| reason | the sentence route 6 rested on is false. C erases a parameter's extent (C11 §6.7.6.3p7), so route 6's check compares the author's declaration to the author's argument, while route 4's field check is against the header. Route 6 would REMOVE a clang-verified extent and put two author-written numbers in its place |
| design.md § | §1.12 (robustness, rank 3), §1.6 and Principle 0, §4.19 |
| panel | 165, provisional — author ratification pending |
| ratification | queued as `panel 165` in `docs/work/DECIDE.md` |

## Falsified three times, with three instruments

| seat | instrument | result |
|---|---|---|
| ffi-pragmatist | `_Generic` on the function's address | `long (*)(const char *)` **passes**, `long (*)(const char (*)[8])` **fails** |
| compiler-engineer | `__builtin_types_compatible_p` | `void f(int8_t[8])` and `void g(int8_t*)` are **one type** |
| coordinator | clang's AST, both dump forms | `f_ptr` and `f_arr8` are both `{"qualType": "char *"}` |

**And the inversion**: route 4 already checks a field's extent against the header
— `error[ffi_field_type]: clang read the header's struct and the field
disagrees`, run on the live tree.

## The measurement the sitting was queued behind, on four legs

| | Darwin | Linux ×2 | Windows UCRT+shared |
|---|---|---|---|
| `.h` walked | 3120 | 5404 / 5411 | 345 |
| spelled as an array | **82** | **182**, the same SET | **0** |
| fixed **and** byte-typed | 12 | 31 | 0 |

Windows spells none, and structurally: the UCRT assembles declarations from
macros taking the type and the name as separate arguments. A second, independent
text instrument over all 66 UCRT headers agrees.

**`L_tmpnam` is 1024 on Darwin, 20 on glibc and 260 on Windows**, each compiled
and run on a real machine, the third on the box on 2026-09-19. Odin's shipped
binding says 15; it is stale, and the historian had named that failure mode a few
paragraphs before its own example proved it.

## Three routes nobody had listed, and the critic found two

**Route 12 — a header `constant` as the extent — dissolves the sitting's
headline.** Every seat and the coordinator treated `i8[SL_NAME_LEN]` as a wall
because the parser refuses it. The wall is the Heroes grammar and nothing else:

```c
_Static_assert(sizeof(((struct sl2 *)0)->name) == SL_NAME_LEN, "...");  /* passes */
_Static_assert(sizeof(((struct sl2 *)0)->name) == 20, "...");           /* fails  */
```

The macro expands in the emitted C **against the real header**, so a named extent
is portable *and* header-checked. That is the property three seats proved route 6
cannot have, and it kills the coordinator's own headline finding — a declared
extent cannot be portable **as a literal**; as a name it can.

**Route 13** derives the extent from the field; **route 14** tightens route 3's
own check and is adopted, because it closes defect 063.

## Three defects, and the register said zero

- **063** — an overstated extent on `f.ptr()` **corrupts memory**: `n: 64` over an
  `i8[8]` field moved the sibling `id` from 7 to `1094795585` (`0x41414141`) at
  exit 0, ASan stack-buffer-overflow on both READ and WRITE. Shipped eight
  commits ago at `ef7b013b`, as defect 061's own repair.
- **064** — `t.name == u.name` on two fixed fields: `check` 0, `build` an internal
  error. The class is every `.binary`; the unguarded sibling `.call` is held shut
  only by the two refusals route 6 exists to remove.
- **065** — C writes into a binding declared `=`, no `@` anywhere: `72` → `65`,
  exit 0. Falsifies `spec § 3` and `spec § 5`, and a shipped golden demonstrates
  the write as correct — on a `@` binding, which is why it never showed.

## The coordinator wrote six briefs and the seats corrected five things in them

`check/ffi.hero` is **275 `code_lines`**, not the 384 of `wc -l`. Route 3 shipped
after M-readable-bytes, not at it. The warden was sent to merge into a sentence
that **does not exist** — § 13 enumerates fields, never parameters — and the same
non-existent sentence was quoted to the **llm-ergonomist as specification text**,
contaminating the one seat whose input is controlled. And *"172 extern
functions"* was wrong twice: five came from `archive/bootstrap-rs/`, which every
brief forbids, and `sort -u` counts names rather than declarations. Properly:
**316 declarations, 0 array-spelled** — a stronger conclusion from a denominator
nobody had checked.

This is CL-077's own shape arriving again: a brief is the one document in a
sitting nobody is assigned to check, because the seats check the world against the
brief and nothing checks the brief against the world.
