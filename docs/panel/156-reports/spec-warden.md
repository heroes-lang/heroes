# Panel 156 — spec-warden report

- `verdict`: **approve** the zero-token direction (R1-R5's repair); **veto**, narrowly,
  any amendment to `spec § 6` or `§ 11` at this sitting. Not provisional: the binding
  count was read from the instrument in this session, exit 0.
- `section`: design.md **§1.6** (budget and the unconditional payment rule), **§1.12**
  (a Heroes program must not segfault; *a guarantee that ends quietly is not one*),
  **§1.2** (tokens × (1 + rewrite rate)), **§1.0**/Principle 0; CLAUDE.md **§12**
  (spec beats compiler) and **§ Precedence** rank 3.
- `spec_token_delta`:
  - **Direction A (recommended): 0 / 0.** MEASURED this session,
    `./heroes measure spec/heroes-spec.md`, exit 0:
    `claude-legacy 5863 · cl100k_base 5989 · maximum 5989 · real 7974` (claude-opus-5,
    taken 2026-09-15, digest-fresh — `measure` exits 1 on a stale digest and it did
    not, so 7974 is a count of the bytes on disk today). Against the **10240** ceiling
    grepped at `docs/design/design.md:255`: 2266 headroom, 2206 net of §4.19's FFI floor.
  - **Direction B (buy a sentence): +7 to +14 vendored, ~+9 to +19 real (ESTIMATED).**
    Measured whole-document, spec copies in scratchpad, run from the repository root:
    - B1 `…saying why — a fault inside a C library too; …` → 5871 / **5997** (+8)
    - B2 `…saying why, keeping what the program already printed; …` → 5870 / **5996** (+7)
    - B3 both → 5877 / **6003** (+14)
    The REAL delta is **estimated** at the measured ratio 7974/5989 = 1.3315 → +11 / +9 /
    +19. It cannot be measured: `heroes measure <draft> --refresh` **exits 2** ("refreshes
    the record for `spec/heroes-spec.md` and `CLAUDE.md`, and <path> has none", panel 123
    R5), measured, and `ANTHROPIC_API_KEY` is unset in this session. UNRUN, named below.
- `removal`: **nothing — and that is correct**, because direction A spends nothing.
  Under direction B the payment rule is unconditional at 10240 (§1.6) and no removal is
  on the ballot, which is by itself a reason to refuse B.
- `needed_for_self_hosting`: **no**. Nothing here is a form the compiler needs. All of
  it is runtime and instrument.
- `argument`: § 6's promise **covers** the fault. Three measured grounds. (1)
  `.claude/rules/spec-shape.md`: *the definition of abort is in § 6 and nowhere else*,
  and panel 087 **vetoed a closed list** because the runtime aborts in more places than
  the document names — so reading "an abort" as only the named ones revives exactly the
  list that was vetoed. (2) The runtime already raises this one as an abort:
  `runtime/parts/stack.c:461` prints `panic: …` and calls `abort()`. (3) §1.12 settles
  it without § 6: *a Heroes program must not segfault*, *a goal of the language, not a
  quality of its implementation*, and *a guarantee that ends quietly is not one*.
  Windows exit 139, both streams empty, is that sentence's own counter-example.
  So: compiler bug (CLAUDE.md §12), **zero spec tokens**, and any purchase fails
  Principle 0 — the compiler needs no such form and no Part 11 effect was measured.
- `prediction`:
  1. The whole repair moves the spec by **exactly 0**: after it lands,
     `./heroes measure spec/heroes-spec.md` still prints `real 7974`, `cl100k_base 5989`,
     exit 0, digest unchanged. Any non-zero delta falsifies me.
  2. Narrow `tests/harness/suite_surface.hero:283` to `code: -1` plus
     `.err_has("panic: a null pointer was read through")` and nothing else: **Linux goes
     green, macOS stays green, Windows stays red** at 139 with empty streams. Falsified if
     Linux stays red or Windows goes green.
  3. If B1 is bought anyway: vendored 5989 → **5997** (measured), real 7974 → **7985 ± 3**
     (estimated). Falsified by a `--refresh` outside 7982-7988.
  4. Buying B1 or B2 changes the rewrite rate by **0.00**: no model writes a different
     Heroes program because the document promises a message on a C fault. §1.2 therefore
     prices B at +9 to +19 tokens for zero saved correction round-trips — a pure loss.
- `condition`: I drop the narrow veto on either of two measurements. (a) A corpus run
  showing models write materially different programs when the sentence is present —
  a registered prediction naming a live instrument (panel 012 as amended by 046). (b) A
  **fourth** stopping behaviour that neither §1.12 nor § 6 reaches, which would mean the
  document really is silent rather than merely unenumerated. I also withdraw the
  non-provisional status if `./heroes measure spec/heroes-spec.md` ever exits 1: a
  withheld verdict has no binding number.

---

## What I grepped, and what else bears on it

The coordinator's reading of `spec § 6` is right and **incomplete**. Grepped
`abort|panic|segfault|crash` over `spec/heroes-spec.md`: nine hits, and the only
DEFINITION is line 147. The other eight are sites saying *this aborts* beside their own
operation — § 7 shift count and overflow and division by zero, § 9 recursion too deep,
§ 10 index and slice, § 11 a non-UTF-8 argument, § 13 a lease nobody ends. That is
precisely the shape `.claude/rules/spec-shape.md` records: one definition, open set of
sites, **no section lists every abort** (panel 087's veto, falsified with a running
program).

Two further things bear on it that the brief did not name:

- **`§ 11`'s `print` paragraph says nothing about buffering, flushing, or what survives.**
  Measured: `grep -n "stderr\|stdout"` over the spec → **0 hits**. The document never
  names a stream. So R3 is genuinely unwritten, and R1's blame line is too: § 6 promises
  *saying why*, never *saying where*.
- **The word `panic` appears 0 times in the specification** (measured). The runtime's
  wire vocabulary is unbound by the document, which is correct — panel 120's *the spec
  buys prose and not signatures* generalises. Anyone proposing to pin the wording is
  making a third purchase with the same unmet burden.

## R1-R5, insofar as the document bears on them

**R1 — which blame line.** The document **does not bear on it**: § 6 says *saying why*,
not *saying where*. No token is owed in either direction and none may be spent. My input
to the seats that do decide: design.md **§4.17** ("an LLM does not have the project open;
every error carries all the context needed to fix it") favours the **Heroes caller**,
because that is the line the author can edit; `node_value` is inside the header. Verdict:
**approve** naming the Heroes caller, at zero spec cost. **Object** to any spec sentence
about frames.

**R2 — the arm64 walk.** Document silent. But I found something stronger than the brief
reports, and it makes R2 a defect on every platform rather than a platform question.
`runtime/parts/stack.c:450-457`, the handler's own comment:

> *Nothing is printed when the walk finds no Heroes name, which is a fault raised
> entirely inside C.*

That is **false of the code beneath it**. `hero_stack_blame` returns `first` — the
`dladdr` symbol at the faulting PC — when the walk finds nothing, so `who != NULL` and
`hero_stack_say_heroes_name(who)` prints a name that is **not** a Heroes name. macOS's
`called from node_value` is that path. So the runtime contradicts its own contract, and
the suite row has been pinning the contradiction as the required answer since defect
045 landed. Verdict: **defect**, and it is repairable at zero spec cost either way the
panel resolves R1.

**R3 — must an aborting program keep what it printed.** The document is **silent**, and
this is the one place a sentence could honestly go. I still refuse it: Principle 0 is
unmet (the compiler needs no such form; no measured Part 11 effect was offered), and
§1.2 prices it at +9 real for zero saved round-trips. Recommendation: **make the
behaviour uniform in the runtime**, then let the instrument assert a measured uniformity
rather than a promised one. If the panel declines the flush, then row 283's
`.out_is("7\n")` must go, because an instrument may not assert what neither the document
nor the runtime guarantees.

**R4 — the missing Windows arm.** **Defect**, and §1.12 names its symptom by name. Zero
spec tokens. The repair's shape is the ffi seat's; the direction is forced by
§ Precedence rank 3, which puts robustness above token cost, ergonomics, compiler size
and speed.

**R5 — the red `main`.** The red is **correct and should stand**, with one correction
that is not a concession. Part of today's red is the instrument over-claiming:
`called from node_value` pins one platform's *fallback*, i.e. the frame walk **failing**.
Deleting a false assertion is not making CI green — Windows stays red at 139 until the
third arm lands, which is the red that carries information. Verdict: **narrow the row,
keep the red.** Going green by any other route re-hides the class that only a machine
nobody runs locally could see.

## The second question: is a platform-pinned golden a golden?

**First, a correction from the world.** There is no `.expected` file.
`find tests/golden/surface-fixtures/nullread -type f` returns exactly two:
`main.hero` and `node.h`. The expectation is a **row** in
`tests/harness/suite_surface.hero:283`, and its three assertions have three different
standings:

| assertion | what it rests on | keep? |
|---|---|---|
| `.err_has("panic: a null pointer was read through")` | § 6 *saying why* + §1.12 | **keep** — platform-independent, it is the promise |
| `.out_is("7\n")` | nothing in the document (R3) | keep **only** if the flush lands |
| `.err_has("called from node_value")` | one platform's frame-walk **failure** | **remove** — it is not a promise anywhere |

**So: no.** A golden that encodes a platform-specific answer is a recording of an
accident, and this one records a defect the runtime's own comment disowns.

**The replacement shape already exists in this tree, twice, and I invent nothing.**

1. **Assert the class, not the frame.** The very next row,
   `suite_surface.hero:284`, is the model: *"deep recursion stops with a word at -O0,
   **on every platform**"*, asserting `panic: stack exhausted` and nothing machine-specific.
   Three rows run on that pattern (`deep`, `deepthread`, `wideframe` at both levels) and
   none has ever needed a platform arm.
2. **Where an answer genuinely differs by machine, ask the machine.**
   `tests/harness/suite_corpus.hero:295-299`, on `--sanitize`: *"The port has no platform
   test, so this asks the machine the way `patience` does: a sanitised build of a trivial
   program either runs or it does not."*

**And what must NOT be used:** `may_skip()`. Its own comment
(`suite_surface.hero:105-107`) says *green where this machine lacks the C library the
case binds*, and `probe.hero:197-207` fails the suite when skips exceed half the table.
Marking a platform-varying answer `may_skip` would state a false reason and spend a
budget reserved for missing libraries. This fixture binds no library.

## What I left UNRUN

- **The real (`claude-opus-5`) token delta of B1/B2/B3.** Structurally unrunnable here:
  `./heroes measure <draft> --refresh` exits 2 by design, and no API key is set. What
  would settle it: place the draft at `spec/heroes-spec.md` **in a full copy of the tree**
  and run `./heroes measure spec/heroes-spec.md --refresh` with a key. I did not, because
  the working tree is frozen and B is a direction I am voting against. The vendored
  deltas (+8 / +7 / +14) are measured and are the movement detector §1.6 says they are.
- **The three legs of prediction 2.** Would need `./heroes run
  tests/golden/surface-fixtures/nullread/main.hero -O0` on Linux and Windows, plus the
  narrowed row. Over the watchdog and off this box.
- **Whether the narrowed row passes the `annotations` and `fixes` suites** that
  `.claude/rules/verification.md` names for `tests/golden/surface-fixtures/**`. Command:
  `./heroes run tests/harness/main.hero -- ./heroes annotations` then `… fixes`. Not run:
  the tree is frozen and the row is not edited yet.

## Files

- `/Users/joseph/Temp/heroes-lang/docs/design/design.md:253-314` (§1.6, ceiling 10240),
  `:577-600` (§1.12), `:1988` (§4.17)
- `/Users/joseph/Temp/heroes-lang/spec/heroes-spec.md:147` (the one definition of abort),
  `:300-313` (§ 11's `print`), `:328-381` (§ 13)
- `/Users/joseph/Temp/heroes-lang/.claude/rules/spec-shape.md` (the abort rule; panel 087)
- `/Users/joseph/Temp/heroes-lang/tests/harness/suite_surface.hero:283` (the row the brief
  calls `.expected`), `:284` (the shape that replaces it), `:105-107` (`may_skip`)
- `/Users/joseph/Temp/heroes-lang/tests/harness/suite_corpus.hero:295-299` (ask the machine)
- `/Users/joseph/Temp/heroes-lang/tests/harness/probe.hero:180-207` (the skip floor)
- `/Users/joseph/Temp/heroes-lang/runtime/parts/stack.c:450-469` (the comment its own code
  falsifies)
- Drafts and measurements:
  `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/bf49271c-9c48-4701-9a34-ec92d4cc09af/scratchpad/p156/`
