/**
 * A page in the nav is never shown as somebody's child.
 *
 * Author decision 2026-09-17, given as two sentences one after the other:
 * *"some pages show the crumb with the path and others do not, shall we make it
 * uniform?"* and then *"all right, but then the ones that have an entry in the
 * menu, never put them as children"*.
 *
 * Measured the same hour, and both halves of the complaint were real. Sixteen
 * hand-written pages carried a `.crumb` and nine did not, with no rule dividing
 * them. And two of the sixteen were nav pages wearing somebody else's parent:
 * `/spec/` read `Heroes / Guide / specification` while Spec is the ninth entry
 * of the nav row, and `/about/thanks/` read `Heroes / Author / thanks` while
 * Thanks is the tenth. A row in the nav is a top-level place; a trail that puts
 * it under another page tells a reader the opposite of what the row tells them,
 * and a reader who arrives from a search engine sees the trail first.
 *
 * So the rule has two halves and this file checks both:
 *
 * - **a page the nav points at carries no crumb**, because it is not a child;
 * - **every other page carries one**, because it is, and the reader arriving
 *   mid-site needs to know where they are.
 *
 * After it, the divide is not a habit: the fourteen chapters, every example
 * program and `/why/not/` carry a trail, and the ten nav pages carry none.
 *
 * WHY THIS READS `dist/` AND NOT THE FRAGMENTS. The examples pages are
 * generated (`examples-render.ts` writes their crumb) and their prose fragments
 * are three lines with no crumb in them, so a source-side check would have to
 * carry a list of which fragments are whole pages. That list is a second place
 * where truth lives, and this repository has paid for those. The output has no
 * such ambiguity: every `index.html` under `dist/` is one page at one address.
 *
 * WHAT IT DOES NOT CHECK: that the parent named in a trail is the right one.
 * The trail is prose in a fragment, and a wrong parent is a sentence a reader
 * checks. This checks the shape the author ruled on, and says so rather than
 * implying more.
 */

import { readdirSync, readFileSync, statSync } from 'node:fs';
import { join } from 'node:path';
import { readText } from './repo.ts';

const NAV_FILE = 'site/src/components/SiteNav.astro';

/** A crumb, as the pages write it and as the generated example pages write it. */
const CRUMB = /<p class="crumb">/g;

/**
 * The paths the nav row points at, read from the component that draws it.
 *
 * Never a list typed here: the row changed four times in one month, and a copy
 * of it in a checker is a copy that goes stale the day somebody edits the row
 * and leaves this file alone. The floor refuses a parse that reads nothing,
 * because a checker that found no nav entries would bless every page on the
 * site and print a green line saying so.
 */
export function navPaths(): Set<string> {
  const text = readText(NAV_FILE);
  const out = new Set<string>(['/']);
  for (const [, slug] of text.matchAll(/slug:\s*'([a-z0-9/-]+)'/g)) out.add(`/${slug}/`);
  if (out.size < 6) {
    throw new Error(
      `${NAV_FILE}: read ${out.size - 1} nav entries and the floor is five.\n` +
        `  a parse that reads nothing would bless every page on the site.`
    );
  }
  return out;
}

/** Every built page, as the address it answers at. */
function builtPages(dist: string): { url: string; file: string }[] {
  const out: { url: string; file: string }[] = [];
  const walk = (dir: string, prefix: string) => {
    for (const entry of readdirSync(dir)) {
      const full = join(dir, entry);
      if (statSync(full).isDirectory()) walk(full, `${prefix}${entry}/`);
      else if (entry === 'index.html') out.push({ url: prefix, file: full });
    }
  };
  walk(dist, '/');
  return out.sort((a, b) => (a.url < b.url ? -1 : a.url > b.url ? 1 : 0));
}

/**
 * The rule, over the output. Returns the line the build log prints.
 *
 * Throws naming every page that breaks it, on both sides, because a message
 * saying only "a crumb is wrong" sends somebody opening a hundred and
 * eighty-eight pages.
 */
export function assertCrumbsMatchTheNav(dist: string): string {
  const nav = navPaths();
  const problems: string[] = [];
  let trails = 0;
  let tops = 0;

  for (const { url, file } of builtPages(dist)) {
    const path = url.startsWith('/it/') ? url.slice(3) : url === '/it/' ? '/' : url;
    if (path.startsWith('/404')) continue;
    // The built file is read by its absolute path, the one the hook handed us.
    // Deriving a repository-relative path from it would mean finding `site/`
    // inside a string that begins with the machine's own directories, and this
    // repository has a `site` in its path twice on one developer's machine.
    const found = (readFileSync(file, 'utf-8').match(CRUMB) ?? []).length;
    if (nav.has(path)) {
      tops += 1;
      if (found > 0) {
        problems.push(
          `${url} is in the nav row and carries a trail naming a parent. ` +
            `A page the nav points at is not a child (author decision 2026-09-17): remove the \`p.crumb\`.`
        );
      }
    } else {
      trails += 1;
      if (found === 0) {
        problems.push(
          `${url} is not in the nav row and carries no trail. ` +
            `A page a reader can land on mid-site says where it sits: add a \`p.crumb\` naming its parent.`
        );
      } else if (found > 1) {
        problems.push(`${url} carries ${found} trails, and one page has one place.`);
      }
    }
  }

  if (problems.length > 0) {
    throw new Error(
      `${problems.length} page(s) disagree with the nav about where they sit.\n    ` +
        problems.join('\n    ') +
        `\n  A page in the nav carries no crumb; every other page carries one (site/src/lib/crumbs.ts).\n`
    );
  }
  return `crumbs: ${trails} pages carry a trail, ${tops} nav pages carry none`;
}
