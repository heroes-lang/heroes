/**
 * Where the repository is, and how this build reads the files it shows.
 *
 * The examples pages are generated from `examples/` at build time, so nothing
 * under `site/` is a copy of a program and no copy can go stale. That means the
 * build reads files OUTSIDE `site/`, which is the one thing an Astro project
 * does not do by default, and this module is the only place that happens.
 *
 * Why the root is found by walking up rather than taken from Astro. Astro
 * exposes `root` through `astro:config/server`, and that works inside a build.
 * It does not work inside a plain `node` script, and the round-trip check that
 * guards the highlighter is exactly such a script: it runs the tokenizer over
 * every `.hero` file in the tree and asserts the spans it produces strip back to
 * the source bytes. Tying this module to Astro would put that check out of
 * reach, so the root is a fact about the filesystem in hand instead: the nearest
 * directory at or above the working directory that carries all three markers of
 * this repository. It fails loudly, naming what it looked for and where it
 * started, because a silently wrong root would render pages from nothing
 * (CLAUDE.md § 11, a narrowing asks the value and never the world).
 */

import { appendFileSync, mkdirSync, readdirSync, readFileSync, statSync } from 'node:fs';
import { join, dirname, sep } from 'node:path';

/**
 * The three files that together mean "this is the Heroes repository".
 *
 * One marker would be a guess: `README.md` exists in half the world's
 * directories. These three are the tree this build actually reads from, and the
 * pages are generated from the first two.
 */
const MARKERS = ['examples/README.md', 'spec/heroes-spec.md', 'CLAUDE.md'];

let cachedRoot: string | null = null;

function looksLikeRoot(dir: string): boolean {
  return MARKERS.every((marker) => {
    try {
      return statSync(join(dir, ...marker.split('/'))).isFile();
    } catch {
      return false;
    }
  });
}

/**
 * The repository root, as an absolute path with no trailing separator.
 *
 * Memoised: a build renders 178 pages off one model, and the walk is pointless
 * after the first answer.
 */
export function repoRoot(): string {
  if (cachedRoot !== null) return cachedRoot;

  const start = process.cwd();
  let dir = start;

  for (;;) {
    if (looksLikeRoot(dir)) {
      cachedRoot = dir;
      return dir;
    }
    const parent = dirname(dir);
    if (parent === dir) break;
    dir = parent;
  }

  throw new Error(
    `the site build cannot find the repository root.\n` +
      `  started at: ${start}\n` +
      `  looked upward for a directory holding all of: ${MARKERS.join(', ')}\n` +
      `  run the build from inside the repository (npm run build from site/).`
  );
}

/** An absolute path for a repository-relative POSIX path. */
export function absolute(relative: string): string {
  return join(repoRoot(), ...relative.split('/'));
}

/**
 * Every repository path this build reads, written down as it is read, for the
 * end-of-build audit that holds `.github/workflows/deploy-site.yml`'s `paths:`
 * to them (`triggers.ts`).
 *
 * A FILE rather than a Set, for the reason `claims.ts` gives for its own: the
 * pages run this code from Astro's prerender bundle and the build-done hook
 * imports it from source, so the two never share a module's memory, and what
 * they share is the output directory. The Set below only keeps one instance of
 * this module from writing a path twice. The audit reads the file and deletes
 * it, so it never ships.
 *
 * The output is not an input: a read under `site/dist/` is the build reading
 * what it wrote, and nothing a push changes.
 */
export const READS_FILE = 'site/dist/.reads';

const recorded = new Set<string>();

function record(relative: string): void {
  if (relative === 'site/dist' || relative.startsWith('site/dist/')) return;
  if (recorded.has(relative)) return;
  recorded.add(relative);
  const file = absolute(READS_FILE);
  mkdirSync(dirname(file), { recursive: true });
  appendFileSync(file, `${relative}\n`);
}

