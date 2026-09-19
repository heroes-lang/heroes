# Panel 165 — measurements the coordinator ran AFTER the briefs went out

**This file is not a seat and gives no verdict.** It exists because three
measurements were run after the five briefs were sent, so no seat had them, and
hiding that would be worse than the irregularity of recording it. They were
provoked by the llm-ergonomist's report, which arrived first and asserted two of
them from the specification's text alone, marked `unrun`. They are run here.

Nothing in the working tree was changed. All three are `check`/`build` runs on
scratchpad files.

## 1. An array extent must be a LITERAL — a header constant cannot be one

The ergonomist derived this from `spec § 3`'s production,
`Type = Prefix { "[" integer "]" }`, where `integer` is a lexer token, and marked
it unrun. It is true, and the compiler has a dedicated diagnostic for it:

```
$ ./heroes check konst.hero
extern "konst.h"
    constant SLOT_NAME_LEN: i64
    record Slot2 tag slot2
        name: i8[SLOT_NAME_LEN]

error[expected_array_length]: expected the array's length after `[`, found a
  name (`SLOT_NAME_LEN`) — a C array member is `i32[4]`, and the length is part
  of the type (§4.19)
  at konst.hero:4:18
```

**Consequence for route 6.** `spec § 13`'s header constant — the one mechanism
the language has for a number that comes from the header — **cannot be used as an
extent**. So an author cannot write `i8[L_tmpnam]`; they must write `i8[20]` or
`i8[1024]`, and the shared brief measured that one of those two is wrong on the
other platform. Route 6's portability problem is therefore **structural**, not a
matter of author discipline. No seat was told this.

## 2. A FIELD's wrong extent is caught by clang, loudly, on every platform

```
$ ./heroes build wrong.hero        # header says char name[8]; Heroes declares i8[20]
error[ffi_field_type]: `Slot3.name` is not `i8[20]` in `wrong.h` — clang read the
  header's struct and the field disagrees
  at wrong.hero:3:9
  note: a group's `record` IS the header's struct (§4.19), so every field is at
        the header's own width and sign
```

So a field's literal extent is safe **by refusal**: a declaration that disagrees
with the header on the machine you are building on does not build. Writing
`i8[20]` for a field whose header says 1024 fails on that machine, at build time.

## 3. And a PARAMETER's extent cannot be checked that way, by construction

C adjusts a parameter written `T a[N]` to `T *a` (C11 §6.7.6.3p7), measured in
the shared brief with the AST dumps. **So the header's declaration of that
parameter contains no extent for clang to compare against.** The check route 6
would add compares the Heroes declaration to the Heroes argument; nothing
compares either of them to the header.

**This inverts route 6's own selling point** as panel 164 stated it — *"it is the
only route of the seven where the compiler CHECKS the extent instead of trusting
the author or the callee"*. The extent IS checked, and what it is checked against
is the author's other declaration. The one fact nobody can check is the one the
field mechanism in §2 above checks today.

**And the coordinator must correct its own sentence here**, because the first
draft of this file called recovering the extent *a question, not a claim* — and
this session had already built the instrument that does it. `census.py`, the tool
behind the shared brief's numbers, reads a parameter's written extent out of the
header by taking clang's `ParmVarDecl` **source range** and re-reading the
header's own bytes through it. That is an existence proof: **a parameter's
declared extent IS recoverable from the header.**

So the honest statement of §3 is narrower and more useful than *it cannot be
done*:

- `selfhost/emit/extern_probe.hero` works by **compiling C** against the real
  header, and C has thrown the number away before that probe can ask. By the
  route the compiler uses today, the extent is unrecoverable.
- Recovering it needs a **second kind of instrument** — a source-range reader for
  headers, which is not a C compilation and is a new class of thing at the
  boundary. Its cost is unpriced and belongs to the compiler-engineer and the
  ffi-pragmatist, not here.

That distinction matters to the verdict, because it turns *route 6's check cannot
reach the header* from a soundness refusal into **a price**. Whether the price is
worth paying for the numbers in the shared brief is the sitting's question, and
whether clang exposes the extent by some cheaper route — `__counted_by`, a
source-level query — the coordinator did not search for.
