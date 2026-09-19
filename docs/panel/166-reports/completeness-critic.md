# Panel 166 — completeness critic

**This seat gives no verdict.** It names what is missing, settles the seats'
checkable disagreements, and says what the resolution must account for that no
seat raised.

Every number below was run on 2026-09-19 in a scratchpad copy of the tree at
`3702e3d2`, with `archive/` deleted and the compiler built there from the seed —
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 4.98 user
4.88`. Apple clang 21.0.0, arm64-apple-darwin25.6.0. Where a claim could not be
run it says *unrun* in those words.

---

## 0. The headline, before the detail

**Three things this sitting does not have.**

1. **`f.ptr()` has a THIRD hole and no document in the sitting names it: the
   lend has no LIFETIME rule.** C may keep the address past the frame. Measured,
   nine lines, exit 0, no diagnostic: `AddressSanitizer: stack-use-after-scope`.
   The brief's own question is *what should a `ptr` lend PROMISE*, and all seven
   routes answer only about extent and mutability.
2. **A route nobody listed closes 065 with a mechanism that already ships and no
   new mark anywhere: emit the lend as `const void *` unless the write is
   asked for.** Proven end-to-end on the compiler's own emitted C under the
   project's own flag set. The compiler rejected this in a comment
   (`selfhost/emit/field_lend.hero:17-22`) on a false dichotomy — always-const
   or never-const — and nobody re-opened it.
3. **The denominator every seat argued over measures the wrong population.**
   Route C is a rule about the lend's argument position. That population in this
   repository is **four call sites in one file**, not 66, not 18, not 46, not 12.

---

## 1. The four named contradictions, settled

### 1.1 Route B: the compiler-engineer's veto is CORRECT, and it is wider than it states — but it does not reach what the other two seats are arguing for

**The fact, re-run.** `function give(@p: ptr, n: i64)` with `give(p: @q, n: 8)`:

```
$ ./heroes build b_at.hero --emit-c -o b_at.c
b_at.c:71  static void hero_ffi_probe_h_bat_give(void * * a0, int64_t a1) { (void)(give)((void *)a0, a1); }
b_at.c:117     (void)give((void *)&h0_q, t2);
$ ./heroes run b_at.hero     # against  static inline void give(void **pp, int64_t n)
ok, exit 0
```

`@p: ptr` today is a `void **` out-parameter, and the probe casts it to
`(void *)` before calling, so clang cannot see the confusion. **The engineer is
right on the fact and right that resolution 2 would give one spelling two
meanings.**

**And the veto is wider than the engineer stated, which no seat measured.** `@`
is occupied at the **call site** too, and the compiler says so in its own words:

```
$ ./heroes check atcall.hero        # sl_fill(p: @s.name.ptr(), n: 8)
error[not_a_place]: only a name, a field or an element can be passed as `@` —
  the callee copies its result back into the argument, so `@` must name where
  the value goes
```

*The callee copies its result back into the argument.* That is a third meaning
for `@`, at the site the llm-ergonomist recommends (`@b.bytes.ptr()`) and the
historian's Swift/Hylo precedent points at. So the ergonomist's B and the
historian's B collide with an occupied spelling at **both** ends, not one.

**How each seat is right.** The ergonomist's ground is locality — *the call line
must say C writes* — and the historian's is precedent — *Swift, Hylo, Ada and
Oberon all mark the mutation where a reader sees it*. Neither ground requires
the mark to be spelled `@`; both were stated as `@` because `@` is the mark this
language has. **The disagreement is not about whether to mark the write. It is
about the SPELLING, and the spelling is taken twice over.** Nobody separated
those two questions, and separating them dissolves the deadlock: the engineer's
own veto-lifting condition is *"the write direction gets a spelling of its own —
a distinct mark on the parameter, not `@` overloaded"*, and a distinct mark on
the **lend** satisfies the engineer, the ergonomist and the historian at once.
See § 3.2.

**The command that settles it** is the one above, and it is already run:
`./heroes build <prog> --emit-c` on a `@p: ptr` extern, plus `./heroes check` on
a call-site `@` over a lend. Both are in this file.

### 1.2 `__attribute__((access))`: both seats are right on the facts and the historian's CONCLUSION is false

Re-run, all three:

```
$ printf '#if __has_attribute(access)\n#error HAS_ACCESS\n#endif\nint x;\n' | clang -x c -fsyntax-only -
(no output — __has_attribute(access) is 0)                      historian: CORRECT

