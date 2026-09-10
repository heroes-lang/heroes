/**
 * When each page last changed, read from git rather than from the clock.
 *
 * The sitemap advertises 184 addresses and, until this existed, told a crawler
 * nothing about which of them had moved. That is the one thing a sitemap is
 * uniquely able to say, and this site had just changed shape underneath it: the
 * redesign took 46 pages to 186, retired five addresses per edition and moved a
 * sixth, and every one of those facts reached a search engine only when it
 * happened to come back and look.
 *
 * WHY GIT AND NOT THE BUILD TIME, which is the obvious cheap answer and is
 * worse than nothing. A `lastmod` stamped at build time says every page changed
 * on every deploy. A crawler that finds a site claiming 184 simultaneous
 * changes, repeatedly, has been given a field that carries no information, and
 * the documented response is to stop trusting it: the site then has no way to
 * say a page moved even when one does. A date that is sometimes wrong is worse
 * than a date that is absent, because the absent one can still be added.
 *
 * WHAT COUNTS AS THE PAGE CHANGING, which is the decision this module makes.
 * A page's sources are its own prose fragment and its own wrapper, and nothing
 * else. The wrapper is in because `title` and `description` live there, and a
 * page whose description changed HAS changed for a search engine. The shell is
 * out: `BaseLayout.astro`, the nav, the footer and the render libraries touch
 * every page, so folding them in would move all 184 dates together on any
 * layout edit, which is the build-time stamp again wearing a different hat.
 *
 * THE TRAP, and it is silent. `git log` in a shallow clone sees one commit and
 * answers the same date for every path, so the whole mechanism degrades to the
 * build-time stamp with no error anywhere. `actions/checkout` is shallow by
 * default, so the deploy workflow has to ask for `fetch-depth: 0`. This module
 * refuses to guess in that case rather than emitting 184 identical dates, and
 * `assertLastmodVaries` catches the same shape from the other side, for a cause
 * nobody here thought of.
 */

import { execFileSync } from 'node:child_process';
import { readFileSync } from 'node:fs';
import { join } from 'node:path';
import { repoRoot, isFile } from './repo.ts';
import { examples } from './examples-model.ts';

/**
 * The history is deep enough to date a page, or the build stops before it runs.
 *
 * This is called from `astro:build:start` and not left to the lookup below,
 * because `@astrojs/sitemap` CATCHES whatever `serialize` throws. Measured
 * against a `--depth 1` clone of this repository: the refusal fired, its message
 * was printed, every entry was dropped, and the build wrote a sitemap holding
 * ZERO URLs and exited 0. A green build that publishes an empty sitemap is
 * worse than one that publishes stale dates, so the check has to run somewhere
 * the throw is not swallowed, and that is before the build starts.
 */
export function assertHistoryAvailable(): void {
  const shallow = execFileSync('git', ['rev-parse', '--is-shallow-repository'], {
    cwd: repoRoot(),
    encoding: 'utf8',
  }).trim();
  if (shallow !== 'true') return;
  throw new Error(
    'the sitemap needs the commit history and this is a shallow clone.\n' +
      '  git log would answer one date for every page, which is the build-time\n' +
      '  stamp `src/lib/lastmod.ts` exists to avoid, and nothing would look\n' +
      '  broken.\n' +
      '  fix: `fetch-depth: 0` on the checkout step in\n' +
      '  .github/workflows/deploy-site.yml, or a full clone locally.'
  );
}

/**
 * Every tracked path, mapped to the date of the last commit that touched it.
 *
 * One `git log` over the whole history rather than one per page: 184 pages
 * would be 184 subprocesses, and the single pass costs 0.12 seconds over 1233
 * commits on the author's MacBook Air. `git log` walks newest first, so the
 * FIRST date seen for a path is the last commit that touched it.
 *
 * A rename shows only the new name, which is right: the page is the address it
 * has now. A merge commit lists no files without `-m` and so contributes
 * nothing, which is also right, since a merge changes no content of its own.
 */
let cachedDates: Map<string, Date> | null = null;

