"""reach_examples.py <files-root> <floors-list> <examples-root>: for each folder of examples-root holding a heroes.toml,
whether a module of it reaches, through `use <x>/<path>` and the library's own `use` lines, a floor binding a member's
header; and whether one of its own modules names a hero_*.h group itself."""
import os, re, sys

root, floors_list, ex_root = sys.argv[1], sys.argv[2], sys.argv[3]
floors = {os.path.relpath(l.strip(), root) for l in open(floors_list) if l.strip()}
uses = {}
for d, _, fs in os.walk(root):
    for f in fs:
        if f.endswith('.hero'):
            p = os.path.join(d, f)
            uses[os.path.relpath(p, root)] = [m.group(1) + '.hero' for l in open(p, encoding='utf-8')
                                              for m in [re.match(r'^use ([a-z_0-9/]+)', l)] if m]
memo = {}
def reaches(m, stack=()):
    if m in memo:
        return memo[m]
    if m in stack:
        return set()
    out = {m} if m in floors else set()
    for u in uses.get(m, []):
        if u in uses:
            out |= reaches(u, stack + (m,))
    memo[m] = out
    return out

total = reaching = own = 0
for ex in sorted(os.listdir(ex_root)):
    d = os.path.join(ex_root, ex)
    if not os.path.isfile(os.path.join(d, 'heroes.toml')):
        continue
    total += 1
    hit, binds = set(), False
    for dd, _, fs in os.walk(d):
        for f in fs:
            if f.endswith('.hero'):
                for l in open(os.path.join(dd, f), encoding='utf-8'):
                    m = re.match(r'^use <[a-z_0-9]+>/([a-z_0-9/]+)', l)
                    if m:
                        hit |= reaches(m.group(1) + '.hero')
                    if re.match(r'^extern "hero_[a-z_]+\.h"', l) and not re.match(r'^extern "hero_(os|compiler)\.h"', l):
                        binds = True
    reaching += bool(hit)
    own += binds
    print(f"{ex}: {'REACH' if hit else '-'} {'OWN' if binds else ''} {len(hit)}")
print(f"examples {total}, reaching a member floor {reaching}, binding a member header themselves {own}")
