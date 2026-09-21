# Panel 172 — ffi-pragmatist

Every number below was produced by a command run on 2026-09-21 in
`<scratch>/ffi-pragmatist-172` (a copy of `abf9a17e`, `heroes` built from the
seed) against this Mac's SDK — Apple clang 21.0.0, `MacOSX.sdk`, SQLite
**3.51.0** (`sqlite3.h:155`), libcurl **8.7.1** (`curlver.h:35`). The command is
beside the number.

## Verdict

**VETO on route B. OBJECT to route A as written. VETO on any implementation of
either that emits address-keyed runtime bookkeeping for a `ptr` or `cstr`.**

Route B's keeps-word cannot be written truthfully in the two headers this
project actually binds. Route A is ABI-safe only if it emits nothing, and the
one existing mechanism it would naturally reuse produces a **false abort at exit
134 on a correct program** — compiled and run below.

The resolution I put on the table is **route C plus what already exists**:
defect 070's shape has a correct spelling today, measured `check` 0, `run` 0,
**0 AddressSanitizer lines**, and it needs no new word.

## Section

**design.md §1.11** (the founding constraint) and **§4.19**, and the section
that decides this sitting is **design.md :2337-2341**, which already contains
the measurement route B would have to overturn:

> no declaration-site mark can express retention at all: `sqlite3.h:4888` puts
> the decision in the **fifth argument** of one declaration, `curl_easy_setopt`
> in its second, and 0 of 71 `cstr` parameters in this tree are decidable from a
> header.

Panel 124 measured that in 2026-09-09. **It re-measures true on today's SDK**
(§4 below). Route B proposes a word whose home document already records that it
cannot be written.

Also load-bearing: **CLAUDE.md § Precedence rank 3** (robustness beats
ergonomics and compiler size) for the veto, and **§12 / CL-005** for route C's
refusal needing a named falsifier.

---

## 1. The classification, compiled against the SDK

`$W/classify.c`, nine ownership shapes, each the C a Heroes binding must be true
about.

    clang -fsyntax-only -Wall -Wextra -I$(xcrun --show-sdk-path)/usr/include $W/classify.c
    EXIT=0          # no warnings

| C function | what it really does | is `consumes` (*C frees what it is handed*) truthful? |
|---|---|---|
| `free(void*)` | frees | **TRUE** |
| `sqlite3_free(void*)` | frees, **only** what `sqlite3_malloc` made | TRUE, with a heap caveat no word carries |
| `freeaddrinfo(struct addrinfo*)` | frees a list **C** made | TRUE, and the pointer is C's already |
| `putenv(char*)` | **keeps forever, never frees** (POSIX: becomes part of the environment) | **FALSE.** And no keeps-word helps: there is no later function to name, because nothing ever frees it |
| `strdup(const char*)` | copies; frees nothing | **FALSE** |
| `sqlite3_bind_text(...,const char*,int,void(*)(void*))` 3rd param | **consumes, keeps, or copies — chosen by the 5th argument, per call** | no word is true; see §4 |
| `curl_easy_setopt(CURL*, CURLoption, ...)` | keeps or copies, per option | **there is no parameter to word**: `easy.h:42` is `...` |
| `CFRelease(CFTypeRef)` | drops a reference, not a malloc block | TRUE only if "consumes" means refcount, which the word does not say |
| `execv`'s `argv` | keeps nothing; on success never returns | neither word applies |

**The case the brief asks about — *keeps now, frees later with a function I
name*.** That is `sqlite3_bind_text`/`bind_blob` with a real destructor, and
**no word in route A or route B is true of it**, because the function that frees
is an **argument**, not a name in the declaration. The only true statement about
it is the one the program makes at the call site by handing the disposer over —
which is exactly measurement 037's R5, and the language already compiles it
(§2).

### And the type system cannot tell these apart

