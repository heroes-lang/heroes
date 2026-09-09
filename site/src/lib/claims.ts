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
 * Three things the same seat found about the first version of this file, each
 * now closed and each worth knowing about the shape of an instrument:
 *
 * - A row whose page never rendered was skipped in silence, so renaming a page
 *   killed its claims without a sound. Every row is now ticked off as its page
 *   passes through, and `assertEveryClaimVisited()` runs when the build ends.
 * - A count is a weak fact. `topLevelWords()` counted `else if` arms and would
 *   have read six if the first arm were spelled `if`. The parser carries a
 *   SECOND enumeration of the same words, in the `expected_declaration`
 *   message, so the two are read and asserted equal: a mismatch is the
 *   compiler disagreeing with itself, which is a better red than a number.
 * - A count survives a rename. Eleven verbs stayed eleven if `mutate` became
 *   something else, and the page listing them went stale. The names themselves
 *   are asserted on the page now, each as `<code>name</code>`.
 *
 * What this does NOT do: it cannot verify a sentence it does not know about.
 * A claim enters this table when it is written, and a claim that is not here
 * is a claim only a reader checks. That is the honest scope.
 */

import { appendFileSync, readFileSync, existsSync, unlinkSync, mkdirSync } from 'node:fs';
import { dirname } from 'node:path';
import { readText, filesIn, absolute } from './repo.ts';
import { number } from './i18n.ts';

const AGENTS_DIR = '.claude/agents';
const DECL_FILE = 'selfhost/parse/decl.hero';
const TABLE_FILE = 'selfhost/cli/table.hero';
const DOCTOR_FILE = 'selfhost/cli/doctor.hero';
const CHAPTERS_DIR = 'site/src/html/docs';
const SPEC_SUITE = 'tests/harness/suite_spec.hero';

/* -- The facts, each read from the one place the tree keeps it ------------- */

/**
 * The specification's token ceiling, read from the suite that holds the
 * specification under it on every commit, never typed here. The claim under
 * the name on both home pages carries it and the specification page shows it,
 * so a panel that raised it would otherwise leave three pages quoting the old
 * number. It was a literal in `spec-page.ts` until the home started saying it.
 */
export function specCeiling(): number {
  const text = readText(SPEC_SUITE);
  const found = /^constant CEILING: i64\n\s+(\d+)$/m.exec(text);
  if (found === null) {
    throw new Error(`${SPEC_SUITE}: no \`constant CEILING: i64\` with its number on the line under it.`);
  }
  const n = Number(found[1]);
  if (n < 1024) throw new Error(`${SPEC_SUITE}: the ceiling reads ${n}, below any budget the language has had.`);
  return n;
}

/**
 * The ceiling as the pages say it: `6K`, the author's own word for it (2026-09-09,
 * *"fewer than 6K tokens"*). Derived, never typed, and refused if the ceiling
 * stops being a whole number of K: a claim that rounds would be a claim the
 * tree does not make.
 */
export function ceilingK(): string {
  const n = specCeiling();
  if (n % 1024 !== 0) throw new Error(`${SPEC_SUITE}: the ceiling ${n} is not a whole number of K, and the pages say it in K.`);
  return `${n / 1024}K`;
}

/**
 * The specification's REAL count and the model that produced it, read from the
 * same suite, which pins them under the ceiling on every commit and refuses a
 * text that moved without them (the suite's `real` row and `SPEC_DIGEST`). The specification
 * page says the number beside the ceiling because a reader about to paste the
 * document into a prompt asks "how much fewer?", and a threshold alone left the
 * question open (marketing seat, 2026-09-09). Generated, so it cannot go stale;
 * the date the count was taken stays off the page (`site/CLAUDE.md`).
 */
