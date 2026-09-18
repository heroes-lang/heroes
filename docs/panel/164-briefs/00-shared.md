# Panel 164 — shared brief: a fixed-array field cannot cross the boundary in either direction

**Every number here was produced by a command run on 2026-09-18 while this brief
was written, and the command is named beside it** (CL-077). **Every negative
sentence here was run, or is marked as a question** — panel 163's rule, adopted
the same day after six of that brief's premises were false.

## The question

**A header record's `char[N]` field can be READ as text since panel 162. It
cannot be PASSED to any C function. How should it cross?**

## The defect, measured in its three shapes

A local header declares `char name[16]` in a struct and three functions:
`slot_len(const char *)`, `slot_first(const void *)`, `slot_fill(Slot *)`.
Bound as `name: i8[16]`, `heroes check`:

| the program writes | the compiler says |
|---|---|
| `slot_len(s.name)` against `s: cstr` | `error[type_mismatch]: expected cstr, found i8[16]` |
| `slot_first(s.name)` against `p: ptr` | `error[type_mismatch]: expected ptr, found i8[16]` |
| `slot_len(s.name.cstr())` | `error[bad_operand]: cstr takes str, found i8[16]` |

**In C every one of these is legal**: `char name[16]` decays to `char *` at a
call, which is why `strlen(u.sysname)` is the idiom every POSIX program writes.
The ffi-pragmatist at panel 162 found this beside another question and said:
*the field is not only unreadable — it is unpassable, and no Heroes-side shim
can route around it.* That is the completeness failure
`.claude/rules/c-boundary.md` names: a library the author must leave C code
around for.

## The scale, measured

- `grep -rhoE "^\s+function \w+\([^)]*\b(cstr|ptr)\b" examples tests/golden`:
  **80** extern functions in this repository take a `cstr` or a `ptr`; **58**
  take a plain `cstr`. Every one of them is a function a byte field could
  plausibly be handed to.
- `grep -rhoE ": [iu]8\[[0-9]+\]" examples tests/golden`: **5** fixed byte
  fields declared in the repository, all short.
- Panel 162's ffi-pragmatist walked 16 real headers: **50** `char[N]` fields of
  712, and **only 13 reliably NUL-terminated**. That number decides how a field
  may be handed to a `cstr` parameter, because `strlen` walks until it finds a
  zero.

## What the language already has, and what it does not — each one run

- **It HAS a lend for a `str`**: `s.cstr()` gives C a pointer to a `str`'s bytes
  for the duration of one call, and `check/lending.hero` refuses it anywhere but
  as an argument of a call (panel 122 R2). `guard_arguments` in `emit/ops.hero`
  checks it for null on the way out.
- **It HAS a lease**: `x: cstr @ s.lease()` is a copy C may keep until
  `end_lease(@x)`.
- **It HAS the read**: `f.validated_bytes()` since panel 162, to its first zero or
  whole.
- **It does NOT decay** — measured above, all three shapes refused. This is a
  question and not a premise: *is there any position today where a fixed byte
  field reaches a C pointer parameter?* The three probes above say no; a seat
  that finds one has found the cheapest answer.
- **`storageless.hero` renders a fixed array at its use site** because C gives it
  no assignable storage; a call argument is one such site, and `mangle.value`
  there names a temporary that does not exist (panel 162's implementation found
  this the hard way).

## Routes named, and four is not a claim about the set (CL-057)

1. **Automatic decay.** A fixed byte field is accepted wherever a `cstr` or `ptr`
   parameter is declared — C's own rule, applied at the checker. The cheapest to
   write and the one that inherits C's oldest overread: 37 of 50 real fields
   have no terminator, and `strlen` over one walks into the next field.
2. **An explicit lend to `cstr`**: `f.cstr()` widened from `str` to a fixed byte
   field, under the same position rule the `str` lend already obeys. The reader
   sees the lend; the termination question is the same as route 1's.
3. **An explicit lend to `ptr` only**: `f.ptr()` or the like, refusing `cstr`
   because a `cstr` is a promise of a terminator that a `char[N]` field does not
   make. A C function taking `const void *` plus a length gets the field; one
   taking `const char *` does not, and the author says why in the binding.
4. **Refuse.** A field is read with `validated_bytes` and never passed; a C
   function that wants the field's bytes gets them through a `static inline` the
   header declares. Held to design.md Part 6's standard.

## Constraints every seat is held to

- **CLAUDE.md § Precedence**: robustness first. A route that lets a program
  compile and overread is worse than one that refuses.
- **Principle 0**: the compiler self-hosts without this. The thesis branch
  carries it or nothing does.
- **The budget**: `./heroes measure spec/heroes-spec.md` reads **6041**
  vendored and **8040** real against **10240**, after two amendments already
  landed in this milestone at +46 and +10.
- **Build in a copy**; seed with `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`; never rebuild from `selfhost/`; never
  `archive/bootstrap-rs/`. The working tree is frozen.
- **Say what your route costs to IMPLEMENT, in the files it lands in.** Panel
  162's resolution was corrected three times at implementation and panel 163's
  brief was wrong six times; both because nobody priced the code.

## What the sitting must deliver

A resolution that closes defect 061 or a refusal that says why the field is not
owed a way across — with the termination question answered either way, because
it is the one that decides whether a route is sound.
