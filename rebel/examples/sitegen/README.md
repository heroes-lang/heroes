# sitegen

sitegen: a static site generator. A folder holding `site.toml`, a
`content/` folder of Markdown pages with YAML (`---`) or TOML (`+++`) front
matter, a theme under `themes/<name>/` and a `static/` folder is written to
its output folder as a site: each page in each of its languages, with its
dates and numbers written as the language writes them; a home page, a list
for each section, an A to Z index ordered as the language orders text, and
a page for each tag; the images scaled to thumbnails and converted to a
kind a browser shows; a sitemap and an Atom feed for each language; and a
Brotli copy beside every text file. Its watch mode builds again after each
change, running only the jobs the changed files feed.

    sitegen build <dir>    write the site of <dir> once
    sitegen watch <dir>    write it, then again after every change, until Ctrl-C
    sitegen demo [<dir>]   write the bundled site (build/sitegen-demo), build it,
                           list its files, and watch three edits

The demo is this program's test: `main.args` runs it and its transcript is
`main.expected`. A command that fails prints `sitegen: ` and the reason,
which names the file at fault, and exits 1; arguments it cannot read exit
2.

What it uses of Rebel, and for what:

- `markdown` reads each page; `template` (its HTML mode) sets it in the
  theme's layouts and partials; `html` reads the page again to give its
  headings ids, list its contents, point its links and show its images as
  thumbnails; `yaml` and `toml` read the front matter and `site.toml`.
- `i18n` reads the theme's gettext catalogs and their plural forms;
  `i18n/locales` makes each language's tag canonical and finds its script's
  direction; `i18n/dates` and `i18n/numbers` write dates, counts and sizes
  as the language does; `i18n/collate` orders the A to Z index.
- `unicode/normalize` and `unicode/segment` make slugs from titles and
  heading ids, count words and cut summaries at sentence boundaries;
  `url` parses, resolves and escapes links; `url/idna` writes the base
  URL's host and every link to an internationalized domain in ASCII.
- `image` scales and converts pictures, read and written by `image/png`,
  `image/jpeg` (its EXIF orientation applied), `image/gif` (an animated
  thumbnail), `image/webp` (lossless), `image/bmp`, `image/netpbm`,
  `image/tiff` (its page's orientation applied) and `image/ico` (an
  icon's largest image).
- `xml` writes the sitemap and the feeds; `compress/brotli` the
  precompressed copies; `crypto` fingerprints sources and outputs so only
  changed files are rebuilt and written.
- `fs` and `path` find, read and write the files; `fs/watch` tells the
  watch mode what changed; `os` catches Ctrl-C; `time` reads dates in the
  site's zone; `flags` reads the command line; `strings`, `bytes`, `lists`
  and `json` hold the rest together.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/sitegen/main.hero -- demo build/sitegen-demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 37 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`compress/brotli`](https://heroes-lang.org/rebel/compress/brotli/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`fs/watch`](https://heroes-lang.org/rebel/fs/watch/), [`html`](https://heroes-lang.org/rebel/html/), [`i18n`](https://heroes-lang.org/rebel/i18n/), [`i18n/collate`](https://heroes-lang.org/rebel/i18n/collate/), [`i18n/dates`](https://heroes-lang.org/rebel/i18n/dates/), [`i18n/locales`](https://heroes-lang.org/rebel/i18n/locales/), [`i18n/numbers`](https://heroes-lang.org/rebel/i18n/numbers/), [`image`](https://heroes-lang.org/rebel/image/), [`image/bmp`](https://heroes-lang.org/rebel/image/bmp/), [`image/gif`](https://heroes-lang.org/rebel/image/gif/), [`image/ico`](https://heroes-lang.org/rebel/image/ico/), [`image/jpeg`](https://heroes-lang.org/rebel/image/jpeg/), [`image/netpbm`](https://heroes-lang.org/rebel/image/netpbm/), [`image/png`](https://heroes-lang.org/rebel/image/png/), [`image/tiff`](https://heroes-lang.org/rebel/image/tiff/), [`image/webp`](https://heroes-lang.org/rebel/image/webp/), [`json`](https://heroes-lang.org/rebel/json/), [`lists`](https://heroes-lang.org/rebel/lists/), [`markdown`](https://heroes-lang.org/rebel/markdown/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`strings`](https://heroes-lang.org/rebel/strings/), [`template`](https://heroes-lang.org/rebel/template/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`unicode/normalize`](https://heroes-lang.org/rebel/unicode/normalize/), [`unicode/segment`](https://heroes-lang.org/rebel/unicode/segment/), [`url`](https://heroes-lang.org/rebel/url/), [`url/idna`](https://heroes-lang.org/rebel/url/idna/), [`xml`](https://heroes-lang.org/rebel/xml/), [`yaml`](https://heroes-lang.org/rebel/yaml/).

## Its folders

10 folders, 34 files and 6,636 lines, this page aside; of them 31 modules, `.hero` files, of 6,444 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 369 | 1 | 177 |
| `content/` | 4 | 923 | 4 | 923 |
| `demo/` | 4 | 1,159 | 4 | 1,159 |
| `locale/` | 2 | 295 | 2 | 295 |
| `media/` | 2 | 443 | 2 | 443 |
| `output/` | 4 | 423 | 4 | 423 |
| `pipeline/` | 5 | 894 | 5 | 894 |
| `render/` | 5 | 1,207 | 5 | 1,207 |
| `site/` | 3 | 683 | 3 | 683 |
| `watch/` | 1 | 240 | 1 | 240 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
