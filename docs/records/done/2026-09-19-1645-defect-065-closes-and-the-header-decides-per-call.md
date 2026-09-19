# Defect 065 closes, and the header decides per call

2026-09-19, M-declared-extents step 5. Found by panel 165's completeness critic;
the route that closes it, route H, was found by panel 166's completeness critic
and ratified by the author the same day, reading the sitting. The first landing
of the route was reverted that afternoon (`6823cca7`), and what it stopped on is
the one question this step answers.

- [x] **065 — C writes into an IMMUTABLE binding, with no `@` anywhere** | a field lent with `f.ptr()` from a binding declared `=`, or from an immutable parameter, is written by C and read back changed, falsifying `spec § 5` at exit 0 | `spec § 5`'s `@` sentence, `selfhost/emit/field_lend.hero`

    **Origin:** panel 165's completeness critic, 2026-09-19. No seat found it,
    and the shipped golden `tests/golden/run/ffi-a-byte-field-crosses-to-c.hero`
    demonstrates the write as correct behaviour — on a `@` binding, which is why
    it never showed.

    **Reproducer**, and `t` is bound with `=`:

        t = sl_make()
        print(to_str(t.name[0].to_i64().must()))   # 72
        sl_fill(p: t.name.ptr(), n: 8)             # no @ on the binding,
        print(to_str(t.name[0].to_i64().must()))   # none on the parameter,
                                                   # none at the call site
        before: 72
        after : 65

    **What it falsifies, in the document a reader is told to trust.** `spec § 5`:
    *"`=` binds once, forever"* and *"only a declared `@` name can be mutated"*.
    `spec § 3`: *"Every value behaves as an independent copy."* Both are false
    for this program and both are what the language sells.

    **CORRECTION, 2026-09-19, by panel 166's spec-warden and re-run by the
    session that wrote the line above. `spec § 3` is NOT false.** Measured: after
    the lend is written, the lent binding reads **65** while a copy of it reads
    **72**, a record field reads **72** and an array element reads **72**. No
    other value this language owns sees the write, because a lend stands only as
    an argument of a call (`selfhost/check/lending.hero`, `field_lend_escapes`),
    so no two owned values ever alias. **Exactly one sentence is false**, § 5's
    *"only a declared `@` name can be mutated"* — and § 13 already licenses what
    falsifies it, so the document is **contradictory rather than incomplete**.

    **And the falsification is WIDER than the `=` binding**, which the entry
    above did not know: an immutable **parameter**'s field is written and read
    back inside the callee while the caller still sees the old bytes. So a repair
    written as *"refuse an immutable binding"* leaves § 5 false in every
    by-value function.

    **What is owed, and it is a question this entry does not answer.** Either
    the lend of a field to a `ptr` a C function may write requires `@` at the
    binding, the parameter and the call site — which is `spec § 13`'s own
    sentence *"A C out-parameter is an `@` parameter"* reaching one more type,
    and is what panel 165's llm-ergonomist held its veto for — or the language
    says in writing that a `ptr` lend is a hole in `=`. The first is a language
    change and owes a sitting.

    **Closed by** `M-declared-extents step 5`, route H as panel 166 adopted it:
    the header decides, per call, and the compiler carries the answer.

## What the route is, in one sentence

A lend rooted at a name the program may not write crosses as `const void *`; a
lend rooted at a `@` cell crosses as `void *`; and clang refuses the first
against a parameter the header spells without `const`, with
`-Werror=incompatible-pointer-types-discards-qualifiers`, a flag this compiler
has shipped since panel 103. Zero spec tokens, no new mark, no grammar.

## The question the first landing stopped on, and its answer

`6823cca7` built the route far enough to run and reverted it: the lend's result
is a temporary, `emit/body.hero`'s prologue declares temporaries **by Heroes
type**, `ptr` has one C spelling, and so `t4 = (const void *)(h0_t.name)` into
`void *t4` discarded the qualifier on the assignment — one line before the call,
and refusing the honest READ from a `=` binding on the way. Its last line was
*"the blocker is one question: what C type does a lend's temporary have?"*

**The answer: the lend's own.** `emit/field_lend.hero` now answers which values
of a function are const lends (`const_lends`, keyed by value id), and the
prologue declares those `const void *` and everything else as before. The fact
it reads is the IR's: `SlotKind.local_slot` gained `mutable: bool`, carried from
`resolved.Local.mutable` at the three lowering sites through one helper,
`build.local_kind`, and a by-value parameter was already `param_slot(mutable:
false)`. The language's own *built with every field, named* rule made every
construction site a compile error, so the six test fixtures could not be missed.

## The diagnostic, and why it reads clang's line rather than writing a marker

clang's refusal lands at the **call's** `#line` — the probe casts a `ptr`
argument to `(void *)` and so never fails on a lend — and no `_Static_assert` can
carry a message for an implicit conversion at an argument: C11 has no form that
names the k-th parameter's type. `cli/pointee.hero` gets that text from an AST
dump of `extern __typeof__(name)`, which no macro-named function survives, so a
marker built from the dump would leave the commonest libc shapes falling through
to exit 2. `emit/ffi_lend.hero` therefore gates on the phrase `ffi_mutable` has
gated on since panel 058 — `discards qualifiers` — at a line where the IR holds
an extern call whose argument is a lend from an immutable root, and points at
**the lend**, not at the declaration:

