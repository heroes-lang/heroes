# Panel 148 — ffi-pragmatist

## verdict

**`veto` on option C. `approve` on option A.** A third option is priced below
and I recommend it over both.

The veto is not on ABI grounds — I measured that and neither option touches the
ABI. It is on the second clause of my mandate: option C makes bindings
categorically harder for **12 of the 19 libraries whose headers I opened**, and
the way it makes them harder is a `bad-free` abort that I reproduced twice under
AddressSanitizer against real `libsqlite3` and real `libcurl`.

## section

- **design.md §1.11** — the founding constraint. Everything a real program needs
  comes from C, so a rule that makes 12 of 19 real libraries unbindable-as-declared
  is not a surface question.
- **design.md §4.19** — *"treat its ergonomics as a priority rather than an
  afterthought."*
- **design.md Part 6, the Borrow-checker row** — this is the citation that
  decides the sitting, and it already ruled against option C's premise in this
  document: a lifetime checker over foreign memory would need *"ownership facts
  **no C header carries**"*. Option C claims to derive exactly such a fact from
  the set of functions a header declares. The row was written after that
  premise was measured false.
- **design.md §1.12 / CLAUDE.md § Precedence rank 3** — a Heroes program must not
  corrupt memory. It beats ergonomics, token cost and compiler size.

## experiment

### 1. The count (the brief's item 2) — the number that decides option C

**Test applied**: within ONE header file — the string a Heroes `extern "…"`
group would name — does the header declare both (a) a function that ends the
life of handle type `T`, and (b) a function that hands back a `T` the caller
must **not** release?

**Result: 12 of 19.** (13 of 19 if the borderline FreeType case is admitted.)
Of the nine libraries the brief named by name: **8 of 9**.

| library | header opened | releaser | borrowed hand-back of the same type | ? |
|---|---|---|---|---|
| sqlite3 | `…/MacOSX.sdk/usr/include/sqlite3.h` | `sqlite3_finalize` :5564 | `sqlite3_next_stmt` :6996 | **yes** |
| | same | `sqlite3_close` :352 | `sqlite3_db_handle` :6861, `sqlite3_context_db_handle` :6209 | |
| curl | `…/MacOSX.sdk/usr/include/curl/curl.h` | `curl_slist_free_all` :2808 | `curl_slist_append` :2798 (returns the list it was given) | **yes** |
| libxml2 | `…/MacOSX.sdk/usr/include/libxml2/libxml/tree.h` | `xmlFreeNode` :1018, `xmlFreeNs` :818 | `xmlDocGetRootElement` :962, `xmlGetLastChild` :964, `xmlNextElementSibling` :1337, `xmlSearchNs` :1029 | **yes** |
| cairo | `/opt/homebrew/include/cairo/cairo.h` | `cairo_surface_destroy` :2489, `cairo_pattern_destroy` :2940, `cairo_font_face_destroy` :1629, `cairo_scaled_font_destroy` :1712, `cairo_device_destroy` :2340 | `cairo_get_target` :2120, `cairo_get_source` :2078, `cairo_get_font_face` :1577, `cairo_get_scaled_font` :1584, `cairo_surface_get_device` :2492 | **yes** |
| harfbuzz | `/opt/homebrew/include/harfbuzz/hb-font.h` | `hb_font_destroy` :1129 | `hb_font_get_parent` :1160, `hb_font_get_empty` :1123 | **yes** |
| OpenSSL | `/opt/homebrew/opt/openssl@3/include/openssl/ssl.h` | `SSL_CTX_free` :1632, `SSL_free` :1987 | `SSL_get_SSL_CTX` :2199, `SSL_get0_connection`, `SSL_get0_listener`, `SSL_get0_domain` | **yes** |
| libuv | `/opt/homebrew/include/uv.h` | `uv_loop_close` :292, `uv_loop_delete` :304 | `uv_default_loop` :290, `uv_handle_get_loop` :487 | **yes** |
| SDL2 | `/opt/homebrew/include/SDL2/SDL_render.h` | `SDL_DestroyRenderer` :1790, `SDL_DestroyTexture` :1776 | `SDL_GetRenderer` :264, `SDL_GetRenderTarget` :800 | **yes** |
| SDL3 | `/opt/homebrew/include/SDL3/SDL_render.h` | `SDL_DestroyRenderer` :2654, `SDL_DestroyTexture` :2638 | `SDL_GetRenderer` :434, `SDL_GetRendererFromTexture` :977 | **yes** |
| json-c | `/opt/homebrew/include/json-c/json_object.h` | `json_object_put` :181 | `json_object_object_get`, `json_object_array_get_idx` (the header itself says so at :167-168) | **yes** |
| glib | `/opt/homebrew/include/glib-2.0/glib/gmain.h` | `g_main_context_unref` :498, `g_main_loop_unref` | `g_main_context_default` :500, `g_main_context_get_thread_default` :581, `g_main_loop_get_context` | **yes** |
| fontconfig | `/opt/homebrew/include/fontconfig/fontconfig.h` | `FcConfigDestroy` :463, `FcCharSetDestroy` | `FcConfigGetCurrent` :469, `FcPatternGetCharSet` (out-param) | **yes** |
| FreeType | `…/freetype2/freetype/ftmodapi.h` | `FT_Remove_Module` | `FT_Get_Module` :303 | *borderline* |
| FreeType | `…/freetype2/freetype/freetype.h` | `FT_Done_Face` :2879, `FT_Done_FreeType` :2371 | none | no |
| raylib | `/opt/homebrew/include/raylib.h` | 30 `Unload*`/`Close*`, all taking a struct **by value** | no opaque handle exists | no |
| zstd | `/opt/homebrew/include/zstd.h` | `ZSTD_freeCCtx`, `ZSTD_freeDCtx`, … | every `ZSTD_create*` is owned | no |
| libpng | `/opt/homebrew/include/png.h` | `png_destroy_read_struct` :1480 | none | no |
| libsndfile | `/opt/homebrew/include/sndfile.h` | `sf_close` | none | no |
| libyaml | `/opt/homebrew/include/yaml.h` | `yaml_document_delete` :847 | `yaml_document_get_root_node` :881 hands back a borrowed `yaml_node_t*`, but the header declares **no** `yaml_node` releaser — fails the same-type test | no |
| FFmpeg | `/opt/homebrew/include/libavformat/avformat.h` | `avformat_free_context` :2028 | `avformat_new_stream` :2099 hands back a borrowed `AVStream*`, no public `AVStream` releaser | no |
| libm | `…/MacOSX.sdk/usr/include/math.h` | — | — | n/a |

