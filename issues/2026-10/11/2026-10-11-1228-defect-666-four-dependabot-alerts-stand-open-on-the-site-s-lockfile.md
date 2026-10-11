---
kind: defect
area: site
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **666 — four Dependabot alerts stand open on the site's lockfile, three of them high** | `gh api 'repos/heroes-lang/heroes/dependabot/alerts?state=open'` lists four, all in `site/package-lock.json` and all transitive: alert 18, high, `sharp` < 0.35.5 (GHSA-wq5f-xc86-pv6w, its librsvg), fixed in 0.35.5, locked at 0.35.4 under `astro@7.2.9` and under `wrangler@4.146.0`'s `miniflare@5.20261001.0-alpha`, held there by the site's own `overrides` entry `sharp: ^0.35.4`; alert 17, medium, `smol-toml` <= 1.8.0 (GHSA-r4xh-jqrq-34v2, a quadratic-time `parse()`), fixed in 1.9.0, locked at 1.8.0 under `astro` and `@astrojs/internal-helpers@0.10.4`; alert 16, high, `source-map-js` >= 1.0.0 < 1.2.2 (GHSA-68fv-2mgg-jv7q, CVE-2026-93749, an event-loop denial of service through indexed source-map section offsets), fixed in 1.2.2, locked at 1.2.1 under `magicast@0.5.4`, `css-tree@3.2.1` and `@2.2.1`, and `postcss@8.5.26`; alert 15, high, `http-cache-semantics` <= 4.2.0 (GHSA-ch52-4w7c-c8xp, CVE-2026-93748, a `max-stale` request served a shared-cache entry zeroed for another user), with no patched version on the advisory, locked at 4.2.0 under `astro`. None reaches a page a visitor loads: each is a build-time or deploy-time package. The repository is public, and an open high on it reads as a choice | `site/package.json` (`astro` ^7.2.9, `wrangler` pinned exactly at 4.146.0 since `3508c82b`, the `sharp` override since `8a9a50bf`), `site/package-lock.json`, `site/astro.config.mjs` (whether the build ever fetches a remote image, the path that reaches `http-cache-semantics`) · **class: blocking**

    **Origin:** the coordinator, 2026-10-11, reading the open alerts at 12:26 by `date` before release v0.3.0, with `npm ls` the same minute; filed by lane b22-deps at 12:28, which measured the four again in its own worktree (`npm ci` from the trunk's lock at `2edee78b`, then `npm ls sharp smol-toml source-map-js http-cache-semantics`).

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a security advisory open on a dependency the published site is built and deployed with.