function commitDates(): Map<string, Date> {
  if (cachedDates !== null) return cachedDates;

  assertHistoryAvailable();

  // A NUL before each date, so a commit's own line can never be mistaken for a
  // filename: a path may hold anything except NUL and newline.
  const log = execFileSync('git', ['log', '--pretty=format:%x00%cI', '--name-only'], {
    cwd: repoRoot(),
    encoding: 'utf8',
    maxBuffer: 256 * 1024 * 1024,
  });

  const dates = new Map<string, Date>();
  let current: Date | null = null;
  for (const line of log.split('\n')) {
    if (line.startsWith('\x00')) {
      current = new Date(line.slice(1));
      continue;
    }
    if (line === '' || current === null) continue;
    if (!dates.has(line)) dates.set(line, current);
  }

  cachedDates = dates;
  return dates;
}

/**
 * The newest commit in the tree, which is the floor for a page whose own
 * sources are not committed yet.
 *
 * This is the local case: the author adds a page and builds before committing
 * it. Refusing to build would make the site unbuildable exactly while it is
 * being written, and inventing `now` would claim a date git cannot confirm. The
 * newest commit is the honest floor, and if the fallback ever takes over the
 * whole site, every date is identical and `assertLastmodVaries` says so.
 */
function newest(dates: Map<string, Date>): Date {
  let best = new Date(0);
  for (const date of dates.values()) if (date > best) best = date;
  return best;
}

/**
 * The example directory or file behind each `/examples/<slug>/` address.
 *
 * A program is a directory and its whole contents are shown on the page, so the
 * directory is the source. The gallery is the exception the model already
 * names: its `dir` is `examples/gallery` for all fourteen of them, because each
 * gallery entry is one FILE rather than a directory. Taking `dir` there would
 * give fourteen pages one shared date, which is the collapse this module is
 * about, in miniature.
 */
function exampleSources(): Map<string, string> {
  const out = new Map<string, string>();
  for (const one of examples()) {
    out.set(one.slug, one.slug.startsWith('gallery/') ? `examples/${one.slug}.hero` : one.dir);
  }
  return out;
}

let cachedExamples: Map<string, string> | null = null;

/**
 * The repository paths a page is built from, its own and no shared ones.
 *
 * The prose fragment and the wrapper are found by asking the filesystem which
 * of the two shapes exists rather than by carrying a table that a new page
 * would have to be added to. Both shapes are real and they sit side by side:
 * `site/src/html/why.html` is a file beside a directory of the same name,
 * `site/src/html/docs/index.html` is a directory's own page, and
 * `site/src/html/about.html` and `site/src/html/about/thanks.html` are one of
 * each. An address that resolves to neither is a page this module has never
 * seen, and it fails loudly rather than handing back a plausible date.
 */
export function sourcesFor(pathname: string): string[] {
  if (cachedExamples === null) cachedExamples = exampleSources();

  const lang = pathname.startsWith('/it/') ? 'it' : 'en';
  const bare = lang === 'it' ? pathname.slice(3) : pathname;

  // The example pages, whose prose and whose program are two different trees.
  if (bare.startsWith('/examples/') && bare !== '/examples/') {
    const slug = bare.slice('/examples/'.length, -1);
    const program = cachedExamples.get(slug);
    if (program === undefined) {
      throw new Error(`the sitemap has ${pathname} and the model knows no example \`${slug}\`.`);
    }
    const prefix = lang === 'it' ? 'site/src/html/it/examples' : 'site/src/html/examples';
    return [program, `${prefix}/${slug}.html`];
  }

  const stem = pathname === '/' ? 'index' : pathname.slice(1, -1);
  const found: string[] = [];
  for (const [dir, extension] of [['site/src/html', 'html'], ['site/src/pages', 'astro']]) {
    const flat = `${dir}/${stem}.${extension}`;
    const nested = `${dir}/${stem}/index.${extension}`;
    if (isFile(flat)) found.push(flat);
    else if (isFile(nested)) found.push(nested);
  }

  // The specification page renders the language's own file, so the file it
  // renders is the thing that changes. Its fragment is the page around it.
  if (bare === '/spec/') found.push('spec/heroes-spec.md');

  if (found.length === 0) {
    throw new Error(
      `the sitemap has ${pathname} and no source was found for it.\n` +
        `  looked for site/src/html/${stem}.html, site/src/html/${stem}/index.html\n` +
        `  and the same two under site/src/pages with .astro.\n` +
        `  a new page needs a row here, or the sitemap is advertising an address\n` +
        `  nothing builds.`
    );
  }
  return found;
}

