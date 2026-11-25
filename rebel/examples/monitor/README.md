# monitor

monitor: a system monitor with alerts and a terminal dashboard, in the
family of htop, Netdata and supervisord in small. It runs the services a
configuration names and keeps them running (restarted with a backoff
that doubles, in the order their dependencies give, a failure impairing
whatever depends on it), reads what their processes hold and spend,
watches files and folders for changes, reads a sensor on a serial port,
judges alert rules over every figure with thresholds and durations, runs
a notifier at each alert that fires or resolves, writes its journal and
its services' output to rotating log files, exports its metrics, spans
and journal over OTLP, answers other tools on a control socket, and draws
its dashboard on the terminal.

Run it for real with a configuration file (`conf/reading` shows its
form):

    monitor check --config /etc/monitor.toml
    monitor run --config /etc/monitor.toml --dashboard
    monitor ctl --config /etc/monitor.toml alerts
    monitor ctl --config /etc/monitor.toml ack temperature-high
    monitor ctl --config /etc/monitor.toml restart cache
    monitor bench --config /etc/monitor.toml

`monitor demo` runs the scripted demo: four services (this program again,
as `monitor workload service`), two watched folders it edits, a
thermometer on a pseudo-terminal and an OpenTelemetry collector on
loopback, both stand-ins it serves on its first thread, through fourteen
minutes of a fixed clock; it prints the events, four dashboard frames, the
operator's tools run in other processes, and an account that reads the
same on every run. Where the system makes no pseudo-terminal (Windows),
the thermometer's readings are handed in, and the transcript's last line,
`serial: ...`, is its one line that differs.

The packages of Rebel it uses, and what for:

- `process`, `os`: the services run, fed, ended and reaped; their memory
  and processor time; the machine's processors, memory, load and network;
  the program's own path, its stop on Ctrl-C;
- `ipc`: the control socket and its messages, who is at its other end, and
  the lock that keeps a second monitor of one configuration out;
- `poll`: the one loop over the timer, the services' pipes, the watches
  and the terminal; the stand-ins' loop;
- `serial`: the sensor's port, and the demo's pseudo-terminal;
- `fs/watch`, `fs`, `path`: the watched paths, their snapshots, the log
  folder;
- `metrics`, `otlp`, `tracing`: the registry, its export, the collections'
  spans and the journal as log records;
- `log`, `log/files`: the journal's lines and every log file's rotation;
- `stats`: the figures of a rule's window: mean, 95th percentile, slope;
- `graphs`: the services' start stages, stop order, cycles refused, and
  the failures that propagate;
- `tui`, `term`: the dashboard, its frames and keys; the status table;
- `bench`: the self-benchmark;
- `time`: the monitor's clock, durations, the demo's fixed clock;
- `toml`, `flags`, `json`, `protobuf`, `http`, `net`, `compress`,
  `numbers`, `strings`, `bytes`, `maps`, `math`, `errors`, `io`: the
  configuration, the command line, the control protocol, the collector
  stand-in; the demo's two threads are the language's own (`spawn`).

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/monitor/main.hero -- demo

A line `main.varies` names is printed in another form where another machine decides it, each form written there.

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 33 faces on its `packages` line, each linked here to its reference: [`bench`](https://heroes-lang.org/rebel/bench/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`compress`](https://heroes-lang.org/rebel/compress/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`fs/watch`](https://heroes-lang.org/rebel/fs/watch/), [`graphs`](https://heroes-lang.org/rebel/graphs/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`ipc`](https://heroes-lang.org/rebel/ipc/), [`json`](https://heroes-lang.org/rebel/json/), [`log`](https://heroes-lang.org/rebel/log/), [`log/files`](https://heroes-lang.org/rebel/log/files/), [`maps`](https://heroes-lang.org/rebel/maps/), [`math`](https://heroes-lang.org/rebel/math/), [`metrics`](https://heroes-lang.org/rebel/metrics/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`otlp`](https://heroes-lang.org/rebel/otlp/), [`path`](https://heroes-lang.org/rebel/path/), [`poll`](https://heroes-lang.org/rebel/poll/), [`process`](https://heroes-lang.org/rebel/process/), [`protobuf`](https://heroes-lang.org/rebel/protobuf/), [`serial`](https://heroes-lang.org/rebel/serial/), [`stats`](https://heroes-lang.org/rebel/stats/), [`strings`](https://heroes-lang.org/rebel/strings/), [`term`](https://heroes-lang.org/rebel/term/), [`time`](https://heroes-lang.org/rebel/time/), [`toml`](https://heroes-lang.org/rebel/toml/), [`tracing`](https://heroes-lang.org/rebel/tracing/), [`tui`](https://heroes-lang.org/rebel/tui/).

## Its folders

16 folders, 45 files and 8,687 lines, this page aside; of them 41 modules, `.hero` files, of 8,349 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 508 | 1 | 170 |
| `alerts/` | 2 | 649 | 2 | 649 |
| `bench/` | 1 | 131 | 1 | 131 |
| `cli/` | 2 | 268 | 2 | 268 |
| `collect/` | 4 | 928 | 4 | 928 |
| `conf/` | 2 | 736 | 2 | 736 |
| `control/` | 4 | 421 | 4 | 421 |
| `dash/` | 3 | 576 | 3 | 576 |
| `demo/` | 4 | 731 | 4 | 731 |
| `engine/` | 4 | 969 | 4 | 969 |
| `journal/` | 1 | 150 | 1 | 150 |
| `series/` | 2 | 372 | 2 | 372 |
| `services/` | 4 | 986 | 4 | 986 |
| `standins/` | 4 | 732 | 4 | 732 |
| `telemetry/` | 2 | 381 | 2 | 381 |
| `workload/` | 1 | 149 | 1 | 149 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
