/**
 * Every program under `examples/` and `rebel/examples/`, as the pages need it.
 *
 * The site shows the corpus itself rather than a copy of it, so this module is
 * the whole reading: it discovers the programs the way the net does, by looking,
 * and everything a page states about one is derived here and nowhere else.
 * `examples/README.md` states the rule the discovery follows in its own words:
 * a directory holding a `main.hero` IS a program, so a program joins by
 * existing and cannot be forgotten. One directory is not a program
 * directory: `gallery/`, because each of its files is its own program. The
 * local static index (panel 235's R15) stands beside `examples/` at the
 * repository's root, `packages/` (panel 238's R10), and holds Rebel's tree
 * today; it is checked for its shape on every build, and Rebel's page and its
 * reference pages read it (`reference.ts`).
 *
 * **The examples are two groups** (`.claude/rules/rebel.md`, decision 7 of
 * M-rebel-library): `examples/` uses no library, and `rebel/examples/` holds
 * the programs written with Rebel. The second group's programs are its
 * folders, each a project holding a `main.hero` and its own catalogue, told
 * apart as the check of Rebel's programs tells them
 * (`rebel/examples/README.md` § A10, `tests/harness/rebel_apps_run.hero`):
 * **an application** holds its own `main.expected`, its demo's transcript;
 * **a twin** holds none and bears the name of a program of `examples/`, its
 * original, and is run with the original's arguments and input and held to
 * the original's output, which is what its page shows. One folder is no
 * program: `rebel/examples/reach/` holds the packages' drivers,
 * `reach_<package>.hero` (§ A2), each package read on its reference page and
 * not here, and their one catalogue (§ A10). A file at the top of
 * `rebel/examples/` is the README or one of the two indexes; anything else
 * there or in the drivers' folder, a folder that is neither, an application
 * bearing an original's name, or a twin declaring what the check reads of an
 * application alone, is a build error. Their slugs open
 * `rebel/`, so no name of one group can be read as the other's, and a twin's
 * page and its original's each link the other, the same program with Rebel
 * and without.
 *
 * A program that uses a package carries two more kinds of file, and each is
 * read as what it is: `heroes.toml`, its catalogue, a document shown beside the
 * program; and the files under `heroes.packages/`, the trees `heroes add`
 * placed, the package's and never the program's own, so they count in no
 * module, line or badge of the program and are shown apart, each tree under
 * the entry of the catalogue that names it.
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

import { readText, walk, directoriesIn, isDirectory, isFile, countLines, byBytes, basename } from './repo.ts';
import { highlight, words, assertRoundTrip } from './highlight.ts';
import { fingerprintOf, reproduceCommand, type Fingerprint } from './fingerprint.ts';
import { splitExpectation, outputLines, type Expectation } from './expectation.ts';

const EXAMPLES = 'examples';
const GALLERY = 'examples/gallery';

/**
 * The local static index (panel 235's R15), at the root (panel 238's R10). It
 * held Rebel's source until Rebel's library left it for `rebel/library` (the
 * author's 1a of 2026-11-23), and no index stands in the tree until one a client
 * can read returns; the day it does, `checkIndex` holds its shape.
 */
const INDEX = 'packages';

/** The programs written with Rebel, the second group. */
const REBEL = 'rebel/examples';

/**
 * The drivers' folder of `rebel/examples/`, holding every package's driver and
 * their one catalogue (README § A10), as `tests/harness/rebel_apps_run.hero`'s
 * `DRIVERS` names it: no program, so it has no page.
 */
const DRIVERS = `${REBEL}/reach`;

/** A package's driver in the drivers' folder (README § A2). */
const DRIVER = /^reach_[a-z0-9_]+\.hero$/;

/** A program's catalogue, beside its `main.hero` (spec § 1). */
const CATALOGUE = 'heroes.toml';

/**
 * The two indexes at the top of `rebel/examples/`, written by
 * `rebel/generators/rebel_index/` and held to the tree by its `check`: the programs'
 * short list and Rebel's packages. Pages for a reader, which no program opens,
 * so they are shown under no heading, as a program's own `README.md` is.
 */
const REBEL_INDEXES = new Set(['INDEX.md', 'PACKAGES.md']);

/** The folder a fetch places packages in, which no `use` can spell (panel 235's R7). */
const PLACED = 'heroes.packages';

/**
 * Files that are the harness's inputs rather than the program's own data:
 * the corpus's three, and the two an application of `rebel/examples/`
 * declares about the machines its demo runs on, which the check of Rebel's
 * programs reads (`rebel/examples/README.md` § A10).
 */
