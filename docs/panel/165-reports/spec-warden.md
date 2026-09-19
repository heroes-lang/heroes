# Panel 165 — the spec-warden's report

**Route 6 — declare the extent on the PARAMETER, `function arr_len(s: i8[8])`.**

- `verdict`: **veto**, on Principle 0's burden of proof. **Not** on the budget:
  there is no budget breach and I say so before anything else, because a warden
  who lets a ceiling argument stand in for a justification argument has done
  neither job.
- `section`: design.md **§1.6** (the payment rule, unconditional at every level;
  *"Want three ways to write a loop? Triple cost, zero gain."*), CLAUDE.md **§2**
  (Principle 0), CLAUDE.md **§12** (measurement beats opinion, including the
  panel's).
- `spec_token_delta`: **REAL, not a lower bound** — see the table. The instrument
  was available this sitting and every figure below came off it.
- `removal`: **nothing — and that is a problem.** Detail in § 6.
- `needed_for_self_hosting`: **no**, measured. § 4.

---

## 0. The instrument was live, so nothing here is a lower bound

`.claude/rules/spec-shape.md` allows a draft to be priced as a *lower bound, in
those words* when `--refresh` cannot run. **It ran.** The brief anticipated the
offline route and it was not needed.

```
$ clang -I runtime seed/heroes.c runtime/runtime.c -o heroes     # real 3.08
$ ./heroes measure spec/heroes-spec.md
  claude-legacy      5965   cl100k_base  6089   real  8106   claude-opus-5, 2026-09-18
$ . ./.env && ./heroes measure spec/heroes-spec.md --refresh     # exit 0
  SPEC_REAL_TOKENS 8106   SPEC_DIGEST "3c065c560426eb07"   SPEC_REAL_TAKEN "2026-09-19"
```

The baseline in the shared brief reproduces exactly: **vendored 6089, real 8106,
2134 free, 2074 net of the 60-token FFI floor** (`FFI_FLOOR`,
`tests/harness/suite_spec.hero:256`). I rebuilt from the seed first, hit no
`STALE`, and the trunk's pin is current.

Work was done in a lean copy at `scratchpad/warden/`; the trunk's
`spec/heroes-spec.md` is byte-identical at `7e357934…` and `git status` shows only
this sitting's two untracked brief/report directories.

## 1. Five wordings, each priced alone, each on the reader's tokeniser

Baseline vendored **6089**, real **8106**. Every draft is one edit, applied to
`spec/heroes-spec.md`, refreshed, reverted.

| draft | what it is | vendored | Δ | **real** | **Δ real** | ratio |
|---|---|---|---|---|---|---|
| **D** | minimal **merge** into § 13's *field* list — `…never a `[T]`, which is also the one array an `extern`'s parameter may be, at the header's own extent` | 6113 | +24 | 8134 | **+28** | 1.17 |
| **A** | **merge** into § 13's `f.ptr()` sentence — `…while a parameter the header spells as an array, `s: i8[8]`, takes that field itself and the compiler checks the extent` | 6120 | +31 | 8147 | **+41** | 1.32 |
| **C** | standalone clause, checking rule, **no** platform caveat | 6133 | +44 | 8161 | **+55** | 1.25 |
| **B** | standalone clause, checking rule, **with** the platform caveat | 6151 | +62 | 8181 | **+75** | 1.21 |
| **E** | standalone clause, **truthful about the C boundary**, with the caveat | 6155 | +66 | 8186 | **+80** | 1.21 |

Three things the table settles.

- **Merging beats appending again** (panel 122, `.claude/rules/spec-shape.md`):
  D +28 against C +55 for the same rule, a **27-token** gap on the binding
  instrument.
- **The platform caveat costs +20 real** (B − C). The brief asked which I would
  take. **Take the caveat.** `L_tmpnam` is **1024** on this Mac and **20** on
  glibc — I recompiled and ran it rather than reading it — so a clause without
  the warning invites a portable-looking declaration that is wrong by a factor of
  fifty on one of two platforms. Twenty tokens against a silent 51-byte overread
  is not a close call under CLAUDE.md § Precedence rank 3.
- **Honesty is nearly free: E − B = +5 real.** § 3 is why that matters.

**A vendored delta is not a price, measured again.** Every real delta exceeds its
vendored one, by 17% to 32%. A sitting ranking these offline would have read D as
+24 and E as +66; the true gap is +28 against +80.

**And draft B or E would be the first amendment since panel 134 to break
`DELTA_GATE`.** That gate is **50 vendored tokens** per commit
(`tests/harness/suite_spec.hero:181`). I computed every delta in the ledger's 83
rows: since the gate landed the widest row is panel 164's **+48**. B is +62 and E
is **+66**.

## 2. No budget veto, stated plainly

Worst case is E at **8186 real**, plus the 60-token FFI floor = **8246** against
the **10240** ceiling I grepped out of design.md:255 at the start of this sitting
rather than taking from my brief. **1994 free.** No draft comes near the ceiling
and **I do not hold a budget veto here.** The veto below rests on Principle 0,
which my mandate names as the second ground.

## 3. Panel 164's sentence for route 6 is false, and it is the sentence the route was queued on

> *"It is the only route of the seven where the compiler CHECKS the extent
> instead of trusting the author or the callee."*
> — `docs/panel/164-…:176`

The second half does not hold, and this is a measurement, not a reading. C
adjusts `T a[N]` to `T *a` (C11 §6.7.6.3p7), so **the header's extent is erased
before any checker can see it**:

```
$ clang -Wall -Wextra -Weverything -c mismatch.c      # 8-byte field -> `const char s[16]` prototype
exit 0, and the only warning is -Wpoison-system-directories
```

And §4.19's own mechanism — the `_Static_assert` / `_Generic` pair the whole FFI
checking story rests on — cannot reach it either. Both assertions below were run;
the first passes and the second fails, which is the proof:

```c
typedef __typeof__(arr_len) fn_t;              /* header says (const char s[16]) */
_Static_assert(_Generic((fn_t *)0, long (*)(const char *): 1,          default: 0), …); /* PASSES */
_Static_assert(_Generic((fn_t *)0, long (*)(const char (*)[16]): 1,    default: 0), …); /* FAILS  */
```

So route 6 checks the **author's declaration against the author's own field**. The
header's number stays an author claim that clang cannot test — which is exactly
the class panel 164's own critic named when it refused route 5's `terminated`.
Route 6 does not convert a trust into a check. It **moves** a trust from the call
site to the declaration.

**This is why drafts A, B, C and D are not merely expensive, they are wrong.**
Each says the compiler *checks the extent* or *refuses one that disagrees*, and a
reader of the one document they are told to trust will conclude the header was
consulted. Design.md Part 6 already refuses user-defined annotations with the
reason that applies here word for word: *a tag nobody reads is a comment that
looks like a guarantee, which is the one thing an FFI must never carry.* **If
route 6 ever lands, draft E is the only admissible wording of the five**, and it
costs +5 real over the misleading one.

## 4. Principle 0, both branches, measured

**Compiler need — no.** The compiler's entire foreign surface is four groups and
seventeen members:

```
$ grep -rn '^extern ' --include='*.hero' selfhost/
selfhost/cli/process.hero:37 "hero_os.h"   :49 "stdlib.h"
selfhost/cli/io.hero:34      "hero_os.h"
selfhost/emit/literal.hero:41 "stdlib.h"
```

Every parameter among them is `str`, `cstr`, `i64`, `f64` or `@i64`. **Not one
fixed array, not one array-spelled parameter.** Route 6 is not on the closure
list.

**Serves the thesis — unproven, and the sitting's own test has now answered
against it.** Panel 164 wrote the acceptance condition itself:

> *"Its whole value is a number nobody has measured: how many real headers spell
> the parameter as an array. **Queued, not adopted**, because adopting an
> unmeasured route is the cheap move."*

**That number has now been measured and it came back small and non-portable.**
The shared brief's census: byte-typed **and** fixed is 12 on Darwin, 31 on Linux,
and the intersection is **EMPTY**. I checked the set that actually decides this
project, which the census did not isolate — the headers this repository binds:

```
sqlite3.h   0 array-spelled parameters   (all 28 text hits are doc markup)
raylib.h    0
sys/stat.h  2 — futimens, utimensat, both `const struct timespec __times[2]`
```

Not one byte array. And across the tree, `172` extern functions and **zero**
parameters spelled as an array; the single `tmpnam` is
`tests/golden/check/owned-freer-must-be-declarable.hero:37`, declaring
`buffer: ptr` to provoke `freer_arity`. **No working binding in this tree would
change** — I reproduced the brief's claim and it holds.

**A queue resolves on its own condition.** The measurement route 6 was queued
behind has been taken, and it says the form would serve two non-byte functions in
one header this project does not bind. That is the answer to the question the
brief said was mine: **panel 164's warden objected to route 3 on Principle 0 and
was overridden on robustness, and that reasoning does not transfer.** Route 3 had
a robustness lever — before it, no program could hand C a field's address at all,
and the workaround handed C seventeen bytes where eight were meant. Robustness is
CLAUDE.md § Precedence rank 3 and beats Principle 0 at rank 4, correctly. **Route
6 has no such lever**: the crossing already works.

```
$ ./heroes run rt/r3.hero
2
```

## 5. The one finding that argues FOR route 6, reported because §12 outranks my preference

I attacked the shape next to route 3 and found a real hole. Route 3 lets the
author overstate the extent with nothing refusing it:

```
$ ./heroes check rt/r3-over.hero    # arr_len_p(p: t.name.ptr(), n: 64) on an i8[8] field
exit 0
$ ./heroes run rt/r3-over.hero
2                                   # exit 0 — C was licensed to read 64 bytes of an 8-byte field
```

Route 6 would close that link: `i8[4]` passed to `s: i8[8]` fails on ordinary
type identity. This is the strongest argument the proposal has and **no brief
contains it**.

It still does not carry adoption, for a reason that is itself a §1.6 argument.
**Route 6 does not close that hole — it parks a tighter form beside a looser one
that stays legal.** `.ptr(), n: 64` compiles the day after route 6 lands, and §13
would then offer two spellings of one crossing in one paragraph, which is §1.6's
*three ways to write a loop* with a smaller number. **If the sitting cares about
the overstated extent — and it should — the repair is to tighten route 3's own
check, not to add a second form.** That is a route no sitting has listed
(CL-057). What would have to be true for it to exist: the compiler must be able
to identify which argument is the extent. It cannot in general; it plainly can
when the lent field's type and the sibling argument are both literals in one
call. **That is a checker narrowing at or near zero spec tokens, against route
6's +80**, and it is the option I would have the panel price before this one.

## 6. What comes out — nothing, and that is a problem

§1.6's payment is unconditional and I can name no removal. Panel 164 already
spent the one the previous warden had banked (the `ptr`/`cstr` merge, −6 real). A
prediction is the other route and `heroes mutate` exists, so it is admissible in
principle — but the prediction route 6 needs is *how many wrong-extent programs
this catches*, and the shared brief §6 says in its own words that **nobody has
measured what route 6 would catch**. A sitting cannot pay with the same
unmeasured number twice: it was the reason for the queue in 164 and it would be
the payment in 165.

## 7. Two corrections to the briefs

- **My brief sent me to merge into "the existing sentence that already lists what
  a parameter may be." No such sentence exists.** I grepped all 23 occurrences of
  *parameter* in `spec/heroes-spec.md`, on 21 lines; § 13 enumerates what a **field** may be
  and never what a **parameter** may be. That list lives only in the diagnostic
  and in design.md §4.19. So the cheap merge sites are sentences about something
  else: D lands a parameter rule inside the record paragraph, against
  `.claude/rules/spec-shape.md`'s *every rule has one home, the section of the
  operation it governs*; A lands the second way to do a thing inside the sentence
  that states the first. **The honest home for route 6 is a standalone clause,
  which is E at +80** — the merge discount is not available to this rule.
- **Route 6 owes no grammar change, which no brief says.** `spec:71` already
  reads `Type = Prefix { "[" integer "]" } [ "?" ] .`, and `CParam` takes `Type`,
  so `s: i8[8]` is already derivable. The productions cost **0**. This is in the
  proposal's favour and is reported as such.

## 8. The ledger row

Under this verdict **no row is written**, because the document does not move.
Recorded here so a later sitting need not re-measure: were route 6 adopted in
its **only admissible wording (E)**, the row would read —

> | 6155 | panel 165, 2026-09-19: the extent declared where C spells it | **a parameter the header spells as an array is written that way** — § 13's lend-and-lease paragraph gains a standalone clause after `f.ptr()`. **+66** vendored and **+80** on the reader's instrument (6089 to **6155**; 8106 to **8186** on `claude-opus-5`; digest `10ff609e21fb4d8e`). **Ratio 1.21**, under this document's 1.33. **The first row since panel 134 to break `DELTA_GATE`**, 66 against a 50-token window, so the commit body owes the attribution. **The wording is the truthful one and four cheaper ones were refused as false**: C erases the extent (C11 §6.7.6.3p7), `-Weverything` is silent on an 8-byte field against a `char[16]` prototype, and `_Generic` proves the prototype's parameter is `const char *` and not `const char (*)[16]` — so *the compiler checks the extent* would promise a check the boundary cannot make, at a saving of 5 real tokens. **What paid:** *(unpaid — no removal was named and the prediction available names a number nobody has measured)*. **2054 free**, and 1994 net of the FFI floor. |

## 9. Prediction — falsifiable, with an instrument that exists today

**Registered, scored at the close of the milestone that would land route 6, by
`grep` over `selfhost/` and `examples/` plus the census script already in
`scratchpad/extents/census.py`:**

> **If route 6 is adopted, the count of `extern` parameters spelled as a fixed
> array in this repository will be 0 at that milestone's close, and ≤ 2 at the
> close of the one after.** Today it is 0 of 172.

It is falsified by three or more real bindings adopting the form. If it holds,
route 6 will have cost +80 real tokens to serve nothing that compiles here, and
the ledger row above is the receipt.

## 10. What would change my verdict

Any **one** of these, and I withdraw the veto and approve **draft E** — never
A, B, C or D, which stay refused as false whatever the measurement says:

1. **A measured catch rate.** Run `heroes mutate`'s extent operator, or hand N
   models the § 13 text and count wrong-extent bindings, and show route 6 refuses
   a class route 3 accepts at a rate that beats one correction round-trip
   (500–2000 tokens) against its 80.
2. **A binding this project needs.** One entry on §1.0's closure list, or one
   §4.19 ladder binding in `examples/`, that cannot be written with `f.ptr()`.
   Today the ladder's own headers offer zero byte-typed array parameters.
3. **The Windows column.** It is unrun, the brief says so, and a form justified
   by how headers spell things cannot be decided on two platforms of four. If
   Windows' SDK spells byte parameters as arrays at a materially higher rate,
   that is a new fact and I will re-price on it.
4. **The cheaper route in § 5 priced and refused.** If tightening route 3's own
   extent check is measured and found impossible or dearer than +80 real, route
   6 becomes the best remaining answer to a hole I measured myself, and I would
   rather have it than the hole.