```
error[field_lend_written]: `t.nsap.ptr()` lends a field of `t` to `slot_fill`, and the
  header declares that parameter `void *` — C may write through it, and `t` is not a `@` name
  at tests/golden/fixedbugs/ffi-a-lent-field-c-would-write.hero:33:18
  note: §4.4: only a declared `@` name can be mutated, and a C write is a mutation.
        Declare `t` with `@` — a `@` cell, or a `@` parameter — and the bytes C writes
        land where this program reads them back. To only READ the field, lend it to a
        parameter the header declares `const`
```

No `Fix`, in `lend_errors.hero`'s own tradition: whether the author meant to write
or meant to read through a parameter the header should have marked `const` is
theirs to say, and the note names both roads. The two qualifier readers cannot
claim one line: `ffi_mutable` asks for an extern declared there and a call site
has none.

## The repair, attacked at the shapes beside it (CL-061)

Every row run on `arm64-apple-darwin25.6.0` with the compiler built from
`selfhost/` at this step, against one header with a `const void *` reader and a
`void *` writer:

| root of the lend | to the reader | to the writer |
|---|---|---|
| `t = make()` | **16**, exit 0 | **refused**, exit 1 — the defect's own reproducer, 72 → 65 before |
| by-value parameter `s: Slot` | **16**, exit 0 | **refused** at the lend inside the callee |
| `@` cell `u: Slot @ make()` | 16 | **36**, written and read back, exit 0 |
| `@` parameter `@s: Slot` | — | **36** in the caller after copy-out, exit 0 |
| nested field `o.inner.nsap` of a `=` | **8**, exit 0 | **refused**, message names `o` |
| nested field of a `@` cell | — | **36**, exit 0 |
| a `match` payload `.ok s` | **16**, exit 0 | **refused** |
| a `for` variable | **16 16**, exit 0 | **refused** |
| two lends on one line, one to each | — | **refused once**, at the first lend, naming the `void *` parameter |

`--sanitize` on the legal golden: exit 0. `--emit-c` on the refused program:
exit 1 and **no artifact written**, which is what `cli/artifact.hero` owes since
defect 048. The compiler's own tests: **662**, all passed, three of them new in
`emit/ffi_lend.hero`.

## The shape no seat named, and the clause it cost

A lend from a `=` binding handed to a **Heroes function taking `ptr`** —
`wrap(p: t.nsap.ptr(), n: 8)` with `wrap` calling C — compiled and ran at exit 0
before this step, and under route H it failed **inside clang at the wrapper's
call**, exit 2, `passing 'const void *' to parameter of type 'void *'`: a
Heroes `ptr` parameter is a plain `void *`, and no reader owned that line. The
same through a function value. Measured before the clause below existed, both.

**The clause: a `ptr` lend stands only as an argument of an `extern` call**
(`check/lending.hero`, `field_lend_needs_a_header`). The two questions about a
lend — does C write through it, how far does it read — are answered by the
header and nowhere else, and a function of this program has none to ask; from a
`@` root the lend used to reach C through such a wrapper with nothing checked
about it, which is the extent question route C puts on the group's parameter,
where only an `extern` can carry it. The wrapper §4.19 prescribes as the route
out of a module takes the RECORD and lends inside itself — `total(s: Slot)` in
the run golden — and stays legal. The `cstr` lend keeps panel 122's wider rule;
this clause is the field's own. `tests/golden/check/ffi-a-lent-field-needs-a-place.hero`
gains the fourth refused shape, and its `.expected` was rewritten by hand against
the four `#~` annotations after every line number in it moved.

## What did not change

The four `.ptr()` call sites this repository holds
(`tests/golden/run/ffi-a-byte-field-crosses-to-c.hero`) are all on a `@` cell and
emit `(void *)` exactly as before; the blessed emission is byte-identical. The
`spec` is untouched: § 5 is TRUE again for every program the compiler accepts,
which is the *D1-with-nothing* case panel 166's spec-warden priced at zero.

## Cases

- `tests/golden/run/ffi-a-lent-field-reads-through-const.hero` — the legal half,
  and the shape the first landing broke: a `=` binding and a by-value parameter
  lent to a reader, beside a `@` cell lent to a writer. Five lines of output.
- `tests/golden/fixedbugs/ffi-a-lent-field-c-would-write.hero` — the refused
  shape, annotated `#~ field_lend_written` in its source, with a `surface` row
  asserting the sentence and that neither `internal error` nor `discards
  qualifiers` reaches the author.

## What panel 166 left owed that this does not close

**Defect 066**, the lifetime rule, which that sitting filed and did not price;
and **defect 063**, route C, which is the next step. The engineer's prediction
about route C's commit shape is scored there, not here.
