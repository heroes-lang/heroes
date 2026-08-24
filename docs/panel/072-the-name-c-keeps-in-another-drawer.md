# Panel 072 — the name C keeps in another drawer

**Convened** 2026-08-16, M-selfhost-port, from panel 071's seventh hole — the one
its ffi-pragmatist found while running that sitting's own experiment, and which
was on nobody's list. **Two seats: ffi-pragmatist and historian.**

**That lane is too narrow for what the sitting concluded, and the resolution says
so.** The answer both seats converge on is a **new surface word**, which CLAUDE.md
§4 makes a full-panel matter: the llm-ergonomist judges a word a reader must
learn, and the spec-warden prices the sentence it obliges. Neither sat. So this
file records what is **proven** and refuses to land what it is not entitled to
adopt.

## What was asked

Measured by the coordinator before the briefs went out, on one header carrying
all three shapes C permits:

```c
struct TagOnly { int32_t a; };                     /* tag, no typedef */
typedef struct { int32_t a; } Typedefed;           /* typedef, no tag */
typedef struct NamedBoth { int32_t a; } NamedBoth; /* both */
```

```
record TagOnly    -> exit 2   internal error: use of undeclared identifier 'TagOnly'
record Typedefed  -> exit 0
record NamedBoth  -> exit 0
```

**Exactly and only the tag-without-typedef case fails, and it fails at exit 2** —
the compiler blaming itself for a header the author is entitled to bind. The
emitter writes the record's bare name where C requires `struct TagOnly`.

Candidates: **1** a surface marker on the group's `record` · **2** an emitter
construct that never has to choose · **3** emit `struct <name>` whenever a bare
`<name>` does not compile · **4** something else.

## The lead finding: `struct stat` binds, and then does not

The ffi-pragmatist was asked to bind the real `struct stat` end to end rather
than model it. With the tag fix alone:

```
error[declared_twice]: `stat` is already declared — one file is one module
    function stat(path: cstr, @buf: stat) -> i32
```

Reproduced by the coordinator. **C has four namespaces (C11 6.2.3); Heroes has
one.** `struct stat` and the function `stat()` are different names in C and the
same name here. Measured: **4 of 155 real tags across 19 headers are also a
function or object** — `stat`, `sigaction`, `sigvec`, `timezone`.

So a **boolean** marker does not work. The marker must be able to carry the C
name, because otherwise the most-bound struct in POSIX stays unbindable **beside
the call that fills it**. With a rename, against the real header and real
`stat()`:

```
record FileStat tag stat   ->  rc=0  st_size=10  st_ino_nonzero=true  mtime_sec_nonzero=true
```

18 fields, **no `partial`**, four nested tag-only `struct timespec`, an `i64[2]`
member, `@buf: FileStat` as an **`@` parameter rather than a `ptr`**, exit 0.
`partial` composes with it: four lines bind the struct.

## Candidate 2 is refuted, and the coordinator's own question is what refutes it

The brief asked whether the generated unit could simply emit `typedef struct X X;`
for a tag-only header, and whether a **redundant** typedef is legal where the
header already has one.

**It is legal** — C11 6.7p3, and clang accepts it silently under all thirteen
flags. And the blanket rule still dies three ways:

```c
typedef struct Typedefed Typedefed;  /* typedef redefinition with different types */
typedef struct FILE FILE;            /* 'struct FILE' vs 'struct __sFILE' */
typedef struct stat stat;            /* redefinition of 'stat' as a different kind of symbol */
```

**The third is the acceptance case itself.** `<sys/stat.h>` declares the function
`stat` as well, so the single most-bound struct in POSIX refutes the helper
typedef. Writing `struct stat` at every emitter site instead compiles clean.

No C11 spelling names a struct under both shapes: `struct X` is *incomplete
definition* for a typedef-only header, bare `X` is *undeclared identifier* for a
tag-only one.

## Candidate 3: the objection is soundness, not taste

The ffi-pragmatist **withdrew** its panel-071 charge of prose-matching — a probe
translation unit asks a compile-time question and reads only an exit code, which
is legitimate, and at 62 ms each it is 2.1 s per cache miss for raylib's 35
records, which §13 says is not the argument.

**This is the argument:**

```c
struct Foo { int32_t a; };                            /* the tag */
typedef struct Bar { double d; char pad[24]; } Foo;   /* a DIFFERENT type, same spelling */
```

Legal C11, `sizeof` 4 against 32. A probe asks *does the bare name compile*, is
told yes, and binds **the wrong type** — after which every field assertion passes
against it. The seat is honest about the limit: **0 of 155** tags show this shape
in the 19 headers it preprocessed, so it is constructible rather than observed.
But CLAUDE.md §11 is precisely about premises that expire in silence, and probing
inverts §4.19's thesis: the author declares and clang refutes, it does not guess
and succeed either way.

