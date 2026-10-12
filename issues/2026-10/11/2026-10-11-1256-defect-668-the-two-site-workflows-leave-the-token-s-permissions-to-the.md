---
kind: defect
area: ci
milestone: none
filed: 2026-10-11
commit: a1ec2eb995098507b4d60523e82acfb367befccb
github: https://github.com/heroes-lang/heroes/issues/2008
---

- [x] **668 — the two site workflows leave the token's permissions to the repository's setting** | CodeQL alerts 1 and 2 (`actions/missing-workflow-permissions`, medium). `deploy-site.yml`'s job and `release-site.yml`'s call declare no `permissions`, so GITHUB_TOKEN gets whatever the repository's default grants. That default reads `read` today (`gh api repos/heroes-lang/heroes/actions/permissions/workflow`: `default_workflow_permissions` `read`), so no write was ever granted; a change of the setting would widen both jobs in silence. `ci.yml` already states `contents: read` at its top | `.github/workflows/deploy-site.yml:58`, `.github/workflows/release-site.yml:24` · **class: improvement**

    **Origin:** the author, 2026-10-11, sending the repository's code-scanning page at 12:47 with *check these too*; the coordinator read the eight open alerts the same minute with `gh api 'repos/heroes-lang/heroes/code-scanning/alerts?state=open'`, opened every location, and wrote each repair in lane b22-codeql.

    **Class: improvement**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): no write was granted, measured; the token's reach rested on a setting outside the tree.

    Repaired at `a1ec2eb995098507b4d60523e82acfb367befccb`, 2026-10-11 (lane b22-codeql). Both workflows state `permissions: contents: read` at their top, with the reason: the deploy reads the repository and publishes to Cloudflare under Cloudflare's token, and a called workflow gets at most what its caller grants. `grep` of both files for `github.token`, `GITHUB_TOKEN`, `gh `, `deployments`, `statuses`, `pull-requests` and `id-token` finds no use of the token beyond the checkout; both parse (`ruby -ryaml`). Judged by the deploy's run after the push, and the alerts' closing read then (unrun).

## The repair

Repaired at `a1ec2eb995098507b4d60523e82acfb367befccb`, 2026-10-11 (lane b22-codeql). Both workflows state `permissions: contents: read` at their top, with the reason: the deploy reads the repository and publishes to Cloudflare under Cloudflare's token, and a called workflow gets at most what its caller grants. `grep` of both files for `github.token`, `GITHUB_TOKEN`, `gh `, `deployments`, `statuses`, `pull-requests` and `id-token` finds no use of the token beyond the checkout; both parse (`ruby -ryaml`). Judged by the deploy's run after the push, and the alerts' closing read then (unrun).

**Closed 2026-10-11** with batch 22 (lanes b22-662, b22-663, b22-site, b22-deps and b22-codeql, merged into the round `lane-round-b22` made from the trunk at `2edee78b`, with the trunk's `80c66b00`, `b6accb55` and `e9284f11` merged in), the closing commit carrying VERSION 0.3.0 for release v0.3.0. Its closing gate ran on the round at `4bc0bebc`: the seed regenerated over two generations with VERSION 0.3.0, the runtime's ABI at 30, its fixpoint by `cmp`, SHA-256 beginning `4882373884cc9fa3`, 52,119,283 bytes; the compiler's own tests 1,620, all passed; the net's own tests 341, all passed; the full net 8,306 passed over 29 suites and 1 failed, `records/counts` alone, on the ROADMAP's open count this closing moves, `run` in four shards and `cache` alone after. The emissions were blessed on this Mac for the version line alone (597 files, one line each), and the two cases this Mac skips, 437's and 589's, on Linux arm64 at the same tree (emission 1,225 passed and 2 failed before the bless, those two; 1,227 and 0 after); the other 597 read byte for byte the same on both. Lane b22-codeql's repairs and the trunk's `b6accb55` and `e9284f11` touch no compiler file and were judged by their own cases after the gate. Under the optimistic chain the CI's legs judge it after the push, a red leg filing a new `blocking` defect naming it.
