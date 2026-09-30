#!/usr/bin/env python3
# Panel 183's measurement: which words open a line INSIDE brackets, in every
# tracked .hero file and every code fence of the spec, lexed by a FIXED lexer
# (the c85bccb8 snapshot's, which predates batch 2's column-0 rule), and
# whether each file holding one is accepted by the trunk's `heroes check`.
# Usage: reach.py <frozen heroes> <judging heroes> <repo> <outdir>
import json, os, re, subprocess, sys, collections

frozen, judge, repo, out = sys.argv[1:5]
os.makedirs(out, exist_ok=True)
OPEN = {'lparen', 'lbracket', 'lbrace'}
CLOSE = {'rparen', 'rbracket', 'rbrace'}
DECL = {'kw_record', 'kw_variant', 'kw_constant', 'kw_use', 'kw_extern', 'kw_test'}
STMT = {'kw_if', 'kw_while', 'kw_for', 'kw_match', 'kw_return', 'kw_assert', 'kw_else'}

files = subprocess.run(['git', '-C', repo, 'ls-files', '*.hero'], capture_output=True, text=True).stdout.split()
sources = [(f, os.path.join(repo, f)) for f in files]

# The spec's fences, each written to a file of its own.
spec = open(os.path.join(repo, 'spec/heroes-spec.md'), encoding='utf-8').read()
fences = re.findall(r'```[a-z]*\n(.*?)```', spec, re.S)
for i, body in enumerate(fences):
    p = os.path.join(out, 'fence-%02d.hero' % i)
    open(p, 'w', encoding='utf-8').write(body)
    sources.append(('spec fence %d' % i, p))

tokens_total = 0
first_at_col0 = collections.Counter()      # first token kind of a column-0 line inside brackets
decl_hits, stmt_hits = [], []
for name, path in sources:
    r = subprocess.run([frozen, 'lex', '--dump-tokens', '--json', path], capture_output=True, text=True)
    try:
        toks = json.loads(r.stdout)
    except Exception:
        continue
    tokens_total += len(toks)
    depth = 0
    seen_line = set()
    for k, t in enumerate(toks):
        kind, line, col = t['kind'], t['line'], t['col']
        if kind in ('comment', 'newline', 'indent', 'dedent', 'terminator', 'eof'):
            continue
        if line not in seen_line:
            seen_line.add(line)
            if depth > 0:
                if col == 1:
                    first_at_col0[kind] += 1
                    nxt = toks[k + 1]['kind'] if k + 1 < len(toks) else ''
                    if kind in DECL or (kind == 'kw_function' and nxt == 'ident'):
                        decl_hits.append((name, line, kind))
                if kind in STMT:
                    stmt_hits.append((name, line, col, kind))
        if kind in OPEN:
            depth += 1
        elif kind in CLOSE:
            depth = max(0, depth - 1)

def verdict(path):
    if not os.path.isfile(path):
        return 'n/a'
    r = subprocess.run([judge, 'check', '--brief', path], capture_output=True, text=True, cwd=os.path.dirname(path) or '.')
    return 'exit %d' % r.returncode

print('sources: %d tracked .hero files and %d spec fences, %d tokens' % (len(files), len(fences), tokens_total))
print('first token of a column-0 line inside brackets, by kind:', dict(first_at_col0))
print('declaration words opening a column-0 line inside brackets: %d' % len(decl_hits))
for h in decl_hits:
    print('   ', h)
stmt_files = sorted(set(h[0] for h in stmt_hits))
print('statement words opening a line inside brackets: %d lines in %d files' % (len(stmt_hits), len(stmt_files)))
for f in stmt_files:
    p = os.path.join(repo, f) if not f.startswith('spec fence') else None
    lines = [h for h in stmt_hits if h[0] == f]
    print('   %s: %d lines (%s), check %s' % (f, len(lines), ', '.join('%d:%d %s' % (h[1], h[2], h[3][3:]) for h in lines[:4]), verdict(p) if p else 'fence'))
