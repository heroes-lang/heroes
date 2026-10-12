/**
 * The project page's sizes, read from the tree at build time: the compiler's
 * modules and lines, and the design sittings on the record.
 *
 * They were typed in by hand as `200+` modules, `60k` lines and `150+`
 * sittings, and by the day they were replaced the tree held 618 modules,
 * 161,358 lines and 210 sittings: `60k` read as a count and was wrong by a
 * factor of almost three. `site/CLAUDE.md` § A number on the page is
 * generated, or it is a threshold, prefers the generated number wherever the
 * tree can give it, so the page now carries `{{modules}}`, `{{linesK}}` and
 * `{{sittings}}` and this module fills them.
 *
 * Each is FLOORED (modules and sittings to the ten, lines to the thousand) and
 * printed with a `+` or a `k` by the page. The deploy runs on a push touching
 * the site, not on every compiler commit, so a live page outlives the tree it
 * was built from; a floor of a number that grows stays true until the next
 * deploy, where an exact figure would be stale by the next commit.
 *
 * A parse that finds almost nothing fails the build rather than printing a
 * small number: the failure this guards is a silent `0+` (the shape
 * `tables.ts` guards for the word tables).
 */

import { directoriesIn, filesIn, readText } from './repo.ts';

const COMPILER_DIR = 'selfhost';
const PANEL_DIR = 'docs/panel';

/** Far below today's counts, far above what a broken walk would find. */
const MODULE_FLOOR = 300;
const LINE_FLOOR = 50_000;
const SITTING_FLOOR = 100;

/** A sitting is `docs/panel/NNN-<slug>.md`; its reports live in folders beside it. */
const SITTING_FILE = /^\d{3}-.+\.md$/;

/** Every `.hero` file under `dir`, the directories walked depth first. */
function heroFiles(dir: string): string[] {
  const here = filesIn(dir).filter((path) => path.endsWith('.hero'));
  return here.concat(...directoriesIn(dir).map(heroFiles));
}

function floored(value: number, step: number): number {
  return Math.floor(value / step) * step;
}

function atLeast(found: number, floor: number, what: string): number {
  if (found < floor) {
    throw new Error(
      `the site counts ${found} ${what}, under its floor of ${floor}.\n` +
        `  the project page prints this number, so a walk that lost the tree stops the build instead.`
    );
  }
  return found;
}

let cached: { modules: number; lines: number; sittings: number } | null = null;

/** The three counts, exact. Memoised: two editions ask for the same tree. */
export function sizes(): { modules: number; lines: number; sittings: number } {
  if (cached !== null) return cached;
  const files = heroFiles(COMPILER_DIR);
  const lines = files.reduce((sum, path) => sum + readText(path).split('\n').length - 1, 0);
  const sittings = new Set(
    filesIn(PANEL_DIR)
      .map((path) => path.slice(PANEL_DIR.length + 1))
      .filter((name) => SITTING_FILE.test(name))
      .map((name) => name.slice(0, 3))
  ).size;
  cached = {
    modules: atLeast(files.length, MODULE_FLOOR, `modules under ${COMPILER_DIR}/`),
    lines: atLeast(lines, LINE_FLOOR, `lines under ${COMPILER_DIR}/`),
    sittings: atLeast(sittings, SITTING_FLOOR, `sittings under ${PANEL_DIR}/`),
  };
  return cached;
}

/** `{{modules}}`, `{{linesK}}` and `{{sittings}}` in a fragment, floored. */
export function fillSizes(html: string): string {
  if (!html.includes('{{modules}}') && !html.includes('{{linesK}}') && !html.includes('{{sittings}}')) return html;
  const size = sizes();
  return html
    .replaceAll('{{modules}}', String(floored(size.modules, 10)))
    .replaceAll('{{linesK}}', String(floored(size.lines, 1000) / 1000))
    .replaceAll('{{sittings}}', String(floored(size.sittings, 10)));
}
