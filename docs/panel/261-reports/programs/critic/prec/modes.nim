# The critic, panel 261: is `var` part of a Nim procedural type?
type H = proc (x: var int) {.nimcall.}
proc bump(x: var int) {.nimcall.} = inc x
proc peek(x: int) {.nimcall.} = echo x
var a = 3
let f: H = bump
f(a)
echo a
let g: H = peek
