# Panel 164 — ffi-pragmatist report (verbatim)

## verdict

| route | verdict |
|---|---|
| 1. automatic decay | **veto** |
| 2. lend to `cstr` (`f.cstr()` widened from `str`) | **veto** |
| 3. lend to `ptr` only | **approve** |
| 4. refuse | **object** |

## section

**design.md §1.11, line 547**, in its own list of *"decisions already recorded in this document [that] exist for this reason and should not be revisited"*: **"Strings always NUL-terminated (`len+1` allocated) so `.cstr()` is free with zero copies (4.20)."** Restated at §4.20 (line 2453) and implemented at `runtime/heroes_runtime.h:74` — `const char *ptr; /* NUL-terminated; NULL only for the non-value */` — and `:192`, *"cstr: the FFI spelling (design.md §4.19). Free because of the NUL."* Route 3 rests on §4.10's `"§4.19's ptr/cstr sits outside the guarantee"` (line 1567).

**Where the document is silent, and I say so.** `grep -c` on design.md and spec/heroes-spec.md: `decay` 0/0, `char[` 0/0, `byte field` 0/0, `fixed_array` 0/0. Neither document covers array-to-pointer decay at the boundary. My objection is not covered; §1.11's NUL clause is the nearest binding sentence and it is binding.

## parameter classification, with the command

```sh
# the TU the 16 headers of panel 162 form
clang -fsyntax-only -Xclang -ast-dump=json all.c > all.json
python3 extract.py     # FunctionDecls with >=1 char*/void* parameter, deduped by name
python3 final.py       # split each pointer parameter: extent stated in the call, or not
```
Files: `scratchpad/w/{all.c,extract.py,final.py,final.json}`

| | count |
|---|---|
| functions with a `char*`/`void*` parameter | 113 |
| **`char*`/`void*` PARAMETERS** | **141** |
| `const` (a field could be lent here) | 91 |
| non-`const` (C **writes** it — no route in this sitting reaches these; Heroes has no writable byte buffer) | 50 |
| **class L** — extent STATED IN THE CALL, of the 91 | **8** |
| **class T/F** — extent found INSIDE the object (a terminator, or a width the header fixes), of the 91 | **83** |

The mechanical pass returned 12 `const` in class L; I moved four to T/F after reading the header text, and the headers themselves settle it: `grep "_LIBC_COUNT\|_LIBC_SIZE"` over the sixteen returns **9** annotations (all in `arpa/inet.h` and `dirent.h`), and `inet_net_pton(int, const char *, void *_LIBC_SIZE(__size), __darwin_size_t __size)` puts the size on the `void *`, not the `const char *`. Same for `inet_nsap_addr`; `getgrnam_r`/`getpwnam_r`'s `size_t` belongs to the `char *buf`. The remaining eight: `addr2ascii`, `gethostbyaddr`, `getipnodebyaddr`, `send`, `sendto`, `setsockopt`, `inet_ntop`, `inet_net_ntop`.

**The brief's inference from this split is wrong, and in the interesting direction.** The brief says *"if most pointer parameters are length-carrying, a `ptr` lend serves them soundly and a `cstr` lend is the exception."* Most are **not** length-carrying — 83 of 91. On the brief's logic that would elect route 2. It does not, because the 83 are already served (below), and the 8 are not. Caveat I state rather than hide: these sixteen headers were chosen by panel 162 for their `char[N]` **fields**, not for their length-taking parameters, so 8/91 is a floor on class L and not a census of C.

## what a field lend inherits, and what it lacks

Read at `selfhost/check/lending.hero` and `selfhost/emit/ops.hero:244`.

- **Inherited free**: R2's position rule. `is_lend` → `is_builtin_named` asks the *resolver*, and `argument_of` builds the parent map; adding a name to `is_lend` gives a field lend the identical *only as an argument of a call* rule at no cost.
- **Inherited free, and unnecessary**: `guard_arguments`'s `hero_cstr_nonnull`. Its narrowing is the callee's **declared parameter type**, so a `ptr` parameter gets `extern_probe.is_cast_out`'s `(void *)` and no null guard. A field lives inside a struct the program owns; its address cannot be null.
- **LACKED, and this is the whole soundness argument.** A `str`'s bytes are terminated **by the runtime's own layout** — `heroes_runtime.h`'s comment: *"len+1 bytes, the last of which is NUL. So `.cstr()` is `s.ptr` — zero copies."* A field's bytes are the **header's** and carry no such promise: panel 162 measured 37 of 50 with no reliable terminator. A `cstr` lent from a field would be the first `cstr` in this language that is not NUL-terminated, which falsifies §1.11's clause, `hero_str_cstr`'s zero-copy, and `c.validated()`'s read-to-the-NUL, all at once.