const HARNESS_FILES = new Set(['main.args', 'main.expected', 'main.stdin', 'main.varies', 'main.skips']);

/**
 * A file a program's folder holds for its readers and never opens: the folder's
 * own `README.md`. Shown under no heading. A package's card, which stood here
 * while each program kept a copy under `core/`, is a file of the package's tree
 * now, shown with it.
 */
function isDocument(path: string, dir: string): boolean {
  return path.slice(dir.length + 1) === 'README.md';
}

/** The floor the corpus suite carries, and for the same reason: a walk that finds fewer programs than last time is a broken walk, not a smaller corpus. */
const PROGRAM_FLOOR = 44;
const GALLERY_FLOOR = 10;

/** The drivers `rebel/examples/` held when it was first read: 160 (194 when they moved into `reach/`, 2026-11-23). */
const DRIVER_FLOOR = 150;

export interface SourceFile {
  /** Repository-relative path, which is what a figure's `data-src` carries. */
  path: string;
  /** The name a reader sees, relative to the example's directory. */
  name: string;
  lines: number;
  /** The coloured HTML, built once and shared by both editions. */
  html: string;
}

/** One tree `heroes add` placed under a program's `heroes.packages/`. */
export interface FetchedPackage {
  /** The catalogue's entry that names it, `rebel`. */
  name: string;
  version: string;
  /** The tree's folder, repository-relative. */
  dir: string;
  /** Every file of the tree, bytewise: `.hero` coloured, any other escaped. */
  files: SourceFile[];
}

export interface Example {
  /** `json`, `gallery/03-fallible` or `rebel/jq`. The URL is `/examples/<slug>/`. */
  slug: string;
  /** A folder holding a `main.hero`, or one file of the gallery. */
  kind: 'program' | 'gallery';
  /** Which group it is in: the programs with no library, or those written with Rebel. */
  group: 'examples' | 'rebel';
  /** The other group's program bearing its name, where one does: a twin's original, or an original's twin. */
  twin: string | null;
  /** The directory the fingerprint is taken over. */
  dir: string;
  /**
   * The folder its `main.args`, `main.stdin` and `main.expected` are read
   * from: its own, or a twin's original's, since a twin is run with its
   * original's and held to its original's output (README § A10).
   */
  harnessDir: string;
  /** The `main.expected` its output pane is cut from, where it has one. */
  expectedPath: string | null;
  /** `main.hero` first, then the other modules in bytewise order. */
  modules: SourceFile[];
  /** The data files the program reads, and `main.stdin` where there is one. */
  inputs: SourceFile[];
  /** The program's catalogue, `heroes.toml`, where it uses a package. */
  catalogue: SourceFile | null;
  /** The packages it fetched, each the package's files and never its own. */
  fetched: FetchedPackage[];
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
      pkg: /\bpkgconfig "([^"]+)"/.exec(line)?.[1] ?? null,
    });
  }
  return groups;
}

/**
 * The entries a catalogue names, each with its version, read for the trees they
 * name: `heroes add` writes a table `[dependencies.<name>]` and its `version`.
 * The catalogue's whole shape is the net's to hold (the `packages` suite); a
 * tree no entry names is what this reader exists to refuse.
 */
function catalogueEntries(text: string): { name: string; version: string }[] {
  const entries: { name: string; version: string }[] = [];
  for (const line of text.split('\n')) {
    const table = /^\[dependencies\.([a-z][a-z0-9_]*)\]$/.exec(line);
    if (table !== null) {
      entries.push({ name: table[1], version: '' });
      continue;
    }
    const version = /^version = "([^"]*)"$/.exec(line);
    if (version !== null && entries.length > 0) entries[entries.length - 1].version = version[1];
  }
  return entries;
}

/**
 * The trees under a program's `heroes.packages/`, each under the entry of its
 * catalogue that names it. A tree is named `<name>@<version>-<16 hex>` by the
 * compiler (`selfhost/module/placed.hero`), so a folder of another shape, a
 * file standing in the folder itself, a tree no entry names, or trees with no
 * catalogue at all are each a build error naming the file, never a skip.
 */
