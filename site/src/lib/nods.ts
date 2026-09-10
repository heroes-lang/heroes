/**
 * The nods, and the tracklist that is their key.
 *
 * A nod is the song title set beside a heading in gold, and until now it was
 * unexplained anywhere on the site: thirteen quotations that said nothing at
 * all to a reader who does not know Bowie, on a site whose own rule is that
 * whoever lands here is not assumed to be an expert. The thanks page carries
 * the key now, and every nod links to its own row.
 *
 * Why the rows are GENERATED rather than typed. `site/README.md` kept a ledger
 * of the nods spent so far, by hand, and it drifted without a sound: it listed
 * twenty titles where the markup carried thirteen, and pinned one of them to a
 * section that carries no nod at all. Nothing could tell, because nothing read
 * it. So the markup is the record here, this file holds only what the markup
 * cannot know (the record, the year and where to hear it), and a nod whose data
 * is missing, a row nothing spends, or an edition that has stopped spending the
 * same nods as its twin is a red build rather than a gold line going nowhere.
 *
 * The two Italian strings below are two words long. Prose belongs in the
 * fragments, which is where the edition's own sentences are written and where
 * the author can edit them without opening TypeScript.
 */

import { readText, walk } from './repo.ts';

/**
 * A nod, in either markup shape: block under an `h1` with a literal `·`, or
 * inline inside a heading with `&middot;`. The anchor arrived later, so the
 * pattern reads the classed span's whole content and the title is dug out of
 * it, rather than assuming a link is or is not there.
 */
const NOD = /<span class="nod">([\s\S]*?)<\/span>/g;

/** The nods one edition spends, in the order `walk` finds the files. */
export function nods(lang: 'en' | 'it'): string[] {
  const out: string[] = [];
  for (const file of walk('site/src/html')) {
    if (!file.endsWith('.html')) continue;
    const italian = file.startsWith('site/src/html/it/');
    if (italian !== (lang === 'it')) continue;
    for (const match of readText(file).matchAll(NOD)) {
      out.push(title(match[1]));
    }
  }
  return out;
}

/**
 * The title inside a nod's span: the separators and the anchor stripped, the
 * entity spelled as the character it is, the whitespace collapsed.
 */
function title(inner: string): string {
  return inner
    .replace(/<[^>]*>/g, '')
    .replaceAll('&middot;', '·')
    .replaceAll('·', ' ')
    .replace(/\s+/g, ' ')
    .trim()
    .toLowerCase();
}

/**
 * The anchor a nod points at, derived from the title and never tabulated, so
 * the link in a heading and the id of its row cannot disagree.
 */
export function slug(nod: string): string {
  return nod.replace(/[^a-z0-9]+/g, '-').replace(/^-|-$/g, '');
}

/**
 * What the markup cannot know, **oldest record first** (author instruction
 * 2026-09-10). The panel split on this: the communication seat wanted the
 * order a reader meets the nods, the design seat wanted the years, on the
 * ground that a leading-zero counter over an arbitrary sequence is decoration
 * while over a chronology it is something a reader can use. The author took the
 * years. Within a year the order is the records' own release dates and then the
 * track order inside a record, which is why the three from `Low` (14 January
 * 1977) precede the two from `"Heroes"` (14 October 1977).
 *
 * Every record and year here was read from Wikipedia's own summary in the
 * session that wrote it, and every `hear` is a YouTube id that was put through
 * the oEmbed endpoint, which 404s on a video that is gone and returns the title
 * and the channel for one that is live. Only the artist's channel and the
 * distributor's own art tracks are used: a third party's upload is somebody
 * else's infringement and it disappears. `hunky dory` is an album and not a
 * song, so it has no recording to point at and its row goes to the record.
 */
type Row = {
  /** The title, spelled as a heading spells it where a heading wears it. */
  nod: string;
  /** The record it comes from, as the English edition prints it. */
  record: string;
  /** The record as the Italian edition prints it, where the two differ. */
  recordIt?: string;
  year: number;
  /** Where to hear it, or the record itself when the title names an album. */
  hear: string;
  /**
   * Set where the row is in the list but marks no heading, which today is one
   * row and one only: `"Heroes"` itself. Author instruction 2026-09-10, and it
   * is the obvious absence rather than an exception for its own sake, since the
   * list would otherwise leave out the song the language is named after. The
   * flag exists so the invariant survives: every OTHER row must be spent by a
   * nod, and `assertNodsMirrored` still refuses a row that is not.
   */
  noHeading?: true;
};