`$W/indistinguishable.c`: three functions of identical C type, one freeing, one
keeping, one reading, assigned to each other through one `typedef`; plus
`bind_text` called three times with `SQLITE_STATIC`, `SQLITE_TRANSIENT` and
`free`.

    clang -Wall -Wextra -Wincompatible-pointer-types -fsyntax-only $W/indistinguishable.c
    EXIT=0

clang accepts every assignment and every call. **Ownership is invisible to the C
type**, so it is invisible to `importc`-style header verification. That is the
one property this project guards jealously at the boundary, and `consumes` is
the first mark that falls outside it.

---

## 2. Measurement 037, re-run

Program and `r5.h` reconstructed from the record (`docs/measurements/037-…`,
lines 18-35 and 37-39).

    clang -fsyntax-only -Wall -Wextra -x c $W/r5.h                      EXIT=0 (3 unused-static warnings)
    CPATH=$W ./heroes check $W/r5.hero                                  CHECK_EXIT=0
    CPATH=$W ./heroes run   $W/r5.hero                                  RUN_EXIT=0, prints 72
    CPATH=$W ./heroes build --sanitize $W/r5.hero -o $W/r5bin           BUILD_EXIT=0
    $W/r5bin                                                            BIN_EXIT=0
    grep -c AddressSanitizer $W/r5.err                                  0
    wc -c < $W/r5.err                                                   0

**037 reproduces exactly.**

**Under route A: nothing changes.** Measured, not inferred —
`grep -c '\.lease()' $W/r5.hero` → **0**, `grep -c 'lent' $W/r5.hero` → **0**.
Route A adds a refusal at lend and lease landings; this program has neither.

**Under route B: the program survives but the binding must state a falsehood.**
`b` comes from the program's own `malloc`, so it is "a pointer C made" and
reaches an unmarked parameter. But route B says *three words then say what a
parameter does*, and `lib_keep`'s `p` — **keeps past the call and later frees it
with the disposer it was handed** — is described by none of them. `lent` is
false, the keeps-word is false (it does free), `consumes` is false (it does not
free during this call). **Route B forces a wrong word onto the one give-away
route in the repository that works.**

---

## 3. The ABI question, measured

Take a declaration that carries a mark today (`consumes` is legal on a handle)
and emit C with and without it.

    cp tests/golden/run/ffi-acquires-and-consumes.hero  $W/abi/with_mark.hero
    sed 's/(s: Slot consumes)/(s: Slot)/' … > $W/abi/no_mark.hero
    CPATH=$W/abi ./heroes build --emit-c $W/abi/with_mark.hero -o $W/abi/with.c   EXIT=0
    CPATH=$W/abi ./heroes build --emit-c $W/abi/no_mark.hero   -o $W/abi/no.c     EXIT=0
    diff -u $W/abi/no.c $W/abi/with.c                                              EXIT=1

Setting aside the module-name renaming, **the entire semantic diff is two
lines**:

    +    hero_handle_consumed(t10);
    +    hero_handle_consumed(t11);

The C call is byte-identical — `(void)slot_close(t10);` — and so is the
verification probe:

    __attribute__((unused)) static void hero_ffi_probe_…_slot_close(Slot * a0) { (void)(slot_close)(a0); }

**So a mark reaches no C ABI today: no argument, no type, no layout, no
calling convention.** On that narrow question route A is safe, and I would not
veto it.

### The veto, and it is measured rather than argued

Route A's `consumes` on a `ptr`/`cstr` must emit **nothing**. If it is
implemented by symmetry with the handle case — reusing `hero_handle_consumed`,
the only machinery the marks sweep has — it breaks correct programs. The live
set is **address-keyed** and aborts on an address it never registered
(`runtime/parts/alloc.c:422`, `:436-441`).

    $W/veto2.c:  hero_handle_acquired(h);     /* any program holding one real handle */
                 void *p = malloc(8);          /* an ordinary buffer */
                 hero_handle_consumed(p);      /* what a consuming free(p) would emit */

    clang -I runtime $W/veto2.c runtime/runtime.c -o $W/veto2.bin      COMPILE_EXIT=0
    $W/veto2.bin                                                       RUN_EXIT=134

    panic: 1 C handle(s) given back that were never taken — the set of live
    handles did not hold that address when a call marked `consumes` ran. …
    The first is at 0x104fa9ee0

