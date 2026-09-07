/**
 * `/examples/index.json`: the corpus as data, for whatever reads this site next.
 *
 * Two readers are in view and neither is a browser. The first is the milestone
 * that will let a visitor compile Heroes without installing it: it needs to know
 * which programs name a C header, because a program that binds SQLite cannot run
 * in a page, and that list is here rather than in a document somebody keeps by
 * hand. The second is anybody checking whether the site shows today's programs:
 * every file carries its own hash, so one fetch answers it.
 *
 * There is no date field, and that is the site's rule rather than an omission
 * (`site/CLAUDE.md` § No dates on the page). The hashes are the timestamp: two
 * fetches with the same hash are the same corpus, whenever they happened.
 *
 * It is language-neutral, so there is one of it and not two: nothing in here is
 * prose.
 */

import { examples, totals } from './examples-model.ts';
import { shelfOf } from './descriptions.ts';

export function examplesIndex(): string {
  const body = {
    generated_from: 'examples/',
    count: examples().length,
    totals: totals(),
    examples: examples().map((example) => ({
      slug: example.slug,
      kind: example.kind,
      url: `/examples/${example.slug}/`,
      group: shelfOf(example.slug),
      dir: example.dir,
      lines: example.lines,
      modules: example.modules.length,
      entry: example.modules[0].path,
      files: example.fingerprint.files,
      hash: example.fingerprint.hash,
      reproduce: example.reproduce,
      externs: example.externs,
      links: example.links,
      packages: example.packages,
      threads: example.threads,
      reads_file: example.readsFile,
      writes_file: example.writesFile,
      uses_args: example.usesArgs,
      args: example.args,
      stdin: example.stdinPath,
      command: example.command.replace(/^\$ /, ''),
      has_output: example.expectation !== null,
      exit:
        example.expectation === null
          ? null
          : example.expectation.ending.kind === 'exits'
            ? example.expectation.ending.code
            : 0,
    })),
  };

  return `${JSON.stringify(body, null, 2)}\n`;
}
