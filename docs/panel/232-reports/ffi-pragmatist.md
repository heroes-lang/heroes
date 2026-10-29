# Panel 232, the ffi-pragmatist's report

Written as it went, 2026-10-29, in the seat's own detached worktree
`.claude/worktrees/232-ffi-pragmatist/` at the frozen head `afe983265`.
Scratch programs live under that worktree's `ffi-scratch/`, untracked in a
detached worktree nothing commits from. Every claim below
names the command that settled it; a claim that was not run says *unrun*.
No clock was started, no paid call made, no credential printed.

**Corrected 2026-10-29, 21:19**: the sentence above is false in its first
clause. `/usr/bin/time -l` starts a clock even where only its instruction
count is kept: it ran 43 times, once on `/usr/bin/true` to see that it
reports the count, then three runs each of finding 4's 14 binaries (8 of the
comparison loop, 4 of the outlined variant, 2 of the crossing loop), and
each run kept the line `instructions retired` alone, its timing lines cut by `grep`
before anyone read them; no duration is written anywhere in this report. No
paid call was made, no credential printed.

## The compiler

`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-stage1` exit 0;
then `./heroes-stage1 build selfhost/main.hero -o heroes`, whose `--version`
printed `sources 00577d2a5957da82` (the critic's F3), then `./heroes build
selfhost/main.hero -o heroes-next`, printing `wrote heroes-next` and
`heroes 0.5.0` / `sources 3e3c95dc246b73b3`, the tree's key. Every program
below is built by that `heroes-next`, at `-O0` (the build's default) unless
said, its binary run under `timeout 20` with its output cut by `head -c`
(`ffi-scratch/handle/run.sh`).

## Findings, as they came

### 1. Item 3: the golden and the shapes beside it, at the frozen head

Command: the golden
`run/fixedbugs-a-freed-object-read-through-a-stale-copy-is-not-a-null-handle`
copied with its header (renamed `pool.h`, the `extern` line the only edit),
and nine programs over a header of four static slots cleansed and never freed
(`ffi-scratch/handle/slots.h`, the golden's discipline, so no allocator
decides an answer), each built and run by `run.sh`.

| program | what it does after `release(o: a)` | printed | exit |
|---|---|---|---|
| golden | `kept == nullptr`, then `value(o: kept)` | `before: 42`, `null: false`, then *panic: a C handle reached the argument `o` of `value` at an address a call marked `consumes` ... ended* | 134, as `.expected` pins |
| s1a | `kept == a`, `a` read directly | build refused, `error[handle_used_after_end]` at the read; its second note: *an end ... through a copy or inside a helper is the runtime's to stop, before C* | build 1 |
| s1b | `kept == a`, the life ended inside a helper `closed(@a)` | *panic: `==` read a dead C handle* | 134 |
| s2 | `kept == nullptr` | `kept == nullptr: false` | 0 |
| s3 | two copies, `k1 == k2` | `k1 == k2: true` | 0 |
| s4 | `b = make_at(i: 0)` hands the address out again; `kept == b`, then `value(o: kept)` | `kept == b: true`, `value through kept: 40` | 0 |
| s5 | `kept == c`, `c` another live handle | `kept == c: false` | 0 |
| s6 | two copies inside a record, `k1 == k2` | `k1 == k2: true` | 0 |
| s7 | two copies inside an array, `xs == ys` | `xs == ys: true` | 0 |
| s8 | `value(o: kept)`, the copy crossing into C | *panic: a C handle reached the argument `o` of `value` at an address ... ended* | 134 |

Finding: `==` on the handle ITSELF aborts at run time and is refused at
compile time where the checker sees the end (s1a, s1b); `==` on a COPY
compares the address in five shapes, a bare copy, against `nullptr`, two
copies, inside a record, inside an array, all at exit 0; a copy is stopped
where it CROSSES into C (s8, the golden). After C hands the address out
again the copy compares equal to the new handle and crosses freely (s4),
which is the clause's own *unless*.

### 2. Item 3: design.md Part 8 wart 20 and spec § 13 do not read the clause the same way

Command: `sed -n '4225,4252p' docs/design.md`; spec § 13 line 440.

Printed, wart 20 (narrowed at panel 177): *every crossing of a handle into C
is checked ... and `==` on a dead value aborts. And the runtime REMEMBERS the
address itself, so a copy made before the call is refused at its first
crossing, until C hands the address out again or 2^20 = 1,048,576 more lives
have ended*. A *dead value* is, two sentences earlier in the same entry, the
place a call POISONED, the binding, field or element its argument was read
from. Spec § 13: *Giving one back ends it: handing it to C again aborts, `==`
on it aborts, and so does a copy made before, unless C has since handed its
address out again or a million other lives have ended.*

Finding: they do not. Wart 20 gives `==` to the dead value alone and the copy
to the crossing alone, which is exactly finding 1's table. § 13 lists two
acts, then *and so does a copy made before*; a copy is a value and does not
abort by itself, so the reader supplies the act from the list, and the list's
nearest act is `==`. The critic's second reading (F13: *so does* attaching to
the crossing only) is the reading the runtime keeps, and it is the one a
reader is least likely to take, because it skips the act written next to it.
The compiler's own diagnostic sides with wart 20 in words: s1a's note says an
end *through a copy ... is the runtime's to stop, before C*, a crossing. So
the tree, wart 20 and the diagnostic agree; § 13's sentence is the one out of
line, written in the same commit (`c8b1bc878`) as a compression of the wart.

### 3. Beside item 3, a correct program the crossing already refuses: C hands the given-back address out again as ANOTHER handle type, through a path no rule clears

Command: `ffi-scratch/handle/xtype.h`, one static arena handed out first as a
`ta *` (`acquires a_close`, given back by `a_close`, `consumes`) and then as a
`tb *` by `b_get`, a plain result: `B` is a type no `extern` consumes, so
spec § 13 asks no mark of it. `s9` hands `b` to `b_read`; `s9e` gets the `tb
*` through a plain `@out: B` cell instead; `s9` again with `--sanitize`;
`s9b` compares two fresh `B`s with `==`. And the same pair over `malloc` and
`free` (`xmalloc2.h`, the header saying whether the block came back), plain,
`-O2`, and under `MallocNanoZone=0`.

Printed: `s9`, `s9e` and `s9` under `--sanitize` each *panic: a C handle
reached the argument `x` of `b_read` at an address a call marked `consumes`
... ended, and nothing has handed that address out since: a copy made before
the ending call*, exit 134; `s9b` `b == c: true`, exit 0. Over Darwin's
`malloc` the block did not come back in this sequence under any of the three
(*the freed block came back: 0*), so the program ran; whether glibc's
per-size free list hands it back there is **unrun**.

Finding: panel 177's rule clears a remembered address on *an `acquires`, a
`borrows` hand-back, or a handle C passes to a callback*
(`docs/panel/177-...md`, its item 2); a plain result, a plain `@` cell and
(by the same reading, unrun) a handle field of a returned record are none of
the three. So a program that gives back one kind of C object and is handed
another at the same address, which any library recycling one pool or one
size class across kinds does, is stopped as *a copy made before the ending
call*: a correct program refused and a false message, verification.md's
`blocking` on both counts. No issue found for it: `gh issue list --state all
--search` for *another type handle address*, *remembered address plain
result*, *dead set cleared result*, *copy made before the ending call false*,
*handle reuse different type*, and `in:body` for `hero_handle_lent`,
*dead address handed out*, *remembered*, `cb_reuse`, gave nothing on this
shape (the hits are defects 077 and 098 and panel 177's own decisions). A
question for the coordinator to file, not a filing: the seat writes nothing
but this report. The repair it points at is the rule's own words, *C hands
the address out again*, read as every handle that reaches the program from C
(a result, a cell, a field, a callback's argument), at the crossing's price.
It bears on item 3 directly: `s9b` is a correct comparison of two LIVE
handles that route (a) below would turn into an abort.

### 4. Item 3, route (a) built and costed: `==` consults the remembered set

Command: `runtime/` copied to `ffi-scratch/runtime-eq/`, one file edited,
`parts/alloc.c`: `hero_handle_eq` asks `hero_dead_holds_locked` of each side
under `hero_handle_lock` (a `pthread_mutex_t` on this Mac, `alloc.c:156`),
the crossing's own question, behind an atomic flag set at the first mark so
a program that never gave a handle back takes no lock; its abort words
modelled on the crossing's. Programs built against it with
`HEROES_RUNTIME=ffi-scratch/runtime-eq` (the first place
`cli/runtime_places.hero` looks), the compiler unchanged. Then a loop of
`if a == b` over two live handles at `-O2`, `N = 0` and `N = 10,000,000`,
with no handle given back before the loop and with one given back
(`ffi-scratch/cost/loop_*.hero`), each binary run three times under
`/usr/bin/time -l`, the line `instructions retired` alone kept (its timing
lines were cut by `grep` before anyone read them). `--emit-c` and `objdump`
first: the loop calls `Outer_eq`, which calls `hero_handle_eq` out of line,
so each turn pays it.

Printed, the shapes under route (a): the golden aborts before `null:` prints,
*panic: `==` read a copy of a C handle at an address a call marked
`consumes` ... ended*, so its `.expected` moves; s2, s3, s5, s6 and s7
abort the same way; s4 (after reuse) and s8 (the crossing) as before; and
**s9b, two LIVE `B` handles compared, aborts**, finding 3's false positive
carried from the crossing to `==`. Neither `==` abort names a place: the
runtime is handed none (`hero_handle_eq(const void *, const void *)`), which
the abort on the handle itself (s1b) already lacks at the frozen head.

Instructions retired, the median of three, per comparison as
`(N=10^7 - N=0) / 10^7`:

| runtime | no handle given back | one handle given back |
|---|---|---|
| frozen head | 13.0 (147,184,460 - 17,144,548) | 16.0 (177,145,831 - 17,142,339) |
| route (a) | 24.0 (257,197,686 - 17,301,995) | **137.0** (1,387,370,344 - 17,097,161) |
| route (a), the locked half out of line | 24.3 (259,624,517 - 17,045,564) | 150.0 (1,517,555,258 - 17,219,496) |
| for scale: the loop crossing into C instead, `value(o: a)`, frozen head | | 123.0 (1,247,977,788 - 17,557,412) |

Finding: once any handle has been given back, route (a) adds **121
instructions to every `==` on a handle** (137.0 against 16.0), a lock and two
probes, which is what one crossing into C costs today (123.0 a turn,
including the C call). The +11 before any end is my edit's (`objdump`: the
mutex path's frame is set up before the first test, so the fast path is no
longer a leaf), not the route's; outlining the locked half did not recover
it (24.3), so that floor is a question for whoever lands it. What no count
here measures: a global mutex taken by every handle comparison on every
thread, contended (unrun: it would take a clock). The bindings that would pay
most, named and **unrun** here: those that dispatch by identity, a callback
handed `uv_handle_t *` or `GLFWwindow *` and compared against the program's
list, one lock per element per event.

### 5. Item 3, route (b): the clause narrowed to what the tree does, and what it costs a binding

Command: `spec/heroes-spec.md` copied to `ffi-scratch/spec/`, one phrase of
line 440 replaced, and `env -u ANTHROPIC_API_KEY ./heroes-next measure` on
both copies from the worktree's root (offline, the vendored tables only).

The draft, the smallest edit found: *handing it to C again aborts, `==` on it
aborts, and ~~so does a copy made before~~ **a copy made before aborts where
it reaches C**, unless C has since handed its address out again or a million
other lives have ended.* `==` on a copy then falls under § 13's own earlier
sentence, *`nullptr` is its null and `==` compares the address*, which is
what s2, s3, s5, s6 and s7 print, so no sentence about it is added.

Printed: `maximum 7890` for the frozen text, `maximum 7894` for the draft,
**+4 vendored, a lower bound**; the real count is the spec-warden's
(`--refresh`, which this seat does not call).

Finding: route (b) moves no byte of C, no golden, no runtime, no emission,
and keeps every binding's C as it is; it states wart 20 and the diagnostic's
note in § 13's words. It leaves finding 3 a defect against § 13's own
*unless C has since handed its address out again*, which the runtime fails
to notice on a plain result or cell, under either route.

### 6. Item 6: what a program can bind from each header, and what binding `heroes_runtime.h` reaches

Command: each header's top-level function declarations counted from clang's
JSON AST, a declaration attributed to the file its location names
(`ffi-scratch/decls.py`, `types.py`): `hero_os.h` **57**, `heroes_runtime.h`
**165**. Which of the 57 a Heroes source binds, by `git grep -l` of
`function <name>(` per name over `examples/`, `tests/` without
`tests/golden/recovery/` and `selfhost/` (`ffi-scratch/os_use.tsv`). Then two
programs outside `selfhost/`: `e1` binds `hero_word_bits`, `hero_os_pid`,
`hero_thread_limit`, `hero_file_read_str(path: str, @status: i64) -> str`
from `hero_os.h` and `hero_program_sources() -> str`, `hero_print_int`,
`hero_print_end` from `heroes_runtime.h`; `e2` binds `heroes_runtime.h`'s
`hero_str_release_at(@s: str)` and calls it once on a string built at run
time, plain and `--sanitize`.

Printed: of `hero_os.h`'s 57, `examples/` bind **3** (`hero_thread_spawn`
and `hero_thread_join` in 10 programs each, `hero_thread_limit` in 1),
`tests/` 35, **21 are bound by neither** (the compiler's own helpers, such
as `hero_fs_deny_delete`, `hero_file_unstage`, `hero_fs_set_flag`,
`hero_run_signal`) and 3 by no Heroes source at all. Of `heroes_runtime.h`'s
165, Heroes sources bind three: the library `hero_str_try_from_cstr`
(`library_source.hero`), the compiler `hero_print_str` (`cli/io.hero`) and
`hero_program_sources` (`cli/help.hero`). `e1` built and ran, exit 0, every
line as expected (`bits: 64`, `status: 0, read: 6 bytes`, `42`). `e2`
printed its line, then plain: *panic: a str's block has lost its mark: ...
the block was freed*, exit 134; `--sanitize`: *AddressSanitizer:
heap-use-after-free ... in hero_str_hdr_checked str.c:138 ... hero_str_decref
... hero_str_release_at*, the scope's own release reading the freed block.

Finding: nothing refuses a program's `extern "heroes_runtime.h"`, and a
`str` crosses there as the runtime's `HeroStr`, a form § 13 does not list.
Through it a program with **no C of its own** reaches the runtime's reference
counts and a use-after-free (`e2`). So `heroes_runtime.h` is a second door
nobody named: decision 2228 named `hero_os.h` alone. Promising it would
freeze all 165 of the emitter's entry points for `1.x`, task 2237's removal
among them; leaving it unnamed leaves a door to memory unsafety that §4.10's
*`ptr`/`cstr` sits outside the guarantee* does not cover, since `e2` holds
neither. And `hero_os.h`'s promise, as decision 2228 words it, covers all 57,
21 of which no program outside the compiler binds.

### 7. Item 6: what a `HERO_RUNTIME_ABI` step does to a binding, built

Command: five copies of `runtime/` under `ffi-scratch/runtime-v0..v4`;
`e3` binds `hero_os_pid() -> i64`, `hero_thread_limit() -> i64` and
`hero_run_limit(seconds: i64)` (none of them bound by the library, so an edit
reaches the program alone), built against each copy by `HEROES_RUNTIME`.
`v0` moves `HERO_RUNTIME_ABI` 31 to 32 and nothing else; `v1` to `v4` each
change one declaration, first with the stamp moved, then with it agreeing:
`v1` renames `hero_os_pid` (as task 2237 removes a name), `v2` narrows
`hero_thread_limit`'s result to `int32_t`, `v3` narrows `hero_run_limit`'s
parameter to `int32_t`, `v4` makes that result `uint64_t`.

| runtime | build | what it said |
|---|---|---|
| frozen head | 0, run 0 | `pid positive: true`, `thread limit: 256` |
| `v0`, the stamp alone | **2** | *internal error: compiling the generated C failed: ... static assertion failed due to requirement '32 == 31': heroes_runtime.h is from another compiler*, for the program's unit and the library's |
| `v1`, a name gone | 1 | `error[ffi_unknown_name]: hero_os.h declares no hero_os_pid`, at the binding (its *fix (guess)* names `hero_fs_kind`) |
| `v2`, a result narrowed | 0, run 0 | the same two lines |
| `v3`, a parameter narrowed | 1 | `error[ffi_parameter_type]: seconds of hero_run_limit is declared wider than the header's int`, at the binding |
| `v4`, a result's sign changed | 1 | `error[ffi_return_type]: hero_thread_limit returns uint64_t in hero_os.h, not i64`, at the binding |

Finding: **the stamp never reaches a binding.** The compiler emits
`_Static_assert(HERO_RUNTIME_ABI == 31, ...)` into every unit, so a step
refuses a compiler and a runtime of two releases as a pair, *internal
error* at exit 2 for every program; a later release moves both, and a
program built by it never sees the number. What reaches a program is the
declaration that changed, and clang's check against the header makes every
change of a name, a parameter's width or a result's sign a compile error
at the program's own binding line (exit 1), never a different run; a result
narrowed is silent and holds every value. What the header cannot say is
what reaches a program in silence: whether a `HeroStr` result is the
caller's (`hero_os.h` documents *an owned `str` (+1)* in a comment, which no
check reads), and what a status code means. So a `hero_os.h` promise must
be of names, types **and** what each result owns.

### 8. Item 6: a C library the promise does not ship, changing under an unchanged compiler

Command: one program (`ffi-scratch/thirdparty/prog.hero`: a constant, a
two-field record, a function taking the record and an `i32`, a `-> cstr`
read by `validated().must()`) built by `heroes-next --include <dir>` against
nine versions of its header `mylib.h`, each one change from `v1`; the
binary run where it built. And `examples/sqlite/main.hero` built, `otool -L`
on its binary.

| the header's change | build | run |
|---|---|---|
| `v1` | 0 | `level: 1`, `scaled: 14`, `name: mylib` |
| a constant's value, 1 to 2 | 0 | `level: 2`: the new value, in silence |
| a parameter widened, `int` to `long long` | 0 | unchanged; C converts an `i32` exactly |
| a result widened, `int` to `long long` | **1**, `ffi_return_type`, fix *declare the result `i64`* | |
| the record gains a field | **1**, `ffi_incomplete_record`: *Point does not name z* | |
| the function marked `deprecated` | 0 | unchanged, silent (design.md §4.19, panel 208's R1) |
| the function marked `unavailable` | **1**, `ffi_unavailable`, the header's own words in a note | |
| the result now `malloc`ed for the caller to free, the declaration the same | 0 | unchanged, a leak no check sees |

(`v2b`, the widened parameter with a body that narrowed, was refused
`ffi_header_refused` for my header's own `-Wshorten-64-to-32`, and is
replaced in the table by `v2b2`, whose body does not narrow.) The SQLite
example's binary names `/usr/lib/libsqlite3.dylib (compatibility version
9.0.0, current version 406.0.0)`: the library is loaded at run time, from
the system.

Finding: the `importc` property holds across a library's versions: every
change that would make a call, a field or a result the wrong C type is a
compile error at the binding, under the SAME compiler. So a program the
promise covers can stop compiling because a library it binds moved, and the
paragraph must say so or it promises what no compiler controls. Three
changes pass in silence and change what runs: a constant's value, who frees
a result, and anything the library does behind an unchanged declaration.
And a built binary meets a newer shared library with no check at all; the
promise is about a program's source rebuilt, never a binary.

### 9. Item 6: the paragraph's *no Heroes code can catch that stop* is false, with no C of the program's own

Command: `ffi-scratch/catch/`: `c4` binds `<signal.h>` alone, `constant
SIGABRT: i32` and `function signal(sig: i32, handler: (function(sig: i32) ->
())) -> ptr`, hands it a Heroes function that prints and calls `exit(code:
0)`, then reads past an array's end; `c3` the same through a one-line header
of the program's own; `c5` the handler returning instead of exiting; `c6`
the handler doing further work, a loop, then `exit(code: 0)`; the critic's
F20 program (`critic-scratch/catch/catch.hero` and `catcher.h`, copied,
never run there). Then a runtime copy (`ffi-scratch/runtime-dfl`) whose
`hero_abort` writes `signal(SIGABRT, SIG_DFL);` before `abort()`, one line,
and `c4`, `c3` and F20's program again against it.

Printed, frozen head: `c4` `about to read past the end`, `Heroes code ran
inside the stop, signal 6`, stderr *panic: array index out of range / at
c4_signal_pure_heroes.hero:13:11*, **exit 0**, at `-O0` and `-O2`; `c3` the
same, exit 0; `c5` the handler's lines, then exit 134; `c6` *the handler
keeps working: 30*, exit 0; F20's `caught: the stop did not stop`, exit 0.
Against the reset runtime: `c4`, `c3` and F20's program each print the panic
and **exit 134**, no handler line. Omitting `signal`'s result is refused
`ffi_return_type`; declaring it `-> ptr` raised no FFI error against
`void (*)(int)`, the build's one refusal being `discarded_value` until
`_ =` (why the result check admits a function pointer as a `ptr` is not
asked here).

Finding: the paragraph's repair after F20 rested on *the promise is about the
language, and C is outside it*; `c4` writes no C. A Heroes function handed to
C runs after the runtime's report, may do arbitrary work and end the program
at 0; a supervisor reading the status, the paragraph's own deployment model,
sees success. Two routes, both built: **the sentence narrowed** (*no form of
the language catches it; a function the program hands C for `SIGABRT` runs
after the report, and what it does is outside the promise*), or **the
runtime's own stop made final**, one line in `parts/panic.c`, after which
F20, `c3` and `c4` end at 134. What the second costs, unrun here: a crash
reporter bound through the FFI (Crashpad, Breakpad, Sentry Native install
`SIGABRT` handlers) no longer sees the runtime's stops, only C's own; the
runtime's report on stderr and the status stay for the supervisor. It does
not touch the runtime's lease handler's purpose, which speaks for an abort
*this runtime did not raise* (`parts/os.c:268`), and `hero_runtime_spoke` is
set first. A question, unrun: the handle code drops its lock before every
report (`alloc.c`), so a handler blocking on it needs an abort raised under
the lock, an allocation failing inside `hero_handle_grow`, not forced here.

### 10. Item 6: does a program's own C call the runtime? Not in the tree

Command: `git grep -l -E '\bhero_[a-z_0-9]+\('` over `examples/*.h`,
`examples/*.c` and `tests/*.h`; `git grep -l heroes_runtime.h -- 'tests/*.h'`;
design.md §4.20, lines 2943 to 2995.

Printed: nothing, nothing, `0`. §4.20: *`hero_str_from_bytes` /
`hero_str_from_cstr` ... Without them no `extern function` may return
`str`*, which is the emitter's use (a `cstr` result made a `str`), not a
shim's.

Finding: no program's own C in the tree calls a runtime function, so a
promise that leaves `heroes_runtime.h` out breaks no C the tree holds; a
program's header still SEES the runtime's names, every unit declaring them
first (defect 2046), and C is C. What the exclusion closes is the other
door, `e2`'s: a program with no C binding the runtime's counts.

### 11. Item 3, route (a)'s reach in the pinned programs

Command: the harness's `run` form, `./heroes-next run tests/harness/main.hero
-- ./heroes-next run <filter>`, output to a file and read whole, with the
stock runtime (`dead`) and with `HEROES_RUNTIME=ffi-scratch/runtime-eq`
(`dead`, `limit-`, `stale-copy`); `git grep` of a comparison with `nullptr`
over `examples/`.

Printed: stock `run (only dead): 24 passed, 0 failed`; route (a) `dead` 24
and 0, `limit-` 6 and 0, `stale-copy` 1 and 1, the failure the golden the
brief names, *expected `before: 42` / `null: false`, actual `before: 42`,
exit 134, `==` read a copy of a C handle*; `examples/` holds no comparison
with `nullptr` (count 0).

### 12. Item 6: what a `hero_os.h` result owns can change with the declaration unchanged, and the build says nothing

Command: `runtime/` copied to `ffi-scratch/runtime-own/`, one function of
`parts/os.c` edited: `hero_file_read_str` keeps the string it read in a
static and returns that same `HeroStr` each call, a borrowed (`+0`) result
under the unchanged declaration `HeroStr hero_file_read_str(HeroStr path,
int64_t *status)`. `e4` binds it as `-> str` and reads one file twice,
built against the frozen runtime and against the copy, plain and
`--sanitize`.

Printed: frozen runtime, both reads `status 0, 6 bytes, first byte 104`,
exit 0. The copy: the build exit 0 and silent; `read 1` as before, then
*panic: a str's block has lost its mark ... the block was freed*, exit 134;
under `--sanitize`, *heap-use-after-free ... in hero_str_hdr_checked ...
hero_str_incref ... at e4_owned_moved.hero:6*.

Finding: finding 7's *what the header cannot say*, measured: the binding
check reads types, and `HeroStr` is `HeroStr` whoever owns it, so a `1.x`
runtime that changed one result's ownership would compile every program
that binds it and corrupt each at run time. `hero_os.h` states ownership
in comments alone (*an owned `str` (+1)*). A promise of `hero_os.h` that
does not promise what each result owns promises the half clang already
checks. How many it touches, from `ffi-scratch/os_types.tsv` (clang's types
of the 57): **12 return a `HeroStr`** (`hero_args_at`, `hero_args_shown`,
`hero_dir_at`, `hero_dir_at_shown`, `hero_env_shown`, `hero_exe_path_shown`,
`hero_file_read`, `hero_file_read_shown`, `hero_file_read_str`,
`hero_fs_landing`, `hero_run_discard_path`, `hero_run_exe_suffix`) and 6
take one.

### 13. Item 6: a later runtime that only ADDS a name stops an older program compiling, since no prefix is reserved

Command: `ffi-scratch/prefix/`: `mine.h`, a program's own header defining
`static inline long long hero_tally(long long, long long)`, bound by `p1`
as `function hero_tally(a: i64, b: i64) -> i64`; built against the frozen
runtime, then against `ffi-scratch/runtime-add`, a copy whose
`heroes_runtime.h` gains one declaration, `int64_t hero_tally(const char
*what);`, as a later release adds an entry point.

Printed: frozen runtime, `tally: 5`, exit 0. The copy: build exit 1,
`error[ffi_header_refused]: mine.h ... static declaration of 'hero_tally'
follows non-static declaration`, its note *at the Heroes runtime's own
`hero_tally`, which every unit of this program declares before any group's
header*, its *fix (guess)*: *rename `hero_tally` in `mine.h`, a name the
Heroes runtime declares first in every unit*.

Finding: every unit includes `heroes_runtime.h` before any group's header,
and panel 214's R3 decided *the specification reserves no prefix*
(`.claude/rules/c-boundary.md`), so ANY entry point a `1.x` runtime adds,
and any function a `1.x` `hero_os.h` adds for a program that binds it, can
refuse a `1.0` program whose own C used the name. The draft's *compiles
under every later 1.x compiler* and my own condition *a `1.x` adding to
`hero_os.h`* both need one of two things: the specification reserving the
runtime's forms for C a program writes (`hero_`, `HERO_`, `Hero`,
`HEROES_`, `h_`, the runtime's and the mangler's among the forms panel
214's R3 names, which adds `main`, `f_<field>` and `t<N>`), told before `1.0.0` and
amending that ruling, which is the robust route; or the paragraph
excluding a collision with a name a later runtime adds. The same question
one level up, unrun by this seat and the compiler-engineer's: whether a
built-in a `1.x` library adds can refuse a program that defined a function
of that name.

### 14. Beside items 3 and 6: the `==` abort on a dead handle says no place, against the paragraph's *saying why and where*

Command: s1b's whole stderr, `wc -c` and `cat` of
`ffi-scratch/handle/err_s1b_copy_vs_handle_helper.txt` (the runner cuts at
600 bytes, so the file was read whole); `grep -n hero_handle_eq
runtime/heroes_runtime.h`; `gh issue list --state all --search` for *read a
dead C handle place*, *hero_handle_eq place*, *abort without a place*,
*names no place panic*.

Printed: 270 bytes, the one sentence *panic: `==` read a dead C handle: ...
or keep a reference with `retains`*, no `at <file>:<line>`;
`heroes_runtime.h:350: bool hero_handle_eq(const void *a, const void *b);`,
no `_at` twin; no issue on it (the hits are panel 177's and 230's decisions,
defects 077, 098, 499, 066 and others on other shapes).

Finding: a runtime stop at the frozen head that names no place, so the
paragraph's *ends it at once, saying why and where* (item 6) is false of
it, and panel 230's R1 (*a stop says where*) did not reach this entry
point. Route (a) would add a second such stop unless `hero_handle_eq` gains
a place, which reaches every generated `eq` that walks a handle field (s6,
s7), since the generated comparison is per type and knows no call site. A
question for the coordinator to file, not a filing.

## Verdicts

### Item 3, `==` on a handle given back

- **verdict**: **approve route (b)**, the clause narrowed; **object to route
  (a)** as built and costed. No veto: neither route moves a byte across the C
  boundary.
- **section**: spec § 13 (*and so does a copy made before*) against design.md
  Part 8 wart 20 (*a copy made before the call is refused at its first
  crossing*; *`==` on a dead value aborts*), the same commit `c8b1bc878`;
  design.md §1.12 for what is at stake; CLAUDE.md § 12 (the specification
  beats the compiler unless the sitting rules otherwise): here the compiler
  keeps design.md, the source of truth, and the sentence is what departs.
- **experiment**: findings 1, 3, 4, 5 and 11: `slots.h`, `xtype.h`,
  `xcell.h` with s1 to s9e, plain, `-O2` and `--sanitize`; route (a) as
  `runtime-eq/parts/alloc.c`; the loop's instructions retired; the `run`
  form under both runtimes. clang accepted every header and every edited
  runtime.
- **argument**: `==` on a copy compares two addresses and reads no memory,
  in all five shapes at exit 0; the copy is stopped where memory is at
  stake, its crossing (s8). Wart 20 and the checker's own note say exactly
  that; § 13 compressed it into words a reader takes otherwise. Route (a)
  buys a logic check bounded by 2^20 ends and blind after reuse, for +121
  instructions and a global mutex on every handle `==` once any handle has
  been given back, moves the golden, and aborts s9b, two LIVE handles
  compared, because the remembered set misses a plain result (finding 3).
  Route (b): +4 vendored, nothing in C moves.
- **prediction**: route (b) changes no emission and no golden; route (a)
  as built moves exactly the one pinned program the brief names among the
  31 handle cases (measured), and s9b aborts under it on every leg,
  nothing in it being platform-shaped (unrun off this Mac).
- **condition**: route (a) becomes acceptable if finding 3 is repaired first
  (every handle C hands the program clears its mark) and its price is
  taken without a global lock, or if anyone shows a program whose memory a
  stale `==` reaches; neither was found here.

### Item 6, the promise at the C boundary

- **verdict**: **object** to the draft's C-boundary sentence and to its
  *no Heroes code can catch that stop*. No veto: nothing here breaks the C
  ABI.
- **section**: design.md §1.11 and §4.19 (the header is the contract, read
  by clang at every build), §4.20 (`heroes_runtime.h` is what *generated C
  includes*; the document says nothing of a program binding it, so that
  door is uncovered), §4.10 (`e2` holds no `ptr` and no `cstr`), Part 7 item
  2 as decision 2228 amended it, Part 6's row *Exceptions*.
- **experiment**: findings 6 to 10, 12 and 13: `e1`, `e2`, `e3` against
  `runtime-v0` to `v4`, `e4` against `runtime-own`, `mylib.h` in nine
  versions, `otool -L`, `c3` to `c6` and F20's
  program against the frozen runtime and `runtime-dfl`. clang accepted every
  header but the ones the table says it refused.
- **argument**: the draft promises `hero_os.h` without saying what of it:
  clang re-checks names and types at every build (v1, v3, v4 refused at the
  binding; the stamp, v0, never reaches a binding), but not what a `str`
  result owns (`e4`: owned made borrowed, built silent, a use-after-free
  at run time). It is silent on `heroes_runtime.h`, through which a program
  with no C of its own reaches a use-after-free (`e2`). It is silent on a
  library changing under the program, when the SAME compiler then refuses
  it (record field, `unavailable`, wider result), which its first sentence
  promises cannot happen. And `c4` catches the stop with no C written.
- **prediction**: `c4`, built at the frozen head, exits 0 after the
  runtime's abort on the CI's two Linux legs as on this Mac, and exits 134
  there once `hero_abort` resets `SIGABRT` (unrun off this Mac).
- **condition**: approve when the paragraph (1) promises `hero_os.h` as it
  stands at `1.0.0`, names, types and what each result owns, a `1.x` adding
  and never removing, narrowing or re-owning, the 21 functions no program
  outside the compiler binds moved out first or frozen knowingly, and the
  runtime's name forms reserved from a program's own C so that adding
  stays harmless (finding 13); (2) puts
  `heroes_runtime.h` outside it, by refusing a program's `extern
  "heroes_runtime.h"` once the three declarations Heroes sources bind move
  to `hero_os.h` (robust), or by naming it (conservative); (3) says a C
  library the program binds is outside it, its header re-read at every
  build so a change there may stop the program compiling, and what the
  header cannot show is the library's; (4) makes the stop final, one line
  in `hero_abort` (built: F20, `c3`, `c4` end at 134; it costs a crash
  reporter bound through the FFI the runtime's stops, unrun), or says a
  function handed to C for `SIGABRT` runs after the report, outside the
  promise (conservative).

### Draft words for the paragraph's C half, for the spec-warden to price

> The promise covers the language the specification describes, the
> functions it names, and those `hero_os.h` declares at 1.0.0, which a 1.x
> release may add to but never removes, narrows, or changes in what its
> result hands the caller. Any other C a program binds is outside it: its header is
> read again at every build, so a change in a name, a type or a record's
> fields stops the program compiling where it binds it, and a change no
> header shows, a value or who frees, is that library's to promise.

And for the stop, with it made final in `hero_abort` (finding 9's second
route): *... ends it at once, saying why and where, and nothing catches that
stop.* With the sentence narrowed instead: *... and no form of the language
catches that stop; a function the program hands C for `SIGABRT` runs after
the report, outside this promise.* Either way *and where* is false of the
`==` abort until finding 14 is repaired.

## Edited in place before 21:19, said so (2026-10-29, 21:21)

This report is a record of `docs/panel/` and takes its corrections
underneath; before the coordinator's message of 21:19 the seat edited its
own lines in place while writing them. Each such edit, with what the line
said before:

- the opening paragraph: *(ignored by nothing, removed with the worktree;
  none is committed)* became *untracked in a detached worktree nothing
  commits from*;
- finding 4: *The bindings that pay it are the ones that dispatch by
  identity ...* became *The bindings that would pay most, named and
  **unrun** here ...*;
- finding 4: the outlined variant's figure with no handle given back read
  **24.2**; it is 242,578,953 / 10^7 = 24.26, so **24.3**, in the table
  and in the text, and both outlined figures gained their medians;
- finding 9: *declaring it `-> ptr` passes the result check (clang reads
  `void (*)(int)` as fitting a `ptr`)* stated a cause nobody ran; it now
  keeps what was printed and says the cause is not asked;
- finding 12 gained its last sentences, the counts of `HeroStr` results
  (an addition);
- finding 13: *the forms panel 214's R3 names* became the runtime's and the
  mangler's forms *among* them, R3 also naming `main`, `f_<field>`, `t<N>`;
- the verdicts: item 3's *section* line reworded (*which lets the sitting
  rule that design.md ... is what the sentence failed to say*), its
  *prediction* marked *unrun off this Mac*; item 6's *experiment* and
  *argument* extended to cite findings 12 and 13 and its condition (1) the
  name reservation; the draft words' *never removes, narrows or makes return
  what the caller must free* became *never removes, narrows, or changes in
  what its result hands the caller*; the stop's routes, *R1* and *R2*,
  renamed by finding 9's words, with the *and where* caveat added;
- line 8 itself, rewritten in place at 21:07 and restored at 21:19 with its
  correction underneath; that correction first said *ran it 24 times*,
  fixed within the minute to 43.

## The author's rule, asked of every route here (2026-10-29, 21:22)

`00-shared.md`'s frame: before `1.0.0` no form enters to keep an older
program compiling. Asked of each route this report proposes, by what it
changes:

| route | what it changes | a form of the language? |
|---|---|---|
| item 3 (a), `==` asks the remembered set | `runtime/parts/alloc.c` | no |
| item 3 (b), the clause narrowed | one phrase of spec § 13 | no |
| finding 3's repair, every inbound handle clears its mark | the emitter's handle traffic and the runtime | no |
| finding 14's repair, a place for the `==` abort | an `_at` entry point and the generated `eq` | no |
| item 6 (1), `hero_os.h` promised as names, types and ownership; its compiler-only functions moved out before `1.0.0` | the paragraph, the header | no; the move may refuse a `0.x` program binding a moved function, which the rule allows, told |
| item 6 (1), the runtime's name forms reserved from a program's own C | a sentence of § 13 and a refusal | no; a refusal, told |
| item 6 (2), a program's `extern "heroes_runtime.h"` refused | a checker refusal, three declarations moved to `hero_os.h` | no; it refuses the two `check` goldens and the frozen recovery copy that bind it, told |
| item 6 (4), the stop made final | one line of `parts/panic.c` | no |

None keeps an older binding compiling by a form. Two would refuse older
programs before `1.0.0`, which the rule allows and asks to be told; after
`1.0.0` the `hero_os.h` promise forbids exactly that, which is why the move
and the refusal are owed before the tag.

### 15. Item 3, route (a)'s reach over the whole `run` form (2026-10-29, 21:30)

Command: `HEROES_RUNTIME=ffi-scratch/runtime-eq ./heroes-next run
tests/harness/main.hero -- ./heroes-next run > ffi-scratch/suite_run_routea.txt`,
started 21:00, the file read whole when the harness exited at 21:30.

Printed: `run: 599 passed, 1 failed`; the failure
`run/fixedbugs-a-freed-object-read-through-a-stale-copy-is-not-a-null-handle
at -O0`, *expected `before: 42` / `null: false`, actual `before: 42`, exit
134*, the route's own `==` abort; two cases `SKIP`, *`sys/prctl.h`, which
clang does not find*, judged on Linux (unrun here); `harness: 599 passed, 1
failed`, exit 1.

Finding: route (a) as built moves exactly one of the 600 `run` cases this
Mac judges, the golden the brief names, and no other pinned program
compares a copy of a handle given back.

## Final verdicts, in the brief's structure (2026-10-29, 21:33)

These restate the verdicts above, written before 21:16, with the cost field
the coordinator asked for and findings 12 to 15; where the two differ,
these stand.

### Item 3, `==` on a handle given back

- **verdict**: **approve route (b)**, § 13's clause narrowed to what the
  tree does; **object to route (a)**. No veto: neither route moves a byte
  across the C boundary.
- **section**: spec § 13 (*and so does a copy made before*) against design.md
  Part 8 wart 20 (*refused at its first crossing*; *`==` on a dead value
  aborts*), one commit, `c8b1bc878`; design.md §1.12 for what is at stake;
  CLAUDE.md § 12, the sitting ruling that the sentence, not the compiler,
  departs from design.md.
- **experiment**: `ffi-scratch/handle/`: the golden with its header, `slots.h`
  (four static `outer`s, `release` a `memset`), `xtype.h` and `xcell.h` (one
  arena handed out as `ta *` then `tb *`), programs s1 to s9e, each `heroes-next
  build <f> -o bin_<f>` then `timeout 20 ./bin_<f>`, plain, `-O2`,
  `--sanitize`; route (a) as `ffi-scratch/runtime-eq/parts/alloc.c`,
  `hero_handle_eq` asking `hero_dead_holds_locked` under `hero_handle_lock`,
  built through `HEROES_RUNTIME`; `ffi-scratch/cost/loop_*.hero` at `-O2`
  under `/usr/bin/time -l`, its `instructions retired` alone; the harness's
  `run` form under both runtimes. clang accepted every header and runtime.
- **argument**: `==` on a copy compares two addresses and reads no memory,
  answering by address at exit 0 in five shapes; the copy is stopped where
  memory is at stake, its crossing (s8). Wart 20 and the checker's note say
  exactly that; § 13 compressed it into words most readers take otherwise.
  Route (a) buys a logic check bounded by 2^20 ends and blind after reuse,
  pays a global lock on every handle `==` once any handle was given back,
  and aborts s9b, two LIVE handles compared, because the remembered set
  misses a plain result (finding 3), a correct program the crossing already
  refuses today.
- **cost**: route (a), +121 instructions a handle `==` once any handle has
  been given back (137.0 against 16.0; one crossing into C is 123.0), one
  global `pthread_mutex` a comparison (contended, unrun), 20 lines of
  `alloc.c` as built, a place for its abort still owed (an `_at` entry point
  and every generated `eq` over a handle, unbuilt), 1 of 600 `run` cases
  moved (599 passed, 1 failed, 2 skipped here); route (b), one phrase of
  § 13, +4 on the vendored maximum (7,890 to 7,894, a lower bound; the real
  is the spec-warden's), no byte of C, no golden, no emission.
- **prediction**: route (a) as built moves that one golden on Linux too,
  where the two skipped cases run, and turns s9b into an abort on every
  leg; route (b) moves no emission anywhere (both unrun off this Mac).
- **condition**: route (a) becomes acceptable once finding 3 is repaired
  (every handle C hands the program clears its mark) and its check costs no
  global lock, or once anyone shows a stale `==` reaching memory; neither
  was found. Findings 3 and 14 are owed under either route.

### Item 6, the promise at the C boundary

- **verdict**: **object** to the draft paragraph's C half and to *no Heroes
  code can catch that stop*. No veto: nothing proposed breaks the C ABI.
- **section**: design.md §1.11 and §4.19 (the header is the contract, read
  by clang at every build), §4.20 (`heroes_runtime.h` is what generated C
  includes; a program binding it is a door the document does not cover),
  §4.10 (`e2` holds no `ptr`, no `cstr`), Part 7 item 2 as decision 2228
  amended it, Part 6's row *Exceptions*; panel 214's R3 (*the specification
  reserves no prefix*, `.claude/rules/c-boundary.md`).
- **experiment**: `ffi-scratch/bind/` `e1` (both headers bound, exit 0),
  `e2` (`extern "heroes_runtime.h"` / `function hero_str_release_at(@s:
  str)`: `heap-use-after-free` under `--sanitize`), `e4` against
  `runtime-own` (a `HeroStr` result made borrowed: built silent, then a
  use-after-free); `ffi-scratch/abi/e3` against `runtime-v0` to `v4`;
  `ffi-scratch/thirdparty/prog.hero` with `--include` over nine `mylib.h`;
  `ffi-scratch/prefix/p1` against `runtime-add`; `ffi-scratch/catch/` `c3`
  to `c6` and F20's copied program, against the frozen runtime and
  `runtime-dfl` (`signal(SIGABRT, SIG_DFL);` before `abort()`); `otool -L` on
  the SQLite example. clang accepted every header but those the tables say
  it refused.
- **argument**: the draft promises `hero_os.h` without saying what of it:
  clang re-checks names and types at every build (refused at the binding
  when they move; the stamp never reaches a binding), never what a `str`
  result owns (`e4`). It is silent on `heroes_runtime.h`, where a program
  with no C reaches a use-after-free (`e2`); on a library changing under
  the program, when the SAME compiler then refuses it; and on a runtime
  that only ADDS a name (`p1`), which with no prefix reserved refuses an
  older program. Its first sentence promises neither can happen. And `c4`
  catches the stop with no C written.
- **cost**: the C half as drafted here, 122 on the vendored maximum against
  the draft's 25 (+97, a lower bound); the stop made final, 1 line of
  `parts/panic.c` and its sentence 29 against the draft's 32, its price a
  crash reporter bound through the FFI losing the runtime's stops (unrun);
  the stop narrowed instead, 54 (+22); refusing a program's `extern
  "heroes_runtime.h"`, 3 declarations moved to `hero_os.h`, 2 `check`
  goldens and 1 frozen recovery copy refused, 0 of `examples/`; reserving
  the runtime's name forms in a program's own C, 24 golden headers by a
  pattern (12 `run`, 12 `unsupported`), 0 of `examples/`; moving the
  compiler-only functions out of `hero_os.h`, 21 of 57; the ownership half
  of the promise, 12 of 57 returning a `HeroStr`.
- **prediction**: `c4` exits 0 after the runtime's abort on the CI's Linux
  legs as here and 134 once `hero_abort` resets `SIGABRT`; `p1` stops
  compiling under any runtime that adds `hero_tally`, on every leg (unrun
  off this Mac).
- **condition**: approve when the paragraph (1) promises `hero_os.h` as it
  stands at `1.0.0`, names, types and what each result owns, a `1.x`
  adding and never removing, narrowing or re-owning, the 21 compiler-only
  functions moved out before the tag, and the runtime's name forms reserved
  from a program's own C so that adding stays harmless; (2) puts
  `heroes_runtime.h` outside it, by refusing a program's group naming it
  (robust) or by naming it (conservative); (3) says a C library the program
  binds is outside it, its header re-read at every build so a change may
  stop the program compiling, what no header shows the library's own, and
  a built binary meeting a newer shared library outside it too; (4) makes
  the stop final in `hero_abort` (robust, built) or says a function handed
  to C for `SIGABRT` runs after the report (conservative), *and where*
  waiting on finding 14 either way.