## experiment: the C I wrote, and what clang said

All programs at `scratchpad/w/`; compiler seeded in `scratchpad/panel164-ffi` by `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`. Every Heroes program built `--sanitize`.

**The finding that reframes the sitting — my own panel 162 claim, falsified.** I wrote *"no Heroes-side shim can route around it."* It routes around it today, since M-readable-bytes closed:

| program | what it printed |
|---|---|
| `p1.hero` — `slot_len(s: t.name.validated_bytes().must().cstr())`, `"Hi"` in `i8[8]` | `2`, exit 0, sanitizer clean |
| `p2.hero` — the **same call on a field with no terminator at all** (`fullest!` in `i8[8]`, followed by eight `A`s) | **`8`**, then `fullest!`, exit 0 |
| **`decay.c`** — route 1's C: `slot_len(full.name)` on the identical struct, `clang -std=c11 -Weverything -fsanitize=address,undefined` | **clang accepted it, zero relevant diagnostics, sanitizers silent, and it printed `17`** |

An eight-byte field read seventeen bytes, through `raw[8]` and into `id`. ASan cannot see an intra-object overflow in a C struct — `selfhost/emit/storageless.hero`'s own comment says so. The emitted C for `p1` shows why the sound route is sound: the `str?` lives in a named slot released **after** the call, and the argument is `slot_len(hero_cstr_nonnull(hero_str_cstr(t31)))`.

**A real header, class T** (`p6.h` over the real `<net/if.h>`, `IFNAMSIZ` 16, `ifr_name[IFNAMSIZ]`, `if_nametoindex(const char *)`):
```
lo0 -> 1
filled name is 16 bytes: abcdefghijklmnop
no such interface -> 0
```
The sixteen-byte name went over as sixteen bytes. Panel 162 predicted a terminator-only route would over-read here; the validated route does not.

**Shapes beside it, CL-061** (`p7.hero`: nested record, `u8[8]` beside `i8[8]`, a record obtained from a call, the same field lent twice in one call): `4`, `4`, `4`, `0`. All clean.

**The hole, on a real header.** `arpa/inet.h`: `inet_nsap_ntoa(int __binlen, const unsigned char *_LIBC_COUNT(__binlen), char *)` — the header itself says the extent is the sibling, never a terminator.

| | |
|---|---|
| `p8c.c`, the C binding, clang + ASan/UBSan clean | `0x47.0005.80FF.0000.01` |
| `p8a.hero`, the field to the `ptr` parameter with its length | ``error[type_mismatch]: expected `ptr`, found `u8[8]``` |
| **`p8b.hero`, the only door the language has** | **`the run the language gives: 1 of 8` / `what C then prints: 0x47`, exit 0** |
| `r3.c`, **route 3's emitted C** — `nsap_text((void *)(t20.nsap), INT64_C(8))` | `0x47.0005.80FF.0000.01`, sanitizers clean |

Also measured: `p4.hero`, a real `uuid_t` field (`u8[16]`) → `refused: not_text`; `p5.hero`, valid UTF-8 with an interior zero → `the door gives 2 bytes; the parameter needs 16`.

Route 3's C is already in the emitter. `selfhost/emit/bytes_text.hero:91-96` calls `storageless.fixed_text` and emits `hero_str_try_from_bytes((const char *)(t21.name), INT64_C(8), …)` — the field decayed at a call argument, with its declared width beside it. Route 3 is that line with `(void *)`. Panel 162's `mangle.value` trap does not bite a **field**, because `fixed_text`'s `.field` arm renders `base.field`.

One warning worth the record: `cstr` handed to a `const unsigned char *` parameter emits `-Wpointer-sign` in the probe, which is **not** among the four `#pragma clang diagnostic error` flags in `selfhost/emit/extern_probe.hero:77-80`, so it builds. Route 3's `(void *)` produces only `-Wimplicit-void-ptr-cast`, a C++-only diagnostic. **Route 3 is quieter against the real header than route 2.**

