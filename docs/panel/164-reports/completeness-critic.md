# Panel 164 — completeness critic report (verbatim)

Everything below was run on 2026-09-18, on the critic's own copy at
`scratchpad/panel164-critic/copy`, seeded with `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`. The working tree was not written to. The critic
gives no verdict on the routes.

## 1. Routes nobody listed

**Four is not the option set. There are at least seven, and three of the missing ones are cheaper than two of the four on the table.**

### Route 5 — the terminator declared on the FIELD (`name: i8[16] terminated`)

The compiler seat and the historian converged on it independently. It is a real route, and it is the **most** expensive of the seven, for a reason neither seat named.

It would be the first clause in an `extern` block that **clang cannot check**. I measured that everything else is checked. A wrong width:

```
error[ffi_field_type]: `Slot.name` is not `i8[4]` in `slots.h` — clang read the header's struct and the field disagrees
note: a group's `record` IS the header's struct (§4.19), so every field is at the header's own width and sign
```

and the length is pinned inside panel 162's own marker at `selfhost/emit/extern_field.hero:161`, whose comment says it outright: *"The LENGTH needs no separate conjunct any more: it is inside every association as `(*)[N]`."*

**But "unverifiable" is not novel here, and the compiler seat's framing overstates it.** `consumes`, `acquires`, `borrows` and `owned <freer>` (spec § 13, lines 341-342 and 371-380) are already author claims about C's behaviour that no `_Static_assert` tests. So the precedent exists and the historian's Cyclone/Zig evidence is admissible.

**The real price is that it adds a FOURTH notion of the field's length to a language that already has three, all measured today:**

| where N is already decided | measured |
|---|---|
| construction demands exactly N | `error[fixed_array_length]: i8[256] holds exactly 256, and this literal has 1` (`check/walk.hero:616`) |
| the marker pins `(*)[N]` against the header | `emit/extern_field.hero:161` |
| the index bounds-check aborts at N | `panic: index out of range for a fixed array`, exit 134 |
| `validated_bytes` passes N to the runtime | emitted C: `hero_str_try_from_bytes((const char *)(t21.name), INT64_C(8), &hero_vb_status)` |

Cyclone's own rule, which the historian quotes, is that a `@zeroterm` array's size **includes** the terminator (`char s[4] @zeroterm = "bar"`). So `terminated` makes the usable run N−1 while all four sites above say N. Nothing reconciles them and nothing can check the reconciliation. Plus the full new-surface-form walk in `.claude/rules/diagnostics-and-goldens.md`.

### Route 6 — declare the extent on the PARAMETER, not the field

C spells this itself: `unsigned long arr_len(const char s[8])`. Refused today:

```
error[ffi_type]: `i8[8]` cannot cross the FFI boundary, and it is an `extern`'s parameter — a C header can declare a number, `bool`, `str`, `ptr`, `cstr`, a function type as a parameter, and a `record` declared in this same group (§4.19)
  5 |     function arr_len(s: i8[8]) -> u64
```

This is genuinely distinct from all four. **No new expression, no lend, no built-in, no position rule** — one widening of the §4.19 parameter list, and the checker's ordinary type identity does the matching (`i8[8]` field against `i8[8]` parameter, exact, both sides checked by clang). It is the only route of the seven where the compiler **checks** the extent instead of trusting the author or the callee. What it buys is bounded by how many real headers spell the parameter as an array, and **nobody has taken that measurement** — it is not in any brief or report.

### Route 7 — the write direction, through `@` on the field

Measured, against `function getcwd(@buf: ptr, size: u64) -> cstr`:

```
getcwd(buf: @b.name, size: 8)
  ->  error[type_mismatch]: expected `ptr`, found `i8[8]`      [ONE error, nothing else]
```

With the parameter **not** marked `@`, there are two errors — `marker_mismatch` first. So the grammar parses `@b.name` as an argument and the `@`-marker machinery already accepts a field there. **Only the type rule is missing, and it is the same rule route 3 needs.** The spec already reaches for it at line 121: *"`@` declares a mutable cell and re-binds it, **or a field or element inside one**."*

This matters because it is local in the ergonomist's exact sense: the `@` is on the line, at the argument, and it says C writes here.

## 2. Claims asserted and not measured, now settled

### 2a. "The shared brief's line 123" — the citation resolves to nothing; the measurement is true

```
wc -l docs/panel/164-briefs/00-shared.md          ->  102
grep -n "slice" docs/panel/164-briefs/*.md        ->  1 hit: historian.md:25, "std.mem.sliceTo"
grep -n "\[u8\]" docs/panel/164-briefs/*.md       ->  no hits
git status --porcelain docs/panel/164-briefs/     ->  ?? (untracked; no earlier version exists)
```

