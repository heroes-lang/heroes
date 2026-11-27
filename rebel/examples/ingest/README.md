# ingest

ingest: an event pipeline in the manner of Kafka Connect's S3 sink and
a Kafka Streams aggregation, in small. A member of a Kafka consumer
group reads the events producers write (JSON or MessagePack, by their
`content-type`), cuts them into tumbling windows by their event time
with Kafka Streams' grace for late events, and writes each closed window
per topic to S3 twice under date-partitioned keys: an LZ4 copy for the
hot path and a Zstandard copy for the archive, compressed on threads and
checked against the batch before each put. The window's counts, sums and
percentiles go to PostgreSQL through migrations the pipeline owns, with
a ledger of every object, in one transaction; a put that fails after the
retries' backoff is kept in that transaction as a job of the jobs store,
which the `replay` command writes later. Offsets are committed only past
what is written. A day's archive is exported cold as one bzip2 object.
The pipeline serves its own metrics to Prometheus.

Run for real:

    heroes build rebel/examples/ingest/main.hero -o ingest
    ./ingest check pipeline.toml            # read the file, say what it names
    ./ingest migrate pipeline.toml          # apply the schema
    ./ingest run pipeline.toml              # consume until Ctrl-C or SIGTERM
    ./ingest replay pipeline.toml           # write the writes kept as jobs
    ./ingest export pipeline.toml 2026-03-14

`config/loader.hero` documents the file. The demo, `./ingest demo`,
starts on loopback a Kafka broker, an S3 service and a PostgreSQL server
it carries (`standins/`), produces a scripted morning of events, runs
the pipeline until it has caught up, exports, replays the one write the
S3 service refuses on purpose, exports again, and prints an account
whose transcript is `main.expected`.

Rebel's packages, and what for:

- `kafka`: the consumer group, the dead-letter topic, the demo's producer;
- `s3`: the puts, the listings and reads of the export; its signing pieces
  check the stand-in's signatures;
- `compress/lz4`, `compress/zstd`, `compress/bzip2`: the three tiers;
- `msgpack`, `json`: the events' payloads, the jobs' payloads, the lines;
- `stats`: each window's percentiles;
- `sql`, `sql/postgres`, `sql/migrate`: the aggregates, the ledger, the
  schema; `jobs`, `jobs/postgres_store`: the writes kept for a replay;
- `scram`: the logins to PostgreSQL and Kafka, both sides;
- the language's own threads (`spawn`, mailboxes), no package: the
  compression of a window's copies, the demo's services;
- `retries`: the backoff of every put; `metrics`: the pipeline's own
  metrics; `http`, `net`, `poll`, `io`: serving them, the stand-ins;
- `toml`: the settings' file; `errors`: what a failure was doing, said
  before its message; `crypto`, `hash`: the checks of every copy, the
  stand-ins' SCRAM, SigV4 and CRC-32C;
- `time`, `bytes`, `strings`, `numbers`, `lists`, `math`, `os`, `url`:
  throughout.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/ingest/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 32 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`compress/bzip2`](https://heroes-lang.org/rebel/compress/bzip2/), [`compress/lz4`](https://heroes-lang.org/rebel/compress/lz4/), [`compress/zstd`](https://heroes-lang.org/rebel/compress/zstd/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`errors`](https://heroes-lang.org/rebel/errors/), [`hash`](https://heroes-lang.org/rebel/hash/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`jobs`](https://heroes-lang.org/rebel/jobs/), [`jobs/postgres_store`](https://heroes-lang.org/rebel/jobs/postgres_store/), [`json`](https://heroes-lang.org/rebel/json/), [`kafka`](https://heroes-lang.org/rebel/kafka/), [`lists`](https://heroes-lang.org/rebel/lists/), [`math`](https://heroes-lang.org/rebel/math/), [`metrics`](https://heroes-lang.org/rebel/metrics/), [`msgpack`](https://heroes-lang.org/rebel/msgpack/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`poll`](https://heroes-lang.org/rebel/poll/), [`retries`](https://heroes-lang.org/rebel/retries/), [`s3`](https://heroes-lang.org/rebel/s3/), [`scram`](https://heroes-lang.org/rebel/scram/), [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/migrate`](https://heroes-lang.org/rebel/sql/migrate/), [`sql/postgres`](https://heroes-lang.org/rebel/sql/postgres/), [`stats`](https://heroes-lang.org/rebel/stats/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`url`](https://heroes-lang.org/rebel/url/).

## Its folders

12 folders, 68 files and 14,550 lines, this page aside; of them 65 modules, `.hero` files, of 14,435 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 205 | 1 | 90 |
| `batching/` | 2 | 305 | 2 | 305 |
| `codecs/` | 1 | 237 | 1 | 237 |
| `config/` | 2 | 393 | 2 | 393 |
| `core/` | 4 | 738 | 4 | 738 |
| `demo/` | 5 | 701 | 5 | 701 |
| `events/` | 2 | 273 | 2 | 273 |
| `ops/` | 2 | 237 | 2 | 237 |
| `replay/` | 3 | 363 | 3 | 363 |
| `standins/` | 36 | 10,541 | 36 | 10,541 |
| `storage/` | 2 | 197 | 2 | 197 |
| `store/` | 5 | 360 | 5 | 360 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
