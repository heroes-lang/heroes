# Panel 165 — ffi-pragmatist

**verdict: object.** Not a veto: route 6 does not touch the C ABI, and I measured
that rather than assumed it. It is an objection because route 6's stated reason
for existing is false at the C boundary — measured three ways — and because the
sitting's own premise, *"you are pricing a difference, not a capability"*, is
false for every element type that is not a byte, which is where the real FFI cost
of this milestone actually sits.

**section:** design.md §1.11 (the founding constraint), §4.19 (FFI), §1.12
(robustness, via CLAUDE.md § Precedence rank 3), and spec § 13 for `f.ptr()`.

Everything below was run on this Mac on 2026-09-19, in a `cp -r` of the tree at
`…/scratchpad/tree`, seed built in **3.08 s real / 3.00 user / 0.07 sys**
(`/usr/bin/time -p clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`).
The C lives in `…/scratchpad/ffi/` and `…/scratchpad/ffi/exp/`. Every clang
invocation used the **exact** flag list from `selfhost/cli/flags.hero:92-109`,
wrapped as `exp/hf.sh` so it can be re-run.

---

## 1. There is no ABI difference, and no C-level difference of any kind

The brief's first question. Answer: **the entire difference is on the Heroes
side.** Three call forms, compiled at `-O0` and diffed as instruction bodies:

```c
/* exp/abi.c */
long arr_len(const char s[8]);
struct slot { char name[8]; int id; };
long via_ptr(struct slot *t) { return arr_len((void *)&t->name); }              /* route 3 today */
long via_i8 (struct slot *t) { return arr_len((const char *)(int8_t *)t->name); } /* route 6, int8_t[8] */
long via_ch (struct slot *t) { return arr_len(t->name); }                        /* route 6, char[8] */
```

```
$ clang -std=gnu11 -O0 -S abi.c -o abi.s   # then diff the three bodies
route3 vs route6-i8:   IDENTICAL
route3 vs route6-char: IDENTICAL
```

So the answer to *"say whether anything here touches the same ground"* as the
`cstr` decay is **no**. My refusal there stands and is not engaged here. I am not
voting my veto.

## 2. The reason route 6 was proposed is false, and clang says so

Panel 164: *"It is the only route of the seven where the compiler CHECKS the
extent instead of trusting the author or the callee."*

**At a C parameter the extent does not exist.** C11 §6.7.6.3p7 adjusts `T a[N]`
to `T *a`, and I asked clang directly instead of reading the standard at it:

```c
/* exp/typeq.c */
long arr_len(const char s[8]);
_Static_assert(_Generic(&arr_len, long (*)(const char *): 1, default: 0),
               "the declared type is long(*)(const char *) -- extent GONE");     /* PASSES */
_Static_assert(_Generic(&arr_len, long (*)(const char (*)[8]): 1, default: 0),
               "the extent is NOT part of the parameter type");                  /* FAILS  */
```

```
typeq.c:7:16: error: static assertion failed: the extent is NOT part of the parameter type
```

There is therefore **no `_Generic`, no `__typeof__`, no `_Static_assert` and no
probe** by which Heroes could ever ask a header what extent it declared on a
parameter. That is not a limitation of the current probe; it is a fact about C.

Then the probe itself, written exactly as `selfhost/emit/extern_probe.hero` writes
one, with its four `#pragma clang diagnostic error` names, compiled against both
real spellings and at five declared extents:

```
                                       exit  warnings  errors
  probe int8_t a0[8]   vs  const char s[8]      0      1       0
  probe int8_t a0[4]   vs  const char s[8]      0      1       0    <-- WRONG, not caught
  probe int8_t a0[1]   vs  const char s[8]      0      1       0    <-- WRONG, not caught
  probe int8_t a0[999] vs  const char s[8]      0      1       0    <-- WRONG, not caught
  probe int8_t a0[]    vs  const char s[8]      0      1       0
  probe int8_t a0[8]   vs  const char *s        0      1       0
  probe int8_t a0[4]   vs  const char *s        0      1       0    <-- WRONG, not caught
```

The one warning is the same in all seven rows and it is **`-Wpointer-sign`**, which
fires on the *correct* binding too:

```
r6.hero:2:97: warning: passing 'int8_t *' (aka 'signed char *') to parameter of type
  'const char *' converts between pointers to integer types where one is of the unique
  plain 'char' type and the other is not [-Wpointer-sign]
```

`-Weverything` adds nothing but `-Wpoison-system-directories`. And clang warnings
reach the author raw, with generated C in the message — measured on the live
compiler with a `cstr`/`const unsigned char *` binding:

