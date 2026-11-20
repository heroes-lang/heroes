/**
 * Rebel's page, `/rebel/`, and its reference pages, `/rebel/<face>/`, in both
 * editions.
 *
 * The page's prose is a fragment like every hand-written page
 * (`site/src/html/rebel.html` and its Italian twin), put through `page()` so
 * its figures, claims and nod are checked as theirs are; where it says
 * `{{packages}}` the build writes the packages by layer, read from
 * `examples-rebel/README.md` § A1 (`reference.ts`), each name opening its
 * reference or marked owed. A count of packages or layers on the page is that
 * reading's, never typed (`site/CLAUDE.md` § A number on the page).
 *
 * A reference page is assembled here around `heroes doc`'s output, which it
 * shows verbatim and in English in both editions (decision 22): the trail, the
 * package's row of § A1 as its tagline, the command that printed it, and the
 * output, coloured and proved unchanged by `reference.ts`.
 */

import { page } from './figures.ts';
import { readText } from './repo.ts';
import { number, type Lang } from './i18n.ts';
import { withoutTags } from './tags.ts';
import { accepted, colouredReference, references, referenceUrl, rebelVersion, rows, type Reference, type Row } from './reference.ts';

export interface RebelPage {
  url: string;
  title: string;
  description: string;
  html: string;
}

function root(lang: Lang): string {
  return lang === 'it' ? '/it' : '';
}

function escape(text: string): string {
  return text.replace(/[&<>]/g, (ch) => (ch === '&' ? '&amp;' : ch === '<' ? '&lt;' : '&gt;'));
}

/** A cell of § A1 as HTML: escaped, its Markdown code spans made `<code>`. */
function cell(markdown: string): string {
  return escape(markdown).replace(/`([^`]+)`/g, '<code>$1</code>');
}

/** The same cell as plain text, for a page's description. */
function plainCell(markdown: string): string {
  return markdown.replace(/`/g, '');
}

/** A count up to twenty as the site writes one in prose, in words; above it, digits. */
const WORDS: Record<Lang, string[]> = {
  en: ['no', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten', 'eleven', 'twelve',
       'thirteen', 'fourteen', 'fifteen', 'sixteen', 'seventeen', 'eighteen', 'nineteen', 'twenty'],
  it: ['nessun', 'un', 'due', 'tre', 'quattro', 'cinque', 'sei', 'sette', 'otto', 'nove', 'dieci', 'undici', 'dodici',
       'tredici', 'quattordici', 'quindici', 'sedici', 'diciassette', 'diciotto', 'diciannove', 'venti'],
};

function counted(n: number, lang: Lang): string {
  return n < WORDS[lang].length ? WORDS[lang][n] : number(n, lang);
}

/** One face, as the list shows it: its reference, or what is owed. */
function faceItem(face: string, answer: Reference | undefined, lang: Lang): string {
  if (answer !== undefined && answer.text !== null) {
    return `<a href="${referenceUrl(face, lang)}"><code>${escape(face)}</code></a>`;
  }
  const n = answer?.refusals ?? 0;
  const owed =
    lang === 'en'
      ? `reference owed: <code>heroes doc</code> refuses ${n === 1 ? 'one comment' : `${counted(n, lang)} comments`}`
      : `riferimento in sospeso: <code>heroes doc</code> rifiuta ${n === 1 ? 'un commento' : `${counted(n, lang)} commenti`}`;
  return `<code>${escape(face)}</code> <span class="owed">(${owed})</span>`;
}

