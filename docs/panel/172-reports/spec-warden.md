# Panel 172 — spec-warden

**verdict: object** (not provisional: every number below is measured on
`claude-opus-5` through `POST /v1/messages/count_tokens`, in a copy, today)

**section: design.md §1.6 (ceiling and payment rule), §1.2 (real cost), Part 6
line 2667, and CLAUDE.md §13.**

The budget is not where this proposal is wrong. The ceiling is **10240**,
grepped at the start of this sitting and not taken from my brief
(`grep -n '1\.6' docs/design/design.md`, then
`awk '/^### 1\.6/,/^### 1\.7/' docs/design/design.md`): *the entire language
specification must fit in 10240 tokens, measured by `claude-opus-5`*. Every
draft below clears it by more than 1800. **My objection is Part 6 and the
burden of proof, and it is named as a veto condition at the end.**

## The prices, measured

Baseline confirmed exactly as the brief states it.

    cd <scratch>/spec-warden-172
    . /Users/joseph/Temp/heroes/heroes-lang/.env
    ./heroes measure spec/heroes-spec.md              # vendored
    ./heroes measure spec/heroes-spec.md --refresh    # real + digest
    git checkout -- spec/heroes-spec.md               # between every draft

| draft | vendored | dv | **real** | **dr** | digest |
|---|---|---|---|---|---|
| **baseline** `abf9a17e` | 6172 | . | **8216** | . | `2b1556634e73455a` |
| **A1** merged into the `consumes` sentence | 6203 | +31 | **8258** | **+42** | `008654ed7f54bf63` |
| **A2** appended as its own sentence | 6204 | +32 | **8262** | **+46** | `104ab45035caacee` |
| **A1'** merged, naming whose memory it is (mine) | 6208 | +36 | **8269** | **+53** | `7ddda6086951f675` |
| **B1** three words, keeps-word `borrows` | 6221 | **+49** | **8280** | **+64** | `35ecf9bb207a85ce` |
| **B2** three words, keeps-word `keeps` + grammar | 6226 | **+54** | **8286** | **+70** | `1c448f2b11707de4` |
| **B3** silence still means keeps, `lent`/`consumes` spelled | 6205 | +33 | **8258** | **+42** | `227c7c7124271826` |

Each draft was applied to the real path in the copy, measured, and reverted;
the copy is clean (`git status --short` shows only the untracked brief and
report directories).

Verbatim text priced, so the implementing session can reproduce the digest:

- **A1'**: *On a `cstr` or `ptr` parameter it says C frees what it is handed, so
  nothing Heroes frees reaches it: no lend and no lease.*
- **B1/B2**: *A lend lives for its call and no longer. A pointer parameter says
  `lent` (read and let go), `borrows`|`keeps` (kept, never freed) or `consumes`
  (freed): a lend reaches only `lent`, a lease only `lent` or `borrows`|`keeps`,
  and one saying nothing takes neither, admitting a pointer C made and `nullptr`
  alone.* B2 also adds `| "keeps"` to the `CParam` production at line 400.
- **B3**: *a parameter is taken to keep what it is handed unless declared `lent`
  (read and let go) or `consumes` (C frees it), a lend reaches only `lent`, and
  neither a lend nor a lease reaches a `consumes` parameter.*

### Three things the table settles

1. **No budget veto exists.** Worst case is B2 at 8286 real; plus the 60-token
   FFI floor that is measured against the ceiling, 8346 against 10240. 1894
   free. I cannot veto on §1.6 and I say so rather than pretending headroom is
   tight.
2. **B2 breaches `DELTA_GATE`.** 50 vendored tokens per commit, on both judged
   documents (`tests/harness/suite_spec.hero:181`,
   `selfhost/measure/judged.hero:156`). B2 is **+54** and goes red. **B1 is +49,
   one token under**, which is not a margin: any editorial word added during
   implementation breaks it. Route B cannot land in one commit without either a
   named removal elsewhere in the spec or a split, and no brief says so.
3. **Merge beats append, again, and barely.** A1 8258 against A2 8262: **4 real,
   1 vendored**. The rule the last two sittings recorded holds, but at this size
   it is not what should decide. Placement is: the `consumes` sentence is where
   a reader looking up `consumes` will be.

## Principle 0: the compiler does not need it

    grep -rn 'consumes' selfhost/cli selfhost/emit selfhost/library_source.hero tests/harness/shell.hero

