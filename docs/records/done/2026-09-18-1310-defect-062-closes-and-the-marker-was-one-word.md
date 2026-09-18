- [x] **062 — a WRONG FIELD TYPE is reported as a missing field, and the name is wrong too** | declaring `sysname: cstr` against `char[256]` gave `ffi_incomplete_record: does not name nodename` with `nodename` declared on the line below, and no type error at all | `selfhost/emit/extern_field.hero` · `selfhost/emit/ffi.hero` · `tests/golden/unsupported/ffi-a-cstr-field-blames-the-field.hero`

    **Origin:** panel 162's compiler-engineer, 2026-09-18; narrowed at
    M-readable-bytes' close the same day, because the entry as first written
    was wrong about when it fires, and repaired an hour after that.

    **The cause was ONE WORD.** `emit/extern_field.hero` has six branches, each
    writing a marker clang echoes back on failure — `heroes-ffi-field <record>
    <member>` — and `emit/ffi_field.field_type` reads exactly two words out of
    it. **Five branches wrote the Heroes record's name. The `cstr` branch wrote
    `c_type`**, which for a tagged record is two words, `struct utsname`. So the
    reader took `struct` for the record and `utsname` for the member, found no
    such record, and returned nothing. clang had said the true thing twice —
    `256 == 8` on both fields, read from `build/tu-…/clang-stderr.txt` — and the
    only diagnostic that survived was the completeness probe's.

    **And the probe's message was wrong for a reason measured separately.** It
    is a positional initialiser list: one `0` per declared field, `{0}` for a
    declared array. Declare `cstr` where the header has `char[N]` and the probe
    writes a scalar where C wants an array; C's brace-elision rule then consumes
    that `0` and the next into the array's first two elements, and clang reports
    the NEXT member missing. Measured with a three-line C file: `struct tagged
    v = {0,0}` over `{char name[8]; int32_t id;}` gives *missing field 'id'
    initializer*. **With the types correct the probe writes `{{0},0}` and is
    right**, which is why `tests/golden/run/ffi-a-char-array-member.hero` was
    never affected and why the first filing of this defect blamed the wrong
    thing.

    **Two repairs, both in the direction §4.17 asks.** The `cstr` branch writes
    `hero_name` like the other five, so the type error reaches the author on the
    field they must change. And `emit/ffi.hero`'s `explain()` does not add a
    completeness diagnostic for a record that already has an `ffi_field_type`
    one — the count is unreliable the moment one type is wrong, so it waits for
    the author to fix the type and run again, which is the loud direction.

    **Before and after, on the reproducer:**

    ```
    before   error[ffi_incomplete_record]: `Utsname` does not name `nodename`
               at u.hero:2:12, caret on `Utsname`        (nodename is on line 4)
    after    error[ffi_field_type]: `Utsname.sysname` is not `cstr` in `sys/utsname.h`
               at u.hero:3:9, caret on `sysname`
             error[ffi_field_type]: `Utsname.nodename` is not `cstr` in `sys/utsname.h`
               at u.hero:4:9, caret on `nodename`
    ```

    **The shape beside it was run and holds** (CL-061): two of five fields
    declared at the CORRECT type still gives `ffi_incomplete_record` naming
    `release`, the genuinely undeclared third field. The guard does not reach
    it, because no type error is in hand.

    **The golden case is tagged on purpose.** An untagged `typedef struct {…}`
    gives a one-word `c_type`, so the broken marker parsed by accident and the
    defect did not fire. The first draft used `char name[8]` and the type
    assertion did not fire either — `8 == 8`, an eight-byte array happening to
    be a pointer's size — so it is `char name[16]`. Both choices are in the
    case's own comment, because a case that passes for the wrong reason is
    worse than none.

    **The verification:** `unsupported` 14 → **15 passed, 0 failed**;
    `annotations` 164 → **165**; `canonical` 2; the compiler's own tests
    **655, all passed**; the seed regenerated and the fixpoint verified
    byte-identical.
