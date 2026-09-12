# 052 — M-declared-thresholds: a number that says which way it may move

## Goal

The author had been hitting their head on this repository's ceilings and asked
for a census of every number that acts as one: which make sense, which should be
widened, which rounded. Then the same question of the floors.

What the census found is that the two are one defect with the sign flipped, and
neither is bad luck — it is the shape of the number. **A ceiling posted at the
value measured the day it was written has no room to be obeyed**, so the first
legitimate addition hits the wall: CLAUDE.md stood at 5491 against 5500 with nine
tokens of room, § Where we are at exactly 15 lines of 15. **A floor posted the
same way drifts out from underneath as the tree grows**, so it stops catching,
in silence, with nothing ever turning red.

Three of the four ceilings had the first problem. Eight floors had the second.

## What surprised

**The contract had been 1806 tokens over its ceiling for three days, and every
instrument in the tree said it had nine to spare.** `CLAUDE.md` measures 7306 on
`claude-opus-5` against a stated 5500, because the contract alone was still
judged on the maximum over two vendored tables whose larger is OpenAI's — the
spec had moved to the reader's own count on 2026-09-09 and nothing carried the
move across. This is §1.6's own history happening one document later: the spec
was once 998 over a line it was told it was 40 under. The trace was already in
the tree, `docs/measurements/023` recorded the contract at 7190 real on
2026-09-09, and nothing read it.

**The two ceilings do different jobs, and the sitting had to be corrected on
it.** Panel 134 argued *"two documents with ceilings belong on one scale"* and
reached for the spec's 8192. The author overturned the framing: the spec's
ceiling bears on the LANGUAGE, where every token is a feature paying rent and
squeezing is the point; the contract's is HOUSEKEEPING, and reaching it cuts no
rule — one MOVES, into `.claude/rules/` where it loads only when its paths are
touched, into the case law, or into a skill. So the contract's number measures
the fixed cost paid on every turn and nothing about design, and it is set high on
purpose: 12288, written as `7306 + 4982`, about fifty days of room at the
measured rate.

**A defence can be sound about what a second copy PROVES and wrong about the
form it must take.** `suite_layout`'s ceiling table had two of its rows incised
in an assert below it, defended in a comment as the proof that the lookup reads
the table. It is a real property and the comment was right about it — and keeping
the two copies in step had cost three red CI legs on 2026-09-11 and a second hand
edit the next day. The property is provable FROM the table, for all eighteen rows
instead of two, resting on something the table cannot stop being true of itself:
every row is above CEILING by construction, since that is the condition under
which a row leaves it.

**A number can be written twice and prove nothing.** `assert WHERE_CEILING == 15`
stood under the words *"a number this file states rather than one it remembers"*
and compared a constant with itself. No defect could fail it. It is gone rather
than raised.

**The most protected corpus in the repository had the slackest floor.**
`tests/golden/check` is where `UPDATE_GOLDEN=1` is forbidden outright by the
hard stops, and its floor sat at 65 against 112 cases: 47 could have stopped
running in green.

**And the historian found no precedent for the thing we were doing.** It searched
for an internally-owned, instruction-raisable size budget that ever held its
level and found none — Angular's answer became a vendor page titled *How to
Increase Budget Size*, webpack ships its budget as a warning, the kernel disabled
`CONFIG_FRAME_WARN` rather than defend it, Chromium has no absolute limit at all.
The one shape that held is the ratchet, where the number moves one way with the
human out of the raise. It was equally blunt about the delta gate adopted here:
Chromium runs a 16 KiB per-CL gate with a mandatory footer and grows ~100 KB a
week regardless, so **a delta gate bounds attribution, not level**. That sentence
is in the code rather than in a hope.

**A citation this repository had been making is not true.** The contract's own
comment attributed *"under 200 lines"* to Claude Code's documentation. The
historian fetched the page: the figure is not on it, it is third-party blog
content. What the page says is qualitative, and the measured evidence nearby is
of the adjacent effect. There is no published size at which adherence falls, so
the ceiling is an informed choice and now says so.

## What broke and why

**The seat's prediction scored TRUE and its own pre-emption was the repair.**
The compiler seat predicted that carrying a second judged document would push
`cli/measure.hero` past §11's 300, forcing a split or a nineteenth `DECIDED` row.
It reached 337. Two seams were cut rather than a row added:
`measure/vendored.hero` (what the offline tables say) and `measure/judged.hero`
(which documents are judged, against what, and what is left).

**A defect that would have made the whole change a no-op, found by a judge and
not by a test.** `maximum_of` accepts exit 0 and exit 1, and `measure` prints the
`maximum` row before it can withhold a verdict — so the `contract` check would
have gone on comparing a VENDORED number to a REAL ceiling: green forever, on the
scale the change exists to abolish, hiding exactly the gap the contract was
already inside. There is a `real_of` now, and a stub in the case prints both rows
disagreeing on purpose so one assertion separates them.

**Two seats contradicted each other and the resolution took what both were
asking for.** The warden made a content digest for the contract a condition; the
engineer vetoed it on a measurement — 34 commits in seven days against the spec's
17, so a digest would withhold the verdict about that often, clearable only by a
network refresh and not clearable at all on a fork PR. One vendored
`CONTRACT_TOKENS` pin does the three jobs: staleness detector, delta gate
baseline, and the movement detector the contract never had.

**The gate fired on this milestone's own edit, which is the only proof worth
having.** Rewriting the paragraph in CLAUDE.md that describes the new mechanism
came to +115 vendored tokens; `measure` refused the verdict; the text was cut to
+65 and the pin refreshed with the reason in the commit body.

**The language refused one attempt to test the new check.** Breaking
`ceiling_for` by ignoring its parameter would not compile: `unused_binding`
fires. The falsification had to be done the other way, returning the fallback
from inside the match, and then the case failed with `left: 300, right: 1870` —
which is the message a reader wants.

## What landed, and what carried forward

Landed: the site's ceiling written by the build instead of typed, after three
days at `6K` where nothing checked it; `suite_layout`'s ratchet proved from its
own table; `WHERE_CEILING` 15 → 32 with a tautological assert removed; the
contract judged on the reader's tokeniser at 12288 with a +50 delta gate and a
refused digest; and every counting floor checked from both sides, with eight
re-based to today's count.

Carried forward: `suite_runtime`'s three floors, read in composite conditions
over one sweep, which is its own change. And the named removal panel 134 costed
and nobody spent — CLAUDE.md §5 and §6, about 238 real tokens, where §5 says in
its own words that it *"constrains no new code"*.
