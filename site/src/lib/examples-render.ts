/**
 * The examples pages, as HTML, in either language.
 *
 * One reading of the corpus produces both editions: the code blocks are the
 * same strings on the English and the Italian page, and only the prose and the
 * furniture differ. That is the guarantee `site/README.md` § The Italian
 * edition asks for, made structural rather than remembered, and the build
 * asserts it by comparing the two renders.
 *
 * Every class used here already exists in `site/public/style.css`, because the
 * pages are built out of the shapes the hand-written pages use: `.crumb`,
 * `h1.chapter`, `.tagline`, `figure.example` with its `figcaption`, `.demo`
 * with its `.bar` and `.under`, `.cards`, `.next` with its `.back`. What the
 * stylesheet gains for this section is the badge row, the closed module, the
 * card's own meta line and the fingerprint line.
 *
 * The output pane is a pasted terminal transcript rather than a Run button,
 * which is what Zig's samples and Gleam's homepage do and what a static site
 * can prove: the text is `main.expected`, which the corpus suite asserts byte
 * for byte in three configurations on every push. The harness's ending marker
 * never reaches the page: `!exit: 1` becomes an exit status line.
 */

import { examples, exampleBySlug, totals, outputLines, type Example, type SourceFile } from './examples-model.ts';
import { descriptionOf, shelfOf } from './descriptions.ts';
import { groups } from '../data/examples.ts';
import { readingOrder } from '../data/examples.ts';
import { t, number, type Lang } from './i18n.ts';
import { readText, isFile } from './repo.ts';

export interface RenderedPage {
  url: string;
  title: string;
  description: string;
  html: string;
}

const HOME = 'site/src/html/examples';

function escape(text: string): string {
  return text.replace(/[&<>]/g, (ch) => (ch === '&' ? '&amp;' : ch === '<' ? '&lt;' : '&gt;'));
}

function root(lang: Lang): string {
  return lang === 'it' ? '/it' : '';
}

/**
 * A shared fragment: the footer every page carries, and the two sentences that
 * say why a page shows no output.
 *
 * These are prose, so they live beside the pages rather than in `i18n.ts`. One
 * copy per edition, so 68 pages cannot disagree about them.
 */
function shared(name: string, lang: Lang): string {
  const path = lang === 'en' ? `${HOME}/_${name}.html` : `site/src/html/it/examples/_${name}.html`;
  if (!isFile(path)) {
    throw new Error(`${path} is missing: it is a fragment every examples page may need.`);
  }
  return readText(path).trim();
}

function footer(lang: Lang): string {
  return shared('footer', lang);
}

/** `671 lines · 3 modules · reads a file`, and only what is true. */
function badges(example: Example, lang: Lang): string {
  const items: string[] = [];
  items.push(`${number(example.lines, lang)} ${t('lines', lang)}`);
  if (example.modules.length > 1) items.push(`${example.modules.length} ${t('modules', lang)}`);
  if (example.externs.length > 0) {
    const headers = [...new Set(example.externs)].filter((header) => header !== 'hero_os.h');
    if (headers.length > 0) items.push(`${t('bindsC', lang)}: <code>${escape(headers.join(' '))}</code>`);
  }
  if (example.threads) items.push(t('threads', lang));
  if (example.usesArgs) items.push(t('commandLine', lang));
  if (example.readsFile) items.push(t('readsFile', lang));
  if (example.writesFile) items.push(t('writesFile', lang));
  return `  <ul class="badges">\n${items.map((one) => `    <li>${one}</li>`).join('\n')}\n  </ul>`;
}

/** One `.hero` file as a figure, carrying the `data-src` the drift check reads. */
function figure(file: SourceFile, lang: Lang): string {
  const unit = file.lines === 1 ? t('line', lang) : t('lines', lang);
  return (
    `  <figure class="example" data-src="${file.path}">\n` +
    `<pre><code>${file.html.replace(/\n$/, '')}</code></pre>\n` +
    `    <figcaption>${escape(file.path)}, ${number(file.lines, lang)} ${unit}</figcaption>\n` +
    `  </figure>`
  );
}

/** A data file the program reads: escaped, uncoloured, and named. */
function inputFigure(file: SourceFile, lang: Lang): string {
  const unit = file.lines === 1 ? t('line', lang) : t('lines', lang);
  return (
    `  <figure class="example data" data-src="${file.path}">\n` +
    `<pre><code>${file.html.replace(/\n$/, '')}</code></pre>\n` +
    `    <figcaption>${escape(file.path)}, ${number(file.lines, lang)} ${unit}</figcaption>\n` +
    `  </figure>`
  );
}

