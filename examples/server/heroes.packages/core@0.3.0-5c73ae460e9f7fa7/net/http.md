# http: `use <core>/net/http`

HTTP/1.1 over a `net` connection, Go's `net/http` in Heroes' idiom: the requests a client sends on a connection, each answered by a function of the program, and a request sent and its response read, one after another on one connection.

## Types

`record Request` with `method: str`, `target: str`, `header: {str: str}` and `body: str`: A request as a client sends it and a handler reads it: `target` is what its first line names, `/hello?x=1`, its path and its query, and each name of a `header` the package read is written as Go writes it, `Content-Length`.

`record Response` with `status: i64`, `header: {str: str}` and `body: str`: A response as a handler answers it and a client reads it.

## Functions

- `serve_conn(c: net.Conn, handler: (function(Request) -> Response)) -> i64?`, called `http.serve_conn(c, handler)`: How many requests `handler` answered on `c`, read one after another until the client closes the connection or asks it closed, or the program is asked to stop: the work Go's `http.Serve` does on each connection.
- `round_trip(c: net.Conn, request: Request) -> Response?`, called `http.round_trip(c, request)`: The response to `request`, sent on `c` and read whole: Go's `RoundTripper.RoundTrip`, on the connection the program dialled, so requests one after another share it.
- `not_found(request: Request) -> Response`, called `http.not_found(request)`: The answer to `request` where its target names nothing: 404, with Go's words, `404 page not found`, and the target after them, the two header lines Go's `http.NotFound` writes beside them.

## Codes

- `malformed`, `http.MALFORMED`: What a failure is where a message is no HTTP/1.1 message this package reads, or a request no server could read back.
