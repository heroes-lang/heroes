/**
 * Rebel's reference pages, made from what `heroes doc` prints and held equal to
 * it, and the page that lists Rebel's packages by layer.
 *
 * M-rebel-library's decision 22 (the author, 2026-11-16): the reference pages
 * are in English in both editions of the site, and `examples-rebel/README.md`
 * § A7 says where they come from, *the site's reference pages are made from
 * that output*. So no page here is written by hand. At every build each face of
 * Rebel, the module a program names (each `.hero` of the newest version's tree
 * neither under an `internal/` folder nor a floor, a module named `hero_<x>`,
 * the net's own reading in `tests/harness/rebel_tree.hero`), is put through the
 * compiler the build already needs (`lexed.ts`): `heroes doc <rebel>/<face>`,
 * run in a folder of `dist/` whose catalogue names the tree by a `path` entry
 * with no `packages` line, so every face is reached whatever any program lists.
 *
 * - **A face `heroes doc` accepts** (exit 0) has a page in each edition, and its
 *   body is the output itself, coloured by `markdown.ts` and taken back out of
 *   the HTML by `assertVerbatim` before it is used: the page shows the bytes the
 *   command printed, and a colouring that moved one would be a red build.
 * - **A face it refuses** (exit 1, its diagnostics on stderr) has no page: the
 *   page would be the site's words standing where the compiler's belong. It is
 *   listed as owed, with how many refusals the compiler gave, on Rebel's page,
 *   and **the build ends red while one is owed** (`assertReferenceWhole`, the
 *   last audit of `astro.config.mjs`): every face has a page equal to its
 *   output, or the check says which do not and why. The documentation debt is
 *   Rebel's to pay, in its comments, and the check goes green as it is paid.
 * - **Any other answer** (another exit, a refusal with no diagnostic, an
 *   acceptance with nothing printed) is the build's error at once, never a page
 *   or an owed line: it is the command failing, not the comments.
 *
 * The answers cross from the pages to the audit through a file of `dist/`, as
 * the claims audit's visits do, since the config and the pages load this module
 * twice; the audit removes the folder, so nothing of it is published.
 *
 * Rebel's page lists the packages by layer from README § A1's table, read from
 * the file at every build and never copied: a package's row, its layer and
 * what it holds, and its reference page or its owed line. A face no row names
 * stands in the row of the package whose path is the longest prefix of its own
 * (`tokenizer/tables/o200k_base` under `tokenizer/tables`), and a face no row
 * reaches, or a row no face answers, is a red build.
 */

import { execFile } from 'node:child_process';
import { availableParallelism } from 'node:os';
import { existsSync, mkdirSync, readFileSync, rmSync, writeFileSync } from 'node:fs';
import { absolute, byBytes, directoriesIn, readText, recordRead, walk } from './repo.ts';
import { colourMarkdown, assertVerbatim } from './markdown.ts';

const TREE = 'packages/rebel';
const GUIDE = 'examples-rebel/README.md';
const FOLDER = 'site/dist/.reference';
const VERDICTS = `${FOLDER}/verdicts.json`;

/** 148 faces today; a walk that finds fewer than this is a broken walk. */
const FACES_FLOOR = 140;

/** 145 rows of § A1 today. */
const ROWS_FLOOR = 140;

/** What the compiler answered of one face. */
export interface Reference {
  /** The face's path under the tree, `json/jq`. */
  face: string;
  /** What `heroes doc` printed, where it accepted the face; `null` where it refused it. */
  text: string | null;
  /** How many diagnostics its refusal held, 0 where it accepted. */
  refusals: number;
}

/** One row of README § A1's table. */
export interface Row {
  layer: number;
  package: string;
  /** What the package holds, as the README writes it, Markdown code spans and all. */
  holds: string;
  /** The faces this row stands for: its own, and those under it that no row names. */
  faces: string[];
}

