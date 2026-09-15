# Panel 150 — shared brief

**Two questions, one sitting**, because both are about what the ownership
machinery cannot express and each bears on the other.

- **Q1, defect 037.** A `ptr` producer leaks at exit 0. Where a group consumes a
  `ptr`, nothing demands a mark on the call that hands one back. **When must a
  `ptr` producer be marked?**
- **Q2, defect 038.** A fixed array of handles behind ONE mark, released element
  by element, aborts at 134 against a message naming two causes that do not fit.
  **What does a whole-value mark owe when the release is per element?**

## Read these three sittings before you answer. Nobody cited them last time

**This paragraph exists because of a measured failure.** At panel 149 the
completeness critic ran `grep "147"` over five reports and six briefs and found
**zero** citations, so the historian argued a route panel 147 had refused on its
own axis and ratified the same day. **The omission was in the coordinator's own
shared brief.**

- `docs/panel/147-the-obligation-is-created-by-a-call-and-not-by-a-type.md` —
  **the obligation is created by a CALL and not by a TYPE.** Route A, moving the
  releaser onto the handle type, was refused there. Any proposal that puts the
  word on the type is re-proposing it and must say so.
- `docs/panel/148-the-mark-is-written-and-never-inferred-and-it-names-what-ends-the-life.md`
  — the mark is **written and never inferred**; the inferred form was killed on
  twelve headers of nineteen. `borrows` exists because without it the rule would
  refuse a correct binding.
- `docs/panel/149-the-walk-already-existed-and-the-instrument-was-counting-calls.md`
  — a handle is what a type REACHES, on the consumer side as well as the
  producer side. **One mark is one obligation on the whole value**, which is what
  C ships. **No refusal lands** for a record reaching several handles.

## What was measured this session, before this brief was written

Compiler built from `selfhost/` at the current tip. Every number below is from a
command that was run.

**`acquires` on a `ptr` ALREADY WORKS, completely.** This is the finding that
shrinks Q1 to one question:

| program | exit |
|---|---|
| `opaque_open(n: i64) -> ptr acquires opaque_close`, released | **0** |
| the same, NOT released | **aborts**, *"1 C handle(s) never given back"* |
| the same with NO mark, not released | **check 0, run 0, empty stderr, real leak** |

So the runtime, the emitter and the surface all carry `ptr` ownership today.
**Only the static demand is missing.** Nothing has to be built; something has to
be refused.

**The blast radius of a type-keyed Q1 rule is ONE FILE, measured across the whole
tree.** Mirroring the handle rule — *where a group consumes a `ptr`, every call
handing one back says which it is* — catches a producer only where the same
group also declares `ptr consumes`:

| tree | `ptr`-returning externs | of those, in a group with `ptr consumes` |
|---|---|---|
| `selfhost/` | **0** | 0 |
| `examples/` | **0** | 0 |
| `tests/` | **11** | **1** |

The one file is `tests/golden/check/ffi-consumes-a-borrowed-handle.hero`, which
declares `malloc(size: u64) -> ptr` beside `free(p: ptr consumes)`.

**Q2's shape, re-measured at the current tip.** `record Four` holding
`a: Slot[4]`, producer marked `four_make(n: i64) -> Four acquires slot_close`,
the program releasing each of the four elements: **abort 134, `+3`**. The
runtime message since 2026-09-15 names two causes — a double release, and a
lying `borrows` — and **neither is what happened**. It was not covered by the
older three-cause wording either, whose third cause was an UNMARKED producer.

**Why no mark can carry the number.** The count is C's choice at run time and
both real headers say so in their own text: `jpeglib.h` has
`JQUANT_TBL *quant_tbl_ptrs[NUM_QUANT_TBLS];` commented *"or NULL if not
defined"*, and `<net/route.h>` has `struct sockaddr *rti_info[RTAX_MAX]` where
which slots are filled is a sibling bitmask.

**The route nobody listed, from panel 149's critic.** All three candidates that
sitting weighed act on the PRODUCER's declaration, and the two programs that
disagree differ on the CONSUMER's: releasing the COMPOSITE exits 0, releasing the
ELEMENTS aborts. Its named options were *refuse the element release*, or *let the
mark say WHOLE versus PER-ELEMENT*. **No seat was briefed on either.**

## The lesson panel 149 paid for, which binds this sitting

**Five seats differentiated by input still shared one blind spot**: every one
read the rule as a question about PRODUCERS, and a producer-only widening was
measured to stay silent on raylib's `Font`, the only real shipped-library case,
while erroring on the contrived one. **Before you answer, ask what your proposal
does on the CONSUMER's side**, and say it.

## Rules that bind you

Build in a COPY — `cp -r` the tree to your own scratch directory and
`rm -rf target build` after copying. Never `archive/bootstrap-rs/`. The seed
builds in about 3.4 s (`clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`); rebuilding from `selfhost/` is about 20 minutes and will kill you on
the watchdog. `HEROES_RUNTIME=<copy>/runtime` must be set to build a program.

A claim enters your report only after the command that settles it has been run.
Where you could not run it, write that it is unrun, in your own words. A negative
claim names what you searched for.
