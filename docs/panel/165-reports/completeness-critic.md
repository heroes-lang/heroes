# Panel 165 — completeness critic

**No verdict.** This seat names what is missing. Five seats differentiated by
input still share one blind spot, and this sitting's is nameable in one sentence:
**every seat priced where the extent is WRITTEN, and nothing in the language ties
the number C is given to the field it is given about.** Both defects found during
the sitting are that hole, a third one I found is that hole, and route 6 does not
close it.

Everything below was run on 2026-09-19 in a `cp -r` of the tree at
`…/scratchpad/tree`, `archive/` removed, seed built with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (exit 0). The real
working tree is unchanged: `git status --short` shows only this sitting's two
untracked panel directories. Every command is given. Where I could not run
something I say *unrun* in that word.

---

## 1. The two live defects: both verified, both WORSE than filed

### (A) route 3's overstated extent — filed as an overread, it is also a WRITE

Filed as: `sum_n(p: t.name.ptr(), n: 4096)` on an `i8[8]` field reads 4096 bytes,
exit 0, ASan stack-buffer-overflow. **Verified exactly**, and the wrong answer is
visible without a sanitizer:

```
$ ./heroes check  crit/sumover.hero                       # n: 4096 over an i8[8] field
check exit=0
$ ./heroes run    crit/sumover.hero --include crit
222795                                                    # correct answer at n: 8 is 177
$ ./heroes run    crit/sumover.hero --include crit --sanitize
==14680==ERROR: AddressSanitizer: stack-buffer-overflow … READ of size 1
    [32, 44) 'h0_t' <== Memory access at offset 44 overflows this variable
```

**The shape beside it is the one that matters, and no seat ran it.** The same
overstatement in the WRITE direction — the direction panel 164 adopted — corrupts
the neighbouring field:

```
$ cat crit/wover.hero        # fill_n(p: ptr, n: i64) writes 0x41; t: Slot @ slot_make()
    print(to_str(t.id))                      -> 7
    fill_n(p: t.name.ptr(), n: 64)           # 64 bytes into an i8[8] field
    print(to_str(t.id))                      -> 1094795585      # 0x41414141
$ ./heroes check crit/wover.hero ; echo $?
0
$ ./heroes run   crit/wover.hero --include crit ; echo $?
0
$ ./heroes run   crit/wover.hero --include crit --sanitize
==14821==ERROR: AddressSanitizer: stack-buffer-overflow … WRITE of size 1
```

Defect A is therefore **both clauses of design.md §1.12**, not one: *must not
segfault* AND *must not corrupt memory*. It should be written down as the write,
because the write is what §1.12's second clause names and the write is the half
panel 164 shipped. The spec-warden (§5) and the ffi-pragmatist (§8) each found
the read half independently; neither carried it to the write.

### (B) `t.name == u.name` — verified, and the class is wider than filed

```
$ ./heroes check crit/eq.hero ; echo $?
0
$ ./heroes build crit/eq.hero --include crit -o /tmp/o ; echo $?
internal error: compiling the generated C failed:
crit/eq.hero:9:11: error: use of undeclared identifier 't24'
2
```

Filed narrow class confirmed — the whole record, a scalar field and an indexed
element all build at exit 0. **Two additions the filing does not carry:**

| shape | check | build | |
|---|---|---|---|
| `t.name == u.name` | 0 | 2 | **internal error** |
| `t.name != u.name` | 0 | 2 | **internal error**, same site |
| `if t.name == u.name` | 0 | 2 | **internal error** — not only in `print(to_str(…))` |
| `b = t.name` | 0 | 1 | clean `unsupported[fixed_flow]` — the `.store` arm IS guarded |
| `[t.name, u.name]` | 0 | 1 | clean `unsupported[]` |
| `t.name.validated_bytes() == …` | 0 | 0 | fine |

So the defect is **every `.binary` on two fixed fields**, and the correct
contrast is that the neighbouring arms produce a clean `unsupported[]` refusal
while this one blames the compiler for a `.hero` file. The compiler-engineer
located it correctly at `selfhost/emit/gate.hero:380`, which I read:

```
.lit | .load | .push_owned | .unary | .binary | .cast | .call   => return
```

