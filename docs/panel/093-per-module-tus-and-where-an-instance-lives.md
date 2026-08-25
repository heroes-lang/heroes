# Panel 093 — per-module TUs, and where a monomorphised instance lives

**SOUNDNESS lane** (compiler-engineer + ffi-pragmatist): backend architecture,
no surface, no diagnostic, no spec token. Convened at M-separate-compilation
step 6 on the three questions the record left open for exactly this step —
panel 029 R5b's instance placement, deferred by panel 030 R1 because
whole-program monomorphisation could still see every instance; the descriptor
model under separate TUs; and the shape of the per-module cache key (panel 030
R2 rows 2–3). Row 1 landed as step 5 (`error[extern_across_modules]`).

**Status: provisional — author ratification pending.**

## The proposal, verbatim

1. **Two paths, one emitter.** `heroes build X.hero` emits one `.c` per module
   plus cross-TU prototypes, compiles each to its own cached object, links.
   `--emit-c`, the seed, the 146 blessed emissions and the fixpoint ritual keep
   the fused single unit (`seed/README.md`: the seed stays one clang line).
2. **Instance placement (a).** A monomorphised generic instance is emitted
   `static` in every TU that uses it — the C++ inline model. Generated type
   descriptors and per-type functions likewise: `static`, duplicated per TU.
3. **The per-module cache key** (rows 2–3): a module TU's object is keyed on
   its own source text + the flags + `runtime_text()` (kept) + its extern
   headers' contents + link flags + every dependency's emitted interface.

## What the seats measured

Both seats hand-built the architecture before judging it. Nothing below is an
estimate.

**The ffi-pragmatist** (a real SQLite binding split over two TUs, compiled
under all thirteen flags, linked, run):

- The two-TU binding **works**: db's TU carries `<sqlite3.h>`, asserts, probes
  and wrappers; main's TU is sqlite-free, prototypes only. `version: 3.51.0`,
  cross-TU `str` equality true, ASan/UBSan clean. A wrong declaration fails in
  db's TU at exit 1; main's TU **cannot** see the mistake by construction.
- **Acceptance row 4 is unsatisfiable as written** — "exit 1 in both TUs"
  presumes the calling TU can see the header it deliberately does not have.
  The real rule: the driver compiles **every dirty TU before rendering any
  verdict**, maps the declaring TU's clang failure to exit 1 on the `.hero`
  line, and a cache hit on any other TU never converts the build to success.
- **The prototype-verification gap, made to bleed**: a cross-TU prototype
  disagreeing with its definition (`int64_t` vs `HeroStr`) compiled clean,
  linked clean, and printed `4317207840` at exit 0 — the first compiler-emitted
  C that clang never verifies. The fix, verified: **the declaring TU includes
  the same interface text it exports**, turning any divergence into
  `error: conflicting types` inside the declaring TU.
- **The emitted C is link-blind, measured**: `link "sqlite3"` → `link
  "NOTALIB"` left `--emit-c` output byte-identical. A cache keyed on emitted C
  cannot see a link edit; the key must take the **source text**, and link
  names live in the driver's module graph, recomputed from fresh ASTs each
  build. A `package`'s **resolved** flags — not its name — enter the key,
  because pkg-config's answer is machine state that changes with no source
  edit.
- **Descriptor duplication is behaviourally invisible, measured**: two static
  `HeroDesc` copies at different addresses, a value built in one TU and
  pushed, compared, hashed and dropped in the other — ASan+UBSan silent,
  `hero_runtime_check_leaks()` clean. The checker refuses a generic function
  as a function value (measured), so `static` instances cannot leak two
  addresses into function equality.

**The compiler-engineer** (the fused driver mapped line by line; a two-module
program hand-split into two TUs; both alternatives built):

