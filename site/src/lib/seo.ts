/**
 * What every built page owes a search engine, checked on the built page.
 *
 * `BaseLayout.astro` computes the whole head from one prop, so no page can
 * forget a tag and no page can pass a wrong one. That is a strong guarantee and
 * it is not this one: it says the tags are CONSISTENT, and says nothing about
 * whether they are usable. A description of 188 characters is emitted correctly
 * and cut off in the result; a title that repeats another page's title is
 * emitted correctly and tells a reader nothing about which of the two to open.
 * Four of those existed when this was written, and nothing in the tree could
 * have said so.
 *
 * IT READS `dist/`, and that is the lesson the sitemap's own audit paid for in
 * the same session: an earlier version of that one read a list the config kept
 * while building, the sitemap integration swallowed the errors that would have
 * emptied it, and the check passed on the exact run it existed to catch. The
 * head is only real once it is written, so this opens the files that ship.
 *
 * The two limits are where a result gets cut, not where a sentence gets long,
 * and neither is a rule of grammar: a description over 160 characters and a
 * title over 60 are shown truncated, so the words past them are written for
 * nobody. They are deliberately not tight. Nothing here asks a page to be
 * shorter than it needs to be, only to end before the scissors.
 */

import { readdirSync, readFileSync, statSync } from 'node:fs';
import { join } from 'node:path';

const DESCRIPTION_LIMIT = 160;
const TITLE_LIMIT = 60;
const ORIGIN = 'https://heroes-lang.org';

/** Every built page, as a repository-relative name and the URL it answers at. */
function builtPages(distDir: string): { file: string; url: string }[] {
  const out: { file: string; url: string }[] = [];
  const walk = (dir: string, prefix: string) => {
    for (const entry of readdirSync(dir).sort()) {
      const full = join(dir, entry);
      if (statSync(full).isDirectory()) {
        walk(full, `${prefix}${entry}/`);
      } else if (entry === 'index.html') {
        out.push({ file: full, url: `/${prefix}` });
      } else if (entry === '404.html') {
        // Astro writes the not-found pages flat, because that is the name the
        // host looks for, but the page itself answers at `/404/`. Deriving the
        // URL from the filename would fail a canonical that is correct.
        out.push({ file: full, url: `/${prefix}404/` });
      }
    }
  };
  walk(distDir, '');
  return out;
}

function tagOf(html: string, pattern: RegExp): string | null {
  const found = pattern.exec(html);
  return found === null ? null : found[1].trim();
}

/**
 * A `&amp;` in the markup is one character to a reader and to a search engine.
 *
 * Counting the raw attribute would measure the escaping rather than the
 * sentence, and would report a description as too long because it holds an
 * ampersand. Only the five XML entities can appear here, since Astro escapes
 * nothing else in an attribute value.
 */
function decoded(text: string): string {
  return text
    .replaceAll('&lt;', '<')
    .replaceAll('&gt;', '>')
    .replaceAll('&quot;', '"')
    .replaceAll('&#39;', "'")
    .replaceAll('&amp;', '&');
}

/**
 * Every page that ships carries a usable title, description and canonical.
 *
 * Returns the line the build log prints, so a green build still shows the check
 * ran and over how many pages.
 */
export function assertHeadsUsable(distDir: string): string {
  const pages = builtPages(distDir);
  if (pages.length === 0) throw new Error(`no built pages under ${distDir}.`);

  const problems: string[] = [];
  const titles = new Map<string, string>();
  const descriptions = new Map<string, string>();

  for (const { file, url } of pages) {
    const html = readFileSync(file, 'utf8');
    const title = tagOf(html, /<title>([\s\S]*?)<\/title>/);
    const description = tagOf(html, /<meta name="description" content="([^"]*)"/);
    const canonical = tagOf(html, /<link rel="canonical" href="([^"]*)"/);

    if (title === null || title === '') problems.push(`${url} has no <title>.`);
    if (description === null || description === '') {
      problems.push(`${url} has no <meta name="description">.`);
    }
    if (canonical === null) problems.push(`${url} has no <link rel="canonical">.`);
    else if (canonical !== ORIGIN + url) {
      problems.push(
        `${url} says its canonical is ${canonical}, which is a different address.\n` +
          `    a canonical pointing elsewhere tells a search engine this page is a\n` +
          `    copy of that one, and the page then never appears on its own.`
      );
    }

    if (title !== null) {
      const text = decoded(title);
      if (text.length > TITLE_LIMIT) {
        problems.push(`${url} has a ${text.length}-character title, shown cut at ${TITLE_LIMIT}.`);
      }
      const twin = titles.get(text);
      if (twin !== undefined) problems.push(`${url} and ${twin} share one title: ${text}`);
      else titles.set(text, url);
    }

    if (description !== null) {
      const text = decoded(description);
      if (text.length > DESCRIPTION_LIMIT) {
        problems.push(
          `${url} has a ${text.length}-character description, shown cut at ${DESCRIPTION_LIMIT}.`
        );
      }
      const twin = descriptions.get(text);
      if (twin !== undefined) {
        problems.push(`${url} and ${twin} share one description: ${text}`);
      } else descriptions.set(text, url);
    }
  }

  if (problems.length > 0) {
    throw new Error(
      `${problems.length} page heads are not usable as a search result:\n` +
        problems.map((one) => `  ${one}`).join('\n')
    );
  }

  return `${pages.length} page heads usable, all titles and descriptions distinct`;
}
