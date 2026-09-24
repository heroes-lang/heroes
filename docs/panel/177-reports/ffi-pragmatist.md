<!-- Written out by the coordinator from the seat's final message, 2026-09-24:
the seat's own write was refused by the harness. The one mechanical change: the
transport escaped `->`, `<` and `>` as HTML entities, and they are unescaped
here. Nothing else is altered. -->

I could not write `REPORT.md`: the harness refuses report files from a subagent. The full report follows, for the coordinator to copy to `docs/panel/177-reports/ffi-pragmatist.md`.

# Panel 177 — ffi-pragmatist

## Verdict

**Question 1, the routes**

| route | verdict | ground, in one line |
|---|---|---|
| **R** (live-set check at non-consuming parameters) | **VETO as stated** | It refuses 6 of 8 correct real-library programs on all three legs. One of them has no spelling at all under R. It lets the brief's own 088 reproducer through, and one unused declaration flips its verdict. |
| **P** (poison the binding) | **approve, on four conditions** | No C byte changes. It stops 088 and both 077 reproducers. It misses 3 of 4 copy shapes, and its poison escapes through C structs. |
| **S** (serial in the value) | **VETO as stated** | Today's callback emission becomes a clang error. The serial cannot live in C's structs. The only trampoline a function value can get calls the wrong callback at exit 0. |
| **T** (not listed; built here: a dead set) | **approve, composed with P** | No false refusal on the 8 real programs. It catches all 4 copies P misses. It shares R's blindness to address reuse, which P covers. |
| M (moved binding, at check time) | no FFI objection (argued) | No C change. It reaches the same shapes as P, at compile time, and no copies. |
| A (affine handle) | object (argued, not compiled) | A handle placed in a C struct view and then released is two uses (SDL3's `SDL_GPUBufferBinding`, then `SDL_ReleaseGPUBuffer`). |
| G (generation in the set only) | refuse (argued) | The stale handle and the live one are the same bits. |
| D (the documented limit) | object if alone | §1.12. Also, `--sanitize` is not the remedy for real libraries (finding 2 below). |

**Question 2, the forms**

| form | verdict |
|---|---|
| **The success clause** | approve, **placed on the RESULT**: `-> i32 when 0`. A `retains` on a status result takes the same clause. |
| `retains`, parameter and result forms | approve. The C is one statement after an unchanged call. |
| `transfers <releaser>` | no C-side objection. Not re-run; its call-site C is panel 176's. |

## Section

- design.md **§1.11**: *"FFI ergonomics rank alongside comprehension"*.
- **§1.12**: *"any C library must be bindable"*.
- **§4.19**: clang verifies the header. The callback paragraph (lines 2372-2381 and 2396-2399) says a mismatched function-pointer cast *"silently calls the wrong callback"*.
- **Part 8 wart 20** (line 3592).
- design.md has no sentence on handles inside C structs or on callback-delivered handles. I searched §4.19 for both and found none.

## Cost / delta

- **C bytes, R, P, T, success clause and `retains`: none.** In 76 patched units, 0 lines were removed or changed apart from the runtime's own `hero_handle_consumed` mark lines. No library call, probe or `_Static_assert` differs (`diff`, run).
- **S changes C-visible types** at every callback parameter and every group-record field that holds a handle. That is 17 such fields in SDL3 3.4.16's `SDL_gpu.h` alone (`grep`, run).
- **Run-time price per call** (counted from the patches, not timed):
  - R: one lock and one probe per checked argument.
  - P: one compare per handle argument, plus one store per consuming call.
  - T: one probe per handle argument.
  - Examples of the count: `jsonc_read` has 8 R checks, `sqlite_borrowed` 12, `ossl_pump` 9.

## Prediction (falsifiable)

1. **If R lands as stated:**
   - `work/r/jsonc_read.hero` exits 134 at `json_object_get_string` on Darwin arm64, Linux arm64 and Linux x86-64.
   - `sqlite_borrowed_onedecl.hero` exits 134 and `sqlite_borrowed_nofree.hero` exits 0. The two differ by one unused `sqlite3_value_free` declaration.
2. **If S lands with a two-word handle in a callback parameter:**
   - Without a trampoline, `work/ossl/ossl_verify_cb.hero` fails to build with clang's `incompatible function pointer types`, on all three POSIX legs.
   - With a per-type trampoline, `ossl_verify_value.hero`'s two-store shape runs the wrong callback at exit 0.
3. **P checked only at handle arguments** leaves `work/t/pair_escape.hero` at exit 0, printing `pair value 1`, on all three legs.
4. **The success clause** lands as measured here:
   - `jsonc_success` exits 0 at n=1 and 134 at n=2.
   - With the value written wrong, it exits 134 at n=0, reporting *"never given back"*.
   - Checkable at the landing. Windows is unrun.

## Condition

- **R veto:** withdrawn if R records borrowed handles so that `jsonc_read`, `ossl_pump` and `sqlite_borrowed` run at 0 (that is T), or if someone shows a spelling under R for `sqlite3_column_value` beside `sqlite3_value_free`.
- **S veto:** withdrawn if S keeps one word at every C-visible position (arguments, `@` cells, group-record fields, callback parameters and results, function values handed to C). The measure: zero changed lines in the typedef and probe of `ossl_verify_cb` and `sdl_binding`.
- **P approval:** turns to object if P, with its four conditions, refuses a correct program.
- **Placement:** I would move the success clause to the parameter on one real function whose single call transfers one argument conditionally and another unconditionally. I found none; `ECDSA_SIG_set0` is conditional for both.

## Argument (≤120 words)

R keys on "the set holds it", and a borrowed handle is correct precisely because the set does not hold it. Real libraries hand those out constantly: json-c children, OpenSSL's `SSL_get_wbio`, `X509_get_subject_name` and `SSL_CTX_get_cert_store`, SQLite's `sqlite3_column_value`. R refused six of eight such programs on three legs, and one of them has no spelling at all under R. R also missed 088 itself, because the next `calloc` returned the freed address. S is worse at the boundary: C callbacks and C structs cannot carry a second word. A dead set (T) plus a poisoned place (P) changes no C byte, refused one correct program in eleven (a header model), and caught every wrong one except a handle inside a C struct, which no route catches.

## Evidence (every row run unless marked argued)

**Setup.**
- Compiler built from the seed in my directory: `real 3.31`, `heroes 0.2.0`.
- Darwin arm64: Apple clang 21.0.0. OpenSSL 3.6.4, json-c 0.19, SQLite 3.53.4 (all Homebrew), SDL3 3.4.16, raylib 6.0.
- Linux (both legs): Debian clang 22.1.8, OpenSSL 3.5.7, json-c 0.18, SQLite 3.46.1, installed by `apt` in a throwaway container.
- cJSON 1.7.18, Debian `libcjson-dev`, Linux arm64 only.
- Routes are emulated by patching the emitted C (`exp/patch.py`) against a runtime copy with one added probe (`exp/runtime`, `hero_handle_is_live`). Every cell is three runs, and every cell agreed on all three legs unless it says otherwise.
- The raylib example has no handles at all (`examples/raylib/main.hero`), so raylib is not in any table.

**Q1, the R census: correct programs, each clean under Guard Malloc on Darwin (exit 0).**

| program | borrowed handle → non-consuming call | today | R | P | T |
|---|---|---|---|---|---|
| `jsonc_read` | a json-c child → `json_object_get_string` | 0 | **134** | 0 | 0 |
| `sqlite_borrowed` | `sqlite3_column_value` → `sqlite3_value_type`; `sqlite3_db_handle` and `sqlite3_next_stmt` pass (their address is live) | 0 | **134** | 0 | 0 |
| `sqlite_borrowed_nofree` | the same, without the dup/free pair | 0 | 0 | 0 | — |
| `sqlite_borrowed_onedecl` | nofree plus ONE unused `sqlite3_value_free` declaration | 0 | **134** | 0 | 0 |
| `ossl_pump` | `SSL_get_wbio` after `SSL_set0_wbio` → `BIO_ctrl_pending` | 0 | **134** | 0 | 0 |
| `ossl_cert` | `X509_get_subject_name` → `X509_NAME_entry_count` | 0 | **134** | 0 | 0 |
| `ossl_store` | `SSL_CTX_get_cert_store` → `X509_STORE_add_cert` | 0 | **134** | 0 | 0 |
| `ossl_verify_cb` | a verify callback whose handles alias live ones | 0 | 0 | 0 | 0 |
| `cb_reuse` (header model) | a handle C made and passed to a callback, at an address the program had given back | 0 | **134** | 0 | **134** |

**Q1, the reproducers.**
- 088 (`read_after_consume`): today 0, R **0**, T 0, P **134**. `b == c: true` on all three legs, so the next `calloc` hands out the freed address and R's check passes.
- `use_after_consume` and `helper_consume_then_use`: 134 under every route, as today.
- 077 (`u1_static`, `reuse_malloc`): R 0, T 0, P **134**. Under P, `==` changes: `u1_static` prints `false` and `reuse_malloc` prints `same address: false`.

**Q1, P and the copy.**
- `sqlite_copies` holds four copies of a closed connection: the `T?` result, a record copy, an array element, and the binding itself. These are the ledger's own shapes.
  - P catches only the binding (**n=4, 134**).
  - The other three reach `libsqlite3` as use-after-free (Guard Malloc 139).
  - R and T catch all four.
- In `examples/`, 3 of the 20 release call sites run while a second named copy is alive: the ledger's `opened` (`Db?`) beside `db`.
- The ledger releases through a field of an `@` cell (`db.handle`), so P must poison a PLACE, not only a binding.
- P poisoning unconditionally refuses the correct failure path of a transfer (`jsonc_success` n=1: 134). Composed with the success clause (PX), it runs 0 / 0 / 134, which is right.

**Q1, S in C** (hand-written; the Darwin rows were compiled at the sixteen project flags; Linux arm64 and x86-64 matched with `-Werror=incompatible-pointer-types` for the call and callback rows):
- `s_call.c`: C sees `sqlite3 *` unchanged. The stale copy is refused even when the address is reused (`same address: 1`, then 134).
- `s_struct.c` (SDL3): storing a two-word handle into `SDL_GPUBufferBinding.buffer` is a clang error. C's struct is 16 bytes; an S record would be 24. Today's emitter asserts the field is exactly `SDL_GPUBuffer *`. Darwin only; SDL3 is not in the Linux images.
- `s_callback.c`, today's emission under S:
  `error: incompatible function pointer types passing 'h_fn' (aka 'int (*)(int, HeroH_X509_STORE_CTX)') to parameter of type 'X509_STORE_CTX_verify_cb'`
- `s_callback.c`, with a per-type trampoline: store `a`, given `seen`, printed `strict: error 18`, exit 0. A function value can reach this slot today: `ossl_verify_value.hero` checks and runs at 0.

**A hole every route shares.** `pair_escape` puts a released handle into a C struct passed by value. R, P and T all exit 0. Today the program prints `pair value 3` on Darwin and garbage on Linux; under P it prints `pair value 1`. The fix for P is to check at every READ of a handle binding, not only at handle arguments.

**Q2, the success values, re-run.**

| function | what the header or page says | conditional stated? | run |
|---|---|---|---|
| `json_object_object_add` (json-c 0.19, `json_object.h:403`) | 0 on success, negative on error | no | 0 on success; -1 on self-add, and the caller still owns `val` |
| `json_object_array_add`, `_put_idx`, `_insert_idx` | nothing | no | 0 on success; at index SIZE_MAX, -1 and the caller still owns `val` |
| `cJSON_AddItemToObject` 1.7.18 | nothing: *"Append item to the specified array/object."* | no | 1 on success; 0 on failure, caller keeps it (ASan and LSan clean, Linux arm64) |
| `CMS_add0_cert` (`cms.h:375`) | 1 / 0 | **yes**: *"on success it must not be freed up by the caller"* | failure keeps the certificate (3 injected, plus a data-type CMS). **An equal certificate already present: returns 1 AND is freed inside the call.** |
| `X509_CRL_add0_revoked` (`x509.h:802`) | 1 / 0 | no | 2 injected failures, `rev` kept by the caller |
| `OCSP_request_add0_id` (`ocsp.h:309`) | non-NULL / NULL | no | 3 injected failures, `cid` kept by the caller |
| `ECDSA_SIG_set0` | 1 / 0; its page words the transfer unconditionally | no | on failure `r` is kept by the caller; on success both are taken |
| `SSL_set0_rbio` | `void`, *"cannot fail"* | always | not applicable |

So:
- **Transfers only on success, and documents it:** `CMS_add0_cert` only.
- **Transfers only on success, not documented:** every other one I ran.
- **Transfers always:** `SSL_set0_rbio`.
- **Documents neither the value nor the condition:** the three json-c array adders and cJSON.

**Q2, the two placements.** The spellings, as a binding would read them:

```
function json_object_object_add(obj: Json, key: cstr lent, val: Json transfers json_object_put on 0) -> i32
function json_object_object_add(obj: Json, key: cstr lent, val: Json transfers json_object_put) -> i32 when 0
function CMS_add0_cert(cms: Cms, cert: Cert transfers X509_free on 1) -> i32
function CMS_add0_cert(cms: Cms, cert: Cert transfers X509_free) -> i32 when 1
```

Both placements imply the same C at the call. From the patched unit that ran:

```c
hv_transfer_pre(t13, "json_object_array_put_idx");  /* before C: the set must hold val */
t14 = json_object_array_put_idx(t11, t12, t13);      /* unchanged */
hv_transfer_post(t13, t14 == 0);                     /* obligation ends only on success */
```

- **Which can an author write from the header?** The result placement asks only for what headers state: a success value, under `@return` or `RETURN VALUES`. Per-parameter conditionality is stated by 0 of the 8 rows above.
- **Which fails loudly when wrong?** The same C gives the same loudness. A wrong value exits 134 at exit on the common success path, on 3 legs. But the natural repair its message suggests, putting the child, is a use-after-free inside json-c at exit 0 (Guard Malloc 139; the right clause is 0 under the same instrument).
- **The difference:** the parameter placement adds two wrong spellings the result placement cannot express. One is a clause on a `void` function. The other is two parameters disagreeing, which `ECDSA_SIG_set0` shows is never true.

**Q2, `retains`, as adopted.** Today both programs exit 134 (the stray message). Emulated, both exit 0 on 3 legs and are Guard-Malloc clean.

```c
t16 = X509_up_ref(t15);   if (t16 == 1) hv_retained(t15);  /* page: "returns 1 for success and 0 for failure" */
t8 = json_object_get(t7); hv_retained(t8);                  /* live: +1; a borrowed child: first life */
```

## Found in what ships (for the coordinator)

1. **A false message on a stale handle.** A use-after-free inside `libcrypto` is reported as *"a handle or `ptr` holding `nullptr` reached C"* when the program had just printed `mine is null: false`. 134, three of three, Darwin; Linux gives 139 with no message. The rule is `runtime/parts/stack.c:502-504`. No record names it.
2. **`--sanitize` cannot see a use-after-free inside an uninstrumented library.**
   - `sqlite_copies`: ASan exits 0 on all four copy shapes.
   - `ossl_alias`: ASan reports `SEGV on unknown address` inside `libcrypto`, not a use-after-free.
   - So wart 20's *"The remedy today is `--sanitize`, which names the line"* holds for its reproducer and not for handles given to real libraries. Panel 167 noted the same for a lease.
3. **json-c 0.19's `json_object_put` return value contradicts its header.** A freed int returns 0; an object that is not freed returns 1.
4. **`CMS_add0_cert` on an equal certificate** returns 1 and frees its argument.

## Unrun

- Windows, everything.
- S, M and A as compilers; `s_struct` on Linux.
- The failure paths of `X509_up_ref` and `json_object_array_add`.
- Timing of any route.
- A real TLS handshake in which libssl makes the verify callback's `X509_STORE_CTX`; `cb_reuse` is a header model of it.

## Files

All under `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e7e3d0df-8db3-44f2-b628-af9bb1a6080e/scratchpad/177-ffi-pragmatist/`:
- the emulation kit: `exp/patch.py`, `hv177.c`, `hv177.h`, `run.sh`, `cc.sh`, `linux.sh`, `linux_t.sh`, `darwin_t.sh`, `runtime/`;
- the programs: `work/{repro,r,ossl,p,x,t,s,q2,cjson}/`;
- the Linux results: `linux-out/{arm64,x86_64,arm64_t,x86_64_t}.txt`.
