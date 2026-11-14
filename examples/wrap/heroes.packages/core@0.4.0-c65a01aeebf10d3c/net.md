# net: `use <core>/net`

TCP connections, Go's `net`: listen, accept, dial, read, write, and the platform layer's stop.

## Types

`record Listener` with `socket: hero_net.Socket`: A listening socket, which `close_listener` ends.

`record Conn` with `socket: hero_net.Socket`: A connection, which `close` ends or `number` gives up.

## Functions

- `listen(host: str, port: i64) -> Listener?`, called `net.listen(host, port)`: A socket listening on `host` at `port`: Go's `net.Listen`.
- `port_of(listener: Listener) -> i64?`, called `net.port_of(listener)`: The port `listener` stands on.
- `accept(listener: Listener) -> Conn?`, called `net.accept(listener)`: The next connection `listener` holds, waiting for one: Go's `Listener.Accept`.
- `dial(host: str, port: i64) -> Conn?`, called `net.dial(host, port)`: A connection to `host` at `port`: Go's `net.Dial`.
- `read(c: Conn, most: i64) -> [u8]?`, called `net.read(c, most)`: Up to `most` bytes `c` received, waiting for one: Go's `Conn.Read`.
- `write(c: Conn, data: [u8]) -> ()?`, called `net.write(c, data)`: Every byte of `data` sent on `c`, or a failure: Go's `Conn.Write`.
- `close(@c: Conn)`, called `net.close(@c)`: Closes `c` and gives it back: Go's `Conn.Close`.
- `close_listener(@listener: Listener)`, called `net.close_listener(@listener)`: Closes `listener` and gives it back: Go's `Listener.Close`.
- `number(@c: Conn) -> i64?`, called `net.number(@c)`: The number `c` is given up for, at least 2^62: carry it whole to `of`, never computing with it.
- `of(n: i64) -> Conn?`, called `net.of(n)`: The connection `n` names, taken back once.
- `catch_stop() -> ()?`, called `net.catch_stop()`: Makes Ctrl-C and SIGTERM ask the program to stop instead of ending it.
- `set_deadline(c: Conn, ms: i64) -> ()?`, called `net.set_deadline(c, ms)`: Every wait on `c` fails `timed_out` past `ms` milliseconds from now: Go's `Conn.SetDeadline`.
- `ask_stop()`, called `net.ask_stop()`: Asks this program to stop, from any thread.
- `stop_asked() -> bool`, called `net.stop_asked()`: Whether this program was asked to stop.

## Codes

- `address`, `net.ADDRESS`: The address names nothing this machine reaches or listens on.
- `closed`, `net.CLOSED`: The peer closed its side, and every byte it sent was read: no failure, the end of what it sends.
- `denied`, `net.DENIED`: The system denied the call.
- `in_use`, `net.IN_USE`: Another socket holds the port.
- `invalid`, `net.INVALID`: An argument refused before any call: a host holding a zero byte, a port outside 0 to 65535, a read of no byte.
- `limit`, `net.LIMIT`: No socket, descriptor or memory is left.
- `not_open`, `net.NOT_OPEN`: The socket is closed, or the number names none.
- `other`, `net.OTHER`: Another failure of the system, its own number in the message.
- `refused`, `net.REFUSED`: Nothing listens at the port.
- `reset`, `net.RESET`: The peer reset or closed the connection.
- `stopped`, `net.STOPPED`: The program was asked to stop: every call that would wait fails so, on every thread.
- `timed_out`, `net.TIMED_OUT`: The peer did not answer in time, or the deadline passed.
- `unreachable`, `net.UNREACHABLE`: No route reaches the address.
