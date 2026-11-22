# Panel 254: a value crosses threads whole through a mailbox, a handle belongs to the thread that holds it, a stop drains then holds, and no queue holds itself

Convened 2026-11-22 by the coordinator of M-rebel-library on feature 2647, by the author's instruction of
that day. **The full panel**: the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian
(advisory) and the llm-ergonomist (the blind seat, prepared and **not run**, § The blind seat), with the
completeness critic reading the briefs before any seat and the reports after. **The frozen commit**:
`89f155330d3da43b429f6ad8f8c6c730b171a35d` of `rebel-round`, in the lane `lane-p254`; every seat in a
detached worktree of it under `.claude/worktrees/p254-scratch/<seat>/`, each building its compiler from
the seed and two stages (`sources 6e891e681c14bca0`). Briefs: `docs/panel/254-briefs/`; reports:
`docs/panel/254-reports/` (`critic-briefs.md`, `spec-warden.md`, `historian.md`, `compiler-engineer.md`,
`ffi-pragmatist.md`, `llm-ergonomist.md`, `critic-reports.md`, `resolution/`, `evidence.md`).

**Status: provisional — author ratification pending.** The resolution below is the most robust and
complete the seats' measurements support (CLAUDE.md § 4, and § Precedence as amended at `691531908`).
It is provisional on the author **and on the blind seat**, whose registered run has not been made.

## How the question changed while the sitting was convened

All of 2026-11-22, each relayed by the coordinator of M-rebel-library, meant as:

1. *fix feature 2647: in Heroes a thread receives one value when it starts and cannot exchange values
   while it runs, so gateway serves one connection at a time* (the soundness lane, the library route A);
2. *if it is needed, the specification can be touched too* (both routes asked);
3. *go directly to the most robust solution, and spending up to $20 of tokens is fine*: **a full panel on
   the language route B**, panel 251's Q3 in spelling M, the library route recorded as conservative;
4. **the contract rule committed at `691531908`**, CLAUDE.md § Precedence: *"the most robust solution now
   always wins, a new form included and at the cost of the spec's size: Principle 0, the spec's budget
   (design.md §1.6) and the spec-warden's vetoes yield to it, and the ceiling is raised to fit"*; a
   soundness veto still binds. **By this rule, panel 251's R3 reasons 1 (the spec-warden's veto) and 2
   (Principle 0) no longer bind a robust resolution**, and the spec-warden's definition says the same.

