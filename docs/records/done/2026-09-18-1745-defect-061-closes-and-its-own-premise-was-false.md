- [x] **061 — a fixed-array field cannot be passed to C at all** | `strlen(u.sysname)` against `function strlen(s: cstr) -> u64` is `type_mismatch`, because Heroes gives a fixed array no array-to-pointer decay, so a bound `char[N]` field only LOOKS bound | `selfhost/check/lending.hero` · `selfhost/emit/field_lend.hero` · `spec § 13`

    **Origin:** panel 162's ffi-pragmatist, 2026-09-18, measured while answering
    a question about reading. Closed the same day by panel 164.

    **CORRECTION, 2026-09-18, and it is the largest thing this defect produced.**
    The entry as filed said, in bold, *"No Heroes-side shim can route around
    it."* That is false, and three seats measured it independently — the
    spec-warden, the ffi-pragmatist who filed it, and the completeness critic:

    ```
    slot_len(t.name.validated_bytes().must().cstr())
      terminated i8[8] ("Hi\0…")                     ->  2    exit 0
      UNTERMINATED i8[8] ("fullest!"), 8 'A's after   ->  8    exit 0
    ```

    Eight, not seventeen. The same struct in C, `slot_len(full.name)`, under
    `-Weverything -fsanitize=address,undefined`, prints **17** and reads into the
    next field with every diagnostic silent. The true sentence is narrower: no
    Heroes-side shim lends the field's **ADDRESS**, and the route that works
    **copies**. The whole sitting was convened on the wider wording.

    **SECOND CORRECTION, same day, by panel 164's completeness critic.** The
    entry's body said *"That sitting widens `slice`, `validated` and `repeat` so
    a field can be READ"*. Measured: `t.name.slice(from: 0, to: 2)` is
    `error[bad_operand]: slice takes str or [T], found i8[8]`. The sentence was
    never run. The same falsehood was in `check/lending.hero`'s own comment and
    in `emit/bytes_text.hero`'s module doc; both are corrected in this commit.

    **THIRD CORRECTION.** The headline program was drawn from the SAFE class.
    POSIX requires `struct utsname`'s members to be terminated — *"the data
    stored in them shall be terminated by a null byte"* — so `strlen(u.sysname)`
    is the case where the decay would have been sound, and it works today. Panel
    164's historian found it. The defect's emblem should have come from the 37
    of 50 fields with no terminator, not from the 13 with one.

    **What was actually broken, and it is what closed.** A field's ADDRESS could
    not reach C in either direction. That left two real classes unserved:

    - a **binary** field with its length beside it. `arpa/inet.h`'s
      `inet_nsap_ntoa(int __binlen, const unsigned char *_LIBC_COUNT(__binlen),
      char *)` states the extent in the call and never as a terminator. The only
      door the language had gave a **silently wrong answer at exit 0** — the
      program compiled, ran, and printed `0x47` where the C prints
      `0x47.0005.80FF.0000.01`, because `validated_bytes` stopped at the first
      zero.
    - the **write** direction. **50 of 141** pointer parameters across 16 real
      headers are non-`const` — C fills them. `getcwd(@buf: ptr, size: u64)`
      binds and `heroes check` passes it, and nothing in the language could
      produce the argument. It was on this defect's own unrun list and stayed
      there until the critic ran it.

    **The repair is panel 164's resolution: one type rule, `f.ptr()`.** It lends
    a binding's fixed byte field to a `ptr` parameter the call gives the extent
    to, and C may write back through it. It answers `ptr` and never `cstr`,
    because a `cstr` is a promise of a terminating zero that a header's field
    does not make — design.md §1.11 lists that promise among the decisions not to
    revisit, and it is load-bearing three times over.

    **The defect inside the repair, and it is worth more than the feature.** The
    first implementation rendered the lend with `emit/storageless.hero`'s
    `fixed_text`, which is correct for `validated_bytes` and wrong here: the IR
    loads a record into a fresh temporary before reading a field of it, so the
    emitted C was `(void *)(t13.nsap)` — the address of a **COPY**. Reading
    through it is right by accident, because the copy holds the same bytes.
    Writing through it is **lost in silence**: the probe filled eight bytes, C
    returned, and the program's record was unchanged, at exit 0. Measured:

    ```
    read: 16          after C wrote: 16      <- the copy
    read: 16          after C wrote: 36      <- the place
    ```

    It was found only because the probe made C write and then **read back**,
    rather than stopping at *it compiles and runs*. `emit/field_lend.hero` now
    follows the load to the cell, and `emit/unread.hero` learned that a `ptr`
    reads a PLACE and not its operand, which is that module's whole subject.

    **Two refusals came with it**, each with its own diagnostic and each made to
    fire by a test: a lend kept in a binding is `field_lend_escapes` (panel 122
    R2's position rule, inherited free), and a lend from a value nothing holds is
    `field_lend_needs_a_place` — which before the clause existed reached the
    emitter and aborted at run time saying *this is a compiler bug*.

    **A dynamic `[u8]` is refused deliberately**, not by omission: it is a
    runtime-owned array with its own lifetime and refcount, and lending C its
    interior is a question panel 164 did not put to its seats.

    **Measured at the close**: compiler's own tests **659**, `run` 127,
    `annotations` 166, `unsupported` 15, `canonical` 2, `spec` 20, `grammar` 7,
    `special` 10, `layout` 2. Spec **8106** real on `claude-opus-5`, digest
    `3c065c560426eb07`, ledger row 83. The seed regenerated and the fixpoint
    verified byte-identical.

    **What did NOT close, and it is named rather than folded in.** The extent
    declared on the PARAMETER — `function arr_len(s: i8[8])`, which is how C
    spells it — is panel 164's route 6, and it is the only route where the
    compiler CHECKS the extent instead of trusting the author or the callee. Its
    whole value is a number nobody has measured: how many real headers spell a
    parameter as an array. It is scheduled, not filed, because nothing is broken.
