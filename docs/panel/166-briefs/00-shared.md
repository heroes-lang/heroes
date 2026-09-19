# Panel 166 — shared brief

**The question.** `f.ptr()` lends a fixed byte field's address to C. It shipped
at `ef7b013b` on 2026-09-18 and it has two holes, both measured, both at exit 0:

- **defect 063** — the extent is stated by the CALL and nothing checks it, so C
  reads and **writes** past the field;
- **defect 065** — the binding may be declared `=`, and C writes into it anyway,
  which falsifies two sentences of the specification.

**What should a `ptr` lend of a field promise, who checks the extent, and who
marks the write?**

This sitting exists because panel 165 refused route 6 and found these underneath
it. Panel 165's own resolution named a repair — *"the extent argument is checked
against the field's own extent"* — and that sentence was **corrected the same
day** as not implementable: nothing declares which argument is the extent.

---

## Everything below was RUN while this brief was being written, 2026-09-19

### 1. Defect 063 — the overstated extent corrupts memory

```
s: Sl @ sl_make()                       # struct sl { char name[8]; int id; }
print(to_str(s.id.to_i64().must()))     # 7
sl_fill(p: s.name.ptr(), n: 64)         # C writes 64 bytes into an 8-byte field
print(to_str(s.id.to_i64().must()))     # 1094795585  ==  0x41414141  == "AAAA"
                                        # exit 0, no diagnostic anywhere
```

Read direction, same shape: `sum_n(p: t.name.ptr(), n: 4096)` sums 4096 bytes and
returns, exit 0. Under `./heroes run --sanitize`:

```
AddressSanitizer: stack-buffer-overflow    READ of size 1
This frame has 1 object(s): [32, 44) 'h0_t'
  <== Memory access at offset 44 overflows this variable
```

**design.md §1.12 says a Heroes program must not corrupt memory**, and CLAUDE.md
§ Precedence puts that at rank 3 — above elegance, token cost, ergonomics,
compiler size and speed.

### 2. Defect 065 — C writes into an immutable binding

```
t = sl_make()                                # `=`, not `@`
print(to_str(t.name[0].to_i64().must()))     # 72
sl_fill(p: t.name.ptr(), n: 8)               # no @ on the binding,
print(to_str(t.name[0].to_i64().must()))     # none on the parameter,
                                             # none at the call site
before: 72
after : 65
```

`spec § 5`: *"`=` binds once, forever"*, and *"only a declared `@` name can be
mutated"*. `spec § 3`: *"Every value behaves as an independent copy."* Both are
false for this program.

**And the shipped golden demonstrates the write as correct behaviour** —
`tests/golden/run/ffi-a-byte-field-crosses-to-c.hero` does exactly this on a `@`
binding, which is why nobody saw the `=` case.

### 3. Panel 164's resolution 2 does not exist in the compiler

That sitting adopted: *"`@field` reaches a `@`-marked `ptr` parameter so C can
fill a field"*. Run today, against `function sl_fill(@p: ptr, n: i64)`:

```
sl_fill(p: @s.name, n: 8)
  error[type_mismatch]: expected `ptr`, found `i8[8]`

sl_fill(p: @s.name.ptr(), n: 8)
  error[not_a_place]: only a name, a field or an element can be passed as `@`
```

The first is **the identical error panel 164 quoted** when it wrote *"only the
type rule is missing."* Found by panel 165's completeness critic and verified
twice since. **A ratified resolution that never landed is why this sitting cannot
assume the write direction is designed.**

### 4. What the language offers instead, measured