function fetchedOf(dir: string, paths: string[], catalogue: string | null): FetchedPackage[] {
  if (paths.length === 0) return [];
  const folder = `${dir}/${PLACED}`;
  if (catalogue === null) {
    throw new Error(
      `${folder} holds packages and ${dir} has no ${CATALOGUE}.\n` +
        `  a tree there is placed by \`heroes add\` for an entry of the catalogue,\n` +
        `  so one with no catalogue is left behind: remove it, or add the package.`
    );
  }
  const entries = catalogueEntries(catalogue);
  const trees = new Map<string, string[]>();
  for (const path of paths) {
    const rest = path.slice(folder.length + 1);
    const cut = rest.indexOf('/');
    if (cut < 0) {
      throw new Error(`${path} stands in ${folder} itself, where a fetch places a folder per package and nothing else.`);
    }
    const tree = rest.slice(0, cut);
    trees.set(tree, [...(trees.get(tree) ?? []), path]);
  }
  const out: FetchedPackage[] = [];
  for (const tree of [...trees.keys()].sort(byBytes)) {
    const named = /^([a-z][a-z0-9_]*)@([0-9]+\.[0-9]+\.[0-9]+)-[0-9a-f]{16}$/.exec(tree);
    if (named === null) {
      throw new Error(`${folder}/${tree} is not a tree a fetch places, \`<name>@<version>-<16 hex>\`.`);
    }
    if (!entries.some((entry) => entry.name === named[1] && entry.version === named[2])) {
      throw new Error(
        `${folder}/${tree} is a tree no entry of ${dir}/${CATALOGUE} names.\n` +
          `  \`heroes add\` places a tree for an entry of the catalogue, and no verb removes\n` +
          `  one left behind, which compiles nothing: remove it, or add the package it holds.`
      );
    }
    const files = (trees.get(tree) ?? []).sort(byBytes);
    out.push({
      name: named[1],
      version: named[2],
      dir: `${folder}/${tree}`,
      files: files.map((path) => (path.endsWith('.hero') ? sourceFile(path, dir) : inputFile(path, dir))),
    });
  }
  return out;
}

/**
 * The local static index, read for its shape: `<name>/versions`,
 * `<name>/<version>/list` and `<name>/<version>/files/<path>`, the layout the
 * fetch verb reads (`selfhost/fetch/index_source.hero`). It is shown on no page,
 * since every program shows the tree it placed from it, and a file of another
 * kind in it is a build error, never a skip. Where no index stands, its absence
 * is the one thing read.
 */
function checkIndex(): void {
  // Asked whether it stands, and recorded all the same (`isDirectory`): the
  // day an index returns, the push that brings it deploys and this reads it.
  if (!isDirectory(INDEX)) return;
  const version = '[0-9]+\\.[0-9]+\\.[0-9]+';
  const shapes = [
    new RegExp('^[a-z][a-z0-9_]*/versions$'),
    new RegExp(`^[a-z][a-z0-9_]*/${version}/list$`),
    new RegExp(`^[a-z][a-z0-9_]*/${version}/files/.+$`),
  ];
  for (const path of walk(INDEX)) {
    const rest = path.slice(INDEX.length + 1);
    if (shapes.some((shape) => shape.test(rest))) continue;
    throw new Error(
      `${path} is in ${INDEX}, the local static index, and is none of its files.\n` +
        `  an index holds <name>/versions, <name>/<version>/list and\n` +
        `  <name>/<version>/files/<path> (examples/README.md § The index).`
    );
  }
}

