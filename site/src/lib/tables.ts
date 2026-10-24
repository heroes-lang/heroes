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
const MAIN_FILE = 'selfhost/main.hero';

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
  // The heading may carry a section number, `## 2. Types`, since the
  // specification took numbered sections (M-anchored-spec); the number is
  // what this pattern looks past, and nothing else.
  const heading = /^##\s+(?:\d+\.\s+)?Types\s*$/m.exec(text);
  if (heading === null) {
    throw new Error(
      `${SPEC_FILE}: no \`## Types\` heading, numbered or not, to read the type words from.\n` +
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

/**
 * The contextual words, from `contextual()` in the lexer's own table.
 *
 * They are not keywords: `owned`, `tag` or `as` stays an ordinary name
 * everywhere except the one place the parser reads it as a mark, which is what
 * lets `owned` be an identifier at twenty-odd sites of the compiler. So the
 * highlighter needs them as a separate set and colours them only in those
 * places (`highlight.ts`, `markAt`). Neither this site nor the editor's grammar
 * coloured any of them until 2026-10-12, which two issues of September had
 * filed as a class: the table names fourteen and both colourings knew none.
 *
 * `contextual()` lists `WORD_*` constants rather than strings, so each name is
 * resolved through its own `constant WORD_X: str` and the string on the next
 * line. Both halves failing loudly is the same guard as `keywords()`'s.
 */
let cachedContextual: Set<string> | null = null;
const CONTEXTUAL_FLOOR = 10;

export function contextual(): Set<string> {
  if (cachedContextual !== null) return cachedContextual;

  const text = readText(KEYWORDS_FILE);
  const head = text.indexOf('function contextual() -> [str]');
  if (head < 0) {
    throw new Error(
      `${KEYWORDS_FILE}: no \`function contextual() -> [str]\` to read the contextual words from.\n` +
        `  the site colours them by that list; if the function was renamed, this parse follows it.`
    );
  }
  const close = text.indexOf(']', head + 'function contextual() -> [str]'.length);
  const names = [...text.slice(head, close).matchAll(/^\s+(WORD_[A-Z_]+)\s*$/gm)].map((m) => m[1]);

  const words = new Set<string>();
  for (const name of names) {
    const found = new RegExp(`^constant ${name}: str\\n\\s+"([a-z_]+)"`, 'm').exec(text);
    if (found === null) {
      throw new Error(`${KEYWORDS_FILE}: \`contextual()\` names ${name}, and no \`constant ${name}: str\` holds a word.`);
    }
    words.add(found[1]);
  }

  if (words.size < CONTEXTUAL_FLOOR) {
    throw new Error(
      `${KEYWORDS_FILE}: read only ${words.size} contextual words, and the floor is ${CONTEXTUAL_FLOOR}.\n` +
        `  a parse that falls apart colours none of them, which is why there is a floor instead of trust.`
    );
  }

  cachedContextual = words;
  return words;
}

const FOREIGN_FILE = 'selfhost/foreign_words.hero';
const SHOWN_FILE = 'selfhost/shown_char.hero';
const GRAMMAR_FILE = 'editors/vscode/syntaxes/heroes.tmLanguage.json';

/** 23 words today, in 17 arms. */
const FOREIGN_FLOOR = 15;

let cachedForeign: Set<string> | null = null;
let cachedBidi: [number, number][] | null = null;
const cachedRules = new Map<string, LiteralRule[]>();

/**
 * The words of another language the lexer refuses, from `declared_word` in
 * `selfhost/foreign_words.hero`, the table the compiler tells its repair from.
 * The highlighter paints them as refused, as the editor's grammar does, which
 * the `spec` suite holds to the same arms.
 *
 * An arm may name several words, `"null" | "nil" | "None" => ok(`, and the
 * `_ => fail(` arm ends the table; a parse that never reaches it is refused,
 * since it would not know the table had ended.
 */
export function foreignWords(): Set<string> {
  if (cachedForeign !== null) return cachedForeign;

  const text = readText(FOREIGN_FILE);
  const start = text.indexOf('function declared_word(');
  if (start < 0) {
    throw new Error(`${FOREIGN_FILE}: no \`function declared_word(\` to read the foreign words from.`);
  }
  const words = new Set<string>();
  let closed = false;
  for (const line of text.slice(start).split('\n').slice(1)) {
    if (/^\s*_\s*=>\s*fail\(/.test(line)) {
      closed = true;
      break;
    }
    const arm = /^\s+("[A-Za-z]+"(?:\s*\|\s*"[A-Za-z]+")*)\s*=>\s*ok\(/.exec(line);
    if (arm === null) continue;
    for (const word of arm[1].matchAll(/"([A-Za-z]+)"/g)) words.add(word[1]);
  }
  if (!closed) {
    throw new Error(`${FOREIGN_FILE}: read ${words.size} foreign words and never reached \`declared_word\`'s closing \`_ => fail(\` arm.`);
  }
  if (words.size < FOREIGN_FLOOR) {
    throw new Error(
      `${FOREIGN_FILE}: read only ${words.size} foreign words, and the floor is ${FOREIGN_FLOOR}.\n` +
        `  a parse that falls apart paints none of them as the mistake they are.`
    );
  }
  cachedForeign = words;
  return words;
}

/**
 * The bidirectional controls, as pairs of first and last code, from
 * `BIDI_CONTROL` in `selfhost/shown_char.hero`: with the control characters
 * and U+2028 and U+2029, what a line cannot show as itself (`unshowable`), so
 * what a string refuses raw and writes by its code.
 */
export function bidiControls(): [number, number][] {
  if (cachedBidi !== null) return cachedBidi;

  const text = readText(SHOWN_FILE);
  const start = text.indexOf('constant BIDI_CONTROL: [i64]');
  if (start < 0) throw new Error(`${SHOWN_FILE}: no \`constant BIDI_CONTROL: [i64]\` to read the bidirectional controls from.`);
  const close = text.indexOf(']', text.indexOf('[', start + 'constant BIDI_CONTROL: [i64]'.length));
  const codes = [...text.slice(start + 'constant BIDI_CONTROL: [i64]'.length, close).matchAll(/^\s+(\d+)\s*$/gm)].map((m) => Number(m[1]));
  if (codes.length < 8 || codes.length % 2 !== 0) {
    throw new Error(`${SHOWN_FILE}: read ${codes.length} codes in BIDI_CONTROL, and it holds pairs, four of them at least.`);
  }
  const pairs: [number, number][] = [];
  for (let i = 0; i < codes.length; i += 2) pairs.push([codes[i], codes[i + 1]]);
  cachedBidi = pairs;
  return pairs;
}

/** One rule of the editor's grammar for a literal: its pattern, and whether it paints a mistake. */
export interface LiteralRule {
  pattern: RegExp;
  refused: boolean;
}

/**
 * The editor's grammar's rules for a number or a character literal, in their
 * order, first match winning as an editor reads them.
 *
 * READ, NOT COPIED: the grammar is the one place the shapes of a literal are
 * written as patterns, and the `spec` suite runs these very rules over its
 * table of forms against the lexer (`literals_painted`), so the site paints
 * with patterns the compiler already judges, and `lexed.ts` judges them again
 * as the site runs them. Each is compiled sticky, so it matches where the
 * highlighter stands or not at all; the grammar's patterns use only what an
 * ECMAScript expression reads the same way, the classes, groups, quantifiers
 * and `\\b` its own reader in `tests/harness/textmate.hero` admits.
 */
export function grammarRules(name: 'number' | 'char'): LiteralRule[] {
  const cached = cachedRules.get(name);
  if (cached !== undefined) return cached;

  const grammar = JSON.parse(readText(GRAMMAR_FILE));
  const patterns = grammar?.repository?.[name]?.patterns;
  if (!Array.isArray(patterns)) throw new Error(`${GRAMMAR_FILE}: no \`repository.${name}.patterns\` to read the ${name} rules from.`);
  const rules: LiteralRule[] = patterns.map((rule: { match?: string; name?: string }) => {
    if (typeof rule.match !== 'string' || typeof rule.name !== 'string') {
      throw new Error(`${GRAMMAR_FILE}: a rule of \`${name}\` without a \`match\` and a \`name\`, which this reader cannot run.`);
    }
    const refused = rule.name.startsWith('invalid.illegal');
    if (!refused && !rule.name.startsWith('constant.')) {
      throw new Error(`${GRAMMAR_FILE}: the \`${name}\` rule \`${rule.name}\` paints neither a literal nor a mistake.`);
    }
    return { pattern: new RegExp(rule.match, 'y'), refused };
  });
  const floor = name === 'number' ? 8 : 3;
  if (rules.length < floor) throw new Error(`${GRAMMAR_FILE}: read ${rules.length} \`${name}\` rules, and the floor is ${floor}.`);
  cachedRules.set(name, rules);
  return rules;
}

/** What the two parses found, for the build's own report and for a test. */
export function tableSummary(): string {
  return `${keywords().size} keywords from ${KEYWORDS_FILE}, ${typeWords().size} type words from ${SPEC_FILE}`;
}

/**
 * The version the compiler prints, read from the one constant that holds it.
 *
 * It was typed into both home pages by hand and went stale inside one release:
 * the pages said 0.1.0 while `heroes --version` said 0.2.0, which the language
 * veteran's seat found in its first minute. `site/CLAUDE.md` § A number on the
 * page is generated, or it is a threshold already required this, and the
 * version is exactly the number a stranger checks first, because checking it
 * costs one command.
 *
 * `VERSION` moves only in the commit that carries a release tag
 * (`.claude/rules/records.md` § Release tags), so reading it here means the
 * page cannot disagree with the binary again.
 */
export function version(): string {
  const text = readText(MAIN_FILE);
  const found = /constant VERSION: str\n\s+"([0-9]+\.[0-9]+\.[0-9]+)"/.exec(text);
  if (found === null) {
    throw new Error(
      `${MAIN_FILE}: no \`constant VERSION: str\` holding an X.Y.Z string.\n` +
        `  the site prints the version the compiler prints, and reads it from that constant.`
    );
  }
  return found[1];
}
