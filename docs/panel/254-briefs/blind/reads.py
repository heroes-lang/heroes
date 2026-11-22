# After the run, at no cost (panel 254's critic, finding 25): whether a session's own tool calls named a
# path outside its folder. `scan.py` reads what reached a session; this reads what the session asked for:
# every `tool_use` of its `assistant` entries, and every path its input names (`file_path`, `path`,
# `notebook_path`), each resolved against the entry's `cwd`. Prints one line per transcript: its folder,
# the session's cwd's last part, how many tool calls, and how many named a path outside the cwd; never a
# transcript's text. A transcript holding no entry with a `cwd` is reported as unreadable, never as clean.
# usage (from the blind area): python3 -I tools/reads.py [projects-folder]
import glob, json, os, sys
root = sys.argv[1] if len(sys.argv) > 1 else 'home/.claude/projects'
KEYS = ('file_path', 'path', 'notebook_path')
rows, outside_sessions, unreadable = 0, 0, 0
for p in sorted(glob.glob(os.path.join(root, '*', '*.jsonl'))):
    folder = os.path.basename(os.path.dirname(p))
    cwd, calls, outside = None, 0, 0
    for line in open(p, errors='replace'):
        try:
            e = json.loads(line)
        except Exception:
            continue
        if isinstance(e.get('cwd'), str) and cwd is None:
            cwd = os.path.realpath(e['cwd'])
        if e.get('type') != 'assistant':
            continue
        m = e.get('message')
        content = m.get('content') if isinstance(m, dict) else None
        if not isinstance(content, list):
            continue
        for c in content:
            if not isinstance(c, dict) or c.get('type') != 'tool_use':
                continue
            calls += 1
            inp = c.get('input') or {}
            for k in KEYS:
                v = inp.get(k)
                if isinstance(v, str) and v:
                    base = cwd or ''
                    full = os.path.realpath(v if os.path.isabs(v) else os.path.join(base, v))
                    if not base or not (full == base or full.startswith(base + os.sep)):
                        outside += 1
    rows += 1
    if cwd is None:
        unreadable += 1
    if outside:
        outside_sessions += 1
    print(f"{folder[-40:]} cwd={os.path.basename(cwd) if cwd else 'UNREADABLE'} calls={calls} outside={outside}")
print(f"{rows} transcripts read; sessions with a path outside their folder: {outside_sessions}; without a cwd: {unreadable}")