export const ROWS: Row[] = [
  // The one row whose right-hand column names no other record, because the
  // record IS the row's title. `&rsquo;` and not a straight apostrophe: the
  // Italian edition's typography rule reaches the HTML fragments and nothing
  // reads this directory, so a straight quote walked back in here hours after
  // the last four were removed from the edition.
  { nod: 'hunky dory', record: 'the album itself', recordIt: 'l&rsquo;album stesso', year: 1971, hear: 'https://en.wikipedia.org/wiki/Hunky_Dory' },
  { nod: 'changes', record: '<i>Hunky Dory</i>', year: 1971, hear: 'https://www.youtube.com/watch?v=7fdhI3qUdSs' },
  { nod: 'quicksand, avoided', record: 'Quicksand, on <i>Hunky Dory</i>', recordIt: 'Quicksand, su <i>Hunky Dory</i>', year: 1971, hear: 'https://www.youtube.com/watch?v=kFN_bEgDE0M' },
  { nod: 'five years', record: '<i>Ziggy Stardust</i>', year: 1972, hear: 'https://www.youtube.com/watch?v=2ObjtVdsV3I' },
  { nod: 'moonage daydream', record: '<i>Ziggy Stardust</i>', year: 1972, hear: 'https://www.youtube.com/watch?v=RPUAldgS7Sg' },
  { nod: 'lady stardust', record: '<i>Ziggy Stardust</i>', year: 1972, hear: 'https://www.youtube.com/watch?v=EcKZEOsgvdI' },
  { nod: 'rebel rebel', record: '<i>Diamond Dogs</i>', year: 1974, hear: 'https://www.youtube.com/watch?v=DJxCsVcZL2I' },
  { nod: 'speed of life', record: '<i>Low</i>', year: 1977, hear: 'https://www.youtube.com/watch?v=2oRgZjcfE4g' },
  { nod: 'sound and vision', record: '<i>Low</i>', year: 1977, hear: 'https://www.youtube.com/watch?v=ZV_UsQPTBy4' },
  { nod: 'a new career in a new town', record: '<i>Low</i>', year: 1977, hear: 'https://www.youtube.com/watch?v=kZssy0IiyMA' },
  // The song the language is named after, and the one row no heading wears.
  // Verified through oEmbed like the rest: `David Bowie - "Heroes" (Official
  // Video) [HD]`, the artist's own channel, with Bowie's quotation marks in
  // the title. It is linked from the paragraph above the list as well, because
  // that is the sentence a reader looks for it in; this row is so that the
  // tracklist is not the one place on the site that leaves it out.
  { nod: 'heroes', record: '<i>&ldquo;Heroes&rdquo;</i>', year: 1977, hear: 'https://www.youtube.com/watch?v=lXgkuM2NhYI', noHeading: true },
  { nod: 'sons of the silent age', record: '<i>&ldquo;Heroes&rdquo;</i>', year: 1977, hear: 'https://www.youtube.com/watch?v=QDPx1DzuK_s' },
  { nod: 'fashion', record: '<i>Scary Monsters</i>', year: 1980, hear: 'https://www.youtube.com/watch?v=F-z6u5hFgPk' },
];

/**
 * How many titles a heading wears, which is what the prose claims.
 *
 * NOT `ROWS.length`: the list carries one row no heading wears, so the sentence
 * "all twelve are above" would become false against a thirteen-row list while
 * still reading as correct. The count follows the nods and the prose names the
 * extra row separately.
 */
export function nodCount(): number {
  return ROWS.filter((row) => !row.noHeading).length;
}

/**
 * The tracklist, as the fragment's `{{tracklist}}` becomes.
 *
 * The title carries the link, which is the shape the guide's own chapter list
 * already uses, and it keeps the block from stacking twelve identical words a
 * screen reader would read as "listen, listen, listen". In the Italian edition
 * the title is marked `lang="en"`, so an Italian reader's screen reader does
 * not sound out an English sentence with Italian phonetics.
 *
 * These open in a new tab, and so does every other link that leaves the site:
 * the rule is one rule and `fillOutbound` in `figures.ts` applies it to all of
 * them, so nothing here has to remember it.
 */
