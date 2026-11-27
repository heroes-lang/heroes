"""Panel 262, the synthesis: write the two arms of the blind reading, each the landed spec
(main 77eb38534, sha256 9ff7d2a1...2fc3) with one sentence replaced, asserting the replaced
sentence occurs once. python3 -I arms.py <landed spec> <adopted text> <rival text> <out dir>"""
import hashlib
import sys

NOW = (
    "A callback called after C left a handler of C's by a jump no function of the C library makes "
    "(written in assembly, say), by a `swapcontext` away from it, by a jump or a `setcontext` onto a "
    "stack once given to `makecontext`, or by a jump that lands within it, may abort, saying the "
    "handler was not seen to end, or that C called it from inside one."
)

spec_path, adopted_path, rival_path, out = sys.argv[1:5]
spec = open(spec_path, encoding="utf-8").read()
assert hashlib.sha256(spec.encode()).hexdigest() == (
    "9ff7d2a1bfda4323e54de004b27ef522bd86885a3fa249eb20406fb2a2942fc3"
), "not the landed spec"
assert spec.count(NOW) == 1, spec.count(NOW)
for name, path in (("adopted", adopted_path), ("rival", rival_path)):
    text = open(path, encoding="utf-8").read().strip()
    assert "\n" not in text
    arm = spec.replace(NOW, text)
    assert arm.count(text) == 1
    with open(f"{out}/spec-{name}.md", "w", encoding="utf-8") as f:
        f.write(arm)
    print(name, hashlib.sha256(arm.encode()).hexdigest(), len(arm.encode()), arm.count("\n"))
