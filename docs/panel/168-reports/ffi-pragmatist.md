# Panel 168 — ffi-pragmatist

**Verdict: APPROVE.** The proposal is right — leading header for route A,
§4.19's third reserved case left to the feature that builds it. **No veto: no
ABI breaks.** But the coordinator's ground is weaker than the one the C
supports, and one of the two sentences it rests on is mine from panel 167. **I
withdraw my panel 167 prediction here**, on my own measurement.

**The section I stand on**: design.md §4.19, lines 2332-2338 — *"Three cases to
cover: a pointer you must free (and with which function), a borrowed pointer you
must not touch, and a buffer that C takes ownership of"* — and §1.11, through
`.claude/rules/c-boundary.md`.

Every number below came from a command run in this session, on Darwin arm64,
2026-09-20, in a `cp -r` copy of the tree at
`/private/tmp/claude-501/.../scratchpad/work/tree`, with the compiler built by
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` at **`real 3.77`**.
Nothing in the main tree was modified except this file.

**The four C sources are in the scratchpad, not in the tree**, because my remit
for this sitting was one file:

    .../scratchpad/bind_blob.c    173 lines   the three retention modes, both layouts
    .../scratchpad/allocator.c    123 lines   the route nobody listed
    .../scratchpad/failpath.c      56 lines   the destructor that fires on failure
    .../scratchpad/fieldlease.c   113 lines   route A's own program

A session-specific path goes away with the session, which is the reason panel
147 gave `docs/panel/` its home. **The coordinator should copy all four beside
`layouts.c` in `docs/panel/168-briefs/`** if the sitting wants them to survive.

---

## The C I compiled, and what each sequence did

Real header: `$(xcrun --show-sdk-path)/usr/include/sqlite3.h`, `SQLITE_VERSION
"3.51.0"`. Real library: `otool -L` reads **`/usr/lib/libsqlite3.dylib`
(compatibility version 9.0.0, current version 382.0.0)**. Every build is
`clang -std=gnu11 -Wall -Wextra -Wpedantic -Werror` and **every one was accepted
with zero diagnostics**.

### Experiment 1 — the full sequence, both layouts, three retention modes

`bind_blob.c`. The sequence is the whole of what a Heroes program runs: open,
create, prepare, **lease**, `sqlite3_bind_blob`, step, finalize, read the row
back, **`end_lease`**. `lead_hold`/`lead_release` are transcribed from
`runtime/parts/str.c:419` and `:451`, subtraction at `:461`; the header is
`runtime/heroes_runtime.h:158-160`.

| layout | retention mode | five runs | what happened |
|---|---|---|---|
| leading | `SQLITE_TRANSIENT` | `0 0 0 0 0` | `0A141E28323C4650` back, correct |
| leading | `SQLITE_STATIC` | `0 0 0 0 0` | correct |
| leading | `sqlite3_free` | **`133 133 133 133 133`** | dies **inside `sqlite3_finalize`** |
| trailing | `SQLITE_TRANSIENT` | `0 0 0 0 0` | correct |
| trailing | `SQLITE_STATIC` | `0 0 0 0 0` | correct |
| trailing | `sqlite3_free` | **`134 134 134 134 134`** | correct bytes stored, then the runtime's panic |

**A correction the sitting is owed, and it is to panel 167 as much as to this
brief.** Under the leading header the process does **not** die at `end_lease`.
It dies inside `sqlite3_finalize`, before `end_lease` is reached — `finalized`
never prints, and neither do the stored bytes. The brief's `layouts.c` calls
`c_destructor` from `main`; the real library calls it from
`vdbeMemClearExternAndSetNull`, and a Heroes program's own output is lost with
it on a buffered stdout. **The exit code is also not stable**: `133` five of
five on an unbuffered build, `133 134 133 134 133` on the buffered one. The
brief's *"exit 133, five of five"* reproduces only for one of the two builds.

Under `-fsanitize=address,undefined`:

- **leading + `sqlite3_free`**: `AddressSanitizer: attempting free on address
  which was not malloc()-ed`, `SUMMARY: ... bad-free
  (libsqlite3.dylib:arm64e+0xc6cd0) in vdbeMemClearExternAndSetNull+0xfc`, with
  `allocated by thread T0 here: #1 ... in lead_hold bind_blob.c:34`. exit 134.
- **trailing + `sqlite3_free`**: `AddressSanitizer: heap-use-after-free`,
  `SUMMARY: ... heap-use-after-free bind_blob.c:79 in trail_release_n`, freed by
  `vdbeMemClearExternAndSetNull`. exit 134.

**So the trailing header is worse on both instruments, not merely equal.** The
plain build's message is the runtime's panic saying *"this is a compiler bug,
please report it"* for a fault that is the program's; and ASan's summary line
names **the runtime's release** where the leading header's names **the
library's free** and attributes the allocation to the Heroes side. A wrong
diagnostic that accuses the compiler is worse than no diagnostic, and the
trailing header is the layout that produces it.

