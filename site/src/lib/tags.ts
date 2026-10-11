/**
 * The text of a fragment of the site's own markup, with its tags removed.
 *
 * Three readers took a postcard, a nod or a tagline out of a page and dropped
 * its tags with `.replace(/<[^>]*>/g, '')` (defect 667). That removes every
 * `<` that has a `>` after it, and leaves the one that has none: a fragment
 * ending `<script src="x"` came out with `<script` standing, and reached the
 * feed or a meta description as the start of a tag. The pages are this
 * repository's own, which is why it never happened, and not a reason the code
 * may rely on it.
 *
 * So the text is walked rather than matched: each run from a `<` to the first
 * `>` after it is dropped, exactly what the expression dropped, and a `<` with
 * no `>` after it is a fault in the page, thrown with where it was found rather
 * than carried into the build. A `>` alone stays: HTML allows it in text.
 */
export function withoutTags(html: string, where: string): string {
  let text = '';
  let at = 0;
  while (at < html.length) {
    const open = html.indexOf('<', at);
    if (open < 0) {
      text += html.slice(at);
      break;
    }
    const close = html.indexOf('>', open);
    if (close < 0) {
      throw new Error(
        `${where}: a "<" opens a tag that never closes, in ${JSON.stringify(html.slice(open, open + 80))}.\n` +
          `  text that means the character is written &lt; in the page.`
      );
    }
    text += html.slice(at, open);
    at = close + 1;
  }
  return text;
}