`spec/heroes-spec.md:123` is about `_`, not `slice`. **The claim the seat attributes to the brief is not in the brief, not in any brief, and not at that line of anything.**

**The measurement itself is correct**, replicated:

```
t.name.slice(from: 0, to: 2)
  ->  error[bad_operand]: `slice` takes `str` or `[T]`, found `i8[8]`
```

**And there IS a false sentence about `slice` — in a record, not a brief.** `docs/work/DEFECTS.md`, defect 061's body: *"That sitting widens `slice`, `validated` and `repeat` so a field can be READ."* My probe refutes it for a field. The seat half-remembered a real falsehood and cited the wrong document. The synthesis should carry the measurement, drop the attribution, and the correction belongs under defect 061.

### 2b. A fixed-array field IS indexable. `len` is not. The ergonomist's condition 3 fires

```
t.name[0]                          ->  72
while t.name[n] != 0  (n: i64 @ 0) ->  2          [hand-written scan, compiles and runs]
t.name.len()  /  len(t.name)       ->  error[bad_operand]: `len` takes `str`, `[T]` or `{K: V}`, found `i8[8]`
```

**And the shape beside it, which nobody ran.** The same hand scan over an **unterminated** field:

```
panic: index out of range for a fixed array      exit 134
```

So route 4's discoverability failure is worse than the ergonomist priced, exactly as their condition said — and the failure mode is a **run-time abort**, not a wrong answer and not an overread. Their "silent-overrun rate 0" for route 4 survives; a crash rate they did not price appears, on the 37-of-50 class.

**Their condition 2 also fires.** The whole diagnostic a reader gets is:

```
error[type_mismatch]: expected `cstr`, found `i8[8]`
  at bare.hero:10:20
```

No `note`, no `fix`, and `grep -c validated_bytes` over it returns **0**. And `validated_bytes` is absent from § 11's `Built-ins:` sentence (line 292) — it appears once in the whole spec, at line 364.

### 2c. `validated_bytes` accepts both signs

```
t.name.validated_bytes().must()   on i8[8]  ->  Hi
t.raw.validated_bytes().must()    on u8[8]  ->  Hi!
slot_len(t.raw.validated_bytes().must().cstr())  ->  3
```

The ergonomist's guess was right. The spec does not say so; the sentence at line 364 says *"a field of bytes"* and § 3's byte vocabulary is `u8`.

### 2d. The two corpus numbers — which one the synthesis carries

The brief's numbers all reproduce exactly. They are also **binding lines, not functions**:

| | lines | deduped by name |
|---|---|---|
| take a `cstr` or a `ptr` | **80** | **42** |
| take a plain `cstr` | **58** | **30** |
| take a `ptr` | **31** | **19** |
| take both on one line | **9** | — |
| fixed byte fields declared | **5** | in 4 files |

`58 + 31 − 9 = 80`, exactly.

**The compiler seat's "22 of 80 call sites" is right as a number and wrong as a label.** 22 is `31 − 9`, lines taking a `ptr` and no `cstr`. They are `extern` **declarations in this repository**, not call sites, and the deduped figure is smaller.

**The two denominators answer different questions and must never be merged.**
- **80 / 58 / 5** is *this repository's corpus* — it answers "how many bindings a field could plausibly be handed to here", and it is the right number for a Principle 0 argument.
- **141 / 91 / 8** is *16 real system headers* — it answers "what guarantee a real C parameter needs", and it is the right number for a soundness argument. The ffi seat itself says 8/91 is a floor, because those headers were chosen for their `char[N]` fields, not for their parameters.

Neither is a sample of the other. Carry both, each labelled with its corpus.

## 3. Contradictions, resolved

### 3a. "No Heroes-side shim can route around it" — FALSE, and the false sentence is in the defect record

The compiler seat calls it true. The spec-warden and the ffi seat each ran it. I ran it:

```
slot_len(t.name.validated_bytes().must().cstr())
  terminated i8[8] ("Hi\0...")                    ->  2      exit 0
  UNTERMINATED i8[8] ("fullest!"), 8 'A's after   ->  8      exit 0     [not 17]
```

**The spec-warden and the ffi seat are right; the compiler seat is wrong.** The sentence originates in `docs/work/DEFECTS.md` defect 061 (*"**No Heroes-side shim can route around it.**"*), it was quoted into `00-shared.md:27-30` as the premise of the sitting, and the compiler seat reaffirmed it from there. It is a record that needs a correction underneath it.

**The precise true sentence, from the emitted C.** The route works by **copying**:

```c
211: HeroStr hero_vb_text = hero_str_try_from_bytes((const char *)(t21.name), INT64_C(8), &hero_vb_status);
257: t33 = slot_len(hero_cstr_nonnull(t32));
```

