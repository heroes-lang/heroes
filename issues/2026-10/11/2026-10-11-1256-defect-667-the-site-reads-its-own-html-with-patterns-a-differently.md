---
kind: defect
area: site
milestone: none
filed: 2026-10-11
commit: a1ec2eb995098507b4d60523e82acfb367befccb
github: none
---

- [ ] **667 — the site reads its own HTML with patterns a differently written tag passes** | CodeQL alerts 6, 7 and 8 (`js/incomplete-multi-character-sanitization`, high) and 5 (`js/bad-tag-filter`, high). Three readers drop a fragment's tags with `.replace(/<[^>]*>/g, '')`, which removes every `<` that has a `>` after it and leaves the one that has none: `a <script src="x"` comes out unchanged, `<script` standing, into the feed or a meta description. The script check of the built pages matched `/<script\b([^>]*)>([\s\S]*?)<\/script>/g` alone, so `<SCRIPT>` writing a cookie passed, a `<script` never closed passed, and a declared script with `SRC="https://..."` passed the no-`src` rule, each measured against the old check on a crafted page | `site/src/pages/log.xml.ts:67`, `site/src/lib/nods.ts:53`, `site/src/lib/descriptions.ts:62`, `site/src/lib/scripts.ts:50` · **class: blocking**

    **Origin:** the author, 2026-10-11, sending the repository's code-scanning page at 12:47 with *check these too*; the coordinator read the eight open alerts the same minute with `gh api 'repos/heroes-lang/heroes/code-scanning/alerts?state=open'`, opened every location, and wrote each repair in lane b22-codeql.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a check the site's rule rests on passed three of the four shapes it exists to refuse; and three readers carried an unterminated tag into the build.

    Repaired at `a1ec2eb995098507b4d60523e82acfb367befccb`, 2026-10-11 (lane b22-codeql). `site/src/lib/tags.ts`'s `withoutTags` walks the text: each run from a `<` to the first `>` after it is dropped, what the expression dropped, and a `<` with no `>` after it throws with the page's path. The three readers call it. Measured by `node --experimental-strip-types` over all 198 HTML files of `site/src/html`: the walk equals the old expression on every one, and `a <script src="x"` and `ok</b> and <scr` are refused, where the old expression returned the first unchanged. The script check is case-blind, takes a closing tag with spaces or attributes (`<\/script\b[^>]*>`), reads `src` in any case, and counts the page's `<script` openings against the scripts it paired, refusing a page where they differ. Four crafted pages, old check against new: `<SCRIPT>` writing a cookie, passed and refused; a declared script closed `</script >` that writes a cookie, refused by the old one only because the declared name reached no page, refused by the new one for the cookie; a `<script` never closed, passed and refused; `SRC=`, passed and refused. The site's build after: exit 0, 188 pages, claims 38 and 2 verb lists, nods 12 in each edition, `scripts: 2 enhancing over 188 pages`, so every page's openings pair. Whether CodeQL closes the four alerts is read after the push, when it analyses `main` (unrun; no CodeQL CLI on this Mac).
