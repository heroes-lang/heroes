/**
 * The build log as a feed, so a reader can be told rather than having to come
 * back and look.
 *
 * Both panel seats asked the same question and neither found an answer on any
 * page: how does somebody hear when the repository opens? The site ships no
 * JavaScript and collects nothing, which rules out a signup form, and it does
 * NOT rule out syndication: this is static XML generated at build time, the
 * same shape `sitemap-0.xml` already has, and nothing about the reader reaches
 * anybody. The day the log says the repository is open, every feed reader says
 * so too.
 *
 * One feed, from the English log, because a feed carries titles rather than
 * prose and the Italian edition's entries are the same milestones. Every entry
 * links the anchor of its own postcard, and those ids count from the OLDEST
 * entry so that a new one at the top does not renumber the rest: an id in a
 * feed is a promise to a reader's feed reader, and renumbering would resend all
 * thirty-three.
 *
 * **The one date on this site, and it is here on purpose.** `site/CLAUDE.md`
 * forbids dates on the page, and RFC 4287 requires `updated` on a feed and on
 * every entry: a feed without them is one a strict reader may refuse and a
 * lenient one cannot sort. The rule is about the site saying "on this date this
 * was measured", which is what the author objected to; this is the exchange
 * format's own synchronisation field, the same job `Last-Modified` does on
 * every response already. It is the build's time rather than any entry's,
 * because the entries carry no dates and the order of the list is their
 * chronology. If the author would rather have neither, the answer is to drop
 * the feed rather than to ship an invalid one, and that is one line.
 */
import type { APIRoute } from 'astro';
import { readText } from '../lib/repo.ts';

const ORIGIN = 'https://heroes-lang.org';

/**
 * The typographic entities the log's prose uses. They have to be decoded before
 * the text is escaped for XML, or a feed reader shows `&rsquo;` as five
 * characters: the first pass through this printed exactly that.
 */
const ENTITIES: Record<string, string> = {
  '&rsquo;': '\u2019',
  '&lsquo;': '\u2018',
  '&ldquo;': '\u201c',
  '&rdquo;': '\u201d',
  '&mdash;': '\u2014',
  '&ndash;': '\u2013',
  '&middot;': '\u00b7',
  '&hellip;': '\u2026',
  '&rarr;': '\u2192',
  '&larr;': '\u2190',
  '&amp;': '&',
  '&lt;': '<',
  '&gt;': '>',
};

function decode(text: string): string {
  return text.replace(/&(?:rsquo|lsquo|ldquo|rdquo|mdash|ndash|middot|hellip|rarr|larr|amp|lt|gt);/g, (e) => ENTITIES[e]);
}

/** One entry per postcard: the milestone name and the sentence in bold. */
function entries(): { title: string; summary: string }[] {
  const html = readText('site/src/html/log.html');
  const out: { title: string; summary: string }[] = [];
  const item = /<span class="when">([\s\S]*?)<\/span>\s*<b>([\s\S]*?)<\/b>/g;
  for (const match of html.matchAll(item)) {
    const strip = (s: string) => decode(s.replace(/<[^>]*>/g, '')).replace(/\s+/g, ' ').trim();
    out.push({ title: strip(match[1]), summary: strip(match[2]) });
  }
  if (out.length === 0) {
    throw new Error('site/src/html/log.html gave no entries: the feed reads its `when` and `b` pairs.');
  }
  return out;
}

const escape = (s: string) =>
  s.replace(/[&<>]/g, (c) => (c === '&' ? '&amp;' : c === '<' ? '&lt;' : '&gt;'));

export const GET: APIRoute = () => {
  const all = entries();
  const updated = new Date().toISOString().replace(/\.\d+Z$/, 'Z');
  const body =
    `<?xml version="1.0" encoding="utf-8"?>\n` +
    `<feed xmlns="http://www.w3.org/2005/Atom">\n` +
    `  <title>Heroes: the build log</title>\n` +
    `  <subtitle>One entry per milestone, newest first.</subtitle>\n` +
    `  <link href="${ORIGIN}/log/"/>\n` +
    `  <link rel="self" href="${ORIGIN}/log.xml"/>\n` +
    `  <id>${ORIGIN}/log/</id>\n` +
    `  <updated>${updated}</updated>\n` +
    `  <author><name>Giuseppe Arici</name></author>\n` +
    all
      .map(
        (e, i) =>
          `  <entry>\n` +
          `    <title>${escape(e.title)}</title>\n` +
          `    <link href="${ORIGIN}/log/#${all.length - i}"/>\n` +
          `    <id>${ORIGIN}/log/#${all.length - i}</id>\n` +
          `    <updated>${updated}</updated>\n` +
          `    <summary>${escape(e.summary)}</summary>\n` +
          `  </entry>\n`
      )
      .join('') +
    `</feed>\n`;
  return new Response(body, { headers: { 'content-type': 'application/atom+xml; charset=utf-8' } });
};
