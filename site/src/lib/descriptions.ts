/**
 * The prose beside each example, and the check that none of it is missing.
 *
 * One fragment per example per edition, `site/src/html/examples/<slug>.html`
 * and its Italian twin, holding a tagline and at most two short paragraphs. The
 * fragments are plain HTML files for the reason `site/README.md` gives for
 * every other page here: the prose stays where anybody can open and edit it
 * without knowing anything about the build.
 *
 * Why these are read with `fs` and not with `import.meta.glob`. The glob is
 * Vite's and works only inside a build, and everything else in this browser is
 * readable from a plain `node` script on purpose, because that is what lets the
 * highlighter's round-trip gate exist at all. One loader for all of it is worth
 * more than hot reloading of prose that changes once.
 *
 * The completeness check runs in BOTH directions, and that is the whole point
 * of this module. A missing fragment would be an example the site shows with no
 * words around it; a fragment naming no example would be prose nobody ever
 * reads, which is how a renamed program leaves its description behind. Both are
 * build errors, and every violation is collected into one message rather than
 * thrown one at a time, because fixing 60 of them one build at a time is how
 * somebody gives up.
 */

import { readText, isFile } from './repo.ts';
import { examples } from './examples-model.ts';
import { groups, groupOf, readingOrder } from '../data/examples.ts';
import type { Lang } from './i18n.ts';

const HOME = 'site/src/html/examples';

export interface Description {
  /** The one sentence under the title, and the card's own line. */
  tagline: string;
  /** The rest of the fragment: the paragraphs, as written. */
  body: string;
}

const cache = new Map<string, Description>();

function fragmentPath(slug: string, lang: Lang): string {
  return lang === 'en' ? `${HOME}/${slug}.html` : `site/src/html/it/examples/${slug}.html`;
}

/**
 * The tagline is the first `<p class="tagline">`, and it is required.
 *
 * It does three jobs at once, which is why it cannot be optional: the sentence
 * under the page's title, the card's line on the index, and the page's own
 * `<meta name="description">`. A fragment without one would give a page with no
 * description, and nothing would look broken.
 */
function parse(path: string, html: string): Description {
  const tagline = /<p class="tagline">([\s\S]*?)<\/p>/.exec(html);
  if (tagline === null) {
    throw new Error(
      `${path} has no <p class="tagline">…</p>.\n` +
        `  it is the sentence under the title, the card's line on the index, and the\n` +
        `  page's meta description, so a fragment cannot go without one.`
    );
  }
  const text = tagline[1].replace(/<[^>]*>/g, '').replace(/\s+/g, ' ').trim();
  if (text.length === 0) {
    throw new Error(`${path} has an empty tagline.`);
  }
  return { tagline: text, body: html.slice(tagline.index + tagline[0].length).trim() };
}

/** The description of one example, in one language. */
export function descriptionOf(slug: string, lang: Lang): Description {
  const key = `${lang}:${slug}`;
  const found = cache.get(key);
  if (found !== undefined) return found;

  const path = fragmentPath(slug, lang);
  const parsed = parse(path, readText(path));
  cache.set(key, parsed);
  return parsed;
}

/**
 * Everything that is missing or extra, as one message, or nothing.
 *
 * Called once from the index page's frontmatter, so a build cannot render a
 * single example page while the set is incomplete.
 */
export function assertDescriptionsComplete(): void {
  const problems: string[] = [];
  const all = examples();
  const slugs = new Set(all.map((one) => one.slug));

  // Every example needs a shelf, and one shelf only.
  for (const example of all) {
    const on = groups.filter((group) => group.slugs.includes(example.slug));
    if (on.length === 0) {
      problems.push(`${example.slug} is on no shelf: add it to a group in site/src/data/examples.ts`);
    } else if (on.length > 1) {
      problems.push(`${example.slug} is on ${on.length} shelves: ${on.map((g) => g.key).join(', ')}`);
    }
  }

  // Every shelf names examples that exist.
  for (const group of groups) {
    for (const slug of group.slugs) {
      if (!slugs.has(slug)) {
        problems.push(`site/src/data/examples.ts, group ${group.key}, names \`${slug}\`, which is not an example`);
      }
    }
  }

  // Every example needs both fragments, and each needs a tagline.
  for (const example of all) {
    for (const lang of ['en', 'it'] as Lang[]) {
      const path = fragmentPath(example.slug, lang);
      if (!isFile(path)) {
        problems.push(`${path} is missing: ${example.slug} would be shown with no words around it`);
        continue;
      }
      try {
        descriptionOf(example.slug, lang);
      } catch (error) {
        problems.push((error as Error).message.split('\n')[0]);
      }
    }
  }

  if (problems.length > 0) {
    throw new Error(
      `the examples browser is incomplete: ${problems.length} problems.\n  ` + problems.join('\n  ') + '\n'
    );
  }

  // The reading order has to be the whole set, or Next walks off the end.
  const order = readingOrder();
  if (order.length !== all.length) {
    throw new Error(`the reading order has ${order.length} entries and there are ${all.length} examples.`);
  }
}

/** Where an example sits, for the crumb and the JSON. */
export function shelfOf(slug: string): string {
  return groupOf(slug)?.key ?? 'ungrouped';
}
