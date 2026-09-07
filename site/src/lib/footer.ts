/**
 * The cut between a page and the footer every page shares.
 *
 * A fragment under `site/src/html/` may end in a `<footer>` holding the
 * sentences that belong to that page alone: which file its code blocks are cut
 * from, which edit produced its diagnostic, where else the author can be found.
 * `BaseLayout` renders the page, cuts that element out here, and hands its
 * paragraphs to `SiteFooter`, which puts them above the byline and the
 * copyright line. So a fragment still ends in the `<footer>` a reader of the
 * file expects, and carries nothing that is shared.
 *
 * Two refusals, both so that the old shape cannot come back one page at a
 * time. A page with two `<footer>` elements is an error, because the CSS draws
 * the closing rule and the bolt on each one. And a page whose closing
 * paragraphs carry a byline or a copyright line is an error, because that is
 * the shared tail pasted back into a fragment, which is how 44 copies drifted
 * into five wordings before this existed.
 */
export function splitFooter(html: string, url: string): { body: string; closing: string } {
  const open = html.indexOf('<footer>');
  if (open === -1) return { body: html, closing: '' };
  const close = html.indexOf('</footer>', open);
  if (close === -1) throw new Error(`${url}: a <footer> opens and never closes`);
  if (html.indexOf('<footer', close) !== -1) {
    throw new Error(`${url}: two <footer> elements on one page; the layout expects at most one`);
  }
  const closing = html.slice(open + '<footer>'.length, close).trim();
  if (closing.includes('&copy;') || /(Built by|Fatto da) <a href="https:\/\/giuseppearici\.com/.test(closing)) {
    throw new Error(
      `${url}: the page's <footer> carries the byline or the copyright line.\n` +
        `  Those are written once, in site/src/components/SiteFooter.astro and the two\n` +
        `  _byline.html fragments; a page's own <footer> holds only what belongs to that page.`
    );
  }
  const body = html.slice(0, open) + html.slice(close + '</footer>'.length);
  return { body: body.replace(/\s+$/, '\n'), closing };
}
