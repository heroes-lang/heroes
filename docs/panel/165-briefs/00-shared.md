# Panel 165 — shared brief

**The question.** Panel 164 listed seven routes for a fixed byte field crossing
to C, adopted routes 3 and 4, refused 1, 2 and 5, and **queued route 6 behind a
measurement**. The measurement is run. Should route 6 be adopted, refused, or
left queued?

**Route 6, verbatim from panel 164:** *"declare the extent on the PARAMETER,
`function arr_len(s: i8[8])`, which is how C spells it. Refused today at
`error[ffi_type]`. It needs no new expression, no lend, no built-in and no
position rule — one widening of §4.19's parameter list, with ordinary type
identity doing the matching. It is the only route of the seven where the compiler
CHECKS the extent instead of trusting the author or the callee."*

---

## Everything below was RUN while this brief was being written, 2026-09-19

Commands are given so a later reader can re-run them. Nothing here is copied from
a document; where a figure comes from an artifact produced earlier in this same
session, the artifact and its command are named.

### 1. What the compiler says today — and it is TWO refusals, not one

```
$ ./heroes check r6.hero          # extern "r6.h" { function arr_len(s: i8[8]) -> i64 }
                                  # main: b: i8[8] @ [72, 105, 0, 0, 0, 0, 0, 0]
error[ffi_type]: `i8[8]` cannot cross the FFI boundary, and it is an `extern`'s
  parameter — a C header can declare a number, `bool`, `str`, `ptr`, `cstr`, a
  function type as a parameter, and a `record` declared in this same group (§4.19)
  at r6.hero:2:25

error[fixed_outside_a_group]: `i8[8]` is a C array member, so it belongs to a
  `record` inside an `extern` group — this record's layout is this language's,
  and a fixed array exists to match one a C compiler chose (§4.19)
  at r6.hero:5:8
```

**Panel 164 named the first and not the second.** A fixed array is not a type a
binding may have outside an `extern` record, so route 6's argument can only ever
be **a record's field**. With the argument written as a field the second error
disappears and exactly one remains:

```
$ ./heroes check r6b.hero         # record Slot tag slot { name: i8[8]; id: i32 }
                                  # print(to_str(arr_len(t.name)))
error[ffi_type]: `i8[8]` cannot cross the FFI boundary, and it is an `extern`'s
  parameter …
  at r6b.hero:5:25
```

So panel 164's *one widening* is accurate **only for the field-argument shape**,
and the sitting did not say so. Whether route 6 should also admit a standalone
`i8[N]` binding is a question this sitting has to answer, not assume.

### 2. The same program already runs, under the route that was adopted

```
$ ./heroes run r3.hero
2
```

`r3.hero` is the identical program with the extent given at the call site:
`arr_len_p(p: t.name.ptr(), n: 8)` against
`long arr_len_p(const void *p, long n)`. Route 3 shipped at M-readable-bytes and
`spec § 13` states it: *"`f.ptr()` lends a binding's field to a `ptr` parameter
the call gives the extent to, and C may write back through it."*

**So route 6's marginal value is exactly this**: the extent moves from the call
site into the parameter's type, and the compiler checks it instead of the author
restating it. Nothing else changes. Seats should price that difference and not
the crossing, which already works.

### 3. The census — how many real headers spell a parameter as an array

Instrument: `scratchpad/extents/census.py`, two stages. **Stage one is a text
screen** over every `.h`; **stage two runs clang per candidate and reads the
header's own bytes back through each `ParmVarDecl`'s source range.**

The AST alone cannot answer this, and that was measured first:

```
$ clang -fsyntax-only -Xclang -ast-dump=json probe.c
f_ptr    {"qualType": "char *"}        # void f_ptr(char *b);
f_arr8   {"qualType": "char *"}        # void f_arr8(char b[8]);
f_arr2d  {"qualType": "char (*)[8]"}   # void f_arr2d(char b[4][8]);
```

C adjusts `T a[N]` to `T *a` (C11 §6.7.6.3p7), so the extent is not part of the
type. Only the outermost dimension decays, which is why the 2D row keeps a bound.

Coverage is reported rather than assumed. The first stage-two pass parsed **40 of
127** candidates; the 87 failures were diagnosed and **80 wanted a prerequisite
header**, not C++ and not Objective-C, so a thirteen-header prelude took it to
127/127.

```
                                   Darwin   Linux arm64   Linux x86-64
  .h files walked                    3120          5404           5411
  candidates / parsed / failed    127/127/0     333/333/0      334/334/0
  parameters spelled as an array       82           182   182, the same SET
  with a fixed extent                  31            59             59
  fixed, base system headers only      20            25             25
  fixed AND byte-typed                 12            31             31
  byte-typed and fixed on BOTH          -             0              0
```

