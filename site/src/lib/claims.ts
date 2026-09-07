/**
 * The claims the prose makes, checked against the tree at build time.
 *
 * Everything else on this site is already checked or generated: the code
 * blocks are re-cut from their files, the version is read from the compiler,
 * the example counts are computed. What was left unchecked was the prose, and
 * the language veteran's seat put the finding in one sentence after four
 * sittings: **every false claim it had found was in hand-written prose making a
 * claim about a magnitude or a named thing.** Four words that can start a line
 * where the parser dispatches on seven. Three judges who can refuse where four
 * carry a veto. Twelve chapters on a page beside thirteen. A `0` beside `180+`
 * that no instrument produced.
 *
 * Each of those is a number the tree knows. This module reads the number from
 * the tree and asserts the page spells it, in the edition's own number word, so
 * the day the fact moves the build goes red and names the page, instead of the
 * sentence going on reading as correct.
 *
 * What this does NOT do: it cannot verify a sentence it does not know about.
 * A claim enters this table when it is written, and a claim that is not here
 * is a claim only a reader checks. That is the honest scope.
 */

import { readText, filesIn } from './repo.ts';

const AGENTS_DIR = '.claude/agents';
const DECL_FILE = 'selfhost/parse/decl.hero';
const TABLE_FILE = 'selfhost/cli/table.hero';
const CHAPTERS_DIR = 'site/src/html/docs';

/* -- The facts, each read from the one place the tree keeps it ------------- */

/** The seats of the language panel, and how many of them carry a veto. */
function judges(): { seats: number; vetoes: number } {
  // `filesIn` returns repository-relative paths, not basenames, so a file is
  // read by the path it came back as and the index is excluded by its tail.
  const briefs = filesIn(AGENTS_DIR).filter((f) => f.endsWith('.md'));
  // Case-insensitive, because the historian's brief opens "Advisory panel judge"
  // and the other four "Panel judge": the fifth seat is a seat without a veto,
  // and a capital P was silently making it four seats and four vetoes.
  const panel = briefs.filter((f) => /panel judge/i.test(readText(f)));
  const vetoes = panel.filter((f) => /Has veto power/.test(readText(f)));
  if (panel.length < 3) {
    throw new Error(`${AGENTS_DIR}: read only ${panel.length} panel briefs, and the panel has five seats.`);
  }
  return { seats: panel.length, vetoes: vetoes.length };
}

/**
 * The words that can begin a top-level line: the keywords the parser's
 * top-level loop dispatches on, read from that loop. `kw_use` is among them,
 * `kw_return` is not, and the page must agree with the loop rather than with
 * whoever last counted.
 */
function topLevelWords(): number {
  const text = readText(DECL_FILE);
  const start = text.indexOf('function unexpected_top_level');
  const loop = text.slice(0, start < 0 ? undefined : start);
  const arms = new Set(loop.match(/else if k == \.kw_([a-z]+)/g) ?? []);
  if (arms.size < 5) {
    throw new Error(`${DECL_FILE}: read ${arms.size} top-level dispatch arms, and the language has seven.`);
  }
  return arms.size;
}