$ clang -std=c11 -I. -fsyntax-only rc_lit.c
error: static assertion failed ... 'heroes-ffi-lend ... the call states 64 bytes and the field `name` is 8'
note: expression evaluates to '64 <= 8'                         pragmatist: CORRECT
```

They are not in conflict on the world. They conflict on one sentence of
inference, and it is the historian's:

> *"route C's check must be written in Heroes, and the `[static N]` /
> `_Static_assert` discipline is the only mechanism on the table a C compiler
> will enforce for free — **which makes route E load-bearing rather than
> cosmetic.**"*

**The last clause is false, and the reason is that the `_Static_assert`'s
right-hand side is C's own number.** Re-run with a *named* extent and no route E
anywhere:

```c
_Static_assert((int64_t)(SL_NAME_LEN*2) <= (int64_t)sizeof(((struct sl *)0)->name), "overstated, named");
```
```
error: static assertion failed ... note: expression evaluates to '16 <= 8'
```

The field's size never leaves C. So route E — a named extent in the Heroes
**type** — buys legibility and buys the check nothing. The ffi-pragmatist's
reading is the correct one and the historian's is not.

**And a third thing neither seat had, which changes route C's price.** The
ergonomist's Task-3 route (c) — the careful reader's route, a group `constant`
as the extent — does not reach the checker as a constant at all:

```
$ ./heroes build konst.hero --emit-c -o konst.c        # n: SL_NAME_LEN
konst.c:46   _Static_assert(__builtin_constant_p(SL_NAME_LEN), "heroes-ffi-const SL_NAME_LEN");
konst.c:119      return SL_NAME_LEN;
konst.c:142      t5 = h_konst_SL_NAME_LEN();      <-- the extent argument, lowered to a CALL
```

The extent argument becomes `h_konst_SL_NAME_LEN()`. `contextual.literal_value`
fails on it exactly as it fails on `.name`, so under the compiler-engineer's own
condition 2 — *a variable extent must be refused, not admitted* — **route C
refuses the careful reader's program and accepts the careless one's literal.**
The emitter, meanwhile, already proves the name is a C compile-time constant one
line away (`__builtin_constant_p`).

**Which exposes the real omission: route C is TWO routes wearing one letter, and
each seat priced a different one without noticing.**

| | where the number is compared | needs | covers | diagnoses | priced by |
|---|---|---|---|---|---|
| **C-check** | in Heroes, at check time | `contextual.literal_value`, a literal | literal only; refuses a named or product extent | `.hero`, exit 1 | compiler-engineer, ~210 lines |
| **C-emit** | in C, by `_Static_assert` | the extent's C **text** to reach the emitter | literal, macro name, product, `partial` record | clang text through `cli/pointee.hero` | ffi-pragmatist, **unpriced in lines** |
| **C-spec** | — | `CParam` gains `counted_by ident` | — | — | spec-warden, +51 real |

No seat priced another seat's C. The panel has three numbers for three different
proposals filed under one letter, and the resolution would adopt whichever one
the writer happened to be reading.

### 1.3 § 3 versus § 5: the spec-warden is right on both halves, and defect 065's headline is still false in the tree

Re-run independently of the warden's programs. `via_param(t: Sl)` takes the
record **by value**, fills `t.name` through a lend, and reads it back:

```
$ ./heroes run eq.hero
72        # the caller's u before
65        # t.name[0] inside the callee, after C wrote
72        # the caller's u after — unchanged
```

- **§ 3 is TRUE.** The caller's value is an independent copy and stays 72. The
  warden's structural argument holds and I checked its premise in the document:
  `spec/heroes-spec.md:367` *"A lend and a lease name stand only as an argument
  of a call"*, and the compiler enforces it twice (`field_lend_escapes`,
  `field_lend_needs_a_place`, both re-run below).
- **§ 5 is FALSE, and wider than the `=` binding.** `t` is a by-value parameter,
  not a declared `@` name, and its bytes changed. `spec/heroes-spec.md:119`:
  *"only a declared @ name can be mutated"*.

**So the warden is right and defect 065 as filed is wrong — but the tree already
knows.** `docs/work/DEFECTS.md:88-102` carries the correction. What it does
**not** carry is a correction to its own one-line summary, which still reads
*"falsifying `spec § 3` and `spec § 5`"* (`DEFECTS.md:66`). `records/lists`
reads the first field and the one-liner; a reader scanning the list is told the
falsified sentence. **The summary line is the part every instrument reads and it
is the part still false.** Records are append-only, so the fix is a correction
line, not an edit — but the sitting should say so, because panel 165's
resolution table (`165-...md:144`) carries the same false cell.

**And the disagreement the two seats have about route A is resolvable, which
neither noticed.** The warden says route A must be amended to *"a `@` name"* or
§ 5 stays false in every by-value function, costing two ratified assertions.
The engineer says refusing a by-value parameter is over-refusal, because that is
the read-only wrapper §4.19 prescribes, and names the same two assertions. I read
them: `selfhost/check/lending.hero:304` and `:309` are both `function g(s: S)`
lending `s.b.ptr()` into `f(p: ptr, n: i64)` and both asserted `swept == 0`.
Both seats are describing the same two lines and calling them a cost and a fatal
flaw. **The engineer's sentence is the one to take seriously — *"the question is
does C write, and the language cannot see that from the binding"* — and it is
true of the binding and false of the program: clang can see it from the header,
and Heroes already asks clang.** § 3.1.

Also settled, because the ergonomist asked and nobody ran it — its *"one more
thing the sentence does not bound"*, that `f.ptr()` may reach a plain record and
break § 3 for ordinary programs:

```
$ ./heroes run plain.hero        # record Bag with data: u8[8], no extern group
error[fixed_outside_a_group]: `u8[8]` is a C array member, so it belongs to a
  `record` inside an `extern` group (§4.19)
