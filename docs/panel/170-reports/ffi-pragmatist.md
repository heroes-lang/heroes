# Panel 170 — ffi-pragmatist

**verdict**: object (not veto — nothing proposed breaks the C ABI, and I ran that)

**section**: design.md §4.19, the paragraph that already carries this
measurement — *"no declaration-site mark can express retention at all:
`sqlite3.h:4888` puts the decision in the **fifth argument** of one
declaration, `curl_easy_setopt` in its second, and 0 of 71 `cstr` parameters in
this tree are decidable from a header."* Panel 124 measured it; I re-measured
it today and it holds, wider than it was written. Also §1.11 (the FFI is the
whole library story) and spec § 13's `unread_mark` doctrine.

**experiment**: six C programs linked against the real `libsqlite3` and
seventeen Heroes programs, all compiled, all run, all in
§ The evidence below. The headline: `keeps sqlite3_finalize` on
`sqlite3_bind_blob`, written out in C against the real `sqlite3.h` and linked
against the real `libsqlite3` 3.51.0, in both possible readings, against all
three of the fifth argument's modes. clang accepted every cell. **One of the
six is a double free (ASan `double-free`, exit 134), two leak a block each
(measured on a counted allocator, `live_blocks=1`), and no reading is right for
more than two of three.** Against the ledger's own loop shape SQLite calls the
destructor **999 times out of 1000 BEFORE the finalize the mark names.**

**argument** (≤120 words): The four marks that ship constrain the PROGRAM and
are enforced against it — `acquires`/`consumes` by a live set that aborts,
`owned` by refusing your own free call. A retention mark constrains the
LIBRARY, and on the declaration the shape appears on it is not a property of
the declaration at all: argument five picks it, per call. Written as
`keeps end_fn` it names the wrong call 999 times in 1000 on the ledger's loop,
and one of its two readings is a double free clang cannot see. A weaker mark —
*a lend is refused here*, naming no ending call — is honest, and the only one I
can compile without lying. R5 needs none.

**prediction**: `examples/sqlite/main.hero` — **step 3 of §4.19's ladder —
needs no shim, no mark and no edit under any retention rule this sitting can
adopt**, because it binds no text or blob: its **three** `.cstr()` call sites
(lines 73, 81, 99 — a fourth `grep` hit at line 13 is a comment) reach
`sqlite3_exec`, `sqlite3_prepare_v2` and `sqlite3_open`, all of which copy.
Falsifier, run: `grep -c "bind_text\|bind_blob" examples/sqlite/main.hero` is
**0** today.
**`examples/ledger` is the one that pays**: the day a mark lands on
`sqlite3_bind_text`'s third parameter, `heroes check examples/ledger/main.hero`
goes 0 → 1 at `db/sqlite.hero:342`, and the repair is not one line — the
`bind_text` wrapper at 341-343 cannot hold the lease (measured: `panic: 2
lease(s) never ended`, exit 134), so 4 call sites in `main.hero` (67, 68, 77,
78) hoist a lease each into two loops.

**condition**: I withdraw the objection for a mark that (a) names **no ending
call**, (b) is spelled so that it is true as an **upper bound** — *may retain* —
and (c) is refused on any declaration the author can also reach with a lend
that the compiler can prove ends inside the call. I will vote for that mark. I
veto any mark that schedules a release from a named C call, on the double-free
cell below.

---

## The evidence

Everything here was run 2026-09-20, Darwin 25.6.0 arm64, in a copy of the tree
at `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/`.
Compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, **4.69 s**,
`heroes 0.2.0`. Never built from `selfhost/`. Headers: `sqlite3.h` **3.51.0**
and `curl/curl.h` **8.7.1** from the Xcode SDK, `raylib.h` **6.0** from
`/opt/homebrew`.

### 1. Can a mark be honest about `sqlite3_bind_text`? No, and here is the C

`sqlite3.h:4888-4894`, read today, in the header's own words:

> *^The fifth argument to the BLOB and string binding interfaces controls or
> indicates the lifetime of the object referenced by the third parameter.*