export function tracklist(lang: 'en' | 'it'): string {
  const rows = ROWS.map((row) => {
    const record = lang === 'it' && row.recordIt ? row.recordIt : row.record;
    const marked = lang === 'it' ? ' lang="en"' : '';
    return (
      `    <li id="${slug(row.nod)}"><a href="${row.hear}"${marked}>${row.nod}</a>\n` +
      `      <span class="from">${record}, ${row.year}</span></li>`
    );
  });
  return `  <ol class="tracks">\n${rows.join('\n')}\n  </ol>`;
}

/* -- The checks ------------------------------------------------------------ */

const THANKS = ['site/src/html/about/thanks.html', 'site/src/html/it/about/thanks.html'];

/**
 * The nod on the home page is deliberately inert: `Run it` is the highest-intent
 * heading on the site, and the four commands a visitor might paste sit directly
 * under it, so that section keeps no link that leaves. The marketing and devex
 * seats reached this independently, and the home page's own footer carries the
 * way in instead.
 */
const INERT = ['speed of life'];

const langOf = (pagePath: string): 'en' | 'it' =>
  pagePath.startsWith('site/src/html/it/') ? 'it' : 'en';

/**
 * Every nod on this page is a link to its own row, and every nod on the site
 * has a row. Runs per page, from `checkClaims`.
 */
export function checkNods(html: string, pagePath: string): string[] {
  if (!pagePath.startsWith('site/src/html/')) return [];
  const problems: string[] = [];
  const lang = langOf(pagePath);
  const prefix = lang === 'it' ? '/it/about/thanks/' : '/about/thanks/';
  const known = new Set(ROWS.map((row) => row.nod));

  for (const match of html.matchAll(NOD)) {
    const nod = title(match[1]);
    if (!known.has(nod)) {
      problems.push(
        `${pagePath}: the nod "${nod}" has no row in the tracklist. Add it to ROWS in ` +
          `site/src/lib/nods.ts, with the record and year read from a source in the session ` +
          `that writes them, or take the nod off the page.`
      );
      continue;
    }
    if (INERT.includes(nod)) continue;
    const href = `${prefix}#${slug(nod)}`;
    if (!match[1].includes(`href="${href}"`)) {
      problems.push(
        `${pagePath}: the nod "${nod}" does not link to its own row. Expected href="${href}".` +
          (lang === 'it' ? ' An Italian nod pointing at the English page sends the reader out of their edition.' : '')
      );
    }
  }

  if (THANKS.includes(pagePath) && !html.includes('{{tracklist}}')) {
    problems.push(
      `${pagePath}: the tracklist placeholder {{tracklist}} is gone, so every nod on the site ` +
        `points at a row that is no longer printed.`
    );
  }
  return problems;
}

/**
 * The two editions spend the same nods.
 *
 * Runs once, when the build ends, because it is a fact about the whole tree
 * rather than about one page. It is worth having with or without the tracklist:
 * a nod dropped, duplicated or translated in one edition is invisible today,
 * and `site/README.md` § The Italian edition says a nod is never translated.
 */
export function assertNodsMirrored(): number {
  const en = nods('en').sort();
  const it = nods('it').sort();
  if (en.join('|') !== it.join('|')) {
    const missing = en.filter((nod) => !it.includes(nod));
    const extra = it.filter((nod) => !en.includes(nod));
    throw new Error(
      `The two editions no longer spend the same nods.\n` +
        (missing.length > 0 ? `    only in the English edition: ${missing.join(', ')}\n` : '') +
        (extra.length > 0 ? `    only in the Italian edition: ${extra.join(', ')}\n` : '') +
        `  A nod is a quotation and is never translated (site/README.md § The Italian edition),\n` +
        `  so the two editions carry the same titles or one of them has drifted.\n`
    );
  }
  const unspent = ROWS.filter((row) => !row.noHeading && !en.includes(row.nod));
  if (unspent.length > 0) {
    throw new Error(
      `${unspent.length} row(s) of the tracklist are spent by no nod: ` +
        `${unspent.map((row) => row.nod).join(', ')}.\n` +
        `  The markup is the record. Remove the row, spend the title on a heading, or, if\n` +
        `  the row belongs in the list without a heading the way "Heroes" itself does, mark\n` +
        `  it noHeading in ROWS so the exception is declared rather than assumed.\n`
    );
  }
  return en.length;
}
