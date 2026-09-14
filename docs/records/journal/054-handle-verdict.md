# 054 — M-handle-verdict: a handle is a pointer with a name, and three defects turned out to be one question

## Goal

Three defects were open when this milestone began, filed on three different days
against three different symptoms. **They turned out to be one question asked
three times: what does a `ptr` NOT carry?**

- **029** — it carries no pointee type, so `sqlite3_step(db)` passes a database
  where a statement belongs, compiles, and segfaults on one platform while
  answering wrong on another.
- **030** — it carries no identity, so two copies of a record holding one reach
  a single C object and `==` compares an address that C is free to reuse.
- **031** — it carries no ownership, so one copy can `free` what every other copy
  holds, at exit 0, with `heap-use-after-free` under the sanitiser.

All three closed on the same day, and the thing that closed the first of them is
one sentence of language: **a fieldless `record` inside an `extern` group, with
a `tag` and without `partial`, is C's pointer to what the header names and leaves
opaque.** `record Db tag sqlite3` is `sqlite3 *`. The tag is written verbatim.

Panels **145** and **146**, inheriting **139**. Nine steps if the scouting is
counted, eight if it is not.

## What surprised

**A measurement's headline can say the opposite of what it means.** Step 3 built
the sixteenth mutation operator, `swap-ptr`, deliberately *before* the form it
was meant to judge — so that the denominator would still be comparable
afterwards. It read **15 mutants, 8 killed, 53%**, which looks like a language
that already catches half of this. It catches **none** of it: the eight died on
`unused_binding` and `aliased_mutable_arguments`, rules that fire on the shape of
the edit and know nothing about handles. Where the swap is a pure use — defect
029's own shape — the score was **0 of 7**. After the form: **15 of 15**, and the
honest half **7 of 7**.

Had the operator landed *after* the form, retyping the bindings would have hidden
every site and the form would have been scored against silence. That failure was
named in the measurement's own prediction before it could happen.

**The cheapest resolution was wrong three times, and the seat that found it was
the one added this milestone.** A completeness critic sat beside the five judges,
and it changed the resolution in every sitting it attended:

- at 145 it measured that `curl/curl.h` reads `typedef void CURL;` on this Mac's
  SDK, so the `struct <tag> *` spelling **every one of the five seats had
  proposed** would have reached one of the three handle types in the tree. The
  form writes the tag verbatim because of that one header;
- at 145 it read the code and said the consume rule would refuse **2** of the 17
  correct SQLite calls where the compiler-engineer predicted 0. Measured: **2**,
  and they were the two it named;
- at 146 it refused the coordinator's central sentence and compiled three
  counter-examples to it.

**The form cost a quarter of what it was priced at, and the specification's fence
was the half that decided it.** Five scouts priced ~203 lines and the engineer
estimated 60 to 90; it landed at ~170 with **zero new type cases** and the
checker's type table untouched. But two seats found independently, from different
documents, that §13's *fenced example* mattered more than its prose: a form the
document does not SHOW is a form a model never writes, and the old fence taught
`@out: ptr` and `sqlite3_close(db: ptr)` — the two signatures defect 029 was
filed against. The fence was rewritten, and a suite runs it as a program.

**A repair's real cost was in a word nobody counted: the consume mark
propagates.** `consumes` refused 2 calls of 17, which reads cheap. Marking those
two wrappers `@` obliged every caller to hold a cell rather than a binding —
**24 lines across `examples/ledger/`** for two marks. Twelve times the refusal
rate, and it was not in anybody's estimate.

**And the last sitting, convened to decide a word, found a cast.** Panel 146 sat
on the author's own order that it wait until the form had landed so it could
argue over a number. `ptr` kept its name, four seats to one advisory objection —
the ffi seat renamed every `ptr` spelling in the emitted C and compiled both to
byte-identical object files, the ergonomist wrote four programs from the
specification alone and found the load-bearing line unchanged under every
spelling. **The thing the sitting
actually bought was elsewhere**: the critic showed that `@tail: ptr` makes this
compiler emit `void ** a4` and a call that writes `(void *)a4`, **a cast**, where
`@tail: cstr` emits `const char ** a4` and hands clang the exact type to check.
Robustness outranks naming by the contract's own precedence order, so the
narrowing landed and the naming argument became the smaller half of its own
sitting.

