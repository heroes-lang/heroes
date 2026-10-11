---
kind: defect
area: ci
milestone: none
filed: 2026-10-11
commit: a1ec2eb995098507b4d60523e82acfb367befccb
github: none
---

- [ ] **668 — the two site workflows leave the token's permissions to the repository's setting** | CodeQL alerts 1 and 2 (`actions/missing-workflow-permissions`, medium). `deploy-site.yml`'s job and `release-site.yml`'s call declare no `permissions`, so GITHUB_TOKEN gets whatever the repository's default grants. That default reads `read` today (`gh api repos/heroes-lang/heroes/actions/permissions/workflow`: `default_workflow_permissions` `read`), so no write was ever granted; a change of the setting would widen both jobs in silence. `ci.yml` already states `contents: read` at its top | `.github/workflows/deploy-site.yml:58`, `.github/workflows/release-site.yml:24` · **class: improvement**

    **Origin:** the author, 2026-10-11, sending the repository's code-scanning page at 12:47 with *check these too*; the coordinator read the eight open alerts the same minute with `gh api 'repos/heroes-lang/heroes/code-scanning/alerts?state=open'`, opened every location, and wrote each repair in lane b22-codeql.

    **Class: improvement**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): no write was granted, measured; the token's reach rested on a setting outside the tree.

    Repaired at `a1ec2eb995098507b4d60523e82acfb367befccb`, 2026-10-11 (lane b22-codeql). Both workflows state `permissions: contents: read` at their top, with the reason: the deploy reads the repository and publishes to Cloudflare under Cloudflare's token, and a called workflow gets at most what its caller grants. `grep` of both files for `github.token`, `GITHUB_TOKEN`, `gh `, `deployments`, `statuses`, `pull-requests` and `id-token` finds no use of the token beyond the checkout; both parse (`ruby -ryaml`). Judged by the deploy's run after the push, and the alerts' closing read then (unrun).