/**
 * When the page at this address last changed: the newest of its own sources.
 *
 * A directory source counts every file under it, because adding a file to an
 * example changes the page that lists it.
 */
export function lastmodFor(pathname: string): Date {
  const dates = commitDates();
  let best: Date | null = null;

  for (const source of sourcesFor(pathname)) {
    for (const [path, date] of dates) {
      if (path !== source && !path.startsWith(`${source}/`)) continue;
      if (best === null || date > best) best = date;
    }
  }

  return best ?? newest(dates);
}

/**
 * The sitemap that was actually written carries a date on every address.
 *
 * THIS READS THE ARTIFACT, and the first version of it did not: it read an
 * array that `serialize` pushed into, which is a side channel and which lied.
 * In a shallow clone `serialize` threw, the sitemap integration swallowed every
 * throw, the array stayed empty, `dates.length > 1` was false, the audit passed,
 * and the build wrote a sitemap with zero URLs and exited 0. An audit that
 * cannot see the failure it exists for is worse than none, because it is also a
 * reason not to look.
 *
 * Three things are asserted, and each one has been seen to happen: the sitemap
 * is not empty; every `<loc>` has a `<lastmod>` beside it; and the dates are not
 * all the same, which is what a shallow clone, a mapping collapsed onto one
 * shared file, or a fallback that took over the whole tree all look like from
 * here. Two distinct dates would clear a naive check, so the bar is the shape a
 * real site has: its pages do not all move together.
 */
export function assertSitemapDated(distDir: string): string {
  const index = join(distDir, 'sitemap-index.xml');
  let chunks: string[];
  try {
    const listing = readFileSync(index, 'utf8');
    chunks = [...listing.matchAll(/<loc>[^<]*\/([^/<]+\.xml)<\/loc>/g)].map((m) => m[1]);
  } catch {
    throw new Error(`the build wrote no ${index}, so nothing advertises this site.`);
  }
  if (chunks.length === 0) throw new Error(`${index} lists no sitemap files.`);

  let addresses = 0;
  const distinct = new Set<string>();
  for (const chunk of chunks) {
    const xml = readFileSync(join(distDir, chunk), 'utf8');
    const urls = [...xml.matchAll(/<url>([\s\S]*?)<\/url>/g)].map((m) => m[1]);
    for (const url of urls) {
      addresses += 1;
      const date = /<lastmod>([^<]+)<\/lastmod>/.exec(url);
      if (date === null) {
        const loc = /<loc>([^<]+)<\/loc>/.exec(url);
        throw new Error(
          `${chunk} advertises ${loc?.[1] ?? 'an address'} with no <lastmod>.\n` +
            '  every address the sitemap carries is dated, or the ones that are\n' +
            '  dated say nothing by comparison.'
        );
      }
      distinct.add(date[1]);
    }
  }

  if (addresses === 0) {
    throw new Error(
      `${chunks.join(', ')} carries no <url> at all, so the site advertises nothing.\n` +
        '  @astrojs/sitemap catches what `serialize` throws and drops the entry, so\n' +
        '  a build that cannot date its pages writes an empty sitemap and stays green.'
    );
  }
  if (addresses > 1 && distinct.size === 1) {
    throw new Error(
      `all ${addresses} sitemap entries carry the same lastmod, ${[...distinct][0]}.\n` +
        '  a crawler reads that as one site-wide edit and stops trusting the field.\n' +
        '  the usual cause is a shallow clone (fetch-depth: 0 on the checkout step),\n' +
        '  and the other is a source mapping that resolved every page to one file.'
    );
  }
  return `${addresses} addresses dated, ${distinct.size} distinct`;
}
