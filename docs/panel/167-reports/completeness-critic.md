# Panel 167 — completeness critic: what the sitting did not run

**No verdict.** This seat names what is missing, what was asserted without the
command that settles it, which side of a contradiction is checkable, and the
question the sitting should have asked.

Every number below was produced by a command run on 2026-09-20 on
`arm64-apple-darwin25.6.0`, in a COPY of the tree at `28df0084`
(M-declared-extents step 6, routes H and C both landed), built from the seed in
`4.16s user, exit 0`. Where a claim could not be run it says **unrun** in those
words. `archive/bootstrap-rs/` was never touched. The repository itself was not
modified except by this file; the spec edits below were made in the copy,
measured, reverted, and the revert diffed clean.

---

## 0. Two things I ran that move the ballot, before anything else

### 0.1 The compiler-engineer's completeness ground for the route B veto is false, and the program that falsifies it is in this sitting's own papers

The compiler-engineer's second veto ground reads: *"Route B alone breaches
§1.12's completeness clause, and this is run: nothing reaches a `ptr` parameter
today except `nullptr` and a lend … So `keeps` is a refusal with **no route
out**."* The seat tested `[u8]`, `str` and a `cstr` lease, got three
`type_mismatch`es, and concluded the set was empty.

The llm-ergonomist, reading only the spec, wrote the route out on the first try
and could not compile it. **I compiled it.** `routeout.hero`, verbatim from that
seat's report:

```
extern "stdlib.h"
    function malloc(n: u64) -> ptr
extern "string.h"
    function memcpy(dst: ptr, src: ptr counted_by n, n: u64) -> ptr

function install()
    s: KSlot @ KSlot(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
    buf = malloc(n: 8)
    _ = memcpy(dst: buf, src: s.name.ptr(), n: 8)
    k_register(p: buf, n: 8)
```

```
$ ./heroes check routeout.hero     ->  exit 0, no diagnostic
$ ./heroes run   routeout.hero     ->  10 / 72,  exit 0      # 72 is the honest answer
$ ./heroes run   routeout.hero --sanitize | grep -c AddressSanitizer  ->  0
```

C's own allocator reaches a `ptr` parameter. It needs no new construct, no new
spec token, and it is the one route §1.11 guarantees exists at all, since
everything comes from C. The seat's sentence is a negative claim resting on the
searcher's vocabulary rather than on the world, which is CLAUDE.md § RUN IT's
named shape (CL-018): the tell is *"nothing reaches"*, and the three things
tried were all Heroes-owned types.

**What this does to the ballot.** Route B's only veto is the compiler-engineer's,
and it rests on two grounds. One is now false. The other, the ratchet, I
re-measured and it holds (§ 4 below), but a ratchet is *raise three numbers or
split one file with its measurement in the commit body*, not a refusal. The
sitting is currently carrying a veto whose stated ground nobody checked, and two
seats had the falsifier in their own reports without either reading the other's.

### 0.2 The `cstr` lease, which three seats propose to replicate for fields, is not sound today

Route A is recommended by the ffi-pragmatist (approve), the historian (approve,
best-attested in the survey) and the compiler-engineer (approve on condition),
all on the premise that the `cstr` lease works and its three clauses generalise.
**I ran the shapes beside the lease and the premise does not hold.**

`check/leasing.hero:14-24` states in its own words why clause 1 exists: the
pointer must live in **one cell**, because *"a copy of it … would go on pointing
at the bytes after the cell was nulled, and a read of the copy through C is a
use-after-free `check` could never see."* Clause 1 permits a lease name **as an
argument of a call**. A record constructor is a call.

```
extern "extrec.h"
    record Holder                  # an extern group's record MAY hold a cstr,
        p: cstr                    # and the corpus ships that shape:
        n: i64                     # tests/golden/fixedbugs/a-header-field-that-is-not-const.hero
    function take(h: Holder)
    function peek() -> i64

function main()
    label: cstr @ "Hpayload".lease()
    h = Holder(p: label, n: 8)     # the pointer now lives in a SECOND Heroes cell
    end_lease(@label)              # the first cell is nulled and the bytes freed
    take(h: h)                     # the second cell is handed to C
    print(peek())
```

