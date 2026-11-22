# After the run, at no cost: each session's transcript says whether the leaks stayed shut. Prints one
# line per transcript: its folder; whether the first user entry is the runner's prompt alone (no standard
# input); whether a git status attachment names a commit, or a person other than `reader`; and whether
# any text of CLAUDE.md or a memory index reached it. Prints flags and counts only, never a transcript's
# text.
#
# It reads ONLY what reached the session, never what the session wrote itself (panel 253's critic,
# finding 4: two of panel 251's sessions were flagged for `CLAUDE.md` by their own `context` answers):
# every entry of type `assistant` is skipped, and so is a request blob whose message's role is
# `assistant` (the session's earlier turns resent); of a `user` entry it reads `message` (the prompt, a
# reminder, a tool's result as the session received it) and never `toolUseResult`, the CLI's structured
# copy, which for a write holds the text the session wrote.
#
# usage (from the blind area): python3 -I tools/scan.py [projects-folder]
#   the default folder is home/.claude/projects; the control on panel 251's 51 sessions is
#   python3 -I tools/scan.py /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-llm-ergonomist/transcripts
import glob, json, os, re, sys
PROMPT = "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English."
root = sys.argv[1] if len(sys.argv) > 1 else 'home/.claude/projects'
FOREIGN = ('CLAUDE.md', 'MEMORY.md', 'Memory index')
rows = 0
tally = {'prompt-alone=NO': 0, 'git-status=LEAK': 0, 'claude-md-or-memory=YES': 0}
for p in sorted(glob.glob(os.path.join(root, '*', '*.jsonl'))):
    folder = os.path.basename(os.path.dirname(p))
    first_user, git_bad, git_seen, foreign = None, False, False, False
    for line in open(p, errors='replace'):
        try:
            e = json.loads(line)
        except Exception:
            continue
        t = e.get('type')
        if t == 'assistant':
            continue
        if t == 'api-request-blob' and isinstance(e.get('message'), dict) and e['message'].get('role') == 'assistant':
            continue
        if t == 'user':
            seen = {k: v for k, v in e.items() if k != 'toolUseResult'}
            if first_user is None:
                m = e.get('message', {})
                c = m.get('content') if isinstance(m, dict) else None
                first_user = c if isinstance(c, str) else json.dumps(c)
        else:
            seen = e
        s = json.dumps(seen)
        if 'gitStatus' in s:
            git_seen = True
            g = s[s.find('gitStatus'):]
            if 'Git user: reader' not in g or re.search(r'Recent commits:\\n[0-9a-f]{7}', g):
                git_bad = True
        if any(w in s for w in FOREIGN):
            foreign = True
    rows += 1
    flags = [f"prompt-alone={'yes' if first_user == PROMPT else 'NO'}",
             f"git-status={'none' if not git_seen else ('LEAK' if git_bad else 'reader, no commit')}",
             f"claude-md-or-memory={'YES' if foreign else 'no'}"]
    for f in flags:
        if f in tally:
            tally[f] += 1
    print(f"{folder[-40:]} " + ' '.join(flags))
print(f"{rows} transcripts read; " + ', '.join(f'{k}: {v}' for k, v in tally.items()))
