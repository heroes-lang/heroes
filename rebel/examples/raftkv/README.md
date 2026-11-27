# raftkv

raftkv: Raft consensus and a replicated key-value store, run under a
deterministic simulated network. Each node is a real Raft server (leader
election with randomised timeouts, log replication and commit, snapshots
and log compaction with InstallSnapshot in parts, membership changes one
node at a time, linearizable reads by ReadIndex) that keeps its term, its
vote, its log and its snapshot in files of its own folder, framed and
checksummed, and comes back from them alone after a crash, even one in
the middle of a write. The network between the nodes is a scheduler that
delivers, delays, reorders, loses, doubles and damages messages as a
seeded generator decides, and cuts the cluster into partitions; a checker
asks Raft's five safety properties of every node after every step. The
same seed gives the same run, byte for byte, on every machine.

Run for real:

    raftkv run scenario.toml              # a scenario's transcript
    raftkv run scenario.toml --seed 9     # the same steps, other timings
    raftkv fuzz --runs 100 --span 20s     # faults drawn from 100 seeds
    raftkv inspect build/raftkv-run/node-1   # what a node's disk holds

A scenario is TOML: the cluster's size and seed, the network's delays and
chances of loss, the nodes' settings, and the steps (`leader`, `op`,
`partition`, `heal`, `crash`, `restart`, `add`, `remove`, `campaign`,
`run`, `show`, `say`, `check`); `scenario/script.hero` says each. A fuzz
run that fails prints its seed, which reproduces it.

`raftkv demo <folder>` runs the scenarios of the folder in the order of
their names: an election, writes and a linearizable read; a partition
that leaves the old leader unable to commit or to read while the majority
elects a second leader; the healing, with the old leader's entries cut;
a follower that crashes in the middle of a write and catches up from the
leader's snapshot; a node added and one removed; then a stale node that
loses its election, and the same script with a planted bug (votes given
without the up-to-date check), which the checker catches. It reads one
node's disk back, and ends with a few runs of the fuzzer.

The packages of Rebel it uses, and what for:

- `random`: the election timeouts, the network's delays and chances, the
  fuzzer's faults, all from seeds;
- `containers/heap`: the network's queue of messages in flight;
- `bytes`, `hash`: the wire format's varints and the frames' CRC-32;
- `fs`, `path`: each node's folder, its files written atomically or
  appended, cut back after a torn write;
- `json`: a snapshot's store; `toml`: the scenarios; `time`: their spans
  and the transcript's clock;
- `flags`: the command line; `errors`: failures told with their step;
- `strings`, `numbers`, `maps`, `math`, `lists`, `io`: text, numbers,
  keys removed, bounds, the standard output.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/raftkv/main.hero -- demo rebel/examples/raftkv/scenarios

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 17 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`containers/heap`](https://heroes-lang.org/rebel/containers/heap/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`lists`](https://heroes-lang.org/rebel/lists/), [`maps`](https://heroes-lang.org/rebel/maps/), [`math`](https://heroes-lang.org/rebel/math/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`path`](https://heroes-lang.org/rebel/path/), [`random`](https://heroes-lang.org/rebel/random/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/).

## Its folders

13 folders, 36 files and 6,517 lines, this page aside; of them 30 modules, `.hero` files, of 6,025 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 337 | 1 | 126 |
| `check/` | 1 | 235 | 1 | 235 |
| `cli/` | 1 | 158 | 1 | 158 |
| `demo/` | 1 | 79 | 1 | 79 |
| `disk/` | 2 | 345 | 2 | 345 |
| `fuzz/` | 1 | 200 | 1 | 200 |
| `kv/` | 3 | 452 | 3 | 452 |
| `raft/` | 11 | 2,143 | 11 | 2,143 |
| `scenario/` | 2 | 561 | 2 | 561 |
| `scenarios/` | 3 | 281 | 0 | 0 |
| `sim/` | 3 | 1,014 | 3 | 1,014 |
| `support/` | 1 | 29 | 1 | 29 |
| `wire/` | 3 | 683 | 3 | 683 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
