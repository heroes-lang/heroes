# markdown

The twin of `examples/markdown/`: Markdown to plain text, written with
Rebel. It prints byte for byte what `examples/markdown/` prints from the
same arguments, so that program's `main.expected` is this one's test too.

    heroes run rebel/examples/markdown/main.hero -- examples/markdown/sample.md
    heroes run rebel/examples/markdown/main.hero -- examples/markdown/sample.md outline

    main.hero     the program: arguments, the file, the two views
    render.hero   blocks out, lines in
    inline.hero   one block's inlines, with their markup removed

What Rebel does here that `examples/markdown/` writes for itself:

- `markdown.parse_with` reads the text as CommonMark 0.31.2 into a tree of
  nodes, in place of that program's own block grammar in its `blocks.hero`
  (its `Block` variant, the line tests, the numbered marker, the paragraph
  and fence readers) and its inline scanner in `inline.hero`;
- `strings.lines` cuts a code block into its lines, in place of its
  `strings.hero` (`split_lines`, `trim_space`, `has_prefix`).

What stays this program's own is the plain-text page: a heading underlined,
the list markers rewritten, a quote after `|`, code indented, the blank line
between blocks that are not one run, and the outline view.

Where the two read a text otherwise, none of it in that program's sample:
this one reads CommonMark, so lines of one quote are one paragraph (`> a`
then `> b` is `| a b`, where that program writes two quoted lines), a list
numbers its items from its first (`1.` then `1.` is 1 and 2), a list and a
quote nest, an autolink `<http://x>` is a link, and a link's destination is
percent-encoded; that program reads each line by its first characters alone.

## Its original

The twin of [`examples/markdown/`](../../examples/markdown/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run rebel/examples/markdown/main.hero -- examples/markdown/sample.md

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 2 faces on its `packages` line, each linked here to its reference: [`markdown`](https://heroes-lang.org/rebel/markdown/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

1 folder, 4 files and 431 lines, this page aside; of them 3 modules, `.hero` files, of 428 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 431 | 3 | 428 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
