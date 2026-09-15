# Panel 152 — shared brief

**Three seats, not five, under CL-023**: *convene without asking again, choosing
only the seats whose input differs*. The compiler-engineer and the
ffi-pragmatist ruled at panel 151 and their reports stand as input rather than
being re-run. You are the three who have not seen this.

## The question

**May the compiler ask clang ONE BIT about a `tag` — does this word need
`struct` in front of it? — and keep the author's word as the identity?**

## How it got here, in four sentences

Defect 037: a `ptr` producer leaks at exit 0, and the author of a `getaddrinfo`
binding has no spelling but `ptr`, because `netdb.h` has **no typedef for
`addrinfo`** and Heroes' `tag` takes one identifier where C writes two.
Panel 150 adopted *a `tag` names a C type NAME* and deliberately left the
spelling open. The coordinator then proposed letting the compiler LEARN the
spelling from clang, and **panel 151 refused it with two vetoes**: the emitter's
probe is §4.19's instrument, it works by writing the AUTHOR's word and letting
clang disagree with the header, and a probe generated FROM the header can no
longer disagree with it. **Measured: `record Db tag sqlite3_stmt` gives two
`error[ffi_parameter_type]` today and zero under that route.**

## What is proposed now, and the ffi-pragmatist compiled it

Not the type name. **One bit.**

```c
#include <netdb.h>
addrinfo *hero_tagprobe;     /* does not compile  → emit `struct addrinfo *` */

#include <curl/curl.h>
CURL *hero_tagprobe;         /* compiles          → emit `CURL *` */
```

The author still writes `tag addrinfo`. That word remains the identity, so the
probe still carries it and **every refusal survives** — compiled at panel 151: a
wrong tag is still refused, and a tag naming nothing is still refused, loudly.
Cost **0.02 s per tag**, cacheable under the existing key.

## What the two earlier seats established, so you need not re-derive it

- **Neither spelling works unconditionally.** `netdb.h` has no typedef for
  `addrinfo`, so the bare name does not compile. This Mac's SDK has `typedef void
  CURL;`, so `struct CURL` is a **fabricated incomplete type accepted with a
  warning** — panel 150 and `selfhost/handles.hero`'s own module doc both
  reasoned that it would break, and **neither had run it**.
- **`handles.c_spelling` is dead code**, zero callers; the live line is
  `selfhost/emit/ctype.hero:139`.
- **Counts, corrected**: `examples/` declares **5** handle types, not the 16 an
  earlier brief claimed; **22** handle declarations ship in all trees; **7**
  distinct tags in shipping code, and clang's answer already matches the emitted
  text for all seven, so **ABI breakage is zero**.
- **18** struct-returning entry points in `netdb.h`, and zero `void *` ones.

## Why this needs a sitting at all

**It amends design.md Part 4**, which a two-seat lane cannot touch:
`design.md:2192` says *"The tag is written **verbatim**"*, and §4.19 says **"no
external tool, no libclang"**. Both earlier seats said so independently, and that
agreement is why you were convened. **No `spec/heroes-spec.md` token moves under
either form** — `spec § 13`'s *"`record Db tag sqlite3` is `sqlite3 *`"* stays
literally true — which the spec-warden should verify rather than take from here.

## Rules that bind you

Build in a COPY — `cp -r` the tree, `rm -rf target build`, work there. The seed
builds in about 3.4 s: `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`. **Never rebuild from `selfhost/`** (20 minutes; it will kill you).
`HEROES_RUNTIME=<copy>/runtime` to build a program. There IS an
`ANTHROPIC_API_KEY` in `.env`; source it with `set -a; . ./.env; set +a`.

A claim enters your report only after the command that settles it has been run.
Where you could not run it, say so in your own words. A negative claim names what
you searched for.
