/**
 * Colouring for the one markdown document this site serves verbatim: the
 * specification.
 *
 * The page's whole argument is byte fidelity — a reader copies the block into a
 * prompt, and `cmp` against `spec/heroes-spec.md` is a check somebody runs. So
 * this adds `<span>` elements and nothing else: no character of the document is
 * inserted, moved or dropped, and `plainOf()` below is the proof, asserted at
 * build time over the real file. A colouring that could not prove that would be
 * a decoration on the page whose subject is exactness.
 *
 * WHAT IS COLOURED, and the list is short because the accents are three
 * (site/README.md § Style guide): a heading, a code span or fence, a table's
 * pipes and rules, and bold. Everything else is prose and stays the body
 * colour. The classes are the stylesheet's own — `.k` for a heading, because a
 * heading is the document's structure the way a keyword is a program's, `.s`
 * for code, which is the gold the Heroes blocks already give a literal, `.c`
 * for the dim furniture of a table, and `.b` for bold, which this file adds.
 *
 * WHY NOT A LIBRARY. `site/README.md` allows no external asset, and a markdown
 * renderer would give HTML rather than a coloured copy of the source: the page
 * shows the source, pipes and backticks and all, because that is what a reader
 * pastes.
 */

const ESCAPES: Record<string, string> = { '&': '&amp;', '<': '&lt;', '>': '&gt;' };

function esc(text: string): string {
  return text.replace(/[&<>]/g, (ch) => ESCAPES[ch]);
}

/** The document's own text back out of coloured HTML, for the round-trip check. */
export function plainOf(html: string): string {
  return html
    .replace(/<\/?span[^>]*>/g, '')
    .replace(/&lt;/g, '<')
    .replace(/&gt;/g, '>')
    .replace(/&amp;/g, '&');
}

function span(cls: string, text: string): string {
  return `<span class="${cls}">${esc(text)}</span>`;
}

/**
 * One line of markdown, coloured. Inside a fenced block only the code accent
 * applies, because a `#` there is a Heroes comment and a `|` is a bitwise or.
 */
function colourLine(line: string, inFence: boolean): string {
  if (inFence) return span('s', line);
  if (/^#{1,6} /.test(line)) return span('k', line);
  if (/^\s*\|/.test(line)) return colourTableRow(line);
  return colourInline(line);
}

/** A table row: the pipes and any all-dashes cell are furniture. */
function colourTableRow(line: string): string {
  if (/^\s*\|[\s|:-]*$/.test(line)) return span('c', line);
  let out = '';
  let run = '';
  for (const ch of line) {
    if (ch === '|') {
      if (run) out += colourInline(run);
      run = '';
      out += span('c', '|');
    } else {
      run += ch;
    }
  }
  if (run) out += colourInline(run);
  return out;
}

/** Code spans and bold inside a line of prose. */
function colourInline(text: string): string {
  let out = '';
  let i = 0;
  while (i < text.length) {
    if (text[i] === '`') {
      const close = text.indexOf('`', i + 1);
      if (close > i) {
        out += span('s', text.slice(i, close + 1));
        i = close + 1;
        continue;
      }
    }
    if (text.startsWith('**', i)) {
      const close = text.indexOf('**', i + 2);
      if (close > i) {
        out += span('b', text.slice(i, close + 2));
        i = close + 2;
        continue;
      }
    }
    out += esc(text[i]);
    i += 1;
  }
  return out;
}

/**
 * The whole document, coloured line by line. A fence line belongs to the fence,
 * so that ``` is gold like the code it opens.
 */
export function colourMarkdown(source: string): string {
  const lines = source.split('\n');
  let inFence = false;
  const out: string[] = [];
  for (const line of lines) {
    if (line.startsWith('```')) {
      inFence = !inFence;
      out.push(span('s', line));
      continue;
    }
    out.push(colourLine(line, inFence));
  }
  return out.join('\n');
}

/**
 * Throws unless the colouring is a pure addition: the document back out of the
 * HTML must be the document that went in, byte for byte. Called at build time
 * on the real specification, so the page cannot ship a lie about fidelity.
 */
export function assertVerbatim(source: string, html: string, label: string): void {
  const back = plainOf(html);
  if (back === source) return;
  let at = 0;
  while (at < back.length && at < source.length && back[at] === source[at]) at += 1;
  throw new Error(
    `${label}: the colouring changed the text at byte ${at}.\n` +
      `  source: ${JSON.stringify(source.slice(Math.max(0, at - 30), at + 30))}\n` +
      `  output: ${JSON.stringify(back.slice(Math.max(0, at - 30), at + 30))}\n` +
      `  the specification page's argument is that the block is the file, so a\n` +
      `  colouring that moves a byte is worse than no colouring.`
  );
}
