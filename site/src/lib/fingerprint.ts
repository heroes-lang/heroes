/**
 * The fingerprint an examples page carries, and the one command that reproduces
 * it.
 *
 * The author asked for a hash per example so that a changed program is
 * something anybody can notice. The pages are generated from `examples/` at
 * build time, so the hash is not there to detect drift between the tree and a
 * copy: there is no copy. What it is for is to be quotable. A reader who wants
 * to know whether the page they are looking at shows today's program runs one
 * command and compares twelve characters, and the machine-readable index
 * carries the same value for whatever reads the site next.
 *
 * The shape is `shasum`'s own line format, hashed again:
 *
 *     <sha256 of file 1>  <path 1>
 *     <sha256 of file 2>  <path 2>
 *
 * paths relative to the example's own directory, bytewise sorted, dotfiles
 * excluded, two spaces between hash and path, one trailing newline. Which is
 * exactly what this prints:
 *
 *     cd examples/json && LC_ALL=C find . -type f ! -name '.*' \
 *       | sed 's|^\./||' | sort | xargs shasum -a 256 | shasum -a 256
 *
 * Hashing each file and then the manifest, rather than concatenating the
 * contents, buys two things: the boundary between two files cannot be forged by
 * a file whose text ends where the next one begins, and the per-file hashes are
 * worth publishing on their own, so whatever fetches this later can ask for the
 * one file that moved instead of all of them.
 */

import { createHash } from 'node:crypto';
import { readBytes, byBytes } from './repo.ts';

/** The sha256 of one repository-relative file, lowercase hex. */
export function fileHash(relative: string): string {
  return createHash('sha256').update(readBytes(relative)).digest('hex');
}

export interface FingerprintedFile {
  /** The path relative to the example's directory, as the manifest spells it. */
  path: string;
  sha256: string;
}

export interface Fingerprint {
  /** The 64 hex characters the index publishes. */
  hash: string;
  /** The 12 the page shows. Enough to compare by eye, and it is not the proof. */
  short: string;
  files: FingerprintedFile[];
  /** The manifest itself, so a test can compare it with `shasum`'s output. */
  manifest: string;
}

/**
 * The fingerprint of a directory of files.
 *
 * `dir` is repository-relative and `paths` are repository-relative too; the
 * manifest spells them relative to `dir`, because a fingerprint that carried
 * `examples/json/main.hero` would change if the directory were ever renamed,
 * and the reader's own `cd` into it would then not reproduce it.
 */
export function fingerprintOf(dir: string, paths: string[]): Fingerprint {
  const files: FingerprintedFile[] = paths
    .map((path) => ({ path: path.slice(dir.length + 1), sha256: fileHash(path) }))
    .sort((a, b) => byBytes(a.path, b.path));

  const manifest = files.map((file) => `${file.sha256}  ${file.path}\n`).join('');
  const hash = createHash('sha256').update(manifest, 'utf8').digest('hex');
  return { hash, short: hash.slice(0, 12), files, manifest };
}

/**
 * The command a reader runs to get the same 64 characters back.
 *
 * A program owns its directory, so the command hashes every file in it. A
 * gallery example shares `examples/gallery/` with a dozen others, so its
 * fingerprint covers only its own files and the command names them, in the
 * manifest's own bytewise order: `shasum` prints its lines in argument order,
 * and a `find` over the directory reproduced a hash no page showed (found by
 * the devex seat, 2026-09-09).
 */
export function reproduceCommand(dir: string, files: string[] | null = null): string {
  if (files !== null) {
    const names = files.map((path) => path.slice(dir.length + 1)).sort(byBytes);
    return `cd ${dir} && shasum -a 256 ${names.join(' ')} | shasum -a 256`;
  }
  return (
    `cd ${dir} && LC_ALL=C find . -type f ! -name '.*' | sed 's|^\\./||' | sort ` +
    `| xargs shasum -a 256 | shasum -a 256`
  );
}