Every seat read the author's instruction of 2026-11-22 as **the ruling on the need** (Principle 0): the
compiler-engineer (*"I read the author's instruction of 2026-11-22 as the ruling on the need"*), the
spec-warden (§ 5 of its report), the ffi-pragmatist (*"Principle 0, in one line"*, its report's line 59); the historian gave no reading of it.
**No seat finds the compiler needs the route to compile itself** (`selfhost/main.hero:88-113` starts one
thread and exchanges nothing with it).

## The proposal, verbatim

As put to the seats after the critic's first reading: `docs/panel/254-briefs/arms/spec-m.diff`, made from
the frozen spec by `arms/make_arms.py` (every edit asserted to match once), the semantics rows S1 to S14
of `00-shared.md` § 5 beside it:

````diff
--- spec-now.md
+++ spec-m.md
@@ -107,19 +107,22 @@
 | `()` | nothing: what a function with no `->` returns |
 | `ptr` `cstr` | an opaque pointer and a C string; `nullptr` is the null of both |
 | `(function(A) -> B)` | a function value, of any arity; the parentheses are mandatory |
+| `(thread T)` | a running thread whose function answers a `T`; a handle (section 13) its wait ends |
+| `(mailbox T)` | a queue of `T`s, which every copy of it reaches (section 11) |
 
 - No implicit conversions, widths included: `1 + 2.0` is an error.
 - Every value behaves as an independent copy: after `b = a`, mutating `b`
   never changes `a`. No aliasing exists among the values this language owns.
-  A `ptr` or a `cstr` is a copied ADDRESS, wherever it sits: two copies reach
-  one foreign thing, so a function taking one without `@` may still change, or
-  free, what C holds.
+  A `ptr`, a `cstr` or a `(mailbox T)` is a copied ADDRESS, wherever it sits: two copies reach
+  one foreign thing, or one queue, so a function taking one without `@` may still change, or
+  free, what it reaches.
 - A record or variant holds its fields **by value**, so it may contain itself only
   through `[T]` or `{K: V}`: `children: [Node]` is a tree, `child: Node` has no size.
 
     Type     = Prefix { "[" integer "]" } [ "?" ] .
     Prefix   = ident [ "." ident ] | "[" Type "]" | "{" Type ":" Type "}"
-             | "(" ")" | "(" "function" TypeArgs "->" Type ")" .
+             | "(" ")" | "(" "function" TypeArgs "->" Type ")"
+             | "(" ( "thread" | "mailbox" ) Type ")" .
     TypeArgs = "(" [ TypeArg { "," TypeArg } ] ")" .
     TypeArg  = Type | ident ":" Type .
 
@@ -228,7 +231,7 @@
 → `<<` `>>` → `&` → `^` → `|` → comparisons → `&&` → `||`.
 
 `==` is structural equality on any two values of one type, recursively; a map's
-insertion order does not affect it. A `ptr`, a `cstr` and a function value compare
+insertion order does not affect it. A `ptr`, a `cstr`, a `(thread T)`, a `(mailbox T)` and a function value compare
 as an address and not as an identity — free one and the next allocation may reuse
 that address. `nan` equals nothing, itself included, so `x != x` asks whether it is one;
 ordering one aborts, in `< <= > >=` and in `sort`.
@@ -303,7 +306,8 @@
   the same two, `(function(from: str, to: str) -> ())`. The names are part of the
   type: the other order is another type, and a call through a value names each in
   its own position. A function that has such a pair names it as the type does.
-- `x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods, no
+- `x.f(y)` is sugar for `f(x, y)` (UFCS), except a thread's `wait` and a mailbox's `send`,
+  `receive` and `close` (section 11), which are written only after a dot. There are no other methods, no
   inheritance, no overloading, no default values, no user variadics, no
   anonymous functions.
 - Mutable parameters are marked `@` in the signature and at the call site:
@@ -356,7 +360,7 @@
 Built-ins: `print(...)` · `len` · `push` · `xs.slice(from:, to:)` ·
 `chars` · `keys` · `join(xs, sep)` · `repeat(s, n)` (`n` a `u64`) · `sort` (ascending: a number by value, a `str` by bytes, `false` before `true`; never a type parameter) ·
 `to_f32` · `to_f64` · `to_str` · `to_i8` `to_i16` `to_i32` `to_i64` `to_u8` `to_u16`
-`to_u32` `to_u64` — and, written in Heroes, calling the function in order: `map` · `filter`, keeping what
+`to_u32` `to_u64` · `spawn` · `spawn_sized` · `mailbox` — and, written in Heroes, calling the function in order: `map` · `filter`, keeping what
 the function accepts · `fold` (its function names them `acc` then `item`) · `find` (the first it accepts, else `not_found`) ·
 `any` · `all` (with `find`, stopping at the first that decides) · `range`.
 `slice` and `range(from: a, to: b) -> [i64]` exclude `to`.
@@ -375,6 +379,8 @@
 str) -> ()?` (`write_failed`) · `args() -> [str]` (the arguments after the
 program name; one that is not UTF-8 aborts) · `args_checked() -> [str?]` (which
 does not) · `exit(code: i64)` (ends the program; a code outside 0 to 255 aborts, one written as a number is refused).
+Threads, also provided: `spawn(f, x)` runs `f(x)` on a thread of its own and gives a `(thread T)?`, `T` being `f`'s answer, failing `limit` where the system refuses a thread; `spawn_sized(f, x, bytes: n)` does the same on a stack of at least `n` bytes, and an `n` below 0 aborts. `t.wait()` waits for the thread and gives its answer, and ends the thread's life as a call marked `consumes` ends a handle's (section 13). `mailbox()` gives a new, empty `(mailbox T)`, its type written where it is bound: `m: (mailbox Job) = mailbox()`. `m.send(v)` adds a copy of `v` and never waits, failing `closed` once the mailbox is closed; `m.receive(within_ms: n)` takes the oldest value, waiting at most `n` milliseconds, `-1` for no limit and below that an abort, and fails `timeout`, `closed` once it is closed and empty, or `stopped` once a stop is asked (section 13), even with values in it. `m.close()` closes it for every copy, and closing it again does nothing; the values still in it are received, or dropped with its last copy, a handle among them told when `main` returns as one nobody consumes. Every value crosses a thread whole, as a copy, through `spawn`, `wait` and a mailbox, so threads share nothing but a mailbox; a `ptr` or a `cstr`, or a value holding one, never crosses, and a handle, or a value holding one, crosses only written with `@`, `spawn(f, @x)` or `m.send(@v)`, which empty it, its life going with it, or as the answer `wait` gives.
+The names `spawn`, `spawn_sized` and `mailbox` are taken; `wait`, `send`, `receive` and `close` are written only after a dot, and stay free for the program.
 
 ## 12. Tests and holes
 ```
@@ -392,7 +398,7 @@
 with holes type-checks everything else but produces no binary.
 
 ## 13. FFI
-Anything beyond this document comes from packages or C libraries; threads, processes and files from the language's `hero_os.h`, and sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names one. A thread runs a function of one `i64` or one `str` and gives back one of the same, a `str` copied each way, and nothing else crosses: `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` or `hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64` starts it, and `hero_thread_join(handle: i64) -> i64` or `hero_thread_join_str(handle: i64) -> str`, as it was started, waits for its answer. A group names its header, and `link` a library when the symbols need one; C
+Anything beyond this document comes from packages or C libraries; processes and files from the language's `hero_os.h`, and sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names one. A group names its header, and `link` a library when the symbols need one; C
 reads a module's headers in the order its groups are written: one that needs
 another's names comes after it, and one defining `_GNU_SOURCE` first. clang
 checks every result type, constant and record field against that header, and a result may be
@@ -419,7 +425,7 @@
 aborts, or fails the call where the kernel writes, and so past a local lent with
 `@`, which C receives itself; past a field or an element it is the binding's word.
 A callback is a **parameter**, never a result or a signal handler, nor called by one; its parameters follow the same rule
-and `()` is `void`: `atexit(f: (function() -> ())) -> i32`. No function of the program is a signal handler: after `hero_net_catch_stop() -> i64` (`hero_net.h`), Ctrl-C, SIGTERM and `hero_net_ask_stop() -> ()` only make `hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts any other signal, and `hero_signal_take(sig: i32) -> i64` gives the count since its last call
+and `()` is `void`: `atexit(f: (function() -> ())) -> i32`. No function of the program is a signal handler: after `hero_net_catch_stop() -> i64` (`hero_net.h`), Ctrl-C, SIGTERM and `hero_net_ask_stop() -> ()` only make `hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer, or a mailbox's `receive`, answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts any other signal, and `hero_signal_take(sig: i32) -> i64` gives the count since its last call
 
 A group's `record` is the header's struct: all its fields, and the same name
 unless the header writes it after the word struct, which `tag` gives:
@@ -464,7 +470,7 @@
 `x: cstr @= s.lease()` is a COPY of the bytes that C may read for as long as the
 program says, and `end_lease(@x)` frees it and empties the cell. A lend and a
 lease name stand only as an argument of a call, nothing else writes a lease's
-cell, and a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when
+cell, and a lease nobody ends, like a handle nobody consumes or a thread nobody waits for, aborts when
 `main` returns, saying how many. No word says C frees what it is handed; one
 that frees a lease kills the process, naming the leases live. Bytes C owns come
 from its own allocator, with their disposer.
````

## The critic's first reading, and what it changed before any seat

37 findings (`critic-briefs.md`), each answered in the briefs before a seat started: t2's program 5
moved its second `free` behind a branch the checker cannot decide (§ 13 makes the straight-line read a
compile error, `error[handle_used_after_end]`), program 3 calls `hero_net_catch_stop()` first; D1 graded
by one instrument for both arms; voiding for a missing transcript, another model, a read outside the
folder (`tools/reads.py`, new); `m` out of the draw's letters; the proposal's text repaired (the thread
row, S3 as a handle's end, `spawn_sized`'s result, the UFCS sentence's four exceptions, `==` on a thread
and a mailbox, the mailbox's type where it is bound, `spawn(f, @x)`, a queued handle told); the routes
nobody had listed (A0, Rebel's `ipc` channel by name, run by the critic on t3's shape, 5 of 5 plain and 3
of 3 sanitized; 7.13's own mailbox and its first rung); the soundness questions (a queue holding itself,
`spawn` without `@`, the stop process-wide and permanent); and eight false or loose citations.

## The verdicts

| seat | verdict | the measurement it rests on |
|---|---|---|
| spec-warden | **approve B; object to the text as written**, ten edits for nine defects (W-a to W-j, `p254/spec-w2.md` of its copy); no veto (since `691531908` neither the budget nor Principle 0 vetoes) | four refreshes, `claude-opus-5`: `now` **11,788** real, `m` **12,454 (+666)**, `m0` 12,071 (+283), `w` 12,548 (+760); vendored `now` 9,022, `m` 9,488, `w2` 9,567; **the ceiling needs no raise**: `w` leaves 16,384 - 12,608 = **3,776**; probes: `spawn(f, @x)` cannot complete a hand-over (`mutable_parameter_as_value`, `consumed_borrowed_handle`), 28 of 28 built-in names already taken everywhere |
| compiler-engineer | **approve B, no veto; object to S1, S5, S6, S7, S11, S12**, each amended | one `Ty` case **289 `error[non_exhaustive]` in 110 files**, all four cases 313 in 119 (R3 reopened under 30); `m.send` resolves by §4.11's first lookup (`b.send(5)` 10, `send(b, 5)` 1005), no new kind of name; the runtime half built (`p254/q2/mail.c`, 454 lines): a flat encoding by per-type functions beside the descriptor, five members kept, every program block born and freed on one thread; **188 runs, 65 under ASan, UBSan or TSan, 0 reports**; the gateway emulation 6,400 of 6,400 right, 15.5 times one thread at 16; the stop: stop-wins **8 of 32** connections served; the cycle silent under today's gate |
| ffi-pragmatist | **soundness veto on S11 as written**, lifted by the holder check it built; approve B with S1, S6, S7, S9, S11, S12 amended | TLS handed between threads on Darwin 20/20, ASan 5/5, TSan 5/5, 0 reports; socket stream, listener, UDP, number 20/5/5; SQLite moved 20/5/5, **used by two threads at once killed 20 of 20**; **a copy kept before `m.send(@db)` killed 8 of 10**, with the holder check stopped by name 10 of 10, about 3.6 ns per handle argument; route A built in Heroes, TLS served on 16 workers **11.1 times** one; spelling B's veto still holds (`builtin_name_taken` on C's `exit`) |
| historian (advisory) | **approve B; object to four rows as written** (S5, S6 with S11, S7, S9) as unnamed departures | pages read 2026-11-22: a failed send returns the value (Rust), a second close does nothing (Kotlin), a drain by default (Python 3.13's `shutdown`), a transfer detaches the sender's object (MDN); **no system read makes a stop process-wide and permanent** over queued values |
| llm-ergonomist (blind) | **not run** (§ The blind seat) | 26 sessions drawn, free preflight clean, 0.000000 USD spent |

## The critic's second reading, and what it changed

24 findings (`critic-reports.md`), each settled by a command where one could settle it:

1. **The S11 veto holds, re-run**: without the holder check 9 of 10 killed, with it 10 of 10 stopped and
   10 of 10 clean moves; `spawn` and `wait` covered by the same two calls; the thread number the check
   uses is the runtime's own, so a reused `pthread_t` does not fool it (`deadthread`); **a handle sent back
   makes old copies on its first thread valid again**, sound and not what *"a copy ... aborts"* says; **the
   message is false in three of four shapes**; a handle in a record or a list is covered only if one walk
   visits every handle a value holds. **The text is owed by thread, not by copy.** The `runtime` suite on
   the holder runtime: 10 passed, 0 failed.
2. **The drain is not better on every count**: it serves 32 of 32 but, **with a reused reply mailbox, tells
   a count never applied in 6 of 6 runs**, where stop-wins told none. **A fourth behaviour nobody listed, a
   drain and then `stopped` for ever**, served 32 of 32 and told no wrong count in 12 plain runs and 12 under
   TSan and ASan+UBSan, 0 reports (`p254/s7/m3/`, an emulation on one Mac).
3. **W-j' is non-local**: a function's body judged by a `spawn` elsewhere; W-j'' (`spawn(serve, @c)` with
   `function serve(@c: Conn)`), local, unlisted and unpriced.
4. **W-f is decidable at declarations** (generics on functions only, polymorphic recursion refused); its
   walk misses a thread's answer; it refuses the peer shape (an address of the same message type inside
   the message); **split ends** (Rust's `mpsc`) recorded as the unpriced route that needs no refusal.
5. **Premises**: defect 2588's red was mended at `ee336f2f4` and `804b72763` before the frozen commit (the
   issue stays open for its round); the 257th `spawn` exits 134 (`spawn.c:520-523`); a poll set takes
   neither an `ipc` channel nor a Darwin TLS stream; the seed names the doors 12 times.
6. **Unasked**: a mailbox in a `poll` set, a send that fails `full`, a `wait` with a limit, a one-shot
   reply, a deadlock report.

## Where the seats disagree, stated plainly

- **S7, the stop.** The spec-warden kept *"`stopped`, before all else"* (W-i), written for the stop that
  wins; the compiler-engineer and the ffi-pragmatist amended to a drain; the historian found no precedent
  for a process-wide overtaking stop. **Settled by the critic's table**: stop-wins strands 24 of 32 handed
  connections, the drain tells stale counts in silence on a reused reply mailbox, the drain-then-hold
  behaviour did neither in 24 runs. Not settled on Linux or on the compiler-engineer's stress topologies.
- **S11, how a handle enters a thread.** The spec-warden: through a mailbox only (W-j), since
  `spawn(f, @x)` hands `f` a borrowed parameter it cannot consume (probed). The compiler-engineer: keep
  `spawn(f, @x)` and let `f` own it (W-j', about 20 lines). The critic: W-j' is non-local; W-j''
  unpriced. The ffi-pragmatist: indifferent, provided `spawn` and `wait` move the holder.
- **S11, a copy kept before the send.** The ffi-pragmatist vetoes S11 as written on 8 of 10 runs killed;
  the compiler-engineer's emulation stopped only the emptied cell. Settled by the critic's re-run (9 of
  10 killed without the check, 10 of 10 stopped with it).
- **Where the queue lives.** The ffi-pragmatist first recommended a platform member, then took the
  compiler-engineer's shape (the queue in the runtime, the layer telling it of the stop), *"his keeps the
  dependency's direction for every program"*. No disagreement remains.
- **S5, the queue's life.** The historian objects to a count of copies (Go traces, Rust splits the ends);
  the compiler-engineer: a handle's life does not avoid the count, since a queue reached from several
  threads must outlive every thread inside it. The critic: true of the queue's memory, silent on the leak,
  which split ends would close at the price of a third type constructor.

## The resolutions

### R1. The language route, adopted: values cross threads whole, through `spawn`, `wait` and a mailbox

**Adopted, provisional — author ratification pending.** `(thread T)` and `(mailbox T)` enter the
language as two type constructors (§ 3), `spawn`, `spawn_sized` and `mailbox` as built-ins, `wait`,
`send`, `receive` and `close` written only after a dot and taking no name, and **every value crosses a
thread whole, as a copy**. **The exact spec text the implementation lane is held to** is
`docs/panel/254-reports/resolution/spec-r.diff` against the frozen spec, made by `resolution/
make_resolution.py` from the spec-warden's `spec-w2.md` (sha256 `0c06f838c0841643...`) with four
asserted edits; `spec-r.md` sha256 `c21c162d2612592f...`; **9,677 vendored** (+655 on 9,022,
`heroes-next measure`); its real count is unmeasured (the four refreshes the spec-warden's brief allowed
were spent; `w`'s 12,548 real against its 9,557 vendored puts it near 12,700, an inference, the landing's
own refresh measures it), inside 16,384 with no raise.

````diff
--- spec-now.md
+++ spec-r.md
@@ -107,19 +107,23 @@
 | `()` | nothing: what a function with no `->` returns |
 | `ptr` `cstr` | an opaque pointer and a C string; `nullptr` is the null of both |
 | `(function(A) -> B)` | a function value, of any arity; the parentheses are mandatory |
+| `(thread T)` | a running thread whose function answers a `T`; a handle (section 13) with no null, which its `wait` ends |
+| `(mailbox T)` | a queue of `T`s, which every copy of it reaches (section 11) |
 
 - No implicit conversions, widths included: `1 + 2.0` is an error.
 - Every value behaves as an independent copy: after `b = a`, mutating `b`
-  never changes `a`. No aliasing exists among the values this language owns.
-  A `ptr` or a `cstr` is a copied ADDRESS, wherever it sits: two copies reach
-  one foreign thing, so a function taking one without `@` may still change, or
-  free, what C holds.
+  never changes `a`. No aliasing exists among the values this language owns
+  but a mailbox's queue.
+  A `ptr`, a `cstr` or a `(mailbox T)` is a copied ADDRESS, wherever it sits: two copies reach
+  one foreign thing, or one queue, so a function taking one without `@` may still change, or
+  free, what it reaches.
 - A record or variant holds its fields **by value**, so it may contain itself only
   through `[T]` or `{K: V}`: `children: [Node]` is a tree, `child: Node` has no size.
 
     Type     = Prefix { "[" integer "]" } [ "?" ] .
     Prefix   = ident [ "." ident ] | "[" Type "]" | "{" Type ":" Type "}"
-             | "(" ")" | "(" "function" TypeArgs "->" Type ")" .
+             | "(" ")" | "(" "function" TypeArgs "->" Type ")"
+             | "(" ( "thread" | "mailbox" ) Type ")" .
     TypeArgs = "(" [ TypeArg { "," TypeArg } ] ")" .
     TypeArg  = Type | ident ":" Type .
 
@@ -179,7 +183,7 @@
 `_ = e`, and not a `_` parameter written one. Answer the error. Both read the
 OUTERMOST type, so a `[T?]`, a record holding one, or a type parameter that
 arrived fallible is still dropped.
-Shadowing is a compile error: a `use` binds its name for the whole file, so nothing else in the file may take it.
+Shadowing is a compile error: a `use` binds its name for the whole file, so nothing else in the file may take it, and a built-in's name (section 11) is taken in every file, a field's or a case's aside.
 
     Block     = INDENT { Statement } DEDENT .
     Statement = ident Binding | Simple .
@@ -228,7 +232,7 @@
 → `<<` `>>` → `&` → `^` → `|` → comparisons → `&&` → `||`.
 
 `==` is structural equality on any two values of one type, recursively; a map's
-insertion order does not affect it. A `ptr`, a `cstr` and a function value compare
+insertion order does not affect it. A `ptr`, a `cstr`, a `(thread T)`, a `(mailbox T)` and a function value compare
 as an address and not as an identity — free one and the next allocation may reuse
 that address. `nan` equals nothing, itself included, so `x != x` asks whether it is one;
 ordering one aborts, in `< <= > >=` and in `sort`.
@@ -303,7 +307,8 @@
   the same two, `(function(from: str, to: str) -> ())`. The names are part of the
   type: the other order is another type, and a call through a value names each in
   its own position. A function that has such a pair names it as the type does.
-- `x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods, no
+- `x.f(y)` is sugar for `f(x, y)` (UFCS), except a thread's `wait` and a mailbox's `send`,
+  `receive` and `close` (section 11), written only after a dot and taking no name, though no function so named may take a thread or a mailbox first. There are no other methods, no
   inheritance, no overloading, no default values, no user variadics, no
   anonymous functions.
 - Mutable parameters are marked `@` in the signature and at the call site:
@@ -356,7 +361,7 @@
 Built-ins: `print(...)` · `len` · `push` · `xs.slice(from:, to:)` ·
 `chars` · `keys` · `join(xs, sep)` · `repeat(s, n)` (`n` a `u64`) · `sort` (ascending: a number by value, a `str` by bytes, `false` before `true`; never a type parameter) ·
 `to_f32` · `to_f64` · `to_str` · `to_i8` `to_i16` `to_i32` `to_i64` `to_u8` `to_u16`
-`to_u32` `to_u64` — and, written in Heroes, calling the function in order: `map` · `filter`, keeping what
+`to_u32` `to_u64` · `spawn` · `spawn_sized` · `mailbox` — and, written in Heroes, calling the function in order: `map` · `filter`, keeping what
 the function accepts · `fold` (its function names them `acc` then `item`) · `find` (the first it accepts, else `not_found`) ·
 `any` · `all` (with `find`, stopping at the first that decides) · `range`.
 `slice` and `range(from: a, to: b) -> [i64]` exclude `to`.
@@ -375,6 +380,7 @@
 str) -> ()?` (`write_failed`) · `args() -> [str]` (the arguments after the
 program name; one that is not UTF-8 aborts) · `args_checked() -> [str?]` (which
 does not) · `exit(code: i64)` (ends the program; a code outside 0 to 255 aborts, one written as a number is refused).
+Threads, also provided: `spawn(f, x)` runs `f(x)` on a thread of its own and gives a `(thread T)?`, `T` being `f`'s answer, failing `limit` where the system or the runtime has no thread to give, its message saying why; `spawn_sized(f, x, bytes: n)` does the same on a stack of at least `n` bytes, and an `n` below 0 aborts. `t.wait()` waits for the thread and gives its answer, and ends the thread's life as a call marked `consumes` ends a handle's (section 13). `mailbox()` gives a new, empty `(mailbox T)`, typed as an empty `[]` is (section 10): `m: (mailbox Job) = mailbox()`. `m.send(v)` adds a copy of `v` and never waits, failing `closed` once the mailbox is closed, or `stopped` once it has answered `stopped`, `v` then left as it was; `m.receive(within_ms: n)` takes the oldest value, waiting at most `n` milliseconds, `-1` for no limit and below that an abort; on an empty mailbox it fails `closed` once it is closed, `stopped` once a stop is asked (section 13) and from then on, and `timeout` else, and a value that arrives after a `timeout` is the next receive's. `m.close()` closes it for every copy, and closing it again does nothing; the values still in it are received, or dropped with its last copy, a handle among them told when `main` returns as one nobody consumes. Every value crosses a thread whole, as a copy, through `spawn`, `wait` and a mailbox, so threads share nothing but a mailbox, which crosses as itself whatever it carries; a `ptr` or a `cstr`, or a value holding one, never crosses, and a handle, or a value holding one, crosses only through a mailbox, written `m.send(@v)`, which empties it, its life going with it, or as the answer `wait` gives, and is then that thread's: reaching C through it on a thread that does not hold it aborts, naming the call. Anything else is a compile error, and so is a `(mailbox T)` whose `T` holds a `ptr`, a `cstr` or that mailbox's own type, in a field, an element, a payload, a thread's answer or another mailbox's `T`, so no queue ever holds itself, and a `(thread T)` or a `(mailbox T)` in an `extern` signature, a callback's parameter or a group's record.
 
 ## 12. Tests and holes
 ```
