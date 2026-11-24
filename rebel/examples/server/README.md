# server

The twin of `examples/server/`: a small HTTP server, each connection served
on a thread of its own, written with Rebel. Run with no words it prints byte
for byte what `examples/server/` prints, so that program's `main.expected`
is this one's test too.

    heroes run rebel/examples/server/main.hero
    heroes run rebel/examples/server/main.hero -- serve 8080

Run with no words it is the loopback shape: server and client in one
process, the client on a thread of its own sending two requests on one
connection and printing each answer, then asking the program's stop
(`os.ask_stop`), the server taking that connection on a thread of its own
until the stop ends its accepts, so it prints the same on every run. Run as
`serve <port>` it listens on 127.0.0.1 at that port, 0 for one the system
chooses, until Ctrl-C or a request to end (SIGINT, SIGTERM), then lets every
connection finish what it holds, says what it served, and returns from
`main`, so the runtime's count of what was never freed is asked at its end.

What Rebel does here that `examples/server/` writes for itself:

- `http.serve_conn` reads each request of a connection by RFC 9112 within
  `http.default_limits`, answers what is no request 400 and writes each
  response, and `http.round_trip` writes a request and reads its response,
  in place of that program's `http.hero` (its request and response records,
  its reader and writer of messages, `serve_conn`, `round_trip` and
  `not_found`);
- `net.listen`, `net.accept_number`, `net.conn_of`, `net.connect` and
  `os.catch_stop` and `os.ask_stop`, over the platform layer, in place of
  its `net.hero` (the sockets, the number a connection crosses to a thread
  as, the deadline and the stop);
- `threads.start` and `threads.wait` start and join a thread, every one
  started owed its wait, in place of its own `extern "hero_os.h"` group;
- `net.parse_port` reads the port, in place of its `numbers.hero` (`atoi`);
  `strings.trim` trims an answer's body, in place of its `strings.hero`.

What stays this program's own is the loop that gives each connection its own
thread, and the handler. Rebel's `http.serve` serves one connection at a
time; serving several at once is the program's loop, as `http.serve`'s own
comment says, since a Heroes thread is handed one `i64` and nothing else
(spec § 13): a connection crosses into its thread as the number
`net.accept_number` gives it up for, and the thread takes it back with
`net.conn_of`. The 404's body names the path asked for, this program's
words; Rebel's `http.respond_not_found` writes Go's, which do not.

## Its original

The twin of [`examples/server/`](../../examples/server/), the compiler's own program, which takes no library. Run from the repository's root with no word, as that program is, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run rebel/examples/server/main.hero

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 6 faces on its `packages` line, each linked here to its reference: [`http`](https://heroes-lang.org/rebel/http/), [`io`](https://heroes-lang.org/rebel/io/), [`net`](https://heroes-lang.org/rebel/net/), [`os`](https://heroes-lang.org/rebel/os/), [`strings`](https://heroes-lang.org/rebel/strings/), [`threads`](https://heroes-lang.org/rebel/threads/).

## Its folders

1 folder, 2 files and 316 lines, this page aside; of them 1 module, `.hero` files, of 313 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 316 | 1 | 313 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
