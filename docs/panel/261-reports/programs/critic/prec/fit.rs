// The critic, panel 261: a read-only fn where a fn taking &mut is asked.
struct App { n: i64 }
fn peek(a: &App) -> i64 { a.n }
fn main() {
    let f: fn(&mut App) -> i64 = peek;
    let mut a = App { n: 1 };
    println!("{}", f(&mut a));
}
