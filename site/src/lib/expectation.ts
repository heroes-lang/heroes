/**
 * What a program prints, and how it ends, read from its `main.expected`.
 *
 * This is the JavaScript twin of `tests/harness/expectation.hero`, and the
 * convention is that file's, stated in its own words: an expectation is the
 * program's stdout, optionally followed by ONE last line saying it does not end
 * at exit 0 with a silent stderr.
 *
 *     !panic: <message>   an abort, and the text stderr must carry
 *     !exit: <code>       exit(code), and the status the shell must see
 *
 * Why the site needs it. The examples pages show the real output beside the
 * program, the way Zig's samples and Gleam's homepage do, and that output is
 * already asserted byte for byte in three configurations on every push, so it
 * is the strongest evidence the site can carry for free. What it must never do
 * is print the harness's own last line at a reader: `!exit: 1` on a page is a
 * program that appears to print a marker it does not print. The ending is
 * parsed here and rendered as an exit status instead.
 *
 * A malformed ending throws rather than reading as `silent`, for the reason the
 * Heroes file gives: `!exit: three` is a broken case, and treating it as "no
 * ending expected" would report a mismatch about something else entirely.
 */

export type Ending =
  | { kind: 'silent' }
  | { kind: 'exits'; code: number }
  | { kind: 'panics'; message: string };

export interface Expectation {
  /** The stdout, with its trailing newline, exactly as the harness rebuilds it. */
  stdout: string;
  ending: Ending;
}

const PANIC = '!panic: ';
const EXIT = '!exit: ';

/**
 * Split the text of a `main.expected` into the output it promises and the way
 * the program ends.
 *
 * The line handling follows `strings.lines` and `split`: split on newline, drop
 * one trailing empty piece, take the last line if it is an ending, rejoin, and
 * put one newline back when anything is left. That is why a file ending in a
 * newline and a file not ending in one give the same expectation.
 */
export function splitExpectation(text: string, label: string): Expectation {
  const pieces = text.split('\n');
  if (pieces.length > 0 && pieces[pieces.length - 1] === '') pieces.pop();

  let ending: Ending = { kind: 'silent' };
  let kept = pieces;

  if (pieces.length > 0) {
    const last = pieces[pieces.length - 1];
    if (last.startsWith(PANIC)) {
      ending = { kind: 'panics', message: last.slice(PANIC.length) };
      kept = pieces.slice(0, -1);
    } else if (last.startsWith(EXIT)) {
      const code = Number(last.slice(EXIT.length).trim());
      if (!Number.isInteger(code)) {
        throw new Error(
          `${label}: \`${last}\` is not an exit code.\n` +
            `  the convention is tests/harness/expectation.hero's: the last line may be\n` +
            `  \`!exit: <code>\` or \`!panic: <message>\`, and nothing else.`
        );
      }
      ending = { kind: 'exits', code };
      kept = pieces.slice(0, -1);
    }
  }

  // An ending marker anywhere but the last line is a case nobody can read, and
  // it would reach the page as literal text.
  for (const [index, line] of kept.entries()) {
    if (line.startsWith(PANIC) || line.startsWith(EXIT)) {
      throw new Error(
        `${label}: line ${index + 1} carries an ending marker, and only the last line may.\n` +
          `  the line is: ${JSON.stringify(line)}`
      );
    }
  }

  const stdout = kept.length === 0 ? '' : `${kept.join('\n')}\n`;
  return { stdout, ending };
}

/** How many lines of output there are, for the page's own count. */
export function outputLines(expectation: Expectation): number {
  if (expectation.stdout.length === 0) return 0;
  return expectation.stdout.replace(/\n$/, '').split('\n').length;
}
