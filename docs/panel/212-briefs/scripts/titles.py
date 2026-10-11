#!/usr/bin/env python3
"""The title statistics of 00-shared.md F3, F4 and the rule gaps D1-D7, over every card under
issues/, reproducible: `python3 -I titles.py` from the repository root. Written 2026-10-11 by the
panel-212 coordinator after the completeness critic found the first run unsaved."""
import re, glob, collections, statistics
files = sorted(glob.glob("issues/*/*/*.md"))
def split_fields(line):
    out, cur, tick, i = [], "", False, 0
    while i < len(line):
        if line[i] == "`": tick = not tick
        if not tick and line[i:i+3] == " | ": out.append(cur); cur = ""; i += 3; continue
        cur += line[i]; i += 1
    out.append(cur); return out
rows = []
for f in files:
    t = open(f, encoding="utf-8").read()
    head, body = t.split("\n---\n", 1)
    card = dict(re.findall(r'^(\w+): (.*)$', head.replace("---\n", "", 1), re.M))
    lines = body.lstrip("\n").split("\n")
    item = next((l for l in lines if re.match(r'^- \[[ x]\] ', l)), None)
    heading = next((l[2:].strip() for l in lines if l.startswith("# ")), None)
    logrow = next((l for l in lines if re.match(r'^\d{4}-\d{2}-\d{2} \| ', l)), None)
    kind = card["kind"]
    flds = split_fields(re.sub(r'^- \[[ x]\] ', '', item)) if item else []
    if heading: src, ti = "heading", heading
    elif item and kind == "defect": src, ti = "defect item", flds[0].strip("* ")
    elif len(flds) > 1: src, ti = "item second field", flds[1].strip()
    elif item: src, ti = "single-field item (no second field)", ""
    elif logrow: src, ti = "old log row", split_fields(logrow)[1].strip()
    else: src, ti = "none", ""
    rows.append((f, kind, src, ti, flds[0].strip("* ") if flds else ""))
print("cards:", len(rows))
print("title source:", dict(collections.Counter(r[2] for r in rows)))
no_title_rule = [r for r in rows if r[2] in ("old log row", "none", "single-field item (no second field)")]
print("no title by the rule as written (log rows count as none):", len(no_title_rule), dict(collections.Counter(r[2] for r in no_title_rule)))
titled = [r for r in rows if r[3]]
print("titled (log rows titled by their second field):", len(titled), "| distinct:", len({r[3] for r in titled}))
sh = collections.Counter(r[3] for r in titled); shared = {k: v for k, v in sh.items() if v > 1}
print("titles shared:", len(shared), "covering", sum(shared.values()), "cards; most:", [(k[:40], v) for k, v in sh.most_common(3)])
rule = [r for r in rows if r[2] in ("heading", "defect item", "item second field")]
L = [len(r[3]) for r in rule]
print("cards titled by the rule as written:", len(rule), "| median length", statistics.median(L), "| max", max(L), "| over 256:", sum(l > 256 for l in L), "| over 150:", sum(l > 150 for l in L))
print("with log rows titled too: over 256:", sum(len(r[3]) > 256 for r in titled), "of", len(titled))
sec = [r for r in rows if r[2] == "item second field"]
print("second-field titles: start with a backtick", sum(r[3].startswith("`") for r in sec), "| of those holding a '/' or '.' (a path)", sum(r[3].startswith("`") and bool(re.search(r'[/.]', r[3].split("`")[1] if r[3].count("`") >= 2 else "")) for r in sec), "| contain docs/ issues/ selfhost/ tests/:", sum(bool(re.search(r'(docs|issues|selfhost|tests)/', r[3])) for r in sec))
d = [r for r in rows if r[1] == "defect"]
print("defect cards:", len(d), "| heading already opening 'Defect': ", sum(r[2] == "heading" and bool(re.match(r'^Defects? ', r[3])) for r in d), "of", sum(r[2] == "heading" for r in d), "| with no NNN in the first field and none in the name:", sum(not re.match(r'^(Defects? )?\d{3}', r[4]) and 'defect-' not in r[0].split('/')[-1] for r in d))
