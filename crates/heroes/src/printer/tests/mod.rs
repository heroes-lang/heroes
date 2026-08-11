//! What a formatter has to prove.
//!
//! Three properties, in order of importance:
//!
//! 1. **It preserves meaning.** The tree of the formatted text is the tree of
//!    the original — checked by comparing the two dumps, which is the one
//!    rendering that cannot silently agree with itself (it prints every
//!    parenthesis, and `fmt` prints the fewest).
//! 2. **It is idempotent.** `fmt(fmt(x)) == fmt(x)`, or "canonical" means
//!    nothing and §4.15's "any textual difference is semantic" is false.
//! 3. **It loses nothing.** Comments are not in the tree; a formatter that
//!    drops them is a formatter nobody runs.
//!
//! | file | what it pins |
//! |---|---|
//! | `mod.rs` | the harness, the acceptance program, and the layout policies |
//! | `comments.rs` | §4.1's adjacency rule and blank lines as content |
//! | `modules.rs` | `use` lines: source order, one block, comments (panel 031) |

mod comments;
mod modules;

use crate::source::Source;
use crate::syntax::parse;

use super::{dump_ast, format_file};

pub(super) fn format(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let out = parse(&src);
    let rendered: Vec<String> = out.diagnostics.iter().map(|d| d.render_line(&src)).collect();
    assert_eq!(rendered, Vec::<String>::new(), "the input must parse clean");
    format_file(&out.ast, &out.comments, &src)
}

fn dump(text: &str) -> String {
    let src = Source::new("test.hero".to_string(), text.to_string());
    let out = parse(&src);
    dump_ast(&out.ast, &src)
}

/// Formatting twice changes nothing, and formatting does not change the tree.
pub(super) fn assert_canonical(text: &str) {
    let once = format(text);
    let twice = format(&once);
    assert_eq!(once, twice, "fmt is not idempotent");
    assert_eq!(dump(text), dump(&once), "fmt changed the tree");
}

/// The acceptance program: 317 lines, every construct in the language, read
/// out of design.md's appendix (its single source — see
/// `syntax::tests::acceptance`).
///
/// It asserts more than idempotence: the appendix **is** canonical form, byte
/// for byte. That became true when the formatter learned to align a run of arms
/// — the appendix had been aligning them by hand since before any compiler
/// existed, and the tool now produces what the reference document shows. If the
/// two ever disagree again, one of them is wrong and this test says so.
#[test]
fn the_acceptance_program_is_canonical_and_unchanged_by_formatting() {
    let design = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../design.md"
    ))
    .expect("design.md must exist");
    let appendix = design
        .find("## Appendix — A complete example program")
        .expect("the appendix must exist");
    let tail = &design[appendix..];
    let open = tail.find("```\n").expect("the appendix has a code fence") + 4;
    let close = tail[open..].find("\n```").expect("the fence closes") + open;
    let text = &tail[open..=close];
    assert_canonical(text);
    assert_eq!(format(text), text, "design.md's appendix is not in canonical form");
}

/// A run of single-line arms lines its `=>` up, so a `match` reads as the table
/// it is. The run stops where the author stopped it: at a block-bodied arm,
/// whose `=>` ends its line, and at a blank line, because blank lines are
/// content in this formatter.
#[test]
fn a_run_of_inline_arms_aligns_its_arrows() {
    let text = "\
function f(n: int) -> int
    return match n
        0     => 10
        1     => 20
        30000 => 30

        2 => 40
";
    assert_eq!(format(text), text);
    assert_canonical(text);
}

/// A list the author wrote down the page keeps its shape even though it fits on
/// one line. §4.9 gives the two forms different separators, so this is not
/// wrapping — it is the other spelling, and the grouping is the author's.
#[test]
fn a_multi_line_list_stays_multi_line() {
    let text = "\
function main()
    cases = [
        \"a\"
        \"b\"
    ]
    print(cases)
";
    assert_eq!(format(text), text);
    assert_canonical(text);
    // …and one written on a line that fits stays on it.
    assert_eq!(
        format("function main()\n    print([\"a\", \"b\"])\n"),
        "function main()\n    print([\"a\", \"b\"])\n"
    );
}

/// `examples/gallery/00-first.hero` is already written in canonical form, and stays that
/// way: this test fails the day the formatter's policies change under it.
#[test]
fn the_first_program_is_already_canonical() {
    let text = std::fs::read_to_string(concat!(
        env!("CARGO_MANIFEST_DIR"),
        "/../../examples/gallery/00-first.hero"
    ))
    .expect("examples/gallery/00-first.hero must exist");
    assert_eq!(format(&text), text);
}

