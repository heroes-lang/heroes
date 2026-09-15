# Panel 150 — report of the ffi-pragmatist

## A correction the seat owed before anything else

**Its first scratch copy was three commits stale** — no `check/reaches.hero`, no
`unread_releaser` in the seed, the old three-cause runtime message — and on that
basis it doubted the shared brief. *"The brief was right and I was wrong."* It
re-copied, rebuilt in 3.07 s, and **re-ran every experiment against HEAD
`04661466`. Two findings changed sign.**

> `cp -r` of a live checkout is not a measurement of that checkout — verify the
> commit.

| | verdict |
|---|---|
| **Q1 — the `ptr` producer** | **object**, not veto: the ABI was measured and is untouched |
| **Q2 — the per-element release** | **VETO** on *refuse the element release*; object on the rest |

## Q1 — the counterexample the brief asked for, compiled

**One header, one group, `ptr` both owned and borrowed, two non-interchangeable
releasers.** Check 0, run 0:

```
extern "netdb.h"
    function freeaddrinfo(ai: ptr consumes)
    function freehostent(h: ptr consumes)
    function getaddrinfo(node: cstr, service: cstr, hints: ptr, @out: ptr acquires freeaddrinfo) -> i32
    function getipnodebyname(name: cstr, af: i32, flags: i32, @err: i32) -> ptr acquires freehostent
    function gethostbyname(name: cstr) -> ptr borrows
```

**Swap the two `acquires` and it still checks 0 and runs 0.** The same swap on
handle types is `error[unread_releaser]` twice.

**And obeying the wrong word is not theoretical.** Against real libc, under
AddressSanitizer, twice:

```
AddressSanitizer: SEGV on unknown address 0x47414d534c39
    #2 freeaddrinfo+0x20 (libsystem_info.dylib)
```

**Why it cannot fire on a `ptr`.** `consumed_types` keys on a declaration index
and `is_handle` answers only for a record, so `releaser_reads`'s discriminating
test has nothing to compare.

**Does a type-keyed rule refuse a CORRECT binding? No, and the seat says so
plainly.** Every real binding it wrote had a writable mark, and the vocabulary is
arithmetically complete. **Panel 148's objection does not return in its literal
form.** It returns weaker: the rule compels a word that is **unverifiable on this
type**, where the identical word on a handle is verified. Part 6 already ruled on
that shape.

## The route nobody listed, and this seat COMPILED it

Defect 037 says *"`ptr` is the only spelling that works"* because `netdb.h` has no
typedef for `addrinfo`. The seat verified that half three ways. **But the obstacle
is the SURFACE, not C.** It took the emitter's own `--emit-c` output for `record
AI tag addrinfo` and substituted `struct addrinfo` for the tag, nothing else:

```
clang -std=gnu11 -Wall -Werror=incompatible-pointer-types … -o handle-structtag
./handle-structtag
resolved through a handle
run exit: 0
```

Zero diagnostics. **And in that same emission the cross-wire becomes a hard
compile error:**

```
error: incompatible pointer types passing 'struct servent *' to parameter
       of type 'struct addrinfo *' [-Werror,-Wincompatible-pointer-types]
```

**`netdb.h` declares 19 `struct X *`-returning entry points and zero `void *`
ones.** Let a `tag` name a struct tag and **defect 037's briefed case closes with
no new diagnostic**: `unmarked_handle_producer` already demands the mark,
`unread_releaser` already reads it, `type_mismatch` already refuses the
cross-wire. *"The option set handed to this sitting was short by one, and the
missing one is the only route that increases what clang checks."*

## What a `ptr` rule cannot see, measured from one emission with one type changed

```
ptr:     probe(…, void * a2, void * * a3) { getaddrinfo(a0,a1,a2,(void *)a3); }
handle:  probe(…, void * a2, addrinfo * * a3) { getaddrinfo(a0,a1,a2,a3); }
```

**The `ptr` probe writes an explicit cast that silences the very check §4.19
exists to perform.** So a `ptr` rule cannot see which C type it holds, therefore
cannot see which releaser is right, therefore **cannot read the word it makes
mandatory**.

On `ai_next`: the walk stops dead and **it does not matter**, because
`freeaddrinfo` frees the whole chain in one call. Panel 149's *one mark is one
obligation on the whole value* is exactly right for `ptr` too.

## A class this seat files rather than leaves in prose, and it is NOT `ptr`-specific

**A producer that fails returns NULL — the C convention for all of them — and
`acquires` counts the NULL:**

```
lib = dlopen(path: "/no/such/library.dylib".cstr(), mode: 2)
if lib == nullptr
    print("handled the failure correctly")   # abort 134, "1 C handle never given back"
