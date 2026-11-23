# assembler

The twin of `examples/assembler/`: a tiny assembler, and the machine it
assembles for, written with Rebel. It prints byte for byte what
`examples/assembler/` prints from the same arguments, so that program's
`main.expected` is this one's test too.

    heroes run examples-rebel/assembler/main.hero -- examples/assembler/sample.asm
    heroes run examples-rebel/assembler/main.hero -- examples/assembler/sample.asm listing

    main.hero       the program: arguments, the file, the two views
    machine.hero    four registers, a program counter, and fuel
    assemble.hero   two passes: where the labels are, then the words
    program.hero    the instruction set, and one line of assembly

What Rebel does here that `examples/assembler/` writes for itself:

- `strings.lines`, `trim`, `split_once` and `split` take a line of assembly
  apart, in place of that program's `strings.hero` (`split_lines`,
  `trim_space`, `space_ending`, `space_at`, `split_once`, `first_at`,
  `holds_at`) and its `split_on`;
- `numbers.parse_int` reads an immediate operand and a register's digit, in
  place of its `numbers.hero` (`atoi`, its two cut-off constants and
  `quoted`) and of the byte subtraction its `register_of` reads the digit
  with;
- `math.add_i64` and `math.subtract_i64` add and subtract in the machine,
  failing `does_not_fit` where its `+` and `-` would leave an `i64`.

What stays this program's own is the machine and its assembler: one set of
constants for the encoding and the decoding, two passes because a jump may
name a label further down, and a run bounded by fuel.

## Its original

The twin of [`examples/assembler/`](../../examples/assembler/), the compiler's own program, which takes no library. Run from the repository's root with that program's words, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run examples-rebel/assembler/main.hero -- examples/assembler/sample.asm

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 3 faces on its `packages` line, each linked here to its reference: [`math`](https://heroes-lang.org/rebel/math/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

1 folder, 5 files and 727 lines, this page aside; of them 4 modules, `.hero` files, of 724 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 727 | 4 | 724 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
