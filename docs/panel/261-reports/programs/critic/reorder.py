# The critic: put the report's numbered sections in order, the closing one last.
import re, sys
p = sys.argv[1]
s = open(p).read()
parts = re.split(r'(?m)^(?=## )', s)
head, secs = parts[0], parts[1:]
def key(t):
    m = re.match(r'## (\d+)\.', t)
    return (0, int(m.group(1))) if m else (1, 0)
secs.sort(key=key)
out = head + ''.join(x if x.endswith('\n\n') else x.rstrip('\n') + '\n\n' for x in secs)
open(p, 'w').write(out.rstrip('\n') + '\n')
print([re.match(r'## ([^\n]{0,40})', t).group(1) for t in secs])
