// @ts-check
import { defineConfig } from 'astro/config';
import sitemap from '@astrojs/sitemap';
import { readdirSync, statSync, unlinkSync } from 'node:fs';
import { join } from 'node:path';

const ORIGIN = 'https://heroes-lang.org';

// Astro copies `public/` into the output verbatim, and macOS writes a
// `.DS_Store` into any directory Finder has been asked to look at. Measured:
// `site/public/.DS_Store` and `site/dist/.DS_Store` had the identical hash
// after a build, so the file was reaching the output on every local run.
// `.gitignore` carries a bare `.DS_Store`, which is why it appeared in nobody's
// `git status`.
//
// WHAT THIS DOES NOT CLAIM, because the first draft of this comment claimed it
// and it was false. Nothing was published. `.github/workflows/deploy-site.yml`
// is the site's only automated deploy path, and it checks the repository out on
// a Linux runner, builds there and uploads that dist. No `.DS_Store` is tracked
// and none was ever committed, so on CI there is nothing in `public/` to copy.
// The output that carried one existed only on one Mac.
//
// The guard is still worth its lines, for the path that is left: `README.md`
// documents a `wrangler` run started by hand from a local `dist`, and that one
// would carry it. A `.DS_Store` is a directory listing, naming every file that
// was in the folder including the ones since deleted, which is why publishing
// one is worse than untidy.
//
// Deleting the file is not the fix, because Finder writes it back. The fix has
// to run at the moment the output is assembled, which is here.
function withoutFinderDroppings() {
  return {
    name: 'heroes:no-ds-store',
    hooks: {
      'astro:build:done': ({ dir, logger }) => {
        let removed = 0;
        const walk = (path) => {
          for (const entry of readdirSync(path)) {
            const full = join(path, entry);
            if (statSync(full).isDirectory()) walk(full);
            else if (entry === '.DS_Store') {
              unlinkSync(full);
              removed += 1;
            }
          }
        };
        walk(dir.pathname);
        if (removed > 0) logger.info(`removed ${removed} .DS_Store from the output`);
      },
    },
  };
}

// The two editions live at mirrored paths, `/x/` and `/it/x/`, which is what
// lets this be four lines instead of the pairing table an asymmetric site would
// need.
function alternates(pathname) {
  const en = pathname.startsWith('/it/') ? pathname.slice(3) : pathname;
  return [
    { lang: 'en', url: ORIGIN + en },
    { lang: 'it', url: ORIGIN + '/it' + en },
    { lang: 'x-default', url: ORIGIN + en },
  ];
}

// The tiers a crawler reads as this site's own ranking of itself: the two
// landings, then the pages the nav points at, then the chapters and the
// programs underneath them.
//
// The examples tier is the one this needed and did not have. 134 of the 180
// pages are example programs, and they were all sitting at 0.8, the same figure
// as the seven pages in the nav, which tells a crawler that `examples/nqueens/`
// matters as much as the front door of the documentation. The INDEX of them
// stays at 0.8, because that page is in the nav and is where a reader starts.
function priority(pathname) {
  if (pathname === '/' || pathname === '/it/') return 1.0;
  const path = pathname.startsWith('/it/') ? pathname.slice(3) : pathname;
  if (path === '/examples/') return 0.8;
  if (path.startsWith('/examples/')) return 0.7;
  return path.startsWith('/docs/') ? 0.6 : 0.8;
}

export default defineConfig({
  site: ORIGIN,

  // `directory` and `always`, which is the pair the host actually serves and
  // the pair the author's other site uses. Measured live against this project's
  // own Cloudflare Pages deployment: `/why/` and `/docs/` answer 200, while
  // `/why.html`, `/why`, `/docs` and `/docs/maps/` where the file is
  // `maps.html` all answer 308. Pages strips a `.html` extension whether the
  // site wants it to or not, so the only way to advertise URLs that do not
  // redirect is to advertise the ones it serves.
  //
  // The site was ported at `format: 'file'` first, to keep the hand-written
  // `.html` URLs, and that was the wrong target: it preserved names the host
  // refuses to serve. Changing them cost nothing only because the site had
  // never been published -- no external link, no search index, no bookmark.
  build: { format: 'directory' },
  trailingSlash: 'always',

  integrations: [
    withoutFinderDroppings(),
    sitemap({
      serialize(item) {
        const url = new URL(item.url);
        item.links = alternates(url.pathname);
        item.priority = priority(url.pathname);
        return item;
      },
    }),
  ],
});