**Exit 134 on a correct `free`.** Every SQLite program and every curl program
holds a real handle, so the set is always allocated and this always fires. (With
no handle ever acquired, `hero_handle_cap == 0` and the same program exits 0 —
`$W/veto.bin`, `RUN_EXIT=0` — so the failure is *conditional on the rest of the
program*, which is the worst shape a diagnostic can have.)

**VETO CONDITION, stated so it can be checked:** I veto any landing in which
`consumes` on a `ptr` or `cstr` parameter causes the emitter to write
`hero_handle_acquired`, `hero_handle_consumed`, or any other address-keyed
runtime call. The mark must be **compile-time only** and `--emit-c` must be
byte-identical with and without it. Robustness is rank 3 in § Precedence and
this is a corruption-class abort introduced by a comprehension feature.

---

## 4. Route B's cost on real headers, and the finding that decides it

    grep -o 'const char \*' $SDK/usr/include/sqlite3.h      | wc -l   → 125
    grep -o 'void \*'       $SDK/usr/include/sqlite3.h      | wc -l   → 100
    grep -o 'const char \*' $SDK/usr/include/curl/curl.h    | wc -l   →  45
    grep -o 'void \*'       $SDK/usr/include/curl/curl.h    | wc -l   →  42

(These are **occurrences**, `grep -o | wc -l`. `grep -c` counts lines and reads
119 / 98 / 43 / 40 — a different ruler, recorded so the two are not confused.)

What a binding would have to word:

    grep -c '^    function ' examples/ledger/db/sqlite.hero          → 18
    grep -o ': cstr' examples/ledger/db/sqlite.hero | wc -l          →  8
    grep -o ': ptr'  examples/ledger/db/sqlite.hero | wc -l          →  5
    grep -o 'cstr lent\|ptr lent' examples/ledger/db/sqlite.hero|wc -l→ 3
    grep -rho ': cstr\|: ptr' examples tests/golden selfhost --include='*.hero' | wc -l → 430

So the ledger's 18 functions carry **13** pointer parameters, **3** already
worded, **10 new words**; and the corpus carries **430** `cstr`/`ptr` parameter
tokens across **159** files with an `extern` group.

### The question that decides route B: is the keeps-word ever TRUE at a declaration?

**On these two headers, essentially never — and where it is available it is the
wrong word.** Three findings, each from the header text:

1. **`sqlite3_bind_text` is per call.** Prototype at `sqlite3.h:4958`:
   `int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));`
   The header's own prose, `:4888-4903`, states all three outcomes for the
   **third** parameter, selected by the **fifth**: a destructor (*frees*),
   `SQLITE_STATIC` (*keeps; the application remains responsible*),
   `SQLITE_TRANSIENT` (*copied prior to the return*). One declaration, three
   ownership contracts. No word on the parameter is true.
2. **`curl_easy_setopt` has no parameter to word.** `easy.h:42`:
   `CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);` The keeper
   (`CURLOPT_POSTFIELDS`) and the copier (`CURLOPT_URL`) both arrive through
   `...`. Route B's rule — *every pointer parameter carries a word* — cannot
   even be applied here, because there is no pointer parameter.
3. **The one declaration-level keeper I found is stricter than route B's word,
   not equal to it.** `sqlite3.h:4926`, `sqlite3_bind_pointer`'s `T` parameter:
   *"The T parameter should be a static string, preferably a string literal."*
   That demands **static storage duration**. Route B's keeps-word **admits a
   lease**, and a lease is freed by `end_lease`. So on the single parameter in
   this header where a declaration-level retention contract exists, route B's
   word would **admit exactly the bug it exists to prevent**.

