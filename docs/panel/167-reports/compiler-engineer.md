# Panel 167 — compiler-engineer report: defect 066, what each route costs the compiler

Every number below was produced by a command run on 2026-09-20, on
`arm64-apple-darwin25.6.0`, in a COPY of the tree at `28df0084`
(M-declared-extents step 6, routes H and C both landed), compiled from the seed
in `4.69s user`. Where a claim could not be run it says **unrun** in those
words. The commands are at the foot. The repository itself was not modified
except by this file.

## The seat's verdict

- **verdict**: `object` on routes A, C and D; **`veto` on route B** — not on
  soundness, which is not this seat's ground, but on the ceiling, and the
  breach is a number I ran rather than an argument.
- **section**: design.md §1.1 (*implementation simplicity is a constraint, not
  a goal — it sets the ceiling*) and §1.7 (*this determines the size of your
  compiler*). Part 5's seven constructs are **not** reached by any of the four.
- **implementation_cost**: see the table and the four blocks below.
- **needed_for_self_hosting**: **no**. `grep -rn '\.ptr()' --include='*.hero'
  selfhost` returns 30 hits and I opened every one: all 30 are inside
  diagnostic text (`selfhost/lend_errors.hero`), module comments
  (`selfhost/parse/members.hero:136`) or test fixtures
  (`selfhost/emit/ffi_lend.hero:132`). **The compiler never lends a field.**
  33 lend sites live under `tests/golden/`, zero in `examples/`.
- **argument** (≤120 words): no route here adds a core construct — the lend is
  a built-in call, and `counted_by` is already *"a declaration-site mark the
  checker reads, the lowering drops"* (`selfhost/check/lend_extent.hero:20-21`).
  The ceiling is breached somewhere else. The lend/lease family went from **689
  lines to 2255** across this one milestone while the whole of `selfhost/` grew
  **1675**: **93.5% of the compiler's growth bought one six-word FFI feature**.
  Route B pays that again and cannot be parsed at all without turning `layout`
  red in three ratcheted files. Route A, spelled as an overload of the lease
  already here, needs **no parser, no AST field, no formatter change** — I ran
  it, and the formatter round-trips it byte-identically today.
- **prediction** and **condition**: at the foot.

## The one measurement that decides my seat: three ratchets with one line each

`tests/harness/suite_layout.hero` counts non-blank lines outside `test` blocks,
not `wc -l`. Measured in that unit, and each of these **was run** by adding the
lines and watching the suite:

| file | judging unit | ceiling | headroom | how I know |
|---|---|---|---|---|
| `selfhost/parse/members.hero` | 298 | 300 (§11) | **2** | added 3 lines → `layout: 1 passed, 1 failed`, *"301 lines of code, past §11's 300"* |
| `selfhost/ast.hero` | 524 | 525 (DECIDED) | **1** | added 2 → *"526 … past the 525 it measured when this check was written — a file already over the ceiling may not grow further"* |
| `selfhost/print/fmt.hero` | 1174 | 1175 (DECIDED) | **1** | added 2 → *"1176 … may not grow further"* |
| `selfhost/check/builtins.hero` | 378 | 378 (DECIDED) | **0** | table read at `suite_layout.hero:408-427` |

Restored, and the copy reads `layout: 2 passed, 0 failed` again.

**Every declaration-site parameter mark must touch all three of the first
three.** That is not a guess: `git show --numstat 28df0084` shows route C's own
`counted_by` landing as `ast.hero +4/-1`, `parse/members.hero +39/-1`,
`print/fmt.hero +11/-1`, `print/dump.hero +11/-1`.

## The file sizes a route lands in

`wc -l` first, the layout suite's own count second, headroom to 300 third.

| file | wc | suite | free |
|---|---|---|---|
| `selfhost/check/lend_types.hero` | 138 | 127 | 173 |
| `selfhost/check/leasing.hero` | 245 | 195 | 105 |
| `selfhost/check/lend_extent.hero` | 304 | 216 | 84 |
| `selfhost/emit/field_lend.hero` | 245 | 218 | 82 |
| `selfhost/emit/body.hero` | 322 | 224 | 76 |
| `selfhost/lend_errors.hero` | 275 | 262 | 38 |
| `selfhost/emit/ops.hero` | 456 | 281 | 19 |
| `selfhost/check/lending.hero` | 357 | 285 | 15 |
| `selfhost/emit/lend_extent.hero` | 329 | 290 | 10 |
| `runtime/heroes_runtime.h` | 602 | — | — |
| `runtime/parts/str.c` | 470 | — | — |
| `runtime/parts/alloc.c` | 587 | — | — |