```
s.name.len()
  error[bad_operand]: `len` takes `str`, `[T]` or `{K: V}`, found `i8[8]`

name: i8[SLOT_NAME_LEN]          # SLOT_NAME_LEN is an extern `constant`
  error[expected_array_length]: expected the array's length after `[`, found a
    name (`SLOT_NAME_LEN`) — a C array member is `i32[4]`, and the length is
    part of the type (§4.19)
```

So the author has **no way to name the extent** except an integer literal, and no
way to ask a field how long it is.

**But C can check a named extent**, which panel 165's critic found and this brief
re-ran:

```c
_Static_assert(sizeof(((struct sl2 *)0)->name) == SL_NAME_LEN, "...");  /* passes */
_Static_assert(sizeof(((struct sl2 *)0)->name) == 20, "...");           /* fails:
                                             expression evaluates to '8 == 20' */
```

The macro expands in the emitted C against the real header. **The wall is the
Heroes grammar, not the semantics.**

### 5. The denominator

```sh
grep -rhn "function .*: ptr" --include='*.hero' selfhost/ examples/ tests/ | wc -l
66
grep -rhoE "function [a-z_]+\([^)]*: ptr[^)]*\)" --include='*.hero' selfhost/ examples/ tests/ \
  | grep -cE "(n|len|size|count|bytes|nbytes|cap): (i64|u64|i32|u32)"
18
```

**66** `extern` functions in this repository take a `ptr`; **18** pair it with a
sibling that looks like a length. That is the population a counted-parameter rule
would serve, and it is a *shape* count from a regex — a seat that needs the exact
number should read the eighteen.

### 6. Why the extent cannot be checked as panel 165 first wrote it

`sum_n(p: ptr, n: i64)` declares two independent parameters. **Nothing relates
them.** `selfhost/check/lend_types.hero` types `f.ptr()` by asking only whether
the receiver is a fixed byte run; the extent never appears. So *"check the extent
argument"* has no argument to find until something declares which one it is.

---

## The routes on the table. Add to them — panel 165's critic found two nobody had

- **A — refuse `.ptr()` from an immutable binding.** Closes 065's half. Checkable
  today: the compiler knows `=` from `@`. Cost: reading a field of an immutable
  record now needs `@`, which may be over-refusal.
- **B — implement panel 164's resolution 2**: a `@`-marked `ptr` parameter takes
  `@field`, and an unmarked one refuses a field C could write. Closes 065 at the
  declaration rather than at the binding.
- **C — route 13, the counted parameter**: the `extern` declares which sibling
  carries the extent, e.g. `function sl_fill(@p: ptr counted_by n, n: i64)`, and
  the compiler checks it against the field's extent when both are known. The only
  route where 063's number can be checked at all. Unpriced.
- **D — `len()` on a fixed field**, so the honest call needs no literal. Removes
  the incentive; checks nothing. A `spec § 11` change.
- **E — a named extent**, `i8[SL_NAME_LEN]`. Panel 165 queued this as route 12
  for the *parameter* position; it is listed here because a field declared with a
  named extent is the same grammar change and would make D and C portable.

  **Stated exactly, because the suggestive version is wrong.**
  `selfhost/emit/extern_field.hero:92-115` already emits a `_Static_assert`
  comparing the field's size against its length — but the length it writes is the
  **literal Heroes holds in the type** (`fixed_len`), not a macro name. So the
  mechanism that would check a named extent exists and the *number* it writes
  does not carry a name. Route E therefore needs the type to carry the name as
  far as the emitter, and a seat should price that rather than assume it is free.
- **F — refuse the `ptr` lend entirely** and go back to the copy
  (`validated_bytes`). Closes both defects, removes a capability shipped two days
  ago, and leaves `getcwd` unwritable — the gap panel 164 opened it for.
- **G — write the hole into the specification** and check nothing. Cheapest,
  closes neither defect, and makes `spec § 3` and `§ 5` true again by narrowing
  them.

## Working rules for this sitting

- **Build in a copy.** `cp -r` the tree to your scratchpad, `rm -rf target
  build`, work there. The seed builds in about 3 s:
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
- **The `heroes` binary is `.gitignore`d and can be older than the tree.**
  Rebuild before trusting it; a stale one cost this project two full-net runs on
  2026-09-18 and reported the *documents* as stale.
- **Never `archive/bootstrap-rs/`.** Nothing builds it. Panel 165's own brief was
  caught counting five declarations out of it.
- **Contradict this brief where you can run something that refutes it.** Panel
  165 corrected six things in its briefs and the coordinator wrote every one.
