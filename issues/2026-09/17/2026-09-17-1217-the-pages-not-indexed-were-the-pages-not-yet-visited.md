# The pages Google had not indexed were the pages it had not yet visited

2026-09-17. Out of the author's question, who brought four exports of Google
Search Console's page-indexing report (snapshot of 2026-09-14) and asked why
Google complained of pages it had not indexed: 69 indexed, 119 not, under four
reasons. Every number below was measured in this session against the live site
with `curl`.

## The report and the site agree to the address

The sitemap the site serves declares **184** addresses (`sitemap-index.xml` and
one chunk), 92 in each edition, each with its three `hreflang` alternates and a
`lastmod` read from git (19 distinct dates); it carries no `log.xml`, no
not-found page and no `www`. **184 plus the four addresses the sitemap does not
carry, `http://`, `https://www`, `log.xml` and `/404`, is 188, which is 69 plus
119.** Google knew exactly what the site declares and nothing else.

## What each of the four reasons was

- **Page with redirect, 2**: `http://heroes-lang.org/` and
  `https://www.heroes-lang.org/`, both answering 301 to the apex. One canonical
  hostname, by the decision recorded in `site/README.md` § The domain. Correct,
  and listed by Google for information.
- **Excluded by noindex, 1**: the not-found page, which answers 200 with
  `noindex, follow` at `/404` by the decision of 2026-09-08
  (`2026-09-08-0009-a-wrong-address-gets-a-page-and-a-404-where-the-site.md`).
  Correct.
- **Crawled, currently not indexed, 3**: `log.xml`, an Atom feed linked from
  every page's `<head>`; and `/it/examples/gallery/07-strings/` and
  `/it/examples/gallery/12-interpolation/`, crawled on 2026-09-15 and
  2026-09-14, one day and zero days before the snapshot. Their heads were read
  and are what `site/src/lib/seo.ts` guarantees: self-canonical, three
  `hreflang`, a title and a description no other page carries, and 224 and 275
  words of prose beside the code. Nothing to repair; indexing follows crawling
  by days.
- **Discovered, currently not indexed, 113**: all 113 are in the sitemap
  (compared address by address, zero outside it) and none has ever been fetched
  (last crawl 1970-01-01). 103 are example pages, 53 English and 50 Italian; 10
  are chapters and Italian pages. The site went public on 2026-09-03 and the
  repository on 2026-09-08; Search Console's data begins 2026-09-07. In eleven
  days the crawler fetched 75 of 188 addresses and indexed 69 of them, which on
  a domain with no history and no inbound links is the crawler's pace.

**The premise was wrong in the useful direction: nothing on the site was
broken.** It is the shape of the 2026-09-10-0006 entry again, where the tags
were fine and what was missing was a signal. What the site had left unsaid this
time was smaller: `robots.txt` names three addresses that are not pages,
`/spec.md`, `/llms.txt` and `/log.xml`, in a comment, and no header answered for
them.

## The decision: `site/public/_headers`

Cloudflare Pages reads it from `dist/` as it reads `_redirects`, and serves
neither (`/_redirects` answers 404, measured). Three answers, one per address:

- **`/log.xml` answers `X-Robots-Tag: noindex`.** A feed is for feed readers,
  which never look at the header. Google fetched it because every page links
  it, and without an answer it stays under *crawled, currently not indexed* for
  good; with one it moves to *excluded by noindex*, a state declared rather than
  suffered.
- **`/spec.md` answers `Link: <https://heroes-lang.org/spec/>;
  rel="canonical"`.** It is the same text as `/spec/` (3530 words against 4141,
  the HTML carrying the wrapping copy). Given no preference a search engine
  picks the door itself, and it may pick the bare `.md`. The header changes
  nothing for `curl` or for a model, which still get the file. Preventive: the
  report did not yet list `spec.md`.
- **`/llms.txt` gets nothing, on purpose.** It has no HTML twin, and `noindex`
  would hide it from the search crawlers `robots.txt` says yes to by name,
  since Claude-SearchBot and OAI-SearchBot honour it as Google does.

## Three things refused

- **`noindex` on `spec.md` or `llms.txt`**: the reason above. The site's
  position on model crawlers is written in `robots.txt`, and a header that
  contradicted it would be the louder of the two.
- **Touching the not-found page's canonical**, which names `/404/` while Pages
  answers at `/404` after a 308. Google ignores the canonical of a `noindex`
  page, `seo.ts` requires a self-canonical and maps `404.html` to `/404/` on
  purpose, and an exception there would buy nothing measurable.
- **A build-time audit of `dist/_headers`.** The file is copied verbatim and git
  already guarantees its presence; the fact that matters, the header on the
  wire, exists only at Cloudflare's edge. The instrument is the live check, and
  it is added to the post-publish list in `site/README.md` § The domain rather
  than invented beside it.

## What is expected, and when to read it again

Put to the author by hand, since Search Console offers no path this repository
would take: confirm `sitemap-index.xml` is submitted (the exports cannot say;
*All known pages* is the report's filter), request indexing for the ten
non-example addresses in the discovered list, and run *Test live URL* on one
discovered example, the only way to rule out a bot challenge at Cloudflare that
this machine cannot see. **The prediction that would falsify this entry**:
re-exported in three to four weeks, *discovered* well under 113, `log.xml`
under *excluded by noindex*, the two Italian gallery pages indexed. If
*discovered* is still above 100 after a month, the lever is inbound links and
not the site.