/** The packages by layer, as `{{packages}}` becomes. */
async function packagesList(lang: Lang): Promise<string> {
  const table = rows();
  const answers = new Map((await references()).map((one) => [one.face, one]));
  const layers = [...new Set(table.map((row) => row.layer))].sort((a, b) => a - b);
  const owed = [...answers.values()].filter((one) => one.text === null).length;
  const lead =
    lang === 'en'
      ? `  <p>\n    Rebel ${rebelVersion()} holds ${number(table.length, lang)} packages in ${counted(layers.length, lang)} layers. A package may use any package of a layer below its own and none above, so no two packages need each other. Each name opens the package&rsquo;s reference, which <code>heroes doc</code> prints from the package&rsquo;s own comments; it is in English in both editions, being the compiler&rsquo;s output.${
          owed > 0
            ? ` Where a package is marked owed, <code>heroes doc</code> still refuses some of its comments, and this site shows the compiler&rsquo;s words or none: its page arrives with the comments.`
            : ''
        }\n  </p>`
      : `  <p>\n    Rebel ${rebelVersion()} contiene ${number(table.length, lang)} pacchetti in ${counted(layers.length, lang)} livelli. Un pacchetto può usare qualsiasi pacchetto di un livello più basso del suo e nessuno di uno più alto, così due pacchetti non hanno mai bisogno l&rsquo;uno dell&rsquo;altro. Ogni nome porta al riferimento del pacchetto, che <code>heroes doc</code> stampa dai commenti del pacchetto stesso; è in inglese in tutte e due le edizioni, perché è l&rsquo;output del compilatore, e lo sono anche le descrizioni qui sotto, le parole del repository.${
          owed > 0
            ? ` Dove un pacchetto è segnato in sospeso, <code>heroes doc</code> rifiuta ancora alcuni dei suoi commenti, e questo sito mostra le parole del compilatore o nessuna: la sua pagina arriva con i commenti.`
            : ''
        }\n  </p>`;
  const sections = layers.map((layer) => {
    const items = table
      .filter((row) => row.layer === layer)
      .map((row) => {
        const names = row.faces.map((face) => faceItem(face, answers.get(face), lang)).join(', ');
        const marked = lang === 'it' ? ' lang="en"' : '';
        return `    <dt>${names}</dt>\n    <dd${marked}>${cell(row.holds)}</dd>`;
      })
      .join('\n');
    const heading = lang === 'en' ? `Layer ${layer}` : `Livello ${layer}`;
    return `  <h3 id="layer-${layer}">${heading}</h3>\n  <dl class="why packages">\n${items}\n  </dl>`;
  });
  return [lead, ...sections].join('\n\n');
}

/** Rebel's own page. */
export async function renderRebelPage(lang: Lang): Promise<RebelPage> {
  const fragment = lang === 'en' ? 'site/src/html/rebel.html' : 'site/src/html/it/rebel.html';
  const html = page(readText(fragment), fragment);
  if (!html.includes('{{packages}}')) {
    throw new Error(`${fragment}: the place for the packages, {{packages}}, is gone, so the page would list none.`);
  }
  return {
    url: `${root(lang)}/rebel/`,
    title: lang === 'en' ? 'Rebel: the library to call, or not' : 'Rebel: la libreria da chiamare, o no',
    description:
      lang === 'en'
        ? 'Rebel, the library written for Heroes: what it holds, layer by layer, how a program takes it, and the reference of every package.'
        : 'Rebel, la libreria scritta per Heroes: che cosa contiene, livello per livello, come la prende un programma, e il riferimento di ogni pacchetto.',
    html: `\n${html.replace('{{packages}}', await packagesList(lang))}\n`,
  };
}

/** The faces with a reference page, for the route. */
export async function referenceFaces(): Promise<string[]> {
  return (await accepted()).map((one) => one.face);
}

/** The row of § A1 a face stands in. */
function rowOf(face: string): Row {
  const found = rows().find((row) => row.faces.includes(face));
  if (found === undefined) throw new Error(`the face \`${face}\` stands in no row of examples-rebel/README.md § A1.`);
  return found;
}

