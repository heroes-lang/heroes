# Panel 164 — spec-warden report (verbatim)

All measurements are in, on the real instrument, with the tree left frozen (`git status --porcelain` shows only the untracked `docs/panel/164-briefs/`).

**No budget veto.** Every draft I priced sits between 8034 and 8103 against a ceiling of **10240** (design.md §1.6, grepped this session at `docs/design/design.md:255` — it says 10240, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`). The worst-case route leaves 2137 free. The budget is not what should stop this sitting. Something else should.

## The counts are REAL, not a lower bound

`./heroes measure spec/heroes-spec.md --refresh`, `claude-opus-5`, 2026-09-18, in a seeded copy at `scratchpad/panel164-spec/copy`. Baseline **8040**, digest `15919ac16c8ddbb8` — which matches the ledger's panel 163 row, so the document has not moved since that close. Re-measured after every edit and restored: final baseline check reads 8040 / `15919ac16c8ddbb8` again.

| draft | real | Δ | digest |
|---|---|---|---|
| baseline | 8040 | — | `15919ac1…` |
| **R1** decay, stated truthfully | 8097 | **+57** | `320d2c25…` |
| **R2a** `f.cstr()` bare, appended | 8058 | +18 | `2907251c…` |
| **R2b** `f.cstr()` + termination, appended | 8077 | +37 | `0f052d82…` |
| **R2m** `f.cstr()` + termination, **merged** | 8070 | **+30** | `3dc4ae1c…` |
| **R3** `f.ptr()` + refusal clause, appended | 8103 | +63 | `0dfe8231…` |
| **R3m** `f.ptr()` bare, merged | 8065 | +25 | `752949e3…` |
| **R3m+** `f.ptr()` + refusal clause, merged | 8087 | +47 | `3404457d…` |
| **R3n** `f.ptr()` for-a-length, merged, **with the removal** | 8067 | **+27** | `17f36947…` |
| **R4** refuse, stated | 8072 | +32 | `0b1efef9…` |
| **R4p** pointer to the working route | 8063 | +23 | `4d68df0e…` |
| **R4s** pointer, tightest true wording | 8058 | **+18** | `075f4824…` |
| **R4s + removal** | 8052 (derived: measured 8057 for R4p+removal; R4s+removal not measured alone) | — | — |
| **the removal alone** (§ 3's two address sentences merged) | **8034** | **−6** | `43867523…` |
| R4 refuse, silent | 8040 | 0 | — |

**The termination clause costs +19 real, isolated** (R2b − R2a). That answers brief question 2 with a number. Merging beats appending again: R3 appended is +63, merged +25 — a 38-token gap, panel 122's rule measured a second time.

## Why the sitting should not land routes 1–3: the wall has a door

CLAUDE.md § RUN IT, and panel 163's own rule one day old. I composed the operations the document already carries and **ran** them with the copy's seed-built compiler. Files in `scratchpad/panel164-spec/probe/`:

| shape | today, run |
|---|---|
| `s.name` → `cstr` param (p1) | refused, `error[type_mismatch]` |
| `s.name` → `ptr` param (p2) | refused, `error[type_mismatch]` |
| `s.name.cstr()` (p3) | refused, `error[bad_operand]` |
| **`s.name.validated_bytes().must().cstr()` → `cstr` param (p5, p6)** | **compiles, builds, RUNS: prints `2` for a terminated field and `16` for a field with no zero in it** |
| that same `cstr` → a `ptr` param (p7) | refused, `expected ptr, found cstr` |
| a non-UTF-8 field through it (p8) | `err not_text` at run time |
| the record by value to a `static inline` shim (p4, p9) | runs: `2`, and `600` over a binary field |

`p6` is the one that decides the sitting. A 16-byte field with **no terminator**, handed to `strlen` through the existing composition, returns **16**. Not 17, not the next field. The overread routes 1 and 2 were convened to legislate against **is already closed**, because `validated_bytes` stops at the field's end and `str.cstr()` supplies the zero. So the shared brief's quoted premise — *no Heroes-side shim can route around it* — is false for the `cstr` half, measured. Whether a route I did not compose exists for the `ptr` half is a question, not a claim: I searched by composing the documented operations only.

## `strlen(u.sysname)` — the Part 6 answer the brief demands

design.md Part 6 asks a refusal to name the program fact that would make it wrong. The fact named was `strlen` on a `struct utsname` field. **It works today**: `sysname` is NUL-terminated, `validated_bytes` reads it, `.cstr()` lends it, `strlen` returns its length and cannot walk past the field. The refusal survives its own falsifier.

## Payment (brief question 3)

§1.6 reads **no differently** for a third amendment to one paragraph in one day: the rule is per-amendment and unconditional, with no cumulative clause — grepped, `design.md:305-314`. What differs is what the first two paid with: panel 162's +46 paid with a **registered prediction** (the ledger row says so, and that the seat had wrongly declined the prediction route), and panel 163's +10 paid **nothing** because it was a correction of a silence, not an addition. **So panel 162's −6 removal was never spent, it is still in § 3 today, and it is still worth exactly −6** — measured, 8040 → 8034. It is available to whatever lands. This proposal is *not* a correction: the compiler refuses all three shapes, so the payment rule attaches in full.

---

- **`verdict`**: **veto — route 1**. **object — routes 2 and 3**. **approve — route 4**, as `R4s` (+18) paid by the § 3 removal (−6), net **+12**. Not provisional: every count is real.
- **`section`**: design.md §1.6 (payment, unconditional at every level) and §1.2 (tokens × rewrite rate); the veto stands on CLAUDE.md § Precedence rank 3 / design.md §1.12 (robustness beats token cost and ergonomics) and design.md Part 6's standard for a refusal; Principle 0 per CLAUDE.md § 2.
- **`spec_token_delta`**: measured, real, `claude-opus-5`: before **8040**; R1 **8097** (+57), R2m **8070** (+30), R3n **8067** (+27 with the removal), R4s **8058** (+18), removal alone **8034** (−6). Recommended landing: **8052**, +12 net.
- **`removal`**: § 3's *"A `ptr` is a copied ADDRESS … A `cstr` copies an address too"* merged into one sentence — **−6 real, still unspent, measured today**. No rule is lost.
- **`needed_for_self_hosting`**: **no**. The compiler binds no `char[N]` field and passes none.
- **`argument`**: The brief's load-bearing premise is false as stated. Measured on the frozen tree's seed compiler: `s.name.validated_bytes().must().cstr()` reaches a `cstr` parameter, builds and runs — `2` for a terminated field, `16` for an unterminated one, so no overread (p5, p6). A shim taking the record reads a binary field too (p9, `600`). Routes 1 and 2 therefore buy a second spelling of a working composition, which §1.6 charges double for; route 1 additionally overreads on 37 of 50 real fields and costs the most (+57). Route 3 buys the only real gap — `cstr` is refused where `ptr` is declared (p7), and a non-UTF-8 field fails `not_text` (p8) — but Principle 0 is unmet: zero corpus programs, no Part 11 measurement. (113 words)
- **`prediction`**: if route 4 lands as `R4s` plus the removal, `heroes measure spec/heroes-spec.md --refresh` reads **8052 ± 2** at the M-readable-bytes close, with a new digest and no other spec line moved; scored at that close by an instrument that exists. And the falsifiable half that matters: **a golden program passing a fixed byte field's text to a C `cstr` function will need zero new compiler lines and zero new spec forms** — `tests/golden/run/` will carry it, built with the seed compiler, before the close. If instead any of routes 1–3 lands, I predict the next §13 sitting spends **≥ 25 real tokens** disambiguating which of the two spellings a reader should use.
- **`condition`**: I approve route 3, narrowed to `f.ptr()` for a length-taking parameter at **+27 with the removal**, the moment someone measures a program that needs a **non-UTF-8** fixed field at a `const void *` parameter **in a header the author does not own** — a system header where no `static inline` can be added. p9 shows the shim covers the owned-header case today at zero spec tokens, so that count is the whole question. I withdraw the route-1 veto for nothing short of a decay rule that cannot reach `cstr`, and even then it is dominated by R3n on price.