Three options: a destructor, `SQLITE_STATIC`, `SQLITE_TRANSIENT`. Panel 169's
llm-ergonomist proposed `keeps end_fn` — *it refuses a lend, takes a lease, and
that lease is ended by `end_fn` and not by `end_lease`.* On this declaration
`end_fn` can only be `sqlite3_finalize`. There are exactly two readings of
"ended by", and `scratchpad/keeps_truthtable.c` (`-fsanitize=address`, linked
`-lsqlite3`) and `scratchpad/keeps_counted.c` (counted allocator, so a leak is
measured rather than inferred) run both against all three modes:

| | `SQLITE_TRANSIENT` | `SQLITE_STATIC` | a real destructor |
|---|---|---|---|
| **A** — the compiler releases at `end_fn` | exit 0, `live_blocks=0` | exit 0, `live_blocks=0` | **ASan `double-free`, exit 134** (plain build: 133) |
| **B** — `end_fn` releases, the compiler drops the cell | **`live_blocks=1 live_bytes=8`** | **`live_blocks=1 live_bytes=8`** | exit 0, `live_blocks=0` |

clang compiled all six without a diagnostic. **Neither reading is right for
more than two of three, and the three are one declaration.** Note also that
`leaks --atExit` reported *0 leaks for 0 total leaked bytes* on all six — the
stale local still points at the block, which is reachability and not ownership.
That is why the counted allocator exists, and it is a caution for any seat
tempted to settle this with `leaks`.

### 2. And the call the mark names is the wrong call, 999 times in 1000

`examples/ledger/main.hero:67-72` binds text **inside a loop** and finalizes
**once**. `scratchpad/loopbind.c` runs that exact shape:

    rows=1000 mode=2  allocs=1000  frees=1000  frees_before_finalize=999  peak_live=2

SQLite calls the destructor at the **next bind to the same column**, not at the
finalize. So `keeps sqlite3_finalize` is false for 999 of 1000 values even in
the one mode reading A was right for. And reading A's cost on the mode the
ledger actually passes (`SQLITE_TRANSIENT`), from `scratchpad/loopcost.c`:

    rows=1000 strategy=0 (today: lease, end_lease at the bottom)  peak_live_blocks=1
    rows=1000 strategy=1 (keeps A: hold until end_fn)             peak_live_blocks=1000

One thousand live blocks where the language has one. That is not an
optimisation point; it is the difference between a bounded and an unbounded
program, and CLAUDE.md §13 says a cost that stops a needed program from running
is compiler-need.

### 3. And on libcurl there is no parameter to mark at all

`curl/easy.h:42`, read today:

    CURL_EXTERN CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);

**The pointer arrives through `...`.** Counted from `curl.h`'s own option
table: **291 `CURLOPT_` rows**, of which **87 STRINGPOINT + 11 OBJECTPOINT + 10
SLISTPOINT + 8 BLOB = 116 carry a pointer**, and their retentions differ —
`CURLOPT_POSTFIELDS` (line 1149) retains the caller's bytes,
`CURLOPT_COPYPOSTFIELDS` (line 1688) copies them. The corpus works around the
variadic by declaring a monomorphised view (`examples/curl/main.hero:51`,
`value: cstr`). **One mark on that parameter would have to be true of all 116
options at once**, and the escape — one declaration per option — is closed:

    error[declared_twice]: `curl_easy_setopt` is already declared at line 7
      — one file is one module, and a name means one thing in it

(`scratchpad/curl/two.hero`, `heroes check`.)

### 4. The census, and its ruler

**Ruler, stated so it can be re-run**: clang's own AST of each header
(`clang -fsyntax-only -Xclang -ast-dump`), every `FunctionDecl` whose location
resolves to the target header, classified by the type string clang prints.
Script: `scratchpad/census/census.py`. Nothing in it claims to detect
retention — **that is the finding, not a gap in the ruler.**

| header | functions | with a non-function pointer parameter | with a `void(*)(T*)` disposer parameter | returning a pointer |
|---|---|---|---|---|
| `sqlite3.h` 3.51.0 | 284 | 251 | **25** | 68 |
| `curl/*.h` 8.7.1 (8 files) | 93 | 79 | **0** | 31 |
| `raylib.h` 6.0 | 600 | 212 | **0** | 52 |
| **total** | **977** | **542** | **25** | **151** |

