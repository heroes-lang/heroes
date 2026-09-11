/**
 * Colouring a Heroes program for the page, and proving the colours changed
 * nothing.
 *
 * This is a reading of the compiler's own lexer, close enough to be checkable:
 * `selfhost/scan.hero`'s `line_body` dispatches on the first byte of each token,
 * `selfhost/number.hero` says which characters are a number in each of the four
 * bases, and `selfhost/literals.hero` says a string and a character literal
 * stop at the end of their line. Everything here follows those three, and where
 * it cannot follow them it stays plain rather than guessing.
 *
 * It is deliberately NOT a parser. Five classes exist in the stylesheet, the
 * comment, the keyword, the built-in type, the string and the number, and a
 * user's own type is left uncoloured on purpose (`site/README.md` § Syntax
 * colouring). A word after a dot stays plain too, because `s.cstr()` calls a
 * method and `cstr` there is not the type: that is a fact about the character
 * before the word rather than a premise about the program.
 *
 * The guarantee that makes this safe to run over 118 files nobody re-reads is
 * `assertRoundTrip`: strip the spans, decode the three entities, and what comes
 * back must be the source byte for byte. A highlighter that eats a character is
 * a page that shows a program which does not exist, and the site's whole
 * mechanism is that a reader can check it.
 */

import { keywords, typeWords } from './tables.ts';

/** What a token is coloured as. `ident` and `other` carry no class. */
export type TokenKind = 'c' | 'k' | 't' | 's' | 'n' | 'ident' | 'other';

export interface Token {
  kind: TokenKind;
  text: string;
}

const CLASS_OF: Record<TokenKind, string | null> = {
  c: 'c',
  k: 'k',
  t: 't',
  s: 's',
  n: 'n',
  ident: null,
  other: null,
};

const ENTITIES: Record<string, string> = {
  '&amp;': '&',
  '&lt;': '<',
  '&gt;': '>',
  '&quot;': '"',
  '&#39;': "'",
};

/**
 * The three characters that must be escaped inside a `<pre><code>`, and only
 * those three: it is the set the 160 figures already written by hand use, and
 * matching them means the drift check compares like with like.
 */
export function escapeHtml(text: string): string {
  return text.replace(/[&<>]/g, (ch) => (ch === '&' ? '&amp;' : ch === '<' ? '&lt;' : '&gt;'));
}

/**
 * Decode the entities a code block may carry, in ONE pass.
 *
 * Sequentially replacing `&amp;` and then `&lt;` would turn the escaped text
 * `&amp;lt;` into `<`, which is a different program from the one on the page.
 * One regular expression with one lookup cannot make that mistake.
 */
