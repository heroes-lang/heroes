//! The block shapes: `while`, `for`, `if`, `&&`, `match`.
//!
//! Every test here pins an *edge*, because an edge is what a reader cannot check
//! by looking at one line — and three of these shapes have a wrong version that
//! compiles and runs.

use super::text;

/// A `while` is three blocks, and `continue` re-enters the test. There is nothing
/// to step, so the test is the right target.
#[test]
fn a_while_loop_is_three_blocks_and_continue_re_enters_the_test() {
    let dumped = text(
        "function f(n: i64) -> i64\n    i: i64 @ 0\n    while i < n\n        i @ i + 1\n        continue\n    return i\n",
    );
    assert!(dumped.contains("while: test"), "{dumped}");
    assert!(dumped.contains("while: body"), "{dumped}");
    assert!(dumped.contains("while: exit"), "{dumped}");
    let test = block_named(&dumped, "while: test");
    let body = block_named(&dumped, "while: body");
    assert!(preds_of(&dumped, test).contains(&body), "continue re-enters the test: {dumped}");
}

/// A `for` is **four** blocks, and this is the one worth knowing: `continue` lands
/// on the *step*. If it landed on the test the index would never advance on that
/// path, and the program would hang rather than fail — which is why the adversarial
/// golden pins it too.
#[test]
fn a_for_loop_steps_on_the_continue_path() {
    let dumped = text(
        "function f(xs: [i64]) -> i64\n    n: i64 @ 0\n    for x in xs\n        if x < 0\n            continue\n        n @ n + x\n    return n\n",
    );
    for note in ["for: test", "for: body", "for: step", "for: exit"] {
        assert!(dumped.contains(note), "missing {note}: {dumped}");
    }
    let step = block_named(&dumped, "for: step");
    let test = block_named(&dumped, "for: test");
    // The step feeds the test, and two edges reach the step: the fall-through and
    // the `continue`. Together they say the increment cannot be skipped.
    assert!(preds_of(&dumped, test).contains(&step), "the step must feed the test: {dumped}");
    assert!(preds_of(&dumped, step).len() >= 2, "fall-through and continue: {dumped}");
}

/// The iterable is evaluated once, into a slot. `for x in f()` calling `f` per
/// iteration is a different program, and with a side-effecting `f` a visibly
/// different one.
#[test]
fn a_for_loop_evaluates_its_iterable_once() {
    let dumped = text(
        "function ns() -> [i64]\n    return [1, 2]\n\nfunction f() -> i64\n    n: i64 @ 0\n    for x in ns()\n        n @ n + x\n    return n\n",
    );
    assert_eq!(dumped.matches("call heroes ns()").count(), 1, "{dumped}");
    assert!(dumped.contains("store $xs0 <-"), "the sequence is held in a slot: {dumped}");
}

/// An `if` used as a value writes a join slot from each arm and loads it once —
/// which is what replaces a phi node (panel 019 point 2).
#[test]
fn an_if_with_a_value_uses_a_join_slot() {
    let dumped = text(
        "function f(n: i64) -> i64\n    v = if n > 0\n        1\n    else\n        0\n    return v\n",
    );
    assert!(dumped.contains("$r0: i64"), "the join slot is synthetic: {dumped}");
    assert_eq!(dumped.matches("store $r0 <-").count(), 2, "one per arm: {dumped}");
    assert_eq!(dumped.matches("load $r0").count(), 1, "loaded once, at the join: {dumped}");
}

/// `&&` is not an instruction. §4.14 makes it short-circuit, so the right side sits
/// in its own block and is reached on one edge only.
#[test]
fn and_evaluates_its_right_side_on_one_edge() {
    let dumped = text(
        "function loud(b: bool) -> bool\n    print(1)\n    return b\n\nfunction f(a: bool, b: bool) -> bool\n    return a && loud(b)\n",
    );
    assert!(dumped.contains("&&: right"), "{dumped}");
    let right = block_named(&dumped, "&&: right");
    assert_eq!(preds_of(&dumped, right).len(), 1, "one edge in: {dumped}");
    assert!(!dumped.contains("= and "), "there is no `and` instruction: {dumped}");
}