Of the 542 pointer parameters, **the number a header-reading tool can classify
as retaining is 0.** What it can see is the 25 disposer parameters, and those
split: **12 decide by the argument's VALUE** (`bind_blob`, `bind_blob64`,
`bind_text`, `bind_text16`, `bind_text64`, `result_blob`, `result_blob64`,
`result_text`, `result_text16`, `result_text16be`, `result_text16le`,
`result_text64` — the two families `SQLITE_STATIC`/`SQLITE_TRANSIENT` are
documented for, `sqlite3.h:6354-6355`) and **13 always call the disposer**
(`bind_pointer`, `set_auxdata`, `set_clientdata`, `rollback_hook`,
`create_function_v2`, `create_collation_v2`, `create_module_v2`,
`create_window_function`, `autovacuum_pages`, `result_pointer`,
`rtree_query_callback`, `create_function`, `create_function16`).

**raylib 6.0 and libcurl 8.7.1 declare not one disposer parameter between them
across 693 functions.** Their retention, where it exists, is prose.

### 5. A fifth shape the mark cannot reach — and this project HAS named it

`sqlite3.h:4472-4474`: *^If pzTail is not NULL then \*pzTail is made to point to
the first byte past the end of the first SQL statement in zSql.* It points
**into the caller's buffer**. `examples/ledger/db/sqlite.hero:284-286` declares
`@tail: cstr` and passes `sql.cstr()`, so after the call `tail` holds a pointer
into a lend that `spec § 13` says is over. **No mark on a parameter can say
this**, because the retaining thing is the out-cell and what it retains is
another parameter.

**I first wrote that nobody had named it. That was wrong and I ran the search
that showed it.** `grep -rln pzTail docs/ spec/` returns four files, and the
one that matters is `docs/records/done/2026-09-04-0557-the-rule-that-does-not-work-and-it-is-the-one-that.md`,
which states the case exactly: *"C's type cannot express it — `char **errmsg`
(the caller frees it) and `const char **pzTail` (it points into the caller's own
string) are the same type"*, and it records the ruling — **the rule that works
asks the VALUE, not the world**: an `@` local never read after the call is
refused whatever C owns. That entry names the ledger's two `@tail` sites as
*harmless, same shape*. Panel 108 is the sitting.

**So this shape is precedent, and it cuts against the mark**: the last time this
project met a retention fact a header could not state, it refused to encode the
world and encoded a property of the program instead. That is exactly what I am
asking for in § 6 of the verdict: a refusal, not a guarantee.

### 6. Does the mark make a binding easier or harder than R5?

**R5 needs no mark, and it holds against the measurement that killed the
trailing header.** `scratchpad/r5_sqlite.c`, against real `sqlite3.h` and real
`libsqlite3`, ASan, ten runs each:

    replace=0  exit 0 0 0 0 0 0 0 0 0 0   asan_lines=0   stored 0A141E28323C4650 (honest)
    replace=1  exit 0 0 0 0 0 0 0 0 0 0   asan_lines=0   stored 0A141E28323C4650 (honest)

`replace=1` installs a real replacement allocator through
`sqlite3_config(SQLITE_CONFIG_MALLOC)` with 64-byte-offset blocks — the panel
168 measurement that produced `134 134 134 133 133`. R5 does not notice,
because the author's `free` is paired with the author's `malloc`.

**Measurement 037 replicates on the seed compiler**: `heroes check` 0,
`heroes run` 0, prints `72`, ten runs 0. I could not make the give-away route
fail.

**Harder, measured, is what a mark does to the wrapper idiom.** The ledger hides
the FFI behind `bind_text(statement:at:text:)` at `db/sqlite.hero:341-343`. If
the parameter retains, the lease cannot end inside the wrapper, and an unended
one is not a warning:

    panic: 2 lease(s) never ended — every `.lease()` owes one `end_lease`,
    and this program is missing that many
    run exit=134

