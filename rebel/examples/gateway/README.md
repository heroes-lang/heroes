# gateway

gateway: an API gateway in the manner of Envoy and Kong, in small. It
terminates TLS and speaks HTTP/1.1 and HTTP/2 to its clients (ALPN
chooses), routes each request by its host and path through a prefix tree,
finds the route's upstream by its DNS SRV records, and then forwards plain
HTTP through a response cache (LRU, `Cache-Control` honoured, 304 for a
client holding a copy), relays gRPC calls unary and streaming, transcodes a
plain `GET` into a gRPC call answered as JSON, or answers GraphQL from one
schema composed of two upstreams, validating each query before any
upstream is asked. Every client has a token bucket per route; a refused
connection is tried again on the next instance, an idempotent call after a
failure too, and an instance that refused is skipped by its circuit
breaker for a while. The gateway serves its own metrics in Prometheus's
format. Its connections are served by worker threads, each handed its TLS
connection whole; its state, the cache, the buckets, the breakers and the
metrics, is owned by one thread the workers ask by message
(`core/owner`, `listen/workers`), so a slow client holds only its worker.

Run for real:

    heroes build rebel/examples/gateway/main.hero -o gateway
    ./gateway check routes.toml     # read the routing file, print its routes
    ./gateway serve routes.toml     # serve until Ctrl-C

`settings.hero` (in `config/`) documents the routing file; `main.stdin` is
a whole one. The demo, `./gateway demo < main.stdin`, starts on loopback a
DNS server and three upstreams it carries (a REST catalog over HTTP, an
inventory over gRPC, reviews over GraphQL), registers them in DNS, serves
the gateway, and runs a scripted client over TLS whose transcript is
`main.expected`.

Rebel's packages, and what for:

- `tls`, `x509`, `pem`, `asn1`, `crypto`, `crypto/keypairs`: the listener,
  and the demo's certificate made, written and verified;
- `http`, `http2`, `io`, `net`: the protocols, both ways;
- `grpc`, `protobuf`: relaying calls, transcoding, the inventory stand-in;
- `graphql`, `json`: the composed endpoint and the reviews stand-in;
- `containers/trie`: the route table; `containers/cache`: the response
  cache; `hash`: ETags (CRC-32) and rendezvous hashing (FNV-1a);
- `dns`, `retries`, `math`: discovery by SRV, retries with backoff and
  jitter, circuit breakers, weighted choice;
- `metrics`: the gateway's own metrics; `toml`: the routing file;
- `poll`, `os`: the DNS stand-in's two sockets, the stop; the threads and
  mailboxes are the language's own (`spawn`, `mailbox()`);
- `time`, `strings`, `bytes`, `numbers`, `maps`: throughout.

## Its demo

Run from the repository's root with the words of `main.args` and `main.stdin` on its input, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/gateway/main.hero -- demo < rebel/examples/gateway/main.stdin

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 32 faces on its `packages` line, each linked here to its reference: [`asn1`](https://heroes-lang.org/rebel/asn1/), [`bytes`](https://heroes-lang.org/rebel/bytes/), [`containers/cache`](https://heroes-lang.org/rebel/containers/cache/), [`containers/trie`](https://heroes-lang.org/rebel/containers/trie/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`crypto/keypairs`](https://heroes-lang.org/rebel/crypto/keypairs/), [`dns`](https://heroes-lang.org/rebel/dns/), [`errors`](https://heroes-lang.org/rebel/errors/), [`graphql`](https://heroes-lang.org/rebel/graphql/), [`grpc`](https://heroes-lang.org/rebel/grpc/), [`hash`](https://heroes-lang.org/rebel/hash/), [`http`](https://heroes-lang.org/rebel/http/), [`http2`](https://heroes-lang.org/rebel/http2/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`maps`](https://heroes-lang.org/rebel/maps/), [`math`](https://heroes-lang.org/rebel/math/), [`metrics`](https://heroes-lang.org/rebel/metrics/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`pem`](https://heroes-lang.org/rebel/pem/), [`poll`](https://heroes-lang.org/rebel/poll/), [`protobuf`](https://heroes-lang.org/rebel/protobuf/), [`retries`](https://heroes-lang.org/rebel/retries/), [`strings`](https://heroes-lang.org/rebel/strings/), [`time`](https://heroes-lang.org/rebel/time/), [`tls`](https://heroes-lang.org/rebel/tls/), [`toml`](https://heroes-lang.org/rebel/toml/), [`web`](https://heroes-lang.org/rebel/web/), [`web/chain`](https://heroes-lang.org/rebel/web/chain/), [`x509`](https://heroes-lang.org/rebel/x509/).

## Its folders

12 folders, 40 files and 7,629 lines, this page aside; of them 36 modules, `.hero` files, of 7,382 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 323 | 1 | 76 |
| `cache/` | 1 | 236 | 1 | 236 |
| `config/` | 3 | 755 | 3 | 755 |
| `core/` | 4 | 606 | 4 | 606 |
| `demo/` | 5 | 927 | 5 | 927 |
| `discovery/` | 1 | 342 | 1 | 342 |
| `graph/` | 1 | 275 | 1 | 275 |
| `limits/` | 1 | 175 | 1 | 175 |
| `listen/` | 8 | 1,180 | 8 | 1,180 |
| `proxy/` | 4 | 970 | 4 | 970 |
| `route/` | 2 | 550 | 2 | 550 |
| `standins/` | 5 | 1,290 | 5 | 1,290 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
