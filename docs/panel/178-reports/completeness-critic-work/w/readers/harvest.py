# Critic, panel 178: take a reader's final hand-back out of its transcript, into answer.md. Prints only a status line.
import json, sys
path, out = sys.argv[1], sys.argv[2]
msg = None
for line in open(path):
    try: o = json.loads(line)
    except Exception: continue
    m = o.get('message') if isinstance(o, dict) else None
    if not isinstance(m, dict): continue
    c = m.get('content')
    if not isinstance(c, list): continue
    for part in c:
        if isinstance(part, dict) and part.get('type') == 'tool_use' and part.get('name') == 'SubagentHandback':
            msg = part.get('input', {}).get('message')
        elif isinstance(part, dict) and part.get('type') == 'text' and m.get('role') == 'assistant' and 'TASK 1' in part.get('text', ''):
            msg = msg or part['text']
if msg is None:
    print('NO ANSWER', path); sys.exit(1)
open(out, 'w').write(msg)
print('ok', out, len(msg))