```
$ ./heroes check extrec2.hero   ->  exit 0, no diagnostic
$ ./heroes run   extrec2.hero   ->  0        exit 0      # the honest answer is 72
$ ./heroes run   extrec2.hero --sanitize
==20253==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000001c10
```

The three clauses are otherwise intact and I checked each: a copy in the same
function is `lease_escapes`; `return label` is `lease_escapes` plus
`cstr_out_of_heroes`; a **Heroes** record is `cstr_in_a_record`; `end_lease` in a
callee is `end_lease_of_no_lease` plus `lease_written`. The one door left open is
the `extern` group's record, and the `cstr_in_a_record` diagnostic's own note
says why it is open: *"A group's `record` may hold a `cstr`, because there the
fields are the header's and **C owns the bytes**."* For a lease the bytes are
the **program's**. The rule rests on a premise that a lease falsifies, and that
is `.claude/rules/module-shape.md`'s shape exactly: a premise about the world
that expires in silence.

It carries further. The lease pointer can be carried **out of its function**
this way, which is the thing the llm-ergonomist concluded was impossible:

```
$ ./heroes run carry.hero      ->  72 / 72
                                   panic: 1 lease(s) never ended
```

Writable, readable, correct while it runs, and unreleasable: `end_lease` takes
the cell and the cell died with the callee, so the leak gate fires at exit. The
ergonomist's conclusion (*"either inexpressible or a deliberate abort"*) is
right; the reason they gave is not, and the reason matters, because the place the
language does offer is the one that defeats clause 1.

**The question the sitting should have asked and did not:** *does the escape rule
that makes the lease sound survive being asked of a second type?* It does not
survive being asked of the shapes beside the first type. Route A spelled as the
compiler-engineer recommends makes this strictly worse, and § 2 shows by how much.

---

## 1. Contradiction one: the absorbing sentence at +13, and the instrument

### 1.1 The +13 is real, to the digest

Baseline re-measured on the binding instrument, not the vendored one:

```
$ ./heroes measure spec/heroes-spec.md --refresh
8154 real, claude-opus-5, digest 249990ca1b6b2f66
```

I reconstructed the C-absorbing draft from the spec-warden's report, applied it
to lines 370-374 in the copy, and refreshed:

```
8167 real, digest 44fa44bb11d45b03
```

**+13 confirmed, and the digest is byte-identical to the spec-warden's**, which
means my reconstruction of the wording is theirs. The ceiling is 10240; 8154
leaves 2086 free and 2026 net of §4.19's 60. The budget veto is nowhere near
engaged and that seat says so first, correctly.

### 1.2 What +13 buys is smaller than it looks, and this is checkable

The absorbing form makes *"a lend and a lease name stand only as an argument of a
call"* a **consequence** of *"a lend is over when the call returns"*. For the
lend that derivation is sound. For the **lease** it is backwards, and the
compiler says so in its own comment: clause 1 exists because the pointer must
live in one cell (`check/leasing.hero:14-24`), not because a lease is over when
the call returns, which it is not. So the cheapest draft joins two rules that do
not share a reason, and § 0.2 shows the lease half of the joined sentence is the
half that is currently false in the tree. That is not a verdict on route C; it is
a fact about the sentence, and no seat checked the lease side of it.

### 1.3 No instrument catches the escape behind a real library, and the sanitizer is worse than silent

The ffi-pragmatist's finding reproduced independently, against
`/usr/lib/libsqlite3.dylib`, with a `scribble()` call forcing frame reuse between
the bind and the step:

```
$ ./heroes run blob.hero            # five runs of five
AAAAAAAAAAAAAAAA                    # the honest answer is 0A141E28323C4650
...                                 exit 0, no diagnostic

$ ./heroes run blob.hero --sanitize | grep -c AddressSanitizer
0
$ ./heroes run blob.hero --sanitize
0A141E28323C4650                    # the CORRECT value
```

Confirmed, and **one degree worse than any seat wrote it down**. `--sanitize`
does not merely stay quiet when the retainer is a `.dylib`. It **changes the
answer to the right one**, because the sanitized allocator does not reuse the
frame the same way. A developer who reaches for the sanitizer to check this
program is told the program is correct. That is a heisenbug, and the sitting has
been treating `--sanitize` as the instrument that at least sees the class.

**And there is a second half of the defect no sanitizer can ever see** (§ 5.2).

---

## 2. Contradiction two: the three seats and the lease that copies

