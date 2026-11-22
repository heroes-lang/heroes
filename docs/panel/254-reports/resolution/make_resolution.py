# Panel 254's resolution text: the spec-warden's `spec-w2.md` (its W-a to W-j, `254-reports/spec-warden.md`
# § 2) with the amendments the seats' and the critic's measurements carry, each edit asserted to match once.
# usage: python3 -I make_resolution.py <spec-w2.md> <spec-now.md> <out dir>
import difflib, hashlib, os, sys
w2, now, out = sys.argv[1], sys.argv[2], sys.argv[3]
t = open(w2, encoding='utf-8').read()
EDITS = [
    # S1: the runtime's table of 256 fails `limit` too (the compiler-engineer's and the ffi-pragmatist's
    # amendment; the 257th start exits 134 today, `runtime/parts/spawn.c:520-523`, critic finding 14)
    ("failing `limit` where the system refuses a thread;",
     "failing `limit` where the system or the runtime has no thread to give, its message saying why;"),
    # S6 and S7: a failed send leaves `v` (the compiler-engineer, the ffi-pragmatist, the historian's Q3);
    # the stop as the critic's behaviour 3, a drain, then `stopped` for ever (critic findings 6 and 6b);
    # `closed` before `stopped` (finding 8); the late value after a `timeout` (the compiler-engineer's
    # `timeouts.txt`)
    ("`m.send(v)` adds a copy of `v` and never waits, failing `closed` once the mailbox is closed; "
     "`m.receive(within_ms: n)` takes the oldest value, waiting at most `n` milliseconds, `-1` for no limit and below that an abort, "
     "and fails `timeout`, `closed` once it is closed and empty, or `stopped`, before all else, once a stop is asked (section 13), even with values in it.",
     "`m.send(v)` adds a copy of `v` and never waits, failing `closed` once the mailbox is closed, or `stopped` once it has answered `stopped`, `v` then left as it was; "
     "`m.receive(within_ms: n)` takes the oldest value, waiting at most `n` milliseconds, `-1` for no limit and below that an abort; "
     "on an empty mailbox it fails `closed` once it is closed, `stopped` once a stop is asked (section 13) and from then on, and `timeout` else, "
     "and a value that arrives after a `timeout` is the next receive's."),
    # S11 by thread (the ffi-pragmatist's veto and repair, phrased by thread after critic finding 2); W-f's
    # walk through a thread's answer (critic finding 11); S9's boundary (the ffi-pragmatist's Q2.2)
    ("or as the answer `wait` gives: anything else is a compile error, and so is a `(mailbox T)` whose `T` holds a `ptr`, a `cstr` or that mailbox's own type, "
     "in a field, an element, a payload or another mailbox's `T`, so no queue ever holds itself.",
     "or as the answer `wait` gives, and is then that thread's: reaching C through it on a thread that does not hold it aborts, naming the call. "
     "Anything else is a compile error, and so is a `(mailbox T)` whose `T` holds a `ptr`, a `cstr` or that mailbox's own type, "
     "in a field, an element, a payload, a thread's answer or another mailbox's `T`, so no queue ever holds itself, "
     "and a `(thread T)` or a `(mailbox T)` in an `extern` signature, a callback's parameter or a group's record."),
    # S12: the receive that finds it empty (the compiler-engineer's and the ffi-pragmatist's amendment)
    ("or a mailbox's `receive`, answer stopped;", "or a mailbox's `receive` that finds it empty, answer stopped;"),
]
for old, new in EDITS:
    assert t.count(old) == 1, ('not exactly once', old[:90], t.count(old))
    t = t.replace(old, new)
os.makedirs(out, exist_ok=True)
open(os.path.join(out, 'spec-r.md'), 'w', encoding='utf-8').write(t)
base = open(now, encoding='utf-8').read()
d = ''.join(difflib.unified_diff(base.splitlines(True), t.splitlines(True), fromfile='spec-now.md', tofile='spec-r.md'))
open(os.path.join(out, 'spec-r.diff'), 'w', encoding='utf-8').write(d)
print('spec-r', hashlib.sha256(t.encode()).hexdigest(), len(t.splitlines()), 'lines;', 'w2', hashlib.sha256(open(w2, 'rb').read()).hexdigest()[:16])