Re-derived while writing this brief:

```
$ python3 extents.py
Darwin  42 UNSIZED  24 FIXED-LITERAL  9 SIBLING  7 NAMED
Linux  115 UNSIZED  45 FIXED-LITERAL  8 EXPR    14 NAMED
$ python3 -c '<intersection>'
byte+fixed Darwin 12 Linux 31 intersection EMPTY
fixed on both platforms: ['erand48','futimens','jrand48','lcong48','nrand48','pipe','seed48','utimensat']
```

All 14 Linux `NAMED` extents were opened and read and every one is a macro
constant (`L_tmpnam`, `IF_NAMESIZE`, `SEED_BLOCK_SIZE`, `CAMELLIA_BLOCK_SIZE`,
`LLVM_BLAKE3_KEY_LEN`). All 8 `EXPR` extents name a **sibling parameter**
(`BROTLI_ARRAY_PARAM(data_size)`, `_REGEX_NELTS(__nmatch)`) and are counted with
the siblings.

The two Linux legs were differenced **as sets**, not compared as counts: 0 rows
on each side, after normalising the `<triple>/` directory.

Two thirds of Linux's lead is the distribution, not the platform: the container
carries **140** OpenSSL headers and the Xcode SDK carries **0**
(`ls … | wc -l` on each), and 20 of Linux's byte cases are one file's
`unsigned char ivec[16]`.

### 4. The divergence, which is the finding rather than the count

```
Darwin  _stdio.h:289   char *_LIBC_CSTR tmpnam(char *_LIBC_COUNT(L_tmpnam));
glibc   stdio.h:211    extern char *tmpnam (char[L_tmpnam]) __THROW __wur;

$ ./sizes                (C, compiled and RUN on each platform)
Darwin        L_tmpnam=1024  IF_NAMESIZE=16
Linux glibc   L_tmpnam=20    IF_NAMESIZE=16
```

One function, one C standard, **two spellings and two numbers**. `if_indextoname`
is the same shape reversed: glibc writes `char __ifname[IF_NAMESIZE]`, Darwin's
`net/if.h:448` writes `char *if_indextoname(unsigned int, char *)`.

And the language already reaches the portable number. Run on both platforms, the
second by a compiler built from the seed inside the Linux container
(`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0):

```
extern "stdio.h"
    constant L_tmpnam: i64

Darwin  ->  L_tmpnam=1024 IF_NAMESIZE=16
Linux   ->  L_tmpnam=20   IF_NAMESIZE=16
```

### 5. Principle 0, counted from this tree

```
$ grep -rhn '^    function [a-zA-Z_]' --include='*.hero' . | sed -E 's/.*function ([a-zA-Z0-9_]*).*/\1/' | sort -u | wc -l
172
```

**172** `extern` functions are declared in this repository. Intersected with both
censuses, **exactly one** is spelled with an array parameter by a real header:
`tmpnam`, glibc only. That declaration is
`tests/golden/check/owned-freer-must-be-declarable.hero:37`, which declares
`buffer: ptr` and exists to provoke `freer_arity`. **No working binding in this
tree would change.**

### 6. What this brief does NOT know, stated as questions

- **Windows is unrun.** Two platforms of four were walked. The box was powered on
  while this sitting was convened and was not reachable on the tailnet
  (`offline, last seen 1d ago`); a poll is running. **Any seat that wants to
  argue from four platforms must treat the Windows column as absent, not as
  agreeing with the others.**
- **The screen's false-negative rate is not measured.** A declaration produced
  entirely by a macro body would be attributed to the macro's own file. None was
  seen; none was looked for with an instrument.
- **`_LIBC_COUNT` was not counted as a spelling.** Darwin states extents in an
  annotation on a pointer, 9 times in this corpus. Whether Heroes should read
  that annotation is a **different route that no sitting has listed**, and a seat
  is free to list it.
- **Nobody has measured what route 6 would CATCH.** The census counts headers
  that spell an extent. It does not count programs that would pass the wrong
  field. If you can measure that, it is the number this sitting most lacks.

### Working rules for this sitting

- **Build in a copy.** `cp -r` the tree to your scratchpad, then `rm -rf target
  build`, and work there. The seed builds in about 3 s
  (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`); rebuilding from
  `selfhost/` is ~20 minutes and will kill you on the watchdog.
- **Never `archive/bootstrap-rs/`.** Nothing builds it.
- The working tree is frozen from now until the synthesis is written.
- **Contradict this brief where you can run something that refutes it.** Six of
  panel 163's premises were false and every one was written by a coordinator who
  had a shell and did not use it.