/// `match` over a variant is a `switch` on the tag: dense over the declaration's
/// cases and with **no default edge**, because M-data-declarations proved it exhaustive. A `|` arm
/// is two tags pointing at one block.
#[test]
fn a_match_over_a_variant_is_a_dense_switch() {
    let dumped = text(
        "variant Token\n    num\n        v: i64\n    plus\n    times\n\nfunction f(t: Token) -> i64\n    return match t\n        .num n         => n.v\n        .plus | .times => 0\n",
    );
    let line = dumped.lines().find(|l| l.contains("switch")).expect("a switch");
    for case in 0..3 {
        assert!(line.contains(&format!("case {case} ->")), "{line}");
    }
    assert_eq!(edge_of(line, 1), edge_of(line, 2), "a `|` arm is one block: {line}");
}

/// `match` over `i64` is a chain of comparisons, because C has no `switch` over
/// arbitrary equality and exhaustiveness over `i64` is impossible — `_` is what
/// closes it (§4.7).
#[test]
fn a_match_over_literals_is_a_chain_of_comparisons() {
    let dumped = text(
        "function f(n: i64) -> str\n    return match n\n        0 => \"zero\"\n        1 => \"one\"\n        _ => \"many\"\n",
    );
    assert!(!dumped.contains("switch"), "{dumped}");
    assert_eq!(dumped.matches("= eq ").count(), 2, "one test per literal arm: {dumped}");
    assert!(dumped.contains("match: test"), "{dumped}");
}

/// When every arm jumps, the join has no predecessors — and lowering says
/// `unreachable` rather than loading a slot nothing wrote. Panel 017 A made this
/// legal, so it is a real shape.
#[test]
fn a_match_whose_arms_all_jump_leaves_an_unreachable_join() {
    let dumped = text(
        "variant Step\n    stop\n    skip\n\nfunction f(xs: [Step]) -> i64\n    for s in xs\n        match s\n            .stop => break\n            .skip => continue\n    return 0\n",
    );
    let join = block_named(&dumped, "match: join");
    assert!(preds_of(&dumped, join).is_empty(), "nothing reaches it: {dumped}");
    let terminator = dumped
        .lines()
        .skip_while(|line| !line.trim().starts_with(&format!("bb{join}  match: join")))
        .nth(1)
        .expect("the join's terminator");
    assert!(terminator.contains("unreachable"), "{terminator}");
}

// --- reading the dump ------------------------------------------------------
//
// The tests above assert about *edges*, and the dump is where edges are written
// down. Parsing it back is deliberate: it means a test fails when the artifact a
// reader sees is wrong, not merely when an internal field is.

fn block_named(dumped: &str, note: &str) -> u32 {
    for line in dumped.lines() {
        let trimmed = line.trim();
        if let Some(rest) = trimmed.strip_prefix("bb") {
            let (id, tail) = rest.split_once("  ").unwrap_or((rest, ""));
            if tail.starts_with(note) {
                return id.parse().expect("a block number");
            }
        }
    }
    panic!("no block noted {note} in:\n{dumped}");
}

fn preds_of(dumped: &str, block: u32) -> Vec<u32> {
    let head = format!("bb{block}  ");
    for line in dumped.lines() {
        let trimmed = line.trim();
        if !trimmed.starts_with(&head) {
            continue;
        }
        return match trimmed.split_once("preds ") {
            Some((_, list)) => list
                .split(", ")
                .map(|name| name.trim_start_matches("bb").parse().expect("a block number"))
                .collect(),
            None => Vec::new(),
        };
    }
    panic!("no block bb{block} in:\n{dumped}");
}

/// The target of `case <n> -> bbK` on a `switch` line.
fn edge_of(line: &str, case: u32) -> u32 {
    let needle = format!("case {case} -> bb");
    let rest = line.split(&needle).nth(1).expect("that case");
    rest.split(|c: char| !c.is_ascii_digit()).next().unwrap_or("").parse().expect("a block number")
}
