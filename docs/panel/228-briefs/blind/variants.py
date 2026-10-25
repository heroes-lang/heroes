"""Panel 228's blind variants, second design: four tasks, four shapes, one table.

Every hole's report is the lane's stage-2 compiler's own (`<task>/T.out`, `check --json main.hero`
run in the task's folder), less what two defects add, removed from all four shapes alike: defect
2203's own binding in scope (f1: `q: Point` in the first hole, `d: i64` in the second) and defect
2205's bare type (`Box` and `Money` written `lib.Box` and `lib.Money`). The `read` lines are the
real stderr document's, copied from `<task>/T.err`."""
import json, pathlib, sys
root = pathlib.Path(sys.argv[1]); out = pathlib.Path(sys.argv[2])
H = {
 "f1": [dict(line=12, col=16, expected="Point", scope=[("p", "Point", False)], fields=("Point", [("x", "i64"), ("y", "i64")]), functions=[]),
        dict(line=13, col=14, expected="i64", scope=[("p", "Point", False), ("q", "Point", False)], fields=None, functions=[("dist2(a: Point, b: Point)", None)])],
 "w1": [dict(line=5, col=22, expected="lib.Box", scope=[("edge", "i64", False)], fields=("lib.Box", [("width", "i64"), ("height", "i64"), ("depth", "i64")]),
             functions=[("lib.cube(side: i64)", "# Builds a cube whose edges all measure `side`."), ("lib.tile(width: i64, height: i64)", "# Builds a flat box, one unit deep.")])],
 "w2": [dict(line=5, col=21, expected="lib.Money", scope=[("price", "i64", False)], fields=("lib.Money", [("cents", "i64"), ("currency", "str")]),
             functions=[("lib.dollars(n: i64)", "# An amount in dollars."), ("lib.euros(n: i64)", "# An amount in euros."), ("lib.francs(n: i64)", "# An amount in Swiss francs."),
                        ("lib.pounds(n: i64)", "# An amount in pounds sterling."), ("lib.rupees(n: i64)", "# An amount in rupees.")])],
}
H["f2"] = H["w2"]
SRC = {"f1": "f1", "f2": "w2", "w1": "w1", "w2": "w2"}
q = lambda s: json.dumps(s, ensure_ascii=False)
def read_lines(t):
    err = (root / SRC[t] / "T.err").read_text().splitlines()
    start = err.index('  "read": [')
    end = err.index("  ],", start)
    return err[start:end + 1]
def text(t):
    rows = []
    for h in H[t]:
        rows.append(f"hole at main.hero:{h['line']}:{h['col']}")
        rows.append(f"  expected type: {h['expected']}")
        rows.append("  in scope:")
        rows += [f"    {n}: {ty}" + (" (mutable)" if m else "") for n, ty, m in h["scope"]]
        if h["fields"]:
            rows.append(f"  fields of {h['fields'][0]}: " + ", ".join(f"{n}: {ty}" for n, ty in h["fields"][1]))
        if h["functions"]:
            rows.append("  functions that return it:")
            for call, doc in h["functions"]:
                rows.append(f"    {call}")
                if doc:
                    rows.append(f"      {doc}")
    return "\n".join(rows) + "\n"
def hole_obj(h):
    r = ["    {", '      "file": "main.hero",', f'      "line": {h["line"]},', f'      "col": {h["col"]},', f'      "expected": {q(h["expected"])},']
    r.append('      "in_scope": [\n' + ",\n".join(f'        {{"name": {q(n)}, "type": {q(ty)}, "mutable": {"true" if m else "false"}}}' for n, ty, m in h["scope"]) + "\n      ],")
    r.append('      "more_in_scope": 0,')
    if h["fields"]:
        r.append('      "fields": [\n' + ",\n".join(f'        {{"name": {q(n)}, "type": {q(ty)}}}' for n, ty in h["fields"][1]) + "\n      ],")
    else:
        r.append('      "fields": [],')
    if h["functions"]:
        r.append('      "functions": [\n' + ",\n".join(f'        {{"call": {q(c)}, "doc": {q(d[2:] if d else None)}}}' for c, d in h["functions"]) + "\n      ]")
    else:
        r.append('      "functions": []')
    r.append("    }")
    return "\n".join(r)
def note_list(h):
    notes = ["in scope: " + ", ".join(f"{n}: {ty}" + (" (mutable)" if m else "") for n, ty, m in h["scope"])]
    if h["fields"]:
        notes.append(f"fields of {h['fields'][0]}: " + ", ".join(f"{n}: {ty}" for n, ty in h["fields"][1]))
    if h["functions"]:
        notes.append("functions that return it: " + "; ".join(c + (f" ({d[2:]})" if d else "") for c, d in h["functions"]))
    return "[" + ", ".join(q(n) for n in notes) + "]"
def head(t, kind): return ["{", '  "schema": 2,', f'  "kind": "{kind}",'] + read_lines(t)
def doc_plain(t): return "\n".join(head(t, "diagnostics") + ['  "diagnostics": []', "}"]) + "\n"
def doc_a(t): return "\n".join(head(t, "diagnostics") + ['  "diagnostics": [],', '  "holes": [', ",\n".join(hole_obj(h) for h in H[t]), "  ]", "}"]) + "\n"
def doc_b(t): return "\n".join(head(t, "holes") + ['  "holes": [', ",\n".join(hole_obj(h) for h in H[t]), "  ]", "}"]) + "\n"
def doc_c(t):
    entries = []
    for h in H[t]:
        entries.append("\n".join(["    {", '      "code": "hole",', f'      "message": {q("a hole: expected `" + h["expected"] + "`")},', '      "file": "main.hero",',
                                  f'      "line": {h["line"]},', f'      "col": {h["col"]},', f'      "notes": {note_list(h)},', '      "fixes": []', "    }"]))
    return "\n".join(head(t, "diagnostics") + ['  "diagnostics": [', ",\n".join(entries), "  ]", "}"]) + "\n"
SHAPES = {"T": lambda t: (0, text(t), doc_plain(t)), "A": lambda t: (0, "", doc_a(t)), "B": lambda t: (0, doc_b(t), doc_plain(t)), "C": lambda t: (1, "", doc_c(t))}
INTENT = {"w1": "The program should print the volume of a cube whose edges all measure `edge`: `27`.",
          "w2": "The program should print the price, 12, as an amount in euros: `12.00 EUR`."}
def answer(t, shape):
    code, so, se = SHAPES[shape](t)
    rows = ["## What the compiler answered", "", f"`heroes check --json main.hero`, exit status {code}.", "", "Standard output:", ""]
    rows += ["```", so.rstrip("\n"), "```"] if so else ["(nothing)"]
    rows += ["", "Standard error:", "", "```", se.rstrip("\n"), "```", ""]
    return rows
for t in ("f1", "f2", "w1", "w2"):
    for shape in SHAPES:
        if t.startswith("f"):
            body = ["# The task", "", "A program, `main.hero`, was checked by the language's compiler. You cannot see the program; what the compiler answered is below.", ""]
        else:
            prog = (root / t / "main.hero").read_text().rstrip("\n")
            body = ["# The task", "", INTENT[t], "", "## The program, `main.hero`", "", "It uses a module, `lib.hero`, which you cannot see.", "", "```", prog, "```", ""]
        (out / f"{t}-{shape}.md").write_text("\n".join(body + answer(t, shape)))
