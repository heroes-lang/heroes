# Panel 162 — compiler-engineer

Every number below was produced by a command run on 2026-09-18 in a scratchpad
copy of the tree (`cp -r`, `rm -rf build`, seed built with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a few seconds).
The command is named beside each one. `archive/bootstrap-rs/` was never read or
measured (CL-026). Nothing here is carried from the previous run of this seat,
which died before writing; every claim it might have made was re-measured.

- **verdict**: object
- **section**: design.md §1.7 ("Core plus elaboration — it determines the size
  of your compiler. Anything in the core must be implemented in the type checker
  *and* the lowering *and* the backend", design.md:416), with Part 5's seven
  constructs and the built-in inventory's *Tier 1 in C, Tier 2 in Heroes*
  (design.md:2525-2528) as the test I apply.
- **needed_for_self_hosting**: no. The shared brief measured the compiler-need
  branch closed and I did not reopen it. `selfhost/inventory.hero:73` reads
  `Builtin(name: "read_file", tier: .heroes)` — the compiler reads files through
  the library, and the library already works.
- **implementation_cost**: ~75-110 lines, **zero lowering lines** and no new
  type. Full table in § implementation_cost below, every row with its `wc -l`.
- **argument**: The briefs ask the question one level too high. The conversion is
  already built — `hero_str_try_from_cstr`, `runtime/parts/str.c:299-316`, panel
  089 — and `str` already carries a length (`heroes_runtime.h:73-76`), so a byte
  run of known extent is already the right shape. `read_file` already fixes the
  rule for bad UTF-8: `.err`, `not_text`, measured. What is missing is not a
  conversion but a way to name a fixed-array field's **extent**: `@` here means
  mutable out-parameter, not address-of, measured, so panel 089's library
  precedent does **not** extend and no brief prices what does. Two further gaps
  nobody listed: the inbound byte is `i8` and `spec § 10`'s byte is `u8`; and
  `ffi_incomplete_record` names a field that is present.

---

## 1. The answer already exists, and the rule is already chosen

**`read_file` on invalid UTF-8 answers `.err` with `code == "not_text"`.
Measured, not reasoned.** `printf 'abc\xff\xfe\x80def' > t/bad.bin` (verified
with `xxd`: `6162 63ff fe80 6465 66`), then `./heroes run t/rf.hero` over three
files:

```
ERR code=not_text
OK len=10
ERR code=file_not_found
```

No abort, no replacement character, no lossy decode, no truncation. **The rule
this language already has is: invalid UTF-8 arriving from C is a recoverable
failure value carrying a stable snake_case code.** Any route that invents a
different answer is spending tokens on a contradiction.

Where it is decided, three places, each cited:

- `runtime/parts/os.c:259-263` — the `hero_utf8_valid` pre-check inside
  `hero_file_read`, setting `HERO_OS_NOT_TEXT`. Its comment names panel 087 and
  defect 002, and states the doctrine: *"the repair is a pre-check at the
  caller, never a weaker conversion."*
- `runtime/parts/str.c:270` — `hero_str_from_bytes` **panics** on ill-formed
  UTF-8, and `str.c:295-298` says that abort is deliberately not weakened
  because `hero_str_chars` rests on it for the language-wide well-formedness
  invariant.
- `runtime/parts/str.c:299-316` — **the fallible conversion already exists**:
  `hero_str_try_from_cstr(const char *p, int64_t *status)`, panel 089, written
  *beside* `from_bytes` rather than on top of it, measured 0.87x the infallible
  one because it walks the bytes once instead of twice.

`runtime/parts/str.c:304` is `size_t n = strlen(p);`. **That single line is the
entire difference between what exists and what this sitting wants.** The inbound
door is the same door with a different caller. It is not a new one.

`wc -l` → `runtime/parts/str.c` 408, `runtime/parts/os.c` 347.

## 2. A `str` carries a length **and** a NUL, so the byte run is already the right shape

`runtime/heroes_runtime.h:73-76`:

```c
typedef struct {
    const char *ptr; /* NUL-terminated; NULL only for the non-value */
    int64_t len;     /* bytes, excluding the NUL */
} HeroStr;
```

`runtime/parts/str.c:46`, `hero_str_alloc(int64_t len)`, allocates the header
plus `len + 1`. A byte run of **known length** therefore needs one validity walk
and one `memcpy` — literally `str.c:313-315`. The brief's worry that *a C
`char[N]` need not be NUL-terminated* is answered by the representation and
needs no new rule: the caller passes `N`, or a scan for the first `0` before the
call. Neither is a construct.

## 3. The program in the brief already runs. Only two walls are real

**Measured, end to end.** Generated a 256-element literal with `python3` (768
characters, counted) and ran the brief's program with it:

```
first byte = 68
run length = 6
```

68 is `'D'`; the run is `Darwin`. So **`uname(@u)` writes the struct, `@` on a
`extern record` out-parameter works today, and `u.sysname[i]` reads each byte
today.** The brief's framing — *"the program that cannot be written"* — is
narrower than it reads. Three of its four walls are one wall.

What actually fails, each measured separately:

| attempt | answer |
|---|---|
| `Utsname(sysname: [0], …)` used | `error[fixed_array_length]`: *holds exactly 256, and this literal has 1* |
| `u.sysname.len()` | `error[bad_operand]`: *`len` takes `str`, `[T]` or `{K: V}`, found `i8[256]`* |
| `u.sysname[i]` | **works** |
| `_ = uname(@u).to_i64().must()` | **works** |

So the two walls are **the literal** and **the conversion**. Everything between
them is already built.

## 4. The seam nobody listed: the bytes are `i8`, and the spec's byte is `u8`

**This is the highest-value thing I return.** Declaring the field `u8[256]`
against `char sysname[256]` is refused:

```
error[ffi_field_type]: `Utsname.sysname` is not `u8[256]` in `sys/utsname.h`
  — clang read the header's struct and the field disagrees
  note: a group's `record` IS the header's struct (§4.19), so every field is at
  the header's own width and sign
```

`i8[256]` builds clean (`wrote t/r4`). So a C `char[N]` field **must** be
`i8[N]` in Heroes. And `spec/heroes-spec.md:270` fixes the outbound convention:
*"`s[i]` yields a `u8`."*

**The pair the shared brief wants to read symmetrically cannot.** Outbound, a
byte is `u8`; inbound from a `char[N]`, a byte is `i8`. Every route in the brief
is written as if the source were a `[u8]`, and on the one struct the brief
builds its case around, it is not. An inbound name taking `[u8]` would not
accept `u.sysname` at all; one taking `i8` would make `spec § 10`'s pair read
wrong in the other direction.

This is not hypothetical and it is not old. The three commits at the head of
this tree are `M-arm-platform`, whose step 2 subject is *"C's plain `char` is
signed on every leg, and defects 058 and 059 close on one string."* **This
project paid for the signedness of `char` in the milestone that closed
immediately before this sitting**, in a different place, and no route here names
it. A resolution that does not decide the sign is a resolution that will be
re-opened by the first `unsigned char[N]` field somebody binds.

## 5. Route 2 is not blocked by the rule the brief names, and the compiler lies about why

**The brief's premise is false as written.** It says *"`spec § 13` currently
forbids a record holding a `cstr`, so this moves an existing rule."* Measured,
that rule is a **checker** rule and it binds only records **outside** a group:
`no_cstr_in_a_record`, `selfhost/check/lending.hero:200`, panel 122 R4. The live
test at `selfhost/check/lending.hero:282` asserts a `cstr` field **inside** an
`extern` group yields **zero** diagnostics; `selfhost/check/ffi.hero:219` lists
`cstr` among a group field's legal spellings; `spec/heroes-spec.md:350` says the
same. So **nothing has to move.** Route 2 costs zero rule changes and is still
dead, for a reason nobody listed:

C's type system refuses it — `char sysname[256]` is not `const char *` — and
**the compiler's report of that refusal names the wrong field**:

```
error[ffi_incomplete_record]: `Utsname` does not name `nodename`, and
`sys/utsname.h` says the struct has it — a group's `record` IS the header's
struct, so a field left out is a field C fills with zero
```

`nodename` is named, on the line directly above `sysname`. All five fields are
named. The mechanism is `selfhost/emit/ffi_record.hero:38-60`, which parses
clang's `-Wmissing-field-initializers` wording and maps it back through `#line`;
a pointer initialiser against an array member makes clang say *missing field*,
and the compiler forwards that word verbatim. **This is a defect independent of
this sitting** and I report it as one: a diagnostic that blames the program for
an omission it did not make, in the exact spot a reader would try route 2 first.

`wc -l` → `selfhost/check/lending.hero` 282, `selfhost/check/ffi.hero` 384,
`selfhost/emit/ffi_record.hero` 260.

## 6. The ceiling argument: what a builtin actually costs here, measured

design.md:416 defines core as *"implemented in the type checker **and** the
lowering **and** the backend."* I checked the lowering half rather than assuming
it. **A builtin costs zero lowering lines.** `selfhost/ir/flatten.hero:523-544`
lowers any builtin to `.call(callee: .builtin_fn(at: bt.at))`, indexed off
`inventory.table()[bt.at]`, and `selfhost/ir/print.hero:417` prints it the same
generic way. There is **no per-builtin IR node**. So the honest §1.7 grading is:

| route | checker | lowering | backend | new type |
|---|---|---|---|---|
| 1, a new builtin name | yes | **0 lines** | yes | no |
| 2, a `cstr` view of `char[N]` | — | — | — | dead (§5) |
| 3, a slice-shaped answer | yes | yes | yes | **yes** |
| 4, refuse | 0 | 0 | 0 | no |

`wc -l` → `selfhost/inventory.hero` 201, `selfhost/check/builtins.hero` 545,
`selfhost/emit/builtins.hero` 316, `selfhost/emit/convert.hero` 245.

**Route 1 is two-thirds of core, not three-thirds**, and that is a real
difference the briefs do not state. **Route 3 is all three plus a type**, and it
is the one I would veto — stated as a refusal rather than a price: a type the
checker, the descriptors, the ownership pass, the emitter, the formatter and the
grammar must all handle is not funded by 24 `char[N]` fields across six headers
and **zero** programs in `examples/`. The language reached self-hosting without
one.

## 7. My preferred route is dead, and I measured it dead myself

I wanted the answer to be **library growth, not language growth**, because this
project already decided it that way on a measurement:
`selfhost/library_source.hero:192` is `validated`, a **`.hero` function in the
embedded library**, not a builtin. Panel 089's comment at
`selfhost/library_source.hero:180-183` is verbatim:

> the built-in route costs a second composition arm in the emitter and this
> route costs zero backend lines.

and `:277`: *"Panel 089 added `validated`, the first legitimate growth of the
library since the archive."* design.md:2525-2528 is the frame — *Tier 1 in C,
Tier 2 in Heroes* — and `selfhost/inventory.hero:73` shows `read_file` itself at
`tier: .heroes`.

**It does not work here, and the reason is the finding.** A library function
would need the field's address. There is none:

```
strlen(s: u.sysname)   error[type_mismatch]: expected `cstr`, found `i8[256]`
strlen(s: @u.sysname)  error[not_mutable]: `u` is a parameter without `@` …
```

The second answer is the load-bearing one. **`@` in this language means
*mutable out-parameter*, not *address of*** — the checker answered a mutability
question, not a type question. So there is no address-of operator, by design
(§4.8's copy-in/copy-out), and Heroes has no generic over an array length for
the by-value alternative.

**Therefore the sitting cannot buy this in the library, and that is the sentence
the synthesis needs.** Panel 089's precedent — the one every seat will reach for
— **does not extend to this case**, and a resolution that cites it without §7's
measurement will be citing a precedent that does not hold.

What this sitting is actually deciding is **whether a fixed-array field is a
readable extent**, not what the conversion is called. The conversion is already
built (`runtime/parts/str.c:299-316`); a runtime entry point taking `(p, len,
status)` is ~14 lines beside it and 1 header line. **Every remaining cost is the
cost of getting `p` and `len` out of the field**, and no brief prices that.

## implementation_cost

| what | file | lines today (`wc -l`) | added |
|---|---|---|---|
| `hero_str_try_from_bytes(p, len, status)` beside `:299` | `runtime/parts/str.c` | 408 | **~14** (existing fn is 18 lines, 299-316, minus the `strlen` pair 304-306) |
| its declaration beside `:228` | `runtime/heroes_runtime.h` | — | **1** |
| the builtin's table row | `selfhost/inventory.hero` | 201 | **~2** |
| its type rule (incl. the `i8[N]`/`u8[N]` arm and the sign decision) | `selfhost/check/builtins.hero` | 545 | **~25-40** |
| its C spelling, `&r.field` and `N` | `selfhost/emit/builtins.hero` | 316 | **~15-25** |
| lowering | `selfhost/ir/flatten.hero` | — | **0**, measured (§6) |
| zero default for an `extern` record (build half, §8) | `selfhost/check/walk.hero`, `selfhost/ffi_errors.hero` | — | **~15-30** |

**Total ~75-110 lines, no new type, no new IR node, zero lowering.** Against
Pascal-P4's ~4000 lines it does not move the ceiling and it still fits one
person. Route 3 does not fit this table at all, because a type has no row: it has
a column in every file that switches on a type.

## 8. The build half

**The mechanism already exists and is general.** `zero_of(names, c, ty, mutable)`
at `selfhost/emit/assert_spelling.hero:170` already emits a typed zero for every
shape; its own test at `:277-279` asserts `(HeroStr){0}` for a `str` and
`(Color){0}` for a struct. C's `{0}` zero-initialises a `char[256]` member by the
standard, so **the emitter needs no new spelling** for a zeroed `Utsname`. Panel
021's refcounted-slot zero-initialiser (`.claude/rules/generated-c.md`
§ Arithmetic and memory) is the same mechanism, already ratified as *the one
exception to the no-initialisation rule*. **So yes: the panel-021 mechanism the
brief asks about is already there and already blessed.**

The wall is **one checker call site**: `selfhost/check/walk.hero:616` raises
`fixed_array_length`, defined at `selfhost/ffi_errors.hero:136`. A zero default
for an `extern` record is a **checker** change of ~15-30 lines plus its
diagnostic, and **zero emitter lines**. That is sugar by §1.7's test: erased
before the IR, and the backend never learns it happened. **The build half is the
cheap half and it is not where this sitting should spend its argument.**

On exempting an `@` out-parameter from §5's *all bindings are initialised*: **I
did not measure this and I will not assert it.** What I name is the hole, because
it is nameable — `uname(@u)` writes `u` only if it returns 0, and nothing in the
type system ties the write to the return code. An exemption makes a slot readable
that C left untouched on the failure path, which is
`.claude/rules/c-boundary.md`'s own class and CLAUDE.md § Precedence rank 3. **A
zero default is strictly safer than an exemption and costs the same passes**, so
I see no argument for the exemption and the sitting should not spend on one.

## prediction

**At the M-readable-bytes close**, checkable with
`git diff --stat m-arm-platform..m-readable-bytes -- selfhost/ runtime/`:

1. **`selfhost/ir/` will not gain a new IR node kind** for this feature, because
   `flatten.hero:523-544` already dispatches builtins generically. If the
   milestone adds one, my §6 grading is wrong and route 1 was more expensive than
   I priced it.
2. The whole change will land **under 150 lines** across
   `runtime/parts/str.c`, `selfhost/inventory.hero`,
   `selfhost/check/builtins.hero`, `selfhost/emit/builtins.hero` and
   `selfhost/check/walk.hero`, with `selfhost/emit/descriptors.hero` **untouched**.
   A diff touching `descriptors.hero` means a type was added and my veto
   condition has fired.
3. **The resolution will not state whether the inbound byte is `i8` or `u8`**,
   because no brief names the question (§4). If the synthesis states it, this is
   falsified and I withdraw §4's complaint — which is the outcome I want.
4. `selfhost/emit/ffi_record.hero` will **still** report `ffi_incomplete_record`
   naming a field that is present, for the one-`cstr` `Utsname` case, unless this
   milestone repairs it (§5). Re-runnable in five commands.

## condition

**My first condition is already discharged, by me, against my own preference**:
§7 measured that the library route is unavailable, so I am not asking the sitting
to re-run it.

What moves me from **object** to **approve**:

- a resolution that (a) names a builtin rather than a type, (b) states the sign
  of the inbound byte in one sentence (§4), and (c) says in writing that the
  question is the *extent of a fixed-array field*, not the conversion — because
  the conversion is already built and §7 shows the briefs have the question one
  level too high.

What moves me to **veto**, stated as a refusal rather than a price:

- a slice **type**. A construct the checker, the descriptors, the ownership pass
  and the emitter must all handle fails §1.7's test at design.md:416, and the
  shared brief's own measurement — zero fixed byte arrays anywhere in
  `examples/` — shows nothing is waiting for it.

What would **not** move me: an ergonomics argument, a token-cost argument, or a
count of how awkward the 768-character literal is. CLAUDE.md § Precedence ranks
robustness above ergonomics, token cost and compiler size, and ranks none of them
below convenience.
