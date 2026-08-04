//! Records, variants, `T?`, places, `@` copy-out, `assert`, `test`.
//!
//! This is where the five things panel 019 added to the instruction set are held
//! to account — the ones the ffi-pragmatist found by compiling C rather than by
//! reading the proposal.

use super::{lowered, text};
use crate::ir::{Op, Term};

/// A record construction is **one instruction**, not an expansion. The layout
/// decision belongs to M5c's descriptor pass, and lowering that spelled it out
/// field by field would have taken it.
#[test]
fn a_record_construction_is_one_instruction() {
    let dumped = text(
        "record Point\n    x: int\n    y: int\n\nfunction f() -> Point\n    return Point(x: 1, y: 2)\n",
    );
    assert!(dumped.contains("construct Point($t1, $t2)"), "{dumped}");
}

/// The label is gone, and it cost nothing to erase: §4.9 checks a label against the
/// parameter *at that position*, so the arguments were already in declaration order
/// before lowering saw them. Part 5's "named arguments → positional" row is a
/// surface rule, not a pass.
#[test]
fn a_label_is_not_carried_into_the_ir() {
    let dumped = text(
        "function between(from: int, to: int) -> int\n    return to - from\n\nfunction f() -> int\n    return between(from: 1, to: 4)\n",
    );
    let call = dumped.lines().find(|line| line.contains("call heroes")).expect("the call");
    assert!(call.contains("call heroes between($t1, $t2)"), "{call}");
    assert!(!call.contains("from:"), "the label is erased: {call}");
}

/// A field is an **index** in the IR and a *name* in the dump. The index is what no
/// later pass may re-derive by comparing strings; the name is recovered for the
/// reader, out of the type the base value already has.
#[test]
fn a_field_read_is_an_index_printed_as_a_name() {
    let (program, _, dumped) = lowered(
        "record Point\n    x: int\n    y: int\n\nfunction f(p: Point) -> int\n    return p.y\n",
    );
    assert!(dumped.contains("field $t1.y"), "{dumped}");
    let reads: Vec<u32> = program.functions[0]
        .blocks
        .iter()
        .flat_map(|block| block.insts.iter())
        .filter_map(|inst| match inst.op {
            Op::Field { index, .. } => Some(index),
            _ => None,
        })
        .collect();
    assert_eq!(reads, vec![1], "`y` is field 1 of `Point`");
}

/// `m[k]` is **not** `xs[i]`: a map access yields a `V?` and cannot abort, an array
/// index can (§4.9). One syntax, two instructions — and only one of them is marked.
#[test]
fn a_map_access_and_an_array_index_are_different_instructions() {
    let dumped = text(
        "function f(xs: [int], m: {str: int}) -> int?\n    print(xs[0])\n    return m[\"a\"]\n",
    );
    assert!(dumped.contains("index! $t"), "an array index can abort: {dumped}");
    assert!(dumped.contains("mapget $t"), "a map access cannot: {dumped}");
}

/// A place is a slot plus a path (§4.8: "every place has exactly one root"), and the
/// dump spells it the way the author wrote it. Without this, `l.pos @ l.pos + 1` —
/// a line in design.md's own appendix — would be unwritable.
#[test]
fn a_mutation_writes_through_a_place() {
    let dumped = text(
        "record Lex\n    pos: int\n\nfunction advance(@l: Lex)\n    l.pos @ l.pos + 1\n",
    );
    assert!(dumped.contains("store l.pos <- $t"), "{dumped}");
}

/// §4.8's hardest sentence: **copy-out happens always**, including on early
/// `return` and on `?`. Three exit edges here, and the verifier counts them — a
/// missing one on the `?` edge would lose the caller's mutation on the failing path
/// only, which is the shape of bug that ships.
#[test]
fn every_exit_edge_copies_out_a_mutable_parameter() {
    let (program, _, dumped) = lowered(
        "record R\n    pos: int\n\nfunction one(n: int) -> int?\n    if n == 0\n        return fail(\"zero\", \"no\")\n    return ok(n)\n\nfunction step(@r: R, n: int) -> int?\n    v = one(n)?\n    if v == 1\n        return ok(1)\n    r.pos @ r.pos + v\n    return ok(v)\n",
    );
    let step = program.functions.iter().find(|f| f.name == "step").expect("step");
    let returns = step
        .blocks
        .iter()
        .filter(|block| matches!(block.term, Term::Return(_)))
        .count();
    assert_eq!(returns, 3, "the `?` edge, the early return and the tail");
    assert_eq!(dumped.matches("copyout r").count(), 3, "one per exit edge: {dumped}");
}

