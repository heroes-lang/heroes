# Panel 083 — the pointer that could not be read, and the one that read too much

**Date**: 2026-08-16 · **Session type**: full panel, five judges, five briefs ·
**Status**: `provisional — author ratification pending`

## The question, and the one the sitting actually answered

Convened on: a C struct member declared **pointer-to-const** is `heroes check`
exit 0 and `heroes build` **exit 2** — *"internal error: compiling the generated
C failed"* — the compiler blaming itself for the author's own `extern`
declaration, which is what CLAUDE.md §7's named exception exists to prevent.

The sitting found something worse on the way, and it is the headline: **the one
spelling that did compile reads past the object.**

## The coordinator's brief was wrong three times and the record says so

1. **The scale.** The brief said *"33 of 271 pointer members are pointer-to-const
   (12%)"*. My AST walk tested `'const' in head` as a **substring**, and
   `sqlite3_index_constraint` contains the letters *const*. It was counting type
   names. Corrected mid-sitting and sent to three seats.
2. **The class was scoped to fields.** It is not. The **result** path is the
   bigger half and the brief never mentions it — which made the brief's own
   proposal a repair without its adjacent shape (CLAUDE.md §1, by name).
3. **`const char *` already works** and the brief did not know: `cstr` *is*
   `const char *` in C, so nothing is discarded. That is why most of the raw
   count is not the class.

Four counts were offered across two sittings — 39, 33, 8, 37 — and **every one
was wrong**, including both of mine. The number that stands was obtained by
asking clang the exact question the emitter asks (`void *v = p->m;`) rather than
by any regex:

| | probed | discard | of those `const char *` | **broken** |
|---|---|---|---|---|
| fields | 261–357 | 30–57 | 22–37 | **8** |
| pointer-returning functions | 172 | 49 | 32 | **17** |

Two seats converged on those two numbers independently. The results include
**`sqlite3_column_blob`** and **`sqlite3_column_text`** — §4.19 ladder rung 3's
own *"read a result"*.

## Verdict table

| Judge | Verdict | Rests on | Cost / delta | Prediction | Condition |
|---|---|---|---|---|---|
| **ffi-pragmatist** | **veto (a)** · adopt **(b)** narrowed to where a `ptr` is materialised from C | §1.11, §1.12, §4.19 rung 3 | (b) prototype: corpus **97/97**, `cargo test` **573/573**, 14/14 sqlite3 const returns bind, full blob round-trip | under (b) a `sqlite3_column_blob → sqlite3_bind_blob` round-trip of a 4-byte blob with an embedded NUL exits 0 | withdraws the veto for a program that under (b) corrupts memory **unreachably today**; tried three ways and could not build one |
| **compiler-engineer** | **object**, veto reserved for a `Ty` variant | §4.19:2098 (`const` is filed under Part 7 item 10), §1.7 | **(a) +38/−3** for half the class · **(b) +15/−3**, `emit/` goldens byte-unchanged · (c) needs a 2nd pointer type, 23-site precedent | at M-ffi-ladder close the fix is ≤25 lines confined to `emit/`, and `Ty` still has one opaque-pointer variant | vetoes the moment any resolution adds a `Ty` variant, a keyword or a spec type name for `const`. **If (b) lands it owes a golden with a falsifier** — the compiler has no warning channel |
| **spec-warden** | **object** on scope | §1.6, §1.4, Principle 0 | (b) **delta 0**, and the zero is defended positively; (a) +19 field-only / +32 both, **and both wordings false** | if (b) lands at both doors, `heroes measure` reads exactly **3514** | approve when (b) is scoped to the **class**, and the `cstr` hole is opened as its own defect |
| **llm-ergonomist** (spec only) | **approve** the refusal, with a named flip | §1.3 locality | — | shown the exit-2 text, **≥5/10 blame the tool**; shown a diagnostic, ≤1/10 | flips to object if the check fires on `const char *` as `cstr` — **measured: it does not** |
| **historian** | **approve (b)**, corrected to an explicit cast | CLAUDE.md §12, and it argues §12 does **not** point at (a) | documentary; searched six ways | `--sanitize` **cannot** falsify this — UBSan has no check for const | three flips named; the load-bearing one (`-Wcast-qual` in `FLAGS`) **checked and absent** |

## The finding that outranks the sitting

The spec-warden asked what `-> cstr` accepts, and the answer is **any pointer at
all**. Reproduced by the coordinator against a four-byte blob:

```
declared length: 4
what to_str gives: ABCDSECRET
its len: 10
```

**Six bytes past the object, exit 0, no diagnostic, no abort.** Cause, one line:
`emit/assert_spelling.rs` sent both `Ty::Ptr` and `Ty::Cstr` to `HERO_RET_PTR`,
which is `__builtin_classify_type(c) == 5` — *is it a pointer* — blind to the
pointee. §4.19's whole promise is that clang checks every result type against the
header; for this row it was not being kept.

**`--sanitize` reported nothing**, because the read stays inside the allocation
containing the array. Every instrument this project owns said the program was
fine — panel 022's shape, a green harness defending a wrong program.

