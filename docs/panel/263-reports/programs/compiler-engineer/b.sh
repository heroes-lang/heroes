#!/bin/bash
# b.sh <compiler> <dir> <program> <runtime> <outdir>: emit, rewrite every thunk of shape `int64_t f(int64_t)` to run
# its function through hero_callback_run (route (b)), build plain and --sanitize at both levels.
set -u
C=$1; D=$2; P=$3; RT=$4; O=$5
mkdir -p "$O"; cp "$D"/*.h "$O"/ 2>/dev/null
( cd "$D" && "$C" build "$P.hero" --emit-c -o "$O/$P-emit.c" ) || { echo "emit $P failed"; exit 1; }
python3 - "$O/$P-emit.c" "$O/$P-b.c" <<'PY'
import re, sys
t = open(sys.argv[1]).read()
pat = re.compile(r'(__attribute__\(\(__noinline__\)\) int64_t (h_0cb_\w+)\((int64_t \w+(?:, int64_t \w+)*)\) \{\n    hero_callback_entered\([^\n]*\);\n)    int64_t hero_result = (h_\w+)\(([^)]*)\);\n')
n = 0
def sub(m):
    global n; n += 1
    head, thunk, params, real = m.group(1), m.group(2), m.group(3), m.group(4)
    names = [p.split()[1] for p in params.split(', ')]
    fields = ' '.join('int64_t a%d;' % i for i in range(len(names)))
    call = ', '.join('hero_x->a%d' % i for i in range(len(names)))
    init = ', '.join(names)
    return ('struct hero_x_%s { %s int64_t r; };\nstatic void hero_run_%s(void *hero_p) { struct hero_x_%s *hero_x = hero_p; hero_x->r = %s(%s); }\n' % (thunk, fields, thunk, thunk, real, call)
            + head + '    struct hero_x_%s hero_x = {%s, 0};\n    hero_callback_run(hero_run_%s, &hero_x);\n    int64_t hero_result = hero_x.r;\n' % (thunk, init, thunk))
t = pat.sub(sub, t)
open(sys.argv[2], 'w').write(t)
print("%s: %d thunks switched" % (sys.argv[1].split('/')[-1], n))
PY
H=$(cd "$(dirname "$0")" && pwd)
for lv in O0 O2; do
  "$H/hb.sh" "$O/$P-b.c" "$RT" $lv "$O/$P-$lv" > "$O/build-$P-$lv.txt" 2>&1 || echo "build $P $lv failed"
  "$H/hb.sh" "$O/$P-b.c" "$RT" $lv "$O/$P-san-$lv" -fsanitize=address,undefined -fno-sanitize=function > "$O/build-$P-san-$lv.txt" 2>&1 || echo "build $P san $lv failed"
done
