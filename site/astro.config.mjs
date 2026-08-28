// @ts-check
import { defineConfig } from 'astro/config';
import sitemap from '@astrojs/sitemap';

const ORIGIN = 'https://heroes-lang.org';

// The two editions live at mirrored paths, `/x.html` and `/it/x.html`, which is
// what lets this be four lines instead of the pairing table an asymmetric site
// would need.
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
  if (pathname === '/index.html' || pathname === '/it/index.html') return 1.0;
  return pathname.includes('/docs/') ? 0.6 : 0.8;
}

export default defineConfig({
  site: ORIGIN,

  // `file`, not the default `directory`. It keeps `/docs/maps.html` spelled
  // `/docs/maps.html` instead of turning it into `/docs/maps/index.html`, and
  // every canonical, every hreflang, every og:url and every relative link on
  // these pages is written against those names. This one line is what makes the
  // move to Astro a move of FILES rather than a move of URLs.
  build: { format: 'file' },
  trailingSlash: 'never',

  integrations: [
    sitemap({
      serialize(item) {
        // Astro calls the home page `/`; the pages call it `/index.html` in
        // their own canonical, and a sitemap that disagrees with a canonical is
        // a sitemap arguing with the page it points at.
        const url = new URL(item.url);
        if (url.pathname.endsWith('/')) url.pathname += 'index.html';
        item.url = url.href;
        item.links = alternates(url.pathname);
        item.priority = priority(url.pathname);
        return item;
      },
    }),
  ],
});