## What it cost to be wrong

**The coordinator's own prose carried seventeen framing errors, and the tally
was wrong three ways before anybody counted it.** Four in panel 145's census.
One clause reported to the author as the sitting's ratified text, which no
sitting had written — the correction was *I wrote it myself, in the brief I
prepared for the scouts*. Six in panel 146's brief, including a false claim in
the very section headed **house rules that bind the verdict**, and two
occurrence counts that were line counts because `nullptr` contains `ptr`, which
is the identical error the brief's own text had confessed three lines earlier.
And six more in panel 146's own file, found after it closed by an adversarial
review of the whole step and corrected in place.

**Not one of the seventeen is a wrong verdict.** Every one is a wrong number, a
wrong unit, or a wrong citation underneath a verdict that held — which is the
finding M-deferral-ledger arrived at nine times one milestone ago, *an item's
stated reason is measured false before its verdict can be written*, now happening
to the prose that reports the verdicts rather than to the verdicts.

Every one was found by somebody running a command. None was found by rereading.

**A refusal was proposed, written into a brief, and withdrawn on evidence before
it was implemented.** A `tag` naming a type the header declares COMPLETE was to
be refused — until `record File tag FILE` was compiled and run. `FILE` is
complete on this SDK and the binding writes through `FILE *` correctly. The
fieldless form *means the pointer*, so the completeness of the tag is irrelevant,
and refusing it would have broken a legitimate binding.

**And three defects were introduced or exposed by the work itself**, each found
by attacking the shape next to the one in hand:

- the emitter braced a **handle used as a record field** as though it were an
  aggregate, so clang warned twice on a correct program. Measured: **no handle
  was a field of a record declared INSIDE an `extern` group**, which is the only
  position the completeness probe reaches, and which is how the form reached step
  4 without ever meeting the one position that breaks it. (Said at first as *not
  one anywhere in the tree*, and falsified by grep: three handles ARE fields of
  plain Heroes records, which get no probe and so could not have found it.);
- the editor's TextMate grammar admitted **four** escapes where the spec gives
  **five**, so a legal `\r` was painted `invalid.illegal` — a correct program
  shown as an error. It had been **measured three days earlier**, written into a
  rules file, and left, and that file had named the reason in the same paragraph:
  *nothing judged either file until then*;
- the build cache **replayed warnings from a compile that no longer existed**,
  because the saved file was written only when clang spoke, so a clean recompile
  left its predecessor's text in place. A repaired program reported its old
  warnings forever.

## What changed in the language

One form, one word, and two refusals.

**The handle**, above. **`consumes`** after a C parameter, saying the call ends
that value's life, with the rule that such a call may not be handed a value the
caller borrowed — **114 code lines**, against the 150 the sitting set as its
condition. The rule is local: the argument's place root, and what the resolver
says that name is. No flow analysis, which the checker does not have.

**Refused rather than implemented**: a handle as a map key, directly or through a
record, because a handle is an address C recycles, so a value stored under a
closed handle would be found again by a fresh one. And **two records sharing one
tag**, because they are two Heroes types and one C type, so this compiler refuses
handing one where the other belongs while clang accepts it — a mutant that
survives every instrument the language has, permanently.

**The name `ptr` survives**, with a falsifier attached that did not exist before:
*a compiled program in which a bare `ptr` reaches a position not inside and not
adjacent to an `extern` block*.

## What it moved that nobody scheduled

**design.md's Part 6 borrow-checker row fired its own falsifier.** That row had
named it: *an annotation a binding author writes once per function, with a
measured refusal rate on `examples/` below the 13 of 17 above*. The consume mark
refuses **2 of 17**. So the row's ground moved from soundness to cost, in its own
words, on the day the mark landed, and the row now says so with the three costs
named.

**Panel 140's `raw`-module row lost a number and gained a stronger argument.**
That row measured `ptr` escaping an `extern` group **11 times in 2 files**. The
handle form took nine and the narrowing of `@tail` took the last two: it is now
**zero**, so a region whose purpose is to fence the dangerous operations has
nothing outside a group to fence. The row's other figure could not be reproduced
under any reading, and is corrected as a **question** rather than as a number —
because a figure whose unit was never written down cannot be re-measured, so it
never expires; it just stops being checkable.

