"""reach.py <files-root> <floors-list>: which Rebel packages' faces reach a floor that binds a member's header,
by `use` lines read from the package root (spec 1: inside a package a `use` starts at the package's own root).
Test blocks are not separated: a `use` is a module-level line, read wherever it stands."""
import os, re, sys

root = sys.argv[1]
floors = set()
for line in open(sys.argv[2]):
    p = line.strip()
    if p:
        floors.add(os.path.relpath(p, root))

uses = {}
for d, _, fs in os.walk(root):
    for f in fs:
        if f.endswith('.hero'):
            path = os.path.join(d, f)
            rel = os.path.relpath(path, root)
            us = []
            for l in open(path, encoding='utf-8'):
                m = re.match(r'^use ([a-z_0-9/]+)', l)
                if m:
                    us.append(m.group(1) + '.hero')
            uses[rel] = us

memo = {}
def reaches(m, stack=()):
    if m in memo:
        return memo[m]
    if m in stack:
        return set()
    out = set()
    if m in floors:
        out.add(m)
    for u in uses.get(m, []):
        if u in uses:
            out |= reaches(u, stack + (m,))
    memo[m] = out
    return out

faces = sorted(f for f in uses if '/' not in f)
hit = [(f, sorted(reaches(f))) for f in faces]
reaching = [(f, r) for f, r in hit if r]
print(f"faces {len(faces)}, reaching a member floor {len(reaching)}")
for f, r in reaching:
    print(f"{f[:-5]}: {' '.join(x[:-5] for x in r)}")
mods = [m for m in uses if reaches(m)]
print(f"modules {len(uses)}, reaching a member floor {len(mods)}")
