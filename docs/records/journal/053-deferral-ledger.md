# 053 — M-deferral-ledger: every deferred item gets a date, and nine of them lost their reason first

## Goal

Author decision 2026-09-10, § What production-ready means. **Every item in
design.md Part 7 that carried no milestone had to receive one of three dated
verdicts**: ENTERS, with a milestone of its own; REFUSED, as a Part 6 row naming
the program or compiler fact that would make it wrong; or DEFERRED AGAIN, with a
return condition written as a **falsifiable claim** rather than a hope. The
milestone's own file said how: *the steps are the sittings*, one panel per item.
Then it widened, by the same instruction, to the Part 8 warts with no home —
**15**, **16**, then **5**, **8**, **11** — and to **coverage**, the question a
production reader asks and the document answered with silence.

Nine items, nine sittings, panels **136** through **144**, plus **135** which
opened the ledger. No language form was added. What the milestone delivers is
that no item in either list is now a promise with no clock on it.

## What surprised

**The finding arrived nine times, and it was always the same shape: an item's
stated reason is measured false, or expired, before its verdict can be written.**
Not the verdict — the *reason*. The list had been maintained honestly and its
justifications had rotted underneath it, because nothing in the repository read
them again after the day they were written.

- Item 6's clock had run out and nobody had heard it.
- Item 8's two clauses were both wrong: eight cases compiled that the item said
  could not, including a cross-module generic `sort_by`.
- Item 9's justification named a parser **this repository has never had** — the
  expression parser has been precedence climbing since the hour it was written —
  and the unification it promised **would not compile even if the feature were
  granted**.
- Item 11's premise was contradicted by §4.10 **in the same document**.
- Item 14's hinge expired fourteen days after it was written, and the direction
  it left to a count turned out to be a count that cannot decide: three honest
  methods read 49.5%, 50% and 50%.
- Item 16's premise was false one level up — **nothing in this language
  destructures**, so there was no asymmetry to restore.
- Wart 8 was half true and half **twenty thousand times** stale, and §4.10 had
  carried the repair for twenty days while Part 8 went on quoting the old number.

**The second surprise is that a wart can be wrong in the direction nobody
checks.** Panel 143 found it: not by claiming too much, but by **understating
what it costs**. Wart 15's injected carriage return erases C's own error message
from the terminal, so the one line that would name the bug deletes it. Wart 16
was filed as a missing convenience, and the workaround a reader finds silently
reorders output using only what the language ships.

**The third is the sharpest and it closed the ledger.** At panel 144, three
spellings of one accumulation: a bare local at 1 000 000 pushes reads 0.02 s, a
record field at **50 000** reads 11.51 s, and the same field **lent to an `@`
parameter** at 1 000 000 reads 0.02 s again. Twenty times the work in five
hundred times less time, with nothing in the source to tell them apart. **The
cost lives in the spelling, not in the shape** — and the cheap spelling is one
this repository had already applied to itself on 2026-08-26, taking `heroes
check` from 191 s to 88 s, and then forgotten.

**A method surprise, which is the one that generalises.** A sixth agent was added
to the panel this milestone: a **completeness critic** that reads the five seat
reports and names what is missing — a route nobody listed, a claim asserted and
not measured, a contradiction between seats. **It changed the resolution in every
sitting it ran.** At the last one it falsified the compiler engineer's own
conclusion, found the spelling four seats had missed, and caught two stale counts
in entries this very milestone had written the same day. A panel of five
differentiated by input still shares one blind spot: each seat generalises from
the shape it happened to write.

## What broke and why

**The seed was a commit behind `selfhost/`**, and it was found by obeying CL-054
— run the suite you did not expect to move. `2d905817` had changed
`selfhost/measure/` without regenerating `seed/heroes.c`. Repaired, fixpoint
verified byte-identical, green on all three platforms, `eecccb09`.

**Three defects were found beside the sittings, and all three are one family.**
029: a swapped opaque handle compiles at zero diagnostics and segfaults on
Darwin while answering `rows: -1` at exit 0 on Linux. 030: two copies of one
record reach the same C cursor, while the specification says no aliasing exists
anywhere. 031: one copy can `free` what every other copy holds — exit 0, zero
diagnostics, `heap-use-after-free` under AddressSanitizer on two platforms.
**They are the three things a `ptr` does not carry**: a pointee type, an
identity, an ownership. `M-handle-verdict` owns all three.

**Four times the coordinator was wrong and the record says so under its own
name.** A brief claimed two of three names were already values — false, a
function type cannot carry `@`, and the seat caught it. A construction count of
1457 was a regex counting every dotted call with a labelled first argument; the
true figure from the compiler's own AST is about 626. A correction was filed
against a seat for claiming a misnamed file existed in the tree — **and the seat
was right**: the file was there, 8192 bytes, and the search failed **because the
filename contains a tab and a newline**, the wart defeating the instrument
hunting it. And a wart-8 measurement generalised from one shape, which the warden
caught and the critic then corrected a second time.

