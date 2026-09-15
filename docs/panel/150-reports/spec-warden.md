# Panel 150 — report of the spec-warden

## The premise of the brief that this seat falsified

**The coordinator's brief said `--refresh` would fail for want of a key. It does
not.** `.env:92` carries `ANTHROPIC_API_KEY`, `selfhost/cli/refresh.hero:81` has
curl read it from the environment, and sourced into a subshell it ran and
reproduced the pinned number exactly: **7974, claude-opus-5, digest
`7066ff27485400b0`**.

**Panel 149's *"`ANTHROPIC_API_KEY`, which is unset on this machine"* was true of
the shell and false of the repository.** Every real number below is measured, not
estimated, and no verdict here is provisional. Ceiling grepped fresh from
`docs/design/design.md:255` and `selfhost/measure/judged.hero:56`: **10240**,
`FFI_FLOOR` 60, `DELTA_GATE` 50 vendored.

*Coordinator's confirmation, 2026-09-15: verified independently. `.env` exists,
is gitignored at `.gitignore:59`, and `--refresh` succeeds. The claim written
into the ROADMAP, a commit body, panel 149's verdict and a log entry was a
negative claim nobody searched for, which CLAUDE.md § RUN IT names exactly.*

## The document misdescribes the compiler in THREE places, not one

> **The specification is false about what the compiler does, and panel 149's
> sentence is the smallest of the three falsehoods, not the only one. Landing
> `dsub` alone repairs one third of the debt and leaves the other two
> standing.**

Run by the seat, seed-built compiler, in its own copy:

| probe | § 13 says | the compiler does |
|---|---|---|
| `record Pair` with `s: Slot`, `pair_make(n: i64) -> Pair` unmarked | `acquires` goes *"after a **handle** result"*, and a handle is *"One with a `tag` and no fields"* — `Pair` has fields, so **nothing is owed** | `error[unmarked_handle_producer]`, **exit 1** |
| `slot_close` in group `a.h`, `slot_open(n: i64) -> Slot` in group `b.h` | *"where a **group** consumes a handle type"* — `b.h` consumes nothing, so **nothing is owed** | `error[unmarked_handle_producer]`, **exit 1** |
| `slot_open(n: i64) -> Slot acquires slot_notafunction` | **silent**; no sentence requires the name to resolve | `error[unread_releaser]`, **exit 1** |

Row 1 is panel 149's unpaid `+4`. **Row 2 is a second misdescription no sitting
has ruled on**: `check/acquiring.hero:10-21` says in its own words that the key is
the TYPE and not the group, and that the parser does not retain the word *group*
at all — yet the document still says *group*. **Row 3 is an entire refusal class
with no spec text**, which panel 149's own ratification admits it *understated*.

**This is a correctness debt, not a budget one, and it outranks Q1 and Q2.**
CLAUDE.md § 12's default is displaced here by the 2026-09-15 ratification, which
names the unwritten sentence as *"the one item the ratification leaves open …
owed at the next session with a key."* **This is that session.**

## Every candidate, measured on `claude-opus-5`

`W0` is a control: the paragraph rewrapped with byte-identical content, to prove
no delta is line-wrapping.

| draft | cl100k | **real** | Δ vs 7974 | Δ vs D0 |
|---|---|---|---|---|
| **BASE** | 5988 | **7974** | — | — |
| **W0** rewrap control | 5989 | **7974** | **+0** | — |
| **D0** panel 149's `dsub` | 5990 | **7978** | **+4** | 0 |
| **D1** = D0 + *group* → *any `extern`* | 5993 | **7981** | +7 | +3 |
| **D2** = D0 + releaser clause | 6009 | **8001** | +27 | +23 |
| **Q1M** ptr, MERGED | 5995 | **7985** | +11 | **+7** |
| **Q1A** ptr, APPENDED | 6005 | **7997** | +23 | **+19** |
| **Q2** whole-value sentence | 6015 | **8010** | +36 | **+32** |
| **ALL** = D0+Q1M+Q2 | 6020 | **8017** | +43 | +39 |
| **D1R** = D1 minus the balance clause | 5977 | **7956** | **−18** | — |

**Panel 149's +4 is confirmed exactly, twice.** **Wrapping is free** on the
binding instrument, so every delta is content. **Panel 122 reproduces at 2.7×**:
merged +7, appended +19, same rule.