I searched `sqlite3.h` for `must remain valid`, `remain valid`, `does not make a
copy`, `makes a copy`, `is not copied`, `lifetime of`, `static string`, `string
literal`, `constant string`, and `curl.h` for `must remain`, `not copied`,
`copies the`, `strdup`. **Ten hits in sqlite3.h and four in curl.h**, and every
one is either a per-call contract, a copy notice, or the `bind_pointer`/
`result_pointer` static-string requirement above. **That is the vocabulary I
searched; a keeper I missed would falsify this paragraph.**

**Finding, in the brief's own words: route B's keeps-word cannot be written
truthfully where it is needed.** And the corpus already knows it —
`examples/ledger/db/sqlite.hero:348-354` and `examples/curl/main.hero:70-76` each
carry a comment saying no one word is true of that parameter and that the bytes
therefore go over as a lease.

---

## 5. `strdup`, and what the notes owe a binding author

The contrast, measured, and it is this project's thesis arriving at the boundary
and then stopping:

    # a wrong TYPE
    CPATH=$W ./heroes build $W/wrongtype.hero -o $W/wt.bin       EXIT=1
    error[ffi_return_type]: `strdup` does not return `i64` — that is what
      `string.h` says, and clang read it

    # a right signature with a FALSE ownership claim
    CPATH=$W ./heroes build $W/wrongown.hero -o $W/wo.bin        EXIT=0   (builds, runs)

And the two probes are the same C:

    __attribute__((unused)) static void hero_ffi_probe_…_free(void * a0)        { (void)(free)(a0); }
    __attribute__((unused)) static void hero_ffi_probe_…_strdup(const char * a0){ (void)(strdup)(a0); }

**A wrong type is a compile error because a header is the judge. A wrong
ownership word has no judge at all.** That is why `unread-mark.hero:26` matters:
under route A that line becomes a **legal declaration of a falsehood**, and the
falsehood is load-bearing — it tells the checker to refuse the leases that are
correct and to admit whatever is not.

**Should the notes carry a test? Yes — but not the one the brief proposes.**
*"The man page says the pointer is freed or owned by the callee"* is refuted by
two rows of §1's table: `putenv` is **owned and never freed** (the man page says
owned; `consumes` is still false, and no word is true), and `bind_text` says all
three things in one paragraph. A test that misfires on the two most cited
examples in this sitting's own briefs will be applied wrongly by exactly the
reader it is written for.

The test I would put in the notes, in three questions in this order:

