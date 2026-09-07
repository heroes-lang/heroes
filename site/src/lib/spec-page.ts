/**
 * The specification, on the site, cut from the file at build time.
 *
 * This page exists because of a finding rather than a plan. The site's central
 * claim is that the whole language fits in a prompt with room left for your
 * program, and the artifact that delivers that claim, `spec/heroes-spec.md`,
 * was the one thing not on the site: its only two links pointed into a private
 * repository and answered 404. A visitor could read thirteen chapters ABOUT the
 * language and never reach the language.
 *
 * It is shown VERBATIM, in one block, rather than rendered as prose, and that
 * is the point rather than a shortcut. What a reader wants from this page is
 * the text they would paste into a prompt, byte for byte, so a rendering that
 * dropped a pipe from a table or turned a backtick into a typographic quote
 * would give them something that is no longer the specification. The build cuts
 * it from the file the way the examples pages cut their code, so the page
 * cannot drift from what the compiler is measured against.
 *
 * The counts are computed here and not typed, for the same reason (site/CLAUDE.md
 * § A number on the page is generated, or it is a threshold). The token count is
 * NOT among them: only `heroes measure` can say it, the site build must not
 * depend on the compiler, and the number the reader actually needs is the
 * ceiling, which is fixed and asserted by a suite on every commit.
 */

import { readText, countLines, isFile } from './repo.ts';
import { t, number, type Lang } from './i18n.ts';

const SPEC = 'spec/heroes-spec.md';
const CEILING = 4096;

export interface SpecPage {
  url: string;
  title: string;
  description: string;
  html: string;
}

function escape(text: string): string {
  return text.replace(/[&<>]/g, (ch) => (ch === '&' ? '&amp;' : ch === '<' ? '&lt;' : '&gt;'));
}

/** The specification's own first line, which says what it is. */
function firstLine(text: string): string {
  for (const line of text.split('\n')) {
    const clean = line.replace(/^#+\s*/, '').trim();
    if (clean.length > 0) return clean;
  }
  throw new Error(`${SPEC} is empty.`);
}

export function renderSpecPage(lang: Lang): SpecPage {
  const source = readText(SPEC);
  const lines = countLines(source);
  const bytes = Buffer.byteLength(source, 'utf8');

  const intro = lang === 'en' ? 'site/src/html/spec.html' : 'site/src/html/it/spec.html';
  if (!isFile(intro)) {
    throw new Error(`${intro} is missing: it is the specification page's own prose.`);
  }

  const facts =
    lang === 'en'
      ? `<ul class="stats">
    <li><span class="n">${number(lines, lang)}</span><span class="what">lines, the whole language</span></li>
    <li><span class="n">${CEILING.toLocaleString('en-US')}</span><span class="what">tokens: the ceiling it fits under</span></li>
    <li><span class="n">${number(Math.round(bytes / 1024), lang)}</span><span class="what">kilobytes of text</span></li>
  </ul>`
      : `<ul class="stats">
    <li><span class="n">${number(lines, lang)}</span><span class="what">righe, tutto il linguaggio</span></li>
    <li><span class="n">${number(CEILING, lang)}</span><span class="what">token: il tetto sotto cui sta</span></li>
    <li><span class="n">${number(Math.round(bytes / 1024), lang)}</span><span class="what">kilobyte di testo</span></li>
  </ul>`;

  const caption =
    lang === 'en'
      ? `${SPEC}, ${number(lines, lang)} ${t('lines', lang)}, shown exactly as the file holds it`
      : `${SPEC}, ${number(lines, lang)} ${t('lines', lang)}, mostrato esattamente come sta nel file`;

  const html = [
    readText(intro).trim(),
    facts,
    `  <figure class="example spec" data-src="${SPEC}">\n<pre><code>${escape(source.replace(/\n$/, ''))}</code></pre>\n    <figcaption>${caption}</figcaption>\n  </figure>`,
    lang === 'en'
      ? `  <p class="next">\n    Next: <b><a href="/docs/">the same language explained one idea at a time</a></b>.\n    <a href="/examples/">Every program</a>.\n  </p>`
      : `  <p class="next">\n    Poi: <b><a href="/it/docs/">lo stesso linguaggio spiegato un'idea alla volta</a></b>.\n    <a href="/it/examples/">Tutti i programmi</a>.\n  </p>`,
    readText(lang === 'en' ? 'site/src/html/examples/_footer.html' : 'site/src/html/it/examples/_footer.html').trim().replace(
      /^/,
      '  <footer>\n'
    ) + '\n  </footer>',
  ].join('\n\n');

  return {
    url: lang === 'it' ? '/it/spec/' : '/spec/',
    title:
      lang === 'it'
        ? 'La specifica: tutto il linguaggio, in una pagina'
        : 'The specification: the whole language, on one page',
    description:
      lang === 'it'
        ? `${firstLine(source)} Sotto un tetto di ${CEILING} token, perché la specifica è il prompt.`
        : `${firstLine(source)} Under a ${CEILING}-token ceiling, because the specification is the prompt.`,
    html: `\n${html}\n`,
  };
}