**They are talking about the same construct.** Each measured a different axis of
it, each result survives the other two spellings, and none of the three axes is
the one that decides.

| seat | what it built | what it measured | run by me |
|---|---|---|---|
| compiler-engineer | `p: ptr @ s.name.lease()` / `end_lease(@p)` | SURFACE cost | **confirmed** |
| llm-ergonomist | `p: ptr @ s.bytes.pin()` / `end_pin(@p)` | whether the program can be WRITTEN | confirmed, wrong reason (§ 0.2) |
| ffi-pragmatist | `hero_field_held(&h0_r.payload[0], 8)` into a cell | the RUNTIME layout of the copy | not re-run, **and it does not need to be** |

The compiler-engineer's surface claim, re-run:

```
$ ./heroes check surfA.hero
error[bad_operand]: `lease` takes `str`, found `u8[8]`
error[bad_operand]: `end_lease` takes `@x` where `x: cstr @ s.lease()`, found `ptr`
$ ./heroes fmt surfA.hero | diff - surfA.hero      ->  BYTE-IDENTICAL
```

Two errors, both from `check/lend_types.hero`, and the formatter round-trips.
Confirmed exactly.

**The axis nobody measured, and it is where the three constructs stop being the
same thing.** The `cstr` lease is held sound by clause 1 **plus two type-level
rules that do not exist for `ptr`**:

```
$ ./heroes check esc1.hero    # function install() -> cstr
error[cstr_out_of_heroes] + error[lease_escapes]
$ ./heroes check esc3.hero    # record Box { p: cstr }
error[cstr_in_a_record]

$ ./heroes check ptrhold.hero # record Box { p: ptr } ; function give() -> ptr
exit 0, no diagnostic
```

The compiler-engineer saw half of this and priced it as *"a cost that is real"*,
writing that *"clause 1 carries the whole rule alone"*. **Nobody then asked
whether clause 1 can carry it**, and § 0.2 answers: on `cstr`, with two
type-level rules helping, clause 1 already leaks through an `extern` record
constructor. On `ptr`, with neither rule and with `ptr` held in records and
returned from functions freely, the same construct has one clause between it and
the class the lease was built to close.

**So the cheapest spelling and the sound spelling are not the same spelling**, and
the sitting has been pricing the cheap one. The compiler-engineer's "zero parser,
zero AST, zero formatter" is true and is bought by reusing a type that carries
none of the lease's protections. Spelled `pin`/`end_pin` over a fresh type the
cost returns, and `check/builtins.hero` is at **378 of a DECIDED 378, zero
headroom** — but I checked and that file delegates lend and lease typing to
`lend_types.lend_call` at line 283, so a new builtin's type arm lands in
`check/lend_types.hero` (127 of 300) and `inventory.hero` (152 of 300), not in
the zero-headroom file. Route A is clear of every ratchet in either spelling.

On the ffi-pragmatist's third claim: `hero_str_held` does put the header
**immediately before** the bytes (`runtime/parts/str.c:419-428`, read today), so
the pointer handed to C is `base + sizeof(HeroHeldHeader)` and a C freer
receiving it frees a non-base address. That is confirmed by reading the source;
the SIGABRT itself I did not re-run and it is **unrun** by me.

---

## 3. Contradiction three: retention decided by an argument, priced

