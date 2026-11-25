# closure.py <runtime-dir> <nm-table> : the header closure layer_reach computes,
# checked against every member-to-member symbol edge of an archive.
import os, re, sys
rt, table = sys.argv[1], sys.argv[2]
inc = re.compile(r'^[ \t]*#[ \t]*include[ \t]*["<]([^">]+)[">]', re.M)
def includes(path):
    try:
        return inc.findall(open(path, encoding='utf-8', errors='replace').read())
    except OSError:
        return []
# the files runtime.c names, followed as runtime_key does (quoted includes)
files, todo = [], [os.path.join(rt, 'runtime.c')]
while todo:
    f = todo.pop(0)
    if f in files: continue
    files.append(f)
    for name in re.findall(r'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"', open(f, errors='replace').read(), re.M):
        for cand in (os.path.join(os.path.dirname(f), name), os.path.join(rt, name)):
            if os.path.exists(cand):
                todo.append(os.path.normpath(cand)); break
def closure(named):
    reach = set(os.path.basename(n) for n in named)
    read = set(); grew = True
    while grew:
        grew = False
        for f in files:
            if f in read: continue
            b = os.path.basename(f)
            opens = (b.endswith('.h') and b in reach) or (b.endswith('.c') and '/platform/' in f and b[:-2] + '.h' in reach)
            if opens:
                read.add(f); grew = True
                for n in includes(f): reach.add(os.path.basename(n))
    return reach
members = [f for f in files if '/platform/' in f and (f.endswith('.c') or f.endswith('.swift'))]
def reached_members(named):
    r = closure(named)
    out = set()
    for m in members:
        b = os.path.basename(m)
        stem = b[:-2] if b.endswith('.c') else b[:-6]
        if stem + '.h' in r: out.add(stem)
    return out
defs, uses = {}, []
for line in open(table):
    k, m, s = line.split()
    m = m[:-2]
    if k == 'D': defs[s] = m
    else: uses.append((m, s))
missing = set()
for m, s in uses:
    if s in defs and defs[s] != m:
        if defs[s] not in reached_members([m + '.h']):
            missing.add((m, defs[s], s))
for x in sorted(missing): print('NOT REACHED', *x)
print('members', len(members), 'edges unreached', len(missing))
for h in sys.argv[3:]:
    print(h, '->', sorted(reached_members([h])))