/** The bytes of a repository-relative file. The fingerprint hashes these. */
export function readBytes(relative: string): Buffer {
  record(relative);
  return readFileSync(absolute(relative));
}

/**
 * A path a subprocess reads on the build's behalf, written down as a read. The
 * compiler `lexed.ts` runs is built from `seed/heroes.c` and `runtime/`, which
 * nothing here opens, and a change to either changes what the colouring is held
 * to.
 */
export function recordRead(relative: string): void {
  record(relative);
}

/**
 * The text of a repository-relative file, decoded as UTF-8.
 *
 * `.gitattributes` forces `* text=auto eol=lf`, so a file in this tree holds no
 * CR and the text needs no normalising. If that ever changes the round-trip
 * check is what says so, because a CR would survive into the page and not back.
 */
export function readText(relative: string): string {
  return readBytes(relative).toString('utf8');
}

/** Whether a repository-relative path is a regular file. */
export function isFile(relative: string): boolean {
  // Asked of a path that may not exist, and recorded all the same: a file whose
  // absence a page depends on changes the page the day somebody creates it.
  record(relative);
  try {
    return statSync(absolute(relative)).isFile();
  } catch {
    return false;
  }
}

/** The repository-relative names of the directories directly inside `relative`. */
export function directoriesIn(relative: string): string[] {
  record(relative);
  return readdirSync(absolute(relative), { withFileTypes: true })
    .filter((entry) => entry.isDirectory() && !entry.name.startsWith('.'))
    .map((entry) => `${relative}/${entry.name}`)
    .sort(byBytes);
}

/** The repository-relative names of the regular files directly inside `relative`. */
export function filesIn(relative: string): string[] {
  record(relative);
  return readdirSync(absolute(relative), { withFileTypes: true })
    .filter((entry) => entry.isFile() && !entry.name.startsWith('.'))
    .map((entry) => `${relative}/${entry.name}`)
    .sort(byBytes);
}

/**
 * Every regular file under `relative`, recursively, dotfiles excluded, sorted
 * bytewise by repository-relative path.
 *
 * The order is the fingerprint's order, so it has to be one a reader can
 * reproduce with `sort` and not a locale's idea of alphabetical. `Intl`
 * collation would put `main.args` and `main.hero` in an order that depends on
 * the machine's language, and a fingerprint that moves with the locale is a
 * fingerprint nobody can check.
 */
export function walk(relative: string): string[] {
  record(relative);
  const out: string[] = [];

  const visit = (dir: string) => {
    for (const entry of readdirSync(absolute(dir), { withFileTypes: true })) {
      if (entry.name.startsWith('.')) continue;
      const child = `${dir}/${entry.name}`;
      if (entry.isDirectory()) visit(child);
      else if (entry.isFile()) out.push(child);
    }
  };

  visit(relative);
  return out.sort(byBytes);
}

/**
 * Bytewise string order, the one `LC_ALL=C sort` gives.
 *
 * `String.prototype.localeCompare` is the trap here and `<` is the answer:
 * JavaScript compares strings by UTF-16 code unit, which for the ASCII paths in
 * this tree is byte order.
 */
export function byBytes(a: string, b: string): number {
  if (a < b) return -1;
  if (a > b) return 1;
  return 0;
}

/** The last path segment of a repository-relative path. */
export function basename(relative: string): string {
  const cut = relative.lastIndexOf('/');
  return cut < 0 ? relative : relative.slice(cut + 1);
}

/**
 * How many lines a file has, counted the way `wc -l` counts: the number of
 * newlines. Every file in this tree ends in one, so this is also the number of
 * lines a reader sees.
 */
export function countLines(text: string): number {
  let lines = 0;
  for (let i = 0; i < text.length; i += 1) if (text.charCodeAt(i) === 10) lines += 1;
  return lines;
}

/** The platform separator, exported so a test can say what it expected. */
export const PATH_SEPARATOR = sep;
