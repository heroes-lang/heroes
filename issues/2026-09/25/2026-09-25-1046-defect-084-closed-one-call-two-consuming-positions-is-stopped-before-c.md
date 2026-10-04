---
kind: defect
area: emit
milestone: none
filed: 2026-09-23
commit: 517b8e25692a4cf0075c04fd306e5017fd428cd4
github: none
---

# Defect 084 closed: one call taking a handle at two consuming positions is stopped before C, says so, and the binding is a shim

2026-09-25, M-agreed-retention step 11, in lane `9f813de2`, merged `62324531`:
panel 176's ruling, *a message and a shim, not a word*. Found by panel 176's
ffi-pragmatist (its § 5 item 1).

- [x] **084 — one handle given to two consuming parameters of one call aborts a correct program, and the message calls it a double release** | the live set takes the handle back once per marked parameter, so `SSL_set_bio(s, b, b)`, OpenSSL's socket-BIO idiom, is refused although its C is correct | `selfhost/emit/handle_traffic.hero:101` · `runtime/parts/alloc.c:422` · **closed 2026-09-25**

    **Origin:** panel 176's ffi-pragmatist, 2026-09-23 (its § 5 item 1);
    reproduced by the coordinator the same day before filing.

    **The reproducer**, against Homebrew's OpenSSL 3 (`--include
    /opt/homebrew/opt/openssl@3/include --library /opt/homebrew/opt/openssl@3/lib`):
    `SSL_new`, then `b = BIO_new(type: BIO_s_mem())`, then
    `SSL_set_bio(s: s, rbio: b, wbio: b)` with both BIO parameters `consumes`,
    then `SSL_free(ssl: s)`. `build` 0; **run 134, three of three**, *1 C
    handle(s) given back that were never taken*. OpenSSL's own page says *"If
    the rbio and wbio parameters are the same … then one reference is
    consumed"*, and the seat measured the same C clean under ASan with 0 leaks.

    **Why it is a defect.** A correct program is refused and the message names a
    double release that did not happen: defect 079's class, over the most common
    OpenSSL call there is. The seat measured every route panel 176 weighs
    leaving it at 134, so the repair is owed whatever vocabulary lands.

## The resolution

`SSL_set_bio(s, b, b)` hands one handle to two consuming positions of one call
and the library takes ONE reference. A general *one handle, two positions*
word would be a per-function exception written into the language for one page
of OpenSSL's; the sitting refused it. What lands: the runtime counts every end
a call announces before C runs as PENDING on the entry, and a call announcing
more ends than the address holds references is stopped BEFORE the library
frees twice, with a message that names the shape and the shim (the header's
`C ran` never prints in the golden). The first shape of this landing, whose
check before C read and did not count, let the double free reach C and spoke
only afterwards; the landing's review found it. A handle holding two
references at two positions of one call runs, as C runs it
(`handle-two-references-at-two-consuming-positions`). The binding is a
one-line C shim, `ssl_set_one_bio(s, b)`, with one `transfers` position, which
the golden beside the refusal runs at 0. The transfer names what the SSL will
END the BIO with, `bio_free`: the name in `transfers` is the call the receiver
will make.

## The measurements

| program | before (trunk `fb0b7cb6`) | after |
|---|---|---|
| `run/fixedbugs-one-call-taking-a-handle-at-two-consuming-positions-says-so` | 134 after C, *the same handle given back TWICE* as the only cause | 134 BEFORE C, the message names this shape and the shim |
| `run/handle-a-shim-consumes-a-handle-once-for-two-positions` | — | **0** |
| `run/handle-two-references-at-two-consuming-positions` | — | **0** |

Panel 176's ffi-pragmatist's prediction, *`ssl_same_bio` stays 134 whatever
consuming word lands*: HELD. Linux ×2 and Windows: 134 and the box's 127,
the header's `C ran` never printed on any leg.