/** One face's reference page. */
export async function renderReferencePage(face: string, lang: Lang): Promise<RebelPage> {
  const answer = (await references()).find((one) => one.face === face);
  if (answer === undefined || answer.text === null) {
    throw new Error(`\`${face}\` has no reference page: heroes doc did not accept it.`);
  }
  const row = rowOf(face);
  const coloured = colouredReference(answer);
  const lines = answer.text.replace(/\n$/, '').split('\n').length;
  const command = `heroes doc '&lt;rebel&gt;/${escape(face)}'`;
  const own = row.package === face;
  const tagline = own
    ? cell(row.holds)
    : lang === 'en'
      ? `A module of <code>${escape(row.package)}</code>: ${cell(row.holds)}`
      : `Un modulo di <code>${escape(row.package)}</code>: ${cell(row.holds)}`;
  const parts: string[] = [];
  parts.push(
    `  <p class="crumb"><a href="${root(lang)}/">Heroes</a> / <a href="${root(lang)}/rebel/">Rebel</a> / ${escape(face)}</p>`
  );
  parts.push(`  <h1 class="chapter"><code>&lt;rebel&gt;/${escape(face)}</code></h1>`);
  parts.push(`  <p class="tagline"${lang === 'it' ? ' lang="en"' : ''}>${tagline}</p>`);
  parts.push(
    lang === 'en'
      ? `  <p>\n    What <code>${command}</code> prints, byte for byte: each declaration of the package&rsquo;s face with its signature, the comment that documents it, the call a program writes and its examples, read from the package&rsquo;s own source. A program names it <code>use &lt;rebel&gt;/${escape(face)}</code>, once its catalogue lists it, as <a href="/docs/packages/">the chapter on packages</a> shows.\n  </p>`
      : `  <p>\n    Quello che stampa <code>${command}</code>, byte per byte: ogni dichiarazione della facciata del pacchetto con la sua firma, il commento che la documenta, la chiamata che un programma scrive e i suoi esempi, letti dal sorgente del pacchetto stesso. È in inglese, come i commenti da cui viene. Un programma lo nomina <code>use &lt;rebel&gt;/${escape(face)}</code>, una volta che il suo catalogo lo elenca, come mostra <a href="/it/docs/packages/">il capitolo sui pacchetti</a>.\n  </p>`
  );
  const unit = lang === 'en' ? (lines === 1 ? 'line' : 'lines') : lines === 1 ? 'riga' : 'righe';
  parts.push(
    `  <figure class="example spec reference"${lang === 'it' ? ' lang="en"' : ''}>\n<pre><code>${coloured}</code></pre>\n` +
      `    <figcaption>${command}, ${number(lines, lang)} ${unit}</figcaption>\n  </figure>`
  );
  parts.push(
    lang === 'en'
      ? `  <p class="next">\n    <a href="/rebel/#layer-${row.layer}">Every package of Rebel</a>, layer by layer.\n  </p>`
      : `  <p class="next">\n    <a href="/it/rebel/#layer-${row.layer}">Tutti i pacchetti di Rebel</a>, livello per livello.\n  </p>`
  );
  parts.push(
    lang === 'en'
      ? `  <footer>\n    <p>The block above is <code>heroes doc</code>&rsquo;s output for this face of Rebel ${rebelVersion()}, made when the site is built and checked to be the same bytes the command printed, so it cannot drift from the package&rsquo;s source.</p>\n  </footer>`
      : `  <footer>\n    <p>Il blocco qui sopra è l&rsquo;output di <code>heroes doc</code> per questa facciata di Rebel ${rebelVersion()}, prodotto quando il sito viene generato e verificato byte per byte su quello che il comando ha stampato, quindi non può scostarsi dal sorgente del pacchetto.</p>\n  </footer>`
  );
  const plain = withoutTags(tagline, `the reference of ${face}`).replace(/\s+/g, ' ').trim();
  const lead = lang === 'en' ? `The reference of Rebel's ${face}, as heroes doc prints it: ` : `Il riferimento di ${face} di Rebel, come lo stampa heroes doc: `;
  let description = lead + plainCell(plain);
  if (description.length > 160) description = `${description.slice(0, 157).replace(/\s+\S*$/, '')}…`;
  return {
    url: referenceUrl(face, lang),
    title: lang === 'en' ? `${face}: Rebel's reference` : `${face}: il riferimento di Rebel`,
    description,
    html: `\n${parts.join('\n\n')}\n`,
  };
}
