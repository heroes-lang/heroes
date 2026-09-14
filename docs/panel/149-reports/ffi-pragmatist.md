# Panel 149 — report of the ffi-pragmatist

**Verdict: R1 adopt with condition · R2 adopt · R3 REFUSE THE REFUSAL, with a
narrow veto.** Sections: design.md §1.11 (founding constraint), §4.19 (the
ladder, the handle, `acquires`/`borrows`), §4.20 (the counter), §4.17 (R2), Part
6's user-defined-annotations row (the condition on R1).

## Experiment 1 — `getaddrinfo` is UNWRITABLE, and the defect's canonical case is falsified

Pure-C reference: exit 0, two results in the chain, **one** `freeaddrinfo`. Four
Heroes spellings were tried, and this is the vocabulary searched.

- **the handle form**, `record Ai tag addrinfo`: `heroes check` exit **0**,
  `heroes build` exit **2**, eighteen clang errors — *"use of undeclared
  identifier 'addrinfo'"*, *"must use 'struct' tag to refer to type
  'addrinfo'"*. Cause, measured: `handles.hero`'s `c_spelling` writes the tag
  verbatim and starred, `addrinfo *`, and `grep typedef.*addrinfo` in the SDK's
  `netdb.h` is **empty**, where `sqlite3.h:279` has `typedef struct sqlite3
  sqlite3;`. **The handle form silently assumes a typedef.**
- **`tag struct addrinfo`**: `error[reserved_word]`; the grammar is `"tag" ident`,
  one token.
- **the by-value group record**: checks clean, build gives a correct exit-1
  `ffi_parameter_type` whose note advises the handle form, which does not
  compile.
- **`ptr`**: the only spelling that works. check 0, build 0, run 0. **And no
  field of the chain is reachable** — `ai_family`, `ai_addr`, `ai_next` all live
  behind that `ptr`.

**So the defect's own canonical case is confirmed REAL as a C shape and
FALSIFIED as an R1 case**: on this Mac `getaddrinfo` is not a handle producer at
all, it is a `ptr` producer, and R1 does not touch it. Second independent
instance: `<net/route.h>`'s `struct sockaddr`, same failure, same cause.

**A fifth exit-1 class is owed.** `.claude/rules/c-boundary.md` names four classes
where a clang failure is the author's `extern` and exits 1. A `tag` naming a C
type that needs the `struct` keyword is a fifth, and it exits **2**, blaming the
compiler for a binding the author wrote.

## Experiment 2 — the `ptr` blind spot, exit 0 and a real leak

A group with `freeaddrinfo(ai: ptr consumes)`, producer unmarked, nothing freed:
`check` exit **0**, run exit **0**, zero diagnostics, **real leak**. The
identical shape with a handle type aborts 134.

And `acquires`/`consumes` on a `ptr` **do** count at runtime: the `ptr` version
with the release skipped panics *"1 C handle(s) never given back"*, exit 134,
byte-identical to the handle case. **So the runtime half already works for `ptr`;
only the static demand is handle-gated.**

## Experiment 3 — raylib's `Font`: R3's shape is REAL, compiles, and is already correct with ONE mark

`/opt/homebrew/include/raylib.h:1484-1498`. `Font` is returned **by value**
carrying **two owned pointers of two types**, released by **one** `UnloadFont`.
The binding clang accepted against the real header, built, linked with `package
"raylib"` and executed raylib code:

```
    record Recs tag Rectangle
    record Glyphs tag GlyphInfo
    record Font
        baseSize: i32
        glyphCount: i32
        glyphPadding: i32
        texture: Texture2D
        recs: Recs
        glyphs: Glyphs
    function LoadFont(fileName: cstr) -> Font
    function UnloadFont(font: Font consumes)
```

Verification is intact — the emitted C carries a `_Static_assert` per field
(`heroes-ffi-field Font recs`, `… Font glyphs`), so a wrong field type is still a
compile error. **`importc` verification survives R1 untouched.**

| program | mark on `LoadFont` | exit |
|---|---|---|
| unmarked | none | **134**, defect 033's own message |
| marked | `acquires UnloadFont` | **0** |
| `GetFontDefault() -> Font borrows`, nothing released | `borrows` | **0** |

**Defect 033 is real in a shipped library and R1's repair is exactly right.** It
is also the first evidence R1 does **not** refuse a correct binding: `LoadFont …
acquires UnloadFont` and `GetFontDefault() -> Font borrows` are two functions of
identical C result type, one owning and one lending — the
`sqlite3_prepare_v2` / `sqlite3_next_stmt` pair one level up, and panel 148's
`borrows` reaches it unchanged.

## Experiment 4 — the fixed array: no static count can be right, and none is needed

**3400** headers scanned under `/opt/homebrew/include` and the SDK: 43 structs
hold a fixed array of pointers, 283 hold two or more distinct struct-pointer
fields. The two real ones:

- `<net/route.h>`: `struct rt_addrinfo { int rti_addrs; struct sockaddr
  *rti_info[RTAX_MAX]; }` — `RTAX_MAX` is 8 and **which slots are filled is a
  sibling field**, a bitmask. Unwritable in Heroes today, experiment 1's cause.
- `jpeglib.h:638-639`: `JQUANT_TBL *quant_tbl_ptrs[NUM_QUANT_TBLS];` with the
  header's own comment **`/* … or NULL if not defined */`**.

**Both headers say in their own text that no static count can be right.** A thin
C shim modelled field-for-field on `jpeglib.h:638`, four slots, `how_many`
filled, **one** destructor:

