/**
 * Rebel's two catalogues, as the site shows them: its applications, one card
 * each, and the facts its packages' list adds to § A1's rows.
 *
 * The author's request of 2026-11-27, meant as: *I cannot find a catalogue of
 * Rebel's packages, I cannot find a catalogue of the applications built with
 * it, all very interesting things*; and the same morning, meant as: *the
 * examples must say what they are a clone of*. So `/rebel/apps/` lists every
 * application and `/rebel/packages/` every package, and nothing on either is
 * typed by hand:
 *
 * - **an application is what the walk finds**, a folder of `rebel/examples/`
 *   holding its own `main.expected` (`examples-model.ts`, as the check of
 *   Rebel's programs tells them, README § A10), never a list. The shelves of
 *   `site/src/data/examples.ts` marked `applications` give the order and say
 *   which are tools and which services, and the two must hold the same
 *   programs, or the build says which differ. A card's line is the tagline of
 *   the program's own page (`descriptions.ts`), which opens with what kind of
 *   program it is and, where its README or its code names one, the known
 *   tool it is made in the manner of; its facts are its lines of Heroes, the
 *   packages its catalogue lists, and the platforms its `main.skips` names;
 * - **a package's facts** are its ledger's counts, `<package>.ledger` beside
 *   its source (README § A8: each capability a function or a built-in,
 *   `refused` with its reason, or `pending`), whether C stands under it
 *   (§ A1's last column), and the applications whose catalogue lists it.
 *
 * The home page's section names the counts through `fillRebel`.
 */

import { groups, type Group } from '../data/examples.ts';
import { examples, exampleBySlug, type Example } from './examples-model.ts';
import { descriptionOf } from './descriptions.ts';
import { filesRoot, rows, type Row } from './reference.ts';
import { number, t, type Lang } from './i18n.ts';
import { isFile, readText } from './repo.ts';

function root(lang: Lang): string {
  return lang === 'it' ? '/it' : '';
}

function escape(text: string): string {
  return text.replace(/[&<>]/g, (ch) => (ch === '&' ? '&amp;' : ch === '<' ? '&lt;' : '&gt;'));
}

/** The address of an application's page: the examples browser's. */
function pageOf(slug: string, lang: Lang): string {
  return `${root(lang)}/examples/${slug}/`;
}

/** What an application is called: its folder's name. */
function nameOf(slug: string): string {
  return slug.slice('rebel/'.length);
}

/**
 * The shelves of applications, each with the walk's agreement: every
 * application the walk finds stands on exactly one of them, and each holds
 * applications alone. Throws naming every program that disagrees.
 */
export function applicationShelves(): Group[] {
  const shelves = groups.filter((group) => group.applications !== undefined);
  const walked = examples()
    .filter((one) => one.group === 'rebel' && one.twin === null)
    .map((one) => one.slug);
  const shelved = shelves.flatMap((group) => group.slugs);
  const problems: string[] = [];
  for (const slug of walked) {
    if (!shelved.includes(slug)) problems.push(`${slug} holds its own main.expected and stands on no shelf marked \`applications\``);
  }
  for (const slug of shelved) {
    if (!walked.includes(slug)) problems.push(`${slug} stands on a shelf marked \`applications\` and is no application: a twin, or no program of rebel/examples/`);
  }
  if (shelves.length === 0 || problems.length > 0) {
    throw new Error(
      `Rebel's applications and the shelves of site/src/data/examples.ts disagree:\n    ` +
        (problems.length > 0 ? problems.join('\n    ') : 'no shelf is marked `applications`') +
        `\n  An application is a folder of rebel/examples/ holding its own main.expected (README § A10).\n`
    );
  }
  return shelves;
}

/** Every application, in the shelves' order. */
export function applicationSlugs(): string[] {
  return applicationShelves().flatMap((group) => group.slugs);
}