```
$ ./heroes run ps.hero
ps.hero:2:94: warning: passing 'const char *' to parameter of type 'const unsigned char *' …
    2 | __attribute__((unused)) static void hero_ffi_probe_h_ps_ulen(const char * a0) { (void)(ulen)(a0); }
5
```

So route 6 as spelled would put a warning on **every correct `i8[N]` parameter
binding**. The escape is to spell the probe parameter `char a0[8]`, which I
compiled clean against both header spellings — but that makes `i8` mean `char` at
exactly one position in the emitter, and `selfhost/emit/extern_field.hero:84`
already records in its own comment why that is wrong: *"C's char is a THIRD type,
so char[N] matched int8_t (\*)[N] at none of the eight widths."*

## 3. What route 6 would actually check: the author against himself

It would check the *argument's* extent against the *parameter's declared* extent —
both Heroes-side numbers. The header is not consulted, because it cannot be.
I built the consequence and ran it:

```c
/* exp/wrong6.c — the author wrote 8 where IF_NAMESIZE is 16 */
char *g_indextoname(unsigned int i, char n[IF_NAMESIZE]);   /* the glibc spelling  */
__attribute__((unused)) static void probe_darwin(uint32_t a0, int8_t a1[8]) { (void)(if_indextoname)(a0, (char *)a1); }
__attribute__((unused)) static void probe_glibc (uint32_t a0, int8_t a1[8]) { (void)(g_indextoname )(a0, (char *)a1); }
struct small { char n[8]; };
...  char *r = g_indextoname(1, s.n);
```

```
$ ./hf.sh -c wrong6.c -o /dev/null      # project flags
compile exit=0            (zero diagnostics, BOTH spellings)

$ ./wrong6_plain ;  echo $?             # plain build
134                                     (stack smashed)

$ ./hf.sh -fsanitize=address,undefined wrong6.c -o wrong6 && ./wrong6
==12870==ERROR: AddressSanitizer: stack-buffer-overflow
WRITE of size 16 at 0x00016f49e3a8 thread T0
    #1 0x00018f47fb80 in if_indextoname+0x9c (libsystem_info.dylib)
  This frame has 1 object(s):  [32, 40) 's' <== Memory access at offset 40 overflows
```

A 16-byte write into an 8-byte buffer, against the real `net/if.h`, with route 6's
probe compiling silently. **This is the exact defect class route 6 is advertised
to close, and it does not close it.** design.md §1.12 makes *a Heroes program must
not segfault and must not corrupt memory* a goal of the language; a route that
reports the check as performed while this compiles is worse than no route, because
CLAUDE.md § 12 holds a refusal to a feature's standard and a *guarantee* to a
higher one.

**The one C spelling that could have worked, and Darwin has zero of it.**
`[static N]` is the only form where clang checks a size, and only when the argument
is a real array object rather than a parameter:

```
  probe passes a pointer parameter, header [static 16]  -> nothing
  probe passes the real array member,  header [static 16]
      -> warning: array argument is too small; contains 8 elements,
         callee requires at least 16 [-Warray-bounds]
  probe passes the real array member,  header [16] (no `static`) -> nothing
```

