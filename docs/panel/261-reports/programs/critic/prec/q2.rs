// The critic, panel 261: question 2 in Rust, a fn pointer read out of the
// struct its own call borrows mutably; and whether fn(&App) stands where
// fn(&mut App) is asked.
struct App { n: i64, h: fn(&mut App, i64) -> i64, hs: Vec<fn(&mut App, i64) -> i64> }
fn add(a: &mut App, x: i64) -> i64 { a.n += x; a.h = triple; a.n }
fn triple(a: &mut App, x: i64) -> i64 { a.n *= 3 + 0 * x; a.n }
fn main() {
    let mut app = App { n: 1, h: add, hs: vec![add] };
    println!("{}", (app.h)(&mut app, 2));
    println!("{}", (app.h)(&mut app, 2));
    println!("{}", (app.hs[0])(&mut app, 1));
}
