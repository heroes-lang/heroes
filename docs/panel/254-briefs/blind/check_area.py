# Before any spend, the three repairs of panel 251's leaks (defect 2589), asked of the area itself:
# 1. the area is the top of an empty git repository of its own (no commit, no ref), whose .gitignore
#    holds `*` alone and whose local user is `reader` <reader@invalid>, so the status the CLI renders
#    into a session names no commit and no person;
# 2. the sessions' HOME, `home/`, stands inside the area, so the CLI's transcripts land there and never
#    under the user's ~/.claude/projects/;
# 3. run.sh starts every session with its standard input from /dev/null (read in run.sh's own text);
# 4. and, since panel 253's critic's finding 5, with TMPDIR and CLAUDE_CODE_TMPDIR naming the area's tmp/,
#    which stands (whether the CLI then writes nothing under /private/tmp/claude-<uid>/ is unrun: the
#    runner's free preflight asks it of one session, and the runner asks it again after every session).
# usage (from the blind area): python3 -I tools/check_area.py
import os, subprocess, sys
here = os.getcwd()
bad = []
def git(*a):
    r = subprocess.run(['git', '-C', here, *a], capture_output=True, text=True)
    return r.returncode, r.stdout.strip()
rc, top = git('rev-parse', '--show-toplevel')
if rc != 0 or os.path.realpath(top) != os.path.realpath(here):
    bad.append(f'the area is not the top of its own git repository (toplevel: {top!r})')
rc, refs = git('for-each-ref')
if refs:
    bad.append('the area repository holds a ref')
rc, _ = git('rev-parse', '--verify', '-q', 'HEAD')
if rc == 0:
    bad.append('the area repository holds a commit')
if git('config', '--local', 'user.name')[1] != 'reader' or git('config', '--local', 'user.email')[1] != 'reader@invalid':
    bad.append('the area repository user is not reader <reader@invalid>')
gi = os.path.join(here, '.gitignore')
if not os.path.isfile(gi) or open(gi).read().strip() != '*':
    bad.append('.gitignore does not hold `*` alone')
rc, st = git('status', '--porcelain', '--ignored=no')
if st:
    bad.append(f'git status is not clean: {st[:200]!r}')
if not os.path.isdir(os.path.join(here, 'home')):
    bad.append('home/ is missing')
run = open(os.path.join(here, 'run.sh')).read()
if '< /dev/null' not in run or 'HOME="$HERE/home"' not in run:
    bad.append('run.sh does not give every session /dev/null and HOME="$HERE/home"')
if 'TMPDIR="$HERE/tmp" CLAUDE_CODE_TMPDIR="$HERE/tmp"' not in run:
    bad.append('run.sh does not give every session TMPDIR and CLAUDE_CODE_TMPDIR inside the area')
if not os.path.isdir(os.path.join(here, 'tmp')):
    bad.append('tmp/ is missing')
for b in bad:
    print(b)
sys.exit(2 if bad else 0)