Other headers opened and read while getting there, named so the enumeration is
checkable: `curl/easy.h`, `curl/multi.h`, `curl/urlapi.h` (each of those three
**alone** is safe — every hand-back is owned), `harfbuzz/hb-face.h`,
`hb-buffer.h`, `hb-blob.h`, `hb-shape-plan.h`, `freetype/ftglyph.h`,
`ftsizes.h`, `ftrender.h`, `ftmm.h`, `openssl/x509.h`, `SDL2/SDL_video.h`,
`SDL2/SDL_surface.h`, `SDL3/SDL_surface.h`, `SDL3/SDL_video.h`.
`/opt/homebrew/include/pcre2.h` was opened but its declarations are
macro-mangled by `PCRE2_SUFFIX` and my extractor returned nothing, so pcre2 is
**not** in the count either way — unclassified, not a no.

**Two same-header pairs are the whole argument in one line each.** Adjacent
lines, byte-identical C signatures, opposite ownership:

```c
/* openssl/ssl.h:1827-1828 */
__owur X509 *SSL_get0_peer_certificate(const SSL *s);   /* caller must NOT free */
__owur X509 *SSL_get1_peer_certificate(const SSL *s);   /* caller MUST free     */

/* glib/gmain.h:581,583 */
GMainContext *g_main_context_get_thread_default (void); /* borrowed */
GMainContext *g_main_context_ref_thread_default (void); /* owned    */
```

I compiled a probe that takes the address of both OpenSSL functions through one
`typedef X509 *(*f)(const SSL *)`. **clang accepts it, exit 0, clean under
`-Weverything`.** There is nothing in the header for any rule to key on.

And the headers mostly do not say it in prose either: `cairo.h` contains **zero**
occurrences of *owned by*, *do not free*, *must not be freed*, *borrowed* or
*transfer* (grep, case-insensitive, one hit and it is about image data). SDL2's
`SDL_GetWindowSurface` doc comment names `\sa SDL_DestroyWindowSurface` and never
says the surface must not be freed. That is design.md Part 6's *"ownership facts
no C header carries"*, re-measured on four more libraries.

### 2. The C option C would force. clang accepted it; the process aborted.

