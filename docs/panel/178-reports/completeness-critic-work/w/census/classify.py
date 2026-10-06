# Critic, panel 178: which census records a header-length field (route H) would make one binding.
import re, sys
def parse(path):
    recs = {}           # name -> list of (field, class)
    stack = []          # (depth, record name or None)
    for line in open(path):
        m = re.match(r'^([ |`-]*)(RecordDecl|FieldDecl) ', line)
        if not m: continue
        depth = len(m.group(1))
        while stack and stack[-1][0] >= depth: stack.pop()
        if m.group(2) == 'RecordDecl':
            if ' definition' not in line: stack.append((depth, None)); continue
            k = re.search(r' (struct|union) ([A-Za-z_][A-Za-z_0-9]*) definition', line)
            name = k.group(2) if k else None
            stack.append((depth, name))
            if name and name not in recs: recs[name] = []
            elif name: recs[name] = []   # a later definition replaces
        else:
            parent = stack[-1][1] if stack else None
            if parent is None: continue
            q = re.findall(r"'([^']*)'", line)
            if not q: continue
            ty = q[1] if len(q) > 1 else q[0]
            nm = re.search(r"col:\d+ (?:referenced )?(?:implicit )?([A-Za-z_][A-Za-z_0-9]*) '", line)
            recs[parent].append((nm.group(1) if nm else '<anon>', cls(ty)))
    return recs
SC = {'char':'i8','signed char':'i8','unsigned char':'u8','_Bool':'bool','short':'i16','unsigned short':'u16','int':'i32','unsigned int':'u32',
      'long':'i64','long long':'i64','unsigned long':'u64','unsigned long long':'u64','float':'f32','double':'f64','long double':'f128'}
def cls(t):
    t = re.sub(r'\(anonymous (struct|union) at [^)]*\)', 'anon', t).replace('volatile ', '').replace('const ', '').strip()
    a = re.match(r'^(.*?)\s*\[(\d+)\]$', t)
    if a: return ('arr', cls(a.group(1)), int(a.group(2)))
    if '*' in t or '(' in t: return ('ptr',)
    return ('s', SC.get(t, t))
def arrays(fs):
    out = []
    for n, c in fs:
        if c[0] == 'arr': out.append(c[2])
    return out
legs = {l: parse(f'{l}.txt') for l in ['darwin', 'linux-arm64', 'linux-x86']}
def strip_len(c):
    return ('arr', strip_len(c[1]), '_') if c[0] == 'arr' else c
cands = set()
for l, R in legs.items():
    for n, fs in R.items():
        if not n.startswith('_') and any(x > 8 for x in arrays(fs)): cands.add(n)
rows = []
for n in sorted(cands):
    have = [l for l in legs if n in legs[l]]
    if len(have) < 3:
        rows.append((n, 'not on every leg: ' + ','.join(have))); continue
    fl = [legs[l][n] for l in legs]
    if fl[0] == fl[1] == fl[2]: rows.append((n, 'same on all three')); continue
    sl = [[(f, strip_len(c)) for f, c in x] for x in fl]
    if sl[0] == sl[1] == sl[2]: rows.append((n, 'H makes it one binding (only lengths differ)')); continue
    rows.append((n, 'differs beyond lengths'))
from collections import Counter
for n, r in rows: print(f'{n}\t{r}')
print('---'); print(Counter(r.split(':')[0] for _, r in rows))
print('--- detail for records on all three legs that differ')
for n, r in rows:
    if r != 'differs beyond lengths': continue
    fl = [dict((f, c) for f, c in legs[l][n]) for l in legs]
    names = [ [f for f, _ in legs[l][n]] for l in legs]
    common = [f for f in names[0] if all(f in d for d in fl) and len({str(strip_len(d[f])) for d in fl}) == 1]
    longc = [f for f in common if fl[0][f][0] == 'arr']
    diff = sorted(set(sum(names, [])) - set(common))
    def show(f):
        return '/'.join(str(d.get(f, '-')) for d in fl)
    print(f'{n}: common under H+partial {common}; not common: ' + '; '.join(f'{f}={show(f)}' for f in diff))
