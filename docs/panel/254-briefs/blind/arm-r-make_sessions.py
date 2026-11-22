# Panel 254's fourth arm (R9, the resolution's text), adapted 2026-11-22 by the scoring session from the
# first area's make_sessions.py by asserted edits: one arm, three cells, no verdict session, the first
# draw's letters (q, y, p) out of the pool. The first file's header follows.
# Panel 254's blind seat: draws the labels, then makes one session folder per task, arm and replicate,
# each holding brief.md, task.md and heroes-spec.md and nothing else, and the verdict session's folder.
# The key and the order are written beside s/, never inside it. Refuses to run over an existing s/.
# Written 2026-11-22 by panel 254's coordinator, on panel 253's make_sessions.py.
# usage (from the blind area): python3 -I tools/make_sessions.py
import difflib, hashlib, os, secrets, sys
assert os.path.isfile('tools/make_sessions.py'), 'run from the blind area'
if os.path.exists('s') or os.path.exists('verdict') or os.path.exists('key.txt'):
    sys.exit('s/, verdict/ or key.txt exists: nothing made')
REPLICATES = 5
ARMS = ['r']
CELLS = [  # (task, arm, task file)
    ('t1', 'r', 'task-t1.md'), ('t2', 'r', 'task-t2.md'), ('t3', 'r', 'task-t3.md'),
]
def sha(b): return hashlib.sha256(b).hexdigest()
def read(p): return open(p, 'rb').read()
reg = dict(l.split() for l in open('tools/arms.sha256'))  # the arms' hashes, registered before the draw
for a in ARMS:
    assert sha(read(f'arms/spec-{a}.md')) == reg[f'spec-{a}.md'], a
pool = list('cfghjkvwxz')  # no letter an arm is named by (`m`), the critic's finding 24
labels = secrets.SystemRandom().sample(pool, len(ARMS))
draw = dict(zip(ARMS, labels))
print('draw', ' '.join(f'{a}->{draw[a]}' for a in ARMS))
brief = read('tools/brief-session.md')
rows, cells = [], []
for task, arm, tfile in CELLS:
    for n in range(1, REPLICATES + 1):
        name = f'{task}-{draw[arm]}-{n}'
        d = os.path.join('s', name)
        os.makedirs(d)
        files = {'brief.md': brief, 'task.md': read(f'tools/{tfile}'), 'heroes-spec.md': read(f'arms/spec-{arm}.md')}
        for f, b in files.items():
            open(os.path.join(d, f), 'wb').write(b)
        assert sorted(os.listdir(d)) == sorted(files)
        rows.append(f'{name} {arm} ' + ' '.join(f'{f}={sha(b)}' for f, b in sorted(files.items())))
    cells.append((task, draw[arm]))
# no verdict session for the fourth arm (its registration.md says why)
# the order: REPLICATES rounds over the cells, odd rounds forward and even rounds reversed, the verdict last
order = []
for r in range(1, REPLICATES + 1):
    seq = cells if r % 2 == 1 else list(reversed(cells))
    order += [f'{t}-{l}-{r}' for t, l in seq]
open('key.txt', 'w').write('# label per arm: ' + ' '.join(f'{a}={draw[a]}' for a in ARMS) + '\n' + '\n'.join(rows) + '\n')
open('order.txt', 'w').write('\n'.join(order) + '\n')
print(len(order), 'sessions in order.txt;', sum(len(os.listdir(os.path.join('s', x))) for x in os.listdir('s')), 'files under s/')
