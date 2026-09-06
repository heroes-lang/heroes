# The pre-launch parking gate, disarmed

Until the site was published these three files held every URL of
`heroes-lang.org` on a holding page, with the real site deployed underneath,
complete and warm. They were moved here, out of the deploy path, instead of
being deleted, because the mechanism is proven and may serve again: a redesign
staged behind the same door, a page written before it is meant to be read.

That is the same choice the author's other site made at its own launch, and
`giuseppearici.com`'s `site/parking/` holds the same three files for the same
reason.

| File | Lived at | Role |
|---|---|---|
| `[[path]].js` | `site/functions/[[path]].js`, until 2026-09-03 | the catch-all Pages Function: holding page for everyone, real site behind `?preview=starman` |
| `_parking.html` | `site/public/_parking.html`, until 2026-09-03 | the holding page itself, self-contained, `noindex, follow` |
| `_routes.json` | `site/public/_routes.json`, until 2026-09-03 | excludes `/_parking.html` from the Function, so the page it serves can be fetched at all |

**To re-arm the gate:** move each file back to its *lived at* path and push.
Wrangler collects Pages Functions from a `functions/` directory in the working
directory it runs from, which is `site/`, the deploy step's `working-directory`
in `.github/workflows/deploy-site.yml`. So the catch-all shadows the whole site
again after one deploy, with no route, build command or domain to touch.
Disarming is the same move in reverse.

**Two traps, both of which fail silently.** The Function must sit in
`site/functions/` — where these lived until 2026-09-03 — never inside `dist/`: wrangler never collects from the output
directory it is handed, so a gate in the wrong place deploys green with every URL
of the site open and nothing in the log to say so. And `_routes.json` has to
travel with it: without the exclusion the Function intercepts its own holding
page, which then cannot be fetched.

**The holding page carries no stylesheet, and that is not tidiness.** A request
for `/style.css` reaches the Function like every other URL and comes back as the
holding page, so a stylesheet link in it would render unstyled. Everything it
needs is inside it.

The token in `[[path]].js` is written in the clear on purpose. It is a speed
bump, not protection: all it is asked to do is keep an unfinished page off the
radar of somebody typing URLs, and calling it a secret is how somebody comes to
rely on it.

Full reasoning in `../README.md` under § Deployment.
