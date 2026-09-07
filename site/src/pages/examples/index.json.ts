/**
 * The corpus as data. One of it, not one per edition: nothing in it is prose.
 */
import type { APIRoute } from 'astro';
import { examplesIndex } from '../../lib/examples-json.ts';

export const GET: APIRoute = () =>
  new Response(examplesIndex(), { headers: { 'content-type': 'application/json; charset=utf-8' } });
