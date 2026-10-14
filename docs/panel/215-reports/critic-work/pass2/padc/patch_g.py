# Critic, panel 215 pass 2: insert a form of route G before each lend of the
# record to get_h/set_h in an emitted unit. usage: patch_g.py <form> <in.c> <out.c> <ctype> <field>...
#  naive: the compiler-engineer's runtime call, spans = every DECLARED field (padding.hero without `vouched`)
#  gaps:  the ffi-pragmatist's common.pl macro, generalised: the gap between each two consecutive declared fields
import sys, re
form, src, dst, ct, fields = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5:]
s = open(src).read()
def stmt(place):
    if form == 'naive':
        pairs = ", ".join(f"offsetof({ct}, {f}), sizeof((({ct} *)0)->{f})" for f in fields)
        return f"{{ static const size_t hero_pad_spans[] = {{{pairs}}}; hero_pad_zero({place}, sizeof({ct}), hero_pad_spans, {len(fields)}); }}"
    out = []
    for a, b in zip(fields, fields[1:]):
        out.append(f"memset((char *)({place}) + offsetof({ct}, {a}) + sizeof(({place})->{a}), 0, offsetof({ct}, {b}) - (offsetof({ct}, {a}) + sizeof(({place})->{a})));")
    return " ".join(out)
n = 0
def rep(m):
    global n; n += 1
    return m.group(1) + stmt(m.group(3)) + "\n" + m.group(1) + m.group(2)
s = re.sub(r"^(\s*)((?:\(void\)|t\d+ = )(?:get_h|set_h)\((&h\d+_v)\);)$", rep, s, flags=re.M)
s = s.replace('#include "heroes_standard.h"\n', '#include "heroes_standard.h"\n#include <stddef.h>\n#include <string.h>\n', 1)
open(dst, 'w').write(s)
print(f"{form} {src}: {n} lends patched")
