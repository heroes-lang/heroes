/**
 * The chain, read out of `docs/ROADMAP.md` § The chain at build time.
 *
 * The project page carried the Zen, the principles and the panel and said
 * nothing about what is built and what is next, which is the one question a
 * visitor asks of a project page (author instruction 2026-09-11). The answer
 * has existed all along in one table of the repository, so the page reads that
 * table rather than restating it: `site/CLAUDE.md` § A number on the page is
 * generated, or it is a threshold.
 *
 * WHAT IT READS. Rows of six cells: the number, the milestone's identifier in
 * bold, the status, the tag, the journal link and the notes.
 * `status` is `done <date>` for a closed milestone and `scheduled` or `**OPEN**`
 * otherwise. The notes cell is one long sentence with every scheduling fact the
 * ROADMAP keeps out of its cells, so the page takes its FIRST clause, up to the
 * first ` · ` or the first full stop, which is the milestone's own summary and
 * is where the ROADMAP's own convention puts it.
 *
 * WHAT IT ASSERTS, because a parse that quietly reads nothing renders an empty
 * table and nobody notices: at least sixty rows, at least forty closed, and a row
 * number sequence with no gap. A ROADMAP whose shape changes fails the build
 * rather than the page.
 *
 * IT NO LONGER ASSERTS ONE OPEN ROW. It did until the author's decision of
 * 2026-09-12, and that rule was written before work happened in lanes: one
 * milestone is one file is one worktree now, so two sessions can hold two
 * milestones and the table is the only place a reader could see the second one.
 * The two floors above stay, because they guard a parse that fell apart, which
 * is a different failure and still a real one.
 */

import { readText } from './repo.ts';

export interface ChainRow {
  number: number;
  name: string;
  /** `done`, `open` or `scheduled`. */
  state: 'done' | 'open' | 'scheduled';
  /** The closing date for a done row, `''` otherwise. */
  closed: string;
  /** The tag, or `''` where a milestone closed untagged or is not closed. */
  tag: string;
  /** The first clause of the notes cell: what the milestone delivers. */
  summary: string;
}

// The trailing `|` is optional because nine of the seventy rows do not carry
// one: markdown renders a row either way, so the table has both shapes and a
// reader that demanded the pipe silently read nine milestones fewer.
const ROW = /^\|\s*(\d+)\s*\|\s*\*\*(M-[a-z0-9-]+)\*\*\s*\|([^|]*)\|([^|]*)\|([^|]*)\|(.*?)\|?\s*$/;
const FLOOR_ROWS = 60;
const FLOOR_DONE = 40;

function firstClause(notes: string): string {
  let text = notes.trim();
  // The cells carry `·` between facts and `**§1.1**` as a warrant at the end.
  const dot = text.indexOf(' · ');
  if (dot > 0) text = text.slice(0, dot);
  // A colon or a comma is inside the clause; a full stop ends it.
  const stop = text.search(/\.\s/);
  if (stop > 40) text = text.slice(0, stop + 1);
  return text.replace(/\s+/g, ' ').trim();
}

export function chain(): ChainRow[] {
  const text = readText('docs/ROADMAP.md');
  const start = text.indexOf('\n## The chain');
  if (start < 0) throw new Error('docs/ROADMAP.md: no `## The chain` section to read the milestones from.');
  const after = text.slice(start);
  const end = after.indexOf('\n## ', 1);
  const section = end < 0 ? after : after.slice(0, end);

  const rows: ChainRow[] = [];
  for (const line of section.split('\n')) {
    const m = ROW.exec(line);
    if (m === null) continue;
    const status = m[3].trim();
    const state = status.startsWith('done') ? 'done' : status.includes('OPEN') ? 'open' : 'scheduled';
    rows.push({
      number: Number(m[1]),
      name: m[2],
      state,
      closed: state === 'done' ? status.replace(/^done\s*/, '') : '',
      tag: m[4].replace(/`/g, '').trim() === '—' ? '' : m[4].replace(/`/g, '').trim(),
      summary: firstClause(m[6]),
    });
  }

  if (rows.length < FLOOR_ROWS) {
    throw new Error(
      `docs/ROADMAP.md § The chain: read ${rows.length} milestone rows and the floor is ${FLOOR_ROWS}.\n` +
        `  a parse that falls apart renders an empty table, which is why there is a floor instead of trust.`
    );
  }
  const done = rows.filter((r) => r.state === 'done').length;
  if (done < FLOOR_DONE) {
    throw new Error(`docs/ROADMAP.md § The chain: only ${done} rows read as closed, and the floor is ${FLOOR_DONE}.`);
  }
  rows.forEach((r, i) => {
    if (r.number !== i + 1) {
      throw new Error(`docs/ROADMAP.md § The chain: row ${i + 1} is numbered ${r.number}, so the sequence has a gap.`);
    }
  });
  return rows;
}