export function specReal(): { tokens: number; model: string } {
  const text = readText(SPEC_SUITE);
  const tokens = /^constant REAL_TOKENS: i64\n\s+(\d+)$/m.exec(text);
  const model = /^constant REAL_MODEL: str\n\s+"([a-z0-9.-]+)"$/m.exec(text);
  if (tokens === null || model === null) {
    throw new Error(`${SPEC_SUITE}: no \`constant REAL_TOKENS: i64\` and \`constant REAL_MODEL: str\` with their values on the line under them.`);
  }
  const n = Number(tokens[1]);
  if (n >= specCeiling()) throw new Error(`${SPEC_SUITE}: the real count ${n} is not below the ceiling ${specCeiling()}, and the pages say it is.`);
  return { tokens: n, model: model[1] };
}

/** The seats of the language panel, and how many of them carry a veto. */
function judges(): { seats: number; vetoes: number } {
  // `filesIn` returns repository-relative paths, so each brief is read by the
  // path it came back as. Case-insensitive, because the historian's brief
  // opens "Advisory panel judge" and the other four "Panel judge": the fifth
  // seat is a seat without a veto, and a capital P silently made it four seats
  // with four vetoes on the first run of this file.
  const briefs = filesIn(AGENTS_DIR).filter((f) => f.endsWith('.md'));
  const panel = briefs.filter((f) => /panel judge/i.test(readText(f)));
  const vetoes = panel.filter((f) => /Has veto power/.test(readText(f)));
  if (panel.length < 3) {
    throw new Error(`${AGENTS_DIR}: read only ${panel.length} panel briefs, and the panel has five seats.`);
  }
  return { seats: panel.length, vetoes: vetoes.length };
}

/**
 * The words that can begin a top-level line, read TWICE from the parser and
 * asserted equal: once from the keywords its top-level loop dispatches on,
 * once from the `expected_declaration` message that lists them for a reader
 * who typed something else. Both enumerations live in `selfhost/parse/decl.hero`.
 */