| program | filled | mark | exit |
|---|---|---|---|
| shim | 2 of 4 | `acquires quant_destroy` | **0** |
| shim | 0 of 4 | `acquires quant_destroy` | **0** |
| shim | 2 of 4 | none | **134**, defect 033's message |

**One mark is one increment, whatever the arity.** The binding author writes
nothing about the count, because C does not: `freeaddrinfo` is one free for an
N-long chain, `UnloadFont` one for two arrays, `UnloadModel` one for four
(`raylib.h:1591-1594`), `xmlFreeDoc` one for a whole tree.

## The narrow veto

**The shared brief's sentence *"`Slot[4]` reaches four"* is true of the TYPE and
must not be read as licensing four increments.** A resolution in which the
counter counts reachable handles breaks `Font` (+2/-1), `Model` (+4/-1) and the
shim (+4/-1 with two null) — **every real C composite, at abort 134, on correct
programs.** The seat vetoes that reading specifically. **It does not veto R1.**

## Does R1 refuse a correct binding? The panel-148 objection

**Looked for, and no**, on the condition that R1 stops at the mark and never
reaches the counter. The borrow risk one level down is real and `GetFontDefault()
-> Font borrows` accepts it at exit 0. The arithmetic risk was measured at n=2
(`Font`) and n=4-with-2-null (the shim): one mark, one release, exit 0.

## What the rule cannot see, because a `ptr` is opaque

`struct addrinfo` contains `struct addrinfo *ai_next`. Heroes cannot follow it: a
`ptr` has no pointee in the type system, so the walk stops dead. The rule cannot
see a handle behind a `ptr` field, a handle behind a C type with no typedef, or a
producer whose whole result is a `ptr`.

**Is the hole worse? In kind yes; in scope it is a different defect.** R1's hole
is **loud** — unmarked today the program aborts 134 with a message naming its own
cause. The `ptr` hole is **silent** — exit 0, zero diagnostics, real leak. By
§1.12 and CLAUDE.md § Precedence rank 3 a silent corruption class outranks a loud
one, **so the `ptr` gap is the more serious and R1 closes the less serious.** That
argues for filing the `ptr` gap as its own defect, not against R1: both end at the
same `acquires`/`borrows` vocabulary.

## ABI: no breakage, and no shipped binding newly needs a mark

Measured over the whole tree: 125 files carry an `extern`; 16 handle types
declared; 30 group records carry fields; **7** fields have a handle or
nested-group-record type, and **all 7 are under `tests/golden/run/`**, none under
`examples/` or `selfhost/`. Of those 7, only two are a handle in a field:

- `tests/golden/run/abort-handle-given-back-unmarked.hero` — has `slot_close(s:
  Slot consumes)`, so R1 turns it from an abort-134 run case into a compile
  error. **One file moves**, `run/` → `check/`.
- `tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero` — has **no**
  `consumes` anywhere, so `consumed_types` is empty and `bindings_say_which`
  returns at its first line. **This file does not move.**

The emitted C is unchanged for every program that compiles today: R1 adds a
refusal and no codegen.

## The condition on R1: `acquires <releaser>` does not check its name

Three measurements, all exit 0 at `check` **and** `build`:

- `sqlite3_open(…, @out: Db acquires sqlite3_notafunction)` — builds, runs, exit 0
- `quant_start(…) -> Decomp acquires quant_notafunction` — check exit 0
- `sqlite3_open(…, @out: Db acquires sqlite3_finalize)` — a releaser consuming a
  **`Stmt`**, not a `Db`. Check exit 0.

`unknown_freer` exists at `selfhost/check/freer.hero:187` and guards only panel
109's `owned <freer>`. design.md Part 6's user-defined-annotations row says it
plainly: *"A tag nobody reads is a comment that looks like a guarantee, which is
the one thing an FFI must never carry."* **The condition: R1 lands together with
(i) the name resolving to an `extern` in this module, and (ii) that function
carrying `consumes` on a parameter of the type actually reached.** Both are
declaration facts, both the same 17-line shape as `one_tag_one_type`.

## R2 — adopt the path

`LoadFont(fileName: cstr) -> Font` shows no handle at the line the caret lands
on; the handle is `Font.glyphs`, in a different declaration, and in `Model` it is
four fields deep. §4.17 asks a diagnostic carry everything needed **without
opening another file**. Print the field path, and for the array case the element
with **empty** brackets, `Decomp.quant_tbl_ptrs[]`, because experiment 4 shows
the index is never a compile-time fact.

## Prediction

**Under R1 with the conditions above, exactly one file in the repository changes
verdict: `tests/golden/run/abort-handle-given-back-unmarked.hero`.** Zero files
under `examples/` and zero under `selfhost/` acquire a new diagnostic.

```sh
./heroes run tests/harness/main.hero -- ./heroes check run emission determinism corpus
for f in examples/*/main.hero examples/*/*/*.hero; do ./heroes check "$f" || echo "NEW: $f"; done
```

A second red, or any `examples/` line, means R1 reaches further than measured
here and the verdict reverts to object.

## Conditions that would change the verdict

- **R1 to veto** if reachability changes the counter, that is, a type reaching
  *n* handles demands or emits *n* increments.
- **R1 to object** if it lands without the `acquires`-name check, since it turns
  an unverified word into a mandatory one in more places.
- **R3 to adopt the refusal** if somebody produces a real installed header where
  a struct handed back by an `extern` carries two owned pointers released by
  **two different** calls, and neither `borrows` nor a single `acquires`
  describes it. This is a negative claim resting on the seat's vocabulary: 3400
  headers were searched for pointer-typed struct members and for
  `Unload*`/`free*`/`destroy*` producer-destructor pairs; C++ headers and
  anything needing a shim were not searched.
