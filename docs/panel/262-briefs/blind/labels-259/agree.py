import sys
def read(p):
    d = {}
    for l in open(p):
        if l.startswith("#") or not l.strip(): continue
        s, q, labs = l.rstrip("\n").split("\t")
        d[(s, q)] = set(labs.split(","))
    return d
a, b = read(sys.argv[1]), read(sys.argv[2])
keys = sorted(a)
assert set(a) == set(b), (set(a) ^ set(b))
n = len(keys)
exact = sum(a[k] == b[k] for k in keys)
overlap = sum(bool(a[k] & b[k]) for k in keys)
print(f"units {n}; same set {exact}; at least one label shared {overlap}")
for lab in "W N L H O T I P R S".split():
    pa = [lab in a[k] for k in keys]; pb = [lab in b[k] for k in keys]
    agree = sum(x == y for x, y in zip(pa, pb)) / n
    ya, yb = sum(pa) / n, sum(pb) / n
    pe = ya * yb + (1 - ya) * (1 - yb)
    kappa = (agree - pe) / (1 - pe) if pe < 1 else float("nan")
    print(f"{lab}: A {sum(pa):2d}  B {sum(pb):2d}  both {sum(x and y for x,y in zip(pa,pb)):2d}  agreement {agree:.2f}  kappa {kappa:.2f}")