Two seats reached it independently (spec-warden's `retained_by d`, the
historian's Fortran/MPI placement) and a third gestured at it (the
ffi-pragmatist's mark on the header constant, explicitly flagged **unrun**).
Nobody priced any of them. Here is what can be priced, and one measurement kills
the value-reading version outright.

### 3.1 The discriminating value is not visible to the compiler, and I ran it in Heroes

The ffi-pragmatist proved the per-call split in **C** (`b2.c`). The version that
decides the question is in **Heroes**, through the language's own surface, and
nobody wrote it. One declaration, one call site, one `bool` chosen at run time:

```
extern "disc.h"
    constant D_KEEP: ptr
    constant D_COPY: ptr
    function bind_it(p: ptr counted_by n, n: i64, d: ptr)

function stash(which: bool)
    s: KS @ KS(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
    d: ptr @ D_KEEP
    if which
        d @ D_COPY
    bind_it(p: s.name.ptr(), n: 8, d: d)
```

```
$ ./heroes check disc.hero        ->  exit 0, no diagnostic
stash(which: false)  ->  10 / 10      # C retained; honest answer 72
stash(which: true)   ->  10 / 72      # C copied; correct
```

Both compile with zero diagnostics. **The retention discriminator is a value the
checker never sees**, for two reasons I confirmed separately:

- a group's `constant` **has no body** (spec § 13: *"the header holds the
  value"*), so `SQLITE_STATIC` and `SQLITE_TRANSIENT` are opaque to the compiler.
  `examples/ledger/db/sqlite.hero:54` declares `constant SQLITE_TRANSIENT: ptr`
  with nothing on the right;
- the argument need not be a constant name at all. It can be a local, assigned in
  a branch, as above. And a Heroes `constant` may be initialised from a group
  constant: `./heroes check constval.hero` exits 0.

So `retained_by d` **as drafted, reading the sibling's value, is undecidable
without flow analysis**, which `check/leasing.hero:29` refuses in those words
(*"No flow analysis"*) and which design.md Part 6 refuses as a borrow checker.
The spec-warden refused to price E and was right to; the reason is stronger than
that seat stated, and it is run rather than argued.

The only surviving shape is a mark on the **constant's declaration** (the
binding author's word that *this value means C copies*) **plus** a refusal of any
non-literal discriminator at the call. That is two marks and one new refusal, not
one mark.

### 3.2 The ratchet price of the argument-site route, which is the thing nobody computed

`retained_by d` is structurally identical to `counted_by n`: a `CParam` suffix
naming a sibling parameter. Its landing cost is therefore **measurable from its
own sibling**, which landed this morning:

```
$ git show --numstat --format="" 28df0084 -- selfhost      # +915 / -46, 16 files
    selfhost/parse/members.hero   +39 / -1
    selfhost/ast.hero              +4 / -1
    selfhost/print/fmt.hero       +11 / -1
    selfhost/print/dump.hero      +11 / -1
```

Against today's headroom (§ 4): **39 lines wanted where 2 are free, 4 where 1 is
free, 11 where 1 is free.** So the spec-warden's route E and the historian's
route F are **strictly worse than route B on the ratchet axis**, by the same
mechanism that produced the compiler-engineer's veto, and both were offered as
ways past it. A mark on the **constant** instead lands in
`selfhost/parse/group.hero` (235), `parse/decl.hero` (217), `parse/tails.hero`
(288) or `parse/type.hero` (280), none of them DECIDED, so that half is the
cheap half. Which of those four it lands in is **unrun**.

---

## 4. Contradiction four: the three ratchets, in the unit that judges them

I replicated `suite_layout.hero`'s `code_lines` (everything outside a `test`
block and not a blank line; a `#` at column 0 does **not** close a test block,
which is why an appended comment does not move the number) and then verified by
adding real code lines and watching the suite.

| file | measured | ceiling | free | probe |
|---|---|---|---|---|
| `selfhost/parse/members.hero` | **298** | 300 (§11) | **2** | +2 → 300, still green |
| `selfhost/ast.hero` | **524** | 525 (DECIDED) | **1** | +2 → 526, **red** |
| `selfhost/print/fmt.hero` | **1174** | 1175 (DECIDED) | **1** | +2 → 1176, **red** |
| `selfhost/check/builtins.hero` | **378** | 378 (DECIDED) | **0** | not reached by any route (§ 2) |

```
    selfhost/ast.hero: 526 lines of code, past the 525 it measured when this
    check was written — a file already over the ceiling may not grow further
    selfhost/print/fmt.hero: 1176 lines of code, past the 1175 …
```

Restored; `layout: 2 passed, 0 failed`. **The compiler-engineer's numbers are
exactly right**, independently reproduced, in the suite's own unit.

