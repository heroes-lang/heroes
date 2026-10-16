# Panel 217, the ffi-pragmatist: does a read-only lend bind the C that only reads?

Written 2026-10-16 from about 22:05, in my own copy `<sp>/217-ffi-pragmatist/` (`git -C
/Users/joseph/Temp/heroes/heroes-lang worktree add --detach … f57af53da`, `rm -rf build`), with two
compilers: `heroes` from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`,
`real 9.08 user 8.58 sys 0.18`) and `heroes-r2` from it (`./heroes build selfhost/main.hero -o
heroes-r2`, `real 87.20 user 65.64 sys 15.88`; `strings heroes-r2 | grep -c ffi_lock_copied` prints
1, the seed's `grep -c` prints 0). Every script and probe is in `<sp>/217-ffi-work/`. `<sp>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad`.
No paid run, no credential printed.

## 1. The census: which `@` parameters point at `const` in their header

**Population.** `git grep -l -E '^[[:space:]]*extern ' f57af53da -- '*.hero' ':!docs'` prints 939
files; the one under `archive/` is left out (nothing builds it, CLAUDE.md § 5). Each of the other
938 was built by `heroes-r2 build <file> -o …` in a fresh directory of its own (`<sp>/217-ffi-work/one.sh`,
`xargs -P 8`, `real 130.83`): **347 build at exit 0, 590 are refused at exit 1, 1 timed out at
120 s** (`selfhost/main.hero`, beside seven other builds). By area, built against refused: `run`
310 / 17, `examples` 20 / 1, `fixedbugs` 6 / 34, `surface-fixtures` 7 / 42, `lldb` 2 / 0, `emit`
1 / 0, `tests/` 1 / 4, `selfhost` 0 / 12, and `check` 0 / 142, `unsupported` 0 / 327, `full`
0 / 5, `permissive` 0 / 4, `ir` 0 / 3. Of the 17 `run` and 1 `examples` refusals, 16 are modules
with no `main` (`no_entry_point`, `unknown_module`) and 2 need `sys/prctl.h`, which this Mac lacks.
The modules were harvested through the root that uses them (12 more roots built, all exit 0), the
compiler's 12 modules through my worktree's own build of `selfhost/main.hero`, and the harness's 5
through a build of `tests/harness/main.hero` (exit 0).

**Instrument.** Each build leaves `build/pointee-<key>/dump-*.json`, the compiler's own
`-ast-dump=json` of `extern __typeof__(*f) hero_ty_f;` per bound function
(`selfhost/cli/pointee.hero`), whose `ParmVarDecl`s carry the header's text for each parameter.
`<sp>/217-ffi-work/params.py` reads every extern parameter's declaration from the `.hero`
(1,883 rows), and `<sp>/217-ffi-work/harvest.py` joins each to the header's type and asks clang,
in a unit with that dump's own `#include`s and `words-*.rsp`, the compiler's own read-only question
(`emit/ffi_read_only.hero`'s `read_only_assertion`): `__builtin_types_compatible_p(P,
__typeof__(*(P)0) const *)`, one `_Static_assert` per pointer parameter.

**Result, this Mac.** 1,104 + 53 parameters harvested, every question compiled (no `?` row).
**188 are declared `@`; 11 of them point at `const` in their header, 177 do not.** The 11:

| binding (file under `tests/golden/run/`) | header parameter | `lent` today | F1 lets the `@` go? |
|---|---|---|---|
| `w_deepest(@w: W lent)` (`fixedbugs-2058-…`) | `const W *` | yes | **yes**, `w: W lent` |
| `tg_kind(@t: Tagged lent)` (`fixedbugs-2062-…` × 3) | `const struct tagged *` | yes | **yes** |
| `tagged_fd(@t: Tagged lent)` (`fixedbugs-2062-…-own`) | `const struct tagged *` | yes | **yes** |
| `regexec(@preg: regex_t)` (`fixedbugs-173-…`) | `const regex_t *restrict` | no | only by adding `lent` (glibc's and Darwin's `regexec` keep no pointer to it: a reading of the libraries' contract, unrun) |
| `getaddrinfo(@hints: Hints)` (`fixedbugs-a-tag-names-…`) | `const struct addrinfo *restrict` | no | only by adding `lent` (same) |
| `lock_marked(@m: lock_t)`, `pattern_marked(@p: pattern_t)` (`ffi-a-typedef-…`) | `const lock_t *`, `const pattern_t *` | no | only by adding `lent` (the test's own header keeps nothing) |
| `peek(@buf: One counted_by n)`, `copy_n(…, @src: One counted_by n, …)` (`fixedbugs-092-…`) | `const void *` | no | **no**: a `const` pointer to an ARRAY, which F1 as listed does not reach; `copy_n` beside `@dst` is also the aliasing question |

The 188 by what they lend (`all.tsv`, classified by `.hero` type and the header's text): numbers 71,
arrays 24, handle cells 23, `cstr` cells 7, `ptr` cells 2, all pointing at nothing `const`; records
lent whole 49, of which **9 point at `const` (5 `lent`, 4 not) and 40 do not** (`counter_init`,
`counter_lock`, `counter_unlock`, `regcomp`, `regfree`, `deflateInit_`, `deflateEnd`,
`SDL_PollEvent`, `SDL_PushEvent`, `SHA256_Init`/`Update`/`Final`'s context, `uv_timer_init`,
`getrlimit`, …); counted records 12, 2 pointing at `const void *`. **So F1 lets 5 of the 188 drop
the `@` as written, 9 with an honest `lent` added, and never the other 179.** The 40 records that
point at a writable struct are the veto line's population: under F1 each must stay `@`.

**Checked twice.** My first harvester's pattern only matched by running into the source line clang
prints under each failed assertion; `harvest2.py` parses clang's `…: Q<k>` line itself, and the
re-run over the same 1,157 rows differs in none (`diff <(sort all.tsv) <(sort all2.tsv)`, empty).

**Reach beyond this corpus** (`<sp>/217-ffi-work/reach2.py`, run by `reach-mac.sh`): every function
a library's own files declare, from clang's `-ast-dump=json` of the header, each parameter type
asked of clang as three `_Static_assert`s: a pointer (`__builtin_classify_type == 5`), to a complete
struct or union (class 12 or 13, and `sizeof(*(T)0)` compiles), to `const` (the read-only question).
"Followed by a count" is a text heuristic on the next parameter's name and type, so those are
arrays by inference.

| header (this Mac) | functions | pointer to a complete `const` record | of them followed by a count | pointer to a writable record |
|---|---|---|---|---|
| `SDL3/SDL.h` | 1,250 | 149 in 104 functions | 19 | 159 |
| `uv.h` | 319 | 57 in 55 | 8 | 274 |
| `pthread.h` | 100 | 26 in 26 | 0 | 65 |
| `raylib.h` | 600 | 14 in 14 | 12 | 63 |
| `sys/socket.h` | 22 | 6 in 5 | 0 | 7 |
| `regex.h` | 10 | 5 in 5 | 0 | 9 |
| `sqlite3.h` | 291 | 2 in 2 | 0 | 10 |
| `netdb.h` | 39 | 2 in 2 | 0 | 2 |
| `curl/curl.h` | 93 | 1 in 1 | 0 | 13 |
| `zlib.h` | 81 | 0 | 0 | 66 |

262 parameters over ten headers point at a `const` record C reads, 39 of them by inference an
array. Each is a binding where today the program keeps a `@=` cell it never writes, only to lend it:
`SDL_RenderFillRect(renderer, const SDL_FRect *rect)`, `uv_tcp_bind(…, const struct sockaddr *,
…)`, `nanosleep(const struct timespec *, …)`. Whether C keeps such a pointer is not in any header
(panel 124 measured 0 of 71 `cstr` parameters decidable), so each still owes its `lent` by reading
the library.

## 2. The experiment: the C that F1 implies, compiled against the real header

**F1's C for task C.** `heroes-r2 build c.hero --emit-c` on the kit's key (`<sp>/217-ffi-work/p/key/`,
which builds and prints `6 4 5`) emits `counter_value`'s probe as `static void
hero_ffi_probe_h_c_counter_value(struct counter * a0)` and `doubled` as `int64_t
h_c_doubled(struct counter *ph0_c)`. `f1.c` is that unit with exactly what F1 changes, five lines
(`diff key.c f1.c`): the probe's and the return assertion's `struct counter *` become `const struct
counter *`, `h_c_doubled` takes `const struct counter *`, and the runtime's
`hero_lend_local_name(void *, …)`, which only compares addresses (`runtime/parts/lend.c:479`), takes
a `(void *)` the emitter would write for that one internal call or a `const void *` the runtime
would declare. Compiled with `selfhost/cli/flags.hero`'s nineteen words at `-O0` (`<sp>/217-ffi-work/c/cc.sh`)
and linked with the runtime object the key's build made: **exit 0, no diagnostic, and the binary
prints `6`, `4`, `5`**. The calls `h_c_doubled(&h0_s.f_first)` need no change: C converts `struct
counter *` to `const struct counter *` silently.

**The veto line, held by clang.** The same `f1.c` against a `counter.h` whose `counter_value` takes
`struct counter *` and adds 100 through it (`<sp>/217-ffi-work/c/nc/`): **6 errors, every one
`passing 'const struct counter *' to parameter of type 'struct counter *' discards qualifiers
[-Werror,-Wincompatible-pointer-types-discards-qualifiers]`**, at `c.hero:7` (the declaration's probe)
and `c.hero:18` (the call in `doubled`). The word that makes it an error is
`-Werror=incompatible-pointer-types-discards-qualifiers`, `selfhost/cli/flags.hero:144`.

**The same veto line, broken by one cast.** `f1_voidcast.c` keeps today's `@` probe (`struct counter
* a0`) and crosses the lend as `(void *)ph0_c`, which is how a numeric `@` lend crosses today
(`cli/pointee.hero`: *the emitter passes `(void *)&x`*): against the writing header **it compiles at
exit 0 and prints `206`, `204`, `5`**: C added 100 to a counter the program declared read-only, a
wrong value at exit 0. `(const void *)ph0_c` is still refused against the writing header (6 errors)
and compiles against the real one, but a `const void *` has dropped the struct's identity, so a
`const struct other *` header would take it in silence. **So F1 is sound only if the lend crosses
typed, `const T *`, through a probe whose parameter carries the `const`, never through `void *`.**

**Three doors clang's qualifier check does not see** (`<sp>/217-ffi-work/c/doors/`), each handed
`const struct counter *` by an F1 probe:

| the header declares | clang under the 19 words | what C did |
|---|---|---|
| `void_bump(void *p)` | **refused**, `discards qualifiers` | nothing: no binary |
| `kr_bump()`, no prototype | 2 warnings `-Wdeprecated-non-prototype`, **no error** | wrote: 3 became 103 |
| `va_bump(int k, ...)`, the lend in `...` | **nothing** | wrote: 103 became 203 |
| `liar(const struct counter *c)` that casts the `const` away | nothing | wrote (the header lies; no check can see it) |

The no-prototype door is already shut for every binding: `heroes-r2` refuses `kr_bump(@c: Counter
lent)` against `int64_t kr_bump();` with `error[ffi_parameter_type]: … declares it with no prototype`
(defect 426's question, asked of every function with a parameter). **The variadic door is open
today, for `cstr`** (§ 3). The lying header is the trust the language already extends to `cstr
lent` against `const char *` (defect 411's question): the header is the contract, §1.11's point 3.

## 3. A defect found on the way, live today: a read-only `cstr` lend at a `...` position is written by C

**Reproducer** (`<sp>/217-ffi-work/p/va/`): a header `va.h` declaring `int64_t va_scribble(int32_t
k, ...)` that writes `'X'` into the `char *` it reads with `va_arg`, bound `function va_scribble(k:
i32, s: cstr lent) -> i64`. With `a = "hel" + "lo"`, `b = a`, `va_scribble(1, a.cstr())`:

| compiler | this Mac | Linux arm64 |
|---|---|---|
| `heroes-r2` | `build` exit 0; prints `1`, `Xello`, `Xello`, exit 0 | same, exit 0 |
| `heroes` (the trunk's seed) | `build` exit 0; prints `1`, `Xello`, `Xello`, exit 0 | not run |
| `heroes-r2`, the literal `"hello".cstr()` | **exit 138, SIGBUS** | **exit 139, segmentation fault** |

Two names sharing one string both changed, and a literal kills the process: design.md §1.12's two
promises, broken at exit 0 and at a signal. **Where**: `selfhost/emit/ffi_read_only.hero:273`,
`lend_answers`, asks the read-only question (defect 411's) *only where the declaration's count of
parameters equals the header's* (the comment above it: *"the probe's call tells the count
(`ffi_call_shape`)"*). A variadic function's count never equals the binding's, and its probe's call
is legal C, so nothing is asked and nothing refuses. `cli/header_types.hero:159` drops every other
ask past the prototype the same way. **The shape beside it**: `va_scrub(k: i32, p: ptr counted_by 4
lent)` over a variadic header is refused, but at **exit 2**, `internal error: … heroes-ffi-extent-unasked
va_scrub p: the pointee check read no unit for this counted parameter`, the author's binding blamed
on the compiler; the same lend at a fixed position is `error[field_lend_written]` at exit 1.
**Searched and not found**: the issues for *variadic lent*, *variadic cstr writable*, *va_arg*,
*ellipsis*, *past the prototype*, *another number of parameters*, *ffi_call_shape*; `docs/panel/`
for *sscanf* and *variadic* beside *lent*, *read-only* or *writable*. Panels 094 and 096 sat on
variadic arguments and not on this; defect 411's record (issue 1750) names the count rule and no
variadic case. So it reads as unfiled; **the coordinator files it** (my brief writes one file), class
`blocking` by `.claude/rules/verification.md` (a wrong value, a crash).

**What it means for F1**: F1 asks the same question of a record, so as the machinery stands, `va_bump(k:
i32, c: Counter lent)` would be accepted and C would write through the read-only lend (the doors
table above: 103 became 203). **The repair is one rule for both**: a read-only lend whose position
is past the header's prototype is refused at exit 1 on the parameter, because C's `...` says
nothing of what C does with it; `@` (C may write) and a lease (a private copy) stay open there.
In the corpus that builds, **3 parameters sit past a prototype** (`all2.tsv`, an empty header type):
`examples/curl/main.hero`'s `curl_easy_setopt(…, value: cstr)`, passed a **lease**, and two `i64`s.
So the rule makes **0 of the corpus's buildable bindings unbuildable**.

## 4. Aliasing at the boundary: one place, a writable and a read-only `restrict` pointer

Today two `@` lends reaching one value, whole or a part, are refused
(`error[aliased_mutable_arguments]`, `mv(@p, @p)` and `mv2(@t, @t.a)`, `<sp>/217-ffi-work/p/alias/`).
Under F1 `mv(@p, p)`, with `src: Pt lent` over `int64_t mv(struct pt *restrict dst, const struct pt
*restrict src)`, hands C one address twice. That C, written as F1 would emit it
(`alias_f1.c`), **prints `2 2 2` at `-O0` and `1 2 1` at `-O2`, no diagnostic**, on this Mac and in
Linux arm64 alike: C's `restrict` makes the answer depend on whether the program went through
`heroes build` (`-O0`) or `heroes run` (`-O2`). So F1 owes the overlap refusal for a read-only lend
beside an `@` one, at a C call as at a Heroes call (the critic's question 2, which the briefs ask
of Heroes calls only).

## 5. Callbacks: a parameter F1 would reach that no form names

A C callback receiving `const struct pt *` (libuv's `uv_read_cb` receives `const uv_buf_t *`) has
**no spelling today** (`<sp>/217-ffi-work/p/cb/`): `(function(@p: Pt) -> i64)` is
`error[expected_type]: expected a type, found @`, and `(function(p: Pt) -> i64)` is
`error[ffi_callback_type]: … it says long long (*)(const struct pt *)`. The C an F1 that reached a
function type's parameter would emit, `int64_t h_one(const struct pt *)`, passes to that callback
type and prints `42`, and is refused against `int64_t (*)(struct pt *)`, *incompatible function
pointer types*, under the pragma the emitted C already carries. Design.md §4.19 records that over
`sqlite3.h`'s 106 callback signatures a spelling for a pointer to a header's struct unlocks 64; a
read-only lend in a function type is the `const` part of that, uncounted here.

And F2's one boundary risk is closed by the compiler's own thunk: a callback receiving a struct by
value binds today (`bv.hero`, prints `42`) through a generated `h_0cb_bv_one(struct pt)`; F2's C,
the inner function taking `const struct pt *` and the thunk handing it `&h0_p` (6 lines of
`bv.c`), compiles clean and prints `42`. In the buildable corpus 36 parameters are callbacks and
none hands its callback a record by value (`all2.tsv`).

## 6. The platforms

**Linux arm64** (`heroes-linux-arm64:latest`, Debian clang 22.1.8, my container `ffi217-prag`; the
compiler from the seed `real 13.9`, R2 from it `real 1m48.589s`): the key prints `6 4 5`; F1's C
compiles clean and prints `6 4 5`; the writing header is 6 `discards qualifiers`; the `void *`
cast prints `206 204 5`; the doors, the variadic `cstr`, the aliased `restrict` call agree with
this Mac (above). **The census** (the same 938 files, `xargs -P 8`, `real 4m40.581s`): 342 build,
596 refused; against this Mac, the two `sys/prctl.h` programs and `selfhost/main.hero` build there,
and 8 programs naming `raylib`, `sdl3` or `libuv` are refused `ffi_package` (not in that image).
**161 `@` parameters harvested on both platforms; their `const` answers differ in none, and the 11
are the same 11** (`diff`, empty).

**glibc's own headers** (`<sp>/217-ffi-work/c/glibc/libc_f1.c`): F1's typed `const` probes for
`regexec`, `getaddrinfo`, `nanosleep`, `pthread_attr_getdetachstate` and `bind` compile with **0
errors**, and the four writers handed a `const` lend (`regcomp`, `accept`, `getsockname`,
`pthread_attr_init`) are **4 errors**, on this Mac and on glibc, with and without `_GNU_SOURCE`.
Under `_GNU_SOURCE` glibc's `accept` takes the transparent union `__SOCKADDR_ARG`, and clang's words
change to *passing 'const struct sockaddr *' to parameter of incompatible type '__SOCKADDR_ARG'*:
still refused, but not the phrase *discards qualifiers* a reader keyed on it would recognise. And
today's compiler asks nothing of a record lent to `bind` there (its check unit for
`bind(fd, @a, 16)` over `@addr: SockaddrIn lent` holds no assertion, and the program builds, runs and prints `0`): a parameter whose
header type is not spelled with `*` is left to the probe's call.

**Windows**: `ssh -o BatchMode=yes -o ConnectTimeout=8 win 'echo ok'` printed *connect to host
apponfly-vps port 22: Operation timed out* at about 22:20 and again at 22:37: **unrun**.

## 7. What clang checks today, and what it must check under F1

**Today, for `@c: Counter lent`**: the probe `hero_ffi_probe_h_c_counter_value(struct counter * a0)
{ (void)(counter_value)(a0); }` holds the record's identity against the header (a `struct other *`
is `-Werror=incompatible-pointer-types`); the pointee check asks one question,
`heroes-ffi-whole-void` (the header's pointee is not `void`, `cli/pointee_wants.hero`'s
`whole_record`, panel 194's R7); and the arity question (defect 426). **Nothing asks whether the
header's pointee is `const`, and nothing needs to**: an `@` lend may be written. A non-`@` `cstr`
is asked the read-only question (`pointee_wants.read_only`, defect 411); a non-`@` `ptr` lent from
a field is judged by clang's qualifier error (panel 166, `emit/ffi_lend.hero`).

**Under F1, for `c: Counter lent` with no `@`**, measured above, the binding is sound only if:

- **(a)** the lend crosses **typed**, `const struct counter *`, through a probe whose parameter
  carries the `const`; never `(void *)`, which accepted the writing header and printed `206`, nor
  `(const void *)`, which drops the record's identity. Clang's qualifier error is then a second
  wall (6 errors against the writing header, both platforms);
- **(b)** the read-only question `__builtin_types_compatible_p(P, __typeof__(*(P)0) const *)` is
  asked of every non-`@` record lend as it is of a `cstr` (`pointee_wants.read_only` gains `.named`
  group records), so that the refusal is the compiler's diagnostic on the parameter at exit 1
  (`ffi_writable_parameter`), never clang's words read back, which glibc's transparent unions change;
- **(c)** a read-only lend past the header's prototype is refused (§ 3), which repairs the `cstr`
  door that is open today and keeps F1 from widening it to records;
- **(d)** a read-only lend and an `@` lend reaching one value in one call are refused (§ 4);
- and the no-prototype door stays shut by the arity question it already has; `lent_shape`'s *only
  those two are ever lent* changes its words (the critic's pass).

**What no check can see**: a header that declares `const` and casts it away (`liar`, wrote at exit
0). That is the trust the language already extends to `cstr lent` against `const char *`, and
§1.11's point 3 (the header is what is verified) is its statement.

## 8. Verdict

- **verdict**: **F1 `object`** as written in the shared brief, turning to **approve** on four
  conditions and to **veto** if (a) or (c) is missing (§ 7); F0, F2, F3/U5, U2, U6 **approve**
  (neutral at the boundary); U1 and U4 **approve** on F1's conditions; U3 **object**.
- **section**: design.md §1.11 (*FFI ergonomics rank alongside comprehension*; its point 3, *a wrong
  type in an `extern` is a compile error*), §4.19 (the probe, the pointee check), §1.12 (a wrong
  value at exit 0 and a segmentation fault are the two things it forbids).
- **experiment**: the C F1 implies for task C, `f1.c`, five lines from today's emission: clang
  accepts it, `6 4 5`, this Mac and Linux arm64; against a writing `counter.h`, 6 errors both
  platforms; through a `(void *)` cast, accepted and `206 204 5`; libc probes 0 errors, writers 4,
  glibc and Darwin; variadic `cstr lent` today `Xello Xello` and SIGSEGV; aliased `restrict` call
  `2 2 2` against `1 2 1`. Census: 188 `@` parameters, 11 point at `const`, F1 frees 5 (9 with
  `lent` added), identical on glibc.
- **argument**: F1's C half is not new: `s: cstr lent` already lends bytes read-only to C, held by
  defect 411's question. Extending it to group records changes five lines of emitted C, compiles
  clean against `counter.h` and glibc, prints `6 4 5` on both platforms, and clang refuses every
  writer. But its machinery has a hole: the read-only question is skipped wherever
  the binding's count differs from the header's, so a `cstr lent` in `...` is written by C today
  (`Xello`, SIGSEGV), and F1 would carry records through it. One `void *` cast in the emitter and C
  writes a read-only counter at exit 0 (`206`). Both are my veto line, a C write through a
  read-only lend accepted.
- **prediction**: the compiler-engineer's F1 prototype, if it reuses `lend_answers` as it stands,
  accepts `va_bump(k: i32, c: Counter lent)` over a variadic header that writes, and the program
  prints `103` at exit 0; with condition (c) it is refused at exit 1. Under F1 with (a) to (d),
  `fixedbugs-2062-a-record-holding-a-union-bound-partial-builds.hero` written `tg_kind(t: Tagged
  lent)` and called from a `=` binding builds and prints its `.expected` unchanged on this Mac and
  Linux arm64, and `counter_init(c: Counter lent)` is refused at exit 1, never 0 and never 2.
- **condition**: the prototype shows (a) a typed `const T *` probe and no `void *` crossing, (b)
  the read-only question asked of record lends, (c) the refusal past the prototype, whose repair
  also closes § 3's defect for `cstr`, and (d) the overlap refusal at C calls: then **approve**.
  Without (a) or (c): **veto**. A form that keeps *C does not keep it* apart from *nothing writes
  it* (U1) on the same conditions is better at the boundary, because `sqlite3_create_module`'s
  `const sqlite3_module *` is one SQLite keeps (a reading of SQLite's documentation, unrun), which
  F1's `lent` cannot bind read-only.

| form | verdict | why, at the boundary |
|---|---|---|
| F0 | approve | every binding of today builds; the 262 reach parameters keep a `@=` cell; nothing unsound |
| F1 | object; approve on (a) to (d); veto without (a) or (c) | § 2 to § 4, § 7 |
| F2 | approve (neutral) | C still needs `@`, so no binding gains; callbacks by value keep binding through the thunk (`42`) |
| F3 as listed | approve (neutral) | already the language (the critic's pass); no C changes |
| U1, a new word | approve on F1's conditions | the same C as F1; can say *kept and read-only*, which F1 cannot |
| U2, a mark at the call | approve (neutral) | no C changes |
| U3, by type | object | the binding's text stops saying whether C receives a struct or a pointer; a header that gains a lock moves it (a reading, unrun) |
| U4, a lend method to C | approve on (a), (c), (d) | the `.cstr()`/`.ptr()` pattern for records; reaches C calls, not task C's Heroes helper |
| U5, an `@` only read | approve (neutral) | today's non-`const` probe, unchanged |
| U6, a message | approve (neutral) | no C changes |

## 9. What I did not run

The compiler-engineer's prototype (not built when I wrote this: every F1 sentence above rests on
C written by hand from today's emission, not on F1's emitter). The Windows box (timed out twice).
Whether `regexec`, `getaddrinfo` and `sqlite3_create_module` keep their pointers (a reading of the
libraries, not a measurement). A count of callback typedefs taking `const struct X *` in the ten
headers. U3 in any form. No paid run.
