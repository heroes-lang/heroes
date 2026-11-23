# worker

worker: a background job system on Redis, as Sidekiq and Celery are.
Producers enqueue typed jobs (`worker enqueue`); a supervisor and a fleet
of worker threads (`worker run`) claim them, run each kind's function,
retry the failures with exponential backoff and seeded jitter, keep the
jobs that fail for good as dead letters (`worker dead`), enqueue recurring
jobs on cron schedules, give back the jobs of workers that died, count and
time every attempt in metrics served at `/metrics`, trace each job from its
enqueue to its last attempt over OTLP, and write rotating log files.

Run for real against a Redis 5 or later:

    worker run --redis redis://:password@cache:6379/0 --queue webhooks \
        --queue default --workers 8 --otlp http://collector:4318 \
        --metrics 127.0.0.1:9464 --schedule 'purge_finished@*/15 * * * *'
    worker enqueue deliver_webhook '{"url": "https://example.org/hook", "body": {"n": 1}}' --queue webhooks
    worker stats; worker dead; worker retry 42; worker stop

`worker standin --port 6380` serves the loopback stand-in of Redis alone,
for trying the system where no Redis runs. `worker demo` runs the scripted
demo against loopback stand-ins of Redis (RESP and Lua scripting), an
OpenTelemetry collector and webhook endpoints, on a clock moved by hand,
and prints a fixed account.

The packages of Rebel it uses, and what for:

- `jobs`, `jobs/redis_store`: the jobs, their states and schedules, kept in
  Redis by scripts the server runs whole;
- `redis`: the store's connection, its clock and the fleet's stop key, and
  the stand-in's replies;
- `retries`: the exponential waits between attempts, spread by `random`'s
  seeded generator;
- `threads`: the fleet of workers and the demo's driver;
- `poll`: the stand-ins' loop over every connection, and `/metrics` served
  while the supervisor waits;
- `metrics`, `tracing`, `otlp`: counters, gauges and histograms; a span per
  enqueue and per attempt; both exported to a collector;
- `log`, `log/files`: the lines of each thread's log, in files set aside by
  size and day;
- `containers/heap`: the scheduler's timers;
- `time`: the store's instants and durations, cron schedules;
- `errors`: context on failures, told in one line;
- `uuid`: the run's `service.instance.id`;
- `http`, `url`, `json`, `protobuf`, `compress`, `crypto`, `fs`, `path`,
  `flags`, `io`, `net`, `os`, `bytes`, `strings`, `numbers`, `lists`,
  `maps`, `math`: the kinds' work, the stand-ins, the command line.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run examples-rebel/worker/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../packages/rebel/0.1.0/files` and lists 34 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`compress`](https://heroes-lang.org/rebel/compress/), [`containers/heap`](https://heroes-lang.org/rebel/containers/heap/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`jobs`](https://heroes-lang.org/rebel/jobs/), [`jobs/redis_store`](https://heroes-lang.org/rebel/jobs/redis_store/), [`json`](https://heroes-lang.org/rebel/json/), [`lists`](https://heroes-lang.org/rebel/lists/), [`log`](https://heroes-lang.org/rebel/log/), [`log/files`](https://heroes-lang.org/rebel/log/files/), [`maps`](https://heroes-lang.org/rebel/maps/), [`math`](https://heroes-lang.org/rebel/math/), [`metrics`](https://heroes-lang.org/rebel/metrics/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`otlp`](https://heroes-lang.org/rebel/otlp/), [`path`](https://heroes-lang.org/rebel/path/), [`poll`](https://heroes-lang.org/rebel/poll/), [`protobuf`](https://heroes-lang.org/rebel/protobuf/), [`random`](https://heroes-lang.org/rebel/random/), [`redis`](https://heroes-lang.org/rebel/redis/), [`retries`](https://heroes-lang.org/rebel/retries/), [`strings`](https://heroes-lang.org/rebel/strings/), [`threads`](https://heroes-lang.org/rebel/threads/), [`time`](https://heroes-lang.org/rebel/time/), [`tracing`](https://heroes-lang.org/rebel/tracing/), [`url`](https://heroes-lang.org/rebel/url/), [`uuid`](https://heroes-lang.org/rebel/uuid/).

## Its folders

14 folders, 52 files and 9,960 lines, this page aside; of them 49 modules, `.hero` files, of 9,665 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 422 | 1 | 127 |
| `cli/` | 2 | 481 | 2 | 481 |
| `demo/` | 4 | 558 | 4 | 558 |
| `logs/` | 1 | 54 | 1 | 54 |
| `ops/` | 2 | 259 | 2 | 259 |
| `pool/` | 3 | 449 | 3 | 449 |
| `queue/` | 4 | 316 | 4 | 316 |
| `schedule/` | 4 | 427 | 4 | 427 |
| `standins/` | 1 | 102 | 1 | 102 |
| `standins/collector/` | 4 | 832 | 4 | 832 |
| `standins/lua/` | 11 | 3,272 | 11 | 3,272 |
| `standins/redis/` | 9 | 2,322 | 9 | 2,322 |
| `standins/webhooks/` | 1 | 199 | 1 | 199 |
| `telemetry/` | 2 | 267 | 2 | 267 |

---

This page is written by `generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