C receives the runtime `str`'s pointer, never the field's address. So *"no Heroes-side shim **lends the field's address**"* is true, and *"no Heroes-side shim routes around it"* is false. The whole sitting rests on the second wording.

### 3b. The emitter trap — both seats are right, about different receivers

The shared brief (lines 60-63) and the compiler seat say a fixed value at a call argument makes `mangle.value` name a temporary that does not exist. The ffi seat says `storageless.fixed_text`'s `.field` arm renders `base.field` so the trap does not bite.

Settled from my own build: line 211 above renders **`t21.name`** directly, with no temporary. **For a FIELD receiver the ffi seat is right.** The brief's sentence is about a fixed value that is not a field (a local, a parameter), and nobody ran that shape. It stays a question.

### 3c. The `-Wpointer-sign` cast — already written

The compiler seat: routes 1 and 2 "owe an emitter cast that route 3 does not", and §7 makes it mandatory. Measured on a `u8[8]` field through the shipped route:

```c
hero_str_try_from_bytes((const char *)(t21.raw), INT64_C(8), &hero_vb_status);
```
```
clang -std=c11 -Weverything ...  ->  0 pointer-sign warnings
```

The cast exists, in `emit/bytes_text.hero`, for both signs. The debt the seat priced is one already-written expression. Its underlying finding — that `cstr` routes warn and `ptr` routes do not — is unaffected.

## 4. Questions the sitting should have asked

### Q1 — the write direction, and it is on defect 061's own unrun list

Defect 061's body says, in its own words: *"**The shapes beside it are unrun** (CL-061): a fixed array passed to a `ptr` parameter, **to an `@` out-parameter**, and as a struct member of a value crossing by value."* Two of those three had still not been run when the briefs were written. I ran them.

The ffi seat's own table says **50 of 141** pointer parameters are non-`const` — C writes them — and marks them *"no route in this sitting reaches these"*. Nobody followed it up. Measured:

- `function getcwd(buf: ptr, size: u64) -> cstr` **binds** — `heroes check` passes.
- Nothing in the language can produce the argument: `b.name` and `@b.name` are both `type_mismatch`.
- The working read route cannot serve it twice over: it hands C a `cstr` over a **copy**, and the spec-warden already measured that a `cstr` cannot reach a `ptr` parameter.

**This is the sitting's blind spot.** It was convened on the direction where a working composition already exists, and the direction with no composition at all was on the defect's own unrun list and stayed there. Route 3's single type rule is what both directions need (§1 route 7).

### Q2 — the construction wall: a separate defect, already recorded, never filed, and not a blocker

It is **not** the sitting's real question, and it is **not** new.

`docs/work/milestones/M-readable-bytes.md:39` already records it: *"building the struct | `Utsname(sysname: [0])` is `fixed_array_length`: the literal must hold exactly 256 elements, **804 characters on one line**"*, in a table that also records that `partial` does not help and a fieldless `tag` is a handle. `docs/work/DEFECTS.md` reads `**OPEN: 1**` and carries only 061. So: measured, written down in a milestone file, never made a defect, rediscovered by the compiler seat as "one adjacent defect".

**And it blocks nothing.** I built the full five-field real `utsname` binding, 1280 literal zeros:

```
./heroes run uts5.hero   ->   Darwin arm64     exit 0
```

Its price, on the reader's tokeniser:

| | |
|---|---|
| source | **4312 bytes** for a 9-line program |
| `heroes measure` | **2709** claude-legacy · **4000** cl100k |
| the same program without the literal | 253 bytes (one field) |

There is no escape: a short declaration is refused at `error[ffi_field_type]`. So it is an **ergonomic defect worth filing**, and it is the reason every seat argued from `utsname` and not one of them constructed one. I did.

### Q3 — route 4 is not free, and nobody priced its copy

The seats argued route 4 on soundness and spec tokens. Measured, it is **one runtime allocation per call**: `hero_str_try_from_bytes` builds a `HeroStr`, and `slot_len` receives that, not the field. CLAUDE.md § Precedence rank 3 puts robustness above speed, so this changes no verdict — but a resolution adopting route 4 is adopting a copy at every crossing, and it should say so rather than leave a reader to find it in the emitted C.

### Q4 — the sitting asked "how should the field cross" and never asked "what does a reader do today"

Today a reader who wants the length of a C string in a `char[8]` field can write a hand scan with `t.name[n]`, and it compiles. On a terminated field it gives the right answer. On an unterminated one it **aborts**. That is the program a reader reaches for before `validated_bytes` — the ergonomist predicted it and could not run it. It is the real status quo the four routes are being compared against, and no brief contained it.

### Q5 — how many real headers spell the parameter as an array?

Route 6 exists and its whole value is that number. It has never been measured. The 16-header corpus is already assembled in the ffi seat's `scratchpad/w/`, so it is one `grep` away.