function buildExample(
  slug: string,
  kind: 'program' | 'gallery',
  dir: string,
  heroPaths: string[],
  otherPaths: string[],
  fetchedPaths: string[] = [],
  harnessDir: string = dir
): Example {
  const modules = heroPaths.map((path) => sourceFile(path, dir));
  const sources = heroPaths.map((path) => readText(path));
  const text = sources.join('\n');

  const argsPath = `${harnessDir}/main.args`;
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

  const stdinPath = kind === 'program' && isFile(`${harnessDir}/main.stdin`) ? `${harnessDir}/main.stdin` : null;

  const expectedFile = `${harnessDir}/main.expected`;
  const expectedPath = kind === 'program' && isFile(expectedFile) ? expectedFile : null;
  const expectation = expectedPath === null ? null : splitExpectation(readText(expectedPath), expectedPath);

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

  // What the program reads is asked of the program, never of its folder: a file
  // a line of its `main.args` names, or a file of a folder one names (`kanban`
  // serves the page in the folder its `--web` names), a header one of its
  // `extern` groups names, and `main.stdin`. A folder also holds files the
  // program never opens, the packages program's README and its cards among
  // them, which the folder rule listed as input (defect 2332). A file that is
  // neither read nor a document this module knows is a build error, never a
  // skip.
  const named = new Set(args);
  const underNamed = (path: string) => args.some((one) => path.startsWith(`${one.replace(/\/+$/, '')}/`));
  const headers = new Set(groups.map((group) => `${dir}/${group.header}`));
  const cataloguePath = `${dir}/${CATALOGUE}`;
  const catalogue = kind !== 'gallery' && otherPaths.includes(cataloguePath) ? inputFile(cataloguePath, dir) : null;
  const fetched = fetchedOf(dir, fetchedPaths, catalogue === null ? null : readText(cataloguePath));
  const inputs: SourceFile[] = [];
  for (const path of otherPaths) {
    if (HARNESS_FILES.has(basename(path))) continue;
    if (path === cataloguePath && catalogue !== null) continue;
    if (named.has(path) || underNamed(path) || headers.has(path)) {
      inputs.push(inputFile(path, dir));
      continue;
    }
    if (isDocument(path, dir)) continue;
    throw new Error(
      `${path} is in ${dir} and the program does not read it.\n` +
        `  no line of main.args names it or a folder holding it, no extern group names it as a header, and it\n` +
        `  is neither the folder's README.md, its catalogue nor a file of a package it\n` +
        `  fetched. A page lists under Input only what its program reads (defect 2332):\n` +
        `  name the file in main.args, or teach this module what kind of file it is.`
    );
  }
  // A twin reads what its original's arguments name, files of the original's
  // folder (README § A10), so they are its input too, in the order named.
  if (harnessDir !== dir) {
    for (const path of args) {
      if (path.startsWith(`${harnessDir}/`) && isFile(path)) inputs.push(inputFile(path, harnessDir));
    }
  }
  if (stdinPath !== null) inputs.push(inputFile(stdinPath, harnessDir));

  const allPaths = [...heroPaths, ...otherPaths, ...fetchedPaths].sort(byBytes);

  return {
    slug,
    kind,
    group: slug.startsWith('rebel/') ? 'rebel' : 'examples',
    twin: null,
    dir,
    harnessDir,
    expectedPath,
    modules,
    inputs,
    catalogue,
    fetched,
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
    reproduce: reproduceCommand(dir, kind === 'program' ? null : allPaths),
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

  // The index stands outside `examples/`, so no turn of the loop below reaches
  // it: it is checked here, on every build, or a file of another kind in it
  // would pass with the build green and blind (panel 238's R10).
  checkIndex();
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
    // A placed tree is the package's, so its files are none of the program's
    // own modules, inputs or documents (panel 235's R7).
    const placed = `${dir}/${PLACED}/`;
    const all = walk(dir);
    const fetchedPaths = all.filter((path) => path.startsWith(placed));
    const own = all.filter((path) => !path.startsWith(placed));
    const hero = own.filter((path) => path.endsWith('.hero'));
    const entry = `${dir}/main.hero`;
    const ordered = [entry, ...hero.filter((path) => path !== entry)];
    programs.push(
      buildExample(
        dir.slice(EXAMPLES.length + 1),
        'program',
        dir,
        ordered,
        own.filter((path) => !path.endsWith('.hero')),
        fetchedPaths
      )
    );
  }

  // A gallery file's other files are the headers it names in an `extern` and
  // that live beside it: `13-lease.hero` binds `13-lease.h`, seven lines of C
  // no library the three platforms share could stand in for, and a page that
  // showed the program without the header showed a program nobody could run
  // (found by the devex and marketing seats, 2026-09-09). A header the machine
  // provides, `math.h`, is not beside the file and is not shown.
  const gallery: Example[] = [];
  for (const path of walk(GALLERY).filter((one) => one.endsWith('.hero'))) {
    const name = basename(path).replace(/\.hero$/, '');
    const beside = externGroups(readText(path))
      .map((group) => `${GALLERY}/${group.header}`)
      .filter((header) => isFile(header));
    gallery.push(buildExample(`gallery/${name}`, 'gallery', GALLERY, [path], beside));
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

  const rebel = rebelPrograms(programs);

  // A twin is the same program in the two groups: `examples/<name>/` and the
  // folder of `rebel/examples/` named `<name>` that holds no transcript of its
  // own, run with the original's words. Each page links the other.
  for (const one of rebel) {
    if (one.harnessDir === one.dir) continue;
    const original = programs.find((program) => program.dir === one.harnessDir);
    if (original === undefined) {
      throw new Error(`${one.dir} is read as the twin of ${one.harnessDir}, which is no program of examples/.`);
    }
    one.twin = original.slug;
    original.twin = one.slug;
  }

  cached = [...programs, ...gallery, ...rebel];
  return cached;
}

/**
 * The programs written with Rebel, each a folder of `rebel/examples/` holding
 * a `main.hero`, told apart as the check of Rebel's programs tells them
 * (`tests/harness/rebel_apps_run.hero`, `program_named`): an application by
 * its own `main.expected`, read first, and a twin by its original, the
 * program of `examples/` bearing its name, whose words it is run with.
 * `originals` are `examples/`'s programs. The top of the folder holds the
 * README and the two indexes `rebel/generators/rebel_index/` writes
 * (`REBEL_INDEXES`), and the drivers' folder (`DRIVERS`) holds the drivers
 * and their catalogue (README § A2, § A10); any other file in either, a
 * folder inside the drivers', a folder holding no `main.hero`, a folder that
 * is neither an application nor a twin, an application bearing an original's
 * name, and a twin holding a file the check reads of an application alone
 * are shapes this module does not know, and build errors.
 */
function rebelPrograms(originals: Example[]): Example[] {
  const out: Example[] = [];
  let drivers = 0;
  for (const path of walk(REBEL)) {
    const rest = path.slice(REBEL.length + 1);
    if (rest.includes('/')) continue;
    if (rest === 'README.md' || REBEL_INDEXES.has(rest)) continue;
    throw new Error(
      `${path} is at the top of ${REBEL}/ and is none of what stands there: the README or the two\n` +
        `  indexes rebel/generators/rebel_index/ writes, INDEX.md and PACKAGES.md. A package's driver,\n` +
        `  \`reach_<package>.hero\`, stands in ${DRIVERS}/ beside its catalogue, and a program written\n` +
        `  with Rebel is a folder of its own, a project with its own catalogue (rebel/examples/README.md § A10).`
    );
  }
  for (const path of walk(DRIVERS)) {
    const rest = path.slice(DRIVERS.length + 1);
    if (rest === CATALOGUE) continue;
    if (!rest.includes('/') && DRIVER.test(rest)) {
      drivers += 1;
      continue;
    }
    throw new Error(
      `${path} is in ${DRIVERS}/ and is none of what stands there: a package's driver,\n` +
        `  \`reach_<package>.hero\`, or the drivers' catalogue, ${CATALOGUE} (rebel/examples/README.md § A2, § A10).`
    );
  }
  for (const dir of directoriesIn(REBEL)) {
    if (dir === DRIVERS) continue;
    if (!isFile(`${dir}/main.hero`)) {
      throw new Error(`${dir} is a folder of ${REBEL}/ holding no main.hero, so it is no program, and nothing else stands there.`);
    }
    const name = dir.slice(REBEL.length + 1);
    const original = originals.find((one) => one.dir === `${EXAMPLES}/${name}`) ?? null;
    const application = isFile(`${dir}/main.expected`);
    if (application && original !== null) {
      throw new Error(
        `${dir} is an application, holding its own main.expected, and bears the name of\n` +
          `  ${original.dir}/, a program of examples/. The site pairs the two groups' programs by name,\n` +
          `  so its page would link a program it is not the twin of: name the application otherwise\n` +
          `  (rebel/examples/README.md § A10; M-rebel-library, the author's answer 4a, \`pipeline\` named \`ingest\`).`
      );
    }
    if (!application && original === null) {
      throw new Error(
        `${dir} holds no main.expected, and ${EXAMPLES}/${name}/ holds no main.hero: it is neither an\n` +
          `  application, which carries its demo's transcript, nor a twin, which is held to its\n` +
          `  original's (rebel/examples/README.md § A10), so its page would show nothing true about it.`
      );
    }
    if (!application) {
      for (const file of ['main.args', 'main.stdin', 'main.varies', 'main.skips']) {
        if (isFile(`${dir}/${file}`)) {
          throw new Error(
            `${dir}/${file} stands in a twin's folder. A twin is run with its original's arguments and\n` +
              `  input and held to its original's output on every platform (rebel/examples/README.md § A10),\n` +
              `  so this file is one the check of Rebel's programs never reads: remove it.`
          );
        }
      }
    }
    const all = walk(dir);
    const placed = `${dir}/${PLACED}/`;
    const own = all.filter((path) => !path.startsWith(placed));
    const hero = own.filter((path) => path.endsWith('.hero'));
    const entry = `${dir}/main.hero`;
    out.push(
      buildExample(
        `rebel/${name}`,
        'program',
        dir,
        [entry, ...hero.filter((path) => path !== entry)],
        own.filter((path) => !path.endsWith('.hero')),
        all.filter((path) => path.startsWith(placed)),
        original === null || application ? dir : original.dir
      )
    );
  }
  if (drivers < DRIVER_FLOOR) {
    throw new Error(`${REBEL}/ holds ${drivers} drivers and the floor is ${DRIVER_FLOOR}: a walk that finds fewer is a broken walk.`);
  }
  return out;
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
