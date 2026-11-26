// Does rustc say anything of a `&mut` parameter the function only reads?
struct App { n: i64 }
fn health(a: &mut App, x: i64) -> i64 { a.n + x }
fn bump(a: &mut App, x: i64) -> i64 { a.n += x; a.n }
fn main() {
    let mut app = App { n: 1 };
    let hs: Vec<fn(&mut App, i64) -> i64> = vec![bump, health];
    println!("{} {}", hs[0](&mut app, 2), hs[1](&mut app, 3));
}
