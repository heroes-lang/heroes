---
kind: defect
area: process
milestone: none
filed: 2026-10-11
commit: a1ec2eb995098507b4d60523e82acfb367befccb
github: none
---

- [ ] **670 — the key guard admits any request whose command names the token count anywhere** | CodeQL alert 4 (`py/incomplete-url-substring-sanitization`, high), and behind it a hole: `guard_bash.py` refused a client's segment naming `api.anthropic.com` unless `/v1/messages/count_tokens` appeared ANYWHERE in it. Measured on the trunk's guard: a request to `/v1/messages` carrying the endpoint's words in its body passed, a second URL after a legitimate one (`/v1/organizations`) passed, and `api.anthropic.com.example.org/v1/messages/count_tokens`, a host that would receive the key, passed | `.claude/hooks/guard_bash.py:735` · **class: blocking**

    **Origin:** the author, 2026-10-11, sending the repository's code-scanning page at 12:47 with *check these too*; the coordinator read the eight open alerts the same minute with `gh api 'repos/heroes-lang/heroes/code-scanning/alerts?state=open'`, opened every location, and wrote each repair in lane b22-codeql.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): the guard is what holds the Anthropic key to counting tokens (CLAUDE.md § Hard stops' settings), and three requests it exists to refuse passed it.

    Repaired at `a1ec2eb995098507b4d60523e82acfb367befccb`, 2026-10-11 (lane b22-codeql). `ANTHROPIC_HOST` finds each mention of the host, case-blind, with the characters that follow it to the end of the word, and `ENDPOINT_ONLY` admits the endpoint alone, with a query or a fragment; any other follower, a port, a longer host, another path, is refused, the guard erring toward refusing. The three requests above, through the lane's guard: refused; the legitimate count, passed. `test_the_anthropic_key_reaches_the_token_count_and_nothing_else` in `.claude/hooks/test_hooks.py` holds eleven commands, three that pass (the count, the count with `?beta=true`, a `grep` for the host) and eight refused. The hooks' own tests: 156, OK. Not seen, as before: a host assembled from pieces or held in a variable; the guard is a tripwire, not a sandbox.
