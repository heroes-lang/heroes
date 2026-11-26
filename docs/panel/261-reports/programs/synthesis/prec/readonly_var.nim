# Does nim say anything of a `var` parameter the proc only reads?
type App = object
  n: int
proc health(a: var App, x: int): int = a.n + x
proc bump(a: var App, x: int): int =
  a.n += x
  a.n
var app = App(n: 1)
let hs: seq[proc (a: var App, x: int): int {.nimcall.}] = @[bump, health]
echo hs[0](app, 2), " ", hs[1](app, 3)
