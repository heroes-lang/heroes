/**
 * Every script the build writes, read back and held to the site's rule.
 *
 * The rule (site/README.md, the paragraph that begins "JavaScript is never the
 * only way"): a page works in full without scripts; a script may make one
 * thing easier; it is written here, sets no cookie, touches no storage,
 * fingerprints nobody, loads nothing external and sends nothing anywhere. A
 * rule about what a visitor receives is checked on what a visitor receives, so
 * this walks the OUTPUT directory rather than the sources, the way the sitemap
 * audit does: a script that reached `dist/` through a path nobody thought of
 * is exactly the one a source-side check would miss.
 *
 * What counts as a script here: every `<script` tag in every HTML file. The
 * structured-data blocks are `type="application/ld+json"` and run nothing, so
 * they pass by their type. Anything else must carry `data-enhances="<what>"`,
 * naming the one thing it makes easier, must be inline (no `src`, so nothing is
 * fetched to run), and must not mention the four names that write to a
 * visitor's browser. The word list is deliberately blunt: a false refusal costs
 * a build, a false pass costs the rule.
 */

import { readdirSync, readFileSync, statSync } from 'node:fs';
import { join } from 'node:path';

const WRITES_TO_THE_BROWSER = ['cookie', 'localStorage', 'sessionStorage', 'indexedDB'];

// The scripts this site has asked for, by the name each one declares. A script
// that declares a name not listed here is a script nobody decided on.
const DECLARED = new Set(['spec-copy']);

function htmlFiles(dir: string): string[] {
  const out: string[] = [];
  for (const entry of readdirSync(dir)) {
    const full = join(dir, entry);
    if (statSync(full).isDirectory()) out.push(...htmlFiles(full));
    else if (entry.endsWith('.html')) out.push(full);
  }
  return out;
}

/**
 * Throws on the first script that breaks the rule; returns the line the build
 * log prints, so a log shows the check ran and over how many pages.
 */
export function assertScriptsHonest(distDir: string): string {
  const files = htmlFiles(distDir);
  let pages = 0;
  let enhancing = 0;
  const seen = new Set<string>();
  const tag = /<script\b([^>]*)>([\s\S]*?)<\/script>/g;

  for (const file of files) {
    pages += 1;
    const html = readFileSync(file, 'utf8');
    for (const match of html.matchAll(tag)) {
      const attrs = match[1];
      const body = match[2];
      if (/type\s*=\s*"application\/ld\+json"/.test(attrs)) continue;

      const declared = /data-enhances\s*=\s*"([^"]+)"/.exec(attrs);
      const where = file.slice(distDir.length);
      if (declared === null) {
        throw new Error(
          `${where}: a <script> that declares nothing.\n` +
            `  every script names the one thing it makes easier, data-enhances="...", ` +
            `and is listed in src/lib/scripts.ts (site/README.md, "JavaScript is never the only way").`
        );
      }
      if (!DECLARED.has(declared[1])) {
        throw new Error(`${where}: a script declares "${declared[1]}", which nobody decided on.`);
      }
      if (/\bsrc\s*=/.test(attrs)) {
        throw new Error(`${where}: the "${declared[1]}" script has a src, and a script here is inline or it is not here.`);
      }
      for (const word of WRITES_TO_THE_BROWSER) {
        if (body.includes(word)) {
          throw new Error(
            `${where}: the "${declared[1]}" script mentions ${word}.\n` +
              `  nothing the site serves writes to the visitor's browser (site/README.md).`
          );
        }
      }
      enhancing += 1;
      seen.add(declared[1]);
    }
  }

  for (const name of DECLARED) {
    if (!seen.has(name)) {
      throw new Error(`the "${name}" script is declared in src/lib/scripts.ts and reached no page.`);
    }
  }
  return `scripts: ${enhancing} enhancing over ${pages} pages, ${seen.size} declared, none writing to the browser`;
}
