# Panel 169 — ffi-pragmatist

**verdict: veto** — on **R4** (withdraw the field lend), unchanged in force and on
new ground. **object** to **R1** as the answer to 066. **approve R2** for 068.
**And the route nobody listed is R5: give the ownership away, which every rung of
§4.19's ladder already offers and defect 069 is the only thing blocking.**

**section:** design.md §1.11 (the founding constraint) and §4.19 — specifically
the sentence *"Clang verifies the declared signature against the real header"*,
and the four-rung acceptance ladder, rungs 3 to 5.

Every number below was produced by a command run in this session, 2026-09-20,
Darwin 25.6.0 arm64, in a copy of the tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/tree`
(`rm -rf target build` done; compiler from the seed in `real 3.99 user 3.91
sys 0.08`). Libraries as installed here: **sqlite 3.51.0**, **raylib 6.0**
header, **LIBCURL_VERSION 8.7.1** (SDK). Sources of every experiment are in
`…/scratchpad/w/`.

---

## experiment

### 1. R1 works, for the happy path, with no shim — and I wrote it

`w/r1_static.hero`: `sqlite3_bind_text` with **SQLITE_STATIC**, the buffer a
`record Block tag void` acquired from `malloc` and consumed by `free`, the bytes
copied in by `memcpy` from a `cstr` lend that lives one call.

    ./heroes check   exit 0
    ./heroes build   exit 0
    ./heroes run     exit 0 ten of ten, "stored: the honest answer"

Three refusals had to be answered on the way and all three were correct and
self-repairing: `error[unmarked_handle_producer]` on `memcpy`'s `void *` result
(repair: `borrows`), `error[ffi_parameter_type]` five times on `int`/`size_t`
widths, and `error[bad_operand]` on `to_str` of a `cstr`. **`check` was 0 while
`build` was 1 at both width stages** — the shared brief's item 1, met twice.

The same program written with the mark the language has, `w/lease_static.hero`,
is **39 lines against 56**: +17 lines, **+44%**, two extra `extern` groups
(`stdlib.h`, `string.h`), three extra declarations, two `to_u64` conversions,
and the program's **one** `tag void`.

### 2. R1 closes ZERO of defect 066, measured in the same shape as the lease

`w/r1_early_free.hero` is byte-for-byte the program above with `free(p: buf)`
moved two lines up, before `sqlite3_step` — the gallery reordering, in handles.

    ./heroes check   exit 0
    ./heroes build   exit 0
    ten runs         0 0 0 0 134 134 0 134 0 0
                     seven print "stored: " (empty) where the honest answer is
                     "the honest answer"; three panic on .must()
    --sanitize       heap-use-after-free, READ of size 17 in memcpy under
                     sqlite3VdbeExec <- sqlite3_step, r1_early_free.hero:48

For comparison in this session, the reference reproducers:
`docs/panel/168-briefs/gallery-example-reordered.hero` is `check` 0, `build` 0,
`run` 0, "C still reads 0 bytes"; and 068 (`w/d068.hero`, `hold_it(p: s.b.ptr(),
n: 4)` then `s @ Slot(...)`) is `check` 0, `build` 0, `run` **0 0 0**, printing
`C reads 7, honest 7` then `C reads 99, honest 7`, **with `--sanitize` printing
the identical two lines and exiting 0**.

**Why R1 cannot score here, and it is one sentence.** `acquires`/`consumes` is a
**counter**, not an ordering. It fires when the net is positive at `main`'s
return. The retention window a library opens — bind to finalize — is not a
Heroes event and the counter has no opinion about it. Panel 167's finding stands
for handles exactly as for `cstr`.

The counter is weaker than that, even. `w/double_consume.hero`, one `malloc` and
two `free`s: `check` **0**, `build` **0**, `run` **133 133 133**, ASan
*attempting double-free*. `consumes` refuses a value the caller **borrowed**; it
does not refuse a second consume of a local.

### 3. `one_tag_one_type` caps R1 at one `void *` family per program

`w/two_void.hero` — `malloc`/`free` beside `dlopen`/`dlclose`, both C `void *`:

    error[duplicate_tag]: `Lib` and `Block` are both handles over `void`
    check exit 1

The escape hatch is closed too, and correctly: `w/two_void_lie.hero` retags the
second family `Dl_info`, and that is `check` 0 and **`build` exit 1**,
`error[ffi_return_type]: dlopen does not return Lib`. So under R1 a program that
binds two opaque `void *` families is **unbindable**, not merely awkward.

### 4. What R1 forces instead is the swap the handle form exists to refuse

`w/one_block_swap.hero` — the only legal shape once the cap bites: **one**
`Block tag void` carrying both `malloc`/`free` and `sqlite3_malloc`/`sqlite3_free`,
each buffer released by the **other** library's deallocator.

    check exit 0, build exit 0, run 0 0 0, live set balanced and silent
    "two allocators, one type, both swaps accepted"

That is defect 029's class — one Heroes type over two C types — arriving one
level below the place `record Db tag sqlite3` / `record Stmt tag sqlite3_stmt`
closed it. It does not crash on this Mac only because Apple's SQLite is on the
system malloc; my predecessor's `allocator.c` measured `134 134 133 133 133`
with `SQLITE_CONFIG_MALLOC` replaced.

### 5. `const void *` is the one parameter position §4.19's clang check does not reach

`w/wrong_handle_param.hero` declares `sqlite3_bind_blob`'s third parameter —
`const void *` — as **`Db`**, and binds the open database connection as a blob's
bytes:

    check exit 0, build exit 0, run exit 0, "bound a database as a blob"

C converts every object pointer to `const void *`, so the header refutes
nothing there and neither does the `_Static_assert`. **This is a finding about
the ground, not about a route**: §4.19's *"a wrong FFI signature is a compile
error"* is **false at `void *` parameters**, today, and R1 is the one proposal
that moves more traffic into that position.

### 6. R1 has no way home

Bytes that live in a handle cannot re-enter Heroes.

    w/block_home.hero   b.validated()  -> error[type_mismatch]: expected `cstr`,
                                          found `Block`          check exit 1
    w/block_home2.hero  declaring the same C function's `void *` result `cstr`
                        -> error[ffi_return_type]                 build exit 1

I searched `selfhost/**/*.hero` for `load_u8`, `store_u8` and `peek` as a byte
reader: no such builtin exists. So the return leg of every R1 buffer is a
**shim**, which §4.19 budgets *"for the hard cases only, not for every macro"*.

### 7. Defect 069 bites the handle route too, and it is the whole blocker for R5

`w/cb069.hero`, with a **handle** binding rather than `ptr`:

    d: (function(Block) -> ())   check 0, build 0, run 134 ten of ten
                                 emitted C line 130 hero_unreachable();
                                 line 134 (void)cb_take(t3, t4, t5);
                                 t4 declared line 114, never assigned
    d: ptr                       error[type_mismatch]: expected `ptr`,
                                 found `(function(Block) -> ())`   check exit 1

**There is no spelling by which a Heroes program hands a C library a
destructor.** Both were run.

### 8. R5 — give the ownership away. The route nobody listed, compiled

`w/giveaway.c`: a plain `malloc`'d buffer, and plain **`free` handed to
`sqlite3_bind_text` as argument five**. The library owns the bytes from that
line; the caller has no lifetime to reason about, no lease, no cell, no rule.

    clang -O0 -g -fsanitize=address,undefined -Wall -Wextra -Werror   exit 0
    run  0 0 0, "stored: the honest answer", SQLite frees at sqlite3_finalize

And it survives the exact condition that killed panel 167's route A.
`w/giveaway_custom.c` replaces SQLite's allocator —
`sqlite3_config(SQLITE_CONFIG_MALLOC, &m)` returns **rc 0** — and the program
still reads back `the honest answer`, with `sqlite private blocks still live: 0`,
**exit 0 0 0 under ASan+UBSan**. It survives because the destructor is the
**author's**, paired with the **author's** allocator: the library's heap never
touches the buffer. Panel 168 killed the give-away of a *Heroes-runtime* block;
this gives away a block the binding allocated from the library's own vocabulary
or from `stdlib.h`, and the mismatch cannot arise.

### 9. What the ladder's own headers say, counted this session

Ruler, named because panel 167 read 28 and panel 168 read 37: `clang -E -P`,
whitespace collapsed, split on `;`.

| header | declarations | with `(` | mention `const void *` | result position | parameter position | callback-typedef only |
|---|---|---|---|---|---|---|
| `sqlite3.h` (SDK) | 546 | 420 | **33** | **12** | **17** | **4** |
| `raylib.h` (brew 6.0) | 827 | 606 | **7** | 0 | **7** | 0 |

Of SQLite's 17 parameter positions, the ones that carry a **destructor
argument** — `bind_blob`, `bind_blob64`, `bind_text16`, `result_blob`,
`result_blob64`, `result_text16`, `result_text16le`, `result_text16be` — are
**8**, and every one takes R5. The other nine copy (`complete16`, `open16`,
`prepare16`, `prepare16_v2`, `create_function16`, `result_error16`,
`create_collation16`, `blob_write`, plus one availability-attributed line).

`curl.h`, SDK, LIBCURL 8.7.1: `CURLOPT_POSTFIELDS` (15) sits beside
**`CURLOPT_COPYPOSTFIELDS` (165)** — the library's own copying option, grepped.

`raylib.h`: the seven are `SetShaderValue`, `SetShaderValueV`, `UpdateTexture`,
`UpdateTextureRec`, `UpdateMeshBuffer`, `UpdateSound`, `UpdateAudioStream`, and
the header's own comments call every one an *update* of a GPU or audio buffer;
none takes a destructor. **I did not run raylib in this session** — that is the
header's text and the absence of a destructor parameter, and it stays a question
until somebody opens a window.

### 10. What R2 costs this repository: nothing, and the count is the measurement

    .ptr() field lends outside selfhost/:  33 sites in 8 files, ALL in tests/golden/
    .ptr() in examples/:                   0
    pure re-assignments of a lent root in those 8 files:  0

`examples/ledger/db/sqlite.hero`, the one with a real library behind it: **0**
`.ptr()`, **0** `.lease()`, **4** `.cstr()` (call-duration lends, which R2 does
not touch). `./heroes check examples/ledger/main.hero` exit **0**.

### 11. Why the R4 veto is stronger now than at panel 167

`tests/golden/check/ffi-a-lent-field-needs-a-place.hero:51` —
`slot_sum(p: xs.ptr(), n: 4)` on a `[u8]` **local** — is `#~ bad_operand`. There
is no local-array lend. So the **field lend is the only route by which a C
function reads or writes Heroes bytes without a copy**, and `counted_by` is the
only thing that bounds it. Withdraw it and `slot_fill(p: s.nsap.ptr(), n: 8)`
must go through a handle — and finding 6 says a handle's bytes cannot come back.
The cost of R4 is therefore not 33 declarations; it is **every in-and-out
buffer, with a shim per program**, which is §1.11 failing at the one thing it
exists to do (design.md:580, quoted).

---

## argument

R1 is real — I wrote the binding and it runs — but it scores **zero on 066**,
identically to the lease: seven silent wrong answers in ten. `acquires` counts,
it does not order, and a library's retention window is not a Heroes event. What
R1 adds is three costs I compiled: `one_tag_one_type` makes two `void *`
families unbindable; the legal escape merges them into one type that accepts
every allocator swap, silently; and a handle's bytes have no way back into
Heroes, so every buffer needs a shim. It also pushes traffic into `const void *`,
where I measured that a database bound as a blob is `check` 0 / `build` 0 /
`run` 0 — §4.19's promise is already false there. R4 is worse: the field lend is
C's only write path into Heroes memory. **R5 is the answer the libraries
themselves ship, and 069 is all that stands in front of it.**

## prediction

Falsifiable with instruments that exist today:

1. **Once 069 emits a C function's address instead of `hero_unreachable()`,
   §4.19 ladder step 3 needs no shim for a retaining bind**: the ledger's
   `bind_text` rewritten as a `malloc`'d `Block` handed to `sqlite3_bind_text`
   with `destructor: free` will be `check` 0, `build` 0, `run` 0, and the
   reordering that scores 066 will be **unwritable**, because there is no
   `free(p: buf)` line left to move. Falsified if that program needs one line of
   C this project wrote.
2. **R5 needs one compiler rule beyond 069, and it is nameable**: the destructor
   parameter must mark the buffer `consumes`, and `consumes` must refuse a
   second consume of a **local** — which today is `check` 0, `build` 0, `run`
   **133 133 133**. Falsified if a give-away binding is safe without it.
3. **R1 adopted as the answer to 066 will produce, within one milestone, a
   binding that merges two `void *` families into one `Block`** — because
   `duplicate_tag` leaves no other legal shape — and the swap between them will
   be `check` 0 / `build` 0. Falsified if `one_tag_one_type` is lifted first.
4. **R2 will refuse 0 programs in `examples/` and 0 in `tests/golden/`** when it
   lands, on the counts in finding 10. Falsified by one red golden.

## condition

The veto on **R4** lifts the day a Heroes program can read a byte out of a
C-owned buffer without a shim — a `Block`-to-`cstr` route, or a bounded
`load_u8` — measured on `tests/golden/run/ffi-a-byte-field-crosses-to-c.hero`
rewritten without `.ptr()`.

The objection to **R1** lifts if `one_tag_one_type` is lifted for `tag void`
**and** the `const void *` parameter hole in finding 5 gets a refusal, so that
moving retention onto handles does not trade a loud lease for a quiet handle.
R1 stays welcome as a *spelling* — it is what `unread_mark`'s note already
recommends — and is refused only as the *answer to 066*.

**Approve R2** unconditionally for 068: it is caller-side, it costs this
repository nothing measured, and it is the only route on the ballot whose
corrupting line is Heroes' own.

**069 is a precondition, not a side issue.** It blocks R5 entirely, it blocks
§4.19's third case of the reserved ownership vocabulary (*"a buffer that C takes
ownership of"* — design.md:2334), and it is the one FFI defect whose repair
makes two others unwritable rather than merely refused.
