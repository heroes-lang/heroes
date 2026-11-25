# logs

The twin of `examples/logs/`: a log summariser, a file of lines in and a
page a person can read out, written with Rebel. It prints byte for byte what
`examples/logs/` prints from the same arguments, and ends with the same
status, so that program's `main.expected` is this one's test too.

    heroes run rebel/examples/logs/main.hero -- examples/logs/sample.log
    heroes run rebel/examples/logs/main.hero -- examples/logs/sample.log ERROR

    main.hero      the program: arguments, the file, the page, the status
    summary.hero   counting, and the ordering a count wants
    entry.hero     one line, taken apart, and the lines that are not one
    mask.hero      the levels as a set of bits

It ends **1** when the log holds an error, which is the whole point of
running it from a script: the exit code answers "did anything go wrong last
night" without anyone reading the page.

What Rebel does here that `examples/logs/` writes for itself:

- `strings.lines`, `trim`, `words_n`, `split_once`, `contains` and `split`
  take a line apart, in place of that program's `strings.hero`
  (`split_lines`, `trim_space`, `space_ending`, `space_at`, `contains`,
  `first_at`, `holds_at`, `split_once`) and its `first_words` and `split_on`;
- `maps.counts` tallies, `maps.pick` orders a tally by name and
  `maps.most_common` ranks it by count, ties left in name order, in place of
  its `tally`, `ranked` and `without`;
- `math.count_ones` counts the levels in a mask, in place of its loop over
  the bits.

What stays this program's own is the page, the reader that **skips and
counts** what it cannot parse rather than refusing the file, and the mask of
levels, one bit each.

## Its original

The twin of [`examples/logs/`](../../examples/logs/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run rebel/examples/logs/main.hero -- examples/logs/sample.log

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 3 faces on its `packages` line, each linked here to its reference: [`maps`](https://heroes-lang.org/rebel/maps/), [`math`](https://heroes-lang.org/rebel/math/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

1 folder, 5 files and 605 lines, this page aside; of them 4 modules, `.hero` files, of 602 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 605 | 4 | 602 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
