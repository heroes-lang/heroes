# Panel 154 — report of the ffi-pragmatist

**Verdicts** — **A: object** · **B: adopt** · **C: refuse** · **D: refuse, with veto** · **E: object** · **F (sixth route, mine): adopt** — F is B plus two things B does not say: the guard's C spelling, fixed by measurement, and the written boundary of the class.

**The § I stand on**: design.md §1.12's *Defensive, and the cost is real* paragraph, which already decided the default — *"they check at the boundary even where the value is known to be good … because the alternative is a rule that has to reason about where a value came from, and provenance is a premise about the world. That is one predictable branch per FFI argument, and it is paid deliberately"* — together with the same section's completeness clause, *"any C library must be bindable"*. Routes C and D are provenance rules; B is the paragraph applied. §4.19's ladder and `.claude/rules/c-boundary.md` price the rest.

All work in `/private/tmp/claude-501/-Users-joseph-Temp-heroes-lang/8d7432e4-9b44-486f-80db-fda4fdc56efe/scratchpad/p154-ffi` (copy, seed built in `real 3.09` s, exit 0) and `…/scratchpad/p154-work`. Nothing was written into the repository.

## 1. The census nobody had taken: how often does C accept NULL?

Instrument: `clang -isysroot $(xcrun --show-sdk-path) -Xclang -ast-dump=json -fsyntax-only` over one TU per header, attributing each `FunctionDecl` to its declaring file (`…/p154-work/fam2.py`). Apple splits two of the brief's headers, measured: **`stdlib.h` and `time.h` declare 0 functions themselves** — the bodies live in `_stdlib.h`, `malloc/_malloc.h`, `_stdio.h`, `_time.h` — so each row below is that header's *family*, named in the script.

| header family | functions | with ≥1 pointer param | pointer params |
|---|---|---|---|
| `stdlib.h` (+`_stdlib.h`, `malloc/_malloc*.h`, `alloca.h`, `_abort.h`) | 129 | 87 | 134 |
| `stdio.h` (+`_stdio.h`, `_printf.h`, `_ctermid.h`) | 86 | 81 | 125 |
| `dirent.h` | 19 | 18 | 32 |
| `time.h` (`_time.h`) | 27 | 20 | 29 |
| `netdb.h` | 39 | 18 | 31 |
| `sqlite3.h` (3.51.0, SDK) | 284 | 253 | 465 |
| `curl/curl.h` (+`easy.h`, `multi.h`, libcurl 8.7.1) | 69 | 58 | 99 |
| **total** | **653** | **535** | **915** |

**A documentation census of all 915 was not run — say it plainly, it is unrun.** What I ran: (a) a mechanical grep of `sqlite3.h`'s own doc blocks — **32 of 284 functions** carry a NULL-permitting phrase (`…/p154-work/sqlnull.py`; false positives possible, phrase list in the script); (b) hand-read contracts with citations:

| permitted, and where I read it | refused, and where I read it |
|---|---|
| `free` — `man 3 free`: *"If ptr is a NULL pointer, no operation is performed"* | `fclose` — `man 3 fclose` NOTES: *"does not handle NULL arguments; they will result in a segmentation violation. This is intentional"* |
| `sqlite3_close`/`_close_v2` — `sqlite3.h:349-350` *"harmless no-op"* | `closedir`, `readdir` — `man 3 closedir` says nothing; **measured SIGSEGV** |
| `sqlite3_finalize` — `sqlite3.h:5556`; `sqlite3_free` — `:3226`; `_free_filename` — `:4145`; `_value_free` — `:6131`; `_blob_close` — `:8018` | `freeaddrinfo` — `man 3 getaddrinfo` names it 5 times and **never permits NULL** |
| `sqlite3_randomness(P)` — `:3330`; `sqlite3_uri_parameter(F)` — `:4027` (both **uses**, not releases) | |
| `fflush(NULL)` — `man 3 fflush`; `time(tloc)` — `man 3 time`; `getaddrinfo(hints)` — `man 3 getaddrinfo`: *"If hints is the null pointer…"* (an **unmarked use**) | |
| `curl_easy_cleanup` — SDK `man 3 curl_easy_cleanup`: *"Passing in a NULL pointer in handle makes this function return immediately with no action"* | |