`sqlite3_next_stmt` and `sqlite3_finalize` are in **one** header, so in Heroes
they are one `extern` group. Option C's own words — *"every call in that group
handing back a `T` acquires one"* — put an obligation on `sqlite3_next_stmt`.
This is the program that obligation compels, compiled against the real
`sqlite3.h` and linked against the real `-lsqlite3`:

```c
sqlite3_stmt *again = sqlite3_next_stmt(db, NULL);   /* option C: "acquires" a CStmt */
printf("st=%p again=%p same=%d\n", st, again, st == again);
sqlite3_finalize(st);
sqlite3_finalize(again);   /* the call option C's rule demands */
```

`clang -g -fsanitize=address … -lsqlite3` — **accepted, zero warnings.** Running
it:

```
st=0x634000016c40 again=0x634000016c40 same=1
finalize#1 = 0
==24735==ERROR: AddressSanitizer: attempting free on address which was not malloc()-ed
    #1 sqlite3VdbeDelete+0xc4 (libsqlite3.dylib)
    #2 sqlite3_finalize+0x54 (libsqlite3.dylib)
    #3 main c2.c:11
SUMMARY: AddressSanitizer: bad-free … ABORTING
```

The same experiment on `curl/curl.h`, which is the one binding in the shipped
tree that already uses `consumes`. Three `curl_slist_append` calls build one
list; option C reads three acquisitions:

```
==24804==ERROR: AddressSanitizer: SEGV on unknown address 0x20e
    #1 free+0xdc
    #2 curl_slist_free_all+0x2c (libcurl.4.dylib)
    #3 main c3.c:13
```

The control: `zstd.h` has a releaser for every context and no borrowed
hand-back. `ZSTD_createCCtx` / `ZSTD_freeCCtx` compiled and ran clean, exit 0.
Option C is correct for zstd and for the 5 other `no` rows. It is wrong for 12.

### 3. ABI — measured, and there is no veto here

I emitted the C for `examples/curl/main.hero` twice, once as shipped and once
with `consumes` deleted from `curl_easy_cleanup`. Stripping only the `#line`
directives and the banner (the copy sits at a different path):

```
IDENTICAL after stripping #line/banner:      692 lines each
md5: 9fa5a9b2bd568fa642ae7557b7753c40   (both)
```

The mark has **zero** C footprint. Neither A nor C can move a handle, change how
it is passed or returned, or change `importc` verification — the emitted check
is `_Static_assert(HERO_RET_RECORD(curl_easy_init(), CURL *), …)` and neither
word is expressible in a `_Generic` over a return type. I also compiled that
assertion for both SQLite borrowed getters against the real header:

```
clang: ACCEPTED the two §4.19 assertions for the borrowed getters
runs, exit 0
```

So: **no ABI veto, no header-verification veto.** The veto is on bindings.

### 4. The brief's item 3 — what a binding author writes that they do not today

`examples/ledger/db/sqlite.hero`'s `extern` group has **18** `function` members
(the brief's 17 is the callable count; `sqlite3_free` is the declared-never-called
freer for `owned sqlite3_free`). Under A:

- **2 of 18 gain `acquires`**: `sqlite3_open` (`@out: CDb`) and
  `sqlite3_prepare_v2` (`@statement: CStmt`). Two words.
- 2 more would gain `consumes` — `sqlite3_close`, `sqlite3_finalize` — but that
  is today's word, not A's.
- **No signature becomes ambiguous.** `owned` takes exactly one identifier, so
  `@error: cstr owned sqlite3_free` cannot swallow a following word; and
  `-> Curl acquires` is two identifiers in a row, which no type production
  admits. The word is free: `grep -rn '\bacquires\b' --include='*.hero'` over
  the whole tree returns **nothing**, and the seed today refuses it loudly —
  `error[expected_extern_signature]: … found a name (acquires)`.

### 5. The brief's item 4 — does either option break what compiles today?

Measured, not reasoned: **`consumes` occurs exactly once in `examples/`** —
`examples/curl/main.hero:53`. The 370-line reference SQLite binding declares
**zero**. `./heroes build examples/ledger/main.hero` succeeds at the frozen
commit; `examples/curl` emits C and fails only at the link step in my sandbox.

So **neither option breaks a binding that compiles today**, and that is the
uncomfortable half of option C's case rather than a point in its favour:

- Under C, the feature is **inert on the reference binding**. It fires on one
  group in the whole corpus, and `examples/curl` already calls
  `curl_easy_cleanup` once for one `curl_easy_init`, so it stays green.
