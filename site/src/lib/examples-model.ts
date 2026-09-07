/**
 * Every program under `examples/`, as the pages need it.
 *
 * The site shows the corpus itself rather than a copy of it, so this module is
 * the whole reading: it discovers the programs the way the net does, by looking,
 * and everything a page states about one is derived here and nowhere else.
 * `examples/README.md` states the rule the discovery follows in its own words:
 * a directory holding a `main.hero` IS a program, so a program joins by
 * existing and cannot be forgotten. `gallery/` is the one directory that is not
 * a program directory, because each of its twelve files is its own program.
 *
 * Three things are worth knowing before changing anything here.
 *
 * Every `.hero` is highlighted ONCE, and the two editions share the result.
 * That is not a saving, it is the guarantee that the Italian page cannot show a
 * different program from the English one: there is one string of markup and
 * both pages embed it.
 *
 * A shape this module does not recognise is a build error, never a skip. A new
 * kind of thing under `examples/` would otherwise be a program the site quietly
 * stops showing, which is exactly the failure the corpus suite's floor exists to
 * prevent on the testing side.
 *
 * The badges are derived from the program's TOKENS, not from a grep, because
 * `read_file` inside a comment is prose about a program and not a program that
 * reads a file.
 */

import { readText, walk, directoriesIn, isFile, countLines, byBytes, basename } from './repo.ts';
import { highlight, words, assertRoundTrip } from './highlight.ts';
import { fingerprintOf, reproduceCommand, type Fingerprint } from './fingerprint.ts';
import { splitExpectation, outputLines, type Expectation } from './expectation.ts';

const EXAMPLES = 'examples';
const GALLERY = 'examples/gallery';

/** Files that are the harness's inputs rather than the program's own data. */
const HARNESS_FILES = new Set(['main.args', 'main.expected', 'main.stdin']);

/** The floor the corpus suite carries, and for the same reason: a walk that finds fewer programs than last time is a broken walk, not a smaller corpus. */
const PROGRAM_FLOOR = 44;
const GALLERY_FLOOR = 10;

export interface SourceFile {
  /** Repository-relative path, which is what a figure's `data-src` carries. */
  path: string;
  /** The name a reader sees, relative to the example's directory. */
  name: string;
  lines: number;
  /** The coloured HTML, built once and shared by both editions. */
  html: string;
}

export interface Example {
  /** `json`, or `gallery/03-fallible`. The URL is `/examples/<slug>/`. */
  slug: string;
  kind: 'program' | 'gallery';
  /** The directory the fingerprint is taken over. */
  dir: string;
  /** `main.hero` first, then the other modules in bytewise order. */
  modules: SourceFile[];
  /** The data files the program reads, and `main.stdin` where there is one. */
  inputs: SourceFile[];
  args: string[];
  stdinPath: string | null;
  expectation: Expectation | null;
  /** Why there is no recorded output, where there is none. */
  noOutputBecause: string | null;
  /** The exact shell line, ready to print in a terminal block. */
  command: string;
  lines: number;
  /** The headers the program names in an `extern` group, in source order. */
  externs: string[];
  /** The libraries it links, and the packages it asks the machine to find. */
  links: string[];
  packages: string[];
  threads: boolean;
  readsFile: boolean;
  writesFile: boolean;
  usesArgs: boolean;
  fingerprint: Fingerprint;
  reproduce: string;
}

let cached: Example[] | null = null;

/**
 * Quote one argument for display in the command line.
 *
 * Display, not execution: the block on the page is something a reader copies
 * into their own shell, so an argument holding a space has to survive the paste.
 * `argv/main.args` carries one, which is why this exists.
 */