function topLevelWords(): Set<string> {
  const text = readText(DECL_FILE);
  const start = text.indexOf('function unexpected_top_level');
  if (start < 0) throw new Error(`${DECL_FILE}: no \`function unexpected_top_level\` to read the table from.`);

  const loop = text.slice(0, start);
  const dispatched = new Set([...loop.matchAll(/if k == \.kw_([a-z]+)/g)].map((m) => m[1]));

  // The message reads: ... `use`, `constant`, `function`, `record`, `variant`,
  // `test \"…\"`, or `extern`. Each word is the first token inside its backticks.
  // The message is a concatenation of string pieces around `cursor.found(...)`,
  // so the whole `message:` argument is read, from that label to the `span:`
  // label that follows it. The first version read from the function to the end
  // of the file and picked up `header` and `return` from unrelated messages
  // further down, which is exactly the kind of premise this file exists to refuse.
  const after = text.slice(start);
  const label = after.indexOf('code: "expected_declaration"');
  const from = label < 0 ? -1 : after.indexOf('message:', label);
  const to = from < 0 ? -1 : after.indexOf('span:', from);
  if (from < 0 || to < 0) throw new Error(`${DECL_FILE}: no expected_declaration message to read the words from.`);
  const listed = new Set([...after.slice(from, to).matchAll(/`([a-z]+)[^`]*`/g)].map((m) => m[1]));

  const same = dispatched.size === listed.size && [...dispatched].every((w) => listed.has(w));
  if (!same) {
    throw new Error(
      `${DECL_FILE}: the top-level loop dispatches on {${[...dispatched].sort().join(' ')}} and the\n` +
        `  expected_declaration message lists {${[...listed].sort().join(' ')}}. The compiler disagrees with\n` +
        `  itself about which words can begin a line, and the site cannot say a number until it does not.`
    );
  }
  if (dispatched.size < 5) {
    throw new Error(`${DECL_FILE}: read ${dispatched.size} top-level words, and the language has seven.`);
  }
  return dispatched;
}

/** The verbs of the one command: the names in the `Command(` rows of the argv table. */
function verbs(): string[] {
  const names = [...readText(TABLE_FILE).matchAll(/^\s*Command\(tag: \.[a-z_]+, name: "([a-z]+)"/gm)].map((m) => m[1]);
  if (names.length < 8) {
    throw new Error(`${TABLE_FILE}: read ${names.length} Command rows, and the table has eleven.`);
  }
  return names;
}

/**
 * The lines of the Zen: the numbered lines of the string `heroes this` prints,
 * read from the function that returns it. `site/CLAUDE.md` lists "the twenty
 * lines of the Zen" among the numbers that stay exact, so a twenty-first law
 * goes red here before it goes stale on the page.
 */
function zenLines(): number {
  const text = readText(DOCTOR_FILE);
  const start = text.indexOf('function zen()');
  if (start < 0) throw new Error(`${DOCTOR_FILE}: no \`function zen()\` to count the Zen from.`);
  const body = text.slice(start, text.indexOf('\n\n', start));
  const numbered = body.match(/\\n\s?\d+\. /g) ?? [];
  if (numbered.length < 10) {
    throw new Error(`${DOCTOR_FILE}: counted ${numbered.length} numbered Zen lines, and there are twenty.`);
  }
  return numbered.length;
}

/**
 * What the continuous integration actually does, read from the step and not from
 * the trigger. A sentence on `/project/` said the fixpoint ran on three
 * platforms at a tag; the matrix does widen at a tag, and the step that runs
 * `cmp seed/heroes.c` carries `if: runner.os == 'Linux'` whatever started the
 * run. The repair before this one read lines 40 to 52 and 186 to 200 of the
 * workflow and never line 650, which is the fifth time a sentence about the
 * record was written from part of the record. So the step is read here.
 */
const CI_FILE = '.github/workflows/ci.yml';

function ci(): { fixpointLinuxOnly: boolean; tagPlatforms: number } {
  const text = readText(CI_FILE);
  const lines = text.split('\n');
  const at = lines.findIndex((l) => l.includes('cmp seed/heroes.c'));
  if (at < 0) throw new Error(`${CI_FILE}: no step runs \`cmp seed/heroes.c\`, and the fixpoint proof on /project/ says one does.`);
  // Walk up from the `cmp` line to the step's `- name:` and collect its `if:`.
  let head = at;
  while (head > 0 && !/^\s*- name:/.test(lines[head])) head -= 1;
  const step = lines.slice(head, at + 1).join('\n');
  const gate = /^\s*if:\s*(.+)$/m.exec(step);
  // Equality, not containment. `/Linux/.test(...)` read "Linux || macOS" as one
  // leg and "Linux && tag" as every run; the seat measured five spellings. A
  // gate spelled any other way is a shape this instrument cannot read, and it
  // says so loudly rather than guessing, the way `topLevelWords()` does.
  let fixpointLinuxOnly = false;
  if (gate !== null) {
    const expr = gate[1].trim();
    if (expr === "runner.os == 'Linux'") fixpointLinuxOnly = true;
    else throw new Error(`${CI_FILE}: the fixpoint step's gate is spelled \`${expr}\`, which this instrument cannot read; read the step and say so on the page.`);
  }
  // The FIRST command's claim, "on every milestone tag on three platforms", rests
  // on the matrix alone unless the step that builds from the seed is read too:
  // an `if:` added there would falsify the page with the build green.
  const seedAt = lines.findIndex((l) => l.includes('seed/heroes.c runtime/runtime.c'));
  if (seedAt < 0) throw new Error(`${CI_FILE}: no step compiles \`seed/heroes.c runtime/runtime.c\`, and /project/ says one runs on every leg.`);
  let seedHead = seedAt;
  while (seedHead > 0 && !/^\s*- name:/.test(lines[seedHead])) seedHead -= 1;
  const seedStep = lines.slice(seedHead, seedAt + 1).join('\n');
  if (/^\s*if:/m.test(seedStep)) {
    throw new Error(`${CI_FILE}: the step that builds from the seed now carries an \`if:\`, and /project/ says it runs on every leg of every run.`);
  }
  // And the fallback arm, the one a plain push takes, must be one platform and Linux.
  const fallback = /\|\|\s*'(\[[^\n]*\])'\s*\n\s*\)\s*\}\}/.exec(text);
  if (fallback === null) throw new Error(`${CI_FILE}: no fallback matrix literal to read the push platform from.`);
  const fallbackNames = (fallback[1].match(/"name":/g) ?? []).length;
  if (fallbackNames !== 1 || !/"os":"ubuntu-latest"/.test(fallback[1])) {
    throw new Error(`${CI_FILE}: the push matrix names ${fallbackNames} platform(s) and /project/ says a push runs on Linux alone.`);
  }
  // The tag branch of the matrix is the one literal that names every platform.
  const tagArm = /github\.ref_type == 'tag'[^\n]*\n\s*&&\s*'(\[[^\n]*\])'/.exec(text);
  if (tagArm === null) throw new Error(`${CI_FILE}: no tag-branch matrix literal to count the platforms from.`);
  const tagPlatforms = (tagArm[1].match(/"name":/g) ?? []).length;
  if (tagPlatforms < 2) throw new Error(`${CI_FILE}: the tag matrix names ${tagPlatforms} platform(s), and the site says three.`);
  return { fixpointLinuxOnly, tagPlatforms };
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
  if (n < 0) throw new Error(`no number word for ${n}`);
  // Up to twenty a page writes the word; above it the digits, grouped the way
  // the edition groups them (`4,096` and `4.096`), which is the convention
  // site/README.md sets for the Italian edition's numbers. Through the site's
  // own `number` and not `toLocaleString`: the Italian locale in ICU puts no
  // separator under five digits, so it spelled 4096 as "4096" and the check
  // refused the home page that correctly said 4.096.
  if (n >= table.length) return number(n, lang);
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