export function decodeEntities(html: string): string {
  return html.replace(/&(?:amp|lt|gt|quot|#39);/g, (entity) => ENTITIES[entity]);
}

const isDigit = (ch: string) => ch >= '0' && ch <= '9';
const isAlpha = (ch: string) => (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
const isAlnum = (ch: string) => isAlpha(ch) || isDigit(ch);
const isIdentChar = (ch: string) => isAlnum(ch) || ch === '_';
const isHex = (ch: string) => isDigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');

/**
 * Where a plain string or character literal starting at `start` ends: after
 * its closing quote, or at the end of the line, which is as far as one can run
 * (`literals.hero`). A backslash consumes what follows it.
 */
function literalEnd(source: string, start: number): number {
  const quote = source[start];
  let end = start + 1;
  while (end < source.length && source[end] !== '\n' && source[end] !== quote) {
    end += source[end] === '\\' ? 2 : 1;
  }
  if (end < source.length && source[end] === quote) end += 1;
  return Math.min(end, source.length);
}

/**
 * Where the piece of a string with holes that starts at `start` ends: after
 * the `{` that opens a hole, after the `"` that closes the literal, or at the
 * end of the line. `{{` is one brace of text and stays inside the piece; a
 * lone `}` is text too, because only the opening brace is ever special
 * (`spec/heroes-spec.md`, "`{{` writes one brace").
 */
function pieceEnd(source: string, start: number): { end: number; opensHole: boolean } {
  let end = start;
  while (end < source.length && source[end] !== '\n') {
    const ch = source[end];
    if (ch === '\\') {
      end += 2;
      continue;
    }
    if (ch === '"') return { end: end + 1, opensHole: false };
    if (ch === '{') {
      if (source[end + 1] === '{') {
        end += 2;
        continue;
      }
      return { end: end + 1, opensHole: true };
    }
    end += 1;
  }
  return { end: Math.min(end, source.length), opensHole: false };
}

/**
 * Where the hole whose code starts at `start` closes: the index of the `}`
 * that brings the bracket depth back to zero, with nested literals and nested
 * strings with holes skipped whole. The compiler's lexer does the same by
 * recording the bracket depth when a hole opens (`lex_interp.hero`).
 */
function holeEnd(source: string, start: number): number {
  let depth = 0;
  let at = start;
  while (at < source.length && source[at] !== '\n') {
    const ch = source[at];
    if (ch === 'f' && source[at + 1] === '"' && !isIdentChar(source[at - 1] ?? ' ')) {
      at = interpolatedEnd(source, at);
      continue;
    }
    if (ch === '"' || ch === "'") {
      at = literalEnd(source, at);
      continue;
    }
    if (ch === '(' || ch === '[' || ch === '{') depth += 1;
    if (ch === ')' || ch === ']') depth -= 1;
    if (ch === '}') {
      if (depth === 0) return at;
      depth -= 1;
    }
    at += 1;
  }
  return at;
}

/** Where the string with holes that starts at `start` (its `f`) ends. */
function interpolatedEnd(source: string, start: number): number {
  // The head piece begins at the `f` and includes the quote after it.
  let at = start + 2;
  for (;;) {
    const piece = pieceEnd(source, at);
    if (!piece.opensHole) return piece.end;
    const close = holeEnd(source, piece.end);
    if (close >= source.length || source[close] !== '}') return close;
    at = close;
  }
}

/**
 * The tokens of one whole string with holes: its pieces as strings, the code
 * inside each hole tokenized as the code it is. Every character of `text`
 * lands in exactly one token, which is what the round trip checks.
 */
function interpolatedTokens(text: string): Token[] {
  const out: Token[] = [];
  let at = 0;
  // The head piece includes the `f` and the quote, so it starts at 0 and the
  // scan for its end starts after them.
  let scanFrom = 2;
  for (;;) {
    const piece = pieceEnd(text, scanFrom);
    out.push({ kind: 's', text: text.slice(at, piece.end) });
    if (!piece.opensHole) return out;
    const close = holeEnd(text, piece.end);
    for (const token of tokenize(text.slice(piece.end, close))) out.push(token);
    if (close >= text.length) return out;
    // The `}` that closed the hole is the first character of the next piece.
    at = close;
    scanFrom = close + 1;
  }
}

/** The digits each base admits, `_` included: `selfhost/number.hero`'s Base table. */
function inBase(ch: string, marker: string): boolean {
  if (ch === '_') return true;
  if (marker === 'x') return isHex(ch);
  if (marker === 'o') return ch >= '0' && ch <= '7';
  if (marker === 'b') return ch === '0' || ch === '1';
  return isDigit(ch);
}

/**
 * Split a program into coloured tokens.
 *
 * Every character of the source lands in exactly one token, which is what
 * `assertRoundTrip` then checks: whitespace and punctuation come back as
 * `other`, so nothing is dropped on the floor.
 */
export function tokenize(source: string): Token[] {
  const kw = keywords();
  const types = typeWords();
  const out: Token[] = [];
  let i = 0;

  /**
   * The previous token that decides whether a word is a member, and what it
   * skips is the whole rule.
   *
   * It skips whitespace, because `x .f()` is not written here but a newline and
   * an indent sit between every two lines. It skips COMMENTS, and that one was
   * a real defect rather than a nicety: this started out remembering the last
   * non-space CHARACTER, so a comment ending in a full stop, which is how most
   * comments in this repository end, made the next word look like a member and
   * left `function` uncoloured. Measured against the 160 figures already on the
   * site, it did that 70 times for `function`, 10 for `record` and 2 for
   * `extern`. The compiler's own lexer never had the bug, and says why in its
   * own words: a comment "bypasses `emit` and leaves `last_significant` alone"
   * (`selfhost/scan.hero::comment`).
   */
  let previous: Token | null = null;

  /**
   * And the one before that, which only `::` needs: punctuation is pushed one
   * character at a time, so the two colons are two tokens (panel 132).
   */
  let beforePrevious: Token | null = null;

  const push = (kind: TokenKind, text: string) => {
    const token = { kind, text };
    out.push(token);
    if (kind === 'c') return;
    if (kind === 'other' && text.trim().length === 0) return;
    beforePrevious = previous;
    previous = token;
  };

  /** A word is a member when the token before it is exactly the dot. */
  const afterDot = () => previous !== null && previous.kind === 'other' && previous.text === '.';

  /**
   * ...and also when it follows `::`, the field-name operator (`Point::x`,
   * panel 132). Punctuation is pushed one character at a time here, so the
   * token before the word is the SECOND colon and the one before that is the
   * first. Without this a field called `str` or `record` would be coloured as
   * a type or a keyword — which is the mis-colouring a highlighter does
   * instead of erroring.
   */
  const afterColons = () =>
    previous !== null &&
    previous.kind === 'other' &&
    previous.text === ':' &&
    beforePrevious !== null &&
    beforePrevious.kind === 'other' &&
    beforePrevious.text === ':';

  while (i < source.length) {
    const ch = source[i];

    // A comment runs to the end of the line and never includes the newline,
    // because the newline is what ends it (`scan.hero::comment`).
    if (ch === '#') {
      let end = i;
      while (end < source.length && source[end] !== '\n') end += 1;
      push('c', source.slice(i, end));
      i = end;
      continue;
    }

    // A string with holes, `f"line {n}: {word}"`, read as the compiler's lexer
    // reads it (`selfhost/lex_interp.hero`): `f"` opens one token that runs to
    // the first `{` that is not `{{`, each hole is ordinary code up to the `}`
    // that closes it at its own bracket depth, and the piece after that `}`
    // runs to the next hole or the closing quote. Before this, the string
    // class ran from the opening quote to the FIRST quote it met, so a hole
    // holding a nested literal, `f"seen {seen["Sirius"]}"`, ended the string
    // in the middle of the hole and coloured the rest of the line as code, on
    // the one page whose prose says a hole may hold a nested literal. The `f`
    // is inside the string token because that is where the lexer puts it.
    if (ch === 'f' && source[i + 1] === '"' && !isIdentChar(source[i - 1] ?? ' ')) {
      const end = interpolatedEnd(source, i);
      for (const token of interpolatedTokens(source.slice(i, end))) push(token.kind, token.text);
      i = end;
      continue;
    }

    // A string and a character literal both stop at the end of the line: a
    // string is not multi-line in this language, so an unterminated one has a
    // known extent (`literals.hero`). A backslash consumes what follows it.
    if (ch === '"' || ch === "'") {
      const end = literalEnd(source, i);
      // Both quoted forms take the string class. A character literal IS an
      // integer by the specification's Types table, so the number class would
      // be defensible, and the stylesheet paints `.s` and `.n` the same gold
      // either way. What settles it is the 160 figures already on the site:
      // they were written by hand, read by the author and ratified, and they
      // say `.s`. Agreeing with them costs nothing and leaves the drift check
      // comparing markup with markup.
      push('s', source.slice(i, end));
      i = end;
      continue;
    }

    if (isDigit(ch)) {
      let end = i;
      let marker = 'd';
      if (ch === '0' && end + 1 < source.length && 'xob'.includes(source[end + 1].toLowerCase())) {
        marker = source[end + 1].toLowerCase();
        end += 2;
      }
      while (end < source.length && inBase(source[end], marker)) end += 1;
      // `1.5` is one token and `1.str()` is three: the dot makes a float only
      // when a digit follows it (`number.hero::number`).
      if (marker === 'd' && source[end] === '.' && isDigit(source[end + 1] ?? '')) {
        end += 1;
        while (end < source.length && inBase(source[end], 'd')) end += 1;
      }
      push('n', source.slice(i, end));
      i = end;
      continue;
    }

    if (isAlpha(ch) || ch === '_') {
      let end = i;
      while (end < source.length && (isAlnum(source[end]) || source[end] === '_')) end += 1;
      const word = source.slice(i, end);
      const kind: TokenKind =
      afterDot() || afterColons() ? 'ident' : kw.has(word) ? 'k' : types.has(word) ? 't' : 'ident';
      push(kind, word);
      i = end;
      continue;
    }

    // Whitespace and punctuation, one character at a time. Nothing is coloured
    // here, so nothing needs to know the operator table.
    push('other', ch);
    i += 1;
  }

  return out;
}

/** The program as HTML: the spans the stylesheet colours, and nothing else. */
export function highlight(source: string): string {
  let out = '';
  for (const token of tokenize(source)) {
    const className = CLASS_OF[token.kind];
    const text = escapeHtml(token.text);
    out += className === null ? text : `<span class="${className}">${text}</span>`;
  }
  return out;
}

/** The text a code block holds, with the markup and the entities taken off. */
export function plain(html: string): string {
  return decodeEntities(html.replace(/<span class="[a-z]">/g, '').replace(/<\/span>/g, ''));
}

/**
 * Assert that colouring a program changed none of it.
 *
 * Two comparisons, because they fail differently: the strings differing points
 * at a dropped or duplicated character, and the byte lengths differing while
 * the strings match would mean the decode invented an entity.
 */
export function assertRoundTrip(source: string, label: string): void {
  const restored = plain(highlight(source));
  if (restored === source) {
    const before = Buffer.byteLength(source, 'utf8');
    const after = Buffer.byteLength(restored, 'utf8');
    if (before === after) return;
    throw new Error(
      `${label}: the highlighter round-trips to equal text of a different size, ${before} bytes in and ${after} out.`
    );
  }

  let at = 0;
  while (at < source.length && at < restored.length && source[at] === restored[at]) at += 1;
  const line = source.slice(0, at).split('\n').length;
  const context = (text: string) => JSON.stringify(text.slice(Math.max(0, at - 30), at + 30));
  throw new Error(
    `${label}: the highlighter did not round-trip. First difference at line ${line}, offset ${at}.\n` +
      `  source: ${context(source)}\n` +
      `  after:  ${context(restored)}\n` +
      `  a page that shows a program the file does not hold is the one thing this site must never do.`
  );
}

/**
 * The identifiers a program mentions, comments and strings excluded.
 *
 * The examples model derives its badges from this rather than from a grep,
 * because `read_file` inside a comment is prose about a program and not a
 * program that reads a file.
 */
export function words(source: string): Set<string> {
  const found = new Set<string>();
  for (const token of tokenize(source)) {
    if (token.kind === 'ident' || token.kind === 'k' || token.kind === 't') found.add(token.text);
  }
  return found;
}

/** The string literals a program holds, with their quotes taken off. */
export function stringLiterals(source: string): string[] {
  const out: string[] = [];
  for (const token of tokenize(source)) {
    if (token.kind !== 's') continue;
    const text = token.text;
    if (text.length >= 2 && text.startsWith('"') && text.endsWith('"')) out.push(text.slice(1, -1));
  }
  return out;
}