```

**Refused. The blast radius of 065 is extern-group records only**, and the
resolution should say so, because the document does not bound it.

### 1.4 `getcwd`: the ffi-pragmatist is right and the shared brief is false

```
$ ./heroes run f_getcwd.hero
119        exit 0
$ pwd | tr -d '\n' | wc -c
119
```

with `function getcwd(buf: ptr, size: u64) -> cstr owned free` and a `free`
declared in a second group (which the compiler demands: `error[unknown_freer]`).
**The shared brief's *"leaves `getcwd` unwritable — the gap panel 164 opened it
for"* is false**, and it is load-bearing, because it is the stated cost of route
F in the one place a seat would read it.

**One caveat the pragmatist's report does not mark and the rule requires.** Its
sentence *"`getcwd(NULL, 0)` allocates on macOS and glibc"* is run on macOS and
**unrun on glibc and on Windows**. `.claude/rules/platforms.md` — a platform
fact is run on a platform or it is an inference. POSIX leaves `getcwd(NULL, …)`
unspecified; the allocating behaviour is an extension. Route F's true price is
therefore *unrun on two of the three platforms*, and a sitting weighing F on that
sentence is weighing one platform.

---

## 2. Claims asserted but not settled by the command that settles them

**Including the coordinator's**, as the brief asks. I re-ran the coordinator's
two after-briefs findings and **both hold**: `i32[4]` is refused
(`error[bad_operand]: 'ptr' takes a fixed run of bytes — 'i8[N]' or 'u8[N]',
found 'i32[4]'`), and `selfhost/check/lend_types.hero:123` refuses the dynamic
`[u8]` deliberately and in its own words. `spec/heroes-spec.md:361` does license
`i32[4]` as a field, so § 13 is wider than the compiler — confirmed.

The ones that do not hold:

**a. The compiler-engineer's "exit 133" does not reproduce, and the truth is
worse than the brief.** The engineer's first correction says the shared brief's
own 063 program *"prints 7 and then exits 133"*, and concludes *"a big lie traps,
a small lie corrupts in silence… a resolution written against the trapping case
will aim at the wrong half."* Run on the brief's exact shape — `signed char
name[8]; int32_t id;` — sweeping the extent:

```
n=24 32 48 64 96 128 256 1024 4096   ->  every one: "7", "1094795585", exit 0
```

**Nothing traps. A 4096-byte WRITE past an 8-byte field returns at exit 0.**
The shared brief's *"both at exit 0"* is correct, the engineer's correction of it
is not reproducible here, and the inference built on it is unsupported. This
matters twice: it removes the engineer's argument that the resolution should aim
at the small overshoot, and it makes 063 a **strictly larger** defect than any
document in the sitting states — the brief and the defect entry both show a 4096
byte *read*; the write direction is equally silent.

**b. The compiler-engineer's 46 and 12 do not reproduce under any dedup I could
construct.** The shared brief's 66 and 18 reproduce exactly. Then:

```
grep -rh  "function .*: ptr"           … | sort -u | wc -l            ->  59   (21 with a length sibling)
grep -rhoE "function [a-z_]+\([^)]*: ptr[^)]*\)" … | sort -u | wc -l  ->  34   (10 with a length sibling)
```

Neither is 46/12, and the engineer's central condition on route C rests on
*"I read the twelve"*. The ffi-pragmatist read **eighteen** and classified them
4/7/3/2/2. **Two seats read two different sets, neither of which is the other's,
and both called it the same evidence.** Their shared conclusion — that a
single-name `counted_by` does not express the corpus — survives, because both
found the `n * size` products; the arithmetic under it does not.

**c. The ffi-pragmatist's *"Real C library functions whose extent is a single
named sibling: zero"* is contradicted by its own § 2 output**, four pages
earlier in the same report:

```
ssize_t read(int, void *__attribute__((__sized_by__(__nbyte))), size_t __nbyte);
char *getcwd(char *__attribute__((__counted_by_or_null__(__size))), size_t __size);
```

Those are single named siblings, in the SDK, for the two functions panel 164
opened the lend for. The sentence is true of *this repository's eighteen* and
false as written, and it is quoted as a reason route C serves nobody.

**d. The spec-warden priced one wording and demands another.** D1 is measured as
*"§ 13: lends a **`@`** binding's field"*, **+3 real**. Its verdict then says
*"the refusal must read **a `@` name**, covering the immutable PARAMETER measured
above"*. Those are different strings; +3 prices the first. The warden's own
prediction (*8109 ± 2 real*) is registered against the unpriced one.

**e. The spec-warden's grammar warning is the most useful unattended finding in
the sitting and it is filed as a footnote.** *"The `grammar` suite read 7 passed,
0 failed with D3 applied and with D4 applied — a production no parser accepts is
green."* That is a hole in the net, not a note about this sitting, and under
`.claude/rules/verification.md` § The map is not written here it is exactly the
shape that expires in silence. It belongs in the resolution or in DEFECTS, not
in § 7 of one seat's report.

**f. Nobody ran the historian's own condition 3.** The historian names as a
verdict-changing fact *a stated reason from Swift for the struct-field
carve-out*, and says the reason would be *"a field's count expression names
another field, which a parameter-level wrapper cannot see."* Heroes' case is
precisely a **field passed to an annotated parameter**, and the sitting's answer
to whether that carve-out applies here is **unrun** — the historian has no shell
and the coordinator settled a different prediction.

---

## 3. Routes nobody listed

### 3.1 Route H — the HEADER's own `const` decides, and it already ships

**The lend is emitted as `void *` for every call, read or write.** One line:
`selfhost/emit/field_lend.hero:108`. The file's own comment,
`field_lend.hero:17-22`, says the choice was considered:

> *"WHAT THE CAST IS, AND WHY IT IS `void *` AND NOT `const void *`. C writes
> through this pointer as well as reading through it… A `const` cast would close
> the direction the sitting was convened to open."*

That is a **false dichotomy**: always-const or never-const. Nobody asked for
const-by-default with an opt-in. Measured on the compiler's own emitted C, with
the project's own flag list (`selfhost/cli/flags.hero:90-108`) reproduced
verbatim:

```
baseline, today's void * lend                    -> exit 0
the same file with the lend temp made const:
d063.hero:12:19: error: passing 'const void *' to parameter of type 'void *'
  discards qualifiers [-Werror,-Wincompatible-pointer-types-discards-qualifiers]
