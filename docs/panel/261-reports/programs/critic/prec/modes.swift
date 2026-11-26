// The critic, panel 261: is `inout` part of a Swift function type, and does a
// borrowing (read-only) function convert to an inout type?
struct App { var n: Int }
func bump(_ a: inout App) { a.n += 1 }
func peek(_ a: borrowing App) -> Void { print(a.n) }
func plain(_ a: App) -> Void { print(a.n) }
let f: (inout App) -> Void = bump
let g: (App) -> Void = bump
let h: (inout App) -> Void = peek
let k: (borrowing App) -> Void = plain
print("built")