/** The packages an application's catalogue lists, its `packages` line. */
export function cataloguePackages(example: Example): string[] {
  const path = `${example.dir}/heroes.toml`;
  if (!isFile(path)) throw new Error(`${path} is missing: an application written with Rebel takes it through its catalogue.`);
  const line = /^packages\s*=\s*\[([\s\S]*?)\]/m.exec(readText(path));
  if (line === null) throw new Error(`${path} has no \`packages\` line, so the site cannot say which of Rebel's packages it takes.`);
  const out = [...line[1].matchAll(/"([^"]+)"/g)].map((m) => m[1]);
  if (out.length === 0) throw new Error(`${path} lists no package on its \`packages\` line.`);
  return out;
}

const PLATFORMS: Record<string, string> = { windows: 'Windows', linux: 'Linux', macos: 'macOS' };

/** The platforms an application's `main.skips` names, where its demo is not held to its transcript. */
function skippedOn(example: Example): string[] {
  const path = `${example.dir}/main.skips`;
  if (!isFile(path)) return [];
  const out: string[] = [];
  for (const line of readText(path).split('\n')) {
    if (line.startsWith('#') || line.trim() === '') continue;
    const named = /^([a-z0-9]+):\s/.exec(line);
    if (named === null || PLATFORMS[named[1]] === undefined) {
      throw new Error(`${path}: \`${line}\` names no platform this site knows (${Object.keys(PLATFORMS).join(', ')}).`);
    }
    if (!out.includes(PLATFORMS[named[1]])) out.push(PLATFORMS[named[1]]);
  }
  return out;
}

/** One application's card. */
function applicationCard(slug: string, kind: 'tool' | 'service', lang: Lang): string {
  const example = exampleBySlug(slug);
  const description = descriptionOf(slug, lang);
  const packages = cataloguePackages(example).length;
  const facts: string[] = [
    lang === 'it' ? (kind === 'tool' ? 'strumento' : 'servizio') : kind,
    `${number(example.lines, lang)} ${t('lines', lang)}`,
    lang === 'it'
      ? `${number(packages, lang)} ${packages === 1 ? 'pacchetto' : 'pacchetti'} di Rebel`
      : `${number(packages, lang)} ${packages === 1 ? 'package' : 'packages'} of Rebel`,
  ];
  for (const platform of skippedOn(example)) {
    facts.push(lang === 'it' ? `demo non verificata su ${platform}` : `demo not checked on ${platform}`);
  }
  return (
    `    <li id="${escape(nameOf(slug))}">\n` +
    `      <h3><code>${escape(nameOf(slug))}</code></h3>\n` +
    `      <p class="meta">${facts.join(' &middot; ')}</p>\n` +
    `      <p>${description.tagline}</p>\n` +
    `      <a class="more" href="${pageOf(slug, lang)}">${t('reads', lang)}</a>\n` +
    `    </li>`
  );
}

/** The catalogue's body: a heading per shelf, a card per application. */
export function applicationsList(lang: Lang): string {
  return applicationShelves()
    .map((group) => {
      const cards = group.slugs.map((slug) => applicationCard(slug, group.applications!, lang)).join('\n');
      return `  <h2 id="${group.key}">${lang === 'it' ? group.it : group.en}</h2>\n  <ul class="cards">\n${cards}\n  </ul>`;
    })
    .join('\n\n');
}

/** What a package's ledger answers, counted. */
export interface Ledger {
  offered: number;
  refused: number;
  pending: number;
}

/**
 * One package's ledger, `<package>.ledger` beside its source, counted by the
 * `heroes` line of each `capability` block (README § A8). A block with no such
 * line, or two, is a shape this reader does not know, and the build's error.
 */
