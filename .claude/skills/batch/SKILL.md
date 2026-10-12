---
name: batch
description: Compose the next batch of defect repairs from GitHub's open defects, sixteen to sixty-four in the batch's order, one lane per cluster of shared files, and print it for the coordinator. Read-only - it files, closes, comments on and relabels nothing, and opens no lane. Use when a round of lanes is about to open, or when the author asks what the next batch would hold (they ask in Italian: "prossimo batch", "componi il batch", "quali difetti nel batch").
---

# /batch, the next batch composed from the open defects

Asked for by the author on 2026-10-12, beside the move of the issues to GitHub
(the author's answers of that day): a skill that reads the open issues and
composes a batch. **Its rules are not written here**: the batch's size and
order are `.claude/rules/verification.md` § The batch, the classes and what a
`systemic` defect is owed § Bounded discovery, and the lane per cluster of
shared files `.claude/rules/records.md` § Working in lanes. This file is the
procedure that applies them, and where it and they disagree, they win and this
file is corrected.

**It writes nothing in the tree and nothing on GitHub**: no issue filed, closed,
commented on or relabelled, no worktree, no commit. What it prints goes to the
coordinator, who opens the lanes.

## 1. Read

From the repository's root, into the session's scratchpad (`$S`). Every input
is read in this session (CLAUDE.md § RUN IT), never carried from an earlier
batch's plan:

```sh
S=<the scratchpad>; mkdir -p "$S"
./heroes run tests/harness/issues.hero -- open --kind defect --json > "$S/defects.json"
git ls-files > "$S/tracked"
git tag --list 'm-*' --sort=creatordate --format='%(creatordate:short) %(refname:short)' > "$S/tags"
```

## 2. Compose

What the program below does, in the rules' own order:

- **each open defect** is read for its name (the number its title opens with,
  `Defect <n>:`, else its issue number), its class (its `class:` label), the
  day it was classed (its `**Class: <name>**, YYYY-MM-DD` line, else the day it
  was opened) and **the files its *Where to look* section names**: every token
  that is a tracked file's path, or the tail of exactly one (`check/marks.hero`
  for `selfhost/check/marks.hero`), a `:NNN` line suffix taken off. A directory is not a file, since a word like
  `spec` or `runtime` would join every defect that writes it; a body with no
  such section, an issue of an older shape, is read whole for them;
- **an `adjacent` defect two `m-*` tags old counts as `blocking`**, the tags
  counted from the day it was classed (`.claude/rules/records.md` § The lists),
  a tag on that same day counted too, the safe direction; `records/tagged` is
  the judge, this only the order;
- **a `systemic` defect is left out** and listed apart: it goes to a sitting or
  to the author, never to a lane (§ Bounded discovery);
- **the order**: the `blocking` defects; then the `adjacent` and `improvement`
  defects sharing a file with one of them; then the remaining `adjacent`,
  oldest first; then the remaining `improvement`, oldest first; cut at
  sixty-four, and every one taken when fewer than sixteen are open (§ The
  batch);
- **the lanes**: two defects of the batch share a lane when they name a file
  in common, and so on transitively; one lane per such cluster, its defects in
  the batch's order.
  The clusters' file sets are disjoint by construction, so the lanes may run in
  parallel; a defect naming no tracked file is a lane of its own, said so,
  whose files are found before it opens.

