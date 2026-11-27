# chat

chat: a realtime chat service and its terminal client. People sign in with
a password and get a JSON Web Token; they talk in rooms and in direct
messages over a WebSocket, see who is online and who is typing, page back
through a room's history kept in SQLite, write `:emoji_name:` shortcodes
that turn into emoji by their Unicode names, and edit their lines and their
last message by grapheme clusters, so a flag or a family is one step. A room
may be mirrored to an MQTT topic or a NATS subject, both ways, so devices
and services talk in it.

Run for real:

    chat serve --listen 127.0.0.1:8080 --db chat.db \
        --mirror iot=mqtt:chat/rooms/iot --mqtt mqtt://127.0.0.1:1883 \
        --mirror ops=nats:chat.rooms.ops --nats nats://127.0.0.1:4222
    chat client http://127.0.0.1:8080 --user alice --register --lang en-US

The server's token secret is `CHAT_SECRET` (32 bytes or more); without it a
random one is made, and tokens end with the server. `chat demo` plays a
whole session on this machine, the brokers replaced by stand-ins the
program carries, and prints its transcript.

The Rebel packages it uses, and for what:
- `websocket`: the clients' connections, upgraded by the server and opened
  by the client; `web` and `http`: the API's routes, requests and answers,
  and the client's sign-in; `net` and `poll`: the listener and one loop
  serving every connection; `threads`: the demo's servers.
- `jose`: the tokens (HS256); `crypto` and `os`: passwords kept as
  PBKDF2-HMAC-SHA256 of a random salt.
- `sql` and `sql/sqlite`: accounts, rooms, memberships and the paged
  history; `uuid`: messages' version 7 identifiers.
- `mqtt` and `nats`: the bridge's clients, and the stand-ins' packets;
  `json`: every frame and payload.
- `tui` and `term`: the terminal client's screen and keys;
  `unicode/segment`: the line editor's grapheme and word boundaries and the
  text's width; `unicode/names`: shortcodes resolved by character names.
- `i18n` and `i18n/dates`: the client's phrases with their plural forms and
  its times written in the reader's language; `time`: the clocks and zones.
- `flags`: this command line; `fs` and `path`: the demo's folder; `url`:
  the server a client is given; `bytes`, `strings`, `numbers`, `io`,
  `math` and `unicode`: the text, the bytes and the streams under it all.

## Its demo

Run from the repository's root with the words of `main.args`, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/chat/main.hero -- demo

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 33 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`crypto`](https://heroes-lang.org/rebel/crypto/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`http`](https://heroes-lang.org/rebel/http/), [`i18n`](https://heroes-lang.org/rebel/i18n/), [`i18n/dates`](https://heroes-lang.org/rebel/i18n/dates/), [`io`](https://heroes-lang.org/rebel/io/), [`jose`](https://heroes-lang.org/rebel/jose/), [`json`](https://heroes-lang.org/rebel/json/), [`math`](https://heroes-lang.org/rebel/math/), [`mqtt`](https://heroes-lang.org/rebel/mqtt/), [`nats`](https://heroes-lang.org/rebel/nats/), [`net`](https://heroes-lang.org/rebel/net/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`os`](https://heroes-lang.org/rebel/os/), [`path`](https://heroes-lang.org/rebel/path/), [`poll`](https://heroes-lang.org/rebel/poll/), [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/sqlite`](https://heroes-lang.org/rebel/sql/sqlite/), [`strings`](https://heroes-lang.org/rebel/strings/), [`term`](https://heroes-lang.org/rebel/term/), [`threads`](https://heroes-lang.org/rebel/threads/), [`time`](https://heroes-lang.org/rebel/time/), [`tui`](https://heroes-lang.org/rebel/tui/), [`unicode`](https://heroes-lang.org/rebel/unicode/), [`unicode/names`](https://heroes-lang.org/rebel/unicode/names/), [`unicode/segment`](https://heroes-lang.org/rebel/unicode/segment/), [`url`](https://heroes-lang.org/rebel/url/), [`uuid`](https://heroes-lang.org/rebel/uuid/), [`web`](https://heroes-lang.org/rebel/web/), [`websocket`](https://heroes-lang.org/rebel/websocket/).

## Its folders

10 folders, 40 files and 8,452 lines, this page aside; of them 37 modules, `.hero` files, of 8,283 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 4 | 359 | 1 | 190 |
| `auth/` | 2 | 231 | 2 | 231 |
| `bridge/` | 2 | 495 | 2 | 495 |
| `client/` | 8 | 1,988 | 8 | 1,988 |
| `core/` | 3 | 488 | 3 | 488 |
| `demo/` | 4 | 616 | 4 | 616 |
| `protocol/` | 3 | 551 | 3 | 551 |
| `server/` | 8 | 2,112 | 8 | 2,112 |
| `standins/` | 5 | 1,239 | 5 | 1,239 |
| `store/` | 1 | 373 | 1 | 373 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.