const langOf = (page: string) => (page.startsWith('site/src/html/it/') ? 'it' : 'en');

/**
 * The table. A claim's `shape` is matched case-insensitively against the raw
 * fragment, and the number word is the edition's own. A page that stops
 * carrying a listed sentence altogether also fails, because a claim that
 * quietly leaves the page is the same drift in the other direction.
 */
const CLAIMS: Claim[] = [
  // The panel: five judges, four of whom can refuse. Said on the home, on the
  // project page where the five are listed, and on the thanks page.
  { page: 'site/src/html/index.html', what: 'the number of judges',
    fact: () => judges().seats, shape: (n) => new RegExp(`${n} judges`, 'i') },
  { page: 'site/src/html/index.html', what: 'the judges who can refuse',
    fact: () => judges().vetoes, shape: (n) => new RegExp(`${n} can veto`, 'i') },
  { page: 'site/src/html/it/index.html', what: 'the number of judges',
    fact: () => judges().seats, shape: (n) => new RegExp(`${n} giudici`, 'i') },
  { page: 'site/src/html/it/index.html', what: 'the judges who can refuse',
    fact: () => judges().vetoes, shape: (n) => new RegExp(`${n} dei quali possono porre un veto`, 'i') },
  { page: 'site/src/html/project.html', what: 'the seats of the panel',
    fact: () => judges().seats, shape: (n) => new RegExp(`panel of ${n} seats`, 'i') },
  { page: 'site/src/html/project.html', what: 'the seats that can refuse',
    fact: () => judges().vetoes, shape: (n) => new RegExp(`${n} can veto`, 'i') },
  { page: 'site/src/html/it/project.html', what: 'the seats of the panel',
    fact: () => judges().seats, shape: (n) => new RegExp(`collegio di ${n} seggi`, 'i') },
  { page: 'site/src/html/it/project.html', what: 'the seats that can refuse',
    fact: () => judges().vetoes, shape: (n) => new RegExp(`${n} possono porre un veto`, 'i') },
  { page: 'site/src/html/about/thanks.html', what: 'the number of judges',
    fact: () => judges().seats, shape: (n) => new RegExp(`${n} judges review proposals`, 'i') },
  { page: 'site/src/html/it/about/thanks.html', what: 'the number of judges',
    fact: () => judges().seats, shape: (n) => new RegExp(`${n} giudici valutano le proposte`, 'i') },

  // The specification's ceiling, in the author's own word for it, `6K`
  // (2026-09-09), derived from the suite by `ceilingK` and said on the two
  // home pages, the two why pages and the two specification pages. The shape
  // ignores the formatted number it is handed and reads the K form itself.
  { page: 'site/src/html/index.html', what: 'the specification ceiling in tokens',
    fact: specCeiling, shape: () => new RegExp(`fewer than ${ceilingK()} tokens`) },
  { page: 'site/src/html/it/index.html', what: 'the specification ceiling in tokens',
    fact: specCeiling, shape: () => new RegExp(`meno di ${ceilingK()} token`) },
  { page: 'site/src/html/why.html', what: 'the specification ceiling in tokens',
    fact: specCeiling, shape: () => new RegExp(`below ${ceilingK()} tokens`) },
  { page: 'site/src/html/it/why.html', what: 'the specification ceiling in tokens',
    fact: specCeiling, shape: () => new RegExp(`sotto il limite di ${ceilingK()} token`) },
  { page: 'site/src/html/spec.html', what: 'the specification ceiling in tokens',
    fact: specCeiling, shape: () => new RegExp(`below ${ceilingK()} tokens`) },
  { page: 'site/src/html/it/spec.html', what: 'the specification ceiling in tokens',
    fact: specCeiling, shape: () => new RegExp(`sotto il limite di ${ceilingK()} token`) },

  // The words that can begin a top-level line.
  { page: 'site/src/html/why.html', what: 'the words that can begin a line',
    fact: () => topLevelWords().size, shape: (n) => new RegExp(`${n} words can begin a line`, 'i') },
  { page: 'site/src/html/it/why.html', what: 'the words that can begin a line',
    fact: () => topLevelWords().size, shape: (n) => new RegExp(`${n} parole possono aprire una riga`, 'i') },
  { page: 'site/src/html/docs/the-c-boundary.html', what: 'the words that can begin a line',
    fact: () => topLevelWords().size, shape: (n) => new RegExp(`${n} words that can start a\\s+top-level line`, 'i') },
  { page: 'site/src/html/it/docs/the-c-boundary.html', what: 'the words that can begin a line',
    fact: () => topLevelWords().size, shape: (n) => new RegExp(`${n} parole che possono aprire una\\s+riga`, 'i') },

  // The verbs of the one command: the count in the heading.
  { page: 'site/src/html/start.html', what: 'the verbs of the command',
    fact: () => verbs().length, shape: (n) => new RegExp(`One command, ${n} verbs`, 'i') },
  { page: 'site/src/html/it/start.html', what: 'the verbs of the command',
    fact: () => verbs().length, shape: (n) => new RegExp(`Un comando, ${n} verbi`, 'i') },

  // The lines of the Zen.
  { page: 'site/src/html/project.html', what: 'the lines of the Zen',
    fact: zenLines, shape: (n) => new RegExp(`<h2 id="zen">${n} lines</h2>`, 'i') },
  { page: 'site/src/html/it/project.html', what: 'the lines of the Zen',
    fact: zenLines, shape: (n) => new RegExp(`<h2 id="zen">${n} righe</h2>`, 'i') },

  // What the continuous integration does, from the workflow's own steps.
  { page: 'site/src/html/project.html', what: 'the platforms a milestone tag runs on',
    fact: () => ci().tagPlatforms, shape: (n) => new RegExp(`on ${n} platforms`, 'i') },
  { page: 'site/src/html/it/project.html', what: 'the platforms a milestone tag runs on',
    fact: () => ci().tagPlatforms, shape: (n) => new RegExp(`su ${n} piattaforme`, 'i') },

  // The chapters of the documentation.
  { page: 'site/src/html/docs/index.html', what: 'the number of chapters',
    fact: chapters, shape: (n) => new RegExp(`These ${n} chapters`, 'i') },
  { page: 'site/src/html/it/docs/index.html', what: 'the number of chapters',
    fact: chapters, shape: (n) => new RegExp(`Questi ${n}\\s+capitoli`, 'i') },
];

