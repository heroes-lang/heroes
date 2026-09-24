# Panel 176 — ffi-pragmatist

Seat: design.md §1.11 (FFI ergonomics rank alongside comprehension, and there is
no standard library) and §4.19 (an `extern` is verified by clang against the real
header). The seat holds a veto on C-ABI breakage or on bindings made categorically
harder. Everything below was run on 2026-09-23 in
`<scratchpad>/176-ffi-pragmatist/`, a `git archive` of `a747e5a2` plus the briefs.
The compiler was built there from the seed (`clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, 3.27 s), with `HEROES_RUNTIME` pointing at that
copy. The Linux runs used `heroes-linux-arm64` (Debian clang 22.1.8, OpenSSL
3.5.7) and `heroes-linux` (x86-64, emulated), with my directory mounted read-only
and copied inside, and a compiler built from the seed in each container.
**Windows is UNRUN throughout.** "Project flags" means the sixteen flags of
`selfhost/cli/flags.hero:91-109` (`exp/cc.sh`). "Three runs" means the binary was
run three times and every run gave the number shown.

## Verdicts

| item | verdict | ground |
|---|---|---|
| **V1**, a second consuming word | **approve** | no C change; one word per transfer site; the same run-time verdicts as V2 and V3 on all 16 programs |
| **V2**, transfers inside the acquiring set | **object** (no veto) | one fact written once per acquirer: json-c goes from 15 names to 90, and libcrypto's BIO marks must name libssl's calls; it buys no verdict V1 lacks |
| **V3**, the transfer names its receiver | **object** (no veto) | +5 names, and it changes only a message, on 2 of 16 programs; one of those two messages is false |
| **R1**, a reference is its own mark | **approve, on two conditions** | it must reach a **parameter** as well as a result (35 of 120 real reference-adders return a status or nothing, including all 27 OpenSSL `_up_ref`), and on an address the program does not hold it must begin a life rather than refuse |
| **Q2, A2** (one C function, one set of marks) | **approve** (no veto) | cost: it refuses the one-module-per-mode spelling of `sqlite3_bind_text`, which runs today. The rewrite costs a lease per TRANSIENT call, or a 3-function header |
| **Q3** | see § 3 | neither answer to Q2 makes `lent_static.hero` an error. Dropping `lent` does, and so does a 3-function header shim. Only the shim keeps an exact, lease-free TRANSIENT beside STATIC in one program under A2 |
| **Q4** | **refuse** `owned` on a `const char **` cell, with an `ffi_` diagnostic at exit 1 | of 589 `const char **`-family parameters, one header comment says the caller frees. Following it is a double free |
| **veto** | **none** | no route changes a byte that crosses the boundary (§ 2a) |

---

## 1. The census: transfers, and reference-adders

### 1a. How it was searched, and what the search cannot see

- **Headers, as clang reads them.** I dumped
  `clang -fsyntax-only -fparse-all-comments -Xclang -ast-dump=json` over six units,
  so that each declaration carries the comment clang attaches to it. The Darwin
  units were `d_libc` (35 SDK headers, 2030 functions), `d_libs` (SDK sqlite3,
  zlib, curl, libxml2, libxslt, expat, ncurses, pcap, ldap and others, 3122) and
  `d_fw` (CoreFoundation, CoreGraphics, CoreText, ImageIO, CoreVideo, Security,
  IOKit, CoreAudio and AudioToolbox, 6961). The Homebrew unit was `d_brew` (json-c
  0.19, OpenSSL 3.6.4, glib/gobject/gio, cairo, harfbuzz, pango, fontconfig,
  freetype, FFmpeg, SDL3, raylib, libarchive, dbus, gpgme, libusb, leptonica,
  tesseract, poppler-glib, gnutls, libssh, nghttp2, libuv, pcre2 and others,
  26632). The Linux arm64 units were `l_libc` (1064) and `l_libs` (sqlite3, zlib,
  zstd, curl, libxml2 2.9.14, OpenSSL 3.5.7, gnutls, krb5, libssh2, ldap, z3,
  nghttp2, 11212).
- **Scripts.** `census/sigs.py` reduces each AST to one TSV row per function.
  `census/xfer.py` finds transfer-shaped calls: two or more handle parameters, not
  a releaser by name, and either a name that means "hand in" or an attached
  comment about ownership. `census/refret.py` classifies reference-adders.
- **What cannot be seen:**
  - a transfer whose receiver is a global registry, or an integer id;
  - a name outside the word lists;
  - a comment clang does not attach, such as a group comment, or documentation
    kept in `.c` files or man pages;
  - function-like macros: the brief's 124 OpenSSL names are 92 functions in the
    AST.
- **Two sidecar sources where the header is silent:**
  - GObject Introspection's `.gir` files: `census/gir.py` reads
    `transfer-ownership` per parameter for GLib, GObject, Gio, HarfBuzz, Pango and
    Poppler;
  - OpenSSL's man pages: `census/ossl_man.py` and `census/ossl_page.py`, over
    `/opt/homebrew/opt/openssl@3/share/man/man3`.
- **The classification of each transfer below comes from the header's or the
  page's own words, or from running the call.** The name counts are candidates
  and nothing more.

### 1b. Transfers found, and whether the header's own text says so

| library | the transferring calls | what the text says | where it is said |
|---|---|---|---|
| **json-c 0.19** | `json_object_object_add`, `_add_ex`, `json_object_array_add`, `_put_idx`, `_insert_idx` | the header says it: *"The reference count of `val` will *not* be incremented, in effect transferring ownership that object to `obj`"*. For the array calls it says only *"will *not* be incremented"*. **What happens on failure is not stated** (`_put_idx` has no `@return`) | header |
| **OpenSSL 3.6.4** | 124 `set0`/`add0`/`push0`/`own0` names (the brief's grep, reproduced) | of the 72 AST-visible functions with two handle parameters (92 match the name at all), **1** has an ownership comment (`SSL_CTX_set0_ctlog_store`: *"Ownership of the CTLOG_STORE is transferred"*). 97 of the 124 names have a man page, and 81 of those pages carry an ownership sentence; 27 names have no page | man pages |
| GLib, GObject, Gio, Pango (Homebrew GIR) | 5533 callables, 5327 handle in-parameters, **52** of them `transfer-ownership="full"` (1.0%). Of the 52, **12 hand a life INTO another value** (`g_option_context_add_group`, `g_task_return_error`, `pango_attr_list_insert`, `pango_fontset_simple_append`, …), 5 consume the instance while taking another, and 35 end a life | the headers: no. 3 of the 148 glib candidate rows carry any attached comment | GIR, from the gtk-doc annotations in the `.c` files |
| libxml2 (the SDK's; Linux 2.9.14) | `xmlAddChild`, `xmlAddNextSibling`, `xmlAddChildList`, `xmlDocSetRootElement`, `xmlReplaceNode` | no attached comment on any of them in either header | not read further |
| curl, fontconfig | `curl_mime_subparts`, `curl_slist_append`, `FcFontSetAdd` | no attached comment | not read further |
| SDL3 | 9 functions with a `closeio` parameter (`SDL_LoadBMP_IO`, `SDL_LoadWAV_IO`, …) | yes: *"if true, calls SDL_CloseIO() on `src` before returning, even in the case of an error"* | header |
| leptonica | 46 functions with a `copyflag` parameter (`pixaAddPix`, `boxaAddBox`, …) | the enum in `pix.h`: `L_INSERT` *"stuff it in"*, `L_COPY`, and `L_CLONE` *"make/use clone (ref count)"* | header, the enum only |
| FFmpeg | `av_packet_from_data` (bytes rather than a handle) | yes: *"If this function returns successfully, the data is owned by the underlying AVBuffer"* | header |
| CoreFoundation, xpc | `CFArrayAppendValue`, `xpc_array_append_value` | **not transfers**: *"The value is retained by the array"* (`CFArray.h`) and *"This object is retained by the array"* (`xpc.h:1246`), so the program still owes its release, and no consuming mark belongs there | header |

**So the answer to "does the header say so" is mostly no.** Of the libraries
above, json-c, SDL3, FFmpeg and Apple's retaining containers say it in the header;
OpenSSL (its headers), the GObject family, libxml2, curl and fontconfig do not. That confirms
panel 148's premise at `check/acquiring.hero:257` (*no C header states which, so
the binding author states it*). Every V route rests on that premise, and the
premise holds.

### 1c. The name does not decide, and three transfer shapes no route listed

- **`set0` is not uniformly a transfer, and this was run** (`work/q1/set0_keep.c`
  on Darwin, `linux_set0.sh` on Linux arm64). The calls are
  `X509_STORE_CTX_set0_param` and `X509_STORE_CTX_set0_untrusted`, two `set0`
  functions of one struct, each followed by the caller freeing the object after
  the ctx is freed:
  - `set0_param` is a transfer: the caller's free is an ASan *attempting
    double-free*, exit 134 on Darwin and exit 1 on Linux.
  - `set0_untrusted` is a **keep-without-owning**. The caller's free after the ctx
    is clean, and never freeing it is a LeakSanitizer leak (Linux exit 1). Its page
    says *"sets the internal pointer"*.
  - So a binding author must read each function's page, whatever the route.
- **A conditional transfer, run here.** `work/jc/fail.c` against json-c 0.19:
  - `json_object_array_put_idx(arr, SIZE_MAX, child)` returns -1, the child stays
    allocated with a refcount of 1, and the array's length is 0;
  - the same holds for `_insert_idx` and for `json_object_object_add(obj, "self",
    obj)`;
  - a caller that then frees the child is ASan-clean with 0 leaks.
  - So the life passes on success only. OpenSSL's own pages say the same of their
    calls: `CMS_add0_cert` *"on success it must not be freed up by the caller"*,
    and `ASN1_STRING_set0` *"If there is an error the optional passed in
    ASN1_STRING will not be freed"*.
- **A transfer that depends on aliasing:** `SSL_set_bio(s, rbio, wbio)`. Its page
  says *"If the rbio and wbio parameters are the same … then one reference is
  consumed"*, and `SSL_set_bio(ssl, bio, bio)` is the socket-BIO idiom. The C is
  ASan-clean with 0 leaks (`work/q1/ssl_same_bio.c`).
- **A transfer that depends on a flag:** SDL3's `closeio` (9 functions) and
  leptonica's `copyflag` (46). This is Question 3's shape, but for a handle rather
  than for bytes.

### 1d. Reference-adders: how often a real acquisition has one

**Typed, by strict name, per handle type that has both an acquirer and a releaser
(`xfer.py`):**

| unit | handle types | with a reference-adder |
|---|---|---|
| Darwin libc | 20 | 1 |
| Darwin libraries | 68 | 5 |
| Darwin frameworks | 94 | 33 (35%) |
| Homebrew | 822 | 107 (13%), plus 130 with only an ambiguous same-type `dup`/`clone`/`copy`/`get` |
| Linux libraries | 338 | 37 (11%) |
| Linux libc | 9 | 0 |

- **Generic reference-adders over `void *` reach every type of their family, and a
  per-type count cannot see them:** `CFRetain`, `g_object_ref`, `os_retain`,
  `xpc_retain`, `sec_retain`, `dispatch_retain`. So in CoreFoundation and GObject
  every handle type has one.
- **The name misses at least one.** json-c's adder is `json_object_get`, and its
  header says *"Increment the reference count … thereby taking ownership"*.

**What a reference-adder returns decides R1's form.** I curated 120 same-address
reference-adders by hand from `refret.py`'s output (Darwin and Linux, with false
matches such as `uv_ref`, `g_object_weak_ref` and `OSSL_FUNC_BIO_up_ref`
removed):

- **85 return the handle.** cairo ×7, harfbuzz ×11, glib ×45, dbus ×4,
  `json_object_get`, `libusb_ref_device`, `FcConfigReference`, `pixClone`,
  `CFRetain`, … A result mark can carry R1 for these.
- **35 return a status or nothing, so only a parameter mark can carry R1:**
  - all **27** OpenSSL `_up_ref` functions (`X509_up_ref`, `BIO_up_ref`,
    `EVP_PKEY_up_ref`, `SSL_CTX_up_ref`, … all `-> int`);
  - `FT_Reference_Face` (`FT_Error`);
  - `FcPatternReference`, `gpgme_key_ref`, `gpgme_result_ref`,
    `nghttp2_rcbuf_incref` and `dispatch_retain`, all `void`;
  - `xmlDictReference` (`int`);
  - `Z3_inc_ref(ctx, ast)`, whose handle is the **second** parameter.

**CoreFoundation is out of reach before R1 matters** (`work/cf/`). `CFStringRef` is
`const struct __CFString *`. Both `record CFStr tag __CFString` and `tag void`
give `error[ffi_return_type]` on `CFStringCreateWithCString` and on `CFRetain`, at
exit 1. The checker's note for a `CFRetain` declared with neither mark offers
`acquires` or `borrows`, and both are wrong for a retain: that is defect 079's
shape in the diagnostic.

---

## 2. The routes, bound for real

### 2a. What the C boundary sees: nothing new

I did not build four compilers. The compiler-engineer prices those. I took the C
that **today's compiler emitted** for each program, and `exp/patch.py` rewrote
only its two kinds of mark line (`hero_handle_acquired`, `hero_handle_consumed`)
into one prototype runtime's entries. That runtime is `exp/hv.c` (153 lines) with
`hv.h`, and its entries are:

- `hv_acquired(h, by, names[])`, which is panel 175's set;
- `hv_released(h, by)`, which checks `by` against the set;
- `hv_transferred` for V1;
- `hv_moved_into(h, by, receiver)` for V3;
- `hv_referenced` for R1.

Each patched unit was then compiled at the project flags against the real header
and linked against the real library.

- **112 patched units** (16 programs × 7 routes). **0 differ from the compiler's
  own C in anything but the mark lines**, checked by a `diff` with those lines
  removed. The library calls, the probes and every `_Static_assert` over the
  header are the compiler's bytes, so §4.19's header verification is untouched.
- **0 diagnostics** at project flags, on Darwin arm64, Linux arm64 and Linux
  x86-64. The briefs' own headers' `static` functions give the
  `-Wunused-function` warnings they give under today's compiler, and I excluded
  those. `hv.c` alone is also 0 under `-Wextra -Wconversion`.
- **No value changes layout, nothing is boxed, and no `str` changes.** Every route
  adds `const char *` literals and runtime calls beside the C call, and nothing
  inside it.

**No route breaks the C ABI, so there is no veto on that ground.**

### 2b. What each route does to 16 programs

Each program was run three times per cell. `today` is this seat's compiler,
unpatched. `A1` is panel 175's set of releasers with no transfer word, the
control. All runs are `-O0`, Darwin arm64 (`work/routes/out/darwin.txt`).

| program | what the C does (raw C: `hv_null.c`, ASan and `leaks`) | today | A1 | V1 | V2 | V3 | V1+R1 | V2+R1 | V3+R1 |
|---|---|---|---|---|---|---|---|---|---|
| `popen_fopen` | the wrong release | 0 | 134 | 134 | 134 | 134 | 134 | 134 | 134 |
| `vk_cross` | the crossed releaser | 0 | 134 | 134 | 134 | 134 | 134 | 134 | 134 |
| `xfer_cj` | a transfer; ASan 0, 0 leaks | 0 | **134** | 0 | 0 | 0 | 0 | 0 | 0 |
| `xfer_jsonc` | a transfer (json-c leaks, see § 5) | 0 | **134** | 0 | 0 | 0 | 0 | 0 | 0 |
| `xfer_ssl` | a transfer; 0 leaks | 0 | **134** | 0 | 0 | 0 | 0 | 0 | 0 |
| `jsonc_into_borrowed` | into a **borrowed** receiver | 0 | 134 | 0 | 0 | 0 | 0 | 0 | 0 |
| `jsonc_borrow_get` | `json_object_get` on a borrowed field, json-c's own documented use | 0 | 134 | 0 | 0 | 0 | 0 | 0 | 0 |
| `jsonc_put_child` | put the child after handing it in: json-c's header calls this *"a classic use-after-free"* | 134 | 134 | 134 | 134 | 134 *(owner message)* | 134 | 134 | 134 *(owner message)* |
| `refcount` | two references, ASan 0 | **134** | 134 | 134 | 134 | 134 | **0** | **0** | **0** |
| `jsonc_two_refs` | the same over real json-c, 0 leaks | **134** | 134 | 134 | 134 | 134 | **0** | **0** | **0** |
| `x509_upref` | `X509_up_ref` (returns `int`), then two `X509_free`; 0 leaks | **134** | 134 | 134 | 134 | 134 | **0 (parameter form)** | 0 | 0 |
| `ssl_upref_set0` | the man page's own spelling: `BIO_up_ref`, `SSL_set0_rbio`, `SSL_set0_wbio`; 0 leaks | **134** | 134 | 134 | 134 | 134 | **0 (parameter form)** | 0 | 0 |
| `getter_wrong_repaired` | a raw double free (ASan) | 134 | 134 | 134 | 134 | 134 | 134 | 134 | 134 |
| `ssl_same_bio` | `SSL_set_bio(s, b, b)`: correct, ASan 0, 0 leaks | **134** | 134 | **134** | **134** | **134** | **134** | **134** | **134** |
| `jsonc_failed_add` | the add fails (-1) and the caller frees the child: correct, 0 leaks | **134** | 134 | **134** | **134** | **134, message false** | 134 | 134 | 134 |
| `jsonc_failed_leak` | the add fails and nobody frees the child: a C leak, 896 B by `leaks` on today's binary | 0 | 134 | **0** | **0** | **0** | 0 | 0 | 0 |

**Linux arm64 and x86-64** (`out/heroes-linux-arm64.txt`, `out/heroes-linux.txt`)
ran the 8 programs that need no json-c: `vk_cross`, `xfer_cj`, `xfer_ssl`,
`refcount`, `getter_wrong_repaired`, `x509_upref`, `ssl_same_bio` and
`ssl_upref_set0`. Every cell is identical to Darwin's. **json-c on Linux is UNRUN**:
neither image has its headers.

What the table says:

1. **V1, V2 and V3 give the same verdict on all 16 programs.** V3 differs only in
   its text, on `jsonc_put_child` and `jsonc_failed_add`, and on the second the
   text is false: *"given back while another value owns it"*, of a child the array
   refused (length 0). So the three routes differ in what the binding author
   writes, and not in what a program does.
2. **R1 is needed by four correct programs, and two of them need its parameter
   form.** `x509_upref` and `ssl_upref_set0` run under R1 only because the patch
   marks the argument of an `-> int` call.
3. **R1 read strictly** (*"adds one more reference to a handle already live"*,
   `HV_R1_STRICT=1`) makes `jsonc_borrow_get` exit **134**, three of three: json-c's
   own documented use of `json_object_get` is refused. The tolerant reading (one
   more if held, else the first) runs it at 0, and `refcount` and `jsonc_two_refs`
   stay at 0 under both.
4. **No route runs `ssl_same_bio`, and no route spells a conditional transfer.**
   Every route ends the obligation before the call, unconditionally. So the
   correct failure path is refused, and the leaking one runs silent. This is true
   today as well, which makes it a completeness gap common to all routes rather
   than a regression of any (§1.12: *any C library must be bindable*). **design.md
   does not cover it.** I grepped for `on success|if the call fails|failed
   (add|transfer)|conditional` and §4.19 has no hit.

### 2c. What each route costs a binding author

I wrote two real bindings: a json-c binding of 31 members (12 creators,
`json_tokener_parse`, `json_object_from_file`, `json_object_get`/`_put`, 5 adders,
3 borrowing getters, accessors), and an OpenSSL BIO/SSL/X509 binding of 31
members. The `X` spelling of each was built against the real headers and run:
json-c prints `{ "n": 42 }` and exits 0. OpenSSL's was built with each `a | b | c`
set cut to its first name, since today's grammar takes one, and exits 0. Each route's spelling was
then derived by substitution (`work/bindings/*.binding`). The route spellings do
not parse today, so they were counted and not compiled.

| binding | route | members | names inside marks | longest line | bytes | lines to edit when one transfer is added |
|---|---|---|---|---|---|---|
| json-c | X / V1 | 31 | 15 | 103 / 104 | 2323 / 2328 | 1 |
| json-c | **V2** | 31 | **90** | **232** | **4363 (+88%)** | **16** (the new function, plus 15 acquirers) |
| json-c | V3 | 31 | 20 | 112 | 2368 | 1 |
| json-c | V1+R1 | 31 | 15 | 104 | 2327 | 1 |
| OpenSSL | X (with A′'s sets) / V1 | 31 | 23 | 115 | 1821 / 1826 | 1 |
| OpenSSL | **V2** | 31 | **47** | 172 | 2163 | **7** (the function, plus 6 BIO acquirers) |
| OpenSSL | V3 | 31 | 28 | 115 | 1856 | 1 |
| OpenSSL | V1+R1 (parameter form on 3 `_up_ref`) | 31 | 28 | 115 | 1909 | 1 |

**No route adds a declaration**: members stay 31 everywhere, so none is glue in
§1.11's sense of extra code. **V2 is the one that multiplies**, and it multiplies
a **fact**: *`json_object_object_add` transfers* is written 15 times. The line I
draw for §1.11: **one declaration per C function, and each contract fact stated
once, where it happens.** V1 and V3 keep that. V2 does not, and V2 is non-local
across libraries as well:

- The BIO acquirers of **libcrypto's** group must name **libssl's**
  `SSL_set0_rbio`, `SSL_set0_wbio` and `SSL_set_bio`.
- That works in a program today only because of the inconsistency in § 5 item 4.
  `heroes check` on a module whose mark names a releaser another module declares
  gives 1 alone and 0 inside a program.

### 2d. The strongest reason each route is wrong

- **V1:** it spells every transfer as unconditional, and json-c's five adders and
  OpenSSL's `add0`/`push0` family are conditional on success (§ 1c, and
  `jsonc_failed_add` above). This is not a reason to refuse V1, because every route
  shares it. It is the condition on V1 (below).
- **V2:** it costs a product, acquirers × transfers, where V1 costs a sum. A name
  forgotten on one of 15 lines is a correct program refused at run time with no
  compile-time sign. And it gives the checker nothing to tell a release from a
  transfer, since both are simply "in the set".
- **V3:** it is a receiver relation bought for a message. It degrades silently to
  V1 when the receiver is borrowed (`jsonc_into_borrowed`), and it lies on a
  failed transfer. The one place a receiver relation would catch something V1
  cannot is the **keep-without-owning** shape of § 1c: releasing
  `X509_STORE_CTX_set0_untrusted`'s stack while the ctx still points at it. That
  is **UNRUN** as a use-after-free, and it is not what V3 is drafted for.
- **R1:** a result-only mark leaves OpenSSL's whole `_up_ref` family, 27 functions,
  with no spelling. The strict wording refuses json-c's own documented idiom.

---

## 3. Question 3 in C

`work/q3/modes.c`, SDK SQLite 3.54.0, each mode as `sqlite3.h:5022-5034` says:

| mode | the C call | ASan | leaks |
|---|---|---|---|
| STATIC, bytes kept alive past `sqlite3_finalize` | `sqlite3_bind_text(st, 1, word, -1, SQLITE_STATIC)` | 0, stored the text | 0 |
| STATIC, bytes freed before the step (what `lent_static.hero` compiles to) | the same call, then `free(word)` | **134, heap-use-after-free** | — |
| TRANSIENT, bytes freed at once | `…, SQLITE_TRANSIENT)` then `free(word)` | 0, stored the text | 0 |
| destructor, the program never frees | `…, free)` | 0 | 0 |

Which Heroes spelling is **true** of the call it makes:

| spelling | X (today) | A2 | true? |
|---|---|---|---|
| one declaration, `text: cstr lent`, `destructor: ptr`, passing `nullptr` (`lent_static.hero`) | check 0, run 0, a wrong answer | legal (one declaration) | **false** for STATIC |
| one declaration, `text: cstr` (no `lent`), a lease for both pointer modes (`work/q3/conservative_right.hero`) | check 0, run 0 three of three | legal | **true** for both. For TRANSIENT the lease is not needed and costs 2 lines per call. `lent_static`'s mistake is `error[lend_kept]`, exit 1 (`conservative_wrong.hero`) |
| one module per mode (`work/q3/s2/`: `kept.hero` has `text: cstr`, `transient.hero` has `text: cstr lent`) | check 0, run 0, three of three | **refused** by A2's text (the marks differ on `text`). A2 is not built here, so this is a reading and is **UNRUN** | true, each |
| the destructor mode: `text: ptr`, `destructor: (function(ptr) -> ())` | legal in its own module | legal: no marks differ (a reading, UNRUN) | true |
| **a header shim**, `work/q3/sqlite_modes.h`: 3 `static inline` wrappers, each with the fifth argument fixed, bound in one module (`s3_shim.hero`) | check 0, run 0 three of three, `--sanitize` clean, 0 leaks | legal: three C names | true, each. **`lent_static`'s mistake becomes `error[lend_kept]` at exit 1** (`s3_wrong.hero`) |

So under A2 the true spellings that remain are the conservative single
declaration and the shim. **In both, `lent_static`'s mistake cannot be written**:
passing `word.cstr()` to a parameter without `lent` is `error[lend_kept]` at exit
1, run for both. What separates them is ergonomics:

- The conservative declaration makes every TRANSIENT call take a lease.
- The shim keeps an exact, lease-free `lent` for TRANSIENT beside STATIC in one
  program, under either answer to Q2. It is six lines of C in a header and needs
  no build step, since it is `extern "sqlite_modes.h"`, the way every reproducer
  here binds its own `.h`. That is §1.11's *"thin C shim … for the hard cases"*,
  and a contract chosen per call is a hard case.

**Neither answer to Q2 makes `lent_static` an error.** It is one declaration and
A2 governs two. What makes it an error is the author not writing `lent`, which no
rule over declarations can force, because `lent` is true of the TRANSIENT
calls.

---

## 4. Question 4 in C

`census/q4.py` looked for every parameter whose type, as written or desugared, is
a pointer to a pointer to const character data. That covers `const char **`,
`const gchar **`, `const xmlChar **`, `PCRE2_SPTR *`, `const uint8_t **` and the
like. It found **335** on Darwin (SDK and Homebrew) and **254** on Linux arm64,
**589** in all. The attached comment of **one** says the caller frees:
`tesseract/capi.h:499` over `TessBaseAPIDetectOrientationScript(…, const char
**script_name, …)` says *"Call TessDeleteText(*best_script_name) to free memory
allocated by this function"*.

**It is false, run** (`work/q4/tess_osd.c`, Tesseract 5.5.3, OSD on a page cairo
rendered):

- Two calls return **the same pointer**, three runs of three
  (`first=0x95ad4c1d0 second=0x95ad4c1d0`), so the string is not allocated per
  call.
- Following the header's comment exits 134. Under ASan it is *"attempting
  double-free"*, inside `tesseract::UNICHARSET::clear()`.
- The `const` in the type is the truth, and the comment is the stale half. That
  it went stale when the parameter lost `char **` is my recollection of
  Tesseract's history and is **UNRUN**.

Every caller-owned out-string I know of is a non-const `char **`: `getline`,
`asprintf`, `g_file_get_contents`, `curl_url_get`, `av_opt_get`. **Two of those I
ran through Heroes today** (`work/q4/`):

- `@o: cstr owned f` over `char **` builds and runs;
- over `const char **` it is `internal error`, exit 2 (defect 078);
- over `unsigned char **` and `const unsigned char **` it is already
  `error[ffi_parameter_type]` at exit 1. The class exists, and the qualifier is
  the one member missing from it.
- The shape beside it, a `const char *` **result** with `owned`, builds and runs
  with both a `const char *` and a `void *` releaser, so it needs nothing.

**So `owned` on a `const char **` cell was false in every case I could find, and
the repair is the refusal.** It is an `ffi_` diagnostic at exit 1, the fifth-member
pattern of `.claude/rules/c-boundary.md`. The emitter need not learn the header's
qualifier. The emitted `(char **)&cell` cast is what trips clang, and that is the
right place for it to trip. Only the report is wrong.

---

## 5. Found in what ships, or in the libraries (for the coordinator)

1. **`SSL_set_bio(s, b, b)` is `check` 0 and `run` 134 today**, with 394 B that
   call it *"the same handle given back TWICE, which is a double release and may
   already have corrupted memory"*. The C is correct (ASan 0, 0 leaks), and every
   route leaves it at 134. It is a correct program refused with a false message,
   defect 079's class, over the most common OpenSSL idiom. I searched `docs/panel`
   and `docs/records/log` for `SSL_set_bio|rbio == wbio|same BIO` and found it only
   in this sitting's and panel 175's texts, never as this shape.
2. **A failed transfer**, `jsonc_failed_add` (134, correct) and `jsonc_failed_leak`
   (0, leaking 896 B), today and under every route.
3. **json-c 0.19 leaks every object that had a child, when it is put.** The brief's
   *"(json-c freed the root)"* is false as measured:
   - the pure C `work/jc/jc4.c` shows `put(root)=1`, then `malloc_size(root)=48`
     and `malloc_size(child)=0`;
   - `leaks --atExit` finds the ROOT (912 B), and the brief's own `xfer_jsonc`
     binary shows the same 912 B;
   - 200 000 rounds reach **158.8 MB** maximum RSS against **1.5 MB** without the
     add (`jc6.c`, `/usr/bin/time -l`).
   - The 0.19 ChangeLog's *"Issue #923: Avoid stack recursion in
     json_object_put()"* is the likely cause, and that is an inference.
   - This is not Heroes' defect, but the brief's reproducers that print `put 1`
     are leaking programs.
4. **`unread_releaser`'s scope depends on which file is checked**
   (`work/xmodname/`). `bio.hero` declares `h_open() -> H acquires h_close2`, where
   only `main.hero` declares `h_close2`. `heroes check bio.hero` gives **1** (*"no
   `extern` of this module declares it"*), while `heroes check main.hero`, `build`
   and `run` give **0**. The message says *module* and the rule resolves
   program-wide. I searched `docs/work/` and `docs/records/log/` for
   `unread_releaser` and found no entry naming this.

---

## The seat's answer

- **verdict:** approve V1. Object to V2 and to V3, with no veto. Approve R1 on two
  conditions: a parameter form, and "first if not held". Approve A2, recording its
  cost. Refuse `owned` on `const char **`. No veto: no route changes the C ABI.
- **section:** design.md §1.11 (ergonomics; *"a thin C shim … for the hard
  cases"*), §4.19 (header verification, and *"a binding annotation vocabulary …
  a buffer that C takes ownership of"*), and §1.12 (*any C library must be
  bindable*). The conditional transfer is **not covered** by design.md; I searched
  §4.19 for `on success`, `conditional` and `if the call fails`.
- **experiment:** 112 route-patched C units over json-c 0.19, OpenSSL 3.6.4 and
  3.5.7, SQLite 3.54 and the briefs' headers, which differ from the compiler's C
  only in mark lines. 0 diagnostics at project flags, and clang accepted every
  one on Darwin arm64, Linux arm64 and Linux x86-64. Around them: a census over
  51 k functions in six AST units, 6 GIR files and 97 OpenSSL man pages; Q3's
  three modes in C, and a header shim that runs clean; Q4 run against Tesseract,
  where the header's own advice is a double free. Windows UNRUN.
- **argument:** V1, V2 and V3 give identical verdicts on all 16 programs and all
  three platforms, so they differ only in what a binding author writes. V1 states
  a transfer once, where it happens. V2 states it once per acquirer: json-c goes
  from 15 to 90 names, one new adder is 16 edits, and libcrypto's marks must name
  libssl's calls. V3 buys a message, and that message is false on a failed
  transfer. R1 is needed by four correct programs. Two of them go through
  OpenSSL's `_up_ref`, which returns `int`, so a result-only R1 cannot spell them.
  Every route treats a transfer as unconditional, and real ones mostly are not.
- **prediction:** at the step of M-agreed-retention that lands panel 176's
  consumer vocabulary: if R1 lands as a result mark only, `x509_upref.hero`
  (`X509_up_ref`, then two `X509_free`) has no spelling that runs, and exits 134
  on Darwin arm64, Linux arm64 and Linux x86-64. With a parameter form it exits 0
  on all three. And whichever consuming word lands, `ssl_same_bio.hero` exits 134
  on all three unless the word also says what one handle in two consuming
  parameters means. Checkable at that landing; Windows at the landing, and unrun
  today.
- **condition:**
  - I would drop the parameter-form condition on R1 if the landing's census
    showed OpenSSL's `_up_ref` family reachable some other way with no shim.
  - I would withdraw my objection to V3 if a program were shown where the receiver
    relation changes a verdict, and not only a message.
  - I would move V2 from object to approve if a binding were shown where
    transfers per acquirer stay at one or two.
  - I would change the Q4 refusal to "emit the header's qualifier" on one real
    function whose `const char **` out-parameter is caller-freed, **run and
    confirmed**, and not merely stated in a comment.
  - And I would veto any route that closes the conditional transfer by changing
    what the C call receives: an argument, a wrapper, or a copy.

## Unrun

- Windows, everything.
- json-c on Linux: neither image has its headers.
- Real cJSON: `cj.h` is a mock.
- V1, V2, V3 and R1 as compilers (grammar, formatter, `heroes grammar`,
  `check/marks.hero`). These are the compiler-engineer's; my runs are the
  runtime side, applied to the C by a script.
- A2 as a rule: its verdicts on `s2/` and on the destructor mode are readings of
  its text.
- The run-time cost of the routes: `hv.c` is O(n) on purpose and was not timed.
- The keep-without-owning use-after-free.
- A reader-side effect of any spelling.

## Files

All under
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/176-ffi-pragmatist/`:

- **Census:** `census/xfer.py`, `sigs.py`, `gir.py`, `q4.py`, `refret.py`,
  `ossl_man.py`, `ossl_page.py`, and their outputs (`*.sigs.tsv`, `brew.out`,
  `gir.out`, `gir.rows.tsv`, `ossl0_names.txt`, `ossl0_man.tsv`, `q4_darwin.tsv`,
  `q4_linux.tsv`, `refret_*.out`, `linux/`).
- **Route prototype:** `exp/hv.c`, `exp/hv.h`, `exp/hv_null.c`, `exp/patch.py`,
  `exp/routes.py`, `exp/linux_routes.sh`, `exp/cc.sh`.
- **Route runs:** `work/routes/*.hero`, `work/routes/out/` (every patched `.c`,
  spec `.json` and binary; `darwin.txt`, `heroes-linux-arm64.txt`,
  `heroes-linux.txt`, `darwin_failed_add.txt`, `darwin_failed_leak.txt`).
- **Bindings:** `work/bindings/jsonc_X.hero`, `ossl_X.hero`,
  `ossl_X_today.hero`, `*.binding`.
- **Q1 shapes:** `work/q1/ssl_same_bio.*`, `use_after.hero`, `set0_keep*.c`,
  `linux_set0.sh`; `work/jc/` (json-c's leak and failed-transfer C);
  `work/xmodname/`; `work/cf/`.
- **Q3:** `work/q3/modes.c`, `sqlite_modes.h`, `s3_shim.hero`, `s3_wrong.hero`,
  `s2/`.
- **Q4:** `work/q4/tess_osd.c`, `q4.h`, `t_*.hero`, `t2_*.hero`.
