# Critic, panel 178: pull the three programs out of a reader's answer.
import re, sys, os
d = sys.argv[1]
s = open(os.path.join(d, 'answer.md')).read()
for k in (1, 2, 3):
    m = re.search(r'TASK %d\s*\n```[a-z]*\n(.*?)```' % k, s, re.S)
    open(os.path.join(d, f't{k}.hero'), 'w').write(m.group(1) if m else '# MISSING\n')
mu = re.search(r'MUTEX:\s*(.*)', s)
open(os.path.join(d, 'mutex.txt'), 'w').write((mu.group(1).strip() if mu else '?') + '\n')
