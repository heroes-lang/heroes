#!/usr/bin/env python3
import json, os, subprocess, sys
root, rows_path = sys.argv[1], sys.argv[2]
rows = json.load(open(rows_path))
for r in rows:
    d = os.path.join(root, os.path.dirname(r['file']))
    src = '#include "%s"\nextern __typeof__(%s) hero_ty_%s;\n' % (r['header'], r['fn'], r['fn'])
    p = subprocess.run(['clang', '-x', 'c', '-std=gnu11', '-fsyntax-only', '-I', d, '-I', os.path.join(root, 'runtime'), '-Xclang', '-ast-dump=json', '-Xclang', '-ast-dump-filter=hero_ty_', '-'], input=src, capture_output=True, text=True)
    qt = '?'
    if p.returncode == 0 and p.stdout.strip():
        dec = json.JSONDecoder(); s = p.stdout.strip(); idx = 0; objs = []
        while idx < len(s):
            o, e = dec.raw_decode(s, idx); objs.append(o); idx = e
            while idx < len(s) and s[idx].isspace(): idx += 1
        for o in objs:
            ty = o.get('type', {}).get('desugaredQualType', o.get('type', {}).get('qualType', ''))
            # the function type text: result (params)
            qt = ty
    else:
        qt = 'NO ANSWER: ' + (p.stderr.strip().split('\n')[0] if p.stderr else '')
    r['fn_type'] = qt
    params = qt[qt.find('(')+1: qt.rfind(')')] if '(' in qt else ''
    depth=0; parts=[]; buf=''
    for ch in params:
        if ch in '([': depth+=1
        if ch in ')]': depth-=1
        if ch==',' and depth==0: parts.append(buf.strip()); buf=''
        else: buf+=ch
    if buf.strip(): parts.append(buf.strip())
    qt = parts[r['k']] if r['k'] < len(parts) else qt
    r['c_type'] = qt
    print('%-80s %-14s %-12s k=%d %s' % (r['file'][-80:], r['fn'], r['record'], r['k'], qt))
json.dump(rows, open(rows_path.replace('.json', '-typed.json'), 'w'), indent=1)