/** The terminal block: the command, the real output, and how it ended. */
function runBlock(example: Example, lang: Lang): string {
  if (example.expectation === null) {
    // The command still shows, because it is what a reader would type, and the
    // sentence under it says why there is nothing below rather than leaving an
    // empty pane that reads as a program printing nothing.
    const why = shared(`no-output-${example.noOutputBecause}`, lang);
    return (
      `  <h2>${t('runIt', lang)}</h2>\n` +
      `  <div class="demo run">\n` +
      `    <div class="bar">${escape(example.command)}</div>\n` +
      `    <p class="under">${why}</p>\n` +
      `  </div>`
    );
  }

  const ending = example.expectation.ending;
  const status =
    ending.kind === 'silent'
      ? `${t('exit', lang)} 0`
      : ending.kind === 'exits'
        ? `<span class="status">${t('exit', lang)} ${ending.code}</span>`
        : `<span class="status">panic: ${escape(ending.message)}</span>`;

  const printed = outputLines(example.expectation);
  const count = `${number(printed, lang)} ${printed === 1 ? t('line', lang) : t('lines', lang)}`;

  return (
    `  <h2>${t('runIt', lang)}</h2>\n` +
    `  <div class="demo run">\n` +
    `    <div class="bar">${escape(example.command)}</div>\n` +
    `<pre data-src="${example.dir}/main.expected"><code>${escape(example.expectation.stdout.replace(/\n$/, ''))}</code></pre>\n` +
    `    <p class="under">${status} &middot; ${count}</p>\n` +
    `  </div>`
  );
}

/**
 * Previous and next in reading order, the way a docs chapter does it, and then
 * a way off the shelf entirely.
 *
 * The second line is the repair for a defect the marketing seat found by
 * COUNTING rather than by walking, which is why nobody had seen it: every one
 * of these pages has a next, a previous and a way back to the list, so walking
 * them feels complete. Measured across the built site, example pages carrying a
 * body link to `/start/`, `/docs/` or `/spec/`: **0 of 133**. A reader who
 * lands on `/examples/json/` from a search result can reach every other program
 * on the site and nothing else at all, which on 134 of 180 pages is most of the
 * site being a closed shelf.
 *
 * It is one line here because it is one line in 268 places.
 */
function walkLinks(slug: string, lang: Lang): string {
  const order = readingOrder();
  const at = order.indexOf(slug);
  const base = `${root(lang)}/examples`;
  const parts: string[] = [];

  if (at + 1 < order.length) {
    const next = order[at + 1];
    parts.push(`${t('next', lang)}: <b><a href="${base}/${next}/">${escape(nameOf(next))}</a></b>.`);
  }
  parts.push(`<a href="${base}/">${t('backToTheList', lang)}</a>.`);
  if (at > 0) {
    const previous = order[at - 1];
    parts.push(
      `<span class="back">&larr; ${t('previous', lang)}: <a href="${base}/${previous}/">${escape(nameOf(previous))}</a></span>`
    );
  }

  const away = `  <p class="next away">\n    ${t('runOneYourself', lang)}: <a href="${root(lang)}/start/">${t('theFourCommands', lang)}</a>. ` +
    `<a href="${root(lang)}/docs/">${t('learnTheLanguage', lang)}</a>, ` +
    `${t('orTakeItWhole', lang)} <a href="${root(lang)}/spec/">${t('inOneFile', lang)}</a>.\n  </p>`;

  return `  <p class="next">\n    ${parts.join('\n    ')}\n  </p>\n${away}`;
}

/** What an example is called in a link: the directory, or the gallery file. */
function nameOf(slug: string): string {
  return slug.startsWith('gallery/') ? slug.slice('gallery/'.length) : slug;
}

/** One example page. */
export function renderExamplePage(slug: string, lang: Lang): RenderedPage {
  const example = exampleBySlug(slug);
  const description = descriptionOf(slug, lang);
  const url = `${root(lang)}/examples/${slug}/`;
  const entry = example.kind === 'program' ? `${example.dir}/` : example.modules[0].path;

  const [first, ...rest] = example.modules;

  const parts: string[] = [];
  parts.push(
    `  <p class="crumb"><a href="${root(lang)}/">Heroes</a> / ` +
      `<a href="${root(lang)}/examples/">${t('examples', lang)}</a> / ${escape(nameOf(slug))}</p>`
  );
  parts.push(`  <h1 class="chapter"><code>${escape(entry)}</code></h1>`);
  parts.push(`  <p class="tagline">${description.tagline}</p>`);
  parts.push(badges(example, lang));
  if (description.body.length > 0) parts.push(`  ${description.body}`);
  parts.push(runBlock(example, lang));
  parts.push(`  <h2>${t('theProgram', lang)}</h2>`);
  parts.push(figure(first, lang));

  for (const module of rest) {
    const unit = module.lines === 1 ? t('line', lang) : t('lines', lang);
    parts.push(
      `  <details class="module">\n` +
        `    <summary><code>${escape(module.name)}</code> &middot; ${number(module.lines, lang)} ${unit}</summary>\n` +
        figure(module, lang) +
        `\n  </details>`
    );
  }

  if (example.inputs.length > 0) {
    parts.push(`  <h2>${t('theInput', lang)}</h2>`);
    for (const input of example.inputs) parts.push(inputFigure(input, lang));
  }

  parts.push(
    `  <p class="fingerprint">${t('fingerprint', lang)} sha256 ` +
      `<code class="hash" title="${example.fingerprint.hash}">${example.fingerprint.short}</code> ` +
      `&middot; ${example.fingerprint.files.length} ${t(example.fingerprint.files.length === 1 ? 'file' : 'files', lang)} ` +
      `&middot; <a href="/examples/index.json">index.json</a></p>`
  );
  parts.push(walkLinks(slug, lang));
  parts.push(`  <footer>\n${footer(lang)}\n  </footer>`);

  return {
    url,
    title: `${entry} · Heroes ${t('examples', lang).toLowerCase()}`,
    description: description.tagline,
    html: `\n${parts.join('\n\n')}\n`,
  };
}

