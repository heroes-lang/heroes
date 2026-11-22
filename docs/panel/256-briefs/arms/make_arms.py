# Panel 256: the arms the sitting's proposal is stated in, made from the frozen spec by asserted edits,
# each edit matching exactly once. Written 2026-11-22 by the sitting's coordinator.
# usage (from this folder): python3 -I make_arms.py <frozen spec/heroes-spec.md>
#   writes spec-now.md, spec-q1.md, spec-gc.md, spec-gi.md, spec-gr.md, their .diff against now, and
#   arms.sha256. Refuses to overwrite.
import difflib, hashlib, os, sys
src = open(sys.argv[1]).read()
for f in ('spec-now.md',):
    assert not os.path.exists(f), f'{f} exists: nothing made'

def edit(text, old, new):
    assert text.count(old) == 1, f'not exactly once: {old[:80]!r} ({text.count(old)})'
    return text.replace(old, new)

CROSS_END = ("so no queue ever holds itself, and a `(thread T)` or a `(mailbox T)` in an `extern` "
             "signature, a callback's parameter or a group's record.")

# Q1: a mailbox is a source a wait takes, by a number naming its queue to C (P1, item 1).
q1 = src
q1 = edit(q1, "except a thread's `wait` and a mailbox's `send`,\n  `receive` and `close` (section 11)",
              "except a thread's `wait` and a mailbox's `send`,\n  `receive`, `close` and `number` (section 11)")
q1 = edit(q1, "`m.close()` closes it for every copy, and closing it again does nothing;",
              "`m.number()` gives an `i64` that names its queue to C, for a package that waits on it beside other things: it keeps no copy and reaches no value, and once the queue has gone it names nothing. `m.close()` closes it for every copy, and closing it again does nothing;")

# Q2, gc: judged at each call on the types it gives the letters; a letter's value always sent with `@`.
GC = (" In a generic function these rules are judged at each call, on the types that call gives the "
      "type parameters, and a value whose type names a type parameter is always sent `m.send(@v)`, "
      "which empties it whatever that type holds.")
gc = edit(src, CROSS_END, CROSS_END + GC)

# Q2, gi: judged at each call on the types it gives the letters, the `@` included.
GI = (" In a generic function these rules are judged at each call, on the types that call gives the "
      "type parameters, the `@` of a send included: a generic function that sends a type parameter's "
      "value serves the types that hold a handle, written `m.send(@v)`, or those that hold none, written "
      "`m.send(v)`, and is refused at a call that gives it the other kind.")
gi = edit(src, CROSS_END, CROSS_END + GI)

# Q2, gr: refused in the body, panel 155's shape for `==`.
GR = (" In a generic function a value whose type names a type parameter is not sent, handed to `spawn` "
      "or answered by a thread, and a `(mailbox T)` or `(thread T)` names no type parameter, since the "
      "function cannot know what a call will give it: take the operation as a parameter instead.")
gr = edit(src, CROSS_END, CROSS_END + GR)

arms = {'now': src, 'q1': q1, 'gc': gc, 'gi': gi, 'gr': gr}
lines = []
for name, text in arms.items():
    f = f'spec-{name}.md'
    open(f, 'w').write(text)
    lines.append(f'{f} {hashlib.sha256(text.encode()).hexdigest()}')
    if name != 'now':
        d = ''.join(difflib.unified_diff(src.splitlines(True), text.splitlines(True), fromfile='spec-now.md', tofile=f))
        open(f'spec-{name}.diff', 'w').write(d)
open('arms.sha256', 'w').write('\n'.join(lines) + '\n')
print('\n'.join(lines))