```
$ find "$(xcrun --show-sdk-path)" -name '*.h' | wc -l                              11477
$ grep -rn '\[[[:space:]]*static[[:space:]]' "$SDK" --include='*.h' | wc -l             0
$ grep -rn '\[[[:space:]]*static'            "$SDK/usr/include" --include='*.h' | wc -l 5
      (all five are C++ `[static_cast<…>`, not C array parameters)
```

**Zero C array parameters in the whole Darwin SDK use `[static N]`.** And Darwin's
own annotation is not a fallback: `_LIBC_COUNT` **expands to nothing** under this
project's dialect —

```
$ printf '#include <stdio.h>\nchar *t(char *_LIBC_COUNT(L_tmpnam));\n' | clang -E -P -
char *t(char *);
```

— so the shared brief's open question about reading it is answered: there is
nothing left to read by the time Heroes' clang sees the declaration.

## 4. Route 4 already does what route 6 claims, and against the header

The inversion is the finding. Emitted C for `r3.hero`, line 14:

```c
_Static_assert(_Generic(&((struct slot *)0)->name,
    char (*)[8]: (sizeof(char) == sizeof(int8_t) && …), …, default: 0),
    "heroes-ffi-field Slot name");
```

The extent **is** in the type at a field (`char (*)[8]`), which is why the field
route can check it and the parameter route provably cannot. Run on the tree today,
against the real headers:

```
$ ./heroes run ifbad.hero          # declared i8[8]; net/if.h has char ifr_name[16]
error[ffi_field_type]: `Ifreq.ifr_name` is not `i8[8]` in `net/if.h` — clang read
  the header's struct and the field disagrees

$ ./heroes run fdsbad.hero         # declared i32[3]; the header has int fd[2]
error[ffi_field_type]: `Fds.fd` is not `i32[3]` in `fdscheck.h` — …
```

So panel 164's sentence should read: **route 4 is the route where the compiler
checks the extent against the header; route 6 is the route where it checks the
author against the author.**

## 5. The brief's `if_indextoname` case needs nothing new — measured

Darwin `net/if.h:448` is `char *if_indextoname(unsigned int, char *)`; glibc spells
it `char __ifname[IF_NAMESIZE]`; `IF_NAMESIZE` is `16` at `net/if.h:66` and
`IFNAMSIZ` is `#define IFNAMSIZ IF_NAMESIZE` at `:84`. The binding, whole:

```
extern "net/if.h"
    constant IF_NAMESIZE: i64
    record Ifreq tag ifreq partial
        ifr_name: i8[16]
    function if_indextoname(ifindex: u32, ifname: ptr) -> cstr

function main()
    r: Ifreq @ Ifreq(ifr_name: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    print(to_str(IF_NAMESIZE))
    got = if_indextoname(ifindex: 1, ifname: r.ifr_name.ptr())
    name = got.validated()
    print(name.must())
```

```
$ ./heroes run ifname.hero        $ ./truth        (the same thing in plain C)
16                                 16 lo0
lo0
exit=0
```

It writes back through the lend, it reaches the portable `16` through
`extern constant`, it is portable across both spellings because neither spelling
survives into the type, and the compiler refuses the wrong extent (§4 above).
Route 6 would delete the word `.ptr()` and add nothing. Worth noting what the
compiler said while I wrote it, because it is the case *for* the current design:
it refused `ifindex: i64` with `error[ffi_parameter_type]` and a `guess` fix
*declare it `u32`*, and refused the incomplete `ifreq` with
`error[ffi_incomplete_record]` naming `ifr_ifru`. Two real bugs caught by the
route that already ships.

## 6. Where the sitting is actually wrong: `pipe`

The shared brief says *"you are pricing a difference, not a capability"*. **Run
it.** `pipe` is one of the eight functions the brief itself lists as fixed-spelled
on *both* platforms, Darwin `unistd.h:482` `int pipe(int [2])`:

```
$ ./heroes run pipe.hero
error[bad_operand]: `ptr` takes a fixed run of bytes — `i8[N]` or `u8[N]`, found `i32[2]`
  at pipe.hero:13:23
```

Route 3 **does not cross** `int[2]`. `selfhost/check/lend_types.hero:93` narrows
`f.ptr()` to bytes. The field itself is fine — `record Fds tag hero_fds { fd: i32[2] }`
checks against `int fd[2]` and runs — so the narrowing is in the *lend*, not in the
layout. What a binding author must write today:

```
extern "hero_fds.h"                 # a struct invented to lie about the type
    record Fds tag hero_fds
        b: i8[8]
extern "unistd.h"
    function pipe(fildes: ptr) -> i32
```
```
$ ./heroes run pipe8.hero
0
3
```

That runs, and it costs the author a fabricated header, a fabricated struct, and
hand-reassembly of two `int`s out of eight bytes. **This is the FFI cost of the
milestone, and route 6 as spelled (`i8[8]`) does not pay it either.** Of the eight
both-platform fixed functions the brief names — `erand48 futimens jrand48 lcong48
nrand48 pipe seed48 utimensat` — **none is a byte array**: five are
`unsigned short[3]`/`[7]`, two are `const struct timespec[2]`, one is `int[2]`.

**A question for the spec-warden, not a claim of mine.** spec § 13 says
*"`f.ptr()` lends a binding's field to a `ptr` parameter the call gives the extent
to"* and, three lines above, that a field may be *"a fixed array of one: `i32[4]`"*.
The compiler says bytes only. Under CLAUDE.md § 12 that reads as spec-beats-compiler,
but I did not grep `docs/records/log/` for a ruling that narrowed it deliberately,
so I file it as unrun rather than as a defect.

## 7. The shapes beside it, compiled

```c
/* exp/shapes.c — all six assertions PASS */
_Static_assert(_Generic(&f_fixed,   long(*)(const char *):1,            default:0), "[8]        -> char *");
_Static_assert(_Generic(&f_static,  long(*)(const char *):1,            default:0), "[static 8] -> char *");
_Static_assert(_Generic(&f_unsized, long(*)(const char *):1,            default:0), "[]         -> char *");
_Static_assert(_Generic(&f_2d,      long(*)(const char (*)[8]):1,       default:0), "[4][8]     -> char(*)[8]");
_Static_assert(_Generic(&f_const,   long(*)(const char *const):1,       default:0), "[const 8]  -> char *const");
_Static_assert(_Generic(&f_vla,     long(*)(int, const char *):1,       default:0), "[n]        -> char *");
```

Only a 2D parameter keeps a bound, and it keeps the **inner** one. So route 6's
type identity would match `[8]`, `[static 8]`, `[]` and `[n]` against each other
and against `*`, indiscriminately — and 115 of Linux's 182 are `[]`, which route 6
has no spelling for at all, since `[i8]` is the dynamic array and cannot cross.
Route 6 covers at most 59 of 182 on Linux and 31 of 82 on Darwin, and of Darwin's
12 byte-typed fixed cases I re-derived the brief's own number and then split it:
**11 of 12 are in bundled third-party headers** (`cups/`, `sasl/`, `apr-1/`) and
exactly one — `fparseln`'s `const char[3]` — is in a base header. The brief's
census reproduces; its distribution is worse for route 6 than the brief said.

## 8. What I would take instead

Not a compromise — the more complete repair, per CLAUDE.md § 4. **Widen `f.ptr()`
from `i8[N]|u8[N]` to any fixed array whose element has a C spelling.** It is one
`allowed:` set at `selfhost/check/lend_types.hero:93`; the field assertion already
handles every integer width (measured on `i32[2]`); there is no new boundary type,
no new parameter form, no probe change, no `-Wpointer-sign`, and no ABI change.
It binds `pipe`, `erand48`, `seed48`, `nrand48`, `jrand48` — and with a record
element, `futimens` and `utimensat`, which is seven of the eight portable cases
that route 6 as spelled binds zero of.

The caller-side half route 6 *does* close is real — route 3's call-site extent is
unchecked, and I made it corrupt memory to be sure:

```
$ ./heroes run r3over.hero              # n: 64 over an i8[8] field
8372224                                  exit=0, a garbage number
$ ./heroes run r3over.hero --sanitize
==13376==ERROR: AddressSanitizer: stack-buffer-overflow … READ of size 1
```

But that half is closable without widening the boundary: `t.name.len()` is
`error[bad_operand]: len takes str, [T] or {K: V}, found i8[8]` today, and making
`len` on a fixed field a compile-time constant would let the call site derive the
number it currently restates. Smaller than route 6, and it leaves the boundary
type set untouched.

---

## prediction, falsifiable

**If route 6 is adopted as panel 164 spelled it, then of the eight functions the
shared brief names as fixed-spelled on both platforms — `erand48`, `futimens`,
`jrand48`, `lcong48`, `nrand48`, `pipe`, `seed48`, `utimensat` — exactly ZERO will
bind through it without a shim, because none has a byte-array parameter; and a
route-6 binding of `if_indextoname` that declares `i8[8]` where `IF_NAMESIZE` is
16 will compile with zero diagnostics on both Darwin and glibc and write 16 bytes
into 8.**

Score it by writing the eight bindings. Any one of them compiling and running
through a route-6 parameter without a fabricated struct falsifies the first half.
A compile error on the `i8[8]` `if_indextoname` binding falsifies the second.

## condition — what would change my verdict to approve

Any one of these, measured:

1. A C construct I missed that recovers a parameter's declared extent from a
   header — a `_Generic`, a builtin, an attribute clang honours in C11. §2 above
   is my evidence that none exists; produce one and route 6's stated reason
   becomes true and I approve it the same day.
2. Route 6 widened to **every** element type a fixed field may have, *and* emitted
   so the probe passes the real array member rather than a pointer parameter, *and*
   the diagnostic that fires when the parameter's extent disagrees with the header
   named — with `pipe` and `utimensat` compiled and run under it.
3. A count of programs route 6 would have caught (the shared brief's own open
   question, §6 of that document). If passing the wrong field is a measured
   defect class in this repository's corpus, the caller-side half is worth more
   than I priced it and the objection becomes a preference.

Absent all three, route 6 should be **refused with its reason recorded**, and the
milestone's FFI budget spent on §8's widening of `f.ptr()`, which is where a real
binding is blocked today.
