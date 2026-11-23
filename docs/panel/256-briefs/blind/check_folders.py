# Before any spend (panel 256's, from panel 254's): order.txt names 46 folders, each in key.txt, and every folder not yet run holds
# exactly its registered files at their registered hashes. usage: python3 -I tools/check_folders.py
import hashlib, os, sys
key = {}
for line in open('key.txt'):
    if line.startswith('#') or not line.strip():
        continue
    name, arm, *files = line.split()
    key[name] = dict(f.split('=', 1) for f in files)
order = [l.strip() for l in open('order.txt') if l.strip()]
bad = []
if len(order) != 46 or len(set(order)) != 46:
    bad.append(f'order.txt names {len(order)} folders, {len(set(order))} distinct, not 46')
for name in order:
    d = 'verdict' if name == 'verdict' else os.path.join('s', name)
    if name not in key:
        bad.append(f'{name} not in key.txt'); continue
    if os.path.exists(os.path.join(d, 'run.json')):
        continue
    have = sorted(os.listdir(d)) if os.path.isdir(d) else []
    if have != sorted(key[name]):
        bad.append(f'{name} holds other files: {have}'); continue
    for f, h in key[name].items():
        if hashlib.sha256(open(os.path.join(d, f), 'rb').read()).hexdigest() != h:
            bad.append(f'{name}/{f} is not the registered file')
for b in bad:
    print(b)
sys.exit(2 if bad else 0)
