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
ARMS = ['now', 'm', 'm0']
CELLS = [  # (task, arm, task file)
    ('t1', 'now', 'task-t1.md'), ('t1', 'm', 'task-t1.md'),
    ('t2', 'm', 'task-t2.md'), ('t2', 'm0', 'task-t2.md'),
    ('t3', 'm', 'task-t3.md'),
]
def sha(b): return hashlib.sha256(b).hexdigest()
def read(p): return open(p, 'rb').read()
reg = dict(l.split() for l in open('tools/arms.sha256'))  # the arms' hashes, registered before the draw
for a in ARMS:
    assert sha(read(f'arms/spec-{a}.md')) == reg[f'spec-{a}.md'], a
pool = list('cfghjkpqvwxyz')  # no letter an arm is named by (`m`), the critic's finding 24
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
# the verdict session: the frozen spec and the proposal, their diff, task t1; never graded
os.makedirs('verdict')
a_txt, b_txt = read('arms/spec-now.md'), read('arms/spec-m.md')
diff = ''.join(difflib.unified_diff(a_txt.decode().splitlines(True), b_txt.decode().splitlines(True),
                                    fromfile='spec-A.md', tofile='spec-B.md')).encode()
vf = {'brief.md': read('tools/brief-verdict.md'), 'task.md': read('tools/task-t1.md'),
      'spec-A.md': a_txt, 'spec-B.md': b_txt, 'changes.diff': diff}
for f, b in vf.items():
    open(os.path.join('verdict', f), 'wb').write(b)
rows.append('verdict verdict ' + ' '.join(f'{f}={sha(b)}' for f, b in sorted(vf.items())))
# the order: REPLICATES rounds over the cells, odd rounds forward and even rounds reversed, the verdict last
order = []
for r in range(1, REPLICATES + 1):
    seq = cells if r % 2 == 1 else list(reversed(cells))
    order += [f'{t}-{l}-{r}' for t, l in seq]
order.append('verdict')
open('key.txt', 'w').write('# label per arm: ' + ' '.join(f'{a}={draw[a]}' for a in ARMS) + '\n' + '\n'.join(rows) + '\n')
open('order.txt', 'w').write('\n'.join(order) + '\n')
print(len(order), 'sessions in order.txt;', sum(len(os.listdir(os.path.join('s', x))) for x in os.listdir('s')), 'files under s/')
