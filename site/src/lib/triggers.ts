/**
 * The deploy's triggers, held to what the build reads.
 *
 * `.github/workflows/deploy-site.yml` publishes the site on a push that touches
 * one of its `paths:`, and this build reads files from all over the repository:
 * the chain out of the ROADMAP, the ceiling out of the spec's suite, the verbs
 * out of the compiler's command table, the sealed suite's hashes for the
 * Measured page. A file the build reads and the list does not name changes a
 * page nobody republishes. That is how the chain page went stale (found
 * 2026-10-20), and the list had been widened by hand four times before that,
 * each time after a page had gone stale. So the list is no longer remembered:
 * every read is written down as it happens (`repo.ts`, `record`), and at the end
 * of the build this asks the two directions of one promise. Is every read under
 * a path the workflow names? And is every path the workflow names read by
 * something? A path nothing reads is a deploy that can change nothing, and a
 * list that keeps one has stopped being a measurement.
 *
 * ONLY TWO SHAPES OF PATH, an exact path and `dir/**`, and any other is refused
 * rather than guessed. GitHub's filter syntax also has `*`, `?`, `[...]` and
 * `!`, and a matcher that read one of those wrongly would pass a list that does
 * not cover the reads, which is the failure this module exists to end.
 *
 * WHAT IT CANNOT SEE: a file reached by a subprocess rather than through
 * `repo.ts`. `lastmod.ts`'s `git log` reads the history, which no `paths:` entry
 * could name and which every push moves; it is the one such read today.
 */

import { existsSync, readFileSync, unlinkSync } from 'node:fs';
import { absolute, readText, READS_FILE } from './repo.ts';

const WORKFLOW = '.github/workflows/deploy-site.yml';

/**
 * Fewer reads than this and the log was lost, not short. A build whose log
 * lost its lines would answer both questions with nothing to answer them about,
 * and pass. Measured 1,325 reads on the build that introduced the check
 * (2026-10-24), 678 of them the compiler's modules the project page counts.
 */
const READS_FLOOR = 1000;

/**
 * The `paths:` of the workflow's `push` trigger, in file order.
 *
 * Read by its indentation, the shape the file has: `on:` at column 0, `push:`
 * under it, `paths:` under that, one `- 'path'` per line. Comment lines may
 * stand anywhere; any other line ends the list. A file that has moved away from
 * that shape is refused by name rather than read as an empty list.
 */
export function triggerPaths(yaml: string): string[] {
  const lines = yaml.split('\n');
  const on = lines.findIndex((line) => line === 'on:');
  if (on < 0) throw new Error(`${WORKFLOW}: no \`on:\` at column 0 to read the triggers from.`);
  let push = -1;
  for (let i = on + 1; i < lines.length && !/^\S/.test(lines[i]); i += 1) {
    if (/^ {2}push:\s*$/.test(lines[i])) {
      push = i;
      break;
    }
  }
  if (push < 0) throw new Error(`${WORKFLOW}: no \`push:\` under \`on:\`.`);
  let paths = -1;
  for (let i = push + 1; i < lines.length && !/^ {0,2}\S/.test(lines[i]); i += 1) {
    if (/^ {4}paths:\s*$/.test(lines[i])) {
      paths = i;
      break;
    }
  }
  if (paths < 0) throw new Error(`${WORKFLOW}: no \`paths:\` under \`push:\`, so every push would deploy and this check has nothing to hold.`);

  const out: string[] = [];
  for (let i = paths + 1; i < lines.length; i += 1) {
    const line = lines[i];
    if (/^\s*#/.test(line)) continue;
    const item = /^ {6}- (.+?)\s*$/.exec(line);
    if (item === null) break;
    out.push(shapeOf(item[1].replace(/^(['"])(.*)\1$/, '$2')));
  }
  if (out.length === 0) throw new Error(`${WORKFLOW}: \`paths:\` lists nothing.`);
  return out;
}

/** The pattern as written, refused where it is not one of the two shapes. */
function shapeOf(pattern: string): string {
  const body = pattern.endsWith('/**') ? pattern.slice(0, -3) : pattern;
  if (body === '' || /[*?[\]!]/.test(body) || body.startsWith('/') || body.startsWith('./') || body.endsWith('/')) {
    throw new Error(
      `${WORKFLOW}: the path \`${pattern}\` is not an exact path or \`dir/**\`.\n` +
        `  Those are the two shapes this check reads; any other glob would be guessed at,\n` +
        `  and a guess that covers too much passes a list that does not cover the build's reads.`
    );
  }
  return pattern;
}

/** Whether `pattern` covers the repository path `path`: itself, or anything under its directory. */
export function covers(pattern: string, path: string): boolean {
  if (pattern.endsWith('/**')) {
    const dir = pattern.slice(0, -3);
    return path === dir || path.startsWith(`${dir}/`);
  }
  return path === pattern;
}

/**
 * Ask both questions of this build's reads, delete the log, and say what was
 * held. Throws with every uncovered read and every unread path at once, since a
 * list repaired one line per failed build is a list repaired slowly.
 */
export function assertTriggersCoverReads(): string {
  // Read first, so the workflow is one of the reads it is held to.
  const patterns = triggerPaths(readText(WORKFLOW));
  const file = absolute(READS_FILE);
  const reads = existsSync(file) ? [...new Set(readFileSync(file, 'utf-8').split('\n').filter(Boolean))].sort() : [];
  if (existsSync(file)) unlinkSync(file);

  if (reads.length < READS_FLOOR) {
    throw new Error(
      `the build recorded ${reads.length} repository reads and the floor is ${READS_FLOOR}.\n` +
        `  The log in ${READS_FILE} lost its lines, or a read stopped going through repo.ts.`
    );
  }

  const uncovered = reads.filter((path) => !patterns.some((pattern) => covers(pattern, path)));
  const unread = patterns.filter((pattern) => !reads.some((path) => covers(pattern, path)));
  const problems: string[] = [];
  if (uncovered.length > 0) {
    problems.push(
      `${uncovered.length} path(s) this build read that no push publishing the site would touch:\n` +
        uncovered.map((path) => `      - '${path}'`).join('\n') +
        `\n  A change to one of them changes a page and deploys nothing. Add each to \`paths:\`\n` +
        `  in ${WORKFLOW}, or its directory as \`dir/**\`.`
    );
  }
  if (unread.length > 0) {
    problems.push(
      `${unread.length} path(s) in \`paths:\` that nothing in this build read:\n` +
        unread.map((pattern) => `      - '${pattern}'`).join('\n') +
        `\n  A push there deploys and can change nothing. If the page that read it is gone,\n` +
        `  the line goes; if it is read some way repo.ts does not record, that way is the defect.`
    );
  }
  if (problems.length > 0) {
    throw new Error(`${WORKFLOW} does not match what the site's build reads.\n  ` + problems.join('\n  '));
  }
  return `triggers: ${reads.length} paths read, every one under the ${patterns.length} that publish the site, and each of those read`;
}
