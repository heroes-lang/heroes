# 001 — The lineage behind §4.10, and the five alternatives it declines

**Origin.** 2026-08-10 · reasoning session · the author brought in a prose
argument that value semantics is a considered choice rather than a naive one,
with its precedent chain and a table of rejected alternatives, to be
incorporated into the project. Read: `design.md` §4.10, §1.11 (lines 386–396),
§3.2, Part 2, Part 6, Part 7 item 13, the Historical grounding appendix ·
`CLAUDE.md` §5, §13. No file of code, spec or design modified.

## The question

`design.md` §4.10 states the rule and its three gifts, and the appendix names
Cyclone, Hylo/Val and Swift. What it does not do is answer the objection an
implementer arrives with: **that value semantics reads as amateurish** to
anyone whose model of a real language comes from C++, Java or Rust — no
references, no graphs, copies everywhere. The stated reason for writing the
lineage down is that an implementer who believes the choice is a beginner's
mistake will keep trying to fix it.

## The reasoning, as brought to the session

### Why this is a serious choice and not a naive one — the lineage

**APL, and its descendants J, K, and BQN.** Sixty years of value semantics over
whole arrays, with no aliasing and no references in the surface language. K in
particular runs some of the fastest financial databases in existence — the
value model is not what makes a language slow. What these languages establish
is that *transforming whole values* is a complete programming style, not a
restricted one.

**R and MATLAB.** Copy-on-write value semantics for their core data structures,
and jointly the dominant languages of statistical computing and numerical work
for decades. Millions of programmers have never needed a mutable reference to
get real work done. They also demonstrate the failure mode honestly: both are
slow when someone writes element-at-a-time loops over shared structures, which
is exactly the pattern Heroes also expects to be bad at and does not care about.

**Erlang.** No shared mutable state at all, enforced absolutely, in systems
with famously long uptimes. The relationship to Heroes is close but the motive
is inverted, and that inversion is instructive: Erlang removes sharing **for
concurrency** — isolated processes, a genuine copy on every message. Heroes
removes sharing **for local readability** — copy-on-write, sharing freely at
the physical level as long as nobody writes. Same principle, opposite engines.
Erlang pays copies always and buys isolation; Heroes pays copies only on
mutation and buys the absence of aliasing bugs.

**Swift.** The most direct precedent, and the most reassuring one, because
Swift is not a research language. Its `struct`s, `Array`, `Dictionary`, and
`String` are all value types with copy-on-write, backed by reference counting —
mechanically what Heroes does. The difference is that Swift *also* has `class`,
with shared reference semantics, and therefore inherits everything Heroes
escapes: reference cycles, `weak` and `unowned`, and an atomic counter to make
sharing thread-safe. Heroes takes the half of Swift that works and declines the
half that generates the complexity.

**Hylo (formerly Val).** The current research frontier for exactly this thesis:
that value semantics plus mutable value semantics gives you memory safety
without a borrow checker. The specific insight Heroes depends on comes from
here — **if values are never aliased, reference cycles cannot be constructed,
so reference counting is complete without a cycle collector.** That is not a
compromise; it is the reason the whole design is small.

### Rejected alternatives, and what each would have cost

| Alternative | Why not |
|---|---|
| Pure reference counting with shared references | Cycles become possible, so it needs a cycle collector or `weak`, plus the aliasing bugs the design exists to remove. This is the trap Swift fell into via `class`. |
| ARC / ORC (Swift, Nim) | Same as above, plus compiler complexity for ownership optimisation. ORC additionally needs a cycle collector for its `ref` types. |
| Tracing garbage collection | A runtime we cannot write, cannot explain in a spec, and cannot cheaply hand across an FFI boundary. Violates Part 1.11 directly — a GC'd heap is hostile to C interop. |
| Borrow checker | Unnecessary here. Its whole job is making aliasing safe; with no aliasing there is nothing to check. The cost — lifetimes in the surface syntax, in the spec, and in every error message — is the largest single expense we avoid. |
| Manual allocators (Zig) | Correct and honest, but it puts allocation decisions in every function signature, which is spec tokens and a whole class of plausible LLM error. |
| C++ RAII with references | Manual lifetime reasoning, and the aliasing question in every signature. |

### What the choice buys and costs, stated plainly

**Buys.** No garbage collector to write. No cycle collector. No `weak` or
`unowned` in the language. No borrow checker, no lifetimes, no ownership
annotations. No aliasing bugs and therefore no data races by construction. No
`null`. A counter whose only job is deciding when to copy, and which can stay
non-atomic because nothing is ever genuinely shared.

