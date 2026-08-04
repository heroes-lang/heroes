# Panel 012 — the spec budget rises to 3000 (retro-record)

Date: 2026-08-04. **Decision already taken by the author**; retro-record per
`/panel` step 1 — record the consequences, do not stage dissent. No judges
were convened: panel 011 had measured the artifact hours earlier and both
standing judges had already reported on the same question at 2000. Their
findings are carried forward below rather than re-elicited, which is the
honest thing to do with a number that has now moved twice in one day.

## The decision

design.md §1.6's budget rises from 2000 to **3000 tokens, measured** — the
maximum over the two vendored tokenisers. Applied to §1.6, §"The name",
Part 10, CLAUDE.md, both judge briefs and the site.

## Why this raise is different from the last one

Panel 009 raised 1500 → 2000 and its own resolution, ratified by the author,
set the condition for any further move: *"No second raise without a
measurement, a panel, and a named alternative (cut features instead)."*

- **The measurement now exists** (panel 011, verified independently): spec
  v0 is 1989 / 2048 / 2050 tokens on three real BPE encoders, against the
  1496 the word heuristic had claimed. This is the first budget number in
  the project's history chosen with the artifact's real size in view.
- **The named alternative was cutting features**, and it is worth stating
  what that would have meant: v0 is *already* 2050 on the binding
  instrument, so holding 2000 required a net-negative amendment before
  anything new could land. The mortgaged closure items alone (modules, file
  I/O, `args()`, `exit`) measure ~129 tokens at minimum prose. Holding the
  line meant cutting specced surface — not deferring additions, removing
  what is already there.
- **What 3000 buys**: ~950 tokens of headroom on the binding instrument,
  which is roughly the mortgage plus the three deferred sentences (escapes,
  layout, the `()` row) with room left over.

## Consequences, carried from panels 009 and 011

**The forcing function weakens again, and the governance is what holds.**
Panel 009's rules survive the raise verbatim, with the number substituted:
measured-only; the purse is pre-allocated, not headroom, and *"there is
headroom" is never a reason*; spend only where a wrong guess would be
**silent**; no further raise without a measurement, a panel, and a named
alternative. The soft/hard split rescales to **soft 2000 / hard 3000**:
under 2000, §1.0 alone; between 2000 and 3000, §1.0 plus a named removal or
a pre-registered falsifiable prediction; over 3000 measured, veto.

**The one-person-implementability proxy is now doing all the work.** §1.6's
original argument was never prompt economics — it was that a spec fitting in
a couple of pages describes a language one person can implement. Wirth's
Oberon report, the only calibration the section cites, is sixteen pages. At
3000 tokens Heroes is still an order of magnitude under that, so the proxy
holds; but it is the only leg the number still stands on, and it should be
said out loud rather than implied.

**Attention risk is unmeasured at this size.** The ergonomist's panel-009
position was that 1500–2000 sits comfortably inside the flat region of
instruction-following, with positional degradation appearing far later. 3000
is a further step in that direction and nothing here measures it; the
editorial risks it named (contradiction risk growing superlinearly with
length; material appended at the end read as an appendix) get correspondingly
larger. Its drafting rule therefore binds harder, not softer: **insert each
addition at the rule it completes, never as a trailing section**, and keep
every addition to one sentence with a concrete literal in it.

## Standing predictions, restated against the new ceiling

- warden (011): the closure-complete spec — v0 plus §1.0's mortgaged items
  at minimum prose — lands ≥2100 cl100k. Under a 3000 ceiling that is no
  longer a breach; it is **70% of the budget consumed by debt booked before
  the raise**. Scored at the M6 audit.
- warden (009, still open): the discipline is what keeps 3000 from becoming
  4000. The next raise request is the falsifier.
- ergonomist (009): tokens that close a *silent* corner earn their place;
  tokens that prevent a loud compile error do not. Unchanged, and more
  load-bearing at 3000 than at 2000.