Seven hits, **none of them a binding**: five are comments
(`emit/ffi.hero:112`, `emit/handle_traffic.hero:78,88`, `emit/ops.hero:174,194`),
one is a test fixture string on a HANDLE type
(`emit/ffi_tag.hero:277`, `freeaddrinfo(ai: AI consumes)`), one is a field read
(`emit/handle_traffic.hero:107`).

    grep -rn '\.lease()\|end_lease' selfhost/

Ten hits, **every one inside a diagnostic string or the builtin inventory**
(`lend_errors.hero:70,161,183`, `lease_errors.hero:22,23,32`,
`inventory.hero:54,103,181,193`). **The compiler hands no lease to anything**,
and the bindings it makes of its own say `lent` already
(`selfhost/cli/process.hero:50-58`, `selfhost/emit/literal.hero:41`). So the
closure list does not reach this form: **it enters on the thesis or it waits.**

The thesis limb is met for *a* repair. Defect 070's reproducer is a measured
§1.12 effect: `check` 0, `build` 0, exit 133 with **zero** stderr bytes, ASan
*attempting free on address which was not malloc()-ed*. Robustness is rank 3 in
CLAUDE.md § Precedence, above token cost, so 500 tokens would be payable. **That
is not the same as the burden being met for THIS form**, which is the next
section.

## Part 6: the form is refused today, permanently, and no brief says so

    grep -n 'consumes\|frees\|give-away' docs/design/design.md
    grep -n '^## Part' docs/design/design.md   # Part 6 opens at 2612

**design.md:2667 is inside Part 6 — Rejected permanently**, and it is this:

> A mark (`acquires`, `borrows`, `consumes`) on a `ptr`, or on any result or
> parameter whose type reaches no handle | **refused because the word cannot be
> read there, and a word nobody reads is a comment that looks like a
> guarantee** (panel 150 R3, ratified 2026-09-15; `error[unread_mark]`) ... Not
> widened to `ptr`, because the type system holds no pointee for one and the
> rule that reads the mark has nothing to key on.

Route A and route B are both exactly that row: a mark on a parameter whose type
reaches no handle. It was ratified **six days ago**. CLAUDE.md §13 says *Where
not to go: anything in design.md Part 6.* Neither the shared brief nor mine
names it, and the shared brief's *What is not on the ballot* does not list it
either. **The sitting is amending a permanent refusal without having noticed.**

The row **can** be narrowed honestly, and I say so because a warden who only
blocks is useless. The row's stated reason is that the word *cannot be read*,
because `check/acquiring.hero` keys its map on a handle's DECLARATION. Route A's
reading keys on nothing: it is a **call-site** question, *does a lend or a lease
land here*, answered by `lend_landing.hero` and `leasing.hero`, which already
answer call-site questions about values with no declaration. So the reason does
not reach route A. The row's own named falsifier is not met either (it asks for
*a program that is correct, needs the mark, and has no handle spelling*, and
070's reproducer is a program that must be REFUSED, not one that needs the mark
to work), so the repair is the one panel 139 used on the borrow-checker row at
2646: **narrow it with a dated correction added underneath, never rewritten**
(CL-005, §12, `.claude/rules/records.md` § A record is never rewritten). design.md
is not measured by §1.6, whose subject is the specification, so that correction
**prices nothing** in spec tokens.

## The third reserved case: it is a fifth shape

design.md:2333-2340 reserves *a buffer that C takes ownership of*, then glosses
at 2340 that **all three reserved cases are about a pointer C made**, and 2341
already classes the lease as the **fourth** case, *bytes Heroes made and Heroes
frees*. A lease into a freer is the fourth case's memory reaching the third
case's word: **Heroes' bytes, given away**. It is neither case 3 under 2340's
gloss nor case 4 as written. A correction goes underneath 2340 and I price
nothing for it.

## The two sentences: different memory, and the draft must keep them apart

Asked to settle this with my own eyes, I read both.

`spec/heroes-spec.md:65-67` sits under *Every value behaves as an independent
copy*: *a function taking one without `@` may still change, or free, what C
holds.* Its `@` is the mutable-parameter mark of line 253, its axis is **reach
in space** (aliasing), and its object is **C's** memory.

`:370` sits in § 13: *a parameter is taken to keep what it is handed unless
declared `lent`.* Its axis is **reach in time** (retention past the call), and
what it governs is a lend, which is **Heroes'** bytes.

