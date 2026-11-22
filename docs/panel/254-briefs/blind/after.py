# After one session: saves the CLI's result as result.md beside run.json and prints one line for run.log:
# the folder, the exit, the subtype, the cost, the models, whether report.md and result.md exist.
# usage: python3 -I tools/after.py <folder> <exit status>
import json, os, sys
d, status = sys.argv[1], sys.argv[2]
subtype, cost, models = '?', '?', '?'
try:
    j = json.load(open(os.path.join(d, 'run.json')))
    subtype, cost = j.get('subtype', '?'), j.get('total_cost_usd', '?')
    models = ','.join(sorted((j.get('modelUsage') or {}).keys())) or '?'
    if isinstance(j.get('result'), str) and j['result'].strip():
        open(os.path.join(d, 'result.md'), 'w').write(j['result'])
except Exception as e:
    subtype = f'unreadable ({type(e).__name__})'
print(f"{d} exit={status} subtype={subtype} cost={cost} models={models} "
      f"report.md={'yes' if os.path.exists(os.path.join(d, 'report.md')) else 'no'} "
      f"result.md={'yes' if os.path.exists(os.path.join(d, 'result.md')) else 'no'}")