```

**A handle producer returning NULL does the same.** *Coordinator's confirmation,
2026-09-15: re-run on a handle type. The program prints* handled the failure
correctly *and then aborts at 134.* **And `examples/curl/main.hero:57` never
checks `curl_easy_init()` for NULL**, which is the only reason the corpus is
silent. Recommended for filing today whatever the panel decides on 037.

## Q2 — the instrument is pointed the wrong way

| program | C level | Heroes exit | what the user is told |
|---|---|---|---|
| one mark, whole-value release | correct | **0** | — |
| one mark, four element releases | **memory-safe**, ASan clean | **134**, `+3` | two causes, **neither is this** |
| one mark, elements **and** composite | **double free** | **133** | **0 bytes stdout, 0 bytes stderr** |
| four element producers, four releases | correct | **0** | — |

**It aborts the program that corrupts nothing and says nothing whatsoever about
the one that double-frees**, because `malloc` traps before the exit counter runs.

**The veto: releasing elements IS sometimes correct.** FFmpeg ships
`AVBufferRef *buf[AV_NUM_DATA_POINTERS]` inside `AVFrame` with **both**
`av_buffer_unref` per element and `av_frame_unref` whole-value; raylib ships
`UnloadFontData(glyphs, glyphCount)` beside `UnloadFont`. Both correct, on
different objects. **`UnloadFontData` carries an explicit count — C does not know
the number either, it makes the program carry it.** *"Refusing the safe program
while the corrupting one walks is §1.12 inverted."*

**Two facts from the compiler and the headers.** A `T[N]` cannot be built in
Heroes at all (`error[fixed_outside_a_group]`), so a `Slot[4]`'s elements always
come from C. And **5614 installed headers scanned** — the first pass read 3400,
because `grep -r` does not follow Homebrew's symlinks — giving 33 distinct
fixed-arrays-of-pointers, and for every element type found, **zero** per-element
releasers.

**The seat's own framing**: a program releasing elements is correct exactly when
the elements came from their own producer, which is a fact about **which call
filled the array** — invisible to any mark on either declaration. **Same axis
panel 147 settled: the obligation is created by a call.**

## ABI

**Untouched.** Emitted C, marked versus unmarked, `#line` stripped: **two lines**,
one zero-argument call per acquiring site. `_Static_assert` and `#include` sets
byte-identical. `examples/` at HEAD: **0** `-> ptr` results, **0** `@x: ptr`,
**0** `ptr consumes`.

## Prediction

**Under a type-keyed `ptr` rule the mismatched-deallocator class stays open for
`ptr`, and the shipped golden proves it by silence.** Add the `ptr` twin of
`fixedbugs-a-releaser-that-ends-nothing`'s own two cases and the annotation will
not fire, where the handle lines above it fire four times:

```sh
heroes check tests/golden/check/fixedbugs-a-releaser-that-ends-nothing.hero 2>&1 \
  | grep -c unread_releaser
```

Reads **4** today, and will still read 4 after the `ptr` cases are added, unless
the landed rule gives `ptr` a key `releaser_reads` can discriminate on.

## Conditions

- **Q1's objection withdraws** if the rule is keyed on something narrower than
  *every `ptr`*, so `unread_releaser` can still discriminate; **or if the route
  nobody listed lands first**, so `ptr` stops being the only spelling for the case
  the defect was briefed on.
- **Q2's veto withdraws** if the refusal is narrowed to *elements AND composite in
  one program* — the only shape the seat could make corrupt — rather than to the
  element release as such.