`selfhost/` today: **223 modules, 64351 lines**. Pascal-P4 is ~4000. The lend
and lease family alone — `check/lending`, `check/leasing`, `check/lend_extent`,
`check/lend_types`, `emit/field_lend`, `emit/lend_extent`, `emit/ffi_lend`,
`emit/ffi_extent`, `emit/bytes_text`, `lend_errors` — is **2255 lines**, over
half of Pascal-P4, for one built-in and one mark.

## Route A — the field lease (a copy the program owns)

- **verdict**: **object**, and it becomes `approve` under the condition below.
  It is the only one of the four that adds storage whose lifetime the program
  states, and on my axis it is the cheapest thing that closes anything.
- **section**: design.md §4.19's fourth case (lines 2339-2353) — which is this
  route's precedent, and also its spelling warning; §1.12 for why the hole
  matters at all.
- **core-or-sugar**: **sugar / library.** It is a built-in call over types that
  already exist. No IR instruction (`grep '"ptr"' selfhost/ir selfhost/check/lower.hero`
  is empty — the lend is an ordinary builtin `.call` the EMITTER special-cases at
  `selfhost/emit/ops.hero:93`). No descriptor-pass work. **No ownership-pass
  work**, and that falls out of design.md §4.19's own constraint: *"the release
  is written by the author and never inferred"*, so nothing is swept.
