# Panel 165 — brief for the ffi-pragmatist

**Read `docs/panel/165-briefs/00-shared.md` first.** Everything in it was run,
including the two programs below, which are yours to attack.

## Your question

Route 6 declares an `extern` parameter as a fixed array, `function arr_len(s:
i8[8])`, and has the compiler check the extent. **Write the C, compile it, and
say whether the route is sound at the boundary** — and whether it buys anything
route 3 does not.

## What already runs, so you are pricing a difference and not a crossing

```
$ ./heroes run r3.hero
2
```

```
extern "r3.h"
    record Slot tag slot
        name: i8[8]
        id: i32
    function arr_len_p(p: ptr, n: i64) -> i64
    function slot_make() -> Slot

function main()
    t = slot_make()
    print(to_str(arr_len_p(p: t.name.ptr(), n: 8)))
```

Route 6 would write `arr_len(t.name)` against `long arr_len(const char s[8])`.

## The things only you can settle

1. **What does Heroes emit, and what does clang see?** A C parameter written
   `const char s[8]` IS `const char *s` — C11 §6.7.6.3p7, measured in the shared
   brief. So the emitted call is the same call either way. **Is there any C-level
   difference at all between route 3 and route 6, or is the entire difference on
   the Heroes side?** Compile both and compare the emitted C.
2. **The probe.** `selfhost/emit/extern_probe.hero` checks declarations against
   the real header. Panel 164 recorded that four `#pragma clang diagnostic error`
   flags are set there (lines about 77–80) and that `cstr` into
   `const unsigned char *` emits `-Wpointer-sign`, which is **not** among them.
   **Does a fixed-array parameter probe cleanly against a header that spells the
   parameter as a pointer, and against one that spells it as an array?** Both
   spellings exist for the same function across platforms — that is measured.
3. **Write direction.** Route 3 was widened to `@`-marked `ptr` parameters so C
   can fill a field. Would route 6 need an `@` form too, and what does
   `char buf[16]` as an out-parameter do that `ptr` does not?
4. **The one case that looks made for this route.** `net/if.h`'s
   `if_indextoname(unsigned int, char __ifname[IF_NAMESIZE])` on glibc, with
   `struct ifreq.ifr_name` being `char[IFNAMSIZ]`. Measured on both platforms:
   `IF_NAMESIZE == IFNAMSIZ == sizeof(ifr_name) == 16`. **Bind it and run it**,
   both ways, and say what route 6 gives the author that route 3 does not.
   Darwin spells the same function `char *if_indextoname(unsigned int, char *)`,
   so the binding must work against both spellings or the route is not portable.
5. **Attack the shapes beside it.** `[]`, `[static N]`, `[N][M]`,
   `[_LIBC_COUNT(n)]`, `const` versus not, a 2D field, and an extent that is a
   macro whose value differs by platform. The census says 115 of Linux's 182 are
   unsized; what does route 6 say about those, and is a partial route worse than
   none?

## What you hold a veto on

ABI breakage. Panel 164 recorded your words on the `cstr` decay — *"there is no
ergonomic gain I will trade for it"* — and that refusal stands whatever this
sitting decides. Say whether anything here touches the same ground.

## Build guidance

`cp -r` the tree to your scratchpad, `rm -rf target build`, work there. The seed
builds in about 3 s: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`.
Never `archive/bootstrap-rs/`.

## What your verdict must carry

A verdict (approve / object / veto), the C you compiled with its diagnostics, the
design.md sections it rests on, and **one falsifiable prediction** stated so a
later sitting can score it.