./sl.h:8:34: note: passing argument to parameter 'p' here
```

and the read direction, same patch, against `sum_n(const void *p, int64_t n)`:

```
exit 0 — clean
```

**What that means.** `-Werror=incompatible-pointer-types-discards-qualifiers`
is already in the flag set (`flags.hero:106`, panel 103, 2026-09-03). So a lend
emitted `const` makes the **write direction a hard build error against the real
header**, and the read direction compile clean, with **no mark in Heroes, no
grammar change, no parameter mode, no new keyword, and no new checker rule.** It
is §4.19's own mechanism — *clang verifies the declared signature against the
real header* — applied to the one question the language cannot answer for itself.

It settles the A-versus-B deadlock on its own terms:

- it answers the compiler-engineer's objection to A word for word (*"the
  question is does C write, and the language cannot see that from the binding"*
  — the header can, and Heroes already asks it);
- it leaves `check/lending.hero:304` and `:309` **green**, because those lend
  into a reading function — the exact over-refusal the engineer measured, gone;
- it closes **065** completely and at the class: an unmarked lend cannot reach a
  writing C function at all, so no `=` binding and no by-value parameter is ever
  written, and § 5 becomes true with **zero spec tokens** (the warden's own
  D1-with-nothing case);
- it costs no runtime and no ABI: the same address, a qualifier.

**What it costs and what it owes, honestly.** The write direction then needs a
way to be asked for, and the shipped golden's `slot_fill(p: s.nsap.ptr(), n: 8)`
is one of the **four** real lend call sites in the repository, so the migration
is one line. Two things are **unpriced by me and I am not the seat that prices
them**: the checker half that carries the mark down to the emitter, and the
diagnostic path — my measured clang error carries `d063.hero:12:19` from the
`#line` directives, but it carries no `heroes-ffi-` marker, so
`selfhost/cli/pointee.hero`'s reader would need a row before the user sees exit 1
rather than a clang qualifier error. **The engineer and the pragmatist should
price it; I claim only that it works.**

