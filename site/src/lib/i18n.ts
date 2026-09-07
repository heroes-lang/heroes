/**
 * The handful of words the generated pages need in both languages.
 *
 * The prose of the Italian edition lives in `site/src/html/it/`, written rather
 * than translated (CLAUDE.md § 11's second declared exception). What is here is
 * the furniture a generated page cannot take from a fragment: the word beside a
 * line count, the label on a badge, the word before the next example's name.
 *
 * Two rules this file obeys, and both are checked by an instrument rather than
 * by memory. Every entry stays SHORT, well under the six words
 * `tests/harness/suite_records.hero`'s Italian check needs on a line before it
 * looks, because `site/src/lib/` is not in that check's list of trees where
 * Italian is the deliverable, and a dead exemption is worse than none. And
 * numbers take Italian conventions on the Italian pages, `2.636` where the
 * English says `2,636`, which is `site/README.md` § The Italian edition's rule
 * applied to a count this build produces.
 */

export type Lang = 'en' | 'it';

type Phrase = { en: string; it: string };

const PHRASES: Record<string, Phrase> = {
  examples: { en: 'Examples', it: 'Esempi' },
  lines: { en: 'lines', it: 'righe' },
  line: { en: 'line', it: 'riga' },
  modules: { en: 'modules', it: 'moduli' },
  module: { en: 'module', it: 'modulo' },
  programs: { en: 'programs', it: 'programmi' },
  files: { en: 'files', it: 'file' },
  // The singular exists because the English needs one and the Italian does not:
  // `file` is invariable, so the Italian generator was right on all 13 pages
  // where the English printed `1 files`. On a site whose pitch is that it
  // survives cross-checking, a plural that does not agree is the reader's first
  // free reason to doubt the rest.
  file: { en: 'file', it: 'file' },
  runIt: { en: 'Run it', it: 'Eseguilo' },
  theProgram: { en: 'The program', it: 'Il programma' },
  theInput: { en: 'Input', it: 'Input' },
  exit: { en: 'exit', it: 'exit' },
  next: { en: 'Next', it: 'Poi' },
  previous: { en: 'Previous', it: 'Prima' },
  backToTheList: { en: 'All the examples', it: 'Tutti gli esempi' },
  fingerprint: { en: 'fingerprint', it: 'impronta' },
  readsFile: { en: 'reads a file', it: 'legge un file' },
  writesFile: { en: 'writes a file', it: 'scrive un file' },
  commandLine: { en: 'command line', it: 'riga di comando' },
  threads: { en: 'threads', it: 'thread' },
  bindsC: { en: 'binds C', it: 'usa C' },
  reads: { en: 'Read it', it: 'Leggilo' },
  reproduce: { en: 'Reproduce it', it: 'Riproducila' },
  // The way OFF the examples shelf. Every label here stays under six words:
  // `records/english` reads this file and Italian prose belongs to the edition,
  // not to a table both editions share.
  runOneYourself: { en: 'Run one yourself', it: 'Eseguine uno' },
  theFourCommands: { en: 'the four commands', it: 'i quattro comandi' },
  learnTheLanguage: { en: 'Learn the language', it: 'Impara il linguaggio' },
  orTakeItWhole: { en: 'or take it whole', it: 'oppure prendilo tutto' },
  inOneFile: { en: 'in one file', it: 'in un file solo' },
};

/**
 * Note what is NOT here: the two sentences that explain why a page shows no
 * output. They are prose rather than furniture, they run past six words, and
 * `site/src/lib/` is not one of the trees where
 * `tests/harness/suite_records.hero` accepts Italian, so putting an Italian
 * sentence here would either trip that check or need an exemption that reads as
 * a licence for the next one. They live as fragments beside the pages instead,
 * `site/src/html/examples/_no-output-*.html` and their Italian twins, which is
 * also where the author can edit them without opening a TypeScript file.
 */

/** One phrase in one language. A missing key is a build error, not a blank. */
export function t(key: string, lang: Lang): string {
  const phrase = PHRASES[key];
  if (phrase === undefined) {
    throw new Error(`site/src/lib/i18n.ts has no phrase called \`${key}\`.`);
  }
  return phrase[lang];
}

/**
 * A count, in the reader's own convention: `18.622` in Italian, `18,622` in
 * English.
 */
export function number(value: number, lang: Lang): string {
  const digits = String(value);
  if (digits.length < 4) return digits;
  const separator = lang === 'it' ? '.' : ',';
  let out = '';
  for (let i = 0; i < digits.length; i += 1) {
    if (i > 0 && (digits.length - i) % 3 === 0) out += separator;
    out += digits[i];
  }
  return out;
}

/** `1 line` and `2 lines`, in either language. */
export function countOf(value: number, singular: string, plural: string, lang: Lang): string {
  return `${number(value, lang)} ${t(value === 1 ? singular : plural, lang)}`;
}
