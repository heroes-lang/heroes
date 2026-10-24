/**
 * The site's colouring held to the compiler's lexer, at every build.
 *
 * The editor's grammar has been held to the lexer by the `spec` suite since
 * 2026-10-12 (`literals_painted` and `words_painted`): its rules for each
 * literal are run over a table of forms, and each form is lexed by the
 * compiler. The site's highlighter had no such judge. Its one instrument was
 * the round trip (`assertRoundTrip`), which proves the colours changed no byte
 * and says nothing about whether they are right. The author's request of
 * 2026-10-21, meant as: *do for the site's highlighter what was done for the
 * editor's plugin*. So this asks the lexer itself, three ways.
 *
 * - **Every program the site colours**, the `.hero` files under `examples/`,
 *   token for token: each token the lexer prints with text is one token here,
 *   of a class its kind may take, and nothing here is a token the lexer does
 *   not have but whitespace. The lexer says `ident` where the parser decides,
 *   so an `ident` may be painted a type word, a contextual mark or a declared
 *   name here, each only for a word of its own table.
 * - **The suite's table of literal forms**, `literal_forms` in
 *   `tests/harness/suite_spec.hero`, read from there and never copied: each
 *   written into the program the suite writes for it, lexed, and the painting
 *   where the form stands held to the verdict, as `form_disagreement` holds
 *   the grammar's. A legal form is one literal reaching its end, a refused one
 *   a refusal where it begins, a split one neither, a plain one no refusal.
 * - **Every escape a string can be written with**: a backslash before each
 *   printable character, a code escape for each code up to U+00A0 and at each
 *   end of the compiler's bidirectional ranges, five misspellings, and `{{`,
 *   `}}` and a lone `}` in an `f` literal; each lexed, and the escape painted
 *   `e` where the lexer reads it and `x` where it refuses it. This is where the
 *   editor's grammar is looser than the lexer (measured 2026-10-24, `\u{e9}`
 *   painted an escape and refused), and the site is not.
 *
 * WHAT IT CANNOT SEE: where a contextual word is a mark and where a name. The
 * lexer reads `lent`, `owned` or `as` as a word in either place and the parser
 * decides, so a contextual word painted a mark is accepted here wherever the
 * lexer reads a word; where the site paints one is `markAt`'s, written by the
 * rules the editor's grammar uses for the same places.
 *
 * THE COMPILER is `<root>/heroes`, the binary every gate needs, and its absence
 * refuses the build with the command that makes it: the colouring is a claim
 * about the language, and a check that skipped where it could not ask would be
 * a claim nobody judged. The deploy builds the tree's own before the site
 * (`.github/workflows/deploy-site.yml`).
 */