**No budget breach and therefore no veto.** Worst case 8017 + 60 floor = 8077
against 10240, **2163 free**. *"I say this plainly so nobody mistakes headroom for
permission"* — §1.6: a spec that grows to fill the budget because it can has
failed §1.2.

## The refusal form, priced separately

**A design.md Part 6 row is free, measured rather than assumed**: `heroes measure
docs/design/design.md` prints that no ceiling judges it. **Zero spec tokens.**

**But for Q2 a static refusal is UNAVAILABLE.** `check/leasing.hero:29` and
`check/consuming.hero:22` both state the checker has no flow analysis, and
`acquiring.hero:23-27` cites them by name. *Released element by element* is a flow
question.

## Principle 0, measured

| | `selfhost/` | `examples/` | `tests/` |
|---|---|---|---|
| handle records | **0** | 5 | 16 |
| `ptr`-returning externs | **0** | **0** | 11 |
| files declaring `ptr consumes` | **0** | **0** | **1** |
| fixed array of a handle type | **0** | **0** | **1** |

The brief's own grep over `selfhost` returns 12 — **and all twelve are inside
diagnostic strings, module docs or embedded test fixtures.** **Neither Q1 nor Q2
is compiler-need.** The single `Slot[4]` in the tree is the golden this project
wrote last night to provoke its own rule.

## Verdicts

**Inherited debt — OBJECT: land it, and land more than panel 149 priced.**
Measured 7974 → **7978** (D0) or → **7981** (D1, both corrections). A removal is
available at **−25 real** (the balance clause) and the seat **recommends against
spending it**: panel 148 R4 put it there for a silent double-free-plus-leak, and
§1.2 says a construct that saves tokens and raises error probability is a net
loss. *"A model reading the prompt writes `pair_make(n: i64) -> Pair` unmarked and
is refused. That is one full correction round-trip, 500–2000 tokens under §1.2,
against 4 tokens of repair — a 125×–500× return, the best trade on this table."*
**Land D1 now; file D2 as its own priced item at +23.**

**Q1 — OBJECT: the rule is right and THE ORDER IS WRONG.** Measured:
`-> ptr acquires nosuchthing` **checks at exit 0**, counts, and aborts at runtime.
**On a `ptr`, `acquires` is a word nobody reads** — the exact shape design.md Part
6 forbids and that `unread_releaser` closed for handles yesterday. *"Making the
mark mandatory before making it read ships a compulsory word that can lie, on
every `ptr` producer at once."* And **§1.12 does not rescue it**: a leak is
neither a segfault nor corruption, so Q1 gets no rank-3 priority and must pay its
own way. **Extend `releaser_reads` to `ptr` first; demand the mark second.** The
seat approves the merged +7 the moment that probe goes to exit 1.

**Q2 — OBJECT: no spec sentence, and the repair costs zero spec tokens.** The
seat built and ran both halves: **`four_close(f: Four consumes)` with
`-> Four acquires four_close` builds and exits 0 today**, so **the expressiveness
gap is zero and the sentence buys no program.** The per-element program aborts
naming two causes, neither of which happened — a **runtime message** defect, free
against §1.6. That message is a closed list, **short for the third time in two
days**, and CL-057 and panel 087's veto of a closed abort list are the same
finding. `acquiring.hero:224` already prints *"one mark answers for the whole
value"* at the moment of error. **Paying 32 tokens to restate a note is §1.5
inverted.** The seat approves a sentence only if someone produces a program that
is correct and unwritable without it; its own probe says none exists.

## Prediction, with the command that settles it

> **Land D1 as this sitting's only spec change and `heroes measure
> spec/heroes-spec.md --refresh` reads exactly `7981` real, ±0.** Any other number
> falsifies it and would mean the landed text is not the text priced.

```sh
cd /Users/joseph/Temp/heroes-lang && set -a && . ./.env && set +a \
  && ./heroes measure spec/heroes-spec.md --refresh
```

Corollary: land nothing and it still reads `7974` — *"the number the document has
carried while being false for two days."*

## Two defects this seat files rather than leaves in prose

`acquires` on a `ptr` is never read; and § 13 says *group* where the compiler keys
on the type program-wide, refusing a two-group program the document permits.