And the ffi-pragmatist measured the other half of the trap: with `-> ptr` refused
at exit 2, `-> cstr` was the **only compilable spelling**, and on
`select x'41004243'` it reported **1 byte of 4** — silently truncating at the
embedded NUL. So the author's only route gave a wrong answer.

## Why (b) adds no hazard — three independent proofs, none of them an argument

- **The third door is already open.** An `@out: ptr` over `const struct ops **`
  builds at **exit 0** on the unpatched compiler and **SIGBUSes** on write
  (compiler-engineer). And `examples/sqlite/main.hero:37` — **shipped** — already
  passes `void **` where the header says `const char **` (ffi-pragmatist).
- **`dlopen`/`dlsym`/`memset`**, with the word `const` in no binding, is
  `heroes check` exit 0 and SIGBUS at 138. The hazard needs no const to reach.
- **The standard says so.** UB attaches to the **object**, not the pointer's
  qualifier — C11 6.7.3p6, Rust's *immutable bytes* rule, Zig's `@constCast`,
  converged independently. The C library itself does this: `strchr` returns a
  non-const pointer into a const-qualified argument, and SEI CERT files that as a
  **recommendation** at P4/L3, not a rule.
- The historian searched six ways for **one** documented case of a binding
  generator's const erasure causing harm — CVE, bug report, post-mortem — across
  cgo (where the word `const` does not appear in the documentation at all),
  LuaJIT, ctypes, Nim and pre-9.6 Haskell. **It found none.**

## What (a) would have cost, and why it is vetoed

25 bindings, including rung 3's own result reads, for **zero** measured
robustness. `partial` does not rescue a struct whose members are all const —
`record sqlite3_file partial` is `empty_record`, unbindable at any price. And the
refusal's own diagnostic would name a repair a return type cannot perform.

## Resolution — provisional, author ratification pending

**R1. (b) lands, at both doors** — an explicit `(void *)` where a `ptr` is
materialised from C: a group-record `ptr` field read (`emit/access.rs`) and an
`extern`'s `ptr` result (`emit/ops.rs`). C11 6.3.2.3p7 makes it a conversion
rather than 6.5.16.1's constraint violation, which is the whole repair.

**R2. `cstr` gets its own result assertion**, `HERO_RET_CSTR`, over the six
character-pointer spellings — six because C's `char` is a distinct type from both
signed spellings and real headers return all three. `const void *` is refused, at
exit 1, on the author's line. **This is a §1.12 memory-safety repair and it
outranks R1** in importance if they are ever separated.

**R3. Panel 058's guard is untouched and this was verified, not assumed.**
`-Werror=incompatible-pointer-types-discards-qualifiers` was adopted for the
**write** direction — a C function writing through `.cstr()`'s copy-on-write
buffer. That job still fires as `ffi_writable_parameter` at exit 1 under R1,
measured by two seats and the coordinator. The read was collateral, and the
asymmetry is the tell: the designed firing has a named diagnostic, the accidental
one had *"internal error"*.

**R4. No `Ty` variant, no keyword, no spec type name for `const`** — the
compiler-engineer's veto condition, adopted. §4.19:2098 already files `const`
under Part 7 item 10, and CLAUDE.md §13 closes Part 7 until the fixpoint. **Spec
cost: zero**, and the warden defends the zero positively rather than merely
reporting it.

**R5. The record the compiler-engineer required**, since the compiler has no
warning channel: `fixedbugs/ffi-const-pointer` (both doors, exit 0) and
`fixedbugs/ffi-cstr-is-not-any-pointer` (the over-read, exit 1), driven by one
test, because they are one balance — widening the read without narrowing `cstr`
leaves the over-read, and narrowing without widening leaves rung 3 unbindable.

## Predictions to score

| Judge | Prediction | Checkable at |
|---|---|---|
| ffi-pragmatist | corpus 97/97 and `cargo test` 573/573 under (b), with a blob round-trip at exit 0 | **scored NOW, held** — 574 with the new case |
| compiler-engineer | ≤25 added lines confined to `emit/`; `Ty` keeps one opaque-pointer variant | **scored NOW: 108 insertions across 4 `emit/` files** — over its 25, because R2 was not in its scope; the `Ty` half **holds** |
| spec-warden | `heroes measure` reads exactly 3514 in the landing commit | **scored NOW, held** |
| llm-ergonomist | ≥5/10 blame the tool for the exit-2 text; ≤1/10 for a diagnostic | metric 2 (0 tasks — an observation, paying nothing) |
| historian | `--sanitize` cannot falsify this; `heroes mutate` finds no mutant writing through a const-derived `ptr` | M-program-corpus |

## What is filed rather than fixed

The misattributed `ffi_parameter_type` (a const-returning binding makes the
compiler blame **two correct declarations** elsewhere in the group); `@`
out-parameters passing `void **` where the header says `sqlite3 **`, warning only,
**in a shipped example**; and a C struct member literally named `function` being
unbindable. All three measured by the ffi-pragmatist, none of them this sitting's
question.
