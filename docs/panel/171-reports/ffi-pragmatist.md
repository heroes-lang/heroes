# Panel 171 — ffi-pragmatist

**verdict**: approve (no veto: the emitted crossing is byte-identical, and I ran that)

**section**: design.md §1.11 — *"Modern C++ libraries are reachable only
through a C shim you write yourself. This is a real limitation and it is
accepted"* — and §4.19's paragraph that already carries the finding this
sitting rests on: *"no declaration-site mark can express retention at all:
`sqlite3.h:4888` puts the decision in the fifth argument of one declaration,
`curl_easy_setopt` in its second."* The flip does not contradict that
paragraph. The word says *does not keep* only where that is true of the
declaration, and where retention is decided per call the author writes the
shim §1.11 already priced in.

**experiment**: every binding this repository has, rewritten under the flip
and RUN against the real libraries rather than read from their headers. Four
C programs under `-fsanitize=address` hand each bound function a `malloc`'d
argument, free it the moment the call returns, then make the library do the
work that would read it again: `sqlite3.h` 3.51.0 and `libsqlite3`, `libcurl`
8.7.1, libc, and `runtime/runtime.c`. A three-line shim header compiled with
`-Wall -Wextra -Wpedantic`, linked against both libraries, bound from Heroes
with today's compiler and run; a wrong shim refused by clang from the real
header. The ledger's `bind_text` wrapper rewritten with a lease, run against
`main.expected`, and its emitted C diffed against today's. clang accepted
every cell; the numbers are in § The evidence.

**argument** (≤120 words): Under the flip the word claims less than any mark
that ships: a negative upper bound, enforced by refusing a lend, emitting no
C. I classified every binding this tree has from the world: ASan on freed
arguments says SQLite's `open`, `exec` and `prepare_v2` copy, the nine runtime
bindings and `getenv` never store, and `bind_text` keeps or copies **by its
fifth argument** — so no word fits its raw declaration. §1.11 accepts the shim
as level-1's price: three lines of `static inline` fix the deciding argument,
clang refuses a wrong one from the real header, and the word becomes
true by construction. The emitted crossing is byte-identical; only a
Heroes-side temporary changes. The landing count is eleven declarations, not
nine.

**prediction**: On the commit that flips the rule, with no declaration yet
marked, `heroes check` reports **exactly 8** refusals of the new class across
`examples/` — `ledger/db/sqlite.hero:226, :271, :286, :342`,
`sqlite/main.hero:73, :81, :99`, `curl/main.hero:69` — and **every program
that calls `read_file` or `write_file` is refused** at the library's own
`hero_file_read(path.cstr(), @status)` until `selfhost/library_source.hero:152-153`
carry the word, so the landing marks **11 declarations, 15 parameters**, not
9 and 13. After the word lands on the six SQLite declarations and `bind_text`
is rewritten as in § 3 (lease or shim), `heroes run examples/ledger/main.hero`
matches `main.expected` byte for byte and all **24** `hero_ffi_probe_*` lines
of its `--emit-c` are byte-identical to today's. Falsifiers: `heroes check`,
`diff`, `--emit-c`.