**Is the same true of the other routes? Measured, no.** Route A's landing files
are `check/lend_types.hero` 127, `check/leasing.hero` 195, `emit/body.hero` 224,
`emit/builtins.hero` 244, `emit/ops.hero` **281 (19 free, the tightest)**,
`inventory.hero` 152, plus a new module. Not one is DECIDED; all are under §11's
300. Route C touches no compiler file. Route D deletes. **Only the
declaration-site marks (B, and spec-warden's E) hit the ratchet**, and the
ratchet is the whole of the one non-soundness veto on the ballot.

---

## 5. Contradiction five: the `cstr` lend, reproduced, and the half of the defect nobody has an instrument for

### 5.1 The spec-warden is right, and the counts are right

```
$ ./heroes run keeps.hero            ->  10 / 0,  exit 0, no diagnostic   # honest: 72
$ ./heroes run keeps.hero --sanitize
==19950==ERROR: AddressSanitizer: heap-use-after-free on address 0x606000000270
```

The field probe reproduces beside it: `10 / 10`, exit 0,
`stack-use-after-scope keep.hero:16`. **Two directions, one defect**, confirmed
on my own build.

The corpus counts, re-run:

| | files | occurrences | in `examples/` |
|---|---|---|---|
| `.ptr()` | 8 | **33** | **0** |
| `.cstr()` | 39 | **59** | **10, in four shipped examples** |

`examples/curl/main.hero` 1, `examples/gallery/13-lease.hero` 1,
`examples/ledger/db/sqlite.hero` 4, `examples/sqlite/main.hero` 4. The
spec-warden's figures are exact. **The half nobody put on the ballot is the half
the corpus uses.**

### 5.2 Defect 066 is two defects, and the second one has no instrument at all

My own brief asks whether C keeping the address past the *frame* and C keeping it
past the *program's use of the record* have one answer. I built both. They do
not.

```
function main()
    s: KSlot @ KSlot(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
    k_register(p: s.name.ptr(), n: 8)
    print(k_read_later())                     #  72
    s @ KSlot(name: [1, 2, 3, 4, 5, 6, 7, 8], id: 1)
    print(k_read_later())                     #   1   <- C's view changed under it
```

```
$ ./heroes run alias.hero                                     ->  72 / 1, exit 0
$ ./heroes run alias.hero --sanitize | grep -c AddressSanitizer  ->  0
```

`s` never dies. There is no use-after-anything. The program and C simply disagree
about one field's bytes, and **no sanitizer will ever see this**, not even with
the retainer inlined into the program, because every byte read is live. It is not
a violation of § 3's sentence as it stands (*"No aliasing exists among the values
this language **owns**"*, repaired at panel 139), which is exactly why it is
invisible: the document was narrowed to exclude C, and this is C.

Every seat in this sitting reached for `--sanitize` as the instrument for defect
066. For this half it is not a weak instrument; it is **no instrument**. Routes A
and B close it (a copy, or a refusal). The C-absorbing sentence covers it in
words, since *over when the call returns* forbids it. Route D closes it by
removal. But the sitting has not noticed that half of the defect it is pricing
cannot be demonstrated by the only tool it has been using to demonstrate the
other half.

---

## 6. The coordinator's framing, which nobody was assigned to check

Checked line by line. Most of it holds. Four things do not.

- **"Four clauses, all on the Heroes side of the boundary"** over a table with
  **five rows**, one of which the brief itself annotates *"(clang's)"*. All five
  names are live (`grep` over `selfhost/`), but `field_lend_written` is route H's
  and is enforced by clang, so the sentence is wrong twice in eight words. I
  re-ran both of the ones the sitting depends on: `field_lend_uncounted` and
  `field_lend_written` each fire on a fresh program today.
- **The budget line is internally 60 apart.** *"8214 against the 10240 ceiling
  with the FFI floor's 60 mortgaged, 2086 free."* Measured baseline is **8154**;
  2086 free is `10240 − 8154`, and `10240 − 8214` is 2026. The spec-warden's own
  report states both correctly. Nothing turns on it, and a brief that is quoted
  onward should not carry it.
- **"+48 real, for route C, this morning" — CONFIRMED.** I measured `ef7b013b`'s
  spec at **8106** and today's at **8154**. That one is exact.
- **The file-size table ranks the wrong files, and this is the framing error that
  cost the sitting most.** The brief lists the five files the lend already lives
  in (357, 304, 275, 245, 245 by `wc -l`, all verified) and concludes *"every one
  of these is inside its decided ceiling"*. True and irrelevant: a declaration-site
  mark does not land there. It lands in `parse/members.hero`, `ast.hero` and
  `print/fmt.hero`, which the brief does not name, and which have 2, 1 and 1 lines
  free. The one seat that looked found the only veto in the sitting. A brief that
  ranks the files a route would actually enter would have put that in front of
  five seats instead of one.
- **"No shipped example lends a field at all, so a rule here costs the corpus …
  nothing more"** is true of `.ptr()` and false of the defect, per § 5.1. The
  brief's own § "What the language already owns" quotes the lease sentence and
  calls the asymmetry *"the defect stated as a design fact"*, which frames the
  `cstr` side as the **solved** side. It is not solved; it reproduces, and its
  lease leaks (§ 0.2).

On the *0 of 71* figure the brief rests its central claim on: that number is
about `cstr` parameters and I did **not** re-run it. The ffi-pragmatist re-ran
the underlying question against 3120 SDK headers and found `lifetimebound` in 0
files and `noescape` on block parameters only, which supports the conclusion by a
different route. My § 3.1 supports it by a third, stronger one: even where a
header does say, Heroes cannot read the value.

---

## 7. Did routes H and C narrow the defect? No, and it is run rather than argued

`keep.hero` satisfies **every rule the language has about lends**, including both
repairs that landed hours ago: the parameter declares `counted_by n`, the header
declares it `const`, the root is a `@` name, the callee is an extern, the lend is
in argument position. Both refusals are live and fire on programs that lack them:

```
error[field_lend_uncounted]: … `p` of `uncounted` does not say which argument the extent is
error[field_lend_written]:  … the header declares that parameter `void *` … `t` is not a `@` name
```

And `keep.hero`, which triggers neither, still prints the wrong number at exit 0.
**Routes H and C narrowed defect 066 by zero.** They answered *how far* and
*whether*, and the third question was never in their scope. Nothing in the tree
moved between the filing and this sitting.

---

## 8. What nobody asked

1. **Does the lease's escape rule survive a second type?** Asked as a cost
   (compiler-engineer) and never as a soundness question. Answer, run: it does
   not survive the shapes beside the *first* type (§ 0.2).
2. **What reaches a `ptr` parameter?** Asked by one seat with three guesses and
   answered by another seat's own program, which nobody compiled (§ 0.1).
3. **Can the compiler see the discriminating value?** Proved impossible in C by
   one seat; never asked in Heroes, where the answer is stronger and where it
   kills the two routes offered as ways past the veto (§ 3.1).
4. **Is `--sanitize` an instrument for this defect?** Treated as one by five
   seats. It is misleading behind a `.dylib` (§ 1.3) and blind to half the
   defect entirely (§ 5.2).
5. **What does a route cost in the files it would actually land in?** One seat
   asked; the brief pointed the other four at a different set of files (§ 6).

---

## What the resolution must account for

1. **The `cstr` lease leaks today**: a lease pointer copied into an `extern`
   group's record survives `end_lease`, is handed to C, and reads freed bytes at
   exit 0 with no diagnostic (`extrec2.hero`, `heap-use-after-free` under
   `--sanitize`), so any resolution that replicates the lease for fields
   replicates this.
2. **Route A spelled as an overload of `.lease()` over `ptr` loses two of the
   three protections the `cstr` lease has**, because `record Box { p: ptr }` and
   `function give() -> ptr` both check clean today (`ptrhold.hero`, exit 0) while
   their `cstr` equivalents are `cstr_in_a_record` and `cstr_out_of_heroes`.
3. **The compiler-engineer's completeness ground for the route B veto is false**:
   `malloc` plus `memcpy` reaches a `ptr` parameter today and prints the honest
   answer (`routeout.hero`, exit 0, `10 / 72`, zero ASan reports), so the veto
   reduces to the ratchet.
4. **The ratchet itself is real and reproduced**: `parse/members.hero` 298/300,
   `ast.hero` 524/525, `print/fmt.hero` 1174/1175 in the suite's own unit, with
   the last two going red on two added code lines, so a declaration-site mark
   owes a named split or three raised ceilings with the measurement in its body.
5. **The two routes offered as ways past that veto are worse on the same axis**:
   `retained_by d` is `counted_by n`'s shape, and `counted_by` landed +39 lines
   into the file that has 2 free, +4 into the one with 1, +11 into the other with
   1 (`git show --numstat 28df0084`).
6. **A mark that reads the retention argument's value cannot be checked**: a
   group `constant` has no body, and the discriminator can be a local assigned in
   a branch, so one Heroes program with one call site prints 10 or 72 by a
   run-time `bool` with zero diagnostics either way (`disc.hero`, check exit 0).
7. **The defect is two defects**: besides the dead frame, a live record rewritten
   while C holds its field's address makes the program and C disagree at exit 0
   with **zero** AddressSanitizer reports (`alias.hero`, `72 / 1`), and no
   sanitizer can ever see that half.
8. **`--sanitize` is not the instrument behind a real library, and it is
   actively misleading**: against `libsqlite3.dylib` the plain build prints
   `AAAAAAAAAAAAAAAA` five runs of five where `0A141E28323C4650` is honest, and
   the sanitized build reports nothing **and prints the correct value**.
9. **The defect binds both lend directions and the corpus lives on the other
   one**: `.cstr()` reproduces identically (`keeps.hero`, `10 / 0`,
   `heap-use-after-free`) at 59 occurrences with 10 in four shipped examples,
   against `.ptr()`'s 33 with none.
10. **Routes H and C narrowed nothing**: `keep.hero` declares `counted_by n`, is
    rooted at a `@` name and calls a `const` parameter, and still prints the
    wrong number at exit 0, while both new refusals fire correctly on programs
    that omit those things.
11. **The absorbing sentence costs exactly +13 real** (8154 → 8167, digest
    `44fa44bb11d45b03`, reproduced to the digest), and what it buys should be
    read against item 1: it derives the lease's one-cell rule from a lend fact
    that does not apply to leases, and the lease's one-cell rule is the one that
    is currently broken.
12. **The `cstr` lease's own clause 1 must be repaired whichever route wins**,
    because it is the mechanism every route except D depends on, and today a
    record constructor walks through it.

---

## Commands, so every number re-runs

```sh
cp -r <tree> $S/tree && cd $S/tree && rm -rf archive build target heroes
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes        # 4.16 s user
export HEROES_RUNTIME=$S/tree/runtime

./heroes run keep.hero            # 10 / 10, exit 0   (+ --sanitize: stack-use-after-scope)
./heroes run keeps.hero           # 10 / 0,  exit 0   (+ --sanitize: heap-use-after-free)
./heroes run alias.hero           # 72 / 1,  exit 0;  --sanitize | grep -c AddressSanitizer -> 0
./heroes run blob.hero            # AAAAAAAAAAAAAAAA x5; --sanitize -> 0 reports, right answer
./heroes run routeout.hero        # 10 / 72, exit 0, 0 ASan reports
./heroes run disc.hero            # 10/10 with D_KEEP, 10/72 with D_COPY, both check-clean
./heroes check extrec2.hero       # exit 0 ; ./heroes run -> 0 ; --sanitize -> use-after-free
./heroes run carry.hero           # 72 / 72 then "panic: 1 lease(s) never ended"
./heroes check ptrhold.hero       # exit 0 : a record holds a ptr, a function answers one
./heroes check esc1.hero esc3.hero copy1.hero dbl.hero   # the four refusals that DO fire
./heroes check surfA.hero         # two bad_operand ; ./heroes fmt surfA.hero | diff -  -> identical
./heroes check constval.hero      # exit 0 : a Heroes constant takes a group constant

./heroes run tests/harness/main.hero -- ./heroes layout          # 2 passed, 0 failed
python3 cl.py selfhost/...                                       # suite_layout's own code_lines
  298 parse/members.hero | 524 ast.hero | 1174 print/fmt.hero | 378 check/builtins.hero
  127 check/lend_types  | 195 check/leasing | 281 emit/ops | 224 emit/body | 152 inventory

. <trunk>/.env
./heroes measure spec/heroes-spec.md --refresh                   # 8154, 249990ca1b6b2f66
  with the C-absorbing draft applied                             # 8167, 44fa44bb11d45b03
  git show ef7b013b:spec/heroes-spec.md                          # 8106  => this morning was +48

git show --numstat --format="" 28df0084 -- selfhost              # +915/-46, 16 files
grep -rl '\.ptr()'  --include='*.hero' tests/golden examples | wc -l   # 8   (33 occurrences)
grep -rl '\.cstr()' --include='*.hero' tests/golden examples | wc -l   # 39  (59 occurrences)
```

Working files: `<scratch>/cc/tree` (the built copy),
`<scratch>/cc/work` (the Heroes reproducers and their headers). The repository
was not modified; the two spec edits were made in the copy, measured and
reverted, and `diff` against the pre-edit backup is empty.
