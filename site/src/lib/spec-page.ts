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
 * ceiling, which is asserted by a suite on every commit and read from that
 * suite here (`specCeiling` in `claims.ts`), since the home says it too.
 */

import { readText, countLines, isFile } from './repo.ts';
import { t, number, type Lang } from './i18n.ts';
import { ceilingK, checkClaims } from './claims.ts';
import { fillMeasured } from './figures.ts';
import { colourMarkdown, assertVerbatim } from './markdown.ts';

const SPEC = 'spec/heroes-spec.md';
// The ceiling as the pages say it, `6K`, the author's word (2026-09-09); the exact
// number is the suite's and `ceilingK` refuses one that is not a whole number of K.
const K = ceilingK();

export interface SpecPage {
  url: string;
  title: string;
  description: string;
  html: string;
}

/**
 * This page's own footer, and it needs one.
 *
 * It used to borrow the examples footer, which says every block is read from a
 * file in `examples/` and that the block under each command is a recorded
 * output. Neither is true here: there is no command, no recorded output, and
 * the one block comes from `spec/heroes-spec.md`. A false paragraph on the page
 * whose argument is byte fidelity is the worst place to put one.
 *
 * Only this page's own paragraph is here. The byline and the copyright line
 * are `SiteFooter`'s, on this page as on every other.
 */
function footer(lang: Lang): string {
  const first =
    lang === 'en'
      ? `<p>The block above is read from <code>${SPEC}</code> when the site is
    built, so it cannot drift from the file the compiler is measured against. A
    test asserts the token ceiling on every commit.</p>`
      : `<p>Il blocco qui sopra viene letto da <code>${SPEC}</code> quando il
    sito viene generato, quindi non può scostarsi dal file su cui il
    compilatore viene misurato. Un test verifica il tetto dei token a ogni
    commit.</p>`;
  return `  <footer>\n    ${first}\n  </footer>`;
}

/**
 * The one script this site asks for, and the rule it lives under.
 *
 * site/README.md: JavaScript is never the only way. Without it this page still
 * offers the file as a link, as a `curl` line, and as a block that one click
 * selects whole. With it, a button copies the specification to the clipboard,
 * byte for byte: the block was written without the file's final newline so the
 * frame does not end on a blank line, and the newline goes back on here, which
 * is what `cmp` against `spec/heroes-spec.md` checks by hand at every change.
 *
 * What it does not do, and `src/lib/scripts.ts` refuses at build end if it ever
 * did: set a cookie, touch any storage, fetch anything, send anything. The one
 * piece of state is a two-second timer that puts the label back.
 */
function copyScript(): string {
  return `  <script data-enhances="spec-copy">
(function () {
  document.documentElement.classList.add('js');
  var figure = document.querySelector('figure.example.spec');
  if (!figure) return;
  var code = figure.querySelector('pre code');
  var button = figure.querySelector('button.copy');
  if (!code || !button) return;
  var label = button.textContent;
  var timer = 0;
  button.hidden = false;
  function say(text) {
    button.textContent = text;
    clearTimeout(timer);
    timer = setTimeout(function () { button.textContent = label; }, 2000);
  }
  function select() {
    var selection = window.getSelection();
    var range = document.createRange();
    range.selectNodeContents(code);
    selection.removeAllRanges();
    selection.addRange(range);
    say(button.getAttribute('data-selected'));
  }
  button.addEventListener('click', function () {
    var text = code.textContent + '\\n';
    if (navigator.clipboard && navigator.clipboard.writeText) {
      navigator.clipboard.writeText(text).then(
        function () { say(button.getAttribute('data-copied')); },
        select
      );
    } else {
      select();
    }
  });
})();
  </script>`;
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
    <li><span class="n">${K}</span><span class="what">tokens: the specification&rsquo;s ceiling, held by a test</span></li>
    <li><span class="n">${number(Math.round(bytes / 1024), lang)}</span><span class="what">kilobytes of text</span></li>
  </ul>`
      : `<ul class="stats">
    <li><span class="n">${number(lines, lang)}</span><span class="what">righe, tutto il linguaggio</span></li>
    <li><span class="n">${K}</span><span class="what">token: il tetto della specifica, tenuto da un test</span></li>
    <li><span class="n">${number(Math.round(bytes / 1024), lang)}</span><span class="what">kilobyte di testo</span></li>
  </ul>`;

  const caption =
    lang === 'en'
      ? `${SPEC}, ${number(lines, lang)} ${t('lines', lang)}, identical to the source file`
      : `${SPEC}, ${number(lines, lang)} ${t('lines', lang)}, identico al file originale`;

  // The button is rendered `hidden` and the script below un-hides it, so a
  // visitor without scripts never meets a control that does nothing; for them
  // `style.css` makes one click select the block whole (`html:not(.js)`), and
  // the link and the `curl` line in the lede are the same bytes.
  const labels =
    lang === 'en'
      ? { copy: 'Copy the specification', copied: 'Copied', selected: 'Selected: press Ctrl+C or Cmd+C' }
      : { copy: 'Copia la specifica', copied: 'Copiata', selected: 'Selezionata: premi Ctrl+C o Cmd+C' };
  const button = `<button type="button" class="copy" hidden data-copied="${labels.copied}" data-selected="${labels.selected}">${labels.copy}</button>`;

  // The document, coloured and then proved unchanged. The block is the file, and
  // a reader copies it into a prompt, so the colouring adds `span` elements and
  // moves no byte: `assertVerbatim` takes the text back out of the HTML and
  // compares it with what went in, on every build.
  const body = source.replace(/\n$/, '');
  const coloured = colourMarkdown(body);
  assertVerbatim(body, coloured, SPEC);

  const html = [
    readText(intro).trim(),
    facts,
    `  <figure class="example spec" data-src="${SPEC}">\n<pre><code>${coloured}</code></pre>\n    ${button}\n    <figcaption>${caption}</figcaption>\n  </figure>`,
    copyScript(),
    lang === 'en'
      ? `  <p class="next">\n    Next: <b><a href="/docs/">the same language explained one idea at a time</a></b>.\n    <a href="/examples/">Every program</a>.\n  </p>`
      // `&rsquo;` and not a straight quote: the Italian edition's typography
      // rule reaches the HTML fragments and nothing reads this directory, so a
      // generated string is where the last one hid.
      : `  <p class="next">\n    Poi: <b><a href="/it/docs/">lo stesso linguaggio spiegato un&rsquo;idea alla volta</a></b>.\n    <a href="/it/examples/">Tutti i programmi</a>.\n  </p>`,
    footer(lang),
  ].join('\n\n');
  // This page is assembled here and not by the fragment pipeline, so the claims
  // table's two spec rows are checked on the assembled page, or they would be
  // rows nobody visits — which `assertEveryClaimVisited` refuses at build end.
  checkClaims(html, intro);

  return {
    url: lang === 'it' ? '/it/spec/' : '/spec/',
    title:
      lang === 'it'
        ? 'La specifica: tutto il linguaggio, in una pagina'
        : 'The specification: the whole language, on one page',
    description:
      lang === 'it'
        ? `Tutto il linguaggio Heroes, in un file da inserire nel prompt: meno di ${K} token per descrivere la sintassi e le regole.`
        : `The whole Heroes language, in one file for a model’s prompt: under ${K} tokens describing its syntax and rules.`,
    // The measured count and its model are filled last, after the claims were
    // checked on the prose as written, exactly as the fragment pipeline does.
    html: `\n${fillMeasured(html)}\n`,
  };
}
