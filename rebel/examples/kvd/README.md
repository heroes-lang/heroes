# kvd

kvd: a small key-value server in Redis's manner, and its client. It
speaks RESP2, Redis's protocol, over TCP, and it serves strings, lists,
hashes, sets and sorted sets, keys that expire, transactions with
`WATCH`, publish and subscribe, `SCAN`, `INFO`, and keeps its data in a
snapshot and an append-only file.

Run for real:

    kvd serve --port 6379 --dir ./data --appendfsync everysec \
        --requirepass s3cret
    kvd cli --port 6379 --password s3cret SET greeting hello
    kvd cli --port 6379 --script commands.txt
    kvd cli --port 6379 --pipeline < commands.txt
    kvd cli --port 6379

`kvd serve` listens on 127.0.0.1 unless `--bind` says otherwise, writes
every change to its log unless `--no-appendonly`, and stops at
`SHUTDOWN` or Ctrl-C. `kvd cli` with words runs them as one command, with
`--script` runs a file of command lines, with `--pipeline` sends the
standard input's lines in one write, and with neither reads commands from
the standard input, a prompt shown where it is a terminal. `kvd demo`
plays the session on its standard input against a server on a thread of
its own, on the loopback, its clock manual, and prints the transcript.

The Rebel packages it uses, and for what:
- `net` and `poll`: the listener and one loop serving every connection;
  `redis`: the client, the demo's clients, and the RESP reader the server
  cuts requests with; `io`: the connections' bytes and the prompt.
- `containers/indexed`: the keyspace, hashes, sets and every map that
  loses keys; `containers/deque`: lists, and the skip list's free nodes;
  `sets`: `SINTER`, `SUNION`, `SDIFF`; `random`: the active sweep's draws
  and the skip list's heights, seeded.
- `fs` and `path`: the snapshot written in one step and the log's parts;
  `hash`: the snapshot's CRC-32; `math`: a score's 64 bits; `bytes`: the
  snapshot's numbers and every key; `numbers`: numbers read and written.
- `time`: the system's clock and the poll set's timer; `crypto`: the
  password compared in constant time; `os`: Ctrl-C; `flags`: this command
  line; `strings`: the script's lines.

## Its demo

Run from the repository's root with the words of `main.args` and `main.stdin` on its input, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/kvd/main.hero -- demo < rebel/examples/kvd/main.stdin

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 19 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`containers/deque`](https://heroes-lang.org/rebel/containers/deque/), [`containers/indexed`](https://heroes-lang.org/rebel/containers/indexed/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`io`](https://heroes-lang.org/rebel/io/), [`math`](https://heroes-lang.org/rebel/math/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`poll`](https://heroes-lang.org/rebel/poll/), [`random`](https://heroes-lang.org/rebel/random/), [`redis`](https://heroes-lang.org/rebel/redis/), [`sets`](https://heroes-lang.org/rebel/sets/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/).

## Its folders

8 folders, 36 files and 8,187 lines, this page aside; of them 32 modules, `.hero` files, of 7,771 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 629 | 1 | 213 |
| `cli/` | 2 | 369 | 2 | 369 |
| `commands/` | 8 | 2,146 | 8 | 2,146 |
| `demo/` | 1 | 561 | 1 | 561 |
| `persist/` | 2 | 725 | 2 | 725 |
| `protocol/` | 4 | 641 | 4 | 641 |
| `server/` | 10 | 1,765 | 10 | 1,765 |
| `store/` | 4 | 1,351 | 4 | 1,351 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
