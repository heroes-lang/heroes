# iothub

iothub: an IoT hub. Devices publish to an MQTT broker, an AMQP 0-9-1
broker and a NATS server, or speak NMEA 0183 on a serial line; the hub
reads them all, finds each device in its registry by the address it
publishes at, decodes its payload by the registry's encoding and schema
(SenML packs in CBOR, MessagePack maps, Protocol Buffers read by `.proto`
files at run time, XDR sentences), re-publishes every reading as JSON,
and routes readings from one protocol to another by rules with
conditions. A dashboard watches it all live over a WebSocket feed, and
Prometheus scrapes its metrics. A broker that drops is tried again with
exponential backoff, and what the hub owed it is held and sent when it is
back.

Run for real, with a folder holding `hub.json`, `devices.json` and the
`.proto` files (`config/settings` shows a `hub.json`):

    iothub register --registry hub/devices.json --name fridge-07 \
        --protocol mqtt --address devices/fridge-07/up --encoding cbor \
        --schema senml --field temp=temperature:Cel
    iothub serve --config hub/hub.json
    iothub watch ws://127.0.0.1:8088/feed
    curl http://127.0.0.1:8088/metrics
    iothub decode --registry hub/devices.json --proto hub/plant.proto \
        --device press-2 payload.bin

`iothub demo` plays a whole session on this machine, the brokers replaced
by stand-ins the program carries, and prints its transcript.

The Rebel packages it uses, and for what:
- `mqtt`, `amqp` and `nats`: the hub's links to its three brokers, and
  the demo's devices and observers; `mqtt` and `amqp`'s field tables also
  read and write the stand-ins' packets.
- `cbor`, `msgpack` and `protobuf`: the devices' payloads, decoded by the
  registry's schema, a `.proto` file read at run time for Protocol
  Buffers, and encoded by the demo's devices.
- `serial`: the gateway's ports, and the demo's pseudo-terminal.
- `websocket` and `http`: the dashboard's live feed, `/metrics` and
  `/devices`, served from the hub's own loop, and `iothub watch`.
- `metrics`: the hub's counters, gauges and a histogram, in Prometheus's
  text format.
- `retries`: the exponential backoff with jitter of every lost broker and
  port; `random`: its seeded chance, and the seeded identities.
- `uuid`: every device's identity; `json`: the settings, the registry,
  the readings re-published and the feed.
- `net` and `poll`: one loop for every socket, and the demo's servers,
  on threads of the language's own (`spawn`, mailboxes); `time`: the
  clocks and moments; `flags`: this command line; `fs` and `path`: the
  hub's folder; `bytes`, `strings`, `numbers`, `io`, `math` and `os`:
  the bytes, text and streams under it all.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/iothub/main.hero -- demo

A line `main.varies` names is printed in another form where another machine decides it, each form written there.

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 27 faces on its `packages` line, each linked here to its reference: [`amqp`](https://heroes-lang.org/rebel/amqp/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`cbor`](https://heroes-lang.org/rebel/cbor/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`math`](https://heroes-lang.org/rebel/math/), [`metrics`](https://heroes-lang.org/rebel/metrics/), [`mqtt`](https://heroes-lang.org/rebel/mqtt/), [`msgpack`](https://heroes-lang.org/rebel/msgpack/), [`nats`](https://heroes-lang.org/rebel/nats/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`poll`](https://heroes-lang.org/rebel/poll/), [`protobuf`](https://heroes-lang.org/rebel/protobuf/), [`random`](https://heroes-lang.org/rebel/random/), [`retries`](https://heroes-lang.org/rebel/retries/), [`serial`](https://heroes-lang.org/rebel/serial/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`uuid`](https://heroes-lang.org/rebel/uuid/), [`websocket`](https://heroes-lang.org/rebel/websocket/).

## Its folders

10 folders, 45 files and 10,180 lines, this page aside; of them 41 modules, `.hero` files, of 10,019 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 449 | 1 | 288 |
| `brokers/` | 5 | 656 | 5 | 656 |
| `codecs/` | 6 | 1,122 | 6 | 1,122 |
| `config/` | 2 | 841 | 2 | 841 |
| `demo/` | 5 | 1,089 | 5 | 1,089 |
| `feed/` | 4 | 500 | 4 | 500 |
| `hub/` | 5 | 1,092 | 5 | 1,092 |
| `routes/` | 2 | 426 | 2 | 426 |
| `serial/` | 1 | 173 | 1 | 173 |
| `standins/` | 10 | 3,832 | 10 | 3,832 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