(`scratchpad/marks/wrap2.hero`, `heroes build` then run.) So the mark's true
cost on this corpus is not one line at `db/sqlite.hero:342`; it is **four lease
hoists into two loops in `main.hero`**, and the wrapper stops being a wrapper.

### 7. The symmetry the shared brief asked me to judge rather than assume

The counter-argument on the table is that `owned`, `consumes` and `acquires`
are the author's words too and the compiler enforces rather than audits them.
**The symmetry is false, and the difference is measurable.** Each of the three
existing marks is a claim about what the PROGRAM owes, and each has an
instrument:

- `acquires`/`consumes` — a pointer-keyed live set that aborts. **Re-run today**
  on `malloc`/`free` over `record Block tag void`
  (`scratchpad/marks/handle.hero`): `check` 0, `build` 0, and

      panic: 1 C handle(s) never given back — every call marked `acquires`
      owes one marked `consumes`, and this program is missing that many.
      The first is at 0x102c8de50

  exit **134**.
- And silence itself is already refused where the mark is readable. An unmarked
  producer of a marked handle type is `error[unmarked_handle_producer]`, at
  `check`, with the note *"No C header states which, so the binding author
  states it"*. **That is the spec-warden's objection answered by the compiler
  rather than by me**: the language does not rely on the author remembering — it
  refuses the declaration that says nothing. A retention mark could be given the
  same treatment only if there were a type at which silence is detectable, and
  there is not: every `cstr` parameter in the tree is silent, and
  `grep -rhoE "[a-z_0-9]+: cstr"` over `function` declaration lines in
  `examples/`, `selfhost/` and `tests/` counts **97** of them today.
  design.md §4.19's paragraph says 71; that was panel 124's count of an older
  tree, and the shape has grown by 26 since.
- `owned` — the compiler refuses your own call of the named free.
- A retention mark has **no instrument at all**, and worse: §1's table shows it
  is not a property of the declaration. `consumes` cannot be false in a way the
  program can hide; `keeps sqlite3_finalize` on `bind_text` is false in two of
  three modes of the same line.

And the doctrine is already written in the compiler's own words. `borrows` on a
`cstr` parameter is refused today (`scratchpad/marks/m1.hero`, `heroes check`):

    error[unread_mark]: `borrows` on `t` is a word nothing reads: `cstr` reaches
    no handle, so no rule can say which call hands the value over or takes it
    back, and the word could be false without anything noticing
    …
    note: On a `ptr` none of that can happen, and a wrong word there was
    measured to free memory `malloc` never gave

**A retention mark would be the first mark read on a non-handle**, and the
escape the note suggests does not exist for this case: I gave the bytes a
handle type (`record Bytes tag char`) and a `str` cannot reach it —
`error[type_mismatch]: expected Bytes, found cstr`
(`scratchpad/marks/m2.hero`). The marks are about pointers **C** owns; defect
066 is about bytes **Heroes** owns. That gap is structural, not a spelling.

### 8. Defect 072: the shape is real, and the pair is SQLite's own

**Not artificial.** The forcing pair is `sqlite3_malloc`/`sqlite3_free` against
`malloc`/`free`: both answer `void *`, so both need `tag void`, and

    error[duplicate_tag]: `HeapBlock` and `SqliteBlock` are both handles over
    `void` — one tag, one of each kind

(`scratchpad/d072/two.hero`.) Collapse them as the refusal forces and the
crossing is silent: `check` 0, `build` 0, run **0 ten times**, prints
`survived`, and **`--sanitize` is 0 ASan lines** (`scratchpad/d072/one.hero`).

**The silence is a platform accident, and I proved it.** SQLite's default
allocator *is* `malloc`, so the families coincide. Replace it the way any
embedder may and the same crossing is
`AddressSanitizer: attempting free on address which was not malloc()-ed`,
**exit 134** (`scratchpad/d072/cross.c`).

**A retention mark does not answer 072.** 072 asks *whose block is this*;
retention asks *how long*. Two mechanisms, and I would not fund one with the
other.