### 3.2 Route H′ — a distinct spelling for the write lend, because `@` is occupied at both ends

Measured in § 1.1: `@` means `void **` at the declaration and *copy the result
back* at the call. So the mark the ergonomist and the historian want cannot be
`@` and must be a word of its own — on the **lend**, not on the parameter, which
is the engineer's own veto-lifting condition. Something in the shape of
`s.name.ptr()` for the read and a second built-in for the write. That is the
combination none of the seven routes is: **the mark is at the call site where
the ergonomist's locality rule and Swift/Hylo's precedent both put it, the
refusal is clang's against the real header, and no existing spelling gains a
second meaning.**

It is unpriced. It owes `.claude/rules/diagnostics-and-goldens.md` § A new
surface form nothing — a second built-in is not new surface syntax — which is
the same advantage the engineer measured for route D and the same cost route C
does **not** avoid.

### 3.3 Route I — attach the extent to the LEND rather than to the parameter

The whole of route C's ~210 lines buys one thing: a **relation** between the
pointer and the number, which `sum_n(p: ptr, n: i64)` does not have. Route C
creates it at the declaration. It could be created at the call, inside the lend:
`s.name.ptr(8)`. Today:

```
$ ./heroes check ptrarg.hero
error[builtin_shape]: `ptr` does not take 2 argument(s) of those types
```