/// Minimal parentheses: the table already says what `2 + 3 * 4` means, so the
/// canonical form does not repeat it — and says what it cannot leave out.
#[test]
fn parentheses_survive_exactly_where_they_carry_meaning() {
    assert_eq!(
        format("function main()\n    x = 2 + (3 * 4)\n"),
        "function main()\n    x = 2 + 3 * 4\n"
    );
    assert_eq!(
        format("function main()\n    x = (2 + 3) * 4\n"),
        "function main()\n    x = (2 + 3) * 4\n"
    );
    // Left-associative, so the right operand of an equal-power operator keeps
    // its parentheses: `a - (b - c)` is not `a - b - c`.
    assert_eq!(
        format("function main()\n    x = a - (b - c)\n"),
        "function main()\n    x = a - (b - c)\n"
    );
    assert_eq!(
        format("function main()\n    x = (a - b) - c\n"),
        "function main()\n    x = a - b - c\n"
    );
}

/// A function that returns nothing writes no arrow — `function main()` is
/// the form §4.2 shows, so `-> ()` has one spelling and it is the empty one.
#[test]
fn the_unit_result_is_never_written() {
    assert_eq!(
        format("function main() -> ()\n    print(1)\n"),
        "function main()\n    print(1)\n"
    );
}

/// §4.9: a list too long for one line breaks by **newline**, not by comma —
/// and only inside its brackets, because panel 007 allows no other
/// continuation.
#[test]
fn a_list_too_long_for_one_line_breaks_by_newline() {
    let long = "function main()\n    xs = [\"aaaaaaaaaaaaaaaa\", \"bbbbbbbbbbbbbbbb\", \"cccccccccccccccc\", \"dddddddddddddddd\"]\n";
    assert_eq!(
        format(long),
        "\
function main()
    xs = [
        \"aaaaaaaaaaaaaaaa\"
        \"bbbbbbbbbbbbbbbb\"
        \"cccccccccccccccc\"
        \"dddddddddddddddd\"
    ]
"
    );
    assert_canonical(long);
}

/// An argument list keeps its commas when it breaks: §4.9's newline rule is
/// about literals, and the parser requires `,` in a call.
#[test]
fn an_argument_list_too_long_for_one_line_breaks_by_comma() {
    let long = "function main()\n    copy(from: \"/tmp/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\", to: \"/tmp/bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\")\n";
    assert_eq!(
        format(long),
        "\
function main()
    copy(
        from: \"/tmp/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\",
        to: \"/tmp/bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\"
    )
"
    );
    assert_canonical(long);
}

/// `if` as a value, `match` with its arms, a record and a variant: the shapes
/// whose canonical layout is not obvious.
#[test]
fn control_forms_and_type_declarations_round_trip() {
    let text = "\
variant Token
    num
        v: int
    plus

function state(t: Token) -> str
    label = if t == .plus
        \"+\"
    else
        \"?\"
    return match t
        .num n => n.v.str()
        .plus  => label
";
    assert_eq!(format(text), text);
    assert_canonical(text);
}

/// The defect that made panel 014 expensive: an arm whose body is a control
/// form. `heroes fmt` printed the `if` and **deleted its branches** — a
/// formatter that silently changes what a program computes, reachable in three
/// lines, and no test in this file had the shape.
///
/// It could not be fixed without panel 014: the inline arm body was the only
/// body position rendering outside the block-aware path.
#[test]
fn an_arm_whose_body_is_a_control_form_keeps_its_blocks() {
    let text = "\
function g(x: int) -> int
    return match x
        0 => if x > 0
            1
        else
            2
        _ => 3
";
    assert_eq!(format(text), text);
    assert_canonical(text);
}

/// Panel 014's forms, round-tripped: a check, a jump, and a `return` inline.
#[test]
fn statement_arm_bodies_are_canonical() {
    let text = concat!(
        "function f(ts: [int]) -> int?\n",
        "    total: int @ 0\n",
        "    for t in ts\n",
        "        match t\n",
        "            0 => break\n",
        "            1 => continue\n",
        "            2 => return fail(\"two\", \"no twos\")\n",
        "            _ => total @ total + t\n",
        "    return ok(total)\n",
        "\n",
        "test \"it rejects the unknown\"\n",
        "    match f([2])\n",
        "        .ok _  => assert false\n",
        "        .err e => assert e.code == \"two\"\n",
    );
    assert_eq!(format(text), text);
    assert_canonical(text);
}
