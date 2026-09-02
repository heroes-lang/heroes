// Catch-all: holds every URL of the site on the parking page while it is armed.
//
// THIS FILE IS DORMANT. It sits outside the deploy path, so it shadows nothing;
// `README.md` beside it holds the three paths that arm it and the two traps.
//
// Cloudflare Pages evaluates Functions BEFORE static assets, and falls back to
// a static asset only when no Function matched. So this one file shadows the
// entire site without touching a route, a config or a build command. The real
// site is deployed underneath the whole time, complete and warm; it is simply
// unreachable until this file goes away.
//
// ARMING AND DISARMING is moving this file and `_routes.json` between
// `site/parking/` and the deploy path. Nothing else: no rebuild of anything, no
// domain to move between projects, no deploy setting to find. That is the point
// of doing it this way rather than with a password or a separate staging
// project, and it is why the gate costs nothing to keep.
//
// It lives beside the site because wrangler collects Functions from a
// `functions/` directory in the WORKING DIRECTORY it is run from, never from
// inside the output directory it is handed. The deploy step runs from `site/`,
// so `site/functions/` and that step's `working-directory` are a single
// decision: move one without the other and the deploy still succeeds, silently,
// with every URL of the site open and nothing in the log to say so.
//
// The gate was lifted by the author, who publishes the site ahead of the
// repository and says so on the pages that promise a download. Why that is the
// order, and what it obliges the copy to state, is `site/README.md`
// § Launch order.

// Opens the real site for one browser: visit any URL with `?preview=starman`
// once. The token is a speed bump and not a secret. All it is asked to do is
// keep the site off the radar of somebody typing URLs, and it is written here
// in a public file precisely so nobody mistakes it for protection.
const PREVIEW_TOKEN = 'starman';
const PREVIEW_COOKIE = 'preview';

export const onRequest = async (context) => {
  const { request, env, next } = context;
  const url = new URL(request.url);

  // Hand out the cookie, then bounce to the clean URL, so the token stops
  // riding along in the address bar and cannot leak through a shared link.
  if (url.searchParams.get('preview') === PREVIEW_TOKEN) {
    url.searchParams.delete('preview');
    return new Response(null, {
      status: 302,
      headers: {
        location: url.pathname + url.search,
        'set-cookie': `${PREVIEW_COOKIE}=1; Path=/; Max-Age=604800; Secure; HttpOnly; SameSite=Lax`,
      },
    });
  }

  if (request.headers.get('cookie')?.includes(`${PREVIEW_COOKIE}=1`)) return next();

  const page = await env.ASSETS.fetch(new Request(new URL('/_parking.html', url)));

  return new Response(page.body, {
    // 200 at the front door, 404 everywhere else. Nothing is published at those
    // paths yet and the status code should say so, even while the body stays a
    // page a visitor can read instead of a browser's own error screen. The
    // noindex meta tag inside the page says the same thing a second way.
    status: url.pathname === '/' ? 200 : 404,
    headers: {
      'content-type': 'text/html; charset=utf-8',
      'cache-control': 'no-store',
      'x-robots-tag': 'noindex, follow',
    },
  });
};
