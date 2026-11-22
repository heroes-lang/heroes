# The spend so far: the sum of total_cost_usd over every run.json under s/ and verdict/, one that cannot
# be read counted as 0.50. usage: python3 -I tools/spent.py
import glob, json
total = 0.0
for p in glob.glob('s/*/run.json') + glob.glob('verdict/run.json'):
    try:
        total += float(json.load(open(p)).get('total_cost_usd'))
    except Exception:
        total += 0.50
print(f'{total:.6f}')
