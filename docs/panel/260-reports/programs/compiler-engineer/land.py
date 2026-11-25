#!/usr/bin/env python3
"""land.py <tree> [--dry-run]: what the landing of `layer` owes a tree, written and counted.

Adds ` layer` after the header string of every group, in every tracked `.hero` file, whose header the
rule of `check/layer_header.hero` calls the layer's (`hero_<word>.h` but `hero_os.h` and `hero_compiler.h`,
or `platform/hero_<word>.h`), or whose header is a tracked header that reaches a member's header through
its includes at any depth (the critic's `own_deep.py` reading, recomputed here from the tree). Prints one
line per file changed and the counts by area. Never touches a line that already says `layer`.
"""
import os, re, subprocess, sys

tree = os.path.abspath(sys.argv[1])
dry = "--dry-run" in sys.argv

def names_layer(h):
    while h.startswith("./"):
        h = h[2:]
    def stem(n):
        return n.startswith("hero_") and n.endswith(".h") and len(n) > 7 and "/" not in n and "\\" not in n
    if h.startswith("platform/"):
        return stem(h[len("platform/"):])
    return stem(h) and h not in ("hero_os.h", "hero_compiler.h")

files = subprocess.run(["git", "-C", tree, "ls-files", "-z"], capture_output=True, check=True).stdout.decode().split("\0")
files = [f for f in files if f]
members = {os.path.basename(f)[:-2] + ".h" for f in files if f.startswith("runtime/platform/") and f.endswith(".c")}
inc = re.compile(r'^\s*#\s*(?:include|include_next|import)\s*[<"]([^">]+)[">]', re.M)
cfiles = [f for f in files if f.endswith((".h", ".c")) and not f.startswith(("runtime/", "seed/", "tests/emission/"))]
by_dir = {}
for f in cfiles:
    by_dir.setdefault(os.path.dirname(f), set()).add(os.path.basename(f))
edges = {}
for f in cfiles:
    try:
        text = open(os.path.join(tree, f), encoding="utf-8", errors="replace").read()
    except OSError:
        continue
    edges[f] = inc.findall(text)

def reaches(f, seen):
    if f in seen:
        return False
    seen.add(f)
    for name in edges.get(f, []):
        if os.path.basename(name) in members or names_layer(name):
            return True
        here = os.path.normpath(os.path.join(os.path.dirname(f), name))
        if here in edges and reaches(here, seen):
            return True
    return False

reaching = {f for f in cfiles if f.endswith(".h") and reaches(f, set())}
head = re.compile(r'^extern "([^"]+)"(?!\s+layer\b)')
counts = {}
changed = 0
for f in files:
    if not f.endswith(".hero"):
        continue
    path = os.path.join(tree, f)
    try:
        lines = open(path, encoding="utf-8").read().split("\n")
    except (OSError, UnicodeDecodeError):
        continue
    out, n = [], 0
    for line in lines:
        m = head.match(line)
        if m:
            h = m.group(1)
            own = os.path.normpath(os.path.join(os.path.dirname(f), h))
            if names_layer(h) or own in reaching:
                line = line[: m.end(1) + 1] + " layer" + line[m.end(1) + 1 :]
                n += 1
        out.append(line)
    if n:
        changed += 1
        area = f.split("/")[0] if not f.startswith("tests/") else "/".join(f.split("/")[:3])
        if ".case/" in f:
            area += " (.case/)"
        counts[area] = counts.get(area, 0) + 1
        print(f"{n} {f}")
        if not dry:
            open(path, "w", encoding="utf-8").write("\n".join(out))
print("## headers reaching a member, tracked:", len(reaching))
for h in sorted(reaching):
    print("   ", h)
print("## files changed:", changed)
for a in sorted(counts):
    print(f"   {counts[a]} {a}")
