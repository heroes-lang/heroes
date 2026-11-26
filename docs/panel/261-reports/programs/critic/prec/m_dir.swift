// The critic, panel 261: route (m)'s direction in Swift, a borrowing
// (read-only) function where a by-value function type is asked.
struct App { var n: Int }
func peek(_ a: borrowing App) -> Void { print(a.n) }
let m: (App) -> Void = peek
m(App(n: 4))
