# Applies one of the spec-warden's texts (docs/panel/262-reports/spec-wN.txt) to a copy of
# spec/heroes-spec.md: each span below is asserted to occur exactly once and replaced.
# usage: python3 -I apply.py <N> <spec in> <spec out> <reports dir>
import sys

S0 = ("A callback called after C left a handler of C's by a jump no function of the C library makes "
      "(written in assembly, say), by a `swapcontext` away from it, by a jump or a `setcontext` onto a "
      "stack once given to `makecontext`, or by a jump that lands within it, may abort, saying the handler "
      "was not seen to end, or that C called it from inside one.")
ROOM = ("Where C leaves a callback's frames without their returning (by a jump, an exception or a coroutine "
        "it never resumes, say), a value they held of more than 4,096 bytes, or holding a `partial` record, "
        "and a buffer they lent to C stay held until the program ends, counted with the live ones against "
        "1 GiB for the whole program, past which it aborts, saying how many callbacks have not returned.")
L437 = "A callback is a **parameter**, never a result or a signal handler, nor called by one;"

n, src, dst, reports = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]
text = open(f"{reports}/spec-w{n}.txt").read().rstrip("\n")
spec = open(src).read()


def swap(spec, old, new):
    assert spec.count(old) == 1, (old[:60], spec.count(old))
    return spec.replace(old, new)


if n == "5":
    spec = swap(spec, ROOM + " " + S0, text)
elif n == "6":
    spec = swap(spec, " " + S0, "")
    spec = swap(spec, L437, text)
elif n == "8":
    spec = swap(spec, S0 + " ", "")
else:
    spec = swap(spec, S0, text)
open(dst, "w").write(spec)
