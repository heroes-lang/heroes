# Panel 169 — spec-warden

Every number below was produced by a command run in this session, 2026-09-20,
Darwin arm64, in a COPY of the tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/tree`.
The main tree's `spec/` and `docs/design/` were never modified
(`git status --porcelain spec/ docs/design/` empty after the last revert).

- `verdict`: **object** — on R1 and R3. **Approve** R2M and R4. Not provisional:
  every figure is a `--refresh` on `claude-opus-5`.
- `section`: **design.md §1.2** (real cost = tokens × (1 + rewrite rate)) and
  **CLAUDE.md § 2 / Principle 0**. **Not §1.6** — see `spec_token_delta`: no
  route on the ballot comes near the ceiling, and I decline to manufacture a
  budget objection. The shared brief's point 4 is right and it binds me.

## The ceiling, grepped rather than recalled

`docs/design/design.md:255` reads **10240 tokens, measured by `claude-opus-5`
through `POST /v1/messages/count_tokens`**. Payment rule unconditional
(`design.md:312`). My brief's number and design.md's agree today.

## `spec_token_delta` — measured, all of it

Baseline, `./heroes measure spec/heroes-spec.md --refresh` in the copy, run
today: **real 8154**, `claude-opus-5`, digest `249990ca1b6b2f66`, vendored
maximum 6126, spread 126. FFI floor mortgages 60, so effective **8214** against
**10240**: **2026 free**.

Each draft below was APPLIED to `spec/heroes-spec.md` in the copy, measured with
`--refresh`, and reverted (`diff -q` clean each time).

| draft | real | Δ real | effective | headroom | vendored Δ |
|---|---|---|---|---|---|
| baseline | 8154 | — | 8214 | 2026 | — |
| **R2M** caller-side rule, MERGED into the `@`-name sentence | 8170 | **+16** | 8230 | 2010 | +13 |
| R2 same rule, APPENDED as its own sentence | 8179 | +25 | 8239 | 2001 | — |
| R0 lifetime stated for BOTH lends, merged | 8181 | +27 | 8241 | 1999 | — |
| R3 group-level `keeps` mark + production (FLOOR) | 8214 | +60 | 8274 | 1966 | +44 |
| R1 parameter `keeps` mark + production (FLOOR) | 8223 | +69 | 8283 | 1957 | +53 |
| **R4** withdraw the field lend, `counted_by` out of `CParam` | 8065 | **−89** | 8125 | 2115 | −69 |
| R1+R2M | 8239 | +85 | 8299 | 1941 | — |
| R3+R2M | 8230 | +76 | 8290 | 1950 | — |
| **R4+R1** | 8134 | **−20** | 8194 | 2046 | — |
| **R4+R3** | 8125 | **−29** | 8185 | 2055 | — |

Three things this table settles.

1. **No route breaches the ceiling and none comes within 1900 of it.** There is
   no veto available here and I will not invent one.
2. **Merging beats appending, again**: the identical rule costs **+16** merged
   into the sentence that already carries its other half, **+25** appended.
   Panel 122 measured 36%; this is 36%.
3. **Panel 167 recorded R4 at −80. On the binding instrument it is −89.** The
   vendored table says −69, so the vendored delta understates the real by 29%
   here — `.claude/rules/spec-shape.md`'s rule earning its keep for the fourth
   time.

## Q3, which my brief flagged as possibly decisive: the route nobody listed

**Yes, there is one, and it is a compiler defect rather than a language change.**

`spec § 3:64` reads: *"No aliasing exists among the values this language owns."*
Its own exemption is the next clause, and the exemption is about FOREIGN storage:
*"two copies reach one foreign thing."* I reproduced defect 068 myself
(`/tmp/w169/r68.hero`, `/tmp/w169/r68.h`): `check` **0**, `run` **0**, prints
**8** then **72**, `--sanitize` exit **0** with **zero** sanitizer lines. In that
program C holds an alias into `s.nsap` — a value the language owns, not a foreign
thing — across a Heroes write to `s`. **§ 3:64 is false as measured, and read
strictly it already forbids the program.**

`spec § 13:367` says `f.ptr()` *"lends"* a field *"to a `ptr` **parameter**"*. A
parameter's life is the call. The only reading under which § 3:64 and § 13:367
are both true is that the lend's alias ends with the call — which is exactly the
rule nobody enforces.

**And the brief's premise about the lease sentence is half right, which changes
the price.** I ran the shipped `examples/gallery/13-lease.hero` unmodified: `run`
0, **"C still reads 13 bytes" three times, correct**. I ran the reordered one:
`run` 0, **"C still reads 0 bytes"**, where 13 is honest. So *"a COPY of the bytes
that C may read for as long as the program says"* is a **permission with an end
date**, TRUE of the order the document's own example models and merely
unenforced. It is false only if read as a guarantee. **Therefore no route needs
to spend a single token saying when the bytes die: § 13 already says it.** Any
draft that adds a sentence for that is paying for text the document has.

What IS missing from the prompt is in the gallery instead of the spec:
`examples/gallery/13-lease.hero:3` tells the reader *"`s.cstr()` lends a string to
C **for one call**"*. That rule is in the teaching material and absent from the
document that IS the prompt (§1.6). R0 prices moving it in at **+27** for both
lends.

## Q1, cheapest per defect closed — a different ranking from cheapest

I reproduced both defects and ran the shape that scores R4.

- **068** reproduces through `f.ptr()` (above). I then ran the same shape through
  `s.cstr()` twice — a literal (`/tmp/w169/r68b.hero`, prints **13**, correct) and
  a heap `f"…"` string (`/tmp/w169/r68c.hero`, prints **34**, correct, three runs).
  **068's shape does not reproduce through the `str` lend.** So 068 is a
  `f.ptr()` defect, measured, and R4 closes its only live reproducer.
- **066** reproduces through the lease (gallery reordering, run above) and is
  untouched by both R2M and R4.

| route | defects closed, measured | Δ real | per defect |
|---|---|---|---|
| R4 | 068 | −89 | **−89, paid not spent** |
| R2M | 068 | +16 | +16 |
| R1 | **zero** (see prediction) | +69 floor | undefined |
| R3 | **zero** (see prediction) | +60 floor | undefined |

## Q2, what a removal is worth against an addition that closes the same class

**Strictly more, and design.md says why.** §1.2 prices error PROBABILITY, not
text. A withdrawn form has error probability zero and requires no author to have
read a rule. A `keeps` mark has error probability equal to the rate at which
binding authors omit it — and panel 167 measured that the header **cannot supply
it**, since a copying and a keeping parameter are both `const char *`. So R1 and
R3 spend 69 and 60 real tokens to convert *a silent wrong answer* into *a silent
wrong answer unless somebody already knew*. That is the §1.2 net loss in its
purest form, arriving from the expensive side.

**And Principle 0 decides it.** Run today: `grep -rn '\.ptr()' selfhost/`
excluding strings and comments returns **zero call sites**; `lease()` in
`selfhost/` matches only `hero_dir_release()`. **The compiler uses neither
`f.ptr()` nor `s.lease()`.** It uses `.cstr()` (ten sites in
`selfhost/cli/process.hero` and `selfhost/emit/literal.hero`). So the two forms
carrying both defects are on the *provably serves the thesis* branch, and the
thesis they serve is *every plausible LLM mistake is a compile error* — which
they break. A `keeps` mark the compiler does not need, that closes zero measured
reproducers, **waits** (CLAUDE.md § 2).

## `removal`

**R4 is the removal, and it pays for everything on the ballot.** R4+R3 lands at
**−29 net** and R4+R1 at **−20 net**: both are net REDUCTIONS of the spec that
address both defects' forms and both discharge §1.6's payment rule outright
rather than owing a prediction. Nothing else needs to come out.

If R2M lands alone (+16), the payment is the prediction below, not a removal:
I searched § 13 for a cut and the only candidate that is not load-bearing is the
`package` paragraph (§ 13:386-389) — implemented, golden-tested, used by no
example and needed by no compiler. Removing it to fund 16 tokens is
disproportionate and is its own panel question. I name it as available and do not
recommend it.

## `needed_for_self_hosting`

**no** — measured this session, and it is the load-bearing fact: zero `.ptr()`
call sites and zero `.lease()` call sites in `selfhost/`.

## `argument` (120 words)

The ceiling does not decide this sitting: the widest route leaves 1941 tokens
free. §1.2 and Principle 0 decide it. R1 and R3 buy a declaration-site mark the
header cannot supply, at +69 and +60 real; in all three of 066's reproducers the
mark is absent, so they close the case where the author already knew, which is
not the defect. R4 removes a form the compiler never calls, closes 068's only
measured reproducer, and PAYS 89 real tokens. § 3:64 already says no aliasing
exists among values this language owns, and 068 makes one — so 068 is a
compiler defect against text already written, closable at +16 merged or at −89
by removal. A removal beats an addition because its error probability is zero.

## `prediction` — falsifiable, instrument exists today, scored at M-declared-extents close

Instruments: `./heroes check`, `./heroes run`, `./heroes run --sanitize`,
`./heroes measure spec/heroes-spec.md --refresh`. All four ran in this session.

1. **R1 or R3, landed as specified, leaves all three of 066's reproducers at
   `check` exit 0 and a wrong answer at exit 0**, because in each the retaining
   parameter carries no mark. Concretely:
   `docs/panel/168-briefs/gallery-example-reordered.hero` against an UNMARKED
   `extern "13-lease.h"` stays `check` **0**, `run` **0**, prints **0**.
   *Falsified if it prints 13 or exits non-zero.*
2. **R2M and R4 each close 068 and each close zero of 066.** After either lands,
   `/tmp/w169/r68.hero` moves `check` 0 → **exit 1**, and the gallery reordering
   stays `check` **0** / `run` **0** / prints **0**.
   *Falsified if the reordering starts failing, or if r68 still checks clean.*
3. **Token prediction.** The adopted § 13 text will land within **±12 real
   tokens** of this table's figure for its route. If it exceeds that figure by
   more than **40**, it is a different rule from the one priced and owes a
   re-price before the commit. R4 will land at **8065 ± 12** real.
4. **068 will not be reproducible through `s.cstr()`** at the close: re-running
   `/tmp/w169/r68b.hero` and `/tmp/w169/r68c.hero` prints 13 and 34, correct.
   *Falsified by either printing a wrong length.*

Panel 167's spec-warden prediction was correct and invisible for two sittings.
These four are written as commands with expected exit codes so a coordinator can
score them without reconstructing an argument.

## `condition` — what changes my verdict

- **On R1/R3 I withdraw the objection** if a seat shows a MEASURED route by which
  the mark is supplied by something other than the binding author's knowledge —
  a header attribute, a package manifest, a `heroes` subcommand that reads the
  library's own documentation — or shows one of 066's three reproducers refused
  at `check` with the mark ABSENT.
- **On R4 I withdraw the approval** if the ffi seat re-measures the 28
  `const void *` declarations on today's tree and shows a binding that no handle
  and no `validated_bytes` route reaches, i.e. that withdrawing the field lend
  removes a capability rather than a hazard. Panel 167's veto was measured before
  panel 168 changed the ground; it has not been re-run.
- **I veto** any draft whose `--refresh` reading exceeds **10180** real
  (10240 less the 60 FFI floor), and any draft priced to this sitting on the
  vendored table alone: the vendored delta understated the real by 29% on R4 in
  this very session.