/** The newest version of Rebel the tree holds, by its numbers. */
export function rebelVersion(): string {
  const versions = directoriesIn(TREE)
    .map((dir) => dir.slice(TREE.length + 1))
    .filter((name) => /^[0-9]+\.[0-9]+\.[0-9]+$/.test(name));
  if (versions.length === 0) throw new Error(`${TREE} holds no version folder, \`<major>.<minor>.<patch>\`.`);
  const key = (v: string) => v.split('.').map(Number);
  versions.sort((a, b) => {
    const [x, y] = [key(a), key(b)];
    for (let i = 0; i < 3; i += 1) if (x[i] !== y[i]) return x[i] - y[i];
    return 0;
  });
  return versions[versions.length - 1];
}

function filesRoot(): string {
  return `${TREE}/${rebelVersion()}/files`;
}

/** Every face of the newest version, bytewise. */
export function faces(): string[] {
  const root = filesRoot();
  const out = walk(root)
    .filter((path) => path.endsWith('.hero'))
    .map((path) => path.slice(root.length + 1, -'.hero'.length))
    .filter((rel) => {
      const parts = rel.split('/');
      return !parts.slice(0, -1).includes('internal') && !parts[parts.length - 1].startsWith('hero_');
    })
    .sort(byBytes);
  if (out.length < FACES_FLOOR) {
    throw new Error(`${root} holds ${out.length} faces and the floor is ${FACES_FLOOR}: a walk that finds fewer is a broken walk.`);
  }
  return out;
}

/** README § A1's rows, each with the faces it stands for. */
export function rows(): Row[] {
  const text = readText(GUIDE);
  const start = text.indexOf('\n## A1.');
  const end = text.indexOf('\n## A2.');
  if (start < 0 || end < start) throw new Error(`${GUIDE}: no section between \`## A1.\` and \`## A2.\` to read the packages from.`);
  const out: Row[] = [];
  for (const line of text.slice(start, end).split('\n')) {
    const cells = /^\| (\d+) \| `([a-z0-9_/]+)` \| (.*?) \| .*? \| .*? \|$/.exec(line);
    if (cells === null) continue;
    out.push({ layer: Number(cells[1]), package: cells[2], holds: cells[3], faces: [cells[2]] });
  }
  if (out.length < ROWS_FLOOR) {
    throw new Error(`${GUIDE} § A1: read ${out.length} rows and the floor is ${ROWS_FLOOR}; the table's shape changed under this reader.`);
  }
  const all = faces();
  const named = new Set(out.map((row) => row.package));
  const problems: string[] = [];
  for (const row of out) {
    if (!all.includes(row.package)) problems.push(`the row \`${row.package}\` names no face of ${filesRoot()}`);
  }
  for (const face of all) {
    if (named.has(face)) continue;
    const home = out
      .filter((row) => face.startsWith(`${row.package}/`))
      .sort((a, b) => b.package.length - a.package.length)[0];
    if (home === undefined) problems.push(`the face \`${face}\` has no row, and no row's package holds it`);
    else home.faces.push(face);
  }
  if (problems.length > 0) {
    throw new Error(`${GUIDE} § A1 and Rebel's tree disagree:\n    ${problems.join('\n    ')}`);
  }
  return out;
}

