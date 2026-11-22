# Panel 256: task t2 made from `t2-programs/` (db.hero and prog1 to prog7), so the task and the programs the
# frozen compiler ran are the same bytes. usage (from blind/): python3 -I make_t2.py   (overwrites task-t2.md)
import os
d = 't2-programs'
def body(f): return open(os.path.join(d, f)).read().rstrip('\n')
out = ['# Task', '',
 'Below are a module `db.hero` and seven programs, each a whole `main.hero` beside that module. For each',
 'program, in order, say:', '',
 '1. whether `heroes build main.hero` accepts it, and where it does not, which line it refuses and why;',
 '2. where it is accepted, every line the program writes, in order;',
 '3. how it ends: normally, or by an abort and what the abort says.', '',
 'Each is run once with no arguments, on a machine where `sqlite3.h` and its library are installed.', '',
 '## `db.hero`', '', '```', body('db.hero'), '```', '']
for i in range(1, 8):
    out += [f'## Program {i}', '', '```', body(f'prog{i}.hero'), '```', '']
open('task-t2.md', 'w').write('\n'.join(out))
print(len(out), 'lines')