## argument (≤120 words)

The brief's split decides nothing, because its premise expired this morning. 83 of 91 lendable pointer parameters find their extent inside the object, and that majority is **already served**: `f.validated_bytes().must().cstr()` compiles today, and on an eight-byte unterminated field hands C eight bytes where C's own decay hands it seventeen, silently, under `-Weverything` and ASan. So route 2 buys the overread and nothing else; route 1 is the same overread with no `.cstr()` to see. What is unserved is the eight: a binary field plus its declared length. Measured, that binding compiles, exits 0, and prints `0x47` where the C prints `0x47.0005.80FF.0000.01`. A silently wrong answer, not a refusal.

## what each route refuses, with a named header function

- **Route 1** refuses nothing and therefore refuses robustness: `if_nametoindex(r.ifr_name)` on a filled `IFNAMSIZ` name compiles and reads past the field.
- **Route 2** makes `strlen`-class calls writable that are *already* writable, and makes `uuid_is_null(r.id)` **no** more writable — `uuid_t` has no terminator to find.
- **Route 3** makes `sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*))` writable and leaves `if_nametoindex(const char *)` to the validated route, where it already works.
- **Route 4** refuses `inet_nsap_ntoa`, and design.md Part 6 asks a refusal to name the program fact that would make it wrong. `p8b.hero` is that fact: today it is not refused, it is answered wrongly at exit 0.

## prediction (falsifiable)

**Step 3 of §4.19's ladder needs no shim under route 3, and cannot be written without one under any other.** `examples/sqlite/main.hero` binds seven functions and none takes a blob. Under route 3, `function sqlite3_bind_blob(statement: Stmt, index: i32, value: ptr, n: i32, free: ptr) -> i64` called as `sqlite3_bind_blob(statement: st, index: 1, value: r.id, n: 16, free: nullptr)` with `r.id: u8[16]` will build, link and round-trip sixteen bytes. Under route 2 it will not: `p4.hero` measured that exact field as `not_text`. **I am wrong if a `u8[16]` UUID round-trips through `sqlite3_bind_blob`/`sqlite3_column_blob` without a C shim using only what the language has today.**

## veto, as a refusal

**I refuse any resolution that lets a `cstr` reach C without a NUL at the end of its bytes.** That covers route 1 and route 2 as the brief states them, since the brief itself says route 2's *"termination question is the same as route 1's."* The ground is not taste: design.md §1.11 lists NUL-termination among the decisions *"that should not be revisited"*, and it is load-bearing three times over — `hero_str_cstr` is zero-copy **because of** the NUL, `c.validated()` reads **to** the NUL, and `guard_arguments` checks only null **because** the NUL is assumed. A `cstr` without one falsifies all three at once and, measured, turns an eight-byte field into a seventeen-byte read that clang and both sanitizers pass in silence. This is a refusal, not a price, and there is no ergonomic gain I will trade for it.

I do **not** veto: a lend to `ptr`; a fixed-width literal or zero default in an `extern` record; anything that leaves the field declared at the header's width so panel 162's `_Static_assert` (`256 == 8`) keeps firing.

## condition

I move route 3 from approve to object, and route 2 from veto to object, if either of these is shown:

1. **Route 2 with a copy.** If `f.cstr()` on a field is specified to *copy* into a runtime `str` and lend that — i.e. `validated_bytes().must().cstr()` under a shorter name — my veto does not reach it, because the NUL is the runtime's again. But then it is sugar rather than a route: it still cannot carry `uuid_t` (measured `not_text`) and it still truncates at an interior zero (measured, 2 of 16), so it closes nothing the language does not already have.
2. **Route 3 without an escape rule.** `check/lending.hero`'s R3 stops a Heroes function answering `cstr`; there is no equivalent for `ptr`, and design.md Part 9's re-measurement puts `ptr` escaping a group at zero *today* — a fact about the corpus, not the rule. If route 3 lands without the R3 analogue for a field lend, I object: a lent field address stored in a local and used after the record dies is the heap-use-after-free panel 122 built twice, with no `str` refcount underneath it to make ASan loud.