**Costs.** O(n) mutation of genuinely shared data. No linked structures or
general graphs in the language — those go through the arena-plus-indices
pattern. No shared-mutable-state design patterns: observers, caches, pools,
global registries. No high-performance data structures written in Heroes
itself. The honest summary: **Heroes is strong at transforming data and awkward
at interlinking data.** Since performance is an explicit non-goal (Part 2) and
the target programs are compilers, parsers, and data transformation, this is
the right side of the trade — but it is a trade, and the implementer should not
be surprised by it.

## Audit against the document

Four of the eleven load-bearing claims are already in `design.md`, and citing
them is cheaper than restating them.

| Claim | Where it already lives |
|---|---|
| copy-on-write is Swift's choice | §4.10 ("This is copy-on-write, Swift's choice"); appendix, *Swift* — "`inout` (our `@` parameters), copy-on-write, leading-dot variant syntax" |
| no aliasing → no cycles → refcounting complete, no cycle collector | §4.10 gift 2, in almost these words; appendix, *Hylo / Val* — "the direct ancestor of Part 4.10" |
| the borrow checker is *unnecessary*, not deferred | §4.10 gift 3 **and** Part 6's table row, which already says "**unnecessary** — value semantics removes aliasing, so there is nothing to check" |
| tracing GC is hostile to the FFI | §1.11 line 394: "a tracing GC would need stack maps and safepoints, and would force pinning and finalisers around every pointer handed to C" |
| performance is what licenses all of this | Part 2's first bullet names value semantics with copy-on-write explicitly as the thing it licenses. The citation in the text checks out |

**Genuinely new, and worth having.** Three things the document does not contain
anywhere:

1. **The Swift inversion.** §4.10 and the appendix take Swift as a positive
   precedent and stop there. That Swift *also* has `class`, and therefore
   carries cycles, `weak`/`unowned` and an atomic counter — the half Heroes
   declines — is the sharpest available statement of what §4.10 is buying, and
   it is nowhere in the document.
2. **The Erlang inversion.** The appendix has Erlang as "immutability plus
   message passing chosen specifically for concurrent systems. The reason value
   semantics leaves the concurrency road open." The text supplies the part that
   is missing: same principle, opposite engine — Erlang copies always and buys
   isolation, Heroes copies on mutation and buys the absence of aliasing bugs.
3. **The rejected-alternatives table.** §3.2 documents rejected *backends* so
   they "don't get relitigated"; there is no equivalent for memory models.
   ARC/ORC, manual allocators and C++ RAII appear nowhere. The Zig row is the
   strongest of the five because its argument is in the currency §1.2 uses:
   allocation in every signature is spec tokens plus a plausible-error class.

**The K/APL entry needs its axis stated, or the document contradicts itself.**
The appendix already lists *K / APL* — as "the empirical evidence that
ultra-compact notation **fails** for LLMs, and not only for lack of training
data". The text cites the same family approvingly. Both are correct and they
are about different axes: the notation is rejected, the value model is adopted.
Unstated, a reader finds the same languages cited for and against on the same
page.

**Dangling: the arena-plus-indices pattern.** "Those go through the
arena-plus-indices pattern" cites a pattern `design.md` does not contain —
`arena` and `indices` do not occur in the file. The pattern exists in
`CLAUDE.md` §5 as the **Cyclone rule for the Rust bootstrap compiler** ("owned
data everywhere, indices for links"), which is a rule about writing the
compiler, not about writing Heroes. Whether Heroes programs get the same
pattern named for them is undecided, and §4.10 currently ends its data-model
discussion at the one-element-array wart.

**Precedent still needing a source.** `/panel`'s historian rule — unsourced
precedent is inadmissible, and that role is the most hallucination-prone — does
not relax outside a panel. Three claims are unsourced and none is verified
here: *R and MATLAB have copy-on-write value semantics for their core data
structures* (absent from the document entirely, and the strongest of the new
precedents because it is an argument from scale); *K runs some of the fastest
financial databases in existence* (a superlative, and load-bearing for "the
value model is not what makes a language slow"); *ORC needs a cycle collector
for its `ref` types* (checkable against Nim's own documentation, and Nim is
already a primary source in the appendix).

## What was settled

The argument holds, and its centre of gravity is not the lineage but the two
inversions and the table: **the document currently records what value semantics
buys, and nowhere records what the alternatives would have cost.** §3.2 exists
for exactly that reason on the backend axis, with an explicit rationale — "so
they don't get relitigated".

## What stayed open

None of this lands in `design.md` from here. §4.10 is Part 4 and Part 6 is
Part 6: CLAUDE.md §4 makes both a panel path, and a reasoning note "never
amends design.md, and does not pre-empt a verdict" (this directory's README).
Handed off:

- `docs/panel/OPEN-QUESTIONS.md` watch list — the K/APL double citation, and
  the arena-plus-indices dangling reference.
- `docs/debrief/QUEUE.md` — the three unsourced precedents, to be sourced
  before any of them is admissible; and the panel trigger for a memory-model
  counterpart to §3.2.