/** The index: the lede from its fragment, the totals, then a shelf per group. */
export function renderExamplesIndex(lang: Lang): RenderedPage {
  const path = lang === 'en' ? `${HOME}/index.html` : 'site/src/html/it/examples/index.html';
  if (!isFile(path)) {
    throw new Error(`${path} is missing: it is the examples landing page's own prose.`);
  }
  const lede = readText(path).trim();
  const all = examples();
  const count = totals();

  const parts: string[] = [lede];

  // The same `.stats` block the home page uses, with its own `.n` and `.what`,
  // so this strip needs no style of its own. The numbers come from the model,
  // so the page cannot state a count the tree does not have.
  parts.push(
    `  <ul class="stats">\n` +
      `    <li><span class="n">${number(count.programs + count.gallery, lang)}</span><span class="what">${t('programs', lang)}</span></li>\n` +
      `    <li><span class="n">${number(count.files, lang)}</span><span class="what">${t('files', lang)}</span></li>\n` +
      `    <li><span class="n">${number(count.lines, lang)}</span><span class="what">${t('lines', lang)}</span></li>\n` +
      `  </ul>`
  );

  for (const group of groups) {
    parts.push(`  <h2 id="${group.key}">${lang === 'it' ? group.it : group.en}</h2>`);
    const cards = group.slugs.map((slug) => {
      const example = all.find((one) => one.slug === slug)!;
      const description = descriptionOf(slug, lang);
      const meta = [`${number(example.lines, lang)} ${t('lines', lang)}`];
      if (example.modules.length > 1) meta.push(`${example.modules.length} ${t('modules', lang)}`);
      if (example.externs.filter((h) => h !== 'hero_os.h').length > 0) meta.push(t('bindsC', lang));
      if (example.threads) meta.push(t('threads', lang));
      return (
        `    <li>\n` +
        `      <h3><code>${escape(nameOf(slug))}</code></h3>\n` +
        `      <p class="meta">${meta.join(' &middot; ')}</p>\n` +
        `      <p>${description.tagline}</p>\n` +
        `      <a class="more" href="${root(lang)}/examples/${slug}/">${t('reads', lang)}</a>\n` +
        `    </li>`
      );
    });
    parts.push(`  <ul class="cards">\n${cards.join('\n')}\n  </ul>`);
  }

  // The shelf's own way off it. The 132 program pages gained one in
  // `walkLinks`, and this page needed the same for the same measured reason:
  // its body linked 66 example pages, eight anchors of its own, and nothing
  // else on the site.
  parts.push(
    lang === 'it'
      ? `  <p class="next away">\n` +
          `    Poi: <a href="/it/docs/">il linguaggio, un&rsquo;idea alla volta</a>, oppure\n` +
          `    <a href="/it/spec/">tutto quanto in un file solo</a> da dare a un modello.\n` +
          `    <a href="/it/start/">Come si esegue uno qualsiasi di questi</a>.\n` +
          `  </p>`
      : `  <p class="next away">\n` +
          `    Next: <a href="/docs/">the language, one idea at a time</a>, or\n` +
          `    <a href="/spec/">the whole thing in one file</a> to hand to a model.\n` +
          `    <a href="/start/">How to run any of these</a>.\n` +
          `  </p>`
  );

  parts.push(`  <footer>\n${footer(lang)}\n  </footer>`);

  return {
    url: `${root(lang)}/examples/`,
    title: lang === 'it' ? 'Esempi: ogni programma di Heroes' : 'Examples: every Heroes program',
    description:
      lang === 'it'
        ? `${count.programs + count.gallery} programmi in Heroes, con il codice, il comando e l'output vero.`
        : `${count.programs + count.gallery} programs written in Heroes, with the code, the command and the real output.`,
    html: `\n${parts.join('\n\n')}\n`,
  };
}

/** Every path the dynamic route builds, both editions handled by the caller. */
export function exampleSlugs(): string[] {
  return examples().map((one) => one.slug);
}

export { shelfOf };