A built-in taking an argument is not new surface: no grammar, no `CParam`
clause, no contextual keyword, no formatter round trip, no `tmLanguage`, no
`highlight.ts`, and the checker instrument the engineer already identified
(`check/walk.hero:317-338`'s `fixed_bound_check`) applies unchanged. **Its known
weakness, stated so nobody has to find it: it does not stop `s.name.ptr(8)`
beside `n: 64`** — it makes the honest number checkable without making the
dishonest one impossible. I raise it because the panel is about to pay ~210
lines and +51 spec tokens for a relation, and nobody asked whether the relation
has to live at the declaration.

### 3.4 The `static inline` shim is not in the list at all

The ffi-pragmatist found it and the resolution must enumerate it, because A–G do
not and a resolution that lists seven routes is a claim about the option set
(CL-057). Its property — *the extent is `sizeof x->b`, C's own, which no Heroes
call site can overstate* — makes it the only route where **063 is impossible by
construction**, which is strictly stronger than any check. Combined with route H
it is a complete answer to the write direction with nothing added to the
language: **the lend stays read-only and const, and writes go through a shim
whose extent is C's.** That combination is not on the table and it costs the
language nothing.

### 3.5 Unmeasured, named so it is not rediscovered as new

`-fbounds-safety` on a **probe-only translation unit**. The pragmatist measured
that the dialect breaks the whole emitted C, and two of its seven errors are the
runtime's `const void *elem` callback which *"every emitted unit carries"* — a
probe-only unit might not. **Unrun.** I flag it only to record that the
pragmatist's refusal was measured on the whole unit and not on the narrow one;
the `_Static_assert` route dominates it anyway, since it needs no dialect.

---

## 4. The question the sitting should have asked and did not

**How long is a lent address valid, and what stops C from keeping it?**

The brief asks *what should a `ptr` lend of a fixed byte field PROMISE*. Extent
and mutability are two of three. The third was never asked, and it is a defect
nobody has filed. Nine lines, `--include .`, no diagnostic:

```
function stash()
    s: K @ k_make()
    k_register(p: s.name.ptr())      # C stores the address in a global

function main()
    stash()
    print(to_str(noise()))           # 10   — the frame is reused
    print(to_str(k_read_later()))    # 64   — garbage; the honest answer is 72
```
```
$ ./heroes run keep.hero                 ->  10 / 64,  exit 0
$ ./heroes run keep.hero --sanitize
AddressSanitizer: stack-use-after-scope on address 0x00016f60a320
READ of size 1 ... This frame has 1 object(s): [32, 48) 'h0_s'
```

**Why this is not covered by anything already refused.** The compiler refuses the
two escapes that happen on the **Heroes** side, and I re-ran both:

```
k: ptr @ s.b.ptr()            -> error[field_lend_escapes]
sl_make().name.ptr()          -> error[field_lend_needs_a_place]
xs[0].name.ptr()              -> error[field_lend_needs_a_place]
```

The escape that actually happens at an FFI boundary is on the **C** side, and
nothing looks at it: `sqlite3_bind_text` with `SQLITE_STATIC`, `iovec`,
`setvbuf`, and every registration API in C keeps the pointer. This repository's
own `examples/ledger/db/sqlite.hero:107` declares `sqlite3_bind_text`.

**And the language already has the shape of an answer, which makes the omission
visible rather than merely unlucky.** `spec/heroes-spec.md:369-372`: *"`x: cstr @
s.lease()` is a COPY of the bytes that C may read for as long as the program
says, and `end_lease(@x)` frees it… a lease nobody ends, like a handle nobody
consumes, aborts when `main` returns, saying how many."* The `cstr` lend COPIES
and carries a lifetime; the field lend copies nothing and carries none. **Panel
164 opened the address route without copying the lifetime rule beside it**, and
the diagnostic the compiler prints today already states the danger in its own
words — *"C may write through this pointer, and a write into a value nothing
holds lands in storage the next statement reuses"* — while refusing only the case
where **Heroes** can see the storage end.

This is a third defect of the same eight-commit-old feature and it is rank 3 by
the same clause as 063 (design.md §1.12, memory corruption). It should be filed
and it changes the shape of the resolution: **routes A–G, plus H, H′ and I, close
extent and mutability and not one of them closes lifetime.**

**Second, smaller, and procedural.** Panel 165's resolution 3 adopted route 14
and then corrected it as not implementable, and `docs/work/DECIDE.md:26` still
queues that whole resolution for the author's ratification. Panel 166 must say
what happens to it, or the author ratifies a resolution one of whose clauses is
known to be unimplementable by the session that wrote it.

---

## 5. What the resolution must account for, in one list

1. **Lifetime.** File it. No route on the table touches it, and it is measured:
   `stack-use-after-scope`, exit 0.
2. **`@` is occupied twice** — `void **` at the declaration, copy-back at the
   call. Any resolution that marks the write must spend a NEW word, and the
   place it costs least is the lend.
3. **The header's `const` is a refusal that already ships** and needs no word at
   all. If the panel adopts nothing else, this closes 065 at the class for zero
   spec tokens and leaves the read-only wrapper legal.
4. **Route C must be named as one of three routes**, not one letter. Say whether
   the number is compared in Heroes or in C. They have different coverage
   (`partial` records, products, macro names), different costs, and only one of
   them has been compiled.
5. **A group `constant` as the extent is lowered to a function call.** Route C's
   check-time form refuses the careful reader and accepts the careless one's
   literal. Whatever lands must say what it does with `n: SL_NAME_LEN`.
6. **The population is four call sites**, in `tests/golden/run/ffi-a-byte-field-
   crosses-to-c.hero:47,51,54,55`. Migration cost is four lines; the 66/18/46/12
   argument is about a different question.
7. **The brief's `getcwd` sentence is false** and route F must be re-weighed
   without it — and the replacement (`getcwd(NULL, 0)` allocates) is unrun off
   this Mac.
8. **063 is silent at every extent I tried**, up to a 4096-byte write at exit 0.
   No resolution should rest on a trapping case.
9. **Defect 065's summary line and panel 165's verdict table still say § 3 is
   falsified.** Records are append-only; both owe a correction line.
10. **The `grammar` suite passes a production no parser accepts.** Whatever
    lands, the production and the parser land in one commit — and the hole itself
    deserves an item somewhere other than a footnote.
11. **§ 13 is wider than the compiler in three places**, not one: `i32[4]` (the
    coordinator found it), the plain-record field (refused by
    `fixed_outside_a_group`, run here), and the lifetime silence. A wording that
    repairs § 5 and leaves § 13 unbounded repairs the smaller half.

---

## 6. Commands, so every number above can be re-run

```sh
cp -r <trunk> <scratch>/tree && cd <scratch>/tree && rm -rf archive build target heroes
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes        # real 4.98 user 4.88
export HEROES_RUNTIME=<scratch>/tree/runtime

./heroes run eq.hero                 # 72 / 65 / 72   — § 5 false for a by-value parameter
./heroes run plain.hero              # error[fixed_outside_a_group] — § 3 safe for plain records
./heroes run d063_32.hero            # 7 / 1094795585, exit 0, at every n from 24 to 4096
./heroes run keep.hero               # 10 / 64, exit 0
./heroes run keep.hero --sanitize    # AddressSanitizer: stack-use-after-scope
./heroes run f_getcwd.hero           # 119 == `pwd | tr -d '\n' | wc -c`
./heroes check atcall.hero           # error[not_a_place] — @ at the call site is copy-back
./heroes build b_at.hero --emit-c    # give((void *)&h0_q, t2) — @p: ptr is void **
./heroes build konst.hero --emit-c   # t5 = h_konst_SL_NAME_LEN() — the extent is a CALL
./heroes check ptrarg.hero           # error[builtin_shape]: `ptr` does not take 2 arguments

# route H, on the compiler's own emitted C, with the compiler's own flags
./heroes build d063.hero --emit-c -o d063.c
sed 's|void \* t16;|const void * t16;|; s|t16 = (void \*)|t16 = (const void *)|' d063.c > d063_const.c
clang -std=gnu11 -g -Wall -Werror=return-type -Werror=uninitialized -Werror=format \
  -Werror=conditional-uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks \
  -Werror=shorten-64-to-32 -Werror=sign-conversion \
  -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types \
  -fsigned-char -I runtime -I . -fsyntax-only d063_const.c
  # write direction: error, discards qualifiers, at d063.hero:12:19
  # read direction (konst.c, same patch): exit 0

printf '#if __has_attribute(access)\n#error HAS_ACCESS\n#endif\nint x;\n' | clang -x c -fsyntax-only -
clang -std=c11 -I. -fsyntax-only rc_named.c    # 'expression evaluates to 16 <= 8', no route E
```

Everything in this report was run on `arm64-apple-darwin25.6.0`. The one
platform claim I did not run, and say so in those words, is whether
`getcwd(NULL, 0)` allocates on glibc and on Windows: **unrun**.