function shellQuote(argument: string): string {
  if (/^[A-Za-z0-9_./=:-]+$/.test(argument)) return argument;
  return `'${argument.replace(/'/g, `'\\''`)}'`;
}

/** One `.hero` file, highlighted and round-trip checked on the way in. */
function sourceFile(path: string, dir: string): SourceFile {
  const text = readText(path);
  assertRoundTrip(text, path);
  return { path, name: path.slice(dir.length + 1), lines: countLines(text), html: highlight(text) };
}

/** A data file the program reads, escaped but not coloured: it is not Heroes. */
function inputFile(path: string, dir: string): SourceFile {
  const text = readText(path);
  return {
    path,
    name: path.slice(dir.length + 1),
    lines: countLines(text),
    html: text.replace(/[&<>]/g, (ch) => (ch === '&' ? '&amp;' : ch === '<' ? '&lt;' : '&gt;')),
  };
}

/**
 * The `extern` groups a program declares, read at column 0.
 *
 * Column 0 is the harness's own test and the reason is the same: a header named
 * inside a comment or a string is not a header the program binds, and
 * `examples/sdl/main.hero` names its own `extern` line inside its header
 * comment, four spaces in.
 */
function externGroups(text: string): { header: string; link: string | null; pkg: string | null }[] {
  const groups: { header: string; link: string | null; pkg: string | null }[] = [];
  for (const line of text.split('\n')) {
    if (!line.startsWith('extern "')) continue;
    const header = /^extern "([^"]+)"/.exec(line);
    if (header === null) continue;
    groups.push({
      header: header[1],
      link: /\blink "([^"]+)"/.exec(line)?.[1] ?? null,
      pkg: /\bpackage "([^"]+)"/.exec(line)?.[1] ?? null,
    });
  }
  return groups;
}

function buildExample(slug: string, kind: 'program' | 'gallery', dir: string, heroPaths: string[], otherPaths: string[]): Example {
  const modules = heroPaths.map((path) => sourceFile(path, dir));
  const sources = heroPaths.map((path) => readText(path));
  const text = sources.join('\n');

  const argsPath = `${dir}/main.args`;
  const args =
    kind === 'program' && isFile(argsPath)
      ? readText(argsPath)
          .split('\n')
          .filter((line) => line.length > 0)
      : [];

  if (kind === 'program' && isFile(argsPath) && args.length === 0) {
    throw new Error(
      `${argsPath} is present and empty.\n` +
        `  a program that takes arguments has to say which, because the page prints the\n` +
        `  command a reader will copy.`
    );
  }

  const stdinPath = kind === 'program' && isFile(`${dir}/main.stdin`) ? `${dir}/main.stdin` : null;

  const expectedPath = `${dir}/main.expected`;
  const expectation =
    kind === 'program' && isFile(expectedPath) ? splitExpectation(readText(expectedPath), expectedPath) : null;

  const entry = kind === 'program' ? `${dir}/main.hero` : heroPaths[0];
  let command = `$ heroes run ${entry}`;
  if (args.length > 0) command += ` -- ${args.map(shellQuote).join(' ')}`;
  if (stdinPath !== null) command += ` < ${stdinPath}`;

  const groups = externGroups(text);
  const vocabulary = words(text);

  // Why there is no output, said in the program's own terms rather than left
  // blank. `curl/` prints this machine's libcurl version, which is why
  // `examples/README.md` says it carries no expectation; the gallery files are
  // read rather than run, and one of them exits 1 on purpose.
  let noOutputBecause: string | null = null;
  if (expectation === null) {
    noOutputBecause =
      kind === 'gallery'
        ? 'gallery'
        : groups.some((group) => group.header === 'curl/curl.h')
          ? 'machine'
          : 'unrecorded';
  }

  const inputs = otherPaths
    .filter((path) => !HARNESS_FILES.has(basename(path)))
    .map((path) => inputFile(path, dir));
  if (stdinPath !== null) inputs.push(inputFile(stdinPath, dir));

  const allPaths = [...heroPaths, ...otherPaths].sort(byBytes);

  return {
    slug,
    kind,
    dir,
    modules,
    inputs,
    args,
    stdinPath,
    expectation,
    noOutputBecause,
    command,
    lines: modules.reduce((total, module) => total + module.lines, 0),
    externs: groups.map((group) => group.header),
    links: groups.map((group) => group.link).filter((one): one is string => one !== null),
    packages: groups.map((group) => group.pkg).filter((one): one is string => one !== null),
    threads: vocabulary.has('hero_thread_spawn'),
    readsFile: vocabulary.has('read_file'),
    writesFile: vocabulary.has('write_file'),
    usesArgs: vocabulary.has('args') || vocabulary.has('args_checked'),
    fingerprint: fingerprintOf(dir, allPaths),
    reproduce: reproduceCommand(dir),
  };
}

/**
 * Every example, programs then gallery files, each in bytewise order.
 *
 * Memoised, because a build renders one page per example per edition off this
 * and reading 193 files twice would be reading them 176 times.
 */
export function examples(): Example[] {
  if (cached !== null) return cached;

  const programs: Example[] = [];
  for (const dir of directoriesIn(EXAMPLES)) {
    if (dir === GALLERY) continue;
    if (!isFile(`${dir}/main.hero`)) {
      throw new Error(
        `${dir} is under examples/ and holds no main.hero.\n` +
          `  examples/README.md says a directory holding a main.hero IS a program, so this\n` +
          `  is either a program missing its entry point or a new kind of thing the site\n` +
          `  does not know how to show. Either way it is not silently skipped.`
      );
    }
    const all = walk(dir);
    const hero = all.filter((path) => path.endsWith('.hero'));
    const entry = `${dir}/main.hero`;
    const ordered = [entry, ...hero.filter((path) => path !== entry)];
    programs.push(
      buildExample(dir.slice(EXAMPLES.length + 1), 'program', dir, ordered, all.filter((path) => !path.endsWith('.hero')))
    );
  }

  const gallery: Example[] = [];
  for (const path of walk(GALLERY).filter((one) => one.endsWith('.hero'))) {
    const name = basename(path).replace(/\.hero$/, '');
    gallery.push(buildExample(`gallery/${name}`, 'gallery', GALLERY, [path], []));
  }

  if (programs.length < PROGRAM_FLOOR) {
    throw new Error(
      `the walk found ${programs.length} programs under examples/ and the floor is ${PROGRAM_FLOOR}.\n` +
        `  fewer programs than last time is a broken walk, not a smaller corpus.`
    );
  }
  if (gallery.length < GALLERY_FLOOR) {
    throw new Error(
      `the walk found ${gallery.length} files under examples/gallery/ and the floor is ${GALLERY_FLOOR}.`
    );
  }

  cached = [...programs, ...gallery];
  return cached;
}

/** One example by slug, or a throw naming what exists. */
export function exampleBySlug(slug: string): Example {
  const found = examples().find((one) => one.slug === slug);
  if (found === undefined) {
    throw new Error(`no example is called \`${slug}\`. The slugs are: ${examples().map((one) => one.slug).join(' ')}`);
  }
  return found;
}

/** The totals the index page states about itself, computed rather than typed. */
export function totals(): { programs: number; gallery: number; files: number; lines: number } {
  const all = examples();
  const programs = all.filter((one) => one.kind === 'program');
  const gallery = all.filter((one) => one.kind === 'gallery');
  return {
    programs: programs.length,
    gallery: gallery.length,
    files: all.reduce((total, one) => total + one.modules.length, 0),
    lines: all.reduce((total, one) => total + one.lines, 0),
  };
}

export { outputLines };