### Experiment 2 — THE ROUTE NOBODY LISTED, and it decides the sitting

`allocator.c`. First I gave the trailing header the only shape in which it buys
anything — the give-away with **`end_lease` waived**, which is what a future
third-case spelling would do:

| | five runs |
|---|---|
| leading + `sqlite3_free`, no `end_lease` | `133` — still dies in finalize |
| **trailing + `sqlite3_free`, no `end_lease`** | **`0`, correct bytes, zero ASan reports** |
| `sqlite3_malloc` block + `sqlite3_free`, no `end_lease` | `0`, correct |

So panel 167's mechanism does work — *on this build of this library*. Then I
asked the question panel 167 did not: **is `sqlite3_free` `free`?**

The real header documents the answer at lines 1879-1891: the allocator is
replaceable through `sqlite3_config(SQLITE_CONFIG_MALLOC, ...)`. I registered a
private `sqlite3_mem_methods` whose `xFree` keeps its own 16-byte size word —
what memsys3, memsys5 and every arena allocator in the wild do. The system
libsqlite3 accepted it, **`rc=0`**.

| | five runs |
|---|---|
| **replaced allocator + trailing header** | **`134 134 134 133 133`** |
| replaced allocator + `sqlite3_malloc` block | `0 0 0 0 0` |

The failing runs die inside `sqlite3_finalize` printing my allocator's own
words: `my_free: size word reads 45550143952 — this is not my block`.

**What this settles, and it is the strongest thing in this report.** The
property the give-away case needs is **not the allocation BASE. It is the
allocation HEAP.** No header layout inside the Heroes runtime can supply a block
from the library's own allocator, so **no header layout can make the give-away
case work.** The trailing header's success above is a fact about this build of
libsqlite3, where `sqlite3_free` happens to wrap `free`, and not about the C
ABI.

**I therefore withdraw my own panel 167 prediction** — *"under the
trailing-header allocation, `sqlite3_bind_blob` needs no shim for all three
retention modes"*. It is false, and false for a reason no allocation layout can
repair. Panel 167 recorded two candidate mechanisms (a pointer-keyed side table,
the emitter passing the length); this report adds that **both answer a question
that is not the one that fails**, which is what the shared brief already
suspected, and names the question that does fail.

**And it tells the future feature what it will be.** §4.19:2334 says all three
reserved cases are about *a pointer **C** made*. The give-away case is no
exception: its buffer comes from `sqlite3_malloc`, from the library's own
allocator, reached as a `ptr` — which is exactly what the section already
prescribes. It is **not a lease at any offset**. The proposal's *"its own
allocation question"* is right to defer, and this report answers it in advance:
the answer will not be a header layout.

### Experiment 3 — the destructor fires when the bind FAILS

`failpath.c`. The real header, lines 4891-4894, in its own words:

> ^ (1) A destructor to dispose of the BLOB or string after SQLite has finished
> with it may be passed. **^It is called to dispose of the BLOB or string even
> if the call to the bind API fails**, except the destructor is not called if
> the third parameter is a NULL pointer or the fourth parameter is negative.

Run: bind at parameter index 2 on a one-parameter statement. `bind rc=25`
(`SQLITE_RANGE`), and the buffer is **already freed**. Reading the first byte
back gives `B1` where `0A` is honest; plain exit **133**; under ASan
`heap-use-after-free` with `freed by thread T0 here: #1 ... bindText+0x128
(libsqlite3.dylib)`, exit 134.

A constraint on whatever builds the third case, named here so it is not
rediscovered: **the waiver must cover the failure path too**, and a program that
reads the result code and "recovers" is holding a freed pointer.

### Experiment 4 — the give-away is NOT WRITABLE in Heroes. Five gates, all run

I wrote it in Heroes against the real `sqlite3.h`, not in C. The full sequence
with **`SQLITE_STATIC`** and with **`SQLITE_TRANSIENT`** both run today, exit 0,
`length back: 8` — under the **leading** header that ships. Then the third mode:

| route to a destructor | result |
|---|---|
| `d: ptr`, pass `sqlite3_free` | `error[type_mismatch]: expected `ptr`, found `(function(ptr) -> ())`` — exit 1 |
| `constant sqlite3_free: ptr` | `error[ffi_not_constant]` — exit 1 |
| a lease into `free(p: cstr)` | `error[ffi_writable_parameter]` — build exit 1 |
| a lease into a `ptr` parameter | `error[type_mismatch]: expected `ptr`, found `cstr`` — exit 1; via `.ptr()`, `error[bad_operand]` + `error[field_lend_uncounted]` — exit 1 |
| `d: (function(ptr) -> ())`, pass `sqlite3_free` | **check 0, build 0, RUN 134** — see the defect below |