**And here is the fact that decides this defect's relevance to route 6.** I tried
to reach the sibling `.call` arm and could not:

```
$ ./heroes check crit/callf.hero        # function take(s: i8[8]) -> i64, called with t.name
error[fixed_outside_a_group]: `i8[8]` is a C array member, so it belongs to a
  `record` inside an `extern` group … at crit/callf.hero:6:18
```

The `.call` arm is unguarded and currently unreachable, held shut **only** by
`fixed_outside_a_group` on ordinary functions and `ffi_type` on extern ones.
**Route 6's entire mechanism is to remove exactly those two refusals.** It does
not merely "open the sibling arm on purpose" (the compiler-engineer's phrase) —
it removes the only two things standing between a user program and an unguarded
emitter arm, and defect B is the measured proof of what that arm does when it is
reached.

---

## 2. A THIRD defect no seat found, and it is larger than both

**C writes into a binding Heroes calls immutable, with no `@` at the
declaration, no `@` on the parameter, and no `@` at the call site.**

```
$ cat crit/imm.hero
    function fill_n(p: ptr, n: i64)      # plain ptr, NOT @-marked
    …
    t = slot_make()                      # immutable binding, no @
    print(to_str(t.name[0].to_i64().must()))   -> 72
    fill_n(p: t.name.ptr(), n: 8)
    print(to_str(t.name[0].to_i64().must()))   -> 65

$ ./heroes check crit/imm.hero ; echo $?
0
$ ./heroes run crit/imm.hero --include crit ; echo $?
0
```

72 became 65. spec § 5: *"only a declared `@` name can be mutated."* spec § 3:
*"Every value behaves as an independent copy… No aliasing exists among the values
this language owns."* Both are false at that line, silently.

**This is not my hypothetical — it is the shipped golden.**
`tests/golden/run/ffi-a-byte-field-crosses-to-c.hero:54` is
`slot_fill(p: s.nsap.ptr(), n: 8)` against `function slot_fill(p: ptr, n: i64)`:
no `@` on the parameter, none at the call site, and the case's own comment calls
the write-back "the whole point of the case".

**And panel 164's resolution point 2 was never implemented.** It reads: *"`@field`
reaches a `@`-marked `ptr` parameter so C can fill a field."* Run, all three
spellings, against `function fill_n(@p: ptr, n: i64)`:

```
p: @t.name           error[type_mismatch]: expected `ptr`, found `i8[8]`
p: @t.name.ptr()     error[not_a_place]: only a name, a field or an element can be passed as …
p: t.name.ptr()      error[marker_mismatch]: `fill_n` changes this argument, so the call site …
```

The first is the **identical** error panel 164 quoted as evidence that *"only the
type rule is missing"*. It is still missing. There is no golden anywhere that
lends a field to a `@`-marked `ptr` parameter — I grepped `tests/golden/` and
`examples/`; every `@…: ptr` hit is a handle out-parameter (`@db: ptr`,
`@stmt: ptr`). So the enforcement machinery works (`marker_mismatch` fires
correctly when the author marks the parameter), and **nothing obliges a binding
author to mark a parameter C writes through.** For a handle that is harmless —
the pointee is C's memory. For a field lend it silently falsifies § 3 and § 5.

Two spec sentences are in conflict and the compiler implements one of them: § 13's
*"C may write back through it"* against § 5's *"only a declared `@` name can be
mutated"*. Under CLAUDE.md § 12 that is a **spec defect**, not a compiler defect,
and it is where the llm-ergonomist's veto ground actually lives — in the shipped
route 3, not in route 6.

---

## 3. Routes nobody listed

The parent asked specifically. Listed across both sittings: 1 decay, 2 `cstr`
lend, 3 `ptr` lend, 4 refuse, 5 `terminated`, 6 parameter extent, 7 write
direction; plus, in the reports, 8 tighten route 3's own extent check
(spec-warden §5, and the ffi-pragmatist's `len`-as-constant in §8 is a variant),
9 widen `f.ptr()` to any fixed element type (ffi-pragmatist §8), 11 read
`__counted_by` from the header (named unlisted by the shared brief §6; the
historian surveyed it and the ffi-pragmatist measured it away —
`_LIBC_COUNT(L_tmpnam)` preprocesses to nothing).

**Two more, neither listed by any seat, both with their premises measured.**

### Route 12 — a group `constant` as the EXTENT, checked by clang against the header

Today it is refused at the lexer:

```
$ ./heroes check crit/konst.hero          # name: i8[SLOT_NAME_LEN]
error[expected_array_length]: expected the array's length after `[`, found a
  name (`SLOT_NAME_LEN`) … at crit/konst.hero:4:18
```

Every seat treated that as a wall. **It is not, and the existence proof is one
clang run.** §4.19's own field-checking mechanism — the `_Static_assert`/`_Generic`
pair the emitter already writes — accepts a macro-named extent, because the macro
expands in the emitted C against the real header:

```c
/* compiled against the real konst.h */
_Static_assert(_Generic(&((struct slot2 *)0)->name,
    int8_t (*)[SLOT_NAME_LEN]: 1, default: 0), "named extent matches");   /* PASSES */
_Static_assert(_Generic(&((struct slot2 *)0)->name,
    int8_t (*)[9]: 1, default: 0), "a WRONG named extent must fail");     /* FAILS  */
```

```
$ clang -std=gnu11 -I crit -c /tmp/kc.c -o /tmp/kc.o
/tmp/kc.c:4:16: error: static assertion failed: a WRONG named extent must fail
1 error generated.
```

So Heroes **can** state an extent it does not know at check time and still have it
checked per platform against the real header. That dissolves, with a command, the
tension the historian made the centre of Thread A — *"Route 6 checks an extent at
check time. Family 2 knows the portable number only at C-compile time. **No
language in this survey does both at once.**"* Heroes' check is emitted C compiled
against the real header, which is a mechanism neither Ada (checks a number its own
compiler knows) nor Nim (knows the number, checks nothing) has. The historian
made "a precedent that checks an extent it does not know at check time" its
**condition 1** for adopting named extents; the answer is that this project does
not need the precedent, because it already owns the instrument.

The llm-ergonomist reached the same wall from the other side and called it *"a
real ergonomic reversal I did not expect"*, then filed it as *"a question for the
document rather than for this proposal"*. It is the proposal's question: it is the
difference between route 6 being non-portable **by construction** and portable.

### Route 13 — declare which argument is the extent, and derive it from the field

Nobody proposed that Heroes WRITE what `__counted_by` says, rather than read it:

```
function sum_n(p: ptr sized_by n, n: i64)        # spelling illustrative only
sum_n(p: t.name.ptr())                           # n is derived from the field's extent
```

Both premises are measured, by other seats and by me:

- the field's extent is known to the checker by ordinary type identity
  (compiler-engineer: `error[type_mismatch]: expected i8[4], found i8[8]`);
- the field's extent **is checked against the real header by clang**
  (`error[ffi_field_type]: Slot2.name is not i8[20] in konst.h`, re-run by me).

So the derived number is a **header-checked** number — precisely the property
route 6 provably cannot have, and the property the whole sitting concluded was
unobtainable. It closes defect A in both directions, works for every element type,
and needs no new boundary type. **Its cost is unpriced and I did not price it**;
that belongs to the compiler-engineer and the ffi-pragmatist. I list it because
CL-057 makes the option set itself a measurement, and this option was not in it.

Route 12 and route 13 compose: 12 makes the field's extent portable, 13 makes the
call's number the field's extent. Together they are what route 6 claims to be.

---

## 4. Claims asserted but not measured, including the coordinator's

**(a) "172 `extern` functions are declared in this repository" — the command does
not make that claim, and the number is wrong twice.** It is in the shared brief
§5, re-asserted by the spec-warden (§4) and by the compiler-engineer, who wrote
*"reproduces on my ruler"*.

```
$ grep -rhn '^    function [a-zA-Z_]' --include='*.hero' . | sed -E '…' | sort -u | wc -l
172                                     # the brief's command, on the real tree
$ … --exclude-dir=archive …
167                                     # the same command without archive/
$ comm -13 …                            # the five names only archive/ has
hero_args_at  hero_args_count  hero_exit  hero_file_read  hero_file_write
```

Five of the 172 come from **`archive/bootstrap-rs/`** — the tree this sitting's own
working rules say never to touch and which nothing builds. And `sort -u` counts
unique **names**, not declarations; the pattern `^    function` has no notion of
`extern` at all. Counted properly, by tracking `extern` blocks:

```
$ find . -name '*.hero' -not -path './archive/*' -print0 | xargs -0 awk '…'
extern function declarations: 316
array-spelled extern parameters: 0
```

**The conclusion survives and gets stronger — 0 of 316, not 1 of 172** — but three
seats built a Principle 0 argument on a denominator none of them checked, and the
one that matters (the numerator) is 0, not the 1 the brief reports. The brief's
`tmpnam` hit declares `buffer: ptr`, so it was never a numerator.

**(b) The llm-ergonomist was fed a fact about the specification that is not in
it.** That seat's contract is *"your only input is `spec/heroes-spec.md`"*. Its
brief opens: *"Today `spec § 13` says a C function's parameter may be a number,
`bool`, `str`, `ptr`, `cstr`, a function type, or a `record` declared in the same
group."* The spec-warden measured that no such sentence exists — 23 occurrences of
*parameter* on 21 lines, none of them that list — and `grep -c parameter
spec/heroes-spec.md` returns **21** on my run, confirming the warden's grep. That
list lives in the **diagnostic text** and in design.md §4.19. So the one seat
whose input is controlled had its input contaminated with a design-document fact,
and its Task 1 and Task 3 answers were written against it. This does not overturn
its verdict — its objection rests on § 3, § 5 and § 9, which it quoted correctly —
but it is exactly the failure the seat exists to prevent, and it should be
recorded.

**(c) The historian's "working precedent" has the property the other seats call
fatal.** The report approves partly because *"Ada has shipped it for decades…
length-checked on entry, `Constraint_Error` when it disagrees."* Its own table
cell says what is checked: *"a view conversion to the formal's nominal subtype."*
`pragma Import (C, …)` takes no header; Ada has no access to the C declaration at
all. **So Ada is the author-against-author check** — the same thing the
compiler-engineer calls *"calling the agreement of two author claims a
verification"*, the spec-warden calls *"moving a trust from the call site to the
declaration"*, and the ffi-pragmatist proves impossible to do otherwise. The
precedent is real; it supports a strictly weaker claim than the one route 6 was
queued on, and the historian's `approve` does not say so.

**(d) The coordinator's §3 is right about parameters and misses that it is wrong
about fields.** It says recovering an extent needs *"a second kind of instrument —
a source-range reader for headers… a new class of thing at the boundary"*, and
prices that as the sitting's missing measurement. For a **parameter** that is
correct and three seats proved it. For a **field** the existing instrument already
does it with a named extent (§3 above, run). The coordinator corrected its own
first draft once in that file; this is the second correction the same paragraph
needs.

**(e) `spec § 13` promises a crossing the compiler refuses, and no ruling narrows
it.** The ffi-pragmatist filed this as *unrun* in §6 because it did not grep. I
did. § 13 says a field may be *"a fixed array of one: `i32[4]`"* and that
*"`f.ptr()` lends a binding's field to a `ptr` parameter"* — no narrowing to
bytes. The compiler refuses:

```
$ ./heroes check crit/fds.hero          # pipe(fildes: f.fd.ptr()) on a i32[2] field
error[bad_operand]: `ptr` takes a fixed run of bytes — `i8[N]` or `u8[N]`, found `i32[2]`
$ ./heroes run crit/fds2.hero --include crit
2                                       # the i32[2] FIELD itself is fine
```

I read `selfhost/check/lend_types.hero:76-96` in full: the comment block there
explains at length why the lend answers `ptr` and never `cstr` (panel 164's
resolution) and says **nothing** about the element type. I grepped
`docs/records/log/` and `docs/panel/164-*.md` for a deliberate narrowing and found
only the `cstr` reasoning and a note that `validated_bytes` takes both signs.
**That is a negative resting on my search vocabulary** — I looked for `ptr()`,
`i8[N]`, `u8[N]`, `bytes only`, `fixed run of bytes` — but on what I ran, CLAUDE.md
§ 12 applies: spec beats compiler, and this is a defect rather than a design.

---

## 5. The contradictions, and how each is settled

| # | the disagreement | checkable? | the command that settles it |
|---|---|---|---|
| 1 | **historian `approve` vs three seats' *the check is worthless*.** Historian: Ada is a working precedent for a checked extent. Others: the check compares two author claims. | **Yes, already settled** | Read the historian's own cited cell and Ada RM B.3(70)/4.6(37): the check is a view conversion to the **Ada** formal's subtype. Ada never sees the C header. Both are right; the historian's precedent supports the weaker claim, and its report should say so. |
| 2 | **spec-warden: "route 6 owes no grammar change, productions cost 0"** — asserted from the spec's text. | **Yes — and it holds** | `./heroes grammar` prints `CParam = [ "@" ] ident ":" Type [ "owned" ident ]` and `Type = Prefix { "[" integer "]" } …`. Run by me; the warden read it, the instrument confirms it. Note `.claude/rules/verification.md` puts `grammar` on the `spec/heroes-spec.md` row, so this is the suite that would have judged it. |
| 3 | **ffi-pragmatist §5 ("it writes back through the lend", plain `.ptr()`, unmarked `ptr` parameter) vs panel 164's resolution 2 ("`@field` reaches a `@`-marked `ptr` parameter").** | **Yes — settled, and the pragmatist is right about the compiler** | The three runs in § 2 above. `@field` → `@ptr` does not compile; the unmarked write does. **Panel 164's resolution 2 is unimplemented and its own golden demonstrates the unmarked form.** |
| 4 | **compiler-engineer `veto` vs ffi-pragmatist `object` on the same §1.12 ground.** | **Yes** | Not a disagreement about the world — the pragmatist's §8 offers a substitute (widen `f.ptr()`) and the engineer's condition 1 offers a repair (a header source-range reader). Settle it by pricing route 12/13 above, which is the substitute neither listed. |
| 5 | **"you are pricing a difference, not a capability" (shared brief §2) vs ffi-pragmatist §6 (`pipe` does not cross at all).** | **Yes — settled, the brief is wrong** | `./heroes check crit/fds.hero` → `bad_operand … found i32[2]`. Reproduced. Of the eight both-platform fixed functions the brief itself names, **none is a byte array**, so route 6 as spelled binds zero of them. The brief's framing sent five seats to price the wrong thing. |
| 6 | **llm-ergonomist: "this would be the FIRST caller-visible mutation not marked `@` at both sites."** | **Yes — settled, it is not the first** | `crit/imm.hero` and `tests/golden/run/ffi-a-byte-field-crosses-to-c.hero:54`. The shipped language already has one. This **strengthens** the ergonomist's objection rather than weakening it: the condition it makes for approving route 6 is a repair route 3 needs today. |
| 7 | **spec-warden §5 vs ffi-pragmatist §8 — two different unlisted repairs for the same hole**, neither seat aware of the other's. | **Yes** | Warden: tighten `.ptr()`'s sibling-literal check. Pragmatist: make `len` on a fixed field a compile-time constant. Both are strictly weaker than route 13 above, which derives the number instead of checking or exposing it. Price all three together or none. |

---

## 6. The question the sitting should have asked and did not

**It asked where the extent should be WRITTEN. The measurable hole is what makes
the extent TRUE.**

The census — how many real headers spell a parameter as an array — answers the
first question and is irrelevant to the second. That is why the measurement route
6 was queued behind came back, in the spec-warden's words, *"small and
non-portable"* and settled nothing: it was never the number that decides. The
shared brief §6 says so itself — *"Nobody has measured what route 6 would CATCH…
it is the number this sitting most lacks"* — and then the sitting ran the other
one anyway and five seats priced it.

Asked properly, the sitting's question has an answer the reports already contain
in pieces: **three measured memory-safety holes** (A's read, A's write, § 2's
unmarked mutation), **all three in the SHIPPED route**, all three about a number
nobody binds to a field, and **none of them closed by route 6.**

A second question that went unasked: **is the fixed-field mechanism portable at
all?** Three measured facts compose into a wall nobody named:

- an extent must be a literal (`expected_array_length`, re-run);
- a literal that disagrees with the header is refused at **build**
  (`ffi_field_type`, re-run);
- real header extents are platform-varying macros — the brief measured
  `L_tmpnam` at 1024 here and 20 on glibc, both run.

I measured the third on the emblem panel 164 chose for itself: Darwin's
`sys/utsname.h:72` is `#define _SYS_NAMELEN 256` and `sizeof(u.sysname)` compiled
and run on this Mac is **256**. So `record Utsname` must be written `i8[256]` here.
glibc's `_UTSNAME_LENGTH` is **unrun by me** — I have no Linux box in this session
— but the refusal mechanism is measured and platform-independent by construction,
so on any platform whose macro differs the build is refused. Panel 164's own
headline program is platform-locked, and the sitting priced route 6's portability
**caveat** at +20 spec tokens without noticing that the shipped route has the same
problem in a worse form: a build refusal rather than a silent wrong number.
**Route 12 closes it.**

---

## 7. Do the defects change the resolution?

**Defect B: yes, decisively, and against route 6.** It is the measured behaviour
of `check_fixed_flow`'s unguarded `.binary` arm. The sibling `.call` arm is held
shut today **only** by the two refusals route 6 exists to remove — I ran that and
it is `fixed_outside_a_group`, not a design decision. So route 6's first act is to
open, on purpose, the arm whose twin already produces `internal error: use of
undeclared identifier` at `heroes check` exit 0. That is not a cost to be priced;
it is a prerequisite repair, and it must land before route 6 can be evaluated at
all. **B is a defect of the compiler and belongs in `docs/work/DEFECTS.md` today**
(`**OPEN: 0**` is currently false), with a fourth row in
`tests/golden/unsupported/fixed-array-flow.hero`.

**Defect A: yes, and it moves the milestone rather than route 6.** A is the only
argument route 6 has — the spec-warden found it and said so: *"This is the
strongest argument the proposal has and no brief contains it."* But the warden
also named why it does not carry adoption, and I verified the reason: route 6
**parks a tighter form beside a looser one that stays legal**. `n: 4096` compiles
the day after route 6 lands. And A is worse than either seat measured — it is a
WRITE overflow that silently corrupted `t.id` from 7 to 0x41414141 at exit 0 — so
it is §1.12's *must not corrupt memory*, which CLAUDE.md § Precedence puts at rank
3, above compiler size, ergonomics and token cost. **A is the milestone's real
work and route 6 is not its repair.**

**The third defect (§ 2): yes, and it is the one that should reopen a decision.**
C mutating a Heroes value with no `@` anywhere falsifies two spec sentences, is
demonstrated as correct behaviour by a shipped golden, and means panel 164's
resolution point 2 was ratified by delegation and never implemented. It is both a
spec defect (§ 3/§ 5 against § 13) and an unfinished adoption. Under CLAUDE.md
§ 14 and § Verification's clean-list rule, a milestone cannot be tagged over it.

**What I would have the sitting weigh, in one line.** Route 6 moves a number;
routes 12 and 13 bind it to a field whose extent clang checks against the real
header. Three seats concluded the boundary cannot check an extent — that is true
of a **parameter** and false of a **field**, and the difference is one clang run
that nobody made. Whatever the panel decides about route 6, the option set it
decided from was incomplete, and CL-057 makes that a measurement error rather than
a matter of taste.

## What I could not run, stated as such

- **Windows and Linux.** Every run here is `arm64-apple-darwin`. The shared brief
  already flags Windows as absent; I add that `_UTSNAME_LENGTH` on glibc is unrun
  by me and is cited, not measured.
- **The cost of routes 12 and 13.** Their premises are measured; their line counts
  are not, and pricing them is the compiler-engineer's instrument, not mine.
- **Whether `.ptr()`'s narrowing to bytes was a deliberate ruling.** I read
  `lend_types.hero:76-96` and grepped `docs/records/log/` and `docs/panel/164-*`
  for five spellings. Finding nothing rests on my vocabulary.
- **No timing.** I ran no `/usr/bin/time -p` clock, so this report makes no
  duration claim.
