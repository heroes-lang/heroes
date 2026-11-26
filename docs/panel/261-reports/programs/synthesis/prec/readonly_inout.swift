// Does swiftc say anything of an `inout` parameter the function only reads?
struct App { var n: Int }
func health(_ a: inout App, _ x: Int) -> Int { return a.n + x }
func bump(_ a: inout App, _ x: Int) -> Int { a.n += x; return a.n }
var app = App(n: 1)
let hs: [(inout App, Int) -> Int] = [bump, health]
print(hs[0](&app, 2), hs[1](&app, 3))