```sh
python3 -I - "$S" <<'PY'
import json, re, sys
S = sys.argv[1]
raw = open(S + "/defects.json", encoding="utf-8").read()
try:
    items = json.loads(raw)
except ValueError:
    items = [json.loads(line) for line in raw.splitlines() if line.strip()]
if isinstance(items, dict):
    items = items.get("issues", [])
tracked = set(open(S + "/tracked", encoding="utf-8").read().splitlines())
tails = {}
for p in tracked:
    parts = p.split("/")
    for i in range(1, len(parts)):
        tails.setdefault("/".join(parts[i:]), []).append(p)
tags = [l.split()[0] for l in open(S + "/tags", encoding="utf-8") if l.strip()]

def field(d, *names):
    for n in names:
        if d.get(n) is not None:
            return d[n]
    return ""

def files_of(body):
    m = re.search(r"^#+ Where to look\s*$(.*?)(?=^#+ |\Z)", body, re.M | re.S)
    text = m.group(1) if m else body
    out = set()
    for tok in re.findall(r"[A-Za-z0-9_./-]+", text):
        tok = re.sub(r":\d+(-\d+)?$", "", tok.strip("./:,;")).rstrip("/")
        if tok in tracked:
            out.add(tok)
        elif "." in tok.rsplit("/", 1)[-1] and len(tails.get(tok, [])) == 1:
            out.add(tails[tok][0])
    return out

defects = []
for d in items:
    labels = [l["name"] if isinstance(l, dict) else l for l in field(d, "labels") or []]
    cls = next((l.split(":", 1)[1] for l in labels if l.startswith("class:")), "none")
    title, body = field(d, "title"), field(d, "body")
    created = str(field(d, "created_at", "createdAt", "created"))[:10]
    named = re.match(r"Defects? (\d+)", title)
    classed = re.search(r"\*\*Class: [a-z]+\*\*, (\d{4}-\d{2}-\d{2})", body)
    day = classed.group(1) if classed else created
    if cls == "adjacent" and sum(1 for t in tags if t >= day) >= 2:
        cls = "blocking (aged)"
    defects.append(dict(issue=field(d, "number"), name=named.group(1) if named else str(field(d, "number")),
                        cls=cls, day=created, title=title, files=files_of(body)))

def shares(a, b):
    return bool(a["files"] & b["files"])

open_ = [d for d in defects if d["cls"] != "systemic"]
by_age = lambda ds: sorted(ds, key=lambda d: (d["day"], int(d["issue"] or 0)))
blocking = by_age([d for d in open_ if d["cls"].startswith("blocking")])
near = by_age([d for d in open_ if d not in blocking and d["cls"] in ("adjacent", "improvement")
               and any(shares(d, b) for b in blocking)])
rest_adj = by_age([d for d in open_ if d not in blocking + near and d["cls"] == "adjacent"])
rest_imp = by_age([d for d in open_ if d not in blocking + near + rest_adj])
order = blocking + near + rest_adj + rest_imp
batch = order if len(order) < 16 else order[:64]

lanes = []
for d in batch:
    joined = [l for l in lanes if d["files"] and any(shares(d, x) for x in l)]
    merged = [d] + [x for l in joined for x in l]
    lanes = [l for l in lanes if l not in joined] + [sorted(merged, key=batch.index)]

count = lambda c: sum(1 for d in batch if d["cls"].startswith(c))
print(f"batch: {len(batch)} of {len(defects)} open defects: {count('blocking')} blocking, "
      f"{count('adjacent')} adjacent, {count('improvement')} improvement; {len(lanes)} lanes")
for i, lane in enumerate(sorted(lanes, key=lambda l: batch.index(l[0])), 1):
    files = sorted(set().union(*(d["files"] for d in lane)))
    print(f"\nlane {i}, {len(lane)} defects, files: {' '.join(files) or '(none named: find them before it opens)'}")
    for d in lane:
        print(f"  #{d['issue']}  defect {d['name']}  [{d['cls']}, filed {d['day']}]  {d['title']}")
left = [d for d in defects if d not in batch]
if left:
    print("\nleft out:")
    for d in left:
        why = "systemic: a sitting or the author" if d["cls"] == "systemic" else "past sixty-four"
        print(f"  #{d['issue']}  defect {d['name']}  [{d['cls']}]  {why}")
PY
```

## 3. Tell

Print the program's output as it is, then, in the chat's language and plain
words, three sentences at most: how many defects and lanes, which lane holds
the `blocking` work, and what was left out and why. **It recommends nothing
beyond the composition**: whether the round opens, and when, is the
coordinator's, and a defect the coordinator moves between lanes is said in the
round's first commit.

## What this skill does not do

It does not file, close, comment on or relabel an issue, it does not open a
worktree or a lane, it does not run a suite, and it does not judge a class: a
defect that looks misclassed is reported as found, with its issue's URL, for
the coordinator or `/decide`.
