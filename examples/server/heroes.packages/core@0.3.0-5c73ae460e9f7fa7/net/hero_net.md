# hero_net: `use <core>/net/hero_net`

The platform layer's sockets, `hero_net.h`, bound thin: one function per C function, each answering what C answers.

## From `hero_net.h`

- `record Socket tag hero_net_socket`, a handle, written `hero_net.Socket`: A socket the layer holds open.
- `hero_net.HERO_NET_OK`, `hero_net.HERO_NET_INVALID`, `hero_net.HERO_NET_NOT_OPEN`, `hero_net.HERO_NET_ADDRESS`, `hero_net.HERO_NET_IN_USE`, `hero_net.HERO_NET_REFUSED`, `hero_net.HERO_NET_RESET`, `hero_net.HERO_NET_TIMED_OUT`, `hero_net.HERO_NET_UNREACHABLE`, `hero_net.HERO_NET_LIMIT`, `hero_net.HERO_NET_DENIED`, `hero_net.HERO_NET_OTHER` and `hero_net.HERO_NET_STOPPED`, each `i64`: The kinds of failure `last_error` answers.

## Functions

- `listen(address: str, port: i64) -> Socket`, called `hero_net.listen(address, port)`: A socket listening on `address` at `port`, or `nullptr`.
- `connect(address: str, port: i64) -> Socket`, called `hero_net.connect(address, port)`: A connection to `address` at `port`, or `nullptr`.
- `accept(listener: Socket) -> Socket`, called `hero_net.accept(listener)`: The next connection `listener` holds, waiting for one, or `nullptr`.
- `port_of(s: Socket) -> i64`, called `hero_net.port_of(s)`: The port `s` stands on, or -1.
- `send(s: Socket, @data: [u8], n: u64) -> i64`, called `hero_net.send(s, @data, n)`: How many of the first `n` bytes of `data` were sent, or -1.
- `recv(s: Socket, @buffer: [u8], n: u64) -> i64`, called `hero_net.recv(s, @buffer, n)`: How many bytes were received into `buffer`, 0 where the peer closed, or -1.
- `close(@s: Socket) -> i64`, called `hero_net.close(@s)`: 0, or -1 where the system failed; `s` is given back either way.
- `number(@s: Socket) -> i64`, called `hero_net.number(@s)`: The number `s` is given up for, or -1.
- `of(n: i64) -> Socket`, called `hero_net.of(n)`: The socket number `n` names, taken back once, or `nullptr`.
- `catch_stop() -> i64`, called `hero_net.catch_stop()`: 0 once SIGINT and SIGTERM ask a stop instead of ending the program, or -1.
- `last_error() -> i64`, called `hero_net.last_error()`: The kind of the last call's failure on this thread.
- `last_os_error() -> i64`, called `hero_net.last_os_error()`: The platform's own number for the last call's failure, 0 where the layer refused.
