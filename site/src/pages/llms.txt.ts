/**
 * `/llms.txt`, the page addressed to a model, generated rather than copied.
 *
 * It lived in `public/` and was copied into `dist/` untouched, which is how it
 * went on saying the specification "stays below 4096 tokens" after the ceiling
 * moved to 6144 and the real count to 5369: nothing in the build read it, so
 * nothing went red. The marketing seat found it on 2026-09-09, on the one file
 * whose reader is the audience the language is for. The prose stays a plain
 * text file, `site/src/llms.txt`, and the three numbers in it are placeholders
 * the build fills from the suite that pins them, the same way `{{version}}`
 * and `{{realTokens}}` are filled in the HTML fragments. A placeholder the
 * build does not know is a build error, never a brace shipped to a reader.
 */
import type { APIRoute } from 'astro';
import { readText } from '../lib/repo.ts';
import { ceilingK, specReal } from '../lib/claims.ts';

const SOURCE = 'site/src/llms.txt';

function filled(): string {
  const real = specReal();
  const values: Record<string, string> = {
    ceilingK: ceilingK(),
    realTokens: String(real.tokens),
    realModel: real.model,
  };
  const text = readText(SOURCE).replace(/\{\{(\w+)\}\}/g, (whole, name: string) => {
    const value = values[name];
    if (value === undefined) throw new Error(`${SOURCE}: no value for the placeholder ${whole}.`);
    return value;
  });
  if (text.includes('{{')) throw new Error(`${SOURCE}: a placeholder survived the fill.`);
  return text;
}

export const GET: APIRoute = () =>
  new Response(filled(), {
    headers: { 'content-type': 'text/plain; charset=utf-8' },
  });
