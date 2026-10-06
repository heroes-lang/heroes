#!/usr/bin/env python3
"""Lane ffi13, 2026-10-06: every `@` parameter of an extern function whose
declared type is a group record that is not a handle, in the tracked .hero
files of examples/ and tests/golden/, with the header's own parameter type
asked of clang the way cli/pointee.hero asks it (__typeof__ redeclaration,
-ast-dump=json)."""
import json, os, re, subprocess, sys
root = sys.argv[1]
files = subprocess.run(['git', 'ls-files', '--', 'examples', 'tests/golden'], cwd=root, capture_output=True, text=True).stdout.split()
files = [f for f in files if f.endswith('.hero')]
BUILTIN = {'i8','i16','i32','i64','u8','u16','u32','u64','f32','f64','bool','str','ptr','cstr'}
rows = []
for f in files:
    text = open(os.path.join(root, f), encoding='utf-8', errors='replace').read()
    lines = text.split('\n')
    records = {}   # name -> (tag, partial, nfields, header)
    groups = []    # (header, [member lines])
    cur = None
    i = 0
    while i < len(lines):
        ln = lines[i]
        m = re.match(r'^extern\s+"([^"]*)"', ln)
        if m:
            cur = m.group(1); i += 1; continue
        if cur is not None and ln and not ln.startswith(' ') and not ln.startswith('#'):
            cur = None
        if cur is not None:
            mr = re.match(r'^    record\s+(\w+)(?:\s+tag\s+(\w+))?(\s+partial)?\s*(#.*)?$', ln)
            if mr:
                n = 0; j = i + 1
                while j < len(lines) and (lines[j].startswith('        ') or lines[j].strip() == '' or lines[j].strip().startswith('#')):
                    if lines[j].startswith('        ') and not lines[j].strip().startswith('#') and ':' in lines[j]:
                        n += 1
                    if lines[j].strip() == '' : break
                    j += 1
                records[mr.group(1)] = (mr.group(2), bool(mr.group(3)), n, cur)
            mf = re.match(r'^    function\s+(\w+)\s*\(', ln)
            if mf:
                sig = ln; j = i
                while sig.count('(') > sig.count(')') and j + 1 < len(lines):
                    j += 1; sig += ' ' + lines[j].strip()
                groups.append((cur, mf.group(1), sig, f, i + 1))
        i += 1
    for header, fn, sig, path, lineno in groups:
        inside = sig[sig.index('(') + 1: sig.rindex(')')] if ')' in sig else ''
        # split params at top-level commas
        depth = 0; parts = []; buf = ''
        for ch in inside:
            if ch in '([{': depth += 1
            if ch in ')]}': depth -= 1
            if ch == ',' and depth == 0: parts.append(buf); buf = ''
            else: buf += ch
        if buf.strip(): parts.append(buf)
        for k, p in enumerate(parts):
            mp = re.match(r'\s*(@?)(\w+)\s*:\s*([\w.]+)(.*)$', p.strip() if False else p)
            if not mp: continue
            at, pname, ty, rest = mp.groups()
            if not at or ty in BUILTIN: continue
            rec = records.get(ty)
            if rec is None: continue
            tag, partial, nfields, rheader = rec
            handle = (tag is not None) and (not partial) and nfields == 0
            if handle: continue
            rows.append(dict(file=path, line=lineno, header=header, fn=fn, k=k, param=pname, record=ty, partial=partial, rest=rest.strip(), params=len(parts)))
print(json.dumps(rows, indent=1))