**condition**: **Veto** the moment the word reaches the emitted C in any form
— a `noescape` attribute on the probe or the prototype, a different argument
path than `hero_cstr_nonnull(hero_str_cstr(s))` — because header verification
must see the declaration the header sees. **Object** if an unmarked parameter
stops admitting `nullptr`, a header constant, a lease, or a `cstr`/`ptr` that C
itself returned (§ 6's `getenv → puts` program); if the rule reaches `@`
out-parameters (`@db`, `@statement`, `@tail` are the program's own cells); or if
the word is refused on a function declared in a local header, which kills the
shim route. **Approve stands** only if the landing marks the library's two
declarations with the nine.

---

## The evidence

Run 2026-09-20, Darwin 25.6.0 arm64, in a copy named for this seat:
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/ffi-pragmatist-171/tree`,
`git log -1` = `b6e26fcc`, `rm -rf target build` done. Compiler
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, **real 3.51 s**,
`heroes 0.2.0`. Never built from `selfhost/`. Headers: `sqlite3.h` **3.51.0**
and `curl/curl.h` from the Xcode SDK (`curl_version()` prints
`libcurl/8.7.1`), the two curl man pages from the same SDK
(`usr/share/man/man3/curl_easy_setopt.3`, `CURLOPT_URL.3`). The C files are
under `…/ffi-pragmatist-171/c/`, the shim under `…/shim/` and `…/shimcurl/`,
the ledger rewrite under `…/ledger-lease/`, the emitted C under `…/emit/`.

### 1. Does each parameter KEEP what it is handed? Asked of the library, under ASan

`c/sqlite_retention.c`, one cell per process. Each cell hands the parameter a
`malloc`'d buffer, frees it the instant the call returns, then makes SQLite do
the work that would read it again (a `step`, a `sqlite3_sql`, a read of the
out-value). ASan `heap-use-after-free` = **keeps**; silence = **copies**.

| parameter | after the free, SQLite is made to… | exit | ASan lines | verdict |
|---|---|---|---|---|
| `sqlite3_open` `filename` | run a statement, `sqlite3_db_filename` | 0 | 0 | **does not keep** |
| `sqlite3_exec` `sql` | count the rows it inserted (`rows=2`) | 0 | 0 | **does not keep** |
| `sqlite3_prepare_v2` `sql` | `step`, then `sqlite3_sql(st)` (prints the copy) | 0 | 0 | **does not keep** (header 4493: *"contains a copy of the original SQL text"*) |
| `sqlite3_prepare_v2` `*pzTail` | print `tail` | **134** | **2** | the OUT-CELL aliases into `sql` (header 4472) — see § 5 |
| `sqlite3_bind_text` `text`, `SQLITE_STATIC` | `step` | **134** | **2** | **keeps** |
| `sqlite3_bind_text` `text`, `SQLITE_TRANSIENT` | `step`, read back `stored=hello, sqlite` | 0 | 0 | **copies** |

`sqlite3.h:4888-4903`, read today: *"The fifth argument to the BLOB and string
binding interfaces controls or indicates the lifetime of the object referenced
by the third parameter"* — (1) a destructor, (2) `SQLITE_STATIC`: *"must remain
valid until either the prepared statement is finalized or the same SQL
parameter is bound to something else"*, (3) `SQLITE_TRANSIENT`: *"copied prior
to the return from sqlite3_bind_*()"*. **So `text` keeps in two of three modes,
and the pessimistic default is the truthful reading of that declaration.** No
word belongs on it.

`c/curl_retention.c`: `CURLOPT_URL` set from a `malloc`'d buffer, buffer
freed, `curl_easy_perform` on a `nosuch://` scheme (parses the URL, no
network): exit 0, **0 ASan lines**, `perform=1 (Unsupported protocol)`.
`man curl_easy_setopt`, this SDK: *"Strings passed to libcurl as 'char *'
arguments, are copied by the library; the string storage associated to the
pointer argument may be discarded or reused after curl_easy_setopt(3) returns.
The only exception to this rule is really CURLOPT_POSTFIELDS"*, and *"Before
version 7.17.0, strings were not copied."* `man CURLOPT_URL`: *"The application
does not have to keep the string around after setting this option."*

`c/libc_retention.c`: `getenv` from a freed `name`, result read
(`value_len=1692`): **0 ASan lines** — the result points into the environment
and does not alias the argument. `man 3 getenv`: *"obtains the current value of
the environment variable, name"*; nothing about keeping it. `atof` answers a
`double`; there is no pointer to keep. I do not have the C11 text on this
machine, so **the section numbers (7.22.4.6, 7.22.1.1) are where a reader will
find these two, cited and not re-read here**; the man page and the run are the
measurement.

`c/runtime_retention.c` and `c/run_go_retention.c`, linked against the real
`runtime/runtime.c`: all nine of the compiler's bindings with freed arguments,
results read afterwards —
`mkdir=0 write=0 read_status=0 read=payload exists=1 isdir=1 rename=0 scan=1 first=a/b/y.txt`
and `run_go=0 status=0 out=ran`, **0 ASan lines** in both. **My first
`hero_run_go` cell answered `-1` and proved nothing**: `run.c`'s body returns
`-1` at its fourth line when `hero_run_count == 0`, so the fork was never
reached and the four paths never read — I say so because that cell was unrun
until `hero_run_arg` was called first. The bodies agree with the runs:
`fs.c:51-58` and `:38-48` are one `stat`, `:79-93` `memcpy` the path into a
local `work[]`, `:142` `remove`, `:180-188` `rename`; `dir.c:86` stores
`joined`, a name the runtime allocates, never `root`; `os.c` `hero_file_read`
and `hero_file_write` are one `fopen` each; `run.c`'s child opens the three
paths before `exec`.

**Every one of the nine — eleven with the library's two — does not keep, and
takes the word truthfully.**

### 2. The two shapes where the truthful answer is "sometimes", and the C the author writes

`shim/hero_shims.h`, the whole of it that matters:

    static inline int hero_bind_text_copied(sqlite3_stmt *s, int column, const char *text) {
        return sqlite3_bind_text(s, column, text, -1, SQLITE_TRANSIENT);
    }
    static inline CURLcode hero_curl_set_url(CURL *h, const char *url) {
        return curl_easy_setopt(h, CURLOPT_URL, url);
    }

Each shim **fixes the deciding argument**, so the shim's own parameter has one
retention and the word is true of it by construction — not by the author's
promise. Measured:

- header alone, `clang -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only`:
  accepted (two `-Wunused-function` warnings, the expected ones for a header
  compiled by itself);
- `shim/use_shims.c` linked `-lsqlite3 -lcurl` under ASan: exit 0,
  `bind rc=0 stored=shimmed`, `url rc=0`;
- **a wrong shim is still refused from the real header** — `bad_shim.h` drops
  the fifth argument: `error: too few arguments to function call, expected 5,
  have 4`. The project's thesis at the boundary survives one indirection;
- bound from Heroes with today's compiler, `shim/main.hero`: `check` 0, `run`
  0, prints `bind rc=0 stored=shimmed`. Its `--emit-c` carries the same
  verification every extern gets:
  `_Static_assert(HERO_RET_INT(hero_bind_text_copied(…)), …)` and
  `hero_ffi_probe_h_main_hero_bind_text_copied(sqlite3_stmt * a0, int32_t a1, const char * a2)`;
- `shimcurl/main.hero`, `link "curl"`: `check` 0, `run` 0,
  `set=0 perform_unsupported=true`, probe `(CURL * a0, const char * a1)`.

**What it costs the author**: three lines of C in a header beside the `.hero`,
the shape `examples/gallery/13-lease.h` already uses; zero nanoseconds at the
call (the lend stays, so the emitted argument is today's `hero_str_cstr`, a
field read — measurement 038); and one declaration per option for curl, which
is the only per-option route there is, because panel 170 measured the raw
variadic declared twice as `error[declared_twice]`.

The alternative the author may take instead, with no C file: **a lease inside
the wrapper**, § 3. Costs 33.5 ns a call (measurement 038) and two lines. Both
are honest; the shim is the one I would ship in a library binding, because it
makes the per-call decision unwritable at the call site.

What I would refuse in review, and the compiler cannot: **the word on the raw
`sqlite3_bind_text` declaration.** True of the ledger today, which passes
`SQLITE_TRANSIENT` at its one site, and false of the next site that passes
`SQLITE_STATIC` — which would then compile, lend, and dangle (§ 1, row 5).

### 3. The ledger, rewritten, and the ABI proof

`ledger-lease/db/sqlite.hero`, the `bind_text` wrapper at 341-343 becomes:

    copy: cstr @ text.lease()
    rc = sqlite3_bind_text(statement: statement.handle, column: to_i32(at).must(), text: copy, length: -1, destructor: SQLITE_TRANSIENT)
    end_lease(@copy)
    if rc != SQLITE_OK

`heroes check` **0**, `heroes run` **0**, stdout **identical to
`examples/ledger/main.expected`, 29 lines**. So the wrapper stays a wrapper —
the four lease hoists into `main.hero`'s loops that I priced at panel 170 were
the cost of `keeps end_fn`, and the flip does not incur them.

Emitted C, today versus the lease version at the **same relative path**
(`emit/ledger_today.c`, `emit/ledger_lease_samepath.c`):

- **all 24 `hero_ffi_probe_*` lines byte-identical**, `_Static_assert` lines
  untouched (0 in the diff);
- with `#line` stripped, **12 hunks, every one inside `h_dbsqlite_bind_text`**,
  and 0 outside it;
- the crossing itself, today
  `t14 = hero_str_cstr(t13); … t17 = sqlite3_bind_text(t2, t12, hero_cstr_nonnull(t14), t15, t16);`
  and with the lease
  `t2 = hero_str_held(t1); … t18 = sqlite3_bind_text(t4, t14, hero_cstr_nonnull(t15), t16, t17); … hero_held_release(&h3_copy);`
  — same callee, same arity, same C types, `const char *` through
  `hero_cstr_nonnull` either way. What changes is which Heroes-side function
  produced the pointer and a `free` after the call.

And the mark's own slot is already proven inert at the boundary by the
ledger's existing declarations: `@error: cstr owned sqlite3_free` probes as
`char * * a4`, `@out: CDb acquires sqlite3_close` as `sqlite3 * * a1`,
`db: CDb consumes` as `sqlite3 * a0`. Nothing in the `CParam` tail reaches the
probe. A word placed beside `counted_by` will not either — **provided it is
never emitted**, which is the veto line above.

### 4. The census under the flip, from the world

**Ruler**: `grep -rn --include='*.hero' -E '\.cstr\(\)|\.ptr\(\)'`, comments and
string fixtures included, then read.

| tree | raw hits | real lends | into declarations | day-one refusals | takes the word truthfully | rewritten instead |
|---|---|---|---|---|---|---|
| `examples/` | 10 (2 are comments: `gallery/13-lease.hero:3`, `sqlite/main.hero:13`) | **8** | 8 (5 distinct C functions) | **8** | **6**: `sqlite3_open`, `sqlite3_exec`, `sqlite3_prepare_v2`, in both `ledger/db/sqlite.hero` and `sqlite/main.hero` | **2**: `ledger/db/sqlite.hero:342` (shim or lease), `curl/main.hero:69` (shim per option) |
| `selfhost/cli/process.hero`, `selfhost/emit/literal.hero` | 55 across `selfhost/` | **13 at 12 lines** (`:134` lends `from` and `to`) | **9**, 13 `cstr` parameters | 13 | **9 / 13** | 0 |
| `selfhost/library_source.hero` | counted as fixtures by the brief | **2** (`:208`, `:222`) | **2** (`:152-153`, `hero_file_read`, `hero_file_write`) | every program calling `read_file`/`write_file` | **2 / 2** | 0 |
| `tests/golden/` | 47 `cstr` + 33 `ptr` = **80** under this ruler (the brief's 53 + 33 used another; not this seat's part) | — | 90 per the brief | all | — | — |

Extern functions in `examples/` with a `cstr` or `ptr` parameter: **11**
(`grep -E '^\s+function .*: (cstr|ptr)'`), the brief's figure re-measured. Of
the three not in the table: `keep_label(s: cstr)` in `13-lease.hero` already
takes a **lease** and is untouched; `puts(s: cstr)` in `08-ffi.hero` is declared
and **never called** (one grep hit, the declaration); `sqlite3_free(p: ptr)`,
the `callback`/`context`/`destructor` `ptr` parameters receive `nullptr` or a
header constant, never a lend, so they stay unmarked and the rule **must go on
admitting** those two argument kinds.

**The library's two are the finding the brief did not have.**
`selfhost/main.hero:149-153` reads `library = library_source.text()` and hands
it to `input.read_compilation(path:, library:)`, which is
`modules.load_text(… library: library)`: the library text is part of every
compilation the checker sees. That is a reading of the driver, not a run —
the instrument that settles it is the landing commit's own `heroes check` on
any program calling `read_file`. If the two are not marked in the same commit
as the nine, the fixpoint does not merely break: **every Heroes program that
reads a file stops compiling.**

### 5. The out-cell that aliases a lend, and why the flip is still safe there

`sqlite3_prepare_v2`'s `sql` takes the word truthfully — SQLite keeps its own
copy (§ 1). But `*pzTail` is *"made to point to the first byte past the end of
the first SQL statement in zSql"* (`sqlite3.h:4472`), and the ASan `tail` cell
read freed bytes: **the retaining thing is the out-cell, and what it retains is
another parameter.** No word on a parameter can say this; I said so at panel
170 § 5 and panel 108 had ruled it before me.

Under the flip it stays safe for a reason I ran today: `tail/main.hero` tries
to answer `tail` out of the function that made it and is refused at `check` —
`error[cstr_out_of_heroes]: rest answers cstr … only a function inside an
extern group may answer one`. Inside the function `sql` is a live parameter,
so the alias is valid for as long as it can be named. The ledger's two `@tail`
cells are never read (`db/sqlite.hero:286-288`). The rule this sitting adds
should not reach `@` parameters at all: `@tail` carries the address of the
program's own cell, which is not a lend, and every SQLite `@out` binding would
otherwise be refused.

### 6. What the unmarked parameter must keep admitting — run

`onward/main.hero`: `v = getenv(name: "HOME".cstr())` then `puts(s: v)`.
`check` 0, `run` 0, prints the home directory. Under the flip the first call
is refused until `getenv` carries the word (truthfully — § 1); the second hands
`puts` **a `cstr` that C itself returned**, which is no lend and must stay
admitted at an unmarked parameter, or every pass-through of a C pointer into C
breaks. Same for `nullptr` (`callback: nullptr` in both SQLite bindings), a
header constant (`destructor: SQLITE_TRANSIENT`), and a lease (`13-lease.hero`).

### 7. Answers to the shared brief's five questions, from this seat

1. **The word.** The FFI needs exactly this from it: negative polarity (*does
   not keep*), no named ending call, and writable on a `cstr` or `ptr`
   parameter of **any** extern function including a `static inline` from a
   local header — the shim route dies otherwise. `borrows` on a parameter
   would be a trap for a binding author: on a result it means *C keeps this*,
   and on a parameter it would mean *C does not*, the opposite reading of one
   token.
2. **The rule.** Refuse a lend (`.cstr()`, `.ptr()`) at an unmarked `cstr` or
   `ptr` parameter, at `check`. Admit a lease, `nullptr`, a header constant,
   and a `cstr`/`ptr` value that came from C. Do not reach `@` parameters. Do
   not reach handles, which have their own marks.
3. **The diagnostic's `Fix`.** Adding the word: `guess` (a claim about C).
   Passing a lease: `guess` (where to end it is the author's; inside the
   wrapper is right for `SQLITE_TRANSIENT`, § 3, and wrong for `SQLITE_STATIC`).
   The notes should name the third route by name — *fix the deciding argument
   in a `static inline` shim and mark the shim's parameter* — because it is the
   only one that makes the word TRUE for a per-call API.
4. **The landing.** Step one, the word parsed with today's rule, seed
   regenerated. Step two, the rule flipped **and eleven declarations marked**:
   `process.hero:42-53` (nine), `literal.hero:42`, and `library_source.hero:152-153`.
   `.claude/rules/verification.md`'s rule applies in full: a change to what
   the checker refuses is judged by every golden tree.
5. **Corpus cost.** Day one: 8 refusals in `examples/`, 15 in the compiler's
   own text. Repairs: 6 + 11 declarations gain the word truthfully; 2 sites
   are rewritten — one shim or one lease each — and `examples/ledger` runs to
   the same 29 lines afterwards.