**Correction to the shared brief.** Its premise sentence names three calls as "legal C". Two are documented (`free`, `sqlite3_close`); **`freeaddrinfo(NULL)` is not documented anywhere on this Mac** — it merely works here. A rule argued from it would rest on an undocumented behaviour.

**And the sharpest fact in the whole census**: `fclose`'s documented contract says NULL *segfaults*, while the shipped libc **returns `-1` with `errno=14` (EFAULT) and exits 0**. Documentation and implementation disagree, in opposite directions, on the most basic library on the machine. No rule may infer NULL legality from either.

## 2. The compiles and the runs. Every exit code

Behaviour probe, one `fork` per call, `clang -O2 -std=gnu11 probe_null.c -lsqlite3 -lcurl` → **exit 0, empty stderr** (`…/p154-work/probe_null.c`; the listing prints twice — my harness's children inherit an unflushed stdout, an artefact of the harness and not of any library):

- **24 of 26 entry points tolerated NULL**: `sqlite3_close` rc=0, `_finalize` rc=0, `_step` rc=21 (SQLITE_MISUSE), `_reset`, `_exec`, `_errmsg`, `_changes`, `_prepare_v2`, `_bind_int64/_double/_text`, `_column_count/_type/_int/_int64/_double/_text` — **17 of 17 "uses" tolerated it** — plus `curl_easy_setopt` rc=43, `curl_easy_cleanup`, `curl_easy_strerror`, `freeaddrinfo`, `free`, `fclose` rc=-1, `fflush(NULL)` rc=0.
- **2 crashed: `closedir` and `readdir`, SIGNAL 11**, identically at `-O0` and `-O2`.

Heroes programs built with `HEROES_RUNTIME=…/p154-ffi/runtime ./heroes build`:

| program (all build exit 0, empty stderr) | -O0 | -O2 |
|---|---|---|
| `p154-work/dirnull.hero` — real `dirent.h`, `opendir` → nullptr, `closedir(d)` | **134**, `panic: a null pointer was read through … called from dirnull.main` | **134**, same panic, `… called from pthread_mutex_lock` |
| `p154-work/dirnull_guarded.hero` — same, with `if d == nullptr` | **0**, `opendir gave nullptr` | **0**, same |
| `p154-work/closenull.hero` — `db: Db @ nullptr`, `sqlite3_close(db)` | **0**, `sqlite3_close(nullptr) returned 0 and SQLITE_OK is 0` | **0**, same |
| `p154-work/shimnull.hero` — §4.19's own shim shape: `static inline` release marked `consumes` | **134**, named panic | **133, both streams empty** |
| `p154-work/sdlstore.hero` — real packaged SDL3, `SDL_RectToFRect` (`SDL_FORCE_INLINE`, `SDL_rect.h:129`), `ptr` args | **134**, named panic | **133, both streams empty** |

**This narrows defect 045's class, and the brief does not state the boundary.** When the dereference is out of line in a `.dylib` the fault is unfoldable and `runtime/parts/stack.c` names it **at `-O2` too** (`dirnull`, exit 134 both levels). The `-O2` hole needs the dereference **visible in the Heroes translation unit**: `static inline`. Measured: **zero** `static inline` definitions in `sqlite3.h`, `curl.h`, `netdb.h`, `dirent.h`, `_stdio.h`, `_stdlib.h`, `_time.h`, `raylib.h`; **12** pointer-taking `SDL_FORCE_INLINE` definitions in `/opt/homebrew/include/SDL3/*.h`. So the class is real and it lives in header-inline code and in shims — which §4.19 recommends by name — and it reaches through **`ptr` as well as handles** (`sdlstore.hero` uses `ptr`).

## 3. The guard's C spelling. This is where my veto nearly fired

`…/p154-work/guard.h`, three spellings, compiled against the real `sqlite3.h` under the project's own fourteen flags (`selfhost/cli/flags.hero:37-52`, replayed verbatim), with the right call and with defect 029's wrong call (`sqlite3_step(db)` where `db` is `sqlite3 *`):

| spelling | right call | **wrong handle** | first stderr line |
|---|---|---|---|
| 1 — `void *hero_ptr_nonnull(void *)`, the `cstr` guard widened | 0 | **0 — compiles, zero diagnostics** | (none) |
| 2 — `(hero_ptr_check(x), (x))`, comma, type preserved | 0 | **1** | `wrong2.c:4:22: error: incompatible pointer types passing 'sqlite3 *' (aka 'struct sqlite3 *') to paramete…` |
| 3 — statement expression with `__typeof__` | 0 | **1** | `wrong3.c:4:22: error: incompatible pointer types passing 'typeof (db)' (aka 'struct sqlite3 *') to parame…` |

And spelling 2 at `-O2`: **exit 134**, `panic: a null handle was passed to a C function`.

**Spelling 1 is a veto.** §4.19's mechanism is that *"clang checks the call and nothing else"*; widening `hero_cstr_nonnull`'s signature to `void *` deletes that check on every handle argument and re-opens defect 029's silent-wrong-answer class at the clang half of the boundary, at exit 0, under this project's own `-Werror=incompatible-pointer-types`. Spelling 2 keeps both checks and is free: the emitted C already lowers **every** handle argument to a temporary — `sqlite3_step(t17)`, `sqlite3_close(t24)`, `curl_easy_cleanup(t73)`, 26 of 26 sites in `/tmp/p154_sqlite.c`, `/tmp/p154_curl.c`, `/tmp/p154_ledger.c` — so the comma evaluates a local twice and nothing else. Spelling 3 works but prints `'typeof (db)'` where spelling 2 prints `'sqlite3 *'`; §4.17 makes that a real difference.

**No route touches the ABI.** The guard passes the identical pointer value, adds no marshalling and no boxing, and leaves the `_Static_assert` and `hero_ffi_probe_*` lines untouched. No veto on A, B, C, E or F on ABI grounds.

## 4. Pricing the routes on the shipped bindings

Baseline, before anything: `examples/sqlite/main.hero`, `examples/curl/main.hero`, `examples/ledger/main.hero` each **build exit 0** at `-O0` and `-O2` and run at **exit 0** (`rows: 3` / `longest: 6`; `libcurl/8.7.1 …` / `the url was accepted`; `the book, as Heroes reads it`). The "after" is hand-written C compiled against the real header, **not** a compiler change: implementing a route is unrun.

Declared handle parameters across the three bindings (`record … tag …` types only): **28** — 4 `acquires` out-params, **5 `consumes`**, **19 unmarked**. Handle-argument **call sites in the emitted C**: **26** (sqlite 7, curl 2, ledger 17).

| route | words the author writes | sites that gain a guard | refuses a call the corpus makes today |
|---|---|---|---|
| A | 0 | **26 of 26** | yes, at runtime: `examples/sqlite/main.hero:109` and the two in `examples/ledger/` close a handle on the failure path, and `sqlite3.h:3691-3695` says that handle **is NULL when allocation fails**, where `:349-350` makes the close a harmless no-op. Measured today: exit 0, `returned 0`. Under A: exit 134 on the out-of-memory path. The escape exists and costs one `if` — `dirnull_guarded.hero` proves a skipped release does **not** trip the acquire ledger (exit 0) |
| B | **5** (`sqlite3_close` ×2, `sqlite3_finalize` ×2, `curl_easy_cleanup`) | **19 of 26** | no |
| C | **23** | 23 if none is forgotten | no — and a forgotten word is a silent hole |
| D | 0 | **19 of 26 — byte-identical to B on this corpus** | no |
| E | 0 | 0, and 0 bytes of emitted C change | no, and it closes nothing here |

## 5. Route D's premise, tested directly — and why I veto it

D says *a `consumes` parameter is a release and releases usually accept NULL; an unmarked one is a use and uses usually do not.* Both halves fail on the headers I counted:

- **Releases that refuse NULL exist and are one man page away**: `closedir` (SIGSEGV 11, measured) and `fclose`, whose man page says segfault *intentionally*. A Heroes binding writes `closedir(handle: Dir consumes)` — I compiled exactly that — so D skips the guard precisely where C crashes.
- **Uses that accept NULL are everywhere**: 17 of 17 SQLite uses tolerated it; `getaddrinfo(hints)` and `sqlite3_uri_parameter(F)` are *documented* NULL-legal unmarked uses. D guards them for nothing.
- **And D and B emit identical C on all three shipped bindings** — 19 of 26 sites. D is therefore not a cheaper B; it is B's default *guessed*, and the guess is free only until the first binding it gets wrong.

**The veto ground is narrower and it is mine**: D makes one mark mean two orthogonal things, so **no spelling remains for "a release that must be guarded"**. `p154-work/shimnull.hero` is that binding, in the shape §4.19 recommends by name, and under D its `-O2` behaviour is **exit 133 with both streams empty** — an anonymous death, no diagnostic, nothing the binding author can write to fix it. A route that makes a category of binding unprotectable with no available spelling is categorically harder, and that is the refusal my seat holds.

**Why A is only an objection**: A is sound and keeps clang's check. It costs one `if` at each documented-legal NULL release — small — but it charges that `if` to every binding author forever to pay for a class that `runtime/parts/stack.c` already names whenever the dereference is out of line, and it refuses the C idiom (`sqlite3_close(NULL)`, `curl_easy_cleanup(NULL)`) that two shipped bindings sit next to. **B costs 5 words, refuses nothing, and a forgotten word is safe.** §1.12 decides between them: safe by default, and the completeness clause forbids a boundary that cannot express the library's own contract.

**Why E is only an objection**: the witness is a runtime value. `opendir` returns NULL at run time and `db: Db @ nullptr` reaches C through three call hops; a static-nullptr rule sees neither. E changes zero bytes and closes zero of my five witnesses. Harmless as an addition to B, insufficient alone.

**Route F, mine**: B, with (i) the guard emitted as `(hero_ptr_check(t), t)` — never `void *` in/out, on the measurement in §3; (ii) the class written as *handle-typed arguments only*, because `ptr` legitimately carries `nullptr` — `examples/sqlite/main.hero:73` passes three of them at one call site; (iii) the `-O2` boundary recorded in §4.19 as the measurement in §2, so the next reader knows the runtime already names the out-of-line case and that `static inline` and shims are the residue.

## 6. Prediction, and the command that scores it

Under **F** (B + comma guard), **step 3 of §4.19's ladder needs no shim and one word twice**: `examples/sqlite/main.hero` builds and runs unchanged at `-O0` and `-O2` printing `rows: 3` / `longest: 6`, with `accepts_null` on `sqlite3_close` and `sqlite3_finalize` only, and the emitted C gains a guard at **exactly 4 sites in sqlite, 1 in curl, 14 in ledger — 19 of 26 — and 0 at the 7 sites of those two declarations plus `curl_easy_cleanup`**:

```sh
for ex in sqlite curl ledger; do
  src=examples/$ex/main.hero; [ $ex = ledger ] && src=examples/ledger/main.hero
  HEROES_RUNTIME=$PWD/runtime ./heroes build $src --emit-c -o /tmp/after_$ex.c
  printf "%s guards=%s\n" $ex "$(diff /tmp/p154_$ex.c /tmp/after_$ex.c | grep -c hero_ptr_check)"
  HEROES_RUNTIME=$PWD/runtime ./heroes build $src -O2 -o /tmp/after_$ex && /tmp/after_$ex | head -2
done
# scores 4 / 1 / 14 and the three baselines of §4 byte for byte; anything else falsifies me
```

**What would change my verdict.** B becomes a **veto** if the guard ships spelled `void *`-in/`void *`-out: measured, the wrong-handle call then compiles at exit 0 with zero diagnostics under the project's own fourteen flags. B becomes an **objection** if the word is made mandatory rather than optional, since that makes every existing binding a compile error. **D returns from refusal** only on a header-derivable NULL contract — an attribute, a `_Nonnull` qualifier, anything clang can read — with a measured agreement rate against the documentation I cite in §1; `fclose`'s man page against `fclose`'s behaviour is the case it has to beat.