# The author amends the batch: sixteen to thirty-two defects, gated once

2026-10-03, written at 21:51 by the clock (`date`) and completed at 21:56. The
author, on the coordinator's plan for panel 188's landing (a lane of five
defects, the batch's maximum until then, and a lane of two), gave three
instructions on process, which CLAUDE.md § 4 lets the author give with no
sitting; recorded as a reading. In their words, each followed by what it
meant:

1. *"No, qua mi sa che devi fare dei lotti con dentro almeno una quindicina di
   difetti insieme perché non diventa troppo lungo, eh."* Meant as: *here you
   have to make batches holding at least about fifteen defects together,
   because otherwise it gets too long.*
2. Between 21:51 and 21:53 by the clock read before and after it: *"Metti
   lotti max 20 difetti."* Meant as: *make batches of at most twenty
   defects.*
3. Between 21:53 and 21:55 the same way: *"Scegli tu la forbice ma osa un po
   10 20 16 32."* Meant as: *you choose the range, but dare a little: ten to
   twenty, sixteen to thirty-two.*

**The coordinator chose sixteen to thirty-two.** The open list of that
evening, 47 once the sitting's filings are in (8 `blocking`, 25 `adjacent`,
14 `improvement`), clears in two batches at that size and in three at ten to
twenty, and each batch pays one seed and fixpoint, one full net, one census,
two platform legs and one push. The price is a red gate's bisect of five steps
instead of four. **Measured the same evening, by the commits' dates**: seven
round gates in 27 hours under the rule it replaces, from `e2d59fdb`
(2026-10-02 13:36) to `da0ff6c9` (2026-10-03 16:17), the seventh over one
defect (218).

**What changed**, each rule in its one home:
- `.claude/rules/verification.md` § The batch: a batch holds sixteen to
  thirty-two defects, in as many lanes as its clusters need (one lane per
  cluster of shared files, parallel lanes for disjoint ones), merged into one
  round and gated once; filled from the open list, `blocking` first, then the
  `adjacent` and `improvement` items that share a cluster's files, then the
  oldest `adjacent`, then `improvement`, so that no `blocking` item waits for
  others to be found and a list of fewer than sixteen is taken whole; a batch
  closes when its items are repaired, before any push, or when the author
  asks, never past thirty-two; a red gate's bisect is four or five steps; the
  inline `adjacent` repairs stay at two.
- CLAUDE.md § Verification's line, from *at most five repairs in sequence* to
  *a batch is sixteen to thirty-two defects*.
- `docs/records/contract/case-law.md`, a dated paragraph beneath CL-079.

**The fill order is the coordinator's reading**, not the author's words: the
instructions name a size, and the order keeps § Bounded discovery's *a
`blocking` item is never deferred* true under it. The author may correct it.

**The first batch under it**, round 8, as the coordinator plans it: 23 of the
47, every `blocking` item in, in four lanes whose file sets are to be read
disjoint before they open: the FFI lane, panel 188's landing (216 and the
items filed beside it, with the `adjacent` 225, 226 and 183 on the same
files); the source lane (220, 227); the recovery lane, the oldest `adjacent`
items of the parser (177 to 182 and 184); and the emitter's costs (185 and
228 to 231). The second, round 9, the other 24: the parser's later
`adjacent` items (193 to 202, 204, 205, 208) and the `improvement` items. And
push 3's recommendation changes with it: the seventh round, already gated,
travels with batch 8, one push and one Windows leg for both.
