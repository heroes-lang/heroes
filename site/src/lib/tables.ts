/**
 * The two word tables the highlighter colours by, read from the compiler and
 * from the specification at build time.
 *
 * The site has coloured code since it existed, and `site/README.md` § Syntax
 * colouring already promises where the colours come from: the keyword class is
 * the lexer's own table and the type class is the specification's list, so a
 * user's own type stays uncoloured because inferring one from a capital letter
 * would be a premise about a convention the spec does not state. That promise
 * was kept by hand, by a generator that lived outside the repository. Here it is
 * kept by reading the two files, so a word that enters the language colours
 * itself and a word that leaves it stops colouring.
 *
 * Both parses fail loudly, with a floor and a shape assertion, because the
 * failure this guards against is not an exception but a silent empty table: a
 * regular expression that stops matching would leave every keyword plain and
 * nothing would look broken enough to notice (CLAUDE.md § 11, and a premise
 * about the world owes a test that fires when it dies).
 */

import { readText } from './repo.ts';

const KEYWORDS_FILE = 'selfhost/keywords.hero';
const SPEC_FILE = 'spec/heroes-spec.md';

/**
 * The table has 21 rows today. The floor is 15 rather than 21 so that adding a
 * keyword does not break the site build, while a parse that falls apart and
 * matches three rows does.
 */
const KEYWORD_FLOOR = 15;

/** 14 type words today, and three that must be among them whatever else moves. */
const TYPE_FLOOR = 10;
const TYPES_EXPECTED = ['i64', 'str', 'bool'];

let cachedKeywords: Set<string> | null = null;
let cachedTypes: Set<string> | null = null;

/**
 * The keywords, from `keyword()` in the lexer's own table.
 *
 * The rows read `"constant" => ok(.kw_constant)`, and the parse asserts the two
 * halves agree: the quoted word must be the variant case's name after `kw_`.
 * That is what makes this a reading of the table rather than a regular
 * expression that happens to find quoted words, and it is why a row like
 * `"fn" => ok(.kw_function)` in the FOREIGN table below cannot leak in: this
 * function stops at the arm that closes `keyword()`.
 */
export function keywords(): Set<string> {
  if (cachedKeywords !== null) return cachedKeywords;

  const text = readText(KEYWORDS_FILE);
  const start = text.indexOf('function keyword(');
  if (start < 0) {
    throw new Error(
      `${KEYWORDS_FILE}: no \`function keyword(\` to read the table from.\n` +
        `  the site colours keywords by that table (site/README.md, Syntax colouring).\n` +
        `  if the function was renamed, this parse follows it.`
    );
  }

  const words = new Set<string>();
  const rows = /^\s*"([a-z]+)"\s*=>\s*ok\(\.kw_([a-z]+)\)\s*$/;
  let closed = false;

  for (const line of text.slice(start).split('\n').slice(1)) {
    const row = rows.exec(line);
    if (row !== null) {
      const [, word, variant] = row;
      if (word !== variant) {
        throw new Error(
          `${KEYWORDS_FILE}: the row for \`${word}\` names the case \`kw_${variant}\`.\n` +
            `  this parse reads the table by that agreement, so it cannot read this row.\n` +
            `  if the two are deliberately different, the site's parse needs the new rule.`
        );
      }
      words.add(word);
      continue;
    }
    // The `_ => fail(...)` arm ends the table. Stopping here is what keeps the
    // foreign-word table, further down the same file, out of the keyword set.
    if (/^\s*_\s*=>\s*fail\(/.test(line)) {
      closed = true;
      break;
    }
  }

  if (!closed) {
    throw new Error(
      `${KEYWORDS_FILE}: read ${words.size} keyword rows and never reached the closing \`_ => fail(\` arm.\n` +
        `  without that arm this parse cannot tell where the keyword table ends and\n` +
        `  the foreign-word table begins, and foreign words must not colour as keywords.`
    );
  }

  if (words.size < KEYWORD_FLOOR) {
    throw new Error(
      `${KEYWORDS_FILE}: read only ${words.size} keywords, and the floor is ${KEYWORD_FLOOR}.\n` +
        `  a parse that falls apart returns a small table and colours nothing, which is\n` +
        `  why there is a floor instead of trust.`
    );
  }

  cachedKeywords = words;
  return words;
}

/**
 * The built-in type words, from the specification's Types table.
 *
 * The first column carries words and shapes together, `i8` beside `[T]` and
 * `{K: V}` and `T?` and `()`. Only the words can be coloured by name, so the
 * shapes are filtered out by the one property that separates them: a type word
 * is lowercase letters and digits and nothing else.
 */
export function typeWords(): Set<string> {
  if (cachedTypes !== null) return cachedTypes;

  const text = readText(SPEC_FILE);
  const heading = /^##\s+Types\s*$/m.exec(text);
  if (heading === null) {
    throw new Error(
      `${SPEC_FILE}: no \`## Types\` heading to read the type words from.\n` +
        `  the site colours built-in types by that table (site/README.md, Syntax colouring).`
    );
  }

  const after = text.slice(heading.index + heading[0].length);
  const end = /^##\s+/m.exec(after);
  const section = end === null ? after : after.slice(0, end.index);

  const words = new Set<string>();
  for (const line of section.split('\n')) {
    if (!line.startsWith('| `')) continue;
    const cell = line.slice(1, line.indexOf('|', 1));
    for (const token of cell.match(/`[^`]+`/g) ?? []) {
      const word = token.slice(1, -1);
      if (/^[a-z][a-z0-9]*$/.test(word)) words.add(word);
    }
  }

  for (const expected of TYPES_EXPECTED) {
    if (!words.has(expected)) {
      throw new Error(
        `${SPEC_FILE}: the Types table gave ${words.size} words and \`${expected}\` is not among them.\n` +
          `  read: ${[...words].join(' ')}\n` +
          `  this parse reads the first column of rows beginning with a backticked type.`
      );
    }
  }

  if (words.size < TYPE_FLOOR) {
    throw new Error(
      `${SPEC_FILE}: read only ${words.size} type words, and the floor is ${TYPE_FLOOR}.\n` +
        `  read: ${[...words].join(' ')}`
    );
  }

  cachedTypes = words;
  return words;
}

/** What the two parses found, for the build's own report and for a test. */
export function tableSummary(): string {
  return `${keywords().size} keywords from ${KEYWORDS_FILE}, ${typeWords().size} type words from ${SPEC_FILE}`;
}