- **implementation_cost**, and the surface half is measured rather than
  estimated:
  - **Parser, AST, formatter, dumper, highlighters: ZERO**, if it is spelled as
    an overload of `.lease()`/`end_lease` rather than as `pin`/`end_pin`. I ran
    it: `p: ptr @ s.name.lease()` … `end_lease(@p)` parses today, and
    `heroes fmt` returns it **byte-identical**. The only two errors are
    `error[bad_operand]: lease takes str, found u8[8]` and
    `error[bad_operand]: end_lease takes @x where x: cstr @ s.lease(), found ptr`
    — which are exactly `selfhost/check/lend_types.hero:34-44`.
    Spelled `pin`/`end_pin` instead it costs two `inventory.hero` entries (152
    of 300 used), a moved index assertion at `inventory.hero:192-193`, and the
    § 11 `Built-ins:` sentence, which `suite_spec`'s `offered` check judges.
  - **Typing**: ~10 lines in `selfhost/check/lend_types.hero` (127 of 300) —
    a `fixed_byte_run` arm for `lease` and a `ptr` arm for `end_lease`. Both
    predicates are already in that file at lines 102-133.
  - **The cell rule**: ~60-90 lines (estimate, from the sibling's measured
    size) in `selfhost/check/leasing.hero` (195 of 300). The three clauses
    generalise unchanged, because `is_lease_cell` recognises by **origin**
    (the declaration's initialiser) and not by type. One cost that is real and
    that the `cstr` side does not pay: `cstr` is hemmed in by panel 122's R3/R4
    — no Heroes function answers it, no record holds one — while **`ptr` is a
    type everybody holds**. I ran `function give() -> ptr`: legal, only
    `unused_binding` fires. So a held `ptr` cell has no type-level second
    witness and clause 1 carries the whole rule alone.
  - **Emitter**: a new module, sibling of `selfhost/emit/field_lend.hero` (245
    wc / 218 suite), ~90-140 lines (estimate). It exists for that file's stated
    reason: the operand is a **storageless** fixed-array place, so the argument
    cannot be read from `arguments[0]`. It reuses `field_lend.lent_place`
    (`emit/field_lend.hero:71`) for `h0_s.name` and the `sizeof(place)` shape
    `emit/lend_extent.hero:150` already writes. Plus ~4 lines at
    `emit/ops.hero:93` (281 of 300, 19 free) and ~10 in `emit/body.hero`'s slot
    loop (224 of 300).
  - **Runtime**: **~30 lines of C and 2 header lines** — the smallest part.
    `runtime/parts/alloc.c:349-358` already has `hero_alloc_held` /
    `hero_release_held` over the `hero_live_held` counter, so the *"a lease
    nobody ends … aborts when `main` returns, saying how many"* the spec already
    promises is **free**.
  - **Spec**: measured on the binding instrument, not the vendored one. Baseline
    re-measured today `8154`; the merged draft reads **8197**, so **+43 real**,
    under `DELTA_GATE`'s 50.

**Where do the copied bytes live — the thing the brief says nobody priced.**
In the held block, and **not** in a `[u8]`. Three reasons, each run or cited:

1. `hero_str_held(HeroStr s)` cannot serve. Its source is a `HeroStr`; a
   field's source is an array lvalue of C's own width with no NUL promise.
   A second entry point is needed: `const void *hero_bytes_held(const void
   *src, int64_t n)`, which is `runtime/parts/str.c:419-428` minus the NUL and
   plus a length.
2. `hero_held_release` cannot serve either, and this is **run**:
   `hero_held_release(&p)` for a `void *p` is
   `error: incompatible pointer types passing 'void **' to parameter of type
   'const char **' [-Werror,-Wincompatible-pointer-types]` under the compiler's
   own sixteen flags (`selfhost/cli/flags.hero:93-108`). So a second release,
   `void hero_bytes_release(const void **slot)` — `str.c:451-469` verbatim.
3. **Not a `[u8]`.** design.md §4.19 line 2348 binds the spelling: the lease
   *"is a copy and never a pin, since a pin would alias `.cstr()` and a
   copy-on-write mutation would rewrite what C reads"*. A `[u8]` is refcounted
   and copy-on-write; handing C its interior is exactly that. `check/lend_types.hero:123-127`
   already refuses a dynamic `[u8]` at `.ptr()` for the same reason, deliberately.

**What the emitter would write** for `p: ptr @ s.name.lease()` / `end_lease(@p)`:

```c
    const void *h0_p;                                     /* emit/body.hero, slot loop */
    ...
    t7 = hero_bytes_held((const void *)(h0_s.name), (int64_t)sizeof(h0_s.name));
    h0_p = t7;
    k_register(h0_p, INT64_C(8));
    hero_bytes_release(&h0_p);
```

**And this is where I object.** As the brief drafts it, route A re-opens the
defect route H closed nineteen hours ago. `selfhost/emit/field_lend.hero:57-65`
names that class in its own words — *"A silently useless write is the class this
language exists to refuse"* — and panel 164 measured that **50 of 141 pointer
parameters across 16 real headers are non-`const`** (quoted from
`emit/field_lend.hero:19-21`; I did not re-measure it, so that number is
**unrun** by me). I ran the two routes side by side, in the C the emitter would
write, clean under the compiler's own flags:

```
lend  : name[0]=65 id=72      the write lands in the field
lease : name[0]=0  id=72      the write lands in the copy and is lost, exit 0
```

- **condition**: I move to `approve` if the lease cell is declared
  **`const void *`** in the prologue. That is not new machinery — it is route
  H's, already built: `field_lend.const_lends` (`emit/field_lend.hero:198-210`)
  is read by `emit/body.hero:170` for exactly this, and
  `-Werror=incompatible-pointer-types-discards-qualifiers`
  (`selfhost/cli/flags.hero:106`) then makes handing a copy to a header's
  non-`const` parameter a hard error **at the call's own line**. Cost: ~10
  lines in `emit/body.hero`'s slot loop, zero new concepts. A library that only
  READS — every registration API, which is the shape defect 066 is about — works;
  a library that writes is refused instead of being silently defeated.
  I move to `veto` if route A lands with the cell spelled `void *`.

## Route B — a mark the binding author writes (`keeps`)

- **verdict**: **veto**, on the ceiling. A veto here is a refusal plus a demand
  for a written answer, not a block.
- **section**: design.md §1.1 and §1.7. And, against the route's own premise,
  **design.md §4.19 lines 2344-2347**, which has already ruled in writing:
  *"no declaration-site mark can express retention at all: `sqlite3.h:4888` puts
  the decision in the **fifth argument** of one declaration, `curl_easy_setopt`
  in its second, and 0 of 71 `cstr` parameters in this tree are decidable from a
  header."* That sentence is why the LEASE exists. Route B proposes the mark
  that sentence says cannot express the thing.
- **core-or-sugar**: **sugar.** Erased after the checker; the IR never learns it
  (`grep counted_by selfhost/ir` is empty).
- **implementation_cost**, priced from its own sibling rather than estimated.
  Route C's `counted_by` landed as **+915 / −46 across 16 `selfhost/` files,
  runtime untouched** (`git show --numstat --format="" 28df0084 -- selfhost`).
  `keeps` is a bare word rather than a named sibling, so the closest analogue is
  `borrows`: **31 lines across 9 files** (`grep -rn '\bborrows\b' --include='*.hero'
  selfhost`). Realistic floor, given the landing precedent:
  - `selfhost/parse/members.hero` +5 code +10 doc — **and the file has 2 lines**;
  - `selfhost/ast.hero` a `Param` field and its default — **1 line**;
  - `selfhost/print/fmt.hero` a suffix function — **1 line**;
  - `selfhost/print/dump.hero` the same (93 free, fine);
  - `selfhost/check/lend_extent.hero` a `keeps_landing` beside `uncounted_landing`,
    ~25 lines — the landing machinery (`extern_callee`, `argument_position`) is
    already there at lines 118-224, which is the one genuinely cheap part;
  - `selfhost/lend_errors.hero` one diagnostic, ~25 lines (38 free);
  - `selfhost/check/marks.hero` (115 wc) — a fifth mark must be judged against
    the other four, as `marks.hero:47-71` judges them today.
  - **Emitter: zero. Runtime: zero. IR: zero.**
  - Spec: measured. Appended as its own sentence it reads **8206, +52 real,
    OVER `DELTA_GATE`'s 50**; merged into the existing sentence it reads
    **8193, +39**. So it is affordable only if it merges.
- **argument**: three things, and the first is the veto.
  1. **It cannot be landed without moving three ratchets.** Two of the three
     are DECIDED rows whose failure message is *"a file already over the ceiling
     may not grow further"*. So the true cost is a parser split or three raised
     ceilings, and neither appears in any pricing of this route. That is the
     ceiling breach, and I ran it.
  2. **Route B alone breaches §1.12's completeness clause**, and this is run:
     nothing reaches a `ptr` parameter today except `nullptr` and a lend.
     `[u8]` gives `error[type_mismatch]: expected ptr, found [u8]`,
     `str` gives the same, and a `cstr` lease gives
     `error[type_mismatch]: expected ptr, found cstr`. So `keeps` is a
     refusal with **no route out**: the author cannot copy, because no copy
     reaches `ptr`. §1.12 says *"any C library must be bindable"*.
  3. **The corpus says it buys nothing today.** `examples/` holds 5 `: ptr`
     parameters and not one takes a field lend (`SQLITE_TRANSIENT` is a
     constant, `callback`/`destructor` are function pointers, `p` is a free,
     `error` is an out-parameter). Every lend site in the tree is a golden.
- **condition**: I withdraw the veto if the sitting lands route A **first** and
  route B second, and if route B's landing commit either splits
  `parse/members.hero` along a named seam or raises the three ceilings with the
  measurement in its body. The mark is defensible as a conservative
  over-approximation — *retention is POSSIBLE here* — once there is something a
  refused lend can turn into.

## Route C — write the hole into the specification

- **verdict**: **object**, and I say plainly that **my axis is the wrong one to
  refuse it on**.
- **section**: design.md §1.12 — *"A Heroes program must not segfault and must
  not corrupt memory. That is a goal of the language, not a quality of its
  implementation."* Not §1.1: on the ceiling this route is free, and pretending
  otherwise would be inventing a rationale.
- **core-or-sugar**: neither; no compiler change at all.
- **implementation_cost**: **0 lines in `selfhost/`, 0 in `runtime/`.** Spec
  measured: merged into the existing sentence it reads **8177, +23 real**, the
  cheapest of the four.
- **argument**: it is cheap because it does nothing. I reproduced the defect on
  today's tree with every rule the language has satisfied — `counted_by n`
  declared, root a name, callee an extern, lend in argument position — and it
  printed `10` where the honest answer is `72`, at exit 0 with no diagnostic.
  Spec'ing that makes a §1.12 violation legal by declaration. Three seats vetoed
  this shape at panel 166 for the write direction; the read direction is the
  same shape.
- **condition**: I would approve if a seat produces a measured case where the
  refusal hides a defect rather than surfacing it — which is §1.12's own named
  falsifier at design.md:605-608.

## Route D — withdraw the field lend

- **verdict**: **object**, and this seat must say the uncomfortable half:
  **on my axis route D wins by a distance.**
- **section**: §1.1 and §1.7 favour it; §1.12's completeness clause refuses it,
  which is where panel 166 left it as route F.
- **core-or-sugar**: a removal.
- **implementation_cost**: it **deletes** roughly **1120 lines** — five modules
  that exist only for the field lend: `check/lend_extent.hero` 304,
  `emit/lend_extent.hero` 329, `emit/field_lend.hero` 245, `emit/ffi_lend.hero`
  169, `emit/ffi_extent.hero` 73 — plus the `.ptr()` clauses of
  `check/lending.hero:232-271` and the `field_lend_*` diagnostics of
  `lend_errors.hero`. That is ~2% of the compiler. The sum is measured by `wc`;
  the claim that a removal would take exactly those files is an inference from
  their module docs, not a change anyone has made. Panel 166's **−44 real** spec
  figure I did **not** re-measure: **unrun** by me.
- **argument**: I am the seat whose axis this route serves and I still do not
  take it, because §1.12's completeness clause outranks §1.1 for a form already
  admitted (§1.12 line 592: *"What §1.12 decides is the shape of a form already
  admitted"*). What I do take from it is the warning: the lend has now cost four
  sittings and **+1566 lines in one milestone**, and a fifth sitting on the same
  six words is the shape that ends in withdrawal anyway.
- **condition**: I move to `approve` if a sitting measures that no binding in
  the intended corpus needs a field's address at all — the `validated_bytes`
  copy-out already serves the read direction, and route H has shown the write
  direction is a `const` question. That measurement does not exist today.

## The route nobody listed, and it is two

**E — A and B are not alternatives; they are a sequence, and only the pair
closes 066.** The brief offers four routes as choices. Measured, they are not:
route B alone refuses the shape and leaves nothing in its place (nothing but
`nullptr` and a lend inhabits `ptr` — run, three times, above), and route A
alone closes nothing, because a bare `f.ptr()` at an unmarked parameter stays
legal. **A must land first and B is what turns it into a closure.** That
ordering is also the cheap one: A costs zero ratcheted files and B costs three,
so landing A first buys a milestone in which the parser split can be paid
deliberately rather than under a defect.

**F — the `const` spelling makes route A honest for free**, and nobody proposed
it because route H landed the day before the brief was written. Declaring the
lease cell `const void *` reuses `field_lend.const_lends` + `emit/body.hero:170`
+ `-Werror=incompatible-pointer-types-discards-qualifiers` and turns the
silently-lost write I measured (`name[0]=0`, exit 0) into a clang error at the
call's own line. ~10 lines, no new concept, no new diagnostic. It is the
difference between a copy that is a feature and a copy that is defect 065
wearing a new name.

One route I considered and discarded, recorded so it is not re-proposed:
**`end_lease` copies the bytes BACK into the field they came from** (the
`[In,Out]` marshalling shape). It is buildable — the emitter already computes
the place — and it opens a class nothing refuses: between the lease and the
release the program can read the field and see stale bytes. Closing that needs a
rule that the field is unreadable while leased, which is a borrow checker, which
is design.md Part 6.

## Prediction

**If route B (a `keeps` parameter mark) lands without a module split, the
`layout` suite goes red naming `selfhost/parse/members.hero`,
`selfhost/ast.hero` and `selfhost/print/fmt.hero`; and route A, spelled as an
overload of `.lease()`/`end_lease`, lands with 0 changed lines in
`selfhost/parse/`, `selfhost/ast.hero` and `selfhost/print/fmt.hero`, and under
250 net new lines in `selfhost/`.**

Instruments: `./heroes run tests/harness/main.hero -- ./heroes layout` for the
first half; `git show --numstat --format="" <landing commit> -- selfhost | grep
-v seed` for the second. Checkable at the close of the milestone that repairs
defect 066 — the next `M-` row after M-declared-extents in
`docs/ROADMAP.md` § The chain.

## Commands, so every number can be re-run

```sh
cp -r <tree> <scratch>/tree && cd <scratch>/tree && rm -rf archive build target heroes
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes          # 4.69s user

./heroes run keep.hero                        # 10 / 10, exit 0, no diagnostic
./heroes check lease_at_ptr.hero              # expected `ptr`, found `cstr`
./heroes check what_is_a_ptr.hero             # [u8] and str are both refused
./heroes check routeA_surface.hero            # only the two lend_types.hero arms
./heroes fmt routeA_surface.hero | diff - routeA_surface.hero   # byte-identical
./heroes run tests/harness/main.hero -- ./heroes layout          # 2 passed, 0 failed
./heroes measure spec/heroes-spec.md --refresh                   # 8154 baseline

git show --numstat --format="" 28df0084 -- selfhost   # +915 / -46, 16 files
git show --numstat --format="" 28df0084 -- runtime    # empty
grep -rn '\bborrows\b' --include='*.hero' selfhost | wc -l       # 31
grep -rn '\.ptr()' --include='*.hero' selfhost | wc -l           # 30, all in strings
grep -rn ': ptr' --include='*.hero' examples                     # 5
find selfhost -name '*.hero' -exec cat {} + | wc -l              # 64351
clang <the sixteen flags of cli/flags.hero> routeA_write.c && ./routeA_write
```
