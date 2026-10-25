"""Panel 228's blind seat, scored by the rules `docs/panel/228-briefs/blind/README.md` registered
before the run. It reads each folder's `task.md` to know WHICH task it holds (the scores need the
holes' places and the expected output), never the shape, and never opens `key.txt`."""
import json, pathlib, re, subprocess, sys, tempfile, shutil
ROOT = pathlib.Path("/Users/joseph/Temp/heroes/heroes-lang/build")
SCRATCH = pathlib.Path(sys.argv[1])
HEROES = "/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rechecked-issues/heroes"
HOLES = {"f1": [("12:16", "Point"), ("13:14", "i64")], "f2": [("5:21", "Money")]}
OUT = {"w1": "27\n", "w2": "12.00 EUR\n"}
LIB = {"w1": SCRATCH / "w1" / "lib.hero", "w2": SCRATCH / "w2" / "lib.hero"}
def task_of(text):
    if "volume of a cube" in text: return "w1"
    if "12.00 EUR" in text: return "w2"
    if "637f215728e6" in text: return "f1"
    if "d8be077ebe52" in text: return "f2"
    raise SystemExit("unknown task")
SECTIONS = ("finished", "places", "program", "choice_points", "context")
def sections(text):
    marks = []
    for name in SECTIONS:
        m = re.search(r"(?im)^[#*\s`-]*" + name + r"[`*]*\s*[:*]*", text)
        if m: marks.append((m.start(), m.end(), name))
    marks.sort()
    out = {}
    for i, (start, end, name) in enumerate(marks):
        stop = marks[i + 1][0] if i + 1 < len(marks) else len(text)
        out[name] = text[end:stop]
    return out
def first_word(s):
    m = re.search(r"[A-Za-z]+", s or "")
    return m.group(0).lower() if m else ""
rows = []
for n in range(1, 81):
    folder = ROOT / "b" / str(n)
    task = task_of((folder / "task.md").read_text())
    row = {"n": n, "task": task, "void": "", "finished": False, "second": False, "cost": 0.0}
    path = ROOT / "b-out" / f"{n}.jsonl"
    events = [json.loads(l) for l in path.read_text().splitlines() if l.strip()] if path.exists() else []
    results = [e for e in events if e.get("type") == "result"]
    for e in events:
        if e.get("type") == "assistant":
            for c in e["message"].get("content", []):
                if c.get("type") == "tool_use":
                    p = str(c.get("input", {}).get("file_path", ""))
                    if c.get("name") != "Read" or not p.startswith(str(folder) + "/"):
                        row["void"] = f"tool {c.get('name')} {p}"
    if not results or results[-1].get("subtype") != "success":
        row["void"] = row["void"] or "no answer: " + (results[-1].get("subtype", "none") if results else "none")
        rows.append(row); continue
    row["cost"] = results[-1].get("total_cost_usd") or 0.0
    text = results[-1].get("result", "")
    (SCRATCH / "answers").mkdir(exist_ok=True)
    (SCRATCH / "answers" / f"{n}.md").write_text(text)
    s = sections(text)
    row["context"] = " ".join((s.get("context") or "").split())[:300]
    row["finished"] = first_word(s.get("finished")) == "no"
    if task.startswith("f"):
        places = s.get("places") or ""
        lines = places.splitlines()
        row["second"] = all(any(at in l and ty in l for l in lines) for at, ty in HOLES[task])
    else:
        blocks = [b for b in re.findall(r"```[^\n]*\n(.*?)```", text, re.S) if "function main" in b]
        if blocks:
            work = pathlib.Path(tempfile.mkdtemp(prefix=f"score228-{n}-", dir=SCRATCH))
            (work / "main.hero").write_text(blocks[-1])
            shutil.copyfile(LIB[task], work / "lib.hero")
            checked = subprocess.run([HEROES, "check", "main.hero"], cwd=work, capture_output=True, text=True, timeout=120)
            row["check"] = checked.returncode
            if checked.returncode == 0:
                ran = subprocess.run([HEROES, "run", "main.hero"], cwd=work, capture_output=True, text=True, timeout=120)
                row["second"] = ran.returncode == 0 and ran.stdout == OUT[task]
                row["printed"] = ran.stdout[:40]
            else:
                row["refused"] = (checked.stdout + checked.stderr).strip().splitlines()[0][:160] if (checked.stdout + checked.stderr).strip() else ""
        else:
            row["check"] = "no program"
    rows.append(row)
(SCRATCH / "scores.json").write_text(json.dumps(rows, indent=1))
print("scored", len(rows), "voids", sum(1 for r in rows if r["void"]), "cost", round(sum(r["cost"] for r in rows), 4))
