# Panel 256: the resolution's spec text and the blind run's final arms, made by asserted edits from the
# frozen spec and the spec-warden's repaired texts. Written 2026-11-22 by the sitting's coordinator.
# usage (from this folder): python3 -I make_resolution.py
#   writes spec-r.md (the resolution: spec-wq's `m.number()` and spec-wi's generic sentence, its "these
#   rules" repaired after the critic's second reading, finding 6), spec-r.diff, and the blind arms
#   arm-gc.md, arm-gi.md, arm-gr.md (gi's is spec-r itself), with arms.sha256.
import difflib, hashlib
W = '../spec-warden/'
now = open('../../256-briefs/arms/spec-now.md').read()
wq = open(W + 'spec-wq.md').read()
wi = open(W + 'spec-wi.md').read()
wc = open(W + 'spec-wc.md').read()
wr = open(W + 'spec-wr.md').read()
def sha(t): return hashlib.sha256(t.encode()).hexdigest()
assert sha(wq) == '0c53c68c6f9bfd83639223ac2474b7f8907e0f4093fe2895d20125e04262b7d5'
assert sha(wi) == '2ab0f22449c262e1b239e89f8f2ca8c02a6f1510071764ec0e83055a6a38d1c6'
assert sha(wc) == '83d752d6ba1695a474acd30ae73a1447e8c29b388679792a32b0a169082f2657'
assert sha(wr) == '32b0504b44f98852db86431e13a87078f0ce43e00b59e3491168120fb91db13f'
def one(text, old, new):
    assert text.count(old) == 1, (old[:80], text.count(old)); return text.replace(old, new)
END = "a callback's parameter or a group's record."
# spec-wi's sentence, taken from it whole
i = wi.index(END) + len(END)
gi_sentence = wi[i:wi.index('\n', i)]
assert gi_sentence.startswith(' A send is written `m.send(@v)` exactly where'), gi_sentence[:60]
THESE = 'In a generic function these rules are judged at each call'
FIXED = 'In a generic function every rule of this paragraph on what a value holds is judged at each call'
gi_fixed = one(gi_sentence, THESE, FIXED)
r = one(wq, END, END + gi_fixed)
arm_gi = one(now, END, END + gi_fixed)
arm_gc = one(wc, THESE, FIXED)
arm_gr = wr
out = {'spec-r.md': r, 'arm-gi.md': arm_gi, 'arm-gc.md': arm_gc, 'arm-gr.md': arm_gr}
lines = []
for f, t in out.items():
    open(f, 'w').write(t); lines.append(f'{f} {sha(t)}')
open('spec-r.diff', 'w').write(''.join(difflib.unified_diff(now.splitlines(True), r.splitlines(True), fromfile='spec-now.md', tofile='spec-r.md')))
open('arms.sha256', 'w').write('\n'.join(lines) + '\n')
print('\n'.join(lines))