**But 072 has a binding-level answer today, and it costs twelve lines of C.**
The invented-tag route alone is correctly closed by header verification —
`error[ffi_unknown_name]: sqlite3.h declares no HeroSqliteBlock`, at **`build`,
exit 1, not at `check`** (the shared brief's trap 1, and I hit it). A twelve-line
shim header that declares the two struct tags and four `static inline`
forwarders gives the two families two Heroes types, and then the crossing is
`error[type_mismatch]` at **`check`, exit 1**, while the matched program is
`check` 0 / `build` 0 / run 0 (`scratchpad/d072/blocks.h`,
`shim.hero`, `shim_ok.hero`). That is the honest price of 072 today: one shim
per allocator family, and §1.11 already accepts a shim as the cost of level-1
interop.

### 9. ABI: nothing here breaks it, and I ran that rather than asserting it

Two programs identical but for `acquires free` / `consumes`, emitted with
`--emit-c` and diffed: **10 diff lines, and the call is byte-identical in both**
— `t2 = malloc(t1);` and `(void)free(t3);`. The marks add
`hero_handle_acquired(t2)` and `hero_handle_consumed(t3)` on the Heroes side and
change nothing that crosses (`scratchpad/abi/`). A retention mark is the same
kind of thing. **No veto.** The one proposal element that WOULD change emitted
code is reading A's release scheduling, and that is Heroes-side lifetime, not
ABI — I object to it on §1 and §2, not on the boundary.

### 10. One thing I saw once and could not reproduce, named as unrun

At 16:07 the seed-built compiler emitted, for `lib_keep(p: b, n: 8, d: free)`:

    hero_unreachable(); /* the gate refuses this form */

and the program died `panic: entered unreachable code — this is a compiler bug`,
exit 134, twice (on `main.hero` and on `t2.hero`). Minutes later the **same
binary** emitted `t6 = free;` for the same input, and I could not reproduce the
gate in **20 cold-cache attempts** (`rm -rf build` before each) or in 20 runs.
Both C files are preserved: `scratchpad/r5/t2.c` (with the gate) and
`scratchpad/r5/t2b.c` (without), and they differ on **one line**. The emitter's
determinism rule is `.claude/rules/generated-c.md` — byte-identical on a second
emission of the same input — and on exactly the form this sitting rests on I
have two files that are not. **I have no recipe, so this is a question and not
a defect**: what I searched was the build cache, the header directory, the group
order and the binary identity, and none of the four explained it. It is worth
one session from somebody who knows the emitter.

---

## Answers to the four parts of the shared brief

1. **What is the mark?** Not `keeps end_fn` — §1 and §2 kill it. The only mark I
   can compile honestly is a one-word **`retains`** on a `cstr` or `ptr`
   parameter meaning *the bytes may outlive this call*, naming nothing. It is
   true as an upper bound of all 12 argument-decided sqlite functions, all 13
   always-disposing ones, and all 116 curl options.
2. **What does it refuse?** A **lend** — `.cstr()`, `.ptr()` — at that
   parameter, at `check`. It admits a lease (the author ends it), and it admits
   a bare `ptr` from the author's own allocator, which is R5 and needs nothing.
   It must NOT schedule any release.
3. **What does an unmarked retaining parameter cost?** The spec-warden is right
   that the mark closes the case where the author already knew, and §7 above is
   why I still want it: it is the only one of the marks whose absence is
   *invisible at every stage*, and the language has three words that are
   enforced against the program and none that a binding author can even write
   here. But §7 also says the symmetry is false, so the mark must claim strictly
   less than the three do. **It buys a refusal, not a guarantee.**
4. **Does it answer 072?** No (§8). One mechanism does not serve both, and 072
   has a twelve-line shim answer today that costs the language nothing.

## Where I stand relative to my own reframe

R5 still works and I measured it again. What R5 does not do is stop a program
from writing `.cstr()` into a parameter that keeps the bytes — and on this
corpus that is exactly two sites (`examples/ledger/db/sqlite.hero:342`,
`examples/curl/main.hero:69`), both of which happen to be safe and neither of
which says so in anything a compiler reads. A mark that only refuses the lend
would turn both into compile errors and cost the ledger four lease hoists. That
trade I will vote for. A mark that also says WHEN is a lie I can produce a
double free from.
