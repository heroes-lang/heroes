/**
 * The check the site has promised since it existed and never had.
 *
 * `site/README.md` states the rule for every code block on this site: it is a
 * slice of a file in `examples/`, copied from the file and never retyped, with
 * `data-src` and an optional `data-lines` as the machine-readable half. Then it
 * says, in its own words, **"Nothing enforces it"**, and adds that the paragraph
 * once credited a `check.py` that does not exist, which is the more expensive
 * half of the failure, because a sentence naming an instrument stops anybody
 * from writing it.
 *
 * This is that instrument, and it lives in the build rather than in the harness
 * for one reason: the build is what the deploy runs, so a drifted figure is a
 * red workflow and never a live page. `docs/work/SCHEDULED.md` planned it as a
 * row in the net instead and said "not a build of the site, which the one
 * command rule keeps out of `heroes`". That rule is about the `heroes` binary's
 * surface, and this is not a `heroes` subcommand: it is the site's own build
 * checking the site's own claim. Both places could hold it; this is the one that
 * can stop a bad page reaching a reader.
 *
 * The generated examples pages do not need it, because they cut their blocks
 * from the file at build time and cannot drift. What needs it is the 160 figures
 * written by hand across the 46 pages that came before, and every one written
 * from here on.
 */

import { readText, isFile } from './repo.ts';
import { plain } from './highlight.ts';
import { version } from './tables.ts';
import { checkClaims } from './claims.ts';

/** One figure, as the check reads it. */
interface Figure {
  src: string;
  from: number | null;
  to: number | null;
  body: string;
  caption: string | null;
}

const FIGURE =
  /<figure class="example[^"]*"([^>]*)>\s*<pre><code>([\s\S]*?)<\/code><\/pre>\s*(?:<figcaption>([\s\S]*?)<\/figcaption>)?/g;

function figuresIn(html: string): Figure[] {
  const out: Figure[] = [];
  for (const match of html.matchAll(FIGURE)) {
    const attributes = match[1];
    const src = /data-src="([^"]+)"/.exec(attributes)?.[1];
    if (src === undefined) continue;
    const range = /data-lines="(\d+)-(\d+)"/.exec(attributes);
    out.push({
      src,
      from: range === null ? null : Number(range[1]),
      to: range === null ? null : Number(range[2]),
      body: match[2],
      caption: match[3] ?? null,
    });
  }
  return out;
}

/**
 * Check every figure on one page against the file it names.
 *
 * Throws with the page, the figure's position, the file, the range and the
 * first line that differs, because a message saying only "a figure drifted"
 * sends somebody hunting through 160 of them.
 */
export function checkFigures(html: string, pagePath: string): void {
  const problems: string[] = [];
  const figures = figuresIn(html);

  for (const [index, figure] of figures.entries()) {
    const where = `${pagePath}: figure ${index + 1} (${figure.src}${
      figure.from === null ? '' : ` lines ${figure.from}-${figure.to}`
    })`;

    if (!isFile(figure.src)) {
      problems.push(`${where}: that file does not exist`);
      continue;
    }

    const source = readText(figure.src);
    const lines = source.split('\n');

    if (figure.from !== null) {
      if (figure.from < 1 || figure.to! > lines.length) {
        problems.push(`${where}: the slice is outside the file, which has ${lines.length} lines`);
        continue;
      }
    }

    const want =
      figure.from === null
        ? source.replace(/\n$/, '')
        : lines.slice(figure.from - 1, figure.to!).join('\n');
    const got = plain(figure.body);

    if (got !== want) {
      const a = got.split('\n');
      const b = want.split('\n');
      let at = 0;
      while (at < a.length && at < b.length && a[at] === b[at]) at += 1;
      problems.push(
        `${where}: first difference at line ${at + 1}\n` +
          `      page: ${JSON.stringify(a[at] ?? null)}\n` +
          `      file: ${JSON.stringify(b[at] ?? null)}`
      );
      continue;
    }

    // The caption is the same promise made to a reader, so it may not disagree
    // with the attribute. `site/README.md` says so and nothing checked it.
    if (figure.caption !== null) {
      const caption = figure.caption.replace(/\s+/g, ' ').trim();
      if (!caption.includes(figure.src)) {
        problems.push(`${where}: the caption does not name that file: ${JSON.stringify(caption)}`);
      } else if (figure.from !== null) {
        const numbers = caption.match(/\d+/g) ?? [];
        if (!numbers.includes(String(figure.from)) || !numbers.includes(String(figure.to))) {
          problems.push(
            `${where}: the caption does not carry that range: ${JSON.stringify(caption)}`
          );
        }
      }
    }
  }

  if (problems.length > 0) {
    throw new Error(
      `${problems.length} code block(s) no longer match the file they are cut from.\n    ` +
        problems.join('\n    ') +
        `\n  Every block on this site is a slice of a repository file (site/README.md).\n` +
        `  Re-cut the block from the file rather than editing the page to agree with it.\n`
    );
  }
}

/**
 * What a page wrapper passes to `set:html`, checked on the way through.
 *
 * One line per wrapper, so the 46 hand-written pages gain the check without
 * changing shape.
 */
export function page(html: string, pagePath: string): string {
  checkFigures(html, pagePath);
  // The prose's own numbers, against the tree. The code blocks were the first
  // thing this function checked and the prose around them was the last thing
  // anybody did, which is where four false claims in one day came from.
  checkClaims(html, pagePath);
  return fillVersion(html, pagePath);
}

/**
 * `{{version}}` in a fragment becomes the version the compiler prints.
 *
 * One placeholder rather than a second templating idea: the fragments are plain
 * HTML files anybody can edit, and this is the only value in them that a
 * release moves. A fragment that carries the placeholder and gets no
 * substitution would ship the braces at a reader, so the substitution is
 * unconditional and the absence of the placeholder is simply nothing to do.
 */
export function fillVersion(html: string, pagePath: string): string {
  if (!html.includes('{{version}}')) return html;
  const value = version();
  if (!/^[0-9]+\.[0-9]+\.[0-9]+$/.test(value)) {
    throw new Error(`${pagePath}: the version read from the compiler is not X.Y.Z: ${value}`);
  }
  return html.replaceAll('{{version}}', value);
}
