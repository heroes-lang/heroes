---
kind: defect
area: process
milestone: none
filed: 2026-10-11
commit: a1ec2eb995098507b4d60523e82acfb367befccb
github: none
---

- [x] **670 — the key guard admits any request whose command names the token count anywhere** | CodeQL alert 4 (`py/incomplete-url-substring-sanitization`, high), and behind it a hole: `guard_bash.py` refused a client's segment naming `api.anthropic.com` unless `/v1/messages/count_tokens` appeared ANYWHERE in it. Measured on the trunk's guard: a request to `/v1/messages` carrying the endpoint's words in its body passed, a second URL after a legitimate one (`/v1/organizations`) passed, and `api.anthropic.com.example.org/v1/messages/count_tokens`, a host that would receive the key, passed | `.claude/hooks/guard_bash.py:735` · **class: blocking**

    **Origin:** the author, 2026-10-11, sending the repository's code-scanning page at 12:47 with *check these too*; the coordinator read the eight open alerts the same minute with `gh api 'repos/heroes-lang/heroes/code-scanning/alerts?state=open'`, opened every location, and wrote each repair in lane b22-codeql.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): the guard is what holds the Anthropic key to counting tokens (CLAUDE.md § Hard stops' settings), and three requests it exists to refuse passed it.

    Repaired at `a1ec2eb995098507b4d60523e82acfb367befccb`, 2026-10-11 (lane b22-codeql). `ANTHROPIC_HOST` finds each mention of the host, case-blind, with the characters that follow it to the end of the word, and `ENDPOINT_ONLY` admits the endpoint alone, with a query or a fragment; any other follower, a port, a longer host, another path, is refused, the guard erring toward refusing. The three requests above, through the lane's guard: refused; the legitimate count, passed. `test_the_anthropic_key_reaches_the_token_count_and_nothing_else` in `.claude/hooks/test_hooks.py` holds eleven commands, three that pass (the count, the count with `?beta=true`, a `grep` for the host) and eight refused. The hooks' own tests: 156, OK. Not seen, as before: a host assembled from pieces or held in a variable; the guard is a tripwire, not a sandbox.

## The repair

Repaired at `a1ec2eb995098507b4d60523e82acfb367befccb`, 2026-10-11 (lane b22-codeql). `ANTHROPIC_HOST` finds each mention of the host, case-blind, with the characters that follow it to the end of the word, and `ENDPOINT_ONLY` admits the endpoint alone, with a query or a fragment; any other follower, a port, a longer host, another path, is refused, the guard erring toward refusing. The three requests above, through the lane's guard: refused; the legitimate count, passed. `test_the_anthropic_key_reaches_the_token_count_and_nothing_else` in `.claude/hooks/test_hooks.py` holds eleven commands, three that pass (the count, the count with `?beta=true`, a `grep` for the host) and eight refused. The hooks' own tests: 156, OK. Not seen, as before: a host assembled from pieces or held in a variable; the guard is a tripwire, not a sandbox.

**Closed 2026-10-11** with batch 22 (lanes b22-662, b22-663, b22-site, b22-deps and b22-codeql, merged into the round `lane-round-b22` made from the trunk at `2edee78b`, with the trunk's `80c66b00`, `b6accb55` and `e9284f11` merged in), the closing commit carrying VERSION 0.3.0 for release v0.3.0. Its closing gate ran on the round at `4bc0bebc`: the seed regenerated over two generations with VERSION 0.3.0, the runtime's ABI at 30, its fixpoint by `cmp`, SHA-256 beginning `4882373884cc9fa3`, 52,119,283 bytes; the compiler's own tests 1,620, all passed; the net's own tests 341, all passed; the full net 8,306 passed over 29 suites and 1 failed, `records/counts` alone, on the ROADMAP's open count this closing moves, `run` in four shards and `cache` alone after. The emissions were blessed on this Mac for the version line alone (597 files, one line each), and the two cases this Mac skips, 437's and 589's, on Linux arm64 at the same tree (emission 1,225 passed and 2 failed before the bless, those two; 1,227 and 0 after); the other 597 read byte for byte the same on both. Lane b22-codeql's repairs and the trunk's `b6accb55` and `e9284f11` touch no compiler file and were judged by their own cases after the gate. Under the optimistic chain the CI's legs judge it after the push, a red leg filing a new `blocking` defect naming it.