/** The verbs of the one command: the `Command(` rows of the argv table. */
function verbs(): number {
  const rows = readText(TABLE_FILE).match(/^\s*Command\(tag: \./gm) ?? [];
  if (rows.length < 8) {
    throw new Error(`${TABLE_FILE}: read ${rows.length} Command rows, and the table has eleven.`);
  }
  return rows.length;
}

/** The chapters of the documentation: every fragment under docs/ but the index. */
function chapters(): number {
  return filesIn(CHAPTERS_DIR).filter((f) => f.endsWith('.html') && !f.endsWith('/index.html')).length;
}

/* -- The number words, so a page is checked in its own language ----------- */

const WORDS: Record<'en' | 'it', string[]> = {
  en: ['zero', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten',
       'eleven', 'twelve', 'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen',
       'nineteen', 'twenty'],
  it: ['zero', 'uno', 'due', 'tre', 'quattro', 'cinque', 'sei', 'sette', 'otto', 'nove', 'dieci',
       'undici', 'dodici', 'tredici', 'quattordici', 'quindici', 'sedici', 'diciassette', 'diciotto',
       'diciannove', 'venti'],
};

function word(n: number, lang: 'en' | 'it'): string {
  const table = WORDS[lang];
  if (n < 0 || n >= table.length) {
    throw new Error(`no number word for ${n}: the claims table spells numbers up to twenty.`);
  }
  return table[n];
}

/* -- The claims: which page says what, and how the sentence spells it ------ */

interface Claim {
  /** The fragment that carries the sentence. */
  page: string;
  /** What the sentence is about, for the error message. */
  what: string;
  /** The number the tree gives. */
  fact: () => number;
  /** The sentence shape, with the number word where the page spells it. */
  shape: (n: string) => RegExp;
}

const en = (page: string) => (page.startsWith('site/src/html/it/') ? 'it' : 'en');

/**
 * The table. A claim's `shape` is matched case-insensitively, against the raw
 * fragment, and the number word is the edition's own. A page that stops
 * carrying a listed sentence altogether also fails, because a claim that
 * quietly leaves the page is the same drift in the other direction.
 */
const CLAIMS: Claim[] = [
  // The panel: five judges, four of whom can refuse.
  { page: 'site/src/html/index.html', what: 'the number of judges',
    fact: () => judges().seats, shape: (n) => new RegExp(`${n} judges`, 'i') },
  { page: 'site/src/html/index.html', what: 'the judges who can refuse',
    fact: () => judges().vetoes, shape: (n) => new RegExp(`${n}\\s+of whom can refuse`, 'i') },
  { page: 'site/src/html/it/index.html', what: 'the number of judges',
    fact: () => judges().seats, shape: (n) => new RegExp(`${n} giudici`, 'i') },
  { page: 'site/src/html/it/index.html', what: 'the judges who can refuse',
    fact: () => judges().vetoes, shape: (n) => new RegExp(`${n} dei quali possono rifiutarlo`, 'i') },

  // The words that can begin a top-level line.
  { page: 'site/src/html/why.html', what: 'the words that can begin a line',
    fact: topLevelWords, shape: (n) => new RegExp(`${n} words can begin a line`, 'i') },
  { page: 'site/src/html/it/why.html', what: 'the words that can begin a line',
    fact: topLevelWords, shape: (n) => new RegExp(`${n} parole possono aprire una riga`, 'i') },
  { page: 'site/src/html/docs/the-c-boundary.html', what: 'the words that can begin a line',
    fact: topLevelWords, shape: (n) => new RegExp(`${n} words that can start a\\s+top-level line`, 'i') },
  { page: 'site/src/html/it/docs/the-c-boundary.html', what: 'the words that can begin a line',
    fact: topLevelWords, shape: (n) => new RegExp(`${n} parole che possono aprire una\\s+riga`, 'i') },

  // The verbs of the one command.
  { page: 'site/src/html/start.html', what: 'the verbs of the command',
    fact: verbs, shape: (n) => new RegExp(`One command, ${n} verbs`, 'i') },
  { page: 'site/src/html/it/start.html', what: 'the verbs of the command',
    fact: verbs, shape: (n) => new RegExp(`Un comando, ${n} verbi`, 'i') },

  // The chapters of the documentation.
  { page: 'site/src/html/docs/index.html', what: 'the number of chapters',
    fact: chapters, shape: (n) => new RegExp(`These ${n} chapters`, 'i') },
  { page: 'site/src/html/it/docs/index.html', what: 'the number of chapters',
    fact: chapters, shape: (n) => new RegExp(`Questi ${n}\\s+capitoli`, 'i') },
];

/**
 * Check every claim a page carries. Throws with the page, the claim, what the
 * tree says and how the page would have to spell it, because "a claim drifted"
 * sends somebody reading twelve pages.
 */
export function checkClaims(html: string, pagePath: string): void {
  const problems: string[] = [];
  for (const claim of CLAIMS) {
    if (claim.page !== pagePath) continue;
    const n = claim.fact();
    const spelled = word(n, en(pagePath));
    if (!claim.shape(spelled).test(html)) {
      problems.push(
        `${pagePath}: ${claim.what} is ${n} in the tree, and the page does not say "${spelled}" where it should.\n` +
          `      expected a sentence matching: ${claim.shape(spelled)}`
      );
    }
  }
  if (problems.length > 0) {
    throw new Error(
      `${problems.length} claim(s) on the page disagree with the repository, or left the page.\n    ` +
        problems.join('\n    ') +
        `\n  A number in prose is a claim about the tree (site/src/lib/claims.ts). Change the sentence to\n` +
        `  what the tree says, or if the sentence was deliberately removed, remove its row from the table.\n`
    );
  }
}

/** What the table holds, for the build's own report and for a test. */
export function claimsSummary(): string {
  const j = judges();
  return `${j.seats} judges, ${j.vetoes} vetoes, ${topLevelWords()} top-level words, ${verbs()} verbs, ${chapters()} chapters; ${CLAIMS.length} claims checked`;
}