/// `?` propagates the error **unchanged**: the failure that already exists is
/// wrapped into the caller's `T?` rather than rebuilt from its code and msg, which
/// would lose anything else the callee put there.
#[test]
fn try_wraps_the_existing_failure() {
    let dumped = text(
        "function one() -> int?\n    return ok(1)\n\nfunction f() -> int?\n    v = one()?\n    return ok(v + 1)\n",
    );
    assert!(dumped.contains("?: propagate"), "{dumped}");
    assert!(dumped.contains("payload $t"), "{dumped}");
    assert!(dumped.contains("construct err($t"), "{dumped}");
}

/// `.is_err()` is the cheap row of Part 5's table: a tag read and one comparison,
/// no edge at all. `.must()` and `.default(v)` need one each.
#[test]
fn is_err_needs_no_branch_and_must_needs_one() {
    let cheap = text("function f(v: int?) -> bool\n    return v.is_err()\n");
    assert!(cheap.contains("= tag $t"), "{cheap}");
    assert!(!cheap.contains("branch"), "no edge is needed: {cheap}");

    let costly = text("function f(v: int?) -> int\n    return v.must()\n");
    assert!(costly.contains("must: abort"), "{costly}");
    assert!(costly.contains("abort must"), "{costly}");
    assert!(costly.contains("unreachable"), "an abort ends the block: {costly}");
}

/// `assert` carries the source text **and both sides** (§4.18, and the spec's own
/// line 157). Lowering it without them would guarantee this function is rewritten
/// at M6, which is §1.2's cost formula turned on the compiler's own source.
#[test]
fn assert_carries_its_source_text_and_both_sides() {
    let (program, _, dumped) = lowered(
        "function add(a: int, b: int) -> int\n    return a + b\n\ntest \"it holds\"\n    assert add(a: 2, b: 3) == 5\n",
    );
    assert!(dumped.contains("\"add(a: 2, b: 3) == 5\""), "the text is interned: {dumped}");
    // Three operands: the text, the left value, the right value.
    let mut operands = None;
    for function in &program.functions {
        for block in &function.blocks {
            for inst in &block.insts {
                if let Op::Abort { args, .. } = inst.op {
                    operands = Some(function.args_of(args).len());
                }
            }
        }
    }
    assert_eq!(operands, Some(3), "text, left, right");
}

/// A `test` is a zero-argument function, entered only by `heroes test` (M6). The
/// name is the title, quotes and all: it is a string literal, not an identifier.
#[test]
fn a_test_block_is_a_zero_argument_function() {
    let (program, _, dumped) = lowered("test \"it holds\"\n    assert 1 == 1\n");
    assert_eq!(program.functions.len(), 1);
    assert!(program.functions[0].params.is_empty());
    assert!(dumped.starts_with("strings"), "{dumped}");
    assert!(dumped.contains("test \"it holds\""), "{dumped}");
}

/// `.num _` binds **nothing** (§4.7), so there is no payload read and no slot. This
/// was wrong on the first attempt: a slot named `_` appeared in the dump, holding a
/// type the checker had never given anything.
#[test]
fn a_wildcard_payload_binds_nothing() {
    let dumped = text(
        "variant Token\n    num\n        v: int\n    plus\n\nfunction f(t: Token) -> int\n    return match t\n        .num _ => 0\n        .plus  => 1\n",
    );
    assert!(!dumped.contains("store _ <-"), "{dumped}");
    assert!(!dumped.contains("payload"), "nothing is read: {dumped}");
}

/// A hole lowers to an instruction rather than stopping the pass: §4.16 says a file
/// with `???` type-checks everything else, so it must reach the IR — and M5a is
/// what refuses to emit for it.
#[test]
fn a_hole_lowers_to_an_instruction() {
    let dumped = text("function f() -> int\n    return ???\n");
    assert!(dumped.contains("= ???"), "{dumped}");
}
