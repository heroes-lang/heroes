"""Each replacement of make_adopted.py applied alone to the base, one file each."""
import runpy, sys
src = open("make_adopted.py", encoding="utf-8").read()
ns = {}
exec(compile(src.split("REPLACEMENTS = [", 1)[0].replace("base_path, out_path = sys.argv[1], sys.argv[2]\ntext = open(base_path, encoding=\"utf-8\").read()\n", ""), "h", "exec"), ns)
reps = eval("[" + src.split("REPLACEMENTS = [", 1)[1].split("\n]\n", 1)[0] + "]")
base = open("base.md", encoding="utf-8").read()
for i, (old, new) in enumerate(reps):
    assert base.count(old) == 1
    open(f"piece-{i+1}.md", "w", encoding="utf-8").write(base.replace(old, new))
print(len(reps))
