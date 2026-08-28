// @ts-check
import { defineConfig } from 'astro/config';
import sitemap from '@astrojs/sitemap';

const ORIGIN = 'https://heroes-lang.org';

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

// The same three tiers the hand-written sitemap carried: the two landings, the
// pages the nav points at, the documentation chapters underneath them.
function priority(pathname) {
  if (pathname === '/' || pathname === '/it/') return 1.0;
  return pathname.includes('/docs/') ? 0.6 : 0.8;
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