/** Each face's answer, `heroes doc` run beside a catalogue that reaches all of them. */
async function ask(): Promise<Reference[]> {
  const compiler = absolute('heroes');
  if (!existsSync(compiler)) {
    throw new Error(`Rebel's reference pages are \`heroes doc\`'s output, and there is no compiler at ${compiler} (lexed.ts says how to build it).`);
  }
  recordRead('seed/heroes.c');
  recordRead('runtime');
  const folder = absolute(FOLDER);
  mkdirSync(folder, { recursive: true });
  const depth = FOLDER.split('/').length;
  writeFileSync(`${folder}/heroes.toml`, `[dependencies.rebel]\npath = "${'../'.repeat(depth)}${filesRoot()}"\n`);
  const all = faces();
  for (const face of all) recordRead(`${filesRoot()}/${face}.hero`);
  const out: Reference[] = new Array(all.length);
  let next = 0;
  const one = (face: string) =>
    new Promise<Reference>((resolve, reject) => {
      execFile(compiler, ['doc', `<rebel>/${face}`], { cwd: folder, encoding: 'utf8', maxBuffer: 64 * 1024 * 1024 }, (error, stdout, stderr) => {
        const code = error === null ? 0 : typeof (error as { code?: unknown }).code === 'number' ? (error as { code: number }).code : -1;
        const refusals = (stderr.match(/^error\[[a-z_]+\]/gm) ?? []).length;
        if (code === 0 && stdout.length > 0 && refusals === 0) return resolve({ face, text: stdout, refusals: 0 });
        if (code === 1 && refusals > 0 && stdout.length === 0) return resolve({ face, text: null, refusals });
        reject(
          new Error(
            `\`heroes doc <rebel>/${face}\` answered exit ${code}, ${refusals} refusal(s) and ${stdout.length} bytes of reference:\n` +
              `  a reference is printed at exit 0 and a refusal told at exit 1, so this is the command failing.\n` +
              stderr.split('\n').slice(0, 12).map((line) => `    ${line}`).join('\n')
          )
        );
      });
    });
  const workers = Array.from({ length: Math.min(4, availableParallelism()) }, async () => {
    while (next < all.length) {
      const at = next;
      next += 1;
      out[at] = await one(all[at]);
    }
  });
  await Promise.all(workers);
  writeFileSync(
    absolute(VERDICTS),
    JSON.stringify({ version: rebelVersion(), faces: out.map((r) => ({ face: r.face, refusals: r.refusals, accepted: r.text !== null })) })
  );
  return out;
}

let cached: Promise<Reference[]> | null = null;

/** Every face's answer, asked once per build. */
export function references(): Promise<Reference[]> {
  if (cached === null) cached = ask();
  return cached;
}

/** The faces with a page, and the owed ones with their refusals. */
export async function accepted(): Promise<Reference[]> {
  return (await references()).filter((one) => one.text !== null);
}

/** The reference's body as the page shows it, proved to be the output byte for byte. */
export function colouredReference(one: Reference): string {
  if (one.text === null) throw new Error(`\`${one.face}\` has no reference: heroes doc refused it.`);
  const body = one.text.replace(/\n$/, '');
  const html = colourMarkdown(body);
  assertVerbatim(body, html, `the reference of <rebel>/${one.face}`);
  return html;
}

/** The address of a face's page, in one edition. */
export function referenceUrl(face: string, lang: 'en' | 'it'): string {
  return `${lang === 'it' ? '/it' : ''}/rebel/${face}/`;
}

/**
 * The last audit of the build: every face has its page, or the build says
 * which are owed and how many refusals each holds, and ends red. Removes the
 * folder the answers crossed in, so nothing of it is published.
 */
export function assertReferenceWhole(): string {
  const file = absolute(VERDICTS);
  if (!existsSync(file)) {
    throw new Error(`${VERDICTS} is missing: no page asked \`heroes doc\` of Rebel's faces, so no reference was checked.`);
  }
  const read: { version: string; faces: { face: string; refusals: number; accepted: boolean }[] } = JSON.parse(readFileSync(file, 'utf8'));
  const verdicts = read.faces;
  rmSync(absolute(FOLDER), { recursive: true, force: true });
  const owed = verdicts.filter((one) => !one.accepted);
  const pages = verdicts.length - owed.length;
  if (owed.length > 0) {
    const refusals = owed.reduce((total, one) => total + one.refusals, 0);
    throw new Error(
      `Rebel's reference: ${pages} of ${verdicts.length} faces have their page, and ${owed.length} are owed, ` +
        `${refusals} refusal(s) of \`heroes doc\` among them:\n    ` +
        owed.map((one) => `<rebel>/${one.face}: ${one.refusals}`).join('\n    ') +
        `\n  Each page is \`heroes doc\`'s output, so a face it refuses has none. The debt is the face's\n` +
        `  comments (examples-rebel/README.md § A7): run \`heroes doc '<rebel>/<face>'\` beside a catalogue\n` +
        `  that reaches Rebel and write what it asks; this check goes green when every face is accepted.\n`
    );
  }
  return `reference: ${pages} of ${verdicts.length} faces of Rebel ${read.version}, each page equal to \`heroes doc\``;
}
