# Panel 254's blind seat, read by the scoring session 2026-11-22: every session's run.json, its three success
# fields, its cost, turns and models, and their sum. usage: python3 -I read_runs.py <area>

import json, sys, os
root = sys.argv[1]
tot = 0.0
rows = []
for name in sorted(os.listdir(os.path.join(root, 's'))) + ['verdict']:
    d = os.path.join(root, 's', name) if name != 'verdict' else os.path.join(root, 'verdict')
    j = json.load(open(os.path.join(d, 'run.json')))
    c = j.get('total_cost_usd')
    tot += c
    models = list((j.get('modelUsage') or {}).keys())
    print(f"{name:10} subtype={j.get('subtype')} is_error={j.get('is_error')} terminal={j.get('terminal_reason')} cost={c:.6f} turns={j.get('num_turns')} models={models}")
print(f"sum={tot:.6f}")
