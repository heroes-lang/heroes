/**
 * The order the examples are read in, and the shelves they sit on.
 *
 * This file holds no prose. It answers two questions the corpus itself cannot:
 * which programs belong together for somebody who has never seen the language,
 * and which one to read first. The descriptions live in
 * `site/src/html/examples/`, one short fragment per example per edition, where
 * anybody can edit them without opening a TypeScript file.
 *
 * The gallery comes first because it is the only group written to be READ
 * rather than tested, twelve programs with one idea each in reading order
 * (`examples/README.md` § gallery). Everything after it is the corpus, grouped
 * by what a visitor is looking for rather than by what the program exercises:
 * the corpus README indexes by what a program can get wrong, which is the right
 * index for the people maintaining it and the wrong one for somebody deciding
 * whether this language can open a database.
 *
 * The flattened order is also the previous and next order on every page, so a
 * reader who keeps clicking Next walks the gallery, then the shelves, and lands
 * nowhere unexpected.
 *
 * The build asserts this covers the world in both directions: every example is
 * in exactly one group, and every slug here names an example. That is why the
 * groups are checked rather than trusted, and it is measured against the walk
 * rather than against this list.
 */

export interface Group {
  /** Used in the URL of nothing: it is a heading id and a JSON field. */
  key: string;
  en: string;
  it: string;
  slugs: string[];
}

export const groups: Group[] = [
  {
    key: 'gallery',
    en: 'Start here',
    it: 'Da dove partire',
    slugs: [
      'gallery/00-first',
      'gallery/01-points',
      'gallery/02-tokens',
      'gallery/03-fallible',
      'gallery/04-loops',
      'gallery/05-mutation',
      'gallery/06-generics',
      'gallery/07-strings',
      'gallery/08-ffi',
      'gallery/09-holes',
      'gallery/10-maps',
      'gallery/11-trees',
      'gallery/12-interpolation',
    ],
  },
  {
    key: 'text',
    en: 'Reading and processing text',
    it: 'Lettura e analisi del testo',
    slugs: [
      'calculator',
      'interpreter',
      'json',
      'markdown',
      'csv',
      'ini',
      'template',
      'rpn',
      'assembler',
      'roman',
      'words',
      'wrap',
    ],
  },
  {
    key: 'numbers',
    en: 'Numbers and algorithms',
    it: 'Numeri e algoritmi',
    slugs: [
      'sieve',
      'nbody',
      'spectral',
      'fannkuch',
      'binarytrees',
      'checksum',
      'diff',
      'dates',
      'floats',
      'widths',
      'palette',
    ],
  },
  {
    key: 'data',
    en: 'Data structures',
    it: 'Strutture dati',
    slugs: ['query', 'spreadsheet', 'tree', 'routes', 'deck', 'readings', 'shapes'],
  },
  {
    key: 'files',
    en: 'Files and the command line',
    it: 'File e riga di comando',
    slugs: ['todo', 'logs', 'argv', 'tally', 'pipeline'],
  },
  {
    key: 'c',
    en: 'C libraries',
    it: 'Librerie C',
    slugs: ['sqlite', 'ledger', 'curl', 'raylib', 'sdl', 'ctime'],
  },
  {
    key: 'threads',
    en: 'Threads',
    it: 'Thread',
    slugs: [
      'threads',
      'mandelbrot',
      'matmul',
      'dotproduct',
      'montecarlo',
      'nqueens',
      'collatz',
      'firsthit',
      'histogram',
      'wordbands',
    ],
  },
  {
    key: 'games',
    en: 'Games and simulations',
    it: 'Giochi e simulazioni',
    slugs: ['adventure', 'maze', 'board'],
  },
];

/** Every slug, in reading order. This is the previous and next order. */
export function readingOrder(): string[] {
  return groups.flatMap((group) => group.slugs);
}

/** Which group a slug is on, or undefined if the list has a hole in it. */
export function groupOf(slug: string): Group | undefined {
  return groups.find((group) => group.slugs.includes(slug));
}
