# Panel 254's arms: the frozen spec (`now`), this sitting's proposal (`m`, panel 251's spelling M with
# every semantic the implementation lane is held to), and panel 251's own text ported to today's spec
# (`m0`). Each edit is asserted to match exactly once in the frozen spec before it is made.
# usage: python3 -I make_arms.py <frozen spec> <out dir>
import difflib, hashlib, os, sys

src, out = sys.argv[1], sys.argv[2]
now = open(src, encoding='utf-8').read()
os.makedirs(out, exist_ok=True)

FN_ROW = "| `(function(A) -> B)` | a function value, of any arity; the parentheses are mandatory |\n"
ADDR_OLD = ("  A `ptr` or a `cstr` is a copied ADDRESS, wherever it sits: two copies reach\n"
            "  one foreign thing, so a function taking one without `@` may still change, or\n"
            "  free, what C holds.\n")
ADDR_NEW = ("  A `ptr`, a `cstr` or a `(mailbox T)` is a copied ADDRESS, wherever it sits: two copies reach\n"
            "  one foreign thing, or one queue, so a function taking one without `@` may still change, or\n"
            "  free, what it reaches.\n")
PREFIX_OLD = '             | "(" ")" | "(" "function" TypeArgs "->" Type ")" .\n'
PREFIX_NEW = ('             | "(" ")" | "(" "function" TypeArgs "->" Type ")"\n'
              '             | "(" ( "thread" | "mailbox" ) Type ")" .\n')
UFCS_OLD = "- `x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods, no\n"
BUILTINS_OLD = "`to_u32` `to_u64` — and, written in Heroes, calling the function in order: `map` · `filter`, keeping what\n"
BUILTINS_NEW = "`to_u32` `to_u64` · `spawn` · `spawn_sized` · `mailbox` — and, written in Heroes, calling the function in order: `map` · `filter`, keeping what\n"
EXIT_END = "does not) · `exit(code: i64)` (ends the program; a code outside 0 to 255 aborts, one written as a number is refused).\n"
S13_THREADS_OLD = ("Anything beyond this document comes from packages or C libraries; threads, processes and files from the language's `hero_os.h`, "
                   "and sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names one. "
                   "A thread runs a function of one `i64` or one `str` and gives back one of the same, a `str` copied each way, and nothing else crosses: "
                   "`hero_thread_spawn(body: (function(i64) -> i64), arg: i64) -> i64` or `hero_thread_spawn_str(body: (function(str) -> str), arg: str) -> i64` starts it, "
                   "and `hero_thread_join(handle: i64) -> i64` or `hero_thread_join_str(handle: i64) -> str`, as it was started, waits for its answer. ")
S13_THREADS_NEW = ("Anything beyond this document comes from packages or C libraries; processes and files from the language's `hero_os.h`, "
                   "and sockets and clocks from its platform layer's `hero_net.h` and `hero_time.h`, linked where a group names one. ")
STOP_OLD = "only make `hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer answer stopped;"
STOP_NEW = "only make `hero_net_stop_asked() -> i64` answer 1 and a waiting call of that layer, or a mailbox's `receive`, answer stopped;"
LEASE_OLD = "cell, and a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when\n"
LEASE_NEW = "cell, and a lease nobody ends, like a handle nobody consumes or a thread nobody waits for, aborts when\n"

# This sitting's proposal: the rows, the UFCS sentence made true of itself, and one paragraph of § 11.
M_ROWS = ("| `(thread T)` | a running thread whose function answers a `T`; a handle (section 13) its wait consumes |\n"
          "| `(mailbox T)` | a queue of `T`s, which every copy of it reaches (section 11) |\n")
M_UFCS = ("- `x.f(y)` is sugar for `f(x, y)` (UFCS), but on a `(thread T)` or a `(mailbox T)`, whose\n"
          "  operations (section 11) are written only after a dot. There are no other methods, no\n")