- **The split cuts only in the emitter and the driver** — the frontend stays
  whole-program (one `Source`, one check/lower/mono pass). Cost, module by
  module: **~480–720 non-test lines, ~700–1000 with tests** — design.md:2788's
  own pricing holds. Panel 030 prediction 8 ("instance placement alone exceeds
  the whole use+resolver cost") will score **false**: placement measured
  near-zero because the whole-program frontend survived; the cost migrated
  into the key/interface machinery.
- **The two-TU prototype is byte-identical to the fused binary** through
  cross-TU equality, map hashing computed by two different static `hash`
  copies, drops across TUs, ASan and the leak gate. Generated descriptors are
  **already `static` in the fused unit** (`emit_descriptors.hero:138`), so
  that piece is nearly free. `nm` shows genuine duplication (local symbols),
  binary bloat 110456 vs 110544 bytes — negligible.
- **Full duplication produced 11+13 `-Wunused-function` warnings**: the
  per-TU `reached`/`constructed` trim is **mandatory** for §7's zero-warning
  bar, not an optimisation.
- **The brief's reason against define-site placement was wrong, and the seat
  built the refutation**: alternative (b) — instance external in the defining
  TU — links and runs, because the whole-program frontend can spell every
  typedef in every TU. The real killer is the **cache arrow**: under (b) the
  defining TU's text depends on its *dependents* (which instantiations exist),
  and a key of own-source + dependencies' interfaces cannot see a dependent's
  change — a new instantiation in a caller leaves a stale defining-TU object:
  undefined symbol at link, or silent dead code. (c), first-user placement,
  adds file-order fragility. **(a) is the only placement where a TU's text is
  a function of its own module and its dependencies.**
- **The interface text must be rendered by the emitter's own walks, never
  curated — falsifier built**: reordering two *empty* variant cases in a
  dependency leaves the struct typedef byte-identical and changes only the
  tag enum (`= 0` → `= 1`) and the consumer's `case` labels. A curated
  "typedefs" list misses it, and the warm-cache consumer **prints the wrong
  case at exit 0**.
- **Name-stability defects found in advance**: option/function typedef names
  are program-wide-numbered today, so an unrelated root edit renames a
  dependency's signature text (`h_toy_0opt1` → `h_toy_0opt2` on `h_geom_half`)
  — benign for linkage, fatal for clean-vs-incremental byte-identity. Fix:
  content-named typedefs (the typehash machinery exists) or per-TU numbering;
  `hero_str_N` likewise per-TU.
- The runtime invariant grep-verified: the only descriptor-address
  comparisons anywhere are `sort.c:121-132` and `text.c:103`, both against
  runtime-owned `extern const` primitives that TUs reference and never copy.

## Verdict table

| seat | verdict | ground | cost | condition |
|---|---|---|---|---|
| compiler-engineer | **object** on the cache-key piece as briefed; accept pieces 1–2 | Part 10 step 18 (design.md:787, :2788, :2806) | ~480–720 non-test / ~700–1000 with tests | interface text = the emitter-walk-rendered byte contribution (tag enums included); "descriptor externs" replaced by static duplication; two instruments: per-TU double-emit determinism, warm-cache staleness golden |
| ffi-pragmatist | **approve**; accept-with-conditions on prototypes and the key | §1.11, §4.19, §3.1's cache sentence | — | the declaring TU includes the interface text it exports (or provably one printer for both); keys = source text + `runtime_text` + search + resolved package flags; a test pins that runtime descriptors are referenced, never copied |

## Where the seats agreed without coordination

Both found, independently, that **the cache key must not be derived from the
emitted C** and both named the same repair direction (source text in, plus
everything that reaches the object by another road). Both verified the
descriptor invariant against the same two runtime files. Neither found a
memory-safety hole in duplication — one via ASan on a hand-built crossing,
one via the leak gate and `nm`.

## Corrections to the convener's brief (recorded first, panel 089's rule)

1. *"Define-site placement would need caller types it cannot name"* — *
  **false**. It links and runs; the cache arrow is the real reason (b) dies.
2. *"Descriptor externs it imports"* in the key sketch — names an import the
  architecture abolishes; generated descriptors were already `static`.

## Resolution — provisional, author ratification pending

Adopted, most conservative composition of both seats:

- **R1**: two paths, one emitter. The fused unit remains the artifact path
  (seed, blessing, determinism, fixpoint); per-module TUs are the build path,
  and the build path owes its **own** double-emit determinism check per TU.
- **R2**: instance placement (a) — `static` per using TU, descriptors and
  per-type functions included, with the per-TU `reached`/`constructed` trim
  from day one (§7's zero-warning bar). Alternatives (b)/(c) are refused on
  the cache arrow, with the corrected reason on the record.
- **R3**: a module's **interface text** is the emitter-walk-rendered byte
  contribution a user TU receives — prototypes, typedefs INCLUDING tag
  enums, nothing curated by hand — and **the declaring TU includes the same
  text it exports**, so a prototype/definition divergence is a clang error in
  the declaring TU rather than a silent miscompile (the pragmatist's measured
  exit-0 garbage).
- **R4**: the per-module key = own source text + flags + `runtime_text()` +
  extern headers' contents + search paths + **resolved** package flags +
  every dependency's interface text. Link names never enter the emitted C and
  never the key alone: the driver's module graph, recomputed from fresh ASTs,
  carries them. The `HERO_RUNTIME_ABI` stamp is emitted in **every** TU.
- **R5**: acceptance row 4 is re-read as the seats measured it: every dirty
  TU compiles before any verdict; the declaring TU's clang failure maps to
  exit 1 on the `.hero` line; a cache hit never converts a failure to
  success.
- **R6**: name stability owed before the cache can be honest: option and
  function-type typedef names become content-derived (typehash) or per-TU,
  and string literals number per-TU.
- **Instruments owed with the landing**: per-TU double-emit determinism; a
  warm-cache staleness golden (the tag-reorder witness); the
  prototype-agreement mechanism; a pin that runtime descriptors are
  referenced, never copied.

**What a veto would compel**: none was cast. The engineer's objection is
absorbed by R3/R4 as conditions; if the author overturns R2 toward (b), the
key must grow a reverse edge (dependents' instantiation sets), which both
seats' evidence prices as the more fragile architecture.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | the milestone diff lands ≤1000 lines; the two-module harness case compiles per-TU with zero clang warnings; the tag-reorder golden prints the correct case on a warm cache | M-separate-compilation close |
| compiler-engineer (in advance) | panel 030 prediction 8 scores **false** — placement near-zero, cost in key/interface machinery | M-separate-compilation close |
| ffi-pragmatist | `examples/sqlite` split as db-module + main needs no shim; a wrong extern is exit 1 on the db line regardless of TU order | step 6 acceptance |
| ffi-pragmatist (falsifier) | if a module's object is keyed on emitted C instead of source text, editing only the `link` name hits the cache and links the old library at exit 0 or fires `ffi_missing_library` against the wrong name | step 6 acceptance |
