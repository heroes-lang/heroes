# Panel 254's blind seat, t1 `now`, graded mechanically by the rows of
# docs/panel/254-briefs/llm-ergonomist.md as registered: the answer's main.hero is the longest fenced
# block under that name in its result (report.md is missing in every session), built by a stage 3 at
# sources 6e891e681c14bca0 and run under `timeout -s KILL 20` five times on the task's own documents;
# then four substitutions, each one edit asserted to match once in a copy, built and run once.
# usage: python3 -I mech.py <compiler> <session folder> <work folder>
import os, re, subprocess, sys

compiler, session, work = sys.argv[1], sys.argv[2], sys.argv[3]
text = open(os.path.join(session, 'result.md')).read()

# Every fenced block; the ones under a `main.hero` mention in the 400 characters before the fence.
blocks = []
for m in re.finditer(r'^```[a-z]*\n(.*?)^```', text, re.S | re.M):
    before = text[max(0, m.start() - 400):m.start()]
    if 'main.hero' in before:
        blocks.append(m.group(1))
src = max(blocks, key=len)
assert 'function main()' in src, 'the longest block under main.hero holds no main'

BASE = ['a: 4 words, 4 distinct', 'b: 3 words, 3 distinct', 'c: 4 words, 3 distinct', 'd: 1 words, 1 distinct']
SUBS = [
    ('S1', '"the cat sat on the mat"', '"the cat|dog sat on the mat"', ['a: 4 words, 4 distinct'] + BASE[1:]),
    ('S2', '"the cat sat on the mat"', '"the cat\\nsat on the mat"', ['a: 3 words, 3 distinct'] + BASE[1:]),
    ('S3', '"the cat sat on the mat"',
     '"the c,a:t; d\\tog=x \\"q\\" \\\\ \\u{1e}r\\u{1f}s # mat mat"', ['a: 8 words, 7 distinct'] + BASE[1:]),
    ('S4', 'name: "a"', 'name: "a|b,c: d\\ne"', ['a|b,c: d', 'e: 4 words, 4 distinct'] + BASE[1:]),
]

env = dict(os.environ)
env['TMPDIR'] = os.path.join(work, 'tmp')
os.makedirs(env['TMPDIR'], exist_ok=True)

def build_and_run(tag, program, expected, runs):
    d = os.path.join(work, tag)
    os.makedirs(d, exist_ok=True)
    with open(os.path.join(d, 'main.hero'), 'w') as f:
        f.write(program)
    b = subprocess.run([compiler, 'build', 'main.hero', '-o', 'prog'], cwd=d, env=env,
                       capture_output=True, text=True)
    if b.returncode != 0:
        first = (b.stderr or b.stdout).strip().splitlines()
        return False, f'build exit {b.returncode}: ' + (first[0] if first else '')
    notes = []
    ok = True
    for k in range(runs):
        r = subprocess.run(['timeout', '-s', 'KILL', '20', './prog'], cwd=d, env=env, capture_output=True)
        out = r.stdout.decode('utf-8', 'replace').split('\n')
        if out and out[-1] == '':
            out = out[:-1]
        if r.returncode != 0 or out != expected:
            ok = False
            notes.append(f'run {k + 1}: exit {r.returncode}, wrote {out!r}, stderr {r.stderr.decode("utf-8", "replace").strip()[:200]!r}')
    return ok, '; '.join(notes) if notes else f'{runs} run(s), exit 0, the expected lines'

verdicts = []
ok, note = build_and_run('base', src, BASE, 5)
print(f'base: {"right" if ok else "WRONG"} ({note})')
verdicts.append(ok)
for tag, old, new, expected in SUBS:
    n = src.count(old)
    if n != 1:
        print(f'{tag}: the edit {old!r} matches {n} times, not once: made by hand or not made')
        verdicts.append(False)
        continue
    ok, note = build_and_run(tag, src.replace(old, new), expected, 1)
    print(f'{tag}: {"right" if ok else "WRONG"} ({note})')
    verdicts.append(ok)
print(f'RIGHT={all(verdicts)}')