## Scale, and why nobody hit this before

Tag-only against total tags, measured: **POSIX 46/47 · curl 43/47 · zlib 14/15 ·
sqlite3 3/23 · raylib 0/35.**

The seat's sentence is the one to keep: *"the corpus that never hit this is
precisely the part of C that does not have the problem."* Every binding this
project has written is against an application library; the platform is the other
kind.

**The historian explains the split rather than counting it.** The Linux kernel
coding style, chapter 5, says in those words that using a typedef for a structure
is *a mistake*, with five narrow exceptions. System headers follow that; SDL3 and
raylib do not. **Neither default is safe by frequency**, which is an argument for
letting the binding say which it means.

## The historian: four projects, four open bugs, no reversals

| system | mechanism | what happened |
|---|---|---|
| **Nim `importc`** | the binding carries the C spelling, **as a string** | No bug on this seam. But the string is **not a name** — the manual says *"a string containing the C identifier"* and the stdlib writes `"struct stat"`, two tokens; the compiler field is literally `snippet` and nothing validates it. Its generator has emitted the wrong one since [c2nim #131](https://github.com/nim-lang/c2nim/issues/131), 2018, **open** |
| **Go cgo** | a rule: `C.struct_stat` | [golang/go#19487](https://github.com/golang/go/issues/19487), 2017, **open** — `var V C.struct_X` for a struct that does not exist **compiles and prints `{}`**. That is this project's own thesis class |
| **Zig translate-c** | a rule, and **the tag is canonical** — the only "tag as default" precedent found | [ziglang/zig#21746](https://github.com/ziglang/zig/issues/21746), 2024, **open**. And Zig is leaving: `@cImport` is **deprecated in 0.16**, translation moved out of the language |
| **rust-bindgen** | a rule, both namespaces into one | [#3162](https://github.com/rust-lang/rust-bindgen/issues/3162), 2025, **open** — emits code that does not compile |
| **Swift ClangImporter** | tag used directly, collision machinery in C++ | mechanism needs Clang embedded; inadmissible here |
| **C++** | unifies the namespaces (Annex C, C.8.7) | it can, because it *compiles* C. Heroes **emits** C, and clang still enforces C's rule |

**Every project that let a rule pick the namespace has an open bug on it — the
oldest eight years, none ever closed.** The one that lets the *binding* say it
has no such bug. The historian's conclusion: *"the collision is not solvable by
naming policy. Only by letting the binding say which namespace it means."*

And CLAUDE.md §10 forbids a second binary, so every generator-based answer is
inadmissible here regardless — which is itself the finding, because it is the
majority answer.

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **ffi-pragmatist** | approve **1** with a **non-optional amendment** · **refute 2**, veto the helper typedef as a blanket rule · **object to 3** on soundness | It bound the real `struct stat` and found the sitting's shape was wrong: a boolean marker leaves `stat`, `sigaction`, `sigvec` and `timezone` `declared_twice`, so **the marker must carry a C name**. Prototype: 6 files, **73 lines**, all 14 examples green, `cargo test` 476 + 7 + 14, double-emit byte-identical, `HERO_RUNTIME_ABI` **unchanged at 14** — `runtime/` is untouched |
| **historian** (advisory) | approve **Nim's location, refuse Nim's freedom** | Four projects, four open bugs, zero reversals. Nim's string is unvalidated verbatim C — `"unsigned long long"`, `"const char *"` all work — and free-form C at the `extern` boundary would add a **fifth** author-caused clang-failure class that §7's `declaration()` narrowing cannot recover. `record Font partial` (`spec:220`) is already the modifier slot: **one token, no new class, `certain`-fixable** |

## Three riders, all measured, all costs of this fix

1. **The marker string breaks §7's named exception.** The field assertion's
   marker is `heroes-ffi-field {c_type} {field}`, and with `c_type` = `struct
   stat` that is three tokens; `emit/ffi_record.rs:46` splits on whitespace and
   takes token 1 as the record name. **The same author mistake gets two
   verdicts** — exit 1 on a typedef'd record, exit 2 *"internal error"* on a
   tagged one. One line (the marker carries the *declaration's* name), applied
   and re-measured by the seat, exit 1 restored.
2. **The `union` half takes a veto**, and it is independent of tags: `record`
   cannot express a union today. Measured on a **typedef'd** union, so
   pre-existing — `UDef(i: 1, f: 1.0)` emits a compound literal, last field wins,
   `a.i` reads **1065353216** at exit 0 with only a warning. Widening reach into
   that path is refused until it is fixed. Only 2 union tags exist in the 19
   headers.
3. **The word cannot be `struct` or `union`.** Both are in the foreign-word
   registry with a `certain` fix (`lexer/keywords.rs:61,63`). And
   `emit/typedefs.rs` goes **291 → 303**, past §11's ceiling, so the change
   carries a split.

## The resolution — `ratified 2026-08-24` (author instruction, batch over 069-079; § Author's verdict below)

1. **Candidate 2 is refused** on four compiles, and the refutation is recorded
   with its C: no spelling names a struct under both shapes, and the helper
   typedef dies on the acceptance case itself.
2. **Candidate 3 is refused** on soundness: a probe can bind a *different type of
   the same name*, which is legal C11 and which every field assertion would then
   pass against.
3. **Candidate 1 is the shape**, and its amendment is not optional: the marker
   **carries a C name**, because 4 of 155 real tags collide with a function or
   object in Heroes' single namespace and one of them is `stat`.
4. **It is NOT adopted here, and that is the conservative resolution rather than
   a delay.** A new surface word is CLAUDE.md §4's full-panel trigger, and the
   two seats that judge a surface word did not sit: the **llm-ergonomist**, who
   would meet it as a reader with only the spec, and the **spec-warden**, who
   prices the sentence § FFI then owes. This sitting is entitled to say *the
   mechanism is this and no other*; it is not entitled to put a word in the
   language on two seats. **The exact spelling is the missing seats' question**,
   and the prototype makes it cheap to price.
5. **Two things land regardless of the spelling, because they are true today**:
   rider 1's whitespace split gives the *same mistake* two different exit codes,
   and rider 2's union hole produces a **wrong value at exit 0** on a typedef'd
   union with no tag anywhere near it. Both are filed as their own items.
6. **`struct stat` is the acceptance test** for whatever spelling wins, and the
   ffi-pragmatist's four-line binding is the artifact it must reproduce.

**What a veto at ratification would compel**: nothing is landed, so nothing is
reverted. The prototype stays in the scratchpad.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| ffi-pragmatist | `record FileStat tag stat partial` + `function stat(path: cstr, @buf: FileStat)` binds `<sys/stat.h>` in **one module, four lines, no shim and no `ptr`**; `struct timeval`, `struct sockaddr_in` and `struct in_addr` follow with no further mechanism | M-ffi-ladder's next rung |
| ffi-pragmatist | SQLite ladder step 3 needs **no shim** under this rule — sqlite3's 3 tag-only structs are all `sqlite3_index_*`, none on the ladder | §4.19's ladder |
| historian | if the **free-form string** is adopted, at least one binding in the corpus will carry a spelling that is **not** `struct <identifier>` — a pointer, a qualifier, `unsigned long`, a union. If none appears, the string bought nothing a keyword would not have, at the price of a new clang-failure class | M-ffi-ladder, by grep |

**Scored in this sitting**: the coordinator's question *"is a redundant `typedef
struct X X;` legal, and does clang accept it under our flags?"* — **yes to both**,
and the answer was still not enough, which is the more useful result.

## Conditions on the record

- **ffi-pragmatist, approve → object** if the marker is adopted as a **boolean**
  without the C name. Not ergonomics: `stat`, `sigaction`, `sigvec` and
  `timezone` are `declared_twice`, measured.
- **ffi-pragmatist, object → veto** on candidate 3 if it is ever adopted without
  a companion assertion that the probed name denotes the type whose fields were
  checked.
- **ffi-pragmatist, veto** on extending the marker to `union` until
  `record`-binds-a-union is repaired.
- **historian reverses** on a language with no generator step that made the tag
  the default *and recorded why* (Zig is the closest and is a generator, so it
  does not count), or on a corpus split measured across system **and**
  application headers.
- **The coordinator's own count is briefed, not verified by the historian**,
  which that seat said in its own words. The ffi-pragmatist's 155-tag / 19-header
  measurement supersedes it and is the number to cite.

## Author's verdict

**2026-08-24: ratified** (author instruction, *"ratifica anche quelle 11"* — a batch yes over panels 069-079, given after being told plainly that every one had shipped as a provisional default and that the tree had been green over all of them for eight days. The author was offered the alternative of reading each first and chose the batch.)

What the yes settles: **candidates 2 and 3 are both refused, and the refutations are recorded with the C that produced them** — no spelling names a struct under both shapes, the helper typedef dies on the acceptance case itself, and a probe can bind *a different type of the same name*, which is legal C11 and would silently corrupt every field assertion built on it. The sitting's own note that its resolution is **narrower than its answer** stands: the yes ratifies the narrow half and nothing beyond it.

**On the batch.** This is the fourth blanket ratification in this project's record and the largest. It closes a gap of eight days in which eleven sittings sat queued with nobody asking — the same shape panels 085-087 sat in for four. What the batch does NOT do is re-open anything each sitting left explicitly open: every reserved veto, queued follow-up and unmet condition inside this file stands exactly as written, and a yes over the resolution is not a yes over the questions the resolution deferred.
