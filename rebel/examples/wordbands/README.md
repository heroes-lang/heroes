# wordbands

The twin of `examples/wordbands/`: words counted in text each thread writes
for itself, written with Rebel. It prints byte for byte what
`examples/wordbands/` prints, so that program's `main.expected` is this
one's test too.

    heroes run rebel/examples/wordbands/main.hero

What Rebel does here that `examples/wordbands/` writes for itself:

- `threads.run_all` starts one thread a band, at most `BANDS` at once, waits
  for every one and answers their counts in order, in place of that
  program's own `extern "hero_os.h"` group (`hero_thread_spawn`,
  `hero_thread_join`) and the list of handles it joins by hand;
- `strings.words` cuts a band's text into its words, in place of its
  `strings.hero` (`fields`, `space_at`);
- `maps.counts` counts each word, in place of its loop over a map;
- `math.sum_i64` adds the bands' counts, failing `does_not_fit` where the sum
  leaves an `i64` rather than aborting.

What stays this program's own is the work each thread does: it builds the
text it reads from the one `i64` it is given, so nothing is shared, and only
a number crosses back.

## Its original

The twin of [`examples/wordbands/`](../../examples/wordbands/), the compiler's own program, which takes no library. Run from the repository's root with no word, as that program is, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run rebel/examples/wordbands/main.hero

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 4 faces on its `packages` line, each linked here to its reference: [`maps`](https://heroes-lang.org/rebel/maps/), [`math`](https://heroes-lang.org/rebel/math/), [`strings`](https://heroes-lang.org/rebel/strings/), [`threads`](https://heroes-lang.org/rebel/threads/).

## Its folders

1 folder, 2 files and 107 lines, this page aside; of them 1 module, `.hero` files, of 104 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 107 | 1 | 104 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