- The day somebody writes `consumes` on `sqlite3_close`, a compile error appears
  in a function that did not change. The obligation arrives as a side effect of
  editing an unrelated line. That is action at a distance in the one place
  design.md asks for the opposite.

### 6. A third option, priced on the instrument

Option C's *detection* is good and its *conclusion* is not. Keep the detection,
refuse the conclusion:

> **D** — option A's `acquires`, plus: in a group that consumes handle type `T`,
> a call handing back a `T` must say `acquires` or `borrows`; unmarked is an
> error naming the call.

The compiler enumerates the sites, as it already did for the mark's own
constructors (6 sites across 5 files, `docs/measurements/032`), and the **author**
answers — the one party who has the fact, since panel 139 measured that no
header carries it. `sqlite3_next_stmt` gets `borrows` and the abort above never
compiles.

Measured on the same instrument, from the frozen `spec/heroes-spec.md`:

| | cl100k_base | delta |
|---|---|---|
| baseline | 5863 | — |
| A (reproduced, matches the brief exactly) | 5927 | +64 |
| C (brief's figure, not re-measured by me) | 5919 | +56 |
| **D** | **5967** | **+104** |

Headroom on the binding `real` row is 386, so D fits. The `real` delta for D is
**unrun**: `heroes measure` prints the `real` row only for a path a ceiling
judges, and my drafts live in the scratchpad.

## argument

Option C reads an ownership fact off the set of functions a header declares.
design.md Part 6 already ruled that fact is not in the headers, and I re-measured
it on twelve more: `SSL_get0_peer_certificate` and `SSL_get1_peer_certificate`
are adjacent lines, identical to clang, opposite in ownership. So C is not a
cheaper spelling of A — it is a guess, and it guesses wrong for 12 of the 19
libraries I opened, including the two the corpus already binds. Under A a
mistake loses a leak check. Under C a mistake compels a release the program must
not make: measured twice, `bad-free` in `sqlite3VdbeDelete`, SEGV in
`curl_slist_free_all`. A fails safe, C fails unsafe. §1.12 outranks 8 tokens.

## prediction

Falsifiable, on a named binding. Add to `examples/ledger/db/sqlite.hero`'s
group, which is `extern "sqlite3.h"` and therefore one group:

```
    function sqlite3_finalize(statement: CStmt consumes) -> i64
    function sqlite3_next_stmt(db: CDb, statement: CStmt) -> CStmt
```

- **Under A**: both lines compile, `heroes build examples/ledger/main.hero`
  stays green, and no shim is needed. The `_Static_assert` for
  `sqlite3_next_stmt` passes — I compiled it.
- **Under C**: that pair cannot be written. The author's only repairs are to
  drop the `consumes` (losing the check for the whole type) or to declare the
  result `ptr` — which re-opens the group escape panel 146 re-measured to **zero**
  on 2026-09-14 and would put it back at 1.
- **Under D**: it compiles with `borrows` on the second line, and the compiler
  names the site if the word is missing.

Second prediction, cheaper to test: under C, `examples/curl/main.hero` extended
with `curl_slist_append` and `curl_slist_free_all consumes` — the ordinary way to
set an HTTP header — needs **one** `free_all` call per list and C demands one per
`append`. Bind three headers and the program will not compile; force it and it
SEGVs, which is the run above.

## condition

I withdraw the veto on C if either is produced:

1. **A rule, derivable from the header text alone, that separates
   `SSL_get0_peer_certificate` from `SSL_get1_peer_certificate` and
   `g_main_context_get_thread_default` from `g_main_context_ref_thread_default`**
   — with a measured false-acquire rate of zero across the 12 `yes` rows above.
   Naming conventions do not qualify: `sqlite3_next_stmt` and `cairo_get_target`
   share no prefix, suffix or digit with either OpenSSL spelling.
2. **A count, from headers, that makes my 12 of 19 wrong.** My test, my headers,
   my line numbers are all above; re-run it. If the true figure is 1 or 2, C is
   a cost argument and I will take it.

I approve A unconditionally today. I recommend **D** over A on CLAUDE.md § 4's
own rule — *the most robust and complete resolution, never the cheapest and
never a compromise* — and record that the conservative choice is A at +64.

---

*Everything above was run at commit `94b94bcd` in a scratch copy; the working
tree was not modified. `heroes` was built from `seed/` in 3.12 s wall; `build/`
and `target/` were removed afterwards. The C probes were compiled with clang
against the real headers and linked against the real `libsqlite3`, `libcurl` and
`libzstd` on this Mac.*