## What is still owed, said rather than implied

Three questions the last sitting refused to decide, because a sitting convened on
a name may not settle them:

- whether the **13 lines across 7 files that recommend `ptr`** are still right
  after the handle form. The sitting claimed one of them was measured wrong;
  running all thirteen at the close withdrew that — each names a position no
  handle can reach, and for the `@tail` position the compiler was already
  recommending `cstr`. So the open question is the other way round: should a
  **fourteenth** place now recommend the handle where it says nothing?
- the **§ 3 routing clause**. A reader chooses a type in § 3 and does not meet the
  handle until § 13, three hundred lines later, and both are glossed with the same
  word, *opaque*. The rule is derivable and never written;
- whether **`ptr` is an FFI type or a core type**. Every argument at the sitting
  assumed the former; one six-line fixture with no `extern` group says otherwise.

And the older debt this milestone answered rather than paid: the two tools that
**colour** a program still know none of the seven contextual marks. Measured —
none of the seven is a keyword, so colouring them unconditionally would paint a
variable named `tag` as syntax, trading an under-colouring for a mis-colouring.
That half is refused with the measurement that refuses it, and the half that was a
real defect is fixed with an instrument that holds the grammar to the compiler's
own tables.

## What landed, and what carried forward

**Three defects closed and the form that closed them is one sentence.** A
fieldless `record` inside an `extern` group, with a `tag` and without `partial`,
is C's pointer to what the header names and leaves opaque, the tag written
**verbatim** and never `struct <tag> *` — a spelling every one of five seats had
proposed and one header falsified. `nullptr` is its null, construction is
refused, `==` compares the address, and a handle does not convert to `ptr`. All
three shipped bindings use it: `examples/sqlite`, `examples/ledger` and
`examples/curl`, the last of which is the case the critic raised, `CURL` being
`typedef void CURL;` on this SDK.

**`consumes` is the word, and its rule is local.** A call that ends a value's
life may not be handed one the caller borrowed: the argument's place root, and
what the resolver says that name is. **114 code lines** against the 150 the
sitting set, refusing **2 of 17** calls in the shipped SQLite binding — which
fired design.md's Part 6 borrow-checker row's own falsifier and moved that row's
ground from soundness to cost, in its own words, the day the mark landed.

**Two refusals rather than implementations**: a handle as a map key, because a
handle is an address C recycles; and two records sharing one tag, because they
are two Heroes types and one C type, a mutant that survives every instrument
this language has, permanently.

**And the name survives, with a falsifier that did not exist before.** Panel 146
kept `ptr`, four seats to one advisory objection, refused the `opaque` family by
name on two independently measured grounds, and narrowed `@tail` to `cstr` at
both sites — which is the thing that mattered. Panel 140 had measured `ptr`
escaping an `extern` group **11 times in 2 files**; after the form and the
narrowing it is **0**, and the eight positions left are every one of them inside
a group.

**Predictions scored at this close.** Panel 145's spec-warden **holds** on the
half that could be scored: § 3's `ptr` sentence is byte-identical to the
milestone's start, measured by diff. Its other half is **unscorable** and is
marked so rather than claimed — `two.hero` was a scratchpad witness and is not in
the tree, so nothing can re-run it. Panel 145's historian **holds vacuously**:
`docs/work/DEFECTS.md` is at zero, so no defect was filed against defect 030's
class this milestone; the prediction runs for two more closes. Panel 146's
engineer prediction on `swap-ptr` under a rename is **unscorable by
construction** — no rename happened — and is left standing for whenever one is
attempted.

**What carries forward.** Three questions the last sitting refused to decide,
each filed rather than smuggled through a sitting convened on a name: whether the
**13 lines across 7 files** that recommend `ptr` are still right, and whether a
fourteenth place should now recommend the handle; the **§ 3 routing clause**,
which is the defect that sitting turned out to be about; and whether `ptr` is an
FFI type or a core type, which one six-line fixture says it is not.

**Chain entry.** Row 55, **M-handle-verdict**, done 2026-09-14, tag
`m-handle-verdict` — the ruling on telling one C handle from another, and the
three defects that were one question.
