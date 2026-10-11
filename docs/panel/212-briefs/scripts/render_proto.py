#!/usr/bin/env python3
"""A SKETCH of `heroes issues render`, written by the panel-212 coordinator on 2026-10-11 so
that the samples can be regenerated and audited. It is NOT the rule and NOT a design: it
follows `.claude/rules/records.md` section "The issues" as written plus the readings listed
at the foot of this docstring, each of which is a gap in the rule the panel is asked to rule.

Usage: python3 -I render_proto.py <issue file relative to the repo root> [--repo <root>]

Readings the rule does not give (gaps D1 to D7 of 00-shared.md):
  title   a `# ` heading; else a defect's bold first field; else the item's second field; else
          (an old log row) its second field; the `Defect NNN:` prefix is added only when the
          title does not already open with `Defect`.
  closed  the day the body says `**Closed YYYY-MM-DD**` or `Closed YYYY-MM-DD`; else the author
          date of the commit the card names; else the card's `filed`. The source is printed.
  hash    a card's `self` is the first commit that touched the file (`git log --follow`).
  fields  an item line is split on ` | ` outside code spans.
"""
import re, sys, subprocess, pathlib, json
ROOT = pathlib.Path(__file__).resolve().parents[4]
if "--repo" in sys.argv: ROOT = pathlib.Path(sys.argv[sys.argv.index("--repo") + 1])
def git(*a): return subprocess.run(["git", "-C", str(ROOT)] + list(a), capture_output=True, text=True).stdout.strip()
def split_fields(line):
    out, cur, tick = [], "", False
    i = 0
    while i < len(line):
        if line[i] == "`": tick = not tick
        if not tick and line[i:i+3] == " | ": out.append(cur); cur = ""; i += 3; continue
        cur += line[i]; i += 1
    out.append(cur); return out
def unwrap_hashes(text):
    return re.sub(r'`([0-9a-f]{7,40})`', r'\1', text)
def render(rel):
    t = (ROOT / rel).read_text(encoding="utf-8")
    head, body = t.split("\n---\n", 1)
    card = dict(re.findall(r'^(\w+): (.*)$', head.replace("---\n", "", 1), re.M))
    body = body.lstrip("\n"); rows = body.split("\n")
    kind = card["kind"]
    item_i = next((i for i, r in enumerate(rows) if re.match(r'^- \[[ x]\] ', r)), None)
    heading = next((r[2:].strip() for r in rows if r.startswith("# ")), None)
    logrow = next((r for r in rows if re.match(r'^\d{4}-\d{2}-\d{2} \| ', r)), None)
    fields = split_fields(re.sub(r'^- \[[ x]\] ', '', rows[item_i])) if item_i is not None else []
    first = fields[0].strip("* ") if fields else ""
    notes = []
    if heading: title = heading
    elif item_i is not None and kind == "defect": title = first
    elif len(fields) > 1: title = fields[1].strip()
    elif item_i is not None: title = first; notes.append("single-field item: the bold first field was used")
    elif logrow: title = split_fields(logrow)[1].strip(); notes.append("old log row: its second field was used (the rule is silent)")
    else: title = ""; notes.append("NO TITLE SOURCE")
    if kind == "defect" and not re.match(r'^Defects? ', title):
        m = re.match(r'^(\d{3}) (?:—|:) (.*)$', title)
        title = f"Defect {m.group(1)}: {m.group(2)}" if m else title
        if not m: notes.append("defect with no number: no `Defect NNN:` prefix, no field value")
    labels = [f"kind:{kind}"]
    c = re.search(r'\*\*class: (\w+)\*\*', t)
    if kind == "defect": labels.append(f"class:{c.group(1)}" if c else "(no class)")
    labels.append(f"area:{card['area']}" if card.get("area", "none") != "none" else "(area none: no label)")
    h = card["commit"]
    if h == "self":
        h = (git("log", "--follow", "--diff-filter=A", "--format=%H", "--", rel).split("\n") or [""])[-1] or git("log", "--follow", "--format=%H", "--", rel).split("\n")[-1]
    state = "open" if re.search(r'^- \[ \] ', t, re.M) else "closed"
    cd = re.search(r'\*\*Closed (\d{4}-\d{2}-\d{2})|^Closed (\d{4}-\d{2}-\d{2})', t, re.M)
    if cd: closed, src = (cd.group(1) or cd.group(2)), "the body's Closed line"
    elif len(h) == 40: closed, src = git("log", "-1", "--format=%cs", h), "the card's commit date"
    else: closed, src = card["filed"], "the card's filed (no other source)"
    out = [f"Filed {card['filed']} · closed {closed}" if state == "closed" else f"Filed {card['filed']} · open", ""]
    sect = None
    def push(h_, text):
        out.append(f"### {h_}"); out.append(text.rstrip()); out.append("")
    if item_i is not None:
        if kind == "decision": push("The question", fields[1].strip() if len(fields) > 1 else first)
        elif kind == "defect": push("What happens", fields[1].strip() if len(fields) > 1 else "")
        else: push("What it delivers", fields[1].strip() if len(fields) > 1 else first)
        if len(fields) > 2: push("Where to look", fields[2].strip())
    elif logrow: push("The question", title)
    # the body: label paragraphs and ## sections become headings
    i = (item_i + 1) if item_i is not None else 0
    para, cur_h = [], None
    def flush():
        nonlocal para
        if para and any(p.strip() for p in para): push(cur_h or "Notes", unwrap_hashes("\n".join(p[4:] if p.startswith("    ") else p for p in para)))
        para = []
    rows2 = rows[i:]
    for r in rows2:
        s = r.strip()
        m = re.match(r'^\*\*(Origin|Recommendation|Ratified[^*]*|Verdict[^*]*)\*\*:?,?\s*(.*)$', s)
        mc = re.match(r'^\*\*Class: (\w+)\*\*,?\s*(.*)$', s)
        if re.match(r'^## (.*)$', s):
            flush(); cur_h = re.match(r'^## (.*)$', s).group(1); continue
        if m: flush(); cur_h = "Origin" if m.group(1) == "Origin" else ("Recommendation" if m.group(1) == "Recommendation" else "Verdict"); para = ["    " + m.group(2)]; continue
        if mc: flush(); cur_h = "Class"; para = ["    " + mc.group(1) + ", " + mc.group(2)]; continue
        if s == "" and para and cur_h in (None,): flush(); cur_h = None; continue
        para.append(r)
    flush()
    out.append(f"Record: https://github.com/heroes-lang/heroes/blob/<the commit the run is pinned to>/{rel}")
    comment = ((f"Fixed by {h}" if kind == "defect" else f"Settled by {h}") if state == "closed" and len(h) == 40 else None)
    return {"title": title, "labels": labels, "milestone": card["milestone"], "state": state, "notes": notes,
            "closed_source": src, "body": "\n".join(out), "closing_comment": comment}
if __name__ == "__main__":
    r = render(sys.argv[1])
    sys.stdout.write(f"# {r['title']}\n\n**labels**: {', '.join(r['labels'])} | **milestone**: {r['milestone']} | **state**: {r['state']}\n")
    for n in r["notes"]: sys.stdout.write(f"\n> sketch note: {n}\n")
    sys.stdout.write(f"\n---\n\n{r['body']}\n\n---\n**closing comment**: {r['closing_comment']}\n\n> sketch note: the closed day came from {r['closed_source']}\n")