**The last sitting broke the instrument that judges it.** `records` died with
`panic: string slice splits a character` on a legal line of prose. Bisected to
one line, then to a length: **79 and 80 bytes panic, 81 and 82 pass**, because
`head_of` cut every long line at a fixed 78 bytes and an em dash's three bytes
straddled exactly that offset. The harness's own string module opens with a
paragraph explaining why nothing in it may slice — *a harness that dies on a
filename is worse than no harness* — and the repair that taught it had never
reached this caller. Fixed at the module that owns the reasoning, with a test
that attacks the shapes beside it, and the other three fixed-offset cuts in the
harness were checked and are safe by construction: each follows a proven ASCII
match, and an ASCII byte never appears inside a multi-byte character.

## What landed, and what carried forward

**Part 7 no longer holds an item without a date.** Five left the list for Part 6,
each with a falsifier a reader can check: **traits** (the hole is two operations
wide and the corpus wants one of them), **a variant's case as a value** (its
justification named a parser that never existed), **a `raw` module** (this
document had already ruled against its premise, in this document), **`private`**
(its hinge expired and the count left to decide it cannot), and **symmetric
variant syntax** (there is no asymmetry to restore, because nothing here
destructures). Two were deferred again with dated return conditions written as
falsifiable claims: **`alias`** and **doctests**. Part 6's **borrow-checker** row
was corrected — its stated reason was false, and it now stands on a narrower one
with defects 030 and 031 as the facts that moved it. **Part 9 was corrected
beneath itself** rather than rewritten.

**Part 8's homeless warts now have dated verdicts too.** 15 and 16 stay, with
both entries corrected for understating their cost. 5 stays, with panel 034's
ruling unchanged and its number refreshed from *0 of 25* to **59 mutants, 0
killed, 0% over 120 programs**. 11 stays, its stated cause replaced. 8 stays,
**rewritten so the cost is named in the spelling**, its `Builder` promise struck
on the engineer's veto. And **coverage is refused as the first Part 6 row whose
subject is a tool**, with the column head widened in the same edit so the
precedent breaks loudly rather than silently.

**Predictions scored at this close.** Panel 134's contract prediction **holds**:
CLAUDE.md reads **7402** real against a predicted ceiling of 7700, and **5570**
vendored against 5790. Its spec prediction **holds**: 7531 real and 5655
vendored at the landing commit, exactly as written, the milestone having spent
none of the budget. Its layout prediction is **falsified**:
`code_lines(selfhost/cli/measure.hero)` reads **198** against a predicted
*greater than 300*, so no split and no nineteenth `DECIDED` row were forced.
Panel 138's wrapper scan **holds** at **0**. And panel 134's delta-gate
prediction — *the +50 gate fires on at most 2 of the next 20 commits touching
CLAUDE.md* — **lapses**: only **2** of those 20 commits exist, so it cannot be
scored, and CLAUDE.md § Verification's rule is score or lapse, never renew under
a new milestone's name.

**What carries forward, and the first one is a single command.** Three
corrections to `spec/heroes-spec.md` are drafted, measured and blocked behind
**one** authorised call: § 10's cost condition from this milestone's last sitting
(+35 vendored, measured), § 3's *no aliasing exists anywhere* for defect 030, and
§ 13's *unmarked pointers are never freed* for defect 031. The spec carries a
content-addressed digest and `measure` withholds its verdict the moment the file
moves, so `heroes measure spec/heroes-spec.md --refresh` is the only way back to
a green suite — and it reaches the network, which is asked for every time. **Two
queued items already ask for it**, panels 139 and 144.

**Ten ratifications wait**, panels 136 to 144 and the sitting-brief question.
**Three defects stand**, and by `records/tagged` a tag may not be taken over
them, so **this milestone closes untagged** on M-anchored-spec's precedent at
chain rows 45 to 47; the tag lands on the first commit where both lists are
clean. **`M-handle-verdict` owns all three defects** and is the next row.

**And two doors in the panel protocol are open, both named twice now.** The
historian seat has no file write, so its report never reaches the sitting's
directory and the completeness critic cannot audit it — panel 143 found it and
panel 144 found it again, unchanged. And the llm-ergonomist's isolation is not
enforced by the machinery that convenes it: the seat reported rules files loaded
into its context unasked, refused them as evidence and said so. The seat whose
whole value is reading only the specification cannot today be guaranteed that,
and a guarantee that rests on the seat's own honesty is not one.

---

**The closing block, as § Where we are and § The chain took it.**

> | **Last closed** | **M-deferral-ledger**, 2026-09-13, **untagged while defects 029, 030 and 031 stand** ([053](journal/053-deferral-ledger.md)) — every deferred item now carries a date, and nine times the reason under it had rotted first |

> | 54 | **M-deferral-ledger** | done 2026-09-13 | **untagged**, defects 029, 030, 031 | [053](journal/053-deferral-ledger.md) | every Part 7 item with no milestone gets a dated verdict or a return condition, and Part 8's homeless warts with it · nine sittings, and **nine times the item's stated reason was measured false or expired before its verdict could be written** · five refused to Part 6 with falsifiers, two deferred again with conditions somebody can check · coverage refused as Part 6's **first row whose subject is a tool** · wart 8 rewritten because the cost is in the **spelling**: the same push is 0.02 s at a million through a plain name or a lent field, and 11.51 s at fifty thousand through a field in place |

**Milestones closed 54 of 77, and 53 tags** — one fewer than the closes, because
`records/tagged` allows no tag over an open defect and three stand. The tag
`m-deferral-ledger` lands on the first commit where both lists are clean, the way
`m-cstr-lifetime`'s did on 2026-09-09.
