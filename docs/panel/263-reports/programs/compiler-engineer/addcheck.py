# addcheck.py <in.c> <out.c> <module.function>...: put hero_stack_check("<m.f>") first in each named Heroes
# function's body (the line `#line` before it names the .hero; a definition opens `<type> h_<m>_<f>(...) {`), after
# hero_thread_guard where one stands: (c2)'s set applied by hand to an emission of another platform.
import re, sys
src, out, names = sys.argv[1], sys.argv[2], sys.argv[3:]
t = open(src).read()
n = 0
for who in names:
    m, f = who.split('.')
    cm = m.replace('_', '')
    pat = re.compile(r'^((?:int64_t|void|_Bool|double) h_%s_%s\([^)]*\) \{ (?:hero_thread_guard\("[^"]*"\); )?)' % (re.escape(cm), re.escape(f)), re.M)
    t, k = pat.subn(lambda mo: mo.group(1) + 'hero_stack_check("%s"); ' % who, t, count=1)
    n += k
open(out, 'w').write(t)
print("%s: %d of %d checked" % (src.split('/')[-1], n, len(names)))