**The document does not contradict itself.** Different memory, different axis.
But they describe one syntactic position, and read as one rule they produce
exactly defect 070's belief: *an unmarked `cstr` parameter may free what it is
handed* is TRUE of a pointer C made and FALSE of a lease, whose only legal
release is `end_lease`.

**So yes, the sentence this sitting lands must be written so the two cannot be
read as one rule, and A1, A2, B1, B2 and B3 all fail that test.** Every one of
them says *C frees what it is handed*, which is the licence :65-67 already
grants. That is why I priced **A1'**: *so nothing Heroes frees reaches it*. It
costs **11 real tokens** more than A1 and it is the only draft on the table that
names whose memory is at stake.

## Which draft I would land

**A1'**, at **8269 real / 6208 vendored / +36 vendored against `DELTA_GATE` 50 /
digest `7ddda6086951f675`** — merged into the `consumes` sentence, because merge
priced 4 real tokens under append and because that is where a reader looks the
word up.

Route B is refused on three measured grounds, not on price: B2 **breaches
`DELTA_GATE`** at +54 and B1 clears it by one token; B1's `borrows` makes one
word mean *the receiver does not own* on a result (`:385`, C keeps the handle it
hands back, so Heroes must not free) and *the receiver does keep* on a
parameter, a polarity flip by position that is a rewrite-rate generator under
§1.2; and B2's honest spelling `keeps` needs a new keyword, which drags
`selfhost/keywords.hero` and the `grammar` suite in (`.claude/rules/verification.md`,
the spec row is judged by `spec` `special` `grammar`). The corpus cost is the
shared brief's, uncontested: **12 lease sites in 11 files** all move under B,
none under A.

My panel 170 finding stands and I do not hide that it argues the other way:
route A **fails unsafe** when the word is forgotten, which is 070 itself, and
route B fails safe. Route B is the more robust axis and CLAUDE.md § Precedence
would normally take it. **It still does not land today**, because it cannot be
written in one commit under `DELTA_GATE` and because it has the same Part 6
obstacle as A with twelve call sites attached. Record B as the conservative
option refused on instrument grounds, so the author can take it.

## Prediction, falsifiable, on an instrument that exists

`tests/harness/suite_spec.hero` (`spec` suite) pins `SPEC_REAL_TOKENS` and
`SPEC_DIGEST` and goes STALE when they disagree with the document.

**I predict A1' as pinned above lands at exactly 8269 real, 6208 vendored,
digest `7ddda6086951f675`, and `./heroes run tests/harness/main.hero -- ./heroes spec`
is green.** If the implementing session rewraps § 13's paragraph rather than
inserting the three lines verbatim, the real count will differ from 8269 by more
than 3 and the pin will be wrong. Falsified by one `heroes measure --refresh`.

**And a rewrite-rate number.** `tests/golden/check/unread-mark.hero:18` and `:26`
both become legal under any draft here, and `:26`
(`function strdup(s: cstr consumes) -> cstr`) is **FALSE**: `strdup` does not
free its argument. **I predict that if route A lands without moving that line,
`check` reports green on a binding that lies**, which is a worse outcome than
070 and costs more than the 500 to 2000 tokens one correction round trip is
worth. Run `./heroes run tests/harness/main.hero -- ./heroes check` after the
widening; the line must be deleted or rewritten in the same commit.

## Veto condition

**I veto if any draft here lands in `spec/heroes-spec.md` without, in the same
commit, a dated correction added underneath design.md:2667** narrowing panel 150
R3's Part 6 row to say that `consumes` on a `cstr` or `ptr` PARAMETER is read at
the call site and keys on no declaration. CLAUDE.md §13, design.md Part 6, CL-005.
Not a price: a refusal.

I also veto **B2** on `DELTA_GATE` (+54 against 50) unless it carries a named
removal, and I object to **B1** landing on a +49 margin without one.

**What would change my verdict to approve**: that correction drafted and in the
commit, plus A1' or a truer sentence at or under +53 real, plus
`tests/golden/check/unread-mark.hero:26` repaired in the same commit.

**Unrun in those words**: I did not compile any draft, did not run the `spec`,
`grammar` or `check` suites, and did not run defect 070's reproducer myself; the
070 numbers above are the shared brief's, quoted as its measurement and not as
mine.