@@ -392,7 +398,7 @@
 with holes type-checks everything else but produces no binary.
 
 ## 13. FFI
-Anything beyond this document comes from packages or C libraries; threads, processes and files from the language's `hero_os.h`, and sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names one. A thread runs a function of one `i64` or one `str` and gives back one of the same, a `str` copied each way, and nothing else crosses: `hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` or `hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64` starts it, and `hero_thread_join(handle: i64) -> i64` or `hero_thread_join_str(handle: i64) -> str`, as it was started, waits for its answer. A group names its header, and `link` a library when the symbols need one; C
+Anything beyond this document comes from packages or C libraries; processes and files from the language's `hero_os.h`, and sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names one. A group names its header, and `link` a library when the symbols need one; C
 reads a module's headers in the order its groups are written: one that needs
 another's names comes after it, and one defining `_GNU_SOURCE` first. clang
 checks every result type, constant and record field against that header, and a result may be
@@ -419,7 +425,7 @@
 aborts, or fails the call where the kernel writes, and so past a local lent with
 `@`, which C receives itself; past a field or an element it is the binding's word.
 A callback is a **parameter**, never a result or a signal handler, nor called by one; its parameters follow the same rule
-and `()` is `void`: `atexit(f: (function() -> ())) -> i32`. No function of the program is a signal handler: after `hero_net_catch_stop() -> i64` (`hero_net.h`), Ctrl-C, SIGTERM and `hero_net_ask_stop() -> ()` only make `hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts any other signal, and `hero_signal_take(sig: i32) -> i64` gives the count since its last call
+and `()` is `void`: `atexit(f: (function() -> ())) -> i32`. No function of the program is a signal handler: after `hero_net_catch_stop() -> i64` (`hero_net.h`), Ctrl-C, SIGTERM and `hero_net_ask_stop() -> ()` only make `hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer, or a mailbox's `receive` that finds it empty, answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts any other signal, and `hero_signal_take(sig: i32) -> i64` gives the count since its last call
 
 A group's `record` is the header's struct: all its fields, and the same name
 unless the header writes it after the word struct, which `tag` gives:
@@ -464,7 +470,7 @@
 `x: cstr @= s.lease()` is a COPY of the bytes that C may read for as long as the
 program says, and `end_lease(@x)` frees it and empties the cell. A lend and a
 lease name stand only as an argument of a call, nothing else writes a lease's
-cell, and a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when
+cell, and a lease nobody ends, like a handle nobody consumes or a thread nobody waits for, aborts when
 `main` returns, saying how many. No word says C frees what it is handed; one
 that frees a lease kills the process, naming the leases live. Bytes C owns come
 from its own allocator, with their disposer.
````

**No veto stands against it.** The ffi-pragmatist's soundness veto on S11 *as written* is lifted by R3,
its own condition; the spec-warden's budget and Principle 0 vetoes yield to the rule of `691531908` and
are recorded as cost (+655 vendored, about +900 real, an inference); the compiler-engineer's *"core,
not sugar"* (313 `match` sites in 119 files, about 1,100 to 1,400 lines under `selfhost/` and 540 under
`runtime/`) likewise. **Panel 251's R3, its four reasons re-measured**: (1) and (2) yield to `691531908`; (3) the cost
is 289 in 110 for one `Ty` case, ten times the 30 R3 named, a cost and not a refusal under the rule; (4)
**7.13's heaps are kept**: the flat encoding is written on the sending thread into one block of the
runtime's own and rebuilt on the receiving thread into blocks it allocates, so every block a program's
value owns is born and freed on one thread, as `spawn_str.c` already makes a `str`; only the runtime's
transport block and the queue object cross, as `spawn_str.c`'s box and the layer's TLS state already do
(the compiler-engineer's Q2.2). The descriptor keeps its five members and the ABI does not move.

### R2. The rows, as the lane is held to them

| row | adopted |
|---|---|
| S1 `spawn(f, x)` | `(thread T)?`, failing `limit` where the system **or the runtime's table of 256** has no thread to give, its message saying why (the system's own number kept, which `selfhost/main.hero:107-110` prints) |
| S2 `spawn_sized` | the same on at least `n` bytes; `n` below 0 aborts |
| S3 `t.wait()` | gives the answer and ends the thread's life as a `consumes` call ends a handle's: a compile error where every path waited, else an abort; a thread is a handle **with no null** (W-a); a thread nobody waits for aborts when `main` returns, saying how many |
| S4 `mailbox()` | typed as an empty `[]` is (W-e), so `Req(reply: mailbox())` reads |
| S5 the life | **a value whose copies reach one queue**, counted atomically, given back with its last copy and the values in it; § 3's no-aliasing sentence gains *"but a mailbox's queue"* (W-b); **no queue holds itself** (R5) |
| S6 `m.send(v)` | never waits (unbounded); fails `closed` once closed, or `stopped` once the mailbox has answered `stopped`; **`v` then left as it was** (the emitter empties `v` only on success) |
| S7 `m.receive(within_ms: n)` | the oldest value; `-1` no limit, below that an abort; **on an empty mailbox**: `closed` once closed, **`stopped` once a stop is asked, and from then on**, `timeout` else; a value arriving after a `timeout` is the next receive's (R4) |
| S8 `m.close()` | for every copy; again, nothing; the values still in it received, or dropped with its last copy, a handle among them told at `main`'s end as one nobody consumes |
| S9 what crosses | every value whole, function values included (a top-level function is a bare C pointer, `heroes_runtime.h:659-661`); a mailbox crosses as itself whatever it carries |
| S10 never crosses | a `ptr` or a `cstr`, or a value holding one: a compile error |
| S11 a handle | crosses only through a mailbox, `m.send(@v)`, or as `wait`'s answer, **and is then the receiving thread's: reaching C through it on a thread that does not hold it aborts, naming the call** (R3); `spawn(f, x)` with a handle in `x` is a compile error (W-j) |
| S12 the stop | the layer's stop reaches the queues; a mailbox's `receive` **that finds it empty** answers `stopped`; the runtime never calls `hero_net.h` (R6) |
| S13 the names | a built-in's name is taken in every file, a field's or a case's aside (W-c, in § 5); `wait`, `send`, `receive`, `close` never enter the built-in table and take no name (W-d), **though no function so named may take a thread or a mailbox first** (W-h); § 11's names line removed (W-g) |
| S14 the doors | leave the spec's text; leave `hero_os.h` in the order of R7 |
| boundary | **a `(thread T)` or a `(mailbox T)` never reaches C**: in an `extern` signature, a callback's parameter or a group's record a compile error (`crosses_the_boundary`'s arm, `selfhost/check/ffi.hero:79-95`) |

### R3. A handle belongs to the thread that holds it: the runtime's holder check

**Adopted, the ffi-pragmatist's repair, phrased by thread after the critic's reading.** Each handle's
life records the thread holding it, by **the runtime's own thread number** (never a `pthread_t`, which
Darwin reuses: the critic's `deadthread`); the check sits in `hero_handle_alive_at`, which the emitter
already writes before every C call taking a handle (`selfhost/emit/handle_before.hero`, its head comment),
so it covers a handle reached inside a record or a list too. **The hand-over is made by the per-type
crossing walk for every handle a value holds, on both sides**: at `send` once the send has succeeded (so
S6's *"left as it was"* holds), at `receive`, and at `wait`'s answer. Measured: 9 of 10 runs killed
without it, 10 of 10 stopped by name with it, 10 of 10 clean moves; the `runtime` suite on it 10 passed,
0 failed; about 3.6 ns per handle argument (the ffi-pragmatist). **What it is**: a thread check, so a
handle sent back makes old copies on its first thread valid again, which is sound; the text says *"on a
thread that does not hold it"*, never *"a copy aborts"*. **Five runtime messages are owed**, each
blocking-class by CLAUDE.md § 8: the holder message where it is false (the critic's `deadthread`,
`spawnforgot`, `waitforgot`), the dead value after a send naming *"`consumes` or `transfers`"*, and a
handle dropped with a queue never naming the mailbox (the compiler-engineer's Q2.5).

### R4. The stop: a drain, then `stopped` for ever

**Adopted, the critic's behaviour 3**: after a stop, a `receive` still gives what is queued; on an empty
mailbox it answers `stopped`, and that mailbox answers `stopped` from then on, a later `send` to it
failing `stopped` with `v` left as it was. It strands no handed connection (stop-wins stranded 24 of 32)
and reads no stale reply (the drain read a count never applied in 6 of 6 runs with a reused reply
mailbox), measured on the compiler-engineer's gateway in 12 plain runs and 12 under TSan and ASan+UBSan,
0 reports (`critic-reports.md` finding 6b). **Its condition**: it is an emulation on one Mac; **the
implementation lane runs the compiler-engineer's four stress topologies on it before it is held to it,
and the CI's Linux legs after**; should either fail, the drain with the stale-reply sentence is the
fallback, and the record says which ran. `closed` comes before `stopped` (finding 8). A `stopped` receive
says nothing of whether the request it waited on was applied. The hazard no stop touches, a reply that
arrives after a `timeout` and is read by the next request (the compiler-engineer's 72 and 76 clients),
is said in the text; a reply mailbox per request avoids it.

### R5. No queue holds itself: W-f, and a gate behind it

**Adopted**: a compile error for a `(mailbox T)` whose `T` reaches that mailbox's own type through a
field, an element, a payload, **a thread's answer** (the critic's finding 11) or another mailbox's `T`,
decided at record and variant declarations (about 50 checker lines, one diagnostic class); and **the
compiler-engineer's gate at `main`'s end**, which tells a queue still alive once every thread was waited
for (built: `self` and `pair` exit 134 with *"1 mailbox(es) still hold values when main returned"*, where
today's gate is silent, exit 0). design.md §4.10's *"Reference cycles become impossible"*
(`docs/design.md:1827-1830`) stays true by the rule, and the gate tells a hole in it. **What it refuses**:
a message type that carries its own mailbox type (a hub's `Event` carrying `(mailbox Event)` splits into
`Command` and `Event`; the spec-warden's L4 predicts none of the four applications refused); a network of
peers passing each other's addresses of the same message type cannot split, which **split ends**
(Rust's `mpsc`, a copyable send end and a receiving end that is a handle) would allow at the price of a
third type constructor, **unpriced**, recorded for the author.

### R6. Where the queue lives: the runtime, told of the stop by the layer

**Adopted, the compiler-engineer's shape, which the ffi-pragmatist took**: the queue is a part of the
runtime (`parts/mail.c`), **the runtime never calls the platform layer**, and `hero_net.c` tells the
queues of a stop by one line at the end of `hero_net_nudge_all` (`runtime/platform/hero_net.c:874-882`).
A `print(1)` program gains 25 symbols and 4,080 bytes and links no `hero_net` code; a program that asks
the stop links the member as before (`nm`, both seats). The wake measured 0.017 to 0.048 ms. **A rule the
landing writes down** (the critic's finding 7): no queue operation ever takes the layer's lock, the order
being the layer's lock, then the queues' list lock, then a queue's. A program that links no layer member
cannot be stopped, so its `receive` never answers `stopped`. The three statics owe `SHARED_BY_DECISION`
entries (`tests/harness/suite_runtime.hero:227`; the prototype read 9 and 1 without them).

### R7. The thread doors leave in four steps

**Adopted** (S14; the critic's finding 19): the doors leave the spec's text with R1; they leave
`hero_os.h` and the runtime's exports only after (1) a seed refresh that knows `spawn_sized`, (2)
`selfhost/main.hero:101-113` moved to it, keeping its exit-2 message with the system's number, (3) a
second seed refresh that names no door (`grep -c hero_thread_spawn seed/heroes.c`: 12 today), and (4)
every one of the 69 files that declare a door moved (5 of them the compiler's and the net's). A program
still binding one is then told `error[ffi_unknown_name]` (the spec-warden's probe), no new class.

### R8. `threads.ledger` and `threads.hero`: the coordinator's reading, from the seats' measurements

No seat ruled on the ledger; this is the coordinator's reading, for the author. **`thread/shared-state`
is amended**: *channels* leave its description, since a mailbox is now the language's (a new capability,
`thread/mailbox`, *"values sent to a running thread and received from it, copied whole"*, `heroes` the
language's `(mailbox T)`, beside Go's `chan`, Rust's `mpsc` and Python's `queue.Queue`); its refusal of
locks, conditions, once-only values and memory several threads reach **stands**, its reason re-worded
*"threads share nothing but a mailbox's queue, whose values cross as copies, so no value is held by two
threads and nothing needs a lock"* (true by R1 and R3). **`thread/cancel`'s premise is false under R1**
(*"nothing crosses into a running thread"*): re-worded so that a running thread is asked to stop by a
value on a mailbox it receives from, or by the program's stop, which ends its waits `stopped`; a forced
stop stays refused. `threads.hero`'s module comment (lines 5-11, *"no lock, channel or shared memory
here"*) is rewritten, and Rebel's `threads` is rewritten over the built-ins, its `spawn` and
`spawn_sized` (`threads.hero:61`, `:77`) and `mail/internal/address.hero:141`'s `mailbox` renamed, since
a built-in's name is taken everywhere.

### R9. The blind seat is owed, and a fourth arm with it

The registered run (26 sessions, arms `now`, `m`, `m0`) is owed, started by the session that holds the
author's word (`254-reports/llm-ergonomist.md`), scoring the spec-warden's W1 to W11 as registered at
02:11 (`warden-at-start.md`, sha256 `fd1418d7...`). **The resolution's text is a fourth arm whose
predictions are registered before any session of it runs**, t2's program 3 row re-registered for it as
`got queued`. A reading that shows a row of R2 read wrong by two or more of five readers reopens that
row's text (the spec-warden's condition).

### What conservative would have been, and what robust alternatives were not adopted

- **Route A, the library route** (no spec token): a platform member `hero_mail.c` (525 lines, header 107,
  tag 15, the last of four bits), `hero_stream_number`/`of` (+90 lines), a Rebel face (about 150 lines and
  13 binding edits in 6 files), **built by the ffi-pragmatist and measured**: the gateway shape with TLS
  accepted on one thread and served on 16, 20/20 plain, 5/5 ASan, 5/5 TSan, 11.1 times one thread. It
  fixes feature 2647 on today's language; every richer value is serialised into text, which t1 exists to
  measure (unrun).
- **Route A0**, Rebel's `ipc` channel by name: runs t3's shape (the critic 5/5 and 3/3; the
  ffi-pragmatist 20/5/5 with a keeper thread), joins no `poll` set on this Mac, and carries no TLS
  connection: the owner half of 2647 alone.
- **W-j'** (`spawn(f, @x)`, `f` owning it): non-local; **W-j''** (`spawn(serve, @c)`, `function
  serve(@c: Conn)`): local, unpriced, the convenient form if the author wants one, to be priced first.
- **Split ends**, the stop's **behaviour 2 with a deadlock report** (exact, hangs on a forgotten close),
  **a bounded send that fails `full`**, **a `wait` with a limit**, **a one-shot reply**, and **a mailbox in a
  `poll` set** (chat's and notes' loops): each named by a seat or the critic, none priced, none adopted;
  each a question for a later sitting, not a refusal.

## The prior art, per resolution

Re-read 2026-11-22 by this session, each page's own words checked against the historian's quotation
(`254-reports/historian.md`, which read them the same day):

- **R4**: Python's `queue.Queue.shutdown` (a drain by default, *"Once the queue is empty, future calls to
  get() will raise ShutDown"*); Rust's `Receiver::recv` (*"messages sent before the disconnect will still be
  properly received"*). These go into the decision issue.
- **R2's S6**: Rust's `Sender::send` (*"returning it back if it could not be sent"*). **S8**: Kotlin's
  `SendChannel.close` (*"If the channel was already closed, this function does nothing"*). **S1**: Rust's
  `Builder::spawn` (*"yields an io::Result to capture any failure to create the thread at the OS
  level"*). **S10**: the Rustonomicon (*"raw pointers are neither Send nor Sync"*). **R1's copying**: Dart's
  `SendPort.send` (*"all other objects will be copied"*). **R3's move**: MDN's transferable objects
  (*"Following a transfer, the original object is no longer usable"*). These go into the decision issue.
- **R5, R6, R7, R8, R9**: none.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| spec-warden | W1 to W11 on the blind counts (registered 02:11) | the blind run |
| spec-warden | W12 real `m` +610 (held, +666), W13 `m0` +245 (held, +283), W14 `now` 11,788 (hit) | scored here |
| spec-warden | L1 the landed `w2` refreshes to 12,560 real (12,553 to 12,575); L2 `spec` 0 failed only with three inventory rows and the four pins; L3 `grammar` 0 failed with `thread`, `mailbox` as `WORD_` constants; L4 W-f refuses 0 tracked programs and 0 of the four applications | the lane's gate |
| compiler-engineer | the first lane commit with the two `Ty` cases reads 280 to 300 `non_exhaustive` in 105 to 115 files; the landing adds at least 1,100 lines under `selfhost/` and 450 under `runtime/`; the `runtime` suite 0 failed only with three `SHARED_BY_DECISION` entries | the lane's commits and gate |
| ffi-pragmatist | P1 TLS by `m.send(@conn)` 20/20 and 5/5 with no edit to the 13 bindings; P2 the four live `close` bindings build unchanged; P3 a `Db` sent without `@` refused at compile time; P4 `print(1)` has 0 `_hero_net_*`; P5 the TLS hand-over on Linux 20/20 and 5/5 TSan; P6 a `(mailbox T)` in an `extern` refused `ffi_type`; P7 `keep = db; m.send(@db)` builds and, without R3, dies in at least 7 of 10 runs | the lane's gate; the CI's Linux leg |
| historian | P2 on t2, a send after the close predicted to abort more often than `closed` on arm `m`; P3 no seat finds a shipped system with a process-wide permanent stop over queued replies (**held**: no report names one) | the blind run; scored here |

## What this sitting files and owes

- **An `adjacent` defect**, found by the ffi-pragmatist and reproduced by the critic 3 of 3: `hero_ipc`'s
  lock holder compared by `pthread_equal` after its thread ended (`runtime/platform/hero_ipc.c:2897-2916`),
  so a new thread that Darwin gives the dead holder's `pthread_t` releases, or is told it holds, a lock
  it never took; the program is `p254-scratch/ffi-pragmatist/p254/c/ipclock_alive.c`; the repair, the
  runtime's own thread number as R3 uses. **Unfiled**: the coordinator files it.
- **The blind run and its fourth arm** (R9), started by the session holding the author's word.
- **The decision issue**, drafted at `.claude/worktrees/rebel-scratch/coord/issues/p254-decision.md`,
  unfiled.

## What the implementation lane builds (`lane-mailbox`, held to R1 to R8)

1. **The spec**: `resolution/spec-r.diff`, refreshed at landing (`--refresh`), its commit body saying what
   paid (+655 vendored against `DELTA_GATE` 50: the rule of `691531908`, and § 13's doors sentence, -143
   vendored); `inventory.hero` three rows appended; the four pins moved.
2. **The parser and the surface walk**: `thread` and `mailbox` as `WORD_` constants of `keywords.hero`
   read after a type's `(`, two `TypeKind` cases (24 sites in 12 files), the formatter's arm, `heroes
   grammar`, the editors' grammar (two words), the formatter's strided probe after the push.
3. **The checker**: two `Ty` cases (289 sites in 110 files, each a decision written, never a catch-all);
   `spawn`, `spawn_sized`, `mailbox` typed (by context as an empty `[]`); the four operations in §4.11's
   first lookup (about 6 resolver and 15 dispatch lines, about 90 for the operations); S10, W-f and W-h
   as three new diagnostic classes on `check/reaches.hero`'s walk, each with a `tests/golden/check/` case
   annotated in its source; S11's `@` by `marker_mismatch`; `v` after `m.send(@v)` typed as the `when`
   precedent types a conditional end (priced by nobody: the lane prices it); the boundary arm in
   `crosses_the_boundary`.
4. **The emitter**: a put and a take per crossed type beside `emit/sides.hero`'s shape (about 300 lines),
   the hand-over calls of R3 at `send`, `receive`, `spawn` and `wait`, the cell emptied only on a
   successful send.
5. **The runtime**: `parts/mail.c` (the queue, the count, the flat encoding's walk, the stop's
   behaviour 3, the gate of R5), the holder check of R3 in `parts/alloc.c` with its five messages, S1's
   `limit` for the table of 256 with the system's reason, the one line in `hero_net_nudge_all`, the Windows
   arm (written by both seats, never compiled: owed to the CI's Windows leg or the box), three
   `SHARED_BY_DECISION` entries.
6. **The measurements it owes before it is held to R4**: the compiler-engineer's stress topologies on
   behaviour 3; then the CI's Linux legs (Docker did not answer on 2026-11-22: `docker info`).
7. **Rebel**: `threads` rewritten over the built-ins, its ledger per R8, the names of R8 renamed; the four
   applications (gateway, registry, chat, notes) rewritten to serve connections on threads, the TLS
   hand-over by `m.send(@conn)` (the ffi-pragmatist's P1).
8. **The doors' four steps** of R7, in order, the seed refreshes in the rhythm `.claude/rules/
   verification.md` § The batch sets.

## How the sitting went, for the next

- **Three lanes in one night**: convened as the soundness lane, widened twice and made a full panel at
  the author's word; the briefs were rewritten for the full panel before any seat started, and the
  critic's first reading changed the proposal's own text.
- **The 5-hour limit stopped two seats** (the compiler-engineer and the ffi-pragmatist) mid-work; both
  resumed from their own context with their reports intact, written as they went.
- **The blind seat's paid run** was left to the session holding the author's word (§ R9); its free
  preflight ran clean with the four repairs of defect 2589 and `tools/reads.py`.
- **A premise of the coordinator's was false**: *defect 2588 says it is red at base* (the suite read 10 and
  0 at the frozen commit). And the coordinator's first S5 and S11 texts admitted a silent leak and a
  two-thread handle, both found by the critic before any seat ran.

## The blind seat's run, 2026-11-22

Added 2026-11-22 by the session that scored the blind seat, started by the coordinator of M-rebel-library;
every number below is a command's, each named in `254-reports/llm-ergonomist-blind.md`, the seat's report of
the run. **Corrected here, underneath, the lines above that this run falsifies**: the head's *"prepared and
**not run**"*, the status's *"whose registered run has not been made"*, the verdicts table's *"**not run**"*
and *"0.000000 USD spent"*, R9's *"is owed"*, § What this sitting files and owes on *"The blind run and its
fourth arm (R9)"* and on two items *"unfiled"*, and § How the sitting went on the paid run *"left to the
session holding the author's word"*: it was run, as follows.

**The registered run.** 26 sessions (arms `now`, `m`, `m0`, drawn `q`, `y`, `p`), 09:27:34 to 10:03:16, from
the session holding the author's word, by the registered `run.sh`: 26 of 26 a success by the three fields,
`claude-opus-5-5` alone, **6.445551 USD**; `scan.py` over 27 transcripts, prompt-alone=NO 0, git-status=LEAK
0, claude-md-or-memory=YES 0; `reads.py`, run after, 0 sessions with a path outside their folder; **0
voided**. The seats had no compiler, so a session's *accepted* is its own reading; t1 `now` was also built
and run by a stage 3 at the frozen commit's `sources 6e891e681c14bca0`, five runs and four substitutions
each (a `|`, a newline, every escape, a name holding a newline).

**R9's fourth arm had not run** (`order.txt` and `key.txt` name three arms) **and ran** 10:45:51 to 11:04:36
from the scoring session, within the 5.00 USD its brief allowed: the resolution's text, `spec-r.md` (sha256
`c21c162d2612592f...`, made by `patch` from `resolution/spec-r.diff` and by `make_resolution.py` to the same
bytes), t1, t2 and t3 five times each; **its predictions registered at 10:45:21, before any folder of it
existed** (`254-briefs/blind/arm-r-registration.md`, sha256 `dda5c922fe4d85aa...`), t2's program 3 row
re-registered as `got queued`. 15 of 15 a success, **3.370710 USD**, `scan.py` over 16 transcripts 0, 0, 0,
`reads.py` 0, **0 voided**. **In all 9.816261 USD** of the author's 20 for this seat.

**The counts, right of 5.** t1 `now` 5 by the reader's rows and 5 mechanically; t1 `m` 5; t2 `m` 5 on each of
the five programs; t2 `m0` p1 0, p2 0, p3 5, p4 0, p5 4; t3 `m` 5. Arm `r`: t1 5, t2 5 on each program
(program 3 `got queued`, 5 of 5), t3 5. **D1 does not hold** (E1 = 5 - 5 = 0: every reader of `now` framed
the text by its length both ways, and every one ran right under the four substitutions); **D2 holds** (d of
5 on p1, p2 and p4) and `m`'s text suffices for all five programs; **D3 holds**. On `r` the same: D1 does not
hold, D2 and D3 hold.

**The predictions this run settles.**

| prediction | registered | counted | score | deciding sessions |
|---|---|---|---|---|
| spec-warden W1, t1 `now`, reader's rows | 2 (1 to 3) | 5 | miss | `t1-q-1` to `t1-q-5` |
| W2, t1 `now`, mechanical | 2 (1 to 3) | 5 | miss | `t1-q-1` to `t1-q-5` |
| W3, t1 `m` | 4 (4 to 5) | 5 | miss on the point, inside its range | `t1-y-1` to `t1-y-5` |
| W4, D1 does not hold, E1 = 2 | | does not hold, E1 = 0 | hit on the rule, its E1 missed | `t1-q-*`, `t1-y-*` |
| W5, t2 `m` | 5, 5, 4, 4, 2 | 5, 5, 5, 5, 5 | miss (p5 outside 1 to 3) | `t2-y-1` to `t2-y-5` |
| W6, t2 `m0` | 0, 0, 2, 0, 0 | 0, 0, 5, 0, 4 | miss (p3 and p5 outside their ranges) | `t2-p-1` to `t2-p-5` |
| W7, D2 holds on p1, p2, p4; the text not enough for p5 | | holds on p1, p2, p4; enough for all five | hit on the rule; miss on p5 | `t2-y-*`, `t2-p-*` |
| W8, t3 `m` | 4 (3 to 5) | 5 | miss on the point, inside its range | `t3-y-1` to `t3-y-5` |
| W9, D3 holds | | holds | hit | `t3-y-*` |
| W10, 0 voided | | 0 | hit | all 26 |
| W11, the verdict approves | | approve | hit | `verdict` |
| historian P2, on `m` an abort after the close more often than `closed` | | `closed` 5, abort 0 | **miss** | `t2-y-1` to `t2-y-5` |
| the seat's own, under the resolution program 3 prints `got queued` | | 5 of 5 | **hit** | `t2-w-1` to `t2-w-5` |
| fourth arm R-1 to R-9 | 9 predictions | | 6 hit (R-1, R-3, R-4, R-5, R-7, R-9); 3 miss on a point inside its range (R-2's p3 and p5 at 5, not 4; R-6 at 0, not 1; R-8 at 3.370710, not 3.40) | the 15 |

Of the warden's eleven: three hits (W9, W10, W11), two hits on a rule with its number missed (W4, W7), two
misses inside their ranges (W3, W8), four outside (W1, W2, W5, W6); every count of right sessions it missed,
it missed low.

**R9's condition, per R2 row: no row of R2 is reopened.** No row was read wrong by two of five readers, nor
by one, on arm `r` (R2's own text) or on arm `m` where its text is R2's: S1, S3, S4, S5, S6, S7, S8, S9, S11,
S12, S13 and R5 were each read right by every session that read them. S7 with S12, the row most at risk,
read 5 of 5 on `r` as R4's drain (`got queued`, each session naming *"on an empty mailbox"* or *"finds it
empty"*); S11's program 5 ended at `free(b)` for the emptied handle in 5 of 5, three citing R3's *"a thread
that does not hold it"* beside it and none alone. Read by no task: S2, S10, the boundary row, and parts of
S4 (a `mailbox()` typed by a constructor's field), S5 (the count, the leak), S6 (*"`v` then left as it
was"*), S7 (the value after a `timeout`), S12 (a send after `stopped`) and S13 (W-h); R9's condition says
nothing of those.

**The verdict's findings that bear on the resolution.** The A/B verdict approves (W11). **It judged the
proposal, not the resolution**: its `changes.diff` equals `254-briefs/arms/spec-m.diff` below the two header
lines and differs from `resolution/spec-r.diff` (`diff`, run by the scoring session); no A/B verdict was run
on `spec-r`, R9 asking an arm. Its choice point under spec-B, in its words: *"The spec doesn't say what
copying a `(thread T)` means; the diff adds `mailbox` to the "copied ADDRESS" list but not `thread`."* **So
what copying a `(thread T)` means, a `for` loop variable or a thread held in an array, is unsaid**, and the
resolution leaves it so: `spec-r.md`'s copied-ADDRESS sentence names `ptr`, `cstr` and `(mailbox T)` (its
line 117), and R2's thread row says *"a handle (section 13) with no null, which its `wait` ends"*. The
verdict's own spec-B program holds its threads in a `[(thread Tally)]` and waits through `for t in threads`;
every graded t1 and t3 session, 10 of 10 on `m` and 10 of 10 on `r`, kept its threads in named bindings
and named the question as the reason, so no graded program depends on an answer, and R9's condition, which
counts readings, does not reach it. Its conditions name what would move it: *"Toward object: if a build
showed that spec-B refuses `t.wait()` on a `for` loop variable, or a `(thread T)` inside an array, without
spec-B saying so"*, and *"if waiting on a copy of an array entry stopped with an error at run time"*; the
implementation lane meets that question first, when its checker decides what a `[(thread T)]` and a `for`
over one are. A search of this sitting, its briefs and its reports for an array, a list or a copy of a
`(thread T)` found only C handles copied across a send (R3); a search with those words, a question for the
author and the lane rather than a premise. Its veto condition (*"if a task using `mailbox` showed that the
rule for when `receive` gives `stopped` can't be followed from its line"*) is not met: `m`'s stop was read
as written 5 of 5 (`receive: stopped`), R4's drain 5 of 5 (`got queued`).

**The two items unfiled above** were filed the same day by the coordinator of M-rebel-library, read on
GitHub by the scoring session: `hero_ipc`'s lock holder as **defect 2673** (open, `class:adjacent`), and
the decision as **issue 2674**, *"Panel 254: ... R1 to R9, provisional"* (open, `kind:decision`).

## Author's verdict

Pending.

**Ratified by the author's reading, 2026-11-22, late morning**, in conversation (the answer *1a* to the
coordinator's recommendation, meant as: *ratify R1 to R9 now; the implementation lane merges only once the
blind seat's scoring shows no row of R2 read wrong by two of five readers, and a row it reopens comes back
alone*). The scoring above (§ The blind seat's run) reopened no row, so `lane-mailbox` merges under R1 to
R8. Beside it, the same day: the lane's sentence in spec § 11 on what a copy of a `(thread T)` is, written
after the verdict named that silence and held by one check case and two run cases (`threads-a-copy-of-a-thread-is-that-one-thread`,
`threads-a-thread-in-a-list-waited-for-again-aborts`), which no arm of the run read; and the register of
the decisions M-rebel-library invalidates (issue 2610) gains decision 2498 and panel 251's R3, a thread
exchanging one `i64` or one `str` at its start and end, invalidated by R1.