export function ledgerOf(row: Row): Ledger {
  const path = `${filesRoot()}/${row.package}.ledger`;
  if (!isFile(path)) throw new Error(`${path} is missing: every package of README § A1 keeps a ledger (§ A8).`);
  const out: Ledger = { offered: 0, refused: 0, pending: 0 };
  const blocks = readText(path).split(/^capability /m).slice(1);
  for (const block of blocks) {
    const answers = [...block.matchAll(/^ {2}heroes +(.*)$/gm)].map((m) => m[1]);
    if (answers.length !== 1) {
      throw new Error(`${path}: the capability \`${block.split('\n')[0]}\` holds ${answers.length} \`heroes\` lines, and § A8 gives each one.`);
    }
    if (/^refused\b/.test(answers[0])) out.refused += 1;
    else if (/^pending\b/.test(answers[0])) out.pending += 1;
    else out.offered += 1;
  }
  if (blocks.length === 0) throw new Error(`${path} holds no capability block.`);
  return out;
}

/** Every package's ledger, summed: the figures the packages' page leads with. */
export function ledgerTotals(): Ledger & { packages: number } {
  const out = { offered: 0, refused: 0, pending: 0, packages: 0 };
  for (const row of rows()) {
    const one = ledgerOf(row);
    out.offered += one.offered;
    out.refused += one.refused;
    out.pending += one.pending;
    out.packages += 1;
  }
  return out;
}

/**
 * Which applications list each package on their catalogue's `packages` line,
 * a face listed under a package's row (`tokenizer/tables/cl100k_base` under
 * `tokenizer/tables`, `reference.ts`) counting for that row. A line naming no
 * face of any row is a catalogue this site cannot read, and the build's error.
 */
export function applicationsByPackage(): Map<string, string[]> {
  const table = rows();
  const out = new Map<string, string[]>(table.map((row) => [row.package, []]));
  const problems: string[] = [];
  for (const slug of applicationSlugs()) {
    for (const name of cataloguePackages(exampleBySlug(slug))) {
      const row = table.find((one) => one.faces.includes(name));
      if (row === undefined) {
        problems.push(`${slug}'s catalogue lists \`${name}\`, which is no face of a package of rebel/examples/README.md § A1`);
        continue;
      }
      const users = out.get(row.package)!;
      if (!users.includes(slug)) users.push(slug);
    }
  }
  if (problems.length > 0) throw new Error(`the applications' catalogues and § A1 disagree:\n    ${problems.join('\n    ')}`);
  return out;
}

/** The facts line under a package's row: its ledger, C under it, the applications that take it. */
export function packageFacts(row: Row, users: string[], lang: Lang): string {
  const ledger = ledgerOf(row);
  const parts: string[] = [
    lang === 'it'
      ? `registro: ${number(ledger.offered, lang)} offerte, ${number(ledger.refused, lang)} rifiutate, ${number(ledger.pending, lang)} da fare`
      : `ledger: ${number(ledger.offered, lang)} offered, ${number(ledger.refused, lang)} refused, ${number(ledger.pending, lang)} pending`,
  ];
  if (row.c.trim() !== '') parts.push(lang === 'it' ? 'sopra il C' : 'over C');
  if (users.length > 0) {
    const links = users.map((slug) => `<a href="${pageOf(slug, lang)}">${escape(nameOf(slug))}</a>`).join(', ');
    parts.push(`${lang === 'it' ? 'lo usano' : 'used by'} ${links}`);
  }
  return parts.join(' &middot; ');
}

/**
 * The placeholders a fragment may carry: `{{rebelPackages}}` and
 * `{{rebelApplications}}`, the counts, read only where a page asks for one.
 */
export function fillRebel(html: string, pagePath: string): string {
  if (!html.includes('{{rebelPackages}}') && !html.includes('{{rebelApplications}}')) return html;
  const lang: Lang = pagePath.startsWith('site/src/html/it/') ? 'it' : 'en';
  let out = html;
  if (out.includes('{{rebelPackages}}')) out = out.replaceAll('{{rebelPackages}}', number(rows().length, lang));
  if (out.includes('{{rebelApplications}}')) out = out.replaceAll('{{rebelApplications}}', number(applicationSlugs().length, lang));
  return out;
}