> 1. Does the documentation name **an argument** that disposes of this pointer
>    (a destructor, a free-function parameter, a `SQLITE_STATIC`-style flag)? If
>    yes, **no word on this parameter is true** — the ownership is decided per
>    call. Hand the buffer over the way `docs/measurements/037` does: allocate
>    through the library's own allocator and pass the disposer.
> 2. Otherwise, does it say this call **frees, destroys or takes ownership of**
>    the pointer, unconditionally, for every call? Only then is `consumes` true.
> 3. If it says the pointer must **outlive** the call but names nothing that
>    frees it (`putenv`, `bind_pointer`'s `T`), `consumes` is false and so is
>    any keeps-word that admits a lease: it needs **static storage duration**,
>    which a lease does not have.

Question 1 is the one that matters, and it is the one no ecosystem in panel
167's survey asks.

---

## What I would land instead

**Defect 070's shape has a correct spelling today.** `$W/g70/today.hero`:
allocate through the program's own `malloc`, `memcpy` the bytes in, end the
lease, hand C's own pointer to C's freer.

    CPATH=$W/g70 ./heroes check $W/g70/today.hero                      CHECK_EXIT=0
    CPATH=$W/g70 ./heroes run   $W/g70/today.hero                      RUN_EXIT=0
    CPATH=$W/g70 ./heroes build --sanitize … -o $W/g70/today.bin       SANBUILD_EXIT=0
    $W/g70/today.bin                                                   SANRUN_EXIT=0
    grep -c AddressSanitizer $W/g70/err                                0
    wc -c < $W/g70/err                                                 0

against the defect, re-measured, ten runs:

    CPATH=docs/panel/172-briefs ./heroes build …/lease070.hero -o $W/lease070.bin   EXIT=0
    exit/stderr-bytes: 133/0 134/0 133/0 133/0 133/0 133/0 133/0 133/0 133/0 133/0

And **today's `unread_mark` already prescribes the repair**, better than route A
would. Re-run verbatim (`…/consumes070.hero`, `CHECK_EXIT=1`):

> `consumes` on `s` is a word nothing reads: `cstr` reaches no handle, so no
> rule can say which call hands the value over or takes it back, **and the word
> could be false without anything noticing**
> note: … **Name the C type the pointer stands for and mark that**: `record
> Block tag void` is C's `void *` …

Route A's premise is that this refusal is in the way. The refusal is **right**,
it says why in its own words, and the reason it gives — *the word could be false
without anything noticing* — is precisely what §5 above measured at exit 0.
Widening it deletes a true sentence to admit an uncheckable one. Route C should
write that limit down, with §1 question 1 as the falsifier CL-005 requires.

## The falsifiable prediction

Instrument: **`heroes run tests/harness/main.hero -- ./heroes corpus`**, the
suite `.claude/rules/verification.md` names for `examples/**`, and
`examples/ledger/db/sqlite.hero`, which exists today.

> **Under route A, `examples/ledger/db/sqlite.hero` needs zero words added and
> zero lines changed, and `corpus` stays green.** Under route B it needs
> **10** new words (13 pointer parameters minus the 3 already `lent`), and at
> least one of them — `sqlite3_bind_text`'s `text` parameter, which this binding
> calls with `SQLITE_TRANSIENT` at `:355` — **cannot be written truthfully**, so
> either `corpus` goes red or the shipped binding states a falsehood the
> compiler will then act on.

Falsify it by producing a word, spelled any way you like, that is true of
`sqlite3_bind_text`'s third parameter for **all three** values of its fifth.

## Condition — what would change my verdict

- **On route B:** a keeps-word demonstrated TRUE at the declaration for at least
  one `const char *` parameter in `sqlite3.h` or `curl/curl.h` on this SDK,
  *where a lease is a legal argument*. `bind_pointer`'s `T` does not count: it
  needs static storage duration. Produce one and route B becomes a cost argument
  instead of an impossibility.
- **On route A:** I lift the objection if (a) the mark is compile-time only, (b)
  `--emit-c` is byte-identical with and without it on a `tests/golden/emit/`
  case, verified by the `emit` and `determinism` suites, and (c) the diagnostic
  carries §5's three-question test, so a binding author is told that a
  destructor **argument** means no word is true. Without (c), route A ships
  `unread-mark.hero:26`'s falsehood as a supported form.
- **The veto does not lift.** Address-keyed runtime bookkeeping for a `ptr` or
  `cstr` is exit 134 on a correct program, measured above.

## Unrun, and named

- **Linux and Windows**, for everything here. Darwin arm64 only
  (`.claude/rules/platforms.md`), and §3's veto experiment is the one I would
  most want re-run on the Linux leg, where LeakSanitizer exists.
- **glibc's `putenv`** — whether it frees a previously-`putenv`'d string on a
  later `setenv`. I did not run it; this Mac has no glibc. It is a question, not
  a premise, and it does not change §1's row: `consumes` is false of `putenv` on
  either answer, because the call being marked does not free.
- **`g_free`/`g_strdup`** — glib is not on this SDK. Searched
  `$(xcrun --show-sdk-path)/usr/include` and found no `glib.h`; not classified.
- **The full net.** I ran no suite; every measurement here is a direct command.