**The coordinator's claim is true and understated.** It says the give-away case
is unreachable *in a correct program*. It is unreachable in **any** program: the
type system refuses four routes at exit 1, and the fifth is an emitter hole that
aborts rather than compiling. **The trailing header is paying for a case the
language cannot express, with a worse diagnostic in the case it can't reach.**

### Experiment 5 — route A's own program needs no allocation base

`fieldlease.c`, and it answers the brief's question 3 directly. A fixed-array
field out of a real header's struct — `<dirent.h>`, `sizeof(struct dirent)=1048`
with `d_name` a `char[1024]`, both printed by the program — copied out, handed
to libsqlite3 under **`SQLITE_STATIC`** (the library keeps the pointer), stepped,
reset, `sqlite3_clear_bindings`, then `end_lease`.

**Both layouts: `0 0 0 0 0`**, three rows stored, longest name 25, and **zero
`AddressSanitizer` lines under `-fsanitize=address,undefined` on both.**

**Route A's field lease does not need the allocation base.** Nothing in it is
ever handed to a freer: the bytes go out `const`, the library reads them, and
the program frees its own block. The pointer C receives being the base matters
only when C frees it, and that is the case §4.19 reserved and this language
cannot currently write.

---

## Questions 5 and 6 — ABI, and corrections the sitting is owed

**No ABI break, so no veto.** On the compiler I built from the seed in my copy:
`examples/sqlite/main.hero` runs **exit 0** (`rows: 3`, `longest: 6`) and
`examples/ledger/main.hero` runs **exit 0**. Neither uses a lease.
`grep -rn '\.lease()' --include='*.hero' examples/` returns **2** hits, both in
`examples/gallery/13-lease.hero`, and one of them is the comment at `:8` — so
the corpus has exactly **one** lease site, `:23`, and its C side is a local shim
header rather than a real library. The whole-tree counts reproduce exactly:
`.lease()` **28**, `.cstr()` **126**, `.ptr()` **65** over `examples/ tests/
selfhost/`.

**The one real-library lease in this project has never been run.** That is why
panel 167's prediction went unchallenged for a day: it was scored on a
hand-written C model, and the Heroes program that would falsify it did not
exist. It does now, in the scratchpad.

Three corrections:

1. **The shared brief's measurement 3 is wrong as written.** It says `free(p:
   cstr)` in a group *"is refused before any of this is reachable ... exit 1"*.
   Re-run: the **declaration alone** is `heroes check` **exit 0** and `heroes
   build` **exit 0**. `ffi_writable_parameter` fires only when the function is
   **called** with a `cstr`, at build, exit 1. That matches
   `.claude/rules/c-boundary.md`'s fifth clang-failure class — *"the one member
   that points at a call rather than at a declaration"* — and not the brief's
   wording. The conclusion the brief draws from it still holds; its premise
   needed correcting.

2. **"The give-away case is reached only through a `const`-qualified parameter"
   is true of the compiler and false of the header.** Three non-`const`
   ownership-taking parameters in the SDK's `sqlite3.h`:
   `sqlite3_bind_pointer(sqlite3_stmt*, int, void*, const char*, void(*)(void*))`
   at `:4965`, `sqlite3_set_clientdata` at `:6337`, `sqlite3_result_pointer` at
   `:6525`. The future feature cannot assume `const`.

3. **My `const void *` count differs from panel 167's.**
   `grep -cE "const void[[:space:]]*\*"` on the SDK's `sqlite3.h` reads **37**
   where panel 167 recorded 28, and `/opt/homebrew/include/raylib.h` reads
   **7**, which matches. Different rulers; I name mine and do not claim the
   other wrong.

I also re-ran the coordinator's **measurement 1** because the cost side rests on
it, and it reproduces: with a `while` between them, `--dump-ir` puts
`$t4: cstr = call builtin lease($t3)` in **`bb0`** and
`call builtin end_lease(@label)` in **`bb3`**, and the release instruction
carries the slot and nothing else.

---

## Two defects to file

**Defect A — the emitter gate, and it inverts §4.19's thesis.**
`selfhost/emit/inst.hero:311`: a `.func_ref` whose callee is `.extern_fn` emits
`hero_unreachable(); /* the gate refuses this form */`, and the emitted C then
goes on to make the call anyway with an **uninitialised** `t34` as the
destructor argument. Measured: `heroes check` **exit 0**, `heroes build` **exit
0**, and the built binary **exit 134** with `panic: entered unreachable code —
this is a compiler bug, please report it`. Taking the address of a C function is
a legal thing to want — it is how every callback in every C library is passed —
and design.md §4.19's central promise is that a wrong FFI form is a **compile**
error. This one is a run-time abort. It should be a diagnostic on the `.hero`
line naming the form. The abort does keep §1.12 (nothing corrupt reaches C,
because `hero_unreachable` fires first), so this is a diagnostics defect and not
a robustness one. I grepped `docs/work/DEFECTS.md` (`OPEN: 2`) and `func_ref`
across `docs/`: **no entry names it** — a negative claim, and those are the two
things I searched.

**Defect B — `ffi_writable_parameter`'s note is false for a lease, and its fix
is the wrong direction.** Handing a lease to `free(p: cstr)` is correctly
refused, but the note reads *"§4.20: `s.cstr()` lends the string's own bytes,
and they may be shared with other values"*. That is true of a **lend** and false
of a **lease**: the lease is a copy the program owns alone, which
`runtime/parts/str.c:413` states in as many words. And the
`fix (guess): declare it ptr` points at the one spelling that would open the
hole, if `ptr` ever accepted a lease. File against the diagnostic, not the
layout.

---

## Prediction, with the instrument that exists today

**Registered, falsifiable, on a named binding.**

1. **Step 3 of §4.19's ladder — SQLite — needs no shim under the LEADING header
   for the two retention modes Heroes can express.** Concretely: a program in
   `tests/golden/run/` that leases and binds against the real `sqlite3.h` with
   `SQLITE_STATIC` and with `SQLITE_TRANSIENT` is **exit 0 with zero
   `--sanitize` reports**, on a compiler built from the seed. *Falsifier: one
   exit code that is not 0, or one AddressSanitizer line.* I ran both today, in
   Heroes, and both are exit 0 with `length back: 8`.

2. **The third mode stays unwritable until a feature waives `end_lease`.**
   `heroes check` on the `d: ptr` + `sqlite3_free` spelling stays
   `error[type_mismatch]` exit 1. *Falsifier: any spelling that compiles and
   hands `sqlite3_free` to `sqlite3_bind_blob`.* Note that defect A is **not** a
   falsifier — it builds but aborts, and never reaches the library.

3. **When §4.19's third reserved case is built, its buffer will come from the
   LIBRARY's allocator, not from `hero_alloc_held` at any offset.** *Falsifier:
   a give-away binding over a `hero_alloc_held` block that survives
   `sqlite3_config(SQLITE_CONFIG_MALLOC, ...)` with a non-`malloc` allocator.* I
   ran that today: `134 134 134 133 133`.

---

## What I could not settle, and why

- **Whether the replaced-allocator result reproduces on Linux or Windows.**
  Unrun. I have one Mac, and `.claude/rules/platforms.md` says a platform fact
  run on one platform is an inference about the others. The C ABI argument is
  platform-independent; the exit codes are not.
- **Whether `sqlite3_config(SQLITE_CONFIG_MALLOC)` is reachable from Heroes.**
  It takes a struct of function pointers by address, and defect A says the
  emitter refuses `&extern_fn`, so probably not — but I did not exhaust the
  spellings and this goes out as a question. It does not weaken experiment 2:
  the fact demonstrated is about **libraries**, not about Heroes, and holds
  wherever the binding is written.
- **Whether any other shipping library's give-away destructor is guaranteed to
  be `free`.** I checked `sqlite3.h` only. A general negative would need
  `curl.h`, `raylib.h` and more; what I searched was `sqlite3.h` and I did not
  run the others.
- **Whether the trailing header's alignment padding differs off Darwin arm64.**
  Still unrun, as panel 167 recorded. It no longer matters for this decision.

---

## The resolution I vote for

1. **Build route A on the leading header that ships.** No allocation change, no
   `hero_held_release_n`, no side table. Measured: route A's own program is
   identical on both layouts, exit 0 five of five, ASan clean.
2. **Strike clause 1's allocation half from panel 167.** Its stated ground — *a
   trailing header closes §4.19's third reserved case* — is falsified by
   experiment 2, by the seat that supplied it.
3. **Leave §4.19's third case to the feature that builds it**, and record what
   this sitting learned about it in advance: the buffer comes from the
   **library's allocator**, the release is waived **including on the failure
   path**, and it is a `ptr` C made rather than a lease — which is what
   §4.19:2334 already says of all three reserved cases.
4. **File defect A** (the emitter gate: check 0, build 0, run 134, blaming the
   compiler) and **defect B** (`ffi_writable_parameter`'s note and its guess fix,
   false for a lease).
5. **Clause 2 stays.** I re-ran its premise: `record Box { p: ptr }` with
   `function give() -> ptr` is `heroes check` **exit 0** and `heroes run` exit 0
   today. Neither type rule exists for `ptr`, and nothing in this report touches
   the argument for widening them.

**No veto.** Nothing here breaks the C ABI, and the proposal makes bindings
easier rather than harder: it leaves the runtime alone.
