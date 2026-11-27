# langserver

langserver: a language server for Kite, a small statically typed language
of records, functions and `let` bindings that this program defines, its
lexer, parser, scopes and type checker included. It speaks the Language
Server Protocol over its standard streams, JSON-RPC 2.0 framed by
`Content-Length` headers: `initialize` and the lifecycle, documents kept
in step by incremental sync with places counted in UTF-16 (or UTF-8 where
the client offers it), diagnostics published after every change, hover,
go to definition, find references, rename, completion, the document's
outline, formatting and quick fixes.

Kite, as the server reads it:

    # The square of the distance between two points.
    fn dist2(a: Point, b: Point) -> Int {
        let dx = a.x - b.x;
        let dy = a.y - b.y;
        dx * dx + dy * dy
    }

Run for real:

    langserver serve                  # what an editor starts
    langserver check shapes.kite      # findings as a compiler prints them
    langserver format messy.kite -w   # rewrites it in the one layout

An editor starts `langserver serve` for files ending `.kite`: in Neovim,
`vim.lsp.start({ name = "kite", cmd = { "langserver", "serve" } })`; in
Helix, a `[language-server.kite]` with `command = "langserver"` and
`args = ["serve"]`.

`langserver demo <folder>` plays a whole session over the samples of the
folder: a client stand-in starts this same program's `serve` as its
child, over two pipes, and prints every message as it crosses them.

The packages of Rebel it uses, and what for:

- `jsonrpc`, `json`: the protocol's messages, their framing and their
  values;
- `io`, `process`, `os`: the standard streams, and the server started as
  the demo's child;
- `scanner`: Kite's tokens;
- `diff`: the nearest name offered for one misspelt, and formatting sent
  as the fewest line edits;
- `graphs`: the records that hold themselves by value;
- `unicode`: places counted in UTF-16 or UTF-8;
- `lists`, `maps`, `strings`: lists sorted and joined, documents closed,
  text searched and padded;
- `flags`, `path`, `errors`: the command line, the samples' paths, a
  failure dropped on purpose.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/langserver/main.hero -- demo rebel/examples/langserver/samples

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 15 faces on its `packages` line, each linked here to its reference: [`diff`](https://heroes-lang.org/rebel/diff/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`graphs`](https://heroes-lang.org/rebel/graphs/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`jsonrpc`](https://heroes-lang.org/rebel/jsonrpc/), [`lists`](https://heroes-lang.org/rebel/lists/), [`maps`](https://heroes-lang.org/rebel/maps/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`process`](https://heroes-lang.org/rebel/process/), [`scanner`](https://heroes-lang.org/rebel/scanner/), [`strings`](https://heroes-lang.org/rebel/strings/), [`unicode`](https://heroes-lang.org/rebel/unicode/).

## Its folders

8 folders, 40 files and 6,401 lines, this page aside; of them 35 modules, `.hero` files, of 6,161 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 275 | 1 | 81 |
| `app/` | 1 | 107 | 1 | 107 |
| `cli/` | 1 | 95 | 1 | 95 |
| `demo/` | 2 | 493 | 2 | 493 |
| `lang/` | 13 | 3,276 | 13 | 3,276 |
| `lsp/` | 15 | 1,798 | 15 | 1,798 |
| `samples/` | 2 | 46 | 0 | 0 |
| `text/` | 2 | 311 | 2 | 311 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
