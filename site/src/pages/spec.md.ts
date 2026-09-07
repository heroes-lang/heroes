/**
 * The specification as the file, rather than as a page around it.
 *
 * One of it and not one per edition: it is the language, and the language has
 * no editions. The page at `/spec/` is the same bytes with the counts and the
 * reasoning around them; this is what a `curl` wants, and what the readers this
 * site names in its own hero would rather have than 17 KB of HTML wrapping
 * 12 KB of markdown.
 */
import type { APIRoute } from 'astro';
import { readText } from '../lib/repo.ts';

export const GET: APIRoute = () =>
  new Response(readText('spec/heroes-spec.md'), {
    headers: { 'content-type': 'text/plain; charset=utf-8' },
  });