M_PARA = ("Threads, also provided: `spawn(f, x)` runs `f(x)` on a thread of its own and gives a `(thread T)?`, `T` being `f`'s answer, "
          "failing `limit` where the system refuses a thread; `spawn_sized(f, x, bytes: n)` asks a stack of at least `n` bytes, and an `n` below 0 aborts. "
          "`t.wait()` waits for the thread and gives its answer; a thread waited for twice aborts. "
          "`mailbox()` gives a new, empty `(mailbox T)`, `T` taken from where it is used. "
          "`m.send(v)` adds a copy of `v` and never waits, failing `closed` once the mailbox is closed; "
          "`m.receive(within_ms: n)` takes the oldest value, waiting at most `n` milliseconds, `-1` for no limit and below that an abort, "
          "and fails `timeout`, `closed` once it is closed and empty, or `stopped` once a stop is asked (section 13), even with values in it. "
          "`m.close()` closes it for every copy, and closing it again does nothing; the values still in it are received, or given back with its last copy. "
          "Every value crosses a thread whole, as a copy, through `spawn`, `wait` and a mailbox, so threads share nothing but a mailbox; "
          "a `ptr` or a `cstr`, or a value holding one, never crosses, and a handle, or a value holding one, crosses only through `spawn`, `wait`, "
          "or a `send` written `m.send(@v)`, which empties `v`, its life going with it.\n")
M_NAMES = ("The names `spawn`, `spawn_sized` and `mailbox` are taken; `wait`, `send`, `receive` and `close` are written only after a dot, "
           "and stay free for the program.\n")

# Panel 251's text (its `bw/arms2/proposal-m.diff`), ported.
M0_ROWS = ("| `(thread T)` | a running thread whose function answers a `T`; a handle (section 13) its wait consumes |\n"
           "| `(mailbox T)` | a queue of `T`s; a handle (section 13) whose copies reach one queue |\n")
M0_UFCS = ("- `x.f(y)` is sugar for `f(x, y)` (UFCS). There are no methods but a thread's and a mailbox's\n"
           "  (section 11), no\n")
M0_PARA = ("`spawn(f, x)` runs `f(x)` on a thread of its own and gives a `(thread T)`, `T` being `f`'s answer, which `t.wait()` waits for and gives; "
           "`spawn_sized(f, x, bytes: n)` gives the thread a stack of at least `n` bytes. "
           "`mailbox()` begins a mailbox's life, which `m.close()` ends, as a handle's (section 13); "
           "`m.send(v)` adds to it and `m.receive(within_ms:)` takes the oldest, failing `timeout`, or `stopped` once a stop is asked (section 13). "
           "Every value crosses whole, as a copy, so threads share nothing but a mailbox; a `ptr` or `cstr` never crosses, "
           "and another handle only into `spawn` or a send, its life ending in the sender.\n"
           "A thread's and a mailbox's operations are written only after a dot, and their names stay free for the program.\n")


def edit(text, old, new):
    assert text.count(old) == 1, ('not exactly once', old[:80], text.count(old))
    return text.replace(old, new)


def arm(rows, addr, ufcs, para, stop):
    t = now
    t = edit(t, FN_ROW, FN_ROW + rows)
    t = edit(t, ADDR_OLD, addr)
    t = edit(t, PREFIX_OLD, PREFIX_NEW)
    t = edit(t, UFCS_OLD, ufcs)
    t = edit(t, BUILTINS_OLD, BUILTINS_NEW)
    t = edit(t, EXIT_END, EXIT_END + para)
    t = edit(t, S13_THREADS_OLD, S13_THREADS_NEW)
    if stop:
        t = edit(t, STOP_OLD, STOP_NEW)
    t = edit(t, LEASE_OLD, LEASE_NEW)
    return t


arms = {
    'now': now,
    'm': arm(M_ROWS, ADDR_NEW, M_UFCS, M_PARA + M_NAMES, True),
    'm0': arm(M0_ROWS, ADDR_NEW, M0_UFCS, M0_PARA, False),
}
for name, text in arms.items():
    p = os.path.join(out, f'spec-{name}.md')
    open(p, 'w', encoding='utf-8').write(text)
    print(name, hashlib.sha256(text.encode()).hexdigest(), len(text.splitlines()), 'lines')
    if name != 'now':
        d = ''.join(difflib.unified_diff(now.splitlines(True), text.splitlines(True),
                                         fromfile='spec-now.md', tofile=f'spec-{name}.md'))
        open(os.path.join(out, f'spec-{name}.diff'), 'w', encoding='utf-8').write(d)
