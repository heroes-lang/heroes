// The critic, panel 261: question 2 in Swift, a callee read out of the place
// its own call passes inout.
struct App {
    var n: Int
    var h: (inout App, Int) -> Int
    var hs: [(inout App, Int) -> Int]
}
func add(_ a: inout App, _ x: Int) -> Int { a.n += x; a.h = triple; return a.n }
func triple(_ a: inout App, _ x: Int) -> Int { a.n *= 3; return a.n }
var app = App(n: 1, h: add, hs: [add])
print(app.h(&app, 2))
print(app.h(&app, 2))
print(app.hs[0](&app, 1))