import { execFileSync } from 'node:child_process';
import { existsSync, mkdirSync, unlinkSync, writeFileSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { absolute, readText, recordRead, repoRoot, walk } from './repo.ts';
import { tokenize, type Token, type TokenKind } from './highlight.ts';
import { bidiControls, contextual, typeWords } from './tables.ts';

const COMPILER = 'heroes';
const SPEC_SUITE = 'tests/harness/suite_spec.hero';
const SCRATCH = 'site/dist/.lexed.hero';

/** 120 programs today. */
const PROGRAMS_FLOOR = 100;

/** 76 forms today: 42 numbers, 18 character literals, 8 in strings, 8 in comments. */
const FORMS_FLOOR = 70;

interface LexToken {
  kind: string;
  text: string;
  line: number;
  col: number;
}

interface Form {
  rule: string;
  written: string;
  lexed: string;
}

interface Asked {
  code: number;
  out: string;
}

function compiler(): string {
  const path = join(repoRoot(), COMPILER);
  if (!existsSync(path)) {
    throw new Error(
      `the site's colouring is held to the compiler's lexer, and there is no compiler at ${path}.\n` +
        `  Build it from the repository's root, as CLAUDE.md § Commands does:\n` +
        `    clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
    );
  }
  // The binary is built from these, which no reader here opens.
  recordRead('seed/heroes.c');
  recordRead('runtime');
  return path;
}

function ask(path: string, words: string[]): Asked {
  try {
    const out = execFileSync(path, words, { cwd: repoRoot(), encoding: 'utf8', stdio: ['ignore', 'pipe', 'pipe'], maxBuffer: 64 * 1024 * 1024 });
    return { code: 0, out };
  } catch (error) {
    const failed = error as { status?: number | null; stdout?: string };
    if (typeof failed.status === 'number') return { code: failed.status, out: failed.stdout ?? '' };
    throw error;
  }
}

/* -- The programs ---------------------------------------------------------- */

const CLASSED = (kind: string) =>
  kind === 'comment' || kind.startsWith('kw_') || kind === 'ident' || kind === 'int' || kind === 'float' ||
  kind === 'str' || kind === 'char' || kind === 'str_piece' || kind === 'hole';

function takes(kind: string, ours: TokenKind, text: string): boolean {
  if (kind === 'comment') return ours === 'c';
  if (kind.startsWith('kw_')) return ours === 'k';
  if (kind === 'int' || kind === 'float') return ours === 'n';
  if (kind === 'str' || kind === 'char' || kind === 'str_piece') return ours === 's';
  if (kind === 'hole') return ours === 'h';
  // `ident`: the parser decides what a word is, so the lexer's word may be
  // painted as one of the parser's, each only for a word of its own table.
  if (ours === 'ident' || ours === 'd') return true;
  if (ours === 't') return typeWords().has(text);
  if (ours === 'k') return contextual().has(text);
  return false;
}

function said(token: Token | undefined): string {
  return token === undefined ? 'nothing' : `${token.kind} \`${token.text}\``;
}

/** Where the site's tokens of one program part from the lexer's; empty where they agree. */
function partings(path: string, source: string, lexed: LexToken[]): { problems: string[]; compared: number } {
  const ours = tokenize(source).filter((token) => !(token.kind === 'other' && token.text.trim() === ''));
  let at = 0;
  let compared = 0;
  for (const theirs of lexed) {
    if (theirs.text === '') continue;
    const where = `${path}:${theirs.line}:${theirs.col}`;
    if (CLASSED(theirs.kind)) {
      const token = ours[at];
      if (token === undefined || token.text !== theirs.text || !takes(theirs.kind, token.kind, theirs.text)) {
        return { problems: [`${where}: the lexer reads ${theirs.kind} \`${theirs.text}\`, and the site paints ${said(token)}`], compared };
      }
      at += 1;
    } else {
      // Punctuation is one token to the lexer and one character at a time here.
      let text = '';
      while (text.length < theirs.text.length && at < ours.length && ours[at].kind === 'other') text += ours[at++].text;
      if (text !== theirs.text) {
        return { problems: [`${where}: the lexer reads ${theirs.kind} \`${theirs.text}\`, and the site reads \`${text}\` there`], compared };
      }
    }
    compared += 1;
  }
  if (at < ours.length) return { problems: [`${path}: the site paints ${said(ours[at])} where the lexer has no token`], compared };
  return { problems: [], compared };
}

/* -- The table of forms ---------------------------------------------------- */

/** A Heroes string literal's text, its quotes off and its escapes read. */
function heroesString(literal: string): string {
  let out = '';
  for (let i = 1; i < literal.length - 1; i += 1) {
    if (literal[i] !== '\\') {
      out += literal[i];
      continue;
    }
    const next = literal[i + 1];
    if (next === 'u') {
      const close = literal.indexOf('}', i);
      out += String.fromCodePoint(parseInt(literal.slice(i + 3, close), 16));
      i = close;
      continue;
    }
    out += next === 'n' ? '\n' : next === 't' ? '\t' : next === 'r' ? '\r' : next;
    i += 1;
  }
  return out;
}

/** The string literals of a `[...]` list, in order. */
function stringList(list: string): string[] {
  const out: string[] = [];
  let i = 0;
  while (i < list.length) {
    if (list[i] !== '"') {
      i += 1;
      continue;
    }
    let end = i + 1;
    while (end < list.length && list[end] !== '"') end += list[end] === '\\' ? 2 : 1;
    out.push(heroesString(list.slice(i, end + 1)));
    i = end + 1;
  }
  return out;
}

interface Line {
  indent: number;
  text: string;
  inner: Line[];
}

/**
 * `literal_forms` read as the program it is: its `for` loops over lists of
 * strings and its `push` of each `Form`, run here. Any other line refuses the
 * build by name, since a reader that skipped a line it did not know would read
 * a table with forms missing and report every one of them painted right.
 */
function literalForms(): Form[] {
  const text = readText(SPEC_SUITE);
  const head = text.indexOf('\nfunction literal_forms() -> [Form]\n');
  if (head < 0) throw new Error(`${SPEC_SUITE}: no \`function literal_forms() -> [Form]\` to read the forms from.`);
  const body: string[] = [];
  for (const line of text.slice(head + 1).split('\n').slice(1)) {
    if (/^\S/.test(line)) break;
    body.push(line);
  }

  const root: Line[] = [];
  const open: Line[] = [];
  for (const raw of body) {
    const trimmed = raw.trim();
    if (trimmed === '' || trimmed.startsWith('#')) continue;
    const line: Line = { indent: raw.length - raw.trimStart().length, text: trimmed, inner: [] };
    while (open.length > 0 && open[open.length - 1].indent >= line.indent) open.pop();
    (open.length === 0 ? root : open[open.length - 1].inner).push(line);
    open.push(line);
  }

  const forms: Form[] = [];
  const value = (expr: string, env: Map<string, string>): string => {
    if (expr.startsWith('"')) return heroesString(expr);
    const bound = env.get(expr);
    if (bound === undefined) throw new Error(`${SPEC_SUITE}: literal_forms names \`${expr}\`, which no loop around it binds.`);
    return bound;
  };
  const run = (lines: Line[], env: Map<string, string>) => {
    for (const line of lines) {
      if (line.text === 'out: [Form] @= []' || line.text === 'return out') continue;
      const loop = /^for (\w+) in (\[.*\])$/.exec(line.text);
      if (loop !== null) {
        for (const item of stringList(loop[2])) run(line.inner, new Map([...env, [loop[1], item]]));
        continue;
      }
      const push = /^out @ out\.push\(Form\(rule: (.+?), written: (.+?), lexed: "(\w+)"\)\)$/.exec(line.text);
      if (push !== null) {
        forms.push({ rule: value(push[1], env), written: value(push[2], env), lexed: push[3] });
        continue;
      }
      throw new Error(`${SPEC_SUITE}: literal_forms holds a line this reader cannot run, so it cannot know the table:\n    ${line.text}`);
    }
  };
  run(root, new Map());

  if (forms.length < FORMS_FLOOR) {
    throw new Error(`${SPEC_SUITE}: read ${forms.length} literal forms, and the floor is ${FORMS_FLOOR}.`);
  }
  return forms;
}

/** The program the suite writes for a form (`form_program`), and where the form stands in it. */
function formProgram(form: Form): { program: string; at: number } {
  if (form.rule === 'string' || form.rule === 'interpolated') {
    const head = `function main()\n    print(${form.rule === 'interpolated' ? 'f"' : '"'}a`;
    return { program: `${head}${form.written}z")\n`, at: head.length };
  }
  if (form.rule === 'comment' || form.rule === 'section') {
    const head = `${form.rule === 'section' ? '##' : '#'} a`;
    return { program: `${head}${form.written}b\nfunction main()\n    print(1)\n`, at: head.length };
  }
  const head = 'function main()\n    print(';
  return { program: `${head}${form.written})\n`, at: head.length };
}

/** The token the site paints at `at`, and where it starts. */
function paintedAt(tokens: Token[], at: number): { token: Token; start: number } {
  let start = 0;
  for (const token of tokens) {
    if (at < start + token.text.length) return { token, start };
    start += token.text.length;
  }
  throw new Error(`nothing is painted at offset ${at}`);
}

/** Whether the site refuses what stands at `at`, as a token or as a part of one. */
function refusedAt(token: Token, start: number, at: number): boolean {
  if (token.kind === 'x') return true;
  return (token.parts ?? []).some((part) => part.kind === 'x' && start + part.start <= at && at < start + part.end);
}

/** A form in a sentence, every character a line cannot show written by its code. */
function shown(text: string): string {
  return [...text].map((c) => (c.charCodeAt(0) < 32 || c.charCodeAt(0) === 127 ? `<U+${c.charCodeAt(0).toString(16).padStart(4, '0')}>` : c)).join('');
}

function formParting(form: Form, code: number): string {
  const { program, at } = formProgram(form);
  const { token, start } = paintedAt(tokenize(program), at);
  const literal = form.rule === 'number' ? 'n' : 's';
  const asLiteral = start === at && token.text === form.written && token.kind === literal;
  const asError = refusedAt(token, start, at);
  const what = `\`${shown(form.written)}\` (${form.rule}, ${form.lexed} in ${SPEC_SUITE})`;
  const expected = form.lexed === 'refused' ? 1 : 0;
  if (code !== expected) return `${what}: the lexer exits ${code}`;
  if (form.lexed === 'refused' && !asError) return `${what}: the lexer refuses it, and the site paints ${said(token)}`;
  if (form.lexed === 'plain' && asError) return `${what}: the lexer reads it, and the site paints a refusal in it`;
  if (form.lexed === 'legal' && !asLiteral) return `${what}: the lexer reads one literal, and the site paints ${said(token)}`;
  if (form.lexed === 'split' && (asLiteral || asError)) return `${what}: the lexer reads several tokens and refuses none, and the site paints ${said(token)}`;
  return '';
}

/* -- The escapes ----------------------------------------------------------- */

interface Escape {
  written: string;
  interpolated: boolean;
}

/** Every escape asked of the lexer, generated rather than listed. */
function escapes(): Escape[] {
  const out: Escape[] = [];
  for (let c = 0x20; c < 0x7f; c += 1) out.push({ written: `\\${String.fromCharCode(c)}`, interpolated: false });
  const codes = new Set<number>();
  for (let c = 0; c <= 0xa0; c += 1) codes.add(c);
  for (const [first, last] of bidiControls()) for (const c of [first - 1, first, last, last + 1]) codes.add(c);
  for (const c of [0x2027, 0x2028, 0x2029, 0x202a, 0xd7ff, 0xd800, 0xdfff, 0xe000, 0xfffd, 0x10ffff, 0x110000]) codes.add(c);
  for (const c of [...codes].sort((a, b) => a - b)) out.push({ written: `\\u{${c.toString(16)}}`, interpolated: false });
  for (const written of ['\\u{1B}', '\\u{01b}', '\\u{}', '\\u{1b', '\\u1b']) out.push({ written, interpolated: false });
  for (const written of ['{{', '}}', '}']) out.push({ written, interpolated: true });
  return out;
}

function escapeParting(escape: Escape, code: number, program: string, at: number): string {
  const { token, start } = paintedAt(tokenize(program), at);
  const part = (token.parts ?? []).find((p) => start + p.start <= at && at < start + p.end);
  const what = `\`${escape.written}\` in ${escape.interpolated ? 'an `f` literal' : 'a string'}`;
  if (code === 0 && (part === undefined || part.kind !== 'e' || start + part.start !== at || start + part.end !== at + escape.written.length)) {
    return `${what}: the lexer reads it, and the site paints ${part === undefined ? 'no escape' : `${part.kind} over \`${token.text.slice(part.start, part.end)}\``}`;
  }
  if (code !== 0 && (part === undefined || part.kind !== 'x')) {
    return `${what}: the lexer refuses it, and the site paints ${part === undefined ? 'nothing' : `an escape over \`${token.text.slice(part.start, part.end)}\``}`;
  }
  return '';
}

/* -- The check ------------------------------------------------------------- */

/** Ask all three questions; throws with every disagreement, and says what was held. */
export function assertColouringLexed(): string {
  const heroes = compiler();
  const problems: string[] = [];

  const programs = walk('examples').filter((path) => path.endsWith('.hero'));
  if (programs.length < PROGRAMS_FLOOR) throw new Error(`examples/: ${programs.length} programs, and the floor is ${PROGRAMS_FLOOR}.`);
  let tokens = 0;
  for (const path of programs) {
    const asked = ask(heroes, ['lex', path, '--dump-tokens', '--json']);
    if (asked.code !== 0) {
      problems.push(`${path}: the lexer exits ${asked.code}, so the site shows a program the compiler refuses`);
      continue;
    }
    const found = partings(path, readText(path), JSON.parse(asked.out) as LexToken[]);
    problems.push(...found.problems);
    tokens += found.compared;
  }

  const scratch = absolute(SCRATCH);
  mkdirSync(dirname(scratch), { recursive: true });
  const forms = literalForms();
  const asked = escapes();
  try {
    for (const form of forms) {
      writeFileSync(scratch, formProgram(form).program);
      const parting = formParting(form, ask(heroes, ['lex', scratch]).code);
      if (parting !== '') problems.push(parting);
    }
    for (const escape of asked) {
      const head = `function main()\n    print(${escape.interpolated ? 'f"' : '"'}a`;
      const program = `${head}${escape.written}z")\n`;
      writeFileSync(scratch, program);
      const parting = escapeParting(escape, ask(heroes, ['lex', scratch]).code, program, head.length);
      if (parting !== '') problems.push(parting);
    }
  } finally {
    if (existsSync(scratch)) unlinkSync(scratch);
  }

  if (problems.length > 0) {
    throw new Error(
      `${problems.length} place(s) where the site's colouring and the compiler's lexer part:\n    ` +
        problems.join('\n    ') +
        `\n  The lexer is the authority (CLAUDE.md § 12). Teach src/lib/highlight.ts what it reads;\n` +
        `  where the site reads a rule from the editor's grammar, the grammar is held to the lexer\n` +
        `  by the \`spec\` suite too, and both are wrong together.\n`
    );
  }
  return `lexed: ${programs.length} programs, ${tokens} tokens as the lexer reads them; ${forms.length} literal forms and ${asked.length} escapes painted as it rules`;
}