export function chainCounts(): { total: number; done: number; next: ChainRow | null } {
  const rows = chain();
  const done = rows.filter((r) => r.state === 'done').length;
  const next = rows.find((r) => r.state === 'open') ?? rows.find((r) => r.state === 'scheduled') ?? null;
  return { total: rows.length, done, next };
}

/**
 * The chain as one section of a page: the counts, then the milestones that are
 * next, then the closed ones behind a `details` a reader opens if they want
 * them. Sixty-nine rows read as a wall; the five that have not happened yet are
 * what somebody came for.
 *
 * Every number here is the table's own. Nothing is written by hand, which is
 * why the page cannot go stale against the repository the way a summary would.
 */
export function chainSection(lang: 'en' | 'it'): string {
  const rows = chain();
  const done = rows.filter((r) => r.state === 'done');
  const ahead = rows.filter((r) => r.state !== 'done');
  // More than one row may be open, since work happens in lanes: one milestone,
  // one file, one worktree. The sentence has to carry all of them, because a
  // page that names the first of two is a page that is wrong about the second.
  const open = rows.filter((r) => r.state === 'open');
  const it = lang === 'it';
  const names = open.map((r) => `<code>${esc(r.name)}</code>`);
  const listed = (last: string) =>
    names.length === 1 ? names[0] : `${names.slice(0, -1).join(', ')} ${last} ${names[names.length - 1]}`;

  const openIt = open.length === 0
    ? ', e nessuno è aperto in questo momento'
    : open.length === 1
      ? `, e quello aperto è ${listed('e')}`
      : `, e quelli aperti sono ${listed('e')}`;
  const openEn = open.length === 0
    ? ', and none is open right now'
    : open.length === 1
      ? `, and the open one is ${listed('and')}`
      : `, and the open ones are ${listed('and')}`;

  const lead = it
    ? `<p>La catena è una tabella nel repository, una riga per traguardo, chiusi prima e programmati dopo. Questa pagina la legge quando il sito viene generato: <b>${done.length}</b> traguardi chiusi di <b>${rows.length}</b>${openIt}.</p>`
    : `<p>The chain is one table in the repository, a row per milestone, closed first and scheduled after. This page reads it when the site is built: <b>${done.length}</b> milestones closed of <b>${rows.length}</b>${openEn}.</p>`;

  const aheadRows = ahead
    .map(
      (r) =>
        `      <tr><td class="n">${r.number}</td><td><code>${esc(r.name)}</code></td><td>${cell(r.summary)}</td></tr>`
    )
    .join('\n');

  const doneRows = done
    .slice()
    .reverse()
    .map(
      (r) =>
        `      <tr><td class="n">${r.number}</td><td><code>${esc(r.name)}</code></td><td class="when">${esc(r.closed)}</td><td>${cell(r.summary)}</td></tr>`
    )
    .join('\n');

  const aheadHead = it
    ? `<h3>Quello che viene</h3>\n  <table class="chain">\n    <thead><tr><th>#</th><th>traguardo</th><th>cosa consegna</th></tr></thead>\n    <tbody>`
    : `<h3>What comes next</h3>\n  <table class="chain">\n    <thead><tr><th>#</th><th>milestone</th><th>what it delivers</th></tr></thead>\n    <tbody>`;

  const doneHead = it
    ? `<summary>I ${done.length} traguardi chiusi, il più recente per primo</summary>\n  <table class="chain">\n    <thead><tr><th>#</th><th>traguardo</th><th>chiuso</th><th>cosa ha consegnato</th></tr></thead>\n    <tbody>`
    : `<summary>The ${done.length} closed milestones, the most recent first</summary>\n  <table class="chain">\n    <thead><tr><th>#</th><th>milestone</th><th>closed</th><th>what it delivered</th></tr></thead>\n    <tbody>`;

  return [
    lead,
    `  ${aheadHead}`,
    aheadRows,
    '    </tbody>',
    '  </table>',
    `  <details class="chain">`,
    `  ${doneHead}`,
    doneRows,
    '    </tbody>',
    '  </table>',
    '  </details>',
  ].join('\n');
}

function esc(text: string): string {
  return text.replace(/[&<>]/g, (ch) => (ch === '&' ? '&amp;' : ch === '<' ? '&lt;' : '&gt;'));
}

/**
 * A summary cell: escaped, then the ROADMAP's own backticks become `code`, so
 * that `heroes doc` reads as a command on the page and not as a quotation. The
 * escape runs first, so a `<` inside a backticked fragment is text either way.
 */
function cell(text: string): string {
  return esc(text).replace(/`([^`]+)`/g, '<code>$1</code>');
}