/**
 * The pages that must name every verb, each as `<code>verb</code>`. A count
 * survives a rename; the names do not.
 */
const NAME_EVERY_VERB = ['site/src/html/start.html', 'site/src/html/it/start.html'];

/**
 * A chapter's footer says how many diagnostics the page showed: "The two
 * diagnostics are that file with one label removed ..." The number is a claim
 * about the page itself, and it drifts the same way the others do, when a
 * fourth transcript is added above a footer that still says three. Both are on
 * the same page, so the check needs no tree fact: count the `$ heroes check` and
 * `$ heroes build` transcripts and compare. Measured over all 26 chapters before
 * this was written: every count matched, which is the moment to pin it.
 */
function checkChapterDiagnostics(html: string, pagePath: string): string[] {
  if (!/\/docs\/[^/]+\.html$/.test(pagePath) || pagePath.endsWith('/index.html')) return [];
  const flat = html.replace(/\s+/g, ' ');
  const said = /The (\w+) diagnostics? (?:are|is) |Le (\w+) diagnostiche sono |(La) diagnostica /.exec(flat);
  if (said === null) return [];
  const w = (said[1] ?? said[2] ?? said[3]).toLowerCase();
  const lang = langOf(pagePath);
  const n = w === 'la' ? 1 : WORDS[lang].indexOf(w);
  if (n < 0) return [`${pagePath}: the footer counts diagnostics with a word this table cannot read: "${w}".`];
  // A transcript is a `<pre>` whatever class it carries: `pre.diag` marks the
  // ones whose message may wrap, and this count saw one of two the day that
  // class reached the chapters (2026-09-09).
  const shown = (flat.match(/<pre(?: class="[^"]*")?><code>\$ heroes (?:check|build) /g) ?? []).length;
  if (shown !== n) {
    return [`${pagePath}: the footer says ${w} diagnostic(s) and the page shows ${shown} \`heroes check\`/\`heroes build\` transcript(s).`];
  }
  return [];
}

/**
 * The pages whose claims have been checked in this build, for the end-of-build
 * audit. A FILE rather than a Set, and the reason is a module boundary: the
 * pages run this code from Astro's prerender bundle, and the build-done hook in
 * `astro.config.mjs` imports this file from source, so the two never share an
 * in-memory variable. What they share is the output directory. The hook reads
 * this file, runs the audit and deletes it, so it never ships.
 */
const VISITED_FILE = 'site/dist/.claims-visited';

function markVisited(pagePath: string): void {
  const file = absolute(VISITED_FILE);
  mkdirSync(dirname(file), { recursive: true });
  appendFileSync(file, `${pagePath}\n`);
}

/**
 * Check every claim a page carries. Throws with the page, the claim, what the
 * tree says and how the page would have to spell it, because "a claim drifted"
 * sends somebody reading twelve pages.
 */
export function checkClaims(html: string, pagePath: string): void {
  const problems: string[] = [];
  for (const claim of CLAIMS) {
    if (claim.page !== pagePath) continue;
    markVisited(pagePath);
    const n = claim.fact();
    const spelled = word(n, langOf(pagePath));
    if (!claim.shape(spelled).test(html)) {
      problems.push(
        `${pagePath}: ${claim.what} is ${n} in the tree, and the page does not say "${spelled}" where it should.\n` +
          `      expected a sentence matching: ${claim.shape(spelled)}`
      );
    }
  }
  problems.push(...checkChapterDiagnostics(html, pagePath));
  problems.push(...checkNoWrongTwin(html, pagePath));
  problems.push(...checkFixpointLeg(html, pagePath));
  problems.push(...checkRepositoryIsOpen(html, pagePath));
  if (NAME_EVERY_VERB.includes(pagePath)) {
    markVisited(pagePath);
    for (const verb of verbs()) {
      if (!html.includes(`<code>${verb}</code>`)) {
        problems.push(`${pagePath}: the command has a verb \`${verb}\` and the page does not name it as <code>${verb}</code>.`);
      }
    }
  }
  if (problems.length > 0) {
    throw new Error(
      `${problems.length} claim(s) on the page disagree with the repository, or left the page.\n    ` +
        problems.join('\n    ') +
        `\n  A number in prose is a claim about the tree, and so is a promise about what a reader\n` +
        `  cannot do yet (site/src/lib/claims.ts). Change the sentence to what the tree says, or if\n` +
        `  the sentence was deliberately removed, remove its row from the table.\n`
    );
  }
}

/**
 * The repository is open, and no page may say otherwise.
 *
 * This is the check the site did not have on the day it needed one. For five
 * days the site was public and the repository was not, and more than twenty
 * sentences across nine pages in two editions said so: the download command
 * *"is not available yet"*, the issue tracker *"will open when the code becomes
 * public"*, `GitHub · private` in the nav of every page. The moment the
 * repository opened, every one of them became false at once, and nothing here
 * could tell: this file checks numbers and named things, and its own header
 * says what that leaves out, that it cannot verify a sentence it does not know
 * about.
 *
 * So the sentence is known about now. This is a forbidden-phrase check rather
 * than a live one on purpose: the build is offline and deterministic, and
 * asking GitHub at build time would make a page's correctness depend on a
 * network call. What it actually guards is the realistic failure, a page or a
 * paragraph restored from an older copy, which is how a promise like this comes
 * back.
 *
 * A phrase leaves this list only when the repository stops being open.
 */
const CLOSED_REPOSITORY: { phrase: RegExp; lang: 'en' | 'it' }[] = [
  { phrase: /repository is still private/i, lang: 'en' },
  { phrase: /repository, still\s+private/i, lang: 'en' },
  { phrase: /which is still private/i, lang: 'en' },
  { phrase: /when the code is (?:made )?public/i, lang: 'en' },
  { phrase: /when the code becomes public/i, lang: 'en' },
  { phrase: /will open with the code/i, lang: 'en' },
  { phrase: /GitHub · private/i, lang: 'en' },
  { phrase: /repository è ancora privato/i, lang: 'it' },
  { phrase: /repository, ancora\s+privato/i, lang: 'it' },
  { phrase: /che è ancora privato/i, lang: 'it' },
  { phrase: /quando il codice (?:verrà|sarà|diventerà)/i, lang: 'it' },
  { phrase: /si apre insieme al codice/i, lang: 'it' },
  { phrase: /GitHub · privato/i, lang: 'it' },
];

function checkRepositoryIsOpen(html: string, pagePath: string): string[] {
  const out: string[] = [];
  const flat = html.replace(/\s+/g, ' ');
  for (const { phrase } of CLOSED_REPOSITORY) {
    const m = flat.match(phrase);
    if (m) {
      out.push(
        `${pagePath}: says "${m[0]}". The repository is open, so this promise is a false claim. ` +
          `Write what a reader can do now, not what they will be able to do.`
      );
    }
  }
  return out;
}

/**
 * Presence is not absence. Every row above asserts the right sentence exists;
 * none said a wrong twin does not, so "five judges" in one paragraph and "four
 * judges" in the next would have passed. For the nouns whose count is one fact
 * on the whole page, every number word written before the noun must be that
 * fact's word. Only nouns that mean one thing on the page: `judges` is always
 * the panel, where `lines` is the Zen in one paragraph and Rust in another.
 */
const UNAMBIGUOUS: { page: RegExp; noun: RegExp; fact: () => number }[] = [
  { page: /^site\/src\/html\/(index|project|about\/thanks)\.html$/, noun: /(judges|seats)/, fact: () => judges().seats },
  { page: /^site\/src\/html\/it\/(index|project|about\/thanks)\.html$/, noun: /(giudici|seggi)/, fact: () => judges().seats },
  { page: /^site\/src\/html\/start\.html$/, noun: /verbs/, fact: () => verbs().length },
  { page: /^site\/src\/html\/it\/start\.html$/, noun: /verbi/, fact: () => verbs().length },
  { page: /^site\/src\/html\/docs\/index\.html$/, noun: /chapters/, fact: chapters },
  { page: /^site\/src\/html\/it\/docs\/index\.html$/, noun: /capitoli/, fact: chapters },
];

function checkNoWrongTwin(html: string, pagePath: string): string[] {
  const out: string[] = [];
  const lang = langOf(pagePath);
  const flat = html.replace(/\s+/g, ' ');
  for (const rule of UNAMBIGUOUS) {
    if (!rule.page.test(pagePath)) continue;
    const right = word(rule.fact(), lang);
    // A digit counts, and so does one adjective between the number and the noun:
    // "4 judges" and "four separate judges" both slipped past the first version.
    const re = new RegExp(`\\b([a-z0-9]+)\\s+(?:[a-z]+\\s+)?${rule.noun.source}\\b`, 'gi');
    for (const m of flat.matchAll(re)) {
      const w = m[1].toLowerCase();
      const asWord = /^\d+$/.test(w) ? (WORDS[lang][Number(w)] ?? w) : w;
      if ((WORDS[lang].includes(asWord)) && asWord !== right) {
        out.push(`${pagePath}: says "${m[0]}" where the tree says ${right}. A wrong count beside the right one is still a wrong count.`);
      }
    }
  }
  return out;
}

/**
 * The fixpoint proof's footer says which leg runs it. That sentence must be
 * present exactly when the workflow gates the `cmp` step to Linux, and absent
 * when it does not, because either drift is the page contradicting the step.
 */
function checkFixpointLeg(html: string, pagePath: string): string[] {
  const en = pagePath === 'site/src/html/project.html';
  const it = pagePath === 'site/src/html/it/project.html';
  if (!en && !it) return [];
  const flat = html.replace(/\s+/g, ' ');
  const says = en ? /run only on Linux/.test(flat) : /vengono eseguiti solo su Linux/.test(flat);
  const gated = ci().fixpointLinuxOnly;
  if (gated && !says) return [`${pagePath}: ${CI_FILE} gates the fixpoint step to Linux and the page no longer says so.`];
  if (!gated && says) return [`${pagePath}: the page says the fixpoint runs on the Linux leg alone and ${CI_FILE} no longer gates it.`];
  return [];
}

/**
 * Every page the table names must have passed through `checkClaims` by the time
 * the build ends. Otherwise a renamed or deleted page takes its claims with it
 * in silence, which the veteran's seat demonstrated by rendering the home under
 * another path and watching every row skip. Returns how many pages were seen.
 */
export function assertEveryClaimVisited(): number {
  const file = absolute(VISITED_FILE);
  const visited = new Set(existsSync(file) ? readFileSync(file, 'utf-8').split('\n').filter(Boolean) : []);
  if (existsSync(file)) unlinkSync(file);
  const named = new Set([...CLAIMS.map((c) => c.page), ...NAME_EVERY_VERB]);
  const missed = [...named].filter((p) => !visited.has(p)).sort();
  if (missed.length > 0) {
    throw new Error(
      `${missed.length} page(s) named in the claims table never reached checkClaims():\n    ` +
        missed.join('\n    ') +
        `\n  A page that was renamed or removed takes its claims with it in silence unless this fires.\n` +
        `  Point the row at the page's new path, or remove the row if the page is gone.`
    );
  }
  return visited.size;
}

/** What the table holds, for the build's own report and for a test. */
export function claimsSummary(pages: number): string {
  const j = judges();
  const c = ci();
  return `${j.seats} judges, ${j.vetoes} vetoes, ${topLevelWords().size} top-level words, ${verbs().length} verbs, ${zenLines()} Zen lines, ${chapters()} chapters, ${specCeiling()}-token ceiling, ${c.tagPlatforms} tag platforms, fixpoint ${c.fixpointLinuxOnly ? 'Linux-only' : 'every leg'}; ${CLAIMS.length} claims and ${NAME_EVERY_VERB.length} verb lists checked over ${pages} pages`;
}
